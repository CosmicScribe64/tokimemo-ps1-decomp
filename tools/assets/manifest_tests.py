"""Invented-data tests for safe publishing and media CLI integration (Docker)."""
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

from assets import manifest
from assets.input import open_input
from export_assets import export_source


def tim():
    # One 16-bit red texel with its STP bit set; invented, not game data.
    return struct.pack("<III4HH", 0x10, 2, 14, 0, 0, 1, 1, 0x801f)


class ManifestTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.root = Path(self.temporary.name).resolve()
        self.cache = self.root / ".asset-cache"
        self.repository_patch = patch.object(manifest, "REPOSITORY", self.root)
        self.cache_patch = patch.object(manifest, "CACHE", self.cache)
        self.repository_patch.start()
        self.cache_patch.start()

    def tearDown(self):
        self.cache_patch.stop()
        self.repository_patch.stop()
        self.temporary.cleanup()

    def test_outputs_are_confined_and_never_replace_different_bytes(self):
        with self.assertRaises(ValueError):
            manifest.write_bytes(self.root / "original.bin", b"no")
        path = self.cache / "media" / "same.bin"
        manifest.write_bytes(path, b"invented")
        manifest.write_bytes(path, b"invented")
        with self.assertRaises(ValueError):
            manifest.write_bytes(path, b"changed")
        self.assertEqual(path.read_bytes(), b"invented")
        (self.cache / "escape").symlink_to(self.root, target_is_directory=True)
        with self.assertRaises(ValueError):
            manifest.write_bytes(self.cache / "escape" / "outside.bin", b"no")

    def test_cli_publishes_deterministically_with_unsupported_coverage(self):
        source = self.root / "source"
        source.mkdir()
        (source / "test.TIM").write_bytes(tim())
        (source / "unknown.NBN").write_bytes(b"invented unknown container")
        output = self.cache / "media" / "run"
        with open_input(source) as opened:
            result = export_source(opened, output)
        before = (output / "manifest.json").read_bytes()
        self.assertEqual(result["coverage"]["status"], {"exported": 1, "unsupported": 1})
        record = next(r for r in result["assets"] if r["status"] == "exported")
        self.assertEqual(len(record["outputs"]), 2)
        original = self.root / record["original"]["path"]
        self.assertEqual(original.read_bytes(), tim())
        with open_input(source) as opened:
            export_source(opened, output)
        self.assertEqual((output / "manifest.json").read_bytes(), before)
        self.assertFalse(json.loads(before)["original_reconstruction_proven"])

    def test_filters_keep_complete_inventory_and_invalid_files_are_recorded(self):
        source = self.root / "source"
        source.mkdir()
        (source / "test.TIM").write_bytes(tim())
        (source / "bad.TIM").write_bytes(b"\x10\0\0\0")
        with open_input(source) as opened:
            result = export_source(opened, self.cache / "media" / "filtered", include=["other.*"])
        self.assertEqual(result["coverage"]["status"], {"invalid": 1, "supported": 1})
        self.assertFalse(list((self.cache / "media" / "filtered").rglob("*.png")))

    def test_vab_pair_is_published_at_explicit_audition_rate(self):
        source = self.root / "source"
        source.mkdir()
        header = bytearray(0xc20)
        header[:4] = b"pBAV"
        struct.pack_into("<III", header, 4, 7, 0, len(header) + 16)
        struct.pack_into("<HHH", header, 0x12, 1, 1, 1)
        struct.pack_into("<H", header, 0xa22, 2)  # reserved size[0], one 16-byte sample[1]
        body = b"\x0c\x01" + b"\x11" * 14
        (source / "test.VH").write_bytes(header)
        (source / "test.VB").write_bytes(body)
        with open_input(source) as opened:
            result = export_source(opened, self.cache / "media" / "audio", sample_rate=22050)
        record = next(r for r in result["assets"] if r["source"]["path"] == "test.VH")
        self.assertEqual(record["status"], "exported")
        self.assertTrue(any(o["path"].endswith(".wav") for o in record["outputs"]))
        self.assertEqual((self.root / record["paired_original"]["path"]).read_bytes(), body)
        with open_input(source) as opened:
            deferred = export_source(opened, self.cache / "media" / "no-rate")
        self.assertEqual(next(r["status"] for r in deferred["assets"]
                              if r["source"]["path"] == "test.VH"), "deferred")

    def test_main_ui_profile_requires_and_preserves_paired_executable(self):
        from tools.test_asset_image_packs import fixture
        pack, executable = fixture()
        source = self.root / "source"
        pack_path = source / "CDROM" / "FUKUTEST" / "MAINROM.NBN"
        pack_path.parent.mkdir(parents=True)
        pack_path.write_bytes(pack)
        with open_input(source) as opened:
            result = export_source(opened, self.cache / "media" / "missing-exe")
        self.assertEqual(result["assets"][0]["status"], "deferred")
        (source / "SLPM_86.053").write_bytes(executable)
        with open_input(source) as opened:
            result = export_source(opened, self.cache / "media" / "main-ui")
        record = next(r for r in result["assets"] if r["format"] == "tokimemo-main-ui")
        self.assertEqual(record["status"], "exported")
        self.assertEqual(record["paired_source"]["sha256"], manifest.digest(executable))
        self.assertEqual((self.root / record["paired_original"]["path"]).read_bytes(), executable)
        self.assertEqual(record["export"]["images"][0]["frame_index"], 0)
        self.assertTrue(record["export"]["metadata"]["unmapped_sheet_extents"])


if __name__ == "__main__":
    unittest.main()
