extends RefCounted
## T-39311: engine/gpu_effects.gd on invented memory. Expectations are derived by hand from src/main/gpu_effects.c.
## libgpu, libc and the other C files' functions are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF

func run() -> Array[String]:
	s.failures.clear()
	_test_gradient_line()
	_test_fill_rect()
	_test_bg_slot_setup()
	_test_disc_tiles()
	_test_glow_disc()
	_test_flashes()
	s.shutdown_all()
	return s.failures

func _ot(e, buffer: int, slot: int) -> int:
	return e.memory.global_address("draw_ot_tags") + (buffer << 10) + slot * 4

func _engine(log: Array, names: Array, results: Dictionary = {}) -> Object:
	var e = s.new_engine()
	s.record(e, names, log, results)
	e.memory.write_global("draw_buffer", 1)
	e.call_c("SetWorkBase", [0x80160000, 1])
	s.set_gs_s16(e, "draw_origin_x", 1, 100)
	s.set_gs_s16(e, "draw_origin_y", 1, -20)
	return e

func _test_gradient_line() -> void:
	var log: Array = []
	var e = _engine(log, ["AddPrim", "SetSemiTrans", "func_8009F11C"])
	var m = e.memory
	var p: int = 0x80160000
	# K&R narrowing: x0 = 0x10005 -> s16 5, y1 = 0xFFF6 -> -10; flags 0x1FF -> u8 0xFF: semi-trans (bit 7), slot 0x7F
	var r: int = e.call_c("prim_gradient_line", [0x10005, 2, 3, 0xFFF6, 0x112233, 0xFFAA5501, 0x1FF])
	s.check(r == 0, "returns 0 (the C never sets v0; the port returns 0)")
	s.check(log == [["func_8009F11C", p], ["SetSemiTrans", p, 1], ["AddPrim", _ot(e, 1, 0x7F), p]], "SetLineG2, SetSemiTrans(1), AddPrim(slot 0x7F): %s" % [log])
	s.check(s.bytes(e, p + 4, 3) == [0x11, 0x22, 0x33] and s.bytes(e, p + 0xC, 3) == [0xAA, 0x55, 0x01], "colours: c0 bytes 16..23, 8..15, 0..7; c1 likewise (the top byte of c1 is dropped)")
	s.check([m.s16(p + 8), m.s16(p + 0xA), m.s16(p + 0x10), m.s16(p + 0x12)] == [105, -18, 103, -30], "coordinates with origin (100, -20)")
	log.clear()
	e.call_c("prim_gradient_line", [0, 0, 0, 0, 0, 0, 0x05])
	s.check(log[1] == ["SetSemiTrans", p + 0x14 * 4, 0] and log[2][1] == _ot(e, 1, 5), "bit 7 clear: SetSemiTrans(line, 0); slot 5")

func _test_fill_rect() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["prim_gradient_rect"], log)
	# RECT {x = -5, y = 7, w = 0x20, h = -3} in a1 = x | y << 16, a2 = w | h << 16 (each field s16-extended)
	e.call_c("prim_fill_rect", [0x1234, (7 << 16) | 0xFFFB, (0xFFFD << 16) | 0x20, 0xABCDEF])
	s.check(log == [["prim_gradient_rect", M32 & -5, 7, 0x20, M32 & -3, 1, 0xABCDEF, 1]], "prim_gradient_rect(x, y, w, h, 1, arg3, 1): %s" % [log])

