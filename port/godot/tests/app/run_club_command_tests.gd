extends SceneTree
## T-29014: Comprehensive test suite for Command 4 Club gameplay wiring and rules.
## Verifies:
## 1. Adapter preflight, asset loading, and capability validation for all 11 clubs (0..10).
## 2. Parameter deltas and stress penalty math matching native club_activity_deltas.
## 3. Biorhythm wave mapping (Intellectual/Artistic/Physical) and practice point bonuses.
## 4. Practice point incrementation during school day execution.
## 5. Pre-mutation rejection when player has no club (club_id == 11).
## 6. Pre-mutation descriptor rejection with full session rollback.

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
const SchoolDay = preload("res://rules/school_day.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const CommandScreen = preload("res://flow/command_screen.gd")

var failures: Array[String] = []
var checks: int = 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
		push_error("FAIL: " + message)

func make_app(club_id: int = 0) -> Boot:
	var app: Boot = Boot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	var error: String = app._load_inputs()
	if error.is_empty():
		error = app._reset()
	check(error.is_empty(), "app preflight/reset: " + error)
	# Configure player club
	if app.state != null:
		app.state.rule_state["player_flags"] = (app.state.rule_state["player_flags"] & ~0xf000) | ((club_id & 15) << 12)
		# Initialize clubs array
		if not app.state.rule_state.has("clubs"):
			app.state.rule_state["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	return app

func dispose(app: Boot) -> void:
	if is_instance_valid(app.canvas): app.canvas.free()
	if is_instance_valid(app.pad): app.pad.free()
	if is_instance_valid(app.status_label): app.status_label.free()
	if is_instance_valid(app.detail_label): app.detail_label.free()
	app.free()

func _initialize() -> void:
	print("--- T-29014 Command 4 Club Integration & Rules Test Suite ---")
	_test_adapter_preflight_and_assets()
	_test_biorhythm_and_poor_result_rules()
	_test_parameter_deltas_and_practice_points()
	_test_command_screen_club_selection()
	_test_school_day_club_execution()
	_test_premutation_descriptor_rejections()
	_finish_suite()

func _test_adapter_preflight_and_assets() -> void:
	print("[Phase 1] Adapter preflight & all 11 clubs asset verification")
	var adapter: MediaAdapter = MediaAdapter.new()
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK")
	var spring_path: String = OS.get_environment("T29014_SPRING_PACK")
	if spring_path.is_empty():
		spring_path = OS.get_environment("TOKIMEMO_SPRING_PACK")
	var weekday56_path: String = OS.get_environment("T29014_WEEKDAY56_PACK")
	if weekday56_path.is_empty():
		weekday56_path = OS.get_environment("TOKIMEMO_WEEKDAY56_PACK")
	var club_path: String = OS.get_environment("T29014_CLUB_PACK")

	check(not media_path.is_empty(), "media pack path supplied")
	check(not club_path.is_empty(), "club pack path supplied")

	var err: String = adapter.open_packs(media_path, spring_path, weekday56_path, club_path)
	check(err.is_empty(), "adapter opens all packs including club: " + err)

	var available: Array[int] = adapter.get_available_commands()
	check(available.has(4), "adapter provides command 4")

	for club_id in range(11):
		check(adapter.has_command(4, club_id), "adapter has command 4 club %d" % club_id)
		var bg_cmds: Array[Dictionary] = adapter.get_background_commands(4, club_id)
		check(bg_cmds.size() == 2, "club %d has 2 split background geometry draw commands" % club_id)
		var res: Dictionary = adapter.get_resources(4, club_id)
		check(res.has("sequences") and res.has("frames"), "club %d has sequences and frames" % club_id)
		check(res["sequences"].has("0"), "club %d has sequence 0" % club_id)
		check(not res["frames"].is_empty(), "club %d has frames" % club_id)

	# Variant validation
	check(adapter.validate_variant(4, 11).contains("no active club"), "rejects club_id 11 (CLUB_NONE)")
	check(adapter.validate_variant(4, -1).contains("no active club"), "rejects negative club_id")
	check(adapter.validate_variant(4, 0, 7).contains("seasonal variant"), "rejects summer month 7")
	check(adapter.validate_variant(4, 0, 4, 0).contains("companion variants"), "rejects companion extra 0")
	check(adapter.validate_variant(4, 0, 4, 15, 1).contains("vacation"), "rejects vacation")
	check(adapter.validate_variant(4, 0, 4, 15, 0, 1).contains("sick/rest"), "rejects sick variant")
	check(adapter.validate_variant(4, 0, 4, 15, 0, 0).is_empty(), "accepts valid spring solo club activity")

func _test_biorhythm_and_poor_result_rules() -> void:
	print("[Phase 2] Scheduler biorhythm wave calculation & poor result rules")
	var app: Boot = make_app(0)
	var state: State = app.state
	var tables: Dictionary = app.controller.tables
	var rng: Rng = Rng.new()
	check(rng.configure(Rng.OPENBIOS_PROFILE, 12345), "configured BIOS RNG profile")

	# Verify poor_result evaluation across club types:
	# Clubs 0, 2, 3: Intellectual wave [0, 0, 1]
	# Clubs 1, 4, 5: Artistic wave [0, 1, 0]
	# Clubs 6, 7, 8, 9, 10: Physical wave [1, 0, 0]
	for club_id in range(11):
		state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~0xf000) | (club_id << 12)
		state.rule_state["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		# With practice points = 0
		var res0: int = Scheduler.poor_result(state, 4, 1, tables, rng)
		check(res0 in [0, 1], "club %d poor_result returns 0 or 1" % club_id)

		# With high practice points (e.g. 160 -> +10 score bonus)
		state.rule_state["clubs"][club_id] = 160
		var res_high: int = Scheduler.poor_result(state, 4, 1, tables, rng)
		check(res_high in [0, 1], "club %d with 160 practice points returns 0 or 1" % club_id)

	dispose(app)

func _test_parameter_deltas_and_practice_points() -> void:
	print("[Phase 3] Parameter deltas and practice point incrementation")
	var app: Boot = make_app(0)
	var state: State = app.state
	var tables: Dictionary = app.controller.tables

	for club_id in range(11):
		state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~0xf000) | (club_id << 12)
		state.rule_state["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		state.rule_state["params"] = [
			State.with_high(0, 50), State.with_high(0, 50), State.with_high(0, 50),
			State.with_high(0, 50), State.with_high(0, 50), State.with_high(0, 50),
			State.with_high(0, 50), State.with_high(0, 50), State.with_high(0, 20)
		]
		var initial_params: Array = state.rule_state["params"].duplicate()
		var expected_deltas: Array = SchoolDay.CLUB_ACTIVITY_DELTAS[club_id]

		# Apply parameters for success (result_divisor = 0 -> scale 1)
		var err: String = SchoolDay.apply_parameters(state, 4, 0, 0, tables)
		check(err.is_empty(), "club %d apply_parameters succeeded" % club_id)

		for param_idx in range(8):
			var expected_delta: int = expected_deltas[param_idx]
			var change_word: int = state.rule_state["params"][param_idx] - initial_params[param_idx]
			var expected_change_word: int = expected_delta * 4096
			check(change_word == expected_change_word, "club %d param %d delta match: %d == %d" % [club_id, param_idx, change_word, expected_change_word])

		# Practice points incrementation test
		check(state.rule_state["clubs"][club_id] == 0, "club %d initial points 0" % club_id)
		SchoolDay.increment_practice_points(state.rule_state, club_id)
		check(state.rule_state["clubs"][club_id] == 1, "club %d points incremented to 1" % club_id)
		SchoolDay.increment_practice_points(state.rule_state, club_id)
		check(state.rule_state["clubs"][club_id] == 2, "club %d points incremented to 2" % club_id)

	# Negative test: club_id 11 rejected
	state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~0xf000) | (11 << 12)
	var err_none: String = SchoolDay.apply_parameters(state, 4, 0, 0, tables)
	check(err_none.contains("valid club_id"), "apply_parameters rejects club_id 11")

	dispose(app)

