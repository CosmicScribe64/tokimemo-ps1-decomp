extends RefCounted
## T-29006: desktop quick-save requests, serviced between committed logic steps.
## Input handlers enqueue only. The app's simulation callbacks never call storage.

const SaveService = preload("res://services/save_service.gd")
const SaveAdapter = preload("res://services/playable_save_adapter.gd")
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/school_day_state.gd")
const InputFrame = preload("res://core/input_frame.gd")
const SOURCE_PROFILE: String = "687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3"
const BEHAVIOR_PROFILE: String = "rest-week-7a7b095d-f5f9-v2"
const SLOT: String = "quick"
enum Request { NONE, SAVE, LOAD }
var pending: Request = Request.NONE
var storage: SaveService
var adapter: SaveAdapter
var notice: Label
var notice_timer: Timer
var transient_notice: bool = false
var last_error: String = ""
var last_action: String = ""
var last_checkpoint: String = ""
var completed_requests: int = 0
var storage_calls: int = 0

func configure(app: Node, user_root: String) -> String:
	if storage != null:
		return "save controls are already configured"
	var trusted_root: String = OS.get_user_data_dir().simplify_path().trim_suffix("/")
	if not user_root.is_absolute_path() or user_root != user_root.simplify_path() or not user_root.begins_with(trusted_root + "/"):
		return "save directory must be inside the application user-data directory"
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK").path_join("manifest.json")
	var media_sha: String = FileAccess.get_sha256(media_path)
	if media_sha.length() != 64 or OS.get_environment("TOKIMEMO_BOUNDARY_SHA").length() != 64 or OS.get_environment("TOKIMEMO_TABLES_SHA").length() != 64:
		return "save controls require the preflighted content identity"
	storage = SaveService.new(user_root)
	adapter = SaveAdapter.new({"source_version": SOURCE_PROFILE,
		"content_version": JSON.stringify([OS.get_environment("TOKIMEMO_BOUNDARY_SHA"),
			OS.get_environment("TOKIMEMO_TABLES_SHA"), media_sha]).sha256_text(),
		"behavior_version": BEHAVIOR_PROFILE})
	notice = Label.new()
	notice.position = Vector2(16, 106)
	notice.size = Vector2(608, 36)
	notice.z_index = 110
	notice.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	notice.add_theme_font_size_override("font_size", 12)
	notice.text = ""
	notice.hide()
	app.add_child(notice)
	notice_timer = Timer.new()
	notice_timer.one_shot = true
	notice_timer.wait_time = 1.8
	notice_timer.process_mode = Node.PROCESS_MODE_ALWAYS
	notice_timer.timeout.connect(_expire_notice)
	app.add_child(notice_timer)
	return ""

func handle_input(app: Node, event: InputEvent) -> bool:
	if not event is InputEventKey or not event.pressed or event.echo or event.physical_keycode not in [KEY_F5, KEY_F9]:
		return false
	request(app, Request.SAVE if event.physical_keycode == KEY_F5 else Request.LOAD)
	return true

## Keyboard and pause-menu actions share the same admission and boundary guards.
func request(app: Node, action: Request) -> bool:
	if action not in [Request.SAVE, Request.LOAD]: return false
	if storage == null:
		_error("Save controls are unavailable.")
		return false
	if not _idle_and_focused(app):
		_error("Return focus and release the controls before saving or loading.")
		return false
	if pending != Request.NONE:
		_error("A save or load request is already queued.")
		return false
	pending = action
	last_error = ""
	_message("Save queued." if pending == Request.SAVE else "Load queued.", true)
	return true

