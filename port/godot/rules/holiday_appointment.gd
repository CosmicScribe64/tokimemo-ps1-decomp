class_name HolidayAppointmentRules
extends RefCounted

const NativeRng = preload("res://core/native_rng.gd")

const PAD_CONFIRM := 0x20
const PAD_CANCEL := 0x40
const PAD_UP := 0x1000
const PAD_RIGHT := 0x2000
const PAD_DOWN := 0x4000
const PAD_LEFT := 0x8000

class HolidayState:
	var year := 95
	var month := 4
	var day := 9
	var weekday := 6
	var day_kind := 1
	var parameters: Array[int] = [97, 43, 40, 40, 38, 33, 59, 4, 3]
	var frame_count := 0
	var game_mode := 0x46
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
	var date_stage := 0
	var date_step := 0
	var date_substep := 0
	var date_choice_cursor := 0
	var date_chosen_option := -1
	var date_score := 0
	var date_score_class := 0
	var date_choice_order: Array[int] = [0, 1, 2]
	var date_line_wait := 1
	var cursor_style := 156
	var player_flags := 0


class StepResult:
	var events: Array[Dictionary] = []

	func emit(kind: String, payload: Dictionary = {}) -> void:
		events.append({"kind": kind, "payload": payload})


static func holiday_icons_set_available(state: HolidayState) -> void:
	state.icon_enabled = [1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0]

const GAME_MODE_HOLIDAY := 0x46
const GAME_MODE_DATE := 0x50
const DAY_EVENT_APPOINTMENT := 0x80
const MODE_STAGE_APPOINTMENT := 0xFE

const SPEAKER_STYLE_PLAYER_THOUGHT := 0x1F
const SPEAKER_STYLE_GIRL := 0x00

const TALK_CMD_GIRL := 0x01
const TALK_CMD_GIRL_REPLY := 0x02
const TALK_CMD_THREE_SELECT := 0x03
const TALK_CMD_PLAYER_PAREN := 0x04

const MENU_SLOT_APPOINTMENT := 1
const MENU_BAR_STYLE_APPOINTMENT := 0x822F7F2F
const MENU_CHOICE_X := -120
const MENU_CHOICE_Y0 := 50
const MENU_CHOICE_STEP_Y := 16
const MENU_CHOICE_W := 240
const MENU_CHOICE_H := 16

# VN Scene Text Layout constants (talk_speak_part + talk_render, src/main/scene_library.c:1185, 1340-1346, 1681-2052)
const VN_ROW_Y0 := 48
const VN_ROW_STEP_Y := 16
const VN_QUOTE_X := -86
const VN_BODY_X := -72

# VN Scene Blinking/Bobbing Wait Marker (hud_text_marker_set + hud_text_marker_bob, src/main/hud_screens.c:1740-1768)
const VN_WAIT_MARKER_ACTOR_ID := 2
const VN_WAIT_MARKER_PRIORITY := 2
const VN_WAIT_MARKER_FRAME_ID := 1
const VN_WAIT_MARKER_SEQUENCE_SLOT := 1
const VN_WAIT_MARKER_X := 116
const VN_WAIT_MARKER_Y0 := 72
const VN_WAIT_MARKER_Y_MAX := 78

# 3-Choice Date Menu constants (normal_date_three_select, src/main/scene_library.c:1583-1593, 2196-2253)
const DATE_THREE_SELECT_SLOT := 1
const DATE_THREE_SELECT_STYLE := 0x822F7F2F
const DATE_THREE_SELECT_X := -128
const DATE_THREE_SELECT_TEXT_X := -114
const DATE_THREE_SELECT_Y0 := 48
const DATE_THREE_SELECT_STEP_Y := 16
const DATE_THREE_SELECT_W := 252
const DATE_THREE_SELECT_H := 16
const DATE_THREE_SELECT_CURSOR_FRAME := 156
const DATE_THREE_SELECT_CURSOR_X := 120
const DATE_THREE_SELECT_CURSOR_Y0 := 56

