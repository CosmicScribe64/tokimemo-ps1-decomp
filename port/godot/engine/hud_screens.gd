extends RefCounted
## Port of src/main/hud_screens.c (original object 0x800674B0-0x8006CB30, T-39313 / T-39301 contract).
## Calendar and magazine screens, the save/load slot panel, appointment and club lists, the mascot, the cursor
## actor, biorhythm and command-roll helpers and the event-log scan. Actor records (0x44 bytes, include/game/
## display_records.h Actor) are written at actor_pool + 0x44 * index; bit-fields are little-endian LSB first
## (include/game/schedule_records.h: AppointmentBits active = bit 29, girl = bits 25..28; AppointmentDate
## month = bits 12..15, day = bits 16..20; BirthdayBits month = bits 0..3, day = bits 4..8).
## Return values the C leaves undefined (ret_undefined) are 0: an unobserved placeholder, every caller discards them.
## Contract: wiki/engine-porting-contract.md. Tests: tests/engine/hud_screens_test.gd and
## config/port/engine-diff/hud_screens.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/hud_screens.c"
const C_FUNCTIONS: Array[String] = [
	"hud_day_bgm_load", "hud_bgm_load_finish_step", "hud_mascot_display_switch", "hud_mascot_set",
	"hud_mascot_update", "portrait_event_face_mouth_lipsync", "portrait_event_face_eyes_animation_start",
	"portrait_event_face_mouth_animation_start", "calendar_screen_init", "calendar_marks_build",
	"calendar_cell_x", "calendar_cell_y", "calendar_month_load", "hud_biorhythm_graph_draw",
	"calendar_view_update", "cal_base_show", "appraisal_base_show", "magazine_base_show",
	"cal_sprite_disp_switch", "cal_sprite_init", "data_save_load_class_init", "hud_save_slot_info_show",
	"hud_magazine_mark_read", "hud_magazine_page_build", "hud_appointment_find_today",
	"hud_appointment_count_for_girl", "hud_appointment_list_sort", "hud_appointment_add",
	"hud_appointment_prompt_noop", "hud_club_list_menu", "hud_sprite_light_effect_draw", "hud_cursor_update",
	"hud_cursor_actor_init", "hud_cursor_show", "hud_command_roll_poor_result", "hud_biorhythm_probe_discard",
	"hud_biorhythm_value", "hud_biorhythm_birth_offset", "hud_corner_icon_set", "hud_text_marker_set",
	"hud_text_marker_bob", "hud_event_log_scan", "hud_confirm_button_label", "hud_cancel_button_label",
]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"hud_day_bgm_load": {"args": [], "ret": "void"},
	"hud_bgm_load_finish_step": {"args": [], "ret": "void"},
	"hud_mascot_display_switch": {"args": ["s32"], "ret": "void"},
	"hud_mascot_set": {"args": ["s32"], "ret": "void"},
	"hud_mascot_update": {"args": [], "ret": "void"},
	"portrait_event_face_mouth_lipsync": {"args": [], "ret": "void"},
	"portrait_event_face_eyes_animation_start": {"args": [], "ret": "s32", "ret_undefined": "s32 in the C with no return statement"},
	"portrait_event_face_mouth_animation_start": {"args": [], "ret": "s32", "ret_undefined": "s32 in the C with no return statement"},
	"calendar_screen_init": {"args": [], "ret": "void"},
	"calendar_marks_build": {"args": [], "ret": "void"},
	"calendar_cell_x": {"args": ["s32", "s32"], "ret": "s16"},
	"calendar_cell_y": {"args": ["s32", "s32"], "ret": "s16"},
	"calendar_month_load": {"args": ["u8", "u8", "s32"], "ret": "void"},
	"hud_biorhythm_graph_draw": {"args": [], "ret": "void"},
	"calendar_view_update": {"args": [], "ret": "void"},
	"cal_base_show": {"args": [], "ret": "void"},
	"appraisal_base_show": {"args": [], "ret": "void"},
	"magazine_base_show": {"args": [], "ret": "void"},
	"cal_sprite_disp_switch": {"args": ["s32"], "ret": "void"},
	"cal_sprite_init": {"args": [], "ret": "void"},
	"data_save_load_class_init": {"args": [], "ret": "void"},
	"hud_save_slot_info_show": {"args": ["s32", "s32"], "ret": "void"},
	"hud_magazine_mark_read": {"args": ["s32", "s32"], "ret": "s32", "ret_undefined": "implicit-int K&R definition with no return statement"},
	"hud_magazine_page_build": {"args": [], "ret": "void"},
	"hud_appointment_find_today": {"args": ["s32", "s32", "s32"], "ret": "s32"},
	"hud_appointment_count_for_girl": {"args": ["s32"], "ret": "s32"},
	"hud_appointment_list_sort": {"args": [], "ret": "void"},
	"hud_appointment_add": {"args": ["u32"], "ret": "void"},
	"hud_appointment_prompt_noop": {"args": [], "ret": "void"},
	"hud_club_list_menu": {"args": [], "ret": "void"},
	"hud_sprite_light_effect_draw": {"args": [], "ret": "void"},
	"hud_cursor_update": {"args": [], "ret": "void"},
	"hud_cursor_actor_init": {"args": ["s32"], "ret": "void"},
	"hud_cursor_show": {"args": ["s32"], "ret": "void"},
	"hud_command_roll_poor_result": {"args": ["s32"], "ret": "s32"},
	"hud_biorhythm_probe_discard": {"args": [], "ret": "void"},
	"hud_biorhythm_value": {"args": ["s32", "s32", "s32", "s32"], "ret": "s32"},
	"hud_biorhythm_birth_offset": {"args": [], "ret": "s32"},
	"hud_corner_icon_set": {"args": ["s32"], "ret": "void"},
	"hud_text_marker_set": {"args": ["s32"], "ret": "void"},
	"hud_text_marker_bob": {"args": [], "ret": "s16", "ret_undefined": "s16 in the C with no return statement"},
	"hud_event_log_scan": {"args": ["ptr", "ptr", "s32"], "ret": "s32"},
	"hud_confirm_button_label": {"args": [], "ret": "ptr"},
	"hud_cancel_button_label": {"args": [], "ret": "ptr"},
}

const ACTOR_POOL: int = 0x8011ECD0 # actor_pool
const ACTOR_SIZE: int = 0x44
const GAME_STATE: int = 0x800E6280
const SPRITE_QUADS: int = 0x801217D0 # sprite_quads (SpriteQuad, 0x24 bytes)
const SYSTEM_DATA: int = 0x800E7D10
## strncat count observed in a2 at the two-argument club-name call (captures wpc-sn-save, wpc-sn-load).
const CLUB_NAME_STRNCAT_COUNT: int = 8
const BLOCK: int = 0x80123120 # memory_card_block
const PAD_R1: int = 0x0008
const PAD_R2: int = 0x0002
const PAD_SQUARE: int = 0x0080
const SCALE_SCORE: int = 16 # include/main/hud.h HUD_BIORHYTHM_SCORE_SCALE
const SCALE_ZERO_PROBE: int = 3 # HUD_BIORHYTHM_ZERO_WAVE_PROBE_SCALE
const SCALE_DISCARDED: int = 2 # HUD_BIORHYTHM_DISCARDED_PROBE_SCALE
const SCALE_GRAPH: int = -32 # HUD_BIORHYTHM_GRAPH_SCALE

# Shift-JIS literals of the object (config/objects/main.txt rodata 0x800B0F90-0x800B1140; bytes in the executable).
const S_BAN_ALBUM: int = 0x800B0F90 # "番アルバム" (system arm)
const S_SYSTEM: int = 0x800B0F9C # "システム"
const S_UNUSED: int = 0x800B0FA8 # "未使用"
const S_DATA_ERROR: int = 0x800B0FB0 # "データ異常"
const S_YEAR: int = 0x800B0FBC # "年"
const S_MONTH: int = 0x800B0FC0 # "月"
const S_DAY: int = 0x800B0FC4 # "日"
const S_NAME: int = 0x800B0FC8 # "名前…"
const S_NICKNAME: int = 0x800B0FD0 # "あだ名…"
const S_CLUB: int = 0x800B0FDC # "所属部‥"
const S_CLEARED: int = 0x800B0FE8 # "クリア人数‥"
const S2_UNUSED: int = 0x800B0FF8 # the card-image arm repeats the literals
const S2_DATA_ERROR: int = 0x800B1000
const S2_BAN_ALBUM: int = 0x800B100C
const S2_YEAR: int = 0x800B1018
const S2_MONTH: int = 0x800B101C
const S2_DAY: int = 0x800B1020
const S2_NAME: int = 0x800B1024
const S2_NICKNAME: int = 0x800B102C
const S2_CLUB: int = 0x800B1038
const S2_CLEARED: int = 0x800B1044
const S_CLUB_LIST_TITLE: int = 0x800B1054 # "部活一覧"
const S_CANCEL: int = 0x800B1060 # "取り消し"
const S_CLUB_PROMPT: int = 0x800B106C # "入部する部活を選択してください"

