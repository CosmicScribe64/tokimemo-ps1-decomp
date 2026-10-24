"""Read source-backed external text tables outside the talk containers (T-28001).

NAME_KAN: NAME_ENT/80132000.c loads six sectors at LBA 0x9DBF to
0x801E0000, relocates 50 offsets at +0x2B2C, and indexes 45 character
lists using u16 counts at the start of the file. The lists are glyph
selection data, not dialogue and not bitmap OCR.
"""
from __future__ import annotations

import struct

from .text import TextFormatError, sha256
from .text_fixed import tokenize_fixed_text
from .script_validation import checked_cd_trailer

NAME_KAN_PROFILE = {
    "id": "name-entry-candidates", "path": "CDROM/FUKUTEST/NAME_KAN.NBN",
    "sha256": "29c22ef10985a33a93acb2fef4a6bb6942c1b73e6aff7f4ea974a265213039a9",
    "load_address": 0x801E0000, "counts_offset": 0, "counts_count": 48,
    "offsets_offset": 0x2B2C, "offsets_count": 50, "list_count": 45,
    "evidence": ["src/ovl/NAME_ENT/80132000.c:184,199-201,706-713,818-825,1043-1077",
                 "include/ovl/NAME_ENT.h:D_8014CE64,D_8014CE68",
                 "asm/ovl/NAME_ENT/data/NAME_ENT/80132000.data.s:D_8014CE64,D_8014CE68"],
}


def parse_character_lists(data: bytes, profile=None) -> dict:
    """Read exact counted glyph lists and preserve relocation/table metadata."""
    profile = NAME_KAN_PROFILE if profile is None else profile
    digest = sha256(data)
    if profile.get("sha256") and digest != profile["sha256"]:
        raise TextFormatError("character-list source differs from verified layout")
    co, cc = profile["counts_offset"], profile["counts_count"]
    po, pc = profile["offsets_offset"], profile["offsets_count"]
    count = profile["list_count"]
    if co < 0 or po < 0 or co + cc * 2 > len(data) or po + pc * 4 > len(data) or not 0 < count <= min(cc, pc):
        raise TextFormatError("character-list tables outside source")
    counts = struct.unpack_from("<" + "H" * cc, data, co)
    offsets = struct.unpack_from("<" + "I" * pc, data, po)
    records = []
    spans = []
    used = bytearray(len(data))
    used[co:co + cc * 2] = b"\x01" * (cc * 2)
    used[po:po + pc * 4] = b"\x01" * (pc * 4)
    for index in range(count):
        start, length = offsets[index], counts[index] * 2
        if start < co + cc * 2 or start + length + 2 > po:
            raise TextFormatError("character list outside its source data region")
        if data[start + length:start + length + 2] != b"\0\0":
            raise TextFormatError("counted character list lacks its u16 terminator")
        if any(max(start, a) < min(start + length + 2, b) for a, b in spans):
            raise TextFormatError("character list extents overlap")
        spans.append((start, start + length + 2))
        used[start:start + length + 2] = b"\x01" * (length + 2)
        raw = data[start:start + length]
        view = tokenize_fixed_text(raw, "load_string")
        if view["source_decoded_text"] is None or len(view["source_decoded_text"]) != counts[index]:
            raise TextFormatError("character list is not exact full-width Shift-JIS")
        records.append({"id": f"character-list:{profile['path']}:{index:02d}",
                        "source_path": profile["path"], "source_sha256": digest,
                        "index": index, "offset": start, "length": length, "count": counts[index],
                        "count_offset": co + index * 2, "pointer_offset": po + index * 4,
                        "pointer_encoding": "relative_to_load_address", "pointer_value": start,
                        "terminator_hex": "0000", "text": view})
    trailer = checked_cd_trailer(data)
    if trailer.get("marker_present"):
        used[-4:] = b"\x01" * 4
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
        unclaimed.append({"offset": start, "length": len(raw), "raw_hex": raw.hex(),
                          "sha256": sha256(raw), "classification": "zero_padding" if not any(raw) else "uninterpreted"})
    return {"schema": "tokimemo.character-lists.v1", "profile": profile,
            "source": {"path": profile["path"], "sha256": digest, "size": len(data)},
            "counts": list(counts), "offsets": list(offsets),
            "records": records, "cd_trailer": trailer, "unclaimed_ranges": unclaimed,
            "coverage": {"lists": len(records), "glyph_entries": sum(counts[:count]),
                         "unreferenced_count_slots": list(range(count, cc)),
                         "unreferenced_pointer_slots": list(range(count, pc))}}

