extends RefCounted
## T-39311: engine/screen_layers.gd on invented memory. Expectations are derived by hand from
## src/main/screen_layers.c. libgpu/libgte/VRAM calls and the other files' functions are recording doubles;
## memcpy runs (libc is ported).

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF
const SDK: Array = ["AddPrim", "SetSemiTrans", "SetShadeTex", "func_8009F040", "func_8009F07C", "func_8009ECB0", "func_8009ED74"]

func run() -> Array[String]:
	s.failures.clear()
	_test_init_functions()
	_test_tpage_show()
	_test_move_255_line()
	_test_get_fnt()
	_test_mod_trans_vram()
	_test_srn_vram_set()
	_test_srn_set()
	_test_mod_set_gpu()
	_test_mod_set()
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

func _words(e, address: int, words: Array) -> void:
	for i: int in words.size():
		e.memory.write_u32(address + i * 4, words[i])

func _test_init_functions() -> void:
	var e = s.new_engine()
	var m = e.memory
	_words(e, 0x80180000, [0x1234AB56, 0x0A0B0C0D, 0x11223344, 0xDEADBEEF, 0xCAFEF00D])
	m.write_u32(m.global_address("layer_records") + 2 * 0x28 + 0x24, 0xFFFFFFFF)
	e.call_c("srn_init", [2, 0x80180000, 3])
	var p: int = m.global_address("layer_records") + 2 * 0x28
	s.check([m.u8(p + 0xD), m.s16(p + 2), m.s16(p + 6), m.u8(p + 0xC), m.u32(p + 8)] == [0xAB, 0, 0, 0, 0x80180014], "srn_init: +0xD = byte 1 of word 0, zeros, +8 = arg1 + 5 words")
	s.check(s.bytes(e, p + 0xE, 4) == [0x0A, 0x0B, 0x0C, 0x0D] and s.bytes(e, p + 0x12, 4) == [0x11, 0x22, 0x33, 0x44], "srn_init: bytes of words 1 and 2, high byte first")
	s.check([m.s16(p + 0x16), m.s16(p + 0x18), m.u32(p + 0x1C), m.u32(p + 0x20), m.u32(p + 0x24)] == [0, 0x1E0, 0xCAFEF00D, 0xDEADBEEF, m.global_address("layer_tileset_records") + 3 * 0x14],
		"srn_init: 0, 0x1E0, word 4 at +0x1C, word 3 at +0x20, tile-set record arg2")
	_words(e, 0x80181000, [0x7F123456, 0x04030201, 0x0000009A, 0x000000BC])
	m.write_u8(0x80181000 + 8, 0x9A)
	m.write_u8(0x80181000 + 0xC, 0xBC)
	e.call_c("layer_tileset_init", [1, 0x80181000, 0x80190000])
	var t: int = m.global_address("layer_tileset_records") + 0x14
	s.check([m.u32(t), m.u32(t + 4), m.u8(t + 8), m.s16(t + 0xA)] == [0x80190000, 0x80181010, 0x7F, 0], "layer_tileset_init: bank table, data = arg1 + 4 words, mode byte, +0xA = 0")
	s.check(s.bytes(e, t + 0xC, 4) == [4, 3, 2, 1] and m.s16(t + 0x10) == 0x9A and m.s16(t + 0x12) == 0xBC, "layer_tileset_init: cols, rows, size bytes and the two byte widths")
	var before: PackedByteArray = m.snapshot()
	e.call_c("func_8004DAC4", [0x1234])
	s.check(m.snapshot() == before, "func_8004DAC4 changes nothing")

