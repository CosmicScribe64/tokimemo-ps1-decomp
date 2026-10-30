extends RefCounted
## Strict content store for the seasonal, condition, and vacation command variants pack.
## Supports summer uniform variants, vacation variants, and sick rest condition variants.

const SCHEMA := "tokimemo.seasonal-command-content.v1"
const MANIFEST_SHA := "a39933d4c22567cd9355c81c0401031cf42350b07073f43b79d3eb7e83e89384"
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const EXE_SHA := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const READERS := [
	"tools/assets/input.py:open_input",
	"tools/assets/image_packs_sd.py:export_sd_ui",
	"tools/assets/image_mdec.py:export_mdec",
]

const MAX_MANIFEST := 16 * 1024 * 1024
const MAX_PNG := 2 * 1024 * 1024

var _root := ""
var _routes: Array = []
var _components: Dictionary = {}
var _backgrounds: Dictionary = {}
var _sd_sets: Dictionary = {}
var _error := ""
var _images: Dictionary = {}
var _decoded_bytes := 0

func open_pack(root: String, expected_manifest_sha256: String = "") -> bool:
	_clear()
	if not root.is_absolute_path():
		return _fail("seasonal content requires an absolute pack path")
	var canonical := root.simplify_path()
	if canonical.begins_with("//") or _has_symlink(canonical):
		return _fail("seasonal content pack path is unsafe")
	var assets_root := canonical.path_join("assets")
	if _has_symlink(assets_root) or not DirAccess.dir_exists_absolute(assets_root):
		return _fail("seasonal content assets directory is missing, linked, or unsafe")
	var manifest_path := canonical.path_join("manifest.json")
	if _is_link(manifest_path) or not FileAccess.file_exists(manifest_path) or FileAccess.get_size(manifest_path) > MAX_MANIFEST:
		return _fail("seasonal content manifest is missing, linked, or oversized")
	if expected_manifest_sha256.is_empty(): expected_manifest_sha256 = MANIFEST_SHA
	if FileAccess.get_sha256(manifest_path) != expected_manifest_sha256:
		return _fail("seasonal content manifest hash differs from expected pack")

	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or not _keys(parsed, ["schema", "version", "provenance", "routes", "backgrounds", "sd_sets", "components"]):
		return _fail("seasonal content manifest fields differ from contract")
	if parsed.get("schema") != SCHEMA or parsed.get("version") != 1:
		return _fail("unsupported seasonal content schema")

	var provenance: Variant = parsed.get("provenance")
	if not provenance is Dictionary or provenance.get("original_input_sha256") != TRACK_SHA or provenance.get("original_executable_sha256") != EXE_SHA or provenance.get("readers") != READERS:
		return _fail("seasonal content provenance differs from reviewed inputs")
	if not parsed.get("backgrounds") is Dictionary:
		return _fail("seasonal backgrounds are missing")
	for background: Variant in parsed.backgrounds.values():
		if not preload("res://services/mdec_model_identity.gd").valid(background):
			return _fail("seasonal background decoder model differs")

	_root = canonical
	if not _prepare_payloads(parsed): return _fail("seasonal payload path/hash/size/geometry differs")
	_routes = parsed.get("routes", [])
	_components = parsed.get("components", {})
	_backgrounds = parsed.get("backgrounds", {})
	_sd_sets = parsed.get("sd_sets", {})
	_error = ""
	return true

func has_variant(command: int, club_id: int = 0, is_summer: bool = false, condition_bucket: int = 0, vacation_kind: int = 0, season_index: int = 0) -> bool:
	var s_idx: int = season_index if condition_bucket == 1 else (1 if is_summer else 0)
	var key := _route_key(command, club_id, is_summer, condition_bucket, vacation_kind, s_idx)
	if _components.has(key):
		return true
	return _components.has(_route_key(command, club_id, is_summer, condition_bucket, vacation_kind, 0)) or _components.has(_route_key(command, club_id, is_summer, condition_bucket, vacation_kind, 1))

func get_variant_component(command: int, club_id: int = 0, is_summer: bool = false, condition_bucket: int = 0, vacation_kind: int = 0, season_index: int = 0) -> Dictionary:
	var s_idx: int = season_index if condition_bucket == 1 else (1 if is_summer else 0)
	var key := _route_key(command, club_id, is_summer, condition_bucket, vacation_kind, s_idx)
	if _components.has(key):
		return _components[key].duplicate(true)
	var fallback_0 := _route_key(command, club_id, is_summer, condition_bucket, vacation_kind, 0)
	if _components.has(fallback_0):
		return _components[fallback_0].duplicate(true)
	var fallback_1 := _route_key(command, club_id, is_summer, condition_bucket, vacation_kind, 1)
	if _components.has(fallback_1):
		return _components[fallback_1].duplicate(true)
	return {}

