"""Synthetic receipts for tools/port/compare_route_modes.py (FAST vs real-time
route equivalence); not evidence of game parity.
Run in Docker: python3 -m unittest tools.port.test_compare_route_modes"""
import json
from pathlib import Path
import tempfile
import unittest

from tools.port.compare_route_modes import compare


def write_run(root, mode, frames=6, rei_tick=2, blocks_per_frame=1, image="aa", state=None, extra_png=None, fifo=0):
    root.mkdir(parents=True)
    pcm = 7
    lines = ["tick\tframe_count\tkey\timage_sha256\tpcm_blocks\twall_ms"]
    blocks = ["index\tsource\tlast_tick\tpcm_sha256"] + ["%d\tprebuffer\t-1\tp%d" % (i, i) for i in range(7)]
    for tick in range(frames):
        if tick > rei_tick:
            for _ in range(blocks_per_frame):
                blocks.append("%d\tpush\t%d\tr%d" % (pcm, tick - 1, pcm - 7))
                pcm += 1
        key = "15/%d/0/0" % (tick // 3)
        lines.append("%d\t%d\t%s\t%s%d\t%d\t%d" % (tick, 100 + tick, key, image, tick, pcm, 1000 + tick * (5 if mode == "fast" else 50)))
    (root / "frames.tsv").write_text("\n".join(lines) + "\n")
    (root / "audio-blocks.tsv").write_text("\n".join(blocks) + "\n")
    (root / "route-speed.json").write_text(json.dumps({"mode": mode, "published_frames": frames, "wall_seconds": 1.0 if mode == "fast" else 10.0,
                                                       "frames_per_second": 60.0 if mode == "fast" else 6.0}))
    (root / "rei-proof.json").write_text(json.dumps({"checks": 3, "failed": False, "states": {"stage8-0": {"frame": 1}}, "final": {"x": 1}, "entry_image": {},
                                                     "original_audio": {"race_clock": {"rei_tick": rei_tick, "max_fifo": fifo}, "sink": {"skips": 0},
                                                                        "maximum_pump_gap_us": 123 if mode == "fast" else 99999}}))
    (root / "states").mkdir()
    (root / "states" / "34-1-0-0.json").write_text(json.dumps(state or {"state": {"frame_count": 105}, "rng": {"state": 7}}))
    (root / "stage8-0.png").write_bytes(b"native-png")
    (root / "captures").mkdir()
    (root / "captures" / "15-12-0-0.png").write_bytes(b"capture")
    (root / "board.native.png").write_bytes(b"native")
    if extra_png:
        (root / extra_png).write_bytes(b"window")


class CompareRouteModesTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def test_identical_runs_pass_and_window_capture_is_listed_only(self):
        write_run(self.root / "rt", "realtime", extra_png="board.png")
        write_run(self.root / "fast", "fast")
        report = compare(self.root / "rt", self.root / "fast", audio="require-race")
        self.assertTrue(report["identical"], report["diverged"])
        self.assertEqual(report["checks"]["pngs"]["window_captures_not_compared"], ["board.png"])
        self.assertEqual(report["checks"]["audio_race_blocks"]["compared_blocks"], 3)
        self.assertAlmostEqual(report["speed"]["b_over_a"], 10.0)

    def test_frame_image_divergence_fails(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast", image="bb")
        report = compare(self.root / "rt", self.root / "fast")
        self.assertIn("frames", report["diverged"])
        self.assertEqual(report["checks"]["frames"]["first_difference"]["row"], 0)

    def test_state_divergence_fails(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast", state={"state": {"frame_count": 106}, "rng": {"state": 7}})
        self.assertIn("states", compare(self.root / "rt", self.root / "fast")["diverged"])

    def test_fast_pacing_requires_one_block_per_frame(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast", blocks_per_frame=2)
        report = compare(self.root / "rt", self.root / "fast")
        self.assertIn("fast_pacing:" + str(self.root / "fast"), report["diverged"])

    def test_fast_fifo_fails(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast", fifo=2)
        self.assertFalse(compare(self.root / "rt", self.root / "fast")["identical"])

    def _receipt_to_playtest(self, run, audio):
        (run / "rei-proof.json").unlink()
        (run / "playtest.json").write_text(json.dumps({"reason": "limit", "final_frame": 9, **({"original_audio": audio} if audio is not None else {})}))

    def test_fast_without_audio_receipt_fails(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast")
        for run in ("rt", "fast"):
            self._receipt_to_playtest(self.root / run, None)
        report = compare(self.root / "rt", self.root / "fast")
        self.assertIn("fast_pacing:" + str(self.root / "fast"), report["diverged"])
        self.assertIsNone(report["fast_pacing"][str(self.root / "fast")]["receipt"])

    def test_fast_playtest_receipt_is_checked_and_its_audio_is_not_compared(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast", blocks_per_frame=2)
        self._receipt_to_playtest(self.root / "rt", {"race_clock": {"rei_tick": 2, "max_fifo": 9}, "sink": {"skips": 40}})
        self._receipt_to_playtest(self.root / "fast", {"race_clock": {"rei_tick": 2, "max_fifo": 1}, "sink": {"skips": 0}})
        report = compare(self.root / "rt", self.root / "fast")
        self.assertTrue(report["checks"]["playtest.json"]["identical"])
        self.assertEqual(report["fast_pacing"][str(self.root / "fast")]["receipt"], "playtest.json")
        self.assertIn("fast_pacing:" + str(self.root / "fast"), report["diverged"])

    def test_fast_missing_skip_count_fails(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast")
        self._receipt_to_playtest(self.root / "rt", {})
        self._receipt_to_playtest(self.root / "fast", {"race_clock": {"rei_tick": 2, "max_fifo": 1}})
        self.assertFalse(compare(self.root / "rt", self.root / "fast")["identical"])

    def test_fast_early_stop_reads_rei_failure_audio_clock(self):
        write_run(self.root / "fast-a", "fast")
        write_run(self.root / "fast-b", "fast")
        for run, skips in (("fast-a", 0), ("fast-b", 3)):
            (self.root / run / "rei-proof.json").unlink()
            (self.root / run / "rei-failure.json").write_text(json.dumps({"audio_clock": {"race_clock": {"rei_tick": 2, "max_fifo": 1}, "sink": {"skips": skips}}}))
        report = compare(self.root / "fast-a", self.root / "fast-b")
        self.assertEqual(report["diverged"], ["fast_pacing:" + str(self.root / "fast-b")])
        self.assertEqual(report["fast_pacing"][str(self.root / "fast-a")]["receipt"], "rei-failure.json")

    def test_fast_route_ending_before_rei_needs_no_race_clock(self):
        write_run(self.root / "fast-a", "fast")
        write_run(self.root / "fast-b", "fast")
        for run in ("fast-a", "fast-b"):
            (self.root / run / "rei-proof.json").unlink()
            (self.root / run / "rei-failure.json").write_text(json.dumps({"audio_clock": {"race_clock": {}, "sink": {"skips": 0}}}))
        report = compare(self.root / "fast-a", self.root / "fast-b")
        self.assertTrue(report["identical"], report["diverged"])
        self.assertTrue(report["fast_pacing"][str(self.root / "fast-a")]["ended_before_rei"])

    def test_wall_clock_audio_diagnostics_are_ignored_but_substeps_are_not(self):
        write_run(self.root / "rt", "realtime")
        write_run(self.root / "fast", "fast")
        rows = (self.root / "fast" / "frames.tsv").read_text().splitlines()
        cols = rows[3].split("\t")
        cols[2] = "15/9/0/0"
        rows[3] = "\t".join(cols)
        (self.root / "fast" / "frames.tsv").write_text("\n".join(rows) + "\n")
        report = compare(self.root / "rt", self.root / "fast")
        self.assertIn("substep_frames", report["diverged"])
        self.assertTrue(report["checks"]["rei-proof.json"]["identical"])

    def _resumed_run(self, name, start, image="aa"):
        """A FAST run that started from a dev checkpoint at tick `start`: only
        frames and blocks from there on, no states and no earlier PNGs."""
        write_run(self.root / name, "fast", image=image)
        run = self.root / name
        rows = (run / "frames.tsv").read_text().splitlines()
        kept = [rows[0]] + [r for r in rows[1:] if int(r.split("\t")[0]) >= start]
        (run / "frames.tsv").write_text("\n".join(kept) + "\n")
        first_block = int(kept[1].split("\t")[4]) - 1 if int(kept[1].split("\t")[0]) > 2 else 7
        blocks = (run / "audio-blocks.tsv").read_text().splitlines()
        (run / "audio-blocks.tsv").write_text("\n".join([blocks[0]] + [b for b in blocks[1:] if int(b.split("\t")[0]) >= first_block]) + "\n")
        for path in [run / "states" / "34-1-0-0.json", run / "stage8-0.png"]:
            path.unlink()
        (run / "resumed-from.json").write_text(json.dumps({"tick": start, "key": "15/1/0/0"}))
        return run

    def test_resumed_run_is_refused_without_flag(self):
        write_run(self.root / "full", "fast")
        resumed = self._resumed_run("resumed", 4)
        report = compare(self.root / "full", resumed)
        self.assertFalse(report["identical"])
        self.assertEqual(report["diverged"], ["resumed_run"])

    def test_resumed_run_matches_full_run_from_its_tick(self):
        write_run(self.root / "full", "fast")
        resumed = self._resumed_run("resumed", 4)
        report = compare(self.root / "full", resumed, audio="require-all", resumed=True)
        self.assertTrue(report["identical"], report["diverged"])
        self.assertEqual(report["checks"]["frames"]["rows"], 2)
        self.assertEqual(report["resumed"]["tick"], 4)

    def test_resumed_run_divergence_fails(self):
        write_run(self.root / "full", "fast")
        resumed = self._resumed_run("resumed", 4, image="bb")
        report = compare(self.root / "full", resumed, resumed=True)
        self.assertIn("frames", report["diverged"])

    def test_resumed_flag_needs_exactly_one_resumed_run(self):
        write_run(self.root / "a", "fast")
        write_run(self.root / "b", "fast")
        report = compare(self.root / "a", self.root / "b", resumed=True)
        self.assertEqual(report["diverged"], ["resumed_run"])


if __name__ == "__main__":
    unittest.main()
