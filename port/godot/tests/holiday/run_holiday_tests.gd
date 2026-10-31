extends SceneTree
## Invented unit cases. Native replay and actual media/render proof are separate.

const Fixture = preload("res://tests/rules/run_rule_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Legacy = preload("res://scenes/school_day_controller.gd")
const Controller = preload("res://scenes/holiday_controller.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")
const Rules = preload("res://rules/holiday.gd")
const SchoolDay = preload("res://rules/school_day.gd")

var failures: Array[String] = []
var controllers: Array[RefCounted] = []

func check(ok: bool, message: String) -> void:
	if not ok:failures.append(message)

func _empty(_state: State, _rng: Rng, _result: Session.FrameResult) -> void:
	pass

func tables() -> Dictionary:
	return {"seasonal_room_background_lbas":[1000,1001,1002,1003],"weekday_room_background_lbas":[1100,1101,1102,1103],
		"holiday_activity_background_lbas":[1200,1201,1202,1203,1204,1205,1206,1207],
		"holiday_sd_non_summer":[1,1,1,1,1,1,1,1],"holiday_sd_summer":[0,0,0,0,0,0,0,0],
		"holiday_sd_lbas":[2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012]}

func fixture() -> Session:
	var state: State=Fixture.new().synthetic_state()
	var data: Dictionary=state.rule_state
	data["day"]=9;data["weekday"]=6
	data["day_event_index"]=9
	data["day_events"][0]=[0,0,0,15]
	data["day_events"][9]=[0x10,0,0,0]
	data["resources"]["available_holiday_commands"]=[0]
	data["holiday"]={"icon_mode":0,"icon_count":14,"input_device":1,"screen_brightness":128,
		"tile_layer_enabled":0,"boot_hold":0,"background_brightness":128,"clubs":[0,0,0,0,0,0,0,0,0,0,0,0],
		"dial_keys":[1,2,3,4,5,6,7,8],"dial_pos":3}
	state.reset_game_mode_fields(0x46)
	var session: Session=Session.new()
	var legacy: Legacy=Legacy.new(Fixture.new().synthetic_tables())
	var holiday: Controller=Controller.new(Fixture.new().synthetic_tables(),tables())
	controllers.append(legacy);controllers.append(holiday)
	check(legacy.attach(session,state).is_empty(),"legacy attachment")
	check(holiday.attach(session,state).is_empty(),"additive holiday attachment")
	session.rng.configure(Rng.OPENBIOS_PROFILE,123)
	session.draw_logic=_empty;session.cd_logic=_empty;session.audio_logic=_empty
	return session

func step(session: Session, events: Array[Session.ServiceEvent]=[]) -> Session.FrameResult:
	return session.step(InputFrame.new(session.clock.logic_iterations),events)

func _initialize() -> void:
	var session: Session=fixture()
	var state: State=session.state as State
	var result: Session.FrameResult=step(session)
	check(result.committed and state.mode_step==1,"holiday open starts explicit original room resource wait")
	check(state.rule_state["holiday"]["dial_keys"]==[255,255,255,255,255,255,255,8],"source clears seven dial entries and preserves eighth byte")
	check(state.rule_state["icon_enabled"][9]==0 and state.rule_state["icon_enabled"][13]==0 and state.rule_state["icon_enabled"][0]==1,"source holiday icon capabilities")
	var wrong: Session.ServiceEvent=Session.ServiceEvent.new(1,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,"holiday.resources",{"request":"room","lba":999,"ready":true,"decode_result":1})
	var before: String=session.state_hash()
	result=step(session,[wrong])
	check(not result.committed and session.state_hash()==before,"wrong resource identity rolls back mode/clock/RNG/dial/profile state")
	var ready: Session.ServiceEvent=Session.ServiceEvent.new(1,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,"holiday.resources",{"request":"room","lba":1000,"ready":true,"decode_result":1})
	result=step(session,[ready])
	check(result.committed and state.mode_stage==1,"room readiness advances to native holiday menu")
	session.latched_input=InputFrame.new(1,0x2020)
	result=step(session)
	check(result.committed and state.mode_stage==5 and state.rule_state["day_events"][9][1]==0 and state.rule_state["menu_cursor"]==1,"holiday confirms old cursor before same-frame directional update")
	check(session.rng.draw_count==1,"holiday poor-result selection consumes one sourced RNG draw")
	result=step(session)
	check(result.committed and state.rule_state["progress_state"][0x15]==1,"activity load performs one source counter update with safe profile reanchor")
	var mirror: int=state.rule_state["progress_state"][Ordinary.MIRROR_SNAPSHOT_START+0x15]
	check(mirror==0,"counter update does not invent an early native parameter snapshot")
	_test_fixed_timer()
	_test_phone_book_frames()
	_test_stops()
	_test_wrapper_tails()
	for failure: String in failures:push_error(failure)
	print("Holiday invented test failures: %d" % failures.size())
	quit(0 if failures.is_empty() else 1)

## Controller path into the phone book: the click frame only sets stage 0x10;
## holiday_tel_init runs on the next frame (emulator golden dates-phone-golden-v1).
func _test_phone_book_frames() -> void:
	var session: Session=fixture()
	var state: State=session.state as State
	var holiday: Controller=controllers[controllers.size()-1]
	holiday.enable_subscreens=true
	step(session)
	var ready: Session.ServiceEvent=Session.ServiceEvent.new(1,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,"holiday.resources",{"request":"room","lba":1000,"ready":true,"decode_result":1})
	step(session,[ready])
	check(state.mode_stage==1 and state.rule_state["icon_enabled"][8]!=0,"phone icon available on the holiday menu")
	state.rule_state["menu_cursor"]=8
	session.latched_input=InputFrame.new(1,0x20)
	var result: Session.FrameResult=step(session)
	check(result.committed and state.mode_stage==0x10 and state.mode_step==0,"click frame sets stage 0x10 and leaves holiday_tel_init for the next frame")
	check(not holiday.handles_subscreen(state),"phone book is not drawn before telephone_class_init")
	session.latched_input=InputFrame.new(2,0)
	result=step(session)
	check(result.committed and state.mode_step==1 and state.rule_state["menu_cursor"]==0,"next frame runs holiday_tel_init (menu_cursor_0 = 0)")
	check(holiday.handles_subscreen(state),"phone book drawn from step 1")
	session.latched_input=InputFrame.new(3,0x40)
	result=step(session)
	check(result.committed and state.game_mode==0x46 and state.mode_stage in [0, 1],"cross leaves the phone book (mode_stage_set(0); the port restores the menu at once)")

func _test_fixed_timer() -> void:
	var session: Session=fixture()
	var state: State=session.state as State
	check(Ordinary.snapshot_parameters(state).is_empty(),"anchor source profile for isolated activity test")
	state.mode_stage=5;state.mode_step=2;state.mode_step_timer=31
	state.rule_state["actor_state"]={"96":{"id":96,"flags":5,"sequence_slot":0,"sequence":{"counter":9,"active":true}}}
	var expected: State=State.new();expected.restore(state.projection())
	SchoolDay.apply_parameters(expected,0,1,0,Fixture.new().synthetic_tables())
	session.latched_input=InputFrame.new(-1,0x2000)
	var result: Session.FrameResult=step(session)
	check(result.committed and state.rule_state["params"]==expected.rule_state["params"],"timer32 awards holiday-factor4 parameters while actor is still active")
	check(state.rule_state["menu_cursor"]==1,"activity HUD tail consumes the source directional edge after its parameter operation")
	var awarded: Array=state.rule_state["params"].duplicate()
	for index: int in range(32):result=step(session)
	check(state.mode_step_timer==64 and state.game_mode==0x46 and state.rule_state["params"]==awarded,"timer64 holds with no repeated award")
	result=step(session)
	check(result.committed and state.game_mode==0x31 and state.rule_state["day_event_index"]==10 and state.rule_state["day"]==9,"timer65 exits to next queue record without invented Monday increment")
	check(state.rule_state["actor_state"].is_empty(),"holiday exit clears both source actors")
	result=step(session)
	check(result.committed and state.game_mode==0x34 and state.mode_stage==0,"native NONE dispatcher returns to next command setup")

func _test_wrapper_tails() -> void:
	var session: Session=fixture()
	var state: State=session.state as State
	step(session)
	session.latched_input=InputFrame.new(0,0x2000)
	var result: Session.FrameResult=step(session)
	check(result.committed and state.mode_step==1 and state.rule_state["menu_cursor"]==1,"open background wait still runs its source icon HUD/cursor tail")
	session=fixture();state=session.state as State
	state.rule_state["day_event_index"]=10
	state.reset_game_mode_fields(0x34)
	state.mode_step=1
	state.rule_state["holiday"]["screen_brightness"]=0
	session.latched_input=InputFrame.new(-1,0x2000)
	result=step(session)
	check(result.committed and state.mode_step==2 and state.rule_state["holiday"]["screen_brightness"]==0,"normal command setup does not apply the unrelated reload-only fade")
	check(state.rule_state["menu_cursor"]==1,"normal command setup preserves its source HUD/cursor tail")

func _test_stops() -> void:
	var session: Session=fixture()
	var state: State=session.state as State
	state.rule_state["resources"]["available_holiday_commands"]=[]
	var before: String=session.state_hash()
	var result: Session.FrameResult=step(session)
	check(not result.committed and session.state_hash()==before,"missing mandatory holiday capability stops before progression")
	session=fixture();state=session.state as State
	state.mode_stage=1;state.rule_state["menu_cursor"]=8
	session.latched_input=InputFrame.new(-1,0x20)
	before=session.state_hash();result=step(session)
	check(not result.committed and session.state_hash()==before and session.rng.draw_count==0,"reachable telephone choice stops transactionally without random draw")
	session=fixture();state=session.state as State
	Ordinary.snapshot_parameters(state)
	state.mode_stage=5;state.rule_state["progress_state"][0]^=1
	before=session.state_hash();result=step(session)
	check(not result.committed and session.state_hash()==before,"counter update refuses an edited source profile before reanchor")
	for mutation: String in ["capability_type","pending_type","pending_slot","dial_byte","menu_extent"]:
		session=fixture();state=session.state as State
		match mutation:
			"capability_type":state.rule_state["resources"]["available_holiday_commands"]="invalid"
			"pending_type":state.rule_state["resources"]["holiday_pending"]="invalid"
			"pending_slot":state.rule_state["resources"]["holiday_pending"]={"request":"room","lba":1000,"slot":2,"ready":false}
			"dial_byte":state.rule_state["holiday"]["dial_keys"][0]=256
			"menu_extent":state.rule_state["menu_disabled"].pop_back()
		before=session.state_hash();result=step(session)
		check(not result.committed and session.state_hash()==before,"corrupt holiday boundary fails before progression: "+mutation)
	session=fixture();state=session.state as State
	state.rule_state.erase("holiday")
	state.rule_state["day"]=8;state.rule_state["weekday"]=5
	state.rule_state["day_event_index"]=8;state.rule_state["day_events"][8]=[0xfb,0,0,0]
	state.reset_game_mode_fields(0x31)
	before=session.state_hash();result=step(session)
	check(not result.committed and session.state_hash()==before and state.rule_state["day"]==8,"missing holiday extension rolls back the preceding day-end calendar/queue transition")
