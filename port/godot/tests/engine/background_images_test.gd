extends RefCounted
## T-39311: engine/background_images.gd on invented memory. Expectations are derived by hand from
## src/main/background_images.c. libgpu/MDEC calls, the CD read and goto_tpage are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF
const SDK: Array = ["AddPrim", "SetSemiTrans", "SetShadeTex", "func_8009F0A4", "goto_tpage"]

func run() -> Array[String]:
	s.failures.clear()
	_test_state_functions()
	_test_cd_read_and_decode()
	_test_capture_and_wipe()
	_test_draw()
	_test_fades()
	s.shutdown_all()
	return s.failures

func _ot(e, slot: int) -> int:
	return e.memory.global_address("draw_ot_tags") + (1 << 10) + slot * 4

func _engine(log: Array, names: Array, results: Dictionary = {}) -> Object:
	var e = s.new_engine()
	s.record(e, names, log, results)
	e.memory.write_global("draw_buffer", 1)
	e.call_c("SetWorkBase", [0x80160000, 1])
	s.set_gs_s16(e, "draw_origin_x", 1, 100)
	s.set_gs_s16(e, "draw_origin_y", 1, -20)
	return e

func _test_state_functions() -> void:
	var e = s.new_engine()
	var m = e.memory
	e.call_c("set_dec_bri", [0x1FF])
	s.check(m.read_global("bg_brightness") == 0xFF, "set_dec_bri stores the low byte")
	m.write_global("dec_bg_last_screen_mode", 7)
	s.check(e.call_c("get_last_gamen_mode") == 7, "get_last_gamen_mode")
	for i: int in 4:
		m.write_global("bg_lba_cache", 100 + i, i)
		m.write_u8(0x800B5938 + i, 1)
	m.write_s32(0x800B5934, 55)
	e.call_c("dec_bg_reset")
	s.check([m.read_global("bg_lba_cache", 0), m.read_global("bg_lba_cache", 1), m.read_global("bg_lba_cache", 2), m.read_global("bg_lba_cache", 3)] == [0, 0, 0, 0], "dec_bg_reset clears the four cached LBAs")
	s.check([m.u8(0x800B5938), m.u8(0x800B5939), m.u8(0x800B593A), m.u8(0x800B593B)] == [0, 0, 0, 1], "show flags 0, 1, 2 cleared, 3 untouched")
	s.check(m.read_global("bg_brightness") == 0x80 and m.read_global("dec_bg_last_screen_mode") == -1 and m.s32(0x800B5934) == 0, "brightness 0x80, last mode -1, D_800B5934 = 0")
	e.call_c("dec_bg_show_switch", [2])
	s.check(m.u8(0x800B593A) == 1, "show_switch: 1 - 0")
	e.call_c("dec_bg_show_switch", [2])
	s.check(m.u8(0x800B593A) == 0, "show_switch: 1 - 1")
	e.call_c("dec_bg_show_set", [1, 6])
	s.check(m.u8(0x800B5939) == 0, "show_set stores arg1 & 1 (6 -> 0)")
	e.call_c("dec_bg_show_set", [1, 7])
	s.check(m.u8(0x800B5939) == 1, "show_set (7 -> 1)")
	e.call_c("bg_fade_set", [0x180])
	s.check(m.read_global("bg_fade_amount") == 0x80, "bg_fade_set stores the low byte")

