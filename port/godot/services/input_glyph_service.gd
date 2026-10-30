extends RefCounted
## T-30002 WP3: button prompts as plain-text names, never images.
## The family (xbox/playstation/nintendo/generic) comes from the SDL GUID vendor
## or the joypad name. Style 'auto' follows the last-used device; an explicit
## controls.prompt_style overrides it. Prompts never change native input masks.

const Remaps = preload("res://services/input_remap_service.gd")
const STYLES := ["auto", "keyboard", "xbox", "playstation", "nintendo"]
const FAMILIES := ["xbox", "playstation", "nintendo", "generic"]
## USB vendor ids as they appear in SDL GUID bytes 4-5 (little endian).
const VENDORS := {0x045e: "xbox", 0x054c: "playstation", 0x057e: "nintendo"}
## Lower-case name fragments, checked in order after the vendor id.
const NAME_HINTS := [
	["xbox", "xbox"], ["x-box", "xbox"], ["xinput", "xbox"],
	["dualsense", "playstation"], ["dualshock", "playstation"], ["playstation", "playstation"],
	["ps5", "playstation"], ["ps4", "playstation"], ["ps3", "playstation"], ["sony", "playstation"],
	["nintendo", "nintendo"], ["switch", "nintendo"], ["joy-con", "nintendo"], ["pro controller", "nintendo"],
]
## Positional (SDL) buttons -> printed label per family.
const BUTTON_LABELS := {
	JOY_BUTTON_A: {"xbox": "A", "playstation": "Cross", "nintendo": "B", "generic": "South button"},
	JOY_BUTTON_B: {"xbox": "B", "playstation": "Circle", "nintendo": "A", "generic": "East button"},
	JOY_BUTTON_X: {"xbox": "X", "playstation": "Square", "nintendo": "Y", "generic": "West button"},
	JOY_BUTTON_Y: {"xbox": "Y", "playstation": "Triangle", "nintendo": "X", "generic": "North button"},
	JOY_BUTTON_BACK: {"xbox": "View", "playstation": "Share", "nintendo": "Minus", "generic": "Select"},
	JOY_BUTTON_GUIDE: {"xbox": "Xbox button", "playstation": "PS button", "nintendo": "Home", "generic": "Guide"},
	JOY_BUTTON_START: {"xbox": "Menu", "playstation": "Options", "nintendo": "Plus", "generic": "Start"},
	JOY_BUTTON_LEFT_STICK: {"xbox": "LS", "playstation": "L3", "nintendo": "Left stick press", "generic": "Left stick press"},
	JOY_BUTTON_RIGHT_STICK: {"xbox": "RS", "playstation": "R3", "nintendo": "Right stick press", "generic": "Right stick press"},
	JOY_BUTTON_LEFT_SHOULDER: {"xbox": "LB", "playstation": "L1", "nintendo": "L", "generic": "Left shoulder"},
	JOY_BUTTON_RIGHT_SHOULDER: {"xbox": "RB", "playstation": "R1", "nintendo": "R", "generic": "Right shoulder"},
	JOY_BUTTON_DPAD_UP: {"xbox": "D-pad Up", "playstation": "D-pad Up", "nintendo": "D-pad Up", "generic": "D-pad Up"},
	JOY_BUTTON_DPAD_DOWN: {"xbox": "D-pad Down", "playstation": "D-pad Down", "nintendo": "D-pad Down", "generic": "D-pad Down"},
	JOY_BUTTON_DPAD_LEFT: {"xbox": "D-pad Left", "playstation": "D-pad Left", "nintendo": "D-pad Left", "generic": "D-pad Left"},
	JOY_BUTTON_DPAD_RIGHT: {"xbox": "D-pad Right", "playstation": "D-pad Right", "nintendo": "D-pad Right", "generic": "D-pad Right"},
	JOY_BUTTON_MISC1: {"xbox": "Share", "playstation": "Mute", "nintendo": "Capture", "generic": "Misc button"},
}
const TRIGGER_LABELS := {
	JOY_AXIS_TRIGGER_LEFT: {"xbox": "LT", "playstation": "L2", "nintendo": "ZL", "generic": "Left trigger"},
	JOY_AXIS_TRIGGER_RIGHT: {"xbox": "RT", "playstation": "R2", "nintendo": "ZR", "generic": "Right trigger"},
}
const MOUSE_LABELS := {MOUSE_BUTTON_LEFT: "Left click", MOUSE_BUTTON_RIGHT: "Right click", MOUSE_BUTTON_MIDDLE: "Middle click"}
## PS1 digital pad bits behind each port action (input_remap_service MASKS).
## The native game confirms with Circle and cancels with Cross.
const PS1_LEGEND := [
	{"action": "confirm", "ps1": "Circle"}, {"action": "cancel", "ps1": "Cross"},
	{"action": "square", "ps1": "Square"}, {"action": "triangle", "ps1": "Triangle"},
	{"action": "start", "ps1": "Start"}, {"action": "select", "ps1": "Select"},
	{"action": "l1", "ps1": "L1"}, {"action": "r1", "ps1": "R1"},
	{"action": "l2", "ps1": "L2"}, {"action": "r2", "ps1": "R2"},
	{"action": "up", "ps1": "Up"}, {"action": "down", "ps1": "Down"},
	{"action": "left", "ps1": "Left"}, {"action": "right", "ps1": "Right"},
	{"action": "skip", "ps1": "None (port skip)"},
]

