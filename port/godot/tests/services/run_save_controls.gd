extends SceneTree
## Actual app input-handler/storage/continuation tests. Fixtures stay external.
## write/read run in separate engine containers sharing only user data and proof.
const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
var apps: Array[Boot] = []
var failures: Array[String] = []
var checks: Array[String] = []
var report: Dictionary = {}
var reference: Dictionary = {}
var output: String = ""

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	output = OS.get_environment("TOKIMEMO_SAVE_UI_OUTPUT")
	_check(output.is_absolute_path() and OS.get_user_data_dir().begins_with("/userdata/"),
		"explicit proof directory and persistent user-data mount are active")
	var phase: String = OS.get_environment("TOKIMEMO_SAVE_UI_PHASE")
	if phase == "write":
		_write_phase()
	elif phase == "read":
		var parser: JSON = JSON.new()
		if parser.parse(FileAccess.get_file_as_string(output.path_join("ui-reference.json"))) != OK or not parser.data is Dictionary:
			_check(false, "previous process continuation reference is available")
		else:
			reference = parser.data
			_read_phase()
	else:
		_check(false, "write/read phase is explicit")
	report["phase"] = phase
	report["checks"] = checks
	report["failures"] = failures
	report["status"] = "PASS" if failures.is_empty() else "FAIL"
	report["scope"] = "actual app _input F5/F9 InputEventKey injection, separate-engine persistent disk saves, canonical session/controller/media; no human OS keyboard or native card/audio timing claim"
	if output.is_absolute_path():
		var file: FileAccess = FileAccess.open(output.path_join("ui-" + phase + "-proof.json"), FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(report, "\t"))
			file.close()
	for app: Boot in apps:
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
	if failures.is_empty():
		print("PASS: save controls %s phase; %d checks" % [phase, checks.size()])
		quit(0)
	else:
		for error: String in failures:
			push_error(error)
		quit(1)

