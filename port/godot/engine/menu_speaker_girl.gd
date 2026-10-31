extends RefCounted
## Port of src/main/menu_speaker_girl.c (original object 0x8004F870-0x80053650, T-39313 / T-39301 contract).
## Menu registration, hit tests and cursor movement (menu 0 and 1 share the interleaved MenuItem table at
## GameState +0xF90: item (i, menu) lives at menu_items + (i * 2 + menu) * 8), speaker labels and narration
## text, girl name / state / score queries and the talk-text and voice steps.
## Return values the C leaves undefined (functions with ret_undefined or ret_defined) are 0, an unobserved placeholder
## (every caller discards them), except where the asm makes the leftover v0 deterministic (menu_check_2,
## girl_points_others; both verified against the captures).
## Contract: wiki/engine-porting-contract.md. Tests: tests/engine/menu_speaker_girl_test.gd and
## config/port/engine-diff/menu_speaker_girl.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/menu_speaker_girl.c"
const C_FUNCTIONS: Array[String] = [
	"menu_set", "menu_check", "menu_check_1", "menu_check_2", "menu_cursor_update", "menu_cursor_to_edge",
	"get_near_menu", "menu_bar_color", "menu_bar_show", "x_taku_menu_set", "x_taku_string_set", "gnsx",
	"sndisp", "sndi", "set_c_girl", "get_g_name", "get_p_name", "get_g_zyotai_s", "get_g_zyotai_h",
	"menu_girl_taku_set", "xa_wait", "text_wait_confirm", "talk_voice_play", "talk_text_step",
	"get_h_tokimeki", "get_h_yuukou", "get_h_tokimeki_table", "get_h_yuukou_table", "birth_day_check",
	"birth_day_check_days", "girl_points_shiori", "girl_points_others", "girl_scores",
	"girl_player_sports_club_check", "girl_find_by_club",
]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"menu_set": {"args": ["s32", "u8", "ptr", "ptr", "ptr", "ptr"], "ret": "s32", "ret_defined": [-1]},
	"menu_check": {"args": ["s32", "s16", "s16"], "ret": "s32"},
	"menu_check_1": {"args": ["s32", "s16", "s16"], "ret": "s32"},
	"menu_check_2": {"args": ["s32", "s32", "s32"], "ret": "s32"},
	"menu_cursor_update": {"args": ["s32"], "ret": "void"},
	"menu_cursor_to_edge": {"args": ["s32", "s32"], "ret": "s32"},
	"get_near_menu": {"args": ["s32", "s32", "s32"], "ret": "s32"},
	"menu_bar_color": {"args": ["s32", "s32", "s32", "s32"], "ret": "void"},
	"menu_bar_show": {"args": ["s32"], "ret": "void"},
	"x_taku_menu_set": {"args": ["s32", "s32"], "ret": "void"},
	"x_taku_string_set": {"args": ["s32", "s32", "s32", "s32"], "ret": "void"},
	"gnsx": {"args": ["ptr"], "ret": "void"},
	"sndisp": {"args": ["ptr", "s32", "s32"], "ret": "void"},
	"sndi": {"args": ["ptr", "s32", "s32"], "ret": "void"},
	"set_c_girl": {"args": ["u8"], "ret": "void"},
	"get_g_name": {"args": ["ptr", "s32"], "ret": "s32"},
	"get_p_name": {"args": ["ptr", "u32"], "ret": "void"},
	"get_g_zyotai_s": {"args": ["s32"], "ret": "s32"},
	"get_g_zyotai_h": {"args": ["s32"], "ret": "s32"},
	"menu_girl_taku_set": {"args": ["s32", "ptr"], "ret": "void"},
	"xa_wait": {"args": [], "ret": "void"},
	"text_wait_confirm": {"args": [], "ret": "void"},
	"talk_voice_play": {"args": ["s32", "s32", "s32", "s32"], "ret": "void"},
	"talk_text_step": {"args": [], "ret": "void"},
	"get_h_tokimeki": {"args": ["s32"], "ret": "u32"},
	"get_h_yuukou": {"args": ["s32"], "ret": "u32"},
	"get_h_tokimeki_table": {"args": ["ptr"], "ret": "void"},
	"get_h_yuukou_table": {"args": ["ptr"], "ret": "void"},
	"birth_day_check": {"args": ["s32"], "ret": "void"},
	"birth_day_check_days": {"args": ["s32", "s32", "s32"], "ret": "s32"},
	"girl_points_shiori": {"args": ["ptr", "ptr"], "ret": "s32", "ret_defined": [0]},
	"girl_points_others": {"args": ["ptr", "ptr"], "ret": "s32"},
	"girl_scores": {"args": ["ptr", "ptr"], "ret": "s32"},
	"girl_player_sports_club_check": {"args": [], "ret": "s32"},
	"girl_find_by_club": {"args": ["s32"], "ret": "s32"},
}

const GAME_STATE: int = 0x800E6280
const MENU_ITEMS: int = 0x800E6280 + 0xF90 # GameState.menu_items (MenuItem 8 bytes)
const MENU_COUNT: int = 0x800E6280 + 0x1090 # MenuSelection: count[2], pad, current[2] (s8)
const MENU_MODE: int = 0x800E6280 + 0x1092
const MENU_CURSOR_0: int = 0x800E6280 + 0x1093
const MENU_HIT: int = 0x800E6280 + 0x1095 # s8 menu_hit[13]
const PAD_L1: int = 0x0004
const PAD_SELECT: int = 0x0100
const PAD_CROSS: int = 0x0040
const VOICE_PENDING: int = 0
const VOICE_PLAYING: int = 1
const VOICE_DONE: int = 2
const SPEAKER_SPIN_LIMIT: int = 0x10000

# Shift-JIS literals (config/objects/main.txt rodata of this object and of memory_card.c's neighbour 0x800AFB70).
const S_QUOTE_FIRST: int = 0x800AFB70 # "『"
const S_QUOTE_PAREN: int = 0x800AFB74 # "（"
const S_QUOTE_CORNER: int = 0x800AFB78 # "「"
const S_SHIORI: int = 0x800AFB7C # "詩織"
const S_CHAN: int = 0x800AFB84 # "ちゃん"
const S_SAN: int = 0x800AFB8C # "さん"

var engine # GameEngine; set when the engine registers the module

