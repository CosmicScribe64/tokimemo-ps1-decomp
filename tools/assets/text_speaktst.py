"""Strict SPEAKTST layout inventory, without file installation or runtime claims.

The shipped twelve layouts preserve a reviewed conditional talk-grammar view.
Fixed-stride records and metadata bytes remain opaque. Reconstruction consumes
stored spans; validation independently reparses the original and compares every
field. Neither API writes files or consults research/cache artifacts.
"""
from __future__ import annotations

import hashlib
import json
import struct
from collections.abc import Mapping
from pathlib import Path, PurePosixPath
from types import MappingProxyType

from .text import TextFormatError, parse_script

PROFILE_SHA256 = "7a087996cb48c6b128d819e16852516d11b4ada28f45abbd0c1898f51fb2cdbf"
FLAGS = (
    "actual_file_loader_unproved", "actual_group_root_installation_unproved",
    "runtime_execution_unproved", "record24_field_semantics_unproved",
    "metadata_byte_semantics_unproved", "conditional_talk_grammar_model_only",
)


def _digest(data):
    return hashlib.sha256(data).hexdigest()


def _require(condition, message):
    if not condition:
        raise TextFormatError(message)


def _freeze(value):
    if isinstance(value, dict):
        return MappingProxyType({key: _freeze(item) for key, item in value.items()})
    if isinstance(value, list):
        return tuple(_freeze(item) for item in value)
    return value


def _plain(value):
    if isinstance(value, Mapping):
        return {key: _plain(item) for key, item in value.items()}
    if isinstance(value, (list, tuple)):
        return [_plain(item) for item in value]
    return value


def _load_profiles():
    try:
        raw = Path(__file__).with_name("text_speaktst_profiles.json").read_bytes()
        _require(_digest(raw) == PROFILE_SHA256, "SPEAKTST shipped profile hash mismatch")
        layouts = json.loads(raw)
        _require(isinstance(layouts, list) and len(layouts) == 12,
                 "SPEAKTST requires twelve shipped layouts")
        result = {item["path"]: _freeze(item) for item in layouts}
        _require(len(result) == 12 and sum(item["text_records"] for item in layouts) == 16702,
                 "SPEAKTST shipped profile cardinality mismatch")
        return MappingProxyType(result)
    except (OSError, ValueError, KeyError, TypeError) as error:
        raise TextFormatError("cannot load SPEAKTST shipped profiles") from error


SPEAKTST_LAYOUT_PROFILES = _load_profiles()
SPEAKTST_PROFILES = SPEAKTST_LAYOUT_PROFILES


def _integer(value, name, minimum=0):
    _require(type(value) is int and value >= minimum, "invalid SPEAKTST " + name)
    return value


def _path(path):
    _require(isinstance(path, str) and bool(path) and "\\" not in path,
             "invalid SPEAKTST source path")
    p = PurePosixPath(path)
    _require(not p.is_absolute() and str(p) == path and ".." not in p.parts,
             "invalid SPEAKTST source path")
    return path


def _profile(path, profile):
    # Recheck the shipped bytes at the public boundary, including repeated calls.
    shipped = _load_profiles()
    if path in shipped:
        canonical = _plain(shipped[path])
        if profile is not None:
            _require(_plain(profile) == canonical, "cannot override a shipped SPEAKTST layout")
        return canonical
    _require(isinstance(profile, Mapping), "unsupported SPEAKTST input path")
    result = _plain(profile)
    _require(result.get("path") == path, "SPEAKTST profile path mismatch")
    return result


def _span(data, offset, length, role):
    _integer(offset, "span offset")
    _integer(length, "span length")
    _require(offset + length <= len(data), "SPEAKTST span outside original input")
    raw = data[offset:offset + length]
    return {"offset": offset, "length": length, "raw_hex": raw.hex(),
            "sha256": _digest(raw), "role": role}


def _word(data, offset):
    _span(data, offset, 4, "pointer_check")
    return struct.unpack_from("<I", data, offset)[0]


