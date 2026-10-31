extends RefCounted
## Port of src/main/text_system.c (original object 0x8004E500-0x8004F6B0, T-39311 on the T-39301 engine).
## The 52-entry text list and the typewriter state (text_state), glyph upload to the text VRAM pages, the text
## palettes and the per-frame glyph sprites. The text lives only in game memory (text_list, text_state, the
## glyph upload ring); the C hands nothing to a separate text layer, so engine.text_layer is not used here: the
## glyphs reach VRAM through LoadSquare (frame_render_audio.gd) and the sprites through SPRT_8 primitives in the
## ordering table, like the original. libgpu calls (AddPrim, SetSemiTrans, SetShadeTex, func_8009F090 SetSprt8,
## func_8009ED74 GetClut, func_8009C7F8 ClearImage, func_8009C674 DrawSync) and printf are call_c and refuse by
## name until their owners are ported. Contract: wiki/engine-porting-contract.md.
## Tests: tests/engine/text_system_test.gd, config/port/engine-diff/text_system.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/text_system.c"
const C_FUNCTIONS: Array[String] = [
	"text_glyph_pages_clear", "text_system_reset", "text_set_advance", "text_unused_stub", "set_kanji_string",
	"k_disp_start", "k_disp_switch", "get_k_work", "get_ksys", "set_k_work", "k_reset", "k_sub_reset_point_set",
	"k_sub_reset", "k_sub_disp_start", "k_disp_goto_line_end", "k_speed_set", "get_k_speed", "k_disp_inc",
	"check_end_k", "check_set_k", "get_now_k", "disp_string", "load_string", "vload_font", "text_glyph_expand",
]
const C_UNPORTED: Array[String] = []
## set_k_work takes a TextEntry by value: o32 passes its 8 bytes in a1 (x | y << 16) and a2 (attribute | glyph_count
## << 8 | extra << 16 | active << 24), so the function has three argument words; "c_params" is the C count.
## load_string and vload_font are K&R functions with one parameter that their callers pass extra arguments to
## (the callees ignore them), so the port takes one.
const C_SIGNATURES: Dictionary = {
	"text_glyph_pages_clear": {"args": ["s32"], "ret": "void"},
	"text_system_reset": {"args": [], "ret": "void"},
	"text_set_advance": {"args": ["u8"], "ret": "void"},
	"text_unused_stub": {"args": [], "ret": "void"},
	"set_kanji_string": {"args": ["s16", "s16", "u8", "ptr", "s32"], "ret": "s32"},
	"k_disp_start": {"args": ["s32"], "ret": "void"},
	"k_disp_switch": {"args": ["s32", "s32"], "ret": "void"},
	"get_k_work": {"args": ["s32"], "ret": "ptr"},
	"get_ksys": {"args": [], "ret": "ptr"},
	"set_k_work": {"args": ["s32", "u32", "u32"], "ret": "void", "c_params": 2},
	"k_reset": {"args": ["s32"], "ret": "void"},
	"k_sub_reset_point_set": {"args": [], "ret": "void"},
	"k_sub_reset": {"args": [], "ret": "void"},
	"k_sub_disp_start": {"args": ["s32"], "ret": "void"},
	"k_disp_goto_line_end": {"args": [], "ret": "s16"},
	"k_speed_set": {"args": ["u8"], "ret": "void"},
	"get_k_speed": {"args": [], "ret": "u8"},
	"k_disp_inc": {"args": [], "ret": "s32"},
	"check_end_k": {"args": [], "ret": "s32"},
	"check_set_k": {"args": [], "ret": "u8"},
	"get_now_k": {"args": [], "ret": "u8"},
	"disp_string": {"args": [], "ret": "void"},
	"load_string": {"args": ["ptr"], "ret": "s32"},
	"vload_font": {"args": ["s32"], "ret": "void"},
	"text_glyph_expand": {"args": ["s32", "ptr"], "ret": "void"},
}

## text_list: TextEntry (include/main/text.h), 8 bytes: x s16, y s16, attribute, glyph_count, extra, active.
const ENTRY_SIZE: int = 8
const ENTRY_COUNT: int = 0x34
## text_state (TextState, 0xC): count s16 +0, total s16 +2, mark s16 +4, speed u8 +6, pos s16 +8, glyph_advance +0xA.
const TS_COUNT: int = 0
const TS_TOTAL: int = 2
const TS_MARK: int = 4
const TS_SPEED: int = 6
const TS_POS: int = 8
const TS_ADVANCE: int = 0xA
## The glyph upload ring: u32 words, 0x18 words (0x60 bytes) per slot, 16 slots.
const RING_SLOT_BYTES: int = 0x60
## Original stack frame offsets (asm/matchings/main/text_system/): the RECT of text_glyph_pages_clear (+0x3C), of
## text_system_reset (+0x34), and text_glyph_expand's 144-byte buf (+0x20).
const PAGES_CLEAR_RECT: int = 0x3C
const RESET_RECT: int = 0x34
const EXPAND_BUF: int = 0x20
## "<kanji.c>error(No-sjis)" (asm/data/main/text_system.rodata.s)
const STR_NO_SJIS: int = 0x800AFB50

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func _list() -> int:
	return engine.memory.global_address("text_list")

