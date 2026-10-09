"""Unit tests for tools/trailing_pad.py (T-1310) on synthetic objects (no game data).

Run from tools/: python3 test_trailing_pad.py
(inside Docker: tools/docker.sh sh -c 'cd tools && python3 test_trailing_pad.py')
"""
import os
import subprocess
import tempfile
import unittest

import trailing_pad as tp

PREAMBLE = ".set noreorder\n.section .text,\"ax\"\n"

FUNCS = """
.globl f1
.type f1,@function
f1:
    jr $ra
    nop
.size f1, .-f1
%(pad1)s
.globl f2
.type f2,@function
f2:
    jal f3
    nop
    jal .Lloc
    nop
    lui $a0, %%hi(ext)
    addiu $a0, $a0, %%lo(ext)
    jr $ra
    nop
.size f2, .-f2
%(pad2)s
.globl f3
.type f3,@function
f3:
    beqz $a0, 1f
    nop
    nop
1:  jr $ra
    nop
.Lloc:
    jr $ra
    nop
.size f3, .-f3
"""

DATA = '.data\n.word .Lloc\n.word f3\n'


def source(n1, n2, data=DATA, extra=""):
    return PREAMBLE + FUNCS % {"pad1": "nop\n" * n1, "pad2": "nop\n" * n2} + extra + data


def rd(path):
    with open(path, "rb") as f:
        return f.read()


def assemble(tmp, name, text):
    s = os.path.join(tmp, name + ".s")
    o = os.path.join(tmp, name + ".o")
    with open(s, "w") as f:
        f.write(text)
    subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o", o, s], check=True)
    return o


def link(tmp, obj, name):
    out = os.path.join(tmp, name + ".elf")
    subprocess.run(["mips-linux-gnu-ld", "-EL", "-Ttext=0x80100000", "-Tdata=0x80200000",
                    "--defsym", "ext=0x80300000", "-e", "f1", "-o", out, obj], check=True)
    parts = []
    for sec in (".text", ".data"):
        raw = os.path.join(tmp, name + sec)
        subprocess.run(["mips-linux-gnu-objcopy", "--dump-section", sec + "=" + raw, out, os.devnull],
                       check=True)
        with open(raw, "rb") as f:
            parts.append(f.read())
    # gas pads .text to 16 and the pass inserts before that padding; only trailing zeros differ
    parts[0] = parts[0].rstrip(b"\0")
    return parts


def pad_object(tmp, name, text, pads):
    obj = assemble(tmp, name, text)
    data = tp.insert_zero_words(rd(obj), pads)
    with open(obj, "wb") as f:
        f.write(data)
    return obj


