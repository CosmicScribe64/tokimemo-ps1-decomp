extends RefCounted
## T-29013: Rules, state machines, and mathematical models for:
## 1. Calendar Screen (icon 12; home_screens.c, hud_screens.c)
## 2. Holiday Phone Book Screen (icon 8; holiday.c::holiday_tel)
## 3. Holiday Magazine Screen (icon 10; holiday.c::holiday_mode_stage_magazine, hud_screens.c)

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")

# Biorhythm birth phase table (birthday_day_table in hud_screens.c:1710, u16[13])
const BIRTHDAY_DAY_TABLE: Array[int] = [
	0, 95, 64, 35, 370, 340, 309, 279, 248, 217, 187, 156, 126
]

# Girl phone digits (13 rows of 7 digits, src/main/holiday.c:1307-1309, D_800B3778)
const GIRL_PHONE_DIGITS: Array[Array] = [
	[0, 1, 2, 1, 0, 2, 0], # Girl 0: Shiori Fujisaki
	[1, 1, 2, 0, 2, 0, 1], # Girl 1: Mio Kisaragi
	[2, 2, 2, 0, 0, 1, 0], # Girl 2: Yuina Himo
	[2, 1, 0, 1, 0, 1, 0], # Girl 3: Ayako Katagiri
	[2, 1, 0, 2, 2, 1, 1], # Girl 4: Saki Nijino
	[0, 0, 0, 1, 2, 2, 1], # Girl 5: Yukari Koshiki
	[2, 1, 1, 0, 1, 2, 1], # Girl 6: Nozomi Kiyokawa
	[2, 2, 1, 0, 1, 1, 2], # Girl 7: Mira Kagami
	[0, 2, 0, 1, 2, 0, 2], # Girl 8: Yuko Asahina
	[0, 1, 2, 2, 1, 0, 0], # Girl 9: Megumi Mikihara
	[0, 0, 0, 2, 2, 1, 0], # Girl 10 (Yoshio Saotome): 0x800B37BE
	[0, 0, 2, 1, 2, 0, 1], # Girl 11: Yumi Saotome
	[2, 1, 0, 0, 2, 1, 0], # Girl 12: Miharu / Ray
]

# Contact display names (girl_full_name_table, D_800B3538)
const CONTACT_NAMES: Array[String] = [
	"早乙女好雄",   # Slot 0: Yoshio Saotome (always present)
	"藤崎詩織",     # Slot 1: Girl 0 (Shiori)
	"如月未緒",     # Slot 2: Girl 1 (Mio)
	"紐緒結奈",     # Slot 3: Girl 2 (Yuina)
	"片桐彩子",     # Slot 4: Girl 3 (Ayako)
	"虹野沙希",     # Slot 5: Girl 4 (Saki)
	"古式ゆかり",   # Slot 6: Girl 5 (Yukari)
	"清川望",       # Slot 7: Girl 6 (Nozomi)
	"鏡魅羅",       # Slot 8: Girl 7 (Mira)
	"朝日奈夕子",   # Slot 9: Girl 8 (Yuko)
	"美樹原愛",     # Slot 10: Girl 9 (Megumi)
	"早乙女優美",   # Slot 11: Girl 10 (Yumi)
	"伊集院レイ",   # Slot 12: Ray Ijuin (always present)
	"取り消し",     # Slot 13: Cancel (always present)
]

# Precomputed GTE quarter sine table (1025 entries, 0..0x400)
static var _sine_quarter: PackedInt32Array = _init_sine_table()

static func _init_sine_table() -> PackedInt32Array:
	var table := PackedInt32Array()
	table.resize(1025)
	for i in range(1025):
		table[i] = int(round(sin(i * TAU / 4096.0) * 4096.0))
	return table

# PS1 libgte rsin(angle): 4096 angle steps = 360 degrees, returns fixed-point -4096..4096
static func rsin(angle: int) -> int:
	var neg := false
	if angle < 0:
		angle = -angle
		neg = true
	angle = angle & 0xFFF
	var value: int
	if angle < 0x401:
		value = _sine_quarter[angle]
	elif angle < 0x801:
		value = _sine_quarter[0x800 - angle]
	elif angle < 0xC01:
		value = -_sine_quarter[angle - 0x800]
	else:
		value = -_sine_quarter[0x1000 - angle]
	return -value if neg else value

