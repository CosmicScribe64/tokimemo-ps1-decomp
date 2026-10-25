extends RefCounted
## T-29004: source scheduler.c::calendar_advance_day for the initial April slice.
## Month/year rollover requires additional quarter/appointment services.

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")

static func birthday_matches(word: int, month: int, day: int) -> bool:
	if month == 13:
		month = 1
	return (word & 15) == month and ((Math.u32(word) >> 4) & 31) == day

static func advance_initial_april(state: State) -> String:
	var data: Dictionary = state.rule_state
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
