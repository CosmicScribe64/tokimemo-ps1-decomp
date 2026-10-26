extends SceneTree
## T-29018 Lane 6: Test suite for School Events Overlays (TAIIKU, BUNKASAI, BUNKA_SD, SHUGAKU, KANGEI, RENSYU, EN_NICHI)
## Verifies:
## 1. Sports Festival (TAIIKU) rules, mashing mechanics, ranking rewards, and folk dance selection.
## 2. Culture Festival (BUNKASAI/BUNKA_SD) booth preparation checks, visitor picking, companion routing, and stage shows.
## 3. School Trip (SHUGAKU) destination routing, sightseeing companion selection, response choices, corridor encounters, and calendar advance.
## 4. Summer Camp (KANGEI), Practice Match (RENSYU), and Festival Stalls (EN_NICHI) mechanics and scoring.
## 5. Authentic 320x240 PS1 presentation rendering with zero glyph bounding-box violations.

const Overlay = preload("res://overlays/school_events_overlay.gd")
const Presenter = preload("res://presentation/school_events_presenter.gd")

var failures: Array[String] = []

func check(ok: bool, message: String) -> void:
	if not ok:
		failures.append(message)
		print("FAIL: " + message)
	else:
		print("PASS: " + message)

func _initialize() -> void:
	print("==================================================================")
	print("Starting T-29018 School Events Overlays Comprehensive Test Suite")
	print("==================================================================")

	# 1. TAIIKU (Sports Festival) Tests
	_test_sports_festival_rules()
	_test_sports_mashing_and_finish()
	_test_sports_ranking_rewards()
	_test_sports_folk_dance()

	# 2. BUNKASAI / BUNKA_SD (Culture Festival) Tests
	_test_culture_booth_preparation()
	_test_culture_visitor_pick()
	_test_culture_companion_tour()
	_test_culture_stage_shows()

	# 3. SHUGAKU (School Trip) Tests
	_test_school_trip_destinations()
	_test_school_trip_companion_selection()
	_test_school_trip_response_stat_deltas()
	_test_school_trip_corridor_and_completion()

	# 4. KANGEI, RENSYU, EN_NICHI Tests
	_test_summer_camp_day_progression()
	_test_practice_match_evaluation()
	_test_festival_stall_games()

	# 5. Authentic PS1 Presentation Rendering & Capture Generation
	_test_presentation_rendering_and_captures()

	print("==================================================================")
	if failures.is_empty():
		print("ALL T-29018 SCHOOL EVENTS OVERLAY TESTS PASSED SUCCESSFULLY!")
		print("==================================================================")
		quit(0)
	else:
		print("TOTAL FAILURES: %d" % failures.size())
		for f in failures:
			print(" - " + f)
		print("==================================================================")
		quit(1)

# ==============================================================================
# 1. TAIIKU Tests
# ==============================================================================
func _test_sports_festival_rules() -> void:
	print("\n--- Testing Sports Festival State Initialization ---")
	var state: Dictionary = Overlay.sports_init_state(Overlay.SportsCompetition.DASH_100M)
	check(state["competition"] == Overlay.SportsCompetition.DASH_100M, "Initial competition is 100m Dash")
	check(state["phase"] == "entry", "Initial phase is entry")
	check(state["track_position"] == 0, "Initial position is 0")

func _test_sports_mashing_and_finish() -> void:
	print("\n--- Testing Sports Festival Runner Mashing & Finish ---")
	var state: Dictionary = Overlay.sports_init_state(Overlay.SportsCompetition.DASH_100M)

	# 15 frames with mashing
	for f in range(15):
		state = Overlay.sports_step_race(state, true, 80)
	check(state["input_latched"] == true, "Circle press latches input")

	# Frame 16: speed should update to positive based on athletics
	state = Overlay.sports_step_race(state, false, 80)
	check(state["player_speed"] > 0, "Latched mashing yields positive player speed")
	check(state["motion_increment"] > 0, "Motion increment increases positively")

	# Simulate race until finish line crossing
	for f in range(100):
		state = Overlay.sports_step_race(state, true, 95)
		if state["phase"] == "finished":
			break

	check(state["finish_order"].size() > 0, "Finish order recorded at least one runner")
	check(state["finish_order"][0] == 0, "High athletics player finished first (1st place)")

