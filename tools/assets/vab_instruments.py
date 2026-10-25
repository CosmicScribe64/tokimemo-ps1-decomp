"""Read documented VAB program/tone attributes and bind tones to bank-local VAGs.

This module inventories one already-selected VH/VB pair. It does not select a
bank for a SEQ, render audio, or infer a VAG sample rate. Tone and program
fields retain their raw byte-level values alongside the documented names.
"""

from __future__ import annotations

import hashlib
import struct
from typing import Any

from .audio import VAB_MAGIC, inspect_audio

PROGRAM_TABLE_OFFSET = 0x20
PROGRAM_TABLE_ENTRIES = 128
PROGRAM_RECORD_SIZE = 0x10
TONE_TABLE_OFFSET = PROGRAM_TABLE_OFFSET + PROGRAM_TABLE_ENTRIES * PROGRAM_RECORD_SIZE
TONES_PER_PROGRAM = 16
TONE_RECORD_SIZE = 0x20
MAX_SOURCE_AUDIO_BYTES = 16 * 1024 * 1024


class VabInventoryError(ValueError):
    """Raised when a VAB/VH and paired VB cannot be inventoried safely."""


def _loop_flags(sample: bytes, sample_index: int) -> dict[str, Any]:
    if len(sample) % 16:
        raise VabInventoryError(f"VAG({sample_index}) extent is not 16-byte aligned")
    markers: list[dict[str, Any]] = []
    block_flags = [sample[offset + 1] for offset in range(0, len(sample), 16)]
    termination_block: int | None = None
    for block_index, flags in enumerate(block_flags):
        marker = {"block": block_index, "sample": block_index * 28, "raw_flags": flags}
        if flags & 0x04:
            markers.append({"kind": "loop_start", **marker})
        if flags & 0x01:
            termination_block = block_index
            markers.append({"kind": "end", "sample": (block_index + 1) * 28,
                            "repeat": bool(flags & 0x02), "block": block_index,
                            "raw_flags": flags})
            break
        if flags & 0x02:
            markers.append({"kind": "ignored_repeat_bit_without_end",
                            "sample": (block_index + 1) * 28,
                            "block": block_index, "raw_flags": flags})
    consumed = (termination_block + 1) * 16 if termination_block is not None else len(sample)
    trailing = sample[consumed:]
    return {
        "markers": markers,
        "source_block_flags": block_flags,
        "termination_block": termination_block,
        "trailing_source_bytes": len(trailing),
        "trailing_source_sha256": hashlib.sha256(trailing).hexdigest() if trailing else None,
        "status": "flags_scanned_only",
    }


