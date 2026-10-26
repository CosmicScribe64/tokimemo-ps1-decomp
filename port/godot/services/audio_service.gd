extends RefCounted
## T-29006 / T-29012 ordered audio service boundary and software VAB memory bridge.
## Story/opening rules consume and emit events explicitly in declared session phases.

const VabMemory = preload("res://services/native_vab_memory.gd")
const BankLoader = preload("res://services/native_bank_loader.gd")

enum Phase { BEFORE_MODE, DRAW_BEFORE_INPUT, CD, AUDIO }

const MAX_QUEUED_EVENTS: int = 4096
const MAX_CLIP_BYTES: int = 64 * 1024 * 1024
const WAV_HEADER_MIN_BYTES: int = 44
const MIN_SAMPLE_RATE: int = 8000
const MAX_SAMPLE_RATE: int = 192000
const MIN_CHANNELS: int = 1
const MAX_CHANNELS: int = 2
const BYTES_PER_SAMPLE_16: int = 2
const MUTED_VOLUME_DB: float = -80.0
const MAX_PAYLOAD_DEPTH: int = 16
const MAX_STRING_BYTES: int = 4096
const MAX_KEY_BYTES: int = 128
const SE_SLOT_INDEX: int = 0
const MUSIC_SLOT_INDEX: int = 1
const SE_SLOT_SPU_ADDRESS: int = 0x1010
const SE_SLOT_SPU_CAPACITY: int = 0x10000
const MUSIC_SLOT_SPU_ADDRESS: int = 0x11010
const MUSIC_SLOT_SPU_CAPACITY: int = 0x28000
const ID_PATTERN: String = "^[A-Za-z0-9_.-]{1,96}$"
const HASH_PATTERN: String = "^[0-9a-f]{64}$"
const ALLOWED_KINDS: Array[String] = [
	"start",
	"stop",
	"pause",
	"resume",
	"mute",
	"finished",
	"error",
	"bank_load",
	"bgm_play",
	"bgm_stop",
	"se_play",
	"voice_play",
]
const NATIVE_EVENT_KINDS: Array[String] = [
	"bank_load",
	"bgm_play",
	"bgm_stop",
	"se_play",
	"voice_play",
]

var queue: Array[Dictionary] = []
var active: Dictionary = {}
var registered_pcm: Dictionary = {}
var muted: bool = false
var vab_memory: VabMemory = VabMemory.new()
var bank_loader: BankLoader = BankLoader.new()
var opening_pack: Dictionary = {}
var opening_pack_root: String = ""
var witness_header_digests: Dictionary = {}
var active_bank_role: String = ""
var bank_ready: Dictionary = {}
var bgm_state: Dictionary = {
	"playing": false,
	"role": "",
	"bank": -1,
	"sequence": -1,
}
var se_log: Array[Dictionary] = []
var voice_log: Array[Dictionary] = []
var event_history: Array[Dictionary] = []
var _sequence: int = 0
var _current_tick: int = -1
var _player: AudioStreamPlayer
var _base_volume_db: float = 0.0


func checkpoint() -> Dictionary:
	return {
		"queue": queue.duplicate(true),
		"active": _checkpoint_active(),
		"muted": muted,
		"vab_memory": vab_memory.checkpoint(),
		"active_bank_role": active_bank_role,
		"bank_ready": bank_ready.duplicate(true),
		"bgm_state": bgm_state.duplicate(true),
		"se_log": se_log.duplicate(true),
		"voice_log": voice_log.duplicate(true),
		"event_history": event_history.duplicate(true),
		"sequence": _sequence,
		"current_tick": _current_tick,
	}


func _checkpoint_active() -> Dictionary:
	var copy: Dictionary = {}
	for key: Variant in active:
		var key_str: String = str(key)
		var entry: Dictionary = active[key_str]
		var entry_copy: Dictionary = {}
		for field: Variant in entry:
			var field_str: String = str(field)
			if field_str != "stream":
				entry_copy[field_str] = entry[field_str]
		copy[key_str] = entry_copy
	return copy


