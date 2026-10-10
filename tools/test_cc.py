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
