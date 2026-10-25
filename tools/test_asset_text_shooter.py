"""Invented native-reader rejection fixtures plus required original proof replay."""
import copy
import hashlib
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.assets import text_shooter
from tools.assets.text_renderers import read_argument_specs


class ShooterContractTests(unittest.TestCase):
    def fixture(self, root):
        repo, inputs = root / 'source', root / 'inputs'
        repo.mkdir(); inputs.mkdir()
        words = [(36 << 26) | (4 << 21) | (8 << 16),
                 (4 << 21) | (17 << 11) | 37,
                 (36 << 26) | (17 << 21) | (9 << 16),
                 (36 << 26) | (17 << 21) | (10 << 16) | 1,
                 (9 << 26) | (17 << 21) | (17 << 16) | 1,
                 (5 << 26) | (10 << 21) | 0xFFFC, 0]
        base, entry = 0x1000, 0x1020
        data = bytearray(128)
        lines = ['glabel shooter_draw_text']
        for index, word in enumerate(words):
            offset = 32 + index * 4
            raw = word.to_bytes(4, 'little')
            data[offset:offset + 4] = raw
            lines.append(f'/* {offset:X} {entry + index * 4:08X} {raw.hex().upper()} */ fixture_op')
        (repo / 'reader.s').write_text('\n'.join(lines) + '\n')
        (repo / 'reader.c').write_text('/* Invented source evidence. */\n')
        (repo / 'types.h').write_text('typedef unsigned char FixtureByte;\n')
        (inputs / 'SYNTHETIC.EXN').write_bytes(data)
        profile = {'source_pins': {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in repo.iterdir()},
            'path': 'SYNTHETIC.EXN', 'sha256': hashlib.sha256(data).hexdigest(), 'size': len(data),
            'load_address': base, 'header_size': 0, 'function': 'shooter_draw_text',
            'assembly_path': 'reader.s', 'entry_address': entry, 'instruction_words': len(words),
            'reader_facts': [{'address': entry, 'opcode': 36, 'rs': 4, 'rt': 8, 'immediate': 0},
                             {'address': entry + 4, 'opcode': 0, 'rs': 4, 'rt': 0, 'rd': 17, 'function': 37},
                             {'address': entry + 12, 'opcode': 36, 'rs': 17, 'immediate': 1},
                             {'address': entry + 16, 'opcode': 9, 'rs': 17, 'rt': 17, 'immediate': 1}]}
        return repo, inputs, profile

    def verify(self, repo, inputs, profile):
        spec = dict(text_shooter.SHOOTER_TEXT_READ_SPEC)
        spec['callee_address'] = profile.get('entry_address')
        with patch.object(text_shooter, 'SHOOTER_TEXT_CONTRACT', text_shooter._freeze(profile)), \
                patch.object(text_shooter, 'SHOOTER_TEXT_READ_SPEC', text_shooter._freeze(spec)):
            return text_shooter.verify_shooter_text_contract(repo, inputs)

    def test_verified_contract_exposes_unimplemented_model_without_registration(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            before = read_argument_specs('shooter_draw_text', 'TACO')
            result = self.verify(repo, inputs, profile)
            self.assertTrue(result['verified'])
            self.assertEqual(result['code_proof']['instruction_words_verified'], 7)
            expected = dict(text_shooter.SHOOTER_TEXT_READ_SPEC)
            expected['callee_address'] = profile['entry_address']
            self.assertEqual(result['read_spec'], expected)
            self.assertIsNone(result['read_spec']['renderer'])
            self.assertEqual(result['read_spec']['renderer_status'], 'unimplemented_fixed_renderer')
            self.assertFalse(result['renderer_model_implemented'])
            self.assertFalse(result['runtime_reachability_proven'])
            self.assertFalse(result['runtime_read_length_proven'])
            self.assertEqual(read_argument_specs('shooter_draw_text', 'TACO'), before)
            self.assertTrue(all('raw_hex' not in v for v in result['code_proof']['instructions']))
            result['read_spec']['category'] = 'caller_modified_output_copy'
            self.assertEqual(self.verify(repo, inputs, profile)['read_spec']['category'], 'display')

    def test_changed_header_rejects_even_with_self_consistent_manifest(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            (repo / 'types.h').write_text('typedef unsigned short FixtureByte;\n')
            (repo / 'snapshot-manifest.json').write_text('{}')
            result = self.verify(repo, inputs, profile)
            self.assertFalse(result['verified'])
            self.assertEqual(result['blockers'], ['reviewed_source_pin_mismatch'])
            self.assertNotIn('read_spec', result)

    def test_missing_sources_and_required_proof_fail_closed(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            changed = copy.deepcopy(profile)
            changed.pop('assembly_path')
            self.assertFalse(self.verify(repo, inputs, changed)['verified'])
            changed = copy.deepcopy(profile)
            changed['reader_facts'] = []
            self.assertFalse(self.verify(repo, inputs, changed)['verified'])
            (repo / 'reader.s').unlink()
            self.assertFalse(self.verify(repo, inputs, profile)['verified'])

    def test_original_hash_and_actual_instruction_bytes_are_both_enforced(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            path = inputs / 'SYNTHETIC.EXN'
            data = bytearray(path.read_bytes()); data[32] ^= 1; path.write_bytes(data)
            self.assertEqual(self.verify(repo, inputs, profile)['blockers'], ['original_input_hash_or_size_mismatch'])
            profile['sha256'] = hashlib.sha256(data).hexdigest()
            self.assertEqual(self.verify(repo, inputs, profile)['blockers'], ['renderer_instruction_mismatch_or_discontinuity'])

    def test_repinned_changed_argument_read_still_fails_native_fact(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            path = inputs / 'SYNTHETIC.EXN'
            data = bytearray(path.read_bytes()); old = data[32:36]; data[32] = 1
            new = data[32:36]; path.write_bytes(data)
            asm = repo / 'reader.s'
            asm.write_text(asm.read_text().replace(old.hex().upper(), new.hex().upper(), 1))
            profile['sha256'] = hashlib.sha256(data).hexdigest()
            profile['source_pins']['reader.s'] = hashlib.sha256(asm.read_bytes()).hexdigest()
            result = self.verify(repo, inputs, profile)
            self.assertEqual(result['blockers'], ['native_reader_instruction_fact_mismatch'])

    def test_entry_label_and_extent_cannot_be_reinterpreted(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            changed = copy.deepcopy(profile); changed['entry_address'] += 4
            self.assertEqual(self.verify(repo, inputs, changed)['blockers'], ['renderer_entry_or_instruction_extent_mismatch'])
            asm = repo / 'reader.s'
            asm.write_text(asm.read_text().replace('glabel shooter_draw_text', 'glabel other_reader'))
            profile['source_pins']['reader.s'] = hashlib.sha256(asm.read_bytes()).hexdigest()
            self.assertEqual(self.verify(repo, inputs, profile)['blockers'], ['renderer_entry_label_missing_or_ambiguous'])

    def test_source_and_input_symlink_escapes_fail_closed(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            repo, inputs, profile = self.fixture(root)
            outside = root / 'outside.h'; outside.write_bytes((repo / 'types.h').read_bytes())
            (repo / 'types.h').unlink(); (repo / 'types.h').symlink_to(outside)
            self.assertFalse(self.verify(repo, inputs, profile)['verified'])
            (repo / 'types.h').unlink(); (repo / 'types.h').write_bytes(outside.read_bytes())
            outside = root / 'outside.bin'; outside.write_bytes((inputs / 'SYNTHETIC.EXN').read_bytes())
            (inputs / 'SYNTHETIC.EXN').unlink(); (inputs / 'SYNTHETIC.EXN').symlink_to(outside)
            self.assertFalse(self.verify(repo, inputs, profile)['verified'])

    def test_source_change_during_proof_is_detected(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            original_read = Path.read_bytes
            def read(path):
                raw = original_read(path)
                if path == inputs / 'SYNTHETIC.EXN':
                    (repo / 'types.h').write_text('/* Changed after initial pin read. */\n')
                return raw
            with patch.object(Path, 'read_bytes', read):
                result = self.verify(repo, inputs, profile)
            self.assertEqual(result['blockers'], ['source_evidence_changed_during_verification'])

    def test_public_proof_metadata_is_recursively_immutable(self):
        spec = text_shooter.SHOOTER_TEXT_READ_SPEC
        contract = text_shooter.SHOOTER_TEXT_CONTRACT
        with self.assertRaises(TypeError):
            spec['category'] = 'tampered'
        with self.assertRaises(TypeError):
            spec['callee_address'] = 0x1234
        with self.assertRaises(TypeError):
            contract['entry_address'] = 0x1234
        with self.assertRaises(TypeError):
            contract['source_pins'][next(iter(contract['source_pins']))] = '0' * 64
        with self.assertRaises(TypeError):
            contract['reader_facts'][0]['opcode'] = 0
        self.assertEqual(spec['category'], 'display')
        self.assertEqual(spec['callee_address'], 0x801432F0)
        self.assertEqual(contract['entry_address'], 0x801432F0)

    def test_read_spec_cannot_bind_a_different_verified_entry(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, profile = self.fixture(Path(folder))
            with patch.object(text_shooter, 'SHOOTER_TEXT_CONTRACT', text_shooter._freeze(profile)):
                result = text_shooter.verify_shooter_text_contract(repo, inputs)
            self.assertEqual(result['blockers'], ['read_spec_entry_binding_mismatch'])

    def test_actual_current_source_and_original_renderer_proof_without_skip(self):
        repo = Path(os.environ.get('TOKIMEMO_TEXT_SOURCE_ROOT', '/work/.asset-cache/shared/source-snapshot-phase2-687b5ce7'))
        inputs = Path(os.environ.get('TOKIMEMO_TEXT_INPUT_ROOT', '/work/disc/files'))
        result = text_shooter.verify_shooter_text_contract(repo, inputs)
        self.assertTrue(result['verified'], result.get('blockers'))
        self.assertEqual(result['source_pins_verified'], 53)
        self.assertEqual(result['code_proof']['instruction_words_verified'], 161)
        self.assertEqual(result['reader_instruction_facts_verified'], 6)
        self.assertEqual(result['entry_address'], 0x801432F0)
        self.assertIsNone(result['read_spec']['renderer'])


if __name__ == '__main__':
    unittest.main()