func _test_sports_ranking_rewards() -> void:
	print("\n--- Testing Sports Festival Ranking Rewards ---")
	var dummy_girls: Array = []
	for i in range(13):
		dummy_girls.append({"tokimeki": 10, "affection": 10})

	# 1st Place: all 13 girls +4 tokimeki, selected girl +2 tokimeki / +10 affection
	var res1: Dictionary = Overlay.sports_calculate_ranking_rewards(1, 0, dummy_girls)
	check(res1["tokimeki_all_delta"] == 4, "1st place awards +4 tokimeki to all girls")
	check(res1["selected_girl_tokimeki_delta"] == 2, "1st place awards +2 tokimeki to selected girl")
	check(res1["selected_girl_affection_delta"] == 10, "1st place awards +10 affection to selected girl")
	check(dummy_girls[0]["affection"] == 20, "Selected girl affection updated to 20")
	check(dummy_girls[0]["tokimeki"] == 16, "Selected girl tokimeki updated to 16 (10 + 4 + 2)")
	check(dummy_girls[1]["tokimeki"] == 14, "Other girl tokimeki updated to 14 (10 + 4)")

	# 2nd Place: all 13 girls +2 tokimeki, selected girl +1 tokimeki / +5 affection
	var res2: Dictionary = Overlay.sports_calculate_ranking_rewards(2, 1, dummy_girls)
	check(res2["tokimeki_all_delta"] == 2, "2nd place awards +2 tokimeki to all girls")
	check(res2["selected_girl_affection_delta"] == 5, "2nd place awards +5 affection to selected girl")

	# 4th Place: 0 reward
	var res4: Dictionary = Overlay.sports_calculate_ranking_rewards(4, 0, dummy_girls)
	check(res4["tokimeki_all_delta"] == 0 and res4["selected_girl_affection_delta"] == 0, "4th place awards no bonus")

func _test_sports_folk_dance() -> void:
	print("\n--- Testing Folk Dance Participant Selection ---")
	# Case A: < 3 met girls -> ineligible / skipped
	var res_few: Dictionary = Overlay.sports_select_folk_dance([0, 1])
	check(not res_few["eligible"], "Folk dance skipped when met girls < 3")
	check(res_few["variant"] == 2, "Variant set to 2 (skip)")

	# Case B: >= 3 met girls -> Shiori (0) + 2 distinct girls shuffled
	var res_ok: Dictionary = Overlay.sports_select_folk_dance([0, 1, 2, 3, 4], 77)
	check(res_ok["eligible"], "Folk dance eligible when met girls >= 3")
	check(res_ok["participants"].size() == 3, "Exactly 3 participants chosen")
	check(0 in res_ok["participants"], "Shiori (girl 0) is among participants")
	check(res_ok["event_log_id"] == 0x13, "Folk dance event log ID is 0x13")

# ==============================================================================
# 2. BUNKASAI Tests
# ==============================================================================
func _test_culture_booth_preparation() -> void:
	print("\n--- Testing Culture Festival Booth Preparation ---")
	# Science club with high science stat
	var sci_stats := {"science": 85, "art": 20, "look": 30, "humanities": 40, "trivia": 30}
	var res_sci: Dictionary = Overlay.culture_booth_preparation_check(Overlay.CultureClub.SCIENCE, sci_stats, 30)
	check(res_sci["success"], "Science club booth succeeds with high science stat")
	check(res_sci["reputation_delta"] > 0, "Successful booth increases reputation")

	# Science club with low science stat
	var low_sci_stats := {"science": 25, "art": 20, "look": 30, "humanities": 40, "trivia": 30}
	var res_low: Dictionary = Overlay.culture_booth_preparation_check(Overlay.CultureClub.SCIENCE, low_sci_stats, 0)
	check(not res_low["success"], "Science club booth fails with low science stat")
	check(res_low["stress_delta"] > 10, "Failed booth increases stress")

