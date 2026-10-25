"""Trust-boundary regressions for the native bank provenance exporter."""
import copy
import json
import shutil
import tempfile
import unittest
from pathlib import Path

from tools.assets import manifest, sequence_music
from tools.assets.sequence_music_export import export_directory
from tools.assets.sequence_music_validate import validate_directory
from tools.assets import sequence_bank_provenance as bank
from tools.assets.sequence_bank_validate import _validate_output_manifest


ROOT = Path(manifest.REPOSITORY)
SEQ_MANIFEST = ROOT / ".asset-cache/music/sequence-candidate-v9/exports-v9-json/manifest.json"


class TrustedNativeProfileTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.authority = bank.verify_authority()
        cls.catalog, cls.packages, cls.catalog_sha = bank.validated_sequence_packages(SEQ_MANIFEST)
        cls.graph = bank.build_graph(cls.authority, cls.catalog, cls.packages, cls.catalog_sha)

    def test_original_source_native_and_iso_closure(self):
        self.assertEqual((self.authority["native_words"], self.authority["sdk_words"]), (24373, 839))
        self.assertEqual(self.authority["iso_file_record_joins"], 159)
        self.assertEqual(len(self.authority["iso_files"]), 139)

    def test_empty_source_or_native_proofs_are_rejected(self):
        profile = copy.deepcopy(self.authority["profile"])
        profile["source_pins"] = {}
        with self.assertRaisesRegex(ValueError, "source pin set"):
            bank._verify_original_code(profile)
        profile = copy.deepcopy(self.authority["profile"])
        source_path = next(iter(profile["source_pins"]))
        profile["source_pins"][source_path] = "0" * 64
        with self.assertRaisesRegex(ValueError, "source pin mismatch"):
            bank._verify_original_code(profile)
        profile = copy.deepcopy(self.authority["profile"])
        profile["verified_function_assembly"] = {}
        with self.assertRaisesRegex(ValueError, "proof profile"):
            bank._verify_original_code(profile)
        profile = copy.deepcopy(self.authority["profile"])
        profile["native_sites"] = []
        with self.assertRaisesRegex(ValueError, "proof profile"):
            bank._verify_original_code(profile)

    def test_slot_99_and_fabricated_vab_contract_are_rejected(self):
        profile = copy.deepcopy(self.authority["profile"])
        profile["load_set_contracts"][0]["music_vab_slot"] = 99
        with self.assertRaisesRegex(ValueError, "load-set"):
            bank._verify_original_code(profile)
        profile = copy.deepcopy(self.authority["profile"])
        profile["music_cache_condition"] = "always loaded from guessed filename"
        with self.assertRaisesRegex(ValueError, "cache"):
            bank._verify_original_code(profile)

    def test_reversed_fresh_c_tree_keeps_call_indexes_and_graph(self):
        with tempfile.TemporaryDirectory(prefix="bank-v3-reordered-", dir=manifest.CACHE) as temp:
            fake_src = Path(temp) / "src"
            c_files = list((ROOT / "src").rglob("*.c"))
            self.assertEqual(len(c_files), 437)
            for source in reversed(c_files):
                target = fake_src / source.relative_to(ROOT / "src")
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(source, target)
            reordered_calls = bank._verify_calls(self.authority["profile"], fake_src)
            self.assertEqual(reordered_calls, self.authority["calls"])
            authority = dict(self.authority)
            authority["calls"] = reordered_calls
            authority["contexts"] = bank.validate_context_catalog(authority)
            graph = bank.build_graph(authority, self.catalog, self.packages, self.catalog_sha)
            self.assertEqual(graph, self.graph)

    def test_fresh_valid_reexports_with_changed_context_and_unbound_seq_are_rejected(self):
        for iso_path in ("CDROM/VAB_SEQ/ASA.SEQ", "CDROM/VAB_SEQ/AUTUMN2.SEQ"):
            with self.subTest(path=iso_path), tempfile.TemporaryDirectory(
                    prefix="bank-v3-modified-seq-", dir=manifest.CACHE) as temp:
                fake_root = Path(temp)
                fake_input = fake_root / "disc/files/CDROM/VAB_SEQ"
                fake_input.mkdir(parents=True)
                for original in sorted((ROOT / "disc/files/CDROM/VAB_SEQ").glob("*.SEQ")):
                    target = fake_input / original.name
                    raw = original.read_bytes()
                    if original.name == Path(iso_path).name:
                        package = sequence_music.inspect_seq(raw, "disc/files/" + iso_path)
                        changed = False
                        for seq in package["sequences"]:
                            try:
                                events = sequence_music.parse_events(raw, seq)["events"]
                            except ValueError:
                                continue
                            event = next((item for item in events if item.get("kind") == 0xC0), None)
                            if event is None:
                                continue
                            content = bytearray(raw)
                            offset = event["source_end"] - 1
                            content[offset] = (content[offset] + 1) & 0x7F
                            raw = bytes(content)
                            changed = True
                            break
                        self.assertTrue(changed, "fixture has no complete program-change event")
                    target.write_bytes(raw)

                original_repo, original_cache = manifest.REPOSITORY, manifest.CACHE
                manifest.REPOSITORY = fake_root
                manifest.CACHE = fake_root / ".asset-cache"
                output_dir = manifest.CACHE / "music/reexport"
                try:
                    exported = export_directory(fake_input, output_dir)
                    self.assertEqual(exported["coverage"]["files"], 63)
                    validate_directory(fake_input, output_dir)
                    catalog = json.loads((output_dir / "manifest.json").read_text(encoding="utf-8"))
                    packages = {}
                    for row in catalog["files"]:
                        package_path = manifest.output_root(fake_root / row["output"]["path"])
                        packages[row["source"]["path"].removeprefix("disc/files/")] = json.loads(
                            package_path.read_text(encoding="utf-8"))
                    with self.assertRaisesRegex(ValueError, "differ from original ISO"):
                        bank.validate_sequence_iso_identity(self.authority, catalog, packages)
                finally:
                    manifest.REPOSITORY, manifest.CACHE = original_repo, original_cache

    def test_missing_and_duplicate_literal_contexts_are_rejected(self):
        profile = copy.deepcopy(self.authority["profile"])
        profile["calls"].pop()
        with self.assertRaisesRegex(ValueError, "call multiset"):
            bank._verify_calls(profile)
        profile = copy.deepcopy(self.authority["profile"])
        profile["calls"].append(copy.deepcopy(profile["calls"][0]))
        with self.assertRaisesRegex(ValueError, "call multiset"):
            bank._verify_calls(profile)

    def test_fake_vh_hash_and_missing_table_row_are_rejected(self):
        authority = copy.deepcopy(self.authority)
        authority["profile"]["literal_bindings"][0]["d_vh"] = {
            "path": "CDROM/VAB_SE/FABRICATED.VH", "lba": 1, "size": 1,
            "sha256": "0" * 64, "record_lba": 1, "record_offset": 0}
        with self.assertRaisesRegex(ValueError, "literal contexts"):
            bank.validate_context_catalog(authority)
        authority = copy.deepcopy(self.authority)
        authority["profile"]["conditional_table_bindings"].pop()
        with self.assertRaisesRegex(ValueError, "table rows"):
            bank.validate_context_catalog(authority)


