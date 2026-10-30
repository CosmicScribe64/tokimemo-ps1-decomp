extends Control
## Always-available desktop settings overlay, including a reserved F10 recovery key.
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

func _ready() -> void:
	set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	mouse_filter = Control.MOUSE_FILTER_STOP
	theme = _settings_theme()
	resized.connect(_layout)

func rebuild() -> void:
	var previous_tab := tabs.current_tab if is_instance_valid(tabs) else 0
	var focused := get_viewport().gui_get_focus_owner()
	var focus_key: String = str(focused.get_meta("settings_focus_key", "")) if focused != null else ""
	var scroll_positions := {}
	if is_instance_valid(tabs):
		for child: Node in tabs.get_children():
			if child is ScrollContainer: scroll_positions[child.name] = child.scroll_vertical
	var prior_draft: Dictionary = draft.duplicate(true) if visible and not draft.is_empty() else runtime.preferences.effective_settings().display.duplicate(true)
	for child: Node in get_children(): remove_child(child); child.queue_free()
	navigation.clear(); focus_items.clear()
	var shade := ColorRect.new(); shade.color = Color(0.01,0.025,0.025,0.82); shade.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT); add_child(shade)
	card = PanelContainer.new(); add_child(card)
	card.minimum_size_changed.connect(_layout)
	column = VBoxContainer.new(); column.add_theme_constant_override("separation",12); card.add_child(column)
	var header := HBoxContainer.new(); column.add_child(header)
	var title := Label.new(); title.text = "Settings"; title.add_theme_font_size_override("font_size",26); title.size_flags_horizontal = Control.SIZE_EXPAND_FILL; header.add_child(title)
	var hint := Label.new(); hint.text = "F10  ·  L3 + R3"; hint.add_theme_color_override("font_color",Color("a7bbb2")); header.add_child(hint)
	column.add_child(HSeparator.new())
	var body := HBoxContainer.new(); body.add_theme_constant_override("separation",16); body.size_flags_vertical = Control.SIZE_EXPAND_FILL; column.add_child(body)
	sidebar = VBoxContainer.new(); sidebar.custom_minimum_size.x = 128; sidebar.add_theme_constant_override("separation",6); body.add_child(sidebar)
	tabs = TabContainer.new(); tabs.tabs_visible = false; tabs.size_flags_horizontal = Control.SIZE_EXPAND_FILL; tabs.size_flags_vertical = Control.SIZE_EXPAND_FILL; body.add_child(tabs)
	draft = prior_draft
	var display := page("Display")
	option(display, "Window mode", ["windowed", "borderless", "exclusive"], draft.mode, func(v): draft.mode = v)
	var monitors: Array = []
	for index: int in maxi(DisplayServer.get_screen_count(), 1): monitors.append(index)
	option(display, "Monitor", monitors, draft.monitor, func(v): draft.monitor = v)
	option(display, "Resolution", ["960×720", "1280×720", "1920×1080", "2560×1440"], "%d×%d" % [draft.width, draft.height], func(v): var parts = v.split("×"); draft.width = int(parts[0]); draft.height = int(parts[1]))
	option(display, "Scaling", ["integer", "nearest", "linear"], draft.filter, func(v): draft.filter = v)
	option(display, "Aspect layout", ["original", "wide", "auto"], draft.aspect, func(v): draft.aspect = v)
	option(display, "Frame limit", [60, 120, 144, 0], draft.fps, func(v): draft.fps = v)
	toggle(display, "Vertical sync", draft.vsync, func(v): draft.vsync = v)
	toggle(display, "Long display confirmation (60 seconds)", draft.long_confirmation, func(v): draft.long_confirmation = v)
	button(display, "Apply display changes", _preview_display)
	confirmation = HBoxContainer.new()
	button(confirmation, "Keep display", func():
		if runtime.preferences.confirm_display():
			runtime.first_launch = false; had_preview = false; draft = runtime.preferences.settings.display.duplicate(true)
			rebuild(); notice.text = "Display settings saved.")
	revert_button = button(confirmation, "Revert", func(): runtime.preferences.revert_display(); had_preview = false; draft = runtime.preferences.settings.display.duplicate(true); rebuild(); notice.text = "Display settings restored.")
	var audio := page("Audio")
	for bus: String in runtime.Preferences.BUSES:
		var spec: Dictionary = runtime.preferences.settings.audio.buses[bus]
		audio_bus(audio, bus, spec)
	option(audio, "Output device", Array(AudioServer.get_output_device_list()), runtime.preferences.settings.audio.device, func(v): save_value("audio", "device", v))
	toggle(audio, "Mute in background", runtime.preferences.settings.audio.mute_unfocused, func(v): save_value("audio", "mute_unfocused", v))
	toggle(audio, "Mono output", runtime.preferences.settings.audio.mono, func(v): save_value("audio", "mono", v))
	var controls := page("Controls")
	toggle(controls, "Swap confirm and cancel", runtime.preferences.settings.controls.swap_confirm, func(v): save_value("controls", "swap_confirm", v))
	slider(controls, "Stick deadzone", runtime.preferences.settings.controls.deadzone, 0.05, 0.9, 0.05, func(v): save_value("controls", "deadzone", v))
	slider(controls, "Menu repeat delay", runtime.preferences.settings.controls.repeat_delay, 0.15, 1.0, 0.05, func(v): save_value("controls", "repeat_delay", v))
	slider(controls, "Menu repeat interval", runtime.preferences.settings.controls.repeat_interval, 0.03, 0.3, 0.01, func(v): save_value("controls", "repeat_interval", v))
	for action: String in runtime.Remaps.ACTIONS:
		var item := button(controls, action.capitalize() + ": " + runtime.remaps.prompt(action) + " / " + runtime.remaps.prompt(action, true), func(): begin_capture(action))
		item.name = action
		_register_focus(item,"binding:"+action)
	button(controls, "Reset controls", func(): runtime.preferences.restore_category("controls"); rebuild())
	var accessible := page("Accessibility")
	var access: Dictionary = runtime.preferences.settings.accessibility
	option(accessible, "Text reveal speed", ["slow", "normal", "fast", "instant"], access.text_speed, func(v): save_value("accessibility", "text_speed", v))
	slider(accessible, "Extra reading time after voice (seconds)", access.auto_delay, 0.0, 10.0, 0.5, func(v): save_value("accessibility", "auto_delay", v))
	option(accessible, "Prologue skip (Ctrl / remappable)", ["read", "all"], access.skip, func(v): save_value("accessibility", "skip", v))
	var skip_help := Label.new(); skip_help.text = "Read mode skips only after every prologue line has been read."; skip_help.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; accessible.add_child(skip_help)
	slider(accessible, "Dialogue text scale", access.font_scale, 1.0, 1.5, 0.1, func(v): save_value("accessibility", "font_scale", v))
	slider(accessible, "Dialogue backdrop opacity", access.backdrop, 0.0, 1.0, 0.1, func(v): save_value("accessibility", "backdrop", v))
	toggle(accessible, "Reduce screen flashes", access.reduce_flashes, func(v): save_value("accessibility", "reduce_flashes", v))
	var language := page("Language")
	var active := Label.new(); active.text = "Image language: " + runtime.packs.language; language.add_child(active)
	button(language, "Japanese images (Original)", func(): runtime.select_image_pack(""); rebuild())
	button(language, "Choose translated image pack…", choose_images)
	var text_language := Label.new(); text_language.text = "Dialogue language: " + runtime.text_pack.language; language.add_child(text_language)
	if not runtime.text_pack.unsupported_entries.is_empty():
		var fallback := Label.new(); fallback.text = "%d lines contain unsupported characters and will use Japanese." % runtime.text_pack.unsupported_entries.size()
		fallback.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; language.add_child(fallback)
	button(language, "Japanese dialogue (Original)", func(): runtime.select_text_pack(""); rebuild())
	button(language, "Choose dialogue translation…", choose_text)
	var voice_language := Label.new(); voice_language.text = "Voice language: " + runtime.voice_pack.language; language.add_child(voice_language)
	button(language, "Japanese voice (Original)", func(): runtime.select_voice_pack(""); rebuild())
	button(language, "Choose voice pack…", choose_voice)
	if runtime.save_host() != null:
		var game := page("Game")
		var help := Label.new(); help.text = "Quick save is available at command selection or the first result. Load restores the quick slot. Release game controls before using either action."
		help.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; game.add_child(help)
		button(game,"Quick save",func(): runtime.request_save(false))
		button(game,"Load quick save",func(): runtime.request_save(true))
	column.add_child(HSeparator.new())
	column.add_child(confirmation); confirmation.visible = not runtime.preferences.pending.is_empty()
	var footer := HBoxContainer.new(); footer.add_theme_constant_override("separation",16); column.add_child(footer)
	notice = Label.new(); notice.text = "Display changes need confirmation. Other settings save automatically."; notice.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; notice.add_theme_font_size_override("font_size",13); notice.size_flags_horizontal = Control.SIZE_EXPAND_FILL; footer.add_child(notice)
	button(footer, "Return to game", func(): runtime.toggle_menu())
	_select_page(clampi(previous_tab,0,navigation.size()-1))
	for child: Node in tabs.get_children():
		if child is ScrollContainer: child.set_deferred("scroll_vertical",scroll_positions.get(child.name,0))
	_layout()
	call_deferred("_layout")
	var restore: Control = focus_items.get(focus_key, navigation[tabs.current_tab])
	if not restore.is_visible_in_tree(): restore = navigation[tabs.current_tab]
	restore.call_deferred("grab_focus")

