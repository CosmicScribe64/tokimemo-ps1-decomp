from __future__ import annotations

import hashlib
import json
import tempfile
import unittest
from pathlib import Path

from font_content import font_content, write_external_font_content


def synthetic_source() -> tuple[bytes, dict]:
    profile = {
        "path": "synthetic-font.bin",
        "load_address": 0x1000,
        "header_size": 0,
        "range_address": 0x1000,
        "range_count": 3,
        "glyph_address": 0x1020,
        "glyph_count": 2,
        "evidence": ["synthetic test only"],
    }
    ranges = b"\x40\x81\x00\x00\x42\x81\x01\x00\x44\x81\x00\x00"
    glyphs = bytes(range(36))
    return ranges + bytes(20) + glyphs, profile


class FontContentTests(unittest.TestCase):
    def test_reuses_canonical_reader_and_preserves_exact_extents(self):
        source, profile = synthetic_source()
        package = font_content(source, profile)
        self.assertEqual(package["metadata"]["range_raw_hex"], source[:12].hex())
        self.assertEqual(package["glyph_bank"], source[32:68])
        self.assertEqual(package["payload_sha256"], hashlib.sha256(source[32:68]).hexdigest())
        self.assertEqual(package["metadata"]["glyph_stride"], 18)

    def test_cache_payload_is_immutable_external_and_hashed(self):
        source, profile = synthetic_source()
        with tempfile.TemporaryDirectory() as directory:
            cache = Path(directory) / ".asset-cache" / "port"
            cache.mkdir(parents=True)
            result = write_external_font_content(source, cache, profile, repository=directory)
            self.assertEqual(result["payload_path"], "fonts/native-font-v2/glyph_bank.bin")
            self.assertEqual(result["descriptor_path"], "fonts/native-font-v2/font.json")
            self.assertEqual((cache / result["payload_path"]).read_bytes(), source[32:68])
            descriptor = json.loads((cache / result["descriptor_path"]).read_text())
            self.assertEqual(descriptor["schema"], "tokimemo.native-font.v1")
            self.assertEqual(descriptor["payload_sha256"], result["payload_sha256"])
            with self.assertRaises(FileExistsError):
                write_external_font_content(source, cache, profile, repository=directory)

    def test_rejects_nested_external_and_traversing_cache_before_writes(self):
        source, profile = synthetic_source()
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            repo = root / "repo"
            repo.mkdir()
            for cache in (repo / "tracked" / ".asset-cache" / "port",
                          root / "outside" / ".asset-cache" / "port",
                          repo / "child" / ".." / ".asset-cache" / "port"):
                with self.subTest(cache=cache), self.assertRaises(ValueError):
                    write_external_font_content(source, cache, profile, repository=repo)
            self.assertEqual(list(repo.iterdir()), [])
            self.assertFalse((root / "outside").exists())

    def test_rejects_symlinked_cache_before_writes(self):
        source, profile = synthetic_source()
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            target = root / "elsewhere"
            target.mkdir()
            (root / ".asset-cache").symlink_to(target, target_is_directory=True)
            with self.assertRaises(ValueError):
                write_external_font_content(source, root / ".asset-cache" / "port",
                                            profile, repository=root)
            self.assertEqual(list(target.iterdir()), [])

    def test_rejects_source_that_cannot_contain_declared_bank(self):
        source, profile = synthetic_source()
        with self.assertRaisesRegex(Exception, "glyph bank outside source"):
            font_content(source[:40], profile)


if __name__ == "__main__":
    unittest.main()
