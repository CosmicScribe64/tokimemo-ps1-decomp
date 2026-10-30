extends RefCounted
## T-30002 WP7 + T-30001 WP5: window, scaling and scene layout. Window options
## go through the shell's Keep/Revert draft; wing fill and interface size apply
## live. None of these options change the native 320x240 picture.
const TITLE := "Display"
const SIZES := ["960×720", "1280×960", "1600×900", "1280×720", "1920×1080", "2560×1440"]

func available(_ctx: RefCounted) -> bool:
	return true

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var draft: Dictionary = menu.draft
	w.option(box, "Window mode", ["windowed", "borderless", "exclusive"], draft.mode, func(v: String) -> void: draft.mode = v)
	var monitors: Array = []
	for index: int in maxi(DisplayServer.get_screen_count(), 1): monitors.append(index)
	var monitor_names := {}
	for index: int in monitors: monitor_names[index] = "Display %d" % (index + 1)
	w.option(box, "Monitor", monitors, draft.monitor, func(v: int) -> void: draft.monitor = v, "", monitor_names)
	var sizes: Array = SIZES.duplicate()
	var current := "%d×%d" % [draft.width, draft.height]
	if draft.has("size_mode"):
		sizes.push_front("desktop")
		if draft.size_mode == "desktop": current = "desktop"
	var size_names := {"desktop": "Desktop resolution"}
	if current not in SIZES and current != "desktop": size_names[current] = current + " (remembered)"
	w.option(box, "Window size", sizes, current, func(v: String) -> void:
		if v == "desktop":
			draft.size_mode = "desktop"; return
		var parts := v.split("×"); draft.width = int(parts[0]); draft.height = int(parts[1])
		if draft.has("size_mode"): draft.size_mode = "preset" if v in SIZES else "custom",
		"", size_names)
	w.option(box, "Scaling", ["integer", "nearest", "linear"], draft.filter, func(v: String) -> void: draft.filter = v)
	w.note(box, "Sharp whole pixels shows the original picture at an exact multiple. Fill options use the whole window height.")
	if ctx.widescreen_available():
		w.option(box, "Scene layout", ["original", "wide", "auto"], draft.aspect, func(v: String) -> void: draft.aspect = v)
		w.note(box, "Scene art is never stretched or invented. Automatic uses the wide layout when the window is at least 3:2.")
		if ctx.has_pref("display.wing_fill"):
			w.option(box, "Side areas", ["black", "source"], ctx.pref("display.wing_fill"), func(v: String) -> void:
				var error: String = ctx.set_pref("display.wing_fill", v)
				menu.set_notice(error if not error.is_empty() else "Side areas updated."))
			w.note(box, "Continue scene pattern only extends the original checker background on command screens; other scenes keep black bars.")
	if ctx.has_pref("display.ui_scale"):
		w.slider(box, "Interface size", ctx.ui_scale(), 1.0, 2.0, 0.25, func(v: float) -> void:
			var error: String = ctx.set_pref("display.ui_scale", v)
			if not error.is_empty(): menu.set_notice(error)
			else: menu.request_relayout())
		w.note(box, "Interface size changes menus only, never the game picture.")
	w.option(box, "Frame limit", [60, 120, 144, 0], draft.fps, func(v: int) -> void: draft.fps = v, "", {60: "60 FPS", 120: "120 FPS", 144: "144 FPS", 0: "Unlimited"})
	w.toggle(box, "Vertical sync", draft.vsync, func(v: bool) -> void: draft.vsync = v)
	w.toggle(box, "Long display confirmation (60 seconds)", draft.long_confirmation, func(v: bool) -> void: draft.long_confirmation = v)
	w.info(box, ctx.drawable_readout(), "drawable_readout")
	w.button(box, "Apply display changes", menu.preview_display)