func _test_culture_visitor_pick() -> void:
	print("\n--- Testing Culture Festival Visitor Selection ---")
	var met := [0, 1, 4, 5]
	var picked: int = Overlay.culture_visitor_pick(met, 10)
	check(picked in met, "Picked visitor is one of the met girls")

func _test_culture_companion_tour() -> void:
	print("\n--- Testing Culture Festival Tour Invitation ---")
	# High affection: accepted
	var res_accept: Dictionary = Overlay.culture_companion_invite(0, 35, 30)
	check(res_accept["accepted"], "Companion accepts when affection >= 25")
	check(res_accept["relationship_delta"]["affection"] == 5, "Tour acceptance grants +5 affection")

	# Low affection: declined
	var res_decline: Dictionary = Overlay.culture_companion_invite(1, 5, 5)
	check(not res_decline["accepted"], "Companion declines when affection < 25")

	# Already declined girl: declined immediately
	var res_prev: Dictionary = Overlay.culture_companion_invite(1, 40, 40, 1)
	check(not res_prev["accepted"], "Already declined girl refuses invitation")

func _test_culture_stage_shows() -> void:
	print("\n--- Testing Culture Stage Shows ---")
	var res_show: Dictionary = Overlay.culture_stage_show_evaluate(Overlay.CultureClub.BRASS_BAND, Overlay.CultureClub.BRASS_BAND, 80, true)
	check(res_show["rating"] == "great_success", "Well-prepared stage show with Circle input earns great success")
	check(res_show["tokimeki_reward"] == 4, "Great success awards +4 tokimeki")

# ==============================================================================
# 3. SHUGAKU Tests
# ==============================================================================
func _test_school_trip_destinations() -> void:
	print("\n--- Testing School Trip Destinations ---")
	var trip_kyoto: Dictionary = Overlay.trip_init(Overlay.TripDestination.KYOTO_NARA)
	check(trip_kyoto["destination_name"] == "京都", "Destination 0 name is Kyoto")

	var trip_okinawa: Dictionary = Overlay.trip_init(Overlay.TripDestination.OKINAWA)
	check(trip_okinawa["destination_name"] == "沖縄", "Destination 1 name is Okinawa")

	var trip_hokkaido: Dictionary = Overlay.trip_init(Overlay.TripDestination.HOKKAIDO)
	check(trip_hokkaido["destination_name"] == "北海道", "Destination 2 name is Hokkaido")

func _test_school_trip_companion_selection() -> void:
	print("\n--- Testing School Trip Companion Selection (Low Status Rank) ---")
	var girl_recs: Array[Dictionary] = [
		{"status_rank": 3, "tokimeki": 20, "affection": 20}, # Girl 0 (rank 3)
		{"status_rank": 1, "tokimeki": 30, "affection": 40}, # Girl 1 (rank 1, sum 70)
		{"status_rank": 1, "tokimeki": 40, "affection": 45}, # Girl 2 (rank 1, sum 85 -> best!)
		{"status_rank": 2, "tokimeki": 50, "affection": 50}, # Girl 3 (rank 2)
	]
	var met: Array[int] = [0, 1, 2, 3]
	var chosen: int = Overlay.trip_select_companion(met, girl_recs)
	check(chosen == 2, "Companion selector chooses rank 1 girl with highest tokimeki+affection tiebreak (Girl 2)")

func _test_school_trip_response_stat_deltas() -> void:
	print("\n--- Testing School Trip Response Stat Deltas ---")
	var d0: Dictionary = Overlay.trip_response_stat_deltas(0)
	check(d0["tokimeki"] == 3 and d0["affection"] == 2 and d0["syoushin"] == -15, "Choice 0 grants +3 tokimeki, +2 affection, -15 syoushin")

	var d1: Dictionary = Overlay.trip_response_stat_deltas(1)
	check(d1["tokimeki"] == 2 and d1["affection"] == 1 and d1["syoushin"] == -10, "Choice 1 grants +2 tokimeki, +1 affection, -10 syoushin")

	var d_other: Dictionary = Overlay.trip_response_stat_deltas(2)
	check(d_other["tokimeki"] == 0 and d_other["affection"] == 1 and d_other["syoushin"] == 10, "Other choice grants +1 affection, +10 syoushin")

