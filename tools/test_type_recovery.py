#!/usr/bin/env python3
"""Tests for tools/type_recovery.py with synthetic splat-style asm (no game data).

Run: tools/docker.sh python3 tools/test_type_recovery.py
"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import type_recovery as tr  # noqa: E402


def asm(name, body):
    """Splat-style function text; body lines are instructions or `.Lxxxxxxxx:` labels."""
    out = ["nonmatching %s, 0x0" % name, "", "glabel %s" % name]
    addr = 0x80010000
    for line in body:
        if line.startswith("."):
            out.append("  " + line)
            continue
        out.append("    /* %X %08X 00000000 */  %s" % (addr - 0x80000000, addr, line))
        addr += 4
    out.append("endlabel " + name)
    return out


def syms(*addrs):
    s = tr.Symbols()
    for a in addrs:
        s.add_label("main", "D_%08X" % a, a)
    return s


def run(name, body, s, unit="main"):
    return tr.analyze_function(name, asm(name, body), unit, s)


class Analyze(unittest.TestCase):
    def test_indexed_access_gives_stride(self):
        s = syms(0x80100000)
        r = run("f", ["lui $t6, %hi(D_80100000)",
                      "sll $t7, $a0, 2",
                      "addu $t6, $t6, $t7",
                      "lw $v0, %lo(D_80100000)($t6)",
                      "jr $ra", " nop"], s)
        idx = [a for a in r.accesses if a.kind == "idx"]
        self.assertEqual([(a.addr, a.stride, a.width) for a in idx], [(0x80100000, 4, 4)])

    def test_shift_add_index_0x44(self):
        # x * 0x44 = ((x << 4) + x) << 2, the way IDO computes a record offset
        s = syms(0x80100003)
        r = run("f", ["sll $t6, $a0, 4",
                      "addu $t6, $t6, $a0",
                      "sll $t6, $t6, 2",
                      "lui $at, %hi(D_80100003)",
                      "addu $at, $at, $t6",
                      "lbu $v0, %lo(D_80100003)($at)"], s)
        self.assertEqual([(a.stride, a.sign) for a in r.accesses], [(0x44, "u")])

    def test_multu_by_constant(self):
        s = syms(0x80100000)
        r = run("f", ["addiu $t0, $zero, 0x38",
                      "multu $a0, $t0",
                      "mflo $t1",
                      "lui $at, %hi(D_80100000)",
                      "addu $at, $at, $t1",
                      "lh $v0, %lo(D_80100000)($at)"], s)
        self.assertEqual([(a.stride, a.sign) for a in r.accesses], [(0x38, "s")])

    def test_ordered_store_then_load(self):
        s = syms(0x80100000, 0x80100004)
        r = run("f", ["lui $t6, %hi(D_80100000)",
                      "lh $t6, %lo(D_80100000)($t6)",
                      "lui $at, %hi(D_80100000)",
                      "addiu $t7, $t6, 1",
                      "sh $t7, %lo(D_80100000)($at)",
                      "lui $t8, %hi(D_80100004)",
                      "lh $t8, %lo(D_80100004)($t8)"], s)
        self.assertEqual(r.orders, [("main", 0x80100000, 0x80100004)])

    def test_no_order_across_call_or_label(self):
        s = syms(0x80100000, 0x80100004)
        r = run("f", ["lui $at, %hi(D_80100000)",
                      "sw $zero, %lo(D_80100000)($at)",
                      "jal func_80010000",
                      " nop",
                      "lui $t8, %hi(D_80100004)",
                      "lw $t8, %lo(D_80100004)($t8)",
                      "lui $at, %hi(D_80100000)",
                      "sw $zero, %lo(D_80100000)($at)",
                      ".L80010020:",
                      "lui $t8, %hi(D_80100004)",
                      "lw $t8, %lo(D_80100004)($t8)"], s)
        self.assertEqual(r.orders, [])

    def test_pointer_fields_and_walk_with_end(self):
        s = syms(0x80100000, 0x80100040)
        r = run("f", ["lui $a0, %hi(D_80100000)",
                      "addiu $a0, $a0, %lo(D_80100000)",
                      "lui $a1, %hi(D_80100040)",
                      "addiu $a1, $a1, %lo(D_80100040)",
                      ".L80010010:",
                      "sh $zero, 0x4($a0)",
                      "addiu $a0, $a0, 0x10",
                      "bne $a0, $a1, .L80010010",
                      " nop"], s)
        self.assertIn(("main", 0x80100000, 0x80100004), r.ptrs)
        self.assertIn(("main", 0x80100000, 0x10), r.walks)
        self.assertIn(("main", 0x80100000, 0x80100040), r.ends)

    def test_shared_lui_at(self):
        s = syms(0x80100000, 0x80100002)
        r = run("f", ["lui $at, %hi(D_80100000)",
                      "sh $zero, %lo(D_80100000)($at)",
                      "sh $zero, %lo(D_80100002)($at)"], s)
        self.assertEqual(r.shared_at, [("main", 0x80100000, 0x80100002)])

    def test_overlay_symbol_space(self):
        s = tr.Symbols()
        s.add_label("DATE", "D_80150000", 0x80150000)
        self.assertEqual(s.resolve("DATE", "D_80150000"), ("DATE", 0x80150000))
        self.assertEqual(s.resolve("DATE", "D_800E643C"), ("main", 0x800E643C))
        self.assertEqual(s.resolve("DATE", "D_80170000"), ("ext", 0x80170000))


class Proposals(unittest.TestCase):
    def test_indexed_array_absorbs_later_elements(self):
        base = 0x80100000
        s = syms(base + 2, base + 6, base + 0x38 * 3 + 2, base + 0x38 * 3 + 6)
        idx = run("g", ["sll $t6, $a0, 3", "subu $t6, $t6, $a0", "sll $t6, $t6, 3",
                        "lui $at, %hi(D_80100002)", "addu $at, $at, $t6",
                        "lh $v0, %lo(D_80100002)($at)",
                        "lui $at, %hi(D_80100006)", "addu $at, $at, $t6",
                        "lh $v1, %lo(D_80100006)($at)"], s)
        direct = run("h", ["lui $t6, %hi(D_801000AA)", "lh $t6, %lo(D_801000AA)($t6)",
                           "lui $at, %hi(D_801000AA)", "sh $t6, %lo(D_801000AA)($at)",
                           "lui $t7, %hi(D_801000AE)", "lh $t7, %lo(D_801000AE)($t7)"], s)
        props = tr.build_proposals([idx, direct], s)
        arr = [p for p in props if p["stride"] == 0x38]
        self.assertEqual(len(arr), 1)
        p = arr[0]
        self.assertEqual(p["base_addr"], base + 2)
        self.assertEqual(p["count"], 4)
        self.assertIn("D_801000AA", p["absorbs"])
        self.assertEqual({f["offset"] for f in p["fields"]}, {0, 4})
        self.assertEqual(p["fields"][0]["sign"], "signed")
        self.assertEqual(p["evidence"]["order"], 1)

    def test_order_pairs_from_two_functions_are_high(self):
        s = syms(0x80100000, 0x80100004)
        body = ["lui $at, %hi(D_80100000)", "sw $zero, %lo(D_80100000)($at)",
                "lui $t8, %hi(D_80100004)", "lw $t8, %lo(D_80100004)($t8)"]
        props = tr.build_proposals([run("a", body, s), run("b", body, s)], s)
        self.assertEqual([(p["base"], p["absorbs"], p["confidence"]) for p in props],
                         [("D_80100000", ["D_80100004"], "high")])
        one = tr.build_proposals([run("a", body, s)], s)
        self.assertEqual(one[0]["confidence"], "medium")

    def test_cooccurrence_is_low(self):
        s = syms(0x80100000, 0x80100001)
        body = ["lui $t6, %hi(D_80100000)", "lbu $t6, %lo(D_80100000)($t6)",
                "lui $t7, %hi(D_80100001)", "lbu $t7, %lo(D_80100001)($t7)"]
        props = tr.build_proposals([run("a", body, s), run("b", body, s)], s)
        self.assertEqual([(p["confidence"], p["evidence"]) for p in props], [("low", {"cooc": 2})])

    def test_pick_stride(self):
        self.assertEqual(tr.pick_stride([0x44, 0x330, 0x88]), 0x44)
        self.assertEqual(tr.pick_stride([1, 0x24]), 0x24)
        self.assertIsNone(tr.pick_stride([]))


class FakeSites(unittest.TestCase):
    def test_index_trick_is_covered(self):
        with tempfile.TemporaryDirectory() as d:
            os.makedirs(os.path.join(d, "include"))
            os.makedirs(os.path.join(d, "src", "main"))
            with open(os.path.join(d, "include", "main_api.h"), "w") as f:
                f.write("extern s16 D_80100000;\n")
            with open(os.path.join(d, "src", "main", "80010000.c"), "w") as f:
                f.write("void f(void) {\n    /* FAKE: first symbol */\n    (&D_80100000)[2] += 1;\n"
                        "    ((u8 *)&D_80100000)[0x50] = 0;\n}\n")
            prop = {"space": "main", "addrs": [0x80100000, 0x80100004], "symbols": [], "base": "D_80100000",
                    "confidence": "high"}
            sites = tr.fake_sites(d, [prop])
        self.assertEqual([(s["target"], s["covered"]) for s in sites],
                         [("D_80100004", True), ("D_80100050", False)])


if __name__ == "__main__":
    unittest.main()
