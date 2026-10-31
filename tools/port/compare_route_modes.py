"""Compare two installed route runs, normally one real-time and one FAST
(tools/port/run_installed_route.sh --fast, run_opening_frame_timing.sh --fast),
and fail if the game diverged.

Both runs write tests/route_receipts.gd receipts. Compared, all exactly:
  frames      frames.tsv rows (logic tick, frame_count, mode/stage/step/substep,
              SHA-256 of the native 320x240 image) for every published frame;
              the per-substep frame table is derived from these rows
  transitions transitions.json / transitions-all.json when present
  states      every states/*.json file (committed game state, rule state, RNG)
  pngs        every PNG both runs saved (native image checkpoints and the
              .native.png beside each window capture); window captures that
              only the real-time run can take are listed, not compared
  receipts    the logic parts of rei-proof.json, command-select.json and
              playtest.json (wall-clock audio diagnostics, original_audio
              included, and the render frame counter are left out)
  audio       native BGM blocks (audio-blocks.tsv). --audio report (default)
              only reports them: before the Rei start the real-time backend
              renders as many blocks as the wall-clock device consumes, not one
              per logic frame, so real-time PCM depends on host speed (a route
              running below 60 logic frames/s renders several blocks per logic
              frame). --audio require-race fails unless the blocks rendered
              from the Rei start by the logic-locked clock match, --audio
              require-all unless every block matches (two FAST runs).
Session owner tokens built from Godot instance ids
("cold-title-no-card-session:<id>") differ in every process and are
normalised before JSON comparison.
A FAST run must also show its pacing: exactly one native block per published
logic frame from the Rei start, no underrun, and a race FIFO of at most one
block (the Rei-start tick renders its first logic-locked block after the
pre-Rei pump has already filled the look-ahead that tick). The BGM
diagnostics come from original_audio in rei-proof.json, command-select.json
or playtest.json, or from audio_clock in rei-failure.json (a route that
stopped early). A FAST run with no such receipt, or with no sink skip count,
fails; a receipt without a race clock means the route ended before the Rei
start, and only the underrun check applies.
The offline FAST device drains one block per logic tick and cannot underrun,
so FAST pacing does not replace the real-time underrun checks.
Speed: logic frames per second of each run (route-speed.json).
Dev resumes: a run that started from a dev checkpoint (resumed-from.json,
tools/port/run_installed_route.sh --resume) is refused unless --resumed is
given. With --resumed, exactly one run must be resumed; the other run's frames
from the checkpoint tick on and its audio blocks from the resumed run's first
block on are compared with the resumed run's, receipts as usual, and states
and PNGs only where both runs wrote them (a resumed run writes nothing from
before its checkpoint). --resumed is a development check of checkpoint
exactness, never an acceptance or parity result.

usage (Docker): python3 -I tools/port/compare_route_modes.py <run-a> <run-b> [--json out.json] [--audio report|require-race|require-all] [--resumed]
Exit 0 only when every compared item is identical.
"""
import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

LOGIC_RECEIPT_KEYS = {
    "rei-proof.json": ["checks", "failed", "states", "final", "entry_image"],
    "command-select.json": ["checks", "failed", "tick", "state", "rule_state", "rng"],
    "playtest.json": None,  # every field is logic except WALL_CLOCK_KEYS
}
WALL_CLOCK_KEYS = {"playtest.json": ["original_audio"]}
# Receipts that carry the BGM owner's diagnostics, in lookup order, and the key.
AUDIO_RECEIPTS = [("rei-proof.json", "original_audio"), ("command-select.json", "original_audio"),
                  ("playtest.json", "original_audio"), ("rei-failure.json", "audio_clock")]


def read_frames(run):
    path = run / "frames.tsv"
    if not path.is_file():
        return None
    rows = []
    for line in path.read_text().splitlines()[1:]:
        cols = line.split("\t")
        if len(cols) >= 6:
            rows.append({"tick": int(cols[0]), "frame_count": int(cols[1]), "key": cols[2], "image": cols[3],
                         "pcm_blocks": int(cols[4]), "wall_ms": int(cols[5])})
    return rows


def substep_table(rows):
    """First frame_count of each mode/stage/step/substep, in order, and its length."""
    table = []
    for row in rows:
        if not table or table[-1][0] != row["key"]:
            table.append([row["key"], row["frame_count"], 0])
        table[-1][2] += 1
    return table