func restore(snapshot: Dictionary) -> String:
	if not snapshot.get("queue") is Array or not snapshot.get("vab_memory") is Dictionary:
		return "invalid audio service checkpoint"
	var mem_err: String = vab_memory.restore(snapshot["vab_memory"])
	if not mem_err.is_empty():
		return mem_err
	var restored_queue: Array[Dictionary] = []
	for item: Variant in snapshot["queue"]:
		if item is Dictionary:
			var item_dict: Dictionary = item
			restored_queue.append(item_dict.duplicate(true))
	queue = restored_queue
	active.clear()
	var saved_active: Dictionary = snapshot.get("active", {})
	for key: Variant in saved_active:
		var key_str: String = str(key)
		var item_entry: Dictionary = saved_active[key_str].duplicate(true)
		if registered_pcm.has(key_str):
			item_entry["stream"] = registered_pcm[key_str]["stream"]
		active[key_str] = item_entry
	muted = bool(snapshot.get("muted", false))
	active_bank_role = str(snapshot.get("active_bank_role", ""))
	bank_ready = snapshot.get("bank_ready", {}).duplicate(true)
	bgm_state = snapshot.get("bgm_state", {"playing": false, "role": "", "bank": -1, "sequence": -1}).duplicate(true)
	var restored_se: Array[Dictionary] = []
	for se_item: Variant in snapshot.get("se_log", []):
		if se_item is Dictionary:
			var se_dict: Dictionary = se_item
			restored_se.append(se_dict.duplicate(true))
	se_log = restored_se
	var restored_voice: Array[Dictionary] = []
	for voice_item: Variant in snapshot.get("voice_log", []):
		if voice_item is Dictionary:
			var voice_dict: Dictionary = voice_item
			restored_voice.append(voice_dict.duplicate(true))
	voice_log = restored_voice
	var restored_history: Array[Dictionary] = []
	for hist_item: Variant in snapshot.get("event_history", []):
		if hist_item is Dictionary:
			var hist_dict: Dictionary = hist_item
			restored_history.append(hist_dict.duplicate(true))
	event_history = restored_history
	_sequence = int(snapshot.get("sequence", 0))
	_current_tick = int(snapshot.get("current_tick", -1))
	return ""


func configure_opening_pack(pack_root: String, witness_receipt_path: String = "") -> String:
	if pack_root.is_empty() or not pack_root.is_absolute_path() or pack_root.contains(".."):
		return "opening pack root must be a normalized absolute path"
	var opening_json_path: String = pack_root.path_join("opening.json")
	if not FileAccess.file_exists(opening_json_path):
		return "opening.json is missing from opening pack root"
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(opening_json_path))
	if not parsed is Dictionary or not parsed.get("bank_roles") is Dictionary:
		return "opening.json lacks bank_roles dictionary"
	var pack_dict: Dictionary = parsed
	for required_role: String in ["prologue_arrival", "normal_gameplay"]:
		if not pack_dict["bank_roles"].has(required_role):
			return "opening.json missing required bank role: " + required_role
	opening_pack = pack_dict
	opening_pack_root = pack_root
	witness_header_digests.clear()
	if not witness_receipt_path.is_empty():
		if not FileAccess.file_exists(witness_receipt_path):
			return "witness receipt file does not exist: " + witness_receipt_path
		var receipt_parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(witness_receipt_path))
		if not receipt_parsed is Dictionary or not receipt_parsed.get("files") is Dictionary:
			return "witness receipt lacks files dictionary"
		var receipt_dict: Dictionary = receipt_parsed
		var receipt_files: Dictionary = receipt_dict["files"]
		for dump_file: String in [
			"prologue_arrival-slot0-done-se-header.bin",
			"prologue_arrival-slot1-done-music-header.bin",
			"normal_gameplay-slot0-done-se-header.bin",
			"normal_gameplay-slot1-done-music-header.bin",
		]:
			if receipt_files.has(dump_file):
				witness_header_digests[dump_file] = str(receipt_files[dump_file])
	return ""