func _test_cd_read_and_decode() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["cd_read_sectors_nowait", "bg_still_buffers_set", "bg_still_decode_to_vram"], log, {"bg_still_decode_to_vram": 0x77})
	var m = e.memory
	m.write_global("bg_lba_cache", 5, 0)
	m.write_global("bg_lba_cache", 6, 1)
	m.write_global("bg_lba_cache", 8, 2)
	m.write_global("bg_lba_cache", 9, 3)
	s.check(e.call_c("dec_bg_cd_read", [5, 0]) == 0 and e.call_c("dec_bg_cd_read", [6, 0]) == 1 and e.call_c("dec_bg_cd_read", [8, 1]) == 0 and e.call_c("dec_bg_cd_read", [9, 1]) == 1 and log.is_empty(), "cache hits return 0 (first) or 1 (second) without a read")
	s.check(e.call_c("dec_bg_cd_read", [7, 0]) == -1 and log == [["cd_read_sectors_nowait", 0xA, 0x80162000, 7]] and m.read_global("bg_lba_cache", 1) == 7, "layer 0 miss with show[0] = 0: read, entry 1 replaced")
	m.write_u8(0x800B5938, 1)
	e.call_c("dec_bg_cd_read", [17, 0])
	s.check(m.read_global("bg_lba_cache", 0) == 17, "show[0] = 1: entry 0 replaced (bg_lba_cache[1] - 4 bytes)")
	log.clear()
	s.check(e.call_c("dec_bg_cd_read", [11, 1]) == -1 and m.read_global("bg_lba_cache", 3) == 11, "layer 1 miss with D_800B5939 = 0: entry 3")
	m.write_u8(0x800B5939, 1)
	e.call_c("dec_bg_cd_read", [12, 1])
	s.check(m.read_global("bg_lba_cache", 2) == 12, "D_800B5939 = 1: entry 2")
	# decode
	log.clear()
	m.write_u8(0x800B5938, 0)
	s.check(e.call_c("dec_bg_decode_to_vram", [0]) == 1 and log == [["bg_still_buffers_set", 0x80167000, 0x80162000, 0x80162000], ["bg_still_decode_to_vram", 0x140, 0x1A0, 0x140, 0x58]] and m.read_global("bg_decode_result") == 0x77, "layer 0, show 0: second buffer, returns 1, result stored: %s" % [log])
	log.clear()
	m.write_u8(0x800B5938, 1)
	s.check(e.call_c("dec_bg_decode_to_vram", [0]) == 0 and log[1] == ["bg_still_decode_to_vram", 0x2C0, 0xA0, 0x140, 0x58], "layer 0, show 1: first buffer, returns 0")
	log.clear()
	m.write_u8(0x800B5939, 0)
	s.check(e.call_c("dec_bg_decode_to_vram", [1]) == 1 and log[1] == ["bg_still_decode_to_vram", 0x140, 0x100, 0x140, 0xA0], "layer 1, show 0")
	log.clear()
	m.write_u8(0x800B5939, 1)
	s.check(e.call_c("dec_bg_decode_to_vram", [1]) == 0 and log[1] == ["bg_still_decode_to_vram", 0x2C0, 0, 0x140, 0xA0], "layer 1, show 1")
	log.clear()
	e.call_c("dec_bg_decode_to_vram", [2])
	s.check(log.size() == 1 and m.read_global("bg_decode_result") == 0, "another layer: only the buffers are set, the result stays 0")

func _test_capture_and_wipe() -> void:
	var log: Array = []
	var rects: Array = []
	var e = s.new_engine()
	s.record(e, ["func_8009C93C", "func_8009C674"], log, {}, {"func_8009C93C": func(a: Array) -> void: rects.append(s.bytes(e, a[0], 8))})
	var m = e.memory
	m.write_global("draw_buffer", 1)
	m.write_global("bg_lba_cache", 9, 3)
	m.write_global("bg_wipe_count", 5)
	e.call_c("bg_capture_screen", [0x50])
	var frame: int = e.sp - 0x40 + 0x34
	s.check(log == [["func_8009C93C", frame, 0x140, 0x100], ["func_8009C674", 0]] and rects[0] == [0, 0, 0xF0, 0, 0x40, 1, 0x50, 0], "D_800B5939 = 0: v = 1, MoveImage(&rect, 0x140, 0x100) with rect {0, buffer * 0xF0, 0x140, arg0}: %s %s" % [log, rects])
	s.check(m.read_global("bg_lba_cache", 3) == 0 and m.u8(0x800B5939) == 1 and m.read_global("bg_wipe_count") == 0, "bg_lba_cache[3] = 0, layer 1 shown flag = 1 - 0, wipe count 0")
	log.clear()
	rects.clear()
	m.write_global("draw_buffer", 0)
	e.call_c("bg_capture_screen", [0x20])
	s.check(log[0] == ["func_8009C93C", frame, 0x2C0, 0] and m.read_global("bg_lba_cache", 2) == 0 and m.u8(0x800B5939) == 0, "D_800B5939 = 1: v = 0, MoveImage(&rect, 0x2C0, 0), bg_lba_cache[2] = 0, flag back to 0")
	# wipe
	var wl: Array = []
	var e2 = s.new_engine()
	s.record(e2, ["prim_gradient_line"], wl)
	var m2 = e2.memory
	m2.write_global("bg_brightness", 0x80)
	m2.write_global("bg_wipe_count", 0)
	e2.call_c("bg_wipe_step")
	s.check(m2.read_global("bg_brightness") == 0x78 and m2.read_global("bg_wipe_count") == 1 and wl.size() == 15 * 2, "brightness - 8, count 1: width 2, 15 bands: %d lines" % wl.size())
	s.check(wl[0] == ["prim_gradient_line", M32 & (0x1F - 0xA0), M32 & -0x78, M32 & -0xA0, M32 & (0x1F - 0x78), 0, 0, 0xD] and wl[1][1] == M32 & (0x1E - 0xA0), "band 0 (offset < 0xA0): (point - 0xA0, -0x78, -0xA0, point - 0x78) for point 0x1F, 0x1E: %s" % [wl[0]])
	s.check(wl[10] == ["prim_gradient_line", 0xBF - 0xA0, M32 & -0x78, M32 & (0xBF - 0x140), 0x28, 0, 0, 0xD], "band 5 (offset 0xA0 < 0x140): (point - 0xA0, -0x78, point - 0x140, 0x28): %s" % [wl[10]])
	s.check(wl[20] == ["prim_gradient_line", 0xA0, 0x15F - 0x1B8 & M32, M32 & (0x15F - 0x140), 0x28, 0, 0, 0xD], "band 10 (offset 0x140): (0xA0, point - 0x1B8, point - 0x140, 0x28): %s" % [wl[20]])
	m2.write_global("bg_brightness", 4)
	e2.call_c("bg_wipe_step")
	s.check(m2.read_global("bg_brightness") == 0, "4 - 8 wraps to 0xFC (u8), above 0x80: 0")
	wl.clear()
	m2.write_global("bg_wipe_count", 0x13)
	e2.call_c("bg_wipe_step")
	s.check(wl.size() == 15 * 0x20, "count 20: 2 * 20 = 40 capped to width 0x20: %d lines" % wl.size())
	wl.clear()
	m2.write_global("bg_wipe_count", 0x3FFF)
	e2.call_c("bg_wipe_step")
	s.check(wl.is_empty(), "count 0x4000: doubled = 0x8000 is -32768 as an s16: width negative, no lines")

