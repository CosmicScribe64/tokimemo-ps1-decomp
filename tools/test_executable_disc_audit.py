"""T-39510: synthetic bounds, relocation and derived-consumer regressions."""
import copy
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.patch.audit_executable_disc import (
    FunctionOwners, audit_derived, audit_slots, audit_source_only, bind_extent, pair_target,
    payload_references, read_address, referenced_candidates, snapshot_translator, verify_hooks,
)
from tools.patch.text.image import Image, Slot


class ExecutableAuditTest(unittest.TestCase):
    def test_source_only_literal_must_be_in_its_own_executable_and_terminated(self):
        records = {'line': SimpleNamespace(unit={'context': {'link_unit': 'TEL'}})}
        payloads = {'line': b'Hello'}
        path = 'CDROM/EXEDIR/TEL.EXN'
        result = audit_source_only(records, payloads, {path: b'xxHello\0'})
        self.assertEqual(result['checked'], 1)
        self.assertEqual(result['records']['line']['offset'], 2)
        for files in ({path: b'Hello!'}, {path: b'wrong', 'other': b'Hello\0'}):
            with self.assertRaisesRegex(ValueError, 'absent'):
                audit_source_only(records, payloads, files)
        with self.assertRaisesRegex(ValueError, 'no audited executable'):
            audit_source_only(records, payloads, {})
        with self.assertRaisesRegex(ValueError, 'Missing source-only target'):
            audit_source_only(records, {}, {path: b'Hello\0'})

    def test_source_only_unloaded_unit_is_explicitly_excluded(self):
        records = {'retired': SimpleNamespace(unit={'context': {'link_unit': 'EVENT'}})}
        result = audit_source_only(records, {}, {})
        self.assertEqual(result['checked'], 0)
        self.assertEqual(result['excluded_unloaded'], ['retired'])

    def test_census_reads_actual_words_and_does_not_limit_text_to_512_bytes(self):
        data = bytearray(800)
        base = 0x80132000
        struct.pack_into('<I', data, 0, base + 32)
        payload = b'A' * 600 + '\u3042'.encode('shift_jis') + b'\0'
        data[32:32 + len(payload)] = payload
        refs = SimpleNamespace(words={123: [base]}, pairs={})
        result = referenced_candidates(data, base, refs, {})
        self.assertEqual([row['address'] for row in result['candidates']], [base + 32])
        struct.pack_into('<I', data, 0, base + 799)
        self.assertEqual(referenced_candidates(data, base, refs, {})['candidates'], [])

    def test_census_checks_each_shared_high_and_rejects_invalid_opcodes(self):
        data = bytearray(80)
        base = 0x80132000
        struct.pack_into('<III', data, 0, 0x3C088013, 0x3C088014, 0x25082030)
        data[48:51] = '\u3042'.encode('shift_jis') + b'\0'
        refs = SimpleNamespace(words={}, pairs={0: [([base, base + 4], base + 8)]})
        result = referenced_candidates(data, base, refs, {})
        self.assertEqual(result['targets'], 2)
        self.assertEqual(len(result['candidates']), 1)
        struct.pack_into('<I', data, 0, 0)
        with self.assertRaisesRegex(ValueError, 'not LUI'):
            referenced_candidates(data, base, refs, {})
        result = referenced_candidates(data, base, refs, {'hook': {'site': base}})
        self.assertEqual(len(result['excluded_pairs']), 2)
        self.assertEqual(result['targets'], 0)

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
        new.syms = {**old.syms, 'replacement': (self.base + 16, True, True)}
        struct.pack_into('<II', self.data, 0, 0x08000000 | ((self.base + 16) >> 2 & 0x3FFFFFF), 0)
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new, {'consumer': 'replacement'})
        self.assertEqual(result['status'], 'pending')
        self.assertEqual(result['replacement_functions'], {'consumer': 'replacement'})
        struct.pack_into('<II', self.data, 16, 0x3C088013, 0x25082030)
        new.pairs = {self.base + 48: [([self.base + 16], self.base + 20)]}
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new, {'consumer': 'replacement'})
        self.assertEqual(result['status'], 'replacement_function_bound')
        new.syms = old.syms
        new.pairs = {self.base + 48: [([self.base], self.base + 4)]}
        struct.pack_into('<I', self.data, 0, 0x08000000)
        result = bind_extent(record, container, {b'Hello'}, self.data, self.base,
                             self.base + 80, old, new)
        self.assertEqual(result['status'], 'pending')
        self.assertEqual(len(result['rejected_pairs']), 1)

    def test_hook_requires_actual_jump_target_and_nop_delay(self):
        symbols = {'old': (self.base, True, True), 'new': (self.base + 32, True, True)}
        jump = 0x08000000 | ((self.base + 32) >> 2 & 0x3FFFFFF)
        struct.pack_into('<II', self.data, 0, jump, 0)
        self.assertEqual(verify_hooks(self.data, self.base, symbols, {'old': 'new'})['old']['target'],
                         self.base + 32)
        for word, delay in ((jump + 1, 0), (jump, 1), (jump | 0x04000000, 0)):
            struct.pack_into('<II', self.data, 0, word, delay)
            with self.assertRaisesRegex(ValueError, 'hook differs'):
                verify_hooks(self.data, self.base, symbols, {'old': 'new'})

    def test_replacement_payloads_require_every_label_in_selected_region(self):
        struct.pack_into('<II', self.data, 0, 0x3C088013, 0x25082030)
        refs = SimpleNamespace(pairs={self.base + 48: [([self.base], self.base + 4)]})
        result = payload_references(self.data, self.base, refs, {'greeting': b'Hello'},
                                    lambda site: site == self.base + 4)
        self.assertEqual(result['greeting'][0]['target'], self.base + 48)
        for payloads, accepts in (({'greeting': b'Hello', 'other': b'Bye'}, lambda _: True),
                                  ({'greeting': b'Hello'}, lambda _: False)):
            with self.assertRaisesRegex(ValueError, 'Missing replacement'):
                payload_references(self.data, self.base, refs, payloads, accepts)


if __name__ == '__main__':
    unittest.main()
