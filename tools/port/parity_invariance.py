#!/usr/bin/env python3
"""T-39230: compare a modern-invariance run with its Parity run.

Each run directory comes from tests/support/run_parity_invariance_route.gd
(via tools/port/run_parity_invariance.sh) and holds parity-mode.json,
invariance-profile.json and invariance-frames.jsonl ({frame, phase, state,
image} per completed game iteration: state hash and native 320x240 RGBA8
hash). Rules:

* the parity run passed the Parity gate (play_mode parity, no violations);
* each modern run is an explicit modern-invariance run (play_mode modern,
  invariance_run true) whose profile matches its name;
* all runs used the same input script and the same feature registry;
* ``modern`` must equal parity frame by frame on phase, state and image;
  ``modern_state_only`` on phase and state;
* game_affecting entries were never enabled: every excluded registry entry is
  absent from the modern profiles' changes, and the report lists them with
  their declared invariants.

A different number of frames is a divergence, reported after the common
prefix compares equal. ``--control`` names a second Parity run of the same
script: if it differs from the first, the route itself is not reproducible
(for example a host-clock RNG seed) and the status is
``baseline_nondeterministic``; modern results are then inconclusive. The
report goes to --out (a new file).
Exit 0 invariant, 1 divergence or nondeterministic baseline, 2 invalid input.
"""
import argparse
import json
from pathlib import Path
import sys

try:
    from tools.port import parity_gate
except ModuleNotFoundError:  # run as a script from tools/port
    import parity_gate

PROFILE_SCHEMA = "tokimemo.parity-invariance-profile.v1"
FRAME_SCHEMA = "tokimemo.parity-invariance-frame.v1"
REPORT_SCHEMA = "tokimemo.parity-invariance-report.v1"


class Invalid(ValueError):
    pass


def load_run(directory, expected_profile):
    directory = Path(directory)
    try:
        identity = parity_gate.load_identity(directory)
        mode = parity_gate.check_identity(identity, allow_modern_invariance=expected_profile != "parity")
    except parity_gate.ParityError as error:
        raise Invalid("%s: %s" % (directory, error))
    if (expected_profile == "parity") != (mode == "parity"):
        raise Invalid("%s: profile %s ran in play mode %s" % (directory, expected_profile, mode))
    profile_path = directory / "invariance-profile.json"
    frames_path = directory / "invariance-frames.jsonl"
    if not profile_path.is_file() or not frames_path.is_file():
        raise Invalid("%s: missing invariance-profile.json or invariance-frames.jsonl" % directory)
    profile = json.loads(profile_path.read_text())
    if profile.get("schema") != PROFILE_SCHEMA or profile.get("profile") != expected_profile:
        raise Invalid("%s: profile record is not %s" % (directory, expected_profile))
    frames = []
    for number, line in enumerate(frames_path.read_text().splitlines(), 1):
        record = json.loads(line)
        if record.get("schema") != FRAME_SCHEMA or not isinstance(record.get("frame"), int) \
                or not all(isinstance(record.get(k), str) for k in ("phase", "state", "image")):
            raise Invalid("%s:%d: not an invariance frame record" % (frames_path, number))
        frames.append(record)
    if not frames:
        raise Invalid("%s: no frames recorded" % frames_path)
    return {"dir": str(directory), "identity": identity, "profile": profile, "frames": frames}


def compare_frames(parity, other, fields):
    for index, (left, right) in enumerate(zip(parity, other)):
        for field in fields:
            if left.get(field) != right.get(field):
                divergence = {"index": index, "frame": left.get("frame"), "field": field,
                              "parity": left.get(field), "other": right.get(field)}
                parts_left, parts_right = left.get("parts"), right.get("parts")
                if field == "state" and isinstance(parts_left, dict) and isinstance(parts_right, dict):
                    divergence["parts"] = sorted(k for k in set(parts_left) | set(parts_right)
                                                 if parts_left.get(k) != parts_right.get(k))
                return divergence
    if len(parity) != len(other):
        return {"index": min(len(parity), len(other)), "field": "length", "parity": len(parity), "other": len(other)}
    return None


