#!/usr/bin/env python3
"""
Unit and integration tests for release_catalog.py.

Tests:
1. Release catalog profile definitions and versions.txt loader
2. PS1 release identification for all 5 cataloged releases + ambiguous rev1/shokai fallback
3. Boot target validation from SYSTEM.CNF
4. Corrupt/unknown executable handling
5. Disc image reading (MODE2/2352 raw track)
6. CUE sheet parsing and resolution
7. ZIP container handling (files zip and cue/bin zip)
8. Content compatibility separation (container valid vs release supported)
"""

import os
import shutil
import tempfile
import unittest
import zipfile

from release_catalog import (
    DEFAULT_PROFILES,
    COMMON_BYTE_IDENTICAL_OVERLAYS,
    OVERLAY_NAMES,
    ReleaseCatalog,
    ReleaseProfile,
    DiscReader,
    identify_release,
)


class TestReleaseCatalogProfiles(unittest.TestCase):
    """Test release profiles and catalog loader."""

    def test_default_profiles_contain_all_five_releases(self):
        catalog = ReleaseCatalog()
        expected_keys = [
            "best_slpm_86053",
            "rev4_slps_00065_v143",
            "rev2_slps_00065_v125",
            "rev1_slps_00065_v110",
            "shokai_slps_00064_v110",
        ]
        for key in expected_keys:
            self.assertIn(key, catalog.profiles)
            profile = catalog.profiles[key]
            self.assertTrue(profile.exe_sha1)
            self.assertGreater(profile.exe_size, 0)
            self.assertEqual(len(profile.overlays), 27)

    def test_only_best_is_supported_for_extraction(self):
        catalog = ReleaseCatalog()
        for key, profile in catalog.profiles.items():
            if key == "best_slpm_86053":
                self.assertTrue(profile.supported_for_content_extraction)
            else:
                self.assertFalse(profile.supported_for_content_extraction)

    def test_load_from_versions_txt(self):
        # /workspace/config/versions.txt in container
        versions_path = os.path.abspath(
            os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..", "..", "config", "versions.txt")
        )
        if not os.path.exists(versions_path):
            versions_path = "/workspace/config/versions.txt"
        if os.path.exists(versions_path):
            catalog = ReleaseCatalog.load_from_versions_txt(versions_path)
            self.assertIn("best_slpm_86053", catalog.profiles)
            self.assertEqual(len(catalog.profiles), 5)