func install_bank_role(role: String) -> Dictionary:
	if role.is_empty() or not _matches(ID_PATTERN, role):
		return _failure("bank role requires a safe identifier")
	if not opening_pack.is_empty() and opening_pack.get("bank_roles", {}).has(role):
		var role_record: Dictionary = opening_pack["bank_roles"][role]
		var assets: Dictionary = role_record.get("assets", {})
		var se_h: Dictionary = assets.get("se_header", {})
		var se_b: Dictionary = assets.get("se_body", {})
		var mu_h: Dictionary = assets.get("music_header", {})
		var mu_b: Dictionary = assets.get("music_body", {})
		var reads_dir: String = opening_pack_root.path_join("reads")
		var se_header_bytes: PackedByteArray = FileAccess.get_file_as_bytes(reads_dir.path_join(role + "-se_header.bin")).slice(0, int(se_h.get("size", 0)))
		var se_body_bytes: PackedByteArray = FileAccess.get_file_as_bytes(reads_dir.path_join(role + "-se_body.bin")).slice(0, int(se_b.get("size", 0)))
		var mu_header_bytes: PackedByteArray = FileAccess.get_file_as_bytes(reads_dir.path_join(role + "-music_header.bin")).slice(0, int(mu_h.get("size", 0)))
		var mu_body_bytes: PackedByteArray = FileAccess.get_file_as_bytes(reads_dir.path_join(role + "-music_body.bin")).slice(0, int(mu_b.get("size", 0)))
		var before_mem: Dictionary = vab_memory.checkpoint()
		var out0: VabMemory.Outcome = vab_memory.install(
			SE_SLOT_INDEX,
			SE_SLOT_SPU_ADDRESS,
			SE_SLOT_SPU_CAPACITY,
			se_header_bytes,
			se_body_bytes,
			{"header_sha256": str(se_h.get("sha256", "")), "body_sha256": str(se_b.get("sha256", ""))}
		)
		if not out0.error.is_empty() or not out0.installed:
			vab_memory.restore(before_mem)
			return _failure("SE bank install failed for " + role + ": " + out0.error)
		var out1: VabMemory.Outcome = vab_memory.install(
			MUSIC_SLOT_INDEX,
			MUSIC_SLOT_SPU_ADDRESS,
			MUSIC_SLOT_SPU_CAPACITY,
			mu_header_bytes,
			mu_body_bytes,
			{"header_sha256": str(mu_h.get("sha256", "")), "body_sha256": str(mu_b.get("sha256", ""))}
		)
		if not out1.error.is_empty() or not out1.installed:
			vab_memory.restore(before_mem)
			return _failure("music bank install failed for " + role + ": " + out1.error)
		var slot0_sha: String = str(vab_memory.banks[SE_SLOT_INDEX]["header_sha256"])
		var slot1_sha: String = str(vab_memory.banks[MUSIC_SLOT_INDEX]["header_sha256"])
		var witness0_key: String = role + "-slot0-done-se-header.bin"
		var witness1_key: String = role + "-slot1-done-music-header.bin"
		if witness_header_digests.has(witness0_key) and witness_header_digests[witness0_key] != slot0_sha:
			vab_memory.restore(before_mem)
			return _failure("mutated SE VAB header SHA256 differs from hardware witness for " + role)
		if witness_header_digests.has(witness1_key) and witness_header_digests[witness1_key] != slot1_sha:
			vab_memory.restore(before_mem)
			return _failure("mutated music VAB header SHA256 differs from hardware witness for " + role)
		active_bank_role = role
		bank_ready[role] = true
		return {
			"error": "",
			"role": role,
			"slot0_sample_bytes": out0.sample_bytes,
			"slot1_sample_bytes": out1.sample_bytes,
			"slot0_header_sha256": slot0_sha,
			"slot1_header_sha256": slot1_sha,
		}
	active_bank_role = role
	bank_ready[role] = true
	return {
		"error": "",
		"role": role,
		"slot0_sample_bytes": int(vab_memory.banks.get(SE_SLOT_INDEX, {}).get("sample_bytes", 0)),
		"slot1_sample_bytes": int(vab_memory.banks.get(MUSIC_SLOT_INDEX, {}).get("sample_bytes", 0)),
		"slot0_header_sha256": str(vab_memory.banks.get(SE_SLOT_INDEX, {}).get("header_sha256", "")),
		"slot1_header_sha256": str(vab_memory.banks.get(MUSIC_SLOT_INDEX, {}).get("header_sha256", "")),
	}


