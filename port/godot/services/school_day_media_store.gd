extends RefCounted
## Strict data-only loader for finite school-day-media v1/v2 external packs.

const SCHEMA := "tokimemo.school-day-media.v1"
const COMMAND7_SCHEMA := "tokimemo.school-day-media.v2"
const HOLIDAY_SCHEMA := "tokimemo.school-day-media.v3"
const HOLIDAY_PROOF_PATH := "res://schemas/holiday_command0_profile.v1.json"
const COMMAND7_PROOF_PATH := "res://schemas/command7_background_profile.v1.json"
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const EXE_SHA := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const MEDIA_SHA := "0c85bbc54be82c7b2597528ccbfc915cd1fb5f99405bd18d845452026b56be37"
const NATIVE_SHA := "ac246a30ed894bf5b631beaf4f053a8622324094859b11a65a4c0c03d55cb4a3"
const SD_SOURCE_SHAS := {"holiday_command_0_sd": "d3d769779a8fd6d7d088978db69870c1fe2f9008c303d40f14f43a6ef6220d36", "command_0_sd": "6fea04f95b7eb68731816fa21395605942b1b912b0b915afcfea4d540da7eb6f", "command_7_sd": "1ad4b906e8aefbb57fd0b9a22afea9af83a0d5109234f9621abc5f91829e7d28"}
const SD_SOURCE_IDS := {"holiday_command_0_sd": "asset-a549506ecd420dccd3d1bbf4d52b344142f1257c2484c0c8bd13dc48e7bc618a", "command_0_sd": "asset-0c557c34ef671750c707a8dc3573c2c2ea0c31af6e773654523f3024f68bf4e5", "command_7_sd": "asset-c15fc44db85fe2b1485503e27b84a50e5a7bf9d28866d5efef247d9e172a8c28"}
const MAX_MANIFEST := 8 * 1024 * 1024
const MAX_PNG := 2 * 1024 * 1024
const MAX_PIXELS := 4 * 1024 * 1024
const PART_KEYS := ["part_index", "source_record_offset", "x", "y", "width", "height", "texture_id", "sheet_index", "sheet_source_offset", "u", "v", "palette_info", "flip_flags", "clut_vram", "palette_source_offset", "runtime_palette_override", "palette_mapping"]
const PART_PALETTE_MAPPING := "override0x10 and palette_mode2 without bit0x80 select full256-color row at x0; encoded row/column ignored"
const SD_STP_POLICY := "source palette load mode1 leaves STP unchanged; separate bit15 mask"
const READER_PINS := ["tools/assets/input.py:open_input", "tools/assets/image_packs_sd.py:export_sd_ui", "tools/assets/bs_cohort_profile.py:build_profile_context", "tools/assets/image_mdec.py:export_mdec"]
const ExternalContentPath = preload("res://services/external_content_path.gd")

var _root := ""
var _components: Dictionary = {}
var _route_tables: Dictionary = {}
var _loaded_images: Dictionary = {}
var _error := ""

