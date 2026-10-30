#!/usr/bin/env python3
"""T-39023 (T-39018 WP-06): emulator player for tokimemo.input-script.v1.

A script holds pad edges keyed by game iteration. Each segment waits until a
predicate on projected state (game_mode, mode_stage, mode_step, substep) first
holds; that observed iteration is the anchor A. An event {at, buttons, hold} is
visible to the iteration whose post-update frame_count is N = A + at and stays
held through iteration N + hold - 1. The next segment's anchor is the first
observed iteration, at or after this segment's last press (or its anchor plus
run_for), in which the next predicate holds.

PCSX-Redux only lets emu.lua press and release buttons at vsync callbacks
(--input FRAME:BUTTON[+BUTTON]/HOLD). The player therefore needs a calibration
trace of the same route from the same start state: one row per vsync with the
observed post-update frame_count and state (an emu.py --sample-every 1 run with
--dump 800E7374:32, or a per-iteration log with vsync numbers). A press visible
to iteration N is applied at the last vsync whose observed frame_count is N-1;
its release at the last vsync observed at N+hold-1. Inputs change the route
after their own iteration only, so a long script is compiled segment by
segment: run with the events compiled so far, sample, compile the next segment
(``--through``). Nothing is guessed: a segment whose anchor or iterations are
not in the calibration trace stops compilation with its id.

A segment may also carry ``expect_state``: values the entry must hold on the
same state that expect_next checks (the first state after the last press that
leaves wait_until). The names are the native name-entry working buffers as hex
(name_entry_family_name 0x8014CC74, 8 bytes; given 0x8014CC7C, 8; nickname
0x8014CC84, 12) and the 0-based selections of name_entry_player_month/day/blood
(0x8014CC90/94/98) and name_entry_shiori_month/day/blood (0x8014CC9C/A0/A4), the
same encoding as the port entry controller. ``verify`` needs dump
8014CC74:52 in the trace for these; a missing value fails the segment.

Subcommands:
  preflight SCRIPT [--side native|port]   expected unsupported stops and segments
  compile SCRIPT --calibration FILE [--format samples|logic] [--through SEGMENT]
                                           prints JSON with the emu.py --input value
  verify SCRIPT --trace FILE [--format ...] check anchors, expect_next and expect_state on a run
"""
import argparse
import json
from pathlib import Path
import sys

BUTTONS = ["SELECT", "START", "UP", "RIGHT", "DOWN", "LEFT", "L2", "R2", "L1", "R1",
           "TRIANGLE", "CIRCLE", "CROSS", "SQUARE"]
GAME_STATE = 0x800E7374  # dump 800E7374:32 covers frame_count (+4) and mode/stage/step/substep (+0x14..+0x19)
PREDICATE_KEYS = ("game_mode", "mode_stage", "mode_step", "substep")
NAME_ENTRY = 0x8014CC74  # dump 8014CC74:52 covers the name buffers and both birthday selections
NAME_PARTS = (("family", 0, 8), ("given", 8, 8), ("nickname", 16, 12))
PROFILE_FIELDS = (("player", 0x1C), ("shiori", 0x28))  # month, day, blood: s32 each


class ScriptError(ValueError):
    pass


def load(path):
    script = json.loads(Path(path).read_text())
    errors = semantic_errors(script)
    if errors:
        raise ScriptError("; ".join(errors))
    return script


def semantic_errors(script):
    errors = []
    if script.get("schema") != "tokimemo.input-script.v1":
        return ["not a tokimemo.input-script.v1 script"]
    ids = [segment.get("id") for segment in script.get("segments", [])]
    if len(set(ids)) != len(ids):
        errors.append("duplicate segment id")
    for segment in script.get("segments", []):
        held = {}
        last = -1
        for event in segment.get("events", []):
            if event["at"] < last:
                errors.append("%s: events are not in iteration order" % segment["id"])
            last = event["at"]
            for button in event["buttons"]:
                if button not in BUTTONS:
                    errors.append("%s: unknown button %s" % (segment["id"], button))
                if held.get(button, -1) > event["at"]:
                    errors.append("%s: %s pressed again while still held" % (segment["id"], button))
                held[button] = event["at"] + event["hold"]
        if not segment.get("wait_until"):
            errors.append("%s: empty wait_until" % segment["id"])
        if segment.get("expect_state") and not segment.get("expect_next"):
            errors.append("%s: expect_state needs expect_next (both are checked on the same state)" % segment["id"])
        if segment.get("expect_state") and not segment.get("events"):
            errors.append("%s: expect_state needs at least one event" % segment["id"])
    return errors


