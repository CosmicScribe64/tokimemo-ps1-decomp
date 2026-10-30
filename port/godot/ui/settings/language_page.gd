extends RefCounted
## T-30002 WP7: one game-language choice that drives every pack, plus the
## custom pack pickers under Advanced. Voice stays Japanese (Original).
const TITLE := "Language"

func available(_ctx: RefCounted) -> bool:
	return true

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var choice: String = ctx.language_choice()
	var choices := ["ja", "en"]
	if choice == "custom": choices.append("custom")
	w.option(box, "Game language", choices, choice, func(value: String) -> void:
		if value == "custom": return
		var error: String = ctx.select_language(value)
		menu.set_notice(error if not error.is_empty() else "Loading %s…" % ctx.language_names().get(value, value))
		menu.rebuild_after_language(),
		"", ctx.language_names())
	w.note(box, ctx.language_disclosure(), "disclosure")
	if not ctx.language_available("en"):
		w.note(box, "English is not installed in this build yet. Japanese (Original) stays active.", "english_unavailable")
	w.info(box, ctx.coverage_line(), "coverage")
	if ctx.has_pref("language.allow_draft_images"):
		w.toggle(box, "Draft English artwork (unreviewed)", bool(ctx.pref("language.allow_draft_images")), func(value: bool) -> void:
			var error: String = ctx.set_pref("language.allow_draft_images", value)
			if not error.is_empty(): menu.set_notice(error))
		w.note(box, "Off by default. Images without reviewed artwork stay original Japanese.")
	var voice_custom: bool = not str(ctx.pref("language.voice_manifest", "")).is_empty()
	w.info(box, "Voice: custom pack (%s)" % ctx.runtime.voice_pack.language if voice_custom else "Voice: Japanese (Original)", "voice")
	if bool(menu.advanced_open):
		_advanced(box, ctx)
	else:
		w.button(box, "Advanced / custom packs…", func() -> void: menu.advanced_open = true; menu.rebuild(), "advanced_open")

func _advanced(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var runtime: Node = ctx.runtime
	w.subheading(box, "Advanced / custom packs")
	w.note(box, "Custom packs are checked against the file you choose. A pack that fails the check is not used, and a notice says so.")
	w.info(box, "Image pack: " + runtime.packs.language)
	w.button(box, "Original images", func() -> void: _report(menu, runtime.select_image_pack("")), "custom_images_clear")
	w.button(box, "Choose image pack…", func() -> void: menu.choose_file("*.json ; Translation manifest", func(path: String) -> String: return runtime.select_image_pack(path)), "custom_images")
	w.info(box, "Dialogue pack: " + runtime.text_pack.language)
	if not runtime.text_pack.unsupported_entries.is_empty():
		w.note(box, "%d lines contain unsupported characters and will use Japanese." % runtime.text_pack.unsupported_entries.size())
	w.button(box, "Original dialogue", func() -> void: _report(menu, runtime.select_text_pack("")), "custom_dialogue_clear")
	w.button(box, "Choose dialogue pack…", func() -> void: menu.choose_file("*.json ; Text translation manifest", func(path: String) -> String: return runtime.select_text_pack(path)), "custom_dialogue")
	var layer_active: bool = runtime.packs.text_layer != null
	w.info(box, "Interface text layer: " + ("custom" if layer_active else "original"))
	w.button(box, "Original interface text", func() -> void: _report(menu, runtime.select_ui_text_pack("")), "custom_text_layer_clear")
	w.button(box, "Choose interface text layer…", func() -> void: menu.choose_file("*.json ; Text layer manifest", func(path: String) -> String: return runtime.select_ui_text_pack(path)), "custom_text_layer")
	w.info(box, "Voice pack: " + runtime.voice_pack.language)
	w.button(box, "Original voice", func() -> void: _report(menu, runtime.select_voice_pack("")), "custom_voice_clear")
	w.button(box, "Choose voice pack…", func() -> void: menu.choose_file("*.json ; Voice manifest", func(path: String) -> String: return runtime.select_voice_pack(path)), "custom_voice")
	w.button(box, "Hide advanced options", func() -> void: menu.advanced_open = false; menu.rebuild(), "advanced_close")

static func _report(menu: Control, error: String) -> void:
	menu.rebuild()
	if not error.is_empty(): menu.set_notice(error)
