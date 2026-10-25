"""Bounded original BUNKA_SD prefix-stream and direct-display table reader.

Source readers display each prefixed string at +1, then inspect its first byte.
The separate direct tables go through argument-0 set_kanji_string wrappers.
All discovery is rooted in explicit source-selected tables and initialized
assembly label spans. No binary string scanning or execution is performed.
"""
from __future__ import annotations

import hashlib
import re
from collections import Counter
from pathlib import Path

from .text import TextFormatError, source_path
from .text_binary import _spans
from .text_fixed import tokenize_fixed_text
from .text_source import _decoded_view

_LABEL = re.compile(r"^\s*(dlabel|enddlabel)\s+(\S+)\s*$")
_WORD = re.compile(r"^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*\.word\s+(\S+)\s*$")
_INSTRUCTION = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)")


def _source_files(repo, profile):
    sources = {}
    for relative, digest in profile["source_pins"].items():
        source_path(relative)
        path = (repo / relative).resolve(strict=True)
        if not path.is_relative_to(repo) or not path.is_file():
            raise TextFormatError("BUNKA_SD source proof escapes selected repository")
        raw = path.read_bytes()
        if hashlib.sha256(raw).hexdigest() != digest:
            raise TextFormatError("BUNKA_SD source differs from pinned proof")
        text = raw.decode("utf-8", errors="strict")
        if "\ufffd" in text or text.encode("utf-8") != raw:
            raise TextFormatError("BUNKA_SD source is not strict round-trip UTF-8")
        sources[relative] = (path, text)
    return sources


def _blocks(sources, paths):
    result = {}
    for path in paths:
        active = None
        for number, line in enumerate(sources[path][1].splitlines(), 1):
            label = _LABEL.fullmatch(line)
            if label:
                active = None if label[1] == "enddlabel" else {"line": number, "rows": []}
                if active is not None:
                    result.setdefault((path, label[2]), []).append(active)
            elif active is not None and line.strip():
                active["rows"].append((number, line))
    return result


def _word_table(data, base, blocks, table, used, *, voice=False):
    key = (table["assembly_path"], table["label"])
    matches = blocks.get(key, [])
    if len(matches) != 1:
        raise TextFormatError("BUNKA_SD pointer/voice label is missing or ambiguous")
    count = table["pair_count"] * 2 if voice else table["count"]
    if not isinstance(count, int) or isinstance(count, bool) or not 0 < count <= 200:
        raise TextFormatError("BUNKA_SD table count outside source bounds")
    rows = matches[0]["rows"]
    if len(rows) != count:
        raise TextFormatError("BUNKA_SD table word extent differs from source count")
    output = []
    for index, (number, line) in enumerate(rows):
        match = _WORD.fullmatch(line)
        if match is None:
            raise TextFormatError("BUNKA_SD table has an unsupported word initializer")
        offset, address = int(match[1], 16), int(match[2], 16)
        raw = bytes.fromhex(match[3])
        if (address != table["address"] + index * 4 or offset != address - base or
                offset < 0 or data[offset:offset + 4] != raw):
            raise TextFormatError("BUNKA_SD word disagrees with original input/address")
        value = int.from_bytes(raw, "little")
        if voice:
            try:
                declared = int(match[4], 0)
            except ValueError as exc:
                raise TextFormatError("BUNKA_SD voice word is not an explicit numeric value") from exc
            if declared & 0xFFFFFFFF != value:
                raise TextFormatError("BUNKA_SD voice initializer disagrees with original word")
        elif not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", match[4]):
            raise TextFormatError("BUNKA_SD string pointer is not an explicit source label")
        used[offset:offset + 4] = b"\1" * 4
        output.append({"index": index, "address": address, "offset": offset,
                       "raw_hex": raw.hex(), "value": value, "target_label": match[4],
                       "assembly_path": table["assembly_path"], "assembly_line": number})
    return output