const TEXT_TODAY_PREFIX := "今日は、"
const TEXT_TODAY_PREFIX_HEX := "8da193fa82cd8141"
const TEXT_TODAY_SUFFIX := "とデートだ）"
const TEXT_TODAY_SUFFIX_HEX := "82c68366815b836782be816a"

const TEXT_CHOICE_PREFIX := "『"
const TEXT_CHOICE_PREFIX_HEX := "8177"
const TEXT_CHOICE_CLOSE := "』"
const TEXT_CHOICE_CLOSE_HEX := "8178"
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
const TEXT_FULLWIDTH_CLOSE_PAREN := "）"
const TEXT_FULLWIDTH_CLOSE_PAREN_HEX := "816a"
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
	{"id": 10, "text": "ボーリング場", "bytes_hex": "837b815b838a8393834f8fea", "symbol": "place_name_10_bowling"},
	{"id": 11, "text": "カラオケ屋", "bytes_hex": "834a83898349835089ae", "symbol": "place_name_11_karaoke"},
	{"id": 12, "text": "遊園地", "bytes_hex": "97568980926e", "symbol": "place_name_12_amusement_park"},
	{"id": 13, "text": "スタジアム", "bytes_hex": "8358835e835783418380", "symbol": "place_name_13_stadium"},
	{"id": 14, "text": "映画館", "bytes_hex": "896689e68ad9", "symbol": "place_name_14_cinema"},
	{"id": 15, "text": "コンサート会場", "bytes_hex": "835283938354815b836789ef8fea", "symbol": "place_name_15_concert_hall"},
	{"id": 16, "text": "プール", "bytes_hex": "8376815b838b", "symbol": "place_name_16_pool"},
	{"id": 17, "text": "海", "bytes_hex": "8a43", "symbol": "place_name_17_beach"},
	{"id": 18, "text": "神社", "bytes_hex": "905f8ed0", "symbol": "place_name_18_shrine"},
	{"id": 19, "text": "スケート場", "bytes_hex": "83588350815b83678fea", "symbol": "place_name_19_skating_rink"},
	{"id": 20, "text": "スキー場", "bytes_hex": "8358834c815b8fea", "symbol": "place_name_20_ski_resort"},
]

const GIRL_SHORT_NAME_TABLE := [
	{"id": 0, "short_text": "藤崎", "short_hex": "93a18de8", "called_text": "詩織", "called_hex": "8e8d9044"},
	{"id": 1, "short_text": "如月", "short_hex": "94408c8e", "called_text": "如月さん", "called_hex": "94408c8e82b382f1"},
	{"id": 2, "short_text": "紐緒", "short_hex": "95528f8f", "called_text": "紐緒さん", "called_hex": "95528f8f82b382f1"},
	{"id": 3, "short_text": "片桐", "short_hex": "95d08bcb", "called_text": "片桐さん", "called_hex": "95d08bcb82b382f1"},
	{"id": 4, "short_text": "虹野", "short_hex": "93f896ec", "called_text": "虹野さん", "called_hex": "93f896ec82b382f1"},
	{"id": 5, "short_text": "古式", "short_hex": "8cc38eae", "called_text": "古式さん", "called_hex": "8cc38eae82b382f1"},
	{"id": 6, "short_text": "清川", "short_hex": "90b490ec", "called_text": "清川さん", "called_hex": "90b490ec82b382f1"},
	{"id": 7, "short_text": "鏡", "short_hex": "8bbe", "called_text": "鏡さん", "called_hex": "8bbe82b382f1"},
	{"id": 8, "short_text": "朝日奈", "short_hex": "92a993fa93de", "called_text": "朝日奈さん", "called_hex": "92a993fa93de82b382f1"},
	{"id": 9, "short_text": "美樹原", "short_hex": "94fc8ef78cb4", "called_text": "美樹原さん", "called_hex": "94fc8ef78cb482b382f1"},
	{"id": 10, "short_text": "早乙女", "short_hex": "918189b38f97", "called_text": "優美ちゃん", "called_hex": "974494fc82bf82e182f1"},
	{"id": 11, "short_text": "館林", "short_hex": "8ad997d1", "called_text": "館林さん", "called_hex": "8ad997d182b382f1"},
	{"id": 12, "short_text": "伊集院", "short_hex": "88c98f578940", "called_text": "伊集院", "called_hex": "88c98f578940"},
]

