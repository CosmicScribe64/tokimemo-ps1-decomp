"""Conservative readers for Sony VAGp samples and VAB/VH+VB banks.

The reader exports decodable ADPCM samples as audition WAVs; it does not render
SEQ music, recreate instrument playback, or decode XA/STR/CDDA streams.
"""

from __future__ import annotations

import json
import hashlib
import re
import struct
import wave
from pathlib import Path
from typing import Any

VAG_HEADER_SIZE = 0x30
VAG_MAGIC = b"VAGp"
VAB_MAGIC = b"pBAV"
MAX_SOURCE_AUDIO_BYTES = 16 * 1024 * 1024
MAX_DECODED_SAMPLES = 2_000_000
_COEFFICIENTS = ((0, 0), (60, 0), (115, -52), (98, -55), (122, -60))


def _decode_vag_blocks(payload: bytes) -> tuple[list[int], list[dict[str, Any]], dict[str, Any]]:
    if len(payload) > MAX_SOURCE_AUDIO_BYTES:
        raise ValueError(f"compressed sample exceeds {MAX_SOURCE_AUDIO_BYTES}-byte decode limit")
    pcm: list[int] = []
    loops: list[dict[str, Any]] = []
    block_flags = [payload[offset + 1] for offset in range(0, len(payload) - 15, 16)]
    hist1 = hist2 = 0
    termination_block = None
    trailing = b""
    for offset in range(0, len(payload), 16):
        block = payload[offset:offset + 16]
        if len(block) != 16:
            raise ValueError("VAG ADPCM data must contain complete 16-byte blocks")
        predictor, shift = block[0] >> 4, block[0] & 0x0F
        flags = block[1]
        if predictor >= len(_COEFFICIENTS) or shift > 12:
            raise ValueError(f"invalid ADPCM predictor/shift at block {offset // 16}")
        if len(pcm) + 28 > MAX_DECODED_SAMPLES:
            raise ValueError(f"decoded sample exceeds {MAX_DECODED_SAMPLES}-sample limit")
        marker = {"block": offset // 16, "sample": len(pcm), "raw_flags": flags}
        if flags & 0x04:
            loops.append({"kind": "loop_start", **marker})
        c1, c2 = _COEFFICIENTS[predictor]
        for packed in block[2:]:
            for nibble in (packed & 0x0F, packed >> 4):
                sample = nibble if nibble < 8 else nibble - 16
                sample = (sample << 12) >> shift
                sample += (hist1 * c1 + hist2 * c2 + 32) >> 6
                sample = max(-32768, min(32767, sample))
                pcm.append(sample)
                hist2, hist1 = hist1, sample
        if flags & 0x01:
            termination_block = offset // 16
            loops.append({"kind": "end", "sample": len(pcm), "repeat": bool(flags & 0x02), **{k: v for k, v in marker.items() if k != "sample"}})
            trailing = payload[offset + 16:]
            break
        elif flags & 0x02:
            loops.append({"kind": "ignored_repeat_bit_without_end", "sample": len(pcm), **{k: v for k, v in marker.items() if k != "sample"}})
    info = {
        "decoded_block_count": len(pcm) // 28,
        "source_block_count": len(block_flags),
        "source_block_flags": block_flags,
        "termination_block": termination_block,
        "trailing_source_bytes": len(trailing),
        "trailing_source_sha256": hashlib.sha256(trailing).hexdigest() if trailing else None,
        "playback_model": "linear audition through first end block; loop markers are reported, not replayed",
    }
    return pcm, loops, info


def _inspect_vab_header(data: bytes, source_path: str | Path) -> dict[str, Any]:
    result: dict[str, Any] = {
        "source": str(source_path), "format": "sony_vab_header", "status": "invalid",
        "size_bytes": len(data), "exportable": False, "limitations": [],
    }
    if len(data) < 0x20 or data[:4] != VAB_MAGIC:
        result["limitations"].append("truncated or invalid pBAV header")
        return result
    version, vab_id, total_size = struct.unpack_from("<III", data, 4)
    if version not in (6, 7):
        result["status"] = "unsupported"
        result["limitations"].append(f"unsupported VAB version {version}; supported versions are 6 and 7")
        return result
    program_count, tone_count, vag_count = struct.unpack_from("<HHH", data, 0x12)
    # Sony's table enumerates VAG(0) through VAG(VAG count). The shipped set
    # has an empty size entry at index 0 in every inspected bank; see check below.
    # Program count is direct in the shipped headers.
    programs = program_count
    vags = vag_count + 1
    # The Sony VAB header contains 128 program records, 16 tone records per
    # declared program, then a fixed 256-entry table of sizes in 8-byte units.
    sizes_offset = 0x820 + programs * 0x200
    if programs > 128 or vags > 256 or sizes_offset + 0x200 > len(data):
        result["limitations"].append("VAB counts exceed the bounded Sony header tables")
        return result
    sizes = [struct.unpack_from("<H", data, sizes_offset + i * 2)[0] * 8 for i in range(vags)]
    if sizes and sizes[0] != 0:
        result["status"] = "unsupported"
        result["limitations"].append("VAG(0) has nonzero size; this reader only supports the observed game variant with an empty index 0")
        return result
    if any(size % 16 for size in sizes):
        result["limitations"].append("VAB sample size is not aligned to 16-byte ADPCM blocks")
        return result
    sample_offsets: list[dict[str, int]] = []
    cursor = 0
    for index, size in enumerate(sizes):
        sample_offsets.append({"index": index, "offset": cursor, "size_bytes": size, "empty": size == 0})
        cursor += size
    if total_size and total_size < len(data):
        result["limitations"].append("declared VAB total size is smaller than the header file")
        return result
    result.update({
        "status": "needs_pair", "metadata": {
            "version": version, "vab_id": vab_id, "declared_total_size": total_size,
            "program_count": programs, "tone_count_field": tone_count,
            "declared_vag_count_field": vag_count,
            "sample_table_entry_count": len(sample_offsets), "sample_sizes_table_offset": sizes_offset,
            "sample_data_size_bytes": cursor, "samples": sample_offsets,
            "paired_body_name": Path(source_path).with_suffix(".VB").name,
        },
        "limitations": ["VAB samples contain no native sample rate; WAV output needs an explicit audition rate"],
    })
    # Combined .VAB stores its body immediately after the size table.
    combined_start = sizes_offset + 0x200
    if len(data) >= combined_start + cursor:
        result["metadata"]["combined_body_offset"] = combined_start
        result["metadata"]["combined_body"] = True
    else:
        result["metadata"]["combined_body"] = False
    return result


def inspect_audio(data: bytes, source_path: str | Path) -> dict[str, Any]:
    """Describe recognized audio input without guessing unsupported formats."""
    name = str(source_path)
    suffix = Path(name).suffix.lower()
    if data.startswith(VAB_MAGIC):
        return _inspect_vab_header(data, source_path)
    result: dict[str, Any] = {
        "source": name,
        "format": "unknown",
        "status": "unsupported",
        "size_bytes": len(data),
        "exportable": False,
        "limitations": [],
    }
    if data.startswith(VAG_MAGIC):
        result["format"] = "sony_vagp"
        if len(data) < VAG_HEADER_SIZE:
            result["status"] = "invalid"
            result["limitations"].append("truncated VAGp header")
            return result
        version, reserved, body_size, rate = struct.unpack_from(">IIII", data, 4)
        if version not in (0x02, 0x20):
            result["status"] = "unsupported"
            result["limitations"].append(f"VAGp version 0x{version:08x} is not a supported SPU-ADPCM version")
            return result
        body_end = VAG_HEADER_SIZE + body_size
        if reserved != 0 or body_size == 0 or body_size % 16 or body_end > len(data):
            result["status"] = "invalid"
            result["limitations"].append("invalid VAGp reserved field or declared sample extent")
            return result
        if rate == 0 or rate > 192000:
            result["status"] = "unsupported"
            result["limitations"].append("VAGp sample rate is outside the supported range 1 through 192000 Hz")
            return result
        try:
            _, loops, decode_info = _decode_vag_blocks(data[VAG_HEADER_SIZE:body_end])
        except ValueError as exc:
            result["status"] = "invalid"
            result["limitations"].append(str(exc))
            return result
        result.update({
            "status": "supported", "exportable": True,
            "metadata": {
                "sample_rate_hz": rate, "channels": 1, "encoding": "Sony SPU ADPCM",
                "version": version, "data_offset": VAG_HEADER_SIZE,
                "data_size_bytes": body_size, "trailing_bytes": len(data) - body_end,
                "loop_markers": loops,
                "decode": decode_info,
            },
            "limitations": ["standalone sample decode only; no VAB instrument mapping or sequence rendering"],
        })
        return result
    if suffix in {".xa", ".str"}:
        result["format"] = "xa_or_str_stream"
        result["limitations"].append("CD-XA/Form2 stream decoding and video multiplex handling are unsupported")
    elif suffix in {".seq", ".vab", ".vb", ".vabp"}:
        result["format"] = "game_sound_bank_or_sequence"
        result["limitations"].append("game VAB/sequence pack layout is not established; samples and songs cannot be inferred")
    elif suffix in {".cda", ".cdda"}:
        result["format"] = "cd_da_reference"
        result["limitations"].append("CD-DA reference has no audio payload in this file")
    return result


def export_audio(
    data: bytes, output_dir: str | Path, asset_id: str, *,
    sample_rate: int | None = None, body: bytes | None = None,
    sample_index: int | None = None,
) -> dict[str, Any]:
    """Export supported samples as PCM WAV plus JSON provenance.

    For raw VAB samples ``sample_rate`` is an explicit audition rate. If no
    sample index is supplied, each non-empty sample is written separately.
    """
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._-]{0,127}", asset_id) or asset_id in {".", ".."}:
        raise ValueError("asset_id must be a simple filename component (letters, numbers, dot, underscore, hyphen)")
    info = inspect_audio(data, asset_id)
    if info["format"] == "sony_vab_header":
        meta = info.get("metadata")
        if not meta or info["status"] == "invalid":
            return info
        if body is None and meta.get("combined_body"):
            start = meta["combined_body_offset"]
            body = data[start:start + meta["sample_data_size_bytes"]]
        if body is None:
            info["limitations"].append("matching VB sample body was not supplied")
            return info
        if len(body) < meta["sample_data_size_bytes"]:
            info["status"] = "invalid"
            info["limitations"].append("paired VB body is shorter than VAB sample-size table")
            return info
        if sample_rate is None or sample_rate <= 0 or sample_rate > 192000:
            info["limitations"].append("pass an explicit sample_rate from 1 through 192000 Hz to export raw VAB samples")
            return info
        samples = [sample for sample in meta["samples"] if sample["index"] != 0]
        if sample_index is not None:
            if sample_index < 1 or sample_index >= meta["sample_table_entry_count"]:
                info["status"] = "invalid"
                info["limitations"].append("sample_index is outside the VAB sample table")
                return info
            samples = [meta["samples"][sample_index]]
        out_dir = Path(output_dir)
        out_dir.mkdir(parents=True, exist_ok=True)
        planned_names = [f"{asset_id}.sample-{sample['index']:03d}.wav" for sample in samples if sample["size_bytes"]]
        if f"{asset_id}.json" in {path.name for path in out_dir.iterdir()} or any((out_dir / filename).exists() for filename in planned_names):
            raise FileExistsError("audio export would overwrite an existing output")
        files = []
        unsupported_samples = []
        for sample in samples:
            size = sample["size_bytes"]
            if not size:
                continue
            start = sample["offset"]
            try:
                pcm, loops, decode_info = _decode_vag_blocks(body[start:start + size])
            except ValueError as exc:
                unsupported_samples.append({"sample_index": sample["index"], "reason": str(exc)})
                continue
            wav_path = out_dir / f"{asset_id}.sample-{sample['index']:03d}.wav"
            with wav_path.open("xb") as output_file, wave.open(output_file, "wb") as wav:
                wav.setnchannels(1)
                wav.setsampwidth(2)
                wav.setframerate(sample_rate)
                wav.writeframes(struct.pack(f"<{len(pcm)}h", *pcm))
            files.append({"sample_index": sample["index"], "path": wav_path.name, "decoded_samples": len(pcm), "loop_markers": loops, **decode_info})
        result = dict(info)
        result["status"] = "exported" if files and not unsupported_samples else "partial" if files else "unsupported"
        result["exportable"] = bool(files)
        result["exported"] = bool(files)
        result["audition_sample_rate_hz"] = sample_rate
        result["outputs"] = files
        result["unsupported_samples"] = unsupported_samples
        (out_dir / f"{asset_id}.json").write_text(json.dumps(result, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        return result
    if not info["exportable"]:
        return info
    meta = info["metadata"]
    end = meta["data_offset"] + meta["data_size_bytes"]
    pcm, _, decode_info = _decode_vag_blocks(data[meta["data_offset"]:end])
    out_dir = Path(output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    wav_path = out_dir / f"{asset_id}.wav"
    if wav_path.exists() or (out_dir / f"{asset_id}.json").exists():
        raise FileExistsError("audio export would overwrite an existing output")
    with wav_path.open("xb") as output_file, wave.open(output_file, "wb") as wav:
        wav.setnchannels(1)
        wav.setsampwidth(2)
        wav.setframerate(meta["sample_rate_hz"])
        wav.writeframes(struct.pack(f"<{len(pcm)}h", *pcm))
    result = dict(info)
    result["output"] = wav_path.name
    result["decoded_samples"] = len(pcm)
    result["decode"] = decode_info
    result["status"] = "exported"
    result["exported"] = True
    (out_dir / f"{asset_id}.json").write_text(json.dumps(result, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return result