def _layout(data, path, layout):
    _require(isinstance(data, bytes), "SPEAKTST data must be bytes")
    size = _integer(layout.get("size"), "profile size", 4)
    count = _integer(layout.get("text_records"), "text record count", 1)
    _require(len(data) == size and _digest(data) == layout.get("sha256"),
             "SPEAKTST exact original input hash/size required")
    _require((sum(data[:-4]) & 65535) == int.from_bytes(data[-4:-2], "big"),
             "SPEAKTST native checksum mismatch")
    profile = layout.get("profile")
    _require(isinstance(profile, dict) and profile.get("sha256") == layout["sha256"],
             "SPEAKTST parser profile identity mismatch")
    base = _integer(profile.get("load_address"), "load address")
    sections = profile.get("sections")
    _require(isinstance(sections, list) and len(sections) == 1,
             "SPEAKTST requires one candidate section")
    section = sections[0]
    _require(isinstance(section, dict) and type(section.get("group_count")) is int
             and section["group_count"] == 1, "SPEAKTST requires one candidate group")
    for key in ("text_table", "command_table", "voice_table"):
        address = _integer(section.get(key), key)
        _span(data, address - base, 4, key)
    metadata = _integer(layout.get("metadata_array_offset"), "metadata array offset")
    _span(data, metadata, count, "opaque_metadata_array")
    text_list = _word(data, section["text_table"] - base) - base
    command_list = _word(data, section["command_table"] - base) - base
    record_list = _word(data, section["voice_table"] - base) - base
    _span(data, text_list, count * 4, "candidate_text_pointer_array")
    _span(data, command_list, count + 1, "candidate_command_array")
    _span(data, record_list, count * 4, "candidate_record24_pointer_array")
    _require(data[command_list + count] == 255 and 255 not in data[command_list:command_list + count],
             "SPEAKTST FF position/count mismatch")
    first = _word(data, record_list) - base
    _span(data, first, count * 24, "opaque_record24_array")
    for index in range(count):
        _require(_word(data, record_list + index * 4) - base == first + index * 24,
                 "SPEAKTST record24 pointer stride mismatch")
    return profile, section, count, base, record_list, first, metadata


def _check_tokens(raw, tokens, offset):
    _integer(offset, "text source offset")
    _require(isinstance(tokens, list), "invalid SPEAKTST token list")
    used = bytearray(len(raw))
    for token in tokens:
        _require(isinstance(token, dict), "invalid SPEAKTST token")
        start = _integer(token.get("offset"), "token offset")
        piece = _hex(token.get("raw_hex"))
        _require(start + len(piece) <= len(raw) and raw[start:start + len(piece)] == piece,
                 "SPEAKTST token bytes outside/different from source")
        _require(type(token.get("source_offset")) is int and token["source_offset"] == offset + start,
                 "SPEAKTST token physical offset mismatch")
        used[start:start + len(piece)] = b"\1" * len(piece)
    _require(all(used), "SPEAKTST tokens do not retain all source bytes")


