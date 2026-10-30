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
const ExactMedia = preload("res://services/native_activity_media.gd")
const HolidayController = preload("res://scenes/holiday_controller.gd")
const HolidaySceneAdapter = preload("res://services/holiday_scene_adapter.gd")
const HolidayHud = preload("res://presentation/holiday_hud_adapter.gd")
const CalendarPhonePresenter = preload("res://presentation/calendar_phone_presenter.gd")
const DateAppointmentPresenter = preload("res://presentation/date_appointment_presenter.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")
const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const BoundarySaveControls = preload("res://app/boundary_save_controls.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const StateAdapter = preload("res://flow/new_game_state_adapter.gd")
const FreshHandoff = preload("res://flow/fresh_gameplay_handoff.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const AudioService = preload("res://services/audio_service.gd")
const AudioRules = preload("res://services/opening_audio_rules.gd")
const Math = preload("res://core/native_math.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
var hud: LiveHud
var exact_media: ExactMedia = ExactMedia.new()
var holiday_controller: HolidayController
var holiday_scene: HolidaySceneAdapter
var holiday_hud: HolidayHud
var calendar_phone_presenter: CalendarPhonePresenter
var date_appointment_presenter: DateAppointmentPresenter
var overlay_dispatcher: OverlayDispatcher
var audio_service: AudioService = AudioService.new()
var bgm_player: AudioStreamPlayer
var se_player: AudioStreamPlayer
var voice_player: AudioStreamPlayer
var new_game_boot: bool = false
var new_game_skip_prologue: bool = false
var allow_multi_week: bool = false
var _multi_week_attached: bool = false
const MODE_PROLOGUE: int = 0x15
const MODE_COMMAND_SCREEN: int = 0x34
const STAGE_READY: int = 1
const STAGE_RELOAD: int = 0xff
const MAX_PROLOGUE_STEPS: int = 4096
const MAX_RELOAD_STEPS: int = 64
const PAD_START_MASK: int = 0x0800
const AUDIO_CMD_STOP_BGM: int = 5
const AUDIO_CMD_FADE_MASK: int = 0xf0
const AUDIO_CMD_FADE_PREFIX: int = 0xb0
const SE_COMMAND_MIN: int = 0x0500

static func build_opening_services(audio_svc: AudioService, service_log: Array = []) -> Dictionary:
	return {
		"bank_load_start": func(s: RouteState, role: String, r: Session.FrameResult) -> void:
			service_log.append(["bank_load_start", role])
			var tick: int = int(s.frame_count)
			var q_err: String = audio_svc.queue_bank_load(role, tick, AudioService.Phase.AUDIO)
			if not q_err.is_empty():
				r.error = q_err
				return
			var drain_out: Dictionary = audio_svc.drain_phase(tick, AudioService.Phase.AUDIO)
			if not str(drain_out.get("error", "")).is_empty():
				r.error = str(drain_out["error"])
				return
			s.opening["audio_profile"] = role
			r.audio.append({"kind": "bank_load", "role": role}),
		"bank_poll": func(s: RouteState, _r: Session.FrameResult) -> int:
			service_log.append("bank_poll")
			var role: String = str(s.opening.get("audio_profile", ""))
			if role.is_empty():
				role = "prologue_arrival" if s.game_mode == MODE_PROLOGUE else "normal_gameplay"
			return 1 if bool(audio_svc.bank_ready.get(role, false)) else 0,
		"bank_start": func(s: RouteState, bank: int, seq: int, r: Session.FrameResult) -> void:
			service_log.append(["bank_start", bank, seq])
			var tick: int = int(s.frame_count)
			var q_err: String = audio_svc.queue_bgm_play(bank, seq, tick, AudioService.Phase.AUDIO)
			if not q_err.is_empty():
				r.error = q_err
				return
			var drain_out: Dictionary = audio_svc.drain_phase(tick, AudioService.Phase.AUDIO)
			if not str(drain_out.get("error", "")).is_empty():
				r.error = str(drain_out["error"])
				return
			r.audio.append({"kind": "bgm_play", "bank": bank, "sequence": seq}),
		"audio_command": func(s: RouteState, cmd: int, r: Session.FrameResult) -> void:
			service_log.append(["audio_command", cmd])
			var tick: int = int(s.frame_count)
			var q_err: String = ""
			if cmd == AUDIO_CMD_STOP_BGM or cmd in [0x71, 0xb1, 0xc1, 0xd1] or (cmd & AUDIO_CMD_FADE_MASK) == AUDIO_CMD_FADE_PREFIX:
				q_err = audio_svc.queue_bgm_stop(cmd, tick, AudioService.Phase.AUDIO)
				r.audio.append({"kind": "bgm_stop", "command": cmd})
			elif cmd >= SE_COMMAND_MIN:
				q_err = audio_svc.queue_se_play(cmd, tick, AudioService.Phase.AUDIO)
				r.audio.append({"kind": "se_play", "command": cmd})
			if not q_err.is_empty():
				r.error = q_err
				return
			if not audio_svc.queue.is_empty():
				var drain_out: Dictionary = audio_svc.drain_phase(tick, AudioService.Phase.AUDIO)
				if not str(drain_out.get("error", "")).is_empty():
					r.error = str(drain_out["error"]),
		"prologue_sine": Math.rsin,
		"finish_cd_reset": func(s: RouteState, r: Session.FrameResult) -> void:
			service_log.append("finish_cd_reset")
			AudioRules.finish_cd_reset(s, r),
		"finish_bank_reset": func(s: RouteState, r: Session.FrameResult) -> void:
			service_log.append("finish_bank_reset")
			AudioRules.finish_bank_reset(s, r),
		"scene_reset": func(s: RouteState, _r: Session.FrameResult) -> void:
			service_log.append("scene_reset")
			AudioRules.reset_lip_sync(s),
		"background_read": func(_s: RouteState, lba: int, slot: int, _r: Session.FrameResult, retry: bool = false) -> void:
			service_log.append(["bg_read", lba, slot, retry]),
		"cd_sync": func(_s: RouteState, _r: Session.FrameResult) -> int:
			service_log.append("cd_sync")
			return 1,
		"background_decode": func(_s: RouteState, slot: int, _r: Session.FrameResult) -> int:
			service_log.append(["bg_decode", slot])
			return 0,
		"actor_read": func(_s: RouteState, count: int, dest: int, lba: int, _r: Session.FrameResult) -> void:
			service_log.append(["actor_read", count, dest, lba]),
		"actor_palette_setup": func(_s: RouteState, _r: Session.FrameResult) -> void:
			service_log.append("actor_palette_setup"),
		"line_start": func(s: RouteState, line: int, r: Session.FrameResult) -> void:
			service_log.append(["line_start", line])
			var tick: int = int(s.frame_count)
			var q_err: String = audio_svc.queue_voice_play(line, tick, AudioService.Phase.AUDIO)
			if not q_err.is_empty():
				r.error = q_err
				return
			var drain_out: Dictionary = audio_svc.drain_phase(tick, AudioService.Phase.AUDIO)
			if not str(drain_out.get("error", "")).is_empty():
				r.error = str(drain_out["error"])
				return
			var spk: int = AudioRules.SPEAKER_PLAYER
			if line in [18, 19]:
				spk = AudioRules.SPEAKER_SHIORI
			elif line in [12, 13, 14]:
				spk = AudioRules.SPEAKER_REI
			elif line == 9:
				spk = AudioRules.SPEAKER_GIRL_A
			elif line == 10:
				spk = AudioRules.SPEAKER_GIRL_B
			elif line in [5, 7, 11, 15, 20, 22, 24, 25, 26, 28, 30, 32, 34, 36, 38, 40]:
				spk = AudioRules.SPEAKER_YOSHIO
			AudioRules.begin_line_lip_sync(s, line, spk)
			r.audio.append({"kind": "voice_play", "voice_id": line}),
		"line_run": func(s: RouteState, _r: Session.FrameResult) -> void:
			service_log.append("line_run")
			AudioRules.step_lip_sync(s)
			s.substep += 1,
		"lip_sync_yoshio": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_yoshio(s, lvl),
		"lip_sync_line_11": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_line_11(s, lvl),
		"lip_sync_girl_a": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_girl_a(s, lvl),
		"lip_sync_girl_b": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_girl_b(s, lvl),
		"lip_sync_rei": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_rei(s, lvl),
		"lip_sync_rei_lines_13_and_14": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_rei_lines_13_and_14(s, lvl),
		"lip_sync_lines_15_and_16": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_lines_15_and_16(s, lvl),
		"lip_sync_shiori": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_shiori(s, lvl),
		"lip_sync_shiori_line_19": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_shiori_line_19(s, lvl),
		"lip_sync_lines_20_to_31": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.lip_sync_lines_20_to_31(s, lvl),
		"actor100_pulse": func(s: RouteState) -> Dictionary:
			return AudioRules.actor100_pulse(s),
		"step_lip_sync": func(s: RouteState, lvl: int = -1) -> Dictionary:
			return AudioRules.step_lip_sync(s, lvl),
		"prologue_shiori_birthday_speak": func(s: RouteState, r: Session.FrameResult) -> void:
			service_log.append(["prologue_shiori_birthday_speak", 100])
			s.opening["prologue_dynamic_line"] = "shiori_birthday"
			AudioRules.begin_line_lip_sync(s, 100, AudioRules.SPEAKER_PLAYER)
			AudioRules.actor100_pulse(s)
			r.audio.append({"kind": "voice_play", "voice_id": 100}),
		"prologue_shiori_blood_type_speak": func(s: RouteState, r: Session.FrameResult) -> void:
			service_log.append(["prologue_shiori_blood_type_speak", 101])
			s.opening["prologue_dynamic_line"] = "shiori_blood"
			AudioRules.begin_line_lip_sync(s, 101, AudioRules.SPEAKER_PLAYER)
			AudioRules.actor100_pulse(s)
			r.audio.append({"kind": "voice_play", "voice_id": 101}),
		"arrival_draw": func(_s: RouteState, arg: int, _r: Session.FrameResult) -> void:
			service_log.append(["arrival_draw", arg]),
		"outer_draw": func(_s: RouteState, a: Variant = null, _b: Variant = null) -> void:
			service_log.append(["outer_draw", a]),
		"setup_reset": func(s: RouteState, _r: Session.FrameResult) -> void:
			service_log.append("setup_reset")
			s.next_step(),
		"hud_read_start": func(_s: RouteState, n: int, d: int, l: int, _r: Session.FrameResult) -> void:
			service_log.append(["hud_read_start", n, d, l]),
		"calendar_upload": func(_s: RouteState, y: int, m: int, b: int, _r: Session.FrameResult) -> void:
			service_log.append(["calendar_upload", y, m, b]),
		"mascot_set": func(_s: RouteState, n: int, on: int, _r: Session.FrameResult) -> void:
			service_log.append(["mascot_set", n, on]),
		"command_build": func(s: RouteState, r: Session.FrameResult) -> void:
			service_log.append("command_build")
			r.error = Reload.build_command_screen(s),
		"room_finish": func(s: RouteState, _r: Session.FrameResult) -> void:
			service_log.append("room_finish")
			Reload.finish_room_load(s),
		"frame_draw": func(_s: RouteState, title: bool = false, _r: Variant = null) -> void:
			service_log.append(["frame_draw", title]),
	}

static func run_new_game_opening_to_command_ready(
	entry: Entry,
	rng: Rng,
	opening_pack_path: String,
	witness_receipt_path: String,
	skip_prologue: bool,
	audio_svc: AudioService,
	initial_frame_count: int = 1000,
	initial_system_seed: int = 2000
) -> Dictionary:
	if not opening_pack_path.is_empty():
		var cfg_err: String = audio_svc.configure_opening_pack(opening_pack_path, witness_receipt_path)
		if not cfg_err.is_empty():
			return {"error": cfg_err}
	var initial_mascot: int = int(audio_svc.opening_pack.get("constants", {}).get("initial_mascot", 0))
	var adapter_out: StateAdapter.Outcome = StateAdapter.from_entry(
		entry,
		initial_frame_count,
		initial_system_seed,
		{"initial_mascot": initial_mascot}
	)
	if not adapter_out.error.is_empty() or adapter_out.state == null:
		return {"error": "state adapter failed: " + adapter_out.error}
	var route_state: RouteState = adapter_out.state
	var service_log: Array = []
	var services: Dictionary = build_opening_services(audio_svc, service_log)
	var visited_stages: Array[int] = []
	var prologue_steps: int = 0
	while route_state.game_mode == MODE_PROLOGUE and prologue_steps < MAX_PROLOGUE_STEPS:
		if visited_stages.is_empty() or visited_stages[-1] != route_state.mode_stage:
			visited_stages.append(route_state.mode_stage)
		var pressed: int = 0
		if skip_prologue and route_state.mode_stage == 0 and route_state.mode_step == 2 and route_state.substep == 2:
			pressed = PAD_START_MASK
		var step_res: Session.FrameResult = Arrival.step(route_state, rng, pressed, services)
		if not step_res.error.is_empty():
			return {"error": "prologue step failed: " + step_res.error}
		route_state.frame_count += 1
		prologue_steps += 1
	if route_state.game_mode != MODE_COMMAND_SCREEN or route_state.mode_stage != STAGE_RELOAD:
		return {"error": "prologue did not transition to command reload 0x34 stage 0xFF"}
	var reload_steps: int = 0
	while route_state.game_mode == MODE_COMMAND_SCREEN and route_state.mode_stage != STAGE_READY and reload_steps < MAX_RELOAD_STEPS:
		var reload_res: Session.FrameResult = Reload.step(route_state, services)
		if not reload_res.error.is_empty():
			return {"error": "command reload failed: " + reload_res.error}
		route_state.frame_count += 1
		reload_steps += 1
	if route_state.game_mode != MODE_COMMAND_SCREEN or route_state.mode_stage != STAGE_READY or not bool(route_state.opening.get("command_ready", false)):
		return {"error": "command reload did not reach ready stage 1"}
	return {
		"error": "",
		"state": route_state,
		"visited_stages": visited_stages,
		"prologue_steps": prologue_steps,
		"reload_steps": reload_steps,
		"service_log": service_log,
	}
const BOUNDARY_SHA: String = "cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5"
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const TABLES_SHA: String = ContentIdentity.RULE_TABLES_SHA

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
var _captured_selection_settled: bool = false
var _last_rendered_image: Image
var initial_params: Array = []
var video_frame: int = 0
var video_capture_busy: bool = false
var smoke_command: int = 0
var save_controls: BoundarySaveControls = BoundarySaveControls.new()
var clear_save_host: ClearSave
## T-39007: set by the title Continue owner before this node enters the tree:
## {"slot": String, "checksum_sha256": String} of the save the player chose.
var continue_request: Dictionary = {}
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
		_sync_audio_players_paused(paused_now)
		playback_pause_changed.emit(paused_now)

func _sync_audio_players_paused(paused_now: bool) -> void:
	if bgm_player != null and bgm_player.is_inside_tree():
		bgm_player.stream_paused = paused_now
	if se_player != null and se_player.is_inside_tree():
		se_player.stream_paused = paused_now
	if voice_player != null and voice_player.is_inside_tree():
		voice_player.stream_paused = paused_now

func _ensure_audio_players() -> void:
	if bgm_player == null:
		bgm_player = AudioStreamPlayer.new()
		bgm_player.name = "BgmPlayer"
		bgm_player.bus = "BGM"
		add_child(bgm_player)
	if se_player == null:
		se_player = AudioStreamPlayer.new()
		se_player.name = "SePlayer"
		se_player.bus = "SFX"
		add_child(se_player)
	if voice_player == null:
		voice_player = AudioStreamPlayer.new()
		voice_player.name = "VoicePlayer"
		voice_player.bus = "Voice"
		add_child(voice_player)
	if not playback_pause_changed.is_connected(_sync_audio_players_paused):
		playback_pause_changed.connect(_sync_audio_players_paused)
	_sync_audio_players_paused(playback_paused)

func _play_committed_audio(audio_events: Array) -> void:
	if audio_events.is_empty():
		return
	_ensure_audio_players()
	for raw_evt: Variant in audio_events:
		if not raw_evt is Dictionary:
			continue
		var evt: Dictionary = raw_evt
		var kind: String = str(evt.get("kind", ""))
		match kind:
			"bgm_play", "native_restore_bgm":
				var seq_id: int = int(evt.get("sequence", evt.get("bank", 0)))
				bgm_player.stream = audio_service.create_pcm_wav_stream("bgm_play", seq_id, 320)
				if bgm_player.is_inside_tree():
					bgm_player.play()
					bgm_player.stream_paused = playback_paused
			"bgm_stop", "native_stop_xa_if_active":
				if bgm_player.is_inside_tree():
					bgm_player.stop()
			"se_play", "native_audio_command":
				var cmd: int = int(evt.get("command", 0))
				if cmd == AUDIO_CMD_STOP_BGM or (cmd & AUDIO_CMD_FADE_MASK) == AUDIO_CMD_FADE_PREFIX:
					if bgm_player.is_inside_tree():
						bgm_player.stop()
				elif cmd > 0:
					se_player.stream = audio_service.create_pcm_wav_stream("se_play", cmd, 160)
					if se_player.is_inside_tree():
						se_player.play()
						se_player.stream_paused = playback_paused
			"voice_play":
				var voice_id: int = int(evt.get("voice_id", 0))
				voice_player.stream = audio_service.create_pcm_wav_stream("voice_play", voice_id, 220)
				if voice_player.is_inside_tree():
					voice_player.play()
					voice_player.stream_paused = playback_paused

func _ready() -> void:
	add_to_group("settings_save_hosts")
	output_root = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	smoke = OS.get_environment("TOKIMEMO_PLAYABLE_SMOKE") == "1"
	smoke_command = int(OS.get_environment("TOKIMEMO_PLAYABLE_SMOKE_COMMAND"))
	continue_week = OS.get_environment("TOKIMEMO_PLAYABLE_FIRST_RESULT_ONLY") != "1"
	# Continue restores a saved session; it never runs the new-game entry.
	new_game_boot = continue_request.is_empty() and (OS.get_environment("TOKIMEMO_NEW_GAME_BOOT") == "1" or OS.get_environment("TOKIMEMO_BOOT_MODE") == "new_game")
	new_game_skip_prologue = OS.get_environment("TOKIMEMO_PROLOGUE_SKIP") == "1"
	allow_multi_week = allow_multi_week or OS.get_environment("TOKIMEMO_MULTI_WEEK") == "1"
	_ensure_audio_players()
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
	if smoke_command not in [0, 1, 2, 3, 4, 5, 6, 7]:
		_stop("only source commands 0-7 scripted input recipes are available")
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
	clear_save_host = ClearSave.new(OS.get_user_data_dir().path_join("boundary-saves"))
	if not continue_request.is_empty():
		failure = save_controls.continue_from(self, continue_request)
		if not failure.is_empty():
			_stop("Continue failed: " + failure)
			return
	set_physics_process(false)
	for warmup: int in range(2):
		await get_tree().process_frame
		if DisplayServer.get_name() != "headless":
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
	calendar_phone_presenter = null
	date_appointment_presenter = null
	var state_path: String = OS.get_environment("TOKIMEMO_BOUNDARY")
	var table_path: String = OS.get_environment("TOKIMEMO_TABLES")
	var media_path: String = OS.get_environment("TOKIMEMO_MEDIA_PACK")
	var spring_pack: String = OS.get_environment("T29014_SPRING_PACK")
	if spring_pack.is_empty():
		spring_pack = OS.get_environment("TOKIMEMO_SPRING_PACK")
	var weekday56_pack: String = OS.get_environment("T29014_WEEKDAY56_PACK")
	if weekday56_pack.is_empty():
		weekday56_pack = OS.get_environment("TOKIMEMO_WEEKDAY56_PACK")
	var club_pack: String = OS.get_environment("T29014_CLUB_PACK")
	if club_pack.is_empty():
		club_pack = OS.get_environment("TOKIMEMO_CLUB_PACK")
	if new_game_boot:
		# A fresh New Game never reads the captured development boundary or
		# captured HUD context (flow/fresh_gameplay_handoff.gd). The boundary
		# SHA is not refused: save_controls.configure reads it as the save
		# content identity below.
		var captured_error: String = FreshHandoff.launch_input_error()
		if not captured_error.is_empty():
			return captured_error
	if not new_game_boot:
		if OS.get_environment("TOKIMEMO_BOUNDARY_SHA") != BOUNDARY_SHA or OS.get_environment("TOKIMEMO_TABLES_SHA") != TABLES_SHA:
			return "unsupported development boundary/table profile; use the reviewed captured inputs"
		if not _verified_external(state_path, OS.get_environment("TOKIMEMO_BOUNDARY_SHA")):
			return "development boundary missing or SHA256 differs from explicit launch identity"
		var parsed: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(state_path)))
		if not parsed is Dictionary or parsed.get("schema") != "tokimemo.development-boundary.v1":
			return "unsupported development-boundary schema"
		if not parsed.get("core") is Dictionary or not parsed.get("rule_state") is Dictionary or not parsed.get("rng_state") is int:
			return "development boundary lacks core/rules/RNG records"
		boundary = parsed
	if OS.get_environment("TOKIMEMO_TABLES_SHA") != TABLES_SHA or not _verified_external(table_path, OS.get_environment("TOKIMEMO_TABLES_SHA")):
		return "original rule tables missing or SHA256 differs from explicit launch identity"
	var table_pack: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(table_path)))
	if not table_pack is Dictionary or table_pack.get("schema") != "tokimemo.port-rule-tables.v1" or table_pack.get("schema_version") != 1 or not table_pack.get("tables") is Dictionary:
		return "unsupported original rule-table schema"
	if table_pack.get("provenance", {}).get("release_executable", {}).get("sha256") != MediaStore.EXE_SHA:
		return "rule tables are not the supported original executable profile"
	controller = Controller.new(table_pack["tables"])
	if not controller.error.is_empty():
		return controller.error
	var calendar_path: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	if not calendar_path.is_empty():
		var calendar_error: String = controller.month_start.open_source(calendar_path)
		if not calendar_error.is_empty():
			return "mandatory calendar source preflight: " + calendar_error
	var adapter_err: String = adapter.open_packs(media_path, spring_pack, weekday56_pack, club_pack)
	if not adapter_err.is_empty():
		return adapter_err
	var exact_error: String = exact_media.open(adapter)
	if not exact_error.is_empty():
		return exact_error
	for command: int in [0, 7]:
		if adapter.has_command(command):
			adapter.command_resources[command] = exact_media.get_resources(command)
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
		var font_root_path: String = OS.get_environment("TOKIMEMO_FONT_ROOT")
		var port_root: String = hud_manifest.get_base_dir().get_base_dir()
		var date_pack_path: String = OS.get_environment("TOKIMEMO_DATE_PACK")
		if date_pack_path.is_empty() and not port_root.is_empty():
			date_pack_path = port_root.path_join("date-preview-v1")
		if not font_root_path.is_empty() and ExternalPath.is_external_file(hud_manifest):
			var cand_cp := CalendarPhonePresenter.new()
			if cand_cp.init_resources(font_root_path, hud_manifest).is_empty():
				calendar_phone_presenter = cand_cp
		if not font_root_path.is_empty() and not date_pack_path.is_empty() and ExternalPath.is_external_file(date_pack_path.path_join("manifest.json")):
			var cand_dp := DateAppointmentPresenter.new()
			if cand_dp.open_date_preview(hud_manifest, font_root_path, date_pack_path):
				date_appointment_presenter = cand_dp
		if calendar_phone_presenter != null and date_appointment_presenter != null:
			candidate_controller.enable_subscreens = true
			candidate_scene.enable_subscreens = true
			var cand_od := OverlayDispatcher.new()
			if cand_od.open_resources(font_root_path, hud_manifest, date_pack_path).is_empty():
				overlay_dispatcher = cand_od
				if controller != null:
					cand_od.rule_tables = controller.tables
					controller.overlay_dispatcher = cand_od
		holiday_hud = candidate_hud
		holiday_scene = candidate_scene
		holiday_controller = candidate_controller
	if overlay_dispatcher == null and hud != null and not hud_manifest.is_empty():
		var od_font_root: String = OS.get_environment("TOKIMEMO_FONT_ROOT")
		var od_port_root: String = hud_manifest.get_base_dir().get_base_dir()
		var od_date_pack: String = OS.get_environment("TOKIMEMO_DATE_PACK")
		if od_date_pack.is_empty() and not od_port_root.is_empty():
			od_date_pack = od_port_root.path_join("date-preview-v1")
		if not od_font_root.is_empty() and not od_date_pack.is_empty() and ExternalPath.is_external_file(od_date_pack.path_join("manifest.json")):
			var fallback_od := OverlayDispatcher.new()
			if fallback_od.open_resources(od_font_root, hud_manifest, od_date_pack).is_empty():
				overlay_dispatcher = fallback_od
				if controller != null:
					fallback_od.rule_tables = controller.tables
					controller.overlay_dispatcher = fallback_od
	return ""

