extends Control
## T-30002 WP6: settings shell. It hosts page modules (ui/settings/*_page.gd,
## each with TITLE, available(ctx) and build(container, ctx)), the preset row,
## the fidelity badge, Back handling, the pending-display close prompt, the
## language loading state and the interface-size layer. It holds no
## page-specific controls. F10 / L3+R3 remain the reserved open/close keys.
const Widgets = preload("res://ui/settings/settings_widgets.gd")
const Context = preload("res://ui/settings/settings_context.gd")
const PAGE_SCRIPTS: Array[Script] = [preload("res://ui/settings/language_page.gd"),
	preload("res://ui/settings/display_page.gd"), preload("res://ui/settings/audio_page.gd"),
	preload("res://ui/settings/controls_page.gd"), preload("res://ui/settings/accessibility_page.gd"),
	preload("res://ui/settings/game_page.gd")]
const PauseMenu = preload("res://ui/pause_menu.gd")
const STICK_BITS := {JOY_BUTTON_LEFT_STICK: 1, JOY_BUTTON_RIGHT_STICK: 2}
const PRESET_NAMES := {"original": "Original (PS1 exact)", "modern": "Modern English", "custom": "Custom"}

var runtime: Node
var column: VBoxContainer
var tabs: TabContainer
var notice: Label
var draft: Dictionary
var capture := ""
var confirmation: HBoxContainer
var card: PanelContainer
var sidebar: VBoxContainer
var navigation: Array[Button] = []
var focus_items: Dictionary = {}
var revert_button: Button
var had_preview := false
## Language page: whether the Advanced / custom packs section is expanded.
var advanced_open := false
var widgets: RefCounted
var ctx: RefCounted
var page_titles: Array[String] = []
var fidelity_badge: Label
var language_badge: Label
var preset_buttons: Dictionary = {}
## T-39230: focus keys locked by Parity mode in the last rebuild.
var parity_locked: Array[String] = []
var close_prompt: PanelContainer
var loading_panel: PanelContainer
var _loading := false
var _close_after_prompt := false
var _layout_queued := false
## L3+R3 state per device, tracked here so a close chord with a pending display
## preview shows the Keep/Revert prompt before the runtime's toggle sees it.
var _sticks: Dictionary = {}
## True while Settings closes for a quick save or load, so a pause menu that
## opened Settings resumes the game instead of reappearing.
var closing_for_save := false
var _sticks_latched: Dictionary = {}
## Interface factor of the last layout, keyed by request and window area.
var _factor_key := Vector3(-1, 0, 0)
var _factor := 1.0

func _ready() -> void:
	set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	mouse_filter = Control.MOUSE_FILTER_STOP
	theme = Widgets.settings_theme()
	resized.connect(_queue_layout)
	visibility_changed.connect(_on_visibility_changed)
	# The settings runtime owns this menu; the pause menu joins the same layer
	# so F10 and L3+R3 open it in game scenes without a runtime edit.
	if runtime != null and runtime.get("layer") is CanvasLayer:
		PauseMenu.install.call_deferred(runtime)

