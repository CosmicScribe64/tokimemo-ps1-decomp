extends SceneTree
## T-29028. Original-backed Boot/save/HUD/service composition at explicit test
## boundaries. The May1 phase fixture is synthetic, not a played/native route.

const Boot = preload("res://app/playable_boot.gd")
const SaveAdapter = preload("res://services/playable_save_adapter.gd")
const MonthStart = preload("res://scenes/month_start_controller.gd")
const Calendar = preload("res://rules/calendar.gd")
const CalendarSource = preload("res://services/native_calendar_source.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")

var checks: int = 0
var failures: Array[String] = []
var apps: Array[Boot] = []
var diagnostics: Dictionary = {}
var renderer_saw_trusted_store: bool = false
var expected_renderer_store: MonthStart

func _initialize() -> void:
	call_deferred("_run")

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures.append(label)

func _new_app() -> Boot:
	var app := Boot.new()
	app.canvas = Boot.Canvas.new()
	app.pad = Boot.Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	apps.append(app)
	return app

func _make_app(source_path: String) -> Boot:
	var saved: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	OS.set_environment("TOKIMEMO_CALENDAR_SOURCE", source_path)
	var app: Boot = _new_app()
	var error: String = app._load_inputs()
	if error.is_empty():
		error = app._reset()
	OS.set_environment("TOKIMEMO_CALENDAR_SOURCE", saved)
	check(error.is_empty(), "actual Boot preflight/reset: " + error)
	return app

func _preflight_rejected(path: String, wanted: String) -> void:
	var saved: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	OS.set_environment("TOKIMEMO_CALENDAR_SOURCE", path)
	var app: Boot = _new_app()
	var error: String = app._load_inputs()
	OS.set_environment("TOKIMEMO_CALENDAR_SOURCE", saved)
	check(error.begins_with("mandatory calendar source preflight:") and error.contains(wanted),
		"explicit invalid calendar launch rejects before reset: " + error)
	check(app.controller != null and app.controller.month_start.identity().is_empty(),
		"rejected explicit source grants no controller identity")
	diagnostics[path.get_file()] = error

func _write(path: String, bytes: PackedByteArray) -> bool:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return false
	file.store_buffer(bytes)
	file.close()
	return true

