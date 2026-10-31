extends "res://tests/services/settings_test_tree.gd"
## T-30002 WP6: settings shell navigation. Every registered control is reached
## keyboard-only, controller-only and mouse-only with real injected events;
## Back leaves a page then closes; a pending display preview is never reverted
## silently on close; held input is cleared on open and the dismiss input never
## reaches the native pad; presets drive the fidelity badge; focus and scroll
## survive rebuilds.
const Pad = preload("res://ui/native_pad.gd")
const MENU_SOURCE := "res://ui/settings_menu.gd"
## Overlay-only controls that are reachable only while their overlay is shown.
const OVERLAY_KEYS := ["prompt:keep", "prompt:revert", "Keep display", "Revert"]

var runtime: Node
var menu: Control
var pad: Node

func _initialize() -> void: call_deferred("run")

func _process(delta: float) -> bool:
	_elapsed += delta
	if failures > 0: quit(1)
	elif _elapsed > 90.0:
		printerr("Settings navigation test did not finish within 90 seconds"); quit(1)
	return false

func run() -> void:
	runtime = root.get_node("ModernSettings")
	pad = Pad.new(); root.add_child(pad)
	await process_frame
	_shell_has_no_page_controls()
	await _held_input_cleared_on_open()
	menu = runtime.menu
	check(menu.page_titles == ["Language", "Display", "Audio", "Controls", "Text & Story", "Game"])
	var keyboard := await _reach_all("keyboard")
	var controller := await _reach_all("controller")
	var mouse := await _reach_all_mouse()
	var expected := _expected_keys()
	for key: String in expected:
		if not keyboard.has(key): _fail("keyboard cannot reach " + key)
		if not controller.has(key): _fail("controller cannot reach " + key)
		if not mouse.has(key): _fail("mouse cannot reach " + key)
	print("Reached %d controls by keyboard, %d by controller, %d by mouse (expected %d)" % [keyboard.size(), controller.size(), mouse.size(), expected.size()])
	await _back_semantics()
	await _pending_close_prompt()
	await _presets_and_badge()
	await _focus_and_scroll_survive_rebuild()
	if menu.visible: runtime.toggle_menu()
	check(not paused)
	print("Settings navigation: page host, keyboard/controller/mouse reachability, Back, pending-display close prompt, held/dismiss input, presets and badge, focus/scroll rebuild passed")
	finish()

func _fail(message: String) -> void:
	printerr(message); check(false)

func _shell_has_no_page_controls() -> void:
	var source := FileAccess.get_file_as_string(MENU_SOURCE)
	for page_word: String in ["Window mode", "swap_confirm", "Quick save", "text_speed", "Output device", "select_image_pack", "Scaling"]:
		if source.contains(page_word): _fail("settings_menu.gd still contains page-specific control: " + page_word)

func _held_input_cleared_on_open() -> void:
	var enter := _key(KEY_ENTER, true); root.push_input(enter)
	check(pad.sample(0).held == 0x20)
	Input.action_press("ui_down")
	runtime.toggle_menu(); await process_frame
	check(runtime.menu.visible and paused)
	check(not Input.is_action_pressed("ui_down"))
	check(pad.sample(1).held == 0)
	root.push_input(_key(KEY_ENTER, false))

func _expected_keys() -> Array[String]:
	var result: Array[String] = []
	for key: String in menu.focus_items:
		if key in OVERLAY_KEYS: continue
		result.append(key)
	return result

## Breadth-first walk over the focus graph using only injected events.
## Sidebar buttons are activated with accept to open each page.
func _reach_all(device: String) -> Dictionary:
	var reached := {}
	for index: int in menu.navigation.size():
		menu._select_page(0); menu.navigation[0].grab_focus(); await process_frame
		# Walk the sidebar to page `index` with Down, then accept it.
		for step: int in index: _press(device, "down")
		await process_frame
		var owner := root.gui_get_focus_owner()
		if owner != menu.navigation[index]: _fail("%s: sidebar Down did not reach page %d" % [device, index]); continue
		_press(device, "accept"); await process_frame
		if menu.tabs.current_tab != index: _fail("%s: accept did not open page %d" % [device, index])
		var queue: Array[Control] = [menu.navigation[index]]
		var seen := {menu.navigation[index]: true}
		while not queue.is_empty():
			var control: Control = queue.pop_front()
			if not is_instance_valid(control) or not control.is_visible_in_tree(): continue
			reached[str(control.get_meta("settings_focus_key", ""))] = true
			for direction: String in ["up", "down", "left", "right"]:
				control.grab_focus()
				_press(device, direction)
				var next := root.gui_get_focus_owner()
				if next != null and not seen.has(next) and next.has_meta("settings_focus_key"):
					seen[next] = true; queue.append(next)
		await process_frame
	return reached

