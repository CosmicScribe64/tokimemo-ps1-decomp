extends RefCounted
## T-30002 WP7: remapping, prompt style (text names only), confirm side,
## per-device reset and the PS1 button legend. F10 and L3+R3 stay reserved.
const TITLE := "Controls"
const STYLE_NAMES := {"auto": "Automatic (last used device)", "keyboard": "Keyboard", "xbox": "Xbox",
	"playstation": "PlayStation", "nintendo": "Nintendo"}

func available(_ctx: RefCounted) -> bool:
	return true

func build(box: VBoxContainer, ctx: RefCounted) -> void:
	var w: RefCounted = ctx.widgets
	var menu: Control = ctx.menu
	var runtime: Node = ctx.runtime
	var controls: Dictionary = runtime.preferences.settings.controls
	if ctx.has_pref("controls.prompt_style"):
		w.option(box, "Prompt style", ctx.PROMPT_STYLES, ctx.pref("controls.prompt_style"), func(v: String) -> void:
			var error: String = ctx.set_pref("controls.prompt_style", v)
			if not error.is_empty(): menu.set_notice(error)
			else: menu.rebuild(), "", STYLE_NAMES)
	w.toggle(box, "Swap confirm and cancel", controls.swap_confirm, func(v: bool) -> void: menu.save_value("controls", "swap_confirm", v); menu.rebuild())
	w.info(box, ctx.confirm_side_label(), "confirm_side")
	w.slider(box, "Stick deadzone", controls.deadzone, 0.05, 0.9, 0.05, func(v: float) -> void: menu.save_value("controls", "deadzone", v))
	w.slider(box, "Menu repeat delay", controls.repeat_delay, 0.15, 1.0, 0.05, func(v: float) -> void: menu.save_value("controls", "repeat_delay", v))
	w.slider(box, "Menu repeat interval", controls.repeat_interval, 0.03, 0.3, 0.01, func(v: float) -> void: menu.save_value("controls", "repeat_interval", v))
	w.subheading(box, "Bindings")
	w.note(box, "Choose an action, then press a key, controller button, stick or mouse button. F10 cancels.")
	for action: String in runtime.Remaps.ACTIONS:
		var legend: String = ctx.PS1_LEGEND.get(action, "")
		var label := action.capitalize() + (" (PS1 " + legend + ")" if not legend.is_empty() else "")
		var item: Button = w.button(box, label + ": " + ctx.prompt(action, false) + " / " + ctx.prompt(action, true), func() -> void: menu.begin_capture(action), "binding:" + action)
		item.name = action
	w.button(box, "Reset keyboard and mouse", func() -> void: _reset(menu, ctx, "keyboard"), "reset_keyboard")
	w.button(box, "Reset controller", func() -> void: _reset(menu, ctx, "controller"), "reset_controller")
	w.button(box, "Reset all controls", func() -> void: runtime.preferences.restore_category("controls"); menu.rebuild(), "Reset controls")
	w.note(box, "Settings: F10 or L3 + R3. These two are reserved and cannot be rebound.")

static func _reset(menu: Control, ctx: RefCounted, family: String) -> void:
	var error: String = ctx.reset_device(family)
	menu.rebuild()
	menu.set_notice(error if not error.is_empty() else ("Keyboard and mouse" if family == "keyboard" else "Controller") + " bindings restored.")
