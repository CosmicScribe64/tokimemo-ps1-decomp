"""Source-backed MAINROM visual-review context, without making text decisions.

This module describes runtime owners and palette-selection evidence for the
known MAINROM frame table. It deliberately leaves raw, unmapped source sheets
uncolored and every preview unreviewed.
"""
from __future__ import annotations

import re
from pathlib import PurePosixPath
from typing import Any

from .image_packs import OVERRIDES, TEXTURE_OFFSETS
from .visual_text import CatalogError, _stable_region_id
from .visual_text_profiles import get_profile

MEDIA_SCHEMA = "tokimemo.main-ui-evidence.v1"
MAINROM_PATH = "CDROM/FUKUTEST/MAINROM.NBN"
UNMAPPED_OFFSETS = (0x2000, 0x6000, 0xC000, 0x10000, 0x14000)
OVERRIDE_CONTEXTS = {
    "hud-43": ["parameter digits whose actor requests CLUT row 0x43",
               "weekday label whose actor requests CLUT row 0x43"],
    "hud-44": ["parameter digits whose actor requests CLUT row 0x44",
               "weekday label whose actor requests CLUT row 0x44"],
    "icon-51": ["disabled command-icon plate CLUT row 0x51"],
    "icon-52": ["hovered command-icon plate CLUT row 0x52"],
    "icon-53": ["normal command-icon plate CLUT row 0x53"],
}
EVIDENCE = [
    "src/main/boot_assets.c:boot_data_load_step,boot_hud_textures_load (MAINROM read and 10 immediate texture-page locks)",
    "src/main/boot_assets.c:boot_hud_init (palette initialization and STP edits)",
    "src/main/actor_sprites.c:actor_draw_all (frame/sequence, texture and CLUT selection)",
    "src/main/hud_panels.c:parameter_show_init,hizuke_init,message_window_init,hud_icon_bar_init",
    "wiki/spec/h-graphics.md:Actor pool, image lookup and palette selection",
    "wiki/spec/m-hud.md:sections 5-8 (date, parameter, message and command icon actors)",
]
_SHA256 = re.compile(r"^[0-9a-f]{64}$")
_SHEET_OFFSETS = set(range(0, 0x1A000, 0x2000))


def _fail(message: str) -> None:
    raise CatalogError(message)


def _cache_path(value: Any) -> str:
    if not isinstance(value, str) or "\\" in value:
        _fail("PNG output path must be a relative POSIX cache path")
    path = PurePosixPath(value)
    if path.is_absolute() or ".." in path.parts or len(path.parts) < 3 or path.parts[0:2] != (".asset-cache", "media"):
        _fail("PNG output path must stay under .asset-cache/media/")
    return value


def _palette_binding(variant: dict) -> dict:
    name = variant.get("variant", "base")
    parts = variant.get("parts") or []
    if name == "base":
        bindings = [{"part_index": p.get("part_index"),
                     "palette_exe_address": p.get("palette_exe_address"),
                     "palette_exe_offset": p.get("palette_exe_offset"),
                     "clut_vram": p.get("clut_vram"),
                     "source_palette_words": p.get("source_palette_words"),
                     "status": "source-exact frame-part palette"}
                    for p in parts]
        return {"status": "source-default-per-part" if bindings else "unknown",
                "parts": bindings,
                "runtime_palette_override_possible": bool(bindings),
                "note": "actor clut_row can replace the frame-part default CLUT"}
    override = OVERRIDES.get(name)
    if override is None:
        return {"status": "unknown", "parts": []}
    address, clut_x, clut_y = override
    frame = variant.get("frame_index")
    if name in {"hud-43", "hud-44"}:
        actor_contexts = (["date-panel weekday actor 23"] if frame is not None and 0x52 <= frame <= 0x58 else [])
        if frame is not None and 0x44 <= frame <= 0x4D:
            actor_contexts.append("date/parameter digit actors requesting CLUT row 0x%02X" %
                                  (0x43 if name == "hud-43" else 0x44))
    else:
        actor_contexts = (["command-icon plate actor slots 64-78; target frame is 2"]
                          if frame == 2 else [])
    return {"status": "known palette override candidate",
            "palette_exe_address": address, "clut_vram": [clut_x, clut_y],
            "part_palette_data": [{"part_index": part.get("part_index"),
                                   "palette_exe_address": part.get("palette_exe_address"),
                                   "palette_exe_offset": part.get("palette_exe_offset"),
                                   "clut_vram": part.get("clut_vram"),
                                   "source_palette_words": part.get("source_palette_words"),
                                   "preview_palette_words": part.get("preview_palette_words")}
                                  for part in parts],
            "runtime_context_candidates": actor_contexts,
            "palette_role": OVERRIDE_CONTEXTS[name],
            "applicability": "candidate; source proves this CLUT is loaded and these actors can request its row; no visual state is inferred"}


