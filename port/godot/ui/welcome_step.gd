extends Control
## T-30002 WP8: one-time Welcome step shown after the original game data is
## imported. The player confirms the game language (English preselected), a
## preset (Original visuals preselected) and the window mode, which uses the
## 60-second Keep/Revert (started by "Try window setting" or by Continue
## when the choice differs from the saved mode). Every choice is saved through the preferences
## service and can be changed later in Settings. Nothing here touches native
## game, save or RNG state.
signal finished
const Widgets = preload("res://ui/settings/settings_widgets.gd")
const Context = preload("res://ui/settings/settings_context.gd")
const MARKER_SCHEMA := "tokimemo.port-welcome.v1"
const DEFAULT_MARKER := "user://welcome-step.json"
const WINDOW_NAMES := {"windowed": "Window", "borderless": "Fullscreen"}
## T-39230: the Welcome step also offers the Parity / Modern play mode.
const Parity = preload("res://services/parity_mode.gd")

var runtime: Node
var marker_path := DEFAULT_MARKER
var language := "en"
var preset := "original"
## 'modern' preselected (the language and preset choices apply); 'parity'
## locks both to the original game. A forced launch starts at 'parity'.
var play_mode := ""
## Starts as the saved display mode; see _build().
var window_mode := ""
var notice: Label
var keep_row: HBoxContainer
var focus_items: Dictionary = {}
var ctx: RefCounted
var _acknowledged_fallback := false
var _waiting_for_language := false
var _done := false
## Set when Continue started the window preview: Keep or Revert then resumes Continue.
var _continue_after_window := false
## True while a preview started here is pending (detects the 60 s timeout).
var _previewing := false

static func completed(path: String = DEFAULT_MARKER) -> bool:
	if not FileAccess.file_exists(path): return false
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	return parsed is Dictionary and parsed.get("schema") == MARKER_SCHEMA and parsed.get("completed") == true

## Headless runs and scripted drivers start with all-Original defaults.
static func enabled() -> bool:
	return DisplayServer.get_name() != "headless" and OS.get_environment("TOKIMEMO_SKIP_WELCOME") != "1"

func _ready() -> void:
	set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	mouse_filter = Control.MOUSE_FILTER_STOP
	theme = Widgets.settings_theme()
	process_mode = Node.PROCESS_MODE_ALWAYS
	_build()

func _build() -> void:
	for child: Node in get_children(): remove_child(child); child.queue_free()
	var widgets := Widgets.new(); widgets.focus_items = focus_items; focus_items.clear()
	ctx = Context.new(runtime, self, widgets)
	if window_mode.is_empty(): window_mode = _saved_mode()
	if play_mode.is_empty(): play_mode = "parity" if _parity_active() else "modern"
	var shade := ColorRect.new(); shade.color = Color("0b1716"); shade.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT); add_child(shade)
	var center := CenterContainer.new(); center.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT); add_child(center)
	var card := PanelContainer.new(); center.add_child(card)
	var box := VBoxContainer.new(); box.add_theme_constant_override("separation", 10); box.custom_minimum_size.x = 520; card.add_child(box)
	widgets.heading(box, "Welcome")
	widgets.note(box, "Choose how to start. You can change all of this later in Settings (F10 or L3\u00a0+\u00a0R3).")
	widgets.option(box, "Game language", ["en", "ja"], language, func(v: String) -> void: language = v; _acknowledged_fallback = false,
		"", ctx.language_names())
	widgets.note(box, ctx.language_disclosure(), "disclosure")
	if not ctx.language_available("en"):
		widgets.note(box, "English is not installed in this build yet. Japanese (Original) will be used.", "english_unavailable")
	widgets.option(box, "Preset", ["original", "modern"], preset, func(v: String) -> void: preset = v,
		"", {"original": "Original visuals (PS1 exact)", "modern": "Modern English"})
	widgets.option(box, "Play mode", Parity.MODES, play_mode, func(v: String) -> void: play_mode = v; _acknowledged_fallback = false; _build.call_deferred(),
		"", Parity.LABELS)
	widgets.note(box, Parity.EXPLANATION, "parity_explanation")
	widgets.option(box, "Window", ["windowed", "borderless"], window_mode, func(v: String) -> void: window_mode = v,
		"", WINDOW_NAMES)
	widgets.button(box, "Try window setting", _try_window, "try_window")
	keep_row = HBoxContainer.new(); keep_row.add_theme_constant_override("separation", 10); box.add_child(keep_row)
	widgets.button(keep_row, "Keep window", _keep_window, "keep_window")
	widgets.button(keep_row, "Revert window", _revert_window, "revert_window")
	keep_row.visible = not runtime.preferences.pending.is_empty()
	notice = widgets.note(box, "")
	widgets.button(box, "Continue", confirm, "continue")
	widgets.lock_parity_rows(play_mode == "parity", _forced())
	_close_focus_loop(focus_items["Game language"], focus_items["continue"])
	(focus_items["Game language"] as Control).call_deferred("grab_focus")