func _state() -> int:
	return engine.memory.global_address("text_state")

func _ts_s16(offset: int) -> int:
	return engine.memory.s16(_state() + offset)

func _ts_set(offset: int, value: int) -> void:
	engine.memory.write_s16(_state() + offset, value)

func _db() -> int:
	return engine.memory.read_global("draw_buffer")

## Clears the glyph pages x = 0x320 + i * 0x40, y = 0x100, 0x20 x 0x100 (ClearImage) and waits for the GPU.
func text_glyph_pages_clear(page_count: int) -> void:
	var m: Object = engine.memory
	var r: int = engine.frame_base() + PAGES_CLEAR_RECT
	var i: int = 0
	while i < page_count:
		if engine.halted():
			return
		m.write_u16(r + 0, 0x320 + i * 0x40)
		m.write_u16(r + 2, 0x100)
		m.write_u16(r + 4, 0x20)
		m.write_u16(r + 6, 0x100)
		engine.call_c("func_8009C7F8", [r, 0, 0, 0])
		i += 1
	engine.call_c("func_8009C674", [0])

## Uploads the four text palettes and the two blank strips, clears the glyph pages and the palette cell
## (0x102, 0x1F8, 2 x 8 to grey 0x80), empties the text list and resets the typewriter state.
func text_system_reset() -> void:
	var m: Object = engine.memory
	var g: Callable = func(name: String) -> int:
		return m.global_address(name)
	engine.call_c("LoadSquare", [0x100, 0x1F8, 2, 8, g.call("text_palette_0")])
	engine.call_c("LoadSquare", [0x120, 0x1F8, 2, 8, g.call("text_palette_1")])
	engine.call_c("LoadSquare", [0x110, 0x1F8, 2, 8, g.call("text_shadow_palette_0")])
	engine.call_c("LoadSquare", [0x130, 0x1F8, 2, 8, g.call("text_shadow_palette_1")])
	engine.call_c("LoadSquare", [0x320, 0x1FF, 0x20, 1, g.call("text_blank_strip")])
	engine.call_c("LoadSquare", [0x360, 0x1FF, 0x20, 1, g.call("text_blank_strip")])
	engine.call_c("text_glyph_pages_clear", [2])
	var r: int = engine.frame_base() + RESET_RECT
	m.write_u16(r + 0, 0x102)
	m.write_u16(r + 2, 0x1F8)
	m.write_u16(r + 4, 2)
	m.write_u16(r + 6, 8)
	engine.call_c("func_8009C7F8", [r, 0x80, 0x80, 0x80])
	var list: int = _list()
	for i: int in ENTRY_COUNT:
		var e: int = list + i * ENTRY_SIZE
		m.write_s16(e + 0, 0)
		m.write_s16(e + 2, 0)
		m.write_u8(e + 4, 0)
		m.write_u8(e + 5, 0)
		m.write_u8(e + 6, 0)
		m.write_u8(e + 7, 0)
	_ts_set(TS_TOTAL, 0)
	_ts_set(TS_COUNT, 0)
	_ts_set(TS_POS, 0)
	_ts_set(TS_MARK, 0)
	var advance: int = m.u8(_state() + TS_ADVANCE)
	if advance != 0xE and advance != 0x10:
		m.write_u8(_state() + TS_ADVANCE, 0xE)

func text_set_advance(glyph_advance: int) -> void:
	engine.memory.write_u8(_state() + TS_ADVANCE, 0x10 if Math.u8(glyph_advance) == 0x10 else 0xE)

func text_unused_stub() -> void:
	pass

