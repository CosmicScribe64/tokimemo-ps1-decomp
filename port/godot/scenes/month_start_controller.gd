extends RefCounted
## T-29028. scheduler.c::schedule_month_start_mode_handler/data_load_step.
## State-free controller: original source is immutable; pending work is primitive
## rule state and is rolled back by GameSession with calendar/queue/RNG/clocks.

const State = preload("res://rules/school_day_state.gd")
const Core = preload("res://core/game_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Source = preload("res://services/native_calendar_source.gd")
const AudioBridge = preload("res://services/month_start_audio_bridge.gd")
const BankRequest = preload("res://services/month_start_bank_request.gd")
const SOURCE_KEY: String = "native_calendar_source"
const PENDING_KEY: String = "month_calendar_pending"
const SERVICE: String = "month_start.resources"
const LBA: int = 0x9b94
const SECTORS: int = 0x95
const ADDRESS: int = 0x801a0000

var _source: Source = Source.new()
var audio: AudioBridge = AudioBridge.new()
var _notice_owner: Variant = null
var _notice_generation: int = 0
var _notice_published: Dictionary = {}

static func notice_owner_error(value: Variant) -> String:
	if not value is Object or not is_instance_valid(value) or not value is RefCounted:
		return "native month notice requires a live source text owner"
	for method: String in ["month_start_notice", "month_start_notice_validate", "month_start_notice_publish"]:
		if not value.has_method(method):
			return "native month notice source interface is unavailable"
	return ""

func bind_notice_owner(value: Variant) -> String:
	var error: String = notice_owner_error(value)
	if not error.is_empty():
		return error
	_notice_owner = value
	_notice_generation += 1
	_notice_published.clear()
	return ""

func reset_services() -> void:
	audio.reset()
	_notice_owner = null
	_notice_generation += 1
	_notice_published.clear()

func prepare_services(state: State) -> String:
	if state == null:
		return "month services require native state"
	var failure: String = state.structural_error()
	if not failure.is_empty():
		return failure
	if state.rule_state["vacation"] < 0 or state.rule_state["vacation"] > 3:
		return "month services require an original vacation selector"
	if state.game_mode == 0x38 and state.mode_stage == 0 and state.mode_step in [0, 1] \
		and (state.mode_step == 1 or state.substep == 1) \
		and branch_for(state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]) in ["quarter_music", "september_notice"]:
		return audio.prepare(state, bank_request(state.rule_state))
	return ""

func discard_services(frame_count: int) -> String:
	return audio.discard(frame_count)

func open_source(path: String) -> String:
	return _source.open_source(path)

func identity() -> String:
	return _source.identity()

