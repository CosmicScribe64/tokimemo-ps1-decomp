extends RefCounted
## T-39311: engine/gpu_primitives.gd on invented memory. Every expectation is derived by hand from
## src/main/gpu_primitives.c (not from the port). libgpu calls are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Layout = preload("res://core/game_layout.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const SDK: Array = ["AddPrim", "SetSemiTrans", "SetShadeTex", "func_8009F040", "func_8009F004", "func_8009F054",
	"func_8009F0B8", "func_8009D294", "func_8009ECB0", "func_8009ED74"]

func run() -> Array[String]:
	s.failures.clear()
	_test_work_base()
	_test_sprite_ft4()
	_test_tpage_and_tiles()
	_test_light_effect()
	_test_gradient_rect()
	s.shutdown_all()
	return s.failures

func _ot(engine, buffer: int, slot: int) -> int:
	return engine.memory.global_address("draw_ot_tags") + (buffer << 10) + slot * 4

func _test_work_base() -> void:
	var e = s.new_engine()
	e.call_c("SetWorkBase", [0x80150000, 1])
	var a: int = e.gs_address("prim_arena") + 12
	s.check([e.memory.u32(a), e.memory.u32(a + 4), e.memory.u32(a + 8)] == [0x10C00, 0, 0x80150000], "SetWorkBase(base, 1): size 0x10C00, used 0, base")
	s.check(e.memory.s32(e.gs_address("prim_arena") + 8) == 0, "SetWorkBase(_, 1) leaves arena 0 alone")
	s.check((e.call_c("GetWorkBase", [0x28, 1]) & 0xFFFFFFFF) == 0x80150000, "GetWorkBase returns base + used * 4 (used 0)")
	s.check(e.memory.s32(a + 4) == 0x28, "GetWorkBase(0x28): used += ((0x28 + 3) / 4) * 4 = 0x28")
	s.check((e.call_c("GetWorkBase", [0xC, 1]) & 0xFFFFFFFF) == 0x80150000 + 0x28 * 4, "second allocation at base + used * 4")
	s.check(e.memory.s32(a + 4) == 0x28 + 0xC, "used = 0x34")
	e.call_c("GetWorkBase", [1, 1])
	s.check(e.memory.s32(a + 4) == 0x34 + 4, "(1 + 3) / 4 * 4 = 4")
	e.call_c("GetWorkBase", [5, 1])
	s.check(e.memory.s32(a + 4) == 0x38 + 8, "(5 + 3) / 4 * 4 = 8")
	e.call_c("GetWorkBase", [-1, 1])
	s.check(e.memory.s32(a + 4) == 0x40 + 0, "(-1 + 3) / 4 * 4 = 0 (2 / 4 truncates to 0)")
	e.call_c("GetWorkBase", [-8, 1])
	s.check(e.memory.s32(a + 4) == 0x40 - 4, "(-8 + 3) / 4 = -1 (truncation toward zero): used -= 4")

func _setup_prim_engine(log: Array, results: Dictionary = {}) -> Object:
	var e = s.new_engine()
	s.record(e, SDK, log, results)
	e.memory.write_global("draw_buffer", 1)
	e.call_c("SetWorkBase", [0x80160000, 1])
	e.call_c("SetWorkBase", [0x80140000, 0])
	s.set_gs_s16(e, "draw_origin_x", 1, 100)
	s.set_gs_s16(e, "draw_origin_y", 1, -20)
	s.set_gs_s16(e, "draw_origin_x", 0, 7)
	s.set_gs_s16(e, "draw_origin_y", 0, 9)
	return e

