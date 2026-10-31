"""T0 tests: input-script v2 rule, expand, strategy compiler (synthetic states only).

Run (Docker): python3 -I -m unittest discover -s tools/emu -p 'test_route_script.py'
Writes nothing unless ROUTE_PARITY_WRITE=1, which regenerates
port/godot/tests/fixtures/route_rule_parity.json for the GDScript player test.
"""
import copy
import json
import os
from pathlib import Path
import sys
import unittest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / "tools/port"))
import route_script as rs  # noqa: E402
import route_strategy  # noqa: E402

PARITY = ROOT / "port/godot/tests/fixtures/route_rule_parity.json"
STRATEGY = ROOT / "config/port/route-strategies/three_year_default.json"
COMPILED = ROOT / "config/port/input-scripts/three_year_default.json"
CONTINUOUS = ROOT / "config/port/input-scripts/three_year_default_continuous.json"
SELECT = {"game_mode": "0x34", "mode_stage": 1, "mode_step": 0}


def script(segments, checkpoints=(), unsupported=(), unsampled_ok=None):
    out = {"schema": rs.SCHEMA_V2, "name": "synthetic", "description": "synthetic unit script",
           "provenance": {"source": "unit test fixture", "derivation": "hand-written"},
           "segments": list(segments), "checkpoints": list(checkpoints), "unsupported": list(unsupported)}
    if unsampled_ok is not None:
        out["unsampled_ok"] = unsampled_ok
    return out


def row(fc, mode, stage=0, step=0, **extra):
    out = {"frame_count": fc, "game_mode": mode, "mode_stage": stage, "mode_step": step, "substep": 0}
    out.update(extra)
    return out