func _test_draw() -> void:
	var log: Array = []
	var e = _engine(log, SDK)
	var m = e.memory
	e.call_c("dec_bg_draw", [-1])
	s.check(log.is_empty() and m.read_global("dec_bg_last_screen_mode") == 0, "a = -1: nothing at all")
	m.write_global("bg_brightness", 0x66)
	m.write_u8(0x800B593A, 1)
	e.call_c("dec_bg_draw", [2])
	s.check(m.read_global("dec_bg_last_screen_mode") == 2, "the last screen mode is recorded")
	var p: int = 0x80160000
	# mode 2: left sprite x = 100 - 0xA0, y = -20 - 0x78, v0 = 0, 0xC0 x 0xF0
	s.check(s.bytes(e, p + 4, 3) == [0x66, 0x66, 0x66] and [m.s16(p + 8), m.s16(p + 0xA), m.u8(p + 0xC), m.u8(p + 0xD), m.s16(p + 0x10), m.s16(p + 0x12)] == [100 - 0xA0, -20 - 0x78, 0, 0, 0xC0, 0xF0], "mode 2 left sprite: tall, from the top")
	s.check(log.map(func(x): return x[0]) == ["func_8009F0A4", "SetSemiTrans", "SetShadeTex", "AddPrim", "goto_tpage", "func_8009F0A4", "SetSemiTrans", "SetShadeTex", "AddPrim", "goto_tpage"], "call order: %s" % [log.map(func(x): return x[0])])
	s.check(log[1] == ["SetSemiTrans", p, 0] and log[3] == ["AddPrim", _ot(e, 13), p] and log[4] == ["goto_tpage", 0x140, 0x100, 0xD, 0, 2] and log[9] == ["goto_tpage", 0x200, 0x100, 0xD, 0, 2], "opaque sprites into slot 13, shown flag 1 for layer 2: tpages 0x140 and 0x200 at y 0x100")
	var r: int = p + 0x14 * 4 + 0xC * 4 * 2 * 0
	# the right sprite (second SPRT) follows goto_tpage's allocations: 0x14 sprite, then 0xC draw mode + 0xC tile
	var second: int = log[8][2]
	s.check([m.s16(second + 8), m.s16(second + 0xA), m.s16(second + 0x10), m.s16(second + 0x12)] == [100 + 0x20, -20 - 0x78, 0x80, 0xF0], "right sprite: x + 0x20, width 0x80")
	log.clear()
	e.call_c("dec_bg_draw", [0])
	var s0: int = log[3][2]
	s.check([m.s16(s0 + 0xA), m.u8(s0 + 0xD), m.s16(s0 + 0x12)] == [-20 - 0x30, 0xA0, 0x58] and log[4] == ["goto_tpage", 0x2C0, 0, 0xD, 0, 2], "other modes: y - 0x30, v0 0xA0, height 0x58")
	m.write_u8(0x800B5938, 1)
	log.clear()
	e.call_c("dec_bg_draw", [0])
	s.check(log[4] == ["goto_tpage", 0x140, 0x100, 0xD, 0, 2] and log[9] == ["goto_tpage", 0x200, 0x100, 0xD, 0, 2], "shown flag set: the 0x140 / 0x200 buffers at y 0x100")

