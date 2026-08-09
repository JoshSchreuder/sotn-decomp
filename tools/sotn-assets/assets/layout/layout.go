package layout

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/datarange"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/psx"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/sotn"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/util"
	"hash/fnv"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
)

// every layout entry is 10 bytes: X, Y, ID, flags, slot, spawn ID, params
const layoutEntrySize = 10

type layoutEntry struct {
	X       int16  `json:"x"`
	Y       int16  `json:"y"`
	ID      string `json:"id"`
	Flags   uint8  `json:"flags"` // TODO properly de-serialize this
	Slot    uint8  `json:"slot"`
	SpawnID uint8  `json:"spawnId"`
	Params  uint16 `json:"params"`
	YOrder  *int   `json:"yOrder,omitempty"`
}

type layouts struct {
	Entities [][]layoutEntry `json:"entities"`
	Indices  []int           `json:"indices"`
	Dead     []deadRun       `json:"dead,omitempty"`
}

// deadRun is layout data stored between the blocks the room table points at,
// which nothing reads but which still has to be rebuilt byte for byte. Only NO0
// has any, and its X-ordered and Y-ordered copies hold the same runs, so a run
// is recorded once and emitted into both.
type deadRun struct {
	// AfterBlock is the index in Entities of the block this run follows.
	AfterBlock int `json:"afterBlock"`
	// Blocks are complete layout blocks no room points at.
	Blocks [][]layoutEntry `json:"blocks,omitempty"`
	// StrayMarker is a lone block marker with no entries and no terminator.
	StrayMarker bool `json:"strayMarker,omitempty"`
	// Table is a pointer table listing Blocks, stored right after them.
	Table bool `json:"table,omitempty"`
}

// Utility function that finds the index of the given value in the given list.
// Returns -1 (invalid index) if value is not in list.
func indexOf(searchList []int, searchVal int) int {

	for i, value := range searchList {
		if value == searchVal {
			return i
		}
	}
	return -1
}

func fetchEntityIDsFromHeaderFile(overlay string) (map[int]string, error) {
	var path = "src/st"
	if strings.HasPrefix(overlay, "bo") || strings.HasPrefix(overlay, "rbo") ||
		overlay == "mar" {
		path = "src/boss"
	}
	path += "/" + overlay
	return sotn.FetchEnumWithMin(path, overlay, "EntityID", 0x100)
}

func readEntityLayoutEntry(file io.ReadSeeker, ovlName string) (layoutEntry, error) {
	entityIDs, _ := fetchEntityIDsFromHeaderFile(ovlName)

	bs := make([]byte, layoutEntrySize)
	if _, err := io.ReadFull(file, bs); err != nil {
		return layoutEntry{}, err
	}
	return decodeLayoutEntry(bs, entityIDs), nil
}

func decodeLayoutEntry(bs []byte, entityIDs map[int]string) layoutEntry {
	var entityIDStr string
	id := int(bs[4])
	entityIDStr = entityIDs[id]
	if entityIDStr == "" {
		entityIDStr = fmt.Sprintf("0x%02X", id)
	}

	return layoutEntry{
		X:       int16(binary.LittleEndian.Uint16(bs[0:2])),
		Y:       int16(binary.LittleEndian.Uint16(bs[2:4])),
		ID:      entityIDStr,
		Flags:   bs[5],
		Slot:    bs[6],
		SpawnID: bs[7],
		Params:  binary.LittleEndian.Uint16(bs[8:10]),
	}
}

