extends RefCounted
## T-39311: cd_iris_texture_build and cd_iris_wipe_draw of engine/reset_iris_wipe.gd on invented memory. Expectations
## are derived by hand from src/main/reset_iris_wipe.c. libgpu calls, LoadSquare and dtd_on_tpage are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF

func run() -> Array[String]:
	s.failures.clear()
	_test_texture()
	_test_wipe()
	s.shutdown_all()
	return s.failures

func _word(e, base: int, x: int, y: int) -> int:
	return e.memory.u32(base + (x / 8 + y * 16) * 4)

func _test_texture() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["LoadSquare"], log)
	var m = e.memory
	var base: int = 0x80180000
	for i: int in 2048 + 4:
		m.write_u32(base + i * 4, 0xFFFFFFFF)
	# flat colour 5: pixels with x^2 + y^2 < 0x3F01 (radius 127 excluded) get colour 5 in nibble x % 8
	e.call_c("cd_iris_texture_build", [base, 0x25, 0x05])
	s.check(m.u32(base + 2048 * 4) == 0xFFFFFFFF, "exactly 2048 words are rewritten (128 rows of 16 words)")
	s.check(_word(e, base, 0, 0) == 0x55555555 and _word(e, base, 120, 0) == 0x05555555, "row 0: colour 5 up to x = 126 (126^2 = 15876 < 16129), x = 127 (16129) outside")
	s.check(m.u32(base + (0 + 127 * 16) * 4) == 0 and (m.u32(base + (0 + 126 * 16) * 4) & 0xF) == 5, "(0, 127): 16129 not below the limit: empty; (0, 126): inside")
	s.check(_word(e, base, 127, 127) == 0, "far corner empty")
	s.check(log == [["LoadSquare", 0x140, 0x80, 32, 128, base]], "LoadSquare(page x = (0x25 & 15) << 6, y = (0x25 & 0x10) * 16 + (0x25 & 0x20) * 4, 32, 128, buffer): %s" % [log])
	# colour 0: the pixels with 16130 or more get colour 1, the inside stays 0
	e.call_c("cd_iris_texture_build", [base, 0, 0x00])
	s.check(_word(e, base, 0, 0) == 0 and _word(e, base, 120, 0) == 0 and _word(e, base, 120, 1) == 0x10000000, "colour 0: (127, 0) (16129) stays 0, (127, 1) (16130) gets 1 in nibble 7")
	s.check(_word(e, base, 120, 127) & 0xF0000000 == 0x10000000, "(127, 127) is outside")
	# radial shade (arg2 / 16 & 0xFF nonzero): the first shade from 15 down whose ring (15 - shade)^2 * 64 holds x^2 + y^2
	e.call_c("cd_iris_texture_build", [base, 0, 0x80])
	s.check(_word(e, base, 0, 0) & 0xF == 14, "(0, 0): shade 15 needs 0 < 0 (no), shade 14 needs 0 < 64: 14")
	s.check(_word(e, base, 8, 0) & 0xF == 13, "(8, 0): distance^2 64: shade 14 needs 64 < 64 (no), 13 needs 64 < 256: 13")
	s.check((_word(e, base, 96, 0) >> 16) & 0xF == 2, "(100, 0): 10000 < (15 - 2)^2 * 64 = 10816 but not < 9216 (shade 3): 2")
	s.check(((_word(e, base, 127, 127) >> 28) & 0xF) == 0, "(127, 127) is beyond every ring: 0")
	# arg2 / 16 & 0xFF: 0x0F is flat (colour 15 everywhere inside), 0x10 is radial
	e.call_c("cd_iris_texture_build", [base, 0, 0x0F])
	s.check(_word(e, base, 0, 0) == 0xFFFFFFFF, "arg2 = 0x0F: flat colour 15")
	e.call_c("cd_iris_texture_build", [base, 0, 0x10])
	s.check(_word(e, base, 0, 0) & 0xF == 14, "arg2 = 0x10: radial")

