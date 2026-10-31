#!/usr/bin/env python3
"""Route-driver input scripts: tokimemo.input-script.v2 (v1 is still accepted).

v2 keeps the v1 timing rule (tools/emu/input_script.py) and adds what a long
route needs. Every player (port: port/godot/tests/route/route_script_player.gd,
emulator: tools/emu/route_driver/player.lua) applies the same rule at the same
point: a boundary is the moment after one game iteration has finished its logic
and before the next one reads the pad. The player sees the state S of that
finished iteration (post-update frame_count f) and decides which buttons are
held for iteration f + 1. Nothing else is written into the game.

Predicates (wait_until, expect_next, until, skip_when, checkpoint when) may name
game_mode ("0xNN"), mode_stage, mode_step, substep, and the calendar and cursor
fields year, month, day, weekday, day_count (GameState +0x03E..+0x042) and
menu_cursor (GameState +0x1093, s8) and icon_count (+0xF6D). A field the
predicate does not name is not checked.

Any segment may carry ``only_if`` (predicates): when none holds at the anchor,
the segment ends there and sends nothing (recorded in ``skipped``).

Segment kinds:
  input (default)  the v1 segment: anchor, events {at, buttons, hold}, run_for,
                   expect_next.
  pulse            after the anchor A, press pulse.buttons for pulse.hold
                   iterations at A + offset + k * every (k = 0 .. max - 1,
                   offset >= 1, default 1). At each boundary from the anchor on,
                   first: when ``until`` holds the segment ends (no further
                   pulses); then a pulse due for the next iteration is skipped
                   (k still advances) when any ``skip_when`` predicate holds, and
                   otherwise presses the buttons of the first ``choose`` entry
                   whose ``when`` holds (default pulse.buttons). When pulse ``max``
                   was reached and ``until`` has not held ``every`` iterations
                   after the last one, the run stops with "pulse exhausted". A
                   pulse with until already true at the anchor sends nothing.

Delivery check: a row's ``pad_held`` (the mask the game sampled for that
iteration) must contain every button due for it. A miss stops the run
("input slip"), with one exception: a pulse press missed in iteration f is
recorded in ``unsampled`` and the run goes on when the boundary state just
before f (the state after iteration f - 1, when the pad for f is read) matches
one of the script's ``unsampled_ok`` predicates. Each of those names a loading
state in which the original was observed not to sample the pad, with the run
and frame as evidence. Any other miss is an input slip, so a port whose input
owner loses a press fails instead of retrying it later.
  save_relaunch    save to card slot ``slot`` and relaunch. No player implements it
                   yet; each side stops there with its pinned refusal (see
                   ``unsupported``).

Pinned stops (``unsupported``): {side, stop, at_segment, reason, ticket?}. A run
passes only when the script completes or when its stop text equals ``stop``
AND the segment the player was on when it stopped is ``at_segment``, so the
same text at another week or segment fails. ``stop`` has the "<gate>: <failure>"
shape of the stop text the player writes. Gaps that have no exact stop text yet
go in ``known_gaps`` ({side, gap, reason, ticket?}); they are listed by
preflight and never make a run pass.
  repeat           only in source scripts: ``body`` (segments) repeated ``count``
                   times; ``{i}`` (``first`` .. first + count - 1, default 1, zero-padded to 3) is replaced.
                   ``expand`` unrolls it; players refuse unexpanded scripts.

Checkpoints (v2: objects): {id, when, recurring?, limit?, ram?, save_state?}.
They are observed, never acted on: a checkpoint fires at a rising edge of
``when`` (holds at this boundary, did not hold at the previous one). A single
checkpoint fires once with its id; a recurring one fires on every rising edge
as "<id>-NNN" (1-based) up to ``limit``.

Subcommands:
  validate SCRIPT              semantic checks (v1 or v2)
  expand SCRIPT [--out FILE]   unroll repeat segments; output must re-expand to itself
  preflight SCRIPT [--side native|port]
  verdict SCRIPT --run ROUTE_JSON
      recompute a run's pass/fail from its route.json (reason, stop, segment at
      the stop) against the script's pinned stops
  lua SCRIPT --out FILE        SCRIPT = {...} for tools/emu/route_driver/player.lua
  simulate SCRIPT --trace FILE [--delivered FILE]
      re-run the rule on a recorded boundary trace (route-trace.jsonl written by
      either player) and, with --delivered, check that the player sent exactly
      the edges the rule gives.
"""
import argparse
import copy
import json
import re
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import input_script as v1  # noqa: E402

