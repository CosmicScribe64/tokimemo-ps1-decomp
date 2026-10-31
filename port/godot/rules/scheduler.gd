extends RefCounted
## T-29004: real initial-April queue construction, with explicit branch limits.
## Witnesses: scheduler.c, menu_speaker_girl.c, hud_screens.c; tables external.

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")
const Calendar = preload("res://rules/calendar.gd")
const Rng = preload("res://core/native_rng.gd")
const Session = preload("res://core/game_session.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")

## The captured first week: command select of the New Game, 95/4/4.
static func initial_boundary(data: Dictionary) -> bool:
	return [data["year"], data["month"], data["day"], data["weekday"], data["day_count"]] == [95, 4, 4, 1, 0]

## T-39002: a later week start of April 1995, the command select on a Sunday
## (weekday 6) whose following seven days day+1..day+7 stay in April
## (day_event_queue_build_week without its month rollover), before day_count
## 29 (schedule_special_scene_select's Miharu gate, not modeled, stays shut).
static func april_week_boundary(data: Dictionary) -> bool:
	return data["year"] == 95 and data["month"] == 4 and data["weekday"] == 6 and data["vacation"] == 0 \
		and data["day"] + 7 <= Calendar.days_in_month(95, 4) and data["day_count"] < 29

static func initial_branch_error(state: State) -> String:
	var data: Dictionary = state.rule_state
	if data.get("allow_multi_week", false):
		var cmd: int = data["last_command"] & 15
		if cmd not in [0, 1, 2, 3, 4, 5, 6, 7]:
			return "initial queue supports only commands0-7"
		if cmd == 4 and ((data["player_flags"] >> 12) & 15) >= 11:
			return "initial queue club command requires valid club_id 0..10"
		return ""
	var initial: bool = initial_boundary(data)
	if not initial and not april_week_boundary(data):
		return "scheduler supports the April 1995 week starts only (the captured April4 boundary or a Sunday whose next week stays in April)"
	var club_id: int = (data["player_flags"] >> 12) & 15
	if data["vacation"] != 0 or ((data["player_flags"] >> 8) & 15) != 0 or club_id > 11 or (data["player_flags_upper"] & 1) != 0:
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
	# The days the queue covers: April 5..9 for the first week, day+1..day+7 later.
	for day: int in (range(5, 10) if initial else range(data["day"] + 1, data["day"] + 8)):
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

## girl_state_rank / get_g_zyotai_s & 0x7F (src/main/menu_speaker_girl.c,
## wiki/spec/core.md): the 0..4 mood rank scenes switch on.
static func girl_state_rank(data: Dictionary, index: int, tables: Dictionary) -> int:
	var affection: int = _weighted_affection(data["girls"][index], index, tables)
	var tokimeki: int = _weighted_tokimeki(data, index, tables)
	if affection < 0xA:
		return 4
	if affection < 0x1E:
		return 3
	if affection < 0x32:
		return 3 if tokimeki < 0x32 else 1
	if affection < 0x50:
		return 2 if tokimeki < 0x32 else 1
	return 2 if tokimeki < 0x32 else 0

## get_g_zyotai_h (src/main/menu_speaker_girl.c): the rank event_log_add stores.
static func girl_log_rank(data: Dictionary, index: int, tables: Dictionary) -> int:
	var affection: int = _weighted_affection(data["girls"][index], index, tables)
	var tokimeki: int = _weighted_tokimeki(data, index, tables)
	if affection < 0xA:
		return 4
	if affection < 0x32:
		return 3
	if tokimeki < 0x32:
		return 2
	if tokimeki < 0x50:
		return 1
	return 0

# Rank bars in address order 0x800B6748..0x800B6758 (config/symbol_addrs.txt,
# include/main/schedule.h); command0_rank_threshold is the first word.
const RANK_BAR_LIBERAL_ARTS_ATHLETICS := 0
const RANK_BAR_ART := 1
const RANK_BAR_TRIVIA := 2
const RANK_BAR_SHIORI_ATHLETICS := 3
const RANK_BAR_SCIENCE_LOOKS := 4
const CLUB_RANK_BAR := 0xB
# schedule_command_girl_select command 4: player club -> heroine it tests.
const CLUB_COMMAND_GIRL: Dictionary = {0: 1, 1: 1, 2: 2, 3: 2, 4: 3, 5: 3, 6: 4, 7: 4, 8: 5, 9: 6, 10: 10}

static func _rank_bar(tables: Dictionary, which: int) -> Variant:
	if tables.get("command_rank_bars") is Array and (tables["command_rank_bars"] as Array).size() == 5:
		return int(tables["command_rank_bars"][which])
	if which == RANK_BAR_LIBERAL_ARTS_ATHLETICS and tables.get("command0_rank_threshold") is Array:
		return int(tables["command0_rank_threshold"][0])
	return null

