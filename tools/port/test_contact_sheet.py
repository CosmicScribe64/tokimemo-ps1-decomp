"""T-39026: contact sheets and the visual review queue on synthetic images only."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

from tools.port import contact_sheet as sheet
from tools.port import schema_check
from tools.port.progress import review_queue as rq

SCHEMA = json.loads((Path(__file__).resolve().parent / "schemas" / "visual-review.v1.schema.json").read_text())


def image(path, size=(320, 240), color=(8, 16, 24), pixels=None):
    picture = Image.new("RGB", size, color)
    for xy, value in (pixels or {}).items():
        picture.putpixel(xy, value)
    picture.save(path)


class Workspace:
    def __init__(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.cache = self.root / ".asset-cache"
        self.inputs = self.root / "inputs"
        self.inputs.mkdir()

    def spec(self, rows):
        path = self.inputs / "spec.json"
        path.write_text(json.dumps({"schema": sheet.SPEC_SCHEMA, "rows": rows}))
        return path

    def close(self):
        self.tmp.cleanup()


def row(key="stage8-1", **extra):
    base = {"key": key, "sync_status": "known_diff", "defect_id": "D1", "native": "native.png",
            "port_ja": "ja.png", "port_en": "en.png", "port_wide": "wide.png",
            "native_encoding": "bit_replicated", "port_encoding": "zero_extended", "severity": 2, "kind": "subjective"}
    base.update(extra)
    return base


class ContactSheetTests(unittest.TestCase):
    def setUp(self):
        self.ws = Workspace()
        # Native uses bit replication for code 1 (8|0 = 8) and code 31 (248|7 = 255).
        image(self.ws.inputs / "native.png", color=(8, 8, 8), pixels={(1, 1): (255, 255, 255), (2, 2): (16, 16, 16)})
        image(self.ws.inputs / "ja.png", color=(8, 8, 8), pixels={(1, 1): (248, 248, 248), (2, 2): (24, 16, 16)})
        image(self.ws.inputs / "en.png", color=(8, 8, 8))
        image(self.ws.inputs / "wide.png", size=(1280, 720))

    def tearDown(self):
        self.ws.close()

    def test_row_layout_scale_metrics_and_hashes(self):
        result = sheet.build(self.ws.spec([row(findings=[{"cell": "port_en", "rect": [10, 10, 50, 20], "rule": "overflow_source_box"}])]),
                             self.ws.cache / "sheet-1")
        record = result["rows"][0]
        self.assertEqual(record["metrics"]["exact_changed_pixels"], 2)
        # (255) vs (248) is the same 5-bit code under the declared encodings; (16) vs (24) is not.
        self.assertEqual(record["metrics"]["rgb555_residual"], 1)
        self.assertEqual(record["metrics"]["rgb555_out_of_encoding"], 0)
        self.assertEqual(set(record["input_sha256"]), {"native", "port_ja", "port_en", "port_wide"})
        png = self.ws.cache / "sheet-1" / record["image"]
        self.assertEqual(sheet.sha256_file(png), record["image_sha256"])
        with Image.open(png) as rendered:
            self.assertEqual(rendered.height, sheet.HEADER_HEIGHT + 480 + sheet.LABEL_HEIGHT)
            self.assertEqual(rendered.width, 640 * 5 + 853 + sheet.GAP * 5)
            # 2x nearest: native (1,1) white covers (2..3, 2..3) of the first cell.
            self.assertEqual(rendered.getpixel((2, sheet.HEADER_HEIGHT + 3)), (255, 255, 255))
            # Finding box outline on port_en at 2x.
            left = 2 * (640 + sheet.GAP) + 20
            self.assertEqual(rendered.getpixel((left, sheet.HEADER_HEIGHT + 20)), (255, 230, 0))

    def test_out_of_encoding_is_marked_not_dropped(self):
        image(self.ws.inputs / "ja.png", color=(8, 8, 8), pixels={(0, 0): (3, 8, 8)})
        record = sheet.build(self.ws.spec([row()]), self.ws.cache / "s")["rows"][0]
        self.assertEqual(record["metrics"]["rgb555_out_of_encoding"], 1)

    def test_missing_and_mismatched_inputs_are_unavailable(self):
        image(self.ws.inputs / "ja.png", size=(320, 224))
        record = sheet.build(self.ws.spec([row(port_en=None, port_wide="absent.png")]), self.ws.cache / "s")["rows"][0]
        self.assertIn("port_ja", record["unavailable"])
        self.assertIn("port_en", record["unavailable"])
        self.assertIn("port_wide", record["unavailable"])
        self.assertEqual(record["metrics"], {})

    def test_rejections(self):
        spec = self.ws.spec([row()])
        with self.assertRaises(sheet.SheetError):
            sheet.build(spec, self.ws.root / "outside")
        (self.ws.cache / "exists").mkdir(parents=True)
        with self.assertRaises(sheet.SheetError):
            sheet.build(spec, self.ws.cache / "exists")
        for bad in (row(native_encoding="guess"), row(sync_status="fine"), row(key="../x"), row(severity=9), row(kind="vibes"),
                    row(kind="objective"), row(findings="overflow"),
                    row(native="../native.png"), row(findings=[{"cell": "exact_diff", "rect": [0, 0, 1, 1]}])):
            with self.assertRaises(sheet.SheetError):
                sheet.build(self.ws.spec([bad]), self.ws.cache / "bad")
        with self.assertRaises(sheet.SheetError):
            sheet.build(self.ws.spec([row(), row()]), self.ws.cache / "dupe")


class ReviewQueueTests(unittest.TestCase):
    def setUp(self):
        self.ws = Workspace()
        for name in ("native.png", "ja.png", "en.png"):
            image(self.ws.inputs / name)
        rows = [row("low", severity=0, port_wide=None), row("high", severity=3, port_wide=None),
                row("objective-1", severity=2, kind=None, port_wide=None,
                    findings=[{"cell": "port_en", "rect": [1, 1, 4, 4], "rule": "overflow_source_box"}])]
        self.sheet = sheet.build(self.ws.spec(rows), self.ws.cache / "sheet")
        self.sheet_path = self.ws.cache / "sheet" / "sheet.json"
        self.queue_path = self.ws.cache / "queue.json"

    def tearDown(self):
        self.ws.close()

    def _hash(self, queue, item):
        return rq._item(queue, item)["image_sha256"]

    def test_cap_and_severity_priority(self):
        queue = rq.load(self.queue_path)
        result = rq.enqueue(queue, self.sheet_path, cap=2)
        self.assertEqual(result["enqueued"], ["high", "objective-1"])
        self.assertEqual([d["item_id"] for d in result["deferred"]], ["low"])
        self.assertEqual(rq.enqueue(queue, self.sheet_path, cap=2)["enqueued"], ["low"])
        schema_check.validate(queue, SCHEMA)

    def test_human_verdict_is_bound_to_hash_and_invalidated_by_change(self):
        queue = rq.load(self.queue_path); rq.enqueue(queue, self.sheet_path, cap=10)
        digest = self._hash(queue, "high")
        with self.assertRaises(rq.QueueError):
            rq.human_verdict(queue, "high", "approved", "user", "0" * 64)
        with self.assertRaises(rq.QueueError):
            rq.human_verdict(queue, "high", "pending", "user", digest)
        rq.human_verdict(queue, "high", "approved", "user", digest, "looks right")
        self.assertEqual(rq._item(queue, "high")["closed_by"], "human")
        # Re-render with a different image: the verdict is invalidated.
        image(self.ws.inputs / "ja.png", color=(200, 0, 0))
        rows = [row("high", severity=3, port_wide=None)]
        sheet.build(self.ws.spec(rows), self.ws.cache / "sheet-2")
        rq.enqueue(queue, self.ws.cache / "sheet-2" / "sheet.json", cap=10)
        item = rq._item(queue, "high")
        self.assertEqual(item["state"], "pending")
        self.assertIsNone(item["closed_by"])
        self.assertEqual(item["invalidated"][0]["state"], "approved")
        self.assertEqual(len(item["human_verdicts"]), 1)
        schema_check.validate(queue, SCHEMA)

    def test_image_changed_on_disk_resets_item(self):
        queue = rq.load(self.queue_path); rq.enqueue(queue, self.sheet_path, cap=10)
        digest = self._hash(queue, "low")
        Path(rq._item(queue, "low")["image_path"]).write_bytes(b"changed")
        with self.assertRaises(rq.QueueError):
            rq.human_verdict(queue, "low", "approved", "user", digest)
        self.assertEqual(len(rq._item(queue, "low")["invalidated"]), 1)

    def test_agent_notes_are_separate_and_cannot_close_subjective(self):
        queue = rq.load(self.queue_path); rq.enqueue(queue, self.sheet_path, cap=10)
        digest = self._hash(queue, "high")
        rq.agent_note(queue, "high", "agent-a", digest, "first pass", "approved")
        item = rq._item(queue, "high")
        self.assertEqual(item["state"], "pending")
        self.assertEqual(item["human_verdicts"], [])
        with self.assertRaises(rq.QueueError):
            rq.agent_note(queue, "high", "agent-a", digest, "close", "approved", close=True)
        objective = self._hash(queue, "objective-1")
        with self.assertRaises(rq.QueueError):
            rq.agent_note(queue, "objective-1", "agent-a", objective, "close", None, close=True)
        self.assertEqual(rq._item(queue, "objective-1")["kind"], "objective")
        for state in ("approved", "rejected", "needs-fix", "pending"):
            with self.assertRaises(rq.QueueError):
                rq.agent_note(queue, "objective-1", "agent-a", objective, "close", state, close=True)
        rq.agent_note(queue, "objective-1", "agent-a", objective, "overflow fixed", "resolved-objective", close=True)
        item = rq._item(queue, "objective-1")
        self.assertEqual((item["state"], item["closed_by"]), ("resolved-objective", "agent"))
        # A human cannot use the agent-only state, and status never counts the agent close as approval.
        with self.assertRaises(rq.QueueError):
            rq.human_verdict(queue, "high", "resolved-objective", "user", digest)
        rq.human_verdict(queue, "high", "approved", "user", digest)
        status = rq.status(queue)
        self.assertEqual(status["human_approved"], 1)
        self.assertEqual(status["closed_by_agent"]["resolved-objective"], 1)
        self.assertEqual(status["closed_by_agent"]["approved"], 0)
        self.assertEqual(status["pending"], 1)
        schema_check.validate(queue, SCHEMA)

    def test_objective_kind_needs_findings(self):
        sheet_path = self.ws.cache / "sheet" / "sheet.json"
        manifest = json.loads(sheet_path.read_text())
        manifest["rows"][0]["kind"] = "objective"
        manifest["rows"][0]["findings"] = []
        forged = self.ws.cache / "sheet" / "forged.json"
        forged.write_text(json.dumps(manifest))
        with self.assertRaises(rq.QueueError):
            rq.enqueue(rq.load(self.queue_path), forged, cap=10)

    def test_hash_change_invalidates_even_past_the_cap(self):
        queue = rq.load(self.queue_path); rq.enqueue(queue, self.sheet_path, cap=10)
        old = rq._item(queue, "low")
        old_path, old_hash = old["image_path"], old["image_sha256"]
        rq.human_verdict(queue, "low", "approved", "user", old_hash)
        # New sheet: "low" re-rendered with another image and new findings, plus a new severity-3 row; cap 1.
        image(self.ws.inputs / "ja.png", color=(200, 0, 0))
        rows = [row("low", severity=0, port_wide=None, defect_id="D9", kind=None,
                    findings=[{"cell": "port_en", "rect": [0, 0, 2, 2], "rule": "untranslated_glyph"}]),
                row("new-high", severity=3, port_wide=None)]
        sheet.build(self.ws.spec(rows), self.ws.cache / "sheet-3")
        result = rq.enqueue(queue, self.ws.cache / "sheet-3" / "sheet.json", cap=1)
        self.assertEqual(result["enqueued"], ["new-high"])
        self.assertEqual(result["invalidated"], ["low"])
        self.assertEqual([d["item_id"] for d in result["deferred"]], ["low"])
        item = rq._item(queue, "low")
        self.assertEqual((item["state"], item["closed_by"]), ("pending", None))
        self.assertNotEqual(item["image_path"], old_path)
        self.assertIn("sheet-3", item["image_path"])
        self.assertEqual(item["invalidated"][-1]["state"], "approved")
        self.assertEqual((item["findings"], item["defect_id"], item["kind"]), (["untranslated_glyph"], "D9", "objective"))
        self.assertEqual(rq.status(queue)["human_approved"], 0)
        with self.assertRaises(rq.QueueError):
            rq.human_verdict(queue, "low", "approved", "user", old_hash)
        # Deferred items are not published; the next run surfaces them.
        published = rq.publish(queue, self.ws.cache / "media-cap")
        self.assertNotIn(item["image_sha256"], {entry["sha256"] for entry in published})
        again = rq.enqueue(queue, self.ws.cache / "sheet-3" / "sheet.json", cap=1)
        self.assertEqual((again["enqueued"], again["deferred"]), (["low"], []))
        schema_check.validate(queue, SCHEMA)

    def test_publish_matches_progress_server_manifest(self):
        from tools.port.progress import server
        queue = rq.load(self.queue_path); rq.enqueue(queue, self.sheet_path, cap=10)
        rq.human_verdict(queue, "low", "approved", "user", self._hash(queue, "low"))
        media = self.ws.cache / "media"
        items = rq.publish(queue, media)
        self.assertEqual(len(items), 2)
        served = server.safe_entries(media)
        self.assertEqual({e["sha256"] for e in served}, {self._hash(queue, "high"), self._hash(queue, "objective-1")})
        self.assertEqual(served[0]["sha256"], self._hash(queue, "high"))
        with self.assertRaises(rq.QueueError):
            rq.publish(queue, media)
        with self.assertRaises(rq.QueueError):
            rq.publish(queue, self.ws.root / "outside-media")

    def test_cli_roundtrip_and_queue_outside_cache_refused(self):
        self.assertEqual(rq.main(["--queue", str(self.queue_path), "enqueue", "--sheet", str(self.sheet_path), "--cap", "5"]), 0)
        queue = json.loads(self.queue_path.read_text())
        schema_check.validate(queue, SCHEMA)
        digest = [i for i in queue["items"] if i["item_id"] == "high"][0]["image_sha256"]
        self.assertEqual(rq.main(["--queue", str(self.queue_path), "verdict", "--item", "high", "--state", "needs-art",
                                  "--reviewer", "user", "--image-sha256", digest]), 0)
        self.assertEqual(rq.main(["--queue", str(self.ws.root / "q.json"), "status"]), 2)

    def test_schema_rejects_bad_queue(self):
        queue = rq.empty_queue("q")
        schema_check.validate(queue, SCHEMA)
        queue["items"].append({"item_id": "x"})
        with self.assertRaises(schema_check.SchemaError):
            schema_check.validate(queue, SCHEMA)


if __name__ == "__main__":
    unittest.main()
