extends Node2D
## T-29008: captured-boundary development app, not the complete opening.
## External packs are preloaded before rules run. Completions are delivered at
## next BEFORE_MODE; this explicit development policy is not native CD latency.

const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const CoreState = preload("res://core/game_state.gd")
const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Rng = preload("res://core/native_rng.gd")
const Pad = preload("res://ui/native_pad.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const MediaStore = preload("res://services/school_day_media_store.gd")
const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const LiveHud = preload("res://presentation/live_hud_adapter.gd")
const HolidayController = preload("res://scenes/holiday_controller.gd")
const HolidaySceneAdapter = preload("res://services/holiday_scene_adapter.gd")
const HolidayHud = preload("res://presentation/holiday_hud_adapter.gd")
const BoundarySaveControls = preload("res://app/boundary_save_controls.gd")
var hud: LiveHud
var holiday_controller: HolidayController
var holiday_scene: HolidaySceneAdapter
var holiday_hud: HolidayHud
const BOUNDARY_SHA: String = "cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5"
const TABLES_SHA: String = "a1f30e57ef4f40ee7f9786043ccc5e007cfe316ba42fbd2c3e6896bf1d5f7060"

var session: Session
var controller: Controller
var state: State
var pad: Pad
var canvas: Canvas
var media: MediaStore
var adapter: MediaAdapter = MediaAdapter.new()
var actor_service: ActorService = ActorService.new()
var resources: Dictionary = {}
var background: Array[Dictionary] = []
var command_resources: Dictionary = {}
var command_backgrounds: Dictionary = {}
const MEDIA_ROUTES: Dictionary = MediaAdapter.MEDIA_ROUTES
var staged_commands: Array[Dictionary] = []
var staged_services: Array[Dictionary] = []
var pending_services: Array[Dictionary] = []
var boundary: Dictionary = {}
var trace: Array[Dictionary] = []
var status_label: Label
var detail_label: Label
var stopped: bool = false
var failure: String = ""
var smoke: bool = false
var capture_pending: String = ""
var output_root: String = ""
var captured_actor: bool = false
var captured_effect: bool = false
var initial_params: Array = []
var video_frame: int = 0
var video_capture_busy: bool = false
var smoke_command: int = 0
var save_controls: BoundarySaveControls = BoundarySaveControls.new()
var continue_week: bool = true
var checkpoints: Array[Dictionary] = []
var staged_hud_refresh: bool = false
var supported_stop: Dictionary = {}
const HOLIDAY_STOP: String = "ordinary-week next branch requires native type0x10 at index9 (95-04-09)"

func _capture_prefix() -> String:
	if holiday_controller != null:
		return "T-29013-sunday"
	var cmd: int = smoke_command
	if cmd == 0 and state != null and state.rule_state.has("last_command"):
		cmd = state.rule_state["last_command"] & 15
	if cmd == 7:
		return "T-29009-rest-week" if continue_week else "T-29008-rest"
	if cmd == 0:
		return "T-29009-week" if continue_week else "T-29008-playable"
	return ("T-29014-command%d-week" if continue_week else "T-29014-command%d") % cmd

func _week_continuation_enabled() -> bool:
	return continue_week

# Desktop adaptation: future playback providers must pause/resume their cursors
# with this signal. There is no active audio player in this muted development app.
signal playback_pause_changed(paused: bool)
var focus_paused: bool = false
var playback_paused: bool = false
var _window_focused: bool = true
var _application_focused: bool = true
var _resume_input_pending: bool = false
var _replay_requested: bool = false

func _notification(what: int) -> void:
	match what:
		NOTIFICATION_WM_WINDOW_FOCUS_OUT: _window_focused = false
		NOTIFICATION_WM_WINDOW_FOCUS_IN: _window_focused = true
		NOTIFICATION_APPLICATION_FOCUS_OUT: _application_focused = false
		NOTIFICATION_APPLICATION_FOCUS_IN: _application_focused = true
		_: return
	var paused_now: bool = not (_window_focused and _application_focused)
	if paused_now == focus_paused:
		return
	focus_paused = paused_now
	_resume_input_pending = true
	_replay_requested = false
	if focus_paused and save_controls != null:
		save_controls.cancel_pending("Save/load cancelled on focus loss.")
	if pad != null:
		pad.clear_devices()
		# Rearm only at the first focused logical boundary, after clearing the
		# session latch. Focus callbacks never mutate session/rules/clock/RNG.
		pad.set_process_input(false)
	if focus_paused:
		_set_playback_paused(true)

func _set_playback_paused(paused_now: bool) -> void:
	if paused_now != playback_paused:
		playback_paused = paused_now
		playback_pause_changed.emit(paused_now)

func _ready() -> void:
	output_root = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	smoke = OS.get_environment("TOKIMEMO_PLAYABLE_SMOKE") == "1"
	smoke_command = int(OS.get_environment("TOKIMEMO_PLAYABLE_SMOKE_COMMAND"))
	continue_week = OS.get_environment("TOKIMEMO_PLAYABLE_FIRST_RESULT_ONLY") != "1"
	RenderingServer.set_default_clear_color(Color.BLACK)
	var backdrop := ColorRect.new()
	backdrop.size = Vector2(640, 480)
	backdrop.color = Color.BLACK
	backdrop.z_index = -100
	add_child(backdrop)
	canvas = Canvas.new()
	canvas.scale = Vector2(2, 2)
	canvas.z_index = -10
	add_child(canvas)
	pad = Pad.new()
	add_child(pad)
	pad.set_process_input(not focus_paused and not _resume_input_pending)
	# Native imagery occupies the full logical viewport. Keep diagnostic labels
	# available to test/save hooks, without covering original pixels.
	status_label = _label(Vector2(16, 12), 16)
	detail_label = _label(Vector2(16, 334), 12)
	status_label.hide()
	detail_label.hide()
	failure = _load_inputs()
	if not failure.is_empty():
		_stop(failure)
		return
	if smoke_command not in [0, 1, 2, 3, 5, 6, 7]:
		_stop("only source commands 0-3, 5-7 scripted input recipes are available")
		return
	if smoke and (not output_root.is_absolute_path() or not DirAccess.dir_exists_absolute(output_root)):
		_stop("smoke requires an existing absolute output directory")
		return
	failure = _reset()
	if not failure.is_empty():
		_stop(failure)
		return
	failure = save_controls.configure(self, OS.get_user_data_dir().path_join("boundary-saves"))
	if not failure.is_empty():
		_stop(failure)
		return
	set_physics_process(false)
	for warmup: int in range(2):
		await get_tree().process_frame
		await RenderingServer.frame_post_draw
	await _capture("selection")
	set_physics_process(true)

func _label(position_value: Vector2, size: int) -> Label:
	var label := Label.new()
	label.position = position_value
	label.z_index = 100
	label.add_theme_font_size_override("font_size", size)
	add_child(label)
	return label

func _load_inputs() -> String:
	holiday_controller = null
	holiday_scene = null
	holiday_hud = null
	var state_path: String = OS.get_environment("TOKIMEMO_BOUNDARY")
	var table_path: String = OS.get_environment("TOKIMEMO_TABLES")
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK")
	var spring_pack: String = OS.get_environment("T29014_SPRING_PACK")
	if spring_pack.is_empty():
		spring_pack = OS.get_environment("TOKIMEMO_SPRING_PACK")
	var weekday56_pack: String = OS.get_environment("T29014_WEEKDAY56_PACK")
	if weekday56_pack.is_empty():
		weekday56_pack = OS.get_environment("TOKIMEMO_WEEKDAY56_PACK")
	if OS.get_environment("TOKIMEMO_BOUNDARY_SHA") != BOUNDARY_SHA or OS.get_environment("TOKIMEMO_TABLES_SHA") != TABLES_SHA:
		return "unsupported development boundary/table profile; use the reviewed captured inputs"
	if not _verified_external(state_path, OS.get_environment("TOKIMEMO_BOUNDARY_SHA")):
		return "development boundary missing or SHA256 differs from explicit launch identity"
	if not _verified_external(table_path, OS.get_environment("TOKIMEMO_TABLES_SHA")):
		return "original rule tables missing or SHA256 differs from explicit launch identity"
	var parsed: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(state_path)))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.development-boundary.v1":
		return "unsupported development-boundary schema"
	if not parsed.get("core") is Dictionary or not parsed.get("rule_state") is Dictionary or not parsed.get("rng_state") is int:
		return "development boundary lacks core/rules/RNG records"
	boundary = parsed
	var table_pack: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(table_path)))
	if not table_pack is Dictionary or table_pack.get("schema") != "tokimemo.port-rule-tables.v1" or table_pack.get("schema_version") != 1 or not table_pack.get("tables") is Dictionary:
		return "unsupported original rule-table schema"
	if table_pack.get("provenance", {}).get("release_executable", {}).get("sha256") != MediaStore.EXE_SHA:
		return "rule tables are not the supported original executable profile"
	controller = Controller.new(table_pack["tables"])
	if not controller.error.is_empty():
		return controller.error
	var adapter_err: String = adapter.open_packs(media_path, spring_pack, weekday56_pack)
	if not adapter_err.is_empty():
		return adapter_err
	media = adapter.media
	command_resources = adapter.command_resources
	command_backgrounds = adapter.command_backgrounds
	controller.command_capabilities.assign(adapter.get_available_commands())
	resources = command_resources.get(0, {})
	background = command_backgrounds.get(0, [])
	var hud_manifest: String = OS.get_environment("TOKIMEMO_HUD_MANIFEST")
	var require_hud: bool = OS.get_environment("TOKIMEMO_REQUIRE_HUD") == "1" \
		or not hud_manifest.is_empty() \
		or not OS.get_environment("TOKIMEMO_HUD_SHA").is_empty() \
		or not OS.get_environment("TOKIMEMO_FONT_ROOT").is_empty() \
		or not OS.get_environment("TOKIMEMO_HUD_CONTEXT").is_empty() \
		or not OS.get_environment("TOKIMEMO_HUD_CONTEXT_SHA").is_empty()
	if require_hud:
		hud = LiveHud.new()
		if not hud.open(hud_manifest, OS.get_environment("TOKIMEMO_HUD_SHA"),
			OS.get_environment("TOKIMEMO_FONT_ROOT"), OS.get_environment("TOKIMEMO_HUD_CONTEXT"), OS.get_environment("TOKIMEMO_HUD_CONTEXT_SHA")):
			return "mandatory HUD preflight: " + hud.error
	else:
		hud = null
	var holiday_extension: String = OS.get_environment("TOKIMEMO_HOLIDAY_EXTENSION")
	var holiday_text: String = OS.get_environment("TOKIMEMO_HOLIDAY_TEXT")
	var holiday_text_sha: String = OS.get_environment("TOKIMEMO_HOLIDAY_TEXT_SHA")
	var require_holiday: bool = OS.get_environment("TOKIMEMO_REQUIRE_HOLIDAY") == "1" \
		or not holiday_extension.is_empty() \
		or not holiday_text.is_empty() \
		or not holiday_text_sha.is_empty()
	if require_holiday:
		if hud == null:
			return "mandatory holiday preflight: complete native HUD capability is required"
		var media_manifest_path: String = media_path.path_join("manifest.json")
		var media_manifest_sha: String = FileAccess.get_sha256(media_manifest_path) if ExternalPath.is_external_file(media_manifest_path) else ""
		var expected_text_sha: String = HolidayHud.TEXT_SHA if holiday_text_sha.is_empty() else holiday_text_sha
		var candidate_hud := HolidayHud.new()
		if not candidate_hud.open(hud_manifest, OS.get_environment("TOKIMEMO_HUD_SHA"),
			OS.get_environment("TOKIMEMO_FONT_ROOT"), media_path, media_manifest_sha,
			holiday_text, expected_text_sha):
			return "mandatory holiday HUD preflight: " + candidate_hud.error
		var profile_error: String = candidate_hud.validate_profile(_holiday_profile())
		if not profile_error.is_empty():
			return "mandatory holiday HUD profile preflight: " + profile_error
		var next_room: Dictionary = candidate_hud.get_background("next_command", HolidaySceneAdapter.ROUTE["next_command"])
		var candidate_scene := HolidaySceneAdapter.new()
		var scene_error: String = candidate_scene.open(media, holiday_extension, next_room.get("image_data"))
		if not scene_error.is_empty():
			return "mandatory holiday scene preflight: " + scene_error
		var candidate_controller := HolidayController.new(table_pack["tables"], candidate_scene.tables)
		if not candidate_controller.error.is_empty():
			return "mandatory holiday controller preflight: " + candidate_controller.error
		holiday_hud = candidate_hud
		holiday_scene = candidate_scene
		holiday_controller = candidate_controller
	return ""

