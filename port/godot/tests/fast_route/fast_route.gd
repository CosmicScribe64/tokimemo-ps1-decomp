extends Node
## FAST route test mode (TOKIMEMO_FAST_ROUTE=1, set only by
## tools/port/run_installed_route.sh --fast and run_opening_frame_timing.sh
## --fast). Installed by the observer base (tests/run_installed_next_arrival_boundary.gd)
## before the app boots; absent from every production and real-time run.
##
## What changes against a real-time route, and nothing else:
## - The runner starts Godot with --fixed-fps 60 --max-fps 0 --disable-vsync and
##   the dummy rendering driver on an Xvfb window: every process frame runs one
##   60 Hz physics (logic) step with delta exactly 1/60 and nothing waits for
##   the wall clock or the GPU. The native 320x240 image is still composed on
##   the CPU, hashed per frame (tests/route_receipts.gd) and saved as PNG at the
##   observers' checkpoints.
## - res://bgm_sink.gd is the offline sink (tests/fast_route/offline_bgm_sink.gd,
##   bound over it by the runner): one 735-frame block consumed per logic tick,
##   never a real-time device.
## - The physical START presses that tools/port/drive_installed_first_scene.py
##   sends with xdotool are injected here as InputEventKey through
##   Input.parse_input_event, one per new entry_N phase, with the driver's edge
##   pattern counted in 60 Hz ticks: pressed, released HOLD_TICKS ticks later
##   (the driver's 0.12 s sleep), and the next press no sooner than GAP_TICKS
##   ticks after that release (the driver's 0.05 s status poll), so every
##   press follows at least one frame with the key up. A phase published while
##   the key is down or inside the gap is queued and pressed at the first
##   allowed tick. The last press (entry_6) stays held, as the driver does.
##   The key is TOKIMEMO_TEST_START_KEY (default Return), the driver's
##   --entry-key. Only routes that use the X11 driver in real time inject them
##   (TOKIMEMO_FAST_ENTRY_KEYS=1); scripted-entry routes (frame timing,
##   command, playtest) send no key in either mode.
##   These events never pass through DisplayServerX11 (keysym translation,
##   window focus, the OS keymap), so the FAST default route does not test
##   physical key delivery: the real-time X11 route stays the required check
##   for entry input.
## - The offline sink is an ideal 60 Hz device that drains only on logic
##   ticks and so never underruns: FAST runs do not replace the real-time
##   underrun and pump checks (tests/fast_route/offline_bgm_sink.gd).
## PLAYTEST_PRESSES/PLAYTEST_COMMANDS presses already go through the app's
## input owner at exact logic frames in both modes and are not touched.
const HOLD_TICKS := 7
## The driver's poll after a release (0.05 s), in 60 Hz ticks.
const GAP_TICKS := 3
## xdotool key names that Godot spells differently.
const X11_KEY_NAMES := {"Return": "Enter"}
const LAST_ENTRY_PHASE := "entry_6"
var entry_keys := OS.get_environment("TOKIMEMO_FAST_ENTRY_KEYS") == "1"
var start_key := OS.get_environment("TOKIMEMO_TEST_START_KEY") if not OS.get_environment("TOKIMEMO_TEST_START_KEY").is_empty() else "Return"
var ticks := 0
var frames := 0
var _ticks_at_frame := 0
var error := ""
var presses: Array = []
var _seen: Dictionary = {}
var _release_at := -1
var _next_press_at := 0
var _pending: Array[String] = []

static func enabled() -> bool:
	return OS.get_environment("TOKIMEMO_FAST_ROUTE") == "1"

func _init() -> void:
	name = "FastRouteHarness"
	process_physics_priority = -2000
	process_mode = Node.PROCESS_MODE_ALWAYS

## Returns an error when the process was not started in the fast configuration.
func install(tree: SceneTree) -> String:
	Engine.max_fps = 0
	if Engine.physics_ticks_per_second != 60:
		error = "FAST route mode requires 60 physics ticks per second, got %d" % Engine.physics_ticks_per_second
	elif not load("res://bgm_sink.gd").get_script_constant_map().has("GENERATOR_CAPACITY"):
		error = "FAST route mode requires the offline sink bound over res://bgm_sink.gd"
	elif _keycode() == KEY_NONE:
		error = "TOKIMEMO_TEST_START_KEY is not a key name: " + start_key
	tree.root.add_child(self)
	return error

## --fixed-fps 60 (engine arguments are not visible to scripts) shows as
## exactly one physics step and a 1/60 s process delta in every frame; any
## other frame means the process is not in the fast configuration.
func _process(delta: float) -> void:
	frames += 1
	# Main start re-reads application/run/max_fps (60) after the observer's
	# _initialize; keep the frame rate uncapped.
	if Engine.max_fps != 0: Engine.max_fps = 0
	var steps := ticks - _ticks_at_frame
	_ticks_at_frame = ticks
	if frames > 2 and error.is_empty() and (steps != 1 or delta != 1.0 / 60.0):
		error = "FAST route frame %d ran %d physics steps with delta %f: start Godot with --fixed-fps 60" % [frames, steps, delta]
		printerr(error)
		get_tree().quit(1)

## Receipt /out/fast-route.json, written as the tree shuts down.
func _exit_tree() -> void:
	var file := FileAccess.open("/out/fast-route.json", FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(diagnostics(), "\t"))
		file.close()

func _physics_process(_delta: float) -> void:
	ticks += 1
	if _release_at >= 0 and ticks >= _release_at:
		_release_at = -1
		_key(false)
		presses[-1]["released"] = ticks
		_next_press_at = ticks + GAP_TICKS
	_press_pending()

## Mirrors drive_installed_first_scene.py: one START per new entry_N phase.
func observe_status(record: Dictionary) -> void:
	var phase := str(record.get("phase", ""))
	if not entry_keys or not phase.begins_with("entry_") or _seen.has(phase): return
	_seen[phase] = true
	_pending.append(phase)
	_press_pending()

## Presses the oldest queued phase once the key is up and the gap has passed.
func _press_pending() -> void:
	if _pending.is_empty() or _key_down() or ticks < _next_press_at: return
	var phase: String = _pending.pop_front()
	var last := phase == LAST_ENTRY_PHASE
	_key(true)
	_release_at = -1 if last else ticks + HOLD_TICKS
	presses.append({"phase": phase, "tick": ticks, "held": last})

func _key_down() -> bool:
	return _release_at >= 0 or (not presses.is_empty() and presses[-1]["held"])

func _key(pressed: bool) -> void:
	var event := InputEventKey.new()
	var code := _keycode()
	event.keycode = code
	event.physical_keycode = code
	event.key_label = code
	event.pressed = pressed
	Input.parse_input_event(event)

func _keycode() -> Key:
	return OS.find_keycode_from_string(X11_KEY_NAMES.get(start_key, start_key))

func diagnostics() -> Dictionary:
	return {"ticks": ticks, "entry_keys": entry_keys, "start_key": start_key, "presses": presses, "pending": _pending,
		"hold_ticks": HOLD_TICKS, "gap_ticks": GAP_TICKS, "error": error, "max_fps": Engine.max_fps,
		"physics_ticks_per_second": Engine.physics_ticks_per_second, "frames": frames}