SCHEMA_V1 = "tokimemo.input-script.v1"
SCHEMA_V2 = "tokimemo.input-script.v2"
BUTTONS = v1.BUTTONS
MASKS = {"L2": 0x1, "R2": 0x2, "L1": 0x4, "R1": 0x8, "TRIANGLE": 0x10, "CIRCLE": 0x20, "CROSS": 0x40, "SQUARE": 0x80,
         "SELECT": 0x100, "START": 0x800, "UP": 0x1000, "RIGHT": 0x2000, "DOWN": 0x4000, "LEFT": 0x8000}
PREDICATE_KEYS = ("game_mode", "mode_stage", "mode_step", "substep", "year", "month", "day", "weekday",
                  "day_count", "menu_cursor", "icon_count")
RANGES = {"mode_stage": (0, 255), "mode_step": (0, 255), "substep": (0, 255), "year": (0, 255), "month": (1, 13),
          "day": (1, 31), "weekday": (0, 6), "day_count": (0, 0xFFFF), "menu_cursor": (-1, 15), "icon_count": (0, 15)}
KINDS = ("input", "pulse", "save_relaunch", "repeat")
CHECKPOINT_KEYS = {"id", "when", "recurring", "limit", "ram", "save_state", "note"}
SEGMENT_KEYS = {"id", "kind", "note", "wait_until", "timeout_iterations", "run_for", "events", "expect_next",
                "expect_state", "pulse", "slot", "count", "first", "body", "only_if"}
PULSE_KEYS = {"buttons", "hold", "every", "offset", "max", "until", "skip_when", "choose"}
SCRIPT_KEYS = {"schema", "name", "description", "provenance", "start", "segments", "checkpoints", "unsupported",
               "known_gaps", "unsampled_ok"}
UNSUPPORTED_KEYS = {"side", "stop", "at_segment", "reason", "ticket"}
KNOWN_GAP_KEYS = {"side", "gap", "reason", "ticket"}
UNSAMPLED_OK_KEYS = {"when", "evidence"}
# "<gate>: <failure>": the stop text a player writes (app failure gate, "input script", "save_relaunch").
STOP_SHAPE = re.compile(r"^[a-z][a-z0-9_ ]*: \S.*$")


class ScriptError(ValueError):
    pass


def predicate_errors(where, predicate):
    if not isinstance(predicate, dict) or not predicate:
        return ["%s: predicate must be a non-empty object" % where]
    errors = []
    for key, value in predicate.items():
        if key not in PREDICATE_KEYS:
            errors.append("%s: unknown predicate field %s" % (where, key))
        elif key == "game_mode":
            if not (isinstance(value, str) and len(value) == 4 and value.startswith("0x")):
                errors.append("%s: game_mode must be \"0xNN\"" % where)
            else:
                try:
                    int(value, 16)
                except ValueError:
                    errors.append("%s: game_mode must be \"0xNN\"" % where)
        else:
            low, high = RANGES[key]
            if not isinstance(value, int) or isinstance(value, bool) or not low <= value <= high:
                errors.append("%s: %s must be an integer in %d..%d" % (where, key, low, high))
    return errors


def ptext(predicate):
    """Canonical predicate text for messages (identical in every player)."""
    return ",".join("%s=%s" % (k, predicate[k] if k == "game_mode" else int(predicate[k]))
                    for k in PREDICATE_KEYS if k in predicate)


def key_text(state):
    return "%02x/%d/%d/%d" % tuple(int(state.get(k, -1)) for k in ("game_mode", "mode_stage", "mode_step", "substep"))


def matches(predicate, state):
    for key in PREDICATE_KEYS:
        if key not in predicate:
            continue
        want = int(predicate[key], 16) if key == "game_mode" else predicate[key]
        if state.get(key) != want:
            return False
    return True


