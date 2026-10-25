extends RefCounted

const ContentStoreScript = preload("res://services/content_store.gd")
const CANONICAL_PATH := "CDROM/PROLODIR/CUT_05.NBN"
const PNG_1X1 := "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR4nGP4z9DwHwAGAAJ/IQJzOwAAAABJRU5ErkJggg=="

func run() -> Array[String]:
	var failures: Array[String] = []
	var store = ContentStoreScript.new()
	var invented_png := Marshalls.base64_to_raw(PNG_1X1)
	if not store.validate_png_preview(invented_png, 1, 1).is_empty():
		failures.append("valid invented PNG did not decode")
	if store.validate_png_preview(PackedByteArray([137, 80, 78, 71, 13, 10, 26, 10]), 128, 120).is_empty():
		failures.append("eight-byte truncated PNG was accepted")
	if store.validate_png_preview(invented_png, 2048, 2048).is_empty():
		failures.append("PNG geometry mismatch was accepted")
	var root := OS.get_cache_dir().path_join("tokimemo-content-test-" + str(Time.get_ticks_usec()))
	var unsupported_path := root.path_join("unsupported-capability")
	var missing_format_path := root.path_join("missing-format")
	var unknown_field_path := root.path_join("unknown-field")
	var wrong_cell_path := root.path_join("wrong-canonical")
	var partial_path := root.path_join("partial")
	var source_format_path := root.path_join("source-format")
	var creator_keys_path := root.path_join("creator-keys")
	var root_format_path := root.path_join("root-format")
	var dot_path := root.path_join("dot-path")
	var parent_path := root.path_join("parent-path")
	var invalid_hash_path := root.path_join("unsupported-invalid-hash")
	if _write_inventory_fixture(unsupported_path, true, false, false, false):
		if store.open_pack(unsupported_path) or not store.error_message().contains("cannot advertise capabilities"):
			failures.append("unsupported inventory advertised an available capability")
	else:
		failures.append("could not create unsupported-capability fixture")
	if _write_inventory_fixture(missing_format_path, false, true, false, false):
		if store.open_pack(missing_format_path) or not store.error_message().contains("source fields or format"):
			failures.append("missing required source format was accepted")
	else:
		failures.append("could not create missing-format fixture")
	if _write_inventory_fixture(unknown_field_path, false, false, true, false):
		if store.open_pack(unknown_field_path) or not store.error_message().contains("component fields"):
			failures.append("unknown component field was accepted")
	else:
		failures.append("could not create unknown-field fixture")
	if _write_inventory_fixture(wrong_cell_path, false, false, false, true):
		if store.open_pack(wrong_cell_path) or not store.error_message().contains("canonical region"):
			failures.append("incorrect canonical cell ID was accepted")
	else:
		failures.append("could not create wrong-cell fixture")
	if _write_partial_fixture(partial_path):
		if store.open_pack(partial_path):
			failures.append("malformed second component was accepted")
		elif not store.get_component("first-entry").is_empty() or store.has_capability("fixture.preview"):
			failures.append("failed open exposed partially loaded state")
	else:
		failures.append("could not create partial-load fixture")
	if _write_inventory_fixture(source_format_path, false, false, false, false, true, false):
		if store.open_pack(source_format_path):
			failures.append("unknown source.format was accepted")
	else:
		failures.append("could not create unknown source.format fixture")
	if _write_inventory_fixture(creator_keys_path, false, false, false, false, false, true):
		if store.open_pack(creator_keys_path):
			failures.append("unknown created_by key was accepted")
	else:
		failures.append("could not create unknown created_by fixture")
	if _write_inventory_fixture(root_format_path, false, false, false, false, false, false, true):
		if store.open_pack(root_format_path):
			failures.append("unknown root source.format was accepted")
	else:
		failures.append("could not create unknown root source.format fixture")
	for unsafe_path in [{"fixture": dot_path, "source": "."}, {"fixture": parent_path, "source": "foo/.."}]:
		if _write_inventory_fixture(unsafe_path["fixture"], false, false, false, false, false, false, false, unsafe_path["source"]):
			if store.open_pack(unsafe_path["fixture"]):
				failures.append("unsafe dot-segment path was accepted: " + unsafe_path["source"])
		else:
			failures.append("could not create dot-segment path fixture")
	if _write_inventory_fixture(invalid_hash_path, false, false, false, false, false, false, false, "", true):
		if store.open_pack(invalid_hash_path):
			failures.append("unsupported component with malformed source SHA was accepted")
	else:
		failures.append("could not create malformed unsupported SHA fixture")
	var real_pack := OS.get_environment("TOKIMEMO_CONTENT_PACK")
	if not real_pack.is_empty():
		var real_store = ContentStoreScript.new()
		if not real_store.open_pack(real_pack):
			failures.append("actual canonical Python pack rejected: " + real_store.error_message())
		else:
			if not real_store.has_capability("prologue.nbn_native_cells.static_preview"):
				failures.append("actual pack's declared preview capability is unavailable")
			if real_store.has_capability("school_day.route"):
				failures.append("actual pack overclaimed school-day route support")
			var components: Array = JSON.parse_string(FileAccess.get_file_as_string(real_pack.path_join("manifest.json"))).get("components", [])
			if components.size() != 24:
				failures.append("actual pack did not contain 24 canonical cells")
			else:
				for item in components:
					var canonical := str(item.get("metadata", {}).get("canonical_region_id", ""))
					if canonical != str(item.get("id", "")):
						failures.append("actual component ID lost its canonical region cross-reference")
						break
			var first_id := str(components[0].get("id", "")) if not components.is_empty() else ""
			var copied := real_store.get_component(first_id)
			copied["id"] = "caller mutation"
			if real_store.get_component(first_id).get("id") != first_id:
				failures.append("component metadata leaked mutable internal state")
	_remove_tree(root)
	return failures

