"""Relate text code units to original font bitmaps, without image export (T-28001).

Evidence: load_string/text_glyph_expand in src/main/text_system.c and FontRange
in include/main/text.h. This metadata lets a visual catalog share stable glyph
IDs with script records. It does not identify Japanese lettering baked into
unrelated image assets; the media catalog owns those regions.
"""
from __future__ import annotations

import hashlib
import struct

from .text import TextFormatError, sha256


def verify_font_source_proof(data: bytes, repository, files_root, profile=None) -> dict:
    """Replay the current font reader and original range/glyph data evidence."""
    from .text_tables import verify_table_source_proof
    return verify_table_source_proof(data, FONT_PROFILE if profile is None else profile,
                                     repository, files_root)

FONT_PROFILE = {'path': 'SLPM_86.053',
 'sha1': 'e823bd844a8f8fa4d05483b59c66bc54b8393b26',
 'load_address': 2147749888,
 'header_size': 2048,
 'range_address': 2148220972,
 'range_count': 46,
 'glyph_address': 2148232528,
 'glyph_count': 3418,
 'sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
 'evidence': ['src/main/text_system.c::load_string',
              'src/main/text_system.c::text_glyph_expand',
              'include/main/text.h:FontRange'],
 'data_proofs': [{'label': 'text_font_range_table',
                  'address': 2148220972,
                  'length': 184,
                  'assembly_path': 'asm/data/main/text_system.data.s'},
                 {'label': 'text_font_glyphs',
                  'address': 2148232528,
                  'length': 61524,
                  'assembly_path': 'asm/data/data_800B6D30.data.s'}],
 'source_checks': [{'path': 'src/main/text_system.c', 'contains': 'for (i = 1; i < 46; i++)'},
                   {'path': 'src/main/text_system.c', 'contains': 'p = &text_font_glyphs[arg0 * 18];'},
                   {'path': 'include/main/text.h', 'contains': 'u16 first;'},
                   {'path': 'include/main/text.h', 'contains': 'u16 glyph;'},
                   {'path': 'include/main/text.h', 'contains': 'FontRange text_font_range_table[46];'}],
 'code_proofs': [{'assembly_path': 'asm/matchings/main/text_system/load_string.s',
                  'source_path': 'SLPM_86.053',
                  'source_sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
                  'load_address': 2147749888,
                  'header_size': 2048},
                 {'assembly_path': 'asm/matchings/main/text_system/text_glyph_expand.s',
                  'source_path': 'SLPM_86.053',
                  'source_sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
                  'load_address': 2147749888,
                  'header_size': 2048}],
 'source_pins': {'asm/data/data_800B6D30.data.s': 'ca2171c86cc3076aed4c2140ae285ff766c09c6c6683c24277a0e755a19c8cb1',
                 'asm/data/main/text_system.data.s': '9eafc7dd540f7823b7afa43c8b06dc39b9fab41ad7bdf39df1b921e077931510',
                 'asm/matchings/main/text_system/load_string.s': '3b729b79dd57ea83588cd87244efb4490b67e627915d0876f8bb1d977b1cdc53',
                 'asm/matchings/main/text_system/text_glyph_expand.s': '7c974e62255aa7dd6091f6e7aa8324fa045b57204ee6b05532fa234eb9b83317',
                 'config/symbol_addrs.txt': '003c6169775031a58015faebba3d8674754a5d3ed166a9b06f9528055cbc3bfe',
                 'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
                 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
                 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                 'src/main/text_system.c': '9da8802817d460b8e89b90fd86f3be929e9856c55f2e9d24f0e83ea41200a522'}}