# Original addresses/counts are established by the named pointer arrays and
# consumers in main_api/NAME_ENT. Fixed-width presets use the source's % 15 and
# 8/8/12-byte strncpy strides, not string scanning.
DICTIONARY_PROFILES = [
    {"id": "main-terms", "path": "SLPM_86.053",
     "sha1": "e823bd844a8f8fa4d05483b59c66bc54b8393b26",
     "load_address": 0x80041000, "header_size": 0x800,
     "tables": [
         {"id": "parameter_names", "address": 0x800B331C, "count": 9, "kind": "pointer_strings"},
         {"id": "place_names", "address": 0x800B342C, "count": 21, "kind": "pointer_strings"},
         {"id": "girl_full_names", "address": 0x800B3538, "count": 16, "kind": "pointer_strings"},
         {"id": "girl_short_names", "address": 0x800B35F4, "count": 16, "kind": "pointer_strings"},
         {"id": "club_names", "address": 0x800B3908, "count": 12, "kind": "pointer_strings"},
     ],
     "evidence": ["include/main_api.h:param_name_table,place_name_table,girl_full_name_table,girl_short_name_table,club_name_table",
                  "config/symbol_addrs.txt", "config/symbol_addrs_main.txt",
                  "src/main/8004F870.c:get_g_name,get_p_name"]},
    {"id": "name-entry-terms", "path": "CDROM/EXEDIR/NAME_ENT.EXN",
     "sha1": "e9a9a0f059dc0029e292363ecf09cffb4068dad9",
     "load_address": 0x80132000, "header_size": 0,
     "tables": [
         {"id": "hiragana_grid", "address": 0x8014CD28, "count": 17, "kind": "pointer_strings"},
         {"id": "katakana_grid", "address": 0x8014CD6C, "count": 17, "kind": "pointer_strings"},
         {"id": "reading_labels", "address": 0x8014CDB0, "count": 45, "kind": "pointer_strings"},
         {"id": "preset_family_names", "address": 0x8014CE6C, "count": 15, "kind": "fixed_strings", "stride": 8},
         {"id": "preset_given_names", "address": 0x8014CEE4, "count": 15, "kind": "fixed_strings", "stride": 8},
         {"id": "preset_nicknames", "address": 0x8014CF5C, "count": 15, "kind": "fixed_strings", "stride": 12},
     ],
     "evidence": ["include/ovl/NAME_ENT.h:NameKanaPages44,D_8014CDB0",
                  "src/ovl/NAME_ENT/80132000.c:func_80132000",
                  "src/ovl/NAME_ENT/80132000.c:1015-1077"]},
]


def parse_dictionary(data: bytes, profile: dict) -> dict:
    """Read explicit pointer/fixed-width term tables; retain padding and slots."""
    import hashlib
    if profile.get("sha1") and hashlib.sha1(data).hexdigest() != profile["sha1"]:
        raise TextFormatError("dictionary source differs from verified layout")
    digest = sha256(data)
    base, bias = profile["load_address"], profile["header_size"]
    records = []
    for table in profile["tables"]:
        if table["kind"] not in ("pointer_strings", "fixed_strings"):
            raise TextFormatError("unsupported dictionary slot kind")
        root = table["address"] - base + bias
        stride = 4 if table["kind"] == "pointer_strings" else table["stride"]
        if root < 0 or root + table["count"] * stride > len(data):
            raise TextFormatError("dictionary table outside source")
        for index in range(table["count"]):
            slot = root + index * stride
            record = {"id": f"dictionary:{profile['path']}:{table['id']}:{index:02d}",
                      "table": table["id"], "index": index, "source_path": profile["path"],
                      "source_sha256": digest, "slot_offset": slot}
            if table["kind"] == "pointer_strings":
                pointer = struct.unpack_from("<I", data, slot)[0]
                start = pointer - base + bias
                if not 0 <= start < len(data):
                    raise TextFormatError("dictionary string pointer outside source")
                end = data.find(b"\0", start, min(start + 65536, len(data)))
                if end < 0:
                    raise TextFormatError("dictionary string lacks terminator")
                record.update(pointer_value=pointer, slot_raw_hex=data[slot:slot + 4].hex())
            elif table["kind"] == "fixed_strings":
                start = slot
                end = data.find(b"\0", start, start + stride)
                if end < 0:
                    raise TextFormatError("fixed dictionary slot lacks terminator")
                record.update(stride=stride, slot_raw_hex=data[slot:slot + stride].hex(),
                              padding_raw_hex=data[end + 1:slot + stride].hex())
            else:
                raise TextFormatError("unsupported dictionary slot kind")
            raw = data[start:end]
            view = tokenize_fixed_text(raw, "load_string")
            if view["source_decoded_text"] is None:
                raise TextFormatError("dictionary has unsupported source encoding")
            record.update(offset=start, length=len(raw), text=view)
            records.append(record)
    return {"schema": "tokimemo.dictionary.v1", "profile": profile,
            "source_sha256": digest, "records": records,
            "coverage": {"tables": len(profile["tables"]), "records": len(records)}}