func _write_phase() -> void:
	for scenario: String in ["command", "result", "explicit", "rest", "rest_command", "rest_week", "unsupported", "focus", "missing", "corrupt", "neutral", "busy"]:
		_clear_case(scenario)
	var command: Boot = _app("command")
	if command == null:
		return
	var command_before: String = _simulation(command)
	_press(command, KEY_F5)
	_check(command.save_controls.pending == Controls.Request.SAVE and command.save_controls.storage_calls == 0
		and not FileAccess.file_exists(_save_path("command")), "F5 input callback queues and performs no file I/O")
	command._physics_process(1.0 / 60.0)
	_check(command.save_controls.completed_requests == 1 and command.save_controls.last_checkpoint == "command"
		and FileAccess.file_exists(_save_path("command")) and _simulation(command) == command_before,
		"F5 publishes captured command boundary between steps without a logic tick")
	reference["command_file_sha256"] = FileAccess.get_sha256(_save_path("command"))
	reference["command_continuation"] = _week(command)
	_check(reference["command_continuation"].size() == 243, "uninterrupted command reference reaches all 243 supported commits")
	var result: Boot = _app("result")
	if result == null:
		return
	_advance(result, 80)
	var result_before: String = _simulation(result)
	_press(result, KEY_F5)
	result._physics_process(1.0 / 60.0)
	_check(result.save_controls.last_error.is_empty() and result.save_controls.last_checkpoint == "first_result"
		and _simulation(result) == result_before and not result.checkpoints.is_empty(),
		"F5 saves the actual first result and nonempty controller ledger without advancing")
	reference["result_file_sha256"] = FileAccess.get_sha256(_save_path("result"))
	reference["result_continuation"] = _week(result)
	_check(reference["result_continuation"].size() == 163, "uninterrupted first-result reference has 163 remaining supported commits")
	var explicit: Boot = _app("explicit", false)
	if explicit == null:
		return
	_advance(explicit, 80)
	_check(explicit.stopped and not explicit.continue_week, "explicit first-result app policy is paused")
	_press(explicit, KEY_F5)
	explicit._physics_process(1.0 / 60.0)
	_check(explicit.save_controls.last_error.is_empty() and FileAccess.file_exists(_save_path("explicit"))
		and explicit.session.clock.logic_iterations == 80 and explicit.stopped,
		"F5 works while explicit first-result policy is stopped")
	reference["explicit_file_sha256"] = FileAccess.get_sha256(_save_path("explicit"))
	reference["explicit_simulation_sha256"] = _simulation(explicit).sha256_text()
	var rest: Boot = _app("rest", false)
	if rest == null:
		return
	_advance(rest, 163, 7)
	var rest_before: String = _simulation(rest)
	_check(rest.stopped and rest.state.rule_state["last_command"] == 7, "actual rest command reaches its first result")
	_press(rest, KEY_F5)
	rest._physics_process(1.0 / 60.0)
	_check(rest.save_controls.last_error.is_empty() and rest.save_controls.last_checkpoint == "first_result"
		and _simulation(rest) == rest_before, "F5 saves the actual rest result without a native tick")
	reference["rest_file_sha256"] = FileAccess.get_sha256(_save_path("rest"))
	reference["rest_simulation_sha256"] = _simulation(rest).sha256_text()
	reference["rest_render_sha256"] = _render(rest).sha256_text()
	reference["rest_replay"] = rest.trace.map(func(row: Dictionary) -> Array: return [row["state_hash"], row["event_hash"]])
	var rest_command: Boot = _app("rest_command")
	if rest_command == null:
		return
	_press(rest_command, KEY_F5)
	rest_command._physics_process(1.0 / 60.0)
	_check(rest_command.save_controls.last_error.is_empty(), "F5 saves the default rest-week command entry")
	reference["rest_command_file_sha256"] = FileAccess.get_sha256(_save_path("rest_command"))
	reference["rest_command_continuation"] = _week(rest_command, 7)
	_check(reference["rest_command_continuation"].size() == 335, "default rest-week reference preserves all 335 supported commits")
	var rest_week: Boot = _app("rest_week")
	if rest_week == null:
		return
	_advance(rest_week, 163, 7)
	var rest_week_before: String = _simulation(rest_week)
	_press(rest_week, KEY_F5)
	rest_week._physics_process(1.0 / 60.0)
	_check(rest_week.save_controls.last_error.is_empty() and rest_week.continue_week and not rest_week.stopped
		and _simulation(rest_week) == rest_week_before, "F5 preserves the reviewed continuing rest first-result policy")
	reference["rest_week_file_sha256"] = FileAccess.get_sha256(_save_path("rest_week"))
	reference["rest_week_render_sha256"] = _render(rest_week).sha256_text()
	reference["rest_week_continuation"] = _week(rest_week, 7)
	_check(reference["rest_week_continuation"].size() == 172, "default rest-result reference has all 172 remaining commits")
	_negative_controls()
	var file: FileAccess = FileAccess.open(output.path_join("ui-reference.json"), FileAccess.WRITE)
	file.store_string(JSON.stringify(reference, "\t"))
	file.close()
	report["persisted_files"] = [reference["command_file_sha256"], reference["result_file_sha256"], reference["explicit_file_sha256"], reference["rest_file_sha256"]]

