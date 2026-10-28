extends RefCounted
## T-29024. Exact collision-only mutators over a PRIVATE validated working copy.
## Sources: shooter_3d/collision.c, enemy_bullets_and_pickups.c and
## hit_and_explosion_effects.c::shooter_spawn_hit_spark. No death/item/bomb driver.

const Math = preload("res://core/native_math.gd")
const SHOT_POOLS: Array[String] = ["player_primary", "player_secondary", "wingman_primary", "wingman_secondary"]

static func _integer(record: Dictionary, key: String, low: int, high: int) -> bool:
	return record.get(key) is int and record[key] >= low and record[key] <= high

static func _actors_error(state: Dictionary) -> String:
	if not state.get("effects") is Array or not state.get("actors") is Array or state["actors"].size() != 79:
		return "collision needs ordered effects and exactly79 native actor records"
	for value: Variant in state["actors"]:
		if not value is Dictionary:
			return "collision actor is not a primitive record"
		var actor: Dictionary = value
		for key: String in ["x", "y", "z", "counter"]:
			if not _integer(actor, key, -32768, 32767):
				return "collision actor needs native signed16: " + key
		if not _integer(actor, "mode", 0, 255):
			return "collision actor mode must be a native byte"
		for key: String in ["flag_bytes", "stats"]:
			var count: int = 2 if key == "flag_bytes" else 4
			if not actor.get(key) is Array or actor[key].size() != count:
				return "collision actor byte-array extent differs: " + key
			for item: Variant in actor[key]:
				if not item is int or item < 0 or item > 255:
					return "collision actor contains a non-native byte: " + key
	return ""

static func _pool_error(state: Dictionary, key: String, moving: bool = false, shots: bool = false) -> String:
	if not state.get(key) is Array or state[key].size() != 16:
		return "collision needs exactly16 particle records: " + key
	for value: Variant in state[key]:
		if not value is Dictionary:
			return "particle must be a primitive record: " + key
		var particle: Dictionary = value
		var words: Array[String] = ["x", "y", "z"]
		if moving:
			words.append_array(["velocity_x", "velocity_y", "velocity_z"])
		for word: String in words:
			if not _integer(particle, word, -32768, 32767):
				return "particle needs native signed16: " + key + "." + word
		for byte: String in ["flags", "age"]:
			if not _integer(particle, byte, 0, 255):
				return "particle needs a native byte: " + key + "." + byte
		if shots and (particle["flags"] & 0x80) != 0 and (particle["flags"] & 15) > 8:
			return "active shot kind exceeds the source nine-byte damage table: " + key
	return ""

static func _sparks_error(state: Dictionary) -> String:
	if not state.get("effects") is Array or not state.get("hit_sparks") is Array or state["hit_sparks"].size() != 16:
		return "hit-spark spawn needs ordered effects and16 ShooterParticleView records"
	for value: Variant in state["hit_sparks"]:
		if not value is Dictionary:
			return "hit spark must be a primitive record"
		var spark: Dictionary = value
		for key: String in ["position", "velocity"]:
			if not spark.get(key) is Dictionary:
				return "hit spark needs source ShooterPosition: " + key
			for word: String in ["x", "y", "z", "w"]:
				if not _integer(spark[key], word, -32768, 32767):
					return "hit-spark position/velocity must contain native signed16 words"
		for byte: String in ["unused_byte", "image_index", "age", "flags"]:
			if not _integer(spark, byte, 0, 255):
				return "hit spark needs native byte: " + byte
	if state["hit_sparks"][0]["position"]["w"] < 0:
		return "negative native hit-spark ring index would address before the pool"
	return ""

static func _shots_error(state: Dictionary, pool: String, tables: Dictionary) -> String:
	var error: String = _actors_error(state)
	if not error.is_empty(): return error
	error = _pool_error(state, pool, false, true)
	if not error.is_empty(): return error
	error = _sparks_error(state)
	if not error.is_empty(): return error
	if not _integer(state, "score", -2147483648, 2147483647) or not _integer(state, "frame_count", 0, 4294967295) or not _integer(state, "mode_stage", 0, 255):
		return "shot collision needs native score/frame_count/mode_stage words"
	for key: String in ["damage_by_kind", "score_by_class"]:
		var count: int = 9 if key == "damage_by_kind" else 17
		if not tables.get(key) is Array or tables[key].size() != count:
			return "external collision table has wrong extent: " + key
		for value: Variant in tables[key]:
			var low: int = 0 if key == "damage_by_kind" else -2147483648
			var high: int = 255 if key == "damage_by_kind" else 2147483647
			if not value is int or value < low or value > high:
				return "external collision table exceeds its source width: " + key
	return ""

static func _sound(state: Dictionary, code: int, play_se: Callable) -> void:
	state["effects"].append({"kind": "shooter_play_se", "arg0": 1, "arg1": code})
	if play_se.is_valid():
		play_se.call(state, 1, code)