# ---- helpers

func _s8(address: int) -> int:
	return engine.memory.s8(address)

func _item(i: int, menu: int) -> int:
	return MENU_ITEMS + (i * 2 + menu) * 8

func _count(menu: int) -> int:
	return engine.memory.u8(MENU_COUNT + menu)

func _current(menu: int) -> int:
	return engine.memory.s8(MENU_CURSOR_0 + menu)

func _set_current(menu: int, value: int) -> void:
	engine.memory.write_u8(MENU_CURSOR_0 + menu, value)

func _gs(field: String) -> int:
	return engine.gs_address(field)

func _gaddr(name: String) -> int:
	return engine.memory.global_address(name)

func _cs32(function_name: String, args: Array) -> int:
	return Math.s32(engine.call_c(function_name, args))

func _div(a: int, b: int) -> int:
	if b == 0:
		engine.refuse("menu_speaker_girl", "division by zero")
		return 0
	return a / b

func _cstring_bytes_at(address: int, limit: int) -> int:
	var n: int = 0
	while n < limit and engine.memory.u8(address + n) != 0:
		n += 1
	return n

# ---- menus

func menu_set(menu: int, count: int, x: int, y: int, w: int, h: int) -> int:
	var m = engine.memory
	var menu_i: int = Math.s32(menu)
	var count_u: int = Math.u8(count)
	for i: int in 16:
		var item: int = _item(i, menu_i)
		m.write_u8(item + 7, 0) # disabled
		m.write_u8(item + 5, 0) # h
		m.write_u8(item + 4, 0) # w
	if count_u >= 17:
		return -1
	m.write_u8(MENU_COUNT + menu_i, count_u)
	m.write_u8(MENU_HIT + menu_i, 0xFF)
	if menu_i >= 2:
		return -1
	var px: int = x
	var py: int = y
	var pw: int = w
	var ph: int = h
	for i: int in count_u:
		var item: int = _item(i, menu_i)
		m.write_u16(item, m.s16(px))
		px += 2
		m.write_u16(item + 2, m.s16(py))
		py += 2
		if m.s16(pw) * m.s16(ph) == 0:
			m.write_u8(item + 7, 1)
		else:
			m.write_u8(item + 7, 0)
		m.write_u8(item + 4, m.s16(pw))
		pw += 2
		m.write_u8(item + 5, m.s16(ph))
		ph += 2
		m.write_u8(item + 6, 0) # hover
	return 0

## In mode 2 with L1 held the C returns menu_check_2's v0 (&game_state + menu, see menu_check_2).
func menu_check(checked_menu: int, check_pointer_x: int, check_pointer_y: int) -> int:
	var menu: int = Math.s32(checked_menu)
	var px: int = Math.s16(check_pointer_x)
	var py: int = Math.s16(check_pointer_y)
	if menu >= 2:
		return -1
	var mode: int = engine.gs_get("menu_mode") & 3
	if mode == 0:
		return engine.call_c("menu_check_1", [menu, px, py])
	if mode == 1 or mode == 3:
		engine.call_c("menu_check_2", [menu, px, py])
		return _s8(MENU_CURSOR_0 + menu)
	if engine.gs_get("pad_pressed") & PAD_L1:
		return engine.call_c("menu_check_1", [menu, px, py])
	if engine.gs_get("pad_held") & PAD_L1:
		return engine.call_c("menu_check_2", [menu, px, py])
	return engine.call_c("menu_check_1", [menu, px, py])

func menu_check_1(hit_test_menu: int, hit_test_x: int, hit_test_y: int) -> int:
	var m = engine.memory
	var menu: int = Math.s32(hit_test_menu)
	var px: int = Math.s16(hit_test_x)
	var py: int = Math.s16(hit_test_y)
	var hit_count: int = 0
	var hit_item: int = -1
	var i: int = 0
	while i < _count(menu):
		var item: int = _item(i, menu)
		m.write_u8(item + 6, 0)
		var dx: int = px - m.s16(item)
		if Math.s32(dx * (dx - m.u8(item + 4))) < 0:
			var dy: int = py - m.s16(item + 2)
			if Math.s32(dy * (dy - m.u8(item + 5))) < 0:
				m.write_u8(item + 6, 1)
				hit_count += 1
				hit_item = i
		i += 1
	if hit_count == 0:
		_set_current(menu, -1)
	if (engine.gs_get("pad_held") & PAD_L1) == 0 and hit_item != -1:
		m.write_u8(MENU_HIT + menu, hit_item)
	return hit_item

func menu_check_2(cursor_menu: int, _arg1: int, _arg2: int) -> int:
	var m = engine.memory
	var menu: int = Math.s32(cursor_menu)
	var cursor_item: int = -1
	m.write_u8(MENU_HIT + menu, 0xFF)
	var i: int = 0
	if _count(menu) != 0:
		while true:
			var item: int = _item(i, menu)
			if i != _current(menu):
				m.write_u8(item + 6, 0)
			else:
				m.write_u8(item + 6, 10)
				cursor_item = i
			i += 1
			if not (i < _count(menu)):
				break
	if (engine.gs_get("pad_held") & PAD_L1) == 0 and cursor_item != -1:
		m.write_u8(MENU_HIT + menu, cursor_item)
	# v0 at the exit (asm menu_check_2.s): &game_state, plus menu when count[menu] != 0 (the loop's base register).
	return GAME_STATE + (menu if _count(menu) != 0 else 0)

## Wraps current[menu] into 0..count-1 and skips disabled items in `direction` (at most 16 steps).
func _wrap_current(menu: int) -> void:
	if _current(menu) >= _count(menu):
		_set_current(menu, 0)
	if _current(menu) < 0:
		_set_current(menu, _count(menu) - 1)

func _skip_disabled(menu: int, direction: int) -> void:
	var budget: int = 16
	while engine.memory.u8(_item(_current(menu), menu) + 7) == 1:
		var spent: bool = budget == 0
		budget -= 1
		if spent:
			break
		if direction == 1:
			_set_current(menu, _current(menu) + 1)
		if direction == -1:
			_set_current(menu, _current(menu) - 1)
		_wrap_current(menu)