## Queues one string in entry `count` (-1 when the 52 entries are full): a null pointer or a text whose first or
## second byte is 0 makes the entry inactive; otherwise it records y, attribute (color_index), x, the glyph count
## from load_string (which also uploads the glyphs), the ordering slot, and active = 1. Returns the new count.
func set_kanji_string(x: int, y: int, color_index: int, text: int, ordering_slot: int) -> int:
	var m: Object = engine.memory
	x = Math.s16(x) # parameters narrow on entry
	y = Math.s16(y)
	color_index = Math.u8(color_index)
	if _ts_s16(TS_COUNT) >= ENTRY_COUNT:
		return -1
	var e: int = _list() + _ts_s16(TS_COUNT) * ENTRY_SIZE
	if text == 0:
		m.write_u8(e + 7, 0)
	elif m.u8(text) == 0 or m.u8(text + 1) == 0:
		m.write_u8(e + 7, 0)
	else:
		m.write_s16(e + 2, y)
		m.write_u8(e + 4, color_index)
		m.write_s16(e + 0, x)
		m.write_u8(e + 5, engine.call_c("load_string", [text]))
		m.write_u8(e + 6, ordering_slot)
		m.write_u8(e + 7, 1)
	_ts_set(TS_COUNT, _ts_s16(TS_COUNT) + 1)
	return _ts_s16(TS_COUNT)

## pos = sum of the glyph counts of the first `first_entry` entries.
func k_disp_start(first_entry: int) -> void:
	_ts_set(TS_POS, 0)
	for i: int in maxi(first_entry, 0):
		_ts_set(TS_POS, _ts_s16(TS_POS) + engine.memory.u8(_list() + i * ENTRY_SIZE + 5))

func k_disp_switch(entry_index: int, visible: int) -> void:
	engine.memory.write_u8(Math.u32(_list() + 7 + entry_index * ENTRY_SIZE), 1 if visible > 0 else 0)

func get_k_work(entry_index: int) -> int:
	if entry_index >= 0x35:
		return engine.memory.global_address("text_list_last")
	return Math.u32(_list() + entry_index * ENTRY_SIZE)

func get_ksys() -> int:
	return _state()

## entry_value: o32 passes TextEntry {s16 x, s16 y, u8 attribute, glyph_count, extra, s8 active} in a1, a2.
func set_k_work(entry_index: int, xy: int, flags: int) -> void:
	var m: Object = engine.memory
	var p: int = Math.u32(_list() + entry_index * ENTRY_SIZE)
	m.write_s16(p + 0, xy)
	m.write_s16(p + 2, xy >> 16)
	m.write_u8(p + 4, flags)
	m.write_u8(p + 5, flags >> 8)
	m.write_u8(p + 6, flags >> 16)
	m.write_u8(p + 7, flags >> 24)

## Keeps the first `entry_count` entries: count = entry_count, total = pos = their glyph counts' sum (an s16 sum).
func k_reset(entry_count: int) -> void:
	var glyph_total: int = 0
	for i: int in maxi(entry_count, 0):
		glyph_total = Math.s16(glyph_total + engine.memory.u8(_list() + i * ENTRY_SIZE + 5))
	_ts_set(TS_COUNT, entry_count)
	_ts_set(TS_TOTAL, glyph_total)
	_ts_set(TS_POS, glyph_total)

func k_sub_reset_point_set() -> void:
	_ts_set(TS_MARK, _ts_s16(TS_COUNT))

func k_sub_reset() -> void:
	engine.call_c("k_reset", [_ts_s16(TS_MARK)])

func k_sub_disp_start(entry_offset: int) -> void:
	engine.call_c("k_disp_start", [Math.s32(_ts_s16(TS_MARK) + entry_offset)])

## Moves pos to the end of the entry that contains it and returns that end; -1 when pos is past every entry.
func k_disp_goto_line_end() -> int:
	var glyph_end: int = 0
	for i: int in ENTRY_COUNT:
		glyph_end += engine.memory.u8(_list() + i * ENTRY_SIZE + 5)
		if glyph_end >= _ts_s16(TS_POS):
			_ts_set(TS_POS, glyph_end)
			return glyph_end
	return -1

func k_speed_set(reveal_speed: int) -> void:
	reveal_speed = Math.u8(reveal_speed)
	engine.memory.write_u8(_state() + TS_SPEED, reveal_speed)
	if reveal_speed >= 0x81:
		engine.memory.write_u8(_state() + TS_SPEED, 0x7F)

func get_k_speed() -> int:
	return engine.memory.u8(_state() + TS_SPEED)

## One more glyph every speed + 1 frames (frame_count is unsigned): 1 when everything is shown.
func k_disp_inc() -> int:
	var speed: int = engine.memory.u8(_state() + TS_SPEED)
	if engine.gs_get("frame_count") % (speed + 1) == 0:
		if _ts_s16(TS_POS) < _ts_s16(TS_TOTAL):
			_ts_set(TS_POS, _ts_s16(TS_POS) + 1)
			return 0
		return 1
	if _ts_s16(TS_POS) == _ts_s16(TS_TOTAL):
		return 1
	return 0