class SelectedSequenceManifestTests(unittest.TestCase):
    def _alternate_manifest(self, mutate=None):
        temp = tempfile.TemporaryDirectory(prefix="bank-v2-manifest-", dir=manifest.CACHE)
        self.addCleanup(temp.cleanup)
        directory = Path(temp.name)
        value = json.loads(SEQ_MANIFEST.read_text(encoding="utf-8"))
        if mutate is not None:
            mutate(value, directory)
        path = directory / "manifest.json"
        path.write_text(json.dumps(value, ensure_ascii=True, sort_keys=True, indent=2) + "\n")
        return path

    def _mutate_package(self, catalog, directory, change):
        row = catalog["files"][0]
        original = manifest.output_root(ROOT / row["output"]["path"])
        package = json.loads(original.read_text(encoding="utf-8"))
        change(package, row)
        raw = (json.dumps(package, ensure_ascii=True, sort_keys=True, indent=2) + "\n").encode()
        package_path = directory / "mutated-package.json"
        package_path.write_bytes(raw)
        row["output"] = {"path": package_path.relative_to(ROOT).as_posix(),
                         "size": len(raw), "sha256": bank.sha256(raw)}

    def test_exact_alternate_manifest_is_the_manifest_validated(self):
        path = self._alternate_manifest()
        catalog, packages, _digest = bank.validated_sequence_packages(path)
        self.assertEqual(len(catalog["files"]), 63)
        self.assertEqual(len(packages), 63)

    def test_manifest_source_identity_mutation_is_rejected(self):
        def mutate(catalog, _directory):
            catalog["files"][0]["source"]["sha256"] = "0" * 64
        path = self._alternate_manifest(mutate)
        with self.assertRaises(ValueError):
            bank.validated_sequence_packages(path)

    def test_program_value_and_event_offset_mutations_are_rejected(self):
        for field in ("program", "event_offset"):
            def mutate(catalog, directory, field=field):
                def change(package, _row):
                    events = next(entry["event_parse"]["events"] for entry in package["entries"]
                                  if entry.get("event_parse", {}).get("events"))
                    if field == "program":
                        event = next(event for event in events if event.get("kind") == 0xC0)
                        event["data"][0] = (event["data"][0] + 1) & 0x7F
                    else:
                        events[0]["source_offset"] += 1
                self._mutate_package(catalog, directory, change)
            path = self._alternate_manifest(mutate)
            with self.subTest(field=field), self.assertRaises(ValueError):
                bank.validated_sequence_packages(path)

    def test_physical_offset_and_slot_99_mutations_are_rejected(self):
        for field in ("offset", "slot99"):
            def mutate(catalog, directory, field=field):
                def change(package, row):
                    entry = package["entries"][0]
                    if field == "offset":
                        entry["native_entry"]["offset"] += 1
                        entry["identity"]["physical_offset"] += 1
                        row["sequences"][0]["physical_offset"] += 1
                    else:
                        entry["identity"]["runtime_slots"] = [99]
                        row["sequences"][0]["runtime_slots"] = [99]
                self._mutate_package(catalog, directory, change)
            path = self._alternate_manifest(mutate)
            with self.subTest(field=field), self.assertRaises(ValueError):
                bank.validated_sequence_packages(path)

    def test_package_path_escape_is_rejected(self):
        def mutate(catalog, _directory):
            catalog["files"][0]["output"]["path"] = "../../../../tmp/forged-package.json"
        path = self._alternate_manifest(mutate)
        with self.assertRaises(ValueError):
            bank.validated_sequence_packages(path)

    def test_manifest_path_escape_is_rejected(self):
        with self.assertRaises(ValueError):
            bank.validated_sequence_packages(ROOT / "wiki/manifest.json")


