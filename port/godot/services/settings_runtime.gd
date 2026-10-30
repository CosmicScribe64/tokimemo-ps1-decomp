extends Node
## Desktop-only settings. Native save data and simulation state stay independent.
signal preferences_changed(settings: Dictionary)
const Preferences = preload("res://services/user_preferences_service.gd")
const Remaps = preload("res://services/input_remap_service.gd")
const Menu = preload("res://ui/settings_menu.gd")
const Packs = preload("res://services/translation_pack_service.gd")
const Accessibility = preload("res://services/accessibility_service.gd")
const Pacing = preload("res://services/presentation_pacing.gd")
const History = preload("res://services/read_history_service.gd")
const PrologueText = preload("res://services/prologue_text_store.gd")
const VoicePack = preload("res://services/voice_pack_store.gd")
const TextPack = preload("res://services/text_translation_store.gd")
var text_pack := TextPack.new()
var voice_pack := VoicePack.new()
var read_history := History.new()
var pacing := Pacing.new()
var accessibility := Accessibility.new()
var packs := Packs.new()
var translated_images: RefCounted = null
## T-39016: active source-keyed text layer, or null for original text.
var translated_text: RefCounted = null
var mono_effect := AudioEffectStereoEnhance.new()
var preferences := Preferences.new()
var remaps := Remaps.new()
var menu: Control
var layer: CanvasLayer
var focused := true
var window_focused := true
var application_focused := true
var first_launch := false
var previous_pause := false
var applied_display: Dictionary = {}
var repeat_action := ""
var repeat_elapsed := 0.0
var settings_buttons: Dictionary = {}
var chord_latched: Dictionary = {}

func _ready() -> void:
	process_mode = Node.PROCESS_MODE_ALWAYS
	first_launch = not FileAccess.file_exists(preferences.path)
	preferences.load_preferences()
	read_history.load_history()
	Input.joy_connection_changed.connect(_gamepad_connection_changed)
	for bus: String in Preferences.BUSES:
		if AudioServer.get_bus_index(bus) < 0:
			AudioServer.add_bus(); AudioServer.set_bus_name(AudioServer.bus_count - 1, bus)
			AudioServer.set_bus_send(AudioServer.bus_count - 1, "Master")
	preferences.changed.connect(apply_preferences)
	mono_effect.pan_pullout = 0.0
	AudioServer.add_bus_effect(0, mono_effect)
	apply_preferences(preferences.effective_settings())
	layer = CanvasLayer.new(); layer.layer = 100; add_child(layer)
	menu = Menu.new(); menu.runtime = self; layer.add_child(menu); menu.hide()
	if not preferences.error.is_empty(): show_notice(preferences.error)
	elif preferences.recovered: show_notice("Previous settings restored after an interrupted or damaged update.")
	elif first_launch: show_notice("Settings: press F10 or both controller stick buttons (L3 + R3).")

func apply_preferences(value: Dictionary) -> void:
	accessibility.configure(value.accessibility)
	pacing.configure(value.accessibility)
	if not remaps.configure(value.controls): remaps.configure(Preferences.defaults().controls)
	get_tree().call_group("native_input_devices", "clear_devices")
	var display: Dictionary = value.display
	Engine.max_fps = display.fps
	var window := get_window()
	match display.aspect:
		"wide": window.content_scale_aspect = Window.CONTENT_SCALE_ASPECT_KEEP_HEIGHT
		"auto": window.content_scale_aspect = Window.CONTENT_SCALE_ASPECT_EXPAND
		_: window.content_scale_aspect = Window.CONTENT_SCALE_ASPECT_KEEP
	window.content_scale_stretch = Window.CONTENT_SCALE_STRETCH_INTEGER if display.filter == "integer" else Window.CONTENT_SCALE_STRETCH_FRACTIONAL
	window.canvas_item_default_texture_filter = Viewport.DEFAULT_CANVAS_ITEM_TEXTURE_FILTER_LINEAR if display.filter == "linear" else Viewport.DEFAULT_CANVAS_ITEM_TEXTURE_FILTER_NEAREST
	if DisplayServer.get_name() != "headless" and display != applied_display:
		window.mode = Window.MODE_WINDOWED
		window.current_screen = clampi(display.monitor, 0, DisplayServer.get_screen_count() - 1)
		window.size = Vector2i(display.width, display.height)
		if display.mode != "windowed": window.mode = Window.MODE_EXCLUSIVE_FULLSCREEN if display.mode == "exclusive" else Window.MODE_FULLSCREEN
		DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_ENABLED if display.vsync else DisplayServer.VSYNC_DISABLED)
	applied_display = display.duplicate(true)
	var devices := AudioServer.get_output_device_list()
	AudioServer.output_device = value.audio.device if value.audio.device in devices else "Default"
	for effect_index: int in AudioServer.get_bus_effect_count(0):
		if AudioServer.get_bus_effect(0, effect_index) == mono_effect:
			AudioServer.set_bus_effect_enabled(0, effect_index, value.audio.mono)
	if value.language.text_manifest != packs.manifest or value.language.text_sha256 != packs.digest:
		if not packs.select_images(value.language.text_manifest, value.language.text_sha256): packs.select_images("", "")
		translated_images = packs.images
	var active_layer: String = "" if packs.text_layer == null else packs.text_layer.manifest
	var active_layer_sha: String = "" if packs.text_layer == null else packs.text_layer.digest
	if value.language.ui_text_manifest != active_layer or value.language.ui_text_sha256 != active_layer_sha:
		if not packs.select_text_layer(value.language.ui_text_manifest, value.language.ui_text_sha256): packs.select_text_layer("", "")
		translated_text = packs.text_layer
	if value.language.voice_manifest != voice_pack.manifest or value.language.voice_sha256 != voice_pack.digest:
		var candidate := VoicePack.new()
		if value.language.voice_manifest.is_empty() or candidate.open_pack(value.language.voice_manifest, value.language.voice_sha256): voice_pack = candidate
		else: voice_pack = VoicePack.new()
	if value.language.dialogue_manifest != text_pack.manifest or value.language.dialogue_sha256 != text_pack.digest:
		var candidate := TextPack.new()
		if value.language.dialogue_manifest.is_empty() or candidate.open_pack(value.language.dialogue_manifest, value.language.dialogue_sha256): text_pack = candidate
		else: text_pack = TextPack.new()
	for bus: String in Preferences.BUSES:
		var spec: Dictionary = value.audio.buses[bus]
		var index := AudioServer.get_bus_index(bus)
		AudioServer.set_bus_volume_db(index, linear_to_db(maxf(spec.volume, 0.00001)))
		AudioServer.set_bus_mute(index, spec.mute or spec.volume == 0 or (bus == "Master" and value.audio.mute_unfocused and not focused))
	preferences_changed.emit(value.duplicate(true))

