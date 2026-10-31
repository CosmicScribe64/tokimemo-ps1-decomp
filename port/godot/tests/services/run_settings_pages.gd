extends "res://tests/services/settings_test_tree.gd"
## T-30002 WP7 + T-30001 WP5 menu entries. Part 1 runs the real ModernSettings
## autoload with the real settings-core v2 preferences and the real runtime
## wiring; no language bundle is installed in the test, so English reports a
## visible fallback, and the display frame service offers scene layout. Part 2
## runs the same pages on FakeRuntime: the real core preferences, presets,
## remaps and glyph service, with select_language, display_frame and the voice
## route faked (ui/settings/settings_context.gd lists every call). Fidelity: the UI never touches a
## native GameSession (state/RNG hash unchanged) and its sources never refer
## to native state.
const Session = preload("res://core/game_session.gd")
const Fakes = preload("res://tests/services/settings_v2_fakes.gd")
const Context = preload("res://ui/settings/settings_context.gd")
const UI_SOURCES := ["res://ui/settings_menu.gd", "res://ui/settings/settings_context.gd",
	"res://ui/settings/settings_widgets.gd", "res://ui/settings/language_page.gd",
	"res://ui/settings/display_page.gd", "res://ui/settings/audio_page.gd",
	"res://ui/settings/controls_page.gd", "res://ui/settings/accessibility_page.gd",
	"res://ui/settings/game_page.gd", "res://ui/pause_menu.gd", "res://ui/welcome_step.gd"]

var session: RefCounted

func _initialize() -> void: call_deferred("run")

func _process(delta: float) -> bool:
	_elapsed += delta
	if failures > 0: quit(1)
	elif _elapsed > 60.0:
		printerr("Settings pages test did not finish within 60 seconds"); quit(1)
	return false

func run() -> void:
	session = Session.new()
	var native_before: String = session.state_hash()
	_sources_never_touch_native_state()
	await _v1_pages(root.get_node("ModernSettings"))
	await _v2_pages()
	check(session.state_hash() == native_before)
	print("Settings pages: v1 fallbacks and absences, v2 contract (scene layout, side areas, interface size, text box, prompt style, drafts off, coverage, loading lock, presets), native session hash unchanged passed")
	finish()

func _fail(message: String) -> void:
	printerr(message); check(false)

func _sources_never_touch_native_state() -> void:
	for path: String in UI_SOURCES:
		if not ResourceLoader.exists(path): continue
		var source := FileAccess.get_file_as_string(path)
		for word: String in ["GameState", "game_session", "native_rng", "NativeCanvas", "native_canvas", ".rng.", "save_image"]:
			if source.contains(word): _fail(path + " refers to native state: " + word)

func _texts(control: Node) -> Array[String]:
	var result: Array[String] = []
	if control is Label: result.append(control.text)
	if control is Button: result.append(control.text)
	for child: Node in control.get_children(): result.append_array(_texts(child))
	return result

func _page_text(menu: Control, title: String) -> String:
	var index: int = menu.page_titles.find(title)
	return "\n".join(_texts(menu.tabs.get_child(index))) if index >= 0 else ""

