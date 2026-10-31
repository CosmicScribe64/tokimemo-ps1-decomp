extends RefCounted
## T-39312: engine/scene_library.gd on invented memory. Every expectation is derived by hand from
## src/main/scene_library.c (not from the port); the emulator captures are replayed by the differential harness
## (config/port/engine-diff/scene_library.json).

const GameEngine = preload("res://engine/engine.gd")

const QUAD_SIZE: int = 0x24
const ACTOR_SIZE: int = 0x44

var failures: Array[String] = []
var _made: Array = []

func _new():
	var engine = GameEngine.new()
	_made.append(engine)
	return engine

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	_test_step_functions()
	_test_sprite_brightness()
	_test_check_para_limit()
	_test_event_log_add()
	_test_day_plus()
	_test_cd_level()
	_test_don_wait()
	_test_simple_scene_commands()
	_test_waits_and_selects()
	_test_choice_menus()
	_test_talk_speak_part()
	_shutdown()
	return failures

func _shutdown() -> void:
	for engine in _made:
		engine.shutdown()
	_made.clear()

## The one-step scene commands: store(s), then mode_step_next (mode_step + 1, step timer and substep cleared).
func _test_step_functions() -> void:
	var engine = _new()
	var m = engine.memory
	engine.gs_set("date_girl", 7)
	engine.gs_set("current_girl", 2)
	engine.gs_set("mode_step", 4)
	engine.call_c("change_girl")
	_check(engine.gs_get("current_girl") == 7 and engine.gs_get("mode_step") == 5, "change_girl: current_girl = date_girl, then the next step")
	engine.call_c("change_yoshio")
	_check(engine.gs_get("current_girl") == 13 and engine.gs_get("mode_step") == 6, "change_yoshio: current_girl = GIRL_YOSHIO (13)")
	m.write_global("D_80120666", 99)
	m.write_global("D_801206AA", 5)
	engine.call_c("event_face0")
	_check(m.read_global("D_80120666") == 2 and m.read_global("D_801206AA") == 0 and engine.gs_get("mode_step") == 7, "event_face0: D_80120666 = 2, D_801206AA = 0")
	m.write_global("D_80120666", 99)
	engine.call_c("event_face_kuchi0")
	_check(m.read_global("D_80120666") == 2 and engine.gs_get("mode_step") == 8, "event_face_kuchi0: D_80120666 = 2")
	m.write_global("D_80122D10", 9)
	m.write_global("D_80120652", 9)
	engine.call_c("event_kuchi_ani_no")
	_check(m.read_global("D_80122D10") == 0 and m.read_global("D_80120652") == 4 and engine.gs_get("mode_step") == 9, "event_kuchi_ani_no: D_80122D10 = 0, D_80120652 = 4")
	# hizuke_hide: hizuke_disp_switch(0), then the next step
	var pool: int = m.global_address("actor_pool")
	m.write_u8(pool + 16 * ACTOR_SIZE + 3, 0xFF)
	engine.call_c("hizuke_hide")
	_check(m.u8(pool + 16 * ACTOR_SIZE + 3) == 0x7F and engine.gs_get("mode_step") == 10, "hizuke_hide clears bit 7 of actor 16 and steps")
	# face_expression_N: portrait_layout_set(N), then the next step (stubbed here)
	var seen: Array = []
	engine.set_stub("portrait_layout_set", func(args: Array) -> int:
		seen.append(args.duplicate())
		return 0)
	for index: int in 5:
		engine.call_c("face_expression_%d" % index)
	_check(seen == [[0], [1], [2], [3], [4]] and engine.gs_get("mode_step") == 15, "face_expression_0..4 call portrait_layout_set(0..4) and step: %s" % [seen])
	seen.clear()
	engine.set_stub("get_g_zyotai_h", func(args: Array) -> int:
		return 0x185 if args == [6] else -1)
	engine.gs_set("current_girl", 6)
	engine.call_c("face_expression_mood")
	_check(seen == [[5]], "face_expression_mood passes get_g_zyotai_h(current_girl) & 0x7F: %s" % [seen])