func select_image_pack(path: String) -> String:
	var digest := "" if path.is_empty() else FileAccess.get_sha256(path)
	var candidate := Packs.new()
	if not candidate.select_images(path, digest): return candidate.error
	var before: Dictionary = preferences.settings.language.duplicate(true)
	preferences.settings.language.text = candidate.language
	preferences.settings.language.text_manifest = path
	preferences.settings.language.text_sha256 = digest
	if not preferences.save():
		preferences.settings.language = before
		return preferences.error
	apply_preferences(preferences.effective_settings())
	return ""

## T-39016: select the source-keyed text layer built by
## tools/port/build_text_pack.py; an empty path restores original text.
func select_ui_text_pack(path: String) -> String:
	var digest := "" if path.is_empty() else FileAccess.get_sha256(path)
	var candidate := Packs.new()
	if not candidate.select_text_layer(path, digest): return candidate.text_layer_error
	var before: Dictionary = preferences.settings.language.duplicate(true)
	preferences.settings.language.ui_text_manifest = path
	preferences.settings.language.ui_text_sha256 = digest
	if not preferences.save(): preferences.settings.language = before; return preferences.error
	# Reuse the validated layer instead of parsing the pack a second time.
	packs.text_layer = candidate.text_layer
	translated_text = packs.text_layer
	apply_preferences(preferences.effective_settings()); return ""

func select_voice_pack(path: String) -> String:
	var candidate := VoicePack.new()
	var digest := "" if path.is_empty() else FileAccess.get_sha256(path)
	if not path.is_empty() and not candidate.open_pack(path, digest): return candidate.error
	var before: Dictionary = preferences.settings.language.duplicate(true)
	preferences.settings.language.voice = candidate.language
	preferences.settings.language.voice_manifest = path
	preferences.settings.language.voice_sha256 = "" if path.is_empty() else digest
	if not preferences.save(): preferences.settings.language = before; return preferences.error
	apply_preferences(preferences.effective_settings()); return ""

func select_text_pack(path: String) -> String:
	var candidate := TextPack.new(); var digest := "" if path.is_empty() else FileAccess.get_sha256(path)
	if not path.is_empty() and not candidate.open_pack(path, digest): return candidate.error
	var before: Dictionary = preferences.settings.language.duplicate(true)
	preferences.settings.language.dialogue_manifest = path
	preferences.settings.language.dialogue_sha256 = "" if path.is_empty() else digest
	if not preferences.save(): preferences.settings.language = before; return preferences.error
	apply_preferences(preferences.effective_settings())
	if not text_pack.unsupported_entries.is_empty(): show_notice("Some translated lines contain unsupported characters and will use Japanese.")
	return ""

func _process(delta: float) -> void:
	preferences.tick(Time.get_ticks_msec() / 1000.0, focused)
	if not is_instance_valid(menu) or not menu.visible or not focused: repeat_action = ""; return
	var action := ""
	for direction: String in ["ui_up", "ui_down", "ui_left", "ui_right"]:
		if Input.is_action_pressed(direction): action = direction; break
	if action != repeat_action:
		repeat_action = action; repeat_elapsed = -float(preferences.settings.controls.repeat_delay)
	elif not action.is_empty():
		repeat_elapsed += delta
		if repeat_elapsed >= 0:
			repeat_elapsed = -float(preferences.settings.controls.repeat_interval)
			var event := InputEventAction.new(); event.action = action; event.pressed = true
			get_viewport().push_input(event)

