extends SceneTree
## T-29026. Real app rules/HUD/Pad/service join; manual fixed-step integration test.
## This test is separate from the actual OS/viewport player-route witness.

const Boot = preload("res://app/playable_boot.gd")
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const Harness = preload("res://tests/app/run_command7_app_tests.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const SaveAdapter = preload("res://services/playable_save_adapter.gd")

var failures: Array[String] = []
var checks: int = 0
var points: Array = []

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func key(app: Boot, down: bool) -> void:
	var event := InputEventKey.new()
	event.physical_keycode = KEY_ENTER
	event.pressed = down
	app.pad._input(event)

const DEFAULT_ENV: Dictionary = {
	"TOKIMEMO_BOUNDARY": "/workspace/.claude/orchestrator/state/T-29004/evidence/development-boundary.json",
	"TOKIMEMO_BOUNDARY_SHA": "cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5",
	"TOKIMEMO_TABLES": "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json",
	"TOKIMEMO_TABLES_SHA": ContentIdentity.RULE_TABLES_SHA,
	"TOKIMEMO_MEDIA_PACK": "/workspace/.asset-cache/port/holiday-command0-current-964ce",
	"TOKIMEMO_HUD_MANIFEST": "/workspace/.asset-cache/port/hud-production-v1/manifest.json",
	"TOKIMEMO_HUD_SHA": ContentIdentity.HUD_MANIFEST_SHA,
	"TOKIMEMO_FONT_ROOT": "/workspace/.asset-cache/port/fonts/native-font-v2",
	"TOKIMEMO_HUD_CONTEXT": "/workspace/.claude/orchestrator/state/T-29010/app-integration-v1/proof/development-hud-context.json",
	"TOKIMEMO_HUD_CONTEXT_SHA": "2ec736f93177c47d7d0dce13444183215cbc65a88b40a065319c73bee9ce0451",
	"TOKIMEMO_HOLIDAY_EXTENSION": "/workspace/.claude/orchestrator/state/T-29013/evidence/holiday-boundary-extension.json",
	"TOKIMEMO_HOLIDAY_TEXT": "/workspace/.asset-cache/port/holiday-hud-text-v1/manifest.json",
	"TOKIMEMO_HOLIDAY_FIXTURE": "/workspace/.claude/orchestrator/state/T-29013/evidence/native-rules.json",
	"TOKIMEMO_DATE_PACK": "/tmp/disabled",
	"TOKIMEMO_PLAYABLE_OUTPUT": "/tmp"
}

static func _ensure_default_env() -> void:
	for env_key: String in DEFAULT_ENV:
		if OS.get_environment(env_key).is_empty():
			OS.set_environment(env_key, DEFAULT_ENV[env_key])

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	_ensure_default_env()
	var unopened := Boot.new()
	unopened.state = Boot.State.new()
	unopened.state.rule_state = {"resources": {"available_commands": [0]}}
	var untouched: Dictionary = unopened.state.projection()
	check(not unopened._enable_multi_week().is_empty(), "missing holiday capability refuses multi-week preflight")
	check(unopened.state.projection() == untouched, "failed multi-week preflight changes no gameplay/capability fields")
	unopened.free()
	var harness := Harness.new()
	# Launch configuration is the only multi-week seam. Rules, queue, RNG,
	# resource completions and actor lifetimes all run through the production app.
	OS.set_environment("TOKIMEMO_MULTI_WEEK", "1")
	var app: Boot = harness.make_app()
	app.continue_week = true
	var live_hud: Variant = app.hud
	check(harness.failures.is_empty(), "real original app preflight: " + str(harness.failures))
	check(app.state.rule_state["resources"]["available_commands"] == [0, 7], "capabilities equal opened packs; no forced commands")
	var menu_tick: int = -1
	var next_tick: int = -1
	var release_tick: int = -1
	var sunday_actors: int = 0
	var monday_actors: int = 0
	var sunday_sequence_positions: Dictionary = {}
	var monday_sequence_positions: Dictionary = {}
	var sunday_terminal: bool = false
	var monday_terminal: bool = false
	var reached_monday_result: bool = false
	var next_snapshot: Dictionary = {}
	var rollback_tested: bool = false
	var save_adapter := SaveAdapter.new({"source_version": "t29026-candidate-v1", "content_version": "original-exact-sequences", "behavior_version": "real-sunday-week2"})
	for iteration: int in range(900):
		var tick: int = app.session.clock.logic_iterations
		if tick == 17:
			key(app, true)
		if tick == 23 or tick == release_tick:
			key(app, false)
		if app.state.game_mode == 0x46 and app.state.mode_stage == 1 and menu_tick < 0:
			menu_tick = tick
		if menu_tick >= 0 and tick == menu_tick + 32:
			key(app, true)
			release_tick = tick + 2
		if app.state.game_mode == 0x34 and app.state.mode_stage == 1 and app.state.rule_state["day_event_index"] > 0 and next_tick < 0:
			next_tick = tick
			var captured: Dictionary = save_adapter.capture(app, "next_command")
			check(captured["error"].is_empty(), "real returned command save capture: " + captured["error"])
			if captured["error"].is_empty():
				next_snapshot = captured["snapshot"]
		if next_tick >= 0 and tick == next_tick + 8:
			key(app, true)
			release_tick = tick + 2
		app.hud = null if (tick > 0 and app.state.game_mode == 0x40) else live_hud
		app._physics_process(1.0 / 60.0)
		app.hud = live_hud
		if not app.failure.is_empty():
			check(false, "app failed at%d: %s" % [tick, app.failure])
			break
		var data: Dictionary = app.state.rule_state
		var phase: Array = [app.state.game_mode, app.state.mode_stage, app.state.mode_step, app.state.substep]
		if points.is_empty() or points[-1]["phase"] != phase:
			points.append({"tick": tick, "phase": phase, "date": [data["year"], data["month"], data["day"]],
				"queue_index": data["day_event_index"], "queue": data["day_events"].duplicate(true), "rng": app.session.rng.state})
		if data["actor_state"].has("96"):
			var actor: Dictionary = data["actor_state"]["96"]
			var seq: Dictionary = actor["sequence"]
			if app.state.game_mode == 0x46:
				sunday_actors += 1
				sunday_sequence_positions[JSON.stringify(seq)] = true
			elif data["day"] == 10 and app.state.game_mode == 0x40:
				monday_actors += 1
				monday_sequence_positions[JSON.stringify(seq)] = true
				if not rollback_tested:
					_test_missing_frame_rollback(app)
					rollback_tested = true
		for service: Dictionary in app.pending_services:
			if service["service"] == "school_day.actor96":
				if app.state.game_mode == 0x46:
					sunday_terminal = true
				elif data["day"] == 10:
					monday_terminal = true
		if data["day"] == 10 and app.state.game_mode == 0x31 and data["day_event_index"] == 1:
			reached_monday_result = true
			break
	check(menu_tick >= 0 and next_tick > menu_tick, "Sunday requires real menu input before returning")
	check(sunday_actors >= 33 and sunday_sequence_positions.size() >= 30 and sunday_terminal,
		"Sunday original actor advances and actually reaches terminal: %d draws/%d positions" % [sunday_actors, sunday_sequence_positions.size()])
	check(monday_actors >= 33 and monday_sequence_positions.size() >= 30 and monday_terminal,
		"Week2 actor lifetime is source sequence driven: %d draws/%d positions" % [monday_actors, monday_sequence_positions.size()])
	check(reached_monday_result, "actual scheduler reaches April10 first result without Sunday skip")
	if not next_snapshot.is_empty():
		var loaded: Boot = harness.make_app()
		loaded.continue_week = true
		var loaded_hud: Variant = loaded.hud
		var restored: String = save_adapter.restore(loaded, next_snapshot, loaded._present_committed)
		check(restored.is_empty(), "returned command restore owns real holiday controller: " + restored)
		if restored.is_empty():
			var start_tick: int = loaded.session.clock.logic_iterations
			while loaded.session.clock.logic_iterations < app.session.clock.logic_iterations:
				var tick: int = loaded.session.clock.logic_iterations
				if tick == next_tick + 8:
					key(loaded, true)
				if tick == next_tick + 10:
					key(loaded, false)
				loaded.hud = null if (tick > 0 and loaded.state.game_mode == 0x40) else loaded_hud
				loaded._physics_process(1.0 / 60.0)
				loaded.hud = loaded_hud
				if not loaded.failure.is_empty():
					break
			check(loaded.failure.is_empty() and loaded.session.state_hash() == app.session.state_hash(),
				"restored real next-command reaches identical Week2 state/RNG: " + loaded.failure)
			check(loaded.trace.slice(start_tick) == app.trace.slice(start_tick), "save/load Week2 continuation preserves every state/event/input-derived presentation hash")
		harness.dispose(loaded)
	var output := FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("week2-services.json"), FileAccess.WRITE)
	if output != null:
		output.store_string(JSON.stringify({"checks": checks, "failures": failures, "points": points,
			"sunday_draws": sunday_actors, "monday_draws": monday_actors,
			"final_state": app.state.projection(), "rng": app.session.rng.checkpoint(), "trace": app.trace}, "\t"))
		output.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "sunday_draws": sunday_actors, "monday_draws": monday_actors}))
	harness.dispose(app)
	harness.free()
	quit(0 if failures.is_empty() else 1)

