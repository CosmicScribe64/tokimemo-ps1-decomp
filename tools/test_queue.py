#!/usr/bin/env python3
"""Tests for tools/queue.py with synthetic splat-style asm (no game data).

Run: tools/docker.sh python3 tools/test_queue.py
"""
import importlib.util
import io
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("work_queue", HERE / "queue.py")
wq = importlib.util.module_from_spec(spec)
import sys
sys.path.insert(0, str(HERE))
spec.loader.exec_module(wq)


def asm(name, body):
    """Splat-style .s text. body: list of strings, each an instruction or a `.Lxxxxxxxx:` label."""
    out = ["nonmatching %s, 0x0" % name, "", "glabel %s" % name]
    addr = 0x80010000
    for line in body:
        if line.startswith("."):
            out.append("  " + line)
            continue
        out.append("    /* %X %08X 00000000 */  %s" % (addr - 0x80000000, addr, line))
        addr += 4
    out.append("endlabel " + name)
    return "\n".join(out) + "\n"


def at(n):
    return ".L%08X:" % (0x80010000 + 4 * n)


def lbl(n):
    return ".L%08X" % (0x80010000 + 4 * n)


# the T-0018 shape: v1 loaded in three blocks, compare chain, increments after the call
PROMOTED = [
    "lui        $v1, %hi(D_800E738D)",
    "lbu        $v1, %lo(D_800E738D)($v1)",
    "addiu      $sp, $sp, -0x28",
    "bnez       $v1, " + lbl(15),
    " sw         $ra, 0x14($sp)",
    "jal        func_80044E8C",
    " nop",
    "lui        $v1, %hi(D_800E738D)",
    "lbu        $v1, %lo(D_800E738D)($v1)",
    "lui        $at, %hi(D_800E738D)",
    "addiu      $t6, $v1, 0x1",
    "b          " + lbl(15),
    " sb         $t6, %lo(D_800E738D)($at)",
    "nop",
    "nop",
    at(15),
    "addiu      $at, $zero, 0x1",
    "bne        $v1, $at, " + lbl(19),
    " nop",
    "jr         $ra",
    " nop",
]


