extends RefCounted
## T-29004 / T-29014: bounded command→real initial scheduler→one school-day result.
## Content/services remain explicit. No fallback table or animation duration.

const State = preload("res://rules/school_day_state.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")
const CoreState = preload("res://core/game_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const Command = preload("res://flow/command_screen.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const Calendar = preload("res://rules/calendar.gd")
const OrdinaryWeek = preload("res://rules/ordinary_week.gd")
const WeekEnd = preload("res://rules/week_end.gd")
const SchoolDay = preload("res://rules/school_day.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")
const MonthStart = preload("res://scenes/month_start_controller.gd")
const HudSession = preload("res://presentation/hud_session_context.gd")

const TABLE_SHAPES: Dictionary = {"command_deltas": [8, 9], "birthday_offsets": [13],
	"sine_quarter": [1025], "tokimeki_weights": [13], "bonus_param_masks": [13],
	"ladder_kinds": [13], "param_bonus": [13, 8], "ladder_bases": [7],
	"affection_weights": [13], "closeness_grid": [20, 20],
	"command0_rank_threshold": [1], "weekday_background_lbas": [8],
	"weekday_sd_lbas": [25], "sd_select_non_summer": [16], "sd_select_summer": [16]}

const OPTIONAL_RANK_BARS := "command_rank_bars"

var tables: Dictionary = {}
var error: String = ""
var command_capabilities: Array[int] = []
var opening_services: Dictionary = {}
var month_start: MonthStart = MonthStart.new()
var overlay_dispatcher: OverlayDispatcher = OverlayDispatcher.new()
## Mutable command-HUD session handed over by flow/fresh_gameplay_handoff.gd.
## When set, every 0x34 stage 1 frame runs its source mirrors (see _command).
var hud_session: HudSession = null
## TypeA icon positions (D_800B5AE4) for the HUD session's hud_icon_bar_init
## at school_day_screen_init; set with hud_session by the fresh handoff.
var hud_icon_positions: Array = []
## T-39007: the ending step built from a successful "ending.clear_save"
## completion, with the request it answers. It lives only between the
## BEFORE_MODE delivery and the overlay step of the same frame, never in the
## serialisable rule state.
var _clear_save_step: Dictionary = {}

func _init(validated_tables: Dictionary) -> void:
	# The caller first validates external-pack provenance against original input.
	# This second gate protects rule operations from missing/malformed arrays.
	for key: String in TABLE_SHAPES:
		if not validated_tables.has(key) or not _has_shape(validated_tables[key], TABLE_SHAPES[key], 0):
			error = "required original table missing or malformed: " + key
			return
		var minimum: int = 0
		var maximum: int = 255
		if key in ["command_deltas", "param_bonus"]:
			minimum = -128
			maximum = 127
		elif key == "birthday_offsets":
			maximum = 65535
		elif key == "sine_quarter":
			minimum = -32768
			maximum = 32767
		elif key == "command0_rank_threshold":
			minimum = -2147483648
			maximum = 2147483647
		elif key in ["weekday_background_lbas", "weekday_sd_lbas"]:
			maximum = 2147483647
		elif key in ["sd_select_non_summer", "sd_select_summer"]:
			maximum = 24
		if not _in_range(validated_tables[key], minimum, maximum):
			error = "original table element exceeds sourced width/index bounds: " + key
			return
	# Optional: packs exported before the full rank-bar row lack it; the
	# scheduler then rejects the commands that need a missing bar.
	if validated_tables.has(OPTIONAL_RANK_BARS):
		if not _has_shape(validated_tables[OPTIONAL_RANK_BARS], [5], 0) or not _in_range(validated_tables[OPTIONAL_RANK_BARS], -2147483648, 2147483647):
			error = "original table malformed: " + OPTIONAL_RANK_BARS
			return
		if validated_tables[OPTIONAL_RANK_BARS][0] != validated_tables["command0_rank_threshold"][0]:
			error = "command_rank_bars[0] differs from command0_rank_threshold at the same address"
			return
	tables = validated_tables.duplicate(true)
	overlay_dispatcher.rule_tables = tables

func _in_range(value: Variant, minimum: int, maximum: int) -> bool:
	if value is Array:
		for item: Variant in value:
			if not _in_range(item, minimum, maximum):
				return false
		return true
	return value >= minimum and value <= maximum

func _has_shape(value: Variant, shape: Array, depth: int) -> bool:
	if depth == shape.size():
		return value is int
	if not value is Array or value.size() != shape[depth]:
		return false
	for item: Variant in value:
		if not _has_shape(item, shape, depth + 1):
			return false
	return true

func attach(session: Session, state: State) -> String:
	if not error.is_empty():
		return error
	var failure: String = state.structural_error()
	if not failure.is_empty():
		return failure
	for binding: Array in [[0x15, _prologue], [0x34, _command], [0x30, _schedule], [0x31, _day_screen], [0x40, _school_day], [0x32, _week_end], [0x38, month_start.step]]:
		failure = session.router.bind_mode(binding[0], binding[1])
		if not failure.is_empty():
			return failure
	for ovl_mode: int in DayEventModes.all_overlay_native_modes():
		if not session.router._handlers.has(ovl_mode):
			session.router._handlers[ovl_mode] = _overlay_mode
	session.state = state
	session.service_completion = service_completion
	return ""

func attach_multi_week(session: Session, state: State) -> String:
	var failure: String = attach(session, state)
	if not failure.is_empty():
		return failure
	state.rule_state["allow_multi_week"] = true
	for ovl_mode: int in DayEventModes.all_overlay_native_modes():
		if not session.router._handlers.has(ovl_mode):
			session.router._handlers[ovl_mode] = _overlay_mode
	return ""

func start_overlay(state: State, mode: int, initial_overlay_state: Dictionary = {}, event: Array = []) -> Dictionary:
	var data: Dictionary = state.rule_state
	var ovl: Dictionary = overlay_dispatcher.init_overlay(mode, data, event)
	for k: String in initial_overlay_state.keys():
		ovl[k] = initial_overlay_state[k]
	data["overlay_state"] = ovl
	if state.game_mode != mode:
		state.reset_game_mode_fields(mode)
	return ovl

func _apply_overlay_deltas(state: State, deltas: Dictionary) -> void:
	if deltas.is_empty():
		return
	var data: Dictionary = state.rule_state
	if deltas.get("state_writes") is Dictionary:
		# Whole saved-state values a source-backed resolver computed (T-39006).
		for key: String in (deltas["state_writes"] as Dictionary).keys():
			data[key] = deltas["state_writes"][key]
	var girl_id: int = int(deltas.get("girl_id", 0))
	if data.get("girls") is Array and girl_id >= 0 and girl_id < (data["girls"] as Array).size():
		var g_rec: Dictionary = data["girls"][girl_id]
		if deltas.has("affection_delta"):
			var cur_aff: int = State.high(int(g_rec.get("affection_word", 0)))
			var next_aff: int = clampi(cur_aff + int(deltas["affection_delta"]), 0, 255)
			g_rec["affection_word"] = State.with_high(int(g_rec.get("affection_word", 0)), next_aff)
			if g_rec.has("affection"):
				g_rec["affection"] = next_aff
		if deltas.has("tokimeki_delta"):
			var cur_tok: int = State.high(int(g_rec.get("tokimeki_word", 0)))
			var next_tok: int = clampi(cur_tok + int(deltas["tokimeki_delta"]), 0, 255)
			g_rec["tokimeki_word"] = State.with_high(int(g_rec.get("tokimeki_word", 0)), next_tok)
			if g_rec.has("tokimeki"):
				g_rec["tokimeki"] = next_tok
		if deltas.has("syoushin_delta"):
			var cur_syou: int = State.high(int(g_rec.get("syoushin_word", 0)))
			var next_syou: int = clampi(cur_syou + int(deltas["syoushin_delta"]), 0, 255)
			g_rec["syoushin_word"] = State.with_high(int(g_rec.get("syoushin_word", 0)), next_syou)
			if g_rec.has("syoushin"):
				g_rec["syoushin"] = next_syou
		if deltas.has("date_count_delta"):
			var dc_add: int = int(deltas["date_count_delta"])
			if g_rec.get("counters") is Array and (g_rec["counters"] as Array).size() > 1:
				g_rec["counters"][1] = int(g_rec["counters"][1]) + dc_add
			if g_rec.get("scene_counters") is Array and (g_rec["scene_counters"] as Array).size() > 1:
				g_rec["scene_counters"][1] = int(g_rec["scene_counters"][1]) + dc_add
			if g_rec.has("date_count"):
				g_rec["date_count"] = int(g_rec["date_count"]) + dc_add

	if deltas.get("param_deltas") is Dictionary and data.get("params") is Array:
		var p_deltas: Dictionary = deltas["param_deltas"]
		var params_arr: Array = data["params"]
		for p_idx_var: Variant in p_deltas.keys():
			var p_idx: int = int(p_idx_var)
			if p_idx >= 0 and p_idx < params_arr.size():
				var cur_p: int = State.high(int(params_arr[p_idx]))
				var next_p: int = clampi(cur_p + int(p_deltas[p_idx_var]), 0, 999)
				params_arr[p_idx] = State.with_high(int(params_arr[p_idx]), next_p)

	if deltas.get("appointment_words") is Array:
		data["appointment_words"] = (deltas["appointment_words"] as Array).duplicate()

	for k: String in [
		"ending_completed", "confessed_girl", "ending_type", "ending_phases", "clear_save_completed", "album_clear_mask", "clear_name_slot",
		"talk_result_code", "rpg_result_code", "loot_rewards",
		"high_scores", "high_score_names", "stages_cleared", "score", "total_score", "menu_hit_3", "system_data_flag",
		"tt_stages_cleared", "tt_last_time_frames", "tt_rankings",
	]:
		if deltas.has(k):
			data[k] = deltas[k]
	if deltas.has("return_payload") and deltas["return_payload"] is Dictionary:
		data["subgame_return_payload"] = (deltas["return_payload"] as Dictionary).duplicate(true)

func _overlay_mode(base: CoreState, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	var ovl_state: Dictionary = data.get("overlay_state", {}) if data.get("overlay_state") is Dictionary else {}
	var mode: int = int(ovl_state.get("mode", state.game_mode))
	var idx: int = clampi(int(data.get("day_event_index", 0)), 0, 31)
	var event: Array = data["day_events"][idx] if data.get("day_events") is Array and idx < (data["day_events"] as Array).size() else [0xff, 0, 0, 0]

	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)

	if ovl_state.is_empty() or int(ovl_state.get("mode", -1)) != mode:
		ovl_state = overlay_dispatcher.init_overlay(mode, data, event, rng)
		data["overlay_state"] = ovl_state
		if not String(ovl_state.get("error", "")).is_empty():
			# Fail closed; the stored state keeps the refusal, so no second draw.
			result.error = ovl_state["error"]
			return
		if input.pressed == 0:
			result.presentation.append({
				"kind": "overlay_mode",
				"mode": mode,
				"dispatcher_mode": ovl_state.get("dispatcher_mode", mode),
				"year": data["year"],
				"month": data["month"],
				"day": data["day"],
				"overlay_state": ovl_state.duplicate(true),
				"completed": false,
			})
			return

	var step_out: Dictionary = {}
	var clear_save_step: Dictionary = _clear_save_step
	_clear_save_step = {}
	if not clear_save_step.is_empty() and clear_save_step["request"] == ovl_state.get("clear_save_request") \
			and ovl_state.get("clear_save_status") == "requested":
		step_out = clear_save_step["step_out"]
	else:
		# GameState.timer_ticks (+0x10F4) is one live word, advanced by the timer
		# interrupt; the port keeps it in the core record. Event rules that read
		# it (culture_visitor_pick's srand) see that word for this call only, so
		# rule_state never holds a stale copy.
		var had_ticks: bool = data.has("timer_ticks")
		var prior_ticks: Variant = data.get("timer_ticks")
		data["timer_ticks"] = state.timer_ticks
		step_out = overlay_dispatcher.step_overlay(mode, ovl_state, input.pressed, data, rng)
		if had_ticks:
			data["timer_ticks"] = prior_ticks
		else:
			data.erase("timer_ticks")
	if not String(step_out.get("error", "")).is_empty():
		# Fail closed: saved state is untouched and the overlay stays where it was.
		result.error = step_out["error"]
		return
	var updated_ovl: Dictionary = step_out.get("state", ovl_state)
	data["overlay_state"] = updated_ovl
	var is_completed: bool = bool(step_out.get("completed", false))

	result.presentation.append({
		"kind": "overlay_mode",
		"mode": mode,
		"dispatcher_mode": step_out.get("dispatcher_mode", mode),
		"year": data["year"],
		"month": data["month"],
		"day": data["day"],
		"overlay_state": updated_ovl.duplicate(true),
		"completed": is_completed,
	})

	if not is_completed:
		return

	_apply_overlay_deltas(state, step_out.get("deltas", {}))
	data["last_completed_overlay"] = updated_ovl.duplicate(true)
	data.erase("overlay_state")

	if mode == 0x5c or mode == 0x63 or int(step_out.get("dispatcher_mode", -1)) == OverlayDispatcher.GAME_MODE_ENDING:
		data["ending_completed"] = 1
		data["confessed_girl"] = int(updated_ovl.get("confessed_girl", EndingOverlay.GIRL_YOSHIO))
		data["ending_type"] = String(updated_ovl.get("ending_type", "yoshio_bad_ending"))
		data["ending_phases"] = updated_ovl.get("history", []).duplicate()
		data["clear_save_completed"] = bool(updated_ovl.get("clear_save_completed", false))
		state.set_stage(0xff)
		return

	var completed_disp_mode: int = int(step_out.get("dispatcher_mode", mode))
	if completed_disp_mode == OverlayDispatcher.GAME_MODE_SHUGAKU:
		# School Trip (SHUGAKU, native 0x51 / dispatcher 0x4C) spans 6 days (Mon..Sat -> Sun)
		for _trip_day: int in range(6):
			Calendar.advance_day(state)
	elif completed_disp_mode == OverlayDispatcher.GAME_MODE_RENSYU:
		# Club Camp (RENSYU, native 0x5F / dispatcher 0x51) advances 1 calendar day per camp record
		Calendar.advance_day(state)

	# Advance queue record only after interactive overlay completes
	if data["day_event_index"] < 31:
		data["day_event_index"] = Math.u8(data["day_event_index"] + 1)
	state.reset_game_mode_fields(0x31)
	data["voice_wait_frames"] = 0
	data["message_title_visible"] = false
	result.transitions.append({"kind": "native_mode", "mode": 0x31, "day_event_index": data["day_event_index"]})

func _prologue(base: CoreState, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var route_state: RouteState = base as RouteState
	if route_state == null or opening_services.is_empty():
		result.error = "prologue mode requires NewGameRouteState and configured opening services"
		return
	var step_out: Session.FrameResult = Arrival.step(route_state, rng, input.pressed, opening_services)
	result.error = step_out.error
	result.audio.append_array(step_out.audio)
	result.presentation.append_array(step_out.presentation)
	result.transitions.append_array(step_out.transitions)

func _state(base: CoreState, result: Session.FrameResult) -> State:
	var state: State = base as State
	if state == null:
		result.error = "school-day controller needs its transactional state subclass"
		return null
	result.error = state.structural_error()
	if not result.error.is_empty():
		return null
	return state

func _command(base: CoreState, input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	if state.mode_stage in [0, 0xff]:
		var route_state: RouteState = base as RouteState
		if route_state == null or opening_services.is_empty():
			if state.rule_state.get("allow_multi_week", false) and state.mode_stage == 0:
				state.next_stage()
			else:
				result.error = "command screen requires captured native stage1/step0"
				return
		else:
			var reload_out: Session.FrameResult = Reload.step(route_state, opening_services)
			result.error = reload_out.error
			result.audio.append_array(reload_out.audio)
			result.presentation.append_array(reload_out.presentation)
			result.transitions.append_array(reload_out.transitions)
			return
	var hud_checkpoint: Dictionary = {}
	if hud_session != null:
		# boot_frame_begin (main_loop.c:88) clears the cursor guard, then
		# settings_command_screen_select_frame (home_screens.c:526-535) runs
		# hud_icon_bar_update(0) and hud_cursor_update before the confirm.
		hud_checkpoint = hud_session.checkpoint()
		var message: String = "HUD session state was modified outside the source mirrors" if hud_checkpoint.is_empty() else hud_session.begin_outer_frame()
		if message.is_empty():
			message = hud_session.command_screen_frame(input.held, input.pressed)
		if not message.is_empty():
			if not hud_checkpoint.is_empty():
				hud_session.restore(hud_checkpoint)
			result.error = message
			return
	if input.pressed & 0x20:
		# Reject a reachable unimplemented scheduler branch before confirmation.
		result.error = Scheduler.initial_branch_error(state)
	if result.error.is_empty():
		Command.step(state, input, result, command_capabilities)
	if hud_session != null:
		# Command.select_cursor and the HUD menu_cursor_update are the same
		# source call on two records; they must agree.
		if result.error.is_empty() and hud_session.menu_cursor() != int(state.rule_state.get("menu_cursor", -1)):
			result.error = "HUD session menu cursor %d differs from the command cursor %s" % [hud_session.menu_cursor(), str(state.rule_state.get("menu_cursor"))]
		if not result.error.is_empty():
			hud_session.restore(hud_checkpoint)

func _schedule(base: CoreState, _input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	if state.mode_stage != 0:
		result.error = "initial scheduler requires native stage0"
		return
	result.error = Scheduler.initial_branch_error(state)
	if not result.error.is_empty():
		return
	match state.mode_step:
		0:
			match state.substep:
				0:
					for index: int in range(11):
						if data["girl_last_date_spot_timer"][index] != 0:
							data["girl_last_date_spot_timer"][index] = Math.u8(data["girl_last_date_spot_timer"][index] - 1)
					data["message_title_visible"] = false
					result.audio.append({"kind": "native_restore_bgm", "bank": 0, "sequence": 0})
					state.substep = 1
				1:
					if data["memory_card_job"] != 0:
						result.error = "memory-card save polling outside captured unsaved route"
						return
					data["memory_card_busy"] = 0
					state.substep = 2
				2: state.next_step()
		1: result.error = Scheduler.build_initial_week(state, tables, rng)
		2: result.error = Scheduler.resolve_initial_encounters(state, tables, rng)
		3: Scheduler.weekly_girl_update(state, tables)
		4:
			result.error = Calendar.advance_initial_april(state)
			if not result.error.is_empty():
				return
			state.reset_game_mode_fields(0x31)
			data["voice_wait_frames"] = 0
			data["message_title_visible"] = false
			result.audio.append({"kind": "native_stop_xa_if_active"})
			result.transitions.append({"kind": "native_mode", "mode": 0x31})
		_: result.error = "unsupported initial scheduler step: %d" % state.mode_step
	# _schedule_init tail (scheduler.c:66-71): parameter_show .. hud_icon_bar_update(0).
	_hud_icon_bar_tail(state, _input, result)

func _day_screen(base: CoreState, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	if data.get("overlay_state") is Dictionary and not (data["overlay_state"] as Dictionary).is_empty():
		_overlay_mode(base, input, rng, result)
		return
	if state.mode_step == 1 and data.get("allow_multi_week", false) and int(data.get("year", 95)) >= 98 and int(data.get("month", 4)) >= 3 and int(data.get("day", 1)) != 0:
		state.reset_game_mode_fields(0x5c)
		data["overlay_state"] = overlay_dispatcher.init_overlay(0x5c, data, [0x21, 0, 0, 0], rng)
		result.transitions.append({"kind": "native_mode", "mode": 0x5c})
		return
	OrdinaryWeek.dispatch(state, result)
	# schedule_day_screen_handler tail (scheduler.c:1534-1540): hud_icon_bar_update(0).
	_hud_icon_bar_tail(state, input, result)
	if result.error.is_empty() and state.game_mode not in [0x30, 0x31, 0x32, 0x34, 0x38, 0x40, 0x46]:
		var idx: int = clampi(int(data.get("day_event_index", 0)), 0, 31)
		var ev: Array = data["day_events"][idx] if data.get("day_events") is Array and idx < (data["day_events"] as Array).size() else [0xff, 0, 0, 0]
		data["overlay_state"] = overlay_dispatcher.init_overlay(state.game_mode, data, ev, rng)

## hud_icon_bar_update after the step in the mode 0x30 and 0x31 handlers. Its
## menu_check / menu_cursor_update(0) (hud_panels.c:1081-1082) runs on the same
## menu as command select: the HUD session mirror and the rule cursor
## (Command.select_cursor, the same source call) advance together and must
## agree, as in _command. Without a HUD session (development routes) nothing
## runs here.
func _hud_icon_bar_tail(state: State, input: InputFrame, result: Session.FrameResult, screen_init: bool = false) -> void:
	if hud_session == null or not result.error.is_empty():
		return
	var checkpoint: Dictionary = hud_session.checkpoint()
	var cursor_before: Variant = state.rule_state.get("menu_cursor")
	# boot_frame_begin (main_loop.c:88) clears the cursor guard every frame.
	var message: String = "HUD session state was modified outside the source mirrors" if checkpoint.is_empty() else hud_session.begin_outer_frame()
	if message.is_empty() and screen_init:
		message = "school_day_screen_init needs the source icon positions" if hud_icon_positions.size() != 14 else hud_session.school_day_screen_init(hud_icon_positions)
	if message.is_empty():
		message = hud_session.hud_icon_bar_update(1 if state.game_mode == 0x40 else 0, input.held, input.pressed)
	if message.is_empty():
		var selection: Session.FrameResult = Session.FrameResult.new()
		Command.select_cursor(state, input, selection)
		message = selection.error
		if message.is_empty():
			result.presentation.append_array(selection.presentation)
	if message.is_empty() and hud_session.menu_cursor() != int(state.rule_state.get("menu_cursor", -1)):
		message = "HUD session menu cursor %d differs from the rule cursor %s" % [hud_session.menu_cursor(), str(state.rule_state.get("menu_cursor"))]
	if not message.is_empty():
		state.rule_state["menu_cursor"] = cursor_before
		if not checkpoint.is_empty():
			hud_session.restore(checkpoint)
		result.error = message

func _week_end(base: CoreState, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state != null:
		# Stage 0xFF runs school_day_week_end_exit, which has its own
		# hud_icon_bar_update(0) (school_day.c:39-47) before the handler's
		# tail (scheduler.c:2258-2264): two menu_cursor_update calls.
		var exit_frame: bool = state.mode_stage == 0xFF
		WeekEnd.step(state, rng, result)
		if exit_frame:
			_hud_icon_bar_tail(state, input, result)
		_hud_icon_bar_tail(state, input, result)

static func _sd_table_index(command: int, extra: int) -> int:
	var variant: int = 1 if extra != 0 else 0
	if command == 3 and extra == 5:
		variant = 2
	match command:
		0: return 0 + variant
		1: return 2 + variant
		2: return 4 + variant
		3: return 6 + variant
		5: return 9 + variant
		6: return 11 + variant
		7: return 13
		_: return -1

const CLUB_BACKGROUND_LBAS: Array[int] = [16215, 16224, 16234, 16244, 16252, 16260, 16168, 16177, 16206, 16197, 16187]
const CLUB_SD_SELECT_NON_SUMMER: Array = [
	[1, 3], [30, 31], [5, 7], [9, 11], [27, 29], [23, 25],
	[12, 13], [14, 15], [20, 21], [18, 19], [16, 17]
]
# club_sd_set_summer (wiki/spec/m-weekday.md); variant-1 column equals SchoolDay.SUMMER_CLUB_SOLO_SD_SETS.
const CLUB_SD_SELECT_SUMMER: Array = [
	[0, 2], [30, 31], [4, 6], [8, 10], [26, 28], [22, 24],
	[12, 13], [14, 15], [20, 21], [18, 19], [16, 17]
]

## Companion seen by the club command: the queued record's extra on school
## days; vacation setup re-runs command_girl_select(4) on saved state instead.
func club_companion(data: Dictionary, event: Array) -> int:
	if int(data.get("vacation", 0)) == 0:
		return int(event[3])
	var selection: Dictionary = Scheduler.command_girl_select(data, 4, tables)
	return -1 if not String(selection["error"]).is_empty() else int(selection["companion"])

func _load_descriptor(state: State, request: String) -> Dictionary:
	var data: Dictionary = state.rule_state
	var event: Array = data["day_events"][data["day_event_index"]]
	var command: int = event[1]
	var month: int = data.get("month", 4)
	var is_summer: bool = month in [6, 7, 8, 9]
	var vacation_kind: int = data.get("vacation", 0)
	var health: int = (data.get("player_flags", 0) >> 8) & 15
	var condition_bucket: int = 1 if health == 3 else 0
	var season_index: int = 1 if is_summer else (0 if month in [3, 4, 5] else (2 if month in [9, 10, 11] else 3))
	var club_id: int = (data.get("player_flags", 0) >> 12) & 15 if command == 4 else 0

	var resolved: Dictionary = SchoolDay.resolve_bg_and_sd(command, club_id, is_summer, condition_bucket, vacation_kind, season_index, 0)
	var bg_lba: int = resolved.get("background_lba", -1)
	var slot: int = resolved.get("sd_set_id", -1)
	var sd_lba: int = resolved.get("sd_lba", -1)
	var sectors: int = resolved.get("sectors", 41)

	if command == 4 and club_id >= 0 and club_id <= 10:
		# club_sd_set_*[club][variant]: variant 0 only for companion 0 (m-weekday,
		# school_day_sd_set_select). Vacation days ask command_girl_select(4) live.
		# _school_day rejects a failed lookup (-1) before any descriptor is built.
		var companion: int = club_companion(data, event)
		var pairs: Array = CLUB_SD_SELECT_SUMMER if is_summer else CLUB_SD_SELECT_NON_SUMMER
		slot = int(pairs[club_id][0 if companion == 0 else 1]) if companion >= 0 else -1
		sd_lba = 27834 + 41 * slot if slot >= 0 else -1

	if vacation_kind == 0 and condition_bucket == 0 and command != 4:
		var bg_table: Array = tables.get("weekday_background_lbas", [])
		if command >= 0 and command < bg_table.size():
			bg_lba = int(bg_table[command])
		var sd_index: int = _sd_table_index(command, int(event[3]))
		var sd_select_key: String = "sd_select_summer" if is_summer else "sd_select_non_summer"
		var sd_select_table: Array = tables.get(sd_select_key, [])
		if sd_index >= 0 and sd_index < sd_select_table.size():
			slot = int(sd_select_table[sd_index])
		var sd_lba_table: Array = tables.get("weekday_sd_lbas", [])
		if slot >= 0 and slot < sd_lba_table.size():
			sd_lba = int(sd_lba_table[slot])

	return {"kind": "school_day_load", "request": request, "command": command,
		"day_event_index": data["day_event_index"],
		"background_lba": bg_lba,
		"sd_set_id": slot, "sd_lba": sd_lba, "sectors": sectors}

func _school_day(base: CoreState, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	var seasonal_allowed: bool = tables.get("allow_seasonal_variants", false) or data.get("allow_seasonal_variants", false) or data.get("allow_multi_week", false)
	if not seasonal_allowed:
		# T-39002: the non-seasonal April variants, April 5..29 1995 (the
		# dispatcher's bound, rules/day_end.gd).
		if data["vacation"] != 0 or data["year"] != 95 or data["month"] != 4 or data["day"] < 5 or data["day"] > 29:
			result.error = "school-day route outside the April5–29 ordinary weeks"
			return
	var event: Array = data["day_events"][data["day_event_index"]]
	if event[0] != 0 or event[1] not in [0, 1, 2, 3, 4, 5, 6, 7]:
		result.error = "unsupported activity queue record"
		return
	# extra is schedule_command_girl_select's result: a heroine 0..10 or 0xF.
	if int(event[3]) != 15 and (int(event[3]) < 0 or int(event[3]) > 10):
		result.error = "school-day companion %d is not a command_girl_select result" % int(event[3])
		return
	if event[1] == 4:
		var club_id: int = (data["player_flags"] >> 12) & 15
		if int(data.get("vacation", 0)) != 0 and club_companion(data, event) < 0:
			result.error = "vacation club command needs command_girl_select(4) rank inputs"
			return
		if club_id >= 11:
			result.error = "unsupported club command with no active club (club_id 11)"
			return
	# week_day (school_day.c:49-75): the stage handler, then the HUD set with
	# hud_icon_bar_update(1); school_day_screen_init substep 0 also rebuilds
	# the icon bar and the cursor actor.
	var screen_init: bool = state.mode_stage == 0 and state.mode_step == 0 and state.substep == 0 and int(data.get("vacation", 0)) == 0
	match state.mode_stage:
		0: _setup(state, result)
		1:
			if state.mode_step != 0:
				result.error = "unsupported school-day main step"
				return
			var actor: Dictionary = data["actor_state"].get("96", {})
			var award: bool = (actor.get("flags", 1) & 1) == 0 and Math.s16(actor.get("sequence", {}).get("counter", -1)) == 0
			SchoolDay.main_step(state, tables, result)
			if award and result.error.is_empty():
				result.error = OrdinaryWeek.snapshot_parameters(state)
		2:
			if state.mode_step != 0:
				result.error = "unsupported school-day exit step"
				return
			SchoolDay.exit_step(state, result)
		_: result.error = "unsupported school-day stage: %d" % state.mode_stage
	_hud_icon_bar_tail(state, _input, result, screen_init)

func _setup(state: State, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	if (state.mode_step == 0 and state.substep not in [0, 1, 2]) or (state.mode_step == 1 and state.substep not in [0, 1]):
		result.error = "unsupported school-day setup substep"
		return
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	data["day_event_done_bits"] = 0
	data["day_event_count_today"] = 0
	match state.mode_step:
		0:
			match state.substep:
				0:
					# dec_bg_cd_read reads bg_lba_cache[0..1] and dec_bg_show[0].
					for field: String in ["background_cache", "background_show"]:
						var unsourced: String = State.unsourced_error(data, field)
						if not unsourced.is_empty():
							result.error = unsourced
							return
					var descriptor: Dictionary = _load_descriptor(state, "background")
					result.error = OrdinaryWeek.snapshot_parameters(state)
					if not result.error.is_empty():
						return
					data["actor_state"].clear()
					var cached_slot: int = data["background_cache"].slice(0, 2).find(descriptor["background_lba"])
					if cached_slot >= 0:
						data["background_show"] = cached_slot
						state.substep = 2
						result.presentation.append({"kind": "school_day_cached_background", "lba": descriptor["background_lba"], "slot": cached_slot})
					else:
						data["background_cache"][1 - data["background_show"]] = descriptor["background_lba"]
						data["resources"]["background"] = {"ready": false, "lba": descriptor["background_lba"]}
						result.presentation.append(descriptor)
						state.substep = 1
				1:
					var background: Dictionary = data["resources"].get("background", {})
					if background.get("ready", false):
						if background.get("decode_result", -1) == -1:
							background["ready"] = false
							data["background_cache"] = [0, background["lba"], 0, 0]
							data["background_show"] = 0
							result.presentation.append({"kind": "native_background_reset"})
							result.presentation.append(_load_descriptor(state, "background"))
						else:
							data["background_show"] = 1 - data["background_show"]
							state.substep = 2
				2: state.next_step()
		1:
			if state.substep == 0:
				var descriptor: Dictionary = _load_descriptor(state, "sd")
				data["scene_work"] = descriptor["sd_set_id"]
				data["resources"]["sd"] = {"ready": false, "lba": descriptor["sd_lba"]}
				result.presentation.append(descriptor)
				state.substep = 1
			elif data["resources"].get("sd", {}).get("ready", false):
				if data["resources"]["sd"].get("checksum_ok", false):
					state.next_step()
				else:
					state.substep = 0
		2:
			var event: Array = data["day_events"][data["day_event_index"]]
			# Rest ids 15/16 are MAIN_SD ids; club ids 15/16 (soccer, basketball) keep the poor flag.
			if event[1] != 4 and data["scene_work"] in [15, 16]:
				event[2] = 0
			var actors: Array = [_actor(96, event[2], -80, data["sd_anim_fast"])]
			var kind: int = data["calendar"][0]["day_kind"][data["day"]]
			var companion_actor: bool = event[1] != 7 and kind not in [1, 2, 3] and event[3] != 15
			if int(data.get("vacation", 0)) != 0:
				# school_day_vacation_sd_actors_init: actor B only for the club command, live lookup.
				companion_actor = event[1] == 4 and club_companion(data, event) != 15
			if companion_actor:
				actors.append(_actor(97, event[2] + 2, -24, data["sd_anim_fast"]))
			data["actor_state"] = {}
			for actor: Dictionary in actors:
				data["actor_state"][str(actor["id"])] = {"id": actor["id"], "sequence_slot": actor["sequence_slot"],
					"x": actor["x"], "y": actor["y"], "flags": 5, "draw_flags": 0x84, "quad_flags": 0x01000000,
					"sequence": {"frame": 0, "counter": 0, "active": true, "period": actor["blink_period"], "last_iteration": -1}}
			result.presentation.append({"kind": "school_day_actors", "actors": actors, "sd_set_id": data["scene_work"]})
			state.next_stage()
		_: result.error = "unsupported school-day setup step"

func _actor(ident: int, slot: int, x: int, fast: int) -> Dictionary:
	return {"id": ident, "sequence_slot": slot, "x": x, "y": -64,
		"flags": 5, "draw_flags": 0x84, "quad_flags": 0x01000000,
		"blink_period": 0x82 if fast != 0 else 1, "unk_01": 1, "unk_05": 2, "unk_43": 0x10}

func service_completion(event: Session.ServiceEvent, base: CoreState, _rng: Rng, result: Session.FrameResult) -> void:
	if event.service == MonthStart.SERVICE:
		month_start.service_completion(event, base, _rng, result)
		return
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	if event.service == "ending.clear_save":
		# T-39007: the app host answers a clear-save request after its storage
		# transaction; only this completion can mark the clear as saved. A
		# success finishes the ending in this frame's overlay step; a failure
		# is stored as the failure notice.
		if not data.get("overlay_state") is Dictionary:
			result.error = "clear-save completion arrived outside the ending"
			return
		var ovl: Dictionary = data["overlay_state"]
		var step_out: Dictionary = OverlayDispatcher.clear_save_step_out(int(ovl.get("mode", state.game_mode)), ovl, event.payload)
		if not step_out["error"].is_empty():
			result.error = step_out["error"]
			return
		if step_out["completed"]:
			_clear_save_step = {"request": (ovl["clear_save_request"] as Dictionary).duplicate(), "step_out": step_out}
		else:
			data["overlay_state"] = step_out["state"]
		return
	if event.service == "school_day.actor96":
		if state.game_mode != 0x40 or state.mode_stage != 1:
			result.error = "actor96 completion arrived outside its native main-stage boundary"
			return
		if not data["actor_state"].has("96") or event.payload.get("active", true) != false or not event.payload.get("counter") is int:
			result.error = "native actor96 completion requires sourced inactive flag/counter"
			return
		var actor: Dictionary = data["actor_state"]["96"]
		if event.payload.get("sequence_slot", -1) != actor["sequence_slot"] or event.payload["counter"] != 0:
			result.error = "actor96 terminal counter/slot differs from flags5 source path"
			return
		if (actor["flags"] & 1) != 0:
			actor["flags"] &= ~5
			actor["sequence"]["active"] = false
			actor["sequence"]["counter"] = event.payload["counter"]
	elif event.service == "school_day.resources":
		var request: String = event.payload.get("request", "")
		if request not in ["background", "sd"] or not data["resources"].has(request):
			result.error = "resource completion has no matching native request"
			return
		var expected_step: int = 0 if request == "background" else 1
		if state.game_mode != 0x40 or state.mode_stage != 0 or state.mode_step != expected_step or state.substep != 1:
			result.error = "resource completion arrived outside the pending native wait"
			return
		if event.payload.get("lba", -1) != data["resources"][request]["lba"] or not event.payload.get("ready") is bool:
			result.error = "resource completion identity/readiness differs from native request"
			return
		if request == "background" and event.payload.get("decode_result", -2) not in [-1, 0, 1]:
			result.error = "background completion needs native decode result"
			return
		if request == "sd" and not event.payload.get("checksum_ok") is bool:
			result.error = "SD completion needs checksum result"
			return
		data["resources"][request].merge(event.payload, true)
	else:
		result.error = "unsupported required service completion: " + event.service