var engine # GameEngine; set when the engine registers the module

# ---- helpers

func _a(index: int) -> int:
	return ACTOR_POOL + index * ACTOR_SIZE

## Actor field setters (offsets from include/game/display_records.h).
func _a8(index: int, offset: int, value: int) -> void:
	engine.memory.write_u8(_a(index) + offset, value)

func _a16(index: int, offset: int, value: int) -> void:
	engine.memory.write_u16(_a(index) + offset, value)

func _a32(index: int, offset: int, value: int) -> void:
	engine.memory.write_u32(_a(index) + offset, value)

## An unported callee's s32 result: call_c leaves unported (stubbed or SDK) results unnarrowed.
func _cs32(function_name: String, args: Array) -> int:
	return Math.s32(engine.call_c(function_name, args))

func _g(name: String, index: int = 0) -> int:
	return engine.memory.read_global(name, index)

func _gaddr(name: String) -> int:
	return engine.memory.global_address(name)

func _set_g(name: String, value: int, index: int = 0) -> void:
	engine.memory.write_global(name, value, index)

## Division and modulo by a zero divisor are refusals, never results (contract).
func _div(a: int, b: int) -> int:
	if b == 0:
		engine.refuse("hud_screens", "division by zero")
		return 0
	return a / b

func _mod(a: int, b: int) -> int:
	if b == 0:
		engine.refuse("hud_screens", "modulo by zero")
		return 0
	return a % b

func _gs(field: String) -> int:
	return engine.gs_address(field)

## The sprite/actor defaults most UI actors copy: frame table, sequence table, texture blocks, D_800C9A54.
func _actor_tables(index: int) -> void:
	_a32(index, 0xC, _gaddr("sprite_frame_table"))
	_a32(index, 0x10, _gaddr("actor_default_sequence_table"))
	_a32(index, 0x34, _gaddr("actor_default_texture_blocks"))
	_a16(index, 0x14, _g("D_800C9A54"))

# ---- BGM and mascot

func hud_day_bgm_load() -> void:
	engine.call_c("audio_command", [5])
	engine.call_c("audio_command", [0x71])
	var vacation_type: int = engine.gs_get("vacation")
	if engine.gs_get("year") == 0x60 and engine.gs_get("month") == 9 and engine.gs_get("day") == 1:
		vacation_type = 1
	match vacation_type:
		0:
			var season: int = _cs32("hud_season_index", []) & 3
			engine.call_c("audio_bank_load_start", [0, 0xBF98, 0xBF79, _g("D_800B5BE8", season), _g("D_800B5BF8", season), _g("D_800B5BD8", season)])
		1:
			engine.call_c("audio_bank_load_start", [0, 0xBF98, 0xBF79, 0xD7D1, 0xD7AF, 0xD7A4])
		2:
			engine.call_c("audio_bank_load_start", [0, 0xBF98, 0xBF79, 0xD73F, 0xD71E, 0xD717])
		3:
			engine.call_c("audio_bank_load_start", [0, 0xBF98, 0xBF79, 0xD780, 0xD752, 0xD743])

func hud_bgm_load_finish_step() -> void:
	if engine.call_c("audio_bank_load_poll") == 1:
		engine.call_c("audio_bank_start", [0, 0])
		engine.call_c("mode_stage_next")

## D_8011F443 is element 1 (stride 0x44) of the table at D_8011F3FF: bit 7 of the flags byte of two actors.
func hud_mascot_display_switch(visible: int) -> void:
	var base: int = _gaddr("D_8011F3FF")
	var m = engine.memory
	if visible != 0:
		m.write_u8(base, m.u8(base) | 0x80)
		m.write_u8(base + 0x44, m.u8(base + 0x44) | 0x80)
	else:
		m.write_u8(base, m.u8(base) & 0x7F)
		m.write_u8(base + 0x44, m.u8(base + 0x44) & 0x7F)

func hud_mascot_set(mascot_selection: int) -> void:
	var sel: int = Math.s32(mascot_selection)
	if sel == 0 or sel >= 0xE:
		_set_g("mascot_anchor_x", 0x78)
		_set_g("mascot_anchor_y", -0x37)
		_set_g("mascot_length", 0xF)
		_set_g("mascot_swing", 8)
		_set_g("hud_mascot_choice", 0)
		engine.call_c("hud_mascot_display_switch", [0])
		return
	_set_g("hud_mascot_choice", sel)
	engine.call_c("actor_clear", [0x1B])
	_mascot_actor(0x1B, _g("hud_mascot_choice") - 1)
	engine.call_c("actor_clear", [0x1C])
	_mascot_actor(0x1C, 0xD)

## The shared stores of the two mascot actors (0x1B, 0x1C) after actor_clear; `frame` goes to unk_18 last.
func _mascot_actor(index: int, frame: int) -> void:
	_a8(index, 1, 1)
	_a8(index, 2, 0x40)
	_a32(index, 0x38, 0x40000000)
	_a8(index, 3, 0x84)
	_a32(index, 0xC, _gaddr("D_800C5FD4"))
	_a32(index, 0x10, _gaddr("D_800C6010"))
	_a32(index, 0x34, _gaddr("D_800C5FCC"))
	_a16(index, 0x14, _g("D_800C6014"))
	_a8(index, 4, 8)
	_a8(index, 5, 1)
	_a16(index, 0x16, 0)
	_a16(index, 0x18, frame)

func hud_mascot_update() -> void:
	var m = engine.memory
	var choice: int = _g("hud_mascot_choice")
	if choice == 0 or choice >= 0xE:
		engine.call_c("hud_mascot_set", [0])
		return
	engine.call_c("hud_mascot_set", [choice])
	if _g("hud_mascot_choice") >= 0xE:
		return
	if (Math.u32(engine.gs_get("pad_pressed")) >> 16) != 0:
		_set_g("mascot_swing", (_g("mascot_length") + 1) / 2)
	if (engine.gs_get("frame_count") & 0x3F) == 0x3F and _g("mascot_swing") != 0:
		_set_g("mascot_swing", _g("mascot_swing") - 1)
	var wave: int = _cs32("rsin", [(engine.gs_get("frame_count") & 0x7F) << 5])
	var dx: int = Math.s16((Math.s32(wave * _g("mascot_swing"))) >> 12)
	_set_g("D_8011F41C", ((-dx * 2) / 3) << 12)
	var length: int = _g("mascot_length")
	var root: int = _cs32("SquareRoot0", [Math.s32(length * length - dx * dx)])
	var dy: int = Math.s16(0xF - root)
	var x1: int = _g("mascot_anchor_x") + dx
	_a16(0x1B, 0x26, x1)
	var y1: int = _g("mascot_length") + _g("mascot_anchor_y")
	_a16(0x1B, 0x2A, y1 - dy)
	_a16(0x1B, 0x18, _g("hud_mascot_choice") - 1)
	_a16(0x1C, 0x26, _g("mascot_anchor_x"))
	_a16(0x1C, 0x2A, _g("mascot_anchor_y"))
	var held: int = engine.gs_get("pad_held")
	if held & PAD_R2:
		_a16(0x1C, 0x18, 0xE)
		match held & 0x5000:
			0x1000:
				if _g("mascot_length") >= 3:
					_set_g("mascot_length", _g("mascot_length") - 2)
					_set_g("mascot_swing", (_g("mascot_length") + 1) / 2)
					y1 = _g("mascot_anchor_y") + _g("mascot_length")
			0x4000:
				if _g("mascot_length") < 0xC4:
					_set_g("mascot_length", _g("mascot_length") + 2)
					_set_g("mascot_swing", (_g("mascot_length") + 1) / 2)
					y1 = _g("mascot_anchor_y") + _g("mascot_length")
	elif held & PAD_R1:
		_a16(0x1C, 0x18, 0xE)
		match held & 0x5000:
			0x1000:
				_set_g("mascot_anchor_y", _g("mascot_anchor_y") - 4)
				y1 = _g("mascot_anchor_y") + _g("mascot_length")
			0x4000:
				_set_g("mascot_anchor_y", _g("mascot_anchor_y") + 4)
				y1 = _g("mascot_anchor_y") + _g("mascot_length")
		match held & 0xA000:
			0x8000:
				_set_g("mascot_anchor_x", _g("mascot_anchor_x") - 4)
				x1 -= 4
			0x2000:
				_set_g("mascot_anchor_x", _g("mascot_anchor_x") + 4)
				x1 += 4
	else:
		_a16(0x1C, 0x18, 0xD)
	engine.call_c("prim_gradient_line", [_g("mascot_anchor_x"), _g("mascot_anchor_y"), Math.s16(x1), Math.s16(y1 - dy), 0xF8B0C8, 0xF8B0C8, 1])
	if _g("mascot_anchor_x") >= 0x201:
		_set_g("mascot_anchor_x", -0x200)
	if _g("mascot_anchor_x") < -0x200:
		_set_g("mascot_anchor_x", 0x200)
	if _g("mascot_anchor_y") >= 0x201:
		_set_g("mascot_anchor_y", -0x200)
	if _g("mascot_anchor_y") < -0x200:
		_set_g("mascot_anchor_y", 0x200)

