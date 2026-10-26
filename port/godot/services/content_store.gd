class_name ContentStore
extends RefCounted
## Bounded data-only reader for tokimemo.port-content.v1 packs.

const SCHEMA := "tokimemo.port-content.v1"
const MAX_MANIFEST_BYTES := 16 * 1024 * 1024
const MAX_COMPONENTS := 100000
const REVIEWED_INPUT_SHA256 := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const REVIEWED_MEDIA_MANIFEST_SHA256 := "26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde"
const REVIEWED_VISUAL_CATALOG_SHA256 := "d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6"
const REVIEWED_CELL_MAP_SHA256 := "5eb43589bb9722a2792c6d8c880141c36a7aa01fcc14fdfbe82be6de07282209"
const CANONICAL_CELLS_SHA256 := "d2c91b1b8ea63506e751a2a4b64cce39586c39c448610cb5cff00733fba36be2"
const CANONICAL_CELLS_PATH := "res://schemas/canonical_prologue_nbn_cells.v1.json"
const BASE_COMPONENT_KEYS := ["id", "version", "kind", "status", "capabilities", "source"]
const PREVIEW_KEYS := ["id", "version", "kind", "status", "capabilities", "source", "payload", "payload_size_bytes", "payload_sha256", "metadata"]
const SOURCE_KEYS := ["format", "sha256", "size_bytes", "read_only"]
const COMPONENT_SOURCE_KEYS := ["path", "format", "size_bytes", "sha256", "locator"]
const PREVIEW_METADATA_KEYS := ["canonical_region_id", "cell_index", "source_variant", "palette_variant", "use_aliases", "width", "height", "grid_column", "grid_row", "source_texture_dimensions", "source_spans", "unused_source_spans_for_cell", "clut_bit15_status", "static_only", "animation_composition_proven"]

var _root := ""
var _components: Dictionary = {}
var _available: Array = []
var _unavailable: Array = []
var _error := ""

