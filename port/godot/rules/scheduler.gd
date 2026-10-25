extends RefCounted
## T-29004: real initial-April queue construction, with explicit branch limits.
## Witnesses: scheduler.c, menu_speaker_girl.c, hud_screens.c; tables external.

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")
const Calendar = preload("res://rules/calendar.gd")
const Rng = preload("res://core/native_rng.gd")
const Session = preload("res://core/game_session.gd")

static func initial_branch_error(state: State) -> String:
	var data: Dictionary = state.rule_state
	if [data["year"], data["month"], data["day"], data["weekday"], data["day_count"]] != [95, 4, 4, 1, 0]:
		return "scheduler supports the captured April4 initial boundary only"
	if data["vacation"] != 0 or ((data["player_flags"] >> 8) & 15) != 0 or (data["player_flags"] >> 12) != 11 or (data["player_flags_upper"] & 1) != 0:
		return "initial scheduler health/club/term branch differs from captured route"
	if data["memory_card_job"] != 0:
		return "memory-card save polling outside captured unsaved route"
	for index: int in range(1, 11):
		if (data["girls"][index]["flags"] & 2) != 0:
			return "additional met girl requires broader encounter-selection branches"
	for index: int in range(11):
		for counter: int in data["girls"][index]["counters"].slice(1, 21):
			if counter != 0:
				return "girl special-scene history outside initial route"
	if (Math.u32(data["girls"][12]["flags"]) >> 24) >= 21 or data["scene_flags"][52] == 1:
		return "reachable special-scene branch outside initial route"
	for day: int in range(5, 10):
		if data["calendar"][0]["marker"][day] != 0 or data["calendar"][0]["day_kind"][day] not in [0, 1, 2]:
			return "annual event/vacation marker outside initial ordinary week"
	for appointment: int in data["appointment_words"]:
		if appointment != 0:
			return "appointment rules outside captured empty-appointment route"
	return ""

static func _weighted_affection(girl: Dictionary, index: int, tables: Dictionary) -> int:
	var raw: int = Math.divmod32(State.high(girl["affection_word"]) * tables["affection_weights"][index], 100).quotient
	return clampi(raw, 0, 100)

static func _weighted_tokimeki(data: Dictionary, index: int, tables: Dictionary) -> int:
	var raw: int = Math.divmod32(State.high(data["girls"][index]["tokimeki_word"]) * tables["tokimeki_weights"][index], 100).quotient
	var kind: int = tables["ladder_kinds"][index]
	var offsets: Array = []
	match kind:
		1: offsets = [50, 35, 20, 20, 5, 5, 0]
		2: offsets = [0, 0, 0, 0, 0, 0, 0]
		3: offsets = [50, 50, 50, 20, 20, 20, 20]
		4: offsets = [10, 10, 10, 10, 0, 0, 0]
		5: offsets = [20, 20, 20, 20, 0, -5, 0]
		6: offsets = [0, 0, 0, 0, -5, -5, -5]
	if not offsets.is_empty():
		for parameter: int in range(1, 7):
			if (tables["bonus_param_masks"][index] & (1 << parameter)) == 0:
				continue
			var step: int = 7
			for candidate: int in range(7):
				# The bases are unsigned bytes promoted under the original K&R ABI.
				if Math.u32(State.high(data["params"][parameter])) > Math.u32(tables["ladder_bases"][candidate] + offsets[candidate]):
					step = candidate
					break
			raw += tables["param_bonus"][index][step]
	return clampi(raw, 0, 100)

static func _closeness(data: Dictionary, index: int, tables: Dictionary) -> int:
	var affection: int = Math.divmodu32(_weighted_affection(data["girls"][index], index, tables), 5).quotient
	var tokimeki: int = Math.divmodu32(_weighted_tokimeki(data, index, tables), 5).quotient
	return tables["closeness_grid"][mini(tokimeki, 19)][mini(affection, 19)]

static func _sine(angle: int, tables: Dictionary) -> int:
	var signed_angle: int = Math.s32(angle)
	var negative: bool = signed_angle < 0
	var phase: int = (Math.u32(-signed_angle) if negative else signed_angle) & 4095
	var value: int
	if phase <= 1024:
		value = tables["sine_quarter"][phase]
	elif phase <= 2048:
		value = tables["sine_quarter"][2048 - phase]
	elif phase <= 3072:
		value = -tables["sine_quarter"][phase - 2048]
	else:
		value = -tables["sine_quarter"][4096 - phase]
	return -value if negative else value