func rebuild() -> void:
	var previous_tab := tabs.current_tab if is_instance_valid(tabs) else 0
	var previous_title: String = page_titles[previous_tab] if previous_tab < page_titles.size() else ""
	var focused := get_viewport().gui_get_focus_owner()
	var focus_key: String = str(focused.get_meta("settings_focus_key", "")) if focused != null else ""
	var scroll_positions := {}
	if is_instance_valid(tabs):
		for child: Node in tabs.get_children():
			if child is ScrollContainer: scroll_positions[child.name] = child.scroll_vertical
	var prior_draft: Dictionary = draft.duplicate(true) if visible and not draft.is_empty() else runtime.preferences.effective_settings().display.duplicate(true)
	for child: Node in get_children(): remove_child(child); child.queue_free()
	navigation.clear(); focus_items.clear(); page_titles.clear(); preset_buttons.clear(); _factor_key = Vector3(-1, 0, 0)
	widgets = Widgets.new(); widgets.focus_items = focus_items
	ctx = Context.new(runtime, self, widgets)
	draft = prior_draft
	var shade := ColorRect.new(); shade.color = Color(0.01, 0.025, 0.025, 0.82); shade.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT); add_child(shade)
	card = PanelContainer.new(); add_child(card)
	card.minimum_size_changed.connect(_queue_layout)
	column = VBoxContainer.new(); column.add_theme_constant_override("separation", 10); card.add_child(column)
	_build_header()
	_build_preset_row()
	column.add_child(HSeparator.new())
	var body := HBoxContainer.new(); body.add_theme_constant_override("separation", 16); body.size_flags_vertical = Control.SIZE_EXPAND_FILL; column.add_child(body)
	sidebar = VBoxContainer.new(); sidebar.custom_minimum_size.x = 140; sidebar.add_theme_constant_override("separation", 6); body.add_child(sidebar)
	tabs = TabContainer.new(); tabs.tabs_visible = false; tabs.size_flags_horizontal = Control.SIZE_EXPAND_FILL; tabs.size_flags_vertical = Control.SIZE_EXPAND_FILL; body.add_child(tabs)
	for script: Script in PAGE_SCRIPTS:
		var module: RefCounted = script.new()
		if not module.available(ctx): continue
		var box := page(str(module.TITLE))
		module.build(box, ctx)
	confirmation = HBoxContainer.new(); confirmation.add_theme_constant_override("separation", 10)
	widgets.button(confirmation, "Keep display", _keep_display)
	revert_button = widgets.button(confirmation, "Revert", _revert_display)
	column.add_child(HSeparator.new())
	column.add_child(confirmation); confirmation.visible = not runtime.preferences.pending.is_empty()
	var footer := HBoxContainer.new(); footer.add_theme_constant_override("separation", 16); column.add_child(footer)
	notice = Label.new(); notice.text = "Display changes need confirmation. Other settings save automatically. Esc, B or right-click goes back."
	notice.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; notice.add_theme_font_size_override("font_size", 14); notice.size_flags_horizontal = Control.SIZE_EXPAND_FILL; footer.add_child(notice)
	widgets.button(footer, "Return to game", request_close)
	_build_close_prompt()
	_build_loading_panel()
	parity_locked = widgets.lock_parity_rows(ctx.parity_active(), not ctx.parity_forced_by().is_empty())
	var restore_tab := page_titles.find(previous_title)
	_select_page(clampi(restore_tab if restore_tab >= 0 else previous_tab, 0, navigation.size() - 1))
	_restore_scroll(tabs, scroll_positions)
	_update_badges()
	_layout()
	_queue_layout()
	var restore: Control = focus_items.get(focus_key, navigation[tabs.current_tab])
	if not restore.is_visible_in_tree(): restore = navigation[tabs.current_tab]
	_focus_later(restore)

## Scroll offsets are restored once the rebuilt pages have a layout.
func _restore_scroll(host: TabContainer, positions: Dictionary) -> void:
	if positions.is_empty(): return
	await get_tree().process_frame
	if not is_instance_valid(host): return
	for child: Node in host.get_children():
		if child is ScrollContainer and positions.has(child.name): child.scroll_vertical = positions[child.name]

## Deferred focus that tolerates a second rebuild in the same frame.
func _focus_later(control: Control) -> void:
	(func() -> void:
		if is_instance_valid(control) and control.is_inside_tree() and control.is_visible_in_tree(): control.grab_focus()).call_deferred()

func _build_header() -> void:
	var header := HBoxContainer.new(); header.add_theme_constant_override("separation", 12); column.add_child(header)
	var title := Label.new(); title.text = "Settings"; title.add_theme_font_size_override("font_size", 26); header.add_child(title)
	var badges := VBoxContainer.new(); badges.size_flags_horizontal = Control.SIZE_EXPAND_FILL; header.add_child(badges)
	fidelity_badge = Label.new(); fidelity_badge.add_theme_font_size_override("font_size", 15); fidelity_badge.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; badges.add_child(fidelity_badge)
	language_badge = Label.new(); language_badge.add_theme_font_size_override("font_size", 14); language_badge.add_theme_color_override("font_color", Widgets.NOTE_COLOR); badges.add_child(language_badge)
	var hint := Label.new(); hint.text = "F10  ·  L3 + R3"; hint.add_theme_color_override("font_color", Widgets.NOTE_COLOR); header.add_child(hint)

