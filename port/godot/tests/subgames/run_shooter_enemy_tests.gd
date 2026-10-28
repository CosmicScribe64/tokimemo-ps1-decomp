extends SceneTree
## Invented ABI/model/pose/normalization seams, not production gameplay evidence.
const Previous = preload("res://tests/subgames/run_shooter_stage3_tests.gd")
const Tables = preload("res://services/shooter_enemy_tables.gd")
const Spawn = preload("res://overlays/shooter_enemy_spawn_rules.gd")
const Enemy = preload("res://overlays/shooter_stage3_enemy_rules.gd")
const Particles = preload("res://overlays/shooter_enemy_particles.gd")
const Rng = preload("res://core/native_rng.gd")
var checks: int = 0
var failures: Array[String] = []
var malformed: bool = false
var rebind_models: bool = false
var rebind_poses: bool = false
var replace_identity: bool = false

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition: failures.append(label)

func fixture() -> Dictionary:
	var helper: Previous = Previous.new()
	var output: Dictionary = helper.fixture()
	helper.free()
	return output

func tables() -> Dictionary:
	var output: Dictionary = {"keys_d":[0,1,2,4,2,0],"sine_quarter":[]}
	for key: String in ["model_base","joint_poses","joint_count","scale","radius"]:
		output[key] = []
		for i: int in range(17): output[key].append(0 if key == "joint_count" else 100)
	output["joint_count"][13] = 2
	output["zero_position_a"] = [0,0,0,0]; output["zero_position_b"] = [0,0,0,0]
	for i: int in range(1025): output["sine_quarter"].append(i*4)
	return output

func model_seam(state: Dictionary, args: Dictionary, name: String) -> String:
	state["effects"].append({"kind":name,"args":args.duplicate(true)})
	if rebind_models: state["actors"][16] = state["actors"][16].duplicate(true)
	if replace_identity: state["actors"][16] = state["actors"][0].duplicate(true)
	if malformed: state["joint_models"][0]["links"][0] = 1 << 40
	return ""

func services() -> Dictionary:
	var output: Dictionary = {}
	for name: String in Spawn.REQUIRED: output[name] = model_seam.bind(name)
	return output

func normalize_seam(direction: Array) -> Dictionary:
	return {"error":"","value":direction.map(func(value: int) -> int: return value+17)}

func pose_seam(state: Dictionary, args: Dictionary) -> String:
	state["effects"].append({"kind":"invented_pose","args":args.duplicate(true)})
	if rebind_poses: state["actors"][args["actor"]] = state["actors"][args["actor"]].duplicate(true)
	if replace_identity: state["actors"][args["actor"]] = state["actors"][0].duplicate(true)
	if malformed: state["joint_models"][0]["links"][0] = 1 << 40
	return ""

