extends "res://tests/services/settings_test_tree.gd"
const Entry = preload("res://app/new_game_entry_boot.gd")
func _initialize() -> void: call_deferred("run")
func key(code: int) -> InputEventKey:
	var event := InputEventKey.new(); event.physical_keycode = code; event.pressed = true; return event
func run() -> void:
	var runtime := root.get_node("ModernSettings")
	var entry := Entry.new(); root.add_child(entry); entry.set_physics_process(false)
	var pack := "/work/.asset-cache/port/installed-runtime-v5"
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(pack.path_join("manifest.json")))
	entry.controller._content = entry.Entry.load_content(pack.path_join(manifest.roles.name_entry_content))
	check(not entry.controller._content.is_empty())
	entry.controller.stage = 1; entry.controller.step_id = 1
	entry._input(key(KEY_RIGHT)); check(entry.navigation_cell == Vector2i(1,0))
	var button := InputEventJoypadButton.new(); button.button_index = JOY_BUTTON_A; button.pressed = true
	entry._input(button); check(entry.queue.pop_back() == {"kind":"cell", "column":1, "row":0})
	button.button_index = JOY_BUTTON_B; entry._input(button); check(entry.queue.pop_back().kind == "cross")
	entry._input(key(KEY_ENTER)); check(entry.queue.pop_back().kind == "start")
	var mapped: Dictionary = runtime.remaps.rebind("start", {"kind":"key", "code":KEY_G})
	check(runtime.preferences.set_value("controls", "bindings", mapped.bindings))
	entry._input(key(KEY_G)); check(entry.queue.pop_back().kind == "start")
	entry.controller.step_id = 9
	button.button_index = JOY_BUTTON_A; entry._input(button); check(entry.queue.pop_back().kind == "yes")
	var mouse := InputEventMouseButton.new(); mouse.button_index = MOUSE_BUTTON_RIGHT; mouse.pressed = true
	entry._input(mouse); check(entry.queue.pop_back().kind == "cross")
	mapped = runtime.remaps.rebind("cancel", {"kind":"mouse", "code":MOUSE_BUTTON_MIDDLE})
	check(runtime.preferences.set_value("controls", "bindings", mapped.bindings))
	entry._input(mouse); check(entry.queue.is_empty())
	mouse.button_index = MOUSE_BUTTON_MIDDLE; entry._input(mouse); check(entry.queue.pop_back().kind == "cross")
	entry.controller.step_id = 0
	button.button_index = JOY_BUTTON_A; entry._input(button); check(entry.queue.is_empty())
	entry.queue_free(); await process_frame
	print("Entry remapping: directional cell selection, controller confirm/cancel, default and remapped Start, mouse remap, automatic-stage guard passed")
	finish()