## cursor_x = item.x + item.w - 4, cursor_y = item.y + 8 for the current item.
func _cursor_to_current(menu: int) -> void:
	var m = engine.memory
	var item: int = _item(_current(menu), menu)
	engine.memory.write_global("cursor_x", m.s16(item) + m.u8(item + 4) - 4)
	engine.memory.write_global("cursor_y", m.s16(item + 2) + 8)

func menu_cursor_update(menu: int) -> void:
	var m = engine.memory
	var menu_i: int = Math.s32(menu)
	var direction: int = 0
	if engine.gs_get("pad_pressed") & PAD_SELECT:
		engine.gs_set("menu_mode", engine.gs_get("menu_mode") + 1)
		_set_current(menu_i, -1)
		return
	match engine.gs_get("menu_mode") & 3:
		0:
			engine.call_c("hud_cursor_update")
			var i: int = 0
			while i < _count(menu_i):
				if m.u8(_item(i, menu_i) + 6) != 0:
					_set_current(menu_i, i)
					break
				i += 1
		1:
			if (engine.gs_get("pad_held") & 0xF) == 0:
				match engine.gs_get("pad_pressed") & 0xF000:
					0x1000, 0x8000:
						direction = -1
						_set_current(menu_i, _current(menu_i) - 1)
					0x2000, 0x4000:
						direction = 1
						_set_current(menu_i, _current(menu_i) + 1)
			_wrap_current(menu_i)
			_skip_disabled(menu_i, direction)
			_cursor_to_current(menu_i)
		2:
			if engine.gs_get("pad_held") & PAD_L1:
				if engine.gs_get("pad_pressed") & PAD_L1:
					_set_current(menu_i, _s8(MENU_HIT + menu_i))
				else:
					match engine.gs_get("pad_pressed") & 0xF000:
						0x2000, 0x4000:
							direction = 1
							_set_current(menu_i, _current(menu_i) + 1)
						0x1000, 0x8000:
							direction = -1
							_set_current(menu_i, _current(menu_i) - 1)
					_wrap_current(menu_i)
					_skip_disabled(menu_i, direction)
				_cursor_to_current(menu_i)
			else:
				engine.call_c("hud_cursor_update")
				var j: int = 0
				while j < _count(menu_i):
					if m.u8(_item(j, menu_i) + 6) != 0:
						_set_current(menu_i, j)
					j += 1
		3:
			var found: int = -1
			match engine.gs_get("pad_pressed") & 0xF000:
				0x1000:
					found = engine.call_c("get_near_menu", [1, -1, menu_i])
				0x4000:
					found = engine.call_c("get_near_menu", [1, 1, menu_i])
				0x2000:
					found = engine.call_c("get_near_menu", [0, 1, menu_i])
				0x8000:
					found = engine.call_c("get_near_menu", [0, -1, menu_i])
			if found != -1:
				_set_current(menu_i, found)
			if _current(menu_i) >= _count(menu_i):
				_set_current(menu_i, engine.call_c("menu_cursor_to_edge", [menu_i, 1]))
			if _current(menu_i) < 0:
				_set_current(menu_i, engine.call_c("menu_cursor_to_edge", [menu_i, 0]))
			_cursor_to_current(menu_i)
	var cursor_x: int = m.read_global("cursor_x")
	var cursor_y: int = m.read_global("cursor_y")
	if cursor_x > 140:
		m.write_global("cursor_x", 140)
	if m.read_global("cursor_x") < -150:
		m.write_global("cursor_x", -150)
	if cursor_y < -100:
		m.write_global("cursor_y", -100)
	if m.read_global("cursor_y") > 100:
		m.write_global("cursor_y", 100)

func menu_cursor_to_edge(menu: int, reverse: int) -> int:
	var m = engine.memory
	var menu_i: int = Math.s32(menu)
	if Math.s32(reverse) != 0:
		var i: int = _count(menu_i) - 1
		while i >= 0:
			var item: int = _item(i, menu_i)
			if m.u8(item + 4) != 0 and m.u8(item + 5) != 0:
				_set_current(menu_i, i)
				return i
			i -= 1
	else:
		var j: int = 0
		while j < _count(menu_i):
			var item2: int = _item(j, menu_i)
			if m.u8(item2 + 4) != 0 and m.u8(item2 + 5) != 0:
				_set_current(menu_i, j)
				return j
			j += 1
	return -1

func get_near_menu(axis: int, direction: int, menu: int) -> int:
	var m = engine.memory
	var menu_i: int = Math.s32(menu)
	var item_count: int = 16
	var best_distance: int = 320
	if _count(menu_i) < 16:
		item_count = _count(menu_i)
	var cur: int = _item(_current(menu_i), menu_i)
	var distances: Array = []
	distances.resize(16)
	distances.fill(0)
	var field: int = 0 if Math.s32(axis) == 0 else 2
	if Math.s32(axis) != 0 and Math.s32(axis) != 1:
		# The C's axis switch leaves axis_distances uninitialised (stale stack) for any other axis: no value is invented.
		engine.refuse("get_near_menu", "axis %d: axis_distances is uninitialised" % Math.s32(axis))
		return -1
	for i: int in item_count:
		distances[i] = Math.s16(m.s16(_item(i, menu_i) + field) - m.s16(cur + field))
	if Math.s32(direction) == -1:
		for i: int in item_count:
			if distances[i] > 0:
				distances[i] = 0
			else:
				distances[i] = Math.s16(-distances[i])
	elif Math.s32(direction) == 1:
		for i: int in item_count:
			if distances[i] < 0:
				distances[i] = 0
	var best_item: int = -1
	for i: int in item_count:
		var item: int = _item(i, menu_i)
		if m.u8(item + 7) == 0 and distances[i] != 0:
			if axis == 0 and m.s16(item + 2) == m.s16(cur + 2):
				if best_distance > 0:
					best_distance = -320
				var span: int = -_cs32("abs", [m.s16(item) - m.s16(cur)])
				if best_distance < span:
					best_distance = Math.s16(span)
					best_item = i
			elif axis == 1 and m.s16(item) == m.s16(cur):
				if best_distance > 0:
					best_distance = -320
				var span2: int = -_cs32("abs", [m.s16(item + 2) - m.s16(cur + 2)])
				if best_distance < span2:
					best_distance = Math.s16(span2)
					best_item = i
			elif best_distance > 0:
				if distances[i] < best_distance:
					best_distance = distances[i]
					best_item = i
				elif best_distance == distances[i]:
					if axis == 0:
						if _cs32("abs", [m.s16(_item(best_item, menu_i) + 2) - m.s16(cur + 2)]) > _cs32("abs", [m.s16(item + 2) - m.s16(cur + 2)]):
							best_item = i
					elif axis == 1:
						if _cs32("abs", [m.s16(_item(best_item, menu_i)) - m.s16(cur)]) > _cs32("abs", [m.s16(item) - m.s16(cur)]):
							best_item = i
		if engine.halted():
			return -1
	if best_item >= 0:
		return best_item
	return -1

