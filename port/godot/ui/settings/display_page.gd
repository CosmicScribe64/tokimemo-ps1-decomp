extends RefCounted
## T-30002 WP7 + T-30001 WP5: window, scaling and scene layout. Window options
## go through the shell's Keep/Revert draft; wing fill and interface size apply
## live. None of these options change the native 320x240 picture.
const TITLE := "Display"
const SIZES := ["960×720", "1280×960", "1600×900", "1280×720", "1920×1080", "2560×1440"]
const WingArtPreview = preload("res://ui/settings/wing_art_preview.gd")
## T-39220 side art: decoration from the original game's own pictures only.
const WING_ART_MODES := ["off", "seasonal", "scene", "fixed:yozakura", "fixed:spring_day",
	"fixed:summer", "fixed:autumn", "fixed:winter", "fixed:school", "fixed:park", "fixed:town",
	"fixed:seaside", "fixed:legend_tree"]
const WING_ART_NAMES := {"off": "Off", "seasonal": "Seasonal", "scene": "Match the scene",
	"fixed:yozakura": "Night sakura", "fixed:spring_day": "Sakura avenue", "fixed:summer": "Summer night",
	"fixed:autumn": "Ginkgo avenue", "fixed:winter": "Snowfield", "fixed:school": "School grounds",
	"fixed:park": "Park avenue", "fixed:town": "Home street", "fixed:seaside": "Seaside",
	"fixed:legend_tree": "Legendary tree", "auto": "Same as Side art"}
const WING_ART_NOTES := {
	"off": "Off: the side areas stay black, as on the original console.",
	"seasonal": "Seasonal: follows the in-game month (night sakura, summer night, ginkgo avenue, snowfield).",
	"scene": "Match the scene: shows the place of the scene when the port knows it, else the season.",
	"fixed:yozakura": "Night sakura: the opening's sakura avenue at night with drifting petals, all year.",
	"fixed:spring_day": "Sakura avenue: the same avenue in daylight with drifting petals, all year.",
	"fixed:summer": "Summer night: a night sky with faint fireworks that stay inside the side areas.",
	"fixed:autumn": "Ginkgo avenue: the autumn avenue with falling leaves, all year.",
	"fixed:winter": "Snowfield: the snowfield at dusk, still, all year.",
	"fixed:school": "School grounds: the school front, with the season's petals or leaves.",
	"fixed:park": "Park avenue: the park's tree avenue, with the season's petals or leaves.",
	"fixed:town": "Home street: the street near home, with the season's petals or leaves.",
	"fixed:seaside": "Seaside: the cove under a cloudy sky, still.",
	"fixed:legend_tree": "Legendary tree: the tree from the opening, with the season's petals or leaves.",
	"auto": "Same as Side art: these scenes use the Side art choice above."}
## Added to the Seasonal and Match notes while the game does not yet report
## its month and scene to the display (no WingArtSceneFeed attached).
const NOT_ACTIVE_NOTE := " Not active yet in this build: the game does not report its month and scene to the display, so this shows the April look."
## Per scene type selectors: preference key, label, one-line explanation.
const SCENE_TYPES := [
	["display.wing_art_command", "Side art: command screens", "Command screens, school days and club practice."],
	["display.wing_art_story", "Side art: dates and talks", "Dates, walks home and phone calls."],
	["display.wing_art_other", "Side art: other scenes", "Title, calendar, events, CG pictures and the ending."]]
const OVERRIDES_NOTE := "Advanced: rules per place or background can go in wing-art-overrides.json in the game's user folder (schema tokimemo.wing-art.overrides.v1; tried before the built-in rules)."

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
	if ctx.has_pref("display.wing_art"):
		_side_art(box, ctx)
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

## Side art selector, the selected option's one-line explanation, a live
## preview, each option's explanation as its tooltip, and one selector per
## scene type. Applies live.
func _side_art(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var current: String = str(ctx.pref("display.wing_art"))
	var host: Object = ctx.frame_service()
	var follows := follows_game(host)
	var refs := {}  # lambdas capture locals by value; the label is set below
	var preview := WingArtPreview.new()
	var item: OptionButton = w.option(box, "Side art", WING_ART_MODES, current, func(v: String) -> void:
		var error: String = ctx.set_pref("display.wing_art", v)
		menu.set_notice(error if not error.is_empty() else "Side art updated.")
		if refs.has("label"): refs.label.text = mode_note(v, follows)
		preview.set_mode(v), "", WING_ART_NAMES)
	for index: int in item.item_count:
		var mode: String = WING_ART_MODES[index] if index < WING_ART_MODES.size() else current
		item.set_item_tooltip(index, mode_note(mode, follows))
	refs["label"] = w.note(box, mode_note(current, follows), "wing_art_explanation")
	var access: Variant = ctx.pref("accessibility.reduce_motion", false) if ctx.has_pref("accessibility.reduce_motion") else false
	preview.setup(host if host != null and host.has_method("wing_art_preview") else null, current, access == true)
	box.add_child(preview)
	w.note(box, "Side art uses only the original game's own pictures. The game picture itself never changes.")
	var values: Array = ["auto"]
	values.append_array(WING_ART_MODES)
	for entry: Array in SCENE_TYPES:
		var key: String = entry[0]
		if not ctx.has_pref(key):
			continue
		var choice: OptionButton = w.option(box, entry[1], values, str(ctx.pref(key)), func(v: String) -> void:
			var error: String = ctx.set_pref(key, v)
			menu.set_notice(error if not error.is_empty() else "Side art updated."), "", WING_ART_NAMES)
		for index: int in choice.item_count:
			var mode: String = values[index] if index < values.size() else "auto"
			choice.set_item_tooltip(index, mode_note(mode, follows))
		w.note(box, entry[2] + " Applies only while Side art is not Off.")
	w.note(box, OVERRIDES_NOTE)

## True when the display host reports that the game feeds it the month and
## scene (NativeDisplayHost.wing_art_follows_game).
static func follows_game(host: Object) -> bool:
	if host == null or not host.has_method("capabilities"):
		return false
	var caps: Variant = host.capabilities()
	return caps is Dictionary and caps.get("wing_art_follows_game") == true

static func mode_note(mode: String, follows: bool) -> String:
	var text: String = WING_ART_NOTES.get(mode, "")
	if not follows and mode in ["seasonal", "scene"]:
		text += NOT_ACTIVE_NOTE
	return text
