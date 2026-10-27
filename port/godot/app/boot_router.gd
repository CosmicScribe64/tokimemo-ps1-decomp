class_name BootRouter
extends RefCounted
## Boot routing coordinator for Tokimeki Memorial (T-29016).
## Preflights installed content packs against required launch capabilities.
## Directs startup to playable gameplay when valid, or first-launch setup when required.

const StoreScript = preload("res://services/installed_pack_store.gd")
const BackendScript = preload("res://services/original_game_import_backend.gd")
const LaunchProfile = preload("res://services/installed_launch_profile.gd")
const Launch = preload("res://app/installed_launch.gd")

const SETUP_SCENE_PATH := "res://app/first_launch_setup.tscn"
const DEFAULT_PLAYABLE_SCENE_PATH := "res://app/new_game_entry.tscn"
const DEFAULT_CAPABILITIES: Array[String] = [LaunchProfile.ENTRY_CAP,LaunchProfile.OPENING_RESOURCES_CAP]
const ENV_FORCE_SETUP := "TOKIMEMO_FORCE_SETUP"
const ENV_PLAYABLE_SCENE := "TOKIMEMO_PLAYABLE_SCENE"

var store: RefCounted
var backend: Object
var required_capabilities: Array[String] = []

func _init(store_override: RefCounted = null, backend_override: Object = null, capabilities: Array = []) -> void:
	store = store_override if store_override != null else StoreScript.new()
	backend = backend_override if backend_override != null else BackendScript.new()
	if capabilities.is_empty():
		required_capabilities = DEFAULT_CAPABILITIES.duplicate()
	else:
		required_capabilities.assign(capabilities)

func is_setup_forced() -> bool:
	if OS.get_environment(ENV_FORCE_SETUP) == "1":
		return true
	for arg: String in OS.get_cmdline_user_args():
		if arg == "--setup" or arg == "--reimport" or arg == "--repair":
			return true
	return false

func check_boot_state(caps_override: Array = []) -> Dictionary:
	var check_caps: Array = caps_override if not caps_override.is_empty() else required_capabilities
	var installed: Dictionary = store.check_installed(check_caps)
	var force_setup := is_setup_forced()
	var state := str(installed.get("state", "failed"))
	var launch_ready: bool = bool(installed.get("launch_ready", false))
	var profile: Dictionary = installed.get("launch_profile", {})
	if launch_ready and not profile.get("available_capabilities", []).has(LaunchProfile.ENTRY_CAP):
		launch_ready = false
		state = "installed_incomplete"
	
	if state == "ready" and launch_ready and not force_setup:
		return {
			"action": "playable",
			"launch_ready": true,
			"target": _determine_playable_target(""),
			"reason": "Validated original name-entry data is installed. Gameplay remains unavailable.",
			"installed": installed,
			"pack_root": str(installed.get("pack_root", "")),
			"manifest_sha256": str(installed.get("manifest_sha256", "")),
			"launch_profile": installed.get("launch_profile", {}).duplicate(true),
		}
	
	var reason := "No valid pack installed."
	if force_setup:
		reason = "Repair or re-import was explicitly requested."
	elif state == "installed_incomplete":
		reason = "Installed pack lacks required capabilities: " + str(installed.get("missing_capabilities", []))
	elif state == "failed":
		reason = "Installed pack validation failed: " + str(installed.get("message", ""))
	
	return {
		"action": "setup",
		"launch_ready": false,
		"target": SETUP_SCENE_PATH,
		"reason": reason,
		"installed": installed,
		"pack_root": str(installed.get("pack_root", "")),
		"manifest_sha256": str(installed.get("manifest_sha256", "")),
	}

func route(host_node: Node = null, playable_scene_path: String = "") -> Dictionary:
	var decision := check_boot_state()
	var target_scene := str(decision["target"])
	if decision["action"] == "playable" and not playable_scene_path.is_empty():
		target_scene = playable_scene_path
		decision["target"] = target_scene
	
	if host_node != null and host_node.is_inside_tree():
		var launch_error: String = Launch.change_scene(host_node, target_scene, decision.get("launch_profile", {}))
		if not launch_error.is_empty():
			decision["action"] = "setup"
			decision["launch_ready"] = false
			decision["target"] = SETUP_SCENE_PATH
			decision["reason"] = launch_error
			Launch.show_repair(host_node, launch_error)
	
	return decision

func _determine_playable_target(explicit_path: String) -> String:
	if not explicit_path.is_empty():
		return explicit_path
	var env_scene := OS.get_environment(ENV_PLAYABLE_SCENE)
	if not env_scene.is_empty():
		return env_scene
	return DEFAULT_PLAYABLE_SCENE_PATH
