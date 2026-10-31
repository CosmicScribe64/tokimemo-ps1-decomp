extends SceneTree
## School-events family: native mode routing (overlay_loader.c), the birthday
## pushes of the weekly queue (scheduler.c) and the GEKO player-birthday state
## rules (player_birthday.c). Fixtures are invented values chosen so each
## source branch is taken; these are bounded rule tests, not route acceptance.
##   godot --headless --path port/godot --script res://tests/rules/run_school_events_family_tests.gd

const RuleTests = preload("res://tests/rules/run_rule_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")
const OverlayLoadTable = preload("res://rules/overlay_load_table.gd")
const BirthdayQueue = preload("res://rules/girl_encounters/birthday_queue.gd")
const PlayerBirthday = preload("res://rules/girl_encounters/player_birthday.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const Scheduler = preload("res://rules/scheduler.gd")

var failures: Array[String] = []
var checks: int = 0
var fixtures: RuleTests = RuleTests.new()


func _initialize() -> void:
	_test_routing()
	_test_birthday_check()
	_test_push_birthdays()
	_test_player_birthday_init_and_finish()
	_test_player_birthday_select()
	_test_player_birthday_log_and_inputs()
	for failure: String in failures:
		printerr("FAIL: " + failure)
	print("school-events family: %d checks, %d failures" % [checks, failures.size()])
	quit(1 if not failures.is_empty() else 0)


func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)


func _tables() -> Dictionary:
	return fixtures.synthetic_tables()


func _data() -> Dictionary:
	var data: Dictionary = fixtures.synthetic_state().rule_state
	for girl: Dictionary in data["girls"]:
		girl["flags"] = 0
		girl["birthday"] = 0
		girl["affection_word"] = 0
		girl["tokimeki_word"] = 0
	data["player_birthday"] = 0
	data["player_flags"] = 0
	data["current_girl"] = 0
	data["event_log"] = []
	data["event_log_count"] = 0
	return data


## Birthday word: month in bits 0..3, day in bits 4..8.
func _bday(month: int, day: int) -> int:
	return month | (day << 4)


func _test_routing() -> void:
	# Queue type -> native mode -> overlay, as the controller passes them.
	var expected: Dictionary = {0x32: [0x57, "GEKO"], 0x33: [0x56, "GEKO"], 0x44: [0x59, "GEKO"],
		0x45: [0x5D, "GEKO"], 0x46: [0x5A, "GEKO"], 0x41: [0x51, "SHUGAKU"], 0x42: [0x5F, "KANGEI"],
		0x48: [0x5B, "SHOUGATU"], 0x30: [0x55, "SHOUGATU"], 0x20: [0x72, "TAIIKU"]}
	for event: int in expected.keys():
		var native: int = DayEventModes.mode_for(event, 0)
		_check(native == int(expected[event][0]), "event 0x%02X sets native 0x%02X (got 0x%02X)" % [event, expected[event][0], native])
		_check(OverlayLoadTable.overlay_for(native) == String(expected[event][1]), "native 0x%02X loads %s" % [native, expected[event][1]])
		var disp: int = DayEventModes.dispatcher_mode_for(native, event)
		_check(disp == OverlayLoadTable.dispatcher_mode_for_native(native),
			"event 0x%02X routes to the %s catalog key (got 0x%02X)" % [event, expected[event][1], disp])
	_check(DayEventModes.mode_for(0x24, 5) == 0x70 and DayEventModes.mode_for(0x24, 6) == 0x71, "culture festival picks 0x70 below club 6, 0x71 from club 6")
	_check(DayEventModes.dispatcher_mode_for(0x70, 0x24) == 0x48 and DayEventModes.dispatcher_mode_for(0x71, 0x24) == 0x4A, "culture festival modes route to BUNKASAI/BUNKAKEN")
	# A catalog key passed with an unrelated event keeps the catalog key.
	_check(DayEventModes.dispatcher_mode_for(OverlayDispatcher.GAME_MODE_TEL, 0x46) == OverlayDispatcher.GAME_MODE_TEL, "TEL catalog key is not rerouted by event 0x46")
	# Main-resident modes have no overlay.
	for mode: int in [0x30, 0x31, 0x34, 0x40, 0x41, 0x42, 0x46]:
		_check(OverlayLoadTable.overlay_for(mode).is_empty() and OverlayLoadTable.dispatcher_mode_for_native(mode) == -1, "native 0x%02X is main-resident" % mode)
	_check(OverlayLoadTable.lba_for(0x56) == 0x946A and OverlayLoadTable.lba_for(0x51) == 0x98EA, "GEKO and SHUGAKU LBAs")
	# Production refusal names the GEKO overlay for the player birthday.
	var dispatcher: OverlayDispatcher = OverlayDispatcher.new()
	var data: Dictionary = _data()
	var ovl: Dictionary = dispatcher.init_overlay(0x56, data, [0x33, 0, 0, 0])
	_check(int(ovl["dispatcher_mode"]) == OverlayDispatcher.GAME_MODE_GEKO, "player birthday overlay state carries the GEKO key")
	var before: Dictionary = data.duplicate(true)
	var out: Dictionary = dispatcher.step_overlay(0x56, ovl, OverlayDispatcher.PAD_CIRCLE, data)
	_check(String(out.get("error", "")).contains("refused with saved state") and String(out["error"]).contains("player-birthday"),
		"saved-state player birthday is refused with the GEKO reason (%s)" % out.get("error", ""))
	_check(data == before and not bool(out.get("completed", true)), "refusal writes nothing")