func _test_sprite_brightness() -> void:
	var engine = _new()
	var m = engine.memory
	var base: int = m.global_address("sprite_quads")
	engine.call_c("sprite_brightness", [3, 0x1A5]) # u8 parameter: 0xA5
	for offset: int in [0x14, 0x15, 0x16]:
		_check(m.u8(base + 3 * QUAD_SIZE + offset) == 0xA5, "sprite_brightness writes the low byte of the brightness at +0x%X" % offset)
	_check(m.u8(base + 3 * QUAD_SIZE + 0x17) == 0 and m.u8(base + 2 * QUAD_SIZE + 0x16) == 0, "and nothing around it")
	engine.call_c("sprite_brightness", [-1, 7]) # s16 index -1: the previous record (index * 36)
	_check(m.u8(base - QUAD_SIZE + 0x14) == 7, "a negative s16 index addresses backwards")

func _test_check_para_limit() -> void:
	var engine = _new()
	var m = engine.memory
	var base: int = engine.gs_address("param_condition")
	# nine words: a negative s16 value (high half) -> 0; a value >= 0x3E7 -> word 0x03E70000; otherwise unchanged
	var words: Array = [0xFFFF0007, 0x03E70000, 0x03E80001, 0x03E60005, 0x00000000, 0x7FFF0000, 0x80000000, 0x00010002, 0x03E7FFFF]
	var want: Array = [0, 0x03E70000, 0x03E70000, 0x03E60005, 0, 0x03E70000, 0, 0x00010002, 0x03E70000]
	for i: int in 9:
		m.write_u32(base + 4 * i, words[i])
	engine.call_c("check_para_limit")
	for i: int in 9:
		_check(m.u32(base + 4 * i) == want[i], "check_para_limit word %d: 0x%X -> 0x%X, got 0x%X" % [i, words[i], want[i], m.u32(base + 4 * i)])

func _test_event_log_add() -> void:
	var engine = _new()
	var m = engine.memory
	var log_base: int = engine.gs_address("event_log")
	var override: int = engine.modules.scene_library.G_TALK_RANK_OVERRIDE # not in the generated layout (T-39312 ticket: missing data symbols)
	engine.set_stub("get_g_zyotai_h", func(_args: Array) -> int: return 3)
	engine.gs_set("current_girl", 5)
	engine.gs_set("year", 2)
	engine.gs_set("month", 11)
	engine.gs_set("day", 23)
	engine.gs_set("weekday", 4)
	engine.gs_set("event_log_count", 10)
	engine.call_c("event_log_add", [0x1FF, 0x102]) # u8 parameters: 0xFF and 0x02
	var rec: int = log_base + 10 * 8
	_check([m.u8(rec), m.u8(rec + 1), m.u8(rec + 2), m.u8(rec + 3), m.u8(rec + 4), m.u8(rec + 5), m.u8(rec + 6), m.u8(rec + 7)] == [5, 2, 0xFF, 3, 2, 11, 23, 4], "event_log_add record: girl, flag, scene, rank (zyotai 3), year, month, day, weekday")
	_check(engine.gs_get("event_log_count") == 11 and m.s32(override) == 0, "the count advances and talk_rank_override is cleared")
	# talk_rank_override 1..6 replaces the rank by override - 1; 7 and above are ignored
	m.write_u32(override, 4)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 11 * 8 + 3) == 3, "override 4 gives rank 3 (override - 1)")
	m.write_u32(override, 6)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 12 * 8 + 3) == 5, "override 6 gives rank 5")
	m.write_u32(override, 7)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 13 * 8 + 3) == 3, "override 7 is ignored: the zyotai rank")
	# the rival (GIRL_REI = 12) always gets rank 0
	engine.gs_set("current_girl", 12)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 14 * 8 + 3) == 0, "GIRL_REI: rank 0")
	# the count saturates at 0xFE when it reaches 0xFF
	engine.gs_set("event_log_count", 0xFD)
	engine.call_c("event_log_add", [1, 0])
	_check(engine.gs_get("event_log_count") == 0xFE, "0xFD + 1 = 0xFE stays")
	engine.call_c("event_log_add", [1, 0])
	_check(engine.gs_get("event_log_count") == 0xFE, "0xFE + 1 = 0xFF is replaced by 0xFE")

