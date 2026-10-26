class_name TelephoneOverlay
extends RefCounted
## T-29018: Port of TEL overlay (src/ovl/telephone/)
## Handles:
## 1. GAME_MODE_TELEPHONE_PROFILE (0x3E / 0x45) Stage 0 (Yoshio Saotome profile notebook & appraisal)
## 2. GAME_MODE_TELEPHONE_CALL (0x3D / 0x43) & Profile Stage 1:
##    - Call connection, ringing, answering machine, busy, pickup
##    - Date Location menu (phone_date_place_availability_build)
##    - Date Day menu (phone_available_date_slots_build)
##    - Girl invitation acceptance evaluation & appointment booking into game_state.unk_54C[0..7]

const HolidayAppointmentRules = preload("res://rules/holiday_appointment.gd")
const Math = preload("res://core/native_math.gd")
const NativeRng = preload("res://core/native_rng.gd")

# Game mode constants
const GAME_MODE_TELEPHONE_CALL := 0x3D
const GAME_MODE_TELEPHONE_PROFILE := 0x3E
const GAME_MODE_TELEPHONE_LEGACY := 0x43
const GAME_MODE_TELEPHONE_PROFILE_LEGACY := 0x45
const GAME_MODE_HOLIDAY := 0x46
const GAME_MODE_DATE := 0x50

# Girls (0..12)
const GIRL_SHIORI := 0
const GIRL_MIO := 1
const GIRL_YUINA := 2
const GIRL_AYAKO := 3
const GIRL_SAKI := 4
const GIRL_YUKARI := 5
const GIRL_NOZOMI := 6
const GIRL_MIRA := 7
const GIRL_YUKO := 8
const GIRL_MEGUMI := 9
const GIRL_YOSHIO := 10
const GIRL_YUMI := 11
const GIRL_REI := 12

# 21 Date spot identifiers (place_name_table)
const PLACE_NEARBY_PARK := 0        # 近所の公園
const PLACE_CENTRAL_PARK := 1       # きらめき中央公園
const PLACE_SHOPPING_STREET := 2    # ショッピング街
const PLACE_AQUARIUM := 3           # 水族館
const PLACE_ZOO := 4                # 動物園
const PLACE_BOTANICAL_GARDEN := 5   # 植物園
const PLACE_PLANETARIUM := 6        # プラネタリウム
const PLACE_ART_MUSEUM := 7         # 美術館
const PLACE_LIBRARY := 8            # 図書館
const PLACE_GAME_CENTER := 9        # ゲームセンター
const PLACE_BOWLING_ALLEY := 10     # ボーリング場
const PLACE_KARAOKE := 11           # カラオケ屋
const PLACE_AMUSEMENT_PARK := 12    # 遊園地
const PLACE_STADIUM := 13           # スタジアム
const PLACE_MOVIE_THEATER := 14     # 映画館
const PLACE_CONCERT_HALL := 15      # コンサート会場
const PLACE_POOL := 16              # プール (Summer)
const PLACE_BEACH := 17             # 海 (Summer)
const PLACE_SHRINE := 18            # 神社 (Summer festival / New Year)
const PLACE_SKATE_RINK := 19        # スケート場 (Winter)
const PLACE_SKI_RESORT := 20        # スキー場 (Winter)

const PLACE_NAMES: Array[String] = [
	"近所の公園",
	"きらめき中央公園",
	"ショッピング街",
	"水族館",
	"動物園",
	"植物園",
	"プラネタリウム",
	"美術館",
	"図書館",
	"ゲームセンター",
	"ボーリング場",
	"カラオケ屋",
	"遊園地",
	"スタジアム",
	"映画館",
	"コンサート会場",
	"プール",
	"海",
	"神社",
	"スケート場",
	"スキー場"
]

const GIRL_NAMES: Array[String] = [
	"藤崎詩織", "如月未緒", "紐緒結奈", "片桐彩子", "虹野沙希",
	"古式ゆかり", "清川望", "鏡魅羅", "朝日奈夕子", "美樹原愛",
	"早乙女好雄", "早乙女優美", "伊集院レイ"
]

const GIRL_SHORT_NAMES: Array[String] = [
	"詩織", "未緒", "結奈", "彩子", "沙希",
	"ゆかり", "望", "魅羅", "夕子", "愛",
	"好雄", "優美", "レイ"
]