func _verified_external(path: String, digest: String) -> bool:
	return ExternalPath.is_external_file(path) and digest.length() == 64 and FileAccess.get_size(path) < 8 * 1024 * 1024 and FileAccess.get_sha256(path) == digest

func _reset() -> String:
	state = State.new()
	var core_fields: Dictionary = CoreState.new().projection()
	if boundary["core"].size() != core_fields.size():
		return "development core record differs from supported field set"
	for key: String in core_fields:
		if not boundary["core"].has(key) or typeof(boundary["core"][key]) != typeof(core_fields[key]):
			return "development core field has wrong type: " + key
		state.set(key, boundary["core"][key])
	state.rule_state = boundary["rule_state"].duplicate(true)
	if [state.game_mode, state.mode_stage, state.mode_step] != [0x34, 1, 0] or [state.rule_state.get("year"), state.rule_state.get("month"), state.rule_state.get("day")] != [95, 4, 4]:
		return "only the genuine initial April4 command boundary is supported"
	# Content capabilities are immutable controller configuration; preserving the
	# legacy record also preserves all existing command0 state/event hashes.
	state.rule_state["resources"]["available_commands"] = [0]
	if holiday_controller != null:
		if holiday_scene == null or holiday_hud == null or hud == null:
			return "holiday controller requires preflighted holiday scene and HUD"
		state.rule_state["holiday"] = holiday_scene.initial_extension()
		state.rule_state["resources"]["available_holiday_commands"] = [0]
	session = Session.new()
	if not session.rng.configure(Rng.OPENBIOS_PROFILE, boundary["rng_state"]):
		return session.rng.configuration_error()
	var error: String = controller.attach(session, state)
	if not error.is_empty():
		return error
	if holiday_controller != null:
		error = holiday_controller.attach(session, state)
		if not error.is_empty():
			return error
	session.draw_logic = _native_draw
	session.cd_logic = _bounded_cd
	session.audio_logic = _bounded_audio
	pad.clear_devices()
	pending_services.clear()
	trace.clear()
	checkpoints.clear()
	supported_stop.clear()
	stopped = false
	failure = ""
	captured_actor = false
	captured_effect = false
	initial_params = state.rule_state["params"].duplicate()
	canvas.present([])
	if hud != null:
		hud.reset(state.rule_state)
		error = _present_committed()
		if not error.is_empty():
			return error
	_update_labels()
	status_label.hide()
	detail_label.hide()
	return ""

