"""Publish source-bound MDEC backgrounds without changing the shared exporter.

T-28002/T-28004. Run in Docker. Contexts come from media_background_context;
per-file hashes and LBAs must still match the original input. Missing or
conflicting geometry remains unsupported, with the coefficient census retained.
An optional earlier media manifest carries other exports into the same census.
"""
import argparse
from copy import deepcopy
import json
from pathlib import Path
import re
import sys
import tempfile

if __package__ in (None, ""):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.image_mdec import export_mdec, probe_mdec
from assets.input import open_input
from assets.manifest import SCHEMA, asset_id, digest, output_root, summarize, write_bytes, write_manifest
from assets.media_coverage import _compact_location


def _context_for(entry, report):
    streams = [stream for stream in report["streams"] if stream.get("source", {}).get("path") == entry["path"]]
    for stream in streams:
        source = stream["source"]
        if source.get("sha256") != entry.get("sha256"):
            raise ValueError("context source hash differs: " + entry["path"])
        wanted_lba = source.get("location", {}).get("lba")
        actual_lba = entry.get("location", {}).get("lba")
        if wanted_lba is not None and actual_lba is not None and wanted_lba != actual_lba:
            raise ValueError("context source LBA differs: " + entry["path"])
    exact = [stream for stream in streams if stream.get("source_offset") == 0
             and stream.get("context_status") in ("proved_single_shape", "proved_single_shape_with_unresolved_uses")
             and len(stream.get("shape_candidates", [])) == 1]
    shapes = {(stream["shape_candidates"][0]["width"], stream["shape_candidates"][0]["height"]) for stream in exact}
    if len(shapes) != 1:
        return None
    width, height = next(iter(shapes))
    proof = []
    for stream in exact:
        for call in stream.get("contexts", []):
            if (call.get("width"), call.get("height")) != (width, height):
                continue
            proof.append(f'{call["file"]}:{call["line"]}:{call.get("function")}:{call["symbol"]}')
            for ref in call.get("decoder_evidence", []):
                proof.append(f'{ref["file"]}:{ref["line"]}:{ref["function"]}:{width}x{height}')
    if not proof:
        return None
    return {"width": width, "height": height, "evidence": sorted(set(proof)),
            "source_context_records": exact,
            "binding_scope": "source-file hash plus static read/decoder association; unresolved other uses retained"}


