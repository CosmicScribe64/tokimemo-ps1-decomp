extends Node
## T-29005: device mapping only. sample() is called by the native input latch.
## PS1 masks follow include/game_enums.h; host events never advance rules.

const InputFrame = preload("res://core/input_frame.gd")
const KEY_MASKS: Dictionary = {KEY_UP: 0x1000, KEY_RIGHT: 0x2000,
	KEY_DOWN: 0x4000, KEY_LEFT: 0x8000, KEY_ENTER: 0x20, KEY_SPACE: 0x20,
	KEY_ESCAPE: 0x40, KEY_BACKSPACE: 0x40}
const BUTTON_MASKS: Dictionary = {JOY_BUTTON_DPAD_UP: 0x1000,
	JOY_BUTTON_DPAD_RIGHT: 0x2000, JOY_BUTTON_DPAD_DOWN: 0x4000,
	JOY_BUTTON_DPAD_LEFT: 0x8000, JOY_BUTTON_A: 0x20, JOY_BUTTON_B: 0x40}

var _devices: Dictionary = {}
var _previous: int = 0
var _requested_pulse := 0

func request_button_pulse(mask: int) -> void:
	_requested_pulse |= mask & 0xffff

func _ready() -> void:
	add_to_group("native_input_devices")
	Input.joy_connection_changed.connect(_gamepad_connection_changed)

func _gamepad_connection_changed(device: int, connected: bool) -> void:
	if connected: return
	for token: String in _devices.keys():
		if token.begins_with("joy:%d:" % device) or token.begins_with("axis:%d:" % device): _devices.erase(token)
	# Keep the previous sampled mask so the next source latch sees one release.

func _input(event: InputEvent) -> void:
	var settings := get_node_or_null("/root/ModernSettings") if is_inside_tree() else null
	if settings != null:
		if event is InputEventKey and event.echo: return
		var token: String = settings.remaps.event_token(event)
		if token != "": _devices[token] = settings.remaps.event_mask(event)
		return
	if event is InputEventKey and not event.echo and KEY_MASKS.has(event.physical_keycode):
		_devices["key:%d" % event.physical_keycode] = int(KEY_MASKS[event.physical_keycode]) if event.pressed else 0
	elif event is InputEventJoypadButton and BUTTON_MASKS.has(event.button_index):
		_devices["joy:%d:%d" % [event.device, event.button_index]] = int(BUTTON_MASKS[event.button_index]) if event.pressed else 0

func sample(iteration: int) -> RefCounted:
	var held: int = _requested_pulse
	_requested_pulse = 0
	for value: int in _devices.values():
		held |= value
	var frame: RefCounted = InputFrame.new(iteration, held, _previous)
	_previous = held
	return frame

func clear_devices() -> void:
	_devices.clear()
	_previous = 0
	_requested_pulse = 0
