extends RefCounted
## T-30002 WP7: text pacing and comfort options. Each option whose route-wide
## consumer is not verified yet (WP9) carries an "opening only" tag. Subtitles
## and colour filters are intentionally absent (design defaults).
const TITLE := "Text & Story"

func available(_ctx: RefCounted) -> bool:
	return true

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var access: Dictionary = ctx.runtime.preferences.settings.accessibility
	var save := func(key: String, value: Variant) -> void: menu.save_value("accessibility", key, value)
	w.option(box, "Text speed", ["slow", "normal", "fast", "instant"], access.text_speed, func(v: String) -> void: save.call("text_speed", v), ctx.scope_tag("text_speed"))
	if access.has("auto_advance"):
		w.toggle(box, "Auto-advance", bool(access.auto_advance), func(v: bool) -> void: save.call("auto_advance", v), ctx.scope_tag("auto_advance"))
	w.slider(box, "Extra reading time after voice (seconds)", access.auto_delay, 0.0, 10.0, 0.5, func(v: float) -> void: save.call("auto_delay", v), ctx.scope_tag("auto_delay"))
	w.option(box, "Skip", ["read", "all"], access.skip, func(v: String) -> void: save.call("skip", v), ctx.scope_tag("skip"))
	w.note(box, "Read text only skips a line after it has been read once. Skip uses Ctrl or its remapped control.")
	w.slider(box, "Text size", access.font_scale, 1.0, 1.5, 0.1, func(v: float) -> void: save.call("font_scale", v), ctx.scope_tag("font_scale"))
	w.slider(box, "Text box opacity", access.backdrop, 0.0, 1.0, 0.1, func(v: float) -> void: save.call("backdrop", v), ctx.scope_tag("backdrop"))
	if access.has("text_box"):
		w.option(box, "Dialogue text box", ["native", "modern"], access.text_box, func(v: String) -> void: save.call("text_box", v))
		w.note(box, "Modern text box draws translated lines in a readable box over the original window. The game picture underneath is unchanged.")
	w.toggle(box, "Reduce screen flashes", access.reduce_flashes, func(v: bool) -> void: save.call("reduce_flashes", v), ctx.scope_tag("reduce_flashes"))
	w.toggle(box, "Reduce screen shake", access.reduce_shake, func(v: bool) -> void: save.call("reduce_shake", v), ctx.scope_tag("reduce_shake"))
	if access.has("reduce_motion"):
		w.toggle(box, "Reduce motion", bool(access.reduce_motion), func(v: bool) -> void: save.call("reduce_motion", v))
		w.note(box, "Reduce motion keeps the side art still: no drifting petals, leaves or fireworks.")