func _reach_all_mouse() -> Dictionary:
	var reached := {}
	for index: int in menu.navigation.size():
		await _click(menu.navigation[index])
		if menu.tabs.current_tab != index: _fail("mouse: clicking sidebar did not open page %d" % index)
		var scroll: ScrollContainer = menu.tabs.get_child(index)
		for key: String in menu.focus_items:
			var control: Control = menu.focus_items[key]
			if not control.is_visible_in_tree(): continue
			if scroll.is_ancestor_of(control): scroll.ensure_control_visible(control); await process_frame
			_move_mouse(control)
			var hovered := root.gui_get_hovered_control()
			if hovered == control or (hovered != null and control.is_ancestor_of(hovered)): reached[key] = true
	return reached

func _back_semantics() -> void:
	for device: String in ["keyboard", "controller", "mouse"]:
		if not menu.visible: runtime.toggle_menu(); await process_frame
		var display_index: int = menu.page_titles.find("Display")
		menu._select_page(display_index)
		var inner: Control = menu.focus_items["Window mode"]
		inner.grab_focus()
		_back(device); await process_frame
		check(menu.visible)
		if root.gui_get_focus_owner() != menu.navigation[display_index]: _fail(device + ": Back did not leave the page")
		_back(device); await process_frame
		if menu.visible: _fail(device + ": second Back did not close")
		check(not paused)
		# The dismiss input was consumed: the native pad never sees it.
		check(pad.sample(10).held == 0)
		_back_release(device)
		check(pad.sample(11).held == 0)
	# Advanced section closes before the page is left.
	runtime.toggle_menu(); await process_frame
	menu._select_page(0)
	(menu.focus_items["advanced_open"] as Button).pressed.emit(); await process_frame
	check(menu.advanced_open and menu.focus_items.has("custom_images"))
	(menu.focus_items["custom_images"] as Control).grab_focus()
	_back("keyboard"); await process_frame; await process_frame
	check(not menu.advanced_open and menu.visible)
	check(root.gui_get_focus_owner() == menu.focus_items.get("advanced_open"))

func _pending_close_prompt() -> void:
	if not menu.visible: runtime.toggle_menu(); await process_frame
	var saved: Dictionary = runtime.preferences.settings.display.duplicate(true)
	# l3r3 drives the real autoload chord (no manual hook): the runtime's own
	# toggle would revert silently, so the shell must show the prompt first.
	for choice: String in ["revert", "keep", "f10", "l3r3"]:
		if not menu.visible: runtime.toggle_menu(); await process_frame
		var before: int = runtime.preferences.settings.display.width
		menu.draft.width = 1280 if before != 1280 else 960
		menu.preview_display(); await process_frame
		check(not runtime.preferences.pending.is_empty())
		menu.navigation[0].grab_focus()
		if choice == "f10": root.push_input(_key(KEY_F10, true)); root.push_input(_key(KEY_F10, false))
		elif choice == "l3r3":
			for button: JoyButton in [JOY_BUTTON_LEFT_STICK, JOY_BUTTON_RIGHT_STICK]: root.push_input(_stick(button, true))
			for button: JoyButton in [JOY_BUTTON_LEFT_STICK, JOY_BUTTON_RIGHT_STICK]: root.push_input(_stick(button, false))
		else: _back("keyboard")
		await process_frame
		# Negative: closing never reverts silently.
		check(menu.visible and menu.close_prompt.visible)
		check(not runtime.preferences.pending.is_empty())
		if root.gui_get_focus_owner() != menu.focus_items["prompt:keep"]: _fail("close prompt does not focus Keep")
		if choice == "keep":
			(menu.focus_items["prompt:keep"] as Button).pressed.emit(); await process_frame
			check(runtime.preferences.pending.is_empty() and runtime.preferences.settings.display.width == menu.draft.width)
		else:
			(menu.focus_items["prompt:revert"] as Button).pressed.emit(); await process_frame
			check(runtime.preferences.pending.is_empty() and runtime.preferences.settings.display.width == before)
		if menu.visible or paused: _fail("close prompt %s did not close (visible=%s paused=%s)" % [choice, menu.visible, paused])
	# Restore the original display settings for later checks.
	runtime.toggle_menu(); await process_frame
	menu.draft = saved.duplicate(true); menu.preview_display(); (menu.focus_items["Keep display"] as Button).pressed.emit(); await process_frame
	check(runtime.preferences.pending.is_empty())

