"""Invented boundary/selector fixtures only; original-disc proof is external."""

import copy
import hashlib
import json
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.assets import weekday_56_profile as profile
from tools.assets.images import _png
from tools.port import weekday_56_content as content


class Weekday56Tests(unittest.TestCase):
    def setUp(self):
        self.root = Path(tempfile.mkdtemp(prefix="weekday56-test-"))
        self.pack = self.root / ".asset-cache/port/invented"
        self.pack.mkdir(parents=True)
        self.tables = {
            "weekly_background": list(range(100, 108)),
            "non_summer": list(range(16)),
            "weekly_sd_lbas": list(range(200, 225)),
        }

    def tearDown(self):
        shutil.rmtree(self.root)

    def fixture(self):
        assets = self.pack / "assets"
        assets.mkdir()

        def image(name, width, height):
            raw = _png(width, height, bytes([25, 50, 75, 255]) * width * height, 4)
            (assets / name).write_bytes(raw)
            return {"path": name, "size": len(raw), "sha256": hashlib.sha256(raw).hexdigest()}

        active = image("active.png", 320, 88)
        padded = image("padded.png", 320, 96)
        frame = image("frame.png", 3, 5)
        component = {
            "background": {
                "image": active,
                "stp_mask": active,
                "padded_image": padded,
                "padded_stp_mask": padded,
            },
            "sd": {"frames": [{"bounds": [-2, -1, 3, 5], "png": frame, "stp_png": frame}]},
        }
        doc = {
            "schema": content.SCHEMA,
            "version": 1,
            "components": {"command_5": component, "command_6": copy.deepcopy(component)},
        }
        self.write_manifest(doc)
        return doc

    def write_manifest(self, doc):
        raw = (json.dumps(doc, sort_keys=True, separators=(",", ":")) + "\n").encode()
        (self.pack / "manifest.json").write_bytes(raw)
        return hashlib.sha256(raw).hexdigest()

    def test_selectors_use_distinct_native_offsets(self):
        for command, selector in ((5, 10), (6, 12)):
            route = profile.resolve_selection(self.tables, command)
            self.assertEqual(route["selector_index"], selector)
            self.assertEqual(route["background_lba"], 100 + command)
            self.assertEqual(route["sd_lba"], 200 + selector)
        self.assertNotEqual(profile.resolve_selection(self.tables, 5)["selector_index"], 11)

    def test_unavailable_variants_fail_before_input_or_output(self):
        for values in (
            {"command": 4},
            {"command": True},
            {"month": 6},
            {"extra": 0},
            {"scene": 1},
            {"vacation": 1},
            {"health": 1},
            {"extra": True},
        ):
            request = {"command": 5, **values}
            with self.subTest(request=request), self.assertRaises(ValueError):
                profile.resolve_selection(self.tables, **request)
        with patch.object(content, "open_input", side_effect=AssertionError("input touched")):
            with self.assertRaisesRegex(ValueError, "other seasonal"):
                content.export_pack("missing", str(self.pack / "out"), month=7)
        self.assertFalse((self.pack / "out").exists())

    def test_table_shapes_types_and_index_domain_fail_closed(self):
        for name in self.tables:
            altered = copy.deepcopy(self.tables)
            altered[name] = [1]
            with self.subTest(name=name), self.assertRaises(ValueError):
                profile.resolve_selection(altered, 5)
        for bad in (True, 25, -1):
            altered = copy.deepcopy(self.tables)
            altered["non_summer"][10] = bad
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                profile.resolve_selection(altered, 5)

    def test_inventory_rejects_rebound_source_and_iso_location(self):
        entries = []
        for asset in profile.ASSETS:
            entries.append(
                {
                    "path": asset["path"],
                    "kind": "file",
                    "size": asset["size"],
                    "sha256": asset["sha256"],
                    "location": {
                        "lba": asset["lba"],
                        "extent_bytes": asset["size"],
                        "record_lba": asset["record_lba"],
                        "record_offset": asset["record_offset"],
                        "sectors": list(range(asset["file_sectors"])),
                    },
                }
            )
        profile.validate_inventory(entries)
        for field in ("lba", "record_lba", "record_offset", "extent_bytes"):
            altered = copy.deepcopy(entries)
            altered[0]["location"][field] += 1
            with self.subTest(field=field), self.assertRaises(ValueError):
                profile.validate_inventory(altered)
        with self.assertRaisesRegex(ValueError, "unique"):
            profile.validate_inventory(entries + entries[:1])

    def test_native_caller_pin_survives_rehashed_executable(self):
        exe = b"invented caller bytes"
        span = {
            "name": "invented",
            "exe_offset": 2,
            "size": 5,
            "sha256": hashlib.sha256(exe[2:7]).hexdigest(),
        }
        with patch.object(profile, "EXE_SHA256", hashlib.sha256(exe).hexdigest()), patch.object(
            profile, "TABLES", [span]
        ), patch.object(profile, "NATIVE_CALLERS", []):
            profile.validate_executable(exe)
            changed = exe[:2] + b"X" + exe[3:]
            with patch.object(profile, "EXE_SHA256", hashlib.sha256(changed).hexdigest()):
                with self.assertRaisesRegex(ValueError, "caller bytes"):
                    profile.validate_executable(changed)

    def test_valid_invented_pack_and_coherent_manifest_edit(self):
        doc = self.fixture()
        digest = self.write_manifest(doc)
        with patch.object(content, "MANIFEST_SHA", digest):
            self.assertEqual(content.validate_pack(self.pack), doc)
            changed = copy.deepcopy(doc)
            changed["components"]["command_5"]["sd"]["palette"] = {"mode": 9}
            self.write_manifest(changed)
            with self.assertRaisesRegex(ValueError, "reviewed exact"):
                content.validate_pack(self.pack)

    def test_corrupt_missing_wrong_dimensions_and_symlink_png(self):
        doc = self.fixture()
        digest = self.write_manifest(doc)
        image = self.pack / "assets/frame.png"
        original = image.read_bytes()
        with patch.object(content, "MANIFEST_SHA", digest):
            image.write_bytes(original[:-1] + bytes([original[-1] ^ 1]))
            with self.assertRaisesRegex(ValueError, "PNG bytes differ"):
                content.validate_pack(self.pack)
            image.unlink()
            with self.assertRaisesRegex(ValueError, "PNG missing"):
                content.validate_pack(self.pack)
            other = self.root / "outside.png"
            other.write_bytes(original)
            image.symlink_to(other)
            with self.assertRaisesRegex(ValueError, "PNG missing/linked"):
                content.validate_pack(self.pack)
            image.unlink()
            image.write_bytes(original)
        changed = copy.deepcopy(doc)
        changed["components"]["command_5"]["sd"]["frames"][0]["bounds"][2] = 4
        with patch.object(content, "MANIFEST_SHA", self.write_manifest(changed)):
            with self.assertRaisesRegex(ValueError, "dimensions"):
                content.validate_pack(self.pack)

    def test_project_root_and_symlink_paths_are_rejected(self):
        self.fixture()
        project = self.root / "port/godot"
        project.mkdir(parents=True)
        nested = project / "pack"
        shutil.copytree(self.pack, nested)
        with patch.object(content, "repository_root", return_value=self.root):
            for value in (project, nested):
                with self.subTest(path=value), self.assertRaisesRegex(
                    ValueError, "outside the Godot"
                ):
                    content.validate_pack(value)
            linked = self.root / "linked"
            linked.symlink_to(self.pack, target_is_directory=True)
            with self.assertRaisesRegex(ValueError, "symlink"):
                content.validate_pack(linked)
            with self.assertRaisesRegex(ValueError, "parent-directory"):
                content.validate_pack(str(self.pack / "../invented"))

    def test_failed_build_removes_stage_without_publishing(self):
        out = self.root / ".asset-cache/port/new"

        class Source:
            def __enter__(self):
                return self

            def __exit__(self, *args):
                pass

            def entries(self):
                return []

        with patch.object(content, "repository_root", return_value=self.root), patch.object(
            content, "open_input", return_value=Source()
        ), patch.object(
            content, "_build_manifest", side_effect=ValueError("invented failed proof")
        ):
            with self.assertRaisesRegex(ValueError, "failed proof"):
                content.export_pack("invented", str(out))
        self.assertFalse(out.exists())
        self.assertEqual(list(out.parent.iterdir()), [self.pack])


if __name__ == "__main__":
    unittest.main()