func menu_bar_color(menu_index: int, rgb_color: int, ordering_slot: int, style_flags: int) -> void:
	var index: int = Math.s32(menu_index)
	if index >= 0x10:
		engine.memory.write_global("menu_bar_style", 0x832F7FAF, 0)
		engine.memory.write_global("menu_bar_style", 0x822F7F2F, 1)
		return
	engine.memory.write_global("menu_bar_style", Math.u32(((ordering_slot & 0xF) << 0x18) + rgb_color + ((style_flags & 0xF) << 0x1C)), index)

func menu_bar_show(menu: int) -> void:
	var m = engine.memory
	var menu_i: int = Math.s32(menu)
	var style: int = Math.u32(m.read_global("menu_bar_style", menu_i))
	var ordering_slot: int = (style >> 24) & 15
	var box_style: int = (style >> 28) & 7
	var transparency: int = (style >> 31) << 7
	var cursor_item: int = _current(menu_i)
	if cursor_item >= 0 and cursor_item < _count(menu_i):
		var item: int = _item(cursor_item, menu_i)
		engine.call_c("prim_gradient_rect", [m.s16(item), m.s16(item + 2), m.u8(item + 4), m.u8(item + 5), ordering_slot, style & 0xFFFFFF, transparency | box_style])
		engine.call_c("dtd_on_tpage", [0, 0, ordering_slot, 0, 1])

## Frame 0x70: x[4] at sp+0x60, width[4] at sp+0x58, height[4] at sp+0x50 (s16 arrays).
func x_taku_menu_set(menu: int, count: int) -> void:
	var m = engine.memory
	var base: int = engine.frame_base()
	var n: int = Math.s32(count)
	for i: int in n:
		m.write_u16(base + 0x60 + i * 2, -120)
		m.write_u16(base + 0x58 + i * 2, 240)
		m.write_u16(base + 0x50 + i * 2, 16)
	var rows: int = 3 if n == 3 else 2
	engine.call_c("menu_set", [menu, rows, base + 0x60, _gaddr("text_row_y"), base + 0x58, base + 0x50])

func x_taku_string_set(first_label: int, second_label: int, third_label: int, choice_count: int) -> void:
	engine.call_c("set_kanji_string", [-0x78, 0x32, 0, first_label, 0])
	engine.call_c("set_kanji_string", [-0x78, 0x42, 0, second_label, 0])
	if Math.s32(choice_count) == 3:
		if third_label != 0:
			engine.call_c("set_kanji_string", [-0x78, 0x52, 0, third_label, 0])

func gnsx(speaker_name: int) -> void:
	match _cs32("strlen", [speaker_name]):
		2:
			engine.call_c("set_kanji_string", [-0x72, 0x32, 0, speaker_name, 0])
		4:
			engine.call_c("set_kanji_string", [-0x79, 0x32, 0, speaker_name, 0])
		_:
			engine.call_c("set_kanji_string", [-0x80, 0x32, 0, speaker_name, 0])

## Frame 0x168: block_text[256] at sp+0x4C, line_break_offsets[5] at sp+0x14C.
func sndisp(text: int, first_row: int, speaker_style: int) -> void:
	var m = engine.memory
	var base: int = engine.frame_base()
	var block: int = base + 0x4C
	var offsets: int = base + 0x14C
	for k: int in 5:
		m.write_u8(offsets + k, 0)
	engine.call_c("strncpy", [block, text, 256])
	var byte_phase: int = 0
	var breaks: int = 0
	var i: int = 0
	while m.u8(block + i) != 0 and i < 0x200:
		if m.u8(block + i) == 0x20 and (byte_phase & 1) == 0:
			m.write_u8(offsets + breaks, i)
			m.write_u8(block + i, 0)
			byte_phase = -1
			breaks += 1
		if m.u8(block + i) == 0x50 and (byte_phase & 1) == 0: # 'P'
			byte_phase = -1
		if m.u8(block + i) == 0x47 and (byte_phase & 1) == 0: # 'G'
			byte_phase = -1
		i += 1
		byte_phase += 1
	engine.gs_set("text_first_line", engine.call_c("check_set_k"))
	engine.call_c("sndi", [block, first_row, speaker_style])
	for k: int in breaks:
		engine.call_c("sndi", [block + m.u8(offsets + k) + 1, first_row + 1 + k, speaker_style])
	engine.gs_set("text_lines_total", engine.call_c("check_set_k"))
	engine.gs_set("text_scroll_state", 0x80)

