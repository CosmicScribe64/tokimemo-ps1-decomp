"""Bounded OPTION.NBN actor-frame and glyph export from the source loader.

The overlay reads OPTION.NBN at LBA 0x9EA4 into 0x801B0000, whose source image
is 0x1E800 bytes (61 sectors). OPTION actors use the four-entry texture-page
table at file offset 0x132C8 and 130 SpriteFrame pointers at 0x132D8. Only
source-backed 8bpp pages and CLUT rows are rendered; other data remains
unclassified in the input manifest.
"""
from __future__ import annotations

import hashlib
import re
import struct
from pathlib import Path

from .images import ImageFormatError, _color, _png

OPTION_SIZE = 0x1E800
LOAD_ADDRESS = 0x801B0000
TPAGE_TABLE_OFFSET = 0x132C8
TPAGE_COUNT = 4
FRAME_TABLE_OFFSET = 0x132D8
FRAME_COUNT = 130
SEQUENCE_TABLE_OFFSET = 0x134E0
GLYPH_PAGE_BYTES = 0x4000
BACKGROUND_PAGE_OFFSET = 0x17CF0

# OPTION's loader publishes four 256-color source CLUTs at rows 0x1F0..0x1F3,
# a cycling set at rows 0x1FA..0x1FD, and the source block at row 0x1FF.
CLUT_SOURCES = {**{row: 0x134F0 + (row - 0x1F0) * 0x200 for row in range(0x1F0, 0x1F4)},
                **{row: 0x1BEF0 + (row - 0x1FA) * 0x200 for row in range(0x1FA, 0x1FE)},
                0x1FF: 0x0000}


def _fail(message: str) -> None:
    raise ImageFormatError(message)


