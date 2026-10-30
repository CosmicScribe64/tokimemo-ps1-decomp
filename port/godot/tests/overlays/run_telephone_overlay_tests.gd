extends SceneTree
## T-29018 / T-29022: Test suite for TEL Telephone Overlay (rules + presenter + live state + OverlayDispatcher)
## Tests:
## 1. Girl profile & appraisal star rating rules, heartbreak bomb indicator, zodiac signs
## 2. Date place availability rules (page 0 & page 1 seasonal/event checks)
## 3. Date day availability rules (28-day candidate scan, Sunday/holiday filter, blackout rules)
## 4. Invitation acceptance / refusal evaluation formula
## 5. Appointment booking into game_state.unk_54C[0..7] and list sorting
## 6. Rendering 4 authentic 320x240 PS1 screens with explicit live state and asserting zero bounding box violations
## 7. Live-state HUD parameter & date mutation checks across telephone screens + OverlayDispatcher integration

const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const OverlayRules = TelephoneOverlay
const HolidayAppointmentRules = preload("res://rules/holiday_appointment.gd")
const TelephoneOverlayPresenter = preload("res://presentation/telephone_overlay_presenter.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const NativeRng = preload("res://core/native_rng.gd")

const PARAM_PANEL_RECT := Rect2i(28, 23, 264, 42)
const DATE_PILLAR_RECT := Rect2i(294, 45, 20, 114)

var _failures: Array[String] = []
var _checks: int = 0

func _initialize() -> void:
	call_deferred("_run")

func _check(cond: bool, msg: String) -> void:
	_checks += 1
	if not cond:
		_failures.append(msg)
		printerr("FAIL: ", msg)
	else:
		print("PASS: ", msg)

func _make_rng(seed_val: int) -> NativeRng:
	var rng := NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, seed_val)
	return rng

func _build_native_prompt() -> Dictionary:
	return {
		"schema": "tokimemo.phone-wait-marker-view.v1",
		"source_function": "talk_text_step",
		"waiting_for_input": true,
		"k_disp_inc_result": 1,
		"voice_mode": 0,
		"speaker_voice_state": 0,
		"marker_visible": true,
		"marker_native_y": 0x48,
	}

func _build_sunday_apr9_live_rule_state() -> Dictionary:
	var cal0 := {
		"days": 30,
		"day_kind": [
			30,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			2, 1, 0
		],
		"marker": [
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		],
	}
	var cal1 := {
		"days": 31,
		"day_kind": [
			31,
			0, 0, 2, 0, 2, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0
		],
		"marker": [
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		],
	}
	var stats: Array[int] = [97, 43, 40, 40, 38, 33, 59, 4, 3]
	var params: Array[int] = []
	for s: int in stats:
		params.append(s << 16)
	var progress: Array[int] = []
	progress.resize(21)
	progress.fill(1)
	var girls: Array[Dictionary] = []
	for i: int in range(11):
		girls.append({
			"met": i == 0,
			"rank": 4 if i == 0 else 80,
			"points": 120 if i == 0 else 20,
			"syoushin": 10,
			"closeness": 60 if i == 0 else 10,
			"affection": 60 if i == 0 else 10,
			"tokimeki": 55 if i == 0 else 10,
		})
	return {
		"year": 95,
		"month": 4,
		"day": 9,
		"weekday": 6,
		"day_kind": 1,
		"day_count": 5,
		"player_birthday": 0x19B,
		"stats": stats,
		"params": params,
		"calendar": [cal0, cal1],
		"progress_state": progress,
		"quarter_flags": {"b13": 1, "b14": 1},
		"appointment_words": [0, 0, 0, 0, 0, 0, 0, 0],
		"girls": girls,
	}

func _region_diff_bytes(img_a: Image, img_b: Image, rect: Rect2i) -> int:
	var reg_a: Image = img_a.get_region(rect)
	var reg_b: Image = img_b.get_region(rect)
	var data_a: PackedByteArray = reg_a.get_data()
	var data_b: PackedByteArray = reg_b.get_data()
	var diffs: int = 0
	var count: int = mini(data_a.size(), data_b.size())
	for i: int in range(count):
		if data_a[i] != data_b[i]:
			diffs += 1
	return diffs