## The step is modal: Tab, Shift+Tab and the arrow keys wrap between the first
## and last controls instead of leaving the step for whatever is behind it.
func _close_focus_loop(first: Control, last: Control) -> void:
	first.focus_previous = first.get_path_to(last); first.focus_neighbor_top = first.get_path_to(last)
	last.focus_next = last.get_path_to(first); last.focus_neighbor_bottom = last.get_path_to(first)

func _parity_active() -> bool:
	return runtime.preferences.has_method("parity_active") and bool(runtime.preferences.parity_active())

func _forced() -> bool:
	return "parity_forced_by" in runtime.preferences and not str(runtime.preferences.parity_forced_by).is_empty()

func _saved_mode() -> String:
	return str(runtime.preferences.settings.display.get("mode", "windowed"))

func _try_window() -> void:
	var candidate: Dictionary = runtime.preferences.settings.display.duplicate(true)
	candidate.mode = window_mode
	# First-launch previews always use the 60-second confirmation.
	if not runtime.preferences.begin_display(candidate, Time.get_ticks_msec() / 1000.0, true):
		notice.text = str(runtime.preferences.error); return
	_previewing = true
	keep_row.show(); (focus_items["keep_window"] as Control).grab_focus()
	notice.text = "Keep this window setting? It is restored in 60 seconds unless you keep it."

func _keep_window() -> void:
	runtime.preferences.confirm_display(); keep_row.hide(); notice.text = "Window setting saved."
	_previewing = false
	(focus_items["continue"] as Control).grab_focus()
	_resume_continue()

## Revert also resets the Window choice to the restored mode, so Continue does
## not start the same preview again.
func _revert_window() -> void:
	runtime.preferences.revert_display(); keep_row.hide(); notice.text = "Window setting restored."
	_previewing = false
	_sync_window_choice()
	(focus_items["continue"] as Control).grab_focus()
	_resume_continue()

func _sync_window_choice() -> void:
	window_mode = _saved_mode()
	var window: OptionButton = focus_items["Window"]
	var label: String = WINDOW_NAMES.get(window_mode, window_mode.capitalize())
	for index: int in window.item_count:
		if window.get_item_text(index) == label: window.select(index)

func _resume_continue() -> void:
	if not _continue_after_window: return
	_continue_after_window = false
	confirm()

## Saves the choices. A missing English bundle is reported once; pressing
## Continue again proceeds with Japanese.
func confirm() -> void:
	if _done or _waiting_for_language: return
	if not runtime.preferences.pending.is_empty():
		notice.text = "Keep or revert the window setting first."; return
	# A Window choice that was never tried is previewed now; Keep or Revert
	# then finishes Continue.
	if window_mode != _saved_mode():
		_try_window()
		if not runtime.preferences.pending.is_empty():
			_continue_after_window = true
			notice.text = "Keep this window setting? It is restored in 60 seconds unless you keep it. Continue follows."
		else:
			# The mode was refused (error shown); the next Continue keeps the saved mode.
			_sync_window_choice()
		return
	if play_mode == "parity":
		# Parity pins language and preset to the original game in one write.
		if not _forced() and not runtime.preferences.set_mode("parity"):
			notice.text = str(runtime.preferences.error); return
		_finish(); return
	if _parity_active() and not _forced() and not runtime.preferences.set_mode("modern"):
		notice.text = str(runtime.preferences.error); return
	if not _acknowledged_fallback:
		var applied: Dictionary = ctx.apply_preset(preset, false)
		var errors: Array[String] = []
		if not str(applied.error).is_empty(): errors.append(str(applied.error))
		if not applied.display.is_empty() and not runtime.preferences.begin_display(applied.display, Time.get_ticks_msec() / 1000.0, true):
			errors.append(str(runtime.preferences.error))
		if ctx.language_choice() != language:
			var language_error: String = ctx.select_language(language)
			if not language_error.is_empty() and not errors.has(language_error): errors.append(language_error)
		if not errors.is_empty():
			_acknowledged_fallback = true
			notice.text = " ".join(errors) + " Press Continue to start."
			return
	if ctx.language_loading():
		_waiting_for_language = true; notice.text = "Loading language packs…"; return
	_finish()

func _process(_delta: float) -> void:
	if keep_row != null: keep_row.visible = not runtime.preferences.pending.is_empty()
	if _previewing and runtime.preferences.pending.is_empty():
		# The 60 s deadline restored the old mode: show that, and do not
		# continue on the player's behalf.
		_previewing = false; _continue_after_window = false
		_sync_window_choice(); notice.text = "Window setting restored."
	if _waiting_for_language and not ctx.language_loading():
		_waiting_for_language = false; _finish()

func _finish() -> void:
	if _done: return
	_done = true
	var file := FileAccess.open(marker_path, FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify({"schema": MARKER_SCHEMA, "completed": true}) + "\n"); file.close()
	finished.emit()

func set_notice(text: String) -> void:
	if notice != null: notice.text = text

func _input(event: InputEvent) -> void:
	# The step is modal: game inputs never pass through while it is shown.
	if _waiting_for_language: accept_event()
