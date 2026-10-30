extends SceneTree
## T-39004: real installed entry and physical X11 input; draft opt-in only here.
const Store = preload("res://services/installed_pack_store.gd")
const Router = preload("res://app/boot_router.gd")
const App = preload("res://app/first_arrival_boot.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const Images = preload("res://services/translated_image_store.gd")
var expected := [[1,1],[1,3],[1,5],[1,7],[1,9],[2,1],[2,3]]
var phase := 0
var capturing := false
var translations := Images.new()
var captures: Array[Dictionary] = []
var translation_path := ""

func _initialize() -> void:
	_start.call_deferred()

func _status(value: Dictionary) -> void:
	var file := FileAccess.open("/out/entry-keyboard-status.json",FileAccess.WRITE)
	file.store_string(JSON.stringify(value))

func _fail(message: String) -> void:
	_status({"phase":"failed","message":message})
	printerr(message)
	quit(1)

func _start() -> void:
	var stage := "/out/content-install/.staging/prepared-original"
	var store := Store.new("/out/content-install")
	var published := store.publish_stage(stage,FileAccess.get_sha256(stage.path_join("manifest.json")),Profile.RELEASE,[Profile.ENTRY_CAP,Profile.OPENING_RESOURCES_CAP])
	if not published.get("valid",false):
		_fail("installed original publication failed: "+str(published))
		return
	var path := ProjectSettings.globalize_path("res://../..").simplify_path().path_join(".asset-cache/translation-assets/name-runtime-drafts-v1/manifest.json")
	if not OS.get_environment("TOKIMEMO_TEST_TRANSLATED_IMAGES").is_empty(): path = OS.get_environment("TOKIMEMO_TEST_TRANSLATED_IMAGES")
	translation_path = path
	if not translations.open_pack(path,FileAccess.get_sha256(path),true):
		_fail(translations.error)
		return
	var host := Node.new()
	root.add_child(host)
	current_scene = host
	var route := Router.new(store).route(host)
	if route.get("action") != "playable": _fail("installed route failed")

func _process(_delta: float) -> bool:
	if capturing: return false
	# Observe the production first-run hint's real expiry; do not hide it for capture.
	if not get_nodes_in_group("settings_notices").is_empty(): return false
	var app := current_scene as App
	if app == null: return false
	if not app.failure.is_empty():
		_fail(app.failure)
		return false
	if phase == expected.size() and app.consumer != null and app.consumer.opening.state != null:
		var work: Variant = app.consumer.opening.state.opening.get("prologue_text_work")
		if work is Dictionary and work.get("line_id") == 0 and work.get("pos",0) >= work.get("total",1):
			capturing = true
			app._active = false
			_capture_school.call_deferred(app)
		return false
	if not is_instance_valid(app._entry): return false
	var scene = app._entry
	if scene.entry_images == null:
		_fail("actual entry image renderer unavailable: " + scene.failure)
		return false
	if phase < expected.size() and [scene.controller.stage,scene.controller.step_id] == expected[phase]:
		if phase == 1 and scene.controller.names.family.slice(0,4).hex_encode() != "82a28140":
			_fail("physical source-cell click and mouse-only backtracking did not replace the first slot")
			return false
		if phase == 4 and scene.controller.player != {"month":0,"day":0,"blood":0}:
			_fail("physical birthday hit regions did not select January 1 / A")
			return false
		capturing = true
		_capture.call_deferred(scene)
	return false

func _capture(scene: Node) -> void:
	# Hold this actual native draw checkpoint across language repaint comparisons.
	# This freezes only the isolated observer's entry scene, not production logic.
	scene.set_physics_process(false)
	var settings := root.get_node("ModernSettings")
	var state := [scene.controller.stage,scene.controller.step_id]
	var evidence: Dictionary = {"stage":state[0],"step":state[1],"drafts":true,"actual_scene":scene.scene_file_path}
	evidence["translation_manifest_sha256"] = FileAccess.get_sha256(translation_path)
	evidence["entry_physics_held_for_language_repaint"] = true
	evidence["settings_notice_count"] = get_nodes_in_group("settings_notices").size()
	evidence["names_hex"] = {}
	for part: String in ["family","given","nickname"]:
		evidence.names_hex[part] = scene.controller.names[part].hex_encode()
	evidence["player"] = scene.controller.player.duplicate(true)
	evidence["shiori"] = scene.controller.shiori.duplicate(true)
	evidence["tab"] = scene.controller.tab
	evidence["cursor"] = scene.controller.cursor
	for language: String in ["native","english","restored"]:
		settings.translated_images = translations if language == "english" else null
		settings.preferences_changed.emit(settings.preferences.effective_settings())
		await process_frame
		await RenderingServer.frame_post_draw
		if [scene.controller.stage,scene.controller.step_id] != state:
			_fail("physical input advanced while capture was pending")
			return
		var screenshot := root.get_texture().get_image()
		var filename := "%s-%d-%d.png" % [language,state[0],state[1]]
		if screenshot.save_png("/out/"+filename) != OK:
			_fail("viewport screenshot failed")
			return
		evidence[language] = {"file":filename,"sha256":FileAccess.get_sha256("/out/"+filename),"replaced_frames":scene.entry_images.replaced_frames.duplicate()}
		evidence[language]["differential_snapshot"] = scene.differential_snapshot()
		evidence[language]["draw_records"] = scene.entry_images.last_draw_records.duplicate(true)
		evidence[language]["native_frame"] = _save_native_frame(scene.last_rendered_image,"%s-%d-%d.frame.png" % [language,state[0],state[1]])
		if evidence[language].native_frame.is_empty(): return
		if evidence[language].native_frame.pixel_sha256 != evidence[language].differential_snapshot.composed_image.pixel_sha256:
			_fail("native image differs from corresponding post-draw snapshot")
			return
	if evidence.native.sha256 != evidence.restored.sha256 or evidence.native.sha256 == evidence.english.sha256:
		_fail("actual viewport switch/fallback comparison failed")
		return
	captures.append(evidence)
	if phase == expected.size()-1 and OS.get_environment("TOKIMEMO_CAPTURE_FIRST_SCHOOL") != "1":
		_status({"phase":"passed","captures":captures,"scope":"actual installed entry screens; draft images; sequence timing parity unverified"})
		quit(0)
		return
	scene.set_physics_process(true)
	_status({"phase":"entry_%d" % phase,"native_canvas":scene.differential_snapshot().native_canvas})
	phase += 1
	capturing = false

func _capture_school(app: App) -> void:
	await RenderingServer.frame_post_draw
	var state = app.consumer.opening.state
	var image := root.get_texture().get_image()
	if image.save_png("/out/first-school-native.png") != OK:
		_fail("first-school viewport capture failed")
		return
	var record := {"game_mode":state.game_mode,"stage":state.mode_stage,"step":state.mode_step,
		"substep":state.substep,"frame_count":state.frame_count,
		"text_work":state.opening.prologue_text_work.duplicate(true),
		"hud":state.opening.get("prologue_hud_live",{}).duplicate(true),
		"brightness":state.opening.get("screen_brightness"),
		"entry_context":app.consumer.live_handoff().context,
		"file":"first-school-native.png","sha256":FileAccess.get_sha256("/out/first-school-native.png"),
		"scope":"actual source first line; elapsed-clock and emulator pixel parity unverified"}
	record["native_frame"] = _save_native_frame(app.last_rendered_image,"first-school-native.frame.png")
	if record.native_frame.is_empty(): return
	_status({"phase":"passed","captures":captures,"first_school":record})
	quit(0)

func _save_native_frame(image: Image, filename: String) -> Dictionary:
	if image == null or image.get_size() != Vector2i(320,240):
		_fail("actual composed image is not native320x240")
		return {}
	if image.save_png("/out/"+filename) != OK:
		_fail("actual composed native image could not be saved")
		return {}
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(image.get_data())
	return {"file":filename,"sha256":FileAccess.get_sha256("/out/"+filename),
		"pixel_sha256":hash.finish().hex_encode(),"width":320,"height":240,
		"format":image.get_format(),"origin":"actual-renderer-after-frame_post_draw","resized":false}
