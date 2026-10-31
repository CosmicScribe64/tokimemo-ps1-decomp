extends RefCounted
## T-39312: engine/scene_display.gd on invented memory. Every expectation is derived by hand from
## src/main/scene_display.c (not from the port); the emulator captures are replayed by the differential harness
## (config/port/engine-diff/scene_display.json).

const GameEngine = preload("res://engine/engine.gd")

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
	_test_timer_and_fades()
	_test_screen_reset()
	_test_rot_2d()
	_test_small_functions()
	_test_audio_latch()
	_test_select_girl_by_scores()
	for engine in _made:
		engine.shutdown()
	_made.clear()
	return failures

func _test_timer_and_fades() -> void:
	var engine = _new()
	var m = engine.memory
	engine.gs_set("timer_ticks", 100)
	engine.call_c("Vblnk_Timer_Init")
	_check(m.read_global("vsync_timer_start") == 100 and m.read_global("vsync_timer_elapsed") == 0, "Vblnk_Timer_Init: start = timer_ticks, elapsed = 0")
	engine.gs_set("timer_ticks", 107)
	_check(engine.call_c("Vblnk_Timer") == 7 and m.read_global("vsync_timer_elapsed") == 7, "Vblnk_Timer returns and stores timer_ticks - start")
	engine.gs_set("timer_ticks", 0x7FFFFFFF) # s32 subtraction wraps
	m.write_global("vsync_timer_start", -2)
	_check(engine.call_c("Vblnk_Timer") == -0x80000000 + 1, "the elapsed time wraps like the s32 subtraction: got %d" % engine.call_c("Vblnk_Timer"))
	# Fade_Out_Main: a new tick interval (elapsed != 0): brightness < 3 -> 0 and return 1, else brightness -= 4 (u8) and restart the timer
	engine.gs_set("timer_ticks", 50)
	m.write_global("vsync_timer_start", 50)
	engine.gs_set("screen_brightness", 0x40)
	_check(engine.call_c("Fade_Out_Main") == 0 and engine.gs_get("screen_brightness") == 0x40, "no new tick: nothing changes, returns 0")
	m.write_global("vsync_timer_start", 49)
	_check(engine.call_c("Fade_Out_Main") == 0 and engine.gs_get("screen_brightness") == 0x3C and m.read_global("vsync_timer_start") == 50, "Fade_Out_Main: 0x40 -> 0x3C, timer restarted, returns 0")
	m.write_global("vsync_timer_start", 49)
	engine.gs_set("screen_brightness", 2)
	_check(engine.call_c("Fade_Out_Main") == 1 and engine.gs_get("screen_brightness") == 0 and m.read_global("vsync_timer_start") == 49, "Fade_Out_Main: brightness 2 (< 3) becomes 0, returns 1, the timer is not restarted")
	m.write_global("vsync_timer_start", 49)
	engine.gs_set("screen_brightness", 3)
	engine.call_c("Fade_Out_Main")
	_check(engine.gs_get("screen_brightness") == 0xFF, "brightness 3 is not below 3: 3 - 4 wraps in the u8 to 0xFF")
	# Fade_In_Main: brightness >= 0x7E -> 0x80, return 1; else += 4 (u8, no clamp)
	m.write_global("vsync_timer_start", 49)
	engine.gs_set("screen_brightness", 0x7D)
	_check(engine.call_c("Fade_In_Main") == 0 and engine.gs_get("screen_brightness") == 0x81, "Fade_In_Main: 0x7D + 4 = 0x81 (no clamp), returns 0")
	m.write_global("vsync_timer_start", 49)
	engine.gs_set("screen_brightness", 0x7E)
	_check(engine.call_c("Fade_In_Main") == 1 and engine.gs_get("screen_brightness") == 0x80, "Fade_In_Main: 0x7E -> 0x80, returns 1")
	m.write_global("vsync_timer_start", 50)
	engine.gs_set("screen_brightness", 0x10)
	_check(engine.call_c("Fade_In_Main") == 0 and engine.gs_get("screen_brightness") == 0x10, "Fade_In_Main with no new tick changes nothing")
	engine.call_c("Fade_Out_Init")
	engine.call_c("Fade_In_Init")
	_check(m.read_global("vsync_timer_start") == 50, "Fade_Out_Init and Fade_In_Init restart the timer interval")