# Sign threshold day of month (phone_profile_sign_day_thresholds)
# 12 entries for Jan..Dec
const SIGN_DAY_THRESHOLDS: Array[int] = [
	20, 19, 21, 20, 21, 22, 23, 23, 23, 24, 23, 22
]

const SIGN_NAMES: Array[String] = [
	"やぎ座", "みずがめ座", "うお座", "おひつじ座", "おうし座", "ふたご座",
	"かに座", "しし座", "おとめ座", "てんびん座", "さそり座", "いて座"
]

const BLOOD_TYPE_NAMES: Array[String] = ["Ａ型", "Ｂ型", "Ｏ型", "ＡＢ型"]

const CLUB_NAMES: Array[String] = [
	"文芸部", "演劇部", "科学部", "美術部", "吹奏楽部", "家庭科部",
	"水泳部", "陸上部", "野球部", "サッカー部", "テニス部", "パソコン部", "帰宅部"
]

# Girl profile data [month, day, blood_type_idx, club_idx, bwh_str, notes_l1, notes_l2]
const GIRL_PROFILES: Dictionary = {
	GIRL_SHIORI: {
		"birthday_month": 5, "birthday_day": 27, "blood": 0, "club": 12,
		"bwh": "83 - 56 - 84", "notes1": "容姿端麗、成績優秀な", "notes2": "パーフェクトな幼なじみ。"
	},
	GIRL_MIO: {
		"birthday_month": 2, "birthday_day": 3, "blood": 0, "club": 0,
		"bwh": "80 - 59 - 82", "notes1": "本が大好きで気が弱く", "notes2": "病弱な女の子。"
	},
	GIRL_YUINA: {
		"birthday_month": 7, "birthday_day": 7, "blood": 1, "club": 2,
		"bwh": "84 - 58 - 84", "notes1": "世界征服を企む", "notes2": "マッドサイエンティスト。"
	},
	GIRL_AYAKO: {
		"birthday_month": 9, "birthday_day": 29, "blood": 1, "club": 3,
		"bwh": "86 - 59 - 87", "notes1": "英語交じりで話す", "notes2": "元気な芸術家肌。"
	},
	GIRL_SAKI: {
		"birthday_month": 10, "birthday_day": 25, "blood": 0, "club": 8,
		"bwh": "82 - 57 - 83", "notes1": "根性で何事もやり遂げる", "notes2": "世話好きなマネージャー。"
	},
	GIRL_YUKARI: {
		"birthday_month": 12, "birthday_day": 11, "blood": 1, "club": 10,
		"bwh": "80 - 58 - 83", "notes1": "おっとりした箱入り娘の", "notes2": "お嬢様。"
	},
	GIRL_NOZOMI: {
		"birthday_month": 12, "birthday_day": 3, "blood": 0, "club": 6,
		"bwh": "83 - 58 - 83", "notes1": "男勝りな水泳少女。", "notes2": "実は恋に臆病。"
	},
	GIRL_MIRA: {
		"birthday_month": 11, "birthday_day": 15, "blood": 2, "club": 12,
		"bwh": "90 - 60 - 89", "notes1": "親衛隊を持つ美貌の女王様。", "notes2": "素顔は家庭的で弟思い。"
	},
	GIRL_YUKO: {
		"birthday_month": 10, "birthday_day": 17, "blood": 1, "club": 12,
		"bwh": "85 - 60 - 86", "notes1": "お調子者で流行り物好き。", "notes2": "いつも明るいムードメーカー。"
	},
	GIRL_MEGUMI: {
		"birthday_month": 9, "birthday_day": 5, "blood": 0, "club": 12,
		"bwh": "81 - 57 - 82", "notes1": "人見知りで恥ずかしがり屋。", "notes2": "動物が大好きな女の子。"
	},
	GIRL_YUMI: {
		"birthday_month": 1, "birthday_day": 1, "blood": 1, "club": 12,
		"bwh": "78 - 56 - 80", "notes1": "好雄の妹。天真爛漫で", "notes2": "元気いっぱいの後輩。"
	},
	GIRL_REI: {
		"birthday_month": 8, "birthday_day": 23, "blood": 3, "club": 13,
		"bwh": "84 - 58 - 84", "notes1": "容姿端麗な伊集院家跡取り。", "notes2": "謎に包まれた学園の貴公子。"
	}
}

