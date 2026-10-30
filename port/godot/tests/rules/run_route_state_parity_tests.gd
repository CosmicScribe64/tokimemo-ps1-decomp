extends RefCounted
## T-39006 (review R2, R13): production handlers read saved player/heroine
## state. Fixtures are invented public values; the culture-festival case also
## replays the emulator check of wiki/spec/o-bunkasai.md 8.1. These are bounded
## rule tests, not actual-route acceptance.

const RuleTests = preload("res://tests/rules/run_rule_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const CultureFestival = preload("res://rules/culture_festival.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")

var failures: Array[String] = []
var fixtures: RuleTests = RuleTests.new()

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append("route-state parity: " + message)

func _tables() -> Dictionary:
	var tables: Dictionary = fixtures.synthetic_tables()
	# Closeness = 5 * (weighted affection / 5): rank follows saved affection.
	for t: int in range(20):
		for a: int in range(20):
			tables["closeness_grid"][t][a] = a * 5
	tables["command_rank_bars"] = [10, 20, 30, 40, 50]
	tables["command0_rank_threshold"] = [10]
	return tables

func _state() -> State:
	var state: State = fixtures.synthetic_state()
	for girl: Dictionary in state.rule_state["girls"]:
		girl["affection_word"] = 0
		girl["flags"] = 0
	return state

func _set_girl(state: State, girl: int, affection: int, flags: int) -> void:
	state.rule_state["girls"][girl]["affection_word"] = affection << 16
	state.rule_state["girls"][girl]["flags"] = flags

func _rng(seed_word: int) -> Rng:
	var rng: Rng = Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, seed_word)
	return rng

func run() -> Array[String]:
	failures.clear()
	_test_companion_select()
	_test_companion_select_branches()
	_test_girl_ranks()
	_test_companion_queue_and_school_day()
	_test_production_rejections()
	_test_culture_festival_rules()
	_test_culture_festival_branches()
	_test_culture_festival_rejections_keep_rng()
	_test_culture_festival_through_router()
	return failures

func _companion(state: State, command: int, tables: Dictionary) -> int:
	var out: Dictionary = Scheduler.command_girl_select(state.rule_state, command, tables)
	return -1 if not String(out["error"]).is_empty() else int(out["companion"])

func _test_companion_select() -> void:
	var tables: Dictionary = _tables()
	var state: State = _state()
	_set_girl(state, 0, 0, 2)
	_check(_companion(state, 1, tables) == 15, "science with low Shiori rank and Yuina unmet has no companion")
	_set_girl(state, 0, 80, 2)
	_check(_companion(state, 1, tables) == 0, "raising saved Shiori affection above the science bar selects her")
	_set_girl(state, 2, 95, 2)
	_check(_companion(state, 1, tables) == 2, "met Yuina ranked above Shiori takes the science companion slot")
	_set_girl(state, 2, 95, 0)
	_check(_companion(state, 1, tables) == 0, "clearing Yuina's saved met bit returns the slot to Shiori")
	# Athletics: two candidates; the larger score wins only above the self score.
	state = _state()
	_set_girl(state, 0, 0, 2)
	_set_girl(state, 5, 60, 2)
	_set_girl(state, 6, 80, 2)
	_check(_companion(state, 3, tables) == 6, "athletics picks the higher-ranked of girls 5 and 6")
	_set_girl(state, 6, 40, 2)
	_check(_companion(state, 3, tables) == 5, "lowering girl 6 hands athletics to girl 5")
	# Club command: player club 2, Yuina met + b2 + club 2.
	state = _state()
	state.rule_state["player_flags"] = 2 << 12
	_set_girl(state, 0, 0, 2 | (5 << 20))
	_set_girl(state, 2, 60, 2 | 4 | (2 << 20))
	_check(_companion(state, 4, tables) == 2, "science-club member Yuina accompanies the club command")
	_set_girl(state, 2, 60, 2 | (2 << 20))
	_check(_companion(state, 4, tables) == 15, "without Yuina's saved b2 bit the club command is solo")
	# Missing rank bars fail closed except where command0_rank_threshold suffices.
	var old_tables: Dictionary = _tables()
	old_tables.erase("command_rank_bars")
	state = _state()
	_set_girl(state, 0, 80, 2)
	_check(_companion(state, 1, old_tables) == -1, "science companion without its rank bar is rejected, not nobody")
	_check(_companion(state, 0, old_tables) == 0, "liberal arts uses command0_rank_threshold, the same word")
	_check(_companion(state, 7, old_tables) == 15, "rest never has a companion")