func _test_screen_reset() -> void:
	var engine = _new()
	var calls: Array = []
	for name: String in ["draw2d3d", "actor_draw_enable", "sprite_tables_clear", "actor_clear_upper", "back_clear_switch", "text_system_reset", "tpage_buf_clear_all", "tpage_buf_clear"]:
		engine.set_stub(name, func(args: Array) -> int:
			calls.append([name, args.duplicate()])
			return 0)
	engine.gs_set("screen_brightness", 7)
	engine.gs_set("tile_layer_enabled", 1)
	engine.call_c("scene_screen_reset", [5, 0])
	_check(calls == [["draw2d3d", [1, 0]], ["actor_draw_enable", [1]], ["sprite_tables_clear", []], ["actor_clear_upper", []],
		["back_clear_switch", [5]], ["text_system_reset", []], ["tpage_buf_clear", []]], "scene_screen_reset(5, 0): %s" % [calls])
	_check(engine.gs_get("clear_color_r") == 0 and engine.gs_get("clear_color_g") == 0 and engine.gs_get("clear_color_b") == 0 \
		and engine.gs_get("screen_brightness") == 0x80 and engine.gs_get("tile_layer_enabled") == 0, "the clear colour is black, brightness 0x80, the tile layer off")
	calls.clear()
	engine.call_c("scene_screen_reset", [-1, 3]) # arg1 != 0: tpage_buf_clear_all, and nothing after it
	_check(calls[4] == ["back_clear_switch", [-1]] and calls[calls.size() - 1] == ["tpage_buf_clear_all", []] and calls.size() == 7, "scene_screen_reset(-1, 3) ends with tpage_buf_clear_all: %s" % [calls])

func _test_rot_2d() -> void:
	var engine = _new()
	var m = engine.memory
	engine.set_stub("rsin", func(_args: Array) -> int: return 1000)
	engine.set_stub("func_800A0140", func(args: Array) -> int:
		return 2000 if args == [77] else -1)
	var frame: int = engine.frame_base()
	m.write_u32(frame + 0x40, -3000) # *arg1 (x)
	m.write_u32(frame + 0x44, 500) # *arg2 (y)
	engine.call_c("Rot_2D", [77, frame + 0x40, frame + 0x44, frame + 0x48, frame + 0x4C])
	# C: *arg3 = c * *arg1 / 4096 - *arg2 * s / 4096; *arg4 = s * *arg1 / 4096 + *arg2 * c / 4096 (division truncates toward 0)
	# c = 2000, s = 1000: -6000000 / 4096 = -1464, 500000 / 4096 = 122 -> -1586; -3000000 / 4096 = -732, 1000000 / 4096 = 244 -> -488
	_check(m.s32(frame + 0x48) == -1586 and m.s32(frame + 0x4C) == -488, "Rot_2D: got %d, %d, wanted -1586, -488" % [m.s32(frame + 0x48), m.s32(frame + 0x4C)])

func _test_small_functions() -> void:
	var engine = _new()
	var m = engine.memory
	# girl_state_rank(a) = (u8) get_g_zyotai_s(a) & 0x7F
	engine.set_stub("get_g_zyotai_s", func(args: Array) -> int:
		return 0x1FF if args == [3] else 0x85)
	_check(engine.call_c("girl_state_rank", [3]) == 0x7F, "girl_state_rank: (u8) 0x1FF = 0xFF, & 0x7F")
	_check(engine.call_c("girl_state_rank", [4]) == 5, "girl_state_rank: 0x85 & 0x7F")
	# actor_pair_swap_sequences: swap the two s16 values, clear D_80120668, D_801206F0, D_80120658
	m.write_global("D_80120666", 111)
	m.write_global("D_801206EE", -222)
	m.write_global("D_80120668", 5)
	m.write_global("D_801206F0", 6)
	m.write_global("D_80120658", 7)
	engine.call_c("actor_pair_swap_sequences")
	_check(m.read_global("D_80120666") == -222 and m.read_global("D_801206EE") == 111, "the two halfwords are swapped")
	_check(m.read_global("D_80120668") == 0 and m.read_global("D_801206F0") == 0 and m.read_global("D_80120658") == 0, "the three words are cleared")
	# Yubi(idx): hud_cursor_update(); the s16 at +0x26 and +0x2A of actor idx = cursor_x, cursor_y
	var updates: Array = []
	engine.set_stub("hud_cursor_update", func(_args: Array) -> int:
		updates.append(1)
		return 0)
	m.write_global("cursor_x", -12)
	m.write_global("cursor_y", 34)
	engine.call_c("Yubi", [2])
	var actor: int = m.global_address("actor_pool") + 2 * 0x44
	_check(updates.size() == 1 and m.s16(actor + 0x26) == -12 and m.s16(actor + 0x2A) == 34, "Yubi stores the cursor position in actor 2")
	# Default_Disp: the eight per-frame calls in the C order, hud_screen_frame_draw with 2
	var order: Array = []
	for name: String in ["k_disp_inc2", "check_k_scroll", "scene_frame_update", "message_window_show", "hizuke_show", "hud_screen_frame_draw", "message_window_title_update", "draw_shake_update"]:
		engine.set_stub(name, func(args: Array) -> int:
			order.append([name, args.duplicate()])
			return 0)
	engine.call_c("Default_Disp")
	_check(order == [["k_disp_inc2", []], ["check_k_scroll", []], ["scene_frame_update", []], ["message_window_show", []], ["hizuke_show", []],
		["hud_screen_frame_draw", [2]], ["message_window_title_update", []], ["draw_shake_update", []]], "Default_Disp: %s" % [order])