# ---- portrait face animation

## The two `max` loops of the C are dead (max is never read), so they are not ported.
func portrait_event_face_mouth_lipsync() -> void:
	if engine.gs_get("voice_mode") == 0:
		return
	var level: int = 0
	if engine.call_c("cd_audio_state") & 4:
		level = _cs32("getCDlevel", []) + 1
	_set_g("D_80125C80", level, engine.gs_get("frame_count") & 3)
	var n: int = 0
	var girl_id: int = engine.gs_get("current_girl")
	for i: int in 3:
		if engine.memory.u8(_gaddr("D_800B5A68") + girl_id * 3 + i) < level:
			n = i + 1
	if n >= 4:
		n = 3
	if _g("D_80120668") == 0 or (engine.gs_get("frame_count") & 1) == 0:
		if level == 0 or n == 0:
			_set_g("D_80120668", 0)
		elif n == 1:
			_set_g("D_80120668", 1)
		elif n == 2:
			_set_g("D_80120668", 2)
		else:
			_set_g("D_80120668", 3)

func portrait_event_face_eyes_animation_start() -> int:
	var value: int = Math.u8(_g("D_80120696"))
	if (value & 1) == 0:
		value |= 3
		_set_g("D_80120696", value)
	return 0

func portrait_event_face_mouth_animation_start() -> int:
	var value: int = Math.u8(_g("D_80120652"))
	if (value & 1) == 0:
		value |= 3
		_set_g("D_80120652", value)
	return 0

# ---- calendar

## Actor 0x60 as the calendar / save-load / club screens set it up (the three share these stores).
func _screen_actor(kind: int, frame: int) -> void:
	_a8(0x60, 1, kind)
	_a8(0x60, 2, 0x40)
	_a32(0x60, 0x38, 0x40000000)
	_a8(0x60, 3, 0x84)
	_actor_tables(0x60)
	_a8(0x60, 4, 8)
	_a8(0x60, 5, 1)
	_a16(0x60, 0x18, frame)
	_a16(0x60, 0x26, -0x10)
	_a16(0x60, 0x2A, -0x13)

func calendar_screen_init() -> void:
	engine.call_c("actor_clear", [0x60])
	_screen_actor(4, 0x43)
	engine.call_c("calendar_marks_build")

func data_save_load_class_init() -> void:
	engine.call_c("actor_clear", [0x60])
	_screen_actor(3, 0x43)

## Calendar marks live in actor records 0x61.. (at most 0x3F): see the C comment. `n` counts marks; the
## `repeated_calendar_frame` local is only read after a repeat count was set in the same call.
func calendar_marks_build() -> void:
	var m = engine.memory
	for i: int in 0x20:
		engine.call_c("actor_clear", [i + 0x61])
		var idx: int = i + 0x61
		_a8(idx, 1, 3)
		_a8(idx, 2, 0x40)
		_a32(idx, 0x38, 0)
		_a8(idx, 3, 0x8C)
		_actor_tables(idx)
		_a8(idx, 4, 8)
		_a8(idx, 5, 1)
		_a8(idx, 6, 8)
		_a16(idx, 0x18, 0x62)
		_a16(idx, 0x26, 0x100)
		_a16(idx, 0x2A, 0x100)
		_a8(idx, 7, 0x80)
	var state: Dictionary = {"n": 0}
	var repeat_days: int = 0
	var repeated_frame: int = 0
	var month: int = engine.gs_get("month")
	var birthday: int = m.u32(_gs("player_birthday"))
	var cal_month: int = birthday & 0xF
	var cal_day: int = (birthday >> 4) & 0x1F
	if cal_month == month:
		_cal_mark(state, 0x6E, 0, cal_day - 1)
	if cal_month == engine.gs_get("month") + 1:
		_cal_mark(state, 0x6E, 1, cal_day - 1)
	var girls: int = _gs("girls")
	for i: int in 11:
		var bday: int = m.u32(girls + i * 0x38 + 0x10)
		cal_month = bday & 0xF
		cal_day = (bday >> 4) & 0x1F
		if (m.u32(girls + i * 0x38 + 0xC) >> 6) & 1:
			if cal_month == engine.gs_get("month"):
				_cal_mark(state, 0x6D, 0, cal_day - 1)
			if cal_month == engine.gs_get("month") + 1 or engine.gs_get("month") + 1 == cal_month + 12:
				_cal_mark(state, 0x6D, 1, cal_day - 1)
	var appointments: int = GAME_STATE + 0x54C
	for i: int in 8:
		var word: int = m.u32(appointments + i * 4)
		cal_month = (word & 0xFFFF) >> 12
		cal_day = (word >> 16) & 0x1F
		if (word >> 29) & 1:
			if cal_month == engine.gs_get("month"):
				_cal_mark(state, 0x62, 0, cal_day - 1)
			if cal_month == engine.gs_get("month") % 12 + 1:
				_cal_mark(state, 0x62, 1, cal_day - 1)
	var club: int = m.u16(_gs("player_flags")) >> 12
	var calendar: int = _gs("month_calendar")
	if club >= 6 and club < 11:
		var third_current: int = 0x15 - m.u8(calendar + 0x20)
		var third_next: int = 0x15 - m.u8(calendar + 0x40 + 0x20)
		_cal_mark(state, 0x6F, 0, third_current - 1)
		if engine.gs_get("year") != 0x62 or engine.gs_get("month") != 2:
			_cal_mark(state, 0x6F, 1, third_next - 1)
	# The two month columns: side 0 repeats for events of 6/7 days, the 0x6F festival marks depend on the month.
	for side: int in 2:
		var column: int = calendar + side * 0x40
		var festival_month: int = 8 if side == 0 else 7
		var skip_month: int = 2 if side == 0 else 1
		repeat_days = 0
		var i: int = 0
		while i < m.u8(column):
			var marker: int = m.u8(column + 0x20 + i + 1)
			var kind: int = m.u8(_gaddr("D_800B5C10") + marker)
			if marker != 0 and kind != 0xFF:
				if kind == 0x6F:
					if engine.gs_get("month") == festival_month and (m.u16(_gs("player_flags")) >> 12) < 11:
						_cal_mark(state, _day_kind(column, i), side, i)
				else:
					_cal_mark(state, _day_kind(column, i), side, i)
				kind = _day_kind(column, i)
				if kind == 0x66 and engine.gs_get("month") != skip_month:
					repeated_frame = kind
					repeat_days = 6
				elif kind == 0x6C:
					repeated_frame = kind
					repeat_days = 6
				elif kind == 0x67:
					repeated_frame = kind
					repeat_days = 7
				elif kind == 0x6F and engine.gs_get("month") == festival_month and (m.u16(_gs("player_flags")) >> 12) < 11:
					repeated_frame = kind
					repeat_days = 6
			elif repeat_days > 0:
				_cal_mark(state, repeated_frame, side, i)
			repeat_days -= 1
			if state.n + 0x61 >= 0xA0:
				return
			i += 1
	var log: int = _gs("event_log")
	for i: int in 0x100:
		var entry: int = log + i * 8
		var year: int = m.u8(entry + 4)
		var log_month: int = m.u8(entry + 5)
		var day: int = m.u8(entry + 6)
		if year == engine.gs_get("year") and log_month == engine.gs_get("month"):
			if m.u8(entry) == 0xC:
				if m.u8(SYSTEM_DATA + 0x14 + 12) != 0 or m.u8(_gs("girl_cleared") + 12) != 0:
					_cal_mark(state, 0x62, 0, day - 1)
			else:
				_cal_mark(state, 0x62, 0, day - 1)
		if state.n + 0x61 >= 0xA0:
			return
		if m.u8(entry + 4) == 0:
			return

## DAY_KIND(side): D_800B5C10[marker[i + 1]], read again at every use as in the C.
func _day_kind(column: int, i: int) -> int:
	return engine.memory.u8(_gaddr("D_800B5C10") + engine.memory.u8(column + 0x20 + i + 1))

## CAL_MARK: frame in unk_18, then the cell position (calendar_cell_x, calendar_cell_y), then n++.
func _cal_mark(state: Dictionary, frame: int, side: int, day_index: int) -> void:
	var idx: int = state.n + 0x61
	var d: int = Math.s32(day_index)
	_a16(idx, 0x18, frame)
	_a16(idx, 0x26, engine.call_c("calendar_cell_x", [side, d]))
	_a16(idx, 0x2A, engine.call_c("calendar_cell_y", [side, d]))
	state.n += 1

