#!/usr/bin/env python3
"""T-39022 (T-39018 WP-05): opening per-iteration state/RNG traces for compare_trace.py.

Builds two traces in the compare_trace.py record format ({iteration, phase,
source, state}) from

* native: an emu.py run with --sample-every 1, the dumps the projection config
  requires and an exec breakpoint on rand (result.json hits), projected through
  native_memory_projection.project_sample with config/port/opening-trace-projection.json;
* port: the JSONL written by port/godot/tests/opening/run_opening_trace_emitter.gd,

aligns them by iteration = frame_count - frame_count at the opening onset
(0x15 0/0/0) on each side, applies the config's compare rules, and reports the
first divergent record with compare_trace.compare. Excluded fields (lip-sync
byte, CPU cycles, vsync) never enter a record. Native iterations no vsync sample
holds are listed and dropped from both traces; nothing is interpolated.

Fields compared ``when_port_defined`` are compared only where the port state
holds them. A configured field the port never defines is listed in
``never_defined`` and makes the report unequal; native values left out by the
gating are counted per field in ``gated_native_values``.

usage: opening_trace.py --config C --native-dir D --port-trace P --out NEW_DIR
Exit: 0 traces equal, 1 first divergence or never-defined field reported,
2 invalid input.
"""
import argparse
import bisect
import hashlib
import json
from pathlib import Path
import sys

try:
    from tools.port import compare_trace
    from tools.port.native_memory_projection import project_sample
except ModuleNotFoundError:
    import compare_trace
    from native_memory_projection import project_sample

SOURCE = "opening-trace-projection.v1"
RAND = 0x800AE0D0
STATE_KEYS = ("game_mode", "mode_stage", "mode_step", "substep")


class Invalid(ValueError):
    pass


def select(value, path):
    for key in path:
        if not isinstance(value, dict) or key not in value:
            return None
        value = value[key]
    return value


def at(state, predicate):
    return all(state[key] == predicate[key] for key in STATE_KEYS)


def native_rows(samples, hits, config, map_sha):
    """One raw row per native iteration: first vsync sample holding its frame_count.

    ``samples``/``hits`` may also be a list of (samples, hits) parts: consecutive emulator
    runs where each part starts from the previous part's final save state (emu.py keeps at
    most 4096 samples per run). Vsync numbers restart per part; rand counts carry over."""
    parts = samples if hits is None else [(samples, hits)]
    rows, seen, missing = [], set(), []
    previous = None
    base = 0
    for part_samples, part_hits in parts:
        rand_frames = sorted(hit["frame"] for hit in part_hits)
        for sample in part_samples:
            if sample.get("state_load_phase") == "before_restore":
                continue
            fields = project_sample(sample, config, map_sha)["fields"]
            fc = fields["frame_count"]
            if fc in seen:
                continue
            if previous is not None:
                if fc < previous:
                    raise Invalid("native frame_count went backwards at vsync %d" % sample["frame"])
                missing.extend(range(previous + 1, fc))
            seen.add(fc)
            previous = fc
            fields["rand_calls"] = base + bisect.bisect_left(rand_frames, sample["frame"])
            fields["vsync"] = sample["frame"]
            rows.append(fields)
        base += len(rand_frames)
    return rows, missing


def port_rows(records, config, reseeds=None):
    """Raw port rows. The session RNG's draw_count restarts at 0 when the game reseeds
    (srand), so the cumulative count adds each iteration's increase, or the new count
    after a reseed; draws made earlier in a reseed iteration are not visible to it.
    Reseed iterations (frame_count) are appended to ``reseeds``."""
    rows, previous, missing = [], None, []
    last_draws, total = None, 0
    for record in records:
        fc = record["frame_count"]
        if previous is not None and fc != previous + 1:
            if fc <= previous:
                raise Invalid("port frame_count not increasing at %d" % fc)
            missing.extend(range(previous + 1, fc))
        previous = fc
        draws = select(record, config["rng"]["port"])
        if draws is not None:
            draws = int(draws)
            if last_draws is not None and draws < last_draws:
                total += draws
                if reseeds is not None:
                    reseeds.append(fc)
            elif last_draws is not None:
                total += draws - last_draws
            last_draws = draws
        row = {"frame_count": fc, "rand_calls": None if draws is None else total}
        for field in config["fields"]:
            row[field["name"]] = select(record, field["port_path"])
        rows.append(row)
    return rows, missing