func check_end_k() -> int:
	return 1 if _ts_s16(TS_POS) == _ts_s16(TS_TOTAL) else 0

func check_set_k() -> int:
	return engine.memory.u8(_state())

## Index of the entry holding glyph pos, or 0xFF.
func get_now_k() -> int:
	var glyph_end: int = 0
	var i: int = 0
	while i < _ts_s16(TS_POS):
		glyph_end += engine.memory.u8(_list() + i * ENTRY_SIZE + 5)
		if glyph_end >= _ts_s16(TS_POS):
			return i
		i += 1
	return 0xFF

## Draws the revealed glyphs (i < min(total, pos)): per glyph a white SPRT_8 at the entry's position plus k *
## glyph_advance and a black one offset by (1, 1), both into the entry's ordering slot (0 -> 1), followed by a
## goto_tpage for the glyph's page (0x300 for glyphs < 0x80, 0x340 for < 0x100, else 0x380) whenever the slot
## changes or the page boundary (glyph 0x7F, 0xFF) is reached, and a final goto_tpage for the last page.
func disp_string() -> void:
	var m: Object = engine.memory
	var list: int = _list()
	var j: int = 0
	var ot: int = m.u8(list + 6)
	if ot == 0:
		ot = 1
	var flag: int = 1
	var i: int = 0
	var k: int = 0
	while i < _ts_s16(TS_TOTAL) and i < _ts_s16(TS_POS):
		if engine.halted():
			return
		if k >= m.u8(list + j * ENTRY_SIZE + 5):
			k = 0
			j += 1
		var e: int = list + j * ENTRY_SIZE
		if m.s8(e + 7) == 1:
			if ot != m.u8(e + 6):
				ot = m.u8(e + 6)
				flag = 1
				if ot == 0:
					ot = 1
			var attribute: int = m.u8(e + 4)
			var advance: int = m.u8(_state() + TS_ADVANCE)
			var p: int = Math.u32(engine.call_c("GetWorkBase", [0x10, _db()]))
			engine.call_c("func_8009F090", [p])
			engine.call_c("SetSemiTrans", [p, 0])
			engine.call_c("SetShadeTex", [p, 0])
			var bright: int = engine.gs_get("screen_brightness")
			m.write_u8(p + 4, bright)
			m.write_u8(p + 5, bright)
			m.write_u8(p + 6, bright)
			var ox: int = m.s16(engine.gs_address("draw_origin_x") + _db() * 2)
			var oy: int = m.s16(engine.gs_address("draw_origin_y") + _db() * 2)
			m.write_s16(p + 8, m.s16(e) + ox + k * advance)
			m.write_s16(p + 0xA, m.s16(e + 2) + oy)
			m.write_u8(p + 0xC, (i & 7) * 0x10 + 0x80)
			m.write_u8(p + 0xD, (i & 0x78) * 2)
			m.write_u16(p + 0xE, engine.call_c("func_8009ED74", [(attribute & 8) * 4 + 0x100, (attribute & 7) + 0x1F8]))
			engine.call_c("AddPrim", [_ot(ot), p])
			p = Math.u32(engine.call_c("GetWorkBase", [0x10, _db()]))
			engine.call_c("func_8009F090", [p])
			engine.call_c("SetSemiTrans", [p, 0])
			engine.call_c("SetShadeTex", [p, 0])
			m.write_u8(p + 4, 0)
			m.write_u8(p + 5, 0)
			m.write_u8(p + 6, 0)
			ox = m.s16(engine.gs_address("draw_origin_x") + _db() * 2)
			oy = m.s16(engine.gs_address("draw_origin_y") + _db() * 2)
			m.write_s16(p + 8, m.s16(e) + ox + k * m.u8(_state() + TS_ADVANCE) + 1)
			m.write_s16(p + 0xA, m.s16(e + 2) + oy + 1)
			m.write_u8(p + 0xC, (i & 7) * 0x10 + 0x80)
			m.write_u8(p + 0xD, (i & 0x78) * 2)
			m.write_u16(p + 0xE, engine.call_c("func_8009ED74", [(attribute & 8) * 4 + 0x110, (attribute & 7) + 0x1F8]))
			engine.call_c("AddPrim", [_ot(ot), p])
			if i == 0x7F:
				engine.call_c("goto_tpage", [0x300, 0x100, ot, 0, 0])
				flag = 0
			elif i == 0xFF:
				engine.call_c("goto_tpage", [0x340, 0x100, ot, 0, 0])
				flag = 0
			elif flag != 0:
				flag = 0
				if i >= 0x100:
					engine.call_c("goto_tpage", [0x380, 0x100, ot, 0, 0])
				elif i >= 0x80:
					engine.call_c("goto_tpage", [0x340, 0x100, ot, 0, 0])
				else:
					engine.call_c("goto_tpage", [0x300, 0x100, ot, 0, 0])
		i += 1
		k += 1
	if i >= 0x100:
		engine.call_c("goto_tpage", [0x380, 0x100, ot, 0, 0])
	elif i >= 0x80:
		engine.call_c("goto_tpage", [0x340, 0x100, ot, 0, 0])
	else:
		engine.call_c("goto_tpage", [0x300, 0x100, ot, 0, 0])

