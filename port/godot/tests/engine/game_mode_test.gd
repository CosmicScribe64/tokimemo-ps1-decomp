extends RefCounted
## T-39301: engine/game_mode.gd and engine/reset_iris_wipe.gd on invented memory. Every expectation is
## derived by hand from src/main/game_mode.c and src/main/reset_iris_wipe.c (not from the port).

const GameEngine = preload("res://engine/engine.gd")

var failures: Array[String] = []
var _made: Array = []

## Engines are shut down at the end of run() so the engine <-> module cycles are freed.
func _new():
	var engine = GameEngine.new()
	_made.append(engine)
	return engine

func _shutdown_all() -> void:
	for engine in _made:
		engine.shutdown()
	_made.clear()

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	_test_setters()
	_test_game_mode_set_and_next()
	_test_request_overlay()
	_test_dispatch()
	_test_reset_to_title()
	_test_digits()
	_shutdown_all()
	return failures

## A GameState with every field the mode functions touch set to a distinct nonzero value.
func _dirty(engine) -> Object:
	var gs = engine.memory.game_state()
	gs.set_field("game_mode", 0x34)
	gs.set_field("mode_stage", 3)
	gs.set_field("mode_step", 5)
	gs.set_field("game_mode_timer", 11)
	gs.set_field("mode_stage_timer", 12)
	gs.at("mode_step_timer").set_field("w", 13)
	gs.set_field("game_mode_scratch", 14)
	gs.set_field("mode_stage_scratch", 15)
	gs.set_field("substep", 16)
	gs.set_field("ovl_target_game_mode", 0x77)
	return gs

func _fields(gs) -> Array:
	return [gs.get_field("game_mode"), gs.get_field("mode_stage"), gs.get_field("mode_step"), gs.get_field("game_mode_timer"),
		gs.get_field("mode_stage_timer"), gs.at("mode_step_timer").get_field("w"), gs.get_field("game_mode_scratch"),
		gs.get_field("mode_stage_scratch"), gs.get_field("substep")]

func _test_setters() -> void:
	var engine = _new()
	var gs = _dirty(engine)
	_check(engine.call_c("mode_step_next") == 0, "mode_step_next returns 0 (z read through p)")
	_check(_fields(gs) == [0x34, 3, 6, 11, 12, 0, 14, 15, 0], "mode_step_next: step + 1, step timer and substep cleared, nothing else: %s" % [_fields(gs)])
	gs.set_field("mode_step", 0xFF)
	engine.call_c("mode_step_next")
	_check(gs.get_field("mode_step") == 0, "mode_step is a u8: 0xFF + 1 wraps to 0")
	gs = _dirty(engine)
	_check(engine.call_c("mode_stage_next") == 0, "mode_stage_next returns 0")
	_check(_fields(gs) == [0x34, 4, 0, 11, 0, 0, 14, 0, 0], "mode_stage_next: stage + 1, step, stage timer, step timer, stage scratch, substep cleared: %s" % [_fields(gs)])
	gs = _dirty(engine)
	engine.call_c("mode_stage_set", [0x1A5])
	_check(_fields(gs) == [0x34, 0xA5, 0, 11, 0, 0, 14, 0, 0], "mode_stage_set stores the low byte and clears step, stage timer, step timer, stage scratch, substep: %s" % [_fields(gs)])
	gs = _dirty(engine)
	engine.call_c("mode_step_set", [9])
	_check(_fields(gs) == [0x34, 3, 9, 11, 12, 0, 14, 15, 0], "mode_step_set stores the step and clears the step timer and substep: %s" % [_fields(gs)])

func _test_game_mode_set_and_next() -> void:
	var engine = _new()
	var shown: Array = []
	engine.set_stub("message_window_title_show", func(args: Array) -> int:
		shown.append(args)
		return 0)
	var gs = _dirty(engine)
	engine.memory.write_global("talk_voice_wait_frames", 99)
	engine.call_c("game_mode_set", [0x31])
	_check(_fields(gs) == [0x31, 0, 0, 0, 0, 0, 0, 0, 0], "game_mode_set(0x31) sets the mode and clears stage, step, all timers, scratches and substep: %s" % [_fields(gs)])
	_check(engine.memory.read_global("talk_voice_wait_frames") == 0, "game_mode_set clears talk_voice_wait_frames")
	_check(shown == [[0]], "target 0x77 != 0x5E, 0x50 and mode 0x31 is no overlay mode: message_window_title_show(0) runs")
	shown.clear()
	engine.call_c("game_mode_set", [0x50])
	_check(gs.get_field("game_mode") == 2 and gs.get_field("ovl_target_game_mode") == 0x50 and shown.is_empty(), "game_mode_set(0x50): request_overlay redirects to the loader (mode 2, target 0x50) and the 0x50 target shows no title")
	engine.call_c("game_mode_set", [0x5E])
	_check(gs.get_field("ovl_target_game_mode") == 0x5E and shown.is_empty(), "target 0x5E shows no title")
	engine.call_c("game_mode_set", [0x5F])
	_check(gs.get_field("ovl_target_game_mode") == 0x5F and shown == [[0]], "target 0x5F shows the title")
	gs = _dirty(engine)
	gs.set_field("game_mode", 0xFF)
	engine.call_c("game_mode_next")
	_check(gs.get_field("game_mode") == 0 and _fields(gs).slice(1) == [0, 0, 0, 0, 0, 0, 0, 0], "game_mode_next: 0xFF + 1 wraps to mode 0 and clears everything below it (mode 0 is no overlay mode)")
	gs.set_field("game_mode", 0x4F)
	engine.call_c("game_mode_next")
	_check(gs.get_field("game_mode") == 2 and gs.get_field("ovl_target_game_mode") == 0x50, "game_mode_next into 0x50 asks the overlay loader")