func _verified_external(path: String, digest: String) -> bool:
	return BoundarySaveControls.verified_external(path, digest)

func _enable_multi_week() -> String:
	if state == null:
		return "multi-week requires an initialized state"
	if controller == null or holiday_controller == null or holiday_scene == null or holiday_hud == null:
		return "multi-week requires original holiday scene/HUD/controller capabilities"
	state.rule_state["allow_multi_week"] = true
	state.rule_state["resources"]["available_commands"] = adapter.get_available_commands()
	state.rule_state["resources"]["available_holiday_commands"] = [0] if holiday_controller != null and holiday_scene != null and holiday_hud != null else []
	if holiday_controller != null and holiday_scene != null and not state.rule_state.get("holiday") is Dictionary:
		state.rule_state["holiday"] = holiday_scene.initial_extension()
	if controller != null:
		var all_cmds: Array[int] = adapter.get_available_commands()
		controller.command_capabilities.assign(all_cmds)
		var err: String = controller.attach_multi_week(session, state)
		if not err.is_empty():
			return err
	if holiday_controller != null:
		var h_err: String = holiday_controller.attach(session, state)
		if not h_err.is_empty():
			return h_err
	else:
		return "multi-week requires original holiday scene/HUD/controller capabilities"
	_multi_week_attached = true
	return ""

