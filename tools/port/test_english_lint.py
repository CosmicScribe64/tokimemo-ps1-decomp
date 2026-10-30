"""T-39025: English linter rules on the synthetic fixtures (no game data)."""
import copy
import json
from pathlib import Path
import tempfile
import unittest

from tools.port import english_lint as lint

FIXTURES = Path(__file__).resolve().parent / "tests" / "fixtures" / "english_lint"


def receipt(name):
    return json.loads((FIXTURES / name).read_text())


def approvals():
    return lint.load_approvals(FIXTURES / "approvals.json")


def rules(result):
    return sorted(f["rule"] for f in result["findings"])


class EnglishLintTests(unittest.TestCase):
    def test_clean_receipt_has_no_findings_and_full_coverage(self):
        approvals = lint.load_approvals(FIXTURES / "approvals.json")
        result = lint.lint_receipt(lint.draw_receipt(receipt("clean.json"), "clean"), approvals)
        self.assertEqual(result["findings"], [])
        # One English run and one declared Japanese fallback: the fallback is not English.
        self.assertEqual(result["english_text_percent"], 50.0)
        self.assertEqual(result["declared_fallback_text_percent"], 50.0)
        self.assertEqual(result["english_art_percent"], 100.0)
        self.assertEqual(result["art_states"]["approved-used"], 1)

    def test_all_fallback_run_is_not_english(self):
        data = receipt("clean.json")
        data["text_runs"] = [data["text_runs"][1]]
        result = lint.lint_receipt(lint.draw_receipt(data, "x"), lint.load_approvals(FIXTURES / "approvals.json"))
        self.assertEqual((result["english_text_percent"], result["declared_fallback_text_percent"]), (0.0, 100.0))

    def test_unverified_approval_is_not_english_art(self):
        result = lint.lint_receipt(lint.draw_receipt(receipt("clean.json"), "clean"))
        self.assertEqual(result["art_states"]["approved-unverified"], 1)
        self.assertEqual(result["art_states"]["approved-used"], 0)
        self.assertEqual(result["english_art_percent"], 0.0)
        self.assertEqual(rules(result), ["art_approval_unverified"])

    def test_self_reported_art_fields_are_required(self):
        approved = {"status": "approved", "output_sha256": "b" * 64}
        for image in ({"asset_id": "a", "has_text": True, "replacement": {"status": "approved"}},
                      {"asset_id": "a", "has_text": True, "replacement": approved},
                      {"asset_id": "a", "has_text": True, "drawn_sha256": "B" * 64, "replacement": approved},
                      {"asset_id": "a", "has_text": True, "drawn_sha256": "b" * 64,
                       "replacement": {"status": "approved", "output_sha256": "short"}}):
            for approvals in (None, {"b" * 64}):
                with self.assertRaises(lint.LintError):
                    lint.art_state(image, approvals)
        for has_text in (None, "yes", 1):
            data = receipt("clean.json")
            if has_text is None:
                del data["images"][0]["has_text"]
            else:
                data["images"][0]["has_text"] = has_text
            with self.assertRaises(lint.LintError):
                lint.lint_receipt(lint.draw_receipt(data, "x"))

    def test_every_rule_fires_on_the_dirty_receipt(self):
        result = lint.lint_receipt(lint.draw_receipt(receipt("dirty.json"), "dirty"))
        self.assertEqual(rules(result), sorted([
            "untranslated_cp932", "overflow_source_box", "clipped_by_window", "line_overflow",
            "page_overflow", "missing_glyph", "font_fallback", "art_approval_unverified", "art_draft", "art_missing"]))
        verified = lint.lint_receipt(lint.draw_receipt(receipt("dirty.json"), "dirty"),
                                     lint.load_approvals(FIXTURES / "approvals.json"))
        self.assertIn("art_approved_unused", rules(verified))
        self.assertNotIn("art_approval_unverified", rules(verified))
        untranslated = [f for f in result["findings"] if f["rule"] == "untranslated_cp932"]
        self.assertEqual(untranslated[0]["unit_id"], "unit:jp")
        self.assertEqual(untranslated[0]["code_points"], ["U+3042", "U+3044"])
        self.assertEqual(result["english_text_percent"], 80.0)
        self.assertEqual(result["english_art_percent"], 0.0)
        self.assertTrue(all(f["objective"] for f in result["findings"]))

    def test_declared_fallback_only_exempts_its_own_unit(self):
        data = receipt("clean.json")
        data["declared_fallbacks"] = []
        self.assertEqual(rules(lint.lint_receipt(lint.draw_receipt(data, "x"), approvals())), ["untranslated_cp932"])

    def test_full_width_latin_is_not_japanese(self):
        data = receipt("clean.json")
        data["text_runs"][0] = dict(data["text_runs"][0], text=None, cp932_hex="826082818250")
        del data["text_runs"][0]["text"]
        self.assertEqual(lint.lint_receipt(lint.draw_receipt(data, "x"), approvals())["findings"], [])
        for char in "ア一ｱ":
            self.assertTrue(lint.is_japanese(char))

    def test_draft_never_counts_as_approved(self):
        approvals = lint.load_approvals(FIXTURES / "approvals.json")
        data = receipt("clean.json")
        result = lint.lint_receipt(lint.draw_receipt(data, "x"), approvals)
        self.assertEqual(result["art_states"]["approved-used"], 1)
        # A receipt that claims approval for a hash without an exact approve event is a draft.
        data["images"][0]["replacement"]["output_sha256"] = "e" * 64
        data["images"][0]["drawn_sha256"] = "e" * 64
        result = lint.lint_receipt(lint.draw_receipt(data, "x"), approvals)
        self.assertEqual(result["art_states"]["draft"], 1)
        self.assertEqual(rules(result), ["art_draft", "art_unverified_approval"])
        for status in ("draft", "agent-reviewed-in-game", "pending"):
            image = {"asset_id": "a", "has_text": True, "drawn_sha256": "f" * 64,
                     "replacement": {"status": status, "output_sha256": "f" * 64}}
            self.assertEqual(lint.art_state(image, None)[0], "draft")

    def test_malformed_receipts_are_rejected(self):
        with self.assertRaises(lint.LintError):
            lint.draw_receipt({"schema": "other"}, "x")
        data = receipt("clean.json"); data["language"] = "ja"
        with self.assertRaises(lint.LintError):
            lint.draw_receipt(data, "x")
        for mutate in (lambda d: d["text_runs"][0].pop("source_box"),
                       lambda d: d["text_runs"][0].update(rect=[0, 0, -1, 4]),
                       lambda d: d["text_runs"][0].update(lines="2"),
                       lambda d: d["text_runs"][0].pop("max_lines"),
                       lambda d: d["text_runs"][1].update(cp932_hex="zz"),
                       lambda d: d["text_runs"][0].pop("unit_id"),
                       lambda d: d["images"][0].update(replacement="approved")):
            data = receipt("clean.json"); mutate(data)
            with self.assertRaises(lint.LintError):
                lint.lint_receipt(lint.draw_receipt(data, "x"))
        with self.assertRaises(lint.LintError), tempfile.TemporaryDirectory() as tmp:
            bad = Path(tmp) / "a.json"; bad.write_text(json.dumps({"schema": "x"}))
            lint.load_approvals(bad)

    def test_report_counts_and_cli_exit_codes(self):
        report = lint.lint([FIXTURES / "clean.json", FIXTURES / "dirty.json"])
        self.assertFalse(report["passed"])
        self.assertEqual(report["finding_counts"]["untranslated_cp932"], 1)
        self.assertEqual(len(report["inputs"]), 2)
        # Without --approvals the clean receipt's art approval is unverified, so it does not pass.
        self.assertEqual(lint.main([str(FIXTURES / "clean.json")]), 1)
        self.assertEqual(lint.main([str(FIXTURES / "clean.json"), "--approvals", str(FIXTURES / "approvals.json")]), 0)
        self.assertEqual(lint.main([str(FIXTURES / "clean.json"), str(FIXTURES / "dirty.json"),
                                    "--approvals", str(FIXTURES / "approvals.json")]), 1)
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "r.json"
            self.assertEqual(lint.main([str(FIXTURES / "clean.json"), "--approvals", str(FIXTURES / "approvals.json"),
                                        "--output", str(out)]), 0)
            self.assertTrue(json.loads(out.read_text())["approvals_verified"])
            self.assertEqual(lint.main([str(FIXTURES / "clean.json"), "--output", str(out)]), 2)

    def test_fixture_is_not_mutated_by_lint(self):
        data = receipt("dirty.json"); before = copy.deepcopy(data)
        lint.lint_receipt(lint.draw_receipt(data, "x"))
        self.assertEqual(data, before)


if __name__ == "__main__":
    unittest.main()
