extends SceneTree
## Seasonal-events P0: calendar markers come only from the original CALROM,
## vacation changes only in the 0xFD day notice, and seasonal native modes keep
## their source identity. Needs TOKIMEMO_CALENDAR_SOURCE (original CALROM.NBN,
## read-only) and TOKIMEMO_SOURCE_ROOT (repository root with src/ and include/).
## A missing input fails the run; nothing is skipped.

const RuleTests = preload("res://tests/rules/run_rule_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Calendar = preload("res://rules/calendar.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const DayNotice = preload("res://rules/day_notice.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")
const SeasonalModes = preload("res://rules/seasonal_mode_identity.gd")
const CalendarSource = preload("res://services/native_calendar_source.gd")
const CalromMonths = preload("res://tests/fixtures/calrom_months.gd")
const Rng = preload("res://core/native_rng.gd")
const Session = preload("res://core/game_session.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const OrdinaryWeek = preload("res://rules/ordinary_week.gd")
const InputFrame = preload("res://core/input_frame.gd")

## Seasonal and notice markers read from CALROM (independent python decode,
## scratch dump of disc/files/CDROM/FUKUTEST/CALROM.NBN, 2026-10-30).
const CALROM_MARKERS: Array = [
	[95, 4, 4, 1], [95, 6, 3, 4], [96, 6, 1, 4], [97, 6, 7, 4],
	[95, 7, 24, 6], [95, 9, 1, 0x0a], [96, 9, 9, 0x0b],
	[95, 10, 14, 0x0c], [96, 10, 12, 0x0c], [97, 10, 11, 0x0c],
	[95, 12, 24, 0x0d], [95, 12, 25, 0x0e], [96, 1, 1, 0x0f], [96, 1, 8, 0x10],
	[96, 2, 14, 0x11], [98, 2, 23, 0x12], [96, 3, 14, 0x13], [97, 3, 14, 0x13],
	[96, 3, 24, 0x14], [96, 4, 4, 2], [97, 4, 4, 2], [98, 3, 1, 21],
]
## Dates the removed generator invented; CALROM has no marker there.
const INVENTED_DATES: Array = [[95, 4, 20], [95, 6, 12], [95, 11, 3], [95, 12, 23], [96, 4, 6], [96, 9, 16], [96, 3, 19]]

var checks: int = 0
var failures: Array[String] = []
var rules := RuleTests.new()
var source: CalendarSource = null
## Sessions hold controller callables weakly; keep the controllers alive.
var controllers: Array = []

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures.append(label)

func _initialize() -> void:
	source = CalromMonths.open_from_env()
	check(source != null, "original CALROM opens from TOKIMEMO_CALENDAR_SOURCE")
	_test_no_generated_calendar()
	_test_day_notice()
	_test_mode_identity_tables()
	if source != null:
		_test_calrom_markers()
		_test_rollover()
		_test_week_queue()
		_test_queued_day_notice_refusal()
		_test_graduation_rollover()
		_test_multi_day_rollover()
	_test_meeting_route()
	_test_source_switches()
	print("seasonal calendar checks=%d failures=%d" % [checks, failures.size()])
	for failure: String in failures:
		print("FAIL: " + failure)
	quit(0 if failures.is_empty() else 1)

func _state_at(year: int, month: int, day: int) -> State:
	var state: State = rules.synthetic_state()
	var data: Dictionary = state.rule_state
	data["year"] = year
	data["month"] = month
	data["day"] = day
	data["weekday"] = Calendar.weekday_for_date(year, month, day)
	data["allow_multi_week"] = true
	return state

func _test_no_generated_calendar() -> void:
	var names: Array[String] = []
	for method: Dictionary in (Calendar as Script).get_script_method_list():
		names.append(String(method["name"]))
	for removed: String in ["_fill_single_month", "populate_month_calendar", "update_vacation_for_date"]:
		check(not names.has(removed), "rules/calendar.gd has no generated-calendar function " + removed)
	# Unloaded rows: the week builder refuses instead of generating markers.
	var state: State = _state_at(95, 5, 28)
	state.rule_state["calendar"][0]["day_kind"].fill(0)
	state.rule_state["last_command"] = 7
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	check(Scheduler.build_week(state, rules.synthetic_tables(), rng) == "week queue requires the loaded original CALROM month rows",
		"week builder refuses unloaded calendar rows")

func _test_day_notice() -> void:
	# [vacation, month, day] -> [vacation, notice]: schedule_vacation_notice_step.
	var cases: Array = [
		[0, 7, 24, 1, DayNotice.NOTICE_SUMMER], [0, 12, 25, 2, DayNotice.NOTICE_WINTER],
		[0, 3, 24, 3, DayNotice.NOTICE_SPRING], [1, 9, 1, 0, DayNotice.NOTICE_NEW_TERM],
		[1, 9, 2, 0, DayNotice.NOTICE_NEW_TERM], [2, 1, 8, 0, DayNotice.NOTICE_NEW_TERM],
		[3, 4, 4, 0, DayNotice.NOTICE_NEW_TERM], [0, 4, 4, 0, ""], [0, 9, 1, 0, ""],
	]
	for row: Array in cases:
		var data: Dictionary = {"vacation": row[0], "month": row[1], "day": row[2]}
		var out: Dictionary = DayNotice.vacation_notice(data)
		check(out["error"].is_empty() and data["vacation"] == row[3] and out["notice"] == row[4],
			"day notice %s -> vacation %d notice '%s'" % [str(row.slice(0, 3)), row[3], row[4]])
	var bad: Dictionary = {"vacation": 4, "month": 7, "day": 24}
	check(not DayNotice.vacation_notice(bad)["error"].is_empty() and bad["vacation"] == 4, "out-of-enum vacation is refused unchanged")
	check(not DayNotice.vacation_notice({"month": 7})["error"].is_empty(), "missing native fields are refused")
	# The native modes stay unbound: the controller attaches no handler and the
	# router refuses them by name instead of running an overlay preview.
	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller := Controller.new(rules.synthetic_tables())
	check(controller.attach_multi_week(session, rules.synthetic_state()).is_empty(), "controller attaches")
	for mode: int in [0x39, 0x41, 0x42]:
		check(session.router.missing_handler(mode) == "unsupported native mode 0x%02x: no implemented handler" % mode,
			"native 0x%02x has no handler and is refused" % mode)

func _test_mode_identity_tables() -> void:
	for native: int in [0x39, 0x41, 0x42]:
		check(not DayEventModes.NATIVE_TO_DISPATCHER_MODE.has(native), "native 0x%02x is not an overlay mode" % native)
		check(SeasonalModes.is_main_executable(native), "native 0x%02x is a main-executable handler" % native)
	for event: int in [0xfd, 0x28, 0x2c]:
		check(not DayEventModes.EVENT_TO_DISPATCHER_MODE.has(event), "queue 0x%02x has no overlay mapping" % event)
	check(DayEventModes.dispatcher_mode_for(0x57, 0x32) == 0x44 and DayEventModes.dispatcher_mode_for(0x56, 0x33) == 0x44, "birthdays load GEKO")
	check(DayEventModes.dispatcher_mode_for(0x5B) == 0x4E and DayEventModes.dispatcher_mode_for(0x55, 0x30) == 0x4E, "meeting and seasonal party load SHOUGATU")
	check(DayEventModes.dispatcher_mode_for(0x54, 0x31) == 0x4F and DayEventModes.dispatcher_mode_for(0x72, 0x20) == 0x47, "Valentine and sports festival keep VALEN/TAIIKU")
	var entries: Dictionary = {}
	for mode: int in [0x52, 0x53, 0x55, 0x5B]:
		var entry: Dictionary = SeasonalModes.overlay_entry(mode)
		check(entry.get("overlay") == "SHOUGATU" and entry.get("lba") == 0x988A, "native 0x%02x loads SHOUGATU" % mode)
		entries[entry.get("entry")] = mode
	check(entries.size() == 4, "the four SHOUGATU modes have four distinct entries")

func _test_calrom_markers() -> void:
	for row: Array in CALROM_MARKERS:
		var month: Dictionary = source.month(row[0], row[1])
		check(month["error"].is_empty() and month["marker"][row[2]] == row[3],
			"CALROM %02d-%02d-%02d marker 0x%02x" % [row[0], row[1], row[2], row[3]])
	for row: Array in INVENTED_DATES:
		check(source.month(row[0], row[1])["marker"][row[2]] == 0, "CALROM %s has no invented marker" % str(row))

func _test_rollover() -> void:
	# Mid-month advance needs no row load; vacation is never written.
	var state: State = _state_at(95, 7, 25)
	state.rule_state["vacation"] = 1
	check(Calendar.advance_day(state).is_empty() and state.rule_state["day"] == 26 and state.rule_state["vacation"] == 1,
		"mid-month advance keeps vacation")
	# Rollover without the source is refused before any write.
	state = _state_at(95, 12, 31)
	var before: Dictionary = state.rule_state.duplicate(true)
	check(Calendar.advance_day(state) == "calendar month rollover requires the validated original CALROM source"
		and state.rule_state == before, "no-source rollover is refused with no writes")
	# With CALROM rows: year rollover keeps both rows for the FE load and keeps vacation.
	state = _state_at(95, 12, 31)
	check(CalromMonths.install(state.rule_state, source, 95, 12).is_empty(), "95-12/96-01 CALROM rows install")
	state.rule_state["vacation"] = 2
	var rows: Array = state.rule_state["calendar"].duplicate(true)
	check(Calendar.advance_day(state).is_empty() and [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]] == [96, 1, 1]
		and state.rule_state["calendar"] == rows and state.rule_state["vacation"] == 2,
		"95-12-31 -> 96-01-01 keeps rows and vacation")
	# Three-year span: each month's first weekday follows the calendar arithmetic.
	for index: int in range(36):
		var absolute: int = 95 * 12 + 3 + index
		var year: int = absolute / 12
		var month: int = absolute % 12 + 1
		check(source.month(year, month)["marker"][0] == Calendar.weekday_for_date(year, month, 1)
			and source.month(year, month)["day_kind"][0] == Calendar.days_in_month(year, month),
			"%02d-%02d CALROM count/first weekday match the date arithmetic" % [year, month])

func _queue_types(state: State) -> Array[int]:
	var types: Array[int] = []
	for event: Array in state.rule_state["day_events"]:
		types.append(int(event[0]))
	return types

func _test_week_queue() -> void:
	var tables: Dictionary = rules.synthetic_tables()
	var rng := Rng.new()
	# Week of 95-05-29: crosses the FE month start into June; 95-06-03 is TAIIKU.
	var state: State = _state_at(95, 5, 28)
	check(CalromMonths.install(state.rule_state, source, 95, 5).is_empty(), "95-05/06 CALROM rows install")
	state.rule_state["last_command"] = 7
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var error: String = Scheduler.build_week(state, tables, rng)
	var types: Array[int] = _queue_types(state)
	check(error.is_empty() and types.has(0xfe) and types.has(0x20) and types.find(0xfe) < types.find(0x20),
		"week of 95-05-29 queues FE then the sports festival (0x20) on 95-06-03: " + error)
	# Week of 95-04-17: the invented 95-04-20 seasonal party is gone.
	state = _state_at(95, 4, 16)
	check(CalromMonths.install(state.rule_state, source, 95, 4).is_empty(), "95-04/05 CALROM rows install")
	state.rule_state["last_command"] = 7
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	error = Scheduler.build_week(state, tables, rng)
	types = _queue_types(state)
	check(error.is_empty() and not types.has(0x30) and not types.has(0x20), "week of 95-04-17 has no seasonal event: " + error)
	# Week of 95-07-24: the summer notice (marker 6) queues 0xFD.
	state = _state_at(95, 7, 23)
	check(CalromMonths.install(state.rule_state, source, 95, 7).is_empty(), "95-07/08 CALROM rows install")
	state.rule_state["last_command"] = 7
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	error = Scheduler.build_week(state, tables, rng)
	check(error.is_empty() and _queue_types(state).has(0xfd), "week of 95-07-24 queues the 0xFD summer notice: " + error)

func _empty(_state: Variant, _rng: Rng, _result: Session.FrameResult) -> void:
	pass

## A multi-week controller session on `state`, stepping like the installed route.
func _session_for(state: State) -> Session:
	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var controller := Controller.new(rules.synthetic_tables())
	controllers.append(controller)
	check(controller.attach_multi_week(session, state).is_empty(), "controller attaches for the route step")
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty
	return session

func _step(session: Session, pad: int) -> Session.FrameResult:
	session.latched_input = InputFrame.new(session.clock.logic_iterations - 1, pad, 0)
	return session.step(InputFrame.new(session.clock.logic_iterations, 0), [])

## The 0xFD record built from CALROM for the week of 95-07-24 runs through
## OrdinaryWeek.dispatch into native 0x39, which the router then refuses by
## name without touching saved state.
func _test_queued_day_notice_refusal() -> void:
	var state: State = _state_at(95, 7, 23)
	check(CalromMonths.install(state.rule_state, source, 95, 7).is_empty(), "95-07/08 CALROM rows install for the 0xFD route step")
	state.rule_state["last_command"] = 7
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	check(Scheduler.build_week(state, rules.synthetic_tables(), rng).is_empty(), "week of 95-07-24 builds")
	var index: int = _queue_types(state).find(0xfd)
	check(index >= 0, "0xFD is queued for the week: %s" % str(_queue_types(state).slice(0, 12)))
	if index < 0:
		return
	var data: Dictionary = state.rule_state
	data["year"] = 95
	data["month"] = 7
	data["day"] = 24
	data["weekday"] = Calendar.weekday_for_date(95, 7, 24)
	data["day_event_index"] = index
	state.reset_game_mode_fields(0x31)
	var session: Session = _session_for(state)
	var res: Session.FrameResult = _step(session, 0)
	var ovl: Variant = data.get("overlay_state")
	check(res.error.is_empty() and state.game_mode == 0x39 and (not ovl is Dictionary or (ovl as Dictionary).is_empty()),
		"queued 0xFD dispatches to native 0x39 with no overlay (%s)" % res.error)
	var before: Dictionary = data.duplicate(true)
	var core: Array = [state.game_mode, state.mode_stage, state.mode_step]
	res = _step(session, 0)
	check(res.error == "unsupported native mode 0x39: no implemented handler", "native 0x39 is refused by name (%s)" % res.error)
	check(data == before and [state.game_mode, state.mode_stage, state.mode_step] == core,
		"native 0x39 refusal leaves rule_state and mode/stage/step unchanged")

## day_event_dispatch checks the graduation date once at entry
## (scheduler.c:1646-1649). The day-end on 98-02-28 advances to 98-03-01 and
## dispatches the next queued event; the following pass reaches step 1.
func _test_graduation_rollover() -> void:
	var state: State = _state_at(98, 2, 28)
	var data: Dictionary = state.rule_state
	check(CalromMonths.install(data, source, 98, 2).is_empty(), "98-02/03 CALROM rows install")
	var queue: Array = []
	for i: int in range(32):
		queue.append([0xff, 0, 0, 0])
	queue[0] = [0x00, 0, 0, 0]
	queue[1] = [0xfb, 0, 0, 0]
	queue[2] = [0x00, 1, 0, 0]
	data["day_events"] = queue
	data["day_event_index"] = 1
	state.reset_game_mode_fields(0x31)
	var result := Session.FrameResult.new()
	OrdinaryWeek.dispatch(state, result)
	check(result.error.is_empty() and [data["year"], data["month"], data["day"]] == [98, 3, 1],
		"98-02-28 day-end advances to 98-03-01 (%s)" % result.error)
	check(state.mode_step == 0 and state.game_mode == 0x40 and data["day_event_index"] == 2 and data["icon_enabled"][1] == 1,
		"98-03-01's queued event (0x00 command 1) is dispatched before the graduation gate")
	state.reset_game_mode_fields(0x31)
	result = Session.FrameResult.new()
	OrdinaryWeek.dispatch(state, result)
	check(result.error.is_empty() and state.game_mode == 0x31 and state.mode_step == 1,
		"the next day-screen pass on 98-03-01 reaches step 1 (%s)" % result.error)
	# The controller's step 1 is schedule_ending_start, refused by name.
	var session: Session = _session_for(state)
	var res: Session.FrameResult = _step(session, 0)
	check(res.error.begins_with("schedule_ending_start:"), "day-screen step 1 refuses schedule_ending_start (%s)" % res.error)

## SHUGAKU/RENSYU completion advances several days; a month end without the
## validated source is refused before the first write.
func _test_multi_day_rollover() -> void:
	var data: Dictionary = _state_at(95, 9, 26).rule_state
	check(Calendar.advance_days_source_error(data, 4).is_empty(), "95-09-26 +4 days stays in month")
	check(Calendar.advance_days_source_error(data, 6) == "calendar month rollover requires the validated original CALROM source",
		"95-09-26 +6 days without CALROM is refused")
	check(CalromMonths.install(data, source, 95, 9).is_empty(), "95-09/10 CALROM rows install")
	check(Calendar.advance_days_source_error(data, 6).is_empty(), "95-09-26 +6 days with CALROM is allowed")
	check(Controller._completion_calendar_days(0x4C) == 6 and Controller._completion_calendar_days(0x51) == 1
		and Controller._completion_calendar_days(0x4E) == 0, "SHUGAKU 6 days, RENSYU 1 day, others none")

## Queue 0x48 (meeting) enters native 0x5B, which loads SHOUGATU
## (overlay_loader.c, entry D_80137144). SHOUGATU has no production handler,
## so the saved-state frame is refused; ETC never runs.
func _test_meeting_route() -> void:
	check(DayEventModes.dispatcher_mode_for(0x5B, 0x48) == 0x4E, "dispatcher_mode_for(0x5B, 0x48) is SHOUGATU")
	var state: State = _state_at(95, 6, 5)
	var data: Dictionary = state.rule_state
	var queue: Array = []
	for i: int in range(32):
		queue.append([0xff, 0, 0, 0])
	queue[0] = [0x00, 0, 0, 0]
	queue[1] = [0x48, 0, 0, 0]
	data["day_events"] = queue
	data["day_event_index"] = 1
	state.reset_game_mode_fields(0x31)
	var session: Session = _session_for(state)
	var res: Session.FrameResult = _step(session, 0)
	var ovl: Dictionary = data.get("overlay_state", {})
	check(res.error.is_empty() and state.game_mode == 0x5B and int(ovl.get("dispatcher_mode", -1)) == 0x4E,
		"queue 0x48 enters native 0x5B with the SHOUGATU overlay (%s, dispatcher 0x%02x)" % [res.error, int(ovl.get("dispatcher_mode", -1))])
	var before: Dictionary = data.duplicate(true)
	res = _step(session, 0x20)
	check(res.error.contains("refused with saved state") and state.game_mode == 0x5B and data == before,
		"meeting (SHOUGATU) is refused in production with saved state unchanged (%s)" % res.error)

## Compare the identity tables with the source switch statements.
func _test_source_switches() -> void:
	var root: String = OS.get_environment("TOKIMEMO_SOURCE_ROOT")
	var enums: String = FileAccess.get_file_as_string(root.path_join("include/game_enums.h"))
	var loader: String = FileAccess.get_file_as_string(root.path_join("src/main/overlay_loader.c"))
	var scheduler: String = FileAccess.get_file_as_string(root.path_join("src/main/scheduler.c"))
	check(not enums.is_empty() and not loader.is_empty() and not scheduler.is_empty(), "TOKIMEMO_SOURCE_ROOT provides the source files")
	if enums.is_empty() or loader.is_empty() or scheduler.is_empty():
		return
	var values: Dictionary = {}
	var enum_re := RegEx.create_from_string("(GAME_MODE_[A-Z0-9_]+) = (0x[0-9A-Fa-f]+)")
	for found: RegExMatch in enum_re.search_all(enums):
		values[found.get_string(1)] = found.get_string(2).hex_to_int()
	var enter_at: int = loader.find("void ovl_load_enter(")
	var lbas: Dictionary = _case_values(loader.substr(0, enter_at), "ovl_load_lba = (0x[0-9A-Fa-f]+);", values)
	var entries: Dictionary = _case_values(loader.substr(enter_at), "ovl_entry = \\(u32\\) &D_([0-9A-Fa-f]{8});", values)
	for mode: int in SeasonalModes.OVERLAY_ENTRIES:
		var expected: Dictionary = SeasonalModes.OVERLAY_ENTRIES[mode]
		check(lbas.get(mode, -1) == expected["lba"], "native 0x%02x LBA matches ovl_load_start" % mode)
		check(entries.get(mode, -1) == expected["entry"], "native 0x%02x entry matches ovl_load_enter" % mode)
	for mode: int in SeasonalModes.MAIN_EXECUTABLE_HANDLERS:
		check(not entries.has(mode) and not lbas.has(mode), "native 0x%02x has no overlay case" % mode)
	var dispatch_at: int = scheduler.find("op = game_state.day_events[game_state.day_event_index].type;")
	var dispatch_end: int = scheduler.find("void schedule_quarter_flags_rotate(")
	var modes: Dictionary = _case_values(scheduler.substr(dispatch_at, dispatch_end - dispatch_at), "game_mode_set\\((GAME_MODE_[A-Z0-9_]+)\\)", values)
	check(dispatch_at > 0 and dispatch_end > dispatch_at, "day_event_dispatch switch located")
	for event: int in SeasonalModes.EVENT_NATIVE_MODE:
		check(modes.get(event, -1) == SeasonalModes.EVENT_NATIVE_MODE[event], "queue 0x%02x -> native 0x%02x per day_event_dispatch" % [event, SeasonalModes.EVENT_NATIVE_MODE[event]])
		check(DayEventModes.mode_for(event, 0) == SeasonalModes.EVENT_NATIVE_MODE[event], "EVENT_MODES agrees for queue 0x%02x" % event)

## Walk "case X:" labels; the first value line after them binds every pending
## label, and "break;"/"return;" clears them. Values may be GAME_MODE names.
func _case_values(text: String, value_pattern: String, names: Dictionary) -> Dictionary:
	var out: Dictionary = {}
	var pending: Array[int] = []
	var case_re := RegEx.create_from_string("^\\s*case ([A-Z0-9_x]+):")
	var value_re := RegEx.create_from_string(value_pattern)
	for line: String in text.split("\n"):
		var c: RegExMatch = case_re.search(line)
		if c != null:
			var label: String = c.get_string(1)
			pending.append(label.hex_to_int() if label.begins_with("0x") else (int(names[label]) if names.has(label) else label.to_int()))
			continue
		var v: RegExMatch = value_re.search(line)
		if v != null and not pending.is_empty():
			var raw: String = v.get_string(1)
			var value: int = int(names[raw]) if names.has(raw) else raw.hex_to_int()
			for label: int in pending:
				if not out.has(label):
					out[label] = value
			pending.clear()
		elif line.strip_edges() in ["break;", "return;"]:
			pending.clear()
	return out
