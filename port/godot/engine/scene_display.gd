extends RefCounted
## Port of src/main/scene_display.c (original object 0x80085E30-0x80086810, T-39312).
## 
## Contract: wiki/engine-porting-contract.md. Tests: config/port/engine-diff/scene_display.json (emulator captures) and
## tests/engine/scene_display_test.gd (hand checks written from the C).
## Each function follows the C statement by statement: integer widths and K&R promotion are applied where the C applies
## them (a 32-bit wrap is written out only where the value range allows an overflow), memory is read and written
## with the C types, calls go through engine.call_c, loops that call out test engine.halted().

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/scene_display.c"
const C_FUNCTIONS: Array[String] = [
	"scene_screen_reset",
	"Vblnk_Timer_Init",
	"Vblnk_Timer",
	"Scroll",
	"Rot_2D",
	"Fade_Out_Init",
	"Fade_Out_Main",
	"Fade_In_Init",
	"Fade_In_Main",
	"Yubi",
	"Default_Disp",
	"select_girl_by_scores",
	"actor_pair_swap_sequences",
	"girl_state_rank",
	"portrait_read_step",
	"audio_bank_wait_latch",
]
const C_UNPORTED: Array[String] = [
]
const C_SIGNATURES: Dictionary = {
	"scene_screen_reset": {"args": ["s32", "s32"], "ret": "void"},
	"Vblnk_Timer_Init": {"args": [], "ret": "void"},
	"Vblnk_Timer": {"args": [], "ret": "s32"},
	"Scroll": {"args": ["s32", "s32", "s16", "s16", "s16"], "ret": "void"},
	"Rot_2D": {"args": ["s32", "ptr", "ptr", "ptr", "ptr"], "ret": "void"},
	"Fade_Out_Init": {"args": [], "ret": "void"},
	"Fade_Out_Main": {"args": [], "ret": "s32"},
	"Fade_In_Init": {"args": [], "ret": "void"},
	"Fade_In_Main": {"args": [], "ret": "s32"},
	"Yubi": {"args": ["s32"], "ret": "void"},
	"Default_Disp": {"args": [], "ret": "void"},
	"select_girl_by_scores": {"args": [], "ret": "s32"},
	"actor_pair_swap_sequences": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"girl_state_rank": {"args": ["s32"], "ret": "s32"},
	"portrait_read_step": {"args": ["s32"], "ret": "void"},
	"audio_bank_wait_latch": {"args": [], "ret": "void"},
}

## game_state (GAME_STATE_BASE of core/game_layout.gd, config/symbol_addrs.txt).
const GS: int = 0x800E6280
## face_images (config/symbol_addrs*.txt)
const G_FACE_IMAGES: int = 0x800CA120
## face_frame_tables (config/symbol_addrs*.txt)
const G_FACE_FRAME_TABLES: int = 0x800CA124
## face_sequences (config/symbol_addrs*.txt)
const G_FACE_SEQUENCES: int = 0x800CA128
## vsync_timer_start (config/symbol_addrs*.txt)
const G_VSYNC_TIMER_START: int = 0x800E36F0
## vsync_timer_elapsed (config/symbol_addrs*.txt)
const G_VSYNC_TIMER_ELAPSED: int = 0x800E36F4
## draw_ot_tags (config/symbol_addrs*.txt)
const G_DRAW_OT_TAGS: int = 0x800E8CA0
## draw_buffer (config/symbol_addrs*.txt)
const G_DRAW_BUFFER: int = 0x8011ECA0
## actor_pool (config/symbol_addrs*.txt)
const G_ACTOR_POOL: int = 0x8011ECD0
## cursor_x (config/symbol_addrs*.txt)
const G_CURSOR_X: int = 0x8011ECF6
## cursor_y (config/symbol_addrs*.txt)
const G_CURSOR_Y: int = 0x8011ECFA
## D_80120658 (config/symbol_addrs*.txt)
const G_D_80120658: int = 0x80120658
## D_80120666 (config/symbol_addrs*.txt)
const G_D_80120666: int = 0x80120666
## D_80120668 (config/symbol_addrs*.txt)
const G_D_80120668: int = 0x80120668
## D_801206EE (config/symbol_addrs*.txt)
const G_D_801206EE: int = 0x801206EE
## D_801206F0 (config/symbol_addrs*.txt)
const G_D_801206F0: int = 0x801206F0
## audio_bank_wait_done (config/symbol_addrs*.txt)
const G_AUDIO_BANK_WAIT_DONE: int = 0x80122EA0

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

