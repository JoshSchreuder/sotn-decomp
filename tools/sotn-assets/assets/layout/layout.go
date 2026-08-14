package layout

import (
	"bytes"
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
	"reflect"
	"sort"
	"strconv"
	"strings"
)

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

// deadPointers is a table of pointers to entity layout blocks that sits in
// between them and that nothing in the game refers to. It still is part of the
// overlay, so it has to be reproduced to keep the build byte matching, and it
// forces the generated array to be split in two: a pointer cannot be spelled out
// in the middle of an array of u16.
type deadPointers struct {
	// AfterBlock is the index into layouts.Entities the table follows.
	AfterBlock int `json:"afterBlock"`
	// Blocks are the indices into layouts.Entities the table points at.
	Blocks []int `json:"blocks"`
}

type layouts struct {
	Entities     [][]layoutEntry `json:"entities"`
	Indices      []int           `json:"indices"`
	DeadPointers []deadPointers  `json:"deadPointers,omitempty"`
}

const (
	// size of a single LayoutEntity, in bytes and in u16 words
	layoutEntrySize  = 10
	layoutEntryWords = layoutEntrySize / 2
)

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

func readEntityLayoutEntry(file io.Reader, ovlName string) (layoutEntry, error) {
	entityIDs, _ := fetchEntityIDsFromHeaderFile(ovlName)

	bs := make([]byte, layoutEntrySize)
	if _, err := io.ReadFull(file, bs); err != nil {
		return layoutEntry{}, err
	}

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
	}, nil
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

// readEntityLayoutBlock reads the entries of a single block, up to the -1 entry
// that terminates it. A truncated block instead ends where its data does: the
// blocks no room refers to can lack a terminator, and the game never walks them
// looking for one that is not there.
func readEntityLayoutBlock(r io.Reader, ovlName string, truncated bool) ([]layoutEntry, error) {
	var entries []layoutEntry
	for {
		entry, err := readEntityLayoutEntry(r, ovlName)
		if err == io.EOF && truncated && len(entries) > 0 {
			break
		}
		if err != nil {
			return nil, err
		}
		entries = append(entries, entry)
		if entry.X == -1 && entry.Y == -1 {
			break
		}
	}
	// sanity check on the first entry
	if entries[0].X != -2 || entries[0].Y != -2 {
		return nil, fmt.Errorf(
			"first layout entry does not mark the beginning of the array: %v", entries[0])
	}
	return entries, nil
}

// decodePointerTable reports whether the whole of data is an array of pointers,
// each one aimed at the beginning of one of the given layout blocks.
func decodePointerTable(data []byte, blocks map[psx.Addr]int) ([]psx.Addr, bool) {
	if len(data) == 0 || len(data)%4 != 0 {
		return nil, false
	}
	targets := make([]psx.Addr, 0, len(data)/4)
	for i := 0; i < len(data); i += 4 {
		target := psx.GetAddr(data[i : i+4])
		if _, ok := blocks[target]; !ok {
			return nil, false
		}
		targets = append(targets, target)
	}
	return targets, true
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
	var l layouts
	var xRanges []datarange.DataRange
	appendBlock := func(addr psx.Addr, entries []layoutEntry) {
		pool[addr] = len(l.Entities)
		l.Entities = append(l.Entities, entries)
	}

	// claimDeadData makes sense of the bytes sitting in between two blocks the
	// layout table refers to. They hold more blocks, which no room ever reaches,
	// optionally followed by a table of pointers to them. It is all dead, but
	// still part of the overlay, so it has to be claimed for the build to keep
	// matching. A gap that cannot be made sense of is rolled back and left
	// alone, so that Info keeps reporting it.
	claimDeadData := func(begin, end psx.Addr) error {
		if err := begin.MoveFile(r, baseAddr); err != nil {
			return err
		}
		buf := make([]byte, begin.DistanceTo(end))
		if _, err := io.ReadFull(r, buf); err != nil {
			return err
		}

		var claimed []psx.Addr
		rollback := func() {
			l.Entities = l.Entities[:len(l.Entities)-len(claimed)]
			for _, addr := range claimed {
				delete(pool, addr)
			}
		}
		gap := bytes.NewReader(buf)
		for pos := 0; pos < len(buf); {
			// a pointer table can only be told apart from entry data by the fact
			// that every one of the remaining words points at a block
			if targets, ok := decodePointerTable(buf[pos:], pool); ok {
				dead := deadPointers{AfterBlock: len(l.Entities) - 1}
				for _, target := range targets {
					dead.Blocks = append(dead.Blocks, pool[target])
				}
				l.DeadPointers = append(l.DeadPointers, dead)
				return nil
			}
			addr := begin.Sum(pos)
			entries, err := readEntityLayoutBlock(gap, ovlName, true)
			if err != nil {
				rollback()
				return err
			}
			appendBlock(addr, entries)
			claimed = append(claimed, addr)
			pos += len(entries) * layoutEntrySize
		}
		return nil
	}

	sorted := util.SortUniqueOffsets(blockOffsets)
	for i, blockOffset := range sorted {
		if err := blockOffset.MoveFile(r, baseAddr); err != nil {
			return layouts{}, nil, err
		}
		entries, err := readEntityLayoutBlock(r, ovlName, false)
		if err != nil {
			return layouts{}, nil, err
		}
		appendBlock(blockOffset, entries)

		blockEnd := blockOffset.Sum(len(entries) * layoutEntrySize)
		xRanges = append(xRanges, datarange.New(blockOffset, blockEnd))
		if i+1 == len(sorted) || blockEnd >= sorted[i+1] {
			continue
		}
		if err := claimDeadData(blockEnd, sorted[i+1]); err != nil {
			_, _ = fmt.Fprintf(os.Stderr,
				"WARNING: leaving the %d dead bytes at %s alone: %v\n",
				blockEnd.DistanceTo(sorted[i+1]), blockEnd, err)
			continue
		}
		xRanges = append(xRanges, datarange.New(blockEnd, sorted[i+1]))
	}
	// the very last entry needs to be aligned by 4
	xRanges[len(xRanges)-1] = xRanges[len(xRanges)-1].Align4()

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
		// the dead pointers are stored once and emitted for both axes, so the
		// two copies have to agree
		if !reflect.DeepEqual(l.DeadPointers, yLayouts.DeadPointers) {
			return layouts{}, nil, fmt.Errorf(
				"the X and Y copies of the dead layout pointers differ")
		}
		// Keep block ranges separate here. Info consolidates adjacent runs while
		// leaving gaps unclaimed; Extract does not consume the ranges.
		laydefRange, err := datarange.Merge([]datarange.DataRange{
			datarange.New(off, endOfArray), yRanges[0]})
		if err != nil {
			return layouts{}, nil, err
		}
		layoutRanges := append(
			[]datarange.DataRange{laydefRange}, xRanges...)
		return l, append(layoutRanges, yRanges[1:]...), nil
	} else {
		return l, append(
			[]datarange.DataRange{datarange.New(off, endOfArray)},
			xRanges...), nil
	}
}

