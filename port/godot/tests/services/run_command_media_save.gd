extends SceneTree
## Actual captured-command/rest save composition. Original inputs remain external.
const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const Save = preload("res://services/save_service.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/school_day_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
var apps: Array[Boot] = []
var checks: Array[String] = []
var failures: Array[String] = []
var report: Dictionary = {}

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var versions: Dictionary = {"source_version": "687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3",
		"content_version": JSON.stringify([OS.get_environment("TOKIMEMO_BOUNDARY_SHA"), OS.get_environment("TOKIMEMO_TABLES_SHA"),
			FileAccess.get_sha256(OS.get_environment("TOKIMEMO_MEDIA_PACK").path_join("manifest.json"))]).sha256_text(),
		"behavior_version": "command7-f7aa5c83-save-media-v1"}
	var adapter: Adapter = Adapter.new(versions)
	var source: Boot = _app()
	if source == null:
		_finish()
		return
	var output: String = OS.get_environment("TOKIMEMO_COMMAND_SAVE_OUTPUT")
	var save: Save = Save.new(output.path_join("saves"))
	var command: Dictionary = adapter.capture(source, "command")
	_check(command["error"].is_empty() and source.controller.command_capabilities == [0, 7],
		"initial command capture binds both validated launch capabilities")
	if not command["error"].is_empty():
		_finish()
		return
	_check(save.save_slot("command", command["snapshot"])["error"].is_empty(), "command snapshot publishes to disk")
	var target: Boot = _app()
	if target == null:
		_finish()
		return
	var command_load: Dictionary = save.load_slot("command")
	_check(command_load["error"].is_empty() and adapter.restore(target, command_load["snapshot"]).is_empty(),
		"saved command restores into fresh default-command0 app")
	_check(target.controller.command_capabilities == [0, 7], "command restore retains authenticated rest eligibility")
	var original_rest: Array = _rest(source)
	var restored_rest: Array = _rest(target)
	_check(original_rest.size() == 163 and restored_rest == original_rest,
		"all 163 actual rest state/event pairs match after command reload")
	_check(source.failure.is_empty() and source.stopped and source.state.rule_state["last_command"] == 7,
		"native mapped input actually selects rest and reaches its declared first result")
	var result: Dictionary = adapter.capture(source, "first_result")
	_check(result["error"].is_empty(), "actual rest first-result render IDs capture successfully")
	if not result["error"].is_empty():
		_finish()
		return
	_check(save.save_slot("rest_result", result["snapshot"])["error"].is_empty(), "rest first result publishes to disk")
	var fresh: Boot = _app()
	if fresh == null:
		_finish()
		return
	var alias_ids: Array = fresh.background.map(func(draw: Dictionary) -> String: return draw["id"])
	var loaded_result: Dictionary = save.load_slot("rest_result")
	_check(loaded_result["error"].is_empty() and adapter.restore(fresh, loaded_result["snapshot"]).is_empty(),
		"rest first result restores into a fresh app whose legacy aliases still point at command0")
	_check(fresh.background.map(func(draw: Dictionary) -> String: return draw["id"]) == alias_ids,
		"restore preserves immutable app alias configuration")
	_check(_render(fresh) == _render(source) and fresh.canvas._commands == source.canvas._commands,
		"restored rest source IDs, positions and image bytes exactly match actual rest rendering")
	_check(fresh.controller.command_capabilities == [0, 7]
		and JSON.stringify(adapter.capture(fresh, "first_result")["snapshot"]) == JSON.stringify(result["snapshot"]),
		"all rest rule/RNG/clock/input/controller records round-trip with trusted capabilities")
	var first_result_tick: int = fresh.session.clock.logic_iterations
	fresh._physics_process(1.0 / 60.0)
	_check(fresh.stopped and fresh.session.clock.logic_iterations == first_result_tick, "rest reload remains at the explicit stopped result boundary")
	var replay_key: InputEventKey = InputEventKey.new()
	replay_key.physical_keycode = KEY_R
	replay_key.pressed = true
	fresh._input(replay_key)
	fresh._physics_process(1.0 / 60.0)
	var replay: Array = [[fresh.trace[0]["state_hash"], fresh.trace[0]["event_hash"]]] + _rest(fresh)
	_check(replay == original_rest and fresh.controller.command_capabilities == [0, 7],
		"R continuation from restored rest result retains all 163 replay hashes and rest eligibility")
	var selected: Boot = _app()
	if selected != null:
		for index: int in range(90):
			_step(selected)
		var selected_command: Dictionary = adapter.capture(selected, "command")
		_check(selected_command["error"].is_empty() and selected.state.rule_state["menu_cursor"] == 7,
			"actual neutral rest-selected command boundary captures")
		_check(save.save_slot("selected", selected_command["snapshot"])["error"].is_empty(), "rest-selected command publishes to disk")
		var selected_target: Boot = _app()
		if selected_target != null:
			var selected_load: Dictionary = save.load_slot("selected")
			_check(selected_load["error"].is_empty() and adapter.restore(selected_target, selected_load["snapshot"]).is_empty()
				and selected_target.state.rule_state["menu_cursor"] == 7, "rest-selected command restores through exact ordered codec")
			var selected_suffix: Array = _rest(selected)
			var loaded_suffix: Array = _rest(selected_target)
			_check(selected_suffix.size() == 73 and selected_suffix == loaded_suffix,
				"all 73 rest-selected command continuation pairs match after reload")
			report["selected_command_continuation"] = selected_suffix
	_negative(adapter, command["snapshot"], result["snapshot"])
	report["command_to_rest_continuation"] = original_rest
	report["rest_result_replay"] = replay
	report["rest_render_sha256"] = _render(source).sha256_text()
	report["source_caps"] = source.controller.command_capabilities
	report["restored_caps"] = fresh.controller.command_capabilities
	_finish()

func _negative(adapter: Adapter, command: Dictionary, result: Dictionary) -> void:
	var target: Boot = _app()
	if target == null:
		return
	var before: String = _model(target)
	var old_controller = target.controller
	var injected: Dictionary = command.duplicate(true)
	injected["gameplay_state"]["command_capabilities"] = [0, 1, 7]
	_check(not adapter.restore(target, injected).is_empty() and _model(target) == before,
		"extra saved capability fields cannot mutate live state or grant commands")
	var rebind_error: String = adapter.restore(target, result, func(app: Node) -> String:
		app.get("controller").command_capabilities.clear()
		return "injected command rebind failure")
	_check(rebind_error == "injected command rebind failure" and target.controller == old_controller
		and target.controller.command_capabilities == [0, 7] and _model(target) == before,
		"failed restore preserves the original controller and nonaliased trusted configuration")
	target.controller.command_capabilities.clear()
	var no_caps: String = _model(target)
	_check(not adapter.restore(target, command).is_empty() and _model(target) == no_caps,
		"missing authenticated capabilities reject before mutable apply")
	target.controller.command_capabilities.assign([0, 7])
	var frame_key: String = str(target.command_resources[7]["frames"].keys()[0])
	var old_image: Image = target.command_resources[7]["frames"][frame_key]["image"]
	var changed: Image = old_image.duplicate()
	changed.set_pixel(0, 0, Color.MAGENTA if changed.get_pixel(0, 0) != Color.MAGENTA else Color.CYAN)
	target.command_resources[7]["frames"][frame_key]["image"] = changed
	before = _model(target)
	_check(not adapter.restore(target, command).is_empty() and _model(target) == before,
		"command0 save identity binds unreferenced rest frame payloads in the preloaded map")
	target.command_resources[7]["frames"][frame_key]["image"] = old_image
	var reshaped_size: Vector2i = Vector2i(old_image.get_width() << 1, old_image.get_height() >> 1) \
		if old_image.get_height() % 2 == 0 else Vector2i(old_image.get_width() >> 1, old_image.get_height() << 1)
	var reshaped: Image = Image.create_from_data(reshaped_size.x, reshaped_size.y, old_image.has_mipmaps(),
		old_image.get_format(), old_image.get_data())
	_check(reshaped.get_data() == old_image.get_data() and reshaped.get_size() != old_image.get_size(),
		"negative image geometry fixture preserves the exact pixel bytes")
	target.command_resources[7]["frames"][frame_key]["image"] = reshaped
	before = _model(target)
	_check(not adapter.restore(target, command).is_empty() and _model(target) == before,
		"same-byte altered rest image geometry rejects before mutable apply")
	target.command_resources[7]["frames"][frame_key]["image"] = old_image
	var old_id: String = target.command_resources[7]["frames"][frame_key]["id"]
	target.command_resources[7]["frames"][frame_key]["id"] = target.background[0]["id"]
	_check(not adapter.capture(target, "command")["error"].is_empty(),
		"conflicting source ID payloads reject instead of selecting the last registered image")
	target.command_resources[7]["frames"][frame_key]["id"] = old_id
	var untrusted_rule: Dictionary = command.duplicate(true)
	untrusted_rule["gameplay_state"]["state"]["rule_state"]["resources"]["available_commands"] = [0, 1, 7]
	untrusted_rule["gameplay_state"]["state"]["rule_state"]["menu_cursor"] = 1
	_rehash(untrusted_rule)
	_check(adapter.restore(target, untrusted_rule).is_empty() and target.controller.command_capabilities == [0, 7],
		"saved legacy capability record cannot override trusted launch configuration")
	var events: Array[Session.ServiceEvent] = []
	var first: Session.FrameResult = target.session.step(InputFrame.new(0, 0x20), events)
	_check(first.committed, "negative capability probe uses the actual input latch")
	var pre_reject: String = target.session.state_hash()
	var rejected: Session.FrameResult = target.session.step(InputFrame.new(1), events)
	_check(not rejected.committed and target.session.state_hash() == pre_reject,
		"injected saved fallback command1 remains unavailable and rejection rolls back")

func _app() -> Boot:
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	var error: String = app._load_inputs()
	if error.is_empty(): error = app._reset()
	if not error.is_empty():
		_check(false, "actual command-map app preflight: " + error)
		app.canvas.free(); app.pad.free(); app.status_label.free(); app.detail_label.free(); app.free()
		return null
	apps.append(app)
	return app

func _step(app: Boot) -> void:
	var tick: int = app.session.clock.logic_iterations
	if tick < 75 and tick % 10 in [7, 1]:
		var key: InputEventKey = InputEventKey.new()
		key.physical_keycode = KEY_RIGHT
		key.pressed = tick % 10 == 7
		app.pad._input(key)
	if tick in [97, 103]:
		var key: InputEventKey = InputEventKey.new()
		key.physical_keycode = KEY_ENTER
		key.pressed = tick == 97
		app.pad._input(key)
	app._physics_process(1.0 / 60.0)

func _rest(app: Boot) -> Array:
	var hashes: Array = []
	for iteration: int in range(301):
		if app.stopped: break
		var before: int = app.trace.size()
		_step(app)
		if app.trace.size() != before + 1:
			_check(false, "actual rest continuation committed its expected step")
			break
		var row: Dictionary = app.trace.back()
		hashes.append([row["state_hash"], row["event_hash"]])
	return hashes

func _model(app: Boot) -> String:
	return JSON.stringify([app.state.projection(), app.session.rng.checkpoint(), app.session.clock.snapshot(),
		app.session.latched_input.snapshot(), app.pending_services, app.controller.command_capabilities,
		app.stopped, app.failure, app.trace, app.checkpoints, _render(app)])

func _render(app: Boot) -> String:
	var output: Array = []
	for command: Dictionary in app.staged_commands:
		var image: Image = command["image"]
		var hash: HashingContext = HashingContext.new()
		hash.start(HashingContext.HASH_SHA256)
		hash.update(image.get_data())
		output.append([command["id"], command["position"].x, command["position"].y,
			image.get_width(), image.get_height(), hash.finish().hex_encode()])
	return JSON.stringify(output)

func _rehash(snapshot: Dictionary) -> void:
	var session: Session = Session.new()
	var state: State = State.new()
	state.restore(snapshot["gameplay_state"]["state"])
	session.state = state
	session.rng.restore(snapshot["native_rng"])
	session.clock.restore(snapshot["native_clocks"])
	var latch: Dictionary = snapshot["gameplay_state"]["input_latch"]
	session.latched_input = InputFrame.new(latch["tick"], latch["held"], (latch["held"] & ~latch["pressed"]) | latch["released"])
	snapshot["gameplay_state"]["session_hash"] = session.state_hash()

func _check(ok: bool, message: String) -> void:
	if ok: checks.append(message)
	else: failures.append(message)

func _finish() -> void:
	report["checks"] = checks
	report["failures"] = failures
	report["status"] = "PASS" if failures.is_empty() else "FAIL"
	report["scope"] = "exact sealed command7 app plus save adapter delta; captured command/rest result; rest continuation beyond first result remains unsupported"
	var output: String = OS.get_environment("TOKIMEMO_COMMAND_SAVE_OUTPUT")
	if output.is_absolute_path():
		var file: FileAccess = FileAccess.open(output.path_join("command-media-save-proof.json"), FileAccess.WRITE)
		file.store_string(JSON.stringify(report, "\t")); file.close()
	for app: Boot in apps:
		app.canvas.free(); app.pad.free(); app.status_label.free(); app.detail_label.free(); app.free()
	if failures.is_empty():
		print("PASS: command-media save composition; %d checks; rest163/selected73/replay163 hash pairs" % checks.size())
		quit(0)
	else:
		for message: String in failures: push_error(message)
		quit(1)
