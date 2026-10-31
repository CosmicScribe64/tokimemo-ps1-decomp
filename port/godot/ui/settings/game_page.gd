extends RefCounted
## T-30002 WP7: quick save and load through the scene's guarded save host.
## T-39230: the Parity / Modern play mode, with its explanation. The page is
## always present because the play mode is; quick save and load appear only
## where a save host exists, and Parity mode locks them.
const TITLE := "Game"
const Parity = preload("res://services/parity_mode.gd")

func available(_ctx: RefCounted) -> bool:
	return true

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	if ctx.runtime.save_host() != null:
		w.info(box, "Quick save is available at command selection or the first result. Load restores the quick slot. Release game controls before using either action.")
		w.button(box, "Quick save", func() -> void: ctx.menu.request_save(false))
		w.button(box, "Load quick save", func() -> void: ctx.menu.request_save(true))
	w.subheading(box, "Play mode")
	w.option(box, "Play mode", Parity.MODES, ctx.play_mode(), func(v: String) -> void: ctx.menu.select_mode(v), "", Parity.LABELS)
	w.note(box, Parity.EXPLANATION, "parity_explanation")
	if not ctx.parity_forced_by().is_empty():
		w.note(box, Parity.FORCED_REFUSAL, "parity_forced")