func _read_phase() -> void:
	for scenario: String in ["command", "result", "explicit", "rest", "rest_command", "rest_week"]:
		_check(FileAccess.get_sha256(_save_path(scenario)) == reference[scenario + "_file_sha256"],
			"quick-save bytes survive a new engine/container for " + scenario)
	var command: Boot = _app("command")
	if command == null:
		return
	_unsupported_stop(command)
	var held_before: String = _simulation(command)
	_press(command, KEY_F9)
	command._physics_process(1.0 / 60.0)
	_check(command.stopped and command.save_controls.storage_calls == 0
		and command.save_controls.last_error.contains("Release") and _simulation(command) == held_before,
		"F9 cannot recover while a game device remains held")
	_device(command, KEY_ENTER, false)
	_check(command.pad.get("_previous") != 0 and command.session.latched_input.held != 0,
		"actual rejected command leaves a stale latch after physical key release")
	var tick_before: int = command.session.clock.logic_iterations
	_press(command, KEY_F9)
	_check(command.save_controls.pending == Controls.Request.LOAD and command.save_controls.storage_calls == 0
		and command.session.clock.logic_iterations == tick_before, "F9 input callback queues without loading or stepping")
	command._physics_process(1.0 / 60.0)
	_check(command.save_controls.last_error.is_empty() and command.save_controls.completed_requests == 1
		and command.save_controls.last_checkpoint == "command" and command.session.clock.logic_iterations == 0
		and command.failure.is_empty() and not command.stopped,
		"F9 recovers the stopped command after physical release without R reset or an extra tick")
	var command_hashes: Array = _week(command)
	_check(command_hashes == reference["command_continuation"] and command_hashes.size() == 243,
		"F5/F9 across engine processes preserves all 243 command/week state-event pairs")
	var result: Boot = _app("result")
	if result == null:
		return
	_advance(result, 30)
	_press(result, KEY_F9)
	result._physics_process(1.0 / 60.0)
	_check(result.save_controls.last_error.is_empty() and result.session.clock.logic_iterations == 80
		and result.state.rule_state["day_event_index"] == 1 and result.continue_week,
		"F9 restores first-result state/controller policy before continuing")
	var result_hashes: Array = _week(result)
	_check(result_hashes == reference["result_continuation"] and result_hashes.size() == 163,
		"F5/F9 across engine processes preserves all 163 first-result/week state-event pairs")
	var explicit: Boot = _app("explicit")
	if explicit == null:
		return
	_press(explicit, KEY_F9)
	explicit._physics_process(1.0 / 60.0)
	_check(explicit.save_controls.last_error.is_empty() and explicit.stopped and not explicit.continue_week
		and _simulation(explicit).sha256_text() == reference["explicit_simulation_sha256"],
		"F9 restores the explicit paused first-result policy and complete simulation")
	explicit._physics_process(1.0 / 60.0)
	_check(explicit.session.clock.logic_iterations == 80, "loaded paused first result remains stopped until a user action")
	_focus_resume(explicit)
	var explicit_bytes: String = FileAccess.get_sha256(_save_path("explicit"))
	var calls_before_replay: int = explicit.save_controls.storage_calls
	_press(explicit, KEY_F5)
	_press(explicit, KEY_R)
	explicit._physics_process(1.0 / 60.0)
	_check(explicit.session.clock.logic_iterations == 1 and explicit.state.game_mode == 0x34
		and explicit.save_controls.storage_calls == calls_before_replay
		and FileAccess.get_sha256(_save_path("explicit")) == explicit_bytes,
		"R cancels a queued save and replays a loaded result without a write")
	var rest: Boot = _app("rest")
	if rest == null:
		return
	_advance(rest, 30)
	_press(rest, KEY_F9)
	rest._physics_process(1.0 / 60.0)
	_check(rest.save_controls.last_error.is_empty() and rest.stopped and rest.controller.command_capabilities == [0, 7]
		and _simulation(rest).sha256_text() == reference["rest_simulation_sha256"]
		and _render(rest).sha256_text() == reference["rest_render_sha256"],
		"F9 restores actual rest state and exact source image payloads into a fresh default-command0 app")
	_focus_resume(rest)
	_press(rest, KEY_R)
	rest._physics_process(1.0 / 60.0)
	_advance(rest, 162, 7)
	var rest_replay: Array = rest.trace.map(func(row: Dictionary) -> Array: return [row["state_hash"], row["event_hash"]])
	_check(rest_replay == reference["rest_replay"] and rest_replay.size() == 163,
		"loaded rest result can replay all 163 original state-event pairs with rest eligibility retained")
	var rest_command: Boot = _app("rest_command")
	if rest_command == null:
		return
	_advance(rest_command, 30)
	_press(rest_command, KEY_F9)
	rest_command._physics_process(1.0 / 60.0)
	_check(rest_command.save_controls.last_error.is_empty() and rest_command.session.clock.logic_iterations == 0
		and rest_command.continue_week, "F9 restores the current default-week command entry")
	var rest_command_hashes: Array = _week(rest_command, 7)
	_check(rest_command_hashes == reference["rest_command_continuation"] and rest_command_hashes.size() == 335,
		"F5/F9 preserves all 335 default rest-week state-event pairs across processes")
	var rest_week: Boot = _app("rest_week")
	if rest_week == null:
		return
	_advance(rest_week, 30)
	_press(rest_week, KEY_F9)
	rest_week._physics_process(1.0 / 60.0)
	_check(rest_week.save_controls.last_error.is_empty() and rest_week.session.clock.logic_iterations == 163
		and rest_week.continue_week and not rest_week.stopped and _render(rest_week).sha256_text() == reference["rest_week_render_sha256"],
		"F9 restores continuing rest first-result state, policy and exact media payloads")
	var rest_week_hashes: Array = _week(rest_week, 7)
	_check(rest_week_hashes == reference["rest_week_continuation"] and rest_week_hashes.size() == 172,
		"F5/F9 preserves all 172 continuing rest-result state-event pairs across processes")
	report["command_continuation_pairs"] = command_hashes.size()
	report["first_result_continuation_pairs"] = result_hashes.size()
	report["command_continuation_sha256"] = JSON.stringify(command_hashes).sha256_text()
	report["first_result_continuation_sha256"] = JSON.stringify(result_hashes).sha256_text()
	report["rest_replay_pairs"] = rest_replay.size()
	report["rest_command_continuation_pairs"] = rest_command_hashes.size()
	report["rest_first_result_continuation_pairs"] = rest_week_hashes.size()

