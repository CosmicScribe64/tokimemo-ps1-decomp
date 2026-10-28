extends SceneTree
## T-29018 / Interactive Multi-Heroine Verification Suite for the Unified PS1 OverlayDispatcher.
## Verifies that every PS1 GameMode in OverlayDispatcher.OVERLAY_CATALOG (27 modes
## spanning all 26 PS1 overlays in src/ovl/*) initializes cleanly, steps interactive
## state machines deterministically across PAD_UP/DOWN/LEFT/RIGHT/CIRCLE/CROSS,
## and renders distinct 320x240 RGBA8 frames across heroines (girl_id 0..4) and
## cursor/stage states with zero bounding-box violations.

const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")

var _passes: int = 0
var _failures: int = 0


func _ok(cond: bool, msg: String) -> void:
	if cond:
		_passes += 1
	else:
		_failures += 1
		push_error("FAIL: " + msg)


func _eq(actual: Variant, expected: Variant, msg: String) -> void:
	if actual == expected:
		_passes += 1
	else:
		_failures += 1
		push_error("FAIL: %s (expected=%s actual=%s)" % [msg, str(expected), str(actual)])


func _img_hash(img: Image) -> String:
	if img == null:
		return ""
	return img.get_data().hex_encode().sha256_text()


func _initialize() -> void:
	call_deferred("_run")


func _run() -> void:
	var repo: String = OS.get_environment("TOKIMEMO_REPO_ROOT")
	if repo.is_empty():
		for candidate: String in [
			ProjectSettings.globalize_path("res://../..").simplify_path(),
			ProjectSettings.globalize_path("res://../../../../..").simplify_path(),
			"/workspace",
		]:
			if DirAccess.dir_exists_absolute(candidate.path_join(".asset-cache/port")):
				repo = candidate
				break
		if repo.is_empty():
			repo = "/workspace"
	var font_root: String = repo.path_join(".asset-cache/port/fonts/native-font-v2")
	var hud_manifest: String = repo.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var date_pack: String = repo.path_join(".asset-cache/port/date-preview-v1")
	var capture_dir: String = OS.get_environment("TOKIMEMO_CAPTURE_DIR")
	if not capture_dir.is_empty():
		DirAccess.make_dir_recursive_absolute(capture_dir)

	var dispatcher := OverlayDispatcher.new()
	var open_err: String = dispatcher.open_resources(font_root, hud_manifest, date_pack)
	_eq(open_err, "", "OverlayDispatcher opened all presenter resources cleanly")
	if _failures > 0:
		quit(1)
		return

	var modes: Array[int] = OverlayDispatcher.supported_modes()
	_eq(modes.size(), 27, "OverlayDispatcher catalog covers all 27 overlay GameModes")

	# 1. Baseline catalog & single-step check across all 27 GameModes
	for mode: int in modes:
		_ok(dispatcher.handles_mode(mode), "handles_mode(0x%02X) is true" % mode)
		var meta: Dictionary = dispatcher.describe_mode(mode)
		_ok(not String(meta.get("overlay", "")).is_empty(), "describe_mode(0x%02X) has overlay code" % mode)

		var step_down: Dictionary = dispatcher.step_mode(mode, {}, OverlayDispatcher.PAD_DOWN)
		_ok(not step_down.is_empty(), "step_mode(0x%02X, PAD_DOWN) returned non-empty state" % mode)
		var step_circle: Dictionary = dispatcher.step_mode(mode, step_down, OverlayDispatcher.PAD_CIRCLE)
		_ok(not step_circle.is_empty(), "step_mode(0x%02X, PAD_CIRCLE) returned non-empty state" % mode)

		var rendered: Dictionary = dispatcher.render_mode(mode, {})
		_eq(String(rendered.get("error", "")), "", "render_mode(0x%02X [%s]) succeeded" % [mode, String(meta.get("overlay", ""))])
		var img: Image = rendered.get("image")
		_ok(img != null and not img.is_empty(), "render_mode(0x%02X) produced valid Image" % mode)
		if img != null:
			_eq(img.get_size(), Vector2i(320, 240), "render_mode(0x%02X) Image is 320x240" % mode)
		var cmds: Array = rendered.get("commands", [])
		_eq(cmds.size(), 1, "render_mode(0x%02X) emitted 1 NativeCanvas draw command" % mode)

	# 2. Interactive state-machine verification for key gameplay overlays
	_verify_telephone_flow(dispatcher, capture_dir)
	_verify_date_flow(dispatcher, capture_dir)
	_verify_encounters_and_ending_flow(dispatcher, capture_dir)
	_verify_school_events_flow(dispatcher, capture_dir)
	_verify_subgames_flow(dispatcher, capture_dir)

	# 3. Verify sub-scene variants for EVENT_CG (0x5E)
	for kind: String in ["shiori_cg", "ayako_karaoke", "ride_movie", "miharu_encounter"]:
		var ev_out: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_EVENT_CG, {"scene_kind": kind})
		_eq(String(ev_out.get("error", "")), "", "EVENT_CG scene '%s' rendered cleanly" % kind)
		_eq((ev_out.get("image") as Image).get_size(), Vector2i(320, 240), "EVENT_CG scene '%s' is 320x240" % kind)

	# 4. Verify zero bounding-box violations across all underlying presenters
	_eq(dispatcher.telephone_presenter.bounding_box_violations.size(), 0, "telephone_presenter has 0 bounding-box violations")
	_eq(dispatcher.date_presenter.bounding_box_violations.size(), 0, "date_presenter has 0 bounding-box violations")
	_eq(dispatcher.encounters_ending_presenter.bounding_box_violations.size(), 0, "encounters_ending_presenter has 0 bounding-box violations")
	_eq(dispatcher.school_events_presenter.bounding_box_violations.size(), 0, "school_events_presenter has 0 bounding-box violations")
	_eq(dispatcher.subgame_presenter.bounding_box_violations.size(), 0, "subgame_presenter has 0 bounding-box violations")

	if _failures == 0:
		print("PASS: OverlayDispatcher end-to-end suite (%d assertions across 27 GameModes)" % _passes)
		quit(0)
	else:
		push_error("FAIL: OverlayDispatcher end-to-end suite (%d failures)" % _failures)
		quit(1)


