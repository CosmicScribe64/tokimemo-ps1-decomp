"""Bounded Sony PlayStation SEQp reader with byte-exact reconstruction."""

from __future__ import annotations

import hashlib
import struct
from pathlib import Path
from typing import Any

MAGIC = b"pQES"
END_MARKER = b"\xff\x2f\x00"
MAX_INPUT = 16 * 1024 * 1024


def _u(data: bytes, start: int, size: int) -> int:
    if start < 0 or start + size > len(data):
        raise ValueError("truncated SEQp header")
    return int.from_bytes(data[start : start + size], "big")


def inspect_seq(data: bytes, source_path: str = "") -> dict[str, Any]:
    """Parse the game's 16-slot offset table and bounded SEQp entries."""
    if len(data) > MAX_INPUT:
        raise ValueError("input exceeds 16 MiB safety limit")
    if len(data) < 0x40:
        raise ValueError("truncated game sequence offset table")
    offsets = [_u_le(data, i * 4, 4) for i in range(16)]
    first = offsets[0]
    resolved = [off if off else first for off in offsets]
    starts = [off + 0x10 for off in resolved]
    if any(off < 0x40 or off >= len(data) or not data.startswith(MAGIC, off) for off in starts):
        raise ValueError("sequence offset table points outside file or not to SEQp")
    unique_starts = sorted(set(starts))
    sequences = []
    for start in unique_starts:
        header_end = start + 0x0F
        if header_end > len(data):
            raise ValueError("truncated SEQp header")
        limit = min((other for other in unique_starts if other > start), default=len(data))
        # Event payloads may contain the marker byte pattern, so within the
        # next-entry bound use the last marker as the documented terminator.
        if limit < header_end:
            raise ValueError("sequence entries overlap their fixed header")
        marker_offsets = []
        cursor = header_end
        while True:
            cursor = data.find(END_MARKER, cursor, limit)
            if cursor < 0:
                break
            marker_offsets.append(cursor)
            cursor += 1
        score = data[header_end:limit]
        sequences.append({
            "offset": start,
            "bound_end": limit,
            "header_raw_hex": data[start:header_end].hex(),
            "header": {
                "magic_ascii": "SEQp",
                "version_raw_hex": data[start + 4 : start + 8].hex(),
                "version_u32_be": _u(data, start + 4, 4),
                "quarter_note_resolution": _u(data, start + 8, 2),
                "tempo_raw_hex": data[start + 10 : start + 13].hex(),
                "tempo_u24_be": _u(data, start + 10, 3),
                "rhythm_raw_hex": data[start + 13 : start + 15].hex(),
                "rhythm_bytes": list(data[start + 13 : start + 15]),
            },
            "score": {
                "offset": header_end,
                "size": len(score),
                "sha256": hashlib.sha256(score).hexdigest(),
                "raw_hex": score.hex(),
                "event_semantics": "opaque; no event grammar or terminator boundary inferred",
            },
            "end_marker_candidates": marker_offsets,
        })
    return {
        "schema": "tokimemo.sequence.v1",
        "source": source_path,
        "source_size": len(data),
        "source_sha256": hashlib.sha256(data).hexdigest(),
        "source_raw_hex": data.hex(),
        "offset_table_raw_hex": data[:0x40].hex(),
        "offset_table_le_u32": offsets,
        "prefix_gap_hex": data[0x50:min(starts)].hex(),
        "sequence_slots": [{"slot": i, "resolved_offset": starts[i]} for i in range(16)],
        "reverb_depth_raw_bytes": list(data[0x40:0x50]),
        "sequences": sequences,
        "roundtrip_sha256": hashlib.sha256(data).hexdigest(),
    }


