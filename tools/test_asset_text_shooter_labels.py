"""Invented fail-closed fixtures and required original native-helper replay."""
import copy
import hashlib
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.assets import text_shooter_labels as labels
from tools.assets.text_renderers import read_argument_specs


class ShooterLabelContractTests(unittest.TestCase):
    def fixture(self, root):
        repo, inputs = root / 'source', root / 'inputs'
        repo.mkdir(); inputs.mkdir()
        (repo / 'fixture.c').write_text('/* Invented source witness. */\n')
        (repo / 'fixture.h').write_text('typedef unsigned char FixtureByte;\n')
        names = ['shooter_draw_label', 'shooter_draw_ranking_text',
                 'shooter_draw_ranking_glyph', 'shooter_draw_glyph']
        entries = {name: 0x1020 + index * 0x30 for index, name in enumerate(names)}
        image = bytearray(256)
        sequences = {}
        for name in names:
            if name in names[:2]:
                start = [(36 << 26) | (4 << 21) | (14 << 16),
                         (12 << 26) | (14 << 21) | (4 << 16) | 255]
            elif name == names[2]:
                start = [(12 << 26) | (4 << 21) | (14 << 16) | 255,
                         (14 << 21) | (4 << 11) | 37]
            else:
                start = [(12 << 26) | (4 << 21) | (17 << 16) | 255, 0]
            target = entries[names[2] if name == names[1] else names[3]]
            sequences[name] = start + [0, 0, 0, (3 << 26) | (target >> 2), 0]
            lines = ['glabel ' + name]
            for index, word in enumerate(sequences[name]):
                offset = entries[name] - 0x1000 + index * 4
                raw = word.to_bytes(4, 'little')
                image[offset:offset + 4] = raw
                lines.append(f'/* {offset:X} {entries[name] + index * 4:08X} {raw.hex().upper()} */ fixture_op')
            (repo / (name + '.s')).write_text('\n'.join(lines) + '\n')
        (inputs / 'SYNTHETIC.EXN').write_bytes(image)
        pins = {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in repo.iterdir()}
        proofs = {}
        for name in names:
            offset = entries[name] - 0x1000
            proofs[name] = {'unit': 'TACO', 'source': 'fixture.c', 'source_sha256': pins['fixture.c'],
                'assembly': name + '.s', 'assembly_sha256': pins[name + '.s'],
                'binary': 'SYNTHETIC.EXN', 'binary_sha256': hashlib.sha256(image).hexdigest(),
                'entry_address': entries[name], 'load_address': 0x1000, 'header_size': 0,
                'verified_instruction_words': 7, 'window_offset': offset, 'window_size': 28,
                'window_sha256': hashlib.sha256(image[offset:offset + 28]).hexdigest(),
                'read_arguments': [], 'evidence': 'Invented native reader fixture.'}
        facts = []
        flows = []
        for name in names[:3]:
            target = entries[names[2] if name == names[1] else names[3]]
            facts.append({'function_name': name, 'address': entries[name] + 20,
                          'opcode': 3, 'callee_address': target})
            facts.append({'function_name': name, 'address': entries[name],
                          'opcode': 12 if name == names[2] else 36, 'rs': 4, 'rt': 14,
                          'immediate': 255 if name == names[2] else 0})
            flows.append({'function_name': name, 'register': 4,
                          'start_address': entries[name] + 4,
                          'end_address': entries[name] + 20, 'control_edges': {}})
        facts.append({'function_name': names[3], 'address': entries[names[3]],
                      'opcode': 12, 'rs': 4, 'rt': 17, 'immediate': 255})
        descriptor = {'source_pins': pins, 'proofs': proofs, 'facts': facts,
                      'preserved_register_paths': flows}
        self.required_sites = frozenset((f['function_name'], f['address']) for f in facts)
        self.required_paths = frozenset((f['function_name'], f['register'], f['start_address'], f['end_address']) for f in flows)
        specs = {name: {**dict(spec), 'callee_address': entries[name]}
                 for name, spec in labels.SHOOTER_LABEL_READ_SPECS.items()}
        return repo, inputs, descriptor, specs

    def verify(self, repo, inputs, descriptor, specs):
        with patch.object(labels, 'SHOOTER_LABEL_CONTRACT', labels._freeze(descriptor)), \
                patch.object(labels, 'SHOOTER_LABEL_READ_SPECS', labels._freeze(specs)), \
                patch.object(labels, '_REQUIRED_FACT_SITES', self.required_sites), \
                patch.object(labels, '_REQUIRED_FORWARDING_PATHS', self.required_paths):
            return labels.verify_shooter_label_contract(repo, inputs)

    def rewrite_word(self, repo, inputs, descriptor, name, address, word):
        proof = descriptor['proofs'][name]
        offset = address - proof['load_address']
        image_path = inputs / proof['binary']
        image = bytearray(image_path.read_bytes())
        old = image[offset:offset + 4]
        raw = word.to_bytes(4, 'little')
        image[offset:offset + 4] = raw
        image_path.write_bytes(image)
        asm = repo / proof['assembly']
        lines = asm.read_text().splitlines()
        line_index = 1 + (address - proof['entry_address']) // 4
        lines[line_index] = lines[line_index].replace(old.hex().upper(), raw.hex().upper())
        asm.write_text('\n'.join(lines) + '\n')
        descriptor['source_pins'][proof['assembly']] = hashlib.sha256(asm.read_bytes()).hexdigest()
        proof['assembly_sha256'] = descriptor['source_pins'][proof['assembly']]
        for item in descriptor['proofs'].values():
            item['binary_sha256'] = hashlib.sha256(image).hexdigest()
            start, size = item['window_offset'], item['window_size']
            item['window_sha256'] = hashlib.sha256(image[start:start + size]).hexdigest()

    def test_verified_metadata_remains_unimplemented_and_does_not_register_roles(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            before = read_argument_specs('shooter_draw_label', 'TACO')
            result = self.verify(repo, inputs, descriptor, specs)
            self.assertTrue(result['verified'], result.get('blockers'))
            self.assertEqual(result['byte_forwarding_paths_verified'], 3)
            self.assertFalse(result['renderer_model_implemented'])
            self.assertFalse(result['runtime_reachability_proven'])
            self.assertFalse(result['runtime_read_length_proven'])
            self.assertIsNone(result['read_specs']['shooter_draw_label']['renderer'])
            self.assertEqual(read_argument_specs('shooter_draw_label', 'TACO'), before)
            result['read_specs']['shooter_draw_label']['category'] = 'changed_output_copy'
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['read_specs']['shooter_draw_label']['category'], 'display')

    def test_header_changes_fail_independently_of_manifest(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            (repo / 'fixture.h').write_text('typedef unsigned short FixtureByte;\n')
            (repo / 'snapshot-manifest.json').write_text('{}')
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['reviewed_source_or_header_pin_mismatch'])

    def test_missing_helper_proof_or_critical_facts_fail_closed(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            changed = copy.deepcopy(descriptor); changed['facts'] = []
            self.assertFalse(self.verify(repo, inputs, changed, specs)['verified'])
            changed = copy.deepcopy(descriptor); changed['proofs'].pop('shooter_draw_glyph')
            self.assertFalse(self.verify(repo, inputs, changed, specs)['verified'])
            changed = copy.deepcopy(descriptor); changed['preserved_register_paths'].pop()
            self.assertFalse(self.verify(repo, inputs, changed, specs)['verified'])
            (repo / 'shooter_draw_glyph.s').unlink()
            self.assertFalse(self.verify(repo, inputs, descriptor, specs)['verified'])

    def test_original_code_corruption_is_rejected_by_shared_replay(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            path = inputs / 'SYNTHETIC.EXN'
            raw = bytearray(path.read_bytes()); raw[32] ^= 1; path.write_bytes(raw)
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['shared_original_source_code_replay_failed'])

    def test_jal_target_is_verified_after_consistent_repinning(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            name = 'shooter_draw_label'; address = descriptor['proofs'][name]['entry_address'] + 20
            self.rewrite_word(repo, inputs, descriptor, name, address, (3 << 26) | (0x1100 >> 2))
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['native_byte_call_primitive_fact_mismatch'])

    def test_intervening_call_cannot_preserve_caller_saved_byte_argument(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            name = 'shooter_draw_label'; address = descriptor['proofs'][name]['entry_address'] + 8
            self.rewrite_word(repo, inputs, descriptor, name, address, (3 << 26) | (0x1100 >> 2))
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['native_byte_forwarding_register_overwritten_or_unmodeled'])

    def test_syscall_and_break_are_unmodeled_fail_closed_writes(self):
        self.assertIsNone(labels._written_registers(12))
        self.assertIsNone(labels._written_registers(13))
        clobbers = labels._written_registers((3 << 26) | 1)
        self.assertTrue({1, 4, 15, 24, 28, 31}.issubset(clobbers))
        self.assertFalse({16, 23, 29, 30} & clobbers)
        self.assertTrue({4, 28, 31}.issubset(labels._written_registers((5 << 21) | (31 << 11) | 9)))

    def test_unreviewed_branch_and_reviewed_local_edge_are_distinguished(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            name = 'shooter_draw_label'; start = descriptor['proofs'][name]['entry_address']
            self.rewrite_word(repo, inputs, descriptor, name, start + 8, (4 << 26) | 1)
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['native_byte_forwarding_control_edge_not_reviewed'])
            descriptor['preserved_register_paths'][0]['control_edges'][start + 8] = start + 16
            self.assertTrue(self.verify(repo, inputs, descriptor, specs)['verified'])

    def test_endpoint_jal_delay_cannot_clobber_argument(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            name = 'shooter_draw_label'; start = descriptor['proofs'][name]['entry_address']
            self.rewrite_word(repo, inputs, descriptor, name, start + 24, (4 << 11) | 37)
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['native_byte_argument_clobbered_in_call_delay_slot'])

    def test_load_origin_is_not_assumed_ready_without_r3000_delay_model(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            descriptor['preserved_register_paths'][0]['start_address'] -= 4
            # Even a deliberately reviewed synthetic load-origin identity
            # needs a load-delay model before it can certify the argument.
            self.required_paths = frozenset((f['function_name'], f['register'], f['start_address'], f['end_address'])
                                            for f in descriptor['preserved_register_paths'])
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['native_byte_argument_capture_or_load_delay_unmodeled'])

    def test_entry_address_must_match_immutable_scoped_spec(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            specs['shooter_draw_label']['callee_address'] += 4
            self.assertEqual(self.verify(repo, inputs, descriptor, specs)['blockers'], ['native_read_spec_entry_mismatch'])

    def test_source_and_input_path_escapes_fail_closed(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            repo, inputs, descriptor, specs = self.fixture(root)
            path = repo / 'fixture.h'; outside = root / 'outside.h'; outside.write_bytes(path.read_bytes())
            path.unlink(); path.symlink_to(outside)
            self.assertFalse(self.verify(repo, inputs, descriptor, specs)['verified'])
            path.unlink(); path.write_bytes(outside.read_bytes())
            path = inputs / 'SYNTHETIC.EXN'; outside = root / 'outside.bin'; outside.write_bytes(path.read_bytes())
            path.unlink(); path.symlink_to(outside)
            self.assertFalse(self.verify(repo, inputs, descriptor, specs)['verified'])

    def test_contract_specs_nested_pins_and_facts_are_immutable(self):
        with self.assertRaises(TypeError):
            labels.SHOOTER_LABEL_READ_SPECS['shooter_draw_label']['category'] = 'tampered'
        with self.assertRaises(TypeError):
            labels.SHOOTER_LABEL_CONTRACT['source_pins'][next(iter(labels.SHOOTER_LABEL_CONTRACT['source_pins']))] = '0' * 64
        with self.assertRaises(TypeError):
            labels.SHOOTER_LABEL_CONTRACT['facts'][0]['opcode'] = 0

    def test_duplicate_or_missing_critical_site_cannot_keep_a_partial_proof_valid(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            changed = copy.deepcopy(descriptor)
            changed['facts'][1] = copy.deepcopy(changed['facts'][0])
            self.assertEqual(self.verify(repo, inputs, changed, specs)['blockers'], ['native_critical_fact_sites_missing_duplicated_or_unreviewed'])
            changed = copy.deepcopy(descriptor)
            changed['facts'].pop(1)
            self.assertEqual(self.verify(repo, inputs, changed, specs)['blockers'], ['native_critical_fact_sites_missing_duplicated_or_unreviewed'])

    def test_duplicated_or_shifted_forwarding_path_cannot_replace_canonical_identity(self):
        with tempfile.TemporaryDirectory() as folder:
            repo, inputs, descriptor, specs = self.fixture(Path(folder))
            changed = copy.deepcopy(descriptor)
            changed['preserved_register_paths'][0] = copy.deepcopy(changed['preserved_register_paths'][1])
            self.assertEqual(self.verify(repo, inputs, changed, specs)['blockers'], ['native_byte_forwarding_path_identity_missing_duplicated_or_unreviewed'])
            changed = copy.deepcopy(descriptor)
            changed['preserved_register_paths'][0]['start_address'] += 4
            self.assertEqual(self.verify(repo, inputs, changed, specs)['blockers'], ['native_byte_forwarding_path_identity_missing_duplicated_or_unreviewed'])

    def test_actual_original_helpers_and_wrappers_replay_without_skip(self):
        repo = Path(os.environ.get('TOKIMEMO_TEXT_SOURCE_ROOT', '/work/.asset-cache/shared/source-snapshot-phase2-687b5ce7'))
        inputs = Path(os.environ.get('TOKIMEMO_TEXT_INPUT_ROOT', '/work/disc/files'))
        result = labels.verify_shooter_label_contract(repo, inputs)
        self.assertTrue(result['verified'], result.get('blockers'))
        self.assertEqual(result['source_pins_verified'], 58)
        self.assertEqual(result['critical_facts_verified'], 39)
        self.assertEqual(sum(p['instruction_checks'][0]['instruction_words'] for p in result['code_proof']['proofs'].values()), 569)
        self.assertEqual(len(result['code_proof']['proofs']), 4)
        self.assertEqual(result['read_specs']['shooter_draw_label']['callee_address'], 0x801357B0)
        self.assertEqual(result['read_specs']['shooter_draw_ranking_text']['callee_address'], 0x801371B4)


if __name__ == '__main__':
    unittest.main()
