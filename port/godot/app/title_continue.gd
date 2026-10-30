extends Control
## T-39007 (reviews R10, R12): installed title owner shown at normal launch
## when save files exist. It offers Continue for a valid save, explicit backup
## recovery, setting aside interrupted or damaged files, and New Game.
##
## Source flow (wiki spec m-flow 3.8): the title leads to NAME_ENT, whose file
## screens start a new game or load a save; a loaded game enters the weekly
## cycle. The port keeps one session slot ("quick") and the system record; the
## source's 14 slots and its copy/delete screens are not modelled here.
## Nothing on this screen deletes a file.
##
## Limit: a restored session needs the captured development boundary and rule
## tables (playable_boot._load_inputs). No installed launcher provides them, so
## on an installed launch Continue is shown disabled with that reason.

const SaveService = preload("res://services/save_service.gd")
const SystemRecords = preload("res://services/system_record_service.gd")
const Controls = preload("res://app/boundary_save_controls.gd")
const Launch = preload("res://app/installed_launch.gd")
const SAVE_DIRECTORY: String = "boundary-saves"
const PLAYABLE_SCRIPT: String = "res://app/playable_boot.gd"
const SLOT_FILES: Array[String] = [".json", ".json.bak", ".json.tmp", ".json.bak.tmp"]

var installed_launch_profile: Dictionary = {}
var save_root: String = ""
var actions: Array[Dictionary] = []
var status_lines: Array[String] = []
var last_message: String = ""
var _list: VBoxContainer
var _status: Label
## T-30002 WP8: port-level Settings affordance in this modern layer only.
var settings_button: Button
var settings_hint: Label

static func default_root() -> String:
	return OS.get_user_data_dir().simplify_path().trim_suffix("/").path_join(SAVE_DIRECTORY)

## True when any session or system save file exists, valid or not, so the
## player is offered Continue or recovery instead of a silent New Game.
static func has_save_files(root: String) -> bool:
	for slot: String in [Controls.SLOT, SystemRecords.SLOT]:
		for suffix: String in SLOT_FILES:
			if FileAccess.file_exists(root.path_join(slot + suffix)):
				return true
	return false

func _ready() -> void:
	if save_root.is_empty():
		save_root = default_root()
	set_anchors_preset(Control.PRESET_FULL_RECT)
	var backdrop := ColorRect.new()
	backdrop.color = Color.BLACK
	backdrop.size = Vector2(640, 480)
	add_child(backdrop)
	_add_settings_affordance()
	# Starts below the settings hint that the ModernSettings autoload draws.
	var column := VBoxContainer.new()
	column.position = Vector2(48, 96)
	column.size = Vector2(544, 360)
	column.add_theme_constant_override("separation", 10)
	add_child(column)
	_status = Label.new()
	_status.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	_status.custom_minimum_size = Vector2(544, 0)
	column.add_child(_status)
	_list = VBoxContainer.new()
	_list.add_theme_constant_override("separation", 6)
	column.add_child(_list)
	refresh()

## Hint line and mouse button for the port Settings menu. They sit between the
## autoload's notice and the action column, never on a native canvas.
func _add_settings_affordance() -> void:
	var row := HBoxContainer.new()
	row.position = Vector2(48, 56)
	row.size = Vector2(544, 32)
	row.add_theme_constant_override("separation", 12)
	add_child(row)
	settings_hint = Label.new()
	settings_hint.text = "F10 / L3+R3: Settings"
	settings_hint.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	settings_hint.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
	row.add_child(settings_hint)
	settings_button = Button.new()
	settings_button.text = "Settings"
	settings_button.focus_mode = Control.FOCUS_ALL
	settings_button.pressed.connect(open_settings)
	row.add_child(settings_button)

func open_settings() -> void:
	var settings: Node = get_node_or_null("/root/ModernSettings")
	if settings != null and is_instance_valid(settings.menu) and not settings.menu.visible:
		settings.toggle_menu()

func refresh() -> void:
	actions = compute_actions()
	_status.text = "\n".join(status_lines + ([last_message] if not last_message.is_empty() else []))
	for child: Node in _list.get_children():
		_list.remove_child(child)
		child.queue_free()
	var focused: bool = false
	for action: Dictionary in actions:
		var button := Button.new()
		button.text = action["label"] if action["enabled"] else action["label"] + " (" + action["reason"] + ")"
		button.disabled = not action["enabled"]
		# Long reasons wrap inside the 544-pixel column instead of running off screen.
		button.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
		button.custom_minimum_size = Vector2(544, 0)
		button.focus_mode = Control.FOCUS_ALL
		button.pressed.connect(_on_pressed.bind(action["id"]))
		_list.add_child(button)
		if action["enabled"] and not focused:
			button.grab_focus.call_deferred()
			focused = true

func _on_pressed(id: String) -> void:
	var error: String = perform(id)
	if not error.is_empty():
		last_message = error
		refresh()