func _initialize() -> void:
	var data: Dictionary = tables()
	var rng: Rng = Rng.new(); rng.configure(Rng.OPENBIOS_PROFILE,123)
	check(Tables.validate(data).is_empty(),"invented table widths valid")
	var loader: Tables = Tables.new()
	var original_path: String = OS.get_environment("TACO_ENEMY_TABLES")
	if not original_path.is_empty():
		check(loader.open_manifest(original_path).is_empty(),"external original numeric table authentication")
	check(not loader.open_manifest("res://enemy.json").is_empty(),"original tables cannot load from project content")
	check(Enemy.sine(-1024,data["sine_quarter"]) == -4096 and Enemy.cosine(-2048,data["sine_quarter"]) == -4096,"native sign/quadrant mapping")
	var state: Dictionary = fixture()
	var layout: Dictionary = {"actor_base":0x100000,"joint_coordinate_base":0x200000}
	var before: Dictionary = state.duplicate(true)
	var result: Dictionary = Spawn.spawn(state,13,0x23,[0,0,0,0],[1,2,3,4],[0,0,0,7],40,0xd0,data,layout,{})
	check(not result["error"].is_empty() and state == before,"missing real model provider refuses before allocation")
	result = Spawn.spawn(state,13,0x23,[0,0,0,0],[1,2,3,4],[0,0,0,7],40,0xd0,data,layout,services())
	check(result["error"].is_empty() and result["value"] == 16,"first native inactive slot allocated")
	var actor: Dictionary = state["actors"][16]
	check(actor["stats"] == [100,40,13,0xd1] and actor["counter"] == 32767 and actor["param"] == 7,"native spawn numeric fields and byteflags")
	check(state["coordinate_owner"].slice(0,3) == [16,16,0] and state["joint_models"][1]["links"][0] == 0x200050,"joint slots allocated in first-free order")
	check(state["effects"].map(func(event: Dictionary) -> String: return event["kind"]) == ["map_model","init_actor_coordinate","link_actor_model","init_joint_coordinate","apply_initial_pose","link_joint_model","init_joint_coordinate","apply_initial_pose","link_joint_model"],"native model-map/link/pose call order")
	state = fixture(); rebind_models = true
	result = Spawn.spawn(state,13,0x23,[0,0,0,0],[1,2,3,4],[0,0,0,7],40,0xd0,data,layout,services())
	check(result["error"].is_empty() and state["actors"][16]["flag_bytes"][1] == 0x80 and state["actors"][16]["stats"] == [100,40,13,0xd1],"successful model record rebind receives final native spawn stores")
	rebind_models = false
	state = fixture(); replace_identity = true
	result = Spawn.spawn(state,13,0x23,[0,0,0,0],[1,2,3,4],[0,0,0,7],40,0xd0,data,layout,services())
	check(result["error"].contains("identity"),"different actor replacement by model service rejects")
	replace_identity = false
	state = fixture()
	for i: int in range(16,63): state["actors"][i]["flag_bytes"][1] = 0x80
	result = Spawn.spawn(state,2,0x20,[0,0,0,0],[1,2,3,4],[0,0,0,0],4,0,data,layout,{})
	check(result["error"].is_empty() and result["value"] == -1,"full actor pool is native exhaustion rather than provider success")
	state = fixture(); malformed = true
	result = Spawn.spawn(state,13,0x23,[0,0,0,0],[1,2,3,4],[0,0,0,0],40,0,data,layout,services())
	check(not result["error"].is_empty(),"successful malformed model service cannot continue")
	malformed = false
	state = fixture(); actor = state["actors"][16]
	actor.merge({"mode":0x20,"z":-20000,"counter":301,"argument_x":50,"argument_y":-50},true)
	check(Enemy.behavior(state,16,data,rng,normalize_seam).is_empty() and actor["z"] == -19744,"swarm native approach starts after counter300")
	state = fixture(); actor = state["actors"][16]
	actor.merge({"mode":0x21,"x":50,"y":-1200,"z":-20000,"counter":180},true)
	var checkpoint: Dictionary = rng.checkpoint()
	check(Enemy.behavior(state,16,data,rng,normalize_seam).is_empty() and actor["counter"] == 180 and state["enemy_bullets"][0]["flags"] == 0,"wall firing bands preserve native upper cutoff")
	actor["counter"] = 181
	check(Enemy.behavior(state,16,data,rng,normalize_seam).is_empty() and actor["counter"] == 0 and state["enemy_bullets"][0]["argument"] == 0xc0 and rng.draw_count == checkpoint["draw_count"]+3,"wall reset can fire jittered aimed shot in same tick")
	state = fixture(); actor = state["actors"][16]
	actor.merge({"mode":0x22,"argument_x":0,"param":1024},true)
	check(Enemy.behavior(state,16,data,rng,normalize_seam).is_empty() and actor["x"] == 1000 and actor["y"] == -700 and actor["argument_x"] == 1,"orbit init uses native table and state")
	actor.merge({"argument_x":2,"counter":601},true)
	Enemy.behavior(state,16,data,rng,normalize_seam)
	check(actor["argument_x"] == 3 and actor["counter"] == 0,"orbit lifetime gate at601")
	actor["counter"] = 16; actor["flag_bytes"][1] = 0x80
	Enemy.behavior(state,16,data,rng,normalize_seam)
	check(actor["flag_bytes"][1] == 0,"native shrink removes active actor")
	state = fixture(); actor = state["actors"][16]
	actor.merge({"mode":0x23,"argument_x":2,"counter":15,"x":2,"y":3,"z":4},true)
	check(Enemy.behavior(state,16,data,rng,normalize_seam).is_empty() and state["enemy_bullets"][0]["x"] == 2 and state["enemy_bullets"][0]["argument"] == 0xc2,"guardian core fires at source snapshot position")
	state = fixture(); actor = state["actors"][16]
	actor["flag_bytes"][1] = 0x80; actor["mode"] = 0x20
	actor["stats"] = [1,0,2,0xf1]; actor["counter"] = 10
	check(Enemy.update_enemies(state,data,rng,normalize_seam,pose_seam).is_empty(),"actual death/drop path executes")
	check(state["items"].slice(0,9).map(func(item: Dictionary) -> int: return item["argument"]) == [6,6,7,2,0,1,5,3,4],"native full drop order")
	check(actor["mode"] == 0xff and actor["phase"] == 2 and actor["flag_bytes"][1] == 0x90 and actor["counter"] == 1,"death starts before behavior and generic counter increment")
	check(state["explosions"][0]["position"]["w"] == 1 and state["explosions"][0]["flags"] == 0x80,"explosion copies actor counter then increments ring alias")
	actor["counter"] = 61
	check(Enemy.update_enemies(state,data,rng,normalize_seam,pose_seam).is_empty() and actor["flag_bytes"][1] == 0 and actor["counter"] == 62,"removed enemy still receives source loop counter increment")
	state = fixture(); actor = state["actors"][16]
	actor["stats"][2] = 13; actor["phase"] = 1; actor["weapon"] = 5
	state["coordinate_owner"][0] = 16; state["coordinate_owner"][1] = 16
	check(Enemy.animate(state,16,data,pose_seam).is_empty() and actor["auxiliary"] == 1 and actor["weapon"] == 0 and state["effects"].size() == 2,"class13 key dispatch every6ticks preserves joint order")
	check(state["effects"][0]["args"]["initial"] and state["effects"][1]["args"]["pose_ordinal"] == 1,"key initial pose reset dispatch")
	state = fixture(); actor = state["actors"][16]
	actor["stats"][2] = 13; actor["phase"] = 1; actor["weapon"] = 5
	state["coordinate_owner"][0] = 16; state["coordinate_owner"][1] = 16
	rebind_poses = true
	check(Enemy.animate(state,16,data,pose_seam).is_empty() and state["actors"][16]["auxiliary"] == 1,"successful pose record rebind receives auxiliary increment")
	state = fixture(); actor = state["actors"][16]
	actor["stats"] = [100,40,13,1]; actor["flag_bytes"][1] = 0x80
	actor["mode"] = 0x23; actor["phase"] = 1; actor["weapon"] = 5
	state["coordinate_owner"][0] = 16; state["coordinate_owner"][1] = 16
	check(Enemy.update_enemies(state,data,rng,normalize_seam,pose_seam).is_empty() and state["actors"][16]["auxiliary"] == 1 and state["actors"][16]["counter"] == 1,"full enemy loop increments current record after pose rebind")
	rebind_poses = false
	actor = state["actors"][16]
	actor["weapon"] = 5; replace_identity = true
	check(Enemy.animate(state,16,data,pose_seam).contains("identity"),"different actor replacement by pose service rejects")
	replace_identity = false
	state = fixture(); actor = state["actors"][16]
	actor["stats"][2] = 13; actor["phase"] = 1; actor["weapon"] = 5
	state["coordinate_owner"][0] = 16; state["coordinate_owner"][1] = 16
	actor["weapon"] = 5
	check(Enemy.animate(state,16,data,Callable()).contains("missing"),"missing native pose service is not skipped")
	malformed = true; actor["weapon"] = 5
	check(not Enemy.animate(state,16,data,pose_seam).is_empty(),"malformed successful pose service rejects")
	malformed = false
	state = fixture(); checkpoint = rng.checkpoint()
	for item: Dictionary in state["items"]: item["flags"] = 0x80
	check(Particles.spawn_item(state,[1,2,3,4],6,rng) == -1 and rng.checkpoint() == checkpoint,"full item pool consumes no RNG")
	state = fixture(); checkpoint = rng.checkpoint()
	check(not Particles.spawn_bullet(state,[0,0,0,0],256,0x80,rng,Callable())["error"].is_empty() and state["enemy_bullets"][0]["flags"] == 0 and rng.checkpoint() == checkpoint,"missing native normalize fails before particle/RNG writes")
	state["explosions"][0]["position"]["w"] = 17
	check(Particles.spawn_explosion(state,[1,2,3,4],[5,6,7,8]).is_empty() and state["explosions"][0]["position"]["w"] == 0 and state["explosions"][1]["position"]["w"] == 4,"native explosion ring old17 repeats slot1 then resets")
	print(JSON.stringify({"checks":checks,"failures":failures,"scope":"invented model/pose/normalization seams; no native, renderer or playable-stage acceptance"}))
	quit(0 if failures.is_empty() else 1)
