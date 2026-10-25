extends SceneTree
var checks: int = 0
var failures: int = 0
func require(value: bool, message: String) -> void:
	if not value: printerr("FAIL "+message); failures += 1
	checks += 1
func _initialize() -> void:
	call_deferred("run")
func tick_frames(count: int = 3) -> void:
	for i in count: await physics_frame
func click(scene: Node, label: String) -> void:
	var buttons: Array[Node] = scene.find_children("*","Button",true,false)
	for button in buttons:
		if button.text == label:
			button.pressed.emit(); return
	printerr("FAIL no button "+label); failures += 1
func capture(path: String) -> void:
	await RenderingServer.frame_post_draw
	var image: Image = root.get_texture().get_image()
	require(image.save_png(path)==OK,"capture saved")
func run() -> void:
	var scene: Control = load("res://app/new_game_entry.tscn").instantiate()
	root.add_child(scene)
	await tick_frames()
	require(scene.failure.is_empty() and scene.controller.step_id==1,"actual external-pack entry begins")
	await capture("/out/name-entry.png")
	for i in 3:
		click(scene,"Done / START"); await tick_frames()
	require(scene.controller.step_id==7 and scene.rng.draw_count==3,"three native name finish actions")
	click(scene,"Finish / START"); await tick_frames()
	require(scene.controller.step_id==9 and scene.rng.draw_count==6,"actual player UI action finalizes")
	click(scene,"Yes / START"); await tick_frames()
	require(scene.controller.stage==2 and scene.controller.step_id==1,"actual Shiori UI step")
	click(scene,"Finish / START"); await tick_frames()
	click(scene,"Yes / START"); await tick_frames()
	require(scene.failure.is_empty() and scene.controller.step_id==5,"actual UI commits independent native profile data")
	require(not scene.controller.blocked.is_empty(),"prologue dependency stop shown")
	await capture("/out/profile-committed.png")
	var file: FileAccess = FileAccess.open("/out/entry-trace.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"method":"actual queued UI buttons with authenticated original packs; no captured game state", "trace":scene.controller.trace,"rng":scene.rng.checkpoint(),"stage":scene.controller.stage,"step":scene.controller.step_id,"blocked":scene.controller.blocked},"\t")); file.close()
	print("PASS: %d rendered New Game UI checks"%checks)
	quit(1 if failures else 0)
