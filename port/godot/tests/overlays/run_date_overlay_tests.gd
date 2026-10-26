extends SceneTree
## Headless verification & 320x240 PNG capture runner for T-29018 (DATE & DATE2 Overlays).

const DateOverlayRules = preload("res://overlays/date_overlay.gd")
const DateOverlayPresenterScript = preload("res://presentation/date_overlay_presenter.gd")

var _checks: Array[Dictionary] = []
var _failed := 0


func _init() -> void:
	var args := OS.get_cmdline_user_args()
	var hud_manifest := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
	var font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
	var date_pack_root := "/workspace/.asset-cache/port/date-preview-v1"
	var capture_dir := "/workspace/.claude/orchestrator/state/T-29018/date-overlays-v1/captures"

	var i := 0
	while i < args.size():
		match args[i]:
			"--hud-manifest":
				if i + 1 < args.size():
					hud_manifest = args[i + 1]
					i += 1
			"--font-root":
				if i + 1 < args.size():
					font_root = args[i + 1]
					i += 1
			"--date-pack":
				if i + 1 < args.size():
					date_pack_root = args[i + 1]
					i += 1
			"--capture-dir":
				if i + 1 < args.size():
					capture_dir = args[i + 1]
					i += 1
		i += 1

	_run_rules_tests()
	_run_presenter_and_capture_tests(hud_manifest, font_root, date_pack_root, capture_dir)

	var total := _checks.size()
	var passed := total - _failed
	print("DATE_OVERLAY_TESTS summary: %d/%d passed (%d failed)" % [passed, total, _failed])
	quit(1 if _failed > 0 else 0)


func _assert_eq(label: String, actual: Variant, expected: Variant) -> void:
	var ok: bool = (actual == expected)
	_checks.append({"label": label, "ok": ok, "actual": actual, "expected": expected})
	if not ok:
		_failed += 1
		push_error("FAIL [%s]: expected %s, got %s" % [label, str(expected), str(actual)])


func _assert_true(label: String, cond: bool) -> void:
	_assert_eq(label, cond, true)