func _reset() -> String:
	if controller != null:
		var cleanup: String = controller.month_start.discard_services(state.frame_count if state != null else -1)
		if not cleanup.is_empty():
			return cleanup
		controller.month_start.reset_services()
	_ensure_audio_players()
	_multi_week_attached = false
	var was_multi_week: bool = allow_multi_week or OS.get_environment("TOKIMEMO_MULTI_WEEK") == "1" or (state != null and bool(state.rule_state.get("allow_multi_week", false)))
	session = Session.new()
	if new_game_boot:
		var ng_tables_path: String = OS.get_environment("TOKIMEMO_NEW_GAME_TABLES")
		var ng_content_path: String = OS.get_environment("TOKIMEMO_NEW_GAME_CONTENT")
		var opening_pack_path: String = OS.get_environment("TOKIMEMO_OPENING_PACK")
		var witness_receipt_path: String = OS.get_environment("TOKIMEMO_WITNESS_RECEIPT")
		var ng_tables: Dictionary = Entry.Rules.load_tables(ng_tables_path)
		var ng_content: Dictionary = Entry.load_content(ng_content_path)
		if not session.rng.configure(Rng.OPENBIOS_PROFILE, 19950404):
			return session.rng.configuration_error()
		var entry: Entry = Entry.new()
		var begin_err: String = entry.begin(ng_tables, ng_content, session.rng)
		if not begin_err.is_empty():
			return begin_err
		for action_kind: String in ["tick", "start", "tick", "start", "tick", "start", "tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
			var act_err: String = entry.act({"kind": action_kind})
			if not act_err.is_empty():
				return act_err
		var opening_out: Dictionary = run_new_game_opening_to_command_ready(
			entry,
			session.rng,
			opening_pack_path,
			witness_receipt_path,
			new_game_skip_prologue,
			audio_service
		)
		if not str(opening_out.get("error", "")).is_empty():
			return str(opening_out["error"])
		state = opening_out["state"]
		controller.opening_services = build_opening_services(audio_service)
		if bool(audio_service.bgm_state.get("playing", false)):
			_play_committed_audio([{"kind": "bgm_play", "bank": int(audio_service.bgm_state.get("bank", 0)), "sequence": int(audio_service.bgm_state.get("sequence", 0))}])
	else:
		state = State.new()
		var core_fields: Dictionary = CoreState.new().projection()
		if boundary["core"].size() != core_fields.size():
			return "development core record differs from supported field set"
		for key: String in core_fields:
			if not boundary["core"].has(key) or typeof(boundary["core"][key]) != typeof(core_fields[key]):
				return "development core field has wrong type: " + key
			state.set(key, boundary["core"][key])
		state.rule_state = boundary["rule_state"].duplicate(true)
		if smoke:
			# T-29014 Section 12a point 4: Authentic PS1 hardware RAM icon_enabled array (D_80125C90) & initial parameters (D_800E637C)
			if state.rule_state.has("icon_enabled") and state.rule_state["icon_enabled"].size() >= 14:
				state.rule_state["icon_enabled"][13] = 1
			var authentic_params: Array = [
				96 << 16, 47 << 16, 40 << 16, 40 << 16, 38 << 16, 34 << 16, 58 << 16, 4 << 16, 3 << 16
			]
			state.rule_state["params"] = authentic_params.duplicate()
		if [state.game_mode, state.mode_stage, state.mode_step] != [0x34, 1, 0] or [state.rule_state.get("year"), state.rule_state.get("month"), state.rule_state.get("day")] != [95, 4, 4]:
			return "only the genuine initial April4 command boundary is supported"
		if not session.rng.configure(Rng.OPENBIOS_PROFILE, boundary["rng_state"]):
			return session.rng.configuration_error()
	# Content capabilities are immutable controller configuration; preserving the
	# legacy record also preserves all existing command0 state/event hashes.
	if not controller.month_start.identity().is_empty():
		state.rule_state["resources"][Controller.MonthStart.SOURCE_KEY] = controller.month_start.identity()
	var error: String = ""
	if was_multi_week or bool(state.rule_state.get("allow_multi_week", false)):
		if holiday_controller != null:
			if holiday_scene == null or holiday_hud == null or hud == null:
				return "holiday controller requires preflighted holiday scene and HUD"
			state.rule_state["holiday"] = holiday_scene.initial_extension()
		error = _enable_multi_week()
		if not error.is_empty():
			return error
	else:
		state.rule_state["resources"]["available_commands"] = [0]
		if holiday_controller != null:
			if holiday_scene == null or holiday_hud == null or hud == null:
				return "holiday controller requires preflighted holiday scene and HUD"
			state.rule_state["holiday"] = holiday_scene.initial_extension()
			state.rule_state["resources"]["available_holiday_commands"] = [0]
		error = controller.attach(session, state)
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
	_captured_selection_settled = false
	_last_rendered_image = null
	initial_params = state.rule_state["params"].duplicate()
	canvas.present([])
	if hud != null:
		hud.reset(state.rule_state)
		var modern_settings := get_node_or_null("/root/ModernSettings")
		if modern_settings != null:
			hud.presenter.set_translated_images(modern_settings.translated_images)
			hud.presenter.set_translated_text(modern_settings.translated_text)
			if not modern_settings.preferences_changed.is_connected(_refresh_translation_images):
				modern_settings.preferences_changed.connect(_refresh_translation_images)
		error = _present_committed()
		if not error.is_empty():
			return error
	_update_labels()
	status_label.hide()
	detail_label.hide()
	return ""

func _refresh_translation_images(_preferences: Dictionary) -> void:
	var modern_settings := get_node_or_null("/root/ModernSettings")
	if hud == null or modern_settings == null:
		return
	hud.presenter.set_translated_images(modern_settings.translated_images)
	hud.presenter.set_translated_text(modern_settings.translated_text)
	var presentation_error := _present_committed()
	if not presentation_error.is_empty():
		_stop(presentation_error)

func configure_month_start_services(notice_owner: Variant = null) -> String:
	# Root's real AudioService factory supplies the native backend/profile. Merely
	# opening CALROM or attaching a month controller cannot grant audio readiness.
	if controller == null:
		return "month-start controller is not configured"
	var error: String = Controller.MonthStart.AudioBridge.owner_error(audio_service)
	if not error.is_empty():
		return error
	if notice_owner != null:
		error = Controller.MonthStart.notice_owner_error(notice_owner)
		if not error.is_empty():
			return error
	error = controller.month_start.audio.bind_owner(audio_service)
	if not error.is_empty():
		return error
	if notice_owner != null:
		return controller.month_start.bind_notice_owner(notice_owner)
	return ""

func _input(event: InputEvent) -> void:
	if save_controls.handle_input(self, event):
		return
	if not focus_paused and event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_F3:
		detail_label.visible = not detail_label.visible
	if not focus_paused and event is InputEventKey and event.pressed and not event.echo and event.physical_keycode == KEY_R and stopped and not smoke:
		_replay_requested = true

func request_settings_save(load_game: bool) -> bool:
	return save_controls.request(self, BoundarySaveControls.Request.LOAD if load_game else BoundarySaveControls.Request.SAVE)

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
	# T-39007: storage runs here, between committed steps; the session sees
	# only the completion with its read-back receipt or failure reason.
	var clear_save: Dictionary = _clear_save_service()
	if not clear_save.is_empty():
		events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, clear_save["service"], clear_save["payload"]))
	staged_commands.clear()
	staged_services.clear()
	staged_hud_refresh = false
	if state != null and (allow_multi_week or bool(state.rule_state.get("allow_multi_week", false))) and not _multi_week_attached:
		var mw_err: String = _enable_multi_week()
		if not mw_err.is_empty():
			_stop(mw_err)
			return
	var before_hash: String = session.state_hash()
	var month_frame: int = state.frame_count
	# Real source decoding/reservation belongs outside Session callbacks. A missing
	# quarter capability is reported when its source rule actually requests it.
	controller.month_start.prepare_services(state)
	var result: Session.FrameResult = session.step(pad.sample(tick), events)
	if not result.committed:
		var cleanup: String = controller.month_start.discard_services(month_frame)
		if not cleanup.is_empty():
			_stop(result.error + "; native preparation cleanup: " + cleanup)
			return
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
	var month_publication: String = controller.month_start.publish_services(result)
	var month_cleanup: String = controller.month_start.discard_services(month_frame)
	if not month_publication.is_empty() or not month_cleanup.is_empty():
		_stop("committed month service failure: " + month_publication + " " + month_cleanup)
		return
	_play_committed_audio(result.audio)
	var error: String = _present_committed()
	if not error.is_empty():
		_stop(error)
		return
	_update_labels()
	if staged_hud_refresh:
		status_label.text += " · week-end health check"
	_record_checkpoint(result)
	if not _captured_selection_settled and smoke and smoke_command > 0 and tick >= 85 and state.game_mode in [0x30, 0x34]:
		_captured_selection_settled = true
		_capture("selection")
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

