extends RefCounted
## T-39313: engine/menu_speaker_girl.gd on invented memory; expectations derived by hand from
## src/main/menu_speaker_girl.c. The emulator captures (config/port/engine-diff/menu_speaker_girl.json) cover
## the functions the menus and dialogue of the sampled scenes reach; these checks cover the rest.

const GameEngine = preload("res://engine/engine.gd")
const Math = preload("res://core/native_math.gd")

const ITEMS: int = 0x800E6280 + 0xF90
const COUNT: int = 0x800E6280 + 0x1090
const CURSOR: int = 0x800E6280 + 0x1093

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
	_test_registration()
	_test_menu_set()
	_test_edge_and_near()
	_test_choice_menus()
	_test_girl_queries()
	_test_text()
	_test_voice()
	_test_refusals_and_layout()
	for engine in _made:
		engine.shutdown()
	_made.clear()
	return failures

func _test_registration() -> void:
	var engine = _new()
	var module = engine.modules.get("menu_speaker_girl")
	_check(engine.registration_errors.is_empty(), "registration errors: %s" % [engine.registration_errors])
	_check(module != null and module.C_FUNCTIONS.size() == 35 and module.C_UNPORTED.is_empty(), "menu_speaker_girl lists all 35 functions")

func _item(i: int, menu: int) -> int:
	return ITEMS + (i * 2 + menu) * 8

func _put_rects(engine, address: int, values: Array) -> void:
	for i: int in values.size():
		engine.memory.write_u16(address + i * 2, values[i])

func _test_menu_set() -> void:
	var engine = _new()
	var m = engine.memory
	var xs: int = 0x80150000
	_put_rects(engine, xs, [10, 20, 30])
	_put_rects(engine, xs + 0x20, [1, 2, 3])
	_put_rects(engine, xs + 0x40, [0x40, 0, 0x30])
	_put_rects(engine, xs + 0x60, [0x10, 0x10, 0])
	m.fill(ITEMS, 0x100, 0x55)
	engine.call_c("menu_set", [1, 3, xs, xs + 0x20, xs + 0x40, xs + 0x60])
	_check(m.u8(COUNT + 1) == 3 and m.s8(0x800E6280 + 0x1095 + 1) == -1, "count[menu] = 3, menu_hit[menu] = -1")
	_check(m.s16(_item(0, 1)) == 10 and m.s16(_item(0, 1) + 2) == 1 and m.u8(_item(0, 1) + 4) == 0x40 and m.u8(_item(0, 1) + 5) == 0x10 and m.u8(_item(0, 1) + 7) == 0, "item 0: x, y, w, h, enabled")
	_check(m.u8(_item(1, 1) + 7) == 1 and m.u8(_item(2, 1) + 7) == 1, "w * h == 0 disables the item (item 1: w = 0, item 2: h = 0)")
	_check(m.u8(_item(5, 1) + 4) == 0 and m.u8(_item(5, 1) + 5) == 0 and m.u8(_item(5, 1) + 7) == 0, "items 3..15 of the menu are cleared (w, h, disabled)")
	_check(m.u8(_item(5, 0) + 4) == 0x55, "the other menu's records are not touched")
	_check(engine.call_c("menu_set", [0, 17, xs, xs, xs, xs]) == -1 and m.u8(COUNT) != 17, "count >= 17: -1, count not stored")
	_check(engine.call_c("menu_set", [2, 3, xs, xs, xs, xs]) == -1 and m.u8(COUNT + 2) == 3, "menu >= 2: -1, but count[menu] and menu_hit[menu] were already stored (as in the C)")
	# menu_check: modes
	var seen: Array = []
	engine.set_stub("menu_check_1", func(a: Array) -> int:
		seen.append(["1", a.duplicate()])
		return 4)
	engine.set_stub("menu_check_2", func(a: Array) -> int:
		seen.append(["2", a.duplicate()])
		return 0)
	_check(engine.call_c("menu_check", [2, 0, 0]) == -1 and seen.is_empty(), "menu >= 2 -> -1")
	engine.gs_set("menu_mode", 0)
	_check(engine.call_c("menu_check", [0, -5, 7]) == 4 and seen[0] == ["1", [0, -5, 7]], "mode 0: hit test with the (s16) pointer")
	engine.gs_set("menu_mode", 1)
	m.write_u8(CURSOR + 1, 6)
	_check(engine.call_c("menu_check", [1, 1, 2]) == 6 and seen[1][0] == "2", "mode 1: menu_check_2 then the cursor of that menu")
	engine.gs_set("menu_mode", 2)
	engine.gs_set("pad_pressed", 4)
	_check(engine.call_c("menu_check", [0, 1, 2]) == 4 and seen[2][0] == "1", "mode 2, L1 pressed: pointer hit test")
	engine.gs_set("pad_pressed", 0)
	engine.gs_set("pad_held", 4)
	engine.call_c("menu_check", [0, 1, 2])
	_check(seen[3][0] == "2", "mode 2, L1 held: cursor update")
	engine.gs_set("pad_held", 0)
	engine.call_c("menu_check", [0, 1, 2])
	_check(seen[4][0] == "1", "mode 2, no L1: pointer hit test")

