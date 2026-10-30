#!/usr/bin/env python3
"""T-39027: opening frame pacing receipt (native cadence vs port cadence, host metrics).

Inputs, all read only:

- native: the emulator's per-vsync memory samples (``frames.jsonl`` from a
  tools/emu job such as .asset-cache/emu/opening-timing-v4/fcb-start20), one
  JSON object per vsync with ``f`` (vsync), ``fc`` (game_state frame_count),
  ``mode``, ``stage``, ``step`` and ``sub``;
- port: the receipt written by port/godot/tests/support/pacing_probe.gd
  (schema ``tokimemo.pacing-probe.v1``): ``ticks``, one record per logical
  tick of the port's nominal 60/1 clock (the port's vsync for cadence), and
  ``frames``, one record per presented host frame (host metrics only).

For each opening substep (mode 0x15, key ``stage/step/sub``) the report lists
the game iterations and the histogram of presented frames (vsyncs) per
iteration, native and port, and fails when either differs. Iterations are
identified by frame_count: an iteration's vsync count is the number of samples
that carry its frame_count, and its substep is the one of its last sample (the
state the iteration ends in; a native sample taken during a multi-vsync stall
can show a half-updated state). A frame_count that advances by more than one
between samples means iterations that ran without a vsync wait (the
original's catch-up after a long iteration); they count as zero-vsync
iterations, so histograms can have a "0" bin.
Native and port frame_count bases differ by one, so only counts are compared.
A vsync gap in the native samples, a logical tick gap in the port receipt or a
frame_count that goes backwards is an input error, never resynchronised.

Host metrics (frame interval and process/render time p50/p95/p99, logical
ticks run per host frame (catch-up), hitches over
1.5x and 3x the median interval attributed to the MDEC/STR/XA/VAB events the
probe recorded in the hitch frame or the frame before, BGM pump underruns,
launch-to-first-interactive time) are recorded per host. Under Xvfb, or when
the receipt does not say the host is a real desktop, they are labelled
``relative_only``: regression signals between runs on the same host, never
absolute pacing evidence. Cadence equality is T1/T2 evidence for the compared
run only; the report never claims a real-desktop (T4) result.
"""
from __future__ import annotations

import argparse
import bisect
import hashlib
import json
from pathlib import Path
import sys

SCHEMA = "tokimemo.frame-pacing-report.v1"
PROBE_SCHEMA = "tokimemo.pacing-probe.v1"
OPENING_MODE = 0x15
EVENT_KINDS = ("mdec", "str", "xa", "vab")
HITCH_FACTORS = (1.5, 3.0)


class PacingError(ValueError):
    """Malformed or inconsistent pacing input."""


def _sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _int(record: dict, key: str, where: str) -> int:
    value = record.get(key)
    if type(value) is not int:
        raise PacingError(f"{where}: field {key} must be an integer")
    return value


def read_jsonl(data: bytes, where: str) -> list[dict]:
    rows = []
    for number, line in enumerate(data.decode("utf-8").split("\n"), 1):
        if not line.strip():
            continue
        value = json.loads(line)
        if not isinstance(value, dict):
            raise PacingError(f"{where}:{number}: record is not an object")
        rows.append(value)
    return rows


def loop_keys(rows: list[dict]) -> list[tuple[int, str | None]]:
    """Loop-boundary transitions (tools/emu ``loops.jsonl``: ``fc`` and ``k`` =
    mode/stage/step/sub) as sorted (frame_count, opening key or None)."""
    transitions = []
    for index, row in enumerate(rows):
        where = f"native loop record {index}"
        fields = str(row.get("k", "")).split("/")
        if len(fields) != 4 or not all(f.isdigit() for f in fields):
            raise PacingError(where + ": k must be mode/stage/step/sub")
        key = "/".join(fields[1:]) if int(fields[0]) == OPENING_MODE else None
        transitions.append((_int(row, "fc", where), key))
    transitions.sort()
    if len({fc for fc, _ in transitions}) != len(transitions):
        raise PacingError("native loop records repeat a frame_count")
    return transitions


def loop_key(loops: list[tuple[int, str | None]], frame_count: int) -> str | None:
    """Opening key of the boot-loop iteration numbered frame_count."""
    position = bisect.bisect_right([fc for fc, _ in loops], frame_count) - 1
    return loops[position][1] if position >= 0 else None


def native_frames(rows: list[dict], loops: list[tuple[int, str | None]] | None = None) -> list[dict]:
    """Normalise emulator per-vsync samples to {frame_count, key}.

    With loop-boundary records the key of every sample comes from them (the
    state the boot loop iteration runs in), which also keys the zero-vsync
    catch-up iterations exactly; without them the sampled state is used.
    """
    frames = []
    previous = None
    for index, row in enumerate(rows):
        where = f"native sample {index}"
        vsync = _int(row, "f", where)
        if previous is not None and vsync != previous + 1:
            raise PacingError(f"{where}: vsync gap {previous} -> {vsync}; native samples must be every vsync")
        previous = vsync
        frame_count = _int(row, "fc", where)
        if loops:
            key = loop_key(loops, frame_count)
        else:
            key = None
            if _int(row, "mode", where) == OPENING_MODE:
                key = "%d/%d/%d" % (_int(row, "stage", where), _int(row, "step", where), _int(row, "sub", where))
        frames.append({"frame_count": frame_count, "key": key})
    return frames


