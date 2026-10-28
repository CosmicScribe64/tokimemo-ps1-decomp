extends RefCounted
## T-29024. player_control.c integer rules; private working state only.
## Caller commits state and native RNG together after checking error.

const Math = preload("res://core/native_math.gd")
const ACTOR_WORDS: Array[String] = ["rotation_x", "rotation_y", "rotation_z", "param",
	"scale_x", "scale_y", "scale_z", "weapon", "x", "y", "z", "counter",
	"argument_x", "argument_y", "argument_z", "auxiliary"]

static func outcome(value: int = 0, error: String = "") -> Dictionary:
	return {"value":value, "error":error}

static func actor_error(state: Dictionary) -> String:
	if not state.get("actors") is Array or state["actors"].size() != 79 or not state.get("effects") is Array:
		return "shooter rules require79 native actors and ordered effects"
	for slot: int in [0, 8]:
		var raw: Variant = state["actors"][slot]
		if not raw is Dictionary:
			return "shooter ship record is missing"
		var actor: Dictionary = raw
		for key: String in ACTOR_WORDS:
			if not actor.get(key) is int or actor[key] < -32768 or actor[key] > 32767:
				return "shooter ship signed16 field is invalid: " + key
		for key: String in ["mode", "phase"]:
			if not actor.get(key) is int or actor[key] < 0 or actor[key] > 255:
				return "shooter ship byte field is invalid: " + key
		for pair: Array in [["flag_bytes", 2], ["stats", 4]]:
			if not actor.get(pair[0]) is Array or actor[pair[0]].size() != pair[1]:
				return "shooter ship native byte record is invalid: " + pair[0]
			for byte: Variant in actor[pair[0]]:
				if not byte is int or byte < 0 or byte > 255:
					return "shooter ship native byte exceeds its width"
	return ""

static func reset_ship_state(state: Dictionary) -> Dictionary:
	var error: String = actor_error(state)
	if not error.is_empty(): return outcome(0, error)
	for index: int in [0, 8]:
		var actor: Dictionary = state["actors"][index]
		actor["x"] = 0
		actor["y"] = 0
		actor["z"] = 0x800
		actor["mode"] = 0
		actor["rotation_z"] = 0
	# Source does not reset the wingman's phase.
	state["actors"][0]["phase"] = 0
	return outcome()

static func init_ships(state: Dictionary, model_bindings: Array = []) -> Dictionary:
	var error: String = actor_error(state)
	if not error.is_empty(): return outcome(0, error)
	# Outputs of the source model-map/link provider, NOT a model-ready flag.
	# Provider owns source model authentication and coordinate/self-link identity.
	if model_bindings.size() != 2:
		return outcome(0, "shooter_init_actor_model requires verified model-map/link outputs for objects0/1")
	for binding: Variant in model_bindings:
		if not binding is Dictionary or binding.size() != 2 or not binding.get("attribute") is int or binding["attribute"] != 0 or not binding.get("links") is Array or binding["links"].size() != 3:
			return outcome(0, "source ship model binding requires attribute0 and three native links")
		for word: Variant in binding["links"]:
			if not word is int or word < -2147483648 or word > 2147483647:
				return outcome(0, "source ship model link must be a native signed32 word")
	for index: int in range(2):
		var slot: int = index * 8
		var actor: Dictionary = state["actors"][slot]
		actor["model"] = model_bindings[index].duplicate(true)
		state["effects"].append({"kind":"shooter_init_actor_model", "model_address":Math.s32(0x8019c800), "object_index":index, "actor":slot})
		actor["flag_bytes"][1] = 0x80
		actor["scale_x"] = 0x400
		actor["scale_y"] = 0x400
		actor["scale_z"] = 0x400
	for slot: int in [0, 8]: state["actors"][slot]["counter"] = 0
	for slot: int in [0, 8]: state["actors"][slot]["weapon"] = -1
	for slot: int in [0, 8]: state["actors"][slot]["auxiliary"] = -1
	state["actors"][0]["argument_x"] = 4
	state["actors"][0]["argument_y"] = 3
	state["actors"][0]["argument_z"] = 0
	state["actors"][0]["stats"][1] = 255
	state["actors"][8]["stats"][1] = 128
	reset_ship_state(state)
	state["bonus_popup_timer"] = 0
	state["bonus_popup_value"] = 0
	return outcome()