func _test_missing_frame_rollback(app: Boot) -> void:
	# Explicit negative injection into preloaded media; the tested player route
	# never supplies synthetic completion or changes a gameplay field.
	var media: Dictionary = app.exact_media.get("_canonical")[0]
	var frames: Dictionary = media["frames"]
	var before: Dictionary = {"state": app.state.projection(), "rng": app.session.rng.checkpoint(),
		"clock": app.session.clock.snapshot(), "hash": app.session.state_hash()}
	frames.clear()
	app.staged_commands.clear()
	app.staged_services.clear()
	var rejected: Session.FrameResult = app.session.step(InputFrame.new(app.session.clock.logic_iterations), [])
	check(not rejected.committed and not rejected.error.is_empty(), "missing exact frame rejects the whole native step")
	check(before == {"state": app.state.projection(), "rng": app.session.rng.checkpoint(),
		"clock": app.session.clock.snapshot(), "hash": app.session.state_hash()}, "missing frame rolls back rules/RNG/clock/hash")
	check(app.staged_commands.is_empty() and app.staged_services.is_empty(), "failed media stages no draw/service publication")
	# The resource dictionary is the fixture seam; restore its complete contents
	# from the immutable already-preflighted adapter before normal stepping.
	var restored: Dictionary = Boot.ExactMedia.from_media(app.media, "command_0_sd")
	media["frames"] = restored["frames"]
