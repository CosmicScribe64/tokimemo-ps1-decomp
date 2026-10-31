extends RefCounted
## FAST route test mode only: offline stand-in for res://bgm_sink.gd.
## tools/port/run_installed_route.sh --fast and run_opening_frame_timing.sh
## --fast bind this file read-only over port/godot/bgm_sink.gd (recorded as a
## source-override line); production and real-time runs never load it.
##
## Same API and limits as the real sink (7 prebuffer blocks of 735 frames,
## MAX_QUEUED_FRAMES look-ahead, generator capacity 8191). Instead of an
## AudioStreamGenerator drained by the audio driver's wall clock, an
## OfflineDevice node consumes exactly one block (735 frames, 1/60 s at
## 44100 Hz) per physics tick in which its host runs logic. That is the ideal
## 60 Hz device: the native engine is rendered one block per logic frame and
## no wall clock reaches the audio path. A drain that finds less than one
## block queued is an underrun, reported as a skip, as the generator would.
## Every pushed block (and the prebuffer, split into blocks) goes to `tap`,
## the same hook the real sink exposes for route receipts.
## Because the device drains only while its host runs physics, and the
## pre-pump refills it every tick, it cannot underrun: the observers'
## active_skips == 0 checks always pass in FAST mode, and the extra pumps in
## the observers' capture pauses are never needed. A FAST run therefore does
## not stand in for the real-time underrun and pump checks. The device does
## not drain during capture pauses on purpose: a drain paced by anything but
## logic ticks would make FAST PCM depend on the host again.

const BLOCK_FRAMES: int = 735
const PREBUFFER_BLOCKS: int = 7
const MAX_QUEUED_FRAMES: int = BLOCK_FRAMES * PREBUFFER_BLOCKS
## AudioStreamGenerator rounds buffer_length 0.16 s x 44100 up to a 8192-frame
## ring buffer; the real sink reads 8191 free frames from an empty playback.
const GENERATOR_CAPACITY: int = 8191
static var tap: Callable = Callable()
var generation: int = 1
var host: Node
var _reserved: Dictionary = {}
var _active: Dictionary = {}
var _device: Node
var _start_skips: int = 0

## One consumer per sink; it runs before the app's own physics step.
class OfflineDevice extends Node:
	var sink: RefCounted
	var ticks: int = 0
	var drained_blocks: int = 0
	var skips: int = 0
	func _init(owner_sink: RefCounted) -> void:
		sink = owner_sink
		name = "OfflineBgmDevice"
		process_physics_priority = -1000
	func _physics_process(_delta: float) -> void:
		var parent := get_parent()
		if sink == null or parent == null or not parent.is_physics_processing(): return
		ticks += 1
		sink._drain_one_block(self)

func _init(parent: Node) -> void:
	host = parent

static func _tap_blocks(pcm: PackedVector2Array, source: String) -> void:
	if not tap.is_valid(): return
	for start: int in range(0, pcm.size(), BLOCK_FRAMES):
		tap.call(pcm.slice(start, start + BLOCK_FRAMES), source)

func _host_ok() -> bool:
	return is_instance_valid(host) and not host.is_queued_for_deletion() and host.is_inside_tree()

func reserve(ticket: int, pcm: PackedVector2Array) -> String:
	if _reserved.has(ticket):
		return "duplicate sink reservation"
	if not _host_ok():
		return "sink parent is unavailable"
	if pcm.size() != MAX_QUEUED_FRAMES:
		return "exact qualified source prebuffer required"
	if GENERATOR_CAPACITY < pcm.size():
		return "actual generator rejected prebuffer"
	if not is_instance_valid(_device):
		_device = OfflineDevice.new(self)
		host.add_child(_device)
	_reserved[ticket] = {"queued": pcm.size(), "generation": generation, "capacity": GENERATOR_CAPACITY, "pcm": pcm}
	return ""

func validate(ticket: int) -> String:
	if not _reserved.has(ticket):
		return "sink reservation missing"
	if _reserved[ticket]["generation"] != generation or not _host_ok():
		return "sink reservation invalidated"
	return ""

func activate_validated(ticket: int) -> String:
	var error: String = validate(ticket)
	if not error.is_empty():
		return error
	var reservation: Dictionary = _reserved[ticket]
	_reserved.erase(ticket)
	_tap_blocks(reservation["pcm"], "prebuffer")
	reservation.erase("pcm")
	_active = reservation
	_active["paused"] = false
	_start_skips = _device.skips if is_instance_valid(_device) else 0
	return ""

func discard(ticket: int) -> void:
	_reserved.erase(ticket)

func _playing() -> bool:
	return not _active.is_empty() and _host_ok()

func can_push() -> bool:
	return _playing() and not _active["paused"] and _active["capacity"] - _active["queued"] >= BLOCK_FRAMES and _active["queued"] + BLOCK_FRAMES <= MAX_QUEUED_FRAMES

func validate_active(expected_generation: int) -> String:
	if generation != expected_generation or not _host_ok():
		return "active sink host/generation changed"
	if not _playing() or _active["paused"]:
		return "active sink player/playback changed"
	return ""

func push(pcm: PackedVector2Array) -> String:
	if pcm.size() != BLOCK_FRAMES or not can_push():
		return "sink cannot accept exact current block"
	_active["queued"] += BLOCK_FRAMES
	if tap.is_valid(): tap.call(pcm, "push")
	return ""

## OfflineDevice: one 1/60 s device period per logic tick.
func _drain_one_block(device: Node) -> void:
	if not _playing() or _active["paused"]: return
	if _active["queued"] < BLOCK_FRAMES: device.skips += 1
	_active["queued"] = maxi(0, _active["queued"] - BLOCK_FRAMES)
	device.drained_blocks += 1

func queued_frames() -> int:
	return int(_active["queued"]) if not _active.is_empty() else 0

func skips() -> int:
	return (_device.skips - _start_skips) if is_instance_valid(_device) and not _active.is_empty() else 0

func total_skips() -> int:
	return _device.skips if is_instance_valid(_device) else 0

func metrics() -> Dictionary:
	var capacity: int = int(_active.get("capacity", 0))
	return {"capacity": capacity, "available": capacity - queued_frames() if not _active.is_empty() else 0, "queued": queued_frames(), "skips": total_skips(), "output_rate": 44100.0, "stream_rate": 44100.0 if not _active.is_empty() else 0.0,
		"offline_device": {"ticks": _device.ticks, "drained_blocks": _device.drained_blocks} if is_instance_valid(_device) else {}}

func playing() -> bool:
	return _playing()

func pause(paused: bool) -> void:
	if not _active.is_empty():
		_active["paused"] = paused

func flush_queued() -> void:
	if not _active.is_empty():
		_active["queued"] = 0

func reset() -> void:
	generation += 1
	_reserved.clear()
	_active = {}
	_start_skips = 0
