#!/usr/bin/env python3
"""Build a reviewable visual text localization catalog from media manifest v1."""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
import re
import sys
from pathlib import Path
from typing import Any

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.manifest import REPOSITORY, digest, output_root, write_manifest  # noqa: E402

SCHEMA = "tokimemo.visual-text.v1"
REVIEW_STATES = {
    "unreviewed", "suspected", "confirmed_japanese_text", "confirmed_no_text",
    "confirmed_other_text",
}
CONFIRMED_STATES = {
    "confirmed_japanese_text", "confirmed_no_text", "confirmed_other_text"
}
REVIEW_STATE_MEANINGS = {
    "unreviewed": "No visual classification has been made.",
    "suspected": "Text may be present; visual confirmation is pending.",
    "confirmed_japanese_text": "Japanese text was confirmed by manual visual review.",
    "confirmed_no_text": "Manual visual review found no text in the region.",
    "confirmed_other_text": "Visible non-Japanese letters, numerals, or punctuation were confirmed manually.",
}
REPLACEMENT_WORK = {"undecided", "redraw", "font", "string_rendering", "none"}
CONFIRMATION_METHODS = {"manual_visual", "visual_reference"}
EVIDENCE_METHODS = CONFIRMATION_METHODS | {"ocr_candidate"}
PROBABLE_IMAGE_SUFFIXES = {".NBN", ".BS", ".TIM", ".BIN", ".TMD"}
_SHA256 = re.compile(r"^[0-9a-f]{64}$")


class CatalogError(ValueError):
    """An input manifest or human review does not satisfy the catalog schema."""


def _require(condition: bool, message: str) -> None:
    if not condition:
        raise CatalogError(message)


def _stable_region_id(parent_asset_id: str, frame: Any, palette_index: Any,
                      rect: dict[str, int], variant: Any = None) -> str:
    payload = json.dumps([parent_asset_id, frame, palette_index, variant,
                          rect["x"], rect["y"], rect["width"], rect["height"]],
                         ensure_ascii=True, separators=(",", ":"))
    return "region-" + hashlib.sha256(payload.encode("utf-8")).hexdigest()


def _validate_media_manifest(manifest: dict[str, Any]) -> list[dict[str, Any]]:
    _require(isinstance(manifest, dict), "media manifest must be an object")
    _require(manifest.get("schema") == "tokimemo.media.v1", "unsupported media manifest schema")
    records = manifest.get("assets")
    _require(isinstance(records, list), "media manifest assets must be a list")
    seen = set()
    for record in records:
        _require(isinstance(record, dict), "media asset record must be an object")
        asset_id = record.get("id")
        _require(isinstance(asset_id, str) and asset_id, "media asset id is required")
        _require(asset_id not in seen, "duplicate media asset id: " + asset_id)
        seen.add(asset_id)
    return records


def _review_map(reviews: list[dict[str, Any]] | None,
                current_hashes: dict[str, str]) -> dict[str, dict[str, Any]]:
    if reviews is None:
        reviews = []
    _require(isinstance(reviews, list), "reviews must be a list")
    result = {}
    allowed_keys = {"region_id", "png_sha256", "state", "confidence", "evidence", "replacement_work"}
    for item in reviews:
        _require(isinstance(item, dict), "each review must be an object")
        _require(set(item) <= allowed_keys, "review contains unknown fields")
        region_id = item.get("region_id")
        _require(isinstance(region_id, str) and region_id in current_hashes,
                 "review references an unknown or stale region id")
        _require(region_id not in result, "duplicate review for region " + region_id)
        _require(item.get("png_sha256") == current_hashes[region_id],
                 "review PNG hash is stale for region " + region_id)
        state = item.get("state")
        _require(state in REVIEW_STATES, "invalid review state")
        confidence = item.get("confidence")
        if state == "unreviewed":
            _require(confidence is None, "unreviewed confidence must be null")
            _require(item.get("evidence") is None, "unreviewed region cannot cite review evidence")
        else:
            _require(isinstance(confidence, (int, float)) and not isinstance(confidence, bool)
                     and 0 <= confidence <= 1, "reviewed confidence must be between 0 and 1")
        evidence = item.get("evidence")
        if evidence is not None:
            _require(isinstance(evidence, dict) and set(evidence) == {"method", "reference"},
                     "evidence must contain only method and reference")
            _require(evidence.get("method") in EVIDENCE_METHODS, "invalid evidence method")
            _require(isinstance(evidence.get("reference"), str) and evidence["reference"].strip(),
                     "evidence reference is required")
        if state in CONFIRMED_STATES:
            _require(isinstance(evidence, dict) and evidence.get("method") in CONFIRMATION_METHODS,
                     "confirmation requires manual visual evidence; OCR alone is insufficient")
        elif state == "suspected":
            _require(evidence is None or evidence.get("method") in EVIDENCE_METHODS,
                     "suspected evidence method is invalid")
        work = item.get("replacement_work", ["undecided"])
        _require(isinstance(work, list) and work and all(w in REPLACEMENT_WORK for w in work),
                 "replacement_work must be a nonempty list of known work states")
        _require(len(work) == len(set(work)), "replacement_work values must be unique")
        _require("none" not in work or len(work) == 1, "none cannot be combined with replacement work")
        _require("undecided" not in work or len(work) == 1,
                 "undecided cannot be combined with a selected replacement task")
        result[region_id] = {
            "state": state,
            "confidence": confidence,
            "evidence": evidence,
            "replacement_work": list(work),
        }
    return result