func _input(event: InputEvent) -> void:
	if save_controls.handle_input(self, event):
		return
	if not focus_paused and event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_F3:
		detail_label.visible = not detail_label.visible
	if not focus_paused and event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_R and stopped and not smoke:
		_replay_requested = true

func _physics_process(_delta: float) -> void:
	if focus_paused:
		return
	if _replay_requested:
		save_controls.cancel_pending("Save/load cancelled by replay.")
		_replay_requested = false
		var error: String = _reset()
		if not error.is_empty():
			_stop(error)
	var tick: int = session.clock.logic_iterations
	if _resume_input_pending:
		# One neutral resume boundary discards any stale held state/edge from
		# before focus loss or the focus-return event queue. Fresh later presses
		# follow the native one-iteration input latch, without a phantom edge.
		pad.clear_devices()
		session.latched_input = InputFrame.new(tick - 1)
		pad.set_process_input(true)
		_resume_input_pending = false
		# Playback resumes at the same logical boundary as simulation, never
		# from the focus-in signal before the stale session latch is cleared.
		_set_playback_paused(false)
	# Focus neutralization also runs while stopped; it never advances rules,
	# RNG or clocks. F9 may then recover through the ordinary focused boundary.
	if save_controls.process_queued(self):
		return
	if stopped:
		return
	if smoke:
		# Original command0 recipe: step next-input belongs to native vsyncN+1.
		var mask_key: int = KEY_ENTER
		var pressed_tick: int = 17 if smoke_command == 0 else 97
		var released_tick: int = 23 if smoke_command == 0 else 103
		if smoke_command > 0 and tick < 75:
			var step_index: int = tick / 10
			if step_index < smoke_command:
				mask_key = KEY_RIGHT
				pressed_tick = tick if tick % 10 == 7 else -1
				released_tick = tick if tick % 10 == 1 else -1
		elif holiday_controller != null and smoke_command == 0 and tick >= 246:
			pressed_tick = 278
			released_tick = 284
		if tick == pressed_tick or tick == released_tick:
			var event := InputEventKey.new()
			event.physical_keycode = mask_key
			event.pressed = tick == pressed_tick
			# Deliver the synthetic device event through the same mapping handler;
			# OS dispatch may run between several fixed steps in a slow capture.
			pad._input(event)
	var events: Array[Session.ServiceEvent] = []
	for descriptor: Dictionary in pending_services:
		events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, descriptor["service"], descriptor["payload"]))
	staged_commands.clear()
	staged_services.clear()
	staged_hud_refresh = false
	var before_hash: String = session.state_hash()
	var result: Session.FrameResult = session.step(pad.sample(tick), events)
	if not result.committed:
		if _week_continuation_enabled() and result.error == HOLIDAY_STOP and state.game_mode == 0x31 and state.rule_state["day_event_index"] == 8 and [state.rule_state["year"], state.rule_state["month"], state.rule_state["day"]] == [95, 4, 8] and session.state_hash() == before_hash:
			supported_stop = {"error": result.error, "before_hash": before_hash,
				"after_hash": session.state_hash(), "committed": false, "target": [95, 4, 9, 0x10, 9]}
			stopped = true
			status_label.position = Vector2(16, 454)
			status_label.text = "Sunday unavailable" if smoke else "Sunday unavailable · R: replay"
			status_label.show()
			_finish()
			return
		if not output_root.is_empty():
			var rejected := FileAccess.open(output_root.path_join("T-29008-rejected-step.json"), FileAccess.WRITE)
			if rejected != null:
				rejected.store_string(JSON.stringify({"error": result.error, "before_hash": before_hash,
					"after_hash": session.state_hash(), "state": state.projection()}))
				rejected.close()
		_stop(result.error)
		return
	pending_services = staged_services.duplicate(true)
	# Images and mutable renderer caches are resolved only after commit.
	trace.append({"iteration": tick, "state_hash": result.state_hash, "event_hash": result.event_hash,
		"mode": state.game_mode, "stage": state.mode_stage, "step": state.mode_step,
		"substep": state.substep, "frame_count": state.frame_count,
		"params": state.rule_state["params"].duplicate(), "rng": session.rng.state,
		"presentation": result.presentation, "audio": result.audio})
	var error: String = _present_committed()
	if not error.is_empty():
		_stop(error)
		return
	_update_labels()
	if staged_hud_refresh:
		status_label.text += " · week-end health check"
	_record_checkpoint(result)
	if not captured_actor and not state.rule_state["actor_state"].is_empty():
		captured_actor = true
		_capture("activity")
	if not captured_effect and state.rule_state["params"] != initial_params:
		captured_effect = true
		_capture("effect")
	if holiday_controller != null:
		if [state.game_mode, state.mode_stage, state.mode_step] == [0x46, 1, 0] and tick == 245:
			_capture("holiday-selection")
		elif [state.game_mode, state.mode_stage, state.mode_step] == [0x46, 5, 2] and tick == 284:
			_capture("holiday-activity")
	if not _week_continuation_enabled() and state.game_mode == 0x31 and state.rule_state["day_event_index"] == 1:
		stopped = true
		_finish()
	elif smoke and holiday_controller != null and [state.game_mode, state.mode_stage, state.mode_step] == [0x34, 1, 0] and state.rule_state.get("day_event_index") == 10:
		stopped = true
		_finish()
	elif smoke and tick >= 600:
		_stop("scripted input smoke exceeded its bounded supported route")