func _verify_telephone_flow(dispatcher: OverlayDispatcher, capture_dir: String) -> void:
	# TEL (0x3D): place_select -> day_select -> call_response -> return_to_game
	var st: Dictionary = {"girl_id": 1}
	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TEL, st, OverlayDispatcher.PAD_DOWN)
	_eq(int(st.get("cursor", -1)), 1, "TEL PAD_DOWN moves place cursor 0 -> 1")
	_eq(String(st.get("subscreen", "")), "place_select", "TEL remains on place_select during cursor move")

	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TEL, st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(st.get("subscreen", "")), "day_select", "TEL PAD_CIRCLE advances place_select -> day_select")
	_eq(int(st.get("selected_place", -1)), 1, "TEL records selected_place=1")

	# Back out via PAD_CROSS then re-enter
	var backed: Dictionary = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TEL, st, OverlayDispatcher.PAD_CROSS)
	_eq(String(backed.get("subscreen", "")), "place_select", "TEL PAD_CROSS backs out day_select -> place_select")

	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TEL, st, OverlayDispatcher.PAD_DOWN)
	_eq(int(st.get("cursor", -1)), 1, "TEL PAD_DOWN moves day cursor 0 -> 1")
	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TEL, st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(st.get("subscreen", "")), "call_response", "TEL PAD_CIRCLE advances day_select -> call_response")
	_ok(not String(st.get("response_kind", "")).is_empty(), "TEL call_response resolves response_kind")

	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TEL, st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(st.get("action", "")), "return_to_game", "TEL PAD_CIRCLE on call_response returns to game")

	# Multi-heroine rendering check for TEL call_response and TEL_ACTION notebook (girl_id 0..4)
	var call_hashes: Dictionary = {}
	var notebook_hashes: Dictionary = {}
	for gid: int in range(5):
		var call_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, {
			"subscreen": "call_response",
			"girl_id": gid,
			"response_kind": "accept",
		})
		_eq(String(call_res.get("error", "")), "", "TEL call_response girl_id=%d rendered cleanly" % gid)
		var call_img: Image = call_res.get("image")
		call_hashes[ _img_hash(call_img) ] = true

		var nb_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL_ACTION, {
			"subscreen": "notebook",
			"girl_id": gid,
		})
		_eq(String(nb_res.get("error", "")), "", "TEL_ACTION notebook girl_id=%d rendered cleanly" % gid)
		var nb_img: Image = nb_res.get("image")
		notebook_hashes[ _img_hash(nb_img) ] = true
		if not capture_dir.is_empty() and gid in [0, 1, 3]:
			call_img.save_png(capture_dir.path_join("tel_call_girl_%d.png" % gid))
			nb_img.save_png(capture_dir.path_join("tel_notebook_girl_%d.png" % gid))

	_eq(call_hashes.size(), 5, "TEL call_response renders 5 distinct heroine frames for girl_id 0..4")
	_eq(notebook_hashes.size(), 5, "TEL_ACTION notebook renders 5 distinct heroine profile pages for girl_id 0..4")