func _v1_pages(runtime: Node) -> void:
	runtime.toggle_menu(); await process_frame
	var menu: Control = runtime.menu
	check(menu.page_titles == ["Language", "Display", "Audio", "Controls", "Text & Story", "Game"])
	var all := "\n".join(_texts(menu))
	for absent: String in ["Subtitle", "subtitle", "Colour", "Color", "colour filter"]:
		if all.contains(absent): _fail("v1: hidden option is visible: " + absent)
	# Language: one choice, visible fallback, coverage, fixed voice, Advanced.
	var language := _page_text(menu, "Language")
	check(language.contains("Japanese (Original)") and language.contains("Coverage:"))
	check(language.contains("Voice: Japanese (Original)"))
	check(language.contains("English is not installed in this build yet"))
	# The AI-translation disclosure and label are shown even before a bundle exists.
	var language_option: OptionButton = menu.focus_items["Game language"]
	check(language_option.get_item_text(1) == "English (AI translation)")
	check(language.contains(Context.DEFAULT_DISCLOSURE))
	# Core v2 has the draft key; drafts stay off by default.
	check(menu.focus_items.has("Draft English artwork (unreviewed)"))
	check(not (menu.focus_items["Draft English artwork (unreviewed)"] as CheckButton).button_pressed)
	check(not menu.focus_items.has("custom_text_layer"))
	var choice: OptionButton = menu.focus_items["Game language"]
	choice.select(1); choice.item_selected.emit(1); await process_frame
	check(menu.ctx.language_choice() == "ja")
	check(menu.notice.text.contains("Japanese is kept"))
	(menu.focus_items["advanced_open"] as Button).pressed.emit(); await process_frame
	for key: String in ["custom_images", "custom_dialogue", "custom_text_layer", "custom_voice"]: check(menu.focus_items.has(key))
	menu.advanced_open = false; menu.rebuild(); await process_frame
	# Display: scene layout and side areas need the display frame service,
	# which the runtime now owns (every native boot frames with
	# NativeDisplayHost); interface size is a core v2 preference.
	for present: String in ["Scene layout", "Side areas"]:
		if not menu.focus_items.has(present): _fail("v1: %s missing with the display frame service" % present)
	check(menu.focus_items.has("Interface size"))
	check(_page_text(menu, "Display").contains("Drawing area"))
	var sizes: OptionButton = menu.focus_items["Window size"]
	check(sizes.get_item_text(0) == "Desktop resolution")
	# Controls: legend, confirm side, per-device reset keeps the other device.
	var controls := _page_text(menu, "Controls")
	check(controls.contains("PS1 Circle") and controls.contains("Confirm: bottom face button"))
	check(menu.focus_items.has("Prompt style"))
	var remaps: RefCounted = runtime.remaps
	var keyed: Dictionary = remaps.rebind("left", {"kind": "key", "code": KEY_H})
	check(keyed.error.is_empty() and runtime.preferences.set_value("controls", "bindings", keyed.bindings))
	var buttoned: Dictionary = remaps.rebind("left", {"kind": "button", "code": JOY_BUTTON_PADDLE1})
	check(buttoned.error.is_empty() and runtime.preferences.set_value("controls", "bindings", buttoned.bindings))
	(menu.focus_items["reset_keyboard"] as Button).pressed.emit(); await process_frame
	var left: Array = runtime.preferences.settings.controls.bindings.left
	check(left.has({"kind": "key", "code": KEY_LEFT}) and not left.has({"kind": "key", "code": KEY_H}))
	check(left.has({"kind": "button", "code": JOY_BUTTON_PADDLE1}))
	# A custom key binding must survive the controller reset.
	keyed = remaps.rebind("left", {"kind": "key", "code": KEY_H})
	check(keyed.error.is_empty() and runtime.preferences.set_value("controls", "bindings", keyed.bindings))
	check(runtime.preferences.settings.controls.bindings.left.has({"kind": "key", "code": KEY_H}))
	(menu.focus_items["reset_controller"] as Button).pressed.emit(); await process_frame
	left = runtime.preferences.settings.controls.bindings.left
	check(left.has({"kind": "button", "code": JOY_BUTTON_DPAD_LEFT}) and not left.has({"kind": "button", "code": JOY_BUTTON_PADDLE1}))
	check(left.has({"kind": "key", "code": KEY_H}))
	# Text & Story: scope tags until WP9; core v2 has text box and auto-advance.
	var story := _page_text(menu, "Text & Story")
	check(story.contains("[opening only]"))
	# No shake consumer exists yet, not even in the opening.
	check(story.contains("[not wired yet]") and menu.ctx.scope_tag("reduce_shake") == "not wired yet")
	check(menu.focus_items.has("Dialogue text box") and menu.focus_items.has("Auto-advance"))
	check(not _page_text(menu, "Audio").contains("In-game voice option"))
	runtime.toggle_menu(); await process_frame

