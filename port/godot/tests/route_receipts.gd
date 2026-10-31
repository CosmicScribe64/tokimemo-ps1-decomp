extends RefCounted
## Route receipts written by the installed route observers in both real-time
## and FAST mode (tests/fast_route/fast_route.gd), so the two can be compared
## (tools/port/compare_route_modes.py):
##   /out/frames.tsv        one row per published logic frame:
##                          tick, frame_count, mode/stage/step/substep, SHA-256
##                          of the native 320x240 image (CPU-side Image data),
##                          native PCM blocks seen so far, wall-clock ms.
##   /out/audio-blocks.tsv  one row per native BGM block that reached the sink
##                          (bgm_sink.gd tap): index, prebuffer/push, last
##                          published tick, SHA-256 of the float PCM.
##   /out/route-speed.json  logic frames, wall seconds and frames per second.
##   /out/audio.wav         the same PCM as 16-bit stereo, only when
##                          ROUTE_AUDIO_WAV=1.
## The wall-clock column and route-speed.json are receipts only; nothing here
## feeds back into the app.
const Sink = preload("res://bgm_sink.gd")
const OUT := "/out"
var mode := "fast" if OS.get_environment("TOKIMEMO_FAST_ROUTE") == "1" else "realtime"
var _frames: FileAccess
var _blocks: FileAccess
var _wav: FileAccess
var _wav_frames := 0
var _pcm_blocks := 0
var _published := 0
var _first_tick := -1
var _last_tick := -1
var _first_ms := -1
var _last_ms := -1
var _app: Node
## Set by a dev resume (tests/checkpoint): the checkpoint this run started from.
var resumed_from := ""

func install() -> void:
	_frames = FileAccess.open(OUT.path_join("frames.tsv"), FileAccess.WRITE)
	_blocks = FileAccess.open(OUT.path_join("audio-blocks.tsv"), FileAccess.WRITE)
	if _frames != null: _frames.store_line("tick\tframe_count\tkey\timage_sha256\tpcm_blocks\twall_ms")
	if _blocks != null: _blocks.store_line("index\tsource\tlast_tick\tpcm_sha256")
	if OS.get_environment("ROUTE_AUDIO_WAV") == "1":
		_wav = FileAccess.open(OUT.path_join("audio.wav"), FileAccess.WRITE)
		if _wav != null: _wav.store_buffer(_wav_header(0))
	Sink.tap = _on_pcm

## A dev resume restores the saving run's counters; the speed receipt then
## measures only the frames this process publishes.
func restart_speed_window() -> void:
	_published = 0
	_first_tick = -1
	_first_ms = -1
	_last_tick = -1
	_last_ms = -1

func bind(app: Node) -> void:
	_app = app
	app.frame_published.connect(_on_frame)

func _on_frame(tick: int) -> void:
	if not is_instance_valid(_app) or _app.consumer == null or _app.consumer.opening.state == null: return
	var state = _app.consumer.opening.state
	var image: Image = _app.last_rendered_image
	var digest := ""
	if image != null and not image.is_empty():
		var hashing := HashingContext.new()
		hashing.start(HashingContext.HASH_SHA256)
		hashing.update(image.get_data())
		digest = hashing.finish().hex_encode()
	var now := Time.get_ticks_msec()
	if _first_tick < 0: _first_tick = tick; _first_ms = now
	_last_tick = tick; _last_ms = now; _published += 1
	if _frames != null:
		_frames.store_line("%d\t%d\t%02x/%d/%d/%d\t%s\t%d\t%d" % [tick, int(state.frame_count), state.game_mode, state.mode_stage, state.mode_step, state.substep, digest, _pcm_blocks, now])
	if _published % 600 == 0: write_summary()

func _on_pcm(pcm: PackedVector2Array, source: String) -> void:
	var bytes := pcm.to_byte_array()
	if _blocks != null:
		var hashing := HashingContext.new()
		hashing.start(HashingContext.HASH_SHA256)
		hashing.update(bytes)
		_blocks.store_line("%d\t%s\t%d\t%s" % [_pcm_blocks, source, _last_tick, hashing.finish().hex_encode()])
	_pcm_blocks += 1
	if _wav != null:
		var samples := PackedByteArray()
		samples.resize(pcm.size() * 4)
		for i: int in pcm.size():
			samples.encode_s16(i * 4, clampi(roundi(pcm[i].x * 32767.0), -32768, 32767))
			samples.encode_s16(i * 4 + 2, clampi(roundi(pcm[i].y * 32767.0), -32768, 32767))
		_wav.store_buffer(samples)
		_wav_frames += pcm.size()

static func _wav_header(frames: int) -> PackedByteArray:
	var header := PackedByteArray()
	header.resize(44)
	header.encode_u32(0, 0x46464952) # RIFF
	header.encode_u32(4, 36 + frames * 4)
	header.encode_u32(8, 0x45564157) # WAVE
	header.encode_u32(12, 0x20746d66) # fmt
	header.encode_u32(16, 16)
	header.encode_u16(20, 1)
	header.encode_u16(22, 2)
	header.encode_u32(24, 44100)
	header.encode_u32(28, 44100 * 4)
	header.encode_u16(32, 4)
	header.encode_u16(34, 16)
	header.encode_u32(36, 0x61746164) # data
	header.encode_u32(40, frames * 4)
	return header

func write_summary() -> void:
	if _frames != null: _frames.flush()
	if _blocks != null: _blocks.flush()
	var seconds := (_last_ms - _first_ms) / 1000.0 if _first_ms >= 0 else 0.0
	var file := FileAccess.open(OUT.path_join("route-speed.json"), FileAccess.WRITE)
	if file == null: return
	file.store_string(JSON.stringify({"mode": mode, "published_frames": _published, "first_tick": _first_tick, "last_tick": _last_tick,
		"wall_seconds": seconds, "frames_per_second": (_published - 1) / seconds if seconds > 0.0 else 0.0,
		"pcm_blocks": _pcm_blocks, "resumed_from": resumed_from, "engine": {"physics_ticks_per_second": Engine.physics_ticks_per_second, "max_fps": Engine.max_fps}}, "\t"))
	file.close()

## Called by the observer before it quits; also runs when the observer is freed.
func finish() -> void:
	if Sink.tap == _on_pcm: Sink.tap = Callable()
	write_summary()
	if _wav != null:
		_wav.seek(0)
		_wav.store_buffer(_wav_header(_wav_frames))
		_wav.close(); _wav = null
	if _frames != null: _frames.close(); _frames = null
	if _blocks != null: _blocks.close(); _blocks = null
