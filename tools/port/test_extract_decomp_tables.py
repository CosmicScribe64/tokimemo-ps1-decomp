#!/usr/bin/env python3
"""test_extract_decomp_tables.py: Unit tests for decomp table extraction and CP932 hex linter."""

import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

# Setup import path
ROOT = next(p for p in Path(__file__).resolve().parents if (p / "config/symbol_addrs.txt").is_file())
sys.path.insert(0, str(Path(__file__).resolve().parent))

from extract_decomp_tables import (
    KNOWN_UPSTREAM_BUGS,
    SCHEMA_BUNDLE,
    SCHEMA_DATE_SPOTS,
    SCHEMA_ENDING_MASTER,
    SCHEMA_INLINE_STRINGS,
    SCHEMA_PHONE_DIR,
    decode_cp932_hex,
    extract_all,
    extract_date_spot_tables,
    extract_ending_and_club_master_tables,
    extract_inline_c_strings,
    extract_phone_directory_tables,
    find_repo_root,
    lint_godot_hex,
    normalize_cjk_for_cp932,
    round_trip_cp932,
    write_extracted_tables,
)


class TestCP932Helpers(unittest.TestCase):
    """Test round-trip CP932 encoding, decoding, and normalization."""

    def test_round_trip_valid_japanese(self):
        cases = [
            ("藤崎詩織", "93a18de88e8d9044"),
            ("近所の公園", "8bdf8f8a82cc8cf68980"),
            ("ボーリング場", "837b815b838a8393834f8fea"),
            ("コンサート会場", "835283938354815b836789ef8fea"),
            ("スケート場", "83588350815b83678fea"),
            ("スキー場", "8358834c815b8fea"),
            ("早乙女好雄", "918189b38f978d449759"),
            ("体育祭", "91cc88e78dd5"),
        ]
        for text, expected_hex in cases:
            ok, h, err = round_trip_cp932(text)
            self.assertTrue(ok, f"Failed round-trip for {text}: {err}")
            self.assertEqual(h, expected_hex)

    def test_wave_dash_normalization(self):
        s_wave = "あ〜あ、留守みたいだな。"
        s_tilde = "あ～あ、留守みたいだな。"
        self.assertEqual(normalize_cjk_for_cp932(s_wave), s_tilde)
        ok, h, _ = round_trip_cp932(s_wave)
        self.assertTrue(ok)
        dec_ok, dec_text, _ = decode_cp932_hex(h)
        self.assertTrue(dec_ok)
        self.assertEqual(dec_text, s_tilde)

    def test_decode_cp932_hex_valid(self):
        ok, text, err = decode_cp932_hex("93a18de8")
        self.assertTrue(ok)
        self.assertEqual(text, "藤崎")

    def test_decode_cp932_hex_invalid(self):
        # Odd length
        ok, _, err = decode_cp932_hex("93a18")
        self.assertFalse(ok)
        self.assertIn("Odd length", err)

        # Non-hex characters
        ok, _, err = decode_cp932_hex("93a18z")
        self.assertFalse(ok)

        # Illegal multibyte sequence
        ok, _, err = decode_cp932_hex("87b9")
        self.assertFalse(ok)