## Frame 0x118: original[64] sp+0xD4, decorated[40] sp+0xAC, player_expanded[40] sp+0x84, expanded_text[40]
## sp+0x5C, expanded_name[20] sp+0x48. A 'P' (player name) or 'G' (girl name) code at an even offset expands.
func sndi(text: int, row: int, speaker_style: int) -> void:
	var m = engine.memory
	var base: int = engine.frame_base()
	var original: int = base + 0xD4
	var decorated: int = base + 0xAC
	var player_expanded: int = base + 0x84
	var expanded: int = base + 0x5C
	var name: int = base + 0x48
	var style: int = Math.s32(speaker_style)
	engine.call_c("strncpy", [original, text, 64])
	engine.gs_set("text_scroll_state", 0)
	var found: bool = false
	if (style & 15) == 15:
		var i: int = 0
		while m.u8(original + i) != 0 and i < 0x200 and not engine.halted():
			var at: int = original + i
			if m.u8(at) == 0x50 and (i & 1) == 0:
				m.write_u8(at, 0)
				found = true
				engine.call_c("strcpy", [player_expanded, original])
				engine.call_c("get_p_name", [name, style & 15])
				engine.call_c("strcat", [player_expanded, name])
				engine.call_c("strcat", [player_expanded, at + 1])
				break
			i += 1
		if not found:
			engine.call_c("strcpy", [player_expanded, original])
		found = false
		i = 0
		while m.u8(player_expanded + i) != 0 and i < 0x200 and not engine.halted():
			var at2: int = player_expanded + i
			if m.u8(at2) == 0x47 and (i & 1) == 0:
				m.write_u8(at2, 0)
				found = true
				engine.call_c("strcpy", [expanded, player_expanded])
				engine.call_c("get_g_name", [name, engine.gs_get("current_girl")])
				engine.call_c("strcat", [expanded, name])
				engine.call_c("strcat", [expanded, at2 + 1])
				break
			i += 1
		if not found:
			engine.call_c("strcpy", [expanded, player_expanded])
	else:
		var j: int = 0
		while m.u8(original + j) != 0 and j < 0x200 and not engine.halted():
			var at3: int = original + j
			if m.u8(at3) == 0x50 and (j & 1) == 0:
				m.write_u8(at3, 0)
				found = true
				engine.call_c("strcpy", [expanded, original])
				engine.call_c("get_p_name", [name, engine.gs_get("current_girl")])
				engine.call_c("strcat", [expanded, name])
				engine.call_c("strcat", [expanded, at3 + 1])
				break
			j += 1
		if not found:
			engine.call_c("strcpy", [expanded, original])
	match Math.s32(row):
		0:
			if style == 0xF:
				engine.call_c("strcpy", [decorated, S_QUOTE_FIRST])
				engine.call_c("strcat", [decorated, expanded])
			elif style == 0x1F:
				engine.call_c("strcpy", [decorated, S_QUOTE_PAREN])
				engine.call_c("strcat", [decorated, expanded])
			elif style == 0x2F:
				engine.call_c("strcpy", [decorated, expanded])
			else:
				engine.call_c("strcpy", [decorated, S_QUOTE_CORNER])
				engine.call_c("strcat", [decorated, expanded])
			if style == 0xFF:
				engine.call_c("set_kanji_string", [-0x80, 0x32, 0, decorated, 0])
			else:
				engine.call_c("set_kanji_string", [-0x56, 0x32, 0, decorated, 0])
		1:
			if style == 0xFF:
				engine.call_c("set_kanji_string", [-0x80, 0x42, 0, expanded, 0])
			else:
				engine.call_c("set_kanji_string", [-0x48, 0x42, 0, expanded, 0])
		_:
			if style == 0xFF:
				engine.call_c("set_kanji_string", [-0x80, 0x52, 0, expanded, 0])
			else:
				engine.call_c("set_kanji_string", [-0x48, 0x52, 0, expanded, 0])

func set_c_girl(girl_id: int) -> void:
	engine.gs_set("current_girl", Math.u8(girl_id))

func get_g_name(name_buf: int, girl_id: int) -> int:
	var id: int = Math.s32(girl_id)
	var m = engine.memory
	if id == 0:
		engine.call_c("strcpy", [name_buf, S_SHIORI])
	elif id == 0xA:
		engine.call_c("strcpy", [name_buf, Math.u32(m.read_global("girl_short_name_yumi_given"))])
		engine.call_c("strcat", [name_buf, S_CHAN])
	elif id == 0xC:
		engine.call_c("strcpy", [name_buf, Math.u32(m.read_global("girl_short_name_rei"))])
	else:
		engine.call_c("strcpy", [name_buf, m.u32(_gaddr("girl_short_name_table") + (id & 0xF) * 4)])
		engine.call_c("strcat", [name_buf, S_SAN])
	if id >= 0x10 or id < 0:
		return -1
	return 1

## Player name form the girl `girl_form` uses: given / family / nickname plus the familiar or polite suffix,
## chosen by the girl's state (get_g_zyotai_s bit 7 = high affection).
func get_p_name(name_buf: int, girl_form: int) -> void:
	var form: int = Math.u32(girl_form)
	var family: int = _gs("player_family_name")
	var given: int = _gs("player_given_name")
	var nick: int = _gs("player_nickname")
	var fam: int = _gaddr("girl_name_suffix_familiar")
	var pol: int = _gaddr("girl_name_suffix_polite")
	match form:
		0:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				engine.call_c("strcpy", [name_buf, nick])
				return
			_name(name_buf, given, fam)
		1:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, family, fam)
				return
			_name(name_buf, family, pol)
		2:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, given, fam)
				return
			_name(name_buf, family, fam)
		3:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				engine.call_c("strcpy", [name_buf, nick])
				return
			_name(name_buf, family, fam)
		4:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, given, fam)
				return
			var _state: int = engine.call_c("get_g_zyotai_s", [form]) & 0x7F
			_name(name_buf, family, fam)
		5:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, given, pol)
				return
			var _state5: int = engine.call_c("get_g_zyotai_s", [form]) & 0x7F
			_name(name_buf, family, pol)
		6:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				engine.call_c("strcpy", [name_buf, nick])
				return
			_name(name_buf, family, fam)
		7:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, given, fam)
				return
			_name(name_buf, family, fam)
		8:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				engine.call_c("strcpy", [name_buf, nick])
				return
			_name(name_buf, given, fam)
		9:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, family, fam)
				return
			_name(name_buf, family, pol)
		10:
			if engine.call_c("get_g_zyotai_s", [form]) & 0x80:
				_name(name_buf, given, pol)
				return
			_name(name_buf, family, pol)
		13:
			engine.call_c("strcpy", [name_buf, given])
		11:
			_name(name_buf, given, fam)
		12:
			_name(name_buf, family, fam)
		15:
			engine.call_c("strcpy", [name_buf, family])
			engine.call_c("strncat", [name_buf, given, 6])
		_:
			_name(name_buf, family, fam)

func _name(buffer: int, part: int, suffix: int) -> void:
	engine.call_c("strcpy", [buffer, part])
	engine.call_c("strcat", [buffer, suffix])

func get_g_zyotai_s(girl_id: int) -> int:
	if engine.call_c("get_h_yuukou", [girl_id]) < 0xA:
		return 4
	if engine.call_c("get_h_yuukou", [girl_id]) < 0x1E:
		return 3
	if engine.call_c("get_h_yuukou", [girl_id]) < 0x32:
		if engine.call_c("get_h_tokimeki", [girl_id]) < 0x32:
			return 3
		return 1
	if engine.call_c("get_h_yuukou", [girl_id]) < 0x50:
		if engine.call_c("get_h_tokimeki", [girl_id]) < 0x32:
			return 2
		return 1
	if engine.call_c("get_h_tokimeki", [girl_id]) < 0x32:
		return 0x82
	return 0x80

