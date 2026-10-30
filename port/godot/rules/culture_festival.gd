extends RefCounted
## T-39006: BUNKASAI (club-member culture festival) state effects on saved state.
## Source: src/ovl/culture_festival/festival_entry.c (culture_festival_mode_handler
## phase 8, culture_result_apply), wiki/spec/o-bunkasai.md sections 5.1, 5.2,
## 8.1, 8.2 and 8.4. Scene dialogue and media are not played here. Every scene
## branch with a state write this file does not reproduce is rejected.

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const Rng = preload("res://core/native_rng.gd")

# culture_club_param_index (0x8016052C): club -> parameter block (1 liberal arts, 2 science, 3 art).
const CLUB_PARAM_INDEX: Array[int] = [1, 1, 2, 2, 3, 3]
# culture_stage_intro club mate: clubs 0,1 -> girl 1; 2,3 -> girl 2; 4,5 -> girl 3.
const CLUB_MATE: Array[int] = [1, 1, 2, 2, 3, 3]
# culture_tokimeki_gain_95/96/97 (0x80160534/584/5D4): 13 girls x 6 clubs.
# Bytes match BUNKASAI_data.data.s and the emulator-checked spec table (o-bunkasai 8.1).
const GAIN: Dictionary = {
	95: [[1, 2, 2, 2, 2, 2], [3, 2, 1, 2, 0, 2], [0, 1, 2, 1, 2, 1], [0, 2, 1, 1, 3, 2],
		[2, 1, 1, 1, 1, 1], [2, 2, 1, 1, 1, 1], [1, 1, 0, 3, 2, 1], [1, 1, 0, 1, 3, 2],
		[0, 3, 2, 1, 2, 2], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
	96: [[1, 1, 3, 0, 0, 1], [3, 2, 2, 0, 0, 1], [0, 0, 1, 1, 0, 0], [1, 1, 1, 1, 2, 3],
		[2, 1, 1, 1, 0, 1], [2, 1, 2, 1, 1, 1], [1, 0, 3, 2, 0, 1], [1, 1, 3, 0, 0, 0],
		[0, 0, 1, 2, 0, 2], [2, 1, 3, 1, 0, 0], [0, 3, 2, 3, 0, 2], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
	97: [[2, 3, 2, 2, 3, 3], [3, 3, 1, 1, 1, 3], [0, 0, 2, 2, 3, 3], [2, 2, 3, 3, 3, 3],
		[2, 1, 3, 3, 2, 1], [1, 2, 1, 1, 2, 1], [3, 3, 2, 2, 1, 2], [1, 3, 2, 2, 2, 1],
		[0, 1, 3, 3, 1, 0], [2, 2, 2, 2, 2, 2], [2, 0, 3, 3, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
}
const FLAG_MET := 1 << 1
const FLAG_MEMBER_ENTRY := 1 << 2
const FLAG_FESTIVAL_CALL_95 := 1 << 12
const FLAG_FESTIVAL_CALL_96 := 1 << 13
const EVENT_LOG_SCENE := 20
const EVENT_LOG_CAPACITY := 0xFE

static func _club_of(data: Dictionary, girl: int) -> int:
	return (Math.u32(int(data["girls"][girl]["flags"])) >> 20) & 15

static func _missing_inputs(data: Dictionary, tables: Dictionary) -> String:
	for key: String in ["girls", "params", "player_flags", "year", "month", "day", "weekday", "day_event_done_bits"]:
		if not data.has(key):
			return "saved state lacks " + key
	if not data.get("clubs") is Array or (data["clubs"] as Array).size() < 6:
		return "saved state lacks clubs[].practice_points read by culture_result_apply"
	var club: int = (int(data["player_flags"]) >> 12) & 15
	if club < 6:
		# culture_result_apply reads only the player's club entry. The saved
		# shape is {"practice_points": int}: no default, no bare-int form.
		var entry: Variant = data["clubs"][club]
		if not entry is Dictionary or not (entry as Dictionary).get("practice_points") is int:
			return "saved state lacks clubs[%d].practice_points read by culture_result_apply" % club
	if not data.get("event_log") is Array or not data.get("event_log_count") is int:
		return "saved state lacks event_log/event_log_count written by culture_event_log_add"
	for key: String in ["affection_weights", "tokimeki_weights", "closeness_grid", "ladder_kinds", "ladder_bases", "bonus_param_masks", "param_bonus"]:
		if not tables.has(key):
			return "validated rule tables lack " + key + " (girl_state_rank inputs)"
	return ""

## Shape checked by _missing_inputs before plan() succeeds.
static func _practice_points(data: Dictionary, club: int) -> int:
	return Math.u16(int(data["clubs"][club]["practice_points"]))

## Phase 0..7 decisions that select the scene. Returns {error, girl, block}.
## The visitor pick reseeds like srand(GameState.timer_ticks) before rand().
## Rejections that do not depend on the pick run before it. A rejection after
## the pick restores the RNG checkpoint, so a refused step leaves the shared
## native RNG as it was.
static func plan(data: Dictionary, tables: Dictionary, rng: Rng) -> Dictionary:
	var missing: String = _missing_inputs(data, tables)
	if not missing.is_empty():
		return {"error": "BUNKASAI: " + missing}
	var club: int = (int(data["player_flags"]) >> 12) & 15
	var year: int = int(data["year"])
	if club > 5:
		return {"error": "BUNKASAI club %d: source resets to title (mode 0x71 BUNKAKEN owns other clubs)" % club}
	if year < 95 or year > 97:
		return {"error": "BUNKASAI year %d has no festival gain table" % year}
	var birthday: int = int(data["girls"][0]["birthday"])
	if int(data["month"]) == (birthday & 15) and int(data["day"]) == ((Math.u32(birthday) >> 4) & 31) and (int(data["day_event_done_bits"]) & 1) == 0:
		return {"error": "BUNKASAI phase 9 girl-0 birthday scene (culture_birthday_scene) is not ported"}
	# Club 3 in year 0x60 goes to the computer-game exhibit for every girl
	# (each *_visit_entry and member block), so this does not depend on the pick.
	if club == 3 and year == 96:
		return {"error": "BUNKASAI computer-club 1996 game exhibit launches TT; not chained"}
	var girl: int = -1
	var block: String = ""
	if _club_of(data, 0) == club:
		girl = 0
		block = "member"
	else:
		var mate: int = CLUB_MATE[club]
		if _club_of(data, mate) == club and (int(data["girls"][mate]["flags"]) & FLAG_MET) != 0:
			girl = mate
			block = "member"
	var checkpoint: Dictionary = {}
	if girl < 0:
		if not data.get("timer_ticks") is int:
			return {"error": "BUNKASAI culture_visitor_pick seeds srand(GameState.timer_ticks); saved state lacks timer_ticks"}
		if rng == null or not rng.configuration_error().is_empty():
			return {"error": "BUNKASAI culture_visitor_pick needs the configured native rand()"}
		# mode_stage_dispatch.c culture_visitor_pick tests the met bit of every girl 0..10.
		var met: Array[int] = []
		for index: int in range(11):
			if (int(data["girls"][index]["flags"]) & FLAG_MET) != 0:
				met.append(index)
		if met.is_empty():
			return {"error": "BUNKASAI culture_visitor_pick found no met heroine; source rand() % 0 has no defined result"}
		checkpoint = rng.checkpoint()
		rng.reseed(int(data["timer_ticks"]))
		var draw: int = rng.next("src/ovl/culture_festival::culture_visitor_pick")
		if draw < 0:
			var reason: String = rng.error
			rng.restore(checkpoint)
			return {"error": "BUNKASAI visitor rand() refused: " + reason}
		girl = met[draw % met.size()]
		block = "visit"
	var rejection: String = ""
	if girl in [1, 2, 3]:
		rejection = "BUNKASAI %s scenes of girl %d write flag bit 2 in dialogue branches that are not ported" % [block, girl]
	elif girl in [9, 10] and year == 95:
		# megumi_visit.c / yumi_visit.c *_visit_entry: club 0 sends every year
		# except 0x60 to the speech contest; other clubs have their own scene.
		var visit_file: String = "megumi" if girl == 9 else "yumi"
		rejection = ("BUNKASAI girl %d 1995 visit: culture_%s_visit_entry (src/ovl/culture_festival/%s_visit.c) routes club %d to %s, "
			+ "which is not ported or verified; the 1995 gain row for this girl is zero") % [girl, visit_file, visit_file, club,
			"the speech-contest scene" if club == 0 else "a non-0x60 scene"]
	if not rejection.is_empty():
		if not checkpoint.is_empty():
			rng.restore(checkpoint)
		return {"error": rejection}
	return {"error": "", "girl": girl, "block": block, "club": club, "year": year,
		"rank": Scheduler.girl_state_rank(data, girl, tables)}

## Applies every saved-state write of the planned run to `data` in source order:
## member-entry bit 2, scene festival-call flag, culture_result_apply, culture_event_log_add.
static func apply(data: Dictionary, festival_plan: Dictionary, tables: Dictionary) -> Dictionary:
	var club: int = festival_plan["club"]
	var year: int = festival_plan["year"]
	var girl: int = festival_plan["girl"]
	var writes: Array[String] = []
	var girls: Array = data["girls"]
	if festival_plan["block"] == "member":
		girls[girl]["flags"] = int(girls[girl]["flags"]) | FLAG_MEMBER_ENTRY
		writes.append("girls[%d].flags|=bit2 (member entry)" % girl)
	elif club == 0 and year in [95, 96] and int(festival_plan["rank"]) != 4:
		var bit: int = FLAG_FESTIVAL_CALL_95 if year == 95 else FLAG_FESTIVAL_CALL_96
		girls[girl]["flags"] = int(girls[girl]["flags"]) | bit
		writes.append("girls[%d].flags|=festival_call" % girl)
	# culture_result_apply: unsigned sum masked to a byte, capped at 10.
	var practice: int = _practice_points(data, club)
	var param: int = State.high(int(data["params"][CLUB_PARAM_INDEX[club]]))
	# ParamValue.value is s16: signed division by 20, then an unsigned byte sum.
	var bonus: int = (Math.divmodu32(practice >> 3, year - 94).quotient + Math.divmod32(param, 20).quotient) & 0xFF
	if bonus >= 11:
		bonus = 10
	var gain: Array = GAIN[year]
	for index: int in range(13):
		var tokimeki: int = State.high(int(girls[index]["tokimeki_word"]))
		tokimeki = Math.s16(tokimeki + bonus + (int(gain[index][club]) >> 1))
		girls[index]["tokimeki_word"] = State.with_high(int(girls[index]["tokimeki_word"]), tokimeki)
	var amount: int = int(gain[girl][club])
	if amount != 0:
		amount -= 1
	var affection: int = Math.s16(State.high(int(girls[girl]["affection_word"])) + 2)
	girls[girl]["affection_word"] = State.with_high(int(girls[girl]["affection_word"]), affection)
	var own: int = Math.s16(State.high(int(girls[girl]["tokimeki_word"])) + amount)
	girls[girl]["tokimeki_word"] = State.with_high(int(girls[girl]["tokimeki_word"]), own)
	writes.append("culture_result_apply bonus=%d visitor=%d amount=%d" % [bonus, girl, amount])
	# culture_event_log_add.
	var flag: int = 4 * club + 1
	var mate: int = CLUB_MATE[club]
	var mate_in_club: bool = _club_of(data, mate) == club and (int(girls[mate]["flags"]) & FLAG_MET) != 0
	if _club_of(data, 0) == club or mate_in_club:
		flag += 1
	elif club == 4 and girl == 10 and Scheduler.girl_state_rank(data, 10, tables) == 4:
		return {"writes": writes, "event_log": null}
	var entry: Dictionary = {"girl": girl, "scene": EVENT_LOG_SCENE, "flag": flag,
		"rank": Scheduler.girl_log_rank(data, girl, tables), "year": year,
		"month": int(data["month"]), "day": int(data["day"]), "weekday": int(data["weekday"])}
	var count: int = int(data["event_log_count"])
	var log: Array = data["event_log"]
	while log.size() <= count:
		log.append({})
	log[count] = entry
	data["event_log_count"] = mini(count + 1, EVENT_LOG_CAPACITY)
	writes.append("event_log[%d]" % count)
	return {"writes": writes, "event_log": entry}