func open_pack(root: String) -> bool:
	_clear()
	if not _safe_root(root):
		return _fail("pack root path is unsafe or contains a symlink")
	var manifest_path := root.path_join("manifest.json")
	if _is_link(manifest_path) or not FileAccess.file_exists(manifest_path):
		return _fail("manifest.json is missing or linked")
	if FileAccess.get_size(manifest_path) > MAX_MANIFEST:
		return _fail("manifest exceeds size limit")
	var assets_path := root.path_join("assets")
	if _is_link(assets_path) or not DirAccess.dir_exists_absolute(assets_path):
		return _fail("assets directory is missing or linked")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary:
		return _fail("manifest JSON must be an object")
	var holiday: bool = parsed.get("schema") == HOLIDAY_SCHEMA
	var holiday_proof: Dictionary = {}
	var manifest_keys := ["schema", "version", "provenance", "route_capabilities", "unsupported", "components"]
	if holiday:
		manifest_keys.append("route_tables")
		var proof: Variant = JSON.parse_string(FileAccess.get_file_as_string(HOLIDAY_PROOF_PATH))
		if not proof is Dictionary:
			return _fail("holiday technical content profile is unavailable")
		holiday_proof = proof
	if not _exact_keys(parsed, manifest_keys):
		return _fail("manifest keys differ from v1 contract")
	var extended: bool = parsed.get("schema") in [COMMAND7_SCHEMA, HOLIDAY_SCHEMA]
	var version: int = 3 if holiday else 2 if extended else 1
	if parsed.get("schema") not in [SCHEMA, COMMAND7_SCHEMA, HOLIDAY_SCHEMA] or not _exact_int(parsed.get("version"), version) or int(parsed["version"]) != version:
		return _fail("unsupported schema/version")
	var provenance: Variant = parsed.get("provenance")
	var provenance_keys := ["original_input_sha256", "original_executable_sha256", "reviewed_media_manifest_sha256", "native_profile_sha256", "readers"]
	var readers := READER_PINS.duplicate()
	if holiday:
		provenance_keys.append("holiday_profile_sha256")
		readers.append("tools/assets/holiday_content_profile.py:build_holiday_profile")
	if not provenance is Dictionary or not _exact_keys(provenance, provenance_keys) or provenance.get("original_input_sha256") != TRACK_SHA or provenance.get("original_executable_sha256") != EXE_SHA or provenance.get("reviewed_media_manifest_sha256") != MEDIA_SHA or provenance.get("native_profile_sha256") != NATIVE_SHA or not _string_array_equals(provenance.get("readers"), readers):
		return _fail("source provenance is not the reviewed route profile")
	if holiday and provenance.get("holiday_profile_sha256") != holiday_proof["native_profile_sha256"]:
		return _fail("holiday native profile SHA differs")
	var caps: Variant = parsed.get("route_capabilities")
	var expected_caps := ["command_0_static_background", "command_0_sd_sequence_0", "command_7_sd_sequence_0"]
	if extended:
		expected_caps.append("command_7_static_background")
	if holiday:
		expected_caps.append_array(["holiday_room_static_background", "holiday_command_0_static_background", "holiday_command_0_sd_sequence_0"])
	if not _string_array_equals(caps, expected_caps):
		return _fail("route capability declarations differ from supported components")
	var unsupported: Variant = parsed.get("unsupported")
	var unsupported_keys := ["scene_composition", "full_route"]
	if not extended:
		unsupported_keys.append("command_7_background")
	if not unsupported is Dictionary or not _exact_keys(unsupported, unsupported_keys) or (not extended and unsupported.get("command_7_background") != "no fresh accepted geometry profile in this tranche") or unsupported.get("scene_composition") != "runtime placement/compositing not established by this media adapter" or unsupported.get("full_route") != "not claimed":
		return _fail("unsupported route capabilities must be explicit")
	var components: Variant = parsed.get("components")
	var component_keys := ["command_0_background", "command_0_sd", "command_7_sd"]
	if extended:
		component_keys.append("command_7_background")
	if holiday:
		component_keys.append_array(["holiday_room_background", "holiday_command_0_background", "holiday_command_0_sd"])
	if not components is Dictionary or not _exact_keys(components, component_keys):
		return _fail("component set differs from finite preview contract")
	_root = root
	for command in ([0, 7] if extended else [0]):
		var bg: Variant = components["command_%d_background" % command]
		var fields := ["kind", "id", "source", "active_size", "padded_size", "active_crop", "native_route", "image", "stp_mask", "padded_image", "padded_stp_mask", "geometry_context", "reader", "capability"]
		if command == 7:
			fields.append("native_read_extent")
		if not bg is Dictionary or not _exact_keys(bg, fields):
			return _fail("background fields differ from media contract")
		if not _validate_background(bg, command):
			return _fail(_error)
	if holiday:
		if not _validate_route_tables(parsed.get("route_tables"), holiday_proof["route_tables"]):
			return _fail("holiday/weekday room dependency tables differ")
		for key in ["holiday_room_background", "holiday_command_0_background"]:
			if not _metadata_equal(components[key], holiday_proof["components"][key]):
				return _fail("holiday background source/native/geometry descriptor differs")
	var sd_profiles := [["command_0_sd", 3], ["command_7_sd", 16]]
	if holiday:
		sd_profiles.append(["holiday_command_0_sd", 1])
	for pair in sd_profiles:
		var key: String = pair[0]
		var expected_profile: int = pair[1]
		var sd: Variant = components[key]
		var fields := ["kind", "id", "source", "profile_index", "sequence_index", "native_route", "frames", "sequences", "palette", "reader", "capability"]
		if key == "holiday_command_0_sd":
			fields.append_array(["caller_profile", "source_profile"])
		if not sd is Dictionary or not _exact_keys(sd, fields):
			return _fail("SD fields differ from v1 contract: " + key)
		if not sd is Dictionary or sd.get("kind") != "sd_animation" or sd.get("id") != SD_SOURCE_IDS[key] or sd.get("reader") != "tools/assets/image_packs_sd.py:export_sd_ui" or sd.get("capability") != "validated_static_sd_frames_and_sequence_prefix" or not _exact_int(sd.get("profile_index"), expected_profile) or int(sd["profile_index"]) != expected_profile or not _exact_int(sd.get("sequence_index"), 0) or int(sd["sequence_index"]) != 0:
			return _fail("invalid SD profile/sequence descriptor: " + key)
		if key == "holiday_command_0_sd":
			var projection: Dictionary = sd.duplicate(true)
			for omitted in ["frames", "sequences", "palette"]:
				projection.erase(omitted)
			if not _metadata_equal(projection, holiday_proof["components"][key]):
				return _fail("holiday SD source/caller/native profile differs")
		if not _validate_sd(sd, key):
			return _fail(_error)
		if key == "holiday_command_0_sd":
			var metadata := {"frames": sd["frames"], "sequences": sd["sequences"], "palette": sd["palette"]}
			if _sha(JSON.stringify(_integer_metadata(metadata), "", true).to_utf8_buffer()) != holiday_proof["sd_metadata_sha256"]:
				return _fail("holiday SD frame/sequence/palette metadata digest differs")
	if not _load_all_payloads(components):
		return _fail(_error)
	_components = components.duplicate(true)
	if holiday:
		_route_tables = parsed["route_tables"]["values"].duplicate(true)
	_error = ""
	return true

