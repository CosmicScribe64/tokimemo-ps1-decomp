extends SceneTree
## Real canonical app join with externally supplied captured input and media.
## No original fixture or state value is embedded in this public test script.

const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Save = preload("res://services/save_service.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/school_day_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
var failures: Array[String] = []
var checks: Array[String] = []
var apps: Array[Boot] = []
var report: Dictionary = {}

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var versions: Dictionary = {"source_version": "687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3",
		"content_version": JSON.stringify([OS.get_environment("TOKIMEMO_BOUNDARY_SHA"), OS.get_environment("TOKIMEMO_TABLES_SHA"),
			FileAccess.get_sha256(OS.get_environment("TOKIMEMO_MEDIA_PACK").path_join("manifest.json"))]).sha256_text(),
		"behavior_version": "4d9f2a5cb57b6d3f10163552be9a3d2838d10a1a"}
	var adapter: Adapter = Adapter.new(versions)
	var source: Boot = _app()
	if source == null:
		_finish()
		return
	var output: String = OS.get_environment("TOKIMEMO_SAVE_OUTPUT")
	if not output.is_absolute_path():
		_check(false, "explicit absolute ignored output root is required")
		_finish()
		return
	var saves: Save = Save.new(output.path_join("saves"))
	var command: Dictionary = adapter.capture(source, "command")
	_check(command["error"].is_empty(), "captured original command boundary is saveable")
	if not command["error"].is_empty():
		_finish()
		return
	_check(saves.save_slot("route", command["snapshot"])["error"].is_empty(), "actual command snapshot publishes")
	var loaded_command: Dictionary = saves.load_slot("route")
	var resumed_command: Boot = _app()
	if resumed_command == null or not loaded_command["error"].is_empty():
		_check(false, "fresh app and saved command load succeed")
		_finish()
		return
	_check(adapter.restore(resumed_command, loaded_command["snapshot"]).is_empty(), "actual command restores into fresh app")
	_check(_snapshot_text(adapter, resumed_command, "command") == JSON.stringify(command["snapshot"]), "complete command-owned records and insertion order round-trip")
	var uninterrupted: Array = _advance(source, 80)
	var resumed: Array = _advance(resumed_command, 80)
	_check(uninterrupted.size() == 80 and resumed == uninterrupted, "all 80 command-to-first-result state/event hash pairs equal uninterrupted")
	_check(source.state.game_mode == 0x31 and source.state.rule_state["day_event_index"] == 1 and source.stopped,
		"actual app pauses at its first result")
	var first_result: Dictionary = adapter.capture(source, "first_result")
	_check(first_result["error"].is_empty(), "actual first-result boundary is saveable")
	if not first_result["error"].is_empty():
		_finish()
		return
	_check(saves.save_slot("route", first_result["snapshot"])["error"].is_empty(), "actual first result atomically replaces command save")
	var previous: Dictionary = saves.load_slot("route", true)
	_check(previous["error"].is_empty() and JSON.stringify(previous["snapshot"]) == JSON.stringify(command["snapshot"]),
		"previous backup preserves exact captured command boundary")
	var loaded_result: Dictionary = saves.load_slot("route")
	var resumed_result: Boot = _app()
	if resumed_result == null or not loaded_result["error"].is_empty():
		_check(false, "fresh app and saved result load succeed")
		_finish()
		return
	_check(adapter.restore(resumed_result, loaded_result["snapshot"]).is_empty(), "first result restores into fresh preloaded app")
	_check(_snapshot_text(adapter, resumed_result, "first_result") == JSON.stringify(first_result["snapshot"]),
		"all result rule/actor/controller/RNG/clock/input/service records round-trip")
	var isolated_before: String = _snapshot_text(adapter, resumed_result, "first_result")
	loaded_result["snapshot"]["gameplay_state"]["state"]["rule_state"]["params"][0] = -1
	loaded_result["snapshot"]["native_rng"]["trace"][0]["before"] = -1
	loaded_result["snapshot"]["gameplay_state"]["host"]["initial_params"][0] = -1
	_check(_snapshot_text(adapter, resumed_result, "first_result") == isolated_before,
		"restored live records do not alias caller-held decoded save data")
	# Explicit development resume of the app's paused result; the session uses
	# the canonical ordinary-week controller, media service and native draw path.
	source.stopped = false
	resumed_result.stopped = false
	var next_uninterrupted: Array = _advance(source, 40)
	var next_resumed: Array = _advance(resumed_result, 40)
	_check(next_uninterrupted.size() == 40 and next_resumed == next_uninterrupted,
		"all 40 first-result continuation state/event hashes equal uninterrupted")
	_check(source.state.rule_state["day_event_index"] > 1 and not source.state.rule_state["actor_state"].is_empty(),
		"first-result continuation executes canonical day-end and next activity")
	var target: Boot = _app()
	if target != null:
		_negative_restores(adapter, target, command["snapshot"], first_result["snapshot"])
		_step_rollback(adapter, target, command["snapshot"])
	var scripted_source: Boot = _app()
	var scripted_target: Boot = _app()
	if scripted_source != null and scripted_target != null:
		scripted_source.smoke = true
		var scripted: Dictionary = adapter.capture(scripted_source, "command")
		_check(scripted["error"].is_empty() and adapter.restore(scripted_target, scripted["snapshot"]).is_empty()
			and scripted_target.smoke, "scripted-input controller policy is captured and restored")
		_check(_advance(scripted_source, 20, false) == _advance(scripted_target, 20, false),
			"scripted-input continuation preserves automatic command-confirmation hashes")
	var week_source: Boot = _app()
	var week_target: Boot = _app()
	if week_source != null and week_target != null:
		week_source.continue_week = true
		var week_command: Dictionary = adapter.capture(week_source, "command")
		_check(week_command["error"].is_empty() and saves.save_slot("week", week_command["snapshot"])["error"].is_empty(),
			"default-week command snapshot publishes through the exact ordered codec")
		var loaded_week: Dictionary = saves.load_slot("week")
		_check(loaded_week["error"].is_empty() and adapter.restore(week_target, loaded_week["snapshot"]).is_empty()
			and week_target.continue_week, "default ordinary-week controller policy is preserved")
		var week_prefix: Array = _advance(week_source, 80)
		var resumed_prefix: Array = _advance(week_target, 80)
		var week_result: Dictionary = adapter.capture(week_source, "first_result")
		_check(week_result["error"].is_empty() and not week_source.checkpoints.is_empty()
			and saves.save_slot("week_result", week_result["snapshot"])["error"].is_empty(),
			"default-week first result saves its nonempty native checkpoint ledger")
		var result_target: Boot = _app()
		var loaded_week_result: Dictionary = saves.load_slot("week_result")
		if result_target == null or not loaded_week_result["error"].is_empty():
			_check(false, "default-week result target/slot loads")
			_finish()
			return
		_check(adapter.restore(result_target, loaded_week_result["snapshot"]).is_empty(),
			"default-week first result restores its controller/checkpoint policy")
		var week_suffix: Array = _complete_week(week_source)
		var week_uninterrupted: Array = week_prefix + week_suffix
		var week_resumed: Array = resumed_prefix + _complete_week(week_target)
		var result_suffix: Array = _complete_week(result_target)
		_check(week_uninterrupted.size() == 243 and week_resumed == week_uninterrupted,
			"all 243 current default-week state/event hash pairs equal after command reload")
		_check(result_suffix.size() == 163 and result_suffix == week_suffix,
			"all 163 default-week continuation hashes equal after first-result reload")
		_check(week_source.checkpoints == week_target.checkpoints and week_source.checkpoints.size() == 15
			and result_target.checkpoints == week_source.checkpoints and week_source.supported_stop == week_target.supported_stop
			and result_target.supported_stop == week_source.supported_stop,
			"native checkpoint/controller ledgers and explicit unsupported stop agree after reload")
		report["default_week_continuation"] = week_uninterrupted
		report["default_week_first_result_continuation"] = result_suffix
	report["schema"] = "tokimemo.actual-save-resume-proof.v1"
	report["base_revision"] = versions["behavior_version"]
	report["versions"] = versions
	report["command_continuation"] = uninterrupted
	report["first_result_continuation"] = next_uninterrupted
	report["command_snapshot_sha256"] = JSON.stringify(command["snapshot"]).sha256_text()
	report["first_result_snapshot_sha256"] = JSON.stringify(first_result["snapshot"]).sha256_text()
	report["scope"] = "actual captured-boundary app/controller/media continuation; existing preloaded completion policy and OpenBIOS RNG profile; no native CD/audio timing parity"
	_finish()

func _app() -> Boot:
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.continue_week = OS.get_environment("TOKIMEMO_PLAYABLE_FIRST_RESULT_ONLY") != "1"
	var error: String = app._load_inputs()
	if error.is_empty():
		error = app._reset()
	if not error.is_empty():
		_check(false, "actual app preflight: " + error)
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
		return null
	apps.append(app)
	return app

func _advance(app: Boot, count: int, feed_recipe: bool = true) -> Array:
	var hashes: Array = []
	for index: int in range(count):
		var tick: int = app.session.clock.logic_iterations
		if feed_recipe and tick in [17, 23]:
			var key: InputEventKey = InputEventKey.new()
			key.physical_keycode = KEY_ENTER
			key.pressed = tick == 17
			app.pad._input(key)
		var before: int = app.trace.size()
		app._physics_process(1.0 / 60.0)
		if app.trace.size() != before + 1:
			_check(false, "actual app failed to commit tick %d: %s" % [tick, app.failure])
			break
		var row: Dictionary = app.trace.back()
		hashes.append([row["state_hash"], row["event_hash"]])
	return hashes

func _complete_week(app: Boot) -> Array:
	var hashes: Array = []
	for iteration: int in range(601):
		var tick: int = app.session.clock.logic_iterations
		if tick in [17, 23]:
			var key: InputEventKey = InputEventKey.new()
			key.physical_keycode = KEY_ENTER
			key.pressed = tick == 17
			app.pad._input(key)
		var before: int = app.trace.size()
		app._physics_process(1.0 / 60.0)
		if app.trace.size() == before + 1:
			var row: Dictionary = app.trace.back()
			hashes.append([row["state_hash"], row["event_hash"]])
		if app.stopped:
			break
	_check(app.failure.is_empty() and not app.supported_stop.is_empty()
		and app.supported_stop.get("before_hash") == app.supported_stop.get("after_hash"),
		"current default-week app reaches its explicit holiday refusal without mutation")
	return hashes

func _negative_restores(adapter: Adapter, target: Boot, command: Dictionary, result: Dictionary) -> void:
	var mutations: Array[Callable] = [
		func(s: Dictionary) -> void: s["source_version"] = "wrong",
		func(s: Dictionary) -> void: s["content_version"] = "wrong",
		func(s: Dictionary) -> void: s["behavior_version"] = "wrong",
		func(s: Dictionary) -> void: s["gameplay_state"]["state"]["frame_count"] = "bad",
		func(s: Dictionary) -> void: s["native_clocks"]["phase"] = 1000000000,
		func(s: Dictionary) -> void: s["native_rng"]["state"] = -1,
		func(s: Dictionary) -> void: s["native_rng"]["draw_count"] += 1,
		func(s: Dictionary) -> void: s["gameplay_state"]["input_latch"]["tick"] += 1,
		func(s: Dictionary) -> void: s["gameplay_state"]["input_latch"]["pressed"] = 1,
		func(s: Dictionary) -> void: s["gameplay_state"]["pad"]["previous"] = 1,
		func(s: Dictionary) -> void: s["queued_service_events"].append({"service": "invented"}),
		func(s: Dictionary) -> void: s["gameplay_state"]["draw"].append({"id": "missing", "x": 0, "y": 0}),
		func(s: Dictionary) -> void: s["gameplay_state"]["host"]["focus_paused"] = true,
		func(s: Dictionary) -> void: s["gameplay_state"]["controller_identity"] = "wrong",
		func(s: Dictionary) -> void: s["gameplay_state"]["host"]["trace"].append({"object": RefCounted.new()}),
	]
	var before: String = _snapshot_text(adapter, target, "command")
	var original_session: Session = target.session
	target.focus_paused = true
	target.playback_paused = true
	target._window_focused = false
	target._resume_input_pending = true
	var unfocused: String = _simulation_text(target)
	_check(not adapter.restore(target, command).is_empty() and target.session == original_session
		and _simulation_text(target) == unfocused and target.focus_paused and target.playback_paused
		and not target._window_focused and target._resume_input_pending,
		"load cannot overwrite the live external focus/resume policy")
	target.focus_paused = false
	target.playback_paused = false
	target._window_focused = true
	target._resume_input_pending = false
	target.video_capture_busy = true
	_check(not adapter.restore(target, command).is_empty() and target.session == original_session,
		"load rejects an in-flight render capture")
	target.video_capture_busy = false
	target.session.rng.begin_step(func(_reason: String) -> void: pass)
	_check(not adapter.restore(target, command).is_empty() and target.session == original_session,
		"load rejects an active session step")
	target.session.rng.end_step()
	for mutate: Callable in mutations:
		var invalid: Dictionary = command.duplicate(true)
		mutate.call(invalid)
		_check(not adapter.restore(target, invalid).is_empty() and target.session == original_session
			and _snapshot_text(adapter, target, "command") == before, "invalid saved record rejected without mutating live app")
	var bad_actor: Dictionary = result.duplicate(true)
	bad_actor["gameplay_state"]["state"]["rule_state"]["actor_state"] = {
		"96": {"id": 96, "sequence_slot": 0, "x": -80, "y": -64, "flags": 0,
			"draw_flags": 0x84, "quad_flags": 0x01000000,
			"sequence": {"frame": 999999, "counter": 0, "active": false, "period": 1, "last_iteration": 1}}}
	_rehash_state(bad_actor)
	_check(not adapter.restore(target, bad_actor).is_empty() and _snapshot_text(adapter, target, "command") == before,
		"invalid saved actor state cannot mutate live command session")
	var minimum_coordinate: Dictionary = command.duplicate(true)
	minimum_coordinate["gameplay_state"]["draw"].append({"id": target.background[0]["id"],
		"x": -9223372036854775807 - 1, "y": 0})
	_check(not adapter.restore(target, minimum_coordinate).is_empty() and _snapshot_text(adapter, target, "command") == before,
		"signed-minimum coordinate is rejected before Vector2i narrowing")
	var failed: String = adapter.restore(target, result, func(app: Node) -> String:
		app.get("session").state.rule_state["params"][0] = -1
		app.get("session").rng.next("injected-rebind-failure")
		app.get("session").clock.pending_steps += 7
		return "injected renderer rebind failure")
	_check(failed == "injected renderer rebind failure" and target.session == original_session
		and _snapshot_text(adapter, target, "command") == before,
		"failed post-apply rebind rolls back full original state/RNG/clocks/input/queues/controller")

func _step_rollback(adapter: Adapter, target: Boot, command: Dictionary) -> void:
	_check(adapter.restore(target, command).is_empty(), "command loads before rollback probe")
	for iteration: int in range(70):
		if not target.state.rule_state["actor_state"].is_empty():
			break
		_advance(target, 1)
	_check(not target.state.rule_state["actor_state"].is_empty(), "rollback probe reaches an actual active actor draw")
	var before: String = _simulation_text(target)
	var original_cd: Callable = target.session.cd_logic
	target.session.cd_logic = func(state, rng, result) -> void:
		state.rule_state["params"][0] = -1
		rng.next("injected-service-failure")
		target.session.clock.pending_steps += 9
		result.error = "injected service failure after restored mode/draw"
	var events: Array[Session.ServiceEvent] = []
	for event: Dictionary in target.pending_services:
		events.append(Session.ServiceEvent.new(target.session.clock.logic_iterations, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, event["service"], event["payload"]))
	var failed: Session.FrameResult = target.session.step(InputFrame.new(target.session.clock.logic_iterations), events)
	target.session.cd_logic = original_cd
	_check(not failed.committed and _simulation_text(target) == before,
		"restored session rolls back actual actor advancement/nested rules/RNG trace/clocks/input/services after failure")

func _snapshot_text(adapter: Adapter, app: Boot, boundary: String) -> String:
	var result: Dictionary = adapter.capture(app, boundary)
	return JSON.stringify(result.get("snapshot", {"error": result["error"]}))

func _simulation_text(app: Boot) -> String:
	return JSON.stringify([app.state.projection(), app.session.rng.checkpoint(), app.session.clock.snapshot(),
		app.session.latched_input.snapshot(), app.pending_services, app.pad.get("_devices"), app.pad.get("_previous")])

func _rehash_state(snapshot: Dictionary) -> void:
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
	if ok:
		checks.append(message)
	else:
		failures.append(message)

func _finish() -> void:
	report["checks"] = checks
	report["failures"] = failures
	report["status"] = "PASS" if failures.is_empty() else "FAIL"
	var output: String = OS.get_environment("TOKIMEMO_SAVE_OUTPUT")
	if output.is_absolute_path():
		DirAccess.make_dir_recursive_absolute(output)
		var file: FileAccess = FileAccess.open(output.path_join("actual-save-resume-proof.json"), FileAccess.WRITE)
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
		print("PASS: actual command/result save-resume; %d checks; 80+40+243+163 continuation hash pairs" % checks.size())
		quit(0)
	else:
		for message: String in failures:
			push_error(message)
		quit(1)