## Lists every action with whether it is available now and why not.
func compute_actions() -> Array[Dictionary]:
	var result: Array[Dictionary] = []
	status_lines.clear()
	var session_store: SaveService = SaveService.new(save_root)
	var session_report: Dictionary = session_store.inspect_slot(Controls.SLOT)
	var system_report: Dictionary = SystemRecords.new(save_root).storage.inspect_slot(SystemRecords.SLOT)
	if not session_report["error"].is_empty() or not system_report["error"].is_empty():
		status_lines.append("The save folder cannot be used: " + session_report["error"] + system_report["error"])
		result.append(_action("new_game", "New Game", ""))
		return result
	status_lines.append("Session save: " + _describe(session_report))
	status_lines.append("Clear records: " + _describe(system_report))
	var check: Dictionary = continue_check()
	result.append(_action("continue", "Continue", check["error"]))
	_recovery_actions(result, session_report, "session", "save")
	_recovery_actions(result, system_report, "system", "clear record")
	result.append(_action("new_game", "New Game", ""))
	return result

## Continue is offered only for a valid, current save this build can restore.
func continue_check() -> Dictionary:
	var store: SaveService = SaveService.new(save_root)
	var report: Dictionary = store.inspect_slot(Controls.SLOT)
	if not report["error"].is_empty():
		return {"error": report["error"]}
	if not report["staging"].is_empty():
		return {"error": "an interrupted save must be set aside first"}
	match report["primary"]["state"]:
		"missing":
			return {"error": "no save"}
		"invalid":
			return {"error": "the save is damaged"}
	var loaded: Dictionary = store.load_slot(Controls.SLOT)
	if not loaded["error"].is_empty():
		return {"error": "the save is damaged"}
	# Same input preflight the playable app runs for a restored session
	# (playable_boot._load_inputs), so Continue is never offered and then
	# refused. The reviewed identities are read from the playable script.
	var playable: Variant = ResourceLoader.load(PLAYABLE_SCRIPT)
	if not playable is Script:
		return {"error": "the game screen could not be loaded"}
	var constants: Dictionary = (playable as Script).get_script_constant_map()
	var content_error: String = Controls.continue_content_error(str(constants.get("BOUNDARY_SHA", "")), str(constants.get("TABLES_SHA", "")))
	if not content_error.is_empty():
		return {"error": content_error}
	var versions: Dictionary = Controls.profile_versions()
	if not versions["error"].is_empty():
		return {"error": "game data for this save is not available"}
	for key: String in versions["versions"]:
		if loaded["snapshot"][key] != versions["versions"][key]:
			return {"error": "the save belongs to other game data or another version"}
	return {"error": "", "checksum_sha256": loaded["checksum_sha256"]}

func perform(id: String) -> String:
	var available: Array = actions.filter(func(action: Dictionary) -> bool: return action["id"] == id)
	if available.is_empty() or not available[0]["enabled"]:
		return "That option is not available now."
	var session_store: SaveService = SaveService.new(save_root)
	var system_store: SaveService = SystemRecords.new(save_root).storage
	var outcome: Dictionary = {}
	match id:
		"continue":
			var check: Dictionary = continue_check()
			if not check["error"].is_empty():
				return "Continue is not available: " + check["error"]
			return Launch.change_scene(self, Launch.PLAYABLE_SCENE, installed_launch_profile, null, null,
				{"slot": Controls.SLOT, "checksum_sha256": check["checksum_sha256"]})
		"new_game":
			return Launch.change_scene(self, Launch.ENTRY_SCENE, installed_launch_profile)
		"session_recover":
			outcome = session_store.recover_from_backup(Controls.SLOT)
		"session_staging":
			outcome = session_store.set_aside_staging(Controls.SLOT)
		"session_invalid":
			outcome = session_store.set_aside_invalid(Controls.SLOT)
		"system_recover":
			outcome = system_store.recover_from_backup(SystemRecords.SLOT)
		"system_staging":
			outcome = system_store.set_aside_staging(SystemRecords.SLOT)
		"system_invalid":
			outcome = system_store.set_aside_invalid(SystemRecords.SLOT)
		_:
			return "That option is not available now."
	last_message = ("Done. " + ", ".join(outcome.get("moved", []))).strip_edges() if outcome["error"].is_empty() else "Not changed: " + outcome["error"]
	refresh()
	return "" if outcome["error"].is_empty() else outcome["error"]

func _recovery_actions(result: Array[Dictionary], report: Dictionary, prefix: String, noun: String) -> void:
	if not report["staging"].is_empty():
		result.append(_action(prefix + "_staging", "Set aside interrupted " + noun + " files", ""))
	var primary: String = report["primary"]["state"]
	var backup_valid: bool = report["backup"]["state"] == "valid"
	if primary != "valid" and backup_valid:
		result.append(_action(prefix + "_recover", "Restore " + noun + " from backup",
			"" if report["staging"].is_empty() else "set aside interrupted files first"))
	if primary == "invalid" and not backup_valid:
		result.append(_action(prefix + "_invalid", "Set aside damaged " + noun, ""))

func _describe(report: Dictionary) -> String:
	var text: String = report["primary"]["state"]
	if report["primary"]["state"] == "invalid":
		text += " (" + report["primary"]["error"] + ")"
	text += "; backup " + report["backup"]["state"]
	if not report["staging"].is_empty():
		text += "; interrupted files: " + ", ".join(report["staging"])
	return text

static func _action(id: String, label: String, reason: String) -> Dictionary:
	return {"id": id, "label": label, "enabled": reason.is_empty(), "reason": reason}
