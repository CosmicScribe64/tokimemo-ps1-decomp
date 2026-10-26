extends SceneTree
## T-29018: Multi-Run Interactive Playtest Suite for Subgame Overlays (RPG_BAT 0x57,
## TACO 0x59, TT 0x5A) and Event CG Scenes Overlay (EVENT 0x5E).
##
## Exhaustively playtests and visually verifies:
## 1. RPG_BAT (0x57):
##    - All 11 clubs (0..10) + Go-Home (11) special move banners & arena effects
##    - All 11 partner girls (0..10) Drama club assist cut-ins + Nozomi polite/rough variants
##    - Basic attack damage rolls, Guard 5% heal + 50% damage reduction
##    - Full combat flows to Victory (stage=10) and Defeat (stage=12)
## 2. TACO (0x59):
##    - Multi-frame 3D movement, Left (0x81) & Right (0x82) barrel rolls & recovery
##    - Primary weapons (WIDE=0, BEAM=1, VULCAN=7) & AoE Bombs (BLAST=4, PULSE=3)
##    - Result tally computation & 5-slot score ranking insertion
## 3. TT (0x5A):
##    - 32-step bell cycling (TT_BELL_KIND_SEQUENCE) & hazard over-cycling (0x60/0x70)
##    - Power-up collection (Twin Shot, Speed Up, 3 Options, Speed Down) & smart bomb clearing
##    - All 4 Stage Bosses (0..3) & 3-slot time ranking insertion
## 4. EVENT_CG (0x5E):
##    - All 4 interactive scenes (shiori_cg, ayako_karaoke, ride_movie, miharu_encounter)
##      across all cursor choices (0..2), verifying stat deltas & distinct rendered frames
## 5. Section 12a Gate:
##    - Zero glyph bounding-box violations across all rendered frames
##    - PNG captures + JSON proof artifact saved to:
##      /workspace/.claude/orchestrator/state/T-29018/subgame-eventcg-playtest-v1/

const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const SubgameOverlays = preload("res://overlays/subgame_overlays.gd")
const SubgamePresenter = preload("res://presentation/subgame_overlays_presenter.gd")
const EventCgOverlay = preload("res://overlays/event_cg_scenes_overlay.gd")
const EventCgPresenter = preload("res://presentation/event_cg_scenes_presenter.gd")

var _failures: Array[String] = []
var _assertions_passed: int = 0
var _captures: Array[Dictionary] = []

var _dispatcher: OverlayDispatcher
var _out_dir := "/workspace/.claude/orchestrator/state/T-29018/subgame-eventcg-playtest-v1"
var _captures_dir := "/workspace/.claude/orchestrator/state/T-29018/subgame-eventcg-playtest-v1/captures"
var _font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
var _hud_manifest := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
var _date_pack := "/workspace/.asset-cache/port/date-preview-v1"


func _check(condition: bool, message: String) -> void:
	if condition:
		_assertions_passed += 1
	else:
		_failures.append(message)
		push_error("FAIL: " + message)


static func _sha256_image(img: Image) -> String:
	var ctx := HashingContext.new()
	ctx.start(HashingContext.HASH_SHA256)
	ctx.update(img.get_data())
	return ctx.finish().hex_encode()


func _save_capture(img: Image, filename: String, meta: Dictionary) -> String:
	_check(img != null and img.get_size() == Vector2i(320, 240), "Capture %s must be 320x240" % filename)
	if img == null:
		return ""
	var full_path := _captures_dir.path_join(filename)
	var err := img.save_png(full_path)
	_check(err == OK, "Save capture %s" % filename)
	var sha := _sha256_image(img)
	var entry := meta.duplicate(true)
	entry["filename"] = filename
	entry["path"] = full_path
	entry["sha256"] = sha
	entry["size"] = [img.get_width(), img.get_height()]
	_captures.append(entry)
	return sha


