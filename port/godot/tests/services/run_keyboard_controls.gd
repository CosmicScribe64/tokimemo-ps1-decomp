extends SceneTree
## Rendered X11 keyboard test. Keys arrive through the window system; no test
## InputEvent is constructed here. Host stepping makes edge timing observable.
const Boot = preload("res://app/playable_boot.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
var app: Boot
var phase: String = "boot"
var output: String
var checks: Array[String] = []
var failures: Array[String] = []
var saved_model: String = ""
var saved_tick: int = -1
var done: bool = false

func _initialize() -> void:
	output = OS.get_environment("TOKIMEMO_KEYBOARD_OUTPUT")
	call_deferred("_start")

func _start() -> void:
	if not output.is_absolute_path() or not OS.get_user_data_dir().begins_with("/userdata/"):
		push_error("explicit persistent user data and ignored output required")
		quit(1)
		return
	app = (load("res://app/playable.tscn") as PackedScene).instantiate() as Boot
	root.add_child(app)
	process_frame.connect(_tick)
	_status()

func _tick() -> void:
	if done or app == null:
		return
	if phase == "boot":
		if app.session != null and app.save_controls.storage != null and app.is_physics_processing() \
			and not app.focus_paused and not app._resume_input_pending:
			app.set_physics_process(false)
			for suffix: String in ["", ".bak", ".tmp", ".bak.tmp"]:
				var path: String = app.save_controls.storage.storage_root.path_join("quick.json" + suffix)
				if FileAccess.file_exists(path): DirAccess.remove_absolute(path)
			_set_phase("ready_f5")
	elif phase == "ready_f5" and app.save_controls.pending == Controls.Request.SAVE:
		saved_model = _model()
		saved_tick = app.session.clock.logic_iterations
		_check(app.save_controls.storage_calls == 0, "X11 F5 reaches the queue before any save I/O")
		app._physics_process(1.0 / 60.0)
		_check(app.save_controls.completed_requests == 1 and app.save_controls.last_error.is_empty()
			and _model() == saved_model, "X11 F5 writes at the command boundary without gameplay mutation")
		_set_phase("right_down")
	elif phase == "right_down" and (_devices() & 0x2000) != 0:
		app._physics_process(1.0 / 60.0)
		app._physics_process(1.0 / 60.0)
		_check(app.state.rule_state["menu_cursor"] == 1, "X11 Right passes through native input latching")
		_set_phase("right_up")
	elif phase == "right_up" and _devices() == 0:
		app._physics_process(1.0 / 60.0)
		app._physics_process(1.0 / 60.0)
		_set_phase("enter_down")
	elif phase == "enter_down" and (_devices() & 0x20) != 0:
		app._physics_process(1.0 / 60.0)
		app._physics_process(1.0 / 60.0)
		_check(app.stopped and not app.failure.is_empty() and app.session.latched_input.held != 0,
			"X11 Return reaches a genuine unsupported-command stop and retained native latch")
		_set_phase("f9_held")
	elif phase == "f9_held" and app.save_controls.pending == Controls.Request.LOAD:
		var before: String = _model()
		app._physics_process(1.0 / 60.0)
		_check(app.save_controls.storage_calls == 1 and app.save_controls.last_error.contains("Release")
			and _model() == before, "X11 F9 cannot load while Return remains physically held")
		_set_phase("enter_up")
	elif phase == "enter_up" and _devices() == 0:
		_check(app.pad.get("_previous") != 0 and app.session.latched_input.held != 0,
			"X11 Return release leaves the rejected step's committed latch stale")
		_set_phase("f9_released")
	elif phase == "f9_released" and app.save_controls.pending == Controls.Request.LOAD:
		app._physics_process(1.0 / 60.0)
		_check(app.save_controls.last_error.is_empty() and not app.stopped and app.failure.is_empty()
			and app.session.clock.logic_iterations == saved_tick and _model() == saved_model,
			"X11 F9 recovers after release with the exact saved model and no extra tick")
		_finish("")

func _devices() -> int:
	var held: int = 0
	for value: int in app.pad.get("_devices").values(): held |= value
	return held

func _model() -> String:
	return JSON.stringify([app.state.projection(), app.session.rng.checkpoint(), app.session.clock.snapshot(),
		app.session.latched_input.snapshot(), app.pad.get("_devices"), app.pad.get("_previous"),
		app.pending_services, app.trace, app.checkpoints, app.stopped, app.failure, app.controller.command_capabilities])

func _set_phase(value: String) -> void:
	phase = value
	_status()

func _status() -> void:
	var path: String = output.path_join("keyboard-status.json")
	var file: FileAccess = FileAccess.open(path + ".tmp", FileAccess.WRITE)
	file.store_string(JSON.stringify({"phase": phase, "failed": not failures.is_empty()}))
	file.close()
	DirAccess.rename_absolute(path + ".tmp", path)

func _check(ok: bool, text: String) -> void:
	if ok: checks.append(text)
	else: failures.append(text)

func _finish(error: String) -> void:
	if done: return
	done = true
	if not error.is_empty(): failures.append(error)
	phase = "passed" if failures.is_empty() else "failed"
	_status()
	var file: FileAccess = FileAccess.open(output.path_join("keyboard-proof.json"), FileAccess.WRITE)
	file.store_string(JSON.stringify({"status": "PASS" if failures.is_empty() else "FAIL", "checks": checks,
		"failures": failures, "scope": "real X11 key-event delivery through Godot/app/NativePad; controlled logical stepping; no human hardware claim",
		"saved_tick_equals_restored": app != null and app.session.clock.logic_iterations == saved_tick}, "\t"))
	file.close()
	if app != null:
		await RenderingServer.frame_post_draw
		root.get_texture().get_image().save_png(output.path_join("keyboard-recovered.png"))
	if failures.is_empty(): print("PASS: X11 save/stop/release/F9 recovery; %d checks" % checks.size())
	else:
		for message: String in failures: push_error(message)
	quit(0 if failures.is_empty() else 1)
