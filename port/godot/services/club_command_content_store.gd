extends RefCounted
## Strict reader for the Command 4 club content pack.
## Supports all 11 non-summer club activities (clubs 0..10) for opening week solo activity.

const SCHEMA := "tokimemo.club-command-content.v1"
const MANIFEST_SHA := "00b844999ad42c174a4163906ca6552929d48ebbe9b96e12d6285b7e2aae65d9"
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const EXE_SHA := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const READERS := [
	"tools/assets/input.py:open_input",
	"tools/assets/image_packs_sd.py:export_sd_ui",
	"tools/assets/image_mdec.py:export_mdec",
]
const CAPABILITIES := [
	"command_4_club_0_april_school_day",
	"command_4_club_1_april_school_day",
	"command_4_club_2_april_school_day",
	"command_4_club_3_april_school_day",
	"command_4_club_4_april_school_day",
	"command_4_club_5_april_school_day",
	"command_4_club_6_april_school_day",
	"command_4_club_7_april_school_day",
	"command_4_club_8_april_school_day",
	"command_4_club_9_april_school_day",
	"command_4_club_10_april_school_day",
]
const MAX_MANIFEST := 8 * 1024 * 1024
const MAX_PNG := 2 * 1024 * 1024

const EXPECTED_ROUTES: Dictionary = {
	0: {"bg_lba": 16215, "sd_set": 3, "sd_lba": 27957},
	1: {"bg_lba": 16224, "sd_set": 31, "sd_lba": 29105},
	2: {"bg_lba": 16234, "sd_set": 7, "sd_lba": 28121},
	3: {"bg_lba": 16244, "sd_set": 11, "sd_lba": 28285},
	4: {"bg_lba": 16252, "sd_set": 29, "sd_lba": 29023},
	5: {"bg_lba": 16260, "sd_set": 25, "sd_lba": 28859},
	6: {"bg_lba": 16168, "sd_set": 13, "sd_lba": 28367},
	7: {"bg_lba": 16177, "sd_set": 15, "sd_lba": 28449},
	8: {"bg_lba": 16206, "sd_set": 21, "sd_lba": 28695},
	9: {"bg_lba": 16197, "sd_set": 19, "sd_lba": 28613},
	10: {"bg_lba": 16187, "sd_set": 17, "sd_lba": 28531},
}

var _root := ""
var _components: Dictionary = {}
var _error := ""

func open_pack(root: String, expected_manifest_sha256: String = MANIFEST_SHA) -> bool:
	_clear()
	if not root.is_absolute_path() or expected_manifest_sha256 != MANIFEST_SHA:
		return _fail("club content requires the reviewed absolute pack identity")
	var canonical := root.simplify_path()
	if canonical.begins_with("//") or _has_symlink(canonical):
		return _fail("club content pack path is unsafe")
	var assets_root := canonical.path_join("assets")
	if _has_symlink(assets_root) or not DirAccess.dir_exists_absolute(assets_root):
		return _fail("club content assets directory is missing, linked, or unsafe")
	var manifest_path := canonical.path_join("manifest.json")
	if _is_link(manifest_path) or not FileAccess.file_exists(manifest_path) or FileAccess.get_size(manifest_path) > MAX_MANIFEST:
		return _fail("club content manifest is missing, linked, or oversized")
	if FileAccess.get_sha256(manifest_path) != expected_manifest_sha256:
		return _fail("club content manifest hash differs from reviewed pack")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or not _keys(parsed, ["schema", "version", "provenance", "route_capabilities", "unsupported", "routes", "components"]):
		return _fail("club content manifest fields differ from contract")
	if parsed.get("schema") != SCHEMA or parsed.get("version") != 1:
		return _fail("unsupported club content schema")
	var provenance: Variant = parsed.get("provenance")
	if not provenance is Dictionary or provenance.get("original_input_sha256") != TRACK_SHA or provenance.get("original_executable_sha256") != EXE_SHA or provenance.get("readers") != READERS:
		return _fail("club content provenance differs from reviewed inputs")
	if parsed.get("route_capabilities") != CAPABILITIES:
		return _fail("club route capability set differs from reviewed tranche")
	var routes: Variant = parsed.get("routes")
	var components: Variant = parsed.get("components")
	if not routes is Array or routes.size() != 11 or not components is Dictionary or components.size() != 11:
		return _fail("club content must contain exactly 11 clubs (0..10)")

	_root = canonical
	for club_id in range(11):
		var route: Variant = routes[club_id]
		var expected_info: Dictionary = EXPECTED_ROUTES[club_id]
		if not route is Dictionary or route.get("command") != 4 or route.get("club_id") != club_id or route.get("event_extra") != 15 or route.get("variant") != 1 or route.get("background_lba") != expected_info["bg_lba"] or route.get("sd_set_id") != expected_info["sd_set"] or route.get("sd_lba") != expected_info["sd_lba"] or route.get("season_domain") != "months 3–5 (non-summer table)" or route.get("route_domain") != "ordinary school day, non-vacation, non-sick-rest":
			return _fail("route table selection for club %d differs from original table values" % club_id)
		var comp_key := "club_%d" % club_id
		var component: Variant = components.get(comp_key)
		if not component is Dictionary or component.get("route") != route:
			return _fail("club %d component route differs from selected original route" % club_id)
		if not _valid_background(component.get("background"), club_id, expected_info["bg_lba"]):
			return _fail("club %d background source/geometry payload is invalid" % club_id)
		if not _valid_sd(component.get("sd"), club_id, expected_info["sd_set"], expected_info["sd_lba"]):
			return _fail("club %d SD source/table/animation payload is invalid" % club_id)

	_root = canonical
	_components = components.duplicate(true)
	_error = ""
	return true