func _record_checkpoint(result: Session.FrameResult) -> void:
	if not _week_continuation_enabled():
		return
	var phase: String = ""
	if state.game_mode == 0x40 and state.mode_stage == 0 and state.mode_step == 0 and state.substep == 0:
		phase = "activity_entry"
	elif state.game_mode == 0x31 and state.rule_state["day_event_index"] in [1, 3, 5, 7]:
		phase = "activity_return"
	elif state.game_mode == 0x32:
		phase = "week_end_stage_%d" % state.mode_stage
	elif state.game_mode == 0x31 and state.rule_state["day_event_index"] == 8:
		phase = "week_end_return"
	for intent: Dictionary in result.presentation:
		if intent.get("kind") == "school_day_cached_background":
			phase = "cached_background"
	if not phase.is_empty():
		checkpoints.append({"phase": phase, "state": state.projection(), "rng": session.rng.checkpoint(),
			"clock": session.clock.snapshot(), "trace_count": trace.size()})
	if state.game_mode == 0x40 and state.mode_stage == 1 and state.mode_step_timer == 0:
		_capture("activity-%02d" % state.rule_state["day"])

func _process(_delta: float) -> void:
	if focus_paused or OS.get_environment("TOKIMEMO_PLAYABLE_RECORD") != "1" or output_root.is_empty() or video_capture_busy:
		return
	video_capture_busy = true
	await RenderingServer.frame_post_draw
	if focus_paused:
		video_capture_busy = false
		return
	var frame_root: String = output_root.path_join(".video-frames")
	if DirAccess.make_dir_recursive_absolute(frame_root) != OK:
		video_capture_busy = false
		_stop("actual video frame directory creation failed")
		return
	var path: String = frame_root.path_join("%s-frame-%04d.png" % [_capture_prefix(), video_frame])
	if get_viewport().get_texture().get_image().save_png(path) != OK:
		_stop("actual video frame capture failed")
	video_frame += 1
	video_capture_busy = false