func _test_birthday_check() -> void:
	var data: Dictionary = _data()
	data["player_birthday"] = _bday(1, 9)
	data["girls"][3]["birthday"] = _bday(4, 30)
	_check(BirthdayQueue.birth_day_check_days(data, 0xF, 1, 9) == 1, "player birthday matches")
	_check(BirthdayQueue.birth_day_check_days(data, 0xF, 0xD, 9) == 1, "month 0xD reads as January")
	_check(BirthdayQueue.birth_day_check_days(data, 0xF, 0xE, 9) == 0, "month 0xE is not folded")
	_check(BirthdayQueue.birth_day_check_days(data, 0x1F, 1, 9) == 1, "subject uses its low nibble")
	_check(BirthdayQueue.birth_day_check_days(data, 3, 4, 30) == 1 and BirthdayQueue.birth_day_check_days(data, 3, 4, 29) == 0, "girl birthday day match")
	# Bits above bit 8 of the word do not take part.
	data["girls"][3]["birthday"] = _bday(4, 30) | (0x7F << 9)
	_check(BirthdayQueue.birth_day_check_days(data, 3, 4, 30) == 1, "high birthday bits are ignored")


func _test_push_birthdays() -> void:
	var data: Dictionary = _data()
	data["month"] = 12
	data["girls"][0]["birthday"] = _bday(1, 3)          # Shiori: no met/b6 test
	data["girls"][2]["birthday"] = _bday(1, 3)
	data["girls"][2]["flags"] = 2 | 0x40                # met + b6: pushed
	data["girls"][4]["birthday"] = _bday(1, 3)
	data["girls"][4]["flags"] = 2                       # met only: skipped
	data["girls"][5]["birthday"] = _bday(1, 3)
	data["girls"][5]["flags"] = 1 | 0x40                # bit 0 is not met: skipped
	data["girls"][11]["birthday"] = _bday(1, 3)
	data["girls"][11]["flags"] = 2 | 0x40               # outside 1..10: skipped
	data["player_birthday"] = _bday(1, 3)
	data["day_events"][5] = [0xFF, 9, 7, 6]
	data["day_events"][6] = [0xFF, 9, 7, 6]
	data["day_events"][7] = [0xFF, 9, 7, 6]
	var out: Dictionary = BirthdayQueue.push_birthdays(data, 5, 1, 3)
	_check(out["error"].is_empty() and int(out["index"]) == 8, "three records pushed (got %s)" % str(out))
	_check(data["day_events"][5] == [0x32, 0, 7, 6] and data["day_events"][6] == [0x32, 2, 7, 6],
		"girl records write type and girl only")
	_check(data["day_events"][7] == [0x33, 9, 7, 6], "player record writes the type only")
	# Sick player: no player birthday.
	data = _data()
	data["player_birthday"] = _bday(4, 10)
	data["player_flags"] = 3 << 8
	out = BirthdayQueue.push_birthdays(data, 0, 0, 10)
	_check(int(out["index"]) == 0, "health 3 suppresses the player birthday")
	data["player_flags"] = 2 << 8
	out = BirthdayQueue.push_birthdays(data, 0, 0, 10)
	_check(int(out["index"]) == 1, "other health values keep it")
	# Overflow is refused, not written.
	data = _data()
	data["girls"][0]["birthday"] = _bday(4, 10)
	out = BirthdayQueue.push_birthdays(data, 32, 0, 10)
	_check(not String(out["error"]).is_empty() and int(out["index"]) == 32, "push past the queue is refused")