static func _wave(data: Dictionary, day_count: int, selected: Array, scale: int, tables: Dictionary) -> int:
	var birthday: int = data["player_birthday"]
	var month: int = birthday & 15
	var day: int = (Math.u32(birthday) >> 4) & 31
	var offset: int = -tables["birthday_offsets"][month] - day + 0x1563
	if month == 4 and day < 4:
		offset = -tables["birthday_offsets"][3] - day + 0x1582
	var phase: int = offset + Math.u16(day_count)
	var total: int = 0
	var count: int = 0
	var factors: Array[int] = [178, 146, 124]
	for index: int in range(3):
		if selected[index] == 0:
			continue
		count += 1
		var wave: int = _sine(Math.s32(Math.mult32(phase, factors[index]).lo), tables)
		if absi(wave) < 409:
			total -= 4096
		elif wave > 0:
			total += wave
		else:
			total -= Math.divmod32(wave, 2).quotient
	if count == 0:
		return 1
	if Math.divmod32(total, count).quotient < 0:
		return 0
	return Math.divmod32(Math.divmod32(total * scale, 4096).quotient, count).quotient

static func poor_result(state: State, command: int, days_ahead: int, tables: Dictionary, rng: Rng) -> int:
	var data: Dictionary = state.rule_state
	var day_count: int = Math.u16(data["day_count"] + days_ahead)
	var score: int = 16
	if command == 0:
		score = _wave(data, day_count, [0, 0, 1], 16, tables) + Math.divmod32(State.high(data["params"][1]), 16).quotient
	score += 6
	var stress: int = State.high(data["params"][8])
	var stress_factor: int = 4 if stress >= 97 else Math.divmod32(stress, 32).quotient
	score = rng.next("src/main/hud_screens.c::hud_command_roll_poor_result") % (score + 4)
	score = Math.divmod32((8 - stress_factor) * score, 8).quotient
	var zeroes: int = 0
	for selected: Array in [[1, 0, 0], [0, 1, 0], [0, 0, 1]]:
		if _wave(data, day_count, selected, 3, tables) == 0:
			zeroes += 1
	if zeroes == 3:
		score = Math.divmod32(score, 4).quotient
	return 0 if score >= 4 else 1

static func build_initial_week(state: State, tables: Dictionary, rng: Rng) -> String:
	var failure: String = initial_branch_error(state)
	if not failure.is_empty():
		return failure
	var data: Dictionary = state.rule_state
	var command: int = data["last_command"] & 15
	if command != 0 and command != 7:
		return "initial queue supports only commands0/7"
	data["progress_state"][0x15 + command % 9] = Math.u8(data["progress_state"][0x15 + command % 9] + 1)
	if data["girls"][11]["counters"][0] != 0:
		data["girls"][11]["counters"][0] = Math.u8(data["girls"][11]["counters"][0] - 1)
	var companion: int = 15
	if command == 0 and _closeness(data, 0, tables) > tables["command0_rank_threshold"][0]:
		companion = 0
	data["last_picked_girl"] = -1
	var recovery: int = (data["player_flags_upper"] >> 5) & 7
	if recovery != 0:
		data["player_flags_upper"] = (data["player_flags_upper"] & ~0xe0) | ((recovery - 1) << 5)
	var queue: Array = []
	var random_count: int = 0
	for weekday: int in range(2, 7):
		var day: int = weekday + 3
		var kind: int = data["calendar"][0]["day_kind"][day]
		if kind in [1, 2]:
			# schedule_miharu_scene4_ready cannot pass its year==98 gate here;
			# girl_scores only computes local output arrays and consumes no RNG.
			queue.append([0x10, 0, 0, 0])
		else:
			var poor: int = poor_result(state, command, weekday, tables, rng)
			queue.append([0, command, 0 if command == 7 else poor, companion])
			if random_count < 2 and (rng.next("src/main/scheduler.c::day_event_queue_build_week/encounter") & 3) == 3:
				random_count += 1
				queue.append([0x44, 0, 0, 0])
		if Calendar.birthday_matches(data["girls"][0]["birthday"], 4, day):
			queue.append([0x32, 0, 0, 0])
		if Calendar.birthday_matches(data["player_birthday"], 4, day):
			queue.append([0x33, 0, 0, 0])
		if weekday == 5:
			queue.append([0xfc, 0, 0, 0])
		if weekday != 6:
			queue.append([0xfb, 0, 0, 0])
	if queue.size() > 32:
		return "native day-event queue capacity exceeded"
	while queue.size() < 32:
		queue.append([0xff, 0, 0, 0])
	data["day_events"] = queue
	data["day_event_index"] = 0
	state.next_step()
	return ""

