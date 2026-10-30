extends "res://tests/opening/run_installed_command_playtest.gd"
## T-39060 visual QA: the installed New Game playtest
## (run_installed_command_playtest.gd, unchanged presses, checks and outputs)
## with presentation preferences applied before the entry (ROUTE_PREFS,
## ROUTE_WINDOW, see route_presentation_prefs.gd) and window captures for a
## playthrough contact sheet. Presentation only: captures are taken while the
## logic is paused, so they never change a logic tick, the game state or the
## native Image.
##
## PLAYTHROUGH_CAPTURES=1 writes /out/window/<frame>-<mode key>-<reason>.png:
##   line    the first frame each opening dialogue line is fully revealed,
##   mode    the first frame of every mode/stage/step after command select,
##   every   each PLAYTHROUGH_EVERY frames (default 300),
## and /out/window/index.json (name, frame, mode key, reason, line).
## Runner: ROUTE_OBSERVER=playthrough sh tools/port/run_installed_route.sh <out> ...
const RoutePrefs = preload("res://tests/qa/route_presentation_prefs.gd")
var _captures_on := OS.get_environment("PLAYTHROUGH_CAPTURES") == "1"
var _every := int(OS.get_environment("PLAYTHROUGH_EVERY")) if OS.get_environment("PLAYTHROUGH_EVERY").is_valid_int() else 300
var _window := Vector2i.ZERO
var _capturing := false
var _lines_seen: Dictionary = {}
var _keys_seen: Dictionary = {}
var _index: Array = []

func _initialize() -> void:
	var prefs_error := RoutePrefs.apply(root)
	check(prefs_error.is_empty(), prefs_error)
	_window = RoutePrefs.window_size()
	super._initialize()

func _frame_published(tick: int, app: App) -> void:
	super._frame_published(tick, app)
	# The boots size the window from the preferences when they start, so the
	# requested size is (re)applied on published frames.
	if _window != Vector2i.ZERO and not _capturing and root.size != _window: root.size = _window
	if not _captures_on or _capturing or finishing or not app.is_physics_processing(): return
	var reason := _capture_reason(app.consumer.opening.state)
	if reason.is_empty(): return
	app.set_physics_process(false)
	_capture.call_deferred(app, reason)

## "" when this frame is not captured.
func _capture_reason(state) -> String:
	var frame := int(state.frame_count)
	var work: Variant = state.opening.get("prologue_text_work")
	if work is Dictionary and work.get("line_id") is int and int(work.get("pos", 0)) >= int(work.get("total", 1)) and not _lines_seen.has(work.line_id):
		_lines_seen[work.line_id] = frame
		return "line%02d" % int(work.line_id)
	var key := "%02x-%d-%d" % [state.game_mode, state.mode_stage, state.mode_step]
	if _select_frame >= 0 and not _keys_seen.has(key):
		_keys_seen[key] = frame
		return "mode"
	if _every > 0 and frame % _every == 0:
		return "every"
	return ""

func _capture(app: App, reason: String) -> void:
	_capturing = true
	var state = app.consumer.opening.state
	var key := "%02x-%d-%d-%d" % [state.game_mode, state.mode_stage, state.mode_step, state.substep]
	var frame := int(state.frame_count)
	for i: int in 3:
		_pump(app)
		await process_frame
	_pump(app)
	await RenderingServer.frame_post_draw
	var image := RoutePrefs.window_image(root.get_texture().get_image(), root.get_window().size)
	DirAccess.make_dir_recursive_absolute("/out/window")
	var name := "%06d-%s-%s" % [frame, key, reason]
	if image != null: image.save_png("/out/window/%s.png" % name)
	_index.append({"name": name, "frame": frame, "key": key, "reason": reason,
		"window": [root.get_window().size.x, root.get_window().size.y]})
	var file := FileAccess.open("/out/window/index.json", FileAccess.WRITE)
	file.store_string(JSON.stringify(_index, "\t")); file.close()
	_capturing = false
	if is_instance_valid(app) and not finishing: app.set_physics_process(true)

func _pump(app: App) -> void:
	if is_instance_valid(app) and app.consumer != null:
		app.bgm.pump(app.consumer.opening.state)