func _test_sprite_ft4() -> void:
	var log: Array = []
	var e = _setup_prim_engine(log, {"func_8009ED74": 0x1234})
	var q: int = 0x80170000
	var m = e.memory
	m.write_s32(q + Layout.SPRITE_QUAD_FLAGS, 0x40000000)
	m.write_s16(q + Layout.SPRITE_QUAD_X, 50)
	m.write_s16(q + Layout.SPRITE_QUAD_Y, 60)
	m.write_s16(q + Layout.SPRITE_QUAD_W, 0xC000) # (u16) of a negative s16 is 0xC000
	m.write_u16(q + Layout.SPRITE_QUAD_H, 16)
	m.write_s16(q + Layout.SPRITE_QUAD_UNK_0C, 0x15)
	m.write_u8(q + Layout.SPRITE_QUAD_UNK_0E, 0xF0)
	m.write_u8(q + Layout.SPRITE_QUAD_UNK_0F, 0x10)
	m.write_s16(q + Layout.SPRITE_QUAD_UNK_10, 3)
	m.write_s16(q + Layout.SPRITE_QUAD_UNK_12, 4)
	m.write_u8(q + Layout.SPRITE_QUAD_TINT_R, 0x11)
	m.write_u8(q + Layout.SPRITE_QUAD_TINT_G, 0x22)
	m.write_u8(q + Layout.SPRITE_QUAD_TINT_B, 0x33)
	m.write_s16(q + Layout.SPRITE_QUAD_ANCHOR_X, 5)
	m.write_s16(q + Layout.SPRITE_QUAD_ANCHOR_Y, 6)
	e.call_c("sprite_set_gpu_poly_ft4", [q, 9, 1])
	var p: int = 0x80160000
	s.check(log.size() == 5, "5 SDK calls: %s" % [log])
	s.check(log[0] == ["func_8009F040", p] and log[1] == ["SetSemiTrans", p, 1] and log[2] == ["SetShadeTex", p, 0], "init, semi-trans 1 (flag 0x40000000), shade-tex 0: %s" % [log.slice(0, 3)])
	s.check(log[3] == ["func_8009ED74", 3, 4], "GetClut(unk_10, unk_12)")
	s.check(log[4] == ["AddPrim", _ot(e, 1, 9), p], "AddPrim(draw_ot_tags + (1 << 10) + 9 * 4, p)")
	s.check(m.u32(e.gs_address("prim_arena") + 12 + 4) == 0x28, "0x28 bytes allocated from arena 1")
	var x0: int = 100 + 50 - 5
	var y0: int = -20 + 60 - 6
	s.check(s.bytes(e, p + 4, 3) == [0x11, 0x22, 0x33], "tint")
	s.check([m.s16(p + 8), m.s16(p + 0xA), m.s16(p + 0x10), m.s16(p + 0x12), m.s16(p + 0x18), m.s16(p + 0x1A), m.s16(p + 0x20), m.s16(p + 0x22)]
		== [x0, y0, Math.s16(x0 + 0xC000), y0, x0, y0 + 16, Math.s16(x0 + 0xC000), y0 + 16], "vertices (width is added as u16)")
	s.check([m.u8(p + 0xC), m.u8(p + 0xD), m.u8(p + 0x14), m.u8(p + 0x15), m.u8(p + 0x1C), m.u8(p + 0x1D), m.u8(p + 0x24), m.u8(p + 0x25)]
		== [0xF0, 0x10, (0xF0 + 0xC000) & 0xFF, 0x10, 0xF0, 0x20, (0xF0 + 0xC000) & 0xFF, 0x20], "u/v bytes wrap at 8 bits")
	s.check(m.u16(p + 0x16) == 0x95 and m.u16(p + 0xE) == 0x1234, "tpage word (unk_0C | 0x80), clut word from the callee")
	# flag bit 31: nothing at all
	log.clear()
	m.write_s32(q + Layout.SPRITE_QUAD_FLAGS, 0x80000000)
	e.call_c("sprite_set_gpu_poly_ft4", [q, 9, 1])
	s.check(log.is_empty() and m.u32(e.gs_address("prim_arena") + 12 + 4) == 0x28, "flags bit 31: no allocation, no SDK call")
	# no semi-trans bit
	m.write_s32(q + Layout.SPRITE_QUAD_FLAGS, 0)
	e.call_c("sprite_set_gpu_poly_ft4", [q, 2, 1])
	s.check(log[1][0] == "SetSemiTrans" and log[1][2] == 0, "SetSemiTrans(p, 0) without bit 30")
	# a refusal inside a libgpu call (a double that refuses; the SDK layer now provides it, T-39314)
	var e2 = s.new_engine()
	e2.set_stub("func_8009F040", func(_a: Array) -> int:
		e2.refuse("func_8009F040", "double")
		return 0)
	e2.memory.write_global("draw_buffer", 0)
	e2.call_c("SetWorkBase", [0x80140000, 0])
	e2.call_c("sprite_set_gpu_poly_ft4", [q, 0, 0])
	s.check(e2.halted() and e2.refusal.contains("func_8009F040"), "a refusing libgpu call stops the run: %s" % e2.refusal)

