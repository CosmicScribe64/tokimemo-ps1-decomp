extends SceneTree

const Store = preload("res://services/school_day_media_store.gd")

func _initialize() -> void:
	var root := OS.get_environment("TOKIMEMO_MEDIA_PACK")
	if root.is_empty():
		_fail("TOKIMEMO_MEDIA_PACK was not set")
		return
	var store = Store.new()
	if not store.open_pack(root):
		_fail("actual pack rejected: " + store.error_message())
		return
	if not store.error_message().is_empty():
		_fail("successful open retained an error: " + store.error_message())
		return
	var bg := store.get_background()
	if bg.is_empty() or bg["image_data"].get_size() != Vector2i(320, 88) or bg["padded_image_data"].get_size() != Vector2i(320, 96):
		_fail("profiled command-0 background is unavailable or has wrong geometry")
		return
	var total_frames := 0
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(root.path_join("manifest.json")))
	var extended: bool = manifest.get("schema") == "tokimemo.school-day-media.v2"
	var rest_bg: Dictionary = store.get_background("command_7_background")
	if extended:
		if rest_bg.is_empty() or rest_bg["image_data"].get_size() != Vector2i(320, 88) or rest_bg["padded_image_data"].get_size() != Vector2i(320, 96):
			_fail("command-7 background did not resolve through production getter")
			return
	elif not rest_bg.is_empty():
		_fail("v1 pack exposed unadvertised command-7 background")
		return
	for key in ["command_0_sd", "command_7_sd"]:
		var seq := store.get_sequence(key, 0)
		if seq.is_empty():
			_fail("sequence zero is unavailable: " + key)
			return
		if not store.get_sequence(key, 1).is_empty():
			_fail("unadvertised nonzero sequence was exposed")
			return
		for descriptor in manifest["components"][key]["frames"]:
			var frame := store.get_frame(key, descriptor["frame_index"])
			if frame.is_empty() or frame["image"].is_empty() or frame["stp_mask"].get_size() != frame["image"].get_size():
				_fail("validated SD frame/image is unavailable: " + key)
				return
			total_frames += 1
	if total_frames != 16:
		_fail("sequence frame count changed: %d" % total_frames)
		return
	var cases: Array = [
		{"name": "bad-provenance", "mutate": func(d): d["provenance"]["original_input_sha256"] = "0".repeat(64)},
		{"name": "bad-native-route-profile", "mutate": func(d): d["components"]["command_0_sd"]["native_route"]["profile"] = "invalid-profile"},
		{"name": "sequence-129", "mutate": func(d): _expand_sequence_zero(d)},
		{"name": "capability-overclaim", "mutate": func(d): d["route_capabilities"].append("command_7_background")},
		{"name": "active-crop", "mutate": func(d): d["components"]["command_0_background"]["active_crop"] = [0, 0, 1, 1]},
		{"name": "frame-origin", "mutate": func(d): d["components"]["command_0_sd"]["frames"][0]["origin"] = "bad"},
		{"name": "sequence-duration", "mutate": func(d): d["components"]["command_0_sd"]["sequences"][0]["steps"][0]["duration"] = 32768},
		{"name": "part-width", "mutate": func(d): d["components"]["command_0_sd"]["frames"][0]["parts"][0]["width"] = 129},
		{"name": "palette-size", "mutate": func(d): d["components"]["command_0_sd"]["frames"][0]["source_palette_words"] = [0]},
	]
	if extended:
		cases.append_array([
			{"name": "missing-command7", "mutate": func(d): d["components"].erase("command_7_background")},
			{"name": "command7-source", "mutate": func(d): d["components"]["command_7_background"]["source"]["lba"] = 16284},
			{"name": "command7-route", "mutate": func(d): d["components"]["command_7_background"]["native_route"]["command"] = 0},
			{"name": "command7-overread", "mutate": func(d): d["components"]["command_7_background"]["native_read_extent"]["overread_payload_used_for_mdec"] = true},
			{"name": "command7-table", "mutate": func(d): d["components"]["command_7_background"]["geometry_context"]["source_profile"]["executable"]["table_words"][7] = 16284},
			{"name": "command7-iso", "mutate": func(d): d["components"]["command_7_background"]["geometry_context"]["source_profile"]["assets"][0]["iso_directory_record"]["record_payload_offset"] = 1117},
			{"name": "command7-bool", "mutate": func(d): d["components"]["command_7_background"]["geometry_context"]["width"] = true},
		])
	for case in cases:
		if not _reject_mutated_pack(root, case["name"], case["mutate"]):
			_fail("invalid fixture was accepted or exposed partial content: " + case["name"])
			return
	if not _reject_oversized_png(root):
		_fail("oversized PNG header was accepted or reached an unsafe decode")
		return
	var bad_root := "/tmp/tokimemo-school-day-invalid"
	DirAccess.make_dir_recursive_absolute(bad_root)
	var bad_manifest := FileAccess.open(bad_root.path_join("manifest.json"), FileAccess.WRITE)
	bad_manifest.store_string("{\"schema\":\"wrong\"}")
	bad_manifest.close()
	if store.open_pack(bad_root):
		_fail("wrong schema unexpectedly opened")
		return
	if not store.get_background().is_empty() or not store.get_background("command_7_background").is_empty() or not store.get_frame("command_0_sd", 0).is_empty():
		_fail("failed open exposed stale content")
		return
	DirAccess.remove_absolute(bad_root.path_join("manifest.json"))
	DirAccess.remove_absolute(bad_root)
	print("PASS: school-day media actual-pack getters, malformed-pack negatives, fail-closed state")
	quit()

