"""Publish source-bound MDEC backgrounds without changing the shared exporter.

T-28002/T-28004. Run in Docker. Contexts come from media_background_context;
per-file hashes and LBAs must still match the original input. Missing or
conflicting geometry remains unsupported, with the coefficient census retained.
An optional earlier media manifest carries other exports into the same census.
"""
import argparse
from copy import deepcopy
import hashlib
import json
import math
from pathlib import Path
import re
import sys
import tempfile

if __package__ in (None, ""):
    # Direct execution needs the same package identity as module imports.
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
    __package__ = "tools.assets"
from .image_mdec import export_mdec, probe_mdec
from .input import open_input
from .manifest import SCHEMA, asset_id, digest, output_root, summarize, write_bytes, write_manifest
from .media_coverage import _compact_location
from .bs_cohort_profile import build_profile_context

def _cohort_context_for(entry, cohort_record, geometry_profile):
    source_record = cohort_record.get("source", {})
    directory = cohort_record.get("iso_directory_record", {})
    if (source_record.get("path") != entry.get("path")
            or source_record.get("sha256") != entry.get("sha256")
            or source_record.get("size") != entry.get("size")
            or source_record.get("location", {}).get("lba") != entry.get("location", {}).get("lba")
            or directory.get("record_lba") != entry.get("location", {}).get("record_lba")
            or directory.get("record_payload_offset") != entry.get("location", {}).get("record_offset")
            or directory.get("extent_lba") != entry.get("location", {}).get("lba")
            or directory.get("extent_size_bytes") != entry.get("size")
            or directory.get("sector_count") != math.ceil(entry.get("size", 0) / 2048)
            or directory.get("identifier") != entry.get("path", "").rsplit("/", 1)[-1] + ";1"):
        raise ValueError("BS cohort file hash, size, or ISO directory location differs: " + entry["path"])
    expected_overread = max(0, 10 - math.ceil(entry["size"] / 2048))
    native = cohort_record.get("fixed_reader_extent", {})
    if (native.get("requested_sectors") != 10
            or native.get("sectors_after_iso_file_extent") != expected_overread
            or native.get("post_file_sectors_claimed_consumed_by_decoder") is not False):
        raise ValueError("BS cohort native read extent is inconsistent: " + entry["path"])
    evidence = cohort_record.get("source_evidence", [])
    return {
        "width": 320,
        "height": 88,
        "evidence": evidence,
        "binding_scope": "exact original file hash/LBA/ISO record plus verified source witness and executable table",
        "trusted_profile_asset": cohort_record,
        "caller_geometry_profile": deepcopy(geometry_profile),
        "native_cd_read_extent": deepcopy(native),
        "decoded_source_bounds": "original ISO file bytes only; native overread sectors are recorded but excluded from coefficient input",
    }


def _native_cd_read_extent(source, entry, track_hash, requested_sectors):
    if not hasattr(source, "iter_raw_sectors"):
        raise ValueError("raw original sector reader is unavailable")
    lba = entry.get("location", {}).get("lba")
    if not isinstance(lba, int) or isinstance(lba, bool):
        raise ValueError("source entry has no native LBA")
    size = requested_sectors * 2048
    raw_entry = {"path": entry["path"], "kind": "file", "size": size,
                 "location": {"lba": lba, "extent_bytes": size}}
    raw_hash, payload_hash = hashlib.sha256(), hashlib.sha256()
    sectors = []
    for item in source.iter_raw_sectors(raw_entry, track_hash):
        if not item.get("subheader_valid") or item.get("form") != 1 or item.get("payload_size") != 2048:
            raise ValueError("native ten-sector read contains a non-Form1 or unverified sector")
        raw_hash.update(item["raw"])
        payload_hash.update(item["payload"])
        sectors.append({"lba": item["lba"], "form": item["form_name"],
                        "raw_sha256": item["raw_sha256"],
                        "payload_sha256": item["payload_sha256"],
                        "subheader_valid": item["subheader_valid"]})
    if len(sectors) != requested_sectors:
        raise ValueError("native read extent is incomplete")
    file_sectors = math.ceil(entry["size"] / 2048)
    return {"first_lba": lba, "requested_sector_count": requested_sectors,
            "last_lba_inclusive": lba + requested_sectors - 1,
            "declared_iso_size_bytes": entry["size"],
            "iso_extent_sector_count": file_sectors,
            "sectors_after_iso_extent": max(0, requested_sectors - file_sectors),
            "overread_lbas": list(range(lba + file_sectors, lba + requested_sectors)),
            "raw_sector_extent_sha256": raw_hash.hexdigest(),
            "payload_extent_sha256": payload_hash.hexdigest(),
            "sectors": sectors,
            "overread_payload_used_for_mdec": False}


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