def reconstruct_seq(record: dict[str, Any], original: bytes | None = None) -> bytes:
    """Rebuild from parsed tables, raw headers, bounded score extents and gaps."""
    table = bytes.fromhex(record["offset_table_raw_hex"])
    if len(table) != 0x40:
        raise ValueError("offset table raw extent must be 64 bytes")
    offsets = [_u_le(table, i * 4, 4) for i in range(16)]
    if offsets != record["offset_table_le_u32"]:
        raise ValueError("parsed offset table differs from raw table")
    reverb = bytes(record["reverb_depth_raw_bytes"])
    if len(reverb) != 16:
        raise ValueError("reverb-depth table raw extent must be 16 bytes")
    first = offsets[0]
    starts = [((off if off else first) + 0x10) for off in offsets]
    expected_slots = [{"slot": i, "resolved_offset": starts[i]} for i in range(16)]
    if record["sequence_slots"] != expected_slots:
        raise ValueError("resolved runtime slots disagree with source offset table")
    unique = sorted(set(starts))
    sequences = record["sequences"]
    if [item["offset"] for item in sequences] != unique:
        raise ValueError("physical sequence list disagrees with source offset table")
    gap = bytes.fromhex(record["prefix_gap_hex"])
    cursor = 0x50 + len(gap)
    parts = [table, reverb, gap]
    for sequence in sequences:
        start = sequence["offset"]
        end = sequence["bound_end"]
        header = bytes.fromhex(sequence["header_raw_hex"])
        score = bytes.fromhex(sequence["score"]["raw_hex"])
        if start != cursor or len(header) != 0x0F or end != start + len(header) + len(score):
            raise ValueError("overlapping, gapped or inconsistent sequence extent")
        if header[:4] != MAGIC:
            raise ValueError("raw sequence header lost SEQp magic")
        parsed_header = sequence["header"]
        if (_u(header, 4, 4) != parsed_header["version_u32_be"]
                or _u(header, 8, 2) != parsed_header["quarter_note_resolution"]
                or _u(header, 10, 3) != parsed_header["tempo_u24_be"]
                or list(header[13:15]) != parsed_header["rhythm_bytes"]):
            raise ValueError("decoded header fields disagree with preserved raw header")
        parts.extend((header, score))
        cursor = end
    rebuilt = b"".join(parts)
    if len(rebuilt) != record["source_size"] or hashlib.sha256(rebuilt).hexdigest() != record["source_sha256"]:
        raise ValueError("parsed fields and preserved gaps do not reconstruct source hash")
    if bytes.fromhex(record["source_raw_hex"]) != rebuilt:
        raise ValueError("reconstructed source differs from retained whole-file bytes")
    if original is not None and original != rebuilt:
        raise ValueError("parsed structure does not reconstruct original input")
    return rebuilt


def _u_le(data: bytes, start: int, size: int) -> int:
    if start < 0 or start + size > len(data):
        raise ValueError("truncated sequence offset table")
    return int.from_bytes(data[start : start + size], "little")


def inspect_file(path: str | Path) -> dict[str, Any]:
    """Read and inspect a local sequence file."""
    p = Path(path)
    return inspect_seq(p.read_bytes(), p.as_posix())


SUPPORTED_DATA_LENGTHS = {0x90: 2, 0xB0: 2, 0xC0: 1, 0xE0: 2}
SUPPORTED_CONTROLLERS = {0, 1, 6, 7, 10, 11, 64, 91, 98, 99, 100, 101, 121}


class UnsupportedEvent(ValueError):
    """The source bytes are retained but the event grammar is unsupported."""


def _read_delta(data: bytes, pos: int, limit: int) -> tuple[int, int]:
    value = 0
    for count in range(4):
        if pos >= limit:
            raise ValueError("truncated delta-time VLQ")
        byte = data[pos]
        pos += 1
        value = (value << 7) | (byte & 0x7F)
        if byte < 0x80:
            return value, pos
    raise ValueError("delta-time VLQ exceeds Sony's four-byte limit")