static func _girl_flag(data: Dictionary, girl: int, mask: int) -> bool:
	return (int(data["girls"][girl]["flags"]) & mask) != 0

static func _girl_club(data: Dictionary, girl: int) -> int:
	return (Math.u32(int(data["girls"][girl]["flags"])) >> 20) & 15

## src/main/scheduler.c::schedule_command_girl_select. Reads saved heroine
## met (bit 1), b2 (bit 2) and club (bits 20-23) plus girl_scores closeness.
## Returns the week's companion 0..10 or 0xF. A missing rank bar fails closed
## instead of substituting "nobody".
static func command_girl_select(data: Dictionary, command: int, tables: Dictionary) -> Dictionary:
	var kind: int = command % 9
	var needed: Array[int] = []
	match kind:
		0: needed = [RANK_BAR_LIBERAL_ARTS_ATHLETICS]
		1, 6: needed = [RANK_BAR_SCIENCE_LOOKS]
		2: needed = [RANK_BAR_ART]
		3: needed = [RANK_BAR_LIBERAL_ARTS_ATHLETICS, RANK_BAR_SHIORI_ATHLETICS]
		5: needed = [RANK_BAR_TRIVIA]
	for which: int in needed:
		if _rank_bar(tables, which) == null:
			return {"companion": -1, "error": "schedule_command_girl_select command %d needs original rank bar %d; table pack lacks command_rank_bars" % [command, which]}
	var rank: Array[int] = []
	for girl: int in range(11):
		rank.append(_closeness(data, girl, tables))
	var first_girl: int = 0
	var first_score: int = 0
	var second_girl: int = 0
	var second_score: int = 0
	var self_score: int = 0
	var candidates: Dictionary = {0: 1, 1: 2, 2: 3, 5: 8, 6: 7}
	if candidates.has(kind):
		var bar: int = _rank_bar(tables, needed[0])
		var girl: int = candidates[kind]
		if _girl_flag(data, girl, 2):
			first_girl = girl
			first_score = rank[girl] - bar
		self_score = rank[0] - bar
	elif kind == 3:
		var bar: int = _rank_bar(tables, RANK_BAR_LIBERAL_ARTS_ATHLETICS)
		if _girl_flag(data, 5, 2):
			first_girl = 5
			first_score = rank[5] - bar
		if _girl_flag(data, 6, 2):
			second_girl = 6
			second_score = rank[6] - bar
		self_score = rank[0] - int(_rank_bar(tables, RANK_BAR_SHIORI_ATHLETICS))
	elif kind == 4:
		var club: int = (int(data["player_flags"]) >> 12) & 15
		if club == _girl_club(data, 0):
			self_score = rank[0] - CLUB_RANK_BAR
			if not _girl_flag(data, 0, 4):
				self_score = 0
			if club == 10 and _girl_flag(data, 10, 2) and _girl_flag(data, 10, 4) and _girl_club(data, 10) == 10:
				first_girl = 10
				first_score = rank[10] - CLUB_RANK_BAR
		elif CLUB_COMMAND_GIRL.has(club):
			var mate: int = CLUB_COMMAND_GIRL[club]
			if _girl_flag(data, mate, 2) and _girl_flag(data, mate, 4) and _girl_club(data, mate) == club:
				# Clubs 8 and 9 also require Shiori outside the club; always true in this else branch.
				first_girl = mate
				first_score = rank[mate] - CLUB_RANK_BAR
	if first_girl != 0 and second_girl != 0:
		if second_score < first_score:
			if self_score < first_score and first_score > 0:
				return {"companion": first_girl, "error": ""}
		elif self_score < second_score and second_score > 0:
			return {"companion": second_girl, "error": ""}
	elif first_girl != 0:
		if self_score < first_score and first_score > 0:
			return {"companion": first_girl, "error": ""}
	elif second_girl != 0 and self_score < second_score and second_score > 0:
		return {"companion": second_girl, "error": ""}
	return {"companion": 0 if self_score > 0 else 15, "error": ""}

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
	elif command == 1:
		score = _wave(data, day_count, [0, 0, 1], 16, tables) + Math.divmod32(State.high(data["params"][2]), 16).quotient
	elif command == 2:
		score = _wave(data, day_count, [0, 1, 0], 16, tables) + Math.divmod32(State.high(data["params"][3]), 16).quotient
	elif command == 3:
		score = _wave(data, day_count, [1, 0, 0], 16, tables) + Math.divmod32(State.high(data["params"][4]), 16).quotient
	elif command == 4:
		var club_id: int = (data["player_flags"] >> 12) & 15
		var wave_kind: Array
		match club_id:
			0, 2, 3: wave_kind = [0, 0, 1]
			1, 4, 5: wave_kind = [0, 1, 0]
			6, 7, 8, 9, 10: wave_kind = [1, 0, 0]
			_: wave_kind = []
		if wave_kind.is_empty():
			score = 0
		else:
			score = _wave(data, day_count, wave_kind, 16, tables)
		var practice_points: int = 0
		if data.has("clubs") and data["clubs"] is Array and club_id < data["clubs"].size():
			var entry: Variant = data["clubs"][club_id]
			if entry is int:
				practice_points = entry
			elif entry is Dictionary and entry.has("practice_points"):
				practice_points = int(entry["practice_points"])
		score += Math.divmod32(practice_points, 16).quotient
	elif command == 5:
		score = _wave(data, day_count, [0, 1, 0], 16, tables) + Math.divmod32(State.high(data["params"][5]), 16).quotient
	elif command == 6:
		score = _wave(data, day_count, [0, 1, 0], 16, tables) + Math.divmod32(State.high(data["params"][6]), 16).quotient
	elif command == 7:
		score = 16
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

