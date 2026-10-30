extends RefCounted
## T-39004: optional exact-source English image replacements. Null means fallback.

const ExternalPath = preload("res://services/external_content_path.gd")
const MAX_MANIFEST := 4 * 1024 * 1024
const MAX_IMAGE := 16 * 1024 * 1024
const MAX_ASSETS := 4096
const MAX_DECODED_BYTES := 256 * 1024 * 1024
var error: String = ""
var language: String = ""
var _images: Dictionary = {}

func clear() -> void:
	_images.clear()
	language = ""
	error = ""

func _fail(message: String) -> bool:
	clear()
	error = message
	return false

static func _hash(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character: String in value:
		if character not in "0123456789abcdef":
			return false
	return true

static func _relative(value: Variant) -> bool:
	if not value is String or value.is_empty() or value.contains("\\") or value.contains(":") or value.begins_with("/"):
		return false
	for part: String in value.split("/"):
		if part in ["", ".", ".."]:
			return false
	return true

static func _integer(value: Variant) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and float(value) == floor(float(value))

static func _key(container_sha: String, frame: int, palette: String, png_sha: String) -> String:
	return container_sha + "|" + str(frame) + "|" + palette + "|" + png_sha

func open_pack(manifest_path: String, expected_sha256: String, allow_drafts: bool = false) -> bool:
	clear()
	if not _hash(expected_sha256) or not ExternalPath.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > MAX_MANIFEST or FileAccess.get_sha256(manifest_path) != expected_sha256:
		return _fail("translated image manifest path/hash is invalid")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.translated-image-pack.v1" or parsed.get("language") != "en" or not parsed.get("assets") is Array or not parsed.get("development_drafts") is bool or not _hash(parsed.get("snapshot_sha256")):
		return _fail("unsupported translated image pack")
	if parsed["development_drafts"] and not allow_drafts:
		return _fail("draft translation pack needs explicit development opt-in")
	if parsed["assets"].size() > MAX_ASSETS:
		return _fail("translation pack exceeds asset limit")
	var pending: Dictionary = {}
	var decoded_bytes := 0
	for row: Variant in parsed["assets"]:
		if not row is Dictionary or not _hash(row.get("source_container_sha256")) or not _hash(row.get("source_png_sha256")) or not _hash(row.get("output_sha256")) or not _integer(row.get("frame")) or int(row["frame"]) < 0 or int(row["frame"]) > 65535:
			return _fail("invalid translation source identity")
		if not row.get("palette") is String or row["palette"].is_empty() or row["palette"].contains("|") or not _relative(row.get("output")):
			return _fail("invalid translation mapping or output path")
		if not allow_drafts and row.get("style") != "approve":
			return _fail("replacement lacks exact current style approval")
		var size: Variant = row.get("dimensions")
		if not size is Array or size.size() != 2 or not _integer(size[0]) or not _integer(size[1]) or int(size[0]) < 1 or int(size[1]) < 1 or int(size[0]) > 4096 or int(size[1]) > 4096:
			return _fail("invalid replacement dimensions")
		decoded_bytes += int(size[0]) * int(size[1]) * 4
		if decoded_bytes > MAX_DECODED_BYTES:
			return _fail("translation pack exceeds decoded memory limit")
		var path := manifest_path.get_base_dir().path_join(row["output"])
		if not ExternalPath.is_external_file(path) or FileAccess.get_size(path) > MAX_IMAGE or FileAccess.get_sha256(path) != row["output_sha256"]:
			return _fail("translation PNG path/hash is invalid")
		var image := Image.new()
		if image.load_png_from_buffer(FileAccess.get_file_as_bytes(path)) != OK or image.get_width() != int(size[0]) or image.get_height() != int(size[1]):
			return _fail("translation PNG dimensions/format differ")
		var key := _key(row["source_container_sha256"], int(row["frame"]), row["palette"], row["source_png_sha256"])
		if pending.has(key):
			return _fail("duplicate translation source mapping")
		pending[key] = image
	_images = pending
	language = "en"
	return true

func replacement(container_sha: String, frame: int, palette: String, source_png_sha: String) -> Image:
	var value: Image = _images.get(_key(container_sha, frame, palette, source_png_sha))
	return value.duplicate() if value != null else null

func asset_count() -> int:
	return _images.size()
