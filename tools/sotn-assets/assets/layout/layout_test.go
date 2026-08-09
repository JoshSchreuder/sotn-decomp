package layout

import (
	"bytes"
	"encoding/binary"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/psx"
)

const testBase = psx.Addr(0x80180000)

// entry is one 10-byte layout entry, only the coordinates matter here.
func entry(x, y int16) []byte {
	bs := make([]byte, layoutEntrySize)
	binary.LittleEndian.PutUint16(bs[0:2], uint16(x))
	binary.LittleEndian.PutUint16(bs[2:4], uint16(y))
	return bs
}

// block is a complete layout block: a marker, n filler entries, a terminator.
func block(n int) []byte {
	bs := entry(-2, -2)
	for i := 0; i < n; i++ {
		bs = append(bs, entry(int16(i), int16(i))...)
	}
	return append(bs, entry(-1, -1)...)
}

func table(addrs ...psx.Addr) []byte {
	bs := make([]byte, 0, len(addrs)*4)
	for _, a := range addrs {
		bs = binary.LittleEndian.AppendUint32(bs, uint32(a))
	}
	return bs
}

func readTestGap(data []byte) (deadRun, psx.Addr, error) {
	r := bytes.NewReader(data)
	run, gap, err := readGap(r, "", testBase, testBase, testBase.Sum(len(data)))
	return run, gap.End(), err
}

func TestReadGap(t *testing.T) {
	t.Run("a run of blocks followed by their own pointer table", func(t *testing.T) {
		// the shape NO0 has: three unreferenced blocks and a table listing them,
		// with the blocks ending on the 4-byte boundary the table starts at
		data := block(4)
		second := testBase.Sum(len(data))
		data = append(data, block(3)...)
		third := testBase.Sum(len(data))
		data = append(data, block(3)...)
		require.Zero(t, len(data)%4, "the table must start 4-byte aligned")
		data = append(data, table(testBase, second, third)...)

		run, end, err := readTestGap(data)
		require.NoError(t, err)
		assert.Equal(t, testBase.Sum(len(data)), end)
		assert.True(t, run.Table)
		assert.False(t, run.StrayMarker)
		require.Len(t, run.Blocks, 3)
		assert.Len(t, run.Blocks[0], 6) // marker + 4 entries + terminator
		assert.Len(t, run.Blocks[1], 5)
	})

	t.Run("a pointer table padded to alignment", func(t *testing.T) {
		data := block(4)
		second := testBase.Sum(len(data))
		data = append(data, block(3)...)
		require.NotZero(t, len(data)%4, "this case is about reaching the boundary")
		data = append(data, 0, 0) // padding up to the table
		data = append(data, table(testBase, second)...)

		run, end, err := readTestGap(data)
		require.NoError(t, err)
		assert.Equal(t, testBase.Sum(len(data)), end)
		assert.Len(t, run.Blocks, 2)
	})

	t.Run("a run of blocks with no pointer table", func(t *testing.T) {
		data := append(block(2), block(1)...)
		run, end, err := readTestGap(data)
		require.NoError(t, err)
		assert.Equal(t, testBase.Sum(len(data)), end)
		assert.False(t, run.Table)
		assert.Len(t, run.Blocks, 2)
	})

	t.Run("a stray block marker", func(t *testing.T) {
		// the other shape NO0 has: a marker whose terminator belongs to the
		// referenced block that follows it, so the hole is one entry long
		run, end, err := readTestGap(entry(-2, -2))
		require.NoError(t, err)
		assert.Equal(t, testBase.Sum(layoutEntrySize), end)
		assert.True(t, run.StrayMarker)
		assert.Empty(t, run.Blocks)
	})

	t.Run("rejects a stray marker carrying entity data", func(t *testing.T) {
		data := entry(-2, -2)
		data[4] = 0x01 // an entity id in what must be an empty marker
		_, _, err := readTestGap(data)
		assert.Error(t, err)
	})

	t.Run("rejects a gap not opened by a block marker", func(t *testing.T) {
		_, _, err := readTestGap(append(entry(0, 0), entry(-1, -1)...))
		assert.Error(t, err)
	})

	t.Run("rejects a block that runs past the gap", func(t *testing.T) {
		// a marker and filler, but the terminator lives outside the gap
		data := append(entry(-2, -2), entry(1, 1)...)
		r := bytes.NewReader(append(data, entry(-1, -1)...))
		_, _, err := readGap(r, "", testBase, testBase, testBase.Sum(len(data)))
		assert.Error(t, err)
	})

	t.Run("rejects a trailing table that does not list the blocks", func(t *testing.T) {
		data := append(block(4), table(testBase.Sum(4))...)
		_, _, err := readTestGap(data)
		assert.Error(t, err)
	})
}

// mkBlock is a layout block of n real entries, bracketed by the markers.
func mkBlock(n int) []layoutEntry {
	entries := []layoutEntry{{X: -2, Y: -2, ID: "E_NONE"}}
	for i := 0; i < n; i++ {
		entries = append(entries, layoutEntry{X: int16(i * 16), Y: int16(i), ID: "E_NONE"})
	}
	return append(entries, layoutEntry{X: -1, Y: -1, ID: "E_NONE"})
}

