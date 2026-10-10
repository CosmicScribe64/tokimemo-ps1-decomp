#!/usr/bin/env python3
"""Tests for tools/aggregate_audit.py with synthetic splat-style asm (no game data).

Run: tools/docker.sh python3 tools/test_aggregate_audit.py
"""
import io
import os
import sys
import tempfile
import unittest
from contextlib import redirect_stdout

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import aggregate_audit as aa  # noqa: E402
import type_recovery as tr  # noqa: E402

BASE = 0x80100000
RANGES = aa.Ranges([("D_80100000", BASE, BASE + 0x100)])


def asm(name, body):
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


def run(body, name="f"):
    return aa.analyze_function(name, asm(name, body), "main", tr.Symbols(), RANGES)


def rmw(addr, reg, tmp, imm=1, op="lw", st="sw"):
    """`D += imm` the way IDO emits it for a scalar or a struct member."""
    return ["lui %s, %%hi(D_%08X)" % (reg, addr), "%s %s, %%lo(D_%08X)(%s)" % (op, reg, addr, reg),
            "lui $at, %%hi(D_%08X)" % addr, "addiu %s, %s, %d" % (tmp, reg, imm),
            "%s %s, %%lo(D_%08X)($at)" % (st, tmp, addr)]


class Kinds(unittest.TestCase):
    def test_direct_access(self):
        r = run(["lui $v0, %hi(D_8010003A)", "lbu $v0, %lo(D_8010003A)($v0)", "jr $ra", " nop"])
        self.assertEqual([(a.addr, a.kind, a.store) for a in r.accesses], [(BASE + 0x3A, "direct", False)])

    def test_shared_hi_is_base_relative(self):
        r = run(["lui $v0, %hi(D_80100010)", "lbu $a0, %lo(D_80100010)($v0)",
                 "lbu $a1, %lo(D_80100020)($v0)"])
        self.assertEqual([a.kind for a in r.accesses], ["direct", "sharedhi"])

    def test_pointer_offsets(self):
        r = run(["lui $v0, %hi(D_80100000)", "addiu $v0, $v0, %lo(D_80100000)",
                 "lh $a0, 0x3A($v0)", "sw $zero, 0x40($v0)"])
        self.assertEqual([(a.addr - BASE, a.kind, a.store) for a in r.accesses],
                         [(0x3A, "ptr", False), (0x40, "ptr", True)])

    def test_indexed_and_strided_pointer(self):
        r = run(["lui $at, %hi(D_80100008)", "addu $at, $at, $t6", "lbu $v0, %lo(D_80100008)($at)",
                 "lui $v1, %hi(D_80100000)", "addiu $v1, $v1, %lo(D_80100000)", "addu $v1, $v1, $t7",
                 "lbu $a0, 0x4($v1)"])
        self.assertEqual([a.kind for a in r.accesses], ["idx", "ptridx"])

    def test_hi_from_elsewhere_is_unknown(self):
        r = run([".L80010000:", "lw $t8, %lo(D_80100020)($t8)"])
        self.assertEqual([a.kind for a in r.accesses], ["lo?"])

    def test_outside_aggregate_not_listed(self):
        r = run(["lui $v0, %hi(D_80200000)", "lw $v0, %lo(D_80200000)($v0)"])
        self.assertEqual(r.accesses, [])