## Rank bars are [10, 20, 30, 40, 50]; closeness equals 5 * (affection / 5).
func _test_companion_select_branches() -> void:
	var tables: Dictionary = _tables()
	# Command 5 (trivia, bar 30) tests girl 8; command 6 (looks, bar 50) tests girl 7.
	var state: State = _state()
	_set_girl(state, 0, 0, 2)
	_set_girl(state, 8, 80, 2)
	_check(_companion(state, 5, tables) == 8, "trivia command 5 takes met girl 8 (score 80 - 30)")
	_check(_companion(state, 6, tables) == 15, "looks command 6 does not test girl 8")
	state = _state()
	_set_girl(state, 0, 0, 2)
	_set_girl(state, 7, 80, 2)
	_check(_companion(state, 6, tables) == 7, "looks command 6 takes met girl 7 (score 80 - 50)")
	_check(_companion(state, 5, tables) == 15, "trivia command 5 does not test girl 7")
	# Club 10 with Shiori in the club: girl 10 needs met, b2 and club 10.
	state = _state()
	state.rule_state["player_flags"] = 10 << 12
	_set_girl(state, 0, 0, 2 | (10 << 20))
	_set_girl(state, 10, 80, 2 | 4 | (10 << 20))
	_check(_companion(state, 4, tables) == 10, "club 10 picks girl 10 (met, b2, club 10) over Shiori without b2")
	_set_girl(state, 10, 80, 2 | 4 | (9 << 20))
	_check(_companion(state, 4, tables) == 15, "girl 10 outside club 10 is not the club-10 companion")
	# scheduler.c 1004-1006: Shiori in the player's club without b2 scores 0.
	state = _state()
	state.rule_state["player_flags"] = 2 << 12
	_set_girl(state, 0, 80, 2 | (2 << 20))
	_check(_companion(state, 4, tables) == 15, "Shiori in the club without b2: self score 0, no companion")
	_set_girl(state, 0, 80, 2 | 4 | (2 << 20))
	_check(_companion(state, 4, tables) == 0, "Shiori in the club with b2 scores 80 - 11 and accompanies")
	# Equal first and second scores: the second girl wins (second < first is false).
	state = _state()
	_set_girl(state, 0, 0, 2)
	_set_girl(state, 5, 70, 2)
	_set_girl(state, 6, 70, 2)
	_check(_companion(state, 3, tables) == 6, "athletics tie between girls 5 and 6 goes to the second girl, 6")

## get_g_zyotai_s & 0x7F and get_g_zyotai_h (src/main/menu_speaker_girl.c),
## worked by hand with unit weights and no ladder bonus.
func _test_girl_ranks() -> void:
	var tables: Dictionary = _tables()
	var state: State = _state()
	# [affection, tokimeki, girl_state_rank, girl_log_rank]
	var cases: Array = [[5, 90, 4, 4], [20, 90, 3, 3], [40, 40, 3, 3], [40, 60, 1, 3], [60, 40, 2, 2],
		[60, 60, 1, 1], [90, 40, 2, 2], [90, 60, 0, 1], [90, 85, 0, 0]]
	for row: Array in cases:
		state.rule_state["girls"][4]["affection_word"] = int(row[0]) << 16
		state.rule_state["girls"][4]["tokimeki_word"] = int(row[1]) << 16
		var s_rank: int = Scheduler.girl_state_rank(state.rule_state, 4, tables)
		var h_rank: int = Scheduler.girl_log_rank(state.rule_state, 4, tables)
		_check(s_rank == row[2] and h_rank == row[3], "affection %d tokimeki %d gives ranks %d/%d, got %d/%d" % [row[0], row[1], row[2], row[3], s_rank, h_rank])