static func build_week(state: State, tables: Dictionary, rng: Rng) -> String:
	var data: Dictionary = state.rule_state
	var command: int = data["last_command"] & 15
	if command not in [0, 1, 2, 3, 4, 5, 6, 7]:
		return "initial queue supports only commands0-7"
	if command == 4 and ((data["player_flags"] >> 12) & 15) >= 11:
		return "initial queue club command requires valid club_id 0..10"
	var guard: String = Ordinary.profile_guard(data)
	if not guard.is_empty():
		return guard
	data["progress_state"][0x15 + command % 9] = Math.u8(data["progress_state"][0x15 + command % 9] + 1)
	if data["girls"][11]["counters"][0] != 0:
		data["girls"][11]["counters"][0] = Math.u8(data["girls"][11]["counters"][0] - 1)
	if data.get("allow_multi_week", false):
		for i: int in range(11):
			if data["girl_last_date_spot_timer"][i] != 0:
				data["girl_last_date_spot_timer"][i] = Math.u8(data["girl_last_date_spot_timer"][i] - 1)
	var selection: Dictionary = command_girl_select(data, command, tables)
	if not String(selection["error"]).is_empty():
		return selection["error"]
	var companion: int = int(selection["companion"]) & 15
	data["last_picked_girl"] = -1
	var recovery: int = (data["player_flags_upper"] >> 5) & 7
	if recovery != 0:
		data["player_flags_upper"] = (data["player_flags_upper"] & ~0xe0) | ((recovery - 1) << 5)
	Ordinary.reanchor_profile(data)

	var day: int = data["day"]
	var month: int = data["month"]
	var month_off: int = 0
	var month_days: int = data["calendar"][0]["day_kind"][0]
	if month_days <= 0:
		month_days = Calendar.days_in_month(data["year"], month)
		Calendar.populate_month_calendar(data, data["year"], month)
	var wd: int = (data["weekday"] + 1) % 7
	var queue: Array = []
	if day >= month_days:
		queue.append([0xfe, 0, 0, 0])
		day = 0
		month_off = 1
	day += 1

	var first_marker: int = data["calendar"][month_off]["marker"][mini(day, 31)]
	if first_marker == 0x0b:
		queue.append([0x41, 0, 0, 0])
		queue.append([0x10, 0, 0, 0])
		while queue.size() < 32:
			queue.append([0xff, 0, 0, 0])
		data["day_events"] = queue
		data["day_event_index"] = 0
		state.next_step()
		return ""
	if first_marker == 8 and ((data["player_flags"] >> 12) & 15) != 11 and command == 4:
		for j: int in range(6):
			var camp_poor: int = poor_result(state, command, wd + j, tables, rng)
			queue.append([0x42, 4, camp_poor, companion])
		queue.append([0x10, 0, 0, 0])
		while queue.size() < 32:
			queue.append([0xff, 0, 0, 0])
		data["day_events"] = queue
		data["day_event_index"] = 0
		state.next_step()
		return ""

	var random_count: int = 0
	while wd < 7:
		if day > month_days:
			queue.append([0xfe, 0, 0, 0])
			day = 1
			month_off = 1
		var cal: Dictionary = data["calendar"][mini(month_off, 1)]
		var marker: int = cal["marker"][mini(day, 31)]
		var kind: int = cal["day_kind"][mini(day, 31)]
		if marker in [2, 6, 0x0a, 0x0e, 0x10, 0x14]:
			queue.append([0xfd, 0, 0, 0])
		if marker == 4:
			queue.append([0x20, 0, 0, 0])
		elif marker == 0x0c:
			queue.append([0x24, 0, 0, 0])
		elif marker == 0x0d:
			queue.append([0x28, 0, 0, 0])
		elif marker == 0x0f:
			queue.append([0x2c, 0, 0, 0])
		elif marker == 0x11:
			queue.append([0x31, 0, 0, 0])
		elif marker == 0x13:
			queue.append([0x30, 0, 0, 0])
		elif marker == 2 and data["year"] == 96:
			queue.append([0x48, 10, 0, 0])
		elif marker == 2 and data["year"] == 97:
			queue.append([0x43, 0, 0, 0])
		elif kind in [1, 2]:
			queue.append([0x10, 0, 0, 0])
		elif marker == 0x12 and data["year"] == 98:
			queue.append([0x4a, 0, 0, 0])
		elif marker == 5 or (not queue.is_empty() and queue[0][0] == 0x40):
			queue.append([0x40, 0, 0, 0])
		else:
			var poor: int = poor_result(state, command, wd, tables, rng)
			queue.append([0, command, 0 if command == 7 else poor, companion])
			if kind == 0 and random_count < 2 and (rng.next("src/main/scheduler.c::day_event_queue_build_week/encounter") & 3) == 3:
				random_count += 1
				queue.append([0x44, 0, 0, 0])
		var eff_month: int = month + month_off
		if Calendar.birthday_matches(data["girls"][0]["birthday"], eff_month, day):
			queue.append([0x32, 0, 0, 0])
		for g_idx: int in range(1, 11):
			if (data["girls"][g_idx]["flags"] & 0x42) == 0x42 and Calendar.birthday_matches(data["girls"][g_idx]["birthday"], eff_month, day):
				queue.append([0x32, g_idx, 0, 0])
		if ((data["player_flags"] >> 8) & 15) != 3 and Calendar.birthday_matches(data["player_birthday"], eff_month, day):
			queue.append([0x33, 0, 0, 0])
		if wd == 5:
			queue.append([0xfc, 0, 0, 0])
		if wd != 6:
			queue.append([0xfb, 0, 0, 0])
		wd += 1
		day += 1
	if queue.size() > 32:
		queue.resize(32)
	while queue.size() < 32:
		queue.append([0xff, 0, 0, 0])
	data["day_events"] = queue
	data["day_event_index"] = 0
	state.next_step()
	return ""

