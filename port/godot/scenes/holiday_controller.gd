extends RefCounted
## T-29013. Additive captured Sunday controller + integrated Holiday sub-screens:
## - 0x10 Phone Book (holiday.c::holiday_tel)
## - 0x20 Magazine (holiday.c::holiday_mode_stage_magazine)
## - 0x30 Calendar (home_screens.c::settings_command_screen_mode_stage_calendar)
## - 0xFE Date Appointment -> 0x50 Date Arrival & 3-Choice Prompt (holiday.c, entry.c, scene_library.c)

const State = preload("res://rules/school_day_state.gd")
const Core = preload("res://core/game_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Rules = preload("res://rules/holiday.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")
const Screen = preload("res://flow/holiday_screen.gd")
const Command = preload("res://flow/command_screen.gd")
const Legacy = preload("res://scenes/school_day_controller.gd")
const CalendarPhoneRules = preload("res://rules/calendar_phone_screens.gd")
const AppointmentRules = preload("res://rules/holiday_appointment.gd")

const SHAPES: Dictionary = {"seasonal_room_background_lbas":4,"weekday_room_background_lbas":4,
	"holiday_activity_background_lbas":8,"holiday_sd_non_summer":8,"holiday_sd_summer":8,"holiday_sd_lbas":13}
var tables: Dictionary = {}
var common: Dictionary = {}
var error: String = ""
var enable_subscreens: bool = false
var _legacy_command: Callable
var _legacy_day: Callable
var _legacy_completion: Callable

func _init(common_tables: Dictionary, holiday_tables: Dictionary) -> void:
	var validator: Legacy = Legacy.new(common_tables)
	error=validator.error
	if not error.is_empty():return
	for key: String in SHAPES:
		if not holiday_tables.get(key) is Array or holiday_tables[key].size()!=SHAPES[key]:
			error="required original holiday table missing or malformed: "+key
			return
		for value: Variant in holiday_tables[key]:
			var maximum: int=12 if key in ["holiday_sd_non_summer","holiday_sd_summer"] else 2147483647
			if not value is int or value<0 or value>maximum:
				error="holiday table width/selector exceeds native bound: "+key
				return
	common=validator.tables.duplicate(true)
	tables=holiday_tables.duplicate(true)

func attach(session: Session, state: State) -> String:
	if not error.is_empty():return error
	if not session.router.missing_handler(0x34).is_empty() or not session.router.missing_handler(0x31).is_empty():return "attach the source school-day controller before holiday extension"
	_legacy_command=session.router.handler(0x34)
	_legacy_day=session.router.handler(0x31)
	_legacy_completion=session.service_completion
	var failure: String=session.router.bind_mode(0x46,_holiday)
	if not failure.is_empty():return failure
	# Appointment/date previews do not register a native DATE capability.
	failure=session.router.bind_mode(0x34,_command)
	if not failure.is_empty():return failure
	failure=session.router.bind_mode(0x31,_day_screen)
	if not failure.is_empty():return failure
	session.state=state
	session.service_completion=service_completion
	return ""

func _day_screen(base: Core, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	_legacy_day.call(base,input,rng,result)
	if not result.error.is_empty():return
	var state: State=base as State
	if state==null:return
	var data: Dictionary=state.rule_state
	if state.game_mode==0x46 or (state.game_mode==0x34 and [data["year"],data["month"],data["day"],data["day_event_index"]]==[95,4,9,10]):
		# This validation is still inside the same session transaction as the
		# dispatcher/calendar change, so a missing extension rolls that back too.
		result.error=Rules.validate(state, enable_subscreens)
		if result.error.is_empty() and not data["resources"].get("available_holiday_commands",[]).has(0):
			result.error="required original holiday/next-command route capability is unavailable"

func _state(base: Core, result: Session.FrameResult) -> State:
	var state: State=base as State
	if state==null:
		result.error="holiday controller requires transactional school-day state"
		return null
	result.error=Rules.validate(state, enable_subscreens)
	return state if result.error.is_empty() else null

func _holiday(base: Core, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State=_state(base,result)
	if state==null:return
	var data: Dictionary=state.rule_state
	if data["day_events"][data["day_event_index"]][0] not in [0x10, 0x80]:
		result.error="holiday mode requires the actual Sunday queue record"
		return
	if not data["resources"].get("available_holiday_commands",[]).has(0):
		result.error="mandatory original holiday room/menu/calendar/activity capabilities are unavailable"
		return
	var entry_stage: int=state.mode_stage
	match state.mode_stage:
		0:
			if state.mode_step==0:
				data["holiday"]["dial_pos"]=0
				for index: int in range(7):data["holiday"]["dial_keys"][index]=255
				Rules.icons(state,false,enable_subscreens)
				if data["icon_enabled"][9] != 0:
					data["menu_cursor"] = 9
				data["actor_state"].clear()
				result.error=Ordinary.snapshot_parameters(state)
				if not result.error.is_empty():return
				result.presentation.append({"kind":"holiday_screen_init","screen":"holiday","prompt":"holiday_open_init/default","icon_count":14})
				if _background(state,"room",tables["seasonal_room_background_lbas"][0],result):state.next_stage()
				else:state.next_step()
			elif state.mode_step==1:
				if _background_ready(state,result):state.next_stage()
			elif state.mode_step==2:
				_restore_holiday_hud(state,result)
			else:result.error="unsupported holiday open step"
		1:
			if state.mode_step!=0:
				result.error="unsupported holiday menu step"
				return
			Screen.step(state,input,common,rng,result,enable_subscreens)
			if result.error.is_empty() and state.mode_stage in [0x10, 0x20, 0x30, 0xFE]:
				_init_subscreen_on_enter(state, rng, result)
		5:
			state.mode_step_timer=Math.add32(state.mode_step_timer,1)
			match state.mode_step:
				0:_load(state,result)
				1:
					var descriptor: Dictionary=Rules.actor(state)
					data["actor_state"]={"96":{"id":96,"flags":5,"sequence_slot":descriptor["sequence_slot"],"sequence":{"counter":0,"active":true}}}
					result.presentation.append({"kind":"holiday_actors","actors":[descriptor],"sd_set_id":data["scene_work"]})
					state.next_step()
				2:Rules.play(state,common,result)
				_:result.error="unsupported holiday activity step"
		0x10:
			_step_subscreen_phone(state, input, result)
		0x20:
			_step_subscreen_magazine(state, input, result)
		0x30:
			_step_subscreen_calendar(state, input, result)
		0xFE:
			_step_subscreen_appointment(state, input, rng, result)
		_:result.error="reachable holiday club/telephone/calendar/options branch is unimplemented"
	# Source open/activity wrappers update the icon HUD after their operation,
	# including the frame that exits to mode31. Menu and sub-screens handle their own input.
	if result.error.is_empty() and entry_stage in [0, 5]:
		Command.select_cursor(state,input,result)

func _restore_holiday_hud(state: State, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	Rules.icons(state, false, enable_subscreens)
	data["actor_state"].clear()
	var room_lba: int = int(tables["seasonal_room_background_lbas"][0])
	var slot: int = data["background_cache"].slice(0, 2).find(room_lba)
	if slot >= 0:
		data["background_show"] = slot
		data["holiday"]["background_brightness"] = 128
	state.mode_stage = 1
	state.mode_step = 0
	state.substep = 0
	state.mode_step_timer = 0
	result.presentation.append({"kind":"holiday_screen_init","screen":"holiday","prompt":"holiday_open_init/default","icon_count":14})

func _init_subscreen_on_enter(state: State, rng: Rng, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	var neutral := InputFrame.new(0, 0, 0)
	match state.mode_stage:
		0x10:
			data["phone_state"] = {"cursor": 0, "substep": 0}
			CalendarPhoneRules.step_holiday_tel(state, neutral, result, data["phone_state"])
		0x20:
			data["mag_state"] = {"substep": 0, "page": 0}
			CalendarPhoneRules.step_holiday_magazine(state, neutral, result, data["mag_state"])
		0x30:
			data["calendar_month_offset"] = 0
			CalendarPhoneRules.step_calendar(state, neutral, result)
		0xFE:
			_init_appointment_flow(state, rng, result)

func _step_subscreen_phone(state: State, input: InputFrame, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	if not data.get("phone_state") is Dictionary:
		data["phone_state"] = {"cursor": 0, "substep": 0}
	CalendarPhoneRules.step_holiday_tel(state, input, result, data["phone_state"])
	if state.game_mode == 0x46 and state.mode_stage == 0:
		_restore_holiday_hud(state, result)

func _step_subscreen_magazine(state: State, input: InputFrame, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	if not data.get("mag_state") is Dictionary:
		data["mag_state"] = {"substep": 0, "page": 0}
	CalendarPhoneRules.step_holiday_magazine(state, input, result, data["mag_state"])
	if state.mode_step == 2:
		CalendarPhoneRules.step_holiday_magazine(state, InputFrame.new(input.tick, 0, 0), result, data["mag_state"])
	if state.game_mode == 0x46 and state.mode_stage == 0:
		_restore_holiday_hud(state, result)

func _step_subscreen_calendar(state: State, input: InputFrame, result: Session.FrameResult) -> void:
	CalendarPhoneRules.step_calendar(state, input, result)
	if state.mode_step == 2:
		CalendarPhoneRules.step_calendar(state, InputFrame.new(input.tick, 0, 0), result)
	if state.game_mode == 0x46 and state.mode_stage == 0:
		_restore_holiday_hud(state, result)

func _build_holiday_state(state: State) -> AppointmentRules.HolidayState:
	var data: Dictionary = state.rule_state
	var hs := AppointmentRules.HolidayState.new()
	hs.year = int(data.get("year", 95))
	hs.month = int(data.get("month", 4))
	hs.day = int(data.get("day", 9))
	hs.weekday = int(data.get("weekday", 6))
	hs.day_kind = int(data["calendar"][0]["day_kind"][hs.day]) if data.get("calendar") is Array and not data["calendar"].is_empty() else 1
	hs.parameters = []
	for word: int in data.get("params", []):
		hs.parameters.append(word >> 16)
	if hs.parameters.size() != 9:
		hs.parameters = [97, 43, 40, 40, 38, 33, 59, 4, 3]
	hs.frame_count = state.frame_count
	hs.game_mode = state.game_mode
	hs.icon_enabled.assign(data["icon_enabled"].slice(0, 14))
	hs.menu_cursor = int(data.get("menu_cursor", 0))
	hs.last_command = int(data.get("last_command", 0))
	hs.mode_stage = state.mode_stage
	hs.stage_step = state.mode_step
	hs.substep = state.substep

	var saved: Dictionary = data.get("appointment_flow", {})
	hs.chosen_place = int(saved.get("chosen_place", 0xFF))
	hs.appointment_place_1 = int(saved.get("appointment_place_1", 0xFF))
	hs.appointment_place_2 = int(saved.get("appointment_place_2", 0xFF))
	hs.appointment_place_3 = int(saved.get("appointment_place_3", 0xFF))
	hs.date_stage = int(saved.get("date_stage", 0))
	hs.date_step = int(saved.get("date_step", 0))
	hs.date_substep = int(saved.get("date_substep", 0))
	hs.date_choice_cursor = int(saved.get("date_choice_cursor", 0))
	hs.date_chosen_option = int(saved.get("date_chosen_option", -1))
	hs.date_score = int(saved.get("date_score", 0))
	hs.date_score_class = int(saved.get("date_score_class", 0))
	hs.date_line_wait = int(saved.get("date_line_wait", 1))
	return hs

func _save_holiday_state(state: State, hs: AppointmentRules.HolidayState, extra: Dictionary = {}) -> void:
	var data: Dictionary = state.rule_state
	var saved: Dictionary = data.get("appointment_flow", {}).duplicate(true)
	saved["chosen_place"] = hs.chosen_place
	saved["appointment_place_1"] = hs.appointment_place_1
	saved["appointment_place_2"] = hs.appointment_place_2
	saved["appointment_place_3"] = hs.appointment_place_3
	saved["date_stage"] = hs.date_stage
	saved["date_step"] = hs.date_step
	saved["date_substep"] = hs.date_substep
	saved["date_choice_cursor"] = hs.date_choice_cursor
	saved["date_chosen_option"] = hs.date_chosen_option
	saved["date_score"] = hs.date_score
	saved["date_score_class"] = hs.date_score_class
	saved["date_line_wait"] = hs.date_line_wait
	for k: String in extra:
		saved[k] = extra[k]
	data["appointment_flow"] = saved

func _init_appointment_flow(state: State, rng: Rng, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	var hs := _build_holiday_state(state)
	var step_res := AppointmentRules.StepResult.new()
	var start_info := AppointmentRules.appointment_start(hs, data["appointment_words"], step_res)
	var extra := {"place_cursor": 0, "start_info": start_info}
	if hs.stage_step == 1:
		# Multi-place appointment: advance substep 0 -> 1 -> 2 to build the place choice menu
		AppointmentRules.appointment_place_choose(hs, data["appointment_words"], 0, rng, false, step_res)
		var choose_s1 := AppointmentRules.appointment_place_choose(hs, data["appointment_words"], 0, rng, false, step_res)
		extra["menu_data"] = choose_s1.get("menu", {})
	elif hs.stage_step == 2:
		# Single-place appointment: advance substep 0 -> 1 -> 2 to build the confirmation presentation
		AppointmentRules.appointment_place_confirm(hs, data["appointment_words"], false, step_res)
		var confirm_s1 := AppointmentRules.appointment_place_confirm(hs, data["appointment_words"], false, step_res)
		extra["confirm_data"] = confirm_s1.get("presentation", {})
	state.mode_step = hs.stage_step
	state.substep = hs.substep
	_save_holiday_state(state, hs, extra)
	result.presentation.append({
		"kind": "appointment_init",
		"stage_step": state.mode_step,
		"distinct_place_count": int(start_info.get("distinct_place_count", 1)),
	})

func _step_subscreen_appointment(state: State, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	if not data.get("appointment_flow") is Dictionary:
		_init_appointment_flow(state, rng, result)
	var hs := _build_holiday_state(state)
	var saved: Dictionary = data["appointment_flow"]
	var step_res := AppointmentRules.StepResult.new()

	match state.mode_step:
		1:
			# Place choice menu (2 or 3 places)
			var menu_data: Dictionary = saved.get("menu_data", {})
			var count: int = maxi(int(menu_data.get("count", 2)), 1)
			var place_cursor: int = int(saved.get("place_cursor", 0))
			if (input.pressed & 0x1000) != 0: # UP
				place_cursor = (place_cursor + count - 1) % count
				saved["place_cursor"] = place_cursor
				result.audio.append({"kind": "native_audio_command", "command": 0x509})
				result.presentation.append({"kind": "appointment_place_cursor", "cursor": place_cursor})
			elif (input.pressed & 0x4000) != 0: # DOWN
				place_cursor = (place_cursor + 1) % count
				saved["place_cursor"] = place_cursor
				result.audio.append({"kind": "native_audio_command", "command": 0x509})
				result.presentation.append({"kind": "appointment_place_cursor", "cursor": place_cursor})
			if (input.pressed & 0x20) != 0: # PAD_CONFIRM (Circle)
				AppointmentRules.appointment_place_choose(hs, data["appointment_words"], input.pressed, rng, false, step_res, place_cursor)
				# Build confirm presentation right away for stage_step == 2
				AppointmentRules.appointment_place_confirm(hs, data["appointment_words"], false, step_res)
				var confirm_s1 := AppointmentRules.appointment_place_confirm(hs, data["appointment_words"], false, step_res)
				state.mode_step = hs.stage_step
				state.substep = hs.substep
				_save_holiday_state(state, hs, {"confirm_data": confirm_s1.get("presentation", {}), "place_cursor": place_cursor})
				result.audio.append({"kind": "native_audio_command", "command": 0x50A})
				result.presentation.append({"kind": "appointment_place_confirmed", "chosen_place": hs.chosen_place})
		2:
			# Confirm presentation -> launch GAME_MODE_DATE (0x50) on PAD_CONFIRM (0x20)
			if (input.pressed & 0x20) != 0:
				AppointmentRules.appointment_place_confirm(hs, data["appointment_words"], false, step_res, true)
				var launch_payload := AppointmentRules.appointment_launch(hs, data["appointment_words"], step_res, data["day_event_index"])
				data["day_events"][data["day_event_index"]] = launch_payload["day_event"]
				state.reset_game_mode_fields(AppointmentRules.GAME_MODE_DATE)
				state.mode_stage = 0
				state.mode_step = 0
				state.substep = 0
				var prompt_data := AppointmentRules.build_date_three_choice_prompt(hs.chosen_place, hs.date_choice_order)
				_save_holiday_state(state, hs, {
					"launch_payload": launch_payload,
					"prompt_data": prompt_data,
				})
				result.transitions.append({
					"kind": "native_mode",
					"mode": AppointmentRules.GAME_MODE_DATE,
					"chosen_place": hs.chosen_place,
					"girl": int(launch_payload["current_girl"]),
				})
				result.presentation.append({
					"kind": "date_arrival_init",
					"launch_payload": launch_payload,
				})

func _date_mode(base: Core, input: InputFrame, _rng: Rng, result: Session.FrameResult) -> void:
	var state: State = _state(base, result)
	if state == null:
		return
	var data: Dictionary = state.rule_state
	var hs := _build_holiday_state(state)
	var saved: Dictionary = data.get("appointment_flow", {})
	var step_res := AppointmentRules.StepResult.new()

	match state.mode_stage:
		0:
			# Date Arrival greeting (date_mode_stage_arrival, src/ovl/date/entry.c:695)
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			if (input.pressed & 0x20) != 0: # PAD_CONFIRM advances to 3-choice prompt
				state.mode_stage = 1
				state.mode_step = 0
				state.substep = 0
				state.mode_step_timer = 0
				hs.date_stage = 1
				hs.date_step = 0
				hs.date_choice_cursor = 0
				var prompt_data := AppointmentRules.build_date_three_choice_prompt(hs.chosen_place, hs.date_choice_order)
				_save_holiday_state(state, hs, {"prompt_data": prompt_data})
				result.audio.append({"kind": "native_audio_command", "command": 0x50A})
				result.presentation.append({
					"kind": "date_three_choice_init",
					"prompt": prompt_data,
				})
		1:
			match state.mode_step:
				0:
					# 3-Choice Date Menu (normal_date_three_select, src/main/scene_library.c:2196-2253)
					var choice_out := AppointmentRules.step_date_three_choice(hs, input.pressed, step_res)
					if bool(choice_out.get("confirmed", false)):
						state.mode_step = 1
						state.substep = 0
						state.mode_step_timer = 0
						_save_holiday_state(state, hs, {
							"response_data": choice_out["response"],
							"selected_row": int(choice_out["selected_row"]),
						})
						result.audio.append({"kind": "native_audio_command", "command": 0x50A})
						result.presentation.append({
							"kind": "date_choice_response",
							"choice_id": int(choice_out["choice_id"]),
							"score_class": int(choice_out["score_class"]),
							"date_score": int(choice_out["date_score"]),
							"response": choice_out["response"],
						})
					else:
						_save_holiday_state(state, hs)
						if (input.pressed & (0x1000 | 0x4000)) != 0:
							result.audio.append({"kind": "native_audio_command", "command": 0x509})
							result.presentation.append({
								"kind": "date_choice_cursor",
								"cursor": hs.date_choice_cursor,
							})
				1:
					# Shiori's response (talk_speak_part case 0x2, src/main/scene_library.c:1604-1658)
					state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
					if (input.pressed & 0x20) != 0:
						state.mode_step = 2
						state.substep = 0
						state.mode_step_timer = 0
						hs.date_step = 2
						_save_holiday_state(state, hs)
						result.audio.append({"kind": "native_audio_command", "command": 0x50A})
						result.presentation.append({"kind": "date_choice_reaction_complete"})
				_:
					state.mode_step_timer = Math.add32(state.mode_step_timer, 1)

func handles_subscreen(state: State) -> bool:
	if state == null:
		return false
	if state.game_mode == AppointmentRules.GAME_MODE_DATE:
		return true
	if state.game_mode == 0x46 and state.mode_stage in [0x10, 0x20, 0x30, 0xFE]:
		return true
	return false

func render_active_subscreen(state: State, calendar_phone_presenter: RefCounted, date_appointment_presenter: RefCounted) -> Dictionary:
	var data: Dictionary = state.rule_state
	if state.game_mode == 0x46:
		match state.mode_stage:
			0x10:
				if calendar_phone_presenter == null:
					return {"error": "calendar_phone_presenter is not open"}
				var ps: Dictionary = data.get("phone_state", {})
				var cursor: int = int(ps.get("cursor", 0))
				var substep: int = int(ps.get("substep", 0))
				var empty_unlocked: Array[int] = []
				var img: Image = calendar_phone_presenter.render_holiday_phone_screen(cursor, empty_unlocked, substep)
				return {
					"error": "",
					"id": "holiday_phone_book",
					"image": img,
					"commands": [{"id": "holiday_phone_book:%d" % cursor, "image": img, "position": Vector2i.ZERO, "blend_mode": 0}],
				}
			0x20:
				if calendar_phone_presenter == null:
					return {"error": "calendar_phone_presenter is not open"}
				var ms: Dictionary = data.get("mag_state", {})
				var substep: int = int(ms.get("substep", 0))
				var img: Image = calendar_phone_presenter.render_holiday_magazine_screen(substep)
				return {
					"error": "",
					"id": "holiday_magazine",
					"image": img,
					"commands": [{"id": "holiday_magazine:%d" % substep, "image": img, "position": Vector2i.ZERO, "blend_mode": 0}],
				}
			0x30:
				if calendar_phone_presenter == null:
					return {"error": "calendar_phone_presenter is not open"}
				var month_offset: int = int(data.get("calendar_month_offset", 0))
				var img: Image = calendar_phone_presenter.render_calendar_screen(5, 0x19B, state.frame_count)
				return {
					"error": "",
					"id": "holiday_calendar",
					"image": img,
					"commands": [{"id": "holiday_calendar:%d" % month_offset, "image": img, "position": Vector2i.ZERO, "blend_mode": 0}],
				}
			0xFE:
				if date_appointment_presenter == null:
					return {"error": "date_appointment_presenter is not open"}
				var hs := _build_holiday_state(state)
				var saved: Dictionary = data.get("appointment_flow", {})
				if state.mode_step == 1:
					var menu_data: Dictionary = saved.get("menu_data", {})
					var place_cursor: int = int(saved.get("place_cursor", 0))
					var out: Dictionary = date_appointment_presenter.render_appointment_place_choice(hs, menu_data, place_cursor)
					out["error"] = ""
					return out
				var confirm_data: Dictionary = saved.get("confirm_data", {})
				if confirm_data.is_empty():
					confirm_data = AppointmentRules.build_appointment_confirm_Presentation(hs, data["appointment_words"])
				var out2: Dictionary = date_appointment_presenter.render_appointment_place_confirm(hs, confirm_data)
				out2["error"] = ""
				return out2
	elif state.game_mode == AppointmentRules.GAME_MODE_DATE:
		if date_appointment_presenter == null:
			return {"error": "date_appointment_presenter is not open"}
		var hs := _build_holiday_state(state)
		var saved: Dictionary = data.get("appointment_flow", {})
		if state.mode_stage == 0:
			var launch_payload: Dictionary = saved.get("launch_payload", {"chosen_place": hs.chosen_place if hs.chosen_place != 0xFF else 0})
			var out: Dictionary = date_appointment_presenter.render_date_arrival_scene(hs, launch_payload)
			out["error"] = ""
			return out
		elif state.mode_stage == 1 and state.mode_step == 0:
			var prompt_data: Dictionary = saved.get("prompt_data", {})
			var out2: Dictionary = date_appointment_presenter.render_date_three_choice_prompt(hs, prompt_data, hs.date_choice_cursor)
			out2["error"] = ""
			return out2
		else:
			var response_data: Dictionary = saved.get("response_data", {})
			var out3: Dictionary = date_appointment_presenter.render_date_choice_response(hs, response_data)
			out3["error"] = ""
			return out3
	return {"error": "not in an active holiday subscreen"}

func _load(state: State, result: Session.FrameResult) -> void:
	var data: Dictionary=state.rule_state
	var multi: bool=data.get("allow_multi_week",false)
	var idx: int=data["day_event_index"] if multi else 9
	var event: Array=data["day_events"][idx]
	var cmd: int=event[1]
	if not multi and cmd!=0:
		result.error="holiday load supports captured command0 only"
		return
	if multi and cmd not in [0,1,2,3,4,5,6,7]:
		result.error="holiday load supports commands 0..7 only"
		return
	match state.substep:
		0:
			for index: int in range(14):data["icon_enabled"][index]=1 if index==cmd else 0
			data["last_command"]=(data["last_command"] & ~0xf0) | (cmd<<4)
			result.error=Ordinary.increment_holiday_command_counter(state,cmd if multi else 0)
			if not result.error.is_empty():return
			var bg_idx: int=mini(cmd,tables["holiday_activity_background_lbas"].size()-1)
			state.substep=2 if _background(state,"activity",tables["holiday_activity_background_lbas"][bg_idx],result) else 1
		1:
			if _background_ready(state,result):state.substep=2
		2:
			var sd_idx: int=mini(cmd,tables["holiday_sd_non_summer"].size()-1)
			data["scene_work"]=tables["holiday_sd_non_summer"][sd_idx]
			var lba: int=tables["holiday_sd_lbas"][data["scene_work"]]
			data["resources"]["holiday_pending"]={"request":"sd","lba":lba,"ready":false}
			result.presentation.append({"kind":"holiday_load","request":"sd","lba":lba,"sectors":41,"sd_set_id":data["scene_work"]})
			state.substep=3
		3:
			if data["resources"].get("holiday_pending",{}).get("ready",false):state.next_step()
		_:result.error="unsupported holiday activity load substep"

func _background(state: State, request: String, lba: int, result: Session.FrameResult) -> bool:
	var data: Dictionary=state.rule_state
	var slot: int=data["background_cache"].slice(0,2).find(lba)
	if slot>=0:
		data["background_show"]=slot
		data["holiday"]["background_brightness"]=128
		result.presentation.append({"kind":"holiday_background_show","request":request,"lba":lba,"slot":slot,"cached":true})
		return true
	slot=1-data["background_show"]
	data["background_cache"][slot]=lba
	data["resources"]["holiday_pending"]={"request":request,"lba":lba,"slot":slot,"ready":false}
	result.presentation.append({"kind":"holiday_load","request":request,"lba":lba,"sectors":10})
	return false

func _background_ready(state: State, result: Session.FrameResult) -> bool:
	var data: Dictionary=state.rule_state
	var pending: Dictionary=data["resources"].get("holiday_pending",{})
	if not pending.get("ready",false):return false
	if pending["decode_result"]==-1:
		data["background_cache"]=[0,0,0,0]
		data["background_show"]=0
		data["holiday"]["background_brightness"]=128
		result.presentation.append({"kind":"native_background_reset"})
		_background(state,pending["request"],pending["lba"],result)
		return false
	data["background_show"]=pending["slot"]
	data["holiday"]["background_brightness"]=128
	result.presentation.append({"kind":"holiday_background_show","request":pending["request"],"lba":pending["lba"],"slot":pending["slot"],"cached":false})
	return true

func _command(base: Core, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var possible: State=base as State
	var multi: bool=possible!=null and possible.rule_state.get("allow_multi_week",false)
	var is_post_sunday: bool=possible!=null and ([possible.rule_state.get("year"),possible.rule_state.get("month"),possible.rule_state.get("day"),possible.rule_state.get("day_event_index")]==[95,4,9,10] or (multi and int(possible.rule_state.get("day_event_index",0))>0))
	if not is_post_sunday:
		_legacy_command.call(base,input,rng,result)
		return
	var state: State=_state(base,result)
	if state==null:return
	var data: Dictionary=state.rule_state
	var idx: int=data["day_event_index"]
	if data["day_events"][idx][0]!=0xff:
		result.error="next-command controller cannot skip a reachable birthday/annual queue record"
		return
	if not data["resources"].get("available_holiday_commands",[]).has(0):
		result.error="required original next-command room/menu/calendar resources are unavailable"
		return
	if state.mode_stage==1:
		if (input.pressed & 0x20)!=0:
			if multi:
				_legacy_command.call(base,input,rng,result)
				return
			result.error="next-week confirmation requires the following scheduler tranche"
			return
		Command.select_cursor(state,input,result)
		return
	if state.mode_stage!=0:
		result.error="unsupported next-command stage"
		return
	state.mode_step_timer=Math.add32(state.mode_step_timer,1)
	match state.mode_step:
		0:
			data["actor_state"].clear()
			data["holiday"]["tile_layer_enabled"]=0
			data["holiday"]["boot_hold"]=0
			data["holiday"]["background_brightness"]=0
			state.next_step()
		1:state.next_step()
		2:
			if not multi and (data["calendar"][0]["marker"][10]!=0 or data["calendar"][0]["marker"][15]!=0):
				result.error="next-command annual/exam notice branch is unimplemented"
				return
			Rules.icons(state,true,multi)
			result.error=Ordinary.snapshot_parameters(state)
			if not result.error.is_empty():return
			result.presentation.append({"kind":"holiday_screen_init","screen":"next_command","prompt":"settings_command_screen_build/default","icon_count":14})
			if _background(state,"next_command",tables["weekday_room_background_lbas"][0],result):
				data["holiday"]["screen_brightness"]=128
				state.next_stage()
			else:state.next_step()
		3:
			if _background_ready(state,result):
				data["holiday"]["screen_brightness"]=128
				state.next_stage()
		_:result.error="unsupported next-command setup step"
	# Normal stage0 setup has no stage0xff reload brightness increment.
	if result.error.is_empty():Command.select_cursor(state,input,result)

func service_completion(event: Session.ServiceEvent, base: Core, rng: Rng, result: Session.FrameResult) -> void:
	if event.service in ["holiday.actor96","school_day.actor96"] and base.game_mode==0x46:
		var actor_state: State=_state(base,result)
		if actor_state==null:return
		if actor_state.mode_stage!=5 or actor_state.mode_step!=2 or not actor_state.rule_state["actor_state"].has("96") or not event.payload.get("active") is bool or event.payload["active"]!=false or not event.payload.get("counter") is int or not event.payload.get("sequence_slot") is int:
			result.error="holiday actor terminal event arrived outside the source animation state"
			return
		var actor: Dictionary=actor_state.rule_state["actor_state"]["96"]
		if event.payload["sequence_slot"]!=actor["sequence_slot"]:
			result.error="holiday actor terminal sequence differs from source descriptor"
			return
		actor["flags"]=0
		actor["sequence"]["counter"]=Math.s16(event.payload["counter"])
		actor["sequence"]["active"]=false
		return
	if event.service!="holiday.resources":
		_legacy_completion.call(event,base,rng,result)
		return
	var state: State=_state(base,result)
	if state==null:return
	var pending: Dictionary=state.rule_state["resources"].get("holiday_pending",{})
	var allowed: Array[String]=["request","lba","ready"]
	if pending.get("request")!="sd":allowed.append("decode_result")
	for key: String in event.payload:
		if key not in allowed:
			result.error="holiday completion contains an unsupported payload field: "+key
			return
	if pending.is_empty() or event.payload.get("request")!=pending["request"] or event.payload.get("lba")!=pending["lba"] or not event.payload.get("ready") is bool:
		result.error="holiday completion lacks matching native pending resource"
		return
	var valid_wait: bool=(state.game_mode==0x46 and ((state.mode_stage==0 and state.mode_step==1 and pending["request"]=="room") or (state.mode_stage==5 and state.mode_step==0 and ((state.substep==1 and pending["request"]=="activity") or (state.substep==3 and pending["request"]=="sd"))))) or (state.game_mode==0x34 and state.mode_stage==0 and state.mode_step==3 and pending["request"]=="next_command")
	if not valid_wait:
		result.error="holiday completion arrived outside its source wait"
		return
	if pending["request"]!="sd" and event.payload.get("decode_result",-2) not in [-1,0,1]:
		result.error="holiday background completion needs native decode result"
		return
	pending["ready"]=event.payload["ready"]
	if pending["request"]!="sd":pending["decode_result"]=event.payload["decode_result"]