func _setup_menu(engine, menu: int, rects: Array) -> void:
	var m = engine.memory
	m.write_u8(COUNT + menu, rects.size())
	for i: int in rects.size():
		var item: int = _item(i, menu)
		m.write_u16(item, rects[i][0])
		m.write_u16(item + 2, rects[i][1])
		m.write_u8(item + 4, rects[i][2])
		m.write_u8(item + 5, rects[i][3])
		m.write_u8(item + 6, 0)
		m.write_u8(item + 7, 0 if rects[i][2] * rects[i][3] != 0 else 1)

func _test_edge_and_near() -> void:
	var engine = _new()
	var m = engine.memory
	# three items in a row, the middle one disabled-empty (w = 0)
	_setup_menu(engine, 0, [[0, 0, 16, 16], [20, 0, 0, 16], [40, 0, 16, 16]])
	_check(engine.call_c("menu_cursor_to_edge", [0, 0]) == 0 and m.s8(CURSOR) == 0, "to_edge forward: the first item with w and h")
	_check(engine.call_c("menu_cursor_to_edge", [0, 1]) == 2 and m.s8(CURSOR) == 2, "to_edge reverse: the last item with w and h")
	_setup_menu(engine, 1, [[0, 0, 0, 0]])
	_check(engine.call_c("menu_cursor_to_edge", [1, 0]) == -1, "to_edge with no usable item: -1")
	# get_near_menu on a 2 x 2 grid: items 0 (0,0) 1 (50,0) 2 (0,30) 3 (50,30), current 0
	_setup_menu(engine, 0, [[0, 0, 16, 16], [50, 0, 16, 16], [0, 30, 16, 16], [50, 30, 16, 16]])
	m.write_u8(CURSOR, 0)
	engine.set_stub("abs", func(a: Array) -> int: return absi(Math.s32(a[0])))
	_check(engine.call_c("get_near_menu", [0, 1, 0]) == 1, "right of item 0 on the same row: item 1")
	_check(engine.call_c("get_near_menu", [1, 1, 0]) == 2, "below item 0 in the same column: item 2")
	_check(engine.call_c("get_near_menu", [0, -1, 0]) == -1, "nothing to the left of item 0")
	m.write_u8(CURSOR, 3)
	_check(engine.call_c("get_near_menu", [1, -1, 0]) == 1, "above item 3: item 1")
	# menu_cursor_update mode 3 (direction pad moves to the nearest item)
	m.write_u8(CURSOR, 0)
	engine.gs_set("menu_mode", 3)
	engine.gs_set("pad_pressed", 0x2000)
	engine.call_c("menu_cursor_update", [0])
	_check(m.s8(CURSOR) == 1 and m.read_global("cursor_x") == 50 + 16 - 4 and m.read_global("cursor_y") == 0 + 8, "mode 3: RIGHT moves to item 1 and puts the cursor at (x + w - 4, y + 8)")
	engine.gs_set("pad_pressed", 0x0100) # SELECT steps menu_mode and clears the current item
	engine.call_c("menu_cursor_update", [0])
	_check(engine.gs_get("menu_mode") == 4 and m.s8(CURSOR) == -1, "SELECT: menu_mode + 1 and current = -1")
	# mode 1 skips disabled items in the pressed direction
	_setup_menu(engine, 0, [[0, 0, 16, 16], [20, 0, 0, 16], [40, 0, 16, 16]])
	m.write_u8(CURSOR, 0)
	engine.gs_set("menu_mode", 1)
	engine.gs_set("pad_held", 0)
	engine.gs_set("pad_pressed", 0x2000)
	engine.call_c("menu_cursor_update", [0])
	_check(m.s8(CURSOR) == 2, "mode 1: RIGHT from 0 skips the disabled item 1")
	engine.gs_set("pad_pressed", 0x8000)
	engine.call_c("menu_cursor_update", [0])
	_check(m.s8(CURSOR) == 0, "mode 1: LEFT from 2 skips it back to 0")
	engine.gs_set("pad_pressed", 0x8000)
	engine.call_c("menu_cursor_update", [0])
	_check(m.s8(CURSOR) == 2, "mode 1: LEFT from 0 wraps to the last item")

