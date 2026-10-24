#!/usr/bin/env python3
"""Inventory original inputs and export the supported media subset (T-28002/3).

Run in Docker, for example:
  python3 tools/export_assets.py disc/track1.bin --inventory-only
  python3 tools/export_assets.py disc/files --include '*.TIM'
  python3 tools/export_assets.py disc/files --include '*.VH' --sample-rate 22050

Persistent output is restricted to ignored .asset-cache/. Exported PCM from VAB
is an audition at the explicitly selected rate, not rendered sequenced music.
Existing files with different bytes are refused. Use a new --output directory
for different options. Text parsing remains a separate tools/assets/text API.
"""
import argparse
import fnmatch
from pathlib import Path
import sys
import tempfile

from assets.audio import export_audio, inspect_audio
from assets.images import export_images, probe_images
from assets.image_packs import export_main_ui, probe_main_ui
from assets.image_packs_calendar import export_calendar_ui, probe_calendar_ui
from assets.image_packs_portrait import export_portrait, probe_portrait
from assets.input import open_input
from assets.manifest import (SCHEMA, asset_id, digest, output_root, summarize,
                             write_bytes, write_manifest)


DEFERRED = {
    ".BS": "MDEC background decoding is unsupported",
    ".NBN": "game texture/container decoding and frame mapping are unsupported",
    ".BIN": "game-specific container decoding is unsupported",
    ".TMD": "3D model decoding is unsupported",
    ".SEQ": "sequenced music rendering with instruments is unsupported",
    ".VB": "raw VAB body requires its matching header and explicit audition rate",
    ".XA": "XA audio stream demultiplexing/decoding is unsupported",
    ".STR": "STR video/audio demultiplexing/decoding is unsupported",
    ".DA": "CDDA reference/track mapping and PCM export are unsupported",
    ".EXN": "executable overlay; media decoding is unsupported",
}

PAIRED_UI_READERS = {
    "CDROM/FUKUTEST/MAINROM.NBN": ("tokimemo-main-ui", probe_main_ui, export_main_ui),
    "CDROM/FUKUTEST/CALROM.NBN": ("tokimemo-calendar-ui", probe_calendar_ui, export_calendar_ui),
}


def _classification(data, path):
    image = probe_images(data, path)
    if image["status"] != "unsupported":
        return "image", image
    audio = inspect_audio(data, path)
    if audio["status"] != "unsupported":
        return "audio", audio
    suffix = Path(path).suffix.upper()
    return None, {"status": "unsupported", "format": suffix.lstrip(".").lower() or "unknown",
                  "reason": DEFERRED.get(suffix, "no supported media reader for this file")}


def _publish_stage(stage, destination):
    files = []
    for path in sorted(stage.rglob("*")):
        if path.is_symlink():
            raise ValueError("decoder created a symlink")
        if path.is_file():
            files.append(write_bytes(destination / path.relative_to(stage), path.read_bytes()))
    return files


