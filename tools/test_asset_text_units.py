import csv
import json
from pathlib import Path
import tempfile
import unittest

from tools.assets.text_units import UnitExportError, export_units, iter_units
from tools.assets.text_overlay_tables import parse_overlay_fixed_tables


def _text(raw_hex, decoded, *, tokens=None):
    return {"raw_hex": raw_hex, "source_decoded_text": decoded,
            "source_decode_status": "ok", "tokens": tokens or [],
            "unknown_token_count": 0, "original_copy_limits": {"capacity": 12}}


def _catalog():
    script_record = {"id": "talk:fixture:g0000:l0000", "source_path": "CDROM/TEST.BIN",
                     "source_sha256": "a" * 64, "section": "s0", "group": 0, "line": 0,
                     "command": {"value": 1}, "speaker": {"label": "symbolic"},
                     "text": {**_text("4109420a00", "A\tB\n\0", tokens=[{"kind": "opaque", "raw_hex": "00"}]),
                              "offset": 32, "length": 5}}
    empty_record = {"id": "talk:fixture:g0000:l0001", "source_path": "CDROM/TEST.BIN",
                    "source_sha256": "a" * 64, "section": "s0", "group": 0, "line": 1,
                    "command": {"value": 1}, "speaker": {},
                    "text": {**_text("", ""), "offset": 40, "length": 0}}
    extent_id = "CDROM/EXEDIR/UNIT.EXN:00000080:aaaaaaaaaaaa"
    dictionary_item = {"id": "dictionary:fixture:names:01", "table": "names", "index": 1,
                       "source_path": "SLPM.fixture", "source_sha256": "c" * 64,
                       "offset": 128, "length": 3, "slot_offset": 4, "slot_raw_hex": "00000000",
                       "source_inventory_record_ids": [extent_id],
                       "context": {"girl": 1, "year": 2, "answer": 3, "place": 4, "reserved": 5},
                       "text": _text("58595a", "XYZ", tokens=[{"kind": "glyph", "raw_hex": "5859"}])}
    empty_dictionary_item = {"id": "dictionary:fixture:names:00", "table": "names", "index": 0,
                       "source_path": "SLPM.fixture", "source_sha256": "b" * 64,
                       "offset": 8, "length": 0, "slot_offset": 0, "slot_raw_hex": "00000000",
                       "text": _text("", "")}
    gift_item = {"id": "dictionary:fixture:gift_names:00", "table": "gift_names", "index": 0,
                 "source_path": "SLPM.fixture", "source_sha256": "b" * 64,
                 "offset": 12, "length": 1, "slot_offset": 12, "slot_raw_hex": "5100",
                 "text": _text("51", "Q", tokens=[{"kind": "renderer_rejected_pair", "raw_hex": "5100"}])}
    source_record = {"id": extent_id, "stable_id": None,
                     "source": {"path": "SLPM.fixture", "offset": 128, "sha256": "c" * 64},
                     "raw_hex": "58595a00", "raw_length": 4, "source_decoded_text": "XYZ",
                     "source_decode_status": "ok", "source_roundtrip": True,
                     "extent_status": "verified_by_assembly_prefix_and_original_terminal_nul",
                     "reference_status": "direct_assembler_symbol_refs_found",
                     "category": "display", "category_basis": "unique source-proven consumer role",
                     "context_category": "menu", "context_category_basis": "unique source-proven consumer role",
                     "source_contexts": [], "mips_contexts": [], "source_terminal_nul_excluded": True}
    zero_record = {"id": "source:padding", "stable_id": "main:80001004:0",
                   "source": {"path": "SLPM.fixture", "offset": 132, "sha256": "c" * 64},
                   "raw_hex": "", "raw_length": 0, "source_decode_status": "ok"}
    c_site = {"stable_id": "c:main:80002000:0", "stable_id_status": "verified_matching_function_entry_and_local_ordinal",
              "source_locator_id": "path:line:locator", "source_decode_status": "ok",
              "source_decoded_text": "XYZ", "raw_hex": "58595a", "raw_length": 3,
              "source_site": {"path": "src/renamed.c", "call": "sndi", "call_argument_index": 0},
              "binary_status": "candidate_found_unconfirmed", "binary_candidate_status": "single",
              "function_reference_status": "unique_same_function_label_and_exact_bytes",
              "function_binary_candidates": [extent_id],
              "link_unit": "main", "category": "display", "category_basis": "proven",
              "renderer_view_status": "tokenized", "renderer_view": {"renderer": "sndi",
              "tokens": [{"kind": "glyph", "raw_hex": "5859"}]}}
    unresolved_site = {**c_site, "stable_id": None, "stable_id_status": "unresolved_original_provenance",
                       "source_locator_id": "src/unresolved.c:9:7", "source_site": {"path": "src/unresolved.c", "line": 9,
                       "column": 7, "call": None}, "function_reference_status": "function_unresolved",
                       "function_binary_candidates": [], "raw_hex": "52", "source_decoded_text": "R"}
    conditional = {"rule_id": "fixture-rule", "selection_status": "source_proved",
                   "targets": [{"record_id": script_record["id"], "source_path": "CDROM/TEST.BIN",
                                "source_sha256": "a" * 64, "offset": 32, "relative_byte_offset": 0,
                                "conditional_runtime_view": {
                                    "source_record_id": script_record["id"],
                            "status": "conditional_derived_view", "conditions": ["selector"],
                                    "source_storage_unchanged": True,
                                    "text": {**_text("52", "R"), "length": 1}}}]}
    return {"schema": "tokimemo.text.catalog.v1",
            "files": [{"status": "exported", "validation": {"structural_checks_pass": True,
                         "strict_source_decode_failures": 0, "renderer_anomalies": []},
                       "script": {"profile_id": "fixture", "records": [script_record, empty_record]}}],
            "dictionaries": [{"profile": {"tables": [{"id": "names", "role": "name"},
                                                         {"id": "gift_names", "kind": "fixed_strings"}]},
                              "records": [empty_dictionary_item, dictionary_item, gift_item]}],
            "source_inventory": {"records": [source_record, zero_record], "c_literal_sites": [c_site, unresolved_site]},
            "runtime_text_mutations": {"rules": [conditional]}}