// the Y-ordered entries list has a different order than the X-ordered one. The order cannot consistently get
// restored by just sorting entries by Y as usually entries with the same Y results swapped.
// This algorithm will fill the optional field YOrder, only useful to restore the original order.
func hydrateYOrderFields(x layouts, y layouts) error {
	if len(x.Indices) != len(y.Indices) {
		return fmt.Errorf("number of X and Y layout indices do not match")
	}
	if len(x.Entities) != len(y.Entities) {
		return fmt.Errorf("number of X and Y layout entries do not match")
	}

	populateYOrderField := func(xEntries []layoutEntry, yEntries []layoutEntry) {
		yIndexMap := make(map[layoutEntry]int, len(yEntries))
		for i, e := range yEntries {
			yIndexMap[e] = i
		}
		for i := 0; i < len(xEntries); i++ {
			if yOrder, found := yIndexMap[xEntries[i]]; found {
				xEntries[i].YOrder = &yOrder
			}
		}
	}

	for i := 0; i < len(x.Entities); i++ {
		xList := x.Entities[i]
		yList := y.Entities[i]
		if len(xList) != len(yList) {
			return fmt.Errorf("number of X and Y entries do not match")
		}
		populateYOrderField(xList, yList)
	}
	return nil
}

// NO0 stores layout blocks that no entry of the 53-element table points at, in
// both the X-ordered and Y-ordered copies. They still sit between referenced
// blocks, so extraction has to record them and the range has to cover them, or
// the merge below reports a hole and a rebuild loses the bytes. recoverGaps
// parses each hole once it is satisfied the bytes really are abandoned layout
// data rather than something else.
func recoverGaps(r io.ReadSeeker, ovlName string, baseAddr psx.Addr, ranges []datarange.DataRange) ([]deadRun, []datarange.DataRange, error) {
	known := append([]datarange.DataRange{}, ranges...)
	var runs []deadRun
	var recovered []datarange.DataRange
	for {
		gapBegin, gapEnd, found := firstGap(known)
		if !found {
			return runs, recovered, nil
		}
		run, gap, err := readGap(r, ovlName, baseAddr, gapBegin, gapEnd)
		if err != nil {
			return nil, nil, fmt.Errorf("unable to explain layout data at %s: %w", gapBegin, err)
		}
		known = append(known, gap)
		recovered = append(recovered, gap)
		runs = append(runs, run)
	}
}

// readGap parses the hole [begin, end) as abandoned layout data. It expects
// either a stray block marker, or a run of complete blocks optionally followed
// by a pointer table listing them, and errors out on anything else so a
// genuinely unexplained gap is still reported rather than silently absorbed.
func readGap(r io.ReadSeeker, ovlName string, baseAddr, begin, end psx.Addr) (deadRun, datarange.DataRange, error) {
	fail := func(format string, args ...any) (deadRun, datarange.DataRange, error) {
		return deadRun{}, datarange.DataRange{}, fmt.Errorf(format, args...)
	}
	size := begin.DistanceTo(end)
	if size <= 0 || size%2 != 0 {
		return fail("gap of %d bytes cannot hold layout data", size)
	}
	if err := begin.MoveFile(r, baseAddr); err != nil {
		return deadRun{}, datarange.DataRange{}, err
	}
	entityIDs, _ := fetchEntityIDsFromHeaderFile(ovlName)

	// a stray marker is a block prologue whose terminator belongs to the
	// referenced block that follows it, so the hole is exactly one entry long
	if size == layoutEntrySize {
		bs := make([]byte, layoutEntrySize)
		if _, err := io.ReadFull(r, bs); err != nil {
			return deadRun{}, datarange.DataRange{}, err
		}
		if err := checkBlockMarker(bs, begin); err != nil {
			return deadRun{}, datarange.DataRange{}, err
		}
		return deadRun{StrayMarker: true}, datarange.New(begin, end), nil
	}

	var blockAddrs []psx.Addr
	var blocks [][]layoutEntry
	addr := begin
	for addr < end {
		// once what is left is exactly a table of the blocks already found, the
		// run of blocks is over and the rest of the hole is that table
		if len(blockAddrs) > 0 && addr.Align4().DistanceTo(end) == len(blockAddrs)*4 {
			break
		}
		blockBegin := addr
		var entries []layoutEntry
		terminated := false
		bs := make([]byte, layoutEntrySize)
		for addr < end {
			if _, err := io.ReadFull(r, bs); err != nil {
				return deadRun{}, datarange.DataRange{}, err
			}
			addr = addr.Sum(layoutEntrySize)
			if addr == blockBegin.Sum(layoutEntrySize) {
				if err := checkBlockMarker(bs, blockBegin); err != nil {
					return deadRun{}, datarange.DataRange{}, err
				}
			}
			entry := decodeLayoutEntry(bs, entityIDs)
			entries = append(entries, entry)
			if entry.X == -1 && entry.Y == -1 {
				terminated = true
				break
			}
		}
		if !terminated {
			return fail("layout block at %s is not terminated before %s", blockBegin, end)
		}
		blockAddrs = append(blockAddrs, blockBegin)
		blocks = append(blocks, entries)
	}
	if addr == end {
		return deadRun{Blocks: blocks}, datarange.New(begin, end), nil
	}

	// the blocks stopped short of the hole's end; the tail is a table listing them
	if err := readGapTable(r, baseAddr, addr, end, blockAddrs); err != nil {
		return deadRun{}, datarange.DataRange{}, err
	}
	return deadRun{Blocks: blocks, Table: true}, datarange.New(begin, end), nil
}

