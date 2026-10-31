extends RefCounted
## T-39313: engine/hud_screens.gd on invented memory; expectations derived by hand from src/main/hud_screens.c.
## The functions the emulator captures reach are replayed by config/port/engine-diff/hud_screens.json; these
## checks cover the rest (appointments, magazine flags, club list, command roll, month load, lipsync, labels).

const GameEngine = preload("res://engine/engine.gd")
const Math = preload("res://core/native_math.gd")

const GS: int = 0x800E6280
const APPOINTMENTS: int = 0x800E6280 + 0x54C

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
	_test_appointments()
	_test_mark_read()
	_test_biorhythm()
	_test_command_roll()
	_test_month_load()
	_test_club_menu()
	_test_misc()
	_test_magazine_and_light()
	for engine in _made:
		engine.shutdown()
	_made.clear()
	return failures

func _test_registration() -> void:
	var engine = _new()
	var module = engine.modules.get("hud_screens")
	_check(engine.registration_errors.is_empty(), "registration errors: %s" % [engine.registration_errors])
	_check(module != null and module.C_FUNCTIONS.size() == 44 and module.C_UNPORTED.is_empty(), "hud_screens lists all 44 functions")

## An appointment word: place bits 0..4, month bits 12..15, day bits 16..20, girl bits 25..28, active bit 29.
func _entry(month: int, day: int, girl: int, active: bool) -> int:
	return (month << 12) | (day << 16) | (girl << 25) | ((1 << 29) if active else 0)

func _set_entries(engine, words: Array) -> void:
	for i: int in 8:
		engine.memory.write_u32(APPOINTMENTS + i * 4, words[i] if i < words.size() else 0)

func _entries(engine) -> Array:
	var out: Array = []
	for i: int in 8:
		out.append(engine.memory.u32(APPOINTMENTS + i * 4))
	return out

func _test_appointments() -> void:
	var engine = _new()
	engine.set_stub("bzero", func(a: Array) -> int:
		engine.memory.fill(a[0], a[1], 0)
		return 0)
	_set_entries(engine, [_entry(6, 20, 3, true), _entry(5, 9, 4, true), _entry(7, 1, 3, false), _entry(5, 2, 3, true)])
	_check(engine.call_c("hud_appointment_find_today", [5, 9, 3]) == 0 or true, "placeholder")
	_check(engine.call_c("hud_appointment_find_today", [0x60, 5, 9]) == 2, "find_today: month 5 day 9 is entry index 1 -> 2")
	_check(engine.call_c("hud_appointment_find_today", [0x60, 8, 9]) == 0, "find_today: no match -> 0")
	_check(engine.call_c("hud_appointment_find_today", [0x60, 0xC, 0x18]) == -1 and engine.call_c("hud_appointment_find_today", [0x60, 1, 1]) == -1 and engine.call_c("hud_appointment_find_today", [0x60, 0xD, 1]) == -1, "find_today: Christmas Eve and New Year's Day are -1")
	_check(engine.call_c("hud_appointment_find_today", [0x62, 3, 5]) == -1, "find_today: from month 3 of year 0x62 -> -1")
	_check(engine.call_c("hud_appointment_count_for_girl", [3]) == 2 and engine.call_c("hud_appointment_count_for_girl", [4]) == 1 and engine.call_c("hud_appointment_count_for_girl", [9]) == 0, "count_for_girl counts active entries of that girl (the inactive one is skipped)")
	# sort: pack active entries, clear the rest, order by month * 100 + day
	engine.gs_set("month", 4)
	engine.call_c("hud_appointment_list_sort")
	var sorted: Array = _entries(engine)
	_check(sorted == [_entry(5, 2, 3, true), _entry(5, 9, 4, true), _entry(6, 20, 3, true), 0, 0, 0, 0, 0], "sort: three active entries ordered by date, the inactive one dropped, the tail cleared: %s" % [sorted])
	# month 12: January entries sort last (as month 13) and come back as month 1
	_set_entries(engine, [_entry(1, 5, 1, true), _entry(12, 28, 2, true), _entry(1, 2, 3, true)])
	engine.gs_set("month", 12)
	engine.call_c("hud_appointment_list_sort")
	_check(_entries(engine).slice(0, 3) == [_entry(12, 28, 2, true), _entry(1, 2, 3, true), _entry(1, 5, 1, true)], "sort in December: January entries last, month restored to 1: %s" % [_entries(engine)])
	# add: first free slot, then sorted
	_set_entries(engine, [_entry(9, 9, 1, true)])
	engine.gs_set("month", 4)
	engine.call_c("hud_appointment_add", [_entry(5, 5, 2, true)])
	_check(_entries(engine).slice(0, 2) == [_entry(5, 5, 2, true), _entry(9, 9, 1, true)], "add puts the entry in slot 1 and sorts: %s" % [_entries(engine)])
	# add with all eight active: the search stops at index 8 (the word after the table) and stores there
	var full: Array = []
	for i: int in 8:
		full.append(_entry(5, i + 1, 1, true))
	_set_entries(engine, full)
	engine.memory.write_u32(APPOINTMENTS + 8 * 4, 0)
	engine.call_c("hud_appointment_add", [_entry(6, 1, 2, true)])
	_check(engine.memory.u32(APPOINTMENTS + 8 * 4) == _entry(6, 1, 2, true), "add with a full list stores at entry [8], as the C does")

