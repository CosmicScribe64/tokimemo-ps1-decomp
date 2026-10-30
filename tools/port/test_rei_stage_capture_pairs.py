"""Synthetic tests for tools/port/rei_stage_capture_pairs.py (T-39008). No game data.

Run in Docker: python3 -m unittest tools.port.test_rei_stage_capture_pairs
"""
import hashlib
import json
import os
from pathlib import Path
import tempfile
import unittest

from PIL import Image

try:
    from tools.port import rei_stage_capture_pairs as tool
except ModuleNotFoundError:
    import rei_stage_capture_pairs as tool

VSYNC_PC = 0x800ADABC


def dumps(fc, stage, sub, line=0, counter=0, bri=0, gsi=0, mode=0x15, step=0):
    game = bytearray(32)
    game[4:8] = fc.to_bytes(4, "little")
    game[0x14], game[0x15], game[0x16], game[0x19] = mode, stage, step, sub
    prologue = bytes([line, 0, 0, 0]) + counter.to_bytes(4, "little", signed=True) + bytes(4)
    return [{"addr": 0x800E7374, "length": 32, "status": "ok", "data": game.hex()},
            {"addr": 0x8014D180, "length": 12, "status": "ok", "data": prologue.hex()},
            {"addr": 0x801220E0, "length": 4, "status": "ok", "data": bytes([bri, bri, bri, 0]).hex()},
            {"addr": 0x80129628, "length": 2, "status": "ok", "data": gsi.to_bytes(2, "little").hex()}]


def default_timeline():
    """Native timeline: vsync v has frame_count 99+v up to v=10. Key (8,1) and line 3 start at fc 105 (v=6);
    vsync 11 is a torn mid-iteration read (frame_count 109 held), then key (8,2) starts at fc 110 (v=12).
    Each row is a dict: v, fc, pc, mode, stage, step, sub, line, phase."""
    fc = 101
    timeline = []
    for v in range(2, 16):
        if v == 11:
            pc = 0x80086000   # torn mid-iteration vsync, frame_count not advanced
        else:
            pc = VSYNC_PC
            fc += 1 if v > 2 else 0
        stage, sub = (8, 0) if fc < 105 else (8, 1) if fc < 110 else (8, 2)
        timeline.append({"v": v, "fc": fc, "pc": pc, "mode": 0x15, "stage": stage, "step": 0, "sub": sub,
                         "line": 2 if fc < 105 else 3, "phase": "restored"})
    return timeline


def modified(**change):
    """Default timeline with per-vsync overrides: modified(v7={"line": 4}) patches the row of vsync 7."""
    return [dict(r, **change.get("v%d" % r["v"], {})) for r in default_timeline()]


class Fixture:
    def __init__(self, root, timeline=None, drop_shots=(), receipt_extra=None, observer_after_receipt=False):
        self.native, self.port = root / "native", root / "port"
        self.native.mkdir(); self.port.mkdir()
        self.observer = root / "run_installed_rei_banner.gd"
        self.observer.write_text('var key := "stage%d-%d" % [state.mode_stage,state.substep]\n')
        shots, samples = [], []
        for r in timeline or default_timeline():
            d = dumps(r["fc"], r["stage"], r["sub"], line=r["line"], gsi=r["fc"] & 1, mode=r["mode"], step=r["step"])
            samples.append({"frame": r["v"], "cycles": 1000 * r["v"], "pc": r["pc"], "mode": 21,
                            "phase": "vsync_after_input_override_before_next_cpu_tick",
                            "state_load_phase": r["phase"], "dumps": d, "tag": "sample_%d" % r["v"]})
            if r["v"] in drop_shots or r["phase"] != "restored":
                continue
            # image: text area turns white two vsyncs after the line-3 start iteration (fc 105 -> visible at fc 107)
            colour = (248, 248, 248) if r["fc"] >= 107 else (0, 0, 0)
            img = Image.new("RGB", (320, 240), (0, 0, 0))
            img.paste(colour, (24, 166, 296, 230))
            png = self.native / ("shot_%d.png" % r["v"])
            img.save(png)
            shots.append({"frame": r["v"], "png": png.name, "png_sha256": hashlib.sha256(png.read_bytes()).hexdigest(),
                          "image_status": "ok", "path": str(self.native / ("shot_%d.raw" % r["v"])), "width": 320, "height": 240,
                          "bpp": 16, "mode": 21, "pc": r["pc"], "cycles": 1000 * r["v"],
                          "phase": "vsync_after_input_override_before_next_cpu_tick", "state_load_phase": r["phase"], "dumps": d})
        stream = "".join(json.dumps(s, sort_keys=True) + "\n" for s in samples).encode()
        (self.native / "memory-samples.jsonl").write_bytes(stream)
        (self.native / "result.json").write_text(json.dumps({"shots": shots, "memory_samples": {"sha256": hashlib.sha256(stream).hexdigest()}}))
        observed = {}
        for key, frame in (("stage8-0", 40), ("stage8-1", 50), ("stage8-2", 60)):
            Image.new("RGBA", (320, 240), (255, 255, 255, 255)).save(self.port / (key + ".png"))
            observed[key] = {"frame": frame, "counter": 0, "opening": {"prologue_line": 3, "D_801220E0": 0}}
        receipt = self.port / "rei-failure.json"
        receipt.write_text(json.dumps(dict({"observed": observed}, **(receipt_extra or {}))))
        stamp = receipt.stat().st_mtime + (10 if observer_after_receipt else -10)
        os.utime(self.observer, (stamp, stamp))