func get_frame(component_key: String, frame_index: int) -> Dictionary:
	var sd: Dictionary = _components.get(component_key, {})
	for frame in sd.get("frames", []):
		if frame.get("frame_index") == frame_index:
			var result: Dictionary = frame.duplicate(true)
			result["image"] = _loaded_images[frame["png"]["path"]]
			result["stp_mask"] = _loaded_images[frame["stp_png"]["path"]]
			return result
	return {}

func get_sequence(component_key: String, sequence_index: int = 0) -> Array:
	var sd: Dictionary = _components.get(component_key, {})
	for seq in sd.get("sequences", []):
		if seq.get("sequence_index") == sequence_index:
			return seq.get("steps", []).duplicate(true)
	return []

func get_background(component_key: String = "command_0_background") -> Dictionary:
	var bg: Dictionary = _components.get(component_key, {})
	if bg.get("kind") != "mdec_background":
		return {}
	var result := bg.duplicate(true)
	result["image_data"] = _loaded_images[bg["image"]["path"]]
	result["stp_mask_data"] = _loaded_images[bg["stp_mask"]["path"]]
	result["padded_image_data"] = _loaded_images[bg["padded_image"]["path"]]
	result["padded_stp_mask_data"] = _loaded_images[bg["padded_stp_mask"]["path"]]
	return result

func get_route_tables() -> Dictionary:
	return _route_tables.duplicate(true)

func error_message() -> String:
	return _error

