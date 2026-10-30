"""T-39023 (T-39018 WP-06): input-script v1 schema and emulator player, synthetic traces only.

Run: tools/port/run_playtest_unit_tests.sh tools.emu.test_input_script
"""
import contextlib
import copy
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import input_script as tool  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
SEED = ROOT / "config/port/input-scripts/new_game_to_school_handoff.json"
SCHEMA = ROOT / "tools/port/schemas/input-script.v1.schema.json"
CATALOG = ROOT / "config/port/playtest-checkpoints.json"


def script(*segments, unsupported=()):
    return {"schema": "tokimemo.input-script.v1", "name": "synthetic", "description": "synthetic unit script",
            "provenance": {"source": "unit test fixture", "derivation": "hand-written"},
            "segments": list(segments), "checkpoints": [], "unsupported": list(unsupported)}


def trace(states, drops=(), start_vsync=1, start_fc=100):
    """One row per vsync. states(fc) -> (mode, stage, step, sub). A vsync in ``drops`` repeats the
    previous frame_count (the iteration took two vsyncs)."""
    rows, fc = [], start_fc
    for i in range(400):
        vsync = start_vsync + i
        if i and vsync not in drops:
            fc += 1
        mode, stage, step, sub = states(fc)
        rows.append({"vsync": vsync, "frame_count": fc, "game_mode": mode, "mode_stage": stage, "mode_step": step, "substep": sub})
    return rows


def name_states(fc):
    return (0x14, 1, 1 if fc < 150 else 2, 0)


SEG_A = {"id": "a", "wait_until": {"game_mode": "0x14", "mode_stage": 1, "mode_step": 1},
         "events": [{"at": 5, "buttons": ["RIGHT"], "hold": 3}, {"at": 20, "buttons": ["START"], "hold": 2}],
         "expect_next": {"game_mode": "0x14", "mode_stage": 1, "mode_step": 2}}
SEG_B = {"id": "b", "wait_until": {"game_mode": "0x14", "mode_stage": 1, "mode_step": 2},
         "events": [{"at": 4, "buttons": ["CIRCLE", "UP"], "hold": 1}]}


class SchemaTests(unittest.TestCase):
    def validator(self):
        from jsonschema import Draft202012Validator  # required, never skipped
        return Draft202012Validator(json.loads(SCHEMA.read_text()))

    def test_seed_script_is_valid_and_its_checkpoints_exist(self):
        seed = json.loads(SEED.read_text())
        self.assertEqual(list(self.validator().iter_errors(seed)), [])
        self.assertEqual(tool.semantic_errors(seed), [])
        keys = {entry["key"] for entry in json.loads(CATALOG.read_text())["checkpoints"]}
        self.assertEqual(sorted(set(seed["checkpoints"]) - keys), [])
        ids = [s["id"] for s in seed["segments"]]
        self.assertEqual(ids[0], "family-name")
        self.assertEqual(ids[-1], "school-handoff")
        prologue = next(s for s in seed["segments"] if s["id"] == "prologue")
        self.assertEqual(prologue["events"], [])  # START would skip the prologue

    def test_schema_negatives(self):
        seed = json.loads(SEED.read_text())
        for name, change in (("unknown button", lambda s: s["segments"][0]["events"][0].update(buttons=["TURBO"])),
                             ("negative at", lambda s: s["segments"][0]["events"][0].update(at=-1)),
                             ("empty predicate", lambda s: s["segments"][0].update(wait_until={})),
                             ("predicate on RAM", lambda s: s["segments"][0]["wait_until"].update(address="0x800E7388")),
                             ("capability injection", lambda s: s["segments"][0].update(capabilities=["entry"])),
                             ("unsupported without reason", lambda s: s["unsupported"][0].pop("reason"))):
            with self.subTest(name):
                bad = copy.deepcopy(seed)
                change(bad)
                self.assertTrue(list(self.validator().iter_errors(bad)))

    def test_semantic_negatives(self):
        overlap = copy.deepcopy(SEG_A)
        overlap["events"] = [{"at": 5, "buttons": ["RIGHT"], "hold": 10}, {"at": 8, "buttons": ["RIGHT"], "hold": 1}]
        self.assertTrue(any("still held" in e for e in tool.semantic_errors(script(overlap))))
        unordered = copy.deepcopy(SEG_A)
        unordered["events"].reverse()
        self.assertTrue(any("order" in e for e in tool.semantic_errors(script(unordered))))
        self.assertTrue(any("duplicate" in e for e in tool.semantic_errors(script(SEG_A, SEG_A))))


