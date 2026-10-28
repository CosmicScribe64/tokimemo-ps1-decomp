extends SceneTree
## Invented fixtures only. This is not an original-game or rendered oracle.
const Rules = preload("res://overlays/shooter_collision_rules.gd")
var checks: int = 0
var failures: Array[String] = []

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures.append(label)

static func fixture() -> Dictionary:
	var actors: Array = []
	for index: int in range(79):
		actors.append({"flag_bytes": [0, 0], "mode": 0, "phase": 0,
			"rotation_x": 0, "rotation_y": 0, "rotation_z": 0, "param": 0,
			"scale_x": 0, "scale_y": 0, "scale_z": 0, "weapon": 0,
			"x": 30000, "y": 30000, "z": 30000, "counter": 0,
			"argument_x": 0, "argument_y": 0, "argument_z": 0, "auxiliary": 0,
			"stats": [0, 0, 0, 0]})
	actors[0]["x"] = 0; actors[0]["y"] = 0; actors[0]["z"] = 1000
	actors[0]["stats"][1] = 100
	actors[8]["x"] = 5000; actors[8]["y"] = 0; actors[8]["z"] = 1000
	actors[8]["stats"][1] = 100
	var state: Dictionary = {"actors": actors, "effects": [], "score": 0,
		"mode_stage": 1, "frame_count": 1, "stages_cleared": 0, "hit_sound_played": 99}
	for name: String in ["player_primary", "player_secondary", "wingman_primary", "wingman_secondary", "enemy_bullets"]:
		var pool: Array = []
		for index: int in range(16):
			pool.append({"x": 30000, "y": 30000, "z": 30000, "w": 0,
				"velocity_x": 0, "velocity_y": 0, "velocity_z": 0, "unused_word": 0,
				"unused_byte": 0, "argument": 0, "age": 0, "flags": 0})
		state[name] = pool
	var sparks: Array = []
	for index: int in range(16):
		sparks.append({"position": {"x": 0, "y": 0, "z": 0, "w": 0},
			"velocity": {"x": 0, "y": 0, "z": 0, "w": 0},
			"unused_byte": 0, "image_index": 0, "age": 0, "flags": 0})
	state["hit_sparks"] = sparks
	return state

static func tables() -> Dictionary:
	var scores: Array = []
	for index: int in range(17): scores.append((index + 1) * 100)
	return {"damage_by_kind": [0, 3, 5, 7, 9, 11, 13, 15, 17], "score_by_class": scores}

func enemy(state: Dictionary, index: int, hp: int = 10) -> void:
	state["actors"][index]["flag_bytes"][1] = 0x80
	state["actors"][index]["stats"] = [1, hp, 0, 0xaf]
	state["actors"][index]["x"] = 0
	state["actors"][index]["y"] = 0
	state["actors"][index]["z"] = 900

func _source_sound(state: Dictionary, arg0: int, arg1: int) -> void:
	state["effects"].append({"kind": "invented_callback", "arg0": arg0, "arg1": arg1, "hp_before": state["actors"][0]["stats"][1]})