static func _spark(state: Dictionary, x: int, y: int, z: int) -> void:
	# The source's ring intentionally repeats slots0/1 at counters16/17.
	var pool: Array = state["hit_sparks"]
	var old: int = pool[0]["position"]["w"]
	var index: int = Math.divmod32(old, 16).remainder
	var spark: Dictionary = pool[index]
	spark["position"]["x"] = Math.s16(x)
	spark["position"]["y"] = Math.s16(y)
	spark["position"]["z"] = Math.s16(z)
	spark["age"] = 1
	spark["image_index"] = 1
	spark["flags"] = 0x80
	pool[0]["position"]["w"] = 0 if old >= 17 else Math.s16(old + 1)
	state["effects"].append({"kind": "shooter_spawn_hit_spark", "index": index,
		"x": Math.s16(x), "y": Math.s16(y), "z": Math.s16(z)})

static func spawn_hit_spark(state: Dictionary, x: int, y: int, z: int) -> String:
	var error: String = _sparks_error(state)
	if not error.is_empty(): return error
	_spark(state, x, y, z)
	return ""

static func _ships_bullets(state: Dictionary, play_se: Callable) -> void:
	state["hit_sound_played"] = 0
	for ship: int in [0, 8]:
		var actor: Dictionary = state["actors"][ship]
		if (actor["mode"] & 0x80) != 0: continue
		for bullet: Dictionary in state["enemy_bullets"]:
			if (bullet["flags"] & 0x80) == 0: continue
			if absi(bullet["z"] - actor["z"]) < 256 and absi(bullet["y"] - actor["y"]) < 128 and absi(bullet["x"] - actor["x"]) < 128:
				bullet["flags"] = 0
				if ship == 0:
					_sound(state, 0x506, play_se)
					actor["mode"] = Math.u8(actor["mode"] | 4)
				actor["stats"][1] = actor["stats"][1] - 4 if actor["stats"][1] >= 5 else 0
				actor["counter"] = 16
				break

static func hit_ships_with_bullets(state: Dictionary, play_se: Callable = Callable()) -> String:
	var error: String = _actors_error(state)
	if not error.is_empty(): return error
	error = _pool_error(state, "enemy_bullets")
	if not error.is_empty(): return error
	_ships_bullets(state, play_se)
	return ""

static func _ships_bodies(state: Dictionary, play_se: Callable) -> void:
	state["hit_sound_played"] = 0
	for ship: int in [0, 8]:
		var actor: Dictionary = state["actors"][ship]
		if (actor["mode"] & 0x80) != 0: continue
		# Native wingman scans actors0..15, including itself. Do not correct it.
		for index: int in (range(16, 63) if ship == 0 else range(16)):
			var enemy: Dictionary = state["actors"][index]
			if (enemy["flag_bytes"][1] & 0xc0) == 0 or enemy["stats"][1] == 0: continue
			var limit: int = enemy["stats"][0] << 4
			if absi(enemy["z"] - actor["z"]) < 256 and absi(enemy["y"] - actor["y"]) < limit and absi(enemy["x"] - actor["x"]) < limit:
				if ship == 0:
					_sound(state, 0x506, play_se)
					actor["mode"] = Math.u8(actor["mode"] | 4)
				actor["stats"][1] = actor["stats"][1] - 12 if actor["stats"][1] >= 13 else 0
				actor["counter"] = 16
				break

static func hit_ships_with_bodies(state: Dictionary, play_se: Callable = Callable()) -> String:
	var error: String = _actors_error(state)
	if not error.is_empty(): return error
	_ships_bodies(state, play_se)
	return ""

static func _enemy_active(enemy: Dictionary) -> bool:
	return (enemy["flag_bytes"][1] & 0x80) != 0 and (enemy["stats"][3] & 1) != 0 and enemy["stats"][1] != 0 and enemy["stats"][0] != 0

static func _score_class(state: Dictionary, enemy: Dictionary, tables: Dictionary) -> int:
	match state["mode_stage"]:
		0x0c: return 400
		0x16: return 200
		0x20: return 800
	# Native copies signed external words into a u32 local scores[256], then
	# reads only indices stats[2]%17. No other copied local element is observed.
	return Math.u32(tables["score_by_class"][enemy["stats"][2] % 17])