func choose_images() -> void:
	var dialog := FileDialog.new()
	dialog.access = FileDialog.ACCESS_FILESYSTEM
	dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
	dialog.filters = PackedStringArray(["*.json ; Translation manifest"])
	add_child(dialog)
	dialog.file_selected.connect(func(path: String):
		var error: String = runtime.select_image_pack(path)
		if error != "": notice.text = error
		else: rebuild()
		dialog.queue_free())
	dialog.canceled.connect(dialog.queue_free)
	dialog.popup_centered_ratio(0.8)

func choose_voice() -> void:
	var dialog := FileDialog.new(); dialog.access = FileDialog.ACCESS_FILESYSTEM
	dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE; dialog.filters = PackedStringArray(["*.json ; Voice manifest"])
	add_child(dialog)
	dialog.file_selected.connect(func(path: String):
		var error: String = runtime.select_voice_pack(path)
		if error != "": notice.text = error
		else: rebuild()
		dialog.queue_free())
	dialog.canceled.connect(dialog.queue_free); dialog.popup_centered_ratio(0.8)

func choose_text() -> void:
	var dialog := FileDialog.new(); dialog.access = FileDialog.ACCESS_FILESYSTEM
	dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE; dialog.filters = PackedStringArray(["*.json ; Text translation manifest"])
	add_child(dialog)
	dialog.file_selected.connect(func(path: String):
		var error: String = runtime.select_text_pack(path)
		if error != "": notice.text = error
		else: rebuild()
		dialog.queue_free())
	dialog.canceled.connect(dialog.queue_free); dialog.popup_centered_ratio(0.8)

