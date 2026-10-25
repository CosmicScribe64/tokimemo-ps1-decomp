extends SceneTree
## T-29009: full composed ordinary rest weekdays with external native witnesses.

const Boot = preload("res://app/playable_boot.gd")
const Harness = preload("res://tests/app/run_command7_app_tests.gd")

var failures: Array[String] = []
var checks: int = 0
var commands: Dictionary = {}
var native: Dictionary
const FIELDS: Array[String] = ["year", "month", "day", "weekday", "day_count", "calendar",
	"params", "player_flags", "player_flags_upper", "player_birthday", "girls", "progress_state",
	"day_events", "day_event_index", "day_event_done_bits", "day_event_count_today", "daily_check_done",
	"last_command", "appointment_words", "icon_enabled", "girl_last_date_spot_timer", "vacation"]

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func _initialize() -> void:
	native = Boot._integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_NATIVE_WEEK_EVIDENCE"))))
	var harness: Harness = Harness.new()
	for command: int in [0, 7]:
		var reference: Array = []
		for rendered: int in [0, 1, 3]:
			var app: Boot = harness.make_app()
			app.continue_week = true
			var full_trace: Array[Dictionary] = []
			for tick: int in range(601):
				Harness.device_recipe(app, command, tick)
				var before: Dictionary = app.state.projection()
				var before_rng: Dictionary = app.session.rng.checkpoint()
				app.session.clock.rendered_frames += rendered
				app._physics_process(1.0 / 60.0)
				if app.trace.size() == tick + 1 and rendered == 0:
					full_trace.append({"before": before, "before_rng": before_rng,
						"after": app.state.projection(), "after_rng": app.session.rng.checkpoint(),
						"result": app.trace[-1].duplicate(true)})
				if app.stopped:
					break
			check(app.stopped and app.failure.is_empty() and not app.supported_stop.is_empty(), "command%d stops at the captured unsupported holiday" % command)
			check(app.state.game_mode == 0x31 and app.state.rule_state["day_event_index"] == 8 and app.state.rule_state["day"] == 8, "command%d retains April8/index8 before holiday" % command)
			check(app.supported_stop.get("before_hash") == app.supported_stop.get("after_hash") and app.supported_stop.get("committed") == false, "holiday attempt preserves complete session hash")
			check(app.trace.size() == app.session.clock.logic_iterations, "no failed trace row is committed")
			check(app.checkpoints.size() == 15, "command%d has all fifteen source checkpoints" % command)
			var cached: int = 0
			var hud: int = 0
			var draws: Dictionary = {}
			var hashes: Array = []
			for row: Dictionary in app.trace:
				hashes.append([row["state_hash"], row["event_hash"]])
				for intent: Dictionary in row["presentation"]:
					if intent.get("kind") == "school_day_cached_background":
						cached += 1
					if intent.get("kind") == "week_end_hud_init":
						hud += 1
					if intent.get("kind") == "native_actor_draw":
						draws[str(row["iteration"])] = intent["id"]
			check(cached == 3 and hud == 1, "three cached backgrounds and one source week-end HUD init")
			check(not draws.is_empty(), "native actor sequence is actually consumed")
			if rendered == 0:
				reference = hashes
				commands[str(command)] = {"trace": app.trace.duplicate(true), "full_trace": full_trace,
					"checkpoints": app.checkpoints.duplicate(true), "native_checkpoints": compare_native(app, command),
					"final_state": app.state.projection(), "rng": app.session.rng.checkpoint(),
					"supported_stop": app.supported_stop.duplicate(true)}
			else:
				check(reference == hashes, "render schedule changes no ordinary-week state/event hash")
			harness.dispose(app)
	check(harness.failures.is_empty(), "all external preflights and resets succeeded")
	harness.free()
	var proof: Dictionary = {"failures": failures, "checks": checks, "commands": commands,
		"render_schedules": [0, 1, 3], "source": OS.get_environment("TOKIMEMO_NATIVE_WEEK_EVIDENCE")}
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_REST_WEEK_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify(proof, "\t"))
	output.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"failures": failures, "checks": checks, "command0_steps": commands.get("0", {}).get("trace", []).size(),
		"command7_steps": commands.get("7", {}).get("trace", []).size(), "native_checkpoints_per_command": 15}))
	quit(0 if failures.is_empty() else 1)

func compare_native(app: Boot, command: int) -> Array[Dictionary]:
	var hits: Array = []
	for route: Dictionary in native["routes"]:
		if route["command"] == command:
			hits = route["hits"]
	var reports: Array[Dictionary] = []
	for checkpoint: Dictionary in app.checkpoints:
		var actual: Dictionary = checkpoint["state"]["rule_state"]
		var phase: String = checkpoint["phase"]
		var expected: Dictionary = {}
		var expected_core: Dictionary = {}
		var expected_rng: int
		if phase == "activity_entry" and actual["day_event_index"] == 0:
			var initial: Dictionary = app.boundary["expected"][str(command)]
			expected = initial["rule_state"]
			expected_rng = initial["rng_state"]
			for index: int in range(4):
				expected_core[["game_mode", "mode_stage", "mode_step", "substep"][index]] = initial["core_mode"][index]
			for index: int in range(3):
				expected_core[["game_mode_timer", "mode_stage_timer", "mode_step_timer"][index]] = initial["native_mode_timers"][index]
			check(checkpoint["state"]["frame_count"] == initial["native_frame_count"], "original command%d entry frame" % command)
		else:
			for hit: Dictionary in hits:
				if hit["rule_state"]["day_event_index"] != actual["day_event_index"]:
					continue
				var mode: Dictionary = hit["core"]
				var matching: bool = (phase == "activity_entry" and hit["label"] == "week_day_init" and mode["mode_step"] == 0 and mode["substep"] == 0) or (phase == "cached_background" and hit["label"] == "week_day_init" and mode["mode_step"] == 0 and mode["substep"] == 2) or (phase in ["activity_return", "week_end_return"] and hit["label"] == "day_event_dispatch") or (phase.begins_with("week_end_stage_") and hit["label"] == "schedule_week_end_mode_handler" and mode["mode_stage"] == checkpoint["state"]["mode_stage"])
				if matching:
					expected = hit["rule_state"]
					expected_core = mode
					expected_rng = hit["rng_state"]
					break
		check(not expected.is_empty(), "native command%d checkpoint exists: %s" % [command, phase])
		if expected.is_empty():
			continue
		var differences: Array[String] = []
		for field: String in FIELDS:
			if actual[field] != expected[field]:
				differences.append(field)
		for field: String in ["game_mode", "mode_stage", "mode_step", "substep", "game_mode_timer", "mode_stage_timer", "mode_step_timer"]:
			if checkpoint["state"][field] != expected_core[field]:
				differences.append(field)
		if checkpoint["rng"]["state"] != expected_rng:
			differences.append("rng")
		check(differences.is_empty(), "command%d native %s index%d: %s" % [command, phase, actual["day_event_index"], str(differences)])
		reports.append({"phase": phase, "queue_index": actual["day_event_index"], "differences": differences,
			"later_absolute_elapsed_frames_compared": false})
	return reports