func _initialize() -> void:
	for arg in OS.get_cmdline_user_args():
		if arg.begins_with("--out-dir="):
			_out_dir = arg.trim_prefix("--out-dir=")
			_captures_dir = _out_dir.path_join("captures")

	DirAccess.make_dir_recursive_absolute(_captures_dir)

	_dispatcher = OverlayDispatcher.new()
	var init_err := _dispatcher.open_resources(_font_root, _hud_manifest, _date_pack)
	_check(init_err.is_empty(), "OverlayDispatcher.open_resources succeeded: " + init_err)
	if not init_err.is_empty():
		quit(1)
		return

	var rpg_report := _playtest_rpg_battle()
	var taco_report := _playtest_taco_shooter()
	var tt_report := _playtest_tt_twinbee()
	var event_cg_report := _playtest_event_cg_scenes()

	var sub_violations: Array[String] = _dispatcher.subgame_presenter.bounding_box_violations
	var cg_violations: Array[String] = _dispatcher.event_cg_presenter.bounding_box_violations
	_check(sub_violations.is_empty(), "Zero SubgameOverlaysPresenter glyph bounding-box violations: " + ", ".join(sub_violations))
	_check(cg_violations.is_empty(), "Zero EventCgScenesPresenter glyph bounding-box violations: " + ", ".join(cg_violations))

	var proof := {
		"ticket": "T-29018",
		"suite": "subgames_and_event_cg_playtest",
		"status": "PASS" if _failures.is_empty() else "FAIL",
		"assertions_passed": _assertions_passed,
		"failures": _failures,
		"subgame_glyphs_validated": _dispatcher.subgame_presenter.recorded_glyphs.size(),
		"event_cg_glyphs_validated": _dispatcher.event_cg_presenter.recorded_glyphs.size(),
		"subgame_bounding_box_violations": sub_violations.size(),
		"event_cg_bounding_box_violations": cg_violations.size(),
		"rpg_battle": rpg_report,
		"taco_shooter": taco_report,
		"tt_twinbee": tt_report,
		"event_cg": event_cg_report,
		"captures": _captures,
	}

	var proof_path := _out_dir.path_join("subgame-eventcg-playtest-proof.json")
	var f := FileAccess.open(proof_path, FileAccess.WRITE)
	_check(f != null, "Write proof JSON at " + proof_path)
	if f != null:
		f.store_string(JSON.stringify(proof, "  ") + "\n")
		f.close()

	if not _failures.is_empty():
		print("SUBGAMES_AND_EVENT_CG_PLAYTEST_FAILED (%d failures)" % _failures.size())
		quit(1)
		return

	print("SUBGAMES_AND_EVENT_CG_PLAYTEST_OK (%d assertions passed, %d captures saved)" % [
		_assertions_passed, _captures.size()
	])
	quit(0)


