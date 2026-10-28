extends RefCounted
## T-29024. Source stage31 frame, not a replacement for the other TACO stages.
## Pure rule/service callbacks operate only on this transaction's private state.
## A returned effect is an ordered intent; playback must not execute rules twice.
const Math = preload("res://core/native_math.gd")
const Rng = preload("res://core/native_rng.gd")
const Combat = preload("res://scenes/shooter_combat_controller.gd")
const State = preload("res://overlays/shooter_combat_state.gd")
const Pool = preload("res://overlays/shooter_enemy_pool_rules.gd")
const StageRules = preload("res://overlays/shooter_stage3_rules.gd")
const Player = preload("res://overlays/shooter_player_rules.gd")
const Primary = preload("res://overlays/shooter_primary_rules.gd")
const Lifecycle = preload("res://overlays/shooter_lifecycle_rules.gd")
const REQUIRED: Array[String] = ["audio_bank_transfer_poll", "shooter_draw_text",
	"shooter_draw_hud", "shooter_frame_update", "shooter_draw_models"]
var combat: Combat
var providers: Dictionary

func _init(native_combat: Combat, native_providers: Dictionary) -> void:
	combat = native_combat
	providers = native_providers.duplicate()

static func validate(state: Dictionary) -> String:
	if not Combat._primitive(state): return "native stage state must remain a bounded plain record"
	var error: String = State.validate(state)
	if error.is_empty(): error = Pool.validate(state)
	if not error.is_empty(): return error
	for key: String in ["load_request", "sound_bank_wait_state", "sound_bank_poll_result"]:
		if not state.get(key) is int or state[key] < 0 or state[key] > 255:
			return "native stage service field exceeds unsigned8: " + key
	return ""

func _invoke(state: Dictionary, name: String, args: Dictionary, rng: Rng) -> Dictionary:
	var raw: Variant = providers[name].call(state, args, rng)
	if not raw is Dictionary or not raw.get("error") is String:
		return {"error":"native stage service must return an explicit result: " + name}
	if not raw["error"].is_empty(): return raw
	var error: String = validate(state)
	return raw if error.is_empty() else {"error":error}

func _script_providers() -> Dictionary:
	var output: Dictionary = {}
	for name: String in StageRules.REQUIRED:
		# Capture a separate value for each closure.
		output[name] = _script_call.bind(name)
	return output

func _script_call(state: Dictionary, argument: int, rng: Rng, name: String) -> String:
	return _invoke(state, name, {"argument":argument}, rng)["error"]

func _run_step(state: Dictionary, rng: Rng) -> String:
	var error: String = ""
	match state["mode_step"]:
		0:
			state["clear_color"] = [8, 0x10, 0x60]
			state["background_flags"] = 4
			Pool.reset_enemies(state)
			error = Player.reset_ship_state(state)["error"]
			if error.is_empty(): error = Primary.clear_player_shots(state)["error"]
			if not error.is_empty(): return error
			state["load_request"] = 0x34
			state["sound_bank_wait_state"] = 0
			state["effects"].append({"kind":"audio_bgm_play_short", "bank":0, "command":0x200})
			Lifecycle.next_step(state)
		1:
			error = StageRules.update_script(state, _script_providers(), rng)
			if not error.is_empty(): return error
			if state["load_request"] == 0 and state["sound_bank_wait_state"] == 0:
				state["sound_bank_wait_state"] = 1
			if state["sound_bank_wait_state"] == 1:
				var poll: Dictionary = _invoke(state, "audio_bank_transfer_poll", {}, rng)
				if not poll["error"].is_empty(): return poll["error"]
				# The native return is stored into u8 before the equality check.
				if not poll.get("value") is int or poll["value"] < -2147483648 or poll["value"] > 2147483647:
					return "native audio-bank poll must return a signed32 value"
				state["sound_bank_poll_result"] = Math.u8(poll["value"])
				if state["sound_bank_poll_result"] == 1: state["sound_bank_wait_state"] = 2
		2:
			if state["load_request"] == 0:
				state["load_request"] = 0x38
				Pool.reset_enemies(state)
				state["effects"].append({"kind":"audio_bgm_play_short", "bank":1, "command":0x200})
				Lifecycle.set_stage(state, 32)
	return error

func step(state: Dictionary, held: int, pressed: int, rng: Rng) -> Dictionary:
	var error: String = validate(state)
	if not error.is_empty(): return {"committed":false, "error":error}
	if state["mode_stage"] != 31: return {"committed":false, "error":"stage-3 frame requires native stage31"}
	if held < 0 or held > 0xffffffff or pressed < 0 or pressed > 0xffffffff:
		return {"committed":false, "error":"native held/pressed masks must be unsigned32"}
	if rng == null or not rng.configuration_error().is_empty():
		return {"committed":false, "error":"native stage frame requires configured RNG"}
	if combat == null: return {"committed":false, "error":"native combat controller is missing"}
	for name: String in REQUIRED + StageRules.REQUIRED:
		if not providers.get(name) is Callable or not providers[name].is_valid():
			return {"committed":false, "error":"required native stage service is not implemented: " + name}
	var checkpoint: Dictionary = rng.checkpoint()
	var work: Dictionary = state.duplicate(true)
	work["effects"] = []
	# shooter_entry increments only this timer here. The outer game loop owns
	# frame_count and input latching; do not invent a second frame clock.
	work["mode_stage_timer"] = Math.add32(work["mode_stage_timer"], 1)
	if Math.u32(work["mode_stage_timer"]) < 0x60 and (work["frame_count"] & 0x10) != 0:
		error = _invoke(work, "shooter_draw_text",
			{"text_key":"stage3_label", "x":-0x40, "y":0, "palette":2}, rng)["error"]
	if error.is_empty(): error = _run_step(work, rng)
	if error.is_empty(): error = _invoke(work, "shooter_draw_hud", {}, rng)["error"]
	if error.is_empty():
		var prior_effects: Array = work["effects"].duplicate(true)
		var battle: Dictionary = combat.update_battle(work, held, pressed, rng)
		if not battle["committed"]: error = battle["error"]
		else:
			work = battle["state"]
			prior_effects.append_array(work["effects"])
			work["effects"] = prior_effects
	if error.is_empty(): error = Lifecycle.check_battle_end(work, pressed)
	if error.is_empty(): error = _invoke(work, "shooter_frame_update", {}, rng)["error"]
	if error.is_empty(): error = _invoke(work, "shooter_draw_models", {}, rng)["error"]
	if error.is_empty(): error = rng.step_error()
	if error.is_empty(): error = validate(work)
	if not error.is_empty():
		rng.restore(checkpoint)
		return {"committed":false, "error":error}
	return {"committed":true, "error":"", "state":work,
		"rng":rng.checkpoint(), "effects":work["effects"].duplicate(true)}