// checkBlockMarker requires bs to be the empty X:-2 Y:-2 entry every layout
// block opens with.
func checkBlockMarker(bs []byte, addr psx.Addr) error {
	x := int16(binary.LittleEndian.Uint16(bs[0:2]))
	y := int16(binary.LittleEndian.Uint16(bs[2:4]))
	if x != -2 || y != -2 {
		return fmt.Errorf("data at %s does not begin a layout block: x=%d y=%d", addr, x, y)
	}
	for _, b := range bs[4:] {
		if b != 0 {
			return fmt.Errorf("layout block marker at %s carries entity data", addr)
		}
	}
	return nil
}

// readGapTable checks that [begin, end) is a pointer table naming exactly the
// blocks recovered from the hole, which is what makes them safe to absorb.
func readGapTable(r io.ReadSeeker, baseAddr, begin, end psx.Addr, blockAddrs []psx.Addr) error {
	// the last block is padded to a 4-byte boundary before the table starts
	tableBegin := begin.Align4()
	if tableBegin > end {
		return fmt.Errorf("layout blocks overrun the gap at %s", begin)
	}
	if size := tableBegin.DistanceTo(end); size%4 != 0 || size/4 != len(blockAddrs) {
		return fmt.Errorf("%d trailing bytes at %s are not a table of the %d blocks found",
			tableBegin.DistanceTo(end), tableBegin, len(blockAddrs))
	}
	if err := tableBegin.MoveFile(r, baseAddr); err != nil {
		return err
	}
	table := make([]psx.Addr, len(blockAddrs))
	if err := binary.Read(r, binary.LittleEndian, table); err != nil {
		return err
	}
	for i, addr := range table {
		if addr != blockAddrs[i] {
			return fmt.Errorf("table at %s points at %s, not the block at %s",
				tableBegin, addr, blockAddrs[i])
		}
	}
	return nil
}

// firstGap is the lowest hole between the given ranges, as a (begin, end) pair.
func firstGap(ranges []datarange.DataRange) (psx.Addr, psx.Addr, bool) {
	sorted := append([]datarange.DataRange{}, ranges...)
	sort.Slice(sorted, func(i, j int) bool { return sorted[i].Begin() < sorted[j].Begin() })
	end := psx.RamNull
	for _, cur := range sorted {
		if end != psx.RamNull && end < cur.Begin() {
			return end, cur.Begin(), true
		}
		if cur.End() > end {
			end = cur.End()
		}
	}
	return psx.RamNull, psx.RamNull, false
}

// blockEndingAt is the index of the referenced block that ends where a dead run
// begins. The run's position has to be expressed relative to a block, since the
// addresses themselves are not part of the extracted JSON.
func blockEndingAt(pool map[psx.Addr]int, ranges []datarange.DataRange, addr psx.Addr) (int, error) {
	for _, r := range ranges {
		if r.End() == addr {
			if i, ok := pool[r.Begin()]; ok {
				return i, nil
			}
		}
	}
	return 0, fmt.Errorf("layout data at %s does not follow a known block", addr)
}

