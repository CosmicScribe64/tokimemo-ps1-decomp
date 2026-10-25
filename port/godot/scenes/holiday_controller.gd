extends RefCounted
## T-29013. Additive captured Sunday controller; root owns app/service bindings.

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

const SHAPES: Dictionary = {"seasonal_room_background_lbas":4,"weekday_room_background_lbas":4,
	"holiday_activity_background_lbas":8,"holiday_sd_non_summer":8,"holiday_sd_summer":8,"holiday_sd_lbas":13}
var tables: Dictionary = {}
var common: Dictionary = {}
var error: String = ""
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
		result.error=Rules.validate(state)
		if result.error.is_empty() and not data["resources"].get("available_holiday_commands",[]).has(0):
			result.error="required original holiday/next-command route capability is unavailable"

func _state(base: Core, result: Session.FrameResult) -> State:
	var state: State=base as State
	if state==null:
		result.error="holiday controller requires transactional school-day state"
		return null
	result.error=Rules.validate(state)
	return state if result.error.is_empty() else null

func _holiday(base: Core, input: InputFrame, rng: Rng, result: Session.FrameResult) -> void:
	var state: State=_state(base,result)
	if state==null:return
	var data: Dictionary=state.rule_state
	if data["day_event_index"]!=9 or data["day_events"][9][0]!=0x10:
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
				Rules.icons(state,false)
				data["actor_state"].clear()
				result.error=Ordinary.snapshot_parameters(state)
				if not result.error.is_empty():return
				result.presentation.append({"kind":"holiday_screen_init","screen":"holiday","prompt":"holiday_open_init/default","icon_count":14})
				if _background(state,"room",tables["seasonal_room_background_lbas"][0],result):state.next_stage()
				else:state.next_step()
			elif state.mode_step==1:
				if _background_ready(state,result):state.next_stage()
			else:result.error="unsupported holiday open step"
		1:
			if state.mode_step!=0:
				result.error="unsupported holiday menu step"
				return
			Screen.step(state,input,common,rng,result)
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
		_:result.error="reachable holiday club/telephone/calendar/options branch is unimplemented"
	# Source open/activity wrappers update the icon HUD after their operation,
	# including the frame that exits to mode31. Menu handles its own tail.
	if result.error.is_empty() and entry_stage!=1:
		Command.select_cursor(state,input,result)

func _load(state: State, result: Session.FrameResult) -> void:
	var data: Dictionary=state.rule_state
	var event: Array=data["day_events"][9]
	if event[1]!=0:
		result.error="holiday load supports captured command0 only"
		return
	match state.substep:
		0:
			for index: int in range(14):data["icon_enabled"][index]=1 if index==0 else 0
			data["last_command"]=(data["last_command"] & ~0xf0) | (event[1]<<4)
			result.error=Ordinary.increment_holiday_command_counter(state,0)
			if not result.error.is_empty():return
			state.substep=2 if _background(state,"activity",tables["holiday_activity_background_lbas"][0],result) else 1
		1:
			if _background_ready(state,result):state.substep=2
		2:
			data["scene_work"]=tables["holiday_sd_non_summer"][0]
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
	if possible==null or [possible.rule_state.get("year"),possible.rule_state.get("month"),possible.rule_state.get("day"),possible.rule_state.get("day_event_index")]!=[95,4,9,10]:
		_legacy_command.call(base,input,rng,result)
		return
	var state: State=_state(base,result)
	if state==null:return
	var data: Dictionary=state.rule_state
	if data["day_events"][10][0]!=0xff:
		result.error="next-command controller cannot skip a reachable birthday/annual queue record"
		return
	if not data["resources"].get("available_holiday_commands",[]).has(0):
		result.error="required original next-command room/menu/calendar resources are unavailable"
		return
	if state.mode_stage==1:
		if (input.pressed & 0x20)!=0:
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
			if data["calendar"][0]["marker"][10]!=0 or data["calendar"][0]["marker"][15]!=0:
				result.error="next-command annual/exam notice branch is unimplemented"
				return
			Rules.icons(state,true)
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