func _run() -> void:
	var source_path: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	var output: String = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	check(not source_path.is_empty(), "calendar source is an explicit external launch input")
	var original: PackedByteArray = FileAccess.get_file_as_bytes(source_path)
	check(original.size() == CalendarSource.SOURCE_SIZE, "original source fixture has its exact extent")
	if original.size() != CalendarSource.SOURCE_SIZE:
		_finish(output)
		return
	var truncated_path: String = output.path_join("calendar-truncated-original.bin")
	var mutated_path: String = output.path_join("calendar-mutated-original.bin")
	check(_write(truncated_path, original.slice(0, original.size() - 1)), "external truncated original fixture writes")
	var mutated: PackedByteArray = original.duplicate()
	mutated[mutated.size() - 1] = mutated[-1] ^ 1
	check(_write(mutated_path, mutated), "external one-byte mutation fixture writes")
	_preflight_rejected(truncated_path, "size")
	_preflight_rejected(mutated_path, "identity")
	var app: Boot = _make_app(source_path)
	if app.controller == null or app.state == null or app.hud == null:
		check(false, "original Boot fixture is unavailable")
		_finish(output)
		return
	check(app.controller.month_start.identity() == CalendarSource.SOURCE_SHA,
		"live controller owns verified CALROM preflight")
	check(app.state.rule_state["resources"].get(MonthStart.SOURCE_KEY) == CalendarSource.SOURCE_SHA,
		"Boot reset records the trusted calendar SHA in primitive metadata")
	check(app.session.router.handler(0x38).get_object() == app.controller.month_start,
		"Boot binds source mode38 to its retained month controller")
	var adapter := SaveAdapter.new({"source_version": "t29028-month-start-app-test",
		"content_version": "external-original-calendar", "behavior_version": "retained-source-v1"})
	var captured: Dictionary = adapter.capture(app, "command")
	check(captured["error"].is_empty(), "calendar-enabled genuine April4 command captures: " + captured["error"])
	if not captured["error"].is_empty():
		_finish(output)
		return
	var snapshot: Dictionary = captured["snapshot"]
	var target: Boot = _make_app(source_path)
	var trusted: MonthStart = target.controller.month_start
	var original_source: Variant = trusted.get("_source")
	var restored: String = adapter.restore(target, snapshot, target._present_committed)
	check(restored.is_empty(), "calendar-enabled command restores through real renderer: " + restored)
	check(target.controller.month_start == trusted and target.controller.month_start.get("_source") == original_source,
		"restore retains the exact trusted live source object without reopening")
	check(target.controller.month_start != app.controller.month_start,
		"save data cannot replace target ownership with the source app's store")
	check(target.session.router.handler(0x38).get_object() == trusted,
		"postrestore mode38 handler remains bound to the live trusted store")
	for tamper: String in ["missing", "different"]:
		var changed: Dictionary = snapshot.duplicate(true)
		var resources: Dictionary = changed["gameplay_state"]["state"]["rule_state"]["resources"]
		if tamper == "missing":
			resources.erase(MonthStart.SOURCE_KEY)
		else:
			resources[MonthStart.SOURCE_KEY] = "0".repeat(64)
		var before: Dictionary = _fingerprint(target)
		var refused: String = adapter.restore(target, changed)
		check(refused.contains("calendar source identity"), "saved %s calendar identity rejects: %s" % [tamper, refused])
		check(_fingerprint(target) == before, "saved %s identity rejection is atomic" % tamper)
		diagnostics["saved_" + tamper] = refused
	var no_source: Boot = _make_app("")
	check(no_source.controller.month_start.identity().is_empty()
		and not no_source.state.rule_state["resources"].has(MonthStart.SOURCE_KEY),
		"unconfigured launch invents no calendar capability")
	var closed_before: Dictionary = _fingerprint(no_source)
	var missing_live: String = adapter.restore(no_source, snapshot)
	check(not missing_live.is_empty() and _fingerprint(no_source) == closed_before,
		"save cannot grant missing trusted source; rejection is atomic")
	diagnostics["missing_live_source"] = missing_live
	# A failing post-apply renderer sees only the candidate restore and returns
	# an error. It does no I/O and never mutates the shared source controller.
	expected_renderer_store = trusted
	var before_renderer: Dictionary = _fingerprint(target)
	var renderer_error: String = adapter.restore(target, snapshot, _refuse_renderer)
	check(renderer_error == "invented renderer rebind failure" and renderer_saw_trusted_store,
		"renderer failure occurs after candidate source/handler rebind")
	check(_fingerprint(target) == before_renderer and target.controller.month_start == trusted,
		"failed renderer rebind restores original session/controller/source/queues/canvas")
	_test_date_rollover(target)
	_test_month_phase(target)
	_finish(output)

func _refuse_renderer(app: Boot) -> String:
	renderer_saw_trusted_store = app.controller.month_start == expected_renderer_store \
		and app.session.router.handler(0x38).get_object() == expected_renderer_store
	return "invented renderer rebind failure"

func _fingerprint(app: Boot) -> Dictionary:
	return {"session": app.session, "controller": app.controller,
		"holiday_controller": app.holiday_controller, "source": app.controller.month_start,
		"state": app.state.projection(), "rng": app.session.rng.checkpoint(),
		"clock": app.session.clock.snapshot(), "input": app.session.latched_input.snapshot(),
		"pending": app.pending_services.duplicate(true), "services": app.staged_services.duplicate(true),
		"draw": app.staged_commands.duplicate(true), "trace": app.trace.duplicate(true),
		"canvas": app.canvas.get("_commands").duplicate(true), "textures": app.canvas.get("_textures").duplicate(),
		"pad": app.pad.get("_devices").duplicate(true), "previous": app.pad.get("_previous"),
		"identity": app.controller.month_start.identity(), "state_hash": app.session.state_hash()}

