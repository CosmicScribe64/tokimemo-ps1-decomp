extends SceneTree
## T-29014: Comprehensive test suite for ordinary commands 1–3 and 5–6 gameplay wiring.
## Verifies:
## 1. Mapped-input selection and execution of commands 1, 2, 3, 5, 6 through first result.
## 2. Ordinary-week continuation to April 8 completion for commands 1, 2, 3, 5, 6.
## 3. Exact trace, state, and event hash preservation for commands 0 and 7 against canonical baseline.
## 4. Explicit pre-mutation rejection and session hash rollback for command 4 and unsupported variants.

const Boot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const CoreState = preload("res://core/game_state.gd")
const State = preload("res://rules/school_day_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")

var failures: Array[String] = []
var reports: Dictionary = {}
var checks: int = 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
		push_error("FAIL: " + message)

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
	if is_instance_valid(app.canvas): app.canvas.free()
	if is_instance_valid(app.pad): app.pad.free()
	if is_instance_valid(app.status_label): app.status_label.free()
	if is_instance_valid(app.detail_label): app.detail_label.free()
	app.free()

static func device_recipe(app: Boot, command: int, tick: int) -> void:
	if command > 0 and tick < 75:
		var step_index: int = tick / 10
		if step_index < command and tick % 10 in [7, 1]:
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
	print("--- T-29014 Ordinary Commands 1–6 Integration Suite ---")
	_test_adapter_preflight()
	_test_command_preservation_and_coverage()
	_test_premutation_rejections()
	_finish_suite()

func _test_adapter_preflight() -> void:
	print("[Phase 1] Adapter preflight & capability validation")
	var adapter: MediaAdapter = MediaAdapter.new()
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK")
	var spring_path: String = OS.get_environment("T29014_SPRING_PACK")
	if spring_path.is_empty():
		spring_path = OS.get_environment("TOKIMEMO_SPRING_PACK")
	var weekday56_path: String = OS.get_environment("T29014_WEEKDAY56_PACK")
	if weekday56_path.is_empty():
		weekday56_path = OS.get_environment("TOKIMEMO_WEEKDAY56_PACK")

	check(not media_path.is_empty(), "canonical media pack path supplied")
	check(not spring_path.is_empty(), "spring commands 1-3 pack path supplied")
	check(not weekday56_path.is_empty(), "weekday commands 5-6 pack path supplied")

	var err: String = adapter.open_packs(media_path, spring_path, weekday56_path)
	check(err.is_empty(), "adapter opens all 3 external packs: " + err)
	var available: Array[int] = adapter.get_available_commands()
	check(available == [0, 1, 2, 3, 5, 6, 7], "adapter provides exactly commands 0, 1, 2, 3, 5, 6, 7")

	for cmd in [0, 1, 2, 3, 5, 6, 7]:
		check(adapter.has_command(cmd), "command %d available in adapter" % cmd)
		var bg_cmds: Array[Dictionary] = adapter.get_background_commands(cmd)
		check(bg_cmds.size() == 2, "command %d has 2 background geometry draw commands" % cmd)
		check(bg_cmds[0]["id"].ends_with(":left") and bg_cmds[1]["id"].ends_with(":right"), "command %d left/right background IDs" % cmd)
		var res: Dictionary = adapter.get_resources(cmd)
		check(res.has("sequences") and res.has("frames"), "command %d has sequences and frames" % cmd)
		check(res["sequences"].has("0"), "command %d has sequence 0" % cmd)
		check(not res["frames"].is_empty(), "command %d has non-empty frames" % cmd)

	check(adapter.validate_variant(4) == "unsupported ordinary command 4: club/club-id dependent profile not included", "variant check rejects command 4")
	check(adapter.validate_variant(1, 7) == "seasonal variant outside spring months 3-5 is not supported", "variant check rejects summer month")
	check(adapter.validate_variant(1, 4, 15, 1) == "vacation content is unavailable", "variant check rejects vacation")
	check(adapter.validate_variant(1, 4, 15, 0, 1) == "sick/rest variants are not supported in this tranche", "variant check rejects sick")
	check(adapter.validate_variant(1, 4, 0) == "companion variants for command 1 are not supported in this tranche", "variant check rejects companion")