class ReiStagePairsTest(unittest.TestCase):
    def run_tool(self, root, latency=2, **fixture):
        fx = Fixture(root, **fixture)
        out = root / "out"
        tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                   "--out", str(out), "--latency", str(latency), "--latency-basis", "synthetic"])
        return out, json.loads((out / "sync-evidence.json").read_text())

    def assert_rejected(self, out, ev, key, problem):
        entry = ev["checkpoints"][key]
        self.assertEqual(entry["status"], "unsynchronized")
        self.assertIn(problem, entry["problems"])
        self.assertFalse((out / ("pair-%s.json" % key)).exists())

    def test_clean_window_produces_pair_with_unverified_sync(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp))
            entry = ev["checkpoints"]["stage8-1"]
            self.assertEqual(entry["status"], "window_ok")
            self.assertEqual(entry["problems"], [])
            self.assertEqual(entry["native_iteration_frame_count"], 105)
            self.assertEqual(entry["image_vsync_frame_count"], entry["native_iteration_frame_count"] + 2)
            pair = json.loads((out / "pair-stage8-1.json").read_text())
            self.assertEqual(pair["synchronization"]["status"], "unverified")
            self.assertEqual(pair["checkpoint"]["expected"], {"stage": 8})
            line = next(m for m in pair["mappings"] if m["name"] == "prologue_line")
            self.assertEqual(line["compatibility"], "equivalent")
            self.assertNotIn("reason", line)
            binding = json.loads((out / "port-png-bindings.json").read_text())
            self.assertIn("post-hoc", binding["binding_origin"])

    def test_observer_is_pinned(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            out, _ = self.run_tool(root)
            binding = json.loads((out / "port-png-bindings.json").read_text())
            observer = binding["observer"]
            self.assertEqual(observer["sha256"], hashlib.sha256((root / "run_installed_rei_banner.gd").read_bytes()).hexdigest())
            self.assertFalse(observer["modified_after_receipt"])
            self.assertIn("observer", binding["observed"]["stage8-1"]["key_origin"])

    def test_observer_modified_after_receipt_is_flagged(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, _ = self.run_tool(Path(tmp), observer_after_receipt=True)
            binding = json.loads((out / "port-png-bindings.json").read_text())
            self.assertTrue(binding["observer"]["modified_after_receipt"])

    def test_observer_is_required(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            fx = Fixture(root)
            with self.assertRaises(SystemExit):
                tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--out", str(root / "out"),
                           "--latency", "2", "--latency-basis", "synthetic"])
            self.assertFalse((root / "out").exists())

    def test_multi_vsync_iteration_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp))
            entry = ev["checkpoints"]["stage8-2"]
            self.assertEqual(entry["status"], "unsynchronized")
            self.assertFalse((out / "pair-stage8-2.json").exists())
            self.assertIn("pc 0x80086000 outside VSync", " ".join(entry["problems"]))
            # stage8-0 has no preceding sample, so its first iteration is unobserved and gets no duration
            self.assertEqual(ev["checkpoints"]["stage8-0"]["status"], "unsynchronized")
            self.assertIsNone(ev["key_durations"]["rows"][0]["native_iterations"])
            self.assertEqual(ev["key_durations"]["rows"][1]["native_iterations"], 5)

    def test_previous_sample_two_iterations_back_is_rejected(self):
        # skipped iteration: vsync 5 shows fc 103 (PC in VSync), vsync 6 shows the key at fc 105
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), timeline=modified(v5={"fc": 103}))
            self.assert_rejected(out, ev, "stage8-1", "first iteration of key not sampled at its own vsync (previous frame_count 103)")

    def test_frame_count_held_inside_window_is_rejected(self):
        # vsyncs 7 and 8 both in VSync with fc 106: one iteration spans two vsyncs inside F..F+2
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), timeline=modified(v8={"fc": 106}))
            self.assert_rejected(out, ev, "stage8-1", "vsync 8 frame_count 106 breaks one-iteration-per-vsync")

    def test_stage_change_inside_window_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), timeline=modified(v7={"stage": 9, "sub": 0}))
            self.assert_rejected(out, ev, "stage8-1", "mode/stage/step changes inside the latency window")
            self.assertEqual(ev["checkpoints"]["stage8-1"]["problems"], ["mode/stage/step changes inside the latency window"])

    def test_missing_shot_at_image_vsync_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), drop_shots=(8,))
            self.assert_rejected(out, ev, "stage8-1", "no shot captured at vsync 8")
            self.assertEqual(ev["checkpoints"]["stage8-1"]["problems"], ["no shot captured at vsync 8"])

    def test_rows_before_restore_are_ignored(self):
        # a pre-restore sample already holding key (8,1) must not become iteration F
        pre = dict(default_timeline()[0], v=1, fc=50, stage=8, sub=1, phase="before_restore")
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), timeline=[pre] + default_timeline())
            entry = ev["checkpoints"]["stage8-1"]
            self.assertEqual(entry["status"], "window_ok")
            self.assertEqual(entry["native_iteration_frame_count"], 105)
            self.assertTrue((out / "pair-stage8-1.json").exists())

    def test_key_requires_opening_mode_and_step_zero(self):
        # vsync 4 (fc 103) holds stage 8 substep 1 but with another mode or step; F must stay 105
        for name, patch in (("mode", {"mode": 0x14}), ("step", {"step": 1})):
            with self.subTest(name), tempfile.TemporaryDirectory() as tmp:
                out, ev = self.run_tool(Path(tmp), timeline=modified(v4=dict(patch, sub=1)))
                entry = ev["checkpoints"]["stage8-1"]
                self.assertEqual(entry["status"], "window_ok")
                self.assertEqual(entry["native_iteration_frame_count"], 105)

    def test_prologue_line_change_inside_window_is_incompatible(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), timeline=modified(v7={"line": 4}, v8={"line": 4}))
            self.assertEqual(ev["checkpoints"]["stage8-1"]["status"], "window_ok")
            pair = json.loads((out / "pair-stage8-1.json").read_text())
            line = next(m for m in pair["mappings"] if m["name"] == "prologue_line")
            self.assertEqual(line["compatibility"], "incompatible")
            self.assertIn("changes inside the latency window", line["reason"])

    def test_port_failure_frame_gets_no_pair(self):
        extra = {"message": "Rei observer stopped at 8/0/1: gate: synthetic", "state": {"frame_count": 50}}
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), receipt_extra=extra)
            entry = ev["checkpoints"]["stage8-1"]
            self.assertEqual(entry["status"], "port_failure_frame")
            self.assertEqual(entry["window_status"], "window_ok")
            self.assertIn("gate: synthetic", entry["port_failure"])
            self.assertFalse((out / "pair-stage8-1.json").exists())
            # native timing diagnostics are kept
            self.assertEqual(ev["key_durations"]["rows"][1]["native_iterations"], 5)
            binding = json.loads((out / "port-png-bindings.json").read_text())
            self.assertEqual(binding["receipt"]["stop_frame"], 50)

    def test_receipt_state_without_message_is_not_a_failure(self):
        with tempfile.TemporaryDirectory() as tmp:
            out, ev = self.run_tool(Path(tmp), receipt_extra={"state": {"frame_count": 50}})
            self.assertEqual(ev["checkpoints"]["stage8-1"]["status"], "window_ok")
            self.assertTrue((out / "pair-stage8-1.json").exists())

    def test_missing_port_checkpoint_is_reported(self):
        with tempfile.TemporaryDirectory() as tmp:
            _, ev = self.run_tool(Path(tmp))
            self.assertEqual(ev["checkpoints"]["stage9-0"]["status"], "port_checkpoint_missing")

    def test_onset_probe_measures_latency(self):
        with tempfile.TemporaryDirectory() as tmp:
            _, ev = self.run_tool(Path(tmp))
            events = [e for e in ev["latency_probe"]["events"] if e["event"] == "line_start 3"]
            self.assertEqual(len(events), 1)
            self.assertEqual(events[0]["onset_latency"], 2)

    def test_output_must_be_new(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "out").mkdir()
            fx = Fixture(root)
            with self.assertRaises(FileExistsError):
                tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                           "--out", str(root / "out"), "--latency", "2", "--latency-basis", "synthetic"])

    def test_changed_samples_are_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            fx = Fixture(root)
            with (fx.native / "memory-samples.jsonl").open("a") as fh:
                fh.write("\n")
            with self.assertRaises(SystemExit):
                tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                           "--out", str(root / "out"), "--latency", "2", "--latency-basis", "synthetic"])


if __name__ == "__main__":
    unittest.main()
