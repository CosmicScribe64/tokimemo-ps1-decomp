extends RefCounted
## T-29004: source scheduler.c::calendar_advance_day for the initial April slice.
## Month/year rollover requires additional quarter/appointment services.

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")
const CalendarSource = preload("res://services/native_calendar_source.gd")

static func birthday_matches(word: int, month: int, day: int) -> bool:
	if month > 12:
		month = ((month - 1) % 12) + 1
	return (word & 15) == month and ((Math.u32(word) >> 4) & 31) == day

static func days_in_month(year: int, month: int) -> int:
	var m: int = ((month - 1) % 12) + 1
	match m:
		1, 3, 5, 7, 8, 10, 12:
			return 31
		4, 6, 9, 11:
			return 30
		2:
			return 29 if year == 96 else 28
	return 30

static func weekday_for_date(year: int, month: int, day: int) -> int:
	# 1995-04-04 is Tuesday (weekday == 1, where 0=Mon..6=Sun).
	var total_days: int = 0
	var y: int = 95
	var m: int = 4
	while y < year or (y == year and m < month):
		total_days += days_in_month(y, m)
		m += 1
		if m > 12:
			m = 1
			y += 1
	total_days += day - 4
	return ((1 + total_days) % 7 + 7) % 7

static func _fill_single_month(cal: Dictionary, year: int, month: int) -> void:
	var count: int = days_in_month(year, month)
	var first_wd: int = weekday_for_date(year, month, 1)
	cal["day_kind"].fill(0)
	cal["marker"].fill(0)
	cal["day_kind"][0] = count
	cal["marker"][0] = first_wd
	for d: int in range(1, count + 1):
		var wd: int = (first_wd + d - 1) % 7
		var is_vacation: bool = false
		if (month == 7 and d >= 20) or month == 8:
			is_vacation = true
		elif (month == 12 and d >= 25) or (month == 1 and d <= 7):
			is_vacation = true
		elif (month == 3 and d >= 20 and year < 98) or (month == 4 and d <= 4 and year > 95):
			is_vacation = true
		if wd == 6:
			cal["day_kind"][d] = 1
		elif (month == 4 and d == 29) or (month == 5 and d in [3, 4, 5]) or (month == 9 and d in [15, 23]) or (month == 10 and d == 10) or (month == 11 and d in [3, 23]) or (month == 1 and d in [1, 15]) or (month == 2 and d == 11) or (month == 3 and d == 20):
			cal["day_kind"][d] = 2
		elif is_vacation:
			cal["day_kind"][d] = 3
		else:
			cal["day_kind"][d] = 0

		# Annual markers from src/main/scheduler.c::day_event_queue_build_week
		if month == 4 and d == 6 and year in [96, 97]:
			cal["marker"][d] = 2
		elif month == 4 and d == 20 and year == 95:
			cal["marker"][d] = 0x13
		elif month == 6 and d == 12:
			cal["marker"][d] = 4
		elif month == 7 and d >= 10 and d <= 15 and wd < 6:
			cal["marker"][d] = 5
		elif month == 7 and d == 19:
			cal["marker"][d] = 6
		elif month == 8 and d == 14:
			cal["marker"][d] = 8
		elif month == 9 and d == 16 and year == 96:
			cal["marker"][d] = 0x0b
		elif month == 11 and d == 3:
			cal["marker"][d] = 0x0c
		elif month == 12 and d >= 11 and d <= 16 and wd < 6:
			cal["marker"][d] = 5
		elif month == 12 and d == 23:
			cal["marker"][d] = 0x0e
		elif month == 12 and d == 24:
			cal["marker"][d] = 0x0d
		elif month == 1 and d == 1:
			cal["marker"][d] = 0x0f
		elif month == 1 and d == 8:
			cal["marker"][d] = 0x10
		elif month == 2 and d == 14:
			cal["marker"][d] = 0x11
		elif month == 2 and d == 18 and year == 98:
			cal["marker"][d] = 0x12
		elif month == 3 and d >= 9 and d <= 14 and year < 98 and wd < 6:
			cal["marker"][d] = 5
		elif month == 3 and d == 19 and year < 98:
			cal["marker"][d] = 0x14

static func populate_month_calendar(data: Dictionary, year: int, month: int) -> void:
	if not data.get("calendar") is Array or data["calendar"].size() < 2:
		return
	_fill_single_month(data["calendar"][0], year, month)
	var next_y: int = year
	var next_m: int = month + 1
	if next_m > 12:
		next_m = 1
		next_y += 1
	_fill_single_month(data["calendar"][1], next_y, next_m)

static func update_vacation_for_date(data: Dictionary) -> void:
	var y: int = data["year"]
	var m: int = data["month"]
	var d: int = data["day"]
	if (m == 7 and d >= 20) or m == 8:
		data["vacation"] = 1
	elif (m == 12 and d >= 25) or (m == 1 and d <= 7):
		data["vacation"] = 2
	elif (m == 3 and d >= 20 and y < 98) or (m == 4 and d <= 4 and y > 95):
		data["vacation"] = 3
	else:
		data["vacation"] = 0

static func advance_day(state: State) -> String:
	var data: Dictionary = state.rule_state
	data["weekday"] = Math.u8((data["weekday"] + 1) % 7)
	data["day"] = Math.u8(data["day"] + 1)
	var limit: int = days_in_month(data["year"], data["month"])
	if data["day"] > limit:
		data["day"] = 1
		data["month"] = Math.u8(data["month"] + 1)
		if data["month"] >= 13:
			data["year"] = Math.u8(data["year"] + 1)
			data["month"] = 1
		# T-29028: the source FE mode rotates the original current/next records
		# only after its CD wait. Preserve them until that validated load completes.
		if data["resources"].get("native_calendar_source") != CalendarSource.SOURCE_SHA:
			populate_month_calendar(data, data["year"], data["month"])
		if data["month"] % 3 == 0:
			for i: int in range(11):
				data["girl_last_date_spot_timer"][i] = 0
			# Mirrors schedule_quarter_flags_rotate (src/main/scheduler.c:1981-2002): b14 = b13; b13 = 0
			var b13: int = (data["player_flags_upper"] >> 13) & 1
			data["player_flags_upper"] = (data["player_flags_upper"] & ~(3 << 13)) | (b13 << 14)
		# Amusement reset belongs to its scheduler callers, including FE upload.
	data["day_count"] = Math.u16(data["day_count"] + 1)
	update_vacation_for_date(data)
	data["day_event_done_bits"] = 0
	data["day_event_count_today"] = 0
	data["daily_check_done"] = 1 if data["day_event_index"] == 0 else 0
	if data["weekday"] == 6 and (data["player_flags_upper"] & 1) != 0 and data["day_events"][0][0] != 0x40:
		data["player_flags_upper"] &= ~1
	return ""

static func advance_initial_april(state: State) -> String:
	var data: Dictionary = state.rule_state
	if data.get("allow_multi_week", false):
		return advance_day(state)
	if data["year"] != 95 or data["month"] != 4 or data["day"] != 4 or data["weekday"] != 1:
		return "calendar branch outside captured April4→5 boundary"
	for appointment: int in data["appointment_words"]:
		if appointment != 0:
			return "appointment sorting/launch is outside captured empty-appointment boundary"
	data["weekday"] = Math.u8(data["weekday"] + 1)
	data["day"] = Math.u8(data["day"] + 1)
	data["day_count"] = Math.u16(data["day_count"] + 1)
	data["day_event_done_bits"] = 0
	data["day_event_count_today"] = 0
	data["daily_check_done"] = 1 if data["day_event_index"] == 0 else 0
	return ""
