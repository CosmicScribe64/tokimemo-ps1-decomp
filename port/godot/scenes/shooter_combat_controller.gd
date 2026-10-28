extends RefCounted
## T-29024. Transactional shooter_update_battle call order; not a stage substitute.
## Required provider functions must be actual source-backed rules, not fixtures.

const Player = preload("res://overlays/shooter_player_rules.gd")
const Primary = preload("res://overlays/shooter_primary_rules.gd")
const Collision = preload("res://overlays/shooter_collision_rules.gd")
const Sound = preload("res://overlays/shooter_sound_rules.gd")
const Effects = preload("res://overlays/shooter_effects_rules.gd")
const Lifecycle = preload("res://overlays/shooter_lifecycle_rules.gd")
const Rng = preload("res://core/native_rng.gd")
const State = preload("res://overlays/shooter_combat_state.gd")
const REQUIRED: Array[String] = ["shooter_update_wingman", "shooter_update_secondary_shots",
	"shooter_update_bomb", "shooter_collect_items", "shooter_update_items"]
var tables: Dictionary
var providers: Dictionary
const COMBAT_STAGES: Array[int] = [11, 12, 21, 22, 31, 32]

static func _primitive(value: Variant, depth: int = 0) -> bool:
	if depth > 64: return false
	if value == null or value is int or value is bool or value is String: return true
	if value is Array:
		for item: Variant in value:
			if not _primitive(item, depth + 1): return false
		return true
	if value is Dictionary:
		for key: Variant in value:
			if not key is String or not _primitive(value[key], depth + 1): return false
		return true
	return false

func _init(validated_tables: Dictionary, native_providers: Dictionary = {}) -> void:
	tables = validated_tables.duplicate(true)
	providers = native_providers.duplicate()

func _refuse(error: String) -> Dictionary:
	return {"committed":false, "error":error}

func _invoke(name: String, work: Dictionary, context: Dictionary, rng: Rng) -> String:
	var result: Variant = providers[name].call(work, context, rng, tables)
	return result if result is String else "native provider must return an explicit diagnostic String: " + name

func update_battle(state: Dictionary, held: int, pressed: int, rng: Rng) -> Dictionary:
	if not _primitive(state): return _refuse("TACO state must remain a bounded plain primitive record")
	var error: String = State.validate(state)
	if not error.is_empty(): return _refuse(error)
	if state["mode_stage"] not in COMBAT_STAGES: return _refuse("battle controller requires a native combat stage")
	if held < 0 or held > 0xffffffff or pressed < 0 or pressed > 0xffffffff:
		return _refuse("TACO pad input must retain native unsigned32 masks")
	if rng == null or not rng.configuration_error().is_empty(): return _refuse("TACO battle needs the actual configured native RNG")
	# Source flags is uninitialized on this branch; do not invent a bomb input.
	if (state["actors"][0]["mode"] & 0x40) != 0:
		return _refuse("native actor mode0x40 battle flags require original instruction evidence")
	for name: String in REQUIRED:
		if not providers.get(name) is Callable or not providers[name].is_valid():
			return _refuse("required native TACO operation is not implemented: " + name)
	var auxiliary: int = state["actors"][0]["auxiliary"]
	var secondary: String = ""
	if auxiliary == 0: secondary = "shooter_fire_secondary_homing"
	elif auxiliary == 1: secondary = "shooter_fire_secondary_spread"
	if not secondary.is_empty() and (not providers.get(secondary) is Callable or not providers[secondary].is_valid()):
		return _refuse("required native TACO operation is not implemented: " + secondary)
	error = Effects.validate(state, "hit_sparks")
	if error.is_empty(): error = Effects.validate(state, "explosions")
	if not error.is_empty(): return _refuse(error)
	var checkpoint: Dictionary = rng.checkpoint()
	var work: Dictionary = state.duplicate(true)
	work["effects"] = []
	var context: Dictionary = {"held":held, "pressed":pressed, "flags":0}
	var faults: Array[String] = []
	var sound: Callable = func(target: Dictionary, channel: int, command: int) -> void:
		var result: Dictionary = Sound.play_se(target, channel, command, tables, rng)
		if not result["error"].is_empty(): faults.append(result["error"])
	var result: Dictionary = Player.update_player(work, held, pressed)
	error = result["error"]
	if error.is_empty(): error = _invoke("shooter_update_wingman", work, context, rng)
	if error.is_empty(): error = _invoke("shooter_update_secondary_shots", work, context, rng)
	if error.is_empty(): error = Primary.update_primary_all(work)["error"]
	if error.is_empty():
		result = Player.shot_input_flags(work, held, pressed)
		error = result["error"]
		context["flags"] = result["value"]
	if error.is_empty():
		var weapon: int = work["actors"][0]["weapon"]
		if weapon == 0: result = Primary.fire_wide(work, 0, context["flags"], tables, rng)
		elif weapon == 1: result = Primary.fire_beam(work, 0, context["flags"], tables, rng)
		else: result = Primary.fire_vulcan(work, 0, context["flags"], tables, rng)
		error = result["error"]
	if error.is_empty() and not secondary.is_empty(): error = _invoke(secondary, work, context, rng)
	if error.is_empty(): error = _invoke("shooter_update_bomb", work, context, rng)
	if error.is_empty(): error = Collision.run_collisions(work, tables, sound)
	if error.is_empty() and not faults.is_empty(): error = faults[0]
	if error.is_empty(): error = Effects.update_hit_sparks(work)
	if error.is_empty(): error = Effects.update_explosions(work, rng)
	if error.is_empty(): error = Collision.update_enemy_bullets(work)
	if error.is_empty(): error = _invoke("shooter_collect_items", work, context, rng)
	if error.is_empty(): error = _invoke("shooter_update_items", work, context, rng)
	if error.is_empty(): error = rng.step_error()
	if error.is_empty() and not _primitive(work): error = "native provider produced a nonprimitive state"
	if error.is_empty(): error = State.validate(work)
	if not error.is_empty():
		rng.restore(checkpoint)
		return _refuse(error)
	# Caller owns the source stage handler, enemy scripts, native end check,
	# frame hook/rendering, and external service validation. No automatic exit.
	return {"committed":true, "error":"", "state":work, "rng":rng.checkpoint(),
		"effects":work["effects"].duplicate(true)}

func check_battle_end(state: Dictionary, pressed: int) -> Dictionary:
	if not _primitive(state): return _refuse("TACO state must remain a bounded plain primitive record")
	var validation: String = State.validate(state)
	if not validation.is_empty(): return _refuse(validation)
	if state.get("mode_stage") not in COMBAT_STAGES: return _refuse("battle end check requires a native combat stage")
	var work: Dictionary = state.duplicate(true)
	var error: String = Lifecycle.check_battle_end(work, pressed)
	if not error.is_empty(): return _refuse(error)
	return {"committed":true, "error":"", "state":work}

func compute_result(state: Dictionary) -> Dictionary:
	if not _primitive(state): return _refuse("TACO state must remain a bounded plain primitive record")
	var validation: String = State.validate(state)
	if not validation.is_empty(): return _refuse(validation)
	if state.get("mode_stage") != 8 or state.get("mode_step") != 0: return _refuse("native result compute requires result stage8/step0")
	var work: Dictionary = state.duplicate(true)
	work["effects"] = []
	var error: String = Lifecycle.compute_result(work)
	if not error.is_empty(): return _refuse(error)
	return {"committed":true, "error":"", "state":work, "effects":work["effects"].duplicate(true)}
