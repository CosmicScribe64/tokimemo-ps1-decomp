"""Tests for numeric script coverage diagnostics."""
import unittest
import os
import hashlib
import json
import tempfile
from pathlib import Path

from tools.assets.script_coverage import audit_profile, audit_source_roots
from tools.assets.script_coverage import _read_source, _function_body, _verify_profile_source_pins


class ScriptCoverageTests(unittest.TestCase):
    def test_current_object_and_data_names_require_exact_ledger_addresses(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root/'config/renames').mkdir(parents=True)
            (root/'src/ovl/renamed').mkdir(parents=True)
            (root/'config/renames/overlay-files.txt').write_text('file DATE 80132000 entry 0x80132000 evidence\n')
            (root/'config/renames/t27074-paths.txt').write_text('dir DATE DATE renamed - evidence\n')
            (root/'config/symbols_DATE.txt').write_text('table_text = 0x8015BDC0;\n')
            raw = (b'/* table_text = 0x80199999; } */\n'
                   b'void setup(void) { table_text = 0x80190000; unrelated = 0x80190004; }\n')
            (root/'src/ovl/renamed/entry.c').write_bytes(raw)
            (root/'snapshot-manifest.json').write_text(json.dumps({'files': {
                'src/ovl/renamed/entry.c': hashlib.sha256(raw).hexdigest()}}))
            relative, source = _read_source(root, 'src/ovl/DATE/80132000.c')
            self.assertEqual(relative, 'src/ovl/renamed/entry.c')
            self.assertIn('D_8015BDC0 = 0x80190000;', source)
            self.assertIn('unrelated = 0x80190004;', source)
            self.assertNotIn('0x80199999', source)
            (root/'src/ovl/renamed/entry.c').write_bytes(raw+b' ')
            with self.assertRaisesRegex(ValueError, 'pin mismatch'):
                _read_source(root, 'src/ovl/DATE/80132000.c')

    def test_profile_pin_rejects_changed_source_even_with_consistent_manifest(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            original = b'void setup(void) {}\n'
            changed = original+b' '
            (root/'proof.c').write_bytes(changed)
            (root/'snapshot-manifest.json').write_text(json.dumps({'files': {
                'proof.c': hashlib.sha256(changed).hexdigest()}}))
            profile = {'source_pins': {'proof.c': hashlib.sha256(original).hexdigest()}}
            with self.assertRaisesRegex(ValueError, 'profile source pin mismatch'):
                audit_source_roots(root, [profile])
            with self.assertRaisesRegex(ValueError, 'conflicting'):
                _verify_profile_source_pins(root, [profile, {'source_pins': {'proof.c': '0'*64}}])
            with self.assertRaisesRegex(ValueError, 'file missing'):
                _verify_profile_source_pins(root, [{'source_pins': {'missing.c': '0'*64}}])
            _verify_profile_source_pins(root, [{}])

    def test_initializer_rename_requires_one_exact_original_entry(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root/'src/ovl/TEST').mkdir(parents=True)
            asm = root/'asm/ovl/TEST/matchings/TEST/80132000'
            asm.mkdir(parents=True)
            (root/'src/ovl/TEST/80132000.c').write_text('void setup(void) { target = 0x80190000; }')
            (asm/'setup.s').write_text('glabel setup\n/* 0 80132000 00000000 */ nop\n')
            self.assertIn('target = 0x80190000;', _function_body(root, 'src/ovl/TEST/80132000.c::func_80132000')[1])
            with self.assertRaisesRegex(ValueError, 'one exact'):
                _function_body(root, 'src/ovl/TEST/80132000.c::func_80132004')

    def test_unclaimed_shift_jis_candidate_is_numeric_only(self):
        base = 0x1000
        data = bytearray(64)
        data[0:4] = (base + 12).to_bytes(4, "little")
        data[4:8] = (base + 16).to_bytes(4, "little")
        data[8:12] = (base + 20).to_bytes(4, "little")
        data[16] = 0xFF
        data[24:29] = b"\x82\xa0\x82\xa2\0"
        profile = {
            "id": "synthetic",
            "path": "SYNTHETIC.BIN",
            "load_address": base,
            "sections": [{
                "id": "main",
                "text_table": base,
                "command_table": base + 4,
                "voice_table": base + 8,
                "group_count": 1,
            }],
        }
        result = audit_profile(bytes(data), profile)
        self.assertEqual(result["text_record_count"], 0)
        self.assertEqual(result["unclaimed_candidate_strings"], [
            {"offset": 24, "length": 4},
        ])
        self.assertNotIn("text", result)

    def test_source_initialized_date_and_event_roots_are_profiled(self):
        source_root = os.environ.get("TOKIMEMO_TEXT_SOURCE_ROOT")
        if not source_root:
            self.skipTest("set TOKIMEMO_TEXT_SOURCE_ROOT for the read-only source-root integration audit")
        repo_root = Path(source_root).resolve(strict=True)
        result = audit_source_roots(repo_root)
        self.assertEqual(result["date_initialized_triad_count"], 293)
        self.assertEqual(result["date_directly_consumed_triad_count"], 293)
        self.assertEqual(result["event_initializer_triad_count"], 79)
        self.assertEqual(result["event_directly_consumed_triad_count"], 47)
        self.assertEqual(result["event_profile_triad_count"], 79)
        self.assertEqual(result["event_initializer_roots_missing_from_profiles"], 0)
        self.assertEqual(result["event_consumed_roots_missing_from_profiles"], 0)
        self.assertEqual(result["event_consumed_roots_missing_from_correct_profile"], 0)
        self.assertEqual(result["event_initializer_roots_without_direct_consumer"], 32)
        self.assertEqual(result["bunka"]["initializer_function_count"], 50)
        self.assertEqual(result["bunka"]["unique_initializer_triad_count"], 46)
        self.assertEqual(result["bunka"]["profile_triad_count"], 46)
        self.assertEqual(result["bunka"]["duplicate_initializer_count"], 4)
        self.assertEqual(result["bunka"]["source_roots_missing_from_profiles"], 0)
        self.assertEqual(result["bunka"]["profile_roots_not_in_source"], 0)
        other = result["other_and_p_birth_literal_initializers"]
        self.assertEqual(other["profile_count"], 21)
        self.assertEqual(other["missing_profile_roots"], 0)
        self.assertEqual(other["profile_roots_not_in_source"], 0)
        master = result["master_literal_initializers"]
        self.assertEqual(master["profile_count"], 8)
        self.assertEqual(master["missing_profile_roots"], 0)
        self.assertEqual(master["profile_roots_not_in_source"], 0)
        self.assertTrue(all(item["missing_expected_roots"] == 0
                            and item["unexpected_profile_roots"] == 0
                            for item in result["event_distribution"]))
        self.assertTrue(all(item["initialized_roots_missing_from_profile"] == 0
                            and item["profile_roots_not_in_initializer"] == 0
                            and item["initialized_roots_without_direct_consumer"] == 0
                            for item in result["date_fillers"]))


if __name__ == "__main__":
    unittest.main()