class TestDecompTableExtraction(unittest.TestCase):
    """Test extraction of structured game tables from decomp C sources and symbol files."""

    def setUp(self):
        self.root = find_repo_root()

    def test_extract_date_spots(self):
        spots = extract_date_spot_tables(self.root)
        self.assertEqual(spots["schema"], SCHEMA_DATE_SPOTS)
        self.assertEqual(spots["place_count"], 21)
        self.assertEqual(len(spots["places"]), 21)

        # Verify spot 0 (近所の公園)
        p0 = spots["places"][0]
        self.assertEqual(p0["id"], 0)
        self.assertEqual(p0["name"], "近所の公園")
        self.assertEqual(p0["bytes_hex"], "8bdf8f8a82cc8cf68980")

        # Verify spot 10 (ボーリング場) has authentic 8fea (場)
        p10 = spots["places"][10]
        self.assertEqual(p10["id"], 10)
        self.assertEqual(p10["name"], "ボーリング場")
        self.assertTrue(p10["bytes_hex"].endswith("8fea"))

        # Check availability rules structure
        rules = spots["availability_rules"]
        self.assertEqual(len(rules["page_0_places"]), 10)
        self.assertEqual(len(rules["page_1_places"]), 11)
        self.assertTrue(len(rules["page_0_special_windows"]) >= 5)
        self.assertTrue(len(rules["page_1_special_windows"]) >= 10)

        # Check asset bindings
        assets = spots["asset_bindings"]
        self.assertTrue(len(assets["backgrounds"]) >= 2)
        self.assertIn("default", assets["audio_bank_sectors_by_place"])

    def test_extract_phone_directory(self):
        phone = extract_phone_directory_tables(self.root)
        self.assertEqual(phone["schema"], SCHEMA_PHONE_DIR)
        self.assertEqual(len(phone["directory_slots_14"]), 14)

        # Verify slot 0 is Yoshio
        s0 = phone["directory_slots_14"][0]
        self.assertEqual(s0["slot"], 0)
        self.assertEqual(s0["full_name"], "早乙女好雄")
        self.assertEqual(s0["target_girl"], 13)

        # Verify slot 12 is Miharu
        s12 = phone["directory_slots_14"][12]
        self.assertEqual(s12["slot"], 12)
        self.assertEqual(s12["full_name"], "館林見晴")
        self.assertEqual(s12["target_girl"], 11)

        # Verify slot 13 is Cancel
        s13 = phone["directory_slots_14"][13]
        self.assertEqual(s13["slot"], 13)
        self.assertEqual(s13["full_name"], "取り消し")
        self.assertIsNone(s13["target_girl"])

        # Check 13x7 digits
        digits = phone["phone_digits_13x7"]
        self.assertEqual(len(digits), 13)
        self.assertEqual(len(digits["0_shiori"]), 7)
        self.assertEqual(digits["0_shiori"], [0, 1, 2, 1, 0, 2, 0])

        # Check appraisal stars
        appraisal = phone["yoshio_appraisal"]
        self.assertEqual(appraisal["bomb_threshold"]["syoushin_min"], 60)
        self.assertEqual(len(phone["sign_day_thresholds"]), 12)
        self.assertEqual(len(phone["blood_types"]), 4)
        self.assertEqual(len(phone["club_names"]), 13)

    def test_extract_ending_and_club_master(self):
        end = extract_ending_and_club_master_tables(self.root)
        self.assertEqual(end["schema"], SCHEMA_ENDING_MASTER)

        # Check Miharu 18-step chance table
        miharu = end["miharu_chance_by_level"]
        self.assertEqual(len(miharu), 18)
        self.assertEqual(miharu[0], 1)
        self.assertEqual(miharu[-1], 256)

        # Check Shiori requirements
        shiori = end["shiori_requirements"]
        self.assertEqual(shiori["closeness_min"], 85)
        self.assertEqual(shiori["parameter_minimums"]["liberal_arts"], 130)
        self.assertEqual(shiori["parameter_minimums"]["trivia"], 120)
        self.assertEqual(shiori["parameter_minimums"]["looks"], 100)
        self.assertEqual(shiori["event_scene_counters_sum_min"], 8)
        self.assertEqual(shiori["points_awarded_on_match"], 785)

        # Check other girls scoring
        other = end["other_girls_scoring"]
        self.assertEqual(other["base_closeness_multiplier"], 4)
        self.assertEqual(len(other["syoushin_weights_by_girl"]), 12)
        self.assertEqual(len(other["mask_strong_1_5x_by_girl"]), 12)
        self.assertEqual(len(other["mask_normal_1_0x_by_girl"]), 12)
        self.assertEqual(len(other["param_curve_15"]), 15)
        self.assertEqual(len(other["stress_curve_15"]), 15)

        # Check club master
        cm = end["club_master"]
        self.assertEqual(cm["target_practice_points"], 350)
        self.assertEqual(cm["mastery_award_gate"]["required_grade_min"], 3)
        self.assertEqual(cm["mastery_award_gate"]["effective_practice_points_min"], 350)

    def test_extract_inline_c_strings(self):
        inline = extract_inline_c_strings(self.root)
        self.assertEqual(inline["schema"], SCHEMA_INLINE_STRINGS)
        self.assertTrue(inline["total_extracted"] > 100)
        self.assertEqual(inline["total_extracted"], inline["verified_roundtrip_count"])

    def test_extract_all_bundle_and_write(self):
        bundle = extract_all(self.root)
        self.assertEqual(bundle["schema"], SCHEMA_BUNDLE)
        self.assertIn("date_spots", bundle)
        self.assertIn("phone_directory", bundle)
        self.assertIn("ending_and_club_master", bundle)
        self.assertIn("inline_c_strings", bundle)

        with tempfile.TemporaryDirectory() as tmp_dir:
            tmp_path = Path(tmp_dir)
            write_extracted_tables(bundle, tmp_path)
            self.assertTrue((tmp_path / "date_spots.json").is_file())
            self.assertTrue((tmp_path / "phone_directory.json").is_file())
            self.assertTrue((tmp_path / "ending_and_club_master.json").is_file())
            self.assertTrue((tmp_path / "inline_c_strings.json").is_file())
            self.assertTrue((tmp_path / "decomp_tables_bundle.json").is_file())

            # Verify bundle loads back identically
            loaded = json.loads((tmp_path / "decomp_tables_bundle.json").read_text(encoding="utf-8"))
            self.assertEqual(loaded["schema"], SCHEMA_BUNDLE)