# phone_invitation_base_adjustments: 11 rows of 21 bytes (from TEL_data.data.s:1040-1275)
const PHONE_INVITATION_BASE_ADJUSTMENTS: Array[Array] = [
	# 0: Shiori
	[0xCC, 0xB3, 0xB3, 0xB3, 0xB3, 0x80, 0xB3, 0x80, 0x80, 0x66, 0x80, 0x80, 0xB3, 0x66, 0xB3, 0xB3, 0xB3, 0xCC, 0xCC, 0x80, 0xCC],
	# 1: Mio
	[0x80, 0xCC, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xCC, 0x4C, 0x66, 0x66, 0x80, 0xB3, 0xCC, 0x80, 0x66, 0x66, 0xCC, 0x80, 0x00],
	# 2: Yuina
	[0x66, 0x80, 0x66, 0x80, 0xB3, 0xB3, 0xB3, 0x80, 0xB3, 0x80, 0x80, 0x66, 0x66, 0x4C, 0xB3, 0x80, 0x80, 0x80, 0x80, 0x66, 0x00],
	# 3: Ayako
	[0x80, 0xB3, 0x80, 0x80, 0xB3, 0x80, 0xB3, 0xCC, 0x66, 0x80, 0xB3, 0xCC, 0x80, 0x80, 0x80, 0xB3, 0x26, 0x26, 0xB3, 0xB3, 0x00],
	# 4: Saki
	[0xB3, 0xB3, 0x80, 0xB3, 0x66, 0x66, 0xB3, 0x66, 0x80, 0x80, 0xB3, 0x66, 0x80, 0xCC, 0x80, 0x80, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3],
	# 5: Yukari
	[0xB3, 0xCC, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0x4C, 0x66, 0x66, 0xB3, 0xB3, 0xB3, 0x80, 0xB3, 0xB3, 0xCC, 0x80, 0x80],
	# 6: Nozomi
	[0x66, 0x80, 0xB3, 0xB3, 0x80, 0xB3, 0x80, 0x80, 0x66, 0x80, 0xB3, 0x80, 0x80, 0x66, 0xB3, 0xB3, 0xCC, 0xCC, 0xB3, 0xCC, 0xCC],
	# 7: Mira
	[0x4C, 0x66, 0x80, 0x80, 0x66, 0x66, 0x80, 0x80, 0x66, 0xB3, 0x66, 0x80, 0x80, 0x66, 0xB3, 0xB3, 0xB3, 0xCC, 0xB3, 0x66, 0xCC],
	# 8: Yuko
	[0x4C, 0x66, 0xB3, 0xB3, 0xB3, 0x80, 0x80, 0x80, 0x66, 0xB3, 0xB3, 0xCC, 0xB3, 0x66, 0x80, 0xCC, 0xB3, 0xCC, 0xCC, 0x80, 0xB3],
	# 9: Megumi
	[0xB3, 0xB3, 0xCC, 0xB3, 0xCC, 0xB3, 0xCC, 0x66, 0x66, 0xB3, 0x80, 0x66, 0xB3, 0x66, 0xB3, 0xB3, 0x80, 0x80, 0xB3, 0x80, 0x00],
	# 10: Yumi
	[0x80, 0xB3, 0xB3, 0xB3, 0xB3, 0x66, 0x80, 0x4C, 0x4C, 0xCC, 0xB3, 0xB3, 0xCC, 0xB3, 0xB3, 0x80, 0xCC, 0xCC, 0xCC, 0xB3, 0xCC]
]

# ---------------------------------------------------------------------------
# State Container
# ---------------------------------------------------------------------------

class TelephoneSessionState:
	var game_mode: int = GAME_MODE_TELEPHONE_PROFILE
	var mode_stage: int = 0
	var mode_step: int = 0
	var substep: int = 0
	var mode_step_timer: int = 0
	var current_girl: int = GIRL_SHIORI
	var selected_target_girl: int = GIRL_SHIORI
	var menu_cursor_0: int = 0
	var menu_cursor_1: int = 0

	# Date invitation selections
	var invitation_place: int = PLACE_NEARBY_PARK
	var invitation_year: int = 95
	var invitation_month: int = 4
	var invitation_day: int = 9
	var invitation_result: String = "" # "accepted", "refused_soft", "refused_med", "refused_hard", "answering_machine"

	# Girl appraisal cached state (for Yoshio's notebook)
	var appraisal_list: Array[Dictionary] = [] # [{girl: int, met: bool, stars: int, bomb: bool, blink: int}]
	var available_places_page0: Array[int] = []
	var available_places_page1: Array[int] = []
	var available_date_slots: Array[Dictionary] = [] # [{month: int, day: int}]

	# Dialogue lines
	var dialogue_speaker: String = ""
	var dialogue_lines: Array[String] = []