def _event_errors(sid, events):
    errors = []
    held = {}
    last = -1
    for event in events:
        if set(event) - {"at", "buttons", "hold", "note"} or not {"at", "buttons", "hold"} <= set(event):
            errors.append("%s: event needs exactly at, buttons, hold (and note)" % sid)
            continue
        if event["at"] < last:
            errors.append("%s: events are not in iteration order" % sid)
        last = event["at"]
        if not 1 <= event["hold"] <= 600:
            errors.append("%s: hold out of range" % sid)
        for button in event["buttons"]:
            if button not in BUTTONS:
                errors.append("%s: unknown button %s" % (sid, button))
            if held.get(button, -1) > event["at"]:
                errors.append("%s: %s pressed again while still held" % (sid, button))
            held[button] = event["at"] + event["hold"]
    return errors


def segment_errors(segment, expanded):
    sid = segment.get("id", "<no id>")
    kind = segment.get("kind", "input")
    errors = []
    if kind not in KINDS:
        return ["%s: unknown segment kind %s" % (sid, kind)]
    unknown = set(segment) - SEGMENT_KEYS
    if unknown:
        errors.append("%s: unknown segment fields %s" % (sid, sorted(unknown)))
    if kind == "repeat":
        if expanded:
            return ["%s: repeat segment in an expanded script (run expand first)" % sid]
        if not isinstance(segment.get("count"), int) or segment["count"] < 1 or not segment.get("body"):
            errors.append("%s: repeat needs count >= 1 and a body" % sid)
        for inner in segment.get("body", []):
            if "{i}" not in inner.get("id", ""):
                errors.append("%s: repeated segment id %s has no {i}" % (sid, inner.get("id")))
            errors += segment_errors(inner, expanded=True) if inner.get("kind") != "repeat" else [
                "%s: nested repeat" % sid]
        return errors
    if "wait_until" not in segment:
        errors.append("%s: empty wait_until" % sid)
    else:
        errors += predicate_errors(sid + ".wait_until", segment["wait_until"])
    if "expect_next" in segment:
        errors += predicate_errors(sid + ".expect_next", segment["expect_next"])
    if "only_if" in segment:
        if not isinstance(segment["only_if"], list) or not segment["only_if"]:
            errors.append("%s: only_if is a non-empty list of predicates" % sid)
        else:
            for index, alt in enumerate(segment["only_if"]):
                errors += predicate_errors("%s.only_if[%d]" % (sid, index), alt)
        if kind == "save_relaunch":
            errors.append("%s: save_relaunch is never conditional" % sid)
    if kind == "input":
        errors += _event_errors(sid, segment.get("events", []))
        if segment.get("expect_state") and not segment.get("expect_next"):
            errors.append("%s: expect_state needs expect_next (both are checked on the same state)" % sid)
    elif kind == "pulse":
        pulse = segment.get("pulse")
        if not isinstance(pulse, dict):
            return errors + ["%s: pulse segment needs a pulse object" % sid]
        if set(pulse) - PULSE_KEYS:
            errors.append("%s: unknown pulse fields %s" % (sid, sorted(set(pulse) - PULSE_KEYS)))
        for key in ("buttons", "hold", "every", "max", "until"):
            if key not in pulse:
                errors.append("%s: pulse needs %s" % (sid, key))
        if errors:
            return errors
        for button in pulse["buttons"]:
            if button not in BUTTONS:
                errors.append("%s: unknown button %s" % (sid, button))
        if not (1 <= pulse["hold"] < pulse["every"]):
            errors.append("%s: pulse hold must be at least 1 and below every (a held button is never pressed again)" % sid)
        if pulse.get("offset", 1) < 1 or pulse["max"] < 1:
            errors.append("%s: pulse offset >= 1 (the anchor iteration has already run) and max >= 1" % sid)
        errors += predicate_errors(sid + ".until", pulse["until"])
        for index, skip in enumerate(pulse.get("skip_when", [])):
            errors += predicate_errors("%s.skip_when[%d]" % (sid, index), skip)
        for index, choice in enumerate(pulse.get("choose", [])):
            if set(choice) != {"when", "buttons"} or not choice["buttons"]:
                errors.append("%s.choose[%d]: needs exactly when and non-empty buttons" % (sid, index))
                continue
            errors += predicate_errors("%s.choose[%d]" % (sid, index), choice["when"])
            for button in choice["buttons"]:
                if button not in BUTTONS:
                    errors.append("%s: unknown button %s" % (sid, button))
        if segment.get("events") or segment.get("expect_state"):
            errors.append("%s: pulse segments carry no events or expect_state" % sid)
    elif kind == "save_relaunch":
        if not isinstance(segment.get("slot"), int) or not 0 <= segment["slot"] <= 13:
            errors.append("%s: save_relaunch needs slot 0..13" % sid)
    return errors


