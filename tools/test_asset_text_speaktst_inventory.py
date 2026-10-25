"""Security and canonical persistence tests using an invented tiny bank."""
import copy
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import struct
import tempfile
import unittest
from unittest.mock import patch

from tools.assets import text_speaktst_inventory as inventory
from tools.assets.text import TextFormatError
from tools.assets.text_speaktst import parse_speaktst_layout, validate_speaktst_layout

BASE = 0x80180000
REL = "CDROM/SPEAKTST/FIXTURE.BIN"


def fixture():
    data = bytearray(256)
    for offset, target in ((0, 32), (4, 48), (8, 64), (32, 96), (36, 100),
                           (64, 128), (68, 152)):
        struct.pack_into("<I", data, offset, BASE + target)
    data[48:51] = bytes((1, 1, 255))
    data[96:99] = "テ".encode("shift_jis") + b"\0"
    data[100:102] = b"\xff\0"
    data[128:176] = bytes(range(48))
    data[200:202] = b"\x12\x34"
    data[-4:-2] = (sum(data[:-4]) & 65535).to_bytes(2, "big")
    raw = bytes(data)
    profile = {"path": REL, "sha256": hashlib.sha256(raw).hexdigest(),
               "size": len(raw), "text_records": 2, "metadata_array_offset": 200,
               "profile": {"id": "fixture", "sha256": hashlib.sha256(raw).hexdigest(),
                           "load_address": BASE, "sections": [{"id": "fixture",
                           "text_table": BASE, "command_table": BASE + 4,
                           "voice_table": BASE + 8, "group_count": 1}]}}
    return raw, profile


class InventoryTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.input_root = self.root / "inputs"
        self.repo = self.root / "repo"
        source = self.input_root / REL
        source.parent.mkdir(parents=True)
        self.repo.mkdir()
        self.data, self.profile = fixture()
        source.write_bytes(self.data)
        self.source = source
        self.bank = parse_speaktst_layout(self.data, REL, self.profile)

    def patched(self):
        return [patch.object(inventory, "SPEAKTST_PROFILES", {REL: self.profile}),
                patch.object(inventory, "PROFILE_COUNT", 1),
                patch.object(inventory, "TOTAL_BYTES", len(self.data)),
                patch.object(inventory, "TOTAL_RECORDS", len(self.bank["records"])),
                patch.object(inventory, "TOTAL_CONTROLS", len(self.bank["controls"])),
                patch.object(inventory, "UNKNOWN_TOKENS", 1),
                patch.object(inventory, "parse_speaktst_layout",
                             lambda data, path: parse_speaktst_layout(data, path, self.profile)),
                patch.object(inventory, "validate_speaktst_layout",
                             lambda data, path, bank: validate_speaktst_layout(data, path, bank,
                                                                                self.profile))]

    def run_patched(self, callback):
        managers = self.patched()
        for manager in managers:
            manager.start()
        try:
            return callback()
        finally:
            for manager in reversed(managers):
                manager.stop()

    def test_export_is_idempotent_and_validation_reconstructs(self):
        def run():
            first = inventory.export_inventory(self.input_root, self.repo)
            second = inventory.export_inventory(self.input_root, self.repo)
            checked = inventory.validate_inventory(self.input_root, self.repo)
            self.assertEqual(first["text_records"], 2)
            self.assertEqual(second["status"], first["status"])
            self.assertTrue(checked["all_fields_canonical"])
            self.assertFalse(checked["semantic_coverage_complete"])
            folder = self.repo / inventory.OUTPUT_RELATIVE
            self.assertEqual({p.name for p in folder.iterdir()},
                             {"manifest.json", "FIXTURE.BIN.json", "validation.json"})
            bank = json.loads((folder / "FIXTURE.BIN.json").read_text())
            self.assertEqual(bank["status"], "layout_only_inventory")
            self.assertTrue(all(row["target_text"] is None for row in bank["records"]))
            self.assertTrue(all(not row["readiness"]["translation_ready"] for row in bank["records"]))
        self.run_patched(run)

    def test_persisted_field_tampering_fails_even_with_rehashed_manifest(self):
        def run():
            inventory.export_inventory(self.input_root, self.repo)
            folder = self.repo / inventory.OUTPUT_RELATIVE
            path = folder / "FIXTURE.BIN.json"
            bank = json.loads(path.read_bytes())
            bank["records"][0]["readiness"]["translation_ready"] = True
            path.write_bytes(inventory._json_bytes(bank))
            manifest_path = folder / "manifest.json"
            manifest = json.loads(manifest_path.read_bytes())
            manifest["text_records"] = 2
            manifest_path.write_bytes(inventory._json_bytes(manifest))
            with self.assertRaises(TextFormatError):
                inventory.validate_inventory(self.input_root, self.repo)
        self.run_patched(run)

    def test_conflict_and_extra_files_reject_without_overwrite(self):
        def run():
            inventory.export_inventory(self.input_root, self.repo)
            folder = self.repo / inventory.OUTPUT_RELATIVE
            path = folder / "manifest.json"
            old = path.read_bytes()
            path.write_bytes(old + b" ")
            with self.assertRaises(TextFormatError):
                inventory.export_inventory(self.input_root, self.repo)
            self.assertEqual(path.read_bytes(), old + b" ")
            path.write_bytes(old)
            (folder / "unexpected").write_bytes(b"")
            with self.assertRaises(TextFormatError):
                inventory.validate_inventory(self.input_root, self.repo)
        self.run_patched(run)

    def test_validation_receipt_conflict_rejects_export(self):
        def run():
            inventory.export_inventory(self.input_root, self.repo)
            inventory.validate_inventory(self.input_root, self.repo)
            receipt = self.repo / inventory.OUTPUT_RELATIVE / "validation.json"
            valid = receipt.read_bytes()
            receipt.write_bytes(b"{}\n")
            with self.assertRaises(TextFormatError):
                inventory.export_inventory(self.input_root, self.repo)
            self.assertEqual(receipt.read_bytes(), b"{}\n")
            receipt.write_bytes(valid)
        self.run_patched(run)

    def test_output_symlink_hardlink_and_noncanonical_path_reject(self):
        folder = self.repo / inventory.OUTPUT_RELATIVE
        folder.parent.mkdir(parents=True)
        target = self.root / "outside"
        target.mkdir()
        folder.symlink_to(target, target_is_directory=True)
        with self.assertRaises(TextFormatError):
            inventory.export_inventory(self.input_root, self.repo)
        folder.unlink()

        def run_hardlink():
            inventory.export_inventory(self.input_root, self.repo)
            target_file = folder / "manifest.json"
            alias = folder / "alias"
            os.link(target_file, alias)
            with self.assertRaises(TextFormatError):
                inventory.validate_inventory(self.input_root, self.repo)
        self.run_patched(run_hardlink)
        with self.assertRaises(TextFormatError):
            inventory.export_inventory(self.input_root, self.repo, self.repo / "elsewhere")

    def test_custom_output_is_allowed_only_inside_asset_cache_text(self):
        custom = self.repo / ".asset-cache/text/fresh-run/nested"
        self.run_patched(lambda: inventory.export_inventory(self.input_root, self.repo, custom))
        self.assertTrue((custom / "manifest.json").is_file())
        outside = self.repo / ".asset-cache/elsewhere"
        with self.assertRaises(TextFormatError):
            inventory.export_inventory(self.input_root, self.repo, outside)
        traversal = self.repo / ".asset-cache/text/../elsewhere"
        with self.assertRaises(TextFormatError):
            inventory.export_inventory(self.input_root, self.repo, traversal)

    def test_profile_path_traversal_and_duplicate_basename_reject(self):
        with self.assertRaises(TextFormatError):
            inventory._safe_relative("../escape")
        with patch.object(inventory, "SPEAKTST_PROFILES", {"../FIXTURE.BIN": self.profile}):
            with self.assertRaises(TextFormatError):
                inventory._profiles()

    def test_fail_closed_path_checks_remain_active_under_optimized_python(self):
        code = ("from tools.assets.text_speaktst_inventory import _safe_relative; "
                "from tools.assets.text import TextFormatError; "
                "exec('try: _safe_relative(\\\"../escape\\\")\\n'"
                "+'except TextFormatError: pass\\n'"
                "+'else: raise SystemExit(7)')")
        result = subprocess.run([sys.executable, "-O", "-c", code], cwd=Path(__file__).resolve().parents[1],
                                capture_output=True, text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
