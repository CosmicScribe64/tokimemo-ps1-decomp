extends RefCounted
## T-29024. primary_weapons.c pool state and reached sound_requests.c effects.
## Draw intents require the exact GTE/render backend; no pixel parity implied.

const Math = preload("res://core/native_math.gd")
const Rng = preload("res://core/native_rng.gd")
const Player = preload("res://overlays/shooter_player_rules.gd")
const Sound = preload("res://overlays/shooter_sound_rules.gd")
const POOLS: Array[String] = ["player_primary", "wingman_primary"]

static func pool_error(state: Dictionary, name: String) -> String:
	if not state.get(name) is Array or state[name].size() != 16:
		return "native shooter pool needs16 records: " + name
	for raw: Variant in state[name]:
		if not raw is Dictionary: return "native shooter particle must be a primitive record"
		var particle: Dictionary = raw
		for key: String in ["x", "y", "z", "velocity_x", "velocity_y", "velocity_z"]:
			if not particle.get(key) is int or particle[key] < -32768 or particle[key] > 32767:
				return "native particle signed16 field is invalid: " + key
		for key: String in ["flags", "age"]:
			if not particle.get(key) is int or particle[key] < 0 or particle[key] > 255:
				return "native particle byte field is invalid: " + key
	return ""

static func _preflight(state: Dictionary, ship: int) -> String:
	if ship not in [0, 1]: return "primary weapon ship selector must be native0/1"
	var error: String = Player.actor_error(state)
	return pool_error(state, POOLS[ship]) if error.is_empty() else error

static func fire_vulcan(state: Dictionary, ship: int, flags: int, tables: Dictionary = {}, rng: Rng = null) -> Dictionary:
	return _fire(state, ship, flags, 0, tables, rng)

static func fire_wide(state: Dictionary, ship: int, flags: int, tables: Dictionary = {}, rng: Rng = null) -> Dictionary:
	return _fire(state, ship, flags, 1, tables, rng)

static func fire_beam(state: Dictionary, ship: int, flags: int, tables: Dictionary = {}, rng: Rng = null) -> Dictionary:
	return _fire(state, ship, flags, 2, tables, rng)

static func _fire(state: Dictionary, ship: int, flags: int, weapon: int, tables: Dictionary, rng: Rng) -> Dictionary:
	var error: String = _preflight(state, ship)
	if not error.is_empty(): return Player.outcome(0, error)
	if (flags & 0x20) == 0: return Player.outcome()
	var pool: Array = state[POOLS[ship]]
	var index: int = -1
	for slot: int in range(16):
		if (pool[slot]["flags"] & 128) == 0:
			if index == -1: index = slot
		elif weapon == 2: return Player.outcome(-1)
	if index == -1: return Player.outcome(-1)
	if ship == 0:
		var sound: Dictionary = Sound.play_se(state, 1, [0x500, 0x502, 0x501][weapon], tables, rng)
		if not sound["error"].is_empty(): return Player.outcome(0, sound["error"])
	var actor: Dictionary = state["actors"][ship * 8]
	if weapon == 2:
		var particle: Dictionary = pool[0]
		particle["flags"] = 0xb0
		for slot: int in [0, 1]:
			pool[slot]["x"] = actor["x"]
			pool[slot]["y"] = Math.s16(actor["y"] + 32)
			pool[slot]["z"] = actor["z"]
		particle["velocity_x"] = 0
		particle["velocity_y"] = 0
		particle["velocity_z"] = 0
		particle["age"] = 0
	else:
		var particle: Dictionary = pool[index]
		particle["flags"] = 0x94 if weapon == 0 else 0x98
		particle["x"] = Math.s16(actor["x"] + ((index & 1) << 7) - 64)
		particle["y"] = Math.s16(actor["y"] - (ship << (7 if weapon == 0 else 6)) + (128 if weapon == 0 else 64))
		particle["z"] = actor["z"]
		particle["velocity_x"] = 0
		particle["velocity_y"] = 0
		particle["velocity_z"] = -512
		particle["age"] = 0
	return Player.outcome(1)

static func update_primary(state: Dictionary, ship: int) -> Dictionary:
	var error: String = _preflight(state, ship)
	if not error.is_empty(): return Player.outcome(0, error)
	var pool: Array = state[POOLS[ship]]
	for index: int in range(16):
		var particle: Dictionary = pool[index]
		particle["age"] = Math.u8(particle["age"] + 1)
		if particle["age"] == 255: particle["flags"] = 0
		if particle["z"] < -32000: particle["flags"] = 0
		var kind: int = particle["flags"] & 0x2f
		if kind in [4, 8] and (particle["flags"] & 128) != 0:
			for axis: String in ["x", "y", "z"]:
				particle[axis] = Math.s16(particle[axis] + particle["velocity_" + axis])
			state["effects"].append({"kind":"shooter_draw_vulcan_shot" if kind == 4 else "shooter_draw_wide_shot",
				"pool":POOLS[ship], "index":index, "particle":particle.duplicate(true)})
	if (pool[0]["flags"] & 0xa0) == 0xa0:
		pool[0]["age"] = Math.u8(pool[0]["age"] + 1)
		if pool[0]["age"] >= 3:
			var actor: Dictionary = state["actors"][ship * 8]
			pool[0]["x"] = Math.s16(actor["x"] + 64)
			pool[1]["x"] = Math.s16(actor["x"] - 64)
			for slot: int in [0, 1]:
				pool[slot]["y"] = Math.s16(actor["y"] + 32)
				pool[slot]["z"] = actor["z"]
			state["effects"].append({"kind":"shooter_draw_beam", "pool":POOLS[ship], "ship_index":ship,
				"particles":[pool[0].duplicate(true), pool[1].duplicate(true)]})
		if pool[0]["age"] >= 129:
			pool[0]["flags"] = 0
			pool[1]["flags"] = 0
	return Player.outcome()

static func update_primary_all(state: Dictionary) -> Dictionary:
	for ship: int in [0, 1]:
		var error: String = _preflight(state, ship)
		if not error.is_empty(): return Player.outcome(0, error)
	update_primary(state, 0)
	return update_primary(state, 1)

static func clear_beam_flags(state: Dictionary, ship: int) -> Dictionary:
	var error: String = _preflight(state, ship)
	if not error.is_empty(): return Player.outcome(0, error)
	for particle: Dictionary in state[POOLS[ship]]: particle["flags"] &= 0xdf
	return Player.outcome()

static func clear_player_shots(state: Dictionary) -> Dictionary:
	for name: String in ["player_primary", "wingman_primary", "player_secondary", "wingman_secondary", "bomb_shots"]:
		var error: String = pool_error(state, name)
		if not error.is_empty(): return Player.outcome(0, error)
	state["pulse_bomb_active"] = 0
	state["pulse_bomb_timer"] = 0
	for index: int in range(16):
		for name: String in ["player_primary", "wingman_primary", "player_secondary", "wingman_secondary", "bomb_shots"]:
			state[name][index]["flags"] = 0
	return Player.outcome()

static func clear_enemy_bullets(state: Dictionary) -> Dictionary:
	var error: String = pool_error(state, "enemy_bullets")
	if not error.is_empty(): return Player.outcome(0, error)
	for particle: Dictionary in state["enemy_bullets"]: particle["flags"] = 0
	return Player.outcome()
