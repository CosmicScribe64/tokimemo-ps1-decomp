"""Validate stable SEQ manifest identities, reconstruction, safe outputs, and SMF."""
import json
from pathlib import Path

from tools.assets import manifest
from tools.assets.sequence_music import export_smf, inspect_seq, parse_events, reconstruct_seq, validate_event_records


def _parse_smf(data):
    """Independent format-0 reader for the deliberately limited preview SMF."""
    if data[:4] != b"MThd" or int.from_bytes(data[4:8], "big") != 6:
        raise ValueError("invalid MIDI header")
    if (int.from_bytes(data[8:10], "big"), int.from_bytes(data[10:12], "big")) != (0, 1):
        raise ValueError("expected one-track SMF type 0")
    division = int.from_bytes(data[12:14], "big")
    if data[14:18] != b"MTrk":
        raise ValueError("missing MIDI track")
    end = 22 + int.from_bytes(data[18:22], "big")
    if end != len(data):
        raise ValueError("MIDI track extent mismatch")
    pos, tick, events, eot = 22, 0, [], None
    while pos < end:
        delta = 0
        for _ in range(4):
            if pos >= end:
                raise ValueError("truncated MIDI delta VLQ")
            byte = data[pos]
            pos += 1
            delta = (delta << 7) | (byte & 0x7F)
            if byte < 0x80:
                break
        else:
            raise ValueError("MIDI delta VLQ exceeds four bytes")
        tick += delta
        if pos >= end:
            raise ValueError("MIDI event is truncated")
        status = data[pos]
        pos += 1
        if status == 0xFF:
            if pos + 2 > end:
                raise ValueError("MIDI meta event truncated")
            kind, size = data[pos], data[pos + 1]
            pos += 2
            if pos + size > end:
                raise ValueError("MIDI meta data out of bounds")
            payload = data[pos : pos + size]
            pos += size
            if kind == 0x2F:
                if size or pos != end:
                    raise ValueError("bad or non-final MIDI EOT")
                eot = tick
                break
            events.append((tick, "meta", kind, payload))
        else:
            kind = status & 0xF0
            width = 1 if kind == 0xC0 else 2
            if kind not in (0x90, 0xB0, 0xC0, 0xE0) or pos + width > end:
                raise ValueError("unsupported or truncated MIDI channel event")
            values = tuple(data[pos : pos + width])
            if any(value >= 0x80 for value in values):
                raise ValueError("MIDI channel data has status bit set")
            events.append((tick, status, values))
            pos += width
    if eot is None:
        raise ValueError("missing MIDI EOT")
    return division, events, eot