def parse_events(data: bytes, seq: dict[str, Any]) -> dict[str, Any]:
    """Decode proved native channel events and fixed-width FF51/FF2F meta.

    Sony's runtime overview specifies format-1 SMF-derived event fields, running
    status and a four-byte VLQ limit. Unsupported statuses/controllers fail
    closed while the package-level raw extent remains intact.
    """
    begin = seq["offset"] + 0x0F
    limit = seq["bound_end"]
    if seq["header"]["version_u32_be"] != 1:
        raise UnsupportedEvent("unsupported SEQ version %d" % seq["header"]["version_u32_be"])
    pos = begin
    running_status = None
    tick = 0
    events = []
    while pos < limit:
        event_begin = pos
        # Native ReadDeltaValue framing is authoritative: every iteration
        # consumes the next delta before interpreting an event or EOT status.
        delta, pos = _read_delta(data, pos, limit)
        tick += delta
        if pos + 3 <= limit and data[pos : pos + 3] == END_MARKER:
            loops = _loop_candidates(events)
            return {
                "status": "complete",
                "events": events,
                "loop_marker_candidates": loops,
                "end_marker_offset": pos,
                "end_marker_form": "explicit_ff_type2f_reserved_zero",
                "end_tick": tick,
                "trailer_offset": pos + 3,
                "trailer_size": limit - (pos + 3),
                "trailer_hex": data[pos + 3 : limit].hex(),
            }
        if pos >= limit:
            raise ValueError("delta VLQ has no following event")
        first = data[pos]
        implicit_meta = first < 0x80 and running_status == 0xFF
        meta = None
        explicit_ff_status = False
        if first & 0x80:
            status = first
            pos += 1
            if status == 0xFF:
                explicit_ff_status = True
                running_status = 0xFF
                if pos >= limit:
                    raise ValueError("truncated native meta type")
                meta = data[pos]
                pos += 1
            else:
                kind = status & 0xF0
                if kind not in SUPPORTED_DATA_LENGTHS:
                    raise UnsupportedEvent("unsupported channel/system status %02x" % status)
                running_status = status
                width = SUPPORTED_DATA_LENGTHS[kind]
                values = []
        elif implicit_meta:
            status = 0xFF
            meta = first
            pos += 1
        else:
            if running_status is None:
                raise ValueError("running status used before channel status")
            status = running_status
            kind = status & 0xF0
            width = SUPPORTED_DATA_LENGTHS[kind]
            values = [first]
            pos += 1

        if meta is not None:
            if meta == 0x2F:
                # GetMetaEvent's native 2F branch does not consume a payload.
                # Preserve the conventional explicit FF2F00 marker as one raw
                # terminator candidate; for cached/implicit 2F, the following
                # bytes remain an opaque trailer.
                reserved_zero_present = explicit_ff_status and pos < limit and data[pos] == 0
                trailer_offset = pos + int(reserved_zero_present)
                if not (pos <= trailer_offset <= limit):
                    raise ValueError("native EOT trailer offset escapes bounded sequence")
                return {
                    "status": "complete", "events": events,
                    "loop_marker_candidates": _loop_candidates(events),
                    "end_marker_offset": pos - (2 if not implicit_meta else 1),
                    "end_marker_form": (
                        "implicit_cached_ff" if implicit_meta else
                        "explicit_ff_type2f_reserved_zero" if reserved_zero_present else
                        "explicit_ff_type2f"
                    ),
                    "end_tick": tick, "trailer_offset": trailer_offset,
                    "trailer_size": limit - trailer_offset,
                    "trailer_hex": data[trailer_offset:limit].hex(),
                }
            if meta == 0x51:
                if pos + 3 > limit:
                    raise ValueError("truncated native FF51 tempo payload")
                payload = data[pos : pos + 3]
                pos += 3
                tempo = int.from_bytes(payload, "big")
                events.append({
                    "tick": tick, "delta": delta, "status": 0xFF,
                    "kind": "meta_tempo", "meta_type": 0x51,
                    "tempo_u24": tempo, "payload_hex": payload.hex(),
                    "channel": None, "data": [], "source_offset": event_begin,
                    "source_end": pos, "raw_hex": data[event_begin:pos].hex(),
                    "running_status": implicit_meta,
                    "implicit_status": implicit_meta,
                })
                # Native GetMetaEvent calls ReadDeltaValue but leaves cached
                # running status FF intact for implicit meta events.
                continue
            raise UnsupportedEvent("unsupported meta event %02x" % meta)
        for _ in range(width - len(values)):
            if pos >= limit:
                raise ValueError("truncated channel event data")
            byte = data[pos]
            if byte & 0x80:
                raise ValueError("status byte encountered inside fixed-width channel data")
            values.append(byte)
            pos += 1
        if kind == 0xB0 and values[0] not in SUPPORTED_CONTROLLERS:
            raise UnsupportedEvent("unsupported control-change number %d" % values[0])
        events.append({
            "tick": tick,
            "delta": delta,
            "status": status,
            "kind": kind,
            "channel": status & 0x0F,
            "data": values,
            "source_offset": event_begin,
            "source_end": pos,
            "raw_hex": data[event_begin:pos].hex(),
            "running_status": first < 0x80,
        })
    raise ValueError("bounded sequence has no explicit end marker")