func calendar_cell_x(month_side: int, day_index: int) -> int:
	var side: int = Math.s32(month_side)
	var v: int = engine.memory.u8(_gs("month_calendar") + 0x20 + side * 0x40)
	if v == 6:
		v -= 7
	return Math.s16(side * 0x88 + ((v + Math.s32(day_index) + 1) % 7) * 16 - 0x78)

func calendar_cell_y(month_side: int, day_index: int) -> int:
	var side: int = Math.s32(month_side)
	var v: int = engine.memory.u8(_gs("month_calendar") + 0x20 + side * 0x40)
	if v == 6:
		v -= 7
	return Math.s16(((v + Math.s32(day_index) + 1) / 7) * 16 - 0x30)

## month_data_table is a `s32 **` holding the table base; entries are s32 pointers, indexed as s32 elements.
func calendar_month_load(calendar_year: int, calendar_month: int, populate_next_month: int) -> void:
	var m = engine.memory
	var index: int = (Math.u8(calendar_year) - 95) * 12 + Math.u8(calendar_month) - 4
	if index >= 35:
		return
	var table: int = Math.u32(_g("month_data_table"))
	engine.call_c("LoadSquare", [0x3A0, 0x100, 0x20, 0x80, m.u32(table + index * 4)])
	engine.call_c("LoadSquare", [0x3A0, 0x180, 0x20, 0x80, m.u32(table + (index + 1) * 4)])
	if populate_next_month == 0:
		return
	var calendar: int = _gs("month_calendar")
	engine.call_c("memcpy", [calendar, calendar + 0x40, 0x40])
	var entry: int = m.u32(Math.u32(_g("month_data_table")) + (index + 1) * 4)
	var q: int = entry + 0x800 * 4
	var days: int = m.s32(q)
	var first_weekday: int = m.s32(q + 4)
	m.write_u8(calendar + 0x40, days) # month_calendar[1].day_kind[0]
	m.write_u8(calendar + 0x40 + 0x20, first_weekday) # marker[0]
	var i: int = 0
	while i < days:
		var base: int = Math.u32(_g("month_data_table"))
		var data: int = m.u32(base + (index + 1) * 4)
		m.write_u8(calendar + 0x40 + i + 1, m.s32(data + (0x802 + i) * 4))
		base = Math.u32(_g("month_data_table"))
		data = m.u32(base + (index + 1) * 4)
		m.write_u8(calendar + 0x40 + 0x20 + i + 1, m.s32(data + (0x802 + days + i) * 4))
		i += 1

func hud_biorhythm_graph_draw() -> void:
	var phase: int = engine.gs_get("frame_count")
	var half: int
	if phase & 0x80:
		half = (0x7F - (phase & 0x7F)) / 2
	else:
		half = (phase & 0x7F) / 2
	engine.call_c("prim_gradient_line", [-0x80, -0xC, 0x7C, -0xC, half * 0x10101, half * 0x10101, 0x81])
	var saved: int = Math.u16(engine.gs_get("day_count"))
	engine.gs_set("day_count", saved - 8)
	var v23: Array = []
	var v28: Array = []
	var v33: Array = []
	for i: int in 0x10:
		v23.append(engine.call_c("hud_biorhythm_value", [1, 0, 0, SCALE_GRAPH]))
		v28.append(engine.call_c("hud_biorhythm_value", [0, 1, 0, SCALE_GRAPH]))
		v33.append(engine.call_c("hud_biorhythm_value", [0, 0, 1, SCALE_GRAPH]))
		engine.gs_set("day_count", engine.gs_get("day_count") + 1)
		if i != 0:
			var shade: int = ((i + phase) & 0xF) * 4 + half
			var x0: int = i * 0x10 - 0x90
			var x1: int = i * 0x10 - 0x80
			engine.call_c("prim_gradient_line", [x0, v23[i - 1] - 0xC, x1, v23[i] - 0xC, shade * 0x101, shade * 0x101, 0x81])
			engine.call_c("prim_gradient_line", [x0, v28[i - 1] - 0xC, x1, v28[i] - 0xC, shade * 0x10100 + 0xF0F00, shade * 0x10100 + 0xF0F00, 0x81])
			engine.call_c("prim_gradient_line", [x0, v33[i - 1] - 0xC, x1, v33[i] - 0xC, shade * 0x10001, shade * 0x10001, 0x81])
	engine.gs_set("day_count", saved)

func calendar_view_update() -> void:
	engine.call_c("hud_biorhythm_graph_draw")
	engine.call_c("dtd_on_tpage", [0, 0, 1, 2, 0])
	engine.call_c("cal_base_show")
	var t: int = engine.memory.s16(_a(0x61) + 8)
	_a16(0x61, 8, t + 1)
	for i: int in range(0x61, 0xA0):
		var scale: int = 0
		match (t >> 6) & 3:
			0:
				scale = (t & 0x3F) << 6
			1:
				scale = 0x1000
			2:
				scale = (0x3F - (t & 0x3F)) << 6
			3:
				scale = 0
		_a16(i, 0x1A, scale)
		_a16(i, 0x1C, scale)

func cal_base_show() -> void:
	engine.call_c("prim_gradient_rect", [-0xA0, -0x57, 0x11B, 0x7D, 5, 0xE0E0E0, 2])
	engine.call_c("dtd_on", [5])

func appraisal_base_show() -> void:
	engine.call_c("prim_gradient_rect", [-0xA0, -0x68, 0x11B, 0x8E, 5, 0xE0E0E0, 2])
	engine.call_c("dtd_on", [5])

func magazine_base_show() -> void:
	engine.call_c("prim_gradient_rect", [-0xA0, -0x78, 0x140, 0x9E, 5, 0xE0E0E0, 2])
	engine.call_c("dtd_on", [5])

func cal_sprite_disp_switch(arg0: int) -> void:
	var m = engine.memory
	for k: int in 4:
		var flags: int = SPRITE_QUADS + k * 0x24
		if Math.s32(arg0) == 1:
			m.write_u32(flags, m.u32(flags) & 0x7FFFFFFF)
		else:
			m.write_u32(flags, m.u32(flags) | 0x80000000)

## One sprite quad as cal_sprite_init sets it (SpriteQuad, include/game/draw_records.h).
func _quad_init(k: int, x: int, y: int, w: int, h: int, u0e: int, u0f: int, u10: int) -> void:
	var m = engine.memory
	var q: int = SPRITE_QUADS + k * 0x24
	m.write_u32(q, 0)
	m.write_u16(q + 4, x)
	m.write_u16(q + 6, y)
	m.write_u16(q + 8, w)
	m.write_u16(q + 0xA, h)
	m.write_u16(q + 0xC, 0x1E)
	m.write_u8(q + 0xE, u0e)
	m.write_u8(q + 0xF, u0f)
	m.write_u16(q + 0x10, u10)
	m.write_u16(q + 0x12, 0x1E0)
	m.write_u8(q + 0x14, 0x80)
	m.write_u8(q + 0x15, 0x80)
	m.write_u8(q + 0x16, 0x80)
	m.write_u16(q + 0x18, 0)
	m.write_u16(q + 0x1A, 0)
	m.write_u16(q + 0x1C, 0x1000)
	m.write_u16(q + 0x1E, 0x1000)
	m.write_u32(q + 0x20, 0)
	m.write_u8(_gs("quad_layer") + k, 0x84)

func cal_sprite_init() -> void:
	_quad_init(2, -0x88, -0x58, 0x7E, 0x7E, 0x80, 0, 0xC0)
	_quad_init(3, 0, -0x58, 0x7F, 0x7F, 0x80, 0x80, 0xC0)
	_quad_init(0, -0x58, -0x58, 0x18, 0x10, 0xB0, 0, 0xB0)
	_quad_init(1, 0x30, -0x58, 0x18, 0x10, 0xB0, 0x80, 0xB0)

# ---- save slot panel