def inventory_vab(header: bytes, body: bytes, *, header_path: str = "", body_path: str = "") -> dict[str, Any]:
    """Return a bank-local VAB inventory without producing PCM or WAV files."""
    if len(header) > MAX_SOURCE_AUDIO_BYTES or len(body) > MAX_SOURCE_AUDIO_BYTES:
        raise VabInventoryError("VAB/VB source exceeds the 16 MiB bounded input limit")
    if not header.startswith(VAB_MAGIC):
        raise VabInventoryError("header does not begin with Sony pBAV magic")
    probe = inspect_audio(header, header_path or "bank.VH")
    if probe.get("status") != "needs_pair":
        raise VabInventoryError("existing VAB header reader did not accept this supported header: " + "; ".join(probe.get("limitations", [])))
    meta = probe["metadata"]
    programs = int(meta["program_count"])
    sample_rows = meta["samples"]
    sample_bytes = int(meta["sample_data_size_bytes"])
    if len(body) < sample_bytes:
        raise VabInventoryError(f"paired VB is short: {len(body)} bytes, expected at least {sample_bytes}")
    declared_total_size = int(meta["declared_total_size"])
    paired_extent = len(header) + len(body)
    if declared_total_size != paired_extent:
        raise VabInventoryError(
            f"VAB declared file size {declared_total_size} bytes does not match paired VH/VB extent {paired_extent} bytes"
        )
    if len(header) < TONE_TABLE_OFFSET + programs * TONES_PER_PROGRAM * TONE_RECORD_SIZE:
        raise VabInventoryError("truncated program/tone attribute tables")
    if sum(header[PROGRAM_TABLE_OFFSET + i * PROGRAM_RECORD_SIZE] for i in range(programs)) != int(meta["tone_count_field"]):
        raise VabInventoryError("program tone counts do not sum to the VAB effective-tone count")

    samples: list[dict[str, Any]] = []
    for row in sample_rows:
        index = int(row["index"])
        offset = int(row["offset"])
        size = int(row["size_bytes"])
        samples.append({
            "index": index,
            "offset": offset,
            "size_bytes": size,
            "empty": size == 0,
            "loop_metadata": _loop_flags(body[offset:offset + size], index),
        })

    program_rows: list[dict[str, Any]] = []
    for program_index in range(programs):
        offset = PROGRAM_TABLE_OFFSET + program_index * PROGRAM_RECORD_SIZE
        raw = header[offset:offset + PROGRAM_RECORD_SIZE]
        tone_count = raw[0]
        if tone_count > TONES_PER_PROGRAM:
            raise VabInventoryError(f"program {program_index} declares {tone_count} tones; maximum is 16")
        program = {
            "index": program_index,
            "offset": offset,
            "raw_hex": raw.hex(),
            "tone_count": tone_count,
            "volume": raw[1],
            "priority": raw[2],
            "mode": raw[3],
            "pan": raw[4],
            "attribute_raw_u16": struct.unpack_from("<H", raw, 6)[0],
            "tones": [],
        }
        for tone_index in range(tone_count):
            tone_offset = TONE_TABLE_OFFSET + (program_index * TONES_PER_PROGRAM + tone_index) * TONE_RECORD_SIZE
            tone = header[tone_offset:tone_offset + TONE_RECORD_SIZE]
            owner_field, vag_index = struct.unpack_from("<HH", tone, 20)
            if owner_field != program_index:
                raise VabInventoryError(f"program {program_index} tone {tone_index} owner field is {owner_field}")
            if vag_index >= len(samples) or vag_index == 0:
                raise VabInventoryError(f"program {program_index} tone {tone_index} references invalid VAG index {vag_index}")
            fields = tone[:14]
            sample = samples[vag_index]
            program["tones"].append({
                "index": tone_index,
                "offset": tone_offset,
                "raw_hex": tone.hex(),
                "priority": fields[0], "mode": fields[1], "volume": fields[2], "pan": fields[3],
                "center_note": fields[4], "pitch_shift": fields[5],
                "minimum_note": fields[6], "maximum_note": fields[7],
                "vibrato_width": fields[8], "vibrato_time": fields[9],
                "portamento_width": fields[10], "portamento_time": fields[11],
                "pitch_bend_minimum": fields[12], "pitch_bend_maximum": fields[13],
                "adsr1_raw_u16": struct.unpack_from("<H", tone, 16)[0],
                "adsr2_raw_u16": struct.unpack_from("<H", tone, 18)[0],
                "owner_program_field": owner_field,
                "vag_index": vag_index,
                "sample_offset": sample["offset"],
                "sample_size_bytes": sample["size_bytes"],
                "sample_loop_metadata": sample["loop_metadata"],
                "native_sample_rate_hz": None,
            })
        program_rows.append(program)

    return {
        "schema": "tokimemo.vab_instrument_inventory.v1",
        "header_source": header_path,
        "body_source": body_path,
        "header_sha256": hashlib.sha256(header).hexdigest(),
        "body_sha256": hashlib.sha256(body).hexdigest(),
        "header_size_bytes": len(header),
        "body_size_bytes": len(body),
        "header_declared_file_size_bytes": declared_total_size,
        "paired_extent_bytes": paired_extent,
        "body_declared_sample_extent_bytes": sample_bytes,
        "body_trailing_bytes": len(body) - sample_bytes,
        "body_trailing_sha256": hashlib.sha256(body[sample_bytes:]).hexdigest() if len(body) > sample_bytes else None,
        "bank": {
            "version": meta["version"], "id": meta["vab_id"],
            "program_count": programs, "effective_tone_count": meta["tone_count_field"],
            "declared_vag_count_field": meta["declared_vag_count_field"],
            "sample_size_table_offset": meta["sample_sizes_table_offset"],
        },
        "programs": program_rows,
        "samples": samples,
        "limitations": [
            "bank-local attributes only; no SEQ-to-bank selection or runtime playback assignment",
            "VAB header contains no native sample rate; pitch is represented by raw tone attributes",
            "loop flags are inventoried without ADPCM decode, PCM rendering, or loop playback",
            "tone field names follow Sony Psy-Q Run-Time Library Overview 4.4 VAB structures",
        ],
    }