func _test_school_trip_corridor_and_completion() -> void:
	print("\n--- Testing School Trip Corridor & Completion ---")
	var corridor: Dictionary = Overlay.trip_hotel_corridor_encounter(0, 5)
	check(corridor["event"] == "talk_in_corridor", "Corridor choice 0 initiates corridor talk")
	check(corridor["affection_delta"] == 3, "Corridor talk grants +3 affection")

	var state: Dictionary = Overlay.trip_init(Overlay.TripDestination.KYOTO_NARA)
	state["current_girl"] = 1
	var dummy_girls: Array = [{"tokimeki": 10, "affection": 10}, {"tokimeki": 10, "affection": 10}]
	state = Overlay.trip_complete_day(state, dummy_girls)
	check(dummy_girls[1]["tokimeki"] == 11 and dummy_girls[1]["affection"] == 11, "Trip completion advances current girl relationship")
	check(state["day"] == 2, "Trip completion advances day counter to 2")

# ==============================================================================
# 4. KANGEI, RENSYU, EN_NICHI Tests
# ==============================================================================
func _test_summer_camp_day_progression() -> void:
	print("\n--- Testing Summer Camp Day Progression ---")
	var camp_day: Dictionary = Overlay.camp_step_day(1, Overlay.CultureClub.BASEBALL, 20, 80)
	check(camp_day["new_practice_points"] == 26, "Summer camp training adds +6 practice points")
	check(camp_day["new_stamina"] == 65, "Summer camp training consumes 15 stamina")
	check(camp_day["cooking_quality"] == true, "Cooking quality succeeds when stamina > 40")

func _test_practice_match_evaluation() -> void:
	print("\n--- Testing Practice Match Evaluation (RENSYU) ---")
	# Match 0: threshold is 30 (0x1E)
	# Case A: practice points = 70 (diff = +40 >= 30) -> Grade 5 (Blowout Victory)
	var res_win: Dictionary = Overlay.practice_match_evaluate(6, 70, 0, 0)
	check(res_win["grade"] == 5, "Practice match points 70 vs 30 gives Grade 5")
	check(res_win["victory"] == true, "Grade 5 is victory")
	check(res_win["tokimeki_all_delta"] == 2, "Victory awards +2 tokimeki to all girls")
	check(res_win["attending_girl_affection_delta"] == 2, "Attending girl gains +2 affection on match")
	check(res_win["new_match_count"] == 1, "Victory increments match win count")

	# Case B: practice points = 10 (diff = -20 < 0) -> Loss (Grade 1)
	var res_loss: Dictionary = Overlay.practice_match_evaluate(6, 10, 0, -1)
	check(res_loss["victory"] == false, "Practice match points 10 vs 30 results in defeat")
	check(res_loss["guts_delta"] == 10 and res_loss["stress_delta"] == 10, "Defeat adds +10 guts and +10 stress")
	check(res_loss["new_match_count"] == 0, "Defeat does not increment win count")

func _test_festival_stall_games() -> void:
	print("\n--- Testing Festival Stall Games (EN_NICHI) ---")
	# Shooting game: 15 frames hold -> bullseye 100
	var res_shoot: Dictionary = Overlay.stall_game_play(Overlay.StallGame.SHOOTING, 15, 0)
	check(res_shoot["score"] == 100, "Bullseye hold gives 100 score")
	check(res_shoot["won_prize"] == true, "100 score wins prize")
	check(res_shoot["affection_delta"] == 5, "Winning stall prize awards +5 affection")

	# Goldfish scooping: poor hold -> failed scooping
	var res_goldfish_fail: Dictionary = Overlay.stall_game_play(Overlay.StallGame.GOLDFISH, 20, 0)
	check(not res_goldfish_fail["won_prize"], "Improper scoop timing fails prize")
	check(res_goldfish_fail["affection_delta"] == 1, "Failed stall game awards baseline +1 affection")