func _test_tpage_show() -> void:
	var log: Array = []
	var e = _engine(log, SDK, {"func_8009ECB0": 0x11, "func_8009ED74": 0x22})
	var m = e.memory
	e.gs_set("screen_brightness", 0x70)
	# RECT {x = 10, y = 20, w = 300, h = 50}
	m.write_s16(0x80170000, 10)
	m.write_s16(0x80170002, 20)
	m.write_s16(0x80170004, 300)
	m.write_s16(0x80170006, 50)
	e.call_c("srn_tpage_show", [0x31, 1, 5, 0x80170000, 0x1234, 0, 7, 8, 4])
	var tp: Array = log.filter(func(x): return x[0] == "func_8009ECB0")
	# divisor 2, page width 0x80; page x = ((0x31 & 0xF) << 6) + (0x1234 & 0x7F00) / 2 = 0x40 + 0x900; y = 0x100 + 0x80
	s.check(tp == [["func_8009ECB0", 1, 1, 0x940, 0x180], ["func_8009ECB0", 1, 1, 0x3C0, 0], ["func_8009ECB0", 1, 1, 0x9C0, 0x180], ["func_8009ECB0", 1, 1, 0x3C0, 0], ["func_8009ECB0", 1, 1, 0xA40, 0x180]],
		"GetTPage(mode, semi & 3, page x [+ width, + 2 width], page y) for page, column, page, column, page: %s" % [tp])
	var adds: Array = log.filter(func(x): return x[0] == "AddPrim")
	var slot: int = _ot(e, 1, 4)
	s.check(adds.map(func(x): return x[1]) == [slot, slot - 4, slot, slot, slot], "AddPrim slots: arg8, arg8 - 1 for the first column, arg8 ... (%s)" % [adds.map(func(x): return x[1])])
	var p: int = 0x80160000
	s.check(s.bytes(e, p + 4, 3) == [0x70, 0x70, 0x70], "brightness")
	# left page: v = (0x31 & 0x20) * 4 = 0x80; x0 = 100 - 0x34 + 10 = 58, y0 = 0, width 0xFF, height 50
	s.check(s.bytes(e, p + 0xC, 2) == [0, 0x80] and s.bytes(e, p + 0x14, 2) == [0xFF, 0x80] and s.bytes(e, p + 0x1C, 2) == [0, 0xB2] and s.bytes(e, p + 0x24, 2) == [0xFF, 0xB2], "left page uv")
	s.check([m.s16(p + 8), m.s16(p + 0xA), m.s16(p + 0x10), m.s16(p + 0x12), m.s16(p + 0x18), m.s16(p + 0x1A), m.s16(p + 0x20), m.s16(p + 0x22)] == [58, 0, 313, 0, 58, 50, 313, 50], "left page xy")
	s.check(m.u16(p + 0x16) == 0x11 and m.u16(p + 0xE) == 0x22, "tpage and clut words")
	# first column: u = 0x1234 / 256 * 4 + 3 = 0x4B, v 0 and h; x0 = 100 + (0xFF - 0x34) + 10 = 313, x3 = 100 - 0x34 + 10 + 0x100 = 314
	var c: int = p + 0xA0
	s.check(s.bytes(e, c + 0xC, 2) == [0x4B, 0] and s.bytes(e, c + 0x14, 2) == [0x4C, 0] and s.bytes(e, c + 0x1C, 2) == [0x4B, 50] and s.bytes(e, c + 0x24, 2) == [0x4C, 50], "column uv")
	s.check([m.s16(c + 8), m.s16(c + 0x10), m.s16(c + 0x18), m.s16(c + 0x20)] == [313, 314, 313, 314], "column x: (0xFF - scroll), +1, and the x3 that uses -scroll + 0x100")
	# the second column has u + 7 / + 8
	var c2: int = p + 3 * 0xA0
	s.check(s.bytes(e, c2 + 0xC, 1) == [0x12 * 4 + 7] and s.bytes(e, c2 + 0x14, 1) == [0x12 * 4 + 8], "second column uv + 7 / + 8")
	# a colour mode outside 0..2 leaves the C's locals uninitialised: refused
	var e2 = _engine([], SDK)
	e2.call_c("srn_tpage_show", [0, 3, 0, 0x80170000, 0, 0, 0, 0, 0])
	s.check(e2.halted() and e2.refusal.contains("srn_tpage_show"), "arg1 = 3 refuses (%s)" % e2.refusal)

func _test_move_255_line() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["MoveSquare"], log)
	e.call_c("move_255_line", [3, 5, 0x11])
	s.check(log == [["MoveSquare", 3 * 0x40 + 0x3F, 0, 1, 0xFF, 0x40, 0x100], ["MoveSquare", 4 * 0x40 + 0x3F, 0, 1, 0xFF, 0x41, 0x100], ["MoveSquare", 5 * 0x40 + 0x3F, 0, 1, 0xFF, 0x42, 0x100]], "MoveSquare per page: %s" % [log])
	log.clear()
	e.call_c("move_255_line", [0x400, 0x400, 0])
	s.check(log == [["MoveSquare", 0x3F, 0, 1, 0xFF, 0, 0]], "the source x wraps at 16 bits (0x400 * 0x40 + 0x3F = 0x1003F)")
	log.clear()
	e.call_c("move_255_line", [5, 4, 0])
	s.check(log.is_empty(), "arg0 > arg1: no call")

