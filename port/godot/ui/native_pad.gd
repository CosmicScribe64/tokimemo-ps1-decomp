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

func _input(event: InputEvent) -> void:
	if event is InputEventKey and not event.echo and KEY_MASKS.has(event.physical_keycode):
		_devices["key:%d" % event.physical_keycode] = int(KEY_MASKS[event.physical_keycode]) if event.pressed else 0
	elif event is InputEventJoypadButton and BUTTON_MASKS.has(event.button_index):
		_devices["joy:%d:%d" % [event.device, event.button_index]] = int(BUTTON_MASKS[event.button_index]) if event.pressed else 0

func sample(iteration: int) -> RefCounted:
	var held: int = 0
	for value: int in _devices.values():
		held |= value
	var frame: RefCounted = InputFrame.new(iteration, held, _previous)
	_previous = held
	return frame

func clear_devices() -> void:
	_devices.clear()
	_previous = 0