# Calculate birthday phase offset (hud_screens.c:1708 hud_biorhythm_birth_offset)
static func biorhythm_birth_offset(player_birthday_word: int) -> int:
	var month: int = player_birthday_word & 0xF
	var day: int = (player_birthday_word >> 4) & 0x1F
	var offset: int = -BIRTHDAY_DAY_TABLE[month] - day + 0x1563
	if month == 4 and day < 4:
		offset = -BIRTHDAY_DAY_TABLE[3] - day + 0x1582
	return offset

# Calculate biorhythm wave value for a day (hud_screens.c:1631 hud_biorhythm_value)
# scale > 0: average strength * scale / 4096
# scale < 0: first selected wave * scale / 4096
static func biorhythm_value(day_count: int, player_birthday_word: int,
		inc_23: bool, inc_28: bool, inc_33: bool, scale: int) -> int:
	var birth_offset: int = biorhythm_birth_offset(player_birthday_word)
	var bio_day: int = birth_offset + (day_count & 0xFFFF)
	var wave_23: int = rsin(bio_day * 0xB2)
	var wave_28: int = rsin(bio_day * 0x92)
	var wave_33: int = rsin(bio_day * 0x7C)

	if scale < 0:
		if inc_23:
			return int(wave_23 * scale / 4096)
		if inc_28:
			return int(wave_28 * scale / 4096)
		if inc_33:
			return int(wave_33 * scale / 4096)
		return 0

	var selected_count: int = 0
	var combined_total: int = 0
	var critical_count: int = 0

	if inc_23:
		selected_count += 1
		if abs(wave_23) < 0x199:
			combined_total -= 0x1000
			critical_count += 1
		elif wave_23 > 0:
			combined_total += wave_23
		else:
			combined_total -= wave_23 / 2

	if inc_28:
		selected_count += 1
		if abs(wave_28) < 0x199:
			combined_total -= 0x1000
			critical_count += 1
		elif wave_28 > 0:
			combined_total += wave_28
		else:
			combined_total -= wave_28 / 2

	if inc_33:
		selected_count += 1
		if abs(wave_33) < 0x199:
			combined_total -= 0x1000
			critical_count += 1
		elif wave_33 > 0:
			combined_total += wave_33
		else:
			combined_total -= wave_33 / 2

	if selected_count == 0:
		return 1

	if scale > 0:
		if combined_total / selected_count < 0:
			return 0
		return int((combined_total * scale / 4096) / selected_count)

	# scale == 0: critical probe
	if selected_count == 3:
		return -1 if critical_count >= 2 else 1
	if critical_count != 0:
		return -1
	return 1

# Calendar cell coordinates (hud_screens.c:522 calendar_cell_x, calendar_cell_y)
# month_side: 0 = this month, 1 = next month
# day_index: 0 = 1st of month, 1 = 2nd of month, etc.
# first_weekday: day of week of the 1st of the month (0=Sun..6=Sat)
static func calendar_cell_pos(month_side: int, day_index: int, first_weekday: int) -> Vector2i:
	var v: int = first_weekday
	if v == 6:
		v -= 7
	var x: int = month_side * 0x88 + ((v + day_index + 1) % 7) * 16 - 0x78
	var y: int = ((v + day_index + 1) / 7) * 16 - 0x30
	return Vector2i(x, y)

