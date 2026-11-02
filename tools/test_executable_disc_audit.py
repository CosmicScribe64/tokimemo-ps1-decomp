"""T-39510: synthetic bounds, relocation and derived-consumer regressions."""
import copy
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.patch.audit_executable_disc import (
    FunctionOwners, audit_derived, audit_slots, bind_extent, pair_target,
    read_address, snapshot_translator,
)
from tools.patch.text.image import Image, Slot


class ExecutableAuditTest(unittest.TestCase):
    def setUp(self):
        self.base = 0x80132000
        self.data = bytearray(96)
        struct.pack_into('<I', self.data, 4, self.base + 48)
        self.data[48:54] = b'Hello\0'
        self.payloads = {'physical': b'Hello'}
        self.slots = {'physical': {'exact': True}}
        self.record = {'id': 'view', 'kind': 'stage_text_record', 'context': {
            'source_inventory_record_ids': ['physical'],
            'stage_context': {'body_relative_offset': 1,
                              'uses': [{'pointer_evidence': {'address': self.base}}]}}}

    def check_view(self, record=None):
        return audit_derived(record or self.record, self.data, self.base,
                             lambda address: address + 4, self.payloads, self.slots)

    def test_uses_relocated_consumer_word(self):
        result = self.check_view()
        self.assertEqual(result['pointers'][0]['site'], self.base + 4)
        self.assertEqual(result['pointers'][0]['target'], self.base + 48)

    def test_wrong_pointer_fails_even_if_expected_string_exists(self):
        struct.pack_into('<I', self.data, 4, self.base + 64)
        with self.assertRaisesRegex(ValueError, 'does not reach'):
            self.check_view()

    def test_bad_pointer_and_truncated_string_fail(self):
        for target in (self.base - 1, self.base + 93, self.base + 200):
            struct.pack_into('<I', self.data, 4, target)
            with self.assertRaisesRegex(ValueError, 'does not reach'):
                self.check_view()

    def test_every_consumer_must_have_pointer_evidence(self):
        self.record['context']['stage_context']['uses'].append({})
        with self.assertRaisesRegex(ValueError, 'no pointer evidence'):
            self.check_view()

    def test_missing_consumers_and_physical_sources_fail(self):
        for field, value, error in (
                ('source_inventory_record_ids', [], 'no physical source'),
                ('source_inventory_record_ids', ['unknown'], 'unverified'),
                ('stage_context', {'uses': []}, 'no consumers')):
            record = copy.deepcopy(self.record)
            record['context'][field] = value
            with self.assertRaisesRegex(ValueError, error):
                self.check_view(record)

    def test_initial_buffer_requires_verified_physical_storage(self):
        self.record['kind'] = 'initial_runtime_buffer'
        self.assertEqual(self.check_view()['pointers'], [])
        self.slots['physical']['exact'] = False
        with self.assertRaisesRegex(ValueError, 'unverified'):
            self.check_view()

    def test_unmapped_unaligned_and_out_of_bounds_sites_fail(self):
        for address in (None, self.base + 1, self.base - 4, self.base + 96):
            with self.assertRaises(ValueError):
                audit_derived(self.record, self.data, self.base, lambda _: address,
                              self.payloads, self.slots)

    def test_negative_slice_and_truncation_are_not_accepted(self):
        for address, size in ((self.base - 1, 1), (self.base + 95, 4), (self.base, -1)):
            with self.assertRaisesRegex(ValueError, 'outside file'):
                read_address(self.data, self.base, address, size)

    def test_slots_use_retargeted_address_and_correct_executable_base(self):
        original_base = 0x8000F800
        final_base = 0x80040800
        original = bytearray(self.data)
        original[16:20] = b'Old\0'
        container = SimpleNamespace(
            records={'physical': SimpleNamespace(kind='source_extent')},
            image=Image('synthetic', original, original_base,
                        [Slot('physical', 16, b'Old', capacity=16)], []))
        placements = {'retargeted': {}}
        rows = audit_slots(container, self.data, final_base,
                           lambda address: address - original_base + final_base + 32,
                           self.payloads, placements)
        self.assertTrue(rows['physical']['exact'])
        placements['retargeted']['physical'] = {'to': final_base + 48}
        rows = audit_slots(container, self.data, final_base, lambda _: None,
                           self.payloads, placements)
        self.assertTrue(rows['physical']['exact'])
        self.data[53] = 1
        rows = audit_slots(container, self.data, final_base, lambda _: None,
                           self.payloads, placements)
        self.assertFalse(rows['physical']['exact'])

    def test_changed_live_link_is_rejected_before_translation(self):
        with tempfile.TemporaryDirectory() as tmp:
            frozen, live = Path(tmp) / 'frozen', Path(tmp) / 'live'
            frozen.mkdir()
            live.mkdir()
            (frozen / 'SLPM_86.053.elf').write_bytes(b'frozen')
            (live / 'SLPM_86.053.elf').write_bytes(b'changed')
            with self.assertRaisesRegex(ValueError, 'differs from frozen'):
                snapshot_translator('main', frozen, live)

    def test_pair_decodes_signed_carry_and_unsigned_ori(self):
        struct.pack_into('<III', self.data, 0, 0x3C088014, 0x25088010, 0x3C098014)
        self.assertEqual(pair_target(self.data, self.base, [self.base, self.base + 8],
                                     self.base + 4), 0x80138010)
        struct.pack_into('<I', self.data, 4, 0x35088010)
        self.assertEqual(pair_target(self.data, self.base, [self.base], self.base + 4),
                         0x80148010)

    def test_pair_rejects_wrong_opcodes_and_conflicting_highs(self):
        for high, low, other in ((0, 0x25080010, 0x3C098014),
                                 (0x3C088014, 0, 0x3C098014),
                                 (0x3C088014, 0x25080010, 0x3C098015)):
            struct.pack_into('<III', self.data, 0, high, low, other)
            with self.assertRaises(ValueError):
                pair_target(self.data, self.base, [self.base, self.base + 8], self.base + 4)

    def test_owner_cannot_leak_into_rodata_or_next_object(self):
        refs = SimpleNamespace(syms={'first': (self.base, True, True)},
                               placed={('obj', '.text'): (self.base, 16),
                                       ('next', '.text'): (self.base + 32, 16)})
        owners = FunctionOwners(refs)
        self.assertEqual(owners.at(self.base + 12), 'first')
        self.assertIsNone(owners.at(self.base + 20))
        self.assertIsNone(owners.at(self.base + 40))

    def test_payload_in_wrong_function_is_not_binding(self):
        struct.pack_into('<II', self.data, 0, 0x3C088013, 0x25082030)
        placed = {('obj', '.text'): (self.base, 32)}
        old = SimpleNamespace(syms={'consumer': (self.base, True, True)}, placed=placed,
                              pairs={self.base + 80: [([self.base], self.base + 4)]}, words={})
        new = SimpleNamespace(syms={'other': (self.base, True, True)}, placed=placed,
                              pairs={self.base + 48: [([self.base], self.base + 4)]}, words={})
        record = {'source_offset': 80, 'raw_hex': '4f6c6400'}
        container = SimpleNamespace(image=SimpleNamespace(base=self.base))
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new)
        self.assertEqual(result['status'], 'pending')
        new.syms = old.syms
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new)
        self.assertEqual(result['status'], 'function_bound')
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new, {'consumer': 'replacement'})
        self.assertEqual(result['status'], 'pending')
        self.assertEqual(result['replacement_functions'], {'consumer': 'replacement'})
        struct.pack_into('<I', self.data, 0, 0x08000000)
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new)
        self.assertEqual(result['status'], 'pending')
        self.assertEqual(len(result['rejected_pairs']), 1)


if __name__ == '__main__':
    unittest.main()
