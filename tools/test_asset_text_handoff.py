"""Synthetic translator-return contract tests (T-28007; no original game text)."""
import copy
import csv
import json
from pathlib import Path
import tempfile
import unittest

from tools.assets import text_handoff as handoff


def unit(identifier="synthetic:one", source="aGb", tokens=None):
    return {"id": identifier, "source_text": source, "raw_hex": source.encode("shift_jis").hex(),
            "target_text": None, "kind": "script_record", "source_path": "synthetic.bin",
            "context": {"section": "main", "group": 0, "line": 0},
            "tokens": tokens if tokens is not None else [
                {"kind": "substitution", "offset": 1, "raw_hex": "47", "name": "girl_name"}],
            "readiness": {"status": "ready_for_review", "translation_ready": True, "blockers": []}}


class HandoffTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.units = self.root / "units.jsonl"
        self.output = self.root / ".asset-cache/text/return.jsonl"

    def prepare(self, units=None):
        self.units.write_text("".join(json.dumps(value) + "\n" for value in (units or [unit()])), encoding="utf-8")
        batches = handoff.batches(self.units)
        relative, rows = next(iter(batches.items()))
        self.batch = self.root / Path(relative).name
        self.rows = rows
        self.write()

    def write(self):
        with self.batch.open("w", encoding="utf-8-sig", newline="") as stream:
            writer = csv.DictWriter(stream, fieldnames=handoff.FIELDS, delimiter="\t", lineterminator="\n")
            writer.writeheader()
            writer.writerows(self.rows)

    def run_import(self):
        return handoff.import_returns(self.units, [self.batch], self.output, self.root)

    def reject(self):
        self.write()
        with self.assertRaises(handoff.HandoffError):
            self.run_import()
        self.assertFalse(self.output.exists())

    def test_blank_return_preserves_source(self):
        self.prepare()
        before = self.units.read_bytes()
        self.assertEqual(self.run_import()["translated_rows"], 0)
        self.assertEqual(self.units.read_bytes(), before)
        self.assertEqual(self.output.read_bytes(), b"")

    def test_valid_return_no_old_length_limit(self):
        self.prepare()
        self.rows[0]["translation"] = "Example\t\n\"quote\" " * 10000 + "[[T001:girl_name]]"
        self.rows[0]["translator_note"] = "A note\nwith a line break"
        self.write()
        self.assertEqual(self.run_import()["translated_rows"], 1)
        row = json.loads(self.output.read_text())
        self.assertEqual(row["target_text"], self.rows[0]["translation"])

    def test_each_protected_column_rejected(self):
        self.prepare()
        for key in handoff.FIELDS:
            if key in handoff.EDITABLE:
                continue
            with self.subTest(key=key):
                saved = self.rows[0][key]
                self.rows[0][key] += "changed"
                self.reject()
                self.rows[0][key] = saved

    def test_missing_marker_rejected(self):
        self.prepare()
        self.rows[0]["translation"] = "Example"
        self.reject()

    def test_duplicate_marker_rejected(self):
        self.prepare()
        self.rows[0]["translation"] = "[[T001:girl_name]]" * 2
        self.reject()

    def test_unknown_marker_rejected(self):
        self.prepare()
        self.rows[0]["translation"] = "[[T001:girl_name]][[T002:fake]]"
        self.reject()

    def test_malformed_marker_rejected(self):
        self.prepare()
        self.rows[0]["translation"] = "[[T001:girl_name]][[T002"
        self.reject()

    def test_reordered_markers_rejected(self):
        value = unit(source="G P", tokens=[
            {"kind": "substitution", "offset": 0, "raw_hex": "47", "name": "girl"},
            {"kind": "separator", "offset": 1, "raw_hex": "20", "role": "row"},
            {"kind": "substitution", "offset": 2, "raw_hex": "50", "name": "player"}])
        self.prepare([value])
        references = json.loads(self.rows[0]["protected_tokens"])
        self.rows[0]["translation"] = "".join(ref["marker"] for ref in reversed(references))
        self.reject()

    def test_missing_row_rejected(self):
        self.prepare([unit(), unit("synthetic:two")])
        self.rows.pop()
        self.reject()

    def test_duplicate_row_rejected(self):
        self.prepare()
        self.rows.append(copy.deepcopy(self.rows[0]))
        self.reject()

    def test_duplicate_batch_rejected_before_output(self):
        self.prepare()
        with self.assertRaises(handoff.HandoffError):
            handoff.import_returns(self.units, [self.batch, self.batch], self.output, self.root)
        self.assertFalse(self.output.exists())

    def test_note_only_retains_null_target(self):
        self.prepare()
        self.rows[0]["translator_note"] = "Context needed"
        self.write()
        self.run_import()
        self.assertIsNone(json.loads(self.output.read_text())["target_text"])

    def test_inline_label_marker_only_protected(self):
        value = unit(source=">AB text", tokens=[{"kind": "inline_label", "offset": 0, "raw_hex": "3e4142"}])
        text, refs, blocker = handoff.template(value)
        self.assertIsNone(blocker)
        self.assertEqual(text, "[[T001:inline_label]]AB text")
        self.assertEqual(refs[0]["raw_hex"], "3e")

    def test_unresolved_keeps_status(self):
        value = unit()
        value["readiness"] = {"status": "flagged", "translation_ready": False, "blockers": ["runtime_unproved"]}
        self.prepare([value])
        self.rows[0]["translation"] = self.rows[0]["copy_template"]
        self.write()
        self.run_import()
        self.assertIn("runtime_unproved", json.loads(self.output.read_text())["review_status"])

    def test_bad_span_deferred(self):
        value = unit()
        value["tokens"][0]["offset"] = 0
        self.assertIn("token_spans_not_projectable", handoff.project(value)["review_status"])

    def test_literal_newline_protected(self):
        value = unit(source="abc\n", tokens=[{"kind": "format_literal", "offset": 0, "raw_hex": "6162630a"}])
        row = handoff.project(value)
        self.assertEqual(row["review_status"], "ready_for_review")
        self.assertEqual(row["copy_template"], "abc[[T001:line_feed]]")
        self.assertEqual(row["japanese"], "abc⟦U+000A⟧")

    def test_batch_limit_and_neighbor_links(self):
        self.prepare([unit(f"synthetic:{i}") for i in range(401)])
        batches = handoff.batches(self.units)
        self.assertEqual(sorted(map(len, batches.values())), [1, 400])
        first, second = batches.values()
        self.assertIn("next_id=synthetic:400", first[-1]["context"])
        self.assertIn("previous_id=synthetic:399", second[0]["context"])

    def test_output_safety(self):
        self.prepare()
        for output in (self.root / "outside.jsonl", self.root / ".asset-cache/text/../../outside.jsonl"):
            with self.assertRaises(handoff.HandoffError):
                handoff.import_returns(self.units, [self.batch], output, self.root)
        self.output.parent.mkdir(parents=True)
        self.output.symlink_to(self.units)
        with self.assertRaises(handoff.HandoffError):
            self.run_import()

    def test_source_targets_must_remain_null(self):
        value = unit()
        value["target_text"] = "unexpected"
        with self.assertRaises(handoff.HandoffError):
            handoff.project(value)


if __name__ == "__main__":
    unittest.main()