def validate_event_records(data: bytes, seq: dict[str, Any], parsed: dict[str, Any]) -> None:
    """Re-read event fields from source and compare every stored parsed value."""
    if parse_events(data, seq) != parsed:
        raise ValueError("stored event records differ from source bytes")


def _loop_candidates(events: list[dict[str, Any]]) -> list[dict[str, Any]]:
    """Report Sony-documented CC99 loop label values without applying them."""
    out = []
    for index, event in enumerate(events):
        if event["kind"] == 0xB0 and event["data"][0] == 99 and event["data"][1] in (20, 30):
            out.append({
                "event_index": index,
                "tick": event["tick"],
                "channel": event["channel"],
                "cc99_value": event["data"][1],
                "candidate": "loop-start" if event["data"][1] == 20 else "loop-end",
                "semantics": "Sony SDK documents CC99 values 20 and 30 as loop delimiters; runtime loop count/control not inferred",
            })
    return out


def _vlq(value: int) -> bytes:
    if value < 0 or value > 0x0FFFFFFF:
        raise ValueError("MIDI delta outside four-byte VLQ range")
    out = [value & 0x7F]
    value >>= 7
    while value:
        out.append((value & 0x7F) | 0x80)
        value >>= 7
    return bytes(reversed(out))


def export_smf(seq: dict[str, Any], parsed: dict[str, Any]) -> bytes:
    """Write one deterministic SMF format-0 track from a fully parsed entry."""
    if parsed["status"] != "complete":
        raise ValueError("cannot export incomplete sequence")
    header = seq["header"]
    ppqn = header["quarter_note_resolution"]
    tempo = header["tempo_u24_be"]
    if not (1 <= ppqn <= 0x7FFF and 1 <= tempo <= 0xFFFFFF):
        raise ValueError("header timing fields are outside SMF range")
    events = []
    # Equal-tick metadata is emitted before source channel events, which keeps
    # source event ordering stable and makes the file self-timed.
    events.append((0, -2, b"\xff\x51\x03" + tempo.to_bytes(3, "big")))
    for index, event in enumerate(parsed["events"]):
        if event.get("kind") == "meta_tempo":
            if event["tempo_u24"] == 0:
                raise ValueError("native FF51 tempo zero cannot be serialized as a usable MIDI tempo")
            msg = b"\xff\x51\x03" + bytes.fromhex(event["payload_hex"])
        else:
            msg = bytes((event["status"], *event["data"]))
        events.append((event["tick"], index, msg))
    events.sort(key=lambda item: (item[0], item[1]))
    track = bytearray()
    previous_tick = 0
    for tick, _, message in events:
        track.extend(_vlq(tick - previous_tick))
        track.extend(message)
        previous_tick = tick
    track.extend(_vlq(parsed["end_tick"] - previous_tick))
    track.extend(b"\xff\x2f\x00")
    midi_header = b"MThd" + struct.pack(">IHHH", 6, 0, 1, ppqn)
    midi_track = b"MTrk" + struct.pack(">I", len(track)) + bytes(track)
    return midi_header + midi_track
