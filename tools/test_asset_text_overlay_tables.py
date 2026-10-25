import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text import TextFormatError
from tools.assets.text_overlay_tables import (OVERLAY_TABLE_PROFILES, parse_overlay_fixed_tables,
                                              verify_profile_source_pins)


class OverlayFixedTableTests(unittest.TestCase):
    def test_preserves_bounded_string_empty_slot_and_full_unterminated_slot(self):
        data = b"\x82\xa0\0\xfe\0\0\0\0\0\0\0\0"
        profile = {"id": "fixture", "path": "fixture.bin", "load_address": 0x80132000,
                   "tables": [{"id": "slots", "role": "fixture_label",
                               "address": 0x80132000, "count": 3,
                               "stride": 4, "dimensions": [3], "context": ["slot_index"]}]}
        parsed = parse_overlay_fixed_tables(data, profile)
        rows = parsed["records"]
        self.assertEqual(rows[0]["slot_raw_hex"], "82a000fe")
        self.assertEqual(rows[0]["offset"], rows[0]["slot_offset"])
        self.assertEqual(rows[0]["length"], 2)
        self.assertEqual(rows[0]["role"], "fixture_label")
        self.assertEqual(rows[0]["content_raw_hex"], "82a0")
        self.assertEqual(rows[0]["bytes_after_terminator_hex"], "fe")
        self.assertEqual(rows[0]["text"]["source_decoded_text"], "あ")
        self.assertEqual(rows[1]["string_extent_status"], "nul_bounded_within_slot")
        self.assertEqual(rows[1]["text"]["source_decoded_text"], "")
        self.assertEqual(rows[2]["slot_raw_hex"], "00000000")
        self.assertEqual(parsed["coverage"]["empty_slots"], 2)
        self.assertEqual(parsed["coverage"]["all_slot_bytes_preserved"], True)

    def test_full_width_slot_is_raw_and_not_claimed_as_terminated_c_string(self):
        data = b"\x82\xa0\x82\xa2"
        profile = {"id": "fixture", "path": "fixture.bin", "load_address": 0x80132000,
                   "tables": [{"id": "slots", "address": 0x80132000, "count": 1,
                               "stride": 4, "dimensions": [1], "context": ["slot_index"]}]}
        row = parse_overlay_fixed_tables(data, profile)["records"][0]
        self.assertEqual(row["slot_raw_hex"], "82a082a2")
        self.assertIsNone(row["terminator_offset"])
        self.assertEqual(row["string_extent_status"], "no_nul_within_slot")
        self.assertIsNone(row["text"])

    def test_rejects_hash_mismatch_and_invalid_dimensions(self):
        data = b"\0\0"
        profile = {"id": "fixture", "path": "fixture.bin", "load_address": 0x80132000,
                   "sha256": "0" * 64,
                   "tables": [{"id": "slots", "address": 0x80132000, "count": 1,
                               "stride": 2, "dimensions": [1]}]}
        with self.assertRaises(TextFormatError):
            parse_overlay_fixed_tables(data, profile)
        del profile["sha256"]
        profile["tables"][0]["dimensions"] = [2]
        with self.assertRaises(TextFormatError):
            parse_overlay_fixed_tables(data, profile)

    def test_profiles_pin_known_overlay_tables_and_source_inputs(self):
        by_id = {profile["id"]: profile for profile in OVERLAY_TABLE_PROFILES}
        self.assertEqual(len(by_id), 7)
        self.assertEqual([(t["count"], t["stride"]) for t in by_id["geko-place-and-encounter-names"]["tables"]],
                         [(12, 20), (19, 33), (99, 33)])
        self.assertEqual([(t["count"], t["stride"]) for t in by_id["kangei-club-names"]["tables"]],
                         [(11, 20), (11, 21), (99, 33)])
        self.assertEqual(by_id["bunkaken-name-grid"]["tables"][0]["dimensions"], [3, 3])
        self.assertEqual(by_id["bunkasai-name-grid"]["tables"][0]["dimensions"], [3, 3])
        gifts = [table for profile in OVERLAY_TABLE_PROFILES for table in profile["tables"]
                 if table["id"] == "gift_names"]
        self.assertEqual(len(gifts), 5)
        self.assertTrue(all((table["count"], table["stride"], table["dimensions"], table["role"])
                            == (99, 33, [11, 3, 3], "item_name") for table in gifts))
        self.assertEqual(sum(table["count"] for p in OVERLAY_TABLE_PROFILES for table in p["tables"]), 587)
        shougatu_tables = {table["id"]: table for table in by_id["shougatu-name-choices"]["tables"]}
        self.assertEqual([(shougatu_tables[name]["count"], shougatu_tables[name]["stride"])
                          for name in ("fortune_result_names", "fortune_labels")], [(7, 5), (7, 5)])
        for profile in OVERLAY_TABLE_PROFILES:
            self.assertEqual(len(profile["sha256"]), 64)
            self.assertTrue(profile["source_pins"])

    def test_source_pin_verifier_accepts_exact_file_and_rejects_changed_or_escaping_pin(self):
        with tempfile.TemporaryDirectory() as temp, tempfile.TemporaryDirectory() as external:
            root = Path(temp).resolve()
            (root / "source.c").write_bytes(b"fixture source")
            digest = hashlib.sha256(b"fixture source").hexdigest()
            profile = {"id": "fixture", "source_pins": {"source.c": digest}}
            self.assertEqual(verify_profile_source_pins(root, profile)["source_pins_verified"], 1)
            profile["source_pins"]["source.c"] = "0" * 64
            with self.assertRaises(TextFormatError):
                verify_profile_source_pins(root, profile)
            profile["source_pins"] = {"../outside.c": digest}
            with self.assertRaises(TextFormatError):
                verify_profile_source_pins(root, profile)
            profile["source_pins"] = {}
            with self.assertRaises(TextFormatError):
                verify_profile_source_pins(root, profile)

    def test_source_pin_verifier_rejects_symlink_escape(self):
        with tempfile.TemporaryDirectory() as temp, tempfile.TemporaryDirectory() as external:
            root = Path(temp).resolve()
            outside = Path(external) / "source.c"
            outside.write_bytes(b"fixture source")
            (root / "source.c").symlink_to(outside)
            profile = {"id": "fixture", "source_pins": {
                "source.c": hashlib.sha256(b"fixture source").hexdigest()}}
            with self.assertRaises(TextFormatError):
                verify_profile_source_pins(root, profile)


if __name__ == "__main__":
    unittest.main()
