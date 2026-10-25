"""Validate the bounded tokimemo.port-content.v1 inventory contract (T-29003)."""
import hashlib
import argparse
import json
import re
import sys
from pathlib import Path, PurePosixPath
for _parent in Path(__file__).resolve().parents:
    if (_parent / "tools" / "assets" / "input.py").is_file():
        sys.path.insert(0, str(_parent))
        break
from tools.assets.visual_text_equivalence import EquivalenceError, _decode_png

SCHEMA = "tokimemo.port-content.v1"
SCHEMA_VERSION = 1
SAFE_ID = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,127}$")
HASH = re.compile(r"^[0-9a-f]{64}$")
MAX_MANIFEST_BYTES = 16 * 1024 * 1024
MAX_COMPONENTS = 100_000
MAX_COMPONENT_BYTES = 8 * 1024 * 1024
TOP_LEVEL_KEYS = {"schema", "schema_version", "release_profile", "source", "created_by", "components", "route_capabilities"}
SOURCE_KEYS = {"format", "sha256", "size_bytes", "read_only"}
CREATOR_KEYS = {"reader", "media_schema", "reviewed_component_schema", "media_manifest_sha256", "visual_catalog_sha256", "cell_map_sha256", "input_reader_source_sha256", "importer_source_sha256", "validator_source_sha256"}
COMPONENT_BASE_KEYS = {"id", "version", "kind", "status", "capabilities", "source"}
COMPONENT_PREVIEW_KEYS = COMPONENT_BASE_KEYS | {"payload", "payload_size_bytes", "payload_sha256", "metadata"}
COMPONENT_SOURCE_KEYS = {"path", "format", "size_bytes", "sha256", "locator"}
PREVIEW_METADATA_KEYS = {"canonical_region_id", "cell_index", "source_variant", "palette_variant", "use_aliases", "width", "height", "grid_column", "grid_row", "source_texture_dimensions", "source_spans", "unused_source_spans_for_cell", "clut_bit15_status", "static_only", "animation_composition_proven"}
REVIEWED_MEDIA_MANIFEST_SHA256 = "26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde"
REVIEWED_VISUAL_CATALOG_SHA256 = "d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6"
REVIEWED_CELL_MAP_SHA256 = "5eb43589bb9722a2792c6d8c880141c36a7aa01fcc14fdfbe82be6de07282209"
CANONICAL_CELLS_SHA256 = "d2c91b1b8ea63506e751a2a4b64cce39586c39c448610cb5cff00733fba36be2"
REVIEWED_INPUT_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"


class ContentError(ValueError):
    """Pack is malformed, stale, unsafe, or missing an explicit capability."""


def _need(condition, message):
    if not condition:
        raise ContentError(message)


def _relative(value, label):
    _need(isinstance(value, str) and value and "\\" not in value, f"{label} must be a safe relative path")
    _need(not value.startswith("/") and all(part not in ("", ".", "..") for part in value.split("/")), f"{label} is not canonical")
    path = PurePosixPath(value)
    _need(not path.is_absolute() and all(part not in ("", ".", "..") for part in path.parts), f"{label} escapes pack root")
    return path