func _test_choice_menus() -> void:
	var engine = _new()
	var m = engine.memory
	var calls: Array = []
	engine.set_stub("menu_set", func(a: Array) -> int:
		var row: Array = [a[0], a[1]]
		for k: int in 4:
			var values: Array = []
			var source: int = a[2 + k]
			for i: int in 3:
				values.append(m.s16(source + i * 2))
			row.append(values)
		calls.append(row)
		return 0)
	for i: int in 4:
		m.write_u16(0x800B40F0 + i * 2, 0x32 + 0x10 * i) # text_row_y
	engine.call_c("x_taku_menu_set", [1, 3])
	_check(calls[0][0] == 1 and calls[0][1] == 3 and calls[0][2] == [-120, -120, -120] and calls[0][3] == [0x32, 0x42, 0x52] and calls[0][4] == [240, 240, 240] and calls[0][5] == [16, 16, 16], "x_taku_menu_set(menu, 3): three centred 240 x 16 rows at text_row_y: %s" % [calls])
	engine.call_c("x_taku_menu_set", [0, 2])
	_check(calls[1][1] == 2 and calls[1][0] == 0, "x_taku_menu_set with any other count: two rows")
	var kanji: Array = []
	engine.set_stub("set_kanji_string", func(a: Array) -> int:
		kanji.append(a.duplicate())
		return 5)
	engine.call_c("x_taku_string_set", [0x100, 0x200, 0x300, 3])
	_check(kanji == [[-0x78, 0x32, 0, 0x100, 0], [-0x78, 0x42, 0, 0x200, 0], [-0x78, 0x52, 0, 0x300, 0]], "x_taku_string_set: three labels on rows 0x32, 0x42, 0x52")
	kanji.clear()
	engine.call_c("x_taku_string_set", [0x100, 0x200, 0, 3])
	_check(kanji.size() == 2, "a null third label is skipped")
	kanji.clear()
	engine.call_c("x_taku_string_set", [0x100, 0x200, 0x300, 2])
	_check(kanji.size() == 2, "two choices: the third label is not drawn")
	# menu_girl_taku_set: girls 0 and 12 met, the rest not
	calls.clear()
	kanji.clear()
	var girls: int = engine.gs_address("girls")
	m.write_u32(girls + 0xC, 2) # girl 0: flag bit 1 (met)
	m.write_u32(girls + 12 * 0x38 + 0xC, 2) # girl 12
	for i: int in 13:
		m.write_u32(0x800B35F4 + i * 4, 0x80170000 + i * 0x10)
	var out: int = 0x80150000
	engine.call_c("menu_girl_taku_set", [1, out])
	_check(kanji.size() == 2 and kanji[0][3] == 0x80170000 and kanji[0][0] == -0x80 and kanji[0][1] == 0x32, "menu_girl_taku_set names met girls only (girl 0 at i = 0)")
	_check(kanji[1][3] == 0x800000 + 0x80170000 - 0x800000 + 12 * 0x10 and kanji[1][0] == (11 / 3) * 64 - 0x80 and kanji[1][1] == (11 % 3) * 16 + 0x32, "entry 11 is girl 12, at column (i / 3) row (i % 3)")
	_check(calls.size() == 12 and calls[11][4][0] == 0x30 and calls[11][1] == 12, "menu_set runs on every pass (12 times) with count 12")
	_check(m.s32(out) == 5, "the last set_kanji_string result is stored through the pointer")