func page(title: String) -> VBoxContainer:
	var scroll := ScrollContainer.new(); scroll.name = title; tabs.add_child(scroll)
	scroll.horizontal_scroll_mode = ScrollContainer.SCROLL_MODE_DISABLED
	scroll.follow_focus = true
	var index := navigation.size()
	var category := button(sidebar,title,func(): _select_page(index))
	category.toggle_mode = true; category.alignment = HORIZONTAL_ALIGNMENT_LEFT; navigation.append(category)
	var box := VBoxContainer.new(); box.add_theme_constant_override("separation",10); box.size_flags_horizontal = Control.SIZE_EXPAND_FILL; scroll.add_child(box)
	var heading := Label.new(); heading.text = title; heading.add_theme_font_size_override("font_size",21); heading.add_theme_color_override("font_color",Color("e9c77d")); box.add_child(heading)
	return box

func button(parent: Node, title: String, callback: Callable) -> Button:
	var item := Button.new(); item.text = title; item.custom_minimum_size.y = 34; parent.add_child(item); item.pressed.connect(callback); _register_focus(item,title); return item

func option(parent: Node, title: String, choices: Array, selected: Variant, callback: Callable) -> void:
	var row := HBoxContainer.new(); row.add_theme_constant_override("separation",10); parent.add_child(row)
	var label := Label.new(); label.text = title; label.size_flags_horizontal = Control.SIZE_EXPAND_FILL; label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; row.add_child(label)
	var item := OptionButton.new(); item.custom_minimum_size = Vector2(170,34); row.add_child(item)
	var available := choices.duplicate()
	if selected not in available: available.append(selected)
	var names := {"original":"Original (4:3)", "wide":"Widescreen", "auto":"Automatic", "windowed":"Windowed", "borderless":"Borderless fullscreen", "exclusive":"Exclusive fullscreen", "integer":"Integer · nearest", "nearest":"Nearest fit", "linear":"Smooth fit", "read":"Read only", "all":"All text"}
	for choice: Variant in available:
		var text := str(names.get(choice,str(choice).capitalize()))
		if title == "Frame limit": text = "Unlimited" if choice == 0 else "%d FPS" % choice
		if title == "Monitor": text = "Display %d" % (int(choice)+1)
		item.add_item(text)
	item.select(available.find(selected)); item.item_selected.connect(func(i): callback.call(available[i])); _register_focus(item,title)