func _test_get_fnt() -> void:
	var e = s.new_engine()
	var m = e.memory
	var atlas: int = 0x80190000
	for i: int in 0x80 * 40:
		m.write_u8(atlas + i, (i * 7 + i / 128) & 0xFF)
	var out: int = 0x801A0000
	# width 8, divisor 1, glyph 17: 16 tiles per row, tile column 1, tile row 1 -> rows 8..15, bytes 8..15 of each atlas row
	e.call_c("get_fnt", [out, atlas, 17, 1, 8, 0])
	var ok: bool = true
	for r: int in 8:
		for k: int in 8:
			ok = ok and m.u8(out + r * 8 + k) == m.u8(atlas + (8 + r) * 128 + 8 + k)
	s.check(ok, "get_fnt(width 8, divisor 1, glyph 17) copies the 8 x 8 cell row by row")
	# flag 1 flips the rows: row r of the result is row 7 - r of the cell
	for r: int in 8:
		for k: int in 8:
			m.write_u8(out + r * 8 + k, 0)
	e.call_c("get_fnt", [out, atlas, 17, 1, 8, 1])
	ok = true
	for r: int in 8:
		for k: int in 8:
			ok = ok and m.u8(out + r * 8 + k) == m.u8(atlas + (8 + 7 - r) * 128 + 8 + k)
	s.check(ok, "flag 1: rows in reverse order")
	# flag 2 mirrors the 8 bytes of each row
	e.call_c("get_fnt", [out, atlas, 17, 1, 8, 2])
	ok = true
	for r: int in 8:
		for k: int in 8:
			ok = ok and m.u8(out + r * 8 + k) == m.u8(atlas + (8 + r) * 128 + 8 + (7 - k))
	s.check(ok, "flag 2: bytes of each row reversed")
	# flags 3: rows flipped and then mirrored
	e.call_c("get_fnt", [out, atlas, 17, 1, 8, 3])
	ok = true
	for r: int in 8:
		for k: int in 8:
			ok = ok and m.u8(out + r * 8 + k) == m.u8(atlas + (8 + 7 - r) * 128 + 8 + (7 - k))
	s.check(ok, "flags 3: both")
	# width 16, divisor 2 (4 bits per pixel): 8 tiles per row, glyph 3: tile column 3 -> source offset 3 * 16 / 2 = 24, 8 bytes per row
	e.call_c("get_fnt", [out, atlas, 3, 2, 16, 0])
	ok = true
	for r: int in 16:
		for k: int in 8:
			ok = ok and m.u8(out + r * 8 + k) == m.u8(atlas + (r << 7) / 2 + 24 + k)
	s.check(ok, "width 16, divisor 2: 8 bytes per row at row << 7 / 2 + 24")
	# the frame holds the 256-byte copy at +0x8C: nothing outside [sp - 0x1A0, sp) is written (the engine checks via min_sp in the harness)
	var e2 = s.new_engine()
	e2.call_c("get_fnt", [out, atlas, 1, 1, 0x90, 0])
	s.check(e2.halted() and e2.refusal.contains("get_fnt"), "width 0x90: 0 tiles per row, a division by zero in the original, refused (%s)" % e2.refusal)

func _tileset(e, idx: int, bank_table: int, data: int, mode: int, cols: int, rows: int, size: int, h: int = 0) -> int:
	var p: int = e.memory.global_address("layer_tileset_records") + idx * 0x14
	e.memory.write_u32(p, bank_table)
	e.memory.write_u32(p + 4, data)
	e.memory.write_u8(p + 8, mode)
	e.memory.write_u8(p + 0xC, cols)
	e.memory.write_u8(p + 0xD, rows)
	e.memory.write_u8(p + 0xE, size)
	e.memory.write_u8(p + 0xF, h)
	return p

