extends RefCounted
## T-39300: development dump of the port's game memory model at a route key, for
## tools/port/compare_game_memory.py. Observation only: the state is read, never changed.
## The memory is filled from the state through core/rule_state_bridge.gd; with an installed
## executable (found under <out>/content-install/packs/*/audio/first-arrival/executable.bin,
## read at runtime, never copied) its text image is loaded first, as on the original.
## The dump holds game memory of the user's installed content: it stays under ignored
## .asset-cache/ and is never committed.

const Memory = preload("res://core/game_memory.gd")
const Bridge = preload("res://core/rule_state_bridge.gd")

## Writes <out_dir>/game-memory/<key>.ram.bin and <key>.json. Returns the metadata.
static func dump(state: Object, out_dir: String, key: String, frame_count: int) -> Dictionary:
	var mem := Memory.new()
	var exe_path: String = _find_executable(out_dir)
	var exe_error: String = ""
	if not exe_path.is_empty():
		exe_error = mem.load_executable_file(exe_path)
	Bridge.push(state, mem)
	var directory: String = out_dir.path_join("game-memory")
	DirAccess.make_dir_recursive_absolute(directory)
	var stem: String = key.replace("/", "-")
	var write_error: String = mem.dump_to_file(directory.path_join(stem + ".ram.bin"))
	var meta: Dictionary = {
		"key": key, "frame_count": frame_count, "ram_bytes": Memory.RAM_SIZE,
		"origin": "port memory model: rule_state/core fields pushed through core/rule_state_bridge.gd",
		"executable_loaded": not exe_path.is_empty() and exe_error.is_empty(),
		"executable_error": exe_error, "memory_error": mem.error, "write_error": write_error,
		"mapped_keys": Bridge.MAPPED_KEYS, "port_only_keys": Bridge.PORT_ONLY_KEYS,
	}
	var file := FileAccess.open(directory.path_join(stem + ".json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(meta, "\t", false))
		file.close()
	return meta

static func _find_executable(out_dir: String) -> String:
	var packs: String = out_dir.path_join("content-install/packs")
	var dir: DirAccess = DirAccess.open(packs)
	if dir == null:
		return ""
	var names: Array[String] = []
	for entry: String in dir.get_directories():
		names.append(entry)
	names.sort()
	for entry: String in names:
		var candidate: String = packs.path_join(entry).path_join("audio/first-arrival/executable.bin")
		if FileAccess.file_exists(candidate):
			return candidate
	return ""