func _test_tpage_and_tiles() -> void:
	var log: Array = []
	var e = _setup_prim_engine(log, {"func_8009ECB0": 0x1ABCD})
	var m = e.memory
	e.call_c("goto_tpage", [1, 2, 7, 3, 4])
	var w: int = 0x80160000
	s.check(log[0] == ["func_8009ECB0", 4, 3, 1, 2], "GetTPage(arg4, arg3, arg0, arg1)")
	s.check(log[1] == ["func_8009D294", w, 0, 0, 0xABCD, 0], "SetDrawMode(work, 0, 0, (u16) tpage, 0)")
	s.check(log[2] == ["AddPrim", _ot(e, 1, 7), w], "draw mode primitive into slot 7")
	s.check(log[3] == ["func_8009F0B8", w + 0x30] and log[4] == ["AddPrim", _ot(e, 1, 7), w + 0x30] and log.size() == 5, "safe_env(7): tile after the draw mode: %s" % [log])
	s.check(m.s16(w + 0x30 + 8) == 0 and m.s16(w + 0x30 + 0xA) == 0, "tile at 0, 0")
	log.clear()
	m.write_s16(w + 0x90 + 8, 77)
	e.call_c("dtd_on_tpage", [1, 2, 5, 3, 4])
	s.check(log[0] == ["func_8009ECB0", 4, 3, 1, 2] and log[1] == ["func_8009D294", w + 0x60, 0, 1, 0x1ABCD, 0], "dtd_on_tpage: dither 1 and the full (not narrowed) tpage: %s" % [log.slice(0, 2)])
	s.check(log[2] == ["AddPrim", _ot(e, 1, 5), w + 0x60] and log[3][0] == "func_8009F0B8", "AddPrim then safe_env")
	log.clear()
	e.call_c("dtd_on", [3])
	s.check(log[0][0] == "func_8009D294" and log[0].slice(2) == [0, 1, 0, 0] and log[1][0] == "AddPrim" and log[1][1] == _ot(e, 1, 3), "dtd_on: SetDrawMode(work, 0, 1, 0, 0), AddPrim slot 3")
	s.check(log.size() == 4 and log[2][0] == "func_8009F0B8" and log[3][0] == "AddPrim", "dtd_on ends with safe_env")

func _test_light_effect() -> void:
	var log: Array = []
	var e = _setup_prim_engine(log)
	var m = e.memory
	e.gs_set("frame_count", 0) # rsin(0) = 0 and abs(rsin(arg8 * 0xFF0)) with arg8 = 0 is 0
	e.call_c("_sprite_set_light_effect1", [10, 20, 30, 40, 0, 0, 6, 0x00804020, 0])
	var p: int = 0x80160000
	s.check(log[0] == ["func_8009F004", p] and log[1] == ["SetSemiTrans", p, 1], "init and semi-trans")
	# r = 0x80, g = 0x40, b = 0x20 with a zero pulse
	s.check(s.bytes(e, p + 4, 3) == [0x80 * 3 / 5, 0x40 * 3 / 5, 0x20 * 3 / 5], "vertex 0 is 3/5 of the colour")
	s.check(s.bytes(e, p + 0xC, 3) == [0, 0, 0] and s.bytes(e, p + 0x14, 3) == [0, 0, 0], "vertices 1 and 2 are black")
	s.check([m.s16(p + 8), m.s16(p + 0xA), m.s16(p + 0x10), m.s16(p + 0x12), m.s16(p + 0x18), m.s16(p + 0x1A)] == [110, 0, 130, 20, 130, 20],
		"arg4 == 0: base edge at arg2 -/+ 0 (zero width) at arg3, with origin (100, -20)")
	s.check(log[2] == ["AddPrim", _ot(e, 1, 6), p], "AddPrim slot 6")
	# a non-zero pulse: frame 1 -> rsin(0xFF0) (angle 4080 of 4096) is negative
	log.clear()
	e.gs_set("frame_count", 4) # 4 * 0xFF0 = 0x3FC0, angle 0xFC0: rsin = -402, pulse = -402 * 32 / 4096 = -3 (floor would be -4)
	e.call_c("_sprite_set_light_effect1", [1, 2, 3, 4, 50, 60, 0, 0x00FF0000, 0])
	var sine: int = Math.rsin(0x3FC0)
	var pulse: int = (sine << 5) / 4096
	s.check(sine < 0 and pulse != 0, "the test needs a negative pulse (rsin(0x3FC0) = %d)" % sine)
	var p2: int = p + 0x1C * 4
	s.check(s.bytes(e, p2 + 4, 3) == [((255 + pulse) * 3 / 5) & 0xFF, ((0 + pulse) * 3 / 5) & 0xFF, ((0 + pulse) * 3 / 5) & 0xFF],
		"pulse added to each channel; a negative product / 5 truncates toward zero and the store keeps the low byte")
	s.check([m.s16(p2 + 8), m.s16(p2 + 0xA), m.s16(p2 + 0x10), m.s16(p2 + 0x12), m.s16(p2 + 0x18), m.s16(p2 + 0x1A)] == [101, -18, 103, -16, 150, 40],
		"arg4 != 0: the three vertices are given")
	# width with arg8: s = abs(rsin((fc + arg8) * 0xFF0)); w = s * 8 / 4096
	log.clear()
	e.gs_set("frame_count", 0)
	e.call_c("_sprite_set_light_effect1", [0, 0, 100, 10, 0, 0, 1, 0, 0x40]) # 0x40 * 0xFF0 = 0x3FC00, angle 0xC00: rsin = -4096
	var width: int = 4096 * 8 / 4096
	s.check(Math.rsin(0xC00) == -4096, "table: rsin(0xC00) = -4096")
	var p3: int = p + 0x38 * 4
	s.check(m.s16(p3 + 0x10) == 100 + 100 - width and m.s16(p3 + 0x18) == 100 + 100 + width, "base edge half width = abs(rsin) * 8 / 4096 = %d" % width)