class Detector(unittest.TestCase):
    def test_reload_in_two_blocks_same_register_is_R(self):
        f = wq.analyze(asm("f", PROMOTED))
        self.assertTrue(f.reload)

    def test_dispatch_on_v1_is_V(self):
        f = wq.analyze(asm("f", PROMOTED))
        self.assertTrue(f.dispatch)

    def test_single_v1_use_is_not_V(self):
        # func_80042400 shape: v1 loaded, used once, v0 is the result
        body = ["lui $v1, %hi(D_1)", "lw $v1, %lo(D_1)($v1)", "lui $at, %hi(D_1)", "addiu $v0, $v1, 0x377",
                "jr $ra", " sw $v0, %lo(D_1)($at)"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)
        self.assertFalse(f.reload)

    def test_v0_load_is_not_flagged(self):
        body = ["lui $v0, %hi(D_1)", "lbu $v0, %lo(D_1)($v0)", "bnez $v0, " + lbl(7), " nop",
                "addiu $at, $zero, 1", "beq $v0, $at, " + lbl(7), " nop", at(7), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)

    def test_v1_with_live_v0_is_not_V(self):
        # a call result in v0 is still read after the load: IDO has to use v1 (matched shape)
        body = ["jal func_8005742C", " nop", "lui $v1, %hi(D_1)", "lbu $v1, %lo(D_1)($v1)",
                "bne $v0, $v1, " + lbl(8), " nop", "beq $v1, $zero, " + lbl(8), " nop", at(8), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)

    def test_array_index_load_is_not_a_global_load(self):
        body = ["sll $t6, $a0, 6", "lui $v1, %hi(D_1)", "addu $v1, $v1, $t6", "lbu $v1, %lo(D_1)($v1)",
                "bnez $v1, " + lbl(8), " nop", "beqz $v1, " + lbl(8), " nop", at(8), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)
        self.assertFalse(f.reload)

    def test_argument_register_reload_is_not_R(self):
        body = ["lui $a0, %hi(D_1)", "lw $a0, %lo(D_1)($a0)", "jal f2", " nop",
                "lui $a0, %hi(D_1)", "lw $a0, %lo(D_1)($a0)", "jal f2", " nop", "jr $ra", " nop"]
        self.assertFalse(wq.analyze(asm("f", body)).reload)

    def test_reload_inside_loop_is_not_R(self):
        body = ["lui $v0, %hi(D_1)", at(1), "lbu $v0, %lo(D_1)($v0)", "jal g", " nop", "lui $v0, %hi(D_1)",
                "lbu $v0, %lo(D_1)($v0)", "bnez $v0, " + lbl(1), " nop", "jr $ra", " nop"]
        # the first load is outside the loop label? it is in a loop (label at 1 precedes both lbu)
        self.assertFalse(wq.analyze(asm("f", body)).reload)

    def test_different_registers_is_not_R(self):
        body = ["lui $t6, %hi(D_1)", "lw $t6, %lo(D_1)($t6)", "jal g", " nop",
                "lui $t7, %hi(D_1)", "lw $t7, %lo(D_1)($t7)", "jr $ra", " nop"]
        self.assertFalse(wq.analyze(asm("f", body)).reload)

    def test_jump_table_loop_calls_leaf(self):
        body = ["lui $at, %hi(jtbl_800AF7F0)", "lw $t8, %lo(jtbl_800AF7F0)($at)", at(2), "addiu $a0, $a0, -1",
                "bnez $a0, " + lbl(2), " nop", "jal g", " nop", "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertTrue(f.jtbl)
        self.assertTrue(f.loop)
        self.assertEqual(f.calls, 1)
        self.assertEqual(f.size, 36)

    def test_forward_branch_is_no_loop(self):
        body = ["beqz $a0, " + lbl(4), " nop", "addiu $a0, $a0, 1", at(4), "jr $ra", " nop"]
        self.assertFalse(wq.analyze(asm("f", body)).loop)

    def test_string_reference(self):
        body = ["lui $a0, %hi(D_800AF340)", "addiu $a0, $a0, %lo(D_800AF340)", "jr $ra", " nop"]
        self.assertTrue(wq.analyze(asm("f", body), {"D_800AF340"}).strings)
        self.assertFalse(wq.analyze(asm("f", body), set()).strings)

    def test_trailing_single_nop(self):
        body = ["jr $ra", " nop", "nop"]
        self.assertTrue(wq.analyze(asm("f", ["addiu $v0, $zero, 1"] + body)).pad)
        self.assertFalse(wq.analyze(asm("f", ["addiu $v0, $zero, 1", "jr $ra", " nop"])).pad)


def func(file, name, size, calls=0, flags=""):
    f = wq.Facts(size, calls, "L" in flags, "J" in flags, "S" in flags, "P" in flags, "R" in flags, "V" in flags)
    return wq.Func(file, name, "x.s", f)


class Ranking(unittest.TestCase):
    def test_unblocked_leaf_first_then_size(self):
        a = func("F", "a", 100)
        b = func("F", "b", 8, calls=1)
        c = func("F", "c", 4, flags="V")
        d = func("F", "d", 12)
        self.assertEqual([f.name for f in sorted([a, b, c, d], key=wq.rank_key)], ["d", "a", "b", "c"])

    def test_select_by_file(self):
        fs = [func("80041000", "a", 4), func("TEL", "b", 4), func("TT", "c", 4)]
        self.assertEqual([f.name for f in wq.select(fs, ["tel", "800410"])], ["a", "b"])
        self.assertEqual(len(wq.select(fs, [])), 3)

    def test_blocker_flags(self):
        self.assertTrue(func("F", "a", 4, flags="R").blocked)
        self.assertFalse(func("F", "a", 4, flags="L").blocked)


class Data(unittest.TestCase):
    def test_cases_table(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "c.md"
            p.write_text("text\n\n| file | function | category | symptom |\n|---|---|---|---|\n"
                         "| TEL | `func_8013A40C` | promo | chain |\n| 8005A0B0 | `GetWorkBase` | regorder | x |\n")
            self.assertEqual(wq.read_cases(p), [("TEL", "func_8013A40C", "promo", "chain"),
                                                ("8005A0B0", "GetWorkBase", "regorder", "x")])
            self.assertEqual(wq.read_cases(Path(d) / "none.md"), [])

    def test_dupes_groups(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "dupes.txt"
            p.write_text("func_80010000 func_80020000 func_80030000\nfunc_80040000\n")
            self.assertEqual(wq.read_dupes(p), {"func_80020000": "func_80010000", "func_80030000": "func_80010000"})

    def test_table_output(self):
        out = io.StringIO()
        wq.print_table([func("TEL", "func_1", 8, flags="LR")], {"func_1": "func_0"}, out)
        self.assertIn("LR", out.getvalue())
        self.assertIn("=func_0", out.getvalue())


class Project(unittest.TestCase):
    """load() over a tiny synthetic tree."""

    def test_load_remaining_and_matched(self):
        with tempfile.TemporaryDirectory() as d:
            r = Path(d)
            (r / "src/main").mkdir(parents=True)
            (r / "src/ovl").mkdir(parents=True)
            (r / "src/main/80041000.c").write_text(
                'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041000);\n'
                "void func_80041010(void) {\n}\n")
            nm = r / "asm/nonmatchings/main/80041000"
            nm.mkdir(parents=True)
            (nm / "func_80041000.s").write_text(asm("func_80041000", ["jr $ra", " nop"]))
            mt = r / "asm/matchings/main/80041000"
            mt.mkdir(parents=True)
            (mt / "func_80041010.s").write_text(asm("func_80041010", ["jr $ra", " nop"]))
            remaining, matched = wq.load(r)
            self.assertEqual([(f.file, f.name, f.facts.size) for f in remaining], [("80041000", "func_80041000", 8)])
            self.assertEqual([f.name for f in matched], ["func_80041010"])


if __name__ == "__main__":
    unittest.main()
