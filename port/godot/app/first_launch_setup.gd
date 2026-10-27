extends Control
## Additive import screen; the launcher supplies its packaged backend and capability needs.
## Defaults to OriginalGameImportBackend when running standalone.

const FlowScript = preload("res://app/first_launch_import_flow.gd")
const BackendScript = preload("res://services/original_game_import_backend.gd")
const Router = preload("res://app/boot_router.gd")
const Launch = preload("res://app/installed_launch.gd")
const DEFAULT_CAPABILITIES: Array[String] = Router.DEFAULT_CAPABILITIES

signal gameplay_ready(pack_root: String, manifest_sha256: String)

@export var required_launch_capabilities: Array[String] = []

var importer_backend: Object
var _flow: RefCounted
var _status: Label
var _progress: ProgressBar
var _source_label: Label
var _start: Button
var _cancel: Button
var _file_dialog: FileDialog
var _folder_dialog: FileDialog
var _location_dialog: FileDialog
var _selected_source := ""
var _choose_file_button: Button
var _choose_folder_button: Button
var _choose_location_button: Button
var _play: Button

func _ready() -> void:
	_build_ui()
	if importer_backend == null:
		importer_backend = BackendScript.new()
	if required_launch_capabilities.is_empty():
		required_launch_capabilities = DEFAULT_CAPABILITIES.duplicate()
	if required_launch_capabilities == DEFAULT_CAPABILITIES:
		gameplay_ready.connect(_continue_installed.call_deferred)
	_flow = FlowScript.new(null, importer_backend)
	var installed: Dictionary = _flow.check_existing(required_launch_capabilities)
	_show_result(installed)
	if get_tree().has_meta("installed_launch_failure"):
		_status.text = str(get_tree().get_meta("installed_launch_failure"))
		get_tree().remove_meta("installed_launch_failure")
	set_process(false)

func _continue_installed(_pack_root: String, _manifest_sha: String) -> void:
	# Revalidate the remembered receipt before entering the interactive profile.
	var checked: Dictionary = _flow.store.check_installed(DEFAULT_CAPABILITIES)
	if checked.get("state") != "ready":
		_status.text = "Game data could not be verified. Import your original game again."
		return
	var launch_error: String = Launch.change_scene(self, Launch.ENTRY_SCENE, checked.get("launch_profile", {}))
	if not launch_error.is_empty():
		_status.text = launch_error

func configure_importer(backend: Object, capabilities: Array[String]) -> void:
	importer_backend = backend
	required_launch_capabilities = capabilities.duplicate()
	if not is_inside_tree():
		return
	_flow = FlowScript.new(null, importer_backend)
	_show_result(_flow.check_existing(required_launch_capabilities))

func _build_ui() -> void:
	var margin := MarginContainer.new()
	margin.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	margin.add_theme_constant_override("margin_left", 28)
	margin.add_theme_constant_override("margin_right", 28)
	margin.add_theme_constant_override("margin_top", 24)
	margin.add_theme_constant_override("margin_bottom", 24)
	add_child(margin)
	var column := VBoxContainer.new()
	column.add_theme_constant_override("separation", 14)
	margin.add_child(column)
	var title := Label.new()
	title.text = "Install original game data"
	title.add_theme_font_size_override("font_size", 24)
	column.add_child(title)
	_status = Label.new()
	_status.text = "Checking installed content."
	_status.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	column.add_child(_status)
	_source_label = Label.new()
	_source_label.text = "No source selected."
	_source_label.text_overrun_behavior = TextServer.OVERRUN_TRIM_ELLIPSIS
	column.add_child(_source_label)
	_progress = ProgressBar.new()
	_progress.min_value = 0.0
	_progress.max_value = 1.0
	_progress.value = 0.0
	_progress.show_percentage = true
	column.add_child(_progress)
	var buttons := HFlowContainer.new()
	buttons.add_theme_constant_override("h_separation", 10)
	buttons.add_theme_constant_override("v_separation", 10)
	column.add_child(buttons)
	_choose_file_button = Button.new()
	_choose_file_button.text = "Choose disc image or archive…"
	_choose_file_button.pressed.connect(_choose_file)
	buttons.add_child(_choose_file_button)
	_choose_folder_button = Button.new()
	_choose_folder_button.text = "Choose extracted game folder…"
	_choose_folder_button.pressed.connect(_choose_folder)
	buttons.add_child(_choose_folder_button)
	_choose_location_button = Button.new()
	_choose_location_button.text = "Choose content location…"
	_choose_location_button.pressed.connect(_choose_location)
	buttons.add_child(_choose_location_button)
	_start = Button.new()
	_start.text = "Verify and import"
	_start.disabled = true
	_start.pressed.connect(_start_import)
	buttons.add_child(_start)
	_cancel = Button.new()
	_cancel.text = "Cancel import"
	_cancel.disabled = true
	_cancel.pressed.connect(_cancel_import)
	buttons.add_child(_cancel)
	_play = Button.new()
	_play.text = "Open name entry"
	_play.disabled = true
	_play.pressed.connect(_continue_installed.bind("", ""))
	buttons.add_child(_play)
	_file_dialog = FileDialog.new()
	_file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
	_file_dialog.access = FileDialog.ACCESS_FILESYSTEM
	_file_dialog.title = "Choose an original PS1 image or supported archive"
	_file_dialog.file_selected.connect(_source_selected)
	add_child(_file_dialog)
	_folder_dialog = FileDialog.new()
	_folder_dialog.file_mode = FileDialog.FILE_MODE_OPEN_DIR
	_folder_dialog.access = FileDialog.ACCESS_FILESYSTEM
	_folder_dialog.title = "Choose an extracted original PS1 game folder"
	_folder_dialog.dir_selected.connect(_source_selected)
	add_child(_folder_dialog)
	_location_dialog = FileDialog.new()
	_location_dialog.file_mode = FileDialog.FILE_MODE_OPEN_DIR
	_location_dialog.access = FileDialog.ACCESS_FILESYSTEM
	_location_dialog.title = "Choose a persistent content directory"
	_location_dialog.dir_selected.connect(_location_selected)
	add_child(_location_dialog)