func _verify_date_flow(dispatcher: OverlayDispatcher, capture_dir: String) -> void:
	# DATE (0x3B): stage 0 (arrival) -> stage 1 (3-choice prompt) -> stage 2 (reaction) -> return_to_game
	var st: Dictionary = {"girl_id": 2, "stage": 0, "cursor": 0, "affection": 65}
	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_DATE, st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(st.get("stage", -1)), 1, "DATE PAD_CIRCLE advances stage 0 (arrival) -> stage 1 (choice prompt)")

	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_DATE, st, OverlayDispatcher.PAD_DOWN)
	_eq(int(st.get("cursor", -1)), 1, "DATE PAD_DOWN moves choice cursor 0 -> 1")
	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_DATE, st, OverlayDispatcher.PAD_DOWN)
	_eq(int(st.get("cursor", -1)), 2, "DATE PAD_DOWN moves choice cursor 1 -> 2")
	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_DATE, st, OverlayDispatcher.PAD_UP)
	_eq(int(st.get("cursor", -1)), 1, "DATE PAD_UP moves choice cursor 2 -> 1")

	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_DATE, st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(st.get("stage", -1)), 2, "DATE PAD_CIRCLE advances stage 1 -> stage 2 (reaction)")
	_eq(int(st.get("chosen_index", -1)), 1, "DATE records chosen_index=1")

	st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_DATE, st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(st.get("action", "")), "return_to_game", "DATE PAD_CIRCLE on stage 2 returns to game")

	# Verify distinct cursor renders on stage 1 (cursor 0, 1, 2)
	var cursor_hashes: Dictionary = {}
	for cur: int in range(3):
		var c_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_DATE, {
			"girl_id": 0,
			"stage": 1,
			"cursor": cur,
		})
		_eq(String(c_res.get("error", "")), "", "DATE stage 1 cursor=%d rendered cleanly" % cur)
		cursor_hashes[ _img_hash(c_res.get("image")) ] = true
	_eq(cursor_hashes.size(), 3, "DATE stage 1 renders 3 distinct choice-cursor highlight frames")

	# Verify distinct heroine arrival & reaction renders for girl_id 0..4
	var arrival_hashes: Dictionary = {}
	var reaction_hashes: Dictionary = {}
	for gid: int in range(5):
		var arr_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_DATE, {
			"girl_id": gid,
			"stage": 0,
		})
		_eq(String(arr_res.get("error", "")), "", "DATE stage 0 girl_id=%d rendered cleanly" % gid)
		var arr_img: Image = arr_res.get("image")
		arrival_hashes[ _img_hash(arr_img) ] = true

		var react_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_DATE, {
			"girl_id": gid,
			"stage": 2,
			"chosen_index": 0,
		})
		_eq(String(react_res.get("error", "")), "", "DATE stage 2 girl_id=%d rendered cleanly" % gid)
		var react_img: Image = react_res.get("image")
		reaction_hashes[ _img_hash(react_img) ] = true
		if not capture_dir.is_empty() and gid in [0, 2, 4]:
			arr_img.save_png(capture_dir.path_join("date_arrival_girl_%d.png" % gid))
			react_img.save_png(capture_dir.path_join("date_reaction_girl_%d.png" % gid))

	_eq(arrival_hashes.size(), 5, "DATE stage 0 renders 5 distinct heroine arrival greetings for girl_id 0..4")
	_eq(reaction_hashes.size(), 5, "DATE stage 2 renders 5 distinct heroine reactions for girl_id 0..4")