func readEntityLayout(r io.ReadSeeker, ovlName string, off, baseAddr psx.Addr, count int, isX bool) (layouts, []datarange.DataRange, error) {
	if err := off.MoveFile(r, baseAddr); err != nil {
		return layouts{}, nil, err
	}

	// there are two copies of the layout, one ordered by X and the other one ordered by Y
	// we will only read the first one, which is ordered by Y
	blockOffsets := make([]psx.Addr, count)
	if err := binary.Read(r, binary.LittleEndian, blockOffsets); err != nil {
		return layouts{}, nil, err
	}

	// the order of each layout entry must be preserved
	pool := map[psx.Addr]int{}
	var blocks [][]layoutEntry
	var xRanges []datarange.DataRange
	for _, blockOffset := range util.SortUniqueOffsets(blockOffsets) {
		if err := blockOffset.MoveFile(r, baseAddr); err != nil {
			return layouts{}, nil, err
		}
		var entries []layoutEntry
		for {
			entry, err := readEntityLayoutEntry(r, ovlName)
			if err != nil {
				return layouts{}, nil, err
			}
			if entry.X == -1 && entry.Y == -1 {
				entries = append(entries, entry)
				break
			}
			entries = append(entries, entry)
		}

		// sanity check on the first entry
		if entries[0].X != -2 || entries[0].Y != -2 {
			err := fmt.Errorf("first layout entry does not mark the beginning of the array: %v", entries[0])
			return layouts{}, nil, err
		}

		pool[blockOffset] = len(blocks)
		blocks = append(blocks, entries)
		xRanges = append(xRanges, datarange.FromAddr(blockOffset, len(entries)*10))
	}
	// the very last entry needs to be aligned by 4
	xRanges[len(xRanges)-1] = xRanges[len(xRanges)-1].Align4()

	// blocks the table does not point at still sit between the ones it does
	deadRuns, gaps, err := recoverGaps(r, ovlName, baseAddr, xRanges)
	if err != nil {
		return layouts{}, nil, err
	}
	for i, gap := range gaps {
		// anchor the run to the block it follows, the only thing that survives
		// into the JSON, so the builder can put it back in the same place
		after, err := blockEndingAt(pool, xRanges, gap.Begin())
		if err != nil {
			return layouts{}, nil, err
		}
		deadRuns[i].AfterBlock = after
	}
	xRanges = append(xRanges, gaps...)
	sort.Slice(deadRuns, func(i, j int) bool { return deadRuns[i].AfterBlock < deadRuns[j].AfterBlock })

	l := layouts{Entities: blocks, Dead: deadRuns}
	for _, blockOffset := range blockOffsets {
		l.Indices = append(l.Indices, pool[blockOffset])
	}

	endOfArray := off.Sum(count * 4)
	if isX { // we want to do the same thing with the vertically aligned layout
		yLayouts, yRanges, err := readEntityLayout(r, ovlName, endOfArray, baseAddr, count, false)
		if err != nil {
			return layouts{}, nil, fmt.Errorf("readEntityLayout failed on Y: %w", err)
		}
		if err := hydrateYOrderFields(l, yLayouts); err != nil {
			return layouts{}, nil, fmt.Errorf("unable to populate YOrder field: %w", err)
		}
		xMerged := datarange.MergeDataRanges(xRanges)
		yMerged := yRanges[1]
		return l, []datarange.DataRange{
			datarange.MergeDataRanges([]datarange.DataRange{datarange.New(off, endOfArray), yRanges[0]}),
			datarange.MergeDataRanges([]datarange.DataRange{xMerged, yMerged}),
		}, nil
	} else {
		return l, []datarange.DataRange{datarange.New(off, endOfArray), datarange.MergeDataRanges(xRanges)}, nil
	}
}

