"""Invented physical-storage and typed-table context link regressions."""
import unittest
import tempfile
from unittest.mock import patch
from tools.assets.text_export import export_text, _attach_storage_overlap_metadata, _attach_typed_dictionary_links, _attach_source_renderer_views


def record(rid, start, raw, digest="same"):
    return {"id": rid, "category": "unknown", "source": {"path": "TEST", "offset": start,
            "sha256": digest}, "raw_hex": raw.hex(), "raw_length": len(raw)}


class LinkTests(unittest.TestCase):
    def test_source_export_keeps_unresolved_evidence_and_enriched_font_links(self):
        site = {"source_site": {"call": "load_string", "call_argument_index": 0}, "raw_hex": "8140"}
        runtime_token = {"kind": "glyph", "raw_hex": "8140"}
        inventory = {"c_literal_sites": [site], "records": [], "blocked": [],
                     "source_context": {"blocked": []}}

        def mips_report(data, repository, files_root):
            self.assertIs(data, inventory)
            self.assertEqual(repository, files_root)
            data["mips_context"] = {"blocked": [{"status": "invented_failed_proof"}],
                                    "unresolved_constant_targets": [{"status": "invented_target"}]}
            return data

        def mutation_report(data, repository, files_root):
            data["runtime_text_mutations"] = {"blocked": [], "unresolved_candidate_ids": ["invented-write"],
                                              "rules": [{"targets": [{"conditional_runtime_view": {
                                                  "text": {"tokens": [runtime_token]}}}]}]}
            data["font"] = {"id": "font:INVENTED", "source_path": "INVENTED", "source_sha256": "synthetic",
                            "ranges": [{"first": 0x8140, "glyph": 0}, {"first": 0x8141, "glyph": 1}],
                            "glyph_count": 1, "glyph_offset": 0}
            return data

        def binary_report(data, repository, files_root):
            mips_report(data, repository, files_root)
            data["binary_text_supplement"] = {"records_added": 0, "blocked": [], "calls_considered": 2,
                "resolutions": [{"status": "recovered_verified_label_initial_string"},
                                {"status": "initially_zero_runtime_buffer"}]}
            return data

        with tempfile.TemporaryDirectory() as directory, \
                patch("tools.assets.text_source.inventory_source_text", return_value=inventory), \
                patch("tools.assets.text_context.enrich_source_context", return_value=inventory), \
                patch("tools.assets.text_binary.enrich_binary_text", side_effect=binary_report), \
                patch("tools.assets.script_mutations.enrich_script_mutations", side_effect=mutation_report):
            catalog = export_text(directory, profiles=[], include_source=True, source_repository=directory)
        self.assertIs(catalog["source_inventory"], inventory)
        self.assertFalse(catalog["text_completeness"]["complete"])
        self.assertEqual(catalog["text_completeness"]["source_mips_blockers"], 1)
        self.assertEqual(catalog["text_completeness"]["unresolved_mips_constant_targets"], 1)
        self.assertEqual(catalog["text_completeness"]["unresolved_runtime_write_candidates"], 1)
        self.assertEqual(site["renderer_view"]["tokens"][0]["font_reference"]["id"], "font:INVENTED:g0000")
        self.assertEqual(runtime_token["font_reference"]["id"], "font:INVENTED:g0000")

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


if __name__ == "__main__":
    unittest.main()
