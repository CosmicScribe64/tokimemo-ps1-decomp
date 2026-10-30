#!/usr/bin/env python3
"""T-39018 WP-01: load and check the playtest checkpoint catalog.

The JSON schema (tools/port/schemas/playtest-checkpoints.v1.schema.json)
checks shape. ``semantic_errors`` checks what a schema cannot: unique keys,
every defect and reach reference resolves, seed states carry provenance, input
scripts exist, frame-timing entries cover the fixture's substeps exactly, a
known_diff residual never exceeds its exact count and its bbox lies inside the
native frame, and no entry embeds image data.

Usage: python3 tools/port/playtest_catalog.py [catalog.json]  (exit 1 on errors)
"""
import argparse
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "config/port/playtest-checkpoints.json"
SCHEMA = ROOT / "tools/port/schemas/playtest-checkpoints.v1.schema.json"
NATIVE_SIZE = (320, 240)
# Substrings that would mean original pixels or blobs were pasted into the catalog.
EMBEDDED_DATA = ("data:image", "base64,", "\"pixels\": [")


def load(path=CATALOG):
    return json.loads(Path(path).read_text())


def schema_errors(catalog):
    """Draft 2020-12 validation; jsonschema is required (tools/port/requirements-test.txt)."""
    from jsonschema import Draft202012Validator
    validator = Draft202012Validator(json.loads(SCHEMA.read_text()))
    return ["%s: %s" % ("/".join(str(p) for p in error.absolute_path), error.message)
            for error in sorted(validator.iter_errors(catalog), key=lambda e: list(e.absolute_path))]


def _inside(box):
    return box[0] <= box[2] < NATIVE_SIZE[0] and box[1] <= box[3] < NATIVE_SIZE[1]


def semantic_errors(catalog, root=ROOT):
    errors = []
    defects = [d.get("id") for d in catalog.get("defects", [])]
    if len(set(defects)) != len(defects):
        errors.append("duplicate defect id")
    defect_ids = set(defects)
    reach = catalog.get("reach", {})
    for name, recipe in reach.items():
        if recipe.get("kind") == "input_script" and not (root / recipe.get("script", "")).is_file():
            errors.append("reach %s: input script missing: %s" % (name, recipe.get("script")))
        if recipe.get("kind") == "seed_state" and recipe.get("label") != "path not verified":
            errors.append("reach %s: seed state must be labelled 'path not verified'" % name)
    seen = set()
    timing = {}
    for entry in catalog.get("checkpoints", []):
        key = entry.get("key")
        if key in seen:
            errors.append("duplicate key " + str(key))
        seen.add(key)
        for side in ("native", "port"):
            ref = entry.get("reach", {}).get(side)
            if ref not in reach:
                errors.append("%s: unknown %s reach %s" % (key, side, ref))
            elif reach[ref].get("side") != side:
                errors.append("%s: reach %s is not a %s recipe" % (key, ref, side))
        expected = entry.get("expected", {})
        status = expected.get("status")
        if status == "known_diff":
            for ref in [expected.get("defect_id")] + list(expected.get("related_defects", [])):
                if ref not in defect_ids:
                    errors.append("%s: unknown defect %s" % (key, ref))
            if expected.get("defect_id") in expected.get("related_defects", []):
                errors.append("%s: primary defect repeated in related_defects" % key)
            if expected.get("residual_pixels", 0) > expected.get("exact_rgba_pixels", 0):
                errors.append("%s: rgb555 residual exceeds the exact RGBA count" % key)
            for field in ("bbox", "exact_bbox"):
                box = expected.get(field)
                if not isinstance(box, list) or len(box) != 4 or not _inside(box):
                    errors.append("%s: %s outside the native frame" % (key, field))
            if entry.get("kind") != "frame":
                errors.append("%s: known_diff residuals are pixel bookkeeping; kind must be frame" % key)
        if entry.get("kind") == "frame_timing":
            ref = entry.get("timing_reference", {})
            if ref.get("substep") != entry.get("port_observer_key"):
                errors.append("%s: timing substep and port observer key differ" % key)
            predicate = entry.get("native_predicate", {})
            if ref.get("substep") != "%s/%s/%s" % (predicate.get("mode_stage"), predicate.get("mode_step"), predicate.get("substep")):
                errors.append("%s: timing substep does not match its native predicate" % key)
            for pad in ref.get("pads", []):
                timing.setdefault((ref.get("fixture"), pad), []).append(ref.get("substep"))
    for (fixture, pad), substeps in timing.items():
        path = root / str(fixture)
        try:
            rows = json.loads(path.read_text())["pads"][pad]["substeps"]
        except (OSError, KeyError, ValueError):
            errors.append("timing fixture %s has no pad %s" % (fixture, pad))
            continue
        if substeps != [row[0] for row in rows]:
            errors.append("timing entries for pad %s do not cover the fixture substeps in order" % pad)
    rendered = json.dumps(catalog)
    if any(marker in rendered for marker in EMBEDDED_DATA):
        errors.append("catalog embeds image or pixel data")
    return errors


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("catalog", nargs="?", type=Path, default=CATALOG)
    args = parser.parse_args(argv)
    catalog = load(args.catalog)
    errors = schema_errors(catalog) + semantic_errors(catalog)
    for error in errors:
        print(error)
    print("%d checkpoints, %d errors" % (len(catalog.get("checkpoints", [])), len(errors)))
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
