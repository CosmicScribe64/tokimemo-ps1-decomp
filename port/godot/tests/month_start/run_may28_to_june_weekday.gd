extends SceneTree
## T-29023 / T-29027 / T-29032: Headless end-to-end verification from the
## frozen 1995-05-28 Sunday boundary through the 1995-06-01 quarterly transition
## (CALROM.NBN upload, summer HUD fade 0xef..0xff->0x00, warm OriginalAudioEngine
## COM_MIX + SUMMER bank load/poll [-1, -1, 1]/same-frame start(0, 0) with
## gain 86, raw_reverb_depth 56, vm_initializations 1, preserved AudioStreamPlayer
## and sample_cursor), continuing through 1995-06-01..02 June weekday activities,
## 1995-06-03 Saturday boundary, 1995-06-04 Sunday holiday command/activity, and
## controllable June weekday command selection into 1995-06-05 weekday activity.

const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const CoreState = preload("res://core/game_state.gd")
const State = preload("res://rules/school_day_state.gd")
const Rng = preload("res://core/native_rng.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const HolidayController = preload("res://scenes/holiday_controller.gd")
const MediaStore = preload("res://services/school_day_media_store.gd")
const ExactMedia = preload("res://services/native_activity_media.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
const HudCompositor = preload("res://presentation/hud_compositor.gd")
const Bridge = preload("res://services/month_start_audio_bridge.gd")
const NativeBackend = preload("res://bgm_backend.gd")
const NativeSink = preload("res://bgm_sink.gd")
const NativeService = preload("res://services/shared_month_audio_service.gd")

const EXPECTED_LIB_SHA: String = "f1eecf80c1c6538a33f6bee9c7d10a79553b1745d520688f556bbe60fb1687b9"
const EXPECTED_MAY28_HASH: String = "ad782763f6f6cccc6fcd504ce06cb60f44d4f5f104d07d867621dd92d39aae45"
const EXPECTED_JUNE1_SCHEDULE_RNG: int = 2966941936
const PAD_CONFIRM: int = 0x20

var checks: int = 0
var failures: Array[String] = []
var native_service: NativeService
var native_backend: NativeBackend
var native_sink: NativeSink
var first_owner: Dictionary = {}

func _initialize() -> void:
	call_deferred("_run")

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures.append(label)
		push_error(label)

func _ensure_env(key: String, fallback: String) -> String:
	var val: String = OS.get_environment(key)
	if val.is_empty():
		val = fallback
		OS.set_environment(key, val)
	return val

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

func _originals(disc_root: String, summer: bool) -> Array[PackedByteArray]:
	var names: Array = [
		"SLPM_86.053",
		"CDROM/VAB_SE/COM_MIX.VH",
		"CDROM/VAB_SE/COM_MIX.VB",
		"CDROM/VAB_SEQ/SUMMER.VH",
		"CDROM/VAB_SEQ/SUMMER.VB",
		"CDROM/VAB_SEQ/SUMMER.SEQ",
	] if summer else [
		"SLPM_86.053",
		"CDROM/VAB_SE/SYS_MIX.VH",
		"CDROM/VAB_SE/SYS_MIX.VB",
		"CDROM/VAB_SEQ/PRO_VOL.VH",
		"CDROM/VAB_SEQ/PRO_VOL.VB",
		"CDROM/VAB_SEQ/PRO_VOL.SEQ",
	]
	var inputs: Array[PackedByteArray] = []
	for name: String in names:
		inputs.append(FileAccess.get_file_as_bytes(disc_root.path_join(name)))
	return inputs

func _run() -> void:
	var disc_root: String = _ensure_env("ORIGINAL_FILES", "/workspace/disc/files")
	var calendar_path: String = _ensure_env("TOKIMEMO_CALENDAR_SOURCE", disc_root.path_join("CDROM/FUKUTEST/CALROM.NBN"))
	var table_path: String = _ensure_env("TOKIMEMO_TABLES", "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json")
	var media_path: String = _ensure_env("TOKIMEMO_MEDIA_PACK", "/workspace/.asset-cache/port/holiday-command0-current-964ce")
	var ref_path: String = _ensure_env(
		"TOKIMEMO_RESUME_REFERENCE",
		"/workspace/.claude/orchestrator/state/T-29023/runs/checkpoint-current-v2-through-june4/resume-reference.json"
	)

	var descriptor: String = "res://original_audio.gdextension"
	check(FileAccess.get_sha256("res://bin/liboriginal_audio.linux.template_release.arm64.so") == EXPECTED_LIB_SHA, "reviewed warm V4 native library SHA matches")
	if not GDExtensionManager.is_extension_loaded(descriptor):
		check(GDExtensionManager.load_extension(descriptor) == GDExtensionManager.LOAD_STATUS_OK, "explicit GDExtension bootstrap succeeds")
	check(ClassDB.class_exists("OriginalAudioEngine"), "OriginalAudioEngine class registered")

	var host := Node.new()
	root.add_child(host)
	native_sink = NativeSink.new(host)
	native_backend = NativeBackend.new(native_sink)
	native_service = NativeService.new()
	first_owner = NativeBackend.ORIGINAL_IDENTITIES.duplicate(true)
	first_owner.merge({
		"role": "prologue_arrival",
		"epoch": 1,
		"ready": true,
		"load_set": 0,
		"sequence_pack_set": 0,
		"sequence": 0,
		"music_slot": 1,
		"SE_slot": 0,
		"state_flags": 0x30010,
		"reverb_preset": 3,
		"reverb_depth": 60,
		"reverb_enabled": true,
	})
	var prepared: Dictionary = native_backend.prepare_start(first_owner, 1, _originals(disc_root, false))
	check(str(prepared.get("error", "")).is_empty(), "warm prologue_arrival native engine prepares")
	check(native_backend.commit_start(prepared.get("ticket", -1), first_owner).is_empty(), "warm prologue_arrival native engine starts")
	var active_player: AudioStreamPlayer = native_sink._active
	check(active_player != null, "warm AudioStreamPlayer created by NativeSink")

	var main_flags: int = (int(first_owner["state_flags"]) | 0x100) & ~0x1000 & ~0x100000
	check(native_service.bind_month_start_backend(native_backend, _originals(disc_root, true), main_flags, 0x38).is_empty(), "SharedMonthAudioService binds warm NativeBackend with COM_MIX + SUMMER")

	var pump_error: String = ""
	var pump_until: int = Time.get_ticks_msec() + 150
	while Time.get_ticks_msec() < pump_until and pump_error.is_empty():
		pump_error = native_backend.pump(first_owner)
		await process_frame
	check(pump_error.is_empty(), "warm prologue_arrival playback pumps cleanly before May28->June1 route")
	var initial_native: Dictionary = native_backend.month_start_context()
	check(int(initial_native.get("sample_cursor", 0)) > 0, "warm sample_cursor advanced during initial pump")

	var table_pack: Dictionary = _integers(JSON.parse_string(FileAccess.get_file_as_string(table_path)))
	var controller := Controller.new(table_pack["tables"])
	check(controller.error.is_empty(), "SchoolDayController initializes with original rule tables")
	check(controller.month_start.open_source(calendar_path).is_empty(), "MonthStartController opens authenticated CALROM.NBN")
	check(controller.month_start.audio.bind_owner(native_service).is_empty(), "MonthStartController binds SharedMonthAudioService")

	var media_store := MediaStore.new()
	check(media_store.open_pack(media_path), "MediaStore opens command0/holiday media pack")
	var cmd0_resources: Dictionary = ExactMedia.from_media(media_store, "command_0_sd")
	var holiday_resources: Dictionary = ExactMedia.from_media(media_store, "holiday_command_0_sd")
	check(not cmd0_resources.is_empty() and not holiday_resources.is_empty(), "actor96 weekday and holiday SD resources open")
	var holiday_tables: Dictionary = _integers(media_store.get_route_tables())
	var holiday_controller := HolidayController.new(table_pack["tables"], holiday_tables)
	check(holiday_controller.error.is_empty(), "HolidayController initializes with original holiday route tables")
	var actor_service := ActorService.new()

	var parsed_ref: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(ref_path)))
	check(parsed_ref is Dictionary and parsed_ref.has("target"), "resume-reference.json target witness loaded")
	var target: Dictionary = parsed_ref["target"]

	var session := Session.new()
	var state := State.new()
	state.restore(target["state"])
	controller.command_capabilities.assign([0, 7])
	check(controller.attach_multi_week(session, state).is_empty(), "SchoolDayController attaches multi-week handlers")
	session.router._handlers.erase(0x72)
	check(holiday_controller.attach(session, state).is_empty(), "HolidayController attaches Sunday and post-Sunday handlers")
	session.clock.restore(target["clock"])
	session.rng.restore(target["rng"])
	var latch: Dictionary = target["input_latch"]
	var prev_mask: int = (int(latch["held"]) & ~int(latch["pressed"])) | int(latch["released"])
	session.latched_input = InputFrame.new(int(latch["tick"]), int(latch["held"]), prev_mask)
	check(session.state_hash() == EXPECTED_MAY28_HASH, "restored 1995-05-28 state hash matches frozen witness")
	check([state.rule_state["year"], state.rule_state["month"], state.rule_state["day"], state.rule_state["weekday"]] == [95, 5, 28, 6], "starting date is Sunday 1995-05-28")
	check([state.game_mode, state.mode_stage, state.mode_step] == [0x46, 1, 0], "starting mode is 0x46/1/0 Sunday command screen")

	var staged_services: Array[Dictionary] = []
	var pending_services: Array[Dictionary] = []

	session.draw_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		var current: State = base as State
		result.error = HudSeason.draw_step(current, result)
		if not result.error.is_empty():
			return
		var prepared_services: Array[Dictionary] = []
		var calendar_stage: Dictionary = controller.month_start.stage_services(current, result)
		if not calendar_stage["error"].is_empty():
			result.error = calendar_stage["error"]
			return
		prepared_services.append_array(calendar_stage["services"])
		for intent: Dictionary in result.presentation:
			var kind: String = str(intent.get("kind", ""))
			if kind == "school_day_load":
				prepared_services.append({
					"service": "school_day.resources",
					"payload": {
						"request": intent["request"],
						"ready": true,
						"lba": intent["background_lba"] if intent["request"] == "background" else intent["sd_lba"],
						"decode_result": 1,
						"checksum_ok": true,
					},
				})
			elif kind == "holiday_load":
				var h_payload: Dictionary = {
					"request": intent["request"],
					"lba": intent["lba"],
					"ready": true,
				}
				if intent["request"] != "sd":
					h_payload["decode_result"] = 1
				prepared_services.append({
					"service": "holiday.resources",
					"payload": h_payload,
				})
			elif kind == "school_day_actors":
				var init_act: Dictionary = actor_service.initialize(intent["actors"], cmd0_resources)
				if not init_act["error"].is_empty():
					result.error = init_act["error"]
					return
				current.rule_state["actor_state"] = init_act["actor_state"]
			elif kind == "holiday_actors":
				var init_h: Dictionary = actor_service.initialize(intent["actors"], holiday_resources)
				if not init_h["error"].is_empty():
					result.error = init_h["error"]
					return
				current.rule_state["actor_state"] = init_h["actor_state"]
		if not current.rule_state["actor_state"].is_empty():
			var res_for_draw: Dictionary = holiday_resources if current.game_mode == 0x46 else cmd0_resources
			var drawn: Dictionary = actor_service.native_draw(
				current.rule_state["actor_state"],
				res_for_draw,
				session.clock.logic_iterations,
				Vector2i(80, 56) if current.game_mode == 0x46 else Vector2i(160, 120)
			)
			if not drawn["error"].is_empty():
				result.error = drawn["error"]
				return
			current.rule_state["actor_state"] = drawn["actor_state"]
			for svc: Dictionary in drawn["services"]:
				if current.game_mode == 0x46 and svc.get("service") == "school_day.actor96":
					prepared_services.append({"service": "holiday.actor96", "payload": svc["payload"]})
				else:
					prepared_services.append(svc)
		staged_services.append_array(prepared_services)

	session.cd_logic = func(base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
		result.error = controller.month_start.precommit_services(base as State, result)
	session.audio_logic = session.cd_logic

	var seen_dates: Array[String] = []
	var june1_polls: Array[int] = []
	var june1_operations: Array[String] = []
	var june1_entered_tick: int = -1
	var june1_exited_tick: int = -1
	var june1_upload_season_fade: int = -1
	var june1_pre_upload_season_view: Dictionary = {}
	var june1_post_upload_season_view: Dictionary = {}
	var may29_schedule_rng: int = -1
	var may29_fe_record: Array = []
	var june1_thursday_activity_reached: bool = false
	var june2_friday_activity_reached: bool = false
	var june3_unbound_stop_verified: bool = false
	var june3_stop_tick: int = -1
	var june3_bind_rejection: String = ""
	var june4_holiday_reached: bool = false
	var june5_weekday_command_reached: bool = false
	var june5_weekday_activity_reached: bool = false

	var expansion_phase: String = "waiting_menu"
	var expansion_tick: int = 0
	var held_mask: int = 0
	var prev_held_mask: int = 0
	var max_steps: int = 1600
	var step_count: int = 0
	var step_failure: String = ""

	while step_count < max_steps and step_failure.is_empty() and not june5_weekday_activity_reached:
		var tick: int = session.clock.logic_iterations
		var rs: Dictionary = state.rule_state
		var date_str: String = "%02d-%02d-%02d" % [int(rs["year"]), int(rs["month"]), int(rs["day"])]
		if seen_dates.is_empty() or seen_dates[-1] != date_str:
			seen_dates.append(date_str)

		var on_controllable_menu: bool = (state.game_mode in [0x34, 0x46] and state.mode_stage == 1 and state.mode_step == 0) or state.game_mode == 0x72
		if [int(rs["year"]), int(rs["month"]), int(rs["day"])] == [95, 6, 4] and state.game_mode == 0x46 and state.mode_stage == 1 and state.mode_step == 0:
			june4_holiday_reached = true
		if [int(rs["year"]), int(rs["month"]), int(rs["day"])] == [95, 6, 4] and state.game_mode == 0x34 and state.mode_stage == 1 and state.mode_step == 0 and int(rs["day_event_index"]) == 16:
			june5_weekday_command_reached = true

		# Match actual_app_observer.gd's 1-frame overlap->continue boundary at tick 3367
		if tick > 3367:
			if expansion_phase == "pressed":
				if tick - expansion_tick >= 2:
					held_mask = 0
					expansion_phase = "waiting_menu"
			elif expansion_phase == "ready":
				if tick - expansion_tick >= 8:
					held_mask = PAD_CONFIRM
					expansion_tick = tick
					expansion_phase = "pressed"
			elif on_controllable_menu:
				expansion_tick = tick
				expansion_phase = "ready"

		if state.game_mode == 0x38 and june1_entered_tick < 0:
			june1_entered_tick = tick
			june1_pre_upload_season_view = HudSeason.view(rs)

		var events: Array[Session.ServiceEvent] = []
		for desc: Dictionary in pending_services:
			events.append(Session.ServiceEvent.new(
				tick,
				Session.Phase.BEFORE_MODE,
				Session.ServiceKind.COMPLETION,
				desc["service"],
				desc["payload"]
			))
		staged_services.clear()
		var month_frame: int = state.frame_count
		var prep_err: String = controller.month_start.prepare_services(state)
		if not prep_err.is_empty():
			step_failure = "prepare_services failed: " + prep_err
			break
		var before_step_hash: String = session.state_hash()
		var input_frame := InputFrame.new(tick, held_mask, prev_held_mask)
		prev_held_mask = held_mask
		var result: Session.FrameResult = session.step(input_frame, events)
		if not result.committed:
			controller.month_start.discard_services(month_frame)
			if not june3_unbound_stop_verified and [int(rs["year"]), int(rs["month"]), int(rs["day"])] == [95, 6, 3] and state.game_mode == 0x72 and result.error.contains("0x72") and session.state_hash() == before_step_hash:
				june3_unbound_stop_verified = true
				june3_stop_tick = tick
				june3_bind_rejection = session.router.bind_mode(0x72, controller._overlay_mode)
				session.router._handlers[0x72] = controller._overlay_mode
				continue
			step_failure = "step failed at tick %d: %s" % [tick, result.error]
			break
		pending_services = staged_services.duplicate(true)
		var pub_err: String = controller.month_start.publish_services(result)
		var disc_err: String = controller.month_start.discard_services(month_frame)
		if not pub_err.is_empty() or not disc_err.is_empty():
			step_failure = "postcommit month service error: %s %s" % [pub_err, disc_err]
			break

		step_count += 1

		if state.game_mode == 0x30 and state.mode_step == 3 and may29_schedule_rng < 0:
			may29_schedule_rng = session.rng.state
			may29_fe_record = rs["day_events"][7].duplicate()
		for raw_audio: Variant in result.audio:
			if raw_audio is Dictionary and raw_audio.get("kind") == Bridge.KIND:
				var op: String = str(raw_audio.get("operation", ""))
				june1_operations.append(op)
				if op == "load":
					june1_upload_season_fade = int(rs["resources"].get(HudSeason.KEY, -1))
					june1_post_upload_season_view = HudSeason.view(rs)
				elif op == "poll":
					june1_polls.append(int(raw_audio.get("status", 0)))
		if june1_entered_tick >= 0 and june1_exited_tick < 0 and state.game_mode == 0x31 and int(rs["day_event_index"]) == 8:
			june1_exited_tick = session.clock.logic_iterations
		if [int(rs["year"]), int(rs["month"]), int(rs["day"])] == [95, 6, 1] and state.game_mode == 0x40:
			june1_thursday_activity_reached = true
		if [int(rs["year"]), int(rs["month"]), int(rs["day"])] == [95, 6, 2] and state.game_mode == 0x40:
			june2_friday_activity_reached = true
		if [int(rs["year"]), int(rs["month"]), int(rs["day"])] == [95, 6, 5] and state.game_mode == 0x40:
			june5_weekday_activity_reached = true

	check(step_failure.is_empty(), "simulation stepped cleanly from 1995-05-28 to 1995-06-05 activity: " + step_failure)
	check(may29_schedule_rng == EXPECTED_JUNE1_SCHEDULE_RNG, "1995-05-29 schedule build RNG matches 2966941936")
	check(may29_fe_record == [0xfe, 0, 0, 0], "1995-05-29 week queue places [0xfe, 0, 0, 0] at index 7 for 1995-06-01")
	check(june1_entered_tick == 3631, "1995-06-01 enters mode 0x38 at tick 3631")
	check(june1_exited_tick == 3637, "1995-06-01 exits mode 0x38 to 0x31 index 8 at tick 3637")
	check(june1_operations == ["load", "poll", "poll", "poll", "start"], "1995-06-01 native audio operations are load, poll, poll, poll, start")
	check(june1_polls == [-1, -1, 1], "1995-06-01 native driver polls are [-1, -1, 1]")
	check(june1_pre_upload_season_view.get("checker_season") == 0 and june1_pre_upload_season_view.get("checker_fade") == 255, "1995-06-01 pre-upload retains spring CLUT (season 0, fade 255)")
	check(june1_upload_season_fade == 0xf0 and june1_post_upload_season_view.get("checker_season") == 1 and june1_post_upload_season_view.get("checker_fade") == 0xf0, "1995-06-01 CALROM promotion sets hud_season_fade = 0xef (advanced to 0xf0 in DRAW) for summer CLUT transition")
	var compositor := HudCompositor.new()
	check(compositor._get_season_checker_tile(1, 0xef).get_data() == compositor._get_season_checker_tile(0, 255).get_data(), "HudCompositor 0xef summer fade starts at spring CLUT")
	check(compositor._get_season_checker_tile(1, 255).get_data() != compositor._get_season_checker_tile(0, 255).get_data(), "HudCompositor 0xff summer fade reaches summer CLUT")
	compositor = null

	var final_native: Dictionary = native_backend.month_start_context()
	check(int(final_native.get("vm_initializations", 0)) == 1, "OriginalAudioEngine VM initialized exactly once across warm transition")
	check(int(final_native.get("gain", 0)) == 86, "OriginalAudioEngine SUMMER sequence 0 gain is 86")
	check(int(final_native.get("raw_reverb_depth", 0)) == 56, "OriginalAudioEngine SUMMER sequence 0 raw_reverb_depth is 56")
	check(bool(final_native.get("started", false)), "OriginalAudioEngine SUMMER sequence 0 is started")
	check(str(final_native.get("profile", "")) == "season.summer.june1", "OriginalAudioEngine profile is season.summer.june1")
	check(int(final_native.get("SE_header_lba", 0)) == 49048 and int(final_native.get("music_header_lba", 0)) == 54503, "OriginalAudioEngine loaded COM_MIX (49048) and SUMMER (54503)")
	check(int(final_native.get("sample_cursor", -1)) == int(initial_native.get("sample_cursor", -2)), "warm transition preserves sample_cursor")
	check(native_sink._active == active_player, "warm transition preserves the exact AudioStreamPlayer instance")
	check(seen_dates == ["95-05-28", "95-05-29", "95-05-30", "95-05-31", "95-06-01", "95-06-02", "95-06-03", "95-06-04", "95-06-05"], "progression visits every calendar date from 1995-05-28 through 1995-06-05")
	check(june1_thursday_activity_reached, "reached Thursday 1995-06-01 June weekday activity (0x40)")
	check(june2_friday_activity_reached, "reached Friday 1995-06-02 June weekday activity (0x40)")
	check(june3_unbound_stop_verified and june3_stop_tick == 3744, "verified transactional rollback at Saturday 1995-06-03 tick 3744 (mode 0x72)")
	check(june3_bind_rejection.contains("0x72"), "ModeRouter.bind_mode rejects unimplemented production route 0x72")
	check(june4_holiday_reached, "reached controllable June Sunday holiday command screen (1995-06-04 0x46/1/0)")
	check(june5_weekday_command_reached, "reached controllable June post-Sunday weekday command screen (0x34/1/0)")
	check(june5_weekday_activity_reached, "confirmed June weekday command and entered 1995-06-05 weekday activity (0x40)")

	var report: Dictionary = {
		"checks": checks,
		"failures": failures,
		"seen_dates": seen_dates,
		"may29_schedule_rng": may29_schedule_rng,
		"may29_fe_record": may29_fe_record,
		"june1_entered_tick": june1_entered_tick,
		"june1_exited_tick": june1_exited_tick,
		"june1_operations": june1_operations,
		"june1_polls": june1_polls,
		"june1_upload_season_fade": june1_upload_season_fade,
		"same_player": native_sink._active == active_player,
		"june1_thursday_activity_reached": june1_thursday_activity_reached,
		"june2_friday_activity_reached": june2_friday_activity_reached,
		"june3_unbound_stop_verified": june3_unbound_stop_verified,
		"june3_stop_tick": june3_stop_tick,
		"june3_bind_rejection": june3_bind_rejection,
		"june4_holiday_reached": june4_holiday_reached,
		"june5_weekday_command_reached": june5_weekday_command_reached,
		"june5_weekday_activity_reached": june5_weekday_activity_reached,
		"initial_native": initial_native,
		"final_native": final_native,
	}
	var out_dir: String = OS.get_environment("TOKIMEMO_CALENDAR_OUTPUT")
	if not out_dir.is_empty():
		var f := FileAccess.open(out_dir.path_join("may28-to-june-weekday.json"), FileAccess.WRITE)
		if f != null:
			f.store_string(JSON.stringify(report, "\t"))
			f.close()

	session.draw_logic = Callable()
	session.cd_logic = Callable()
	session.audio_logic = Callable()
	session.service_completion = Callable()
	session.router._handlers.clear()
	cmd0_resources.clear()
	holiday_resources.clear()
	media_store = null
	actor_service = null
	holiday_controller = null
	controller.month_start.reset_services()
	controller = null
	session = null
	state = null
	native_service.reset_month_start_owner()
	host.queue_free()
	native_service = null
	native_backend = null
	native_sink = null
	await process_frame
	await create_timer(0.2).timeout
	print(JSON.stringify({
		"checks": checks,
		"failures": failures,
		"seen_dates": seen_dates,
		"june1_polls": june1_polls,
		"june1_thursday_activity_reached": june1_thursday_activity_reached,
		"june2_friday_activity_reached": june2_friday_activity_reached,
		"june4_holiday_reached": june4_holiday_reached,
		"june5_weekday_command_reached": june5_weekday_command_reached,
		"june5_weekday_activity_reached": june5_weekday_activity_reached,
	}))
	quit(0 if failures.is_empty() else 1)