def semantic_errors(script, expanded=None):
    schema = script.get("schema")
    if schema == SCHEMA_V1:
        return v1.semantic_errors(script)
    if schema != SCHEMA_V2:
        return ["not a %s or %s script" % (SCHEMA_V1, SCHEMA_V2)]
    errors = []
    for key in ("name", "description", "provenance", "segments", "checkpoints", "unsupported"):
        if key not in script:
            errors.append("missing %s" % key)
    unknown = set(script) - SCRIPT_KEYS
    if unknown:
        errors.append("unknown script fields %s" % sorted(unknown))
    if errors:
        return errors
    if expanded is None:
        expanded = not any(s.get("kind") == "repeat" for s in script["segments"])
    ids = []
    for segment in script["segments"]:
        errors += segment_errors(segment, expanded)
        if segment.get("kind") == "repeat" and isinstance(segment.get("count"), int):
            first = segment.get("first", 1)
            ids += [_substitute(s.get("id", ""), i) for i in range(first, first + segment["count"])
                    for s in segment.get("body", [])]
        else:
            ids.append(segment.get("id"))
    if len(set(ids)) != len(ids):
        errors.append("duplicate segment id")
    cids = []
    for checkpoint in script["checkpoints"]:
        if not isinstance(checkpoint, dict):
            errors.append("v2 checkpoints are objects {id, when}")
            continue
        cid = checkpoint.get("id", "<no id>")
        cids.append(cid)
        if set(checkpoint) - CHECKPOINT_KEYS:
            errors.append("checkpoint %s: unknown fields %s" % (cid, sorted(set(checkpoint) - CHECKPOINT_KEYS)))
        if "when" not in checkpoint:
            errors.append("checkpoint %s: needs when" % cid)
            continue
        errors += predicate_errors("checkpoint %s" % cid, checkpoint["when"])
        if not {"game_mode", "mode_stage", "mode_step"} <= set(checkpoint["when"]) and "month" not in checkpoint["when"]:
            errors.append("checkpoint %s: when must name game_mode, mode_stage and mode_step (or a month)" % cid)
        if "limit" in checkpoint and not checkpoint.get("recurring"):
            errors.append("checkpoint %s: limit only applies to recurring checkpoints" % cid)
    if len(set(cids)) != len(cids):
        errors.append("duplicate checkpoint id")
    for row in script["unsupported"]:
        if row.get("side") not in ("native", "port") or not row.get("stop") or len(row.get("reason", "")) < 10:
            errors.append("unsupported rows need side native|port, stop and a reason")
            continue
        if set(row) - UNSUPPORTED_KEYS:
            errors.append("unsupported row %r: unknown fields %s" % (row["stop"], sorted(set(row) - UNSUPPORTED_KEYS)))
        if not STOP_SHAPE.match(row["stop"]):
            errors.append("unsupported row %r: stop is not a \"<gate>: <failure>\" stop text (use known_gaps for a gap "
                          "without one)" % row["stop"])
        if row.get("at_segment") not in ids:
            errors.append("unsupported row %r: at_segment %r is not a segment id of the script" % (
                row["stop"], row.get("at_segment")))
    pins = [(r.get("side"), r.get("stop"), r.get("at_segment")) for r in script["unsupported"]]
    if len(set(pins)) != len(pins):
        errors.append("duplicate unsupported row")
    for row in script.get("known_gaps", []):
        if (not isinstance(row, dict) or set(row) - KNOWN_GAP_KEYS or row.get("side") not in ("native", "port")
                or not row.get("gap") or len(row.get("reason", "")) < 10):
            errors.append("known_gaps rows are {side native|port, gap, reason, ticket?}")
    for index, row in enumerate(script.get("unsampled_ok", [])):
        if not isinstance(row, dict) or set(row) != UNSAMPLED_OK_KEYS or len(str(row.get("evidence", ""))) < 10:
            errors.append("unsampled_ok[%d]: needs exactly when and evidence (the run and frame it was observed in)" % index)
            continue
        errors += predicate_errors("unsampled_ok[%d].when" % index, row["when"])
        if "game_mode" not in row["when"]:
            errors.append("unsampled_ok[%d].when must name game_mode" % index)
    return errors