func _initialize() -> void:
	var state: Dictionary = fixture()
	state["enemy_bullets"][0].merge({"x": 0, "y": 0, "z": 1000, "flags": 0x80}, true)
	state["actors"][0]["stats"][1] = 5
	check(Rules.hit_ships_with_bullets(state, _source_sound).is_empty(), "ship bullet call succeeds")
	check(state["actors"][0]["stats"][1] == 1 and state["actors"][0]["mode"] == 4 and state["actors"][0]["counter"] == 16, "player damage flags and counter")
	check(state["effects"][1]["hp_before"] == 5, "sound callback runs before source HP mutation")
	for edge: Dictionary in [{"x": 128, "y": 0, "z": 1000}, {"x": 0, "y": 128, "z": 1000}, {"x": 0, "y": 0, "z": 1256}]:
		state = fixture(); state["enemy_bullets"][0].merge(edge, true); state["enemy_bullets"][0]["flags"] = 0x80
		Rules.hit_ships_with_bullets(state)
		check(state["actors"][0]["stats"][1] == 100, "strict ship collision edge is excluded")
	state = fixture(); state["actors"][0]["mode"] = 0x80
	state["actors"][8]["flag_bytes"][1] = 0xc0; state["actors"][8]["stats"] = [1, 13, 0, 0]
	Rules.hit_ships_with_bodies(state)
	check(state["actors"][8]["stats"][1] == 1 and state["actors"][8]["mode"] == 0 and state["effects"].is_empty(), "wingman body loop includes self without player sound/flag")
	state = fixture(); enemy(state, 16, 3)
	state["player_primary"][0].merge({"x": 0, "y": 0, "z": 900, "flags": 0x81, "age": 40}, true)
	Rules.hit_enemies_with_shots(state, "player_primary", 1, 0, tables())
	check(state["actors"][16]["stats"][1] == 0 and state["actors"][16]["stats"][3] == 0xa0 and state["score"] == 130, "ordinary kill damage, score and low-nibble clear")
	check(state["player_primary"][0]["flags"] == 0 and state["player_primary"][0]["age"] == 0, "ordinary shot is consumed")
	check(state["effects"][0]["arg1"] == 0x507 and state["effects"][1]["kind"] == "shooter_spawn_hit_spark" and state["effects"][2]["arg1"] == 0x508, "sound/spark/kill source order")
	state = fixture(); enemy(state, 16, 1); enemy(state, 17, 1)
	state["player_primary"][0].merge({"x": 0, "y": 0, "z": -30000, "flags": 0x20}, true)
	state["player_primary"][1].merge({"x": 0, "y": 0}, true)
	Rules.hit_enemies_with_shots(state, "player_primary", 1, 0, tables())
	check(state["actors"][16]["stats"][1] == 0 and state["actors"][17]["stats"][1] == 0 and state["score"] == 210, "paired hit uses ship Z, scans every enemy, and awards one-point kills")
	check(state["actors"][16]["stats"][3] == 0xaf and state["player_primary"][0]["flags"] == 0x20, "paired hit retains flags and particle")
	state = fixture(); state["score"] = -19
	Rules.run_collisions(state, tables())
	check(state["score"] == -9, "score low-digit normalization uses signed truncation")
	state = fixture(); state["score"] = 10000000
	Rules.run_collisions(state, tables())
	check(state["score"] == 9999999, "score maximum clamp")
	state = fixture(); state["mode_stage"] = 12; state["stages_cleared"] = 1; state["score"] = 42
	state["player_primary"][0]["flags"] = 0x8f
	check(Rules.run_collisions(state, {}).is_empty() and state["score"] == 42, "stage-complete path skips unused shot tables and normalization")
	state = fixture(); state["hit_sparks"][0]["position"]["w"] = 17
	Rules.spawn_hit_spark(state, 40000, -40000, -33280)
	check(state["hit_sparks"][0]["position"]["w"] == 0 and state["hit_sparks"][1]["position"]["x"] == -25536 and state["hit_sparks"][1]["position"]["z"] == 32256, "spark ring and signed16 argument narrowing")
	state = fixture(); state["enemy_bullets"][0].merge({"x": 32760, "z": 10000, "velocity_x": 20, "velocity_z": 10, "flags": 0x80, "age": 255}, true)
	state["enemy_bullets"][1]["age"] = 254
	Rules.update_enemy_bullets(state)
	check(state["enemy_bullets"][0]["x"] == -32756 and state["enemy_bullets"][0]["z"] == 10010 and state["enemy_bullets"][0]["age"] == 0 and state["enemy_bullets"][1]["age"] == 255, "bullet pre-move range and all-slot byte age behavior")
	check(state["effects"] == [{"kind": "shooter_draw_enemy_bullet", "index": 0}], "bullet drawing is ordered after pool movement")
	for corruption: String in ["kind", "table", "spark", "actor"]:
		state = fixture()
		var input_tables: Dictionary = tables()
		match corruption:
			"kind": state["wingman_secondary"][15]["flags"] = 0x89
			"table": input_tables["damage_by_kind"][1] = 256
			"spark": state["hit_sparks"][0]["position"]["w"] = -1
			"actor": state["actors"][78]["stats"][0] = 256
		var before: Dictionary = state.duplicate(true)
		check(not Rules.run_collisions(state, input_tables).is_empty() and state == before, "malformed source dependency rejects before mutation: " + corruption)
	for failure: String in failures: push_error(failure)
	print(JSON.stringify({"checks": checks, "failures": failures}))
	quit(0 if failures.is_empty() else 1)