func _test_player_birthday_init_and_finish() -> void:
	var data: Dictionary = _data()
	data["day_event_done_bits"] = 0x10
	var first: Dictionary = PlayerBirthday.init_step(data)
	_check(not bool(first["finish"]) and int(data["day_event_done_bits"]) == 0x14, "init sets done bit 0x04")
	var again: Dictionary = PlayerBirthday.init_step(data)
	_check(bool(again["finish"]) and int(data["day_event_done_bits"]) == 0x14, "second init finishes without changes")
	data["day_event_index"] = 3
	var fin: Dictionary = PlayerBirthday.finish(data)
	_check(int(fin["mode"]) == 0x31 and int(data["day_event_index"]) == 4, "finish runs day_event_next(1)")
	data["day_event_done_bits"] = 0x40
	fin = PlayerBirthday.finish(data)
	_check(int(fin["mode"]) == 0x52 and int(fin["audio_command"]) == 0xCF and int(data["day_event_index"]) == 4, "done bit 0x40 redirects to NEW_YEAR_A")
	data["day_event_index"] = 255
	data["day_event_done_bits"] = 0
	PlayerBirthday.finish(data)
	_check(int(data["day_event_index"]) == 0, "day_event_index is a byte")
	data["day"] = 7
	data["calendar"][0]["marker"][7] = 5
	_check(PlayerBirthday.audio_delay_ticks(data) == 0x80, "marker 5 holds 128 ticks")
	data["calendar"][0]["marker"][7] = 4
	_check(PlayerBirthday.audio_delay_ticks(data) == 0, "other markers do not")
	data["day_event_done_bits"] = 0x08
	_check(not PlayerBirthday.chime_enabled(data), "done bit 0x08 suppresses the chime")


func _set_girl(data: Dictionary, girl: int, affection: int, tokimeki: int, met: bool) -> void:
	data["girls"][girl]["affection_word"] = affection << 16
	data["girls"][girl]["tokimeki_word"] = tokimeki << 16
	data["girls"][girl]["flags"] = 2 if met else 1      # bit 0 alone is not met


