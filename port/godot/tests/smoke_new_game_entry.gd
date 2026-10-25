extends SceneTree
var checks: int = 0
var failures: int = 0
func require(value: bool, message: String) -> void:
	if not value: printerr("FAIL "+message); failures += 1
	checks += 1
func _initialize() -> void:
	call_deferred("run")
func tick_frames(count: int = 3) -> void:
	for i in count:
		await physics_frame
		await process_frame
func click(scene: Node, label: String) -> void:
	var buttons: Array[Node] = scene.find_children("*","Button",true,false)
	for button in buttons:
		if button.text == label:
			button.grab_focus(); button.pressed.emit(); return
	printerr("FAIL no button "+label); failures += 1
func capture(path: String) -> void:
	await RenderingServer.frame_post_draw
	var image: Image = root.get_texture().get_image()
	require(image.save_png(path)==OK,"capture saved")
func visible_text(scene: Node) -> String:
	var result: String=""
	for label in scene.find_children("*","Label",true,false): result+=label.text+"\n"
	return result
func run() -> void:
	var scene: Control = load("res://app/new_game_entry.tscn").instantiate()
	root.add_child(scene)
	await tick_frames()
	require(scene.failure.is_empty() and scene.controller.step_id==1,"actual external-pack entry begins")
	require(visible_text(scene).begins_with("New Game\n") and not visible_text(scene).contains("RNG") and not visible_text(scene).contains("native"),"default entry view keeps technical details out")
	await capture("/out/name-entry.png")
	for i in 3:
		click(scene,"Katakana"); await tick_frames()
		if i == 1:
			# Real input dispatch while a grid-tab Button owns focus must still
			# produce START, rather than activating that Button or the DONE action.
			var enter := InputEventKey.new(); enter.keycode=KEY_ENTER; enter.pressed=true
			Input.parse_input_event(enter); await tick_frames()
			var released := InputEventKey.new(); released.keycode=KEY_ENTER; released.pressed=false; Input.parse_input_event(released)
		else:
			click(scene,"Done"); await tick_frames()
		if i < 2: require(scene.controller.tab == (1 if i == 1 else 0),"actual DONE resets and keyboard START retains katakana for next name")
	require(scene.controller.step_id==7 and scene.rng.draw_count==3,"three native name finish actions")
	click(scene,"Finish"); await tick_frames()
	require(scene.controller.step_id==9 and scene.rng.draw_count==6,"actual player UI action finalizes")
	require(visible_text(scene).contains("Birthday:") and not visible_text(scene).contains("month\":") and not visible_text(scene).contains("255"),"confirmation shows readable profile values")
	click(scene,"Confirm"); await tick_frames()
	require(scene.controller.stage==2 and scene.controller.step_id==1,"actual Shiori UI step")
	click(scene,"Finish"); await tick_frames()
	click(scene,"Confirm"); await tick_frames()
	require(scene.failure.is_empty() and scene.controller.step_id==5,"actual UI commits independent native profile data")
	require(not scene.controller.blocked.is_empty(),"prologue dependency stop shown")
	require(visible_text(scene).contains("The prologue is not available yet.") and not visible_text(scene).contains("unowned") and not visible_text(scene).contains("captured"),"continuation view is concise; detailed limitations remain in debug state")
	await capture("/out/profile-committed.png")
	var file: FileAccess = FileAccess.open("/out/entry-trace.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"method":"actual queued UI buttons with authenticated original packs; no captured game state", "trace":scene.controller.trace,"rng":scene.rng.checkpoint(),"stage":scene.controller.stage,"step":scene.controller.step_id,"blocked":scene.controller.blocked},"\t")); file.close()
	print("PASS: %d rendered New Game UI checks"%checks if not failures else "FAIL: rendered New Game UI checks")
	quit(1 if failures else 0)
