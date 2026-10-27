"""Synthetic contract tests for installed-runtime bundle assembly."""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

MODULE = Path(__file__).with_name("build_installed_runtime.py")
SPEC = importlib.util.spec_from_file_location("build_installed_runtime", MODULE)
runtime = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(runtime)


class InstalledRuntimeBundleTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="installed-runtime-test-")
        self.root = Path(self.temp.name)
        self.bundle = self.root / "bundle"
        (self.bundle / "packs/rules").mkdir(parents=True)
        (self.bundle / "packs/rules/manifest.json").write_text('{"fixture":true}\n')
        self.exe = self.root / "exe.bin"
        self.exe.write_bytes(b"synthetic executable identity")
        self.track = self.root / "track.bin"
        self.track.write_bytes(b"synthetic track identity")

    def tearDown(self):
        self.temp.cleanup()

    def test_manifest_hashes_files_and_source_inputs_deterministically(self):
        roles = {"rule_tables": "packs/rules/manifest.json"}
        first = runtime._manifest(self.bundle, self.exe, self.track, roles, {"limitations": []})
        second = runtime._manifest(self.bundle, self.exe, self.track, roles, {"limitations": []})
        self.assertEqual(first, second)
        self.assertEqual(first["schema"], "tokimemo.installed-runtime.v1")
        self.assertEqual(first["release_profile"], "original-ja-ps-the-best")
        self.assertEqual(first["files"]["packs/rules/manifest.json"], {
            "size": (self.bundle / "packs/rules/manifest.json").stat().st_size,
            "sha256": hashlib.sha256((self.bundle / "packs/rules/manifest.json").read_bytes()).hexdigest(),
        })
        self.assertEqual(first["source"]["track_sha256"], hashlib.sha256(self.track.read_bytes()).hexdigest())
        self.assertEqual(first["roles"], roles)

    def test_manifest_rejects_unknown_role_and_wrong_role_kind(self):
        for roles in ({"made_up": "packs/rules/manifest.json"},
                      {"font_root": "packs/rules/manifest.json"}):
            with self.subTest(roles=roles), self.assertRaises(runtime.BundleError):
                runtime._manifest(self.bundle, self.exe, None, roles, {})

    def test_manifest_rejects_missing_or_escaping_role(self):
        for target in ("packs/missing/manifest.json", "../outside.json"):
            with self.subTest(target=target), self.assertRaises(runtime.BundleError):
                runtime._manifest(self.bundle, self.exe, None,
                                  {"rule_tables": target}, {})

    def test_manifest_rejects_noncanonical_role_locators(self):
        inside_absolute = str(self.bundle / "packs/rules/manifest.json")
        for target in (inside_absolute, "packs/../packs/rules/manifest.json",
                       "packs//rules/manifest.json", "packs\\rules\\manifest.json"):
            with self.subTest(target=target), self.assertRaises(runtime.BundleError):
                runtime._manifest(self.bundle, self.exe, None,
                                  {"rule_tables": target}, {})

    def test_track_hash_is_bound_only_to_the_input_track(self):
        runtime._check_track_binding(self.track, self.track)
        runtime._check_track_binding(self.root, None)
        with self.assertRaises(runtime.BundleError):
            runtime._check_track_binding(self.root, self.track)
        with self.assertRaises(runtime.BundleError):
            runtime._check_track_binding(self.exe, self.track)

    def test_failed_geko_role_is_removed_before_manifest_inventory(self):
        stage = self.root / "stage"
        partial = stage / "packs/geko_dialogue"
        partial.mkdir(parents=True)
        (partial / "table-0.json").write_text('{"partial":true}\n')
        runtime._discard_partial_role(stage, "packs/geko_dialogue")
        self.assertFalse(partial.exists())
        document = runtime._manifest(stage, self.exe, None, {}, {})
        self.assertEqual(document["files"], {})

    def test_symlinked_generated_content_is_rejected(self):
        target = self.root / "outside.bin"
        target.write_bytes(b"outside")
        (self.bundle / "linked.bin").symlink_to(target)
        with self.assertRaises(runtime.BundleError):
            runtime._manifest(self.bundle, self.exe, None,
                              {"rule_tables": "packs/rules/manifest.json"}, {})

    def test_manifest_json_is_stable_and_does_not_claim_capabilities(self):
        document = runtime._manifest(self.bundle, self.exe, None,
                                     {"rule_tables": "packs/rules/manifest.json"},
                                     {"limitations": ["synthetic fixture"]})
        encoded = json.dumps(document, sort_keys=True, ensure_ascii=False, indent=2) + "\n"
        self.assertEqual(encoded, json.dumps(document, sort_keys=True, ensure_ascii=False, indent=2) + "\n")
        self.assertNotIn("capabilities", document)

    def test_geko_role_is_directory_and_keeps_section_index_hashable(self):
        geko = self.bundle / "packs/geko_dialogue"
        section = geko / "table-0"
        section.mkdir(parents=True)
        (section / "content.json").write_text('{"fixture":true}\n')
        (geko / "index.json").write_text(
            '{"section_count":12,"sections":[{"id":"table-0","path":"table-0/content.json"}]}\n'
        )
        document = runtime._manifest(self.bundle, self.exe, None,
                                     {"geko_dialogue": "packs/geko_dialogue"}, {})
        self.assertIn("packs/geko_dialogue/index.json", document["files"])
        self.assertIn("packs/geko_dialogue/table-0/content.json", document["files"])
        self.assertEqual(runtime.ROLE_KINDS["geko_dialogue"], "dir")

    def test_font_role_uses_native_loader_relative_root(self):
        font = self.bundle / ".asset-cache/port/fonts/native-font-v2"
        font.mkdir(parents=True)
        (font / "font.json").write_text('{"schema":"tokimemo.native-font.v1"}\n')
        (font / "glyph_bank.bin").write_bytes(b"synthetic")
        document = runtime._manifest(
            self.bundle, self.exe, None,
            {"font_root": ".asset-cache/port/fonts/native-font-v2"}, {})
        self.assertEqual(document["roles"]["font_root"], ".asset-cache/port/fonts/native-font-v2")


if __name__ == "__main__":
    unittest.main()