func _negative_controls() -> void:
	var unsupported: Boot = _app("unsupported")
	if unsupported == null:
		return
	_advance(unsupported, 50)
	var before: String = _simulation(unsupported)
	_press(unsupported, KEY_F5)
	unsupported._physics_process(1.0 / 60.0)
	_check(not unsupported.save_controls.last_error.is_empty() and unsupported.save_controls.storage_calls == 0
		and not FileAccess.file_exists(_save_path("unsupported")) and _simulation(unsupported) == before,
		"F5 during activity reports unsupported boundary without saving or mutating simulation")
	_check(unsupported.save_controls.notice.text.contains("command selection or the first result"),
		"unsupported save diagnostic states the supported user boundaries")
	var focus: Boot = _app("focus")
	if focus == null:
		return
	_press(focus, KEY_F5)
	focus._notification(Node.NOTIFICATION_WM_WINDOW_FOCUS_OUT)
	before = _simulation(focus)
	focus._physics_process(1.0 / 60.0)
	_check(focus.save_controls.pending == Controls.Request.NONE and focus.save_controls.storage_calls == 0
		and focus.focus_paused and focus._resume_input_pending and _simulation(focus) == before
		and not FileAccess.file_exists(_save_path("focus")), "queued save cancels on focus loss without I/O or unpausing")
	_press(focus, KEY_F9)
	_check(focus.save_controls.pending == Controls.Request.NONE and focus.save_controls.storage_calls == 0,
		"F9 while unfocused cannot queue a load")
	var missing: Boot = _app("missing")
	if missing == null:
		return
	before = _simulation(missing)
	_press(missing, KEY_F9)
	missing._physics_process(1.0 / 60.0)
	_check(missing.save_controls.last_error.contains("Load failed") and _simulation(missing) == before,
		"missing quick save is a visible load error with no mutable apply")
	var corrupt: Boot = _app("corrupt")
	if corrupt == null:
		return
	_write_bytes(_save_path("corrupt"), "{truncated".to_utf8_buffer())
	_unsupported_stop(corrupt)
	_device(corrupt, KEY_ENTER, false)
	before = _simulation(corrupt)
	_press(corrupt, KEY_F9)
	corrupt._physics_process(1.0 / 60.0)
	_check(corrupt.save_controls.last_error.contains("Load failed") and _simulation(corrupt) == before,
		"corrupt F9 recovery leaves the stopped simulation and stale latch untouched")
	var neutral: Boot = _app("neutral")
	if neutral == null:
		return
	var held: InputEventKey = InputEventKey.new()
	held.physical_keycode = KEY_ENTER
	held.pressed = true
	neutral.pad._input(held)
	_press(neutral, KEY_F5)
	neutral._physics_process(1.0 / 60.0)
	_check(neutral.save_controls.last_error.contains("Release") and neutral.save_controls.storage_calls == 0,
		"held game input cannot be captured as a phantom edge by a quick save")
	var busy: Boot = _app("busy")
	if busy == null:
		return
	busy.video_capture_busy = true
	_press(busy, KEY_F5)
	_check(busy.save_controls.pending == Controls.Request.NONE and busy.save_controls.storage_calls == 0,
		"save input rejects a busy capture")
	busy.video_capture_busy = false
	busy.session.rng.begin_step(func(_reason: String) -> void: pass)
	_press(busy, KEY_F5)
	_check(busy.save_controls.pending == Controls.Request.NONE and busy.save_controls.storage_calls == 0,
		"save input cannot perform I/O in an active session step")
	busy.session.rng.end_step()
	var echo: InputEventKey = InputEventKey.new()
	echo.physical_keycode = KEY_F5
	echo.pressed = true
	echo.echo = true
	busy._input(echo)
	_check(busy.save_controls.pending == Controls.Request.NONE, "OS key repeat does not enqueue repeated save writes")
	_press(busy, KEY_F5)
	_press(busy, KEY_F9)
	busy._physics_process(1.0 / 60.0)
	_check(busy.save_controls.storage_calls == 1 and busy.save_controls.last_action == "save",
		"a second request cannot replace the queued user action")

