extends SceneTree
func _initialize() -> void: call_deferred("run")
func run() -> void:
	await process_frame
	root.get_node("ModernSettings").toggle_menu()
	await process_frame
	await RenderingServer.frame_post_draw
	var menu = root.get_node("ModernSettings").menu
	print("menu layout ", menu.size, " card ", menu.card.position, " ", menu.card.size, " minimum ",menu.card.get_combined_minimum_size(), " tabs ",menu.tabs.size, " tab ",menu.tabs.current_tab)
	root.get_texture().get_image().save_png("/work/.asset-cache/modern-settings.png")
	print("Settings screenshot captured")
	quit()
