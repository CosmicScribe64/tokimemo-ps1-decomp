extends SceneTree
## Invented service seams test local dispatch/rollback, never playable parity.
const Fixture = preload("res://tests/shooter_fixtures.gd")
const CollisionFixture = preload("res://tests/run_shooter_collision_tests.gd")
const Pool = preload("res://overlays/shooter_enemy_pool_rules.gd")
const StageRules = preload("res://overlays/shooter_stage3_rules.gd")
const Controller = preload("res://scenes/shooter_stage3_controller.gd")
const Combat = preload("res://scenes/shooter_combat_controller.gd")
const Rng = preload("res://core/native_rng.gd")
var checks: int = 0
var failures: Array[String] = []
var service_failure: String = ""
var poll_value: int = 0
var malformed_key: String = ""

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
	state.merge({"game_mode":0x90, "mode_stage":31, "mode_step":1, "mode_stage_scratch":7,
		"substep":3, "mode_stage_timer":50, "mode_step_timer":60, "score":0, "time_left":18000,
		"stages_cleared":2, "count_target":0, "count_current":0, "clear_color":[10,20,30],
		"model_slots":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0], "hit_sound_played":0,
		"coordinate_owner":[], "joint_models":[], "load_request":0,
		"sound_bank_wait_state":0, "sound_bank_poll_result":0}, true)
	for key: String in Pool.SCRIPT_FIELDS: state[key] = 0
	for i: int in range(64):
		state["coordinate_owner"].append(0)
		state["joint_models"].append({"attribute":0, "links":[100+i,200+i,300+i]})
	state["actors"][0]["stats"][1] = 90
	state["actors"][8]["stats"][1] = 70
	state["actors"][0]["weapon"] = -1
	state["actors"][0]["auxiliary"] = -1
	return state

func combat_seam(state: Dictionary, _context: Dictionary, _rng: Rng, _tables: Dictionary) -> String:
	state["effects"].append({"kind":"invented_combat_seam"})
	return ""

func script_seam(state: Dictionary, argument: int, _rng: Rng, name: String) -> String:
	state["effects"].append({"kind":name, "argument":argument})
	return ""

func script_bindings() -> Dictionary:
	var output: Dictionary = {}
	for name: String in StageRules.REQUIRED: output[name] = script_seam.bind(name)
	return output

func stage_seam(state: Dictionary, args: Dictionary, rng: Rng, name: String) -> Dictionary:
	state["effects"].append({"kind":name, "args":args.duplicate(true)})
	if name == service_failure:
		rng.next("invented stage failure draw")
		return {"error":"invented stage service fault"}
	if name == "shooter_update_enemies" and not malformed_key.is_empty():
		if malformed_key == "model": state["actors"][16].erase("model")
		elif malformed_key == "owner": state["coordinate_owner"][0] = 256
		elif malformed_key == "script": state["script_timer"] = 1 << 40
		else: state["score"] = 1 << 40
	return {"error":"", "value":poll_value}

func stage_bindings() -> Dictionary:
	var output: Dictionary = {}
	for name: String in Controller.REQUIRED + StageRules.REQUIRED:
		output[name] = stage_seam.bind(name)
	return output

func controller() -> Controller:
	var tables: Dictionary = {"score_by_class":[], "damage_by_kind":[], "se_cooldown_range":[]}
	tables["score_by_class"].resize(17); tables["score_by_class"].fill(120)
	tables["damage_by_kind"].resize(9); tables["damage_by_kind"].fill(2)
	tables["se_cooldown_range"].resize(10); tables["se_cooldown_range"].fill(3)
	var combat_providers: Dictionary = {}
	for name: String in Combat.REQUIRED: combat_providers[name] = combat_seam
	return Controller.new(Combat.new(tables, combat_providers), stage_bindings())

func effect_kinds(state: Dictionary) -> Array:
	return state["effects"].map(func(event: Dictionary) -> String: return event["kind"])

