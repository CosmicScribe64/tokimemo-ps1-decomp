extends RefCounted
## T-29021: external effects publish only after commit; signals enqueue observations.
## The next BEFORE_MODE delivery, not a player getter, decides logical progression.

const Session = preload("res://core/game_session.gd")
const AudioService = preload("res://services/audio_service.gd")
const VoicePlayback = preload("res://services/original_voice_playback.gd")

var audio: AudioService
var voice: VoicePlayback = VoicePlayback.new()
var _voice_player: AudioStreamPlayer
var _bgm_player: AudioStreamPlayer
var _pending: Array[Dictionary] = []
var _active_request: Dictionary = {}
var _completion_index: int = 0

const INTENT_FIELDS: Dictionary = {
	"original_bgm_start": ["kind", "role", "bank_epoch", "request_serial", "session_generation"],
	"voice_play": ["kind", "voice_id", "speaker", "xa_request", "request_key", "epoch", "line_id", "clip_index"],
	"bank_load": ["kind", "role", "bank_epoch"],
	"bgm_stop": ["kind", "command"],
	"native_audio_control": ["kind", "command"],
	"native_stop_xa_if_active": ["kind", "epoch"],
}


func _schema(event: Dictionary) -> String:
	if not event.get("kind") is String or not INTENT_FIELDS.has(event["kind"]):
		return "unsupported opening audio intent kind"
	var fields: Array = INTENT_FIELDS[event["kind"]]
	if event.size() != fields.size() or not event.has_all(fields):
		return "opening audio intent fields differ from its supported schema"
	for key: String in ["voice_id", "speaker", "epoch", "line_id", "clip_index", "command", "bank_epoch", "request_serial", "session_generation"]:
		if event.has(key) and not event[key] is int:
			return "opening audio field requires an exact integer: " + key
	for key: String in ["request_key", "role"]:
		if event.has(key) and (not event[key] is String or not audio._matches(AudioService.ID_PATTERN, event[key])):
			return "opening audio field requires a bounded identity: " + key
	if event.has("xa_request") and not event["xa_request"] is Dictionary:
		return "opening audio XA request must be a dictionary"
	if event.has("xa_request"):
		var request: Dictionary = event["xa_request"]
		var request_fields: Array[String] = ["source_sha256", "source_lba", "start_lba", "end_lba", "file_number", "channel", "native_evidence"]
		if request.size() != request_fields.size() or not request.has_all(request_fields):
			return "opening XA request fields differ from source descriptor schema"
		if not request["source_sha256"] is String or not audio._matches(AudioService.HASH_PATTERN, request["source_sha256"]) or not request["native_evidence"] is String or request["native_evidence"].is_empty() or request["native_evidence"].length() > 4096:
			return "opening XA request lacks bounded source provenance"
		for key: String in ["source_lba", "start_lba", "end_lba", "file_number", "channel"]:
			var value: Variant = request[key]
			# JSON resource descriptors retain integral floats. Validate before any
			# native-request equality; heterogeneous Variant comparison can throw.
			if not (value is int or value is float) or not is_finite(float(value)) or float(value) != floor(float(value)) or value < 0 or value > (255 if key in ["channel", "file_number"] else 0xffffffff):
				return "opening XA request contains an invalid native integer: " + key
	if event.has("epoch") and event["epoch"] < 1 or event.has("bank_epoch") and event["bank_epoch"] < 1:
		return "opening audio ownership epoch is invalid"
	for key: String in ["bank_epoch", "request_serial", "session_generation"]:
		if event.has(key) and (event[key] < 1 or event[key] > 0x7fffffff): return "opening audio ownership integer is out of range: " + key
	return ""


func bind(service: AudioService, voice_player: AudioStreamPlayer, bgm_player: AudioStreamPlayer = null) -> String:
	if not is_instance_valid(voice_player):
		return "original voice player is unavailable"
	if is_instance_valid(_voice_player) and (_voice_player != voice_player or audio != service) and (not _active_request.is_empty() or not _pending.is_empty()):
		return "active session audio ownership cannot be rebound to another player/service"
	if is_instance_valid(_voice_player) and _voice_player != voice_player and _voice_player.finished.is_connected(_finished):
		_voice_player.finished.disconnect(_finished)
	audio = service
	_voice_player = voice_player
	_bgm_player = bgm_player
	voice.attach(voice_player)
	if not voice_player.finished.is_connected(_finished):
		voice_player.finished.connect(_finished)
	return ""


func reset_session() -> void:
	# Explicit fresh-session operation. bind() alone preserves a live session.
	voice.detach()
	if is_instance_valid(_voice_player) and _voice_player.finished.is_connected(_finished):
		_voice_player.finished.disconnect(_finished)
	if is_instance_valid(_bgm_player):
		_bgm_player.stop()
	_pending.clear()
	_active_request.clear()
	_completion_index = 0
	_voice_player = null
	_bgm_player = null
	if audio != null:
		audio.reset_session_observations()


