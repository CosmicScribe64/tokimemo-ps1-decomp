extends SceneTree
## Invented focus cases; actual original-route proof remains a separate test.

const Boot = preload("res://app/playable_boot.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Pad = preload("res://ui/native_pad.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Fixture = preload("res://tests/rules/run_rule_tests.gd")

var failures: Array[String] = []
var checks: int = 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func _mode(base: State, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	if (input.pressed & 0x2000) != 0:
		base.rule_state["menu_cursor"] += 1
	rng.next("synthetic focus regression draw")
	result.presentation.append({"kind": "synthetic_input", "held": input.held, "pressed": input.pressed})

func _service(event: Session.ServiceEvent, _base: State, _rng: Rng, result: Session.FrameResult) -> void:
	result.presentation.append({"kind": "synthetic_service", "value": event.payload["value"]})

func app_for_test() -> Boot:
	var app: Boot = Boot.new()
	app.state = Fixture.new().synthetic_state()
	app.session = Session.new()
	app.session.state = app.state
	app.session.rng.configure(Rng.OPENBIOS_PROFILE, 123)
	app.session.router.bind_mode(0x34, _mode)
	app.session.draw_logic = app._native_draw
	app.session.cd_logic = app._bounded_cd
	app.session.audio_logic = app._bounded_audio
	app.session.service_completion = _service
	app.pad = Pad.new()
	app.canvas = Canvas.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.initial_params = app.state.rule_state["params"].duplicate()
	app.pending_services = [{"service": "focus.synthetic", "payload": {"value": 17}}]
	return app

func snapshot(app: Boot) -> Dictionary:
	return {"state": app.state.projection(), "hash": app.session.state_hash(),
		"clock": app.session.clock.snapshot(), "rng": app.session.rng.checkpoint(),
		"input": app.session.latched_input.snapshot(), "trace": app.trace.duplicate(true),
		"pending": app.pending_services.duplicate(true), "staged": app.staged_services.duplicate(true)}

func key(pressed: bool, code: int = KEY_RIGHT) -> InputEventKey:
	var event: InputEventKey = InputEventKey.new()
	event.physical_keycode = code
	event.pressed = pressed
	return event

func dispose(app: Boot) -> void:
	app.pad.free()
	app.canvas.free()
	app.status_label.free()
	app.detail_label.free()
	app.free()

func _initialize() -> void:
	for order: Array in [[Node.NOTIFICATION_WM_WINDOW_FOCUS_OUT, Node.NOTIFICATION_APPLICATION_FOCUS_OUT,
		Node.NOTIFICATION_WM_WINDOW_FOCUS_IN, Node.NOTIFICATION_APPLICATION_FOCUS_IN],
		[Node.NOTIFICATION_APPLICATION_FOCUS_OUT, Node.NOTIFICATION_WM_WINDOW_FOCUS_OUT,
		Node.NOTIFICATION_APPLICATION_FOCUS_IN, Node.NOTIFICATION_WM_WINDOW_FOCUS_IN]]:
		var app: Boot = app_for_test()
		var reference: Boot = app_for_test()
		var playback: Array[bool] = []
		app.playback_pause_changed.connect(func(paused: bool) -> void: playback.append(paused))
		app.pad._input(key(true))
		app.session.latched_input = InputFrame.new(-1, 0x2000)
		var before: Dictionary = snapshot(app)
		app.notification(order[0])
		app.notification(order[1])
		check(app.focus_paused and playback == [true], "both focus-out notifications pause playback once")
		check(app.pad.sample(0).held == 0, "focus-out clears stale held device edges")
		check(snapshot(app) == before, "notifications never mutate session state, input latch, clock, RNG or queued events")
		for index: int in range(8):
			app._physics_process(1.0)
			app._process(1.0)
		check(snapshot(app) == before, "paused physics/render calls freeze state, clocks, RNG and events")
		app.notification(order[2])
		app._physics_process(1.0)
		check(app.focus_paused and snapshot(app) == before and playback == [true], "partial focus return cannot resume while the other surface is unfocused")
		app.notification(order[3])
		check(not app.focus_paused and app.playback_paused and playback == [true] and snapshot(app) == before, "full focus return keeps playback paused until the logical resume boundary")
		# Simulate a stale buffered device event even though automatic input is disabled.
		app.pad._input(key(true))
		app._physics_process(1.0)
		reference._physics_process(1.0)
		check(not app.playback_paused and playback == [true, false], "playback resumes once at the same neutral logical boundary")
		check(app.state.rule_state["menu_cursor"] == 0 and app.session.latched_input.held == 0, "neutral resume clears both old native latch and buffered held input")
		check(snapshot(app) == snapshot(reference), "resume matches neutral reference state/clock/RNG/event hashes and consumes each queued completion once")
		app.pad._input(key(true))
		reference.pad._input(key(true))
		app._physics_process(1.0)
		reference._physics_process(1.0)
		check(app.state.rule_state["menu_cursor"] == 0, "fresh next press uses the native following-iteration latch")
		app._physics_process(1.0)
		reference._physics_process(1.0)
		check(app.state.rule_state["menu_cursor"] == 1 and snapshot(app) == snapshot(reference), "fresh press advances once with identical uninterrupted state/event hashes")
		app._physics_process(1.0)
		reference._physics_process(1.0)
		check(app.state.rule_state["menu_cursor"] == 1 and snapshot(app) == snapshot(reference), "continued held input creates no phantom repeated edge")
		app.stopped = true
		before = snapshot(app)
		app._input(key(true, KEY_R))
		check(app._replay_requested and snapshot(app) == before, "replay input callback requests work without writing progression")
		dispose(app)
		dispose(reference)
	var proof: Dictionary = {"checks": checks, "failures": failures,
		"scope": "synthetic actual Boot notifications/fixed-step input; muted playback pause signal contract only"}
	var path: String = OS.get_environment("TOKIMEMO_FOCUS_PROOF")
	if not path.is_empty():
		var output: FileAccess = FileAccess.open(path, FileAccess.WRITE)
		output.store_string(JSON.stringify(proof, "\t"))
		output.close()
	for failure: String in failures:
		push_error(failure)
	print(JSON.stringify(proof))
	quit(0 if failures.is_empty() else 1)