func step(base: Core, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = base as State
	result.error = _phase_error(state)
	if not result.error.is_empty():
		return
	var data: Dictionary = state.rule_state
	if state.mode_stage == 1:
		var completed: Variant = data["resources"].get(PENDING_KEY)
		if state.substep != 0 or not completed is Dictionary or not _matches_pending(state, completed) or not completed["ready"] or not _matches_loaded_calendar(state):
			result.error = "month-start exit lacks its completed original calendar upload"
			return
		if branch_for(data["year"], data["month"], data["day"]) in ["quarter_music", "september_notice"]:
			var bank_pending: Variant = data["resources"].get(AudioBridge.PENDING)
			if not AudioBridge._pending_shape(bank_pending) or bank_pending["request"] != bank_request(data):
				result.error = "month-start exit lost its source bank identity"
				return
			result.error = audio.active(state)
			if not result.error.is_empty():
				return
		data["resources"].erase(PENDING_KEY)
		data["resources"].erase(AudioBridge.PENDING)
		data["resources"].erase(AudioBridge.EMISSION)
		data["resources"].erase("month_notice_pending")
		data["day_event_index"] = Math.u8(data["day_event_index"] + 1)
		state.reset_game_mode_fields(0x31)
		result.transitions.append({"kind": "native_mode", "mode": 0x31, "day_event_index": data["day_event_index"]})
		return
	if state.mode_step == 1:
		if branch_for(data["year"], data["month"], data["day"]) not in ["quarter_music", "september_notice"] or not _matches_loaded_calendar(state):
			result.error = "month audio wait lacks its exact quarter upload"
			return
		var calendar_pending: Variant = data["resources"].get(PENDING_KEY)
		if not calendar_pending is Dictionary or not _matches_pending(state, calendar_pending) or not calendar_pending["ready"]:
			result.error = "month audio wait lost its completed CALROM request"
			return
		var pending: Variant = data["resources"].get(AudioBridge.PENDING)
		if not AudioBridge._pending_shape(pending) or pending["request"] != bank_request(data):
			result.error = "month audio wait lacks its actual source bank request"
			return
		var polled: Dictionary = audio.operation(state, bank_request(data), "poll", result)
		result.error = polled["error"]
		if not result.error.is_empty() or polled["status"] != 1:
			return
		var started: Dictionary = audio.operation(state, bank_request(data), "start", result)
		result.error = started["error"]
		if result.error.is_empty():
			state.next_stage()
		return
	if state.substep == 0:
		var request: Dictionary = _request(state)
		data["resources"][PENDING_KEY] = request.duplicate(true)
		data["resources"][PENDING_KEY]["ready"] = false
		result.presentation.append({"kind": "month_calendar_load", "request": request})
		state.substep = 1
		return
	var pending: Variant = data["resources"].get(PENDING_KEY)
	if not pending is Dictionary or not _matches_pending(state, pending):
		result.error = "month-start wait lacks the exact native resource request"
		return
	if not pending["ready"]:
		return
	result.error = _rotate_calendar(state)
	if not result.error.is_empty():
		return
	result.presentation.append({"kind": "month_calendar_uploaded", "year": data["year"], "month": data["month"], "source_sha256": identity()})
	var branch: String = branch_for(data["year"], data["month"], data["day"])
	if branch == "ordinary":
		state.next_stage()
	elif branch == "september96_skip":
		_quarter_effects(state, result)
		state.next_stage()
	else:
		if branch == "september_notice":
			result.error = _start_notice(state, result)
			if not result.error.is_empty():
				return
		var loaded: Dictionary = audio.operation(state, bank_request(data), "load", result)
		result.error = loaded["error"]
		if not result.error.is_empty():
			return
		state.next_step()
		_quarter_effects(state, result)

static func bank_request(data: Dictionary) -> Dictionary:
	return BankRequest.build(data)

func _start_notice(state: State, result: Session.FrameResult) -> String:
	if not is_instance_valid(_notice_owner):
		return "native September notice source/text owner is unavailable"
	var payload: Dictionary = {"selector": "schedule_month_data_load_step/september_notice",
		"date": [state.rule_state["year"], 9, 1], "frame_count": state.frame_count, "state": state.projection(),
		"calls": ["text_system_reset", "message_window_set_title_hidden", "message_window_title_show",
			"gnsx", "sndisp", "k_disp_start"], "title_show": 0, "text_attribute": 0x1f, "display_mode": 3}
	var reply: Variant = _notice_owner.call("month_start_notice", payload.duplicate(true))
	if not is_instance_valid(_notice_owner) or not reply is Dictionary or not AudioBridge._schema(reply, "tokimemo.month-notice.v1") \
		or not reply.get("error") is String or not AudioBridge._digest_shape(reply.get("request_sha256")) or reply["request_sha256"] != AudioBridge.digest(payload):
		return "native notice did not return its exact source receipt"
	if not reply["error"].is_empty():
		return reply["error"]
	if not reply.get("token") is String or reply["token"].is_empty() or reply["token"].length() > 128 \
		or not AudioBridge._digest_shape(reply.get("source_sha256")):
		return "native notice source/token identity is malformed"
	var intent: Dictionary = {"kind": "month_start_notice", "payload": payload,
		"token": reply["token"], "source_sha256": reply["source_sha256"], "notice_generation": _notice_generation}
	var error: String = _notice_validate(intent, "rule")
	if not error.is_empty():
		return error
	state.rule_state["resources"]["month_notice_pending"] = intent.duplicate(true)
	result.presentation.append(intent)
	return ""

func _notice_validate(intent: Dictionary, phase: String) -> String:
	if not _notice_shape(intent) or not is_instance_valid(_notice_owner) or intent["notice_generation"] != _notice_generation:
		return "native notice owner was disposed"
	var reply: Variant = _notice_owner.call("month_start_notice_validate", intent.duplicate(true), phase)
	if not is_instance_valid(_notice_owner) or not reply is Dictionary or not AudioBridge._schema(reply, "tokimemo.month-notice-validation.v1") \
		or not reply.get("error") is String or not AudioBridge._digest_shape(reply.get("intent_sha256")) or reply["intent_sha256"] != AudioBridge.digest(intent):
		return "native notice validation lacks its exact receipt"
	return reply["error"]

static func _notice_shape(intent: Dictionary) -> bool:
	if not intent.get("kind") is String or intent["kind"] != "month_start_notice" \
		or not intent.get("notice_generation") is int or not intent.get("token") is String \
		or intent["token"].is_empty() or intent["token"].length() > 128 \
		or not AudioBridge._digest_shape(intent.get("source_sha256")) or not intent.get("payload") is Dictionary:
		return false
	var payload: Dictionary = intent["payload"]
	return payload.get("selector") is String and payload["selector"] == "schedule_month_data_load_step/september_notice" \
		and payload.get("date") is Array and payload["date"].size() == 3 \
		and payload["date"][0] is int and payload["date"][0] in [95, 97] \
		and payload["date"][1] is int and payload["date"][1] == 9 and payload["date"][2] is int and payload["date"][2] == 1 \
		and payload.get("frame_count") is int and payload["frame_count"] >= 0 \
		and payload.get("state") is Dictionary and payload.get("calls") is Array \
		and payload["calls"] == ["text_system_reset", "message_window_set_title_hidden", "message_window_title_show", "gnsx", "sndisp", "k_disp_start"] \
		and payload.get("title_show") is int and payload["title_show"] == 0 \
		and payload.get("text_attribute") is int and payload["text_attribute"] == 0x1f \
		and payload.get("display_mode") is int and payload["display_mode"] == 3

static func _notice_batch_error(result: Session.FrameResult, current: Variant = null) -> String:
	var source: Dictionary = AudioBridge.source_projection(result)
	var source_error: String = AudioBridge.result_error(source)
	if not source_error.is_empty(): return source_error
	var projection: Dictionary = source["projection"]
	var resources: Dictionary = projection["rule_state"]["resources"]
	if typeof(current) != TYPE_NIL:
		var current_error: String = AudioBridge.projection_error(current)
		if not current_error.is_empty(): return current_error
		var final: Dictionary = current["rule_state"]["resources"]
		if resources.has("month_notice_pending") != final.has("month_notice_pending") \
			or AudioBridge.digest(resources.get("month_notice_pending")) != AudioBridge.digest(final.get("month_notice_pending")):
			return "native notice final receipt differs from pre-draw source emission"
	var expected: Array = []
	if resources.has("month_notice_pending"):
		var notice: Variant = resources["month_notice_pending"]
		if not notice is Dictionary or not _notice_shape(notice): return "native notice source emission is malformed"
		if notice["payload"]["frame_count"] == ((projection["frame_count"] - 1) & 0xffffffff):
			expected.append(AudioBridge.digest(notice))
	var actual: Array = []
	for intent: Dictionary in result.presentation:
		if not intent.get("kind") is String: return "native notice presentation kind is malformed"
		if intent["kind"] == "month_start_notice": actual.append(AudioBridge.digest(intent))
		elif intent.has("notice_generation") or (intent.get("payload") is Dictionary and intent["payload"].get("selector") is String and intent["payload"]["selector"] == "schedule_month_data_load_step/september_notice"):
			return "native September notice was relabeled as a foreign kind"
	if actual != expected: return "complete native September notice differs from pre-draw source emission"
	return ""

func precommit_services(state: State, result: Session.FrameResult) -> String:
	var notice_error: String = _notice_batch_error(result, state.projection())
	if not notice_error.is_empty(): return notice_error
	var error: String = audio.precommit(state, result)
	if not error.is_empty():
		return error
	var seen_notice: bool = false
	for intent: Dictionary in result.presentation:
		if not intent.get("kind") is String:
			return "native presentation intent kind is malformed"
		if intent["kind"] == "month_start_notice":
			if seen_notice:
				return "native notice source call was duplicated"
			seen_notice = true
			if state.rule_state["resources"].get("month_notice_pending") != intent:
				return "native notice pending receipt changed before commit"
			error = _notice_validate(intent, "precommit")
			if not error.is_empty():
				return error
			if intent["payload"]["date"] != [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]] or intent["payload"]["frame_count"] != ((state.frame_count - 1) & 0xffffffff):
				return "native notice source date/frame changed before commit"
	return ""