## &draw_ot_tags[(draw_buffer << 10) + slot * 4]
func _ot(slot: int) -> int:
	return Math.s32(engine.memory.global_address("draw_ot_tags") + Math.sll32(_db(), 10) + slot * 4)

## Decodes the string's two-byte (Shift-JIS) codes: a code below the first range prints the error and ends;
## a code in range i loads glyph (c - first[i-1]) + glyph[i-1] through vload_font; a code past the last range
## is skipped. Ends at a zero byte; returns the number of glyphs loaded (total - first_glyph).
func load_string(text: int) -> int:
	var m: Object = engine.memory
	var table: int = m.global_address("text_font_range_table")
	var first_glyph: int = _ts_s16(TS_TOTAL)
	while true:
		if engine.halted():
			return 0
		var hi: int = m.u8(text)
		var lo_at: int = text + 1
		text = Math.u32(text + 2)
		if hi == 0 or m.u8(lo_at) == 0:
			return Math.s32(_ts_s16(TS_TOTAL) - first_glyph)
		var c: int = m.u8(lo_at) + (hi << 8)
		if c < m.u16(table):
			engine.call_c("printf", [STR_NO_SJIS])
			return Math.s32(_ts_s16(TS_TOTAL) - first_glyph)
		for i: int in range(1, 46):
			if c < m.u16(table + i * 4):
				var base: int = m.u16(table + (i - 1) * 4)
				engine.call_c("vload_font", [(c - base) + m.u16(table + (i - 1) * 4 + 2)])
				break
	return 0

## Expands glyph arg0 into the upload ring slot (total & 0xF), uploads it to the text page of its number
## (glyphs < 0x80: x 0x320; < 0x100: x 0x360; else 0x3A0; 4 x 12 words at ((total & 7) * 4, (total & 0x78) * 2
## + 0x100)), waits for the GPU and counts the glyph.
func vload_font(arg0: int) -> void:
	var ring: int = engine.memory.global_address("text_glyph_upload_ring") + (_ts_s16(TS_TOTAL) & 0xF) * RING_SLOT_BYTES
	engine.call_c("text_glyph_expand", [arg0, ring])
	var total: int = _ts_s16(TS_TOTAL)
	var base_x: int = 0x320 if total < 0x80 else (0x360 if total < 0x100 else 0x3A0)
	engine.call_c("LoadSquare", [(total & 7) * 4 + base_x, (total & 0x78) * 2 + 0x100, 4, 0xC, ring])
	engine.call_c("func_8009C674", [0])
	_ts_set(TS_TOTAL, _ts_s16(TS_TOTAL) + 1)

## 1-bit-per-pixel 12 x 12 glyph at text_font_glyphs + arg0 * 18 (18 bytes) to 4-bit pixels: bit-by-bit into the
## 144-byte buf on the stack (+0x20), then six groups of four words (8, 4, 8 and 4 pixels packed low nibble
## first) into arg1.
func text_glyph_expand(arg0: int, arg1: int) -> void:
	var m: Object = engine.memory
	var buf: int = engine.frame_base() + EXPAND_BUF
	var p: int = Math.u32(m.global_address("text_font_glyphs") + Math.s32(arg0 * 18))
	for i: int in 18:
		var b: int = m.u8(p + i)
		for bit: int in 8:
			m.write_u8(buf + i * 8 + bit, (b >> (7 - bit)) & 1)
	var q: int = buf
	var out: int = arg1
	for _group: int in 6:
		for pixels: int in [8, 4, 8, 4]:
			var word: int = 0
			for n: int in pixels:
				word = Math.s32(word + Math.s32(m.u8(q) << (n * 4)))
				q += 1
			m.write_u32(out, word)
			out = Math.u32(out + 4)