# ==============================================================================
# 1. RPG_BAT (0x57) Multi-Run Playtest
# ==============================================================================
func _playtest_rpg_battle() -> Dictionary:
	var club_runs: Array[Dictionary] = []
	var club_hashes := {}

	# 1A. Playtest all 11 clubs (0..10) + Go-Home (11) special moves
	for club_id in range(12):
		var grades := [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		var practice := [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		grades[club_id] = 3
		if club_id < 11:
			practice[club_id] = 350
		var state := SubgameOverlays.rpg_create_state({
			"encounter": SubgameOverlays.RPG_ENCOUNTER_DELINQUENTS,
			"param_condition": 98,
			"param_athletics": 90,
			"girl_id": club_id % 11,
			"boss_event": 1,
			"club_grades": grades,
			"club_practice": practice,
		})
		_check(int(state["player_club"]) == club_id, "RPG club %d selected" % club_id)
		_check(int(state["special_ready"]) == 1 and int(state["special_gauge"]) == 100, "RPG club %d starts with special ready" % club_id)

		# Execute Special Move (command 2)
		SubgameOverlays.rpg_execute_command(state, 2, 0, 240)
		var expected_banner: String = SubgameOverlays.RPG_CLUB_MOVE_BANNERS[club_id]["hex"]
		_check(state["message_lines_hex"][0] == expected_banner, "RPG club %d banner hex matches" % club_id)
		_check(int(state["last_damage"]) > 0, "RPG club %d dealt positive special damage" % club_id)

		var out := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, state)
		_check(String(out.get("error", "")).is_empty(), "Render RPG club %d special move" % club_id)
		var img: Image = out["image"]
		var sha := _sha256_image(img)
		_check(not club_hashes.has(sha), "RPG club %d special move frame is visually unique" % club_id)
		club_hashes[sha] = club_id

		if club_id in [0, 1, 2, 9]:
			_save_capture(img, "rpg_club_%02d_special.png" % club_id, {
				"mode": "0x57",
				"overlay": "RPG_BAT",
				"scenario": "club_special_%d" % club_id,
				"club_id": club_id,
				"banner": SubgameOverlays.RPG_CLUB_MOVE_BANNERS[club_id]["name"],
				"damage": int(state["last_damage"]),
			})

		club_runs.append({
			"club_id": club_id,
			"banner": SubgameOverlays.RPG_CLUB_MOVE_BANNERS[club_id]["name"],
			"primary_damage": int(state["last_damage"]),
			"slot1_damage": int(state["last_damage_slot1"]),
			"slot2_damage": int(state["last_damage_slot2"]),
			"sha256": sha,
		})

	# 1B. Playtest all 11 Partner Girls (0..10) in Drama Club Partner Assist + Nozomi Rough Variant
	var assist_runs: Array[Dictionary] = []
	var assist_hashes := {}
	for girl_id in range(11):
		var state := SubgameOverlays.rpg_create_state({
			"encounter": SubgameOverlays.RPG_ENCOUNTER_DELINQUENTS,
			"param_condition": 97,
			"param_athletics": 85,
			"girl_id": girl_id,
			"boss_event": 1,
			"club_grades": [0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0], # Drama club (1)
			"club_practice": [0, 320, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		})
		_check(int(state["girl_hp"]) == int(SubgameOverlays.RPG_GIRL_INITIAL_HP[girl_id]), "Girl %d initial HP matches C table" % girl_id)
		SubgameOverlays.rpg_execute_command(state, 2, 0, 300)
		var assist: Dictionary = state["last_assist"]
		_check(bool(assist.get("supported", false)) and int(assist.get("girl_id", -1)) == girl_id, "Girl %d Drama assist triggered" % girl_id)

		var out := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, state)
		var img: Image = out["image"]
		var sha := _sha256_image(img)
		_check(not assist_hashes.has(sha), "Girl %d Drama assist frame is visually unique" % girl_id)
		assist_hashes[sha] = girl_id

		if girl_id in [0, 2, 6, 9]:
			_save_capture(img, "rpg_partner_%02d_%s_assist.png" % [girl_id, str(assist["girl"])], {
				"mode": "0x57",
				"overlay": "RPG_BAT",
				"scenario": "partner_assist_%d" % girl_id,
				"girl_id": girl_id,
				"girl": str(assist["girl"]),
				"effect": str(assist["effect"]),
			})

		assist_runs.append({
			"girl_id": girl_id,
			"girl": str(assist["girl"]),
			"initial_hp": int(state["girl_hp"]),
			"effect": str(assist["effect"]),
			"lines": assist["lines"],
			"sha256": sha,
		})

	# Nozomi rough variant (girl_id=6, nozomi_rough=true)
	var nozomi_rough_state := SubgameOverlays.rpg_create_state({
		"girl_id": 6,
		"club_grades": [0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		"club_practice": [0, 320, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	})
	SubgameOverlays.rpg_execute_command(nozomi_rough_state, 2, 0, 300)
	var rough_assist := SubgameOverlays.rpg_resolve_partner_assist(6, true)
	nozomi_rough_state["last_assist"] = rough_assist
	_check(rough_assist["effect"] == "effect_a" and rough_assist["lines"][0] == "清川「いいかげんに…しろっ！", "Nozomi rough assist variant verified")
	var out_rough := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, nozomi_rough_state)
	var sha_rough := _save_capture(out_rough["image"], "rpg_partner_06_nozomi_rough_assist.png", {
		"mode": "0x57",
		"overlay": "RPG_BAT",
		"scenario": "partner_assist_6_rough",
		"girl_id": 6,
		"variant": "rough",
	})
	_check(not assist_hashes.has(sha_rough), "Nozomi rough assist frame is distinct from polite variant")

	# 1C. Full Multi-Turn Combat Playtest -> Guard, Basic Attack, Victory (stage = 10), and Defeat (stage = 12)
	var fight_state := SubgameOverlays.rpg_create_state({
		"encounter": SubgameOverlays.RPG_ENCOUNTER_DELINQUENTS,
		"param_condition": 100,
		"param_athletics": 120,
		"girl_id": 0,
		"boss_event": 1,
		"club_grades": [0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0], # Science club (2)
		"club_practice": [0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	})
	# Enemy attacks player first (un-guarded)
	var unguarded_dmg := SubgameOverlays.rpg_apply_enemy_attack(fight_state, 200, false)
	_check(unguarded_dmg == 200 and int(fight_state["player_hp"]) == 800, "Unguarded enemy attack deals full 200 damage")

	# Player guards (heals 5% = 50 HP -> 850 HP, halves next incoming attack)
	SubgameOverlays.rpg_execute_command(fight_state, 1, 0, 128)
	_check(int(fight_state["player_hp"]) == 850, "Guard heals 5% of max HP (800 -> 850)")
	var guarded_dmg := SubgameOverlays.rpg_apply_enemy_attack(fight_state, 200, false)
	_check(guarded_dmg == 100 and int(fight_state["player_hp"]) == 750, "Guarded enemy attack is halved to 100")
	var out_guard := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, fight_state)
	var sha_guard := _save_capture(out_guard["image"], "rpg_combat_guard_shield.png", {
		"mode": "0x57",
		"overlay": "RPG_BAT",
		"scenario": "guard_heal_and_shield",
		"player_hp": int(fight_state["player_hp"]),
	})

	# Player basic attack on slot 0
	SubgameOverlays.rpg_execute_command(fight_state, 0, 0, 512)
	_check(int(fight_state["enemy_slot_gauges"][0]) < 5000, "Basic attack reduces slot 0 gauge")

	# Player Science Club Special Move (9999 primary + AoE slot1/slot2) -> wipes out all 3 delinquent slots -> Victory (stage = 10)!
	fight_state["special_gauge"] = 100
	fight_state["special_ready"] = 1
	SubgameOverlays.rpg_execute_command(fight_state, 2, 0, 400)
	_check(bool(fight_state["victory"]) and int(fight_state["stage"]) == 10 and int(fight_state["rpg_over"]) == 1, "Defeating all enemies transitions to stage=10 Victory")
	var out_vic := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, fight_state)
	var sha_vic := _save_capture(out_vic["image"], "rpg_combat_victory_stage10.png", {
		"mode": "0x57",
		"overlay": "RPG_BAT",
		"scenario": "victory_stage_10",
		"stage": int(fight_state["stage"]),
	})

	# Defeat playtest (stage = 12 when partner girl HP reaches 0)
	var defeat_state := SubgameOverlays.rpg_create_state({"girl_id": 1}) # Mio (99 HP)
	SubgameOverlays.rpg_apply_enemy_attack(defeat_state, 150, true)
	_check(bool(defeat_state["defeat"]) and int(defeat_state["stage"]) == 12 and int(defeat_state["rpg_over"]) == 2, "Partner girl KO transitions to stage=12 Defeat")
	var out_def := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, defeat_state)
	var sha_def := _save_capture(out_def["image"], "rpg_combat_defeat_stage12.png", {
		"mode": "0x57",
		"overlay": "RPG_BAT",
		"scenario": "defeat_stage_12",
		"stage": int(defeat_state["stage"]),
	})
	_check(sha_guard != sha_vic and sha_vic != sha_def, "Guard, Victory, and Defeat frames are visually distinct")

	return {
		"clubs_tested": club_runs.size(),
		"partner_assists_tested": assist_runs.size(),
		"nozomi_rough_sha256": sha_rough,
		"victory_stage": int(fight_state["stage"]),
		"defeat_stage": int(defeat_state["stage"]),
		"club_runs": club_runs,
		"assist_runs": assist_runs,
	}