# ---------------------------------------------------------------------------
# 1. Profile & Appraisal Rules (Stage 0)
# ---------------------------------------------------------------------------

## Calculates appraisal rating (1..6 stars, or 0 if points >= 500) and syoushin bomb indicator
static func calculate_girl_appraisal(girl_index: int, met: bool, rank: int, points: int, syoushin: int) -> Dictionary:
	if not met:
		return {"met": false, "stars": 0, "bomb": false, "blink": 0}

	var stars: int = 1
	if points >= 500:
		stars = 0
	elif rank < 6:
		stars = 6
	elif rank < 11:
		stars = 5
	elif rank < 21:
		stars = 4
	elif rank < 41:
		stars = 3
	elif rank < 61:
		stars = 2
	else:
		stars = 1

	var bomb: bool = (syoushin >= 60)
	var blink_period: int = 0
	if bomb:
		blink_period = -(int(abs(syoushin - 60)) / 10) * 2 + 8
		if blink_period <= 0:
			blink_period = 1

	return {
		"met": true,
		"stars": stars,
		"bomb": bomb,
		"blink": blink_period
	}

## Calculates zodiac sign name from month and day
static func get_zodiac_sign(month: int, day: int) -> String:
	var m_idx: int = (month - 1) % 12
	var v: int = m_idx
	if day > SIGN_DAY_THRESHOLDS[m_idx]:
		v = (v + 1) % 12
	return SIGN_NAMES[v]

## Builds full appraisal list for all 11 girls
static func build_full_appraisal_list(girls_data: Array[Dictionary]) -> Array[Dictionary]:
	var result: Array[Dictionary] = []
	for i in range(11):
		var g: Dictionary = girls_data[i] if i < girls_data.size() else {}
		var met: bool = bool(g.get("met", false))
		var rank: int = int(g.get("rank", 100))
		var points: int = int(g.get("points", 0))
		var syoushin: int = int(g.get("syoushin", 0))
		var app: Dictionary = calculate_girl_appraisal(i, met, rank, points, syoushin)
		app["girl_id"] = i
		app["name"] = GIRL_NAMES[i]
		result.append(app)
	return result


# ---------------------------------------------------------------------------
# 2. Date Place Availability Rules (Page 0 & Page 1)
# ---------------------------------------------------------------------------

## Returns vacation kind (1: summer, 2: winter, 0: ordinary)
static func get_vacation_kind(month: int, day: int) -> int:
	if month == 7 and day >= 21:
		return 1
	if month == 8:
		return 1
	if month == 9 and day <= 1:
		return 1
	if month == 12 and day >= 24:
		return 2
	if month == 1 and day <= 7:
		return 2
	return 0

