extends "res://tests/opening/run_installed_command_route.gd"
## T-39060: the actual installed command route (run_installed_command_route.gd,
## unchanged checks and outputs) with modern presentation preferences applied
## before the entry starts, plus optional window captures for visual QA.
##
## ROUTE_PREFS: JSON object of "section.key" -> value written through
##   ModernSettings.preferences.set_values before the first scene, for example
##   {"language.bundle":"en","display.aspect":"wide","accessibility.text_box":"modern"}.
##   The written preferences are echoed to /out/route-prefs.json.
## ROUTE_WINDOW: "<W>x<H>" window size kept on every published frame (window
##   size is not a set_values preference and the boots resize the window from
##   the preferences), e.g. 1280x720 so a wide aspect has wing columns.
##   Presentation only.
## VISUAL_QA=1: pauses the logic at three points and writes window captures
##   /out/qa/<point>-<aspect>-<W>x<H>.png for every aspect in QA_ASPECTS and
##   size in QA_SIZES, then restores the starting size and aspect:
##   dialogue-modern  first fully revealed line >= 12 (text box as configured),
##   dialogue-native  first fully revealed line >= 20 after switching the text
##                    box to native (native English glyphs when English is on),
##   command-select   the first command-select frame, before the route's own
##                    finish writes its outputs.
## Text box (every published frame, while accessibility.text_box is modern):
##   the box is hidden whenever the native message window holds no line
##   (FirstArrivalPresenter.message_line < 0), and when shown it shows the
##   window's line. Once, on the first shown line >= 4, the dialogue pack is
##   swapped for an empty one and back through preferences_changed (as a
##   language switch or a late bundle does) and the box must follow both
##   swaps within the same line. On reload and command-select frames (drawn by
##   the command reload renderer, whose window holds the command HUD message)
##   the box must be hidden. Counts go to /out/modern-box.json.
## Source wings (when ROUTE_PREFS sets display.wing_fill=source, a wide
##   aspect and a window wider than 4:3, e.g. display.width/height 1280x720):
##   every published mode 0x34 reload frame must get checker wings from
##   ProjectionCheckerWingProvider (host.last_wing_source == "checker", empty
##   host.error) and the 0x34/0xFF/0/0 switch frame, which still shows the
##   opening picture, black wings without an error; counts go to
##   /out/modern-wings.json.
## Captures happen while the logic is paused (as the base observer's own window
## captures), so they never change a logic tick; the base observer's capture
## frames are skipped. Presentation only; game state is untouched.
## Runner: ROUTE_OBSERVER=modern sh tools/port/run_installed_route.sh <out> ...
const Presenter = preload("res://presentation/first_arrival_scene_presenter.gd")
const TextPack = preload("res://services/text_translation_store.gd")
const ReloadRenderer = preload("res://presentation/command_reload_renderer.gd")
const RoutePrefs = preload("res://tests/qa/route_presentation_prefs.gd")
const QA_SIZES := [Vector2i(1280, 720), Vector2i(1920, 1080), Vector2i(960, 720)]
const QA_ASPECTS := ["original", "wide"]
var visual_qa := OS.get_environment("VISUAL_QA") == "1"
var _qa_busy := false
var _qa_done: Dictionary = {}
var _route_window := Vector2i.ZERO
var _wings := {"frames_0x34": 0, "checker": 0, "switch_black": 0, "other_modes": 0, "first_0x34_frame": -1}
var _box := {"modern_frames": 0, "shown": 0, "hidden_empty_window": 0, "hidden_no_translation": 0, "hidden_command_frames": 0, "store_swap_line": -1}

func _initialize() -> void:
	var prefs_error := RoutePrefs.apply(root)
	check(prefs_error.is_empty(), prefs_error)
	_route_window = RoutePrefs.window_size()
	super._initialize()

