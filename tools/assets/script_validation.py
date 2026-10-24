"""Independent structural checks for explicitly profiled scripts (T-28001).

These checks verify supplied roots and extents, not discovery or runtime
reachability. Source initializers and consumers remain the evidence for roots.
No report contains dialogue, raw text bytes, or decoded strings.
"""
from __future__ import annotations

import struct

from .text import parse_script, sha256

INTERPRETATION_EVIDENCE = {
    "cursor": "src/main/8007C030.c:2034-2046",
    "choices": "src/main/8007C030.c:1498-1517",
    "reply_voice": "src/main/8007C030.c:1519-1564",
    "renderer_stepping": "src/main/8007C030.c:1635-1652",
    "glyph_rejection": "src/main/8004E500.c:332-345",
    "cd_marker": "src/main/800451D0.c:324",
    "cd_checksum": "src/main/800451D0.c:511-538",
}


def checked_cd_trailer(data: bytes) -> dict:
    """Inspect the marker and byte sum used by the two distinct CD checks.

    cd_read_sectors checks the marker; file_checksum_ok separately checks the
    sum. A sum mismatch therefore does not imply the ordinary loader rejects it.
    """
    if len(data) < 4 or len(data) % 2048:
        return {"status": "not_sector_sized"}
    stored = int.from_bytes(data[-4:-2], "big")
    computed = sum(data[:-4]) & 0xFFFF
    return {"offset": len(data) - 4, "length": 4,
            "marker_present": data[-2:] == b"\x12\x10",
            "stored_sum": stored, "computed_sum": computed,
            "checksum_matches": stored == computed,
            "marker_evidence": INTERPRETATION_EVIDENCE["cd_marker"],
            "checksum_evidence": INTERPRETATION_EVIDENCE["cd_checksum"]}


def _inside_pointer(data, base, offset):
    return (0 <= offset <= len(data) - 4 and
            base <= struct.unpack_from("<I", data, offset)[0] < base + len(data))


def validate_script(data: bytes, path: str, profile: dict, parsed=None) -> dict:
    """Check bounded root tables, record fidelity, menus and voice extents.

    Reports missing/overlapping roots without interpreting unclaimed bytes as
    text. Shared nested text/voice data is allowed. Nonstandard explicit-group
    layouts must supply their own root-boundary evidence.
    """
    parsed = parse_script(data, path, profile) if parsed is None else parsed
    base = profile["load_address"]
    issues = []
    root_spans = []
    boundary_checks = []
    expected_groups = set()
    known_root_starts = {section[key] - base for section in profile["sections"]
                         for key in ("text_table", "command_table", "voice_table") if key in section}
    for section in profile["sections"]:
        sid = section["id"]
        count = section["group_count"]
        expected_groups.update((sid, group) for group in range(count))
        if not all(key in section for key in ("text_table", "command_table", "voice_table")):
            issues.append({"code": "root_boundary_requires_explicit_evidence", "section": sid})
            continue
        for kind in ("text", "command", "voice"):
            offset = section[kind + "_table"] - base
            root_spans.append((offset, offset + count * 4, sid, kind))
            valid = all(_inside_pointer(data, base, offset + i * 4) for i in range(count))
            next_is_pointer = _inside_pointer(data, base, offset + count * 4)
            next_is_root = offset + count * 4 in known_root_starts
            boundary_checks.append({"section": sid, "kind": kind, "offset": offset,
                                    "count": count, "all_pointers_in_input": valid,
                                    "next_word_is_input_pointer": next_is_pointer,
                                    "next_slot_is_known_root": next_is_root})
            if not valid:
                issues.append({"code": "root_pointer_outside_input", "section": sid, "kind": kind})
            if kind != "voice" and next_is_pointer and not next_is_root:
                issues.append({"code": "possible_omitted_group", "section": sid, "kind": kind,
                               "next_slot_offset": offset + count * 4})
    for i, left in enumerate(root_spans):
        for right in root_spans[i + 1:]:
            if max(left[0], right[0]) < min(left[1], right[1]):
                issues.append({"code": "overlapping_root_extents",
                               "left": {"section": left[2], "kind": left[3]},
                               "right": {"section": right[2], "kind": right[3]}})
    if parsed["source"]["sha256"] != sha256(data):
        issues.append({"code": "parsed_source_hash_mismatch"})
    ids = set()
    group_records = {}
    menus = []
    reply_counts = {}
    renderer_anomalies = []
    strict_source_failures = 0
    for record in parsed["records"]:
        rid = record["id"]
        if rid in ids:
            issues.append({"code": "duplicate_record_id", "record_id": rid})
        ids.add(rid)
        group_records.setdefault((record["section"], record["group"]), []).append(record)
        text = record.get("text")
        if text is None:
            continue
        raw = bytes.fromhex(text["raw_hex"])
        if data[text["offset"]:text["offset"] + len(raw) + 1] != raw + b"\0":
            issues.append({"code": "text_source_extent_mismatch", "record_id": rid})
        token_bytes = b"".join(bytes.fromhex(t["raw_hex"]) for t in text["tokens"])
        if token_bytes != raw:
            issues.append({"code": "token_bytes_not_lossless", "record_id": rid})
        try:
            decoded = raw.decode("shift_jis", errors="strict")
            if decoded.encode("shift_jis", errors="strict") != raw:
                raise UnicodeError("source does not roundtrip")
        except UnicodeError:
            strict_source_failures += 1
        unknown = [t for t in text["tokens"] if t["kind"] == "unknown"]
        if unknown:
            renderer_anomalies.append({"record_id": rid, "text_offset": text["offset"],
                                       "unknown_tokens": len(unknown),
                                       "tab_offsets": [text["offset"] + i for i, byte in enumerate(raw)
                                                       if byte == 9]})
        if "choice" in text:
            choice = text["choice"]
            source_rows = choice["option_count"] if raw else 0
            menus.append({"record_id": rid, "source_rows": source_rows,
                          "selectable_options": choice["expected_count"],
                          "empty_source": not raw,
                          "counts_differ": source_rows != choice["expected_count"]})
        if "reply_variant_count" in text:
            count = text["reply_variant_count"]
            reply_counts[count] = reply_counts.get(count, 0) + 1
            if len(record.get("voice", {}).get("pairs", [])) != count:
                issues.append({"code": "reply_voice_extent_mismatch", "record_id": rid})
    if set(group_records) != expected_groups:
        issues.append({"code": "profile_record_groups_mismatch"})
    for (sid, group), records in group_records.items():
        if ([r["line"] for r in records] != list(range(len(records))) or
                records[-1]["command"]["value"] != 255 or
                any(r["command"]["value"] == 255 for r in records[:-1])):
            issues.append({"code": "noncontiguous_or_unterminated_group", "section": sid, "group": group})
    return {"schema": "tokimemo.text.validation.v1", "source_path": path,
            "source_sha256": sha256(data), "profile_id": profile["id"],
            "issues": issues, "structural_checks_pass": not issues,
            "root_boundaries": boundary_checks,
            "groups": len(group_records), "records": len(parsed["records"]),
            "menus": menus, "reply_variant_histogram": reply_counts,
            "strict_source_decode_failures": strict_source_failures,
            "renderer_anomalies": renderer_anomalies,
            "cd_trailer": checked_cd_trailer(data),
            "source_evidence": profile.get("evidence", []),
            "interpretation_evidence": INTERPRETATION_EVIDENCE,
            "limits": ["does_not_prove_runtime_reachability", "does_not_discover_unprofiled_roots",
                       "does_not_execute_runtime_text_mutations"]}