func register_pcm_clip(descriptor: Dictionary, approved_pack_root: String) -> Dictionary:
	var required: Array[String] = ["clip_id", "path", "sha256", "byte_size", "sample_rate", "channels", "format"]
	for field: String in required:
		if not descriptor.has(field):
			return _failure("PCM descriptor is missing " + field)
	if not _matches(ID_PATTERN, descriptor["clip_id"]) or not (descriptor["clip_id"] is String):
		return _failure("unsafe PCM clip identifier")
	if not (descriptor["path"] is String) or not _safe_relative_path(descriptor["path"]):
		return _failure("PCM clip path must be a safe relative path")
	if not (descriptor["sha256"] is String) or not _matches(HASH_PATTERN, descriptor["sha256"]):
		return _failure("PCM clip SHA256 is malformed")
	if not (descriptor["byte_size"] is int) or descriptor["byte_size"] <= WAV_HEADER_MIN_BYTES or descriptor["byte_size"] > MAX_CLIP_BYTES:
		return _failure("PCM clip size is outside the supported bounds")
	if not (descriptor["sample_rate"] is int) or descriptor["sample_rate"] < MIN_SAMPLE_RATE or descriptor["sample_rate"] > MAX_SAMPLE_RATE:
		return _failure("PCM sample rate is outside the supported bounds")
	if not (descriptor["channels"] is int) or descriptor["channels"] < MIN_CHANNELS or descriptor["channels"] > MAX_CHANNELS:
		return _failure("PCM channel count must be mono or stereo")
	if descriptor["format"] != "pcm16":
		return _failure("only explicitly validated 16-bit PCM WAV clips are supported")
	if not approved_pack_root.is_absolute_path() or approved_pack_root.contains(".."):
		return _failure("approved PCM pack root must be a normalized absolute path")
	var root: String = approved_pack_root.simplify_path().trim_suffix("/")
	var rel_path: String = descriptor["path"]
	var absolute_path: String = root.path_join(rel_path).simplify_path()
	if not absolute_path.begins_with(root + "/"):
		return _failure("PCM clip path escaped the approved pack root")
	if _has_symlink_component(root) or _has_symlink_component(absolute_path):
		return _failure("PCM clip path contains a symbolic link or reparse point")
	var file: FileAccess = FileAccess.open(absolute_path, FileAccess.READ)
	if file == null:
		return _failure("validated PCM clip file is unavailable")
	var expected_size: int = descriptor["byte_size"]
	if file.get_length() != expected_size:
		file.close()
		return _failure("PCM clip size does not match its validated descriptor")
	var bytes: PackedByteArray = file.get_buffer(file.get_length())
	file.close()
	var expected_sha: String = descriptor["sha256"]
	if bytes.size() != expected_size or _sha256(bytes) != expected_sha:
		return _failure("PCM clip bytes do not match their validated descriptor")
	var stream: AudioStreamWAV = AudioStreamWAV.load_from_file(absolute_path)
	if stream == null:
		return _failure("external file is not a readable WAV stream")
	var expected_rate: int = descriptor["sample_rate"]
	if stream.format != AudioStreamWAV.FORMAT_16_BITS or stream.mix_rate != expected_rate:
		return _failure("WAV sample format or rate does not match the descriptor")
	var expected_channels: int = descriptor["channels"]
	if int(stream.stereo) + 1 != expected_channels:
		return _failure("WAV channel count does not match the descriptor")
	if stream.data.is_empty() or stream.data.size() % (expected_channels * BYTES_PER_SAMPLE_16) != 0:
		return _failure("WAV sample data is empty or not frame-aligned")
	var clip_id: String = descriptor["clip_id"]
	registered_pcm[clip_id] = {"stream": stream, "descriptor": descriptor.duplicate(true)}
	return {"error": "", "clip_id": clip_id, "capability": "validated_pcm_wav"}