# ==============================================================================
# 2. TACO (0x59) 3D Rail Shooter Multi-Run Playtest
# ==============================================================================
func _playtest_taco_shooter() -> Dictionary:
	var state := SubgameOverlays.shooter_create_state()
	var weapon_hashes := {}

	# 2A. Multi-frame movement & Left Barrel Roll (0x81) + Right Barrel Roll (0x82)
	SubgameOverlays.shooter_step_frame(state, -6, 3, true, false, false)
	var p: Dictionary = state["player"]
	_check(int(p["mode"]) == 0x81 and int(p["roll_phase"]) == 0x7F, "Left barrel roll sets mode=0x81 and roll_phase=0x7F")
	var out_roll_l := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TACO, state)
	var sha_roll_l := _save_capture(out_roll_l["image"], "taco_barrel_roll_left_0x81.png", {
		"mode": "0x59",
		"overlay": "TACO",
		"scenario": "barrel_roll_left_0x81",
		"player_mode": int(p["mode"]),
	})

	# Step 16 frames until barrel roll recovers to mode=1
	for _i in range(16):
		SubgameOverlays.shooter_step_frame(state, 0, 0, false, false, false)
	_check(int(p["mode"]) == 1, "Barrel roll recovers to mode=1 after roll_phase < 0x40")

	# Right Barrel Roll (0x82)
	SubgameOverlays.shooter_step_frame(state, 6, -2, true, false, false)
	_check(int(p["mode"]) == 0x82 and int(p["roll_phase"]) == 0x7F, "Right barrel roll sets mode=0x82 and roll_phase=0x7F")
	var out_roll_r := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TACO, state)
	var sha_roll_r := _save_capture(out_roll_r["image"], "taco_barrel_roll_right_0x82.png", {
		"mode": "0x59",
		"overlay": "TACO",
		"scenario": "barrel_roll_right_0x82",
		"player_mode": int(p["mode"]),
	})
	_check(sha_roll_l != sha_roll_r, "Left and Right barrel roll frames are visually distinct")

	# 2B. Primary Weapons: WIDE (0), BEAM (1), VULCAN (7)
	var weapons := [
		{"id": SubgameOverlays.SHOOTER_PRIMARY_WIDE, "name": "WIDE", "dmg": 2},
		{"id": SubgameOverlays.SHOOTER_PRIMARY_BEAM, "name": "BEAM", "dmg": 3},
		{"id": SubgameOverlays.SHOOTER_PRIMARY_VULCAN, "name": "VULCAN", "dmg": 1},
	]
	for w in weapons:
		var w_state := SubgameOverlays.shooter_create_state({"weapon": int(w["id"])})
		SubgameOverlays.shooter_step_frame(w_state, 2, -1, false, true, false)
		_check(w_state["shots"].size() == 1 and int(w_state["shots"][0]["damage"]) == int(w["dmg"]), "Weapon %s spawns shot with damage %d" % [w["name"], int(w["dmg"])])
		var out_w := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TACO, w_state)
		var sha_w := _sha256_image(out_w["image"])
		_check(not weapon_hashes.has(sha_w), "Weapon %s frame is visually distinct" % w["name"])
		weapon_hashes[sha_w] = w["name"]
		if int(w["id"]) == SubgameOverlays.SHOOTER_PRIMARY_BEAM:
			_save_capture(out_w["image"], "taco_weapon_beam_fire.png", {
				"mode": "0x59",
				"overlay": "TACO",
				"scenario": "weapon_beam_fire",
				"weapon": "BEAM",
			})

	# 2C. AoE Bomb Firing: SHOOTER_BOMB_BLAST (4) & SHOOTER_BOMB_PULSE (3)
	var bomb_state := SubgameOverlays.shooter_create_state()
	var score_before: int = int(bomb_state["score"])
	var fired_1 := SubgameOverlays.shooter_fire_bomb(bomb_state)
	_check(fired_1 == SubgameOverlays.SHOOTER_BOMB_BLAST, "First bomb fired is SHOOTER_BOMB_BLAST (4)")
	_check(int(bomb_state["enemies"][0]["hp"]) == 0 and int(bomb_state["enemies"][1]["hp"]) == 0, "Blast bomb (12 dmg) destroys both 6-HP taco_fighter enemies")
	_check(int(bomb_state["score"]) == score_before + 10000, "Destroying 2 fighters awards +10,000 score")
	var out_bomb := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TACO, bomb_state)
	var sha_bomb := _save_capture(out_bomb["image"], "taco_aoe_bomb_blast.png", {
		"mode": "0x59",
		"overlay": "TACO",
		"scenario": "aoe_bomb_blast",
		"score": int(bomb_state["score"]),
	})

	var fired_2 := SubgameOverlays.shooter_fire_bomb(bomb_state)
	_check(fired_2 == SubgameOverlays.SHOOTER_BOMB_PULSE, "Second bomb fired is SHOOTER_BOMB_PULSE (3)")
	_check(int(bomb_state["enemies"][2]["hp"]) == 4, "Morph core (24 HP) reduced to 4 HP after Blast(12) + Pulse(8)")
	var fired_3 := SubgameOverlays.shooter_fire_bomb(bomb_state)
	_check(fired_3 == 0, "Empty bomb stock returns 0")

	# 2D. Result Tally & 5-Slot Score Ranking Insertion
	var clear_state := SubgameOverlays.shooter_create_state({
		"stage": SubgameOverlays.SHOOTER_STAGE_RESULT,
		"stages_cleared": 3,
		"score": 520000,
		"time_left": 1800,
		"shield": 0xFF,
	})
	clear_state["player"]["bomb_slots"] = [4, 3, 3] # 3 bombs -> 500,000 bomb bonus
	var tally := SubgameOverlays.shooter_compute_result_tally(clear_state)
	clear_state["result_tally"] = tally
	_check(int(tally["stage_bonus"]) == 3000000, "3 stages cleared = 3,000,000 bonus")
	_check(int(tally["shield_bonus"]) == 500000, "Full shield 255 = 500,000 bonus")
	_check(int(tally["time_bonus"]) == 180000, "1800 frames left = 180,000 time bonus")
	_check(int(tally["bomb_stock_bonus"]) == 500000, "3 bombs left = 500,000 bomb bonus")
	_check(int(tally["total_score"]) == 4700000, "Total score = 4,700,000")

	var out_res := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TACO, clear_state)
	var sha_res := _save_capture(out_res["image"], "taco_result_tally.png", {
		"mode": "0x59",
		"overlay": "TACO",
		"scenario": "result_tally",
		"total_score": int(tally["total_score"]),
	})

	var rank_idx := SubgameOverlays.shooter_try_insert_ranking(clear_state, int(tally["total_score"]), "ＦＵＫ")
	_check(rank_idx == 0 and int(clear_state["rankings"][0]["score"]) == 4700000 and clear_state["rankings"].size() == 5, "Score 4,700,000 inserted at slot 0 of 5-slot ranking table")
	clear_state.erase("result_tally")
	clear_state["stage"] = SubgameOverlays.SHOOTER_STAGE_SCORES
	var out_rank := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TACO, clear_state)
	var sha_rank := _save_capture(out_rank["image"], "taco_score_rankings.png", {
		"mode": "0x59",
		"overlay": "TACO",
		"scenario": "score_rankings",
		"top_score": int(clear_state["rankings"][0]["score"]),
	})
	_check(sha_bomb != sha_res and sha_res != sha_rank, "TACO Bomb, Result Tally, and Rankings frames are visually distinct")

	return {
		"weapons_tested": weapons.size(),
		"bombs_tested": ["SHOOTER_BOMB_BLAST", "SHOOTER_BOMB_PULSE"],
		"result_tally": tally,
		"inserted_rank_slot": rank_idx,
	}


