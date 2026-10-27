extends SceneTree
## T-29026. Synthetic branch coverage using genuine external Sunday state/media.
## Changes to result1 and queue positions are test-only variations, not a played
## route, a fresh native witness, or audiovisual acceptance.

const Boot = preload("res://app/playable_boot.gd")
const Harness = preload("res://tests/app/run_command7_app_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Holiday = preload("res://rules/holiday.gd")

var checks: int = 0
var failures: Array[String] = []
var branches: Array[Dictionary] = []

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func _initialize() -> void:
	var fixture: Variant = Boot._integers(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_HOLIDAY_FIXTURE"))))
	if not fixture is Dictionary or not fixture.get("checkpoints") is Array:
		check(false, "genuine native Sunday fixture is required")
		_finish()
		return
	var activity: Dictionary = {}
	var returned: Dictionary = {}
	for point: Dictionary in fixture["checkpoints"]:
		var core: Dictionary = point["core"]
		if activity.is_empty() and [core["game_mode"], core["mode_stage"], core["mode_step"]] == [0x46, 5, 2]:
			activity = point
		if [core["game_mode"], core["mode_stage"], core["mode_step"]] == [0x34, 1, 0]:
			returned = point
	check(not activity.is_empty() and not returned.is_empty(), "native fixture has genuine activity and returned-command states")
	if activity.is_empty() or returned.is_empty():
		_finish()
		return
	var harness := Harness.new()
	var app: Boot = harness.make_app()
	check(harness.failures.is_empty() and app.holiday_scene != null and app.holiday_hud != null,
		"actual Boot preflights native holiday scene/HUD from external originals")
	if not harness.failures.is_empty() or app.holiday_scene == null or app.holiday_hud == null:
		harness.dispose(app)
		harness.free()
		_finish()
		return
	_test_activity(app, activity, 9)
	_test_activity(app, activity, 15)
	_test_returned(app, returned)
	harness.dispose(app)
	harness.free()
	_finish()

func _copy_native(point: Dictionary) -> State:
	var state := State.new()
	for key: String in point["core"]:
		state.set(key, point["core"][key])
	state.rule_state = point["rule_state"].duplicate(true)
	return state

func _synthetic_queue(state: State, sunday_index: int, returned: bool) -> void:
	var data: Dictionary = state.rule_state
	var old_sunday: int = data["day_event_index"] - 1 if returned else data["day_event_index"]
	var sunday: Array = data["day_events"][old_sunday].duplicate()
	var end: Array = data["day_events"][old_sunday + 1].duplicate()
	sunday[2] = 1 # Synthetic branch variation; no native result1 route is claimed.
	data["day_events"][sunday_index] = sunday
	data["day_events"][sunday_index + 1] = end
	data["day_event_index"] = sunday_index + (1 if returned else 0)
	if sunday_index != old_sunday:
		data["allow_multi_week"] = true # Test-only admission of the moved queue.

func _row(state: State, presentation: Array) -> Dictionary:
	return {"mode": state.game_mode, "stage": state.mode_stage, "step": state.mode_step,
		"substep": state.substep, "frame_count": state.frame_count,
		"params": state.rule_state["params"].duplicate(), "presentation": presentation.duplicate(true)}

func _screen_init(screen: String) -> Dictionary:
	return {"kind": "holiday_screen_init", "screen": screen,
		"prompt": "holiday_open_init/default" if screen == "holiday" else "settings_command_screen_build/default",
		"icon_count": 14}

func _test_activity(app: Boot, source: Dictionary, sunday_index: int) -> void:
	var state: State = _copy_native(source)
	_synthetic_queue(state, sunday_index, false)
	var label: String = "synthetic result1 Sunday index%d" % sunday_index
	var source_steps: Array = app.holiday_scene.resources["sequences"]["1"]
	var source_zero: Array = app.holiday_scene.resources["sequences"]["0"]
	var last_key: String = str(source_steps[-1]["frame_index"])
	var unique_frame: Dictionary = app.holiday_scene.resources["frames"][last_key]
	check(source_steps[-1]["frame_index"] != source_zero[-1]["frame_index"], label + " has its distinct original result endpoint")
	var draws_to_terminal: int = 0
	for step: Dictionary in source_steps:
		draws_to_terminal += maxi(1, step["duration"])
	var history: Array[Dictionary] = []
	# Minimal synthetic committed history: menu initialization precedes the
	# activity. Subsequent rows follow actual adapter-native-draw progression.
	var init: Dictionary = _row(state, [_screen_init("holiday")])
	init["stage"] = 1
	init["step"] = 0
	init["substep"] = 0
	init["frame_count"] -= 1
	history.append(init)
	var last_staged: Dictionary = {}
	var terminal_count: int = 0
	for iteration: int in range(draws_to_terminal):
		var result := Session.FrameResult.new()
		if iteration == 0:
			result.presentation.append({"kind": "holiday_actors", "actors": [Holiday.actor(state)], "sd_set_id": 1})
		var staged: Dictionary = app.holiday_scene.stage(state, iteration, result)
		check(staged["error"].is_empty(), label + " stages actual source actor tick%d" % iteration)
		if not staged["error"].is_empty():
			return
		for service: Dictionary in staged["services"]:
			if service["service"] == "school_day.actor96":
				terminal_count += 1
				check(iteration == draws_to_terminal - 1 and service["payload"]["sequence_slot"] == 1,
					label + " completion is the authentic result1 sequence terminal")
		history.append(_row(state, result.presentation))
		last_staged = staged
		if iteration < draws_to_terminal - 1:
			state.frame_count += 1
			state.mode_step_timer += 1
	check(terminal_count == 1 and state.rule_state["actor_state"]["96"]["flags"] == 0,
		label + " actor genuinely completes after original durations")
	var commands: Array[Dictionary] = []
	commands.assign(last_staged["draw_commands"])
	check(commands[-1]["id"] == unique_frame["id"], label + " last draw is the unique original result1 texture")
	var pixels_before: String = _image_sha(commands[-1]["image"])
	var composed: Dictionary = app.holiday_hud.compose(state.rule_state, state.projection(), app._holiday_profile(), history, commands, last_staged["active_lba"])
	check(composed["error"].is_empty() and not composed["retain"] and composed["commands"].size() == 1,
		label + " actual HUD accepts source result1 endpoint: " + composed["error"])
	check(_image_sha(commands[-1]["image"]) == pixels_before, label + " HUD leaves immutable source pixels unchanged")
	var tampered: Array[Dictionary] = []
	for command: Dictionary in commands:
		tampered.append(command.duplicate(true))
	var wrong: Image = tampered[-1]["image"].duplicate()
	var pixel: Color = wrong.get_pixel(0, 0)
	wrong.set_pixel(0, 0, Color(1.0 - pixel.r, 1.0 - pixel.g, 1.0 - pixel.b, pixel.a))
	tampered[-1]["image"] = wrong
	var rejected: Dictionary = app.holiday_hud.compose(state.rule_state, state.projection(), app._holiday_profile(), history, tampered, last_staged["active_lba"])
	check(not rejected["error"].is_empty() and rejected["commands"].is_empty(),
		label + " rejects wrong pixels despite a genuine result1 texture ID")
	branches.append({"synthetic_branch": true, "sunday_index": sunday_index, "result": 1,
		"source_frame_id": unique_frame["id"], "native_draws": draws_to_terminal,
		"accepted_error": composed["error"], "tamper_error": rejected["error"]})

func _test_returned(app: Boot, source: Dictionary) -> void:
	var state: State = _copy_native(source)
	_synthetic_queue(state, 15, true)
	var result := Session.FrameResult.new()
	result.presentation.append(_screen_init("next_command"))
	var staged: Dictionary = app.holiday_scene.stage(state, 0, result)
	check(staged["error"].is_empty(), "synthetic returned index16 resolves the exact original next-command room")
	if not staged["error"].is_empty():
		return
	var history: Array[Dictionary] = [_row(state, result.presentation)]
	var commands: Array[Dictionary] = []
	commands.assign(staged["draw_commands"])
	var composed: Dictionary = app.holiday_hud.compose(state.rule_state, state.projection(), app._holiday_profile(), history, commands, staged["active_lba"])
	check(composed["error"].is_empty() and not composed["retain"],
		"synthetic post-Sunday queue16 accepts prior source Sunday15/result1: " + composed["error"])
	state.rule_state["day_events"][15][0] = 0xff
	var rejected: Dictionary = app.holiday_hud.compose(state.rule_state, state.projection(), app._holiday_profile(), history, commands, staged["active_lba"])
	check(not rejected["error"].is_empty(), "dynamic returned boundary requires the actual prior Sunday opcode")

func _image_sha(image: Image) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(image.get_data())
	return hash.finish().hex_encode()

func _finish() -> void:
	var report: Dictionary = {"scope": "synthetic result1/queue-position service and HUD branches using original external media; no player-route/native result1 acceptance",
		"checks": checks, "failures": failures, "branches": branches}
	var file: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("holiday-result1-hud.json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(report, "\t"))
		file.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "synthetic_branches": branches.size()}))
	quit(0 if failures.is_empty() else 1)
