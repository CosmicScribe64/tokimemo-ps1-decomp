extends SceneTree
## T-29018 / T-29024 (Lane 5): Headless unit, interactive play-through & visual
## verification suite for the three interactive subgame overlays:
## 1. RPG_BAT (`src/ovl/rpg_battle/`)
## 2. TACO    (`src/ovl/shooter_3d/`)
## 3. TT      (`src/ovl/TT/`)

const Overlays = preload("res://overlays/subgame_overlays.gd")
const Presenter = preload("res://presentation/subgame_overlays_presenter.gd")

var _failures: Array[String] = []
var _assertions_passed: int = 0

func _initialize() -> void:
	var font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
	var capture_dir := "/workspace/.claude/orchestrator/state/T-29024/rpg-bat-authentic-actor130-and-vram-v1/captures"
	for arg in OS.get_cmdline_user_args():
		if arg.begins_with("--font-root="):
			font_root = arg.trim_prefix("--font-root=")
		elif arg.begins_with("--capture-dir="):
			capture_dir = arg.trim_prefix("--capture-dir=")

	_test_rpg_battle_rules()
	_test_rpg_battle_interactive_playthrough()
	_test_taco_shooter_rules()
	_test_taco_shooter_interactive_playthrough()
	_test_tt_twinbee_rules()
	_test_tt_twinbee_interactive_playthrough()
	_test_presentation_and_captures(font_root, capture_dir)
	_test_presenter_scene_nodes(font_root)

	if not _failures.is_empty():
		for msg in _failures:
			push_error("FAIL: " + msg)
		print("SUBGAME_OVERLAYS_TESTS_FAILED (%d failures)" % _failures.size())
		quit(1)
		return

	print("SUBGAME_OVERLAYS_TESTS_OK (%d assertions passed)" % _assertions_passed)
	quit(0)

func _check(condition: bool, message: String) -> void:
	if condition:
		_assertions_passed += 1
	else:
		_failures.append(message)

