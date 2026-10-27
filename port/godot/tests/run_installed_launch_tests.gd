extends SceneTree
## T-29025: real original import, restart, interactive ownership and failure gates.

const Backend = preload("res://services/original_game_import_backend.gd")
const Store = preload("res://services/installed_pack_store.gd")
const Flow = preload("res://app/first_launch_import_flow.gd")
const Router = preload("res://app/boot_router.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const Launch = preload("res://app/installed_launch.gd")
const EntryScene = preload("res://app/new_game_entry.tscn")
var checks := 0
var failures: Array[String] = []
var report: Dictionary = {}

func _initialize() -> void:
	_run.call_deferred()

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)
		printerr("FAIL: ", message)

func _run() -> void:
	var storage: String = OS.get_user_data_dir().path_join("content-install")
	var store = Store.new(storage)
	var mode: String = OS.get_environment("T29025_TEST_MODE")
	var original: String = "/original/track1.bin" if mode == "track" else ("/out/original-input.zip" if mode == "zip" else "/original/files")
	if mode != "restart":
		var flow = Flow.new(store, Backend.new())
		_check(flow.choose_source(original, Router.DEFAULT_CAPABILITIES).is_empty(), "real original import starts")
		for step in range(16):
			flow.tick()
			if flow.state not in ["verifying", "importing"]:
				break
		_check(flow.state == "ready", "native import publishes authenticated entry: " + flow.error)
		_check(flow.latest_result.get("available_capabilities") == [Profile.ENTRY_CAP], "static preview does not authorize gameplay")
		_check(not str(flow.latest_result.get("launch_profile", {}).get("paths", {}).get("font_root", "")).contains("/.staging/"), "published locators survive atomic rename")
		var cancel_flow = Flow.new(store, Backend.new())
		_check(cancel_flow.choose_source(original, Router.DEFAULT_CAPABILITIES).is_empty(), "repair import starts")
		cancel_flow.tick()
		_check(cancel_flow.cancel().is_empty(), "repair import can cancel")
		cancel_flow.tick()
		_check(cancel_flow.state == "cancelled", "cancel retains previous install")
	var decision: Dictionary = Router.new(store).check_boot_state()
	_check(decision.get("action") == "playable" and decision.get("target") == Launch.ENTRY_SCENE, "remembered pack routes into interactive entry")
	var profile: Dictionary = decision.get("launch_profile", {})
	_check(profile.get("available_capabilities", []).has(Profile.ENTRY_CAP), "authenticated entry capability survives restart")
	_check(not profile.get("available_capabilities", []).has(Profile.GAMEPLAY_CAP), "missing gameplay resources stay explicit")
	var scene: Node = EntryScene.instantiate()
	scene.installed_launch_profile = profile
	root.add_child(scene)
	current_scene = scene
	_check(scene.failure.is_empty(), "interactive scene starts without development environment: " + scene.failure)
	_check(scene.controller.stage == 1 and scene.controller.step_id == 0 and scene.rng.draw_count == 0, "startup does not choose names or consume profile RNG")
	scene.set_physics_process(false)
	scene._physics_process(0.0)
	_check(scene.controller.step_id == 1, "one native initialization tick exposes name controls")
	await _capture("installed-name-entry")
	var before_rng: int = scene.rng.state
	scene._send({"kind": "cell", "column": 0, "row": 0})
	scene._physics_process(0.0)
	_check(scene.controller.cursor == 1 and scene.rng.state == before_rng, "selected original kana preserved without random draw")
	var selected_pair: PackedByteArray = scene.controller.names.family.slice(0, 2)
	for action: Dictionary in [{"kind":"done"}, {"kind":"done"}, {"kind":"done"}]:
		scene._send(action)
		scene._physics_process(0.0)
		scene._physics_process(0.0)
	_check(scene.controller.step_id == 7, "three user completions reach player profile")
	for selection: int in [3, 12, 2]:
		scene._send({"kind":"select", "value":selection}); scene._physics_process(0.0)
		scene._send({"kind":"circle"}); scene._physics_process(0.0)
	scene._physics_process(0.0)
	scene._send({"kind":"yes"}); scene._physics_process(0.0)
	scene._physics_process(0.0)
	for selection: int in [5, 6, 1]:
		scene._send({"kind":"select", "value":selection}); scene._physics_process(0.0)
		scene._send({"kind":"circle"}); scene._physics_process(0.0)
	scene._physics_process(0.0)
	scene._send({"kind":"yes"}); scene._physics_process(0.0)
	scene._physics_process(0.0)
	_check(scene.failure.is_empty() and scene.profile_complete, "actual profile actions commit without scripted defaults")
	_check(scene.controller.names.family.slice(0, 2) == selected_pair, "entered family glyph survives native commit")
	_check(scene.controller.player == {"month":3,"day":12,"blood":2} and scene.controller.shiori == {"month":5,"day":6,"blood":1}, "explicit user birthdays and blood selections survive commit")
	_check(scene.profile_handoff.get("entry") == scene.controller and scene.profile_handoff.get("rng") == scene.rng, "handoff retains exact live controller and RNG objects")
	var final_state: int = scene.rng.state
	scene._physics_process(0.0)
	_check(scene.rng.state == final_state, "completed entry does not advance or rerun defaults")
	_check(scene.get_tree().current_scene == scene, "unsupported opening remains visible and does not auto-skip")
	await _capture("installed-profile-complete")
	var manifest_path: String = str(decision.get("pack_root", "")).path_join("manifest.json")
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	var unsafe: Dictionary = manifest.duplicate(true)
	unsafe.roles.font_root = "../elsewhere"
	_check(not Profile.read_pack(decision.pack_root, unsafe).get("valid", true), "manifest traversal is rejected")
	unsafe = manifest.duplicate(true)
	unsafe.files[unsafe.roles.name_entry_rules].sha256 = "0".repeat(64)
	_check(not Profile.read_pack(decision.pack_root, unsafe).get("valid", true), "changed inventoried file is rejected")
	var preview_check: Dictionary = store.check_installed(["prologue.nbn_native_cells.static_preview"])
	_check(not preview_check.get("launch_ready", true), "entry pack cannot satisfy historical preview capability")
	var unknown := EntryScene.instantiate()
	unknown.installed_launch_profile = {"paths":{}}
	root.add_child(unknown)
	unknown.set_physics_process(false)
	_check(not unknown.failure.is_empty(), "invalid installed locators fail visibly without environment fallback")
	var recovery_buttons: Array[String] = []
	for container: Node in unknown.panel.get_children():
		for control: Node in container.get_children():
			if control is Button:
				recovery_buttons.append(control.text)
	_check(recovery_buttons.has("Return to setup") and recovery_buttons.has("Exit"), "failed entry offers retry and exit controls")
	unknown.free()
	report = {"checks":checks,"failures":failures,"mode":mode,"manifest_sha256":decision.get("manifest_sha256"),
		"entry_draw_count":scene.rng.draw_count,"startup_seed":scene.load_diagnostics.get("startup_seed", -1),
		"profile_handoff_retained":not scene.profile_handoff.is_empty(),"normal_opening_available":false}
	var file := FileAccess.open("/out/installed-launch-" + (mode if not mode.is_empty() else "write") + ".json", FileAccess.WRITE)
	file.store_string(JSON.stringify(report, "\t") + "\n"); file.close()
	print("T29025 installed launch: ", checks, " checks, ", failures.size(), " failures")
	scene.free()
	quit(0 if failures.is_empty() else 1)

func _capture(name: String) -> void:
	if DisplayServer.get_name() == "headless":
		return
	await process_frame
	await process_frame
	await RenderingServer.frame_post_draw
	_check(root.get_texture().get_image().save_png("/out/" + name + ".png") == OK, "actual viewport capture saved: " + name)
