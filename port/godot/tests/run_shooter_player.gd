extends SceneTree
const Player = preload("res://overlays/shooter_player_rules.gd")
const Primary = preload("res://overlays/shooter_primary_rules.gd")
const Fixture = preload("res://tests/shooter_fixtures.gd")
const Rng = preload("res://core/native_rng.gd")
var failures: Array[String] = []
var checks: int = 0
func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures.append(label)

func _initialize() -> void:
	var state: Dictionary = Fixture.state()
	state["actors"][8]["phase"] = 71
	state["actors"][0]["rotation_x"] = 123
	Player.reset_ship_state(state)
	check(state["actors"][8]["phase"] == 71 and state["actors"][0]["rotation_x"] == 123, "reset preserves fields the source does not clear")
	var before: Dictionary = state.duplicate(true)
	check(not Player.init_ships(state)["error"].is_empty() and state == before, "model linking is an explicit dependency, before mutation")
	var bindings: Array = [{"attribute":0,"links":[11,22,33]}, {"attribute":0,"links":[44,55,66]}]
	check(Player.init_ships(state, bindings)["error"].is_empty(), "verified provider model outputs initialize numeric ship state")
	check(state["actors"][0]["stats"][1] == 255 and state["actors"][8]["stats"][1] == 128 and state["actors"][0]["weapon"] == -1, "native asymmetric initial health and weapon")
	state["effects"].clear()
	state["actors"][0]["x"] = 4096
	Player.update_player(state, 0x8000, 1)
	check(state["actors"][0]["x"] == 4104 and state["actors"][0]["phase"] == 126, "roll follows clamp and may move eight beyond it")
	check(state["actors"][8]["mode"] == 1, "roll increments native wingman mode")
	state = Fixture.state()
	Player.update_player(state, 0x9000, 0)
	check(state["actors"][0]["x"] == 45 and state["actors"][0]["y"] == -45, "native diagonal movement uses45 rather than normalized32")
	state["actors"][0]["mode"] = 4; state["actors"][0]["counter"] = 0
	Player.update_player(state, 0, 0)
	check(state["actors"][0]["counter"] == -1 and state["actors"][0]["mode"] == 0, "hit counter post-decrements even at zero")
	state["settings"]["auto_shot"] = 1; state["frame_count"] = 4
	check(Player.shot_input_flags(state, 0x20, 0x40)["value"] == 0x60, "auto shot uses held cadence while bomb uses pressed")
	state["frame_count"] = 5
	check(Player.shot_input_flags(state, 0x20, 0)["value"] == 0, "auto shot consumes exact modulo4 cadence")
	before = state.duplicate(true)
	check(not Player.update_wingman(state)["error"].is_empty() and before == state, "wingman GTE/secondary/RNG dependency is not approximated")
	state = Fixture.state()
	for item: Dictionary in state["player_primary"]: item["flags"] = 128
	check(Primary.fire_vulcan(state, 0, 32)["value"] == -1 and state["effects"].is_empty(), "full native pool has no audio or shot mutation")
	state["player_primary"][3]["flags"] = 0
	Primary.fire_vulcan(state, 0, 32)
	check(state["player_primary"][3]["x"] == 64 and state["player_primary"][3]["y"] == 128 and state["player_primary"][3]["velocity_z"] == -512, "vulcan uses first free slot parity and native velocity")
	state = Fixture.state(); state["background_flags"] = 128
	before = state.duplicate(true)
	check(not Primary.fire_wide(state, 0, 32)["error"].is_empty() and state == before, "missing reached sound RNG/table prevents shot mutation")
	var rng: Rng = Rng.new(); rng.configure(Rng.OPENBIOS_PROFILE, 123)
	var tables: Dictionary = {"se_cooldown_range":[0,0,0,0,0,0,0,0,0,0]}
	Primary.fire_wide(state, 0, 32, tables, rng)
	check(rng.draw_count == 1 and state["se_cooldowns"][2] == 0, "cooldown range zero still consumes source RNG draw")
	state["se_cooldowns"][2] = 9
	var effects: int = state["effects"].size()
	Primary.fire_wide(state, 0, 32, tables, rng)
	check(rng.draw_count == 1 and state["effects"].size() == effects and state["player_primary"][1]["flags"] == 0x98, "cooldown suppresses sound, not gameplay firing")
	check(Primary.fire_wide(state, 0, 32)["error"].is_empty() and state["player_primary"][2]["flags"] == 0x98, "throttled source sound returns before requiring unused table or RNG")
	state = Fixture.state(); state["wingman_primary"][1]["flags"] = 3
	check(Primary.fire_beam(state, 1, 32)["value"] == 1 and state["wingman_primary"][1]["flags"] == 3 and state["effects"].is_empty(), "beam leaves second slot flag unchanged; wingman has no firing sound")
	Primary.update_primary(state, 1)
	check(state["wingman_primary"][0]["age"] == 2 and state["effects"].is_empty(), "beam first age increments twice without drawing")
	Primary.update_primary(state, 1)
	check(state["wingman_primary"][0]["age"] == 4 and state["effects"][0]["kind"] == "shooter_draw_beam", "beam draw attaches two endpoints only after source age gate")
	state["effects"].clear(); state["wingman_primary"][0]["age"] = 127
	Primary.update_primary(state, 1)
	check(state["wingman_primary"][0]["flags"] == 0 and state["wingman_primary"][1]["flags"] == 0 and state["effects"].size() == 1, "beam draws once before its129 expiry clear")
	state = Fixture.state(); state["player_primary"][0]["age"] = 255
	Primary.update_primary(state, 0)
	check(state["player_primary"][0]["age"] == 0 and state["player_primary"][1]["age"] == 1, "inactive particles still age and byte-wrap")
	for item: Dictionary in state["player_primary"]: item["flags"] = 0xb0
	Primary.clear_beam_flags(state, 0)
	check(state["player_primary"][0]["flags"] == 0x90, "clear beam masks only bit0x20")
	before = state.duplicate(true); state["bomb_shots"][0]["age"] = 256
	var damaged: Dictionary = state.duplicate(true)
	check(not Primary.clear_player_shots(state)["error"].is_empty() and state == damaged, "all clear-pool validation precedes global mutation")
	print(JSON.stringify({"checks":checks,"failures":failures,"scope":"invented player/primary rule tests; no original-native/GTE/render parity"}))
	for failure: String in failures: push_error(failure)
	quit(0 if failures.is_empty() else 1)
