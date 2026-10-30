extends SceneTree
## Physical input observer: no controller mutations or synthetic scene transitions.
const Store = preload("res://services/installed_pack_store.gd")
const Router = preload("res://app/boot_router.gd")
const App = preload("res://app/first_arrival_boot.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
var phase := 0
var capturing := false
var captures: Array[Dictionary] = []
var observer_frames := 0
var hold_started := -1
var held_position := Vector2i.ZERO
var held_tick := 0
var pause_checks: Array[Dictionary] = []

func _initialize() -> void: _start.call_deferred()

func _status(value: Dictionary) -> void:
	var file := FileAccess.open("/out/entry-keyboard-status.json",FileAccess.WRITE)
	file.store_string(JSON.stringify(value))

func _fail(message: String) -> void:
	_status({"phase":"failed","message":message}); printerr(message); quit(1)

func _start() -> void:
	var stage := "/out/content-install/.staging/prepared-original"
	var store := Store.new("/out/content-install")
	var result := store.publish_stage(stage,FileAccess.get_sha256(stage.path_join("manifest.json")),Profile.RELEASE,[Profile.ENTRY_CAP,Profile.OPENING_RESOURCES_CAP])
	if not result.get("valid",false): _fail("publication failed: "+str(result)); return
	var host := Node.new(); root.add_child(host); current_scene = host
	if Router.new(store).route(host).get("action") != "playable": _fail("installed route failed")
	var requested := OS.get_environment("TOKIMEMO_TEST_WINDOW")
	if not requested.is_empty():
		if requested not in ["1024x768","1280x720"]: _fail("unsupported explicit resize test"); return
		var dimensions := requested.split("x")
		root.size = Vector2i(int(dimensions[0]),int(dimensions[1]))
		root.position = Vector2i.ZERO

func _process(_delta: float) -> bool:
	observer_frames += 1
	if observer_frames == 60:
		print("NAME_TAB_OBSERVER ",current_scene," script=",current_scene.get_script()," notices=",get_nodes_in_group("settings_notices").size())
		root.get_texture().get_image().save_png("/out/observer-wait.png")
	if capturing or not get_nodes_in_group("settings_notices").is_empty(): return false
	var app := current_scene as App
	if app == null: return false
	if not app.failure.is_empty(): _fail(app.failure); return false
	if not is_instance_valid(app._entry): return false
	var scene = app._entry
	if not scene.failure.is_empty(): _fail(scene.failure); return false
	var entry = scene.controller
	var pointer_file := FileAccess.open("/out/name-pointer-live.json",FileAccess.WRITE)
	pointer_file.store_string(JSON.stringify({"pointer":scene.name_pointer.snapshot(),"tick":scene.tick,"available":scene._source_pointer_ready()}))
	if phase >= 7:
		_observe_pause(scene)
		return false
	if observer_frames % 60 == 0:
		var live := FileAccess.open("/out/name-tabs-live.json",FileAccess.WRITE)
		live.store_string(JSON.stringify({"phase":phase,"snapshot":scene.differential_snapshot()}))
	if entry.stage != 1 or entry.step_id != 1: return false
	var ready: bool = [entry.tab == 0,entry.tab == 2,entry.tab == 2 and entry.page_scroll == 1,
		entry.tab == 3,entry.tab == 3 and entry.reading == 1 and entry.page_scroll == 0,
		entry.cursor == 1,entry.cursor == 0][mini(phase,6)]
	if ready: capturing = true; _capture.call_deferred(scene)
	return false

func _capture(scene: Node) -> void:
	await process_frame
	await RenderingServer.frame_post_draw
	if scene.last_rendered_image == null: _fail("native image missing"); return
	var path := "/out/tabs-%d.frame.png" % phase
	scene.last_rendered_image.save_png(path)
	root.get_texture().get_image().save_png("/out/tabs-%d.viewport.png" % phase)
	var row: Dictionary = scene.differential_snapshot()
	row["frame_file"] = path.get_file(); row["frame_sha256"] = FileAccess.get_sha256(path)
	row["draw_records"] = scene.entry_images.last_draw_records.duplicate(true)
	captures.append(row)
	if phase == 6:
		var expected: PackedByteArray = PackedByteArray(scene.controller._content.candidate_lists[1].slice(0,2))
		if scene.controller.names.family.slice(0,2) != expected: _fail("physical Kanji click did not insert source candidate"); return
		if OS.get_environment("TOKIMEMO_NAME_KEYBOARD") == "1":
			phase = 7; capturing = false
			_status({"phase":"tabs_pause","native_canvas":scene.differential_snapshot().native_canvas})
			return
		_finish(); return
	_status({"phase":"tabs_%d" % phase,"native_canvas":scene.differential_snapshot().native_canvas})
	phase += 1
	capturing = false

func _finish() -> void:
	var file := FileAccess.open("/out/name-tabs-evidence.json",FileAccess.WRITE)
	file.store_string(JSON.stringify({"captures":captures,"pause_checks":pause_checks,
		"input_method":"physical keyboard" if OS.get_environment("TOKIMEMO_NAME_KEYBOARD") == "1" else "physical mouse",
		"checks":["Symbols tab","scroll row","Kanji tab","reading selection resets scroll","candidate insertion","native name-slot selection"]},"\t"))
	_status({"phase":"passed"}); quit()

func _observe_pause(scene: Node) -> void:
	var settings := root.get_node("ModernSettings")
	var suspended: bool = (settings.menu.visible and paused) if phase in [7,8] else scene.focus_paused
	var expect_paused: bool = phase in [7,9]
	if suspended != expect_paused: return
	if hold_started < 0:
		hold_started = Time.get_ticks_msec()
		if expect_paused: held_position = scene.name_pointer.position; held_tick = scene.tick
	if Time.get_ticks_msec()-hold_started < 200: return
	if scene.name_pointer.position != held_position or (expect_paused and scene.tick != held_tick):
		_fail("physical pause/focus changed source cursor or advanced paused entry")
		return
	var snapshot: Dictionary = scene.differential_snapshot()
	if not expect_paused and (not snapshot.sampled_input is Dictionary or snapshot.sampled_input.held != 0 or snapshot.sampled_input.pressed != 0):
		_fail("held input leaked across settings/focus recovery")
		return
	pause_checks.append({"phase":phase,"paused":expect_paused,"snapshot":snapshot})
	hold_started = -1
	if phase == 10: _finish(); return
	phase += 1
	_status({"phase":{8:"tabs_resume",9:"tabs_focus_away",10:"tabs_focus_back"}[phase],"native_canvas":snapshot.native_canvas})