func _test_request_overlay() -> void:
	var engine = _new()
	var gs = engine.memory.game_state()
	var overlay_modes: Array = [0x11, 0x13, 0x14, 0x15, 0x20, 0x21, 0x22, 0x35, 0x36, 0x37, 0x3A, 0x43, 0x45, 0x50, 0x5E, 0x63, 0x70, 0x76, 0x80, 0x82, 0x90, 0x91, 0xC2, 0xC4]
	for mode: int in overlay_modes:
		gs.set_field("game_mode", mode)
		gs.set_field("ovl_target_game_mode", 0)
		engine.call_c("game_mode_request_overlay")
		_check(gs.get_field("game_mode") == 2 and gs.get_field("ovl_target_game_mode") == mode, "overlay mode 0x%02X is redirected to the loader" % mode)
	for mode: int in [0x00, 0x01, 0x02, 0x10, 0x12, 0x30, 0x34, 0x40, 0x46, 0x64, 0x83, 0xC1, 0xC5, 0xD0, 0xFF]:
		gs.set_field("game_mode", mode)
		gs.set_field("ovl_target_game_mode", 0x66)
		engine.call_c("game_mode_request_overlay")
		_check(gs.get_field("game_mode") == mode and gs.get_field("ovl_target_game_mode") == 0x66, "mode 0x%02X is not an overlay mode: unchanged" % mode)

func _test_dispatch() -> void:
	var engine = _new()
	var gs = engine.memory.game_state()
	var called: Array = []
	var stub: Callable = func(args: Array) -> int:
		called.append(args.duplicate())
		return 0
	var handlers: Dictionary = {0x00: "boot_mode_handler", 0xFF: "exit_function", 0xFE: "menu_vram_view_mode_handler",
		0x30: "schedule_init", 0x31: "schedule_day_screen_handler", 0x34: "settings_command_screen_mode_handler",
		0x12: "title_mode_handler", 0x40: "week_day", 0x46: "holiday_mode_handler", 0x44: "uwasa_main",
		0x41: "settings_new_year_prep_mode_handler", 0x42: "settings_christmas_prep_mode_handler",
		0x10: "game_mode_start_mode_handler", 0x38: "schedule_month_start_mode_handler",
		0x39: "schedule_day_notice_mode_handler", 0x32: "schedule_week_end_mode_handler",
		0x02: "ovl_loader_mode_handler", 0xD0: "game_mode_tt_game_mode_handler"}
	var names: Dictionary = {}
	for mode: int in handlers:
		names[handlers[mode]] = true
		engine.set_stub(handlers[mode], func(args: Array) -> int:
			called.append(handlers[mode])
			return 0)
	for mode: int in handlers:
		if mode == 0x00:
			continue # mode 0 is covered with mode 1 below
		called.clear()
		gs.set_field("game_mode", mode)
		gs.set_field("game_mode_timer", 41)
		engine.call_c("game_mode_dispatch")
		_check(called == [handlers[mode]] and gs.get_field("game_mode_timer") == 42, "mode 0x%02X calls %s once, then game_mode_timer + 1" % [mode, handlers[mode]])
	called.clear()
	gs.set_field("game_mode", 0)
	gs.set_field("game_mode_timer", 0x7FFFFFFF)
	engine.call_c("game_mode_dispatch")
	_check(called == ["boot_mode_handler"] and gs.get_field("game_mode_timer") == -0x80000000, "mode 0 calls boot_mode_handler; the s32 timer wraps")
	# mode 1: waits for boot_hold
	engine.clear_stubs()
	engine.set_stub("message_window_title_show", stub)
	gs.set_field("game_mode", 1)
	gs.set_field("boot_hold", 1)
	gs.set_field("game_mode_timer", 5)
	engine.call_c("game_mode_dispatch")
	_check(gs.get_field("game_mode") == 1 and gs.get_field("game_mode_timer") == 6, "mode 1 with boot_hold != 0 only counts the timer")
	gs.set_field("boot_hold", 0)
	engine.call_c("game_mode_dispatch")
	_check(gs.get_field("game_mode") == 0 and gs.get_field("game_mode_timer") == 1, "mode 1 with boot_hold == 0 sets mode 0 (timer cleared by game_mode_set, then + 1)")
	# overlay modes: "LOAD FUNC EXEC", then the overlay entry through ovl_entry
	engine.clear_failure()
	engine.clear_stubs()
	called.clear()
	engine.set_stub("debug_print_string", func(args: Array) -> int:
		called.append(["debug_print_string", args.duplicate()])
		return 0)
	engine.set_stub("ovl_entry", func(args: Array) -> int:
		called.append("ovl_entry")
		return 0)
	engine.memory.write_global("ovl_entry", 0x80132000)
	gs.set_field("game_mode", 0x22)
	gs.set_field("game_mode_timer", 9)
	engine.call_c("game_mode_dispatch")
	_check(called == [["debug_print_string", [0x800AF390, 0x14, 0x2C]], "ovl_entry"] and gs.get_field("game_mode_timer") == 10,
		"overlay mode 0x22 prints LOAD FUNC EXEC (0x800AF390, 0x14, 0x2C) and calls ovl_entry: %s" % [called])
	called.clear()
	gs.set_field("game_mode", 3)
	engine.call_c("game_mode_dispatch")
	_check(called == ["ovl_entry"], "mode 3 calls ovl_entry without the debug print")
	# the dispatch refuses (and does not count the timer) when ovl_entry has no port
	engine.clear_stubs()
	gs.set_field("game_mode_timer", 70)
	engine.call_c("game_mode_dispatch")
	_check(engine.refusal.contains("0x80132000") and gs.get_field("game_mode_timer") == 70, "an unported overlay entry refuses and the timer is not counted")
	# any other mode resets to the title (its first unported callee refuses)
	engine.clear_failure()
	gs.set_field("game_mode", 0x99)
	engine.call_c("game_mode_dispatch")
	_check(engine.refusal.contains("game_mode_reset_to_title -> ovl_load_reset"), "an unknown mode calls game_mode_reset_to_title: %s" % engine.refusal)

