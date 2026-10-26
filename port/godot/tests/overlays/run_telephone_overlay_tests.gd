extends SceneTree
## T-29018: Test suite for TEL Telephone Overlay (rules + presenter)
## Tests:
## 1. Girl profile & appraisal star rating rules, heartbreak bomb indicator, zodiac signs
## 2. Date place availability rules (page 0 & page 1 seasonal/event checks)
## 3. Date day availability rules (28-day candidate scan, Sunday/holiday filter, blackout rules)
## 4. Invitation acceptance / refusal evaluation formula
## 5. Appointment booking into game_state.unk_54C[0..7] and list sorting
## 6. Rendering 4 authentic 320x240 PS1 screens and asserting zero bounding box violations

const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const OverlayRules = TelephoneOverlay
const HolidayAppointmentRules = preload("res://rules/holiday_appointment.gd")
const TelephoneOverlayPresenter = preload("res://presentation/telephone_overlay_presenter.gd")
const NativeRng = preload("res://core/native_rng.gd")

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
	# Met == false
	var unmeet := TelephoneOverlay.calculate_girl_appraisal(0, false, 5, 100, 0)
	_check(unmeet["met"] == false and unmeet["stars"] == 0, "unmet girl has 0 stars")

	# Stars rating mapping
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

	# Heartbreak bomb indicator (syoushin >= 60)
	var no_bomb := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, 59)
	_check(no_bomb["bomb"] == false, "syoushin 59 has no bomb")

	var bomb60 := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, 60)
	_check(bomb60["bomb"] == true and bomb60["blink"] > 0, "syoushin 60 triggers bomb with blink period")

	var bomb80 := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, 80)
	_check(bomb80["bomb"] == true, "syoushin 80 triggers bomb")

	# Zodiac sign thresholds
	_check(TelephoneOverlay.get_zodiac_sign(5, 27) == "ふたご座", "May 27 is Gemini (ふたご座)")
	_check(TelephoneOverlay.get_zodiac_sign(2, 3) == "みずがめ座", "Feb 3 is Aquarius (みずがめ座)")
	_check(TelephoneOverlay.get_zodiac_sign(12, 25) == "やぎ座", "Dec 25 is Capricorn (やぎ座)")


# ---------------------------------------------------------------------------
# Suite 2: Date Place Availability
# ---------------------------------------------------------------------------