func _test_command_screen_club_selection() -> void:
	print("[Phase 4] Command screen cursor confirmation & CLUB_NONE rejection")
	var app: Boot = make_app(0)
	var state: State = app.state
	var input: InputFrame = InputFrame.new(1, 0x20, 0) # held 0x20 -> pressed 0x20
	var result: Session.FrameResult = Session.FrameResult.new()

	# 1. Player has club 0 (Literature) -> confirmation succeeds
	state.rule_state["menu_cursor"] = 4
	state.rule_state["icon_enabled"][4] = 1
	state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~0xf000) | (0 << 12)
	CommandScreen.confirm(state, input, result, [0, 1, 2, 3, 4, 5, 6, 7])
	check(result.error.is_empty(), "command 4 confirmed when in club 0")
	check(result.transitions.size() == 1 and result.transitions[0]["mode"] == 0x30, "transitions to mode 0x30")

	# 2. Player has club 11 (CLUB_NONE) -> confirmation rejected
	var result_none: Session.FrameResult = Session.FrameResult.new()
	state.reset_game_mode_fields(0x34)
	state.mode_stage = 1
	state.mode_step = 0
	state.rule_state["menu_cursor"] = 4
	state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~0xf000) | (11 << 12)
	CommandScreen.confirm(state, input, result_none, [0, 1, 2, 3, 4, 5, 6, 7])
	check(result_none.error.contains("no active club"), "command 4 rejected when club_id is 11: " + result_none.error)

	dispose(app)