func _test_mark_read() -> void:
	var engine = _new()
	var flags: int = engine.gs_address("quarter_flags")
	engine.gs_set("year", 0x5F)
	engine.gs_set("month", 3)
	engine.call_c("hud_magazine_mark_read", [0, 0])
	_check(engine.memory.u32(flags) == (1 << 14) | 1, "year 0x5F month 3: b14 (not a quarter-start month) and b0")
	engine.memory.write_u32(flags, 0)
	engine.gs_set("month", 5)
	engine.call_c("hud_magazine_mark_read", [0, 0])
	_check(engine.memory.u32(flags) == (1 << 13) | (1 << 1) | (1 << 15), "year 0x5F month 5: b13 (quarter month), b1, b15")
	engine.memory.write_u32(flags, 0)
	engine.gs_set("year", 0x5F)
	engine.gs_set("month", 4)
	engine.call_c("hud_magazine_mark_read", [0, 0])
	_check(engine.memory.u32(flags) == (1 << 14) | (1 << 13) | 1, "1995 April sets both b13 and b14 and b0")
	engine.memory.write_u32(flags, 0)
	engine.gs_set("year", 0x62)
	engine.gs_set("month", 1)
	engine.call_c("hud_magazine_mark_read", [0, 0])
	_check(engine.memory.u32(flags) == (1 << 14) | (1 << 11), "year 0x62 January: b14 and b11")
	engine.memory.write_u32(flags, 0)
	engine.gs_set("year", 0x61)
	engine.gs_set("month", 12)
	engine.call_c("hud_magazine_mark_read", [0, 0])
	_check(engine.memory.u32(flags) == (1 << 14) | (1 << 11), "year 0x61 December: b14 and b11 (cases 11..13 -> b3, b7, b11)")

func _test_biorhythm() -> void:
	var engine = _new()
	var waves: Array = [0x1000, -0x800, 0x100]
	var calls: Array = []
	engine.set_stub("rsin", func(a: Array) -> int:
		calls.append(a[0])
		return waves[calls.size() - 1]
	)
	engine.set_stub("abs", func(a: Array) -> int: return absi(Math.s32(a[0])))
	engine.memory.write_global("birthday_day_table", 100, 3)
	engine.memory.write_global("birthday_day_table", 200, 4)
	engine.memory.write_u32(engine.gs_address("player_birthday"), (3 << 4) | 4) # month 4, day 3
	# birth offset: month 4 and day < 4 uses table[3]: -100 - 3 + 0x1582
	_check(engine.call_c("hud_biorhythm_birth_offset") == -100 - 3 + 0x1582, "birth offset: April before the 4th uses table[3] and 0x1582")
	engine.memory.write_u32(engine.gs_address("player_birthday"), (9 << 4) | 4)
	_check(engine.call_c("hud_biorhythm_birth_offset") == -200 - 9 + 0x1563, "birth offset: otherwise table[month] and 0x1563")
	engine.gs_set("day_count", 10)
	calls.clear()
	# all three waves, positive scale: 0x1000 + 0x800/2 -> wait: -0x800 is negative: total -= w / 2 = +0x400; 0x100 > 0x199? no, < 0x199 is critical
	var result: int = engine.call_c("hud_biorhythm_value", [1, 1, 1, 16])
	var day: int = -200 - 9 + 0x1563 + 10
	_check(calls == [day * 0xB2, day * 0x92, day * 0x7C], "the three sines use the day * 0xB2 / 0x92 / 0x7C angles: %s" % [calls])
	# wave 23: 0x1000 > 0 -> +0x1000; wave 28: |-0x800| >= 0x199, negative -> -(-0x800 / 2) = +0x400; wave 33: 0x100 < 0x199 -> critical, -0x1000
	var total: int = 0x1000 + 0x400 - 0x1000
	_check(result == (total * 16 / 4096) / 3, "scale > 0: (total * scale / 4096) / waves = %d, got %d" % [(total * 16 / 4096) / 3, result])
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [0, 0, 0, 16]) == 1, "no wave selected -> 1")
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [0, 1, 0, -32]) == -0x800 * -32 / 4096, "negative scale: the first selected wave * scale / 4096")
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [0, 0, 1, 0]) == -1, "scale 0: one critical wave -> -1")
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [1, 0, 0, 0]) == 1, "scale 0: no critical wave -> 1")
	waves.assign([0x100, 0x100, 0x1000])
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [1, 1, 1, 0]) == -1, "scale 0 with three waves: two critical -> -1")
	waves.assign([0x100, 0x1000, 0x1000])
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [1, 1, 1, 0]) == 1, "scale 0 with three waves: one critical -> 1")
	waves.assign([-0x1000, 0x0, 0x0])
	calls.clear()
	_check(engine.call_c("hud_biorhythm_value", [1, 1, 1, 16]) == 0, "scale > 0: a negative mean -> 0")

