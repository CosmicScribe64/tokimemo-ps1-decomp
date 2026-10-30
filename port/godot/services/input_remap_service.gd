extends RefCounted
## T-30002: events map to native masks; timing remains in the simulation latch.
const MASKS := {"up": 0x1000, "right": 0x2000, "down": 0x4000, "left": 0x8000,
	"confirm": 0x20, "cancel": 0x40, "square": 0x80, "triangle": 0x10,
	"start": 0x800, "select": 0x100, "l1": 0x4, "r1": 0x8, "l2": 0x1, "r2": 0x2, "skip": 0}
const ACTIONS := ["up", "right", "down", "left", "confirm", "cancel", "square", "triangle", "start", "select", "l1", "r1", "l2", "r2", "skip"]
var bindings: Dictionary = defaults()
var swap_confirm := false
var error := ""

static func defaults() -> Dictionary:
	var keys := [KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_SPACE, KEY_ESCAPE, KEY_Q, KEY_W, KEY_TAB, KEY_F2, KEY_A, KEY_D, KEY_Z, KEY_C, KEY_CTRL]
	var buttons := [JOY_BUTTON_DPAD_UP, JOY_BUTTON_DPAD_RIGHT, JOY_BUTTON_DPAD_DOWN, JOY_BUTTON_DPAD_LEFT,
		JOY_BUTTON_A, JOY_BUTTON_B, JOY_BUTTON_X, JOY_BUTTON_Y, JOY_BUTTON_START, JOY_BUTTON_BACK,
		JOY_BUTTON_LEFT_SHOULDER, JOY_BUTTON_RIGHT_SHOULDER, -1, -1, JOY_BUTTON_MISC1]
	var result := {}
	for index: int in ACTIONS.size():
		var events: Array = [{"kind": "key", "code": keys[index]}]
		if buttons[index] >= 0: events.append({"kind": "button", "code": buttons[index]})
		result[ACTIONS[index]] = events
	result.confirm.append({"kind": "key", "code": KEY_ENTER})
	result.cancel.append({"kind": "key", "code": KEY_BACKSPACE})
	result.up.append({"kind": "axis", "code": JOY_AXIS_LEFT_Y, "direction": -1})
	result.down.append({"kind": "axis", "code": JOY_AXIS_LEFT_Y, "direction": 1})
	result.left.append({"kind": "axis", "code": JOY_AXIS_LEFT_X, "direction": -1})
	result.right.append({"kind": "axis", "code": JOY_AXIS_LEFT_X, "direction": 1})
	result.l2.append({"kind": "axis", "code": JOY_AXIS_TRIGGER_LEFT, "direction": 1})
	result.r2.append({"kind": "axis", "code": JOY_AXIS_TRIGGER_RIGHT, "direction": 1})
	result.confirm.append({"kind": "mouse", "code": MOUSE_BUTTON_LEFT})
	result.cancel.append({"kind": "mouse", "code": MOUSE_BUTTON_RIGHT})
	return result

static func valid_binding(spec: Variant) -> bool:
	if not spec is Dictionary or not spec.get("kind") is String or not spec.get("code") is int: return false
	if spec.size() != (3 if spec.kind == "axis" else 2): return false
	var code: int = spec.code
	match spec.kind:
		"key": return code > 0 and code <= KEY_SPECIAL + 65535 and code != KEY_F10
		"button": return code >= 0 and code < JOY_BUTTON_MAX and code not in [JOY_BUTTON_LEFT_STICK, JOY_BUTTON_RIGHT_STICK]
		"mouse": return code >= 1 and code <= MOUSE_BUTTON_XBUTTON2
		"axis": return code >= 0 and code < JOY_AXIS_MAX and spec.get("direction") in [-1, 1]
	return false

static func encode(event: InputEvent) -> Dictionary:
	if event is InputEventKey and event.pressed and not event.echo: return {"kind": "key", "code": event.physical_keycode}
	if event is InputEventJoypadButton and event.pressed: return {"kind": "button", "code": event.button_index}
	if event is InputEventMouseButton and event.pressed: return {"kind": "mouse", "code": event.button_index}
	if event is InputEventJoypadMotion and absf(event.axis_value) > 0.65: return {"kind": "axis", "code": event.axis, "direction": 1 if event.axis_value > 0 else -1}
	return {}

static func decode(spec: Dictionary) -> InputEvent:
	match spec.kind:
		"key":
			var event := InputEventKey.new(); event.physical_keycode = spec.code; return event
		"button":
			var event := InputEventJoypadButton.new(); event.device = -1; event.button_index = spec.code; return event
		"mouse":
			var event := InputEventMouseButton.new(); event.button_index = spec.code; return event
		"axis":
			var event := InputEventJoypadMotion.new(); event.device = -1; event.axis = spec.code; event.axis_value = spec.direction; return event
	return null