func _run() -> void:
	print("--- T-29018 Suite 1: Profile & Appraisal Rules ---")
	_test_girl_profile_appraisal_rules()

	print("--- T-29018 Suite 2: Date Place Availability ---")
	_test_date_place_availability()

	print("--- T-29018 Suite 3: Date Day Availability ---")
	_test_date_day_availability()

	print("--- T-29018 Suite 4: Invitation Acceptance & Appointment Booking ---")
	_test_invitation_acceptance_and_booking()

	print("--- T-29018 Suite 5: Rendering & Zero Bounding-Box Violations ---")
	await _test_rendering_and_captures()

	print("--- T-29022 Suite 6: Live-State HUD Mutations & OverlayDispatcher Integration ---")
	_test_live_state_hud_and_dispatcher_mutations()

	if not _failures.is_empty():
		printerr("=======================================================")
		printerr("T29018 telephone overlay: %d/%d assertions failed." % [_failures.size(), _checks])
		printerr("=======================================================")
		quit(1)
		return

	print("=======================================================")
	print("T29018 telephone overlay: ALL %d assertions PASS." % _checks)
	print("=======================================================")
	quit(0)


# ---------------------------------------------------------------------------
# Suite 1: Profile & Appraisal Rules
# ---------------------------------------------------------------------------

func _test_girl_profile_appraisal_rules() -> void:
	var unmeet := TelephoneOverlay.calculate_girl_appraisal(0, false, 5, 100, 0)
	_check(unmeet["met"] == false and unmeet["stars"] == 0, "unmet girl has 0 stars")

	var star6 := TelephoneOverlay.calculate_girl_appraisal(0, true, 4, 100, 0)
	_check(star6["stars"] == 6, "rank < 6 yields 6 stars")

	var star5 := TelephoneOverlay.calculate_girl_appraisal(0, true, 8, 100, 0)
	_check(star5["stars"] == 5, "rank < 11 yields 5 stars")

	var star4 := TelephoneOverlay.calculate_girl_appraisal(0, true, 15, 100, 0)
	_check(star4["stars"] == 4, "rank < 21 yields 4 stars")

	var star3 := TelephoneOverlay.calculate_girl_appraisal(0, true, 30, 100, 0)
	_check(star3["stars"] == 3, "rank < 41 yields 3 stars")

	var star2 := TelephoneOverlay.calculate_girl_appraisal(0, true, 50, 100, 0)
	_check(star2["stars"] == 2, "rank < 61 yields 2 stars")

	var star1 := TelephoneOverlay.calculate_girl_appraisal(0, true, 80, 100, 0)
	_check(star1["stars"] == 1, "rank >= 61 yields 1 star")

	var star0 := TelephoneOverlay.calculate_girl_appraisal(0, true, 2, 500, 0)
	_check(star0["stars"] == 0, "points >= 500 yields 0 stars")

	var no_bomb := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, 59)
	_check(no_bomb["bomb"] == false, "syoushin 59 has no bomb")

	var bomb60 := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, 60)
	_check(bomb60["bomb"] == true and bomb60["blink"] > 0, "syoushin 60 triggers bomb with blink period")

	var bomb80 := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, 80)
	_check(bomb80["bomb"] == true, "syoushin 80 triggers bomb")

	_check(TelephoneOverlay.get_zodiac_sign(5, 27) == "ふたご座", "May 27 is Gemini (ふたご座)")
	_check(TelephoneOverlay.get_zodiac_sign(2, 3) == "みずがめ座", "Feb 3 is Aquarius (みずがめ座)")
	_check(TelephoneOverlay.get_zodiac_sign(12, 25) == "やぎ座", "Dec 25 is Capricorn (やぎ座)")


# ---------------------------------------------------------------------------
# Suite 2: Date Place Availability
# ---------------------------------------------------------------------------

