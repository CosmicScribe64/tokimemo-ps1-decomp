extends SceneTree
## T-29026. Core differential producer from an exact captured native boundary.
## Uses preflighted production draw/resource/actor services, without HUD replays.
## Original physical CD wait timing is retained separately in the native trace.

const Boot = preload("res://app/playable_boot.gd")
const Harness = preload("res://tests/app/run_command7_app_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")

func _initialize() -> void:
	var path: String = "/repo/.claude/orchestrator/state/T-29026/candidate-v1/native-week2/start-boundary.json"
	var boundary: Dictionary = Boot._integers(JSON.parse_string(FileAccess.get_file_as_string(path)))
	if not boundary.get("core") is Dictionary or not boundary.get("rule_state") is Dictionary:
		push_error("captured native start boundary is unavailable")
		quit(1)
		return
	var harness := Harness.new()
	var app: Boot = harness.make_app()
	if not harness.failures.is_empty():
		push_error(str(harness.failures))
		quit(1)
		return
	app.state = State.new()
	for key: String in boundary["core"]:
		app.state.set(key, boundary["core"][key])
	app.state.rule_state = boundary["rule_state"].duplicate(true)
	# Only adapter capability/integrity metadata is added; the entire measured
	# original gameplay projection and initial input are preserved verbatim.
	app.state.rule_state["allow_multi_week"] = true
	app.state.rule_state["resources"]["available_commands"] = app.adapter.get_available_commands()
	app.state.rule_state["resources"]["available_holiday_commands"] = [0]
	app.state.rule_state[Ordinary.PROFILE_HASH_FIELD] = Ordinary._profile_signature(app.state.rule_state)
	app.session = Session.new()
	app.session.rng.configure(Boot.Rng.OPENBIOS_PROFILE, boundary["rng_state"])
	var failure: String = app.controller.attach_multi_week(app.session, app.state)
	if failure.is_empty():
		failure = app.holiday_controller.attach(app.session, app.state)
	app.session.draw_logic = app._native_draw
	app.session.cd_logic = app._bounded_cd
	app.session.audio_logic = app._bounded_audio
	var measured_input: Dictionary = boundary["input"]
	app.session.latched_input = InputFrame.new(-1, measured_input["held"],
		(measured_input["held"] & ~measured_input["pressed"]) | measured_input["released"])
	var samples: Array = []
	var pending: Array = []
	for tick: int in range(600):
		if not failure.is_empty():
			break
		var before: Dictionary = app.state.projection()
		var before_rng: Dictionary = app.session.rng.checkpoint()
		var before_input: Dictionary = app.session.latched_input.snapshot()
		var events: Array[Session.ServiceEvent] = []
		for descriptor: Dictionary in pending:
			events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION,
				descriptor["service"], descriptor["payload"]))
		app.staged_commands.clear()
		app.staged_services.clear()
		# The approved original recipe presses Circle at vsync32 for6 frames.
		# Core tick0 corresponds to the first resumed native frame1.
		var result: Session.FrameResult = app.session.step(InputFrame.new(tick, 0x20 if tick + 1 >= 32 and tick + 1 < 38 else 0), events)
		failure = result.error
		samples.append({"iteration": tick, "before": before, "after": app.state.projection(),
			"rng_before": before_rng, "rng_after": app.session.rng.checkpoint(), "input": before_input,
			"incoming_services": pending.duplicate(true), "services": app.staged_services.duplicate(true),
			"presentation": result.presentation, "audio": result.audio, "committed": result.committed, "error": failure})
		pending = app.staged_services.duplicate(true)
		if [app.state.game_mode, app.state.mode_stage] == [0x46, 1] and app.state.rule_state["day"] == 16:
			break
	var file := FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("native-start-week2-core.json"), FileAccess.WRITE)
	file.store_string(JSON.stringify({"scope": "core-only shared genuine native initial boundary, actual immutable service adapters; no OS/viewport or CD timing claim",
		"start_boundary_sha256": FileAccess.get_sha256(path), "input_recipe": [{"frame": 32, "held": 0x20, "frames": 6}],
		"failure": failure, "samples": samples, "final_state": app.state.projection(), "rng": app.session.rng.checkpoint()}, "\t"))
	file.close()
	print(JSON.stringify({"failure": failure, "steps": samples.size(), "date": [app.state.rule_state["year"], app.state.rule_state["month"], app.state.rule_state["day"]],
		"mode": [app.state.game_mode, app.state.mode_stage], "rng": app.session.rng.state}))
	harness.dispose(app)
	harness.free()
	quit(0 if failure.is_empty() else 1)