def read_blocks(run):
    path = run / "audio-blocks.tsv"
    if not path.is_file():
        return None
    out = []
    for line in path.read_text().splitlines()[1:]:
        cols = line.split("\t")
        if len(cols) == 4:
            out.append({"index": int(cols[0]), "source": cols[1], "last_tick": int(cols[2]), "sha": cols[3]})
    return out


SESSION_TOKEN = re.compile(r"(cold-title-no-card-session:)-?\d+")


def normalise(value):
    if isinstance(value, dict):
        return {k: normalise(v) for k, v in value.items()}
    if isinstance(value, list):
        return [normalise(v) for v in value]
    if isinstance(value, str):
        return SESSION_TOKEN.sub(r"\1<instance>", value)
    return value


def load_json(path):
    try:
        return json.loads(path.read_text())
    except (OSError, ValueError):
        return None


def load_logic_json(path):
    return normalise(load_json(path))


def run_mode(run):
    speed = load_json(run / "route-speed.json") or {}
    return speed.get("mode", "unknown")


def audio_receipt(run):
    """(receipt name, BGM diagnostics) of the first AUDIO_RECEIPTS file present, else (None, {})."""
    for name, key in AUDIO_RECEIPTS:
        data = load_json(run / name)
        if isinstance(data, dict) and isinstance(data.get(key), dict):
            return name, data[key]
    return None, {}


def audio_diagnostics(run):
    return audio_receipt(run)[1]


def race_blocks(run, rows, blocks):
    """Blocks rendered by the logic-locked clock: the k-th is logic frame rei_tick+k."""
    rei_tick = audio_diagnostics(run).get("race_clock", {}).get("rei_tick")
    if rei_tick is None or rows is None or blocks is None:
        return None, None
    at = next((r for r in rows if r["tick"] == rei_tick), None)
    if at is None:
        return rei_tick, None
    return rei_tick, [b["sha"] for b in blocks[at["pcm_blocks"]:]]


def first_difference(a, b, keys):
    for i, (x, y) in enumerate(zip(a, b)):
        if any(x[k] != y[k] for k in keys):
            return {"row": i, "a": {k: x[k] for k in keys}, "b": {k: y[k] for k in keys}}
    if len(a) != len(b):
        return {"row": min(len(a), len(b)), "a_rows": len(a), "b_rows": len(b)}
    return None


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def pngs(run):
    return {str(p.relative_to(run)): p for p in run.rglob("*.png") if "content-install" not in p.parts}


def fast_pacing(run, rows):
    """FAST-only: one block per published logic frame from the Rei start, no FIFO, no underrun."""
    source, audio = audio_receipt(run)
    race = audio.get("race_clock") or {}
    sink = audio.get("sink") or {}
    out = {"receipt": source, "rei_tick": race.get("rei_tick"), "max_fifo": race.get("max_fifo"), "skips": sink.get("skips")}
    problems = []
    if source is None:
        out["problems"] = ["no BGM diagnostics receipt (%s)" % ", ".join(name for name, _ in AUDIO_RECEIPTS)]
        return out
    # No race clock in the receipt: the route ended before the Rei start.
    out["ended_before_rei"] = race.get("rei_tick") is None
    if out["skips"] is None and not (out["ended_before_rei"] and not sink):
        problems.append("%s has no sink skip count" % source)
    if not out["ended_before_rei"] and not rows:
        problems.append("no frames.tsv rows to check blocks per frame")
    if rows and race.get("rei_tick") is not None:
        after = [r for r in rows if r["tick"] >= race["rei_tick"]]
        deltas = {}
        for prev, cur in zip(after, after[1:]):
            d = cur["pcm_blocks"] - prev["pcm_blocks"]
            deltas[d] = deltas.get(d, 0) + 1
            if d != 1 and len(problems) < 5:
                problems.append("tick %d pushed %d blocks" % (cur["tick"], d))
        out["blocks_per_frame"] = {str(k): v for k, v in sorted(deltas.items())}
        if not deltas:
            problems.append("no published frames after the Rei start tick %d" % race["rei_tick"])
    if not out["ended_before_rei"] and race.get("max_fifo") is None:
        problems.append("%s race clock has no max_fifo" % source)
    if race.get("max_fifo") not in (None, 0, 1):
        problems.append("race FIFO reached %s" % race.get("max_fifo"))
    if out["skips"] not in (None, 0):
        problems.append("offline device underruns %s" % out["skips"])
    out["problems"] = problems
    return out


