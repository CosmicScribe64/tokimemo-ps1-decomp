extends SceneTree
## External original fixture test; no original payload is embedded in this file.

const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")

var failures: Array[String] = []
var reports: Array[Dictionary] = []

func check(ok: bool, message: String) -> void:
	if not ok:
		failures.append(message)

func _initialize() -> void:
	var reference: Array = []
	var native: Dictionary = Boot._integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_NATIVE_WEEK_EVIDENCE"))))
	for rendered: int in [0, 1, 3]:
		var app: Boot = Boot.new()
		app.canvas = Canvas.new()
		app.pad = Pad.new()
		app.status_label = Label.new()
		app.detail_label = Label.new()
		app.continue_week = true
		var error: String = app._load_inputs()
		if error.is_empty():error = app._reset()
		check(error.is_empty(), error)
		if not error.is_empty():break
		for tick: int in range(601):
			if tick in [17, 23]:
				var key: InputEventKey = InputEventKey.new()
				key.physical_keycode = KEY_ENTER
				key.pressed = tick == 17
				app.pad._input(key)
			app.session.clock.rendered_frames += rendered
			app._physics_process(1.0 / 60.0)
			if app.stopped:break
		check(app.stopped and app.failure.is_empty() and not app.supported_stop.is_empty(), "app must stop only at declared unsupported holiday")
		check(app.state.game_mode == 0x31 and app.state.rule_state["day_event_index"] == 8 and app.state.rule_state["day"] == 8, "holiday refusal retains actual April8/index8 boundary")
		check(app.supported_stop.get("before_hash") == app.supported_stop.get("after_hash") and app.supported_stop.get("committed") == false, "unsupported holiday leaves state/input/RNG/clock hash unchanged")
		check(app.trace.size() == app.session.clock.logic_iterations, "failed holiday attempt appends no committed trace row")
		check(app.checkpoints.size() == 15, "all fifteen source checkpoint boundaries are recorded")
		var cached_count: int = 0
		var hud_count: int = 0
		for row: Dictionary in app.trace:
			for intent: Dictionary in row["presentation"]:
				if intent.get("kind") == "school_day_cached_background":cached_count += 1
				if intent.get("kind") == "week_end_hud_init":hud_count += 1
		check(cached_count == 3 and hud_count == 1, "actual route consumes three native cache hits and one week-end HUD initialization")
		var hashes: Array = []
		for row: Dictionary in app.trace:
			hashes.append([row["state_hash"],row["event_hash"]])
		if rendered == 0:
			reference = hashes
			compare_native(app, native)
		else:
			check(reference == hashes, "render count changes no joined ordinary-week state/event hash")
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_WEEK_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify({"failures":failures,"native_checkpoints":reports,"render_schedules":[0,1,3]},"\t"))
	output.close()
	for failure: String in failures:push_error(failure)
	print(JSON.stringify({"failures":failures,"native_checkpoints":reports.size(),"committed_steps":reference.size(),"render_schedules":[0,1,3]}))
	quit(0 if failures.is_empty() else 1)

func compare_native(app: Boot, native: Dictionary) -> void:
	var original: Array = native["routes"][0]["hits"]
	for checkpoint: Dictionary in app.checkpoints:
		var actual: Dictionary = checkpoint["state"]["rule_state"]
		var phase: String = checkpoint["phase"]
		var expected: Dictionary = {}
		var expected_core: Dictionary = {}
		var expected_rng: int = 0
		if phase == "activity_entry" and actual["day_event_index"] == 0:
			var initial: Dictionary = app.boundary["expected"]["0"]
			expected = initial["rule_state"]
			expected_rng = initial["rng_state"]
			var fields: Array[String] = ["game_mode","mode_stage","mode_step","substep"]
			for index: int in range(fields.size()):expected_core[fields[index]]=initial["core_mode"][index]
			fields=["game_mode_timer","mode_stage_timer","mode_step_timer"]
			for index: int in range(fields.size()):expected_core[fields[index]]=initial["native_mode_timers"][index]
			check(checkpoint["state"]["frame_count"] == initial["native_frame_count"], "initial native command-entry absolute frame count remains exact")
		else:
			for hit: Dictionary in original:
				if hit["rule_state"]["day_event_index"] != actual["day_event_index"]:continue
				var mode: Dictionary = hit["core"]
				var matching: bool = (phase == "activity_entry" and hit["label"] == "week_day_init" and mode["mode_step"] == 0 and mode["substep"] == 0) or (phase == "cached_background" and hit["label"] == "week_day_init" and mode["mode_step"] == 0 and mode["substep"] == 2) or (phase in ["activity_return","week_end_return"] and hit["label"] == "day_event_dispatch") or (phase.begins_with("week_end_stage_") and hit["label"] == "schedule_week_end_mode_handler" and mode["mode_stage"] == checkpoint["state"]["mode_stage"])
				if matching:
					expected=hit["rule_state"]
					expected_core=mode
					expected_rng=hit["rng_state"]
					break
		check(not expected.is_empty(), "original checkpoint missing: "+phase)
		if expected.is_empty():continue
		var mismatches: Array[String] = []
		for field: String in ["year","month","day","weekday","day_count","calendar","params","player_flags","player_flags_upper","girls","progress_state","day_events","day_event_index","day_event_done_bits","day_event_count_today","daily_check_done","last_command","appointment_words"]:
			if actual[field] != expected[field]:mismatches.append(field)
		if checkpoint["rng"]["state"] != expected_rng:mismatches.append("rng")
		if not expected_core.is_empty():
			for key: String in ["game_mode","mode_stage","mode_step","substep","game_mode_timer","mode_stage_timer","mode_step_timer"]:
				if checkpoint["state"][key] != expected_core[key]:mismatches.append(key)
		check(mismatches.is_empty(), "native joined "+phase+" index%d differs: %s" % [actual["day_event_index"],str(mismatches)])
		var profile_differences: Array[int] = []
		for offset: int in range(actual["progress_state"].size()):
			if actual["progress_state"][offset] != expected["progress_state"][offset]:profile_differences.append(offset)
		reports.append({"phase":phase,"queue_index":actual["day_event_index"],"day":actual["day"],"mismatches":mismatches,
			"profile_differing_offsets":profile_differences,"absolute_elapsed_frame_count_compared":false,"native_rng":expected_rng,"actual_rng":checkpoint["rng"]["state"]})
