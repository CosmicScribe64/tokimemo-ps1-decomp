extends "res://tests/services/settings_test_tree.gd"
## T-30002 WP8: entry points. Welcome step (defaults, 60 s Keep/Revert,
## visible English fallback, async English through the v2 contract fake,
## shown once, first_launch_setup ordering), the title Settings affordance,
## and the modern pause menu (F10 and L3+R3 in game, tree paused, no input
## leak, Settings round trip, quick save/load through the save host, F10 still
## opens Settings outside a game scene).
const Welcome = preload("res://ui/welcome_step.gd")
const Pause = preload("res://ui/pause_menu.gd")
const Pad = preload("res://ui/native_pad.gd")
const Title = preload("res://app/title_continue.gd")
const SetupScene = preload("res://app/first_launch_setup.tscn")
const Fakes = preload("res://tests/services/settings_v2_fakes.gd")

class Host extends Node:
	var requests: Array[bool] = []
	func _ready() -> void: add_to_group("settings_save_hosts")
	func request_settings_save(load_game: bool) -> bool:
		requests.append(load_game); return true

var runtime: Node
var pad: Node

func _initialize() -> void: call_deferred("run")

func _process(delta: float) -> bool:
	_elapsed += delta
	if failures > 0: quit(1)
	elif _elapsed > 60.0:
		printerr("Entry point test did not finish within 60 seconds"); quit(1)
	return false

func run() -> void:
	runtime = root.get_node("ModernSettings")
	pad = Pad.new(); root.add_child(pad)
	await process_frame
	check(not Welcome.enabled())  # headless runs keep all-Original defaults
	await _welcome_v1()
	await _welcome_window_on_continue()
	await _welcome_v2_english()
	await _setup_runs_welcome_before_launch()
	await _title_affordance()
	await _pause_menu()
	print("Entry points: welcome defaults/once/keep-revert/fallback/async English, setup ordering, title Settings affordance, pause menu F10/L3+R3/settings/save/no-leak passed")
	finish()

func _fail(message: String) -> void:
	printerr(message); check(false)

func _marker(name: String) -> String:
	var path := "user://" + name
	DirAccess.remove_absolute(ProjectSettings.globalize_path(path))
	return path

func _welcome_v1() -> void:
	var marker := _marker("welcome-v1.json")
	check(not Welcome.completed(marker))
	var step: Control = Welcome.new(); step.runtime = runtime; step.marker_path = marker
	var done := [false]
	step.finished.connect(func() -> void: done[0] = true)
	root.add_child(step); await process_frame
	var language: OptionButton = step.focus_items["Game language"]
	var preset: OptionButton = step.focus_items["Preset"]
	check(language.get_item_text(language.selected) == "English (AI translation)")
	# Design WP-E: the Welcome step shows the AI-translation disclosure.
	var disclosed := false
	for label: Node in step.find_children("*", "Label", true, false):
		if (label as Label).text == "English text is AI-translated and not yet human-reviewed; voices remain Japanese.": disclosed = true
	check(disclosed)
	check(preset.get_item_text(preset.selected) == "Original visuals (PS1 exact)")
	# The step closes its own focus loop, even with a visible control behind it.
	var decoy := Button.new(); decoy.text = "decoy"; root.add_child(decoy); root.move_child(decoy, step.get_index())
	var continue_button: Control = step.focus_items["continue"]
	check(continue_button.find_next_valid_focus() == language and language.find_prev_valid_focus() == continue_button)
	decoy.queue_free()
	# Window choice uses the first-launch 60 s Keep/Revert.
	var window: OptionButton = step.focus_items["Window"]
	window.select(1); window.item_selected.emit(1)
	var now := Time.get_ticks_msec() / 1000.0
	(step.focus_items["try_window"] as Button).pressed.emit(); await process_frame
	check(not runtime.preferences.pending.is_empty())
	check(float(runtime.preferences.pending.deadline) >= now + 59.0)
	check(runtime.preferences.pending.candidate.mode == "borderless")
	# Negative: Continue is refused while the window change is pending.
	step.confirm(); await process_frame
	check(not done[0] and step.notice.text.contains("Keep or revert"))
	(step.focus_items["revert_window"] as Button).pressed.emit(); await process_frame
	check(runtime.preferences.pending.is_empty() and runtime.preferences.settings.display.mode == "windowed")
	# English unavailable: visible fallback first, then Continue proceeds.
	step.confirm(); await process_frame
	check(not done[0] and step.notice.text.contains("Japanese is kept"))
	step.confirm(); await process_frame
	check(done[0] and Welcome.completed(marker))
	check(runtime.preferences.settings.accessibility.text_speed == "normal" and runtime.preferences.settings.display.filter == "integer")
	step.queue_free(); await process_frame