static func _enemies_shots(state: Dictionary, pool_name: String, wingman_shots: int, tables: Dictionary, play_se: Callable) -> void:
	var pool: Array = state[pool_name]
	state["hit_sound_played"] = 0
	if (pool[0]["flags"] & 0x20) != 0 and (state["frame_count"] & 1) != 0:
		for index: int in range(2):
			var shot: Dictionary = pool[index]
			var z: int = state["actors"][0 if wingman_shots == 0 else 8]["z"]
			for enemy_index: int in range(16, 63):
				var enemy: Dictionary = state["actors"][enemy_index]
				if not _enemy_active(enemy): continue
				var radius: int = enemy["stats"][0] * 16
				if absi(shot["x"] - enemy["x"]) < radius + 24 and absi(shot["y"] - enemy["y"]) < radius + 24 and enemy["z"] < z:
					if state["hit_sound_played"] == 0 and wingman_shots == 0:
						state["hit_sound_played"] = 1
						_sound(state, 0x507, play_se)
					enemy["stats"][1] = Math.u8(enemy["stats"][1] - 1)
					_spark(state, shot["x"], shot["y"], enemy["z"] - 512)
					var score: int = _score_class(state, enemy, tables)
					state["score"] = Math.add32(state["score"], Math.divmodu32(score, 20).quotient)
					if enemy["stats"][1] == 0:
						state["score"] = Math.add32(state["score"], score)
	for shot: Dictionary in pool:
		var kind: int = shot["flags"] & 15
		if (shot["flags"] & 0x80) == 0 or kind == 0: continue
		var damage: int = tables["damage_by_kind"][kind]
		var width: int = 96 if kind == 8 else 48
		for enemy_index: int in range(16, 63):
			var enemy: Dictionary = state["actors"][enemy_index]
			if not _enemy_active(enemy): continue
			var radius: int = enemy["stats"][0] * 16
			if absi(shot["z"] - enemy["z"]) < radius + 256 and absi(shot["x"] - enemy["x"]) < radius + width and absi(shot["y"] - enemy["y"]) < radius + width:
				if state["hit_sound_played"] == 0 and wingman_shots == 0:
					state["hit_sound_played"] = 1
					_sound(state, 0x507, play_se)
				enemy["stats"][1] = Math.u8(enemy["stats"][1] - damage) if damage < enemy["stats"][1] else 0
				if enemy["stats"][1] == 0:
					enemy["stats"][3] &= 0xf0
				shot["age"] = 0
				shot["flags"] = 0
				_spark(state, Math.divmod32(enemy["x"] + shot["x"], 2).quotient, Math.divmod32(enemy["y"] + shot["y"], 2).quotient, shot["z"] - 512)
				var score: int = _score_class(state, enemy, tables)
				state["score"] = Math.add32(state["score"], Math.u32(Math.divmodu32(score, 10).quotient * damage))
				if enemy["stats"][1] == 0:
					_sound(state, 0x508, play_se)
					state["score"] = Math.add32(state["score"], score)
				break

static func hit_enemies_with_shots(state: Dictionary, pool_name: String, _unused_arg1: int, wingman_shots: int, tables: Dictionary, play_se: Callable = Callable()) -> String:
	if pool_name not in SHOT_POOLS:
		return "collision shot pool is outside the four source player/wingman pools"
	var error: String = _shots_error(state, pool_name, tables)
	if not error.is_empty(): return error
	_enemies_shots(state, pool_name, wingman_shots, tables, play_se)
	return ""

static func run_collisions(state: Dictionary, tables: Dictionary, play_se: Callable = Callable()) -> String:
	var error: String = _actors_error(state)
	if not error.is_empty(): return error
	error = _pool_error(state, "enemy_bullets")
	if not error.is_empty(): return error
	if not _integer(state, "mode_stage", 0, 255) or not _integer(state, "stages_cleared", 0, 255):
		return "collision driver needs native stage and cleared-stage bytes"
	var skip_shots: bool = (state["mode_stage"] == 12 and state["stages_cleared"] != 0) or (state["mode_stage"] == 22 and state["stages_cleared"] >= 2) or (state["mode_stage"] == 32 and state["stages_cleared"] >= 3)
	if not skip_shots:
		for pool: String in SHOT_POOLS:
			error = _shots_error(state, pool, tables)
			if not error.is_empty(): return error
	_ships_bullets(state, play_se)
	_ships_bodies(state, play_se)
	if skip_shots: return ""
	_enemies_shots(state, "player_primary", 0, tables, play_se)
	_enemies_shots(state, "player_secondary", 0, tables, play_se)
	_enemies_shots(state, "wingman_primary", 1, tables, play_se)
	_enemies_shots(state, "wingman_secondary", 1, tables, play_se)
	state["score"] = Math.add32(Math.divmod32(state["score"], 10).quotient * 10, 1)
	if state["score"] >= 10000000:
		state["score"] = 9999999
	return ""

static func update_enemy_bullets(state: Dictionary) -> String:
	if not state.get("effects") is Array:
		return "enemy-bullet update needs ordered effects"
	var error: String = _pool_error(state, "enemy_bullets", true)
	if not error.is_empty(): return error
	for particle: Dictionary in state["enemy_bullets"]:
		particle["age"] = Math.u8(particle["age"] + 1)
		if particle["age"] == 255 or particle["z"] >= 10001:
			particle["flags"] = 0
		if (particle["flags"] & 0x80) != 0:
			for axis: String in ["x", "y", "z"]:
				particle[axis] = Math.s16(particle[axis] + particle["velocity_" + axis])
	for index: int in range(16):
		if (state["enemy_bullets"][index]["flags"] & 0x80) != 0:
			state["effects"].append({"kind": "shooter_draw_enemy_bullet", "index": index})
	return ""
