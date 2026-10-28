extends RefCounted
## T-29024. hit_and_explosion_effects.c pool updates, including unconditional RNG.
const Math = preload("res://core/native_math.gd")
const Rng = preload("res://core/native_rng.gd")

static func validate(state: Dictionary, key: String) -> String:
	if not state.get("effects") is Array or not state.get(key) is Array or state[key].size() != 16:
		return "native effect update requires sixteen records and ordered effects: " + key
	for raw: Variant in state[key]:
		if not raw is Dictionary: return "native effect record is malformed"
		for vector: String in ["position", "velocity"]:
			if not raw.get(vector) is Dictionary: return "native effect vector is missing"
			for axis: String in ["x", "y", "z", "w"]:
				if not raw[vector].get(axis) is int or raw[vector][axis] < -32768 or raw[vector][axis] > 32767: return "native effect vector exceeds signed16"
		for byte: String in ["unused_byte", "image_index", "age", "flags"]:
			if not raw.get(byte) is int or raw[byte] < 0 or raw[byte] > 255: return "native effect field exceeds byte"
	return ""

static func _age(particle: Dictionary) -> void:
	var age: int = particle["age"]
	particle["age"] = Math.u8(age + 1)
	if age >= 5:
		particle["age"] = 0
		var image: int = particle["image_index"]
		particle["image_index"] = Math.u8(image + 1)
		if image >= 7:
			particle["flags"] = 0
			particle["image_index"] = 0

static func update_hit_sparks(state: Dictionary) -> String:
	var error: String = validate(state, "hit_sparks")
	if not error.is_empty(): return error
	for particle: Dictionary in state["hit_sparks"]:
		if particle["flags"] != 0:
			var position: Dictionary = particle["position"]
			state["effects"].append({"kind":"shooter_draw_hit_spark",
				"args":[position["x"], position["y"], position["z"], particle["image_index"] - 1]})
			_age(particle)
	return ""

static func update_explosions(state: Dictionary, rng: Rng) -> String:
	var error: String = validate(state, "explosions")
	if not error.is_empty(): return error
	if rng == null or not rng.configuration_error().is_empty(): return "native explosion update requires configured RNG"
	# Both rand() calls happen even with all sixteen slots inactive.
	var offset_x: int = rng.next("src/ovl/shooter_3d/hit_and_explosion_effects.c::shooter_update_explosions:offset_x") & 127
	var offset_y: int = rng.next("src/ovl/shooter_3d/hit_and_explosion_effects.c::shooter_update_explosions:offset_y") & 63
	for particle: Dictionary in state["explosions"]:
		if particle["flags"] != 0:
			var position: Dictionary = particle["position"]
			state["effects"].append({"kind":"shooter_draw_explosion_sprite",
				"args":[position["x"], position["y"], position["z"], particle["image_index"] - 1]})
			state["effects"].append({"kind":"shooter_draw_explosion_sprite",
				"args":[Math.s16(position["x"] + offset_x), Math.s16(position["y"] + offset_y), position["z"], particle["image_index"] + 2]})
			for axis: String in ["x", "y", "z"]:
				position[axis] = Math.s16(position[axis] + particle["velocity"][axis])
			_age(particle)
	return ""
