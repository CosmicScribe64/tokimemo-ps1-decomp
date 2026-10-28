extends RefCounted
## T-29024. Validate every field owned by this combat projection before commit.
## This is not a complete TACO/global GameState serialization schema.
const Lifecycle = preload("res://overlays/shooter_lifecycle_rules.gd")
const Player = preload("res://overlays/shooter_player_rules.gd")
const Effects = preload("res://overlays/shooter_effects_rules.gd")
const POOLS: Array[String] = ["player_primary", "wingman_primary", "player_secondary",
	"wingman_secondary", "bomb_shots", "enemy_bullets", "items"]
const PARTICLE_WORDS: Array[String] = ["x", "y", "z", "w", "velocity_x", "velocity_y", "velocity_z", "unused_word"]

static func _integer(record: Dictionary, key: String, low: int, high: int) -> bool:
	return record.get(key) is int and record[key] >= low and record[key] <= high

static func _bytes(record: Dictionary, key: String, count: int) -> bool:
	if not record.get(key) is Array or record[key].size() != count: return false
	for value: Variant in record[key]:
		if not value is int or value < 0 or value > 255: return false
	return true

static func validate(state: Dictionary) -> String:
	var error: String = Lifecycle.validate(state)
	if not error.is_empty(): return error
	for key: String in ["hit_sound_played", "background_flags", "bonus_popup_value", "pulse_bomb_active", "pulse_bomb_timer"]:
		if not _integer(state, key, -2147483648, 2147483647): return "native combat signed32 field invalid: " + key
	if not _integer(state, "bonus_popup_timer", -128, 127): return "native bonus popup timer exceeds signed8"
	if not _bytes(state, "se_cooldowns", 10) or not _bytes(state, "clear_color", 3): return "native combat cooldown/color byte records invalid"
	if not state.get("settings") is Dictionary: return "native shooter settings are missing"
	for key: String in ["roll_button", "shot_button", "bomb_button", "auto_shot"]:
		if not _integer(state["settings"], key, 0, 255): return "native shooter setting exceeds byte: " + key
	for raw: Variant in state["actors"]:
		if not raw is Dictionary: return "native actor must be a plain record"
		for key: String in Player.ACTOR_WORDS:
			if not _integer(raw, key, -32768, 32767): return "native actor signed16 field invalid: " + key
		for key: String in ["mode", "phase"]:
			if not _integer(raw, key, 0, 255): return "native actor byte field invalid: " + key
		if not _bytes(raw, "flag_bytes", 2) or not _bytes(raw, "stats", 4): return "native actor byte arrays invalid"
		if raw.has("model"):
			var model: Variant = raw["model"]
			if not model is Dictionary or not _integer(model, "attribute", -2147483648, 2147483647) or not model.get("links") is Array or model["links"].size() != 3: return "native model-handle projection invalid"
			for link: Variant in model["links"]:
				if not link is int or link < -2147483648 or link > 2147483647: return "native model link exceeds signed32"
	for name: String in POOLS:
		if not state.get(name) is Array or state[name].size() != 16: return "native combat pool extent invalid: " + name
		for particle: Variant in state[name]:
			if not particle is Dictionary: return "native particle must be a plain record"
			for key: String in PARTICLE_WORDS:
				if not _integer(particle, key, -32768, 32767): return "native particle signed16 field invalid: " + key
			for key: String in ["unused_byte", "argument", "age", "flags"]:
				if not _integer(particle, key, 0, 255): return "native particle byte field invalid: " + key
	for name: String in ["hit_sparks", "explosions"]:
		error = Effects.validate(state, name)
		if not error.is_empty(): return error
	if not state.get("model_slots") is Array or state["model_slots"].size() != 16: return "native model slots missing"
	for attribute: Variant in state["model_slots"]:
		if not attribute is int or attribute < -2147483648 or attribute > 2147483647: return "native model slot exceeds signed32"
	if state.has("result_values"):
		if not state["result_values"] is Array or state["result_values"].size() != 5: return "native result-value extent invalid"
		for value: Variant in state["result_values"]:
			if not value is int or value < -2147483648 or value > 2147483647: return "native result value exceeds signed32"
	return ""