def image_mismatches(parity, other):
    """Frame indexes whose native Image differs (diagnostic, all frames)."""
    return [i for i, (a, b) in enumerate(zip(parity, other)) if a.get("image") != b.get("image")]


def compare(parity_dir, runs, control_dir=None):
    """runs: {"modern": dir, "modern_state_only": dir} (either may be absent);
    control_dir: an optional second Parity run of the same script."""
    if not runs:
        raise Invalid("nothing to compare: give --modern and/or --state-only")
    parity = load_run(parity_dir, "parity")
    excluded = {row["id"]: row for row in parity["profile"].get("excluded", [])}
    if not excluded:
        raise Invalid("parity profile lists no excluded entries; the registry record is incomplete")
    control = None
    if control_dir is not None:
        repeat = load_run(control_dir, "parity")
        if repeat["profile"].get("input_script_sha256") != parity["profile"].get("input_script_sha256"):
            raise Invalid("control: input_script_sha256 differs from the parity run")
        divergence = compare_frames(parity["frames"], repeat["frames"], ["frame", "phase", "state", "image"])
        control = {"dir": repeat["dir"], "frames": len(repeat["frames"]),
                   "image_mismatches": image_mismatches(parity["frames"], repeat["frames"]),
                   "status": "reproducible" if divergence is None else "diverged", "first_divergence": divergence}
    results = {}
    for name, directory in runs.items():
        run = load_run(directory, name)
        for key in ("input_script_sha256", "registry_sha256"):
            if run["profile"].get(key) != parity["profile"].get(key):
                raise Invalid("%s: %s differs from the parity run" % (name, key))
        leaked = sorted(set(run["profile"].get("changes", {})) & (set(excluded) - {"profile.mode"}))
        if leaked:
            raise Invalid("%s enabled excluded entries: %s" % (name, ", ".join(leaked)))
        fields = ["frame", "phase", "state"] + (["image"] if name == "modern" else [])
        divergence = compare_frames(parity["frames"], run["frames"], fields)
        results[name] = {"dir": run["dir"], "compared": fields, "frames": len(run["frames"]),
                         "image_mismatches": image_mismatches(parity["frames"], run["frames"]),
                         "changes": sorted(run["profile"].get("changes", {})),
                         "status": "invariant" if divergence is None else "diverged", "first_divergence": divergence}
    return {"schema": REPORT_SCHEMA, "parity": {"dir": parity["dir"], "frames": len(parity["frames"])},
            "registry_sha256": parity["profile"]["registry_sha256"], "input_script_sha256": parity["profile"]["input_script_sha256"],
            "status": "baseline_nondeterministic" if control and control["status"] != "reproducible"
            else "invariant" if all(r["status"] == "invariant" for r in results.values()) else "diverged",
            "control": control, "runs": results,
            "excluded": [{"id": k, "class": v.get("class"), "invariant": v.get("invariant")} for k, v in sorted(excluded.items())],
            "scope": "frames the runs reached; game_affecting entries are excluded and covered only by their declared invariants"}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--parity", type=Path, required=True)
    parser.add_argument("--modern", type=Path)
    parser.add_argument("--state-only", type=Path)
    parser.add_argument("--control", type=Path, help="a second Parity run of the same script (reproducibility control)")
    parser.add_argument("--out", type=Path, required=True, help="new report file")
    args = parser.parse_args(argv)
    runs = {name: path for name, path in (("modern", args.modern), ("modern_state_only", args.state_only)) if path}
    try:
        report = compare(args.parity, runs, args.control)
        with args.out.open("x") as handle:
            json.dump(report, handle, indent=1, sort_keys=True)
    except (Invalid, OSError, ValueError, KeyError) as error:
        print(json.dumps({"status": "invalid", "error": str(error)}), file=sys.stderr)
        return 2
    print(json.dumps({"status": report["status"], "report": str(args.out),
                      "runs": {k: v["status"] for k, v in report["runs"].items()}}))
    return 0 if report["status"] == "invariant" else 1


if __name__ == "__main__":
    raise SystemExit(main())