def validate_manifest(manifest):
    _need(isinstance(manifest, dict), "manifest must be an object")
    _need(set(manifest) == TOP_LEVEL_KEYS, "manifest fields do not match the v1 contract")
    _need(manifest.get("schema") == SCHEMA, f"unsupported content schema {manifest.get('schema')!r}; expected {SCHEMA}")
    _need(type(manifest.get("schema_version")) is int and manifest["schema_version"] == SCHEMA_VERSION, f"unsupported content schema_version {manifest.get('schema_version')!r}")
    _need(manifest.get("release_profile") == "original-ja-ps-the-best", "unsupported release_profile")
    source = manifest.get("source")
    _need(isinstance(source, dict), "source provenance is required")
    _need(set(source) == SOURCE_KEYS and source.get("format") == "mode2-2352", "source fields or format do not match the v1 contract")
    _need(HASH.fullmatch(str(source.get("sha256", ""))) is not None, "source.sha256 must be lowercase SHA-256")
    _need(source["sha256"] == REVIEWED_INPUT_SHA256, "source SHA-256 is not the reviewed original data track")
    _need(type(source.get("size_bytes")) is int and source["size_bytes"] >= 0, "source.size_bytes must be nonnegative")
    _need(source.get("read_only") is True, "source must be marked read_only")
    creator = manifest.get("created_by")
    _need(isinstance(creator, dict) and set(creator) == CREATOR_KEYS and creator.get("reader") == "tools.assets.input.open_input", "created_by reader provenance is missing or unknown")
    for key in ("media_manifest_sha256", "visual_catalog_sha256", "cell_map_sha256", "input_reader_source_sha256", "importer_source_sha256", "validator_source_sha256"):
        _need(HASH.fullmatch(str(creator.get(key, ""))) is not None, f"created_by.{key} must be lowercase SHA-256")
    _need(creator["media_manifest_sha256"] == REVIEWED_MEDIA_MANIFEST_SHA256, "created_by media manifest is not the accepted publication")
    _need(creator["visual_catalog_sha256"] == REVIEWED_VISUAL_CATALOG_SHA256, "created_by visual catalog is not the reviewed catalog")
    _need(creator["cell_map_sha256"] == REVIEWED_CELL_MAP_SHA256, "created_by canonical cell map is not the accepted map")
    _need(creator.get("media_schema") == "tokimemo.media.v1" and creator.get("reviewed_component_schema") == "tokimemo-prologue-nbn-source-profile.v2", "created_by reviewed component schema provenance is unsupported")
    components = manifest.get("components")
    _need(isinstance(components, list) and len(components) <= MAX_COMPONENTS, "components must be a bounded array")
    seen = set()
    for component in components:
        _need(isinstance(component, dict), "component must be an object")
        ident = component.get("id")
        _need(isinstance(ident, str) and SAFE_ID.fullmatch(ident), "component id is unsafe")
        _need(ident not in seen, f"duplicate component id {ident}")
        seen.add(ident)
        _need(type(component.get("version")) is int and component["version"] == 1, f"component {ident} has invalid version")
        _need(component.get("kind") in ("inventory_entry", "png_preview"), f"component {ident} has unsupported kind")
        if component.get("kind") == "png_preview":
            _need(set(component) == COMPONENT_PREVIEW_KEYS, f"component {ident} fields do not match the v1 contract")
        else:
            _need(set(component) == COMPONENT_BASE_KEYS, f"component {ident} fields do not match the v1 contract")
        _need(component.get("status") in ("available", "unsupported"), f"component {ident} has invalid status")
        loc = component.get("source")
        _need(isinstance(loc, dict), f"component {ident} source locator is required")
        _need(set(loc) == COMPONENT_SOURCE_KEYS and loc.get("format") == "mode2-iso-file", f"component {ident} source fields or format are invalid")
        _relative(loc.get("path"), f"component {ident} source path")
        _need(type(loc.get("size_bytes")) is int and loc["size_bytes"] >= 0, f"component {ident} size is invalid")
        _need(HASH.fullmatch(str(loc.get("sha256", ""))) is not None, f"component {ident} source sha256 is invalid")
        component_caps = component.get("capabilities")
        _need(isinstance(component_caps, list) and all(isinstance(c, str) and c for c in component_caps) and len(set(component_caps)) == len(component_caps), f"component {ident} capabilities must be unique nonempty strings")
        _need((component["status"] == "available") == (component["kind"] == "png_preview"), f"component {ident} has inconsistent kind/status")
        _need(component["status"] != "unsupported" or not component_caps, f"unsupported component {ident} cannot advertise capabilities")
        _need(component["kind"] != "png_preview" or bool(component_caps), f"preview component {ident} must declare a capability")
        if component["kind"] == "png_preview":
            meta = component.get("metadata")
            _need(isinstance(meta, dict) and set(meta) == PREVIEW_METADATA_KEYS, f"component {ident} metadata fields do not match the v1 contract")
            canonical = _canonical_cell(loc["path"], meta.get("cell_index"))
            _need(canonical is not None and meta.get("canonical_region_id") == canonical["canonical_region_id"] and ident == canonical["canonical_region_id"], f"component {ident} canonical cell identity is invalid")
            _need(loc.get("sha256") == canonical["source_sha256"] and component.get("payload_sha256") == canonical["png_sha256"], f"component {ident} source or catalog PNG identity is invalid")
            palette = meta.get("palette_variant")
            _need(isinstance(palette, dict) and palette == {"palette_index": canonical["palette_index"], "source_sha256": canonical["palette_source_sha256"], "source_offset": canonical["palette_source_offset"], "status": canonical["palette_status"]}, f"component {ident} palette variant is invalid")
            _need(meta.get("source_variant") == canonical["source_variant"], f"component {ident} source variant is invalid")
            _need(meta.get("use_aliases") == [{"caller": canonical["caller"], "screen_context": canonical["screen_context"]}], f"component {ident} use-site alias is invalid")
            _need(meta.get("width") == canonical["width"] and meta.get("height") == canonical["height"], f"component {ident} dimensions do not match canonical cell")
            for key in ("grid_column", "grid_row", "source_texture_dimensions", "source_spans", "unused_source_spans_for_cell", "clut_bit15_status"):
                _need(meta.get(key) == canonical[key], f"component {ident} canonical {key} mismatch")
            _need(meta.get("static_only") is True and meta.get("animation_composition_proven") is False, f"component {ident} preview scope metadata is invalid")
            _need(isinstance(meta.get("use_aliases"), list) and len(meta["use_aliases"]) > 0, f"component {ident} use aliases are required")
            _relative(component.get("payload"), f"component {ident} payload")
            _need(str(component["payload"]).lower().endswith(".png"), f"component {ident} preview payload must be PNG")
            _need(type(component.get("payload_size_bytes")) is int and 0 <= component["payload_size_bytes"] <= MAX_COMPONENT_BYTES, f"component {ident} payload size is invalid")
            _need(HASH.fullmatch(str(component.get("payload_sha256", ""))) is not None, f"component {ident} payload sha256 is invalid")
    caps = manifest.get("route_capabilities")
    _need(isinstance(caps, dict) and isinstance(caps.get("available"), list) and isinstance(caps.get("unavailable"), list), "route_capabilities needs available and unavailable arrays")
    available, unavailable = caps["available"], caps["unavailable"]
    _need(all(isinstance(c, str) and c for c in available + unavailable), "route capabilities must be nonempty strings")
    _need(len(set(available)) == len(available) and len(set(unavailable)) == len(unavailable) and set(available).isdisjoint(unavailable), "route capabilities must be unique and disjoint")
    component_capabilities = {cap for component in components if component["status"] == "available" for cap in component["capabilities"]}
    _need(set(available) <= component_capabilities, "available route capability has no supporting component")
    return manifest


