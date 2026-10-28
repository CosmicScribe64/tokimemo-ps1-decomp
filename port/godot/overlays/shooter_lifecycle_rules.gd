extends RefCounted
## T-29024. Native battle-end, result arithmetic and model-record reset.
## Source player_control.c, result_screen.c, game_mode.c, model_3d.c.

const Math = preload("res://core/native_math.gd")
const Player = preload("res://overlays/shooter_player_rules.gd")
const NATIVE_GAME_MODE: int = 0x90
const TT_NATIVE_GAME_MODE: int = 0xd0
const GAME_END_STAGE: int = 45

static func _word(state: Dictionary, key: String) -> bool:
	return state.get(key) is int and state[key] >= -2147483648 and state[key] <= 2147483647

static func validate(state: Dictionary) -> String:
	var error: String = Player.actor_error(state)
	if not error.is_empty(): return error
	for key: String in ["score", "mode_stage_timer", "mode_step_timer", "count_target", "count_current"]:
		if not _word(state, key): return "native shooter signed32 field is missing or out of range: " + key
	if not state.get("frame_count") is int or state["frame_count"] < 0 or state["frame_count"] > 0xffffffff:
		return "native shooter frame counter exceeds unsigned32"
	if not state.get("time_left") is int or state["time_left"] < -32768 or state["time_left"] > 32767:
		return "native shooter time_left exceeds signed16"
	if not state.get("mode_stage_scratch") is int or state["mode_stage_scratch"] < -128 or state["mode_stage_scratch"] > 127:
		return "native mode-stage scratch exceeds signed8"
	for key: String in ["game_mode", "mode_stage", "mode_step", "substep", "stages_cleared"]:
		if not state.get(key) is int or state[key] < 0 or state[key] > 255:
			return "native shooter byte field is missing or out of range: " + key
	if state["game_mode"] != NATIVE_GAME_MODE: return "TACO requires native mode0x90"
	return ""

static func set_stage(state: Dictionary, stage: int) -> void:
	state["mode_stage"] = Math.u8(stage)
	state["mode_step"] = 0
	state["mode_stage_timer"] = 0
	state["mode_step_timer"] = 0
	state["mode_stage_scratch"] = 0
	state["substep"] = 0

static func next_step(state: Dictionary) -> void:
	state["mode_step"] = Math.u8(state["mode_step"] + 1)
	state["mode_step_timer"] = 0
	state["substep"] = 0

static func check_battle_end(state: Dictionary, pressed: int) -> String:
	var error: String = validate(state)
	if not error.is_empty(): return error
	if pressed < 0 or pressed > 0xffffffff: return "native pad word exceeds unsigned32"
	# Source decrements zero to -1 too; do not invent a saturating countdown.
	if state["stages_cleared"] < 3:
		state["time_left"] = Math.s16(state["time_left"] - 1)
	if state["time_left"] == 0: set_stage(state, GAME_END_STAGE)
	if (pressed & 0x800) != 0: set_stage(state, GAME_END_STAGE)
	if state["actors"][0]["stats"][1] == 0: set_stage(state, GAME_END_STAGE)
	return ""

static func _result_boundary_error(state: Dictionary) -> String:
	var error: String = validate(state)
	if not error.is_empty(): return error
	if not state.get("clear_color") is Array or state["clear_color"].size() != 3:
		return "result needs the actual three native clear-color bytes"
	for color: Variant in state["clear_color"]:
		if not color is int or color < 0 or color > 255: return "native clear color is not a byte"
	if not state.get("model_slots") is Array or state["model_slots"].size() != 16:
		return "result needs all sixteen native model-slot attributes"
	for attribute: Variant in state["model_slots"]:
		if not attribute is int or attribute < -2147483648 or attribute > 2147483647:
			return "native model-slot attribute exceeds signed32"
	for raw: Variant in state["actors"]:
		if not raw is Dictionary: return "native actor record missing"
		var actor: Dictionary = raw
		for key: String in Player.ACTOR_WORDS:
			if not actor.get(key) is int or actor[key] < -32768 or actor[key] > 32767:
				return "result native actor signed16 field is malformed"
		for shape: Array in [["stats", 4], ["flag_bytes", 2]]:
			if not actor.get(shape[0]) is Array or actor[shape[0]].size() != shape[1]: return "result actor byte record missing"
			for byte: Variant in actor[shape[0]]:
				if not byte is int or byte < 0 or byte > 255: return "result actor byte out of range"
	return ""

static func release_actor_records(state: Dictionary) -> void:
	# Native shooter_actors aliases actor_pool + 2*0x44. The original loop
	# initializes records2,4,...158; coordinate and model handle words survive.
	for actor: Dictionary in state["actors"]:
		actor["flag_bytes"][0] = 1
		actor["flag_bytes"][1] = 0
		actor["mode"] = 0
		actor["phase"] = 0
		for key: String in Player.ACTOR_WORDS: actor[key] = 0
		for key: String in ["scale_x", "scale_y", "scale_z"]: actor[key] = 0x1000
		actor["stats"][0] = 0
		actor["stats"][1] = 1
		actor["stats"][2] = 0
		# Native view_model_record_init does not write stats[3].

static func compute_result(state: Dictionary) -> String:
	var error: String = _result_boundary_error(state)
	if not error.is_empty(): return error
	state["clear_color"] = [0, 0, 0]
	var cleared: int = state["stages_cleared"]
	var actor: Dictionary = state["actors"][0]
	state["score"] = Math.add32(state["score"], cleared * 1000000)
	var shield: int = actor["stats"][1]
	var shield_bonus: int = shield * 100
	var time_bonus: int = 0
	if cleared >= 3:
		shield_bonus = 500000 if shield == 255 else shield * 1000
		time_bonus = Math.s32(state["time_left"] * 100)
	var bomb_bonus: int = 0
	if actor["argument_z"] != 0: bomb_bonus = 500000
	elif actor["argument_y"] != 0: bomb_bonus = 250000
	elif actor["argument_x"] != 0: bomb_bonus = 100000
	var total: int = Math.add32(Math.add32(Math.add32(state["score"], shield_bonus), bomb_bonus), time_bonus)
	if total >= 10000000: total = 9999999
	state["result_values"] = [state["score"], shield_bonus, bomb_bonus, time_bonus, total]
	state["effects"].append({"source":"audio_command", "args":[0x24]})
	state["model_slots"].fill(-2147483648)
	release_actor_records(state)
	next_step(state)
	return ""

static func count_up_number(state: Dictionary, target: int, draw_x: int, draw_y: int, count_mode: int) -> Dictionary:
	if not _word(state, "count_target") or not _word(state, "count_current") or not state.get("frame_count") is int or state["frame_count"] < 0 or state["frame_count"] > 0xffffffff or not state.get("effects") is Array:
		return {"error":"native result counter fields are malformed", "value":0}
	if target < -2147483648 or target > 2147483647: return {"error":"counter target exceeds signed32", "value":0}
	if count_mode == 0:
		state["count_target"] = target
		state["count_current"] = 0
		return {"error":"", "value":0}
	var current: int = state["count_current"]
	var expected: int = state["count_target"]
	if current < expected and (state["frame_count"] & 1) == 0:
		for scale: int in [1, 10, 100, 1000, 10000, 100000, 1000000]:
			if current < Math.divmod32(expected, scale * 10).remainder:
				current = Math.add32(current, scale)
				break
	if expected < current: current = expected
	state["count_current"] = current
	state["effects"].append({"source":"shooter_draw_number", "args":[current, draw_x, draw_y, 0]})
	return {"error":"", "value":current}
