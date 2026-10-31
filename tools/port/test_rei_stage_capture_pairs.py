"""Synthetic tests for tools/port/rei_stage_capture_pairs.py (T-39008). No game data.

Run in Docker: python3 -m unittest tools.port.test_rei_stage_capture_pairs
"""
import contextlib
import hashlib
import io
import json
import os
from pathlib import Path
import tempfile
import unittest
import unittest.mock

from PIL import Image

try:
    from tools.port import checkpoint_receipt
    from tools.port import parity_gate
    from tools.port import rei_stage_capture_pairs as tool
except ModuleNotFoundError:
    import checkpoint_receipt
    import parity_gate
    import rei_stage_capture_pairs as tool

PIN = parity_gate.load_pin()

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
        # T-39230: the legacy port run passed the Parity gate.
        (self.port / "parity-mode.json").write_text(json.dumps({"schema": "tokimemo.parity-mode-identity.v1", "play_mode": "parity",
                                                                "invariance_run": False, "violations": [], "gate": "passed",
                                                                "registry_sha256": PIN["registry_sha256"],
                                                                "locked_values_sha256": PIN["locked_values_sha256"]}))
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



def write_v2(port, failure=True, tamper=None, resign=False):
    """Synthetic v2 receipt set for the fixture's port PNGs (T-39019 shape), bound with real digests.

    ``tamper`` edits the receipts after they were bound (a post-write edit).
    With ``resign`` the digest is recomputed after the edit, so only the other
    checks (key, clocks, pixels) can catch it.
    """
    receipts = {}
    for key, frame in (("stage8-0", 40), ("stage8-1", 50), ("stage8-2", 60)):
        png = port / (key + ".png")
        stage, sub = (int(x) for x in key[len("stage"):].split("-"))
        receipts[key] = {
            "schema": "tokimemo.checkpoint-receipt.v2", "key": key,
            "native_canvas": {"width": 320, "height": 240, "source": "cpu_composed_image", "png": png.name,
                              "png_sha256": hashlib.sha256(png.read_bytes()).hexdigest(),
                              "rgba8_sha256": checkpoint_receipt.rgba8_sha256(png)[0]},
            "clocks": {"frame_count": frame, "game_mode": 0x15, "mode_stage": stage, "mode_step": 0, "substep": sub},
            "rng": {"draw_count": 7}, "settings": {"source": "synthetic", "language": "ja", "aspect": "original", "filter": "integer",
                                                   "play_mode": "parity", "parity_violations": [], "invariance_run": False,
                                                   "parity_registry_sha256": PIN["registry_sha256"],
                                                   "parity_locked_values_sha256": PIN["locked_values_sha256"]},
            "content": {}, "renderer": {"engine": "4.7.2", "rendering_method": "gl_compatibility", "adapter": "", "vendor": "", "api": ""},
            "observer": {"path": "res://tests/support/checkpoint_observer.gd", "sha256": "1" * 64,
                         "producer": "res://tests/opening/run_installed_rei_banner.gd", "producer_sha256": "2" * 64},
            "extra": {"prologue_scene_counter": 0, "opening": {"prologue_line": 3, "D_801220E0": 0, "volume": 0.25}}}
        receipts[key]["receipt_sha256"] = checkpoint_receipt.receipt_digest(receipts[key])
    if tamper:
        tamper(receipts)
        if resign:
            for receipt in receipts.values():
                receipt["receipt_sha256"] = checkpoint_receipt.receipt_digest(receipt)
    (port / "checkpoint-receipts.json").write_text(json.dumps({"schema": "tokimemo.checkpoint-receipt.v2-set", "receipts": receipts, "errors": []}))
    if not failure:
        (port / "rei-failure.json").unlink()
    return receipts


