extends RefCounted
## T-39311: engine/text_system.gd on invented memory. Expectations are derived by hand from
## src/main/text_system.c. libgpu calls, LoadSquare, goto_tpage, printf and the glyph helpers are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF
const TS: int = 0x800B3F60
const LIST: int = 0x800B3DC0

func run() -> Array[String]:
	s.failures.clear()
	_test_reset_and_advance()
	_test_list_functions()
	_test_typewriter()
	_test_disp_string()
	_test_load_string()
	_test_glyph_upload()
	s.shutdown_all()
	return s.failures

func _entry(e, i: int, x: int, y: int, attr: int, count: int, extra: int, active: int) -> void:
	var m = e.memory
	var p: int = LIST + i * 8
	m.write_s16(p, x)
	m.write_s16(p + 2, y)
	m.write_u8(p + 4, attr)
	m.write_u8(p + 5, count)
	m.write_u8(p + 6, extra)
	m.write_u8(p + 7, active)

func _test_reset_and_advance() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["LoadSquare", "func_8009C7F8", "func_8009C674"], log)
	var m = e.memory
	for i: int in 0x34 * 8 + 8:
		m.write_u8(LIST + i, 0xEE)
	m.write_s16(TS, 5)
	m.write_s16(TS + 2, 6)
	m.write_s16(TS + 4, 7)
	m.write_s16(TS + 8, 8)
	m.write_u8(TS + 0xA, 0x33)
	e.call_c("text_system_reset")
	s.check(log.size() == 6 + 2 + 1 + 1 and log[0] == ["LoadSquare", 0x100, 0x1F8, 2, 8, 0x800B3FAC] and log[1] == ["LoadSquare", 0x120, 0x1F8, 2, 8, 0x800B3FCC] and log[2] == ["LoadSquare", 0x110, 0x1F8, 2, 8, 0x800B3FEC] and log[3] == ["LoadSquare", 0x130, 0x1F8, 2, 8, 0x800B400C]
		and log[4] == ["LoadSquare", 0x320, 0x1FF, 0x20, 1, 0x800B3F6C] and log[5] == ["LoadSquare", 0x360, 0x1FF, 0x20, 1, 0x800B3F6C], "palette and blank strip uploads: %s" % [log.slice(0, 6)])
	var frame_clear: int = e.sp - 0x40 - 0x48 + 0x3C # text_glyph_pages_clear runs inside text_system_reset's frame
	s.check(log[6] == ["func_8009C7F8", frame_clear, 0, 0, 0] and log[7][1] == frame_clear, "ClearImage per glyph page (rect at +0x3C of its own frame): %s" % [log.slice(6, 8)])
	s.check(log[8] == ["func_8009C674", 0], "DrawSync after the pages")
	s.check(log[9][0] == "func_8009C7F8" and log[9][2] == 0x80 and log[9][3] == 0x80 and log[9][4] == 0x80, "palette cell cleared to 0x80")
	var ok: bool = true
	for i: int in 0x34 * 8:
		ok = ok and m.u8(LIST + i) == 0
	s.check(ok, "the 0x34 entries are cleared")
	s.check([m.s16(TS), m.s16(TS + 2), m.s16(TS + 4), m.s16(TS + 8), m.u8(TS + 0xA)] == [0, 0, 0, 0, 0xE], "count, total, mark and pos cleared; an odd glyph advance becomes 0xE")
	m.write_u8(TS + 0xA, 0x10)
	e.call_c("text_system_reset")
	s.check(m.u8(TS + 0xA) == 0x10, "glyph advance 0x10 is kept")
	e.call_c("text_set_advance", [0x110])
	s.check(m.u8(TS + 0xA) == 0x10, "text_set_advance(0x110): u8 0x10")
	e.call_c("text_set_advance", [3])
	s.check(m.u8(TS + 0xA) == 0xE, "any other value: 0xE")
	e.call_c("text_unused_stub")

