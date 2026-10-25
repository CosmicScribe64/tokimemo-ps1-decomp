extends RefCounted
## Invented public fixtures. Real table/route comparisons are separate ignored proof.

const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const SchoolDay = preload("res://rules/school_day.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const Command = preload("res://flow/command_screen.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")

var failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func _zeroes(count: int, value: int = 0) -> Array:
	var output: Array = []
	output.resize(count)
	output.fill(value)
	return output

func synthetic_tables() -> Dictionary:
	var tables: Dictionary = {}
	for key: String in Controller.TABLE_SHAPES:
		var shape: Array = Controller.TABLE_SHAPES[key]
		if shape.size() == 1:
			tables[key] = _zeroes(shape[0])
		else:
			var rows: Array = []
			for index: int in range(shape[0]):
				rows.append(_zeroes(shape[1]))
			tables[key] = rows
	tables["affection_weights"] = _zeroes(13, 100)
	tables["tokimeki_weights"] = _zeroes(13, 100)
	tables["command0_rank_threshold"] = [10]
	# Invented deltas with independently worked binary fixed-point outcomes.
	tables["command_deltas"][0] = [-8, 16, 0, 0, 0, 0, 0, 0, 8]
	tables["command_deltas"][7] = [16, 0, 0, 0, 0, 0, 0, 0, -16]
	tables["sd_select_non_summer"][1] = 3
	tables["sd_select_non_summer"][13] = 16
	for index: int in range(8):
		tables["weekday_background_lbas"][index] = 1000 + index
	for index: int in range(25):
		tables["weekday_sd_lbas"][index] = 2000 + index
	return tables

func synthetic_state() -> State:
	var state: State = State.new()
	state.game_mode = 0x34
	state.mode_stage = 1
	var girls: Array = []
	for index: int in range(16):
		girls.append({"tokimeki_word": 0, "affection_word": 50 << 16, "syoushin_word": 0,
			"flags": 2 if index == 0 else 0, "birthday": 0x117, "counters": _zeroes(36)})
	var calendar: Array = []
	for index: int in range(2):
		calendar.append({"day_kind": _zeroes(32), "marker": _zeroes(32)})
	calendar[0]["day_kind"][0] = 30
	calendar[0]["day_kind"][9] = 1
	var queue: Array = []
	for index: int in range(32):
		queue.append([0xff, 0, 0, 0])
	state.rule_state = {"year": 95, "month": 4, "day": 4, "weekday": 1, "day_count": 0,
		"calendar": calendar, "params": [10 << 16, 1 << 16, 0, 0, 0, 0, 0, 0, 10 << 16],
		"player_flags": 11 << 12, "player_flags_upper": 0, "player_birthday": 0x117,
		"girls": girls, "progress_state": _zeroes(156), "day_events": queue,
		"day_event_index": 0, "scene_flags": _zeroes(252), "day_event_done_bits": 0,
		"day_event_count_today": 0, "daily_check_done": 0, "last_command": 0,
		"menu_cursor": 0, "menu_mode": 1, "menu_disabled": _zeroes(14),
		"icon_enabled": _zeroes(15, 1), "vacation": 0, "sd_anim_fast": 0,
		"memory_card_job": 0, "memory_card_busy": 0, "girl_last_date_spot_timer": _zeroes(16),
		"appointment_words": _zeroes(8), "last_picked_girl": -1, "actor_state": {},
		"resources": {"available_commands": [0, 7]}, "scene_work": 0,
		"background_cache": [0, 0, 0, 0], "background_show": 0}
	return state

func run() -> Array[String]:
	failures.clear()
	_test_command_selection()
	_test_fixed_point()
	_test_real_queue_shape()
	_test_gate_hold_and_timeout()
	_test_extended_rollback()
	_test_resource_gates()
	_test_missing_capability_and_tables()
	return failures

func _test_command_selection() -> void:
	var state: State = synthetic_state()
	for index: int in range(7):
		Command.step(state, InputFrame.new(index, 0x2000), Session.FrameResult.new())
	_check(state.rule_state["menu_cursor"] == 7, "seven native directional edges select command7, including disabled-looking icons")
	var result: Session.FrameResult = Session.FrameResult.new()
	Command.step(state, InputFrame.new(8, 0x20), result)
	_check(result.error.is_empty() and state.game_mode == 0x30 and (state.rule_state["last_command"] & 15) == 7, "source command7 confirmation enters scheduler")
	state = synthetic_state()
	state.rule_state["icon_enabled"][0] = 0
	Command.step(state, InputFrame.new(0, 0x20), Session.FrameResult.new())
	_check(state.game_mode == 0x34, "get_icon_can_use blocks disabled progression")

func _test_fixed_point() -> void:
	var state: State = synthetic_state()
	var tables: Dictionary = synthetic_tables()
	_check(SchoolDay.apply_parameters(state, 0, 0, 0, tables).is_empty(), "supported integer parameter change")
	_check(state.rule_state["params"][0] == 622592 and state.rule_state["params"][1] == 131072 and state.rule_state["params"][8] == 688128, "invented delta row yields 9.5 condition,2 liberal arts,10.5 stress")
	state = synthetic_state()
	SchoolDay.apply_parameters(state, 7, 0, 1, tables)
	_check(state.rule_state["params"][0] == 688128 and state.rule_state["params"][8] == 688128, "poor result halves positive recovery and scales negative stress plus source penalty")

func _test_real_queue_shape() -> void:
	var state: State = synthetic_state()
	var rng: Rng = Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var tables: Dictionary = synthetic_tables()
	_check(Scheduler.build_initial_week(state, tables, rng).is_empty(), "source initial-week builder runs")
	var school: int = 0
	var day_end: int = 0
	var week_end: int = 0
	var holiday: int = 0
	var encounters: int = 0
	for event: Array in state.rule_state["day_events"]:
		match event[0]:
			0: school += 1
			0xfb: day_end += 1
			0xfc: week_end += 1
			0x10: holiday += 1
			0x44: encounters += 1
	_check(school == 4 and day_end == 4 and week_end == 1 and holiday == 1, "complete initial queue contains four activities,day ends,week-end and holiday")
	_check(encounters == 2, "invented seed preserves actual random encounter placeholders")
	_check(Scheduler.resolve_initial_encounters(state, tables, rng).is_empty(), "initial source encounter resolver runs")
	var skips: int = 0
	for event: Array in state.rule_state["day_events"]:
		if event[0] == 0xfa:
			skips += 1
	_check(skips == 2, "source resolver preserves two skipped encounter queue records")
	Scheduler.weekly_girl_update(state, tables)
	_check(State.high(state.rule_state["girls"][0]["affection_word"]) == 49, "weekly affection decay is applied, not skipped")

func _actor_state(state: State) -> void:
	state.game_mode = 0x40
	state.mode_stage = 1
	state.rule_state["day"] = 5
	state.rule_state["day_count"] = 1
	state.rule_state["day_events"][0] = [0, 0, 0, 15]
	state.rule_state["actor_state"] = {"96": {"flags": 5, "sequence_slot": 0, "sequence": {"counter": 0, "active": true}}}

func _test_gate_hold_and_timeout() -> void:
	var state: State = synthetic_state()
	_actor_state(state)
	var before: Array = state.rule_state["params"].duplicate()
	SchoolDay.main_step(state, synthetic_tables(), Session.FrameResult.new())
	_check(state.rule_state["params"] == before and state.mode_stage == 1, "active native bit0 prevents parameter award")
	state.rule_state["actor_state"]["96"]["flags"] = 0
	SchoolDay.main_step(state, synthetic_tables(), Session.FrameResult.new())
	var awarded: Array = state.rule_state["params"].duplicate()
	for index: int in range(12):
		SchoolDay.main_step(state, synthetic_tables(), Session.FrameResult.new())
	_check(state.mode_stage == 1 and state.rule_state["params"] == awarded, "source result hold does not award twice or exit before old counter13")
	SchoolDay.main_step(state, synthetic_tables(), Session.FrameResult.new())
	_check(state.mode_stage == 2, "old counter13 advances to source exit stage")
	state = synthetic_state()
	_actor_state(state)
	state.mode_step_timer = 600
	before = state.rule_state["params"].duplicate()
	SchoolDay.main_step(state, synthetic_tables(), Session.FrameResult.new())
	_check(state.mode_stage == 2 and state.rule_state["params"] == before, "native601 timeout exits active animation without inventing award")

func _empty(_state: CoreState, _rng: Rng, _result: Session.FrameResult) -> void:
	pass

const CoreState = preload("res://core/game_state.gd")

func _test_extended_rollback() -> void:
	var state: State = synthetic_state()
	_actor_state(state)
	state.rule_state["actor_state"]["96"]["flags"] = 0
	var session: Session = Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller: Controller = Controller.new(synthetic_tables())
	_check(controller.attach(session, state).is_empty(), "bounded controllers bind actual native modes")
	session.cd_logic = _empty
	session.audio_logic = _empty
	session.draw_logic = func(_state: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = "invented required renderer failure"
	var before: String = session.state_hash()
	var result: Session.FrameResult = session.step(InputFrame.new(0), [])
	_check(result.error == "invented required renderer failure" and not result.committed and session.state_hash() == before, "nested params/actor counter rollback extends core transaction/hash")
	_check(State.high(state.rule_state["params"][1]) == 1 and state.rule_state["actor_state"]["96"]["sequence"]["counter"] == 0, "rolled-back award restores extended records")

func _resource_event(session: Session, request: String, checksum: bool = true) -> Array[Session.ServiceEvent]:
	var events: Array[Session.ServiceEvent] = [Session.ServiceEvent.new(session.clock.logic_iterations,
		Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, "school_day.resources",
		{"request": request, "ready": true, "lba": session.state.rule_state["resources"][request]["lba"], "decode_result": 1, "checksum_ok": checksum})]
	return events

func _step(session: Session, events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	return session.step(InputFrame.new(session.clock.logic_iterations), events)

func _test_resource_gates() -> void:
	var state: State = synthetic_state()
	state.game_mode = 0x40
	state.mode_stage = 0
	state.rule_state["day"] = 5
	state.rule_state["day_events"][0] = [0, 0, 0, 15]
	var session: Session = Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller: Controller = Controller.new(synthetic_tables())
	_check(controller.attach(session, state).is_empty(), "resource-gated controller setup")
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty
	var result: Session.FrameResult = _step(session)
	_check(result.error.is_empty() and state.substep == 1 and state.mode_step == 0, "background read requested before SD progression")
	_step(session)
	_check(state.substep == 1 and state.mode_step == 0, "background wait retains native setup boundary")
	_step(session, _resource_event(session, "background"))
	_check(state.substep == 2 and state.mode_step == 0, "successful native decode reaches final background substep")
	_step(session)
	_step(session)
	_check(state.mode_step == 1 and state.substep == 1 and state.rule_state["scene_work"] == 3, "original selector controls pending SD request")
	_step(session, _resource_event(session, "sd", false))
	_check(state.mode_step == 1 and state.substep == 0, "failed SD checksum retries without advancing")
	_step(session)
	_step(session, _resource_event(session, "sd"))
	_check(state.mode_step == 2, "valid SD checksum permits actor setup")
	result = _step(session)
	_check(result.error.is_empty() and state.mode_stage == 1 and state.rule_state["actor_state"].keys() == ["96"], "extra15 spawns only source actor96")
	var events: Array[Session.ServiceEvent] = [Session.ServiceEvent.new(session.clock.logic_iterations,
		Session.Phase.BEFORE_MODE, Session.ServiceKind.COMPLETION, "school_day.actor96",
		{"active": false, "counter": 0, "sequence_slot": 0})]
	result = _step(session, events)
	_check(result.error.is_empty() and State.high(state.rule_state["params"][1]) == 2, "explicit sourced terminal event unlocks the result")
	_check(state.rule_state["actor_state"]["96"]["sequence"]["counter"] == 1, "result hold aliases actor duration counter")

func _test_missing_capability_and_tables() -> void:
	var broken: Dictionary = synthetic_tables()
	broken["sd_select_non_summer"][0] = 25
	_check(not Controller.new(broken).error.is_empty(), "out-of-range SD table selector rejected before lookup")
	var state: State = synthetic_state()
	var session: Session = Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller: Controller = Controller.new(synthetic_tables())
	controller.attach(session, state)
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty
	state.rule_state["menu_cursor"] = 6
	state.rule_state["resources"]["available_commands"] = [0]
	session.latched_input = InputFrame.new(-1, 0x2020)
	var before: String = session.state_hash()
	var result: Session.FrameResult = _step(session)
	_check(not result.error.is_empty() and not result.committed and session.state_hash() == before, "same-frame new selection checks its own missing capability before progression")
