extends RefCounted
## One main-thread generator sink; staged players remain private and paused.

const BLOCK_FRAMES: int = 735
const PREBUFFER_BLOCKS: int = 7
const MAX_QUEUED_FRAMES: int = BLOCK_FRAMES * PREBUFFER_BLOCKS
var generation: int = 1
var host: Node
var _reserved: Dictionary = {}
var _active: AudioStreamPlayer
var _playback: AudioStreamGeneratorPlayback
var _capacity: int = 0
var _start_skips: int = 0

func _init(parent: Node) -> void:
	host = parent

func reserve(ticket: int, pcm: PackedVector2Array) -> String:
	if _reserved.has(ticket):
		return "duplicate sink reservation"
	if not is_instance_valid(host) or host.is_queued_for_deletion() or not host.is_inside_tree():
		return "sink parent is unavailable"
	if pcm.size() != MAX_QUEUED_FRAMES:
		return "exact qualified source prebuffer required"
	var stream: AudioStreamGenerator = AudioStreamGenerator.new()
	stream.mix_rate = 44100.0
	# Linux dummy-driver qualification pulls 4096 frames in one burst.
	# Keep one burst plus a callback of slack; this is a bounded 116.7 ms
	# source queue, not target macOS latency acceptance.
	stream.buffer_length = 0.16
	var player: AudioStreamPlayer = AudioStreamPlayer.new()
	player.stream = stream
	host.add_child(player)
	player.stream_paused = true
	player.play()
	player.stream_paused = true
	var playback: AudioStreamGeneratorPlayback = player.get_stream_playback() as AudioStreamGeneratorPlayback
	if playback == null:
		player.queue_free()
		return "actual generator playback unavailable"
	var capacity: int = playback.get_frames_available()
	if capacity < pcm.size() or not playback.push_buffer(pcm):
		player.stop()
		player.queue_free()
		return "actual generator rejected prebuffer"
	_reserved[ticket] = {"player": player, "playback": playback, "generation": generation, "capacity": capacity}
	player.tree_exiting.connect(_reservation_exiting.bind(ticket))
	return ""

func _reservation_exiting(ticket: int) -> void:
	# Parent/player disposal can happen outside this owner. Stop the paused
	# reservation while the node is still valid, then invalidate its token.
	if not _reserved.has(ticket):
		return
	var raw_player: Variant = _reserved[ticket].get("player")
	if is_instance_valid(raw_player) and raw_player is AudioStreamPlayer:
		raw_player.stop()
		raw_player.stream = null
	_reserved.erase(ticket)

func validate(ticket: int) -> String:
	if not _reserved.has(ticket):
		return "sink reservation missing"
	var reservation: Dictionary = _reserved[ticket]
	var raw_player: Variant = reservation.get("player")
	if not is_instance_valid(raw_player) or not raw_player is AudioStreamPlayer:
		return "sink reservation player was freed"
	var player: AudioStreamPlayer = raw_player
	# Godot's playing getter is false while paused; the reserved playback
	# identity and paused state are the precommit ownership evidence.
	if reservation["generation"] != generation or not is_instance_valid(host) or host.is_queued_for_deletion() or not host.is_inside_tree() or player.is_queued_for_deletion() or not player.is_inside_tree() or not player.stream_paused or player.get_stream_playback() != reservation["playback"]:
		return "sink reservation invalidated"
	return ""

func activate_validated(ticket: int) -> String:
	# Caller validates immediately before this non-yielding main-thread operation.
	var error: String = validate(ticket)
	if not error.is_empty():
		return error
	var reservation: Dictionary = _reserved[ticket]
	var previous: Variant = _active
	_active = reservation["player"]
	_playback = reservation["playback"]
	_capacity = reservation["capacity"]
	_start_skips = _playback.get_skips()
	_reserved.erase(ticket)
	_active.stream_paused = false
	if is_instance_valid(previous):
		previous.stop()
		previous.queue_free()
	return ""

func discard(ticket: int) -> void:
	if not _reserved.has(ticket):
		return
	var raw_player: Variant = _reserved[ticket].get("player")
	if is_instance_valid(raw_player) and raw_player is AudioStreamPlayer:
		var player: AudioStreamPlayer = raw_player
		player.stop()
		player.stream = null
		player.queue_free()
	_reserved.erase(ticket)

func can_push() -> bool:
	return is_instance_valid(_active) and _active.playing and not _active.stream_paused and _playback != null and _playback.get_frames_available() >= BLOCK_FRAMES and queued_frames() + BLOCK_FRAMES <= MAX_QUEUED_FRAMES

func push(pcm: PackedVector2Array) -> String:
	if pcm.size() != BLOCK_FRAMES or not can_push():
		return "sink cannot accept exact current block"
	return "" if _playback.push_buffer(pcm) else "actual generator push rejected"

func queued_frames() -> int:
	return _capacity - _playback.get_frames_available() if _playback != null else 0

func skips() -> int:
	return _playback.get_skips() - _start_skips if _playback != null else 0

func total_skips() -> int:
	return _playback.get_skips() if _playback != null else 0

func metrics() -> Dictionary:
	return {"capacity": _capacity, "available": _playback.get_frames_available() if _playback != null else 0, "queued": queued_frames(), "skips": total_skips(), "output_rate": AudioServer.get_mix_rate(), "stream_rate": (_active.stream as AudioStreamGenerator).mix_rate if is_instance_valid(_active) else 0.0}

func playing() -> bool:
	return is_instance_valid(_active) and _active.playing

func pause(paused: bool) -> void:
	if is_instance_valid(_active):
		_active.stream_paused = paused

func flush_queued() -> void:
	if _playback != null:
		_playback.clear_buffer()

func reset() -> void:
	generation += 1
	for raw_ticket: Variant in _reserved.keys():
		discard(int(raw_ticket))
	if is_instance_valid(_active):
		_active.stop()
		_active.queue_free()
	_active = null
	_playback = null
	_capacity = 0
	_start_skips = 0