func _notification(what: int) -> void:
	match what:
		NOTIFICATION_WM_WINDOW_FOCUS_OUT: window_focused = false
		NOTIFICATION_APPLICATION_FOCUS_OUT: application_focused = false
		NOTIFICATION_WM_WINDOW_FOCUS_IN: window_focused = true
		NOTIFICATION_APPLICATION_FOCUS_IN: application_focused = true
		_: return
	focused = window_focused and application_focused
	if not focused:
		settings_buttons.clear(); chord_latched.clear()
		if is_instance_valid(menu):
			preferences.tick(Time.get_ticks_msec() / 1000.0, false)
			apply_preferences(preferences.effective_settings())
	else:
		if is_instance_valid(menu): apply_preferences(preferences.effective_settings())

func _gamepad_connection_changed(device: int, connected: bool) -> void:
	if connected: return
	settings_buttons.erase(device); chord_latched.erase(device)
	repeat_action = ""; repeat_elapsed = 0.0

func _input(event: InputEvent) -> void:
	if event is InputEventJoypadButton and event.button_index in [JOY_BUTTON_LEFT_STICK, JOY_BUTTON_RIGHT_STICK]:
		var held: int = settings_buttons.get(event.device, 0)
		var bit := 1 if event.button_index == JOY_BUTTON_LEFT_STICK else 2
		held = (held | bit) if event.pressed else (held & ~bit)
		settings_buttons[event.device] = held
		if held == 0: chord_latched[event.device] = false
		if held == 3 and not chord_latched.get(event.device, false):
			chord_latched[event.device] = true; toggle_menu()
		get_viewport().set_input_as_handled(); return
	if is_instance_valid(menu) and menu.visible and event is InputEventKey and event.echo:
		for action: String in ["ui_up", "ui_down", "ui_left", "ui_right"]:
			if event.is_action_pressed(action, true): get_viewport().set_input_as_handled(); return
	if event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_F10:
		if menu.visible and not menu.capture.is_empty(): menu.cancel_capture()
		else: toggle_menu()
		get_viewport().set_input_as_handled()

func toggle_menu() -> void:
	get_tree().call_group("native_input_devices", "clear_devices")
	menu.capture = ""
	if menu.visible:
		preferences.revert_display(); menu.hide(); get_tree().paused = previous_pause
	else:
		previous_pause = get_tree().paused; get_tree().paused = true
		get_tree().call_group("settings_notices", "hide")
		get_tree().call_group("native_input_devices", "clear_devices")
		menu.rebuild(); menu.show()

func save_host() -> Node:
	var hosts := get_tree().get_nodes_in_group("settings_save_hosts")
	return hosts[0] if hosts.size() == 1 else null

func request_save(load_game: bool) -> void:
	var host := save_host()
	if host == null:
		menu.notice.text = "Save/load is unavailable in this scene."
		return
	if menu.visible: toggle_menu()
	host.request_settings_save(load_game)

func preview_audio(bus: String) -> void:
	var wave := AudioStreamWAV.new(); wave.format = AudioStreamWAV.FORMAT_16_BITS; wave.mix_rate = 22050
	var data := PackedByteArray(); data.resize(4410 * 2)
	for i: int in 4410:
		data.encode_s16(i * 2, int(sin(float(i) * TAU * 440.0 / 22050.0) * 5000 * (1.0 - float(i) / 4410)))
	wave.data = data
	var player := AudioStreamPlayer.new(); player.bus = bus; player.stream = wave
	add_child(player); player.finished.connect(player.queue_free); player.play()

func observe_prologue_read(opening: Dictionary) -> void:
	if not pacing.reading_complete(): return
	var work: Variant = opening.get("prologue_text_work")
	if work is Dictionary and work.get("line_id") is int:
		read_history.mark_read(PrologueText.PAYLOAD_SHA + ":" + str(work.line_id))

func request_prologue_skip(pad: Node) -> bool:
	if preferences.settings.accessibility.skip == "read":
		var required: Array[String] = []
		for line: int in range(43): required.append(PrologueText.PAYLOAD_SHA + ":" + str(line))
		for line: int in [100, 101]: required.append(PrologueText.PAYLOAD_SHA + ":" + str(line))
		if not read_history.contains_all(required):
			show_notice("This prologue contains unread text. Choose All in Settings to skip it.")
			return false
	# Use the source's owned Start skip path, including its voice cancellation.
	pacing.reset(); pad.request_button_pulse(0x800); return true

func show_notice(message: String) -> void:
	if is_instance_valid(menu) and menu.visible:
		menu.notice.text = message; return
	var panel := PanelContainer.new(); panel.position = Vector2(16, 16); panel.size = Vector2(560, 72)
	var label := Label.new(); label.text = message; label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	panel.add_child(label); layer.add_child(panel)
	panel.add_to_group("settings_notices")
	get_tree().create_timer(3.0).timeout.connect(func(): if is_instance_valid(panel): panel.queue_free())