def matches(predicate, row):
    for key in PREDICATE_KEYS:
        if key not in predicate:
            continue
        want = int(predicate[key], 16) if key == "game_mode" else predicate[key]
        if row[key] != want:
            return False
    return True


def state_mismatches(expect, state):
    """[] when every expect_state value holds in ``state``; else one message per differing value."""
    errors = []
    for group, wanted in sorted(expect.items()):
        observed = state.get(group)
        for name, want in sorted(wanted.items()):
            got = observed.get(name) if isinstance(observed, dict) else None
            if isinstance(got, str):
                got = got.lower()
            if isinstance(want, str):
                want = want.lower()
            if got != want:
                errors.append("%s.%s: expected %s, observed %s" % (group, name, want, "unavailable" if got is None else got))
    return errors


def entry_values(data):
    """Name buffers and birthday selections from dump 8014CC74:52."""
    values = {"names": {part: data[start:start + size].hex() for part, start, size in NAME_PARTS}}
    for group, start in PROFILE_FIELDS:
        values[group] = {name: int.from_bytes(data[start + 4 * i:start + 4 * i + 4], "little")
                         for i, name in enumerate(("month", "day", "blood"))}
    return values


def rows_from_samples(lines):
    """emu.py memory-samples.jsonl (needs dump 800E7374:32) -> vsync rows, restored samples only."""
    rows = []
    for line in lines:
        sample = json.loads(line)
        if sample.get("state_load_phase") == "before_restore":
            continue
        dump = next((d for d in sample["dumps"]
                     if (int(d["addr"], 0) if isinstance(d["addr"], str) else d["addr"]) == GAME_STATE), None)
        if dump is None or dump.get("status", "ok") != "ok":
            raise ScriptError("sample at vsync %s lacks dump 800E7374:32" % sample.get("frame"))
        game = bytes.fromhex(dump["data"])
        row = {"vsync": sample["frame"], "frame_count": int.from_bytes(game[4:8], "little"),
               "game_mode": game[0x14], "mode_stage": game[0x15], "mode_step": game[0x16], "substep": game[0x19]}
        entry = next((d for d in sample["dumps"]
                      if (int(d["addr"], 0) if isinstance(d["addr"], str) else d["addr"]) == NAME_ENTRY), None)
        if entry is not None and entry.get("status", "ok") == "ok" and len(bytes.fromhex(entry["data"])) >= 0x34:
            row.update(entry_values(bytes.fromhex(entry["data"])))
        rows.append(row)
    return rows


def rows_from_logic(lines):
    """Per-iteration log with vsync numbers ({m, s, st, ss, fc, v}, newgame-golden-v1 logic.jsonl)."""
    return [{"vsync": r["v"], "frame_count": r["fc"], "game_mode": r["m"], "mode_stage": r["s"],
             "mode_step": r["st"], "substep": r["ss"]} for r in map(json.loads, lines)]


def load_rows(path, fmt):
    lines = [line for line in Path(path).read_text().splitlines() if line.strip()]
    rows = rows_from_samples(lines) if fmt == "samples" else rows_from_logic(lines)
    for a, b in zip(rows, rows[1:]):
        if b["vsync"] <= a["vsync"] or b["frame_count"] < a["frame_count"]:
            raise ScriptError("calibration rows are not ordered by vsync with monotonic frame_count")
    if not rows:
        raise ScriptError("empty calibration trace")
    return rows


def walk_anchors(script, rows, through=None):
    """(segment id -> (anchor row index, anchor frame_count), stop message or None), in order,
    up to and including ``through``; the walk stops at the first segment that cannot anchor."""
    result = {}
    start = 0
    for segment in script["segments"]:
        index = next((i for i in range(start, len(rows)) if matches(segment["wait_until"], rows[i])), None)
        if index is None:
            return result, "segment %s: wait_until never holds in the trace" % segment["id"]
        anchor = rows[index]["frame_count"]
        timeout = segment.get("timeout_iterations")
        if timeout is not None and start < len(rows) and anchor - rows[start]["frame_count"] > timeout:
            return result, ("segment %s: wait_until held only after %d iterations (timeout %d)"
                            % (segment["id"], anchor - rows[start]["frame_count"], timeout))
        result[segment["id"]] = (index, anchor)
        if segment["id"] == through:
            break
        # The next segment anchors at or after this one's last press (or anchor + run_for).
        end = max([anchor + e["at"] for e in segment["events"]] + [anchor + segment.get("run_for", 0)])
        start = next((i for i in range(index, len(rows)) if rows[i]["frame_count"] >= end), len(rows))
    if through is not None and through not in result:
        return result, "unknown segment " + through
    return result, None


