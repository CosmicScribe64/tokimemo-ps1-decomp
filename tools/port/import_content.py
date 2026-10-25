"""Build immutable tokimemo.port-content.v1 packs from reviewed media exports (T-29003).

This bounded adapter imports only reviewed static PNG previews for four
source-proved prologue NBN inputs. It does not establish playback behavior.
Run with the repository's Docker wrapper; original input and source manifest
are read-only and all publication stays under ignored .asset-cache/port/.
"""
import argparse
import hashlib
import json
import os
import shutil
import sys
import tempfile
from pathlib import Path

def _repository_root():
    """Find the checkout from either tools/port or an isolated candidate copy."""
    for parent in Path(__file__).resolve().parents:
        if (parent / "tools" / "assets" / "input.py").is_file():
            return parent
    raise RuntimeError("cannot locate repository root containing tools/assets/input.py")


ROOT = _repository_root()
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tools.assets.input import open_input  # noqa: E402
from validate_content import SCHEMA, SCHEMA_VERSION, validate_pack  # noqa: E402

SELECTED = {
    "CDROM/PROLODIR/CUT_05.NBN", "CDROM/PROLODIR/CUT_07.NBN",
    "CDROM/PROLODIR/CUT_08.NBN", "CDROM/PROLODIR/CUT_09.NBN",
}
REVIEWED_MEDIA_MANIFEST_SHA256 = "26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde"
REVIEWED_VISUAL_CATALOG_SHA256 = "d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6"
REVIEWED_CELL_MAP_SHA256 = "5eb43589bb9722a2792c6d8c880141c36a7aa01fcc14fdfbe82be6de07282209"
MAX_COMPONENT_BYTES = 8 * 1024 * 1024


def sha(data):
    return hashlib.sha256(data).hexdigest()


