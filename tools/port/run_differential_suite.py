#!/usr/bin/env python3
"""T-39021 (T-39018 WP-04): differential suite over the playtest checkpoint catalog.

Subcommands:

* ``plan``: write the capture plan for the selected catalog entries: the
  PCSX-Redux run (Docker tokimemo-emu via tools/emu/run.sh) and the port
  observer run (Docker tokimemo-port-gui-test), plus the pairing step
  (tools/port/rei_stage_capture_pairs.py). ``--execute`` runs the plan's
  commands in order and stops at the first failure.
* ``captures-from-pairs``: turn a directory of capture-pair manifests
  (pair-<observer key>.json) into a captures file for ``compare``.
* ``compare``: compare the captures with the catalog's expectations and write
  one suite report.

Result rules (exactly one per selected entry):

* exact frame: ``exact_pass`` only on an exact RGBA match (compare_capture_pair
  pixels, unchanged). If the captures carry a pair manifest, its full gate must
  also exit 0, else ``exact_blocked``. Any pixel difference is ``exact_fail``.
* known_diff frame: the rgb555_diagnostic projection with the catalog's
  declared encodings and exclusions. Residual equal to the pin with the same
  bbox is ``known_diff_stable``; a smaller residual whose bbox lies inside the
  pinned one is ``known_diff_improved`` (re-pin advised); more residual pixels
  is ``known_diff_regressed``; any residual outside the pinned bbox is
  ``known_diff_bbox_moved``; the same residual count with a different bbox
  inside the pin is ``known_diff_changed``; an exact RGBA match on the full
  frame (compare_capture_pair pixels ``equal``) is ``promote``; no difference
  outside the declared exclusions while the full frame still differs (for
  example only the emulator x=0 column) is
  ``known_diff_clean_except_exclusions`` (re-pin advised, never a promotion);
  values outside the declared encoding are ``known_diff_rejected``.
* frame_timing: the port's transitions (run_opening_frame_timing_route.gd
  transitions.json) give the iterations of each substep; equal to the fixture
  is ``exact_pass``, else ``exact_fail``.
* unsynchronizable: ``unsynchronizable_listed``; never compared.
* state entries: ``not_implemented`` (fails when selected).
* a selected entry without captures: ``missing_capture`` (fails).

Pass results: exact_pass, known_diff_stable, known_diff_improved,
known_diff_clean_except_exclusions, promote, unsynchronizable_listed. Everything
else fails the suite.

Reports and images go to a new directory under .asset-cache (or outside the
repository); the tool refuses a repository path elsewhere.
Exit codes: 0 suite passed, 1 a result failed, 2 invalid input.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

from PIL import Image

try:
    from tools.port import compare_capture_pair as gate
    from tools.port import playtest_catalog
    from tools.port import rgb555_diagnostic as rgb555
except ModuleNotFoundError:  # run as a script from tools/port
    import compare_capture_pair as gate
    import playtest_catalog
    import rgb555_diagnostic as rgb555

ROOT = Path(__file__).resolve().parents[2]
PASS = {"exact_pass", "known_diff_stable", "known_diff_improved", "known_diff_clean_except_exclusions", "promote",
        "unsynchronizable_listed"}
CAPTURES_SCHEMA = "tokimemo.differential-captures.v1"
REPORT_SCHEMA = "tokimemo.differential-suite-report.v1"
# Dumps the Rei stage pairing needs (README.capture-pair.md), plus the checker
# phase words T-39008 C1 asks for in the next native run.
REI_DUMPS = ["800E7374:32", "8014D180:12", "801220E0:4", "80120650:72", "80129628:2", "800B5BC8:12"]


class Invalid(ValueError):
    pass


def sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def guarded_output(path, repo=ROOT):
    """New output directory under .asset-cache, or outside the repository."""
    path = Path(path).absolute()
    if path.exists():
        raise Invalid("output exists: %s" % path)
    try:
        rel = path.relative_to(repo)
    except ValueError:
        return path
    if not rel.parts or rel.parts[0] != ".asset-cache":
        raise Invalid("suite output inside the repository must be under .asset-cache: %s" % path)
    return path


def select(catalog, keys=None, groups=None):
    entries = catalog["checkpoints"]
    if keys:
        known = {e["key"] for e in entries}
        missing = sorted(set(keys) - known)
        if missing:
            raise Invalid("unknown catalog keys: " + ", ".join(missing))
        entries = [e for e in entries if e["key"] in keys]
    if groups:
        entries = [e for e in entries if e["group"] in groups]
    if not entries:
        raise Invalid("selection is empty")
    return entries


def pinned_file(base, spec):
    if not isinstance(spec, dict) or not isinstance(spec.get("path"), str):
        raise Invalid("capture file reference must be {path, sha256}")
    path = Path(spec["path"])
    path = path if path.is_absolute() else base / path
    if not path.is_file():
        raise Invalid("missing capture file: %s" % path)
    if sha256(path) != spec.get("sha256"):
        raise Invalid("capture file hash changed: %s" % path)
    return path


def open_png(path):
    with Image.open(path) as image:
        if image.format != "PNG":
            raise Invalid("capture images must be PNG: %s" % path)
        image.load()
        return image.copy()


def within(inner, outer):
    if inner is None:
        return True
    return outer is not None and outer[0] <= inner[0] and outer[1] <= inner[1] and inner[2] <= outer[2] and inner[3] <= outer[3]


def known_diff_result(expected, measured, exact_status):
    """Classify one rgb555 projection against the pinned known_diff (pure; unit-tested).

    ``exact_status`` is compare_capture_pair's pixel status for the whole frame.
    Only that gate can say a frame is exact, so ``promote`` needs it to be
    ``equal``; the projection's exact count leaves the declared exclusions out.
    """
    if exact_status == "equal":
        return "promote"
    exact = measured["exact_rgba8"]["pixels"]
    residual = measured["rgb555_residual"]["pixels"]
    box = measured["rgb555_residual"]["bbox"]
    if exact == 0:
        return "known_diff_clean_except_exclusions"
    if residual > expected["residual_pixels"]:
        return "known_diff_regressed"
    if not within(box, expected["bbox"]):
        return "known_diff_bbox_moved"
    if residual == expected["residual_pixels"]:
        return "known_diff_stable" if box == expected["bbox"] else "known_diff_changed"
    return "known_diff_improved"


def compare_frame(entry, capture, base, out):
    native_path = pinned_file(base, capture.get("native"))
    port_path = pinned_file(base, capture.get("port"))
    expected = entry["expected"]
    row = {"native_sha256": capture["native"]["sha256"], "port_sha256": capture["port"]["sha256"]}
    pixels = gate.pixels(native_path, port_path)
    row["exact_rgba"] = {k: pixels.get(k) for k in ("status", "differing_pixels", "native_size", "port_size")}
    if expected["status"] == "exact":
        if pixels["status"] != "equal":
            return dict(row, result="exact_fail")
        manifest = capture.get("pair_manifest")
        if manifest is None:
            return dict(row, result="exact_pass", note="pixels equal; no pair manifest, so no synchronization or field check ran")
        manifest_path = pinned_file(base, manifest)
        raw = manifest_path.read_bytes()
        report, code = gate.compare(json.loads(raw), manifest_path.parent)
        row["capture_pair_gate"] = {"exit": code, "status": report["status"], "blockers": report["acceptance_blockers"]}
        return dict(row, result="exact_pass" if code == 0 else "exact_blocked")
    if pixels["status"] == "dimension_mismatch":
        return dict(row, result="known_diff_rejected", error="dimension mismatch")
    try:
        measured = rgb555.projection_report(open_png(native_path), open_png(port_path), expected["encodings"]["native"],
                                            expected["encodings"]["port"], expected["exclusions"])
    except rgb555.Rejected as error:
        return dict(row, result="known_diff_rejected", error=str(error), outside_encoding=error.detail)
    with (out / ("rgb555-%s.json" % entry["key"])).open("x") as handle:
        json.dump(measured, handle, indent=1, sort_keys=True)
    row["rgb555"] = {"residual_pixels": measured["rgb555_residual"]["pixels"], "bbox": measured["rgb555_residual"]["bbox"],
                     "pinned_residual_pixels": expected["residual_pixels"], "pinned_bbox": expected["bbox"],
                     "exact_rgba_pixels": measured["exact_rgba8"]["pixels"],
                     "domain_exclusions": measured["domain_exclusions"]}
    result = known_diff_result(expected, measured, pixels["status"])
    if result == "known_diff_clean_except_exclusions":
        row["note"] = ("no difference outside the declared exclusions, but the exact RGBA gate still differs "
                       "(%s pixels); not a promotion" % pixels.get("differing_pixels"))
    return dict(row, result=result, defect_id=expected["defect_id"])


def timing_tables(captures, base, root):
    timing = captures.get("timing")
    if timing is None:
        return None
    transitions = json.loads(pinned_file(base, timing.get("port_transitions")).read_text())["transitions"]
    fixture = json.loads((root / "port/godot/tests/fixtures/opening_frame_timing.json").read_text())
    pad = str(timing.get("pad"))
    if pad not in fixture["pads"]:
        raise Invalid("timing pad %s is not in the fixture" % pad)
    rows = fixture["pads"][pad]["substeps"]
    reference = {key: count for key, count in rows}
    order = [key for key, _ in rows] + ["12/0/0"]
    port = {}
    for key, nxt in zip(order, order[1:]):
        if key in transitions and nxt in transitions:
            port[key] = int(transitions[nxt]) - int(transitions[key])
    return {"pad": pad, "reference": reference, "port": port}


def compare(catalog, captures, base, out, keys=None, groups=None, root=ROOT):
    if captures.get("schema") != CAPTURES_SCHEMA:
        raise Invalid("unsupported captures file")
    entries = select(catalog, keys, groups)
    out.mkdir(parents=True, exist_ok=False)
    timing = timing_tables(captures, base, root)
    frames = captures.get("frames", {})
    results = []
    for entry in entries:
        key, status = entry["key"], entry["expected"]["status"]
        row = {"key": key, "kind": entry["kind"], "expected": status}
        if status == "unsynchronizable":
            row.update(result="unsynchronizable_listed", reason=entry["expected"]["reason"])
            if key in frames:
                row["note"] = "capture supplied but never compared"
        elif entry["kind"] == "frame":
            capture = frames.get(key)
            row.update(compare_frame(entry, capture, base, out) if capture else {"result": "missing_capture"})
        elif entry["kind"] == "frame_timing":
            substep = entry["timing_reference"]["substep"]
            if timing is None or substep not in timing["port"]:
                row["result"] = "missing_capture"
            else:
                want, got = timing["reference"][substep], timing["port"][substep]
                row.update(result="exact_pass" if want == got else "exact_fail", pad=timing["pad"],
                           native_iterations=want, port_iterations=got)
        else:
            row["result"] = "not_implemented"
        results.append(row)
    counts = {}
    for row in results:
        counts[row["result"]] = counts.get(row["result"], 0) + 1
    failed = [row["key"] for row in results if row["result"] not in PASS]
    promote = [row["key"] for row in results if row["result"] == "promote"]
    repin = [row["key"] for row in results
             if row["result"] in ("known_diff_improved", "known_diff_clean_except_exclusions")]
    report = {"schema": REPORT_SCHEMA, "status": "failed" if failed else "passed",
              "scope": "selected catalog entries only; exact RGBA gate unchanged; rgb555 residuals are known_diff bookkeeping, not parity acceptance",
              "catalog_sha256": hashlib.sha256(json.dumps(catalog, sort_keys=True).encode()).hexdigest(),
              "captures_sha256": hashlib.sha256(json.dumps(captures, sort_keys=True).encode()).hexdigest(),
              "counts": counts, "failed": failed, "promote": promote, "repin_advised": repin,
              "results": results}
    with (out / "suite-report.json").open("x") as handle:
        json.dump(report, handle, indent=1, sort_keys=True)
    return report


def captures_from_pairs(pairs_dir, out_file, catalog, transitions=None, pad=None):
    """Captures file from pair-<observer key>.json manifests (rei_stage_capture_pairs.py output),
    optionally with an opening frame-timing transitions.json for one fixture pad."""
    pairs_dir = Path(pairs_dir).absolute()
    observer = {e["port_observer_key"]: e["key"] for e in catalog["checkpoints"] if e["kind"] == "frame"}
    frames = {}
    for manifest in sorted(pairs_dir.glob("pair-*.json")):
        if manifest.name.endswith("-report.json"):
            continue
        observer_key = manifest.stem[len("pair-"):]
        if observer_key not in observer:
            raise Invalid("pair %s has no catalog entry" % manifest.name)
        pair = json.loads(manifest.read_text())
        frames[observer[observer_key]] = {
            side: {"path": str((manifest.parent / pair[side]["image"]["path"]).resolve()), "sha256": pair[side]["image"]["sha256"]}
            for side in ("native", "port")}
        frames[observer[observer_key]]["pair_manifest"] = {"path": str(manifest), "sha256": sha256(manifest)}
    if not frames:
        raise Invalid("no pair manifests in %s" % pairs_dir)
    captures = {"schema": CAPTURES_SCHEMA, "source": str(pairs_dir), "frames": frames}
    if transitions is not None:
        captures["timing"] = {"pad": str(pad), "port_transitions": {"path": str(Path(transitions).absolute()), "sha256": sha256(transitions)}}
    with Path(out_file).open("x") as handle:
        json.dump(captures, handle, indent=1, sort_keys=True)
    return frames


def repo_relative(path, root=ROOT):
    """Docker steps mount the repository at /work, so plan paths must lie inside it."""
    try:
        return Path(path).absolute().relative_to(root)
    except ValueError:
        raise Invalid("plan paths must be inside the repository (.asset-cache): %s" % path)


def plan(entries, out, native_state, port_bundle, frames, shots, input_args=(), root=ROOT):
    """Capture commands for the Rei/opening frame entries (argv lists, run from the repo root, Docker only)."""
    if not any(e["kind"] == "frame" and e["expected"]["status"] != "unsynchronizable" for e in entries):
        raise Invalid("selection has no comparable frame entries")
    rel_out = repo_relative(out, root)
    native_out, port_out, pairs_out = rel_out / "native", rel_out / "port", rel_out / "pairs"
    native = ["tools/emu/run.sh", "boot", "--state", str(repo_relative(native_state, root)), "--frames", str(frames),
              "--sample-every", "1", "--out", str(native_out)]
    for dump in REI_DUMPS:
        native += ["--dump", dump]
    if shots:
        native += ["--shot", ",".join(str(v) for v in shots)]
    native += list(input_args)
    port = ["tools/port/run_opening_frame_timing.sh", str(port_out), "2"]
    name = "tokimemo-diffsuite-pairs-" + rel_out.name
    pairing = ["docker", "run", "--rm", "--name", name, "-v", "%s:/work" % root, "-w", "/work", "tokimemo-recovery",
               "python3", "tools/port/rei_stage_capture_pairs.py", "--native-dir", str(native_out),
               "--port-dir", str(port_out), "--observer", "port/godot/tests/opening/run_installed_rei_banner.gd",
               "--out", str(pairs_out), "--latency", "2",
               "--latency-basis", "T-39008 latency_probe onsets (text-window clear, stage8 white boxes)"]
    return {"schema": "tokimemo.differential-capture-plan.v1", "out": str(rel_out),
            "keys": [e["key"] for e in entries],
            "env": {"OPENING_TIMING_BUNDLE": str(repo_relative(port_bundle, root))},
            "steps": [{"name": "native", "image": "tokimemo-emu", "argv": native},
                      {"name": "port", "image": "tokimemo-port-gui-test", "argv": port},
                      {"name": "pairs", "image": "tokimemo-recovery", "argv": pairing}]}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="cmd", required=True)
    for name in ("compare", "plan"):
        p = sub.add_parser(name)
        p.add_argument("--catalog", type=Path, default=playtest_catalog.CATALOG)
        p.add_argument("--keys", help="comma-separated catalog keys")
        p.add_argument("--group", action="append", help="catalog group (repeatable)")
        p.add_argument("--out", type=Path, required=True, help="new directory under .asset-cache")
    sub.choices["compare"].add_argument("--captures", type=Path, required=True)
    p = sub.choices["plan"]
    p.add_argument("--native-state", type=Path, required=True)
    p.add_argument("--port-bundle", type=Path, required=True)
    p.add_argument("--frames", type=int, required=True)
    p.add_argument("--shots", default="", help="vsyncs to screenshot: V,V,...")
    p.add_argument("--execute", action="store_true", help="run the steps (Docker) after writing the plan")
    p = sub.add_parser("captures-from-pairs")
    p.add_argument("pairs_dir", type=Path)
    p.add_argument("--catalog", type=Path, default=playtest_catalog.CATALOG)
    p.add_argument("--out", type=Path, required=True, help="new captures JSON file")
    p.add_argument("--timing-transitions", type=Path, help="run_opening_frame_timing_route.gd transitions.json")
    p.add_argument("--timing-pad", choices=["2", "3"], help="fixture pad of that run")
    args = parser.parse_args(argv)
    try:
        catalog = playtest_catalog.load(args.catalog)
        errors = playtest_catalog.semantic_errors(catalog)
        if errors:
            raise Invalid("catalog is inconsistent: " + "; ".join(errors[:5]))
        if args.cmd == "captures-from-pairs":
            if (args.timing_transitions is None) != (args.timing_pad is None):
                raise Invalid("--timing-transitions and --timing-pad go together")
            frames = captures_from_pairs(args.pairs_dir, args.out, catalog, args.timing_transitions, args.timing_pad)
            print(json.dumps({"captures": str(args.out), "frames": len(frames)}))
            return 0
        out = guarded_output(args.out)
        keys = set(args.keys.split(",")) if args.keys else None
        if args.cmd == "compare":
            report = compare(catalog, json.loads(args.captures.read_text()), args.captures.parent.absolute(), out,
                             keys, args.group)
            print(json.dumps({"status": report["status"], "counts": report["counts"], "report": str(out / "suite-report.json")}))
            return 0 if report["status"] == "passed" else 1
        shots = [int(v) for v in args.shots.split(",") if v]
        steps = plan(select(catalog, keys, args.group), out, args.native_state, args.port_bundle, args.frames, shots)
        out.mkdir(parents=True)
        with (out / "plan.json").open("x") as handle:
            json.dump(steps, handle, indent=1)
        if args.execute:
            for step in steps["steps"]:
                env = dict(os.environ, **steps["env"])
                if subprocess.run(step["argv"], cwd=ROOT, env=env).returncode != 0:
                    print("step failed: " + step["name"], file=sys.stderr)
                    return 1
        print(json.dumps({"plan": str(out / "plan.json"), "steps": [s["name"] for s in steps["steps"]]}))
        return 0
    except (Invalid, OSError, KeyError, ValueError) as error:
        print(json.dumps({"status": "invalid", "error": str(error)}), file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