func open_pack(root: String) -> bool:
	_root = ""
	_components.clear()
	_available.clear()
	_unavailable.clear()
	_error = ""
	if not _no_root_symlink(root):
		return _fail("pack root path cannot contain symlinks")
	var manifest_path := root.path_join("manifest.json")
	if _is_link(manifest_path) or not FileAccess.file_exists(manifest_path):
		return _fail("manifest.json is missing or a symlink")
	if FileAccess.get_size(manifest_path) > MAX_MANIFEST_BYTES:
		return _fail("manifest exceeds 16 MiB limit")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary:
		return _fail("manifest JSON must be an object")
	if parsed.size() != 7 or not parsed.has("release_profile") or not parsed.has("created_by"):
		return _fail("manifest fields do not match the v1 contract")
	if parsed.get("schema") != SCHEMA:
		return _fail("unsupported content schema; expected " + SCHEMA)
	if not _is_exact_integer(parsed.get("schema_version"), 1) or int(parsed.get("schema_version")) != 1:
		return _fail("unsupported content schema_version")
	if parsed.get("release_profile") != "original-ja-ps-the-best":
		return _fail("unsupported release_profile")
	var source: Variant = parsed.get("source")
	if not source is Dictionary or source.get("read_only") != true or not _safe_sha(str(source.get("sha256", ""))):
		return _fail("invalid original source provenance")
	if not _has_exact_keys(source, SOURCE_KEYS) or source.get("format") != "mode2-2352":
		return _fail("source fields or format do not match the v1 contract")
	if source.get("sha256") != REVIEWED_INPUT_SHA256:
		return _fail("source is not the reviewed original data track")
	if not _is_exact_integer(source.get("size_bytes"), 9223372036854775807):
		return _fail("invalid original source size")
	var creator: Variant = parsed.get("created_by")
	var creator_keys := ["reader", "media_schema", "reviewed_component_schema", "media_manifest_sha256", "visual_catalog_sha256", "cell_map_sha256", "input_reader_source_sha256", "importer_source_sha256", "validator_source_sha256"]
	if not creator is Dictionary or not _has_exact_keys(creator, creator_keys) or creator.get("reader") != "tools.assets.input.open_input":
		return _fail("created_by reader provenance is missing or unknown")
	for key in ["media_manifest_sha256", "visual_catalog_sha256", "cell_map_sha256", "input_reader_source_sha256", "importer_source_sha256", "validator_source_sha256"]:
		if not _safe_sha(str(creator.get(key, ""))):
			return _fail("created_by hash provenance is invalid: " + key)
	if creator.get("media_schema") != "tokimemo.media.v1" or creator.get("reviewed_component_schema") != "tokimemo-prologue-nbn-source-profile.v2":
		return _fail("created_by reviewed component schema is unsupported")
	if creator.get("media_manifest_sha256") != REVIEWED_MEDIA_MANIFEST_SHA256 or creator.get("visual_catalog_sha256") != REVIEWED_VISUAL_CATALOG_SHA256 or creator.get("cell_map_sha256") != REVIEWED_CELL_MAP_SHA256:
		return _fail("created_by does not identify the accepted media publication")
	var canonical_map := _canonical_map()
	if canonical_map.is_empty():
		return _fail("canonical region map is missing or invalid")
	var components: Variant = parsed.get("components")
	if not components is Array or components.size() > MAX_COMPONENTS:
		return _fail("components must be a bounded array")
	_root = root
	for item in components:
		if not item is Dictionary:
			return _fail("component must be an object")
		var ident := str(item.get("id", ""))
		if not _safe_id(ident) or _components.has(ident):
			return _fail("unsafe or duplicate component id")
		if item.get("kind") != "png_preview" and item.get("kind") != "inventory_entry":
			return _fail("unsupported component kind: " + ident)
		var expected_keys := PREVIEW_KEYS if item.get("kind") == "png_preview" else BASE_COMPONENT_KEYS
		if not _has_exact_keys(item, expected_keys):
			return _fail("component fields do not match the v1 contract: " + ident)
		if not _is_exact_integer(item.get("version"), 1) or int(item.get("version")) != 1:
			return _fail("unsupported component version: " + ident)
		if item.get("status") != "available" and item.get("status") != "unsupported":
			return _fail("invalid component status: " + ident)
		var source_locator: Variant = item.get("source")
		if not source_locator is Dictionary or not _has_exact_keys(source_locator, COMPONENT_SOURCE_KEYS) or not _safe_relative(str(source_locator.get("path", ""))):
			return _fail("component source fields or format are invalid: " + ident)
		if source_locator.get("format") != "mode2-iso-file":
			return _fail("unsupported component source format: " + ident)
		if not _is_exact_integer(source_locator.get("size_bytes"), 9223372036854775807):
			return _fail("invalid source byte size: " + ident)
		if not _safe_sha(str(source_locator.get("sha256", ""))):
			return _fail("invalid source byte hash: " + ident)
		if item.get("kind") == "inventory_entry" and item.get("status") != "unsupported":
			return _fail("inventory entry status must be unsupported: " + ident)
		var item_caps: Variant = item.get("capabilities")
		if not item_caps is Array:
			return _fail("component capabilities must be an array: " + ident)
		if item.get("kind") == "png_preview" and (item.get("status") != "available" or item_caps.is_empty()):
			return _fail("preview status/capability is invalid: " + ident)
		if item.get("status") == "unsupported" and not item_caps.is_empty():
			return _fail("unsupported inventory cannot advertise capabilities: " + ident)
		var item_cap_set: Dictionary = {}
		for cap in item_caps:
			if not cap is String or cap.is_empty() or item_cap_set.has(cap):
				return _fail("component capabilities must be nonempty strings: " + ident)
			item_cap_set[cap] = true
		if item.get("kind") == "png_preview":
			var metadata: Variant = item.get("metadata")
			if not metadata is Dictionary or not _has_exact_keys(metadata, PREVIEW_METADATA_KEYS):
				return _fail("preview metadata fields do not match the v1 contract: " + ident)
			if not _is_exact_integer(metadata.get("cell_index"), 5):
				return _fail("canonical cell index is not an exact bounded integer: " + ident)
			var canonical: Variant = canonical_map.get("entries", {}).get(str(source_locator["path"]) + "#" + str(int(metadata.get("cell_index"))))
			if not canonical is Dictionary or not _matches_canonical(item, source_locator, metadata, canonical):
				return _fail("canonical region or palette/use alias identity is invalid: " + ident)
			if not _check_payload(item, canonical):
				return false
		_components[ident] = item.duplicate(true)
	var caps: Variant = parsed.get("route_capabilities")
	if not caps is Dictionary or not caps.get("available") is Array or not caps.get("unavailable") is Array:
		return _fail("route_capabilities requires available and unavailable arrays")
	var route_cap_set: Dictionary = {}
	for capability in caps["available"]:
		if not capability is String or capability.is_empty() or caps["unavailable"].has(capability) or route_cap_set.has(capability):
			return _fail("route capabilities must be nonempty and disjoint")
		route_cap_set[capability] = true
		var backed := false
		for item in components:
			if item.get("status") == "available" and item.get("capabilities", []).has(capability):
				backed = true
		if not backed:
			return _fail("available route capability has no supporting component")
	for capability in caps["unavailable"]:
		if not capability is String or capability.is_empty() or route_cap_set.has(capability):
			return _fail("route capabilities must be nonempty strings")
		route_cap_set[capability] = true
	_available = caps["available"].duplicate()
	_unavailable = caps["unavailable"].duplicate()
	return true