func _run_rules_tests() -> void:
	# 1. Arrival timing & Yuina exception (date_init)
	var st := DateOverlayRules.create_default_state({"frame_count": 4, "date_girl": DateOverlayRules.GIRL_SHIORI})
	DateOverlayRules.date_init(st)
	_assert_eq("date_init_wait_greeting_on_mult4", st["date_arrival"], DateOverlayRules.DATE_ARRIVAL_WAIT_GREETING)
	_assert_eq("date_init_day_event_bit_8", st["day_event_done_bits"] & 8, 8)

	var st_yuina := DateOverlayRules.create_default_state({"frame_count": 4, "date_girl": DateOverlayRules.GIRL_YUINA})
	DateOverlayRules.date_init(st_yuina)
	_assert_eq("date_init_yuina_never_waits", st_yuina["date_arrival"], DateOverlayRules.DATE_ARRIVAL_DIRECT_GREETING)

	var st_direct := DateOverlayRules.create_default_state({"frame_count": 5, "date_girl": DateOverlayRules.GIRL_SHIORI})
	DateOverlayRules.date_init(st_direct)
	_assert_eq("date_init_direct_greeting_non_mult4", st_direct["date_arrival"], DateOverlayRules.DATE_ARRIVAL_DIRECT_GREETING)

	# 2. Meeting place check & wrong-place resolution (date_meeting_place_check / resolve)
	var st_wrong := DateOverlayRules.create_default_state({
		"appointment_place_0": DateOverlayRules.SPOT_NEIGHBORHOOD_PARK,
		"appointment_place_1": DateOverlayRules.SPOT_AQUARIUM,
		"appointment_place_2": DateOverlayRules.SPOT_AQUARIUM,
		"appointment_place_3": DateOverlayRules.SPOT_CENTRAL_PARK,
	})
	DateOverlayRules.date_init(st_wrong)
	var mcheck := DateOverlayRules.date_meeting_place_check(st_wrong)
	_assert_eq("wrong_place_3choice_status", mcheck["status"], "wrong_place_prompt")
	_assert_eq("wrong_place_3choice_group", mcheck["meeting_group"], DateOverlayRules.DATE_MEETING_WRONG_THREE_CHOICE)
	var mres := DateOverlayRules.date_meeting_place_resolve(st_wrong, 1)
	_assert_true("wrong_place_move_to_correct_found", bool(mres["found_girl"]))
	_assert_eq("wrong_place_corrected_arrival", st_wrong["date_arrival"], DateOverlayRules.DATE_ARRIVAL_CORRECTED_PLACE)
	_assert_eq("wrong_place_syoushin_plus_8", st_wrong["girls"][DateOverlayRules.GIRL_SHIORI]["syoushin"], 18)

	var st_missed := DateOverlayRules.create_default_state({
		"appointment_place_0": DateOverlayRules.SPOT_NEIGHBORHOOD_PARK,
		"appointment_place_1": DateOverlayRules.SPOT_AQUARIUM,
		"appointment_place_2": DateOverlayRules.SPOT_ZOO,
		"appointment_place_3": 0xFF,
	})
	DateOverlayRules.date_init(st_missed)
	var mres_miss := DateOverlayRules.date_meeting_place_resolve(st_missed, 0)
	_assert_eq("wrong_place_stay_not_here", mres_miss["found_girl"], false)
	_assert_eq("wrong_place_not_here_group", mres_miss["meeting_group"], DateOverlayRules.DATE_MEETING_NOT_HERE)

	# 3. Looks check (date_looks_check: < 30 general, < 35 Mira)
	var st_looks := DateOverlayRules.create_default_state({"param_looks": 29, "date_girl": DateOverlayRules.GIRL_SHIORI})
	DateOverlayRules.date_init(st_looks)
	var lres := DateOverlayRules.date_arrival_and_looks_check(st_looks)
	_assert_true("looks_below_30_rejected", bool(lres["looks_rejected"]))
	_assert_eq("looks_below_30_group_0x0C", lres["date_arrival_group"], 0x0C)
	_assert_eq("looks_below_30_tokimeki_minus_1", st_looks["girls"][DateOverlayRules.GIRL_SHIORI]["tokimeki"], 59)
	_assert_eq("looks_below_30_affection_minus_1", st_looks["girls"][DateOverlayRules.GIRL_SHIORI]["affection"], 59)
	_assert_eq("looks_below_30_syoushin_plus_5", st_looks["girls"][DateOverlayRules.GIRL_SHIORI]["syoushin"], 15)

	var st_mira := DateOverlayRules.create_default_state({"param_looks": 34, "date_girl": DateOverlayRules.GIRL_MIRA})
	DateOverlayRules.date_init(st_mira)
	var lres_mira := DateOverlayRules.date_arrival_and_looks_check(st_mira)
	_assert_true("mira_looks_below_35_rejected", bool(lres_mira["looks_rejected"]))

	# 4. Birthday checks & gift choice rewards (girl_birthday.c / player_birthday.c)
	var st_gbday := DateOverlayRules.create_default_state({
		"month": 10,
		"day": 23, # Shiori birthday is Oct 24
		"date_girl": DateOverlayRules.GIRL_SHIORI,
		"appointment_place_0": DateOverlayRules.SPOT_AQUARIUM,
	})
	DateOverlayRules.date_init(st_gbday)
	var bchk := DateOverlayRules.date_birthday_check(st_gbday)
	_assert_eq("girl_birthday_triggered_date", bchk["triggered"], "girl_birthday")
	_assert_eq("girl_birthday_stage_2", bchk["mode_stage"], DateOverlayRules.DATE_STAGE_GIRL_BIRTHDAY)
	var gift0 := DateOverlayRules.apply_girl_birthday_gift_choice(st_gbday, 0)
	_assert_eq("girl_bday_gift_choice0_tokimeki", gift0["tokimeki"], 63)
	_assert_eq("girl_bday_gift_choice0_affection", gift0["affection"], 62)
	_assert_eq("girl_bday_gift_choice0_syoushin", gift0["syoushin"], 0) # 10 - 15 clamped to 0

	var st_pbday := DateOverlayRules.create_default_state({
		"month": 5,
		"day": 12,
		"player_bday_month": 5,
		"player_bday_day": 10,
		"appointment_place_0": DateOverlayRules.SPOT_AMUSEMENT_PARK,
	})
	DateOverlayRules.date_init(st_pbday)
	var pbchk := DateOverlayRules.date_birthday_check(st_pbday)
	_assert_eq("player_birthday_triggered_date2", pbchk["triggered"], "player_birthday")
	_assert_eq("player_birthday_date2_mode", pbchk["game_mode"], DateOverlayRules.GAME_MODE_AMUSEMENT_PARK_2)
	_assert_eq("player_birthday_date2_stage_1", pbchk["mode_stage"], DateOverlayRules.DATE2_STAGE_PLAYER_BIRTHDAY)
	DateOverlayRules.apply_player_birthday_gift(st_pbday)
	_assert_eq("player_birthday_year_recorded", st_pbday["birthday_gift_year"], 1995)

	# 5. junban_init permutation (src/main/scene_library.c:2181-2193)
	_assert_eq("junban_fc_0", DateOverlayRules.junban_init(0), [0, 2, 1])
	_assert_eq("junban_fc_1", DateOverlayRules.junban_init(1), [1, 2, 0])
	_assert_eq("junban_fc_2", DateOverlayRules.junban_init(2), [2, 1, 0])
	_assert_eq("junban_fc_3", DateOverlayRules.junban_init(3), [0, 1, 2])

	# 6. All 21 date spots setup & sub-spot branching
	var st_spots := DateOverlayRules.create_default_state()
	for s in range(21):
		var sinfo := DateOverlayRules.setup_date_spot(st_spots, s)
		_assert_eq("spot_%02d_id" % s, sinfo["spot_id"], s)
		_assert_true("spot_%02d_has_banner" % s, String(sinfo["sub_place_title_hex"]).length() >= 4)

	# Central Park cherry blossoms in April (< 16) vs pond on 2nd visit
	var st_cp := DateOverlayRules.create_default_state({"month": 4, "day": 10})
	var cp0 := DateOverlayRules.setup_date_spot(st_cp, DateOverlayRules.SPOT_CENTRAL_PARK)
	_assert_eq("central_park_april_cherry_avenue", cp0["sub_spot_key"], "cherry_avenue")
	st_cp["scene_counters"][DateOverlayRules.SPOT_CENTRAL_PARK] = 1
	var cp1 := DateOverlayRules.setup_date_spot(st_cp, DateOverlayRules.SPOT_CENTRAL_PARK)
	_assert_eq("central_park_visit1_pond", cp1["sub_spot_key"], "pond")

	# Shopping Street 3 shops
	var st_shop := DateOverlayRules.create_default_state()
	_assert_eq("shop_0_boutique", DateOverlayRules.setup_date_spot(st_shop, DateOverlayRules.SPOT_SHOPPING_STREET)["sub_spot_key"], "boutique")
	st_shop["scene_counters"][DateOverlayRules.SPOT_SHOPPING_STREET] = 1
	_assert_eq("shop_1_fancy", DateOverlayRules.setup_date_spot(st_shop, DateOverlayRules.SPOT_SHOPPING_STREET)["sub_spot_key"], "fancy_shop")
	st_shop["scene_counters"][DateOverlayRules.SPOT_SHOPPING_STREET] = 2
	_assert_eq("shop_2_junk", DateOverlayRules.setup_date_spot(st_shop, DateOverlayRules.SPOT_SHOPPING_STREET)["sub_spot_key"], "junk_shop")

	# Aquarium indoor vs Dolphin Show
	var st_aq := DateOverlayRules.create_default_state()
	_assert_eq("aquarium_0_indoor", DateOverlayRules.setup_date_spot(st_aq, DateOverlayRules.SPOT_AQUARIUM)["sub_spot_key"], "indoor_hall")
	st_aq["scene_counters"][DateOverlayRules.SPOT_AQUARIUM] = 2
	_assert_eq("aquarium_2_dolphin_show", DateOverlayRules.setup_date_spot(st_aq, DateOverlayRules.SPOT_AQUARIUM)["sub_spot_key"], "dolphin_show")

	# Art Museum exhibit repeat penalty
	var st_art := DateOverlayRules.create_default_state({"month": 5, "day": 14})
	var art1 := DateOverlayRules.setup_date_spot(st_art, DateOverlayRules.SPOT_ART_MUSEUM)
	_assert_eq("art_museum_may_gargy", art1["sub_spot_key"], "gargy_exhibit")
	var aff_before: int = st_art["girls"][DateOverlayRules.GIRL_SHIORI]["affection"]
	DateOverlayRules.setup_date_spot(st_art, DateOverlayRules.SPOT_ART_MUSEUM)
	_assert_eq("art_museum_repeat_exhibit_affection_minus_2", st_art["girls"][DateOverlayRules.GIRL_SHIORI]["affection"], aff_before - 2)

	# Game Center Medal Corner unlock in Sept 1995+ on 3rd visit
	var st_gc := DateOverlayRules.create_default_state({"year": 1995, "month": 9})
	st_gc["scene_counters"][DateOverlayRules.SPOT_GAME_CENTER] = 2
	_assert_eq("game_center_medals_open", DateOverlayRules.setup_date_spot(st_gc, DateOverlayRules.SPOT_GAME_CENTER)["sub_spot_key"], "medal_game_corner")

	# Karaoke 3rd visit transitions to GAME_MODE_AMUSEMENT_PARK_2 (0x58) stage 3
	var st_kara := DateOverlayRules.create_default_state()
	st_kara["scene_counters"][DateOverlayRules.SPOT_KARAOKE] = 2
	var kara_info := DateOverlayRules.setup_date_spot(st_kara, DateOverlayRules.SPOT_KARAOKE)
	_assert_eq("karaoke_3rd_visit_date2_mode", kara_info["game_mode"], DateOverlayRules.GAME_MODE_AMUSEMENT_PARK_2)
	_assert_eq("karaoke_3rd_visit_stage_3", kara_info["mode_stage"], DateOverlayRules.DATE2_STAGE_KARAOKE)
	var kara_end := DateOverlayRules.finish_date2_karaoke_song(st_kara, 1, true)
	_assert_eq("karaoke_return_to_date_mode", kara_end["game_mode"], DateOverlayRules.GAME_MODE_DATE)
	_assert_eq("karaoke_return_to_date_step_0x11", kara_end["mode_step"], 0x11)
	_assert_eq("karaoke_full_song_tokimeki_plus_2", kara_end["tokimeki"], 62)

	# 7. 5-class choice evaluation & 16.16 fixed-point multipliers (date_choice_score)
	var st_score := DateOverlayRules.create_default_state({
		"param_trivia": 80, # >= 75 -> positive trivia_mult = 4, negative trivia_mult = 4
		"current_girl": DateOverlayRules.GIRL_SHIORI,
	})
	DateOverlayRules.setup_date_spot(st_score, DateOverlayRules.SPOT_AQUARIUM) # Shiori Aquarium affinity = 1 -> pos place_mult = 4, neg place_mult = 4
	# Choice 0 is Class 0 (Best): tokimeki_delta = +3, affection_delta = +2, syoushin_weight = -10
	# m1 = (4, 4), m2 = (4, 4) -> m1.x * m1.y = 16 -> fp_delta = ((delta * 16) << 16) / 16 = delta << 16!
	var s_best := DateOverlayRules.date_choice_score(st_score, 0)
	_assert_eq("score_class0_tier0", s_best["tier"], 0)
	_assert_eq("score_class0_mood_face1", s_best["mood_face"], 1)
	_assert_eq("score_class0_tokimeki_fp_delta", s_best["tokimeki_fp_delta"], 3 * 65536)
	_assert_eq("score_class0_affection_fp_delta", s_best["affection_fp_delta"], 2 * 65536)
	_assert_eq("score_class0_syoushin_fp_delta", s_best["syoushin_fp_delta"], -10 * 65536)

	# Choice 1 is Class 4 (Terrible): tokimeki_delta = -1, affection_delta = -2, syoushin_weight = +10
	var s_terrible := DateOverlayRules.date_choice_score(st_score, 1)
	_assert_eq("score_class4_tier2", s_terrible["tier"], 2)
	_assert_eq("score_class4_mood_face3", s_terrible["mood_face"], 3)
	_assert_eq("score_class4_tokimeki_fp_delta", s_terrible["tokimeki_fp_delta"], -1 * 65536)
	_assert_eq("score_class4_affection_fp_delta", s_terrible["affection_fp_delta"], -2 * 65536)
	_assert_eq("score_class4_syoushin_fp_delta", s_terrible["syoushin_fp_delta"], 10 * 65536)

	# Choice 2 is Class 2 (Mediocre): tokimeki_delta = +1, affection_delta = 0, syoushin_weight = -6
	var s_med := DateOverlayRules.date_choice_score(st_score, 2)
	_assert_eq("score_class2_tier1", s_med["tier"], 1)
	_assert_eq("score_class2_mood_face2", s_med["mood_face"], 2)
	_assert_eq("score_class2_affection_fp_delta_zero", s_med["affection_fp_delta"], 0)

	# Pool athletics < 70 lie caught override
	var st_pool := DateOverlayRules.create_default_state({"param_athletics": 55})
	DateOverlayRules.setup_date_spot(st_pool, DateOverlayRules.SPOT_POOL)
	var s_pool := DateOverlayRules.date_choice_score(st_pool, 0)
	_assert_eq("pool_low_athletics_lie_caught_class4", s_pool["response_class"], 4)
	_assert_eq("pool_low_athletics_reaction_line_5", s_pool["reaction_line"], 5)

	# 8. Post-date farewell, repeat-spot penalty, special scene handoff, and date_end
	var st_end := DateOverlayRules.create_default_state()
	DateOverlayRules.setup_date_spot(st_end, DateOverlayRules.SPOT_LIBRARY) # Library maps last_date_spot 8 -> 7 (Art Museum)
	DateOverlayRules.date_choice_score(st_end, 0)
	var end1 := DateOverlayRules.finish_date(st_end)
	_assert_eq("library_maps_last_spot_to_art_museum_7", end1["mapped_last_spot"], DateOverlayRules.SPOT_ART_MUSEUM)
	_assert_eq("date_end_timer_12", end1["last_date_spot_timer"], 12)
	_assert_eq("date_end_day_event_stage_1", end1["day_event_stage"], 1)

	# Repeat spot at Art Museum right after Library triggers repeat-spot penalty (farewell_group = 0x0B)
	DateOverlayRules.setup_date_spot(st_end, DateOverlayRules.SPOT_ART_MUSEUM)
	DateOverlayRules.date_choice_score(st_end, 0)
	var end2 := DateOverlayRules.finish_date(st_end)
	_assert_true("repeat_spot_penalty_triggered", bool(end2["repeat_spot_penalty_applied"]))
	_assert_eq("repeat_spot_farewell_group_0x0B", end2["farewell_group"], 0x0B)

	# Special scene handoff (Shiori Spring Central Park -> GAME_MODE_DATE_SCENE 0x5E)
	var st_spec := DateOverlayRules.create_default_state({"month": 4, "day": 9})
	DateOverlayRules.setup_date_spot(st_spec, DateOverlayRules.SPOT_CENTRAL_PARK)
	var end_spec := DateOverlayRules.finish_date(st_spec)
	_assert_eq("special_scene_handoff_outcome", end_spec["outcome"], "special_scene")
	_assert_eq("special_scene_game_mode_0x5E", end_spec["game_mode"], DateOverlayRules.GAME_MODE_DATE_SCENE)


