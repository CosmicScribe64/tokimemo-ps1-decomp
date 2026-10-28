extends RefCounted
## T-29021 / T-29025: play exact validated original XA streams, including native
## second clips, and derive deterministic PS1 getCDlevel() envelopes (SD_CalcCDAve
## + SD_GetCDLevel + getCDlevel, src/main/scene_library.c:130-274, sound_driver.c:1059).

var _player: AudioStreamPlayer
var _pending: Array[AudioStreamWAV] = []
var _active_stream: AudioStreamWAV
var _active_levels: Array[int] = []
var _active_line_id: int = -1
var _active_speaker: int = -1
var _active_clip_index: int = 0
var _stream_levels_cache: Dictionary = {}


static func compute_stream_cd_levels(stream: AudioStreamWAV) -> Array[int]:
	if stream == null or stream.format != AudioStreamWAV.FORMAT_16_BITS:
		return []
	var data: PackedByteArray = stream.data
	var channels: int = 2 if stream.stereo else 1
	var bytes_per_sample: int = 2 * channels
	if bytes_per_sample <= 0 or data.size() < bytes_per_sample:
		return []
	var total_samples: int = int(data.size() / bytes_per_sample)
	var mix_rate: int = maxi(1, stream.mix_rate)
	var total_frames: int = maxi(1, int((total_samples * 60) / mix_rate))
	var levels: Array[int] = []
	levels.resize(total_frames)
	for vf: int in range(total_frames):
		var base_sample: int = int((vf * mix_rate) / 60)
		var left_sum: int = 0
		var right_sum: int = 0
		# SD_InitCDLevelInfo: precision_bits=8 (shift=8), sample_count=32 (step=256/32=8).
		for i: int in range(32):
			var idx: int = base_sample + i * 8
			var sl: int = 0
			var sr: int = 0
			if idx >= 0 and idx < total_samples:
				var byte_offset: int = idx * bytes_per_sample
				sl = data.decode_u16(byte_offset)
				sr = data.decode_u16(byte_offset + 2) if channels > 1 else sl
			left_sum += (~(sl - 1) & 0xffff) if (sl & 0x8000) != 0 else (sl & 0xffff)
			right_sum += (~(sr - 1) & 0xffff) if (sr & 0x8000) != 0 else (sr & 0xffff)
		var left_ave: int = int((left_sum * 2) / 32) & 0xffff
		var right_ave: int = int((right_sum * 2) / 32) & 0xffff
		var cd_l: int = (left_ave >> 8) & 0xffff
		var cd_r: int = (right_ave >> 8) & 0xffff
		levels[vf] = (cd_l + cd_r) >> 1
	return levels


func stream_cd_levels(stream: AudioStreamWAV) -> Array[int]:
	if stream == null:
		return []
	var key: int = stream.get_instance_id()
	if _stream_levels_cache.has(key):
		return _stream_levels_cache[key]
	var computed: Array[int] = compute_stream_cd_levels(stream)
	_stream_levels_cache[key] = computed
	return computed


static func sample_cd_level_at_frame(stream: AudioStreamWAV, frame_offset: int) -> int:
	var levels: Array[int] = compute_stream_cd_levels(stream)
	if levels.is_empty() or frame_offset < 0:
		return 0
	return levels[clampi(frame_offset, 0, levels.size() - 1)]


func detach() -> void:
	_pending.clear()
	_active_stream = null
	_active_levels = []
	_active_line_id = -1
	_active_speaker = -1
	_active_clip_index = 0
	if is_instance_valid(_player):
		_player.stop()
		if _player.finished.is_connected(_next):
			_player.finished.disconnect(_next)
	_player = null


func attach(player: AudioStreamPlayer) -> void:
	if _player != null and _player.finished.is_connected(_next):
		_player.finished.disconnect(_next)
	_player = player
	if not _player.finished.is_connected(_next):
		_player.finished.connect(_next)


func play_event(service: RefCounted, event: Dictionary) -> String:
	if _player == null:
		return "original voice player is not attached"
	var resolved: Dictionary = service.resolve_original_audio_event(event)
	if not str(resolved.get("error", "")).is_empty():
		return str(resolved["error"])
	stop_clip()
	_active_line_id = int(event.get("line_id", event.get("voice_id", -1)))
	_active_speaker = int(event.get("speaker", -1))
	_active_clip_index = int(event.get("clip_index", 0))
	if resolved.get("streams") is Array:
		for stream: AudioStreamWAV in resolved["streams"]:
			_pending.append(stream)
	else:
		_pending.append(resolved["stream"])
	_next()
	return ""


func _next() -> void:
	if _player == null or _pending.is_empty():
		_active_stream = null
		_active_levels = []
		return
	_active_stream = _pending.pop_front()
	_active_levels = stream_cd_levels(_active_stream)
	_player.stream = _active_stream
	_player.play()


func stop_clip() -> void:
	_pending.clear()
	_active_stream = null
	_active_levels = []
	if _player != null:
		_player.stop()


func is_voice_complete() -> bool:
	return _pending.is_empty() and (_player == null or not _player.playing)


func check_cd() -> int:
	return 0 if is_voice_complete() else 4


func get_cd_level(frame_offset: int = -1) -> int:
	if is_voice_complete() or _active_levels.is_empty():
		return 0
	var idx: int = frame_offset
	if idx < 0:
		if _player != null and _player.playing:
			idx = int(_player.get_playback_position() * 60.0)
		else:
			idx = 0
	return _active_levels[clampi(idx, 0, _active_levels.size() - 1)]