def _prefix_effect(value, family):
    common = {"value": value, "raw_hex": f"{value:02x}", "translatable": False,
              "body_argument_precedes_control_dispatch": True}
    if value == 0x20:
        return {**common, "effect": "row_continuation", "recognized": True}
    if value == 0x67:
        return {**common, "effect": "row_break", "recognized": True,
                "next_group_start": "pointer_index + 1", "voice_counter_increment": 2,
                "voice_request_condition": "circle not held" + (" and selector != 0" if family == "intro" else "")}
    if value == 0x45 and family == "intro":
        return {**common, "effect": "stream_end_after_audio_and_text_ready", "recognized": True,
                "next_group_start": "pointer_index + 1", "voice_counter_set": 255,
                "voice_request_condition": "circle not held and selector != 0"}
    return {**common, "effect": "unknown_prefix_fallthrough", "recognized": False,
            "known_handler_behavior": "body rendered, then scanner continues to the next pointer"}


def parse_bunka_sd(data: bytes, repository, profile=None) -> dict:
    """Read pinned source-selected tables; return display and full-storage views.

    ``records`` follows dictionary-record conventions with UTF-8 body views.
    ``physical_records`` retains the complete original prefix/body/NUL extent.
    Every body record has a stable physical parent and protected full tokens.
    Repository must match the pinned evidence snapshot; no source is modified.
    """
    profile = BUNKA_SD_PROFILE if profile is None else profile
    data = bytes(data)
    digest = hashlib.sha256(data).hexdigest()
    if digest != profile["sha256"] or len(data) != profile["size"]:
        raise TextFormatError("BUNKA_SD original input differs from pinned source profile")
    source_path(profile["path"])
    repo = Path(repository).resolve(strict=True)
    sources = _source_files(repo, profile)
    base = profile["load_address"]
    used = bytearray(len(data))
    code_proofs = []
    for path in profile["code_assemblies"]:
        words = []
        for number, line in enumerate(sources[path][1].splitlines(), 1):
            match = _INSTRUCTION.search(line)
            if match is None or match[3].startswith("."):
                continue
            address, raw = int(match[1], 16), bytes.fromhex(match[2])
            offset = address - base
            if (offset < 0 or data[offset:offset + 4] != raw or
                    words and address != words[-1]["address"] + 4):
                raise TextFormatError("BUNKA_SD reader instruction disagrees with original input")
            used[offset:offset + 4] = b"\1" * 4
            words.append({"address": address, "offset": offset, "raw_hex": raw.hex(), "assembly_line": number})
        if not words:
            raise TextFormatError("BUNKA_SD reader has no verified original instruction words")
        code_proofs.append({"assembly_path": path, "assembly_sha256": profile["source_pins"][path],
                            "instructions": words})
    blocks = _blocks(sources, profile["data_assemblies"])
    spans = {}
    for path in profile["data_assemblies"]:
        for span in _spans(sources[path][0], repo, (data, base, 0, digest, profile["path"])):
            if span["assembly_sha256"] != profile["source_pins"][path]:
                raise TextFormatError("BUNKA_SD initialized label source changed while reading")
            if span["address"] is not None:
                spans.setdefault((path, span["label"], span["address"]), []).append(span)
    physical, views, tables, issues = {}, {}, [], []
    table_ids = set()
    for table in profile["tables"]:
        mode, family = table["mode"], table["handler_family"]
        if (table["id"] in table_ids or mode not in ("prefixed", "direct") or
                table["body_offset"] != (1 if mode == "prefixed" else 0) or
                family not in (("intro", "scene") if mode == "prefixed" else ("direct",))):
            raise TextFormatError("BUNKA_SD table mode/identity differs from supported source readers")
        table_ids.add(table["id"])
        pointers = _word_table(data, base, blocks, table, used)
        voice = None
        if "voice" in table:
            words = _word_table(data, base, blocks, table["voice"], used, voice=True)
            voice = {"label": table["voice"]["label"], "counter_selection": "runtime_counter selects two consecutive words",
                     "counter_initial_value_not_inferred": True,
                     "pairs": [{"index": index // 2, "offset": words[index]["offset"],
                                "raw_hex": words[index]["raw_hex"] + words[index + 1]["raw_hex"],
                                "clip_start": words[index]["value"], "clip_end": words[index + 1]["value"]}
                               for index in range(0, len(words), 2)]}
        uses, controls = [], []
        for pointer in pointers:
            matches = [span for (path, label, address), values in spans.items()
                       if label == pointer["target_label"] and address == pointer["value"] for span in values]
            body_start = table["body_offset"]
            bounded = [(span, bytes(span["raw"]).find(b"\0", body_start)) for span in matches]
            bounded = [(span, end) for span, end in bounded if end >= body_start]
            extents = {bytes(span["raw"])[:end + 1] for span, end in bounded}
            if len(extents) != 1:
                raise TextFormatError("BUNKA_SD target is not one exact initialized label with bounded body NUL")
            full_raw = next(iter(extents))
            body_raw = full_raw[body_start:-1]
            offset = pointer["value"] - base
            if offset < 0 or data[offset:offset + len(full_raw)] != full_raw:
                raise TextFormatError("BUNKA_SD target label disagrees with original bytes")
            used[offset:offset + len(full_raw)] = b"\1" * len(full_raw)
            physical_id = f"{profile['path']}:{offset:08x}:{hashlib.sha256(full_raw).hexdigest()[:12]}"
            if physical_id not in physical:
                physical[physical_id] = {"id": physical_id, "raw_hex": full_raw.hex(), "raw_length": len(full_raw),
                    "source": {"path": profile["path"], "sha256": digest, "file_size": len(data),
                               "address": pointer["value"], "offset": offset, "length": len(full_raw)},
                    **_decoded_view(full_raw, True), "terminator_relative_offset": len(full_raw) - 1,
                    "extent_semantics": [],
                    "assembly_sites": [{**{key: value for key, value in span.items() if key not in ("raw", "stopped")},
                                        "verified_initialized_prefix_length": len(span["raw"]),
                                        "terminator_relative_offset": end}
                                       for span, end in bounded]}
            extent_semantics = "control_prefix_body_nul" if body_start else "direct_cstring"
            if extent_semantics not in physical[physical_id]["extent_semantics"]:
                physical[physical_id]["extent_semantics"].append(extent_semantics)
            identifier = f"bunka-sd-view:{physical_id}:skip{body_start}"
            prefix = _prefix_effect(full_raw[0], family) if body_start else None
            if prefix:
                controls.append(prefix["value"])
                if not prefix["recognized"]:
                    issues.append({"code": "unrecognized_control_prefix", "record_id": identifier,
                                   "table": table["id"], "index": pointer["index"], "value": prefix["value"]})
            use = {"table": table["id"], "pointer_index": pointer["index"], "pointer_evidence": pointer,
                   "source_conditions": table["conditions"], "source_evidence": table["evidence"],
                   "mode": mode, "handler_family": family, "prefix_control": prefix,
                   "voice_table_label": table.get("voice", {}).get("label"),
                   "voice_counter_expression": "game_state.substep" if family == "intro" else
                                               "u8(game_state.mode_stage_scratch)" if family == "scene" else None,
                   "voice_pair_selection": "runtime counter; no static row-to-pair assignment inferred" if voice else None,
                   "runtime_reachability_proven": False}
            if identifier not in views:
                text = tokenize_fixed_text(body_raw, "set_kanji_string")
                full_tokens = ([{"kind": "protected_control_prefix", "offset": 0,
                                  "raw_hex": full_raw[:1].hex(), "translatable": False}] if body_start else [])
                full_tokens.extend({**token, "offset": token["offset"] + body_start} for token in text["tokens"])
                full_tokens.append({"kind": "source_terminator", "offset": len(full_raw) - 1,
                                    "raw_hex": "00", "translatable": False})
                views[identifier] = {"id": identifier, "table": table["id"], "index": pointer["index"],
                    "source_path": profile["path"], "source_sha256": digest,
                    "source_physical_record_id": physical_id, "source_relative_offset": body_start,
                    "offset": offset + body_start, "length": len(body_raw), "text": text,
                    "slot_offset": pointer["offset"], "slot_raw_hex": pointer["raw_hex"],
                    "full_offset": offset, "full_address": pointer["value"], "full_length": len(full_raw),
                    "full_raw_hex": full_raw.hex(), "full_tokens": full_tokens,
                    "terminator_offset": offset + len(full_raw) - 1,
                    "source_slot_status": "empty" if not body_raw else "whitespace_only" if text["source_decoded_text"] and text["source_decoded_text"].isspace() else "source_text",
                    "context": {"physical_id": physical_id, "body_relative_offset": body_start,
                                "protected_prefix_hex": full_raw[:body_start].hex(), "uses": [],
                                "renderer": "set_kanji_string", "runtime_reachability_proven": False,
                                "renderer_limits_are_provenance_only": True}}
            views[identifier]["context"]["uses"].append(use)
            uses.append({"record_id": identifier, "physical_id": physical_id, **pointer})
        if mode == "prefixed":
            expected_last = 0x45 if family == "intro" else 0x67
            if controls[-1] != expected_last or family == "intro" and controls.count(0x45) != 1:
                issues.append({"code": "source_stream_terminal_control_not_at_table_end", "table": table["id"]})
            if voice and len(voice["pairs"]) != sum(value in ((0x67, 0x45) if family == "intro" else (0x67,)) for value in controls):
                issues.append({"code": "voice_pairs_differ_from_static_break_count", "table": table["id"]})
        tables.append({"id": table["id"], "mode": mode, "count": len(pointers),
                       "source_label": table["label"], "rows": uses, "voice": voice,
                       "prefix_histogram": dict(Counter(controls))})
    # Recheck source pins after parsing to avoid combining evidence from edits.
    _source_files(repo, profile)
    unclaimed = []
    position = 0
    while position < len(data):
        if used[position]:
            position += 1
            continue
        start = position
        while position < len(data) and not used[position]:
            position += 1
        raw = data[start:position]
        unclaimed.append({"offset": start, "length": len(raw), "sha256": hashlib.sha256(raw).hexdigest(),
                          "raw_hex": raw.hex(), "classification": "uninterpreted_original_bytes"})
    records = sorted(views.values(), key=lambda record: (record["offset"], record["source_relative_offset"], record["id"]))
    anomaly_kinds = {"renderer_rejected_pair", "unmapped_numeric_glyph", "not_rendered_after_rejected_pair"}
    return {"schema": "tokimemo.bunka-sd-tables.v1", "profile": profile,
            "source": {"path": profile["path"], "sha256": digest, "size": len(data), "load_address": base},
            "physical_records": sorted(physical.values(), key=lambda record: (record["source"]["offset"], record["id"])),
            "records": records, "tables": tables, "code_proofs": code_proofs,
            "issues": issues, "unclaimed_ranges": unclaimed,
            "coverage": {"tables": len(tables), "pointer_slots": sum(table["count"] for table in tables),
                         "physical_records": len(physical), "display_records": len(records),
                         "body_decode_failures": sum(record["text"]["source_decoded_text"] is None for record in records),
                         "renderer_anomaly_records": [record["id"] for record in records if any(token["kind"] in anomaly_kinds for token in record["text"]["tokens"])],
                         "unrecognized_prefixes": sum(issue["code"] == "unrecognized_control_prefix" for issue in issues),
                         "unclaimed_bytes": sum(span["length"] for span in unclaimed),
                         "whole_file_text_absence_proven": False},
            "limits": profile["limits"]}