func _month_fixture(app: Boot) -> Boot.State:
	# Explicit phase variation from genuine April4 state and the verified source;
	# this is not a player route or an emulator checkpoint for May1.
	var state := Boot.State.new()
	state.restore(app.state.projection())
	state.reset_game_mode_fields(0x38)
	var data: Dictionary = state.rule_state
	data["year"] = 95
	data["month"] = 5
	data["day"] = 1
	data["weekday"] = Calendar.weekday_for_date(95, 5, 1)
	data["day_event_index"] = 0
	data["day_events"][0] = [0xfe, 0, 0, 0]
	data["actor_state"].clear()
	data["resources"].erase(MonthStart.PENDING_KEY)
	data["background_cache"][0] = Boot.MEDIA_ROUTES[0]["background_lba"]
	data["background_show"] = 0
	var source: CalendarSource = app.controller.month_start.get("_source")
	for side: int in range(2):
		var month: Dictionary = source.month(95, 4 + side)
		for key: String in ["day_kind", "marker"]:
			for index: int in range(month[key].size()):
				data["calendar"][side][key][index] = month[key][index]
	return state

func _row(state: Boot.State, presentation: Array) -> Dictionary:
	return {"mode": state.game_mode, "stage": state.mode_stage, "step": state.mode_step,
		"substep": state.substep, "frame_count": state.frame_count,
		"params": state.rule_state["params"].duplicate(), "presentation": presentation.duplicate(true)}

func _test_date_rollover(app: Boot) -> void:
	# A synthetic April30 boundary using both original numeric records. The
	# native date-advance seam must defer their mutation to the FE source load.
	var source_state: Boot.State = _month_fixture(app)
	source_state.game_mode = 0x31
	var data: Dictionary = source_state.rule_state
	data["month"] = 4
	data["day"] = 30
	data["weekday"] = Calendar.weekday_for_date(95, 4, 30)
	data["calendar"][0]["marker"][30] = 0x17 # Explicit legitimate prior-record variation.
	var old_records: Array = data["calendar"].duplicate(true)
	var old_count: int = data["day_count"]
	var legacy := Boot.State.new()
	legacy.restore(source_state.projection())
	legacy.rule_state["resources"].erase(MonthStart.SOURCE_KEY)
	var error: String = Calendar.advance_day(source_state)
	check(error.is_empty() and [data["year"], data["month"], data["day"], data["weekday"]] == [95, 5, 1, 0],
		"source-enabled date advance crosses actual April30 to May1 calendar boundary")
	check(data["calendar"] == old_records, "source-enabled rollover preserves both old calendar records byte-for-byte")
	check(data["day_count"] == old_count + 1 and data["resources"][MonthStart.SOURCE_KEY] == CalendarSource.SOURCE_SHA
		and not data["resources"].has(MonthStart.PENDING_KEY) and source_state.game_mode == 0x31,
		"date advance preserves source identity and defers load/completion/mode ownership")
	var legacy_error: String = Calendar.advance_day(legacy)
	var legacy_data: Dictionary = legacy.rule_state
	check(legacy_error.is_empty() and [legacy_data["year"], legacy_data["month"], legacy_data["day"], legacy_data["weekday"]] == [95, 5, 1, 0],
		"legacy no-source date advance still crosses to May1")
	check(legacy_data["calendar"] != old_records and legacy_data["calendar"][0]["day_kind"][0] == 31
		and legacy_data["calendar"][1]["day_kind"][0] == 30
		and legacy_data["calendar"][0]["marker"][0] == 0 and legacy_data["calendar"][1]["marker"][0] == 3,
		"legacy no-source rollover still synthesizes May/June records")
	check(legacy_data["calendar"][0]["marker"][30] != 0x17,
		"legacy synthesis replaces the old April-only marker rather than silently retaining stale rows")