@warning_ignore("integer_division")
func _div(left: int, right: int) -> int:
	if right == 0:
		engine.refuse("division", "C integer division by zero")
		return 0
	return left / right # GDScript integer division truncates toward zero, as the C does

func _mod(left: int, right: int) -> int:
	if right == 0:
		engine.refuse("modulo", "C integer remainder by zero")
		return 0
	return left % right # the sign follows the dividend, as in C

func scene_screen_reset(arg0: int, arg1: int) -> void:
	arg0 = Math.s32(arg0)
	arg1 = Math.s32(arg1)
	engine.call_c("draw2d3d", [1, 0])
	engine.call_c("actor_draw_enable", [1])
	engine.call_c("sprite_tables_clear")
	engine.call_c("actor_clear_upper")
	engine.call_c("back_clear_switch", [arg0])
	engine.gs_set("clear_color_r", 0)
	engine.gs_set("clear_color_g", 0)
	engine.gs_set("clear_color_b", 0)
	engine.gs_set("screen_brightness", 0x80)
	engine.call_c("text_system_reset")
	engine.gs_set("tile_layer_enabled", 0)
	if arg1 != 0:
		engine.call_c("tpage_buf_clear_all")
		return
	engine.call_c("tpage_buf_clear")

## Starts the VBLANK timer interval.
func Vblnk_Timer_Init() -> void:
	engine.memory.write_global("vsync_timer_start", engine.gs_get("timer_ticks"))
	engine.memory.write_global("vsync_timer_elapsed", 0)

## Updates and returns elapsed ticks in the VBLANK timer interval.
func Vblnk_Timer() -> int:
	engine.memory.write_global("vsync_timer_elapsed", Math.s32(engine.gs_get("timer_ticks") - engine.memory.read_global("vsync_timer_start")))
	return engine.memory.read_global("vsync_timer_elapsed")

