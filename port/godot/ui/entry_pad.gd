extends "res://ui/native_pad.gd"
## Retain this exact device owner across NAME_ENT -> prologue. Enter is START
## in the existing entry UI; Space/face-A remain Circle, Escape/face-B Cross.
var _latest_sample: RefCounted
func _gamepad_connection_changed(device: int, connected: bool) -> void:
	super._gamepad_connection_changed(device,connected)
	if not connected: _devices.erase("entry:joy-start:%d" % device)

func action_mask(event: InputEvent) -> int:
	if event is InputEventKey and event.echo: return 0
	var settings := get_node_or_null("/root/ModernSettings") if is_inside_tree() else null
	var custom_entry: bool = settings != null and (settings.preferences.settings.controls.bindings.has("confirm") or settings.preferences.settings.controls.bindings.has("start"))
	if not custom_entry and event is InputEventKey and event.physical_keycode == KEY_ENTER: return 0x800 if event.pressed else 0
	if settings != null: return settings.remaps.event_mask(event)
	if event is InputEventKey: return int(KEY_MASKS.get(event.physical_keycode, 0)) if event.pressed else 0
	if event is InputEventJoypadButton:
		if event.button_index == JOY_BUTTON_START: return 0x800 if event.pressed else 0
		return int(BUTTON_MASKS.get(event.button_index, 0)) if event.pressed else 0
	return 0
func _input(event: InputEvent) -> void:
	var settings := get_node_or_null("/root/ModernSettings") if is_inside_tree() else null
	if settings != null and (settings.preferences.settings.controls.bindings.has("confirm") or settings.preferences.settings.controls.bindings.has("start")):
		super._input(event)
		return
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
