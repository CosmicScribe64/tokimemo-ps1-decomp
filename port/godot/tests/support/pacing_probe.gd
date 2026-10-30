extends Node
## T-39027: observe-only frame pacing probe. It keeps two streams:
## - ticks: one record per published logical tick (the port's nominal 60/1 clock
##   step, core/native_clock.gd) with the game frame_count and opening substep
##   key. These are the port's "vsyncs" for the cadence comparison.
## - frames: one record per presented host frame with the interval, process and
##   render time, the number of logical ticks run in that frame (catch-up), and
##   the media/audio events noted during it. These are host metrics only.
## It writes tokimemo.pacing-probe.v1, read by tools/port/frame_pacing_report.py.
##
## The probe never writes game, RNG, save or preference state. Callers feed it
## through observe() from a frame_published handler; services that want hitch
## attribution call note_event("mdec" | "str" | "xa" | "vab") and
## note_bgm_underrun(). Those service hooks are not wired in production yet
## (see wiki/playtest-pacing-checklist.md); until they are, hitches are reported
## as "unattributed".

const SCHEMA := "tokimemo.pacing-probe.v1"
const EVENT_KINDS := ["mdec", "str", "xa", "vab"]
const MAX_FRAMES := 400000

var frames: Array[Dictionary] = []
var ticks: Array[Dictionary] = []
var frame_count: int = -1
var _last_tick: int = -1
var _ticks_this_frame: int = 0
var launch_to_first_interactive_ms: Variant = null
var overflowed := false
var _last_usec := -1
var _pending_events: Array[String] = []
var _pending_underruns := 0

func _ready() -> void:
	# Run after every gameplay node of the frame so the record holds the state
	# that frame presented.
	process_priority = 1 << 20
	process_mode = Node.PROCESS_MODE_ALWAYS
	RenderingServer.viewport_set_measure_render_time(get_viewport().get_viewport_rid(), true)

## One published logical tick. key is "stage/step/substep" inside the opening
## (game_mode 0x15) and null elsewhere. A repeated tick is ignored; a tick that
## goes backwards is recorded as is, so the report rejects it.
func observe(tick: int, published_frame_count: int, published_key: Variant) -> void:
	if tick == _last_tick: return
	if ticks.size() >= MAX_FRAMES:
		overflowed = true
		return
	_last_tick = tick
	frame_count = published_frame_count
	_ticks_this_frame += 1
	ticks.append({"tick": tick, "frame_count": published_frame_count, "key": published_key})

func note_event(kind: String) -> void:
	if kind in EVENT_KINDS and not kind in _pending_events: _pending_events.append(kind)

func note_bgm_underrun() -> void:
	_pending_underruns += 1

func mark_interactive() -> void:
	if launch_to_first_interactive_ms == null: launch_to_first_interactive_ms = Time.get_ticks_msec()

func _process(_delta: float) -> void:
	if frame_count < 0: return
	if frames.size() >= MAX_FRAMES:
		overflowed = true
		return
	var now := Time.get_ticks_usec()
	var record := {"frame": frames.size(), "frame_count": frame_count, "ticks": _ticks_this_frame,
		"process_us": int(Performance.get_monitor(Performance.TIME_PROCESS) * 1000000.0),
		"render_us": int(RenderingServer.viewport_get_measured_render_time_cpu(get_viewport().get_viewport_rid()) * 1000.0
			+ RenderingServer.viewport_get_measured_render_time_gpu(get_viewport().get_viewport_rid()) * 1000.0)}
	if _last_usec >= 0: record["interval_us"] = now - _last_usec
	if not _pending_events.is_empty(): record["events"] = _pending_events.duplicate()
	if _pending_underruns > 0: record["bgm_underruns"] = _pending_underruns
	_last_usec = now
	_pending_events.clear()
	_pending_underruns = 0
	_ticks_this_frame = 0
	frames.append(record)

static func host_identity() -> Dictionary:
	var display := OS.get_environment("TOKIMEMO_HOST_DISPLAY")
	if display.is_empty(): display = DisplayServer.get_name().to_lower()
	var screen := DisplayServer.window_get_current_screen() if DisplayServer.get_name() != "headless" else 0
	return {"display": display, "display_server": DisplayServer.get_name(),
		"real_desktop": OS.get_environment("TOKIMEMO_REAL_DESKTOP") == "1" and display != "xvfb",
		"os": OS.get_name(), "renderer": RenderingServer.get_video_adapter_name(),
		"rendering_method": RenderingServer.get_current_rendering_method(),
		"refresh_rate": DisplayServer.screen_get_refresh_rate(screen) if DisplayServer.get_name() != "headless" else -1.0,
		"vsync_mode": DisplayServer.window_get_vsync_mode() if DisplayServer.get_name() != "headless" else -1,
		"engine": Engine.get_version_info().get("string", "")}

func receipt() -> Dictionary:
	var window := get_window()
	return {"schema": SCHEMA, "ticks": ticks, "frames": frames, "overflowed": overflowed,
		"probe_script_sha256": FileAccess.get_sha256(get_script().resource_path),
		"launch_to_first_interactive_ms": launch_to_first_interactive_ms,
		"host": host_identity(),
		"settings": {"max_fps": Engine.max_fps, "content_scale_aspect": window.content_scale_aspect,
			"content_scale_stretch": window.content_scale_stretch, "window_size": [window.size.x, window.size.y]}}

func write(path: String) -> bool:
	var file := FileAccess.open(path, FileAccess.WRITE)
	if file == null: return false
	file.store_string(JSON.stringify(receipt()))
	file.close()
	return true