def export_mdec_source(source, destination, contexts, baseline=None, progress=None):
    """Return a media.v1 manifest; publish only backgrounds with unambiguous source proof."""
    if not isinstance(contexts, dict) or contexts.get("schema") != "tokimemo.background-context.v1" or not isinstance(contexts.get("streams"), list):
        raise ValueError("expected tokimemo.background-context.v1 report")
    if any(not isinstance(stream, dict) or not isinstance(stream.get("source"), dict)
           for stream in contexts["streams"]):
        raise ValueError("context stream records must contain source objects")
    if baseline is not None and (not isinstance(baseline, dict) or baseline.get("schema") != SCHEMA):
        raise ValueError("baseline must be a media.v1 manifest")
    destination = output_root(destination)
    original = Path(source.path).resolve()
    if original == destination or original in destination.parents or destination in original.parents:
        raise ValueError("input and output paths must not overlap")
    baseline_assets = (baseline or {}).get("assets", [])
    if not isinstance(baseline_assets, list):
        raise ValueError("baseline assets must be a list")
    old = {}
    for record in baseline_assets:
        if (not isinstance(record, dict) or not isinstance(record.get("source"), dict)
                or not isinstance(record["source"].get("path"), str)
                or not isinstance(record.get("id"), str)
                or re.fullmatch(r"asset-[0-9a-f]{64}", record["id"]) is None):
            raise ValueError("baseline asset records require a source path and safe asset id")
        old[record["source"]["path"]] = record
    if len(old) != len(baseline_assets):
        raise ValueError("baseline source paths must be unique")
    entries = sorted(source.entries(), key=lambda entry: entry["path"])
    if len({entry["path"] for entry in entries}) != len(entries):
        raise ValueError("input source paths must be unique")
    destination.mkdir(parents=True, exist_ok=True)
    records, processed = [], 0
    for entry in entries:
        previous = old.get(entry["path"])
        if previous and entry.get("sha256") != previous["source"].get("sha256"):
            raise ValueError("baseline source hash differs: " + entry["path"])
        record = deepcopy(previous) if previous else {
            "id": asset_id(entry["path"], entry.get("sha256") or source.describe()["source_sha256"]),
            "status": "unsupported", "format": "unknown", "reason": entry.get("unsupported", "not decoded in this run")}
        record["source"] = deepcopy(entry)
        if "location" in record["source"]:
            record["source"]["location"] = _compact_location(record["source"]["location"])
        records.append(record)
        if entry["kind"] != "file" or not entry["path"].upper().endswith(".BS"):
            continue
        # A baseline may contain a prior decode of this source. Clear its
        # published-output claims before any current-run unsupported path.
        record.pop("export", None)
        for key in ("outputs", "original", "paired_original"):
            record.pop(key, None)
        if entry["size"] > 1024 * 1024:
            record.update(status="unsupported", format="sony-bs-mdec",
                          reason="BS source exceeds 1MiB decoder input budget")
            continue
        context = _context_for(entry, contexts)
        data = source.read(entry)
        if digest(data) != entry.get("sha256"):
            raise ValueError("source changed after inventory: " + entry["path"])
        inspection = probe_mdec(data, entry["path"], context)
        record.update(format="sony-bs-mdec", inspection=inspection)
        if inspection["status"] != "supported":
            record.update(status=inspection["status"], reason=inspection.get("reason", "source geometry unresolved"))
        else:
            target = destination / "assets" / record["id"]
            with tempfile.TemporaryDirectory(prefix=".mdec-", dir=destination) as temporary:
                stage = Path(temporary)
                exported = export_mdec(data, stage, record["id"], source_path=entry["path"], context=context)
                outputs = []
                for path in sorted(stage.iterdir()):
                    if path.is_symlink() or not path.is_file():
                        raise ValueError("MDEC decoder produced an unexpected output entry")
                    outputs.append(write_bytes(target / path.name, path.read_bytes()))
            record.update(status="exported", export=exported, outputs=outputs,
                          original=write_bytes(target / "original.bin", data))
            record.pop("reason", None)
            write_manifest(target / "metadata.json", record)
        processed += 1
        if progress and processed % 16 == 0:
            progress(processed)
    result = {"schema": SCHEMA, "input": source.describe(), "assets": records,
              "coverage": summarize(records),
              "options": {"reader": "source-bound-bs-mdec", "baseline_exports_retained": baseline is not None},
              "context_provenance": {"schema": contexts["schema"], "source_hashes": contexts.get("source_hashes"),
                                     "canonical_report_sha256": digest(json.dumps(contexts, sort_keys=True, separators=(",", ":")).encode("utf-8")),
                                     "original_context_input": contexts.get("original_input")},
              "limitations": ["computed/table-driven source geometry remains unresolved",
                              "static associations do not prove all runtime control-flow paths",
                              "hardware IDCT/color pixel parity and original-byte rebuilding are unverified"],
              "original_reconstruction_proven": False}
    write_manifest(destination / "manifest.json", result)
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input")
    parser.add_argument("--contexts", required=True)
    parser.add_argument("--baseline-media")
    parser.add_argument("--output", required=True)
    args = parser.parse_args(argv)
    try:
        contexts = json.loads(Path(args.contexts).read_text())
        baseline = json.loads(Path(args.baseline_media).read_text()) if args.baseline_media else None
        with open_input(args.input) as source:
            result = export_mdec_source(source, args.output, contexts, baseline,
                                        progress=lambda count: print(f"BS inspected: {count}", flush=True))
        print(json.dumps(result["coverage"], sort_keys=True))
        return 1 if result["coverage"]["status"].get("invalid") else 0
    except (OSError, ValueError, EOFError) as error:
        print("media_mdec_export.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