func _validate_background(bg: Variant, command: int) -> bool:
	var expected_id := "asset-cc9a7f7ff39dfb8c072ef6675934282792dfd7afef494b0c0b180643ea54135d" if command == 0 else "asset-737fb131adbd66af423adbcc776e651450043c0e953d6b83b64412b022d286aa"
	var expected_path := "CDROM/BS/C_01_27.BS" if command == 0 else "CDROM/BS/C_01_44.BS"
	var expected_lba := 16131 if command == 0 else 16283
	var expected_size := 20032 if command == 0 else 17396
	var expected_sha := "7543e723ea06c7db100b148350394f79c1a94ff8961aa65904d4aa1613295992" if command == 0 else "951ec47906f6a19748d2f3b803cb086c0e5af3a467f3cb6819515225bb6365e0"
	if not bg is Dictionary or bg.get("kind") != "mdec_background" or bg.get("id") != expected_id or bg.get("reader") != "tools/assets/image_mdec.py:export_mdec" or bg.get("capability") != "source_profiled_static_background_preview":
		_error = "invalid command background identity"
		return false
	var bg_source: Variant = bg.get("source")
	if not bg_source is Dictionary or not _exact_keys(bg_source, ["path", "lba", "size", "sha256"]) or bg_source.get("path") != expected_path or not _exact_int(bg_source.get("lba"), expected_lba) or int(bg_source["lba"]) != expected_lba or not _exact_int(bg_source.get("size"), expected_size) or int(bg_source["size"]) != expected_size or bg_source.get("sha256") != expected_sha:
		_error = "invalid command background source identity"
		return false
	var native_route: Variant = bg.get("native_route")
	if not native_route is Dictionary or not _exact_keys(native_route, ["command", "actual_last_command", "actors", "profile", "evidence"]):
		_error = "background is not bound to reviewed native route evidence"
		return false
	var bg_evidence: Variant = native_route.get("evidence")
	var bg_actors: Variant = native_route.get("actors")
	if not _exact_int(native_route.get("command"), command) or int(native_route["command"]) != command or not _exact_int(native_route.get("actual_last_command"), command) or int(native_route["actual_last_command"]) != command or not bg_actors is Array or bg_actors.size() != 1 or not _exact_int(bg_actors[0], 96) or int(bg_actors[0]) != 96 or native_route.get("profile") != "OpenBIOS d93840921b5ad7ed515e95b0e414f1b1e2038b1a" or not _valid_evidence(bg_evidence):
		_error = "background is not bound to reviewed native route evidence"
		return false
	if not _exact_int_array(bg.get("active_size"), [320, 88]) or not _exact_int_array(bg.get("padded_size"), [320, 96]) or not _exact_int_array(bg.get("active_crop"), [0, 0, 320, 88]):
		_error = "unsupported background geometry"
		return false
	for item in [["image", Vector2i(320, 88)], ["stp_mask", Vector2i(320, 88)], ["padded_image", Vector2i(320, 96)], ["padded_stp_mask", Vector2i(320, 96)]]:
		if not _validate_payload_spec(bg.get(item[0], {})):
			return false
	if command == 7:
		var proof: Variant = JSON.parse_string(FileAccess.get_file_as_string(COMMAND7_PROOF_PATH))
		if not proof is Dictionary or not _metadata_equal(bg.get("geometry_context"), proof.get("geometry_context")) or not _metadata_equal(bg.get("native_read_extent"), proof.get("native_read_extent")):
			_error = "command-7 source/geometry/native extent proof differs from checked profile"
			return false
	return true