# ==============================================================================
# 5. Presentation Rendering & Capture Generation
# ==============================================================================
func _test_presentation_rendering_and_captures() -> void:
	print("\n--- Testing Authentic 320x240 Presentation Rendering & Captures ---")
	var presenter = Presenter.new()

	# Resolve font root
	var font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
	if not DirAccess.dir_exists_absolute(font_root):
		font_root = "/work/.asset-cache/port/fonts/native-font-v2"
	if not DirAccess.dir_exists_absolute(font_root):
		font_root = ProjectSettings.globalize_path("res://../../.asset-cache/port/fonts/native-font-v2")

	var hud_manifest := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
	if not FileAccess.file_exists(hud_manifest):
		hud_manifest = "/work/.asset-cache/port/hud-production-v1/manifest.json"
	if not FileAccess.file_exists(hud_manifest):
		hud_manifest = ProjectSettings.globalize_path("res://../../.asset-cache/port/hud-production-v1/manifest.json")

	var font_err: String = presenter.init_resources(font_root, hud_manifest)
	check(font_err.is_empty(), "Presenter init_resources loaded native font: " + font_err)

	# Captures output directory
	var captures_dir := "/workspace/.claude/orchestrator/state/T-29018/school-events-overlays-v1/captures"
	if not DirAccess.dir_exists_absolute(captures_dir):
		captures_dir = ProjectSettings.globalize_path("res://../../.claude/orchestrator/state/T-29018/school-events-overlays-v1/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)

	# 1. Render Sports Festival HUD (体育祭)
	var img_sports: Image = presenter.render_sports_festival_hud({
		"competition": Overlay.SportsCompetition.DASH_100M,
		"phase": "running",
		"player_speed": 0x1400,
		"stamina": 85,
		"player_place": 1,
		"track_position": 0x180000
	})
	check(img_sports != null and img_sports.get_size() == Vector2i(320, 240), "Rendered Sports Festival HUD is 320x240")
	var path_sports := captures_dir.path_join("sports_festival_hud.png")
	var err_s := img_sports.save_png(path_sports)
	check(err_s == OK, "Saved captures/sports_festival_hud.png")

	# 2. Render Culture Festival Booth (文化祭)
	var img_culture: Image = presenter.render_culture_festival_booth({
		"selected_booth": 1,
		"companion_girl": 0
	})
	check(img_culture != null and img_culture.get_size() == Vector2i(320, 240), "Rendered Culture Festival Booth is 320x240")
	var path_culture := captures_dir.path_join("culture_festival_booth.png")
	var err_c := img_culture.save_png(path_culture)
	check(err_c == OK, "Saved captures/culture_festival_booth.png")

	# 3. Render School Trip Companion Prompt (修学旅行)
	var img_trip: Image = presenter.render_school_trip_companion({
		"destination": Overlay.TripDestination.KYOTO_NARA,
		"companion_girl": 4
	})
	check(img_trip != null and img_trip.get_size() == Vector2i(320, 240), "Rendered School Trip Companion Prompt is 320x240")
	var path_trip := captures_dir.path_join("school_trip_companion.png")
	var err_t := img_trip.save_png(path_trip)
	check(err_t == OK, "Saved captures/school_trip_companion.png")

	# 4. Render Club Practice Match / Summer Camp (練習試合 / 夏合宿)
	var img_match: Image = presenter.render_club_practice_match_camp({
		"club_id": 6, # Baseball club
		"practice_points": 95,
		"threshold": 60,
		"victory": true
	})
	check(img_match != null and img_match.get_size() == Vector2i(320, 240), "Rendered Club Practice Match / Camp is 320x240")
	var path_match := captures_dir.path_join("practice_match_camp.png")
	var err_m := img_match.save_png(path_match)
	check(err_m == OK, "Saved captures/practice_match_camp.png")

	# Zero glyph bounding-box violation check!
	check(presenter.bounding_box_violations.is_empty(), "Zero glyph bounding box violations encountered across all 4 scenes")
	if not presenter.bounding_box_violations.is_empty():
		for v in presenter.bounding_box_violations:
			print(" - VIOLATION: " + v)

	check(presenter.recorded_glyphs.size() > 50, "Extensive glyph recording coverage (> 50 glyphs rendered)")
	print("Total glyphs rendered and validated: %d" % presenter.recorded_glyphs.size())
