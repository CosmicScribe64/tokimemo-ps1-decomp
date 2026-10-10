#!/usr/bin/env python3
"""Tests for tools/object_boundaries.py, tools/split_objects.py and the srcscan C file list (T-0500).

Synthetic input only (no game data). Run: tools/docker.sh python3 tools/test_objects.py
"""
import io
import os
import shutil
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import object_boundaries as ob  # noqa: E402
import split_objects as so  # noqa: E402
import srcscan  # noqa: E402

BASE = 0x80132000


def insn(addr, op, args=""):
    return "    /* %X %08X 00000000 */  %s %s\n" % (addr - BASE, addr, op, args)


def func_s(name, addr, size, body=(), pad=0):
    """A splat function file: `size` bytes of instructions, `body` = [(op, args)] first."""
    lines = ["nonmatching %s, 0x%X\n\n" % (name, size), "glabel %s\n" % name]
    ops = list(body) + [("nop", "")] * (size // 4 - len(body))
    for k, (op, args) in enumerate(ops):
        lines.append(insn(addr + 4 * k, op, args))
    lines.append("endlabel %s\n" % name)
    for k in range(pad):
        lines.append(insn(addr + size + 4 * k, "nop"))
    return "".join(lines)


def rodata_s(blocks):
    """A splat rodata file from [(name, addr, kind, n words or string)]."""
    out = ['.include "macro.inc"\n\n.section .rodata, "a"\n\n']
    for name, addr, kind, val in blocks:
        out.append(".align 2\nnonmatching %s\n\ndlabel %s\n" % (name, name))
        if kind == "str":
            out.append('    /* %X %08X */ .asciz "%s"\n' % (addr - BASE, addr, val))
        else:
            for k in range(val):
                out.append("    /* %X %08X 00000000 */ .word 0x1\n" % (addr - BASE + 4 * k, addr + 4 * k))
        out.append("enddlabel %s\n\n" % name)
    return "".join(out)


# The synthetic overlay AAA: text 0x80132000-0x80132100, rodata from 0x80132100.
#   func_80132000 (0x38 + 2 nops of object padding), func_80132040, func_80132080
#   0x100 string D_80132100 (func_80132040), 0x104 jtbl_80132104 (2 words, func_80132040),
#   zero padding to 0x110; 0x110 string D_80132110 (func_80132080), 0x114 jtbl_80132114
#   (3 words, func_80132080) ending on 0x120; 0x120 D_80132120, written by func_80132000 (.data)
FUNCS = [
    ("func_80132000", 0x80132000, 0x38, [("lui", "$at, %hi(D_80132120)"), ("sw", "$v0, %lo(D_80132120)($at)")], 2),
    ("func_80132040", 0x80132040, 0x40, [("lui", "$a0, %hi(D_80132100)"), ("lui", "$at, %hi(jtbl_80132104)"),
                                         ("jal", "func_80132080")], 0),
    ("func_80132080", 0x80132080, 0x80, [("lui", "$a0, %hi(D_80132110)"), ("lui", "$at, %hi(jtbl_80132114)")], 0),
]
ITEMS = [("D_80132100", 0x80132100, "str", "ab"), ("jtbl_80132104", 0x80132104, "jtbl", 2),
         ("D_80132110", 0x80132110, "str", "xyz"), ("jtbl_80132114", 0x80132114, "jtbl", 3),
         ("D_80132120", 0x80132120, "data", 1)]


def image():
    data = bytearray(0x200)
    for _n, a, kind, val in ITEMS:
        o = a - BASE
        if kind == "str":
            data[o:o + len(val)] = val.encode()
        else:
            for k in range(val):
                struct.pack_into("<I", data, o + 4 * k, 0x80132010)
    return bytes(data)


def unit():
    return ob.Unit("AAA", "AAA.EXN", -BASE, (BASE, BASE + 0x100), BASE + 0x100, BASE + 0x200,
                   "asm/ovl/AAA/*matchings/**/*.s", "asm/ovl/AAA/data/**/*.s")


def parsed():
    funcs = [ob.parse_function(func_s(n, a, s, b, p)) for n, a, s, b, p in FUNCS]
    items = ob.parse_items(rodata_s(ITEMS))
    return funcs, items


class BoundaryTest(unittest.TestCase):
    def test_parse(self):
        funcs, items = parsed()
        self.assertEqual([f.addr for f in funcs], [0x80132000, 0x80132040, 0x80132080])
        self.assertEqual(funcs[0].stores, frozenset({"D_80132120"}))
        self.assertEqual(funcs[1].calls, frozenset({"func_80132080"}))
        self.assertEqual([i.kind for i in items], ["str", "jtbl", "str", "jtbl", "data"])

    def test_content_end(self):
        it = ob.Item("jtbl_x", 0, "jtbl", frozenset())
        self.assertEqual(ob.content_end(it, b"\x01\0\0\0" * 3 + b"\0" * 4), 12)
        st = ob.Item("D_x", 0, "str", frozenset())
        self.assertEqual(ob.content_end(st, b"ab\0\0\0\0\0\0"), 4)

    def test_analyze(self):
        funcs, items = parsed()
        res = ob.analyze(unit(), funcs, items, image())
        self.assertEqual(res.rodata_end, 0x80132120)
        self.assertEqual([(o.text_start, o.ro_start, o.text_ev, o.ro_ev) for o in res.objects], [
            (0x80132000, None, "start", "-"),
            (0x80132040, 0x80132100, "pad", "start"),
            (0x80132080, 0x80132110, "rodata", "jtbl-pad"),
        ])
        self.assertEqual([o.island for o in res.objects], ["no:no_rodata", "yes", "yes"])
        self.assertEqual(res.problems, [])

    def test_format_roundtrip(self):
        funcs, items = parsed()
        res = ob.analyze(unit(), funcs, items, image())
        with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
            f.write(ob.format_result(res))
        try:
            objs, ro_end, orphans = ob.read_objects(f.name)
        finally:
            os.remove(f.name)
        self.assertEqual(objs, res.objects)
        self.assertEqual(ro_end, res.rodata_end)
        self.assertEqual(orphans, [])

    def test_zeros_beyond_the_padding(self):
        # a table that ends on a 16-byte boundary followed by 16 zero bytes: the object ends at
        # the table, the zeros start the next chunk (a synthetic item names them)
        items = [ob.Item("jtbl_0", 0x0, "jtbl", frozenset()), ob.Item("D_20", 0x20, "data", frozenset())]
        data = b"\x01\0\0\0" * 4 + b"\0" * 16 + b"\x05" * 16
        problems = []
        rb, out = ob.layout_bounds(items, data, 0, 0x30, problems)
        self.assertEqual(rb, {0x10: "jtbl-end"})
        self.assertEqual([i.name for i in out], ["jtbl_0", "D_00000010", "D_20"])
        self.assertEqual(problems, [])

    def test_island_order(self):
        items = [ob.Item("D_1", 0, "str", frozenset()), ob.Item("D_2", 4, "str", frozenset()),
                 ob.Item("jtbl_3", 8, "jtbl", frozenset())]
        fa = ob.Func("a", 0x10, 0x100, frozenset(), frozenset(), frozenset())
        fb = ob.Func("b", 0x20, 0x100, frozenset(), frozenset(), frozenset())
        ok = [{0x10}, {0x20}, {0x20}]
        self.assertEqual(ob.island_check(items, [0, 1, 2], ok, [fa, fb]), "yes")
        swapped = [{0x20}, {0x10}, {0x20}]
        self.assertIn("out_of_function_order", ob.island_check(items, [0, 1, 2], swapped, [fa, fb]))
        sandwich = [{0x10}, set(), {0x10}]
        items2 = items[:2] + [ob.Item("D_4", 12, "str", frozenset())]
        self.assertIn("inside_one_function", ob.island_check(items2, [0, 1, 2], sandwich, [fa, fb]))
        foreign = [{0x10}, {0x999}, {0x20}]
        self.assertIn("another_object", ob.island_check(items, [0, 1, 2], foreign, [fa, fb]))
        short = ob.Func("b", 0x20, 0x20, frozenset(), frozenset(), frozenset())
        self.assertIn("too_short", ob.island_check(items, [0, 1, 2], ok, [fa, short], {2: 12}))

    def test_late_rodata_fits(self):
        self.assertFalse(ob.late_rodata_fits(12, 15))
        self.assertTrue(ob.late_rodata_fits(12, 22))
        self.assertFalse(ob.late_rodata_fits(5, 14))
        self.assertTrue(ob.late_rodata_fits(3, 12))


OVL_YAML = """name: ovl_AAA
options:
  asm_path: asm/ovl/AAA
  symbol_addrs_path:
    - config/symbol_addrs.txt
segments:
  - name: AAA
    type: code
    start: 0x0
    vram: 0x80132000
    subsegments:
      - [0x0, c, AAA]
      - [0x100, rodata, AAA_rodata]
  - [0x30000]
"""

OVL_C = """#include "common.h"
#include "ovl/AAA.h"

extern s32 D_unused;
extern s32 D_80132120;

INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA", func_80132000);

/* a comment that stays with the next function */
s32 func_80132040(void) {
    if (D_80132120 == 1) {
        return func_80132080();
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA", func_80132080);
"""


class SplitTest(unittest.TestCase):
    def setUp(self):
        self.root = tempfile.mkdtemp()
        w = self.write
        w("config/overlays.txt", "AAA 0x80132000 0x100\n")
        w("config/overlays/AAA.yaml", OVL_YAML)
        w("config/SLPM_86.053.yaml", "segments: []\n")
        w("src/ovl/AAA.c", OVL_C)
        w("include/ovl/AAA.h", "#ifndef AAA_H\n#define AAA_H\nextern s32 D_80132120;\n#endif\n")
        for n, a, s, b, p in FUNCS:
            w("asm/ovl/AAA/nonmatchings/AAA/%s.s" % n, func_s(n, a, s, b, p))
        w("asm/ovl/AAA/data/AAA_rodata.rodata.s", rodata_s(ITEMS))
        funcs, items = parsed()
        res = ob.analyze(unit(), funcs, items, image())
        w("config/objects/AAA.txt", ob.format_result(res))

    def tearDown(self):
        shutil.rmtree(self.root)

    def write(self, rel, text):
        p = os.path.join(self.root, rel)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, "w") as f:
            f.write(text)

    def read(self, rel):
        with open(os.path.join(self.root, rel)) as f:
            return f.read()

    def run_split(self):
        writes, protos = so.plan(self.root, ["AAA"], out=io.StringIO())
        so.apply(self.root, writes, protos)
        return writes, protos

    def test_parse_c_roundtrip(self):
        pieces = so.parse_c(OVL_C)
        self.assertEqual("".join(p.text for p in pieces), OVL_C)
        self.assertEqual([p.kind for p in pieces], ["include", "include", "decl", "decl", "asm", "func", "asm"])
        self.assertEqual(pieces[5].func, "func_80132040")
        self.assertEqual(pieces[3].names, {"D_80132120"})

    def test_declared_names(self):
        self.assertEqual(so.declared_names("extern s32 a, b[4];"), ({"a", "b"}, False))
        self.assertEqual(so.declared_names("void f(s32 x);"), ({"f"}, False))
        self.assertEqual(so.declared_names("static s32 n = 3;"), ({"n"}, True))
        self.assertEqual(so.declared_names("typedef struct { s32 a; } Foo;")[0], {"Foo"})

    def test_split_and_rerun(self):
        writes, protos = self.run_split()
        self.assertFalse(os.path.exists(os.path.join(self.root, "src/ovl/AAA.c")))
        a = self.read("src/ovl/AAA/80132000.c")
        b = self.read("src/ovl/AAA/80132040.c")
        c = self.read("src/ovl/AAA/80132080.c")
        self.assertIn('INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA/80132000", func_80132000);', a)
        self.assertNotIn("D_unused", a + b + c)          # declarations go where they are used
        self.assertIn("extern s32 D_80132120;", b)
        self.assertNotIn("extern s32 D_80132120;", c)
        self.assertIn("/* a comment that stays with the next function */\ns32 func_80132040", b)
        self.assertIn('#include "ovl/AAA.h"', c)
        self.assertEqual(protos, [])                     # func_80132080 is asm, nothing to declare
        y = self.read("config/overlays/AAA.yaml")
        self.assertIn("      - [0x40, c, AAA/80132040]\n", y)
        self.assertIn("      - [0x100, .rodata, AAA/80132040]\n", y)
        self.assertIn("      - [0x110, .rodata, AAA/80132080]\n", y)
        self.assertIn("      - [0x120, data, AAA_data]\n", y)
        self.assertNotIn("config/labels", y)
        self.assertIn("  - [0x30000]\n", y)
        # the C file list follows the yaml
        names = [cf.name for cf in srcscan.unit_c_files("AAA", self.root)]
        self.assertEqual(names, ["AAA/80132000", "AAA/80132040", "AAA/80132080"])
        self.assertEqual([cf.island for cf in srcscan.unit_c_files("AAA", self.root)], [False, True, True])
        # running again changes nothing
        writes, protos = so.plan(self.root, ["AAA"], out=io.StringIO())
        self.assertEqual(writes, [])

    def test_prototype_for_cross_object_call(self):
        c = OVL_C.replace('INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA", func_80132080);',
                          "s32 func_80132080(void) {\n    return 2;\n}")
        self.write("src/ovl/AAA.c", c)
        _w, protos = self.run_split()
        self.assertEqual(protos, [("s32 func_80132080(void);", "include/ovl/AAA.h")])
        self.assertIn("s32 func_80132080(void);\n", self.read("include/ovl/AAA.h"))
        self.assertTrue(self.read("include/ovl/AAA.h").rstrip().endswith("#endif"))

    def test_unowned_rodata_gets_include_rodata(self):
        # nobody names D_80132110 any more: it needs an INCLUDE_RODATA in front of the owner of
        # the next symbol... which is none (only a table follows), so at the end of the file,
        # and a label in config/labels/AAA.txt that the yaml reads
        n, a, s, b, p = FUNCS[2]
        self.write("asm/ovl/AAA/nonmatchings/AAA/%s.s" % n, func_s(n, a, s, b[1:], p))
        self.run_split()
        c = self.read("src/ovl/AAA/80132080.c")
        self.assertIn('INCLUDE_RODATA("asm/ovl/AAA/data/AAA/80132080.rodata", D_80132110);', c)
        self.assertIn("D_80132110 = 0x80132110;", self.read("config/labels/AAA.txt"))
        self.assertIn("    - config/labels/AAA.txt\n", self.read("config/overlays/AAA.yaml"))
        writes, _p = so.plan(self.root, ["AAA"], out=io.StringIO())
        self.assertEqual(writes, [])

    def test_not_a_union_of_objects(self):
        self.write("config/overlays/AAA.yaml", OVL_YAML.replace(
            "      - [0x0, c, AAA]\n", "      - [0x0, c, AAA]\n      - [0x60, c, AAA_b]\n"))
        with self.assertRaises(so.SplitError):
            so.plan(self.root, ["AAA"], out=io.StringIO())