def load(path, require_expanded=False):
    script = json.loads(Path(path).read_text())
    errors = semantic_errors(script, expanded=True if require_expanded else None)
    if errors:
        raise ScriptError("; ".join(errors))
    return script


def _substitute(value, index):
    if isinstance(value, str):
        return value.replace("{i}", "%03d" % index)
    if isinstance(value, list):
        return [_substitute(v, index) for v in value]
    if isinstance(value, dict):
        return {k: _substitute(v, index) for k, v in value.items()}
    return value


def _unroll(script):
    out = copy.deepcopy(script)
    segments = []
    for segment in script["segments"]:
        if segment.get("kind") != "repeat":
            segments.append(copy.deepcopy(segment))
            continue
        for index in range(segment.get("first", 1), segment.get("first", 1) + segment["count"]):
            segments += [_substitute(copy.deepcopy(inner), index) for inner in segment["body"]]
    out["segments"] = segments
    return out


def expand(script):
    """v2 script with every repeat unrolled; v1 scripts are returned unchanged.
    The result is checked as an expanded script and must unroll to itself."""
    if script.get("schema") == SCHEMA_V1:
        return copy.deepcopy(script)
    out = _unroll(script)
    errors = semantic_errors(out, expanded=True)
    if errors:
        raise ScriptError("expanded script is invalid: " + "; ".join(errors))
    if _unroll(out) != out:
        raise ScriptError("expanded script does not re-expand to itself")
    return out