func _test_reset_to_title() -> void:
	var engine = _new()
	var order: Array = []
	for name: String in ["ovl_load_reset", "game_state_init", "sprite_tables_clear", "actor_deactivate", "back_clear_switch",
			"dec_bg_reset", "audio_bank_state_reset", "tpage_buf_clear", "audio_command", "audio_shutdown", "audio_init",
			"boot_game_data_init", "boot_settings_defaults_set", "message_window_title_show"]:
		engine.set_stub(name, func(args: Array) -> int:
			order.append([name, args.duplicate()])
			return 0)
	var gs = _dirty(engine)
	engine.memory.write_global("cd_queue_tail", 9)
	var result: int = engine.call_c("game_mode_reset_to_title")
	var expected: Array = [["ovl_load_reset", []], ["game_state_init", [0]], ["message_window_title_show", [0]], ["sprite_tables_clear", []],
		["actor_deactivate", [0]], ["back_clear_switch", [1]], ["dec_bg_reset", []], ["audio_bank_state_reset", []],
		["tpage_buf_clear", []], ["audio_command", [0x7F]], ["audio_shutdown", []], ["audio_init", []],
		["boot_game_data_init", []], ["boot_settings_defaults_set", []]]
	_check(result == -1 and order == expected, "game_mode_reset_to_title calls the C sequence in order and returns -1: %s" % [order])
	_check(gs.get_field("game_mode") == 0x12 and engine.memory.read_global("cd_queue_tail") == 0 and engine.memory.read_global("cd_read_status") == 2 \
			and engine.memory.read_global("cd_queue_phase") == -1, "title mode 0x12 set and cd_command_reset ran (ported, in order)")

func _test_digits() -> void:
	var engine = _new()
	for case: Array in [[1234, 1, 4], [1234, 2, 3], [1234, 3, 2], [1234, 4, 1], [1234, 5, 0], [1234, 6, 0], [0, 3, 0], [-1234, 1, -4], [-1234, 4, -1],
			[0x18000, 1, -8], [0xFFFF, 1, -1], [99, 255, 0]]:
		_check(engine.call_c("cd_decimal_digit", [case[0], case[1]]) == case[2], "cd_decimal_digit(%d, %d) = %d" % case)
	_check(engine.refusal.is_empty(), "no refusal for digit places 1..255")
	engine.call_c("cd_decimal_digit", [5, 0])
	_check(engine.refusal.contains("division by zero"), "digit place 0 divides by zero: refused, not invented: %s" % engine.refusal)
	engine.clear_failure()
	for case: Array in [[0x00, 0], [0x09, 9], [0x10, 10], [0x59, 59], [0x74, 74], [0x99, 99], [0xFF, 165]]:
		_check(engine.call_c("cd_binary_coded_decimal_to_integer", [case[0]]) == case[1], "cd_binary_coded_decimal_to_integer(0x%02X) = %d" % case)