def parse_speaktst_layout(data, path, profile=None):
    """Return a lossless, flagged layout bank; custom fixture profiles are explicit.

    Canonical production paths always require the exact shipped layout, input
    SHA256/size and native checksum. No speaker, voice or character links are
    exported, even where the conditional grammar parser has such hypotheses.
    """
    path = _path(path)
    layout = _profile(path, profile)
    try:
        parser_profile, section, count, base, record_list, first, metadata = _layout(data, path, layout)
        parsed = parse_script(data, path, parser_profile)
        _require(parsed["coverage"]["text_records"] == count and len(parsed["records"]) == count + 1,
                 "SPEAKTST candidate record count mismatch")
        roots = [_span(data, section[key] - base, 4, "candidate_" + key + "_root")
                 for key in ("text_table", "command_table", "voice_table")]
        records, controls = [], []
        for row in parsed["records"]:
            line = row["line"]
            prefix = f"speaktst-layout:{path}:g{row['group']:04d}:l{line:04d}"
            command = _span(data, row["command"]["offset"], 1, "candidate_command_byte")
            if "text" not in row:
                _require(row["command"]["value"] == 255 and line == count,
                         "SPEAKTST unexpected control-only record")
                controls.append({"id": prefix + ":end", "source_path": path,
                                 "source_sha256": layout["sha256"], "group": row["group"],
                                 "line": line, "command_span": command,
                                 "candidate_group_roots": roots, "flags": list(FLAGS),
                                 "runtime_usage_proven": False})
                continue
            text = row["text"]
            raw = bytes.fromhex(text["raw_hex"])
            text_span = _span(data, text["offset"], len(raw) + 1, "nul_terminated_candidate_text")
            _require(bytes.fromhex(text_span["raw_hex"]) == raw + b"\0" and b"\0" not in raw,
                     "SPEAKTST text terminator mismatch")
            tokens = [dict(token, source_offset=text["offset"] + token["offset"])
                      for token in text["tokens"]]
            _check_tokens(raw, tokens, text["offset"])
            records.append({
                "schema": "tokimemo.speaktst.layout-candidate.record.v1",
                "id": prefix + f":o{text['offset']:08x}",
                "physical_text_id": f"speaktst-physical:{path}:{text['offset']:08x}:{_digest(raw)[:12]}",
                "source_path": path, "source_sha256": layout["sha256"],
                "source_offset": text["offset"], "group": row["group"], "line": line,
                "raw_hex": raw.hex(), "raw_sha256": _digest(raw), "tokens": tokens,
                "source_text": text.get("source_decoded_text"),
                "candidate_grammar": {
                    "command_value": row["command"]["value"], "command_name": row["command"]["name"],
                    "tokenization_model": "inherited_native_talk_grammar; file installation unproved",
                    "details": {key: value for key, value in text.items() if key not in (
                        "raw_hex", "tokens", "offset", "length", "pointer_offset", "pointer_value",
                        "source_decoded_text")}},
                "spans": {
                    "text_with_nul": text_span, "command": command,
                    "text_pointer": _span(data, text["pointer_offset"], 4, "candidate_text_pointer"),
                    "group_roots": roots,
                    "record24_pointer": _span(data, record_list + line * 4, 4, "candidate_record24_pointer"),
                    "record24": _span(data, first + line * 24, 24, "opaque_fixed_stride_record24"),
                    "metadata_byte": _span(data, metadata + line, 1, "opaque_metadata_byte")},
                "unproved_field_semantics": ["record24_is_not_proved_voice_link",
                                             "metadata_byte_is_not_proved_character_id"],
                "readiness": {"status": "flagged", "translation_ready": False,
                              "blockers": list(FLAGS) + (["unknown_renderer_tokens"]
                                          if text.get("unknown_token_count") else [])},
                "runtime_usage_proven": False, "target_text": None})
        opaque = [{"source_path": path, "source_sha256": layout["sha256"],
                   "span": _span(data, item["offset"], item["length"],
                                 "uninterpreted_by_inherited_conditional_parser"),
                   "status": "opaque", "may_overlap_exposed_record24_or_metadata_spans": True}
                  for item in parsed["unclaimed_ranges"]]
        bank = {"schema": "tokimemo.speaktst.layout-bank.v1", "status": "layout_only_inventory",
                "source": {"path": path, "sha256": layout["sha256"], "size": len(data),
                           "load_address": base},
                "records": records, "controls": controls, "opaque_ranges": opaque,
                "coverage": {"conditional_parser": parsed["coverage"], "text_records": len(records),
                             "control_records": len(controls), "opaque_ranges": len(opaque),
                             "container_bytes": len(data), "semantic_coverage_complete": False,
                             "runtime_file_installation_proven": False, "runtime_execution_proven": False,
                             "record24_voice_semantics_proven": False,
                             "metadata_character_semantics_proven": False},
                "flags": list(FLAGS)}
        _require(len(controls) == 1, "SPEAKTST requires one FF control")
        _require(reconstruct_speaktst_layout(bank) == data, "SPEAKTST retained spans differ from source")
        return bank
    except (KeyError, TypeError, ValueError, struct.error, OverflowError) as error:
        raise TextFormatError("invalid SPEAKTST layout or parsed fields") from error


def _hex(value):
    _require(isinstance(value, str), "invalid SPEAKTST raw hex")
    try:
        raw = bytes.fromhex(value)
    except ValueError as error:
        raise TextFormatError("invalid SPEAKTST raw hex") from error
    _require(raw.hex() == value, "SPEAKTST hex must be canonical lowercase")
    return raw