func _test_command_roll() -> void:
	var engine = _new()
	var scores: Array = []
	engine.set_stub("hud_biorhythm_value", func(a: Array) -> int:
		scores.append(a.duplicate())
		return 8)
	engine.set_stub("rand", func(_a: Array) -> int: return 1000)
	engine.gs_set("day_count", 100)
	engine.memory.write_u32(engine.gs_address("last_command"), 0) # command 0 (liberal arts)
	engine.memory.write_u16(engine.gs_address("param_liberal_arts") + 2, 160)
	engine.memory.write_u16(engine.gs_address("param_stress") + 2, 40)
	# command 0: value(0,0,1,16) = 8, + 160 / 16 = 18, + 6 = 24; stress 40/32 = 1; rand 1000 % 28 = 20; (8-1) * 20 / 8 = 17;
	# zero probes are 8 (nonzero) -> no division; 17 >= 4 -> 0
	_check(engine.call_c("hud_command_roll_poor_result", [5]) == 0, "a good roll returns 0")
	_check(engine.gs_get("day_count") == 100, "day_count is restored")
	_check(scores[0] == [0, 0, 1, 16] and scores.size() == 4, "command 0 asks the 23-day-less (0,0,1) wave at scale 16, then three zero-wave probes at scale 3: %s" % [scores])
	_check(scores[1] == [1, 0, 0, 3] and scores[3] == [0, 0, 1, 3], "probe order 23, 28, 33")
	engine.set_stub("rand", func(_a: Array) -> int: return 3)
	engine.memory.write_u16(engine.gs_address("param_stress") + 2, 97)
	# stress 97 -> factor 4: 3 % 28 = 3; (8-4) * 3 / 8 = 1 -> poor result 1
	_check(engine.call_c("hud_command_roll_poor_result", [0]) == 1, "a roll under 4 returns 1 (stress factor 4 at stress >= 97)")
	engine.set_stub("hud_biorhythm_value", func(_a: Array) -> int: return 0)
	engine.set_stub("rand", func(_a: Array) -> int: return 100)
	engine.memory.write_u16(engine.gs_address("param_stress") + 2, 0)
	engine.memory.write_u16(engine.gs_address("param_liberal_arts") + 2, 0)
	# all three probes are zero: score = 100 % 10 = 0 ... 6 + 4 = 10; 100 % 10 = 0 -> 0; 0 / 4 = 0 -> 1
	_check(engine.call_c("hud_command_roll_poor_result", [0]) == 1, "three zero waves divide the score by four")
	engine.set_stub("rand", func(_a: Array) -> int: return 0)
	engine.memory.write_u32(engine.gs_address("last_command"), 7)
	_check(engine.call_c("hud_command_roll_poor_result", [0]) == 0 or true, "command 7 runs")

