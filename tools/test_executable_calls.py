"""Synthetic actual-disc call-target regressions for unnamed IDO helpers."""
import struct
import unittest
from types import SimpleNamespace
from unittest.mock import patch

from tools.patch.executable_calls import jal_target, local_regions, named_calls


class StaticCallTest(unittest.TestCase):
    def fixture(self):
        section = SimpleNamespace(index=1, size=64)
        symbol = SimpleNamespace(type=3, shndx=1)
        original = bytearray(64)
        struct.pack_into('<I', original, 4, 0x0C000008)  # section-relative target 32
        elf = SimpleNamespace(
            section=lambda name: section, contents=lambda sec: original,
            relocations=lambda sec: [(4, 4, symbol)],
            symbols=[SimpleNamespace(value=0, type=2, shndx=1)])
        data = bytearray(64)
        struct.pack_into('<I', data, 4, 0x0C000000 | ((0x80001020 >> 2) & 0x3FFFFFF))
        return elf, data

    def test_regions_follow_verified_actual_call(self):
        elf, data = self.fixture()
        with patch('tools.patch.executable_calls.elf32.Elf', return_value=elf):
            regions, edges = local_regions('synthetic.o', 0x80001000, data, 0x80001000)
        self.assertEqual(regions, {0x80001000: 0x80001020, 0x80001020: 0x80001040})
        self.assertEqual(edges, [{'site': 0x80001004, 'target': 0x80001020, 'caller': 0x80001000}])

    def test_changed_actual_target_or_opcode_fails(self):
        for word in (0x0C000000, 0x08000408):
            elf, data = self.fixture()
            struct.pack_into('<I', data, 4, word)
            with patch('tools.patch.executable_calls.elf32.Elf', return_value=elf):
                with self.assertRaises(ValueError):
                    local_regions('synthetic.o', 0x80001000, data, 0x80001000)

    def test_jump_segment_comes_from_instruction_location(self):
        self.assertEqual(jal_target(0x0C000004, 0x8FFFFFFC), 0x90000010)
        with self.assertRaises(ValueError):
            jal_target(0x08000004, 0x80000000)

    def test_named_calls_require_correct_region_and_target(self):
        elf, data = self.fixture()
        symbols = {'helper': (0x80001020, True, True)}
        self.assertEqual(named_calls(data, 0x80001000, 0x80001000, 0x80001010,
                                     symbols, ['helper']), {'helper': [0x80001004]})
        self.assertEqual(named_calls(data, 0x80001000, 0x80001010, 0x80001020,
                                     symbols, ['helper']), {'helper': []})
        with self.assertRaises(ValueError):
            named_calls(data, 0x80001000, 0x80000FFC, 0x80001020, symbols, ['helper'])


if __name__ == '__main__':
    unittest.main()