func _test_day_plus() -> void:
	var engine = _new()
	var order: Array = []
	for name: String in ["calendar_advance_day", "hizuke_init", "hizuke_disp_switch", "hizuke_show"]:
		engine.set_stub(name, func(args: Array) -> int:
			order.append([name, args.duplicate()])
			return 0)
	engine.memory.write_global("scene_trip_day_state", 5)
	engine.memory.write_global("D_8011F113", 0x80)
	engine.gs_set("mode_step", 1)
	engine.call_c("day_plus")
	_check(order == [["calendar_advance_day", []], ["hizuke_init", []], ["hizuke_disp_switch", [1]], ["hizuke_show", []]], "day_plus with the date shown: %s" % [order])
	_check(engine.memory.read_global("scene_trip_day_state") == 0 and engine.gs_get("mode_step") == 2, "scene_trip_day_state cleared, next step")
	order.clear()
	engine.memory.write_global("D_8011F113", 0x7F)
	engine.call_c("day_plus")
	_check(order[2] == ["hizuke_disp_switch", [0]], "day_plus with the date hidden passes 0: %s" % [order])

func _test_cd_level() -> void:
	var engine = _new()
	var m = engine.memory
	m.write_global("audio_cd_level", 0x30)
	m.write_global("D_80125E62", 0x11)
	m.write_u32(m.global_address("audio_state"), 0)
	_check(engine.call_c("getCDlevel") == 0, "getCDlevel is 0 without bit 0x400 of state_flags")
	m.write_u32(m.global_address("audio_state"), 0x400)
	_check(engine.call_c("getCDlevel") == (0x30 + 0x11) >> 1, "getCDlevel = (audio_cd_level + D_80125E62) >> 1")
	m.write_global("audio_cd_level", 0xFFFF)
	m.write_global("D_80125E62", 0xFFFF)
	_check(engine.call_c("getCDlevel") == -1, "(u32)(0xFFFF + 0xFFFF) >> 1 = 0xFFFF, returned as an s16: -1")

## don_wait: after 0x81 frames of the wait counter, k_reset(1) and the next step.
func _test_don_wait() -> void:
	var engine = _new()
	var seen: Array = []
	engine.set_stub("k_reset", func(args: Array) -> int:
		seen.append(args.duplicate())
		return 0)
	var counter: int = engine.gs_address("mode_step_timer")
	engine.gs_set("mode_step", 3)
	engine.memory.write_u32(counter, 0x7F)
	engine.call_c("don_wait")
	_check(seen.is_empty() and engine.memory.u32(counter) == 0x80 and engine.gs_get("mode_step") == 3, "don_wait counts up below 0x81 and does nothing else")
	engine.memory.write_u32(counter, 0x80) # the test reads the old value (0x80) +1 -> 0x81
	engine.call_c("don_wait")
	_check(seen.is_empty(), "the old value 0x80 is still below 0x81")
	engine.call_c("don_wait")
	_check(seen == [[1]] and engine.gs_get("mode_step") == 4, "at 0x81: k_reset(1), then the next step: %s step %d" % [seen, engine.gs_get("mode_step")])

