extends RefCounted
## T-29024. Authenticated external combat-unit tables, not full route content.

const MANIFEST_SHA256: String = "90e9538c47d6d94389c241821d369b041ca66b786e4904637961a00f5fa387cf"
const OVERLAY_SHA256: String = "1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80"
var tables: Dictionary = {}
var error: String = ""

static func _path_valid(path: String) -> bool:
	if not path.begins_with("/") or path.begins_with("//") or path.contains("\\") or path.contains(":") or path.simplify_path() != path: return false
	var project: String = ProjectSettings.globalize_path("res://").trim_suffix("/")
	if path == project or path.begins_with(project + "/"): return false
	var directory: DirAccess = DirAccess.open("/")
	if directory == null: return false
	var current: String = ""
	for part: String in path.split("/", false):
		current += "/" + part
		if directory.is_link(current): return false
	return FileAccess.file_exists(path)

func open_manifest(path: String) -> String:
	tables.clear()
	error = "TACO combat table manifest is unavailable or unauthenticated"
	if not _path_valid(path): return error
	var file: FileAccess = FileAccess.open(path, FileAccess.READ)
	if file == null: return error
	var length: int = file.get_length()
	if length <= 0 or length > 65536: return error
	var raw: PackedByteArray = file.get_buffer(length)
	if raw.size() != length or file.get_length() != length or not _path_valid(path): return error
	var hash: HashingContext = HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(raw)
	if hash.finish().hex_encode() != MANIFEST_SHA256: return error
	var parsed: Variant = JSON.parse_string(raw.get_string_from_utf8())
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.taco-combat-tables.v1" or parsed.get("source_overlay_sha256") != OVERLAY_SHA256: return error
	if not parsed.get("tables") is Dictionary: return error
	var shapes: Dictionary = {"score_by_class":17, "damage_by_kind":9, "se_cooldown_range":10}
	for key: String in shapes:
		if not parsed.tables.get(key) is Array or parsed.tables[key].size() != shapes[key]: return error
		for i: int in parsed.tables[key].size():
			var number: Variant = parsed.tables[key][i]
			if typeof(number) not in [TYPE_INT, TYPE_FLOAT] or not is_finite(float(number)) or float(int(number)) != float(number): return error
			var low: int = -2147483648 if key == "score_by_class" else 0
			var high: int = 2147483647 if key == "score_by_class" else 255
			if number < low or number > high: return error
			parsed.tables[key][i] = int(number)
	tables = parsed.tables.duplicate(true)
	error = ""
	return ""