func _test_date_place_availability() -> void:
	var progress := [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var flags := {}

	# Page 0: all progress 0 -> only page toggle available
	var p0_locked := TelephoneOverlay.build_date_place_availability(0, 95, 4, 9, progress, flags, 4)
	_check(p0_locked[10] == 0x64, "Page 0 toggle button is available")
	_check(p0_locked[0] == 0 and p0_locked[1] == 0, "Spots locked without progress or events")

	# Page 0: Unlocked via progress_state & 0x7F
	progress[0] = 1 # Nearby Park
	progress[2] = 1 # Shopping Street
	var p0_unlocked := TelephoneOverlay.build_date_place_availability(0, 95, 4, 9, progress, flags, 4)
	_check(p0_unlocked[0] == 0x74, "Nearby park unlocked via progress")
	_check(p0_unlocked[2] == 0x74, "Shopping street unlocked via progress")

	# Page 0: Event unlocks (e.g. June 1995 Central park via b1)
	flags["b1"] = 1
	var p0_event := TelephoneOverlay.build_date_place_availability(0, 95, 6, 15, progress, flags, 6)
	_check(p0_event[1] == 0x74, "Central park unlocked in June 95 via event flag b1")

	# Page 1: Summer spots (Pool & Sea) during summer vacation (August)
	var p1_summer := TelephoneOverlay.build_date_place_availability(1, 95, 8, 10, progress, flags, 8)
	_check(p1_summer[6] == 0x74, "Pool available in August (summer vacation)")
	_check(p1_summer[7] == 0x74, "Sea/Beach available in August (summer vacation)")

	# Page 1: Winter spots (Skating & Skiing) during winter vacation (January)
	var p1_winter := TelephoneOverlay.build_date_place_availability(1, 96, 1, 3, progress, flags, 1)
	_check(p1_winter[6] == 0x74, "Skate rink available in January (winter vacation)")
	_check(p1_winter[7] == 0x74, "Ski resort available in January (winter vacation)")

	# Page 1: Shrine during August first week festival
	flags["b15"] = 1
	var p1_shrine := TelephoneOverlay.build_date_place_availability(1, 95, 8, 2, progress, flags, 8)
	_check(p1_shrine[9] == 0x74, "Shrine festival available during first week of August")


# ---------------------------------------------------------------------------
# Suite 3: Date Day Availability
# ---------------------------------------------------------------------------

func _test_date_day_availability() -> void:
	# Build 2-month calendar for April/May 1995:
	# April 1995 (30 days): Sundays on 2, 9, 16, 23, 30; Holiday on 29 (Showa Day)
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
	# May 1995 (31 days): Holidays on 3 (Constitution), 5 (Children's Day); Sundays on 7, 14, 21, 28
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

	# Check that April 9 (Sunday) is included
	var has_apr_9 := false
	for s in slots:
		if s["month"] == 4 and s["day"] == 9:
			has_apr_9 = true
			break
	_check(has_apr_9, "April 9 Sunday is offered as date slot")

	# Now book April 9 and verify it is no longer offered
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
	# High trivia & high closeness -> guaranteed acceptance
	var rng := _make_rng(100)
	var res_high := TelephoneOverlay.evaluate_invitation(OverlayRules.GIRL_SHIORI, OverlayRules.PLACE_NEARBY_PARK, 200, 50, 100, rng)
	_check(res_high["accepted"] == true and res_high["outcome"] == "accepted", "high stats result in date acceptance")

	# 0 trivia & 0 closeness & 0 looks -> refusal
	var rng_low := _make_rng(200)
	var res_low := TelephoneOverlay.evaluate_invitation(OverlayRules.GIRL_SHIORI, OverlayRules.PLACE_GAME_CENTER, 0, 0, 0, rng_low)
	_check(res_low["accepted"] == false and res_low["outcome"].begins_with("refused"), "low stats result in refusal")

	# Test booking multiple appointments and chronological sorting
	var appointments := [0, 0, 0, 0, 0, 0, 0, 0]
	TelephoneOverlay.book_appointment(appointments, OverlayRules.GIRL_SHIORI, OverlayRules.PLACE_NEARBY_PARK, 95, 5, 14)
	TelephoneOverlay.book_appointment(appointments, OverlayRules.GIRL_SAKI, OverlayRules.PLACE_AQUARIUM, 95, 4, 23)
	TelephoneOverlay.book_appointment(appointments, OverlayRules.GIRL_MIO, OverlayRules.PLACE_LIBRARY, 95, 4, 16)

	# Verify appointments sorted by date (April 16 -> April 23 -> May 14)
	var app0 := HolidayAppointmentRules.unpack_appointment_word(appointments[0])
	var app1 := HolidayAppointmentRules.unpack_appointment_word(appointments[1])
	var app2 := HolidayAppointmentRules.unpack_appointment_word(appointments[2])
	_check(app0["month"] == 4 and app0["day"] == 16, "first appointment is April 16 (Library)")
	_check(app1["month"] == 4 and app1["day"] == 23, "second appointment is April 23 (Aquarium)")
	_check(app2["month"] == 5 and app2["day"] == 14, "third appointment is May 14 (Park)")


# ---------------------------------------------------------------------------
# Suite 5: Rendering & Zero Bounding-Box Violations
# ---------------------------------------------------------------------------

func _test_rendering_and_captures() -> void:
	var root_candidates := [
		"/workspace",
		ProjectSettings.globalize_path("res://../../").simplify_path(),
		ProjectSettings.globalize_path("res://").simplify_path().get_base_dir().get_base_dir(),
	]
	var repo_root := "/workspace"
	for r in root_candidates:
		if DirAccess.dir_exists_absolute(r.path_join(".asset-cache/port")):
			repo_root = r
			break

	var hud_manifest := repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root := repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack := repo_root.path_join(".asset-cache/port/date-preview-v1")
	var captures_dir := repo_root.path_join(".claude/orchestrator/state/T-29018/tel-overlay-v1/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)

	var presenter := TelephoneOverlayPresenter.new()
	var opened := presenter.open_telephone_preview(hud_manifest, font_root, date_pack)
	_check(opened, "TelephoneOverlayPresenter initialized resources: " + presenter.error)
	if not opened:
		return

	# 1. Render Yoshio's Girl Profile Notebook Screen (0x3E Stage 0)
	var img_notebook := presenter.render_girl_profile_notebook_screen(OverlayRules.GIRL_SHIORI)
	_check(img_notebook.get_size() == Vector2i(320, 240), "Notebook screen rendered 320x240")
	var save_err1 := img_notebook.save_png(captures_dir.path_join("tel_01_notebook_profile.png"))
	_check(save_err1 == OK, "Saved captures/tel_01_notebook_profile.png")

	# 2. Render Telephone Date Place Selection Menu
	var avail := [0x74, 0x74, 0x74, 0x74, 0x74, 0, 0, 0, 0, 0, 0x64]
	var img_place := presenter.render_date_place_menu_screen(0, 0, avail)
	_check(img_place.get_size() == Vector2i(320, 240), "Date place menu rendered 320x240")
	var save_err2 := img_place.save_png(captures_dir.path_join("tel_02_date_place_menu.png"))
	_check(save_err2 == OK, "Saved captures/tel_02_date_place_menu.png")

	# 3. Render Telephone Date Day Selection Menu
	var candidate_slots: Array[Dictionary] = [
		{"month": 4, "day": 9, "month_offset": 0},
		{"month": 4, "day": 16, "month_offset": 0},
		{"month": 4, "day": 23, "month_offset": 0},
		{"month": 5, "day": 7, "month_offset": 1}
	]
	var img_day := presenter.render_date_day_menu_screen(candidate_slots, 0)
	_check(img_day.get_size() == Vector2i(320, 240), "Date day menu rendered 320x240")
	var save_err3 := img_day.save_png(captures_dir.path_join("tel_03_date_day_menu.png"))
	_check(save_err3 == OK, "Saved captures/tel_03_date_day_menu.png")

	# 4. Render Girl Phone Call Response VN Scene
	var img_call := presenter.render_girl_phone_call_vn_scene(OverlayRules.GIRL_SHIORI, "accepted")
	_check(img_call.get_size() == Vector2i(320, 240), "Girl call VN scene rendered 320x240")
	var save_err4 := img_call.save_png(captures_dir.path_join("tel_04_call_vn_scene.png"))
	_check(save_err4 == OK, "Saved captures/tel_04_call_vn_scene.png")

	# 5. Verify Zero Bounding Box Violations
	_check(presenter.bounding_box_violations.is_empty(), "Zero glyph bounding-box violations in notebook or message window")
	if not presenter.bounding_box_violations.is_empty():
		for v in presenter.bounding_box_violations:
			printerr("VIOLATION: ", v)
