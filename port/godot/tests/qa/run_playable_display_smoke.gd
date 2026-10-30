extends SceneTree
## T-39060: playable_boot's texture-presented canvas (no captured boundary
## needed; the scene stops at its input check, after the display is mounted).
## Checks that the NativeCanvas draws 1:1 into the 320x240 SubViewport, that
## the display host shows that texture on its aperture in original and wide
## aspect, and that the SubViewport pixels equal the drawn native Image.
## X11 only (Xvfb); headless renders no pixels.
const PLAYABLE := "res://app/playable.tscn"
var failures: Array[String] = []

func _initialize() -> void:
	_run.call_deferred()

func check(value: bool, label: String) -> void:
	if not value: failures.append(label)

func _run() -> void:
	var settings: Node = root.get_node("ModernSettings")
	var app: Node = load(PLAYABLE).instantiate()
	root.add_child(app)
	for i: int in 3: await process_frame
	check(app.native_view != null and app.native_view.size == Vector2i(320, 240), "320x240 SubViewport mounted")
	check(app.canvas != null and app.canvas.get_parent() == app.native_view and app.canvas.scale == Vector2.ONE, "canvas draws 1:1 inside the SubViewport")
	check(app.display_host != null and app.display_host.display.texture == app.native_view.get_texture(), "host shows the SubViewport texture")
	var native := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	for y: int in 240:
		for x: int in 320:
			native.set_pixel(x, y, Color8((x * 7) & 255, (y * 5) & 255, (x ^ y) & 255, 255))
	var commands: Array[Dictionary] = [{"id": "smoke", "image": native, "position": Vector2i.ZERO}]
	check(app.canvas.present(commands).is_empty(), "canvas accepts the native image")
	for i: int in 3:
		await process_frame
	await RenderingServer.frame_post_draw
	var shown: Image = app.native_view.get_texture().get_image()
	if shown != null: shown.convert(Image.FORMAT_RGBA8)
	check(shown != null and shown.get_data() == native.get_data(), "SubViewport pixels equal the native image")
	for aspect: String in ["original", "wide"]:
		settings.preferences.set_values({"display.aspect": aspect})
		root.get_window().size = Vector2i(1920, 1080)
		for i: int in 4: await process_frame
		var layout: Dictionary = app.display_host.layout
		check(layout.get("valid", false) and app.display_host.display.position == layout.aperture.position and app.display_host.display.size == layout.aperture.size,
			aspect + ": texture placed on the aperture")
		check(layout.mode == ("wide" if aspect == "wide" else "original"), aspect + ": layout mode")
	settings.preferences.set_values({"display.aspect": "original"})
	print("Playable display smoke failures: ", failures)
	quit(0 if failures.is_empty() else 1)
