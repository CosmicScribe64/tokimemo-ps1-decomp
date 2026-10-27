extends RefCounted
## T-29021: play exact validated original XA streams, including native second clips.

var _player: AudioStreamPlayer
var _pending: Array[AudioStreamWAV] = []


func detach() -> void:
	_pending.clear()
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
	if resolved.get("streams") is Array:
		for stream: AudioStreamWAV in resolved["streams"]:
			_pending.append(stream)
	else:
		_pending.append(resolved["stream"])
	_next()
	return ""


func _next() -> void:
	if _player == null or _pending.is_empty():
		return
	_player.stream = _pending.pop_front()
	_player.play()


func stop_clip() -> void:
	_pending.clear()
	if _player != null:
		_player.stop()


func is_voice_complete() -> bool:
	return _pending.is_empty() and (_player == null or not _player.playing)