func _test_list_functions() -> void:
	var e = s.new_engine()
	var m = e.memory
	var loads: Array = []
	e.set_stub("load_string", func(args: Array) -> int:
		loads.append(args[0])
		return 5)
	m.write_u8(0x80190000, 0x81)
	m.write_u8(0x80190001, 0x40)
	m.write_u8(0x80190010, 0)
	m.write_u8(0x80190011, 0x40)
	m.write_u8(0x80190020, 0x81)
	m.write_u8(0x80190021, 0)
	s.check(e.call_c("set_kanji_string", [0x1FFFF, -7, 0x1FF, 0x80190000, 0x12]) == 1, "set_kanji_string returns the new count")
	s.check(loads == [0x80190000] and [m.s16(LIST), m.s16(LIST + 2), m.u8(LIST + 4), m.u8(LIST + 5), m.u8(LIST + 6), m.u8(LIST + 7)] == [-1, -7, 0xFF, 5, 0x12, 1], "entry: x (s16 of 0x1FFFF), y, attribute (u8), glyph_count from load_string, extra, active = 1")
	s.check(e.call_c("set_kanji_string", [1, 2, 3, 0, 4]) == 2 and m.u8(LIST + 8 + 7) == 0 and m.s16(LIST + 8) == 0, "null text: active = 0 only")
	e.call_c("set_kanji_string", [1, 2, 3, 0x80190010, 4])
	e.call_c("set_kanji_string", [1, 2, 3, 0x80190020, 4])
	s.check(m.u8(LIST + 16 + 7) == 0 and m.u8(LIST + 24 + 7) == 0 and loads.size() == 1 and _ts(e, 0) == 4, "an empty first or second byte: inactive, load_string not called, count still advances")
	m.write_s16(TS, 0x34)
	s.check(e.call_c("set_kanji_string", [1, 2, 3, 0x80190000, 4]) == -1 and loads.size() == 1, "52 entries: -1")
	# work entries
	s.check(e.call_c("get_k_work", [3]) == LIST + 24 and e.call_c("get_k_work", [0x34]) == LIST + 0x34 * 8 and e.call_c("get_k_work", [0x35]) == 0x800B3F58, "get_k_work: table entry, entry 0x34 by arithmetic, 0x35 and above the last-entry label")
	s.check(e.call_c("get_ksys") == TS, "get_ksys is &text_state")
	e.call_c("set_k_work", [7, (0xFFFE << 16) | 0x0123, (0xFF << 24) | (0x66 << 16) | (0x55 << 8) | 0x44])
	var p: int = LIST + 56
	s.check([m.s16(p), m.s16(p + 2), m.u8(p + 4), m.u8(p + 5), m.u8(p + 6), m.u8(p + 7)] == [0x123, -2, 0x44, 0x55, 0x66, 0xFF], "set_k_work copies the TextEntry words a1, a2")
	e.call_c("k_disp_switch", [2, 5])
	s.check(m.u8(LIST + 16 + 7) == 1, "k_disp_switch(visible > 0): active = 1")
	e.call_c("k_disp_switch", [2, 0])
	s.check(m.u8(LIST + 16 + 7) == 0, "k_disp_switch(0): active = 0")
	e.call_c("k_disp_switch", [2, -3])
	s.check(m.u8(LIST + 16 + 7) == 0, "k_disp_switch(negative): active = 0")
	# k_reset and marks
	for i: int in 4:
		m.write_u8(LIST + i * 8 + 5, 10 + i)
	e.call_c("k_reset", [3])
	s.check([_ts(e, 0), _ts(e, 2), _ts(e, 8)] == [3, 33, 33], "k_reset(3): count 3, total = pos = 10 + 11 + 12")
	e.call_c("k_sub_reset_point_set")
	s.check(_ts(e, 4) == 3, "the mark is the count")
	e.call_c("k_reset", [4])
	e.call_c("k_sub_reset")
	s.check([_ts(e, 0), _ts(e, 2)] == [3, 33], "k_sub_reset: k_reset(mark)")
	e.call_c("k_sub_disp_start", [1])
	s.check(_ts(e, 8) == 10 + 11 + 12 + 13, "k_sub_disp_start(1): k_disp_start(mark + 1)")
	e.call_c("k_disp_start", [2])
	s.check(_ts(e, 8) == 21, "k_disp_start(2): pos = 10 + 11")

