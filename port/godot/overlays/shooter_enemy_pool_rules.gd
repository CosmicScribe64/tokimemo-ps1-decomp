extends RefCounted
## T-29024. Native pool bookkeeping, enemy_pool_and_keyframes.c and model_3d.c.
## Call inside a validated transaction. Coordinate/model contents remain owned
## by their native binding provider; this module owns attributes and owner bytes.
const Math = preload("res://core/native_math.gd")
const Player = preload("res://overlays/shooter_player_rules.gd")
const HIDDEN: int = -2147483648
const SCRIPT_FIELDS: Array[String] = ["script_timer", "script_phase",
	"script_sub_timer", "script_counter", "circler_pairing"]

static func validate(state: Dictionary) -> String:
	if not state.get("actors") is Array or state["actors"].size() != 79:
		return "native enemy pool requires 79 actor records"
	for i: int in range(16, 63):
		if not state["actors"][i] is Dictionary or not state["actors"][i].get("model") is Dictionary:
			return "native enemy pool requires actor model handles"
	if not state.get("coordinate_owner") is Array or state["coordinate_owner"].size() != 64:
		return "TACO requires all 64 native coordinate-owner bytes"
	if not state.get("joint_models") is Array or state["joint_models"].size() != 64:
		return "TACO requires all 64 native joint-model handles"
	for i: int in range(64):
		var owner: Variant = state["coordinate_owner"][i]
		if not owner is int or owner < 0 or owner > 255: return "native coordinate owner exceeds unsigned8"
		var model: Variant = state["joint_models"][i]
		if not model is Dictionary or not model.get("attribute") is int or model["attribute"] < -2147483648 or model["attribute"] > 2147483647:
			return "native joint-model attribute exceeds signed32"
	for key: String in SCRIPT_FIELDS:
		if not state.get(key) is int or state[key] < -2147483648 or state[key] > 2147483647:
			return "native script field exceeds signed32: " + key
	return ""

static func next_phase(state: Dictionary) -> void:
	state["script_timer"] = 0
	state["script_phase"] = Math.add32(state["script_phase"], 1)
	state["circler_pairing"] = 0

static func next_subphase(state: Dictionary) -> void:
	state["script_sub_timer"] = 0
	state["script_counter"] = Math.add32(state["script_counter"], 1)

static func count_free_coordinates(state: Dictionary) -> int:
	var count: int = 0
	for owner: int in state["coordinate_owner"]:
		if owner == 0: count += 1
	return count

static func count_active(state: Dictionary) -> int:
	var count: int = 0
	for i: int in range(16, 63):
		if (state["actors"][i]["flag_bytes"][1] & 0x80) != 0: count += 1
	return count

static func count_kind(state: Dictionary, actor_kind: int) -> int:
	var count: int = 0
	for i: int in range(16, 63):
		var actor: Dictionary = state["actors"][i]
		# This source query also clears stale HP on inactive records.
		if actor["stats"][1] != 0 and (actor["flag_bytes"][1] & 0x80) == 0:
			actor["stats"][1] = 0
		if actor_kind == 0xff:
			if actor["stats"][1] != 0: count += 1
		elif actor_kind == actor["stats"][2] and actor["stats"][1] != 0:
			count += 1
	return count

static func set_visibility(state: Dictionary, visibility_mode: int, index: int) -> void:
	var actor: Dictionary = state["actors"][index]
	if (actor["flag_bytes"][1] & 0x80) == 0: return
	if visibility_mode == 1:
		if actor["stats"][2] < 11:
			actor["model"]["attribute"] = 0
			return
		actor["model"]["attribute"] = HIDDEN
		for i: int in range(64):
			if state["coordinate_owner"][i] == index: state["joint_models"][i]["attribute"] = 0
		return
	actor["model"]["attribute"] = HIDDEN
	if actor["stats"][2] < 9: return
	for i: int in range(64):
		if state["coordinate_owner"][i] == index: state["joint_models"][i]["attribute"] = HIDDEN

static func remove_enemy(state: Dictionary, index: int) -> void:
	set_visibility(state, 0, index)
	state["actors"][index]["flag_bytes"][1] = 0
	state["actors"][index]["stats"][2] = 0
	for i: int in range(64):
		if state["coordinate_owner"][i] == index: state["coordinate_owner"][i] = 0

static func reset_enemies(state: Dictionary) -> void:
	for i: int in range(64):
		state["coordinate_owner"][i] = 0
		state["joint_models"][i]["attribute"] = HIDDEN
	for i: int in range(16, 63):
		# shooter_release_actor_model -> view_model_record_init(2*i+2).
		# The model/coordinate and fourth stats byte survive that initializer.
		var actor: Dictionary = state["actors"][i]
		actor["flag_bytes"][0] = 1
		actor["flag_bytes"][1] = 0
		actor["mode"] = 0
		actor["phase"] = 0
		for key: String in Player.ACTOR_WORDS: actor[key] = 0
		for key: String in ["scale_x", "scale_y", "scale_z"]: actor[key] = 0x1000
		actor["stats"][0] = 0
		actor["stats"][1] = 1
		actor["stats"][2] = 1
		actor["model"]["attribute"] = HIDDEN
	for key: String in SCRIPT_FIELDS: state[key] = 0

static func zero_hit_points(state: Dictionary) -> void:
	for i: int in range(16, 63): state["actors"][i]["stats"][1] = 0