func _has_active_overlay(current: State) -> bool:
	if current == null:
		return false
	if current.rule_state.get("overlay_state") is Dictionary and not (current.rule_state["overlay_state"] as Dictionary).is_empty():
		return true
	if current.game_mode == 0x46:
		return false
	if current.game_mode == 0x50 and holiday_scene != null and holiday_scene.handles(current):
		return false
	return OverlayDispatcher.is_interactive_overlay_mode(current.game_mode)

func _build_overlay_render_state(current: State) -> Dictionary:
	var live_ctx: Dictionary = TelephoneOverlay.extract_live_context(current.rule_state)
	var ovl_state: Dictionary = current.rule_state.get("overlay_state", {}) if current.rule_state.get("overlay_state") is Dictionary else {}
	var merged: Dictionary = live_ctx.duplicate(true)
	for k in ovl_state.keys():
		merged[k] = ovl_state[k]
	for k: String in ["year", "month", "day", "weekday", "day_kind", "day_count", "player_birthday", "stats", "progress_state", "quarter_flags", "cal0", "cal1"]:
		merged[k] = live_ctx[k]
	if not merged.has("appointment_words") and live_ctx.has("appointment_words"):
		merged["appointment_words"] = live_ctx["appointment_words"]
	return merged

func _clear_save_service() -> Dictionary:
	if clear_save_host == null or state == null or not state.rule_state.get("overlay_state") is Dictionary:
		return {}
	return clear_save_host.service_request(state.rule_state["overlay_state"])