# Exact date 3-choice script records from CDROM/SERIFU/DATE_FU.BIN (LBA 44329, src/ovl/date/place_scenes.c)
const DATE_THREE_CHOICE_SCRIPTS := {
	0: {
		"place_id": 0,
		"table_vram": "0x801A33B0",
		"question_vram": "0x801A341C",
		"question_text": "こういう道を 散歩するのって、 気持ちがいいね。",
		"question_bytes_hex": "82b182a482a282a493b982f0208e5595e082b782e982cc82c182c48141208b438e9d82bf82aa82a282a282cb8142",
		"park_opening_vram": "0x8019C1C8",
		"park_opening_text": "こういう所で、 お話するのって、 いいね。",
		"park_opening_bytes_hex": "82b182a482a48f8a82c581412082a8986282b782e982cc82c182c481412082a282a282cb8142",
		"choice_vram": "0x801A344C",
		"choice_raw_bytes_hex": "97f688a48fac90e082cc88ea8fea96ca82dd82bd82a282be82cb8142718e5595e082cd814182a895a082aa82b782a282c482ad82e982cb814271967b939681418b438e9d82bf82a282a282cb8142",
		"score_classes": [0, 4, 1],
		"tiers": [0, 2, 0],
		"options": [
			{
				"choice_id": 0,
				"text": "恋愛小説の一場面みたいだね。",
				"bytes_hex": "97f688a48fac90e082cc88ea8fea96ca82dd82bd82a282be82cb8142",
				"score_class": 0,
				"tier": 0,
				"score_delta": 10,
				"reply_vram": "0x801A34D0",
				"reply_text": "私も、そう思ってたの。 こういうのに、 ちょっと憧れてたんだ…。",
				"reply_bytes_hex": "8e8482e0814182bb82a48e7682c182c482bd82cc81422082b182a482a282a482cc82c981412082bf82e582c182c693b282ea82c482bd82f182be81638142",
			},
			{
				"choice_id": 1,
				"text": "散歩は、お腹がすいてくるね。",
				"bytes_hex": "8e5595e082cd814182a895a082aa82b782a282c482ad82e982cb8142",
				"score_class": 4,
				"tier": 2,
				"score_delta": -6,
				"reply_vram": "0x801A351C",
				"reply_text": "そ、そう。 せっかく、 いい気分だったのに…。",
				"reply_bytes_hex": "82bb814182bb82a481422082b982c182a982ad81412082a282a28b4395aa82be82c182bd82cc82c981638142",
			},
			{
				"choice_id": 2,
				"text": "本当、気持ちいいね。",
				"bytes_hex": "967b939681418b438e9d82bf82a282a282cb8142",
				"score_class": 1,
				"tier": 0,
				"score_delta": 8,
				"reply_vram": "0x801A3550",
				"reply_text": "日頃の疲れが 取れていくみたい。",
				"reply_bytes_hex": "93fa8da082cc94e682ea82aa208ee682ea82c482a282ad82dd82bd82a28142",
			},
		],
	},
	3: {
		"place_id": 3,
		"table_vram": "0x801A0540",
		"question_vram": "0x801A059C",
		"question_text": "奇麗なお魚が、 たくさんいるね。",
		"question_bytes_hex": "8aef97ed82c882a88b9b82aa81412082bd82ad82b382f182a282e982cb8142",
		"choice_vram": "0x801A05C8",
		"choice_raw_bytes_hex": "908582cc928682cc8341815b836782be82cb81427182b782b282ad82a882a282b582bb82a482be82cb814271967b939681418aef97ed82be82cb8142",
		"score_classes": [0, 4, 1],
		"tiers": [0, 2, 0],
		"options": [
			{
				"choice_id": 0,
				"text": "水の中のアートだね。",
				"bytes_hex": "908582cc928682cc8341815b836782be82cb8142",
				"score_class": 0,
				"tier": 0,
				"score_delta": 10,
				"reply_vram": "0x801A0634",
				"reply_text": "素敵、ロマンティックね。",
				"reply_bytes_hex": "916693478141838d837d8393836583428362834e82cb8142",
			},
			{
				"choice_id": 1,
				"text": "すごくおいしそうだね。",
				"bytes_hex": "82b782b282ad82a882a282b582bb82a482be82cb8142",
				"score_class": 4,
				"tier": 2,
				"score_delta": -6,
				"reply_vram": "0x801A0658",
				"reply_text": "どうして そんな事しか 考えられないの？",
				"reply_bytes_hex": "82c782a482b582c42082bb82f182c88e9682b582a9208d6c82a682e782ea82c882a282cc8148",
			},
			{
				"choice_id": 2,
				"text": "本当、奇麗だね。",
				"bytes_hex": "967b939681418aef97ed82be82cb8142",
				"score_class": 1,
				"tier": 0,
				"score_delta": 8,
				"reply_vram": "0x801A0688",
				"reply_text": "あっちにも 奇麗な魚がいるよ。",
				"reply_bytes_hex": "82a082c182bf82c982e0208aef97ed82c88b9b82aa82a282e982e68142",
			},
		],
	},
}