func _test_player_birthday_select() -> void:
	var tables: Dictionary = _tables()
	# Lower state wins: affection 60/tokimeki 60 -> state 1; affection 60/tokimeki 10 -> state 2.
	var data: Dictionary = _data()
	_set_girl(data, 2, 60, 10, true)
	_set_girl(data, 5, 60, 60, true)
	_set_girl(data, 7, 90, 90, false)                  # not met
	var sel: Dictionary = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 5 and int(sel["next_step"]) == 2 and int(data["current_girl"]) == 5 and int(sel["section"]) == 7,
		"state 1 beats state 2; Yukari uses section 7 (%s)" % str(sel))
	# Equal state: strictly greater sum wins, an exact tie keeps the incumbent.
	data = _data()
	_set_girl(data, 1, 60, 60, true)
	_set_girl(data, 3, 61, 60, true)
	_set_girl(data, 6, 61, 60, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 3, "greater sum wins, equal sum keeps the earlier girl (%s)" % str(sel))
	# State >= 2 for the winner: default visitor (step 0x17), current girl kept.
	data = _data()
	data["current_girl"] = 9
	_set_girl(data, 4, 60, 10, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["next_step"]) == 0x17 and int(sel["girl"]) == 4 and int(data["current_girl"]) == 9, "state 2 winner goes to step 0x17")
	# No met girl: no winner.
	data = _data()
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 0xFF and int(sel["next_step"]) == 0x17, "no met girl -> 0xFF and step 0x17")
	# The 0xFF incumbent compares against girl 8's sums: a state-4 girl wins
	# only with a greater sum than girl 8 (who need not be met).
	data = _data()
	_set_girl(data, 8, 5, 50, false)
	_set_girl(data, 0, 5, 40, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 0xFF, "state-4 candidate below girl 8's sum is not taken")
	_set_girl(data, 0, 5, 60, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 0 and int(sel["next_step"]) == 0x17, "state-4 candidate above girl 8's sum is taken, then step 0x17")
	# Done bit 0x80 skips the girl whose id + 1 is the talk selector.
	data = _data()
	_set_girl(data, 1, 60, 60, true)
	_set_girl(data, 2, 60, 59, true)
	data["day_event_done_bits"] = 0x80
	data["talk_girl_selector_plus_one"] = 2
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 2 and int(data["talk_girl_selector_plus_one"]) == 0, "bit 0x80 excludes girl 1 and clears the selector")
	# Done bit 0x08 forces the default visitor.
	data = _data()
	_set_girl(data, 1, 60, 60, true)
	data["day_event_done_bits"] = 0x08
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["next_step"]) == 0x17 and int(sel["girl"]) == 1, "done bit 0x08 -> step 0x17")
	# The source loop runs while (i < 11): girl 10 (Yumi) is the last candidate.
	data = _data()
	_set_girl(data, 10, 60, 60, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 10 and int(sel["next_step"]) == 2 and int(data["current_girl"]) == 10 and int(sel["section"]) == 12,
		"girl 10 alone met at state 1 is the visitor with section 12 (%s)" % str(sel))
	# Girl 11 is past the loop bound: met at state 0 she is never considered.
	data = _data()
	_set_girl(data, 11, 90, 90, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 0xFF and int(sel["next_step"]) == 0x17, "girl 11 met at state 0 is not a candidate (%s)" % str(sel))
	_set_girl(data, 10, 60, 60, true)
	sel = PlayerBirthday.select_girl(data, tables)
	_check(int(sel["girl"]) == 10, "girl 10 at state 1 wins over a state-0 girl 11 (%s)" % str(sel))
	# Section table follows the source switch.
	var sections: Array = []
	for girl: int in range(11):
		sections.append(int(PlayerBirthday.GIRL_SCRIPT_SECTION[girl]))
	_check(sections == [1, 6, 3, 4, 10, 7, 8, 5, 0, 9, 12] and PlayerBirthday.DEFAULT_SCRIPT_SECTION == 11, "girl script sections")


func _test_player_birthday_log_and_inputs() -> void:
	var tables: Dictionary = _tables()
	var data: Dictionary = _data()
	data["current_girl"] = 5
	_set_girl(data, 5, 60, 60, true)
	data["event_log_count"] = 2
	var entry: Dictionary = PlayerBirthday.closing_event_log(data, tables)
	_check(int(entry["scene"]) == 0xD and int(entry["flag"]) == 0x25 and int(entry["girl"]) == 5, "closing log scene 0xD flag 0x25")
	_check(int(entry["rank"]) == Scheduler.girl_log_rank(data, 5, tables) and int(data["event_log_count"]) == 3 and data["event_log"][2] == entry, "rank from get_g_zyotai_h, appended at count")
	data["current_girl"] = 12
	data["event_log_count"] = 0xFE
	entry = PlayerBirthday.closing_event_log(data, tables)
	_check(int(entry["rank"]) == 0 and int(data["event_log_count"]) == 0xFE, "Rei logs rank 0; count stays at 0xFE")
	var missing: Dictionary = _data()
	missing.erase("event_log")
	_check(PlayerBirthday.missing_input(missing, tables).contains("event_log"), "missing event log is refused")
	missing = _data()
	missing["day_event_done_bits"] = 0x80
	_check(PlayerBirthday.missing_input(missing, tables).contains("talk_girl_selector_plus_one"), "bit 0x80 without the selector is refused")
	_check(PlayerBirthday.missing_input(_data(), {}).contains("validated rule tables"), "missing rank tables are refused")
	_check(PlayerBirthday.missing_input(_data(), tables).is_empty(), "complete inputs accepted")
