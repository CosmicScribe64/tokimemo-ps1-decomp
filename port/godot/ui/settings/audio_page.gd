extends RefCounted
## T-30002 WP7: volumes and output. The native voice option is shown read-only
## because it lives in the game's own save image and is never synced.
const TITLE := "Audio"

func available(_ctx: RefCounted) -> bool:
	return true

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var runtime: Node = ctx.runtime
	for bus: String in runtime.Preferences.BUSES:
		var spec: Dictionary = runtime.preferences.settings.audio.buses[bus]
		var volume: HSlider = w.slider(box, bus, spec.volume, 0.0, 1.0, 0.01, func(v: float) -> void: menu.set_bus(bus, "volume", v))
		var row := HBoxContainer.new(); row.add_theme_constant_override("separation", 10); box.add_child(row)
		var mute := CheckButton.new(); mute.text = "Mute " + bus; mute.button_pressed = spec.mute; mute.custom_minimum_size.y = 34
		row.add_child(mute); mute.toggled.connect(func(v: bool) -> void: menu.set_bus(bus, "mute", v)); w.register(mute, bus + " mute")
		var test: Button = w.button(row, "Test " + bus, func() -> void: runtime.preview_audio(bus), bus + " preview")
		# Explicit neighbours: geometric search from the full-width slider can
		# skip the mute toggle when the card width changes.
		volume.focus_neighbor_bottom = volume.get_path_to(mute)
		mute.focus_neighbor_top = mute.get_path_to(volume); test.focus_neighbor_top = test.get_path_to(volume)
		mute.focus_neighbor_right = mute.get_path_to(test); test.focus_neighbor_left = test.get_path_to(mute)
	w.option(box, "Output device", Array(AudioServer.get_output_device_list()), runtime.preferences.settings.audio.device, func(v: String) -> void: menu.save_value("audio", "device", v))
	w.toggle(box, "Mute in background", runtime.preferences.settings.audio.mute_unfocused, func(v: bool) -> void: menu.save_value("audio", "mute_unfocused", v))
	w.toggle(box, "Mono output", runtime.preferences.settings.audio.mono, func(v: bool) -> void: menu.save_value("audio", "mono", v))
	var voice_mode: Variant = ctx.native_voice_mode()
	if voice_mode != null:
		w.info(box, "In-game voice option: %s (set from the game's own OPTION menu)" % str(voice_mode), "native_voice_mode")