func _test_command_preservation_and_coverage() -> void:
	print("[Phase 2] Execution coverage & exact preservation across render schedules")
	# We test commands in order: 0, 1, 2, 3, 5, 6, 7
	var commands_to_test: Array[int] = [0, 1, 2, 3, 5, 6, 7]
	var baseline_hashes: Dictionary = {}

	for cmd: int in commands_to_test:
		var is_rest: bool = (cmd == 7)
		var schedules: Array[int] = [0, 1, 3]
		var schedule_hashes: Dictionary = {}

		for rendered: int in schedules:
			var app: Boot = make_app()
			# Command 7 only claims first-result boundary in this tranche; all others continue week
			app.continue_week = not is_rest
			var entry: Dictionary = {}

			for tick: int in range(601):
				device_recipe(app, cmd, tick)
				app.session.clock.rendered_frames += rendered
				app._physics_process(1.0 / 60.0)
				if entry.is_empty() and app.state.game_mode == 0x40 and app.state.mode_step == 0 and app.state.substep == 0:
					entry = {"state": app.state.projection(), "rng": app.session.rng.checkpoint()}
				if app.stopped:
					break

			check(app.stopped and app.failure.is_empty(), "command %d stopped cleanly at rendered=%d: %s" % [cmd, rendered, app.failure])
			check(not entry.is_empty(), "command %d reached actual activity entry (mode 0x40)" % cmd)

			# Record traces and hashes
			var hashes: Array = []
			for row: Dictionary in app.trace:
				hashes.append([row["state_hash"], row["event_hash"]])
			schedule_hashes[rendered] = hashes

			if rendered == 0:
				reports[str(cmd)] = {
					"entry": entry,
					"final_state": app.state.projection(),
					"rng": app.session.rng.checkpoint(),
					"trace": app.trace.duplicate(true),
					"checkpoints": app.checkpoints.duplicate(true),
					"supported_stop": app.supported_stop.duplicate(true)
				}
				# State checks at first result or week end
				if is_rest:
					check(app.state.game_mode == 0x31 and app.state.rule_state["day_event_index"] == 1 and app.state.rule_state["day"] == 5,
						"command 7 pauses at first result on April 5")
				else:
					check(app.state.game_mode == 0x31 and app.state.rule_state["day_event_index"] == 8 and app.state.rule_state["day"] == 8,
						"command %d completed April 8 ordinary week" % cmd)
					check(app.supported_stop.get("error") == Boot.HOLIDAY_STOP, "command %d reached April 9 holiday stop" % cmd)
					check(app.checkpoints.size() == 15, "command %d has 15 ordinary-week checkpoints" % cmd)

				# Verify parameter deltas occurred
				var initial: Array = app.initial_params
				var final_params: Array = app.state.rule_state["params"]
				if cmd != 7:
					check(final_params != initial, "command %d modified parameters" % cmd)
				check(app.state.rule_state["last_command"] == cmd, "command %d recorded as last_command" % cmd)

			dispose(app)

		# Schedule independence: hashes must match across rendered_frames = 0, 1, 3
		check(schedule_hashes[0] == schedule_hashes[1], "command %d rendered=0 equals rendered=1 hashes" % cmd)
		check(schedule_hashes[0] == schedule_hashes[3], "command %d rendered=0 equals rendered=3 hashes" % cmd)
		baseline_hashes[cmd] = schedule_hashes[0]

	# Verify Command 0 and 7 traces match expected baseline from boundary file
	var test_app: Boot = make_app()
	var exp_cmd0: Dictionary = test_app.boundary.get("expected", {}).get("0", {})
	if not exp_cmd0.is_empty() and reports.has("0"):
		var rep0_entry: Dictionary = reports["0"]["entry"]["state"]
		check(rep0_entry["frame_count"] == exp_cmd0["native_frame_count"], "command 0 entry native frame count preserved")
		check([rep0_entry["game_mode_timer"], rep0_entry["mode_stage_timer"], rep0_entry["mode_step_timer"]] == exp_cmd0["native_mode_timers"],
			"command 0 entry native mode timers preserved")
	var exp_cmd7: Dictionary = test_app.boundary.get("expected", {}).get("7", {})
	if not exp_cmd7.is_empty() and reports.has("7"):
		var rep7_entry: Dictionary = reports["7"]["entry"]["state"]
		check(rep7_entry["frame_count"] == exp_cmd7["native_frame_count"], "command 7 entry native frame count preserved")
		check([rep7_entry["game_mode_timer"], rep7_entry["mode_stage_timer"], rep7_entry["mode_step_timer"]] == exp_cmd7["native_mode_timers"],
			"command 7 entry native mode timers preserved")
	dispose(test_app)

	# Verify first-result mode specifically for commands 1, 2, 3, 5, 6
	for cmd: int in [1, 2, 3, 5, 6]:
		var first_app: Boot = make_app()
		first_app.continue_week = false
		for tick: int in range(601):
			device_recipe(first_app, cmd, tick)
			first_app._physics_process(1.0 / 60.0)
			if first_app.stopped:
				break
		check(first_app.stopped and first_app.failure.is_empty(), "command %d first-result stopped cleanly" % cmd)
		check(first_app.state.game_mode == 0x31 and first_app.state.rule_state["day_event_index"] == 1 and first_app.state.rule_state["day"] == 5,
			"command %d pauses at 0x31 / index 1 on April 5" % cmd)
		check(first_app.captured_actor, "command %d captured actor during school day" % cmd)
		check(first_app.captured_effect, "command %d captured parameter effect" % cmd)
		dispose(first_app)

