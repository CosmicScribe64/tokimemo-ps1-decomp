extends RefCounted
## Read history follows source content identities, independently of native saves.
const SCHEMA := "tokimemo.read-history.v1"
var path: String
var lines: Dictionary = {}
var error := ""
func _init(storage_path: String = "user://read-history.json") -> void:
	path = storage_path
func load_history() -> bool:
	lines = {}; error = ""
	if not FileAccess.file_exists(path): return true
	if FileAccess.get_size(path) > 1048576: error = "Read history exceeds its size limit."; return false
	var value: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not value is Dictionary or value.get("schema") != SCHEMA or not value.get("lines") is Dictionary or value.lines.size() > 8192:
		error = "Read history is invalid."; return false
	for key: Variant in value.lines:
		if not key is String or key.is_empty() or key.length() > 160 or value.lines[key] != true:
			error = "Read history contains an invalid identity."; return false
	lines = value.lines.duplicate(); return true
func mark_read(identity: String) -> bool:
	if lines.has(identity): return true
	if identity.is_empty() or identity.length() > 160 or lines.size() >= 8192: return false
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
func contains_all(identities: Array[String]) -> bool:
	if identities.is_empty(): return false
	for identity: String in identities:
		if not lines.has(identity): return false
	return true
