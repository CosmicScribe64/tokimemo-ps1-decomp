extends RefCounted
## Pure rule-side bridge to the existing AudioService owner. No DSP/I/O/player work.
## Native preparation/publication belongs to the service, outside Session.step.
const AudioService = preload("res://services/audio_service.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const BankRequest = preload("res://services/month_start_bank_request.gd")
const PROTOCOL := "tokimemo.month-start-native-audio.v1"
const VALIDATION := "tokimemo.month-start-native-validation.v1"
const KIND := "native_month_start"
const PENDING := "month_audio_pending"
const EMISSION := "month_audio_emission"
var _owner: Variant = null
var _generation: int = 0
var _published: Dictionary = {}
var _prepared_frame: int = -1
var _prepare_error: String = ""
var _last_published_frame: int = -1

static func owner_error(value: Variant) -> String:
	if not value is Object or not is_instance_valid(value) or not value is AudioService:
		return "month-start requires the existing live AudioService owner"
	for method: String in ["month_start_prepare", "month_start_discard", "month_start_operation", "month_start_validate", "month_start_validate_batch", "month_start_publish_batch"]:
		if not value.has_method(method):
			return "AudioService has no validated native month-start interface: " + method
	return ""

func bind_owner(value: Variant) -> String:
	var error: String = owner_error(value)
	if not error.is_empty():
		return error
	_owner = value
	_generation += 1
	_published.clear()
	_prepared_frame = -1
	_last_published_frame = -1
	return ""

func reset() -> void:
	_owner = null
	_generation += 1
	_published.clear()
	_prepared_frame = -1
	_prepare_error = ""
	_last_published_frame = -1

func prepare(state: State, request: Dictionary) -> String:
	_prepared_frame = -1
	_prepare_error = "validated native month-start audio context/profile is unavailable"
	if not _request_matches(state, request):
		_prepare_error = "native month-start preparation has a foreign source request"
		return _prepare_error
	_prepared_frame = state.frame_count
	if _prepared_frame <= _last_published_frame:
		_prepare_error = "native audio frame rewind needs its restored source owner"
		return _prepare_error
	if not _live():
		return _prepare_error
	var payload: Dictionary = {"request": request.duplicate(true), "state": state.projection()}
	var reply: Variant = _owner.call("month_start_prepare", payload)
	if not _live() or not reply is Dictionary or not _schema(reply, VALIDATION) \
		or not reply.get("error") is String or not _digest_shape(reply.get("intent_sha256")) or reply["intent_sha256"] != digest(payload):
		_prepare_error = "native audio preparation lacks its exact receipt"
	else:
		_prepare_error = reply["error"]
	return _prepare_error

func discard(frame_count: int) -> String:
	_prepared_frame = -1
	_prepare_error = ""
	if not _live():
		return ""
	var reply: Variant = _owner.call("month_start_discard", frame_count)
	if not _live() or not reply is Dictionary or not _schema(reply, VALIDATION) \
		or not reply.get("error") is String or not reply.get("frame_count") is int or reply["frame_count"] != frame_count:
		return "native audio discard lacks its exact frame receipt"
	return reply["error"]

func _live() -> bool:
	return is_instance_valid(_owner) and _owner is AudioService

static func _owner_shape(value: Variant) -> bool:
	if not value is Dictionary or value.size() != 3:
		return false
	for key: String in ["session_generation", "bank_epoch", "request_serial"]:
		if not value.get(key) is int or value[key] < 1:
			return false
	return value["request_serial"] > 0

static func digest(value: Variant) -> String:
	return JSON.stringify(value).sha256_text()

static func _schema(value: Dictionary, expected: String) -> bool:
	return value.get("schema") is String and value["schema"] == expected

static func _digest_shape(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for index: int in range(64):
		if not "0123456789abcdef".contains(value[index]):
			return false
	return true

static func _pending_shape(value: Variant) -> bool:
	return value is Dictionary and value.get("request") is Dictionary and value.get("bridge_generation") is int \
		and _owner_shape(value.get("owner")) and value.get("token") is String and not value["token"].is_empty() \
		and value.get("status") is int and value.get("operation") is String and _digest_shape(value.get("source_profile_sha256"))

static func _intent_shape(value: Dictionary) -> bool:
	if not value.get("kind") is String or value["kind"] != KIND or not value.get("operation") is String \
		or value["operation"] not in ["load", "poll", "start"] or not value.get("bridge_generation") is int \
		or not _owner_shape(value.get("owner")) or not value.get("token") is String or value["token"].is_empty() \
		or not value.get("status") is int or not value.get("payload") is Dictionary:
		return false
	var payload: Dictionary = value["payload"]
	return payload.get("request") is Dictionary and payload.get("operation") is String \
		and payload["operation"] == value["operation"] and payload.get("frame_count") is int \
		and _digest_shape(payload.get("state_sha256")) and payload.get("prior") is Dictionary \
		and payload.get("source_projection") is Dictionary and digest(payload["source_projection"]) == payload["state_sha256"] \
		and _context_shape(value.get("source_context"), payload["request"])

static func _context_shape(context: Variant, request: Dictionary) -> bool:
	return request.get("binding") is Dictionary and context is Dictionary and context.size() == 4 \
		and _digest_shape(context.get("profile_sha256")) and _digest_shape(context.get("binding_sha256")) \
		and context["binding_sha256"] == digest(request["binding"]) and _digest_shape(context.get("native_state_sha256")) \
		and context.get("raw_reverb_depth") is int and context["raw_reverb_depth"] >= 0 and context["raw_reverb_depth"] <= 255

static func _request_matches(state: State, request: Dictionary) -> bool:
	if state == null or not state.structural_error().is_empty() or not state._plain_record(request, 0):
		return false
	var expected: Dictionary = BankRequest.build(state.rule_state)
	return not expected.is_empty() and digest(request) == digest(expected)

func _validate(intent: Dictionary, phase: String) -> String:
	if phase != "active" and not _intent_shape(intent):
		return "malformed native month-start intent"
	if not _live():
		return "native month-start audio owner is absent or disposed"
	var reply: Variant = _owner.call("month_start_validate", intent.duplicate(true), phase)
	if not _live() or not reply is Dictionary or not _schema(reply, VALIDATION) \
		or not reply.get("error") is String or not _digest_shape(reply.get("intent_sha256")) or reply["intent_sha256"] != digest(intent):
		return "native audio validation did not return its exact structured receipt"
	return reply["error"]

func operation(state: State, request: Dictionary, op: String, result: Session.FrameResult) -> Dictionary:
	if not _request_matches(state, request):
		return {"error": "native month-start operation has a foreign source request"}
	if not state.sound_active:
		return {"error": "native month-start audio phase is inactive"}
	if not _live():
		return {"error": "validated native month-start audio context/profile is unavailable"}
	if _prepared_frame != state.frame_count or not _prepare_error.is_empty():
		return {"error": _prepare_error if not _prepare_error.is_empty() else "native audio was not prepared outside this frame"}
	var prior: Variant = state.rule_state["resources"].get(PENDING, {})
	if op == "load" and (not prior is Dictionary or not prior.is_empty()):
		return {"error": "native month-start load already has a pending request"}
	if op not in ["load", "poll", "start"]:
		return {"error": "unsupported month-start native operation"}
	if op != "load" and (not _pending_shape(prior) or prior["request"] != request or prior["bridge_generation"] != _generation):
		return {"error": "native month-start operation lacks its current source request/owner"}
	if (op == "poll" and (prior["operation"] not in ["load", "poll"] or prior["status"] not in [-1, 0])) \
		or (op == "start" and (prior["operation"] != "poll" or prior["status"] != 1)):
		return {"error": "native poll/start is outside source request/wait order"}
	var projection: Dictionary = state.projection()
	var payload: Dictionary = {"operation": op, "request": request.duplicate(true),
		"frame_count": state.frame_count, "state_sha256": digest(projection), "source_projection": projection,
		"prior": prior.duplicate(true) if op != "load" else {}}
	var reply: Variant = _owner.call("month_start_operation", payload.duplicate(true))
	if not _live() or not reply is Dictionary or not _schema(reply, PROTOCOL) \
		or not reply.get("error") is String or not _digest_shape(reply.get("request_sha256")) or reply["request_sha256"] != digest(payload):
		return {"error": "native audio operation did not return its exact structured receipt"}
	if not reply["error"].is_empty():
		return {"error": reply["error"]}
	if not _owner_shape(reply.get("owner")) or not reply.get("token") is String \
		or reply["token"].is_empty() or reply["token"].length() > 128 or not reply.get("status") is int:
		return {"error": "native audio reply has malformed owner/token/status"}
	var context: Variant = reply.get("source_context")
	if not _context_shape(context, request):
		return {"error": "native audio reply lacks its validated source/bank/live context"}
	if op == "start" and request["binding"]["sequence"] == 0xd488 and context["raw_reverb_depth"] != 56:
		return {"error": "SUMMER sequence0 requires its source raw reverb byte56"}
	if op != "load" and context["profile_sha256"] != prior.get("source_profile_sha256"):
		return {"error": "native source profile changed during the bank request"}
	if (op != "load" and reply["owner"] != prior["owner"]) or (op == "poll" and reply["status"] not in [-1, 1]) \
		or (op != "poll" and reply["status"] != 0):
		return {"error": "native audio reply has stale owner or non-source poll status"}
	var intent: Dictionary = {"kind": KIND, "operation": op, "payload": payload,
		"owner": reply["owner"].duplicate(true), "token": reply["token"], "status": reply["status"],
		"bridge_generation": _generation, "source_context": context.duplicate(true)}
	var error: String = _validate(intent, "rule")
	if not error.is_empty():
		return {"error": error}
	var pending: Dictionary = {"request": request.duplicate(true), "owner": reply["owner"].duplicate(true),
		"token": reply["token"], "bridge_generation": _generation, "status": reply["status"], "operation": op}
	pending["source_profile_sha256"] = context["profile_sha256"]
	state.rule_state["resources"][PENDING] = pending
	var emission: Variant = state.rule_state["resources"].get(EMISSION, {})
	if not emission is Dictionary or (not emission.is_empty() and not _emission_shape(emission)):
		return {"error": "native audio source emission receipt is malformed"}
	if emission.is_empty() or emission["frame_count"] != state.frame_count:
		emission = {"frame_count": state.frame_count, "intents": []}
	if not emission.get("intents") is Array or emission["intents"].size() >= 2:
		return {"error": "native audio source emission batch is outside source order"}
	emission["intents"].append(digest(intent))
	state.rule_state["resources"][EMISSION] = emission
	result.audio.append(intent)
	return {"error": "", "status": reply["status"]}

func active(state: State) -> String:
	var pending: Variant = state.rule_state["resources"].get(PENDING)
	if not state.sound_active or not _pending_shape(pending) or pending["operation"] != "start" or pending["bridge_generation"] != _generation:
		return "month-start exit lacks its live started native owner"
	return _validate({"kind": KIND, "operation": "active", "request": pending["request"],
		"owner": pending["owner"], "token": pending["token"], "bridge_generation": _generation}, "active")

static func result_error(reply: Dictionary) -> String:
	# A script failure can return a default empty Dictionary. Never interpret a
	# missing/malformed error envelope as successful internal validation.
	return reply["error"] if reply.get("error") is String else "native service validation returned a malformed error envelope"

static func projection_error(value: Variant) -> String:
	if not value is Dictionary:
		return "native service projection must be a complete state Dictionary"
	var probe := State.new()
	var shape: Dictionary = probe.projection()
	if value.size() != shape.size():
		return "native service projection fields differ from the complete native state"
	for key: String in shape:
		if not value.has(key) or typeof(value[key]) != typeof(shape[key]):
			return "native service projection field has a foreign type: " + key
	# Type guards must precede comparisons and restore into typed state fields.
	if value["frame_count"] < 0 or value["frame_count"] > 0xffffffff:
		return "native service projection frame is outside source u32"
	if not probe._plain_record(value["rule_state"], 0):
		return "native service projection rules are not bounded primitive state"
	probe.restore(value)
	return probe.structural_error()

static func _trace_projection(result: Session.FrameResult, phase: String) -> Dictionary:
	var found: bool = false
	var projection: Dictionary = {}
	for record: Dictionary in result.trace:
		if record.get("phase") is String and record["phase"] == phase:
			if found:
				return {"error": "native service trace has a duplicate projection: " + phase}
			found = true
			var error: String = projection_error(record.get("state"))
			if not error.is_empty(): return {"error": error}
			projection = record["state"]
	if not found:
		return {"error": "native service trace lacks its required projection: " + phase}
	return {"error": "", "projection": projection}

static func source_projection(result: Session.FrameResult) -> Dictionary:
	# Deep source projection captured before the draw callbacks.
	return _trace_projection(result, "draw_logic")

static func committed_projection(result: Session.FrameResult) -> Dictionary:
	return _trace_projection(result, "audio")

static func _marked(intent: Dictionary) -> bool:
	return intent.has("bridge_generation") \
		or (intent.get("payload") is Dictionary and intent["payload"].get("request") is Dictionary \
			and intent["payload"]["request"].get("selector") is String and intent["payload"]["request"]["selector"] == "hud_day_bgm_load")

static func _emission_shape(emission: Variant) -> bool:
	if not emission is Dictionary or emission.size() != 2 or not emission.get("frame_count") is int \
		or emission["frame_count"] < 0 or emission["frame_count"] > 0xffffffff \
		or not emission.get("intents") is Array or emission["intents"].size() < 1 or emission["intents"].size() > 2:
		return false
	for entry: Variant in emission["intents"]:
		if not _digest_shape(entry): return false
	return true

static func _batch(result: Session.FrameResult, current: Variant = null) -> Dictionary:
	var source: Dictionary = source_projection(result)
	var source_error: String = result_error(source)
	if not source_error.is_empty(): return {"error": source_error}
	var projection: Dictionary = source["projection"]
	var frame: int = (projection["frame_count"] - 1) & 0xffffffff
	var resources: Dictionary = projection["rule_state"]["resources"]
	if typeof(current) != TYPE_NIL:
		var current_error: String = projection_error(current)
		if not current_error.is_empty(): return {"error": current_error}
		if current["frame_count"] != projection["frame_count"]:
			return {"error": "native audio batch final frame/resources changed"}
		for key: String in [EMISSION, PENDING]:
			var final_resources: Dictionary = current["rule_state"]["resources"]
			if resources.has(key) != final_resources.has(key) or digest(resources.get(key)) != digest(final_resources.get(key)):
				return {"error": "native audio final receipt differs from pre-draw source emission"}
	var expected: Array = []
	if resources.has(EMISSION):
		var emission: Variant = resources[EMISSION]
		if not _emission_shape(emission):
			return {"error": "native audio source emission shape is invalid"}
		if emission["frame_count"] == frame: expected = emission["intents"]
	var actual: Array = []
	var intents: Array[Dictionary] = []
	for intent: Dictionary in result.audio:
		if not intent.get("kind") is String: return {"error": "native audio intent kind is malformed"}
		if intent["kind"] == KIND:
			actual.append(digest(intent))
			intents.append(intent)
		elif _marked(intent):
			return {"error": "native month audio was relabeled as a foreign kind"}
	if actual != expected:
		return {"error": "complete native month audio batch differs from pre-draw source emission"}
	return {"error": "", "intents": intents, "frame_count": frame}

func _validate_batch(intents: Array[Dictionary], projection: Dictionary, phase: String) -> String:
	if not _live(): return "native audio complete batch lost its service owner"
	var payload: Dictionary = {"intents": intents.duplicate(true), "final_projection": projection.duplicate(true), "phase": phase}
	var reply: Variant = _owner.call("month_start_validate_batch", payload["intents"], payload["final_projection"], phase)
	if not _live() or not reply is Dictionary or not _schema(reply, VALIDATION) or not reply.get("error") is String \
		or not _digest_shape(reply.get("intent_sha256")) or reply["intent_sha256"] != digest(payload):
		return "native audio complete batch validation lacks its exact receipt"
	return reply["error"]

func precommit(state: State, result: Session.FrameResult) -> String:
	var batch: Dictionary = _batch(result, state.projection())
	var receipt_error: String = result_error(batch)
	if not receipt_error.is_empty(): return receipt_error
	# A service-owned prepared frame is validated even when zero intents survived.
	if _prepared_frame == batch["frame_count"] or not batch["intents"].is_empty():
		var batch_error: String = _validate_batch(batch["intents"], state.projection(), "precommit")
		if not batch_error.is_empty(): return batch_error
	var tail: Dictionary = {}
	var operations: Array = []
	var seen: Dictionary = {}
	for intent: Dictionary in result.audio:
		if not intent.get("kind") is String:
			return "native audio intent kind is malformed"
		if intent["kind"] != KIND:
			continue
		if not _intent_shape(intent) or seen.has(digest(intent)):
			return "malformed or duplicate native month-start intent"
		if not state.sound_active or not _request_matches(state, intent["payload"]["request"]):
			return "native month-start source request/audio phase changed before commit"
		if (intent["operation"] == "poll" and intent["status"] not in [-1, 1]) \
			or (intent["operation"] != "poll" and intent["status"] != 0):
			return "native month-start status changed before commit"
		if intent["operation"] == "start" and intent["payload"]["request"]["binding"]["sequence"] == 0xd488 and intent["source_context"]["raw_reverb_depth"] != 56:
			return "SUMMER source start reverb changed before commit"
		seen[digest(intent)] = true
		operations.append(intent["operation"])
		if intent["payload"]["frame_count"] != ((state.frame_count - 1) & 0xffffffff):
			return "native audio intent does not belong to this source frame"
		if intent.get("bridge_generation") != _generation:
			return "native audio bridge was rebound before commit"
		var pending: Variant = state.rule_state["resources"].get(PENDING)
		if not _pending_shape(pending) or pending["owner"] != intent["owner"]:
			return "native audio pending owner/token changed before commit"
		tail = intent
		var error: String = _validate(intent, "precommit")
		if not error.is_empty():
			return error
	if not operations.is_empty() and operations not in [["load"], ["poll"], ["poll", "start"]]:
		return "native month-start operation batch differs from source order"
	if not tail.is_empty():
		var pending: Dictionary = state.rule_state["resources"][PENDING]
		for key: String in ["token", "operation", "status", "bridge_generation"]:
			if typeof(pending.get(key)) != typeof(tail[key]) or pending[key] != tail[key]:
				return "native audio final pending operation changed before commit"
		if pending["request"] != tail["payload"]["request"] or pending["source_profile_sha256"] != tail["source_context"]["profile_sha256"]:
			return "native audio final source profile/request changed before commit"
	return ""

func publication_error(result: Session.FrameResult) -> String:
	if not result.committed:
		return "aborted frames cannot publish native month-start audio"
	var batch: Dictionary = _batch(result)
	var receipt_error: String = result_error(batch)
	if not receipt_error.is_empty(): return receipt_error
	if not batch["intents"].is_empty():
		var final: Dictionary = committed_projection(result)
		receipt_error = result_error(final)
		if not receipt_error.is_empty(): return receipt_error
		var final_batch: Dictionary = _batch(result, final["projection"])
		receipt_error = result_error(final_batch)
		if not receipt_error.is_empty(): return receipt_error
		var batch_error: String = _validate_batch(batch["intents"], final["projection"], "publication")
		if not batch_error.is_empty(): return batch_error
	for intent: Dictionary in result.audio:
		if not intent.get("kind") is String:
			return "malformed committed native audio kind"
		if intent["kind"] != KIND:
			continue
		if not _intent_shape(intent):
			return "malformed committed native audio intent"
		if _published.has(digest(intent)):
			return "native month-start operation was already published"
		var error: String = _validate(intent, "publication")
		if not error.is_empty():
			return error
	return ""

func publish(result: Session.FrameResult) -> String:
	var error: String = publication_error(result)
	if not error.is_empty(): return error
	var batch: Dictionary = _batch(result)
	error = result_error(batch)
	if not error.is_empty(): return error
	if batch["intents"].is_empty(): return ""
	var final: Dictionary = committed_projection(result)
	error = result_error(final)
	if not error.is_empty(): return error
	var payload: Dictionary = {"intents": batch["intents"].duplicate(true), "final_projection": final["projection"].duplicate(true)}
	var reply: Variant = _owner.call("month_start_publish_batch", payload["intents"], payload["final_projection"])
	if not _live() or not reply is Dictionary or not _schema(reply, VALIDATION) \
		or not reply.get("error") is String or not _digest_shape(reply.get("intent_sha256")) or reply["intent_sha256"] != digest(payload):
		return "native audio batch publication did not return its exact receipt"
	if not reply["error"].is_empty(): return reply["error"]
	for intent: Dictionary in batch["intents"]:
		_published[digest(intent)] = true
		_last_published_frame = intent["payload"]["frame_count"]
	return ""
