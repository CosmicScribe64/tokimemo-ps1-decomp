extends SceneTree
## Invented operation/transaction fixtures. No complete/native player-route claim.
const Fixture = preload("res://tests/shooter_fixtures.gd")
const CollisionFixture = preload("res://tests/run_shooter_collision_tests.gd")
const Controller = preload("res://scenes/shooter_combat_controller.gd")
const Lifecycle = preload("res://overlays/shooter_lifecycle_rules.gd")
const Effects = preload("res://overlays/shooter_effects_rules.gd")
const Sound = preload("res://overlays/shooter_sound_rules.gd")
const Tables = preload("res://services/shooter_combat_tables.gd")
const Rng = preload("res://core/native_rng.gd")
var checks: int = 0
var failures: Array[String] = []
var malformed_field: String = "score"

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition: failures.append(label)

func fixture() -> Dictionary:
	var state: Dictionary = Fixture.state()
	var collision: Dictionary = CollisionFixture.fixture()
	state["hit_sparks"] = collision["hit_sparks"].duplicate(true)
	state["explosions"] = collision["hit_sparks"].duplicate(true)
	state["items"] = []
	for i: int in range(16): state["items"].append(Fixture.particle())
	state.merge({"game_mode":0x90, "mode_stage":11, "mode_step":1, "mode_stage_scratch":7,
		"substep":3, "mode_stage_timer":50, "mode_step_timer":60, "score":0, "time_left":100,
		"stages_cleared":0, "count_target":0, "count_current":0, "clear_color":[10,20,30],
		"model_slots":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0], "hit_sound_played":0}, true)
	state["actors"][0]["stats"][1] = 90
	state["actors"][8]["stats"][1] = 70
	state["actors"][0]["weapon"] = -1
	state["actors"][0]["auxiliary"] = -1
	return state

func invented_provider(state: Dictionary, context: Dictionary, _rng: Rng, _tables: Dictionary) -> String:
	state["effects"].append({"kind":"invented_provider_seam", "flags":context["flags"]})
	return ""

func failing_provider(state: Dictionary, _context: Dictionary, rng: Rng, _tables: Dictionary) -> String:
	state["score"] = 777
	rng.next("invented rollback case")
	return "invented native provider failure"

func malformed_provider(state: Dictionary, context: Dictionary, _rng: Rng, _tables: Dictionary) -> String:
	match malformed_field:
		"score": state["score"] = 1 << 40
		"actor_stat": state["actors"][20]["stats"][3] = 256
		"particle_word": state["enemy_bullets"][3]["unused_word"] = 32768
		"time_left": state["time_left"] = -32769
		"setting": state["settings"]["auto_shot"] = 256
		"frame_count": state["frame_count"] = -1
	return ""

func bindings() -> Dictionary:
	var output: Dictionary = {}
	for name: String in Controller.REQUIRED: output[name] = invented_provider
	return output

