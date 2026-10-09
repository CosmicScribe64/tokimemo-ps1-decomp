"""Unit tests for the setup logic of tools/m2c.py (synthetic files only, no game data).

Run: tools/docker.sh python3 tools/test_m2c.py   (from the repo root)
"""
import os
import tempfile
import unittest

import m2c

RODATA = """.include "macro.inc"

.section .rodata, "a"

.align 3
nonmatching jtbl_80010000

dlabel jtbl_80010000
    /* 0 80010000 */ .word .L80000010
    /* 4 80010004 */ .word .L80000020
enddlabel jtbl_80010000

nonmatching D_80010100

dlabel D_80010100
    /* 100 80010100 */ .asciz "abc"
enddlabel D_80010100

dlabel D_80010200
    /* 200 80010200 */ .word 0x801B13A4
enddlabel D_80010200

dlabel jtbl_80010300
    /* 300 80010300 */ .word .L80000030
enddlabel jtbl_80010300
"""


def touch(root, rel, text=""):
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(text)
    return path


class Locate(unittest.TestCase):
    def test_main_and_overlay(self):
        with tempfile.TemporaryDirectory() as root:
            a = touch(root, "asm/nonmatchings/main/80041000/func_80041000.s")
            b = touch(root, "asm/ovl/TT/matchings/TT/func_80133BA4.s")
            self.assertEqual(m2c.locate(root, "func_80041000"), (a, None))
            self.assertEqual(m2c.locate(root, "func_80133BA4"), (b, "TT"))

    def test_missing(self):
        with tempfile.TemporaryDirectory() as root:
            with self.assertRaises(LookupError):
                m2c.locate(root, "func_1")


class Context(unittest.TestCase):
    def test_overlay_header_only_if_present(self):
        with tempfile.TemporaryDirectory() as root:
            self.assertEqual(m2c.context_sources(root, "TT"), ["include/game.h"])
            touch(root, "include/ovl/TT.h")
            self.assertEqual(m2c.context_sources(root, "TT"),
                             ["include/game.h", "include/ovl/TT.h"])
            self.assertEqual(m2c.context_sources(root, None), ["include/game.h"])

    def test_context_text_includes_relative_to_include_dir(self):
        self.assertEqual(m2c.context_text(["include/game.h", "include/ovl/TT.h"]),
                         '#include "game.h"\n#include "ovl/TT.h"\n')

    def test_drop_declaration_only_that_symbol(self):
        text = "void func_1(s32 a);\nvoid func_10(void);\nextern s32 D_1;\n"
        self.assertEqual(m2c.drop_declaration(text, "func_1"),
                         "\nvoid func_10(void);\nextern s32 D_1;\n")


class Rodata(unittest.TestCase):
    def test_keeps_referenced_jtbl_and_strings_only(self):
        func = "lw $t7, %lo(jtbl_80010000)($at)\nlui $a0, %hi(D_80010100)\nlw $a1, D_80010200\n"
        out = m2c.rodata_blocks(RODATA, func)
        self.assertTrue(out.startswith(".section .rodata\n"))
        self.assertIn("glabel jtbl_80010000", out)
        self.assertIn('.asciz "abc"', out)
        self.assertNotIn("dlabel", out)
        self.assertNotIn("D_80010200", out)      # data word: m2c would fold it into the code
        self.assertNotIn("jtbl_80010300", out)   # not referenced

    def test_no_reference_gives_header_only(self):
        self.assertEqual(m2c.rodata_blocks(RODATA, "nop"), ".section .rodata\n")


class Draft(unittest.TestCase):
    def test_split_at_definition(self):
        text = "? func_2();   /* extern */\nu32 D_1 = 1;\n\nvoid func_1(void) {\n    func_2();\n}\n"
        decls, body = m2c.split_draft(text, "func_1")
        self.assertEqual(decls, "? func_2();   /* extern */\nu32 D_1 = 1;")
        self.assertEqual(body, "void func_1(void) {\n    func_2();\n}\n")

    def test_no_definition_returns_text(self):
        self.assertEqual(m2c.split_draft("junk\n", "func_1"), ("", "junk\n"))


if __name__ == "__main__":
    unittest.main()