static func create_pcm_wav_stream(kind: String, id_or_seq: int, duration_ms: int = 180) -> AudioStreamWAV:
	var sample_rate: int = 22050
	var clamped_ms: int = maxi(20, duration_ms)
	var sample_count: int = maxi(1, int((sample_rate * clamped_ms) / 1000))
	var pcm_bytes: PackedByteArray = PackedByteArray()
	pcm_bytes.resize(sample_count * BYTES_PER_SAMPLE_16)

	var triads: Array = [
		[261.63, 329.63, 392.00, 523.25], # C4 major triad + C5
		[349.23, 440.00, 523.25, 698.46], # F4 major triad + F5
		[392.00, 493.88, 587.33, 783.99], # G4 major triad + G5
		[293.66, 369.99, 440.00, 587.33], # D4 major triad + D5
	]
	var vowels: Array = [
		[730.0, 1090.0], # /a/
		[270.0, 2290.0], # /i/
		[300.0, 870.0],  # /u/
		[530.0, 1840.0], # /e/
		[570.0, 840.0],  # /o/
	]

	for i: int in range(sample_count):
		var t: float = float(i) / float(sample_rate)
		var norm: float = float(i) / float(sample_count)
		var env: float = 1.0
		if norm < 0.05:
			env = norm / 0.05
		elif norm > 0.85:
			env = (1.0 - norm) / 0.15
		var sample_val: float = 0.0

		match kind:
			"bgm_play", "native_restore_bgm":
				var chord: Array = triads[absi(id_or_seq) % triads.size()]
				var note_idx: int = mini(int(norm * 4.0), 3)
				var lead_freq: float = float(chord[note_idx])
				var root_freq: float = float(chord[0])
				var third_freq: float = float(chord[1])
				var fifth_freq: float = float(chord[2])
				sample_val = (
					0.40 * sin(TAU * lead_freq * t)
					+ 0.22 * sin(TAU * root_freq * t)
					+ 0.18 * sin(TAU * third_freq * t)
					+ 0.18 * sin(TAU * fifth_freq * t)
				) * env * 0.45
			"se_play", "native_audio_command":
				var step_semitone: int = absi(id_or_seq) & 7
				var base_freq: float = 880.0 + float(step_semitone) * 55.0
				var chime_freq: float = base_freq * (1.5 if norm >= 0.35 else 1.0)
				var decay: float = exp(-5.5 * norm)
				sample_val = (
					0.70 * sin(TAU * chime_freq * t)
					+ 0.30 * sin(TAU * (chime_freq * 2.0) * t)
				) * decay * env * 0.55
			"voice_play":
				var formants: Array = vowels[absi(id_or_seq) % vowels.size()]
				var f0: float = 220.0 + float(absi(id_or_seq) % 7) * 8.0 + 4.0 * sin(TAU * 5.5 * t)
				var f1: float = float(formants[0])
				var f2: float = float(formants[1])
				var syll_env: float = sin(PI * norm)
				sample_val = (
					0.45 * sin(TAU * f0 * t)
					+ 0.35 * sin(TAU * f1 * t)
					+ 0.20 * sin(TAU * f2 * t)
				) * syll_env * env * 0.50
			_:
				sample_val = sin(TAU * 440.0 * t) * env * 0.35

		var pcm16: int = clampi(int(round(sample_val * 32767.0)), -32768, 32767)
		var unsigned16: int = pcm16 & 0xFFFF
		pcm_bytes[i * 2] = unsigned16 & 0xFF
		pcm_bytes[i * 2 + 1] = (unsigned16 >> 8) & 0xFF

	var wav: AudioStreamWAV = AudioStreamWAV.new()
	wav.format = AudioStreamWAV.FORMAT_16_BITS
	wav.mix_rate = sample_rate
	wav.stereo = false
	if kind in ["bgm_play", "native_restore_bgm"]:
		wav.loop_mode = AudioStreamWAV.LOOP_FORWARD
		wav.loop_begin = 0
		wav.loop_end = sample_count
	else:
		wav.loop_mode = AudioStreamWAV.LOOP_DISABLED
	wav.data = pcm_bytes
	return wav



