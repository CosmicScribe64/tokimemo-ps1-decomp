#!/usr/bin/env python3
"""Tests of the raw-address scan and patch of the free relink (T-39505). No game data.

Run: tools/docker.sh python3 tools/test_english_link.py
"""
import os
import struct
import sys
import unittest
import subprocess
import tempfile
from types import SimpleNamespace
from unittest.mock import patch
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import english_link  # noqa: E402
import raw_addrs  # noqa: E402
import elf32  # noqa: E402

RANGES = [english_link.MAIN_RANGE, english_link.SLOT_RANGE]


def words(*ws):
    return bytearray(struct.pack("<%dI" % len(ws), *ws))


def lui(rt, imm):
    return 0x3C000000 | (rt << 16) | imm


def itype(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


class SelectedReferencesTest(unittest.TestCase):
    def test_overlay_imports_existing_main_function_at_relinked_address(self):
        main_symbols = {
            'existing_sdk': (0x800A1234, 0, True, True),
            'already_imported': (0x800A5678, 0, True, True),
            'unused_sdk': (0x800A9000, 0, True, True),
            'english_added': (0x80120000, 0, True, True),
        }
        layout = SimpleNamespace(default_syms={name: value for name, value in main_symbols.items()
                                              if name != 'english_added'},
                                 value=lambda address, syms: 0x800A5678)
        with tempfile.TemporaryDirectory() as folder:
            side = Path(folder) / 'imports.ld'
            out = Path(folder) / 'result.ld'
            side.write_text('already_imported = 0x80095678;\n')
            with patch.object(english_link, 'Layout', return_value=layout), \
                    patch.object(english_link, 'redirected_objects', return_value={}), \
                    patch.object(english_link, 'symbols', return_value=main_symbols), \
                    patch.object(english_link, 'side_scripts', return_value=[str(side)]), \
                    patch.object(english_link, 'referenced_names',
                                 side_effect=lambda *a, **kw: set() if kw else
                                 {'existing_sdk', 'already_imported'}):
                english_link.cmd_aux(['ENDING', 'build/en', str(out), '--main-elf', 'main.elf'])
            result = out.read_text()
            self.assertIn('existing_sdk = 0x800A1234;', result)
            self.assertEqual(result.count('already_imported ='), 1)
            self.assertIn('already_imported = 0x800A5678;', result)
            self.assertIn('english_added = 0x80120000;', result)
            self.assertNotIn('unused_sdk', result)

    def test_discarded_code_cannot_keep_its_undefined_entry_alive(self):
        source = """
.text
.globl code_entry
code_entry:
    jal obsolete_interior_entry
    nop
.data
.globl data_entry
data_entry:
    .word live_data_target
.comm common_entry,4,4
.globl absolute_entry
.set absolute_entry,0x1234
"""
        with tempfile.TemporaryDirectory() as folder:
            asm, obj = Path(folder) / 'input.s', Path(folder) / 'input.o'
            asm.write_text(source)
            subprocess.run(['mips-linux-gnu-as', '-EL', '-o', str(obj), str(asm)], check=True)
            elf = elf32.Elf(str(obj))
            data = english_link.selected_names(elf, {'.data'})
            self.assertTrue({'data_entry', 'live_data_target', 'absolute_entry'} <= data)
            self.assertTrue({'code_entry', 'obsolete_interior_entry', 'common_entry'}.isdisjoint(data))
            text = english_link.selected_names(elf, {'.text', 'COMMON'})
            self.assertTrue({'code_entry', 'obsolete_interior_entry', 'common_entry'} <= text)
            self.assertNotIn('live_data_target', text)
            self.assertEqual(english_link.selected_names(elf, {'.data'}, definitions_only=True),
                             {'data_entry'})


class ScanTest(unittest.TestCase):
    def test_lui_addiu_pair(self):
        code = words(lui(4, 0x800E), itype(0x09, 4, 4, 0x60A0))
        found = list(raw_addrs.scan_text(bytes(code), [], RANGES))
        self.assertEqual([(0, "hi/lo", 0x800E60A0)], [f[:3] for f in found])

    def test_relocated_lui_is_not_raw(self):
        code = words(lui(4, 0x800E), itype(0x09, 4, 4, 0x60A0))
        self.assertEqual([], list(raw_addrs.scan_text(bytes(code), [(0, 5, None)], RANGES)))

    def test_slot_base_is_fixed(self):
        code = words(lui(4, 0x8013), itype(0x0D, 4, 4, 0x2000))
        self.assertEqual([], list(raw_addrs.scan_text(bytes(code), [], RANGES)))


class PatchTest(unittest.TestCase):
    def test_word(self):
        data = words(0x8007C784)
        english_link.patch_raw(data, 0x80100000, [(0x80100000, "word", 0x8007C784, 0x8007D894)])
        self.assertEqual(0x8007D894, struct.unpack("<I", data)[0])

    def test_lui_lw_with_carry(self):
        # lui at, 0x8016; lw v0, -0x1268(at) = 0x8015ED98; moved by 0x2000 -> 0x80160D98
        data = words(lui(1, 0x8016), itype(0x23, 1, 2, -0x1268))
        english_link.patch_raw(data, 0x80140000, [(0x80140000, "hi/lo", 0x8015ED98, 0x80160D98)])
        hi, lw = struct.unpack("<II", data)
        self.assertEqual(0x8016, hi & 0xFFFF)
        self.assertEqual(0x0D98, lw & 0xFFFF)

    def test_lui_ori(self):
        data = words(lui(4, 0x800E), itype(0x0D, 4, 4, 0x60A0))
        english_link.patch_raw(data, 0x80042000, [(0x80042000, "hi/lo", 0x800E60A0, 0x800E93D0)])
        hi, lo = struct.unpack("<II", data)
        self.assertEqual((0x800E, 0x93D0), (hi & 0xFFFF, lo & 0xFFFF))

    def test_shared_lui_updates_every_low(self):
        data = words(lui(1, 0x8016), itype(0x23, 1, 2, -0x1268), itype(0x23, 1, 3, -0x1264))
        english_link.patch_raw(data, 0x80140000, [(0x80140000, "hi/lo", 0x8015ED98, 0x8015EDA8)])
        _hi, a, b = struct.unpack("<III", data)
        self.assertEqual((0xEDA8, 0xEDAC), (a & 0xFFFF, b & 0xFFFF))


if __name__ == "__main__":
    unittest.main()