## The one-step scene commands that only store constants (or call one function) and step.
func _test_simple_scene_commands() -> void:
	var engine = _new()
	var m = engine.memory
	var step := func() -> int: return engine.gs_get("mode_step")
	var cases: Array = [
		["event_face1", {"D_80120666": 3, "D_801206AA": 1}], ["event_face2", {"D_80120666": 6, "D_801206AA": 4}],
		["event_face3", {"D_80120666": 7, "D_801206AA": 5}], ["event_face_kuchi1", {"D_80120666": 3}],
		["event_kuchi_ani_yes", {"D_80122D10": 1, "D_80120652": 0}], ["scene_mouth_lock_clear", {"D_80122D20": 0}],
		["ev_me_on", {"D_80122D0C": 0, "D_80120696": 5}], ["ev_me_on_continue", {"D_80122D0C": 1, "D_80120696": 5}],
		["scene_bg_show", {"D_80122CF8": 1, "bg_brightness": 0x80}], ["scene_bg_hide", {"D_80122CF8": 0, "bg_brightness": 0}],
	]
	for c: Array in cases:
		for global_name: String in c[1]:
			m.write_global(global_name, 0x55 if c[1][global_name] != 0x55 else 0x56) # a value the function must overwrite
		var before: int = step.call()
		engine.call_c(c[0])
		for global_name: String in c[1]:
			_check(m.read_global(global_name) == c[1][global_name], "%s: %s = %d, got %d" % [c[0], global_name, c[1][global_name], m.read_global(global_name)])
		_check(step.call() == before + 1, "%s ends with mode_step_next" % c[0])
	# hizuke_appear: hizuke_disp_switch(1), then the next step
	var pool: int = m.global_address("actor_pool")
	engine.call_c("hizuke_appear")
	_check(m.u8(pool + 16 * ACTOR_SIZE + 3) == 0x80 and m.u8(pool + 23 * ACTOR_SIZE + 3) == 0x80 and m.u8(pool + 24 * ACTOR_SIZE + 3) == 0, "hizuke_appear sets bit 7 of actors 16..23")
	# vram_bustup_clear: actor_clear(0x1F), actor_clear(0x1E), tpage_buf[5..9] = -1, then the next step
	var cleared: Array = []
	engine.set_stub("actor_clear", func(args: Array) -> int:
		cleared.append(args[0])
		return 0)
	var tpage: int = engine.gs_address("tpage_buf")
	for i: int in 12:
		m.write_u32(tpage + i * 4, 0x77)
	engine.call_c("vram_bustup_clear")
	_check(cleared == [0x1F, 0x1E], "vram_bustup_clear clears actors 31 and 30: %s" % [cleared])
	for i: int in 12:
		_check(m.s32(tpage + i * 4) == (-1 if i >= 5 and i < 10 else 0x77), "tpage_buf[%d] is -1 only for 5..9" % i)
	# yosi_trans: audio_bank_start(0, 0x200), then the next step; place_init2: k_reset(0), message_window_set_title(scene_place_title_2)
	var seen: Array = []
	for name: String in ["audio_bank_start", "k_reset", "message_window_set_title", "portrait_quad_offsets"]:
		engine.set_stub(name, func(args: Array) -> int:
			seen.append([name, args.duplicate()])
			return 0)
	engine.call_c("yosi_trans")
	engine.call_c("place_init2")
	_check(seen == [["audio_bank_start", [0, 0x200]], ["k_reset", [0]], ["message_window_set_title", [m.global_address("scene_place_title_2")]]], "yosi_trans, place_init2: %s" % [seen])
	# portrait_layout_set(u8): portrait_quad_offsets(arg), then portrait_value_clear_if_high (which takes no argument)
	seen.clear()
	m.write_global("D_800B5A64", 0xB0)
	engine.call_c("portrait_layout_set", [0x1FF])
	_check(seen == [["portrait_quad_offsets", [0xFF]]] and m.read_global("D_800B5A64") == 0, "portrait_layout_set(0x1FF): the u8 argument is 0xFF, then the value is cleared: %s" % [seen])

func _test_waits_and_selects() -> void:
	var engine = _new()
	var m = engine.memory
	# return_step: game_mode_set(saved_game_mode); mode_stage_set(saved_mode_stage); mode_step_set(saved_mode_step)
	var seen: Array = []
	for name: String in ["game_mode_set", "mode_stage_set", "mode_step_set", "select_girl_init", "select_girl_main", "game_mode_reset_to_title", "talk_run"]:
		engine.set_stub(name, func(args: Array) -> int:
			seen.append([name, args.duplicate()])
			return 1 if name == "talk_run" and seen.size() % 2 == 0 else 0)
	engine.gs_set("saved_game_mode", 0x45)
	engine.gs_set("saved_mode_stage", 3)
	engine.gs_set("saved_mode_step", 9)
	engine.call_c("return_step")
	_check(seen == [["game_mode_set", [0x45]], ["mode_stage_set", [3]], ["mode_step_set", [9]]], "return_step: %s" % [seen])
	# select_girl / select_girl2: mode_step_timer 0 -> select_girl_init(0/1), 1 -> select_girl_main(0/1), else game_mode_reset_to_title
	var timer: int = engine.gs_address("mode_step_timer")
	for variant: int in 2:
		var name: String = "select_girl" if variant == 0 else "select_girl2"
		for state: int in 3:
			seen.clear()
			m.write_u32(timer, state)
			engine.call_c(name)
			var want: Array = [["select_girl_init", [variant]]] if state == 0 else ([["select_girl_main", [variant]]] if state == 1 else [["game_mode_reset_to_title", []]])
			_check(seen == want, "%s with the step timer %d: %s" % [name, state, seen])
	# wait_sub_sub(s16 frames): the step timer counts up and (u32) elapsed >= (u32) frames steps; a negative s16 is a huge u32
	engine.clear_stubs()
	engine.gs_set("mode_step", 5)
	m.write_u32(timer, 0)
	engine.call_c("wait_sub_sub", [3])
	engine.call_c("wait_sub_sub", [3])
	_check(m.u32(timer) == 2 and engine.gs_get("mode_step") == 5, "after 2 of 3 frames nothing steps")
	engine.call_c("wait_sub_sub", [3])
	_check(engine.gs_get("mode_step") == 6 and m.u32(timer) == 0, "the 3rd frame steps (and mode_step_next clears the timer)")
	engine.call_c("wait_sub_sub", [-1])
	_check(engine.gs_get("mode_step") == 6 and m.u32(timer) == 1, "frames = -1 compares as 0xFFFFFFFF: no step")
	# normal_date_speak_1line: the table pointers, then talk_run(talk_choice, 1, 0); step when it returns 1
	var runs: Array = []
	var result: Array = [0]
	engine.set_stub("talk_run", func(args: Array) -> int:
		runs.append(args.duplicate())
		return result[0])
	m.write_global("talk_choice", 2)
	engine.gs_set("mode_step", 1)
	engine.call_c("normal_date_speak_1line")
	_check(runs == [[2, 1, 0]] and engine.gs_get("mode_step") == 1, "talk_run(talk_choice, 1, 0) returning 0 does not step: %s" % [runs])
	result[0] = 1
	engine.call_c("normal_date_speak_1line")
	_check(engine.gs_get("mode_step") == 2, "talk_run returning 1 steps")