func _test_companion_queue_and_school_day() -> void:
	var tables: Dictionary = _tables()
	var state: State = _state()
	_set_girl(state, 0, 80, 2)
	state.rule_state["last_command"] = 1
	var rng: Rng = _rng(1)
	_check(Scheduler.build_initial_week(state, tables, rng).is_empty(), "initial week builds with a companion")
	var companions: Array = []
	for event: Array in state.rule_state["day_events"]:
		if event[0] == 0:
			companions.append(event[3])
	_check(not companions.is_empty() and companions.all(func(v: Variant) -> bool: return v == 0), "school-day records carry the selected companion 0")
	var controller: Controller = Controller.new(tables)
	_check(controller.error.is_empty(), "controller accepts the optional command_rank_bars row: " + controller.error)
	var bad: Dictionary = tables.duplicate(true)
	bad["command_rank_bars"] = [11, 20, 30, 40, 50]
	_check(not Controller.new(bad).error.is_empty(), "controller rejects rank bars that disagree with command0_rank_threshold")
	# A companion record is played (no tranche rejection); an impossible extra is rejected.
	state.rule_state["day"] = 5
	state.game_mode = 0x40
	state.mode_stage = 0
	state.mode_step = 0
	state.substep = 0
	var index: int = 0
	while state.rule_state["day_events"][index][0] != 0:
		index += 1
	state.rule_state["day_event_index"] = index
	var result: Session.FrameResult = Session.FrameResult.new()
	controller._school_day(state, InputFrame.new(0, 0), rng, result)
	_check(not result.error.contains("companion"), "companion school day is not rejected: " + result.error)
	state.rule_state["day_events"][index][3] = 12
	result = Session.FrameResult.new()
	controller._school_day(state, InputFrame.new(0, 0), rng, result)
	_check(result.error.contains("not a command_girl_select result"), "companion 12 is rejected")
	# Club command SD variant follows the record's companion.
	state.rule_state["player_flags"] = 2 << 12
	state.rule_state["day_events"][index] = [0, 4, 0, 0]
	var with_shiori: Dictionary = controller._load_descriptor(state, "sd")
	state.rule_state["day_events"][index] = [0, 4, 0, 15]
	var solo: Dictionary = controller._load_descriptor(state, "sd")
	_check(with_shiori["sd_set_id"] == 5 and solo["sd_set_id"] == 7, "science club April uses club_sd_set_other variant 0 for companion 0, else 1")

func _culture_state(club: int, year: int) -> State:
	var state: State = _state()
	var data: Dictionary = state.rule_state
	data["year"] = year
	data["month"] = 10
	data["day"] = 15
	data["weekday"] = 6
	data["player_flags"] = club << 12
	data["clubs"] = []
	for index: int in range(12):
		data["clubs"].append({"practice_points": 16 if index == 0 else 0})
	data["params"][1] = 40 << 16
	data["event_log"] = []
	data["event_log_count"] = 0
	data["timer_ticks"] = 0x1234
	for index: int in range(16):
		data["girls"][index]["tokimeki_word"] = 10 << 16
		data["girls"][index]["affection_word"] = 50 << 16
		data["girls"][index]["flags"] = 11 << 20
		data["girls"][index]["birthday"] = 0x313
	data["girls"][0]["flags"] = 2 | (11 << 20)
	return state

func _run_culture(state: State, tables: Dictionary, seed_word: int = 7) -> Dictionary:
	var dispatcher: OverlayDispatcher = OverlayDispatcher.new()
	dispatcher.rule_tables = tables
	var data: Dictionary = state.rule_state
	var ovl: Dictionary = dispatcher.init_overlay(0x70, data, [0x24, 0, 0, 0])
	var rng: Rng = _rng(seed_word)
	var first: Dictionary = dispatcher.step_overlay(0x70, ovl, OverlayDispatcher.PAD_CIRCLE, data, rng)
	if not String(first.get("error", "")).is_empty():
		return first
	return dispatcher.step_overlay(0x70, first["state"], OverlayDispatcher.PAD_CIRCLE, data, rng)

func _tokimeki(girls: Array, index: int) -> int:
	return State.high(int(girls[index]["tokimeki_word"]))