def week_rows(start=1000, cursor=0, icon_count=14, select_len=40, week_len=300):
    """Command select, mode 0x30.. school days, back to command select; menu_cursor follows RIGHT presses."""
    rows = []
    fc = start
    for _ in range(select_len):
        rows.append(row(fc, 0x34, 1, 0, menu_cursor=cursor, icon_count=icon_count, month=4, day_count=0))
        fc += 1
    for i in range(week_len):
        rows.append(row(fc, 0x30 if i < 5 else (0x31 if i % 50 < 10 else 0x40), month=4, day_count=i // 50))
        fc += 1
    for _ in range(5):
        rows.append(row(fc, 0x34, 0, 2, month=4, day_count=6))
        fc += 1
    for _ in range(40):
        rows.append(row(fc, 0x34, 1, 0, menu_cursor=cursor, icon_count=icon_count, month=4, day_count=6))
        fc += 1
    return rows


def parity_cases():
    """(name, script, rows) shared with the GDScript player test."""
    cases = []
    s1 = script([
        {"id": "w1-cursor", "kind": "pulse", "wait_until": dict(SELECT), "only_if": [dict(SELECT, icon_count=14, menu_cursor=0)],
         "pulse": {"buttons": ["RIGHT"], "hold": 2, "every": 10, "offset": 5, "max": 14, "until": dict(SELECT, menu_cursor=0)}},
        {"id": "w1-confirm", "kind": "pulse", "wait_until": dict(SELECT),
         "pulse": {"buttons": ["CIRCLE"], "hold": 2, "every": 60, "offset": 30, "max": 3, "until": {"game_mode": "0x30"}},
         "expect_next": {"game_mode": "0x30"}},
        {"id": "w1-week", "kind": "pulse", "wait_until": {"game_mode": "0x30"}, "timeout_iterations": 50,
         "pulse": {"buttons": ["CIRCLE"], "hold": 2, "every": 40, "offset": 40, "max": 100, "until": dict(SELECT),
                   "skip_when": [{"game_mode": "0x34"}], "choose": [{"when": {"game_mode": "0x40"}, "buttons": ["CROSS"]}]}},
        {"id": "w2-save", "kind": "save_relaunch", "slot": 0, "wait_until": dict(SELECT)}],
        checkpoints=[{"id": "select", "when": dict(SELECT), "recurring": True, "limit": 5},
                     {"id": "first-31", "when": {"game_mode": "0x31", "mode_stage": 0, "mode_step": 0}},
                     {"id": "enter-40", "when": {"game_mode": "0x40", "mode_stage": 0, "mode_step": 0}, "recurring": True, "limit": 2}])
    rows = week_rows()
    # The confirm lands at select + 30: the next rows are already mode 0x30 in this synthetic trace only after it.
    cases.append(("week-cycle", s1, rows))
    s2 = script([
        {"id": "move", "kind": "pulse", "wait_until": dict(SELECT), "only_if": [dict(SELECT, menu_cursor=3)],
         "pulse": {"buttons": ["RIGHT"], "hold": 2, "every": 10, "max": 2, "until": dict(SELECT, menu_cursor=5)}},
        {"id": "input", "wait_until": dict(SELECT), "events": [{"at": 3, "buttons": ["CIRCLE", "UP"], "hold": 4}],
         "expect_next": {"game_mode": "0x30"}}])
    rows2 = [row(500 + i, 0x34, 1, 0, menu_cursor=0) for i in range(4)] + [row(504 + i, 0x30) for i in range(5)]
    cases.append(("only-if-skip-and-events", s2, rows2))
    s3 = script([{"id": "stuck", "kind": "pulse", "wait_until": {"game_mode": "0x30"},
                  "pulse": {"buttons": ["CIRCLE"], "hold": 1, "every": 3, "max": 2, "until": dict(SELECT)}}])
    cases.append(("exhausted", s3, [row(10 + i, 0x30) for i in range(20)]))
    s4 = script([{"id": "gap", "wait_until": {"game_mode": "0x30"}, "events": [{"at": 2, "buttons": ["CIRCLE"], "hold": 1}]}])
    cases.append(("frame-jump", s4, [row(10, 0x30), row(14, 0x30)]))
    load = [{"id": "load", "kind": "pulse", "wait_until": {"game_mode": "0x46"},
             "pulse": {"buttons": ["CIRCLE"], "hold": 2, "every": 10, "max": 3, "until": {"game_mode": "0x34"}}}]
    s5 = script(load, unsampled_ok=[{"when": {"game_mode": "0x46", "mode_stage": 0, "mode_step": 0},
                                     "evidence": "synthetic: the loading state before the missed iteration"}])
    rows5 = [row(20, 0x46, pad_held=0), row(21, 0x46, pad_held=0), row(22, 0x46, 1, pad_held=0x20)] + \
        [row(23 + i, 0x46, 1, pad_held=0) for i in range(8)] + [row(31, 0x46, 1, pad_held=0x20), row(32, 0x34, pad_held=0x20)]
    cases.append(("unsampled-pulse", s5, rows5))
    # The same miss after a state the script does not list: an input slip, not a retry 10 iterations later.
    s6 = script(load, unsampled_ok=[{"when": {"game_mode": "0x02"}, "evidence": "synthetic: another loading state"}])
    cases.append(("unsampled-pulse-not-allowed", s6, copy.deepcopy(rows5)))
    return cases


def run_case(sc, rows):
    return rs.simulate(sc, rows)


class RuleTests(unittest.TestCase):
    def test_week_cycle(self):
        name, sc, rows = parity_cases()[0]
        result = run_case(sc, rows)
        self.assertEqual(result["failure"], "")
        self.assertEqual(result["stop"], "segment w2-save: save_relaunch slot 0")
        presses = [d for d in result["delivered"] if d["pressed"]]
        self.assertEqual(presses[0], {"frame_count": 1030, "button": "CIRCLE", "pressed": True})
        # No cursor move: the cursor is already on the command.
        self.assertNotIn("RIGHT", [d["button"] for d in presses])
        # choose: CROSS while mode 0x40 is observed; never a press while 0x34 is observed.
        self.assertIn("CROSS", [d["button"] for d in presses])
        by_fc = {r["frame_count"]: r for r in rows}
        for d in presses[1:]:
            self.assertNotEqual(by_fc[d["frame_count"] - 1]["game_mode"], 0x34)
        ids = [c["id"] for c in result["checkpoints"]]
        self.assertEqual(ids[:2], ["select-001", "first-31"])
        self.assertIn("select-002", ids)
        self.assertEqual(sum(1 for i in ids if i.startswith("enter-40")), 2)

    def test_only_if_skips_and_events_fire(self):
        _, sc, rows = parity_cases()[1]
        result = run_case(sc, rows)
        self.assertEqual(result["skipped"], ["move"])
        self.assertEqual([d for d in result["delivered"] if d["pressed"]],
                         [{"frame_count": 503, "button": "CIRCLE", "pressed": True}, {"frame_count": 503, "button": "UP", "pressed": True}])

    def test_pulse_exhausted(self):
        _, sc, rows = parity_cases()[2]
        self.assertTrue(run_case(sc, rows)["failure"].startswith("segment stuck: pulse exhausted after 2 presses"))

    def test_frame_jump_with_edge_due_fails(self):
        _, sc, rows = parity_cases()[3]
        self.assertIn("ran without a boundary while an edge was due", run_case(sc, rows)["failure"])

    def test_input_slip(self):
        sc = script([{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": [{"at": 2, "buttons": ["CIRCLE"], "hold": 2}]}])
        rows = [row(10, 0x34, pad_held=0), row(11, 0x34, pad_held=0), row(12, 0x34, pad_held=0)]
        self.assertTrue(run_case(sc, rows)["failure"].startswith("input slip: CIRCLE due for iteration 12"))
        rows[2]["pad_held"] = 0x20
        self.assertEqual(run_case(sc, rows)["failure"], "")

    def test_unsampled_pulse_is_recorded_not_fatal(self):
        _, sc, rows = parity_cases()[4]
        result = run_case(sc, rows)
        self.assertEqual(result["failure"], "")
        self.assertEqual(result["unsampled"], [{"frame_count": 21, "button": "CIRCLE"}])

    def test_unsampled_pulse_outside_unsampled_ok_is_a_slip(self):
        _, sc, rows = parity_cases()[5]
        result = run_case(sc, rows)
        self.assertEqual(result["failure"],
                         "input slip: CIRCLE due for iteration 21 is not in the sampled pad 0000 (previous boundary 46/0/0/0)")
        self.assertEqual(result["unsampled"], [])
        del sc["unsampled_ok"]
        self.assertTrue(run_case(sc, rows)["failure"].startswith("input slip: CIRCLE due for iteration 21"))

    def test_input_segment_slip_ignores_unsampled_ok(self):
        sc = script([{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": [{"at": 2, "buttons": ["CIRCLE"], "hold": 2}]}],
                    unsampled_ok=[{"when": {"game_mode": "0x34"}, "evidence": "synthetic: would cover the input press"}])
        rows = [row(10, 0x34, pad_held=0), row(11, 0x34, pad_held=0), row(12, 0x34, pad_held=0)]
        self.assertTrue(run_case(sc, rows)["failure"].startswith("input slip: CIRCLE due for iteration 12"))

    def test_checkpoint_rising_edge_only(self):
        sc = script([{"id": "x", "wait_until": {"game_mode": "0x99"}, "events": []}],
                    checkpoints=[{"id": "s", "when": dict(SELECT), "recurring": True}])
        rows = [row(1, 0x34, 1, 0), row(2, 0x34, 1, 0), row(3, 0x30), row(4, 0x34, 1, 0)]
        self.assertEqual([c["id"] for c in run_case(sc, rows)["checkpoints"]], ["s-001", "s-002"])


class VerdictTests(unittest.TestCase):
    """A pinned stop passes only at the segment it was pinned for."""

    def setUp(self):
        self.script = json.loads(COMPILED.read_text())
        self.text = "source_transaction: required route content capability is unavailable for selected command"

    def test_pinned_at_its_segment_passes(self):
        passed, row = rs.verdict(self.script, "port", "app_failure", self.text, "w001-confirm")
        self.assertTrue(passed)
        self.assertEqual(row["at_segment"], "w001-confirm")

    def test_same_text_at_later_segment_fails(self):
        for segment in ("w005-confirm", "w040-confirm", "w001-week"):
            self.assertEqual(rs.verdict(self.script, "port", "app_failure", self.text, segment), (False, None), segment)

    def test_wrong_side_and_other_stops_fail(self):
        self.assertFalse(rs.verdict(self.script, "native", "app_failure", self.text, "w001-confirm")[0])
        self.assertFalse(rs.verdict(self.script, "port", "limit", "route frame limit 10", "w001-confirm")[0])
        self.assertTrue(rs.verdict(self.script, "port", "script_complete", "", "")[0])

    def test_hover_pulse_pinned_only_at_first_planned_move(self):
        text = "publication: hover pulse scale 0x1191 needs the scaled sprite path, which this HUD does not rasterize"
        self.assertTrue(rs.verdict(self.script, "port", "app_failure", text, "w003-cursor")[0])
        self.assertFalse(rs.verdict(self.script, "port", "app_failure", text, "w007-cursor")[0])

    def test_verdict_subcommand_on_a_run(self):
        import tempfile
        with tempfile.TemporaryDirectory() as tmp:
            run = Path(tmp) / "route.json"
            run.write_text(json.dumps({"side": "port", "reason": "app_failure", "stop": self.text,
                                       "segment_at_stop": "w005-confirm", "passed": True}))
            self.assertEqual(rs.main(["verdict", str(COMPILED), "--run", str(run)]), 1)
            run.write_text(json.dumps({"side": "port", "reason": "app_failure", "stop": self.text,
                                       "segment_at_stop": "w001-confirm", "passed": True}))
            self.assertEqual(rs.main(["verdict", str(COMPILED), "--run", str(run)]), 0)


class ValidationTests(unittest.TestCase):
    def errors(self, sc):
        return rs.semantic_errors(sc)

    def test_unknown_predicate_field(self):
        sc = script([{"id": "a", "wait_until": {"game_mode": "0x34", "weather": 1}, "events": []}])
        self.assertTrue(any("unknown predicate field weather" in e for e in self.errors(sc)))

    def test_checkpoint_needs_full_mode_key(self):
        sc = script([{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": []}],
                    checkpoints=[{"id": "c", "when": {"game_mode": "0x34"}}])
        self.assertTrue(any("checkpoint c: when must name" in e for e in self.errors(sc)))

    def test_pulse_hold_must_be_below_every(self):
        sc = script([{"id": "p", "kind": "pulse", "wait_until": {"game_mode": "0x30"},
                      "pulse": {"buttons": ["CIRCLE"], "hold": 5, "every": 5, "max": 1, "until": {"game_mode": "0x34"}}}])
        self.assertTrue(any("hold must be" in e for e in self.errors(sc)))

    def test_pulse_offset_zero_refused(self):
        sc = script([{"id": "p", "kind": "pulse", "wait_until": {"game_mode": "0x30"},
                      "pulse": {"buttons": ["CIRCLE"], "hold": 1, "every": 5, "offset": 0, "max": 1, "until": {"game_mode": "0x34"}}}])
        self.assertTrue(any("offset >= 1" in e for e in self.errors(sc)))

    def test_bad_game_mode_and_range(self):
        sc = script([{"id": "a", "wait_until": {"game_mode": "34", "month": 14}, "events": []}])
        errors = self.errors(sc)
        self.assertTrue(any("game_mode must be" in e for e in errors))
        self.assertTrue(any("month must be" in e for e in errors))

    def test_players_refuse_unexpanded(self):
        sc = script([{"id": "r", "kind": "repeat", "count": 2, "body": [{"id": "w{i}", "wait_until": {"game_mode": "0x34"}, "events": []}]}])
        with self.assertRaises(rs.ScriptError):
            rs.Rule(sc)
        self.assertTrue(any("repeat segment in an expanded script" in e for e in rs.semantic_errors(sc, expanded=True)))

    def test_expand_unrolls_and_is_stable(self):
        sc = script([{"id": "r", "kind": "repeat", "first": 7, "count": 2,
                      "body": [{"id": "w{i}-a", "wait_until": {"game_mode": "0x34"}, "events": [], "note": "week {i}"}]}])
        out = rs.expand(sc)
        self.assertEqual([s["id"] for s in out["segments"]], ["w007-a", "w008-a"])
        self.assertEqual(rs.expand(out), out)

    def test_expand_refuses_duplicate_ids(self):
        sc = script([{"id": "r", "kind": "repeat", "count": 2, "body": [{"id": "w-a", "wait_until": {"game_mode": "0x34"}, "events": []}]}])
        self.assertTrue(any("has no {i}" in e for e in self.errors(sc)))

    def test_unsupported_row_needs_stop_shape_and_segment(self):
        segments = [{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": []}]
        ok = {"side": "port", "stop": "publication: some failure", "at_segment": "a", "reason": "a reason long enough"}
        self.assertEqual(self.errors(script(segments, unsupported=[ok])), [])
        prose = dict(ok, stop="mode 0x39 day notice, 0x30 save poll, PRODUCTION_REJECTIONS overlay modes")
        self.assertTrue(any("is not a \"<gate>: <failure>\"" in e for e in self.errors(script(segments, unsupported=[prose]))))
        unpinned = dict(ok)
        del unpinned["at_segment"]
        self.assertTrue(any("at_segment None" in e for e in self.errors(script(segments, unsupported=[unpinned]))))
        elsewhere = dict(ok, at_segment="w009-week")
        self.assertTrue(any("at_segment 'w009-week'" in e for e in self.errors(script(segments, unsupported=[elsewhere]))))

    def test_known_gaps_and_unsampled_ok_shape(self):
        segments = [{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": []}]
        sc = script(segments, unsampled_ok=[{"when": {"mode_stage": 1}, "evidence": "synthetic evidence"}])
        self.assertTrue(any("must name game_mode" in e for e in self.errors(sc)))
        sc = script(segments, unsampled_ok=[{"when": {"game_mode": "0x46"}}])
        self.assertTrue(any("needs exactly when and evidence" in e for e in self.errors(sc)))
        sc = script(segments)
        sc["known_gaps"] = [{"side": "port", "gap": "0x39 day notice", "reason": "no handler yet in the port"}]
        self.assertEqual(self.errors(sc), [])
        sc["known_gaps"] = [{"side": "port", "stop": "0x39 day notice", "reason": "no handler yet in the port"}]
        self.assertTrue(any("known_gaps rows" in e for e in self.errors(sc)))

    def test_v1_seed_still_accepted(self):
        seed = json.loads((ROOT / "config/port/input-scripts/new_game_to_school_handoff.json").read_text())
        self.assertEqual(rs.semantic_errors(seed), [])


class StrategyTests(unittest.TestCase):
    def setUp(self):
        self.strategy = json.loads(STRATEGY.read_text())

    def test_compiled_script_is_current(self):
        self.assertEqual(route_strategy.compile_strategy(self.strategy), json.loads(COMPILED.read_text()))
        self.assertEqual(route_strategy.compile_strategy(self.strategy, continuous=True), json.loads(CONTINUOUS.read_text()))

    def test_source_form_expands_to_compiled(self):
        strategy = copy.deepcopy(self.strategy)
        strategy["plan"] = [{"from": "1995-04", "cycle": [0, 0, 0, 1, 1, 1, 1, 7]}]
        direct = route_strategy.compile_strategy(strategy, weeks=20)
        source = route_strategy.compile_strategy(strategy, weeks=20, source_form=True)
        self.assertTrue(any(s.get("kind") == "repeat" for s in source["segments"]))
        self.assertEqual(rs.expand(source)["segments"], direct["segments"])

    def test_week_dates_and_first_confirm(self):
        script_ = route_strategy.compile_strategy(self.strategy, weeks=3)
        plan = script_["provenance"]["plan"]
        self.assertEqual([p["select_date"] for p in plan], ["1995-04-04", "1995-04-09", "1995-04-16"])
        confirm = next(s for s in script_["segments"] if s["id"] == "w001-confirm")
        self.assertEqual(confirm["pulse"]["offset"], 30)
        self.assertEqual(confirm["expect_next"], {"game_mode": "0x30", "mode_stage": 0, "mode_step": 0})

    def test_save_relaunch_placement(self):
        segments = route_strategy.compile_strategy(self.strategy)["segments"]
        saves = [s["id"] for s in segments if s.get("kind") == "save_relaunch"]
        self.assertEqual(saves[:3], ["w004-save", "w017-save", "w030-save"])
        self.assertFalse(any(s.get("kind") == "save_relaunch"
                             for s in route_strategy.compile_strategy(self.strategy, continuous=True)["segments"]))

    def test_pins_and_gaps(self):
        script_ = route_strategy.compile_strategy(self.strategy)
        pins = {(r["side"], r["at_segment"]) for r in script_["unsupported"]}
        self.assertEqual(pins, {("port", "w001-confirm"), ("port", "w003-cursor"), ("port", "w004-save"), ("native", "w004-save")})
        self.assertTrue(all(rs.STOP_SHAPE.match(r["stop"]) for r in script_["unsupported"]))
        self.assertIn("PRODUCTION_REJECTIONS", script_["known_gaps"][0]["gap"])
        continuous = route_strategy.compile_strategy(self.strategy, continuous=True)
        self.assertEqual({r["at_segment"] for r in continuous["unsupported"]}, {"w001-confirm", "w003-cursor"})
        self.assertEqual(sum(1 for g in continuous["known_gaps"] if g["gap"].startswith("save_relaunch:")), 2)
        self.assertEqual([u["when"] for u in script_["unsampled_ok"]],
                         [{"game_mode": "0x02"}, {"game_mode": "0x46", "mode_stage": 1, "mode_step": 0}])

    def test_club_command_refused(self):
        strategy = copy.deepcopy(self.strategy)
        strategy["plan"][0]["cycle"] = [4]
        with self.assertRaises(route_strategy.StrategyError):
            route_strategy.compile_strategy(strategy, weeks=2)

    def test_plan_before_first_entry_refused(self):
        strategy = copy.deepcopy(self.strategy)
        strategy["plan"][0]["from"] = "1995-05"
        with self.assertRaises(route_strategy.StrategyError):
            route_strategy.compile_strategy(strategy, weeks=1)


class ParityFixtureTests(unittest.TestCase):
    def test_fixture_matches_rule(self):
        cases = []
        for name, sc, rows in parity_cases():
            result = run_case(sc, rows)
            cases.append({"name": name, "script": sc, "rows": rows,
                          "expected": {k: result[k] for k in ("delivered", "anchors", "skipped", "unsampled", "checkpoints", "failure", "stop")}})
        doc = {"note": "generated by tools/emu/test_route_script.py (ROUTE_PARITY_WRITE=1); the GDScript player must give the same results",
               "cases": cases}
        if os.environ.get("ROUTE_PARITY_WRITE") == "1":
            PARITY.write_text(json.dumps(doc, indent=1) + "\n")
        self.assertEqual(json.loads(PARITY.read_text()), json.loads(json.dumps(doc)))


if __name__ == "__main__":
    unittest.main()