func attach_player(player: AudioStreamPlayer, base_volume_db: float = 0.0) -> String:
	if player == null:
		return "audio player is required"
	_player = player
	_base_volume_db = base_volume_db
	if not _player.finished.is_connected(_on_player_finished):
		_player.finished.connect(_on_player_finished)
	return ""


func queue_event(kind: String, tick: int, phase: int, service_id: String = "", payload: Dictionary = {}) -> String:
	if kind not in ALLOWED_KINDS:
		return "unsupported audio service event kind"
	if tick < 0 or phase < Phase.BEFORE_MODE or phase > Phase.AUDIO:
		return "audio service event needs a valid logic tick and declared service phase"
	if queue.size() >= MAX_QUEUED_EVENTS:
		return "audio service event queue is full"
	if kind == "mute":
		if not service_id.is_empty() or payload.size() != 1 or not payload.has("muted") or not (payload["muted"] is bool):
			return "mute event needs only a boolean muted field"
	elif kind in NATIVE_EVENT_KINDS:
		if not _matches(ID_PATTERN, service_id):
			return "audio service event needs a safe service ID"
		var native_err: String = _validate_native_payload(kind, payload)
		if not native_err.is_empty():
			return native_err
	else:
		if not _matches(ID_PATTERN, service_id):
			return "audio service event needs a safe service ID"
		if kind != "error" and not payload.is_empty():
			return "audio lifecycle event does not accept extra payload fields"
		if kind == "error" and (payload.size() != 1 or not payload.has("code") or not (payload["code"] is String) or not _matches(ID_PATTERN, payload["code"])):
			return "error event needs one bounded error code"
	if not _valid_event_payload(payload):
		return "audio event payload must contain bounded JSON data only"
	if kind == "start" and not registered_pcm.has(service_id):
		return "unsupported audio capability for this clip; register a validated PCM WAV first"
	if kind == "start" and _will_be_active(service_id):
		return "start event rejected while the service ID is already active"
	if kind == "start" and _player != null and _any_clip_will_be_active():
		return "single-player backend cannot overlap active PCM clips"
	if ["stop", "pause", "resume", "finished", "error"].has(kind) and not _will_be_active(service_id):
		return "audio lifecycle event references an inactive service ID"
	queue.append({
		"tick": tick,
		"phase": phase,
		"sequence": _sequence,
		"kind": kind,
		"service_id": service_id,
		"payload": payload.duplicate(true),
	})
	_sequence += 1
	queue.sort_custom(func(a: Dictionary, b: Dictionary) -> bool:
		if a["tick"] != b["tick"]:
			return a["tick"] < b["tick"]
		if a["phase"] != b["phase"]:
			return a["phase"] < b["phase"]
		return a["sequence"] < b["sequence"]
	)
	return ""


func _validate_native_payload(kind: String, payload: Dictionary) -> String:
	match kind:
		"bank_load":
			if not (payload.get("role") is String) or not _matches(ID_PATTERN, payload["role"]):
				return "bank_load event requires a safe role string"
		"bgm_play":
			if not (payload.get("bank") is int) or not (payload.get("sequence") is int):
				return "bgm_play event requires integer bank and sequence"
			var bank_idx: int = payload["bank"]
			var seq_idx: int = payload["sequence"]
			if bank_idx < 0 or bank_idx > 1 or seq_idx < 0 or seq_idx > 3:
				return "bgm_play bank or sequence out of native range"
		"bgm_stop":
			if not (payload.get("command") is int):
				return "bgm_stop event requires integer command"
		"se_play":
			if not (payload.get("command") is int):
				return "se_play event requires integer command"
		"voice_play":
			if not (payload.get("voice_id") is int):
				return "voice_play event requires integer voice_id"
	return ""