func _validate_sd(sd: Dictionary, key: String) -> bool:
	var holiday: bool = key == "holiday_command_0_sd"
	var src: Variant = sd.get("source")
	var expected_path := "CDROM/HOLI_SD/SD_P24W.NBN" if holiday else "CDROM/MAIN_SD/SD_P01W.NBN" if sd.get("profile_index") == 3 else "CDROM/MAIN_SD/SD_P09W.NBN"
	var expected_lba := 41487 if holiday else 42541 if sd.get("profile_index") == 3 else 43074
	if not src is Dictionary or not _exact_keys(src, ["path", "lba", "size", "sha256", "exe_sha256"]) or src.get("path") != expected_path or not _exact_int(src.get("lba"), expected_lba) or int(src["lba"]) != expected_lba or not _exact_int(src.get("size"), 83968) or int(src["size"]) != 83968 or src.get("sha256") != SD_SOURCE_SHAS[key] or src.get("exe_sha256") != EXE_SHA:
		_error = "invalid SD source hashes"
		return false
	if not holiday:
		var route: Variant = sd.get("native_route")
		var expected_command := 0 if sd.get("profile_index") == 3 else 7
		if not route is Dictionary or not _exact_keys(route, ["command", "actual_last_command", "actors", "profile", "evidence"]):
			_error = "SD source is not bound to reviewed native route evidence"
			return false
		var route_evidence: Variant = route.get("evidence")
		var route_actors: Variant = route.get("actors")
		if not _exact_int(route.get("command"), expected_command) or int(route["command"]) != expected_command or not _exact_int(route.get("actual_last_command"), expected_command) or int(route["actual_last_command"]) != expected_command or not route_actors is Array or route_actors.size() != 1 or not _exact_int(route_actors[0], 96) or int(route_actors[0]) != 96 or route.get("profile") != "OpenBIOS d93840921b5ad7ed515e95b0e414f1b1e2038b1a" or not _valid_evidence(route_evidence):
			_error = "SD source is not bound to reviewed native route evidence"
			return false
	var frames: Variant = sd.get("frames")
	if not frames is Array or frames.is_empty() or frames.size() > 512:
		_error = "invalid SD frame list"
		return false
	var seen := {}
	var source_palette: Variant = null
	for frame in frames:
		if not frame is Dictionary or not _exact_keys(frame, ["frame_index", "id", "png", "stp_png", "bounds", "origin", "parts", "source_palette_words"]) or not _exact_int(frame.get("frame_index"), 4096):
			_error = "invalid SD frame index"
			return false
		var idx := int(frame["frame_index"])
		if seen.has(idx):
			_error = "duplicate SD frame index"
			return false
		seen[idx] = true
		var bounds: Variant = frame.get("bounds")
		if not _signed_array(frame.get("origin"), 2, -32768, 32767):
			_error = "frame origin must contain two signed 16-bit values"
			return false
		if not bounds is Array or bounds.size() != 4 or not _signed_array(bounds, 4, -32768, 32767) or int(bounds[2]) < 1 or int(bounds[3]) < 1 or int(bounds[2]) * int(bounds[3]) > MAX_PIXELS:
			_error = "invalid SD frame bounds"
			return false
		if frame.get("id") != str(sd["id"]) + ":frame:" + str(idx):
			_error = "SD frame identity does not match physical source index"
			return false
		if not _uint_array(frame.get("source_palette_words"), 256, 65535):
			_error = "SD source palette must contain 256 unsigned words"
			return false
		if source_palette == null:
			source_palette = frame["source_palette_words"]
		elif not _uint_arrays_equal(source_palette, frame["source_palette_words"]):
			_error = "source palette changed within one SD source pack"
			return false
		var parts: Variant = frame.get("parts")
		if not parts is Array or parts.size() > 128:
			_error = "SD frame part list exceeds bound"
			return false
		for part in parts:
			if not _validate_part(part, int(src["size"])):
				return false
		if not _validate_payload_spec(frame.get("png", {})) or not _validate_payload_spec(frame.get("stp_png", {})):
			return false
	var sequences: Variant = sd.get("sequences")
	if not sequences is Array or sequences.is_empty() or sequences.size() > 512:
		_error = "validated sequence zero is missing"
		return false
	if not sequences[0] is Dictionary or not _exact_keys(sequences[0], ["sequence_index", "source_pointer_offset", "source_offset", "source_size", "steps"]) or sequences[0].get("sequence_index") != 0:
		_error = "validated sequence zero is missing"
		return false
	var sequence_steps: Variant = sequences[0].get("steps")
	if not sequence_steps is Array or sequence_steps.is_empty() or sequence_steps.size() > 128:
		_error = "sequence zero must contain 1..128 supported steps"
		return false
	for step in sequence_steps:
		if not step is Dictionary or not _exact_keys(step, ["frame_index", "duration"]) or not _exact_int(step.get("frame_index"), 32767) or not seen.has(int(step["frame_index"])) or not _exact_int(step.get("duration"), 32767) or int(step["duration"]) < 1:
			_error = "sequence references missing frame or invalid duration"
			return false
	var seen_sequences := {}
	for sequence in sequences:
		if not sequence is Dictionary or not _exact_keys(sequence, ["sequence_index", "source_pointer_offset", "source_offset", "source_size", "steps"]) or not _exact_int(sequence.get("sequence_index"), 32767) or not _exact_int(sequence.get("source_pointer_offset"), int(src["size"]) - 1) or not _exact_int(sequence.get("source_offset"), int(src["size"])) or not _exact_int(sequence.get("source_size"), int(src["size"])) or int(sequence["source_size"]) < 1 or int(sequence["source_offset"]) + int(sequence["source_size"]) > int(src["size"]) or seen_sequences.has(int(sequence["sequence_index"])):
			_error = "sequence fields differ from reader contract"
			return false
		seen_sequences[int(sequence["sequence_index"])] = true
		var steps: Variant = sequence.get("steps")
		if not steps is Array or steps.size() > 1024:
			_error = "sequence step list exceeds reader bounds"
			return false
		for step in steps:
			if not step is Dictionary or not _exact_keys(step, ["frame_index", "duration"]) or not _exact_int(step.get("frame_index"), 32767) or not seen.has(int(step["frame_index"])) or not _exact_int(step.get("duration"), 32767) or int(step["duration"]) < 1:
				_error = "sequence references missing frame or invalid duration"
				return false
	var palette: Variant = sd.get("palette")
	if not palette is Dictionary or not _exact_keys(palette, ["mode", "source_words", "stp_policy"]) or not _exact_int(palette.get("mode"), 2) or int(palette["mode"]) != 2 or not _uint_array(palette.get("source_words"), 256, 65535) or not _uint_arrays_equal(palette["source_words"], frames[0]["source_palette_words"]) or palette.get("stp_policy") != SD_STP_POLICY:
		_error = "selected SD palette metadata is invalid"
		return false
	return true

