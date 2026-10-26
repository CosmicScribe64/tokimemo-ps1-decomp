extends SceneTree
## Genuine April4 -> ordinary week -> Sunday -> next command, real source actors and live HolidayHud.
## No synthetic UI seam: playable_boot.gd preflights and owns HolidayHud, HolidaySceneAdapter, and HolidayController.

const Boot = preload("res://app/playable_boot.gd")
const Harness = preload("res://tests/app/run_command7_app_tests.gd")

var failures: Array[String] = []
var checks: int = 0
var reports: Array[Dictionary] = []

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func _initialize() -> void:
	var fixture: Dictionary = Boot._integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_HOLIDAY_FIXTURE"))))
	var expected: Dictionary = fixture["checkpoints"][-1]
	var harness: Harness = Harness.new()
	var reference: Array = []
	for rendered: int in [0, 1, 3]:
		var app: Boot = harness.make_app()
		check(app.holiday_controller != null and app.holiday_scene != null and app.holiday_hud != null,
			"joined playable_boot preflights HolidayController, HolidaySceneAdapter, and HolidayHud without a test seam")
		var menu_tick: int = -1
		var points: Array[Dictionary] = []
		for tick: int in range(901):
			Harness.device_recipe(app, 0, tick)
			if app.state.game_mode == 0x46 and app.state.mode_stage == 1 and menu_tick < 0:
				menu_tick = tick
			if menu_tick >= 0 and tick in [menu_tick + 32, menu_tick + 38]:
				var event: InputEventKey = InputEventKey.new()
				event.physical_keycode = KEY_ENTER
				event.pressed = tick == menu_tick + 32
				app.pad._input(event)
			app.session.clock.rendered_frames += rendered
			var before: Dictionary = app.state.projection()
			app._physics_process(1.0 / 60.0)
			if app.failure.length() > 0:
				check(false, app.failure)
				break
			if rendered == 0 and app.state.rule_state["day_event_index"] >= 9:
				points.append({"before": before, "after": app.state.projection(), "rng": app.session.rng.checkpoint(),
					"result": app.trace[-1].duplicate(true)})
			if app.state.game_mode == 0x34 and app.state.mode_stage == 1 and app.state.rule_state["day_event_index"] == 10:
				break
		check(app.state.game_mode == 0x34 and app.state.mode_stage == 1 and app.state.rule_state["day_event_index"] == 10, "continuous app reaches Sunday next command")
		var differences: Array[String] = []
		for field: String in ["year", "month", "day", "weekday", "day_count", "calendar", "params", "player_flags", "player_flags_upper", "player_birthday", "girls", "progress_state", "day_events", "day_event_index", "day_event_done_bits", "day_event_count_today", "daily_check_done", "last_command", "menu_cursor", "menu_mode", "menu_disabled", "icon_enabled", "appointment_words", "background_cache", "background_show", "scene_work", "holiday"]:
			if app.state.rule_state[field] != expected["rule_state"][field]:
				differences.append(field)
		check(differences.is_empty(), "original next-command semantic fields: " + str(differences))
		check(app.session.rng.state == expected["rng_state"], "original RNG endpoint")
		var hashes: Array = []
		for row: Dictionary in app.trace:
			hashes.append([row["state_hash"], row["event_hash"]])
		if rendered == 0:
			reference = hashes
			reports.append({"points": points, "trace": app.trace.duplicate(true), "final_state": app.state.projection(),
				"rng": app.session.rng.checkpoint(), "differences": differences, "menu_tick": menu_tick})
		else:
			check(reference == hashes, "continuous render-schedule hashes are identical")
		harness.dispose(app)
	check(harness.failures.is_empty(), "all actual app preflights/reset checks pass")
	harness.free()
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_HOLIDAY_JOIN_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify({"checks": checks, "failures": failures, "runs": reports, "steps": reference.size(),
		"scope": "continuous source rules/services/native actor and real HolidayHud join without synthetic UI seam"}, "\t"))
	output.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "steps": reference.size()}))
	quit(0 if failures.is_empty() else 1)
