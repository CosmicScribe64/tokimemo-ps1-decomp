"""T-39027: frame pacing report on synthetic samples only (no emulator or port runs)."""
import json
from pathlib import Path
import tempfile
import unittest

from tools.port import frame_pacing_report as pacing


def native_rows(plan, start_vsync=1, start_fc=100):
    """plan: list of (stage, step, sub, [vsyncs per iteration...])."""
    rows, vsync, fc = [], start_vsync, start_fc
    # Leading partial iteration (excluded by the report).
    rows.append({"f": vsync, "fc": fc, "mode": 20, "stage": 0, "step": 0, "sub": 0}); vsync += 1; fc += 1
    for stage, step, sub, counts in plan:
        for count in counts:
            for _ in range(count):
                rows.append({"f": vsync, "fc": fc, "mode": 0x15, "stage": stage, "step": step, "sub": sub})
                vsync += 1
            fc += 1
    rows.append({"f": vsync, "fc": fc, "mode": 0x15, "stage": 12, "step": 0, "sub": 0})
    return rows


def probe(plan, intervals=None, events=None, host=None, start_fc=101):
    """Logical ticks follow the plan (one tick per vsync); one host frame per tick."""
    ticks, frames, fc, tick = [], [], start_fc, 500
    ticks.append({"tick": tick, "frame_count": fc, "key": None}); tick += 1; fc += 1
    for stage, step, sub, counts in plan:
        for count in counts:
            for _ in range(count):
                ticks.append({"tick": tick, "frame_count": fc, "key": "%d/%d/%d" % (stage, step, sub)})
                tick += 1
            fc += 1
    ticks.append({"tick": tick, "frame_count": fc, "key": "12/0/0"})
    for index, row in enumerate(ticks):
        frames.append({"frame": index, "frame_count": row["frame_count"], "ticks": 1, "interval_us": 16667,
                       "process_us": 2000, "render_us": 3000})
    for frame, value in (intervals or {}).items():
        frames[frame]["interval_us"] = value
    for frame, kinds in (events or {}).items():
        frames[frame]["events"] = kinds
    return {"schema": pacing.PROBE_SCHEMA, "ticks": ticks, "frames": frames, "host": host or {"display": "xvfb"},
            "launch_to_first_interactive_ms": 1234}


PLAN = [(0, 0, 0, [1]), (0, 1, 1, [1, 1, 2, 1]), (0, 1, 2, [2, 2])]