func get_g_zyotai_h(girl_id: int) -> int:
	if engine.call_c("get_h_yuukou", [girl_id]) < 0xA:
		return 4
	if engine.call_c("get_h_yuukou", [girl_id]) < 0x32:
		return 3
	if engine.call_c("get_h_tokimeki", [girl_id]) < 0x32:
		return 2
	if engine.call_c("get_h_tokimeki", [girl_id]) < 0x50:
		return 1
	return 0

## Frame 0xD8: x[12] sp+0xB4, y[12] sp+0x9C, w[12] sp+0x84, h[12] sp+0x6C (s16). The C calls menu_set inside
## the loop on every pass, with the arrays filled only up to entry i.
func menu_girl_taku_set(menu: int, last_text_entry_count: int) -> void:
	var m = engine.memory
	var base: int = engine.frame_base()
	var xs: int = base + 0xB4
	var ys: int = base + 0x9C
	var ws: int = base + 0x84
	var hs: int = base + 0x6C
	var i: int = 0
	while true:
		var girl_id: int = 12 if i == 11 else i
		var met: bool = ((m.u32(_gs("girls") + girl_id * 0x38 + 0xC) >> 1) & 1) != 0
		if met:
			var count: int = engine.call_c("set_kanji_string", [(i / 3) * 64 - 0x80, (i % 3) * 16 + 0x32, 0,
				m.u32(_gaddr("girl_short_name_table") + girl_id * 4), 0])
			m.write_u32(last_text_entry_count, count)
		m.write_u16(xs + i * 2, (i / 3) * 64 - 0x80)
		m.write_u16(ys + i * 2, (i % 3) * 16 + 0x32)
		m.write_u16(ws + i * 2, 0x30 if met else 0)
		m.write_u16(hs + i * 2, 0x10)
		engine.call_c("menu_set", [menu, 12, xs, ys, ws, hs])
		i += 1
		if i == 12 or engine.halted():
			break

## Shared by xa_wait and talk_text_step: the three-line typewriter scroll. Returns text_complete.
func _typewriter(use_voice_gate: bool) -> int:
	var m = engine.memory
	var text_complete: int = 0
	var revealed: int = _cs32("get_now_k", [])
	if engine.gs_get("text_scroll_state") & 0x80:
		var phase: int = engine.gs_get("text_scroll_state") & 0x7F
		if Math.u32(engine.gs_get("text_first_line") + 2) >= Math.u32(revealed):
			text_complete = engine.call_c("k_disp_inc")
			revealed = _cs32("get_now_k", [])
			var first_visible: int = engine.gs_get("text_first_line")
			if revealed == first_visible + 3:
				if engine.gs_get("text_lines_total") >= first_visible + 3:
					engine.call_c("k_disp_switch", [revealed, 0])
		elif phase == 0:
			engine.call_c("k_disp_switch", [engine.gs_get("text_first_line"), 0])
			var middle: int = engine.call_c("get_k_work", [engine.gs_get("text_first_line") + 1])
			var bottom: int = engine.call_c("get_k_work", [engine.gs_get("text_first_line") + 2])
			m.write_u16(middle + 2, m.s16(middle + 2) - 2)
			m.write_u16(bottom + 2, m.s16(bottom + 2) - 2)
			engine.gs_set("text_scroll_state", engine.gs_get("text_scroll_state") + 1)
		else:
			var middle2: int = engine.call_c("get_k_work", [engine.gs_get("text_first_line") + 1])
			var bottom2: int = engine.call_c("get_k_work", [engine.gs_get("text_first_line") + 2])
			m.write_u16(middle2 + 2, m.s16(middle2 + 2) - 2)
			m.write_u16(bottom2 + 2, m.s16(bottom2 + 2) - 2)
			if m.s16(middle2 + 2) < 0x33:
				m.write_u16(middle2 + 2, 0x32)
				m.write_u16(bottom2 + 2, 0x42)
				engine.gs_set("text_scroll_state", engine.gs_get("text_scroll_state") - 1)
				engine.gs_set("text_first_line", engine.gs_get("text_first_line") + 1)
				engine.call_c("k_disp_switch", [engine.gs_get("text_first_line") + 2, 1])
	else:
		if use_voice_gate:
			var voice: int = engine.gs_get("voice_mode")
			var state: int = m.read_global("speaker_voice_state")
			if (voice == 1 and state != VOICE_PENDING) or (voice == 2 and state != VOICE_PENDING):
				text_complete = engine.call_c("k_disp_inc")
			if engine.gs_get("voice_mode") == 0:
				text_complete = engine.call_c("k_disp_inc")
		else:
			text_complete = engine.call_c("k_disp_inc")
		engine.call_c("get_now_k")
	return text_complete

func _text_marker() -> void:
	if engine.memory.u8(_gaddr("D_8011ED5B")) & 0x80:
		engine.call_c("hud_text_marker_bob")
	else:
		engine.call_c("hud_text_marker_set", [1])

func xa_wait() -> void:
	var text_complete: int = _typewriter(false)
	if text_complete == 1:
		_text_marker()
	if engine.gs_get("pad_pressed") & 0x60:
		if text_complete == 1:
			engine.call_c("hud_text_marker_set", [0])
			engine.gs_set("substep", engine.gs_get("substep") + 1)
			engine.gs_set("text_scroll_state", 0)
			return
		engine.call_c("k_disp_goto_line_end")

func text_wait_confirm() -> void:
	engine.call_c("k_disp_inc")
	if engine.gs_get("pad_pressed") & 0x860:
		engine.call_c("k_disp_goto_line_end")
		if engine.call_c("check_end_k") != 0:
			engine.gs_set("substep", engine.gs_get("substep") + 1)