func _initialize() -> void:
	var state: Dictionary = fixture()
	var rng: Rng = Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 123)
	var before: Dictionary = state.duplicate(true)
	var checkpoint: Dictionary = rng.checkpoint()
	var tables: Dictionary = {"score_by_class":[], "damage_by_kind":[], "se_cooldown_range":[]}
	tables["score_by_class"].resize(17); tables["score_by_class"].fill(120)
	tables["damage_by_kind"].resize(9); tables["damage_by_kind"].fill(2)
	tables["se_cooldown_range"].resize(10); tables["se_cooldown_range"].fill(3)
	var controller: Controller = Controller.new(tables)
	check(not controller.update_battle(state, 0, 0, rng)["committed"] and state == before and rng.checkpoint() == checkpoint, "missing native wingman/secondary/bomb/items refuses before progression")
	controller = Controller.new(tables, bindings())
	var result: Dictionary = controller.update_battle(state, 0x8000, 0x20, rng)
	check(result["committed"] and state == before, "transaction returns a new state without mutating caller")
	check(result["state"]["actors"][0]["x"] == 32, "held native left updates each battle call")
	check(result["state"]["player_primary"][0]["flags"] == 0x94, "fire initializes native vulcan slot")
	check(result["state"]["mode_stage"] == 11, "one fire does not leave TACO")
	state = result["state"]
	result = controller.update_battle(state, 0x8000, 0x20, rng)
	check(result["committed"] and result["state"]["mode_stage"] == 11 and result["state"]["actors"][0]["x"] == 64, "second fire remains native combat, with continuous held movement")
	check(result["state"]["player_primary"][0]["z"] == -512, "existing shot moves before new shot is fired")
	var providers: Dictionary = bindings()
	providers["shooter_update_items"] = failing_provider
	controller = Controller.new(tables, providers)
	before = state.duplicate(true); checkpoint = rng.checkpoint()
	result = controller.update_battle(state, 0, 0x20, rng)
	check(not result["committed"] and state == before and rng.checkpoint() == checkpoint, "late native failure rolls back shot/collision/effect/RNG writes")
	providers["shooter_update_items"] = malformed_provider
	controller = Controller.new(tables, providers)
	for mutation: String in ["score", "actor_stat", "particle_word", "time_left", "setting", "frame_count"]:
		malformed_field = mutation
		before = state.duplicate(true); checkpoint = rng.checkpoint()
		result = controller.update_battle(state, 0, 0x20, rng)
		check(not result["committed"] and state == before and rng.checkpoint() == checkpoint, "successful provider with invalid native field rolls back state and advanced RNG: " + mutation)
	state["actors"][0]["mode"] = 0x40
	check(not controller.update_battle(state, 0, 0, rng)["committed"], "undefined native mode40 bomb flags are explicit unsupported state")
	state = fixture(); state["game_mode"] = 0x59
	check(not controller.update_battle(state, 0, 0, rng)["committed"], "invented legacy mode59 is rejected")
	state = fixture(); state["time_left"] = 0
	check(Lifecycle.check_battle_end(state, 0).is_empty() and state["time_left"] == -1 and state["mode_stage"] == 11, "source signed16 zero timer decrements to minus one")
	state = fixture(); state["time_left"] = -32768
	Lifecycle.check_battle_end(state, 0)
	check(state["time_left"] == 32767, "native countdown wraps signed16")
	state = fixture(); state["time_left"] = 1
	Lifecycle.check_battle_end(state, 0)
	check(state["mode_stage"] == 45 and state["mode_step"] == 0 and state["substep"] == 0 and state["mode_stage_timer"] == 0, "time expiry enters native game-end stage and resets fields")
	state = fixture(); state["stages_cleared"] = 3
	Lifecycle.check_battle_end(state, 0)
	check(state["time_left"] == 100, "all-clear preserves timer")
	Lifecycle.check_battle_end(state, 0x800)
	check(state["mode_stage"] == 45, "source START enters game-end, not main-game return")
	state = fixture(); state["mode_stage"] = 8; state["mode_step"] = 0
	state["stages_cleared"] = 3; state["score"] = 9000000
	state["actors"][0]["stats"] = [8,255,4,155]
	state["actors"][0]["argument_z"] = 1
	controller = Controller.new(tables)
	result = controller.compute_result(state)
	check(result["committed"] and result["state"]["result_values"] == [12000000,500000,500000,10000,9999999], "native result keeps pre-total score and caps only total")
	check(result["state"]["actors"][0]["stats"] == [0,1,0,155] and result["state"]["actors"][0]["scale_x"] == 4096, "native actor release preserves stats3 and resets exact fields")
	check(state["actors"][0]["stats"][1] == 255, "result transaction preserves caller before acceptance")
	state = fixture(); state["explosions"][0]["flags"] = 128
	state["explosions"][0]["age"] = 5; state["explosions"][0]["image_index"] = 7
	state["explosions"][0]["position"]["x"] = 32767
	state["explosions"][0]["velocity"]["x"] = 1
	rng.configure(Rng.OPENBIOS_PROFILE, 1)
	check(Effects.update_explosions(state, rng).is_empty() and rng.draw_count == 2, "explosion call consumes two sourced RNG draws")
	check(state["explosions"][0]["position"]["x"] == -32768 and state["explosions"][0]["flags"] == 0, "explosion position wraps and postincrement expiry is exact")
	state = fixture(); rng.configure(Rng.OPENBIOS_PROFILE, 1)
	Effects.update_explosions(state, rng)
	check(rng.draw_count == 2, "inactive explosion pool still consumes both native RNG calls")
	state["hit_sparks"][0]["flags"] = 1; state["hit_sparks"][0]["age"] = 255; state["hit_sparks"][0]["image_index"] = 255
	Effects.update_hit_sparks(state)
	check(state["hit_sparks"][0]["flags"] == 0 and state["hit_sparks"][0]["age"] == 0 and state["hit_sparks"][0]["image_index"] == 0, "native byte postincrement comparisons precede wrap/reset")
	state = fixture(); state["background_flags"] = 1
	check(Sound.play_se(state, 1, 0x507, tables, rng)["error"].is_empty(), "shared collision SE can execute original cooldown/RNG branch")
	var draws: int = rng.draw_count
	state["se_cooldowns"][7] = 2
	check(Sound.play_se(state, 1, 0x507, {}, null)["suppressed"], "throttled SE ignores unused RNG/table dependency")
	check(rng.draw_count == draws, "suppressed SE consumes no random call")
	Sound.tick(state)
	check(state["se_cooldowns"][7] == 1, "native frame-hook cooldown tick decrements nonzero bytes")
	var loader: Tables = Tables.new()
	check(not loader.open_manifest("res://forbidden.json").is_empty() and loader.tables.is_empty(), "original numeric tables cannot be loaded from res")
	var file: FileAccess = FileAccess.open("/tmp/taco-invented-corrupt.json", FileAccess.WRITE)
	file.store_string("{}"); file.close()
	check(not loader.open_manifest("/tmp/taco-invented-corrupt.json").is_empty(), "table digest/schema corruption stops before capability")
	print(JSON.stringify({"checks":checks, "failures":failures, "scope":"invented controller/effects/lifecycle transaction seams; no complete gameplay claim"}))
	quit(0 if failures.is_empty() else 1)
