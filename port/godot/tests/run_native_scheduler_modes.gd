extends SceneTree
## Diagnostic source-branch tests; not an end-to-end route claim.
const Fixture = preload("res://tests/rules/run_rule_tests.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")
var failures: Array[String] = []
var checks: int = 0
func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures.append(label)
func fresh(event: int, girl: int = 0) -> State:
	var state: State = Fixture.new().synthetic_state()
	state.reset_game_mode_fields(0x31)
	state.rule_state["allow_multi_week"] = true
	state.rule_state["day_event_index"] = 1
	state.rule_state["day_events"][0] = [0, 0, 0, 15]
	state.rule_state["day_events"][1] = [event, girl, 7, 0]
	return state
func _initialize() -> void:
	# Independent explicit cases read from scheduler.c native switch.
	for pair: Array in [[0x20,0x72],[0x25,0x74],[0x26,0x75],[0x27,0x76],[0x28,0x42],[0x2c,0x41],[0x30,0x55],[0x31,0x54],[0x32,0x57],[0x33,0x56],[0x40,0x35],[0x41,0x51],[0x42,0x5f],[0x43,0x36],[0x44,0x59],[0x45,0x5d],[0x46,0x5a],[0x47,0x61],[0x48,0x5b],[0x49,0x58],[0x4a,0x37],[0xfe,0x38],[0xfd,0x39]]:
		var state: State = fresh(pair[0])
		var result := Session.FrameResult.new()
		Ordinary.dispatch(state,result)
		check(result.error.is_empty() and state.game_mode == pair[1], "native event mode %02x" % pair[0])
		check(state.rule_state["day_event_index"] == 1, "event not silently discarded %02x" % pair[0])
	for club: int in [0,5,6,10,11]:
		var state: State = fresh(0x24)
		state.rule_state["player_flags"] = club << 12
		Ordinary.dispatch(state,Session.FrameResult.new())
		check(state.game_mode == (0x70 if club < 6 else 0x71), "festival club boundary")
	var skipped := fresh(0x44,1)
	Ordinary.dispatch(skipped,Session.FrameResult.new())
	check(skipped.game_mode == 0x31 and skipped.rule_state["day_event_index"] == 2 and skipped.rule_state["day_event_count_today"] == 1,"unmet girl skips once")
	var invited := fresh(0x46)
	invited.rule_state["appointment_words"][0] = 1 << 29
	Ordinary.dispatch(invited,Session.FrameResult.new())
	check(invited.game_mode == 0x31 and invited.rule_state["day_event_index"] == 2,"active appointment skips invitation")
	var graduation := fresh(0xff)
	graduation.rule_state["year"] = 98
	graduation.rule_state["month"] = 3
	graduation.rule_state["day"] = 1
	Ordinary.dispatch(graduation,Session.FrameResult.new())
	check(graduation.game_mode == 0x31 and graduation.mode_step == 1 and graduation.substep == 0,"graduation awaits native audio preparation step")
	check(not graduation.rule_state.has("ending_completed"),"graduation not completed by date alone")
	print(JSON.stringify({"checks":checks,"failures":failures}))
	quit(0 if failures.is_empty() else 1)