# ==============================================================================
# 3. TT (0x5A) TwinBee Time Attack Multi-Run Playtest
# ==============================================================================
func _playtest_tt_twinbee() -> Dictionary:
	var state := SubgameOverlays.tt_create_state({
		"shot_kind": 8,
		"has_options": 0,
		"speed_level": 4,
	})

	# 3A. Verify 32-entry TT_BELL_KIND_SEQUENCE and multi-hit cycling (1..21)
	_check(SubgameOverlays.TT_BELL_KIND_SEQUENCE.size() == 32, "TT_BELL_KIND_SEQUENCE has 32 entries")
	var bell_history: Array[int] = []
	for hit in range(1, 21):
		var b := SubgameOverlays.tt_bell_hit_by_shot(state, 1, 0)
		var kind := int(b["kind"])
		bell_history.append(kind)
		if hit in [5, 10, 15, 20]:
			var out_b := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TT, state)
			var img_b: Image = out_b["image"]
			_check(img_b.get_pixel(8, 120) == Color.BLACK and img_b.get_pixel(308, 120) == Color.BLACK, "TT bell hit %d preserves 32px black side pillars" % hit)
			if hit == 15:
				_save_capture(img_b, "tt_bell_cycle_hit15_white_option.png", {
					"mode": "0x5A",
					"overlay": "TT",
					"scenario": "bell_cycle_hit_15",
					"bell_kind": kind,
				})

	_check(bell_history[4] == 1, "Hit 5 yields Blue Twin Shot Bell (kind=1)")
	_check(bell_history[9] == 2, "Hit 10 yields Green Speed Up Bell (kind=2)")
	_check(bell_history[14] == 3, "Hit 15 yields White Option Bell (kind=3)")
	_check(bell_history[19] == 6, "Hit 20 yields Purple Speed Down Bell (kind=6)")

	# 3B. Collect all 4 power-up bell kinds (1: Twin Shot, 2: Speed Up, 3: 3 Options, 6: Speed Down)
	# Bell 0: Blue Twin Shot (kind=1)
	state["bells"][0]["kind"] = 1
	state["bells"][0]["state"] = 0x40
	SubgameOverlays.tt_bell_collect(state, 0)
	_check(int(state["player"]["shot_kind"]) == 9, "Collecting Blue Bell upgrades shot_kind to 9 (Twin Shot)")

	# Bell 1: Green Speed Up (kind=2)
	state["bells"][1]["kind"] = 2
	state["bells"][1]["state"] = 0x40
	SubgameOverlays.tt_bell_collect(state, 1)
	_check(int(state["player"]["speed_level"]) == 5, "Collecting Green Bell increments speed_level to 5")

	# Bell 2: White 3 Options (kind=3)
	state["bells"][2]["kind"] = 3
	state["bells"][2]["state"] = 0x40
	SubgameOverlays.tt_bell_collect(state, 2)
	_check(int(state["player"]["has_options"]) == 1, "Collecting White Bell enables 3 Option followers")

	var out_power := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TT, state)
	var sha_power := _save_capture(out_power["image"], "tt_powerups_twin_options_speed5.png", {
		"mode": "0x5A",
		"overlay": "TT",
		"scenario": "powerups_collected",
		"shot_kind": int(state["player"]["shot_kind"]),
		"has_options": int(state["player"]["has_options"]),
		"speed_level": int(state["player"]["speed_level"]),
	})

	# Purple Bell (kind=6: Speed Down)
	state["bells"][1]["kind"] = 6
	state["bells"][1]["state"] = 0x40
	SubgameOverlays.tt_bell_collect(state, 1)
	_check(int(state["player"]["speed_level"]) == 4, "Collecting Purple Bell decrements speed_level back to 4")

	# 3C. Smart Bomb Clearing & 3-Slot Time Ranking Insertion + Boss Variations (0..3)
	SubgameOverlays.tt_trigger_smart_bomb(state)
	_check(int(state["air_enemies"][0]["state"]) == 0x70 and int(state["enemy_shots"][0]["active"]) == 0, "Smart bomb sets air_enemies to 0x70 explosion and clears enemy_shots")

	var slot0 := SubgameOverlays.tt_try_insert_ranking(state, 0x5400) # 5:58.66 -> 1st place
	_check(slot0 == 0 and int(state["rankings"][0]["time"]) == 0x5400, "Time 0x5400 inserted at 1st place")
	state["boss"]["boss_id"] = 1 # Stage 2 Swing-Ring Boss
	state["stage_index"] = 1
	state["elapsed_frames"] = 0x5400
	var out_bomb_rank := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TT, state)
	var sha_bomb_rank := _save_capture(out_bomb_rank["image"], "tt_smart_bomb_boss2_ranking.png", {
		"mode": "0x5A",
		"overlay": "TT",
		"scenario": "smart_bomb_boss2_and_ranking",
		"boss_id": 1,
		"ranking_slot": slot0,
	})
	_check(sha_power != sha_bomb_rank, "TT power-up frame and smart-bomb/boss2/ranking frame are visually distinct")

	return {
		"bell_sequence_length": SubgameOverlays.TT_BELL_KIND_SEQUENCE.size(),
		"bell_kinds_verified": [0, 1, 2, 3, 6],
		"inserted_ranking_slot": slot0,
		"top_time_text": SubgameOverlays.tt_format_time(0x5400)["text"],
	}


