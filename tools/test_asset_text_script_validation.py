"""Synthetic checks for source-profile validation; run in Docker (T-28001)."""
import copy
import struct
import unittest

from tools.assets.script_validation import checked_cd_trailer, validate_script
from tools.assets.text import parse_script
from tools.test_asset_text import BASE, fixture


class TrailerTests(unittest.TestCase):
    def test_marker_and_checksum_are_independent(self):
        data = bytearray(2048)
        data[0:3] = b"\x01\x02\x03"
        data[-4:-2] = (6).to_bytes(2, "big")
        data[-2:] = b"\x12\x10"
        self.assertTrue(checked_cd_trailer(data)["checksum_matches"])
        data[0] = 7
        result = checked_cd_trailer(data)
        self.assertTrue(result["marker_present"])
        self.assertFalse(result["checksum_matches"])

    def test_nonsector_data_has_no_assumed_trailer(self):
        self.assertEqual(checked_cd_trailer(b"invented")["status"], "not_sector_sized")


class ValidationTests(unittest.TestCase):
    def test_valid_roots_and_source_bytes(self):
        data, profile = fixture()
        report = validate_script(data, profile["path"], profile)
        self.assertTrue(report["structural_checks_pass"])
        self.assertEqual(report["groups"], 1)
        self.assertEqual(report["strict_source_decode_failures"], 0)

    def test_adjacent_unexported_group_is_reported(self):
        data, profile = fixture()
        data = bytearray(data)
        for offset, target in ((0, 32), (4, 32), (8, 48), (12, 48), (16, 64), (20, 64)):
            struct.pack_into("<I", data, offset, BASE + target)
        section = profile["sections"][0]
        section.update(command_table=BASE + 8, voice_table=BASE + 16)
        report = validate_script(bytes(data), profile["path"], profile)
        self.assertEqual(sum(i["code"] == "possible_omitted_group" for i in report["issues"]), 2)

    def test_overlapping_root_extents_are_reported(self):
        data, profile = fixture()
        profile["sections"][0]["voice_table"] = BASE
        report = validate_script(data, profile["path"], profile)
        self.assertIn("overlapping_root_extents", [i["code"] for i in report["issues"]])

    def test_edited_token_stream_and_terminator_are_detected(self):
        data, profile = fixture()
        parsed = parse_script(data, profile["path"], profile)
        parsed["records"][0]["text"]["tokens"] = []
        parsed["records"][-1]["command"]["value"] = 0
        report = validate_script(data, profile["path"], profile, parsed)
        codes = {i["code"] for i in report["issues"]}
        self.assertIn("token_bytes_not_lossless", codes)
        self.assertIn("noncontiguous_or_unterminated_group", codes)

    def test_source_tab_decodes_but_renderer_anomaly_is_separate(self):
        data, profile = fixture(raw=b"\x09\x82\xa0")
        report = validate_script(data, profile["path"], profile)
        self.assertTrue(report["structural_checks_pass"])
        self.assertEqual(report["strict_source_decode_failures"], 0)
        self.assertEqual(report["renderer_anomalies"][0]["tab_offsets"], [96])

    def test_empty_menu_does_not_invent_source_options(self):
        data, profile = fixture(command=3, raw=b"")
        report = validate_script(data, profile["path"], profile)
        self.assertTrue(report["menus"][0]["empty_source"])
        self.assertEqual(report["menus"][0]["source_rows"], 0)
        self.assertTrue(report["menus"][0]["counts_differ"])

    def test_reply_voice_extent_disagreement_is_detected(self):
        data, profile = fixture(command=2, raw=b"\x82\xa0q\x82\xa2")
        parsed = parse_script(data, profile["path"], profile)
        parsed["records"][0]["voice"]["pairs"].pop()
        report = validate_script(data, profile["path"], profile, parsed)
        self.assertIn("reply_voice_extent_mismatch", [i["code"] for i in report["issues"]])

    def test_missing_group_is_detected(self):
        data, profile = fixture()
        parsed = parse_script(data, profile["path"], profile)
        profile = copy.deepcopy(profile)
        profile["sections"][0]["group_count"] = 2
        report = validate_script(data, profile["path"], profile, parsed)
        self.assertIn("profile_record_groups_mismatch", [i["code"] for i in report["issues"]])


if __name__ == "__main__":
    unittest.main()