// layoutPlan says where every entity layout block ends up once the data of one
// axis is written out. Both axes hold the same blocks in the same order, only
// their entries are sorted differently, so a single plan describes either of
// them.
type layoutPlan struct {
	arrayOf    []int         // per block, the array it belongs to
	entryOf    []int         // per block, its first entry within that array
	tableAfter map[int][]int // per block, the dead pointer table following it
}

func planLayout(el layouts) (layoutPlan, error) {
	plan := layoutPlan{
		arrayOf:    make([]int, len(el.Entities)),
		entryOf:    make([]int, len(el.Entities)),
		tableAfter: map[int][]int{},
	}
	for _, dead := range el.DeadPointers {
		if dead.AfterBlock < 0 || dead.AfterBlock >= len(el.Entities) {
			return plan, fmt.Errorf(
				"a dead pointer table follows the out of range block %d", dead.AfterBlock)
		}
		if _, taken := plan.tableAfter[dead.AfterBlock]; taken {
			return plan, fmt.Errorf(
				"two dead pointer tables follow block %d", dead.AfterBlock)
		}
		plan.tableAfter[dead.AfterBlock] = dead.Blocks
	}

	array, entries := 0, 0 // the array being filled and how much of it is planned
	for i := range el.Entities {
		plan.arrayOf[i] = array
		plan.entryOf[i] = entries
		entries += len(el.Entities[i])
		if _, ok := plan.tableAfter[i]; !ok {
			continue
		}
		// the compiler aligns every array by 4, so an array cut short of a
		// multiple of 4 would get padded and shift the data after the table
		if entries*layoutEntrySize%4 != 0 {
			return plan, fmt.Errorf(
				"the %d entries before the dead pointer table of block %d do not fill a multiple of 4 bytes",
				entries, i)
		}
		array++
		entries = 0
	}
	return plan, nil
}

// arrayName names the nth array of an axis. The first one keeps the bare name,
// so that the overlays needing no split at all are generated as they always were.
func arrayName(symbolName, axis string, n int) string {
	if n == 0 {
		return fmt.Sprintf("%s_%s", symbolName, axis)
	}
	return fmt.Sprintf("%s_%s_%d", symbolName, axis, n+1)
}