class ReceiptV2Test(unittest.TestCase):
    def run_v2(self, root, **v2):
        fx = Fixture(root)
        write_v2(fx.port, **v2)
        out = root / "out"
        tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                   "--out", str(out), "--latency", "2", "--latency-basis", "synthetic"])
        return fx, out

    def test_v2_receipts_must_prove_parity(self):
        """T-39230: receipts without a clean Parity play mode are refused before pairing."""
        cases = {"missing": lambda s: s.pop("play_mode"), "modern": lambda s: s.update(play_mode="modern"),
                 "unclean": lambda s: s.update(parity_violations=["display.aspect"]),
                 "invariance": lambda s: s.update(play_mode="modern", invariance_run=True),
                 "stale registry": lambda s: s.update(parity_registry_sha256="0" * 64)}
        for name, change in cases.items():
            def tamper(receipts, change=change):
                change(receipts["stage8-1"]["settings"])
            with self.subTest(name), tempfile.TemporaryDirectory() as tmp:
                with self.assertRaises(SystemExit) as raised:
                    self.run_v2(Path(tmp), failure=False, tamper=tamper, resign=True)
                self.assertIn("not Parity evidence", str(raised.exception))
        # A modern-invariance run is never pair evidence, with or without MODERN_INVARIANCE=1,
        # and the pair builder has no option that accepts one.
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(Path(tmp))
            write_v2(fx.port, failure=False, resign=True,
                     tamper=lambda receipts: [r["settings"].update(play_mode="modern", invariance_run=True) for r in receipts.values()])
            argv = ["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                    "--latency", "2", "--latency-basis", "synthetic"]
            with unittest.mock.patch.dict("os.environ", {"MODERN_INVARIANCE": "1"}), self.assertRaises(SystemExit) as raised:
                tool.main(argv + ["--out", str(Path(tmp) / "out")])
            self.assertIn("not Parity evidence", str(raised.exception))
            with contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as raised:
                tool.main(argv + ["--out", str(Path(tmp) / "out2"), "--modern-invariance"])
            self.assertEqual(raised.exception.code, 2)

    def test_legacy_run_without_parity_identity_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            fx = Fixture(Path(tmp))
            (fx.port / "parity-mode.json").unlink()
            with self.assertRaises(SystemExit) as raised:
                tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                           "--out", str(Path(tmp) / "out"), "--latency", "2", "--latency-basis", "synthetic"])
            self.assertIn("parity-mode.json", str(raised.exception))

    def test_v2_receipts_need_no_post_hoc_binding(self):
        from tools.port.compare_capture_pair import compare
        with tempfile.TemporaryDirectory() as tmp:
            _, out = self.run_v2(Path(tmp), failure=False)
            self.assertFalse((out / "port-png-bindings.json").exists())
            pair = json.loads((out / "pair-stage8-1.json").read_text())
            self.assertEqual(pair["port"]["selector"], ["receipts", "stage8-1"])
            self.assertEqual(pair["port"]["image_sha_path"], ["native_canvas", "png_sha256"])
            self.assertEqual(next(m for m in pair["mappings"] if m["name"] == "stage")["port_path"], ["clocks", "mode_stage"])
            report, code = compare(pair, out)  # the comparator consumes the receipt directly
            self.assertEqual(code, 1)
            self.assertEqual(report["semantics"][0]["port"], 8)
            self.assertEqual(report["clocks"]["port"]["port_rng_draw_count"], 7)

    def test_v2_tampered_receipt_fields_are_rejected(self):
        def edit(path, value):
            def tamper(receipts):
                target = receipts["stage8-1"]
                for name in path[:-1]:
                    target = target[name]
                target[path[-1]] = value
            return tamper

        def frame_and_draws(receipts):
            receipts["stage8-1"]["clocks"]["frame_count"] = 51
            receipts["stage8-1"]["rng"]["draw_count"] = 8

        cases = {"frame_count and draw_count": frame_and_draws,
                 "frame_count": edit(("clocks", "frame_count"), 49),
                 "draw_count": edit(("rng", "draw_count"), 6),
                 "scene counter": edit(("extra", "prologue_scene_counter"), 320),
                 "D_801220E0": edit(("extra", "opening", "D_801220E0"), 128),
                 "settings": edit(("settings", "language"), "en"),
                 "observer hash": edit(("observer", "sha256"), "4" * 64),
                 "producer hash": edit(("observer", "producer_sha256"), "5" * 64),
                 "digest": edit(("receipt_sha256",), "3" * 64)}
        for name, tamper in cases.items():
            with self.subTest(name), tempfile.TemporaryDirectory() as tmp:
                with self.assertRaises(SystemExit) as raised:
                    self.run_v2(Path(tmp), tamper=tamper)
                self.assertIn("receipt fields changed after binding", str(raised.exception))

    def test_v2_resigned_wrong_clock_or_pixels_are_rejected(self):
        def wrong_clock(receipts):
            receipts["stage8-1"]["clocks"]["substep"] = 2

        def wrong_pixels(receipts):
            receipts["stage8-1"]["native_canvas"]["rgba8_sha256"] = "0" * 64

        for name, tamper, message in (("clock", wrong_clock, "clocks hold stage 8 substep 2"),
                                      ("rgba8", wrong_pixels, "decoded pixels differ")):
            with self.subTest(name), tempfile.TemporaryDirectory() as tmp:
                with self.assertRaises(SystemExit) as raised:
                    self.run_v2(Path(tmp), tamper=tamper, resign=True)
                self.assertIn(message, str(raised.exception))

    def test_v2_replaced_png_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            fx = Fixture(root)
            write_v2(fx.port)
            Image.new("RGBA", (320, 240), (0, 0, 0, 255)).save(fx.port / "stage8-1.png")
            with self.assertRaises(SystemExit) as raised:
                tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                           "--out", str(root / "out"), "--latency", "2", "--latency-basis", "synthetic"])
            self.assertIn("PNG changed after binding", str(raised.exception))

    def test_receipt_digest_matches_godot_text(self):
        # Expected text was printed by Godot 4.7.2 JSON.stringify(value, "", true) in Docker tokimemo-port.
        value = {"b": 1, "a": [True, None, 2], "B": {"z": 1, "é": 2, "_": 3},
                 "f": [0.5, 3.0, 2.0 / 3.0, 12345678.123456789, 1e-7, 1e20, -0.001, 0.0, -0.0, 1e15 + 0.5]}
        self.assertEqual(checkpoint_receipt.godot_json(value),
                         '{"B":{"_":3,"z":1,"é":2},"a":[true,null,2],"b":1,'
                         '"f":[0.5,3.0,0.666666666666667,12345678.1234568,0.0000001,100000000000000000000.0,'
                         '-0.001,0.0,0.0,1000000000000000.5]}')
        self.assertEqual(checkpoint_receipt.godot_json("a\vb\x01c\"\\/\n\t"), '"a\\vb\x01c\\"\\\\/\\n\\t"')
        receipt = {"schema": "x", "n": 3.0, "receipt_sha256": "ignored"}
        self.assertEqual(checkpoint_receipt.receipt_digest(receipt), hashlib.sha256(b'{"n":3,"schema":"x"}').hexdigest())

    def test_v2_observer_refusals_block_pairing(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            fx = Fixture(root)
            write_v2(fx.port)
            data = json.loads((fx.port / "checkpoint-receipts.json").read_text())
            data["errors"] = ["stage8-3: native canvas must be 320x240"]
            (fx.port / "checkpoint-receipts.json").write_text(json.dumps(data))
            with self.assertRaises(SystemExit):
                tool.main(["--native-dir", str(fx.native), "--port-dir", str(fx.port), "--observer", str(fx.observer),
                           "--out", str(root / "out"), "--latency", "2", "--latency-basis", "synthetic"])

    def test_v2_receipt_schema(self):
        try:
            from jsonschema import Draft202012Validator
        except ModuleNotFoundError:
            self.fail("jsonschema is required (tools/port/requirements-test.txt)")
        schema = json.loads((Path(__file__).resolve().parent / "schemas" / "checkpoint-receipt.v2.schema.json").read_text())
        validator = Draft202012Validator(schema)
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            fx = Fixture(root)
            receipt = write_v2(fx.port)["stage8-1"]
        self.assertEqual(list(validator.iter_errors(receipt)), [])
        for change in ({"native_canvas": dict(receipt["native_canvas"], width=640)},
                       {"clocks": {"frame_count": 1.5, "game_mode": 21, "mode_stage": 8, "mode_step": 0, "substep": 1}},
                       {"rng": {}}, {"observer": dict(receipt["observer"], path="res://other.gd")}):
            with self.subTest(change=list(change)):
                self.assertTrue(list(validator.iter_errors(dict(receipt, **change))))


if __name__ == "__main__":
    unittest.main()