static func resolve_initial_encounters(state: State, tables: Dictionary, rng: Rng) -> String:
	var failure: String = initial_branch_error(state)
	if not failure.is_empty():
		return failure
	var data: Dictionary = state.rule_state
	var invitation_count: int = 0
	for index: int in range(32):
		var event: Array = data["day_events"][index]
		if event[0] != 0x44:
			continue
		if index == 31:
			return "encounter placeholder lacks a following queue record"
		if data["day_events"][index + 1][0] in [0x32, 0x33, 0x47]:
			event[0] = 0xfa
			continue
		# Exact initial special-scene path still consumes the Miharu roll,
		# whose later day_count>=29 gate fails. Do not short-circuit this draw.
		rng.next("src/main/scheduler.c::schedule_special_scene_select/Miharu")
		var rank: int = _closeness(data, 0, tables)
		var girl: Dictionary = data["girls"][0]
		var eligible: bool = (girl["flags"] & 2) != 0 and State.high(girl["syoushin_word"]) < 60 and _weighted_affection(girl, 0, tables) >= 10
		var invitation: bool = eligible and rank >= 33 and rng.next("src/main/scheduler.c::date_sasoi_judge") % 100 < 8 and data["last_picked_girl"] != 0
		if invitation:
			data["last_picked_girl"] = 0
		if invitation and invitation_count == 0:
			invitation_count += 1
			event[0] = 0x46
			event[1] = 0
		elif eligible and rank >= 17 and rng.next("src/main/scheduler.c::geko_judge") % 100 < 50 and data["last_picked_girl"] != 0:
			data["last_picked_girl"] = 0
			event[1] = 0
		else:
			event[0] = 0xfa
	state.next_step()
	return ""

static func weekly_girl_update(state: State, tables: Dictionary) -> void:
	var data: Dictionary = state.rule_state
	for index: int in range(11):
		var girl: Dictionary = data["girls"][index]
		if (girl["flags"] & 2) == 0:
			continue
		var affection: int = State.high(girl["affection_word"])
		if (data["day"] < 7 or (data["day"] >= 14 and data["day"] < 21 and index == 0)) and affection > 0:
			girl["affection_word"] = State.with_high(girl["affection_word"], affection - 1)
	# Source runs every affection decay before any syoushin growth.
	for index: int in range(11):
		var girl: Dictionary = data["girls"][index]
		if (girl["flags"] & 2) != 0 and _weighted_affection(girl, index, tables) < 10:
			girl["syoushin_word"] = State.with_high(girl["syoushin_word"], State.high(girl["syoushin_word"]) + 5)
	state.next_step()

static func dispatch_first_activity(state: State, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	var event: Array = data["day_events"][data["day_event_index"]]
	if event[0] != 0:
		result.error = "next native queue branch is unsupported: type 0x%02x at index %d" % [event[0], data["day_event_index"]]
		return
	for index: int in range(11):
		var girl: Dictionary = data["girls"][index]
		for field: String in ["tokimeki_word", "affection_word", "syoushin_word"]:
			girl[field] = State.with_high(girl[field], clampi(State.high(girl[field]), 0, 128))
		if State.high(girl["syoushin_word"]) < 50:
			girl["flags"] = Math.u32(girl["flags"] & ~0x30000)
	for index: int in range(14):
		data["icon_enabled"][index] = 1 if index == event[1] else 0
	state.reset_game_mode_fields(0x40)
	result.transitions.append({"kind": "native_mode", "mode": 0x40})