class Rule:
    """The shared boundary rule. feed(state) is called once per boundary with the
    state of the iteration that just finished; it returns the button edges for
    the next iteration as [(button, pressed)], and records anchors, deliveries,
    checkpoints and the first failure exactly as both players do."""

    def __init__(self, script):
        if script.get("schema") == SCHEMA_V2 and any(s.get("kind") == "repeat" for s in script["segments"]):
            raise ScriptError("players need an expanded script")
        self.segments = script["segments"]
        self.checkpoints = [c for c in script.get("checkpoints", []) if isinstance(c, dict)]
        self.unsampled_ok = [row["when"] for row in script.get("unsampled_ok", [])]
        self.prev_state = None
        self.index = 0
        self.anchor = -1
        self.search_from = -1
        self.waiting_since = -1
        self.releases = {}
        self.expect = None
        self.last = -1
        self.pulse_k = 0
        self.pulse_last = -1
        self.failure = ""
        self.stop = ""
        self.done = False
        self.anchors = {}
        self.delivered = []
        self.fired = []
        self.skipped = []
        self._prev_when = {}
        self._counts = {}
        self._held_due = {}
        self.unsampled = []

    def _fail(self, message):
        self.failure = message
        self.done = True

    def _checkpoints(self, state):
        for checkpoint in self.checkpoints:
            cid = checkpoint["id"]
            now = matches(checkpoint["when"], state)
            before = self._prev_when.get(cid, False)
            self._prev_when[cid] = now
            if not now or before:
                continue
            count = self._counts.get(cid, 0)
            if not checkpoint.get("recurring"):
                if count:
                    continue
                name = cid
            else:
                if "limit" in checkpoint and count >= checkpoint["limit"]:
                    continue
                name = "%s-%03d" % (cid, count + 1)
            self._counts[cid] = count + 1
            self.fired.append({"id": name, "checkpoint": cid, "frame_count": state["frame_count"]})

    def _check_expect(self, state):
        if self.expect is None:
            return
        if matches(self.expect["after"], state):
            if state["frame_count"] - self.expect["since"] > v1_expect_limit():
                self._fail("segment %s: still in its own state %d iterations after its last press (expected %s)"
                           % (self.expect["segment"], v1_expect_limit(), ptext(self.expect["want"])))
            return
        if not matches(self.expect["want"], state):
            self._fail("segment %s: expected %s next, observed %s" % (
                self.expect["segment"], ptext(self.expect["want"]), key_text(state)))
            return
        self.expect = None

    def feed(self, state):
        if self.done:
            return []
        observed = state["frame_count"]
        if observed == self.last:
            return []
        if self.last >= 0 and observed > self.last + 1 and self._edge_due_between(self.last + 2, observed):
            self._fail("iterations %d..%d ran without a boundary while an edge was due" % (self.last + 2, observed))
            return []
        pad_held = state.get("pad_held", -1)
        if pad_held is not None and pad_held >= 0:
            for button, (first, last, strict) in sorted(self._held_due.items()):
                if first <= observed <= last and not pad_held & MASKS[button]:
                    # A pulse press the game did not sample is tolerated only after a boundary state the
                    # script lists in unsampled_ok (a loading state in which the original skips the pad
                    # read); the comparer checks the port shows no unsampled press the original lacks.
                    if strict or self.prev_state is None or not any(matches(ok, self.prev_state) for ok in self.unsampled_ok):
                        self._fail("input slip: %s due for iteration %d is not in the sampled pad %04x (previous boundary %s)"
                                   % (button, observed, pad_held,
                                      "none" if self.prev_state is None else key_text(self.prev_state)))
                        return []
                    self.unsampled.append({"frame_count": observed, "button": button})
                if observed >= last:
                    del self._held_due[button]
        self.last = observed
        self.prev_state = state
        nxt = observed + 1
        self._checkpoints(state)
        self._check_expect(state)
        if self.done:
            return []
        edges = [(b, False) for b, at in sorted(self.releases.items()) if at == nxt]
        for button, _ in edges:
            del self.releases[button]
        while self.index < len(self.segments):
            segment = self.segments[self.index]
            kind = segment.get("kind", "input")
            if self.anchor < 0:
                if self.waiting_since < 0:
                    self.waiting_since = observed
                if observed < self.search_from or not matches(segment["wait_until"], state):
                    timeout = segment.get("timeout_iterations", 0)
                    if timeout and observed - self.waiting_since > timeout:
                        self._fail("segment %s: wait_until not reached within %d iterations" % (segment["id"], timeout))
                    break
                self.anchor = observed
                self.anchors[segment["id"]] = observed
                self.pulse_k = 0
                self.pulse_last = -1
                if "only_if" in segment and not any(matches(alt, state) for alt in segment["only_if"]):
                    # Not this case: the segment ends at its anchor and sends nothing.
                    self.skipped.append(segment["id"])
                    self.index += 1
                    self.search_from = observed
                    self.anchor = -1
                    self.waiting_since = -1
                    continue
            if kind == "save_relaunch":
                self.stop = "segment %s: save_relaunch slot %d" % (segment["id"], segment["slot"])
                self.done = True
                break
            if kind == "pulse":
                pulse = segment["pulse"]
                if not matches(pulse["until"], state):
                    due = self.anchor + pulse.get("offset", 1) + self.pulse_k * pulse["every"]
                    if self.pulse_k >= pulse["max"]:
                        if observed >= self.pulse_last + pulse["every"]:
                            self._fail("segment %s: pulse exhausted after %d presses without %s"
                                       % (segment["id"], pulse["max"], ptext(pulse["until"])))
                        break
                    if due == nxt:
                        self.pulse_k += 1
                        self.pulse_last = nxt
                        if not any(matches(skip, state) for skip in pulse.get("skip_when", [])):
                            buttons = next((c["buttons"] for c in pulse.get("choose", []) if matches(c["when"], state)),
                                           pulse["buttons"])
                            for button in buttons:
                                edges.append((button, True))
                                self.releases[button] = nxt + pulse["hold"]
                                self._held_due[button] = (nxt, nxt + pulse["hold"] - 1, False)
                    break
                last = observed
            else:
                last = self.anchor + segment.get("run_for", 0)
                for event in segment.get("events", []):
                    at = self.anchor + event["at"]
                    last = max(last, at)
                    if at == nxt:
                        for button in event["buttons"]:
                            edges.append((button, True))
                            self.releases[button] = at + event["hold"]
                            self._held_due[button] = (at, at + event["hold"] - 1, True)
                if nxt <= last:
                    break
            if "expect_next" in segment:
                self.expect = {"segment": segment["id"], "after": segment["wait_until"], "want": segment["expect_next"],
                               "since": observed}
                self._check_expect(state)
                if self.done:
                    return []
            self.index += 1
            self.search_from = last
            self.anchor = -1
            self.waiting_since = -1
        for button, pressed in edges:
            self.delivered.append({"frame_count": nxt, "button": button, "pressed": pressed})
        if not self.done and self.index >= len(self.segments) and not self.releases and self.expect is None:
            self.done = True
        return edges

    def _edge_due_between(self, first, last):
        if any(first <= at <= last for at in self.releases.values()):
            return True
        if self.index < len(self.segments) and self.anchor >= 0:
            segment = self.segments[self.index]
            if segment.get("kind") == "pulse":
                pulse = segment["pulse"]
                due = self.anchor + pulse.get("offset", 1) + self.pulse_k * pulse["every"]
                return self.pulse_k < pulse["max"] and first <= due <= last
            return any(first <= self.anchor + e["at"] <= last for e in segment.get("events", []))
        return False


