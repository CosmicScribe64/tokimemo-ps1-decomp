extends SceneTree

const Arrival = preload("res://flow/prologue_arrival.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Session = preload("res://core/game_session.gd")

const PAD_START: int = 0x0800
const SEED: int = 0x24681357

var failures: int = 0
var checks: int = 0

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	_test_petal_initialization()
	_test_petal_update()
	_test_start_order_and_finish_cleanup()
	print("PASS: %d prologue source-fix checks" % checks if failures == 0 else "FAIL: %d/%d prologue source-fix checks" % [failures, checks])
	quit(0 if failures == 0 else 1)

func _test_petal_initialization() -> void:
	var state: State = State.new()
	var rng: Rng = _rng()
	var expected_rng: Rng = _rng()
	Arrival._petals_init(state, rng)
	var petals: Array = state.opening["prologue_petals"]
	var raw_unk_18: int = expected_rng.next("expected:unk_18")
	var raw_x: int = expected_rng.next("expected:x")
	var raw_y: int = expected_rng.next("expected:y")
	var raw_fourth: int = expected_rng.next("expected:brightness")
	check(rng.draw_count == 32, "petal init consumes exactly four source RNG draws for each of eight actors")
	check(petals.size() == 8 and int(petals[0]["unk_18"]) == (raw_unk_18 & 7), "first source random draw initializes actor unk_18")
	check(int(petals[0]["x_word"]) == (((raw_x % 440) - 160) << 16), "second source random draw initializes signed x high word over the 440 span")
	check(int(petals[0]["y_word"]) == ((-120 - (raw_y % 120)) << 16), "third source random draw initializes y high word over the 120 span")
	check(int(petals[0]["brightness"]) == (raw_fourth & 0xff) and int(petals[0]["draw_flags"]) == 0xa4, "fourth source random draw sets this actor brightness and initialization sets draw flags")

func _test_petal_update() -> void:
	var state: State = State.new()
	var rng: Rng = _rng()
	var petals: Array = []
	for index: int in range(8):
		petals.append({"unk_18": 0, "unk_07": 0, "x_word": 0x00001234, "y_word": (39 if index == 0 else 40) << 16, "draw_flags": 0xa4})
	state.opening["prologue_petals"] = petals
	var sine_angles: Array[int] = []
	var services: Dictionary = {}
	services["prologue_sine"] = func(angle: int) -> int:
		sine_angles.append(angle)
		return 0
	var update_out: Session.FrameResult = Session.FrameResult.new()
	Arrival._petals_update(state, rng, 1, services, update_out)
	var updated: Array = state.opening["prologue_petals"]
	check(update_out.error.is_empty() and sine_angles == [0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20], "each petal tick obtains its sine value from the explicit caller-provided source service")
	check(rng.draw_count == 14, "all seven y>=40 actors wrap after increment and consume exactly two RNG draws each in motion variant 1")
	var expected_low_x: int = 0x1234
	check(int(updated[0]["unk_07"]) == 1 and int(updated[0]["y_word"]) == (40 << 16) and (int(updated[0]["x_word"]) & 0xffff) == expected_low_x, "below-threshold update advances angle and fixed-point sine while preserving high-word coordinates")
	var first_wrap_x: int = Math.s16(Math.u32(int(updated[1]["x_word"])) >> 16)
	var first_wrap_y: int = Math.s16(Math.u32(int(updated[1]["y_word"])) >> 16)
	check(first_wrap_x >= -160 and first_wrap_x < 160 and first_wrap_y >= -167 and first_wrap_y <= -120, "variant 1 still applies the source wrap ranges after reaching y=41")
	check(int(updated[1]["draw_flags"]) == 0xa4 and int(updated[4]["draw_flags"]) == 0, "variant-1 wrap hides petals at source actor indices 4 through 7 only")
	var expected_wrap_low_x: int = 0x1234
	check((int(updated[1]["x_word"]) & 0xffff) == expected_wrap_low_x and (int(updated[1]["y_word"]) & 0xffff) == 0, "source high-word replacement leaves fixed-point low words intact")
	var missing_state: State = State.new()
	missing_state.opening["prologue_petals"] = petals.duplicate(true)
	var missing_before: Dictionary = missing_state.projection()
	var missing_rng: Rng = _rng()
	var missing_out: Session.FrameResult = Session.FrameResult.new()
	Arrival._petals_update(missing_state, missing_rng, 0, {}, missing_out)
	check(not missing_out.error.is_empty() and missing_state.projection() == missing_before and missing_rng.draw_count == 0, "missing source sine service fails before mutating petal state or consuming RNG")

func _test_start_order_and_finish_cleanup() -> void:
	var state: State = State.new()
	state.game_mode = Arrival.MODE_PROLOGUE
	var rng: Rng = _rng()
	var services: Dictionary = _services()
	var start_res: Session.FrameResult = Arrival.step(state, rng, PAD_START, services)
	check(start_res.error.is_empty() and state.mode_stage == 21 and state.mode_step == 0 and state.substep == 0, "Start is tested after stage 0 step 0 advances, matching the outer C handler")
	check(start_res.audio.is_empty(), "skip applies mode_stage_set directly without synthesizing an audio-stop command")

	var missing_state: State = State.new()
	missing_state.game_mode = Arrival.MODE_PROLOGUE
	missing_state.mode_stage = 21
	var before: Dictionary = missing_state.projection()
	var missing: Session.FrameResult = Arrival.step(missing_state, rng, 0, _services(false))
	check(not missing.error.is_empty() and missing_state.projection() == before, "finish reset callbacks are mandatory at source substep 0 and missing service rolls state back")

	var finish_state: State = State.new()
	finish_state.game_mode = Arrival.MODE_PROLOGUE
	finish_state.mode_stage = 21
	var finish_log: Array[String] = []
	var finish_services: Dictionary = _services(true, finish_log)
	var finish_res: Session.FrameResult = Arrival.step(finish_state, rng, 0, finish_services)
	check(finish_res.error.is_empty() and finish_log == ["cd", "bank", "audio:5", "audio:209", "load:normal_gameplay"], "finish substep 0 requests CD reset, bank reset, raw command 5/fade, then gameplay bank load in C order")
	check(finish_state.opening["D_800B5BD4"] == 0x0f and finish_state.opening["screen_brightness"] == 128 and finish_state.substep == 1, "finish substep 0 applies source state before advancing")

	finish_state.substep = 1
	var final_res: Session.FrameResult = Arrival.step(finish_state, rng, 0, finish_services)
	check(final_res.error.is_empty() and finish_state.game_mode == Arrival.MODE_COMMAND_SCREEN and finish_state.mode_stage == 0xff and finish_state.mode_step == 0, "finish substep 1 resets all game-mode fields before selecting command reload stage")
	check(finish_state.opening["D_800B3DB0"] == 0 and finish_state.opening["text_speed"] == 3 and finish_state.rule_state["menu_mode"] == 1, "finish restores the source mode flag, slow text speed, and menu mode")

	var edge_state: State = State.new()
	edge_state.game_mode = Arrival.MODE_PROLOGUE
	edge_state.mode_stage = 21
	edge_state.substep = 1
	var edge_res: Session.FrameResult = Arrival.step(edge_state, rng, PAD_START, finish_services)
	check(edge_res.error.is_empty() and edge_state.game_mode == Arrival.MODE_COMMAND_SCREEN and edge_state.mode_stage == 21 and edge_state.substep == 0, "post-handler Start check retains C stage-21 mode-change edge semantics")

func _rng() -> Rng:
	var result: Rng = Rng.new()
	result.configure(Rng.OPENBIOS_PROFILE, SEED)
	return result

func _services(include_finish: bool = true, log: Array[String] = []) -> Dictionary:
	var services: Dictionary = {}
	services["bank_load_start"] = func(_s: State, role: String, _out: Session.FrameResult) -> void:
		if role == "normal_gameplay":
			log.append("load:" + role)
	services["scene_reset"] = func(_s: State, _out: Session.FrameResult) -> void:
			pass
	services["bank_poll"] = func(_s: State, _out: Session.FrameResult) -> int:
			return 0
	services["bank_start"] = func(_s: State, _bank: int, _sequence: int, _out: Session.FrameResult) -> void:
			pass
	services["background_read"] = func(_s: State, _lba: int, _slot: int, _out: Session.FrameResult, _retry: bool = false) -> void:
			pass
	services["cd_sync"] = func(_s: State, _out: Session.FrameResult) -> int:
			return 0
	services["background_decode"] = func(_s: State, _slot: int, _out: Session.FrameResult) -> int:
			return 0
	services["actor_read"] = func(_s: State, _sectors: int, _destination: int, _file_id: int, _out: Session.FrameResult) -> void:
			pass
	services["actor_palette_setup"] = func(_s: State, _out: Session.FrameResult) -> void:
			pass
	services["line_start"] = func(_s: State, _line: int, _out: Session.FrameResult) -> void:
			pass
	services["line_run"] = func(_s: State, _out: Session.FrameResult) -> void:
			pass
	services["arrival_draw"] = func(_s: State, _frame: int, _out: Session.FrameResult) -> void:
			pass
	services["outer_draw"] = func(_s: State, _frame: int, _out: Session.FrameResult) -> void:
			pass
	services["prologue_sine"] = func(_angle: int) -> int:
			return 0
	services["audio_command"] = func(_s: State, command: int, _out: Session.FrameResult) -> void:
			log.append("audio:" + str(command))
	if include_finish:
		services["finish_cd_reset"] = func(_s: State, _out: Session.FrameResult) -> void: log.append("cd")
		services["finish_bank_reset"] = func(_s: State, _out: Session.FrameResult) -> void: log.append("bank")
	return services

func check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		push_error("FAIL: " + message)
	else:
		print("PASS: " + message)
