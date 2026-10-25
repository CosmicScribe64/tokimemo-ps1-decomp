"""Invented boundary fixtures and metadata checks for MASTER talk layouts."""
import copy
import hashlib
import struct
import unittest

from tools.assets.script_profiles_master import PROFILES, RESOURCE_PROFILES, verify_boundaries
from tools.assets.text import parse_script


class MasterProfileTests(unittest.TestCase):
    def fixture(self):
        base = 0x801E8000
        data = bytearray(128)
        for offset, values in ((0, (base + 64, base + 68)), (32, (base + 8, base + 9)),
                               (48, (base + 72, base + 76))):
            struct.pack_into("<II", data, offset, *values)
        data[8:10] = b"\xff\xff"
        return data, {"load_address": base, "sections": [{"id": "table-0", "group_count": 2,
                          "text_table": base, "command_table": base + 32, "voice_table": base + 48}]}

    def test_all_eight_talk_files_have_unique_pinned_loader_and_initializer_evidence(self):
        self.assertEqual(len(PROFILES), 8)
        self.assertEqual(len({profile["path"] for profile in PROFILES}), 8)
        self.assertEqual(sum(section["group_count"] for profile in PROFILES for section in profile["sections"]), 326)
        self.assertEqual({profile["tournament_class"] for profile in PROFILES}, {None, 0, 1, 2, 3, 4, 5})
        for profile in PROFILES:
            self.assertEqual(len(profile["sha256"]), 64)
            self.assertEqual(profile["size"], profile["load_sectors"] * 2048)
            self.assertTrue(profile["source_pins"])
            self.assertTrue(all(len(value) == 64 for value in profile["source_pins"].values()))
            self.assertTrue(any("cd_read_sectors" in evidence for evidence in profile["evidence"]))

    def test_linked_command_pointer_bounds_text_root(self):
        data, profile = self.fixture()
        result = verify_boundaries(data, profile)
        self.assertEqual(result["sections"][0]["parallel_root_slots_verified"], 6)
        self.assertTrue(result["unclaimed_bytes_are_not_proven_nontext"])
        profile["id"] = "invented-master"
        parsed = parse_script(bytes(data), "CDROM/MASTER/INVENTED.BIN", profile)
        self.assertEqual(len(parsed["records"]), 2)
        self.assertTrue(all(record["command"]["value"] == 255 for record in parsed["records"]))

    def test_ninth_container_keeps_resource_role_and_unproven_text_absence_explicit(self):
        self.assertEqual(len(RESOURCE_PROFILES), 1)
        resource = RESOURCE_PROFILES[0]
        self.assertEqual(resource["path"], "CDROM/MASTER/TEAR.BIN")
        self.assertEqual(resource["size"], resource["load_sectors"] * 2048)
        self.assertNotIn("sections", resource)
        self.assertTrue(resource["limits"])
        self.assertEqual(len({profile["path"] for profile in PROFILES + RESOURCE_PROFILES}), 9)

    def test_known_original_renderer_defect_is_explicit_and_unrepaired(self):
        profile = next(profile for profile in PROFILES if profile["id"] == "master-kosien")
        anomaly = profile["original_renderer_anomalies"][0]
        self.assertEqual((anomaly["group"], anomaly["line"], anomaly["command"]), (10, 0, 0))
        self.assertEqual(anomaly["relative_offset"] + 1, anomaly["text_length"])
        self.assertFalse(anomaly["repair_applied"])
        self.assertEqual(anomaly["runtime_outcome"], "not_proven")

    def test_wrong_count_or_nonminimal_first_command_is_rejected(self):
        for problem in ("count", "order", "target", "alignment"):
            with self.subTest(problem=problem):
                data, profile = self.fixture()
                if problem == "count":
                    profile["sections"][0]["group_count"] = 3
                elif problem == "order":
                    struct.pack_into("<II", data, 32, 0x801E8009, 0x801E8008)
                elif problem == "target":
                    struct.pack_into("<I", data, 48, 0xFFFFFFFF)
                else:
                    profile["sections"][0]["text_table"] += 1
                with self.assertRaises(ValueError):
                    verify_boundaries(data, profile)

    def test_input_hash_and_file_size_are_verified(self):
        data, profile = self.fixture()
        profile.update(sha256=hashlib.sha256(data).hexdigest(), size=len(data))
        verify_boundaries(data, profile)
        changed = copy.deepcopy(profile)
        changed["size"] += 1
        with self.assertRaises(ValueError):
            verify_boundaries(data, changed)
        data[100] ^= 1
        with self.assertRaises(ValueError):
            verify_boundaries(data, profile)


if __name__ == "__main__":
    unittest.main()