def _frame_usage(frame: int | None, sequences: list[dict]) -> dict:
    direct = []
    if frame == 0x3C:
        direct.append({"actors": [16], "role": "date-panel label"})
    elif frame == 0x3D:
        direct.append({"actors": [59], "role": "parameter-panel label"})
    elif frame == 0x3E:
        direct.append({"actors": [1], "role": "message-window frame"})
    elif frame is not None and 0x44 <= frame <= 0x4D:
        direct.extend([
            {"actors": list(range(17, 23)), "role": "date-panel decimal digit; value/position selects use"},
            {"actors": list(range(32, 59)), "role": "parameter-panel decimal digit; value selects use"},
        ])
    elif frame is not None and 0x52 <= frame <= 0x58:
        direct.append({"actors": [23], "role": "date-panel weekday label"})
    elif frame == 2:
        direct.append({"actors": list(range(64, 79)), "role": "command-icon plate"})
    seq_refs = []
    if frame is not None:
        for sequence in sequences:
            for step_index, step in enumerate(sequence.get("steps", [])):
                if step.get("frame_index") == frame:
                    index = sequence.get("sequence_index")
                    actor_slot = (94 if index == 15 else 93 if index in (13, 14)
                                  else 80 + index if isinstance(index, int) and 0 <= index <= 12
                                  else None)
                    seq_refs.append({"sequence_index": index,
                                     "actor_slot": actor_slot,
                                     "sequence_step": step_index,
                                     "duration": step.get("duration"),
                                     "role": "command-icon glyph sequence"})
    role = "unassigned frame-table image"
    if direct or seq_refs:
        role = "source-proved actor/frame-table use"
    return {"status": "source-backed" if direct or seq_refs else "unknown",
            "role": role, "direct_actor_contexts": direct,
            "sequence_references": seq_refs,
            "evidence": EVIDENCE[2:5] if direct or seq_refs else []}