def _source_profile(source_path: str, *, media_format=None, metadata=None) -> dict[str, Any]:
    """Return evidence-backed context candidates, without asserting visible content."""
    try:
        from assets.visual_text_profiles import get_profile
    except ImportError:
        return {}
    profile = get_profile(source_path, media_format=media_format, metadata=metadata)
    return copy.deepcopy(profile) if isinstance(profile, dict) else {}


def build_catalog(media_manifest: dict, reviews: list[dict] | None = None) -> dict:
    """Build deterministic Japanese-text visual review catalog from exported images.

    Reviews are bound to both a stable region ID and the current PNG hash. Source
    context remains unknown; no screen or text meaning is inferred from filenames.
    """
    assets = _validate_media_manifest(media_manifest)
    regions = []
    unrepresented = []
    for asset in assets:
        source = asset.get("source")
        _require(isinstance(source, dict), "each media asset needs source provenance")
        source_path = source.get("path")
        _require(isinstance(source_path, str) and source_path, "source path is required")
        source_hash = source.get("sha256")
        export = asset.get("export") or {}
        input_hash = (media_manifest.get("input") or {}).get("source_sha256")
        if not (isinstance(source_hash, str) and _SHA256.fullmatch(source_hash)):
            source_hash = None
        if asset.get("status") in {"exported", "partial"} and export.get("images"):
            _require(source_hash is not None, "exported image source SHA-256 is required")
        inspection = asset.get("inspection") or {}
        candidates = inspection.get("candidates") or []
        variants = export.get("images") or []
        if asset.get("status") not in {"exported", "partial"} or not variants:
            suffix = Path(source_path).suffix.upper()
            profile = _source_profile(source_path)
            unresolved_graphics = bool(profile and profile.get("evidence")) or suffix == ".EXN" or Path(source_path).name.upper() == "SLPM_86.053"
            unrepresented.append({
                "parent_asset_id": asset["id"],
                "source": {"path": source_path, "location": source.get("location"),
                           "sha256": source_hash, "input_sha256": input_hash},
                "media_status": asset.get("status"),
                "format": asset.get("format", "unknown"),
                "coverage_status": "unreviewed_unresolved_graphics_source"
                if unresolved_graphics
                else "not_decoded_image",
                "probable_image_suffix": suffix in (PROBABLE_IMAGE_SUFFIXES | {".EXN"})
                or Path(source_path).name.upper() == "SLPM_86.053",
                "source_profile": profile or None,
                "reason": asset.get("reason", "not an exported TIM image variant"),
            })
            continue
        metadata = export.get("metadata") or (candidates[0] if candidates else {})
        _require(isinstance(metadata, dict), "TIM candidate metadata must be an object")
        image_block = metadata.get("image_block")
        variants_source = asset.get("source_dependencies",
                                    asset.get("paired_source", source.get("dependencies", [])))
        profile = _source_profile(source_path, media_format=asset.get("format") or export.get("format"),
                                  metadata=metadata)
        base_rect = metadata.get("region")
        if not isinstance(base_rect, dict):
            base_rect = {}
        rect = {
            "x": base_rect.get("x", 0),
            "y": base_rect.get("y", 0),
            "width": base_rect.get("width", metadata.get("width")),
            "height": base_rect.get("height", metadata.get("height")),
        }
        _require(all(v is None or (isinstance(v, int) and not isinstance(v, bool) and v >= 0)
                     for v in rect.values()),
                 "TIM image rectangle must have nonnegative integer coordinates and dimensions")
        base_rect = dict(rect)
        outputs = asset.get("outputs") or []
        output_by_name = {}
        for output in outputs:
            _require(isinstance(output, dict), "output metadata must be an object")
            output_path = output.get("path")
            output_hash = output.get("sha256")
            _require(isinstance(output_path, str) and output_path,
                     "output path is required")
            _require(isinstance(output_hash, str) and _SHA256.fullmatch(output_hash),
                     "output SHA-256 is required")
            filename = Path(output_path).name
            _require(filename not in output_by_name, "duplicate output filename in media asset")
            output_by_name[filename] = output
        for variant in variants:
            _require(isinstance(variant, dict), "exported image variant must be an object")
            png_name = variant.get("png")
            _require(isinstance(png_name, str) and png_name in output_by_name,
                     "TIM PNG output is missing from media asset hashes")
            png_output = output_by_name[png_name]
            png_hash = png_output.get("sha256")
            _require(isinstance(png_hash, str) and _SHA256.fullmatch(png_hash),
                     "TIM PNG SHA-256 is required")
            palette_index = variant.get("palette_index")
            _require(palette_index is None or
                     (isinstance(palette_index, int) and not isinstance(palette_index, bool)
                      and palette_index >= 0), "palette index must be a nonnegative integer or null")
            frame = variant.get("frame_index", variant.get("frame"))
            _require(frame is None or (isinstance(frame, (int, str)) and not isinstance(frame, bool)),
                     "frame identity must be an integer, string, or null")
            variant_identity = variant.get("variant")
            _require(variant_identity is None or
                     (isinstance(variant_identity, (int, str)) and not isinstance(variant_identity, bool)),
                     "variant identity must be an integer, string, or null")
            variant_rect = variant.get("region") or variant.get("rect")
            _require(variant_rect is None or isinstance(variant_rect, dict),
                     "image variant rectangle must be an object")
            rect = {key: (value if value is not None else 0) for key, value in base_rect.items()}
            if isinstance(variant_rect, dict):
                rect = {key: variant_rect.get(key, rect[key]) for key in ("x", "y", "width", "height")}
                _require(all(isinstance(v, int) and not isinstance(v, bool) and v >= 0 for v in rect.values()),
                         "image variant rectangle must have nonnegative integer coordinates and dimensions")
            if "width" in variant:
                rect["width"] = variant["width"]
            if "height" in variant:
                rect["height"] = variant["height"]
            _require(all(isinstance(v, int) and not isinstance(v, bool) and v >= 0 for v in rect.values()),
                     "image variant rectangle must have nonnegative integer coordinates and dimensions")
            _require(rect["width"] > 0 and rect["height"] > 0,
                     "image variant dimensions must be nonzero")
            region_id = _stable_region_id(asset["id"], frame, palette_index, rect, variant_identity)
            stp_name = variant.get("stp_png")
            stp_output = output_by_name.get(stp_name) if stp_name else None
            if stp_name:
                _require(stp_output is not None, "STP mask output is missing from media asset hashes")
            palette_words = variant.get("palette_words", variant.get("source_palette_words"))
            palette_status = variant.get("palette_status")
            if palette_status is None:
                palette_status = "available" if palette_words is not None else (
                    "not_applicable" if palette_index is None else "unknown")
            source_clut_vram = variant.get("clut_vram", variant.get("source_clut_vram"))
            preview_kind = variant.get("preview_kind")
            alpha_handling = variant.get("alpha_policy", metadata.get("alpha_policy"))
            stp_handling = variant.get("stp_policy", metadata.get("stp_policy"))
            stp_status = variant.get("stp_status")
            if stp_status is None:
                stp_status = "exported" if stp_output else (
                    "not_applicable" if preview_kind == "source_index_diagnostic" else "not_exported")
            regions.append({
                "region_id": region_id,
                "parent_asset_id": asset["id"],
                "parent_media_status": asset.get("status"),
                "parent_format": asset.get("format", "unknown"),
                "export_status": export.get("status"),
                "source": {"path": source_path, "location": source.get("location"),
                           "sha256": source_hash, "input_sha256": input_hash,
                           "dependencies": variants_source},
                "frame": frame,
                "frame_status": "known" if frame is not None else "unknown",
                "palette_index": palette_index,
                "region": dict(rect),
                "png": {"path": png_output["path"], "sha256": png_hash},
                "clut": {"palette_words": palette_words,
                         "palette_status": palette_status,
                         "vram": source_clut_vram},
                "source_placement": {"image_block": image_block,
                                     "frame_parts": variant.get("parts"),
                                     "clut_vram": source_clut_vram},
                "stp": ({"path": stp_output["path"], "sha256": stp_output["sha256"]}
                        if stp_output else None),
                "stp_status": stp_status,
                "alpha_handling": alpha_handling,
                "stp_handling": stp_handling,
                "original_metadata": metadata,
                "variant_metadata": variant,
                "source_profile": profile or None,
                "screen_context": ({"status": "candidate", "contexts": profile.get("screen_context", []),
                                    "evidence": profile.get("evidence"),
                                    "priority": profile.get("priority"),
                                    "source_role": profile.get("source_role"),
                                    "frame_role": profile.get("frame_roles", {}).get(frame)}
                                   if profile and profile.get("evidence") else
                                   {"status": "unknown", "contexts": [],
                                    "evidence": profile.get("evidence") if profile else None,
                                    "priority": profile.get("priority") if profile else None,
                                    "source_role": profile.get("source_role") if profile else None,
                                    "frame_role": None}),
            })
    region_ids = [region["region_id"] for region in regions]
    _require(len(region_ids) == len(set(region_ids)), "duplicate region identity in media manifest")
    current_hashes = {region["region_id"]: region["png"]["sha256"] for region in regions}
    review_map = _review_map(reviews, current_hashes)
    review_counts = {state: 0 for state in sorted(REVIEW_STATES)}
    for region in regions:
        review = review_map.get(region["region_id"], {
            "state": "unreviewed", "confidence": None, "evidence": None,
            "replacement_work": ["undecided"],
        })
        region.update(review)
        review_counts[review["state"]] += 1
    regions.sort(key=lambda region: (
        region["screen_context"].get("priority") is None,
        region["screen_context"].get("priority") if region["screen_context"].get("priority") is not None else 999,
        region["source"]["path"],
        region["parent_asset_id"],
        (0, region["frame"]) if isinstance(region["frame"], int) else
        (1, region["frame"]) if isinstance(region["frame"], str) else (2, ""),
        region["palette_index"] if isinstance(region["palette_index"], int) else -1,
        region["region_id"],
    ))
    unrepresented.sort(key=lambda item: (item["source"]["path"], item["parent_asset_id"]))
    return {
        "schema": SCHEMA,
        "source_media_schema": media_manifest["schema"],
        "review_states": REVIEW_STATE_MEANINGS,
        "corpus_complete": False,
        "japanese_text_detection_complete": False,
        "graphic_source_coverage_complete": False,
        "artifact_validation": {"status": "metadata_only", "files": 0},
        "regions": regions,
        "unrepresented_sources": unrepresented,
        "counts": {
            "regions": len(regions),
            "unrepresented_sources": len(unrepresented),
            "probable_image_sources_unrepresented": sum(bool(x["probable_image_suffix"]) for x in unrepresented),
            "unreviewed_unresolved_graphics_sources": sum(
                x["coverage_status"] == "unreviewed_unresolved_graphics_source" for x in unrepresented),
            "reviews": review_counts,
        },
    }