# Player date choice impression table (date_choice_reaction_text_table at 0x80197B04, src/ovl/date/place_scenes.c:3525-3554)
const DATE_CHOICE_REACTION_TABLE := {
	0: {
		"score_class": 0,
		"vram": "0x80197CC0",
		"text": "バッチリ、良い印象を 与えたみたいだぞ。",
		"bytes_hex": "836f83628360838a814197c782a288f38fdb82f020975e82a682bd82dd82bd82a282be82bc8142",
	},
	1: {
		"score_class": 1,
		"vram": "0x80197CEC",
		"text": "わりと良い印象を 与えたみたいだぞ。",
		"bytes_hex": "82ed82e882c697c782a288f38fdb82f020975e82a682bd82dd82bd82a282be82bc8142",
	},
	2: {
		"score_class": 2,
		"vram": "0x80197D18",
		"text": "まぁ、こんなもんだろう。",
		"bytes_hex": "82dc829f814182b182f182c882e082f182be82eb82a48142",
	},
	3: {
		"score_class": 3,
		"vram": "0x80197D38",
		"text": "あまり良い印象を 持たれなかったかな。",
		"bytes_hex": "82a082dc82e897c782a288f38fdb82f0208e9d82bd82ea82c882a982c182bd82a982c88142",
	},
	4: {
		"score_class": 4,
		"vram": "0x80197D64",
		"text": "悪い印象を 与えちゃったかな。",
		"bytes_hex": "88ab82a288f38fdb82f020975e82a682bf82e182c182bd82a982c88142",
	},
}


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


static func talk_speaker_x_for_hex(speaker_hex: String) -> int:
	# Exact port of talk_speak_part speaker x formula: -0x68 - (strlen(speaker) * 4)
	# (src/main/scene_library.c:1342, 1579, 1645). For 4-byte "藤崎"/"詩織", x = -120.
	var byte_len := speaker_hex.length() / 2
	return -0x68 - (byte_len * 4)