class InsertZeroWords(unittest.TestCase):
    def check(self, n1, n2):
        with tempfile.TemporaryDirectory() as tmp:
            pads = {}
            if n1:
                pads["f1"] = n1
            if n2:
                pads["f2"] = n2
            got = link(tmp, pad_object(tmp, "got", source(0, 0), pads), "got")
            want = link(tmp, assemble(tmp, "want", source(n1, n2)), "want")
            self.assertEqual(got, want)
            return got

    def test_one_nop_is_the_case_asm_processor_cannot_do(self):
        text, data = self.check(1, 0)
        self.assertEqual(text[8:12], b"\0" * 4)            # the inserted word

    def test_two_pads_and_relocations_follow(self):
        self.check(1, 3)

    def test_pad_after_second_function_only(self):
        self.check(0, 2)

    def test_larger_pad(self):
        self.check(5, 1)

    def test_local_label_jal_and_jump_table_addends_are_fixed(self):
        text, data = self.check(2, 1)
        self.assertNotEqual(data[:4], b"\0\0\0\0")

    def test_assembler_end_alignment_is_dropped(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", source(0, 0))
            raw = rd(obj)
            elf = tp.Elf(raw)
            code_end = max(v + s for v, s in tp.text_functions(obj).values())
            self.assertGreater(len(elf.sections[elf.find(".text")]["data"]), code_end)
            out = tp.Elf(tp.insert_zero_words(raw, {"f1": 1}))
            self.assertEqual(len(out.sections[out.find(".text")]["data"]), code_end + 4)

    def test_no_pads_keeps_bytes(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", source(0, 0))
            a = link(tmp, obj, "a")
            data = tp.insert_zero_words(rd(obj), {})
            with open(obj, "wb") as f:
                f.write(data)
            self.assertEqual(link(tmp, obj, "b"), a)

    def test_text_functions(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", source(0, 0))
            funcs = tp.text_functions(obj)
            self.assertEqual(funcs["f1"], (0, 8))
            self.assertEqual(set(funcs), {"f1", "f2", "f3"})


class FailsLoudly(unittest.TestCase):
    def test_hi16_against_text_section_symbol(self):
        extra = ".text\nla_user:\n lui $a0, %hi(.Lloc)\n addiu $a0, $a0, %lo(.Lloc)\n"
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", source(0, 0, extra=extra))
            with self.assertRaises(SystemExit):
                tp.insert_zero_words(rd(obj), {"f1": 1})

    def test_symbol_without_size(self):
        text = PREAMBLE + ".globl g\ng:\n jr $ra\n nop\n.globl h\nh:\n jr $ra\n nop\n"
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", text)
            with self.assertRaises(SystemExit):
                tp.insert_zero_words(rd(obj), {"g": 1})

    def test_unknown_function(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", source(0, 0))
            with self.assertRaises(SystemExit):
                tp.insert_zero_words(rd(obj), {"nope": 1})

    def test_zero_pad_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = assemble(tmp, "o", source(0, 0))
            with self.assertRaises(SystemExit):
                tp.insert_zero_words(rd(obj), {"f1": 0})


FUNC_S = """nonmatching %(n)s, 0x8

glabel %(n)s
    /* 0 80100000 0800E003 */  jr         $ra
    /* 4 80100004 00000000 */   nop
endlabel %(n)s
%(tail)s"""
NOP = "    /* 8 80100008 00000000 */  nop\n"


class AsmLookup(unittest.TestCase):
    def mk(self, root, path, name, tail):
        d = os.path.join(root, path)
        os.makedirs(d, exist_ok=True)
        with open(os.path.join(d, name + ".s"), "w") as f:
            f.write(FUNC_S % {"n": name, "tail": tail})

    def test_trailing_words(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.mk(tmp, "a", "none", "")
            self.mk(tmp, "a", "three", NOP * 3)
            self.assertEqual(tp.trailing_words(os.path.join(tmp, "a/none.s")), 0)
            self.assertEqual(tp.trailing_words(os.path.join(tmp, "a/three.s")), 3)

    def test_non_nop_tail_fails(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.mk(tmp, "a", "bad", "    /* 8 80100008 0800E003 */  jr         $ra\n")
            with self.assertRaises(SystemExit):
                tp.trailing_words(os.path.join(tmp, "a/bad.s"))

    def test_missing_endlabel_fails(self):
        with tempfile.TemporaryDirectory() as tmp:
            p = os.path.join(tmp, "x.s")
            with open(p, "w") as f:
                f.write("glabel x\n")
            with self.assertRaises(SystemExit):
                tp.trailing_words(p)

    def test_dirs_and_lookup(self):
        self.assertEqual(tp.asm_dirs("src/ovl/TT.c"),
                         ["asm/ovl/TT/matchings/TT", "asm/ovl/TT/nonmatchings/TT"])
        self.assertEqual(tp.asm_dirs("src/main/8004E500.c"),
                         ["asm/matchings/main/8004E500", "asm/nonmatchings/main/8004E500"])
        self.assertEqual(tp.asm_dirs("src/other/x.c"), [])
        with tempfile.TemporaryDirectory() as tmp:
            self.mk(tmp, "asm/ovl/TT/matchings/TT", "f1", "")
            self.assertTrue(tp.find_asm("src/ovl/TT.c", "f1", tmp))
            self.assertIsNone(tp.find_asm("src/ovl/TT.c", "f9", tmp))

    def test_pad_functions_end_to_end(self):
        with tempfile.TemporaryDirectory() as tmp:
            d = "asm/ovl/TT/matchings/TT"
            self.mk(tmp, d, "f1", NOP)                 # C function with one trailing nop
            self.mk(tmp, d, "f2", NOP * 2)             # INCLUDE_ASM'd in the source: skipped
            self.mk(tmp, d, "f3", "")
            obj = assemble(tmp, "got", source(0, 0))
            used = tp.pad_functions(obj, "src/ovl/TT.c", {"f2"}, tmp)
            self.assertEqual(len(used), 2)
            got = link(tmp, obj, "got")
            want = link(tmp, assemble(tmp, "want", source(1, 0)), "want")
            self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