func _test_girl_queries() -> void:
	var engine = _new()
	var m = engine.memory
	var girls: int = engine.gs_address("girls")
	# birthday (month 3, day 14): bits 0..3 month, bits 4..8 day
	var word: int = 3 | (14 << 4) | (1 << 9)
	m.write_u32(girls + 2 * 0x38 + 0x10, word)
	m.write_u32(engine.gs_address("player_birthday"), 7 | (21 << 4))
	_check(engine.call_c("birth_day_check_days", [2, 3, 14]) == 1, "girl 2's birthday matches")
	_check(engine.call_c("birth_day_check_days", [2, 3, 15]) == 0 and engine.call_c("birth_day_check_days", [2, 4, 14]) == 0, "a different day or month does not")
	_check(engine.call_c("birth_day_check_days", [0xF, 7, 21]) == 1 and engine.call_c("birth_day_check_days", [0x1F, 7, 21]) == 1, "subject 0xF is the player (the low nibble selects)")
	_check(engine.call_c("birth_day_check_days", [2, 0xD, 14]) == 0 and engine.call_c("birth_day_check_days", [0xF, 0xD, 21]) == 0, "month 13 counts as January")
	m.write_u32(girls + 3 * 0x38 + 0x10, 1 | (5 << 4))
	_check(engine.call_c("birth_day_check_days", [3, 0xD, 5]) == 1, "month 13 matches a January birthday")
	engine.gs_set("month", 3)
	engine.gs_set("day", 14)
	engine.call_c("birth_day_check", [2]) # no return value; must not fault
	_check(not engine.halted(), "birth_day_check runs")
	# girl_find_by_club: the club is the high nibble of flag byte 2
	for i: int in 11:
		m.write_u8(girls + i * 0x38 + 0xC + 2, 0)
	m.write_u8(girls + 4 * 0x38 + 0xC + 2, 0x70)
	_check(engine.call_c("girl_find_by_club", [7]) == 4 and engine.call_c("girl_find_by_club", [9]) == -1, "find_by_club returns the first girl of that club, else -1")
	m.write_u8(girls + 1 * 0x38 + 0xC + 2, 0x75)
	_check(engine.call_c("girl_find_by_club", [7]) == 1, "the low nibble is ignored, lowest index wins")
	# get_g_name
	m.write_u32(0x800B3624, 0x80170100)
	m.write_u32(0x800B3630, 0x80170200)
	var strings: Array = []
	engine.set_stub("strcpy", func(a: Array) -> int:
		strings.append(["strcpy", a[1]])
		return a[0])
	engine.set_stub("strcat", func(a: Array) -> int:
		strings.append(["strcat", a[1]])
		return a[0])
	_check(engine.call_c("get_g_name", [0x80150000, 0]) == 1 and strings == [["strcpy", 0x800AFB7C]], "girl 0 is the literal name only")
	strings.clear()
	engine.call_c("get_g_name", [0x80150000, 0xA])
	_check(strings == [["strcpy", 0x80170200], ["strcat", 0x800AFB84]], "girl 10: the given-name pointer + the -chan suffix")
	strings.clear()
	engine.call_c("get_g_name", [0x80150000, 0xC])
	_check(strings == [["strcpy", 0x80170100]], "girl 12: the rei pointer only")
	strings.clear()
	m.write_u32(0x800B35F4 + 5 * 4, 0x80170300)
	engine.call_c("get_g_name", [0x80150000, 5])
	_check(strings == [["strcpy", 0x80170300], ["strcat", 0x800AFB8C]], "other girls: table entry + the -san suffix")
	_check(engine.call_c("get_g_name", [0x80150000, 0x10]) == -1 and engine.call_c("get_g_name", [0x80150000, -1]) == -1, "out-of-range ids return -1")
	# the player's club check
	m.write_u16(engine.gs_address("player_flags"), 6 << 12)
	_check(engine.call_c("girl_player_sports_club_check") == 1, "club 6 is a sports club")
	m.write_u16(engine.gs_address("player_flags"), 11 << 12)
	_check(engine.call_c("girl_player_sports_club_check") == 0, "club 11 is not")