func talk_voice_play(voice_source_a: int, voice_source_b: int, clip_index: int, voice_selector: int) -> void:
	var m = engine.memory
	var clip: int = Math.s32(clip_index)
	var speaker_code: int = Math.s32(voice_selector) & 15
	if clip == -1:
		if speaker_code != 15:
			if engine.gs_get("voice_mode") != 0:
				engine.call_c("cd_audio_request_set", [voice_source_a, voice_source_b, voice_selector])
				engine.call_c("audio_command", [0x300])
	else:
		if speaker_code != 15:
			if engine.gs_get("voice_mode") != 0:
				var table: int = Math.u32(voice_source_a)
				engine.call_c("cd_audio_request_set", [m.s32(table + clip * 8), m.s32(table + clip * 8 + 4), voice_selector])
				engine.call_c("audio_command", [0x300])
	m.write_global("speaker_voice_source_a", voice_source_a)
	m.write_global("speaker_voice_source_b", voice_source_b)
	m.write_global("speaker_voice_index", clip)
	if speaker_code == 15:
		m.write_global("speaker_voice_state", VOICE_DONE)
		return
	m.write_global("speaker_voice_state", VOICE_PENDING)

func talk_text_step() -> void:
	var m = engine.memory
	var text_complete: int = _typewriter(true)
	if m.read_global("speaker_voice_state") == VOICE_PENDING and (engine.call_c("cd_audio_state") & 4):
		m.write_global("speaker_voice_state", VOICE_PLAYING)
	var request: int = Math.u32(engine.call_c("cd_audio_request_get"))
	if m.read_global("speaker_voice_state") == VOICE_PLAYING:
		if engine.call_c("cd_audio_state") & 2:
			var index: int = m.read_global("speaker_voice_index")
			if index != -1:
				var source_a: int = Math.u32(m.read_global("speaker_voice_source_a"))
				var source_b: int = Math.u32(m.read_global("speaker_voice_source_b"))
				if m.s32(source_a + index * 8) != m.s32(source_b + index * 8) and m.s32(source_a + index * 8) == m.s32(request + 8):
					engine.call_c("cd_audio_request_set", [m.s32(source_b + index * 8), m.s32(source_b + index * 8 + 4), engine.gs_get("current_girl")])
					engine.call_c("audio_command", [0x300])
					m.write_global("speaker_voice_state", VOICE_PENDING)
				else:
					m.write_global("speaker_voice_state", VOICE_DONE)
			else:
				m.write_global("speaker_voice_state", VOICE_DONE)
	if text_complete == 1 and m.read_global("speaker_voice_state") == VOICE_DONE:
		_text_marker()
	if text_complete == 1 and engine.gs_get("voice_mode") == 0:
		_text_marker()
	if engine.gs_get("pad_pressed") & PAD_CROSS:
		if text_complete == 1:
			if engine.gs_get("voice_mode") == 2:
				m.write_global("speaker_voice_state", VOICE_DONE)
				engine.call_c("audio_command", [0x74])
				engine.call_c("cd_audio_command", [9, 0, 0])
			engine.gs_set("text_scroll_state", 0)
		else:
			engine.call_c("k_disp_goto_line_end")
	if engine.gs_get("pad_pressed") & 0x60:
		if text_complete == 1:
			var voice: int = engine.gs_get("voice_mode")
			if voice == 1 or voice == 2:
				if m.read_global("speaker_voice_state") == VOICE_DONE:
					engine.call_c("hud_text_marker_set", [0])
					engine.gs_set("substep", engine.gs_get("substep") + 1)
				engine.gs_set("text_scroll_state", 0)
				return
			engine.call_c("hud_text_marker_set", [0])
			engine.gs_set("substep", engine.gs_get("substep") + 1)
			engine.gs_set("text_scroll_state", 0)
			return
		engine.call_c("k_disp_goto_line_end")

# ---- girl scores

func _param_value(i: int) -> int:
	return engine.memory.s16(_gs("param_condition") + i * 4 + 2)

## Weighted tokimeki: record value * weight / 100 plus one bonus for every parameter of her mask, picked by
## the threshold ladder of her kind. The ladder bases are u32 copies of girl_param_ladder_base, so the
## comparison with the s16 parameter is unsigned (as in the C).
func get_h_tokimeki(girl_id: int) -> int:
	var m = engine.memory
	var girl: int = Math.s32(girl_id) % 13
	var weighted: int = m.s16(_gs("girls") + girl * 0x38 + 2) * m.u8(_gaddr("girl_tokimeki_weight") + girl)
	weighted = weighted / 100
	var mask: int = m.u8(_gaddr("girl_bonus_param_mask") + girl)
	var enabled: Array = [0, 0, 0, 0, 0, 0, 0, 0]
	for i: int in range(1, 8):
		enabled[i] = 1 if (mask & (1 << i)) != 0 else 0
	var kind: int = m.u8(_gaddr("girl_param_ladder_kind") + girl)
	var bases: Array = []
	for k: int in 7:
		bases.append(m.u8(_gaddr("girl_param_ladder_base") + k))
	var bonus: int = _gaddr("girl_param_bonus") + girl * 8
	# Per kind: the offsets added to bases[0..6] in the comparison (the C's literal thresholds).
	var offsets: Array = []
	match kind:
		1:
			offsets = [50, 35, 20, 20, 5, 5, 0]
		2:
			offsets = [0, 0, 0, 0, 0, 0, 0]
		3:
			offsets = [50, 50, 50, 20, 20, 20, 20]
		4:
			offsets = [10, 10, 10, 10, 0, 0, 0]
		5:
			offsets = [20, 20, 20, 20, 0, -5, 0]
		6:
			offsets = [0, 0, 0, 0, -5, -5, -5]
	for i: int in range(1, 7):
		if enabled[i] != 0 and not offsets.is_empty():
			var value: int = Math.u32(_param_value(i))
			var step: int = 7
			for k: int in 7:
				if value > Math.u32(bases[k] + offsets[k]):
					step = k
					break
			weighted += m.s8(bonus + step)
	if weighted >= 0x65:
		return 0x64
	if weighted < 0:
		return 0
	return weighted & 0xFF

func get_h_yuukou(girl_id: int) -> int:
	var m = engine.memory
	var index: int = Math.s32(girl_id) % 13
	var weighted: int = m.s16(_gs("girls") + index * 0x38 + 6) * m.u8(_gaddr("girl_affection_weight") + index)
	weighted = weighted / 100
	if weighted >= 0x65:
		return 0x64
	if weighted < 0:
		return 0
	return weighted & 0xFF

