extends RefCounted
## T-29004: source scheduler.c::calendar_advance_day for the initial April slice.
## Month/year rollover requires additional quarter/appointment services.
## Calendar rows (day kinds and markers) come only from the original CALROM
## (hud_screens.c::calendar_month_load via NativeCalendarSource and the FE
## month-start mode). Vacation changes only in schedule_vacation_notice_step
## (native mode 0x39, rules/day_notice.gd). There is no generated calendar.

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

## A month rollover keeps the current/next CALROM rows until the FE month-start
## mode loads the next month. Without the validated original source there is no
## next row to load, so the rollover is refused before any state changes.
static func rollover_source_error(data: Dictionary) -> String:
	if data["day"] + 1 <= days_in_month(data["year"], data["month"]):
		return ""
	if not data.get("resources") is Dictionary or data["resources"].get("native_calendar_source") != CalendarSource.SOURCE_SHA:
		return "calendar month rollover requires the validated original CALROM source"
	return ""

## Same check for `count` consecutive advance_day calls (SHUGAKU's six trip days,
## RENSYU's camp day). The validated source is fixed across the calls and only a
## month end can refuse, so one check before the first write covers them all.
static func advance_days_source_error(data: Dictionary, count: int) -> String:
	if count <= 0 or data["day"] + count <= days_in_month(data["year"], data["month"]):
		return ""
	if not data.get("resources") is Dictionary or data["resources"].get("native_calendar_source") != CalendarSource.SOURCE_SHA:
		return "calendar month rollover requires the validated original CALROM source"
	return ""

static func advance_day(state: State) -> String:
	var data: Dictionary = state.rule_state
	var error: String = rollover_source_error(data)
	if not error.is_empty():
		return error
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
		if data["month"] % 3 == 0:
			for i: int in range(11):
				data["girl_last_date_spot_timer"][i] = 0
			# Mirrors schedule_quarter_flags_rotate (src/main/scheduler.c:1981-2002): b14 = b13; b13 = 0
			var b13: int = (data["player_flags_upper"] >> 13) & 1
			data["player_flags_upper"] = (data["player_flags_upper"] & ~(3 << 13)) | (b13 << 14)
		# Amusement reset belongs to its scheduler callers, including FE upload.
	data["day_count"] = Math.u16(data["day_count"] + 1)
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
	# T-39002: a later April 1995 week start (Scheduler.april_week_boundary)
	# advances Sunday to Monday inside the month: calendar_advance_day.
	if data["year"] == 95 and data["month"] == 4 and data["weekday"] == 6 and data["day"] + 7 <= days_in_month(95, 4):
		for appointment: int in data["appointment_words"]:
			if appointment != 0:
				return "appointment sorting/launch is outside captured empty-appointment boundary"
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