func _test_culture_festival_rules() -> void:
	var tables: Dictionary = _tables()
	# Emulator check (o-bunkasai 8.1): club 0, 1995, practice 16, liberal arts 40, tokimeki 10, only girl 0 met.
	var state: State = _culture_state(0, 95)
	var before: Dictionary = state.rule_state.duplicate(true)
	var out: Dictionary = _run_culture(state, tables)
	_check(String(out.get("error", "")).is_empty() and bool(out.get("completed", false)), "literature 1995 festival completes: " + String(out.get("error", "")))
	_check(state.rule_state == before, "stepping the overlay does not write saved state before the controller applies it")
	var girls: Array = out["deltas"]["state_writes"]["girls"]
	var gains: Array = []
	for index: int in range(13):
		gains.append(_tokimeki(girls, index) - 10)
	_check(gains == [4, 5, 4, 4, 5, 5, 4, 4, 4, 4, 4, 4, 4], "tokimeki gains match the emulator check: %s" % [gains])
	_check(State.high(int(girls[0]["affection_word"])) == 52, "visitor girl 0 gains affection +2")
	_check((int(girls[0]["flags"]) & CultureFestival.FLAG_FESTIVAL_CALL_95) != 0, "literature exhibit visit sets festival_call_95 when rank is not 4")
	var entry: Dictionary = out["deltas"]["state_writes"]["event_log"][0]
	_check(entry["girl"] == 0 and entry["flag"] == 1 and entry["scene"] == 20 and out["deltas"]["state_writes"]["event_log_count"] == 1, "event log entry matches the emulator check (girl 0, flag 1, scene 0x14)")
	# Mutation: parameter and practice inputs change the bonus.
	var low: State = _culture_state(0, 95)
	low.rule_state["params"][1] = 0
	low.rule_state["clubs"][0] = {"practice_points": 0}
	var high: State = _culture_state(0, 95)
	high.rule_state["params"][1] = 200 << 16
	var low_out: Dictionary = _run_culture(low, tables)
	var high_out: Dictionary = _run_culture(high, tables)
	_check(_tokimeki(low_out["deltas"]["state_writes"]["girls"], 2) == 10 and _tokimeki(high_out["deltas"]["state_writes"]["girls"], 2) == 20,
		"all-zero vs 200 parameters give bonus 0 vs capped 10 (R13 counterexample)")
	# Mutation: Shiori in the player's club takes the member block (no rand, sets bit 2).
	var member: State = _culture_state(0, 95)
	member.rule_state["girls"][0]["flags"] = 2
	member.rule_state.erase("timer_ticks")
	var member_out: Dictionary = _run_culture(member, tables)
	_check(bool(member_out.get("completed", false)) and (int(member_out["deltas"]["state_writes"]["girls"][0]["flags"]) & 4) != 0, "Shiori club member block sets bit 2 without a visitor draw")
	_check(member_out["deltas"]["state_writes"]["event_log"][0]["flag"] == 2, "event log flag adds 1 when Shiori is in the club")
	# Visitor pick depends on saved met bits and srand(timer_ticks).
	var visitors: Dictionary = {}
	for ticks: int in range(40):
		var pick: State = _culture_state(2, 95)
		pick.rule_state["timer_ticks"] = ticks
		pick.rule_state["girls"][7]["flags"] = 2 | (11 << 20)
		var picked: Dictionary = _run_culture(pick, tables)
		if bool(picked.get("completed", false)):
			visitors[int(picked["state"]["girl_id"])] = true
	_check(visitors.keys().all(func(g: Variant) -> bool: return g in [0, 7]) and visitors.size() == 2, "visitor pick draws only met heroines 0 and 7: %s" % [visitors.keys()])
	# Fail closed on unported scene writes and missing saved inputs.
	var no_ticks: State = _culture_state(0, 95)
	no_ticks.rule_state.erase("timer_ticks")
	_check(String(_run_culture(no_ticks, tables).get("error", "")).contains("timer_ticks"), "visitor pick without saved timer_ticks is rejected")
	var mio: State = _culture_state(0, 95)
	mio.rule_state["girls"][1]["flags"] = 2 | (0 << 20)
	_check(String(_run_culture(mio, tables).get("error", "")).contains("girl 1"), "Mio club-mate scenes with unported flag writes are rejected")
	var no_clubs: State = _culture_state(0, 95)
	no_clubs.rule_state.erase("clubs")
	_check(String(_run_culture(no_clubs, tables).get("error", "")).contains("practice_points"), "missing saved club practice points are rejected, not defaulted")
	_check(String(_run_culture(_culture_state(3, 96), tables).get("error", "")).contains("TT"), "computer club 1996 TT exhibit is rejected")
	# Cancel has no source exit: no effect, no completion.
	var dispatcher: OverlayDispatcher = OverlayDispatcher.new()
	dispatcher.rule_tables = tables
	var cancel_state: State = _culture_state(0, 95)
	var ovl: Dictionary = dispatcher.init_overlay(0x70, cancel_state.rule_state, [0x24, 0, 0, 0])
	var cancelled: Dictionary = dispatcher.step_overlay(0x70, ovl, OverlayDispatcher.PAD_CROSS, cancel_state.rule_state, _rng(1))
	_check(not bool(cancelled["completed"]) and cancelled["action"] == "cancel_rejected" and (cancelled["deltas"] as Dictionary).is_empty() and int(cancelled["state"]["stage"]) == 0,
		"Cancel at stage 0 completes nothing and awards nothing")

