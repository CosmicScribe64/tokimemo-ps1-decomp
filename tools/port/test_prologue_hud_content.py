"""Synthetic path and identity checks for the bounded HUD pack builder."""
from __future__ import annotations

import hashlib
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from tools.port.prologue_hud_content import (
    FONT_DESCRIPTOR_SHA256,
    HudPackError,
    _safe_rel,
    _verify_source_pins,
    _validate_output_role,
    build_from_input,
)


class PrologueHudBuilderTests(unittest.TestCase):
    @staticmethod
    def _tree_state(root: Path) -> tuple:
        state = []
        for path in sorted(root.rglob("*")):
            relative = path.relative_to(root).as_posix()
            if path.is_symlink():
                state.append((relative, "symlink", os.readlink(path)))
            elif path.is_dir():
                state.append((relative, "directory"))
            else:
                state.append((relative, "file", hashlib.sha256(path.read_bytes()).hexdigest()))
        return tuple(state)

    def test_portable_path_grammar(self):
        for value in ("assets/a.png", "packs/prologue_hud/manifest.json"):
            self.assertTrue(_safe_rel(value))
        for value in ("", "/outside/file", "../outside", "packs/../escape",
                      "packs\\escape", "C:/outside", "assets//file", "./file"):
            self.assertFalse(_safe_rel(value), value)

    def test_source_pin_rejects_missing_repository_evidence(self):
        with tempfile.TemporaryDirectory() as temp:
            with self.assertRaises(HudPackError):
                _verify_source_pins(Path(temp))

    def test_builder_rejects_foreign_role_leaf_before_export(self):
        with tempfile.TemporaryDirectory() as temp:
            bad_output = Path(temp) / "bundle" / "packs" / "wrong_role"
            bad_output.parent.mkdir(parents=True)
            with self.assertRaisesRegex(HudPackError, "final bundle packs/prologue_hud"):
                _validate_output_role(bad_output)
            self.assertFalse(bad_output.exists())

    def test_descriptor_pin_is_fixed_profile_metadata(self):
        self.assertEqual(len(FONT_DESCRIPTOR_SHA256), 64)
        self.assertTrue(all(char in "0123456789abcdef" for char in FONT_DESCRIPTOR_SHA256))

    def test_no_partial_manifest_after_rejected_input(self):
        # Mock only source/font proof gates so the real bounded input reader
        # rejects this invented file before any role is published.
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "input.bin"
            source.write_bytes(b"synthetic invalid input")
            bundle = root / "bundle"
            output = bundle / "packs" / "prologue_hud"
            (bundle / "packs").mkdir(parents=True)
            font_resource = {"directory": ".asset-cache/port/fonts/native-font-v2",
                             "manifest": "font.json", "manifest_sha256": "0" * 64,
                             "schema": "tokimemo.native-font.v1"}
            font_payload = {"path": ".asset-cache/port/fonts/native-font-v2/glyph_bank.bin",
                            "sha256": "0" * 64, "size": 3418 * 18}
            source_root = Path(os.environ["TOKIMEMO_SOURCE_ROOT"])
            with patch("tools.port.prologue_hud_content._verify_source_pins", return_value=[]), \
                    patch("tools.port.prologue_hud_content._font_identity",
                          return_value=(font_resource, font_payload)):
                with self.assertRaisesRegex(HudPackError, "original input rejected"):
                    build_from_input(source, source_root, output)
            self.assertFalse(output.exists())
            self.assertEqual(list((bundle / "packs").glob(".prologue-hud-stage-*")), [])

    def test_output_overlap_rejects_before_creating_any_input_directories(self):
        source_root = Path(os.environ["TOKIMEMO_SOURCE_ROOT"])
        with tempfile.TemporaryDirectory() as temp:
            original = Path(temp) / "original"
            original.mkdir()
            (original / "marker.bin").write_bytes(b"synthetic input marker")
            output = original / "new-bundle" / "packs" / "prologue_hud"
            before = self._tree_state(original)
            with self.assertRaisesRegex(HudPackError, "overlaps the original input"):
                build_from_input(original, source_root, output)
            self.assertEqual(self._tree_state(original), before)

    def test_symlinked_output_ancestor_is_rejected_before_writing(self):
        source_root = Path(os.environ["TOKIMEMO_SOURCE_ROOT"])
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "input.bin"
            source.write_bytes(b"synthetic input")
            real = root / "real-output"
            real.mkdir()
            alias = root / "output-alias"
            alias.symlink_to(real, target_is_directory=True)
            output = alias / "bundle" / "packs" / "prologue_hud"
            before = self._tree_state(root)
            with self.assertRaisesRegex(HudPackError, "symlink component"):
                build_from_input(source, source_root, output)
            self.assertEqual(self._tree_state(root), before)

    def test_symlinked_input_ancestor_is_rejected_before_writing(self):
        source_root = Path(os.environ["TOKIMEMO_SOURCE_ROOT"])
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            real = root / "real-input"
            real.mkdir()
            (real / "input.bin").write_bytes(b"synthetic input")
            alias = root / "input-alias"
            alias.symlink_to(real, target_is_directory=True)
            output = root / "bundle" / "packs" / "prologue_hud"
            before = self._tree_state(root)
            with self.assertRaisesRegex(HudPackError, "symlink component"):
                build_from_input(alias / "input.bin", source_root, output)
            self.assertEqual(self._tree_state(root), before)

    def test_repository_protected_write_roots_are_rejected(self):
        source_root = Path(os.environ["TOKIMEMO_SOURCE_ROOT"])
        with tempfile.TemporaryDirectory() as temp:
            source = Path(temp) / "input.bin"
            source.write_bytes(b"synthetic input")
            for protected_name in (".git", "disc", "raw"):
                with self.subTest(protected=protected_name):
                    output = (source_root / protected_name / "hud-test-bundle"
                              / "packs" / "prologue_hud")
                    with self.assertRaisesRegex(HudPackError, "protected repository data"):
                        build_from_input(source, source_root, output)
                    self.assertFalse(output.parent.exists())


if __name__ == "__main__":
    unittest.main()