func resolve_variant(command: int, club_id: int = 0, is_summer: bool = false, condition_bucket: int = 0, vacation_kind: int = 0, season_index: int = 0) -> Dictionary:
	var comp: Dictionary = get_variant_component(command, club_id, is_summer, condition_bucket, vacation_kind, season_index)
	if comp.is_empty():
		return {}
	return comp.get("route", {}).duplicate(true)

func get_background(bg_id: String) -> Image:
	if not _backgrounds.has(bg_id):
		return null
	var bg_meta: Dictionary = _backgrounds[bg_id]
	var bg_files: Dictionary = bg_meta.get("files", {})
	var target_rel: String = ""
	for name: String in bg_files:
		if name.ends_with(".background.png"):
			target_rel = bg_files[name].get("path", "")
			break
	if target_rel.is_empty():
		return null
	var verified: Variant = _images.get(target_rel)
	return verified.duplicate() if verified is Image else null

func get_sd_frame(sd_id: String, frame_index: int) -> Dictionary:
	if not _sd_sets.has(sd_id):
		return {}
	var sd_meta: Dictionary = _sd_sets[sd_id]
	for frame: Dictionary in sd_meta.get("frames", []):
		if int(frame.get("frame_index", -1)) == frame_index:
			var png_info: Dictionary = frame.get("png", {})
			var rel_path: String = png_info.get("path", "")
			if rel_path.is_empty():
				return {}
			var verified: Variant = _images.get(rel_path)
			if not verified is Image: return {}
			var img: Image = verified.duplicate()
			var bounds: Array = frame.get("bounds", [0, 0, 0, 0])
			var origin: Array = frame.get("origin", [0, 0])
			return {
				"id": "%s_frame_%d" % [sd_id, frame_index],
				"image": img,
				"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
				"origin": Vector2i(int(origin[0]), int(origin[1])),
			}
	return {}

func get_sd_resources(sd_id: String) -> Dictionary:
	if not _sd_sets.has(sd_id):
		return {}
	var sd_meta: Dictionary = _sd_sets[sd_id]
	var seq_dict: Dictionary = {}
	for seq: Dictionary in sd_meta.get("sequences", []):
		seq_dict[str(int(seq.get("sequence_index", 0)))] = _integers(seq.get("steps", []))
	var frames: Dictionary = {}
	for frame_meta: Dictionary in sd_meta.get("frames", []):
		var f_idx: int = int(frame_meta.get("frame_index", 0))
		var frame_data: Dictionary = get_sd_frame(sd_id, f_idx)
		if not frame_data.is_empty():
			frames[str(f_idx)] = frame_data
	return {"sequences": seq_dict, "frames": frames}

func error_message() -> String:
	return _error

func _route_key(command: int, club_id: int, is_summer: bool, condition_bucket: int, vacation_kind: int, season_index: int) -> String:
	return "cmd_%d_club_%d_sum_%d_cond_%d_vac_%d_s_%d" % [
		command, club_id, 1 if is_summer else 0, condition_bucket, vacation_kind, season_index
	]

func _prepare_payloads(parsed: Dictionary) -> bool:
	if not parsed.get("sd_sets") is Dictionary or parsed.sd_sets.size()>256 or parsed.backgrounds.size()>256: return false
	if not parsed.get("routes") is Array or parsed.routes.size()>4096 or not parsed.get("components") is Dictionary or parsed.components.size()>4096: return false
	for route: Variant in parsed.routes:
		if not route is Dictionary: return false
	for component: Variant in parsed.components.values():
		if not component is Dictionary or not component.get("route") is Dictionary: return false
	for background: Variant in parsed.backgrounds.values():
		if not background is Dictionary or not background.get("files") is Dictionary: return false
		for name: Variant in background.files:
			if not name is String: return false
			var expected := Vector2i(320,96 if ".padded." in name else 88)
			if not _prepare_png(background.files[name],expected): return false
	for sd: Variant in parsed.sd_sets.values():
		if not sd is Dictionary or not sd.get("frames") is Array or sd.frames.is_empty() or sd.frames.size()>4096 or not sd.get("sequences") is Array or sd.sequences.is_empty() or sd.sequences.size()>4096: return false
		var frame_ids: Dictionary = {}
		for frame: Variant in sd.frames:
			if not frame is Dictionary or not frame.get("bounds") is Array or frame.bounds.size()!=4 or not frame.get("origin") is Array or frame.origin.size()!=2: return false
			if not _bounded_integer(frame.get("frame_index"),0,4095) or frame_ids.has(int(frame.frame_index)): return false
			frame_ids[int(frame.frame_index)] = true
			for number: Variant in frame.bounds+frame.origin:
				if not _bounded_integer(number,-4096,4096): return false
			var expected := Vector2i(int(frame.bounds[2]),int(frame.bounds[3]))
			if not _prepare_png(frame.get("png"),expected) or not _prepare_png(frame.get("stp_png"),expected): return false
		var sequence_ids: Dictionary = {}
		for sequence: Variant in sd.sequences:
			if not sequence is Dictionary or not _bounded_integer(sequence.get("sequence_index"),0,4095) or sequence_ids.has(int(sequence.sequence_index)): return false
			sequence_ids[int(sequence.sequence_index)] = true
			if not sequence.get("steps") is Array or sequence.steps.is_empty() or sequence.steps.size()>4096: return false
			for step: Variant in sequence.steps:
				if not step is Dictionary or not _bounded_integer(step.get("duration"),1,32767) or not _bounded_integer(step.get("frame_index"),0,4095) or not frame_ids.has(int(step.frame_index)): return false
	return true