func _test_bg_slot_setup() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["load_tpage_buf_lock", "func_8009ED74", "printf"], log, {"func_8009ED74": 0x7777})
	var m = e.memory
	var tex: int = 0x80180000
	for i: int in 4:
		m.write_s32(tex + i * 4, 0x100 + i)
	e.call_c("prim_bg_slot_setup", [tex, 1, 99, 2, 2, 5, -3, 4, 0x33, 0x10])
	var bg: int = m.global_address("bg_layers") + 0x24
	var map: int = m.global_address("layer_bg_maps") + 0x10
	var cells: int = m.global_address("layer_bg_cells") + 0x200
	var idx: int = m.global_address("layer_bg_cell_indices") + 0x80
	s.check(m.u32(bg) == 0x1000000 and m.s16(bg + 4) == -3 and m.s16(bg + 6) == 4 and m.s16(bg + 8) == 2 << 7 and m.s16(bg + 0xA) == 2 * 0x78, "BgLayer: flags, x, y, w << 7, h * 0x78")
	s.check(m.s16(bg + 0xC) == 0 and m.s16(bg + 0xE) == 0 and s.bytes(e, bg + 0x10, 3) == [0x80, 0x80, 0x80], "zeros and the 0x80 colour")
	s.check(m.u32(bg + 0x14) == map and m.s16(bg + 0x1C) == 0x1000 and m.s16(bg + 0x1E) == 0x1000 and m.u32(bg + 0x20) == 0, "map pointer, scales")
	s.check([m.u8(map), m.u8(map + 1), m.u16(map + 2), m.u16(map + 4), m.u32(map + 8), m.u32(map + 0xC)] == [0x80, 0x78, 2, 2, cells, idx], "BgMap of slot 1")
	s.check(log.slice(0, 4) == [["load_tpage_buf_lock", 0x10, 0x100, 1], ["load_tpage_buf_lock", 0x11, 0x101, 1], ["load_tpage_buf_lock", 0x30, 0x102, 1], ["load_tpage_buf_lock", 0x31, 0x103, 1]],
		"load_tpage_buf_lock(tp + col + row * 32, tex[row * w + col], 1) row by row: %s" % [log.slice(0, 4)])
	s.check(log.slice(4) == [["func_8009ED74", 0, 5], ["func_8009ED74", 0, 5], ["func_8009ED74", 0, 5], ["func_8009ED74", 0, 5]], "GetClut(0, clut) per cell, no printf for 4 cells")
	for i: int in 4:
		var row: int = i / 2
		var col: int = i % 2
		s.check(m.u16(idx + i * 2) == i and m.u8(cells + i * 8) == 0 and m.u8(cells + i * 8 + 1) == row * 0x78 and m.u16(cells + i * 8 + 6) == 0x10 + col
			and m.u16(cells + i * 8 + 2) == 0x7777 and m.u16(cells + i * 8 + 4) == 0, "cell %d: index, u, v = row * 0x78, tpage = tp + col, cba, flag" % i)
	s.check(m.u8(e.gs_address("bg_layer_order") + 1) == 0x33, "bg_layer_order[1] = flag")
	# more than 0x41 cells: BG_ERROR for every cell index above 0x40
	log.clear()
	e.call_c("prim_bg_slot_setup", [tex, 0, 0, 9, 8, 0, 0, 0, 1, 0])
	var printed: Array = log.filter(func(x): return x[0] == "printf")
	s.check(printed.size() == 7 and printed[0] == ["printf", 0x800AFAE0], "72 cells (index 0..0x47): printf(BG_ERROR) for index 0x41..0x47 = 7 times: %d" % printed.size())
	# a refusal in the first loop stops before any cell is written
	var e2 = s.new_engine()
	e2.memory.write_s32(0x80180000, 1)
	e2.call_c("prim_bg_slot_setup", [0x80180000, 0, 0, 1, 1, 0, 0, 0, 7, 0])
	s.check(e2.halted() and e2.refusal.contains("load_tpage_buf_lock") and e2.memory.u8(e2.gs_address("bg_layer_order")) == 0, "unported load_tpage_buf_lock refuses; the later stores do not land")