def validate_pack(root):
    """Validate manifest and any referenced regular payload, rejecting symlink traversal."""
    root = Path(root)
    _need(root.is_dir() and not root.is_symlink(), "pack root must be a real directory")
    path = root / "manifest.json"
    _need(path.is_file() and not path.is_symlink(), "pack manifest.json is missing or a symlink")
    _need(path.stat().st_size <= MAX_MANIFEST_BYTES, "manifest exceeds 16 MiB limit")
    try:
        manifest = validate_manifest(json.loads(path.read_text(encoding="utf-8")))
    except (UnicodeError, json.JSONDecodeError) as exc:
        raise ContentError(f"invalid manifest JSON: {exc}") from exc
    for component in manifest["components"]:
        payload_name = component.get("payload")
        if payload_name is None:
            continue
        rel = _relative(payload_name, "payload")
        current = root
        for part in rel.parts:
            current = current / part
            _need(not current.is_symlink(), f"payload symlink is forbidden: {payload_name}")
        _need(current.is_file(), f"payload is missing: {payload_name}")
        _need(current.stat().st_size <= MAX_COMPONENT_BYTES, f"payload exceeds 8 MiB limit: {payload_name}")
        data = current.read_bytes()
        _need(len(data) == component.get("payload_size_bytes"), f"payload size mismatch: {payload_name}")
        _need(hashlib.sha256(data).hexdigest() == component.get("payload_sha256"), f"payload SHA-256 mismatch: {payload_name}")
        try:
            width, height, _rgba = _decode_png(data)
        except EquivalenceError as exc:
            raise ContentError(f"invalid PNG preview {payload_name}: {exc}") from exc
        _need(width == component["metadata"]["width"] and height == component["metadata"]["height"], f"PNG geometry mismatch: {payload_name}")
    return manifest


def _canonical_cell(source_path, cell_index):
    """Return one reviewed physical NBN cell identity, never a guessed ID."""
    if not isinstance(cell_index, int) or isinstance(cell_index, bool):
        return None
    repo_root = next(parent for parent in Path(__file__).resolve().parents if (parent / "port" / "godot" / "schemas" / "canonical_prologue_nbn_cells.v1.json").is_file())
    path = repo_root / "port" / "godot" / "schemas" / "canonical_prologue_nbn_cells.v1.json"
    data = path.read_bytes()
    _need(hashlib.sha256(data).hexdigest() == CANONICAL_CELLS_SHA256, "canonical cell allowlist hash mismatch")
    mapping = json.loads(data)
    _need(mapping.get("schema") == "tokimemo.prologue-nbn-canonical-cells.v1", "unsupported canonical cell allowlist schema")
    return mapping.get("entries", {}).get(f"{source_path}#{cell_index}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pack", help="External pack directory")
    args = parser.parse_args()
    manifest = validate_pack(args.pack)
    print(f"valid {manifest['schema']}: {len(manifest['components'])} components")


if __name__ == "__main__":
    main()
