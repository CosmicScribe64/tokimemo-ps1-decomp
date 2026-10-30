"""T-39028: translation pack and artwork lint on synthetic trees (no game data)."""
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

from tools.port import lint_translation_assets as lint


def sha(data):
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode()


def png(path, size=(4, 2), color=(1, 2, 3)):
    path.parent.mkdir(parents=True, exist_ok=True)
    Image.new("RGB", size, color).save(path)
    return sha(path.read_bytes())


class Tree:
    """A minimal translations/en tree with one approved and one draft image."""

    def __init__(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name) / "en"
        self.art = self.root / "graphics" / "artwork"
        self.main = [{"id": "talk:a", "target_text": "Hello[[T001:row]]there.", "protected_tokens": [{"marker": "[[T001:row]]"}]},
                     {"id": "talk:b", "target_text": "", "protected_tokens": []}]
        self.extra = [{"id": "supplemental:c", "translation": "Bye.", "protected_tokens": "[]"}]
        self.publication = "user_style_approved; runtime_not_integrated"
        self.approve_draft = False
        self.ledger_assets = None

    def write(self):
        (self.root / "graphics").mkdir(parents=True, exist_ok=True)
        (self.root / "returned-v3.jsonl").write_text("".join(json.dumps(r) + "\n" for r in self.main))
        (self.root / "supplemental.jsonl").write_text("".join(json.dumps(r) + "\n" for r in self.extra))
        nonempty = sum(1 for r in self.main if r.get("target_text"))
        (self.root / "manifest.json").write_text(json.dumps(
            {"main_rows": len(self.main), "supplemental_rows": len(self.extra), "main_nonempty_targets": nonempty}))
        total = len(self.main) + len(self.extra)
        (self.root / "graphics" / "inventory_decisions.json").write_text(json.dumps({"text_approval": {
            "approver": "user", "date": "2026-10-29", "instruction": "synthetic", "source": "test",
            "scope": f"every record in returned-v3.jsonl and supplemental.jsonl ({total} records)"}}))
        approved_hash = png(self.art / "batches" / "b1" / "approved.png")
        draft_hash = png(self.art / "batches" / "b2" / "draft.png", color=(9, 9, 9))
        assets = [("approved", "batches/b1/approved.png", approved_hash, self.publication),
                  ("draft", "batches/b2/draft.png", draft_hash, "draft_pending_review")]
        versions, events = [], []
        for name, output, digest, status in assets:
            source = sha(name.encode())
            versions.append({"batch_id": name, "current": True, "publication_status": status, "assets": [
                {"id": name, "output": output, "output_sha256": digest, "dimensions": [4, 2], "source_sha256": source}]})
            if name == "approved" or self.approve_draft:
                revision = sha(encoded(["draft:" + name, source, digest]))
                events.append({"asset_id": "draft:" + name, "category": "style", "decision": "approve", "event_id": name,
                               "revision": revision, "source_sha256": source, "output_sha256": digest})
        history = "".join(json.dumps(e) + "\n" for e in events).encode()
        (self.art / "style-decisions.jsonl").write_bytes(history)
        (self.art / "manifest.json").write_text(json.dumps({"schema": "tokimemo.translated-artwork-snapshot.v1",
                                                            "review_history_sha256": sha(history), "versions": versions}))
        checkpoint = self.art / "checkpoints" / "cp1"
        frame = png(checkpoint / "drafts" / "frame.png", color=(5, 5, 5))
        assets = self.ledger_assets if self.ledger_assets is not None else [
            {"id": "frame", "path": "drafts/frame.png", "output_sha256": frame, "status": "draft", "human_approved": False}]
        (checkpoint / "ledger.json").write_text(json.dumps({"schema": "tokimemo.translation-checkpoint.v1", "assets": assets}))
        return self.root

    def close(self):
        self.tmp.cleanup()


