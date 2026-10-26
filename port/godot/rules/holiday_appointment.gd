class_name HolidayAppointmentRules
extends RefCounted

const NativeRng = preload("res://core/native_rng.gd")

const PAD_CONFIRM := 0x20

class HolidayState:
	var year := 95
	var month := 4
	var day := 9
	var icon_enabled: Array[int] = [1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0]
	var menu_cursor := 0
	var last_command := 0
	var mode_stage := 0
	var stage_step := 0
	var substep := 0
	var menu_open := 1
	var input_ready := 1
	var chosen_place := 0xFF
	var appointment_place_1 := 0xFF
	var appointment_place_2 := 0xFF
	var appointment_place_3 := 0xFF


class StepResult:
	var events: Array[Dictionary] = []

	func emit(kind: String, payload: Dictionary = {}) -> void:
		events.append({"kind": kind, "payload": payload})


static func holiday_icons_set_available(state: HolidayState) -> void:
	state.icon_enabled = [1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0]

const GAME_MODE_DATE := 0x50
const DAY_EVENT_APPOINTMENT := 0x80
const MODE_STAGE_APPOINTMENT := 0xFE

const SPEAKER_STYLE_PLAYER_THOUGHT := 0x1F
const SPEAKER_STYLE_GIRL := 0x00

const MENU_SLOT_APPOINTMENT := 1
const MENU_BAR_STYLE_APPOINTMENT := 0x822F7F2F
const MENU_CHOICE_X := -120
const MENU_CHOICE_Y0 := 50
const MENU_CHOICE_STEP_Y := 16
const MENU_CHOICE_W := 240
const MENU_CHOICE_H := 16

const TEXT_TODAY_PREFIX := "今日は、"
const TEXT_TODAY_PREFIX_HEX := "8da193fa82cd8141"
const TEXT_TODAY_SUFFIX := "とデートだ）"
const TEXT_TODAY_SUFFIX_HEX := "82c68366815b836782be816a"

const TEXT_CHOICE_PREFIX := "『"
const TEXT_CHOICE_PREFIX_HEX := "8177"
const TEXT_CHOICE_SUFFIX := "で、待ち合わせ』"
const TEXT_CHOICE_SUFFIX_HEX := "82c5814191d282bf8d8782ed82b98178"

const TEXT_CONFIRM_PREFIX := "それじゃ、 "
const TEXT_CONFIRM_PREFIX_HEX := "82bb82ea82b682e1814120"
const TEXT_CONFIRM_MIDDLE := "で、 "
const TEXT_CONFIRM_MIDDLE_HEX := "82c5814120"
const TEXT_CONFIRM_SUFFIX := "を待つか。）"
const TEXT_CONFIRM_SUFFIX_HEX := "82f091d282c282a98142816a"

const TEXT_FULLWIDTH_OPEN_PAREN := "（"
const TEXT_FULLWIDTH_OPEN_PAREN_HEX := "8169"
const TEXT_FULLWIDTH_OPEN_QUOTE := "「"
const TEXT_FULLWIDTH_OPEN_QUOTE_HEX := "8175"

const APPOINTMENT_PLACE_PERMUTATIONS_3 := [
	[0, 1, 2],
	[1, 0, 2],
	[0, 2, 1],
]

const APPOINTMENT_PLACE_PERMUTATIONS_2 := [
	[0, 1],
	[1, 0],
]