func _test_disc_tiles() -> void:
	var log: Array = []
	var rands: Array = [9, 6]
	var e = _engine(log, ["AddPrim", "SetSemiTrans", "func_8009F068", "func_8009ECB0", "func_8009ED74"], {"func_8009ECB0": 0x11234, "func_8009ED74": 0x4321})
	e.set_stub("rand", func(_args: Array) -> int:
		log.append(["rand"])
		return rands.pop_front() if not rands.is_empty() else 0)
	var m = e.memory
	var p: int = 0x80160000
	# radius 8, step 8 x 8 (0x85 & 0x7F = 5 -> 8): only the centre (0, 0) is inside (the others are at distance 8)
	e.call_c("prim_disc_tiles", [0, 0, 8, 0x85, 0x80, 3])
	s.check(log.map(func(x): return x[0]) == ["func_8009F068", "SetSemiTrans", "func_8009ECB0", "rand", "rand", "func_8009ED74", "AddPrim"], "one tile: %s" % [log])
	s.check(log[1] == ["SetSemiTrans", p, 0] and log[2] == ["func_8009ECB0", 0, 0, 0x3C0, 0x100], "SetSemiTrans(poly, 0), GetTPage(0, 0, 0x3C0, 0x100)")
	s.check(log[5] == ["func_8009ED74", 100 + 0 + 8, M32 & (0 + -20 + 0)], "clut position: origin + coordinate + (rand() & 8): first rand -> x (9 & 8 = 8), second -> y (6 & 8 = 0)")
	s.check(log[6] == ["AddPrim", _ot(e, 1, 3), p], "AddPrim slot 3")
	s.check([m.s16(p + 8), m.s16(p + 0xA), m.s16(p + 0x14), m.s16(p + 0x16), m.s16(p + 0x20), m.s16(p + 0x22), m.s16(p + 0x2C), m.s16(p + 0x2E)]
		== [96, -24, 104, -24, 96, -16, 104, -16], "quad corners: (x - 4, y - 4) with 8 x 8 size, origin (100, -20)")
	s.check([m.u8(p + 0xC), m.u8(p + 0xD), m.u8(p + 0x18), m.u8(p + 0x19), m.u8(p + 0x24), m.u8(p + 0x25), m.u8(p + 0x30), m.u8(p + 0x31)] == [0xC0, 0x50, 0xD0, 0x50, 0xC0, 0x60, 0xD0, 0x60], "uv 0xC0, 0x50 + 0x10")
	s.check(s.bytes(e, p + 4, 3) == [0x80, 0x80, 0x80] and s.bytes(e, p + 0x28, 3) == [0x80, 0x80, 0x80], "grey 0x80")
	s.check(m.u16(p + 0x1A) == 0x1234 and m.u16(p + 0xE) == 0x4321, "tpage word (u16 of GetTPage) and clut word")
	# radius 16, step 8: 9 tiles, x outer, y inner
	log.clear()
	e.call_c("prim_disc_tiles", [0, 0, 16, 8, 8, 3])
	var adds: Array = log.filter(func(x): return x[0] == "AddPrim")
	s.check(adds.size() == 9, "radius 16 step 8: dx, dy in {-8, 0, 8} with dx^2 + dy^2 < 256 = 9 tiles, got %d" % adds.size())
	var first: int = p + 0x34 * 4
	s.check(m.s16(first + 8) == 100 - 8 - 4 and m.s16(first + 0xA) == -20 - 8 - 4, "first tile is x = -8, y = -8 (loops start at centre - radius: -16 is outside the disc)")
	# a refusal (rand not ported)
	var e2 = s.new_engine()
	s.record(e2, ["AddPrim", "SetSemiTrans", "func_8009F068", "func_8009ECB0", "func_8009ED74"], [])
	e2.memory.write_global("draw_buffer", 0)
	e2.call_c("SetWorkBase", [0x80160000, 0])
	e2.call_c("prim_disc_tiles", [0, 0, 8, 8, 8, 0])
	s.check(e2.halted() and e2.refusal.contains("rand"), "rand is not ported: refusal by name (%s)" % e2.refusal)