func _build_preset_row() -> void:
	var row := HBoxContainer.new(); row.add_theme_constant_override("separation", 8); column.add_child(row)
	var label := Label.new(); label.text = "Preset"; label.custom_minimum_size.x = 64; row.add_child(label)
	for name: String in ["original", "modern", "custom"]:
		var item: Button = widgets.button(row, PRESET_NAMES[name], select_preset.bind(name), "preset:" + name)
		item.toggle_mode = true; item.size_flags_horizontal = Control.SIZE_EXPAND_FILL
		preset_buttons[name] = item

func _update_badges() -> void:
	var changes: Array[String] = ctx.fidelity_changes()
	var preset: String = ctx.current_preset()
	for name: String in preset_buttons: preset_buttons[name].set_pressed_no_signal(name == preset)
	if ctx.parity_active():
		var forced: String = ctx.parity_forced_by()
		fidelity_badge.text = "Parity mode: ON (original game, enhancements locked%s)" % ("; forced by " + forced if not forced.is_empty() else "")
		fidelity_badge.add_theme_color_override("font_color", Widgets.PARITY_COLOR)
		if not changes.is_empty(): fidelity_badge.text += " NOT CLEAN: " + ", ".join(changes)
	elif changes.is_empty():
		fidelity_badge.text = "Original fidelity: ON"
		fidelity_badge.add_theme_color_override("font_color", Color("9fd68f"))
	else:
		var names: Array[String] = []
		for dotted: String in changes: names.append(ctx.preset_label(dotted))
		fidelity_badge.text = "Original fidelity: OFF (changed: " + ", ".join(names) + ")"
		fidelity_badge.add_theme_color_override("font_color", Widgets.TAG_COLOR)
	language_badge.text = "Game language: " + ctx.language_label()

func select_preset(name: String) -> void:
	if name == "custom":
		set_notice("Custom is chosen automatically when you change an option."); _update_badges(); return
	var result: Dictionary = ctx.apply_preset(name)
	if not result.display.is_empty():
		draft = result.display
	elif runtime.preferences.pending.is_empty():
		# Core apply_preset writes its display keys (aspect, filter) live; the
		# draft must follow, or the next Apply would undo the preset.
		var saved: Dictionary = runtime.preferences.settings.display
		for key: String in ["aspect", "filter", "wing_fill", "wing_art", "wing_art_command", "wing_art_story", "wing_art_other", "bars", "ui_scale"]:
			if saved.has(key) and draft.has(key): draft[key] = saved[key]
	rebuild()
	# "Modern English" may start the asynchronous English switch.
	if ctx.language_loading(): _set_loading(true)
	if not result.display.is_empty(): preview_display()
	set_notice(result.error if not str(result.error).is_empty() else "%s preset applied." % PRESET_NAMES[name])
	_update_badges()

## T-39230: Game page play mode row. Entering Parity writes the original value
## of every locked key at once; the display draft follows the saved display.
func select_mode(name: String) -> void:
	var error: String = ctx.set_play_mode(name)
	if error.is_empty() and runtime.preferences.pending.is_empty():
		draft = runtime.preferences.settings.display.duplicate(true)
	rebuild()
	set_notice(error if not error.is_empty() else ("Parity mode: the original game. Enhancements are locked." if name == "parity" else "Modern mode: every option is unlocked."))

func page(title: String) -> VBoxContainer:
	var scroll := ScrollContainer.new(); scroll.name = title.replace("&", "and").replace(" ", "_"); tabs.add_child(scroll)
	scroll.horizontal_scroll_mode = ScrollContainer.SCROLL_MODE_DISABLED
	scroll.follow_focus = true
	var index := navigation.size()
	var category: Button = widgets.button(sidebar, title, func() -> void: _select_page(index), "page:" + title)
	category.toggle_mode = true; category.alignment = HORIZONTAL_ALIGNMENT_LEFT; navigation.append(category)
	page_titles.append(title)
	var box := VBoxContainer.new(); box.add_theme_constant_override("separation", 10); box.size_flags_horizontal = Control.SIZE_EXPAND_FILL; scroll.add_child(box)
	widgets.heading(box, title)
	return box