const PLACE_NAME_TABLE := [
	{"id": 0, "text": "近所の公園", "bytes_hex": "8bdf8f8a82cc8cf68980", "symbol": "place_name_00_park"},
	{"id": 1, "text": "きらめき中央公園", "bytes_hex": "82ab82e782df82ab9286899b8cf68980", "symbol": "place_name_01_central_park"},
	{"id": 2, "text": "ショッピング街", "bytes_hex": "83568387836283738393834f8a58", "symbol": "place_name_02_shopping"},
	{"id": 3, "text": "水族館", "bytes_hex": "908591b08ad9", "symbol": "place_name_03_aquarium"},
	{"id": 4, "text": "動物園", "bytes_hex": "93ae95a88980", "symbol": "place_name_04_zoo"},
	{"id": 5, "text": "植物園", "bytes_hex": "904195a88980", "symbol": "place_name_05_botanical"},
	{"id": 6, "text": "プラネタリウム", "bytes_hex": "83768389836c835e838a83458380", "symbol": "place_name_06_planetarium"},
	{"id": 7, "text": "美術館", "bytes_hex": "94fc8f708ad9", "symbol": "place_name_07_art_museum"},
	{"id": 8, "text": "図書館", "bytes_hex": "907d8f918ad9", "symbol": "place_name_08_library"},
	{"id": 9, "text": "ゲームセンター", "bytes_hex": "8351815b8380835a8393835e815b", "symbol": "place_name_09_arcade"},
	{"id": 10, "text": "ボーリング場", "bytes_hex": "837b815b838a8393834f8f8a", "symbol": "place_name_10_bowling"},
	{"id": 11, "text": "カラオケ屋", "bytes_hex": "834a83898349835089ae", "symbol": "place_name_11_karaoke"},
	{"id": 12, "text": "遊園地", "bytes_hex": "97568980926e", "symbol": "place_name_12_amusement_park"},
	{"id": 13, "text": "スタジアム", "bytes_hex": "8358835e835783418380", "symbol": "place_name_13_stadium"},
	{"id": 14, "text": "映画館", "bytes_hex": "896689e68ad9", "symbol": "place_name_14_cinema"},
	{"id": 15, "text": "コンサート会場", "bytes_hex": "835283938354815b836789ef8f8a", "symbol": "place_name_15_concert_hall"},
	{"id": 16, "text": "プール", "bytes_hex": "8376815b838b", "symbol": "place_name_16_pool"},
	{"id": 17, "text": "海", "bytes_hex": "8a43", "symbol": "place_name_17_beach"},
	{"id": 18, "text": "神社", "bytes_hex": "905f8ed0", "symbol": "place_name_18_shrine"},
	{"id": 19, "text": "スケート場", "bytes_hex": "83588350815b83678f8a", "symbol": "place_name_19_skating_rink"},
	{"id": 20, "text": "スキー場", "bytes_hex": "8358834c815b8f8a", "symbol": "place_name_20_ski_resort"},
]

const GIRL_SHORT_NAME_TABLE := [
	{"id": 0, "short_text": "藤崎", "short_hex": "93a18de8", "called_text": "詩織", "called_hex": "8e8d9044"},
	{"id": 1, "short_text": "如月", "short_hex": "94408c8e", "called_text": "如月さん", "called_hex": "94408c8e82b382f1"},
	{"id": 2, "short_text": "紐緒", "short_hex": "95528f8f", "called_text": "紐緒さん", "called_hex": "95528f8f82b382f1"},
	{"id": 3, "short_text": "片桐", "short_hex": "95d08bcb", "called_text": "片桐さん", "called_hex": "95d08bcb82b382f1"},
	{"id": 4, "short_text": "虹野", "short_hex": "93f896ec", "called_text": "虹野さん", "called_hex": "93f896ec82b382f1"},
	{"id": 5, "short_text": "古式", "short_hex": "8ca58eae", "called_text": "古式さん", "called_hex": "8ca58eae82b382f1"},
	{"id": 6, "short_text": "清川", "short_hex": "90b490ec", "called_text": "清川さん", "called_hex": "90b490ec82b382f1"},
	{"id": 7, "short_text": "鏡", "short_hex": "8bbe", "called_text": "鏡さん", "called_hex": "8bbe82b382f1"},
	{"id": 8, "short_text": "朝日奈", "short_hex": "92fc93fa93de", "called_text": "朝日奈さん", "called_hex": "92fc93fa93de82b382f1"},
	{"id": 9, "short_text": "美樹原", "short_hex": "94fc8ef78cb4", "called_text": "美樹原さん", "called_hex": "94fc8ef78cb482b382f1"},
	{"id": 10, "short_text": "早乙女", "short_hex": "918189b38ff7", "called_text": "優美ちゃん", "called_hex": "974494fc82bf82e182f1"},
	{"id": 11, "short_text": "館林", "short_hex": "8ad997d1", "called_text": "館林さん", "called_hex": "8ad997d182b382f1"},
	{"id": 12, "short_text": "伊集院", "short_hex": "88c98f578940", "called_text": "伊集院", "called_hex": "88c98f578940"},
]


