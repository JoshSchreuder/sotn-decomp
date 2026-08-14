package layout

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/assets"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/datarange"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/psx"
)

func TestExtractPropagatesLayoutReadError(t *testing.T) {
	base := psx.Addr(0x80180000)
	data := make([]byte, 0x40)
	binary.LittleEndian.PutUint32(data[0x1C:], uint32(base.Sum(0x40)))

	err := Handler.Extract(assets.ExtractArgs{
		Data:     data,
		AssetDir: t.TempDir(),
		Name:     "entity_layouts",
		RamBase:  base,
	})
	assert.Error(t, err)
}

func TestReadEntityLayoutPreservesUnclaimedRanges(t *testing.T) {
	const count = 2
	base := psx.Addr(0x80180000)
	data := make([]byte, 0xA0)

	blockOffsets := []int{0x20, 0x40, 0x60, 0x80}
	for i, off := range blockOffsets {
		binary.LittleEndian.PutUint32(data[i*4:], uint32(base.Sum(off)))
		binary.LittleEndian.PutUint16(data[off:], uint16(0xFFFE))
		binary.LittleEndian.PutUint16(data[off+2:], uint16(0xFFFE))
		binary.LittleEndian.PutUint16(data[off+10:], uint16(0xFFFF))
		binary.LittleEndian.PutUint16(data[off+12:], uint16(0xFFFF))
	}

	layouts, ranges, err := readEntityLayout(
		bytes.NewReader(data), "dummy", base, base, count, true)
	require.NoError(t, err)
	assert.Len(t, layouts.Entities, count)
	assert.Empty(t, layouts.DeadPointers)
	assert.Equal(t, []datarange.DataRange{
		datarange.New(base, base.Sum(0x10)),
		datarange.New(base.Sum(0x20), base.Sum(0x34)),
		datarange.New(base.Sum(0x40), base.Sum(0x54)),
		datarange.New(base.Sum(0x60), base.Sum(0x74)),
		datarange.New(base.Sum(0x80), base.Sum(0x94)),
	}, ranges)
}

func TestReadEntityLayoutClaimsDeadBlocksAndPointers(t *testing.T) {
	const count = 2
	base := psx.Addr(0x80180000)
	data := make([]byte, 0xA0)

	// a block is made of a -2 entry immediately followed by a -1 one
	putBlock := func(off int) {
		binary.LittleEndian.PutUint16(data[off:], 0xFFFE)
		binary.LittleEndian.PutUint16(data[off+2:], 0xFFFE)
		binary.LittleEndian.PutUint16(data[off+10:], 0xFFFF)
		binary.LittleEndian.PutUint16(data[off+12:], 0xFFFF)
	}
	// each axis holds two blocks the layout table refers to, with a block no
	// room reaches and a pointer table aimed at it sitting in between
	for axis, dataBegin := range []int{0x20, 0x60} {
		binary.LittleEndian.PutUint32(data[axis*8:], uint32(base.Sum(dataBegin)))
		binary.LittleEndian.PutUint32(data[axis*8+4:], uint32(base.Sum(dataBegin+0x2C)))
		putBlock(dataBegin)
		putBlock(dataBegin + 0x14)
		binary.LittleEndian.PutUint32(
			data[dataBegin+0x28:], uint32(base.Sum(dataBegin+0x14)))
		putBlock(dataBegin + 0x2C)
	}

	layouts, ranges, err := readEntityLayout(
		bytes.NewReader(data), "dummy", base, base, count, true)
	require.NoError(t, err)
	// the dead block is an ordinary block that no index points at
	assert.Len(t, layouts.Entities, 3)
	assert.Equal(t, []int{0, 2}, layouts.Indices)
	assert.Equal(t, []deadPointers{{AfterBlock: 1, Blocks: []int{1}}},
		layouts.DeadPointers)

	// nothing is left unclaimed, so the whole of the layout data is one range
	consolidated, err := datarange.ConsolidateDataRanges(ranges[1:])
	require.NoError(t, err)
	assert.Equal(t, []datarange.DataRange{
		datarange.New(base.Sum(0x20), base.Sum(0xA0)),
	}, consolidated)
}