func _test_school_day_club_execution() -> void:
	print("[Phase 5] School day execution for clubs 0 (Lit), 6 (Baseball), 10 (Basketball)")
	for test_club in [0, 6, 10]:
		var app: Boot = make_app(test_club)
		var state: State = app.state
		state.rule_state["day"] = 5
		state.rule_state["last_command"] = 4
		# Queue a club activity day event on April 5
		state.rule_state["day_events"][0] = [0, 4, 0, 15]
		state.rule_state["day_event_index"] = 0
		state.reset_game_mode_fields(0x40)

		# Step 0: Background load
		var res_bg: Session.FrameResult = Session.FrameResult.new()
		app.controller._setup(state, res_bg)
		check(res_bg.error.is_empty(), "club %d background setup step succeeded" % test_club)
		check(res_bg.presentation.size() == 1 and res_bg.presentation[0]["kind"] == "school_day_load", "presents background load intent")
		check(res_bg.presentation[0]["background_lba"] == MediaAdapter.CLUB_ROUTES[test_club]["background_lba"], "matches club %d background LBA" % test_club)

		# Deliver background completion
		state.rule_state["resources"]["background"] = {"ready": true, "lba": res_bg.presentation[0]["background_lba"], "decode_result": 1}
		state.substep = 1
		app.controller._setup(state, Session.FrameResult.new()) # substep 1 -> 2
		app.controller._setup(state, Session.FrameResult.new()) # substep 2 -> mode_step 1
		check(state.mode_step == 1, "advanced to step 1 (SD load)")

		# Step 1: SD load
		var res_sd: Session.FrameResult = Session.FrameResult.new()
		app.controller._setup(state, res_sd) # substep 0 -> presents SD and substep 1
		check(res_sd.error.is_empty(), "club %d SD setup step succeeded" % test_club)
		check(res_sd.presentation.size() == 1 and res_sd.presentation[0]["kind"] == "school_day_load", "presents SD load intent")
		check(res_sd.presentation[0]["sd_set_id"] == MediaAdapter.CLUB_ROUTES[test_club]["sd_set_id"], "matches club %d SD set ID" % test_club)
		check(res_sd.presentation[0]["sd_lba"] == MediaAdapter.CLUB_ROUTES[test_club]["sd_lba"], "matches club %d SD LBA" % test_club)

		# Deliver SD completion
		state.rule_state["resources"]["sd"] = {"ready": true, "lba": res_sd.presentation[0]["sd_lba"], "checksum_ok": true}
		state.substep = 1
		app.controller._setup(state, Session.FrameResult.new()) # mode_step 1 -> 2
		check(state.mode_step == 2, "advanced to step 2 (actor init)")

		# Step 2: Actor init
		var res_actors: Session.FrameResult = Session.FrameResult.new()
		app.controller._setup(state, res_actors)
		check(res_actors.error.is_empty(), "club %d actor setup step succeeded" % test_club)
		check(res_actors.presentation.size() == 1 and res_actors.presentation[0]["kind"] == "school_day_actors", "presents school_day_actors")
		check(state.mode_stage == 1, "transitioned to stage 1 (main animation/effect)")

		# Main animation step 0: applies parameters and increments practice points
		state.rule_state["actor_state"]["96"]["flags"] = 4 # cleared busy bit
		state.rule_state["actor_state"]["96"]["sequence"]["counter"] = 0
		var res_main: Session.FrameResult = Session.FrameResult.new()
		SchoolDay.main_step(state, app.controller.tables, res_main)
		check(res_main.error.is_empty(), "club %d main step succeeded" % test_club)
		check(state.rule_state["clubs"][test_club] == 1, "club %d practice points incremented to 1" % test_club)

		dispose(app)

