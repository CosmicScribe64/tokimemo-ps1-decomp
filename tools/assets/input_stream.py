"""Cache-confined preservation of original MODE2 XA/STR raw sectors."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import tempfile

from .input import InputError
from .manifest import CACHE, REPOSITORY, asset_id, output_root, write_manifest


def stage_raw_stream(source, baseline_manifest, entry, output_dir, allow_opaque_subheader=False):
    """Preserve a bounded ISO-described sector run and write its census.

    The baseline binds the open original by its whole-input SHA256. ``entry``
    is the original inventory source record, with path, size, and location.
    The ISO size determines only the number of 2048-byte address units; the
    output file contains concatenated 2352-byte raw sectors.
    """
    if not isinstance(baseline_manifest, dict):
        raise InputError("baseline inventory must be an object")
    baseline_input = baseline_manifest.get("input")
    if not isinstance(baseline_input, dict):
        raise InputError("baseline inventory has no input provenance")
    source_info = source.describe()
    expected = baseline_input.get("source_sha256")
    if not isinstance(expected, str) or source_info.get("source_sha256") != expected:
        raise InputError("baseline input SHA256 does not match the open original")
    if baseline_input.get("source") not in (None, source_info.get("source")):
        # Archive input paths can be relocated, but only when whole-input hash
        # equality already binds the same bytes; the path remains recorded.
        if baseline_input.get("source_sha256") != source_info.get("source_sha256"):
            raise InputError("baseline input path and hash do not match source")
    if not isinstance(entry, dict) or entry.get("kind") not in ("unsupported", "file"):
        raise InputError("entry is not an original inventory source record")
    if entry.get("kind") == "unsupported" and "Form2" not in entry.get("unsupported", ""):
        raise InputError("entry is not the same identified Form2 stream as the baseline")
    if source_info.get("format") not in ("mode2-2352", "zip-mode2-2352"):
        raise InputError("raw stream staging requires a MODE2/2352 source")
    assets = baseline_manifest.get("assets")
    if not isinstance(assets, list):
        raise InputError("baseline inventory has no source records")
    matching = []
    for asset in assets:
        original = asset.get("source") if isinstance(asset, dict) else None
        if not isinstance(original, dict) or original.get("path") != entry.get("path"):
            continue
        original_location = original.get("location")
        entry_location = entry.get("location")
        if (original.get("size") == entry.get("size")
                and isinstance(original_location, dict) and isinstance(entry_location, dict)
                and original_location.get("lba") == entry_location.get("lba")
                and original.get("kind") == entry.get("kind")):
            matching.append((asset, original))
    if len(matching) != 1:
        raise InputError("original entry path, size, and LBA are not uniquely bound by baseline")
    baseline_asset, canonical_entry = matching[0]
    if (canonical_entry.get("kind") != "unsupported"
            or "Form2" not in canonical_entry.get("unsupported", "")
            or entry.get("unsupported") != canonical_entry.get("unsupported")):
        raise InputError("baseline source does not identify this exact Form2 stream")

    requested = Path(output_dir)
    if not requested.is_absolute():
        requested = REPOSITORY / requested
    checked_dir = output_root(requested)
    input_cache = CACHE / "input"
    if checked_dir != input_cache and input_cache not in checked_dir.parents:
        raise InputError("raw stream output must be beneath .asset-cache/input/")
    checked_dir.mkdir(parents=True, exist_ok=True)
    if checked_dir.is_symlink():
        raise InputError("raw stream output directory may not be a symlink")
    original_path = Path(source_info["source"]).resolve()
    if (checked_dir == original_path or checked_dir in original_path.parents
            or original_path in checked_dir.parents):
        raise InputError("raw stream output overlaps the original input path")

    # Use the canonical baseline record after matching it. Caller-supplied
    # metadata cannot change the chosen extent or its Form2 classification.
    path = canonical_entry.get("path")
    size = canonical_entry.get("size")
    location = canonical_entry.get("location")
    if not isinstance(path, str) or not isinstance(location, dict):
        raise InputError("entry is missing original path or extent location")
    lba = location.get("lba")
    key = asset_id(path, expected)
    raw_path = checked_dir / (key + ".raw2352")
    manifest_path = checked_dir / (key + ".json")
    if raw_path.is_symlink() or manifest_path.is_symlink():
        raise InputError("raw stream outputs may not be symlinks")
    sectors = []
    raw_hash = hashlib.sha256()
    payload_hash = hashlib.sha256()
    count = size // 2048 if isinstance(size, int) and not isinstance(size, bool) and size > 0 else 0
    temp_name = None
    try:
        with tempfile.NamedTemporaryFile(mode="wb", dir=str(checked_dir), prefix=".stream-", delete=False) as out:
            temp_name = out.name
            invalid_subheaders = 0
            for sector in source.iter_raw_sectors(canonical_entry, expected,
                                                 allow_invalid_subheaders=allow_opaque_subheader):
                out.write(sector["raw"])
                raw_hash.update(sector["raw"])
                if sector["payload"] is not None:
                    payload_hash.update(sector["payload"])
                if not sector["subheader_valid"]:
                    invalid_subheaders += 1
                sectors.append({k: sector[k] for k in (
                    "sector_index", "lba", "raw_size", "payload_size", "raw_sha256",
                    "payload_sha256", "form", "form_name", "file_number", "channel",
                    "submode", "coding", "subheader_valid", "subheader_primary",
                    "subheader_secondary", "metadata_status", "edc_validation")})
            out.flush()
            os.fsync(out.fileno())
        if len(sectors) != count:
            raise InputError("raw stream sector count changed during staging")
        if invalid_subheaders and not allow_opaque_subheader:
            raise InputError("raw stream contains an invalid duplicated XA subheader")
        if invalid_subheaders > 1:
            raise InputError("opaque preservation is bounded to one invalid XA subheader")
        try:
            os.link(temp_name, raw_path)
        except FileExistsError:
            if not raw_path.is_file() or _hash_file(raw_path) != raw_hash.hexdigest():
                raise InputError("raw stream output exists with different bytes")
        payload_complete = invalid_subheaders == 0
        manifest = {
            "schema": "tokimemo.raw-sector-stream.v1",
            "status": "staged" if payload_complete else "partial",
            "source": {
                "path": path,
                "input_sha256": expected,
                "format": source_info["format"],
                "container_source": source_info.get("source"),
                "track_member": source_info.get("track_member"),
                "baseline_asset_id": baseline_asset.get("id"),
                "lba": lba,
                "declared_iso_size_bytes": size,
                "sector_count": count,
                "size_semantics": "2048-byte ISO address units; not Form2 payload byte length",
                "input_mutation_assumption": "the original source remains unchanged while open and during staging",
                "invalid_subheader_sector_count": invalid_subheaders,
            },
            "raw_stream": {
                "path": raw_path.relative_to(REPOSITORY).as_posix(),
                "size": count * 2352,
                "sha256": raw_hash.hexdigest(),
                "concatenation": "original 2352-byte sectors in ascending LBA order",
            },
            "payload_stream": {
                "size": sum(sector["payload_size"] for sector in sectors if sector["payload_size"] is not None),
                "sha256": payload_hash.hexdigest() if payload_complete else None,
                "complete": payload_complete,
                "invalid_sector_count": invalid_subheaders,
                "concatenation": "each sector's Mode2 payload in ascending LBA order",
                "completeness_note": ("all sectors have validated payload geometry" if payload_complete
                                      else "incomplete: one or more subheader mismatches leave payload geometry unresolved"),
            },
            "edc_validation": "not_performed",
            "sectors": sectors,
        }
        write_manifest(manifest_path, manifest)
        return manifest
    finally:
        if temp_name and os.path.exists(temp_name):
            os.unlink(temp_name)


def _hash_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def _validate_cached_stream(manifest_path, canonical_entry, baseline_asset, input_sha256):
    """Return a cached stream only after checking its source binding and bytes."""
    try:
        with open(manifest_path, "r", encoding="utf-8") as stream:
            manifest = json.load(stream)
    except (OSError, ValueError):
        return None
    source = manifest.get("source") if isinstance(manifest, dict) else None
    if (not isinstance(manifest, dict)
            or manifest.get("schema") != "tokimemo.raw-sector-stream.v1"
            or not isinstance(source, dict)
            or source.get("path") != canonical_entry.get("path")
            or source.get("input_sha256") != input_sha256
            or source.get("baseline_asset_id") != baseline_asset.get("id")
            or source.get("lba") != canonical_entry.get("location", {}).get("lba")
            or source.get("declared_iso_size_bytes") != canonical_entry.get("size")):
        return None
    raw_record = manifest.get("raw_stream")
    payload_record = manifest.get("payload_stream")
    if not isinstance(raw_record, dict) or not isinstance(payload_record, dict):
        return None
    try:
        raw_candidate = REPOSITORY / raw_record["path"]
        if raw_candidate.is_symlink():
            return None
        raw_path = output_root(raw_candidate)
    except (KeyError, ValueError):
        return None
    cache_input = CACHE / "input"
    if raw_path == cache_input or cache_input not in raw_path.parents or raw_path.is_symlink():
        return None
    count = canonical_entry["size"] // 2048
    expected_lba = canonical_entry["location"]["lba"]
    sector_records = manifest.get("sectors")
    if (source.get("sector_count") != count
            or raw_record.get("concatenation") != "original 2352-byte sectors in ascending LBA order"
            or payload_record.get("concatenation") != "each sector's Mode2 payload in ascending LBA order"
            or raw_record.get("size") != count * 2352 or not isinstance(sector_records, list)
            or len(sector_records) != count
            or not raw_path.is_file() or raw_path.stat().st_size != count * 2352):
        return None
    raw_hash = hashlib.sha256()
    payload_hash = hashlib.sha256()
    payload_size = 0
    try:
        with raw_path.open("rb") as stream:
            for index, record in enumerate(sector_records):
                raw = stream.read(2352)
                if len(raw) != 2352 or raw[:12] != b"\x00" + b"\xff" * 10 + b"\x00" or raw[15] != 2 or raw[16:20] != raw[20:24]:
                    return None
                sub = raw[16:20]
                form2 = bool(sub[2] & 0x20)
                payload = raw[24:2348] if form2 else raw[24:2072]
                actual = {
                    "sector_index": index, "lba": expected_lba + index,
                    "raw_size": 2352, "payload_size": len(payload),
                    "raw_sha256": hashlib.sha256(raw).hexdigest(),
                    "payload_sha256": hashlib.sha256(payload).hexdigest(),
                    "form": 2 if form2 else 1,
                    "form_name": "Form2" if form2 else "Form1",
                    "file_number": sub[0], "channel": sub[1], "submode": sub[2],
                    "coding": sub[3], "subheader_valid": True,
                    "subheader_primary": sub.hex(), "subheader_secondary": raw[20:24].hex(),
                    "metadata_status": "validated", "edc_validation": "not_performed",
                }
                required = ("sector_index", "lba", "raw_size", "payload_size", "raw_sha256",
                            "payload_sha256", "form", "file_number", "channel", "submode",
                            "coding", "edc_validation")
                if (not isinstance(record, dict)
                        or any(key not in record for key in required)
                        or record.get("subheader_valid", True) is not True
                        or any(record.get(key, value) != value for key, value in actual.items())):
                    return None
                record.update({key: value for key, value in actual.items() if key not in record})
                raw_hash.update(raw)
                payload_hash.update(payload)
                payload_size += len(payload)
            if stream.read(1):
                return None
    except OSError:
        return None
    if (raw_hash.hexdigest() != raw_record.get("sha256")
            or payload_hash.hexdigest() != payload_record.get("sha256")
            or payload_size != payload_record.get("size")
            or payload_record.get("complete", True) is not True
            or source.get("invalid_subheader_sector_count", 0) != 0
            or manifest.get("edc_validation") != "not_performed"):
        return None
    payload_record.setdefault("complete", True)
    payload_record.setdefault("invalid_sector_count", 0)
    payload_record.setdefault("completeness_note", "all sectors have validated payload geometry")
    return manifest


def stage_form2_inventory(source, baseline_manifest, output_dir,
                          expected_count=16, reuse_dirs=None, aggregate_path=None):
    """Stage every Form2 inventory record and write a deterministic census."""
    baseline_input = baseline_manifest.get("input") if isinstance(baseline_manifest, dict) else None
    source_info = source.describe()
    if (not isinstance(baseline_input, dict)
            or baseline_input.get("source_sha256") != source_info.get("source_sha256")):
        raise InputError("baseline input SHA256 does not match the open original")
    assets = baseline_manifest.get("assets")
    if not isinstance(assets, list):
        raise InputError("baseline inventory has no source records")
    candidates = []
    for asset in assets:
        original = asset.get("source") if isinstance(asset, dict) else None
        if (isinstance(original, dict) and original.get("kind") == "unsupported"
                and "Form2" in original.get("unsupported", "")):
            location = original.get("location")
            if (not isinstance(original.get("path"), str) or not original["path"]
                    or not isinstance(original.get("size"), int) or isinstance(original.get("size"), bool)
                    or original["size"] <= 0 or original["size"] % 2048
                    or not isinstance(location, dict)
                    or not isinstance(location.get("lba"), int) or isinstance(location.get("lba"), bool)
                    or location["lba"] < 0):
                raise InputError("baseline Form2 stream has incomplete source bounds")
            candidates.append((asset, original))
    candidates.sort(key=lambda item: (item[1].get("path", ""), item[1].get("location", {}).get("lba", -1)))
    if expected_count is not None and len(candidates) != expected_count:
        raise InputError("baseline contains %d Form2 streams; expected %d" % (len(candidates), expected_count))

    requested = Path(output_dir)
    if not requested.is_absolute():
        requested = REPOSITORY / requested
    checked_dir = output_root(requested)
    if checked_dir == CACHE / "input" or CACHE / "input" not in checked_dir.parents:
        raise InputError("batch output must be beneath .asset-cache/input/")
    original_path = Path(source_info["source"]).resolve()
    if (checked_dir == original_path or checked_dir in original_path.parents
            or original_path in checked_dir.parents):
        raise InputError("batch output overlaps the original input path")
    checked_dir.mkdir(parents=True, exist_ok=True)
    previous_dirs = [Path(p) for p in (reuse_dirs if reuse_dirs is not None
                                       else [CACHE / "input" / "form2-validation"])]
    aggregate = []
    for asset, entry in candidates:
        stream_manifest = None
        stream_manifest_path = None
        for previous in previous_dirs:
            if not previous.is_absolute():
                previous = REPOSITORY / previous
            try:
                previous_dir = output_root(previous)
            except ValueError:
                continue
            cache_input = CACHE / "input"
            if previous_dir == cache_input or cache_input not in previous_dir.parents:
                continue
            candidate_manifest = previous_dir / (asset_id(entry["path"], baseline_input["source_sha256"]) + ".json")
            if candidate_manifest.is_symlink() or not candidate_manifest.is_file():
                continue
            stream_manifest = _validate_cached_stream(candidate_manifest, entry, asset,
                                                      baseline_input["source_sha256"])
            if stream_manifest is not None:
                stream_manifest_path = candidate_manifest
                break
        if stream_manifest is None:
            try:
                stream_manifest = stage_raw_stream(source, baseline_manifest, entry, checked_dir)
            except InputError as exc:
                if "invalid duplicated XA subheader" not in str(exc):
                    raise
                stream_manifest = stage_raw_stream(source, baseline_manifest, entry, checked_dir,
                                                   allow_opaque_subheader=True)
            stream_manifest_path = checked_dir / (asset_id(entry["path"], baseline_input["source_sha256"]) + ".json")
        aggregate.append({
            "asset_id": asset.get("id"),
            "source_path": entry["path"],
            "lba": entry["location"]["lba"],
            "declared_iso_size_bytes": entry["size"],
            "sector_count": stream_manifest["source"]["sector_count"],
            "raw_stream": stream_manifest["raw_stream"],
            "payload_stream": stream_manifest["payload_stream"],
            "stream_manifest_path": Path(stream_manifest_path).resolve().relative_to(REPOSITORY).as_posix(),
            "status": stream_manifest["status"],
            "invalid_subheader_sector_count": stream_manifest["source"].get("invalid_subheader_sector_count", 0),
            "edc_validation": stream_manifest["edc_validation"],
            "sectors": stream_manifest["sectors"],
        })

    sector_rows = [sector for stream in aggregate for sector in stream["sectors"]]
    def counts(field, transform=lambda value: value):
        result = {}
        for sector in sector_rows:
            if sector.get(field) is None:
                continue
            value = transform(sector[field])
            key = str(value)
            result[key] = result.get(key, 0) + 1
        return dict(sorted(result.items(), key=lambda item: item[0]))
    file_channel_counts = {}
    file_channel_coding_counts = {}
    for row in sector_rows:
        if row.get("file_number") is None or row.get("channel") is None:
            continue
        key = "%d:%d" % (row["file_number"], row["channel"])
        file_channel_counts[key] = file_channel_counts.get(key, 0) + 1
        if row.get("coding") is not None:
            coding_key = "%d:%d:%d" % (row["file_number"], row["channel"], row["coding"])
            file_channel_coding_counts[coding_key] = file_channel_coding_counts.get(coding_key, 0) + 1
    form_counts = counts("form")
    submode_flags = {
        "end_of_record": sum(bool(row["submode"] & 0x01) for row in sector_rows if row.get("submode") is not None),
        "video": sum(bool(row["submode"] & 0x02) for row in sector_rows if row.get("submode") is not None),
        "audio": sum(bool(row["submode"] & 0x04) for row in sector_rows if row.get("submode") is not None),
        "data": sum(bool(row["submode"] & 0x08) for row in sector_rows if row.get("submode") is not None),
        "trigger": sum(bool(row["submode"] & 0x10) for row in sector_rows if row.get("submode") is not None),
        "form2": sum(bool(row["submode"] & 0x20) for row in sector_rows if row.get("submode") is not None),
        "real_time": sum(bool(row["submode"] & 0x40) for row in sector_rows if row.get("submode") is not None),
        "end_of_file": sum(bool(row["submode"] & 0x80) for row in sector_rows if row.get("submode") is not None),
    }
    invalid_sector_records = [
        {"source_path": stream["source_path"], "lba": sector["lba"],
         "raw_sha256": sector["raw_sha256"],
         "subheader_primary": sector["subheader_primary"],
         "subheader_secondary": sector["subheader_secondary"]}
        for stream in aggregate for sector in stream["sectors"]
        if sector.get("subheader_valid") is False
    ]
    result = {
        "schema": "tokimemo.raw-sector-inventory.v1",
        "status": "partial" if any(stream["status"] != "staged" for stream in aggregate) else "staged",
        "input": {"source": source_info.get("source"), "source_sha256": baseline_input["source_sha256"],
                  "format": source_info.get("format"), "track_member": source_info.get("track_member")},
        "scope": "all baseline inventory records identified as Form2 XA/STR streams",
        "stream_count": len(aggregate),
        "sector_count": len(sector_rows),
        "invalid_subheader_sector_count": sum(stream["invalid_subheader_sector_count"] for stream in aggregate),
        "interpreted_metadata_sector_count": sum(row.get("subheader_valid", True) is True for row in sector_rows),
        "raw_bytes": sum(stream["raw_stream"]["size"] for stream in aggregate),
        "payload_bytes": sum(stream["payload_stream"]["size"] for stream in aggregate),
        "raw_sha256_by_stream": {stream["source_path"]: stream["raw_stream"]["sha256"] for stream in aggregate},
        "payload_sha256_by_stream": {stream["source_path"]: stream["payload_stream"]["sha256"] for stream in aggregate},
        "distributions": {
            "form": form_counts,
            "file_number": counts("file_number"),
            "channel": counts("channel"),
            "coding": counts("coding"),
            "submode": counts("submode"),
            "file_channel": dict(sorted(file_channel_counts.items())),
            "file_channel_coding": dict(sorted(file_channel_coding_counts.items())),
            "audio_sectors": submode_flags["audio"],
            "video_sectors": submode_flags["video"],
            "submode_flags": submode_flags,
        },
        "distribution_semantics": {
            "file_number": "raw XA subheader byte values",
            "channel": "raw XA subheader byte values",
            "coding": "raw XA subheader byte values; no codec interpretation",
            "submode": "raw XA subheader byte values and individual bit-set counts",
            "audio_video": "counts sectors with XA audio/video submode bits set; not a decoded-content classification",
            "file_channel_coding": "stable raw tuple counts in file:channel:coding notation; invalid duplicate-subheader sectors are excluded",
        },
        "payload_byte_semantics": "payload_bytes sums only sectors whose duplicated subheaders establish payload geometry; partial stream payloads have no aggregate-complete hash",
        "hash_semantics": {
            "raw_sha256": "SHA256 of concatenated original 2352-byte sectors, reported separately per source extent",
            "payload_sha256": "SHA256 of concatenated Mode2 payloads, reported separately per source extent",
            "iso_logical_file_sha256": "not_computed; ISO byte size is used only to determine 2048-byte sector count",
        },
        "edc_validation": "not_performed",
        "metadata_completeness": "partial; invalid duplicate subheader sectors are preserved opaquely" if any(
            stream["invalid_subheader_sector_count"] for stream in aggregate) else "all staged sectors have matching duplicated subheaders",
        "invalid_sector_records": invalid_sector_records,
        "streams": aggregate,
    }
    if aggregate_path is None:
        aggregate_path = checked_dir / "aggregate.json"
    aggregate_requested = Path(aggregate_path)
    if not aggregate_requested.is_absolute():
        aggregate_requested = REPOSITORY / aggregate_requested
    if aggregate_requested.is_symlink():
        raise InputError("aggregate manifest output may not be a symlink")
    checked_aggregate = output_root(aggregate_requested)
    cache_input = CACHE / "input"
    if checked_aggregate == cache_input or cache_input not in checked_aggregate.parents:
        raise InputError("aggregate manifest must be beneath .asset-cache/input/")
    write_manifest(checked_aggregate, result)
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description="Stage original Form2 MODE2 sectors into the ignored input cache")
    parser.add_argument("--input", required=True, help="original MODE2/2352 image or ZIP")
    parser.add_argument("--baseline-inventory", required=True, help="cached original inventory JSON")
    parser.add_argument("--output-dir", default=".asset-cache/input/form2-batch-v1")
    parser.add_argument("--aggregate", default=None, help="aggregate manifest output (default: OUTPUT_DIR/aggregate.json)")
    parser.add_argument("--reuse-dir", action="append", default=None,
                        help="prior verified .asset-cache/input directory to reuse; may be repeated")
    args = parser.parse_args(argv)
    with open(args.baseline_inventory, "r", encoding="utf-8") as stream:
        baseline = json.load(stream)
    from .input import open_input
    with open_input(args.input) as source:
        result = stage_form2_inventory(source, baseline, args.output_dir,
                                       reuse_dirs=args.reuse_dir, aggregate_path=args.aggregate)
    print(json.dumps({"stream_count": result["stream_count"], "sector_count": result["sector_count"],
                      "raw_bytes": result["raw_bytes"], "payload_bytes": result["payload_bytes"],
                      "aggregate": args.aggregate or str(Path(args.output_dir) / "aggregate.json")}, sort_keys=True))


if __name__ == "__main__":
    main()