func TestPlanLayout(t *testing.T) {
	t.Run("without unused data the plan is just the blocks", func(t *testing.T) {
		el := layouts{Entities: [][]layoutEntry{mkBlock(1), mkBlock(2)}}
		plan, err := planLayout(el)
		require.NoError(t, err)
		require.Len(t, plan, 2)
		assert.Equal(t, 0, plan[0].blockIdx)
		assert.Equal(t, 1, plan[1].blockIdx)
		// one array, so the entries keep the bare symbol name
		segs := segmentPlan(plan)
		require.Len(t, segs, 1)
		assert.Equal(t, []string{"sym_x"}, segNames("sym", "x", segs))
	})

	t.Run("unused blocks and their table split the array", func(t *testing.T) {
		el := layouts{
			Entities: [][]layoutEntry{mkBlock(1), mkBlock(1)},
			Dead: []deadRun{{
				AfterBlock: 0,
				Blocks:     [][]layoutEntry{mkBlock(2)},
				Table:      true,
			}},
		}
		plan, err := planLayout(el)
		require.NoError(t, err)
		require.Len(t, plan, 4) // block, dead block, table, block
		assert.Equal(t, -1, plan[1].blockIdx)
		assert.Equal(t, []int{1}, plan[2].table)

		segs := segmentPlan(plan)
		require.Len(t, segs, 3)
		assert.False(t, segs[0].table)
		assert.True(t, segs[1].table)
		assert.Equal(t, []string{"sym_x", "sym_x_unused_0", "sym_x_1"},
			segNames("sym", "x", segs))

		// the table points at the dead block's offset in u16 units
		pos := placePlan(plan, segs)
		assert.Equal(t, layPos{seg: 0, entry: 3}, pos[1])
		// the room block after the table restarts the entry count
		assert.Equal(t, layPos{seg: 2, entry: 0}, pos[3])
	})

	t.Run("a stray marker occupies one entry", func(t *testing.T) {
		el := layouts{
			Entities: [][]layoutEntry{mkBlock(1), mkBlock(1)},
			Dead:     []deadRun{{AfterBlock: 0, StrayMarker: true}},
		}
		plan, err := planLayout(el)
		require.NoError(t, err)
		require.Len(t, plan, 3)
		assert.True(t, plan[1].stray)

		segs := segmentPlan(plan)
		require.Len(t, segs, 1, "a stray marker does not split the array")
		pos := placePlan(plan, segs)
		// block 0 is 3 entries, the marker is 1, so the next block starts at 4
		assert.Equal(t, layPos{seg: 0, entry: 4}, pos[2])
	})

	t.Run("rejects unused data anchored to a block that does not exist", func(t *testing.T) {
		el := layouts{
			Entities: [][]layoutEntry{mkBlock(1)},
			Dead:     []deadRun{{AfterBlock: 7, StrayMarker: true}},
		}
		_, err := planLayout(el)
		assert.Error(t, err)
	})

	t.Run("rejects a table listing no blocks", func(t *testing.T) {
		el := layouts{
			Entities: [][]layoutEntry{mkBlock(1)},
			Dead:     []deadRun{{AfterBlock: 0, Table: true}},
		}
		_, err := planLayout(el)
		assert.Error(t, err)
	})
}

func TestBuildEntityLayoutsRoundTrip(t *testing.T) {
	// the NO0 shape end to end: the generated C must place the unused blocks
	// between the room blocks and index them the way the retail data does
	el := layouts{
		Entities: [][]layoutEntry{mkBlock(2), mkBlock(1)},
		Indices:  []int{0, 1},
		Dead: []deadRun{{
			AfterBlock: 0,
			Blocks:     [][]layoutEntry{mkBlock(1)},
			Table:      true,
		}},
	}
	plan, err := planLayout(el)
	require.NoError(t, err)
	segs := segmentPlan(plan)
	names := segNames("sym", "x", segs)
	pos := placePlan(plan, segs)

	// block 0 holds 4 entries, so the unused block starts at entry 4 => u16 20
	require.Equal(t, layPos{seg: 0, entry: 4}, pos[1])
	assert.Equal(t, "sym_x", names[0])
	assert.Equal(t, "sym_x_unused_0", names[1])
	// the room block following the table lands at the start of the second array
	assert.Equal(t, layPos{seg: 2, entry: 0}, pos[3])
}

func TestGeneratedLayoutHasNoGaps(t *testing.T) {
	// every entry of every array has to be accounted for, or the rebuilt overlay
	// silently shrinks and everything after it shifts
	el := layouts{
		Entities: [][]layoutEntry{mkBlock(3), mkBlock(2)},
		Indices:  []int{0, 1},
		Dead: []deadRun{
			{AfterBlock: 0, Blocks: [][]layoutEntry{mkBlock(1)}, Table: true},
			{AfterBlock: 1, StrayMarker: true},
		},
	}
	plan, err := planLayout(el)
	require.NoError(t, err)

	entries, tableWords := 0, 0
	for _, item := range plan {
		entries += len(item.entries)
		if item.stray {
			entries++
		}
		tableWords += len(item.table)
	}
	// 5 + 3 + 2 blocks worth of entries, plus the stray marker
	assert.Equal(t, 5+3+4+1, entries)
	assert.Equal(t, 1, tableWords)
}

func TestSegNamesKeepsBareNameWithoutUnusedData(t *testing.T) {
	// overlays with nothing unused must generate exactly the symbols they did
	// before, since their splat and symbol configs already name them
	plan := []layItem{{entries: mkBlock(1), blockIdx: 0}}
	names := segNames("entity_layout_abc", "y", segmentPlan(plan))
	require.Len(t, names, 1)
	assert.Equal(t, "entity_layout_abc_y", names[0])
	assert.False(t, strings.Contains(names[0], "unused"))
}