def anchors(script, rows, through=None):
    result, stop = walk_anchors(script, rows, through)
    if stop:
        raise ScriptError(stop)
    return result


def last_vsync_at(rows, start, frame_count):
    """Vsync of the last row from ``start`` whose observed frame_count is ``frame_count``."""
    found = None
    for row in rows[start:]:
        if row["frame_count"] == frame_count:
            found = row["vsync"]
        elif row["frame_count"] > frame_count:
            break
    return found


def compile_events(script, rows, through=None):
    events = []
    found = anchors(script, rows, through)
    for segment in script["segments"]:
        if segment["id"] not in found:
            break
        index, anchor = found[segment["id"]]
        for event in segment["events"]:
            visible = anchor + event["at"]
            press = last_vsync_at(rows, index, visible - 1)
            release = last_vsync_at(rows, index, visible + event["hold"] - 1)
            if press is None or release is None:
                raise ScriptError("segment %s: iterations %d..%d are not in the calibration trace"
                                  % (segment["id"], visible - 1, visible + event["hold"] - 1))
            events.append({"frame": press, "buttons": list(event["buttons"]), "hold": max(1, release - press),
                           "segment": segment["id"], "visible_frame_count": visible})
    return events


def input_argument(events):
    """emu.py --input value; the emulator holds each press for HOLD vsyncs."""
    frames = [e["frame"] for e in events]
    if len(set(frames)) != len(frames):
        raise ScriptError("two presses land on one vsync; emu.py --input would merge them")
    return ",".join("%d:%s/%d" % (e["frame"], "+".join(e["buttons"]), e["hold"]) for e in events)


def verify(script, rows):
    """Check every reached segment's expect_next on a recorded run: the first state after the
    segment's last press that differs from its wait_until state must satisfy expect_next, and
    the same state must hold every expect_state value (names, birthdays, blood)."""
    report = []
    found, stop = walk_anchors(script, rows)
    for segment in script["segments"]:
        if segment["id"] not in found:
            report.append({"segment": segment["id"], "status": "not_reached"})
            continue
        index, anchor = found[segment["id"]]
        expect = segment.get("expect_next")
        if not expect or not segment["events"]:
            report.append({"segment": segment["id"], "status": "anchored", "anchor": anchor})
            continue
        last_press = anchor + segment["events"][-1]["at"]
        nxt = next((r for r in rows[index:] if r["frame_count"] >= last_press and not matches(segment["wait_until"], r)), None)
        status = "ok" if nxt is not None and matches(expect, nxt) else "expect_next_failed"
        row = {"segment": segment["id"], "status": status, "anchor": anchor,
               "next": None if nxt is None else {k: nxt[k] for k in ("frame_count",) + PREDICATE_KEYS}}
        if status == "ok" and segment.get("expect_state"):
            mismatches = state_mismatches(segment["expect_state"], nxt)
            if mismatches:
                row.update(status="expect_state_failed", mismatches=mismatches)
        report.append(row)
    result = {"segments": report, "passed": stop is None and all(r["status"] in ("ok", "anchored") for r in report)}
    if stop:
        result["stop"] = stop
    return result


def preflight(script, side=None):
    stops = [u for u in script["unsupported"] if side is None or u["side"] == side]
    return {"script": script["name"], "segments": [s["id"] for s in script["segments"]],
            "checkpoints": script["checkpoints"], "expected_unsupported_stops": stops}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("preflight")
    p.add_argument("script", type=Path)
    p.add_argument("--side", choices=["native", "port"])
    for name in ("compile", "verify"):
        p = sub.add_parser(name)
        p.add_argument("script", type=Path)
        p.add_argument("--calibration" if name == "compile" else "--trace", dest="rows", type=Path, required=True)
        p.add_argument("--format", choices=["samples", "logic"], default="samples")
    sub.choices["compile"].add_argument("--through", help="last segment to compile")
    args = parser.parse_args(argv)
    try:
        script = load(args.script)
        if args.cmd == "preflight":
            print(json.dumps(preflight(script, args.side), indent=1))
            return 0
        rows = load_rows(args.rows, args.format)
        if args.cmd == "verify":
            result = verify(script, rows)
            print(json.dumps(result, indent=1))
            return 0 if result["passed"] else 1
        events = compile_events(script, rows, args.through)
        print(json.dumps({"input": input_argument(events), "events": events}, indent=1))
        return 0
    except (ScriptError, OSError, KeyError, ValueError) as error:
        print("input_script: %s" % error, file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
