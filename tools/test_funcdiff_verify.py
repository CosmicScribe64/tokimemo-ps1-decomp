"""Synthetic literal-byte/link/guard tests for T-10840; no game data."""
import os
from pathlib import Path
import struct
import subprocess
import tempfile
from types import SimpleNamespace
import unittest

import funcdiff_verify as proof


class LiteralProof(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        old = os.getcwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, old)
        self.name = "func_80041000"
        self.c = SimpleNamespace(src=Path("src/main/80041000.c"), obj="build/source.o", unit="main")
        for folder in ("src/main", "asm/matchings/main/80041000", "config", "build", "proof"):
            Path(folder).mkdir(parents=True)
        self.c.src.write_text("int func_80041000(void) { return 1; }\n")
        Path("asm/matchings/main/80041000/func_80041000.s").write_text(
            "/* 0 80041000 0800E003 */ jr $ra\n/* 4 80041004 01000224 */ addiu $v0,$zero,1\n"
            "endlabel func_80041000\n")
        Path("target.bin").write_bytes(struct.pack("<II", 0x03e00008, 0x24020001))
        Path("config/SLPM_86.053.yaml").write_text("options:\n  target_path: target.bin\n")
        Path("build/SLPM_86.053.ld").write_text(
            "SECTIONS { .main 0x80041000 : { build/source.o(.text) } }\n")
        for p in ("build/undefined_funcs_auto.txt", "build/undefined_syms_auto.txt",
                  "build/SLPM_86.053_data_syms.ld"):
            Path(p).write_text("")

    def object(self, body):
        Path("code.s").write_text(".set noreorder\n.text\n.globl %s\n.type %s,@function\n%s:\n%s\n.size %s,.-%s\n" %
                                 (self.name, self.name, self.name, body, self.name, self.name))
        subprocess.run(["mips-linux-gnu-as", "-EL", "-o", "build/source.o", "code.s"], check=True)
        return "build/source.o"

    def check(self, body):
        return proof.exact_match(self.object(body), self.c, self.name, Path("proof"), build=False)

    def test_exact_linked_bytes_match_immutable_target(self):
        self.assertEqual(self.check("jr $ra\naddiu $v0,$zero,1"),
                         (True, "linked function bytes equal target (8 bytes)"))

    def test_different_word_cannot_match(self):
        equal, why = self.check("jr $ra\naddiu $v0,$zero,2")
        self.assertFalse(equal)
        self.assertIn("bytes differ", why)

    def test_extra_instruction_cannot_match(self):
        equal, why = self.check("jr $ra\naddiu $v0,$zero,1\nnop")
        self.assertFalse(equal)
        self.assertIn("span differs", why)

    def test_wrong_linked_address_cannot_match(self):
        Path("build/SLPM_86.053.ld").write_text(
            "SECTIONS { .main 0x80041004 : { build/source.o(.text) } }\n")
        self.assertFalse(self.check("jr $ra\naddiu $v0,$zero,1")[0])

    def test_failed_link_cannot_match(self):
        Path("build/SLPM_86.053.ld").write_text("broken build/source.o(\n")
        with self.assertRaisesRegex(ValueError, "failed"):
            self.check("jr $ra\naddiu $v0,$zero,1")

    def test_noncontiguous_original_span_is_refused(self):
        asm = Path("asm/matchings/main/80041000/func_80041000.s")
        asm.write_text(asm.read_text().replace("/* 4", "/* 8"))
        with self.assertRaisesRegex(ValueError, "noncontiguous"):
            proof.target_span(self.c, self.name)


class FullDraft(unittest.TestCase):
    def test_preprocessor_keeps_data_helpers_and_selects_c(self):
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / "source.c"
            source.write_text('#include "common.h"\nstatic s32 data[2] = { 1, 2 };\n'
                              's32 helper(s32 x) { return data[x]; }\n'
                              '#ifdef NON_MATCHING\ns32 draft(void) { return helper(1); }\n'
                              '#else\nINCLUDE_ASM("asm", draft);\n#endif\n'
                              'INCLUDE_RODATA("asm", island);\nINCLUDE_ASM("asm", other);\n')
            pre = proof.preprocess_draft(source)
            self.assertIn("static s32 data[2] = { 1, 2 }", pre)
            self.assertIn("return data[x]", pre)
            self.assertIn("return helper(1)", pre)
            self.assertIn('INCLUDE_ASM("asm",other)', pre.replace(" ", ""))
            self.assertNotIn('INCLUDE_ASM("asm",draft)', pre.replace(" ", ""))
            self.assertIn('INCLUDE_RODATA("asm",island)', pre.replace(" ", ""))


if __name__ == "__main__":
    unittest.main()