def font_metadata(data: bytes, profile=None) -> dict:
    """Read an explicit font layout and retain its exact original source extents."""
    profile = FONT_PROFILE if profile is None else profile
    if profile.get("sha1") and hashlib.sha1(data).hexdigest() != profile["sha1"]:
        raise TextFormatError("font source differs from its verified executable")
    range_offset = profile["range_address"] - profile["load_address"] + profile["header_size"]
    glyph_offset = profile["glyph_address"] - profile["load_address"] + profile["header_size"]
    count = profile["range_count"]
    glyph_size = profile["glyph_count"] * 18
    if count < 2 or range_offset < 0 or range_offset + count * 4 > len(data):
        raise TextFormatError("font range table outside source")
    if glyph_offset < 0 or glyph_offset + glyph_size > len(data):
        raise TextFormatError("font glyph bank outside source")
    ranges = [dict(zip(("first", "glyph"), struct.unpack_from("<HH", data, range_offset + i * 4)))
              for i in range(count)]
    if any(a["first"] >= b["first"] for a, b in zip(ranges, ranges[1:])):
        raise TextFormatError("font range starts must be strictly increasing")
    return {
        "id": "font:" + profile["path"], "source_path": profile["path"],
        "source_sha256": sha256(data), "range_offset": range_offset,
        "range_length": count * 4, "range_raw_hex": data[range_offset:range_offset + count * 4].hex(),
        "ranges": ranges, "glyph_offset": glyph_offset, "glyph_count": profile["glyph_count"],
        "glyph_stride": 18, "glyph_bank_sha256": sha256(data[glyph_offset:glyph_offset + glyph_size]),
        "format": {"encoding": "1bpp_msb_first", "width": 12, "height": 12,
                   "upload_width": 16, "upload_height": 12, "upload_bpp": 4},
        "evidence": profile.get("evidence", []),
    }


def font_reference(raw: bytes, metadata: dict) -> dict:
    """Model the glyph loader's one-unit lookup, including ignored/error cases."""
    if len(raw) == 1:
        return {"status": "terminal_byte_before_nul", "raw_hex": raw.hex()}
    if len(raw) != 2:
        return {"status": "invalid_code_unit_length", "length": len(raw)}
    if 0 in raw:
        return {"status": "string_end", "raw_hex": raw.hex()}
    code = int.from_bytes(raw, "big")
    ranges = metadata["ranges"]
    if code < ranges[0]["first"]:
        return {"status": "renderer_error_and_string_end", "code": code}
    if code >= ranges[-1]["first"]:
        return {"status": "ignored_above_font_sentinel", "code": code}
    selected = next(a for a, b in zip(ranges, ranges[1:]) if a["first"] <= code < b["first"])
    index = code - selected["first"] + selected["glyph"]
    if not 0 <= index < metadata["glyph_count"]:
        return {"status": "renderer_read_outside_font", "code": code, "glyph_index": index}
    return {"status": "glyph", "id": f"{metadata['id']}:g{index:04d}", "glyph_index": index,
            "source_path": metadata["source_path"], "source_sha256": metadata["source_sha256"],
            "offset": metadata["glyph_offset"] + index * 18, "length": 18}


def attach_font_references(catalog: dict, metadata: dict) -> dict:
    """Link rendered code units, including source views and inline labels."""
    catalog["font"] = metadata
    unique = set()

    def reference(raw):
        ref = font_reference(raw, metadata)
        if ref["status"] == "glyph":
            # Bank provenance and stride live once in catalog.font.
            unique.add(ref["id"])
            return {"id": ref["id"], "glyph_index": ref["glyph_index"]}
        return ref

    def visit(node):
        if isinstance(node, dict):
            if node.get("kind") in ("glyph", "unknown", "unmapped_numeric_glyph") and "raw_hex" in node:
                node["font_reference"] = reference(bytes.fromhex(node["raw_hex"]))
            elif node.get("kind") == "inline_label":
                raw = bytes.fromhex(node["label_raw_hex"])
                node["label_font_references"] = [
                    {"relative_byte_offset": offset + 1, **reference(raw[offset:offset + 2])}
                    for offset in range(0, len(raw), 2)]
            for value in tuple(node.values()):
                visit(value)
        elif isinstance(node, list):
            for value in node:
                visit(value)

    visit(catalog)
    catalog["font_link_coverage"] = {"referenced_glyphs": len(unique),
                                     "bitmap_lettering_catalog": "owned_by_media_lane",
                                     "baked_image_text_complete": False}
    return catalog