## Frame 0xF8: text buffers slot_date sp+0xC8, player_name sp+0xA8, nickname sp+0x88, club sp+0x68,
## clear_count sp+0x48. The C calls strncat(club_name_text, club_name) with two arguments, so the count is
## whatever a2 held (left by the preceding strcpy). Captured with strncat as a listed callee (wpc-sn-save,
## wpc-sn-load; OpenBIOS strcpy): a2 was 8 at both club-name calls. The longest club name (table
## club_name_table[0..11]) is 8 bytes, so a count of 8 copies every name whole; with the retail BIOS the
## leftover is not observed. See the ticket.
func hud_save_slot_info_show(arg0: int, left_summary_column: int) -> void:
	var m = engine.memory
	var base: int = engine.frame_base()
	var date_text: int = base + 0xC8
	var name_text: int = base + 0xA8
	var nick_text: int = base + 0x88
	var club_text: int = base + 0x68
	var clear_text: int = base + 0x48
	var summary_x: int = -0x8C if Math.s32(left_summary_column) != 0 else -8
	engine.call_c("k_sub_reset")
	var digits: int = _gaddr("D_800B3288")
	if Math.u8(_g("slot_info_from_system_data")) != 0:
		var cleared: int = 0
		for i: int in 13:
			if m.u8(SYSTEM_DATA + 0x14 + i) != 0:
				cleared += 1
		var sel: int = m.u8(0x800E7D36) # D_800E7D36 == system_data.selected_slot
		engine.call_c("strcpy", [date_text, m.u32(digits + sel * 4)])
		engine.call_c("strncat", [date_text, S_BAN_ALBUM, 2])
		engine.call_c("set_kanji_string", [Math.s16(summary_x + 0xC), -0x50, 0xF, date_text, 0])
		if m.u8(SYSTEM_DATA + 0x26) == 0:
			engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, S_SYSTEM, 0])
		else:
			var status: int = m.u8(SYSTEM_DATA + 4 + m.u8(0x800E7D36))
			if status == 0:
				engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, S_UNUSED, 0])
			elif status == 0xFF:
				engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, S_DATA_ERROR, 0])
			else:
				sel = m.u8(0x800E7D36)
				var year: int = m.u8(SYSTEM_DATA + 0x858 + sel)
				var month: int = m.u8(SYSTEM_DATA + 0x868 + sel)
				var day: int = m.u8(SYSTEM_DATA + 0x878 + sel)
				_date_text(date_text, year, month, day, [S_YEAR, S_MONTH, S_DAY])
				engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, date_text, 0])
				engine.call_c("strcpy", [name_text, S_NAME])
				engine.call_c("strncat", [name_text, SYSTEM_DATA + 0x888 + sel * 8, 6])
				engine.call_c("strncat", [name_text, SYSTEM_DATA + 0x908 + sel * 8, 6])
				engine.call_c("strcpy", [nick_text, S_NICKNAME])
				engine.call_c("strncat", [nick_text, SYSTEM_DATA + 0x988 + sel * 16, 0xD])
				engine.call_c("set_kanji_string", [summary_x, -0x30, 0xF, name_text, 0])
				engine.call_c("set_kanji_string", [summary_x, -0x20, 0xF, nick_text, 0])
				engine.call_c("strcpy", [club_text, S_CLUB])
				engine.call_c("strncat", [club_text, m.u32(_gaddr("club_name_table") + m.u8(SYSTEM_DATA + 0xEC8 + sel) * 4), CLUB_NAME_STRNCAT_COUNT])
				engine.call_c("set_kanji_string", [summary_x, -0x10, 0xF, club_text, 0])
				_clear_count_text(clear_text, cleared, S_CLEARED)
				engine.call_c("set_kanji_string", [summary_x, 0, 0xF, clear_text, 0])
	else:
		var record: int = BLOCK + engine.gs_get("album_slot") * 0xF00
		var cleared: int = 0
		for i: int in 13:
			if m.u8(record + 0x688 + i) != 0:
				cleared += 1
		var save_slot: int = engine.gs_get("save_slot")
		var status: int = m.u8(SYSTEM_DATA + 4 + save_slot)
		if status == 0:
			engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, S2_UNUSED, 0])
		elif status == 0xFF:
			engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, S2_DATA_ERROR, 0])
		else:
			engine.call_c("strcpy", [date_text, m.u32(digits + engine.gs_get("save_slot") * 4)])
			engine.call_c("strncat", [date_text, S2_BAN_ALBUM, 2])
			engine.call_c("set_kanji_string", [Math.s16(summary_x + 0xC), -0x50, 0xF, date_text, 0])
			_date_text(date_text, m.u8(record + 0x205), m.u8(record + 0x206), m.u8(record + 0x207), [S2_YEAR, S2_MONTH, S2_DAY])
			engine.call_c("set_kanji_string", [summary_x, -0x40, 0xF, date_text, 0])
			engine.call_c("strcpy", [name_text, S2_NAME])
			engine.call_c("strncat", [name_text, record + 0x29C, 6])
			engine.call_c("strncat", [name_text, record + 0x2A4, 6])
			engine.call_c("strcpy", [nick_text, S2_NICKNAME])
			engine.call_c("strncat", [nick_text, record + 0x2AC, 0xD])
			engine.call_c("set_kanji_string", [summary_x, -0x30, 0xF, name_text, 0])
			engine.call_c("set_kanji_string", [summary_x, -0x20, 0xF, nick_text, 0])
			engine.call_c("strcpy", [club_text, S2_CLUB])
			engine.call_c("strncat", [club_text, m.u32(_gaddr("club_name_table") + (m.u16(record + 0x2BC) >> 12) * 4), CLUB_NAME_STRNCAT_COUNT])
			engine.call_c("set_kanji_string", [summary_x, -0x10, 0xF, club_text, 0])
			_clear_count_text(clear_text, cleared, S2_CLEARED)
			engine.call_c("set_kanji_string", [summary_x, 0, 0xF, clear_text, 0])
	engine.call_c("k_sub_disp_start", [6])

## The date line "<yy>年<m>月<d>日" (D_800B3288 holds the glyph string of each digit).
func _date_text(buffer: int, year: int, month: int, day: int, suffixes: Array) -> void:
	var digits: int = _gaddr("D_800B3288")
	var m = engine.memory
	engine.call_c("strcpy", [buffer, m.u32(digits + (year / 10) * 4)])
	engine.call_c("strncat", [buffer, m.u32(digits + (year % 10) * 4), 2])
	engine.call_c("strncat", [buffer, suffixes[0], 2])
	engine.call_c("strncat", [buffer, m.u32(digits + month * 4), 4])
	engine.call_c("strncat", [buffer, suffixes[1], 2])
	if day >= 10:
		engine.call_c("strncat", [buffer, m.u32(digits + (day / 10) * 4), 2])
	engine.call_c("strncat", [buffer, m.u32(digits + (day % 10) * 4), 2])
	engine.call_c("strncat", [buffer, suffixes[2], 2])

func _clear_count_text(buffer: int, cleared: int, label: int) -> void:
	var digits: int = _gaddr("D_800B3288")
	var m = engine.memory
	engine.call_c("strcpy", [buffer, label])
	if cleared >= 0x64:
		cleared = 0x63
	if cleared >= 0xA:
		engine.call_c("strncat", [buffer, m.u32(digits + (cleared / 10) * 4), 2])
	engine.call_c("strncat", [buffer, m.u32(digits + (cleared % 10) * 4), 2])

# ---- magazine

func _qf_set(bit: int) -> void:
	var address: int = _gs("quarter_flags")
	engine.memory.write_u32(address, engine.memory.u32(address) | (1 << bit))

## Marks the quarter's magazine as read (QuarterFlagBits b0..b15 of the word at quarter_flags).
func hud_magazine_mark_read(_arg0: int, _arg1: int) -> int:
	var month: int = engine.gs_get("month")
	var year: int = engine.gs_get("year")
	if month != 2 and month != 5 and month != 8 and month != 11:
		_qf_set(14)
	else:
		_qf_set(13)
	if year == 0x5F and month == 4:
		_qf_set(13)
		_qf_set(14)
	match month:
		2, 3, 4:
			_qf_year(year, 0, 4, 8)
		5, 6, 7:
			_qf_year(year, 1, 5, 9)
			_qf_set(15)
		8, 9, 10:
			_qf_year(year, 2, 6, 10)
		11, 12, 13:
			_qf_year(year, 3, 7, 11)
		1:
			if year == 0x60:
				_qf_set(3)
			if year == 0x61:
				_qf_set(7)
			if year == 0x62:
				_qf_set(11)
	return 0

func _qf_year(year: int, first: int, second: int, third: int) -> void:
	if year == 0x5F:
		_qf_set(first)
	if year == 0x60:
		_qf_set(second)
	if year == 0x61:
		_qf_set(third)

## One calendar actor record of the magazine page: frame (unk_18), x, y.
func _mag(index: int, frame: int, x: int, y: int) -> void:
	_a16(index, 0x18, frame)
	_a16(index, 0x26, x)
	_a16(index, 0x2A, y)

