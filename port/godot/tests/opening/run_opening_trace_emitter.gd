extends "res://tests/opening/run_opening_frame_timing_route.gd"
## T-39022 (T-39018 WP-05): the timed opening route (same scripted entry and
## TIMED_ENTRY_PAD phase as run_opening_frame_timing_route.gd) plus one JSONL
## record per completed game iteration in /out/opening-trace.port.jsonl: mode,
## stage, step, substep, frame_count, RNG draw count and the opening fields named
## in config/port/opening-trace-projection.json. Read-only: the emitter copies
## state after frame publication and never changes it. A published frame that
## skipped an iteration is a failure, never interpolated.
## Runner: tools/port/run_opening_trace_differential.sh.
const TRACE_PATH := "/out/opening-trace.port.jsonl"
const OPENING_KEYS := ["prologue_scene_counter", "D_801220E0", "D_801220E1", "D_801220E2"]
var trace_file: FileAccess
var last_trace_frame := -1

func _initialize() -> void:
	trace_file = FileAccess.open(TRACE_PATH, FileAccess.WRITE)
	check(trace_file != null, "cannot write " + TRACE_PATH)
	super._initialize()

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	if trace_file != null and state != null and state.game_mode == 0x15 and int(state.frame_count) != last_trace_frame:
		if last_trace_frame >= 0:
			check(int(state.frame_count) == last_trace_frame + 1, "trace skipped iterations %d..%d" % [last_trace_frame + 1, int(state.frame_count) - 1])
		last_trace_frame = int(state.frame_count)
		var opening := {}
		for key: String in OPENING_KEYS:
			if state.opening.has(key): opening[key] = state.opening[key]
		var layer = state.opening.get("bg_layer_0")
		if layer is Dictionary and layer.has("enabled"): opening["bg_layer_0"] = {"enabled": layer.enabled}
		var actor = state.opening.get("actor96")
		if actor is Dictionary and actor.has("x"): opening["actor96"] = {"x": actor.x}
		var rng = app.consumer.opening.session.rng
		trace_file.store_line(JSON.stringify({"frame_count": int(state.frame_count), "game_mode": int(state.game_mode),
			"mode_stage": int(state.mode_stage), "mode_step": int(state.mode_step), "substep": int(state.substep),
			"opening": opening, "rng": {"draw_count": int(rng.draw_count)}}, "", true))
	super._frame_published(tick, app)

func _finish_rei(app: App) -> void:
	if trace_file != null: trace_file.close()
	super._finish_rei(app)

func _finish_boundary(app: App) -> void:
	if trace_file != null: trace_file.close()
	super._finish_boundary(app)