func _test_gradient_rect() -> void:
	var log: Array = []
	var e = _setup_prim_engine(log)
	var m = e.memory
	var p: int = 0x80160000
	var cases: Dictionary = {
		6: [[0, 0, 0], [0x80, 0x40, 0x20], [0, 0, 0], [0x80, 0x40, 0x20]],
		5: [[0x80, 0x40, 0x20], [0, 0, 0], [0x80, 0x40, 0x20], [0, 0, 0]],
		4: [[0, 0, 0], [0, 0, 0], [0x80, 0x40, 0x20], [0x80, 0x40, 0x20]],
		3: [[0, 0, 0], [0, 0, 0], [0, 0, 0], [0, 0, 0]],
		1: [[0x80, 0x40, 0x20], [0x80, 0x40, 0x20], [0x80, 0x40, 0x20], [0x80, 0x40, 0x20]],
		2: [[0x80, 0x40, 0x20], [0x80, 0x40, 0x20], [0x80, 0x40, 0x20], [0x80 * 8 / 10, 0x40 * 8 / 10, 0x20 * 8 / 10]],
		0: [[(0x80 * 10) & 0xFF, (0x40 * 10) & 0xFF, (0x20 * 10) & 0xFF], [(0x80 * 8) & 0xFF, (0x40 * 8) & 0xFF, (0x20 * 8) & 0xFF], [(0x80 * 6) & 0xFF, (0x40 * 6) & 0xFF, (0x20 * 6) & 0xFF], [(0x80 * 4) & 0xFF, (0x40 * 4) & 0xFF, (0x20 * 4) & 0xFF]],
	}
	# case 3 stores r * 8, g * 8, g * 8 (blue gets green): 0x400 & 0xFF = 0, 0x200 & 0xFF = 0 -> all zero (invented colour below makes it visible)
	cases[3] = [[0, 0x40 * 8 & 0xFF, 0x40 * 8 & 0xFF]]
	var n: int = 0
	for mode: int in [6, 5, 4, 1, 2, 0, 99]:
		var base: int = p + n * 0x24 * 4
		var expect: Array = cases[mode] if cases.has(mode) else cases[0]
		log.clear()
		e.call_c("prim_gradient_rect", [10, 20, 30, 40, 4, 0x804020, mode | 0x80])
		var got: Array = []
		for v: int in 4:
			got.append(s.bytes(e, base + 4 + v * 8, 3))
		s.check(got == expect, "mode %d colours %s expected %s" % [mode, got, expect])
		s.check(log[0] == ["func_8009F054", base] and log[1] == ["SetSemiTrans", base, 1], "mode %d: init, semi-trans 1 (bit 7)" % mode)
		s.check(log[2] == ["AddPrim", _ot(e, 1, 4), base], "mode %d AddPrim slot 4" % mode)
		n += 1
	# case 3 with a colour whose r and g make the stores visible
	var base3: int = p + n * 0x24 * 4
	e.call_c("prim_gradient_rect", [0, 0, 1, 1, 0, 0x010203, 3])
	for off: int in [4, 0xC, 0x14, 0x1C]:
		s.check(s.bytes(e, base3 + off, 3) == [8, 16, 16], "case 3 writes r*8, g*8, g*8 to every vertex (offset %d)" % off)
	s.check(log[-2][0] == "SetSemiTrans" and log[-2][2] == 0, "bit 7 clear: SetSemiTrans(p, 0)")
	# geometry (origin 100, -20) of the first rectangle
	var b0: int = p
	s.check([m.s16(b0 + 8), m.s16(b0 + 0xA), m.s16(b0 + 0x10), m.s16(b0 + 0x12), m.s16(b0 + 0x18), m.s16(b0 + 0x1A), m.s16(b0 + 0x20), m.s16(b0 + 0x22)]
		== [110, 0, 140, 0, 110, 40, 140, 40], "vertex coordinates: (x, y), (x + w, y), (x, y + h), (x + w, y + h) plus the origin")
