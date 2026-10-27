"""Fresh-source and path protection checks; run in Docker."""
import hashlib
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from tools.port.first_arrival_bgm_content import build
from tools.port.original_first_arrival_sources import SOURCES, export_bytes, manifest_bytes


class SourceExportTests(unittest.TestCase):
    def setUp(self):
        self.work = tempfile.TemporaryDirectory()
        self.root = Path(self.work.name)
        self.original = self.root / "original"
        self.original.mkdir()

    def tearDown(self):
        self.work.cleanup()

    def test_input_output_overlap_is_rejected_before_write(self):
        before = tuple(self.original.rglob("*"))
        with self.assertRaises(ValueError):
            build(self.original, self.original / "audio" / "first-arrival")
        self.assertEqual(before, tuple(self.original.rglob("*")))

    def test_linked_output_ancestor_is_rejected(self):
        (self.root / "link").symlink_to(self.original, target_is_directory=True)
        with self.assertRaises(ValueError):
            build(self.original, self.root / "link" / "audio" / "first-arrival")
        self.assertEqual([], list(self.original.iterdir()))

    def test_traversal_is_rejected(self):
        with self.assertRaises(ValueError):
            build(self.original, self.root / "gone" / ".." / "audio" / "first-arrival")
        self.assertFalse((self.root / "gone").exists())

    def test_bad_original_bytes_create_no_output(self):
        target = self.root / "audio" / "first-arrival"
        with self.assertRaises(ValueError):
            export_bytes({row[0]: b"" for row in SOURCES}, target)
        self.assertFalse(target.parent.exists())

    def test_incomplete_identity_set_creates_no_output(self):
        target = self.root / "audio" / "first-arrival"
        with self.assertRaises(ValueError):
            export_bytes({}, target)
        self.assertFalse(target.parent.exists())

    def test_wrong_role_path_is_rejected(self):
        with self.assertRaises(ValueError):
            build(self.original, self.root / "other")
        self.assertFalse((self.root / "other").exists())

    def test_runtime_resource_output_is_rejected_before_read_or_write(self):
        repository = self.root / "repository"
        repository.mkdir()
        target = repository / "port" / "godot" / "audio" / "first-arrival"
        with patch("tools.port.first_arrival_bgm_content.ROOT", repository):
            with patch("tools.port.first_arrival_bgm_content.open_input") as reader:
                with self.assertRaises(ValueError):
                    build(self.original, target)
                reader.assert_not_called()
        self.assertEqual([], list(repository.iterdir()))

    def test_actual_fresh_export_matches_all_six_original_pins(self):
        original = os.environ.get("ORIGINAL_BGM_INPUT")
        if not original:
            self.skipTest("explicit read-only original fixture is required")
        target = self.root / "audio" / "first-arrival"
        result = build(Path(original), target)
        self.assertEqual(result["manifest_sha256"], hashlib.sha256(manifest_bytes()).hexdigest())
        self.assertFalse(result["BGM_PCM_exported"])
        for _, name, _, size, digest in SOURCES:
            data = (target / name).read_bytes()
            self.assertEqual((len(data), hashlib.sha256(data).hexdigest()), (size, digest))
        before = {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in target.iterdir()}
        with self.assertRaises(ValueError):
            build(Path(original), target)
        self.assertEqual(before, {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in target.iterdir()})


if __name__ == "__main__":
    unittest.main()
