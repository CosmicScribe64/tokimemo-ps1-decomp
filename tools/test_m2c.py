"""Unit tests for the setup logic of tools/m2c.py (synthetic files only, no game data).

Run: tools/docker.sh python3 tools/test_m2c.py   (from the repo root)
"""
import os
import shutil
import subprocess
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

    def shared(self, root, with_include_asm):
        """func_80132000 in TT and ETC; `with_include_asm` units still hold it as INCLUDE_ASM."""
        paths = {}
        for unit in ("TT", "ETC"):
            paths[unit] = touch(root, "asm/ovl/%s/nonmatchings/%s/80132000/func_80132000.s" % (unit, unit))
        for unit in with_include_asm:
            touch(root, "src/ovl/%s.c" % unit,
                  '#include "game.h"\nINCLUDE_ASM("asm/ovl/%s/nonmatchings/%s", func_80132000);\n' % (unit, unit))
        return paths

    def test_ambiguous_name_is_refused(self):
        with tempfile.TemporaryDirectory() as root:
            self.shared(root, [])
            with self.assertRaises(m2c.AmbiguousError) as cm:
                m2c.locate(root, "func_80132000")
            self.assertIn("ETC, TT", str(cm.exception))
            self.assertIn("--unit", str(cm.exception))

    def test_ambiguous_when_both_units_still_hold_it(self):
        with tempfile.TemporaryDirectory() as root:
            self.shared(root, ["TT", "ETC"])
            with self.assertRaises(m2c.AmbiguousError):
                m2c.locate(root, "func_80132000")

    def test_infers_unit_from_include_asm(self):
        with tempfile.TemporaryDirectory() as root:
            paths = self.shared(root, ["TT"])
            # per-object layout: src/ovl/<NAME>/<addr>.c
            self.assertEqual(m2c.locate(root, "func_80132000"), (paths["TT"], "TT"))

    def test_infers_unit_from_per_object_c_file(self):
        with tempfile.TemporaryDirectory() as root:
            paths = self.shared(root, [])
            touch(root, "src/ovl/ETC/80132000.c",
                  'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132000);\n')
            self.assertEqual(m2c.locate(root, "func_80132000"), (paths["ETC"], "ETC"))

    def test_explicit_unit_wins_over_inference(self):
        with tempfile.TemporaryDirectory() as root:
            paths = self.shared(root, ["TT"])
            self.assertEqual(m2c.locate(root, "func_80132000", unit="ETC"), (paths["ETC"], "ETC"))
            self.assertEqual(m2c.locate(root, "ETC:func_80132000"), (paths["ETC"], "ETC"))

    def test_unit_from_c_path_and_asm_path(self):
        with tempfile.TemporaryDirectory() as root:
            paths = self.shared(root, [])
            self.assertEqual(m2c.locate(root, "func_80132000", unit="src/ovl/ETC.c")[1], "ETC")
            self.assertEqual(m2c.locate(root, "func_80132000", unit="src/ovl/TT/80132000.c")[1], "TT")
            self.assertEqual(m2c.locate(root, paths["TT"])[1], "TT")

    def test_unit_without_the_function(self):
        with tempfile.TemporaryDirectory() as root:
            self.shared(root, [])
            with self.assertRaises(LookupError) as cm:
                m2c.locate(root, "func_80132000", unit="OLH")
            self.assertIn("ETC, TT", str(cm.exception))

    def test_unit_of_path(self):
        self.assertEqual(m2c.unit_of_path("src/ovl/EVENT.c"), "EVENT")
        self.assertEqual(m2c.unit_of_path("/x/src/ovl/EVENT/800F9680.c"), "EVENT")
        self.assertEqual(m2c.unit_of_path("asm/ovl/TT/matchings/TT/f.s"), "TT")
        self.assertEqual(m2c.unit_of_path("src/main/80041000.c"), "main")
        self.assertEqual(m2c.unit_of_path("asm/nonmatchings/main/80041000/f.s"), "main")
        self.assertIsNone(m2c.unit_of_path("tools/m2c.py"))


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


LO_LITERAL = """glabel f
    /* 0 0 */  lui        $t9, (0x801D2000 >> 16)
    /* 4 4 */  lh         $t0, %lo(D_801D63C8)($t9)
    /* 8 8 */  lui        $at, %hi(D_80120BFC)
    /* c c */  sh         $t0, %lo(D_80120BFC)($at)
    /* 10 10 */  lui        $t1, (0x801E0000 >> 16)
    /* 14 14 */  lb         $t2, %lo(D_801D8010)($t1)
    /* 18 18 */  addiu      $a0, $t1, %lo(D_801D8010 + 0x4)
    /* 1c 1c */  lui        $t3, %hi(D_801D63C8)
    /* 20 20 */  lw         $t4, %lo(D_801D63C8)($t3)
    /* 24 24 */  lw         $t4, %lo(D_8012000C)($t1)
    /* 28 28 */  lw         $t4, %lo(bg_read_sub2)($t1)
"""


class LoLiteral(unittest.TestCase):
    """m2c turns an unpaired %lo into 0 (T-3300): the wrapper writes the literal low half."""

    def test_rewrite(self):
        out = m2c.fix_lo_literals(LO_LITERAL).splitlines()
        self.assertIn("lh         $t0, 0x63C8($t9)", out[2])
        self.assertIn("sh         $t0, %lo(D_80120BFC)($at)", out[4])      # %hi/%lo pair kept
        self.assertIn("lb         $t2, -0x7FF0($t1)", out[6])              # signed low half
        self.assertIn("addiu      $a0, $t1, -0x7FEC", out[7])              # addiu with + offset
        self.assertIn("%lo(D_801D63C8)($t3)", out[9])                      # base from %hi of the symbol
        self.assertIn("lw         $t4, 0xC($t1)", out[10])
        self.assertIn("%lo(bg_read_sub2)", out[11])                        # no address in the name

    def test_text_without_lo_is_unchanged(self):
        text = "glabel f\n    jr $ra\n    nop\n"
        self.assertEqual(m2c.fix_lo_literals(text), text)

    @unittest.skipUnless(shutil.which("m2c"), "m2c is only in the Docker image")
    def test_m2c_prints_the_full_address(self):
        text = LO_LITERAL.split("    /* 1c")[0] + (
            "    sb         $t2, %lo(D_80120BFE)($at)\n    jr         $ra\n    nop\n")
        with tempfile.TemporaryDirectory() as tmp:
            path = touch(tmp, "f.s", text)
            raw = subprocess.run(["m2c", "-t", "mipsel-ido-c", path], text=True,
                                 stdout=subprocess.PIPE).stdout
            self.assertIn("0x801D0000", raw)          # the upstream bug, still there
            with open(path, "w") as f:
                f.write(m2c.fix_lo_literals(text))
            fixed = subprocess.run(["m2c", "-t", "mipsel-ido-c", path], text=True,
                                   stdout=subprocess.PIPE).stdout
            self.assertIn("*(s16 *)0x801D63C8", fixed)
            self.assertIn("0x801D8010", fixed)         # lui 0x801E + lb -0x7FF0


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
