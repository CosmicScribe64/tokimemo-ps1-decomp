#!/usr/bin/env python3
"""Title/file-select golden: fixture export and per-iteration comparison.

The golden runs come from tools/emu/title_filesel/run.sh (the original from
power-on under PCSX-Redux/OpenBIOS, scripted pad, no memory card). Each run
directory holds logic.jsonl (one row per game_mode_dispatch entry),
movie.jsonl (one row per movie_play pad_update entry), inputs.jsonl and
meta.jsonl.

  fixture <golden-dir> <out.json> <run>[+<continuation>]...
      Writes the Godot test fixture (port/godot/tests/fixtures/
      title_file_select_golden.json): per run, the pad input as held words per
      movie poll and per logic iteration, the sha256 of the canonical row
      stream, the key transitions and the NAME_ENT handoff values. Numbers
      only; no game data. A continuation run (capture.lua with load_state =
      the run's final.sstate, stop_mode = 0x15) adds the NAME_ENT rand()
      draws up to the prologue and the rand word at its first dispatch.
  compare <golden-dir> <run> <port-trace.json>
      Diffs a port trace (flow/title_file_select.gd rows, dumped by
      tests/run_title_file_select.gd) against the golden row by row.

Canonical rows (identical to TitleFileSelect.trace_sha256):
  M<poll>,<frame_count>,<timer_ticks>,<seed>
  L<l>,<mode>,<stage>,<step>,<substep>,<frame_count>,<timer_ticks>,<seed>,
   <game_mode_timer>,<mode_stage_timer>,<mode_step_timer>,<scene_work>,
   <rand word>,<pad_pressed>,<memory_card_result>
All words unsigned 32-bit. Logic rows stop before the first dispatch of mode
0x14 stage 1 (the entry controller's first iteration); that row's values are
the handoff. A run that never reaches it keeps every row.
"""
import hashlib
import json
import os
import struct
import sys

BUTTONS = {"SELECT": 0x100, "START": 0x800, "UP": 0x1000, "RIGHT": 0x2000, "DOWN": 0x4000, "LEFT": 0x8000,
           "TRIANGLE": 0x10, "CIRCLE": 0x20, "CROSS": 0x40, "SQUARE": 0x80, "L2": 0x1, "R2": 0x2, "L1": 0x4, "R1": 0x8}
LOGIC_FIELDS = ("l", "m", "s", "st", "ss", "fc", "tt", "seed", "gmt", "sgt", "stt", "sw", "rw", "pp", "cr")


def _rows(path):
    with open(path) as f:
        return [json.loads(line) for line in f if line.strip()]


def load_run(golden, run):
    base = os.path.join(golden, run)
    logic = _rows(os.path.join(base, "logic.jsonl"))
    movie = _rows(os.path.join(base, "movie.jsonl")) if os.path.exists(os.path.join(base, "movie.jsonl")) else []
    inputs = _rows(os.path.join(base, "inputs.jsonl")) if os.path.exists(os.path.join(base, "inputs.jsonl")) else []
    if not logic:
        raise SystemExit(f"{run}: empty logic.jsonl")
    if not movie:
        raise SystemExit(f"{run}: movie.jsonl is missing (capture.lua logs it from vsync 1)")
    return logic, movie, inputs


def handoff_index(logic):
    for i, r in enumerate(logic):
        if r["m"] == 0x14 and r["s"] == 1:
            return i
    return None


def canonical(logic, movie):
    """Canonical lines plus the handoff row (or None)."""
    lines = []
    for k, r in enumerate(movie, start=1):
        lines.append("M" + ",".join(str(v & 0xFFFFFFFF) for v in (k, r["fc"], r["tt"], r["seed"])))
    stop = handoff_index(logic)
    for r in logic[: stop if stop is not None else len(logic)]:
        lines.append("L" + ",".join(str(r[f] & 0xFFFFFFFF) for f in LOGIC_FIELDS))
    return lines, (logic[stop] if stop is not None else None)


def held_inputs(logic, movie, inputs):
    """Held pad word per movie poll and per logic sample.

    A movie poll at vsync v sees a button pressed at vsync b with hold h when
    b <= v < b + h. A dispatch's pad word is latched by the previous draw: the
    first logic row whose pad_pressed has the button starts the hold, which
    then lasts h samples.
    """
    movie_held = []
    for r in movie:
        word = 0
        for e in inputs:
            if e["v"] <= r["v"] < e["v"] + e["hold"]:
                word |= BUTTONS[e["button"]]
        movie_held.append(word)
    logic_events = []
    pending = sorted(inputs, key=lambda e: e["v"])
    pending = [e for e in pending if e["v"] > movie[-1]["v"] or not any(
        e["v"] <= r["v"] < e["v"] + e["hold"] for r in movie)]
    for e in pending:
        bit = BUTTONS[e["button"]]
        start = next((r["l"] for r in logic if r["l"] >= 2 and r["v"] >= e["v"] and (r["pp"] & bit)), None)
        if start is None:
            raise SystemExit(f"input {e} never reached a dispatch")
        logic_events.append({"l": start, "held": bit, "samples": e["hold"]})
    return movie_held, logic_events