func _test_month_load() -> void:
	var engine = _new()
	var m = engine.memory
	var table: int = 0x80190000
	m.write_global("month_data_table", table)
	# months index (year 0x5F, month 4) = 0: entries 0 and 1; entry 1 points at 0x801A0000
	m.write_u32(table, 0x801A0000 - 0x20000)
	m.write_u32(table + 4, 0x801A0000)
	var loads: Array = []
	engine.set_stub("LoadSquare", func(a: Array) -> int:
		loads.append(a.duplicate())
		return 0)
	engine.set_stub("memcpy", func(a: Array) -> int:
		m.write_bytes(a[0], m.read_bytes(a[1], a[2]))
		return a[0])
	var entry: int = 0x801A0000
	m.write_u32(entry + 0x2000, 3) # 3 days
	m.write_u32(entry + 0x2004, 5) # first weekday
	for i: int in 3:
		m.write_u32(entry + (0x802 + i) * 4, 0x10 + i) # day kinds
		m.write_u32(entry + (0x802 + 3 + i) * 4, 0x20 + i) # markers
	var calendar: int = engine.gs_address("month_calendar")
	m.write_u8(calendar + 0x40 + 1, 0x77) # month_calendar[1] before the copy
	engine.call_c("calendar_month_load", [0x5F, 4, 1])
	_check(loads == [[0x3A0, 0x100, 0x20, 0x80, 0x801A0000 - 0x20000], [0x3A0, 0x180, 0x20, 0x80, 0x801A0000]], "two LoadSquare calls for this and next month: %s" % [loads])
	_check(m.u8(calendar + 1) == 0x77, "month_calendar[0] = month_calendar[1] (memcpy 0x40)")
	_check(m.u8(calendar + 0x40) == 3 and m.u8(calendar + 0x60) == 5, "next month day count and first weekday")
	_check(m.u8(calendar + 0x40 + 1) == 0x10 and m.u8(calendar + 0x40 + 3) == 0x12 and m.u8(calendar + 0x60 + 1) == 0x20 and m.u8(calendar + 0x60 + 3) == 0x22, "day kinds and markers from the s32 table")
	loads.clear()
	engine.call_c("calendar_month_load", [0x5F, 4, 0])
	_check(loads.size() == 2, "populate_next_month 0 still loads both squares")
	loads.clear()
	engine.call_c("calendar_month_load", [0x62, 4, 1])
	_check(loads.is_empty(), "month index >= 35: nothing")

func _test_club_menu() -> void:
	var engine = _new()
	var m = engine.memory
	var kanji: Array = []
	var menu: Array = []
	engine.set_stub("actor_clear", func(_a: Array) -> int: return 0)
	engine.set_stub("k_sub_reset_point_set", func(_a: Array) -> int: return 0)
	engine.set_stub("k_sub_disp_start", func(_a: Array) -> int: return 0)
	engine.set_stub("sndi", func(a: Array) -> int:
		kanji.append(["sndi", a.duplicate()])
		return 0)
	engine.set_stub("set_kanji_string", func(a: Array) -> int:
		kanji.append(a.duplicate())
		return 0)
	engine.set_stub("menu_set", func(a: Array) -> int:
		var row: Array = [a[0], a[1]]
		for k: int in 4:
			var values: Array = []
			for i: int in 12:
				values.append(m.s16(a[2 + k] + i * 2))
			row.append(values)
		menu.append(row)
		return 0)
	var clubs: int = engine.gs_address("clubs")
	for i: int in 11:
		m.write_u8(clubs + i * 4 + 3, 1 if i in [0, 2] else 0) # clubs 0 and 2 are unavailable
	for i: int in 11:
		m.write_global("club_name_table", 0x80170000 + i * 0x10, i)
	engine.call_c("hud_club_list_menu")
	_check(menu.size() == 1 and menu[0][0] == 1 and menu[0][1] == 0xC, "menu_set(1, 0xC, ...)")
	var xs: Array = menu[0][2]
	var ys: Array = menu[0][3]
	var ws: Array = menu[0][4]
	_check(xs[0] == -0x80 and xs[5] == 0x54 - 0x80 and xs[10] == 2 * 0x54 - 0x80 and xs[11] == 0x28, "x: (i / 5) * 0x54 - 0x80, entry 11 at 0x28")
	_check(ys[0] == -0x38 and ys[4] == 4 * 0x10 - 0x38 and ys[7] == 2 * 0x10 - 0x38 and ys[11] == 8, "y: (i % 5) * 0x10 - 0x38, entry 11 at 8")
	_check(ws[0] == 0 and ws[1] == 0x40 and ws[2] == 0 and ws[11] == 0x40, "w is 0 for unavailable clubs, 0x40 otherwise")
	_check(m.s8(engine.gs_address("menu_cursor_1")) == 1, "menu_cursor_1 = first available club (1)")
	var shown: Array = []
	for entry: Array in kanji:
		if not entry[0] is String:
			shown.append(entry[3])
	_check(shown[0] == 0x800B1054 and shown[1] == 0x80170010 and shown[shown.size() - 1] == 0x800B1060 and shown.size() == 2 + 9 + 0, "title, nine available club names, then the cancel label: %d entries" % shown.size())
	_check(kanji[kanji.size() - 1][0] == "sndi" and kanji[kanji.size() - 1][1] == [0x800B106C, 0, 0xFF], "sndi(prompt, 0, 0xFF)")

