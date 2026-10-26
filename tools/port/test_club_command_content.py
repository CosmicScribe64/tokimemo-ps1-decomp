"""Unit tests for Command 4 club content exporter and route resolution."""

import copy
import hashlib
import importlib.util
import json
import shutil
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("club_command_content.py")
spec = importlib.util.spec_from_file_location("club_command_content", MODULE_PATH)
content = importlib.util.module_from_spec(spec)
spec.loader.exec_module(content)


class ClubCommandContentTests(unittest.TestCase):
    def setUp(self):
        self.temp_root = Path(tempfile.mkdtemp(prefix="t29014-club-test-"))

    def tearDown(self):
        shutil.rmtree(self.temp_root)

    def test_all_eleven_clubs_route_resolution(self):
        expected_sets = [3, 31, 7, 11, 29, 25, 13, 15, 21, 19, 17]
        expected_bgs = [16215, 16224, 16234, 16244, 16252, 16260, 16168, 16177, 16206, 16197, 16187]
        for club_id in range(11):
            route = content.resolve_club_route(None, club_id, extra=15, month=4)
            self.assertEqual(route["command"], 4)
            self.assertEqual(route["club_id"], club_id)
            self.assertEqual(route["event_extra"], 15)
            self.assertEqual(route["variant"], 1)
            self.assertEqual(route["background_lba"], expected_bgs[club_id])
            self.assertEqual(route["sd_set_id"], expected_sets[club_id])
            self.assertEqual(route["sd_lba"], 27834 + 41 * expected_sets[club_id])
            self.assertEqual(route["season_domain"], "months 3–5 (non-summer table)")
            self.assertEqual(route["route_domain"], "ordinary school day, non-vacation, non-sick-rest")

    def test_club_none_and_out_of_bounds_rejected(self):
        for bad_id in (11, -1, 12, 99, "0", True):
            with self.subTest(bad_id=bad_id), self.assertRaises(ValueError):
                content.resolve_club_route(None, bad_id, extra=15)

    def test_unsupported_variants_rejected_before_execution(self):
        # extra != 15 (companion)
        for bad_extra in (0, 1, 5, 14, 16, True):
            with self.subTest(bad_extra=bad_extra), self.assertRaises(ValueError):
                content.resolve_club_route(None, 0, extra=bad_extra)

        # summer months
        for bad_month in (6, 7, 8):
            with self.subTest(bad_month=bad_month), self.assertRaises(ValueError):
                content.resolve_club_route(None, 0, extra=15, month=bad_month)

        # vacation
        with self.assertRaises(ValueError):
            content.resolve_club_route(None, 0, extra=15, month=4, vacation=1)

        # health / sick
        with self.assertRaises(ValueError):
            content.resolve_club_route(None, 0, extra=15, month=4, health=1)

    def test_safe_output_path_enforcement(self):
        cache = self.temp_root / ".asset-cache/port"
        cache.mkdir(parents=True)
        outside = self.temp_root / "outside"
        outside.mkdir()
        link = cache / "symlink_dir"
        link.symlink_to(outside, target_is_directory=True)

        for bad in (str(cache / "../escape"), str(link / "pack"), str(outside / "pack")):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                content._safe_output(self.temp_root, bad)

    def test_validate_pack_rejects_missing_and_corrupt(self):
        pack = self.temp_root / ".asset-cache/port/mock_pack"
        pack.mkdir(parents=True)
        # Missing manifest
        with self.assertRaisesRegex(ValueError, "manifest.json missing"):
            content.validate_pack(pack)

        # Corrupt manifest
        (pack / "manifest.json").write_text("{}", encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "invalid schema"):
            content.validate_pack(pack)

    def test_validate_exported_pack_if_present(self):
        root = Path(__file__).resolve()
        while root != root.parent and not (root / ".asset-cache/port").is_dir():
            root = root.parent
        candidate_pack = root / ".asset-cache/port/t29014-command4-club-v1"
        if candidate_pack.is_dir():
            doc = content.validate_pack(candidate_pack)
            self.assertEqual(doc["schema"], content.SCHEMA)
            self.assertEqual(len(doc["components"]), 11)
            self.assertEqual(len(doc["route_capabilities"]), 11)


if __name__ == "__main__":
    unittest.main()