class MainYamlTest(unittest.TestCase):
    YAML = """      - { start: 0x800, type: c, name: main/80041000 }
      - { start: 0x1000, type: c, name: main/80041800 }
      - [0x2000, asm, lib]
      - [0x3000, rodata, rodata]
      - [0x3100, .rodata, main/80041800]
      - [0x3200, rodata, rodata_after]
      - [0x4000, data, data]
"""

    def test_split_one_file(self):
        o = lambda t, e, rs, re_: ob.Obj(t, e, rs, re_, "yes", "pad", "start")
        objs = [o(0x80041000, 0x80041400, 0x80043800, 0x80043840),
                o(0x80041400, 0x80041800, 0x80043840, 0x80043880)]
        out = so.rewrite_main_yaml(self.YAML, {"main/80041000": objs}, 0x80043800)
        self.assertEqual(out, """      - { start: 0x800, type: c, name: main/80041000 }
      - { start: 0xC00, type: c, name: main/80041400 }
      - { start: 0x1000, type: c, name: main/80041800 }
      - [0x2000, asm, lib]
      - [0x3000, .rodata, main/80041000]
      - [0x3040, .rodata, main/80041400]
      - [0x3080, rodata, rodata_80043880]
      - [0x3100, .rodata, main/80041800]
      - [0x3200, rodata, rodata_80043A00]
      - [0x4000, data, data]
""")


class SrcscanTest(unittest.TestCase):
    def test_asm_dirs_of_source(self):
        self.assertEqual(srcscan.asm_dirs_of_source("src/ovl/TT/80132000.c"),
                         ["asm/ovl/TT/matchings/TT/80132000", "asm/ovl/TT/nonmatchings/TT/80132000"])
        self.assertEqual(srcscan.asm_dirs_of_source("/work/src/main/80041000.c"),
                         ["asm/matchings/main/80041000", "asm/nonmatchings/main/80041000"])


if __name__ == "__main__":
    unittest.main()