static func talk_render_lines(text_hex: String, text_utf8: String, first_row: int = 0, suffix_hex: String = "", suffix_text: String = "") -> Array[Dictionary]:
	# Exact port of talk_render(-0x48, temp_s1, text_color, line_buffer, 0, suffix, 0, 1)
	# (src/main/scene_library.c:1185, 1340-1346, 1681-2052):
	# Splits on even-byte ' ' (0x20) or 'q' (0x71). Every body line is drawn at x = -72 (-0x48),
	# y = 48 + row * 16 (48, 64, 80 -> screen y = 168, 184, 200).
	var raw_bytes := text_hex.hex_decode()
	var segments_bytes: Array[PackedByteArray] = []
	var current := PackedByteArray()
	var i := 0
	while i < raw_bytes.size():
		if raw_bytes[i] == 0x20 or raw_bytes[i] == 0x71:
			segments_bytes.append(current)
			current = PackedByteArray()
			i += 1
		else:
			current.append(raw_bytes[i])
			if i + 1 < raw_bytes.size():
				current.append(raw_bytes[i + 1])
			i += 2
	segments_bytes.append(current)

	var normalized_utf8 := text_utf8.replace("q", " ")
	var utf8_parts := normalized_utf8.split(" ", true)
	var lines: Array[Dictionary] = []
	var row := first_row
	var last_idx := mini(segments_bytes.size(), 3 - first_row) - 1
	for seg_idx in range(segments_bytes.size()):
		if row > 2:
			break
		var seg_hex := segments_bytes[seg_idx].hex_encode()
		var seg_text := utf8_parts[seg_idx] if seg_idx < utf8_parts.size() else ""
		if seg_idx == last_idx and not suffix_hex.is_empty():
			seg_hex += suffix_hex
			seg_text += suffix_text
		var x := VN_BODY_X
		var y := VN_ROW_Y0 + row * VN_ROW_STEP_Y
		lines.append({
			"row": row,
			"x": x,
			"y": y,
			"screen_x": 160 + x,
			"screen_y": 120 + y,
			"bytes_hex": seg_hex,
			"text": seg_text,
		})
		row += 1
	return lines


static func talk_speak_runs(text_hex: String, text_utf8: String, speaker_hex: String = "93a18de8", speaker_text: String = "藤崎", cmd_kind: int = TALK_CMD_GIRL, first_row: int = 0) -> Dictionary:
	# Exact port of talk_speak_part + talk_render (src/main/scene_library.c:1185, 1340-1346, 1579-1580, 1645-1648):
	# - Speaker at x = -0x68 - (strlen(speaker) * 4) (-120 for 4-byte "藤崎"/"詩織"), y = 48 + first_row * 16
	# - Opening quote "「" (8175) or "（" (8169) at x = -86 (-0x56), y = 48 + first_row * 16
	# - Body lines at x = -72 (-0x48), y = 48 + row * 16 (48, 64, 80)
	var temp_s1 := VN_ROW_Y0 + first_row * VN_ROW_STEP_Y
	var speaker_x := talk_speaker_x_for_hex(speaker_hex)
	var open_hex := TEXT_FULLWIDTH_OPEN_PAREN_HEX if cmd_kind == TALK_CMD_PLAYER_PAREN else TEXT_FULLWIDTH_OPEN_QUOTE_HEX
	var open_text := TEXT_FULLWIDTH_OPEN_PAREN if cmd_kind == TALK_CMD_PLAYER_PAREN else TEXT_FULLWIDTH_OPEN_QUOTE
	var suffix_hex := TEXT_FULLWIDTH_CLOSE_PAREN_HEX if cmd_kind == TALK_CMD_PLAYER_PAREN else ""
	var suffix_text := TEXT_FULLWIDTH_CLOSE_PAREN if cmd_kind == TALK_CMD_PLAYER_PAREN else ""

	var body_lines := talk_render_lines(text_hex, text_utf8, first_row, suffix_hex, suffix_text)
	var runs: Array[Dictionary] = [
		{
			"hex": speaker_hex,
			"position": [speaker_x, temp_s1],
			"attribute": 0,
			"role": "speaker",
		},
		{
			"hex": open_hex,
			"position": [VN_QUOTE_X, temp_s1],
			"attribute": 0,
			"role": "open_quote",
		},
	]
	for line: Dictionary in body_lines:
		runs.append({
			"hex": String(line["bytes_hex"]),
			"position": [int(line["x"]), int(line["y"])],
			"attribute": 0,
			"role": "body",
		})
	return {
		"speaker_text": speaker_text,
		"speaker_hex": speaker_hex,
		"speaker_x": speaker_x,
		"speaker_y": temp_s1,
		"open_quote_text": open_text,
		"open_quote_hex": open_hex,
		"open_quote_x": VN_QUOTE_X,
		"open_quote_y": temp_s1,
		"lines": body_lines,
		"text_runs": runs,
	}


