"""Source-bounded O_01.NBN actor-96 frame parser and static sprite previews.

This profile is intentionally specific to the original 13-sector stage-0 NBN.
It interprets only the five source-linked SpriteFrame records, one sequence,
one 4bpp texture page and the CLUT row used by their part records. It does not
claim which animation frame is selected at runtime or compose a full scene.
"""
from __future__ import annotations

import hashlib
import struct

from .images import ImageFormatError, _color, _png


SOURCE_PATH = "CDROM/PROLODIR/O_01.NBN"
SOURCE_LBA = 0xABAB
SOURCE_SIZE = 0x6800
SOURCE_SHA256 = "bb224a47fa1ac9c102dd4701d3d3e0819ba30cac1e5637183886b8be4f95340c"
LOAD_BASE = 0x80180000
TPAGE_POINTER_OFFSET = 0xA8
FRAME_POINTER_OFFSET = 0xAC
SEQUENCE_POINTER_OFFSET = 0xC0
SEQUENCE_OFFSET = 0x90
PAGE_OFFSET = 0xD0
PAGE_SIZE = 0x4000
PAGE_WIDTH = 256
PAGE_HEIGHT = 128
PALETTE_OFFSET = 0x4490
PALETTE_WORDS = 16
FRAME_POINTERS = (0, 0x18, 0x30, 0x48, 0x6C)
SEQUENCE_FRAME_INDICES = (0, 1, 2, 3, 4)
SEQUENCE_DURATIONS = (3, 3, 3, 1, 1)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _u32(data: bytes, offset: int) -> int:
    if offset < 0 or offset + 4 > len(data):
        raise ImageFormatError("O_01 pointer word is out of bounds")
    return struct.unpack_from("<I", data, offset)[0]