func _test_mod_trans_vram() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["get_fnt", "LoadSquare"], log)
	_words(e, 0x801A0000, [0x80500000, 0x80600000]) # bank table
	_words(e, 0x801B0000, [0, 0, 0x01100003, 0x00210001]) # frame 0 (2 words, unused), frame 1: two tiles
	_tileset(e, 0, 0x801A0000, 0x801B0000, 1, 2, 1, 8)
	var buf: int = e.sp - 0x1A0 + 0x88
	# page 0x31: (0x31 & 0xF) << 6 = 0x40; y = 0x100 + 0x80 + 3; mode byte 1 -> ratio 2; size 8; x = 20 -> xh = 10
	e.call_c("mod_trans_vram", [0, 1, 0x31, 20, 3])
	s.check(log == [["get_fnt", buf, 0x80600000, 0x10, 2, 8, 3], ["LoadSquare", 0x40 + 5 + 0, 0x183, 2, 8, buf],
		["get_fnt", buf, 0x80500000, 0x21, 2, 8, 1], ["LoadSquare", 0x40 + 5 + 2, 0x183, 2, 8, buf]], "per tile: get_fnt(buf, bank pointer, uv, ratio, size, flags) then LoadSquare(x, y, size / 2 / ratio, size, buf): %s" % [log])
	# size 16, ratio 1 (mode 0), two rows: second row at y + 16
	log.clear()
	_words(e, 0x801B0100, [0x00010000, 0x00020000])
	_tileset(e, 1, 0x801A0000, 0x801B0100, 0, 1, 2, 0x10)
	e.call_c("mod_trans_vram", [1, 0, 0, 6, 0])
	s.check(log.size() == 4 and log[1] == ["LoadSquare", 3, 0, 8, 16, buf] and log[3] == ["LoadSquare", 3, 16, 8, 16, buf], "size 16, ratio 1: x / 2 = 3, half size 8, second row 16 lower: %s" % [log])
	# a negative x: xh = -3 and the unsigned division by the ratio 2 (the -cckr u8 promotion)
	log.clear()
	_tileset(e, 0, 0x801A0000, 0x801B0000, 1, 1, 1, 8)
	e.call_c("mod_trans_vram", [0, 1, 0, -6, 0])
	s.check(log[1][1] == Math.s32((M32 & -3) / 2), "xh = -6 / 2 = -3 divided by 2 as unsigned: %X" % log[1][1])

func _test_srn_vram_set() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["func_8009C674", "mod_trans_vram", "move_255_line", "printf"], log)
	var m = e.memory
	var rec: int = m.global_address("layer_records") + 0x28
	var fields: Array = [[0xE, 2], [0xF, 2], [0x10, 4], [0x11, 4], [0x12, 0], [0x13, 0], [0x14, 4], [0x15, 4]]
	for f: Array in fields:
		m.write_u8(rec + f[0], f[1])
	m.write_u32(rec + 8, 0x80190000)
	m.write_s32(rec + 0x1C, 4)
	_words(e, 0x80190000, [0x0000ABCD, 0x00010000, 0x5555, 0x9])
	e.call_c("srn_vram_set", [1, 6, 100, 100])
	s.check(log == [["func_8009C674", 0], ["mod_trans_vram", 0, 0xABCD, 6, 0, 0], ["mod_trans_vram", 0, 0x5555, 6, 8, 0], ["mod_trans_vram", 0, 9, 6, 12, 0]],
		"cols = rows = 8 but left = 4 stops after 4 cells; word 1 has a zero low half: %s" % [log])
	log.clear()
	m.write_u8(m.global_address("D_800B3DB0"), 1)
	e.call_c("srn_vram_set", [1, 6, 2, 100])
	s.check(log == [["func_8009C674", 0], ["mod_trans_vram", 0, 0xABCD, 6, 0, 0], ["move_255_line", 6, 0x11, 0xF]], "arg2 = 2 leaves only column 0 ((col - 1) * 4 < 2), D_800B3DB0 != 0 adds move_255_line(arg1, arg1 + 0xB, 0xF): %s" % [log])
	log.clear()
	m.write_u8(rec + 0xE, 0)
	var r: int = e.call_c("srn_vram_set", [1, 6, 100, 100])
	s.check(r == -1 and log == [["func_8009C674", 0], ["printf", 0x800AFB00]], "zero cell width: printf(\"ERROR\") and -1: %s" % [log])