func hud_magazine_page_build() -> void:
	var m = engine.memory
	engine.call_c("actor_clear_upper")
	engine.call_c("actor_clear", [0x60])
	_a8(0x60 + 0, 1, 5)
	_a8(0x60, 2, 0x40)
	_a32(0x60, 0x38, 0x40000000)
	_a8(0x60, 3, 0x84)
	_actor_tables(0x60)
	_a8(0x60, 4, 8)
	_a8(0x60, 5, 1)
	_a16(0x60, 0x18, 0x76)
	_a16(0x60, 0x26, 0)
	_a16(0x60, 0x2A, -0x28)
	for i: int in 0x20:
		var idx: int = i + 0x61
		engine.call_c("actor_clear", [idx])
		_a8(idx, 1, 4)
		_a8(idx, 2, 0x40)
		_a32(idx, 0x38, 0)
		_a8(idx, 3, 0x84)
		_actor_tables(idx)
		_a8(idx, 4, 8)
		_a8(idx, 5, 1)
		_a8(idx, 6, 8)
		_a16(idx, 0x18, 0)
		_a16(idx, 0x26, 0x100)
		_a16(idx, 0x2A, 0x100)
		_a8(idx, 7, 0x80)
	var cursor: int = 0x61 # calendar_actor_index
	var issue: int = -1 # magazine_issue_index: uninitialised in the C unless a year/month arm below sets it (-1 = unset)
	var year: int = engine.gs_get("year")
	var month: int = engine.gs_get("month")
	var progress: int = _gs("progress_state")
	match month:
		2, 3, 4:
			if year == 0x5F:
				_mag(0x61, 0x79, 0, -0x28)
				issue = 0
				cursor += 1
				engine.call_c("set_kanji_string", [-0x9E, -0x3C, 3, 0x800B5E0C, 0])
				engine.call_c("set_kanji_string", [-0x9E, -0x2C, 3, 0x800B5E25, 0])
			if year == 0x60:
				issue = 4
				if engine.gs_get("month") != 2:
					m.write_u8(progress + 13, 1)
				_mag(cursor, 0x7A, 0, -0x28)
				cursor += 1
				_mag(cursor, 0x81, 8, -0x28)
				cursor += 1
			if year == 0x61:
				issue = 8
				if engine.gs_get("month") != 2:
					m.write_u8(progress + 11, 1)
				_mag(cursor, 0x7A, 0, -0x28)
				cursor += 1
				_mag(cursor, 0x84, 8, -0x28)
				cursor += 1
			if year == 0x62:
				issue = 0xC
				engine.call_c("set_kanji_string", [-0x9E, -0x3C, 3, 0x800B5E70, 0])
				engine.call_c("set_kanji_string", [-0x9E, -0x2C, 3, 0x800B5E89, 0])
		5, 6, 7:
			_mag(0x61, 0x7B, 0, -0x28)
			cursor += 1
			if year == 0x5F:
				issue = 1
				if engine.gs_get("month") != 5:
					m.write_u8(progress + 1, 1)
				_mag(cursor, 0x7E, 8, -0x28)
				cursor += 1
			if year == 0x60:
				issue = 5
				if engine.gs_get("month") != 5:
					m.write_u8(progress + 5, 1)
				_mag(cursor, 0x82, 8, -0x28)
				cursor += 1
			if year == 0x61:
				issue = 9
				engine.call_c("set_kanji_string", [-0x9E, -0x3C, 3, 0x800B5E3E, 0])
				engine.call_c("set_kanji_string", [-0x9E, -0x2C, 3, 0x800B5E57, 0])
		8, 9, 10:
			_mag(0x61, 0x7C, 0, -0x28)
			cursor += 1
			if year == 0x5F:
				issue = 2
				if engine.gs_get("month") != 8:
					m.write_u8(progress + 7, 1)
				_mag(cursor, 0x7F, 8, -0x28)
				cursor += 1
			if year == 0x60:
				issue = 6
				if engine.gs_get("month") != 8:
					m.write_u8(progress + 10, 1)
				_mag(cursor, 0x83, 8, -0x28)
				cursor += 1
			if year == 0x61:
				issue = 0xA
				engine.call_c("set_kanji_string", [-0x9E, -0x3C, 3, 0x800B5E3E, 0])
				engine.call_c("set_kanji_string", [-0x9E, -0x2C, 3, 0x800B5E57, 0])
		11, 12:
			_mag(0x61, 0x7D, 0, -0x28)
			cursor += 1
			if year == 0x5F:
				issue = 3
				if engine.gs_get("month") == 0xC:
					m.write_u8(progress + 3, 1)
				_mag(cursor, 0x80, 8, -0x28)
				cursor += 1
			if year == 0x60:
				issue = 7
				if engine.gs_get("month") == 0xC:
					m.write_u8(progress + 6, 1)
				_mag(cursor, 0x85, 8, -0x28)
				cursor += 1
			if year == 0x61:
				issue = 0xB
				engine.call_c("set_kanji_string", [-0x9E, -0x3C, 3, 0x800B5E70, 0])
				engine.call_c("set_kanji_string", [-0x9E, -0x2C, 3, 0x800B5E89, 0])
		1:
			_mag(0x61, 0x7D, 0, -0x28)
			cursor += 1
			if year == 0x60:
				issue = 3
				m.write_u8(progress + 3, 1)
				_mag(cursor, 0x80, 8, -0x28)
				cursor += 1
			if year == 0x61:
				issue = 7
				m.write_u8(progress + 6, 1)
				_mag(cursor, 0x85, 8, -0x28)
				cursor += 1
			if year == 0x62:
				issue = 0xB
				engine.call_c("set_kanji_string", [-0x9E, -0x3C, 3, 0x800B5E70, 0])
				engine.call_c("set_kanji_string", [-0x9E, -0x2C, 3, 0x800B5E89, 0])
	if engine.gs_get("year") != 0x5F or engine.gs_get("month") >= 6:
		_mag(cursor, 0x77, 0, -0x28)
		cursor += 1
	if issue == -1:
		# First use of magazine_issue_index in the C reads an uninitialised stack word: no value is invented.
		engine.refuse("hud_magazine_page_build", "magazine_issue_index is uninitialised for this year/month")
		return
	if engine.gs_get("year") == 0x5F:
		_mag(cursor, issue + 0x97, 0, -0x28)
		cursor += 1
	elif engine.gs_get("year") == 0x60 and engine.gs_get("month") < 2:
		_mag(cursor, issue + 0x97, 0, -0x28)
		cursor += 1
	else:
		_a8(cursor, 1, m.u8(_a(cursor) + 1) + 1)
		_mag(cursor, 0x78, 0, -0x28)
		cursor += 1
		var picks: int = _gaddr("D_800B3750") + issue * 3 # D_800B3750[issue][3]
		_mag(cursor, m.u8(picks) + 0x90, 0x110, 0x18)
		cursor += 1
		_mag(cursor, m.u8(picks + 1) + 0x90, 0x110, 0x28)
		cursor += 1
		_mag(cursor, m.u8(picks + 2) + 0x90, 0x110, 0x38)
		cursor += 1
		var labels: int = m.u32(_gaddr("D_800B32CC"))
		engine.call_c("set_kanji_string", [0x100, 0x10, 6, m.u32(labels + ((issue * 3 + 2) % 12) * 4), 0])
		engine.call_c("set_kanji_string", [0x100, 0x20, 6, m.u32(labels + ((issue * 3 + 3) % 12) * 4), 0])
		engine.call_c("set_kanji_string", [0x100, 0x30, 6, m.u32(labels + ((issue * 3 + 4) % 12) * 4), 0])
	if m.u8(_gaddr("D_800B5DFC") + issue) != 0xFF:
		_mag(cursor, m.u8(_gaddr("D_800B5DFC") + issue), 0, -0x28)
		cursor += 1
	engine.call_c("set_kanji_string", [0x20, -0x5E, 7, _gaddr("D_800B5D1C") + issue * 17, 0])
	if m.u8(_gaddr("D_800B5D0C") + issue) != 0xFF:
		_mag(cursor, m.u8(_gaddr("D_800B5D0C") + issue), 0, -0x28)
		cursor += 1
	engine.call_c("set_kanji_string", [0x20, -0x3E, 8, _gaddr("D_800B5C2C") + issue * 17, 0])
	if issue < 4:
		_mag(cursor, (issue & 3) + 0x97, 0, -0x28)
		cursor += 1
	if issue >= 4:
		_mag(cursor, (issue & 3) + 0x93, 0, -0x28)
		cursor += 1
	var lines: int = _gaddr("D_800B5EA4") + issue * 108 # D_800B5EA4[issue][3][36]
	engine.call_c("set_kanji_string", [-0x8F, -0xE, 6, lines, 0])
	engine.call_c("set_kanji_string", [-0x8F, 2, 6, lines + 36, 0])
	engine.call_c("set_kanji_string", [-0x8F, 0x12, 6, lines + 72, 0])
	engine.call_c("k_sub_reset_point_set")
	engine.call_c("k_sub_disp_start", [0])
	engine.call_c("gnsx", [_gs("player_family_name")])
	engine.call_c("sndisp", [m.u32(_gaddr("D_800B66A4") + issue * 4), 0, 0x1F])

# ---- appointments

func _appointment_word(i: int) -> int:
	return engine.memory.u32(GAME_STATE + 0x54C + i * 4)