static func _bounded_integer(value: Variant, low: int, high: int) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and float(value)==floor(float(value)) and value>=low and value<=high

func _prepare_png(spec: Variant, expected: Vector2i) -> bool:
	if not spec is Dictionary or not spec.get("path") is String or not spec.get("sha256") is String: return false
	var relative: String = spec.path
	var size: Variant = spec.get("size")
	if relative.is_empty() or relative.get_file()!=relative or "/" in relative or "\\" in relative or ":" in relative or not relative.ends_with(".png"): return false
	if not (size is int or size is float) or not is_finite(float(size)) or float(size)!=floor(float(size)) or size<33 or size>MAX_PNG: return false
	if expected.x<1 or expected.y<1 or expected.x>4096 or expected.y>4096 or expected.x*expected.y>4*1024*1024: return false
	var path := _root.path_join("assets").path_join(relative)
	if not preload("res://services/external_content_path.gd").is_external_file(path) or FileAccess.get_size(path)!=int(size): return false
	var file := FileAccess.open(path,FileAccess.READ)
	if file==null: return false
	var bytes := file.get_buffer(int(size))
	var hash := HashingContext.new(); hash.start(HashingContext.HASH_SHA256); hash.update(bytes)
	if bytes.size()!=int(size) or file.get_length()!=int(size) or hash.finish().hex_encode()!=spec.sha256: return false
	if bytes.slice(0,8).hex_encode()!="89504e470d0a1a0a": return false
	var width: int = (int(bytes[16])<<24)|(int(bytes[17])<<16)|(int(bytes[18])<<8)|int(bytes[19])
	var height: int = (int(bytes[20])<<24)|(int(bytes[21])<<16)|(int(bytes[22])<<8)|int(bytes[23])
	if Vector2i(width,height)!=expected: return false
	if _images.has(relative): return _images[relative].get_size()==expected
	_decoded_bytes += width*height*4
	if _decoded_bytes>128*1024*1024: return false
	var image := Image.new()
	if image.load_png_from_buffer(bytes)!=OK or image.get_size()!=expected: return false
	_images[relative] = image
	return true

func _clear() -> void:
	_root = ""
	_images.clear()
	_decoded_bytes = 0
	_routes.clear()
	_components.clear()
	_backgrounds.clear()
	_sd_sets.clear()
	_error = ""

func _fail(message: String) -> bool:
	_clear()
	_error = message
	return false

static func _has_symlink(path: String) -> bool:
	var current := "/" if path.begins_with("/") else ""
	for part in path.split("/", false):
		current = current.path_join(part)
		if _is_link(current):
			return true
	return false

static func _is_link(path: String) -> bool:
	if not FileAccess.file_exists(path) and not DirAccess.dir_exists_absolute(path):
		return false
	var dir := DirAccess.open(path.get_base_dir())
	if dir == null:
		return false
	return dir.is_link(path.get_file())

static func _keys(dict: Dictionary, required: Array) -> bool:
	for k in required:
		if not dict.has(k):
			return false
	return true

static func _integers(value: Variant) -> Variant:
	if value is Dictionary:
		var output: Dictionary = {}
		for key: String in value:
			output[key] = _integers(value[key])
		return output
	if value is Array:
		var output: Array = []
		for item: Variant in value:
			output.append(_integers(item))
		return output
	if value is float and is_finite(value) and value == floor(value) and abs(value) <= 4294967295.0:
		return int(value)
	return value