## junban_init: talk_choice_order[0] = frame_count % 3, then the other two entries (the C formulas per case).
func _test_choice_order() -> void:
	var engine = _new()
	var order: int = engine.memory.global_address("talk_choice_order")
	# frame_count -> [order0, order1, order2]: F = 3: 0, (3 & 1) + 1 = 2, (4 & 1) + 1 = 1; F = 6: 0, 1, 2;
	# F = 4: 1, (4 & 1) * 2 = 0, (5 & 1) * 2 = 2; F = 7: 1, 2, 0; F = 5: 2, 1 - (5 & 1) = 0, 1 - (6 & 1) = 1; F = 8: 2, 1, 0
	var cases: Dictionary = {3: [0, 2, 1], 6: [0, 1, 2], 4: [1, 0, 2], 7: [1, 2, 0], 5: [2, 0, 1], 8: [2, 1, 0]}
	for frame: int in cases:
		engine.gs_set("frame_count", frame)
		engine.call_c("junban_init")
		var got: Array = [engine.memory.u8(order), engine.memory.u8(order + 1), engine.memory.u8(order + 2)]
		_check(got == cases[frame], "junban_init at frame %d: %s, wanted %s" % [frame, got, cases[frame]])

## The 2- and 3-choice menus of the date dialogue: init (menu_set with the arrays in the frame), main (hover to choice).
func _test_choice_menus() -> void:
	_test_choice_order()
	var engine = _new()
	var m = engine.memory
	var seen: Array = []
	var frames: Array = []
	engine.set_stub("menu_set", func(args: Array) -> int:
		var row: Array = [args[0], args[1]]
		for pointer: int in args.slice(2, 6):
			var values: Array = []
			for i: int in args[1]:
				values.append(m.s16(pointer + i * 2))
			row.append(values)
		seen.append(row)
		frames.append(engine.frame_base())
		return 0)
	engine.gs_set("menu_cursor_1", 5)
	m.write_u32(engine.gs_address("mode_step_timer"), 7)
	engine.call_c("normal_date_two_select_init")
	_check(seen == [[1, 2, [-0x80, -0x80], [0x30, 0x40], [0xFC, 0xFC], [0x10, 0x10]]], "two-choice menu: %s" % [seen])
	_check(engine.gs_get("menu_cursor_1") == 0 and m.read_global("talk_menu_active") == 1 and m.u32(engine.gs_address("mode_step_timer")) == 8, "cursor 0, talk_menu_active 1, step timer + 1")
	seen.clear()
	engine.call_c("normal_date_three_select_init")
	_check(seen == [[1, 3, [-0x80, -0x80, -0x80], [0x30, 0x40, 0x50], [0xFC, 0xFC, 0xFC], [0x10, 0x10, 0x10]]], "three-choice menu: %s" % [seen])
	# main: menu_check(1, cursor_x, cursor_y), menu_cursor_update(1), menu_bar_show(1), then on a confirm press with check_end_k != 0
	# the hovered item 1/3/5 (record index) sets talk_choice (order[k] for three choices), clears bit 7 of D_8011ECD3, talk_menu_active, k_reset(1), next step
	var calls: Array = []
	var ended: Array = [1]
	for name: String in ["menu_check", "menu_cursor_update", "menu_bar_show", "k_reset"]:
		engine.set_stub(name, func(args: Array) -> int:
			calls.append([name, args.duplicate()])
			return 0)
	engine.set_stub("check_end_k", func(_args: Array) -> int: return ended[0])
	var items: int = engine.gs_address("menu_items")
	m.write_global("cursor_x", -5)
	m.write_global("cursor_y", 9)
	var order: int = m.global_address("talk_choice_order")
	m.write_u8(order, 2)
	m.write_u8(order + 1, 0)
	m.write_u8(order + 2, 1)
	engine.gs_set("mode_step", 4)
	engine.gs_set("pad_pressed", 0x20) # bit 0x20 of 0x200020
	for k: int in 3: # which item is hovered: records 1, 3, 5 (8-byte records, hover at +6)
		for i: int in 8:
			m.write_u8(items + i * 8 + 6, 1 if i == 2 * k + 1 else 0)
		calls.clear()
		m.write_global("talk_choice", -1)
		m.write_global("D_8011ECD3", 0xFF)
		m.write_global("talk_menu_active", 1)
		engine.call_c("normal_date_three_select_main")
		_check(m.read_global("talk_choice") == [2, 0, 1][k], "three-choice menu: item %d gives order[%d]" % [2 * k + 1, k])
		_check(calls == [["menu_check", [1, -5, 9]], ["menu_cursor_update", [1]], ["menu_bar_show", [1]], ["k_reset", [1]]], "calls of the confirm path: %s" % [calls])
		_check(m.read_global("D_8011ECD3") == 0x7F and m.read_global("talk_menu_active") == 0, "bit 7 of D_8011ECD3 cleared, talk_menu_active 0")
	_check(engine.gs_get("mode_step") == 7, "each confirm stepped once")
	# no hovered item among 1, 3, 5: return 0 without any of the stores
	for i: int in 8:
		m.write_u8(items + i * 8 + 6, 0)
	m.write_global("talk_choice", 77)
	calls.clear()
	_check(engine.call_c("normal_date_three_select_main") == 0 and m.read_global("talk_choice") == 77 and calls.size() == 3, "no hover: nothing is chosen")
	# not confirmed (pad bit missing) or check_end_k == 0: only the three per-frame calls
	engine.gs_set("pad_pressed", 0x01)
	calls.clear()
	engine.call_c("normal_date_two_select_main")
	_check(calls.size() == 3, "no confirm press: only menu_check, menu_cursor_update, menu_bar_show")
	engine.gs_set("pad_pressed", 0x200000) # the other bit of the mask
	ended[0] = 0
	calls.clear()
	engine.call_c("normal_date_two_select_main")
	_check(calls.size() == 3, "check_end_k() == 0: nothing is chosen")
	# two choices: items 1 -> 0, 3 -> 1
	ended[0] = 1
	m.write_u8(items + 1 * 8 + 6, 0)
	m.write_u8(items + 3 * 8 + 6, 1)
	engine.call_c("normal_date_two_select_main")
	_check(m.read_global("talk_choice") == 1, "two-choice menu: item 3 gives choice 1")
	# the dispatchers: mode_step_timer 0 -> init, 1 -> main, otherwise game_mode_reset_to_title
	var routed: Array = []
	for name: String in ["normal_date_two_select_init", "normal_date_two_select_main", "normal_date_three_select_init", "normal_date_three_select_main", "game_mode_reset_to_title"]:
		engine.set_stub(name, func(_args: Array) -> int:
			routed.append(name)
			return 0)
	for state: int in 3:
		m.write_u32(engine.gs_address("mode_step_timer"), state)
		engine.call_c("normal_date_two_select")
		engine.call_c("normal_date_three_select")
	_check(routed == ["normal_date_two_select_init", "normal_date_three_select_init", "normal_date_two_select_main", "normal_date_three_select_main", "game_mode_reset_to_title", "game_mode_reset_to_title"], "select dispatch: %s" % [routed])

