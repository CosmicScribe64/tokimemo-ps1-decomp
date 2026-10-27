extends RefCounted
## T-29025: pass validated resources and live entry state between scene owners.

const SETUP_SCENE := "res://app/first_launch_setup.tscn"
const ENTRY_SCENE := "res://app/new_game_entry.tscn"
const PLAYABLE_SCENE := "res://app/playable.tscn"

static func change_scene(host: Node, target: String, profile: Dictionary, entry: RefCounted = null, rng: RefCounted = null) -> String:
	if host == null or not host.is_inside_tree():
		return "The game window is unavailable. Restart the application."
	if target not in [SETUP_SCENE, ENTRY_SCENE, PLAYABLE_SCENE]:
		return "This launch destination is unavailable. Return to setup."
	var next: Node
	if target == ENTRY_SCENE:
		# The installed launch owner authenticates opening resources and injects
		# the entry dependencies before its child scene enters the tree.
		var opening_script: Variant = ResourceLoader.load("res://app/first_arrival_boot.gd")
		if not opening_script is Script: return "The New Game screen could not be loaded. Repair the application."
		next = opening_script.new()
		next.set("initialization_profile","desktop-fresh-new-game-cold-prefix.v1")
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
