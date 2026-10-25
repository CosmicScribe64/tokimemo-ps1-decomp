#!/usr/bin/env python3
"""Build a deterministic source/component coverage census from media manifests.

The census separates source files, decoded previews, source extents and review
states. It does not equate a file suffix or a rendered preview with complete
game-graphic coverage.
"""
from __future__ import annotations

import argparse
from copy import deepcopy
import json
from pathlib import Path
import sys
from typing import Any

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.manifest import output_root, write_manifest  # noqa: E402

SCHEMA = "tokimemo.media-coverage.v1"
_GRAPHIC_SUFFIXES = {".NBN", ".BS", ".TIM"}
_AUDIO_SUFFIXES = {".VH", ".VB", ".VAG", ".SEQ", ".XA", ".STR", ".DA"}
_CODE_SUFFIXES = {".EXN"}
_UI_FORMATS = {"tokimemo-main-ui", "tokimemo-calendar-ui", "tokimemo-portrait",
               "tokimemo-title-ui", "tokimemo-filesel-ui", "tokimemo-option-ui",
               "tokimemo-exam-ui", "tokimemo-name-ui", "tokimemo-sd-animation",
               "tokimemo-sports-ui", "tokimemo-rpg-bat-ui"}
_IMAGE_FORMATS = _UI_FORMATS | {"sony-bs-mdec", "psx-tim"}


class CoverageError(ValueError):
    """Invalid or inconsistent coverage inputs."""


def _records(manifest: dict, name: str) -> list[dict]:
    if not isinstance(manifest, dict) or manifest.get("schema") != "tokimemo.media.v1":
        raise CoverageError(name + " must use tokimemo.media.v1")
    records = manifest.get("assets")
    if not isinstance(records, list) or any(not isinstance(item, dict) for item in records):
        raise CoverageError(name + " assets must be a list of objects")
    return records


def _image_regions(asset: dict) -> list[dict]:
    export = asset.get("export") or {}
    regions = export.get("images") or []
    return regions if isinstance(regions, list) else []


def _profile(path: str, fmt: str, *, decoded: bool = False,
             has_coefficient_census: bool = False) -> dict:
    suffix = Path(path).suffix.upper()
    if fmt in _UI_FORMATS:
        if decoded:
            return {"class": "decoded_graphic_container", "evidence": "explicit source-profile image export"}
        return {"class": "known_graphic_container_unsupported", "evidence": "source-profile format recognized; no exported image regions"}
    if fmt == "sony-bs-mdec":
        if decoded:
            return {"class": "decoded_mdec_background", "evidence": "MDEC background image exported with source-backed caller geometry"}
        if has_coefficient_census:
            return {"class": "known_mdec_background_geometry_unresolved",
                    "evidence": "BS coefficient stream parsed, but no image region was exported because geometry is unresolved"}
        return {"class": "known_graphic_container_unsupported",
                "evidence": "BS MDEC background source; no decoded image region"}
    if fmt == "psx-tim" or suffix == ".TIM":
        if not decoded:
            return {"class": "known_image_format_not_decoded", "evidence": "TIM reader/source suffix recognized; no exported image regions"}
        return {"class": "image_format", "evidence": "recognized PlayStation TIM reader"}
    if suffix in _CODE_SUFFIXES or Path(path).name.upper() == "SLPM_86.053":
        return {"class": "code_or_executable_data", "evidence": "executable/overlay source; not classified as image"}
    if suffix in _AUDIO_SUFFIXES:
        return {"class": "audio_or_stream_source", "evidence": "audio/sequence/stream suffix; not an image count"}
    if suffix == ".TMD":
        return {"class": "known_3d_model_source", "evidence": "TMD model container; model rendering not represented as image assets"}
    if suffix in _GRAPHIC_SUFFIXES:
        descriptions = {".BS": "known MDEC background source; image decoding is unsupported",
                        ".NBN": "probable game texture/container; decode not established",
                        ".TIM": "recognized TIM image format"}
        source_class = "known_graphic_container_unsupported" if suffix == ".BS" else "probable_graphic_bearing_source"
        return {"class": source_class, "evidence": descriptions.get(suffix, "suffix candidate only")}
    if suffix == ".BIN":
        return {"class": "ambiguous_binary_source", "evidence": "BIN suffix alone does not establish image content"}
    return {"class": "other_or_unknown_source", "evidence": "no source-format profile"}