## Evaluates availability of all 21 date spots for a given date and game state
static func build_date_place_availability(page: int, year: int, month: int, day: int,
		progress_state: Array, quarter_flags: Dictionary, current_month: int) -> Array[int]:
	var avail: Array[int] = []
	avail.resize(11)
	for i in range(11):
		avail[i] = 0
	avail[10] = 0x64 # Page flip is always available

	if page == 0:
		for i in range(10):
			var prog: int = progress_state[i] if i < progress_state.size() else 0
			if prog & 0x7F:
				avail[i] = 0x74
			else:
				avail[i] = 0
				match i:
					1: # Central Park (きらめき中央公園)
						if quarter_flags.get("b1", 0) == 1 and year == 95 and month == 6:
							avail[i] = 0x74
					3: # Aquarium (水族館)
						if quarter_flags.get("b3", 0) == 1 and year == 95 and month == 12:
							avail[i] = 0x74
					5: # Botanical Garden (植物園)
						if quarter_flags.get("b5", 0) == 1 and year == 96 and month == 6:
							avail[i] = 0x74
					6: # Planetarium (プラネタリウム)
						if quarter_flags.get("b7", 0) == 1 and year == 96 and month == 12:
							avail[i] = 0x74
					7: # Art Museum (美術館)
						if quarter_flags.get("b2", 0) == 1 and year == 95 and month == 9:
							avail[i] = 0x74

	else: # page == 1
		for i in range(6):
			var prog_idx: int = i + 10
			var prog: int = progress_state[prog_idx] if prog_idx < progress_state.size() else 0
			if prog & 0x7F:
				if i < 3: # Bowling (10), Karaoke (11), Amusement Park (12)
					avail[i] = 0x74
				elif i in [3, 4, 5]: # Stadium (13), Movie (14), Concert (15)
					if i == 3 and month in [12, 2, 0, 14]:
						avail[i] = 0 # Off-season for Stadium
					else:
						if month == current_month:
							if quarter_flags.get("b14", 0) == 1:
								avail[i] = 0x74
						elif current_month % 3 == 2:
							if quarter_flags.get("b13", 0) == 1:
								avail[i] = 0x74
						elif quarter_flags.get("b14", 0) == 1:
							avail[i] = 0x74
			else:
				match i:
					0: # Bowling (10)
						if quarter_flags.get("b6", 0) == 1 and year == 96 and month >= 9:
							avail[i] = 0x74
					1: # Karaoke (11)
						if quarter_flags.get("b8", 0) == 1 and year == 97 and month >= 3:
							avail[i] = 0x74
					3: # Stadium (13)
						if quarter_flags.get("b13", 0) == 1 and current_month == 2 and month == 3 and year == 96:
							avail[i] = 0x74
					4: # Movie (14)
						if quarter_flags.get("b13", 0) == 1 and year == 95 and month == 6 and current_month == 5:
							avail[i] = 0x74

		# Seasonal & Special spots (slots 6..9 on page 1)
		var vac: int = get_vacation_kind(month, day)
		# Slot 6: Pool in summer (vac==1), Skating in winter (vac==2)
		if vac == 1:
			avail[6] = 0x74 # Pool (16)
		elif vac == 2:
			avail[6] = 0x74 # Skating (19)
		else:
			avail[6] = 0

		# Slot 7: Sea in summer (vac==1), Skiing in winter (vac==2)
		if vac == 2:
			avail[7] = 0x74 # Skiing (20)
		elif vac == 1:
			avail[7] = 0x74 # Sea/Beach (17)
		else:
			avail[7] = 0

		# Slot 8: Unused
		avail[8] = 0

		# Slot 9: Shrine (18) during first week of August festival
		if quarter_flags.get("b15", 0) == 1 and month == 8 and ((day + 6) / 7 == 1):
			avail[9] = 0x74
		else:
			avail[9] = 0

	return avail


# ---------------------------------------------------------------------------
# 3. Available Date Day Slots (Upcoming 28 Days)
# ---------------------------------------------------------------------------

## Builds available date candidates (Sundays and holidays within 28 days with no existing appointment)
static func build_available_date_slots(start_year: int, start_month: int, start_day: int,
		month0_calendar: Dictionary, month1_calendar: Dictionary, existing_appointments: Array) -> Array[Dictionary]:
	var candidate_day: int = start_day
	var available_slots: Array[Dictionary] = []
	var month_offset: int = 0
	var days_in_cur_month: int = int(month0_calendar.get("days", 30))
	var day_kinds_0: Array = month0_calendar.get("day_kind", [])
	var day_kinds_1: Array = month1_calendar.get("day_kind", [])

	for step in range(28):
		candidate_day += 1
		if candidate_day > days_in_cur_month:
			candidate_day = 1
			month_offset = 1

		var active_month: int = start_month + month_offset
		var active_year: int = start_year
		if active_month > 12:
			active_month = 1
			active_year += 1

		var day_kind: int = 0
		if month_offset == 0:
			day_kind = day_kinds_0[candidate_day] if candidate_day < day_kinds_0.size() else 0
		else:
			day_kind = day_kinds_1[candidate_day] if candidate_day < day_kinds_1.size() else 0

		# Check if Sunday (1) or Holiday (2)
		if day_kind == 1 or day_kind == 2:
			if HolidayAppointmentRules.hud_appointment_find_today(active_year, active_month, candidate_day, existing_appointments) == 0:
				available_slots.append({
					"year": active_year,
					"month": active_month,
					"day": candidate_day,
					"month_offset": month_offset,
					"day_kind": day_kind
				})

	return available_slots


