extends RefCounted
## T-29025: pass validated resources and live entry state between scene owners.
## T-39007: the title Continue owner hands a selected save to the playable app.
## title_handoff: the clock, BIOS rand state and scene_work of NAME_ENT stage 1
## from flow/title_file_select.gd. No installed scene produces it yet (the boot
## screens are not drawn from original resources);
## tests/run_title_file_select.gd covers the consumers.

const SETUP_SCENE := "res://app/first_launch_setup.tscn"
const ENTRY_SCENE := "res://app/new_game_entry.tscn"
const PLAYABLE_SCENE := "res://app/playable.tscn"
const TITLE_SCENE := "res://app/title_continue.gd"

static func change_scene(host: Node, target: String, profile: Dictionary, entry: RefCounted = null, rng: RefCounted = null, continue_request: Dictionary = {}, title_handoff: Dictionary = {}) -> String:
	if host == null or not host.is_inside_tree():
		return "The game window is unavailable. Restart the application."
	if target not in [SETUP_SCENE, ENTRY_SCENE, PLAYABLE_SCENE, TITLE_SCENE]:
		return "This launch destination is unavailable. Return to setup."
	if not continue_request.is_empty() and target != PLAYABLE_SCENE:
		return "Continue can only open the game screen."
	if not title_handoff.is_empty() and target != ENTRY_SCENE:
		return "The title can only open New Game."
	var next: Node
	if target == TITLE_SCENE:
		var title_script: Variant = ResourceLoader.load(TITLE_SCENE)
		if not title_script is Script: return "The title screen could not be loaded. Repair the application."
		next = title_script.new()
	elif target == ENTRY_SCENE:
		# The installed launch owner authenticates opening resources and injects
		# the entry dependencies before its child scene enters the tree.
		var opening_script: Variant = ResourceLoader.load("res://app/first_arrival_boot.gd")
		if not opening_script is Script: return "The New Game screen could not be loaded. Repair the application."
		next = opening_script.new()
		next.set("initialization_profile","desktop-fresh-new-game-cold-prefix.v1")
		if not title_handoff.is_empty(): next.set("title_handoff", title_handoff)
	else:
		var packed: Variant = ResourceLoader.load(target)
		if not packed is PackedScene:
			return "A required game screen could not be loaded. Repair the application."
		next = packed.instantiate()
	if target != SETUP_SCENE:
		next.set("installed_launch_profile", profile.duplicate(true))
	if target == PLAYABLE_SCENE:
		next.set("installed_entry", entry)
		next.set("installed_entry_rng", rng)
		next.set("continue_request", continue_request.duplicate(true))
	var tree: SceneTree = host.get_tree()
	var previous: Node = tree.current_scene
	tree.root.add_child(next)
	tree.current_scene = next
	if previous != null and previous != next:
		previous.queue_free()
	return ""

static func show_repair(host: Node, detail: String) -> void:
	if host == null or not host.is_inside_tree():
		return
	var tree: SceneTree = host.get_tree()
	tree.set_meta("installed_launch_failure", "Game data is incomplete or changed. Import your original game again, or choose another content location.")
	if OS.get_environment("TOKIMEMO_ENTRY_DEBUG") == "1":
		printerr("Installed launch preflight: ", detail)
	change_scene(host, SETUP_SCENE, {})
