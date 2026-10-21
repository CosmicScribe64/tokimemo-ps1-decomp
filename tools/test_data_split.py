#!/usr/bin/env python3
"""Synthetic ELF checks for cutting an object's .data into island sections (T-25005,
tools/data_split.py). Objects come from mips-linux-gnu-as, no game data.

usage (in Docker): python3 tools/test_data_split.py
"""
from pathlib import Path
import os
import struct
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import data_split as ds  # noqa: E402
from trailing_pad import Elf, read_symbols  # noqa: E402

# .data = [part B: bsym, 16 bytes][part A: asym, 8 bytes, .Ltpl (unnamed, like an IDO
# initialiser template), 8 bytes]; the code and .rodata reach .Ltpl only through the .data
# section symbol, as IDO does
SOURCE = """.set noreorder
.text
.globl f
f:
lui $t6, %hi(.Ltpl)
addiu $t6, $t6, %lo(.Ltpl)
lui $t7, %hi(.Ltpl + 4)
lw $t8, %lo(.Ltpl + 4)($t7)
sw $t8, %lo(.Ltpl + 4)($t7)
lui $t9, %hi(bsym)
jr $ra
lw $t9, %lo(bsym)($t9)
.section .rodata
.word .Ltpl
.word .Lb4
.data
.globl bsym
bsym:
.word 1
.Lb4:
.word 2, 3, 4
.globl asym
asym:
.word 0x11, asym
.Ltpl:
.word 0x22, 0x33
"""
REGIONS = [(".data_80002000", "asym", 16), (".data", "bsym", 16)]


def run(args, **kw):
    subprocess.run(args, check=True, **kw)


def assemble(source, tmp):
    obj = Path(tmp) / "t.o"
    run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o", str(obj)], input=source.encode())
    return obj.read_bytes()


def link(obj_bytes, tmp, script):
    """Bytes of the linked image as {section: bytes} at the addresses of `script`."""
    obj = Path(tmp) / "in.o"
    obj.write_bytes(obj_bytes)
    ld = Path(tmp) / "t.ld"
    ld.write_text(script)
    elf = Path(tmp) / "t.elf"
    run(["mips-linux-gnu-ld", "-EL", "-T", str(ld), "-o", str(elf), str(obj)])
    out = {}
    for name in (".text", ".rodata", ".d1", ".d2"):
        dst = Path(tmp) / ("s" + name)
        run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", name, str(elf), str(dst)])
        out[name] = dst.read_bytes()
    return out


LINK_BEFORE = """SECTIONS {
  .text 0x1000 : { *(.text) }  .rodata 0x1100 : { *(.rodata) }
  .d1 0x3000 : { *(.data) }  /DISCARD/ : { *(*) } }
"""
LINK_AFTER = """SECTIONS {
  .text 0x1000 : { *(.text) }  .rodata 0x1100 : { *(.rodata) }
  .d2 0x2000 : { *(.data_80002000) }  .d1 0x3000 : { *(.data) }  /DISCARD/ : { *(*) } }
"""


def words(b):
    return list(struct.unpack("<%dI" % (len(b) // 4), b))


class DataSplitTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.before = assemble(SOURCE, self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def test_parts_move_symbols_and_relocations(self):
        after = Elf(ds.split(self.before, REGIONS))
        d, d2 = after.find(".data"), after.find(".data_80002000")
        self.assertEqual(bytes(after.sections[d]["data"]), struct.pack("<4I", 1, 2, 3, 4))
        self.assertEqual(words(after.sections[d2]["data"])[::2], [0x11, 0x22])
        syms = {s[0]: (s[1], s[5]) for s in read_symbols(after, after.find(".symtab"))}
        self.assertEqual(syms["bsym"], (0, d))
        self.assertEqual(syms["asym"], (0, d2))
        self.assertEqual(after.sections[after.find(".rel.data_80002000")]["info"], d2)

    def test_linked_addresses_follow_the_parts(self):
        split = link(ds.split(self.before, REGIONS), self.tmp.name, LINK_AFTER)
        self.assertEqual(words(split[".rodata"]), [0x2008, 0x3004])
        self.assertEqual(words(split[".d2"]), [0x11, 0x2000, 0x22, 0x33])
        text = words(split[".text"])
        self.assertEqual((text[0] & 0xFFFF, text[1] & 0xFFFF), (0, 0x2008))
        self.assertEqual([w & 0xFFFF for w in text[2:5]], [0, 0x200C, 0x200C])
        self.assertEqual((text[5] & 0xFFFF, text[7] & 0xFFFF), (0, 0x3000))
        whole = link(self.before, self.tmp.name, LINK_BEFORE)
        self.assertEqual(words(whole[".rodata"]), [0x3018, 0x3004])
        self.assertEqual(whole[".d1"][:16], split[".d1"])

    def test_one_region_part_must_stay_in_data(self):
        with self.assertRaises(ds.SplitError):
            ds.split(self.before, [(".data_80002000", "asym", 16), (".data_80003000", "bsym", 16)])

    def test_parts_must_be_back_to_back(self):
        with self.assertRaises(ds.SplitError):
            ds.split(self.before, [(".data_80002000", "asym", 16), (".data", "bsym", 8)])

    def test_parts_must_cover_data(self):
        with self.assertRaises(ds.SplitError):
            ds.split(self.before, [(".data_80002000", "asym", 32), (".data", "bsym", 16)])

    def test_start_symbol_must_be_in_data(self):
        with self.assertRaises(ds.SplitError):
            ds.split(self.before, [(".data_80002000", "f", 16), (".data", "bsym", 16)])

    def test_hi16_shared_across_parts_fails(self):
        source = SOURCE.replace("lui $t9, %hi(bsym)\n", "lw $t9, %lo(.Ltpl - 0x10)($t7)\n")
        with self.assertRaises(ds.SplitError):
            ds.split(assemble(source, self.tmp.name), REGIONS)


if __name__ == "__main__":
    unittest.main()