def _source_evidence(path: str, asset: dict) -> dict | None:
    """Attach existing source-backed runtime context when a specific profile exists."""
    try:
        from assets.visual_text_profiles import get_profile
    except ImportError:
        return None
    export = asset.get("export") or {}
    profile = get_profile(path, media_format=asset.get("format") or export.get("format"),
                          metadata=export.get("metadata") or {})
    if not isinstance(profile, dict) or profile.get("source_role") == "graphic_role_unresolved":
        return None
    return {key: profile[key] for key in ("priority", "source_role", "screen_context",
                                          "evidence", "follow_up", "frame_roles") if key in profile}


def _source_components(asset: dict) -> dict:
    """Preserve decoder-reported source geometry without converting it to coverage claims."""
    export = asset.get("export") or {}
    inspection = asset.get("inspection") or {}
    metadata = dict(inspection.get("metadata") or {})
    metadata.update(export.get("metadata") or {})
    keys = (
        "image_block", "clut_block", "trailing_extent", "unmapped_sheet_extents",
        "frame_table_referenced_sheet_offsets", "months", "uninterpreted_extents",
        "unrepresented_extents", "source_palette_offset", "source_palette_size",
        "sheet_offsets", "sheet_size", "sheet_bytes", "source_size",
        "source_path_profile", "coverage_policy", "round_trip_verified",
        "sample_data_size_bytes", "sample_table_entry_count", "samples",
        "declared_total_size", "combined_body", "paired_body_name",
        "source_lba", "bits_per_pixel", "width", "height", "tim_size",
        "sample_sizes_table_offset", "program_count", "tone_count_field",
        "declared_vag_count_field", "version", "vab_id", "combined_body_offset",
        "data_offset", "data_size_bytes", "trailing_bytes", "sample_rate_hz",
        "channels", "encoding", "loop_markers", "decode",
        "source_sha256", "bs_version", "quantization", "declared_mdec_bytes",
        "decoded_runlevel_halfwords", "block_count", "macroblock_count",
        "escape_codes", "zero_level_escape_codes", "end_marker_bit_offset",
        "consumed_stream_bits", "unused_bits_in_final_source_halfword",
        "geometry_context", "hardware_pixel_parity_verified", "padded_height",
        "evidence", "limitations", "color_policy", "alpha_policy", "stp_policy",
        "source_output_bit15", "coefficient_order", "bit_order", "dc_coding",
        "unrepresented_pixel_regions",
        "background_sheet_offsets", "actor_sheet_offsets",
        "uninterpreted_frame_record_gaps", "partially_represented_extents",
        "unmapped_model_palette_uploads", "source_sequence",
        "actor_sheet_offset", "actor_sheet_bytes", "actor_bits_per_pixel", "background_bits_per_pixel",
        "actor_texture_table_offset", "actor_frame_table_offset", "actor_frame_count",
        "actor_palette_source_offset", "actor_runtime_palette_override", "actor_runtime_palette_mode",
        "background_palette_source_offset", "palette_mapping_scope", "palette_transform",
        "palette_source_offsets", "proved_selected_palette_frame_indices", "empty_source_frames",
        "external_runtime_assets", "unrepresented_runtime_uses", "texture_pages",
        "texture_page_table_offset", "texture_pointer_table_offset", "frame_table_offset",
        "frame_count", "sequence_table_offset", "sequence_pointer_table_offset",
        "sequence_table_prefix_words", "frame_record_extents", "frame_record_unreferenced_gaps",
        "frame_table_padding", "palette_source_extents", "source_palette_policy", "unresolved_frames",
        "direct_texture_upload", "direct_texture_uploads", "direct_texture_page_upload", "page_coverage",
    )
    components = {key: metadata[key] for key in keys if key in metadata}
    regions = _image_regions(asset)
    fmt = asset.get("format") or export.get("format")
    if fmt in {"tokimemo-sd-animation", "tokimemo-sports-ui", "tokimemo-rpg-bat-ui"}:
        # Retain the complete bounded decoder evidence, including caller tables,
        # sequence-prefix limits, opaque extents, palette words and tile crops.
        # None of these source records constitutes whole-pack or text coverage.
        components["decoder_metadata"] = deepcopy(metadata)
        components["decoded_image_regions"] = deepcopy(regions)
    if fmt == "tokimemo-main-ui":
        # Frame part rectangles are many-to-one references into these source sheets.
        # Keep exact offset/extent geometry and mark the distinction from pixel coverage.
        components["referenced_sheet_extents"] = [
            {"offset": offset, "size": components.get("sheet_bytes"),
             "coverage": "sheet referenced by at least one frame part; not all texels proven used"}
            for offset in components.get("frame_table_referenced_sheet_offsets", [])
        ]
        components["frame_part_region_count"] = sum(
            len(image.get("parts") or []) for image in regions if image.get("frame_index") is not None
        )
        # Retain source-linked per-frame part rectangles and the exact CLUT source
        # words/address used by each rendered variant. Deduplicate palette records
        # while preserving the frame/part references that select them.
        part_regions, part_keys, palette_map = [], set(), {}
        for image in regions:
            if image.get("frame_index") is None:
                continue
            for part in image.get("parts") or []:
                region_key = (image.get("frame_index"), part.get("part_index"))
                if region_key not in part_keys:
                    part_keys.add(region_key)
                    part_regions.append({"frame_index": image.get("frame_index"),
                                         **{key: part.get(key) for key in (
                                             "part_index", "sheet_index", "sheet_source_offset", "u", "v",
                                             "width", "height", "x", "y", "texture_id", "flip_flags")}})
                palette_key = (image.get("variant"), part.get("palette_exe_address"),
                               tuple(part.get("clut_vram") or []),
                               tuple(part.get("source_palette_words") or []))
                palette_map.setdefault(palette_key, {
                    "variant": image.get("variant"),
                    "palette_exe_address": part.get("palette_exe_address"),
                    "palette_exe_offset": part.get("palette_exe_offset"),
                    "clut_vram": part.get("clut_vram"),
                    "source_palette_words": part.get("source_palette_words"),
                })
        components["frame_part_source_regions"] = part_regions
        components["frame_part_source_region_count"] = len(part_regions)
        components["palette_sources"] = [palette_map[key] for key in sorted(palette_map, key=repr)]
    elif fmt == "tokimemo-calendar-ui":
        components["calendar_tile_count"] = len(components.get("months") or [])
        components["calendar_tile_extents"] = [
            {"month_index": month.get("month_index"), "offset": month.get("tile_source_offset"),
             "size": month.get("tile_source_size"), "sha256": month.get("tile_sha256")}
            for month in components.get("months", [])
        ]
        palette_map = {}
        for image in regions:
            key = (image.get("variant"), image.get("palette_exe_address"),
                   tuple(image.get("clut_vram") or []), tuple(image.get("source_palette_words") or []))
            palette_map.setdefault(key, {"variant": image.get("variant"),
                                          "palette_exe_address": image.get("palette_exe_address"),
                                          "palette_exe_offset": image.get("palette_exe_offset"),
                                          "clut_vram": image.get("clut_vram"),
                                          "source_palette_words": image.get("source_palette_words")})
        components["palette_sources"] = [palette_map[key] for key in sorted(palette_map, key=repr)]
        components["calendar_numeric_record_extents"] = [
            {"month_index": month.get("month_index"), "offset": (month.get("numeric_records") or {}).get("source_offset"),
             "size": (month.get("numeric_records") or {}).get("source_size"),
             "day_count": (month.get("numeric_records") or {}).get("day_count")}
            for month in components.get("months", [])
        ]
    elif fmt == "tokimemo-portrait":
        components["texture_page_extents"] = [
            {"offset": image.get("source_sheet_offset"), "size": image.get("source_sheet_size"),
             "sha256": image.get("source_sheet_sha256"), "preview_kind": image.get("preview_kind")}
            for image in regions if image.get("preview_kind") == "source_texture_page"
        ]
        components["static_frame_part_count"] = sum(
            len(image.get("parts") or []) for image in regions if image.get("preview_kind") == "static_frame_composite"
        )
        components["static_frame_parts"] = [
            {key: part.get(key) for key in ("part_index", "sheet_index", "sheet_source_offset",
                                            "source_record_offset", "u", "v", "width", "height",
                                            "x", "y", "flip_flags", "texture_id", "palette_info")}
            for image in regions if image.get("preview_kind") == "static_frame_composite"
            for part in image.get("parts") or []
        ]
    elif fmt == "tokimemo-title-ui":
        components["title_image_region_count"] = len(regions)
        components["title_palette_variant_count"] = sum(
            1 for image in regions if image.get("variant") is not None
        )
    elif fmt == "psx-tim":
        # Image and CLUT blocks are exact source byte extents. Palette words live
        # in the image variant records and remain available in the exported manifest.
        components["palette_variant_count"] = len(regions)
    if fmt in {"tokimemo-title-ui", "tokimemo-filesel-ui", "tokimemo-option-ui",
               "tokimemo-exam-ui", "tokimemo-name-ui"}:
        components["static_ui_frame_regions"] = [
            {"frame_index": image.get("frame_index"), "variant": image.get("variant"),
             "width": image.get("width"), "height": image.get("height"),
             "image_palette": {key: image.get(key) for key in (
                 "clut_vram", "palette_source_offset", "source_palette_words", "preview_palette_words",
                 "palette_override", "palette_status", "palette_index", "runtime_selection_evidence")},
             "parts": [{key: part.get(key) for key in (
                 "part_index", "source_record_offset", "sheet_index", "page_index",
                 "sheet_source_offset", "source_sheet_offset", "u", "v", "width", "height",
                 "x", "y", "flip_flags", "texture_id", "texture_info", "palette_info", "clut_info",
                 "clut_vram", "clut_row", "palette_source_offset", "palette_exe_address",
                 "palette_exe_offset", "source_palette_words", "preview_palette_words",
                 "palette_mode", "palette_runtime_override")}
                       for part in image.get("parts") or []]}
            for image in regions if image.get("frame_index") is not None]
        components["source_texture_page_regions"] = [
            {key: image.get(key) for key in (
                "variant", "source_sheet_offset", "source_sheet_size", "source_sheet_sha256",
                "width", "height", "palette_source_offset", "clut_vram", "source_palette_words",
                "preview_palette_words", "palette_status", "vram_rectangle_words",
                "runtime_bg_cell", "composition_status")}
            for image in regions if image.get("preview_kind") == "source_texture_page"]
    if fmt == "sony_vab_header":
        components["sample_source_extents"] = [
            {"sample_index": sample.get("index"), "offset": sample.get("offset"),
             "size": sample.get("size_bytes"), "empty": sample.get("empty")}
            for sample in components.get("samples", [])
        ]
    return components