## Opens a file picker; `select` returns "" or an error to show.
func choose_file(filter: String, select: Callable) -> void:
	var dialog := FileDialog.new(); dialog.access = FileDialog.ACCESS_FILESYSTEM
	dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE; dialog.filters = PackedStringArray([filter])
	add_child(dialog)
	dialog.file_selected.connect(func(path: String) -> void:
		var error: String = select.call(path)
		if error != "": notice.text = error
		else: rebuild()
		dialog.queue_free())
	dialog.canceled.connect(dialog.queue_free)
	dialog.popup_centered_ratio(0.8)

func set_notice(text: String) -> void:
	if is_instance_valid(notice): notice.text = text
	if is_instance_valid(fidelity_badge): _update_badges()

## Called after a language choice; the loading state takes over while the
## bundle service opens packs asynchronously.
func rebuild_after_language() -> void:
	if ctx.language_loading(): _set_loading(true)
	else:
		var message := notice.text if is_instance_valid(notice) else ""
		rebuild()
		if message.begins_with("Loading"): message = "Game language: " + ctx.language_label() + "."
		set_notice(message)

func request_relayout() -> void:
	_layout(); _queue_layout()

# --- display preview (Keep / Revert) -----------------------------------------

func preview_display() -> void:
	# Keys the pages apply live are already saved; never preview a stale copy.
	var saved: Dictionary = runtime.preferences.settings.display
	for dotted: String in ctx.LIVE_DISPLAY_KEYS:
		var key := dotted.get_slice(".", 1)
		if saved.has(key) and draft.has(key): draft[key] = saved[key]
	if not runtime.preferences.begin_display(draft, Time.get_ticks_msec() / 1000.0, runtime.first_launch):
		notice.text = runtime.preferences.error; return
	had_preview = true
	confirmation.show(); _focus_later(revert_button)
	_update_badges()

## Kept for callers and tests written before the shell split.
func _preview_display() -> void:
	preview_display()

func _keep_display() -> void:
	if runtime.preferences.confirm_display():
		runtime.first_launch = false; had_preview = false; draft = runtime.preferences.settings.display.duplicate(true)
		rebuild(); notice.text = "Display settings saved."
	_finish_prompt()

func _revert_display() -> void:
	runtime.preferences.revert_display(); had_preview = false; draft = runtime.preferences.settings.display.duplicate(true)
	rebuild(); notice.text = "Display settings restored."
	_finish_prompt()

# --- Back / close --------------------------------------------------------------

## Closes the menu, or shows Keep/Revert first while a display preview is pending.
func request_close() -> void:
	if not runtime.preferences.pending.is_empty():
		_close_after_prompt = true
		close_prompt.show()
		_focus_later(focus_items["prompt:keep"])
		return
	if visible: runtime.toggle_menu()

## Back leaves the Advanced section or the page content first, then closes.
func go_back() -> void:
	if close_prompt.visible:
		close_prompt.hide(); _close_after_prompt = false
		navigation[tabs.current_tab].grab_focus(); return
	var focused := get_viewport().gui_get_focus_owner()
	if advanced_open and focused != null and tabs.is_ancestor_of(focused):
		advanced_open = false; rebuild(); _focus_later(focus_items.get("advanced_open", navigation[tabs.current_tab])); return
	if focused != null and (tabs.is_ancestor_of(focused) or confirmation.is_ancestor_of(focused)):
		navigation[tabs.current_tab].grab_focus(); return
	request_close()