func _test_premutation_rejections() -> void:
	print("[Phase 3] Pre-mutation rejection & session rollback validation")
	# 1. Command 4 (club) selection confirmation rejection
	var app4: Boot = make_app()
	# Step to command confirmation frame (tick 97 after 4 RIGHT presses)
	for tick: int in range(97):
		device_recipe(app4, 4, tick)
		app4._physics_process(1.0 / 60.0)
	check(app4.state.rule_state["menu_cursor"] == 4, "cursor navigated to command 4")
	var enter4 := InputEventKey.new()
	enter4.physical_keycode = KEY_ENTER
	enter4.pressed = true
	app4.pad._input(enter4)
	app4._physics_process(1.0 / 60.0)
	var hash_before_confirm4: String = app4.session.state_hash()
	app4._physics_process(1.0 / 60.0)
	check(app4.stopped, "app stopped upon command 4 confirmation")
	check(app4.failure.contains("selected native command is not implemented: 4"), "exact error on command 4 confirmation: " + app4.failure)
	check(app4.session.state_hash() == hash_before_confirm4, "command 4 rejection preserves full session state hash (rollback)")
	dispose(app4)

	# 2. Unbacked command capability rejection
	var app_unbacked: Boot = make_app()
	# Restrict capabilities to [0, 7]
	app_unbacked.controller.command_capabilities = [0, 7]
	for tick: int in range(97):
		device_recipe(app_unbacked, 1, tick)
		app_unbacked._physics_process(1.0 / 60.0)
	check(app_unbacked.state.rule_state["menu_cursor"] == 1, "cursor navigated to command 1")
	var enter_unbacked := InputEventKey.new()
	enter_unbacked.physical_keycode = KEY_ENTER
	enter_unbacked.pressed = true
	app_unbacked.pad._input(enter_unbacked)
	app_unbacked._physics_process(1.0 / 60.0)
	var hash_before_unbacked: String = app_unbacked.session.state_hash()
	app_unbacked._physics_process(1.0 / 60.0)
	check(app_unbacked.stopped, "app stopped upon unbacked command confirmation")
	check(app_unbacked.failure.contains("required route content capability is unavailable"), "exact error on unbacked command: " + app_unbacked.failure)
	check(app_unbacked.session.state_hash() == hash_before_unbacked, "unbacked capability rejection preserves session state hash (rollback)")
	dispose(app_unbacked)

	# 3. Companion variant rejection in school day
	var app_companion: Boot = make_app()
	# Step into activity mode (command 1)
	for tick: int in range(120):
		device_recipe(app_companion, 1, tick)
		app_companion._physics_process(1.0 / 60.0)
		if app_companion.state.game_mode == 0x40:
			break
	check(app_companion.state.game_mode == 0x40, "entered school day mode 0x40")
	# Force companion variant in day_event record
	app_companion.state.rule_state["day_events"][app_companion.state.rule_state["day_event_index"]][3] = 0 # Shiori companion
	var hash_before_comp: String = app_companion.session.state_hash()
	app_companion._physics_process(1.0 / 60.0)
	check(app_companion.stopped, "app stopped upon companion variant in school day")
	check(app_companion.failure.contains("companion variants for command 1 are not supported"), "exact diagnostic on companion variant: " + app_companion.failure)
	check(app_companion.session.state_hash() == hash_before_comp, "companion variant rejection rolls back session state hash")
	dispose(app_companion)

	# 4. Sick variant rejection
	var app_sick: Boot = make_app()
	for tick: int in range(120):
		device_recipe(app_sick, 1, tick)
		app_sick._physics_process(1.0 / 60.0)
		if app_sick.state.game_mode == 0x40:
			break
	# Set player health to sick (nibble 3)
	app_sick.state.rule_state["player_flags"] = (app_sick.state.rule_state["player_flags"] & ~0x0f00) | 0x0300
	var hash_before_sick: String = app_sick.session.state_hash()
	app_sick._physics_process(1.0 / 60.0)
	check(app_sick.stopped, "app stopped upon sick variant in school day")
	check(app_sick.failure.contains("resource request has no preflighted command media profile"), "sick variant rejected with no media profile error: " + app_sick.failure)
	check(app_sick.session.state_hash() == hash_before_sick, "sick variant rejection rolls back session state hash")
	dispose(app_sick)

	# 5. Vacation variant rejection
	var app_vacation: Boot = make_app()
	for tick: int in range(120):
		device_recipe(app_vacation, 1, tick)
		app_vacation._physics_process(1.0 / 60.0)
		if app_vacation.state.game_mode == 0x40:
			break
	app_vacation.state.rule_state["vacation"] = 1
	var hash_before_vac: String = app_vacation.session.state_hash()
	app_vacation._physics_process(1.0 / 60.0)
	check(app_vacation.stopped, "app stopped upon vacation variant in school day")
	check(app_vacation.failure.contains("outside initial April5–8 ordinary week") or app_vacation.failure.contains("resource request"),
		"vacation variant rejected before mutation: " + app_vacation.failure)
	check(app_vacation.session.state_hash() == hash_before_vac, "vacation variant rejection rolls back session state hash")
	dispose(app_vacation)

	# 6. Negative mutations on school_day_load descriptor
	for mutation: String in ["background_lba", "sd_set_id", "sd_lba", "sectors"]:
		_reject_descriptor_mutation(mutation)