// layItem is one piece of a layout copy as it is stored in memory: a run of
// entries, or a table of pointers to the runs no room points at.
type layItem struct {
	entries  []layoutEntry
	blockIdx int   // index into layouts.Entities, or -1 for data no room points at
	stray    bool  // a block marker with no entries and no terminator
	table    []int // when set, this item is a table pointing at these plan items
}

// planLayout is the order one layout copy is stored in, including the blocks and
// pointer table no room points at, which still have to be rebuilt in place.
func planLayout(el layouts) ([]layItem, error) {
	deadAfter := map[int][]deadRun{}
	for _, run := range el.Dead {
		if run.AfterBlock < 0 || run.AfterBlock >= len(el.Entities) {
			return nil, fmt.Errorf(
				"unused layout data follows block %d, which does not exist", run.AfterBlock)
		}
		deadAfter[run.AfterBlock] = append(deadAfter[run.AfterBlock], run)
	}

	var plan []layItem
	for i, entries := range el.Entities {
		plan = append(plan, layItem{entries: entries, blockIdx: i})
		for _, run := range deadAfter[i] {
			var targets []int
			for _, block := range run.Blocks {
				targets = append(targets, len(plan))
				plan = append(plan, layItem{entries: block, blockIdx: -1})
			}
			if run.StrayMarker {
				plan = append(plan, layItem{blockIdx: -1, stray: true})
			}
			if run.Table {
				if len(targets) == 0 {
					return nil, fmt.Errorf("unused layout table after block %d lists no blocks", i)
				}
				plan = append(plan, layItem{blockIdx: -1, table: targets})
			}
		}
	}
	return plan, nil
}

// laySeg is one generated C array. A pointer table cannot live inside the u16
// array holding the entries, so it becomes an array of its own and the entries
// around it are split in two.
type laySeg struct {
	items []int // indices into the plan
	table bool
}

func segmentPlan(plan []layItem) []laySeg {
	var segs []laySeg
	for i, item := range plan {
		if item.table != nil {
			segs = append(segs, laySeg{items: []int{i}, table: true})
			continue
		}
		if len(segs) == 0 || segs[len(segs)-1].table {
			segs = append(segs, laySeg{})
		}
		last := len(segs) - 1
		segs[last].items = append(segs[last].items, i)
	}
	return segs
}

// layPos is where a plan item ended up: which generated array, and how many
// entries into it. Both the room table and the unused pointer table are written
// in terms of these.
type layPos struct {
	seg   int
	entry int
}

func placePlan(plan []layItem, segs []laySeg) []layPos {
	pos := make([]layPos, len(plan))
	for segIdx, seg := range segs {
		entry := 0
		for _, i := range seg.items {
			pos[i] = layPos{seg: segIdx, entry: entry}
			entry += len(plan[i].entries)
			if plan[i].stray {
				entry++
			}
		}
	}
	return pos
}

// segNames is the symbol for each generated array. The first array of entries
// keeps the bare name so overlays with nothing unused generate as before.
func segNames(symbolName, suffix string, segs []laySeg) []string {
	names := make([]string, len(segs))
	entriesSeen, tablesSeen := 0, 0
	for i, seg := range segs {
		switch {
		case seg.table:
			names[i] = fmt.Sprintf("%s_%s_unused_%d", symbolName, suffix, tablesSeen)
			tablesSeen++
		case entriesSeen == 0:
			names[i] = fmt.Sprintf("%s_%s", symbolName, suffix)
			entriesSeen++
		default:
			names[i] = fmt.Sprintf("%s_%s_%d", symbolName, suffix, entriesSeen)
			entriesSeen++
		}
	}
	return names
}

