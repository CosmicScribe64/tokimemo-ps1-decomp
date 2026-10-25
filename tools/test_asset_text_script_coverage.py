"""Tests for numeric script coverage diagnostics."""
import unittest
import os
from pathlib import Path

from tools.assets.script_coverage import audit_profile, audit_source_roots


class ScriptCoverageTests(unittest.TestCase):
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