func _fail(message: String) -> void:
	push_error(message)
	quit(1)

func _expand_sequence_zero(doc: Dictionary) -> void:
	var steps: Array = doc["components"]["command_0_sd"]["sequences"][0]["steps"]
	var first: Dictionary = steps[0].duplicate(true)
	steps.clear()
	for i in range(129):
		steps.append(first.duplicate(true))

func _reject_mutated_pack(source: String, label: String, mutate: Callable) -> bool:
	var target := "/tmp/tokimemo-school-day-" + label
	_remove_tree(target)
	if not _copy_tree(source, target):
		return false
	var manifest_path := target.path_join("manifest.json")
	var doc: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not doc is Dictionary:
		_remove_tree(target)
		return false
	mutate.call(doc)
	var file := FileAccess.open(manifest_path, FileAccess.WRITE)
	if file == null:
		_remove_tree(target)
		return false
	file.store_string(JSON.stringify(doc))
	file.close()
	var negative = Store.new()
	var rejected := not negative.open_pack(target)
	var empty := negative.get_background().is_empty() and negative.get_background("command_7_background").is_empty() and negative.get_frame("command_0_sd", 0).is_empty()
	_remove_tree(target)
	return rejected and empty and not negative.error_message().is_empty()

func _copy_tree(source: String, target: String) -> bool:
	if DirAccess.make_dir_recursive_absolute(target) != OK:
		return false
	var dir := DirAccess.open(source)
	if dir == null:
		return false
	for name in dir.get_files():
		if DirAccess.copy_absolute(source.path_join(name), target.path_join(name)) != OK:
			return false
	for name in dir.get_directories():
		if not _copy_tree(source.path_join(name), target.path_join(name)):
			return false
	return true

func _remove_tree(path: String) -> void:
	var dir := DirAccess.open(path)
	if dir == null:
		return
	for name in dir.get_files():
		DirAccess.remove_absolute(path.path_join(name))
	for name in dir.get_directories():
		_remove_tree(path.path_join(name))
	DirAccess.remove_absolute(path)

func _reject_oversized_png(source: String) -> bool:
	var target := "/tmp/tokimemo-school-day-oversized-png"
	_remove_tree(target)
	if not _copy_tree(source, target):
		return false
	var manifest_path := target.path_join("manifest.json")
	var doc: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	var spec: Dictionary = doc["components"]["command_0_background"]["image"]
	var png_path := target.path_join("assets").path_join(spec["path"])
	var input := FileAccess.open(png_path, FileAccess.READ)
	if input == null:
		_remove_tree(target)
		return false
	var bytes := input.get_buffer(input.get_length())
	input.close()
	bytes[16] = 0
	bytes[17] = 1
	bytes[18] = 134
	bytes[19] = 160
	bytes[20] = 0
	bytes[21] = 1
	bytes[22] = 134
	bytes[23] = 160
	var output := FileAccess.open(png_path, FileAccess.WRITE)
	if output == null:
		_remove_tree(target)
		return false
	output.store_buffer(bytes)
	output.close()
	spec["sha256"] = _bytes_sha(bytes)
	var manifest_out := FileAccess.open(manifest_path, FileAccess.WRITE)
	manifest_out.store_string(JSON.stringify(doc))
	manifest_out.close()
	var negative = Store.new()
	var rejected := not negative.open_pack(target)
	var bounded_error := negative.error_message().contains("header dimensions")
	var empty := negative.get_background().is_empty() and negative.get_background("command_7_background").is_empty() and negative.get_frame("command_0_sd", 0).is_empty()
	_remove_tree(target)
	return rejected and bounded_error and empty

func _bytes_sha(bytes: PackedByteArray) -> String:
	var ctx := HashingContext.new()
	ctx.start(HashingContext.HASH_SHA256)
	ctx.update(bytes)
	return ctx.finish().hex_encode()
