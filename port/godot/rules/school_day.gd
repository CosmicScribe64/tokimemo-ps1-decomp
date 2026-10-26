extends RefCounted
## T-29004 / T-29014: actual school_day.c arithmetic and stage1/2 behavior (including Command 4 Club).

const Math = preload("res://core/native_math.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")

const CLUB_ACTIVITY_DELTAS: Array = [
	[-12,  12,   4,   2,   0,   1,  -4,  -1,  18], # 0: Literature (文芸)
	[-14,  14,   0,   6,   2,   1,  -2,   1,  17], # 1: Drama (演劇)
	[-13,   1,  15,   4,   0,   1,  -4,   4,  22], # 2: Science (科学)
	[-13,   1,  14,   6,   0,   4,  -6,   4,  26], # 3: Computer (電脳)
	[-14,   2,   0,  15,   1,   1,  -1,  -2,  15], # 4: Art (美術)
	[-14,   1,   1,  14,   3,   2,  -3,   1,  14], # 5: Brass Band (吹奏楽)
	[-30,  -1,  -1,  -1,  14,   1,  -4,  12,   3], # 6: Baseball (野球)
	[-30,  -1,  -1,  -1,  15,   2,  -3,  11,   4], # 7: Soccer (サッカー)
	[-25,  -1,  -1,   0,  13,   2,  -1,   9,   8], # 8: Tennis (テニス)
	[-31,  -1,  -1,   0,  15,   0,  -2,  11,   3], # 9: Swimming (水泳)
	[-29,  -1,  -1,  -1,  14,   1,  -3,  10,   7], # 10: Basketball (バスケ)
]

static func apply_parameters(state: State, command: int, holiday_factor: int,
	result_divisor: int, tables: Dictionary) -> String:
	if command not in [0, 1, 2, 3, 4, 5, 6, 7]:
		return "parameter_up_down supports only captured commands 0..7"
	var data: Dictionary = state.rule_state
	var deltas: Array
	if command == 4:
		var club_id: int = (data["player_flags"] >> 12) & 15
		if club_id >= 11:
			return "club command requires valid club_id 0..10"
		deltas = tables.get("club_activity_deltas", CLUB_ACTIVITY_DELTAS)[club_id]
	else:
		if not tables.has("command_deltas") or tables["command_deltas"].size() != 8:
			return "required original command_deltas table is missing"
		deltas = tables["command_deltas"][command]
	if deltas.size() != 9:
		return "command delta row must contain nine signed bytes"

	var health: int = (data["player_flags"] >> 8) & 15
	var sickness: int = 2 if health == 3 else 1
	var restriction: int = 2 if health in [1, 2] and command not in [5, 6, 7] else 1
	var holiday: int = 4 if holiday_factor != 0 else 1
	var result_scale: int = 2 if result_divisor != 0 else 1
	var gain_scale: int = Math.divmod32(holiday * 4096, result_scale).quotient
	gain_scale = Math.divmod32(gain_scale, sickness).quotient
	gain_scale = Math.divmod32(gain_scale, restriction).quotient
	for index: int in range(9):
		var delta: int = Math.s8(deltas[index])
		var change: int
		if (index < 8 and delta > 0) or (index == 8 and delta < 0):
			change = Math.s32(Math.multu32(delta, gain_scale).lo)
		else:
			change = Math.sll32(delta, 12)
		if index == 8:
			change = Math.add32(change, (result_scale << 16) - 65536)
		data["params"][index] = Math.parameter_change_word(data["params"][index], change)
	return ""

static func increment_practice_points(data: Dictionary, club_id: int) -> void:
	if not data.has("clubs") or not data["clubs"] is Array or data["clubs"].size() != 12:
		data["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var entry: Variant = data["clubs"][club_id]
	if entry is int:
		data["clubs"][club_id] = (entry + 1) & 65535
	elif entry is Dictionary:
		entry["practice_points"] = (int(entry.get("practice_points", 0)) + 1) & 65535

static func main_step(state: State, tables: Dictionary, result: Session.FrameResult) -> void:
	# week_day_main increments this before calling week_day_main0.
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	var data: Dictionary = state.rule_state
	if not data["actor_state"].has("96"):
		result.error = "required native actor96 state is missing"
		return
	var actor: Dictionary = data["actor_state"]["96"]
	if not actor.get("flags") is int or not actor.get("sequence_slot") is int or not actor.get("sequence") is Dictionary or not actor["sequence"].get("counter") is int:
		result.error = "actor96 completion/counter state is incomplete"
		return
	if (actor["flags"] & 1) == 0:
		var old_counter: int = Math.s16(actor["sequence"]["counter"])
		if old_counter == 0:
			var event: Array = data["day_events"][data["day_event_index"]]
			result.error = apply_parameters(state, event[1], 0, event[2], tables)
			if not result.error.is_empty():
				return
			if event[1] == 4:
				var club_id: int = (data["player_flags"] >> 12) & 15
				increment_practice_points(data, club_id)
			result.audio.append({"kind": "native_audio_command", "command": 0x501 if (actor["sequence_slot"] & 1) == 0 else 0x502})
			result.presentation.append({"kind": "parameter_snapshot", "words": data["params"].duplicate()})
		actor["sequence"]["counter"] = Math.s16(old_counter + 1)
		if old_counter >= 13:
			state.next_stage()
	if Math.u32(state.mode_step_timer) >= 601:
		result.presentation.append({"kind": "native_cd_command_reset"})
		state.next_stage()

static func exit_step(state: State, result: Session.FrameResult) -> void:
	state.mode_step_timer = Math.add32(state.mode_step_timer, 1)
	if state.mode_step == 0:
		state.rule_state["day_event_index"] = Math.u8(state.rule_state["day_event_index"] + 1)
		state.rule_state["actor_state"].clear()
		state.reset_game_mode_fields(0x31)
		state.rule_state["voice_wait_frames"] = 0
		state.rule_state["message_title_visible"] = false
		result.presentation.append({"kind": "school_day_clear_actors", "actors": [96, 97]})
		result.transitions.append({"kind": "native_mode", "mode": 0x31,
			"day_event_index": state.rule_state["day_event_index"]})