func _app(scenario: String, continue_week: bool = true) -> Boot:
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.continue_week = continue_week
	var error: String = app._load_inputs()
	if error.is_empty(): error = app._reset()
	if error.is_empty(): error = app.save_controls.configure(app, _case_root(scenario))
	if not error.is_empty():
		_check(false, "actual app save-controls preflight: " + error)
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
		return null
	apps.append(app)
	return app

func _advance(app: Boot, count: int, command: int = 0) -> void:
	for index: int in range(count):
		_native_input(app, command)
		var before: int = app.trace.size()
		app._physics_process(1.0 / 60.0)
		if app.trace.size() != before + 1:
			_check(false, "actual app commits the requested input/logic step")
			return

func _week(app: Boot, command: int = 0) -> Array:
	var hashes: Array = []
	for index: int in range(601):
		_native_input(app, command)
		var before: int = app.trace.size()
		app._physics_process(1.0 / 60.0)
		if app.trace.size() == before + 1:
			var row: Dictionary = app.trace.back()
			hashes.append([row["state_hash"], row["event_hash"]])
		if app.stopped:
			break
	_check(app.failure.is_empty() and not app.supported_stop.is_empty()
		and app.supported_stop.get("before_hash") == app.supported_stop.get("after_hash"),
		"actual app reaches its declared supported stop without committing unsupported work")
	return hashes

func _native_input(app: Boot, command: int = 0) -> void:
	var tick: int = app.session.clock.logic_iterations
	if command == 7 and tick < 75 and tick % 10 in [7, 1]:
		_device(app, KEY_RIGHT, tick % 10 == 7)
	var press_tick: int = 97 if command == 7 else 17
	var release_tick: int = 103 if command == 7 else 23
	if tick in [press_tick, release_tick]:
		var event: InputEventKey = InputEventKey.new()
		event.physical_keycode = KEY_ENTER
		event.pressed = tick == press_tick
		app.pad._input(event)

func _press(app: Boot, key: int) -> void:
	var event: InputEventKey = InputEventKey.new()
	event.physical_keycode = key
	event.pressed = true
	app._input(event)

func _device(app: Boot, key: int, pressed: bool) -> void:
	var event: InputEventKey = InputEventKey.new()
	event.physical_keycode = key
	event.pressed = pressed
	app.pad._input(event)

func _unsupported_stop(app: Boot) -> void:
	_device(app, KEY_RIGHT, true)
	app._physics_process(1.0 / 60.0)
	_device(app, KEY_RIGHT, false)
	app._physics_process(1.0 / 60.0)
	app._physics_process(1.0 / 60.0)
	_device(app, KEY_ENTER, true)
	app._physics_process(1.0 / 60.0)
	app._physics_process(1.0 / 60.0)
	_check(app.stopped and not app.failure.is_empty() and app.state.game_mode == 0x34
		and app.state.rule_state["menu_cursor"] == 1 and app.session.latched_input.held != 0,
		"actual right/confirm input reaches a rejected-command stop with its committed latch retained")