func has_capability(capability: String) -> bool:
	return _available.has(capability)

func diagnostic_for(capability: String) -> String:
	if has_capability(capability):
		return ""
	if _unavailable.has(capability):
		return "content capability unavailable: " + capability
	return "content capability not declared: " + capability

func get_component(ident: String) -> Dictionary:
	if not _components.has(ident):
		return {}
	return _components[ident].duplicate(true)

func error_message() -> String:
	return _error

func _check_payload(item: Dictionary, canonical: Dictionary) -> bool:
	var relative := str(item.get("payload", ""))
	if not _safe_relative(relative) or not relative.to_lower().ends_with(".png"):
		return _fail("unsafe payload path")
	var path := _root_path_for(relative)
	if not _no_symlink_segments(relative) or not FileAccess.file_exists(path):
		return _fail("payload missing or symlinked: " + relative)
	var size_value: Variant = item.get("payload_size_bytes")
	if not _is_exact_integer(size_value, 8 * 1024 * 1024):
		return _fail("payload size invalid: " + relative)
	var size := int(size_value)
	if FileAccess.get_size(path) != size:
		return _fail("payload size invalid: " + relative)
	if FileAccess.get_sha256(path) != str(item.get("payload_sha256", "")):
		return _fail("payload SHA-256 mismatch: " + relative)
	var payload := FileAccess.get_file_as_bytes(path)
	var png_error := validate_png_preview(payload, int(canonical["width"]), int(canonical["height"]))
	if not png_error.is_empty():
		return _fail(png_error + ": " + relative)
	return true

func validate_png_preview(payload: PackedByteArray, expected_width: int, expected_height: int) -> String:
	var signature := PackedByteArray([137, 80, 78, 71, 13, 10, 26, 10])
	if payload.size() < 33 or payload.size() > 8 * 1024 * 1024 or payload.slice(0, 8) != signature:
		return "PNG preview is truncated or exceeds bounds"
	var width := _read_u32_be(payload, 16)
	var height := _read_u32_be(payload, 20)
	if width <= 0 or height <= 0 or width * height > 4194304 or width != expected_width or height != expected_height:
		return "PNG dimensions exceed bounds or differ from canonical cell"
	var decoded := Image.new()
	if decoded.load_png_from_buffer(payload) != OK or decoded.is_empty() or decoded.get_width() != width or decoded.get_height() != height:
		return "PNG preview failed bounded image decode"
	return ""

func _root_path_for(relative: String) -> String:
	return _root.path_join(relative)