def _layout(data: bytes) -> dict:
    if len(data) != SOURCE_SIZE or _sha(data) != SOURCE_SHA256:
        raise ImageFormatError("O_01 profile requires the exact original source bytes")

    tpage_vma = _u32(data, TPAGE_POINTER_OFFSET)
    sequence_vma = _u32(data, SEQUENCE_POINTER_OFFSET)
    if tpage_vma != LOAD_BASE + PAGE_OFFSET or sequence_vma != LOAD_BASE + SEQUENCE_OFFSET:
        raise ImageFormatError("O_01 texture/sequence pointers differ from the source profile")

    table_size = SEQUENCE_POINTER_OFFSET - FRAME_POINTER_OFFSET
    if table_size % 4:
        raise ImageFormatError("O_01 frame pointer table is not word aligned")
    frame_vmas = tuple(_u32(data, FRAME_POINTER_OFFSET + offset)
                       for offset in range(0, table_size, 4))
    frame_offsets = tuple(pointer - LOAD_BASE for pointer in frame_vmas)
    if frame_offsets != FRAME_POINTERS:
        raise ImageFormatError("O_01 frame pointer rows differ from the source profile")

    sequence_count = struct.unpack_from("<h", data, SEQUENCE_OFFSET)[0]
    if sequence_count != len(FRAME_POINTERS):
        raise ImageFormatError("O_01 sequence frame count differs from its frame table")
    sequence_bytes = 2 * (1 + sequence_count * 2)
    sequence_end = SEQUENCE_OFFSET + sequence_bytes
    if sequence_end > TPAGE_POINTER_OFFSET:
        raise ImageFormatError("O_01 sequence extent overlaps the tpage pointer")
    sequence_values = struct.unpack_from("<" + "h" * (1 + sequence_count * 2), data,
                                         SEQUENCE_OFFSET)
    sequence_rows = tuple((sequence_values[1 + 2 * index], sequence_values[2 + 2 * index])
                          for index in range(sequence_count))
    if tuple(row[0] for row in sequence_rows) != SEQUENCE_FRAME_INDICES or \
            tuple(row[1] for row in sequence_rows) != SEQUENCE_DURATIONS or \
            any(duration <= 0 for _, duration in sequence_rows):
        raise ImageFormatError("O_01 sequence rows differ from the source-linked actor sequence")

    frames = []
    for index, offset in enumerate(frame_offsets):
        if offset + 8 > SEQUENCE_OFFSET:
            raise ImageFormatError("O_01 frame header overlaps sequence data")
        count, origin_x, origin_y, flags = struct.unpack_from("<4h", data, offset)
        if not 0 <= count <= 8:
            raise ImageFormatError("O_01 frame part count exceeds SpriteFrame capacity")
        end = offset + 8 + count * 14
        next_limit = frame_offsets[index + 1] if index + 1 < len(frame_offsets) else SEQUENCE_OFFSET
        if end > next_limit:
            raise ImageFormatError("O_01 SpriteFrame parts overlap the next source table")
        parts = []
        for part_index in range(count):
            record = offset + 8 + part_index * 14
            x, y, texture, palette_info, flip, width, height = struct.unpack_from(
                "<2h3H2h", data, record)
            page_index = texture >> 6
            u = (texture & 7) * 16
            v = ((texture >> 3) & 7) * 16
            if page_index != 0 or flip & ~3 or not 0 < width <= PAGE_WIDTH or \
                    not 0 < height <= PAGE_HEIGHT or u + width > PAGE_WIDTH or \
                    v + height > PAGE_HEIGHT:
                raise ImageFormatError("O_01 part is outside the one source-proved 4bpp page")
            if palette_info != 0x1000:
                raise ImageFormatError("O_01 part palette selector differs from the loaded row")
            parts.append({"part_index": part_index, "source_record_offset": record,
                          "x": x, "y": y, "texture_id": texture,
                          "texture_page_index": page_index, "u": u, "v": v,
                          "palette_info": palette_info, "flip_flags": flip,
                          "width": width, "height": height})
        if parts:
            left = min(part["x"] for part in parts)
            top = min(part["y"] for part in parts)
            right = max(part["x"] + part["width"] for part in parts)
            bottom = max(part["y"] + part["height"] for part in parts)
        else:
            left = top = right = bottom = 0
        if right - left > 640 or bottom - top > 512:
            raise ImageFormatError("O_01 preview frame exceeds the bounded sprite canvas")
        frames.append({"frame_index": index, "source_frame_offset": offset,
                       "source_frame_size": end - offset, "origin": [origin_x, origin_y],
                       "frame_flags": flags, "parts": parts,
                       "bounds": [left, top, right - left, bottom - top]})

    palette_end = PALETTE_OFFSET + PALETTE_WORDS * 2
    page_end = PAGE_OFFSET + PAGE_SIZE
    if page_end > PALETTE_OFFSET or palette_end > len(data):
        raise ImageFormatError("O_01 source page/CLUT extents overlap or exceed the input")

    return {"source_size": len(data), "source_sha256": _sha(data),
            "load_base": LOAD_BASE, "tpage_pointer_offset": TPAGE_POINTER_OFFSET,
            "texture_page_offset": PAGE_OFFSET, "texture_page_size": PAGE_SIZE,
            "texture_page_dimensions": [PAGE_WIDTH, PAGE_HEIGHT],
            "texture_bits_per_pixel": 4,
            "frame_pointer_table_offset": FRAME_POINTER_OFFSET,
            "frame_pointer_table_size": table_size,
            "frame_offsets": list(frame_offsets),
            "sequence_pointer_offset": SEQUENCE_POINTER_OFFSET,
            "sequence_offset": SEQUENCE_OFFSET, "sequence_size": sequence_bytes,
            "sequence_rows": [{"frame_index": frame, "duration_ticks": duration}
                              for frame, duration in sequence_rows],
            "palette_source_offset": PALETTE_OFFSET,
            "palette_source_size": PALETTE_WORDS * 2,
            "palette_clut_vram": [0, 0x1F0], "palette_mode": 2,
            "palette_selection_condition": "Actor.unk_05 bit 7 set with SpritePart palette_info 0x1000; prologue initializes the actor record's byte at +5 to 0x80",
            "frames": frames,
            "opaque_gaps": [
                {"offset": frame_offsets[i] + frames[i]["source_frame_size"],
                 "size": frame_offsets[i + 1] -
                         (frame_offsets[i] + frames[i]["source_frame_size"])}
                for i in range(len(frames) - 1)
                if frame_offsets[i] + frames[i]["source_frame_size"] < frame_offsets[i + 1]
            ] + ([{"offset": frames[-1]["source_frame_offset"] + frames[-1]["source_frame_size"],
                   "size": SEQUENCE_OFFSET - (frames[-1]["source_frame_offset"] +
                                              frames[-1]["source_frame_size"])}]
                 if frames[-1]["source_frame_offset"] + frames[-1]["source_frame_size"] <
                    SEQUENCE_OFFSET else [])}