func _choose_file() -> void:
	_file_dialog.popup_centered_ratio(0.75)

func _choose_folder() -> void:
	_folder_dialog.popup_centered_ratio(0.75)

func _choose_location() -> void:
	_location_dialog.popup_centered_ratio(0.75)

func _location_selected(path: String) -> void:
	var result: Dictionary = _flow.set_storage_location(path)
	_show_result(result)

func _source_selected(path: String) -> void:
	_selected_source = path
	_source_label.text = path.get_file() if FileAccess.file_exists(path) else path
	_start.disabled = false
	_status.text = "Selected source is not modified. Press Verify and import to check its release and required data."

func _start_import() -> void:
	var message: String = _flow.choose_source(_source_label_path(), required_launch_capabilities)
	if not message.is_empty():
		_status.text = _friendly_message()
		return
	set_process(true)
	_update_controls()

func _source_label_path() -> String:
	return _selected_source

func _cancel_import() -> void:
	var message: String = _flow.cancel()
	if not message.is_empty():
		_status.text = _friendly_message()
	_update_controls()

func _process(_delta: float) -> void:
	_flow.tick()
	_progress.value = _flow.progress
	_status.text = _friendly_message()
	_update_controls()
	if _flow.state in ["ready", "installed_incomplete", "cancelled", "failed", "importer_unavailable"]:
		set_process(false)
		if _flow.state == "ready":
			var pack_root := str(_flow.latest_result.get("pack_root", ""))
			var manifest_sha := str(_flow.latest_result.get("manifest_sha256", ""))
			gameplay_ready.emit(pack_root, manifest_sha)

func _update_controls() -> void:
	var active: bool = _flow != null and _flow.state in ["verifying", "importing", "cancelling", "validating", "publishing"]
	_start.disabled = active or _selected_source.is_empty()
	_cancel.disabled = _flow == null or _flow.state not in ["verifying", "importing"]
	_choose_file_button.disabled = active
	_choose_folder_button.disabled = active
	_choose_location_button.disabled = active
	_play.disabled = active or _flow == null or _flow.state != "ready"

func _show_result(result: Dictionary) -> void:
	_status.text = _friendly_state(str(result.get("state", "failed")))
	_progress.value = 1.0 if result.get("state") == "ready" else 0.0
	_update_controls()

func _friendly_message() -> String:
	if _flow.state == "failed" and _flow.error.begins_with("selected source overlaps the content location"):
		return "Choose game files from another folder, or choose another place to keep game data."
	return _friendly_state(_flow.state)

func _friendly_state(flow_state: String) -> String:
	match flow_state:
		"checking":
			return "Checking saved game data…"
		"needs_source":
			return "Choose your original game files to set up the game."
		"verifying":
			return "Checking the selected game data…"
		"importing":
			return "Importing game data…"
		"cancelling":
			return "Cancelling. Your existing install will be kept."
		"validating", "publishing":
			return "Checking imported data before saving…"
		"ready":
			return "Original name-entry data is ready. The playable opening is not available in this build yet."
		"installed_incomplete":
			return "More game data is needed before this build can start. Choose your original game files again."
		"cancelled":
			return "Import cancelled. Your existing install is unchanged."
		"importer_unavailable":
			return "Game setup isn't available in this build yet. Your original files are untouched."
		_:
			return "We couldn't verify or save that game data. Your existing install is unchanged."