# Build calendar marks matching calendar_marks_build() in hud_screens.c:358
static func build_calendar_marks(year: int, month: int, player_birthday: int,
		girls: Array, appointments: Array, event_log: Array, player_flags: int) -> Array[Dictionary]:
	var marks: Array[Dictionary] = []
	var p_month: int = player_birthday & 0xF
	var p_day: int = (player_birthday >> 4) & 0x1F

	# 1. Player birthday (frame 0x6E = 110)
	if p_month == month:
		marks.append({"frame": 0x6E, "side": 0, "day_index": p_day - 1, "kind": "player_birthday"})
	if p_month == (month + 1):
		marks.append({"frame": 0x6E, "side": 1, "day_index": p_day - 1, "kind": "player_birthday"})

	# 2. Girls birthdays (frame 0x6D = 109, if met / b6 flag set)
	for i in range(mini(girls.size(), 11)):
		var g: Dictionary = girls[i]
		var b_word: int = int(g.get("birthday", 0))
		var g_month: int = b_word & 0xF
		var g_day: int = (b_word >> 4) & 0x1F
		var known_birthday: bool = bool(int(g.get("flags", 0)) & 0x40) # b6
		if known_birthday:
			if g_month == month:
				marks.append({"frame": 0x6D, "side": 0, "day_index": g_day - 1, "girl": i})
			if g_month == month + 1 or (month + 1 == g_month + 12):
				marks.append({"frame": 0x6D, "side": 1, "day_index": g_day - 1, "girl": i})

	# 3. Scheduled appointments (frame 0x62 = 98)
	for app: Variant in appointments:
		if not app is Dictionary or not app.get("active", false):
			continue
		var a_month: int = int(app.get("month", 0))
		var a_day: int = int(app.get("day", 0))
		if a_month == month:
			marks.append({"frame": 0x62, "side": 0, "day_index": a_day - 1, "kind": "appointment"})
		if a_month == ((month % 12) + 1):
			marks.append({"frame": 0x62, "side": 1, "day_index": a_day - 1, "kind": "appointment"})

	return marks

# Calendar Screen State Step
# Handles settings_command_screen_mode_stage_calendar() in home_screens.c:644
static func step_calendar(state: State, input: InputFrame, result: Session.FrameResult) -> void:
	match state.mode_step:
		0:
			# Init: sets title, caption, marks, switch icons off
			state.mode_step = 1
			state.mode_step_timer = 0
			result.presentation.append({
				"kind": "calendar_init",
				"title": "カレンダー",
				"caption": "今月／来月の予定と実績",
			})
		1:
			# Wait for CROSS button to exit
			state.mode_step_timer += 1
			if (input.pressed & 0x40) != 0: # PAD_CROSS
				state.mode_step = 2
		2:
			# Exit: return to previous menu stage
			state.mode_stage = 0
			state.mode_step = 2
			result.transitions.append({
				"kind": "calendar_exit",
				"target_stage": 0,
			})

# Holiday Phone Book State Step
# Handles holiday.c::holiday_tel() and telephone_class_init()
static func step_holiday_tel(state: State, input: InputFrame, result: Session.FrameResult,
		phone_state: Dictionary) -> void:
	var data: Dictionary = state.rule_state
	var contacts: Array[Dictionary] = build_telephone_directory(data.get("girls", []))
	var cursor: int = int(phone_state.get("cursor", 0))

	match state.mode_step:
		0:
			# Init: telephone_class_init
			phone_state["cursor"] = 0
			phone_state["substep"] = 0
			state.mode_step = 1
			state.mode_step_timer = 0
			result.presentation.append({
				"kind": "holiday_tel_init",
				"title": "電話番号",
				"prompt": "誰に電話かけようかな？",
				"contacts": contacts,
			})
		1:
			# Call selection / cursor movement
			state.mode_step_timer += 1
			# Handle navigation
			var dir_x: int = 0
			var dir_y: int = 0
			if (input.pressed & 0x1000) != 0: # UP
				dir_y = -1
			elif (input.pressed & 0x4000) != 0: # DOWN
				dir_y = 1
			if (input.pressed & 0x8000) != 0: # LEFT
				dir_x = -1
			elif (input.pressed & 0x2000) != 0: # RIGHT
				dir_x = 1

			if dir_x != 0 or dir_y != 0:
				cursor = _navigate_directory(contacts, cursor, dir_x, dir_y)
				phone_state["cursor"] = cursor
				result.presentation.append({"kind": "phone_cursor", "cursor": cursor})

			# PAD_CROSS: Exit back to holiday menu
			if (input.pressed & 0x40) != 0:
				state.mode_stage = 0
				state.mode_step = 0
				result.transitions.append({"kind": "holiday_menu_return"})
				return

			# PAD_CIRCLE: Confirm selection
			if (input.pressed & 0x20) != 0:
				if cursor == 13: # 取り消し (Cancel)
					state.mode_stage = 0
					state.mode_step = 0
					result.transitions.append({"kind": "holiday_menu_return"})
					return
				# Valid contact selected (src/main/holiday.c:1282-1290)
				result.audio.append({"kind": "native_audio_command", "command": 0x50A})
				var target_girl: int
				if cursor == 0:
					target_girl = 13 # GIRL_YOSHIO = 13 (holiday.c:1284)
				elif cursor == 12:
					target_girl = 11 # GIRL_MIHARU = 11 (holiday.c:1286)
				else:
					target_girl = cursor - 1
				phone_state["target_girl"] = target_girl
				phone_state["substep"] = 0
				state.mode_step = 2
				state.mode_step_timer = 0
		2:
			# Exit / Dialing sequence (holiday_tel_exit in holiday.c:1296-1335)
			state.mode_step_timer += 1
			var substep: int = int(phone_state.get("substep", 0))
			if substep < 11:
				if (state.mode_step_timer & 7) == 7:
					if substep == 0:
						result.audio.append({"kind": "native_audio_command", "command": 0x504}) # Dial tone
					elif substep >= 4:
						var digit_idx: int = substep - 4
						var g_id: int = int(phone_state.get("target_girl", 13))
						var digit_row: int = 10 if g_id == 13 else g_id # holiday.c:1306-1310
						var digit: int = GIRL_PHONE_DIGITS[digit_row][digit_idx]
						result.audio.append({"kind": "native_audio_command", "command": 0x505 + digit})
					substep += 1
					phone_state["substep"] = substep
			else:
				# Reached substep 11: transition to GAME_MODE_TELEPHONE_PROFILE (0x3E)
				result.audio.append({"kind": "native_restore_bgm", "bank": 0, "sequence": 0})
				var target_girl: int = int(phone_state.get("target_girl", 13))
				state.reset_game_mode_fields(0x3E)
				state.mode_stage = 0 if target_girl == 13 else 1
				result.transitions.append({
					"kind": "native_mode",
					"mode": 0x3E,
					"girl": target_girl,
					"stage": state.mode_stage,
				})

