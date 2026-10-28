extends RefCounted
## T-29024. Complete ordinary stage-3 script from swarm_and_orbit_waves.c.
## Providers execute native enemy/spawn behavior, not simplified wave counters.
const Math = preload("res://core/native_math.gd")
const Pool = preload("res://overlays/shooter_enemy_pool_rules.gd")
const Lifecycle = preload("res://overlays/shooter_lifecycle_rules.gd")
const Rng = preload("res://core/native_rng.gd")
const REQUIRED: Array[String] = ["shooter_update_enemies", "shooter_spawn_swarm_ship",
	"shooter_spawn_wall_ships", "shooter_spawn_guardian_ring"]

static func provider_error(providers: Dictionary) -> String:
	for name: String in REQUIRED:
		if not providers.get(name) is Callable or not providers[name].is_valid():
			return "required native stage-3 operation is not implemented: " + name
	return ""

static func invoke(providers: Dictionary, name: String, state: Dictionary, argument: int, rng: Rng) -> String:
	var result: Variant = providers[name].call(state, argument, rng)
	return result if result is String else "native stage-3 provider must return diagnostic String: " + name

static func update_script(state: Dictionary, providers: Dictionary, rng: Rng) -> String:
	var error: String = provider_error(providers)
	if not error.is_empty(): return error
	state["script_timer"] = Math.add32(state["script_timer"], 1)
	error = invoke(providers, "shooter_update_enemies", state, 0, rng)
	if not error.is_empty(): return error
	if Math.divmod32(state["script_timer"], 30).remainder == 0:
		if state["script_phase"] < 4:
			if Pool.count_kind(state, 2) < 10:
				error = invoke(providers, "shooter_spawn_swarm_ship", state, 0, rng)
		elif state["script_phase"] < 6 and Pool.count_kind(state, 2) < 3:
			error = invoke(providers, "shooter_spawn_swarm_ship", state, 0, rng)
	if not error.is_empty(): return error
	match state["script_phase"]:
		0:
			error = invoke(providers, "shooter_spawn_wall_ships", state, state["script_phase"], rng)
			if error.is_empty(): Pool.next_phase(state)
		1, 2, 3:
			if Pool.count_kind(state, 3) == 0:
				error = invoke(providers, "shooter_spawn_wall_ships", state, state["script_phase"], rng)
				if error.is_empty(): Pool.next_phase(state)
		4:
			if Pool.count_kind(state, 3) == 0: Pool.next_phase(state)
		5, 6:
			Pool.next_phase(state)
		7, 8, 9, 10:
			if Pool.count_active(state) == 0:
				error = invoke(providers, "shooter_spawn_guardian_ring", state, state["script_phase"] - 3, rng)
				if error.is_empty(): Pool.next_phase(state)
		11:
			if Pool.count_kind(state, 0xff) == 0: Pool.next_phase(state)
		12:
			if Pool.count_active(state) == 0:
				Pool.reset_enemies(state)
				Lifecycle.next_step(state)
	return error
