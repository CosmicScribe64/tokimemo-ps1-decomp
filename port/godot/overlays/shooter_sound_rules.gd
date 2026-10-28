extends RefCounted
## T-29024. sound_requests.c::shooter_play_se/shooter_tick_se_cooldowns.
## These rules consume RNG even though actual audio transport is external.

const Math = preload("res://core/native_math.gd")
const Rng = preload("res://core/native_rng.gd")

static func _cooldowns_error(state: Dictionary) -> String:
	if not state.get("se_cooldowns") is Array or state["se_cooldowns"].size() != 10:
		return "source shooter sound requires ten cooldown bytes"
	for value: Variant in state["se_cooldowns"]:
		if not value is int or value < 0 or value > 255: return "source shooter cooldown exceeds byte width"
	return ""

static func validate(state: Dictionary, channel: int, command: int, tables: Dictionary, rng: Rng) -> String:
	if not state.get("background_flags") is int or state["background_flags"] < -2147483648 or state["background_flags"] > 2147483647 or not state.get("effects") is Array:
		return "source shooter sound requires native background flags and ordered effects"
	if channel < -2147483648 or channel > 2147483647 or command < -2147483648 or command > 2147483647:
		return "source shooter sound arguments exceed signed32"
	if state["background_flags"] == 0 or channel != 1 or (command & 255) >= 10: return ""
	var error: String = _cooldowns_error(state)
	if not error.is_empty(): return error
	# A throttled call returns before reading the original range or RNG.
	if state["se_cooldowns"][command & 255] != 0: return ""
	if not tables.get("se_cooldown_range") is Array or tables["se_cooldown_range"].size() != 10:
		return "source shooter sound requires validated external cooldown ranges"
	for value: Variant in tables["se_cooldown_range"]:
		if not value is int or value < 0 or value > 255: return "source shooter cooldown range exceeds byte width"
	if rng == null or not rng.configuration_error().is_empty(): return "source shooter sound requires configured native RNG"
	return ""

static func play_se(state: Dictionary, channel: int, command: int, tables: Dictionary, rng: Rng) -> Dictionary:
	var error: String = validate(state, channel, command, tables, rng)
	if not error.is_empty(): return {"error":error, "value":0, "suppressed":false}
	var index: int = command & 255
	if state["background_flags"] != 0 and channel == 1 and index < 10:
		if state["se_cooldowns"][index] != 0: return {"error":"", "value":0, "suppressed":true}
		var random_word: int = rng.next("src/ovl/shooter_3d/sound_requests.c::shooter_play_se")
		state["se_cooldowns"][index] = Math.u8(Math.u32(random_word) % (tables["se_cooldown_range"][index] * 2 + 1))
	state["effects"].append({"kind":"audio_command", "command":(index | (0x500 if (channel & 1) != 0 else 0x600)) & 65535})
	# The emitted arm has no defined C return; callers must ignore value.
	return {"error":"", "value":0, "suppressed":false}

static func tick(state: Dictionary) -> String:
	var error: String = _cooldowns_error(state)
	if not error.is_empty(): return error
	for index: int in range(10):
		if state["se_cooldowns"][index] != 0:
			state["se_cooldowns"][index] -= 1
	return ""