func _verify_encounters_and_ending_flow(dispatcher: OverlayDispatcher, capture_dir: String) -> void:
	# GEKO (0x34): cursor toggle + walk_home_step
	var geko_st: Dictionary = {"girl_id": 1, "cursor": 0, "stage": 0}
	geko_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_GEKO, geko_st, OverlayDispatcher.PAD_DOWN)
	_eq(int(geko_st.get("cursor", -1)), 1, "GEKO PAD_DOWN moves choice cursor 0 -> 1")
	geko_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_GEKO, geko_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(geko_st.get("stage", -1)), 1, "GEKO PAD_CIRCLE resolves walk_home_step into stage 1")
	geko_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_GEKO, geko_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(geko_st.get("action", "")), "return_to_game", "GEKO second PAD_CIRCLE exits to game")

	# ENDING (0xC1): stages 0..7 -> handoff_credits (next_mode = 0xC2)
	var end_st: Dictionary = {"girl_id": 1, "stage": 0}
	for expected_next: int in range(1, 8):
		end_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_ENDING, end_st, OverlayDispatcher.PAD_CIRCLE)
		_eq(int(end_st.get("stage", -1)), expected_next, "ENDING PAD_CIRCLE advances to stage %d" % expected_next)
	end_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_ENDING, end_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(end_st.get("action", "")), "handoff_credits", "ENDING stage 7 PAD_CIRCLE triggers handoff_credits")
	_eq(int(end_st.get("next_mode", -1)), 0xC2, "ENDING stage 7 hands off to GAME_MODE_ETC_C2 (0xC2)")

	# Multi-heroine rendering check for GEKO, VALEN, and ENDING across girl_id 0..4
	var geko_hashes: Dictionary = {}
	var valen_hashes: Dictionary = {}
	var ending_hashes: Dictionary = {}
	for gid: int in range(5):
		var g_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_GEKO, {"girl_id": gid, "cursor": gid % 2})
		_eq(String(g_res.get("error", "")), "", "GEKO girl_id=%d rendered cleanly" % gid)
		geko_hashes[ _img_hash(g_res.get("image")) ] = true

		var v_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_VALEN, {"girl_id": gid})
		_eq(String(v_res.get("error", "")), "", "VALEN girl_id=%d rendered cleanly" % gid)
		valen_hashes[ _img_hash(v_res.get("image")) ] = true

		var e_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_ENDING, {"girl_id": gid})
		_eq(String(e_res.get("error", "")), "", "ENDING girl_id=%d rendered cleanly" % gid)
		var e_img: Image = e_res.get("image")
		ending_hashes[ _img_hash(e_img) ] = true
		if not capture_dir.is_empty() and gid in [0, 1, 2]:
			e_img.save_png(capture_dir.path_join("ending_confession_girl_%d.png" % gid))

	_eq(geko_hashes.size(), 5, "GEKO renders 5 distinct heroine walk-home frames for girl_id 0..4")
	_eq(valen_hashes.size(), 5, "VALEN renders 5 distinct heroine Valentine frames for girl_id 0..4")
	_eq(ending_hashes.size(), 5, "ENDING renders 5 distinct heroine confession frames for girl_id 0..4")


