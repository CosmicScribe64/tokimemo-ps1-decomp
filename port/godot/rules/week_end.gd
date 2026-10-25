extends RefCounted
## T-29009. scheduler.c::schedule_week_end_mode_handler / health_check_roll.
## Healthy outcomes only; treatment/dialogue routes stop transactionally.

const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Rng = preload("res://core/native_rng.gd")
const Math = preload("res://core/native_math.gd")
const Ordinary = preload("res://rules/ordinary_week.gd")

static func step(state: State, rng: Rng, result: Session.FrameResult) -> void:
	var data: Dictionary = state.rule_state
	match state.mode_stage:
		0:
			result.error = Ordinary.snapshot_parameters(state)
			if not result.error.is_empty():
				return
			result.presentation.append({"kind": "week_end_hud_init"})
			state.next_stage()
		1:
			if state.mode_step != 0:
				result.error = "week-end health dialogue/recovery step is unimplemented"
				return
			state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
			result.error = _healthy_roll(state, rng)
			if result.error.is_empty():
				state.set_stage(0xff)
		_:
			if data["day_event_index"] >= 31:
				result.error = "week-end has no next queue record"
				return
			data["day_event_index"] = Math.u8(data["day_event_index"] + 1)
			state.reset_game_mode_fields(0x31)
			data["voice_wait_frames"] = 0
			data["message_title_visible"] = false
			result.transitions.append({"kind": "native_mode", "mode": 0x31})

static func _healthy_roll(state: State, rng: Rng) -> String:
	var data: Dictionary = state.rule_state
	var health: int = (data["player_flags"] >> 8) & 15
	if health != 0:
		return "week-end injury/neurosis/sickness recovery branch is unimplemented"
	if (data["player_flags_upper"] & 255) >> 5 != 0:
		return ""
	var limit: int = mini(Math.divmod32(State.high(data["params"][7]), 8).quotient + 204, 254)
	var condition: int = State.high(data["params"][0])
	var stress: int = State.high(data["params"][8])
	var injury: bool = false
	if condition < 20:
		injury = limit < (rng.next("src/main/scheduler.c::schedule_health_check_roll") & 255)
	injury = injury or condition == 0
	var neurosis: bool = stress >= 81 and Math.u32(limit) < Math.u32(state.system_seed & 255)
	if injury or neurosis:
		return "reachable week-end health change requires unimplemented native dialogue"
	return ""