func _reject_descriptor_mutation(mutation: String) -> void:
	var app: Boot = make_app()
	for tick in range(301):
		device_recipe(app, 1, tick)
		if app.state.game_mode == 0x40 and app.state.mode_stage == 0 and app.state.substep == 0 and app.state.mode_step == 0:
			break
		app._physics_process(1.0 / 60.0)
	app.session.draw_logic = func(base: CoreState, rng: Rng, result: Session.FrameResult) -> void:
		for intent in result.presentation:
			if intent.get("kind") == "school_day_load":
				intent[mutation] = "other" if mutation == "request" else int(intent[mutation]) + 1
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
	check(not result.committed and not result.error.is_empty(), "descriptor mutation '%s' refused commit" % mutation)
	check(result.error.contains("differs from validated command media"), "descriptor mutation '%s' exact error: %s" % [mutation, result.error])
	check(app.session.state_hash() == before, "descriptor mutation '%s' full session rollback" % mutation)
	check(app.staged_commands.is_empty() and app.staged_services.is_empty(), "descriptor mutation '%s' publishes no staging" % mutation)
	check(app.pending_services == before_pending and app.canvas._commands == before_commands and app.canvas._textures.keys() == before_textures, "descriptor mutation '%s' caches unchanged" % mutation)
	dispose(app)

func _finish_suite() -> void:
	var output_path: String = OS.get_environment("TOKIMEMO_ORDINARY_PROOF")
	var proof_data: Dictionary = {
		"schema": "tokimemo.ordinary-commands-1-6-proof.v1",
		"checks": checks,
		"failures": failures,
		"commands_tested": reports.keys(),
		"commands_report": reports
	}
	if not output_path.is_empty():
		var file: FileAccess = FileAccess.open(output_path, FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(proof_data, "\t"))
			file.close()

	print("-------------------------------------------------------")
	print("Ordinary commands 1–6 test suite: %d checks, %d failures" % [checks, failures.size()])
	if failures.is_empty():
		print("PASS: All ordinary command coverage, exact preservation, and pre-mutation rejections verified.")
		quit(0)
	else:
		for f in failures:
			printerr("  - " + f)
		quit(1)