def parse_o01(data: bytes) -> dict:
    """Return numeric source layout for the pinned O_01 actor file."""
    return _layout(data)


def render_o01_frames(data: bytes) -> tuple[dict, dict[str, bytes]]:
    """Render the five linked static source frames using the source CLUT row."""
    layout = _layout(data)
    page = data[PAGE_OFFSET:PAGE_OFFSET + PAGE_SIZE]
    source_palette = struct.unpack_from("<16H", data, PALETTE_OFFSET)
    # The native mode-2 upload clears STP for its first 240 CLUT words. This
    # exported row is entirely inside that range.
    palette = tuple(word & 0x7FFF for word in source_palette)
    outputs: dict[str, bytes] = {}
    image_rows = []
    for frame in layout["frames"]:
        left, top, width, height = frame["bounds"]
        if width == 0 or height == 0:
            continue
        rgba = bytearray(width * height * 4)
        stp = bytearray(width * height)
        for part in reversed(frame["parts"]):
            for dy in range(part["height"]):
                source_y = part["height"] - dy - 1 if part["flip_flags"] & 1 else dy
                for dx in range(part["width"]):
                    source_x = part["width"] - dx - 1 if part["flip_flags"] & 2 else dx
                    px = part["u"] + source_x
                    py = part["v"] + source_y
                    packed = page[py * (PAGE_WIDTH // 2) + (px >> 1)]
                    color_index = (packed >> (4 * (px & 1))) & 0xF
                    color = palette[color_index]
                    rgba_index = ((part["y"] - top + dy) * width +
                                  (part["x"] - left + dx)) * 4
                    rgba[rgba_index:rgba_index + 4] = _color(color)
                    stp[rgba_index // 4] = 255 if color & 0x8000 else 0
        stem = "frame-%02d" % frame["frame_index"]
        png_name, stp_name = "actor96.%s.png" % stem, "actor96.%s.stp.png" % stem
        png_data = _png(width, height, bytes(rgba), 4)
        stp_data = _png(width, height, bytes(stp), 1)
        outputs[png_name] = png_data
        outputs[stp_name] = stp_data
        image_rows.append({"frame_index": frame["frame_index"], "png": png_name,
                           "stp_png": stp_name, "width": width, "height": height,
                           "parts": len(frame["parts"]),
                           "source_frame_offset": frame["source_frame_offset"],
                           "composition": "static source SpriteFrame parts with reversed actor_draw_all iteration order"})
    layout["rendered_images"] = image_rows
    layout["alpha_policy"] = "source BGR555 zero entries are transparent, matching the existing asset preview convention"
    layout["stp_policy"] = "CLUT row 0x1F0 comes from native palette mode 2; first 16 entries have STP cleared"
    layout["actor_record_address"] = 0x80120650
    layout["actor_record_alias_proof"] = {
        "actor_pool_base": 0x8011ECD0, "actor_pool_index": 96,
        "record_size": 0x44, "config_binding": "config/symbol_addrs_main.txt actor_pool_96",
    }
    layout["actor_stage_state"] = {
        "stage0_draw_flags": 0x24,
        "stage0_visible": False,
        "stage0_visibility_rule": "actor_draw_all requires draw_flags bit 0x80",
        "post_hold_draw_flags": 0xA4,
        "post_hold_visible": True,
        "first_visible_after_stage0_hold": True,
        "post_hold_activation": "prologue_arrival_line ORs 0x80 after its 0x3D-tick hold",
        "initial_actor_flags": 1,
        "initial_sequence_slot": 0,
        "initial_frame_index": 0,
        "initial_sequence_counter": 0,
        "sequence_counter_period": 2,
        "sequence_counter_update": "actor_draw_all increments the counter when frame_count % unk_06 == 0; prologue sets unk_06=2",
        "sequence_terminal_behavior": "flags bit 0x01 is set; absent terminal bits 0x02/0x04/0x08/0x10, actor_draw_all resets frame to 0",
    }
    layout["runtime_frame_selection"] = "first visible actor frame is source sequence step 0/frame 0; all five linked static frames are exported"
    layout["pixel_composition_scope"] = "source frame parts, texture indices and CLUT colors only; actor tint, GPU blend, screen placement, brightness, and whole-scene composition are not applied"
    return layout, outputs