static func pack_appointment_word(entry: Dictionary) -> int:
	var place := int(entry.get("place", 0)) & 0x1F
	var month := int(entry.get("month", 0)) & 0xF
	var day := int(entry.get("day", 0)) & 0x1F
	var year := int(entry.get("year", 0)) & 0x7
	var flag := int(entry.get("flag", 0)) & 0x1
	var girl := int(entry.get("girl", 0)) & 0xF
	var active := int(entry.get("active", 0)) & 0x1
	var pad2 := int(entry.get("pad2", 0)) & 0x3
	var w := place | (month << 12) | (day << 16) | (year << 21) | (flag << 24) | (girl << 25) | (active << 29) | (pad2 << 30)
	return w & 0xFFFFFFFF


static func unpack_appointment_word(word: int) -> Dictionary:
	var w := word & 0xFFFFFFFF
	return {
		"w": w,
		"place": w & 0x1F,
		"month": (w >> 12) & 0xF,
		"day": (w >> 16) & 0x1F,
		"year": (w >> 21) & 0x7,
		"flag": (w >> 24) & 0x1,
		"girl": (w >> 25) & 0xF,
		"active": (w >> 29) & 0x1,
		"pad2": (w >> 30) & 0x3,
	}


static func get_g_name(girl_id: int) -> Dictionary:
	if girl_id < 0 or girl_id >= 16:
		return {"ok": -1, "girl_id": girl_id, "text": "", "bytes_hex": "", "speaker_text": "", "speaker_hex": ""}
	var idx := girl_id & 0xF
	for row in GIRL_SHORT_NAME_TABLE:
		if int(row["id"]) == idx:
			return {
				"ok": 1,
				"girl_id": girl_id,
				"text": String(row["called_text"]),
				"bytes_hex": String(row["called_hex"]),
				"speaker_text": String(row["short_text"]),
				"speaker_hex": String(row["short_hex"]),
			}
	return {"ok": 1, "girl_id": girl_id, "text": "", "bytes_hex": "", "speaker_text": "", "speaker_hex": ""}


static func place_name(place_id: int) -> Dictionary:
	for row in PLACE_NAME_TABLE:
		if int(row["id"]) == place_id:
			return row.duplicate(true)
	return {"id": place_id, "text": "", "bytes_hex": "", "symbol": ""}


static func hud_appointment_find_today(calendar_year: int, calendar_month: int, day_of_month: int, appointment_words: Array) -> int:
	# Exact port of hud_appointment_find_today (src/main/hud_screens.c:1232-1257)
	var month := calendar_month
	if month == 13:
		month = 1

	var r := 0
	if (month == 12 and day_of_month == 24) or (month == 1 and day_of_month == 1) or (month >= 3 and calendar_year >= 98):
		return -1

	var limit := mini(appointment_words.size(), 8)
	for i in range(limit):
		var w := int(appointment_words[i]) & 0xFFFFFFFF
		var entry_month := (w >> 12) & 0xF
		var entry_day := (w >> 16) & 0x1F
		if month == entry_month and day_of_month == entry_day:
			r = i + 1
	return r


static func holiday_icons_set_available_appointment(state: HolidayState, appointment_words: Array) -> int:
	# Exact port of holiday_icons_set_available appointment branch (src/main/holiday.c:299-301)
	var found := hud_appointment_find_today(state.year, state.month, state.day, appointment_words)
	if found <= 0:
		state.icon_enabled[9] = 0
	else:
		state.icon_enabled[9] = 1
	return found