func has_club(club_id: int) -> bool:
	return _components.has("club_%d" % club_id)

func get_club(club_id: int) -> Dictionary:
	return _components.get("club_%d" % club_id, {}).duplicate(true)

func get_background(club_id: int) -> Image:
	var comp: Dictionary = get_club(club_id)
	if comp.is_empty():
		return null
	var bg_files: Dictionary = comp.get("background", {}).get("files", {})
	for fname: String in bg_files:
		if fname.ends_with(".background.png") and not fname.ends_with(".padded.png"):
			var full_path := _root.path_join("assets").path_join(bg_files[fname]["path"])
			var img := Image.new()
			if img.load(full_path) == OK and img.get_size() == Vector2i(320, 88):
				return img
	return null

func get_sd_frame(club_id: int, frame_index: int) -> Dictionary:
	var comp: Dictionary = get_club(club_id)
	if comp.is_empty():
		return {}
	var frames: Array = comp.get("sd", {}).get("frames", [])
	for frame in frames:
		if int(frame.get("frame_index", -1)) == frame_index:
			var png_path := _root.path_join("assets").path_join(frame.get("png", {}).get("path", ""))
			var img := Image.new()
			if img.load(png_path) != OK or img.is_empty():
				return {}
			var bounds: Array = frame["bounds"]
			var origin: Array = frame["origin"]
			return {
				"id": "command_4_club_%d_frame_%d" % [club_id, frame_index],
				"image": img,
				"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
				"origin": Vector2i(int(origin[0]), int(origin[1]))
			}
	return {}

func error_message() -> String:
	return _error

func _valid_background(value: Variant, club_id: int, expected_lba: int) -> bool:
	if not value is Dictionary or not _keys(value, ["source", "geometry_context", "reader", "files"]) or value.get("reader") != "tools/assets/image_mdec.py:export_mdec":
		return false
	var source: Variant = value.get("source")
	if not source is Dictionary or source.get("lba") != expected_lba or source.get("size", 0) < 1 or not _sha_ok(source.get("sha256")):
		return false
	var files: Variant = value.get("files")
	if not files is Dictionary or files.size() < 4:
		return false
	for spec in files.values():
		if not _verify_png(spec):
			return false
	return true

func _valid_sd(value: Variant, club_id: int, set_id: int, lba: int) -> bool:
	if not value is Dictionary or value.get("reader") != "tools/assets/image_packs_sd.py:export_sd_ui" or value.get("profile_index") != set_id:
		return false
	var source: Variant = value.get("source")
	if not source is Dictionary or source.get("lba") != lba or source.get("size") != 83968 or source.get("exe_sha256") != EXE_SHA or not _sha_ok(source.get("sha256")):
		return false
	var frames: Variant = value.get("frames")
	var sequences: Variant = value.get("sequences")
	if not frames is Array or frames.is_empty() or not sequences is Array or sequences.is_empty():
		return false
	for frame in frames:
		if not frame is Dictionary or not frame.get("bounds") is Array or frame["bounds"].size() != 4 or not frame.get("origin") is Array or frame["origin"].size() != 2 or not _verify_png(frame.get("png")) or not _verify_png(frame.get("stp_png")):
			return false
	return true

func _verify_png(spec: Variant) -> bool:
	if not spec is Dictionary or not _keys(spec, ["path", "size", "sha256"]):
		return false
	var path: String = spec.get("path", "")
	var size_value: Variant = spec.get("size")
	if path.is_empty() or path.get_file() != path or path.contains("..") or path.contains("/") or path.contains("\\") or not _sha_ok(spec.get("sha256")) or typeof(size_value) not in [TYPE_INT, TYPE_FLOAT] or int(size_value) != size_value or int(size_value) < 8 or int(size_value) > MAX_PNG:
		return false
	var full := _root.path_join("assets").path_join(path)
	if _has_symlink(full) or not FileAccess.file_exists(full) or FileAccess.get_size(full) != int(size_value) or FileAccess.get_sha256(full) != spec["sha256"]:
		return false
	var image := Image.new()
	if image.load(full) != OK or image.is_empty() or image.get_width() * image.get_height() > 4 * 1024 * 1024:
		return false
	return true

func _sha_ok(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character in value:
		if not "0123456789abcdef".contains(character):
			return false
	return true

func _keys(value: Dictionary, expected: Array) -> bool:
	var actual := value.keys()
	actual.sort()
	var wanted := expected.duplicate()
	wanted.sort()
	return actual == wanted

func _has_symlink(path: String) -> bool:
	var current := "/"
	for part in path.trim_prefix("/").split("/"):
		if part.is_empty():
			continue
		current = current.path_join(part)
		if _is_link(current):
			return true
	return false

func _is_link(path: String) -> bool:
	var dir := DirAccess.open(path.get_base_dir())
	return dir != null and dir.is_link(path.get_file())

func _clear() -> void:
	_root = ""
	_components.clear()
	_error = ""

func _fail(message: String) -> bool:
	_clear()
	_error = message
	return false