func _v2_pages() -> void:
	var fake := Fakes.FakeRuntime.new(); fake.name = "FakeSettingsRuntime"; root.add_child(fake)
	await process_frame
	fake.toggle_menu(); await process_frame
	var menu: Control = fake.menu
	check(menu.visible)
	check(menu.fidelity_badge.text == "Original fidelity: ON")
	# Display: capability-gated layout, live side areas, interface size.
	for present: String in ["Scene layout", "Side areas", "Interface size"]:
		if not menu.focus_items.has(present): _fail("v2: missing " + present)
	check(_page_text(menu, "Display").contains("Scene art is never stretched or invented"))
	check(_page_text(menu, "Display").contains("Drawing area 1920×1080, game picture ×4"))
	var sizes: OptionButton = menu.focus_items["Window size"]
	check(sizes.get_item_text(0) == "Desktop resolution")
	var side: OptionButton = menu.focus_items["Side areas"]
	side.select(1); side.item_selected.emit(1); await process_frame
	check(fake.preferences.settings.display.wing_fill == "source" and fake.preferences.pending.is_empty())
	check(menu.fidelity_badge.text.contains("Side areas") and not menu.fidelity_badge.text.contains("Wing Fill"))
	# T-39220 Side art: live, one-line explanation per option, preview, badge.
	check(menu.focus_items.has("Side art"))
	var art: OptionButton = menu.focus_items["Side art"]
	check(art.item_count == 13)
	for index: int in art.item_count: check(not art.get_item_tooltip(index).is_empty())
	check(_page_text(menu, "Display").contains("Off: the side areas stay black"))
	art.select(1); art.item_selected.emit(1); await process_frame
	check(fake.preferences.settings.display.wing_art == "seasonal" and fake.preferences.pending.is_empty())
	check(_page_text(menu, "Display").contains("Seasonal: follows the in-game month"))
	# No scene feed reports the month yet: Seasonal and Match say so.
	check(_page_text(menu, "Display").contains("Not active yet in this build"))
	check(_page_text(menu, "Display").contains("wing-art-overrides.json"))
	for title: String in ["Side art: command screens", "Side art: dates and talks", "Side art: other scenes"]:
		check(menu.focus_items.has(title))
	var per_type: OptionButton = menu.focus_items["Side art: command screens"]
	check(per_type.item_count == 14 and per_type.get_item_text(0) == "Same as Side art")
	for index: int in per_type.item_count: check(not per_type.get_item_tooltip(index).is_empty())
	per_type.select(9); per_type.item_selected.emit(9); await process_frame
	check(fake.preferences.settings.display.wing_art_command == "fixed:school" and fake.preferences.pending.is_empty())
	var location_index := 12  # fixed:legend_tree in the Side art list
	art.select(location_index); art.item_selected.emit(location_index); await process_frame
	check(fake.preferences.settings.display.wing_art == "fixed:legend_tree")
	check(_page_text(menu, "Display").contains("Legendary tree:"))
	art.select(1); art.item_selected.emit(1); await process_frame
	check(menu.fidelity_badge.text.contains("Side art"))
	check(menu.ctx.set_pref("display.wing_art", "fixed:bogus") != "")
	art.select(0); art.item_selected.emit(0); await process_frame
	check(fake.preferences.settings.display.wing_art == "off")
	# Every preset key has a player-facing badge name.
	var context_constants: Dictionary = (load("res://ui/settings/settings_context.gd") as Script).get_script_constant_map()
	for dotted: String in context_constants.PRESET_KEYS: check(context_constants.PRESET_LABELS.has(dotted))
	check(menu.ctx.set_pref("display.wing_fill", "blur") != "")
	check(fake.preferences.settings.display.wing_fill == "source")
	# Apply display changes never previews a stale copy of a live key.
	check(menu.draft.wing_fill == "black")
	menu.draft.filter = "nearest"; menu.preview_display(); await process_frame
	check(fake.preferences.pending.candidate.wing_fill == "source" and fake.preferences.pending.candidate.filter == "nearest")
	(menu.focus_items["Revert"] as Button).pressed.emit(); await process_frame
	check(fake.preferences.settings.display.wing_fill == "source" and fake.preferences.settings.display.filter == "integer")
	check(menu.ctx.set_pref("display.filter", "linear") != "")
	(menu.focus_items["Interface size"] as HSlider).value = 2.0; await process_frame; await process_frame
	var area: Vector2 = menu.get_viewport_rect().size
	var factor: float = menu.card.scale.x
	check(factor > 1.0 and factor <= 2.0 and is_equal_approx(factor, menu.ui_factor(area)))
	# The scaled card always fits the window; the native picture is untouched.
	check(menu.card.position.x >= -0.5 and menu.card.position.x + menu.card.size.x * factor <= area.x + 0.5)
	check(menu.card.position.y >= -0.5 and menu.card.position.y + menu.card.size.y * factor <= area.y + 0.5)
	(menu.focus_items["Interface size"] as HSlider).value = 1.0; await process_frame
	check(is_equal_approx(menu.card.scale.x, 1.0))
	# Text & Story: modern text box applies live, auto-advance offered.
	check(menu.focus_items.has("Auto-advance"))
	var box: OptionButton = menu.focus_items["Dialogue text box"]
	check(box.get_item_text(box.selected) == "Original text window")
	box.select(1); box.item_selected.emit(1); await process_frame
	check(fake.preferences.settings.accessibility.text_box == "modern" and fake.preferences.pending.is_empty())
	check(fake.preferences.settings.profile.preset == "custom" and menu.preset_buttons["custom"].button_pressed)
	# Controls: prompt style drives glyph-service text names; per-device reset.
	check(menu.focus_items.has("Prompt style"))
	var style: OptionButton = menu.focus_items["Prompt style"]
	style.select(3); style.item_selected.emit(3); await process_frame
	check(fake.preferences.settings.controls.prompt_style == "playstation")
	check(_page_text(menu, "Controls").contains("Cross"))
	# Per-device reset goes through core InputRemapService.reset_device.
	var paddle: Dictionary = fake.remaps.rebind("left", {"kind": "button", "code": JOY_BUTTON_PADDLE1})
	check(paddle.error.is_empty() and fake.preferences.set_value("controls", "bindings", paddle.bindings))
	(menu.focus_items["reset_controller"] as Button).pressed.emit(); await process_frame
	check(not fake.preferences.settings.controls.bindings.has("left"))
	check(_page_text(menu, "Audio").contains("In-game voice option: On"))
	# Language: drafts off by default, coverage counts, loading lock.
	var drafts: CheckButton = menu.focus_items["Draft English artwork (unreviewed)"]
	check(not drafts.button_pressed and not fake.preferences.settings.language.allow_draft_images)
	check(_page_text(menu, "Language").contains("44 reviewed images; 173 images stay original Japanese"))
	check(_page_text(menu, "Language").contains("120 approved text records (90 drawable with the original font), 40 dialogue lines"))
	check(not _page_text(menu, "Language").contains("English is not installed"))
	# Design WP-E: the label and disclosure sit under the selector; an applied
	# bundle's own disclosure replaces the default wording.
	check((menu.focus_items["Game language"] as OptionButton).get_item_text(1) == "English (AI translation)")
	check(_page_text(menu, "Language").contains(Context.DEFAULT_DISCLOSURE))
	fake.language_bundle.disclosure = Fakes.BUNDLE_DISCLOSURE
	menu.rebuild(); await process_frame
	check(_page_text(menu, "Language").contains(Fakes.BUNDLE_DISCLOSURE))
	check(not _page_text(menu, "Language").contains(Context.DEFAULT_DISCLOSURE))
	menu._select_page(menu.page_titles.find("Language"))
	var choice: OptionButton = menu.focus_items["Game language"]
	choice.select(1); choice.item_selected.emit(1); await process_frame
	check(menu.is_loading() and menu.loading_panel.visible)
	var tab_before: int = menu.tabs.current_tab
	var escape := InputEventKey.new(); escape.physical_keycode = KEY_ESCAPE; escape.pressed = true
	root.push_input(escape)
	var right := InputEventAction.new(); right.action = "port_r1"; right.pressed = true
	root.push_input(right); await process_frame
	check(menu.visible and menu.tabs.current_tab == tab_before)
	fake.language_loading = false; await process_frame; await process_frame
	check(not menu.is_loading() and not menu.loading_panel.visible)
	check(menu.language_badge.text == "Game language: English (AI translation)")
	# Presets through the core API; Original keeps the language.
	menu.select_preset("original"); await process_frame
	check(fake.preferences.presets_applied.back() == "original")
	check(menu.fidelity_badge.text == "Original fidelity: ON")
	check(menu.language_badge.text == "Game language: English (AI translation)")
	menu.select_preset("modern"); await process_frame
	check(menu.fidelity_badge.text.begins_with("Original fidelity: OFF") and menu.preset_buttons["modern"].button_pressed)
	# Core writes the preset's display keys live; the draft follows them, so a
	# later Apply keeps the preset instead of undoing it.
	check(fake.preferences.settings.display.aspect == "auto" and fake.preferences.settings.display.filter == "nearest")
	check(menu.draft.aspect == "auto" and menu.draft.filter == "nearest")
	check(fake.preferences.settings.profile.preset == "modern")
	# Core presets never own the language: from Japanese, "Modern English"
	# requests English through the runtime and shows the loading lock.
	menu.select_preset("original"); await process_frame
	check(fake.preferences.set_value("language", "bundle", "ja"))
	menu.select_preset("modern"); await process_frame
	check(fake.preferences.settings.language.bundle == "en" and fake.language_loading and menu.is_loading())
	fake.language_loading = false; await process_frame; await process_frame
	check(not menu.is_loading() and menu.language_badge.text == "Game language: English (AI translation)")
	fake.toggle_menu(); await process_frame
	check(not menu.visible)
	fake.queue_free(); await process_frame