def v1_expect_limit():
    return 600


def simulate(script, rows):
    rule = Rule(script)
    for row in rows:
        rule.feed(row)
        if rule.done:
            break
    return {"anchors": rule.anchors, "skipped": rule.skipped, "unsampled": rule.unsampled, "delivered": rule.delivered, "checkpoints": rule.fired,
            "failure": rule.failure, "stop": rule.stop, "done": rule.done, "segment_index": rule.index}


def load_trace(path):
    rows = [json.loads(line) for line in Path(path).read_text().splitlines() if line.strip()]
    for row in rows:
        if isinstance(row.get("game_mode"), str):
            row["game_mode"] = int(row["game_mode"], 16)
    for a, b in zip(rows, rows[1:]):
        if b["frame_count"] < a["frame_count"]:
            raise ScriptError("trace frame_count goes backwards at %d" % b["frame_count"])
    return rows


def lua_literal(value):
    """A Lua table literal of a JSON value (the emulator player has no JSON parser).
    Lists become 1-based sequences; predicate game_mode strings become integers."""
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, int):
        return str(value)
    if isinstance(value, str):
        return json.dumps(value)
    if isinstance(value, list):
        return "{" + ",".join(lua_literal(v) for v in value) + "}"
    if isinstance(value, dict):
        parts = []
        for key, item in value.items():
            if key == "game_mode" and isinstance(item, str):
                item = int(item, 16)
            parts.append("[%s]=%s" % (json.dumps(key), lua_literal(item)))
        return "{" + ",".join(parts) + "}"
    if value is None:
        return "nil"
    raise ScriptError("cannot express %r in Lua" % (value,))


def to_lua(script):
    """SCRIPT = {...} for tools/emu/route_driver/player.lua (expanded scripts only)."""
    rule = Rule(script)  # refuses unexpanded scripts
    keep = {"name": script["name"], "schema": script["schema"], "segments": rule.segments,
            "checkpoints": rule.checkpoints, "unsupported": script.get("unsupported", []),
            "unsampled_ok": script.get("unsampled_ok", [])}
    return "SCRIPT = " + lua_literal(keep) + "\n"


def pinned_row(script, side, stop, segment_id):
    """The unsupported row that pins this stop: same side, same stop text and the
    segment the player was on when it stopped (``at_segment``), else None."""
    for row in script.get("unsupported", []):
        if row.get("side") == side and row.get("stop") == stop and row.get("at_segment") == segment_id:
            return row
    return None