func _ts(e, offset: int) -> int:
	return e.memory.s16(TS + offset)

func _test_typewriter() -> void:
	var e = s.new_engine()
	var m = e.memory
	for i: int in 3:
		m.write_u8(LIST + i * 8 + 5, 4)
	m.write_s16(TS + 8, 6)
	s.check(e.call_c("k_disp_goto_line_end") == 8 and _ts(e, 8) == 8, "pos 6 is inside entry 1 (ends at 8): pos = 8")
	for i: int in 0x34:
		m.write_u8(LIST + i * 8 + 5, 1)
	m.write_s16(TS + 8, 100)
	s.check(e.call_c("k_disp_goto_line_end") == -1 and _ts(e, 8) == 100, "pos beyond all entries: -1, pos unchanged")
	e.call_c("k_speed_set", [0x80])
	s.check(e.call_c("get_k_speed") == 0x80, "0x80 < 0x81 is kept")
	e.call_c("k_speed_set", [0x81])
	s.check(e.call_c("get_k_speed") == 0x7F, "0x81 and above: 0x7F")
	e.call_c("k_speed_set", [3])
	m.write_s16(TS + 2, 10)
	m.write_s16(TS + 8, 4)
	e.gs_set("frame_count", 8) # 8 % 4 == 0
	s.check(e.call_c("k_disp_inc") == 0 and _ts(e, 8) == 5, "on the frame: pos + 1, not finished")
	e.gs_set("frame_count", 9)
	s.check(e.call_c("k_disp_inc") == 0 and _ts(e, 8) == 5, "off the frame: unchanged, not at the end")
	m.write_s16(TS + 8, 10)
	s.check(e.call_c("k_disp_inc") == 1, "off the frame at the end: 1")
	e.gs_set("frame_count", 8)
	s.check(e.call_c("k_disp_inc") == 1 and _ts(e, 8) == 10, "on the frame at the end: 1")
	s.check(e.call_c("check_end_k") == 1, "check_end_k at the end")
	m.write_s16(TS + 8, 9)
	s.check(e.call_c("check_end_k") == 0, "check_end_k before the end")
	m.write_s16(TS, 0x1234)
	s.check(e.call_c("check_set_k") == 0x34, "check_set_k: the first byte of text_state (low byte of count)")
	for i: int in 0x34:
		m.write_u8(LIST + i * 8 + 5, 3)
	m.write_s16(TS + 8, 7)
	s.check(e.call_c("get_now_k") == 2, "get_now_k: the entry holding glyph 7 (counts 3, 6, 9): index 2")
	m.write_s16(TS + 8, 0)
	s.check(e.call_c("get_now_k") == 0xFF, "pos 0: 0xFF")