def _review_counts(asset_id: str, catalog_regions: dict[str, list[dict]]) -> dict:
    counts: dict[str, int] = {}
    for region in catalog_regions.get(asset_id, []):
        state = region.get("state") or "unreviewed"
        counts[state] = counts.get(state, 0) + 1
    return dict(sorted(counts.items()))


def _compact_location(location: Any) -> Any:
    """Retain exact source extents while run-length encoding repeated sector rows."""
    if not isinstance(location, dict):
        return location
    result = {key: value for key, value in location.items() if key != "sectors"}
    sectors = location.get("sectors")
    if not isinstance(sectors, list):
        return result
    runs = []
    for sector in sectors:
        if not isinstance(sector, dict):
            runs.append({"records": [sector]})
            continue
        metadata = {key: value for key, value in sector.items() if key not in {"lba", "byte_offset"}}
        lba = sector.get("lba")
        byte_offset = sector.get("byte_offset")
        if runs:
            previous = runs[-1]
            prev_meta = previous.get("sector_metadata")
            lba_contiguous = lba is not None and previous.get("last_lba") is not None and lba == previous["last_lba"] + 1
            byte_contiguous = (byte_offset is not None and previous.get("last_byte_offset") is not None
                               and byte_offset == previous["last_byte_offset"] + previous["sector_metadata"].get("sector_bytes", 0))
            if metadata == prev_meta and ((lba_contiguous and previous.get("address_kind") == "lba")
                                          or (byte_contiguous and previous.get("address_kind") == "byte_offset")):
                previous["count"] += 1
                if lba is not None:
                    previous["last_lba"] = lba
                if byte_offset is not None:
                    previous["last_byte_offset"] = byte_offset
                continue
        run = {"count": 1, "sector_metadata": metadata}
        if lba is not None:
            run.update(address_kind="lba", first_lba=lba, last_lba=lba)
        elif byte_offset is not None:
            run.update(address_kind="byte_offset", first_byte_offset=byte_offset,
                       last_byte_offset=byte_offset)
        else:
            run["address_kind"] = "unaddressed_record"
        runs.append(run)
    # The source parser emits ordered contiguous records. Runs above preserve the
    # start/count/sector metadata; remove bookkeeping endpoints when they duplicate
    # a start because `count` plus contiguous stride determines the end exactly.
    compact = []
    for run in runs:
        item = {key: value for key, value in run.items() if key not in {"last_lba", "last_byte_offset"}}
        compact.append(item)
    result["sector_runs"] = compact
    result["sector_record_count"] = len(sectors)
    return result