def compare(a, b, audio="report", resumed=False):
    report = {"a": str(a), "b": str(b), "mode_a": run_mode(a), "mode_b": run_mode(b), "checks": {}, "diverged": []}

    def check(name, ok, detail):
        report["checks"][name] = {"identical": ok, **detail}
        if not ok:
            report["diverged"].append(name)

    resume_a, resume_b = load_json(a / "resumed-from.json"), load_json(b / "resumed-from.json")
    if (resume_a is not None or resume_b is not None) and not resumed:
        check("resumed_run", False, {"refused": "a dev-checkpoint resume is not route evidence; use --resumed only to check checkpoint exactness"})
        report["identical"] = False
        return report
    start_tick = None
    if resumed:
        if (resume_a is None) == (resume_b is None):
            check("resumed_run", False, {"refused": "--resumed needs exactly one resumed run"})
            report["identical"] = False
            return report
        start_tick = int((resume_a or resume_b)["tick"])
        report["resumed"] = {"run": str(a if resume_a is not None else b), "tick": start_tick, "checkpoint": resume_a or resume_b}

    rows_a, rows_b = read_frames(a), read_frames(b)
    if start_tick is not None:
        rows_a = None if rows_a is None else [r for r in rows_a if r["tick"] >= start_tick]
        rows_b = None if rows_b is None else [r for r in rows_b if r["tick"] >= start_tick]
        check("resumed_first_frame", bool(rows_a) and bool(rows_b) and rows_a[0]["tick"] == rows_b[0]["tick"] == start_tick,
              {"first_ticks": [rows[0]["tick"] if rows else None for rows in (rows_a, rows_b)]})
    if rows_a is None or rows_b is None:
        check("frames", False, {"missing": [str(r) for r, rows in ((a, rows_a), (b, rows_b)) if rows is None]})
    else:
        diff = first_difference(rows_a, rows_b, ["tick", "frame_count", "key", "image"])
        check("frames", diff is None, {"rows": len(rows_a), "first_difference": diff})
        ta, tb = substep_table(rows_a), substep_table(rows_b)
        check("substep_frames", ta == tb, {"substeps": len(ta),
                                           "first_difference": next(([x, y] for x, y in zip(ta, tb) if x != y), None)})

    for name in ("transitions.json", "transitions-all.json"):
        ja, jb = load_json(a / name), load_json(b / name)
        # A resumed run never writes the opening's transitions (pre-checkpoint).
        if start_tick is not None and (ja is None or jb is None):
            continue
        if ja is not None or jb is not None:
            check(name, ja == jb, {})

    states_a = {p.name: p for p in (a / "states").glob("*.json")}
    states_b = {p.name: p for p in (b / "states").glob("*.json")}
    if states_a or states_b:
        differing = sorted(n for n in states_a.keys() & states_b.keys() if load_logic_json(states_a[n]) != load_logic_json(states_b[n]))
        only = sorted(states_a.keys() ^ states_b.keys()) if start_tick is None else []
        check("states", not differing and not only, {"files": len(states_a), "differing": differing, "only_in_one": only})

    for name, keys in LOGIC_RECEIPT_KEYS.items():
        ja, jb = load_logic_json(a / name), load_logic_json(b / name)
        if ja is None and jb is None:
            continue
        if ja is None or jb is None:
            check(name, False, {"missing": True})
            continue
        if keys is not None:
            ja = {k: ja.get(k) for k in keys}
            jb = {k: jb.get(k) for k in keys}
        else:
            ja = {k: v for k, v in ja.items() if k not in WALL_CLOCK_KEYS.get(name, [])}
            jb = {k: v for k, v in jb.items() if k not in WALL_CLOCK_KEYS.get(name, [])}
        for data in (ja, jb):
            if isinstance(data.get("entry_image"), dict):
                data["entry_image"] = {k: v for k, v in data["entry_image"].items() if k != "render_frame"}
        check(name, ja == jb, {"differing_keys": sorted(k for k in set(ja) | set(jb) if ja.get(k) != jb.get(k))})

    pa, pb = pngs(a), pngs(b)
    common = sorted(pa.keys() & pb.keys())
    differing = [n for n in common if sha(pa[n]) != sha(pb[n])]
    window_only = sorted(n for n in pa.keys() ^ pb.keys() if n[:-4] + ".native.png" in common)
    other_only = sorted(n for n in pa.keys() ^ pb.keys() if n not in window_only) if start_tick is None else []
    check("pngs", not differing and not other_only, {"compared": len(common), "differing": differing,
                                                     "window_captures_not_compared": window_only, "only_in_one": other_only})

    rei_a, race_a = race_blocks(a, rows_a, read_blocks(a))
    rei_b, race_b = race_blocks(b, rows_b, read_blocks(b))
    if race_a is not None and race_b is not None:
        n = min(len(race_a), len(race_b))
        first = next((i for i in range(n) if race_a[i] != race_b[i]), None)
        detail = {"rei_tick": [rei_a, rei_b], "compared_blocks": n, "first_difference_block": first,
                  "blocks": [len(race_a), len(race_b)]}
        if audio == "require-race":
            check("audio_race_blocks", first is None and rei_a == rei_b and n > 0, detail)
        else:
            report["audio_race_blocks"] = {"identical": first is None and rei_a == rei_b and n > 0, **detail}
    blocks_a, blocks_b = read_blocks(a), read_blocks(b)
    if start_tick is not None and blocks_a is not None and blocks_b is not None:
        resumed_blocks = blocks_a if resume_a is not None else blocks_b
        first_index = resumed_blocks[0]["index"] if resumed_blocks else None
        blocks_a = [x for x in blocks_a if first_index is not None and x["index"] >= first_index]
        blocks_b = [x for x in blocks_b if first_index is not None and x["index"] >= first_index]
    if blocks_a is not None and blocks_b is not None:
        first = next((i for i, (x, y) in enumerate(zip(blocks_a, blocks_b)) if x["sha"] != y["sha"]), None)
        detail = {"blocks": [len(blocks_a), len(blocks_b)], "first_difference_block": first}
        same = first is None and len(blocks_a) == len(blocks_b)
        if audio == "require-all":
            check("audio_all_blocks", same, detail)
        else:
            report["audio_all_blocks"] = {"identical": same, **detail}

    for run, rows, mode in ((a, rows_a, report["mode_a"]), (b, rows_b, report["mode_b"])):
        if mode == "fast":
            pacing = fast_pacing(run, rows)
            report.setdefault("fast_pacing", {})[str(run)] = pacing
            if pacing["problems"]:
                report["diverged"].append("fast_pacing:" + str(run))

    report["speed"] = {str(run): load_json(run / "route-speed.json") for run in (a, b)}
    fps = [(load_json(r / "route-speed.json") or {}).get("frames_per_second", 0) for r in (a, b)]
    if all(fps):
        report["speed"]["b_over_a"] = fps[1] / fps[0]
    report["identical"] = not report["diverged"]
    return report


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("a", type=Path)
    parser.add_argument("b", type=Path)
    parser.add_argument("--json", type=Path)
    parser.add_argument("--audio", choices=["report", "require-race", "require-all"], default="report")
    parser.add_argument("--resumed", action="store_true", help="development check of a dev-checkpoint resume (see above)")
    args = parser.parse_args(argv)
    report = compare(args.a, args.b, args.audio, args.resumed)
    text = json.dumps(report, indent=1, sort_keys=True)
    if args.json:
        args.json.write_text(text)
    for name, item in sorted(report["checks"].items()):
        print("%-22s %s" % (name, "identical" if item["identical"] else "DIFFERS " + json.dumps(item)[:400]))
    for name in ("audio_race_blocks", "audio_all_blocks"):
        if name in report:
            print("%-22s %s (report only)" % (name, json.dumps(report[name])))
    for run, pacing in report.get("fast_pacing", {}).items():
        print("fast pacing %s: %s" % (run, json.dumps(pacing)))
    speed = report.get("speed", {})
    for run in (args.a, args.b):
        s = speed.get(str(run)) or {}
        print("speed %s: %s logic frames in %.1f s = %.1f frames/s (%s)" % (run, s.get("published_frames"), s.get("wall_seconds", 0), s.get("frames_per_second", 0), s.get("mode")))
    print("IDENTICAL" if report["identical"] else "DIVERGED: " + ", ".join(report["diverged"]))
    return 0 if report["identical"] else 1


if __name__ == "__main__":
    sys.exit(main())