func preflight(result: Session.FrameResult, state: RefCounted = null) -> String:
	if audio == null:
		return "original audio bridge is not prepared"
	var observations_error: String = _validate_observations(result)
	if not observations_error.is_empty():
		return observations_error
	var voices: int = 0
	for raw: Variant in result.audio:
		if not raw is Dictionary:
			return "malformed original audio intent"
		var error: String = _schema(raw)
		if not error.is_empty():
			return error
		if state == null:
			return "opening audio preflight requires current transactional state"
		error = audio.preflight_original_event(raw)
		if not error.is_empty():
			return error
		match raw["kind"]:
			"original_bgm_start":
				error = audio.original_bgm_service.preflight_event(raw,state)
				if not error.is_empty(): return error
			"bank_load":
				var bank: Dictionary = state.opening.get("audio_bank", {})
				if raw["bank_epoch"] != bank.get("epoch") or raw["role"] != bank.get("role") or bank.get("ready") != false:
					return "bank intent differs from transactional ownership"
				var prepared: Dictionary = audio.prepared_bank_roles[raw["role"]]
				if not prepared.get("memory") is Dictionary:
					return "prepared bank has no software sound RAM checkpoint"
				var probe = AudioService.VabMemory.new()
				error = probe.restore(prepared["memory"])
				if not error.is_empty():
					return error
			"native_stop_xa_if_active":
				var owner: Dictionary = state.opening.get("audio", {})
				if raw["epoch"] != owner.get("epoch") or owner.get("status") != "cancelled" or owner.get("clips") != []:
					return "XA stop intent differs from cancelled transactional owner"
			"native_audio_control":
				if raw["command"] != 5 or (int(state.opening.get("audio_state_flags", 0)) & 0x100000) == 0:
					return "audio control differs from transactional native flags"
		if raw.get("kind") == "voice_play":
			if state == null:
				return "voice preflight requires current transactional state"
			var owner: Dictionary = state.opening.get("audio", {})
			for key: String in ["epoch", "line_id", "clip_index"]:
				if raw.get(key) != owner.get(key):
					return "voice intent differs from transactional ownership: " + key
			var clips: Array = owner.get("clips", [])
			if owner.get("status") != "requested" or raw["clip_index"] >= clips.size() or clips[raw["clip_index"]].get("request_key") != raw.get("request_key"):
				return "voice intent has no matching requested transactional clip"
			if raw["speaker"] != clips[raw["clip_index"]].get("speaker"):
				return "voice speaker differs from transactional clip"
			voices += 1
			if voices > 1 or not is_instance_valid(_voice_player) or not _voice_player.is_inside_tree():
				return "original voice requires one prepared player request per committed frame"
	return ""


func _validate_observations(result: Session.FrameResult) -> String:
	var index: int = 0
	for entry: Dictionary in result.trace:
		if entry.get("kind") != Session.ServiceKind.COMPLETION or entry.get("service") not in ["opening.audio", "opening.audio.bank"]:
			continue
		if not entry.get("payload") is Dictionary or index >= _pending.size() or _pending[index]["service"] != entry["service"] or _pending[index]["payload"] != entry["payload"]:
			return "audio observation differs from queued producer event"
		index += 1
	return ""


func pending_events(logical_tick: int) -> Array[Session.ServiceEvent]:
	var events: Array[Session.ServiceEvent] = []
	for descriptor: Dictionary in _pending:
		events.append(Session.ServiceEvent.new(logical_tick, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, descriptor["service"], descriptor["payload"]))
	return events


func _observe(status: String) -> void:
	_completion_index += 1
	var payload: Dictionary = {}
	for key: String in ["epoch", "line_id", "clip_index", "request_key"]:
		payload[key] = _active_request[key]
	payload["status"] = status
	payload["completion_index"] = _completion_index
	_pending.append({"service": "opening.audio", "payload": payload})


func _finished() -> void:
	if _active_request.is_empty():
		return
	_observe("finished")
	_active_request.clear()


func publish(result: Session.FrameResult, logical_tick: int) -> String:
	if not result.committed:
		return "aborted frame cannot publish original audio effects"
	if audio == null:
		return "original audio bridge is not prepared"
	var error: String = ""
	# State ownership was checked within Session AUDIO before commit. Recheck
	# immutable content here; this publication layer must never mutate Session.
	for raw: Variant in result.audio:
		if not raw is Dictionary:
			return "malformed committed audio intent"
		error = _schema(raw)
		if not error.is_empty():
			return error
		error = audio.preflight_original_event(raw)
		if not error.is_empty():
			return error
	# Failed steps retain these exact observations for a deterministic retry.
	error = _validate_observations(result)
	if not error.is_empty():
		return error
	var acknowledge: int = 0
	for entry: Dictionary in result.trace:
		if entry.get("kind") != Session.ServiceKind.COMPLETION or str(entry.get("service", "")) not in ["opening.audio", "opening.audio.bank"]:
			continue
		acknowledge += 1
	for _i: int in range(acknowledge):
		_pending.pop_front()
	for event: Dictionary in result.audio:
		match event["kind"]:
			"original_bgm_start":
				pass # Physical activation is the first synchronous publisher.
			"voice_play":
				_active_request = event.duplicate(true)
				error = voice.play_event(audio, event)
				if not error.is_empty():
					return error
				_observe("started")
			"bank_load":
				error = audio.publish_prepared_bank(str(event["role"]))
				if not error.is_empty():
					return error
				if audio.original_bgm_service != null:
					error = audio.original_bgm_service.bank_published(event["role"],event["bank_epoch"])
					if not error.is_empty(): return error
				_pending.append({"service": "opening.audio.bank", "payload": {
					"bank_epoch": event["bank_epoch"], "role": event["role"], "ready": true}})
			"bgm_stop":
				if _bgm_player != null:
					_bgm_player.stop()
			"native_stop_xa_if_active":
				voice.stop_clip()
				_active_request.clear()
				_pending = _pending.filter(func(value: Dictionary) -> bool: return value["service"] != "opening.audio")
			"native_audio_control":
				pass
			_:
				return "unsupported opening audio publication kind"
		audio.record_committed_original_event(event, logical_tick)
	return ""
