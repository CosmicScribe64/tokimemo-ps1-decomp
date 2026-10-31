extends SceneTree
## T-39300 development tool: rebuilds the game memory model from a route state file
## (<route-out>/states/<key>.json: state projection and rule_state, written by the route
## observers) and dumps it like tests/support/game_memory_dump.gd, so memory-model changes
## can be compared with the emulator without a new seven-minute route run.
## Environment: M1_STATE_JSON (input), M1_OUT (new output dir), M1_KEY (e.g. 34/1/0/0),
## M1_EXECUTABLE (optional installed executable). Observation only; outputs are ignored data.
const RouteState = preload("res://rules/new_game_route_state.gd")
const GameMemoryDump = preload("res://tests/support/game_memory_dump.gd")

func _initialize() -> void:
	var path: String = OS.get_environment("M1_STATE_JSON")
	var out: String = OS.get_environment("M1_OUT")
	var key: String = OS.get_environment("M1_KEY") if not OS.get_environment("M1_KEY").is_empty() else "34/1/0/0"
	var data: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not data is Dictionary or out.is_empty():
		push_error("M1_STATE_JSON must name a route state file and M1_OUT an output directory")
		quit(2)
		return
	var state := RouteState.new()
	var projection: Dictionary = data["state"]
	state.restore(projection)
	state.opening = projection.get("opening", {}).duplicate(true)
	state.rule_state = data["rule_state"].duplicate(true)
	var meta: Dictionary = GameMemoryDump.dump(state, out, key, int(state.frame_count), OS.get_environment("M1_EXECUTABLE"))
	print("DUMP ", JSON.stringify({"key": key, "memory_error": meta.memory_error, "executable_loaded": meta.executable_loaded}))
	quit(0 if meta.memory_error.is_empty() and meta.write_error.is_empty() else 1)
