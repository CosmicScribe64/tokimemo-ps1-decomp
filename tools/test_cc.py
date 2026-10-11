"""Unit tests for tools/cc.py: pad_text (T-0012) on synthetic objects, and the IDO flag set
(T-0017) on synthetic C snippets compiled by the real IDO 5.3 (no game data).

Run from tools/: python3 test_cc.py   (inside Docker: tools/docker.sh sh -c 'cd tools && python3 test_cc.py')
"""
import os
import subprocess
import tempfile
import unittest

import cc


def build(words, tmp):
    """Assemble `words` nops plus a jal to an external symbol; return (object path)."""
    src = os.path.join(tmp, "t.s")
    obj = os.path.join(tmp, "t.o")
    with open(src, "w") as f:
        f.write('.section .text, "ax"\nglabel_start:\njal ext\nnop\n' + "nop\n" * (words - 2))
    subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o", obj, src], check=True)
    return obj


def text_size(obj):
    out = subprocess.run(["mips-linux-gnu-readelf", "-S", "-W", obj], check=True,
                         stdout=subprocess.PIPE, text=True).stdout
    for line in out.splitlines():
        if " .text " in line:
            return int(line.split(".text")[1].split()[3], 16)
    raise AssertionError("no .text")


class PadText(unittest.TestCase):
    def test_pads_to_multiple_of_16_with_zeros(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = build(5, tmp)                       # 20 bytes
            cc.pad_text(obj)
            self.assertEqual(text_size(obj), 32)
            raw = os.path.join(tmp, "raw")
            subprocess.run(["mips-linux-gnu-objcopy", "--dump-section", ".text=" + raw, obj, os.devnull], check=True)
            self.assertEqual(open(raw, "rb").read()[20:], b"\0" * 12)

    def test_aligned_object_unchanged(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = build(8, tmp)                       # 32 bytes
            before = open(obj, "rb").read()
            cc.pad_text(obj)
            self.assertEqual(open(obj, "rb").read(), before)

    def test_relocation_survives(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = build(3, tmp)
            cc.pad_text(obj)
            out = subprocess.run(["mips-linux-gnu-objdump", "-r", obj], check=True,
                                 stdout=subprocess.PIPE, text=True).stdout
            self.assertIn("ext", out)


def ido_disasm(code, tmp):
    """Compile C `code` with cc.compile_ido (IDO 5.3, project flags); return `objdump -dr` text."""
    src = os.path.join(tmp, "t.c")
    obj = os.path.join(tmp, "t.o")
    with open(src, "w") as f:
        f.write(code)
    cc.compile_ido(src, obj, "5.3")
    return subprocess.run(["mips-linux-gnu-objdump", "-dr", obj], check=True,
                          stdout=subprocess.PIPE, text=True).stdout


class IdoFlags(unittest.TestCase):
    """-Wo,-nokpicopt (T-0017): no address register for a directly accessed global, but
    integer constants stay in registers (hoisted loop bound, multu by a register)."""

    def test_global_read_modify_write_reloads_hi_lo(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = ido_disasm("extern int G;\nint f(void) { G += 0x377; return G; }\n", tmp)
        self.assertEqual(out.count("R_MIPS_HI16\tG"), 2)   # lui for the load, lui at for the store
        self.assertNotIn("addiu", out.split("R_MIPS_HI16")[1].split("R_MIPS_LO16")[0])

    def test_loop_bound_constant_hoisted(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = ido_disasm("void g(int);\nvoid f(void) { int i; for (i = 0; i < 11; i++) g(i); }\n", tmp)
        self.assertRegex(out, r"li\ts[0-7],11")

    def test_repeated_constant_multiplier_stays_in_register(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = ido_disasm("int f(int a, int b) { return a * 0x44 + b * 0x44; }\n", tmp)
        self.assertEqual(out.count("multu"), 2)


class KandRPromotion(unittest.TestCase):
    """-cckr (T-7000): unsigned char/short operands promote to unsigned int, as in the original."""

    def test_unsigned_char_compare_divide_shift_are_unsigned(self):
        code = ("extern unsigned char D; extern int R;\n"
                "void f(void) { if (D < 10) R = D % 0x30 + (D >> 2); }\n")
        with tempfile.TemporaryDirectory() as tmp:
            out = ido_disasm(code, tmp)
        self.assertIn("sltiu", out)
        self.assertIn("divu", out)
        self.assertIn("srl", out)
        self.assertNotIn("slti\t", out)
        self.assertNotIn("sra", out)

    def test_narrow_parameter_copied_back_to_its_register(self):
        code = "void g();\nvoid f(int a, unsigned char d) { if (d > 8) d = 8; g(a, d); }\n"
        with tempfile.TemporaryDirectory() as tmp:
            out = ido_disasm(code, tmp)
        self.assertRegex(out, r"andi\tt6,a1,0xff")
        self.assertRegex(out, r"move\ta1,t6")


def gcc_disasm(code, tmp, ver):
    src = os.path.join(tmp, "g.c")
    obj = os.path.join(tmp, "g.o")
    with open(src, "w") as f:
        f.write(code)
    cc.compile_gcc(src, obj, ver, "2.79")
    return subprocess.run(["mips-linux-gnu-objdump", "-d", "-M", "no-aliases", obj], check=True,
                          stdout=subprocess.PIPE, text=True).stdout


@unittest.skipUnless(os.path.exists("/opt/gcc/2.7.2-psx/cc1") and os.path.exists("/opt/gcc/2.8.1-psx/cc1"),
                     "old gcc not installed")
class GccPath(unittest.TestCase):
    """The PsyQ gcc path (T-9200) on synthetic C; both versions run natively on arm64 too."""

    def test_division_has_aspsx_checks(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = gcc_disasm("int f(int a, int b) { return a / b; }\n", tmp, "2.7.2-psx")
        self.assertIn("break\t0x7", out)
        self.assertIn("break\t0x6", out)

    def test_move_is_addu_zero(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = gcc_disasm("int *f(int *a) { return a; }\n", tmp, "2.7.2-psx")
        self.assertRegex(out, r"addu\tv0,a0,zero")

    def test_28_epilogue_releases_the_stack_in_the_jr_slot(self):
        code = "void g(int);\nvoid f(int *p) { g(p[0]); g(p[1]); }\n"
        for ver, in_slot in (("2.7.2-psx", False), ("2.8.1-psx", True)):
            with tempfile.TemporaryDirectory() as tmp:
                lines = [l for l in gcc_disasm(code, tmp, ver).splitlines() if "\t" in l]
            jr = [i for i, l in enumerate(lines) if "\tjr\t" in l][0]
            self.assertEqual("addiu\tsp,sp" in lines[jr + 1], in_slot, ver)


class IncludeDeps(unittest.TestCase):
    def test_include_asm_and_include_rodata_are_dependencies(self):
        text = 'INCLUDE_ASM("a/b", f1);\nINCLUDE_RODATA("a/c", D_1);\nint x;\n'
        self.assertEqual(cc.INCLUDE_ASM_RE.findall(text), [("a/b", "f1"), ("a/c", "D_1")])


class SjisLiteralTest(unittest.TestCase):
    def test_string_literals_become_octal_escapes(self):
        src = 'f("図", \'a\'); /* 図 */ g("x\\"y");\n'
        self.assertEqual(cc.sjis_literals(src), 'f("\\220\\175", \'a\'); /* 図 */ g("x\\"y");\n')

    def test_second_byte_backslash_is_escaped(self):
        # 表 is 0x95 0x5C in Shift-JIS: the 0x5C must not end up as a raw backslash
        self.assertEqual(cc.sjis_literals('"表"'), '"\\225\\134"')


if __name__ == "__main__":
    unittest.main()