func _frame_published(tick: int, app: App) -> void:
	super._frame_published(tick, app)
	# The boots size the window from the preferences when they start, so the
	# requested size is (re)applied on published frames, outside QA captures.
	if _route_window != Vector2i.ZERO and not _qa_busy and root.size != _route_window: root.size = _route_window
	if not finishing:
		_check_text_box(app)
		_check_wings(app)
	if not visual_qa or _qa_busy or finishing or not app.is_physics_processing(): return
	var state = app.consumer.opening.state
	var work: Variant = state.opening.get("prologue_text_work")
	if not work is Dictionary or not work.get("line_id") is int or int(work.get("pos", 0)) < int(work.get("total", 1)): return
	var line: int = work.line_id
	if state.mode_stage >= 8 and line >= 12 and not _qa_done.has("dialogue-modern"):
		_qa_done["dialogue-modern"] = line
		app.set_physics_process(false)
		_qa_point.call_deferred(app, "dialogue-modern", "native")
	elif _qa_done.has("dialogue-modern") and line >= 20 and not _qa_done.has("dialogue-native"):
		_qa_done["dialogue-native"] = line
		app.set_physics_process(false)
		_qa_point.call_deferred(app, "dialogue-native", "")

func _check_text_box(app: App) -> void:
	var settings: Node = root.get_node_or_null("ModernSettings")
	if settings == null or str(settings.preferences.effective_settings().accessibility.get("text_box", "native")) != "modern": return
	var state = app.consumer.opening.state
	if not state.opening.has("arrival_media"): return
	var overlay: Control = app._host.overlay
	_box.modern_frames += 1
	# Reload and command-select frames are not drawn by the first-arrival
	# presenter: their window holds the command HUD message, not the opening's
	# last line, which stays in the opening state.
	if ReloadRenderer.reload_frame(state):
		check(not overlay.visible, "modern text box is shown on command frame %d (%02x/%d/%d/%d), which has no prologue window" % [state.frame_count, state.game_mode, state.mode_stage, state.mode_step, state.substep])
		_box.hidden_command_frames += 1
		return
	var line: int = Presenter.message_line(state.opening)
	if line < 0:
		check(not overlay.visible, "modern text box is shown at frame %d while the native message window holds no line" % state.frame_count)
		_box.hidden_empty_window += 1
		return
	if not overlay.visible:
		_box.hidden_no_translation += 1
		return
	_box.shown += 1
	check(overlay.active_key.get("line") == line, "modern text box shows line %s while the native window holds line %d (frame %d)" % [str(overlay.active_key.get("line")), line, state.frame_count])
	if line >= 4 and int(_box.store_swap_line) < 0:
		_box.store_swap_line = line
		var text: String = overlay.active_record.get("text", "")
		var kept: RefCounted = settings.text_pack
		settings.text_pack = TextPack.new()
		settings.preferences_changed.emit(settings.preferences.effective_settings())
		check(not overlay.visible and overlay.translation_store == settings.text_pack, "modern text box keeps the old pack's line after the pack was replaced mid-line")
		settings.text_pack = kept
		settings.preferences_changed.emit(settings.preferences.effective_settings())
		check(overlay.visible and overlay.translation_store == kept and overlay.active_record.get("text") == text, "modern text box did not pick the restored pack up within the same line")

func _wings_requested() -> bool:
	var settings: Node = root.get_node_or_null("ModernSettings")
	if settings == null: return false
	var display: Dictionary = settings.preferences.effective_settings().display
	return str(display.get("wing_fill", "")) == "source" and str(display.get("aspect", "")) in ["wide", "auto"]