func _native_draw(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
	var current: State = base as State
	if current.game_mode == 0x46 or (holiday_scene != null and holiday_scene.handles(current)):
		if holiday_scene == null:
			result.error = "holiday scene capability is not open"
			return
		var staged: Dictionary = holiday_scene.stage(current, session.clock.logic_iterations, result)
		if not staged["error"].is_empty():
			result.error = staged["error"]
			return
		staged_commands.append_array(staged["draw_commands"])
		staged_services.append_array(staged["services"])
		return
	var data: Dictionary = current.rule_state
	var prepared_commands: Array[Dictionary] = []
	var prepared_services: Array[Dictionary] = []
	var prepared_hud: bool = false
	for intent: Dictionary in result.presentation:
		if intent.get("kind") == "school_day_load":
			result.error = _resource_request_error(current, intent)
			if not result.error.is_empty():
				return
			prepared_services.append({"service": "school_day.resources", "payload": {
				"request": intent["request"], "ready": true,
				"lba": intent["background_lba"] if intent["request"] == "background" else intent["sd_lba"],
				"decode_result": 1, "checksum_ok": true}})
		elif intent.get("kind") == "school_day_actors":
			var command: int = _activity_command(current)
			if not command_resources.has(command) or intent.get("sd_set_id") != MEDIA_ROUTES[command]["sd_set_id"] or data["scene_work"] != intent["sd_set_id"]:
				result.error = "actor request differs from validated command media profile"
				return
			if command == 7 and (intent["actors"].size() != 1 or intent["actors"][0].get("id") != 96 or intent["actors"][0].get("sequence_slot") != 0):
				result.error = "rest supports only the captured actor96 sequence0"
				return
			if command in [1, 2, 3, 5, 6] and (intent["actors"].size() != 1 or intent["actors"][0].get("id") != 96):
				result.error = "opening week command %d supports only solo actor96" % command
				return
			var initialized: Dictionary = actor_service.initialize(intent["actors"], command_resources[command])
			if not initialized["error"].is_empty():
				result.error = initialized["error"]
				return
			current.rule_state["actor_state"] = initialized["actor_state"]
		elif intent.get("kind") == "school_day_cached_background":
			var slot: int = intent.get("slot", -1)
			var command: int = _activity_command(current)
			if not command_backgrounds.has(command) or slot not in [0, 1] or intent.get("lba") != MEDIA_ROUTES[command]["background_lba"] or data["background_show"] != slot or data["background_cache"][slot] != intent["lba"]:
				result.error = "cached background differs from validated command slot/LBA"
				return
		elif intent.get("kind") == "week_end_hud_init":
			if current.game_mode != 0x32 or current.rule_state["day_events"][current.rule_state["day_event_index"]][0] != 0xfc:
				result.error = "week-end HUD refresh arrived outside its source queue record"
				return
			# Rules already took the source parameter snapshot. The postcommit
			# presenter consumes this intent without changing rules or event hashes.
			prepared_hud = true
	# Select only the source GPU slot made active by a committed resource
	# completion/cache hit. Command frame indices overlap, so resources and
	# immutable texture IDs stay separate throughout preparation and drawing.
	if current.game_mode in [0x40, 0x31, 0x32]:
		var shown_lba: int = data["background_cache"][data["background_show"]]
		for command: int in command_backgrounds:
			if shown_lba == MEDIA_ROUTES[command]["background_lba"]:
				prepared_commands.append_array(command_backgrounds[command])
	if not data["actor_state"].is_empty():
		var command: int = _activity_command(current)
		if not command_resources.has(command):
			result.error = "active actor has no validated command resource set"
			return
		var drawn: Dictionary = actor_service.native_draw(data["actor_state"], command_resources[command],
			session.clock.logic_iterations, Vector2i(160, 120))
		if not drawn["error"].is_empty():
			result.error = drawn["error"]
			return
		data["actor_state"] = drawn["actor_state"]
		prepared_commands.append_array(drawn["draw_commands"])
		prepared_services.append_array(drawn["services"])
		for draw: Dictionary in drawn["draw_commands"]:
			result.presentation.append({"kind": "native_actor_draw", "id": draw["id"],
				"x": draw["position"].x, "y": draw["position"].y})
	# GameSession rolls rule/actor state back on failure. Publish service/render
	# staging only after this entire adapter succeeds; canvas.present is postcommit.
	staged_commands.append_array(prepared_commands)
	staged_services.append_array(prepared_services)
	staged_hud_refresh = prepared_hud

func _activity_command(current: State) -> int:
	if current.game_mode != 0x40:
		return -1
	var data: Dictionary = current.rule_state
	var event: Array = data["day_events"][data["day_event_index"]]
	if event[0] != 0 or event[1] not in [0, 1, 2, 3, 5, 6, 7]:
		return -1
	var health: int = (data["player_flags"] >> 8) & 15
	if event[1] == 7:
		var index: int = data["day_event_index"]
		if event != [0, 7, 0, 15] or index not in [0, 2, 4, 6] or [data["year"], data["month"], data["day"]] != [95, 4, 5 + (index >> 1)] or data["vacation"] != 0 or health != 0:
			return -1
	elif event[1] in [1, 2, 3, 5, 6]:
		var index: int = data["day_event_index"]
		if event[3] != 15 or index not in [0, 2, 4, 6] or [data["year"], data["month"], data["day"]] != [95, 4, 5 + (index >> 1)] or data["vacation"] != 0 or health != 0:
			return -1
	return event[1]

## Save adapter post_apply hook. It rebuilds only owned presentation caches.
func _present_committed(_restored_app: Node = null) -> String:
	if state.game_mode == 0x46 or (holiday_scene != null and holiday_scene.handles(state)):
		if holiday_scene == null or holiday_hud == null or hud == null:
			return "holiday HUD capability is not open"
		var active_lba: int = int(state.rule_state["background_cache"][state.rule_state["background_show"]])
		var composed: Dictionary = holiday_hud.compose(state.rule_state, state.projection(), _holiday_profile(), trace, staged_commands, active_lba)
		if not composed["error"].is_empty():
			return composed["error"]
		if composed["retain"]:
			if canvas.get("_commands").is_empty():
				return "holiday retain requires a prior committed frame"
			return ""
		var holiday_commands: Array[Dictionary] = []
		holiday_commands.assign(composed["commands"])
		canvas.set("_textures", {})
		return canvas.present(holiday_commands)
	if hud == null:
		return canvas.present(staged_commands) # Synthetic engine/focus test seam.
	var command: Dictionary = hud.compose(state.rule_state, state.projection(), trace, staged_commands)
	if command.is_empty():
		return hud.error
	# Dynamic frame IDs must not retain an unbounded texture per logical tick.
	canvas.set("_textures", {})
	return canvas.present([command])

func _holiday_profile() -> Dictionary:
	if hud == null:
		return {}
	var result: Dictionary = {}
	for key: String in ["family_name_hex", "icon_mode", "icon_count", "cursor_style"]:
		if hud.profile.has(key):
			result[key] = hud.profile[key]
	return result

func _resource_request_error(current: State, intent: Dictionary) -> String:
	var command: int = _activity_command(current)
	if not command_resources.has(command) or not command_backgrounds.has(command):
		return "resource request has no preflighted command media profile"
	var expected: Dictionary = MEDIA_ROUTES[command]
	if intent.size() != 8 or intent.get("request") not in ["background", "sd"] or intent.get("command") != command or intent.get("day_event_index") != current.rule_state["day_event_index"] or intent.get("sectors") != 41:
		return "resource request identity differs from validated command media profile"
	for key: String in ["background_lba", "sd_set_id", "sd_lba"]:
		if intent.get(key) != expected[key]:
			return "resource request differs from validated command media field: " + key
	if current.mode_stage != 0 or current.substep != 1 or current.mode_step != (0 if intent["request"] == "background" else 1):
		return "resource request arrived outside its native setup phase"
	return ""

func _bounded_cd(_state: CoreState, _rng: Rng, _result: Session.FrameResult) -> void:
	# Validated immutable resources were read before the first simulation step.
	pass

func _bounded_audio(_state: CoreState, _rng: Rng, _result: Session.FrameResult) -> void:
	# This slice records native audio intents; it has no audio-dependent wait.
	pass

func _update_labels() -> void:
	var data: Dictionary = state.rule_state
	status_label.text = "Command %d · mode 0x%02x · %d/%d/%d" % [data["menu_cursor"], state.game_mode, 1900 + data["year"], data["month"], data["day"]]
	detail_label.text = "Mode 0x%02x · iteration %d · queue %d\nParams %s\nRNG %d · %d draws" % [state.game_mode, session.clock.logic_iterations,
		data["day_event_index"], str(data["params"]), session.rng.state, session.rng.draw_count]

func _finish() -> void:
	await _capture("result")
	if not output_root.is_empty():
		var file := FileAccess.open(output_root.path_join(_capture_prefix() + "-proof.json"), FileAccess.WRITE)
		if file == null:
			_stop("cannot write development proof")
			return
		file.store_string(JSON.stringify({"schema": "tokimemo.ordinary-week-development-proof.v1" if _week_continuation_enabled() else "tokimemo.playable-development-proof.v1",
			"scope": "command%d bounded composed development app; no native loading/CD/interrupt timing or complete HUD/audio parity claim" % state.rule_state["last_command"],
			"resource_policy": "validated preloaded cache; completion next BEFORE_MODE",
			"boundary_sha256": OS.get_environment("TOKIMEMO_BOUNDARY_SHA"),
			"tables_sha256": OS.get_environment("TOKIMEMO_TABLES_SHA"),
			"final_state": state.projection(), "rng": session.rng.checkpoint(), "trace": trace,
			"checkpoints": checkpoints, "supported_stop": supported_stop}, "\t"))
		file.close()
	print("PASS: scripted playable command%d reached its declared supported boundary with session-owned actor sequences" % state.rule_state["last_command"])
	if smoke:
		get_tree().quit(0)

func _capture(name_value: String) -> void:
	if output_root.is_empty():
		return
	for capture_frame: int in range(2):
		await get_tree().process_frame
		await RenderingServer.frame_post_draw
	if get_viewport().get_texture().get_image().save_png(output_root.path_join("%s-%s.png" % [_capture_prefix(), name_value])) != OK:
		_stop("actual viewport capture failed")

func _stop(message: String) -> void:
	stopped = true
	failure = message
	if status_label != null:
		status_label.position = Vector2(16, 454)
		status_label.text = "Stopped · see log" if smoke else "Stopped · R: replay · see log"
		status_label.show()
	push_error(message)
	if smoke:
		if output_root.is_absolute_path() and DirAccess.dir_exists_absolute(output_root):
			var report := FileAccess.open(output_root.path_join("T-29008-stop.json"), FileAccess.WRITE)
			if report != null:
				report.store_string(JSON.stringify({"error": message, "session_started": session != null}))
				report.close()
		get_tree().quit(1)

static func _integers(value: Variant) -> Variant:
	if value is Dictionary:
		var output: Dictionary = {}
		for key: String in value:
			output[key] = _integers(value[key])
		return output
	if value is Array:
		var output: Array = []
		for item: Variant in value:
			output.append(_integers(item))
		return output
	if value is float and is_finite(value) and value == floor(value) and abs(value) <= 4294967295.0:
		return int(value)
	return value