func _build_close_prompt() -> void:
	close_prompt = PanelContainer.new(); close_prompt.hide(); add_child(close_prompt)
	var box := VBoxContainer.new(); box.add_theme_constant_override("separation", 10); close_prompt.add_child(box)
	var text := Label.new(); text.text = "Keep the new display settings before closing?"; text.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	text.custom_minimum_size.x = 320; box.add_child(text)
	var row := HBoxContainer.new(); row.add_theme_constant_override("separation", 10); box.add_child(row)
	widgets.button(row, "Keep display", _keep_display, "prompt:keep")
	widgets.button(row, "Revert", _revert_display, "prompt:revert")

func _finish_prompt() -> void:
	if is_instance_valid(close_prompt): close_prompt.hide()
	if _close_after_prompt:
		_close_after_prompt = false
		if visible: runtime.toggle_menu()

func _build_loading_panel() -> void:
	loading_panel = PanelContainer.new(); loading_panel.mouse_filter = Control.MOUSE_FILTER_STOP; add_child(loading_panel)
	var label := Label.new(); label.text = "Loading language packs…"; label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	label.custom_minimum_size = Vector2(320, 48); loading_panel.add_child(label)
	loading_panel.visible = _loading

func _set_loading(value: bool) -> void:
	_loading = value
	if is_instance_valid(loading_panel): loading_panel.visible = value
	if value: get_viewport().gui_release_focus()

func is_loading() -> bool:
	return _loading

func _on_visibility_changed() -> void:
	if not visible: return
	# Held input never leaks into the menu or back into the game.
	for action: String in ["ui_up", "ui_down", "ui_left", "ui_right", "ui_accept", "ui_cancel"]:
		if InputMap.has_action(action): Input.action_release(action)
	get_tree().call_group("native_input_devices", "clear_devices")
	_close_after_prompt = false
	closing_for_save = false
	if is_instance_valid(close_prompt): close_prompt.hide()

static func is_back_event(event: InputEvent) -> bool:
	if event is InputEventKey: return event.pressed and not event.echo and event.physical_keycode == KEY_ESCAPE
	if event is InputEventMouseButton: return event.pressed and event.button_index == MOUSE_BUTTON_RIGHT
	if event is InputEventJoypadButton: return event.pressed and event.is_action_pressed("ui_cancel")
	return false

# --- layout --------------------------------------------------------------------

func _select_page(index: int) -> void:
	tabs.current_tab = index
	for position: int in navigation.size(): navigation[position].set_pressed_no_signal(position == index)

## Layout runs at most once per idle step. Wrapped labels change the card's
## minimum size when its width changes, so a direct connection could loop.
## Window size the 1x card layout is designed for (the default window).
const UI_DESIGN_SIZE := Vector2(960, 720)

func _queue_layout() -> void:
	if _layout_queued: return
	_layout_queued = true
	(func() -> void: _layout_queued = false; _layout()).call_deferred()

func _layout() -> void:
	if not is_instance_valid(card): return
	var area := get_viewport_rect().size
	var factor := ui_factor(area)
	var logical := area / factor
	card.size = Vector2(minf(logical.x - 32, 940), minf(logical.y - 32, 700))
	card.scale = Vector2(factor, factor)
	card.position = (area - card.size * factor) / 2.0
	for overlay: Control in [close_prompt, loading_panel]:
		if not is_instance_valid(overlay): continue
		overlay.reset_size(); overlay.scale = Vector2(factor, factor)
		overlay.position = (area - overlay.size * factor) / 2.0

## Interface size from preferences, reduced only as far as needed for the
## settings card to fit the window. Independent of the native picture fit.
## Game scenes run the window unscaled (content_scale_size ZERO), so a large
## window would show the card at its 960x720 design size in a corner of the
## screen; the base factor keeps that proportion (up to 2x) before the
## preference multiplies it. Scenes on the project's 640x480 stretch have a
## 640x480 area and a base of 1.
func ui_factor(area: Vector2) -> float:
	var base := clampf(minf(area.x / UI_DESIGN_SIZE.x, area.y / UI_DESIGN_SIZE.y), 1.0, 2.0)
	var requested: float = base * (ctx.ui_scale() if ctx != null else 1.0)
	if requested <= 1.0 or not is_instance_valid(card): return 1.0
	var key := Vector3(requested, area.x, area.y)
	if key.is_equal_approx(_factor_key): return _factor
	# Measure the card at its widest layout so wrapping cannot feed back.
	var minimum := card.get_combined_minimum_size() + Vector2(32, 32)
	var fit := minf(area.x / maxf(minimum.x, 1.0), area.y / maxf(minimum.y, 1.0))
	_factor_key = key
	_factor = clampf(minf(requested, fit), 1.0, 2.0)
	return _factor