func buildEntityLayouts(fileName, outputDir, subDir string, ovlName string) error {
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
			if len(entries) < 3 { // nothing to sort in between the -2 and the -1
				sorting[i] = entries
				continue
			}
			sorting[i] = make([]layoutEntry, len(entries)-2)
			copy(sorting[i], entries[1:len(entries)-1]) // do not sort the -2 and -1 entries
			toSort = sorting[i]
			sort.SliceStable(toSort, less)

			// put back the -2 and -1
			sorting[i] = append([]layoutEntry{entries[0]}, sorting[i]...)
			sorting[i] = append(sorting[i], entries[len(entries)-1])
		}
		return sorting
	}

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

	plan, err := planLayout(el)
	if err != nil {
		return err
	}

	writeLayoutEntries := func(sb *strings.Builder, axis string) error {
		sortByX := axis == "x"
		banks := makeSortedBanks(el.Entities, sortByX)
		nWritten := 0
		sb.WriteString(fmt.Sprintf("u16 %s[] = {\n", arrayName(symbolName, axis, 0)))
		for i, entries := range banks {
			// do a sanity check on the entries as we do not want to build something that will cause the game to crash
			if entries[0].X != -2 || entries[0].Y != -2 {
				return fmt.Errorf("layout entity bank %d needs to have a X:-2 and Y:-2 entry at the beginning", i)
			}
			roomNum := indexOf(el.Indices, i)
			lastEntry := entries[len(entries)-1]
			if (lastEntry.X != -1 || lastEntry.Y != -1) && roomNum >= 0 {
				// a bank the game never walks is allowed to be truncated, as it
				// will not go looking for a terminator that is not there
				return fmt.Errorf("layout entity bank %d needs to have a X:-1 and Y:-1 entry at the end", i)
			}
			if roomNum < 0 {
				sb.WriteString(fmt.Sprintf("// Offset %d, No Room Found\n", plan.entryOf[i]))
			} else {
				sb.WriteString(fmt.Sprintf("// Offset %d, Room 0x%02X\n", plan.entryOf[i], roomNum)) //label each block with offsets
			}
			for _, e := range entries {
				sb.WriteString(fmt.Sprintf("    0x%04X, 0x%04X, %s | 0x%04X, 0x%04X, 0x%04X,\n",
					uint16(e.X), uint16(e.Y), e.ID, int(e.Flags)<<8, int(e.Slot)|(int(e.SpawnID)<<8), e.Params))
			}
			nWritten += len(entries)

			targets, ok := plan.tableAfter[i]
			if !ok {
				continue
			}
			// the array has to be closed so that the dead pointers can be
			// spelled out as relocations instead of as hardcoded addresses
			sb.WriteString("};\n")
			sb.WriteString(fmt.Sprintf("LayoutEntity* %s_dead_ptrs[] = {\n",
				arrayName(symbolName, axis, plan.arrayOf[i])))
			for _, target := range targets {
				sb.WriteString(fmt.Sprintf("    (LayoutEntity*)&%s[%d],\n",
					arrayName(symbolName, axis, plan.arrayOf[target]),
					plan.entryOf[target]*layoutEntryWords))
			}
			sb.WriteString("};\n")
			sb.WriteString(fmt.Sprintf("u16 %s[] = {\n",
				arrayName(symbolName, axis, plan.arrayOf[i]+1)))
		}
		if !sortByX && nWritten%2 != 0 {
			sb.WriteString("    0, // padding\n")
		}
		sb.WriteString("};\n")
		return nil
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
	for _, axis := range []struct{ name, table string }{
		{"x", "entityLayoutHorizontal"},
		{"y", "entityLayoutVertical"},
	} {
		for n := 0; n <= len(el.DeadPointers); n++ {
			laydefFile.WriteString(fmt.Sprintf("extern LayoutEntity %s[];\n",
				arrayName(symbolName, axis.name, n)))
		}
		laydefFile.WriteString(fmt.Sprintf("LayoutEntity* %s[] = {\n", axis.table))
		for _, i := range el.Indices {
			laydefFile.WriteString(fmt.Sprintf("    &%s[%d],\n",
				arrayName(symbolName, axis.name, plan.arrayOf[i]), plan.entryOf[i]))
		}
		laydefFile.WriteString("};\n")
	}

	layoutFile := strings.Builder{}
	layoutFile.WriteString(fmt.Sprintf("#include \"%s\"\n\n", ovlHeaderLoc))
	layoutFile.WriteString("// clang-format off\n")
	if err := writeLayoutEntries(&layoutFile, "x"); err != nil {
		return fmt.Errorf("unable to build X entity layout: %w", err)
	}
	if err := writeLayoutEntries(&layoutFile, "y"); err != nil {
		return fmt.Errorf("unable to build Y entity layout: %w", err)
	}

	if err := util.WriteFile(filepath.Join(outputDir, "gen", subDir, "e_layout.c"), []byte(layoutFile.String())); err != nil {
		return err
	}
	return util.WriteFile(filepath.Join(outputDir, "gen", subDir, "e_laydef.c"), []byte(laydefFile.String()))
}