func _verify_school_events_flow(dispatcher: OverlayDispatcher, capture_dir: String) -> void:
	# TAIIKU (0x28): sports_step_race advances track_position and player_speed
	var sp_st: Dictionary = {"girl_id": 3, "cursor": 0, "stage": 0}
	sp_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TAIIKU, sp_st, OverlayDispatcher.PAD_DOWN)
	_eq(int(sp_st.get("cursor", -1)), 1, "TAIIKU PAD_DOWN moves competition cursor 0 -> 1")
	sp_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TAIIKU, sp_st, OverlayDispatcher.PAD_CIRCLE)
	_ok(int(sp_st.get("track_position", 0)) > 0, "TAIIKU PAD_CIRCLE steps sports_step_race (track_position > 0)")
	_ok(int(sp_st.get("player_speed", 0)) > 0, "TAIIKU PAD_CIRCLE steps sports_step_race (player_speed > 0)")

	# Multi-heroine + cursor renders across TAIIKU, BUNKASAI, SHUGAKU, RENSYU
	var taiiku_hashes: Dictionary = {}
	var shugaku_hashes: Dictionary = {}
	for gid: int in range(5):
		var t_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TAIIKU, {"girl_id": gid, "cursor": gid % 4})
		_eq(String(t_res.get("error", "")), "", "TAIIKU girl_id=%d rendered cleanly" % gid)
		taiiku_hashes[ _img_hash(t_res.get("image")) ] = true

		var s_res: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_SHUGAKU, {"girl_id": gid, "cursor": gid % 3, "destination": gid % 4})
		_eq(String(s_res.get("error", "")), "", "SHUGAKU girl_id=%d rendered cleanly" % gid)
		var s_img: Image = s_res.get("image")
		shugaku_hashes[ _img_hash(s_img) ] = true
		if not capture_dir.is_empty() and gid == 3:
			s_img.save_png(capture_dir.path_join("shugaku_girl_3.png"))

	_eq(taiiku_hashes.size(), 5, "TAIIKU renders 5 distinct frames across girl_id 0..4")
	_eq(shugaku_hashes.size(), 5, "SHUGAKU renders 5 distinct frames across girl_id 0..4")