func _fade_tpages(log: Array) -> Array:
	return log.filter(func(x): return x[0] == "goto_tpage").map(func(x): return x.slice(1))

func _test_fades() -> void:
	var log: Array = []
	var e = _engine(log, SDK)
	var m = e.memory
	e.call_c("bg_fade_set", [0x20])
	m.write_u8(0x800B5939, 1)
	e.call_c("bg_fade_draw", [1])
	s.check(_fade_tpages(log) == [[0x140, 0x100, 0xA, 1, 2], [0x200, 0x100, 0xA, 1, 2], [0x140, 0x100, 0xD, 0, 2], [0x200, 0x100, 0xD, 0, 2]], "fade, shown: %s" % [_fade_tpages(log)])
	var sprites: Array = log.filter(func(x): return x[0] == "AddPrim")
	s.check(sprites.map(func(x): return x[1]) == [_ot(e, 10), _ot(e, 10), _ot(e, 13), _ot(e, 13)], "fade: slots 10, 10, 13, 13")
	var semi: Array = log.filter(func(x): return x[0] == "SetSemiTrans").map(func(x): return x[2])
	s.check(semi == [1, 1, 0, 0], "semi-transparent for the old picture only")
	var first: int = sprites[0][2]
	var third: int = sprites[2][2]
	s.check(s.bytes(e, first + 4, 3) == [0x60, 0x60, 0x60] and s.bytes(e, third + 4, 3) == [0x20, 0x20, 0x20], "greys 0x80 - fade and fade")
	s.check([m.s16(first + 8), m.s16(first + 0xA), m.s16(first + 0x10), m.s16(first + 0x12)] == [100 - 0xA0, -20 - 0x78, 0xC0, 0xA0], "arg0 == 1: top 0xA0 rows")
	log.clear()
	m.write_u8(0x800B5938, 0)
	e.call_c("bg_fade_draw", [0])
	s.check(_fade_tpages(log) == [[0x2C0, 0, 0xA, 1, 2], [0x380, 0, 0xA, 1, 2], [0x2C0, 0, 0xD, 0, 2], [0x380, 0, 0xD, 0, 2]], "fade, layer 0 not shown: the 0x2C0 / 0x380 buffers")
	var other: int = log.filter(func(x): return x[0] == "AddPrim")[0][2]
	s.check([m.s16(other + 0xA), m.u8(other + 0xD), m.s16(other + 0x12)] == [-20 - 0x30, 0xA0, 0x58], "arg0 != 1: lower 0x58 rows from v 0xA0")
	# crossfade: alternating
	log.clear()
	m.write_u8(0x800B5939, 1)
	e.call_c("bg_crossfade_draw", [1])
	s.check(_fade_tpages(log) == [[0x2C0, 0, 0xA, 1, 2], [0x380, 0, 0xA, 1, 2], [0x140, 0x100, 0xC, 0, 2], [0x200, 0x100, 0xC, 0, 2], [0x2C0, 0, 0xD, 0, 2], [0x380, 0, 0xD, 0, 2]], "crossfade, shown: %s" % [_fade_tpages(log)])
	s.check(log.filter(func(x): return x[0] == "AddPrim").map(func(x): return x[1]) == [_ot(e, 10), _ot(e, 10), _ot(e, 12), _ot(e, 12), _ot(e, 13), _ot(e, 13)], "crossfade slots 10, 10, 12, 12, 13, 13")
	log.clear()
	m.write_u8(0x800B5939, 0)
	e.call_c("bg_crossfade_draw", [1])
	s.check(_fade_tpages(log) == [[0x140, 0x100, 0xA, 1, 2], [0x200, 0x100, 0xA, 1, 2], [0x2C0, 0, 0xC, 0, 2], [0x380, 0, 0xC, 0, 2], [0x140, 0x100, 0xD, 0, 2], [0x200, 0x100, 0xD, 0, 2]], "crossfade, not shown: %s" % [_fade_tpages(log)])
