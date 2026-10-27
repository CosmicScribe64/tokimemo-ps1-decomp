extends SceneTree
## Controlled window-system input only; this test constructs no InputEvent.

const Router = preload("res://app/boot_router.gd")
var expected := [[1,1], [1,3], [1,5], [1,7], [1,9], [2,1], [2,3], [2,5]]
var phase := 0
var elapsed := 0
var checks := 0
var initial_rng: int
var capture_started := false
var finishing := false

func _initialize() -> void:
	_start.call_deferred()

func _start() -> void:
	var host := Node.new()
	root.add_child(host)
	current_scene = host
	var decision: Dictionary = Router.new().route(host)
	if decision.get("target") != "res://app/new_game_entry.tscn":
		_failed("remembered install did not select interactive entry")

func _process(_delta: float) -> bool:
	if finishing:
		return false
	elapsed += 1
	if elapsed > 2400:
		_failed("keyboard entry exceeded bounded frame budget")
		return false
	var scene: Node = current_scene
	if scene == null or not scene.get_script().resource_path.ends_with("new_game_entry_boot.gd"):
		return false
	if not scene.failure.is_empty():
		_failed("interactive entry failed")
		return false
	var observed := [scene.controller.stage, scene.controller.step_id]
	if observed != expected[phase]:
		return false
	if phase == 0 and not capture_started:
		capture_started = true
		initial_rng = scene.rng.state
		_capture.call_deferred("keyboard-entry-start")
	if phase == 7:
		finishing = true
		_finish.call_deferred()
		return false
	var marker := FileAccess.open("/out/entry-keyboard-status.json", FileAccess.WRITE)
	marker.store_string(JSON.stringify({"phase":"entry_%d" % phase})); marker.close()
	checks += 1
	phase += 1
	return false

func _capture(name: String) -> void:
	await process_frame
	await RenderingServer.frame_post_draw
	if root.get_texture().get_image().save_png("/out/" + name + ".png") != OK:
		_failed("viewport capture failed")

func _finish() -> void:
	await _capture("keyboard-entry-complete")
	var scene: Node = current_scene
	if not scene.profile_complete or scene.profile_handoff.get("rng") != scene.rng or scene.profile_handoff.get("entry") != scene.controller:
		_failed("completed keyboard profile lost live ownership")
		return
	var record := {"phase":"passed","checks":checks + 1,"input_dispatch":"X11/libX11",
		"entry_draw_count":scene.rng.draw_count,"initial_rng":initial_rng,"final_rng":scene.rng.state,
		"opening_available":false,"live_handoff":true}
	var marker := FileAccess.open("/out/entry-keyboard-status.json", FileAccess.WRITE)
	marker.store_string(JSON.stringify(record, "\t")); marker.close()
	print("T29025 X11 entry: ", checks + 1, " checks PASS")
	await create_timer(0.2).timeout
	quit(0)

func _failed(message: String) -> void:
	finishing = true
	var marker := FileAccess.open("/out/entry-keyboard-status.json", FileAccess.WRITE)
	marker.store_string(JSON.stringify({"phase":"failed","message":message})); marker.close()
	printerr(message)
	quit(1)