func _test_text() -> void:
	var engine = _new()
	var calls: Array = []
	engine.set_stub("k_disp_inc", func(_a: Array) -> int:
		calls.append("inc")
		return 0)
	engine.set_stub("k_disp_goto_line_end", func(_a: Array) -> int:
		calls.append("end")
		return 0)
	var finished: Array = [0]
	engine.set_stub("check_end_k", func(_a: Array) -> int: return finished[0])
	engine.gs_set("substep", 3)
	engine.gs_set("pad_pressed", 0)
	engine.call_c("text_wait_confirm")
	_check(calls == ["inc"] and engine.gs_get("substep") == 3, "no button: only the typewriter advances")
	engine.gs_set("pad_pressed", 0x800)
	engine.call_c("text_wait_confirm")
	_check(calls == ["inc", "inc", "end"] and engine.gs_get("substep") == 3, "button 0x800 jumps to the line end; not finished: substep unchanged")
	finished[0] = 1
	engine.call_c("text_wait_confirm")
	_check(engine.gs_get("substep") == 4, "finished text advances the substep")

func _voice_engine():
	var engine = _new()
	var log: Array = []
	for n: String in ["k_disp_inc", "get_now_k", "k_disp_goto_line_end", "hud_text_marker_set", "hud_text_marker_bob", "audio_command", "cd_audio_command", "cd_audio_request_set"]:
		engine.set_stub(n, func(a: Array) -> int:
			log.append([n, a.duplicate()])
			return 1 if n == "k_disp_inc" else 0)
	return [engine, log]

func _test_voice() -> void:
	# talk_voice_play, voice on
	var pair = _voice_engine()
	var engine = pair[0]
	var log: Array = pair[1]
	var m = engine.memory
	engine.gs_set("voice_mode", 1)
	engine.call_c("talk_voice_play", [0x80160000, 0x80161000, -1, 0x3])
	_check(log == [["cd_audio_request_set", [0x80160000, 0x80161000, 3]], ["audio_command", [0x300]]] and m.read_global("speaker_voice_state") == 0 and m.read_global("speaker_voice_index") == -1, "clip -1: request (a, b, selector), audio_command(0x300), state PENDING: %s" % [log])
	log.clear()
	m.write_u32(0x80160000 + 8, 111)
	m.write_u32(0x80160000 + 12, 222)
	engine.call_c("talk_voice_play", [0x80160000, 0x80161000, 1, 0x3])
	_check(log[0] == ["cd_audio_request_set", [111, 222, 3]] and m.read_global("speaker_voice_index") == 1, "clip 1: the request words come from source_a[clip * 2], [clip * 2 + 1]")
	log.clear()
	engine.call_c("talk_voice_play", [0x80160000, 0x80161000, 1, 0xF])
	_check(log.is_empty() and m.read_global("speaker_voice_state") == 2, "selector & 15 == 15: no request, state DONE")
	engine.gs_set("voice_mode", 0)
	engine.call_c("talk_voice_play", [0x80160000, 0x80161000, -1, 0x3])
	_check(log.is_empty() and m.read_global("speaker_voice_state") == 0, "voice off: no request, state still PENDING")
	# talk_text_step with voice on
	var p2 = _voice_engine()
	var e = p2[0]
	var l: Array = p2[1]
	var mm = e.memory
	var cd_state: Array = [0]
	e.set_stub("cd_audio_state", func(_a: Array) -> int: return cd_state[0])
	var request: int = 0x80150000
	e.set_stub("cd_audio_request_get", func(_a: Array) -> int: return request)
	e.gs_set("voice_mode", 1)
	e.gs_set("text_scroll_state", 0)
	mm.write_global("speaker_voice_state", 0) # PENDING
	e.call_c("talk_text_step")
	_check(not l.any(func(x: Array) -> bool: return x[0] == "k_disp_inc"), "voice 1 and PENDING: the typewriter does not advance")
	cd_state[0] = 4
	e.call_c("talk_text_step")
	_check(mm.read_global("speaker_voice_state") == 1, "PENDING + playback flag (bit 2) -> PLAYING")
	# PLAYING, finished (bit 1), a second part exists and matches the request: chain it
	var src_a: int = 0x80160000
	var src_b: int = 0x80161000
	mm.write_global("speaker_voice_source_a", src_a)
	mm.write_global("speaker_voice_source_b", src_b)
	mm.write_global("speaker_voice_index", 1)
	mm.write_u32(src_a + 8, 500)
	mm.write_u32(src_b + 8, 600)
	mm.write_u32(src_b + 12, 700)
	mm.write_u32(request + 8, 500)
	e.gs_set("current_girl", 4)
	cd_state[0] = 2
	l.clear()
	e.call_c("talk_text_step")
	_check(l.any(func(x: Array) -> bool: return x[0] == "cd_audio_request_set" and x[1] == [600, 700, 4]) and l.any(func(x: Array) -> bool: return x[0] == "audio_command" and x[1] == [0x300]) and mm.read_global("speaker_voice_state") == 0, "PLAYING + finished, a differs from b and a == request[2]: request b, audio_command(0x300), PENDING again")
	mm.write_global("speaker_voice_state", 1)
	mm.write_u32(request + 8, 999)
	e.call_c("talk_text_step")
	_check(mm.read_global("speaker_voice_state") == 2, "PLAYING + finished, request no longer a: DONE")
	mm.write_global("speaker_voice_state", 1)
	mm.write_global("speaker_voice_index", -1)
	e.call_c("talk_text_step")
	_check(mm.read_global("speaker_voice_state") == 2, "PLAYING + finished with no index: DONE")
	# DONE and text complete: marker, then a confirm press advances
	mm.write_global("speaker_voice_state", 2)
	e.gs_set("substep", 5)
	e.gs_set("pad_pressed", 0x20)
	l.clear()
	e.call_c("talk_text_step")
	_check(e.gs_get("substep") == 6 and e.gs_get("text_scroll_state") == 0 and l.any(func(x: Array) -> bool: return x[0] == "hud_text_marker_set" and x[1] == [0]), "voice 1, DONE, text complete, confirm: marker off, substep + 1")
	mm.write_global("speaker_voice_state", 0)
	e.gs_set("substep", 5)
	e.gs_set("voice_mode", 1)
	e.call_c("talk_text_step") # PENDING: the typewriter is gated, text not complete -> confirm only jumps to the line end
	_check(e.gs_get("substep") == 5, "voice 1, PENDING, confirm: substep unchanged")
	# CROSS with voice 2 stops the voice
	e.gs_set("voice_mode", 2)
	mm.write_global("speaker_voice_state", 2)
	e.gs_set("pad_pressed", 0x40)
	l.clear()
	e.call_c("talk_text_step")
	_check(l.any(func(x: Array) -> bool: return x[0] == "audio_command" and x[1] == [0x74]) and l.any(func(x: Array) -> bool: return x[0] == "cd_audio_command" and x[1] == [9, 0, 0]) and mm.read_global("speaker_voice_state") == 2, "CROSS, text complete, voice 2: audio_command(0x74), cd_audio_command(9, 0, 0), state DONE")