static func validated_bindings(proposed: Variant) -> Dictionary:
	var candidate := defaults()
	if not proposed is Dictionary: return {}
	for action: String in proposed:
		if action not in ACTIONS or not proposed[action] is Array or proposed[action].is_empty() or proposed[action].size() > 8:
			return {}
		var list: Array = []
		for raw: Variant in proposed[action]:
			# JSON stores numbers as floats; normalize only exact integers.
			if not raw is Dictionary: return {}
			var spec: Dictionary = raw.duplicate()
			for key: String in ["code", "direction"]:
				if spec.has(key) and spec[key] is float and spec[key] == floor(spec[key]): spec[key] = int(spec[key])
			if not valid_binding(spec): return {}
			list.append(spec)
		candidate[action] = list
	var used := {}
	for action: String in ACTIONS:
		for spec: Dictionary in candidate[action]:
			var identity := "%s:%d:%d" % [spec.kind, spec.code, spec.get("direction", 0)]
			if used.has(identity): return {}
			used[identity] = action
	return candidate

func configure(controls: Dictionary) -> bool:
	var candidate := validated_bindings(controls.get("bindings", {}))
	if candidate.is_empty(): error = "Invalid or conflicting input bindings."; return false
	bindings = candidate; swap_confirm = controls.get("swap_confirm", false)
	var deadzone: float = controls.get("deadzone", 0.25)
	for action: String in ACTIONS:
		var name := "port_" + action
		if not InputMap.has_action(name): InputMap.add_action(name, deadzone)
		InputMap.action_erase_events(name); InputMap.action_set_deadzone(name, deadzone)
		for spec: Dictionary in bindings[action]: InputMap.action_add_event(name, decode(spec))
	for action: String in ["up", "down", "left", "right", "confirm", "cancel"]:
		var target := "ui_" + ("accept" if action == "confirm" else action)
		var source := action
		if swap_confirm and action in ["confirm", "cancel"]: source = "cancel" if action == "confirm" else "confirm"
		InputMap.action_erase_events(target)
		InputMap.action_set_deadzone(target, deadzone)
		for spec: Dictionary in bindings[source]:
			if spec.kind != "mouse": InputMap.action_add_event(target, decode(spec))
	error = ""; return true

func conflicts(action: String, binding: Dictionary) -> Array[String]:
	var result: Array[String] = []
	for other: String in ACTIONS:
		if other != action and binding in bindings[other]: result.append(other)
	return result

func rebind(action: String, binding: Dictionary, replace_conflicts: bool = false) -> Dictionary:
	if action not in ACTIONS or not valid_binding(binding): return {"error": "Unsupported input. F10 and both stick clicks are reserved for Settings."}
	var collisions := conflicts(action, binding)
	if not collisions.is_empty() and not replace_conflicts: return {"error": "Already assigned to " + ", ".join(collisions), "conflicts": collisions}
	var result := bindings.duplicate(true)
	for other: String in collisions:
		result[other].erase(binding)
		if result[other].is_empty(): return {"error": "That would leave " + other + " unbound. Add another binding first."}
	# Replace this device category, keeping other device families available.
	result[action] = result[action].filter(func(spec: Dictionary) -> bool: return spec.kind != binding.kind)
	result[action].append(binding)
	var overrides := {}
	var original := defaults()
	for name: String in ACTIONS:
		if result[name] != original[name]: overrides[name] = result[name]
	return {"error": "", "bindings": overrides}

func held_mask() -> int:
	var held := 0
	for action: String in ACTIONS:
		if Input.is_action_pressed("port_" + action):
			var mapped := action
			if swap_confirm and action in ["confirm", "cancel"]: mapped = "cancel" if action == "confirm" else "confirm"
			held |= int(MASKS[mapped])
	return held

func prompt(action: String, controller: bool = false) -> String:
	var mapped := action
	if swap_confirm and action in ["confirm", "cancel"]: mapped = "cancel" if action == "confirm" else "confirm"
	for spec: Dictionary in bindings.get(mapped, []):
		if controller and spec.kind == "button":
			return {JOY_BUTTON_A: "A / Cross", JOY_BUTTON_B: "B / Circle", JOY_BUTTON_X: "X / Square", JOY_BUTTON_Y: "Y / Triangle", JOY_BUTTON_START: "Start / Options", JOY_BUTTON_BACK: "Select / Share", JOY_BUTTON_LEFT_SHOULDER: "LB / L1", JOY_BUTTON_RIGHT_SHOULDER: "RB / R1", JOY_BUTTON_DPAD_UP: "D-pad Up", JOY_BUTTON_DPAD_DOWN: "D-pad Down", JOY_BUTTON_DPAD_LEFT: "D-pad Left", JOY_BUTTON_DPAD_RIGHT: "D-pad Right"}.get(spec.code, "Button %d" % spec.code)
		if not controller and spec.kind == "key": return OS.get_keycode_string(spec.code)
	return action.capitalize()

func event_token(event: InputEvent) -> String:
	if event is InputEventKey: return "key:%d" % event.physical_keycode
	if event is InputEventJoypadButton: return "joy:%d:%d" % [event.device, event.button_index]
	if event is InputEventJoypadMotion: return "axis:%d:%d" % [event.device, event.axis]
	if event is InputEventMouseButton: return "mouse:%d" % event.button_index
	return ""

func event_mask(event: InputEvent) -> int:
	var result := 0
	for action: String in ACTIONS:
		if event.is_action_pressed("port_" + action, false, true):
			var mapped := action
			if swap_confirm and action in ["confirm", "cancel"]: mapped = "cancel" if action == "confirm" else "confirm"
			result |= int(MASKS[mapped])
	return result