func _test_date_place_availability() -> void:
	var progress := [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var flags := {}

	var p0_locked := TelephoneOverlay.build_date_place_availability(0, 95, 4, 9, progress, flags, 4)
	_check(p0_locked[10] == 0x64, "Page 0 toggle button is available")
	_check(p0_locked[0] == 0 and p0_locked[1] == 0, "Spots locked without progress or events")

	progress[0] = 1
	progress[2] = 1
	var p0_unlocked := TelephoneOverlay.build_date_place_availability(0, 95, 4, 9, progress, flags, 4)
	_check(p0_unlocked[0] == 0x74, "Nearby park unlocked via progress")
	_check(p0_unlocked[2] == 0x74, "Shopping street unlocked via progress")

	flags["b1"] = 1
	var p0_event := TelephoneOverlay.build_date_place_availability(0, 95, 6, 15, progress, flags, 6)
	_check(p0_event[1] == 0x74, "Central park unlocked in June 95 via event flag b1")

	var p1_summer := TelephoneOverlay.build_date_place_availability(1, 95, 8, 10, progress, flags, 8)
	_check(p1_summer[6] == 0x74, "Pool available in August (summer vacation)")
	_check(p1_summer[7] == 0x74, "Sea/Beach available in August (summer vacation)")

	var p1_winter := TelephoneOverlay.build_date_place_availability(1, 96, 1, 3, progress, flags, 1)
	_check(p1_winter[6] == 0x74, "Skate rink available in January (winter vacation)")
	_check(p1_winter[7] == 0x74, "Ski resort available in January (winter vacation)")

	flags["b15"] = 1
	var p1_shrine := TelephoneOverlay.build_date_place_availability(1, 95, 8, 2, progress, flags, 8)
	_check(p1_shrine[9] == 0x74, "Shrine festival available during first week of August")


# ---------------------------------------------------------------------------
# Suite 3: Date Day Availability
# ---------------------------------------------------------------------------

func _test_date_day_availability() -> void:
	var cal0 := {
		"days": 30,
		"day_kind": [0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			2, 1, 0
		]
	}
	var cal1 := {
		"days": 31,
		"day_kind": [0,
			0, 0, 2, 0, 2, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0, 0, 0, 0, 1,
			0, 0, 0
		]
	}

	var existing := [0, 0, 0, 0, 0, 0, 0, 0]
	var slots := TelephoneOverlay.build_available_date_slots(95, 4, 3, cal0, cal1, existing)
	_check(slots.size() > 0, "found available candidate date slots")

	var has_apr_9 := false
	for s in slots:
		if s["month"] == 4 and s["day"] == 9:
			has_apr_9 = true
			break
	_check(has_apr_9, "April 9 Sunday is offered as date slot")

	var booked := TelephoneOverlay.book_appointment(existing, 0, 0, 95, 4, 9)
	_check(booked, "booked April 9 appointment")
	var slots_after := TelephoneOverlay.build_available_date_slots(95, 4, 3, cal0, cal1, existing)
	var has_apr_9_after := false
	for s in slots_after:
		if s["month"] == 4 and s["day"] == 9:
			has_apr_9_after = true
			break
	_check(not has_apr_9_after, "April 9 is excluded after being booked")


# ---------------------------------------------------------------------------
# Suite 4: Invitation Acceptance & Appointment Booking
# ---------------------------------------------------------------------------

func _test_invitation_acceptance_and_booking() -> void:
	var rng := _make_rng(100)
	var res_high := TelephoneOverlay.evaluate_invitation(OverlayRules.GIRL_SHIORI, OverlayRules.PLACE_NEARBY_PARK, 200, 50, 100, rng)
	_check(res_high["accepted"] == true and res_high["outcome"] == "accepted", "high stats result in date acceptance")

	var rng_low := _make_rng(200)
	var res_low := TelephoneOverlay.evaluate_invitation(OverlayRules.GIRL_SHIORI, OverlayRules.PLACE_GAME_CENTER, 0, 0, 0, rng_low)
	_check(res_low["accepted"] == false and res_low["outcome"].begins_with("refused"), "low stats result in refusal")

	var appointments := [0, 0, 0, 0, 0, 0, 0, 0]
	TelephoneOverlay.book_appointment(appointments, OverlayRules.GIRL_SHIORI, OverlayRules.PLACE_NEARBY_PARK, 95, 5, 14)
	TelephoneOverlay.book_appointment(appointments, OverlayRules.GIRL_SAKI, OverlayRules.PLACE_AQUARIUM, 95, 4, 23)
	TelephoneOverlay.book_appointment(appointments, OverlayRules.GIRL_MIO, OverlayRules.PLACE_LIBRARY, 95, 4, 16)

	var app0 := HolidayAppointmentRules.unpack_appointment_word(appointments[0])
	var app1 := HolidayAppointmentRules.unpack_appointment_word(appointments[1])
	var app2 := HolidayAppointmentRules.unpack_appointment_word(appointments[2])
	_check(app0["month"] == 4 and app0["day"] == 16, "first appointment is April 16 (Library)")
	_check(app1["month"] == 4 and app1["day"] == 23, "second appointment is April 23 (Aquarium)")
	_check(app2["month"] == 5 and app2["day"] == 14, "third appointment is May 14 (Park)")


# ---------------------------------------------------------------------------
# Suite 5: Rendering & Zero Bounding-Box Violations
# ---------------------------------------------------------------------------

func _resolve_repo_root() -> String:
	var root_candidates := [
		"/workspace",
		"/repo",
		ProjectSettings.globalize_path("res://../../").simplify_path(),
		ProjectSettings.globalize_path("res://").simplify_path().get_base_dir().get_base_dir(),
	]
	var repo_root: String = OS.get_environment("TOKIMEMO_REPO_ROOT")
	if repo_root.is_empty():
		repo_root = "/workspace"
		for r in root_candidates:
			if FileAccess.file_exists(r.path_join(".asset-cache/port/hud-production-v1/manifest.json")):
				repo_root = r
				break
	return repo_root

func _test_rendering_and_captures() -> void:
	var repo_root := _resolve_repo_root()
	var hud_manifest := repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack := repo_root.path_join(".asset-cache/port/date-preview-v1")
	var captures_dir: String = OS.get_environment("TOKIMEMO_CAPTURE_DIR")
	if captures_dir.is_empty():
		captures_dir = repo_root.path_join(".claude/orchestrator/state/T-29018/tel-overlay-v1/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)

	var presenter := TelephoneOverlayPresenter.new()
	var opened := presenter.open_date_preview(hud_manifest, font_root, date_pack)
	_check(opened, "TelephoneOverlayPresenter initialized resources: " + presenter.error)
	if not opened:
		return

	var live_rule_state: Dictionary = _build_sunday_apr9_live_rule_state()
	var live_ctx: Dictionary = TelephoneOverlay.extract_live_context(live_rule_state)
	presenter.set_live_context(live_ctx)
	var session := TelephoneOverlay.create_session_from_live_state(live_rule_state, OverlayRules.GIRL_SHIORI)
	_check(session.live_year == 95 and session.live_month == 4 and session.live_day == 9 and session.live_weekday == 6,
		"TelephoneSessionState initialized from live Sunday 1995-04-09 rule_state")
	_check(session.live_stats == [97, 43, 40, 40, 38, 33, 59, 4, 3],
		"TelephoneSessionState carries live player parameters [97, 43, 40, 40, 38, 33, 59, 4, 3]")

	var render_stats: Array = live_ctx["stats"]
	var yr: int = int(live_ctx["year"])
	var mo: int = int(live_ctx["month"])
	var dy: int = int(live_ctx["day"])
	var wk: int = int(live_ctx["weekday"])
	var dk: int = int(live_ctx["day_kind"])
	var dc: int = int(live_ctx["day_count"])
	var bday: int = int(live_ctx["player_birthday"])
	var native_prompt: Dictionary = _build_native_prompt()

	# 1. Render Yoshio's Girl Profile Notebook Screen (0x3E Stage 0) with live state
	var img_notebook := presenter.render_girl_profile_notebook_screen(
		OverlayRules.GIRL_SHIORI, render_stats, yr, mo, dy, wk, dk, Vector2i(1, 0)
	)
	_check(img_notebook != null and img_notebook.get_size() == Vector2i(320, 240), "Notebook screen rendered 320x240 with live state")
	var out_notebook := captures_dir.path_join("tel_01_notebook_profile.png")
	var save_err1 := img_notebook.save_png(out_notebook) if img_notebook != null else ERR_INVALID_DATA
	_check(save_err1 == OK, "Saved captures/tel_01_notebook_profile.png")

	# 2. Render Telephone Date Place Selection Menu with live state
	var avail: Array[int] = session.available_places_page0
	var img_place := presenter.render_date_place_menu_screen(
		0, 0, avail, render_stats, yr, mo, dy, wk, dk, Vector2i(22, 21)
	)
	_check(img_place != null and img_place.get_size() == Vector2i(320, 240), "Date place menu rendered 320x240 with live state")
	var out_place := captures_dir.path_join("tel_02_date_place_menu.png")
	var save_err2 := img_place.save_png(out_place) if img_place != null else ERR_INVALID_DATA
	_check(save_err2 == OK, "Saved captures/tel_02_date_place_menu.png")

	# 3. Render Telephone Date Day Selection Menu with live state
	var candidate_slots: Array[Dictionary] = session.available_date_slots
	var img_day := presenter.render_date_day_menu_screen(
		candidate_slots, 0, render_stats, yr, mo, dy, wk, dk, Vector2i(9, 12), dc, bday, 6145
	)
	_check(img_day != null and img_day.get_size() == Vector2i(320, 240), "Date day menu rendered 320x240 with live state")
	var out_day := captures_dir.path_join("tel_03_date_day_menu.png")
	var save_err3 := img_day.save_png(out_day) if img_day != null else ERR_INVALID_DATA
	_check(save_err3 == OK, "Saved captures/tel_03_date_day_menu.png")

	# 4. Render Girl Phone Call Response VN Scene with live state
	var img_call := presenter.render_girl_phone_call_vn_scene(
		OverlayRules.GIRL_SHIORI, "accepted", render_stats, yr, mo, dy, wk, dk, Vector2i(20, 14), native_prompt
	)
	_check(img_call != null and img_call.get_size() == Vector2i(320, 240), "Girl call VN scene rendered 320x240 with live state: " + presenter.error)
	var out_call := captures_dir.path_join("tel_04_call_vn_scene.png")
	var save_err4 := img_call.save_png(out_call) if img_call != null else ERR_INVALID_DATA
	_check(save_err4 == OK, "Saved captures/tel_04_call_vn_scene.png")

	# 5. Verify Zero Bounding Box Violations
	_check(presenter.bounding_box_violations.is_empty(), "Zero glyph bounding-box violations in notebook or message window")
	if not presenter.bounding_box_violations.is_empty():
		for v in presenter.bounding_box_violations:
			printerr("VIOLATION: ", v)


# ---------------------------------------------------------------------------
# Suite 6: Live-State HUD Mutations & OverlayDispatcher Integration
# ---------------------------------------------------------------------------

func _test_live_state_hud_and_dispatcher_mutations() -> void:
	var repo_root := _resolve_repo_root()
	var hud_manifest := repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack := repo_root.path_join(".asset-cache/port/date-preview-v1")

	var presenter := TelephoneOverlayPresenter.new()
	var opened := presenter.open_date_preview(hud_manifest, font_root, date_pack)
	_check(opened, "Suite 6 presenter opened: " + presenter.error)
	if not opened:
		return

	var base_rule_state: Dictionary = _build_sunday_apr9_live_rule_state()
	var base_ctx: Dictionary = TelephoneOverlay.extract_live_context(base_rule_state)
	var base_session := TelephoneOverlay.create_session_from_live_state(base_rule_state, OverlayRules.GIRL_SHIORI)
	var avail: Array[int] = base_session.available_places_page0
	var slots: Array[Dictionary] = base_session.available_date_slots
	var native_prompt: Dictionary = _build_native_prompt()

	# 1. Verify set_live_context supplies live state when explicit args are omitted
	presenter.set_live_context(base_ctx)
	var img_nb_base := presenter.render_girl_profile_notebook_screen(OverlayRules.GIRL_SHIORI)
	var img_pl_base := presenter.render_date_place_menu_screen(0, 0, avail)
	var img_dy_base := presenter.render_date_day_menu_screen(slots, 0)
	var img_vn_base := presenter.render_girl_phone_call_vn_scene(
		OverlayRules.GIRL_SHIORI, "accepted", [], -1, -1, -1, -1, -1, Vector2i(20, 14), native_prompt
	)

	var img_nb_explicit := presenter.render_girl_profile_notebook_screen(
		OverlayRules.GIRL_SHIORI, base_ctx["stats"], 95, 4, 9, 6, 1, Vector2i(1, 0)
	)
	var img_pl_explicit := presenter.render_date_place_menu_screen(
		0, 0, avail, base_ctx["stats"], 95, 4, 9, 6, 1, Vector2i(22, 21)
	)
	var img_dy_explicit := presenter.render_date_day_menu_screen(
		slots, 0, base_ctx["stats"], 95, 4, 9, 6, 1, Vector2i(9, 12), 5, 0x19B, 6145
	)
	var img_vn_explicit := presenter.render_girl_phone_call_vn_scene(
		OverlayRules.GIRL_SHIORI, "accepted", base_ctx["stats"], 95, 4, 9, 6, 1, Vector2i(20, 14), native_prompt
	)
	_check(img_nb_base.get_data() == img_nb_explicit.get_data(),
		"Notebook screen rendered via set_live_context matches explicit live arguments byte-for-byte")
	_check(img_pl_base.get_data() == img_pl_explicit.get_data(),
		"Date place menu rendered via set_live_context matches explicit live arguments byte-for-byte")
	_check(img_dy_base.get_data() == img_dy_explicit.get_data(),
		"Date day menu rendered via set_live_context matches explicit live arguments byte-for-byte")
	_check(img_vn_base != null and img_vn_explicit != null and img_vn_base.get_data() == img_vn_explicit.get_data(),
		"Girl phone call VN scene rendered via set_live_context matches explicit live arguments byte-for-byte")

	# 2. Mutate live stats and verify parameter panel HUD pixels change across place menu, day menu, and phone call VN scene
	# (Note: on the notebook profile screen, appraisal_base_show covers y=16..157 at OT 5 over the top parameter bar at OT 11,
	# whereas cal_base_show on place/day menus starts at y=33 leaving top-row stat digits visible, and call_vn_scene shows all 9 stats.)
	var mut_stats_state: Dictionary = base_rule_state.duplicate(true)
	var mutated_stats: Array[int] = [188, 95, 89, 76, 84, 67, 91, 25, 19]
	var mutated_params: Array[int] = []
	for s: int in mutated_stats:
		mutated_params.append(s << 16)
	mut_stats_state["stats"] = mutated_stats
	mut_stats_state["params"] = mutated_params
	presenter.set_live_context(TelephoneOverlay.extract_live_context(mut_stats_state))

	var img_pl_stats := presenter.render_date_place_menu_screen(0, 0, avail)
	var img_dy_stats := presenter.render_date_day_menu_screen(slots, 0)
	var img_vn_stats := presenter.render_girl_phone_call_vn_scene(
		OverlayRules.GIRL_SHIORI, "accepted", [], -1, -1, -1, -1, -1, Vector2i(20, 14), native_prompt
	)

	_check(img_pl_base.get_data() != img_pl_stats.get_data() and _region_diff_bytes(img_pl_base, img_pl_stats, PARAM_PANEL_RECT) > 0,
		"Date place menu HUD parameter panel pixels change when live stats mutate")
	_check(img_dy_base.get_data() != img_dy_stats.get_data() and _region_diff_bytes(img_dy_base, img_dy_stats, PARAM_PANEL_RECT) > 0,
		"Date day menu HUD parameter panel pixels change when live stats mutate")
	_check(img_vn_base != null and img_vn_stats != null and img_vn_base.get_data() != img_vn_stats.get_data() and _region_diff_bytes(img_vn_base, img_vn_stats, PARAM_PANEL_RECT) > 0,
		"Girl phone call VN scene HUD parameter panel pixels change when live stats mutate")

	# 3. Mutate live date (year/month/day/weekday/day_kind/day_count) and verify date pillar HUD pixels change across all 4 screens
	var mut_date_state: Dictionary = base_rule_state.duplicate(true)
	mut_date_state["year"] = 96
	mut_date_state["month"] = 8
	mut_date_state["day"] = 24
	mut_date_state["weekday"] = 5
	mut_date_state["day_kind"] = 0
	mut_date_state["day_count"] = 142
	mut_date_state["player_birthday"] = 0x0815
	presenter.set_live_context(TelephoneOverlay.extract_live_context(mut_date_state))

	var img_nb_date := presenter.render_girl_profile_notebook_screen(OverlayRules.GIRL_SHIORI)
	var img_pl_date := presenter.render_date_place_menu_screen(0, 0, avail)
	var img_dy_date := presenter.render_date_day_menu_screen(slots, 0)
	var img_vn_date := presenter.render_girl_phone_call_vn_scene(
		OverlayRules.GIRL_SHIORI, "accepted", [], -1, -1, -1, -1, -1, Vector2i(20, 14), native_prompt
	)

	_check(img_nb_base.get_data() != img_nb_date.get_data() and _region_diff_bytes(img_nb_base, img_nb_date, DATE_PILLAR_RECT) > 0,
		"Notebook screen HUD date pillar pixels change when live calendar date mutates")
	_check(img_pl_base.get_data() != img_pl_date.get_data() and _region_diff_bytes(img_pl_base, img_pl_date, DATE_PILLAR_RECT) > 0,
		"Date place menu HUD date pillar pixels change when live calendar date mutates")
	_check(img_dy_base.get_data() != img_dy_date.get_data() and _region_diff_bytes(img_dy_base, img_dy_date, DATE_PILLAR_RECT) > 0,
		"Date day menu HUD date pillar & biorhythm pixels change when live calendar date mutates")
	_check(img_vn_base != null and img_vn_date != null and img_vn_base.get_data() != img_vn_date.get_data() and _region_diff_bytes(img_vn_base, img_vn_date, DATE_PILLAR_RECT) > 0,
		"Girl phone call VN scene HUD date pillar pixels change when live calendar date mutates")

	# 4. Verify live state flows end-to-end through OverlayDispatcher (init_overlay, step_overlay, render_mode)
	var dispatcher := OverlayDispatcher.new()
	var od_err: String = dispatcher.open_resources(font_root, hud_manifest, date_pack)
	_check(od_err.is_empty(), "OverlayDispatcher opened resources: " + od_err)
	if not od_err.is_empty():
		return

	# 4a. GAME_MODE_TEL_ACTION (0x3E) Notebook Profile via OverlayDispatcher
	var tel_act_state_a: Dictionary = dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL_ACTION, base_rule_state)
	var od_nb_a: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL_ACTION, tel_act_state_a)
	var tel_act_state_c: Dictionary = dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL_ACTION, mut_date_state)
	var od_nb_c: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL_ACTION, tel_act_state_c)
	_check(od_nb_a.get("image") is Image and od_nb_c.get("image") is Image,
		"OverlayDispatcher rendered GAME_MODE_TEL_ACTION for base and date-mutated rule states")
	if od_nb_a.get("image") is Image and od_nb_c.get("image") is Image:
		_check(_region_diff_bytes(od_nb_a["image"], od_nb_c["image"], DATE_PILLAR_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL_ACTION updates HUD date pillar when rule_state date changes")

	# 4b. GAME_MODE_TEL (0x3D) Place Menu -> Day Menu -> Call Response via OverlayDispatcher.step_overlay + render_mode
	var tel_state_a: Dictionary = dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL, base_rule_state)
	var od_pl_a: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, tel_state_a)
	var tel_state_b: Dictionary = dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL, mut_stats_state)
	var od_pl_b: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, tel_state_b)
	var tel_state_c: Dictionary = dispatcher.init_overlay(OverlayDispatcher.GAME_MODE_TEL, mut_date_state)
	var od_pl_c: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, tel_state_c)
	if od_pl_a.get("image") is Image and od_pl_b.get("image") is Image and od_pl_c.get("image") is Image:
		_check(_region_diff_bytes(od_pl_a["image"], od_pl_b["image"], PARAM_PANEL_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL (place_select) updates HUD parameter panel when rule_state.params changes")
		_check(_region_diff_bytes(od_pl_a["image"], od_pl_c["image"], DATE_PILLAR_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL (place_select) updates HUD date pillar when rule_state date changes")

	# T-39006: GAME_MODE_TEL has no source-backed production handler, so a step with saved
	# rule_state is refused: no completion, no deltas, saved state and overlay unchanged.
	var tel_saved_before: Dictionary = base_rule_state.duplicate(true)
	var tel_refused: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_state_a, OverlayDispatcher.PAD_CIRCLE, base_rule_state, _make_rng(100))
	_check(String(tel_refused.get("error", "")).contains("refused with saved state") and not bool(tel_refused.get("completed", true))
		and (tel_refused.get("deltas", {}) as Dictionary).is_empty() and base_rule_state == tel_saved_before
		and int(tel_refused["state"].get("stage", -1)) == int(tel_state_a.get("stage", 0)),
		"OverlayDispatcher GAME_MODE_TEL with saved rule_state is refused without deltas or writes")

	# The screens below are stepped in preview (no rule_state); each init state keeps its own HUD context.
	# Step into day_select (press Circle = 0x20)
	var step_day_a: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_state_a, OverlayDispatcher.PAD_CIRCLE)
	var step_day_b: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_state_b, OverlayDispatcher.PAD_CIRCLE)
	var step_day_c: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, tel_state_c, OverlayDispatcher.PAD_CIRCLE)
	_check(String(step_day_a["state"].get("subscreen", "")) == "day_select" and (step_day_a.get("deltas", {}) as Dictionary).is_empty(),
		"OverlayDispatcher GAME_MODE_TEL preview steps into day_select without deltas")
	var od_dy_a: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, step_day_a["state"])
	var od_dy_b: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, step_day_b["state"])
	var od_dy_c: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, step_day_c["state"])
	if od_dy_a.get("image") is Image and od_dy_b.get("image") is Image and od_dy_c.get("image") is Image:
		_check(_region_diff_bytes(od_dy_a["image"], od_dy_b["image"], PARAM_PANEL_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL (day_select) updates HUD parameter panel when rule_state.params changes")
		_check(_region_diff_bytes(od_dy_a["image"], od_dy_c["image"], DATE_PILLAR_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL (day_select) updates HUD date pillar when rule_state date changes")

	# Step into call_response (press Circle = 0x20)
	var step_call_a: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, step_day_a["state"], OverlayDispatcher.PAD_CIRCLE, {}, _make_rng(100))
	var step_call_b: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, step_day_b["state"], OverlayDispatcher.PAD_CIRCLE, {}, _make_rng(100))
	var step_call_c: Dictionary = dispatcher.step_overlay(OverlayDispatcher.GAME_MODE_TEL, step_day_c["state"], OverlayDispatcher.PAD_CIRCLE, {}, _make_rng(100))
	_check(String(step_call_a["state"].get("subscreen", "")) == "call_response" and (step_call_a.get("deltas", {}) as Dictionary).is_empty(),
		"OverlayDispatcher GAME_MODE_TEL preview steps into call_response without deltas")
	var od_vn_a: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, step_call_a["state"])
	var od_vn_b: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, step_call_b["state"])
	var od_vn_c: Dictionary = dispatcher.render_mode(OverlayDispatcher.GAME_MODE_TEL, step_call_c["state"])
	if od_vn_a.get("image") is Image and od_vn_b.get("image") is Image and od_vn_c.get("image") is Image:
		_check(_region_diff_bytes(od_vn_a["image"], od_vn_b["image"], PARAM_PANEL_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL (call_response) updates HUD parameter panel when rule_state.params changes")
		_check(_region_diff_bytes(od_vn_a["image"], od_vn_c["image"], DATE_PILLAR_RECT) > 0,
			"OverlayDispatcher GAME_MODE_TEL (call_response) updates HUD date pillar when rule_state date changes")
