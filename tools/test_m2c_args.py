#!/usr/bin/env python3
"""Tests for tools/m2c_args.py and the callee-prototype / post-increment steps of tools/m2c.py (T-7030).
Synthetic asm only.

Run: tools/docker.sh python3 tools/test_m2c_args.py   (from the repo root)
"""
import os
import shutil
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import m2c  # noqa: E402
import m2c_args  # noqa: E402


def asm(*lines, name="f"):
    out = ["nonmatching %s, 0x40" % name, "", "glabel %s" % name]
    for i, line in enumerate(lines):
        out.append("    /* %X %08X 00000000 */  %s" % (i * 4, 0x80100000 + i * 4, line))
    out.append("endlabel %s" % name)
    return "\n".join(out) + "\n"


class Arity(unittest.TestCase):
    def test_reads_of_argument_registers_before_writes(self):
        self.assertEqual(m2c_args.arity(asm("addu $v0, $a0, $a1", "jr $ra", "nop")), 2)
        self.assertEqual(m2c_args.arity(asm("jr $ra", "move $v0, $a2")), 3)

    def test_a_register_written_first_is_not_a_parameter(self):
        self.assertEqual(m2c_args.arity(asm("li $a1, 3", "addu $v0, $a0, $a1", "jr $ra", "nop")), 1)

    def test_a_call_clobbers_the_argument_registers(self):
        self.assertEqual(m2c_args.arity(asm("jal g", "nop", "addu $v0, $a0, $a1", "jr $ra", "nop")), 0)

    def test_store_reads_its_source(self):
        self.assertEqual(m2c_args.arity(asm("sw $a1, 0($a0)", "jr $ra", "nop")), 2)

    def test_stack_parameter_above_the_frame(self):
        self.assertEqual(m2c_args.arity(asm("addiu $sp, $sp, -0x18", "lw $t6, 0x28($sp)", "jr $ra", "addiu $sp, $sp, 0x18")), 5)

    def test_no_code(self):
        self.assertIsNone(m2c_args.arity("nothing\n"))


LOOP = asm("lui $v1, %hi(D_800E738D)", "lbu $v1, %lo(D_800E738D)($v1)", "addiu $sp, $sp, -0x28",
           "sltiu $v0, $v1, 0x41", "bnez $v0, .L1", "nop", "lui $at, %hi(D_800E738D)", "addiu $t6, $v1, 0x1",
           "sb $t6, %lo(D_800E738D)($at)")
NEWVAL = asm("lui $v1, %hi(D_800E738D)", "lbu $v1, %lo(D_800E738D)($v1)", "nop", "addiu $t6, $v1, 0x1",
             "lui $at, %hi(D_800E738D)", "sb $t6, %lo(D_800E738D)($at)", "sltiu $v0, $t6, 0x41", "bnez $v0, .L1")


class PostIncrement(unittest.TestCase):
    def test_compare_of_the_old_register_is_a_post_increment(self):
        self.assertEqual(m2c_args.post_increment_old(LOOP), {"D_800E738D"})

    def test_compare_of_the_new_register_is_not(self):
        self.assertEqual(m2c_args.post_increment_old(NEWVAL), set())

    def test_no_store_back_is_not(self):
        text = asm("lbu $v1, %lo(D_1)($v1)", "sltiu $v0, $v1, 3", "addiu $t6, $v1, 1")
        self.assertEqual(m2c_args.post_increment_old(text), set())

    def test_rewrite_moves_the_increment_into_the_condition(self):
        draft = "void f(void) {\n    D_800E738D += 1;\n    if ((u8) D_800E738D >= 0x41U) {\n        g();\n    }\n}\n"
        new, done = m2c_args.rewrite_post_increment(draft, {"D_800E738D"})
        self.assertEqual(done, ["D_800E738D"])
        self.assertEqual(new, "void f(void) {\n    if ((u8) D_800E738D++ >= 0x41U) {\n        g();\n    }\n}\n")

    def test_rewrite_decrement_and_while_and_untouched_symbols(self):
        draft = "    D_1 -= 1;\n    while (D_1 != 0) {\n    }\n    D_2 += 1;\n    if (D_2 == 0) {\n"
        new, done = m2c_args.rewrite_post_increment(draft, {"D_1"})
        self.assertEqual(new, "    while (D_1-- != 0) {\n    }\n    D_2 += 1;\n    if (D_2 == 0) {\n")
        self.assertEqual(done, ["D_1"])

    def test_rewrite_leaves_other_statements_between_alone(self):
        draft = "    D_1 += 1;\n    h();\n    if (D_1 == 0) {\n"
        self.assertEqual(m2c_args.rewrite_post_increment(draft, {"D_1"}), (draft, []))


class CalleePrototypes(unittest.TestCase):
    def setUp(self):
        self.root = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.root)

    def put(self, rel, text):
        p = os.path.join(self.root, rel)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, "w") as f:
            f.write(text)

    def test_kr_and_undeclared_callees_get_the_count_of_their_asm(self):
        self.put("asm/nonmatchings/main/80041000/func_80041100.s", asm("addu $v0, $a0, $a1", "jr $ra", "nop", name="func_80041100"))
        self.put("asm/nonmatchings/main/80041000/func_80041200.s", asm("jr $ra", "move $v0, $a0", name="func_80041200"))
        caller = asm("jal func_80041100", "nop", "jal func_80041200", "nop", "jal func_80041300", "nop")
        ctx = "void func_80041100();\nvoid func_80041300(void);\n"
        out, counts = m2c.callee_prototypes(self.root, None, caller, ctx)
        self.assertEqual(counts, {"func_80041100": 2, "func_80041200": 1})
        self.assertIn("void func_80041100(s32, s32);", out)
        self.assertNotIn("func_80041100();", out)
        self.assertIn("s32 func_80041200(s32);", out)
        self.assertIn("void func_80041300(void);", out)      # a prototype stays

    def test_overlay_callee_and_library_file(self):
        self.put("asm/ovl/AAA/nonmatchings/AAA/80132000/func_80132100.s",
                 asm("jr $ra", "move $v0, $a3", name="func_80132100"))
        self.put("asm/libapi_1.s", asm("move $v0, $a0", "jr $ra", "nop", name="abs") + asm("jr $ra", "nop", name="other"))
        caller = asm("jal func_80132100", "nop", "jal abs", "nop")
        out, counts = m2c.callee_prototypes(self.root, "AAA", caller, "")
        self.assertEqual(counts, {"func_80132100": 4, "abs": 1})

    def test_callee_without_parameters_is_left_to_m2c(self):
        self.put("asm/nonmatchings/main/80041000/func_80041100.s", asm("jr $ra", "nop", name="func_80041100"))
        out, counts = m2c.callee_prototypes(self.root, None, asm("jal func_80041100", "nop"), "void func_80041100();\n")
        self.assertEqual(counts, {})
        self.assertIn("func_80041100();", out)


if __name__ == "__main__":
    unittest.main()