func _test_audio_latch() -> void:
	var engine = _new()
	var m = engine.memory
	var polls: Array = []
	engine.set_stub("audio_bank_load_poll", func(_args: Array) -> int:
		polls.append(1)
		return 1 if polls.size() >= 2 else 0)
	engine.call_c("audio_bank_wait_latch")
	_check(m.read_global("audio_bank_wait_done") == 0 and polls.size() == 1, "the latch stays 0 while the poll returns 0")
	engine.call_c("audio_bank_wait_latch")
	_check(m.read_global("audio_bank_wait_done") == 1, "the latch is set when the poll returns 1")
	engine.call_c("audio_bank_wait_latch")
	_check(polls.size() == 2, "once set, the poll is not called again")

## select_girl_by_scores (C: best-girl selection among the girls with the `met` flag): the first met girl is the
## starting best; a later girl with a higher tokimeki + yuukou score replaces it when the rank of the current best
## is not below hers (t >= rank), a tie goes to the larger tokimeki + affection sum under the same rank test.
func _test_select_girl_by_scores() -> void:
	var engine = _new()
	var m = engine.memory
	var base: int = engine.gs_address("girls")
	for girl: int in 11:
		m.write_u32(base + girl * 0x38 + 0xC, 0)
	for girl: int in [0, 2, 3]: # met: bit 1 of the flags word
		m.write_u32(base + girl * 0x38 + 0xC, 2)
	var yuukou: Dictionary = {0: 10, 2: 5, 3: 10}
	var tokimeki: Dictionary = {0: 20, 2: 30, 3: 25}
	var rank: Dictionary = {0: 3, 2: 2, 3: 2}
	engine.set_stub("get_h_yuukou", func(args: Array) -> int: return yuukou[args[0]])
	engine.set_stub("get_h_tokimeki", func(args: Array) -> int: return tokimeki[args[0]])
	engine.set_stub("get_g_zyotai_s", func(args: Array) -> int: return rank[args[0]])
	m.write_u16(base + 2 * 0x38 + 2, 5) # tokimeki of girl 2
	m.write_u16(base + 2 * 0x38 + 6, 6) # affection of girl 2: sum 11
	m.write_u16(base + 3 * 0x38 + 2, 20)
	m.write_u16(base + 3 * 0x38 + 6, 1) # girl 3: sum 21
	# girl 2: score 35 > 30 and rank(0) = 3 >= rank(2) = 2: best = 2. girl 3: score 35 == 35, rank(2) = 2 >= 2, sum 11 < 21: best = 3
	_check(engine.call_c("select_girl_by_scores") == 3, "the tie goes to the larger tokimeki + affection sum")
	rank[3] = 9 # rank(best = 2) = 2 < 9: the tie is not taken
	_check(engine.call_c("select_girl_by_scores") == 2, "a tie with a higher rank stays with the current best")
	rank[0] = 1 # rank(0) = 1 < rank(2) = 2: the higher score of girl 2 does not replace girl 0
	rank[3] = 2
	_check(engine.call_c("select_girl_by_scores") == 0, "a higher score loses against a better rank of the current best")
	for girl: int in [0, 2, 3]:
		m.write_u32(base + girl * 0x38 + 0xC, 1) # bit 0 is not the met bit
	_check(engine.call_c("select_girl_by_scores") == 0, "without the met flag only girl 0 stays (the starting best)")