func _write_inventory_fixture(path: String, unsupported_capability: bool, missing_format: bool, unknown_field: bool, wrong_cell: bool, unknown_format := false, creator_extra := false, unknown_root_format := false, source_path := "", bad_source_hash := false) -> bool:
	if DirAccess.make_dir_recursive_absolute(path) != OK:
		return false
	var source := {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 100352,
		"sha256": "34494d1b0c7305466b097d62c4c68b10486325d70e71cb0115bc58d335ebd87c", "locator": {}}
	if missing_format:
		source.erase("format")
	if unknown_format:
		source["format"] = "mode2-iso-file-extra"
	if not source_path.is_empty():
		source["path"] = source_path
	if bad_source_hash:
		source["sha256"] = "bad"
	var component := {"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported",
		"capabilities": ["fixture.preview"] if unsupported_capability else [], "source": source}
	if unknown_field:
		component["new_schema_feature"] = true
	var components: Array = [component]
	var available: Array = ["fixture.preview"] if unsupported_capability else []
	if wrong_cell:
		components = [_wrong_cell_component()]
		available = ["prologue.nbn_native_cells.static_preview"]
	return _write_pack_manifest(path, components, available, unknown_root_format, creator_extra)

func _wrong_cell_component() -> Dictionary:
	var map: Dictionary = JSON.parse_string(FileAccess.get_file_as_string("res://schemas/canonical_prologue_nbn_cells.v1.json"))
	var canonical: Dictionary = map["entries"][CANONICAL_PATH + "#0"]
	var metadata := {"canonical_region_id": canonical["canonical_region_id"], "cell_index": 0,
		"source_variant": canonical["source_variant"], "palette_variant": {"palette_index": canonical["palette_index"],
		"source_sha256": canonical["palette_source_sha256"], "source_offset": canonical["palette_source_offset"], "status": canonical["palette_status"]},
		"use_aliases": [{"caller": canonical["caller"], "screen_context": canonical["screen_context"]}],
		"width": canonical["width"], "height": canonical["height"], "grid_column": canonical["grid_column"], "grid_row": canonical["grid_row"],
		"source_texture_dimensions": canonical["source_texture_dimensions"], "source_spans": canonical["source_spans"],
		"unused_source_spans_for_cell": canonical["unused_source_spans_for_cell"], "clut_bit15_status": canonical["clut_bit15_status"],
		"static_only": true, "animation_composition_proven": false}
	return {"id": "region-" + "0".repeat(64), "version": 1, "kind": "png_preview", "status": "available",
		"capabilities": ["prologue.nbn_native_cells.static_preview"], "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 100352,
		"sha256": canonical["source_sha256"], "locator": {}}, "payload": "payloads/fake.png", "payload_size_bytes": 8,
		"payload_sha256": canonical["png_sha256"], "metadata": metadata}

func _write_partial_fixture(path: String) -> bool:
	var valid: Dictionary = _wrong_cell_component()
	valid["id"] = "first-entry"
	valid["kind"] = "inventory_entry"
	valid["status"] = "unsupported"
	valid["capabilities"] = []
	valid["source"] = {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 100352,
		"sha256": "34494d1b0c7305466b097d62c4c68b10486325d70e71cb0115bc58d335ebd87c", "locator": {}}
	for key in ["payload", "payload_size_bytes", "payload_sha256", "metadata"]:
		valid.erase(key)
	var invalid := valid.duplicate(true)
	invalid["id"] = "second-entry"
	invalid["capabilities"] = 123
	return _write_pack_manifest(path, [valid, invalid], [])

func _write_pack_manifest(path: String, components: Array, available: Array, unknown_root_format := false, creator_extra := false) -> bool:
	if DirAccess.make_dir_recursive_absolute(path) != OK:
		return false
	var manifest := {"schema": "tokimemo.port-content.v1", "schema_version": 1,
		"release_profile": "original-ja-ps-the-best",
		"source": {"format": "mode2-2352", "sha256": "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72", "size_bytes": 702072000, "read_only": true},
		"created_by": {"reader": "tools.assets.input.open_input", "media_schema": "tokimemo.media.v1",
		"reviewed_component_schema": "tokimemo-prologue-nbn-source-profile.v2",
		"media_manifest_sha256": "26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde",
		"visual_catalog_sha256": "d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6",
		"cell_map_sha256": "5eb43589bb9722a2792c6d8c880141c36a7aa01fcc14fdfbe82be6de07282209",
		"input_reader_source_sha256": "d".repeat(64), "importer_source_sha256": "e".repeat(64), "validator_source_sha256": "f".repeat(64)},
		"components": components, "route_capabilities": {"available": available, "unavailable": ["school_day.route"]}}
	if creator_extra:
		manifest["created_by"]["extra"] = "not-in-v1"
	if unknown_root_format:
		manifest["source"]["format"] = "mode2-2352-extra"
	var file := FileAccess.open(path.path_join("manifest.json"), FileAccess.WRITE)
	if file == null:
		return false
	file.store_string(JSON.stringify(manifest))
	file.close()
	return true

func _remove_tree(path: String) -> void:
	var dir := DirAccess.open(path)
	if dir == null:
		return
	dir.list_dir_begin()
	var entry := dir.get_next()
	while not entry.is_empty():
		if entry != "." and entry != "..":
			var child := path.path_join(entry)
			if dir.current_is_dir():
				_remove_tree(child)
			else:
				DirAccess.remove_absolute(child)
		entry = dir.get_next()
	dir.list_dir_end()
	DirAccess.remove_absolute(path)
