extends SceneTree
## T-29018: Headless verification and 320x240 PNG capture suite for
## Lane 3 (GEKO, MODE_EXAM/GYOZI, OMIMAI/VALEN/SHOUGATU) and Lane 4 (ENDING & MASTER).

const EncountersOverlay = preload("res://overlays/encounters_and_events_overlay.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const Presenter = preload("res://presentation/encounters_and_ending_presenter.gd")

const MANIFEST_PATH: String = "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const FONT_ROOT: String = "/workspace/.asset-cache/port/fonts/native-font-v2"
const CAPTURE_DIR: String = "/workspace/.claude/orchestrator/state/T-29018/encounters-and-ending-v1/captures"

var _failures: int = 0
var _assertions: int = 0


func _assert_true(condition: bool, label: String) -> void:
	_assertions += 1
	if not condition:
		_failures += 1
		printerr("FAIL: ", label)


func _assert_eq(actual: Variant, expected: Variant, label: String) -> void:
	_assertions += 1
	if actual != expected:
		_failures += 1
		printerr("FAIL: %s (expected=%s, actual=%s)" % [label, str(expected), str(actual)])


func _make_girls(count: int = 14) -> Array:
	var arr: Array = []
	for i: int in range(count):
		var counters: Array[int] = []
		counters.resize(36)
		for c: int in range(36):
			counters[c] = 0
		arr.append({
			"id": i,
			"met": 0,
			"affection": 20,
			"tokimeki": 20,
			"syoushin": 0,
			"flags": 0,
			"scene_counters": counters,
			"story_stage": 0,
			"rei_subcount": 0,
		})
	return arr


func _test_geko_walk_home_rules() -> void:
	var girls: Array = _make_girls()
	girls[0]["met"] = 1
	girls[0]["syoushin"] = 10
	girls[0]["tokimeki"] = 60
	girls[0]["affection"] = 50
	girls[1]["met"] = 1
	girls[1]["syoushin"] = 15
	girls[1]["tokimeki"] = 45
	girls[1]["affection"] = 40

	var closeness: Array[int] = [25, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var states: Array[int] = [2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3]

	# 1. geko_judge normal trigger (50% roll < 50, closeness >= 17, syoushin < 60, not last_picked_girl)
	var judge_hit: Dictionary = EncountersOverlay.evaluate_geko_judge(
		girls, 11, -1, closeness, states, [35]
	)
	_assert_true(bool(judge_hit["triggered"]), "geko_judge triggers when met, closeness>=17, syoushin<60, roll<50")
	_assert_eq(int(judge_hit["girl_id"]), 0, "geko_judge picks girl 0 at seed%11==0")

	# Respects last_picked_girl exclusion and advances to next eligible girl
	var judge_skip_last: Dictionary = EncountersOverlay.evaluate_geko_judge(
		girls, 11, 0, closeness, states, [35, 20]
	)
	_assert_true(bool(judge_skip_last["triggered"]), "geko_judge skips last_picked_girl=0 and picks girl 1")
	_assert_eq(int(judge_skip_last["girl_id"]), 1, "geko_judge selects girl 1")

	# 2. Forced high-syoushin confrontation (>= 76 / 0x4C)
	girls[1]["syoushin"] = 78
	var forced: Dictionary = EncountersOverlay.check_forced_high_syoushin_encounter(
		girls, 0, 2, 10, 18, 0
	)
	_assert_true(bool(forced["triggered"]), "Forced walk-home confrontation triggers at syoushin >= 76")
	_assert_eq(int(forced["girl_id"]), 1, "Forced confrontation selects girl 1")
	_assert_eq(int(forced["high_girl_counter"]), 1, "high_girl_counter set to 1")

	# Topic selection: bomb confrontation vs nickname prompt vs seasonal
	var topic_bomb: Dictionary = EncountersOverlay.select_walk_home_topic(1, 2, 1, 10, 18, 0, 5, 0)
	_assert_eq(int(topic_bomb["walk_home_group"]), 1, "High syoushin selects group 1 (confrontation)")
	_assert_true(bool(topic_bomb["is_bomb_confrontation"]), "is_bomb_confrontation is true")

	var topic_nick: Dictionary = EncountersOverlay.select_walk_home_topic(0, 2, 0, 10, 18, 0, 0, 0)
	_assert_eq(int(topic_nick["walk_home_group"]), 4, "1-in-10 roll at state>=2 selects group 4 (nickname prompt)")
	_assert_true(bool(topic_nick["is_nickname_prompt"]), "is_nickname_prompt is true")

	# 3. Accept walk home (talk_choice == 0): increments other met girls' syoushin
	girls[1]["syoushin"] = 10
	var accept_res: Dictionary = EncountersOverlay.apply_walk_home_choice(
		0, 0, 2, 6, girls, 100, [2]
	)
	_assert_eq(int(accept_res["followup_allowed"]), 1, "Accept walk home allows followup at state 2")
	_assert_eq(int(accept_res["girls"][1]["syoushin"]), 10 + 2 + 4, "Other met girl gets syoushin += (rand % (tokimeki/20 + 1)) + 4")

	# 4. Decline walk home (talk_choice == 1): increments current girl's syoushin and group += 2
	var decline_res: Dictionary = EncountersOverlay.apply_walk_home_choice(
		1, 0, 2, 6, girls, 101, []
	)
	_assert_eq(int(decline_res["followup_allowed"]), 0, "Decline walk home disables followup")
	_assert_eq(int(decline_res["walk_home_group"]), 8, "Decline walk home adds 2 to walk_home_group")
	# tokimeki=60 -> band=3 -> 101 % 4 = 1 -> + 1 + 6 = 7
	_assert_eq(int(decline_res["girls"][0]["syoushin"]), 10 + 1 + 6, "Declined girl at state>=2 gets syoushin += offset + 6")

	# 5. Finish walk home adds +2 affection and marks bit 16 on forced confrontation
	var after_finish: Array = EncountersOverlay.finish_walk_home(1, girls, 1, true)
	_assert_eq(int(after_finish[1]["affection"]), 42, "finish_walk_home adds +2 affection")
	_assert_true((int(after_finish[1]["flags"]) & 0x10000) != 0, "finish_walk_home sets bit 16 after high-syoushin confrontation")


func _test_term_exam_and_seasonal_rules() -> void:
	# 1. Correction bonus
	var corr_y1_jul: int = EncountersOverlay.compute_exam_correction_bonus(0x5F, 7, 7)
	_assert_eq(corr_y1_jul, 14, "Year 1 July correction bonus is 14")

	# 2. Subject scores across all 5 weekdays
	var high_params := {
		"liberal_arts": 130,
		"science": 135,
		"trivia": 125,
		"art": 140,
	}
	var mon_res: Dictionary = EncountersOverlay.compute_exam_day_score(1, high_params, 8, 0, 14, 0, 0, 1)
	var tue_res: Dictionary = EncountersOverlay.compute_exam_day_score(2, high_params, 8, 0, 14, 0, 0, 2)
	var wed_res: Dictionary = EncountersOverlay.compute_exam_day_score(3, high_params, 8, 0, 14, 0, 0, 0)
	var thu_res: Dictionary = EncountersOverlay.compute_exam_day_score(4, high_params, 8, 0, 14, 0, 0, 3)
	var fri_res: Dictionary = EncountersOverlay.compute_exam_day_score(5, high_params, 8, 0, 14, 0, 0, 1)
	_assert_eq(int(mon_res["grade"]), 4, "High liberal_arts achieves grade 4 on Monday Language exam")
	_assert_eq(int(tue_res["grade"]), 4, "High science achieves grade 4 on Tuesday Math exam")
	_assert_eq(int(wed_res["grade"]), 4, "High science+trivia achieves grade 4 on Wednesday Science exam")
	_assert_eq(int(thu_res["grade"]), 4, "High trivia+liberal_arts achieves grade 4 on Thursday Social Studies exam")
	_assert_eq(int(fri_res["grade"]), 4, "High art+trivia achieves grade 4 on Friday Art exam")

	# Sick/neurosis penalty halves ret; low parameters trigger red mark (< 40 -> grade 0)
	var low_params := {"liberal_arts": 25, "science": 20, "trivia": 20, "art": 20}
	var fail_res: Dictionary = EncountersOverlay.compute_exam_day_score(1, low_params, 0, 2, 0, 0, 0, 0)
	_assert_eq(int(fail_res["grade"]), 0, "Low parameter + sick flag produces grade 0 (red mark)")
	_assert_true(bool(fail_res["failed"]), "Failed flag is true for score < 40")

	# 3. Exam ranking & relationship bonuses
	var girls: Array = _make_girls()
	girls[0]["met"] = 1
	girls[0]["tokimeki"] = 50
	girls[1]["met"] = 1
	girls[1]["tokimeki"] = 40
	var scene_flags: Array[int] = []
	scene_flags.resize(0xE0)
	for i: int in range(0xE0):
		scene_flags[i] = 0

	var rank_res: Dictionary = EncountersOverlay.build_exam_ranking(485, girls, scene_flags, 0)
	_assert_eq(int(rank_res["player_rank"]), 1, "Total score 485 achieves school Rank #1")
	_assert_eq(int(rank_res["ranking_order"][0]), 13, "Player (13) is first in ranking_order")
	# Girl 0 beaten for first time (+11) + Rank #1 bonus (+2) = +13 -> 63
	_assert_eq(int(rank_res["girls"][0]["tokimeki"]), 63, "First-time beating girl + Rank #1 awards +13 tokimeki")
	_assert_eq(int(rank_res["scene_flags"][0xC8]), 1, "scene_flags[0xC8] marked after first exam victory over Shiori")

	# Remedial week triggered at >= 3 failed subjects
	var remedial_res: Dictionary = EncountersOverlay.build_exam_ranking(150, girls, scene_flags, 3)
	_assert_true(bool(remedial_res["remedial_required"]), ">=3 red marks triggers remedial week")
	_assert_eq(int(remedial_res["next_day_marker"]), 0x17, "Remedial week sets next-day marker to 0x17")

	# 4. Valentine candidate selection
	var v_girls: Array = _make_girls()
	v_girls[0]["met"] = 1
	v_girls[0]["affection"] = 85
	v_girls[0]["tokimeki"] = 90
	v_girls[1]["met"] = 1
	v_girls[1]["affection"] = 70
	v_girls[1]["tokimeki"] = 75
	v_girls[9]["met"] = 0
	v_girls[9]["tokimeki"] = 25 # Unmet Megumi >= 20
	var close_toki: Array[int] = [80, 72, 0, 0, 0, 0, 0, 0, 0, 30, 0]
	var close_yuukou: Array[int] = [75, 65, 0, 0, 0, 0, 0, 0, 0, 50, 0]
	var valen_res: Dictionary = EncountersOverlay.select_valentine_candidates(v_girls, close_toki, close_yuukou)
	_assert_eq(int(valen_res["unmet_megumi_selected"]), 1, "Unmet Megumi with tokimeki>=20 is selected")
	_assert_eq(int(valen_res["ranked_girls"][0]), 0, "Shiori is #1 in Valentine top-3 ranked girls")
	_assert_eq(int(valen_res["ranked_girls"][1]), 1, "Mio is #2 in Valentine top-3 ranked girls")
	_assert_eq(int(valen_res["candidates"][0]), 9, "Unmet Megumi is placed in slot 0 of Valentine candidates")
	_assert_eq(int(valen_res["candidates"][valen_res["candidates"].size() - 1]), 0, "High-tokimeki Shiori is placed in last slot of Valentine candidates")


func _test_ending_and_club_master_rules() -> void:
	# 1. Shiori confession eligibility (requires closeness>=85, healthy, 7 params>=[130,130,130,120,100,100,130], dates>=8, syoushin<51)
	var girls: Array = _make_girls()
	girls[0]["met"] = 1
	girls[0]["affection"] = 120
	girls[0]["tokimeki"] = 120
	girls[0]["syoushin"] = 0
	girls[0]["scene_counters"][1] = 8 # 8 dates

	# [condition, liberal_arts, science, art, fitness, social, style, trivia]
	var shiori_params: Array[int] = [100, 140, 140, 140, 130, 120, 120, 140]
	var scores: Dictionary = EndingOverlay.compute_girl_scores(girls, shiori_params, 10, 0)
	_assert_true(int(scores["closeness_by_girl"][0]) >= 85, "Shiori closeness >= 85 when stats and affection/tokimeki are maxed")
	_assert_eq(int(scores["points_by_girl"][0]), 0x311, "Shiori receives 0x311 (785) confession points when all requirements met")

	# Failing a single parameter minimum drops Shiori points to 0
	var low_art_params: Array[int] = [100, 140, 140, 125, 130, 120, 120, 140]
	var scores_low_art: Dictionary = EndingOverlay.compute_girl_scores(girls, low_art_params, 10, 0)
	_assert_eq(int(scores_low_art["points_by_girl"][0]), 0, "Shiori confession points drop to 0 when art < 130")

	# Bomb / high syoushin (>= 51) drops Shiori points to 0
	girls[0]["syoushin"] = 51
	var scores_bomb: Dictionary = EndingOverlay.compute_girl_scores(girls, shiori_params, 10, 0)
	_assert_eq(int(scores_bomb["points_by_girl"][0]), 0, "Shiori confession points drop to 0 when syoushin >= 51")
	girls[0]["syoushin"] = 0

	# 2. Priority tie-breaking (strict `>` so earlier girl index wins ties)
	girls[1]["met"] = 1
	girls[2]["met"] = 1
	var tie_points: Array[int] = [0, 620, 620, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var empty_flags: Array[int] = []
	empty_flags.resize(0x60)
	var tie_sel: Dictionary = EndingOverlay.select_graduation_candidate(girls, tie_points, empty_flags, 100)
	_assert_eq(int(tie_sel["current_girl"]), 1, "Priority tie-breaking selects earlier index (Mio=1 over Yuina=2 on equal points)")

	# 3. Rei special priority branch (points[0] < 500, rei_subcount >= 70, scene_flags[0x4F] == 2)
	girls[12]["rei_subcount"] = 75
	empty_flags[0x4F] = 2
	var rei_sel: Dictionary = EndingOverlay.select_graduation_candidate(girls, tie_points, empty_flags, 100)
	_assert_eq(int(rei_sel["current_girl"]), 12, "Rei branch overrides non-Shiori candidates when points[0]<500, rei_subcount>=70, flag 0x4F==2")
	empty_flags[0x4F] = 0

	# 4. Miharu secret confession branch when no heroine >= 500
	var low_points: Array[int] = [0, 320, 280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	girls[11]["story_stage"] = 5
	var miharu_sel: Dictionary = EndingOverlay.select_graduation_candidate(girls, low_points, empty_flags, 20)
	_assert_eq(int(miharu_sel["current_girl"]), 11, "Miharu secret confession triggers when story_stage>0, flag 0x4D==0, and roll < threshold")

	# 5. Bad ending (Yoshio = 13) when no candidate qualifies
	girls[11]["story_stage"] = 0
	var yoshio_sel: Dictionary = EndingOverlay.select_graduation_candidate(girls, low_points, empty_flags, 200)
	_assert_eq(int(yoshio_sel["current_girl"]), 13, "Falls back to Yoshio (13) when no heroine or secret candidate qualifies")

	# 6. Legendary Tree stage dispatch (stages 0..7 -> GAME_MODE_ETC_C2 = 0xC2)
	for st: int in range(8):
		var step_info: Dictionary = EndingOverlay.step_ending_stage_flow(st, 0)
		if st < 7:
			_assert_eq(int(step_info["next_game_mode"]), 0xC1, "Stage %d stays in GAME_MODE_ENDING (0xC1)" % st)
		else:
			_assert_eq(int(step_info["next_game_mode"]), 0xC2, "Stage 7 transitions to GAME_MODE_ETC_C2 (0xC2)")

	# 7. Club Master (MASTER) tournament grade & crown flag (b30)
	var master_win: Dictionary = EndingOverlay.compute_master_tournament_grade(385, 0)
	_assert_eq(int(master_win["grade"]), 5, "practice_points=385 (margin +35) achieves grade 5")
	_assert_true(bool(master_win["club_master_crowned"]), "grade >= 3 crowns player Club Master")
	_assert_true((int(master_win["player_flags"]) & (1 << 30)) != 0, "b30 club master flag is set")
	_assert_eq(int(master_win["tokimeki_bonus"]), 25, "Victory awards +25 tokimeki")


func _test_visual_captures_and_bounding_boxes() -> void:
	var presenter := Presenter.new()
	var err: String = presenter.load_manifest(MANIFEST_PATH, FONT_ROOT)
	_assert_eq(err, "", "Presenter loads HUD manifest and native font pack cleanly")
	if not err.is_empty():
		return

	DirAccess.make_dir_recursive_absolute(CAPTURE_DIR)

	var captures: Array[Dictionary] = [
		{
			"name": "walk_home_invitation_320x240.png",
			"render": func() -> Image: return presenter.render_walk_home_invitation_frame(0),
		},
		{
			"name": "term_exam_results_320x240.png",
			"render": func() -> Image: return presenter.render_term_exam_results_frame(),
		},
		{
			"name": "valentine_chocolate_320x240.png",
			"render": func() -> Image: return presenter.render_valentine_chocolate_frame(),
		},
		{
			"name": "legendary_tree_confession_320x240.png",
			"render": func() -> Image: return presenter.render_legendary_tree_confession_frame(),
		},
	]

	for item: Dictionary in captures:
		var filename: String = item["name"]
		var fn: Callable = item["render"]
		var img: Image = fn.call()
		_assert_eq(img.get_width(), 320, "%s width is 320" % filename)
		_assert_eq(img.get_height(), 240, "%s height is 240" % filename)
		var violations: Array[String] = presenter.check_glyph_containment_violations()
		_assert_true(violations.is_empty(), "%s has zero glyph bounding-box violations (got %s)" % [filename, str(violations)])
		var out_path: String = CAPTURE_DIR.path_join(filename)
		var save_err: Error = img.save_png(out_path)
		_assert_eq(save_err, OK, "Saved %s to %s" % [filename, out_path])


func _init() -> void:
	_test_geko_walk_home_rules()
	_test_term_exam_and_seasonal_rules()
	_test_ending_and_club_master_rules()
	_test_visual_captures_and_bounding_boxes()

	if _failures == 0:
		print("PASS: T-29018 encounters_and_ending suite (%d assertions, 0 failures)" % _assertions)
		quit(0)
	else:
		printerr("FAILED: T-29018 encounters_and_ending suite (%d failures / %d assertions)" % [_failures, _assertions])
		quit(1)