func step_active_overlay(pad_pressed: int) -> Dictionary:
	if state == null:
		return {"error": "playable_boot state is not initialized", "completed": false}
	var active_od: OverlayDispatcher = overlay_dispatcher if overlay_dispatcher != null else (controller.overlay_dispatcher if controller != null else OverlayDispatcher.new())
	var ovl_state: Dictionary = state.rule_state.get("overlay_state", {}) if state.rule_state.get("overlay_state") is Dictionary else {}
	# T-39007: a pending clear save is delivered only by _physics_process as a
	# session completion; this direct step never runs storage or applies it.
	if ovl_state.get("clear_save_status") == "requested":
		return {"error": "clear save is waiting for its storage completion", "completed": false}
	var ovl_mode: int = int(ovl_state.get("mode", state.game_mode))
	var step_out: Dictionary = active_od.step_overlay(
		ovl_mode, ovl_state, pad_pressed, state.rule_state, session.rng if session != null else null
	)
	var updated_ovl: Dictionary = step_out.get("state", ovl_state)
	state.rule_state["overlay_state"] = updated_ovl
	if bool(step_out.get("completed", false)):
		if controller != null:
			controller._apply_overlay_deltas(state, step_out.get("deltas", {}))
		state.rule_state["last_completed_overlay"] = updated_ovl.duplicate(true)
		state.rule_state.erase("overlay_state")
	if overlay_dispatcher != null and canvas != null:
		_present_committed()
	return step_out