def transitions(logic, stop):
    out, prev = [], None
    for r in logic[: stop if stop is not None else len(logic)]:
        key = (r["m"], r["s"], r["st"], r["ss"])
        if key != prev:
            out.append([r[f] & 0xFFFFFFFF for f in LOGIC_FIELDS])
        prev = key
    return out


def continuation(golden, cont):
    base = os.path.join(golden, cont)
    rng = _rows(os.path.join(base, "rng.jsonl"))
    draws = []
    for r in rng:
        if r["f"] == "srand":
            break
        draws.append(r["before"] & 0xFFFFFFFF)
    cps = {c["tag"]: c for c in _rows(os.path.join(base, "checkpoints.jsonl"))}
    if "mode15_first" not in cps or not draws:
        raise SystemExit(f"{cont}: no NAME_ENT draws or no prologue checkpoint")
    # GameState at the first prologue dispatch (0x800E6280): player_birthday
    # (+0xF8), girls[0].birthday (+0x1CC) and the club byte (+0x1CA) of
    # girls[0..4] (GirlRecord stride 0x38), the words NAME_ENT's draws decide.
    with open(os.path.join(base, "mode15_first.ram.bin"), "rb") as f:
        ram = f.read()
    gs = 0xE6280
    word = lambda off: struct.unpack_from("<I", ram, gs + off)[0]
    return {"run": cont, "draws": len(draws), "first_before": draws[0],
            "prologue_rand_word": cps["mode15_first"]["rw"] & 0xFFFFFFFF,
            "player_birthday": word(0xF8), "shiori_birthday": word(0x1CC),
            "club_bytes": [ram[gs + 0x1CA + g * 0x38] for g in range(5)]}


def fixture(golden, out_path, runs):
    doc = {"schema": "tokimemo.title-file-select-golden.v1",
           "source": "original under PCSX-Redux/OpenBIOS, tools/emu/title_filesel/run.sh, no memory card",
           "fields": list(LOGIC_FIELDS), "runs": {}}
    for spec in runs:
        run, _, cont = spec.partition("+")
        logic, movie, inputs = load_run(golden, run)
        lines, hand = canonical(logic, movie)
        stop = handoff_index(logic)
        movie_held, logic_events = held_inputs(logic, movie, inputs)
        doc["runs"][run] = {
            "movie_held": movie_held,
            "logic_inputs": logic_events,
            "rows": len(lines),
            "logic_rows": len(lines) - len(movie),
            "sha256": hashlib.sha256("".join(l + "\n" for l in lines).encode()).hexdigest(),
            "transitions": transitions(logic, stop),
            "handoff": None if hand is None else {k: hand[k] & 0xFFFFFFFF for k in ("l", "fc", "tt", "seed", "sw", "rw")},
        }
        if cont:
            doc["runs"][run]["continuation"] = continuation(golden, cont)
    with open(out_path, "w") as f:
        json.dump(doc, f, indent=1, sort_keys=True)
        f.write("\n")
    print(f"wrote {out_path}: " + ", ".join(f"{r} {d['rows']} rows" for r, d in doc["runs"].items()))


def compare(golden, run, trace_path):
    logic, movie, _ = load_run(golden, run)
    lines, hand = canonical(logic, movie)
    with open(trace_path) as f:
        port = json.load(f)
    port_lines = port["lines"]
    bad = 0
    for i, (g, p) in enumerate(zip(lines, port_lines)):
        if g != p:
            bad += 1
            if bad <= 10:
                print(f"row {i}: golden {g}\n        port   {p}")
    if len(lines) != len(port_lines):
        bad += 1
        print(f"row count: golden {len(lines)} port {len(port_lines)}")
    if hand is not None:
        want = {k: hand[k] & 0xFFFFFFFF for k in ("fc", "tt", "seed", "sw", "rw")}
        got = port.get("handoff", {})
        for k, v in want.items():
            if got.get(k) != v:
                bad += 1
                print(f"handoff {k}: golden {v} port {got.get(k)}")
    print(f"{run}: {'PASS' if bad == 0 else 'FAIL'} ({len(lines)} golden rows, {bad} mismatches)")
    return 0 if bad == 0 else 1


def main(argv):
    if len(argv) >= 4 and argv[1] == "fixture":
        fixture(argv[2], argv[3], argv[4:])
        return 0
    if len(argv) == 5 and argv[1] == "compare":
        return compare(argv[2], argv[3], argv[4])
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