func toggle(parent: Node, title: String, value: bool, callback: Callable) -> void:
	var item := CheckButton.new(); item.text = title; item.button_pressed = value; item.custom_minimum_size.y = 34; parent.add_child(item); item.toggled.connect(callback); _register_focus(item,title)

func slider(parent: Node, title: String, value: float, low: float, high: float, step: float, callback: Callable) -> void:
	var block := VBoxContainer.new(); block.add_theme_constant_override("separation",3); parent.add_child(block)
	var row := HBoxContainer.new(); block.add_child(row)
	var label := Label.new(); label.text = title; label.size_flags_horizontal = Control.SIZE_EXPAND_FILL; label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART; row.add_child(label)
	var number := Label.new(); number.add_theme_color_override("font_color",Color("e9c77d")); number.text = _value_text(title,value); number.horizontal_alignment = HORIZONTAL_ALIGNMENT_RIGHT; number.custom_minimum_size.x = 60; row.add_child(number)
	var item := HSlider.new(); item.custom_minimum_size.y = 22; item.min_value = low; item.max_value = high; item.step = step; item.value = value; block.add_child(item)
	item.value_changed.connect(func(v): number.text = _value_text(title,v); callback.call(v)); _register_focus(item,title)

func audio_bus(parent: Node, bus: String, spec: Dictionary) -> void:
	var row := HBoxContainer.new(); row.add_theme_constant_override("separation",10); parent.add_child(row)
	var volume := VBoxContainer.new(); volume.size_flags_horizontal = Control.SIZE_EXPAND_FILL; row.add_child(volume)
	slider(volume,bus,spec.volume,0.0,1.0,0.01,func(v): set_bus(bus,"volume",v))
	var mute := CheckButton.new(); mute.text = "Mute"; mute.button_pressed = spec.mute; row.add_child(mute)
	mute.toggled.connect(func(v): set_bus(bus,"mute",v)); _register_focus(mute,bus+" mute")
	var preview := button(row,"Test",func(): runtime.preview_audio(bus)); _register_focus(preview,bus+" preview")

func _value_text(title: String, value: float) -> String:
	if title.contains("repeat") or title.contains("reading"): return "%.2f s" % value
	return "%d%%" % roundi(value * 100.0)

func _register_focus(control: Control, key: String) -> void:
	control.set_meta("settings_focus_key",key); focus_items[key] = control

func _select_page(index: int) -> void:
	tabs.current_tab = index
	for position: int in navigation.size(): navigation[position].set_pressed_no_signal(position == index)

