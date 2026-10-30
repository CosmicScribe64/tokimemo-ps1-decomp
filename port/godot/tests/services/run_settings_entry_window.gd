extends "res://tests/services/settings_test_tree.gd"
const Entry = preload("res://app/new_game_entry_boot.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
func _initialize() -> void: call_deferred("run")
func run() -> void:
	var saved := Preferences.new("user://entry-display-test.json")
	saved.settings.display.width = 1280; saved.settings.display.height = 720
	check(saved.save())
	var loaded := Preferences.new(saved.path); check(loaded.load_preferences())
	var runtime := root.get_node("ModernSettings")
	runtime.apply_preferences(loaded.settings)
	await process_frame

	check(root.size == Vector2i(1280, 720))
	var entry := Entry.new(); root.add_child(entry)
	await process_frame
	check(root.size == Vector2i(1280, 720))
	check(root.content_scale_size == Vector2i.ZERO)
	check(entry.canvas.scale == Vector2(3,3) and entry.canvas.position == Vector2(160,0))
	loaded.settings.audio.mono = true; runtime.apply_preferences(loaded.settings)
	check(root.size == Vector2i(1280, 720)); check(root.content_scale_size == Vector2i.ZERO)
	loaded.settings.display.aspect = "wide"; runtime.apply_preferences(loaded.settings)
	check(root.content_scale_aspect == Window.CONTENT_SCALE_ASPECT_KEEP_HEIGHT)
	check(root.content_scale_size == Vector2i.ZERO)
	loaded.settings.display.aspect = "auto"; runtime.apply_preferences(loaded.settings)
	check(root.content_scale_aspect == Window.CONTENT_SCALE_ASPECT_EXPAND)
	loaded.settings.display.aspect = "original"; runtime.apply_preferences(loaded.settings)
	check(root.content_scale_aspect == Window.CONTENT_SCALE_ASPECT_KEEP)
	for dimensions: Vector2i in [Vector2i(960,720),Vector2i(1024,768),Vector2i(1280,800)]:
		loaded.settings.display.width = dimensions.x; loaded.settings.display.height = dimensions.y
		runtime.apply_preferences(loaded.settings)
		await process_frame; await process_frame
		entry._layout_native_canvas()
		var factor := floorf(minf(float(dimensions.x)/320.0,float(dimensions.y)/240.0))
		check(entry.canvas.scale == Vector2.ONE*factor)
		check(entry.canvas.position == ((Vector2(dimensions)-Vector2(320,240)*factor)/2.0).floor())
		check(entry.canvas.position.x >= 0 and entry.canvas.position.y >= 0)
	print("Entry retains saved window and fits native artwork within actual-pixel UI")
	finish()