func publish_services(result: Session.FrameResult) -> String:
	if not result.committed:
		return "aborted month frame cannot publish services"
	var notice_error: String = _notice_batch_error(result)
	if not notice_error.is_empty(): return notice_error
	var audio_error: String = audio.publication_error(result)
	if not audio_error.is_empty(): return audio_error
	for intent: Dictionary in result.presentation:
		if not intent.get("kind") is String:
			return "native notice publication kind is malformed"
		if intent["kind"] != "month_start_notice":
			continue
		if _notice_published.has(AudioBridge.digest(intent)):
			return "native notice was already published"
		var error: String = _notice_validate(intent, "publication")
		if not error.is_empty():
			return error
		var reply: Variant = _notice_owner.call("month_start_notice_publish", intent.duplicate(true))
		if not is_instance_valid(_notice_owner) or not reply is Dictionary or not AudioBridge._schema(reply, "tokimemo.month-notice-validation.v1") \
			or not reply.get("error") is String or not AudioBridge._digest_shape(reply.get("intent_sha256")) or reply["intent_sha256"] != AudioBridge.digest(intent):
			return "native notice publication lacks its exact receipt"
		if not reply["error"].is_empty():
			return reply["error"]
		_notice_published[AudioBridge.digest(intent)] = true
	return audio.publish(result)

