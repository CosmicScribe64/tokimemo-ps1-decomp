extends RefCounted
## T-29013. Captured April9 normal holiday; other event/health/club routes stop.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")
const SchoolDay = preload("res://rules/school_day.gd")
const Math = preload("res://core/native_math.gd")

static func validate(state: State) -> String:
	var failure: String = state.structural_error()
	if not failure.is_empty():return failure
	var data: Dictionary = state.rule_state
	if [data["year"],data["month"],data["day"],data["weekday"],data["vacation"]] != [95,4,9,6,0]:
		return "holiday controller supports the captured April9,1995 normal Sunday only"
	if ((data["player_flags"] >> 8) & 15) != 0 or (data["player_flags_upper"] & 1) != 0 or data["player_flags"] >> 12 != 11:
		return "holiday health/remedial/club branch is unimplemented"
	for appointment: int in data["appointment_words"]:
		if appointment != 0:return "holiday appointment branch requires its native handler"
	if not data.get("holiday") is Dictionary:return "holiday native boundary extension is missing"
	var holiday: Dictionary = data["holiday"]
	for key: String in ["icon_mode","icon_count","input_device","screen_brightness","tile_layer_enabled","boot_hold","background_brightness"]:
		if not holiday.get(key) is int:return "holiday native boundary missing integer: "+key
	if holiday["icon_mode"] != 0 or holiday["input_device"] != 1 or data.get("menu_mode") != 1:
		return "holiday input requires captured native pad/menu-modeA/icon-layoutA"
	if data["menu_cursor"]<0 or data["menu_cursor"]>=14 or not data.get("menu_disabled") is Array or data["menu_disabled"].size()!=14:
		return "holiday requires the source fourteen-record menu boundary"
	for value: Variant in data["menu_disabled"]:
		if not value is int or value<0 or value>255:return "holiday menu flags must be native bytes"
	if holiday["icon_count"]<0 or holiday["icon_count"]>15 or holiday["screen_brightness"]<0 or holiday["screen_brightness"]>255 or holiday["background_brightness"]<0 or holiday["background_brightness"]>255:
		return "holiday icon/brightness fields exceed native byte bounds"
	if holiday["tile_layer_enabled"] < -128 or holiday["tile_layer_enabled"]>127 or holiday["boot_hold"] < -128 or holiday["boot_hold"]>127:
		return "holiday tile/boot fields exceed native signed-byte bounds"
	if not holiday.get("clubs") is Array or holiday["clubs"].size() != 12:
		return "holiday native club records are missing"
	for club: Variant in holiday["clubs"]:
		if not club is int or club != 0:return "holiday club history differs from the captured no-club boundary"
	if not holiday.get("dial_keys") is Array or holiday["dial_keys"].size() != 8 or not holiday.get("dial_pos") is int:
		return "holiday native dial-ring boundary is missing"
	if holiday["dial_pos"]<0 or holiday["dial_pos"]>6:return "holiday dial-ring index exceeds native bound"
	for value: Variant in holiday["dial_keys"]:
		if not value is int or value<0 or value>255:return "holiday dial-ring entries must be native bytes"
	var capabilities: Variant=data["resources"].get("available_holiday_commands",[])
	if not capabilities is Array:return "holiday capability list must be an integer array"
	for value: Variant in capabilities:
		if not value is int or value!=0:return "holiday capability advertises an unsupported command"
	if data["resources"].has("holiday_pending"):
		var raw_pending: Variant=data["resources"]["holiday_pending"]
		if not raw_pending is Dictionary:return "holiday pending resource must be a primitive record"
		var pending: Dictionary=raw_pending
		if pending.get("request") not in ["room","activity","sd","next_command"] or not pending.get("lba") is int or pending["lba"]<0 or pending["lba"]>2147483647 or not pending.get("ready") is bool:
			return "holiday pending resource has invalid identity/readiness fields"
		if pending["request"]!="sd":
			if not pending.get("slot") is int or pending["slot"] not in [0,1]:return "holiday pending background slot is outside the native cache"
			if pending["ready"] and pending.get("decode_result",-2) not in [-1,0,1]:return "holiday ready background lacks its native decode result"
	for value: Variant in data["params"]:
		if not value is int or value < -2147483648 or value > 2147483647:return "holiday parameter exceeds native signed32"
	for value: Variant in data["progress_state"]:
		if not value is int or value < 0 or value > 255:return "holiday progress snapshot needs native bytes"
	if data["day_event_index"] not in [9,10]:return "holiday queue index is outside captured Sunday/next-command boundary"
	return ""

static func icons(state: State, weekday: bool) -> void:
	var data: Dictionary = state.rule_state
	data["holiday"]["icon_count"] = 14
	for index: int in range(14):data["icon_enabled"][index]=1
	for index: int in ([4,8,9,10] if weekday else [9,13]):data["icon_enabled"][index]=0
	data["menu_disabled"].fill(0)
	if weekday:data["menu_cursor"]=data["last_command"] & 15

static func actor(state: State) -> Dictionary:
	var data: Dictionary = state.rule_state
	var event: Array = data["day_events"][data["day_event_index"]]
	return {"id":96,"sequence_slot":event[2],"x":-80,"y":-64,"flags":5,
		"draw_flags":0x84,"quad_flags":0x01000000,"blink_period":0x82 if data["sd_anim_fast"] != 0 else 1,
		"unk_01":1,"unk_05":2,"unk_43":0x10}

static func play(state: State, common_tables: Dictionary, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	if not data["actor_state"].has("96"):
		result.error="holiday native actor96 descriptor is missing"
		return
	if state.mode_step_timer == 32:
		var event: Array = data["day_events"][data["day_event_index"]]
		result.audio.append({"kind":"native_audio_command","command":0x501 if (data["actor_state"]["96"]["sequence_slot"] & 1)==0 else 0x502})
		result.error=SchoolDay.apply_parameters(state,event[1],1,event[2],common_tables)
		if not result.error.is_empty():return
		result.error=Ordinary.snapshot_parameters(state)
		if not result.error.is_empty():return
		result.presentation.append({"kind":"parameter_snapshot","words":data["params"].duplicate()})
		return
	if Math.u32(state.mode_step_timer) <= 64:return
	data["actor_state"].clear()
	data["day_event_index"] = Math.u8(data["day_event_index"]+1)
	state.reset_game_mode_fields(0x31)
	data["voice_wait_frames"]=0
	data["message_title_visible"]=false
	result.presentation.append({"kind":"school_day_clear_actors","actors":[96,97]})
	result.transitions.append({"kind":"native_mode","mode":0x31,"day_event_index":data["day_event_index"]})