## Sets the met bit (bit 1) on exactly the listed girls, girl 0 included.
func _only_met(state: State, met: Array) -> void:
	for index: int in range(16):
		var flags: int = int(state.rule_state["girls"][index]["flags"])
		state.rule_state["girls"][index]["flags"] = (flags | 2) if index in met else (flags & ~2)

func _culture_writes(out: Dictionary) -> Dictionary:
	if not String(out.get("error", "")).is_empty() or not bool(out.get("completed", false)):
		return {}
	return out["deltas"]["state_writes"]

func _test_culture_festival_branches() -> void:
	var tables: Dictionary = _tables()
	var both_calls: int = CultureFestival.FLAG_FESTIVAL_CALL_95 | CultureFestival.FLAG_FESTIVAL_CALL_96
	# Literature visit: the festival-call bit is year-specific and skipped at rank 4 (affection < 10).
	for year: int in [95, 96]:
		var bit: int = CultureFestival.FLAG_FESTIVAL_CALL_95 if year == 95 else CultureFestival.FLAG_FESTIVAL_CALL_96
		var liked: Dictionary = _culture_writes(_run_culture(_culture_state(0, year), tables))
		_check(not liked.is_empty() and (int(liked["girls"][0]["flags"]) & both_calls) == bit, "literature %d visit at rank 2 sets only its own call bit" % year)
		var cold_state: State = _culture_state(0, year)
		cold_state.rule_state["girls"][0]["affection_word"] = 5 << 16
		var cold: Dictionary = _culture_writes(_run_culture(cold_state, tables))
		_check(not cold.is_empty() and (int(cold["girls"][0]["flags"]) & both_calls) == 0, "literature %d visit at rank 4 sets neither bit 12 nor 13" % year)
	# festival_entry.c 288-290: club 4, visitor girl 10 at rank 4 returns before the log write.
	for affection: int in [5, 50]:
		var art: State = _culture_state(4, 97)
		_only_met(art, [10])
		art.rule_state["girls"][10]["affection_word"] = affection << 16
		var writes: Dictionary = _culture_writes(_run_culture(art, tables))
		if affection == 5:
			_check(not writes.is_empty() and (writes["event_log"] as Array).is_empty() and writes["event_log_count"] == 0,
				"art club, girl-10 visitor at rank 4: no event_log entry, count unchanged")
		else:
			_check(not writes.is_empty() and writes["event_log_count"] == 1 and writes["event_log"][0]["girl"] == 10 and writes["event_log"][0]["flag"] == 17,
				"art club, girl-10 visitor above rank 4: event_log entry with flag 17")
	# Event-log rank is get_g_zyotai_h after culture_result_apply (girl 0 gets +2 affection, +4 tokimeki).
	# [start affection, start tokimeki, rank]: 52/14 -> 2, 52/64 -> 1, 52/84 -> 0, 42/14 -> 3.
	for row: Array in [[50, 10, 2], [50, 60, 1], [50, 80, 0], [40, 10, 3]]:
		var ranked: State = _culture_state(0, 95)
		ranked.rule_state["girls"][0]["affection_word"] = int(row[0]) << 16
		ranked.rule_state["girls"][0]["tokimeki_word"] = int(row[1]) << 16
		var logged: Dictionary = _culture_writes(_run_culture(ranked, tables))
		_check(not logged.is_empty() and logged["event_log"][0]["rank"] == row[2], "event log rank for affection %d tokimeki %d is %d" % row)
	# culture_visitor_pick reads girl 0's met bit like every other girl's.
	var picks: Dictionary = {}
	for ticks: int in range(20):
		var pick: State = _culture_state(2, 95)
		_only_met(pick, [7])
		pick.rule_state["timer_ticks"] = ticks
		var picked: Dictionary = _run_culture(pick, tables)
		picks[int(picked.get("state", {}).get("girl_id", -1))] = true
	_check(picks.keys() == [7], "girl 0 without her met bit is not in the visitor pool: %s" % [picks.keys()])
	var nobody: State = _culture_state(2, 95)
	_only_met(nobody, [])
	_check(String(_run_culture(nobody, tables).get("error", "")).contains("no met heroine"), "empty visitor pool is rejected (source rand() % 0)")
	# Girl-0 birthday today with day_event_done_bits bit 0 clear goes to the unported phase 9 scene.
	var birthday: State = _culture_state(0, 95)
	birthday.rule_state["girls"][0]["birthday"] = 10 | (15 << 4)
	_check(String(_run_culture(birthday, tables).get("error", "")).contains("birthday"), "girl-0 birthday on the festival day is rejected")
	birthday.rule_state["day_event_done_bits"] = 1
	_check(not _culture_writes(_run_culture(birthday, tables)).is_empty(), "birthday already handled (done bit 0 set) runs the festival")
	# Girls 9 and 10 visiting in 1995 are rejected; in 1996 the literature visit runs.
	for girl: int in [9, 10]:
		var early: State = _culture_state(0, 95)
		_only_met(early, [girl])
		var err: String = String(_run_culture(early, tables).get("error", ""))
		_check(err.contains("1995") and err.contains("speech-contest") and err.contains("megumi" if girl == 9 else "yumi"), "girl %d 1995 visit is rejected with the sourced reason: %s" % [girl, err])
		var later: State = _culture_state(0, 96)
		_only_met(later, [girl])
		_check(not _culture_writes(_run_culture(later, tables)).is_empty(), "girl %d 1996 literature visit runs" % girl)
	# practice_points must be saved as {"practice_points": int} for the player's club.
	for bad: Variant in [{}, 16, {"practice_points": "16"}]:
		var no_points: State = _culture_state(0, 95)
		no_points.rule_state["clubs"][0] = bad
		_check(String(_run_culture(no_points, tables).get("error", "")).contains("clubs[0].practice_points"), "club entry %s is rejected, not defaulted" % [bad])