class LintTranslationAssetsTests(unittest.TestCase):
    def setUp(self):
        self.tree = Tree()

    def tearDown(self):
        self.tree.close()

    def failures(self, section):
        return lint.lint(self.tree.write())[section]["failures"]

    def test_clean_tree_passes(self):
        result = lint.lint(self.tree.write())
        self.assertTrue(result["passed"], result)
        self.assertEqual(result["artwork"]["approved_exact"], 1)

    def test_draft_marked_approved_fails(self):
        self.tree.publication = "user_style_approved"
        self.tree.write()
        # Remove the approve event: the approved claim is no longer backed.
        history = self.tree.art / "style-decisions.jsonl"
        history.write_text("")
        manifest = json.loads((self.tree.art / "manifest.json").read_text())
        manifest["review_history_sha256"] = sha(b"")
        (self.tree.art / "manifest.json").write_text(json.dumps(manifest))
        failures = lint.lint(self.tree.root)["artwork"]["failures"]
        self.assertTrue(any("draft marked approved" in f for f in failures), failures)

    def test_pending_status_is_not_an_approval_claim(self):
        self.tree.publication = "user_style_review_pending"
        self.assertEqual(self.failures("artwork"), [])

    def test_ledger_draft_with_human_approval_fails(self):
        for status in ("draft", "agent-reviewed-in-game"):
            self.tree.ledger_assets = [{"id": "frame", "path": "drafts/frame.png", "status": status, "human_approved": True}]
            self.assertTrue(any("draft marked approved" in f for f in self.failures("artwork")))
        self.tree.ledger_assets = [{"id": "frame", "path": "drafts/frame.png", "status": "draft"}]
        self.assertTrue(any("human_approved must be false" in f for f in self.failures("artwork")))

    def test_images_without_review_record_fail(self):
        self.tree.write()
        png(self.tree.art / "batches" / "b3" / "orphan.png")
        png(self.tree.art / "checkpoints" / "cp1" / "drafts" / "unlisted.png")
        failures = lint.lint(self.tree.root)["artwork"]["failures"]
        self.assertTrue(any("b3/orphan.png" in f and "no manifest" in f for f in failures), failures)
        self.assertTrue(any("unlisted.png" in f and "ledger or review record" in f for f in failures), failures)

    def test_stray_image_anywhere_in_pack_fails(self):
        self.tree.write()
        for stray in (self.tree.art / "extra" / "unreviewed.png", self.tree.root / "graphics" / "loose.png",
                      self.tree.root / "dialogue" / "deep" / "x.png"):
            png(stray)
        failures = lint.lint(self.tree.root)["artwork"]["failures"]
        for name in ("graphics/artwork/extra/unreviewed.png", "graphics/loose.png", "dialogue/deep/x.png"):
            self.assertTrue(any(f.startswith(name + ":") and "no manifest, ledger or review record" in f
                                for f in failures), (name, failures))

    def test_ledger_flip_to_approved_without_event_fails(self):
        self.tree.write()
        ledger_path = self.tree.art / "checkpoints" / "cp1" / "ledger.json"
        ledger = json.loads(ledger_path.read_text())
        ledger["assets"][0].update(status="approved", human_approved=True)
        ledger_path.write_text(json.dumps(ledger))
        failures = lint.lint(self.tree.root)["artwork"]["failures"]
        self.assertTrue(any("frame.png" in f and "draft marked approved" in f for f in failures), failures)
        # The status flip alone (human_approved still false) is also an unbacked claim.
        ledger["assets"][0].update(status="approved", human_approved=False)
        ledger_path.write_text(json.dumps(ledger))
        self.assertTrue(any("draft marked approved" in f for f in lint.lint(self.tree.root)["artwork"]["failures"]))

    def write_history(self, extra_events):
        history = self.tree.art / "style-decisions.jsonl"
        data = history.read_bytes() + "".join(json.dumps(e) + "\n" for e in extra_events).encode()
        history.write_bytes(data)
        manifest = json.loads((self.tree.art / "manifest.json").read_text())
        manifest["review_history_sha256"] = sha(data)
        (self.tree.art / "manifest.json").write_text(json.dumps(manifest))

    def test_ledger_approval_with_hash_bound_event_passes(self):
        self.tree.write()
        checkpoint = self.tree.art / "checkpoints" / "cp1"
        frame = sha((checkpoint / "drafts" / "frame.png").read_bytes())
        extra_a = png(checkpoint / "multi" / "a.png", color=(7, 0, 0))
        extra_b = png(checkpoint / "multi" / "b.png", color=(0, 7, 0))
        ledger = {"assets": [
            {"id": "frame", "path": "drafts/frame.png", "output_sha256": frame, "status": "approved", "human_approved": True},
            {"id": "multi", "paths": ["multi/a.png", "multi/b.png"], "status": "approved", "human_approved": True}]}
        (checkpoint / "ledger.json").write_text(json.dumps(ledger))
        self.write_history([{"asset_id": "checkpoint:" + h[:8], "category": "style", "decision": "approve", "event_id": h[:8], "output_sha256": h}
                            for h in (frame, extra_a)])
        failures = lint.lint(self.tree.root)["artwork"]["failures"]
        # b.png has no event: only it fails.
        self.assertEqual([f for f in failures if "draft marked approved" in f],
                         [f for f in failures if "multi/b.png" in f], failures)
        self.assertTrue(any("multi/b.png" in f for f in failures), failures)
        self.write_history([{"asset_id": "checkpoint:b", "category": "style", "decision": "approve", "event_id": "b", "output_sha256": extra_b}])
        self.assertEqual(lint.lint(self.tree.root)["artwork"]["failures"], [])
        # A later non-approve decision on the same hash withdraws the approval.
        self.write_history([{"asset_id": "checkpoint:b", "category": "style", "decision": "reject", "event_id": "b2", "output_sha256": extra_b}])
        self.assertTrue(any("multi/b.png" in f and "draft marked approved" in f
                            for f in lint.lint(self.tree.root)["artwork"]["failures"]))

    def test_ledger_hash_and_missing_image_fail(self):
        self.tree.ledger_assets = [{"id": "frame", "path": "drafts/frame.png", "output_sha256": "0" * 64, "status": "draft", "human_approved": False},
                                   {"id": "gone", "path": "drafts/gone.png", "status": "draft", "human_approved": False}]
        failures = self.failures("artwork")
        self.assertTrue(any("differ from the ledger hash" in f for f in failures))
        self.assertTrue(any("missing image" in f for f in failures))

    def test_tampered_artwork_and_unknown_approval_fail(self):
        self.tree.write()
        png(self.tree.art / "batches" / "b1" / "approved.png", color=(200, 0, 0))
        self.assertTrue(any("snapshot audit" in f for f in lint.lint(self.tree.root)["artwork"]["failures"]))
        self.tree.write()
        history = self.tree.art / "style-decisions.jsonl"
        data = history.read_bytes() + (json.dumps({"decision": "approve", "event_id": "x", "output_sha256": "f" * 64}) + "\n").encode()
        history.write_bytes(data)
        manifest = json.loads((self.tree.art / "manifest.json").read_text())
        manifest["review_history_sha256"] = sha(data)
        (self.tree.art / "manifest.json").write_text(json.dumps(manifest))
        self.assertTrue(any("does not know" in f for f in lint.lint(self.tree.root)["artwork"]["failures"]))

    def test_text_rules(self):
        self.tree.main.append(dict(self.tree.main[0]))
        self.assertTrue(any("duplicate record id" in f for f in self.failures("text")))
        self.tree.main.pop()
        self.tree.main[0] = dict(self.tree.main[0], protected_tokens=[])
        self.assertTrue(any("differ from protected tokens" in f for f in self.failures("text")))
        self.tree.main[0] = dict(self.tree.main[0], protected_tokens=[{"marker": "[[T001:row]]"}],
                                 target_text="Hello there.")
        self.assertTrue(any("talk:a" in f and "differ from protected tokens" in f for f in self.failures("text")))
        self.tree.main[0] = dict(self.tree.main[0], target_text="Hello[[T001:row]]there.")
        root = self.tree.write()
        (root / "manifest.json").write_text(json.dumps({"main_rows": 9, "supplemental_rows": 1, "main_nonempty_targets": 1}))
        self.assertTrue(any("manifest main_rows" in f for f in lint.lint(root)["text"]["failures"]))
        root = self.tree.write()
        decisions = json.loads((root / "graphics" / "inventory_decisions.json").read_text())
        decisions["text_approval"]["scope"] = decisions["text_approval"]["scope"].replace("(3 records", "(4 records")
        (root / "graphics" / "inventory_decisions.json").write_text(json.dumps(decisions))
        self.assertTrue(any("text approval" in f for f in lint.lint(root)["text"]["failures"]))
        (root / "returned-v3.jsonl").write_text("{not json\n")
        with self.assertRaises(lint.LintFailure):
            lint.lint(root)

    def test_unrepresentable_characters_are_report_only(self):
        self.tree.extra[0]["translation"] = "Café"
        result = lint.lint(self.tree.write())
        self.assertTrue(result["passed"])
        self.assertEqual(result["text"]["report_only"]["characters_without_original_code_unit"], {"U+00E9": 1})

    def test_cli_exit_codes(self):
        self.assertEqual(lint.main(["--root", str(self.tree.write())]), 0)
        png(self.tree.art / "batches" / "b9" / "orphan.png")
        self.assertEqual(lint.main(["--root", str(self.tree.root)]), 1)


if __name__ == "__main__":
    unittest.main()