def verdict(script, side, reason, stop, segment_id):
    """(passed, pinned row or None) for a run's stop, exactly as the players decide it."""
    if reason == "script_complete":
        return True, None
    row = pinned_row(script, side, stop, segment_id) if stop else None
    return row is not None, row


def preflight(script, side=None):
    stops = [{"side": u["side"], "stop": u["stop"], "at_segment": u.get("at_segment")}
             for u in script["unsupported"] if side is None or u["side"] == side]
    gaps = [g for g in script.get("known_gaps", []) if side is None or g["side"] == side]
    checkpoints = [c if isinstance(c, str) else c["id"] for c in script["checkpoints"]]
    kinds = {}
    for segment in script["segments"]:
        kinds[segment.get("kind", "input")] = kinds.get(segment.get("kind", "input"), 0) + 1
    return {"script": script["name"], "schema": script["schema"], "segments": len(script["segments"]),
            "segment_kinds": kinds, "checkpoints": checkpoints,
            "unsampled_ok": [row["when"] for row in script.get("unsampled_ok", [])],
            "expected_unsupported_stops": stops, "known_gaps_not_pinned": gaps}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("validate")
    p.add_argument("script", type=Path)
    p = sub.add_parser("expand")
    p.add_argument("script", type=Path)
    p.add_argument("--out", type=Path)
    p = sub.add_parser("preflight")
    p.add_argument("script", type=Path)
    p.add_argument("--side", choices=["native", "port"])
    p = sub.add_parser("verdict")
    p.add_argument("script", type=Path)
    p.add_argument("--run", type=Path, required=True, help="a player's route.json")
    p = sub.add_parser("lua")
    p.add_argument("script", type=Path)
    p.add_argument("--out", type=Path, required=True)
    p = sub.add_parser("simulate")
    p.add_argument("script", type=Path)
    p.add_argument("--trace", type=Path, required=True)
    p.add_argument("--delivered", type=Path, help="the player's delivered edges (JSON list or JSONL)")
    args = parser.parse_args(argv)
    try:
        script = load(args.script)
        if args.cmd == "validate":
            print("ok %s %s" % (script["schema"], script["name"]))
            return 0
        if args.cmd == "expand":
            text = json.dumps(expand(script), indent=1) + "\n"
            if args.out:
                args.out.write_text(text)
            else:
                sys.stdout.write(text)
            return 0
        if args.cmd == "preflight":
            print(json.dumps(preflight(expand(script), args.side), indent=1))
            return 0
        if args.cmd == "verdict":
            run = json.loads(args.run.read_text())
            segment_id = run.get("segment_at_stop", "")
            passed, row = verdict(expand(script), run["side"], run["reason"], run.get("stop", ""), segment_id)
            out = {"side": run["side"], "reason": run["reason"], "stop": run.get("stop", ""), "segment_at_stop": segment_id,
                   "passed": passed, "pinned_by": row, "recorded_passed": run.get("passed")}
            print(json.dumps(out, indent=1))
            return 0 if passed and run.get("passed") == passed else 1
        if args.cmd == "lua":
            args.out.write_text(to_lua(expand(script)))
            return 0
        result = simulate(expand(script), load_trace(args.trace))
        status = 0
        if args.delivered:
            text = args.delivered.read_text().strip()
            try:
                whole = json.loads(text)
                player = whole["delivered"] if isinstance(whole, dict) else whole  # route.json of either player, or a list
            except json.JSONDecodeError:
                player = [json.loads(line) for line in text.splitlines() if line.strip()]
            player = [{"frame_count": d["frame_count"], "button": d["button"], "pressed": d["pressed"]} for d in player]
            result["delivered_match"] = player == result["delivered"]
            if not result["delivered_match"]:
                first = next((i for i, (a, b) in enumerate(zip(player, result["delivered"])) if a != b),
                             min(len(player), len(result["delivered"])))
                result["first_delivered_difference"] = {"index": first,
                                                        "player": player[first] if first < len(player) else None,
                                                        "rule": result["delivered"][first] if first < len(result["delivered"]) else None}
                status = 1
        print(json.dumps(result, indent=1))
        return status
    except (ScriptError, OSError, KeyError, ValueError) as error:
        print("route_script: %s" % error, file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