static func sndisp_lines(text_hex: String, text_utf8: String, first_row: int, speaker_style: int) -> Array[Dictionary]:
	# Exact port of sndisp (src/main/menu_speaker_girl.c:490-608):
	# Splits on even-byte ASCII space 0x20. Row 0 gets opening bracket/quote at (-86, 50),
	# Row 1 at (-72, 66), Row 2 at (-72, 82).
	var raw_bytes := text_hex.hex_decode()
	var segments_bytes: Array[PackedByteArray] = []
	var current := PackedByteArray()
	var i := 0
	while i < raw_bytes.size():
		if raw_bytes[i] == 0x20:
			segments_bytes.append(current)
			current = PackedByteArray()
			i += 1
		else:
			current.append(raw_bytes[i])
			if i + 1 < raw_bytes.size():
				current.append(raw_bytes[i + 1])
			i += 2
	segments_bytes.append(current)

	var utf8_parts := text_utf8.split(" ", true)
	var lines: Array[Dictionary] = []
	var row := first_row
	for seg_idx in range(segments_bytes.size()):
		if row > 2:
			break
		var seg_hex := segments_bytes[seg_idx].hex_encode()
		var seg_text := utf8_parts[seg_idx] if seg_idx < utf8_parts.size() else ""
		var line_hex := seg_hex
		var line_text := seg_text
		var x := -72
		var y := 50 + row * 16
		if row == 0:
			x = -86
			y = 50
			if speaker_style == SPEAKER_STYLE_PLAYER_THOUGHT:
				line_hex = TEXT_FULLWIDTH_OPEN_PAREN_HEX + seg_hex
				line_text = TEXT_FULLWIDTH_OPEN_PAREN + seg_text
			elif speaker_style >= 0 and speaker_style <= 0x0E:
				line_hex = TEXT_FULLWIDTH_OPEN_QUOTE_HEX + seg_hex
				line_text = TEXT_FULLWIDTH_OPEN_QUOTE + seg_text
		lines.append({
			"row": row,
			"x": x,
			"y": y,
			"screen_x": 160 + x,
			"screen_y": 120 + y,
			"bytes_hex": line_hex,
			"text": line_text,
		})
		row += 1
	return lines


static func holiday_open_init_appointment_notice(state: HolidayState, appointment_words: Array, player_lastname_text: String = "藤崎", player_lastname_hex: String = "93a18de8") -> Dictionary:
	# Exact port of holiday_open_init appointment check (src/main/holiday.c:227-235)
	holiday_icons_set_available_appointment(state, appointment_words)
	var appt_idx := hud_appointment_find_today(state.year, state.month, state.day, appointment_words)
	if appt_idx <= 0:
		return {"has_appointment": false, "appointment_index": appt_idx}

	state.menu_cursor = 9
	var slot := appt_idx - 1
	var entry := unpack_appointment_word(int(appointment_words[slot]))
	var girl_info := get_g_name(int(entry["girl"]))
	var raw_text := TEXT_TODAY_PREFIX + String(girl_info["text"]) + TEXT_TODAY_SUFFIX
	var raw_hex := TEXT_TODAY_PREFIX_HEX + String(girl_info["bytes_hex"]) + TEXT_TODAY_SUFFIX_HEX
	var lines := sndisp_lines(raw_hex, raw_text, 0, SPEAKER_STYLE_PLAYER_THOUGHT)
	return {
		"has_appointment": true,
		"appointment_index": appt_idx,
		"slot": slot,
		"girl_id": int(entry["girl"]),
		"place_id": int(entry["place"]),
		"girl_name": String(girl_info["text"]),
		"girl_name_hex": String(girl_info["bytes_hex"]),
		"speaker_text": player_lastname_text,
		"speaker_hex": player_lastname_hex,
		"notice_line_count": 1,
		"raw_text": raw_text,
		"raw_bytes_hex": raw_hex,
		"sndisp_lines": lines,
	}