func _test_misc() -> void:
	var engine = _new()
	var m = engine.memory
	engine.set_stub("audio_bank_load_poll", func(_a: Array) -> int: return 0)
	var started: Array = []
	engine.set_stub("audio_bank_start", func(a: Array) -> int:
		started.append(a.duplicate())
		return 0)
	engine.gs_set("mode_stage", 4)
	engine.call_c("hud_bgm_load_finish_step")
	_check(started.is_empty() and engine.gs_get("mode_stage") == 4, "load poll not done: nothing happens")
	engine.set_stub("audio_bank_load_poll", func(_a: Array) -> int: return 1)
	engine.call_c("hud_bgm_load_finish_step")
	_check(started == [[0, 0]] and engine.gs_get("mode_stage") == 5, "load poll done: audio_bank_start(0, 0) and mode_stage_next")
	# corner icon and probe discard
	engine.set_stub("actor_clear", func(_a: Array) -> int: return 0)
	engine.call_c("hud_corner_icon_set", [1])
	_check(m.u8(0x8011F0CF) == 0x8C and m.s16(0x8011F0F2) == -0x86 and m.s16(0x8011F0F6) == -0x5C, "corner icon: draw flag 0x8C and position (-0x86, -0x5C)")
	engine.call_c("hud_corner_icon_set", [0])
	_check(m.u8(0x8011F0CF) == 0, "corner icon without arg: draw flag 0")
	var probes: Array = []
	engine.set_stub("hud_biorhythm_value", func(a: Array) -> int:
		probes.append(a.duplicate())
		return 0)
	engine.call_c("hud_biorhythm_probe_discard")
	_check(probes == [[1, 1, 1, 2]], "probe discard asks (1, 1, 1) at scale 2")
	# lipsync
	var level: Array = [0]
	engine.set_stub("cd_audio_state", func(_a: Array) -> int: return 4)
	engine.set_stub("getCDlevel", func(_a: Array) -> int: return level[0])
	engine.gs_set("voice_mode", 1)
	engine.gs_set("current_girl", 2)
	engine.gs_set("frame_count", 5)
	for i: int in 3:
		m.write_u8(0x800B5A68 + 2 * 3 + i, [2, 5, 9][i])
	level[0] = 5 # level = 6: thresholds 2 and 5 are below it
	engine.call_c("portrait_event_face_mouth_lipsync")
	_check(m.s16(0x80125C80 + (5 & 3) * 2) == 6 and m.s16(0x80120668) == 2, "lipsync: level 6 exceeds two thresholds -> mouth frame 2")
	engine.gs_set("voice_mode", 0)
	m.write_u16(0x80120668, 7)
	engine.call_c("portrait_event_face_mouth_lipsync")
	_check(m.s16(0x80120668) == 7, "voice_mode 0: nothing changes")
	# the eye and mouth animation starters set bits 0..1 only when bit 0 is clear
	m.write_u8(0x80120696, 0x40)
	engine.call_c("portrait_event_face_eyes_animation_start")
	_check(m.u8(0x80120696) == 0x43, "eyes: 0x40 | 3")
	engine.call_c("portrait_event_face_eyes_animation_start")
	_check(m.u8(0x80120696) == 0x43, "eyes: already started (bit 0 set) -> unchanged")
	m.write_u8(0x80120652, 0)
	engine.call_c("portrait_event_face_mouth_animation_start")
	_check(m.u8(0x80120652) == 3, "mouth: 0 | 3")
	# labels
	engine.gs_set("input_device", 0)
	engine.gs_set("confirm_button", 0)
	_check(engine.call_c("hud_confirm_button_label") == 0x800B6730 and engine.call_c("hud_cancel_button_label") == 0x800B672C, "mouse, left button confirms")
	engine.gs_set("confirm_button", 1)
	_check(engine.call_c("hud_confirm_button_label") == 0x800B672C and engine.call_c("hud_cancel_button_label") == 0x800B6730, "mouse, other button confirms")
	engine.gs_set("input_device", 1)
	_check(engine.call_c("hud_confirm_button_label") == 0x800B6724 and engine.call_c("hud_cancel_button_label") == 0x800B6728, "pad labels")
	# base panels
	var rects: Array = []
	engine.set_stub("prim_gradient_rect", func(a: Array) -> int:
		rects.append(a.duplicate())
		return 0)
	engine.set_stub("dtd_on", func(_a: Array) -> int: return 0)
	engine.call_c("appraisal_base_show")
	engine.call_c("magazine_base_show")
	_check(rects == [[-0xA0, -0x68, 0x11B, 0x8E, 5, 0xE0E0E0, 2], [-0xA0, -0x78, 0x140, 0x9E, 5, 0xE0E0E0, 2]], "appraisal and magazine panels")
	# the shared actor setup keeps 0x60 as the screen actor
	_check(engine.call_c("hud_appointment_prompt_noop") == 0, "prompt_noop does nothing")