BUNKA_SD_PROFILE = {'id': 'bunka-sd-source-prefix-streams',
 'path': 'CDROM/EXEDIR/BUNKA_SD.EXN',
 'sha256': 'a6929b157190c8e016da76e43f96a1389f67b04afc543c9d6de6252aa9ea4a2b',
 'size': 196608,
 'load_address': 0x80132000,
 'tables': [{'id': 'D_8013B74C',
             'label': 'D_8013B74C',
             'address': 0x8013b74c,
             'count': 18,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'intro',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['D_80122EAC == 0'],
             'evidence': ['src/ovl/BUNKA_SD/80132000.c:27-63,583-653']},
            {'id': 'D_8013B794',
             'label': 'D_8013B794',
             'address': 0x8013b794,
             'count': 7,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'intro',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['D_80122EAC == 1'],
             'evidence': ['src/ovl/BUNKA_SD/80132000.c:27-63,583-653'],
             'voice': {'label': 'D_8013B7B0',
                       'address': 0x8013b7b0,
                       'pair_count': 3,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013B670',
             'label': 'D_8013B670',
             'address': 0x8013b670,
             'count': 17,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'intro',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['D_80122EAC == 2'],
             'evidence': ['src/ovl/BUNKA_SD/80132000.c:27-63,583-653'],
             'voice': {'label': 'D_8013B6B4',
                       'address': 0x8013b6b4,
                       'pair_count': 6,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013B6E4',
             'label': 'D_8013B6E4',
             'address': 0x8013b6e4,
             'count': 14,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'intro',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['D_80122EAC == 3'],
             'evidence': ['src/ovl/BUNKA_SD/80132000.c:27-63,583-653'],
             'voice': {'label': 'D_8013B71C',
                       'address': 0x8013b71c,
                       'pair_count': 6,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013B7C8',
             'label': 'D_8013B7C8',
             'address': 0x8013b7c8,
             'count': 5,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'intro',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['D_80122EAC not in [0,1,2,3]'],
             'evidence': ['src/ovl/BUNKA_SD/80132000.c:27-63,583-653'],
             'voice': {'label': 'D_8013B7DC',
                       'address': 0x8013b7dc,
                       'pair_count': 2,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013B810',
             'label': 'D_8013B810',
             'address': 0x8013b810,
             'count': 74,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'scene',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['source scene setup selected this root; execution and index reachability are not '
                            'proved'],
             'evidence': ['src/ovl/BUNKA_SD/80133540.c:20-23,208-263,478-531'],
             'voice': {'label': 'D_8013B938',
                       'address': 0x8013b938,
                       'pair_count': 29,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013C750',
             'label': 'D_8013C750',
             'address': 0x8013c750,
             'count': 25,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'scene',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['source scene setup selected this root; execution and index reachability are not '
                            'proved'],
             'evidence': ['src/ovl/BUNKA_SD/801369D0.c:20-23,252-324,408-458'],
             'voice': {'label': 'D_8013C7B4',
                       'address': 0x8013c7b4,
                       'pair_count': 12,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013CEF0',
             'label': 'D_8013CEF0',
             'address': 0x8013cef0,
             'count': 54,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 1,
             'mode': 'prefixed',
             'handler_family': 'scene',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 100,
             'conditions': ['source scene setup selected this root; execution and index reachability are not '
                            'proved'],
             'evidence': ['src/ovl/BUNKA_SD/80137B40.c:20-23,235-289,469-517'],
             'voice': {'label': 'D_8013CFC8',
                       'address': 0x8013cfc8,
                       'pair_count': 31,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013BA20',
             'label': 'D_8013BA20',
             'address': 0x8013ba20,
             'count': 29,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 0,
             'mode': 'direct',
             'handler_family': 'direct',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 29,
             'conditions': ['source scene setup selected this root; execution and index reachability are not '
                            'proved'],
             'evidence': ['src/ovl/BUNKA_SD/80133540.c:20-23,208-263,478-531']},
            {'id': 'D_8013C814',
             'label': 'D_8013C814',
             'address': 0x8013c814,
             'count': 12,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 0,
             'mode': 'direct',
             'handler_family': 'direct',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 12,
             'conditions': ['source scene setup selected this root; execution and index reachability are not '
                            'proved'],
             'evidence': ['src/ovl/BUNKA_SD/801369D0.c:20-23,252-324,408-458']},
            {'id': 'D_8013D0C0',
             'label': 'D_8013D0C0',
             'address': 0x8013d0c0,
             'count': 31,
             'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
             'kind': 'pointer_strings',
             'body_offset': 0,
             'mode': 'direct',
             'handler_family': 'direct',
             'role': 'display',
             'renderer': 'set_kanji_string',
             'source_index_bound': 31,
             'conditions': ['source scene setup selected this root; execution and index reachability are not '
                            'proved'],
             'evidence': ['src/ovl/BUNKA_SD/80137B40.c:20-23,235-289,469-517']}],
 'data_assemblies': ['asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
                     'asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata.s',
                     'asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata.s',
                     'asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata.s',
                     'asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata.s'],
 'code_assemblies': ['asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80132000/func_80132148.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80132000/func_80133204.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_80133540.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_80133D54.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_80133FA4.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_801341F0.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_801369D0.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_8013731C.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_801373D0.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_80137620.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_80137824.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_80137B40.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_8013842C.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_8013867C.s',
                     'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_80138860.s'],
 'source_pins': {'include/ovl/BUNKA_SD.h': '1ce7ac2a9710552450a0485fdf5f989c0ee991f7038025ff7914952a35a073a3',
                 'src/ovl/BUNKA_SD/80132000.c': 'c9cfe66aeabdc97bf4f3a060ff5ca0a0143eb5e30d6bc72b9927f979f69b6f8c',
                 'src/ovl/BUNKA_SD/80133540.c': 'e0055c2058afcdba24ae99968eff005b9e5a13a4c80f2b676a34470c9e2ddcc0',
                 'src/ovl/BUNKA_SD/801369D0.c': '49fb95ce566e331c684d5942e43f590b49020d2f57276310f3973455acd5f1ee',
                 'src/ovl/BUNKA_SD/80137B40.c': 'a3ceee96cee41ffb706ffe831e10daaa0618849be958b1457cd5c206ab75de87',
                 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s': '1eccd3b274e6d45942ad2386093c1682cfae11418919326173dc83935d31106e',
                 'asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata.s': 'c1097cd2d973a735efb9375905eaf8557bef38a484ba52de1065a6519598dbf3',
                 'asm/ovl/BUNKA_SD/data/BUNKA_SD/80133540.rodata.s': 'c10700954d8266789663c21a49ff47debd9765cf517d76237fb76143b57b0806',
                 'asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata.s': '7d34cd1064ae8bdca8ab8ce52c071d925732642653cd907d7d157d76364ec9fc',
                 'asm/ovl/BUNKA_SD/data/BUNKA_SD/80137B40.rodata.s': '2762ca17a759c5a363b6bfe6047d7718e164eaa59c062ce789169880d728f553',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80132000/func_80132148.s': '6a5ba0d6298d2fa452ec335051f84c16a11c444254bd4d449c18f15f41b57be2',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80132000/func_80133204.s': '1264c23ba9538edd78a9941548d2e7669c05232a0783e9bcc0d02293b1ebdbc2',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_80133540.s': '344234673e0de9f308704f69c24c1be7a634ace32a1987fefdf3c54f77578fb4',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_80133D54.s': '4414cd56ace5526cfd699dd800e17329dce4626b497ca73e40f4add8e2ddf5f6',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_80133FA4.s': 'e75958633dbe2eb47d1b3a4fb52dd4d1b7f257cacade989f6e0b9b00b124d3a3',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80133540/func_801341F0.s': '80b5761ad21b70c8795960f74f5a059469b0e839759570ed041859e1284961fe',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_801369D0.s': 'c1c6e91f77397dbf861ddef8cd52c81ea1b56d2ce14436baba72040ee4e5d044',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_8013731C.s': 'ead47bcaacda8021974b1ee76e96ba7e1a9d9d3fc9c7520c87d0a0b3d1fff062',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_801373D0.s': '0e8aeb97dd0768ea8e0841d0fa6cb252527b9abafc5d32c075b7ec8b9d31999a',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_80137620.s': 'c5f18deef02e898a933a1b580d61c310e86d72b917882333f712fbc57f847bd4',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/801369D0/func_80137824.s': 'febf57fb06b419d0ffc670df76f8dc956c235cb806688d74ccf2233bd9352959',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_80137B40.s': 'f03447c93ced12950be44a6b2e16a704ff6ddb529ea8e3fddb64fb9c4cef7d19',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_8013842C.s': '92cbc501d3be7461ae3ebcd28facbb8661f05afb8dfcfb82c694b3ac9cd89abc',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_8013867C.s': '91262323034d13add10003079c09d225cc3c1ff86f49aeacaf6e74a6c6fa4299',
                 'asm/ovl/BUNKA_SD/matchings/BUNKA_SD/80137B40/func_80138860.s': '978a7afed7c9b05fead96fb3503d8bad858619851e888f8b0e28c2c29869df68'},
 'limits': ['normal source-selected initialized tables only; no execution or runtime reachability is inferred',
            'voice counters and state conditions remain symbolic; no runtime memory values are read',
            'unclaimed bytes are preserved as uninterpreted ranges, not proved nontext',
            'original counts/coordinates describe provenance, not future translation layout limits']}