func _run_presenter_and_capture_tests(
	hud_manifest: String,
	font_root: String,
	date_pack_root: String,
	capture_dir: String
) -> void:
	DirAccess.make_dir_recursive_absolute(capture_dir)
	var presenter := DateOverlayPresenterScript.new()
	var opened := presenter.open_date_preview(hud_manifest, font_root, date_pack_root)
	_assert_true("presenter_open_date_preview", opened)
	if not opened:
		push_error("Presenter failed to open: " + presenter.error)
		return

	var state := DateOverlayRules.create_default_state({
		"year": 1995,
		"month": 7,
		"day": 23,
		"weekday": 6,
		"appointment_place_0": DateOverlayRules.SPOT_AQUARIUM,
	})
	DateOverlayRules.date_init(state)
	DateOverlayRules.date_arrival_and_looks_check(state)

	# 1. Capture 01: Date Arrival & Greeting
	var cap1 := presenter.render_date_arrival_greeting(state)
	_verify_and_save_capture(cap1, capture_dir.path_join("01_date_arrival_greeting.png"))

	# 2. Capture 02: Date 3-Choice Prompt (Aquarium Indoor)
	DateOverlayRules.setup_date_spot(state, DateOverlayRules.SPOT_AQUARIUM)
	var cap2 := presenter.render_date_three_choice_prompt(state, 0)
	_verify_and_save_capture(cap2, capture_dir.path_join("02_date_three_choice_prompt.png"))

	# 3. Capture 03: Date Spot Reaction (Tier 0 Best)
	var cap3 := presenter.render_date_spot_reaction(state, 0)
	_verify_and_save_capture(cap3, capture_dir.path_join("03_date_spot_reaction.png"))

	# 4. Capture 04: Amusement Park Roller Coaster Sub-scene Choice
	var cap4 := presenter.render_amusement_or_karaoke_subscene(state, 0, "amusement_coaster")
	_verify_and_save_capture(cap4, capture_dir.path_join("04_amusement_or_karaoke_subscene.png"))

	# Also verify Karaoke DATE2 2x2 sub-scene rendering & zero glyph bbox violations
	var cap_kara := presenter.render_amusement_or_karaoke_subscene(state, 1, "karaoke_date2")
	_assert_eq("karaoke_date2_bbox_violations", int(cap_kara["bbox_check"]["violation_count"]), 0)


func _verify_and_save_capture(render_result: Dictionary, out_path: String) -> void:
	var id := String(render_result.get("id", "capture"))
	var img: Image = render_result.get("image")
	_assert_true("%s_image_present" % id, img != null)
	if img == null:
		return
	_assert_eq("%s_width_320" % id, img.get_width(), 320)
	_assert_eq("%s_height_240" % id, img.get_height(), 240)
	var bbox: Dictionary = render_result.get("bbox_check", {})
	_assert_eq("%s_zero_bbox_violations" % id, int(bbox.get("violation_count", -1)), 0)
	_assert_true("%s_checked_glyphs_positive" % id, int(bbox.get("checked_glyph_count", 0)) > 0)

	var save_err := img.save_png(out_path)
	_assert_eq("%s_save_png_ok" % id, save_err, OK)