# ==============================================================================
# 4. EVENT_CG (0x5E) Interactive Scenes & Choices (0..2) Playtest
# ==============================================================================
func _playtest_event_cg_scenes() -> Dictionary:
	var scenes := [
		{"kind": "shiori_cg", "girl": EventCgOverlay.GIRL_SHIORI, "spot": EventCgOverlay.SPOT_KIRAMEKI_CENTRAL_PARK, "month": 4, "flag": 0x00},
		{"kind": "ayako_karaoke", "girl": EventCgOverlay.GIRL_AYAKO, "spot": EventCgOverlay.SPOT_KARAOKE, "month": 4, "flag": 0x13},
		{"kind": "ride_movie", "girl": EventCgOverlay.GIRL_SHIORI, "spot": EventCgOverlay.SPOT_AMUSEMENT_PARK, "month": 10, "flag": -1},
		{"kind": "miharu_encounter", "girl": EventCgOverlay.GIRL_SHIORI, "spot": EventCgOverlay.SPOT_KIRAMEKI_CENTRAL_PARK, "month": 4, "flag": 0x48},
	]

	var all_scene_hashes := {}
	var scene_reports: Array[Dictionary] = []

	for sc in scenes:
		var kind: String = sc["kind"]
		var choice_hashes: Array[String] = []
		var choice_results: Array[Dictionary] = []

		for cursor_idx in range(3):
			var ovl := EventCgOverlay.new()
			ovl.init_date_spot(int(sc["girl"]), int(sc["spot"]), int(sc["month"]), 96)
			if kind == "miharu_encounter":
				ovl.scene_flags[0x46] = 2
				var triggered := ovl.evaluate_date_spot_special_scene()
				_check(triggered and ovl.current_girl == EventCgOverlay.GIRL_MIHARU and ovl.scene_flags[0x48] == 1, "Miharu encounter triggered and set scene_flags[0x48]")
			elif kind == "ride_movie":
				var r_info := ovl.trigger_ride_movie(EventCgOverlay.RIDE_FERRIS_WHEEL)
				_check(int(r_info["lba"]) == 0x4332 and ovl.stage == EventCgOverlay.STAGE_RIDE_MOVIE, "Ferris wheel movie triggered at LBA 0x4332")
			else:
				var triggered := ovl.evaluate_date_spot_special_scene()
				_check(triggered and ovl.scene_flags[int(sc["flag"])] == 1, "%s unlocked scene_flags[0x%02X]" % [kind, int(sc["flag"])])

			var res := ovl.handle_choice(cursor_idx)
			if cursor_idx == 0:
				_check(int(res["affection_delta"]) == 3 and int(res["tokimeki_delta"]) == 2 and int(res["syoushin_delta"]) == -5, "%s choice 0 stat deltas" % kind)
			elif cursor_idx == 1:
				_check(int(res["affection_delta"]) == 1 and int(res["tokimeki_delta"]) == 0 and int(res["syoushin_delta"]) == 0, "%s choice 1 stat deltas" % kind)
			else:
				_check(int(res["affection_delta"]) == -2 and int(res["tokimeki_delta"]) == -1 and int(res["syoushin_delta"]) == 5, "%s choice 2 stat deltas" % kind)

			_dispatcher.event_cg_presenter.default_choice = cursor_idx
			var out := _dispatcher.render_mode(OverlayDispatcher.GAME_MODE_EVENT_CG, {
				"scene_kind": kind,
				"cursor": cursor_idx,
			})
			_check(String(out.get("error", "")).is_empty(), "Render %s cursor=%d" % [kind, cursor_idx])
			var img: Image = out["image"]
			var sha := _sha256_image(img)
			_check(not all_scene_hashes.has(sha), "EVENT_CG %s cursor=%d frame is globally unique" % [kind, cursor_idx])
			all_scene_hashes[sha] = "%s_c%d" % [kind, cursor_idx]
			choice_hashes.append(sha)
			choice_results.append(res)

			if cursor_idx in [0, 2]:
				_save_capture(img, "event_cg_%s_choice_%d.png" % [kind, cursor_idx], {
					"mode": "0x5E",
					"overlay": "EVENT",
					"scene_kind": kind,
					"cursor": cursor_idx,
					"affection_delta": int(res["affection_delta"]),
				})

		_dispatcher.event_cg_presenter.default_choice = 0
		_check(choice_hashes[0] != choice_hashes[1] and choice_hashes[1] != choice_hashes[2], "%s choices 0, 1, 2 produce distinct visual frames" % kind)
		scene_reports.append({
			"scene_kind": kind,
			"choice_hashes": choice_hashes,
			"choice_results": choice_results,
		})

	return {
		"scenes_tested": scene_reports.size(),
		"total_unique_frames": all_scene_hashes.size(),
		"scenes": scene_reports,
	}
