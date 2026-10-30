"""T-39022 (T-39018 WP-05): opening trace projection and first-divergence report, synthetic only.

Native samples and port records are generated here with the committed projection
config; no emulator or game data. Run: tools/port/run_playtest_unit_tests.sh
"""
import contextlib
import copy
import hashlib
import io
import json
from pathlib import Path
import tempfile
import unittest

from tools.port import opening_trace as tool

ROOT = Path(__file__).resolve().parents[2]
CONFIG_PATH = ROOT / "config/port/opening-trace-projection.json"
CONFIG = json.loads(CONFIG_PATH.read_text())
ONSET_NATIVE, ONSET_PORT = 7700, 131


def script(n=12, d1_from=None):
    """Iteration i: substep i, stage 0 until the end at 12/0/0. D_801220E0 becomes 0x80 at
    iteration 5 natively; the port writes 63 there when d1_from is set (the D1 shape)."""
    rows = []
    for i in range(n + 1):
        stage, sub = (12, 0) if i == n else (0 if i < 6 else 8, i % 6)
        bri = 0 if i < 5 else 0x80
        rows.append({"i": i, "stage": stage, "sub": sub, "counter": i * 2, "bri": bri, "x": -160 - i,
                     "layer": 0x41000000 if i % 4 else 0x80000000, "rand": i // 3})
    if d1_from is not None:
        for row in rows[d1_from:]:
            row["port_bri"] = 63
    return rows


def native_samples(rows, drop=(), before=2, stale_x=1234):
    samples, hits = [], []
    vsync = 10
    for _ in range(before):  # pre-opening iterations in name entry
        vsync += 1
        samples.append(sample(vsync, ONSET_NATIVE - (before - _), 0x14, 2, 4, 0, 0, 0, 0, stale_x))
    for row in rows:
        vsync += 1
        hits.extend({"frame": vsync - 1, "address": tool.RAND, "pc": tool.RAND} for _ in range(row["rand"] - (rows[row["i"] - 1]["rand"] if row["i"] else 0)))
        if row["i"] in drop:
            continue  # iteration took no vsync of its own (catch-up)
        samples.append(sample(vsync, ONSET_NATIVE + row["i"], 0x15, row["stage"], 0, row["sub"], row["counter"], row["bri"], row["layer"], row["x"]))
    return samples, hits


def sample(vsync, fc, mode, stage, step, sub, counter, bri, layer, x):
    game = bytearray(32)
    game[4:8] = fc.to_bytes(4, "little")
    game[0x14], game[0x15], game[0x16], game[0x19] = mode, stage, step, sub
    prologue = bytes(4) + counter.to_bytes(4, "little", signed=True) + bytes(4)
    bg = layer.to_bytes(4, "little") + bytes(12) + bytes([bri, 0, 0, 0])
    actors = bytearray(72)
    actors[0x26:0x28] = x.to_bytes(2, "little", signed=True)
    actors[0x18] = vsync & 0xFF  # lip-sync byte: excluded, so it may differ freely
    return {"frame": vsync, "cycles": 1000 * vsync, "pc": 0x800ADABC, "mode": mode, "phase": "vsync", "state_load_phase": "restored",
            "dumps": [{"addr": 0x800E7374, "length": 32, "status": "ok", "data": game.hex()},
                      {"addr": 0x8014D180, "length": 12, "status": "ok", "data": prologue.hex()},
                      {"addr": 0x801220D0, "length": 20, "status": "ok", "data": bg.hex()},
                      {"addr": 0x80120650, "length": 72, "status": "ok", "data": actors.hex()}]}


def port_records(rows, actor_from=0, rand_base=40):
    out = []
    for row in rows:
        opening = {"prologue_scene_counter": row["counter"], "D_801220E0": row.get("port_bri", row["bri"]),
                   "D_801220E1": 0, "D_801220E2": 0, "bg_layer_0": {"enabled": 1 if row["layer"] == 0x41000000 else 0}}
        if row["i"] >= actor_from:
            opening["actor96"] = {"x": row["x"]}
        out.append({"frame_count": ONSET_PORT + row["i"], "game_mode": 0x15, "mode_stage": row["stage"], "mode_step": 0,
                    "substep": row["sub"], "opening": opening, "rng": {"draw_count": rand_base + row["rand"]}})
    return out


def run(samples, hits, port):
    return tool.build(CONFIG, samples, hits, port, "0" * 64)


class ConfigTests(unittest.TestCase):
    def test_config_names_exclusions_with_reasons(self):
        excluded = {e["name"]: e for e in CONFIG["excluded"]}
        self.assertEqual(excluded["actor96_lip_sync_frame"]["address"], "0x80120668")
        self.assertIn("getCDlevel", excluded["actor96_lip_sync_frame"]["reason"])
        self.assertIn("cpu_cycles", excluded)
        names = {f["name"] for f in CONFIG["fields"]}
        self.assertTrue({"mode_stage", "substep", "frame_count", "prologue_scene_counter", "bg_layers",
                         "D_801220E0", "D_801220E1", "D_801220E2", "actor96_x"} <= names)
        self.assertNotIn("0x80120668", {f["address"] for f in CONFIG["fields"]})
        for field in CONFIG["fields"]:
            self.assertIn(field["compare"], CONFIG["compare_rules"])


class TraceTests(unittest.TestCase):
    def test_equal_traces(self):
        rows = script()
        native, port, info = run(*native_samples(rows), port_records(rows))
        self.assertIsNone(tool.compare_trace.compare(native, port))
        self.assertEqual(len(native), len(rows))
        self.assertTrue(info["native_reached_end"] and info["port_reached_end"])
        self.assertNotIn("lip", json.dumps(native))
        self.assertEqual(native[3]["state"]["rand_calls"], 1)

    def test_d1_shape_is_first_divergence(self):
        rows = script(d1_from=5)
        native, port, _ = run(*native_samples(rows), port_records(rows))
        first = tool.compare_trace.compare(native, port)
        self.assertEqual((first["iteration"], first["field"]), (5, "$.state.D_801220E0"))
        self.assertEqual((first["expected"], first["actual"]), (0x80, 63))

    def test_rng_count_divergence(self):
        rows = script()
        port = port_records(rows)
        port[7]["rng"]["draw_count"] += 1
        native, ported, _ = run(*native_samples(rows), port)
        first = tool.compare_trace.compare(native, ported)
        self.assertEqual((first["iteration"], first["field"]), (7, "$.state.rand_calls"))

    def test_when_port_defined_gating_and_derived_layer(self):
        rows = script()
        native, port, info = run(*native_samples(rows, stale_x=999), port_records(rows, actor_from=4))
        self.assertIsNone(tool.compare_trace.compare(native, port))
        self.assertEqual(info["port_defined_from"]["actor96_x"], 4)
        self.assertIsNone(native[0]["state"]["actor96_x"])
        self.assertIs(native[1]["state"]["bg_layers"], True)
        self.assertIs(native[0]["state"]["bg_layers"], False)

    def test_gated_native_values_are_counted(self):
        rows = script()
        native, port, info = run(*native_samples(rows, stale_x=999), port_records(rows, actor_from=4))
        self.assertEqual(info["never_defined"], [])
        gated = info["gated_native_values"]["actor96_x"]
        self.assertEqual((gated["count"], gated["first_iteration"], gated["last_iteration"]), (4, 0, 3))
        self.assertEqual((gated["before_port_definition"], gated["after_port_definition"]), (4, 0))
        self.assertEqual(gated["first_value"], -160)

    def test_port_omitting_a_configured_field_fails(self):
        rows = script(d1_from=5)  # the port value would diverge, but the field is never written
        port = port_records(rows)
        for record in port:
            del record["opening"]["D_801220E0"]
        native, ported, info = run(*native_samples(rows), port)
        self.assertEqual(info["never_defined"], ["D_801220E0"])
        self.assertIsNone(tool.compare_trace.compare(native, ported))  # the records alone would look equal
        gated = info["gated_native_values"]["D_801220E0"]
        self.assertEqual((gated["count"], gated["first_iteration"], gated["first_value"]), (len(rows) - 5, 5, 0x80))
        self.assertEqual(gated["before_port_definition"], len(rows) - 5)
        samples, hits = native_samples(rows)
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            native_dir = root / "native"
            native_dir.mkdir()
            stream = "".join(json.dumps(s) + "\n" for s in samples)
            (native_dir / "memory-samples.jsonl").write_text(stream)
            (native_dir / "result.json").write_text(json.dumps({"hits": hits, "counts": {"0x800AE0D0": len(hits)},
                "memory_samples": {"sha256": hashlib.sha256(stream.encode()).hexdigest()}}))
            (root / "port.jsonl").write_text("".join(json.dumps(r) + "\n" for r in port))
            with contextlib.redirect_stdout(io.StringIO()):
                code = tool.main(["--config", str(CONFIG_PATH), "--native-dir", str(native_dir),
                                  "--port-trace", str(root / "port.jsonl"), "--out", str(root / "out")])
            self.assertEqual(code, 1)
            report = json.loads((root / "out" / "report.json").read_text())
            self.assertFalse(report["equal"])
            self.assertIsNone(report["first_divergence"])
            self.assertEqual(report["never_defined"], ["D_801220E0"])

    def test_unobserved_native_iteration_is_listed_not_interpolated(self):
        rows = script()
        native, port, info = run(*native_samples(rows, drop={4}), port_records(rows))
        self.assertEqual(info["native_unobserved_iterations"], [4])
        self.assertNotIn(4, [r["iteration"] for r in native + port])
        self.assertIsNone(tool.compare_trace.compare(native, port))

    def test_port_field_cleared_again_is_reported_as_a_span(self):
        rows = script()
        port = port_records(rows)
        for record in port[7:9]:
            del record["opening"]["actor96"]
        native, ported, info = run(*native_samples(rows), port)
        self.assertEqual(info["port_undefined_after_first"], {"actor96_x": [[7, 8]]})
        self.assertIsNone(native[7]["state"]["actor96_x"])
        self.assertIsNone(tool.compare_trace.compare(native, ported))

    def test_port_reseed_keeps_cumulative_draws(self):
        rows = script()
        port = port_records(rows)
        reseed_at = 6  # srand: draw_count restarts at 0, then counts draws since the reseed
        base = rows[reseed_at - 1]["rand"]
        for record, row in zip(port[reseed_at:], rows[reseed_at:]):
            record["rng"]["draw_count"] = row["rand"] - base
        native, ported, info = run(*native_samples(rows), port)
        self.assertEqual(info["port_reseed_iterations"], [reseed_at])
        self.assertIsNone(tool.compare_trace.compare(native, ported))

    def test_consecutive_native_parts_join_by_frame_count(self):
        rows = script()
        samples, hits = native_samples(rows)
        cut = 7
        boundary = samples[cut]["frame"]
        first = (samples[:cut + 1], [h for h in hits if h["frame"] < boundary])
        # part 2 restarts vsync numbering at 1 and repeats the last sample of part 1
        shift = boundary - 1
        second_samples = [dict(s, frame=s["frame"] - shift) for s in samples[cut:]]
        second_hits = [dict(h, frame=h["frame"] - shift) for h in hits if h["frame"] >= boundary]
        native, port, _ = tool.build(CONFIG, [first, (second_samples, second_hits)], None, port_records(rows), "0" * 64)
        whole, _, _ = run(samples, hits, port_records(rows))
        self.assertEqual(native, whole)
        self.assertIsNone(tool.compare_trace.compare(native, port))

    def test_port_trace_with_skipped_iteration_is_invalid(self):
        rows = script()
        port = port_records(rows)
        del port[6]
        with self.assertRaises(tool.Invalid):
            run(*native_samples(rows), port)

    def test_missing_onset_or_dump_is_invalid(self):
        rows = script()
        with self.assertRaises(tool.Invalid):
            run(*native_samples(rows), port_records(rows)[1:])
        samples, hits = native_samples(rows)
        samples[3]["dumps"] = samples[3]["dumps"][:2]
        with self.assertRaises(ValueError):
            run(samples, hits, port_records(rows))

    def test_cli_writes_report_and_exit_codes(self):
        rows = script(d1_from=5)
        samples, hits = native_samples(rows)
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            native_dir = root / "native"
            native_dir.mkdir()
            stream = "".join(json.dumps(s) + "\n" for s in samples)
            (native_dir / "memory-samples.jsonl").write_text(stream)
            (native_dir / "result.json").write_text(json.dumps({"hits": hits, "counts": {"0x800AE0D0": len(hits)},
                "memory_samples": {"sha256": hashlib.sha256(stream.encode()).hexdigest()}}))
            (root / "port.jsonl").write_text("".join(json.dumps(r) + "\n" for r in port_records(rows)))
            args = ["--config", str(CONFIG_PATH), "--native-dir", str(native_dir), "--port-trace", str(root / "port.jsonl")]
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(tool.main(args + ["--out", str(root / "out")]), 1)
            report = json.loads((root / "out" / "report.json").read_text())
            self.assertEqual(report["first_divergence"]["iteration"], 5)
            self.assertEqual(report["first_divergence_by_field"]["D_801220E0"], {"iteration": 5, "native": 0x80, "port": 63})
            self.assertIn("actor96_lip_sync_frame", report["excluded"])
            truncated = json.loads((native_dir / "result.json").read_text())
            truncated["counts"] = {"0x800AE0D0": len(hits) + 1}
            (native_dir / "result.json").write_text(json.dumps(truncated))
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(tool.main(args + ["--out", str(root / "out2")]), 2)


if __name__ == "__main__":
    unittest.main()