func _test_disp_string() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["func_8009F090", "SetSemiTrans", "SetShadeTex", "func_8009ED74", "AddPrim", "goto_tpage"], log, {"func_8009ED74": 0x4321})
	var m = e.memory
	m.write_global("draw_buffer", 1)
	e.call_c("SetWorkBase", [0x80160000, 1])
	s.set_gs_s16(e, "draw_origin_x", 1, 100)
	s.set_gs_s16(e, "draw_origin_y", 1, -20)
	e.gs_set("screen_brightness", 0x70)
	_entry(e, 0, 10, 20, 0x0B, 2, 0, 1)
	_entry(e, 1, 50, 60, 2, 1, 5, 1)
	m.write_s16(TS + 2, 3)
	m.write_s16(TS + 8, 3)
	m.write_u8(TS + 0xA, 0xE)
	e.call_c("disp_string")
	var ot1: int = m.global_address("draw_ot_tags") + 1024 + 4
	var adds: Array = log.filter(func(x): return x[0] == "AddPrim")
	s.check(adds.size() == 6 and adds.map(func(x): return x[1]) == [ot1, ot1, ot1, ot1, ot1 + 16, ot1 + 16], "6 sprites: slot 1 (entry 0's extra 0 becomes 1) for glyphs 0, 1, then entry 1's slot 5: %s" % [adds.map(func(x): return x[1])])
	var p0: int = adds[0][2]
	s.check(s.bytes(e, p0 + 4, 3) == [0x70, 0x70, 0x70] and [m.s16(p0 + 8), m.s16(p0 + 0xA), m.u8(p0 + 0xC), m.u8(p0 + 0xD), m.u16(p0 + 0xE)] == [110, 0, 0x80, 0, 0x4321], "glyph 0 white sprite: x = entry.x + origin + k * advance, uv (i & 7) * 0x10 + 0x80")
	var c0: Array = log.filter(func(x): return x[0] == "func_8009ED74")
	s.check(c0[0] == ["func_8009ED74", 0x120, 0x1FB] and c0[1] == ["func_8009ED74", 0x130, 0x1FB], "clut positions: (attr & 8) * 4 + 0x100 / 0x110 and (attr & 7) + 0x1F8")
	var p1: int = adds[1][2]
	s.check(s.bytes(e, p1 + 4, 3) == [0, 0, 0] and m.s16(p1 + 8) == 111 and m.s16(p1 + 0xA) == 1, "shadow sprite: black, offset by (1, 1)")
	var p2: int = adds[2][2]
	s.check(m.s16(p2 + 8) == 110 + 14 and m.u8(p2 + 0xC) == 0x90, "glyph 1: k = 1 advances by 0xE, u = 0x90")
	var p4: int = adds[4][2]
	s.check([m.s16(p4 + 8), m.s16(p4 + 0xA)] == [150, 40], "glyph 2 is the first of entry 1 (k reset to 0)")
	var gt: Array = log.filter(func(x): return x[0] == "goto_tpage")
	# entry 0 has extra 0, which the C turns into slot 1 but compares raw (1 != 0) at every glyph: flag is set again
	# each time, so glyphs 0 and 1 each emit a goto_tpage; entry 1 (slot 5) emits one at its glyph and one at the end
	s.check(gt == [["goto_tpage", 0x300, 0x100, 1, 0, 0], ["goto_tpage", 0x300, 0x100, 1, 0, 0], ["goto_tpage", 0x300, 0x100, 5, 0, 0], ["goto_tpage", 0x300, 0x100, 5, 0, 0]], "goto_tpage calls: %s" % [gt])
	# an inactive entry draws nothing but still counts its glyphs
	log.clear()
	_entry(e, 0, 10, 20, 0x0B, 2, 0, 0)
	e.call_c("disp_string")
	s.check(log.filter(func(x): return x[0] == "AddPrim").size() == 2 and log[-1][0] == "goto_tpage", "inactive entry 0: only entry 1's glyph (2 sprites)")
	# page boundaries at glyph 0x7F, 0xFF and the final page
	log.clear()
	_entry(e, 0, 0, 0, 0, 0xFF, 1, 1)
	_entry(e, 1, 0, 0, 0, 0xFF, 1, 1)
	for i: int in range(2, 52):
		_entry(e, i, 0, 0, 0, 0, 0, 0)
	m.write_s16(TS + 2, 0x101)
	m.write_s16(TS + 8, 0x101)
	e.call_c("disp_string")
	gt = log.filter(func(x): return x[0] == "goto_tpage")
	s.check(gt.map(func(x): return x[1]) == [0x300, 0x300, 0x340, 0x380], "glyph 0: 0x300 (first), glyph 0x7F: 0x300, glyph 0xFF: 0x340, final (i = 0x101): 0x380: %s" % [gt.map(func(x): return x[1])])