func _test_srn_set() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["mod_set_gpu"], log)
	var m = e.memory
	var base: int = m.global_address("layer_records")
	var rec: int = base
	for f: Array in [[0xE, 2], [0xF, 2], [0x10, 4], [0x11, 4], [0x12, 0], [0x13, 0], [0x14, 2], [0x15, 2]]:
		m.write_u8(rec + f[0], f[1])
	m.write_s16(rec + 2, 10)
	m.write_s16(rec + 6, 20)
	m.write_u16(rec + 0x16, 7)
	m.write_u16(rec + 0x18, 8)
	m.write_s32(rec + 0x1C, 3)
	m.write_u8(base + 0xC, 9)
	m.write_u32(rec + 8, 0x80190000)
	_words(e, 0x80190000, [0xFFFF0005, 0, 7])
	var r: int = e.call_c("srn_set", [0])
	s.check(r == 0 and log == [["mod_set_gpu", 0, 5, 10, 20, 7, 8, 9], ["mod_set_gpu", 0, 7, 18, 20, 7, 8, 9]], "tiles with a nonzero low half: mod_set_gpu(0, idx, x, y, wid, hgt, pri of record 0); the third tile ends the loop (left = 0): %s" % [log])
	log.clear()
	m.write_s16(rec + 6, 0x79)
	m.write_s32(rec + 0x1C, 100)
	s.check(e.call_c("srn_set", [0]) == 0 and log.is_empty(), "y >= 0x79: returns 0 before any tile")
	m.write_s16(rec + 6, -0x100)
	s.check(e.call_c("srn_set", [0]) == 0 and log.is_empty(), "every row above the screen (y + h0 * oy < -0x77): nothing drawn, falls off the end")
	m.write_u8(rec + 0xF, 0)
	s.check(e.call_c("srn_set", [0]) == -1, "zero cell height: -1")

func _test_mod_set_gpu() -> void:
	var log: Array = []
	var tpages: Array = [0x15, 0x15]
	var e = _engine(log, SDK + ["goto_tpage", "draw_brightness_get"], {"draw_brightness_get": 0x60, "func_8009ED74": 0x33})
	e.set_stub("search_tpage_multi", func(args: Array) -> int:
		log.append(["search_tpage_multi"] + args.map(func(a): return a & M32))
		return tpages.pop_front())
	var m = e.memory
	_words(e, 0x801A0000, [0x80500000, 0x80600000])
	_words(e, 0x801B0000, [0x01230200, 0x01450000]) # bank 1, uv 0x23, clut 2 / bank 1, uv 0x45, clut 0
	_tileset(e, 0, 0x801A0000, 0x801B0000, 1, 2, 1, 8)
	e.call_c("mod_set_gpu", [0, 0, 5, 7, 0x100, 9, 3])
	var p: int = 0x80160000
	# tile 0: x0 = 5 + 0xA0, y0 = (0 * 8 + 7 + 0x168) - 1 * 0xF0 = 0x7F, u0 = (0x23 & 0xF) * 8, v0 = ((0x23 & 0xF0) >> 4) * 8
	s.check([m.s16(p + 8), m.s16(p + 0xA), m.u8(p + 0xC), m.u8(p + 0xD)] == [0xA5, 0x7F, 0x18, 0x10] and s.bytes(e, p + 4, 3) == [0x60, 0x60, 0x60], "tile 0 position, uv and brightness")
	s.check([m.s16(p + 0x40 + 8), m.u8(p + 0x40 + 0xC), m.u8(p + 0x40 + 0xD)] == [0xA5 + 8, 0x28, 0x20], "tile 1 is 8 right; uv 0x45: u0 = 5 * 8, v0 = 4 * 8")
	var names: Array = log.map(func(x): return x[0])
	s.check(names.count("goto_tpage") == 1 and log[-1] == ["goto_tpage", 0x140, 0x100, 4, 0, 0], "same page twice: one final goto_tpage(((page & 0xF) << 6), (page & 0x10) << 4, pri + 1, 0, 0): %s" % [log[-1]])
	s.check(log.filter(func(x): return x[0] == "func_8009ED74")[0] == ["func_8009ED74", 0x100 + 2 * 16, 9] and log.filter(func(x): return x[0] == "AddPrim")[0] == ["AddPrim", _ot(e, 1, 4), p], "clut = GetClut(cx + clut * 16, cy); AddPrim at pri * 4 + 4")
	s.check(log.filter(func(x): return x[0] == "search_tpage_multi")[0] == ["search_tpage_multi", 0x80600000, 2], "search_tpage_multi(bank pointer, mode 2)")
	# a page change inside the frame: goto_tpage for the previous page before the new one is recorded
	log.clear()
	tpages.assign([0x15, 0x25])
	e.call_c("mod_set_gpu", [0, 0, 5, 7, 0x100, 9, 3])
	var gotos: Array = log.filter(func(x): return x[0] == "goto_tpage")
	s.check(gotos == [["goto_tpage", 0x140, 0x100, 4, 0, 0], ["goto_tpage", 0x140, 0, 4, 0, 0]], "page 0x15 then 0x25: goto_tpage(old) between the tiles, goto_tpage(new) at the end: %s" % [gotos])