class TestGodotHexLinter(unittest.TestCase):
    """Test the codebase-wide CP932 hex literal linter (--lint-godot-hex)."""

    def setUp(self):
        self.root = find_repo_root()

    def test_lint_clean_file(self):
        # school_events_presenter.gd contains 61 clean CP932 hex pairs
        clean_file = self.root / "port" / "godot" / "presentation" / "school_events_presenter.gd"
        if clean_file.is_file():
            mismatches, entries = lint_godot_hex(target_dirs=[clean_file], root=self.root)
            self.assertEqual(mismatches, 0)
            self.assertTrue(len(entries) >= 50)
            self.assertTrue(all(e["status"] == "ok" for e in entries))

    def test_lint_synthetic_pairs(self):
        with tempfile.TemporaryDirectory() as tmp_dir:
            tmp_path = Path(tmp_dir)

            # File with valid pairs
            good_gd = tmp_path / "good.gd"
            good_gd.write_text(
                'const A := "93a18de8" # 藤崎\n'
                'const B := {"short_text": "如月", "short_hex": "94408c8e"}\n'
                'const C := {"text": "水族館", "bytes_hex": "908591b08ad9"}\n',
                encoding="utf-8",
            )
            mismatches, entries = lint_godot_hex(target_dirs=[good_gd], root=tmp_path)
            self.assertEqual(mismatches, 0)
            self.assertEqual(len(entries), 3)

            # File with mismatched comment
            bad_gd = tmp_path / "bad.gd"
            bad_gd.write_text(
                'const BAD := "93a18de8" # 虹野\n',  # 93a18de8 is 藤崎, not 虹野
                encoding="utf-8",
            )
            mismatches, entries = lint_godot_hex(target_dirs=[bad_gd], root=tmp_path)
            self.assertEqual(mismatches, 1)
            self.assertEqual(entries[0]["status"], "mismatch")
            self.assertEqual(entries[0]["decoded"], "藤崎")
            self.assertEqual(entries[0]["expected"], "虹野")

    def test_lint_prologue_presenter_is_clean(self):
        target = self.root / "port" / "godot" / "presentation" / "prologue_presenter.gd"
        if target.is_file():
            mismatches, entries = lint_godot_hex(target_dirs=[target], allow_known=False, root=self.root)
            self.assertEqual(mismatches, 0)
            self.assertGreater(len(entries), 10)

    def test_lint_holiday_appointment_is_clean(self):
        target = self.root / "port" / "godot" / "rules" / "holiday_appointment.gd"
        if target.is_file():
            mismatches, entries = lint_godot_hex(target_dirs=[target], allow_known=False, root=self.root)
            self.assertEqual(mismatches, 0)
            self.assertGreater(len(entries), 20)


class TestCLIIntegration(unittest.TestCase):
    """Test CLI commands and flags."""

    def setUp(self):
        self.script_path = Path(__file__).resolve().parent / "extract_decomp_tables.py"

    def test_cli_verify_cp932(self):
        proc = subprocess.run(
            [sys.executable, str(self.script_path), "--verify-cp932"],
            capture_output=True,
            text=True,
        )
        self.assertEqual(proc.returncode, 0)
        self.assertIn("Extracted 21 date spots", proc.stdout)
        self.assertIn("Extracted 14 phone directory slots", proc.stdout)

    def test_cli_extract_all(self):
        with tempfile.TemporaryDirectory() as tmp_dir:
            proc = subprocess.run(
                [sys.executable, str(self.script_path), "--extract-all", "--output-dir", tmp_dir],
                capture_output=True,
                text=True,
            )
            self.assertEqual(proc.returncode, 0)
            self.assertTrue((Path(tmp_dir) / "decomp_tables_bundle.json").is_file())

    def test_cli_lint_godot_hex_clean_target(self):
        clean_target = find_repo_root() / "port" / "godot" / "presentation" / "school_events_presenter.gd"
        if clean_target.is_file():
            proc = subprocess.run(
                [sys.executable, str(self.script_path), "--lint-godot-hex", "--target-dir", str(clean_target)],
                capture_output=True,
                text=True,
            )
            self.assertEqual(proc.returncode, 0)
            self.assertIn("Zero CP932 hex literal discrepancies found!", proc.stdout)


if __name__ == "__main__":
    unittest.main(verbosity=2)