func hud_appointment_find_today(calendar_year: int, calendar_month: int, day_of_month: int) -> int:
	var year: int = Math.s32(calendar_year)
	var month: int = Math.s32(calendar_month)
	var day: int = Math.s32(day_of_month)
	var r: int = 0
	if month == 0xD:
		month = 1
	if month == 0xC and day == 0x18:
		return -1
	if month == 0xD:
		if day == 1:
			return -1
	if month == 1 and day == 1:
		return -1
	if month >= 3 and year >= 0x62:
		return -1
	for i: int in 8:
		var word: int = _appointment_word(i)
		if month == ((word & 0xFFFF) >> 12) and day == ((word >> 16) & 0x1F):
			r = i + 1
	return r

func hud_appointment_count_for_girl(girl_id: int) -> int:
	var count: int = 0
	for i: int in 8:
		var word: int = _appointment_word(i)
		if ((word >> 29) & 1) != 0 and Math.s32(girl_id) == ((word >> 25) & 0xF):
			count += 1
	return count

## Packs the active entries to the front, clears the rest (bzero), sorts the first n by month * 100 + day
## (selection sort; while the game month is 12, January entries sort as month 13).
func hud_appointment_list_sort() -> void:
	var m = engine.memory
	var table: int = GAME_STATE + 0x54C
	var active_count: int = 0
	for j: int in 8:
		var word: int = m.u32(table + j * 4)
		if (word >> 29) & 1:
			m.write_u32(table + active_count * 4, word)
			active_count += 1
	for j: int in range(active_count, 8):
		engine.call_c("bzero", [table + j * 4, 4])
	if engine.gs_get("month") == 12:
		for j: int in active_count:
			var word2: int = m.u32(table + j * 4)
			if ((word2 & 0xFFFF) >> 12) == 1:
				m.write_u32(table + j * 4, (word2 & ~0xF000) | (13 << 12))
	var i: int = 0
	while i < active_count:
		var first: int = m.u32(table + i * 4)
		var earliest_key: int = ((first >> 16) & 0x1F) + ((first & 0xFFFF) >> 12) * 100
		var earliest: int = i
		for j: int in range(i + 1, active_count):
			var other: int = m.u32(table + j * 4)
			var key: int = ((other >> 16) & 0x1F) + ((other & 0xFFFF) >> 12) * 100
			if key < earliest_key:
				earliest_key = key
				earliest = j
		var temp: int = m.u32(table + i * 4)
		m.write_u32(table + i * 4, m.u32(table + earliest * 4))
		m.write_u32(table + earliest * 4, temp)
		i += 1
	for j: int in active_count:
		var word3: int = m.u32(table + j * 4)
		if ((word3 & 0xFFFF) >> 12) == 13:
			m.write_u32(table + j * 4, (word3 & ~0xF000) | (1 << 12))

## The C reads entry [8] (the first word of scene_flags) in the search loop and may store there when all eight
## are active: ported as written.
func hud_appointment_add(appointment_entry: int) -> void:
	var m = engine.memory
	var table: int = GAME_STATE + 0x54C
	var slot: int = 0
	if (m.u32(table) >> 29) & 1:
		while true:
			slot += 1
			if not (((m.u32(table + slot * 4) >> 29) & 1) != 0 and slot != 8):
				break
	m.write_u32(table + slot * 4, Math.u32(appointment_entry))
	engine.call_c("hud_appointment_list_sort")

func hud_appointment_prompt_noop() -> void:
	pass

## Frame 0xE0: s16 arrays x (sp+0xB8), y (sp+0xA0), w (sp+0x88), h (sp+0x70), twelve entries each.
func hud_club_list_menu() -> void:
	var m = engine.memory
	var base: int = engine.frame_base()
	var xs: int = base + 0xB8
	var ys: int = base + 0xA0
	var ws: int = base + 0x88
	var hs: int = base + 0x70
	engine.call_c("actor_clear", [0x60])
	_screen_actor(4, 0x3F)
	engine.call_c("set_kanji_string", [-0x80, -0x52, 3, S_CLUB_LIST_TITLE, 0])
	var first_available: int = -1
	var clubs: int = _gs("clubs")
	for i: int in 0xB:
		m.write_u16(xs + i * 2, (i / 5) * 0x54 - 0x80)
		m.write_u16(ys + i * 2, (i % 5) * 0x10 - 0x38)
		m.write_u16(ws + i * 2, 0x40)
		m.write_u16(hs + i * 2, 0x10)
		if (m.u8(clubs + i * 4 + 3) & 1) == 0:
			engine.call_c("set_kanji_string", [m.s16(xs + i * 2), m.s16(ys + i * 2), 3, m.u32(_gaddr("club_name_table") + i * 4), 0])
			if first_available == -1:
				first_available = i
		else:
			m.write_u16(ws + i * 2, 0)
	m.write_u16(xs + 22, 0x28)
	m.write_u16(ys + 22, 8)
	m.write_u16(ws + 22, 0x40)
	m.write_u16(hs + 22, 0x10)
	engine.call_c("set_kanji_string", [m.s16(xs + 22), m.s16(ys + 22), 3, S_CANCEL, 0])
	engine.call_c("menu_set", [1, 0xC, xs, ys, ws, hs])
	engine.gs_set("menu_cursor_1", first_available)
	engine.call_c("k_sub_reset_point_set")
	engine.call_c("sndi", [S_CLUB_PROMPT, 0, 0xFF])
	engine.call_c("k_sub_disp_start", [1])

func hud_sprite_light_effect_draw() -> void:
	engine.call_c("_sprite_set_light_effect1", [-0x120, -0xF8, _g("D_800B66E0", 0), 0x3C, 0xA0, 0x3C, 9, 0xA0A0A0, _g("D_800B66F4", 0)])
	for i: int in range(1, 9):
		engine.call_c("_sprite_set_light_effect1", [-0x120, -0xF8, _g("D_800B66E0", i), 0x3C, 0, 0xB4, 9, 0xA0A0A0, _g("D_800B66F4", i)])
	engine.call_c("dtd_on_tpage", [0, 0, 9, 1, 0])
	engine.call_c("_sprite_set_light_effect1", [-0x140, -0x118, -0x142, -0x141, -0xA2, -0x79, 9, 0x808080, 0])
	engine.call_c("dtd_on_tpage", [0, 0, 9, 1, 0])

# ---- cursor

func hud_cursor_update() -> void:
	if Math.u8(_g("D_8011ECA4")) != 0:
		return
	var base_speed: int = engine.gs_get("cursor_speed")
	_set_g("D_8011ECA4", Math.u8(_g("D_8011ECA4")) + 1)
	if base_speed == 0:
		base_speed = 1
	engine.gs_set("cursor_speed", base_speed)
	var step: int = base_speed
	var held: int = engine.gs_get("pad_held")
	if (engine.gs_get("menu_mode") & 3) == 0 and (held & 0xFF) != 0:
		step = base_speed * 2
	if (held & 0xF) == 0:
		match held & 0x5000:
			0x1000:
				_set_g("cursor_y", _g("cursor_y") - step)
			0x4000:
				_set_g("cursor_y", _g("cursor_y") + step)
		match held & 0xA000:
			0x2000:
				_set_g("cursor_x", _g("cursor_x") + step)
			0x8000:
				_set_g("cursor_x", _g("cursor_x") - step)
	if _g("cursor_x") >= 0x8D:
		_set_g("cursor_x", 0x8C)
	if _g("cursor_x") < -0x96:
		_set_g("cursor_x", -0x96)
	if _g("cursor_y") < -0x64:
		_set_g("cursor_y", -0x64)
	if _g("cursor_y") >= 0x65:
		_set_g("cursor_y", 0x64)
	if engine.gs_get("pad_pressed") & PAD_SQUARE:
		var style: int = _g("cursor_style")
		if style == 0:
			_set_g("cursor_style", 0x9B)
		elif _g("cursor_style") == 0xAC:
			_set_g("cursor_style", 0)
		else:
			_set_g("cursor_style", _g("cursor_style") + 1)
		_set_g("D_8011ECE8", _g("cursor_style"))

## Both switch arms of the C are the same test: the frame is cursor_style when it is 0x9B..0xAD, else 0.
func hud_cursor_actor_init(_arg0: int) -> void:
	var saved_x: int = engine.memory.s16(_a(0) + 0x26)
	var saved_y: int = engine.memory.s16(_a(0) + 0x2A)
	engine.call_c("actor_clear", [0])
	_a8(0, 1, 1)
	_a8(0, 2, 0x40)
	_a32(0, 0x38, 0)
	_a8(0, 3, 0x84)
	_actor_tables(0)
	_a8(0, 4, 8)
	_a8(0, 5, 1)
	_a16(0, 0x16, 0)
	var style: int = _g("cursor_style")
	if style == 0 or style < 0x9B:
		_a16(0, 0x18, 0)
	else:
		_a16(0, 0x18, style)
		if style >= 0xAE:
			_a16(0, 0x18, 0)
	_a16(0, 0x26, saved_x)
	_a16(0, 0x2A, saved_y)