def window(rows, config, side):
    onset, end = config["alignment"]["onset"], config["alignment"]["end"]
    start = next((i for i, row in enumerate(rows) if at(row, onset)), None)
    if start is None:
        raise Invalid("%s trace never reaches the opening onset" % side)
    stop = next((i for i in range(start, len(rows)) if at(rows[i], end)), len(rows) - 1)
    return rows[start:stop + 1], rows[start]["frame_count"], rows[start]["rand_calls"], stop < len(rows) - 1 or at(rows[-1], end)


def records(rows, origin, rand_origin, config, defined):
    """compare_trace rows. A when_port_defined field is compared only on iterations in
    ``defined[name]`` (where the port state holds it); both sides record null elsewhere."""
    out = []
    for row in rows:
        iteration = row["frame_count"] - origin
        state = {"rand_calls": None if row["rand_calls"] is None else row["rand_calls"] - rand_origin}
        for field in config["fields"]:
            name, rule = field["name"], field["compare"]
            if rule == "alignment_only":
                continue
            active = rule != "when_port_defined" or iteration in defined.get(name, ())
            state[name] = row["value_" + name] if active else None
        out.append({"iteration": iteration, "phase": "post_update", "source": SOURCE, "state": state})
    return out


def build(config, samples, hits, port_records, map_sha):
    """Return (native records, port records, report fields). ``hits`` None: ``samples`` is a part list."""
    nrows, native_missing = native_rows(samples, hits, config, map_sha)
    reseeds = []
    prows, port_missing = port_rows(port_records, config, reseeds)
    if port_missing:
        raise Invalid("port trace skipped iterations: %s" % port_missing[:10])
    nwin, norigin, nrand, nend = window(nrows, config, "native")
    pwin, porigin, prand, pend = window(prows, config, "port")
    derived = {d["from"]: d for d in config.get("derived", [])}
    for row in nwin:
        for field in config["fields"]:
            value = row[field["name"]]
            if field["name"] in derived:
                value = (value & 0x80000000) == 0
            row["value_" + field["name"]] = value
    for row in pwin:
        for field in config["fields"]:
            value = row[field["name"]]
            if field["name"] in derived:
                value = None if value is None else int(value) == 1
            elif isinstance(value, float) and value == int(value):
                value = int(value)
            row["value_" + field["name"]] = value
    defined = {}
    for row in pwin:
        for field in config["fields"]:
            if field["compare"] == "when_port_defined" and row["value_" + field["name"]] is not None:
                defined.setdefault(field["name"], set()).add(row["frame_count"] - porigin)
    # Iterations where a field the port had defined is absent again (an actor cleared, say)
    # are reported as spans, so the gating is visible rather than silent.
    gaps = {}
    for name, iterations in defined.items():
        spans, start = [], min(iterations)
        for i in range(start, pwin[-1]["frame_count"] - porigin + 1):
            if i not in iterations:
                if spans and spans[-1][1] == i - 1:
                    spans[-1][1] = i
                else:
                    spans.append([i, i])
        if spans:
            gaps[name] = spans
    defined_from = {name: min(iterations) for name, iterations in defined.items()}
    native = records(nwin, norigin, nrand, config, defined)
    port = records(pwin, porigin, prand, config, defined)
    unobserved = sorted({fc - norigin for fc in native_missing if norigin <= fc} & {r["iteration"] for r in port})
    port = [r for r in port if r["iteration"] not in set(unobserved)]
    for row in native + port:
        if "game_mode" in row["state"] and row["state"]["game_mode"] is not None:
            row["state"]["game_mode"] = int(row["state"]["game_mode"])
    gated_fields = [f["name"] for f in config["fields"] if f["compare"] == "when_port_defined"]
    # A gated field the port never writes is never compared: that is a failure, not equality.
    never_defined = [name for name in gated_fields if name not in defined]
    # Native values that the gating keeps out of the comparison (stale pool bytes, or a
    # write the port lacks), so the reader sees what was not compared.
    gated_native = {}
    dropped = set(unobserved)
    for row in nwin:
        iteration = row["frame_count"] - norigin
        if iteration in dropped:
            continue
        for name in gated_fields:
            value = row["value_" + name]
            if iteration in defined.get(name, ()) or value is None or value is False or value == 0:
                continue
            entry = gated_native.setdefault(name, {"count": 0, "first_iteration": iteration, "before_port_definition": 0,
                                                   "after_port_definition": 0, "first_value": value})
            entry["count"] += 1
            entry["last_iteration"] = iteration
            first = defined_from.get(name)
            entry["before_port_definition" if first is None or iteration < first else "after_port_definition"] += 1
    info = {"native_onset_frame_count": norigin, "port_onset_frame_count": porigin,
            "native_reached_end": nend, "port_reached_end": pend,
            "native_unobserved_iterations": unobserved, "port_defined_from": defined_from,
            "never_defined": never_defined, "gated_native_values": gated_native,
            "port_undefined_after_first": gaps,
            "port_reseed_iterations": [fc - porigin for fc in reseeds],
            "native_records": len(native), "port_records": len(port)}
    return native, port, info