static func branch_for(year: int, month: int, day: int) -> String:
	if month % 3 != 0:
		return "ordinary"
	if month == 9 and year == 96 and day == 1:
		return "september96_skip"
	return "september_notice" if month == 9 else "quarter_music"

static func _quarter_effects(state: State, result: Session.FrameResult) -> void:
	if state.rule_state["month"] == 9:
		state.rule_state["player_flags_upper"] &= ~4
	state.rule_state["resources"]["hud_season_fade"] = 0xef
	result.presentation.append({"kind": "month_quarter_effects", "fade": 0xef,
		"amusement_reset_called": true})

func service_completion(event: Session.ServiceEvent, base: Core, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = base as State
	result.error = _phase_error(state)
	if not result.error.is_empty():
		return
	if event.service != SERVICE or [state.mode_stage, state.mode_step, state.substep] != [0, 0, 1]:
		result.error = "month-start completion arrived outside its native read wait"
		return
	var pending: Variant = state.rule_state["resources"].get(PENDING_KEY)
	if not pending is Dictionary or not _matches_pending(state, pending) or event.payload.size() != pending.size():
		result.error = "month-start completion lacks its matching pending source identity"
		return
	for key: String in _request(state):
		if typeof(event.payload.get(key)) != typeof(pending[key]) or event.payload[key] != pending[key]:
			result.error = "month-start completion differs from source request field: " + key
			return
	if not event.payload.get("ready") is bool:
		result.error = "month-start completion requires boolean readiness"
		return
	if pending["ready"]:
		result.error = "month-start resource read is already complete"
		return
	pending["ready"] = event.payload["ready"]

func stage_services(state: State, result: Session.FrameResult) -> Dictionary:
	var prepared: Array[Dictionary] = []
	for intent: Dictionary in result.presentation:
		if intent.get("kind") != "month_calendar_load":
			continue
		var failure: String = _phase_error(state)
		if not failure.is_empty():
			return {"error": failure}
		if intent.size() != 2 or intent.get("request") != _request(state) or [state.mode_stage, state.mode_step, state.substep] != [0, 0, 1]:
			return {"error": "month-start resource intent differs from its source read phase"}
		var pending: Variant = state.rule_state["resources"].get(PENDING_KEY)
		if not pending is Dictionary or not _matches_pending(state, pending) or pending["ready"]:
			return {"error": "month-start resource staging lacks its pending native read"}
		# Read/hash/decode happened during explicit source preflight. Completion
		# reports that the same immutable CALROM payload is available.
		var payload: Dictionary = pending.duplicate(true)
		payload["ready"] = true
		prepared.append({"service": SERVICE, "payload": payload})
	return {"error": "", "services": prepared}

func _phase_error(state: State) -> String:
	if state == null:
		return "month-start requires transactional school-day state"
	var failure: String = state.structural_error()
	if not failure.is_empty():
		return failure
	var data: Dictionary = state.rule_state
	if identity().is_empty() or data["resources"].get(SOURCE_KEY) != identity():
		return "month-start requires the validated external original CALROM source"
	if state.game_mode != 0x38 or data["day_events"][data["day_event_index"]][0] != 0xfe or data["day"] != 1:
		return "month-start requires the actual first-day FE queue record"
	var music_branch: bool = branch_for(data["year"], data["month"], data["day"]) in ["quarter_music", "september_notice"]
	if branch_for(data["year"], data["month"], data["day"]) == "september_notice" and (state.mode_step == 1 or state.mode_stage == 1):
		var notice: Variant = data["resources"].get("month_notice_pending")
		if not notice is Dictionary:
			return "September wait/exit lost its source notice"
		var notice_error: String = _notice_validate(notice, "active")
		if not notice_error.is_empty():
			return notice_error
		if notice["payload"]["date"] != [data["year"], data["month"], data["day"]] or notice["payload"]["frame_count"] >= state.frame_count:
			return "September wait/exit has a foreign source notice date/frame"
	if data["vacation"] < 0 or data["vacation"] > 3:
		return "month-start vacation selector is outside original cases"
	if not data["actor_state"].is_empty() or state.mode_stage not in [0, 1] or state.mode_step not in ([0, 1] if music_branch and state.mode_stage == 0 else [0]) \
		or (state.mode_stage == 0 and state.substep not in ([0, 1] if state.mode_step == 0 else [0])):
		return "month-start actor/control phase is outside the source load/stage-exit path"
	var current: Dictionary = _source.month(data["year"], data["month"])
	var next_year: int = data["year"] + (1 if data["month"] == 12 else 0)
	var next_month: int = 1 if data["month"] == 12 else data["month"] + 1
	var following: Dictionary = _source.month(next_year, next_month)
	if not current["error"].is_empty() or not following["error"].is_empty():
		return "month-start original current/next records are unavailable"
	return ""

func _request(state: State) -> Dictionary:
	return {"request": "calendar", "year": state.rule_state["year"], "month": state.rule_state["month"],
		"lba": LBA, "sectors": SECTORS, "address": ADDRESS, "source_sha256": identity()}

func _matches_pending(state: State, pending: Dictionary) -> bool:
	var expected: Dictionary = _request(state)
	if pending.size() != expected.size() + 1 or not pending.get("ready") is bool:
		return false
	for key: String in expected:
		if typeof(pending.get(key)) != typeof(expected[key]) or pending[key] != expected[key]:
			return false
	return true

func _rotate_calendar(state: State) -> String:
	var data: Dictionary = state.rule_state
	var current: Dictionary = _source.month(data["year"], data["month"])
	# The discarded prior record may contain legitimate runtime markers (e.g.
	# exam_results_react writes 0x17). Native memcpy reads only calendar[1].
	if not _matches_prefix(data["calendar"][1], current):
		return "month-start current record differs from original CALROM; no double shift or synthetic repair"
	var next_year: int = data["year"] + (1 if data["month"] == 12 else 0)
	var next_month: int = 1 if data["month"] == 12 else data["month"] + 1
	var following: Dictionary = _source.month(next_year, next_month)
	var promoted: Dictionary = data["calendar"][1].duplicate(true)
	var next_record: Dictionary = data["calendar"][1].duplicate(true)
	for key: String in ["day_kind", "marker"]:
		for index: int in range(following[key].size()):
			next_record[key][index] = following[key][index]
	data["calendar"] = [promoted, next_record]
	return ""

static func _matches_prefix(actual: Dictionary, expected: Dictionary) -> bool:
	for key: String in ["day_kind", "marker"]:
		if actual[key].slice(0, expected[key].size()) != expected[key]:
			return false
	return true

func _matches_loaded_calendar(state: State) -> bool:
	var data: Dictionary = state.rule_state
	var next_year: int = data["year"] + (1 if data["month"] == 12 else 0)
	var next_month: int = 1 if data["month"] == 12 else data["month"] + 1
	return _matches_prefix(data["calendar"][0], _source.month(data["year"], data["month"])) and _matches_prefix(data["calendar"][1], _source.month(next_year, next_month))