class OutputManifestTests(unittest.TestCase):
    def test_schema_size_and_path_are_enforced(self):
        with tempfile.TemporaryDirectory(prefix="bank-v2-output-", dir=manifest.CACHE) as temp:
            out = Path(temp).resolve()
            graph = {"coverage": {"rows": 1}, "classification": "conditional"}
            raw = (json.dumps(graph, ensure_ascii=True, sort_keys=True, indent=2) + "\n").encode()
            path = out / "conditional-bank-provenance.json"
            path.write_bytes(raw)
            descriptor = {"path": path.relative_to(ROOT).as_posix(),
                          "size": len(raw), "sha256": bank.sha256(raw)}
            value = {"schema": "tokimemo.sequence-bank-provenance-manifest.v2",
                     "sequence_catalog_sha256": "a" * 64,
                     "trusted_profile_sha256": bank.PROFILE_SHA256,
                     "output": descriptor, "coverage": graph["coverage"],
                     "claim_scope": graph["classification"]}
            _validate_output_manifest(value, out, "a" * 64, graph)
            for key, bad in (("schema", "invalid"), ("output", dict(descriptor, size=0)),
                             ("output", dict(descriptor, path="../../tmp/out.json"))):
                changed = copy.deepcopy(value)
                changed[key] = bad
                with self.subTest(key=key, bad=bad), self.assertRaises(ValueError):
                    _validate_output_manifest(changed, out, "a" * 64, graph)


if __name__ == "__main__":
    unittest.main()