func _initialize() -> void:
	var rng: Rng = Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 123)
	var state: Dictionary = fixture()
	state["actors"][16]["stats"] = [10,90,2,88]
	check(Pool.count_kind(state, 2) == 0 and state["actors"][16]["stats"][1] == 0, "count query clears inactive stale HP")
	state["actors"][16]["flag_bytes"][1] = 0x80
	check(Pool.count_active(state) == 1 and Pool.count_kind(state, 0xff) == 0, "active flag and surviving HP are different counts")
	state["coordinate_owner"][0] = 16
	state["actors"][16]["stats"][2] = 10
	Pool.set_visibility(state, 1, 16)
	check(state["actors"][16]["model"]["attribute"] == 0 and state["joint_models"][0]["attribute"] == 0, "kind10 visible uses actor handle")
	Pool.set_visibility(state, 0, 16)
	check(state["joint_models"][0]["attribute"] == Pool.HIDDEN, "kind10 hidden also hides owned joints")
	state["actors"][16]["stats"][2] = 13
	Pool.set_visibility(state, 1, 16)
	check(state["actors"][16]["model"]["attribute"] == Pool.HIDDEN and state["joint_models"][0]["attribute"] == 0, "kind13 visible uses joints")
	Pool.remove_enemy(state, 16)
	check(state["coordinate_owner"][0] == 0 and state["actors"][16]["stats"][3] == 88 and state["actors"][16]["flag_bytes"][1] == 0, "removal releases owner and preserves drop flags")
	state["actors"][16]["coordinate"] = {"invented_preserved_word":77}
	var before: Dictionary = state.duplicate(true)
	Pool.reset_enemies(state)
	check(state["actors"][16]["coordinate"] == before["actors"][16]["coordinate"] and state["actors"][16]["stats"] == [0,1,1,88], "reset preserves coordinate and stats3")
	check(state["actors"][15] == before["actors"][15] and state["actors"][63] == before["actors"][63], "reset bounds are exactly16 through62")
	check(state["joint_models"][0]["links"] == before["joint_models"][0]["links"] and Pool.count_free_coordinates(state) == 64, "reset preserves joint links and frees64 owners")
	state = fixture()
	state["script_timer"] = 29
	state["circler_pairing"] = 5
	check(StageRules.update_script(state, script_bindings(), rng).is_empty(), "script call succeeds only with explicit invented seams")
	check(effect_kinds(state) == ["shooter_update_enemies","shooter_spawn_swarm_ship","shooter_spawn_wall_ships"], "source enemy update precedes periodic spawn and phase spawn")
	check(state["script_phase"] == 1 and state["script_timer"] == 0 and state["circler_pairing"] == 0, "next phase resets timer and pairing")
	for phase: int in range(1, 5):
		state = fixture(); state["script_phase"] = phase
		state["actors"][16]["flag_bytes"][1] = 0x80
		state["actors"][16]["stats"] = [0,1,3,0]
		StageRules.update_script(state, script_bindings(), rng)
		check(state["script_phase"] == phase, "wall HP prevents phase advancement "+str(phase))
	state = fixture(); state["script_phase"] = 11
	state["actors"][16]["flag_bytes"][1] = 0x80
	StageRules.update_script(state, script_bindings(), rng)
	check(state["script_phase"] == 12, "phase11 advances when active dying enemy has zero HP")
	StageRules.update_script(state, script_bindings(), rng)
	check(state["mode_step"] == 1, "phase12 waits for dying enemy active flag to clear")
	Pool.remove_enemy(state,16)
	StageRules.update_script(state, script_bindings(), rng)
	check(state["mode_step"] == 2 and state["substep"] == 0 and state["script_phase"] == 0, "final empty pool resets enemies and advances native mode step")
	state = fixture(); before = state.duplicate(true)
	check(not StageRules.update_script(state, {}, rng).is_empty() and state == before, "missing spawn dependency refuses before timer mutation")
	var driver: Controller = controller()
	state = fixture(); state["mode_step"] = 0; state["frame_count"] = 16
	before = state.duplicate(true)
	var result: Dictionary = driver.step(state, 0x8000, 0x20, rng)
	check(result["committed"] and state == before, "stage transaction leaves caller untouched")
	state = result["state"]
	check(state["load_request"] == 0x34 and state["clear_color"] == [8,16,96] and state["mode_step"] == 1, "stage init uses native bank/color and next step")
	check(state["actors"][0]["x"] == 32 and state["actors"][0]["z"] == 2048 and state["time_left"] == 17999, "stage init then actual player/combat and end check")
	var kinds: Array = effect_kinds(state)
	check(kinds[0] == "shooter_draw_text" and kinds[1] == "audio_bgm_play_short" and kinds[2] == "shooter_draw_hud" and kinds[-2] == "shooter_frame_update" and kinds[-1] == "shooter_draw_models", "prebattle and outer source effects retain order")
	state = fixture(); state["script_phase"] = 4
	state["actors"][16]["flag_bytes"][1] = 0x80
	state["actors"][16]["stats"] = [0,1,3,0]
	for frame: int in range(120):
		state["frame_count"] = frame
		result = driver.step(state, 0x8000, 0x20 if frame == 0 else 0, rng)
		if not result["committed"]: break
		state = result["state"]
	check(result["committed"] and state["mode_stage"] == 31 and state["script_phase"] == 4 and state["actors"][0]["x"] > 2048 and state["actors"][0]["x"] <= 4096 and state["time_left"] == 17880, "120 held-input ticks with invented enemy seam do not synthesize a clear")
	check(state["frame_count"] == 119, "stage controller does not invent outer frame_count ticks")
	for value: int in [1,257,-255]:
		state = fixture(); state["script_phase"] = 13; poll_value = value
		result = driver.step(state, 0, 0, rng)
		check(result["committed"] and result["state"]["sound_bank_poll_result"] == 1 and result["state"]["sound_bank_wait_state"] == 2, "poll return narrows to native byte before comparison "+str(value))
	poll_value = 0
	state = fixture(); state["mode_step"] = 2
	result = driver.step(state, 0, 0, rng)
	check(result["committed"] and result["state"]["mode_stage"] == 32 and result["state"]["load_request"] == 0x38 and result["state"]["time_left"] == 17999, "real stage3 completion enters boss32 and still executes battle/end")
	result = driver.step(state, 0, 0x800, rng)
	check(result["committed"] and result["state"]["mode_stage"] == 45, "START end check follows boss transition in same native frame")
	for name: String in ["shooter_update_enemies","shooter_frame_update","shooter_draw_models"]:
		state = fixture(); before = state.duplicate(true)
		var checkpoint: Dictionary = rng.checkpoint()
		service_failure = name
		result = driver.step(state, 0, 0, rng)
		check(not result["committed"] and state == before and rng.checkpoint() == checkpoint, "state and RNG rollback at "+name)
	service_failure = ""
	for key: String in ["score","script","model","owner"]:
		state = fixture(); before = state.duplicate(true)
		var checkpoint: Dictionary = rng.checkpoint()
		malformed_key = key
		result = driver.step(state,0,0,rng)
		check(not result["committed"] and state == before and rng.checkpoint() == checkpoint, "malformed successful provider rolls back "+key)
	malformed_key = ""
	state = fixture(); driver.providers.erase("shooter_draw_models")
	before = state.duplicate(true)
	check(not driver.step(state,0,0,rng)["committed"] and state == before, "missing outer render dependency blocks before progression")
	print(JSON.stringify({"checks":checks,"failures":failures,"scope":"invented seams and local rules only; no native or actual-app acceptance"}))
	quit(0 if failures.is_empty() else 1)