def import_pack(input_path, media_manifest_path, output_path):
    """Verify the accepted media manifest against fresh original reads, then publish atomically."""
    output_path = Path(output_path).absolute()
    cache = ROOT / ".asset-cache" / "port"
    if (ROOT / ".asset-cache").is_symlink():
        raise ValueError(".asset-cache must not be a symlink")
    if cache not in output_path.parents:
        raise ValueError("output must be beneath .asset-cache/port/")
    relative_output = output_path.relative_to(cache)
    if not relative_output.parts or any(part in ("", ".", "..") for part in relative_output.parts):
        raise ValueError("output path must name a safe pack directory")
    current = ROOT / ".asset-cache"
    for part in ("port",) + relative_output.parts[:-1]:
        current = current / part
        if current.is_symlink():
            raise ValueError("output parent path cannot contain symlinks")
    if output_path.exists():
        raise ValueError("pack destination already exists; immutable packs are never overwritten")
    media_manifest_path = Path(media_manifest_path).absolute()
    if media_manifest_path.name != "manifest.json":
        raise ValueError("reviewed media manifest filename must be manifest.json")
    current = Path(media_manifest_path.anchor)
    for part in media_manifest_path.parts[1:]:
        current = current / part
        if current.is_symlink():
            raise ValueError("reviewed media manifest path cannot contain symlinks")
    if not media_manifest_path.is_file():
        raise ValueError("reviewed media manifest is missing")
    if sha(media_manifest_path.read_bytes()) != REVIEWED_MEDIA_MANIFEST_SHA256:
        raise ValueError("media manifest is not the reviewed V4 prologue publication")
    media_root = media_manifest_path.parent
    catalog_path = media_root / "visual-catalog.json"
    cell_map_path = media_root / "nbn-cell-review-map.json"
    if not catalog_path.is_file() or sha(catalog_path.read_bytes()) != REVIEWED_VISUAL_CATALOG_SHA256:
        raise ValueError("visual catalog missing or differs from reviewed V4 catalog")
    if not cell_map_path.is_file() or sha(cell_map_path.read_bytes()) != REVIEWED_CELL_MAP_SHA256:
        raise ValueError("canonical NBN cell map missing or differs from reviewed V4 map")
    media = json.loads(media_manifest_path.read_text(encoding="utf-8"))
    catalog = json.loads(catalog_path.read_text(encoding="utf-8"))
    cell_map = json.loads(cell_map_path.read_text(encoding="utf-8"))
    if media.get("schema") != "tokimemo.media.v1":
        raise ValueError("unsupported source media manifest schema")
    if media.get("input", {}).get("read_only") is not True:
        raise ValueError("source media manifest does not identify a read-only input")
    with open_input(input_path) as source:
        info = source.describe()
        if info.get("source_sha256") != media.get("input", {}).get("source_sha256"):
            raise ValueError("current original input SHA-256 differs from reviewed media manifest")
        entries = {entry["path"]: entry for entry in source.entries()}
        by_source = {a.get("source", {}).get("path"): a for a in media.get("assets", [])}
        mapped_cells = {}
        for mapping in cell_map.get("entries", []):
            source_path = mapping.get("source_path")
            cell_index = mapping.get("source_cell", {}).get("cell_index")
            if source_path in SELECTED:
                key = (source_path, cell_index)
                if key in mapped_cells:
                    raise ValueError(f"duplicate canonical NBN cell mapping: {key}")
                mapped_cells[key] = mapping
        if len(mapped_cells) != 24:
            raise ValueError(f"reviewed canonical map has {len(mapped_cells)} selected cells; expected 24")
        catalog_by_id = {region.get("region_id"): region for region in catalog.get("regions", [])}
        if len(catalog_by_id) != len(catalog.get("regions", [])):
            raise ValueError("visual catalog contains duplicate region identities")
        chosen = []
        for path in sorted(SELECTED):
            entry, asset = entries.get(path), by_source.get(path)
            if not entry or entry.get("kind") != "file" or not asset:
                raise ValueError(f"required reviewed source entry unavailable: {path}")
            if asset.get("status") != "exported" or asset.get("format") != "tokimemo-prologue-native-cells":
                raise ValueError(f"source entry lacks reviewed prologue native-cell export: {path}")
            if asset["source"].get("size") != entry["size"]:
                raise ValueError(f"reviewed source size mismatch: {path}")
            original = source.read(entry)
            if len(original) != entry["size"] or sha(original) != asset["source"].get("sha256"):
                raise ValueError(f"fresh original byte verification failed: {path}")
            chosen.append((path, entry, asset))
        stage_parent = cache
        stage_parent.mkdir(parents=True, exist_ok=True)
        if stage_parent.is_symlink():
            raise ValueError(".asset-cache/port must not be a symlink")
        output_path.parent.mkdir(parents=True, exist_ok=True)
        current = ROOT / ".asset-cache"
        for part in ("port",) + relative_output.parts[:-1]:
            current = current / part
            if current.is_symlink():
                raise ValueError("output parent path cannot contain symlinks")
        stage = Path(tempfile.mkdtemp(prefix=".staging-", dir=stage_parent))
        try:
            components = []
            for path, entry, asset in chosen:
                source_id = asset["id"]
                for output in asset.get("outputs", []):
                    if not output["path"].endswith(".png") or ".clut-bit15.png" in output["path"]:
                        continue
                    rel = Path(output["path"])
                    if rel.is_absolute() or ".." in rel.parts or rel.parts[:3] != (".asset-cache", "media", "nbn-prologue-published-v4-verified") or any(p in ("", ".") for p in rel.parts):
                        raise ValueError("media output locator escapes export root")
                    source_png = media_root.joinpath(*rel.parts[3:])
                    source_rel = source_png.relative_to(media_root)
                    current = media_root
                    for part in source_rel.parts:
                        current = current / part
                        if current.is_symlink():
                            raise ValueError(f"media output path contains symlink: {rel}")
                    if not source_png.is_file() or source_png.is_symlink():
                        raise ValueError(f"reviewed PNG missing or symlinked: {rel}")
                    data = source_png.read_bytes()
                    if len(data) > MAX_COMPONENT_BYTES or len(data) != output["size"] or sha(data) != output["sha256"]:
                        raise ValueError(f"reviewed PNG size/hash mismatch: {rel}")
                    try:
                        from tools.assets.visual_text_equivalence import _decode_png
                        decoded_width, decoded_height, _pixels = _decode_png(data)
                    except Exception as exc:
                        raise ValueError(f"reviewed PNG is truncated or invalid: {rel}: {exc}") from exc
                    filename = f"{source_id}-{Path(output['path']).name}"
                    target = stage / "payloads" / filename
                    target.parent.mkdir(parents=True, exist_ok=True)
                    target.write_bytes(data)
                    image_meta = next((image for image in asset["export"]["images"] if image.get("png") == Path(output["path"]).name), None)
                    if not image_meta or image_meta.get("preview_kind") != "native_uv_mapped_texture_cell":
                        raise ValueError(f"reviewed native cell metadata missing for {path}")
                    cell_index = image_meta.get("cell_index")
                    cell_record = mapped_cells.get((path, cell_index))
                    if not cell_record:
                        raise ValueError(f"reviewed canonical region missing for {path} cell {cell_index}")
                    canonical_region_id = cell_record.get("evidence", {}).get("canonical_region_id")
                    if (not isinstance(canonical_region_id, str) or not canonical_region_id.startswith("region-")
                            or canonical_region_id != cell_record.get("region_id")):
                        raise ValueError(f"canonical region identity disagreement for {path} cell {cell_index}")
                    catalog_region = catalog_by_id.get(canonical_region_id)
                    if not catalog_region or catalog_region.get("png", {}).get("sha256") != output["sha256"]:
                        raise ValueError(f"canonical region does not resolve to this PNG in visual catalog: {canonical_region_id}")
                    if catalog_region.get("source", {}).get("path") != path or catalog_region.get("source", {}).get("sha256") != entry["sha256"]:
                        raise ValueError(f"visual catalog original source cross-reference mismatch: {canonical_region_id}")
                    catalog_variant = catalog_region.get("variant_metadata", {})
                    if catalog_variant.get("cell_index") != cell_index or (decoded_width, decoded_height) != (image_meta["width"], image_meta["height"]):
                        raise ValueError(f"canonical cell geometry mismatch: {canonical_region_id}")
                    if (cell_record.get("source_cell", {}).get("dimensions") != [image_meta["width"], image_meta["height"]]
                            or cell_record.get("source_cell", {}).get("source_spans") != image_meta["source_spans"]
                            or cell_record.get("palette", {}).get("sha256") != image_meta["palette_source_sha256"]
                            or cell_record.get("palette", {}).get("offset") != image_meta["palette_source_offset"]):
                        raise ValueError(f"canonical cell map geometry/palette reference mismatch: {canonical_region_id}")
                    use_alias = {"caller": cell_record.get("context", {}).get("caller"),
                                 "screen_context": cell_record.get("context", {}).get("screen_context")}
                    if not all(isinstance(value, str) and value for value in use_alias.values()):
                        raise ValueError(f"reviewed use-site alias missing: {canonical_region_id}")
                    components.append({
                        "id": canonical_region_id,
                        "version": 1, "kind": "png_preview", "status": "available",
                        "capabilities": ["prologue.nbn_native_cells.static_preview"], "source": {
                            "path": path, "format": "mode2-iso-file", "size_bytes": entry["size"], "sha256": entry["sha256"],
                            "locator": entry.get("location", {})},
                        "payload": f"payloads/{filename}", "payload_size_bytes": len(data),
                        "payload_sha256": sha(data), "metadata": {
                            "canonical_region_id": canonical_region_id,
                            "cell_index": image_meta["cell_index"],
                            "source_variant": image_meta["variant"],
                            "palette_variant": {"palette_index": catalog_region.get("palette_index"),
                                "source_sha256": image_meta["palette_source_sha256"],
                                "source_offset": image_meta["palette_source_offset"],
                                "status": image_meta["palette_status"]},
                            "use_aliases": [use_alias],
                            "width": image_meta["width"],
                            "height": image_meta["height"], "grid_column": image_meta["grid_column"],
                            "grid_row": image_meta["grid_row"], "source_texture_dimensions": image_meta["source_texture_dimensions"],
                            "source_spans": image_meta["source_spans"], "unused_source_spans_for_cell": image_meta["unused_source_spans_for_cell"],
                            "clut_bit15_status": image_meta["clut_bit15_status"],
                            "static_only": True, "animation_composition_proven": False}})
            manifest = {
                "schema": SCHEMA, "schema_version": SCHEMA_VERSION,
                "release_profile": "original-ja-ps-the-best",
                "source": {"format": info["format"], "sha256": info["source_sha256"],
                           "size_bytes": Path(input_path).stat().st_size if Path(input_path).is_file() else sum(e.get("size", 0) for e in source.entries() if e.get("kind") == "file"), "read_only": True},
                "created_by": {"reader": "tools.assets.input.open_input", "media_schema": media["schema"],
                               "media_manifest_sha256": REVIEWED_MEDIA_MANIFEST_SHA256,
                               "visual_catalog_sha256": REVIEWED_VISUAL_CATALOG_SHA256,
                               "cell_map_sha256": REVIEWED_CELL_MAP_SHA256,
                               "input_reader_source_sha256": sha((ROOT / "tools/assets/input.py").read_bytes()),
                               "importer_source_sha256": sha(Path(__file__).read_bytes()),
                               "validator_source_sha256": sha(Path(__file__).with_name("validate_content.py").read_bytes()),
                               "reviewed_component_schema": "tokimemo-prologue-nbn-source-profile.v2"},
                "components": components,
                "route_capabilities": {"available": ["prologue.nbn_native_cells.static_preview"],
                    "unavailable": ["prologue.animation_composition", "school_day.route", "audio.playback", "dialogue.runtime"]}}
            (stage / "manifest.json").write_text(json.dumps(manifest, sort_keys=True, indent=2) + "\n", encoding="utf-8")
            validate_pack(stage)
            os.rename(stage, output_path)
        except Exception:
            shutil.rmtree(stage, ignore_errors=True)
            raise
    return output_path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="Original source input, opened read-only")
    parser.add_argument("--media-manifest", required=True, help="Reviewed media.v1 manifest")
    parser.add_argument("--output", required=True, help="New directory under .asset-cache/port")
    args = parser.parse_args()
    print(import_pack(args.input, args.media_manifest, args.output))


if __name__ == "__main__":
    main()
