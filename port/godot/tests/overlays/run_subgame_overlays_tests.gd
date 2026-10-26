extends SceneTree
## T-29018 (Lane 7): Headless unit & visual verification suite for the three
## interactive subgame overlays (RPG_BAT, TACO / shooter_3d, TT / TwinBee Time Attack).

const Overlays = preload("res://overlays/subgame_overlays.gd")
const Presenter = preload("res://presentation/subgame_overlays_presenter.gd")

var _failures: Array[String] = []
var _assertions_passed: int = 0

func _initialize() -> void:
	var font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
	var capture_dir := "/workspace/.claude/orchestrator/state/T-29018/subgame-overlays-v1/captures"
	for arg in OS.get_cmdline_user_args():
		if arg.begins_with("--font-root="):
			font_root = arg.trim_prefix("--font-root=")
		elif arg.begins_with("--capture-dir="):
			capture_dir = arg.trim_prefix("--capture-dir=")

	_test_rpg_battle_rules()
	_test_taco_shooter_rules()
	_test_tt_twinbee_rules()
	_test_presentation_and_captures(font_root, capture_dir)

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

func _test_presentation_and_captures(font_root: String, capture_dir: String) -> void:
	var presenter := Presenter.new()
	var err: String = presenter.init_resources(font_root)
	_check(err.is_empty(), "Presenter init_resources should succeed: " + err)
	if not err.is_empty():
		return

	DirAccess.make_dir_recursive_absolute(capture_dir)

	# 1. RPG_BAT Party Battle HUD & Command Menu
	var rpg_state: Dictionary = Overlays.rpg_create_state({
		"encounter": Overlays.RPG_ENCOUNTER_DELINQUENTS,
		"param_condition": 97,
		"girl_id": 0,
		"boss_event": 1,
	})
	rpg_state["command_cursor"] = 2 # Highlight わざ (Special)
	rpg_state["message_lines_hex"] = [Presenter.STR_HEX["assist_shiori_1"]]
	var img_rpg: Image = presenter.render_rpg_battle(rpg_state)
	_check(img_rpg != null and img_rpg.get_size() == Vector2i(320, 240), "RPG_BAT image must be 320x240")
	var path_rpg: String = capture_dir.path_join("01-rpg-battle-party-hud.png")
	_check(img_rpg.save_png(path_rpg) == OK, "Save 01-rpg-battle-party-hud.png")

	# 2. TACO 3D Rail Shooter Horizon & HUD
	var taco_state: Dictionary = Overlays.shooter_create_state()
	var img_taco: Image = presenter.render_taco_shooter(taco_state)
	_check(img_taco != null and img_taco.get_size() == Vector2i(320, 240), "TACO 3D shooter image must be 320x240")
	var path_taco: String = capture_dir.path_join("02-taco-shooter-3d-hud.png")
	_check(img_taco.save_png(path_taco) == OK, "Save 02-taco-shooter-3d-hud.png")

	# 3. TT TwinBee Time Attack Playfield & Scoreboard
	var tt_state: Dictionary = Overlays.tt_create_state()
	var img_tt: Image = presenter.render_tt_twinbee(tt_state)
	_check(img_tt != null and img_tt.get_size() == Vector2i(320, 240), "TT TwinBee image must be 320x240")
	# Verify 32px left/right black pillars from tt_video_init
	_check(img_tt.get_pixel(8, 120) == Color.BLACK and img_tt.get_pixel(308, 120) == Color.BLACK, "TT playfield has authentic 32px black side pillars")
	var path_tt: String = capture_dir.path_join("03-tt-twinbee-time-attack.png")
	_check(img_tt.save_png(path_tt) == OK, "Save 03-tt-twinbee-time-attack.png")

	_check(presenter.bounding_box_violations.is_empty(), "Zero glyph bounding-box violations: " + ", ".join(presenter.bounding_box_violations))
	_check(presenter.recorded_glyphs.size() >= 20, "Presenter recorded glyphs across all 3 subgame screens")
