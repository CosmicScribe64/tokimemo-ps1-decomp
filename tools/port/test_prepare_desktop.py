"""Invented source fixtures for desktop staging, native pins and output isolation."""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

MODULE = Path(__file__).with_name("prepare_desktop.py")
SPEC = importlib.util.spec_from_file_location("prepare_desktop", MODULE)
prepare_desktop = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(prepare_desktop)
prepare = prepare_desktop.prepare


class StagingFixture(unittest.TestCase):
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


class DesktopStagingTests(StagingFixture):
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

    def test_scene_nodes_with_stored_ids_are_staged(self):
        (self.project / "app/playable.tscn").write_text(
            '[gd_scene format=3]\n\n[node name="Root" type="Node2D" unique_id=1799730646]\n'
            '\n[node name="Child" type="Node" parent="." unique_id=42]\n')
        receipt = prepare(self.root, self.destination, "week")
        self.assertIn("app/playable.tscn", receipt["source_files"])

    def test_scene_node_without_stored_id_is_rejected_before_creation(self):
        (self.project / "app/playable.tscn").write_text(
            '[gd_scene format=3]\n\n[node name="Root" type="Node2D" unique_id=1]\n'
            '\n[node name="Child" type="Node" parent="."]\n')
        with self.assertRaisesRegex(ValueError, "no stored unique_id.*app/playable.tscn"):
            prepare(self.root, self.destination, "week")
        self.assertFalse(self.destination.exists())

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


class NativeStagingTests(StagingFixture):
    """Linux arm64 staging ships only the library/descriptor pinned by the bridge."""

    LIBRARY = b"invented library bytes"

    def setUp(self):
        super().setUp()
        (self.project / "shell.tscn").write_text("[gd_scene format=3]\n")
        (self.project / "bin").mkdir()
        (self.project / "services").mkdir()
        (self.project / "bin/liborig.so").write_bytes(self.LIBRARY)
        self.write_descriptor('linux.release.arm64 = "res://bin/liborig.so"\n'
                              'linux.debug.arm64 = "res://bin/liborig.so"\n')
        (self.project / "original_audio.gdextension.uid").write_text("uid://hpij\n")

    def write_descriptor(self, libraries, library_sha=None):
        descriptor = '[configuration]\nentry_symbol = "x"\n[libraries]\n' + libraries
        (self.project / "original_audio.gdextension").write_text(descriptor)
        (self.project / "services/first_arrival_bgm_bridge.gd").write_text(
            'extends RefCounted\n'
            'const LIBRARY := "res://bin/liborig.so"\n'
            + f'const LIBRARY_SHA := "{library_sha or hashlib.sha256(self.LIBRARY).hexdigest()}"\n'
            + 'const DESCRIPTOR := "res://original_audio.gdextension"\n'
            + f'const DESCRIPTOR_SHA := "{hashlib.sha256(descriptor.encode()).hexdigest()}"\n')

    def test_linux_stages_pinned_native_files_and_production_shell(self):
        receipt = prepare(self.root, self.destination, "player", "linux-arm64")
        self.assertEqual(receipt["export_preset"], "Linux arm64")
        self.assertEqual(set(receipt["native_files"]),
                         {"bin/liborig.so", "original_audio.gdextension", "original_audio.gdextension.uid"})
        self.assertEqual((self.destination / "bin/liborig.so").read_bytes(), self.LIBRARY)
        self.assertIn('run/main_scene="res://shell.tscn"', (self.destination / "project.godot").read_text())
        self.assertFalse(receipt["original_game_data_included"])

    def test_macos_is_code_only(self):
        receipt = prepare(self.root, self.destination, "player", "macos")
        self.assertEqual(receipt["native_files"], {})
        self.assertFalse((self.destination / "bin").exists())
        self.assertFalse((self.destination / "original_audio.gdextension").exists())

    def test_unpinned_library_rejected_before_creation(self):
        self.write_descriptor('linux.release.arm64 = "res://bin/liborig.so"\n'
                              'linux.debug.arm64 = "res://bin/liborig.so"\n', library_sha="0" * 64)
        with self.assertRaisesRegex(ValueError, "library differs"):
            prepare(self.root, self.destination, "player", "linux-arm64")
        self.assertFalse(self.destination.exists())

    def test_descriptor_with_other_platform_rejected(self):
        self.write_descriptor('linux.release.arm64 = "res://bin/liborig.so"\n'
                              'linux.debug.arm64 = "res://bin/liborig.so"\n'
                              'macos.release = "res://bin/liborig.dylib"\n')
        with self.assertRaisesRegex(ValueError, "only the reviewed Linux arm64"):
            prepare(self.root, self.destination, "player", "linux-arm64")

    def test_other_shared_library_rejected(self):
        (self.project / "bin/unreviewed.so").write_bytes(b"other")
        with self.assertRaisesRegex(ValueError, "non-source"):
            prepare(self.root, self.destination, "player", "linux-arm64")

    def test_missing_bridge_pin_rejected(self):
        (self.project / "services/first_arrival_bgm_bridge.gd").write_text("extends RefCounted\n")
        with self.assertRaisesRegex(ValueError, "pin LIBRARY"):
            prepare(self.root, self.destination, "player", "linux-arm64")

    def test_unknown_target_rejected(self):
        with self.assertRaisesRegex(ValueError, "unsupported desktop target"):
            prepare(self.root, self.destination, "player", "linux-x86_64")


if __name__ == "__main__":
    unittest.main()
