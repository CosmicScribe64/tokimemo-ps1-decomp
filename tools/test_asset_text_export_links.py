"""Invented physical-storage and typed-table context link regressions."""
import unittest
import tempfile
import hashlib
import json
from pathlib import Path
from unittest.mock import patch
from tools.assets.text_export import (export_text, _attach_storage_overlap_metadata, _attach_typed_dictionary_links,
                                      _attach_source_renderer_views, _attach_stage_storage_links,
                                      _attach_symbolic_buffer_bindings, _attach_initial_buffer_storage_links)


def record(rid, start, raw, digest="same"):
    return {"id": rid, "category": "unknown", "source": {"path": "TEST", "offset": start,
            "sha256": digest}, "raw_hex": raw.hex(), "raw_length": len(raw)}


class LinkTests(unittest.TestCase):
    def test_initial_buffer_links_preserve_unknown_category_and_revoke_stale_links(self):
        exact = record("initial", 4, b"AB\0")
        other_hash = record("other", 4, b"AB\0", "other")
        interior = record("interior", 5, b"AB\0")
        item = {"id": "initial-body", "source_physical_record_id": "initial-slot",
                "source_path": "TEST", "source_sha256": "same", "offset": 4,
                "text": {"raw_hex": "4142"}, "full_raw_hex": "414200ff"}
        bank = {"status": "exported", "records": [item]}
        catalog = {"source_inventory": {"records": [exact, other_hash, interior]},
                   "initial_runtime_buffer_banks": [bank]}
        _attach_initial_buffer_storage_links(catalog)
        self.assertEqual(item["source_inventory_record_ids"], ["initial"])
        self.assertEqual(exact["category"], "unknown")
        self.assertFalse(exact["initial_runtime_buffer_contexts"][0]["runtime_read_value_proven"])
        _attach_initial_buffer_storage_links(catalog)
        self.assertEqual(len(exact["initial_runtime_buffer_contexts"]), 1)
        self.assertEqual(catalog["source_inventory"]["initial_runtime_buffer_storage_links"], 1)
        bank["status"] = "source_evidence_not_verified"
        _attach_initial_buffer_storage_links(catalog)
        self.assertNotIn("initial_runtime_buffer_contexts", exact)
        self.assertEqual(item["source_inventory_record_ids"], [])
        self.assertEqual(exact["category"], "unknown")

    def test_stage_links_distinguish_control_storage_and_display_body(self):
        full, body = b"g\x82\xa0\0", b"\x82\xa0"
        source_full, source_body = record("full", 4, full), record("body", 5, body + b"\0")
        wrong_hash = record("other-release", 4, full, "other")
        item = {"id": "stage", "source_path": "TEST", "source_sha256": "same",
                "offset": 5, "full_offset": 4, "full_raw_hex": full.hex(), "text": {"raw_hex": body.hex()}}
        catalog = {"source_inventory": {"records": [source_full, source_body, wrong_hash]},
                   "stage_text_banks": [{"status": "exported", "records": [item]}]}
        _attach_stage_storage_links(catalog)
        self.assertEqual(source_full["category"], "structured_stage_storage")
        self.assertEqual(source_body["category"], "display")
        self.assertEqual(wrong_hash["category"], "unknown")
        self.assertEqual(source_full["raw_hex"], full.hex())
        self.assertEqual(item["source_inventory_record_ids"], ["body", "full"])
        _attach_stage_storage_links(catalog)
        self.assertEqual(len(source_full["stage_bank_contexts"]), 1)
        catalog["stage_text_banks"][0]["status"] = "source_evidence_not_verified"
        _attach_stage_storage_links(catalog)
        self.assertEqual(source_full["category"], "unknown")
        self.assertEqual(source_body["category"], "unknown")
        self.assertEqual(source_full["stage_bank_contexts"], [])
        self.assertEqual(item["source_inventory_record_ids"], [])

    def test_symbolic_bindings_remain_unknown_and_are_revoked_without_proof(self):
        token = {"kind": "substitution", "code": "M", "raw_hex": "4d"}
        catalog = {"tokens": [token], "symbolic_substitution_contract": {
            "status": "verified_source_and_original_image_pins",
            "buffers": [{"symbol": "BUFFER", "address": 0x1000, "initial_bytes": 4}],
            "marker_branches": [{"marker": "M", "buffer_symbol": "BUFFER"}]}}
        _attach_symbolic_buffer_bindings(catalog)
        self.assertEqual(token["source_buffer_binding"]["runtime_contents"], "unknown")
        self.assertFalse(token["source_buffer_binding"]["capacity_is_translation_limit"])
        self.assertEqual(token["raw_hex"], "4d")
        catalog["symbolic_substitution_contract"]["status"] = "source_contract_not_verified"
        _attach_symbolic_buffer_bindings(catalog)
        self.assertNotIn("source_buffer_binding", token)

    def test_source_export_keeps_unresolved_evidence_and_enriched_font_links(self):
        site = {"source_site": {"call": "load_string", "call_argument_index": 0}, "raw_hex": "8140"}
        runtime_token = {"kind": "glyph", "raw_hex": "8140"}
        inventory = {"c_literal_sites": [site], "records": [], "blocked": [],
                     "source_context": {"blocked": []}}
        expected_reads = None

        def mips_report(data, repository, files_root, *, additional_read_specs=None):
            self.assertIs(data, inventory)
            self.assertEqual(repository, files_root)
            self.assertEqual(additional_read_specs, expected_reads)
            data["mips_context"] = {"blocked": [{"status": "invented_failed_proof"}],
                                    "unresolved_constant_targets": [{"status": "invented_target"}]}
            return data

        def mutation_report(data, repository, files_root):
            data["runtime_text_mutations"] = {"blocked": [], "unresolved_candidate_ids": ["invented-write"],
                                              "source_write_inventory": {"opaque_value_dependencies": [{}]},
                                              "rules": [{"targets": [{"conditional_runtime_view": {
                                                  "text": {"tokens": [runtime_token]}}}]}]}
            data["font"] = {"id": "font:INVENTED", "source_path": "INVENTED", "source_sha256": "synthetic",
                            "ranges": [{"first": 0x8140, "glyph": 0}, {"first": 0x8141, "glyph": 1}],
                            "glyph_count": 1, "glyph_offset": 0}
            return data

        def binary_report(data, repository, files_root):
            data["binary_text_supplement"] = {"records_added": 0, "blocked": [], "calls_considered": 2,
                "resolutions": [{"status": "recovered_verified_label_initial_string"},
                                {"status": "initially_zero_runtime_buffer"}]}
            return data

        with tempfile.TemporaryDirectory() as directory, \
                patch("tools.assets.text_source.inventory_source_text", return_value=inventory), \
                patch("tools.assets.text_context.enrich_source_context", return_value=inventory), \
                patch("tools.assets.text_mips.enrich_mips_context", side_effect=mips_report), \
                patch("tools.assets.text_shooter.verify_shooter_text_contract", return_value={"verified": False}), \
                patch("tools.assets.text_shooter_labels.verify_shooter_label_contract", return_value={"verified": False}), \
                patch("tools.assets.text_binary._enrich_binary_text_from_reports", side_effect=binary_report), \
                patch("tools.assets.script_mutations.enrich_script_mutations", side_effect=mutation_report):
            provenance = {"schema": "synthetic-source-witness", "source_tree_sha256": "synthetic-tree",
                          "stable_copy_verified": True, "revision": "synthetic-revision",
                          "build_source_digest": "synthetic-build", "matching_proof_sha256": "synthetic-proof"}
            manifest_raw = json.dumps(provenance).encode()
            (Path(directory) / "snapshot-manifest.json").write_bytes(manifest_raw)
            catalog = export_text(directory, profiles=[], include_source=True, source_repository=directory)
            from tools.assets.text_shooter import SHOOTER_TEXT_READ_SPEC
            from tools.assets.text_shooter_labels import SHOOTER_LABEL_READ_SPECS
            self.assertEqual(set(SHOOTER_LABEL_READ_SPECS), {"shooter_draw_label", "shooter_draw_ranking_text"})
            expected_addresses = {"shooter_draw_label": 0x801357B0, "shooter_draw_ranking_text": 0x801371B4}
            for first_ok, labels_ok in ((True, False), (False, True), (True, True)):
                with self.subTest(first_contract=first_ok, label_contract=labels_ok):
                    expected_reads = {"TACO": {}}
                    if first_ok:
                        expected_reads["TACO"]["shooter_draw_text"] = ({**SHOOTER_TEXT_READ_SPEC,
                            "source_contract": "shooter_renderer_contract"},)
                        self.assertEqual(expected_reads["TACO"]["shooter_draw_text"][0]["callee_address"], 0x801432F0)
                    if labels_ok:
                        for call, spec in SHOOTER_LABEL_READ_SPECS.items():
                            expected_reads["TACO"][call] = ({**spec,
                                "source_contract": "shooter_label_renderer_contract"},)
                            self.assertEqual(spec["index"], 0)
                            self.assertEqual(spec["callee_address"], expected_addresses[call])
                            self.assertIsNone(spec["renderer"])
                    with patch("tools.assets.text_shooter.verify_shooter_text_contract", return_value={"verified": first_ok}), \
                            patch("tools.assets.text_shooter_labels.verify_shooter_label_contract", return_value={"verified": labels_ok}):
                        positive = export_text(directory, profiles=[], include_source=True, source_repository=directory)
                    self.assertEqual(positive["text_completeness"]["shooter_renderer_contract_blockers"], int(not first_ok))
                    self.assertEqual(positive["text_completeness"]["shooter_label_renderer_contract_blockers"], int(not labels_ok))
        self.assertIs(catalog["source_inventory"], inventory)
        self.assertFalse(catalog["text_completeness"]["complete"])
        self.assertEqual(catalog["text_completeness"]["source_mips_blockers"], 1)
        self.assertEqual(catalog["text_completeness"]["shooter_renderer_contract_blockers"], 1)
        self.assertEqual(catalog["text_completeness"]["shooter_label_renderer_contract_blockers"], 1)
        self.assertEqual(catalog["text_completeness"]["unresolved_mips_constant_targets"], 1)
        self.assertEqual(catalog["text_completeness"]["unresolved_runtime_write_candidates"], 1)
        self.assertEqual(catalog["text_completeness"]["unresolved_opaque_value_dependencies"], 1)
        self.assertEqual(site["renderer_view"]["tokens"][0]["font_reference"]["id"], "font:INVENTED:g0000")
        self.assertEqual(runtime_token["font_reference"]["id"], "font:INVENTED:g0000")
        self.assertEqual(catalog["source_evidence_snapshot"]["revision"], "synthetic-revision")
        self.assertEqual(catalog["source_evidence_snapshot"]["build_source_digest"], "synthetic-build")
        self.assertEqual(catalog["source_evidence_snapshot"]["manifest_sha256"], hashlib.sha256(manifest_raw).hexdigest())

    def test_only_actual_format_argument_gets_format_tokens(self):
        fmt = {"source_site": {"call": "sprintf", "call_argument_index": 1}, "raw_hex": b"%s".hex()}
        payload = {"source_site": {"call": "sprintf", "call_argument_index": 2}, "raw_hex": b"payload%d".hex()}
        coordinate = {"source_site": {"call": "sndi", "call_argument_index": 1}, "raw_hex": b"not-text".hex()}
        inventory = {"c_literal_sites": [fmt, payload, coordinate]}
        _attach_source_renderer_views(inventory)
        self.assertEqual(fmt["renderer_view"]["tokens"][0]["kind"], "format_directive")
        self.assertNotIn("renderer_view", payload)
        self.assertNotIn("renderer_view", coordinate)

    def test_overlaps_preserve_distinct_views_and_do_not_cross_versions(self):
        a, b = record("full", 4, b"ABC\0"), record("fixed", 4, b"AB")
        separate = record("other-release", 4, b"AB", "different")
        inventory = {"records": [a, b, separate, record("adjacent", 8, b"D\0")]}
        _attach_storage_overlap_metadata(inventory)
        self.assertEqual(inventory["storage_overlap_pairs"], 1)
        self.assertEqual(a["storage_overlaps"], [{"record_id": "fixed", "offset": 4, "length": 2}])
        self.assertNotIn("storage_overlaps", separate)
        self.assertEqual(len(inventory["records"]), 4)

    def test_typed_role_requires_hash_address_and_exact_body(self):
        good = record("good", 4, b"AB\0")
        prefix = record("prefix", 4, b"ABC\0")
        wrong_hash = record("wrong-hash", 4, b"AB\0", "different")
        wrong_address = record("wrong-address", 5, b"AB\0")
        item = {"id": "place-0", "source_path": "TEST", "offset": 4, "source_sha256": "same",
                "table": "place_names", "index": 0, "text": {"raw_hex": b"AB".hex()}}
        catalog = {"source_inventory": {"records": [good, prefix, wrong_hash, wrong_address]},
                   "dictionaries": [{"profile": {"evidence": ["invented.c:table"]}, "records": [item]}]}
        _attach_typed_dictionary_links(catalog)
        self.assertEqual(item["source_inventory_record_ids"], ["good"])
        self.assertEqual(good["category"], "place")
        self.assertTrue(all(r["category"] == "unknown" for r in (prefix, wrong_hash, wrong_address)))

    def test_date_gift_dictionary_context_is_linked_as_typed_item_name(self):
        raw = b"\x81\x40"
        source_record = record("date-source", 0x2B108, raw, "date-sha")
        source_record["source"]["path"] = "CDROM/EXEDIR/DATE.EXN"
        item = {"id": "date-gift-slot", "source_path": "CDROM/EXEDIR/DATE.EXN",
                "offset": 0x2B108, "source_sha256": "date-sha", "table": "gift_names",
                "index": 8, "context": {"girl_index": 0, "year_index": 2, "answer_index": 2},
                "text": {"raw_hex": raw.hex()}}
        catalog = {"source_inventory": {"records": [source_record]},
                   "dictionaries": [{"profile": {"evidence": ["synthetic source evidence"]},
                                     "records": [item]}]}
        _attach_typed_dictionary_links(catalog)
        self.assertEqual(source_record["category"], "item_name")
        self.assertEqual(source_record["typed_dictionary_contexts"][0]["index_context"], item["context"])
        self.assertEqual(item["source_inventory_record_ids"], ["date-source"])

    def test_actor_resource_has_no_whole_file_absence_claim_and_custom_profiles_stay_explicit(self):
        payload = bytes(range(16))
        resource = {"id": "fixture-resource", "path": "CDROM/MASTER/TEAR.BIN",
                    "sha256": hashlib.sha256(payload).hexdigest(), "size": len(payload),
                    "load_address": 0x1000, "actor_resource_roots": [0x1000],
                    "actor_halfword_address": 0x1004, "status": "fixture_actor_resource"}
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            master = root / "CDROM/MASTER"
            speech = root / "CDROM/SPEAKTST"
            master.mkdir(parents=True)
            speech.mkdir(parents=True)
            (master / "TEAR.BIN").write_bytes(payload)
            (speech / "TEST.BIN").write_bytes(payload)
            with patch("tools.assets.text_export.PROFILES", []), \
                    patch("tools.assets.text_export.RESOURCE_PROFILES", [resource]), \
                    patch("tools.assets.text_export.NON_TEXT_PROFILES", []), \
                    patch("tools.assets.text_export.NAME_KAN_PROFILE", {"path": "missing"}), \
                    patch("tools.assets.text_export.DICTIONARY_PROFILES", []), \
                    patch("tools.assets.text_export.DATE_TABLE_PROFILE", {"path": "missing"}):
                default_catalog = export_text(root)
                tear = next(item for item in default_catalog["files"] if item["path"] == resource["path"])
                self.assertEqual(tear["status"], "source_resource")
                self.assertFalse(tear["whole_file_text_absence_proven"])
                self.assertEqual(next(item for item in default_catalog["files"]
                                      if item["path"] == "CDROM/SPEAKTST/TEST.BIN")["status"], "unsupported")
                custom_catalog = export_text(root, profiles=[])
                self.assertEqual(next(item for item in custom_catalog["files"]
                                      if item["path"] == resource["path"])["status"], "unsupported")
                self.assertNotIn("dictionaries", custom_catalog)
                self.assertEqual(custom_catalog["source_resource_files"], 0)


if __name__ == "__main__":
    unittest.main()