func _focus_resume(app: Boot) -> void:
	var before: String = _gameplay(app)
	var calls: int = app.save_controls.storage_calls
	app._notification(Node.NOTIFICATION_WM_WINDOW_FOCUS_OUT)
	app._notification(Node.NOTIFICATION_APPLICATION_FOCUS_OUT)
	_press(app, KEY_F9)
	app._physics_process(1.0 / 60.0)
	_check(app.focus_paused and app.playback_paused and app.save_controls.pending == Controls.Request.NONE
		and app.save_controls.storage_calls == calls and _gameplay(app) == before,
		"stopped-app F9 cannot bypass either focus pause or mutate gameplay")
	app._notification(Node.NOTIFICATION_WM_WINDOW_FOCUS_IN)
	app._physics_process(1.0 / 60.0)
	_check(app.focus_paused and app.save_controls.storage_calls == calls and _gameplay(app) == before,
		"partial focus return cannot load or resume stopped gameplay")
	app._notification(Node.NOTIFICATION_APPLICATION_FOCUS_IN)
	_press(app, KEY_F9)
	_check(app._resume_input_pending and app.save_controls.pending == Controls.Request.NONE
		and app.save_controls.storage_calls == calls, "F9 waits for the normal neutral focus-resume boundary")
	app._physics_process(1.0 / 60.0)
	_check(not app._resume_input_pending and not app.playback_paused and app.stopped
		and app.session.latched_input.held == 0 and _gameplay(app) == before,
		"stopped focus return clears only stale input and playback pause, with no rule/RNG/clock tick")
	_press(app, KEY_F9)
	app._physics_process(1.0 / 60.0)
	_check(app.save_controls.last_error.is_empty() and app.save_controls.storage_calls == calls + 1
		and app.stopped and _gameplay(app) == before, "fresh F9 after stopped focus rearming loads normally")

func _gameplay(app: Boot) -> String:
	return JSON.stringify([app.state.projection(), app.session.rng.checkpoint(), app.session.clock.snapshot(),
		app.pending_services, app.controller.command_capabilities, app.trace, app.checkpoints, app.supported_stop])

func _render(app: Boot) -> String:
	var output: Array = []
	for command: Dictionary in app.staged_commands:
		var image: Image = command["image"]
		var hash: HashingContext = HashingContext.new()
		hash.start(HashingContext.HASH_SHA256)
		hash.update(image.get_data())
		output.append([command["id"], command["position"].x, command["position"].y,
			image.get_width(), image.get_height(), image.get_format(), hash.finish().hex_encode()])
	return JSON.stringify(output)

func _simulation(app: Boot) -> String:
	return JSON.stringify([app.state.projection(), app.session.rng.checkpoint(), app.session.clock.snapshot(),
		app.session.latched_input.snapshot(), app.pending_services, app.pad.get("_devices"), app.pad.get("_previous"),
		app.initial_params, app.stopped, app.failure, app.continue_week, app.trace, app.checkpoints, app.supported_stop])

func _case_root(scenario: String) -> String:
	return OS.get_user_data_dir().path_join("t29006-ui-proof").path_join(scenario)

func _save_path(scenario: String) -> String:
	return _case_root(scenario).path_join("quick.json")

func _clear_case(scenario: String) -> void:
	var root: String = _case_root(scenario)
	DirAccess.make_dir_recursive_absolute(root)
	for suffix: String in ["", ".bak", ".tmp", ".bak.tmp"]:
		if FileAccess.file_exists(root.path_join("quick.json" + suffix)):
			DirAccess.remove_absolute(root.path_join("quick.json" + suffix))

func _write_bytes(path: String, data: PackedByteArray) -> void:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	file.store_buffer(data)
	file.close()

func _check(ok: bool, message: String) -> void:
	if ok: checks.append(message)
	else: failures.append(message)