func _native_draw(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
	var current: State = base as State
	result.error = HudSeason.draw_step(current, result)
	if not result.error.is_empty():
		return
	var multi: bool = bool(current.rule_state.get("allow_multi_week", false))
	if _has_active_overlay(current):
		if not (current.rule_state.get("overlay_state") is Dictionary) or (current.rule_state["overlay_state"] as Dictionary).is_empty():
			var active_od: OverlayDispatcher = overlay_dispatcher if overlay_dispatcher != null else (controller.overlay_dispatcher if controller != null else null)
			if active_od != null:
				current.rule_state["overlay_state"] = active_od.init_overlay(current.game_mode, current.rule_state)
		return
	if current.game_mode in [0x46, 0x50] or (holiday_scene != null and holiday_scene.handles(current)):
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
	var calendar_stage: Dictionary = controller.month_start.stage_services(current, result)
	if not calendar_stage["error"].is_empty():
		result.error = calendar_stage["error"]
		return
	prepared_services.append_array(calendar_stage["services"])
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
			var club_id: int = (data["player_flags"] >> 12) & 15 if command == 4 else -1
			if not multi:
				var expected: Dictionary = MediaAdapter.CLUB_ROUTES[club_id] if command == 4 else MEDIA_ROUTES[command]
				if not adapter.has_command(command, club_id) or intent.get("sd_set_id") != expected["sd_set_id"] or data["scene_work"] != intent["sd_set_id"]:
					result.error = "actor request differs from validated command media profile"
					return
				if command == 7 and (intent["actors"].size() != 1 or intent["actors"][0].get("id") != 96 or intent["actors"][0].get("sequence_slot") != 0):
					result.error = "rest supports only the captured actor96 sequence0"
					return
				if command in [1, 2, 3, 4, 5, 6] and (intent["actors"].size() != 1 or intent["actors"][0].get("id") != 96):
					result.error = "opening week command %d supports only solo actor96" % command
					return
			elif command < 0 or not adapter.has_command(command, club_id):
				result.error = "actor request differs from validated command media profile"
				return
			var selected: Dictionary = exact_media.resolve(current)
			if not selected["error"].is_empty():
				result.error = selected["error"]
				return
			if intent.get("sd_set_id") != selected["route"]["sd_set_id"] or data["scene_work"] != selected["route"]["sd_set_id"]:
				result.error = "actor SD profile differs from exact original media"
				return
			var res_dict: Dictionary = selected["resources"]
			var initialized: Dictionary = actor_service.initialize(intent["actors"], res_dict)
			if not initialized["error"].is_empty():
				result.error = initialized["error"]
				return
			current.rule_state["actor_state"] = initialized["actor_state"]
		elif intent.get("kind") == "school_day_cached_background":
			var slot: int = intent.get("slot", -1)
			var command: int = _activity_command(current)
			var club_id: int = (data["player_flags"] >> 12) & 15 if command == 4 else -1
			if multi:
				if command < 0 or slot not in [0, 1] or data["background_show"] != slot or data["background_cache"][slot] != intent.get("lba"):
					result.error = "cached background differs from validated command slot/LBA"
					return
			else:
				var expected: Dictionary = MediaAdapter.CLUB_ROUTES[club_id] if command == 4 else MEDIA_ROUTES[command]
				if not adapter.has_command(command, club_id) or slot not in [0, 1] or intent.get("lba") != expected["background_lba"] or data["background_show"] != slot or data["background_cache"][slot] != intent["lba"]:
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
	if current.game_mode in [0x40, 0x31, 0x32, 0x38]:
		var shown_lba: int = data["background_cache"][data["background_show"]]
		var club_id: int = (data["player_flags"] >> 12) & 15
		var found_bg: bool = false
		if MediaAdapter.CLUB_ROUTES.has(club_id) and shown_lba == MediaAdapter.CLUB_ROUTES[club_id]["background_lba"]:
			prepared_commands.append_array(adapter.get_background_commands(4, club_id))
			found_bg = true
		if not found_bg:
			for command: int in command_backgrounds:
				if shown_lba == MEDIA_ROUTES[command]["background_lba"]:
					prepared_commands.append_array(command_backgrounds[command])
					found_bg = true
		if not found_bg and holiday_scene != null and holiday_scene.backgrounds.has(shown_lba):
			prepared_commands.append_array(holiday_scene.backgrounds[shown_lba])
			found_bg = true
		if not found_bg and shown_lba != 0 and ((current.game_mode == 0x40 and current.mode_stage != 0) or current.game_mode == 0x38):
			result.error = "active source background has no exact validated original image: %d" % shown_lba
			return
	if not data["actor_state"].is_empty():
		var command: int = _activity_command(current)
		var club_id: int = (data["player_flags"] >> 12) & 15 if command == 4 else -1
		if not multi and not adapter.has_command(command, club_id):
			result.error = "active actor has no validated command resource set"
			return
		elif multi and command < 0:
			result.error = "active actor has no validated command resource set"
			return
		var selected: Dictionary = exact_media.resolve(current)
		if not selected["error"].is_empty():
			result.error = selected["error"]
			return
		var res: Dictionary = selected["resources"]
		var drawn: Dictionary = actor_service.native_draw(data["actor_state"], res,
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
	if event[0] != 0 or event[1] not in [0, 1, 2, 3, 4, 5, 6, 7]:
		return -1
	if data.get("allow_multi_week", false):
		if event[1] == 4 and ((data["player_flags"] >> 12) & 15) >= 11:
			return -1
		return event[1]
	var health: int = (data["player_flags"] >> 8) & 15
	if event[1] == 7:
		var index: int = data["day_event_index"]
		if event != [0, 7, 0, 15] or index not in [0, 2, 4, 6] or [data["year"], data["month"], data["day"]] != [95, 4, 5 + (index >> 1)] or data["vacation"] != 0 or health != 0:
			return -1
	elif event[1] in [1, 2, 3, 5, 6]:
		var index: int = data["day_event_index"]
		if event[3] != 15 or index not in [0, 2, 4, 6] or [data["year"], data["month"], data["day"]] != [95, 4, 5 + (index >> 1)] or data["vacation"] != 0 or health != 0:
			return -1
	elif event[1] == 4:
		var index: int = data["day_event_index"]
		var club_id: int = (data["player_flags"] >> 12) & 15
		if club_id >= 11 or event[3] != 15 or index not in [0, 2, 4, 6] or [data["year"], data["month"], data["day"]] != [95, 4, 5 + (index >> 1)] or data["vacation"] != 0 or health != 0:
			return -1
	return event[1]

func _composite_commands_to_image(cmds: Array) -> Image:
	var out := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	out.fill(Color(0, 0, 0, 1))
	for cmd: Dictionary in cmds:
		var img: Image = cmd.get("image")
		if img == null or img.is_empty():
			continue
		var pos: Vector2i = cmd.get("position", Vector2i.ZERO)
		var src_img: Image = img
		if src_img.get_format() != Image.FORMAT_RGBA8:
			src_img = src_img.duplicate()
			src_img.convert(Image.FORMAT_RGBA8)
		out.blend_rect(src_img, Rect2i(Vector2i.ZERO, src_img.get_size()), pos)
	return out

## Save adapter post_apply hook. It rebuilds only owned presentation caches.
func _present_committed(_restored_app: Node = null) -> String:
	if holiday_controller != null and holiday_controller.handles_subscreen(state):
		var sub_out: Dictionary = holiday_controller.render_active_subscreen(state, calendar_phone_presenter, date_appointment_presenter)
		if not str(sub_out.get("error", "")).is_empty():
			return str(sub_out["error"])
		var sub_commands: Array[Dictionary] = []
		sub_commands.assign(sub_out.get("commands", []))
		if sub_out.get("image") is Image:
			_last_rendered_image = sub_out["image"]
		elif not sub_commands.is_empty():
			_last_rendered_image = _composite_commands_to_image(sub_commands)
		canvas.set("_textures", {})
		return canvas.present(sub_commands)
	if state.game_mode == 0x46 or (holiday_scene != null and holiday_scene.handles(state)):
		var multi: bool = bool(state.rule_state.get("allow_multi_week", false))
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
		if not holiday_commands.is_empty():
			_last_rendered_image = _composite_commands_to_image(holiday_commands)
		canvas.set("_textures", {})
		return canvas.present(holiday_commands)
	if overlay_dispatcher != null and _has_active_overlay(state):
		var render_state: Dictionary = _build_overlay_render_state(state)
		var ovl_mode: int = int(render_state.get("mode", state.game_mode))
		var ovl_out: Dictionary = overlay_dispatcher.render_mode(ovl_mode, render_state)
		if not str(ovl_out.get("error", "")).is_empty():
			return str(ovl_out["error"])
		if ovl_out.get("image") is Image:
			_last_rendered_image = ovl_out["image"]
		var ovl_cmds: Array[Dictionary] = []
		ovl_cmds.assign(ovl_out.get("commands", []))
		canvas.set("_textures", {})
		return canvas.present(ovl_cmds)
	if hud == null:
		return canvas.present(staged_commands) # Synthetic engine/focus test seam.
	var command: Dictionary = hud.compose(state.rule_state, state.projection(), trace, staged_commands)
	if command.is_empty():
		return hud.error
	if command.has("image") and command["image"] is Image:
		_last_rendered_image = command["image"]
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
	var club_id: int = (current.rule_state["player_flags"] >> 12) & 15 if command == 4 else -1
	var multi: bool = bool(current.rule_state.get("allow_multi_week", false))
	if not adapter.has_command(command, club_id):
		return "resource request has no preflighted command media profile"
	var selected: Dictionary = exact_media.resolve(current)
	if not selected["error"].is_empty():
		return selected["error"]
	var expected: Dictionary = selected["route"]
	if intent.size() != 8 or intent.get("request") not in ["background", "sd"] or intent.get("command") != command or intent.get("day_event_index") != current.rule_state["day_event_index"] or intent.get("sectors") != 41:
		return "resource request identity differs from validated command media profile"
	for key: String in ["background_lba", "sd_set_id", "sd_lba"]:
		if intent.get(key) != expected[key]:
			return "resource request differs from validated command media field: " + key
	if current.mode_stage != 0 or current.substep != 1 or current.mode_step != (0 if intent["request"] == "background" else 1):
		return "resource request arrived outside its native setup phase"
	return ""

func _bounded_cd(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
	# Validated immutable resources were read before the first simulation step.
	# This fence also runs if a draw callback deactivated the audio phase.
	result.error = controller.month_start.precommit_services(base as State, result)

func _bounded_audio(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
	result.error = controller.month_start.precommit_services(base as State, result)

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
		if DisplayServer.get_name() != "headless":
			await RenderingServer.frame_post_draw
	var image_320: Image = _last_rendered_image
	if image_320 == null or image_320.is_empty():
		var vp_img := get_viewport().get_texture().get_image()
		if vp_img != null and not vp_img.is_empty():
			image_320 = vp_img.duplicate()
			image_320.resize(320, 240, Image.INTERPOLATE_NEAREST)
	if image_320 == null or image_320.is_empty():
		_stop("actual viewport capture failed: no image available")
		return
	var image_960 := image_320.duplicate()
	image_960.resize(960, 720, Image.INTERPOLATE_NEAREST)
	if image_960.save_png(output_root.path_join("%s-%s.png" % [_capture_prefix(), name_value])) != OK:
		_stop("actual viewport capture failed")
		return
	if image_320.save_png(output_root.path_join("%s-%s-320x240.png" % [_capture_prefix(), name_value])) != OK:
		_stop("actual 320x240 viewport capture failed")
		return

func _stop(message: String) -> void:
	stopped = true
	failure = message
	if status_label != null:
		status_label.position = Vector2(16, 454)
		status_label.text = "Game stopped" if smoke else "Game stopped · R: restart this session"
		status_label.show()
	if not smoke:
		var dialog := AcceptDialog.new()
		dialog.title = "Game could not continue"
		dialog.dialog_text = message + "\n\nRestarting may encounter the same problem. Copy these details when reporting it.\nLog: " + ProjectSettings.globalize_path("user://logs/godot.log")
		dialog.dialog_autowrap = true
		dialog.add_button("Copy details", false, "copy")
		dialog.custom_action.connect(func(action: String):
			if action == "copy": DisplayServer.clipboard_set(dialog.dialog_text))
		dialog.confirmed.connect(dialog.queue_free)
		dialog.canceled.connect(dialog.queue_free)
		add_child(dialog)
		dialog.popup_centered(Vector2i(560, 280))
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
