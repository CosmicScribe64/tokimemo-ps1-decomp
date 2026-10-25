extends SceneTree
## External native semantic-rule oracle. Actor/CD/audio execution is separate.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Legacy = preload("res://scenes/school_day_controller.gd")
const Controller = preload("res://scenes/holiday_controller.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")

var controllers: Array[RefCounted] = []
var failures: Array[String] = []
var reports: Array[Dictionary] = []

func ints(value: Variant) -> Variant:
	if value is Dictionary:
		var data: Dictionary={}
		for key: String in value:data[key]=ints(value[key])
		return data
	if value is Array:
		var data: Array=[]
		for item: Variant in value:data.append(ints(item))
		return data
	return int(value) if value is float else value

func _empty(_state: State, _rng: Rng, _result: Session.FrameResult) -> void:
	pass

func _initialize() -> void:
	var fixture: Dictionary=ints(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_HOLIDAY_FIXTURE"))))
	var common: Dictionary=ints(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_TABLES"))))["tables"]
	var holiday: Dictionary=ints(JSON.parse_string(FileAccess.get_file_as_string(OS.get_environment("TOKIMEMO_HOLIDAY_TABLES"))))["tables"]
	var points: Array=fixture["checkpoints"]
	for index: int in range(points.size()-1):
		var current: Dictionary=points[index]
		var expected: Dictionary=points[index+1]
		var reference: Array=[]
		var differences: Array[String]=[]
		for rendered: int in [0,1,3]:
			var state: State=State.new()
			for key: String in current["core"]:state.set(key,current["core"][key])
			state.rule_state=current["rule_state"].duplicate(true)
			# Trusted oracle projection gains only the port's integrity metadata.
			state.rule_state[Ordinary.PROFILE_HASH_FIELD]=Ordinary._profile_signature(state.rule_state)
			var session: Session=Session.new()
			var legacy: Legacy=Legacy.new(common)
			var extension: Controller=Controller.new(common,holiday)
			controllers.append(legacy);controllers.append(extension)
			var failure: String=legacy.attach(session,state)
			if failure.is_empty():failure=extension.attach(session,state)
			if not failure.is_empty():
				failures.append(failure)
				break
			session.rng.configure(Rng.OPENBIOS_PROFILE,current["rng_state"])
			session.draw_logic=_empty;session.cd_logic=_empty;session.audio_logic=_empty
			session.clock.rendered_frames=rendered
			var input: Dictionary=current["input"]
			var previous: int=(input["held"] & ~input["pressed"]) | input["released"]
			session.latched_input=InputFrame.new(-1,input["held"],previous)
			var events: Array[Session.ServiceEvent]=[]
			var pending: Dictionary=state.rule_state["resources"].get("holiday_pending",{})
			var changed: bool=[state.game_mode,state.mode_stage,state.mode_step,state.substep]!=[expected["core"]["game_mode"],expected["core"]["mode_stage"],expected["core"]["mode_step"],expected["core"]["substep"]]
			if not pending.is_empty() and changed:
				var payload: Dictionary={"request":pending["request"],"lba":pending["lba"],"ready":true}
				if pending["request"]!="sd":payload["decode_result"]=expected["rule_state"]["background_show"]
				events.append(Session.ServiceEvent.new(0,Session.Phase.BEFORE_MODE,Session.ServiceKind.COMPLETION,"holiday.resources",
					payload))
			var result: Session.FrameResult=session.step(InputFrame.new(0,expected["input"]["held"]),events)
			if rendered==0:
				if not result.committed:differences.append("error:"+result.error)
				for key: String in ["game_mode","mode_stage","mode_step","substep","game_mode_timer","mode_stage_timer","mode_step_timer","frame_count"]:
					if state.get(key)!=expected["core"][key]:differences.append(key)
				for key: String in ["year","month","day","weekday","day_count","calendar","params","player_flags","player_flags_upper","player_birthday","girls","progress_state","day_events","day_event_index","day_event_done_bits","day_event_count_today","daily_check_done","last_command","menu_cursor","menu_mode","menu_disabled","icon_enabled","appointment_words","background_cache","background_show","scene_work","holiday"]:
					if state.rule_state[key]!=expected["rule_state"][key]:differences.append(key)
				if session.rng.state!=expected["rng_state"]:differences.append("rng")
				reference=[result.state_hash,result.event_hash]
			elif reference!=[result.state_hash,result.event_hash]:differences.append("render-dependent hashes")
		if not differences.is_empty():failures.append("native case%d: %s" % [index,str(differences)])
		reports.append({"index":index,"mode":current["core"]["game_mode"],"stage":current["core"]["mode_stage"],"step":current["core"]["mode_step"],"substep":current["core"]["substep"],"differences":differences})
	var output: FileAccess=FileAccess.open(OS.get_environment("TOKIMEMO_HOLIDAY_PROOF"),FileAccess.WRITE)
	output.store_string(JSON.stringify({"failures":failures,"cases":reports,"render_schedules":[0,1,3],
		"scope":"phase-qualified one-iteration semantic-rule checkpoints; actor draw/CD/audio/interrupt execution excluded"},"\t"))
	output.close()
	for failure: String in failures:push_error(failure)
	print(JSON.stringify({"cases":reports.size(),"failures":failures,"render_schedules":[0,1,3]}))
	quit(0 if failures.is_empty() else 1)
