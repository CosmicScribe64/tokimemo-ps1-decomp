extends RefCounted
## T-30002 WP7: quick save and load through the scene's guarded save host.
const TITLE := "Game"

func available(ctx: RefCounted) -> bool:
	return ctx.runtime.save_host() != null

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	w.info(box, "Quick save is available at command selection or the first result. Load restores the quick slot. Release game controls before using either action.")
	w.button(box, "Quick save", func() -> void: ctx.menu.request_save(false))
	w.button(box, "Load quick save", func() -> void: ctx.menu.request_save(true))
