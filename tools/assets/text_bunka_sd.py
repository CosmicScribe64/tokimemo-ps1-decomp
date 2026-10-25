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
             'evidence': ['src/ovl/culture_stage_shows/literature_speech_contest.c:27-63,583-653']},
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
             'evidence': ['src/ovl/culture_stage_shows/literature_speech_contest.c:27-63,583-653'],
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
             'evidence': ['src/ovl/culture_stage_shows/literature_speech_contest.c:27-63,583-653'],
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
             'evidence': ['src/ovl/culture_stage_shows/literature_speech_contest.c:27-63,583-653'],
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
             'evidence': ['src/ovl/culture_stage_shows/literature_speech_contest.c:27-63,583-653'],
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
             'evidence': ['src/ovl/culture_stage_shows/drama_first_year_show.c:20-23,208-263,478-531'],
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
             'evidence': ['src/ovl/culture_stage_shows/drama_final_year_show.c:20-23,252-324,408-458'],
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
             'evidence': ['src/ovl/culture_stage_shows/drama_second_year_show.c:20-23,235-289,469-517'],
             'voice': {'label': 'D_8013CFC8',
                       'address': 0x8013cfc8,
                       'pair_count': 31,
                       'assembly_path': 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s'}},
            {'id': 'D_8013BA20',
             'label': 'stage_show_scene_a_text_script_addresses',
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
             'evidence': ['src/ovl/culture_stage_shows/drama_first_year_show.c:20-23,208-263,478-531']},
            {'id': 'D_8013C814',
             'label': 'stage_show_scene_b_text_script_addresses',
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
             'evidence': ['src/ovl/culture_stage_shows/drama_final_year_show.c:20-23,252-324,408-458']},
            {'id': 'D_8013D0C0',
             'label': 'stage_show_scene_c_text_script_addresses',
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
             'evidence': ['src/ovl/culture_stage_shows/drama_second_year_show.c:20-23,235-289,469-517']}],
 'data_assemblies': ['asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s',
                     'asm/ovl/BUNKA_SD/data/culture_stage_shows/literature_speech_contest.rodata.s',
                     'asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_first_year_show.rodata.s',
                     'asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata.s',
                     'asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_second_year_show.rodata.s'],
 'code_assemblies': ['asm/ovl/BUNKA_SD/matchings/culture_stage_shows/literature_speech_contest/stage_show_script_select.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/literature_speech_contest/stage_show_script_line_render.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_scene_a_data_load.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_event_list_run.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_drama_first_show_event_list_init.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_drama_first_show_actor_slot_clear.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_scene_b_data_load.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_drama_final_event_list_step.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_scene_b_event_list_run.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_drama_final_event_text_and_audio_display.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_drama_final_kanji_line_start.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_scene_c_data_load.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_scene_c_event_list_run.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_drama_second_event_text_and_audio_display.s',
                     'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_drama_second_kanji_line_start.s'],
 'source_pins': {'include/ovl/culture_stage_shows.h': '839080e79e207945378480c6ae024c6a89028781e47156c4d0c2796d22e2a75a',
                 'src/ovl/culture_stage_shows/literature_speech_contest.c': '91d93e7503f7dd9f1ed072a464ce6ee6768ce37e6c4b3f0daa23b56b9813b8fd',
                 'src/ovl/culture_stage_shows/drama_first_year_show.c': '40f375016f88de8d870c48d69e449b0e763903d08ac6ccb4aad2ff3aedd77cca',
                 'src/ovl/culture_stage_shows/drama_final_year_show.c': 'f2151e112469b0dc2642041fe51a5f389c21472aa1333ff511b2cccc7acd4e9a',
                 'src/ovl/culture_stage_shows/drama_second_year_show.c': '16704ebe28752be56fe8790b2545ab4b18b21668055b0a7f41b7e5c4518a0caa',
                 'asm/ovl/BUNKA_SD/data/BUNKA_SD_data.data.s': 'cae048c3b0a208f6052d3679d9eb1e8e4a528e6e693ccba0857bcc0443176388',
                 'asm/ovl/BUNKA_SD/data/culture_stage_shows/literature_speech_contest.rodata.s': 'c1097cd2d973a735efb9375905eaf8557bef38a484ba52de1065a6519598dbf3',
                 'asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_first_year_show.rodata.s': 'c10700954d8266789663c21a49ff47debd9765cf517d76237fb76143b57b0806',
                 'asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_final_year_show.rodata.s': '7d34cd1064ae8bdca8ab8ce52c071d925732642653cd907d7d157d76364ec9fc',
                 'asm/ovl/BUNKA_SD/data/culture_stage_shows/drama_second_year_show.rodata.s': '2762ca17a759c5a363b6bfe6047d7718e164eaa59c062ce789169880d728f553',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/literature_speech_contest/stage_show_script_select.s': '8bb802b10e30f15a152008cddf3a6da5d470bab4fb5da764d8e45f6ca17ac461',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/literature_speech_contest/stage_show_script_line_render.s': '24abc200e503efbc100b00161b8fed4e2b32443c02048d299174cd6e706e9b07',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_scene_a_data_load.s': 'c196685e83f1278bf5f94d134f05859af468ced9baa0ca071a36ca663171f2a9',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_event_list_run.s': 'bf89dbab1ca9482fae8a98362051c4b3ffee1af8eb7e63b3a5c5e102c5c91d9e',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_drama_first_show_event_list_init.s': '8eab32b8339601df094d9ed931d6589b1de473f42733e891c0bedcd1cff7b6b2',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_first_year_show/stage_show_drama_first_show_actor_slot_clear.s': '6a83cc7de2881793439aac51bebd0e99f986e0b1a80f5189e507a072e94399d4',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_scene_b_data_load.s': 'aa28cc781e88c11339bea56f64ba0c2a5c7e60a5093ccf720339ecbeac4dd9d3',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_drama_final_event_list_step.s': 'aeadf964345d7cb01b3e25d98a0caafa051d71e4101ad49819168b4111949e33',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_scene_b_event_list_run.s': 'c139b27b8e99c96527b9795263ba072de504cf008decd9eeee9adecf742fd47d',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_drama_final_event_text_and_audio_display.s': '9deafb4ba03c242ff37d84fc7b29d678ebc0fc96bf2381dfff26e0e6eee40abc',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_final_year_show/stage_show_drama_final_kanji_line_start.s': 'c46caa6e2b5e2442121b283ff9b65f2ad88db82c4acd8c189b4ae1d4c78f749c',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_scene_c_data_load.s': 'b84d2e6c89ab923045c60f08a9d857fcf9a84719d6d5f6debf05befc11abafcf',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_scene_c_event_list_run.s': '15d0748cd9f229adf3ab92fbacaa06026eadd4484e3b7f58b240f0ef05dd90df',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_drama_second_event_text_and_audio_display.s': '5c71c3d05db0692079fc51fcc53574b66e5b553cea226800897648111d72c7d7',
                 'asm/ovl/BUNKA_SD/matchings/culture_stage_shows/drama_second_year_show/stage_show_drama_second_kanji_line_start.s': '3f7e68487ef0525ce2ecf7dbbe1abbdeb653081343dc78812e70844b4767913b'},
 'limits': ['normal source-selected initialized tables only; no execution or runtime reachability is inferred',
            'voice counters and state conditions remain symbolic; no runtime memory values are read',
            'unclaimed bytes are preserved as uninterpreted ranges, not proved nontext',
            'original counts/coordinates describe provenance, not future translation layout limits']}