func get_h_tokimeki_table(scores_out: int) -> void:
	for i: int in 11:
		engine.memory.write_u8(scores_out + i, engine.call_c("get_h_tokimeki", [i]))

func get_h_yuukou_table(scores_out: int) -> void:
	for i: int in 11:
		engine.memory.write_u8(scores_out + i, engine.call_c("get_h_yuukou", [i]))

func birth_day_check(birthday_subject: int) -> void:
	engine.call_c("birth_day_check_days", [birthday_subject, engine.gs_get("month"), engine.gs_get("day")])

func birth_day_check_days(birthday_subject: int, month: int, day: int) -> int:
	var m = engine.memory
	var month_i: int = Math.s32(month)
	if month_i == 0xD:
		month_i = 1
	var subject: int = Math.s32(birthday_subject) & 0xF
	var word: int
	if subject == 0xF:
		word = m.s32(_gs("player_birthday"))
	else:
		word = m.s32(_gs("girls") + subject * 0x38 + 0x10)
	if month_i == (word & 0xF):
		if Math.u32(Math.s32(day)) == ((Math.u32(word) << 23) & 0xFFFFFFFF) >> 27:
			return 1
	return 0

func girl_points_shiori(closeness_by_girl: int, points_by_girl: int) -> int:
	var m = engine.memory
	if m.u8(closeness_by_girl) < 0x55:
		return 0
	if m.u8(_gs("player_flags") + 1) & 0xF:
		return 0
	var tail: int = _gaddr("girl_shiori_param_minimum_tail")
	var i: int = 1
	while true:
		if Math.u32(_param_value(i)) < m.u8(tail - 1):
			return 0
		i += 1
		tail += 1
		if not (i < 8):
			break
	var girl0: int = _gs("girls")
	var scene_count: int = 0
	for k: int in range(1, 36):
		scene_count += m.u8(girl0 + 0x14 + k)
	if scene_count < 8:
		return 0
	if m.s16(girl0 + 0xA) >= 0x33:
		return 0
	m.write_u16(points_by_girl, 0x311)
	return 0 # unobserved placeholder: the C falls off the end here (v0 is a leftover), all callers discard it

## girl_points[11] is the s16 array at sp+0x48 of the 0x80 frame.
func girl_points_others(closeness_by_girl: int, points_by_girl: int) -> int:
	var m = engine.memory
	var points: int = engine.frame_base() + 0x48
	var girls: int = _gs("girls")
	for g: int in range(1, 11):
		m.write_u16(points + g * 2, m.u8(closeness_by_girl + g) * 4)
		m.write_u16(points + g * 2, m.s16(points + g * 2) + m.s16(girls + g * 0x38 + 0xA) * m.s8(_gaddr("girl_syoushin_weight") + g))
	var score_input: int = _param_value(7) # param_stress is param_condition[7] (GameState +0x11C)
	score_input = m.s16(_gs("param_stress") + 2)
	if score_input >= 141:
		score_input = 140
	m.write_u16(points + 4, m.s16(points + 4) + m.s8(_gaddr("girl_points_stress_curve") + score_input / 10))
	for i: int in range(1, 11):
		for k: int in 7:
			if (m.u8(_gaddr("girl_points_mask_strong") + i) >> (7 - k)) & 1:
				var bucket: int = _param_value(7 - k)
				bucket = bucket / 10
				if bucket >= 15:
					bucket = 14
				var curve: int = m.s8(_gaddr("girl_points_param_curve") + bucket)
				m.write_u16(points + i * 2, m.s16(points + i * 2) + curve)
				m.write_u16(points + i * 2, m.s16(points + i * 2) + curve / 2)
			if (m.u8(_gaddr("girl_points_mask_normal") + i) >> (7 - k)) & 1:
				var bucket2: int = _param_value(7 - k)
				bucket2 = bucket2 / 10
				if bucket2 >= 15:
					bucket2 = 14
				m.write_u16(points + i * 2, m.s16(points + i * 2) + m.s8(_gaddr("girl_points_param_curve") + bucket2))
	for g: int in range(1, 11):
		var total: int = 0
		for k: int in range(1, 36):
			total += m.u8(girls + g * 0x38 + 0x14 + k)
		if total >= 4:
			m.write_u16(points + g * 2, m.s16(points + g * 2) + total * 10)
		else:
			m.write_u16(points + g * 2, 0)
	for g: int in range(1, 11):
		m.write_u16(points_by_girl + g * 2, m.s16(points + g * 2))
	# v0 at the exit (asm girl_points_others.s): the loop's end pointer, sp + 0x5E.
	return engine.frame_base() + 0x5E

## weighted_affection[16] at sp+0x44 and weighted_tokimeki[16] at sp+0x54 of the 0x68 frame.
func girl_scores(closeness_by_girl: int, points_by_girl: int) -> int:
	var m = engine.memory
	var affection: int = engine.frame_base() + 0x44
	var tokimeki: int = engine.frame_base() + 0x54
	engine.call_c("get_h_yuukou_table", [affection])
	engine.call_c("get_h_tokimeki_table", [tokimeki])
	for i: int in 16:
		m.write_u16(points_by_girl + i * 2, 0)
		m.write_u8(affection + i, m.u8(affection + i) / 5)
		if m.u8(affection + i) >= 20:
			m.write_u8(affection + i, 19)
		m.write_u8(tokimeki + i, m.u8(tokimeki + i) / 5)
		if m.u8(tokimeki + i) >= 20:
			m.write_u8(tokimeki + i, 19)
		m.write_u8(closeness_by_girl + i, m.u8(_gaddr("girl_closeness_grid") + m.u8(tokimeki + i) * 20 + m.u8(affection + i)))
	engine.call_c("girl_points_shiori", [closeness_by_girl, points_by_girl])
	return engine.call_c("girl_points_others", [closeness_by_girl, points_by_girl])

func girl_player_sports_club_check() -> int:
	var club: int = engine.memory.u16(_gs("player_flags")) >> 0xC
	if club >= 6 and club < 0xB:
		return 1
	return 0

func girl_find_by_club(club_id: int) -> int:
	var girls: int = _gs("girls")
	for i: int in 11:
		if Math.u32(club_id) == (engine.memory.u8(girls + i * 0x38 + 0xC + 2) >> 4):
			return i
	return -1