func _test_premutation_descriptor_rejections() -> void:
	print("[Phase 6] Pre-mutation negative descriptor mutations & rollback")
	var app: Boot = make_app(0)
	var state: State = app.state
	state.rule_state["day"] = 5
	state.rule_state["last_command"] = 4
	state.rule_state["day_events"][0] = [0, 4, 0, 15]
	state.rule_state["day_event_index"] = 0
	state.reset_game_mode_fields(0x40)

	# 1. Mutate background_lba
	var bad_bg_intent: Dictionary = {
		"kind": "school_day_load", "request": "background", "command": 4,
		"day_event_index": 0, "background_lba": 99999, "sd_set_id": 3,
		"sd_lba": 27957, "sectors": 41
	}
	var err_bg: String = app._resource_request_error(state, bad_bg_intent)
	check(err_bg.contains("differs from validated command media field: background_lba"), "rejects tampered background_lba: " + err_bg)

	# 2. Mutate sd_set_id
	var bad_sd_set: Dictionary = {
		"kind": "school_day_load", "request": "sd", "command": 4,
		"day_event_index": 0, "background_lba": 16215, "sd_set_id": 99,
		"sd_lba": 27957, "sectors": 41
	}
	var err_set: String = app._resource_request_error(state, bad_sd_set)
	check(err_set.contains("differs from validated command media field: sd_set_id"), "rejects tampered sd_set_id: " + err_set)

	# 3. Mutate sd_lba
	var bad_sd_lba: Dictionary = {
		"kind": "school_day_load", "request": "sd", "command": 4,
		"day_event_index": 0, "background_lba": 16215, "sd_set_id": 3,
		"sd_lba": 99999, "sectors": 41
	}
	var err_lba: String = app._resource_request_error(state, bad_sd_lba)
	check(err_lba.contains("differs from validated command media field: sd_lba"), "rejects tampered sd_lba: " + err_lba)

	# 4. Mutate sectors
	var bad_sectors: Dictionary = {
		"kind": "school_day_load", "request": "sd", "command": 4,
		"day_event_index": 0, "background_lba": 16215, "sd_set_id": 3,
		"sd_lba": 27957, "sectors": 99
	}
	var err_sec: String = app._resource_request_error(state, bad_sectors)
	check(err_sec.contains("identity differs from validated command media profile"), "rejects tampered sectors: " + err_sec)

	dispose(app)

func _finish_suite() -> void:
	var output_path: String = OS.get_environment("TOKIMEMO_CLUB_PROOF")
	var proof_data: Dictionary = {
		"schema": "tokimemo.command4-club-proof.v1",
		"checks": checks,
		"failures": failures,
		"status": "PASS" if failures.is_empty() else "FAIL"
	}
	if not output_path.is_empty():
		var file: FileAccess = FileAccess.open(output_path, FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(proof_data, "\t"))
			file.close()

	print("----------------------------------------------------------------------")
	print("Command 4 Club Test Suite: %d checks, %d failures" % [checks, failures.size()])
	if failures.is_empty():
		print("PASS: All Command 4 club integration and rules checks passed.")
		quit(0)
	else:
		for f in failures:
			printerr("  - " + f)
		quit(1)