def export_mdec_source(source, destination, contexts=None, baseline=None, progress=None,
                       only_paths=None, repository_root=None, trusted_bs_profile=False):
    """Return a media.v1 manifest; publish only backgrounds with unambiguous source proof."""
    cohort_mode = bool(trusted_bs_profile)
    cohort_records = None
    source_root = Path(repository_root) if repository_root else Path.cwd()
    if cohort_mode:
        entries = sorted(source.entries(), key=lambda entry: entry["path"])
        if contexts is not None:
            raise ValueError("trusted BS profile mode does not accept an external context/report")
        profile_kind = trusted_bs_profile if isinstance(trusted_bs_profile, str) else "weekly"
        if profile_kind == "holiday_command0":
            from .holiday_content_profile import build_holiday_profile
            contexts = build_holiday_profile(source, entries, source_root)
        else:
            contexts = build_profile_context(source, entries, source_root, profile_kind)
        cohort_records = {record["path"]: record for record in contexts["assets"]}
    elif not isinstance(contexts, dict):
        raise ValueError("contexts report must be an object")
    elif isinstance(contexts, dict) and contexts.get("schema") == "tokimemo.bs-caller-cohort.v1":
        raise ValueError("ignored BS cohort reports are not trusted; use the built-in finite profile")
    elif (contexts.get("schema") == "tokimemo.background-context.v1"
          and isinstance(contexts.get("streams"), list)):
        if any(not isinstance(stream, dict) or not isinstance(stream.get("source"), dict)
               for stream in contexts["streams"]):
            raise ValueError("context stream records must contain source objects")
    else:
        raise ValueError("expected tokimemo.background-context.v1 or the explicit trusted-profile mode")
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
    selected_paths = set(only_paths) if only_paths is not None else None
    if cohort_mode and selected_paths is None:
        selected_paths = set(cohort_records)
    if selected_paths is not None:
        if (not selected_paths or any(not isinstance(path, str) or not path for path in selected_paths)
                or baseline is None):
            raise ValueError("--only-source requires a nonempty exact baseline and source list")
        baseline_input = baseline.get("input", {})
        if baseline_input.get("source_sha256") != source.describe().get("source_sha256"):
            raise ValueError("selected-source baseline must match the original input SHA-256")
        if not selected_paths <= set(old):
            raise ValueError("selected source paths must exist in the exact baseline")
    if cohort_mode:
        expected_paths = set(cohort_records)
        if selected_paths != expected_paths:
            raise ValueError("BS profile mode requires exactly its finite source-bound paths")
        if not expected_paths <= set(old):
            raise ValueError("BS cohort sources are missing from the exact baseline")
    entry_paths = {entry["path"] for entry in entries}
    if selected_paths is not None and not selected_paths <= entry_paths:
        raise ValueError("selected source path is absent from the original input")
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
            if selected_paths is not None and entry["path"] in selected_paths:
                raise ValueError("selected source is not a readable BS file: " + entry["path"])
            continue
        if selected_paths is not None and entry["path"] not in selected_paths:
            continue
        # A baseline may contain a prior decode of this source. Clear its
        # published-output claims before any current-run unsupported path.
        record.pop("export", None)
        for key in ("outputs", "original", "paired_original"):
            record.pop(key, None)
        record.pop("source_bound_context", None)
        record.pop("native_cd_read_extent", None)
        if entry["size"] > 1024 * 1024:
            record.update(status="unsupported", format="sony-bs-mdec",
                          reason="BS source exceeds 1MiB decoder input budget")
            continue
        context = (_cohort_context_for(entry, cohort_records[entry["path"]], contexts["geometry"])
                   if cohort_mode else _context_for(entry, contexts))
        data = source.read(entry)
        if digest(data) != entry.get("sha256"):
            raise ValueError("source changed after inventory: " + entry["path"])
        native_extent = None
        if cohort_mode:
            try:
                native_extent = _native_cd_read_extent(
                    source, entry, source.describe()["source_sha256"], 10)
            except (ValueError, EOFError) as error:
                record.update(status="unsupported", format="sony-bs-mdec",
                              source_bound_context=context,
                              reason="native ten-sector read extent is not fully proven: " + str(error))
                continue
        inspection = probe_mdec(data, entry["path"], context)
        record.update(format="sony-bs-mdec", inspection=inspection,
                      source_bound_context=context)
        if native_extent is not None:
            record["native_cd_read_extent"] = native_extent
        if inspection["status"] != "supported":
            if cohort_mode:
                record.update(status="unsupported", validation_status=inspection["status"],
                              reason="bounded original BS file did not prove a complete 320x88 active frame: "
                              + inspection.get("reason", "decoder validation failed"))
            else:
                record.update(status=inspection["status"], reason=inspection.get("reason", "source geometry unresolved"))
        else:
            target = destination / "assets" / record["id"]
            with tempfile.TemporaryDirectory(prefix=".mdec-", dir=destination) as temporary:
                stage = Path(temporary)
                exported = export_mdec(data, stage, record["id"], source_path=entry["path"], context=context)
                if native_extent is not None:
                    exported["metadata"]["native_cd_read_extent"] = native_extent
                    exported["metadata"]["source_bound_context"] = context
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
    if selected_paths is not None:
        processed_paths = {record["source"]["path"] for record in records
                           if record.get("format") == "sony-bs-mdec"
                           and record["source"]["path"] in selected_paths}
        if processed_paths != selected_paths:
            raise ValueError("not every selected BS source was processed")
    result = {"schema": SCHEMA, "input": source.describe(), "assets": records,
              "coverage": summarize(records),
              "options": {"reader": "source-bound-bs-mdec", "baseline_exports_retained": baseline is not None,
                          "selected_source_paths": sorted(selected_paths) if selected_paths is not None else None},
              "context_provenance": {"schema": contexts["schema"], "source_hashes": contexts.get("source_hashes"),
                                     "canonical_report_sha256": digest(json.dumps(contexts, sort_keys=True, separators=(",", ":")).encode("utf-8")),
                                     "original_context_input": contexts.get("original_input")},
              "limitations": (["caller geometry is proved for these four exact source hashes and does not generalize to other BS assets",
                               "static associations do not prove every runtime control-flow path",
                               "native read-overrun sectors are provenance only and never extend the MDEC coefficient input",
                               "hardware IDCT/color pixel parity and original-byte rebuilding are unverified"]
                              if cohort_mode else
                              ["computed/table-driven source geometry remains unresolved",
                               "static associations do not prove all runtime control-flow paths",
                               "hardware IDCT/color pixel parity and original-byte rebuilding are unverified"]),
              "original_reconstruction_proven": False}
    write_manifest(destination / "manifest.json", result)
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input")
    parser.add_argument("--contexts")
    parser.add_argument("--bs-caller-cohort", action="store_true",
                        help="use the checked-in finite BS profile and verify current source plus original input")
    parser.add_argument("--holiday-command0", action="store_true", help="use the finite April9 holiday room/activity profile")
    parser.add_argument("--bs-command7", action="store_true", help="use the strict command-7 weekly background profile")
    parser.add_argument("--bs-seasonal-cohort", action="store_true",
                        help="use the checked-in four-row seasonal school-day profile")
    parser.add_argument("--baseline-media")
    parser.add_argument("--only-source", action="append",
                        help="refresh only this source path and retain exact-baseline exports for other sources")
    parser.add_argument("--repository-root", help="checkout root containing pinned C and assembly evidence")
    parser.add_argument("--output", required=True)
    args = parser.parse_args(argv)
    try:
        selected_profiles = int(args.bs_caller_cohort) + int(args.bs_seasonal_cohort) + int(args.bs_command7) + int(args.holiday_command0)
        if bool(args.contexts) == (selected_profiles > 0) or selected_profiles > 1:
            raise ValueError("provide exactly one context or built-in BS profile")
        contexts = json.loads(Path(args.contexts).read_text()) if args.contexts else None
        baseline = json.loads(Path(args.baseline_media).read_text()) if args.baseline_media else None
        with open_input(args.input) as source:
            result = export_mdec_source(source, args.output, contexts, baseline,
                                        progress=lambda count: print(f"BS inspected: {count}", flush=True),
                                        only_paths=args.only_source,
                                        repository_root=args.repository_root,
                                        trusted_bs_profile=("holiday_command0" if args.holiday_command0 else "command7" if args.bs_command7 else "seasonal" if args.bs_seasonal_cohort
                                                            else args.bs_caller_cohort))
        print(json.dumps(result["coverage"], sort_keys=True))
        return 1 if result["coverage"]["status"].get("invalid") else 0
    except (OSError, ValueError, EOFError) as error:
        print("media_mdec_export.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
