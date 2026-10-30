extends "res://tests/run_installed_first_scene.gd"
var last_entry_hash := ""
var entry_capture_pending := false
var entry_transform: Dictionary = {}
func _start() -> void:
	var settings := root.get_node("ModernSettings")
	var dimensions := OS.get_environment("HANDOFF_WINDOW").split("x")
	if dimensions.size() == 2:
		settings.preferences.settings.display.width = int(dimensions[0])
		settings.preferences.settings.display.height = int(dimensions[1])
		settings.apply_preferences(settings.preferences.settings)
	super._start()
func _process(delta: float) -> bool:
	var app := current_scene as App
	if app != null and is_instance_valid(app._entry):
		var entry = app._entry
		if entry.last_rendered_image != null:
			last_entry_hash = _image_sha(entry.last_rendered_image)
		if entry.controller.stage == 2 and entry.controller.step_id == 3 and not entry_capture_pending:
			entry_capture_pending = true
			entry_transform = {"scale":[entry.canvas.scale.x,entry.canvas.scale.y],"position":[entry.canvas.position.x,entry.canvas.position.y]}
			_capture_entry.call_deferred()
	return super._process(delta)
func _capture_entry() -> void:
	await RenderingServer.frame_post_draw
	root.get_texture().get_image().save_png("/out/entry-viewport.png")
func _finish(app: App) -> void:
	var retained: Image = app._entry_image
	check(retained != null and retained.get_size() == Vector2i(320,240) and _image_sha(retained) == last_entry_hash,"Entry retained image differs from last actual native submission")
	var viewport := Vector2(root.size)
	var factor := floorf(minf(viewport.x/320.0,viewport.y/240.0))
	check(root.content_scale_size == Vector2i.ZERO,"Opening changed UI root away from actual window pixels")
	check(app._display.size == Vector2(320,240)*factor and app._display.position == (viewport-app._display.size)*0.5,"Opening native viewport is cropped or incorrectly centered")
	retained.save_png("/out/retained-entry-native.png")
	var file := FileAccess.open("/out/handoff-layout.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"window":[root.size.x,root.size.y],"entry_transform":entry_transform,"native_size":[320,240],"last_entry_sha256":last_entry_hash,"retained_sha256":_image_sha(retained),"opening_display":{"size":[app._display.size.x,app._display.size.y],"position":[app._display.position.x,app._display.position.y]},"provenance":app.entry_frame_provenance},"\t")); file.close()
	await super._finish(app)
func _image_sha(image: Image) -> String:
	var hash := HashingContext.new(); hash.start(HashingContext.HASH_SHA256); hash.update(image.get_data())
	return hash.finish().hex_encode()
