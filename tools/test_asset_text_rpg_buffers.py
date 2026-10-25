"""Invented original-image/source fixtures for initial mutable buffer views."""
import copy
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text import TextFormatError
from tools.assets.text_rpg_buffers import RPG_INITIAL_BUFFER_PROFILE, parse_rpg_initial_buffers


class RpgInitialBufferTests(unittest.TestCase):
    def fixture(self, root, bodies=None):
        root.mkdir(exist_ok=True)
        data = bytearray(512)
        base, table = 0x1000, 0x1040
        bodies = bodies or ['ＡＢ'.encode('shift_jis')] * 5
        slots = []
        lines = ['.section .data', 'dlabel pointers']
        for index in range(5):
            address = base + 128 + index * 40
            slots.append({'label': 'slot_' + str(index), 'address': address})
            raw = address.to_bytes(4, 'little')
            offset = table - base + index * 4
            data[offset:offset + 4] = raw
            lines.append(f'/* {offset:X} {table + index * 4:08X} {raw.hex().upper()} */ .word slot_{index}')
        lines.append('enddlabel pointers')
        for index, slot in enumerate(slots):
            address = slot['address']
            offset = address - base
            body = bodies[index]
            raw = body if len(body) == 40 else body + b'\0' + b'\0' * (39 - len(body))
            self.assertEqual(len(raw), 40)
            data[offset:offset + 40] = raw
            lines.extend([f'dlabel slot_{index}',
                          f'/* {offset:X} {address:08X} */ .byte ' + ', '.join(hex(v) for v in raw),
                          f'enddlabel slot_{index}'])
        (root / 'data.s').write_text('\n'.join(lines) + '\n')
        (root / 'proof.h').write_text('/* Invented source declaration witness. */\n')
        proofs = []
        for index in range(5):
            name, path = 'reader_' + str(index), 'code_' + str(index) + '.s'
            address = base + index * 4
            (root / path).write_text(f'glabel {name}\n/* {index * 4:X} {address:08X} 00000000 */ nop\n')
            proofs.append({'function': name, 'assembly_path': path, 'address': address, 'instruction_words': 1})
        profile = {'id': 'invented-initial-buffers', 'path': 'SYNTHETIC.EXN',
                   'sha256': hashlib.sha256(data).hexdigest(), 'size': len(data), 'load_address': base,
                   'stride': 40, 'pointer_table': {'assembly_path': 'data.s', 'label': 'pointers', 'address': table},
                   'buffers': slots, 'code_proofs': proofs,
                   'source_pins': {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in root.iterdir()}}
        return bytes(data), profile

    def test_body_full_storage_and_mutability_are_distinct(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root)
            result = parse_rpg_initial_buffers(data, root, profile)
            self.assertEqual(result['coverage']['initial_buffer_views'], 5)
            self.assertEqual(result['coverage']['claimed_bytes'], 220)
            for record in result['records']:
                raw = bytes.fromhex(record['full_raw_hex'])
                self.assertEqual(record['offset'], record['full_offset'])
                self.assertEqual(record['text']['raw_hex'], raw[:4].hex())
                self.assertEqual(record['full_tokens'][-2]['kind'], 'protected_source_terminator')
                self.assertEqual(record['full_tokens'][-2]['offset'], 4)
                self.assertEqual(record['full_tokens'][-1]['kind'], 'protected_slot_padding')
                self.assertEqual(record['padding_length'], 35)
                self.assertEqual(b''.join(bytes.fromhex(t['raw_hex']) for t in record['full_tokens']), raw)
                self.assertTrue(record['context']['initial_only'])
                self.assertTrue(record['context']['runtime_value_unproven'])
                self.assertTrue(record['context']['runtime_contents_unknown'])
                self.assertFalse(record['context']['execution_proven'])
                self.assertFalse(record['context']['runtime_read_value_proven'])
                self.assertFalse(record['context']['requires_no_overwrite_assumption'])
                self.assertEqual(record['context']['renderer_binding_status'], 'potential_source_reader_association')
                self.assertTrue(record['context']['renderer_model_only'])
                self.assertEqual(record['context']['tokenizer_model'], 'set_kanji_string')
                self.assertNotIn('category', record)

    def test_only_selected_bytes_are_serialized_and_unclaimed_ranges_keep_identity(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root)
            result = parse_rpg_initial_buffers(data, root, profile)
            chunks = [(r['offset'], r['raw_hex']) for r in result['pointer_table']['rows']]
            chunks += [(r['source']['offset'], r['raw_hex']) for r in result['physical_records']]
            restored, seen = bytearray(len(data)), bytearray(len(data))
            for offset, raw_hex in chunks:
                raw = bytes.fromhex(raw_hex)
                self.assertFalse(any(seen[offset:offset + len(raw)]))
                restored[offset:offset + len(raw)] = raw
                seen[offset:offset + len(raw)] = b'\1' * len(raw)
            self.assertEqual(sum(seen), 220)
            self.assertTrue(all(restored[i] == data[i] for i in range(len(data)) if seen[i]))
            for span in result['unclaimed_ranges']:
                self.assertEqual(set(span), {'offset', 'length', 'sha256', 'classification'})
                self.assertEqual(hashlib.sha256(data[span['offset']:span['offset'] + span['length']]).hexdigest(), span['sha256'])
                self.assertFalse(any(seen[span['offset']:span['offset'] + span['length']]))
            self.assertEqual(sum(r['length'] for r in result['unclaimed_ranges']) + sum(seen), len(data))
            self.assertFalse(result['coverage']['unclaimed_payload_serialized'])
            self.assertFalse(result['coverage']['serialized_whole_file_reconstructible'])
            self.assertFalse(result['coverage']['whole_file_text_absence_proven'])

    def test_consistently_repinned_wrong_pointer_still_fails_layout(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root)
            changed = bytearray(data)
            changed[68:72] = changed[64:68]
            old = (0x10A8).to_bytes(4, 'little').hex().upper()
            new = (0x1080).to_bytes(4, 'little').hex().upper()
            path = root / 'data.s'
            path.write_text(path.read_text().replace(old, new))
            profile['source_pins']['data.s'] = hashlib.sha256(path.read_bytes()).hexdigest()
            profile['sha256'] = hashlib.sha256(changed).hexdigest()
            with self.assertRaisesRegex(TextFormatError, 'pointer word'):
                parse_rpg_initial_buffers(changed, root, profile)

    def test_slot_bounds_and_exact_assembly_extent_are_enforced(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root)
            changed = copy.deepcopy(profile)
            changed['stride'] = 41
            with self.assertRaisesRegex(TextFormatError, 'exact declared'):
                parse_rpg_initial_buffers(data, root, changed)
            changed = copy.deepcopy(profile)
            changed['buffers'][0]['address'] = 0x2000
            with self.assertRaises(TextFormatError):
                parse_rpg_initial_buffers(data, root, changed)

    def test_no_nul_search_crosses_into_adjacent_slot(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root, [b'\x82\xa0' * 20, b'', b'', b'', b''])
            with self.assertRaisesRegex(TextFormatError, 'no NUL inside'):
                parse_rpg_initial_buffers(data, root, profile)

    def test_source_and_original_instruction_pins_are_required(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root)
            (root / 'proof.h').write_text('/* Changed declaration witness. */\n')
            with self.assertRaisesRegex(TextFormatError, 'reviewed pin'):
                parse_rpg_initial_buffers(data, root, profile)
            profile['source_pins']['proof.h'] = hashlib.sha256((root / 'proof.h').read_bytes()).hexdigest()
            changed = bytearray(data)
            changed[0] = 1
            profile['sha256'] = hashlib.sha256(changed).hexdigest()
            with self.assertRaisesRegex(TextFormatError, 'instruction differs'):
                parse_rpg_initial_buffers(changed, root, profile)

    def test_outside_repository_source_symlink_is_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder) / 'source'
            data, profile = self.fixture(root)
            outside = root.parent / 'outside.h'
            outside.write_bytes((root / 'proof.h').read_bytes())
            (root / 'proof.h').unlink()
            (root / 'proof.h').symlink_to(outside)
            with self.assertRaisesRegex(TextFormatError, 'escapes repository'):
                parse_rpg_initial_buffers(data, root, profile)

    def test_malformed_codec_preserves_raw_without_replacement_codepoints(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root, [b'\x81\x7f', b'\x82', b'', b'', b''])
            result = parse_rpg_initial_buffers(data, root, profile)
            for record in result['records'][:2]:
                self.assertIsNone(record['text']['source_decoded_text'])
                self.assertEqual(b''.join(bytes.fromhex(t['raw_hex']) for t in record['full_tokens']), bytes.fromhex(record['full_raw_hex']))
            self.assertEqual(sum(v['code'] == 'initial_source_encoding_unsupported' for v in result['issues']), 2)
            self.assertNotIn('\ufffd', repr(result))

    def test_source_label_and_function_renames_preserve_physical_and_view_ids(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            data, profile = self.fixture(root)
            first = parse_rpg_initial_buffers(data, root, profile)
            path = root / 'data.s'
            path.write_text(path.read_text().replace('slot_', 'renamed_slot_'))
            for slot in profile['buffers']:
                slot['label'] = slot['label'].replace('slot_', 'renamed_slot_')
            profile['source_pins']['data.s'] = hashlib.sha256(path.read_bytes()).hexdigest()
            for proof in profile['code_proofs']:
                path = root / proof['assembly_path']
                old = proof['function']
                proof['function'] = 'renamed_' + old
                path.write_text(path.read_text().replace('glabel ' + old, 'glabel ' + proof['function']))
                profile['source_pins'][proof['assembly_path']] = hashlib.sha256(path.read_bytes()).hexdigest()
            second = parse_rpg_initial_buffers(data, root, profile)
            self.assertEqual([r['id'] for r in first['records']], [r['id'] for r in second['records']])
            self.assertEqual([r['id'] for r in first['physical_records']], [r['id'] for r in second['physical_records']])

    def test_default_profile_is_the_reviewed_five_slot_scope(self):
        self.assertEqual(RPG_INITIAL_BUFFER_PROFILE['stride'], 40)
        self.assertEqual(len(RPG_INITIAL_BUFFER_PROFILE['buffers']), 5)
        self.assertEqual(sum(p['instruction_words'] for p in RPG_INITIAL_BUFFER_PROFILE['code_proofs']), 604)
        for path in ('include/ovl/rpg_battle.h', 'include/common.h', 'include/main/text.h',
                     'include/game/game_state.h', 'include/game/display_records.h'):
            self.assertIn(path, RPG_INITIAL_BUFFER_PROFILE['source_pins'])


if __name__ == '__main__':
    unittest.main()
