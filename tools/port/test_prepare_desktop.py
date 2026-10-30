"""Invented source fixtures for code-only desktop staging and output isolation."""
import json
from pathlib import Path
import tempfile
import unittest

from prepare_desktop import prepare


class DesktopStagingTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name) / "repo"
        self.project = self.root / "port/godot"
        (self.project / "app").mkdir(parents=True)
        (self.project / "project.godot").write_text('run/main_scene="res://shell.tscn"\n')
        (self.project / "app/playable.tscn").write_text('[gd_scene format=3]\n')
        (self.root / "tools/port").mkdir(parents=True)
        (self.root / "tools/port/desktop_export.cfg").write_text('invented preset\n')
        (self.root / "tools/port/godot.lock.json").write_text(json.dumps({"test": True}))
        self.destination = self.root / ".asset-cache/port/build/project"

    def test_only_staged_main_scene_changes(self):
        before = (self.project / "project.godot").read_bytes()
        (self.project / "tests").mkdir()
        (self.project / "tests/private.bin").write_bytes(b"not exported")
        receipt = prepare(self.root, self.destination, "week")
        self.assertEqual((self.project / "project.godot").read_bytes(), before)
        self.assertIn("res://app/playable.tscn", (self.destination / "project.godot").read_text())
        self.assertIn("textures/vram_compression/import_s3tc_bptc=true", (self.destination / "project.godot").read_text())
        self.assertFalse((self.destination / "tests").exists())
        self.assertEqual(set(receipt["source_files"]), {"project.godot", "app/playable.tscn"})

    def test_assets_are_rejected_before_creation(self):
        (self.project / "art.png").write_bytes(b"invented image")
        with self.assertRaisesRegex(ValueError, "non-source"):
            prepare(self.root, self.destination, "week")
        self.assertFalse(self.destination.exists())

    def test_imported_script_identifiers_are_retained(self):
        (self.project / "app/example.gd").write_text("extends Node\n")
        (self.project / "app/example.gd.uid").write_text("uid://b123abc\n")
        receipt = prepare(self.root, self.destination, "week")
        self.assertIn("app/example.gd.uid", receipt["source_files"])

    def test_orphan_uid_is_rejected(self):
        (self.project / "app/missing.gd.uid").write_text("uid://b123abc\n")
        with self.assertRaisesRegex(ValueError, "non-source"):
            prepare(self.root, self.destination, "week")

    def test_symlinked_cache_cannot_write_outside(self):
        outside = Path(self.temporary.name) / "outside"
        outside.mkdir()
        (self.root / ".asset-cache").mkdir()
        (self.root / ".asset-cache/port").symlink_to(outside, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, "symlinks"):
            prepare(self.root, self.destination, "week")
        self.assertEqual(list(outside.iterdir()), [])

    def test_existing_build_and_traversal_rejected(self):
        self.destination.parent.mkdir(parents=True)
        marker = self.destination.parent / "source-receipt.json"
        marker.write_text("keep")
        with self.assertRaisesRegex(ValueError, "new"):
            prepare(self.root, self.destination, "week")
        self.assertEqual(marker.read_text(), "keep")
        with self.assertRaisesRegex(ValueError, "traversal"):
            prepare(self.root, self.root / ".asset-cache/port/../escape/project", "week")

    def test_project_symlink_and_missing_scene_rejected(self):
        (self.project / "linked.gd").symlink_to(self.project / "project.godot")
        with self.assertRaisesRegex(ValueError, "symlink"):
            prepare(self.root, self.destination, "week")
        (self.project / "linked.gd").unlink()
        with self.assertRaisesRegex(ValueError, "scene is missing"):
            prepare(self.root, self.destination, "new-game")

    def test_symlinked_project_root_is_not_a_source_tree(self):
        outside = Path(self.temporary.name) / "outside-project"
        self.project.rename(outside)
        self.project.symlink_to(outside, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, "source ancestry"):
            prepare(self.root, self.destination, "week")
        self.assertFalse(self.destination.exists())


if __name__ == "__main__":
    unittest.main()
