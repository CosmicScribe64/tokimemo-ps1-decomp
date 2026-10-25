extends SceneTree
## T-29008: real external route integration with render-frequency independence.
## Original fixture is ignored and explicitly supplied, never a public fixture.

const Boot = preload("res://app/playable_boot.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")

func _initialize() -> void:
	var reference: Array = []
	for rendered: int in [0, 1, 3]:
		var app: Boot = Boot.new()
		app.canvas = Canvas.new()
		app.pad = Pad.new()
		app.status_label = Label.new()
		app.detail_label = Label.new()
		var error: String = app._load_inputs()
		if error.is_empty():
			error = app._reset()
		if not error.is_empty():
			_fail(error)
			return
		var hashes: Array = []
		var entry_checked: bool = false
		for tick: int in range(301):
			if tick in [17, 23]:
				var key := InputEventKey.new()
				key.physical_keycode = KEY_ENTER
				key.pressed = tick == 17
				app.pad._input(key)
			var events: Array[Session.ServiceEvent] = []
			for event: Dictionary in app.pending_services:
				events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
					Session.ServiceKind.COMPLETION, event["service"], event["payload"]))
			app.staged_commands.clear()
			app.staged_services.clear()
			app.session.clock.rendered_frames += rendered
			var result: Session.FrameResult = app.session.step(app.pad.sample(tick), events)
			if not result.committed:
				_fail(result.error)
				return
			app.pending_services = app.staged_services.duplicate(true)
			hashes.append([result.state_hash, result.event_hash])
			if app.state.game_mode == 0x40 and not entry_checked:
				entry_checked = true
				var expected: Dictionary = app.boundary["expected"]["0"]
				if app.state.frame_count != expected["native_frame_count"] or [app.state.game_mode_timer, app.state.mode_stage_timer, app.state.mode_step_timer] != expected["native_mode_timers"]:
					_fail("original command-to-activity frame/timer phase differs")
					return
			if app.state.game_mode == 0x31 and app.state.rule_state["day_event_index"] == 1:
				break
		if app.state.game_mode != 0x31 or app.state.rule_state["day_event_index"] != 1:
			_fail("composed actual route did not reach first return")
			return
		if rendered == 0:
			reference = hashes
		elif reference != hashes:
			_fail("rendered-frame counts changed actual composed state/event hashes")
			return
		app.canvas.free()
		app.pad.free()
		app.status_label.free()
		app.detail_label.free()
		app.free()
	print("PASS: actual app composition/native command-entry phases and all state/event hashes with 0/1/3 rendered frames per step")
	quit(0)

func _fail(error: String) -> void:
	push_error(error)
	quit(1)