var style := "auto"
var last_device := "keyboard"
var last_family := "generic"
## Injectable for tests; defaults read the live joypad identity.
var joy_name_lookup: Callable = func(device: int) -> String: return Input.get_joy_name(device)
var joy_guid_lookup: Callable = func(device: int) -> String: return Input.get_joy_guid(device)

func configure(controls: Dictionary) -> void:
	var value: Variant = controls.get("prompt_style", "auto")
	style = value if value in STYLES else "auto"

static func vendor_from_guid(guid: String) -> int:
	if guid.length() != 32 or not guid.is_valid_hex_number(): return -1
	return ("0x" + guid.substr(10, 2) + guid.substr(8, 2)).hex_to_int()

static func detect_family(joy_name: String, guid: String) -> String:
	var vendor := vendor_from_guid(guid.to_lower())
	if VENDORS.has(vendor): return VENDORS[vendor]
	var lowered := joy_name.to_lower()
	for hint: Array in NAME_HINTS:
		if lowered.contains(hint[0]): return hint[1]
	return "generic"

## Records the device behind a player input. Returns true when the auto style
## changes. Mouse motion and small stick noise do not switch devices.
func note_input(event: InputEvent) -> bool:
	var before := active_style()
	if event is InputEventKey or event is InputEventMouseButton:
		if not event.is_pressed(): return false
		last_device = "keyboard"
	elif event is InputEventJoypadButton or (event is InputEventJoypadMotion and absf(event.axis_value) > 0.5):
		if event is InputEventJoypadButton and not event.pressed: return false
		last_device = "controller"
		last_family = detect_family(str(joy_name_lookup.call(event.device)), str(joy_guid_lookup.call(event.device)))
	else:
		return false
	return active_style() != before

## keyboard | xbox | playstation | nintendo | generic
func active_style() -> String:
	if style != "auto": return style
	return "keyboard" if last_device == "keyboard" else last_family

static func binding_label(spec: Dictionary, family: String) -> String:
	match spec.get("kind"):
		"key": return OS.get_keycode_string(spec.code)
		"mouse": return MOUSE_LABELS.get(spec.code, "Mouse button %d" % spec.code)
		"button":
			var labels: Dictionary = BUTTON_LABELS.get(spec.code, {})
			return labels.get(family, "Button %d" % spec.code)
		"axis":
			if TRIGGER_LABELS.has(spec.code): return TRIGGER_LABELS[spec.code].get(family, "Trigger")
			var stick := "Left stick" if spec.code in [JOY_AXIS_LEFT_X, JOY_AXIS_LEFT_Y] else "Right stick"
			var horizontal: bool = spec.code in [JOY_AXIS_LEFT_X, JOY_AXIS_RIGHT_X]
			var direction := ("Right" if spec.direction > 0 else "Left") if horizontal else ("Down" if spec.direction > 0 else "Up")
			return stick + " " + direction
	return ""

## Text prompt for a port action in the active style, after swap_confirm.
func label(action: String, remaps: RefCounted) -> String:
	var current := active_style()
	var mapped := action
	if remaps.swap_confirm and action in ["confirm", "cancel"]: mapped = "cancel" if action == "confirm" else "confirm"
	var keyboard := current == "keyboard"
	for kinds: Array in ([["key"], ["mouse"]] if keyboard else [["button"], ["axis"]]):
		for spec: Dictionary in remaps.bindings.get(mapped, []):
			if spec.kind in kinds: return binding_label(spec, "generic" if keyboard else current)
	return action.capitalize()

## Which face button confirms on a controller: "south", "east", "west",
## "north" or "" when confirm has no face button.
func confirm_side(remaps: RefCounted) -> String:
	var mapped := "cancel" if remaps.swap_confirm else "confirm"
	for spec: Dictionary in remaps.bindings.get(mapped, []):
		if spec.kind == "button":
			match spec.code:
				JOY_BUTTON_A: return "south"
				JOY_BUTTON_B: return "east"
				JOY_BUTTON_X: return "west"
				JOY_BUTTON_Y: return "north"
	return ""

## Rows for the Controls page legend: action, PS1 button, active prompt text.
func ps1_legend(remaps: RefCounted) -> Array[Dictionary]:
	var rows: Array[Dictionary] = []
	for row: Dictionary in PS1_LEGEND:
		rows.append({"action": row.action, "ps1": row.ps1, "mask": int(Remaps.MASKS[row.action]), "prompt": label(row.action, remaps)})
	return rows