def build_media_coverage(media_manifest: dict, source_inventory: dict | None = None,
                         visual_catalog: dict | None = None) -> dict:
    """Summarize per-source decode status, source components and visual reviews.

    Optional ``source_inventory`` is another input manifest from the original
    reader. It adds stream/unsupported source records absent from a filtered
    media export. Counts always distinguish these files from image regions.
    """
    media_records = _records(media_manifest, "media_manifest")
    inventory_records = _records(source_inventory, "source_inventory") if source_inventory is not None else []
    catalog_by_asset: dict[str, list[dict]] = {}
    if visual_catalog is not None:
        if not isinstance(visual_catalog, dict) or visual_catalog.get("schema") != "tokimemo.visual-text.v1":
            raise CoverageError("visual_catalog must use tokimemo.visual-text.v1")
        visual_regions = visual_catalog.get("regions")
        if not isinstance(visual_regions, list):
            raise CoverageError("visual catalog regions must be a list")
        seen_region_ids = set()
        for region in visual_regions:
            if not isinstance(region, dict) or not isinstance(region.get("parent_asset_id"), str):
                raise CoverageError("visual catalog region needs parent_asset_id")
            region_id = region.get("region_id")
            if not isinstance(region_id, str) or not region_id or region_id in seen_region_ids:
                raise CoverageError("visual catalog region ids must be unique nonempty strings")
            state = region.get("state")
            if state not in {"unreviewed", "suspected", "confirmed_japanese_text",
                             "confirmed_no_text", "confirmed_other_text"}:
                raise CoverageError("visual catalog region has an unknown review state")
            seen_region_ids.add(region_id)
            catalog_by_asset.setdefault(region["parent_asset_id"], []).append(region)

    by_path: dict[str, dict] = {}
    media_ids = set()
    for record in media_records:
        source = record.get("source") or {}
        path = source.get("path")
        if not isinstance(path, str) or not path:
            raise CoverageError("every media source requires a path")
        if path in by_path:
            raise CoverageError("duplicate media source path: " + path)
        asset_id = record.get("id")
        if not isinstance(asset_id, str) or not asset_id or asset_id in media_ids:
            raise CoverageError("media asset ids must be unique nonempty strings")
        media_ids.add(asset_id)
        by_path[path] = record
    inventory_by_path: dict[str, dict] = {}
    for record in inventory_records:
        source = record.get("source") or {}
        path = source.get("path")
        if not isinstance(path, str) or not path:
            raise CoverageError("every source inventory record requires a path")
        if path in inventory_by_path:
            raise CoverageError("duplicate source inventory path: " + path)
        inventory_by_path[path] = record
    for path, record in inventory_by_path.items():
        if path not in by_path:
            # An independently supplied original inventory may contain omitted
            # streams or files. Add those as unresolved source records only.
            inventory_status = record.get("status")
            by_path[path] = {"id": record.get("id"), "source": record.get("source"),
                             "status": "unsupported" if inventory_status == "unsupported" else "unreviewed",
                             "format": record.get("format", "unknown"),
                             "reason": record.get("unsupported") or record.get("reason") or "not in selected media export",
                             "inventory_only": True, "inventory_status": inventory_status}

    sources = []
    for path, asset in sorted(by_path.items()):
        source = asset.get("source") or {}
        original = inventory_by_path.get(path, {})
        inventory_source = original.get("source") or {}
        media_hash = source.get("sha256")
        inventory_hash = inventory_source.get("sha256")
        if media_hash and inventory_hash and media_hash != inventory_hash:
            raise CoverageError("media and original inventory hashes differ for " + path)
        media_size = source.get("size")
        inventory_size = inventory_source.get("size")
        if media_size is not None and inventory_size is not None and media_size != inventory_size:
            raise CoverageError("media and original inventory sizes differ for " + path)
        media_location = source.get("location")
        inventory_location = inventory_source.get("location")
        if (isinstance(media_location, dict) and isinstance(inventory_location, dict)
                and isinstance(media_location.get("lba"), int)
                and isinstance(inventory_location.get("lba"), int)
                and media_location["lba"] != inventory_location["lba"]):
            raise CoverageError("media and original inventory LBAs differ for " + path)
        fmt = asset.get("format") or (asset.get("export") or {}).get("format") or original.get("format") or "unknown"
        status = asset.get("status", "unreviewed")
        coverage_status = status if status in {"decoded", "partial", "unsupported", "unreviewed"} else (
            "decoded" if status == "exported" else "partial" if status == "partial" else
            "unsupported" if status in {"unsupported", "invalid"} else "unreviewed"
        )
        images = _image_regions(asset)
        if fmt in _IMAGE_FORMATS and status == "exported" and not images:
            raise CoverageError("exported image-format asset has no image regions: " + path)
        inspection = asset.get("inspection") or {}
        inspection_metadata = inspection.get("metadata") or {}
        source_class = _profile(
            path, fmt,
            decoded=(status in {"exported", "partial"} and bool(images)),
            has_coefficient_census=(isinstance(inspection_metadata.get("block_count"), int)
                                    and inspection_metadata.get("block_count", 0) > 0),
        )
        regions = catalog_by_asset.get(asset.get("id", ""), [])
        source_hash = source.get("sha256") or inventory_source.get("sha256")
        source_location = source.get("location")
        inventory_location = inventory_source.get("location")
        source_record = {
            "asset_id": asset.get("id"),
            "path": path,
            "size": source.get("size", inventory_source.get("size")),
            "sha256": source_hash,
            "location": _compact_location(source_location if source_location is not None else inventory_location),
            "original_input_location": (_compact_location(inventory_location)
                                         if inventory_location is not None and inventory_location != source_location
                                         else None),
            "media_format": fmt,
            "media_status": status,
            "inventory_status": asset.get("inventory_status"),
            "coverage_status": coverage_status,
            "source_classification": source_class,
            "source_evidence": _source_evidence(path, asset),
            "reason": asset.get("reason") or asset.get("unsupported"),
            "image_variant_count": len(images),
            "visual_region_count": len(regions),
            "visual_review_states": _review_counts(asset.get("id", ""), catalog_by_asset),
            "source_components": _source_components(asset),
            "paired_source": ({key: asset["paired_source"].get(key) for key in
                               ("path", "sha256", "size", "kind")}
                              | {"location": _compact_location(asset["paired_source"].get("location"))}
                              if isinstance(asset.get("paired_source"), dict) else asset.get("paired_source")),
        }
        # Input-only stream records sometimes lack their own payload hash.
        # Preserve that fact rather than borrowing the whole-disc hash as a file hash.
        if source_hash is None:
            source_record["sha256_status"] = "not_available_for_source_record"
        sources.append(source_record)

    indexed_asset_ids = {item["asset_id"] for item in sources if item.get("asset_id")}
    orphan_region_count = sum(
        len(regions) for asset_id, regions in catalog_by_asset.items()
        if asset_id not in indexed_asset_ids
    )

    coverage = {"decoded": 0, "partial": 0, "unsupported": 0, "unreviewed": 0}
    classes: dict[str, int] = {}
    suffixes: dict[str, int] = {}
    total_regions = 0
    review_states: dict[str, int] = {}
    for item in sources:
        coverage[item["coverage_status"]] = coverage.get(item["coverage_status"], 0) + 1
        cls = item["source_classification"]["class"]
        classes[cls] = classes.get(cls, 0) + 1
        suffix = Path(item["path"]).suffix.upper() or "[no suffix]"
        suffixes[suffix] = suffixes.get(suffix, 0) + 1
        total_regions += item["visual_region_count"]
        for state, count in item["visual_review_states"].items():
            review_states[state] = review_states.get(state, 0) + count

    return {
        "schema": SCHEMA,
        "source_media_schema": media_manifest["schema"],
        "source_inventory_schema": source_inventory.get("schema") if source_inventory else None,
        "source_inventory_input": source_inventory.get("input") if source_inventory else None,
        "visual_catalog_schema": visual_catalog.get("schema") if visual_catalog else None,
        "corpus_complete": False,
        "graphic_source_coverage_complete": False,
        "counts": {"source_files": len(sources), "source_coverage_status": dict(sorted(coverage.items())),
                   "source_classifications": dict(sorted(classes.items())),
                   "suffixes": dict(sorted(suffixes.items())),
                   "decoded_image_variants": sum(item["image_variant_count"] for item in sources),
                   "visual_regions": total_regions,
                   "visual_catalog_regions": sum(len(regions) for regions in catalog_by_asset.values()),
                   "visual_catalog_orphan_regions": orphan_region_count,
                   "visual_review_states": dict(sorted(review_states.items()))},
        "limitations": [
            "source files, decoded image variants and visual regions are separate count units",
            "decoded image previews do not establish complete source extent, palette, runtime mapping, or Japanese-text coverage",
            "probable graphic suffixes are triage evidence only; BIN is not classified as an image from its suffix",
            "MDEC backgrounds, remaining NBN/BIN/TMD/EXN content, unused regions/palettes and stream/reference mapping remain unresolved",
            "source inventory hashes are null where the input reader cannot hash a logical payload, including Form2 streams",
            "without source_inventory, omitted streams and original-disc LBA locations are not covered",
        ],
        "sources": sources,
    }


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--media-manifest", required=True)
    parser.add_argument("--source-inventory")
    parser.add_argument("--visual-catalog")
    parser.add_argument("--output", required=True, help="new output path beneath ignored .asset-cache/")
    args = parser.parse_args(argv)
    try:
        media = json.loads(Path(args.media_manifest).read_text(encoding="utf-8"))
        inventory = json.loads(Path(args.source_inventory).read_text(encoding="utf-8")) if args.source_inventory else None
        catalog = json.loads(Path(args.visual_catalog).read_text(encoding="utf-8")) if args.visual_catalog else None
        result = build_media_coverage(media, inventory, catalog)
        write_manifest(output_root(args.output), result)
        counts = result["counts"]
        print("%d sources, %d image variants, %d visual regions" %
              (counts["source_files"], counts["decoded_image_variants"], counts["visual_regions"]))
        return 0
    except (OSError, ValueError, json.JSONDecodeError) as error:
        print("media_coverage.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