# ---------------------------------------------------------------------------
# 4. Invitation Acceptance Evaluation & Appointment Booking
# ---------------------------------------------------------------------------

## Resolves girl invitation response based on trivia, closeness, looks, place preference, and RNG
static func evaluate_invitation(girl_id: int, place_id: int, trivia_param: int, looks_param: int,
		closeness: int, rng: Variant = null) -> Dictionary:
	var lim: int = closeness >> 2
	var hp: int = trivia_param / 4
	var total: int = hp if hp >= lim else lim

	var base_adj: int = 0
	if girl_id >= 0 and girl_id < PHONE_INVITATION_BASE_ADJUSTMENTS.size():
		var row: Array = PHONE_INVITATION_BASE_ADJUSTMENTS[girl_id]
		if place_id >= 0 and place_id < row.size():
			base_adj = int(row[place_id])

	if base_adj != 0:
		total += base_adj
	else:
		total = 0

	if looks_param < 40:
		total = (looks_param + total) - 40

	var rnd: int = 0
	if rng is NativeRng:
		rnd = rng.next("phone_date_invitation_resolve") & 0xFF
	elif rng is Object and rng.has_method("rand"):
		rnd = rng.rand() & 0xFF
	elif rng is Object and rng.has_method("next"):
		rnd = rng.next("phone_date_invitation_resolve") & 0xFF
	else:
		rnd = randi() & 0xFF
	var accepted: bool = (rnd < total)
	var outcome: String = "accepted"

	if not accepted:
		var delta: int = total - rnd
		if delta < 0x66: # < 102
			outcome = "refused_soft"
		elif delta < 0xB3: # < 179
			outcome = "refused_med"
		else:
			outcome = "refused_hard"

	return {
		"accepted": accepted,
		"outcome": outcome,
		"total": total,
		"rnd": rnd,
		"base_adj": base_adj
	}

## Adds an appointment entry into the first inactive slot of unk_54C[0..7] and sorts
static func hud_appointment_add(appointment_entry: int, appointment_words: Array) -> bool:
	var insertion_slot_index: int = 0
	while insertion_slot_index < 8:
		var w: int = int(appointment_words[insertion_slot_index])
		var active: int = (w >> 29) & 1
		if active == 0:
			break
		insertion_slot_index += 1
	if insertion_slot_index >= 8:
		return false
	appointment_words[insertion_slot_index] = appointment_entry
	hud_appointment_list_sort(appointment_words)
	return true

## Packs active schedule entries to the front, clears the rest, and sorts by month * 100 + day
static func hud_appointment_list_sort(appointment_words: Array, cur_month: int = 4) -> void:
	var active_entries: Array[int] = []
	for j in range(8):
		var w: int = int(appointment_words[j])
		if ((w >> 29) & 1) == 1:
			active_entries.append(w)
	for j in range(8):
		if j < active_entries.size():
			appointment_words[j] = active_entries[j]
		else:
			appointment_words[j] = 0

	var active_count: int = active_entries.size()
	for i in range(active_count):
		var earliest_idx: int = i
		var earliest_key: int = 999999
		for j in range(i, active_count):
			var w: int = int(appointment_words[j])
			var unpacked: Dictionary = HolidayAppointmentRules.unpack_appointment_word(w)
			var m: int = int(unpacked["month"])
			if cur_month == 12 and m == 1:
				m = 13
			var d: int = int(unpacked["day"])
			var key: int = m * 100 + d
			if key < earliest_key:
				earliest_key = key
				earliest_idx = j
		if earliest_idx != i:
			var tmp: int = appointment_words[i]
			appointment_words[i] = appointment_words[earliest_idx]
			appointment_words[earliest_idx] = tmp

## Books an accepted appointment into game_state.unk_54C[0..7] using TelDateEntry bitfield packing
static func book_appointment(appointment_words: Array, girl_id: int, place_id: int,
		year: int, month: int, day: int) -> bool:
	var raw := {
		"place": place_id,
		"year": year,
		"month": month,
		"day": day,
		"flag": 0,
		"girl": girl_id,
		"active": 1,
		"pad2": 0
	}
	var word: int = HolidayAppointmentRules.pack_appointment_word(raw)
	return hud_appointment_add(word, appointment_words)