## Returns true when a queued user action consumes this host frame. Returning
## before the next session step prevents an extra native tick after a load.
func process_queued(app: Node) -> bool:
	if pending == Request.NONE:
		return false
	var action: Request = pending
	pending = Request.NONE
	last_action = "save" if action == Request.SAVE else "load"
	last_checkpoint = ""
	if not _idle_and_focused(app):
		_error("Save/load cancelled because focus or the current step changed.")
		return true
	if not _neutral_controls(app, action):
		_error("Release the game controls before saving or loading.")
		return true
	if action == Request.SAVE:
		last_checkpoint = _checkpoint(app)
		if last_checkpoint.is_empty():
			_error("Save unavailable here. Save at command selection or the first result.")
			return true
		var captured: Dictionary = adapter.capture(app, last_checkpoint)
		if not captured["error"].is_empty():
			_error("Save unavailable: " + captured["error"])
			return true
		storage_calls += 1
		var saved: Dictionary = storage.save_slot(SLOT, captured["snapshot"])
		if not saved["error"].is_empty():
			_error("Save failed: " + saved["error"])
			return true
		completed_requests += 1
		last_error = ""
		_message("Saved command selection." if last_checkpoint in ["command", "holiday_command", "next_command"] else "Saved the first result.", true)
	else:
		storage_calls += 1
		var loaded: Dictionary = storage.load_slot(SLOT)
		if not loaded["error"].is_empty():
			_error("Load failed: " + loaded["error"])
			return true
		var restore_callable: Callable = Callable()
		if app.has_method("_present_committed"):
			restore_callable = Callable(app, "_present_committed")
		var error: String = adapter.restore(app, loaded["snapshot"], restore_callable)
		if not error.is_empty():
			_error("Load failed: " + error)
			return true
		last_checkpoint = loaded["snapshot"]["gameplay_state"]["checkpoint_id"]
		completed_requests += 1
		last_error = ""
		_message("Loaded command selection." if last_checkpoint in ["command", "holiday_command", "next_command"] else "Loaded the first result.", true)
	return true

func _checkpoint(app: Node) -> String:
	var state: State = app.get("state")
	var sunday_index: int = adapter._sunday_command_index(state) if adapter != null else -1
	if [state.game_mode, state.mode_stage, state.mode_step] == [0x34, 1, 0]:
		if state.rule_state.get("day_event_index") == 0 or (sunday_index >= 0 and state.rule_state.get("day_event_index") == sunday_index + 1 and app.get("holiday_controller") != null):
			return "command"
	if [state.game_mode, state.mode_stage, state.mode_step] == [0x46, 1, 0] and sunday_index >= 0 and state.rule_state.get("day_event_index") == sunday_index and app.get("holiday_controller") != null:
		return "command"
	if [state.game_mode, state.mode_stage, state.mode_step] == [0x31, 0, 0] and state.rule_state.get("day_event_index") == 1:
		return "first_result"
	return ""

func _idle_and_focused(app: Node) -> bool:
	var session: Session = app.get("session")
	return session != null and not session.rng.get("_failure_receiver").is_valid() \
		and not app.get("focus_paused") and not app.get("playback_paused") \
		and app.get("_window_focused") and app.get("_application_focused") \
		and not app.get("_resume_input_pending") and not app.get("_replay_requested") \
		and not app.get("video_capture_busy")

func _neutral_controls(app: Node, action: Request) -> bool:
	for value: int in app.get("pad").get("_devices").values():
		if value != 0:
			return false
	# A rejected native step retains its old latch while the app is stopped.
	# Physical release is mandatory; stale committed edges must not deadlock
	# recovery. Only a successful validated LOAD replaces that old input state.
	if action == Request.LOAD and app.get("stopped"):
		return true
	var latch: InputFrame = app.get("session").latched_input
	if latch.held != 0 or latch.pressed != 0 or latch.released != 0 or app.get("pad").get("_previous") != 0:
		return false
	return true

func cancel_pending(reason: String) -> void:
	if pending != Request.NONE:
		pending = Request.NONE
		_error(reason)

func _message(text: String, transient: bool = false) -> void:
	if is_instance_valid(notice):
		transient_notice = transient
		if text.is_empty():
			notice.text = ""
			notice.hide()
			if is_instance_valid(notice_timer):
				notice_timer.stop()
		else:
			notice.text = text
			notice.show()
			if is_instance_valid(notice_timer):
				notice_timer.stop()
				if transient and notice_timer.is_inside_tree():
					notice_timer.start()

func _expire_notice() -> void:
	if transient_notice and is_instance_valid(notice):
		transient_notice = false
		notice.text = ""
		notice.hide()

func _error(text: String) -> void:
	last_error = text
	_message(text)
