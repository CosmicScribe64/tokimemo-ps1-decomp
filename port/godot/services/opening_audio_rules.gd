extends RefCounted
## T-29021: transactional prologue XA ownership and native logical completion gate.
## Text ownership stays with the sourced prologue text controller. No player reads.

const Math = preload("res://core/native_math.gd")
const COMPLETION_SERVICE: String = "opening.audio"
const FINISH_HOLD: int = 0x3d
const Session = preload("res://core/game_session.gd")


static func _completion_schema(event: RefCounted, service: String, fields: Array[String]) -> String:
	if event.phase != Session.Phase.BEFORE_MODE or event.kind != Session.ServiceKind.COMPLETION or event.service != service:
		return "opening completion arrived outside its declared service/phase/kind"
	var payload: Dictionary = event.payload
	if payload.size() != fields.size() or not payload.has_all(fields):
		return "opening completion fields differ from its exact schema"
	for key: String in ["epoch", "line_id", "clip_index", "completion_index", "bank_epoch"]:
		if payload.has(key) and not payload[key] is int:
			return "opening completion requires an exact integer: " + key
	for key: String in ["epoch", "completion_index", "bank_epoch"]:
		if payload.has(key) and payload[key] < 1:
			return "opening completion has invalid positive ownership: " + key
	if payload.has("line_id") and (payload["line_id"] < 0 or payload["line_id"] > 42):
		return "opening completion line is outside original voiced lines"
	if payload.has("clip_index") and (payload["clip_index"] < 0 or payload["clip_index"] > 1):
		return "opening completion clip index is invalid"
	for key: String in ["role", "request_key"]:
		if payload.has(key) and (not payload[key] is String or not RegEx.create_from_string("^[A-Za-z0-9_.-]{1,96}$").search(payload[key])):
			return "opening completion identity is invalid: " + key
	if payload.has("status") and (not payload["status"] is String or payload["status"] not in ["started", "finished"]):
		return "opening completion status is invalid"
	if payload.has("ready") and (not payload["ready"] is bool or not payload["ready"]):
		return "opening bank completion must explicitly report ready true"
	return ""


static func begin_bank(state: RefCounted, role: String, result: RefCounted) -> void:
	var epoch: int = int(state.opening.get("audio_bank", {}).get("epoch", 0)) + 1
	state.opening["audio_bank"] = {"epoch": epoch, "role": role, "ready": false}
	state.opening["audio_profile"] = role
	result.audio.append({"kind": "bank_load", "role": role, "bank_epoch": epoch})


static func bank_complete(event: RefCounted, state: RefCounted, result: RefCounted) -> void:
	result.error = _completion_schema(event, "opening.audio.bank", ["bank_epoch", "role", "ready"])
	if not result.error.is_empty():
		return
	var bank: Dictionary = state.opening.get("audio_bank", {})
	if event.phase != 0 or event.service != "opening.audio.bank" or bank.is_empty() or bank.get("ready") != false or event.payload.get("role") != bank.get("role") or event.payload.get("bank_epoch") != bank.get("epoch") or not event.payload.get("ready") is bool or not event.payload["ready"]:
		result.error = "stale, duplicate or mismatched opening bank completion"
		return
	bank["ready"] = true


static func cancel_voice(state: RefCounted, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {}).duplicate(true)
	audio.merge({"epoch": int(audio.get("epoch", 0)) + 1, "clips": [], "voice_started": false,
		"status": "cancelled"}, true)
	state.opening["audio"] = audio
	result.audio.append({"kind": "native_stop_xa_if_active", "epoch": audio["epoch"]})


static func begin_line(state: RefCounted, prepared: Dictionary, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {}).duplicate(true)
	var epoch: int = int(audio.get("epoch", 0)) + 1
	var clips: Array = prepared.get("clips", [])
	audio.merge({"epoch": epoch, "line_id": int(prepared["line_id"]), "clips": clips.duplicate(true),
		"clip_index": 0, "voice_started": false, "status": "silent" if clips.is_empty() else "requested",
		"completion_index": int(audio.get("completion_index", 0))}, true)
	state.opening["audio"] = audio
	if not clips.is_empty():
		result.audio.append(_intent(audio))
	result.trace.append({"source": "prologue_line_start", "line_id": audio["line_id"], "epoch": epoch})


static func _intent(audio: Dictionary) -> Dictionary:
	var event: Dictionary = audio["clips"][audio["clip_index"]].duplicate(true)
	event.merge({"epoch": audio["epoch"], "line_id": audio["line_id"], "clip_index": audio["clip_index"]}, true)
	return event


static func run_line(state: RefCounted, result: RefCounted) -> void:
	var audio: Dictionary = state.opening.get("audio", {})
	var text: Dictionary = state.opening.get("prologue_text", {})
	if audio.is_empty() or text.get("line_id", -1) != audio.get("line_id") or text.get("iteration", -1) != state.frame_count or not text.get("k_disp_inc_result") is int or text["k_disp_inc_result"] not in [0, 1]:
		result.error = "prologue line requires matching sourced text result for this logical iteration"
		return
	var text_complete: bool = text["k_disp_inc_result"] == 1
	if audio["clips"].is_empty():
		if text_complete:
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			text["scroll_state"] = 0
		else:
			state.mode_step_timer = 0
	elif bool(audio["voice_started"]):
		if audio["status"] == "finished":
			if int(audio["clip_index"]) + 1 < audio["clips"].size():
				audio["clip_index"] += 1
				audio["status"] = "requested"
				result.audio.append(_intent(audio))
			elif text_complete:
				state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
				text["scroll_state"] = 0
		else:
			state.mode_step_timer = 0
	# Native voiced-finished/text-incomplete preserves the accumulated timer;
	# narrator text-incomplete and active voiced playback reset it (prologue.c).
	if Math.u32(state.mode_step_timer) >= FINISH_HOLD:
		state.mode_step_timer = 0
		state.substep += 1


static func complete(event: RefCounted, state: RefCounted, result: RefCounted) -> void:
	result.error = _completion_schema(event, COMPLETION_SERVICE, ["epoch", "line_id", "clip_index", "request_key", "status", "completion_index"])
	if not result.error.is_empty():
		return
	var audio: Dictionary = state.opening.get("audio", {})
	var payload: Dictionary = event.payload
	if event.phase != 0 or event.service != COMPLETION_SERVICE or audio.is_empty() or audio["clips"].is_empty():
		result.error = "opening audio completion has no transactional request"
		return
	var expected: Dictionary = _intent(audio)
	for key: String in ["epoch", "line_id", "clip_index", "request_key"]:
		if payload.get(key) != expected.get(key):
			result.error = "stale or mismatched opening audio completion: " + key
			return
	if payload.get("completion_index", -1) != int(audio["completion_index"]) + 1:
		result.error = "opening audio completion is duplicated or out of order"
		return
	match payload.get("status", ""):
		"started":
			if audio["status"] != "requested":
				result.error = "opening audio start arrived outside its request"
				return
			audio["voice_started"] = true
			audio["status"] = "playing"
		"finished":
			if audio["status"] != "playing":
				result.error = "opening audio finish arrived before its start"
				return
			audio["status"] = "finished"
		_:
			result.error = "unsupported opening audio completion status"
			return
	audio["completion_index"] = payload["completion_index"]