## A rejected plan leaves the shared native RNG exactly as it was.
func _test_culture_festival_rejections_keep_rng() -> void:
	var tables: Dictionary = _tables()
	for met: Array in [[1], [9], [3]]:
		var state: State = _culture_state(0, 95)
		_only_met(state, met)
		var rng: Rng = _rng(77)
		rng.next("tests/rules/run_route_state_parity_tests::advance")
		var before: Dictionary = rng.checkpoint()
		var rejected: Dictionary = CultureFestival.plan(state.rule_state, tables, rng)
		var after: Dictionary = rng.checkpoint()
		_check(not String(rejected["error"]).is_empty() and after["state"] == before["state"] and after["draw_count"] == before["draw_count"] and after["trace"] == before["trace"],
			"rejected visitor %s leaves the RNG state, draw count and trace unchanged" % [met])
	var ok_state: State = _culture_state(0, 95)
	var ok_rng: Rng = _rng(77)
	var ok_before: int = ok_rng.state
	var accepted: Dictionary = CultureFestival.plan(ok_state.rule_state, tables, ok_rng)
	_check(String(accepted["error"]).is_empty() and ok_rng.state != ok_before, "an accepted visitor pick reseeds and draws")

func _test_production_rejections() -> void:
	var dispatcher: OverlayDispatcher = OverlayDispatcher.new()
	var state: State = _state()
	for mode: int in [OverlayDispatcher.GAME_MODE_GEKO, OverlayDispatcher.GAME_MODE_EXAM, OverlayDispatcher.GAME_MODE_TAIIKU,
			OverlayDispatcher.GAME_MODE_VALEN, OverlayDispatcher.GAME_MODE_MASTER, OverlayDispatcher.GAME_MODE_TEL,
			OverlayDispatcher.GAME_MODE_SHOUGATU, OverlayDispatcher.GAME_MODE_DATE]:
		var ovl: Dictionary = dispatcher.init_overlay(mode, state.rule_state)
		for pad: int in [OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CROSS]:
			var out: Dictionary = dispatcher.step_overlay(mode, ovl, pad, state.rule_state, _rng(1))
			_check(not String(out.get("error", "")).is_empty() and not bool(out["completed"]) and (out["deltas"] as Dictionary).is_empty(),
				"mode 0x%02X with saved state is rejected, not completed with invented rewards" % mode)
	# Preview stepping (no saved state) may exit but never yields rewards.
	var preview: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_EXAM, {"dispatcher_mode": OverlayDispatcher.GAME_MODE_EXAM, "stage": 0}, OverlayDispatcher.PAD_CROSS)
	_check(bool(preview["completed"]) and (preview["deltas"] as Dictionary).is_empty(), "preview exit carries no fixed reward deltas")