func hud_cursor_show(arg0: int) -> void:
	var address: int = _gaddr("D_8011ECD3")
	if Math.s32(arg0) == 1:
		engine.memory.write_u8(address, engine.memory.u8(address) | 0x80)
	else:
		engine.memory.write_u8(address, engine.memory.u8(address) & 0x7F)

# ---- biorhythm

func hud_command_roll_poor_result(days_to_advance: int) -> int:
	var original: int = Math.u16(engine.gs_get("day_count"))
	engine.gs_set("day_count", original + Math.s32(days_to_advance))
	var score: int = 0
	var club: int = engine.memory.u16(_gs("player_flags")) >> 12
	match engine.memory.u32(_gs("last_command")) & 15:
		0:
			score = _bio(0, 0, 1) + _param("param_liberal_arts") / 16
		1:
			score = _bio(0, 0, 1) + _param("param_science") / 16
		2:
			score = _bio(0, 1, 0) + _param("param_art") / 16
		3:
			score = _bio(1, 0, 0) + _param("param_athletics") / 16
		4:
			match club:
				0, 2, 3:
					score = _bio(0, 0, 1)
				1, 4, 5:
					score = _bio(0, 1, 0)
				6, 7, 8, 9, 10:
					score = _bio(1, 0, 0)
			score += engine.memory.u16(_gs("clubs") + club * 4) >> 4
		5:
			score = _bio(0, 1, 0) + _param("param_trivia") / 16
		6:
			score = _bio(0, 1, 0) + _param("param_looks") / 16
		7:
			engine.call_c("hud_biorhythm_value", [1, 1, 1, SCALE_SCORE])
			score = 16
	score += 6
	var stress: int = _param("param_stress")
	var stress_factor: int = 4 if stress >= 97 else stress / 32
	score = _mod(_cs32("rand", []), score + 4)
	score = _div((8 - stress_factor) * score, 8)
	var zero_waves: int = 0
	if engine.call_c("hud_biorhythm_value", [1, 0, 0, SCALE_ZERO_PROBE]) == 0:
		zero_waves += 1
	if engine.call_c("hud_biorhythm_value", [0, 1, 0, SCALE_ZERO_PROBE]) == 0:
		zero_waves += 1
	if engine.call_c("hud_biorhythm_value", [0, 0, 1, SCALE_ZERO_PROBE]) == 0:
		zero_waves += 1
	if zero_waves == 3:
		score /= 4
	engine.gs_set("day_count", original)
	return 0 if score >= 4 else 1

func _bio(a: int, b: int, c: int) -> int:
	return engine.call_c("hud_biorhythm_value", [a, b, c, SCALE_SCORE])

## ParamValue.value (s16 at +2).
func _param(field: String) -> int:
	return engine.memory.s16(_gs(field) + 2)

func hud_biorhythm_probe_discard() -> void:
	engine.call_c("hud_biorhythm_value", [1, 1, 1, SCALE_DISCARDED])

func hud_biorhythm_value(include_23_day_wave: int, include_28_day_wave: int, include_33_day_wave: int, scale: int) -> int:
	var scale_value: int = Math.s32(scale)
	var offset: int = engine.call_c("hud_biorhythm_birth_offset")
	var day: int = Math.s32(offset + Math.u16(engine.gs_get("day_count")))
	var w23: int = _cs32("rsin", [Math.s32(day * 0xB2)])
	var w28: int = _cs32("rsin", [Math.s32(day * 0x92)])
	var w33: int = _cs32("rsin", [Math.s32(day * 0x7C)])
	var critical: int = 0
	var selected: int = 0
	var total: int = 0
	var included: Array = [Math.s32(include_23_day_wave) != 0, Math.s32(include_28_day_wave) != 0, Math.s32(include_33_day_wave) != 0]
	var waves: Array = [w23, w28, w33]
	for k: int in 3:
		if included[k]:
			selected += 1
			var w: int = waves[k]
			if _cs32("abs", [w]) < 0x199:
				total -= 0x1000
				critical += 1
			elif w > 0:
				total += w
			else:
				total -= w / 2
	if selected == 0:
		return 1
	if scale_value > 0:
		if total / selected < 0:
			return 0
		return (total * scale_value / 4096) / selected
	elif scale_value < 0:
		for k: int in 3:
			if included[k]:
				return waves[k] * scale_value / 4096
		return 0
	if selected == 3:
		return -1 if critical >= 2 else 1
	if selected == 1 or selected == 2:
		return -1 if critical != 0 else 1
	return 1

func hud_biorhythm_birth_offset() -> int:
	var birthday: int = engine.memory.u32(_gs("player_birthday"))
	var table: int = _gaddr("birthday_day_table")
	var day: int = (birthday >> 4) & 0x1F
	var offset: int = -engine.memory.u16(table + (birthday & 0xF) * 2) - day + 0x1563
	if (birthday & 0xF) == 4 and day < 4:
		offset = -engine.memory.u16(table + 3 * 2) - day + 0x1582
	return offset

# ---- icons and markers

func hud_corner_icon_set(arg0: int) -> void:
	engine.call_c("actor_clear", [0xF])
	_set_g("D_8011F0CD", 0)
	_set_g("D_8011F0CE", 0x40)
	_set_g("D_8011F104", 0)
	_set_g("D_8011F0CF", 0x8C if Math.s32(arg0) != 0 else 0)
	_set_g("D_8011F0D8", _gaddr("sprite_frame_table"))
	_set_g("D_8011F0DC", _gaddr("actor_default_sequence_table"))
	_set_g("D_8011F100", _gaddr("actor_default_texture_blocks"))
	_set_g("D_8011F0E0", _g("D_800C9A54"))
	_set_g("D_8011F0D0", 8)
	_set_g("D_8011F0D1", 1)
	_set_g("D_8011F10F", 0)
	_set_g("D_8011F0E2", 0)
	_set_g("D_8011F0E4", 0xAD)
	_set_g("D_8011F0F2", -0x86)
	_set_g("D_8011F0F6", -0x5C)

func hud_text_marker_set(arg0: int) -> void:
	engine.call_c("actor_clear", [2])
	_set_g("D_8011ED59", 2)
	_set_g("D_8011ED5A", 0x40)
	_set_g("D_8011ED90", 0x40000000)
	_set_g("D_8011ED64", _gaddr("sprite_frame_table"))
	_set_g("D_8011ED68", _gaddr("actor_default_sequence_table"))
	_set_g("D_8011ED8C", _gaddr("actor_default_texture_blocks"))
	_set_g("D_8011ED70", 1)
	_set_g("D_8011ED5C", 8)
	_set_g("D_8011ED5D", 1)
	_set_g("D_8011ED7E", 0x74)
	_set_g("D_8011ED82", 0x48)
	var address: int = _gaddr("D_8011ED5B")
	if Math.s32(arg0) == 1:
		engine.memory.write_u8(address, engine.memory.u8(address) | 0x80)
	else:
		engine.memory.write_u8(address, engine.memory.u8(address) & 0x7F)

func hud_text_marker_bob() -> int:
	if _g("D_8011ED82") < 0x4F:
		if (engine.gs_get("frame_count") & 3) == 0:
			_set_g("D_8011ED82", _g("D_8011ED82") + 1)
	else:
		_set_g("D_8011ED82", 0x48)
	return 0

## Scans event_log (a record with year 0 ends the list). `last` gets the girl of the last record whose scene is
## 0, 1, 2 or 0xF (0xFF when none), `final` the girl of the last record. Clears year of every record after the
## list and returns the count, at most 0xFF.
func hud_event_log_scan(last_qualifying_girl_out: int, final_event_girl_out: int, _arg2: int) -> int:
	var m = engine.memory
	var log: int = _gs("event_log")
	var last_girl: int = 0xFF
	var final_girl: int = 0
	var count: int = 0
	for i: int in 0x100:
		var entry: int = log + count * 8
		if m.u8(entry + 4) == 0:
			break
		var scene: int = m.u8(entry + 2)
		var girl: int = m.u8(entry)
		if scene == 0 or scene == 1 or scene == 2 or scene == 0xF:
			last_girl = girl
		count += 1
		final_girl = girl
	for i: int in range(count, 0x100):
		m.write_u8(log + i * 8 + 4, 0)
	m.write_u8(last_qualifying_girl_out, last_girl)
	m.write_u8(final_event_girl_out, final_girl)
	if count >= 0xFF:
		count = 0xFF
	return count

func hud_confirm_button_label() -> int:
	if engine.gs_get("input_device") == 0: # INPUT_MOUSE
		if engine.gs_get("confirm_button") != 0: # SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT
			return 0x800B672C
		return 0x800B6730
	return 0x800B6724

func hud_cancel_button_label() -> int:
	if engine.gs_get("input_device") == 0:
		if engine.gs_get("confirm_button") != 0:
			return 0x800B6730
		return 0x800B672C
	return 0x800B6728
