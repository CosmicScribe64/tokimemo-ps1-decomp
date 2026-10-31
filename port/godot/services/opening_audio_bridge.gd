extends RefCounted
## T-29021 / T-29025: external effects publish only after commit.
## Voice progression is scheduled on logic frames (OpeningAudioRules.xa_tick);
## the player's start/finish signals only release the physical clip and never
## reach the Session. Bank completions still use BEFORE_MODE observations.
## Preserves first-arrival BGM guards and supports mid-prologue bank transitions
## (prologue_rei, prologue_shiori, normal_gameplay) from src/ovl/name_entry/prologue.c.

const Session = preload("res://core/game_session.gd")
const AudioService = preload("res://services/audio_service.gd")
const VoicePlayback = preload("res://services/original_voice_playback.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const Math = preload("res://core/native_math.gd")

var audio: AudioService
var voice: VoicePlayback = VoicePlayback.new()
var _voice_player: AudioStreamPlayer
var _bgm_player: AudioStreamPlayer
var _pending: Array[Dictionary] = []
var _active_request: Dictionary = {}
var _mid_prologue_bgm_state: Dictionary = {}

const INTENT_FIELDS: Dictionary = {
	"original_bgm_start": ["kind", "role", "bank_epoch", "request_serial", "session_generation"],
	"voice_play": ["kind", "voice_id", "speaker", "xa_request", "request_key", "epoch", "line_id", "clip_index"],
	"bank_load": ["kind", "role", "bank_epoch"],
	"bgm_play": ["kind", "bank", "sequence", "role", "bank_epoch"],
	"bgm_stop": ["kind", "command"],
	"se_play": ["kind", "command"],
	"native_audio_control": ["kind", "command"],
	"native_bgm_fade": ["kind", "command"],
	"native_bgm_fade_tick": ["kind", "xa_active", "tick", "gain"],
	"native_stop_xa_if_active": ["kind", "epoch"],
	# native-bank0 (first_arrival_bgm_bridge.gd owns and publishes them).
	"native_bank0_start": ["kind", "sequence", "tick_skip"],
	"native_bank0_load": ["kind"],
	"native_audio_command": ["kind", "command"],
	# Gameplay after the stage 21 handoff (flow/command_screen.gd confirm,
	# scenes/school_day_controller.gd scheduler): cd_audio_command(9, 0, 0)
	# and restore_bgm.
	"native_cd_audio_command": ["kind", "command"],
	"native_restore_bgm": ["kind", "bank", "sequence"],
}
## schedule_step_advance_day (scheduler.c:1522): `if (cd_audio_state() & 4)
## cd_audio_command(9)`, emitted by the gameplay rules without an opening
## voice epoch.
const GAMEPLAY_XA_STOP_FIELDS := ["kind"]


func _schema(event: Dictionary) -> String:
	if not event.get("kind") is String or not INTENT_FIELDS.has(event["kind"]):
		return "unsupported opening audio intent kind"
	var fields: Array = INTENT_FIELDS[event["kind"]]
	if event["kind"] == "native_stop_xa_if_active" and event.size() == GAMEPLAY_XA_STOP_FIELDS.size():
		fields = GAMEPLAY_XA_STOP_FIELDS
	if event.size() != fields.size() or not event.has_all(fields):
		return "opening audio intent fields differ from its supported schema"
	for key: String in ["voice_id", "speaker", "epoch", "line_id", "clip_index", "command", "bank", "sequence", "bank_epoch", "request_serial", "session_generation", "tick", "gain"]:
		if event.has(key) and not event[key] is int:
			return "opening audio field requires an exact integer: " + key
	if event.has("xa_active") and not event.xa_active is bool: return "fade XA ownership requires a boolean"
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
	if audio != null:
		AudioRules.prepare_mid_prologue_bank_roles(audio)
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
	_mid_prologue_bgm_state.clear()
	_voice_player = null
	_bgm_player = null
	if audio != null:
		audio.reset_session_observations()


func mid_prologue_bgm_diagnostics() -> Dictionary:
	return _mid_prologue_bgm_state.duplicate(true)


func _preflight_service_event(raw: Dictionary) -> String:
	var kind: String = str(raw.get("kind", ""))
	if kind in ["native_bgm_fade","native_bgm_fade_tick"]:
		return "" if audio.original_bgm_service != null else "Native fade has no original engine owner"
	if kind == "native_cd_audio_command":
		return "" if int(raw.get("command", -1)) == 9 else "gameplay cd_audio_command other than 9 (pause) is not modeled"
	if kind in ["native_bank0_start","native_bank0_load","native_audio_command","native_restore_bgm"]:
		return "" if audio.original_bgm_service != null else "Native bank0 operation has no original engine owner"
	if kind == "bank_load" and str(raw.get("role", "")) in ["prologue_rei", "prologue_shiori"]:
		var preparation := AudioRules.prepare_mid_prologue_bank_roles(audio)
		if not preparation.is_empty(): return preparation
	if kind == "bgm_stop" and int(raw.get("command", -1)) in [5, 0xb1, 0xc1, 0xd1]:
		return ""
	if kind == "bgm_play":
		var bank_idx: int = int(raw.get("bank", -1))
		var seq_idx: int = int(raw.get("sequence", -1))
		var role: String = str(raw.get("role", ""))
		if bank_idx < 0 or bank_idx > 1 or seq_idx < 0 or seq_idx > 3:
			return "mid-prologue BGM play bank or sequence is out of range"
		if role in ["prologue_rei", "prologue_shiori"]:
			AudioRules.prepare_mid_prologue_bank_roles(audio)
		if not audio.prepared_bank_roles.has(role):
			return "opening bank resources were not prepared before the transaction"
		return ""
	if kind == "se_play":
		return "" if int(raw.get("command", -1)) in [0x503, 0x604, 0x605] else "unsupported prologue SE command"
	return audio.preflight_original_event(raw)


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
		error = _preflight_service_event(raw)
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
			"bgm_play":
				var bank: Dictionary = state.opening.get("audio_bank", {})
				if raw["bank_epoch"] != bank.get("epoch") or raw["role"] != bank.get("role") or bank.get("ready") != true:
					return "mid-prologue BGM start differs from ready transactional bank ownership"
				var bgm_state: Dictionary = state.opening.get("prologue_bgm_state", {})
				if bgm_state.get("playing") != true or bgm_state.get("bank") != raw["bank"] or bgm_state.get("sequence") != raw["sequence"]:
					return "mid-prologue BGM start differs from transactional BGM state"
			"bgm_stop":
				if raw["command"] in [5, 0xb1, 0xc1, 0xd1]:
					var bgm_state: Dictionary = state.opening.get("prologue_bgm_state", {})
					if bgm_state.get("playing") != false or bgm_state.get("last_stop_command") != raw["command"]:
						return "BGM fade/stop command differs from transactional BGM state"
			"se_play":
				var bank: Dictionary = state.opening.get("audio_bank", {})
				if bank.get("ready") != true:
					return "SE play requires a ready transactional sound bank"
			"native_stop_xa_if_active" when raw.size() == GAMEPLAY_XA_STOP_FIELDS.size():
				if (AudioRules.check_cd(state) & 4) != 0:
					return "gameplay XA stop of a playing XA stream is not modeled"
			"native_cd_audio_command":
				# cd_command_push(9) (cd_queue.c:110): a pause queued while no XA plays
				# and the CD ring is idle has no effect on any modeled read or voice.
				if (AudioRules.check_cd(state) & 4) != 0:
					return "gameplay CD pause of a playing XA stream is not modeled"
				var media: Variant = state.opening.get("media")
				if media is Dictionary:
					for request: Variant in (media.get("requests", {}) as Dictionary).values():
						if request is Dictionary and request.get("status") == "pending":
							return "gameplay CD pause behind a pending CD read is not modeled"
					if media.get("cd_ready_frame") is int and Math.s32(int(media.cd_ready_frame) - int(state.frame_count)) > 0:
						return "gameplay CD pause behind a CD read in flight is not modeled"
			"native_stop_xa_if_active":
				var owner: Dictionary = state.opening.get("audio", {})
				if raw["epoch"] != owner.get("epoch") or owner.get("status") != "cancelled" or owner.get("clips") != []:
					return "XA stop intent differs from cancelled transactional owner"
			"native_audio_control":
				if raw["command"] != 5 or (int(state.opening.get("audio_state_flags", 0)) & 0x100000) == 0:
					return "audio control differs from transactional native flags"
			"native_bgm_fade":
				var bank0_d1: bool = raw.command == 0xd1 and state.opening.get("bank0_audio") is Dictionary
				if not (raw.command in [0xb1, 0xc1] or bank0_d1) or not state.opening.get("bgm_fade") is Dictionary: return "Native fade lacks source command ownership"
			"native_bank0_start", "native_bank0_load", "native_audio_command", "native_restore_bgm":
				if not state.opening.get("bank0_audio") is Dictionary: return "Native bank0 operation lacks its source bank0 record"
			"native_bgm_fade_tick":
				if raw.xa_active != ((AudioRules.check_cd(state)&4)!=0): return "Native fade XA flag differs from source playback"
		if raw.get("kind") == "voice_play":
			if state == null:
				return "voice preflight requires current transactional state"
			var owner: Dictionary = state.opening.get("audio", {})
			for key: String in ["epoch", "line_id", "clip_index"]:
				if raw.get(key) != owner.get(key):
					return "voice intent differs from transactional ownership: " + key
			var clips: Array = owner.get("clips", [])
			# xa_tick publishes the clip on the logic frame its playback starts.
			if owner.get("status") != "playing" or owner.get("xa_started_frame") != state.frame_count or raw["clip_index"] >= clips.size() or clips[raw["clip_index"]].get("request_key") != raw.get("request_key"):
				return "voice intent has no matching scheduled transactional clip start"
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


func _finished() -> void:
	# Physical only: the logic already owns this clip's finish frame.
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
		error = _preflight_service_event(raw)
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
			"bank_load":
				var role_name: String = str(event["role"])
				error = audio.publish_prepared_bank(role_name)
				if not error.is_empty():
					return error
				if audio.original_bgm_service != null and role_name == "prologue_arrival":
					error = audio.original_bgm_service.bank_published(role_name, event["bank_epoch"])
					if not error.is_empty(): return error
				_mid_prologue_bgm_state["published_role"] = role_name
				_mid_prologue_bgm_state["published_bank_epoch"] = int(event["bank_epoch"])
				_mid_prologue_bgm_state["contract"] = AudioRules.bank_transition_contract(role_name)
				_pending.append({"service": "opening.audio.bank", "payload": {
					"bank_epoch": event["bank_epoch"], "role": role_name, "ready": true}})
			"bgm_play":
				audio.bgm_state = {
					"playing": true,
					"role": str(event["role"]),
					"bank": int(event["bank"]),
					"sequence": int(event["sequence"]),
				}
				_mid_prologue_bgm_state["playing"] = true
				_mid_prologue_bgm_state["role"] = str(event["role"])
				_mid_prologue_bgm_state["bank"] = int(event["bank"])
				_mid_prologue_bgm_state["sequence"] = int(event["sequence"])
				_mid_prologue_bgm_state["bank_epoch"] = int(event["bank_epoch"])
			"bgm_stop":
				audio.bgm_state["playing"] = false
				audio.bgm_state["last_stop_command"] = int(event["command"])
				_mid_prologue_bgm_state["playing"] = false
				_mid_prologue_bgm_state["last_stop_command"] = int(event["command"])
				if _bgm_player != null:
					_bgm_player.stop()
			"se_play":
				audio.se_log.append({
					"tick": logical_tick,
					"role": audio.active_bank_role,
					"command": int(event["command"]),
				})
			"native_stop_xa_if_active":
				voice.stop_clip()
				_active_request.clear()
				_pending = _pending.filter(func(value: Dictionary) -> bool: return value["service"] != "opening.audio")
			"native_audio_control", "native_bgm_fade", "native_bgm_fade_tick", "native_bank0_start", "native_bank0_load", "native_audio_command", "native_restore_bgm", "native_cd_audio_command":
				pass
			_:
				return "unsupported opening audio publication kind"
		audio.record_committed_original_event(event, logical_tick)
	return ""