def export_source(source, destination, *, include=(), inventory_only=False,
                  sample_rate=None, max_bytes=16 * 1024 * 1024):
    """Build complete entry coverage; path filters restrict decode, never inventory."""
    destination = output_root(destination)
    original = Path(source.path).resolve()
    if original == destination or original in destination.parents or destination in original.parents:
        raise ValueError("input and output directories must not overlap")
    entries = sorted(source.entries(), key=lambda entry: entry["path"])
    lookup = {entry["path"]: entry for entry in entries}
    if len(lookup) != len(entries):
        raise ValueError("duplicate source paths are ambiguous")
    records = []
    destination.mkdir(parents=True, exist_ok=True)
    for entry in entries:
        content_hash = entry.get("sha256")
        identity_hash = content_hash or source.describe()["source_sha256"]
        record = {"id": asset_id(entry["path"], identity_hash), "source": entry,
                  "status": "unsupported", "format": "unknown"}
        records.append(record)
        if entry["kind"] != "file":
            record["reason"] = entry.get("unsupported", "non-file input entry")
            continue
        if entry["size"] > max_bytes:
            record.update(status="deferred", reason="exceeds --max-bytes decode limit")
            continue
        data = source.read(entry)
        if content_hash and digest(data) != content_hash:
            raise ValueError("source changed after inventory: %s" % entry["path"])
        record["source"]["sha256"] = digest(data)
        paired_image = None
        paired_image_entry = None
        ui_reader = PAIRED_UI_READERS.get(entry["path"])
        if (not ui_reader and entry["path"].startswith("CDROM/BUSTUP/")
                and Path(entry["path"]).suffix.upper() == ".BIN"):
            ui_reader = ("tokimemo-portrait", probe_portrait, export_portrait)
        if ui_reader:
            paired_image_entry = lookup.get("SLPM_86.053")
            if not paired_image_entry or paired_image_entry["kind"] != "file":
                record.update(status="deferred", format=ui_reader[0],
                              reason="UI pack decoding requires the paired main executable")
                continue
            if paired_image_entry["size"] > max_bytes:
                record.update(status="deferred", format=ui_reader[0],
                              reason="paired main executable exceeds --max-bytes")
                continue
            paired_image = source.read(paired_image_entry)
            if digest(paired_image) != paired_image_entry.get("sha256", digest(paired_image)):
                raise ValueError("paired main executable changed after inventory")
            record["paired_source"] = paired_image_entry
            lane, inspection = "paired-ui", ui_reader[1](data, paired_image)
        else:
            lane, inspection = _classification(data, entry["path"])
        record["inspection"] = inspection
        record["format"] = inspection.get("format") or "unknown"
        if inspection["status"] == "invalid":
            record.update(status="invalid", reason=inspection.get("reason", "invalid recognized format"))
            continue
        if not lane:
            record["reason"] = inspection.get("reason", "unsupported media")
            continue
        if inventory_only or (include and not any(fnmatch.fnmatchcase(entry["path"], p) for p in include)):
            record.update(status="supported", reason="decode disabled by inventory/path selection")
            continue
        body = None
        body_entry = None
        if lane == "audio" and Path(entry["path"]).suffix.upper() == ".VH":
            paired_path = str(Path(entry["path"]).with_suffix(".VB"))
            body_entry = lookup.get(paired_path)
            if not body_entry or body_entry["kind"] != "file":
                record.update(status="deferred", reason="VAB body pair is missing or unreadable")
                continue
            if body_entry["size"] > max_bytes:
                record.update(status="deferred", reason="VAB body exceeds --max-bytes decode limit")
                continue
            body = source.read(body_entry)
            if digest(body) != body_entry.get("sha256", digest(body)):
                raise ValueError("VAB body changed after inventory")
            record["paired_source"] = body_entry
        # VAB has no single native sample rate; require a caller's audition choice.
        if lane == "audio" and sample_rate is None and data[:4] == b"pBAV":
            record.update(status="deferred", reason="VAB waveform audition requires explicit --sample-rate")
            continue
        target = destination / "assets" / record["id"]
        with tempfile.TemporaryDirectory(prefix=".decode-", dir=destination) as temporary:
            stage = Path(temporary)
            if lane == "image":
                result = export_images(data, stage, record["id"])
            elif lane == "paired-ui":
                result = ui_reader[2](data, paired_image, stage, record["id"])
            else:
                result = export_audio(data, stage, record["id"], sample_rate=sample_rate, body=body)
            record["export"] = result
            record["status"] = result["status"]
            if result["status"] not in ("exported", "partial"):
                record["reason"] = result.get("reason", "reader did not export this format")
                continue
            record["outputs"] = _publish_stage(stage, target)
        record["original"] = write_bytes(target / "original.bin", data)
        if body is not None:
            record["paired_original"] = write_bytes(target / "original-body.bin", body)
        if paired_image is not None:
            record["paired_original"] = write_bytes(target / "original-executable.bin", paired_image)
        write_manifest(target / "metadata.json", record)
    manifest = {"schema": SCHEMA, "input": source.describe(),
                "options": {"include": sorted(include), "inventory_only": inventory_only,
                            "sample_rate": sample_rate, "max_bytes": max_bytes},
                "coverage": summarize(records), "assets": records,
                "limitations": ["directory inputs cannot inventory streams omitted before extraction",
                                "XA/STR/CDDA decoding is unsupported",
                                "MAINROM/CALROM UI and BUSTUP portrait profiles are supported; other MDEC/game texture/frame containers are unsupported",
                                "sequenced music rendering is unsupported",
                                "modern exports do not prove original-byte reconstruction"],
                "original_reconstruction_proven": False}
    write_manifest(destination / "manifest.json", manifest)
    return manifest


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", help="extracted directory, raw MODE2/2352 data track, or ZIP")
    parser.add_argument("--output", default=".asset-cache/media/default")
    parser.add_argument("--include", action="append", default=[], help="case-sensitive source-path glob to decode")
    parser.add_argument("--inventory-only", action="store_true")
    parser.add_argument("--sample-rate", type=int, help="explicit VAB sample audition rate; does not render music")
    parser.add_argument("--max-bytes", type=int, default=16 * 1024 * 1024)
    args = parser.parse_args(argv)
    if args.max_bytes <= 0 or (args.sample_rate is not None and not 1 <= args.sample_rate <= 192000):
        parser.error("max-bytes must be positive; sample-rate must be 1..192000")
    try:
        with open_input(args.input) as source:
            result = export_source(source, args.output, include=args.include,
                                   inventory_only=args.inventory_only,
                                   sample_rate=args.sample_rate, max_bytes=args.max_bytes)
        summary = result["coverage"]
        print("%d entries: %s" % (summary["entries"], summary["status"]))
        return 1 if (summary["status"].get("invalid") or summary["status"].get("partial")) else 0
    except (OSError, ValueError, EOFError) as error:
        print("export_assets.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