static func settings_error(state: Dictionary) -> String:
	if not state.get("settings") is Dictionary:
		return "native shooter settings are missing"
	for key: String in ["roll_button", "shot_button", "bomb_button", "auto_shot"]:
		if not state["settings"].get(key) is int or state["settings"][key] < 0 or state["settings"][key] > 255:
			return "native shooter setting must be a byte: " + key
	return ""

static func update_player(state: Dictionary, held: int, pressed: int) -> Dictionary:
	var error: String = actor_error(state)
	if error.is_empty(): error = settings_error(state)
	if not error.is_empty(): return outcome(0, error)
	held = Math.u32(held)
	pressed = Math.u32(pressed)
	var actor: Dictionary = state["actors"][0]
	var wingman: Dictionary = state["actors"][8]
	var step: int = 45 if (held & 0xa000) != 0 and (held & 0x5000) != 0 else 32
	if held & 0x8000:
		actor["x"] = Math.s16(actor["x"] + (Math.divmod32(step * (4096 - actor["x"]), 2048).quotient if actor["x"] > 2048 else step))
	if held & 0x2000:
		actor["x"] = Math.s16(actor["x"] + (Math.divmod32(step * (-4096 - actor["x"]), 2048).quotient if actor["x"] < -2048 else -step))
	if held & 0x1000:
		actor["y"] = Math.s16(actor["y"] + (Math.divmod32(step * (-2048 - actor["y"]), 256).quotient if actor["y"] < -1792 else -step))
	if held & 0x4000:
		actor["y"] = Math.s16(actor["y"] + (Math.divmod32(step * (384 - actor["y"]), 256).quotient if actor["y"] > 128 else step))
	actor["x"] = clampi(actor["x"], -4096, 4096)
	actor["y"] = clampi(actor["y"], -2048, 384)
	var left_mask: int = [5, 4, 1][state["settings"]["roll_button"] % 3]
	var right_mask: int = [10, 8, 2][state["settings"]["roll_button"] % 3]
	if actor["phase"] == 0:
		if pressed & left_mask:
			wingman["mode"] = Math.u8(wingman["mode"] + 1)
			actor["mode"] = 0x81
			actor["phase"] = 127
		if pressed & right_mask:
			wingman["mode"] = Math.u8(wingman["mode"] + 1)
			actor["mode"] = 0x82
			actor["phase"] = 127
	for bit: int in [1, 2]:
		if actor["mode"] & bit:
			var phase: int = actor["phase"]
			actor["rotation_z"] = Math.s16(actor["rotation_z"] + phase * (1 if bit == 1 else -1))
			actor["x"] = Math.s16(actor["x"] + (8 if bit == 1 else -8))
			if phase < 15:
				if (bit == 1 and (actor["rotation_z"] & 4095) >= 4065) or (bit == 2 and (actor["rotation_z"] & 4095) < 31):
					actor["phase"] = 0
					actor["rotation_z"] = 0
					actor["mode"] = 0
			else:
				actor["phase"] = Math.u8(actor["phase"] - 1)
				if actor["phase"] < 64: actor["mode"] &= 127
	if actor["mode"] & 4:
		var counter: int = actor["counter"]
		actor["counter"] = Math.s16(counter - 1)
		if counter in [2, 10, 14, 15]:
			state["effects"].append({"kind":"prim_flash_color_wide", "rgb":0x800000})
		elif counter == 0: actor["mode"] &= 0xfb
	return outcome()

static func shot_input_flags(state: Dictionary, held: int, pressed: int) -> Dictionary:
	var error: String = actor_error(state)
	if error.is_empty(): error = settings_error(state)
	if not error.is_empty(): return outcome(0, error)
	if state["actors"][0]["mode"] & 0x40:
		return outcome(0, "source battle mode0x40 passes an uninitialized bomb flag; native witness required")
	if not state.get("frame_count") is int:
		return outcome(0, "native frame_count is missing")
	var settings: Dictionary = state["settings"]
	var shot_mask: int = [0x20, 0x40, 0x80][settings["shot_button"] % 3]
	var bomb_mask: int = [0x40, 0x80, 0x20][settings["bomb_button"] % 3]
	var flags: int = 0
	if settings["auto_shot"] != 0:
		if (held & shot_mask) != 0 and (state["frame_count"] & 3) == 0: flags = 0x20
	elif pressed & shot_mask: flags = 0x20
	if pressed & bomb_mask: flags |= 0x40
	return outcome(flags)

static func update_wingman(_state: Dictionary) -> Dictionary:
	return outcome(0, "shooter_update_wingman requires exact target counting, func_800AA460 normalization/GTE, secondary fire and RNG schedule")