class FramePacingReportTests(unittest.TestCase):
    def _run(self, native, port, loops=None, until=None):
        with tempfile.TemporaryDirectory() as tmp:
            n = Path(tmp) / "frames.jsonl"; p = Path(tmp) / "probe.json"; l = None
            n.write_text("\n".join(json.dumps(r) for r in native) + "\n")
            p.write_text(json.dumps(port))
            if loops is not None:
                l = Path(tmp) / "loops.jsonl"
                l.write_text("\n".join(json.dumps(r) for r in loops) + "\n")
            return pacing.build_report(n, p, l, until)

    def test_identical_cadence_passes_with_histograms(self):
        report = self._run(native_rows(PLAN), probe(PLAN))
        self.assertTrue(report["cadence"]["passed"], report["cadence"])
        row = {r["key"]: r for r in report["cadence"]["substeps"]}["0/1/1"]
        self.assertEqual(row["native"]["histogram"], {"1": 3, "2": 1})
        self.assertEqual(row["native"]["iterations"], 4)

    def test_iteration_count_difference_fails(self):
        changed = [(0, 0, 0, [1]), (0, 1, 1, [1, 1, 2]), (0, 1, 2, [2, 2])]
        report = self._run(native_rows(PLAN), probe(changed))
        rows = {r["key"]: r["result"] for r in report["cadence"]["substeps"]}
        self.assertEqual(rows["0/1/1"], "iteration_count_differs")
        self.assertFalse(report["cadence"]["passed"])

    def test_same_iterations_different_multiplicity_fails(self):
        changed = [(0, 0, 0, [1]), (0, 1, 1, [1, 1, 1, 1]), (0, 1, 2, [2, 2])]
        report = self._run(native_rows(PLAN), probe(changed))
        rows = {r["key"]: r["result"] for r in report["cadence"]["substeps"]}
        self.assertEqual(rows["0/1/1"], "vsync_multiplicity_differs")
        self.assertFalse(report["cadence"]["passed"])

    def test_missing_substep_and_order_fail(self):
        report = self._run(native_rows(PLAN), probe(PLAN[:2]))
        self.assertIn("0/1/2", report["cadence"]["mismatches"])
        swapped = [PLAN[0], PLAN[2], PLAN[1]]
        report = self._run(native_rows(PLAN), probe(swapped))
        self.assertFalse(report["cadence"]["order_equal"])
        self.assertFalse(report["cadence"]["passed"])

    def test_zero_vsync_catch_up_iterations_are_counted(self):
        # Native: a 2-vsync iteration followed by an iteration with no vsync wait.
        native = native_rows([(0, 0, 0, [1]), (0, 1, 1, [1, 2]), (0, 1, 2, [1, 1])])
        for row in native[5:]:
            row["fc"] += 1
        report = self._run(native, probe([(0, 0, 0, [1]), (0, 1, 1, [1, 1]), (0, 1, 2, [1, 1, 1])]))
        rows = {r["key"]: r for r in report["cadence"]["substeps"]}
        self.assertEqual(rows["0/1/2"]["native"]["histogram"], {"0": 1, "1": 2})
        self.assertEqual(rows["0/1/2"]["result"], "vsync_multiplicity_differs")
        self.assertEqual(rows["0/1/1"]["native"]["histogram"], {"1": 1, "2": 1})
        self.assertEqual(report["native_keying"][:13], "sampled_state")
        # Loop-boundary records key the unsampled iteration as its own substep.
        loops = [{"fc": 100, "k": "20/0/0/0"}, {"fc": 101, "k": "21/0/0/0"}, {"fc": 102, "k": "21/0/1/1"},
                 {"fc": 104, "k": "21/9/9/9"}, {"fc": 105, "k": "21/0/1/2"}, {"fc": 107, "k": "21/12/0/0"}]
        report = self._run(native, probe([(0, 0, 0, [1]), (0, 1, 1, [1, 1]), (9, 9, 9, [1]), (0, 1, 2, [1, 1])]),
                           loops=loops)
        rows = {r["key"]: r for r in report["cadence"]["substeps"]}
        self.assertEqual(rows["9/9/9"]["native"]["histogram"], {"0": 1})
        self.assertEqual(rows["0/1/2"]["result"], "match")
        self.assertEqual(report["native_keying"], "loop_boundary")
        self.assertEqual(report["cadence"]["mismatches"], ["0/1/1", "9/9/9"])

    def test_until_stops_both_sides(self):
        report = self._run(native_rows(PLAN), probe(PLAN[:2]), until="0/1/2")
        self.assertTrue(report["cadence"]["passed"])
        self.assertNotIn("0/1/2", [r["key"] for r in report["cadence"]["substeps"]])

    def test_malformed_loop_records_rejected(self):
        for loops in ([{"fc": 1, "k": "21/0/0"}], [{"fc": 1, "k": "21/0/0/0"}, {"fc": 1, "k": "21/0/1/0"}]):
            with self.assertRaises(pacing.PacingError):
                self._run(native_rows(PLAN), probe(PLAN), loops=loops)

    def test_native_vsync_gap_is_rejected_not_resynchronised(self):
        rows = native_rows(PLAN)
        del rows[3]
        with self.assertRaises(pacing.PacingError):
            self._run(rows, probe(PLAN))

    def test_frame_count_backwards_and_bad_probe_rejected(self):
        bad = probe(PLAN); bad["ticks"][3]["frame_count"] = 50
        with self.assertRaises(pacing.PacingError):
            self._run(native_rows(PLAN), bad)
        bad = probe(PLAN); bad["schema"] = "other"
        with self.assertRaises(pacing.PacingError):
            self._run(native_rows(PLAN), bad)
        bad = probe(PLAN); del bad["ticks"][4]
        with self.assertRaises(pacing.PacingError):
            self._run(native_rows(PLAN), bad)
        bad = probe(PLAN); bad["ticks"] = []
        with self.assertRaises(pacing.PacingError):
            self._run(native_rows(PLAN), bad)

    def test_host_catch_up_does_not_change_cadence(self):
        port = probe(PLAN)
        # Three logical ticks presented in one host frame: still one tick each.
        port["frames"] = port["frames"][:3] + [dict(port["frames"][3], ticks=3)]
        report = self._run(native_rows(PLAN), port)
        self.assertTrue(report["cadence"]["passed"])
        self.assertEqual(report["host_metrics"]["ticks_per_host_frame"], {"1": 3, "3": 1})

    def test_hitches_attributed_to_events_and_xvfb_relative_only(self):
        port = probe(PLAN, intervals={4: 30000, 6: 60000}, events={5: ["mdec"], 6: ["xa"]})
        port["frames"][7]["bgm_underruns"] = 2
        report = self._run(native_rows(PLAN), port)
        host = report["host_metrics"]
        self.assertEqual(host["label"], "relative_only")
        self.assertEqual(host["hitches"]["1.5"]["count"], 2)
        self.assertEqual(host["hitches"]["3.0"]["count"], 1)
        items = {i["frame"]: i["attributed"] for i in host["hitches"]["1.5"]["items"]}
        self.assertEqual(items[4], ["unattributed"])
        self.assertEqual(items[6], ["mdec", "xa"])
        self.assertEqual(host["bgm_underruns"], 2)
        self.assertEqual(host["launch_to_first_interactive_ms"], 1234)

    def test_real_desktop_label_requires_explicit_non_xvfb_host(self):
        report = self._run(native_rows(PLAN), probe(PLAN, host={"display": "macos", "real_desktop": True}))
        self.assertEqual(report["host_metrics"]["label"], "absolute_on_this_host")
        report = self._run(native_rows(PLAN), probe(PLAN, host={"display": "xvfb", "real_desktop": True}))
        self.assertEqual(report["host_metrics"]["label"], "relative_only")

    def test_percentile_nearest_rank(self):
        self.assertEqual(pacing.percentile([1, 2, 3, 4], 0.5), 2)
        self.assertEqual(pacing.percentile(list(range(1, 101)), 0.99), 99)
        self.assertIsNone(pacing.percentile([], 0.5))

    def test_cli_refuses_existing_output_and_returns_failure(self):
        with tempfile.TemporaryDirectory() as tmp:
            n = Path(tmp) / "frames.jsonl"; p = Path(tmp) / "probe.json"; out = Path(tmp) / "r.json"
            n.write_text("\n".join(json.dumps(r) for r in native_rows(PLAN)))
            p.write_text(json.dumps(probe(PLAN[:2])))
            self.assertEqual(pacing.main(["--native", str(n), "--port", str(p), "--output", str(out)]), 1)
            self.assertEqual(pacing.main(["--native", str(n), "--port", str(p), "--output", str(out)]), 2)


if __name__ == "__main__":
    unittest.main()