def _parse(data: bytes) -> tuple[list[dict], list[dict], list[dict], list[int]]:
    if len(data) != OPTION_SIZE:
        _fail("OPTION profile requires exactly 0x1E800 source bytes")
    page_addresses = struct.unpack_from("<%dI" % TPAGE_COUNT, data, TPAGE_TABLE_OFFSET)
    pages = []
    for index, address in enumerate(page_addresses):
        offset = address - LOAD_ADDRESS
        if address < LOAD_ADDRESS or offset + GLYPH_PAGE_BYTES > len(data):
            _fail("OPTION texture-page pointer %d is outside the source image" % index)
        pages.append({"page_index": index, "source_address": address, "source_offset": offset,
                      "source_size": GLYPH_PAGE_BYTES,
                      "source_sha256": hashlib.sha256(data[offset:offset + GLYPH_PAGE_BYTES]).hexdigest(),
                      "coverage": "full uploaded 8bpp page extent; referenced texel coverage is partial/unknown"})

    frames = []
    extents: dict[int, tuple[int, int]] = {}
    addresses = struct.unpack_from("<%dI" % FRAME_COUNT, data, FRAME_TABLE_OFFSET)
    reserved = ((TPAGE_TABLE_OFFSET, FRAME_TABLE_OFFSET),
                (FRAME_TABLE_OFFSET, FRAME_TABLE_OFFSET + FRAME_COUNT * 4),
                (SEQUENCE_TABLE_OFFSET, SEQUENCE_TABLE_OFFSET + TPAGE_COUNT * 4))
    for index, address in enumerate(addresses):
        offset = address - LOAD_ADDRESS
        if address < LOAD_ADDRESS or offset + 8 > len(data):
            _fail("OPTION frame pointer %d is outside the source image" % index)
        count, origin_x, origin_y, flags = struct.unpack_from("<4h", data, offset)
        if not 0 <= count <= 64 or offset + 8 + count * 14 > len(data):
            _fail("OPTION frame %d has an invalid SpriteFrame extent" % index)
        end = offset + 8 + count * 14
        if any(offset < reserved_end and end > reserved_start
               for reserved_start, reserved_end in reserved):
            _fail("OPTION frame %d overlaps a known pointer table" % index)
        if offset in extents and extents[offset] != (offset, end):
            _fail("OPTION aliased frame pointer has inconsistent extent")
        extents[offset] = (offset, end)
        parts = []
        for part_index in range(count):
            record_offset = offset + 8 + part_index * 14
            x, y, texture, clut, flip, width, height = struct.unpack_from("<2h3H2h", data, record_offset)
            page_index = texture >> 8
            u, v = (texture & 0xF) * 8, ((texture >> 4) & 0xF) * 8
            clut_row = 0x1F0 + ((clut & 0xF00) >> 8)
            if (page_index >= len(pages) or flip & ~3 or not 0 < width <= 128 or not 0 < height <= 128
                    or u + width > 128 or v + height > 128):
                _fail("OPTION frame %d part %d has unsupported 8bpp geometry" % (index, part_index))
            palette_offset = CLUT_SOURCES.get(clut_row)
            parts.append({"part_index": part_index, "source_record_offset": record_offset,
                          "x": x, "y": y, "texture_info": texture, "clut_info": clut,
                          "flip_flags": flip, "width": width, "height": height,
                          "page_index": page_index, "source_sheet_offset": pages[page_index]["source_offset"],
                          "u": u, "v": v, "clut_row": clut_row,
                          "palette_source_offset": palette_offset,
                          "palette_status": "loader-established source CLUT" if palette_offset is not None
                          else "unsupported CLUT row"})
        if parts:
            left, top = min(p["x"] for p in parts), min(p["y"] for p in parts)
            width = max(p["x"] + p["width"] for p in parts) - left
            height = max(p["y"] + p["height"] for p in parts) - top
            if width > 640 or height > 512:
                _fail("OPTION composed frame exceeds preview bounds")
        else:
            left = top = width = height = 0
        frames.append({"frame_index": index, "source_frame_offset": offset,
                       "source_frame_address": address, "record_size": end - offset,
                       "origin": [origin_x, origin_y], "frame_flags": flags,
                       "parts": parts, "bounds": [left, top, width, height]})

    ordered = sorted(extents.values())
    for previous, current in zip(ordered, ordered[1:]):
        if current[0] < previous[1]:
            _fail("OPTION SpriteFrame records overlap")
    sequence_header = list(struct.unpack_from("<4I", data, SEQUENCE_TABLE_OFFSET))
    return pages, frames, [{"offset": start, "size": end - start,
                            "sha256": hashlib.sha256(data[start:end]).hexdigest()}
                           for start, end in ordered], sequence_header


def _uninterpreted_extents(data: bytes, known: list[tuple[int, int, str]]) -> list[dict]:
    """Return exact source-byte gaps outside enumerated tables, pages, CLUTs and frames."""
    ordered = sorted((start, end, label) for start, end, label in known if end > start)
    merged = []
    for start, end, _label in ordered:
        if start < 0 or end > len(data):
            _fail("known OPTION source extent is outside the source image")
        if merged and start <= merged[-1][1]:
            merged[-1] = (merged[-1][0], max(merged[-1][1], end))
        else:
            merged.append((start, end))
    gaps = []
    cursor = 0
    for start, end in merged:
        if start > cursor:
            gaps.append({"offset": cursor, "size": start - cursor,
                         "sha256": hashlib.sha256(data[cursor:start]).hexdigest(),
                         "status": "outside enumerated source pages, palettes, pointer tables, and parsed SpriteFrames"})
        cursor = max(cursor, end)
    if cursor < len(data):
        gaps.append({"offset": cursor, "size": len(data) - cursor,
                     "sha256": hashlib.sha256(data[cursor:]).hexdigest(),
                     "status": "outside enumerated source pages, palettes, pointer tables, and parsed SpriteFrames"})
    return gaps