func _test_wipe() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["func_8009F068", "SetSemiTrans", "SetShadeTex", "func_8009F0F4", "func_8009ECB0", "func_8009ED74", "AddPrim", "dtd_on_tpage"], log, {"func_8009ECB0": 0x11, "func_8009ED74": 0x22})
	var m = e.memory
	m.write_global("draw_buffer", 1)
	e.call_c("SetWorkBase", [0x80160000, 1])
	s.set_gs_s16(e, "draw_origin_x", 1, 100)
	s.set_gs_s16(e, "draw_origin_y", 1, -20)
	var ot: int = m.global_address("draw_ot_tags") + 1024 + 0xB * 4
	# radius = (0x800 << 7) / 4096 = 64; colour 0xF1223344: top nibble set -> semi-transparent, r = 0x22, g = 0x33, b = 0x44
	e.call_c("cd_iris_wipe_draw", [0x25, 0x10, 0x1F0, 0x800, 0, 0, 0xF1223344, 0x1B])
	var prims: Array = log.filter(func(x): return x[0] == "func_8009F068").map(func(x): return x[1])
	s.check(prims.size() == 4 and log.filter(func(x): return x[0] == "SetSemiTrans").slice(0, 4).all(func(x): return x[2] == 1), "four quadrant polygons, semi-transparent")
	var q0: int = prims[0]
	s.check(s.bytes(e, q0 + 4, 3) == [0x22, 0x33, 0x44] and s.bytes(e, q0 + 0x28, 3) == [0x22, 0x33, 0x44], "vertex colours from arg6")
	# quadrant 0: (ox + 0, oy + 0), (+64, 0), (0, +64), (+64, +64)
	s.check([m.s16(q0 + 8), m.s16(q0 + 0xA), m.s16(q0 + 0x14), m.s16(q0 + 0x16), m.s16(q0 + 0x20), m.s16(q0 + 0x22), m.s16(q0 + 0x2C), m.s16(q0 + 0x2E)] == [100, -20, 164, -20, 100, 44, 164, 44], "quadrant 0 corners")
	s.check([m.u8(q0 + 0xC), m.u8(q0 + 0xD), m.u8(q0 + 0x18), m.u8(q0 + 0x19), m.u8(q0 + 0x24), m.u8(q0 + 0x25), m.u8(q0 + 0x30), m.u8(q0 + 0x31)] == [0, 0x80, 0x7F, 0x80, 0, 0xFF, 0x7F, 0xFF], "uv: v from (0x25 & 0x20) * 4 = 0x80, +0x7F")
	s.check(m.u16(q0 + 0x1A) == 0x11 and m.u16(q0 + 0xE) == 0x22, "tpage and clut words")
	var calls: Array = log.filter(func(x): return x[0] == "func_8009ECB0")
	s.check(calls[0] == ["func_8009ECB0", 0, 0, 0x140, 0x80] and log.filter(func(x): return x[0] == "func_8009ED74")[0] == ["func_8009ED74", 0x10, 0x1F0], "GetTPage(0, 0, (arg0 & 0xF) << 6, ...), GetClut(arg1, arg2)")
	var q1: int = prims[1]
	var q2: int = prims[2]
	var q3: int = prims[3]
	s.check([m.s16(q1 + 0x14), m.s16(q1 + 0x22), m.s16(q1 + 0x2C), m.s16(q1 + 0x2E)] == [164, -84, 164, -84], "quadrant 1: x +64, y -64")
	s.check([m.s16(q2 + 0x14), m.s16(q2 + 0x22), m.s16(q2 + 0x2C), m.s16(q2 + 0x2E)] == [36, 44, 36, 44], "quadrant 2: x -64, y +64")
	s.check([m.s16(q3 + 0x14), m.s16(q3 + 0x22), m.s16(q3 + 0x2C), m.s16(q3 + 0x2E)] == [36, -84, 36, -84], "quadrant 3: x -64, y -64")
	var adds: Array = log.filter(func(x): return x[0] == "AddPrim")
	s.check(adds.size() == 8 and adds[0][1] == ot and log.filter(func(x): return x[0] == "dtd_on_tpage").size() == 4 and log.filter(func(x): return x[0] == "dtd_on_tpage")[0] == ["dtd_on_tpage", 0, 0, 0xB, 1, 0], "4 polygons + 4 tiles added to slot 0xB; dtd_on_tpage(0, 0, order, 1, 0) after each polygon")
	var tiles: Array = log.filter(func(x): return x[0] == "func_8009F0F4").map(func(x): return x[1])
	s.check(tiles.size() == 4, "four tiles")
	var t0: int = tiles[0]
	s.check(s.bytes(e, t0 + 4, 3) == [0x22, 0x33, 0x44] and [m.s16(t0 + 8), m.s16(t0 + 0xA), m.s16(t0 + 0xC), m.s16(t0 + 0xE)] == [100 - 0xA0, -20 - 0x78, 0x140, (0 - 64) + 0x78], "tile 0: above the iris: height arg5 - radius + 0x78 = 56")
	s.check([m.s16(tiles[1] + 0xC), m.s16(tiles[1] + 0xE)] == [96, 0xF0], "tile 1: width arg4 - radius + 0xA0 = 96, height 0xF0")
	s.check([m.s16(tiles[2] + 0xA), m.s16(tiles[2] + 0xE), m.s16(tiles[2] + 0xC)] == [-20 + 0 + 64, 56, 0x140], "tile 2: y = oy + arg5 + radius, height -arg5 - radius + 0x78")
	s.check([m.s16(tiles[3] + 8), m.s16(tiles[3] + 0xC), m.s16(tiles[3] + 0xE)] == [100 + 64, 96, 0xF0], "tile 3: x = ox + arg4 + radius, width -arg4 - radius + 0xA0")
	# a big iris: only the right bar has a positive extent (the C allocates all four tiles but adds one)
	log.clear()
	e.call_c("cd_iris_wipe_draw", [0x25, 0, 0, 0x2000, -0xA0, -0x78, 0x01000000, 0x03])
	adds = log.filter(func(x): return x[0] == "AddPrim")
	s.check(log.filter(func(x): return x[0] == "func_8009F0F4").size() == 4 and adds.size() == 4 + 1, "radius 256: 4 tiles allocated, only tile 3 (extent 64) added: %d AddPrim" % adds.size())
	# no colour (alpha nibble 0): grey 0x80, opaque, no tiles
	log.clear()
	e.call_c("cd_iris_wipe_draw", [0, 0, 0, 0x800, 0, 0, 0, 0xFF])
	var g0: int = log.filter(func(x): return x[0] == "func_8009F068")[0][1]
	s.check(s.bytes(e, g0 + 4, 3) == [0x80, 0x80, 0x80] and log.filter(func(x): return x[0] == "SetSemiTrans")[0][2] == 0 and log.filter(func(x): return x[0] == "func_8009F0F4").is_empty() and log.filter(func(x): return x[0] == "AddPrim")[0][1] == m.global_address("draw_ot_tags") + 1024 + 0xF * 4, "arg6 = 0: grey, opaque, slot arg7 & 0xF = 0xF, no tiles")