func buildEntityLayouts(fileName, outputDir, subDir string, ovlName string) error {
	data, err := os.ReadFile(fileName)
	if err != nil {
		return err
	}

	var el layouts
	if err := json.Unmarshal(data, &el); err != nil {
		return err
	}

	h := fnv.New32()
	_, _ = h.Write([]byte(outputDir))
	symbolVariant := strconv.FormatUint(uint64(h.Sum32()), 16)
	symbolName := fmt.Sprintf("entity_layout_%s", symbolVariant)

	makeSortedBanks := func(banks [][]layoutEntry, sortByX bool) [][]layoutEntry {
		var toSort []layoutEntry
		var less func(i, j int) bool
		if sortByX {
			less = func(i, j int) bool {
				return toSort[i].X < toSort[j].X
			}
		} else {
			less = func(i, j int) bool {
				if toSort[i].Y < toSort[j].Y {
					return true
				}
				if toSort[i].Y > toSort[j].Y {
					return false
				}
				if toSort[i].YOrder != nil && toSort[j].YOrder != nil {
					return *toSort[i].YOrder < *toSort[j].YOrder
				}
				return i < j
			}
		}
		sorting := make([][]layoutEntry, len(banks))
		for i, entries := range banks {
			sorting[i] = make([]layoutEntry, len(entries)-2)
			if len(sorting[i]) > 0 { // do not sort if the list is empty
				copy(sorting[i], entries[1:len(entries)-1]) // do not sort the -2 and -1 entries
				toSort = sorting[i]
				sort.SliceStable(toSort, less)
			}

			// put back the -2 and -1
			sorting[i] = append([]layoutEntry{entries[0]}, sorting[i]...)
			sorting[i] = append(sorting[i], entries[len(entries)-1])
		}
		return sorting
	}
	writeEntry := func(sb *strings.Builder, e layoutEntry) {
		sb.WriteString(fmt.Sprintf("    0x%04X, 0x%04X, %s | 0x%04X, 0x%04X, 0x%04X,\n",
			uint16(e.X), uint16(e.Y), e.ID, int(e.Flags)<<8, int(e.Slot)|(int(e.SpawnID)<<8), e.Params))
	}
	// writeLayoutCopy emits every array making up one ordering of the layout. It
	// is usually a single u16 array, but a pointer table among the entries has to
	// be an array of its own, splitting the entries around it.
	writeLayoutCopy := func(sb *strings.Builder, el layouts, plan []layItem, suffix string, sortByX bool) error {
		sorted := make([]layItem, len(plan))
		copy(sorted, plan)
		// sort every block, including the ones no room points at
		var toSort [][]layoutEntry
		for _, item := range plan {
			if item.table == nil && !item.stray {
				toSort = append(toSort, item.entries)
			}
		}
		banks := makeSortedBanks(toSort, sortByX)
		at := 0
		for i, item := range plan {
			if item.table == nil && !item.stray {
				sorted[i].entries = banks[at]
				at++
			}
		}

		segs := segmentPlan(sorted)
		pos := placePlan(sorted, segs)
		names := segNames(symbolName, suffix, segs)
		nWritten := 0
		for segIdx, seg := range segs {
			if seg.table {
				item := sorted[seg.items[0]]
				sb.WriteString(fmt.Sprintf("LayoutEntity* %s[] = {\n", names[segIdx]))
				for _, target := range item.table {
					p := pos[target]
					sb.WriteString(fmt.Sprintf("    (LayoutEntity*)&%s[%d],\n", names[p.seg], p.entry*5))
				}
				sb.WriteString("};\n")
				continue
			}
			sb.WriteString(fmt.Sprintf("u16 %s[] = {\n", names[segIdx]))
			for _, i := range seg.items {
				item := sorted[i]
				if item.stray {
					// a block marker whose terminator belongs to the block after it
					sb.WriteString("// Unused block marker\n")
					writeEntry(sb, layoutEntry{X: -2, Y: -2, ID: "E_NONE"})
					nWritten++
					continue
				}
				entries := item.entries
				// do a sanity check on the entries as we do not want to build something that will cause the game to crash
				if entries[0].X != -2 || entries[0].Y != -2 {
					return fmt.Errorf("layout entity bank %d needs to have a X:-2 and Y:-2 entry at the beginning", i)
				}
				lastEntry := entries[len(entries)-1]
				if lastEntry.X != -1 || lastEntry.Y != -1 {
					return fmt.Errorf("layout entity bank %d needs to have a X:-1 and Y:-1 entry at the end", i)
				}
				switch roomNum := indexOf(el.Indices, item.blockIdx); {
				case item.blockIdx < 0:
					sb.WriteString(fmt.Sprintf("// Offset %d, Unused\n", nWritten))
				case roomNum < 0:
					sb.WriteString(fmt.Sprintf("// Offset %d, No Room Found\n", nWritten))
				default:
					sb.WriteString(fmt.Sprintf("// Offset %d, Room 0x%02X\n", nWritten, roomNum)) //label each block with offsets
				}
				for _, e := range entries {
					writeEntry(sb, e)
				}
				nWritten += len(entries)
			}
			// a table that follows must start 4-byte aligned, and the Y copy as a
			// whole is padded to a word so what comes after it stays aligned
			nextIsTable := segIdx+1 < len(segs) && segs[segIdx+1].table
			if (nextIsTable || (!sortByX && segIdx == len(segs)-1)) && nWritten%2 != 0 {
				sb.WriteString("    0, // padding\n")
				nWritten++
			}
			sb.WriteString("};\n")
		}
		return nil
	}

	plan, err := planLayout(el)
	if err != nil {
		return err
	}
	segs := segmentPlan(plan)
	pos := placePlan(plan, segs)
	// where each room's block landed, which the unused blocks between them shift
	blockPos := make([]layPos, len(el.Entities))
	for i, item := range plan {
		if item.blockIdx >= 0 {
			blockPos[item.blockIdx] = pos[i]
		}
	}

	ovlHeaderLoc := fmt.Sprintf("../%s.h", ovlName)
	if subDir != "" {
		// Look back further if in version specific subdirectory
		ovlHeaderLoc = "../" + ovlHeaderLoc
	}

	laydefFile := strings.Builder{}
	laydefFile.WriteString("#include <stage.h>\n\n")
	laydefFile.WriteString("#include \"common.h\"\n\n")
	laydefFile.WriteString("// clang-format off\n")
	writeLaydef := func(suffix, arrayName string) {
		names := segNames(symbolName, suffix, segs)
		declared := map[string]bool{}
		for _, i := range el.Indices {
			if name := names[blockPos[i].seg]; !declared[name] {
				laydefFile.WriteString(fmt.Sprintf("extern LayoutEntity %s[];\n", name))
				declared[name] = true
			}
		}
		laydefFile.WriteString(fmt.Sprintf("LayoutEntity* %s[] = {\n", arrayName))
		for _, i := range el.Indices {
			laydefFile.WriteString(fmt.Sprintf("    &%s[%d],\n", names[blockPos[i].seg], blockPos[i].entry))
		}
		laydefFile.WriteString("};\n")
	}
	writeLaydef("x", "entityLayoutHorizontal")
	writeLaydef("y", "entityLayoutVertical")

	layoutFile := strings.Builder{}
	layoutFile.WriteString(fmt.Sprintf("#include \"%s\"\n\n", ovlHeaderLoc))
	layoutFile.WriteString("// clang-format off\n")
	if err := writeLayoutCopy(&layoutFile, el, plan, "x", true); err != nil {
		return fmt.Errorf("unable to build X entity layout: %w", err)
	}
	if err := writeLayoutCopy(&layoutFile, el, plan, "y", false); err != nil {
		return fmt.Errorf("unable to build Y entity layout: %w", err)
	}

	if err := util.WriteFile(filepath.Join(outputDir, "gen", subDir, "e_layout.c"), []byte(layoutFile.String())); err != nil {
		return err
	}
	return util.WriteFile(filepath.Join(outputDir, "gen", subDir, "e_laydef.c"), []byte(laydefFile.String()))
}