func TestBuildSplitsTheArrayAtADeadPointerTable(t *testing.T) {
	assetDir := t.TempDir()
	srcDir := t.TempDir()

	// the middle block is the one no room refers to, and the table points at it
	block := []layoutEntry{{X: -2, Y: -2, ID: "0x00"}, {X: -1, Y: -1, ID: "0x00"}}
	serialized, err := json.Marshal(layouts{
		Entities:     [][]layoutEntry{block, block, block},
		Indices:      []int{0, 2},
		DeadPointers: []deadPointers{{AfterBlock: 1, Blocks: []int{1}}},
	})
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(
		filepath.Join(assetDir, "entity_layouts.json"), serialized, 0o644))

	require.NoError(t, Handler.Build(assets.BuildArgs{
		AssetDir: assetDir,
		SrcDir:   srcDir,
		Name:     "entity_layouts",
		OvlName:  "dummy",
	}))

	laydef, err := os.ReadFile(filepath.Join(srcDir, "gen", "e_laydef.c"))
	require.NoError(t, err)
	// the symbol name is a hash of the output directory, so read it back
	_, rest, ok := strings.Cut(string(laydef), "extern LayoutEntity ")
	require.True(t, ok)
	sym, _, ok := strings.Cut(rest, "_x[];")
	require.True(t, ok)

	assert.Contains(t, string(laydef), fmt.Sprintf(`extern LayoutEntity %[1]s_x[];
extern LayoutEntity %[1]s_x_2[];
LayoutEntity* entityLayoutHorizontal[] = {
    &%[1]s_x[0],
    &%[1]s_x_2[0],
};
`, sym))

	entry := "    0xFFFE, 0xFFFE, 0x00 | 0x0000, 0x0000, 0x0000,\n" +
		"    0xFFFF, 0xFFFF, 0x00 | 0x0000, 0x0000, 0x0000,\n"
	generated, err := os.ReadFile(filepath.Join(srcDir, "gen", "e_layout.c"))
	require.NoError(t, err)
	// the array is closed at the table so that its pointers can be relocations,
	// and they are scaled from entries to the u16 the array is made of
	assert.Contains(t, string(generated), fmt.Sprintf(`u16 %[1]s_x[] = {
// Offset 0, Room 0x00
%[2]s// Offset 2, No Room Found
%[2]s};
LayoutEntity* %[1]s_x_dead_ptrs[] = {
    (LayoutEntity*)&%[1]s_x[10],
};
u16 %[1]s_x_2[] = {
// Offset 0, Room 0x01
%[2]s};
`, sym, entry))
}

func TestPlanLayoutHandlesSeveralSplits(t *testing.T) {
	plan, err := planLayout(layouts{
		Entities: [][]layoutEntry{
			make([]layoutEntry, 2), make([]layoutEntry, 2),
			make([]layoutEntry, 2), make([]layoutEntry, 2),
		},
		DeadPointers: []deadPointers{
			{AfterBlock: 1, Blocks: []int{1}},
			{AfterBlock: 2, Blocks: []int{1}},
		},
	})
	require.NoError(t, err)
	assert.Equal(t, []int{0, 0, 1, 2}, plan.arrayOf)
	assert.Equal(t, []int{0, 2, 0, 0}, plan.entryOf)
	assert.Equal(t, "sym_x", arrayName("sym", "x", 0))
	assert.Equal(t, "sym_x_3", arrayName("sym", "x", 2))
}

func TestPlanLayoutRejectsAMisalignedSplit(t *testing.T) {
	_, err := planLayout(layouts{
		Entities:     [][]layoutEntry{make([]layoutEntry, 1)},
		DeadPointers: []deadPointers{{AfterBlock: 0, Blocks: []int{0}}},
	})
	assert.ErrorContains(t, err, "do not fill a multiple of 4 bytes")
}