class Pairs(unittest.TestCase):
    def test_kept_pair_inside(self):
        r = run(rmw(BASE + 0x40, "$t6", "$t7") + rmw(BASE + 0x44, "$t8", "$t9"))
        self.assertEqual(r.pairs, [("in", "kept", BASE + 0x40, BASE + 0x44)])

    def test_hoisted_pair_outside(self):
        # what as1 does with two separate symbols: the second load moves above the first store
        x, y = 0x80200040, 0x80200044
        body = ["lui $t6, %%hi(D_%08X)" % x, "lw $t6, %%lo(D_%08X)($t6)" % x,
                "lui $t8, %%hi(D_%08X)" % y, "lw $t8, %%lo(D_%08X)($t8)" % y,
                "lui $at, %%hi(D_%08X)" % x, "addiu $t7, $t6, 1", "sw $t7, %%lo(D_%08X)($at)" % x,
                "lui $at, %%hi(D_%08X)" % y, "addiu $t9, $t8, 1", "sw $t9, %%lo(D_%08X)($at)" % y]
        r = run(body)
        self.assertEqual(r.pairs, [("out", "hoisted", x, y)])

    def test_dependent_values_are_not_a_pair(self):
        # Y = Y + X: the store of Y depends on the load of X
        x, y = BASE + 0x40, BASE + 0x44
        body = ["lui $t6, %%hi(D_%08X)" % x, "lw $t6, %%lo(D_%08X)($t6)" % x,
                "lui $at, %%hi(D_%08X)" % x, "addiu $t7, $t6, 1", "sw $t7, %%lo(D_%08X)($at)" % x,
                "lui $t8, %%hi(D_%08X)" % y, "lw $t8, %%lo(D_%08X)($t8)" % y,
                "addu $t9, $t8, $t6", "lui $at, %%hi(D_%08X)" % y, "sw $t9, %%lo(D_%08X)($at)" % y]
        self.assertEqual(run(body).pairs, [])

    def test_label_ends_the_block(self):
        r = run(rmw(BASE + 0x40, "$t6", "$t7") + [".L80010100:"] + rmw(BASE + 0x44, "$t8", "$t9"))
        self.assertEqual(r.pairs, [])

    def test_same_word_is_not_a_pair(self):
        r = run(rmw(BASE + 0x40, "$t6", "$t7", op="lbu", st="sb")
                + rmw(BASE + 0x41, "$t8", "$t9", op="lbu", st="sb"))
        self.assertEqual(r.pairs, [])


class BitFields(unittest.TestCase):
    def test_load_into_other_register_is_bitfield(self):
        r = run(["lui $t9, %hi(D_80100081)", "lbu $t0, %lo(D_80100081)($t9)",
                 "lui $at, %hi(D_80100081)", "ori $t1, $t0, 0x40", "sb $t1, %lo(D_80100081)($at)"])
        self.assertEqual(r.bitfields, [BASE + 0x81])

    def test_scalar_or_is_not_bitfield(self):
        r = run(["lui $t9, %hi(D_80100081)", "lbu $t9, %lo(D_80100081)($t9)",
                 "lui $at, %hi(D_80100081)", "ori $t1, $t9, 0x40", "sb $t1, %lo(D_80100081)($at)"])
        self.assertEqual(r.bitfields, [])


class Tree(unittest.TestCase):
    def test_audit_tree_and_report(self):
        with tempfile.TemporaryDirectory() as root:
            os.makedirs(os.path.join(root, "include"))
            os.makedirs(os.path.join(root, "config"))
            with open(os.path.join(root, "include", "t.h"), "w") as f:
                f.write("typedef struct T {\n    /* 0x00 */ s32 a;\n    /* 0x04 */ u8 b[0xFC];\n} T;\n")
            with open(os.path.join(root, "config", "migrate_globals.txt"), "w") as f:
                f.write("aggregate D_80100000 T include/t.h\n")
            d = os.path.join(root, "asm", "nonmatchings", "main", "80010000")
            os.makedirs(d)
            with open(os.path.join(d, "f.s"), "w") as f:
                f.write("\n".join(asm("f", rmw(BASE, "$t6", "$t7") + rmw(BASE + 0x10, "$t8", "$t9",
                                                                       op="lbu", st="sb"))) + "\n")
            aggs, res = aa.audit_tree(root)
            _by, fields, objs, pairs, hoisted, bit = aa.summarize(aggs, res)
            self.assertEqual(sorted(k[1] for k in fields), ["a", "b"])
            self.assertEqual(objs["main/80010000"]["direct"], 4)
            self.assertEqual(pairs[("in", "kept")], 1)
            out = io.StringIO()
            with redirect_stdout(out):
                aa.main(["--root", root, "--fields"])
            self.assertIn("kept 1, hoisted 0", out.getvalue())


if __name__ == "__main__":
    unittest.main()