def port_frames(probe: dict) -> list[dict]:
    """Normalise the probe's logical ticks (nominal 60/1 clock steps) like vsyncs."""
    if not isinstance(probe, dict) or probe.get("schema") != PROBE_SCHEMA:
        raise PacingError("port receipt is not " + PROBE_SCHEMA)
    rows = probe.get("ticks")
    if not isinstance(rows, list) or not rows:
        raise PacingError("port receipt has no logical ticks")
    if not isinstance(probe.get("frames"), list):
        raise PacingError("port receipt has no host frames list")
    frames = []
    previous = None
    for index, row in enumerate(rows):
        where = f"port tick {index}"
        if not isinstance(row, dict):
            raise PacingError(where + ": not an object")
        tick = _int(row, "tick", where)
        if previous is not None and tick != previous + 1:
            raise PacingError(f"{where}: logical tick gap {previous} -> {tick}; every tick must be published")
        previous = tick
        key = row.get("key")
        if key is not None and not isinstance(key, str):
            raise PacingError(where + ": key must be a string or null")
        frames.append({"frame_count": _int(row, "frame_count", where), "key": key})
    return frames


def cadence(frames: list[dict], key_of=lambda _frame_count, sampled: sampled) -> dict:
    """Per substep: iterations and the vsyncs-per-iteration histogram.

    The first and last iteration of the sample window are partial (the window
    can start or stop inside them) and are excluded, as are iterations outside
    the opening.
    """
    runs: list[list] = []  # [frame_count, key of last frame, vsyncs]
    for frame in frames:
        if runs and frame["frame_count"] == runs[-1][0]:
            runs[-1][1] = frame["key"]
            runs[-1][2] += 1
            continue
        if runs and frame["frame_count"] < runs[-1][0]:
            raise PacingError("frame_count went backwards")
        # An iteration that ran without waiting for a vsync (the original's
        # catch-up after a multi-vsync iteration) has no sample of its own:
        # it is a zero-vsync iteration in the state the next sample shows.
        for skipped in range(runs[-1][0] + 1 if runs else frame["frame_count"], frame["frame_count"]):
            runs.append([skipped, key_of(skipped, frame["key"]), 0])
        runs.append([frame["frame_count"], frame["key"], 1])
    substeps: dict[str, dict] = {}
    order: list[str] = []
    for frame_count, key, vsyncs in runs[1:-1]:
        if key is None:
            continue
        if key not in substeps:
            substeps[key] = {"iterations": 0, "vsyncs": 0, "histogram": {}}
            order.append(key)
        entry = substeps[key]
        entry["iterations"] += 1
        entry["vsyncs"] += vsyncs
        entry["histogram"][str(vsyncs)] = entry["histogram"].get(str(vsyncs), 0) + 1
    return {"order": order, "substeps": substeps}


