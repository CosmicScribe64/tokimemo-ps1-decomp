class_name NativeHud
extends RefCounted
## Source-proved native-calendar and parameter digit actor layout.
## Coordinates remain PS1 center-relative until the canvas adapter draws them.

const STAT_LAYOUT := [
	{"name": "condition", "x": -0x49, "y": -0x58},
	{"name": "liberal_arts", "x": 0x0F, "y": -0x58},
	{"name": "science", "x": 0x67, "y": -0x58},
	{"name": "art", "x": -0x49, "y": -0x4A},
	{"name": "athletics", "x": 0x0F, "y": -0x4A},
	{"name": "trivia", "x": 0x67, "y": -0x4A},
	{"name": "looks", "x": -0x49, "y": -0x3C},
	{"name": "guts", "x": 0x0F, "y": -0x3C},
	{"name": "stress", "x": 0x67, "y": -0x3C},
]

static func build_calendar(state: Dictionary) -> Dictionary:
	var required := ["year", "month", "day", "weekday", "day_kind"]
	for key in required:
		if not state.has(key) or typeof(state[key]) != TYPE_INT:
			return _unsupported("calendar state is missing an integer " + key)
	var year: int = int(state["year"])
	var month: int = int(state["month"])
	var day: int = int(state["day"])
	var weekday: int = int(state["weekday"])
	var day_kind: int = int(state["day_kind"])
	if year < 0 or year > 255 or month < 1 or month > 12 or day < 1 or day > 31:
		return _unsupported("calendar fields are outside the native byte/date range")
	if weekday < 0 or weekday > 6 or day_kind < 0 or day_kind > 255:
		return _unsupported("weekday or calendar day-kind byte is outside range")
	var actors: Array[Dictionary] = []
	# actor_draw_all traverses the source actor pool in ascending actor ID.
	actors.append(_actor(16, 0x3C, 0x90, -0x12, true, 0))
	_add_digit(actors, 17, year, 2, 0x8C, -0x41, true)
	_add_digit(actors, 18, year, 1, 0x94, -0x41, true)
	_add_date_pair(actors, 19, 20, month, -0x22)
	_add_date_pair(actors, 21, 22, day, -1)
	var weekday_color: int = 0x43 if weekday == 5 else (0x44 if weekday == 6 else 0)
	if day_kind == 1 or day_kind == 2:
		weekday_color = 0x44
	actors.append(_actor(23, weekday + 0x52, 0x92, 0x1F, true, weekday_color))
	return {"status": "supported", "coordinate_space": "ps1_center_relative_320x240",
		"actors": actors}

static func build_parameters(parameters: Dictionary) -> Dictionary:
	var actors: Array[Dictionary] = []
	for stat: Dictionary in STAT_LAYOUT:
		var name: String = stat["name"]
		if not parameters.has(name) or typeof(parameters[name]) != TYPE_INT:
			return _unsupported("parameter HUD is missing integer " + name)
		var value: int = int(parameters[name])
		if value < 0 or value > 999:
			return _unsupported("parameter %s is outside the source-proved 0..999 display range" % name)
		for digit_index in range(3, 0, -1):
			var actor_index: int = 32 + STAT_LAYOUT.find(stat) * 3 + (3 - digit_index)
			var x: int = int(stat["x"]) + (3 - digit_index) * 8
			var digit: int = _decimal_digit(value, digit_index)
			actors.append(_actor(actor_index, digit + 0x44, x, int(stat["y"]), true, 0))
	actors.append(_actor(59, 0x3D, 0, -0x48, true, 0))
	return {"status": "supported", "coordinate_space": "ps1_center_relative_320x240",
		"actors": actors}

static func _add_date_pair(actors: Array[Dictionary], tens_actor: int, ones_actor: int,
		value: int, y: int) -> void:
	var tens_visible: bool = value >= 10
	_add_digit(actors, tens_actor, value, 2, 0x8C if tens_visible else 0xFF,
		y if tens_visible else 0xFF, tens_visible)
	_add_digit(actors, ones_actor, value, 1, 0x94 if tens_visible else 0x90, y, true)

static func _add_digit(actors: Array[Dictionary], actor_index: int, value: int,
		digit_index: int, x: int, y: int, visible: bool) -> void:
	var digit: int = _decimal_digit(value, digit_index)
	actors.append(_actor(actor_index, digit + 0x44, x, y, visible, 0))

static func _decimal_digit(value: int, digit_index: int) -> int:
	## Equivalent to cd_decimal_digit(s16 value, u8 digit_index), for the bounded
	## nonnegative HUD values. The caller rejects values outside source field bounds.
	var divisor: int = 1
	for _step in range(1, digit_index):
		divisor *= 10
	return int(value / divisor) % 10

static func _actor(actor_index: int, sprite_id: int, x: int, y: int,
		visible: bool, color_mode: int) -> Dictionary:
	return {"actor_index": actor_index, "sprite_id": sprite_id, "x": x, "y": y,
		"visible": visible, "draw_flags": 0x84 if visible else 0,
		"color_mode": color_mode, "coordinate_origin": "screen_center"}

static func _unsupported(message: String) -> Dictionary:
	return {"status": "unsupported", "diagnostic": message}