func _presets_and_badge() -> void:
	if not menu.visible: runtime.toggle_menu(); await process_frame
	menu.select_preset("original"); await process_frame
	check(menu.fidelity_badge.text == "Original fidelity: ON")
	check(menu.preset_buttons["original"].button_pressed and not menu.preset_buttons["custom"].button_pressed)
	# Editing any field switches to Custom and the badge names the change.
	var text_page: int = menu.page_titles.find("Text & Story")
	menu._select_page(text_page)
	var speed: OptionButton = menu.focus_items["Text speed"]
	speed.select(2); speed.item_selected.emit(2); await process_frame
	check(runtime.preferences.settings.accessibility.text_speed == "fast")
	check(menu.fidelity_badge.text.begins_with("Original fidelity: OFF") and menu.fidelity_badge.text.contains("Text speed"))
	check(menu.preset_buttons["custom"].button_pressed and not menu.preset_buttons["original"].button_pressed)
	menu.select_preset("original"); await process_frame
	check(runtime.preferences.settings.accessibility.text_speed == "normal")
	check(menu.fidelity_badge.text == "Original fidelity: ON")
	# Negative: Modern English without the language bundle service keeps
	# Japanese and says so instead of falling back silently.
	menu.select_preset("modern"); await process_frame
	check(menu.ctx.language_choice() == "ja")
	check(menu.notice.text.contains("Japanese is kept"))
	menu.select_preset("original"); await process_frame
	check(menu.fidelity_badge.text == "Original fidelity: ON")
	# A pending display change that leaves Original shows OFF until reverted.
	menu.draft.filter = "linear"; menu.preview_display(); await process_frame
	check(menu.fidelity_badge.text.contains("Scaling") and not menu.fidelity_badge.text.contains("Filter"))
	(menu.focus_items["Revert"] as Button).pressed.emit(); await process_frame
	check(menu.fidelity_badge.text == "Original fidelity: ON")

func _focus_and_scroll_survive_rebuild() -> void:
	if not menu.visible: runtime.toggle_menu(); await process_frame
	var controls: int = menu.page_titles.find("Controls")
	menu._select_page(controls)
	var target: Control = menu.focus_items["binding:skip"]
	var scroll: ScrollContainer = menu.tabs.get_child(controls)
	target.grab_focus(); scroll.ensure_control_visible(target); await process_frame; await process_frame
	var before: int = scroll.scroll_vertical
	check(before > 0)
	menu.rebuild(); await process_frame; await process_frame
	scroll = menu.tabs.get_child(controls)
	check(menu.tabs.current_tab == controls)
	check(root.gui_get_focus_owner() != null and root.gui_get_focus_owner().get_meta("settings_focus_key", "") == "binding:skip")
	if scroll.scroll_vertical != before: _fail("scroll %d did not survive rebuild (%d)" % [before, scroll.scroll_vertical])

# --- input helpers ---------------------------------------------------------------

func _key(code: Key, pressed: bool) -> InputEventKey:
	var event := InputEventKey.new(); event.physical_keycode = code; event.keycode = code; event.pressed = pressed
	return event

func _joy(button: JoyButton, pressed: bool) -> InputEventJoypadButton:
	var event := InputEventJoypadButton.new(); event.device = 0; event.button_index = button; event.pressed = pressed
	event.pressure = 1.0 if pressed else 0.0
	return event

func _press(device: String, direction: String) -> void:
	if device == "keyboard":
		var code: Key = {"up": KEY_UP, "down": KEY_DOWN, "left": KEY_LEFT, "right": KEY_RIGHT, "accept": KEY_ENTER}[direction]
		root.push_input(_key(code, true)); root.push_input(_key(code, false))
	else:
		var button: JoyButton = {"up": JOY_BUTTON_DPAD_UP, "down": JOY_BUTTON_DPAD_DOWN, "left": JOY_BUTTON_DPAD_LEFT, "right": JOY_BUTTON_DPAD_RIGHT, "accept": JOY_BUTTON_A}[direction]
		root.push_input(_joy(button, true)); root.push_input(_joy(button, false))

func _back(device: String) -> void:
	match device:
		"keyboard": root.push_input(_key(KEY_ESCAPE, true))
		"controller": root.push_input(_joy(JOY_BUTTON_B, true))
		"mouse":
			var event := InputEventMouseButton.new(); event.button_index = MOUSE_BUTTON_RIGHT; event.pressed = true
			event.position = Vector2(4, 4); event.global_position = event.position; root.push_input(event)

func _back_release(device: String) -> void:
	match device:
		"keyboard": root.push_input(_key(KEY_ESCAPE, false))
		"controller": root.push_input(_joy(JOY_BUTTON_B, false))
		"mouse":
			var event := InputEventMouseButton.new(); event.button_index = MOUSE_BUTTON_RIGHT; event.pressed = false
			event.position = Vector2(4, 4); event.global_position = event.position; root.push_input(event)

func _screen_point(control: Control) -> Vector2:
	return root.get_final_transform() * control.get_global_rect().get_center()

func _move_mouse(control: Control) -> void:
	var motion := InputEventMouseMotion.new(); motion.position = _screen_point(control); motion.global_position = motion.position
	root.push_input(motion)

func _click(control: Control) -> void:
	_move_mouse(control)
	for pressed: bool in [true, false]:
		var event := InputEventMouseButton.new(); event.button_index = MOUSE_BUTTON_LEFT; event.pressed = pressed
		event.position = _screen_point(control); event.global_position = event.position
		root.push_input(event)
	await process_frame

func _stick(button: JoyButton, pressed: bool) -> InputEventJoypadButton:
	var event := InputEventJoypadButton.new(); event.device = 0; event.button_index = button; event.pressed = pressed
	return event