def probe_option_ui(data: bytes) -> dict:
    """Validate the OPTION profile and report its known frame/page counts."""
    try:
        pages, frames, _extents, sequence_header = _parse(data)
    except ImageFormatError as error:
        return {"status": "invalid", "format": "tokimemo-option-ui", "reason": str(error)}
    return {"status": "supported", "format": "tokimemo-option-ui",
            "metadata": {"texture_pages": len(pages), "frames": len(frames),
                         "parts": sum(len(frame["parts"]) for frame in frames),
                         "sequence_table_prefix_words": sequence_header}}


def export_option_ui(data: bytes, output_dir, asset_id: str) -> dict:
    """Export source-backed OPTION frame previews; never translates or redraws them."""
    pages, frames, frame_extents, sequence_header = _parse(data)
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,159}", asset_id):
        _fail("asset_id must be a safe single filename component")
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    files, images, unresolved = [], [], []
    palettes: dict[int, list[int]] = {}
    for frame in frames:
        if not frame["parts"]:
            unresolved.append({"frame_index": frame["frame_index"], "reason": "empty frame record"})
            continue
        missing = [part for part in frame["parts"] if part["palette_source_offset"] is None]
        if missing:
            unresolved.append({"frame_index": frame["frame_index"],
                               "reason": "part uses a CLUT row without a source loader mapping",
                               "parts": [part["part_index"] for part in missing]})
            continue
        left, top, width, height = frame["bounds"]
        canvas = [0] * (width * height)
        rendered_parts = []
        for part in reversed(frame["parts"]):
            offset = part["palette_source_offset"]
            if offset not in palettes:
                palettes[offset] = list(struct.unpack_from("<256H", data, offset))
            words = palettes[offset]
            page = part["source_sheet_offset"]
            for y in range(part["height"]):
                sy = part["height"] - 1 - y if part["flip_flags"] & 1 else y
                for x in range(part["width"]):
                    sx = part["width"] - 1 - x if part["flip_flags"] & 2 else x
                    pixel_offset = page + (part["v"] + sy) * 128 + part["u"] + sx
                    word = words[data[pixel_offset]]
                    if word:
                        canvas[(part["y"] - top + y) * width + part["x"] - left + x] = word
            rendered_parts.append(dict(part, source_palette_words=words,
                                       preview_palette_words=words,
                                       palette_mode=1))
        name = "%s.frame-%03d.png" % (asset_id, frame["frame_index"])
        stp_name = "%s.frame-%03d.stp.png" % (asset_id, frame["frame_index"])
        color_bytes = b"".join(_color(word) for word in canvas)
        stp_bytes = bytes(255 if word & 0x8000 else 0 for word in canvas)
        with (output_dir / name).open("xb") as output:
            output.write(_png(width, height, color_bytes, 4))
        with (output_dir / stp_name).open("xb") as output:
            output.write(_png(width, height, stp_bytes, 1))
        files.extend((name, stp_name))
        images.append({"png": name, "stp_png": stp_name, "frame_index": frame["frame_index"],
                       "variant": "source-frame", "preview_kind": "static_frame_composite",
                       "width": width, "height": height, "region": {"x": 0, "y": 0,
                       "width": width, "height": height}, "parts": list(reversed(rendered_parts)),
                       "render_policy": "source SpriteFrame parts and loader-established CLUT rows; no translation or replacement art"})
    palette_extents = [{"row": row, "offset": offset, "size": 0x200,
                        "sha256": hashlib.sha256(data[offset:offset + 0x200]).hexdigest(),
                        "evidence": "src/ovl/OPTION/settings_sound_test_and_extras.c:load_palette"}
                       for row, offset in sorted(CLUT_SOURCES.items())]
    background_page = {"source_offset": BACKGROUND_PAGE_OFFSET, "source_address": LOAD_ADDRESS + BACKGROUND_PAGE_OFFSET,
                       "source_size": GLYPH_PAGE_BYTES,
                       "source_sha256": hashlib.sha256(data[BACKGROUND_PAGE_OFFSET:BACKGROUND_PAGE_OFFSET + GLYPH_PAGE_BYTES]).hexdigest(),
                       "evidence": "src/ovl/OPTION/settings_sound_test_and_extras.c:load_tpage_buf_lock(5)",
                       "coverage": "full uploaded 8bpp page extent; frame part usage not established"}
    known_extents = [(page["source_offset"], page["source_offset"] + page["source_size"], "actor texture page")
                     for page in pages]
    known_extents.append((background_page["source_offset"], background_page["source_offset"] + background_page["source_size"],
                          "direct texture page upload"))
    known_extents.extend((item["offset"], item["offset"] + item["size"], "palette") for item in palette_extents)
    known_extents.extend((item["offset"], item["offset"] + item["size"], "frame record") for item in frame_extents)
    known_extents.extend(((TPAGE_TABLE_OFFSET, TPAGE_TABLE_OFFSET + TPAGE_COUNT * 4, "texture page pointer table"),
                          (FRAME_TABLE_OFFSET, FRAME_TABLE_OFFSET + FRAME_COUNT * 4, "frame pointer table"),
                          (SEQUENCE_TABLE_OFFSET, SEQUENCE_TABLE_OFFSET + TPAGE_COUNT * 4, "sequence table prefix")))
    frame_record_end = frame_extents[-1]["offset"] + frame_extents[-1]["size"]
    pre_table_gap = ({"offset": frame_record_end, "size": TPAGE_TABLE_OFFSET - frame_record_end,
                      "sha256": hashlib.sha256(data[frame_record_end:TPAGE_TABLE_OFFSET]).hexdigest(),
                      "status": "source bytes after the final referenced frame record and before the texture-page pointer table; uninterpreted"}
                     if frame_record_end <= TPAGE_TABLE_OFFSET else None)
    metadata = {"source_sha256": hashlib.sha256(data).hexdigest(), "source_size": len(data),
                "source_path_profile": "CDROM/FUKUTEST/OPTION.NBN", "source_lba": 0x9EA4,
                "runtime_load_address": LOAD_ADDRESS, "bits_per_pixel": 8,
                "texture_page_table_offset": TPAGE_TABLE_OFFSET,
                "frame_table_offset": FRAME_TABLE_OFFSET, "frame_count": len(frames),
                "sequence_table_offset": SEQUENCE_TABLE_OFFSET,
                "sequence_table_prefix_words": sequence_header,
                "texture_pages": pages, "frame_record_extents": frame_extents,
                "frame_record_unreferenced_gaps": [
                    {"offset": previous["offset"] + previous["size"],
                     "size": current["offset"] - (previous["offset"] + previous["size"]),
                     "sha256": hashlib.sha256(data[previous["offset"] + previous["size"]:current["offset"]]).hexdigest(),
                     "status": "bytes between referenced SpriteFrame records; uninterpreted"}
                    for previous, current in zip(frame_extents, frame_extents[1:])
                    if current["offset"] > previous["offset"] + previous["size"]],
                "frame_table_padding": pre_table_gap,
                "palette_source_extents": palette_extents,
                "direct_texture_page_upload": background_page,
                "uninterpreted_extents": _uninterpreted_extents(data, known_extents),
                "alpha_policy": "BGR555 word zero transparent; all other words opaque",
                "source_palette_policy": "source and preview words are identical; palette queue mode 1 performs no STP rewrite",
                "stp_policy": "separate grayscale bit-15 mask for every rendered frame",
                "unresolved_frames": unresolved,
                "round_trip_verified": False,
                "limitations": ["base CLUT bytes at source offset 0x0 precede page-zero texture at 0x200",
                                "dynamic UI palette cycling and actor-specific overrides are not rendered"]}
    return {"status": "partial" if unresolved else "exported", "format": "tokimemo-option-ui",
            "files": files, "images": images, "metadata": metadata}