func _test_glow_disc() -> void:
	var log: Array = []
	var e = _engine(log, ["AddPrim", "SetSemiTrans", "func_8009F0F4", "dtd_on_tpage"])
	var m = e.memory
	# radius 4, steps 0 and 1 are raised to 2: x, y in {-4, -2, 0, 2}, dx^2 + dy^2 < 16: 9 tiles
	e.call_c("prim_glow_disc", [0, 0, 4, 0, 1, 0x804020, 7, 5])
	var adds: Array = log.filter(func(x): return x[0] == "AddPrim")
	s.check(adds.size() == 9, "9 glow tiles, got %d" % adds.size())
	s.check(log[-1] == ["dtd_on_tpage", 0, 0, 5, 7 & 3, 0], "dtd_on_tpage(0, 0, arg7, arg6 & 3, 0) last: %s" % [log[-1]])
	s.check(log[0] == ["func_8009F0F4", 0x80160000] and log[1] == ["SetSemiTrans", 0x80160000, 1] and log[2] == ["AddPrim", _ot(e, 1, 5), 0x80160000], "SetTile, SetSemiTrans(1), AddPrim")
	# the first tile is x = -2, y = -2 (x = -4 is outside for every y)
	var t0: int = 0x80160000
	s.check([m.s16(t0 + 8), m.s16(t0 + 0xA), m.s16(t0 + 0xC), m.s16(t0 + 0xE)] == [100 - 2 - 1, -20 - 2 - 1, 2, 2], "tile at (-2, -2) minus half the step, size = x step for both sides")
	# (-2, -2): sum = 2 * 4 - 2 - 2 = 4: r = 4 * 0x80 / 4 = 128, g = 64, b = 32
	s.check(s.bytes(e, t0 + 4, 3) == [128, 64, 32], "colour fades with the distance: %s" % [s.bytes(e, t0 + 4, 3)])
	# the centre tile (0, 0): sum = 8: r = 8 * 0x80 / 4 = 256 -> 255, g = 128, b = 64
	var centre: int = -1
	for i: int in adds.size():
		var tile: int = adds[i][2]
		if m.s16(tile + 8) == 100 - 1 and m.s16(tile + 0xA) == -20 - 1:
			centre = tile
	s.check(centre != -1 and s.bytes(e, centre + 4, 3) == [255, 128, 64], "centre tile saturates red at 255 (r >= 256), g 128, b 64")
	# only the x step sets both sizes: arg3 = 4, arg4 = 2
	log.clear()
	e.call_c("prim_glow_disc", [0, 0, 4, 4, 2, 0x010203, 0, 0])
	var any: int = log.filter(func(x): return x[0] == "AddPrim")[0][2]
	s.check(m.s16(any + 0xC) == 4 and m.s16(any + 0xE) == 4, "w = h = arg3")

func _test_flashes() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["prim_gradient_rect", "dtd_on_tpage"], log)
	e.call_c("prim_flash_gray", [0x80])
	s.check(log == [["prim_gradient_rect", M32 & -0xA0, M32 & -0x78, 0xA0, 0xF0, 1, 0x808080, 0x81], ["prim_gradient_rect", 0, M32 & -0x78, 0xA0, 0xF0, 1, 0x808080, 0x81], ["dtd_on_tpage", 0, 0, 1, 1, 1]],
		"level 0x80: colour 0x808080 (level * 0x10101), two half boxes, draw mode: %s" % [log])
	log.clear()
	e.call_c("prim_flash_gray", [0x100])
	s.check(log[0][6] == 0xFFFFFF, "level 0x100 and above: white")
	log.clear()
	e.call_c("prim_flash_gray", [-1])
	s.check(log[0][6] == (M32 & -0x10101), "level -1 is below 0x100 (signed compare): colour -0x10101")
	log.clear()
	e.call_c("prim_flash_color_wide", [0x123456])
	s.check(log == [["prim_gradient_rect", M32 & -0x100, M32 & -0x78, 0x100, 0xF0, 1, 0x123456, 0x81], ["prim_gradient_rect", 0, M32 & -0x78, 0x100, 0xF0, 1, 0x123456, 0x81], ["dtd_on_tpage", 0, 0, 1, 1, 1]],
		"wide flash: two 0x100-wide boxes in the colour, then the draw mode")