static func holiday_menu_click_appointment(state: HolidayState, appointment_words: Array, result: StepResult) -> bool:
	# Exact port of holiday_menu_click case 9 (src/main/holiday.c:500-506)
	if state.menu_cursor != 9:
		return false
	if state.icon_enabled[9] == 0:
		return false
	if hud_appointment_find_today(state.year, state.month, state.day, appointment_words) <= 0:
		return false
	state.last_command = (state.last_command & 0xF0) | 9
	result.emit("audio_command", {"code": 0x500})
	state.mode_stage = MODE_STAGE_APPOINTMENT
	state.stage_step = 0
	state.substep = 0
	return true


static func appointment_place_candidates(appointment_words: Array) -> Dictionary:
	# Exact port of appointment_place_candidates (src/main/holiday.c:534-593)
	var place_choices := [0xFF, 0xFF, 0xFF]
	var distinct_place_count := 0
	var limit := mini(appointment_words.size(), 8)

	for slot in range(limit):
		var w := int(appointment_words[slot]) & 0xFFFFFFFF
		var active := (w >> 29) & 0x1
		if active != 0:
			var place_id := w & 0x1F
			if distinct_place_count != 0:
				var duplicate := false
				for idx in range(distinct_place_count):
					if place_choices[idx] == place_id:
						duplicate = true
						break
				if not duplicate:
					place_choices[distinct_place_count] = place_id
					distinct_place_count += 1
			else:
				place_choices[0] = place_id
				distinct_place_count = 1

	return {
		"count": distinct_place_count,
		"choices": place_choices,
	}


static func appointment_start(state: HolidayState, appointment_words: Array, result: StepResult) -> Dictionary:
	# Exact port of appointment_start (src/main/holiday.c:595-626)
	state.menu_open = 0
	state.input_ready = 0
	result.emit("holiday_cursor_Sprt_hide")
	result.emit("holiday_icon_hover_reset")
	result.emit("voice_Metrics_reset")
	result.emit("holiday_Command_hud_hide")
	result.emit("action_Tags_clear")
	state.chosen_place = 0xFF
	state.appointment_place_1 = 0xFF
	state.appointment_place_2 = 0xFF
	state.appointment_place_3 = 0xFF

	for slot in range(mini(appointment_words.size(), 8)):
		var w := int(appointment_words[slot]) & 0xFFFFFFFF
		if ((w >> 12) & 0xF) == state.month and ((w >> 16) & 0x1F) == state.day:
			w |= (1 << 29)
			appointment_words[slot] = w & 0xFFFFFFFF

	var cand := appointment_place_candidates(appointment_words)
	var count := int(cand["count"])
	var choices: Array = cand["choices"]
	if count == 1:
		state.chosen_place = int(choices[0])
		state.stage_step += 2
	else:
		state.stage_step += 1
	state.substep = 0
	return {
		"distinct_place_count": count,
		"place_choices": choices.duplicate(),
		"chosen_place": state.chosen_place,
		"next_stage_step": state.stage_step,
	}


