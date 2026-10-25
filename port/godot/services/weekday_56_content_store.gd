extends RefCounted
## External source-backed capability reader for spring commands 5 and 6 (T-29014).
## The full manifest pin binds original selectors, palettes, frames and sequences.
## Gameplay composition, timing and native rendered parity remain unverified.

const SCHEMA := "tokimemo.weekday-56-content.v1"
const MANIFEST_SHA := "7c67739f2dcbc5a9c644e670d0c0e5ea91869ab851d686e77cbfbf6e188105b1"
const MAX_MANIFEST := 8 * 1024 * 1024
const MAX_PNG := 2 * 1024 * 1024
const ExternalContentPath = preload("res://services/external_content_path.gd")

var _root := ""
var _components: Dictionary = {}
var _error := ""

func open_pack(root: String) -> bool:
	_clear()
	if not _safe_root(root):
		return _fail("weekday56 pack must use a canonical absolute external path without symlinks")
	var manifest := root.path_join("manifest.json")
	var manifest_bytes := _read_bounded(manifest, MAX_MANIFEST)
	if manifest_bytes.is_empty():
		return _fail("weekday56 manifest is missing, linked or oversized")
	if _sha_buffer(manifest_bytes) != MANIFEST_SHA:
		return _fail("weekday56 manifest differs from the reviewed exact source export")
	var doc: Variant = JSON.parse_string(manifest_bytes.get_string_from_utf8())
	if not doc is Dictionary or doc.get("schema") != SCHEMA or doc.get("version") != 1:
		return _fail("weekday56 schema differs")
	var components: Variant = doc.get("components")
	if not components is Dictionary or components.size() != 2 or not components.has("command_5") or not components.has("command_6"):
		return _fail("weekday56 requires exactly commands 5 and 6")
	var assets := root.path_join("assets")
	if _has_symlink(assets) or not DirAccess.dir_exists_absolute(assets):
		return _fail("weekday56 assets directory is missing or linked")
	_root = root
	for component in components.values():
		var bg: Dictionary = component["background"]
		for key in ["image", "stp_mask", "padded_image", "padded_stp_mask"]:
			var size := Vector2i(320, 96 if key.begins_with("padded") else 88)
			if not _verify_png(bg[key], size):
				return _fail("weekday56 background PNG bytes/dimensions differ")
		for frame in component["sd"]["frames"]:
			var size := Vector2i(int(frame["bounds"][2]), int(frame["bounds"][3]))
			if not _verify_png(frame["png"], size) or not _verify_png(frame["stp_png"], size):
				return _fail("weekday56 SD PNG bytes/dimensions differ")
	_components = components.duplicate(true)
	return true

func request_error(command: int, month: int = 4, extra: int = 15, scene: int = 0, vacation: int = 0, health: int = 0) -> String:
	if command not in [5, 6]:
		return "weekday56 only commands 5/6 are profiled"
	if month not in [3, 4, 5]:
		return "weekday56 only spring months 3-5 are profiled"
	if extra != 15:
		return "weekday56 only event extra15 is profiled"
	if scene != 0:
		return "weekday56 only scene0/sequence0 is profiled"
	if vacation != 0:
		return "weekday56 vacation content is unavailable"
	if health != 0:
		return "weekday56 only normal health nibble0 is profiled"
	if _components.is_empty():
		return "weekday56 pack is not open"
	return ""

func get_command(command: int, month: int = 4, extra: int = 15, scene: int = 0, vacation: int = 0, health: int = 0) -> Dictionary:
	_error = request_error(command, month, extra, scene, vacation, health)
	if not _error.is_empty():
		return {}
	return _components["command_%d" % command].duplicate(true)

func get_background(command: int, month: int = 4, extra: int = 15, scene: int = 0, vacation: int = 0, health: int = 0) -> Image:
	var component := get_command(command, month, extra, scene, vacation, health)
	if component.is_empty():
		return null
	return _load_image(component["background"]["image"])

func get_sd_frame(command: int, frame_index: int, month: int = 4, extra: int = 15, scene: int = 0, vacation: int = 0, health: int = 0) -> Dictionary:
	var component := get_command(command, month, extra, scene, vacation, health)
	if component.is_empty():
		return {}
	for frame in component["sd"]["frames"]:
		if frame["frame_index"] == frame_index:
			var result: Dictionary = frame.duplicate(true)
			result["image"] = _load_image(frame["png"])
			result["stp_image"] = _load_image(frame["stp_png"])
			if result["image"] == null or result["stp_image"] == null:
				return {}
			return result
	_error = "weekday56 frame is outside the selected original physical frame domain"
	return {}

func error_message() -> String:
	return _error

func _load_image(spec: Dictionary) -> Image:
	var raw := _png_bytes(spec)
	var image := Image.new()
	if raw.is_empty() or image.load_png_from_buffer(raw) != OK:
		_fail("weekday56 image changed after opening the pack")
		return null
	return image

func _verify_png(spec: Dictionary, size: Vector2i) -> bool:
	var raw := _png_bytes(spec)
	if raw.is_empty() or _big_endian(raw, 16) != size.x or _big_endian(raw, 20) != size.y or size.x * size.y > 4 * 1024 * 1024:
		return false
	var image := Image.new()
	return image.load_png_from_buffer(raw) == OK and image.get_size() == size

func _png_bytes(spec: Dictionary) -> PackedByteArray:
	var name: String = spec.get("path", "")
	if name.is_empty() or name.get_file() != name or name.contains("..") or name.contains("/") or name.contains("\\") or not name.ends_with(".png"):
		return PackedByteArray()
	var path := _root.path_join("assets").path_join(name)
	var raw := _read_bounded(path, MAX_PNG)
	if raw.size() != spec["size"] or _sha_buffer(raw) != spec["sha256"]:
		return PackedByteArray()
	if raw.size() < 24 or raw.slice(0, 8) != PackedByteArray([137, 80, 78, 71, 13, 10, 26, 10]):
		return PackedByteArray()
	return raw

func _read_bounded(path: String, maximum: int) -> PackedByteArray:
	if _has_symlink(path):
		return PackedByteArray()
	var file := FileAccess.open(path, FileAccess.READ)
	if file == null or file.get_length() > maximum:
		return PackedByteArray()
	# Hash and parse this one bounded snapshot, even if the path changes later.
	var raw := file.get_buffer(maximum + 1)
	file.close()
	return raw if raw.size() <= maximum else PackedByteArray()

func _sha_buffer(raw: PackedByteArray) -> String:
	var context := HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(raw)
	return context.finish().hex_encode()

func _big_endian(raw: PackedByteArray, offset: int) -> int:
	return (int(raw[offset]) << 24) | (int(raw[offset + 1]) << 16) | (int(raw[offset + 2]) << 8) | int(raw[offset + 3])

func _safe_root(path: String) -> bool:
	return ExternalContentPath.is_external_path(path) and DirAccess.dir_exists_absolute(path)

func _has_symlink(path: String) -> bool:
	var current := "/"
	for part in path.trim_prefix("/").split("/"):
		current = current.path_join(part)
		var parent := DirAccess.open(current.get_base_dir())
		if parent != null and parent.is_link(current.get_file()):
			return true
	return false

func _clear() -> void:
	_root = ""
	_components.clear()
	_error = ""

func _fail(message: String) -> bool:
	_clear()
	_error = message
	return false
