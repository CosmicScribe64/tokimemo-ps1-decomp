extends RefCounted
## Read history follows source content identities, independently of native saves.
## T-30002 WP9: identities for T-39016 text-layer records come from
## text_layer_identity(), so read/skip state follows the source record and its
## source hash, never a save slot or a language.
const SCHEMA := "tokimemo.read-history.v1"
const MAX_IDENTITY := 160
var path: String
var lines: Dictionary = {}
var error := ""
func _init(storage_path: String = "user://read-history.json") -> void:
	path = storage_path
## Stable identity of one text-layer record (for example
## talk:<path>:<section>:g0001:l0002) bound to its source sha256. Long ids are
## hashed so the identity always fits MAX_IDENTITY.
static func text_layer_identity(record_id: String, source_sha256: String) -> String:
	if record_id.is_empty() or source_sha256.is_empty(): return ""
	var identity := "tl:%s@%s" % [record_id, source_sha256.left(16)]
	if identity.length() <= MAX_IDENTITY: return identity
	return "tlh:" + (record_id + "\n" + source_sha256).sha256_text()
func load_history() -> bool:
	lines = {}; error = ""
	if not FileAccess.file_exists(path): return true
	if FileAccess.get_size(path) > 1048576: error = "Read history exceeds its size limit."; return false
	var value: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not value is Dictionary or value.get("schema") != SCHEMA or not value.get("lines") is Dictionary or value.lines.size() > 8192:
		error = "Read history is invalid."; return false
	for key: Variant in value.lines:
		if not key is String or key.is_empty() or key.length() > MAX_IDENTITY or value.lines[key] != true:
			error = "Read history contains an invalid identity."; return false
	lines = value.lines.duplicate(); return true
func mark_read(identity: String) -> bool:
	if lines.has(identity): return true
	if identity.is_empty() or identity.length() > MAX_IDENTITY or lines.size() >= 8192: return false
	var updated := lines.duplicate(); updated[identity] = true
	var destination := ProjectSettings.globalize_path(path)
	if DirAccess.make_dir_recursive_absolute(destination.get_base_dir()) != OK: return false
	var file := FileAccess.open(path + ".tmp", FileAccess.WRITE)
	if file == null: error = "Cannot write read history."; return false
	file.store_string(JSON.stringify({"schema": SCHEMA, "lines": updated})); file.flush()
	var status := file.get_error(); file.close()
	if status != OK or DirAccess.rename_absolute(destination + ".tmp", destination) != OK:
		error = "Cannot save read history."; return false
	lines = updated; error = ""; return true
func is_read(identity: String) -> bool:
	return not identity.is_empty() and lines.has(identity)
## skip_mode is accessibility.skip: 'all' skips anything, 'read' only read lines.
func may_skip(identity: String, skip_mode: String) -> bool:
	return skip_mode == "all" or is_read(identity)
func contains_all(identities: Array[String]) -> bool:
	if identities.is_empty(): return false
	for identity: String in identities:
		if not lines.has(identity): return false
	return true
