extends RefCounted
## Invented public records. No original model, table or sprite payload.
const Player = preload("res://overlays/shooter_player_rules.gd")
const POOLS: Array[String] = ["player_primary", "wingman_primary", "player_secondary", "wingman_secondary", "bomb_shots", "enemy_bullets"]
const ACTOR_KEYS: Array[String] = ["rotation_x", "rotation_y", "rotation_z", "param", "scale_x", "scale_y", "scale_z", "weapon", "x", "y", "z", "counter", "argument_x", "argument_y", "argument_z", "auxiliary"]
const PARTICLE_KEYS: Array[String] = ["x", "y", "z", "w", "velocity_x", "velocity_y", "velocity_z", "unused_word", "unused_byte", "argument", "age", "flags"]

static func actor() -> Dictionary:
	var output: Dictionary = {"flag_bytes":[0, 0], "mode":0, "phase":0, "stats":[0, 0, 0, 0], "model":{"attribute":0, "links":[0, 0, 0]}}
	for key: String in ACTOR_KEYS: output[key] = 0
	return output

static func particle() -> Dictionary:
	var output: Dictionary = {}
	for key: String in PARTICLE_KEYS: output[key] = 0
	return output

static func state() -> Dictionary:
	var output: Dictionary = {"actors":[], "effects":[], "settings":{"roll_button":0, "shot_button":0, "bomb_button":0, "auto_shot":0},
		"frame_count":0, "background_flags":0, "se_cooldowns":[0,0,0,0,0,0,0,0,0,0],
		"bonus_popup_timer":0, "bonus_popup_value":0, "pulse_bomb_active":0, "pulse_bomb_timer":0}
	for index: int in range(79): output["actors"].append(actor())
	for key: String in POOLS:
		output[key] = []
		for index: int in range(16): output[key].append(particle())
	return output

static func vector(data: Dictionary, value: int, seed: int, draws: int) -> Array:
	var output: Array = [value, seed, draws, data["bonus_popup_timer"], data["bonus_popup_value"], data["pulse_bomb_active"], data["pulse_bomb_timer"]]
	output.append_array(data["se_cooldowns"])
	for index: int in [0, 8]:
		var ship: Dictionary = data["actors"][index]
		output.append_array(ship["flag_bytes"])
		output.append(ship["mode"]); output.append(ship["phase"])
		for key: String in ACTOR_KEYS: output.append(ship[key])
		output.append_array(ship["stats"])
		output.append(ship["model"]["attribute"]); output.append_array(ship["model"]["links"])
	for key: String in POOLS:
		for item: Dictionary in data[key]:
			for field: String in PARTICLE_KEYS: output.append(item[field])
	var effects: Array = []
	for effect: Dictionary in data["effects"]:
		match effect["kind"]:
			"audio_command": effects.append([1, effect["command"]])
			"prim_flash_color_wide": effects.append([2, effect["rgb"]])
			"shooter_init_actor_model": effects.append([3, effect["model_address"], effect["object_index"], effect["actor"]])
			"shooter_draw_vulcan_shot", "shooter_draw_wide_shot":
				var p: Dictionary = effect["particle"]
				effects.append([4 if effect["kind"] == "shooter_draw_vulcan_shot" else 5, POOLS.find(effect["pool"]), effect["index"], p["x"], p["y"], p["z"], p["age"], p["flags"], p["velocity_x"], p["velocity_y"], p["velocity_z"]])
			"shooter_draw_beam":
				var left: Dictionary = effect["particles"][0]
				var right: Dictionary = effect["particles"][1]
				effects.append([6, effect["ship_index"], left["x"], left["y"], left["z"], left["age"], left["flags"], right["x"], right["y"], right["z"], right["age"], right["flags"]])
	output.append(effects)
	return output