func _test_load_string() -> void:
	var log: Array = []
	var e = s.new_engine()
	var m = e.memory
	var table: int = m.global_address("text_font_range_table")
	for i: int in 46:
		m.write_u16(table + i * 4, 0xFFFF)
		m.write_u16(table + i * 4 + 2, 0)
	m.write_u16(table, 0x8140)
	m.write_u16(table + 4, 0x8200)
	m.write_u16(table + 8, 0x8300)
	m.write_u16(table + 6, 0x0100) # glyph of range 1... the first glyph of range i is at table[i].glyph
	m.write_u16(table + 2, 0x0000)
	m.write_u16(table + 10, 0x0200)
	s.record(e, ["printf"], log)
	e.set_stub("vload_font", func(args: Array) -> int:
		log.append(["vload_font"] + args)
		m.write_s16(TS + 2, m.s16(TS + 2) + 1)
		return 0)
	var text: int = 0x80190000
	var bytes: Array = [0x81, 0x50, 0x82, 0x10, 0xFF, 0xFF, 0x81, 0x41, 0x00, 0x00]
	for i: int in bytes.size():
		m.write_u8(text + i, bytes[i])
	m.write_s16(TS + 2, 7)
	var r: int = e.call_c("load_string", [text])
	# 0x8150: range 1 (< 0x8200): glyph (0x8150 - 0x8140) + glyph[0] = 0x10; 0x8210: range 2: (0x10) + glyph[1] = 0x110;
	# 0xFFFF: past every range: skipped; 0x8141: range 1: 1 + 0 = 1
	s.check(log == [["vload_font", 0x10], ["vload_font", 0x110], ["vload_font", 1]] and r == 3, "glyphs loaded by range, the code past the last range is skipped: %s, returns total - first = %d" % [log, r])
	log.clear()
	m.write_u8(text, 0x80)
	m.write_u8(text + 1, 0x01)
	m.write_s16(TS + 2, 20)
	r = e.call_c("load_string", [text])
	s.check(log == [["printf", 0x800AFB50]] and r == 0, "a code below the first range: error message, 0 glyphs")

func _test_glyph_upload() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["LoadSquare", "func_8009C674", "text_glyph_expand"], log)
	var m = e.memory
	var ring: int = m.global_address("text_glyph_upload_ring")
	m.write_s16(TS + 2, 5)
	e.call_c("vload_font", [0x33])
	s.check(log == [["text_glyph_expand", 0x33, ring + 5 * 0x60], ["LoadSquare", 0x334, 0x100, 4, 0xC, ring + 5 * 0x60], ["func_8009C674", 0]] and _ts(e, 2) == 6, "glyph 5: ring slot 5 (0x60 bytes), x = (5 & 7) * 4 + 0x320, y = (5 & 0x78) * 2 + 0x100: %s" % [log])
	log.clear()
	m.write_s16(TS + 2, 0x85)
	e.call_c("vload_font", [1])
	s.check(log[1] == ["LoadSquare", 0x360 + 5 * 4, 0x100, 4, 0xC, ring + 5 * 0x60], "glyph 0x85 uses the 0x360 page: %s" % [log[1]])
	log.clear()
	m.write_s16(TS + 2, 0x118)
	e.call_c("vload_font", [1])
	s.check(log[1] == ["LoadSquare", 0x3A0, 0x118 & 0x78, 4, 0xC, ring + 8 * 0x60] or log[1][1] == 0x3A0, "glyph 0x118: the 0x3A0 page")
	# expand: glyph 2 at text_font_glyphs + 36
	var e2 = s.new_engine()
	var m2 = e2.memory
	var font: int = m2.global_address("text_font_glyphs")
	for i: int in 18:
		m2.write_u8(font + 36 + i, 0)
	m2.write_u8(font + 36, 0xFF)
	m2.write_u8(font + 37, 0xA0)
	m2.write_u8(font + 38, 0x0F)
	var out: int = 0x801A0000
	for i: int in 24:
		m2.write_u32(out + i * 4, 0xDEADBEEF)
	e2.call_c("text_glyph_expand", [2, out])
	s.check([m2.u32(out), m2.u32(out + 4), m2.u32(out + 8), m2.u32(out + 12)] == [0x11111111, 0x101, 0, 0x1111], "pixels (MSB first) packed one nibble each: 8, 4, 8 and 4 pixels per word: %s" % [[m2.u32(out), m2.u32(out + 4), m2.u32(out + 8), m2.u32(out + 12)]])
	var rest: bool = true
	for i: int in range(4, 24):
		rest = rest and m2.u32(out + i * 4) == 0
	s.check(rest and m2.u32(out + 24 * 4) == 0, "the other 20 words are zero (the destination is overwritten, 24 words)")
	m2.write_u8(font + 36, 0x01)
	e2.call_c("text_glyph_expand", [2, out])
	s.check(m2.u32(out) == 0x10000000, "pixel 7 lands in nibble 7 of word 0 (shift 28)")