## Continue alone previews an untried Window choice with the 60 s Keep/Revert
## (it is never dropped silently); Keep saves it, Revert or the deadline
## restores the saved mode and resets the visible choice.
func _welcome_window_on_continue() -> void:
	var step: Control = Welcome.new(); step.runtime = runtime; step.marker_path = _marker("welcome-window.json")
	var done := [false]
	step.finished.connect(func() -> void: done[0] = true)
	root.add_child(step); await process_frame
	var window: OptionButton = step.focus_items["Window"]
	check(window.get_item_text(window.selected) == "Window" and runtime.preferences.settings.display.mode == "windowed")
	window.select(1); window.item_selected.emit(1)
	var now := Time.get_ticks_msec() / 1000.0
	step.confirm(); await process_frame
	check(not done[0] and not runtime.preferences.pending.is_empty())
	check(runtime.preferences.pending.candidate.mode == "borderless" and float(runtime.preferences.pending.deadline) >= now + 59.0)
	check(step.keep_row.visible and step.notice.text.contains("Keep this window setting"))
	# Keep saves the mode and resumes Continue (here: the visible English fallback).
	(step.focus_items["keep_window"] as Button).pressed.emit(); await process_frame
	check(runtime.preferences.pending.is_empty() and runtime.preferences.settings.display.mode == "borderless")
	check(not done[0] and step.notice.text.contains("Japanese is kept"))
	step.confirm(); await process_frame
	check(done[0] and runtime.preferences.settings.display.mode == "borderless")
	step.queue_free(); await process_frame
	# Revert: the saved mode stays and the choice shows it again.
	step = Welcome.new(); step.runtime = runtime; step.marker_path = _marker("welcome-window-revert.json")
	done[0] = false
	step.finished.connect(func() -> void: done[0] = true)
	root.add_child(step); await process_frame
	window = step.focus_items["Window"]
	check(window.get_item_text(window.selected) == "Fullscreen")
	window.select(0); window.item_selected.emit(0)
	step.confirm(); await process_frame
	check(not done[0] and runtime.preferences.pending.candidate.mode == "windowed")
	(step.focus_items["revert_window"] as Button).pressed.emit(); await process_frame
	check(runtime.preferences.pending.is_empty() and runtime.preferences.settings.display.mode == "borderless")
	check(window.get_item_text(window.selected) == "Fullscreen" and step.window_mode == "borderless")
	# Deadline: an expired preview restores the mode and does not continue.
	window.select(0); window.item_selected.emit(0)
	step._acknowledged_fallback = false
	step.confirm(); await process_frame
	check(not runtime.preferences.pending.is_empty())
	runtime.preferences.revert_display(); await process_frame; await process_frame
	check(not done[0] and step.window_mode == "borderless" and window.get_item_text(window.selected) == "Fullscreen")
	check(step.notice.text == "Window setting restored.")
	step.queue_free(); await process_frame
	# Restore windowed mode for the later checks.
	var restore: Dictionary = runtime.preferences.settings.display.duplicate(true); restore.mode = "windowed"
	check(runtime.preferences.begin_display(restore, Time.get_ticks_msec() / 1000.0, false))
	runtime.preferences.confirm_display()
	check(runtime.preferences.settings.display.mode == "windowed")

func _welcome_v2_english() -> void:
	var fake: Node = Fakes.FakeRuntime.new(); fake.name = "WelcomeFakeRuntime"; root.add_child(fake)
	await process_frame
	var marker := _marker("welcome-v2.json")
	var step: Control = Welcome.new(); step.runtime = fake; step.marker_path = marker
	var done := [false]
	step.finished.connect(func() -> void: done[0] = true)
	root.add_child(step); await process_frame
	check(not step.focus_items.has("english_unavailable"))
	var preset: OptionButton = step.focus_items["Preset"]
	preset.select(1); preset.item_selected.emit(1)
	step.confirm(); await process_frame
	check(fake.preferences.presets_applied == ["modern"])
	check(fake.preferences.settings.language.bundle == "en")
	# Async language open: the step waits, then finishes once.
	check(not done[0] and fake.language_loading)
	fake.language_loading = false; await process_frame; await process_frame
	check(done[0] and Welcome.completed(marker))
	step.queue_free(); fake.queue_free(); await process_frame

func _setup_runs_welcome_before_launch() -> void:
	var setup: Control = SetupScene.instantiate()
	setup.welcome_enabled = true
	setup.welcome_marker_path = _marker("welcome-setup.json")
	root.add_child(setup); await process_frame
	setup._continue_installed("", "")
	check(is_instance_valid(setup.welcome) and setup.welcome.visible)
	# Modal: Tab and Shift+Tab never leave the Welcome for the hidden setup buttons.
	await process_frame
	var first: Control = setup.welcome.focus_items["Game language"]
	first.grab_focus()
	check(not setup._start.is_visible_in_tree() and not setup._choose_folder_button.is_visible_in_tree())
	for direction: int in [1, -1]:
		var at: Control = first
		for step_index: int in 24:
			at = at.find_next_valid_focus() if direction == 1 else at.find_prev_valid_focus()
			if at == null or not setup.welcome.is_ancestor_of(at):
				_fail("focus left the Welcome step: %s" % (at.get_path() if at != null else "null")); break
	# Tab from Continue wraps to the first control, Shift+Tab from the first to Continue.
	check((setup.welcome.focus_items["continue"] as Control).find_next_valid_focus() == first)
	check(first.find_prev_valid_focus() == setup.welcome.focus_items["continue"])
	var status_before: String = setup._status.text
	setup._continue_installed("", "")  # second request while open: no duplicate
	check(setup.get_children().filter(func(child: Node) -> bool: return child is Welcome).size() == 1)
	setup.welcome.confirm(); setup.welcome.confirm(); await process_frame
	# Launch ran after the step: this container has no install, so the
	# revalidation message replaces the previous status.
	check(setup.welcome == null and setup._status.text != status_before)
	check(setup._status.is_visible_in_tree())  # setup UI shown again after the step
	check(setup._status.text.contains("could not be verified"))
	# Shown once: the next request goes straight to launch.
	setup._continue_installed("", "")
	check(setup.welcome == null)
	setup.queue_free(); await process_frame

