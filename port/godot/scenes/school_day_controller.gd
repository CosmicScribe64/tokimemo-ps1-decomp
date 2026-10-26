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

const TABLE_SHAPES: Dictionary = {"command_deltas": [8, 9], "birthday_offsets": [13],
	"sine_quarter": [1025], "tokimeki_weights": [13], "bonus_param_masks": [13],
	"ladder_kinds": [13], "param_bonus": [13, 8], "ladder_bases": [7],
	"affection_weights": [13], "closeness_grid": [20, 20],
	"command0_rank_threshold": [1], "weekday_background_lbas": [8],
	"weekday_sd_lbas": [25], "sd_select_non_summer": [16], "sd_select_summer": [16]}

var tables: Dictionary = {}
var error: String = ""
var command_capabilities: Array[int] = []
var opening_services: Dictionary = {}

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
	tables = validated_tables.duplicate(true)

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
	for binding: Array in [[0x15, _prologue], [0x34, _command], [0x30, _schedule], [0x31, _day_screen], [0x40, _school_day], [0x32, _week_end]]:
		failure = session.router.bind_mode(binding[0], binding[1])
		if not failure.is_empty():
			return failure
	session.state = state
	session.service_completion = service_completion
	return ""

func attach_multi_week(session: Session, state: State) -> String:
	var failure: String = attach(session, state)
	if not failure.is_empty():
		return failure
	state.rule_state["allow_multi_week"] = true
	for ovl_mode: int in [0x01, 0x35, 0x36, 0x37, 0x3d, 0x3e, 0x3f, 0x44, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x57, 0x58, 0x59, 0x5a, 0x5c, 0x5e]:
		failure = session.router.bind_overlay_mode(ovl_mode, _overlay_mode)
		if not failure.is_empty():
			return failure
	return ""

func _overlay_mode(base: CoreState, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	var mode: int = state.game_mode
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	result.presentation.append({"kind": "overlay_mode", "mode": mode, "year": data["year"], "month": data["month"], "day": data["day"]})
	if mode == 0x5c:
		# Graduation Day Legendary Tree Confession & Ending (src/ovl/ending/graduation_candidate_selection.c)
		var param_vals: Array[int] = []
		for p_word: int in data["params"]:
			param_vals.append(State.high(p_word))
		var eval_girls: Array = []
		for g_idx: int in range(mini(13, data["girls"].size())):
			var g_rec: Dictionary = data["girls"][g_idx]
			eval_girls.append({
				"tokimeki": State.high(g_rec["tokimeki_word"]),
				"affection": State.high(g_rec["affection_word"]),
				"syoushin": State.high(g_rec["syoushin_word"]),
				"met": 1 if (g_rec["flags"] & 2) != 0 else 0,
				"flags": g_rec["flags"],
				"scene_counters": g_rec["counters"],
				"story_stage": int(g_rec.get("story_stage", 0)),
				"rei_subcount": int(g_rec.get("rei_subcount", 0)),
			})
		var health_nibble: int = (data["player_flags"] >> 8) & 0xF
		var scores: Dictionary = EndingOverlay.compute_girl_scores(
			eval_girls, param_vals, param_vals[8] if param_vals.size() > 8 else 0, health_nibble
		)
		var sel: Dictionary = EndingOverlay.select_graduation_candidate(
			eval_girls, scores["points_by_girl"], data["scene_flags"], 0
		)
		var gate: Dictionary = EndingOverlay.evaluate_candidate_flag_gate(int(sel["current_girl"]), eval_girls)
		var best_girl: int = int(gate["redirected_girl"])
		if best_girl == EndingOverlay.GIRL_YOSHIO:
			var best_aff: int = 79
			for g_idx: int in range(11):
				var aff: int = State.high(data["girls"][g_idx]["affection_word"])
				if (data["girls"][g_idx]["flags"] & 2) != 0 and aff > best_aff:
					best_aff = aff
					best_girl = g_idx
		data["ending_completed"] = 1
		data["confessed_girl"] = best_girl
		state.set_stage(0xff)
		return
	if mode == 0x4c:
		# School Trip (SHUGAKU) spans 6 days (Mon..Sat -> Sun)
		for _trip_day: int in range(6):
			Calendar.advance_day(state)
	elif mode == 0x51:
		# Club Camp (RENSYU) advances 1 calendar day per camp record
		Calendar.advance_day(state)
	# Advance queue record after overlay event completes
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
	if input.pressed & 0x20:
		# Reject a reachable unimplemented scheduler branch before confirmation.
		result.error = Scheduler.initial_branch_error(state)
		if not result.error.is_empty():
			return
	Command.step(state, input, result, command_capabilities)

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

func _day_screen(base: CoreState, _input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state != null:
		OrdinaryWeek.dispatch(state, result)

func _week_end(base: CoreState, _input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state != null:
		WeekEnd.step(state, rng, result)

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
		if data["vacation"] != 0 or data["year"] != 95 or data["month"] != 4 or data["day"] < 5 or data["day"] > 8:
			result.error = "school-day route outside initial April5–8 ordinary week"
			return
	var event: Array = data["day_events"][data["day_event_index"]]
	if event[0] != 0 or event[1] not in [0, 1, 2, 3, 4, 5, 6, 7]:
		result.error = "unsupported activity queue record"
		return
	if event[1] in [1, 2, 3, 4, 5, 6] and event[3] != 15:
		result.error = "companion variants for command %d are not supported in this tranche" % event[1]
		return
	if event[1] == 4:
		var club_id: int = (data["player_flags"] >> 12) & 15
		if club_id >= 11:
			result.error = "unsupported club command with no active club (club_id 11)"
			return
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
			if data["scene_work"] in [15, 16]:
				event[2] = 0
			var actors: Array = [_actor(96, event[2], -80, data["sd_anim_fast"])]
			var kind: int = data["calendar"][0]["day_kind"][data["day"]]
			if event[1] != 7 and kind not in [1, 2, 3] and event[3] != 15:
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
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
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