class CompileTests(unittest.TestCase):
    def test_press_lands_before_the_visible_iteration(self):
        rows = trace(name_states)
        events = tool.compile_events(script(SEG_A, SEG_B), rows)
        # anchor fc 100 at vsync 1; RIGHT visible at fc 105 -> pressed at the vsync observed at fc 104
        self.assertEqual(events[0], {"frame": 5, "buttons": ["RIGHT"], "hold": 3, "segment": "a", "visible_frame_count": 105})
        self.assertEqual(events[1]["frame"], 20)
        # segment b anchors at fc 150 (first 1/2 iteration after a's last press) -> visible 154
        self.assertEqual(events[2]["visible_frame_count"], 154)
        self.assertEqual(tool.input_argument(events), "5:RIGHT/3,20:START/2,54:CIRCLE+UP/1")

    def test_dropped_vsync_shifts_vsync_not_iteration(self):
        rows = trace(name_states, drops={3})  # iteration 101 spans vsyncs 2-3
        events = tool.compile_events(script(SEG_A), rows)
        self.assertEqual(events[0]["frame"], 6)  # fc 104 is now observed at vsync 6
        rows = trace(name_states, drops={7})  # a held iteration inside the RIGHT hold
        events = tool.compile_events(script(SEG_A), rows)
        self.assertEqual((events[0]["frame"], events[0]["hold"]), (5, 4))  # hold covers 3 iterations = 4 vsyncs

    def test_press_and_release_use_the_last_vsync_of_a_multi_vsync_iteration(self):
        # Opening shape: every iteration spans three vsyncs (fc = 100 + (vsync - 1) // 3).
        rows = trace(name_states, drops={v for v in range(1, 401) if (v - 1) % 3})
        self.assertEqual([r["frame_count"] for r in rows[12:16]], [104, 104, 104, 105])
        events = tool.compile_events(script(SEG_A), rows)
        # RIGHT visible at fc 105 for 3 iterations: pressed at the LAST vsync of fc 104 (15, not 13),
        # released at the last vsync of fc 107 (24), so HOLD is 9 vsyncs.
        self.assertEqual((events[0]["frame"], events[0]["hold"]), (15, 9))
        # START visible at fc 120 for 2 iterations: last vsync of fc 119 is 60, of fc 121 is 66.
        self.assertEqual((events[1]["frame"], events[1]["hold"]), (60, 6))
        self.assertEqual(tool.last_vsync_at(rows, 0, 104), 15)
        # Uneven spans: fc 104 takes vsyncs 5-8, fc 105 takes 9-10, fc 107 only vsync 12.
        uneven = trace(name_states, drops={6, 7, 8, 10})
        self.assertEqual([r["frame_count"] for r in uneven[4:12]], [104, 104, 104, 104, 105, 105, 106, 107])
        events = tool.compile_events(script(SEG_A), uneven)
        self.assertEqual((events[0]["frame"], events[0]["hold"]), (8, 4))

    def test_anchor_uses_first_observed_iteration_and_through(self):
        late = lambda fc: (0x14, 1, 1, 0) if fc >= 130 else (0x02, 0, 0, 0)
        events = tool.compile_events(script(SEG_A), trace(late))
        self.assertEqual(events[0]["visible_frame_count"], 135)
        both = script(SEG_A, SEG_B)
        self.assertEqual({e["segment"] for e in tool.compile_events(both, trace(name_states), through="a")}, {"a"})
        with self.assertRaises(tool.ScriptError):
            tool.compile_events(both, trace(name_states), through="zzz")

    def test_unresolved_segment_and_timeout_stop_compilation(self):
        never = copy.deepcopy(SEG_B)
        never["wait_until"] = {"game_mode": "0x34"}
        with self.assertRaises(tool.ScriptError) as caught:
            tool.compile_events(script(SEG_A, never), trace(name_states))
        self.assertIn("segment b", str(caught.exception))
        slow = copy.deepcopy(SEG_B)
        slow["timeout_iterations"] = 10
        with self.assertRaises(tool.ScriptError):
            tool.compile_events(script(SEG_A, slow), trace(lambda fc: (0x14, 1, 1 if fc < 180 else 2, 0)))
        short = copy.deepcopy(SEG_A)
        short["events"][1]["at"] = 1000
        with self.assertRaises(tool.ScriptError):
            tool.compile_events(script(short), trace(name_states))

    def test_two_presses_on_one_vsync_are_refused(self):
        with self.assertRaises(tool.ScriptError):
            tool.input_argument([{"frame": 4, "buttons": ["UP"], "hold": 1}, {"frame": 4, "buttons": ["LEFT"], "hold": 1}])

    def test_verify_expect_next(self):
        rows = trace(name_states)
        self.assertTrue(tool.verify(script(SEG_A, SEG_B), rows)["passed"])
        wrong = lambda fc: (0x14, 1, 1 if fc < 150 else 4, 0)
        result = tool.verify(script(SEG_A, SEG_B), trace(wrong))
        self.assertFalse(result["passed"])
        self.assertEqual(result["segments"][0]["status"], "expect_next_failed")

    def test_verify_expect_state(self):
        seg = copy.deepcopy(SEG_A)
        seg["expect_state"] = {"names": {"family": "82BF814081400000"}, "player": {"month": 1, "day": 2, "blood": 1}}

        def with_values(family, blood):
            rows = trace(name_states)
            for row in rows:
                row["names"] = {"family": family if row["frame_count"] >= 140 else "8140814081400000",
                                "given": "8140814081400000", "nickname": "814081408140814081400000"}
                row["player"] = {"month": 1, "day": 2, "blood": blood}
            return rows

        self.assertTrue(tool.verify(script(seg), with_values("82bf814081400000", 1))["passed"])
        # The right transition with a random preset name (a missed kana press) must fail.
        result = tool.verify(script(seg), with_values("8e528add81400000", 1))
        self.assertFalse(result["passed"])
        self.assertEqual(result["segments"][0]["status"], "expect_state_failed")
        self.assertEqual(result["segments"][0]["mismatches"],
                         ["names.family: expected 82bf814081400000, observed 8e528add81400000"])
        result = tool.verify(script(seg), with_values("82bf814081400000", 2))
        self.assertEqual(result["segments"][0]["mismatches"], ["player.blood: expected 1, observed 2"])
        # A trace without the name-entry dump cannot pass an expect_state segment.
        result = tool.verify(script(seg), trace(name_states))
        self.assertFalse(result["passed"])
        self.assertIn("observed unavailable", result["segments"][0]["mismatches"][0])

    def test_expect_state_semantics_and_schema(self):
        seg = copy.deepcopy(SEG_A)
        del seg["expect_next"]
        seg["expect_state"] = {"names": {"family": "82bf814081400000"}}
        self.assertTrue(any("needs expect_next" in e for e in tool.semantic_errors(script(seg))))
        validator = SchemaTests().validator()
        seed = json.loads(SEED.read_text())
        for name, change in (("short hex", lambda s: s["segments"][0]["expect_state"]["names"].update(family="82bf")),
                             ("unknown group", lambda s: s["segments"][0]["expect_state"].update(rng={"draw_count": 0})),
                             ("blood out of range", lambda s: s["segments"][3]["expect_state"]["player"].update(blood=4))):
            with self.subTest(name):
                bad = copy.deepcopy(seed)
                change(bad)
                self.assertTrue(list(validator.iter_errors(bad)))

    def test_seed_script_pins_set_b_values(self):
        seed = {s["id"]: s for s in json.loads(SEED.read_text())["segments"]}
        chi = "82bf814081400000"
        self.assertEqual(seed["family-name"]["expect_state"], {"names": {"family": chi}})
        self.assertEqual(seed["nickname"]["expect_state"]["names"]["nickname"], "82bf81408140814081400000")
        self.assertEqual(seed["player-birthday"]["expect_state"]["player"], {"month": 1, "day": 2, "blood": 1})
        self.assertEqual(seed["shiori-birthday"]["expect_state"]["shiori"], {"month": 0, "day": 0, "blood": 0})
        for name in ("family-name", "given-name", "nickname", "player-birthday", "shiori-birthday", "shiori-confirm"):
            self.assertIn("expect_state", seed[name], name)

    def test_rows_from_samples_and_logic(self):
        game = bytearray(32)
        game[4:8] = (321).to_bytes(4, "little")
        game[0x14], game[0x15], game[0x16], game[0x19] = 0x14, 1, 7, 0
        sample = {"frame": 9, "state_load_phase": "restored", "dumps": [{"addr": 0x800E7374, "length": 32, "status": "ok", "data": game.hex()}]}
        before = dict(sample, state_load_phase="before_restore", frame=1)
        rows = tool.rows_from_samples([json.dumps(before), json.dumps(sample)])
        self.assertEqual(rows, [{"vsync": 9, "frame_count": 321, "game_mode": 0x14, "mode_stage": 1, "mode_step": 7, "substep": 0}])
        with self.assertRaises(tool.ScriptError):
            tool.rows_from_samples([json.dumps(dict(sample, dumps=[]))])
        entry = bytes.fromhex("82bf814081400000" "82bf814081400000" "82bf81408140814081400000")
        entry += b"".join(v.to_bytes(4, "little") for v in (1, 2, 1, 0, 0, 0))
        with_entry = dict(sample, dumps=sample["dumps"] + [{"addr": "0x8014CC74", "length": 52, "status": "ok", "data": entry.hex()}])
        row = tool.rows_from_samples([json.dumps(with_entry)])[0]
        self.assertEqual(row["names"], {"family": "82bf814081400000", "given": "82bf814081400000",
                                        "nickname": "82bf81408140814081400000"})
        self.assertEqual((row["player"], row["shiori"]), ({"month": 1, "day": 2, "blood": 1}, {"month": 0, "day": 0, "blood": 0}))
        logic = tool.rows_from_logic([json.dumps({"m": 21, "fc": 7699, "s": 0, "st": 0, "ss": 0, "v": 123})])
        self.assertEqual(logic[0]["mode_stage"], 0)

    def test_cli(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "s.json").write_text(json.dumps(script(SEG_A, SEG_B, unsupported=[
                {"side": "port", "stop": "x", "reason": "synthetic unsupported stop"}])))
            rows = trace(name_states)
            (root / "t.jsonl").write_text("".join(json.dumps({"m": r["game_mode"], "fc": r["frame_count"], "s": r["mode_stage"],
                                                               "st": r["mode_step"], "ss": r["substep"], "v": r["vsync"]}) + "\n" for r in rows))
            out = io.StringIO()
            with contextlib.redirect_stdout(out):
                self.assertEqual(tool.main(["compile", str(root / "s.json"), "--calibration", str(root / "t.jsonl"), "--format", "logic"]), 0)
            self.assertEqual(json.loads(out.getvalue())["input"], "5:RIGHT/3,20:START/2,54:CIRCLE+UP/1")
            out = io.StringIO()
            with contextlib.redirect_stdout(out):
                self.assertEqual(tool.main(["preflight", str(root / "s.json"), "--side", "port"]), 0)
            self.assertEqual(json.loads(out.getvalue())["expected_unsupported_stops"][0]["stop"], "x")
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(tool.main(["verify", str(root / "s.json"), "--trace", str(root / "t.jsonl"), "--format", "logic"]), 0)
            with contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(tool.main(["preflight", str(root / "missing.json")]), 2)

    def test_seed_preflight_lists_port_stops(self):
        result = tool.preflight(tool.load(SEED), "port")
        stops = [s["stop"] for s in result["expected_unsupported_stops"]]
        self.assertTrue(any("0x34" in s for s in stops))
        self.assertTrue(all(s["side"] == "port" for s in result["expected_unsupported_stops"]))


if __name__ == "__main__":
    unittest.main()