func _will_be_active(service_id: String) -> bool:
	var exists: bool = active.has(service_id)
	for event: Dictionary in queue:
		if event["service_id"] != service_id:
			continue
		match event["kind"]:
			"start":
				exists = true
			"stop", "finished", "error":
				exists = false
	return exists


func _any_clip_will_be_active() -> bool:
	if not active.is_empty():
		return true
	for event: Dictionary in queue:
		if event["kind"] == "start":
			return true
	return false


func queue_start(clip_id: String, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("start", tick, phase, clip_id)


func queue_stop(service_id: String, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("stop", tick, phase, service_id)


func queue_pause(service_id: String, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("pause", tick, phase, service_id)


func queue_resume(service_id: String, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("resume", tick, phase, service_id)


func queue_mute(value: bool, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("mute", tick, phase, "", {"muted": value})


func queue_bank_load(role: String, tick: int, phase: int = Phase.AUDIO, extra: Dictionary = {}) -> String:
	var payload: Dictionary = extra.duplicate(true)
	payload["role"] = role
	return queue_event("bank_load", tick, phase, "vab." + role, payload)


func queue_bgm_play(bank: int, sequence: int, tick: int, phase: int = Phase.AUDIO) -> String:
	var service_id: String = "bgm.%d.%d" % [bank, sequence]
	return queue_event("bgm_play", tick, phase, service_id, {"bank": bank, "sequence": sequence})


func queue_bgm_stop(command: int, tick: int, phase: int = Phase.AUDIO) -> String:
	var service_id: String = "bgm.stop.0x%x" % command
	return queue_event("bgm_stop", tick, phase, service_id, {"command": command})


func queue_se_play(command: int, tick: int, phase: int = Phase.AUDIO) -> String:
	var service_id: String = "se.0x%x" % command
	return queue_event("se_play", tick, phase, service_id, {"command": command})


func queue_voice_play(voice_id: int, tick: int, phase: int = Phase.AUDIO) -> String:
	var service_id: String = "voice.%d" % voice_id
	return queue_event("voice_play", tick, phase, service_id, {"voice_id": voice_id})


## Deterministic backend/test injection. The event is delivered only when the
## caller later drains its declared service phase.
func inject_finished(service_id: String, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("finished", tick, phase, service_id)


func inject_error(service_id: String, error_code: String, tick: int, phase: int = Phase.AUDIO) -> String:
	return queue_event("error", tick, phase, service_id, {"code": error_code})


func drain_phase(tick: int, phase: int) -> Dictionary:
	if tick < 0 or phase < Phase.BEFORE_MODE or phase > Phase.AUDIO:
		return _failure("audio service drain needs a valid logic tick and phase")
	for queued: Dictionary in queue:
		if queued["tick"] < tick or (queued["tick"] == tick and queued["phase"] < phase):
			return _failure("audio event missed its declared service phase")
	var delivered: Array[Dictionary] = []
	var remaining: Array[Dictionary] = []
	for event: Dictionary in queue:
		if event["tick"] == tick and event["phase"] == phase:
			var apply_error: String = _apply_event(event)
			if not apply_error.is_empty():
				return _failure(apply_error)
			var delivered_copy: Dictionary = event.duplicate(true)
			delivered.append(delivered_copy)
			event_history.append(delivered_copy.duplicate(true))
		else:
			remaining.append(event)
	queue = remaining
	_current_tick = tick
	return {
		"error": "",
		"events": delivered,
		"active": active.duplicate(true),
		"muted": muted,
		"active_bank_role": active_bank_role,
		"bgm_state": bgm_state.duplicate(true),
	}


func _apply_event(event: Dictionary) -> String:
	var kind: String = event["kind"]
	var service_id: String = event["service_id"]
	var payload: Dictionary = event["payload"]
	match kind:
		"start":
			var clip: Dictionary = registered_pcm[service_id]
			active[service_id] = {"state": "playing", "stream": clip["stream"], "kind": "validated_pcm_wav"}
			if _player != null:
				_player.stream = clip["stream"]
				_player.play()
		"stop":
			active.erase(service_id)
			if _player != null and _player.playing:
				_player.stop()
		"pause":
			active[service_id]["state"] = "paused"
			if _player != null:
				_player.stream_paused = true
		"resume":
			active[service_id]["state"] = "playing"
			if _player != null:
				_player.stream_paused = false
		"mute":
			muted = bool(payload["muted"])
			if _player != null:
				_player.volume_db = MUTED_VOLUME_DB if muted else _base_volume_db
		"finished", "error":
			active.erase(service_id)
			if _player != null and _player.playing:
				_player.stop()
		"bank_load":
			var role: String = str(payload["role"])
			var install_out: Dictionary = install_bank_role(role)
			if not str(install_out.get("error", "")).is_empty():
				return str(install_out["error"])
			event["payload"]["slot0_header_sha256"] = install_out["slot0_header_sha256"]
			event["payload"]["slot1_header_sha256"] = install_out["slot1_header_sha256"]
			event["payload"]["slot0_sample_bytes"] = install_out["slot0_sample_bytes"]
			event["payload"]["slot1_sample_bytes"] = install_out["slot1_sample_bytes"]
		"bgm_play":
			if active_bank_role.is_empty() or not bool(bank_ready.get(active_bank_role, false)):
				return "bgm_play requires an installed ready sound bank"
			bgm_state = {
				"playing": true,
				"role": active_bank_role,
				"bank": int(payload["bank"]),
				"sequence": int(payload["sequence"]),
			}
		"bgm_stop":
			bgm_state["playing"] = false
			bgm_state["last_stop_command"] = int(payload["command"])
		"se_play":
			if active_bank_role.is_empty() or not bool(bank_ready.get(active_bank_role, false)):
				return "se_play requires an installed ready SE bank"
			se_log.append({
				"tick": int(event["tick"]),
				"role": active_bank_role,
				"command": int(payload["command"]),
			})
		"voice_play":
			voice_log.append({
				"tick": int(event["tick"]),
				"voice_id": int(payload["voice_id"]),
			})
		_:
			return "unsupported audio service event"
	return ""


func _on_player_finished() -> void:
	if active.is_empty() or _current_tick < 0:
		return
	var service_id: String = str(active.keys()[0])
	queue_event("finished", _current_tick + 1, Phase.AUDIO, service_id)


func _valid_event_payload(value: Variant, depth: int = 0) -> bool:
	if depth > MAX_PAYLOAD_DEPTH:
		return false
	match typeof(value):
		TYPE_NIL, TYPE_BOOL, TYPE_INT, TYPE_STRING:
			return not (value is String) or value.length() <= MAX_STRING_BYTES
		TYPE_FLOAT:
			return false
		TYPE_ARRAY:
			for item: Variant in value:
				if not _valid_event_payload(item, depth + 1):
					return false
			return true
		TYPE_DICTIONARY:
			for key: Variant in value:
				if not (key is String) or key.length() > MAX_KEY_BYTES or not _valid_event_payload(value[key], depth + 1):
					return false
			return true
		_:
			return false


func _safe_relative_path(path: String) -> bool:
	if path.is_empty() or path.begins_with("/") or path.contains("\\") or path.contains(":"):
		return false
	for component: String in path.split("/"):
		if component.is_empty() or component == "." or component == "..":
			return false
	return true


func _has_symlink_component(path: String) -> bool:
	var current: String = path.simplify_path()
	while current != current.get_base_dir():
		var parent: DirAccess = DirAccess.open(current.get_base_dir())
		if parent != null and parent.is_link(current.get_file()):
			return true
		current = current.get_base_dir()
	return false


func _matches(pattern_text: String, text: Variant) -> bool:
	if not (text is String):
		return false
	var regex: RegEx = RegEx.new()
	if regex.compile(pattern_text) != OK:
		return false
	var text_str: String = text
	return regex.search(text_str) != null


func _sha256(bytes: PackedByteArray) -> String:
	var context: HashingContext = HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(bytes)
	return context.finish().hex_encode()


func _failure(reason: String) -> Dictionary:
	return {"error": reason}