static func build_initial_week(state: State, tables: Dictionary, rng: Rng) -> String:
	var data: Dictionary = state.rule_state
	if data.get("allow_multi_week", false) and [data["year"], data["month"], data["day"], data["weekday"], data["day_count"]] != [95, 4, 4, 1, 0]:
		return build_week(state, tables, rng)
	var failure: String = initial_branch_error(state)
	if not failure.is_empty():
		return failure
	# Later April weeks run the general day_event_queue_build_week.
	if not initial_boundary(data):
		return build_week(state, tables, rng)
	var command: int = data["last_command"] & 15
	if command not in [0, 1, 2, 3, 4, 5, 6, 7]:
		return "initial queue supports only commands0-7"
	if command == 4 and ((data["player_flags"] >> 12) & 15) >= 11:
		return "initial queue club command requires valid club_id 0..10"
	var guard: String = Ordinary.profile_guard(data)
	if not guard.is_empty():
		return guard
	data["progress_state"][0x15 + command % 9] = Math.u8(data["progress_state"][0x15 + command % 9] + 1)
	if data["girls"][11]["counters"][0] != 0:
		data["girls"][11]["counters"][0] = Math.u8(data["girls"][11]["counters"][0] - 1)
	if data.get("allow_multi_week", false):
		for i: int in range(11):
			if data["girl_last_date_spot_timer"][i] != 0:
				data["girl_last_date_spot_timer"][i] = Math.u8(data["girl_last_date_spot_timer"][i] - 1)
	var selection: Dictionary = command_girl_select(data, command, tables)
	if not String(selection["error"]).is_empty():
		return selection["error"]
	var companion: int = int(selection["companion"]) & 15
	data["last_picked_girl"] = -1
	var recovery: int = (data["player_flags_upper"] >> 5) & 7
	if recovery != 0:
		data["player_flags_upper"] = (data["player_flags_upper"] & ~0xe0) | ((recovery - 1) << 5)
	Ordinary.reanchor_profile(data)
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
	if data.get("vacation", 0) != 0:
		state.next_step()
		return
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