func _preview_display() -> void:
	if not runtime.preferences.begin_display(draft,Time.get_ticks_msec()/1000.0,runtime.first_launch): notice.text = runtime.preferences.error; return
	had_preview = true
	confirmation.show(); revert_button.call_deferred("grab_focus")

func _layout() -> void:
	if not is_instance_valid(card): return
	var area := get_viewport_rect().size
	card.size = Vector2(minf(area.x-32,940),minf(area.y-32,700))
	card.position = (area-card.size)/2.0

func _settings_theme() -> Theme:
	var result := Theme.new(); result.default_font_size = 16
	result.set_color("font_color","Label",Color("e4eee3"))
	var panel := _style(Color("102b29"),Color("698c6b")); panel.content_margin_left = 18; panel.content_margin_right = 18; panel.content_margin_top = 14; panel.content_margin_bottom = 14
	result.set_stylebox("panel","PanelContainer",panel)
	result.set_stylebox("panel","TabContainer",StyleBoxEmpty.new())
	for type: String in ["Button","OptionButton"]:
		result.set_stylebox("normal",type,_style(Color("1b3c36"),Color("456452")))
		result.set_stylebox("hover",type,_style(Color("285348"),Color("8aa478")))
		result.set_stylebox("pressed",type,_style(Color("315b43"),Color("d7b565")))
		var focus := _style(Color(0,0,0,0),Color("f2cb75")); result.set_stylebox("focus",type,focus)
		result.set_color("font_color",type,Color("e4eee3")); result.set_color("font_pressed_color",type,Color("ffe3a0")); result.set_color("font_hover_color",type,Color.WHITE)
	return result

func _style(background: Color, border: Color) -> StyleBoxFlat:
	var style := StyleBoxFlat.new(); style.bg_color = background; style.border_color = border
	style.set_border_width_all(1); style.set_corner_radius_all(5)
	style.content_margin_left = 10; style.content_margin_right = 10; style.content_margin_top = 6; style.content_margin_bottom = 6
	return style

func save_value(section: String, key: String, value: Variant) -> void:
	if not runtime.preferences.set_value(section, key, value): notice.text = runtime.preferences.error

func set_bus(bus: String, key: String, value: Variant) -> void:
	var buses: Dictionary = runtime.preferences.settings.audio.buses.duplicate(true)
	buses[bus][key] = value; save_value("audio", "buses", buses)

func begin_capture(action: String) -> void:
	capture = action; notice.text = "Press a key, controller button, stick, or mouse button. F10 cancels."

func cancel_capture() -> void:
	capture = ""; notice.text = "Binding cancelled."

func _input(event: InputEvent) -> void:
	if not visible: return
	if capture.is_empty():
		if event.is_action_pressed("port_l1") or event.is_action_pressed("port_r1"):
			_select_page(posmod(tabs.current_tab + (-1 if event.is_action_pressed("port_l1") else 1), tabs.get_tab_count()))
			navigation[tabs.current_tab].grab_focus(); accept_event()
		return
	if event is InputEventKey and event.pressed and event.physical_keycode == KEY_F10:
		capture = ""; notice.text = "Binding cancelled."; accept_event(); return
	var spec: Dictionary = runtime.Remaps.encode(event)
	if spec.is_empty(): return
	var result: Dictionary = runtime.remaps.rebind(capture, spec)
	if result.error != "": notice.text = result.error
	else:
		save_value("controls", "bindings", result.bindings); capture = ""; rebuild()
	accept_event()

func _process(_delta: float) -> void:
	if not visible or not is_instance_valid(confirmation): return
	confirmation.visible = not runtime.preferences.pending.is_empty()
	if confirmation.visible:
		had_preview = true
		notice.text = "Keep this display? Restoring previous settings in %d seconds." % ceili(float(runtime.preferences.pending.deadline) - Time.get_ticks_msec() / 1000.0)
	elif had_preview:
		had_preview = false; draft = runtime.preferences.settings.display.duplicate(true)
		rebuild()
		notice.text = "Display settings restored."