func _title_affordance() -> void:
	var title: Control = Title.new(); title.save_root = OS.get_user_data_dir().path_join("title-affordance-saves")
	root.add_child(title); await process_frame
	check(title.settings_hint.text == "F10 / L3+R3: Settings" and title.settings_button.text == "Settings")
	for action: Dictionary in title.actions: check(action.id != "settings")
	check(title.find_children("*", "Node2D", true, false).is_empty())  # no native canvas on this screen
	title.settings_button.pressed.emit(); await process_frame
	check(runtime.menu.visible and paused)
	runtime.toggle_menu(); await process_frame
	check(not runtime.menu.visible and not paused)
	title.queue_free(); await process_frame

func _pause_menu() -> void:
	# The settings menu installs the pause menu itself; install() is idempotent.
	var installed: Node = runtime.layer.get_node_or_null("PauseMenu")
	check(installed != null)
	var pause: Control = Pause.install(runtime)
	check(pause == installed and runtime.layer.get_children().filter(func(child: Node) -> bool: return child is Pause).size() == 1)
	await process_frame
	# Outside a game scene F10 keeps opening Settings.
	_key(KEY_F10, true); _key(KEY_F10, false); await process_frame
	check(runtime.menu.visible and not pause.visible)
	_key(KEY_F10, true); _key(KEY_F10, false); await process_frame
	check(not runtime.menu.visible and not paused)
	var host := Host.new(); root.add_child(host); await process_frame
	# In game: F10 opens the pause menu, pauses the tree and clears held input.
	_key(KEY_ENTER, true); check(pad.sample(0).held == 0x20)
	_key(KEY_F10, true); _key(KEY_F10, false); await process_frame
	check(pause.visible and paused and not runtime.menu.visible)
	check(pad.sample(1).held == 0)
	_key(KEY_ENTER, false); _key(KEY_LEFT, true)  # paused: never reaches the pad
	check(pad.sample(2).held == 0)
	_key(KEY_LEFT, false)
	# Back resumes; the dismiss key does not leak into the game.
	_key(KEY_ESCAPE, true); await process_frame
	check(not pause.visible and not paused)
	check(pad.sample(3).held == 0)
	_key(KEY_ESCAPE, false); check(pad.sample(4).held == 0)
	# L3+R3 opens it too; Settings round trip returns to the pause menu.
	_stick(JOY_BUTTON_LEFT_STICK, true); _stick(JOY_BUTTON_RIGHT_STICK, true); await process_frame
	check(pause.visible and paused and not runtime.menu.visible)
	_stick(JOY_BUTTON_LEFT_STICK, false); _stick(JOY_BUTTON_RIGHT_STICK, false)
	(pause.focus_items["Settings"] as Button).pressed.emit(); await process_frame
	check(runtime.menu.visible and not pause.visible and paused)
	runtime.toggle_menu(); await process_frame; await process_frame
	check(pause.visible and paused)
	# Quick save and load go to the save host after resuming.
	(pause.focus_items["Quick save"] as Button).pressed.emit(); await process_frame
	check(host.requests == [false] and not pause.visible and not paused)
	_key(KEY_F10, true); _key(KEY_F10, false); await process_frame
	(pause.focus_items["Load quick save"] as Button).pressed.emit(); await process_frame
	check(host.requests == [false, true] and not paused)
	# Quit asks first; Back cancels the confirmation without resuming.
	_key(KEY_F10, true); _key(KEY_F10, false); await process_frame
	check(not pause.focus_items.has("Return to title"))
	(pause.focus_items["Quit game"] as Button).pressed.emit(); await process_frame
	check(pause.quit_confirm.visible)
	_key(KEY_ESCAPE, true); _key(KEY_ESCAPE, false); await process_frame
	check(pause.visible and not pause.quit_confirm.visible and paused)
	_key(KEY_F10, true); _key(KEY_F10, false); await process_frame
	check(not pause.visible and not paused)
	host.queue_free(); pause.queue_free(); await process_frame

func _key(code: Key, pressed: bool) -> void:
	var event := InputEventKey.new(); event.physical_keycode = code; event.keycode = code; event.pressed = pressed
	root.push_input(event)

func _stick(button: JoyButton, pressed: bool) -> void:
	var event := InputEventJoypadButton.new(); event.device = 0; event.button_index = button; event.pressed = pressed
	root.push_input(event)