func _test_culture_festival_through_router() -> void:
	var tables: Dictionary = _tables()
	var controller: Controller = Controller.new(tables)
	var state: State = _culture_state(0, 95)
	state.game_mode = 0x70
	state.mode_stage = 0
	state.rule_state["day_events"][0] = [0x24, 0, 0, 0]
	state.rule_state["day_event_index"] = 0
	# The production handler reads the live core word, not a rule_state copy.
	state.timer_ticks = 0x1234
	state.rule_state.erase("timer_ticks")
	var rng: Rng = _rng(3)
	var frames: Array[int] = [0, OverlayDispatcher.PAD_CROSS, OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CIRCLE]
	var errors: Array[String] = []
	for pad: int in frames:
		var result: Session.FrameResult = Session.FrameResult.new()
		controller._overlay_mode(state, InputFrame.new(0, pad), rng, result)
		if not result.error.is_empty():
			errors.append(result.error)
	_check(errors.is_empty(), "culture festival runs through the production overlay handler: %s" % [errors])
	_check(state.game_mode == 0x31 and state.rule_state["day_event_index"] == 1, "completion advances the queue to the day screen")
	_check(_tokimeki(state.rule_state["girls"], 1) == 15 and state.rule_state["event_log_count"] == 1, "controller applied the source writes to saved state")
	_check(not state.rule_state.has("timer_ticks"), "the live timer_ticks word is not left in saved rule state")
	# The production handler seeds the visitor pick from the core timer word:
	# for each tick value it picks what the rule picks with that word.
	var matched: int = 0
	var routed_visitors: Dictionary = {}
	for ticks: int in range(24):
		var routed: State = _culture_state(2, 95)
		routed.rule_state["girls"][7]["flags"] = 2 | (11 << 20)
		var direct: State = _culture_state(2, 95)
		direct.rule_state["girls"][7]["flags"] = 2 | (11 << 20)
		direct.rule_state["timer_ticks"] = ticks
		routed.rule_state.erase("timer_ticks")
		routed.timer_ticks = ticks
		routed.game_mode = 0x70
		routed.rule_state["day_events"][0] = [0x24, 0, 0, 0]
		routed.rule_state["day_event_index"] = 0
		var routed_rng: Rng = _rng(7)
		for pad: int in [0, OverlayDispatcher.PAD_CIRCLE]:
			controller._overlay_mode(routed, InputFrame.new(0, pad), routed_rng, Session.FrameResult.new())
		var expected: Dictionary = CultureFestival.plan(direct.rule_state, tables, _rng(7))
		var picked_state: Variant = routed.rule_state.get("overlay_state", {})
		if picked_state is Dictionary and picked_state.get("girl_id") == expected.get("girl"):
			matched += 1
			routed_visitors[expected.get("girl")] = true
	_check(matched == 24 and routed_visitors.size() == 2, "router visitor pick follows srand(core timer_ticks) for every tick value: %d matched, %s" % [matched, routed_visitors.keys()])
	# The same handler fails closed for an unported event and leaves state untouched.
	var exam: State = _state()
	exam.game_mode = 0x35
	var snapshot: Dictionary = exam.rule_state.duplicate(true)
	snapshot.erase("overlay_state")
	var exam_errors: Array[String] = []
	for pad: int in [0, OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CIRCLE]:
		var result: Session.FrameResult = Session.FrameResult.new()
		controller._overlay_mode(exam, InputFrame.new(0, pad), rng, result)
		if not result.error.is_empty():
			exam_errors.append(result.error)
	var after: Dictionary = exam.rule_state.duplicate(true)
	after.erase("overlay_state")
	_check(not exam_errors.is_empty() and after == snapshot and exam.game_mode == 0x35, "exam overlay rejects in production and leaves saved state unchanged")