func _no_symlink_segments(relative: String) -> bool:
	var current := _root
	for part in relative.split("/"):
		current = current.path_join(part)
		if _is_link(current):
			return false
	return true

func _safe_relative(value: String) -> bool:
	if value.is_empty() or value.begins_with("/") or value.contains("\\"):
		return false
	for part in value.split("/"):
		if part.is_empty() or part == "." or part == "..":
			return false
	return true

func _has_exact_keys(value: Dictionary, expected: Array) -> bool:
	if value.size() != expected.size():
		return false
	for key in expected:
		if not value.has(key):
			return false
	return true

func _canonical_map() -> Dictionary:
	if not FileAccess.file_exists(CANONICAL_CELLS_PATH) or FileAccess.get_sha256(CANONICAL_CELLS_PATH) != CANONICAL_CELLS_SHA256:
		return {}
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(CANONICAL_CELLS_PATH))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.prologue-nbn-canonical-cells.v1" or not parsed.get("entries") is Dictionary:
		return {}
	return parsed

func _matches_canonical(item: Dictionary, source: Dictionary, metadata: Dictionary, canonical: Dictionary) -> bool:
	var ident := str(canonical.get("canonical_region_id", ""))
	if ident != item.get("id") or ident != metadata.get("canonical_region_id"):
		return false
	if source.get("sha256") != canonical.get("source_sha256"):
		return false
	if item.get("payload_sha256") != canonical.get("png_sha256") and not _safe_sha(str(item.get("payload_sha256", ""))):
		return false
	if metadata.get("source_variant") != canonical.get("source_variant"):
		return false
	var expected_palette := {"palette_index": canonical.get("palette_index"), "source_sha256": canonical.get("palette_source_sha256"), "source_offset": canonical.get("palette_source_offset"), "status": canonical.get("palette_status")}
	if metadata.get("palette_variant") != expected_palette:
		return false
	if metadata.get("use_aliases") != [{"caller": canonical.get("caller"), "screen_context": canonical.get("screen_context")}]:
		return false
	for key in ["cell_index", "width", "height", "grid_column", "grid_row", "source_texture_dimensions", "source_spans", "unused_source_spans_for_cell", "clut_bit15_status"]:
		if metadata.get(key) != canonical.get(key):
			return false
	return metadata.get("static_only") == true and metadata.get("animation_composition_proven") == false

func _read_u32_be(data: PackedByteArray, offset: int) -> int:
	return (int(data[offset]) << 24) | (int(data[offset + 1]) << 16) | (int(data[offset + 2]) << 8) | int(data[offset + 3])

func _safe_id(value: String) -> bool:
	if value.is_empty() or value.length() > 128:
		return false
	for i in value.length():
		var c := value.unicode_at(i)
		if not ((c >= 48 and c <= 57) or (c >= 65 and c <= 90) or (c >= 97 and c <= 122) or c == 45 or c == 46 or c == 95):
			return false
	return value.unicode_at(0) != 45 and value.unicode_at(0) != 46 and value.unicode_at(0) != 95

func _safe_sha(value: String) -> bool:
	if value.length() != 64:
		return false
	for i in value.length():
		var c := value.unicode_at(i)
		if not ((c >= 48 and c <= 57) or (c >= 97 and c <= 102)):
			return false
	return true

func _fail(message: String) -> bool:
	_root = ""
	_components.clear()
	_available.clear()
	_unavailable.clear()
	_error = message
	return false

func _is_exact_integer(value: Variant, maximum: int) -> bool:
	if value is bool or not (value is int or value is float):
		return false
	if value is float and (not is_finite(value) or floor(value) != value):
		return false
	return value >= 0 and value <= maximum

func _no_root_symlink(root: String) -> bool:
	if not root.is_absolute_path():
		return false
	var current := "/"
	for part in root.trim_prefix("/").split("/"):
		if part.is_empty():
			continue
		current = current.path_join(part)
		if _is_link(current):
			return false
	return true

func _is_link(path: String) -> bool:
	var dir := DirAccess.open(path.get_base_dir())
	return dir != null and dir.is_link(path.get_file())
