extends RefCounted
## Strict reader for the isolated April command 1–3 content pack.

const SCHEMA := "tokimemo.spring-school-day-content.v1"
const MANIFEST_SHA := "e5d3e8034bb8ece4ba7f0b7da66a8650f189a3aba9b83023eee4d76170d8a874"
const LEGACY_MANIFEST_SHA := "b26ef7e5c0301633c45d9a6a898ca4bdb7be68ee30f163825eed827e4f5c0c28"
const FRESH_TABLES_SHA := "e9b83b8d2ad902430628791146d32daad7aacc66a6ac53ac628c3b70d00bb362"
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const EXE_SHA := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const TABLES_SHA := "a1f30e57ef4f40ee7f9786043ccc5e007cfe316ba42fbd2c3e6896bf1d5f7060"
const MEDIA_SHA := "0c85bbc54be82c7b2597528ccbfc915cd1fb5f99405bd18d845452026b56be37"
const READERS := ["tools/assets/input.py:open_input", "tools/assets/image_packs_sd.py:export_sd_ui",
	"tools/assets/bs_cohort_profile.py:build_profile_context", "tools/assets/image_mdec.py:export_mdec"]
const CAPABILITIES := ["command_1_april_school_day", "command_2_april_school_day", "command_3_april_school_day"]
const MAX_MANIFEST := 8 * 1024 * 1024
const MAX_PNG := 2 * 1024 * 1024
const ExternalContentPath = preload("res://services/external_content_path.gd")

var _root := ""
var _components: Dictionary = {}
var _error := ""

func open_pack(root: String, expected_manifest_sha256: String = MANIFEST_SHA) -> bool:
	_clear()
	if expected_manifest_sha256 not in [MANIFEST_SHA, LEGACY_MANIFEST_SHA]:
		return _fail("spring content requires the reviewed manifest identity")
	if not _canonical_posix_path(root):
		return _fail("spring content path must be a canonical absolute POSIX path")
	if not ExternalContentPath.is_external_path(root):
		var project_root := ProjectSettings.globalize_path("res://").simplify_path().trim_suffix("/")
		if root == project_root or root.begins_with(project_root + "/"):
			return _fail("spring content must remain outside res:// under repository .asset-cache/port")
		if _has_symlink(root):
			return _fail("spring content pack path contains a symlink")
		return _fail("spring content pack path is not a safe external path")
	if _has_symlink(root):
		return _fail("spring content pack path contains a symlink")
	if not DirAccess.dir_exists_absolute(root):
		return _fail("spring content pack directory is missing")
	var canonical := root
	var assets_root := canonical.path_join("assets")
	if _has_symlink(assets_root) or not DirAccess.dir_exists_absolute(assets_root):
		return _fail("spring content assets directory is missing, linked, or unsafe")
	var manifest_path := canonical.path_join("manifest.json")
	if _is_link(manifest_path) or not FileAccess.file_exists(manifest_path) or FileAccess.get_size(manifest_path) > MAX_MANIFEST:
		return _fail("spring content manifest is missing, linked, or oversized")
	if FileAccess.get_sha256(manifest_path) != expected_manifest_sha256:
		return _fail("spring content manifest hash differs from reviewed pack")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or not _keys(parsed, ["schema", "version", "provenance", "route_capabilities", "unsupported", "routes", "components"]):
		return _fail("spring content manifest fields differ from contract")
	if parsed.get("schema") != SCHEMA or parsed.get("version") != 1:
		return _fail("unsupported spring content schema")
	var provenance: Variant = parsed.get("provenance")
	var fresh := expected_manifest_sha256 == MANIFEST_SHA
	var expected_tables := FRESH_TABLES_SHA if fresh else TABLES_SHA
	var expected_media: Variant = null if fresh else MEDIA_SHA
	if not provenance is Dictionary or provenance.get("original_input_sha256") != TRACK_SHA or provenance.get("original_executable_sha256") != EXE_SHA or provenance.get("rule_tables_manifest_sha256") != expected_tables or provenance.get("reviewed_media_publication_sha256") != expected_media or provenance.get("readers") != READERS:
		return _fail("spring content provenance differs from reviewed inputs")
	if parsed.get("route_capabilities") != CAPABILITIES:
		return _fail("spring route capability set differs from reviewed tranche")
	var unsupported: Variant = parsed.get("unsupported")
	var expected_unsupported := {"command_4": "club/club-id dependent profile not included", "commands_5_6": "separate BS cohort not profiled", "seasonal": "only non-summer table; summer selector variants not included", "vacation": "not included", "sick_rest": "not included", "conditional_companion_variants": "only opening-week event extra 0xF; companion and athletics-special variants not included", "scene_composition": "not established", "full_route": "not claimed"}
	if unsupported != expected_unsupported:
		return _fail("unsupported ordinary route variants are not explicit")
	var routes: Variant = parsed.get("routes")
	var components: Variant = parsed.get("components")
	if not routes is Array or routes.size() != 3 or not components is Dictionary or not _keys(components, ["command_1", "command_2", "command_3"]):
		return _fail("spring content must contain exactly commands 1–3")
	_root = canonical
	var expected := {1: [16141, 3, 7, 42705], 2: [16151, 5, 11, 42869], 3: [16160, 7, 13, 42951]}
	for index in range(3):
		var command := index + 1
		var route: Variant = routes[index]
		var values: Array = expected[command]
		if not route is Dictionary or route.get("command") != command or route.get("event_extra") != 15 or route.get("variant") != 1 or route.get("background_lba") != values[0] or route.get("sd_table_index") != values[1] or route.get("sd_set_id") != values[2] or route.get("sd_lba") != values[3] or route.get("season_domain") != "months 3–5 (non-summer table)" or route.get("route_domain") != "ordinary school day, non-vacation, non-sick-rest":
			return _fail("route table selection differs from original April table values")
		var component: Variant = components["command_%d" % command]
		if not component is Dictionary or component.get("route") != route:
			return _fail("command %d component route differs from selected original route" % command)
		if not _valid_background(component.get("background"), command, values[0]):
			return _fail("command %d background source/geometry payload is invalid" % command)
		if not _valid_sd(component.get("sd"), command, values[2], values[3]):
			return _fail("command %d SD source/table/animation payload is invalid" % command)
	_root = canonical
	_components = components.duplicate(true)
	_error = ""
	return true