class TestReleaseIdentification(unittest.TestCase):
    """Test identification of all 5 PS1 releases + ambiguous rev1/shokai fallback."""

    def setUp(self):
        self.catalog = ReleaseCatalog()

    def test_identify_best_release(self):
        eval_res = self.catalog.identify_executable(
            exe_name="SLPM_86.053",
            exe_size=667648,
            exe_sha1="e823bd844a8f8fa4d05483b59c66bc54b8393b26",
            cnf_text="BOOT = cdrom:\\SLPM_86.053;1\r\nTCB = 4\r\nEVENT = 16\r\nSTACK = 801FFFF0\r\n",
            vol_id="VX009J2",
            vol_set_id="TOKIMEKIMEMORIALBEST",
        )
        self.assertEqual(eval_res["release_id"], "best")
        self.assertEqual(eval_res["canonical_profile"], "best_slpm_86053")
        self.assertTrue(eval_res["supported_for_content_extraction"])
        self.assertTrue(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "OK_PLAYSTATION_THE_BEST")

    def test_identify_rev4_release(self):
        eval_res = self.catalog.identify_executable(
            exe_name="PSX.EXE",
            exe_size=667648,
            exe_sha1="60305396bde6f4dfb7187a328c320638aaa7df72",
            cnf_text="BOOT = cdrom:\\PSX.EXE;1\r\nTCB = 4\r\nEVENT = 16\r\nSTACK = 801FFFF0\r\n",
            vol_id="TOKIMEKIMEMORIAL",
            vol_set_id="SLPS00065VER143",
        )
        self.assertEqual(eval_res["release_id"], "rev4")
        self.assertEqual(eval_res["canonical_profile"], "rev4_slps_00065_v143")
        self.assertFalse(eval_res["supported_for_content_extraction"])
        self.assertTrue(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_REV4_UNSUPPORTED")

    def test_identify_rev2_release(self):
        eval_res = self.catalog.identify_executable(
            exe_name="PSX.EXE",
            exe_size=690176,
            exe_sha1="6c3eafe5df4a5dbf5dd596971752efaf7669150b",
            cnf_text="BOOT = cdrom:\\PSX.EXE;1\r\nTCB = 4\r\nEVENT = 16\r\nSTACK = 801FFFF0\r\n",
            vol_id="TOKIMEKIMEMORIAL",
            vol_set_id="SLPS00065VER125",
        )
        self.assertEqual(eval_res["release_id"], "rev2")
        self.assertEqual(eval_res["canonical_profile"], "rev2_slps_00065_v125")
        self.assertFalse(eval_res["supported_for_content_extraction"])
        self.assertTrue(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_REV2_UNSUPPORTED")

    def test_identify_rev1_release_with_pvd(self):
        eval_res = self.catalog.identify_executable(
            exe_name="PSX.EXE",
            exe_size=690176,
            exe_sha1="2d8d474816e4bf937b09dc3b017f550f7ce4e14b",
            cnf_text="BOOT = cdrom:\\PSX.EXE;1\r\nTCB = 4\r\nEVENT = 16\r\nSTACK = 801FFFF0\r\n",
            vol_id="TOKIMEKIMEMORIAL",
            vol_set_id="SLPS00065VER110",
        )
        self.assertEqual(eval_res["release_id"], "rev1")
        self.assertEqual(eval_res["canonical_profile"], "rev1_slps_00065_v110")
        self.assertFalse(eval_res["supported_for_content_extraction"])
        self.assertTrue(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_REV1_UNSUPPORTED")

    def test_identify_shokai_release_with_pvd(self):
        eval_res = self.catalog.identify_executable(
            exe_name="PSX.EXE",
            exe_size=690176,
            exe_sha1="2d8d474816e4bf937b09dc3b017f550f7ce4e14b",
            cnf_text="BOOT = cdrom:\\PSX.EXE;1\r\nTCB = 4\r\nEVENT = 16\r\nSTACK = 801FFFF0\r\n",
            vol_id="TOKIMEKIMEMORIAL",
            vol_set_id="SLPS00064VER110",
        )
        self.assertEqual(eval_res["release_id"], "shokai")
        self.assertEqual(eval_res["canonical_profile"], "shokai_slps_00064_v110")
        self.assertFalse(eval_res["supported_for_content_extraction"])
        self.assertTrue(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_SHOKAI_UNSUPPORTED")

    def test_identify_rev1_or_shokai_ambiguous_without_pvd(self):
        eval_res = self.catalog.identify_executable(
            exe_name="PSX.EXE",
            exe_size=690176,
            exe_sha1="2d8d474816e4bf937b09dc3b017f550f7ce4e14b",
            cnf_text="BOOT = cdrom:\\PSX.EXE;1\r\n",
            vol_id="",
            vol_set_id="",
        )
        self.assertEqual(eval_res["release_id"], "rev1")
        self.assertEqual(eval_res["canonical_profile"], "rev1_or_shokai_v110")
        self.assertFalse(eval_res["supported_for_content_extraction"])
        self.assertTrue(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_REV1_OR_SHOKAI_UNSUPPORTED")

    def test_boot_target_mismatch(self):
        eval_res = self.catalog.identify_executable(
            exe_name="SLPM_86.053",
            exe_size=667648,
            exe_sha1="e823bd844a8f8fa4d05483b59c66bc54b8393b26",
            cnf_text="BOOT = cdrom:\\PSX.EXE;1\r\n",
            vol_id="VX009J2",
        )
        self.assertFalse(eval_res["boot_target_valid"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_INVALID_BOOT_TARGET")

    def test_unknown_executable(self):
        eval_res = self.catalog.identify_executable(
            exe_name="UNKNOWN.EXE",
            exe_size=123456,
            exe_sha1="0000000000000000000000000000000000000000",
            cnf_text="BOOT = cdrom:\\UNKNOWN.EXE;1\r\n",
        )
        self.assertEqual(eval_res["release_id"], "unknown")
        self.assertFalse(eval_res["supported_for_content_extraction"])
        self.assertEqual(eval_res["diagnostic_code"], "ERR_UNKNOWN_EXECUTABLE")


class TestContentCompatibility(unittest.TestCase):
    """Test separation of container validity from release support and overlay compatibility."""

    def setUp(self):
        self.catalog = ReleaseCatalog()

    def test_best_all_27_identical(self):
        best_profile = self.catalog.profiles["best_slpm_86053"]
        report = self.catalog.evaluate_content_compatibility(
            identified_profile_id="best_slpm_86053",
            found_overlays=best_profile.overlays,
        )
        self.assertEqual(report["status"], "FULLY_COMPATIBLE")
        self.assertTrue(report["supported_for_content_extraction"])
        self.assertEqual(report["identical_overlay_count"], 27)
        self.assertEqual(report["total_overlay_count"], 27)

    def test_rev4_has_3_identical_overlays(self):
        rev4_profile = self.catalog.profiles["rev4_slps_00065_v143"]
        report = self.catalog.evaluate_content_compatibility(
            identified_profile_id="rev4_slps_00065_v143",
            found_overlays=rev4_profile.overlays,
        )
        self.assertFalse(report["supported_for_content_extraction"])
        self.assertEqual(report["identical_overlay_count"], 3)
        self.assertEqual(report["total_overlay_count"], 27)
        self.assertEqual(report["status"], "INCOMPATIBLE_REV4_SLPS_00065_V143")
        self.assertIn("EVENT.EXN", report["common_identical_overlays"])
        self.assertIn("GYOZI.EXN", report["common_identical_overlays"])
        self.assertIn("O.BIN", report["common_identical_overlays"])

    def test_rev1_and_shokai_identical_to_each_other(self):
        rev1_profile = self.catalog.profiles["rev1_slps_00065_v110"]
        shokai_profile = self.catalog.profiles["shokai_slps_00064_v110"]
        self.assertEqual(rev1_profile.exe_sha1, shokai_profile.exe_sha1)
        self.assertEqual(rev1_profile.overlays, shokai_profile.overlays)


class TestDiscAndContainerInspection(unittest.TestCase):
    """Test inspecting real workspace sources and synthetic containers."""

    def test_inspect_real_files_directory(self):
        files_dir = "/workspace/disc/files"
        if not os.path.isdir(files_dir):
            files_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..", "..", "disc", "files"))
        if os.path.isdir(files_dir):
            res = identify_release(files_dir)
            container = res["container"]
            eval_res = res["release"]
            compat = res["compatibility"]
            self.assertTrue(container["valid"])
            self.assertEqual(container["type"], "directory")
            self.assertEqual(eval_res["canonical_profile"], "best_slpm_86053")
            self.assertTrue(eval_res["supported_for_content_extraction"])
            self.assertEqual(compat["status"], "FULLY_COMPATIBLE")
            self.assertEqual(compat["identical_overlay_count"], 27)

    def test_inspect_real_raw_disc(self):
        track1_path = "/workspace/disc/track1.bin"
        if not os.path.exists(track1_path):
            track1_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..", "..", "disc", "track1.bin"))
        if os.path.exists(track1_path):
            res = identify_release(track1_path)
            container = res["container"]
            eval_res = res["release"]
            compat = res["compatibility"]
            self.assertTrue(container["valid"])
            self.assertEqual(container["type"], "raw_image")
            self.assertEqual(container["volume_id"], "VX009J2")
            self.assertEqual(container["volume_set_id"], "TOKIMEKIMEMORIALBEST")
            self.assertEqual(eval_res["canonical_profile"], "best_slpm_86053")
            self.assertTrue(eval_res["supported_for_content_extraction"])
            self.assertEqual(compat["status"], "FULLY_COMPATIBLE")
            self.assertEqual(compat["identical_overlay_count"], 27)

    def test_inspect_real_cue_sheet(self):
        cue_path = "/workspace/disc/Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best).cue"
        if not os.path.exists(cue_path):
            cue_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..", "..", "disc", "Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best).cue"))
        if os.path.exists(cue_path):
            res = identify_release(cue_path)
            container = res["container"]
            eval_res = res["release"]
            compat = res["compatibility"]
            self.assertTrue(container["valid"])
            self.assertEqual(container["type"], "cue")
            self.assertEqual(eval_res["canonical_profile"], "best_slpm_86053")
            self.assertTrue(eval_res["supported_for_content_extraction"])
            self.assertEqual(compat["status"], "FULLY_COMPATIBLE")

    def test_synthetic_zip_with_files(self):
        temp_dir = tempfile.mkdtemp()
        try:
            zip_path = os.path.join(temp_dir, "test_game.zip")
            with zipfile.ZipFile(zip_path, "w") as zf:
                zf.writestr("SYSTEM.CNF", "BOOT = cdrom:\\SLPM_86.053;1\r\n")
                zf.writestr("SLPM_86.053", b"dummy_exe_content")

            res = identify_release(zip_path)
            container = res["container"]
            eval_res = res["release"]
            self.assertTrue(container["valid"])
            self.assertEqual(container["type"], "zip_files")
            self.assertEqual(container["boot_executable"], "SLPM_86.053")
            self.assertEqual(eval_res["release_id"], "unknown")
            self.assertEqual(eval_res["diagnostic_code"], "ERR_UNKNOWN_EXECUTABLE")
        finally:
            shutil.rmtree(temp_dir)

    def test_synthetic_zip_with_cue_and_bin(self):
        temp_dir = tempfile.mkdtemp()
        try:
            zip_path = os.path.join(temp_dir, "disc_archive.zip")
            cue_content = 'FILE "game.bin" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n'
            with zipfile.ZipFile(zip_path, "w") as zf:
                zf.writestr("game.cue", cue_content)
                zf.writestr("game.bin", b"\x00" * (2352 * 20))

            res = identify_release(zip_path)
            container = res["container"]
            self.assertEqual(container["type"], "zip_image")
            self.assertFalse(container["valid"])
            self.assertIn("PVD", container["error"])
        finally:
            shutil.rmtree(temp_dir)


if __name__ == "__main__":
    unittest.main()
