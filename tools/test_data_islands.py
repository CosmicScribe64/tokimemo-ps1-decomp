#!/usr/bin/env python3
"""Unit tests for the per-object data tools (T-9010): tools/data_pieces.py, tools/data_island.py and
the .data/.bss part of tools/object_boundaries.py. Synthetic input only, no game data.

usage: python3 tools/test_data_islands.py
"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import data_island  # noqa: E402
import data_pieces as dp  # noqa: E402
import object_boundaries as ob  # noqa: E402

ISLAND_S = """.include "macro.inc"

.section .data, "wa"

nonmatching D_80001000

dlabel D_80001000
    /* 1000 80001000 01000000 */ .word 0x00000001
enddlabel D_80001000

dlabel D_80001004
    /* 1004 80001004 */ .byte 0x07
enddlabel D_80001004

dlabel D_80001005
    /* 1005 80001005 */ .byte 0x08
    /* 1006 80001006 */ .short 0x0000
enddlabel D_80001005

dlabel D_80001008
    /* 1008 80001008 00200080 */ .word func_80002000
    /* 100C 8000100C 00000000 */ .word 0x00000000
enddlabel D_80001008
"""


class DataPiecesTest(unittest.TestCase):
    def setUp(self):
        self.blob = bytes(0x1000) + bytes([1, 0, 0, 0, 7, 8, 0, 0, 0, 0x20, 0, 0x80, 0, 0, 0, 0])

    def test_pieces_start_on_aligned_symbols_and_keep_relocations(self):
        labels, lines = dp.parse_island(ISLAND_S)
        out = dict(dp.pieces(labels, dp.relocations(lines), self.blob, 0x1000, 0x80001000, 16))
        self.assertEqual(sorted(out), ["D_80001000", "D_80001004", "D_80001008"])
        self.assertIn("dlabel D_80001005\n    .byte 0x08, 0x00, 0x00\n", out["D_80001004"])
        self.assertIn("    .word func_80002000\n    .byte 0x00, 0x00, 0x00, 0x00\n", out["D_80001008"])
        self.assertTrue(out["D_80001000"].startswith(".section .data\n"))

    def test_island_must_be_16_aligned(self):
        labels, lines = dp.parse_island(ISLAND_S)
        with self.assertRaises(dp.PieceError):
            dp.pieces(labels, dp.relocations(lines), self.blob, 0x1000, 0x80001000, 12)

    def test_unaligned_relocation_fails(self):
        with self.assertRaises(dp.PieceError):
            dp.relocations([(0x1002, 0x80001002, ".word", "func_80002000")])

    def test_provide_lines(self):
        self.assertEqual(dp.provide_text([(0x80001004, "D_80001004")]),
                         "PROVIDE(D_80001004 = 0x80001004);\n")

    def test_islands_from_config(self):
        cfg = {"segments": [{"name": "X", "type": "code", "start": 0, "subsegments": [
            [0x0, "c", "X/80000000"], [0x100, "data", "X_data"], [0x120, ".data", "X/80000000"],
            [0x140, "data", "X_data_80000140"]]}, [0x200]]}
        self.assertEqual(dp.islands(cfg), [("X/80000000", 0x120, 0x140)])


class DataIslandTest(unittest.TestCase):
    YAML = ("segments:\n  - name: X\n    vram: 0x80000000\n    subsegments:\n"
            "      - [0x0, c, X/80000000]\n      - [0x100, data, X_data]\n  - [0x200]\n")

    def test_split_yaml_middle(self):
        out = data_island.split_yaml(self.YAML, "X", 0x80000120, 0x80000140, "X/80000000")
        self.assertIn("      - [0x100, data, X_data]\n      - [0x120, .data, X/80000000]\n"
                      "      - [0x140, data, X_data_80000140]\n", out)

    def test_split_yaml_at_start_and_end(self):
        out = data_island.split_yaml(self.YAML, "X", 0x80000100, 0x80000200, "X/80000000")
        self.assertIn("      - [0x100, .data, X/80000000]\n  - [0x200]\n", out)
        self.assertNotIn("X_data", out)

    def test_split_yaml_outside(self):
        with self.assertRaises(data_island.IslandError):
            data_island.split_yaml(self.YAML, "X", 0x80000010, 0x80000020, "X/80000000")

    def test_insert_block_after_includes(self):
        text = '#include "common.h"\n#include "ovl/X.h"\n\nINCLUDE_ASM("a", f);\n'
        out = data_island.insert_block(text, ['INCLUDE_RODATA("p", D_1);'])
        self.assertTrue(out.startswith('#include "common.h"\n#include "ovl/X.h"\n/* .data'))
        self.assertLess(out.index("D_1"), out.index("INCLUDE_ASM"))


def func_s(name, addr, body):
    rows = ["nonmatching %s, 0x%X" % (name, 4 * len(body)), "", "glabel %s" % name]
    for k, ins in enumerate(body):
        rows.append("    /* %X %08X 00000000 */  %s" % (addr + 4 * k, addr + 4 * k, ins))
    return "\n".join(rows) + "\n"


class DataBoundsTest(unittest.TestCase):
    def test_at_groups(self):
        text = func_s("f", 0x80000000, [
            "lui        $at, %hi(D_A)", "sw         $zero, %lo(D_A)($at)", "sw         $zero, %lo(D_A4)($at)",
            "lui        $at, %hi(D_B)", "sw         $zero, %lo(D_B)($at)",
            "slti       $at, $v0, 3", "sw         $zero, %lo(D_C)($at)"])
        self.assertEqual(ob.parse_function(text).at_groups, (frozenset({"D_A", "D_A4"}),))

    def test_label_ends_a_group(self):
        text = func_s("f", 0x80000000, ["lui        $at, %hi(D_A)", "sw         $zero, %lo(D_A)($at)"])
        text += "  .L80000008:\n" + func_s("g", 0x80000008, ["sw         $zero, %lo(D_A4)($at)"]).split("glabel g\n")[1]
        self.assertEqual(ob.parse_function(text).at_groups, ())

    def test_bounds_in_object_order(self):
        items = [(0x100, "a1"), (0x104, "a2"), (0x110, "b1"), (0x118, "x"), (0x120, "c1"), (0x130, "c2")]
        ev = {"a1": (0, 1), "b1": (1, 10), "x": (0, 1), "c1": (2, 1)}
        res, problems = ob.data_bounds(items, ev, 0x100, 0x140)
        self.assertEqual([r[:3] for r in res], [(0, 0x100, 0x110), (1, 0x110, 0x120), (2, 0x120, 0x130)])
        self.assertEqual(res[0][3], "start")
        self.assertEqual(problems, [])     # x: a global of object 1 that only object 0 uses

    def test_no_aligned_boundary_drops_the_weaker_item(self):
        items = [(0x100, "a1"), (0x104, "b1"), (0x110, "b2")]
        ev = {"a1": (0, 1), "b1": (1, 10), "b2": (1, 1)}
        res, problems = ob.data_bounds(items, ev, 0x100, 0x120)
        self.assertEqual([r[:3] for r in res], [(1, 0x100, 0x120)])
        self.assertIn("globals of another object", problems[-1])

    def test_parse_bss_items(self):
        text = ("dlabel D_800E3800\n    /* 800E3800 */ .space 0x10\n"
                "dlabel D_800E3810\n    /* 800E3810 */ .space 0x4\n")
        self.assertEqual([a for a, _n, _w in ob.parse_data_items(text)], [0x800E3800, 0x800E3810])

    def test_data_lines_round_trip(self):
        r = ob.DataRange("data", 0x80132000, 0x80140000, 0x80140010, "owner", "owner-min/2", "at:1,ref:2")
        with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
            f.write("object 80132000 80133000 - - no:no_rodata start -\n")
        try:
            ob.write_data(f.name, [r])
            ob.write_data(f.name, [r])
            self.assertEqual(ob.read_data(f.name), [r])
            with open(f.name) as g:
                self.assertEqual(g.read().count("object "), 1)
        finally:
            os.remove(f.name)


if __name__ == "__main__":
    unittest.main()