static func appointment_place_menu_build(state: HolidayState, appointment_words: Array, rng: NativeRng, result: StepResult) -> Dictionary:
	# Exact port of appointment_place_menu_build (src/main/holiday.c:631-704)
	var cand := appointment_place_candidates(appointment_words)
	var count := int(cand["count"])
	var choices: Array = cand["choices"]
	var perm: Array = []

	result.emit("x_taku_menu_set", {"slot": MENU_SLOT_APPOINTMENT, "count": count})
	if count == 3:
		var order: Array = APPOINTMENT_PLACE_PERMUTATIONS_3[rng.next("appointment_place_menu_build") % 3]
		perm = order.duplicate()
		state.appointment_place_1 = int(choices[int(order[0])])
		state.appointment_place_2 = int(choices[int(order[1])])
		state.appointment_place_3 = int(choices[int(order[2])])
	elif count == 2:
		var order2: Array = APPOINTMENT_PLACE_PERMUTATIONS_2[rng.next("appointment_place_menu_build") & 1]
		perm = order2.duplicate()
		state.appointment_place_1 = int(choices[int(order2[0])])
		state.appointment_place_2 = int(choices[int(order2[1])])

	var menu_places := [state.appointment_place_1, state.appointment_place_2, state.appointment_place_3]
	var rows: Array[Dictionary] = []
	for i in range(count):
		var place_id := int(menu_places[i])
		var pinfo := place_name(place_id)
		var line_text := TEXT_CHOICE_PREFIX + String(pinfo["text"]) + TEXT_CHOICE_SUFFIX
		var line_hex := TEXT_CHOICE_PREFIX_HEX + String(pinfo["bytes_hex"]) + TEXT_CHOICE_SUFFIX_HEX
		var y := MENU_CHOICE_Y0 + i * MENU_CHOICE_STEP_Y
		var row_dict := {
			"index": i,
			"place_id": place_id,
			"place_name": String(pinfo["text"]),
			"place_bytes_hex": String(pinfo["bytes_hex"]),
			"text": line_text,
			"bytes_hex": line_hex,
			"x": MENU_CHOICE_X,
			"y": y,
			"w": MENU_CHOICE_W,
			"h": MENU_CHOICE_H,
			"screen_x": 160 + MENU_CHOICE_X,
			"screen_y": 120 + y,
		}
		rows.append(row_dict)
		result.emit("x_taku_string_set", row_dict)

	return {
		"count": count,
		"permutation": perm,
		"appointment_places": menu_places,
		"rows": rows,
	}


static func appointment_place_choose(state: HolidayState, appointment_words: Array, pressed: int, rng: NativeRng, xa_playing: bool, result: StepResult, menu_choice_index: int = -1) -> Dictionary:
	# Exact port of appointment_place_choose (src/main/holiday.c:706-725)
	var summary := {"substep": state.substep}
	match state.substep:
		0:
			if not xa_playing:
				result.emit("clear_message_area")
				state.substep += 1
		1:
			summary["menu"] = appointment_place_menu_build(state, appointment_words, rng, result)
			state.substep += 1
		2:
			result.emit("menu_bar_show", {"slot": MENU_SLOT_APPOINTMENT, "style": MENU_BAR_STYLE_APPOINTMENT})
			var chosen_idx := menu_choice_index
			if chosen_idx < 0 and (pressed & PAD_CONFIRM) != 0:
				chosen_idx = 0
			if chosen_idx >= 0:
				var menu_places := [state.appointment_place_1, state.appointment_place_2, state.appointment_place_3]
				state.chosen_place = int(menu_places[clampi(chosen_idx, 0, 2)])
				result.emit("menu_bar_hide")
				state.stage_step += 1
				state.substep = 0
				summary["chosen_index"] = chosen_idx
				summary["chosen_place"] = state.chosen_place
	return summary


static func build_appointment_confirm_Presentation(state: HolidayState, appointment_words: Array, player_lastname_text: String = "藤崎", player_lastname_hex: String = "93a18de8") -> Dictionary:
	# Builds the exact text/sndisp payload produced by appointment_place_confirm substep 1 (src/main/holiday.c:735-752)
	var matched_girl := 0
	for slot in range(mini(appointment_words.size(), 8)):
		var w := int(appointment_words[slot]) & 0xFFFFFFFF
		var active := (w >> 29) & 0x1
		var place_id := w & 0x1F
		if active != 0 and place_id == state.chosen_place:
			matched_girl = (w >> 25) & 0xF

	var pinfo := place_name(state.chosen_place)
	var ginfo := get_g_name(matched_girl)
	var raw_text := TEXT_CONFIRM_PREFIX + String(pinfo["text"]) + TEXT_CONFIRM_MIDDLE + String(ginfo["text"]) + TEXT_CONFIRM_SUFFIX
	var raw_hex := TEXT_CONFIRM_PREFIX_HEX + String(pinfo["bytes_hex"]) + TEXT_CONFIRM_MIDDLE_HEX + String(ginfo["bytes_hex"]) + TEXT_CONFIRM_SUFFIX_HEX
	var lines := sndisp_lines(raw_hex, raw_text, 0, SPEAKER_STYLE_PLAYER_THOUGHT)
	return {
		"chosen_place": state.chosen_place,
		"place_name": String(pinfo["text"]),
		"place_bytes_hex": String(pinfo["bytes_hex"]),
		"girl_id": matched_girl,
		"girl_name": String(ginfo["text"]),
		"girl_bytes_hex": String(ginfo["bytes_hex"]),
		"speaker_text": player_lastname_text,
		"speaker_hex": player_lastname_hex,
		"raw_text": raw_text,
		"raw_bytes_hex": raw_hex,
		"sndisp_lines": lines,
	}