def validate_directory(input_dir, output_dir):
    root = manifest.output_root(output_dir)
    repository = Path(manifest.REPOSITORY).resolve()
    source_root = Path(input_dir).resolve()
    try:
        source_root.relative_to(repository)
    except ValueError as exc:
        raise ValueError("input directory must be inside repository") from exc
    source_paths = sorted(source_root.glob("*.SEQ"), key=lambda p: p.as_posix())
    exported = json.loads((root / "manifest.json").read_text(encoding="utf-8"))
    if exported.get("original_reconstruction_proven") is not True:
        raise ValueError("manifest does not assert structured reconstruction")
    if exported["coverage"]["files"] != len(source_paths):
        raise ValueError("source file count differs from manifest")
    manifest_file_rows = exported["files"]
    manifest_paths = [row.get("source", {}).get("path") for row in manifest_file_rows]
    expected_paths = [p.resolve().relative_to(repository).as_posix() for p in source_paths]
    if len(manifest_file_rows) != len(source_paths) or len(set(manifest_paths)) != len(manifest_paths):
        raise ValueError("manifest file rows are duplicated or differ from source count")
    if set(manifest_paths) != set(expected_paths):
        raise ValueError("manifest file path set differs from source inputs")
    files = {row["source"]["path"]: row for row in manifest_file_rows}
    seen_ids = set()
    midi_count = 0
    unsupported_count = 0
    slots = 0
    physical_count = 0
    decoded_event_count = 0
    for source_path in source_paths:
        relative = source_path.resolve().relative_to(repository).as_posix()
        raw = source_path.read_bytes()
        source_hash = manifest.digest(raw)
        package_id = manifest.asset_id(relative, source_hash)
        file_row = files.get(relative)
        if file_row is None or file_row["asset_id"] != package_id:
            raise ValueError("source-relative package identity mismatch")
        if file_row["source"] != {"path": relative, "size": len(raw), "sha256": source_hash}:
            raise ValueError("source path/hash/size differs from manifest")
        pkg_path = manifest.output_root(repository / file_row["output"]["path"])
        pkg_raw = pkg_path.read_bytes()
        if manifest.digest(pkg_raw) != file_row["output"]["sha256"]:
            raise ValueError("JSON package output hash mismatch")
        package_record = json.loads(pkg_raw)
        package = package_record["package"]
        if package.get("asset_id") != package_id or package.get("source_relative_path") != relative:
            raise ValueError("serialized package identity/provenance mismatch")
        if reconstruct_seq(package, raw) != raw:
            raise ValueError("structured source reconstruction mismatch")
        parsed_package = inspect_seq(raw, relative)
        expected_package = dict(parsed_package)
        expected_package["asset_id"] = package_id
        expected_package["source_relative_path"] = relative
        if package != expected_package:
            raise ValueError("serialized native package fields differ from fresh source parse")
        for key in ("offset_table_raw_hex", "offset_table_le_u32", "sequence_slots", "reverb_depth_raw_bytes", "prefix_gap_hex", "sequences"):
            if package[key] != parsed_package[key]:
                raise ValueError("package parsed field differs from source: " + key)

        if len(package_record["entries"]) != len(package["sequences"]) or len(file_row["sequences"]) != len(package["sequences"]):
            raise ValueError("physical entry count mismatch")
        for seq, entry, seq_row in zip(package["sequences"], package_record["entries"], file_row["sequences"]):
            physical_count += 1
            offset = seq["offset"]
            extent_hash = manifest.digest(raw[offset : seq["bound_end"]])
            expected_id = manifest.asset_id("%s#seq@%08x" % (relative, offset), source_hash)
            aliases = [s["slot"] for s in package["sequence_slots"] if s["resolved_offset"] == offset]
            ident = entry["identity"]
            if entry.get("native_entry") != seq:
                raise ValueError("serialized native entry fields differ from fresh source parse")
            if seq_row != ident:
                raise ValueError("manifest sequence identity differs from package entry identity")
            if ident["asset_id"] != expected_id or expected_id in seen_ids:
                raise ValueError("physical entry stable identity mismatch or collision")
            seen_ids.add(expected_id)
            if ident["source_relative_path"] != relative or ident["source_sha256"] != source_hash or ident["physical_offset"] != offset:
                raise ValueError("physical entry source provenance mismatch")
            if ident["source_extent_sha256"] != extent_hash or ident["runtime_slots"] != aliases or seq_row["runtime_slots"] != aliases:
                raise ValueError("physical extent or alias slots differ from source")
            expected_identity = {
                "asset_id": expected_id,
                "source_relative_path": relative,
                "source_sha256": source_hash,
                "source_size": len(raw),
                "source_extent_sha256": extent_hash,
                "physical_offset": offset,
                "runtime_slots": aliases,
            }
            if any(ident.get(key) != value for key, value in expected_identity.items()):
                raise ValueError("manifest physical identity/provenance differs from source")
            slots += len(aliases)
            # Source parsing alone determines whether this event stream is
            # unsupported.  Stored metadata validation happens after this
            # narrow exception boundary and can never be reclassified as such.
            try:
                parsed = parse_events(raw, seq)
            except ValueError as exc:
                if seq_row["event_status"] != "unsupported" or entry.get("unsupported_reason") != str(exc):
                    raise ValueError("source event parse failure differs from unsupported metadata") from exc
                if "event_parse" in entry or ident.get("event_count") is not None or "midi_output" in ident:
                    raise ValueError("unsupported raw entry contains fabricated parsed data or MIDI")
                expected_status = "not_exported_unsupported_events" if exported["coverage"]["midi_previews_requested"] else "not_requested"
                if ident.get("midi_preview_status") != expected_status:
                    raise ValueError("unsupported MIDI preview status label mismatch")
                unsupported_count += 1
                continue

            if entry.get("unsupported_reason") is not None or "event_parse" not in entry:
                raise ValueError("supported source entry has unsupported or missing event metadata")
            if entry["event_parse"] != parsed:
                raise ValueError("stored event parse differs from source parse")
            validate_event_records(raw, seq, entry["event_parse"])
            if seq_row["event_status"] != "complete":
                raise ValueError("complete event stream marked unsupported")
            if ident["event_count"] != len(parsed["events"]):
                raise ValueError("source event count differs from manifest row")
            decoded_event_count += len(parsed["events"])
            midi_output = ident.get("midi_output")
            preview_requested = exported["coverage"]["midi_previews_requested"]
            if not preview_requested and midi_output is not None:
                raise ValueError("MIDI exists in canonical JSON-only export")
            if not preview_requested:
                expected_preview_status = "not_requested"
            elif midi_output is not None:
                expected_preview_status = "native_tempo_audition_no_playback_parity"
            elif not (1 <= seq["header"]["quarter_note_resolution"] <= 0x7FFF
                      and 1 <= seq["header"]["tempo_u24_be"] <= 0xFFFFFF) or any(
                    event.get("kind") == "meta_tempo" and event["tempo_u24"] == 0
                    for event in parsed["events"]):
                expected_preview_status = "not_exported_invalid_timing"
            else:
                expected_preview_status = "not_exported_unsupported_events"
            if ident["midi_preview_status"] != expected_preview_status:
                raise ValueError("MIDI preview status label mismatch")
            if preview_requested and midi_output is None and expected_preview_status != "not_exported_invalid_timing":
                raise ValueError("opt-in MIDI preview missing for exportable supported entry")
            if midi_output is not None:
                midi_path = manifest.output_root(repository / midi_output["path"])
                midi = midi_path.read_bytes()
                if manifest.digest(midi) != midi_output["sha256"]:
                    raise ValueError("MIDI output hash mismatch")
                if midi != export_smf(seq, parsed):
                    raise ValueError("MIDI repeat serialization mismatch")
                division, midi_events, end_tick = _parse_smf(midi)
                original_events = []
                for event in parsed["events"]:
                    if event.get("kind") == "meta_tempo":
                        original_events.append((event["tick"], "meta", 0x51, bytes.fromhex(event["payload_hex"])))
                    else:
                        original_events.append((event["tick"], event["status"], tuple(event["data"])))
                ordered = [(0, -2, (0, "meta", 0x51, seq["header"]["tempo_u24_be"].to_bytes(3, "big")))]
                for index, event in enumerate(parsed["events"]):
                    ordered.append((event["tick"], index, original_events[index]))
                expected_events = [row[2] for row in sorted(ordered, key=lambda row: (row[0], row[1]))]
                tempos = [e for e in midi_events if len(e) > 1 and e[1] == "meta" and e[2] == 0x51]
                expected_tempo = seq["header"]["tempo_u24_be"].to_bytes(3, "big")
                if midi_events != expected_events or division != seq["header"]["quarter_note_resolution"] or end_tick != parsed["end_tick"]:
                    raise ValueError("MIDI semantics, event order, or ticks differ")
                expected_tempos = [(0, "meta", 0x51, expected_tempo)] + [
                    (event["tick"], "meta", 0x51, bytes.fromhex(event["payload_hex"]))
                    for event in parsed["events"] if event.get("kind") == "meta_tempo"
                ]
                if tempos != expected_tempos:
                    raise ValueError("MIDI header/source tempo events differ from native source")
                midi_count += 1
    if slots != exported["coverage"]["runtime_slots"]:
        raise ValueError("runtime alias slot count mismatch")
    if physical_count != exported["coverage"]["physical_sequences"]:
        raise ValueError("physical sequence count differs from manifest coverage")
    if decoded_event_count != exported["coverage"]["decoded_source_events"]:
        raise ValueError("decoded event count differs from source-derived coverage")
    if midi_count != exported["coverage"]["midi_previews_exported"]:
        raise ValueError("MIDI preview count mismatch")
    if unsupported_count != exported["coverage"]["unsupported_sequence_entries"]:
        raise ValueError("unsupported entry count mismatch")
    print("files=%d sequences=%d slots=%d midi=%d unsupported=%d structured_roundtrip_failures=0 midi_semantic_mismatches=0" % (
        len(source_paths), len(seen_ids), slots, midi_count, unsupported_count))


def main(argv=None):
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_dir")
    parser.add_argument("output_dir")
    args = parser.parse_args(argv)
    validate_directory(args.input_dir, args.output_dir)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
