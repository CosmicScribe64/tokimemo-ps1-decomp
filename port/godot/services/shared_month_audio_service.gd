extends "res://services/audio_service.gd"
## One adapter on the existing AudioService; the backend retains its live player.
const Backend = preload("res://bgm_backend.gd")
const BankRequest = preload("res://services/month_start_bank_request.gd")
const VALIDATION := "tokimemo.month-start-native-validation.v1"
const PROTOCOL := "tokimemo.month-start-native-audio.v1"
const PROFILE := {
	"schema": "tokimemo.shared-native-audio.summer.v3", "role": "season.summer.june1",
	"authority_sha256": "955648930342f82b2e0d8a90d23b6fcec9ee47dfb109471ef35bcf9baa608bd1",
	"inputs_sha256": ["bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae", "37d22463afde8c0fb7ba0e6b4e16e9ceba4270b582478065d7af3ffdb710e509", "80c7fcb3527b56d02a2b86908731c0252fd981373666f11a9ec8c763aeab37d6", "6dad905bf6a54c9dc2bed21fc72478f101f98c1dc4cd3f22a8a10bac33c44867", "6e7e475e3e20d2dd0747c8cff2a18a14485a3cfb7528a98d0821ae28145affbb", "98ffd3831816ef2a872e891c8532f7edf20e2b809aa08a06e71304bd4bfed046"],
	"LBAs": [49048, 49017, 54503, 54430, 54408], "slots": [0, 1], "load_set": 0,
	"sequence": 0, "gains": [86, 62], "raw_reverb_depth": 56, "reverb_preset": 3,
	"vm_reinitialization": false, "logical_file_bytes": true, "hardware_sector_phase": false}
var backend: Backend
var _files: Array[PackedByteArray] = []
var _stage: Dictionary = {}
var _request: Dictionary = {}
var _source_state: Dictionary = {}
var _active_month: Dictionary = {}
var _active_native: Dictionary = {}
var _committed_frame: int = -1
var month_observations: Array[Dictionary] = []

func reset_month_start_owner() -> void:
	# Explicit app lifecycle boundary. Primitive state rewind cannot call this a restore.
	if backend != null:
		backend.reset()
	backend = null
	_files.clear()
	_stage.clear()
	_request.clear()
	_source_state.clear()
	_active_month.clear()
	_active_native.clear()
	_committed_frame = -1
	month_observations.clear()

static func digest(value: Variant) -> String:
	return JSON.stringify(value).sha256_text()

static func receipt(value: Variant, failure: String = "") -> Dictionary:
	return {"schema": VALIDATION, "error": failure, "intent_sha256": digest(value)}

static func _same(left: Variant, right: Variant, depth: int = 0) -> bool:
	# Godot equality can raise an error for unlike nested Variant types.
	# Check each level before comparing; never turn a script error into success.
	if depth > 32 or typeof(left) != typeof(right):
		return false
	if left is Dictionary:
		if left.size() != right.size():
			return false
		for key: Variant in left:
			if not right.has(key) or not _same(left[key], right[key], depth + 1):
				return false
		return true
	if left is Array:
		if left.size() != right.size():
			return false
		for index: int in range(left.size()):
			if not _same(left[index], right[index], depth + 1):
				return false
		return true
	if typeof(left) not in [TYPE_NIL, TYPE_BOOL, TYPE_INT, TYPE_FLOAT, TYPE_STRING]:
		return false
	return left == right

