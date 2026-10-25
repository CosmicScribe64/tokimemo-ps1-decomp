"""Export original sequence packages to safe, deterministic JSON and optional MIDI."""
import argparse
import json
from pathlib import Path

from tools.assets import manifest
from tools.assets.sequence_music import export_smf, inspect_seq, parse_events, reconstruct_seq


def _write_json(path, value):
    payload = (json.dumps(value, ensure_ascii=True, sort_keys=True, indent=2) + "\n").encode("utf-8")
    return manifest.write_bytes(path, payload)


def export_directory(input_dir, output_dir, midi_preview_assuming_u24_is_mpqn=False):
    """Export bounded SEQ packages beneath the repository's ignored cache."""
    root = manifest.output_root(output_dir)
    input_root = Path(input_dir).resolve()
    repository = Path(manifest.REPOSITORY).resolve()
    try:
        input_root.relative_to(repository)
    except ValueError as exc:
        raise ValueError("input directory must be inside the repository") from exc
    files = sorted(input_root.glob("*.SEQ"), key=lambda p: p.as_posix())
    if not files:
        raise ValueError("input directory contains no .SEQ files")
    (root / "packages").mkdir(parents=True, exist_ok=True)
    if midi_preview_assuming_u24_is_mpqn:
        (root / "midi").mkdir(parents=True, exist_ok=True)

    manifest_files = []
    for source_path in files:
        source_rel = source_path.resolve().relative_to(repository).as_posix()
        raw = source_path.read_bytes()
        source_hash = manifest.digest(raw)
        package_id = manifest.asset_id(source_rel, source_hash)
        package = inspect_seq(raw, source_rel)
        package["asset_id"] = package_id
        package["source_relative_path"] = source_rel
        reconstructed = reconstruct_seq(package, raw)
        if reconstructed != raw:
            raise ValueError("structured package roundtrip mismatch: " + source_rel)

        entries = []
        sequence_rows = []
        for seq in package["sequences"]:
            offset = seq["offset"]
            extent = raw[offset:seq["bound_end"]]
            extent_hash = manifest.digest(extent)
            entry_id = manifest.asset_id("%s#seq@%08x" % (source_rel, offset), source_hash)
            slots = [slot["slot"] for slot in package["sequence_slots"] if slot["resolved_offset"] == offset]
            parsed_events = None
            unsupported_reason = None
            try:
                parsed_events = parse_events(raw, seq)
                if parsed_events["status"] != "complete":
                    raise ValueError("event parser did not reach an explicit end marker")
            except ValueError as exc:
                unsupported_reason = str(exc)

            row = {
                "asset_id": entry_id,
                "source_relative_path": source_rel,
                "source_sha256": source_hash,
                "source_size": len(raw),
                "source_extent_sha256": extent_hash,
                "physical_offset": offset,
                "runtime_slots": slots,
                "event_status": "complete" if parsed_events is not None else "unsupported",
                "midi_preview_status": "not_requested",
                "event_count": len(parsed_events["events"]) if parsed_events is not None else None,
            }
            record = {"identity": row, "native_entry": seq}
            if parsed_events is not None:
                record["event_parse"] = parsed_events
                if midi_preview_assuming_u24_is_mpqn:
                    try:
                        midi = export_smf(seq, parsed_events)
                    except ValueError as exc:
                        if "tempo zero" not in str(exc) and "timing fields are outside SMF range" not in str(exc):
                            raise
                        row["midi_preview_status"] = "not_exported_invalid_timing"
                        midi = None
                    if midi is None:
                        record["event_parse"] = parsed_events
                        entries.append(record)
                        sequence_rows.append(row)
                        continue
                    midi_rel = "midi/%s.mid" % entry_id
                    midi_record = manifest.write_bytes(root / midi_rel, midi)
                    row["midi_preview_status"] = "native_tempo_audition_no_playback_parity"
                    row["midi_tempo_contract"] = "native header and FF51 u24 use microseconds per quarter note; original scheduler math verified"
                    row["midi_output"] = midi_record
            else:
                record["unsupported_reason"] = unsupported_reason
                if midi_preview_assuming_u24_is_mpqn:
                    row["midi_preview_status"] = "not_exported_unsupported_events"
            entries.append(record)
            sequence_rows.append(row)

        package_record = {
            "schema": "tokimemo.sequence-music.v4",
            "midi_policy": "optional MIDI audition; canonical raw/JSON always; native tempo conversion is proven, playback parity is not claimed",
            "native_midi_semantics": "PSX-specific CC99 loop markers and NRPN controls are preserved as MIDI CC; their behavior is not General MIDI playback behavior",
            "package": package,
            "entries": entries,
        }
        json_rel = "packages/%s.json" % package_id
        json_record = _write_json(root / json_rel, package_record)
        manifest_files.append({
            "asset_id": package_id,
            "format": "sony_seq_package",
            "status": "exported_json",
            "source": {"path": source_rel, "size": len(raw), "sha256": source_hash},
            "output": json_record,
            "sequences": sequence_rows,
        })

    coverage = {
        "files": len(manifest_files),
        "physical_sequences": sum(len(row["sequences"]) for row in manifest_files),
        "runtime_slots": sum(len(sequence["runtime_slots"]) for row in manifest_files for sequence in row["sequences"]),
        "midi_previews_requested": bool(midi_preview_assuming_u24_is_mpqn),
        "midi_previews_exported": sum(sequence["midi_preview_status"] == "native_tempo_audition_no_playback_parity" for row in manifest_files for sequence in row["sequences"]),
        "unsupported_sequence_entries": sum(sequence["event_status"] == "unsupported" for row in manifest_files for sequence in row["sequences"]),
        "decoded_source_events": sum(sequence["event_count"] or 0 for row in manifest_files for sequence in row["sequences"]),
        "statuses": {"exported_json": len(manifest_files)},
    }
    manifest_record = {
        "schema": "tokimemo.sequence-music-manifest.v4",
        "coverage": coverage,
        "original_reconstruction_proven": True,
        "midi_timing_claim": "native header/FF51 tempo unit and scheduler conversion are source-proven; MIDI remains an alternate audition, with no hardware playback parity claim",
        "midi_native_semantics": "PSX-specific loop and NRPN control messages are preserved; no General MIDI equivalence is claimed",
        "files": manifest_files,
    }
    manifest_path = manifest.write_manifest(root / "manifest.json", manifest_record)
    return {"coverage": coverage, "manifest": manifest_path, "files": manifest_files}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_dir", help="original CDROM/VAB_SEQ directory")
    parser.add_argument("output_dir", help="output beneath repository .asset-cache/")
    parser.add_argument(
        "--midi-audition", "--midi-preview-assuming-u24-is-mpqn",
        action="store_true",
        dest="midi_preview_assuming_u24_is_mpqn",
        help="opt in to alternate MIDI audition using the source-proven u24 microseconds-per-quarter-note tempo",
    )
    args = parser.parse_args(argv)
    result = export_directory(args.input_dir, args.output_dir, args.midi_preview_assuming_u24_is_mpqn)
    print("files=%d sequences=%d midi=%d unsupported=%d" % (
        result["coverage"]["files"], result["coverage"]["physical_sequences"],
        result["coverage"]["midi_previews_exported"], result["coverage"]["unsupported_sequence_entries"]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
