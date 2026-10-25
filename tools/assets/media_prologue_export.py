"""Merge a source/native-proved bounded prologue NBN page export into media.v1."""
import argparse
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import sys
import tempfile

if __package__ in (None, ""):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from assets.image_packs_prologue import (
        EXE_PATH, FILES, OVERLAY_PATH, PAGE_COUNT, TRACK_SHA256, build_profile_context,
        export_prologue_pages, probe_prologue_pages,
    )
    from assets.input import open_input
    from assets.manifest import SCHEMA, asset_id, digest, output_root, summarize, write_bytes, write_manifest
    from assets.media_coverage import _compact_location
else:
    from .image_packs_prologue import (
        EXE_PATH, FILES, OVERLAY_PATH, PAGE_COUNT, TRACK_SHA256, build_profile_context,
        export_prologue_pages, probe_prologue_pages,
    )
    from .input import open_input
    from .manifest import SCHEMA, asset_id, digest, output_root, summarize, write_bytes, write_manifest
    from .media_coverage import _compact_location


def export_prologue_source(source, destination, baseline, repository_root, only_paths):
    """Merge exactly the four fixed NBNs after validating identities and cache paths."""
    expected_paths = {row["path"] for row in FILES}
    if set(only_paths or ()) != expected_paths:
        raise ValueError("prologue export requires exactly its four fixed CUT source paths")
    if not isinstance(baseline, dict) or baseline.get("schema") != SCHEMA:
        raise ValueError("baseline must be a media.v1 manifest")
    entries = sorted(source.entries(), key=lambda entry: entry["path"])
    description = source.describe()
    if description.get("source_sha256") != TRACK_SHA256:
        raise ValueError("original prologue track hash differs")
    if (baseline.get("input", {}).get("source_sha256") != TRACK_SHA256
            or baseline.get("input", {}).get("source_sha256") != description.get("source_sha256")):
        raise ValueError("baseline must describe the exact original MODE2 track")
    profile = build_profile_context(source, entries, repository_root)
    profile_by_path = {row["path"]: row for row in profile["source_assets"]}
    indexed = {entry["path"]: entry for entry in entries}
    baseline_records = baseline.get("assets")
    if not isinstance(baseline_records, list):
        raise ValueError("baseline assets must be a list")
    old = {record["source"]["path"]: record for record in baseline_records
           if isinstance(record, dict) and isinstance(record.get("source"), dict)
           and isinstance(record["source"].get("path"), str)}
    if len(old) != len(baseline_records):
        raise ValueError("baseline contains malformed or duplicate source paths")

    # Validate every selected source hash and derive its only accepted public ID
    # before creating the output root or invoking the decoder.
    selected = {}
    for path in sorted(expected_paths):
        entry = indexed.get(path)
        if entry is None:
            raise ValueError("fixed prologue source is absent: " + path)
        previous = old.get(path)
        if previous and previous.get("source", {}).get("sha256") != entry.get("sha256"):
            raise ValueError("baseline source bytes differ from the original input: " + path)
        stable_id = asset_id(path, entry.get("sha256") or "")
        if previous and previous.get("id") != stable_id:
            raise ValueError("selected baseline asset ID differs from source-derived stable ID: " + path)
        selected[path] = {"entry": entry, "previous": previous, "id": stable_id,
                          "row": profile_by_path[path]}

    destination = output_root(destination)
    original = Path(source.path).resolve()
    if original == destination or original in destination.parents or destination in original.parents:
        raise ValueError("input and output paths must not overlap")

    # Preflight every final cache path, including ancestors, before the first PNG
    # is staged or published. output_root resolves existing symlinks and rejects
    # any route outside .asset-cache.
    expected_final_paths = [destination / "manifest.json"]
    for path in sorted(expected_paths):
        item = selected[path]
        target = destination / "assets" / item["id"]
        expected_final_paths.extend((target, target / "metadata.json", target / "original.bin",
                                     target / "dependency-0.bin", target / "dependency-1.bin"))
        for index in range(PAGE_COUNT):
            expected_final_paths.extend((target / (item["id"] + ".cell-%d.png" % index),
                                         target / (item["id"] + ".cell-%d.clut-bit15.png" % index)))
    checked_paths = [output_root(path) for path in expected_final_paths]
    if len(checked_paths) != len(expected_final_paths):
        raise ValueError("cache output path preflight is incomplete")

    destination.mkdir(parents=True, exist_ok=True)
    output_root(destination)
    entries_by_path = {entry["path"]: entry for entry in entries}
    records = []
    dependencies = {path: entries_by_path[path] for path in (OVERLAY_PATH, EXE_PATH)}
    dependency_data = {path: source.read(entry) for path, entry in dependencies.items()}
    for entry in entries:
        path = entry["path"]
        item = selected.get(path)
        previous = item["previous"] if item else old.get(path)
        record = deepcopy(previous) if previous else {
            "id": asset_id(path, entry.get("sha256") or description["source_sha256"]),
            "status": "unsupported", "format": "unknown",
            "reason": entry.get("unsupported", "not decoded in this cohort")}
        record["source"] = deepcopy(entry)
        if "location" in record["source"]:
            record["source"]["location"] = _compact_location(record["source"]["location"])
        records.append(record)
        if item is None:
            continue
        # Use the derived ID even after checking any baseline-provided ID.
        record["id"] = item["id"]
        for key in ("export", "outputs", "original", "paired_original", "source_dependencies",
                    "dependency_originals", "inspection"):
            record.pop(key, None)
        row = item["row"]
        data = source.read(entry)
        if digest(data) != entry.get("sha256"):
            raise ValueError("prologue NBN changed after input inventory: " + path)
        inspection = probe_prologue_pages(data, source_row=row)
        record.update(status="unsupported", format="tokimemo-prologue-native-cells", inspection=inspection)
        if inspection.get("status") != "supported":
            record.update(status=inspection.get("status", "invalid"), reason=inspection.get("reason"))
            continue
        target = output_root(destination / "assets" / item["id"])
        with tempfile.TemporaryDirectory(prefix=".prologue-", dir=destination) as tempdir:
            stage = Path(tempdir)
            output_root(stage)
            for index in range(PAGE_COUNT):
                output_root(stage / (item["id"] + ".cell-%d.png" % index))
                output_root(stage / (item["id"] + ".cell-%d.clut-bit15.png" % index))
            exported = export_prologue_pages(data, stage, item["id"], source_row=row)
            outputs = []
            for output in sorted(stage.iterdir()):
                if output.is_symlink() or not output.is_file():
                    raise ValueError("prologue reader produced an unexpected output entry")
                final_path = target / output.name
                output_root(final_path)
                outputs.append(write_bytes(final_path, output.read_bytes()))
        record.update(status=exported["status"], format=exported["format"], export=exported,
                      outputs=outputs, original=write_bytes(target / "original.bin", data),
                      source_dependencies=[deepcopy(dependencies[OVERLAY_PATH]),
                                           deepcopy(dependencies[EXE_PATH])],
                      dependency_originals=[])
        for index, dependency_path in enumerate((OVERLAY_PATH, EXE_PATH)):
            saved = write_bytes(target / ("dependency-%d.bin" % index), dependency_data[dependency_path])
            record["dependency_originals"].append({
                "source": deepcopy(dependencies[dependency_path]), "original": saved})
            record["outputs"].append(saved)
        record.pop("reason", None)
        write_manifest(target / "metadata.json", record)
    result = deepcopy(baseline)
    result.update(schema=SCHEMA, input=description, assets=records,
                  coverage=summarize(records))
    result["options"] = {**(result.get("options") or {}),
                          "additional_reader": "source/native-backed prologue NBN UV-mapped cells",
                          "prologue_selected_sources": sorted(expected_paths),
                          "prologue_profile_schema": profile["schema"]}
    limitations = list(result.get("limitations", []))
    for limitation in (
        "Prologue previews use the embedded source color CLUT; the runtime grayscale alternative is not emitted.",
        "Separate raw source CLUT bit-15 diagnostics do not establish runtime STP behavior.",
        "Individual 128x120 cells follow the source upload/cell UV mapping; final scene composition, transitions, and timing are not reproduced.",
        "Hardware raster/color parity is not verified.",
    ):
        if limitation not in limitations:
            limitations.append(limitation)
    result["limitations"] = limitations
    write_manifest(destination / "manifest.json", result)
    return result

def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input")
    parser.add_argument("--baseline-media", required=True)
    parser.add_argument("--repository-root", required=True)
    parser.add_argument("--only-source", action="append", required=True)
    parser.add_argument("--output", required=True)
    args = parser.parse_args(argv)
    try:
        baseline = json.loads(Path(args.baseline_media).read_text())
        with open_input(args.input) as source:
            result = export_prologue_source(source, args.output, baseline,
                                           args.repository_root, args.only_source)
        print(json.dumps(result["coverage"], sort_keys=True))
        return 1 if result["coverage"]["status"].get("invalid") else 0
    except (OSError, ValueError, EOFError) as error:
        print("media_prologue_export.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