func _validate_payload_spec(spec: Variant) -> bool:
	if not spec is Dictionary or not _exact_keys(spec, ["path", "size", "sha256"]):
		_error = "PNG descriptor fields differ from v1 contract"
		return false
	var rel: String = str(spec.get("path", ""))
	if not rel.is_valid_filename() or rel.contains("/") or rel.contains("\\") or not rel.ends_with(".png") or not _exact_int(spec.get("size"), MAX_PNG) or int(spec["size"]) < 1 or not _is_sha(str(spec.get("sha256", ""))):
		_error = "PNG descriptor path, size, or hash is invalid"
		return false
	return true

func _load_all_payloads(components: Dictionary) -> bool:
	for key in ["command_0_background", "command_7_background", "holiday_room_background", "holiday_command_0_background"]:
		if not components.has(key):
			continue
		var bg: Dictionary = components[key]
		for item in [["image", Vector2i(320, 88)], ["stp_mask", Vector2i(320, 88)], ["padded_image", Vector2i(320, 96)], ["padded_stp_mask", Vector2i(320, 96)]]:
			if not _load_png(bg[item[0]], item[1]):
				return false
	for key in ["command_0_sd", "command_7_sd", "holiday_command_0_sd"]:
		if not components.has(key):
			continue
		var sd: Dictionary = components[key]
		for frame in sd["frames"]:
			var size := Vector2i(int(frame["bounds"][2]), int(frame["bounds"][3]))
			if not _load_png(frame["png"], size) or not _load_png(frame["stp_png"], size):
				return false
	return true