func _test_rpg_battle_rules() -> void:
	# 1. Initial state & partner girl HP table
	var state: Dictionary = Overlays.rpg_create_state({
		"encounter": Overlays.RPG_ENCOUNTER_DELINQUENTS,
		"param_condition": 97,
		"param_athletics": 80,
		"girl_id": 0, # Shiori
		"boss_event": 1,
		"club_grades": [0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		"club_practice": [0, 320, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	})
	_check(int(state["player_hp"]) == 970, "Player HP should be condition(97) * 10 = 970")
	_check(int(state["girl_hp"]) == 256, "Shiori initial HP should be 256")
	_check(int(state["player_club"]) == 1, "Drama club (index 1) grade 3 should be selected")
	_check(int(state["special_ready"]) == 1 and int(state["special_gauge"]) == 100, "Club move should start ready with gauge 100")

	# Verify all partner girl HP constants from include/ovl/rpg_battle.h
	var expected_girl_hp := {
		0: 256, 1: 99, 2: 312, 3: 304, 4: 189, 5: 298,
		6: 1878, 7: 277, 8: 365, 9: 173, 10: 265, 13: 573
	}
	for gid in expected_girl_hp.keys():
		var gs: Dictionary = Overlays.rpg_create_state({"girl_id": gid})
		_check(int(gs["girl_hp"]) == int(expected_girl_hp[gid]), "Girl %d HP mismatch" % gid)

	# Alien encounter forces Yuina (girl_id = 2) and disables initial club move
	var alien_state: Dictionary = Overlays.rpg_create_state({"encounter": Overlays.RPG_ENCOUNTER_ALIENS, "girl_id": 0})
	_check(int(alien_state["girl_id"]) == 2, "Alien encounter must force Yuina (girl_id == 2)")
	_check(int(alien_state["club_move_available"]) == 0, "Alien encounter disables initial club move")

	# Go-Home club (11) requires zero total practice points
	var go_home_valid: Dictionary = Overlays.rpg_create_state({
		"club_grades": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3],
		"club_practice": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	})
	_check(int(go_home_valid["player_club"]) == 11, "Go-Home club with 0 practice points is valid")
	var go_home_invalid: Dictionary = Overlays.rpg_create_state({
		"club_grades": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3],
		"club_practice": [10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	})
	_check(int(go_home_invalid["player_club"]) == 0xFF, "Go-Home club with >0 practice points becomes 0xFF")

	# 2. Basic damage formula (rpg_player_basic_damage)
	_check(Overlays.rpg_player_basic_damage(100, 0) == 9999, "rand_4096 == 0 yields max damage 9999")
	_check(Overlays.rpg_player_basic_damage(100, 32) == 0, "rand_4096 < 64 yields miss (0)")
	var normal_dmg: int = Overlays.rpg_player_basic_damage(100, 500, 100, 750)
	var crit_dmg: int = Overlays.rpg_player_basic_damage(100, 200, 100, 750)
	_check(normal_dmg == 100 + (100 * 9) / 2, "Normal basic damage matches 100 + lv*9/2 = 550")
	_check(crit_dmg == normal_dmg + 750, "Critical basic damage adds crit bonus")

	# 3. All 12 club special moves (0..11)
	for club_id in range(12):
		var roll: Dictionary = Overlays.rpg_player_club_damage(club_id, 300, Overlays.RPG_ENCOUNTER_DELINQUENTS, 200)
		_check(int(roll["primary"]) > 0 and int(roll["primary"]) <= 9999, "Club %d primary damage out of range" % club_id)
		_check(Overlays.RPG_CLUB_MOVE_BANNERS.has(club_id), "Club %d banner missing" % club_id)
	var sci_roll: Dictionary = Overlays.rpg_player_club_damage(2, 300)
	_check(int(sci_roll["primary"]) == 9999 and int(sci_roll["slot1"]) >= 2500, "Science club deals 9999 primary + AoE slot damage")

	# 4. All 11 partner assist moves (0..10), including Nozomi polite vs rough
	for gid in range(11):
		var assist: Dictionary = Overlays.rpg_resolve_partner_assist(gid, false)
		_check(bool(assist["supported"]) and not assist["lines"].is_empty(), "Partner assist %d missing lines" % gid)
	var nozomi_polite: Dictionary = Overlays.rpg_resolve_partner_assist(6, false)
	var nozomi_rough: Dictionary = Overlays.rpg_resolve_partner_assist(6, true)
	_check(nozomi_polite["effect"] == "effect_b" and nozomi_rough["effect"] == "effect_a", "Nozomi polite/rough assist variant mismatch")

	# 5. Command execution (Guard 5% heal, Special move Drama + Partner Assist, Attack)
	state["player_hp"] = 500
	Overlays.rpg_execute_command(state, 1) # Guard
	_check(int(state["player_hp"]) == 500 + (970 / 20), "Guard heals 5% of max HP")
	var guarded_dmg: int = Overlays.rpg_apply_enemy_attack(state, 100, false)
	_check(guarded_dmg == 50, "Guarding halves incoming enemy damage")

	Overlays.rpg_execute_command(state, 2, 0) # Special (Drama club -> triggers Shiori assist)
	_check(int(state["player_action"]) == Overlays.RPG_PLAYER_SPECIAL, "Special command sets RPG_PLAYER_SPECIAL")
	_check(state["last_assist"].get("girl", "") == "shiori", "Drama club special triggers Shiori partner assist")

func _test_rpg_battle_interactive_playthrough() -> void:
	# Multi-round interactive combat play-through:
	# Delinquents -> Bancho escalation -> Victory + Loot rewards + Return payload
	var state: Dictionary = Overlays.enter_overlay(Overlays.OVERLAY_ID_RPG_BAT, {
		"encounter": Overlays.RPG_ENCOUNTER_DELINQUENTS,
		"param_condition": 120,
		"param_athletics": 220,
		"girl_id": 0, # Shiori
		"boss_event": 1,
		"club_grades": [0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		"club_practice": [0, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	})

	# Navigate command menu cursor via interactive input
	Overlays.step_overlay(state, {"down": true})
	_check(int(state["command_cursor"]) == 1, "RPG cursor moves down to Guard (1)")
	Overlays.step_overlay(state, {"confirm": true}) # Execute Guard turn
	_check(int(state["turn_count"]) == 1, "Guard turn increments turn_count to 1")
	_check(int(state["enemy_last_damage"]) > 0, "Enemy counterattacks on Guard turn")

	# Navigate to Item (cursor 3) and use healing item
	state["player_hp"] = 600
	Overlays.step_overlay(state, {"down": true}) # cursor 2
	Overlays.step_overlay(state, {"down": true}) # cursor 3 (Item)
	_check(int(state["command_cursor"]) == 3, "RPG cursor moves down to Item (3)")
	var items_before: int = int(state["items_remaining"])
	Overlays.step_overlay(state, {"confirm": true})
	_check(int(state["items_remaining"]) == items_before - 1, "Using item decrements items_remaining")
	_check(int(state["player_hp"]) > 600, "Using item heals player before enemy counterattack")

	# Execute Special move (cursor 2) and Attacks (cursor 0) until Delinquents fall & Bancho escalates
	Overlays.rpg_step_turn(state, 2, 0) # Special move
	var safety: int = 0
	while not bool(state.get("bancho_escalated", false)) and safety < 40:
		state["player_hp"] = int(state["player_max_hp"])
		state["girl_hp"] = int(state["girl_max_hp"])
		Overlays.rpg_step_turn(state, 0, 0)
		safety += 1
	_check(bool(state.get("bancho_escalated", false)), "Defeating delinquents with boss_event=1 escalates to Bancho")
	_check(int(state["stage"]) == Overlays.RPG_STAGE_BANCHO_INTRO, "Bancho escalation sets stage to RPG_STAGE_BANCHO_INTRO (11)")

	# Step past Bancho intro and defeat Bancho
	Overlays.step_overlay(state, {"confirm": true})
	_check(int(state["stage"]) == Overlays.RPG_STAGE_BATTLE_ACTIVE, "Confirming Bancho intro resumes RPG_STAGE_BATTLE_ACTIVE (9)")
	safety = 0
	while not bool(state.get("victory", false)) and safety < 40:
		state["player_hp"] = int(state["player_max_hp"])
		state["girl_hp"] = int(state["girl_max_hp"])
		Overlays.rpg_step_turn(state, 0, 0)
		safety += 1
	_check(bool(state.get("victory", false)) and int(state["stage"]) == Overlays.RPG_STAGE_VICTORY, "Defeating Bancho reaches RPG_STAGE_VICTORY (10)")
	_check(int(state["talk_result_code"]) == 0, "Victory sets talk_result_code == 0")
	_check(not (state.get("loot_rewards", []) as Array).is_empty(), "Victory populates loot_rewards")

	# Confirm on victory screen exits overlay
	Overlays.step_overlay(state, {"confirm": true})
	_check(bool(state.get("completed", false)) and int(state["stage"]) == Overlays.RPG_STAGE_EXIT, "Confirming victory transitions to RPG_STAGE_EXIT (13)")
	var payload: Dictionary = Overlays.build_return_payload(state)
	_check(bool(payload["victory"]) and int(payload["talk_result_code"]) == 0, "RPG victory return payload is valid")

	# Defeat path verification
	var defeat_state: Dictionary = Overlays.rpg_create_state({
		"encounter": Overlays.RPG_ENCOUNTER_BEAR,
		"enemy_hp": 5000,
		"girl_id": 14,
		"param_condition": 10, # 100 HP
	})
	defeat_state["player_hp"] = 10
	Overlays.rpg_step_turn(defeat_state, 0, 0)
	_check(bool(defeat_state.get("defeat", false)) and int(defeat_state["stage"]) == Overlays.RPG_STAGE_DEFEAT, "Player HP reaching 0 triggers RPG_STAGE_DEFEAT (12)")
	_check(int(defeat_state["talk_result_code"]) == 1, "Defeat sets talk_result_code == 1")
	var def_payload: Dictionary = Overlays.build_return_payload(defeat_state)
	_check(bool(def_payload["defeat"]) and int(def_payload["talk_result_code"]) == 1, "RPG defeat return payload is valid")

func _test_taco_shooter_rules() -> void:
	var state: Dictionary = Overlays.shooter_create_state()
	var center_proj: Vector2i = Overlays.shooter_project_3d(0, 0, 0x800)
	_check(center_proj == Vector2i(160, 120), "3D origin projects to screen center (160, 120)")

	# Step movement, barrel roll invulnerability, primary fire, and bomb fire
	Overlays.shooter_step_frame(state, -4, 2, true, true, false)
	var p: Dictionary = state["player"]
	_check((int(p["mode"]) & 0x80) != 0 and int(p["roll_phase"]) == 0x7F, "Barrel roll sets invulnerability bit 0x80 and phase 0x7F")
	_check(state["shots"].size() == 1, "Firing primary weapon spawns a projectile")

	var fired_first: int = Overlays.shooter_fire_bomb(state)
	_check(fired_first == Overlays.SHOOTER_BOMB_BLAST, "First bomb fired is Blast Bomb (4)")
	_check(int(p["bomb_slots"][0]) == Overlays.SHOOTER_BOMB_PULSE and int(p["bomb_slots"][1]) == 0, "Bomb slots shift after firing")

	# Result tally calculation (result_screen.c:59-109)
	state["stages_cleared"] = 3
	state["score"] = 400000
	state["time_left"] = 1200
	p["shield"] = 0xFF
	p["bomb_slots"] = [4, 3, 0] # 2 bombs remaining -> 250,000 bonus
	var tally: Dictionary = Overlays.shooter_compute_result_tally(state)
	_check(int(tally["stage_bonus"]) == 3000000, "3 stages cleared = 3,000,000 stage bonus")
	_check(int(tally["shield_bonus"]) == 500000, "Full 255 shield on clear = 500,000 shield bonus")
	_check(int(tally["time_bonus"]) == 120000, "Time bonus = time_left * 100")
	_check(int(tally["bomb_stock_bonus"]) == 250000, "2 bombs remaining = 250,000 bomb stock bonus")
	_check(int(tally["total_score"]) == 4270000, "Total tally matches sum of bonuses")

	var rank_idx: int = Overlays.shooter_try_insert_ranking(state, int(tally["total_score"]), "ＦＵＫ")
	_check(rank_idx == 0, "4,270,000 inserts at rank 0 (1st place)")

func _test_taco_shooter_interactive_playthrough() -> void:
	var state: Dictionary = Overlays.enter_overlay(Overlays.OVERLAY_ID_TACO, {"primary_weapon": Overlays.SHOOTER_PRIMARY_BEAM})
	var p: Dictionary = state["player"]
	var cx0: int = int(p["crosshair_x"])
	var cy0: int = int(p["crosshair_y"])

	# Crosshair movement & primary fire
	Overlays.step_overlay(state, {"right": true, "up": true, "confirm": true})
	_check(int(p["crosshair_x"]) > cx0 and int(p["crosshair_y"]) < cy0, "TACO interactive input moves crosshair right and up")
	_check(int(p["ammo"]) < Overlays.SHOOTER_MAX_AMMO, "Firing primary weapon consumes ammo")

	# Reload action
	Overlays.step_overlay(state, {"cancel": true})
	_check(int(p["ammo"]) == Overlays.SHOOTER_MAX_AMMO, "Reload action restores ammo to SHOOTER_MAX_AMMO")

	# Enemy projectile shield damage when not barrel rolling
	p["mode"] = 1
	p["roll_phase"] = 0
	var shield_before: int = int(p["shield"])
	(state["enemy_shots"] as Array).append({"x": int(p["x"]), "y": int(p["y"]), "z": 0x380, "vz": -0x120, "damage": 20})
	Overlays.step_overlay(state, {})
	_check(int(p["shield"]) == shield_before - 20, "Incoming enemy projectile reduces player shield by 20")

	# Interactive stage & boss progression through Stage 1 -> Boss 1 -> Stage 2 -> Boss 2 -> Stage 3 -> Boss 3 -> Clear
	var stages_visited: Array[int] = [int(state["stage"])]
	for _step_i in range(40):
		p["shield"] = 255
		# Damage all active enemies or boss via high-damage shots/bombs
		for enemy: Dictionary in state.get("enemies", []):
			enemy["hp"] = 0
		var boss: Dictionary = state.get("boss", {})
		if int(boss.get("active", 0)) != 0:
			boss["hp"] = 1
			(state["shots"] as Array).append({
				"x": int(boss["x"]),
				"y": int(boss["y"]),
				"z": int(boss["z"]) - 0x100,
				"vz": 0x100,
				"kind": Overlays.SHOOTER_PRIMARY_BEAM,
				"damage": 10,
			})
		Overlays.step_overlay(state, {"confirm": true})
		var cur_stg: int = int(state["stage"])
		if not stages_visited.has(cur_stg):
			stages_visited.append(cur_stg)
		if bool(state.get("completed", false)):
			break

	_check(stages_visited.has(Overlays.SHOOTER_STAGE_BOSS1), "TACO play-through reaches Stage 1 Boss (12)")
	_check(stages_visited.has(Overlays.SHOOTER_STAGE_STAGE2), "TACO play-through advances to Stage 2 (21)")
	_check(stages_visited.has(Overlays.SHOOTER_STAGE_BOSS2), "TACO play-through reaches Stage 2 Boss (22)")
	_check(stages_visited.has(Overlays.SHOOTER_STAGE_STAGE3), "TACO play-through advances to Stage 3 (31)")
	_check(stages_visited.has(Overlays.SHOOTER_STAGE_BOSS3), "TACO play-through reaches Stage 3 Boss (32)")
	_check(bool(state.get("stage_clear", false)) and int(state["stages_cleared"]) == 3, "TACO play-through clears all 3 stages")
	_check(bool(state.get("completed", false)) and int(state["stage"]) == Overlays.SHOOTER_STAGE_SAVE_AND_EXIT, "TACO play-through reaches SHOOTER_STAGE_SAVE_AND_EXIT (46)")
	var payload: Dictionary = Overlays.build_return_payload(state)
	_check(bool(payload["stage_clear"]) and int(payload["stages_cleared"]) == 3 and int(payload["score"]) > 0, "TACO return payload reports 3 stages cleared and positive score")

	# Shield depletion Game Over path
	var go_state: Dictionary = Overlays.shooter_create_state()
	go_state["player"]["shield"] = 10
	(go_state["enemy_shots"] as Array).append({"x": int(go_state["player"]["x"]), "y": int(go_state["player"]["y"]), "z": 0x380, "vz": -0x120, "damage": 20})
	Overlays.shooter_step_interactive(go_state, {})
	_check(bool(go_state.get("game_over", false)) and int(go_state["stage"]) == Overlays.SHOOTER_STAGE_GAME_END, "Shield reaching 0 triggers TACO game_over and SHOOTER_STAGE_GAME_END (45)")

func _test_tt_twinbee_rules() -> void:
	var state: Dictionary = Overlays.tt_create_state({
		"shot_kind": 8,    # Normal shot so kind 1 (Twin Shot) is not suppressed
		"has_options": 0,  # No options yet so kind 3 (Options) is not suppressed
		"speed_level": 4,
	})

	# Verify tt_time_text_set formatting (text_and_time_display.c:91-108)
	var t0: Dictionary = Overlays.tt_format_time(0x63C8)
	var t1: Dictionary = Overlays.tt_format_time(0x71D8)
	var t2: Dictionary = Overlays.tt_format_time(0x7FE8)
	var tmax: Dictionary = Overlays.tt_format_time(0x8CA0)
	_check(t0["text"] == "7:05.73", "Default 1st place time 0x63C8 formats to 7:05.73")
	_check(t1["text"] == "8:05.73", "Default 2nd place time 0x71D8 formats to 8:05.73")
	_check(t2["text"] == "9:05.73", "Default 3rd place time 0x7FE8 formats to 9:05.73")
	_check(tmax["text"] == "9:59.99", "Max time 0x8CA0 formats to 9:59.99")

	# Verify bell color cycling across hits 1..20 (cycling_pickups.c:277-373)
	var kinds_seen := {}
	for hit in range(1, 21):
		var b: Dictionary = Overlays.tt_bell_hit_by_shot(state, 1, 0)
		kinds_seen[hit] = int(b["kind"])
	_check(int(kinds_seen[5]) == 1, "Bell hit 5 cycles to kind 1 (Blue Twin Shot)")
	_check(int(kinds_seen[10]) == 2, "Bell hit 10 cycles to kind 2 (Green Speed Up)")
	_check(int(kinds_seen[15]) == 3, "Bell hit 15 cycles to kind 3 (White Option Followers)")
	_check(int(kinds_seen[20]) == 6, "Bell hit 20 cycles to kind 6 (Purple Speed Down)")

	# Collect Option bell (bell 2 has kind 3)
	Overlays.tt_bell_collect(state, 2)
	_check(int(state["player"]["has_options"]) == 1, "Collecting White Option Bell activates 3 Option followers")

	# Overlap check & smart bomb
	var hit: bool = Overlays.tt_box_overlap(Vector2i(100, 100), Overlays.TT_HITBOX_PLAYER, Vector2i(104, 102), Overlays.TT_HITBOX_PICKUP)
	var miss: bool = Overlays.tt_box_overlap(Vector2i(100, 100), Overlays.TT_HITBOX_PLAYER, Vector2i(130, 100), Overlays.TT_HITBOX_PICKUP)
	_check(hit and not miss, "tt_box_overlap accurately detects AABB overlap")

	Overlays.tt_trigger_smart_bomb(state)
	_check(int(state["air_enemies"][0]["state"]) == 0x70 and int(state["enemy_shots"][0]["active"]) == 0, "Smart bomb explodes air enemies and clears enemy shots")

	var slot: int = Overlays.tt_try_insert_ranking(state, 0x5800)
	_check(slot == 0 and int(state["rankings"][0]["time"]) == 0x5800, "Faster clear time 0x5800 inserts into 1st place and shifts rankings")

func _test_tt_twinbee_interactive_playthrough() -> void:
	var state: Dictionary = Overlays.enter_overlay(Overlays.OVERLAY_ID_TT, {
		"stage_index": 0,
		"tt_stages_cleared": 0,
		"shot_kind": 8,
		"has_options": 1,
	})
	var p: Dictionary = state["player"]
	var x0: int = int(p["x"])
	var y0: int = int(p["y"])

	# Ship movement, air shot, and ground bomb
	Overlays.step_overlay(state, {"left": true, "up": true, "confirm": true, "bomb": true})
	_check(int(p["x"]) < x0 and int(p["y"]) < y0, "TT interactive input moves TwinBee ship left and up")
	_check((state["player_shots"] as Array).size() >= 1, "TT confirm input fires player air shots")
	_check((state["player_bombs"] as Array).size() == 1, "TT bomb input fires ground bomb")

	# Shot-to-bell collision & AABB bell collection during frame step
	var bell0: Dictionary = state["bells"][0]
	bell0["active"] = 1
	bell0["state"] = 0x40
	bell0["x"] = int(p["x"])
	bell0["y"] = int(p["y"]) - 20
	bell0["hits"] = 4
	(state["player_shots"] as Array).append({"active": 1, "x": int(bell0["x"]), "y": int(bell0["y"]), "vy": -8, "damage": 1, "kind": 8})
	Overlays.step_overlay(state, {})
	_check(int(bell0["hits"]) == 5 and int(bell0["kind"]) == 1, "Player shot hitting bell cycles bell to kind 1 (Blue Twin Shot)")

	# Move bell onto player for AABB pickup
	bell0["x"] = int(p["x"])
	bell0["y"] = int(p["y"])
	Overlays.step_overlay(state, {})
	_check(int(p["shot_kind"]) == 9, "Overlapping Blue Bell upgrades player shot_kind to Twin Shot (9)")

	# Clear all 4 stages (stage_index 0..3) and verify tt_stages_cleared writeback (1..4)
	var rule_state := {"tt_stages_cleared": 0}
	for target_stage in range(4):
		var boss: Dictionary = state["boss"]
		boss["active"] = 1
		boss["hp"] = 1
		(state["player_shots"] as Array).append({
			"active": 1,
			"x": int(boss["x"]),
			"y": int(boss["y"]),
			"vy": -8,
			"damage": 5,
			"kind": 9,
		})
		Overlays.step_overlay(state, {}) # Defeat boss -> TT_STATE_STAGE_CLEAR
		_check(int(state["tt_stages_cleared"]) == target_stage + 1, "Defeating TT Stage %d boss sets tt_stages_cleared=%d" % [target_stage + 1, target_stage + 1])
		Overlays.tt_apply_writeback_to_rule_state(state, rule_state)
		_check(int(rule_state["tt_stages_cleared"]) == target_stage + 1, "tt_apply_writeback_to_rule_state updates rule_state['tt_stages_cleared']=%d" % (target_stage + 1))
		Overlays.step_overlay(state, {"confirm": true}) # Advance from TT_STATE_STAGE_CLEAR

	_check(int(state["state"]) == Overlays.TT_STATE_NAME_ENTRY or int(state["state"]) == Overlays.TT_STATE_RANKING_VIEW, "Clearing all 4 TT stages enters name entry / ranking view")
	Overlays.step_overlay(state, {"confirm": true})
	if not bool(state.get("completed", false)):
		Overlays.step_overlay(state, {"confirm": true})
	_check(bool(state.get("completed", false)) and int(state["state"]) == Overlays.TT_STATE_EXIT, "Confirming ranking view completes TT overlay at TT_STATE_EXIT (9)")
	var tt_payload: Dictionary = Overlays.build_return_payload(state)
	_check(int(tt_payload["tt_stages_cleared"]) == 4 and bool(tt_payload["stage_clear"]), "TT return payload reports tt_stages_cleared == 4 and stage_clear == true")

	# Lives decrement & Game Over path
	var go_tt: Dictionary = Overlays.tt_create_state({"lives": 0})
	go_tt["player"]["invincible_timer"] = 0
	(go_tt["enemy_shots"] as Array).append({"active": 1, "x": int(go_tt["player"]["x"]), "y": int(go_tt["player"]["y"]), "vx": 0, "vy": 0})
	Overlays.tt_step_interactive(go_tt, {})
	_check(bool(go_tt.get("game_over", false)) and int(go_tt["state"]) == Overlays.TT_STATE_GAME_OVER, "Enemy shot hit with 0 extra lives triggers TT_STATE_GAME_OVER (8)")

func _test_presentation_and_captures(font_root: String, capture_dir: String) -> void:
	var presenter := Presenter.new()
	var err: String = presenter.init_resources(font_root)
	_check(err.is_empty(), "Presenter init_resources should succeed: " + err)
	if not err.is_empty():
		return

	DirAccess.make_dir_recursive_absolute(capture_dir)

	# 1. RPG_BAT Party Battle HUD, Command Menu & Authentic Actor 130 (`CDROM/BATTLE/*.BIN`, `hud_messages_and_swimming_effect.c:302-363`)
	_check(presenter._rpg_bat_ready, "RPG_BAT presenter loads authentic CDROM/BATTLE/*.BIN sprite/palette/frame tables")
	var rpg_state: Dictionary = Overlays.rpg_create_state({
		"encounter": Overlays.RPG_ENCOUNTER_DELINQUENTS,
		"param_condition": 97,
		"girl_id": 0,
		"boss_event": 1,
	})
	rpg_state["message_lines_hex"] = [Presenter.STR_HEX["assist_shiori_1"]]
	var rpg_cmd_hashes := {}
	var rpg_cmd_files := [
		"01-rpg-battle-cmd-0-attack.png",
		"01-rpg-battle-cmd-1-guard.png",
		"01-rpg-battle-cmd-2-special.png",
	]
	var img_rpg: Image = null
	for cmd_idx in range(3):
		rpg_state["command_cursor"] = cmd_idx
		var img_cmd: Image = presenter.render_rpg_battle(rpg_state)
		_check(img_cmd != null and img_cmd.get_size() == Vector2i(320, 240), "RPG_BAT cmd %d image must be 320x240" % cmd_idx)
		_check(str(rpg_state.get("render_provenance", "")) == "authentic_rpg_bat_vram", "RPG_BAT cmd %d state marks authentic_rpg_bat_vram provenance" % cmd_idx)
		_check(str(img_cmd.get_meta("render_provenance", "")) == "authentic_rpg_bat_vram", "RPG_BAT cmd %d Image metadata marks authentic_rpg_bat_vram provenance" % cmd_idx)
		var actor130: Dictionary = rpg_state.get("actor130", {})
		var img_actor130: Dictionary = img_cmd.get_meta("actor130", {})
		var expected_seq: int = 5 + cmd_idx
		var expected_hit := Rect2i(-43, 57 + 15 * cmd_idx, 35, 15)
		var expected_screen := Rect2i(117, 177 + 15 * cmd_idx, 35, 15)
		_check(int(actor130.get("actor_id", 0)) == 130 and int(img_actor130.get("actor_id", 0)) == 130, "RPG_BAT cmd %d uses actor130 (ACT(96 + 0x22))" % cmd_idx)
		_check(int(actor130.get("sequence", 0)) == expected_seq and int(img_actor130.get("sequence", 0)) == expected_seq, "RPG_BAT cmd %d sets actor130 sequence=%d" % [cmd_idx, expected_seq])
		_check(actor130.get("hit_rect") == expected_hit and img_actor130.get("hit_rect") == expected_hit, "RPG_BAT cmd %d hit_rect matches native (-43, %d, 35, 15)" % [cmd_idx, 57 + 15 * cmd_idx])
		_check(actor130.get("screen_rect") == expected_screen and img_actor130.get("screen_rect") == expected_screen, "RPG_BAT cmd %d screen_rect matches (117, %d, 35, 15)" % [cmd_idx, 177 + 15 * cmd_idx])
		var ctx := HashingContext.new()
		ctx.start(HashingContext.HASH_SHA256)
		ctx.update(img_cmd.get_data())
		var cmd_sha: String = ctx.finish().hex_encode()
		_check(not rpg_cmd_hashes.has(cmd_sha), "RPG_BAT cmd %d render is visually distinct across actor130 sequences 5/6/7" % cmd_idx)
		rpg_cmd_hashes[cmd_sha] = cmd_idx
		_check(img_cmd.save_png(capture_dir.path_join(rpg_cmd_files[cmd_idx])) == OK, "Save " + rpg_cmd_files[cmd_idx])
		if cmd_idx == 0:
			_check(img_cmd.save_png(capture_dir.path_join("01-rpg-battle-dragon-menu.png")) == OK, "Save 01-rpg-battle-dragon-menu.png")
		if cmd_idx == 2:
			img_rpg = img_cmd
	var path_rpg: String = capture_dir.path_join("01-rpg-battle-party-hud.png")
	_check(img_rpg != null and img_rpg.save_png(path_rpg) == OK, "Save 01-rpg-battle-party-hud.png")

	# 2. TACO 3D Rail Shooter Horizon, TMD Models & VRAM HUD (`CDROM/TACO/*` + `TACO.EXN`)
	_check(presenter._taco_vram_ready and presenter._taco_vram.size() == 1024 * 512, "TACO presenter reconstructs 1024x512 PS1 VRAM from CDROM/TACO/*.NBN + TACO.EXN")
	var taco_state: Dictionary = Overlays.shooter_create_state()
	var img_taco: Image = presenter.render_taco_shooter(taco_state)
	_check(img_taco != null and img_taco.get_size() == Vector2i(320, 240), "TACO 3D shooter image must be 320x240")
	_check(str(taco_state.get("render_provenance", "")) == "authentic_taco_vram_tmd", "TACO stage render marks authentic_taco_vram_tmd provenance")
	_check(str(img_taco.get_meta("render_provenance", "")) == "authentic_taco_vram_tmd", "TACO stage Image metadata marks authentic_taco_vram_tmd provenance")
	var path_taco: String = capture_dir.path_join("02-taco-shooter-3d-hud.png")
	_check(img_taco.save_png(path_taco) == OK, "Save 02-taco-shooter-3d-hud.png")

	# Verify TACO Stage 1/2/3 Bosses (`BS01.TMD`, `DRA.TMD`, `TAKO.TMD`) render from authentic TMD + MTX.NBN + VRAM
	var taco_boss_stages := [
		{"stage": Overlays.SHOOTER_STAGE_BOSS1, "kind": "arm_tail_boss", "file": "07-taco-boss-1-bs01.png"},
		{"stage": Overlays.SHOOTER_STAGE_BOSS2, "kind": "morph_boss", "file": "08-taco-boss-2-dra.png"},
		{"stage": Overlays.SHOOTER_STAGE_BOSS3, "kind": "collapse_boss", "file": "09-taco-boss-3-tako.png"},
	]
	for bspec: Dictionary in taco_boss_stages:
		var b_state: Dictionary = Overlays.shooter_create_state({"stage": int(bspec["stage"])})
		var img_tboss: Image = presenter.render_taco_shooter(b_state)
		_check(img_tboss != null and str(img_tboss.get_meta("render_provenance", "")) == "authentic_taco_vram_tmd", "TACO boss %s renders from authentic TMD + VRAM" % str(bspec["kind"]))
		var path_tboss: String = capture_dir.path_join(str(bspec["file"]))
		_check(img_tboss.save_png(path_tboss) == OK, "Save " + str(bspec["file"]))

	# 3. TT TwinBee Time Attack Playfield & Scoreboard (`TBTA.DAT` + `TT.EXN` 1024x512 PS1 VRAM)
	_check(presenter._tt_vram_ready and presenter._tt_vram.size() == 1024 * 512, "TT presenter reconstructs 1024x512 PS1 VRAM from TBTA.DAT + TT.EXN")
	var tt_state: Dictionary = Overlays.tt_create_state()
	tt_state["ranking_slot"] = 0 # Even with ranking_slot >= 0, TT_STATE_STAGE must NOT draw a ranking card over the playfield
	var img_tt: Image = presenter.render_tt_twinbee(tt_state)
	_check(img_tt != null and img_tt.get_size() == Vector2i(320, 240), "TT TwinBee image must be 320x240")
	_check(str(tt_state.get("render_provenance", "")) == "authentic_tbta_vram", "TT stage render marks authentic_tbta_vram provenance")
	_check(str(img_tt.get_meta("render_provenance", "")) == "authentic_tbta_vram", "TT stage Image metadata marks authentic_tbta_vram provenance")
	# Verify lower-right playfield at (192, 184) is NOT covered by a PS1 window border during TT_STATE_STAGE
	_check(img_tt.get_pixel(192, 184) != Color8(208, 220, 248), "TT_STATE_STAGE does not draw ranking card over lower-right playfield")
	# Verify 32px left/right black pillars from tt_video_init
	_check(img_tt.get_pixel(8, 120) == Color.BLACK and img_tt.get_pixel(308, 120) == Color.BLACK, "TT playfield has authentic 32px black side pillars")
	var path_tt: String = capture_dir.path_join("03-tt-twinbee-time-attack.png")
	_check(img_tt.save_png(path_tt) == OK, "Save 03-tt-twinbee-time-attack.png")

	# Verify Stages 2, 3, and 4 bosses render from TBTA.DAT VRAM sprite sets (sets 3..10)
	for boss_id in range(1, 4):
		tt_state["boss"]["boss_id"] = boss_id
		var img_boss: Image = presenter.render_tt_twinbee(tt_state)
		_check(img_boss != null and str(img_boss.get_meta("render_provenance", "")) == "authentic_tbta_vram", "TT Stage %d boss renders from authentic TBTA.DAT VRAM" % (boss_id + 1))
		var path_boss: String = capture_dir.path_join("0%d-tt-twinbee-stage-%d-boss.png" % [boss_id + 3, boss_id + 1])
		_check(img_boss.save_png(path_boss) == OK, "Save 0%d-tt-twinbee-stage-%d-boss.png" % [boss_id + 3, boss_id + 1])

	# Also verify dedicated TT ranking view (state 7) & TACO Boss / Game End banners have zero bounding-box violations
	tt_state["state"] = Overlays.TT_STATE_RANKING_VIEW
	var img_tt_rank: Image = presenter.render_tt_twinbee(tt_state)
	_check(img_tt_rank != null and img_tt_rank.get_size() == Vector2i(320, 240), "TT ranking view image must be 320x240")

	taco_state["stage"] = Overlays.SHOOTER_STAGE_GAME_END
	taco_state["game_end_reason"] = 0
	var img_taco_end: Image = presenter.render_taco_shooter(taco_state)
	_check(img_taco_end != null and img_taco_end.get_size() == Vector2i(320, 240), "TACO game end image must be 320x240")

	_check(presenter.bounding_box_violations.is_empty(), "Zero glyph bounding-box violations: " + ", ".join(presenter.bounding_box_violations))
	_check(presenter.recorded_glyphs.size() >= 20, "Presenter recorded glyphs across all 3 subgame screens")

func _test_presenter_scene_nodes(font_root: String) -> void:
	var presenter := Presenter.new()
	presenter.init_resources(font_root)
	var host := Control.new()

	# 1. TACO scene hierarchy
	var taco_state: Dictionary = Overlays.shooter_create_state()
	Overlays.shooter_spawn_boss(taco_state, 0)
	var taco_view: Control = presenter.present_subgame(host, "TACO", taco_state)
	_check(taco_view != null and taco_view.name == "TacoSubgameView", "build_taco_nodes creates TacoSubgameView")
	_check(taco_view.get_node_or_null("Playfield/Player") != null, "TACO view has Playfield/Player")
	_check(taco_view.get_node_or_null("Playfield/Crosshair") != null, "TACO view has Playfield/Crosshair")
	_check(taco_view.get_node_or_null("Playfield/Enemies") != null, "TACO view has Playfield/Enemies")
	_check(taco_view.get_node_or_null("Playfield/Boss") != null and (taco_view.get_node("Playfield/Boss") as Control).visible, "TACO view has visible Playfield/Boss")
	_check(taco_view.get_node_or_null("HUD/ShieldBar") != null and taco_view.get_node_or_null("HUD/AmmoBar") != null, "TACO view has HUD ShieldBar and AmmoBar")

	# 2. TT scene hierarchy
	var tt_state: Dictionary = Overlays.tt_create_state()
	var tt_view: Control = presenter.present_subgame(host, "TT", tt_state)
	_check(tt_view != null and tt_view.name == "TwinBeeSubgameView", "build_tt_nodes creates TwinBeeSubgameView")
	_check(tt_view.get_node_or_null("Playfield/Player") != null, "TT view has Playfield/Player")
	_check(tt_view.get_node_or_null("Playfield/Bells") != null and tt_view.get_node("Playfield/Bells").get_child_count() == 3, "TT view has 3 active bells")
	_check(tt_view.get_node_or_null("Playfield/Boss") != null, "TT view has Playfield/Boss")
	_check(tt_view.get_node_or_null("HUD/LivesContainer") != null and tt_view.get_node_or_null("HUD/SpeedGauge") != null, "TT view has HUD LivesContainer and SpeedGauge")

	# 3. RPG_BAT scene hierarchy
	var rpg_state: Dictionary = Overlays.rpg_create_state({"girl_id": 0})
	rpg_state["command_cursor"] = 1
	var rpg_view: Control = presenter.present_subgame(host, "RPG_BAT", rpg_state)
	_check(rpg_view != null and rpg_view.name == "RpgBattleSubgameView", "build_rpg_battle_nodes creates RpgBattleSubgameView")
	_check(rpg_view.get_node_or_null("Playfield/Party/Player") != null and rpg_view.get_node_or_null("Playfield/Party/Partner") != null, "RPG view has Player and Partner nodes")
	var cmd_node: Control = rpg_view.get_node_or_null("Playfield/CommandMenu")
	_check(cmd_node != null and rpg_view.get_node_or_null("Playfield/MessageBox") != null, "RPG view has CommandMenu and MessageBox")
	if cmd_node != null:
		_check(int(cmd_node.get_meta("actor_id", 0)) == 130 and int(cmd_node.get_meta("sequence", 0)) == 6, "RPG CommandMenu node exposes actor130 sequence=6 for cursor=1")
		_check(cmd_node.get_meta("hit_rect") == Rect2i(-43, 72, 35, 15) and cmd_node.get_meta("screen_rect") == Rect2i(117, 192, 35, 15), "RPG CommandMenu node exposes native hit_rect and screen_rect")
	_check(rpg_view.get_node_or_null("HUD/PlayerHpBar") != null and rpg_view.get_node_or_null("HUD/GirlHpBar") != null and rpg_view.get_node_or_null("HUD/MpGauge") != null, "RPG view has HUD HP and MP bars")

	host.free()