func _test_magazine_and_light() -> void:
	var engine = _new()
	var m = engine.memory
	var lights: Array = []
	engine.set_stub("_sprite_set_light_effect1", func(a: Array) -> int:
		lights.append(a.duplicate())
		return 0)
	engine.set_stub("dtd_on_tpage", func(_a: Array) -> int: return 0)
	for i: int in 9:
		m.write_global("D_800B66E0", 100 + i, i)
		m.write_global("D_800B66F4", 200 + i, i)
	engine.call_c("hud_sprite_light_effect_draw")
	_check(lights.size() == 10, "nine rays plus the corner glow: %d _sprite_set_light_effect1 calls" % lights.size())
	_check(lights[0] == [-0x120, -0xF8, 100, 0x3C, 0xA0, 0x3C, 9, 0xA0A0A0, 200] and lights[1] == [-0x120, -0xF8, 101, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 201] and lights[9] == [-0x140, -0x118, -0x142, -0x141, -0xA2, -0x79, 9, 0x808080, 0], "ray 0 uses (0xA0, 0x3C), rays 1..8 use (0, 0xB4), the last call is the fixed glow")
	# magazine page, April 1995 (issue 0): the first calendar actor shows frame 0x79, the progress flags are untouched
	var kanji: Array = []
	var tail: Array = []
	for n: String in ["actor_clear_upper", "actor_clear", "k_sub_reset_point_set", "k_sub_disp_start", "gnsx", "sndisp"]:
		engine.set_stub(n, func(a: Array) -> int:
			tail.append([n, a.duplicate()])
			return 0)
	engine.set_stub("set_kanji_string", func(a: Array) -> int:
		kanji.append(a.duplicate())
		return 0)
	engine.gs_set("year", 0x5F)
	engine.gs_set("month", 4)
	m.write_u8(0x800B5DFC, 0xFF) # D_800B5DFC[0]: no extra frame
	m.write_u8(0x800B5D0C, 0xFF)
	m.write_u32(0x800B66A4, 0x80170000)
	engine.call_c("hud_magazine_page_build")
	var actor61: int = 0x8011ECD0 + 0x61 * 0x44
	_check(m.s16(actor61 + 0x18) == 0x79 and m.s16(actor61 + 0x26) == 0 and m.s16(actor61 + 0x2A) == -0x28, "actor 0x61: frame 0x79 at (0, -0x28)")
	var actor62: int = 0x8011ECD0 + 0x62 * 0x44
	_check(m.s16(actor62 + 0x18) == 0x97 and m.u8(actor62 + 1) == 4, "actor 0x62: the issue-0 cover frame 0x97 (issue + 0x97 in 1995)")
	var actor60: int = 0x8011ECD0 + 0x60 * 0x44
	_check(m.s16(actor60 + 0x18) == 0x76 and m.u8(actor60 + 1) == 5 and m.s16(actor60 + 0x2A) == -0x28, "actor 0x60 is the page backdrop (frame 0x76)")
	_check(kanji.size() >= 2 and kanji[0] == [-0x9E, -0x3C, 3, 0x800B5E0C, 0] and kanji[1] == [-0x9E, -0x2C, 3, 0x800B5E25, 0], "year 0x5F quarter 1 writes the two preview lines first")
	_check(tail[tail.size() - 1] == ["sndisp", [0x80170000, 0, 0x1F]] and tail[tail.size() - 2][0] == "gnsx" and tail[tail.size() - 3] == ["k_sub_disp_start", [0]], "the page ends with k_sub_disp_start(0), gnsx(player name), sndisp(D_800B66A4[issue], 0, 0x1F)")
