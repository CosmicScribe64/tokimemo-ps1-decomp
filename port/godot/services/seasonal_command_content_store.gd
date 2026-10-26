extends RefCounted
## Strict content store for the seasonal, condition, and vacation command variants pack.
## Supports summer uniform variants, vacation variants, and sick rest condition variants.

const SCHEMA := "tokimemo.seasonal-command-content.v1"
const MANIFEST_SHA := "236e1b2c12a69eb1b5f82960c572771c63520cc842cf79e43314c7b6262bc61e"
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
	if not expected_manifest_sha256.is_empty() and FileAccess.get_sha256(manifest_path) != expected_manifest_sha256:
		return _fail("seasonal content manifest hash differs from expected pack")

	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or not _keys(parsed, ["schema", "version", "provenance", "routes", "backgrounds", "sd_sets", "components"]):
		return _fail("seasonal content manifest fields differ from contract")
	if parsed.get("schema") != SCHEMA or parsed.get("version") != 1:
		return _fail("unsupported seasonal content schema")

	var provenance: Variant = parsed.get("provenance")
	if not provenance is Dictionary or provenance.get("original_input_sha256") != TRACK_SHA or provenance.get("original_executable_sha256") != EXE_SHA or provenance.get("readers") != READERS:
		return _fail("seasonal content provenance differs from reviewed inputs")

	_root = canonical
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
	var full_path := _root.path_join("assets").path_join(target_rel)
	if not FileAccess.file_exists(full_path):
		return null
	var img := Image.new()
	if img.load(full_path) != OK or img.get_size() != Vector2i(320, 88):
		return null
	return img

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
			var full_path := _root.path_join("assets").path_join(rel_path)
			if not FileAccess.file_exists(full_path):
				return {}
			var img := Image.new()
			if img.load(full_path) != OK or img.is_empty():
				return {}
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

func _clear() -> void:
	_root = ""
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