static func _hash_shape(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for index: int in range(value.length()):
		if not "0123456789abcdef".contains(value[index]):
			return false
	return true

static func _owner_shape(value: Variant) -> bool:
	if not value is Dictionary or value.size() != 3:
		return false
	for key: String in ["session_generation", "bank_epoch", "request_serial"]:
		if not value.get(key) is int or value[key] < 1:
			return false
	return true

static func _pending_shape(value: Variant) -> bool:
	return value is Dictionary and value.size() == 7 and value.get("request") is Dictionary \
		and _owner_shape(value.get("owner")) and value.get("token") is String \
		and not value["token"].is_empty() and value["token"].length() <= 128 \
		and value.get("bridge_generation") is int and value["bridge_generation"] > 0 \
		and value.get("status") is int and value.get("operation") is String \
		and _hash_shape(value.get("source_profile_sha256"))

static func _context_shape(value: Variant) -> bool:
	return value is Dictionary and value.size() == 4 and _hash_shape(value.get("profile_sha256")) \
		and _hash_shape(value.get("binding_sha256")) and _hash_shape(value.get("native_state_sha256")) \
		and value.get("raw_reverb_depth") is int and value["raw_reverb_depth"] >= 0 and value["raw_reverb_depth"] <= 255

static func _projection_shape(value: Variant) -> bool:
	if not value is Dictionary or not value.get("rule_state") is Dictionary or not value["rule_state"].get("resources") is Dictionary or not value.get("sound_active") is bool:
		return false
	for key: String in ["frame_count", "game_mode", "mode_stage", "mode_step", "substep"]:
		if not value.get(key) is int:
			return false
	return true

func bind_month_start_backend(value: Backend, files: Array[PackedByteArray], actual_flags: int, actual_mode: int) -> String:
	if backend != null or value == null or files.size() != 6:
		return "one genuine shared native backend and six original inputs required"
	var failure: String = value.bind_month_context(actual_flags, actual_mode)
	if not failure.is_empty():
		return failure
	backend = value
	for bytes: PackedByteArray in files:
		_files.append(bytes.duplicate())
	return ""

func month_start_prepare(payload: Dictionary) -> Dictionary:
	if backend == null or not _stage.is_empty():
		return receipt(payload, "native owner missing or preparation already pending")
	if not payload.get("request") is Dictionary or not payload.get("state") is Dictionary:
		return receipt(payload, "malformed native preparation")
	var state: Dictionary = payload["state"]
	if not _projection_shape(state) or state["frame_count"] <= _committed_frame or not state["sound_active"] or state["game_mode"] != 0x38:
		return receipt(payload, "native source frame/mode/audio state is unavailable or rewound")
	var data: Dictionary = state["rule_state"]
	if not _same([data.get("year"), data.get("month"), data.get("day"), data.get("vacation")], [95, 6, 1, 0]):
		return receipt(payload, "only the proved June1 source profile is supported")
	if digest(payload["request"]) != digest(BankRequest.build(data)):
		return receipt(payload, "foreign native source request")
	var pending: Variant = data["resources"].get("month_audio_pending", {})
	if not pending is Dictionary:
		return receipt(payload, "malformed source pending request")
	var poll: bool = not pending.is_empty()
	if poll and (not _pending_shape(pending) or _active_month.is_empty() or not _same(pending["owner"], _active_month.get("owner")) or not _same(pending["request"], _request) or not _same(pending["token"], _active_month.get("token")) or pending["operation"] not in ["load", "poll"] or pending["status"] not in [0, -1]):
		return receipt(payload, "source pending request has no actual committed native owner")
	var reply: Dictionary = backend.prepare_month([] if poll else _files, poll)
	if not reply.get("error") is String or not reply["error"].is_empty():
		return receipt(payload, str(reply.get("error", "malformed native prepare reply")))
	_stage = reply
	_stage["frame"] = state["frame_count"]
	_stage["poll"] = poll
	_stage["token"] = "month:%d:%d:%d:%d" % [reply["expected_after"]["session_generation"], reply["expected_after"]["bank_epoch"], reply["expected_after"]["request_serial"], reply["ticket"]]
	_request = payload["request"].duplicate(true)
	_source_state = state.duplicate(true)
	return receipt(payload)

func _owner() -> Dictionary:
	var after: Dictionary = _stage["expected_after"]
	return {"session_generation": after["session_generation"], "bank_epoch": after["bank_epoch"], "request_serial": after["request_serial"]}

func _source_context() -> Dictionary:
	return {"profile_sha256": digest(PROFILE), "binding_sha256": digest(_request["binding"]), "native_state_sha256": digest(_stage["expected_after"]), "raw_reverb_depth": _stage["expected_after"]["raw_reverb_depth"]}

func _operations() -> Array:
	if not _stage["poll"]:
		return ["load"]
	return ["poll", "start"] if _stage["poll_status"] == 1 else ["poll"]

func month_start_operation(payload: Dictionary) -> Dictionary:
	var reply: Dictionary = {"schema": PROTOCOL, "error": "", "request_sha256": digest(payload)}
	var failure: String = ""
	if backend == null or _stage.is_empty():
		failure = "native source frame was not prepared"
	elif payload.size() != 6 or not payload.get("frame_count") is int or payload["frame_count"] != _stage["frame"] or not payload.get("request") is Dictionary or not _same(payload["request"], _request) or not payload.get("operation") is String or not payload.get("prior") is Dictionary or not _projection_shape(payload.get("source_projection")):
		failure = "operation differs from prepared source frame/request"
	else:
		var operations: Array = _operations()
		var index: int = operations.find(payload["operation"])
		var projection: Dictionary = payload["source_projection"]
		if index < 0 or not _hash_shape(payload.get("state_sha256")) or payload["state_sha256"] != digest(projection):
			failure = "foreign source operation/state digest"
		elif projection["frame_count"] != _stage["frame"] or projection["game_mode"] != 0x38 or projection["mode_stage"] != 0 or projection["mode_step"] != (1 if _stage["poll"] else 0) or projection["substep"] != (0 if _stage["poll"] else 1) or not projection["sound_active"]:
			failure = "operation lacks its exact source phase projection"
		elif digest(BankRequest.build(projection["rule_state"])) != digest(_request) or not _same(payload["prior"], projection["rule_state"]["resources"].get("month_audio_pending", {})):
			failure = "operation projection source request/prior differs"
		elif _stage["poll"] and not _pending_shape(payload["prior"]):
			failure = "poll/start prior has an invalid typed envelope"
		elif index > 0 and (not _same(payload["prior"]["token"], _stage["token"]) or not _same(payload["prior"]["owner"], _owner()) or payload["prior"]["operation"] != "poll" or payload["prior"]["status"] != 1):
			failure = "start has no same-frame ready native poll"
		elif index == 0 and not _stage["poll"] and not payload["prior"].is_empty():
			failure = "load has an existing pending request"
		elif index == 0 and _stage["poll"] and not _same(payload["prior"], _source_state["rule_state"]["resources"]["month_audio_pending"]):
			failure = "poll differs from the immutable prior native owner"
		elif index > 0 and (not _same(payload["prior"]["request"], _request) or payload["prior"]["source_profile_sha256"] != digest(PROFILE) or not _same(payload["prior"]["bridge_generation"], _source_state["rule_state"]["resources"]["month_audio_pending"].get("bridge_generation"))):
			failure = "start prior source profile/request/generation differs"
		else:
			failure = backend.validate_month(_stage)
	if not failure.is_empty():
		reply["error"] = failure
		return reply
	reply.merge({"owner": _owner(), "token": _stage["token"], "status": _stage["poll_status"] if payload["operation"] == "poll" else 0, "source_context": _source_context()})
	return reply

func _intent_error(intent: Dictionary) -> String:
	if intent.size() != 8 or not intent.get("kind") is String or not intent.get("operation") is String or not intent.get("token") is String or not _owner_shape(intent.get("owner")) or not intent.get("status") is int or not _context_shape(intent.get("source_context")) or not intent.get("bridge_generation") is int or intent["bridge_generation"] < 1 or not intent.get("payload") is Dictionary:
		return "malformed native intent envelope"
	if _stage.is_empty() or intent["kind"] != "native_month_start" or intent["token"] != _stage["token"] or not _same(intent["owner"], _owner()):
		return "foreign native ticket/owner/intent"
	if not intent.get("payload") is Dictionary:
		return "native operation payload is malformed"
	var reply: Dictionary = month_start_operation(intent["payload"])
	if reply["error"].is_empty() and _same(intent["operation"], intent["payload"].get("operation")) and _same(intent["status"], reply["status"]) and _same(intent["source_context"], reply["source_context"]):
		return ""
	return "intent is not the exact issued native operation receipt"

func month_start_validate(intent: Dictionary, phase: String) -> Dictionary:
	if backend == null:
		return receipt(intent, "native owner missing")
	if phase == "active":
		if intent.size() != 6 or not intent.get("kind") is String or not intent.get("operation") is String or not intent.get("request") is Dictionary or not intent.get("token") is String or not _owner_shape(intent.get("owner")) or not intent.get("bridge_generation") is int:
			return receipt(intent, "malformed active native intent envelope")
		var current: Dictionary = backend.month_start_context()
		if not _same(_active_month.get("operation"), "start") or intent["kind"] != "native_month_start" or intent["operation"] != "active" or not _same(intent["request"], _request) or not _same(intent["token"], _active_month.get("token")) or not _same(intent["owner"], _active_month.get("owner")) or not _same(intent["bridge_generation"], _active_month.get("bridge_generation")) or not _same(current.get("error"), "") or not _same(current.get("profile"), "season.summer.june1") or not _same(current.get("started"), true):
			return receipt(intent, "actual started native owner/context is unavailable")
		if _active_native.is_empty() or not _owner_shape(intent["owner"]):
			return receipt(intent, "committed native after-context is unavailable")
		for key: String in ["session_generation", "bank_epoch", "request_serial"]:
			if not current.get(key) is int or current[key] < 1 or not _same(current[key], intent["owner"][key]):
				return receipt(intent, "active native owner generation/epoch/request changed")
		for key: String in ["session_generation", "bank_epoch", "request_serial", "backend_generation", "sink_generation", "profile", "vm_initializations", "flags", "request_flags", "gain", "handle", "sequence", "next_sequence", "music_slot", "SE_slot", "SE_used", "music_used", "SE_header_lba", "music_header_lba", "depth_register", "raw_reverb_depth", "loading", "started", "poll_ordinal"]:
			if not _same(current.get(key), _active_native.get(key)):
				return receipt(intent, "active committed native context changed: " + key)
		for key: String in ["sample_cursor", "revision"]:
			if not current.get(key) is int or not _active_native.get(key) is int or current[key] < _active_native[key]:
				return receipt(intent, "active native cursor/revision rewound")
		return receipt(intent)
	if phase not in ["rule", "precommit", "publication"]:
		return receipt(intent, "unknown native validation phase")
	var failure: String = _intent_error(intent)
	if failure.is_empty():
		failure = backend.validate_month(_stage)
	return receipt(intent, failure)

func month_start_validate_batch(intents: Array, final_state: Dictionary, phase: String) -> Dictionary:
	# Called even for an empty native subset. Source receipts cannot disappear.
	var payload: Dictionary = {"intents": intents, "final_projection": final_state, "phase": phase}
	if _stage.is_empty() or phase not in ["precommit", "publication"] or not _projection_shape(final_state) or final_state["frame_count"] != ((_stage["frame"] + 1) & 0xffffffff) or not final_state["sound_active"]:
		return receipt(payload, "native final source frame/batch unavailable")
	var selected: Array[Dictionary] = []
	for raw: Variant in intents:
		if not raw is Dictionary or not raw.get("kind") is String:
			return receipt(payload, "malformed audio intent")
		if raw.get("kind") == "native_month_start":
			selected.append(raw)
		elif _same(raw.get("token"), _stage["token"]):
			return receipt(payload, "native source ticket was relabeled")
	var operations: Array = _operations()
	if selected.size() != operations.size():
		return receipt(payload, "missing or duplicate source audio operations")
	for index: int in range(operations.size()):
		if not selected[index].get("operation") is String or selected[index]["operation"] != operations[index]:
			return receipt(payload, "native source batch order differs")
		var validation: Dictionary = month_start_validate(selected[index], "precommit")
		if not validation["error"].is_empty():
			return receipt(payload, validation["error"])
	var tail: Dictionary = selected[-1]
	var expected_emission: Array[String] = []
	for intent: Dictionary in selected:
		expected_emission.append(digest(intent))
	var emission: Variant = final_state["rule_state"]["resources"].get("month_audio_emission")
	if not emission is Dictionary or not _same(emission, {"frame_count": _stage["frame"], "intents": expected_emission}):
		return receipt(payload, "complete native source emission receipt differs")
	if operations == ["poll", "start"]:
		var expected_start: Dictionary = selected[0]["payload"]["source_projection"].duplicate(true)
		var poll: Dictionary = selected[0]
		expected_start["rule_state"]["resources"]["month_audio_pending"] = {"request": _request, "owner": poll["owner"], "token": poll["token"], "bridge_generation": poll["bridge_generation"], "status": 1, "operation": "poll", "source_profile_sha256": poll["source_context"]["profile_sha256"]}
		expected_start["rule_state"]["resources"]["month_audio_emission"] = {"frame_count": _stage["frame"], "intents": [digest(poll)]}
		if not _same(selected[1]["payload"]["source_projection"], expected_start):
			return receipt(payload, "ready poll to start source projection differs")
	var ready: bool = operations == ["poll", "start"]
	if final_state["game_mode"] != 0x38 or final_state["mode_stage"] != (1 if ready else 0) or final_state["mode_step"] != (0 if ready else 1) or final_state["substep"] != 0:
		return receipt(payload, "final source mode phase differs from prepared operations")
	var pending: Variant = final_state.get("rule_state", {}).get("resources", {}).get("month_audio_pending")
	var expected: Dictionary = {"request": _request, "owner": tail["owner"], "token": tail["token"], "bridge_generation": tail["bridge_generation"], "status": tail["status"], "operation": tail["operation"], "source_profile_sha256": tail["source_context"]["profile_sha256"]}
	if not _pending_shape(pending) or not _same(pending, expected):
		return receipt(payload, "final native pending source record differs")
	return receipt(payload)

func month_start_publish_batch(intents: Array, final_state: Dictionary) -> Dictionary:
	var payload: Dictionary = {"intents": intents, "final_projection": final_state}
	var validation: Dictionary = month_start_validate_batch(intents, final_state, "publication")
	if not validation["error"].is_empty():
		return receipt(payload, validation["error"])
	var result: Dictionary = backend.publish_month(_stage)
	if not result.get("error") is String or not result["error"].is_empty():
		return receipt(payload, str(result.get("error", "malformed native publication")))
	for event: Dictionary in result["observations"]:
		var observation: Dictionary = event.duplicate(true)
		observation.merge({"profile": "season.summer.june1", "source_profile_sha256": digest(PROFILE), "session_generation": _owner()["session_generation"], "bank_epoch": _owner()["bank_epoch"], "request_serial": _owner()["request_serial"]})
		month_observations.append(observation)
	_committed_frame = _stage["frame"]
	_active_native = _stage["expected_after"].duplicate(true)
	_active_native["sink_generation"] = _stage["sink_generation"]
	_active_native["backend_generation"] = _stage["backend_generation"]
	var tail: Dictionary = final_state["rule_state"]["resources"]["month_audio_pending"]
	_active_month = {"owner": tail["owner"], "token": tail["token"], "bridge_generation": tail["bridge_generation"], "operation": tail["operation"]}
	return receipt(payload)

func month_start_discard(frame_count: int) -> Dictionary:
	var failure: String = ""
	if not _stage.is_empty():
		if frame_count != _stage["frame"]:
			failure = "discard frame differs from private native ticket"
		else:
			backend.discard_month(_stage["ticket"])
			_stage.clear()
	return {"schema": VALIDATION, "error": failure, "frame_count": frame_count}