func _test_mod_set() -> void:
	var log: Array = []
	var snaps: Array = []
	var tpages: Array = [0x1111, 0x2222]
	var e = _engine(log, ["draw_brightness_get"], {"draw_brightness_get": 0x44})
	var m = e.memory
	e.set_stub("search_tpage_multi", func(_args: Array) -> int:
		return tpages.pop_front())
	e.set_stub("func_80098B80", func(args: Array) -> int:
		log.append(["func_80098B80"] + args.map(func(a): return a & M32))
		snaps.append(s.bytes(e, args[0], 0x24))
		return 0)
	_words(e, 0x801A0000, [0x80500000, 0x80600000])
	# tile words: row 0: bank 0, uv 0x2F, clut 1; row 1: bank 1, uv 0x13, clut 0
	_words(e, 0x801B0000, [(0 << 24) | (0x2F << 16) | (1 << 8), (1 << 24) | (0x13 << 16)])
	_tileset(e, 0, 0x801A0000, 0x801B0000, 0, 1, 2, 16, 12)
	var sp: int = e.sp - 0xE8 + 0xB0 # mod_set frame 0xE8 (asm), quad at +0xB0
	e.call_c("mod_set", [0, 0, 4, 6, 0x80, 5, 0x12345])
	var calls: Array = log.filter(func(x): return x[0] == "func_80098B80")
	s.check(calls == [["func_80098B80", sp, 0x800E8C70 + 0x14, 0x2345], ["func_80098B80", sp, 0x800E8C70 + 0x14, 0x2345]], "func_80098B80(&sp, &draw_ot_table + buffer * 0x14, (u16) pri): %s" % [calls])
	var q: Array = snaps[0]
	# flags 0x01000000 (mode byte 0), x, y, w = 16, h = 12, uv: (0x2F & 7) * 16 = 0x70, ((0x2F & 0xF8) >> 4) * 16 = 0x20, cba = 0x80 + 1 * 16, 5
	s.check(q.slice(0, 4) == [0, 0, 0, 1] and q.slice(4, 8) == [4, 0, 6, 0] and q.slice(8, 12) == [16, 0, 12, 0], "flags 0x01000000, x = 4, y = 6, w = 16, h = 12: %s" % [q.slice(0, 12)])
	s.check(q.slice(0xC, 0x14) == [0x11, 0x11, 0x70, 0x20, 0x90, 0, 5, 0], "tpage word from search_tpage_multi, u0 v0 (16-wide cells), clut x = cx + clut * 16, clut y = cy: %s" % [q.slice(0xC, 0x14)])
	s.check(q.slice(0x14, 0x17) == [0x44, 0x44, 0x44] and q.slice(0x18, 0x1C) == [0, 0, 0, 0], "tint = brightness, anchors 0")
	s.check(q.slice(0x1C, 0x24) == [0, 0x10, 0, 0x10, 0, 0, 0, 0], "scales 0x1000 and unk_20 = 0: %s" % [q.slice(0x1C, 0x24)])
	var q2: Array = snaps[1]
	# row 1: y = 1 * 16 + 6 = 22; uv 0x13: (0x13 & 7) * 16 = 0x30, ((0x13 & 0xF8) >> 4) * 16 = 0x10; clut 0 -> 0x80
	s.check(q2.slice(4, 8) == [4, 0, 22, 0] and q2.slice(0xC, 0x14) == [0x22, 0x22, 0x30, 0x10, 0x80, 0, 5, 0], "second tile (row 1): %s" % [q2.slice(4, 0x14)])
	# modes 1 and 2 clear the flags and an 8-wide tile uses the 8x8 uv formula
	log.clear()
	snaps.clear()
	tpages.assign([1, 2])
	_tileset(e, 1, 0x801A0000, 0x801B0000, 2, 1, 1, 8, 8)
	e.call_c("mod_set", [1, 0, 0, 0, 0, 0, 0])
	var q3: Array = snaps[0]
	s.check(q3.slice(0, 4) == [0, 0, 0, 0] and q3.slice(0xE, 0x10) == [(0x2F & 0xF) * 8, ((0x2F & 0xF0) >> 4) * 8], "mode byte 2: flags 0; width 8: u0 = (uv & 0xF) * 8, v0 = ((uv & 0xF0) >> 4) * 8: %s" % [q3.slice(0, 0x10)])
