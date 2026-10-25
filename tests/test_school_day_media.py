"""Bounded real-pack and negative checks for the school-day media adapter."""
import json
import shutil
import hashlib
import struct
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from validate_school_day_media import validate_pack


class MediaPackTests(unittest.TestCase):
    def setUp(self):
        self.pack = Path(".asset-cache/port/school-day-media-v3")
        validate_pack(str(self.pack))
        self.temp = Path(tempfile.mkdtemp(prefix="school-day-media-negative-"))

    def tearDown(self):
        shutil.rmtree(self.temp, ignore_errors=True)

    def _copy(self, name):
        target = self.temp / name
        shutil.copytree(self.pack, target)
        return target

    def test_actual_pack_has_exact_limited_components(self):
        doc = validate_pack(str(self.pack))
        self.assertEqual(set(doc["components"]), {"command_0_background", "command_0_sd", "command_7_sd"})
        self.assertIn("command_7_background", doc["unsupported"])
        for key in ("command_0_sd", "command_7_sd"):
            component = doc["components"][key]
            self.assertTrue(component["frames"])
            self.assertTrue(component["sequences"][0]["steps"])
            for step in component["sequences"][0]["steps"]:
                self.assertIn(step["frame_index"], [frame["frame_index"] for frame in component["frames"]])

    def test_rejects_tampered_payload(self):
        target = self._copy("tamper")
        image = next((target / "assets").glob("*.png"))
        image.write_bytes(image.read_bytes()[:-1] + b"X")
        with self.assertRaises(ValueError):
            validate_pack(str(target))

    def test_rejects_missing_unsupported_capability_explanation(self):
        target = self._copy("unsupported")
        path = target / "manifest.json"
        doc = json.loads(path.read_text())
        del doc["unsupported"]["command_7_background"]
        path.write_text(json.dumps(doc))
        with self.assertRaises(ValueError):
            validate_pack(str(target))

    def test_rejects_dot_segment_payload_path(self):
        target = self._copy("path")
        path = target / "manifest.json"
        doc = json.loads(path.read_text())
        doc["components"]["command_0_background"]["image"]["path"] = "../outside.png"
        path.write_text(json.dumps(doc))
        with self.assertRaises(ValueError):
            validate_pack(str(target))

    def _mutate_manifest(self, name, mutate):
        target = self._copy(name)
        path = target / "manifest.json"
        doc = json.loads(path.read_text())
        mutate(doc)
        path.write_text(json.dumps(doc))
        return target

    def test_rejects_wrong_original_input_provenance(self):
        target = self._mutate_manifest("wrong-source", lambda d: d["provenance"].update(original_input_sha256="0" * 64))
        with self.assertRaisesRegex(ValueError, "provenance"):
            validate_pack(str(target))

    def test_rejects_command7_background_capability_overclaim(self):
        target = self._mutate_manifest("overclaim", lambda d: d["route_capabilities"].append("command_7_background"))
        with self.assertRaisesRegex(ValueError, "capabilities"):
            validate_pack(str(target))

    def test_rejects_unpinned_native_route_profile(self):
        target = self._mutate_manifest("route-profile", lambda d: d["components"]["command_0_sd"]["native_route"].update(profile="invalid-profile"))
        with self.assertRaisesRegex(ValueError, "native route"):
            validate_pack(str(target))

    def test_rejects_sequence_zero_beyond_native_consumer_limit(self):
        def expand(d):
            steps = d["components"]["command_0_sd"]["sequences"][0]["steps"]
            steps[:] = [dict(steps[0]) for _ in range(129)]
        target = self._mutate_manifest("sequence-129", expand)
        with self.assertRaisesRegex(ValueError, "1..128"):
            validate_pack(str(target))

    def test_rejects_wrong_active_crop(self):
        target = self._mutate_manifest("crop", lambda d: d["components"]["command_0_background"].update(active_crop=[0, 0, 1, 1]))
        with self.assertRaisesRegex(ValueError, "background"):
            validate_pack(str(target))

    def test_rejects_bad_frame_origin_duration_and_parts(self):
        for name, change in [
            ("origin", lambda d: d["components"]["command_0_sd"]["frames"][0].update(origin="bad")),
            ("duration", lambda d: d["components"]["command_0_sd"]["sequences"][0]["steps"][0].update(duration=32768)),
            ("part", lambda d: d["components"]["command_0_sd"]["frames"][0]["parts"][0].update(width=129)),
            ("palette", lambda d: d["components"]["command_0_sd"]["frames"][0].update(source_palette_words=[0])),
        ]:
            target = self._mutate_manifest(name, change)
            with self.subTest(case=name), self.assertRaises(ValueError):
                validate_pack(str(target))

    def test_rejects_assets_directory_symlink(self):
        target = self._copy("asset-link")
        shutil.rmtree(target / "assets")
        (target / "assets").symlink_to(self.pack / "assets", target_is_directory=True)
        with self.assertRaisesRegex(ValueError, "assets path"):
            validate_pack(str(target))

    def test_png_dimensions_are_bounded_before_decoder(self):
        target = self._copy("huge-png-header")
        manifest_path = target / "manifest.json"
        doc = json.loads(manifest_path.read_text())
        spec = doc["components"]["command_0_background"]["image"]
        image_path = target / "assets" / spec["path"]
        data = bytearray(image_path.read_bytes())
        struct.pack_into(">II", data, 16, 0x7fffffff, 0x7fffffff)
        image_path.write_bytes(data)
        spec["sha256"] = hashlib.sha256(data).hexdigest()
        manifest_path.write_text(json.dumps(doc))
        with mock.patch("tools.assets.visual_text_equivalence._decode_png", side_effect=AssertionError("decoder called before bounds")):
            with self.assertRaisesRegex(ValueError, "PNG dimensions"):
                validate_pack(str(target))


if __name__ == "__main__":
    unittest.main()