static func hud_text_marker_bob_y(frame_count: int) -> int:
	# Exact port of hud_text_marker_set + hud_text_marker_bob (src/main/hud_screens.c:1740-1768):
	# Starts at y = 0x48 (72), increments by +1 every 4 frames while < 0x4F (79), then resets to 0x48 (72).
	return VN_WAIT_MARKER_Y0 + ((maxi(frame_count, 0) >> 2) % 7)


static func hud_text_marker_actor_spec(frame_count: int = 0, visible: bool = true) -> Dictionary:
	# Exact port of actor_pool[2] configured by hud_text_marker_set (src/main/hud_screens.c:1740-1758)
	# and updated by talk_run (src/main/scene_library.c:2111-2114).
	var marker_y := hud_text_marker_bob_y(frame_count)
	return {
		"actor_index": VN_WAIT_MARKER_ACTOR_ID,
		"priority": VN_WAIT_MARKER_PRIORITY,
		"frame": VN_WAIT_MARKER_FRAME_ID,
		"frame_id": VN_WAIT_MARKER_FRAME_ID,
		"sequence_slot": VN_WAIT_MARKER_SEQUENCE_SLOT,
		"x": VN_WAIT_MARKER_X,
		"y": marker_y,
		"native_x": VN_WAIT_MARKER_X,
		"native_y": marker_y,
		"screen_x": 160 + VN_WAIT_MARKER_X,
		"screen_y": 120 + marker_y,
		"visible": visible,
	}


static func junban_init(mode: int = 0, _rng: NativeRng = null) -> Array[int]:
	# Exact port of junban_init (src/main/scene_library.c:2168-2193):
	# For mode == 0 (command 0x03 normal_date_three_select), talk_choice_order = [0, 1, 2].
	var order: Array[int] = [0, 1, 2]
	if mode == 0:
		return order
	return order


