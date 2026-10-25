#!/usr/bin/env python3
"""Find conservative same-geometry image review groups without copying reviews.

Groups retain every palette member. Pixel equivalence is only a proof that the
rendered RGBA color-class partition and alpha mask match; it never changes a
visual-text review state or asserts that palette colors have equal contrast.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys
from typing import Any, Mapping
import zlib

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.manifest import REPOSITORY, digest, output_root, write_manifest  # noqa: E402

SCHEMA = "tokimemo.visual-text-equivalence.v1"
MAX_PNG_BYTES = 64 * 1024 * 1024
MAX_PIXELS = 4 * 1024 * 1024
MAX_ALIAS_ENCODED_BYTES = 256 * 1024 * 1024
MAX_PARTITION_COLORS = 65_536
MAX_CONTRAST_COLORS = 4_096
NEAR_DUPLICATE_RGB_DISTANCE = 8.0
_GEOMETRY_PART_KEYS = (
    "source_sheet_offset", "sheet_source_offset", "source_page_offset", "source_extent",
    "sheet_index", "page_index", "u", "v", "x", "y", "width", "height",
    "flip_flags", "orientation", "layer", "layer_index", "z_order", "draw_order",
    "bits_per_pixel", "pixel_depth", "texture_depth", "texture_id", "texture_info",
)
_GEOMETRY_VARIANT_KEYS = (
    "source_sheet_offset", "source_sheet_size", "source_sheet_sha256", "source_page_offset",
    "source_page_size", "source_extent", "source_region", "crop", "tile_source_offset",
    "tile_source_size", "tile_sha256", "month_index", "orientation",
    "layer", "layer_index", "z_order", "draw_order", "image_block",
    "bits_per_pixel", "pixel_mode", "texture_depth", "source_encoding",
)
_GEOMETRY_VARIANT_EXTENTS = (
    ("source_sheet_offset", "source_sheet_size"), ("source_page_offset", "source_page_size"),
    ("tile_source_offset", "tile_source_size"),
)
_SHA256 = re.compile(r"^[0-9a-f]{64}$")


class EquivalenceError(ValueError):
    """Invalid manifests, geometry, or cache artifacts."""


def _json_key(value: Any) -> str:
    return json.dumps(value, ensure_ascii=True, sort_keys=True, separators=(",", ":"))


def _geometry(region: dict) -> dict | None:
    source = region.get("source") or {}
    placement = region.get("source_placement") or {}
    original = region.get("original_metadata") or {}
    variant = region.get("variant_metadata") or {}
    if not all(isinstance(value, dict) for value in (source, placement, original, variant)):
        return None
    source_sha = source.get("sha256")
    bounds = region.get("region")
    if not isinstance(source_sha, str) or not _SHA256.fullmatch(source_sha) or not isinstance(bounds, dict):
        return None
    required_bounds = ("x", "y", "width", "height")
    if any(not isinstance(bounds.get(key), int) or isinstance(bounds.get(key), bool)
           or bounds[key] < 0 for key in required_bounds):
        return None
    if not bounds["width"] or not bounds["height"]:
        return None
    image_block = placement.get("image_block", original.get("image_block"))
    if image_block is not None and not isinstance(image_block, dict):
        return None
    if image_block is not None and not all(isinstance(image_block.get(key), int)
                                           and not isinstance(image_block.get(key), bool)
                                           and image_block[key] >= 0 for key in ("offset", "size")):
        return None
    if image_block is not None and image_block["size"] == 0:
        return None
    variant_geometry = {key: variant[key] for key in _GEOMETRY_VARIANT_KEYS if key in variant}
    for key in ("source_sheet_offset", "source_sheet_size", "source_page_offset", "source_page_size",
                "tile_source_offset", "tile_source_size"):
        if key in variant_geometry and (not isinstance(variant_geometry[key], int)
                                        or isinstance(variant_geometry[key], bool)
                                        or variant_geometry[key] < 0):
            return None
    # A source image extent, a source-sheet extent/crop, or ordered frame parts
    # must identify which source pixels contribute to the rendered region.
    source_parts = placement.get("frame_parts")
    if source_parts is not None and not isinstance(source_parts, list):
        return None
    parts = []
    if isinstance(source_parts, list):
        for part in source_parts:
            if not isinstance(part, dict):
                return None
            selected = {key: part[key] for key in _GEOMETRY_PART_KEYS if key in part}
            source_extent = selected.get("source_extent")
            extent_valid = (isinstance(source_extent, dict)
                            and isinstance(source_extent.get("offset"), int)
                            and not isinstance(source_extent.get("offset"), bool)
                            and isinstance(source_extent.get("size"), int)
                            and not isinstance(source_extent.get("size"), bool)
                            and source_extent["offset"] >= 0 and source_extent["size"] > 0)
            source_page_bound = any(isinstance(selected.get(key), int)
                                    and not isinstance(selected.get(key), bool)
                                    and selected[key] >= 0
                                    for key in ("source_sheet_offset", "sheet_source_offset", "source_page_offset"))
            orientation_valid = ((isinstance(selected.get("flip_flags"), int)
                                  and not isinstance(selected.get("flip_flags"), bool))
                                 or isinstance(selected.get("orientation"), str))
            if (not all(isinstance(selected.get(key), int) and not isinstance(selected.get(key), bool)
                        for key in ("x", "y", "u", "v", "width", "height"))
                    or selected["width"] <= 0 or selected["height"] <= 0
                    or not (source_page_bound or extent_valid) or not orientation_valid):
                return None
            parts.append(selected)
    bounded_variant_extent = any(all(key in variant_geometry for key in pair)
                                 for pair in _GEOMETRY_VARIANT_EXTENTS)
    if isinstance(variant_geometry.get("source_extent"), dict):
        extent = variant_geometry["source_extent"]
        bounded_variant_extent = bounded_variant_extent or (
            isinstance(extent.get("offset"), int) and isinstance(extent.get("size"), int)
            and extent["offset"] >= 0 and extent["size"] > 0)
    source_extent_present = bool(image_block or bounded_variant_extent or parts)
    if not source_extent_present:
        return None
    return {
        "source_sha256": source_sha,
        "source_dependencies": source.get("dependencies"),
        "image_block": image_block,
        "variant_source_geometry": variant_geometry,
        # Part order is significant: it preserves painter/layer compositing order.
        "ordered_source_parts": parts,
        "output_bounds": {key: bounds[key] for key in required_bounds},
    }


def _decode_png(data: bytes) -> tuple[int, int, bytes]:
    """Decode bounded non-interlaced 8-bit PNGs emitted by this repository."""
    if len(data) > MAX_PNG_BYTES or len(data) < 33 or not data.startswith(b"\x89PNG\r\n\x1a\n"):
        raise EquivalenceError("PNG signature/size is invalid or exceeds the comparison bound")
    cursor, compressed, ended = 8, bytearray(), False
    width = height = color_type = bit_depth = None
    while cursor + 12 <= len(data):
        length = struct.unpack_from(">I", data, cursor)[0]
        end = cursor + 12 + length
        if end > len(data):
            raise EquivalenceError("truncated PNG chunk")
        kind = data[cursor + 4:cursor + 8]
        payload = data[cursor + 8:cursor + 8 + length]
        crc = struct.unpack_from(">I", data, cursor + 8 + length)[0]
        if zlib.crc32(kind + payload) & 0xffffffff != crc:
            raise EquivalenceError("PNG chunk CRC mismatch")
        if kind == b"IHDR":
            if cursor != 8 or length != 13:
                raise EquivalenceError("invalid PNG IHDR")
            width, height, bit_depth, color_type, compression, filtering, interlace = struct.unpack(">IIBBBBB", payload)
            if (not width or not height or width * height > MAX_PIXELS or bit_depth != 8
                    or color_type not in {0, 2, 4, 6} or compression or filtering or interlace):
                raise EquivalenceError("unsupported PNG dimensions, depth, color type, or interlace")
        elif kind == b"IDAT":
            compressed.extend(payload)
        elif kind == b"IEND":
            if length:
                raise EquivalenceError("invalid PNG IEND")
            ended = True
            cursor = end
            break
        elif kind not in {b"IHDR"}:
            # Unknown ancillary chunks may change transparency (for example
            # tRNS); only this repository's minimal PNG subset is compared.
            raise EquivalenceError("PNG contains an unsupported or pixel-affecting chunk")
        cursor = end
    if not ended or cursor != len(data) or width is None or not compressed:
        raise EquivalenceError("incomplete PNG")
    channels = {0: 1, 2: 3, 4: 2, 6: 4}[color_type]
    row_bytes = width * channels
    try:
        decoder = zlib.decompressobj()
        raw = decoder.decompress(bytes(compressed), (row_bytes + 1) * height + 1)
        if len(raw) > (row_bytes + 1) * height or decoder.unconsumed_tail:
            raise EquivalenceError("PNG decoded stream exceeds its dimension bound")
        raw += decoder.flush()
    except zlib.error as error:
        raise EquivalenceError("invalid or oversized PNG image stream") from error
    if len(raw) != (row_bytes + 1) * height or not decoder.eof or decoder.unused_data:
        raise EquivalenceError("PNG decoded size does not match dimensions")
    rows: list[bytes] = []
    offset = 0
    prior = bytes(row_bytes)
    for _ in range(height):
        filter_type = raw[offset]
        offset += 1
        filtered = raw[offset:offset + row_bytes]
        offset += row_bytes
        row = bytearray(row_bytes)
        for i, value in enumerate(filtered):
            left = row[i - channels] if i >= channels else 0
            above = prior[i]
            upper_left = prior[i - channels] if i >= channels else 0
            if filter_type == 0:
                predictor = 0
            elif filter_type == 1:
                predictor = left
            elif filter_type == 2:
                predictor = above
            elif filter_type == 3:
                predictor = (left + above) // 2
            elif filter_type == 4:
                p = left + above - upper_left
                pa, pb, pc = abs(p - left), abs(p - above), abs(p - upper_left)
                predictor = left if pa <= pb and pa <= pc else above if pb <= pc else upper_left
            else:
                raise EquivalenceError("unknown PNG row filter")
            row[i] = (value + predictor) & 0xff
        rows.append(bytes(row))
        prior = row
    rgba = bytearray(width * height * 4)
    target = 0
    for row in rows:
        for i in range(0, row_bytes, channels):
            if color_type == 6:
                rgba[target:target + 4] = row[i:i + 4]
            elif color_type == 2:
                rgba[target:target + 4] = bytes((row[i], row[i + 1], row[i + 2], 255))
            elif color_type == 4:
                rgba[target:target + 4] = bytes((row[i], row[i], row[i], row[i + 1]))
            else:
                rgba[target:target + 4] = bytes((row[i], row[i], row[i], 255))
            target += 4
    return width, height, bytes(rgba)


def _pixel_equivalence(images: list[bytes]) -> dict:
    if not images:
        return {"status": "not_checked", "equivalent": False, "reason": "no PNG bytes supplied"}
    first_width, first_height, first_pixels = _decode_png(images[0])
    base_alpha = first_pixels[3::4]
    base_colors: set[bytes] = set()
    for i in range(0, len(first_pixels), 4):
        base_colors.add(first_pixels[i:i + 4])
        if len(base_colors) > MAX_PARTITION_COLORS:
            return {"status": "color_partition_exceeds_comparison_bound", "equivalent": False,
                    "dimensions": [first_width, first_height],
                    "max_colors": MAX_PARTITION_COLORS}
    member_metrics = [_visibility_metrics(first_pixels)]
    for data in images[1:]:
        width, height, pixels = _decode_png(data)
        if (width, height) != (first_width, first_height):
            return {"status": "different_output_dimensions", "equivalent": False,
                    "reference_dimensions": [first_width, first_height], "member_dimensions": [width, height]}
        if pixels[3::4] != base_alpha:
            return {"status": "different_alpha_mask", "equivalent": False,
                    "dimensions": [first_width, first_height]}
        forward, reverse = {}, {}
        for i in range(0, len(first_pixels), 4):
            a, b = first_pixels[i:i + 4], pixels[i:i + 4]
            if (a in forward and forward[a] != b) or (b in reverse and reverse[b] != a):
                return {"status": "different_color_boundary_partition", "equivalent": False,
                        "dimensions": [first_width, first_height]}
            forward[a], reverse[b] = b, a
        member_metrics.append(_visibility_metrics(pixels))
    return {"status": "color_partition_and_alpha_match", "equivalent": True,
            "dimensions": [first_width, first_height], "used_rgba_colors": len(base_colors),
            "alpha_pixels": {"transparent": base_alpha.count(0),
                             "translucent": sum(alpha not in {0, 255} for alpha in base_alpha),
                             "opaque": base_alpha.count(255)},
            "member_visibility_metrics": member_metrics,
            "proof_scope": "exported PNG preview pixels only; runtime GPU composition is not proven"}


def _visibility_metrics(pixels: bytes) -> dict:
    rgb = sorted({pixels[i:i + 3] for i in range(0, len(pixels), 4)})
    if len(rgb) < 2:
        distance = None
        status = "single_rgb_color"
    elif len(rgb) > MAX_CONTRAST_COLORS:
        distance = None
        status = "color_count_bound"
    else:
        best_squared = 3 * 255 * 255 + 1
        for index, first in enumerate(rgb[:-1]):
            for second in rgb[index + 1:]:
                red = second[0] - first[0]
                if red * red >= best_squared:
                    break
                green = second[1] - first[1]
                blue = second[2] - first[2]
                squared = red * red + green * green + blue * blue
                if squared < best_squared:
                    best_squared = squared
        distance = best_squared ** 0.5
        status = "measured"
    return {"unique_rgb_colors": len(rgb), "min_pairwise_used_rgb_euclidean_distance": distance,
            "contrast_flag": "near_duplicate_used_colors" if distance is not None
            and distance <= NEAR_DUPLICATE_RGB_DISTANCE else status,
            "contrast_interpretation": "RGB distance is a near-duplicate flag, not a contrast ratio; backdrop and blend mode remain unknown"}


def _visibility_caveats(member: dict, pixel_stats: dict | None) -> list[str]:
    caveats = []
    preview = member.get("preview_kind")
    if preview == "source_index_diagnostic":
        caveats.append("runtime palette/alpha/placement unresolved; numeric diagnostic cannot authorize shared review")
    clut_value = member.get("clut")
    clut = clut_value if isinstance(clut_value, dict) else {}
    if clut.get("palette_status") in {None, "unknown", "not_applicable"}:
        caveats.append("runtime palette applicability is unknown or not established")
    if not member.get("alpha_handling"):
        caveats.append("alpha handling is not documented")
    if member.get("stp_status") not in {"exported", "not_applicable"}:
        caveats.append("STP/visibility handling is incomplete or unknown")
    if pixel_stats and pixel_stats.get("alpha_pixels", {}).get("translucent", 0):
        caveats.append("translucent pixels may blend differently against screen backgrounds")
    metrics = pixel_stats.get("member_visibility_metrics") if pixel_stats else None
    if metrics and any(m.get("contrast_flag") == "near_duplicate_used_colors" for m in metrics):
        caveats.append("used palette colors include near-duplicate RGB values; manual contrast check is required")
    caveats.append("palette contrast against the eventual screen background is not proven by color bijection")
    return caveats


def _shared_visual_evidence(members: list[dict], stp_bytes: Mapping[str, bytes] | None) -> tuple[bool, list[str]]:
    reasons = []
    for key, label in (("alpha_handling", "alpha policies differ or are unknown"),
                       ("stp_handling", "STP policies differ or are unknown")):
        values = {member.get(key) for member in members}
        if None in values or len(values) != 1:
            reasons.append(label)
    stp_statuses = {member.get("stp_status") for member in members}
    stp_hashes = {member.get("stp", {}).get("sha256") if isinstance(member.get("stp"), dict) else None
                  for member in members}
    if not stp_statuses <= {"exported", "not_applicable"}:
        reasons.append("one or more STP masks are unavailable")
    if any((member.get("stp_status") == "exported"
            and not isinstance(member.get("stp"), dict))
           or (member.get("stp_status") == "not_applicable" and member.get("stp") is not None)
           for member in members):
        reasons.append("STP status and artifact reference disagree")
    if len(stp_hashes) != 1:
        reasons.append("STP masks differ or are missing for some palette variants")
    for member in members:
        if member.get("stp_status") != "exported":
            continue
        reference = member.get("stp") or {}
        payload = stp_bytes.get(member["region_id"]) if stp_bytes is not None else None
        if not isinstance(payload, bytes) or digest(payload) != reference.get("sha256"):
            reasons.append("STP mask bytes are not supplied or hash-verified")
            break
    if not all(isinstance(member.get("clut"), dict)
               and member["clut"].get("palette_status") not in {None, "unknown", "not_applicable"}
               for member in members):
        reasons.append("runtime palette applicability is unknown or not established")
    if any(member.get("preview_kind") == "source_index_diagnostic" for member in members):
        reasons.append("runtime palette/alpha/placement unresolved for a diagnostic preview")
    for member in members:
        scope = str(member.get("palette_variant_scope") or "").lower()
        if any(token in scope for token in ("unconfirmed", "not confirmed", "candidate", "unknown")):
            reasons.append("palette variant is a runtime candidate, not a confirmed applicable palette")
            break
    return not reasons, reasons


def _exact_alias_groups(validated_regions: dict[str, dict], image_bytes: Mapping[str, bytes]) -> list[dict]:
    total_bytes = 0
    by_hash: dict[str, list[tuple[dict, bytes]]] = {}
    decoded_by_hash = {}
    for region_id, region in sorted(validated_regions.items()):
        data = image_bytes.get(region_id)
        png = region["png"]
        if not isinstance(data, bytes):
            raise EquivalenceError("exact-alias scan is missing PNG bytes for " + region_id)
        total_bytes += len(data)
        if total_bytes > MAX_ALIAS_ENCODED_BYTES:
            raise EquivalenceError("exact-alias PNG byte budget exceeded")
        if digest(data) != png["sha256"]:
            raise EquivalenceError("exact-alias PNG hash mismatch for " + region_id)
        if png["sha256"] not in decoded_by_hash:
            width, height, _pixels = _decode_png(data)
            decoded_by_hash[png["sha256"]] = (width, height)
        representative = next((payload for _, payload in by_hash.get(png["sha256"], [])), None)
        if representative is not None and representative != data:
            raise EquivalenceError("PNG SHA-256 collision detected in exact-alias scan")
        by_hash.setdefault(png["sha256"], []).append((region, data))
    groups = []
    for png_hash, items in sorted(by_hash.items()):
        if len(items) < 2:
            continue
        region_ids = set()
        members = []
        for region, _data in items:
            source = region.get("source") or {}
            region_ids.add(region["region_id"])
            metadata = region.get("variant_metadata") or {}
            members.append({
                "region_id": region["region_id"],
                "png": dict(region["png"]),
                "parent_asset_id": region.get("parent_asset_id"),
                "source": {key: source.get(key) for key in ("path", "sha256", "location", "dependencies")},
                "frame": region.get("frame"), "palette_index": region.get("palette_index"),
                "variant": metadata.get("variant"),
                "source_placement": region.get("source_placement"),
                "source_extent_metadata": {key: metadata.get(key) for key in (
                    "image_block", "source_sheet_offset", "source_sheet_size", "source_sheet_sha256",
                    "source_page_offset", "source_page_size", "tile_source_offset", "tile_source_size",
                    "tile_sha256", "source_region", "crop") if key in metadata},
                "clut": region.get("clut"), "runtime_context": region.get("screen_context"),
                "source_profile": region.get("source_profile"),
                "palette_variant_scope": metadata.get("palette_variant_scope"),
                "review_state": region.get("state", "unreviewed"),
                "review_confidence": region.get("confidence"),
                "alpha_handling": region.get("alpha_handling"),
                "stp_handling": region.get("stp_handling"),
                "stp_status": region.get("stp_status"), "stp": region.get("stp"),
            })
        if len(region_ids) < 2:
            continue
        members.sort(key=lambda member: (member["source"].get("path", ""), member["region_id"]))
        confirmed = {member["review_state"] for member in members
                     if member["review_state"] in {"confirmed_japanese_text", "confirmed_no_text",
                                                   "confirmed_other_text"}}
        conflict = len(confirmed) > 1
        stp_hashes = {(member.get("stp") or {}).get("sha256") if isinstance(member.get("stp"), dict) else None
                      for member in members}
        groups.append({
            "group_id": "preview-alias-" + png_hash,
            "status": "exact_png_preview_alias_manual_state_conflict" if conflict else
                     "exact_png_preview_alias_adjudication_possible",
            "proof": {"png_sha256": png_hash, "encoded_bytes_identical": True,
                      "dimensions": list(decoded_by_hash[png_hash]),
                      "proof_scope": "byte-identical PNG preview only; runtime palette, STP, blending and composition are not proven"},
            "manual_preview_text_adjudication_may_be_shared": not conflict,
            "manual_review_state_conflict": conflict,
            "existing_confirmed_states": sorted(confirmed),
            "review_states_are_independent": True,
            "automatic_review_promotion": False,
            "runtime_render_equivalence_proven": False,
            "stp_mask_hashes_differ": len(stp_hashes) > 1,
            "runtime_visibility_concerns": [
                "identical preview bytes do not establish source palette applicability or runtime composition",
                "member CLUTs, screen contexts, alpha/STP policy and existing review states must be retained independently",
                "text-state adjudication may be shared only for the identical preview; no art/runtime classification is shared",
            ],
            "members": members,
        })
    return groups


def build_equivalence(media_manifest: dict, visual_catalog: dict,
                      image_bytes: Mapping[str, bytes] | None = None,
                      stp_bytes: Mapping[str, bytes] | None = None,
                      exact_aliases: bool = False) -> dict:
    """Group exact source geometry and optionally prove RGBA boundary equivalence.

    ``image_bytes`` and ``stp_bytes`` map region IDs to their artifact bytes.
    Without PNG bytes, results are metadata-only candidates. Exported STP masks
    must be hash-verified before a group can be eligible for shared adjudication.
    No review states are copied.
    """
    if not isinstance(media_manifest, dict) or media_manifest.get("schema") != "tokimemo.media.v1":
        raise EquivalenceError("media manifest must use tokimemo.media.v1")
    if not isinstance(visual_catalog, dict) or visual_catalog.get("schema") != "tokimemo.visual-text.v1":
        raise EquivalenceError("visual catalog must use tokimemo.visual-text.v1")
    assets = media_manifest.get("assets")
    regions = visual_catalog.get("regions")
    if not isinstance(assets, list) or not isinstance(regions, list):
        raise EquivalenceError("media assets and visual regions must be lists")
    asset_by_id = {}
    for asset in assets:
        if not isinstance(asset, dict) or not isinstance(asset.get("id"), str) or not asset["id"]:
            raise EquivalenceError("each media asset requires an id")
        if asset["id"] in asset_by_id:
            raise EquivalenceError("duplicate media asset id")
        asset_by_id[asset["id"]] = asset
    groups_by_geometry: dict[str, list[dict]] = {}
    validated_regions: dict[str, dict] = {}
    ungrouped = []
    seen_ids = set()
    for region in regions:
        if (not isinstance(region, dict) or not isinstance(region.get("region_id"), str)
                or not region.get("region_id")):
            raise EquivalenceError("each visual region requires a region_id")
        region_id = region["region_id"]
        if region_id in seen_ids:
            raise EquivalenceError("duplicate visual region id")
        seen_ids.add(region_id)
        parent_id = region.get("parent_asset_id")
        if not isinstance(parent_id, str) or not parent_id:
            ungrouped.append({"region_id": region_id, "reason": "parent asset id is invalid"})
            continue
        parent = asset_by_id.get(parent_id)
        if parent is None:
            ungrouped.append({"region_id": region_id, "reason": "parent asset is absent from media manifest"})
            continue
        source = region.get("source") or {}
        png = region.get("png") or {}
        if not isinstance(source, dict) or not isinstance(png, dict):
            ungrouped.append({"region_id": region_id, "reason": "source or PNG provenance is malformed"})
            continue
        if (not isinstance(source.get("sha256"), str) or not _SHA256.fullmatch(source["sha256"])
                or not isinstance(png.get("sha256"), str) or not _SHA256.fullmatch(png["sha256"])):
            ungrouped.append({"region_id": region_id, "reason": "missing source or PNG hash"})
            continue
        parent_source = parent.get("source") or {}
        if parent_source.get("sha256") != source.get("sha256"):
            raise EquivalenceError("catalog source hash differs from media asset for " + region_id)
        parent_dependencies = parent.get("source_dependencies",
                                         parent.get("paired_source", parent_source.get("dependencies", [])))
        if source.get("dependencies", []) != parent_dependencies:
            raise EquivalenceError("catalog source dependencies differ from media asset for " + region_id)
        output_records = parent.get("outputs") or []
        if output_records:
            matches = [item for item in output_records if isinstance(item, dict)
                       and item.get("path") == png.get("path")]
            if len(matches) != 1 or matches[0].get("sha256") != png.get("sha256"):
                raise EquivalenceError("catalog PNG provenance differs from media outputs for " + region_id)
        validated_regions[region_id] = region
        geometry = _geometry(region)
        if geometry is None:
            ungrouped.append({"region_id": region_id, "reason": "incomplete source geometry; cannot group safely"})
            continue
        metadata = region.get("variant_metadata") or {}
        member = {
            "region_id": region_id,
            "png": {"path": png.get("path"), "sha256": png["sha256"]},
            "parent_asset_id": region.get("parent_asset_id"),
            "source": {"path": source.get("path"), "sha256": source.get("sha256"),
                       "location": source.get("location"),
                       "dependencies": source.get("dependencies")},
            "source_placement": region.get("source_placement"),
            "source_extent_metadata": {key: metadata.get(key) for key in (
                "image_block", "source_sheet_offset", "source_sheet_size", "source_sheet_sha256",
                "source_page_offset", "source_page_size", "tile_source_offset", "tile_source_size",
                "tile_sha256", "source_region", "crop") if key in metadata},
            "frame": region.get("frame"), "palette_index": region.get("palette_index"),
            "variant": metadata.get("variant"),
            "clut": region.get("clut"),
            "runtime_context": region.get("screen_context"),
            "source_profile": region.get("source_profile"),
            "review_state": region.get("state", "unreviewed"),
            "review_confidence": region.get("confidence"),
            "alpha_handling": region.get("alpha_handling"),
            "stp_handling": region.get("stp_handling"),
            "stp_status": region.get("stp_status"),
            "stp": region.get("stp"),
            "preview_kind": metadata.get("preview_kind"),
            "palette_variant_scope": metadata.get("palette_variant_scope"),
            "visibility_concerns": [],
            "_geometry": geometry,
        }
        key = _json_key(geometry)
        groups_by_geometry.setdefault(key, []).append(member)
    groups = []
    for key, members in sorted(groups_by_geometry.items()):
        if len(members) < 2:
            continue
        ordered = sorted(members, key=lambda m: (m["region_id"], m["png"]["sha256"]))
        pixel_stats = None
        if image_bytes is None:
            pixel_stats = {"status": "metadata_only", "equivalent": False,
                           "reason": "PNG pixels were not supplied; RGBA partition not proven"}
        else:
            try:
                payloads = []
                for member in ordered:
                    data = image_bytes.get(member["region_id"])
                    if not isinstance(data, bytes):
                        raise EquivalenceError("PNG bytes missing for region " + member["region_id"])
                    if digest(data) != member["png"]["sha256"]:
                        raise EquivalenceError("PNG hash mismatch for region " + member["region_id"])
                    payloads.append(data)
                pixel_stats = _pixel_equivalence(payloads)
            except EquivalenceError as error:
                pixel_stats = {"status": "pixel_check_unavailable", "equivalent": False,
                               "reason": str(error)}
        shared_visual, shared_visual_reasons = _shared_visual_evidence(ordered, stp_bytes)
        for member in ordered:
            member["visibility_concerns"] = _visibility_caveats(member, pixel_stats)
            member.pop("_geometry")
        if pixel_stats.get("equivalent") and shared_visual:
            status = "png_preview_partition_proven_manual_adjudication_required"
        elif pixel_stats.get("equivalent"):
            status = "pixel_partition_matches_but_visibility_evidence_differs"
        elif pixel_stats.get("status") == "metadata_only":
            status = "geometry_candidate_pixels_not_checked"
        else:
            status = "geometry_candidate_pixel_equivalence_not_proven"
        geometry = json.loads(key)
        groups.append({
            "group_id": "geometry-" + hashlib.sha256(key.encode("utf-8")).hexdigest(),
            "geometry": geometry,
            "status": status,
            "manual_review_may_be_shared": bool(pixel_stats.get("equivalent") and shared_visual),
            "manual_adjudication_conditions": ["visually inspect each member's palette contrast and intended context",
                                               "confirm alpha and STP handling are suitable for the target review"],
            "preview_only_proof": True,
            "runtime_render_equivalence_proven": False,
            "shared_visual_evidence_reasons": shared_visual_reasons,
            "pixel_partition_check": pixel_stats,
            "members": ordered,
            "automatic_review_promotion": False,
            "review_states_are_independent": True,
        })
    groups.sort(key=lambda group: group["group_id"])
    if exact_aliases:
        if image_bytes is None:
            raise EquivalenceError("exact-alias grouping requires PNG bytes for every region")
        if len(validated_regions) != len(regions):
            raise EquivalenceError("exact-alias grouping requires valid source/hash provenance for every region")
        exact_groups = _exact_alias_groups(validated_regions, image_bytes)
    else:
        exact_groups = []
    hash_failures = sum(
        1 for group in groups
        if group["pixel_partition_check"].get("status") == "pixel_check_unavailable"
        and any(marker in str(group["pixel_partition_check"].get("reason", ""))
                for marker in ("hash mismatch", "bytes missing"))
    )
    return {
        "schema": SCHEMA,
        "source_media_schema": media_manifest["schema"],
        "visual_catalog_schema": visual_catalog["schema"],
        "artifact_validation": {
            "status": ("metadata_only" if image_bytes is None else
                       "hash_verification_failed" if hash_failures else "sha256_verified"),
            "hash_failures": hash_failures,
            "member_pngs_checked": len(image_bytes) if image_bytes is not None else 0,
            "member_stp_masks_checked": len(stp_bytes) if stp_bytes is not None else 0},
        "groups": groups,
        "exact_preview_alias_groups": exact_groups,
        "ungrouped_regions": sorted(ungrouped, key=lambda item: item["region_id"]),
        "counts": {"regions": len(regions), "groups": len(groups),
                   "members_in_groups": sum(len(group["members"]) for group in groups),
                   "regions_without_equivalent_partner": len(regions) -
                   sum(len(group["members"]) for group in groups),
                   "ungrouped_regions": len(ungrouped),
                   "manual_share_eligible_groups": sum(group["manual_review_may_be_shared"] for group in groups),
                   "exact_preview_alias_groups": len(exact_groups),
                   "exact_preview_alias_members": sum(len(group["members"]) for group in exact_groups),
                   "exact_preview_alias_state_conflicts": sum(group["manual_review_state_conflict"]
                                                               for group in exact_groups)},
        "limitations": [
            "geometry equality is exact metadata equality over source SHA, extents, ordered crops/orientation/layers, and output bounds",
            "pixel proof compares used-RGBA color-class bijection and identical per-pixel alpha; it does not establish equal color contrast",
            "all member regions and palette variants remain separate; no review state or confidence is propagated",
            "unresolved runtime palettes, diagnostic previews, STP, blending and screen background can prevent shared adjudication",
            "this report does not detect text, transcribe text, or assert corpus completeness",
        ],
    }


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--media-manifest", required=True)
    parser.add_argument("--visual-catalog", required=True)
    parser.add_argument("--output", required=True, help="new output path beneath ignored .asset-cache/")
    parser.add_argument("--exact-aliases", action="store_true",
                         help="also group byte-identical PNG previews across differing source geometry")
    args = parser.parse_args(argv)
    try:
        media_path = Path(args.media_manifest)
        media = json.loads(media_path.read_text(encoding="utf-8"))
        catalog = json.loads(Path(args.visual_catalog).read_text(encoding="utf-8"))
        # Build groups first to discover exactly which members need byte checks.
        metadata = build_equivalence(media, catalog)
        png_bytes, stp_bytes = {}, {}
        if args.exact_aliases:
            region_ids = [region["region_id"] for region in catalog.get("regions", [])]
            member_by_id = {member["region_id"]: member for group in metadata["groups"]
                            for member in group["members"]}
            # Build lightweight artifact references for regions outside geometry groups.
            for region in catalog.get("regions", []):
                if region["region_id"] not in member_by_id:
                    member_by_id[region["region_id"]] = {
                        "region_id": region["region_id"], "png": region.get("png"),
                        "stp": region.get("stp")}
        else:
            member_by_id = {member["region_id"]: member for group in metadata["groups"]
                            for member in group["members"]}
            region_ids = sorted(member_by_id)
        total_encoded_bytes = 0
        grouped_region_ids = {member["region_id"] for group in metadata["groups"]
                              for member in group["members"]}
        for region_id in region_ids:
            member = member_by_id[region_id]
            artifact = member.get("png")
            if (not isinstance(artifact, dict) or not isinstance(artifact.get("path"), str)
                    or not isinstance(artifact.get("sha256"), str)):
                raise EquivalenceError("region has invalid PNG path/hash: " + region_id)
            artifact_path = output_root(REPOSITORY / artifact["path"])
            if not artifact_path.is_file():
                raise EquivalenceError("referenced PNG is missing: " + artifact["path"])
            if artifact_path.stat().st_size > MAX_PNG_BYTES:
                raise EquivalenceError("referenced PNG exceeds the byte comparison bound: " + artifact["path"])
            data = artifact_path.read_bytes()
            if digest(data) != artifact["sha256"]:
                raise EquivalenceError("referenced PNG hash is stale: " + artifact["path"])
            total_encoded_bytes += len(data)
            if args.exact_aliases and total_encoded_bytes > MAX_ALIAS_ENCODED_BYTES:
                raise EquivalenceError("exact-alias encoded PNG budget exceeded")
            png_bytes[region_id] = data
            stp = member.get("stp")
            if isinstance(stp, dict):
                stp_path = output_root(REPOSITORY / stp["path"])
                if not stp_path.is_file():
                    raise EquivalenceError("referenced STP mask is missing: " + stp["path"])
                if stp_path.stat().st_size > MAX_PNG_BYTES:
                    raise EquivalenceError("referenced STP mask exceeds the byte comparison bound: " + stp["path"])
                stp_data = stp_path.read_bytes()
                if digest(stp_data) != stp.get("sha256"):
                    raise EquivalenceError("referenced STP mask is missing or stale: " + stp["path"])
                if region_id in grouped_region_ids:
                    stp_bytes[region_id] = stp_data
        result = build_equivalence(media, catalog, png_bytes, stp_bytes, args.exact_aliases)
        stored = write_manifest(args.output, result)
        print("%d same-geometry groups, %d manually share-eligible; no reviews changed" % (
            result["counts"]["groups"], result["counts"]["manual_share_eligible_groups"]))
        print("report %s sha256 %s" % (stored["path"], stored["sha256"]))
        return 0
    except (OSError, json.JSONDecodeError, ValueError) as error:
        print("visual_text_equivalence.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