## talk_speak_part: the simple speak codes. The text is copied into a 0x200-byte local at sp + 0x7C (strcpy, unless the
## code is 0xFF), dialogue->voice_state and talk_voice_waiting are cleared, and temp_s1 = row_offset * 0x10 + 0x30 (s16).
func _test_talk_speak_part() -> void:
	var engine = _new()
	var m = engine.memory
	var talk_work: int = engine.modules.scene_library.G_TALK_WORK
	var text: int = 0x80100000
	m.write_bytes(text, "ABC".to_ascii_buffer() + PackedByteArray([0]))
	var renders: Array = []
	var texts: Array = []
	var results: Array = [0x1234]
	engine.set_stub("talk_render", func(args: Array) -> int:
		renders.append([args[0], args[1], args[2], args[4], args[7]])
		texts.append(m.read_bytes(args[3] & 0xFFFFFFFF, 4).hex_encode()) # the C passes (s32) address: a negative int
		return results[0])
	var other: Array = []
	for name: String in ["k_disp_start", "game_mode_reset_to_title"]:
		engine.set_stub(name, func(args: Array) -> int:
			other.append([name, args.duplicate()])
			return 0)
	m.write_u32(talk_work + 0x58, 2) # row_offset: temp_s1 = 2 * 0x10 + 0x30 = 0x50
	m.write_u32(talk_work + 0x20, 9) # voice_state: cleared by the call
	m.write_global("talk_voice_waiting", 1)
	m.write_global("talk_next_row_y", 0x66)
	_check(engine.call_c("talk_speak_part", [text, 0x10, 0, 0, 0, 0, 3]) == 0, "speak code 0x10 returns 0")
	_check(renders == [[-0x80, 0x50, 3, 0, 0]] and texts == ["41424300"] and m.u32(talk_work + 0x30) == 1, "0x10: talk_render(-0x80, 0x50, color, text, 0, .., .., 0), line_auto = 1: %s %s" % [renders, texts])
	_check(m.u32(talk_work + 0x20) == 0 and m.read_global("talk_voice_waiting") == 0, "voice_state and talk_voice_waiting are cleared first")
	renders.clear()
	m.write_u32(talk_work + 0x30, 0)
	engine.call_c("talk_speak_part", [text, 0x11, 0, 0, 0, 0, 3])
	_check(renders == [[-0x48, 0x66, 3, 0, 0]] and m.u32(talk_work + 0x30) == 1, "0x11 renders at talk_next_row_y: %s" % [renders])
	renders.clear()
	var order: int = m.global_address("talk_choice_order")
	m.write_u8(order, 7)
	m.write_u8(order + 1, 7)
	engine.call_c("talk_speak_part", [text, 0x8, 0, 0, 0, 0, 4])
	_check(renders == [[-0x72, 0x50, 4, 0, 1]] and other == [["k_disp_start", [0x1234]]] and m.u8(order) == 0 and m.u8(order + 1) == 1, "0x8: two-choice order 0, 1 and k_disp_start(talk_render(...)): %s %s" % [renders, other])
	renders.clear()
	other.clear()
	m.write_u8(order, 7)
	engine.call_c("talk_speak_part", [text, 0x13, 0, 0, 0, 0, 4])
	_check(renders == [[-0x72, 0x50, 4, 0, 1]] and other == [["k_disp_start", [0x1234]]] and m.u8(order) == 0, "0x13: the same with the other brackets")
	# 0xFF: no copy, mode_step_next; an unknown code resets to the title
	renders.clear()
	other.clear()
	engine.gs_set("mode_step", 2)
	_check(engine.call_c("talk_speak_part", [text, 0xFF, 0, 0, 0, 0, 3]) == 0 and engine.gs_get("mode_step") == 3 and renders.is_empty(), "0xFF steps and renders nothing")
	engine.call_c("talk_speak_part", [text, 0x55, 0, 0, 0, 0, 3])
	_check(other == [["game_mode_reset_to_title", []]], "an unknown speak code resets to the title: %s" % [other])
	# the speak code is a u8 parameter: 0x110 is 0x10
	renders.clear()
	engine.call_c("talk_speak_part", [text, 0x110, 0, 0, 0, 0, 3])
	_check(renders.size() == 1 and renders[0][0] == -0x80, "the speak code narrows to u8 (0x110 -> 0x10)")