def percentile(values: list[float], fraction: float) -> float | None:
    """Nearest-rank percentile; None for an empty sample."""
    if not values:
        return None
    ordered = sorted(values)
    rank = max(1, -(-int(fraction * 1000) * len(ordered) // 1000))
    return ordered[min(rank, len(ordered)) - 1]


def host_metrics(probe: dict) -> dict:
    rows = probe["frames"]
    intervals = [float(r["interval_us"]) for r in rows[1:] if isinstance(r.get("interval_us"), (int, float))]
    process = [float(r["process_us"]) for r in rows if isinstance(r.get("process_us"), (int, float))]
    render = [float(r["render_us"]) for r in rows if isinstance(r.get("render_us"), (int, float))]
    median = percentile(intervals, 0.5)
    hitches = {str(f): [] for f in HITCH_FACTORS}
    if median:
        for index in range(1, len(rows)):
            interval = rows[index].get("interval_us")
            if not isinstance(interval, (int, float)):
                continue
            events = sorted({e for r in rows[max(0, index - 1):index + 1] for e in r.get("events", []) if e in EVENT_KINDS})
            for factor in HITCH_FACTORS:
                if interval > factor * median:
                    hitches[str(factor)].append({"frame": index, "interval_us": interval,
                                                 "attributed": events or ["unattributed"]})
    catch_up: dict[str, int] = {}
    for row in rows:
        if type(row.get("ticks")) is int:
            catch_up[str(row["ticks"])] = catch_up.get(str(row["ticks"]), 0) + 1
    host = probe.get("host") if isinstance(probe.get("host"), dict) else {}
    display = str(host.get("display", "unknown"))
    real_desktop = host.get("real_desktop") is True and display != "xvfb"
    return {
        "host": host,
        "label": "absolute_on_this_host" if real_desktop else "relative_only",
        "label_reason": ("real desktop session reported by the probe" if real_desktop else
                         "Xvfb or unknown host: compare only against runs on the same host"),
        "interval_us": {"p50": median, "p95": percentile(intervals, 0.95), "p99": percentile(intervals, 0.99)},
        "process_us": {"p50": percentile(process, 0.5), "p95": percentile(process, 0.95), "p99": percentile(process, 0.99)},
        "render_us": {"p50": percentile(render, 0.5), "p95": percentile(render, 0.95), "p99": percentile(render, 0.99)},
        "hitches": {factor: {"count": len(items), "items": items[:200]} for factor, items in hitches.items()},
        "bgm_underruns": sum(int(r.get("bgm_underruns", 0)) for r in rows),
        "ticks_per_host_frame": catch_up,
        "launch_to_first_interactive_ms": probe.get("launch_to_first_interactive_ms"),
    }


def compare(native: dict, port: dict, until: str | None = None) -> dict:
    """Compare substeps in native order; with until, stop before that key on both sides."""
    for side in (native, port):
        if until is not None and until in side["order"]:
            cut = side["order"].index(until)
            for key in side["order"][cut:]:
                side["substeps"].pop(key)
            side["order"] = side["order"][:cut]
    rows, mismatches = [], []
    keys = list(native["order"]) + [k for k in port["order"] if k not in native["substeps"]]
    for key in keys:
        left, right = native["substeps"].get(key), port["substeps"].get(key)
        row = {"key": key, "native": left, "port": right}
        if left is None or right is None:
            row["result"] = "missing_in_port" if right is None else "missing_in_native"
        elif left["iterations"] != right["iterations"]:
            row["result"] = "iteration_count_differs"
        elif left["histogram"] != right["histogram"]:
            row["result"] = "vsync_multiplicity_differs"
        else:
            row["result"] = "match"
        if row["result"] != "match":
            mismatches.append(key)
        rows.append(row)
    order_equal = native["order"] == port["order"]
    return {"substeps": rows, "mismatches": mismatches, "order_equal": order_equal,
            "passed": not mismatches and order_equal and bool(rows)}


def build_report(native_path: Path, port_path: Path, loops_path: Path | None = None,
                 until: str | None = None) -> dict:
    native_bytes = native_path.read_bytes()
    port_bytes = port_path.read_bytes()
    loops_bytes = loops_path.read_bytes() if loops_path is not None else None
    probe = json.loads(port_bytes)
    loops = loop_keys(read_jsonl(loops_bytes, loops_path.name)) if loops_bytes is not None else None
    key_of = (lambda frame_count, _sampled: loop_key(loops, frame_count)) if loops else (lambda _fc, sampled: sampled)
    native = cadence(native_frames(read_jsonl(native_bytes, str(native_path.name)), loops), key_of)
    port = cadence(port_frames(probe))
    result = compare(native, port, until)
    return {
        "schema": SCHEMA,
        "tier": "T1/T2 cadence for this compared run; host metrics are not T4 evidence",
        "native_keying": "loop_boundary" if loops else "sampled_state (single-iteration substeps can be absorbed)",
        "until": until,
        "inputs": {"native_sha256": _sha256(native_bytes), "port_sha256": _sha256(port_bytes),
                   "native_loops_sha256": _sha256(loops_bytes) if loops_bytes is not None else None,
                   "port_probe_script_sha256": probe.get("probe_script_sha256"),
                   "port_settings": probe.get("settings")},
        "cadence": result,
        "host_metrics": host_metrics(probe),
        "limitations": ["macOS native audio unvalidated until a T4 session records it",
                        "partial first/last iterations of each window are excluded"],
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--native", type=Path, required=True, help="emulator frames.jsonl (per-vsync samples)")
    parser.add_argument("--port", type=Path, required=True, help="pacing_probe.gd receipt JSON")
    parser.add_argument("--native-loops", type=Path, help="emulator loops.jsonl (loop-boundary keys)")
    parser.add_argument("--until", help="stop the comparison before this substep key, e.g. 12/0/0")
    parser.add_argument("--output", type=Path, required=True, help="new report path")
    args = parser.parse_args(argv)
    if args.output.exists():
        print("output exists: " + str(args.output), file=sys.stderr)
        return 2
    try:
        report = build_report(args.native, args.port, args.native_loops, args.until)
    except (PacingError, ValueError, OSError) as error:
        print("FAIL: " + str(error), file=sys.stderr)
        return 2
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=1, sort_keys=True) + "\n")
    cad = report["cadence"]
    print(("PASS" if cad["passed"] else "FAIL") + f": {len(cad['substeps'])} substeps, "
          f"{len(cad['mismatches'])} mismatches, host {report['host_metrics']['label']}")
    return 0 if cad["passed"] else 1


if __name__ == "__main__":
    sys.exit(main())
