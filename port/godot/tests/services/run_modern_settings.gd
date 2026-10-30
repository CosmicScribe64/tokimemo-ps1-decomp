extends "res://tests/services/settings_test_tree.gd"
const Preferences = preload("res://services/user_preferences_service.gd")
const Remaps = preload("res://services/input_remap_service.gd")
const Pad = preload("res://ui/native_pad.gd")
const EntryPad = preload("res://ui/entry_pad.gd")
const Access = preload("res://services/accessibility_service.gd")
const History = preload("res://services/read_history_service.gd")
func _initialize() -> void:
	call_deferred("run")
func run() -> void:
	var path := "user://settings-test.json"
	for suffix: String in ["", ".backup", ".tmp", ".backup.tmp"]: DirAccess.remove_absolute(ProjectSettings.globalize_path(path + suffix))
	var preferences := Preferences.new(path)
	check(preferences.load_preferences())
	check(preferences.set_value("audio", "mono", true))
	var loaded := Preferences.new(path); check(loaded.load_preferences()); check(loaded.settings.audio.mono)
	var display: Dictionary = preferences.settings.display.duplicate(true); display.width = 1280
	check(preferences.begin_display(display, 0)); check(preferences.effective_settings().display.width == 1280)
	loaded = Preferences.new(path); check(loaded.load_preferences()); check(loaded.recovered); check(loaded.settings.display.width == 960)
	preferences.tick(16); check(preferences.pending.is_empty()); check(preferences.settings.display.width == 960)
	check(preferences.begin_display(display, 0, true)); preferences.tick(16); check(not preferences.pending.is_empty())
	preferences.tick(17, false); check(preferences.pending.is_empty())
	check(preferences.begin_display(display, 0)); check(preferences.confirm_display()); check(preferences.settings.display.width == 1280)
	check(not preferences.set_value("audio", "mono", 2))
	check(not preferences.set_value("controls", "bindings", {"confirm": [{"kind": "key", "code": KEY_F10}]}))
	var remaps := Remaps.new(); check(remaps.configure(preferences.settings.controls))
	var event := InputEventKey.new(); event.physical_keycode = KEY_ENTER; event.pressed = true
	check(remaps.event_mask(event) == 0x20); event.pressed = false; check(remaps.event_mask(event) == 0)
	check(remaps.rebind("left", {"kind": "key", "code": KEY_ENTER}).has("conflicts"))
	var left_only: Dictionary = remaps.rebind("left", {"kind": "key", "code": KEY_H})
	check(left_only.bindings.keys() == ["left"])
	var replaced: Dictionary = remaps.rebind("left", {"kind": "key", "code": KEY_ENTER}, true)
	check(replaced.error.is_empty())
	var controls: Dictionary = preferences.settings.controls.duplicate(true); controls.bindings = replaced.bindings
	check(remaps.configure(controls)); event.pressed = true; check(remaps.event_mask(event) == 0x8000)
	controls.bindings = {}; check(remaps.configure(controls))
	var axis := InputEventJoypadMotion.new(); axis.device = 4; axis.axis = JOY_AXIS_LEFT_X; axis.axis_value = -0.9
	check(remaps.event_mask(axis) == 0x8000); axis.axis_value = 0.0; check(remaps.event_mask(axis) == 0)
	var runtime := root.get_node("ModernSettings")
	var preview: Dictionary = runtime.preferences.settings.display.duplicate(true); preview.width = 1280
	check(runtime.preferences.begin_display(preview, Time.get_ticks_msec() / 1000.0))
	check(runtime.applied_display.width == 1280)
	check(runtime.preferences.set_value("audio", "mono", true))
	check(runtime.applied_display.width == 1280 and not runtime.preferences.pending.is_empty())
	check(runtime.preferences.restore_category("audio")); check(runtime.applied_display.width == 1280)
	check(runtime.preferences.revert_display())
	var pad := Pad.new(); root.add_child(pad)
	var entry := EntryPad.new(); root.add_child(entry)
	event.pressed = true; pad._input(event); entry._input(event)
	check(pad.sample(0).held == 0x20); check(entry.sample(0).held == 0x800)
	event.pressed = false; pad._input(event); entry._input(event)
	check(pad.sample(1).held == 0); check(entry.sample(1).held == 0)
	var connected_button := InputEventJoypadButton.new(); connected_button.device = 8; connected_button.button_index = JOY_BUTTON_DPAD_RIGHT; connected_button.pressed = true
	pad._input(connected_button); entry._input(connected_button)
	connected_button.button_index = JOY_BUTTON_START; entry._input(connected_button)
	event.physical_keycode = KEY_SPACE; event.pressed = true; pad._input(event); entry._input(event)
	check(pad.sample(2).held == 0x2020); check(entry.sample(2).held == 0x2820)
	runtime.settings_buttons[8] = 3; runtime.chord_latched[8] = true
	Input.joy_connection_changed.emit(8,false)
	check(not runtime.settings_buttons.has(8) and not runtime.chord_latched.has(8))
	var unplugged = pad.sample(3); var unplugged_entry = entry.sample(3)
	check(unplugged.held == 0x20 and unplugged.released == 0x2000)
	check(unplugged_entry.held == 0x20 and unplugged_entry.released == 0x2800)
	Input.joy_connection_changed.emit(8,true)
	check(pad.sample(4).released == 0 and entry.sample(4).released == 0)
	event.pressed = false; pad._input(event); entry._input(event); pad.sample(5); entry.sample(5)
	var buses: Dictionary = runtime.preferences.settings.audio.buses.duplicate(true)
	buses.BGM.volume = 0.5
	var audio_settings: Dictionary = runtime.preferences.settings.duplicate(true)
	audio_settings.audio.buses = buses; audio_settings.audio.mono = true
	runtime.apply_preferences(audio_settings)
	check(is_equal_approx(AudioServer.get_bus_volume_db(AudioServer.get_bus_index("BGM")), linear_to_db(0.5)))
	check(AudioServer.is_bus_effect_enabled(0, AudioServer.get_bus_effect_count(0) - 1))
	var recording := AudioEffectRecord.new()
	AudioServer.add_bus_effect(0, recording)
	var recording_index := AudioServer.get_bus_effect_count(0) - 1
	audio_settings.audio.mono = false; runtime.apply_preferences(audio_settings)
	check(AudioServer.is_bus_effect_enabled(0, recording_index))
	check(not AudioServer.is_bus_effect_enabled(0, recording_index - 1))
	AudioServer.remove_bus_effect(0, recording_index)
	var access := Access.new(); access.configure(Preferences.defaults().accessibility)
	check(access.visible_glyphs(4, 10) == 4); check(not access.may_skip("line"))
	access.mark_read("line"); check(access.may_skip("line"))
	var accessible: Dictionary = Preferences.defaults().accessibility; accessible.auto_delay = 1.0
	access.configure(accessible); check(not access.auto_advance("line", true, true, 5)); check(not access.auto_advance("line", true, true, 5))
	accessible.auto_advance = true
	access.configure(accessible); check(not access.auto_advance("line", true, false, 5))
	check(not access.auto_advance("line", true, true, 0.5)); check(access.auto_advance("line", true, true, 0.5))
	runtime.toggle_menu(); await process_frame; check(runtime.menu.visible); check(paused)
	runtime.menu._select_page(runtime.menu.page_titles.find("Controls"))
	runtime.menu.focus_items["binding:left"].grab_focus()
	runtime.menu.draft.width = 1184; runtime.menu.draft.height = 888
	runtime.menu.rebuild(); await process_frame
	check(runtime.menu.tabs.current_tab == runtime.menu.page_titles.find("Controls"))
	check(root.gui_get_focus_owner().get_meta("settings_focus_key") == "binding:left")
	var saved_display: Dictionary = runtime.preferences.settings.display.duplicate(true)
	runtime.menu.draft.width = 1280
	runtime.menu._preview_display(); await process_frame
	runtime.menu.focus_items["Keep display"].grab_focus()
	runtime.menu.focus_items["Keep display"].pressed.emit(); await process_frame
	check(runtime.preferences.pending.is_empty() and runtime.applied_display.width == 1280)
	check(root.gui_get_focus_owner() != null and root.gui_get_focus_owner().is_visible_in_tree())
	check(runtime.menu.tabs.current_tab == runtime.menu.page_titles.find("Controls"))
	check(runtime.preferences.begin_display(saved_display,Time.get_ticks_msec()/1000.0))
	check(runtime.preferences.confirm_display())
	runtime.menu.draft = saved_display; runtime.menu.rebuild(); await process_frame
	var resolution: OptionButton = runtime.menu.focus_items["Window size"]
	runtime.menu.draft.width = 1184; runtime.menu.draft.height = 888
	runtime.menu.focus_items["binding:left"].grab_focus()
	runtime.menu.rebuild(); await process_frame
	resolution = runtime.menu.focus_items["Window size"]
	check(resolution.get_item_text(resolution.selected) == "1184×888 (remembered)")
	check(runtime.preferences.begin_display(runtime.menu.draft,Time.get_ticks_msec()/1000.0))
	runtime.menu._process(0)
	runtime.preferences.tick(runtime.preferences.pending.deadline+1,true)
	runtime.menu._process(0); await process_frame
	resolution = runtime.menu.focus_items["Window size"]
	check(resolution.get_item_text(resolution.selected).begins_with("%d×%d" % [runtime.applied_display.width,runtime.applied_display.height]))
	check(runtime.menu.tabs.current_tab == runtime.menu.page_titles.find("Controls"))
	check(root.gui_get_focus_owner().get_meta("settings_focus_key") == "binding:left")
	for toast: Node in get_nodes_in_group("settings_notices"): check(not toast.visible)
	runtime.toggle_menu(); check(not runtime.menu.visible); check(not paused)
	var stick := InputEventJoypadButton.new(); stick.device = 2; stick.button_index = JOY_BUTTON_LEFT_STICK; stick.pressed = true
	runtime._input(stick); check(not runtime.menu.visible)
	stick.device = 3; stick.button_index = JOY_BUTTON_RIGHT_STICK; runtime._input(stick); check(not runtime.menu.visible)
	stick.device = 2; runtime._input(stick); check(runtime.menu.visible and paused)
	runtime._input(stick); check(runtime.menu.visible)
	await process_frame
	check(root.gui_get_focus_owner() != null)
	stick.pressed = false; runtime._input(stick)
	stick.button_index = JOY_BUTTON_LEFT_STICK; runtime._input(stick)
	stick.pressed = true; runtime._input(stick); stick.button_index = JOY_BUTTON_RIGHT_STICK; runtime._input(stick)
	check(not runtime.menu.visible and not paused)
	check(pad.sample(3).held == 0 and entry.sample(3).held == 0)
	var history := History.new("user://history-test.json")
	DirAccess.remove_absolute(ProjectSettings.globalize_path(history.path))
	check(history.load_history()); check(not history.contains_all(["line-0"]))
	check(history.mark_read("line-0")); check(history.mark_read("line-0"))
	var reopened := History.new(history.path); check(reopened.load_history()); check(reopened.contains_all(["line-0"]))
	check(not reopened.contains_all(["line-0", "line-1"]))
	runtime.read_history = history
	check(not runtime.request_prologue_skip(pad)); check(pad.sample(4).held == 0)
	runtime.preferences.settings.accessibility.skip = "all"
	check(runtime.request_prologue_skip(pad)); check(pad.sample(5).pressed == 0x800); check(pad.sample(6).released == 0x800)
	print("Modern settings: persistence, crash/timeout/focus recovery, confirmation, input/axis/entry/conflicts, audio math/mono, accessibility timing, overlay passed")
	finish()
