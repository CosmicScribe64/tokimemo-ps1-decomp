extends SceneTree
## T-30002 WP3: controller family detection, prompt styles as text names,
## per-device reset and unchanged default bindings. Synthetic devices only.
const Glyphs = preload("res://services/input_glyph_service.gd")
const Remaps = preload("res://services/input_remap_service.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
## sha256 of JSON.stringify(Remaps.defaults()) at the phase 4 base snapshot.
const DEFAULT_BINDINGS_SHA := "d332b3ea134388f50503755a08decdb4229301b0dea3a4359e75be633e1369eb"
var failures: Array[String] = []

func check(condition: bool, label: String = "unlabelled check") -> void:
	if not condition: failures.append(label)

func _button(device: int, index: int, pressed: bool = true) -> InputEventJoypadButton:
	var event := InputEventJoypadButton.new(); event.device = device; event.button_index = index; event.pressed = pressed
	return event

func _key(code: int, pressed: bool = true) -> InputEventKey:
	var event := InputEventKey.new(); event.physical_keycode = code; event.pressed = pressed
	return event

func _initialize() -> void:
	# Family detection from SDL GUID vendor ids and joypad names.
	var fixtures := [
		["Xbox 360 Controller", "030000005e0400008e02000014010000", "xbox"],
		["", "030000005e040000e002000003090000", "xbox"],
		["Sony Interactive Entertainment Wireless Controller", "030000004c050000cc09000011810000", "playstation"],
		["", "030000004c050000e60c000011810000", "playstation"],
		["Nintendo Switch Pro Controller", "030000007e0500000920000011810000", "nintendo"],
		["Xbox Wireless Controller", "03000000c82d00000000000000000000", "xbox"],
		["PS5 Controller", "", "playstation"],
		["DualShock 4 (USB)", "zz", "playstation"],
		["Joy-Con (L/R)", "", "nintendo"],
		["8BitDo SN30 Pro", "03000000c82d00000160000011010000", "generic"],
		["", "", "generic"],
		["USB Gamepad", "0300000079000000110000000000000", "generic"],
	]
	for row: Array in fixtures:
		check(Glyphs.detect_family(row[0], row[1]) == row[2], "family of %s / %s is %s (got %s)" % [row[0], row[1], row[2], Glyphs.detect_family(row[0], row[1])])
	check(Glyphs.vendor_from_guid("030000005e0400008e02000014010000") == 0x045e, "vendor read little-endian")
	check(Glyphs.vendor_from_guid("not-a-guid") == -1, "invalid guid has no vendor")

	# Auto style follows the last-used device; explicit styles override it.
	var families := {0: ["Xbox Wireless Controller", "030000005e040000e002000003090000"],
		1: ["Wireless Controller", "030000004c050000cc09000011810000"],
		2: ["Pro Controller", "030000007e0500000920000011810000"],
		3: ["Generic USB Joystick", "03000000aaaa00000100000000000000"]}
	var glyphs := Glyphs.new()
	glyphs.joy_name_lookup = func(device: int) -> String: return families[device][0]
	glyphs.joy_guid_lookup = func(device: int) -> String: return families[device][1]
	var remaps := Remaps.new(); check(remaps.configure(Preferences.defaults().controls), "default controls configure")
	glyphs.configure(Preferences.defaults().controls)
	check(glyphs.style == "auto" and glyphs.active_style() == "keyboard", "auto starts on keyboard")
	check(glyphs.label("confirm", remaps) == "Space" and glyphs.label("cancel", remaps) == "Escape", "keyboard prompts are key names")
	check(glyphs.note_input(_button(0, JOY_BUTTON_A)) and glyphs.active_style() == "xbox", "xbox pad switches auto")
	check(glyphs.label("confirm", remaps) == "A" and glyphs.label("cancel", remaps) == "B" and glyphs.label("l2", remaps) == "LT", "xbox prompts")
	check(glyphs.note_input(_button(1, JOY_BUTTON_START)) and glyphs.active_style() == "playstation", "playstation pad switches auto")
	check(glyphs.label("confirm", remaps) == "Cross" and glyphs.label("triangle", remaps) == "Triangle" and glyphs.label("start", remaps) == "Options" and glyphs.label("r2", remaps) == "R2", "playstation prompts")
	check(glyphs.note_input(_button(2, JOY_BUTTON_A)) and glyphs.active_style() == "nintendo", "nintendo pad switches auto")
	check(glyphs.label("confirm", remaps) == "B" and glyphs.label("cancel", remaps) == "A" and glyphs.label("select", remaps) == "Minus", "nintendo prompts follow printed labels")
	check(glyphs.note_input(_button(3, JOY_BUTTON_A)) and glyphs.label("confirm", remaps) == "South button", "generic prompts are positional names")
	check(not glyphs.note_input(_button(3, JOY_BUTTON_A, false)), "a release does not switch devices")
	var motion := InputEventMouseMotion.new(); motion.relative = Vector2(5, 5)
	check(not glyphs.note_input(motion) and glyphs.active_style() == "generic", "mouse motion does not switch devices")
	var drift := InputEventJoypadMotion.new(); drift.device = 0; drift.axis = JOY_AXIS_LEFT_X; drift.axis_value = 0.2
	check(not glyphs.note_input(drift) and glyphs.active_style() == "generic", "stick noise does not switch devices")
	check(glyphs.note_input(_key(KEY_ENTER)) and glyphs.active_style() == "keyboard", "keyboard switches back")
	var click := InputEventMouseButton.new(); click.button_index = MOUSE_BUTTON_LEFT; click.pressed = true
	check(not glyphs.note_input(click) and glyphs.active_style() == "keyboard", "mouse counts as keyboard")
	for style: String in ["keyboard", "xbox", "playstation", "nintendo"]:
		glyphs.configure({"prompt_style": style})
		glyphs.note_input(_button(1, JOY_BUTTON_A))
		check(glyphs.active_style() == style, "explicit %s overrides the last device" % style)
		glyphs.note_input(_key(KEY_SPACE))
		check(glyphs.active_style() == style, "explicit %s ignores keyboard input" % style)
	glyphs.configure({"prompt_style": "steam"})
	check(glyphs.style == "auto", "unknown style falls back to auto")

	# Text only: every prompt is a short plain name, never a resource path.
	for style: String in ["keyboard", "xbox", "playstation", "nintendo"]:
		glyphs.configure({"prompt_style": style})
		for action: String in Remaps.ACTIONS:
			var text := glyphs.label(action, remaps)
			check(not text.is_empty() and text.length() <= 24 and not text.contains("res://") and not text.contains(".png") and not text.contains("/"), "%s %s prompt is a text name: %s" % [style, action, text])

	# swap_confirm moves the confirm prompt to the east face.
	glyphs.configure({"prompt_style": "xbox"})
	check(glyphs.confirm_side(remaps) == "south", "default confirm is the south face")
	var swapped: Dictionary = Preferences.defaults().controls; swapped.swap_confirm = true
	check(remaps.configure(swapped), "swap configures")
	check(glyphs.label("confirm", remaps) == "B" and glyphs.confirm_side(remaps) == "east", "swapped confirm is the east face")
	check(remaps.configure(Preferences.defaults().controls))

	# PS1 legend: native Circle confirms, Cross cancels.
	var legend := glyphs.ps1_legend(remaps)
	check(legend.size() == Remaps.ACTIONS.size(), "legend covers every action")
	check(legend[0].action == "confirm" and legend[0].ps1 == "Circle" and legend[0].mask == 0x20 and legend[0].prompt == "A", "confirm row")
	check(legend[1].action == "cancel" and legend[1].ps1 == "Cross" and legend[1].mask == 0x40, "cancel row")

	# Defaults, the 15 actions and the reserved inputs are unchanged.
	check(Remaps.ACTIONS.size() == 15, "15 actions")
	var digest := JSON.stringify(Remaps.defaults()).sha256_text()
	check(digest == DEFAULT_BINDINGS_SHA, "default bindings unchanged: " + digest)
	check(not Remaps.valid_binding({"kind": "key", "code": KEY_F10}), "F10 stays reserved")
	check(not Remaps.valid_binding({"kind": "button", "code": JOY_BUTTON_LEFT_STICK}) and not Remaps.valid_binding({"kind": "button", "code": JOY_BUTTON_RIGHT_STICK}), "L3/R3 stay reserved")

	# Per-device reset restores only that device's bindings.
	var keyboard_change: Dictionary = remaps.rebind("left", {"kind": "key", "code": KEY_H})
	check(keyboard_change.error.is_empty(), "keyboard rebind")
	var mixed: Dictionary = Preferences.defaults().controls; mixed.bindings = keyboard_change.bindings
	check(remaps.configure(mixed))
	var pad_change: Dictionary = remaps.rebind("l2", {"kind": "button", "code": JOY_BUTTON_GUIDE})
	check(pad_change.error.is_empty(), "controller rebind")
	mixed.bindings = pad_change.bindings
	check(remaps.configure(mixed) and {"kind": "key", "code": KEY_H} in remaps.bindings.left and {"kind": "button", "code": JOY_BUTTON_GUIDE} in remaps.bindings.l2, "both overrides active")
	var keyboard_reset: Dictionary = remaps.reset_device("keyboard")
	check(keyboard_reset.error.is_empty() and keyboard_reset.bindings.keys() == ["l2"], "keyboard reset keeps the controller override: %s" % [keyboard_reset])
	var controller_reset: Dictionary = remaps.reset_device("controller")
	check(controller_reset.error.is_empty() and controller_reset.bindings.keys() == ["left"] and controller_reset.bindings.left == [{"kind": "button", "code": JOY_BUTTON_DPAD_LEFT}, {"kind": "axis", "code": JOY_AXIS_LEFT_X, "direction": -1}, {"kind": "key", "code": KEY_H}], "controller reset keeps the keyboard override: %s" % [controller_reset])
	mixed.bindings = controller_reset.bindings
	check(remaps.configure(mixed) and remaps.reset_device("keyboard").bindings.is_empty(), "resetting both restores the defaults")
	check(not remaps.reset_device("mouse").error.is_empty(), "unknown device category rejected")
	check(remaps.configure(Preferences.defaults().controls) and remaps.reset_device("controller").bindings.is_empty(), "reset of untouched defaults is empty")

	if failures.is_empty():
		print("Input glyphs: GUID/name family detection, auto/explicit styles, text-only prompts, confirm side, PS1 legend, unchanged defaults, per-device reset passed")
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: " + failure)
		quit(1)
