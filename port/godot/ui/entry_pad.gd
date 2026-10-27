extends "res://ui/native_pad.gd"
## Retain this exact device owner across NAME_ENT -> prologue. Enter is START
## in the existing entry UI; Space/face-A remain Circle, Escape/face-B Cross.
var _latest_sample: RefCounted
func _input(event: InputEvent) -> void:
	if event is InputEventKey and event.physical_keycode == KEY_ENTER:
		if not event.echo: _devices["entry:start"] = 0x800 if event.pressed else 0
	elif event is InputEventJoypadButton and event.button_index == JOY_BUTTON_START:
		_devices["entry:joy-start:%d" % event.device] = 0x800 if event.pressed else 0
	else: super._input(event)
func sample(iteration: int) -> RefCounted:
	_latest_sample = super.sample(iteration)
	return _latest_sample
func owns_sample(value: RefCounted) -> bool:
	return value != null and value == _latest_sample
