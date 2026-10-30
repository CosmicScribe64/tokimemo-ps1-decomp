extends SceneTree
## T-30002 WP7: X11 captures of every settings page, the pause menu and the
## Welcome step for visual review. Run under Xvfb (not headless). Output goes
## to SETTINGS_CAPTURE_DIR (default /work/.asset-cache), never into the repo.
## Fails when the card leaves the window or a label is smaller than 14 px.
const Pause = preload("res://ui/pause_menu.gd")
const Welcome = preload("res://ui/welcome_step.gd")
var failures := 0

func _initialize() -> void: call_deferred("run")

func run() -> void:
	var out := OS.get_environment("SETTINGS_CAPTURE_DIR")
	if out.is_empty(): out = "/work/.asset-cache"
	DirAccess.make_dir_recursive_absolute(out)
	var tag := "%dx%d" % [root.size.x, root.size.y]
	await process_frame
	var runtime: Node = root.get_node("ModernSettings")
	runtime.toggle_menu()
	await process_frame
	var menu: Control = runtime.menu
	for index: int in menu.page_titles.size():
		menu._select_page(index); menu.navigation[index].grab_focus()
		await process_frame; await RenderingServer.frame_post_draw
		_check_layout(menu, menu.page_titles[index])
		var slug: String = menu.page_titles[index].to_lower().replace(" & ", "-").replace(" ", "-")
		root.get_texture().get_image().save_png(out.path_join("settings-%s-%s.png" % [tag, slug]))
	menu._select_page(0); (menu.focus_items["advanced_open"] as Button).pressed.emit()
	await process_frame; await RenderingServer.frame_post_draw
	root.get_texture().get_image().save_png(out.path_join("settings-%s-language-advanced.png" % tag))
	menu.advanced_open = false
	runtime.toggle_menu(); await process_frame
	var pause: Control = Pause.install(runtime)
	pause.open(); await process_frame; await RenderingServer.frame_post_draw
	root.get_texture().get_image().save_png(out.path_join("pause-%s.png" % tag))
	pause.resume(); pause.queue_free()
	var welcome: Control = Welcome.new(); welcome.runtime = runtime; welcome.marker_path = "user://welcome-capture.json"
	root.add_child(welcome); await process_frame; await RenderingServer.frame_post_draw
	root.get_texture().get_image().save_png(out.path_join("welcome-%s.png" % tag))
	print("Settings captures written for %s (%d pages)" % [tag, menu.page_titles.size()])
	quit(1 if failures > 0 else 0)

func _check_layout(menu: Control, page: String) -> void:
	var area := Rect2(Vector2.ZERO, menu.get_viewport_rect().size)
	var card := Rect2(menu.card.position, menu.card.size * menu.card.scale)
	if not area.grow(0.5).encloses(card):
		printerr("%s: card %s leaves window %s" % [page, card, area]); failures += 1
	var physical_scale: float = root.get_final_transform().get_scale().y * menu.card.scale.y
	for label: Label in menu.find_children("*", "Label", true, false):
		if not label.is_visible_in_tree(): continue
		var size: int = label.get_theme_font_size("font_size")
		if size * physical_scale < 14.0 - 0.01:
			printerr("%s: label '%s' is %.1f physical px" % [page, label.text.left(30), size * physical_scale]); failures += 1