func _test_refusals_and_layout() -> void:
	var engine = _new()
	engine.call_c("x_taku_string_set", [1, 2, 3, 3])
	_check(engine.refusal.is_empty(), "x_taku_string_set: set_kanji_string is ported (T-39311), the call now runs without a refusal: %s" % engine.refusal)
	var e2 = _new()
	e2.call_c("sndi", [0x80150000, 0, 0xFF])
	_check(e2.refusal.is_empty(), "sndi: set_kanji_string is ported (T-39311), the call now runs without a refusal: %s" % e2.refusal)
	var e3 = _new()
	e3.call_c("get_near_menu", [2, 1, 0])
	_check(e3.refusal.contains("axis_distances is uninitialised"), "get_near_menu on an axis other than 0/1 refuses: %s" % e3.refusal)
	var e4 = _new()
	e4.call_c("menu_cursor_update", [0]) # menu_mode 0 calls the ported hud_cursor_update: no refusal
	_check(not e4.halted(), "menu_cursor_update in mode 0 runs on ported code")
	var e5 = _new()
	e5.gs_set("menu_mode", 3)
	e5.gs_set("pad_pressed", 0x1000)
	e5.memory.write_u8(0x800E6280 + 0x1090, 2)
	e5.call_c("menu_cursor_update", [0])
	_check(e5.refusal.contains("abs") or not e5.halted(), "menu_mode 3 reaches get_near_menu (abs is an SDK call and refuses when needed)")
	var c: Dictionary = engine.modules["menu_speaker_girl"].get_script().get_script_constant_map()
	_check(c["MENU_ITEMS"] == engine.gs_address("menu_items") and c["MENU_COUNT"] == engine.gs_address("menu_count_0") and c["MENU_MODE"] == engine.gs_address("menu_mode") and c["MENU_CURSOR_0"] == engine.gs_address("menu_cursor_0") and c["MENU_HIT"] == engine.gs_address("menu_hit") and c["GAME_STATE"] == engine.gs_address("video_width"), "menu constants equal the layout")