func _check_wings(app: App) -> void:
	if _qa_busy or not _wings_requested(): return
	var state = app.consumer.opening.state
	var host: Control = app._host
	# A 4:3 window has no wing columns even in wide mode (logical width 320).
	if host.layout.get("mode") != "wide" or int(host.layout.get("logical_width", 320)) <= 320: return
	if state.game_mode != 0x34:
		_wings.other_modes += 1
		return
	_wings.frames_0x34 += 1
	if int(_wings.first_0x34_frame) < 0: _wings.first_0x34_frame = state.frame_count
	# The switch frame still shows the opening picture: black wings, no error.
	var expected := "checker" if ReloadRenderer.reload_frame(state) else "black"
	check(host.last_wing_source == expected and host.error.is_empty(),
		"mode 0x34 frame %d (%02x/%d/%d/%d) got %s wings, expected %s: %s" % [state.frame_count, state.game_mode, state.mode_stage, state.mode_step, state.substep, host.last_wing_source, expected, host.error])
	if host.last_wing_source == "checker": _wings.checker += 1
	else: _wings.switch_black += 1

func _finish_command(app: App) -> void:
	if _wings_requested():
		check(int(_wings.checker) > 0 and int(_wings.checker) + int(_wings.switch_black) == int(_wings.frames_0x34) and int(_wings.switch_black) <= 1, "source wings route checks never ran or fell back: " + JSON.stringify(_wings))
		var wings_file := FileAccess.open("/out/modern-wings.json", FileAccess.WRITE)
		wings_file.store_string(JSON.stringify(_wings, "\t")); wings_file.close()
	var settings: Node = root.get_node_or_null("ModernSettings")
	if settings != null and str(settings.preferences.effective_settings().accessibility.get("text_box", "native")) == "modern" or int(_box.modern_frames) > 0:
		check(int(_box.shown) > 0 and int(_box.hidden_empty_window) > 0 and int(_box.hidden_command_frames) > 0 and int(_box.store_swap_line) >= 0, "modern text box route checks never ran: " + JSON.stringify(_box))
		var file := FileAccess.open("/out/modern-box.json", FileAccess.WRITE)
		file.store_string(JSON.stringify(_box, "\t")); file.close()
	if visual_qa and not _qa_done.has("command-select"):
		_qa_done["command-select"] = true
		await _qa_captures(app, "command-select")
	super._finish_command(app)

## Captures one point, then optionally switches the text box and resumes.
func _qa_point(app: App, point: String, next_text_box: String) -> void:
	await _qa_captures(app, point)
	if not next_text_box.is_empty():
		root.get_node("ModernSettings").preferences.set_values({"accessibility.text_box": next_text_box})
	if is_instance_valid(app) and not finishing: app.set_physics_process(true)

func _qa_captures(app: App, point: String) -> void:
	_qa_busy = true
	var settings: Node = root.get_node("ModernSettings")
	var window := root.get_window()
	var start_size := window.size
	var start_aspect: String = settings.preferences.settings.display.aspect
	DirAccess.make_dir_recursive_absolute("/out/qa")
	var record: Array = []
	for aspect: String in QA_ASPECTS:
		settings.preferences.set_values({"display.aspect": aspect})
		for size: Vector2i in QA_SIZES:
			window.size = size
			for i: int in 4:
				_pump(app)
				await process_frame
			_pump(app)
			await RenderingServer.frame_post_draw
			var image := RoutePrefs.window_image(root.get_texture().get_image(), window.size)
			var name := "%s-%s-%dx%d" % [point, aspect, size.x, size.y]
			if image != null: image.save_png("/out/qa/%s.png" % name)
			record.append({"name": name, "window": [window.size.x, window.size.y], "line": _qa_done.get(point),
				"text_box": settings.preferences.settings.accessibility.text_box, "language": settings.preferences.settings.language.bundle})
	settings.preferences.set_values({"display.aspect": start_aspect})
	window.size = start_size
	for i: int in 3:
		_pump(app)
		await process_frame
	var file := FileAccess.open("/out/qa/%s.json" % point, FileAccess.WRITE)
	file.store_string(JSON.stringify(record, "\t")); file.close()
	_qa_busy = false

func _pump(app: App) -> void:
	if is_instance_valid(app) and app.consumer != null:
		app.bgm.pump(app.consumer.opening.state)