# Build the 14-slot telephone directory matching telephone_class_init()
static func build_telephone_directory(girls: Array) -> Array[Dictionary]:
	var directory: Array[Dictionary] = []
	for i in range(14):
		var x: int = (i / 5) * 0x58 - 0x88
		var y: int = (i % 5) * 0x10 - 0x38
		var w: int = 0x48
		var h: int = 0xE
		var name: String = CONTACT_NAMES[i]
		var available: bool = false

		if i == 0:
			available = true # Yoshio is always available
		elif i == 13:
			y += 0x10
			available = true # Cancel is always available
		elif i == 12:
			available = true # Ray is always available
		elif (i - 1) < girls.size():
			var g: Dictionary = girls[i - 1]
			# flags.f.b3 = phone number acquired flag (bit 3)
			if (int(g.get("flags", 0)) & 0x08) != 0:
				available = true
			else:
				w = 0 # Hidden / unselectable
		else:
			w = 0

		directory.append({
			"index": i,
			"name": name,
			"x": x,
			"y": y,
			"w": w,
			"h": h,
			"available": available,
		})
	return directory

static func _navigate_directory(contacts: Array[Dictionary], current: int,
		dir_x: int, dir_y: int) -> int:
	var col: int = current / 5
	var row: int = current % 5

	if dir_y != 0:
		for step in range(1, 5):
			var target_row: int = (row + dir_y * step + 5) % 5
			var target_idx: int = col * 5 + target_row
			if target_idx < 14 and contacts[target_idx]["available"]:
				return target_idx

	if dir_x != 0:
		for col_step in range(1, 3):
			var target_col: int = (col + dir_x * col_step + 3) % 3
			for r in range(5):
				var target_idx: int = target_col * 5 + r
				if target_idx < 14 and contacts[target_idx]["available"]:
					return target_idx

	return current