def _verify_catalog_files(catalog: dict) -> int:
    """Hash-check referenced PNG and STP files for CLI-generated catalogs."""
    count = 0
    for region in catalog["regions"]:
        for field in ("png", "stp"):
            artifact = region[field]
            if artifact is None:
                continue
            resolved = output_root(REPOSITORY / artifact["path"])
            _require(resolved.is_file(), "referenced image artifact is missing: " + artifact["path"])
            actual = digest(resolved.read_bytes())
            _require(actual == artifact["sha256"], "referenced image artifact hash is stale: " + artifact["path"])
            count += 1
    return count


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--media-manifest", required=True)
    parser.add_argument("--reviews", help="optional local JSON review list")
    parser.add_argument("--output", required=True, help="output beneath ignored .asset-cache")
    args = parser.parse_args(argv)
    try:
        media_manifest = json.loads(Path(args.media_manifest).read_text(encoding="utf-8"))
        reviews = json.loads(Path(args.reviews).read_text(encoding="utf-8")) if args.reviews else None
        catalog = build_catalog(media_manifest, reviews)
        count = _verify_catalog_files(catalog)
        catalog["artifact_validation"] = {
            "status": "sha256_verified" if count else "metadata_only", "files": count,
        }
        result = write_manifest(args.output, catalog)
        print("%d visual regions, %d unrepresented media sources; corpus completeness unknown" % (
            catalog["counts"]["regions"], catalog["counts"]["unrepresented_sources"]))
        print("manifest %s sha256 %s" % (result["path"], result["sha256"]))
        return 0
    except (OSError, json.JSONDecodeError, CatalogError, ValueError) as error:
        print("visual_text.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