func get_command(command: int) -> Dictionary:
	if command not in [1, 2, 3]:
		return {}
	return _components.get("command_%d" % command, {}).duplicate(true)

func error_message() -> String:
	return _error

func _valid_background(value: Variant, command: int, expected_lba: int) -> bool:
	if not value is Dictionary or not _keys(value, ["source", "geometry_context", "reader", "files", "decoder_model"]) or value.get("reader") != "tools/assets/image_mdec.py:export_mdec":
		return false
	if not preload("res://services/mdec_model_identity.gd").valid(value): return false
	var source: Variant = value.get("source")
	var expected_path := "CDROM/BS/C_01_%02d.BS" % (27 + command)
	if not source is Dictionary or source.get("path") != expected_path or source.get("location", {}).get("lba") != expected_lba or source.get("size", 0) < 1 or not _sha_ok(source.get("sha256")):
		return false
	var files: Variant = value.get("files")
	if not files is Dictionary or files.size() < 4:
		return false
	for spec in files.values():
		if not _verify_png(spec):
			return false
	return true

func _valid_sd(value: Variant, command: int, set_id: int, lba: int) -> bool:
	if not value is Dictionary or value.get("reader") != "tools/assets/image_packs_sd.py:export_sd_ui" or value.get("profile_index") != set_id or value.get("source_id", "") == "":
		return false
	var source: Variant = value.get("source")
	var expected_paths := {7: "CDROM/MAIN_SD/SD_P03W.NBN", 11: "CDROM/MAIN_SD/SD_P05W.NBN", 13: "CDROM/MAIN_SD/SD_P07.NBN"}
	var expected_path: String = expected_paths[set_id]
	if not source is Dictionary or source.get("path") != expected_path or source.get("lba") != lba or source.get("size") != 83968 or source.get("exe_sha256") != EXE_SHA or not _sha_ok(source.get("sha256")):
		return false
	var frames: Variant = value.get("frames")
	var sequences: Variant = value.get("sequences")
	if not frames is Array or frames.is_empty() or frames.size() > 512 or not sequences is Array or sequences.is_empty():
		return false
	for frame in frames:
		if not frame is Dictionary or not frame.get("bounds") is Array or frame["bounds"].size() != 4 or not frame.get("origin") is Array or frame["origin"].size() != 2 or not frame.get("source_palette_words") is Array or frame["source_palette_words"].size() != 256 or not _verify_png(frame.get("png")) or not _verify_png(frame.get("stp_png")):
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

func _canonical_posix_path(value: String) -> bool:
	if not value.begins_with("/") or value.begins_with("//") or value.contains("\\") or value.contains("://") or value != value.simplify_path() or (value.length() > 1 and value.ends_with("/")):
		return false
	for part in value.trim_prefix("/").split("/", true):
		if part.is_empty() or part == "." or part == "..":
			return false
	return true

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
