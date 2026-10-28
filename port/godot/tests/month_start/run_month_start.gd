extends SceneTree
## T-29028. GameSession integration using original numeric month prefixes in
## invented structural states. No played-route, GPU, audio, or emulator claim.

const Controller = preload("res://scenes/school_day_controller.gd")
const MonthStart = preload("res://scenes/month_start_controller.gd")
const Source = preload("res://services/native_calendar_source.gd")
const Fixtures = preload("res://tests/rules/run_rule_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Core = preload("res://core/game_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")

var checks: int = 0
var failures: Array[String] = []
var reports: Array[Dictionary] = []
var source: Source = Source.new()
var source_path: String

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures.append(label)

func _initialize() -> void:
	source_path = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	var error: String = source.open_source(source_path)
	check(error.is_empty() and source.identity() == Source.SOURCE_SHA, "exact original CALROM source opens: " + error)
	if not error.is_empty():
		_finish()
		return
	_test_lifecycle()
	_test_phase_and_source_rejections()
	_test_completion_rejections()
	_test_calendar_integrity()
	_test_transaction_retry()
	_finish()

func _record(year: int, month: int, tail: int) -> Dictionary:
	var original: Dictionary = source.month(year, month)
	var record: Dictionary = {}
	for key: String in ["day_kind", "marker"]:
		var row: Array = []
		row.resize(32)
		row.fill(tail)
		for index: int in range(original[key].size()):
			row[index] = original[key][index]
		record[key] = row
	return record

func _state(year: int = 95, month: int = 5) -> State:
	var state: State = Fixtures.new().synthetic_state()
	state.game_mode = 0x38
	state.mode_stage = 0
	state.mode_step = 0
	state.substep = 0
	state.frame_count = 117
	var data: Dictionary = state.rule_state
	data["year"] = year
	data["month"] = month
	data["day"] = 1
	data["weekday"] = source.month(year, month)["marker"][0]
	data["day_event_index"] = 7
	data["day_events"][7] = [0xfe, 0, 0, 0]
	data["day_events"][8] = [0, 0, 0, 15]
	data["resources"][MonthStart.SOURCE_KEY] = Source.SOURCE_SHA
	data["calendar"] = [_record(year - (1 if month == 1 else 0), 12 if month == 1 else month - 1, 173), _record(year, month, 219)]
	return state

func _harness(state: State = null, open_original: bool = true) -> Dictionary:
	if state == null:
		state = _state()
	var controller := Controller.new(Fixtures.new().synthetic_tables())
	check(controller.error.is_empty(), "actual school-day controller accepts invented structural rule tables")
	if open_original:
		check(controller.month_start.open_source(source_path).is_empty(), "actual attached month-start owner preflights original source")
	var session := Session.new()
	check(session.rng.configure(Rng.OPENBIOS_PROFILE, 19950404), "test session configures deterministic native RNG")
	check(controller.attach(session, state).is_empty(), "actual controller attaches month0x38 and completion receiver")
	var draw_settings: Dictionary = {"failure": false}
	var harness: Dictionary = {"controller": controller, "session": session, "state": state, "draw_settings": draw_settings}
	session.draw_logic = func(base: Core, rng: Rng, result: Session.FrameResult) -> void:
		var staged: Dictionary = controller.month_start.stage_services(base as State, result)
		result.error = staged["error"]
		if draw_settings["failure"] and result.error.is_empty():
			(base as State).rule_state["params"][0] ^= 1
			rng.next("T-29028:test-rollback-after-upload")
			result.error = "injected downstream draw failure"
	session.cd_logic = _empty
	session.audio_logic = _empty
	return harness

func _empty(_state: Core, _rng: Rng, _result: Session.FrameResult) -> void:
	pass

func _step(harness: Dictionary, events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	var session: Session = harness["session"]
	return session.step(InputFrame.new(session.clock.logic_iterations), events)

func _payload(harness: Dictionary, ready: bool = true) -> Dictionary:
	var payload: Dictionary = harness["state"].rule_state["resources"][MonthStart.PENDING_KEY].duplicate(true)
	payload["ready"] = ready
	return payload

func _event(harness: Dictionary, payload: Dictionary, phase: Session.Phase = Session.Phase.BEFORE_MODE, service: String = MonthStart.SERVICE) -> Session.ServiceEvent:
	return Session.ServiceEvent.new(harness["session"].clock.logic_iterations, phase, Session.ServiceKind.COMPLETION, service, payload)

func _checkpoint(harness: Dictionary) -> Dictionary:
	var session: Session = harness["session"]
	return {"state": session.state.projection(), "rng": session.rng.checkpoint(), "clock": session.clock.snapshot(),
		"input": session.latched_input.snapshot(), "hash": session.state_hash()}

func _reject(harness: Dictionary, label: String, events: Array[Session.ServiceEvent] = []) -> Session.FrameResult:
	var before: Dictionary = _checkpoint(harness)
	var result: Session.FrameResult = _step(harness, events)
	check(not result.committed and not result.error.is_empty(), label + ": refuses transaction")
	check(_checkpoint(harness) == before, label + ": restores calendar/queue/rules/RNG/clocks/input/hash")
	check(result.presentation.is_empty() and result.audio.is_empty() and result.transitions.is_empty()
		and result.event_hash.is_empty() and result.service_events_consumed == 0,
		label + ": publishes no aborted effects or consumed service count")
	return result

func _request(harness: Dictionary) -> Session.FrameResult:
	var before_calendar: Array = harness["state"].rule_state["calendar"].duplicate(true)
	var result: Session.FrameResult = _step(harness)
	check(result.committed and result.error.is_empty(), "native substep0 issues calendar read")
	check(harness["state"].substep == 1 and harness["state"].rule_state["calendar"] == before_calendar,
		"read request enters wait without shifting calendar")
	return result

func _test_lifecycle() -> void:
	var reference: Dictionary = {}
	for date: Array in [[95, 5], [95, 8], [96, 1], [96, 2]]:
		for renders: int in [0, 1, 3]:
			var harness: Dictionary = _harness(_state(date[0], date[1]))
			var state: State = harness["state"]
			var session: Session = harness["session"]
			var old_calendar: Array = state.rule_state["calendar"].duplicate(true)
			var rng_before: Dictionary = session.rng.checkpoint()
			var hashes: Array = []
			session.clock.rendered_frames += renders
			var requested: Session.FrameResult = _request(harness)
			hashes.append([requested.state_hash, requested.event_hash])
			var request: Dictionary = {"request": "calendar", "year": date[0], "month": date[1],
				"lba": 0x9b94, "sectors": 0x95, "address": 0x801a0000, "source_sha256": Source.SOURCE_SHA}
			check(requested.presentation == [{"kind": "month_calendar_load", "request": request}], "read request carries exact native extent/address/source")
			var staged: Dictionary = harness["controller"].month_start.stage_services(state, requested)
			check(staged["error"].is_empty() and staged["services"] == [{"service": MonthStart.SERVICE, "payload": _payload(harness)}],
				"service staging truthfully completes the preflighted original read")
			session.clock.rendered_frames += renders
			var waiting: Session.FrameResult = _step(harness, [_event(harness, _payload(harness, false))])
			hashes.append([waiting.state_hash, waiting.event_hash])
			check(waiting.committed and state.mode_stage == 0 and state.substep == 1 and state.rule_state["calendar"] == old_calendar
				and not state.rule_state["resources"][MonthStart.PENDING_KEY]["ready"], "false readiness commits a genuine read wait without calendar upload")
			session.clock.rendered_frames += renders
			var uploaded: Session.FrameResult = _step(harness, [_event(harness, _payload(harness))])
			hashes.append([uploaded.state_hash, uploaded.event_hash])
			check(uploaded.committed and [state.game_mode, state.mode_stage, state.mode_step, state.substep] == [0x38, 1, 0, 0],
				"true readiness uploads and enters stage1 without queue advance")
			check(state.rule_state["day_event_index"] == 7 and state.rule_state["calendar"][0] == old_calendar[1],
				"promotion copies the entire old next record exactly once")
			var following: Dictionary = source.month(date[0], date[1] + 1)
			for key: String in ["day_kind", "marker"]:
				var prefix: int = following[key].size()
				check(state.rule_state["calendar"][1][key].slice(0, prefix) == following[key], "next " + key + " uploads exactly count+1 original values")
				check(state.rule_state["calendar"][1][key].slice(prefix) == old_calendar[1][key].slice(prefix),
					"next " + key + " preserves every unwritten destination tail byte")
			check(uploaded.audio.is_empty() and uploaded.presentation == [{"kind": "month_calendar_uploaded", "year": date[0], "month": date[1], "source_sha256": Source.SOURCE_SHA}],
				"non-quarter upload emits source identity and no invented audio")
			session.clock.rendered_frames += renders
			var exited: Session.FrameResult = _step(harness)
			hashes.append([exited.state_hash, exited.event_hash])
			check(exited.committed and state.game_mode == 0x31 and state.rule_state["day_event_index"] == 8
				and not state.rule_state["resources"].has(MonthStart.PENDING_KEY), "stage1 advances one queue record and consumes pending upload")
			check(session.rng.checkpoint() == rng_before, "non-quarter load/wait/upload/exit consume no RNG")
			var key: String = str(date)
			if renders == 0:
				reference[key] = hashes
				reports.append({"date": date, "hashes": hashes, "calendar": state.rule_state["calendar"].duplicate(true)})
			else:
				check(hashes == reference[key], "month-start state/event hashes are independent of render schedule%d" % renders)

func _test_phase_and_source_rejections() -> void:
	var state: State = _state()
	state.rule_state["resources"].erase(MonthStart.SOURCE_KEY)
	_reject(_harness(state), "missing source metadata")
	state = _state()
	state.rule_state["resources"][MonthStart.SOURCE_KEY] = "incorrect source"
	_reject(_harness(state), "mismatched source metadata")
	_reject(_harness(_state(), false), "unopened immutable source")
	for quarter: int in [6, 9, 12]:
		var quarter_harness: Dictionary = _harness(_state(95, quarter))
		_request(quarter_harness)
		_reject(quarter_harness, "quarter%d upload explicitly needs native BGM/text services" % quarter, [_event(quarter_harness, _payload(quarter_harness))])
	for field: String in ["day", "queue", "stage", "step", "substep", "actor"]:
		state = _state()
		match field:
			"day": state.rule_state["day"] = 2
			"queue": state.rule_state["day_events"][7][0] = 0xfb
			"stage": state.mode_stage = 2
			"step": state.mode_step = 1
			"substep": state.substep = 2
			"actor": state.rule_state["actor_state"] = {"96": {"id": 96, "flags": 5, "sequence_slot": 0, "sequence": {"counter": 0}}}
		_reject(_harness(state), "forged month phase: " + field)
	state = _state()
	state.mode_stage = 1
	_reject(_harness(state), "forged exit cannot advance without actual completed upload")
	state = _state()
	state.rule_state["year"] = 99
	_reject(_harness(state), "unavailable source date")

func _test_completion_rejections() -> void:
	for field: String in ["request", "year", "month", "lba", "sectors", "address", "source_sha256", "ready", "missing", "extra"]:
		var harness: Dictionary = _harness()
		_request(harness)
		var payload: Dictionary = _payload(harness)
		match field:
			"request", "source_sha256": payload[field] = "forged"
			"year", "month", "lba", "sectors", "address": payload[field] += 1
			"ready": payload[field] = 1
			"missing": payload.erase("address")
			"extra": payload["unexpected"] = true
		_reject(harness, "malformed completion " + field, [_event(harness, payload)])
	var harness: Dictionary = _harness()
	_request(harness)
	var good: Dictionary = _payload(harness)
	_reject(harness, "duplicate true readiness is atomic", [_event(harness, good), _event(harness, good)])
	var repaired: Session.FrameResult = _step(harness, [_event(harness, good)])
	check(repaired.committed and harness["state"].mode_stage == 1, "valid completion can retry after duplicate-event rollback")
	_reject(harness, "completion at completed stage1 is rejected", [_event(harness, good)])
	harness = _harness()
	_request(harness)
	var delayed: Session.FrameResult = _step(harness, [_event(harness, _payload(harness), Session.Phase.CD)])
	check(delayed.committed and harness["state"].mode_stage == 0 and harness["state"].substep == 1,
		"post-mode completion does not retroactively upload in the same frame")
	_reject(harness, "second readiness before a later upload is rejected", [_event(harness, _payload(harness))])
	check(_step(harness).committed and harness["state"].mode_stage == 1, "ready payload delivered after mode uploads on the following actual mode step")
	harness = _harness()
	var premature: Dictionary = {"request": "calendar", "year": 95, "month": 5, "lba": 0x9b94,
		"sectors": 0x95, "address": 0x801a0000, "source_sha256": Source.SOURCE_SHA, "ready": true}
	_reject(harness, "completion before a real request is refused", [_event(harness, premature)])

func _test_calendar_integrity() -> void:
	for fault: String in ["current_day", "current_marker", "date", "pending", "double_shift"]:
		var harness: Dictionary = _harness()
		_request(harness)
		var state: State = harness["state"]
		match fault:
			"current_day": state.rule_state["calendar"][1]["day_kind"][1] ^= 1
			"current_marker": state.rule_state["calendar"][1]["marker"][1] ^= 1
			"date": state.rule_state["month"] = 7
			"pending": state.rule_state["resources"][MonthStart.PENDING_KEY]["year"] = 96
			"double_shift":
				state.rule_state["calendar"] = [_record(95, 5, 219), _record(95, 6, 219)]
		_reject(harness, "calendar/source integrity " + fault, [_event(harness, _payload(harness))])
	for fault: String in ["old_day", "old_marker"]:
		var prior_harness: Dictionary = _harness()
		_request(prior_harness)
		var prior_state: State = prior_harness["state"]
		var promoted: Dictionary = prior_state.rule_state["calendar"][1].duplicate(true)
		if fault == "old_marker":
			# Source final_exam.c::exam_results_react marks makeup lessons.
			prior_state.rule_state["calendar"][0]["marker"][1] = 0x17
		else:
			prior_state.rule_state["calendar"][0]["day_kind"][1] ^= 1
		var result: Session.FrameResult = _step(prior_harness, [_event(prior_harness, _payload(prior_harness))])
		check(result.committed and prior_state.rule_state["calendar"][0] == promoted,
			"native memcpy discards prior record runtime changes: " + fault)
	var harness: Dictionary = _harness()
	_request(harness)
	check(_step(harness, [_event(harness, _payload(harness))]).committed, "exit-integrity fixture genuinely uploads")
	harness["state"].rule_state["calendar"][0]["marker"][1] ^= 1
	_reject(harness, "forged completed-calendar exit refuses queue advance")

func _test_transaction_retry() -> void:
	var control: Dictionary = _harness()
	var retry: Dictionary = _harness()
	_request(control)
	_request(retry)
	var payload: Dictionary = _payload(retry)
	retry["draw_settings"]["failure"] = true
	_reject(retry, "downstream failure after real upload", [_event(retry, payload)])
	retry["draw_settings"]["failure"] = false
	var retried: Session.FrameResult = _step(retry, [_event(retry, payload)])
	var clean: Session.FrameResult = _step(control, [_event(control, _payload(control))])
	check(retried.committed and [retried.state_hash, retried.event_hash] == [clean.state_hash, clean.event_hash],
		"retry after calendar mutation/RNG draw failure equals untouched control upload")
	check(_checkpoint(retry) == _checkpoint(control), "retry restores all authoritative state before producing the same final state")
	var bogus := Session.FrameResult.new()
	bogus.presentation.append({"kind": "month_calendar_load", "request": {"request": "forged"}})
	var before: Dictionary = _checkpoint(retry)
	var staged: Dictionary = retry["controller"].month_start.stage_services(retry["state"], bogus)
	check(not staged["error"].is_empty() and _checkpoint(retry) == before, "forged resource staging is read-only and rejects without scheduling a completion")

func _finish() -> void:
	var report: Dictionary = {"scope": "controller/session integration in invented structural states using exact original numeric source; no played-route, GPU, audio or emulator acceptance",
		"checks": checks, "failures": failures, "routes": reports}
	var output: String = OS.get_environment("TOKIMEMO_CALENDAR_OUTPUT")
	if output.is_empty():
		output = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	if not output.is_empty():
		var file: FileAccess = FileAccess.open(output.path_join("month-start.json"), FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(report, "\t"))
			file.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "source_backed_structural_cases": reports.size()}))
	quit(0 if failures.is_empty() else 1)
