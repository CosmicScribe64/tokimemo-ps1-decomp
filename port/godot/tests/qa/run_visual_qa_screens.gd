extends SceneTree
## T-39060 visual QA (X11 only; never a fidelity verdict): window captures of
## the first-launch Welcome step, every settings page (before a game scene and
## inside one) and the English-mode name entry (Latin typing on the native
## Symbols page), at each size in SIZES and each aspect in ASPECTS.
## Writes /out/qa/<screen>-<aspect>-<W>x<H>.png and /out/qa/screens.json.
## Needs /out/content-install/.staging/prepared-original: an installed bundle
## with language/en (English) inside. Runner: tools/port/run_visual_qa.sh.
const Store = preload("res://services/installed_pack_store.gd")
const Router = preload("res://app/boot_router.gd")
const Profile = preload("res://services/installed_launch_profile.gd")
const RoutePrefs = preload("res://tests/qa/route_presentation_prefs.gd")
const SETUP_SCENE := "res://app/first_launch_setup.tscn"
const SIZES := [Vector2i(1280, 720), Vector2i(1920, 1080), Vector2i(960, 720)]
const ASPECTS := ["original", "wide"]
const LATIN_TEXT := "Ann"
var settings: Node
var record: Array = []
var failures: Array[String] = []
var _entry: Node

func _initialize() -> void:
	node_added.connect(_node_added)
	_run.call_deferred()

func _node_added(node: Node) -> void:
	if _entry == null and node.get_script() != null and node.get_script().resource_path == "res://app/new_game_entry_boot.gd":
		_entry = node

func _fail(message: String) -> void:
	failures.append(message); printerr("QA: ", message)

func _run() -> void:
	DirAccess.make_dir_recursive_absolute("/out/qa")
	settings = root.get_node("ModernSettings")
	var store := Store.new("/out/content-install")
	var stage := "/out/content-install/.staging/prepared-original"
	var published := store.publish_stage(stage, FileAccess.get_sha256(stage.path_join("manifest.json")), Profile.RELEASE, [Profile.ENTRY_CAP, Profile.OPENING_RESOURCES_CAP])
	if not published.get("valid", false):
		_fail("bundle publication failed: " + str(published.get("message", ""))); _finish(); return
	settings.note_installed_root(store.installed_pack_root())
	var error: String = settings.select_language("en")
	if not error.is_empty(): _fail("English could not start: " + error)
	for i: int in 600:
		await process_frame
		if not settings.language_bundle.loading: break
	if settings.language_bundle.choice != "en": _fail("English bundle not applied: " + settings.language_bundle.notice)
	# Welcome step over the real setup scene (project 640x480 canvas layout).
	var setup: Control = load(SETUP_SCENE).instantiate()
	setup.welcome_enabled = true
	setup.welcome_marker_path = "user://qa-welcome-marker.json"
	root.add_child(setup)
	await process_frame
	# The setup shows the Welcome step after an import; open it directly.
	if not setup._show_welcome(): _fail("Welcome step did not open")
	await _sweep("welcome", Callable())
	await _sweep_settings("settings-setup")
	setup.queue_free()
	await process_frame
	# Name entry through the normal installed boot, then Latin typing.
	var host := Node.new(); root.add_child(host); current_scene = host
	var decision: Dictionary = Router.new(store).route(host)
	if decision.get("target") != "res://app/new_game_entry.tscn": _fail("boot did not choose the entry: " + str(decision))
	for i: int in 300:
		await process_frame
		if _entry != null and _entry.is_node_ready() and _entry.controller.stage == 1 and _entry.controller.step_id == 1: break
	if _entry == null: _fail("entry scene not reached"); _finish(); return
	if not _entry.latin.enabled: _fail("Latin typing is not enabled in English mode")
	_entry.latin.push_text(LATIN_TEXT)
	for i: int in 240:
		await physics_frame
		if _entry.latin.pending() == 0: break
	for i: int in 10: await physics_frame
	var typed := PackedByteArray(_entry.controller.names.family).slice(0, LATIN_TEXT.length() * 2)
	if typed != _entry.latin.encode(LATIN_TEXT): _fail("Latin text not entered: " + typed.hex_encode())
	await _sweep("name-entry-latin", Callable())
	await _sweep_settings("settings-ingame")
	_finish()

func _sweep_settings(prefix: String) -> void:
	settings.toggle_menu()
	await process_frame
	var menu: Control = settings.menu
	for index: int in menu.page_titles.size():
		var title: String = menu.page_titles[index]
		await _sweep("%s-%d-%s" % [prefix, index, title.to_lower().replace(" & ", "-").replace(" ", "-")], func() -> void: menu._select_page(index))
	settings.toggle_menu()
	await process_frame

## Captures one screen at every aspect and size; prepare runs before each.
func _sweep(screen: String, prepare: Callable) -> void:
	var window := root.get_window()
	for aspect: String in ASPECTS:
		settings.preferences.set_values({"display.aspect": aspect})
		# An open menu keeps its unapplied draft across rebuilds; reset it from
		# the effective settings so each capture shows the state it is named for.
		var menu: Variant = settings.get("menu")
		if menu != null and is_instance_valid(menu) and menu.visible:
			menu.draft = settings.preferences.effective_settings().display.duplicate(true)
			menu.rebuild()
			if str(menu.draft.get("aspect")) != aspect: _fail("settings draft still shows %s for the %s capture" % [str(menu.draft.get("aspect")), aspect])
		for size: Vector2i in SIZES:
			window.size = size
			if prepare.is_valid(): prepare.call()
			for i: int in 4: await process_frame
			await RenderingServer.frame_post_draw
			var image := RoutePrefs.window_image(root.get_texture().get_image(), window.size)
			var name := "%s-%s-%dx%d" % [screen, aspect, size.x, size.y]
			if image == null: _fail("no window image for " + name); continue
			image.save_png("/out/qa/%s.png" % name)
			record.append({"name": name, "window": [window.size.x, window.size.y], "image": [image.get_width(), image.get_height()]})

func _finish() -> void:
	var file := FileAccess.open("/out/qa/screens.json", FileAccess.WRITE)
	file.store_string(JSON.stringify({"captures": record, "failures": failures}, "\t")); file.close()
	print(("PASS: visual QA screens (%d captures)" % record.size()) if failures.is_empty() else "FAIL: visual QA screens " + str(failures))
	quit(0 if failures.is_empty() else 1)
