extends RefCounted
## T-29009. Initial-April subset of scheduler.c::day_event_dispatch and
## calendar_advance_day. Call through GameSession so failed routes roll back.

const State = preload("res://rules/school_day_state.gd")
const Math = preload("res://core/native_math.gd")

static func branch_error(state: State) -> String:
	var data: Dictionary = state.rule_state
	if data["year"] != 95 or data["month"] != 4 or data["day"] < 5 or data["day"] > 9 or data["vacation"] != 0:
		return "ordinary-week dispatcher supports only non-vacation April5–9,1995"
	if data["weekday"] != data["day"] - 3:
		return "ordinary-week weekday differs from the initial-April native calendar"
	for appointment: int in data["appointment_words"]:
		if appointment != 0:
			return "ordinary-week appointment sorting/telephone branch is unimplemented"
	if data["day_events"][0][0] not in [0, 0x10]:
		return "ordinary-week queue has an unsupported exam/master/annual header"
	return ""

static func clamp_girls(data: Dictionary) -> void:
	# girl_counters_clamp writes high halfwords; fractions remain untouched.
	for index: int in range(11):
		var girl: Dictionary = data["girls"][index]
		for key: String in ["tokimeki_word", "affection_word", "syoushin_word"]:
			girl[key] = State.with_high(girl[key], clampi(State.high(girl[key]), 0, 128))
		if State.high(girl["syoushin_word"]) < 50:
			girl["flags"] = Math.u32(girl["flags"] & ~0x30000)

static func finish(state: State) -> String:
	var data: Dictionary = state.rule_state
	var current: int = data["day_event_index"]
	if current == 0 or current == 31:
		return "day-end needs native previous/next queue records"
	# These predicates run before calendar progression in day_event_dispatch.
	if data["day_event_count_today"] == 0 and data["weekday"] != 6 and data["calendar"][0]["day_kind"][data["day"]] == 0 and data["day_events"][current - 1][0] == 0:
		for index: int in range(11):
			var girl: Dictionary = data["girls"][index]
			if (girl["flags"] & 2) != 0 and State.high(girl["syoushin_word"]) >= 76 and (girl["flags"] & 0x10000) == 0:
				return "reachable forced girl encounter %d is unimplemented" % index
	for index: int in range(11):
		var girl: Dictionary = data["girls"][index]
		if (girl["flags"] & 2) != 0 and State.high(girl["syoushin_word"]) >= 61 and (girl["flags"] & 0x20000) == 0:
			return "reachable rumor report for girl %d is unimplemented" % index
	if data["daily_check_done"] == 0:
		for index: int in range(11):
			var girl: Dictionary = data["girls"][index]
			if (girl["flags"] & 2) != 0 and State.high(girl["syoushin_word"]) >= 91:
				return "reachable telephone concern for girl %d is unimplemented" % index
		if (data["girls"][11]["flags"] & 0xe00) != 0 and data["scene_flags"][77] == 0:
			return "reachable girl11 telephone RNG branch is unimplemented"
	data["daily_check_done"] = 0
	data["day_event_index"] = Math.u8(current + 1)
	data["weekday"] = (data["weekday"] + 1) % 7
	data["day"] = Math.u8(data["day"] + 1)
	data["day_count"] = Math.u16(data["day_count"] + 1)
	data["day_event_done_bits"] = 0
	data["day_event_count_today"] = 0
	# calendar_advance_day: index is nonzero here, hence daily_check_done=0.
	if data["weekday"] == 6 and (data["player_flags_upper"] & 1) != 0:
		data["player_flags_upper"] &= ~1
	return ""