# Update quarter flags when reading magazine (hud_magazine_mark_read in hud_screens.c:906)
static func mark_magazine_read(quarter_flags: Array, year: int, month: int) -> void:
	if quarter_flags.size() < 2:
		quarter_flags.resize(2)
		quarter_flags.fill(0)

	var flag_word: int = (quarter_flags[1] << 8) | quarter_flags[0]

	if month != 2 and month != 5 and month != 8 and month != 11:
		flag_word |= (1 << 14)
	else:
		flag_word |= (1 << 13)

	if year == 0x5F and month == 4:
		flag_word |= (1 << 13)
		flag_word |= (1 << 14)

	match month:
		2, 3, 4:
			if year == 0x5F: flag_word |= (1 << 0)
			if year == 0x60: flag_word |= (1 << 4)
			if year == 0x61: flag_word |= (1 << 8)
		5, 6, 7:
			if year == 0x5F: flag_word |= (1 << 1)
			if year == 0x60: flag_word |= (1 << 5)
			if year == 0x61: flag_word |= (1 << 9)
			flag_word |= (1 << 15)
		8, 9, 10:
			if year == 0x5F: flag_word |= (1 << 2)
			if year == 0x60: flag_word |= (1 << 6)
			if year == 0x61: flag_word |= (1 << 10)
		11, 12, 13:
			if year == 0x5F: flag_word |= (1 << 3)
			if year == 0x60: flag_word |= (1 << 7)
			if year == 0x61: flag_word |= (1 << 11)
		1:
			if year == 0x60: flag_word |= (1 << 3)
			if year == 0x61: flag_word |= (1 << 7)
			if year == 0x62: flag_word |= (1 << 11)

	quarter_flags[0] = flag_word & 0xFF
	quarter_flags[1] = (flag_word >> 8) & 0xFF

# Get magazine hint index (holiday_magazine_hint_index in holiday.c:1142)
static func get_magazine_hint_index(year: int, month: int) -> int:
	match year:
		0x5F: # 1995
			if month < 5: return 0
			if month < 8: return 1
			if month < 11: return 2
			return 3
		0x60: # 1996
			if month < 2: return 3
			if month < 5: return 4
			if month < 8: return 5
			if month < 11: return 6
			return 7
		0x61: # 1997
			if month == 8: return 13
			if month < 2: return 7
			if month < 5: return 8
			if month < 8: return 9
			if month < 11: return 10
			return 11
		0x62: # 1998
			if month < 2: return 11
			return 12
	return 0

# Get magazine issue data for the date
static func get_magazine_issue_data(year: int, month: int) -> Dictionary:
	# Issue 0: April 1995
	return {
		"issue_index": 0,
		"title": "春号",
		"subtitle": "ＫＮＭ交響楽団",
		"notice_line1": "このコーナーで、最新",
		"notice_line2": "スポットの情報を紹介",
		"article_lines": [
			"このコーナーでは、次号より",
			"盛沢山のイベント情報を",
			"提供していきます。おたのしみに"
		],
		"monologue": "これが、こんど創刊された　情報誌か。　そうそう、カレンダーで　自分のバイオリズムが、　確認できるんだった。",
		"hint": "ページの中央・現在の位置　赤・身体　青・感情　緑・知性　のリズム",
	}

# Holiday Magazine State Step
# Handles holiday_mode_stage_magazine() in holiday.c:1116
static func step_holiday_magazine(state: State, input: InputFrame, result: Session.FrameResult,
		mag_state: Dictionary) -> void:
	var data: Dictionary = state.rule_state
	var q_flags: Array = data.get("quarter_flags", [0, 0])
	var year: int = int(data.get("year", 95))
	var month: int = int(data.get("month", 4))

	match state.mode_step:
		0:
			# Init: mark read, set title, page build
			mark_magazine_read(q_flags, year, month)
			data["quarter_flags"] = q_flags
			mag_state["substep"] = 0
			state.mode_step = 1
			state.mode_step_timer = 0
			result.presentation.append({
				"kind": "magazine_init",
				"title": "情報誌",
				"issue": get_magazine_issue_data(year, month),
			})
		1:
			# Show / dialogue stepping (holiday_magazine_show in holiday.c:1197)
			state.mode_step_timer += 1
			var substep: int = int(mag_state.get("substep", 0))
			if (input.pressed & (0x20 | 0x40)) != 0: # PAD_CIRCLE or PAD_CROSS
				if substep == 0:
					# Advance to hint display
					mag_state["substep"] = 2
					result.presentation.append({
						"kind": "magazine_hint",
						"hint": get_magazine_issue_data(year, month)["hint"],
					})
				elif substep >= 2:
					# Exit
					state.mode_step = 2
		2:
			# Exit: magazine_exit in holiday.c:1230
			state.mode_stage = 0
			state.mode_step = 0
			result.transitions.append({"kind": "holiday_menu_return"})