static func build_date_three_choice_prompt(place_id: int = 0, order: Array = [0, 1, 2]) -> Dictionary:
	# Exact port of talk_speak_part case 0x3 (src/main/scene_library.c:1583-1593) +
	# normal_date_three_select_init (src/main/scene_library.c:2196-2222):
	# - talk_render(-0x72, 0x30, text_color, line_buffer, 0, "』", "『", 1):
	#   prefix "『" (8177) at x = -0x80 (-128, screen x = 32), y = 48 + row * 16
	#   option text + "』" (8178) at x = -0x72 (-114, screen x = 46), y = 48 + row * 16
	# - menu_set(1, 3, x=-128, y=48+row*16, w=252, h=16)
	# - menu_bar_show(1): style 0x822F7F2F at Rect2i(32, 168 + row * 16, 252, 16)
	# - menu_cursor_update(1): 0x9C (Frame 156) hand cursor at (120, 56 + row * 16)
	var script_key := 3 if place_id == 3 else 0
	var script: Dictionary = DATE_THREE_CHOICE_SCRIPTS[script_key]
	var options: Array = script["options"]
	var rows: Array[Dictionary] = []
	var runs: Array[Dictionary] = []

	for row_idx in range(3):
		var choice_id := int(order[row_idx]) if row_idx < order.size() else row_idx
		var opt: Dictionary = options[clampi(choice_id, 0, 2)]
		var y := DATE_THREE_SELECT_Y0 + row_idx * DATE_THREE_SELECT_STEP_Y
		var opt_hex := String(opt["bytes_hex"])
		var opt_text := String(opt["text"])
		var body_plus_close_hex := opt_hex + TEXT_CHOICE_CLOSE_HEX
		var body_plus_close_text := opt_text + TEXT_CHOICE_CLOSE
		var full_hex := TEXT_CHOICE_PREFIX_HEX + body_plus_close_hex
		var full_text := TEXT_CHOICE_PREFIX + body_plus_close_text

		var row_dict := {
			"row": row_idx,
			"index": row_idx,
			"choice_id": choice_id,
			"score_class": int(opt["score_class"]),
			"tier": int(opt["tier"]),
			"score_delta": int(opt["score_delta"]),
			"prefix_text": TEXT_CHOICE_PREFIX,
			"prefix_hex": TEXT_CHOICE_PREFIX_HEX,
			"prefix_x": DATE_THREE_SELECT_X,
			"option_text": opt_text,
			"option_bytes_hex": opt_hex,
			"body_plus_close_text": body_plus_close_text,
			"body_plus_close_hex": body_plus_close_hex,
			"text_x": DATE_THREE_SELECT_TEXT_X,
			"text": full_text,
			"bytes_hex": full_hex,
			"x": DATE_THREE_SELECT_X,
			"y": y,
			"w": DATE_THREE_SELECT_W,
			"h": DATE_THREE_SELECT_H,
			"screen_x": 160 + DATE_THREE_SELECT_X,
			"screen_y": 120 + y,
			"bar_rect": [160 + DATE_THREE_SELECT_X, 120 + y, DATE_THREE_SELECT_W, DATE_THREE_SELECT_H],
			"cursor_x": DATE_THREE_SELECT_CURSOR_X,
			"cursor_y": DATE_THREE_SELECT_CURSOR_Y0 + row_idx * DATE_THREE_SELECT_STEP_Y,
			"cursor_screen_x": 160 + DATE_THREE_SELECT_CURSOR_X,
			"cursor_screen_y": 120 + DATE_THREE_SELECT_CURSOR_Y0 + row_idx * DATE_THREE_SELECT_STEP_Y,
		}
		rows.append(row_dict)
		runs.append({
			"hex": TEXT_CHOICE_PREFIX_HEX,
			"position": [DATE_THREE_SELECT_X, y],
			"attribute": 0,
			"role": "choice_prefix",
		})
		runs.append({
			"hex": body_plus_close_hex,
			"position": [DATE_THREE_SELECT_TEXT_X, y],
			"attribute": 0,
			"role": "choice_body",
		})

	return {
		"place_id": place_id,
		"table_vram": String(script["table_vram"]),
		"choice_vram": String(script["choice_vram"]),
		"question_text": String(script["question_text"]),
		"question_bytes_hex": String(script["question_bytes_hex"]),
		"count": 3,
		"order": order.duplicate(),
		"menu_slot": DATE_THREE_SELECT_SLOT,
		"menu_bar_style": DATE_THREE_SELECT_STYLE,
		"rows": rows,
		"text_runs": runs,
	}


static func build_date_choice_response(place_id: int = 0, choice_id: int = 0, speaker_text: String = "藤崎", speaker_hex: String = "93a18de8", player_lastname_text: String = "藤崎", player_lastname_hex: String = "93a18de8") -> Dictionary:
	# Exact port of talk_speak_part case 0x2 (src/main/scene_library.c:1604-1658) +
	# date_choice_score & date_choice_reaction_speak (src/ovl/date/place_scenes.c:3416-3554):
	var script_key := 3 if place_id == 3 else 0
	var script: Dictionary = DATE_THREE_CHOICE_SCRIPTS[script_key]
	var options: Array = script["options"]
	var opt: Dictionary = options[clampi(choice_id, 0, 2)]
	var score_class := int(opt["score_class"])
	var reply_hex := String(opt["reply_bytes_hex"])
	var reply_text := String(opt["reply_text"])
	var speak_payload := talk_speak_runs(reply_hex, reply_text, speaker_hex, speaker_text, TALK_CMD_GIRL_REPLY, 0)

	var reaction: Dictionary = DATE_CHOICE_REACTION_TABLE[clampi(score_class, 0, 4)]
	var reaction_payload := talk_speak_runs(
		String(reaction["bytes_hex"]),
		String(reaction["text"]),
		player_lastname_hex,
		player_lastname_text,
		TALK_CMD_PLAYER_PAREN,
		0
	)

	return {
		"place_id": place_id,
		"choice_id": choice_id,
		"score_class": score_class,
		"tier": int(opt["tier"]),
		"score_delta": int(opt["score_delta"]),
		"reply_vram": String(opt["reply_vram"]),
		"reply_text": reply_text,
		"reply_bytes_hex": reply_hex,
		"speaker_text": speaker_text,
		"speaker_hex": speaker_hex,
		"speaker_x": int(speak_payload["speaker_x"]),
		"speaker_y": int(speak_payload["speaker_y"]),
		"open_quote_x": int(speak_payload["open_quote_x"]),
		"open_quote_y": int(speak_payload["open_quote_y"]),
		"reply_lines": speak_payload["lines"],
		"text_runs": speak_payload["text_runs"],
		"reaction": {
			"score_class": score_class,
			"vram": String(reaction["vram"]),
			"text": String(reaction["text"]),
			"bytes_hex": String(reaction["bytes_hex"]),
			"lines": reaction_payload["lines"],
			"text_runs": reaction_payload["text_runs"],
		},
	}