func _test_month_phase(app: Boot) -> void:
	var state: Boot.State = _month_fixture(app)
	var rng_before: Dictionary = app.session.rng.checkpoint()
	var request := Session.FrameResult.new()
	var handler: Callable = app.session.router.handler(0x38)
	handler.call(state, InputFrame.new(0), app.session.rng, request)
	check(request.error.is_empty() and state.substep == 1, "retained postrestore mode38 handler emits the real source request")
	if not request.error.is_empty():
		return
	app.staged_commands.clear()
	app.staged_services.clear()
	app._native_draw(state, app.session.rng, request)
	check(request.error.is_empty(), "Boot mode38 stages source media: " + request.error)
	check(app.staged_commands == app.command_backgrounds[0], "Boot month draw selects the exact active source background")
	check(app.staged_services.size() == 1 and app.staged_services[0]["service"] == MonthStart.SERVICE
		and app.staged_services[0]["payload"]["source_sha256"] == CalendarSource.SOURCE_SHA
		and app.staged_services[0]["payload"]["ready"] == true,
		"Boot stages verified readiness only after the complete successful draw")
	check(state.rule_state["resources"][MonthStart.PENDING_KEY]["ready"] == false,
		"draw staging does not deliver resource completion inline")
	var history: Array[Dictionary] = [_row(state, request.presentation)]
	var before_hud: Dictionary = state.projection()
	var composed: Dictionary = app.hud.compose(state.rule_state, state.projection(), history, app.staged_commands)
	check(not composed.is_empty() and composed.get("image") is Image and composed["image"].get_size() == Vector2i(320, 240),
		"real source HUD composes mode38 read-wait phase: " + app.hud.error)
	check(state.projection() == before_hud, "HUD mode38 composition leaves rules/core untouched")
	var invalid: Boot.State = _month_fixture(app)
	invalid.substep = 1
	invalid.rule_state["resources"][MonthStart.PENDING_KEY] = state.rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
	invalid.rule_state["background_cache"][0] = 1 # Explicit late-failure injection.
	var bad_result := Session.FrameResult.new()
	bad_result.presentation.assign(request.presentation)
	var before_commands: Array = app.staged_commands.duplicate(true)
	var before_services: Array = app.staged_services.duplicate(true)
	app._native_draw(invalid, app.session.rng, bad_result)
	check(bad_result.error.contains("active source background"), "unknown month background rejects after readiness preparation")
	check(app.staged_commands == before_commands and app.staged_services == before_services,
		"late draw failure leaves existing staging untouched and publishes no readiness")
	diagnostics["late_draw_failure"] = bad_result.error
	if app.staged_services.size() != 1:
		return
	var completion: Dictionary = app.staged_services[0]
	var delivered := Session.FrameResult.new()
	app.controller.month_start.service_completion(Session.ServiceEvent.new(0, Session.Phase.BEFORE_MODE,
		Session.ServiceKind.COMPLETION, completion["service"], completion["payload"]), state, app.session.rng, delivered)
	check(delivered.error.is_empty(), "staged exact source completion is accepted in its native wait")
	var rotated := Session.FrameResult.new()
	handler.call(state, InputFrame.new(1), app.session.rng, rotated)
	check(rotated.error.is_empty() and state.mode_stage == 1, "postrestore retained source performs native calendar rotation: " + rotated.error)
	if not rotated.error.is_empty():
		return
	app.staged_commands.clear()
	app.staged_services.clear()
	app._native_draw(state, app.session.rng, rotated)
	history.append(_row(state, rotated.presentation))
	var uploaded: Dictionary = app.hud.compose(state.rule_state, state.projection(), history, app.staged_commands)
	check(not uploaded.is_empty() and uploaded.get("image") is Image, "real source HUD composes uploaded mode38 stage1: " + app.hud.error)
	state.mode_stage = 2
	var unsupported: Dictionary = app.hud.compose(state.rule_state, state.projection(), history, app.staged_commands)
	check(unsupported.is_empty() and not app.hud.error.is_empty(), "HUD does not broaden mode38 beyond source stages0/1")
	check(app.session.rng.checkpoint() == rng_before, "month source/staging/HUD ownership consumes no RNG")

func _finish(output: String) -> void:
	var pins: Dictionary = {}
	for path: String in ["res://app/playable_boot.gd", "res://scenes/month_start_controller.gd",
		"res://services/playable_save_adapter.gd", "res://services/native_calendar_source.gd",
		"res://presentation/live_hud_adapter.gd", "res://presentation/command_hud_presenter.gd",
		"res://rules/calendar.gd", "res://tests/month_start/run_month_start_app.gd"]:
		pins[path] = FileAccess.get_sha256(path)
	var file: FileAccess = FileAccess.open(output.path_join("month-start-app.json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify({"checks": checks, "failures": failures,
			"diagnostics": diagnostics, "production_pins": pins,
			"scope": "actual Boot original preflight/save ownership plus synthetic original-backed mode38 staging/HUD phases; no played May route, emulator or audiovisual parity"}, "\t"))
		file.close()
	for failure: String in failures:
		push_error(failure)
	print(JSON.stringify({"checks": checks, "failures": failures}))
	for app: Boot in apps:
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
	quit(0 if failures.is_empty() else 1)