def reconstruct_speaktst_layout(bank):
    """Rebuild bytes exclusively from retained spans, rejecting gaps/conflicts.

    Full semantic/field authenticity additionally requires validate_speaktst_layout
    against the original input. Matching overlaps are expected for opaque fields.
    """
    _require(isinstance(bank, dict), "invalid SPEAKTST bank")
    try:
        source = bank["source"]
        _require(isinstance(source, dict), "invalid SPEAKTST source")
        path = _path(source["path"])
        size = _integer(source["size"], "source size")
        digest = source["sha256"]
        _require(isinstance(digest, str) and len(digest) == 64, "invalid SPEAKTST source hash")
        # Reject impossible allocation requests before allocating a source-sized
        # buffer. A lossless bank necessarily stores at least size span bytes.
        def retained_length(value):
            if isinstance(value, dict):
                if all(key in value for key in ("offset", "length", "raw_hex", "sha256", "role")):
                    return len(_hex(value["raw_hex"]))
                return sum(retained_length(item) for item in value.values())
            if isinstance(value, list):
                return sum(retained_length(item) for item in value)
            return 0

        _require(sum(retained_length(bank[key]) for key in ("records", "controls", "opaque_ranges")) >= size,
                 "SPEAKTST retained spans cannot cover source size")
        restored, used = bytearray(size), bytearray(size)

        def consume(value):
            if isinstance(value, dict):
                if all(key in value for key in ("offset", "length", "raw_hex", "sha256", "role")):
                    start = _integer(value["offset"], "span offset")
                    length = _integer(value["length"], "span length")
                    _require(start + length <= size, "SPEAKTST span outside source")
                    raw = _hex(value["raw_hex"])
                    _require(len(raw) == length and _digest(raw) == value["sha256"],
                             "SPEAKTST span length/hash mismatch")
                    for offset, byte in enumerate(raw, start):
                        _require(not used[offset] or restored[offset] == byte,
                                 "SPEAKTST conflicting retained spans")
                    restored[start:start + length] = raw
                    used[start:start + length] = b"\1" * length
                else:
                    for item in value.values():
                        consume(item)
            elif isinstance(value, list):
                for item in value:
                    consume(item)

        for key in ("records", "controls", "opaque_ranges"):
            rows = bank[key]
            _require(isinstance(rows, list), "invalid SPEAKTST retained rows")
            for row in rows:
                _require(isinstance(row, dict) and row.get("source_path") == path
                         and row.get("source_sha256") == digest, "SPEAKTST row source identity mismatch")
                consume(row)
                if key == "records":
                    raw = _hex(row["raw_hex"])
                    _require(_digest(raw) == row["raw_sha256"], "SPEAKTST text hash mismatch")
                    text_span = row["spans"]["text_with_nul"]
                    _require(text_span["offset"] == row["source_offset"]
                             and _hex(text_span["raw_hex"]) == raw + b"\0" and b"\0" not in raw,
                             "SPEAKTST text differs from retained terminated span")
                    _check_tokens(raw, row["tokens"], row["source_offset"])
        _require(all(used), "SPEAKTST retained spans leave gaps")
        result = bytes(restored)
        _require(_digest(result) == digest, "SPEAKTST reconstructed source hash mismatch")
        return result
    except (KeyError, TypeError, ValueError, OverflowError) as error:
        raise TextFormatError("invalid SPEAKTST retained bank") from error


def validate_speaktst_layout(data, path, bank, profile=None):
    """Require every persisted field to equal a fresh canonical parse, then rebuild."""
    canonical = parse_speaktst_layout(data, path, profile)
    try:
        actual_json = json.dumps(bank, sort_keys=True, ensure_ascii=False, allow_nan=False)
        expected_json = json.dumps(canonical, sort_keys=True, ensure_ascii=False, allow_nan=False)
    except (TypeError, ValueError) as error:
        raise TextFormatError("SPEAKTST bank is not JSON-compatible") from error
    _require(actual_json == expected_json, "SPEAKTST persisted fields differ from canonical reparse")
    _require(reconstruct_speaktst_layout(bank) == data, "SPEAKTST reconstruction differs from original")
    return {"status": "validated_layout_only_inventory", "source": dict(canonical["source"]),
            "text_records": len(bank["records"]), "control_records": len(bank["controls"]),
            "unknown_tokens": sum(token["kind"] == "unknown" for row in bank["records"]
                                  for token in row["tokens"]),
            "reconstructed_bytes": len(data), "canonical_fields_identical": True,
            "semantic_coverage_complete": False, "runtime_file_installation_proven": False,
            "runtime_execution_proven": False}