def first_by_field(native, port):
    """First divergent iteration of each state field on its own (later divergences that
    compare_trace's single first divergence hides)."""
    result = {}
    for left, right in zip(native, port):
        for name in left["state"]:
            if name not in result and left["state"][name] != right["state"].get(name):
                result[name] = {"iteration": left["iteration"], "native": left["state"][name], "port": right["state"].get(name)}
    return dict(sorted(result.items(), key=lambda item: item[1]["iteration"]))


def native_part(directory):
    """(samples, rand hits) of one emu.py run; hits before a state restore belong to the boot, not the game."""
    result = json.loads((directory / "result.json").read_text())
    samples_path = directory / "memory-samples.jsonl"
    if hashlib.sha256(samples_path.read_bytes()).hexdigest() != result["memory_samples"]["sha256"]:
        raise Invalid("memory samples hash differs from result.json: %s" % directory)
    samples = [json.loads(line) for line in samples_path.read_text().splitlines() if line.strip()]
    hits = [h for h in result.get("hits", []) if h.get("address") == RAND or h.get("pc") == RAND]
    if result.get("counts") and sum(result["counts"].values()) != len(hits):
        raise Invalid("rand hits were not all recorded (raise --hits): %s" % directory)
    restore = [e for e in result.get("lifecycle", []) if e.get("kind") == "state_restore"]
    if restore:
        hits = [h for h in hits if h["cycles"] >= restore[0]["cycles_after"]]
    return samples, hits


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--native-dir", type=Path, action="append", required=True,
                        help="emu.py output (result.json, memory-samples.jsonl); repeat for consecutive parts in order")
    parser.add_argument("--port-trace", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args(argv)
    try:
        raw_config = args.config.read_bytes()
        config = json.loads(raw_config)
        parts = [native_part(directory) for directory in args.native_dir]
        port = [json.loads(line) for line in args.port_trace.read_text().splitlines() if line.strip()]
        native, port_trace, info = build(config, parts, None, port, hashlib.sha256(raw_config).hexdigest())
    except (Invalid, OSError, KeyError, ValueError) as error:
        print(json.dumps({"status": "invalid", "error": str(error)}))
        return 2
    args.out.mkdir(parents=True, exist_ok=False)
    for name, rows in (("native", native), ("port", port_trace)):
        with (args.out / ("%s.trace.jsonl" % name)).open("x") as handle:
            for row in rows:
                handle.write(json.dumps(row, sort_keys=True) + "\n")
    mismatch = compare_trace.compare(native, port_trace)
    equal = mismatch is None and not info["never_defined"]
    report = dict(info, schema="tokimemo.opening-trace-report.v1", equal=equal, first_divergence=mismatch,
                  first_divergence_by_field=first_by_field(native, port_trace),
                  config_sha256=hashlib.sha256(raw_config).hexdigest(),
                  excluded=[e["name"] for e in config.get("excluded", [])])
    with (args.out / "report.json").open("x") as handle:
        json.dump(report, handle, indent=1, sort_keys=True)
    print(json.dumps({"equal": equal, "first_divergence": mismatch, "never_defined": info["never_defined"]},
                     sort_keys=True))
    return 0 if equal else 1


if __name__ == "__main__":
    sys.exit(main())