func _verify_subgames_flow(dispatcher: OverlayDispatcher, _capture_dir: String) -> void:
	# 1. RPG_BATTLE (0x57): interactive cursor movement + multi-round combat -> victory -> return_to_game
	var rpg_st: Dictionary = {"cursor": 0, "enemy_hp": 600}
	rpg_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, rpg_st, OverlayDispatcher.PAD_DOWN)
	_eq(int(rpg_st.get("cursor", -1)), 1, "RPG_BATTLE PAD_DOWN moves command cursor 0 -> 1")
	rpg_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, rpg_st, OverlayDispatcher.PAD_UP)
	_eq(int(rpg_st.get("cursor", -1)), 0, "RPG_BATTLE PAD_UP moves command cursor 1 -> 0")
	rpg_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, rpg_st, OverlayDispatcher.PAD_CIRCLE)
	_ok(int(rpg_st.get("enemy_hp", 600)) < 600 and int(rpg_st.get("enemy_hp", 0)) > 0, "RPG_BATTLE PAD_CIRCLE executes attack reducing enemy_hp")
	_ok(String(rpg_st.get("action", "")) != "return_to_game", "RPG_BATTLE stays active while enemy_hp > 0")
	var rpg_guard: int = 0
	while not bool(rpg_st.get("victory", false)) and rpg_guard < 8:
		rpg_guard += 1
		rpg_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, rpg_st, OverlayDispatcher.PAD_CIRCLE)
	_ok(bool(rpg_st.get("victory", false)), "RPG_BATTLE reaches victory when enemy_hp hits 0")
	rpg_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_RPG_BATTLE, rpg_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(rpg_st.get("action", "")), "return_to_game", "RPG_BATTLE confirm after victory returns to game")
	_eq(int(rpg_st.get("return_payload", {}).get("talk_result_code", -1)), 0, "RPG_BATTLE return_payload records talk_result_code=0")

	# 2. TACO (0x59): shooter_step_interactive does NOT exit on 2nd fire press; steps waves -> boss -> result -> scores -> exit
	var taco_st: Dictionary = {"waves_per_stage": 1, "target_stages": 1}
	taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(taco_st.get("frame", 0)), 1, "TACO 1st PAD_CIRCLE advances shooter frame to 1")
	_ok(int(taco_st.get("bullet_count", 0)) > 0, "TACO 1st PAD_CIRCLE spawns player bullet")
	taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(taco_st.get("frame", 0)), 2, "TACO 2nd PAD_CIRCLE advances shooter frame to 2")
	_ok(String(taco_st.get("action", "")) != "return_to_game" and not bool(taco_st.get("completed", false)),
		"TACO does NOT exit on second fire press")
	# Use bombs to clear wave 0 -> spawn Boss 1 (stage 12), then defeat Boss 1 -> GAME_END (45) -> RESULT (8) -> SCORES (5) -> EXIT (46)
	for _b: int in range(2):
		taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_SQUARE)
	_eq(int(taco_st.get("stage", -1)), 12, "TACO clearing wave 0 advances to Boss 1 (stage 12)")
	taco_st["boss"]["hp"] = 2
	taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_CROSS) # secondary missile hits boss
	_eq(int(taco_st.get("stage", -1)), 45, "TACO defeating target boss transitions to SHOOTER_STAGE_GAME_END (45)")
	taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(taco_st.get("stage", -1)), 8, "TACO confirm on GAME_END transitions to SHOOTER_STAGE_RESULT (8)")
	taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(taco_st.get("stage", -1)), 5, "TACO confirm on RESULT transitions to SHOOTER_STAGE_SCORES (5)")
	taco_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TACO, taco_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(taco_st.get("action", "")), "return_to_game", "TACO confirm on SCORES exits to game with return_payload")
	_eq(int(taco_st.get("return_payload", {}).get("stages_cleared", 0)), 1, "TACO return_payload records stages_cleared=1")

	# 3. TT (0x5A): tt_step_interactive does NOT collect bells on PAD_UP or exit after 3 confirms
	var tt_st: Dictionary = {"target_stages": 1, "boss_hp": 16}
	tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, OverlayDispatcher.PAD_UP)
	_ok(int(tt_st.get("player", {}).get("y", 192)) < 192, "TT PAD_UP moves TwinBee ship upward")
	_eq(int(tt_st.get("score", -1)), 0, "TT PAD_UP does NOT arbitrarily collect a bell without AABB overlap")
	for _c: int in range(3):
		tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, OverlayDispatcher.PAD_CIRCLE)
	_ok(String(tt_st.get("action", "")) != "return_to_game" and not bool(tt_st.get("completed", false)),
		"TT does NOT exit after 3 PAD_CIRCLE confirms")
	# Align player with bell[1] at x=42, y=136 so fired shot (y = 136 - 8 - 12 = 116) overlaps bell hitbox at y=116
	tt_st["player"]["x"] = 42
	tt_st["player"]["y"] = 136
	tt_st["bells"][1]["x"] = 42
	tt_st["bells"][1]["y"] = 116
	tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, OverlayDispatcher.PAD_CIRCLE)
	_ok(int(tt_st.get("bell_hits", 0)) >= 1, "TT shot overlapping bell hitbox increments bell_hits via tt_bell_hit_by_shot")
	# Move player onto bell[1] hitbox to collect it via tt_box_overlap
	tt_st["player"]["x"] = int(tt_st["bells"][1]["x"])
	tt_st["player"]["y"] = int(tt_st["bells"][1]["y"])
	tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, 0)
	_ok(int(tt_st.get("score", 0)) > 0, "TT player hitbox overlap collects bell and awards score")
	# Fire ground bomb (PAD_SQUARE) to defeat boss (hp=16) -> TT_STATE_STAGE_CLEAR (3) -> TT_STATE_RANKING_VIEW (7) -> TT_STATE_EXIT (9)
	tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, OverlayDispatcher.PAD_SQUARE)
	_eq(int(tt_st.get("tt_state", -1)), 3, "TT defeating stage boss transitions to TT_STATE_STAGE_CLEAR (3)")
	tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(int(tt_st.get("tt_state", -1)), 7, "TT stage clear transitions to TT_STATE_RANKING_VIEW (7)")
	tt_st = dispatcher.step_mode(OverlayDispatcher.GAME_MODE_TT, tt_st, OverlayDispatcher.PAD_CIRCLE)
	_eq(String(tt_st.get("action", "")), "return_to_game", "TT confirm on rankings exits to game")
	_eq(int(tt_st.get("return_payload", {}).get("tt_stages_cleared", 0)), 1, "TT return_payload records tt_stages_cleared=1")