def build_main_ui_context(media_manifest: dict[str, Any]) -> dict[str, Any]:
    """Create a separate context/todo report; never changes catalog review states."""
    if not isinstance(media_manifest, dict) or media_manifest.get("schema") != "tokimemo.media.v1":
        _fail("unsupported media manifest schema")
    assets = media_manifest.get("assets")
    if not isinstance(assets, list):
        _fail("media manifest assets must be a list")
    matches = []
    for asset in assets:
        if not isinstance(asset, dict):
            _fail("media asset record must be an object")
        source = asset.get("source") or {}
        if source.get("path") == MAINROM_PATH:
            matches.append(asset)
    if len(matches) != 1:
        _fail("expected exactly one MAINROM media record")
    asset = matches[0]
    source = asset.get("source") or {}
    export = asset.get("export") or {}
    metadata = export.get("metadata") or {}
    if asset.get("status") not in {"exported", "partial"} or asset.get("format") != "tokimemo-main-ui":
        _fail("MAINROM source is not an exported source-profile asset")
    if metadata.get("source_path_profile") != MAINROM_PATH:
        _fail("MAINROM export metadata does not identify the source profile")
    if metadata.get("source_lba") != 0x9D85:
        _fail("MAINROM export LBA differs from the source-backed profile")
    asset_id = asset.get("id")
    if not isinstance(asset_id, str) or not asset_id:
        _fail("MAINROM asset id is required for stable region provenance")
    source_hash = source.get("sha256")
    if not isinstance(source_hash, str) or not _SHA256.fullmatch(source_hash):
        _fail("MAINROM source SHA-256 is required")
    if metadata.get("source_sha256") != source_hash:
        _fail("MAINROM export source hash differs from source record")
    if source.get("size") != metadata.get("source_size"):
        _fail("MAINROM export source size differs from source record")
    paired = asset.get("paired_source")
    paired_hash = metadata.get("paired_exe_sha256")
    if (not isinstance(paired, dict) or not isinstance(paired_hash, str)
            or not _SHA256.fullmatch(paired_hash) or paired.get("sha256") != paired_hash):
        _fail("paired executable source hash is missing or inconsistent")
    if not isinstance(paired.get("path"), str) or not paired["path"]:
        _fail("paired executable source path is required")
    if paired.get("path") != "SLPM_86.053" or paired.get("size") != metadata.get("paired_exe_size"):
        _fail("paired executable path or size differs from the source-profile metadata")
    images = export.get("images")
    sequences = metadata.get("sequences")
    if not isinstance(images, list) or not isinstance(sequences, list):
        _fail("MAINROM image and sequence inventories are required")
    if any(not isinstance(sequence, dict) or not isinstance(sequence.get("steps"), list)
           or any(not isinstance(step, dict) for step in sequence["steps"]) for sequence in sequences):
        _fail("MAINROM sequence records must contain step objects")
    output_by_name = {}
    output_paths = set()
    for output in asset.get("outputs") or []:
        if not isinstance(output, dict):
            _fail("MAINROM output metadata must be an object")
        output_path = _cache_path(output.get("path"))
        if output_path in output_paths:
            _fail("duplicate MAINROM output path")
        output_paths.add(output_path)
        name = PurePosixPath(output_path).name
        if name in output_by_name:
            _fail("duplicate MAINROM output basename")
        if not isinstance(output.get("sha256"), str) or not _SHA256.fullmatch(output["sha256"]):
            _fail("MAINROM output SHA-256 is invalid")
        output_by_name[name] = output
    image_records, todo = [], []
    region_ids, image_names, diagnostic_by_offset = set(), set(), {}
    for image in images:
        if not isinstance(image, dict):
            _fail("MAINROM image record must be an object")
        name = image.get("png")
        if not isinstance(name, str) or PurePosixPath(name).name != name or "\\" in name:
            _fail("MAINROM image PNG name must be a plain filename")
        if name in image_names:
            _fail("duplicate MAINROM image PNG")
        image_names.add(name)
        output = output_by_name.get(name)
        if not output or not isinstance(output.get("sha256"), str):
            _fail("MAINROM PNG and its hash must be present in outputs")
        frame = image.get("frame_index")
        if frame is not None and (not isinstance(frame, int) or isinstance(frame, bool) or frame < 0):
            _fail("MAINROM frame index must be a nonnegative integer or null")
        variant_name = image.get("variant", "base")
        if not isinstance(variant_name, str) or not variant_name:
            _fail("MAINROM image variant name is required")
        palette_index = image.get("palette_index")
        if palette_index is not None and (not isinstance(palette_index, int) or isinstance(palette_index, bool)
                                          or palette_index < 0):
            _fail("MAINROM palette index must be a nonnegative integer or null")
        parts = image.get("parts") or []
        if not isinstance(parts, list) or any(not isinstance(part, dict) for part in parts):
            _fail("MAINROM image parts must be a list of objects")
        rect = image.get("region") or image.get("rect") or {}
        if not isinstance(rect, dict):
            _fail("MAINROM image region must be an object")
        region = {"x": rect.get("x", 0), "y": rect.get("y", 0),
                  "width": image.get("width", rect.get("width")),
                  "height": image.get("height", rect.get("height"))}
        if not all(isinstance(value, int) and not isinstance(value, bool) and value >= 0
                   for value in region.values()) or not region["width"] or not region["height"]:
            _fail("MAINROM image region needs positive dimensions and nonnegative origin")
        region_id = _stable_region_id(asset["id"], frame, palette_index, region, variant_name)
        if region_id in region_ids:
            _fail("duplicate MAINROM stable region id")
        region_ids.add(region_id)
        diagnostic = image.get("preview_kind") == "source_index_diagnostic"
        offset = image.get("source_sheet_offset")
        if diagnostic:
            if not isinstance(offset, int) or offset not in UNMAPPED_OFFSETS:
                _fail("unexpected diagnostic sheet offset")
            if offset in diagnostic_by_offset:
                _fail("duplicate MAINROM diagnostic sheet")
            usage = {"status": "unmapped-by-known-frame-table",
                     "role": "MAINROM sheet with no known frame-table part reference",
                     "direct_quad_use": "not established",
                     "palette_binding": "unknown; no source-backed CLUT association",
                     "transparency": "unknown; grayscale indices are diagnostic only",
                     "evidence": [EVIDENCE[0], EVIDENCE[2]]}
        else:
            usage = _frame_usage(frame, sequences)
            profile = get_profile(MAINROM_PATH)
            if frame in profile.get("frame_roles", {}):
                usage["profile_frame_role"] = profile["frame_roles"][frame]
                usage["profile_contexts"] = profile.get("screen_context", [])
                usage["profile_evidence"] = profile.get("evidence", [])
                if usage["status"] == "unknown":
                    usage["status"] = "source-profile-backed"
                    usage["role"] = "source-profile frame role; direct actor slot unresolved"
        record = {"region_id": region_id, "frame_index": frame,
                  "region": region, "variant": variant_name, "png": output["path"],
                  "png_sha256": output["sha256"], "source_sheet_offsets": (
                      [offset] if diagnostic else sorted({
                      p.get("sheet_source_offset") for p in parts
                          if isinstance(p, dict) and isinstance(p.get("sheet_source_offset"), int)})),
                  "usage": usage,
                  "palette_binding": (usage.get("palette_binding") if diagnostic
                                      else _palette_binding(image)),
                  "review": {"state": "unreviewed", "confidence": None,
                             "reason": "context evidence does not classify visible text"}}
        image_records.append(record)
        if diagnostic:
            diagnostic_by_offset[offset] = record
        todo.append({"region_id": region_id, "png": record["png"], "png_sha256": record["png_sha256"],
                     "frame_index": frame, "variant": record["variant"],
                     "review_state": "unreviewed", "required_action": "manual visual review"})
    declared_unmapped = metadata.get("unmapped_sheet_extents") or []
    if not isinstance(declared_unmapped, list):
        _fail("MAINROM unmapped sheet extents must be a list")
    by_offset = {}
    for item in declared_unmapped:
        if not isinstance(item, dict) or not isinstance(item.get("offset"), int):
            _fail("MAINROM unmapped sheet extent needs an integer offset")
        if item["offset"] in by_offset:
            _fail("duplicate MAINROM unmapped sheet extent")
        if not isinstance(item.get("size"), int) or not isinstance(item.get("sha256"), str) or not _SHA256.fullmatch(item["sha256"]):
            _fail("MAINROM unmapped extent size/hash is incomplete")
        by_offset[item["offset"]] = item
    immediate_uploads = {0x6000: 0x1B, 0x10000: 0x3B}
    pointer_indexes = {offset: index for index, offset in enumerate(TEXTURE_OFFSETS)}
    referenced = metadata.get("frame_table_referenced_sheet_offsets")
    if not isinstance(referenced, list) or any(not isinstance(offset, int) for offset in referenced):
        _fail("MAINROM frame-table sheet offsets must be an integer list")
    if len(referenced) != len(set(referenced)) or set(referenced) & set(UNMAPPED_OFFSETS):
        _fail("MAINROM referenced/unmapped sheet offset lists overlap or contain duplicates")
    if set(referenced) | set(UNMAPPED_OFFSETS) != _SHEET_OFFSETS:
        _fail("MAINROM frame-table and unmapped extents do not cover the known source sheets")
    for offset in UNMAPPED_OFFSETS:
        if offset not in by_offset:
            _fail("MAINROM metadata lacks expected unmapped sheet extent 0x%X" % offset)
        if offset not in diagnostic_by_offset:
            _fail("MAINROM image export lacks diagnostic for unmapped sheet 0x%X" % offset)
    if set(by_offset) != set(UNMAPPED_OFFSETS) or set(diagnostic_by_offset) != set(UNMAPPED_OFFSETS):
        _fail("MAINROM export contains unexpected unmapped sheet extents or diagnostics")
    variant_counts = {}
    for image in image_records:
        variant = image["variant"]
        variant_counts[variant] = variant_counts.get(variant, 0) + 1
    return {"schema": MEDIA_SCHEMA, "source": {"asset_id": asset_id, "path": MAINROM_PATH,
            "size": source["size"], "location": source.get("location"),
            "sha256": source_hash, "paired_source": {"path": paired.get("path"),
            "size": paired.get("size"), "sha256": paired_hash}},
            "frame_table_referenced_sheet_offsets": referenced,
            "evidence": EVIDENCE,
            "unmapped_sheets": [{"offset": offset, "size": by_offset[offset].get("size"),
                                 "sha256": by_offset[offset].get("sha256"),
                                 "diagnostic_region_id": diagnostic_by_offset[offset]["region_id"],
                                 "diagnostic_png": diagnostic_by_offset[offset]["png"],
                                 "source_status": by_offset[offset].get("status"),
                                 "texture_pointer_index": pointer_indexes.get(offset),
                                 "extra_texture_pointer_index": {0x6000: 0, 0x10000: 1}.get(offset),
                                 "immediate_boot_upload_tpage": immediate_uploads.get(offset),
                                 "frame_table_use": "no known part references this sheet",
                                 "runtime_use": ("immediate boot upload; no known frame-table reference"
                                                 if offset in immediate_uploads else
                                                 "not in immediate boot-lock list; no known frame-table reference"),
                                 "clut": "unknown", "direct_quad_use": "not established",
                                 "visual_text": "unreviewed"}
                                for offset in UNMAPPED_OFFSETS],
            "images": image_records, "local_review_todo": todo,
            "review_counts": {"unreviewed": len(todo), "classified": 0},
            "variant_counts": variant_counts,
            "completeness": {"corpus_complete": False, "source_coverage_complete": False,
                              "visual_text_review_complete": False,
                              "todo_scope": "all image variants in this export only; excludes unused texels, unexported palettes and other source containers",
                              "mainrom_export_status": asset.get("status"),
                              "emitted_image_variants": len(image_records)},
            "limitations": ["A source-backed actor/context mapping does not classify visible text.",
                            "Palette override previews are candidates; applicability is not implied for every frame.",
                            "Unmapped sheet grayscale indices have no inferred CLUT, transparency or direct-quad use."]}