## Emits textured GPU quads for the scrolling background effect.
func Scroll(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> void:
	arg0 = Math.s32(arg0)
	arg1 = Math.s32(arg1)
	arg2 = Math.s16(arg2)
	arg3 = Math.s16(arg3)
	arg4 = Math.s16(arg4)
	var p: int = 0
	var i: int = 0
	var lo: int = 0
	var pad1: int = 0
	var pad2: int = 0
	i = 0
	while i < 5:
		if engine.halted():
			break
		p = Math.u32(engine.call_c("GetWorkBase", [0x28, engine.memory.read_global("draw_buffer")]))
		engine.call_c("func_8009F040", [p])
		engine.call_c("SetSemiTrans", [p, 0])
		engine.call_c("SetShadeTex", [p, 0])
		engine.memory.write_u8(p + 0x4, engine.gs_get("screen_brightness"))
		engine.memory.write_u8(p + 0x5, engine.gs_get("screen_brightness"))
		engine.memory.write_u8(p + 0x6, engine.gs_get("screen_brightness"))
		lo = (Math.s32(Math.s32(arg0 * 2) + _mod(i, 2) * 0x40)) & 0x7F
		engine.memory.write_u8(p + 0x14, lo + 0x40)
		engine.memory.write_u8(p + 0x24, lo + 0x40)
		engine.memory.write_u8(p + 0xC, lo)
		engine.memory.write_u8(p + 0x1C, lo)
		pad1 = Math.s32(i * 0x20)
		pad2 = Math.s32(arg4)
		engine.memory.write_u8(p + 0x25, arg2 + arg3)
		engine.memory.write_u8(p + 0x1D, arg2 + arg3)
		engine.memory.write_u8(p + 0x15, arg2)
		engine.memory.write_u8(p + 0xD, arg2)
		engine.memory.write_u16(p + 0x8, Math.s32(Math.s32(engine.memory.s16(GS + 0x14 + engine.memory.read_global("draw_buffer") * 2) + Math.s32(i * 0x40)) - 0xA0))
		engine.memory.write_u16(p + 0xA, engine.memory.s16(GS + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg2 - 0x78)
		engine.memory.write_u16(p + 0x10, Math.s32(Math.s32(engine.memory.s16(GS + 0x14 + engine.memory.read_global("draw_buffer") * 2) + Math.s32(i * 0x40)) - 0x60))
		engine.memory.write_u16(p + 0x12, engine.memory.s16(GS + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg2 - 0x78)
		engine.memory.write_u16(p + 0x18, Math.s32(Math.s32(engine.memory.s16(GS + 0x14 + engine.memory.read_global("draw_buffer") * 2) + Math.s32(i * 0x40)) - 0xA0))
		engine.memory.write_u16(p + 0x1A, engine.memory.s16(GS + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg2 + arg3 - 0x78)
		engine.memory.write_u16(p + 0x20, Math.s32(Math.s32(engine.memory.s16(GS + 0x14 + engine.memory.read_global("draw_buffer") * 2) + Math.s32(i * 0x40)) - 0x60))
		engine.memory.write_u16(p + 0x22, engine.memory.s16(GS + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg2 + arg3 - 0x78)
		engine.memory.write_u16(p + 0x16, Math.s32(engine.call_c("func_8009ECB0", [1, 0, Math.s32(_mod(Math.u32((_div(Math.u32(Math.u32(i * 0x20) + Math.u32(arg0)), 0x40)) * 0x40), Math.u32(arg4)) + 0x140), 0])))
		engine.memory.write_u16(p + 0xE, Math.s32(engine.call_c("func_8009ED74", [0, 0x1F0])))
		engine.call_c("AddPrim", [Math.u32(Math.u32(G_DRAW_OT_TAGS + (engine.memory.read_global("draw_buffer") << 10)) + arg1 * 4), Math.s32(p)])
		i = Math.s32(i + 1)

## Rotates a two-dimensional point using the engine sine and cosine values.
func Rot_2D(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> void:
	arg0 = Math.s32(arg0)
	arg1 = Math.u32(arg1)
	arg2 = Math.u32(arg2)
	arg3 = Math.u32(arg3)
	arg4 = Math.u32(arg4)
	var s: int = 0
	var c: int = 0
	s = Math.s32(engine.call_c("rsin"))
	c = Math.s32(engine.call_c("func_800A0140", [arg0]))
	engine.memory.write_u32(arg3, Math.s32(_div(Math.s32(c * engine.memory.s32(arg1)), 0x1000) - _div(Math.s32(engine.memory.s32(arg2) * s), 0x1000)))
	engine.memory.write_u32(arg4, Math.s32(_div(Math.s32(s * engine.memory.s32(arg1)), 0x1000) + _div(Math.s32(engine.memory.s32(arg2) * c), 0x1000)))

## Starts a new fade-out timer interval.
func Fade_Out_Init() -> void:
	engine.call_c("Vblnk_Timer_Init")

## Lowers screen brightness to zero and returns when the fade-out is complete.
func Fade_Out_Main() -> int:
	var temp: int = 0
	if Math.s32(engine.call_c("Vblnk_Timer")) != 0:
		temp = engine.gs_get("screen_brightness")
		if temp < 3:
			engine.gs_set("screen_brightness", 0)
			return 1
		engine.gs_set("screen_brightness", Math.u32(engine.gs_get("screen_brightness") - 4))
		engine.call_c("Vblnk_Timer_Init")
		return 0
	return 0

## Starts a new fade-in timer interval.
func Fade_In_Init() -> void:
	engine.call_c("Vblnk_Timer_Init")

## Raises screen brightness to 0x80 and returns when the fade-in is complete.
func Fade_In_Main() -> int:
	var temp: int = 0
	if Math.s32(engine.call_c("Vblnk_Timer")) != 0:
		temp = engine.gs_get("screen_brightness")
		if temp >= 0x7E:
			engine.gs_set("screen_brightness", 0x80)
			return 1
		engine.gs_set("screen_brightness", engine.gs_get("screen_brightness") + 4)
		engine.call_c("Vblnk_Timer_Init")
		return 0
	return 0

## Updates the cursor actor position from the current HUD cursor.
func Yubi(idx: int) -> void:
	idx = Math.s32(idx)
	var p: int = 0
	engine.call_c("hud_cursor_update")
	p = Math.u32(G_ACTOR_POOL + idx * 0x44)
	engine.memory.write_u16(p + 0x26, engine.memory.read_global("cursor_x"))
	engine.memory.write_u16(p + 0x2A, engine.memory.read_global("cursor_y"))

## Runs the common per-frame scene and message display updates.
func Default_Disp() -> void:
	engine.call_c("k_disp_inc2")
	engine.call_c("check_k_scroll")
	engine.call_c("scene_frame_update")
	engine.call_c("message_window_show")
	engine.call_c("hizuke_show")
	engine.call_c("hud_screen_frame_draw", [2])
	engine.call_c("message_window_title_update")
	engine.call_c("draw_shake_update")

func select_girl_by_scores() -> int:
	var sum1: int = 0
	var sum2: int = 0
	var cur: int = 0
	var n: int = 0
	var i: int = 0
	var best: int = 0
	var bscore: int = 0
	var score: int = 0
	var t: int = 0
	n = 0
	i = 0
	while i < 0xB:
		if ((engine.memory.u32(GS + 0x1BC + i * 56 + 0xC) >> 1) & 0x1) == 1:
			engine.memory.write_u32(engine.frame_base() + 0x6C + n * 4, i)
			n = Math.u32(n + 1) & 0xFF
		i = Math.s32(i + 1)
	t = Math.u32(engine.call_c("get_h_yuukou", [0])) & 0xFF
	bscore = Math.s32(Math.u32(engine.call_c("get_h_tokimeki", [0])) + t)
	best = 0
	i = 1
	while Math.u32(i) < n:
		if engine.halted():
			break
		t = (Math.u32(engine.call_c("get_h_tokimeki", [engine.memory.s32(engine.frame_base() + 0x6C + i * 4)]))) & 0xFF
		score = Math.s32(Math.u32(engine.call_c("get_h_yuukou", [engine.memory.s32(engine.frame_base() + 0x6C + i * 4)])) + t)
		var _c1: bool = bscore < score
		if _c1:
			t = Math.s32(engine.call_c("girl_state_rank", [best])) & 0xFF
			_c1 = t >= (Math.u32(engine.call_c("girl_state_rank", [engine.memory.s32(engine.frame_base() + 0x6C + i * 4)])))
		if _c1:
			best = engine.memory.s32(engine.frame_base() + 0x6C + i * 4)
			bscore = score
		elif bscore == score:
			t = Math.s32(engine.call_c("girl_state_rank", [best])) & 0xFF
			if t >= (Math.u32(engine.call_c("girl_state_rank", [engine.memory.s32(engine.frame_base() + 0x6C + i * 4)]))):
				cur = engine.memory.s32(engine.frame_base() + 0x6C + i * 4)
				sum1 = engine.memory.s16(GS + 0x1BC + best * 56 + 0x2) + engine.memory.s16(GS + 0x1BC + best * 56 + 0x6)
				sum2 = engine.memory.s16(GS + 0x1BC + cur * 56 + 0x2) + engine.memory.s16(GS + 0x1BC + cur * 56 + 0x6)
				if sum1 < sum2:
					bscore = score
					best = cur
		i = Math.s32(i + 1)
	return best

func actor_pair_swap_sequences() -> int:
	var right: int = 0
	var old_right: int = 0
	var old_left: int = 0
	old_left = engine.memory.read_global("D_80120666")
	right = G_D_801206EE
	old_right = engine.memory.s16(right)
	engine.memory.write_global("D_80120666", old_right)
	engine.memory.write_global("D_801206EE", old_left)
	engine.memory.write_global("D_80120668", 0)
	engine.memory.write_global("D_801206F0", 0)
	engine.memory.write_global("D_80120658", 0)
	return 0 # the C falls off the end here (v0 undefined)

func girl_state_rank(a: int) -> int:
	a = Math.s32(a)
	return Math.u8(engine.call_c("get_g_zyotai_s", [a])) & 0x7F

func portrait_read_step(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	if engine.gs_get("substep") == 0:
		engine.call_c("portrait_read_start", [arg0]) # the C passes no argument: a0 of the caller is forwarded
		engine.gs_set("substep", engine.gs_get("substep") + 1)
		return
	if (Math.u8(engine.call_c("cd_read_sync_status")) & 1) != 0:
		if ((((((Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")))) != 0x801820AC or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images") + 4)))) != 0x801860AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images") + 8)))) != 0x8018A0AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images") + 0xC)))) != 0x8018E0AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images") + 0x10)))) != 0x801920AC) or Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_sequences")))) != 0x8018005C) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_sequences") + 4)))) != 0x80180070) or Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_frame_tables")))) != 0x80180000:
			engine.gs_set("substep", 0)
			return
		if Math.s32(engine.call_c("cd_file_checksum_ok", [0x2D, 0x80180000])) == 0:
			engine.gs_set("substep", 0)
			return
		engine.call_c("mode_step_next")

func audio_bank_wait_latch() -> void:
	if engine.memory.read_global("audio_bank_wait_done") == 0:
		if Math.s32(engine.call_c("audio_bank_load_poll")) == 1:
			engine.memory.write_global("audio_bank_wait_done", 1)