func _load_png(spec: Dictionary, expected_size: Vector2i) -> bool:
	var rel: String = str(spec["path"])
	var path := _root.path_join("assets").path_join(rel)
	if _is_link(path) or not FileAccess.file_exists(path):
		_error = "PNG payload is missing or linked"
		return false
	var size := FileAccess.get_size(path)
	if size < 1 or size > MAX_PNG or size != int(spec["size"]):
		_error = "PNG payload exceeds bound or has invalid size"
		return false
	var file := FileAccess.open(path, FileAccess.READ)
	if file == null:
		_error = "PNG payload cannot be read"
		return false
	var bytes := file.get_buffer(size)
	file.close()
	if bytes.size() != size or _sha(bytes) != str(spec["sha256"]):
		_error = "PNG payload hash mismatch"
		return false
	var header_size := _png_header_size(bytes)
	if header_size.x < 1 or header_size.y < 1 or header_size.x > 8192 or header_size.y > 8192 or header_size.x * header_size.y > MAX_PIXELS or header_size != expected_size:
		_error = "PNG header dimensions exceed or differ from descriptor"
		return false
	var image := Image.new()
	if image.load_png_from_buffer(bytes) != OK or image.is_empty() or image.get_size() != expected_size:
		_error = "PNG decode failed after bounded header validation"
		return false
	_loaded_images[rel] = image
	return true

func _png_header_size(bytes: PackedByteArray) -> Vector2i:
	if bytes.size() < 24 or bytes.slice(0, 8) != PackedByteArray([137, 80, 78, 71, 13, 10, 26, 10]) or bytes.slice(8, 12) != PackedByteArray([0, 0, 0, 13]) or bytes.slice(12, 16) != PackedByteArray([73, 72, 68, 82]):
		return Vector2i(-1, -1)
	var width := (int(bytes[16]) << 24) | (int(bytes[17]) << 16) | (int(bytes[18]) << 8) | int(bytes[19])
	var height := (int(bytes[20]) << 24) | (int(bytes[21]) << 16) | (int(bytes[22]) << 8) | int(bytes[23])
	return Vector2i(width, height)

func _validate_part(part: Variant, source_size: int) -> bool:
	if not part is Dictionary or not _exact_keys(part, PART_KEYS):
		_error = "SD part metadata fields differ from reader contract"
		return false
	for key in ["x", "y"]:
		if not _signed_int(part.get(key), -32768, 32767):
			_error = "SD part origin exceeds signed 16-bit range"
			return false
	var limits := {"part_index": [0, 255], "source_record_offset": [0, source_size - 1], "width": [1, 128], "height": [1, 128], "texture_id": [0, 255], "sheet_index": [0, 1], "sheet_source_offset": [0, source_size - 1], "u": [0, 127], "v": [0, 127], "palette_info": [0, 65535], "flip_flags": [0, 3], "palette_source_offset": [0, 511], "runtime_palette_override": [0, 255]}
	for key in limits:
		if not _range_int(part.get(key), limits[key][0], limits[key][1]):
			_error = "invalid bounded SD part field: " + key
			return false
	if int(part["u"]) + int(part["width"]) > 128 or int(part["v"]) + int(part["height"]) > 128 or not _exact_int_array(part.get("clut_vram"), [0, 496]) or part.get("palette_mapping") != PART_PALETTE_MAPPING:
		_error = "SD part texture/palette metadata is outside the selected reader profile"
		return false
	return true

func _metadata_equal(actual: Variant, expected: Variant) -> bool:
	if expected is Dictionary:
		if not actual is Dictionary or not _exact_keys(actual, expected.keys()):
			return false
		for key in expected:
			if not _metadata_equal(actual[key], expected[key]):
				return false
		return true
	if expected is Array:
		if not actual is Array or actual.size() != expected.size():
			return false
		for i in range(expected.size()):
			if not _metadata_equal(actual[i], expected[i]):
				return false
		return true
	if expected is float or expected is int:
		return _range_int(actual, -2147483648, 4294967295) and float(actual) == float(expected)
	return typeof(actual) == typeof(expected) and actual == expected


func _validate_route_tables(actual: Variant, expected: Dictionary) -> bool:
	if not actual is Dictionary or not _exact_keys(actual, ["schema", "values", "provenance"]) or actual["schema"] != expected["schema"] or not _metadata_equal(actual["provenance"], expected["provenance"]) or not actual["values"] is Dictionary or not _exact_keys(actual["values"], expected["provenance"].keys()):
		return false
	for name in expected["provenance"]:
		var proof: Dictionary = expected["provenance"][name]
		var values: Variant = actual["values"][name]
		if not values is Array or values.size() != int(proof["shape"][0]):
			return false
		var signed: bool = proof["type"] == "s32"
		var width: int = 4 if signed else 1
		var raw := PackedByteArray()
		for value in values:
			if not _range_int(value, -2147483648 if signed else 0, 2147483647 if signed else 255):
				return false
			for byte in range(width):
				raw.append((int(value) >> (8 * byte)) & 255)
		if _sha(raw) != proof["raw_sha256"]:
			return false
	return true