class TextUnitProjectionTests(unittest.TestCase):
    def test_layout_candidates_append_without_promoting_semantics_or_controls(self):
        from tools.assets.text_speaktst import FLAGS
        catalog = _catalog()
        before = list(iter_units(catalog))
        tokens = [{"kind": "unknown", "raw_hex": "41", "offset": 0}]
        spans = {"text_with_nul": {"raw_hex": "4100"}, "record24": {"raw_hex": "00" * 24}}
        candidate = {"id": "layout:invented:1", "physical_text_id": "physical:invented:1",
                     "source_path": "CDROM/SPEAKTST/TEST.fixture", "source_sha256": "d" * 64,
                     "source_offset": 32, "raw_hex": "41", "source_text": "A", "tokens": tokens,
                     "spans": spans, "candidate_grammar": {"command_value": 1},
                     "readiness": {"translation_ready": True}, "target_text": "forged",
                     "runtime_usage_proven": True}
        catalog["layout_only_inventories"] = [{"status": "layout_only_inventory", "records": [candidate],
                                              "controls": [{"id": "layout:invented:end"}]}]
        after = list(iter_units(catalog))
        self.assertEqual(after[:-1], before)
        row = after[-1]
        self.assertEqual(row["id"], candidate["id"])
        self.assertEqual(row["parent_id"], candidate["physical_text_id"])
        self.assertEqual(row["kind"], "speaktst_layout_text")
        self.assertEqual(row["raw_hex"], "41")
        self.assertEqual(row["tokens"], tokens)
        self.assertEqual(row["context"]["spans"], spans)
        self.assertIsNone(row["target_text"])
        self.assertFalse(row["readiness"]["translation_ready"])
        self.assertEqual(row["readiness"]["status"], "flagged")
        self.assertTrue(set(FLAGS).issubset(row["readiness"]["blockers"]))
        self.assertFalse(row["context"]["runtime_usage_proven"])
        self.assertFalse(row["context"]["record24_voice_semantics_proven"])
        self.assertFalse(row["context"]["metadata_character_semantics_proven"])

    def test_initial_runtime_buffer_units_are_always_flagged_and_keep_protected_storage(self):
        item = {"id": "initial-body", "source_physical_record_id": "initial-slot",
                "source_path": "TEST", "source_sha256": "same", "offset": 4, "full_offset": 4,
                "text": _text("4142", "AB"), "full_raw_hex": "414200ff",
                "full_tokens": [{"kind": "glyph", "offset": 0, "raw_hex": "4142"},
                                {"kind": "protected_source_terminator", "offset": 2, "raw_hex": "00",
                                 "translatable": False},
                                {"kind": "protected_slot_padding", "offset": 3, "raw_hex": "ff",
                                 "translatable": False}],
                "context": {"initial_contents_only": True}, "source_inventory_record_ids": ["source-extent"]}
        rows = list(iter_units({"schema": "tokimemo.text.catalog.v1",
                               "initial_runtime_buffer_banks": [{"status": "exported", "records": [item]}]}))
        self.assertEqual(len(rows), 1)
        row = rows[0]
        self.assertEqual(row["kind"], "initial_runtime_buffer")
        self.assertEqual(row["raw_hex"], "4142")
        self.assertEqual(row["parent_id"], "initial-slot")
        self.assertIsNone(row["target_text"])
        self.assertFalse(row["readiness"]["translation_ready"])
        self.assertIn("runtime_buffer_value_unproven", row["readiness"]["blockers"])
        self.assertEqual(row["context"]["full_raw_hex"], "414200ff")
        self.assertFalse(row["context"]["runtime_read_value_proven"])
        self.assertFalse(row["context"]["runtime_reachability_proven"])
        self.assertFalse(row["context"]["original_capacities_are_translation_limits"])

    def test_projection_keeps_empty_slots_and_stable_identity(self):
        rows = list(iter_units(_catalog()))
        self.assertEqual(9, len(rows))
        by_kind = {kind: [row for row in rows if row["kind"] == kind]
                   for kind in {row["kind"] for row in rows}}
        self.assertEqual(2, len(by_kind["script_record"]))
        self.assertEqual("empty_source_slot", by_kind["script_record"][1]["readiness"]["blockers"][0])
        self.assertEqual(3, len(by_kind["dictionary_slot"]))
        self.assertIn("empty_dictionary_slot", by_kind["dictionary_slot"][0]["readiness"]["blockers"])
        gift = next(row for row in by_kind["dictionary_slot"] if row["context"]["table"] == "gift_names")
        self.assertEqual("item_name", gift["context"]["table_role"])
        self.assertIn("renderer_tokens_contain_unsupported_or_unmapped_units", gift["readiness"]["blockers"])
        self.assertEqual({"girl": 1, "year": 2, "answer": 3, "place": 4, "reserved": 5},
                         next(row for row in by_kind["dictionary_slot"]
                              if row["context"]["table"] == "names" and
                              row["context"]["index"] == 1)["context"]["source_context"])
        self.assertEqual("CDROM/EXEDIR/UNIT.EXN:00000080:aaaaaaaaaaaa", by_kind["source_extent"][0]["id"])
        self.assertEqual(by_kind["source_extent"][0]["id"], by_kind["source_extent"][0]["context"]["source_record_id"])
        self.assertEqual("XYZ", by_kind["source_extent"][0]["source_text"])
        self.assertEqual([], by_kind["source_extent"][0]["tokens"])
        self.assertEqual(by_kind["source_extent"][0]["id"], by_kind["source_literal"][0]["parent_id"])
        self.assertTrue(by_kind["source_literal"][0]["readiness"]["translation_ready"])
        self.assertIn("renderer_view_ambiguous", by_kind["source_extent"][0]["readiness"]["blockers"])
        self.assertTrue(any(row["id"].startswith("unresolved-c-locator:") and
                            "stable_source_identity_unresolved" in row["readiness"]["blockers"]
                            for row in by_kind["source_literal"]))
        self.assertEqual(len(rows), len({row["id"] for row in rows}))
        self.assertFalse(by_kind["conditional_runtime_variant"][0]["readiness"]["translation_ready"])
        self.assertIsNone(rows[0]["target_text"])

    def test_jsonl_tsv_roundtrip_hash_manifest_and_zero_skip(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            (root / ".asset-cache" / "text").mkdir(parents=True)
            manifest = export_units(_catalog(), root / ".asset-cache" / "text" / "projection",
                                    repository=root)
            self.assertEqual(9, manifest["row_count"])
            self.assertEqual(1, manifest["skipped_zero_source_extents"])
            second = export_units(_catalog(), root / ".asset-cache" / "text" / "projection",
                                  repository=root)
            self.assertEqual(manifest["files"]["jsonl"]["sha256"], second["files"]["jsonl"]["sha256"])
            jsonl = root / ".asset-cache" / "text" / "projection" / "units.jsonl"
            rows = [json.loads(line) for line in jsonl.read_text(encoding="utf-8").splitlines()]
            escaped = next(row for row in rows if row["kind"] == "script_record")
            self.assertEqual("A\tB\n\0", escaped["source_text"])
            self.assertIsNone(escaped["target_text"])
            tsv_path = jsonl.with_name("units.tsv")
            with tsv_path.open(encoding="utf-8", newline="") as stream:
                tsv_rows = list(csv.DictReader(stream, delimiter="\t"))
            source_value = next(row["source_text"] for row in tsv_rows if row["kind"] == "script_record")
            self.assertEqual("A\tB\n\u0000", json.loads(source_value))
            self.assertEqual("null", next(row["target_text"] for row in tsv_rows if row["kind"] == "script_record"))

    def test_unknown_and_embedded_nul_source_extents_stay_flagged(self):
        catalog = _catalog()
        record = catalog["source_inventory"]["records"][0]
        record.update(raw_hex="410042", raw_length=3, source_decoded_text="A\0B",
                      reference_status="ambiguous", context_category="unknown")
        row = next(row for row in iter_units(catalog) if row["kind"] == "source_extent")
        self.assertIn("embedded_nul_extent", row["readiness"]["blockers"])
        self.assertIn("consumer_reference_unresolved_or_ambiguous", row["readiness"]["blockers"])
        self.assertIn("consumer_context_unclassified", row["readiness"]["blockers"])

    def test_literal_binding_removes_only_explicit_terminal_nul(self):
        catalog = _catalog()
        extent = catalog["source_inventory"]["records"][0]
        extent.update(raw_hex="41004200", raw_length=4, source_decoded_text="A\0B")
        site = catalog["source_inventory"]["c_literal_sites"][0]
        site.update(raw_hex="410042", raw_length=3, source_decoded_text="A\0B")
        row = next(row for row in iter_units(catalog)
                   if row["kind"] == "source_literal" and row["id"] == site["stable_id"])
        self.assertEqual(extent["id"], row["parent_id"])
        self.assertIn("embedded_nul_extent", row["readiness"]["blockers"])

    def test_inline_label_marker_is_protected_but_label_text_is_translatable(self):
        catalog = _catalog()
        record = catalog["files"][0]["script"]["records"][0]
        record["text"]["speaker_label"] = {"kind": "inline_label", "marker": ">",
            "raw_hex": "3e5859", "label_raw_hex": "5859", "label_text": "XY",
            "unknown_label_glyphs": 0}
        row = next(row for row in iter_units(catalog) if row["kind"] == "script_record")
        segments = row["context"]["translation_segments"]
        self.assertEqual(["protected_marker", "translatable_inline_label"],
                         [segment["kind"] for segment in segments])
        self.assertEqual("XY", segments[1]["source_text"])
        self.assertEqual(">", segments[0]["source_text"])

    def test_binary_initial_view_keeps_runtime_limits_and_is_not_full_read_proof(self):
        catalog = _catalog()
        record = catalog["source_inventory"]["records"][0]
        record["binary_initial_renderer_views"] = [{
            "renderer": "load_string", "call_pc": 0x80001000,
            "view": {"renderer": "load_string", "tokens": [{"kind": "glyph", "raw_hex": "5859"}]},
            "runtime_read_length_proven": False,
        }]
        row = next(row for row in iter_units(catalog) if row["kind"] == "source_extent")
        self.assertTrue(any(view.get("source") == "verified_binary_initial_view" and
                            view.get("runtime_read_length_proven") is False
                            for view in row["context"]["renderer_views"]))
        self.assertIn("runtime_read_length_unproven", row["readiness"]["blockers"])

    def test_unterminated_overlay_slot_keeps_raw_bytes_and_context(self):
        profile = {"id": "invented", "path": "CDROM/EXEDIR/TEST.EXN", "load_address": 0x1000,
                   "tables": [{"id": "labels", "role": "name", "address": 0x1000,
                               "count": 1, "stride": 4, "dimensions": [1], "context": ["choice_index"]}]}
        dictionary = parse_overlay_fixed_tables(b"ABCD", profile)
        catalog = {"schema": "tokimemo.text.catalog.v1", "dictionaries": [dictionary]}
        row, = iter_units(catalog)
        self.assertIsNone(row["source_text"])
        self.assertEqual(row["raw_hex"], "41424344")
        self.assertEqual(row["source_offset"], 0)
        self.assertEqual(row["context"]["string_extent_status"], "no_nul_within_slot")
        self.assertEqual(row["context"]["source_table_context"]["choice_index"], 0)
        self.assertFalse(row["readiness"]["translation_ready"])

    def test_stage_unit_translates_body_and_keeps_full_protected_storage(self):
        item = {"id": "stage-body", "source_physical_record_id": "physical", "source_path": "TEST",
                "source_sha256": "same", "offset": 5, "full_offset": 4, "full_raw_hex": "6782a000",
                "full_tokens": [{"kind": "protected_control_prefix", "raw_hex": "67", "translatable": False}],
                "source_slot_status": "source_text", "context": {"body_relative_offset": 1},
                "text": {"raw_hex": "82a0", "source_decoded_text": "あ", "source_decode_status": "strict_roundtrip",
                         "tokens": [{"kind": "glyph", "raw_hex": "82a0"}]}}
        row, = iter_units({"schema": "tokimemo.text.catalog.v1",
                           "stage_text_banks": [{"status": "exported", "records": [item], "issues": []}]})
        self.assertEqual((row["kind"], row["parent_id"], row["source_offset"]), ("stage_text_record", "physical", 5))
        self.assertEqual((row["source_text"], row["raw_hex"]), ("あ", "82a0"))
        self.assertEqual(row["context"]["full_raw_hex"], "6782a000")
        self.assertFalse(row["context"]["full_tokens"][0]["translatable"])

    def test_rejects_output_escape_and_symlink(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            cache = root / ".asset-cache" / "text"
            cache.mkdir(parents=True)
            with self.assertRaises(UnitExportError):
                export_units(_catalog(), root / "outside", repository=root)
            target = cache / "linked"
            target.symlink_to(root, target_is_directory=True)
            with self.assertRaises(UnitExportError):
                export_units(_catalog(), target / "out", repository=root)


if __name__ == "__main__":
    unittest.main()