# --- values --------------------------------------------------------------------

## Save or load request from a page, through the runtime's guarded save host.
func request_save(load_game: bool) -> void:
	closing_for_save = true
	runtime.request_save(load_game)
	if visible: closing_for_save = false  # refused: the menu stays open

func save_value(section: String, key: String, value: Variant) -> void:
	if not runtime.preferences.set_value(section, key, value): notice.text = runtime.preferences.error
	_update_badges()

func set_bus(bus: String, key: String, value: Variant) -> void:
	var buses: Dictionary = runtime.preferences.settings.audio.buses.duplicate(true)
	buses[bus][key] = value; save_value("audio", "buses", buses)

func begin_capture(action: String) -> void:
	capture = action; notice.text = "Press a key, controller button, stick, or mouse button. F10 cancels."

func cancel_capture() -> void:
	capture = ""; notice.text = "Binding cancelled."

# --- input ---------------------------------------------------------------------

func _input(event: InputEvent) -> void:
	if event is InputEventJoypadButton and STICK_BITS.has(event.button_index):
		if _close_chord(event): accept_event(); return
	if not visible: return
	if _loading:
		accept_event(); return
	if not capture.is_empty():
		_capture_input(event); return
	if event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_F10 and not runtime.preferences.pending.is_empty():
		request_close(); accept_event(); return
	if is_back_event(event):
		go_back(); accept_event(); return
	if event.is_action_pressed("port_l1") or event.is_action_pressed("port_r1"):
		_select_page(posmod(tabs.current_tab + (-1 if event.is_action_pressed("port_l1") else 1), tabs.get_tab_count()))
		navigation[tabs.current_tab].grab_focus(); accept_event()

## True when this press completes L3+R3 while the menu shows a pending display
## preview: the close prompt opens instead of the runtime reverting silently.
func _close_chord(event: InputEventJoypadButton) -> bool:
	var held: int = _sticks.get(event.device, 0)
	var bit: int = STICK_BITS[event.button_index]
	held = (held | bit) if event.pressed else (held & ~bit)
	_sticks[event.device] = held
	if held == 0: _sticks_latched[event.device] = false
	if held != 3 or _sticks_latched.get(event.device, false): return false
	_sticks_latched[event.device] = true
	if not visible or _loading or not capture.is_empty() or runtime.preferences.pending.is_empty(): return false
	request_close()
	return true

func _capture_input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed and event.physical_keycode == KEY_F10:
		cancel_capture(); accept_event(); return
	var spec: Dictionary = runtime.Remaps.encode(event)
	if spec.is_empty(): return
	var result: Dictionary = runtime.remaps.rebind(capture, spec)
	if result.error != "": notice.text = result.error
	else:
		save_value("controls", "bindings", result.bindings); capture = ""; rebuild()
	accept_event()

func _process(_delta: float) -> void:
	if not visible or not is_instance_valid(confirmation): return
	var loading: bool = ctx.language_loading()
	if loading != _loading:
		_set_loading(loading)
		if not loading:
			rebuild(); set_notice("Game language: " + ctx.language_label() + ".")
	confirmation.visible = not runtime.preferences.pending.is_empty()
	if confirmation.visible:
		had_preview = true
		notice.text = "Keep this display? Restoring previous settings in %d seconds." % ceili(float(runtime.preferences.pending.deadline) - Time.get_ticks_msec() / 1000.0)
	elif had_preview:
		had_preview = false; draft = runtime.preferences.settings.display.duplicate(true)
		var closing := _close_after_prompt
		rebuild()
		notice.text = "Display settings restored."
		if closing: _close_after_prompt = true; _finish_prompt()