func _integer_metadata(value: Variant) -> Variant:
	# Source metadata is integral; JSON parsing exposes numbers as floats.
	if value is float:
		return int(value)
	if value is Array:
		var result: Array = []
		for item in value:
			result.append(_integer_metadata(item))
		return result
	if value is Dictionary:
		var result: Dictionary = {}
		for key in value:
			result[key] = _integer_metadata(value[key])
		return result
	return value


func _safe_root(path: String) -> bool:
	return ExternalContentPath.is_external_path(path) and DirAccess.dir_exists_absolute(path)

func _is_link(path: String) -> bool:
	var dir := DirAccess.open(path.get_base_dir())
	return dir != null and dir.is_link(path.get_file())

func _exact_keys(value: Dictionary, expected: Array) -> bool:
	if value.size() != expected.size():
		return false
	for key in expected:
		if not value.has(key):
			return false
	return true

func _exact_int(value: Variant, maximum: int) -> bool:
	if value is bool or not (value is int or value is float):
		return false
	var n := float(value)
	return is_finite(n) and n >= 0.0 and n <= float(maximum) and floor(n) == n

func _range_int(value: Variant, minimum: int, maximum: int) -> bool:
	if value is bool or not (value is int or value is float):
		return false
	var n := float(value)
	return is_finite(n) and n >= float(minimum) and n <= float(maximum) and floor(n) == n

func _signed_int(value: Variant, minimum: int, maximum: int) -> bool:
	return _range_int(value, minimum, maximum)

func _exact_int_array(value: Variant, expected: Array) -> bool:
	if not value is Array or value.size() != expected.size():
		return false
	for i in range(expected.size()):
		if not _range_int(value[i], expected[i], expected[i]) or int(value[i]) != expected[i]:
			return false
	return true

func _signed_array(value: Variant, size: int, minimum: int, maximum: int) -> bool:
	if not value is Array or value.size() != size:
		return false
	for item in value:
		if not _signed_int(item, minimum, maximum):
			return false
	return true

func _uint_array(value: Variant, size: int, maximum: int) -> bool:
	if not value is Array or value.size() != size:
		return false
	for item in value:
		if not _exact_int(item, maximum):
			return false
	return true

func _uint_arrays_equal(left: Variant, right: Variant) -> bool:
	if not left is Array or not right is Array or left.size() != right.size():
		return false
	for i in range(left.size()):
		if not _range_int(left[i], 0, 65535) or not _range_int(right[i], 0, 65535) or int(left[i]) != int(right[i]):
			return false
	return true

func _string_array_equals(value: Variant, expected: Array) -> bool:
	if not value is Array or value.size() != expected.size():
		return false
	for i in range(expected.size()):
		if not value[i] is String or value[i] != expected[i]:
			return false
	return true

func _valid_evidence(value: Variant) -> bool:
	if not value is Array or value.is_empty() or value.size() > 32:
		return false
	for item in value:
		if not item is Dictionary or not _exact_keys(item, ["path", "sha256", "size"]) or not _is_sha(str(item.get("sha256", ""))) or not _range_int(item.get("size"), 1, 100000000):
			return false
	return true

func _is_sha(value: String) -> bool:
	if value.length() != 64:
		return false
	for c in value:
		if not "0123456789abcdef".contains(c):
			return false
	return true

func _sha(bytes: PackedByteArray) -> String:
	var ctx := HashingContext.new()
	ctx.start(HashingContext.HASH_SHA256)
	ctx.update(bytes)
	return ctx.finish().hex_encode()

func _clear() -> void:
	_root = ""
	_components.clear()
	_route_tables.clear()
	_loaded_images.clear()
	_error = ""

func _fail(message: String) -> bool:
	_clear()
	_error = message
	return false