static func step_date_three_choice(state: HolidayState, pressed: int, result: StepResult, explicit_choice_row: int = -1) -> Dictionary:
	# Exact port of normal_date_three_select_init + normal_date_three_select_main
	# (src/main/scene_library.c:2196-2253) and menu_check(1) (src/main/menu_speaker_girl.c:76-265).
	var place_id := state.chosen_place if state.chosen_place != 0xFF else 0
	var prompt := build_date_three_choice_prompt(place_id, state.date_choice_order)
	var cursor := clampi(state.date_choice_cursor, 0, 2)

	if (pressed & PAD_UP) != 0:
		cursor = (cursor + 2) % 3
		state.date_choice_cursor = cursor
		result.emit("audio_command", {"code": 0x509})
		result.emit("menu_cursor_update", {"slot": DATE_THREE_SELECT_SLOT, "cursor": cursor})
	elif (pressed & PAD_DOWN) != 0:
		cursor = (cursor + 1) % 3
		state.date_choice_cursor = cursor
		result.emit("audio_command", {"code": 0x509})
		result.emit("menu_cursor_update", {"slot": DATE_THREE_SELECT_SLOT, "cursor": cursor})

	result.emit("menu_bar_show", {
		"slot": DATE_THREE_SELECT_SLOT,
		"style": DATE_THREE_SELECT_STYLE,
		"cursor": cursor,
		"rect": Rect2i(32, 168 + cursor * 16, 252, 16),
		"cursor_pos": Vector2i(DATE_THREE_SELECT_CURSOR_X, DATE_THREE_SELECT_CURSOR_Y0 + cursor * DATE_THREE_SELECT_STEP_Y),
	})

	var summary := {
		"cursor": cursor,
		"confirmed": false,
		"prompt": prompt,
	}

	var chosen_row := explicit_choice_row
	if chosen_row < 0 and (pressed & PAD_CONFIRM) != 0:
		chosen_row = cursor

	if chosen_row >= 0:
		chosen_row = clampi(chosen_row, 0, 2)
		state.date_choice_cursor = chosen_row
		var choice_id := int(state.date_choice_order[chosen_row]) if chosen_row < state.date_choice_order.size() else chosen_row
		state.date_chosen_option = choice_id
		var response := build_date_choice_response(place_id, choice_id)
		state.date_score_class = int(response["score_class"])
		state.date_score += int(response["score_delta"])
		state.date_step += 1
		state.date_substep = 0
		state.date_line_wait = 1
		result.emit("audio_command", {"code": 0x50A})
		result.emit("menu_bar_hide", {"slot": DATE_THREE_SELECT_SLOT})
		result.emit("date_choice_confirmed", {
			"selected_row": chosen_row,
			"choice_id": choice_id,
			"score_class": state.date_score_class,
			"date_score": state.date_score,
			"response": response,
		})
		summary["confirmed"] = true
		summary["selected_row"] = chosen_row
		summary["choice_id"] = choice_id
		summary["score_class"] = state.date_score_class
		summary["date_score"] = state.date_score
		summary["response"] = response

	return summary


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

	state.game_mode = GAME_MODE_DATE
	state.date_stage = 0
	state.date_step = 0
	state.date_substep = 0
	state.date_choice_cursor = 0
	state.date_chosen_option = -1
	state.date_choice_order = junban_init(0)
	state.date_line_wait = 1

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
