extends SceneTree
## T-29008: external original-input command0/week and bounded command7 join.

const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const CoreState = preload("res://core/game_state.gd")
const Rng = preload("res://core/native_rng.gd")

var failures: Array[String] = []
var reports: Dictionary = {}
var checks: int = 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func make_app() -> Boot:
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	var error: String = app._load_inputs()
	if error.is_empty():
		error = app._reset()
	check(error.is_empty(), "preflight/reset: " + error)
	return app

func dispose(app: Boot) -> void:
	app.canvas.free()
	app.pad.free()
	app.status_label.free()
	app.detail_label.free()
	app.free()

static func device_recipe(app: Boot, command: int, tick: int) -> void:
	if command == 7 and tick < 75 and tick % 10 in [7, 1]:
		var right: InputEventKey = InputEventKey.new()
		right.physical_keycode = KEY_RIGHT
		right.pressed = tick % 10 == 7
		app.pad._input(right)
	var confirm_tick: int = 17 if command == 0 else 97
	if tick in [confirm_tick, confirm_tick + 6]:
		var confirm: InputEventKey = InputEventKey.new()
		confirm.physical_keycode = KEY_ENTER
		confirm.pressed = tick == confirm_tick
		app.pad._input(confirm)

func _initialize() -> void:
	var baseline: bool = OS.get_environment("TOKIMEMO_COMMAND7_BASELINE_ONLY") == "1"
	for command: int in ([0] if baseline else [0, 7]):
		var reference: Array = []
		for rendered: int in [0, 1, 3]:
			var app: Boot = make_app()
			var entry: Dictionary = {}
			for tick: int in range(601):
				device_recipe(app, command, tick)
				app.session.clock.rendered_frames += rendered
				app._physics_process(1.0 / 60.0)
				if entry.is_empty() and app.state.game_mode == 0x40 and app.state.mode_step == 0 and app.state.substep == 0:
					entry = {"state": app.state.projection(), "rng": app.session.rng.checkpoint()}
				if app.stopped:
					break
			check(app.stopped and app.failure.is_empty(), "command%d must stop at a supported boundary" % command)
			check(not entry.is_empty(), "original command%d reaches actual activity entry" % command)
			var expected: Dictionary = app.boundary["expected"][str(command)]
			if not entry.is_empty():
				check(entry["state"]["frame_count"] == expected["native_frame_count"], "command%d native entry absolute frame" % command)
				check([entry["state"]["game_mode_timer"], entry["state"]["mode_stage_timer"], entry["state"]["mode_step_timer"]] == expected["native_mode_timers"], "command%d native entry timers" % command)
			var hashes: Array = []
			for row: Dictionary in app.trace:
				hashes.append([row["state_hash"], row["event_hash"]])
			if rendered == 0:
				reference = hashes
				reports[str(command)] = {"entry": entry, "final_state": app.state.projection(),
					"rng": app.session.rng.checkpoint(), "trace": app.trace.duplicate(true),
					"checkpoints": app.checkpoints.duplicate(true), "supported_stop": app.supported_stop.duplicate(true)}
			else:
				check(reference == hashes, "command%d state/event hashes independent of rendered-frame schedule" % command)
			if command == 0:
				check(app.checkpoints.size() == 15 and not app.supported_stop.is_empty(), "command0 keeps fifteen ordinary-week checkpoints")
			else:
				check(app.state.game_mode == 0x31 and app.state.rule_state["day_event_index"] == 1 and app.state.rule_state["day"] == 5, "rest always stops at first result even when week continuation was requested")
				check(app.checkpoints.is_empty() and app.supported_stop.is_empty(), "rest claims no ordinary-week continuation")
				var frames: Dictionary = app.get("command_resources")
				var overlap: Array = []
				for frame_index: String in frames[0]["frames"]:
					if frames[7]["frames"].has(frame_index):
						overlap.append(frame_index)
						check(frames[0]["frames"][frame_index]["id"] != frames[7]["frames"][frame_index]["id"], "overlapping frame indices retain distinct source IDs")
				check(not overlap.is_empty(), "fixture exercises overlapping source frame indices")
				var ids: Array = []
				for draw: Dictionary in app.canvas._commands:
					ids.append(draw["id"])
				var backgrounds: Dictionary = app.get("command_backgrounds")
				check(ids == [backgrounds[7][0]["id"], backgrounds[7][1]["id"]], "rest result retains its actual validated background")
			dispose(app)
	if not baseline:
		for mutation: String in ["request", "command", "day_event_index", "background_lba", "sd_set_id", "sd_lba", "sectors", "rest_event", "sequence", "late_request"]:
			reject_request(mutation)
	var proof: Dictionary = {"checks": checks, "failures": failures, "commands": reports,
		"render_schedules": [0, 1, 3], "baseline_only": baseline}
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_COMMAND7_PROOF"), FileAccess.WRITE)
	output.store_string(JSON.stringify(proof, "\t"))
	output.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "commands": reports.keys(), "render_schedules": [0, 1, 3]}))
	quit(0 if failures.is_empty() else 1)

func reject_request(mutation: String) -> void:
	var app: Boot = make_app()
	# Use the real device recipe and committed route to the resource/actor phase.
	for tick: int in range(301):
		device_recipe(app, 7, tick)
		if app.state.game_mode == 0x40 and app.state.mode_stage == 0 and app.state.substep == 0 and app.state.mode_step == (2 if mutation == "sequence" else 0):
			break
		app._physics_process(1.0 / 60.0)
	if mutation == "rest_event":
		app.state.rule_state["day_events"][0][3] = 0
	app.session.draw_logic = func(base: CoreState, rng: Rng, result: Session.FrameResult) -> void:
		var late_request: Dictionary = {}
		for intent: Dictionary in result.presentation:
			if intent.get("kind") == "school_day_load" and mutation not in ["rest_event", "sequence", "late_request"]:
				intent[mutation] = "other" if mutation == "request" else int(intent[mutation]) + 1
			if intent.get("kind") == "school_day_load" and mutation == "late_request":
				late_request = intent.duplicate(true)
				late_request["sd_lba"] += 1
			if intent.get("kind") == "school_day_actors" and mutation == "sequence":
				intent["actors"][0]["sequence_slot"] = 1
		if not late_request.is_empty():
			result.presentation.append(late_request)
		app._native_draw(base, rng, result)
	var before: String = app.session.state_hash()
	var before_pending: Array = app.pending_services.duplicate(true)
	var before_commands: Array = app.canvas._commands.duplicate(true)
	var before_textures: Array = app.canvas._textures.keys()
	app.staged_commands.clear()
	app.staged_services.clear()
	var events: Array[Session.ServiceEvent] = []
	var tick: int = app.session.clock.logic_iterations
	for descriptor: Dictionary in app.pending_services:
		events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, descriptor["service"], descriptor["payload"]))
	var result: Session.FrameResult = app.session.step(InputFrame.new(tick), events)
	check(not result.committed and not result.error.is_empty(), mutation + ": request must reject")
	check(app.session.state_hash() == before, mutation + ": failed step rolls back complete session hash")
	check(app.staged_commands.is_empty() and app.staged_services.is_empty(), mutation + ": failure publishes no render/service staging")
	check(app.pending_services == before_pending and app.canvas._commands == before_commands and app.canvas._textures.keys() == before_textures, mutation + ": renderer caches and pending completions unchanged")
	dispose(app)