static func appointment_place_confirm(state: HolidayState, appointment_words: Array, xa_playing: bool, result: StepResult, wait_finished: bool = true, player_lastname_text: String = "藤崎", player_lastname_hex: String = "93a18de8") -> Dictionary:
	# Exact port of appointment_place_confirm (src/main/holiday.c:729-758)
	var summary := {"substep": state.substep}
	match state.substep:
		0:
			if not xa_playing:
				result.emit("clear_message_area")
				state.substep += 1
		1:
			var pres := build_appointment_confirm_Presentation(state, appointment_words, player_lastname_text, player_lastname_hex)
			result.emit("gnsx", {"speaker_text": pres["speaker_text"], "speaker_hex": pres["speaker_hex"]})
			result.emit("sndisp", pres)
			summary["presentation"] = pres
			state.substep += 1
		2:
			if wait_finished:
				state.stage_step += 1
				state.substep = 0
	return summary


static func appointment_launch(state: HolidayState, appointment_words: Array, result: StepResult, day_event_index: int = 0) -> Dictionary:
	# Exact port of appointment_launch (src/main/holiday.c:760-784)
	var cand := appointment_place_candidates(appointment_words)
	var count := int(cand["count"])
	var choices: Array = cand["choices"]
	state.appointment_place_1 = state.chosen_place
	var unchosen_out := 0
	for i in range(count):
		var candidate_place := int(choices[i])
		if candidate_place != state.chosen_place:
			if unchosen_out == 0:
				state.appointment_place_2 = candidate_place
			elif unchosen_out == 1:
				state.appointment_place_3 = candidate_place
			unchosen_out += 1

	var chosen_girl := 0
	var chosen_flag := 0
	for slot in range(mini(appointment_words.size(), 8)):
		var w := int(appointment_words[slot]) & 0xFFFFFFFF
		var active := (w >> 29) & 0x1
		var place_id := w & 0x1F
		if active != 0 and place_id == state.chosen_place:
			chosen_girl = (w >> 25) & 0xF
			chosen_flag = (w >> 24) & 0x1

	var day_event := [DAY_EVENT_APPOINTMENT, chosen_girl, state.chosen_place, 0]
	var launch_payload := {
		"game_mode": GAME_MODE_DATE,
		"day_event_index": day_event_index,
		"day_event": day_event,
		"current_girl": chosen_girl,
		"date_girl": chosen_girl,
		"appointment_flag": chosen_flag,
		"chosen_place": state.chosen_place,
		"appointment_place_1": state.appointment_place_1,
		"appointment_place_2": state.appointment_place_2,
		"appointment_place_3": state.appointment_place_3,
	}
	result.emit("fade_out", {"frames": 30})
	result.emit("game_mode_set", launch_payload)
	return launch_payload


static func step_stage_fe(state: HolidayState, appointment_words: Array, pressed: int, rng: NativeRng, xa_playing: bool, result: StepResult, menu_choice_index: int = -1, wait_finished: bool = true) -> Dictionary:
	# Exact port of holiday_update case 0xFE (src/main/holiday.c:786-802)
	match state.stage_step:
		0:
			return appointment_start(state, appointment_words, result)
		1:
			return appointment_place_choose(state, appointment_words, pressed, rng, xa_playing, result, menu_choice_index)
		2:
			return appointment_place_confirm(state, appointment_words, xa_playing, result, wait_finished)
		3:
			return appointment_launch(state, appointment_words, result)
	return {}
