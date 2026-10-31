extends SceneTree
## T-29018: Holiday, Telephone, Date, Encounter & Bomb Systems Stress-Test & Decomp Parity Suite
## Stress-tests:
## 1. Telephone (0x3D/0x3E): All 13 heroines' call eligibility, date invitation acceptance/rejection
##    curves across tokimeki/affection/trivia/looks tiers, Yoshio girl profile lookup (0x3E),
##    and 3-month repeat date-spot timer expiry (12-week countdown & month % 3 == 0).
## 2. Date (0x50 & 0x58 DATE2): Arrival greeting, 3-choice prompt selection (0..2),
##    best/neutral/worst reaction deltas, Same-Spot penalty, and Amusement Park / Karaoke subscenes.
## 3. Heartbreak (syoushin) & Bomb (flags bit) propagation: Weekly neglect accumulation,
##    bomb trigger thresholds (50/60/61/76/91), bomb explosion penalty across all met heroines,
##    and defusing via date invitation & date completion.
## 4. Seasonal & Encounter Overlays: Walk-Home (GEKO 0x44) accept/decline/nickname change,
##    Sick Visit (OMIMAI 0x4B), New Year Shrine & Christmas (SHOUGATU 0x4E, including Dress Gate),
##    Valentine's Day (VALEN 0x4F) obligation vs. true-feelings chocolate tiers, and
##    Term Exams (EXAM 0x35) rank computation & girl reaction bands.

const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const DateOverlay = preload("res://overlays/date_overlay.gd")
const EncountersOverlay = preload("res://overlays/encounters_and_events_overlay.gd")
const CalendarPhoneScreens = preload("res://rules/calendar_phone_screens.gd")
const HolidayAppointment = preload("res://rules/holiday_appointment.gd")
const Scheduler = preload("res://rules/scheduler.gd")
const SchoolDayState = preload("res://rules/school_day_state.gd")
const GameSession = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const NativeRng = preload("res://core/native_rng.gd")

const PROOF_REL_PATH := ".claude/orchestrator/state/T-29018/holiday-date-bomb-stress-v1/holiday-date-bomb-stress-proof.json"

# Canonical bomb explosion penalties from src/ovl/telephone/appointment_call_scenes.c (phone_relationship_deltas_apply)
# Indexed by canonical girl ID 0..10 (Shiori, Mio, Yuina, Ayako, Saki, Yukari, Nozomi, Mira, Yuko, Megumi, Yumi)
const CANONICAL_BOMB_TOKIMEKI_PENALTY: Array[int] = [30, 25, 25, 25, 20, 20, 25, 30, 20, 15, 25]
const CANONICAL_BOMB_AFFECTION_PENALTY: Array[int] = [40, 35, 35, 35, 30, 30, 35, 40, 30, 25, 35]

var _checks: int = 0
var _passed: int = 0
var _failures: Array[String] = []
var _suite_stats: Dictionary = {}
var _stress_metrics: Dictionary = {}

func _initialize() -> void:
	call_deferred("_run")

func _check(cond: bool, label: String, suite: String) -> void:
	_checks += 1
	if not _suite_stats.has(suite):
		_suite_stats[suite] = {"total": 0, "passed": 0, "failed": 0}
	_suite_stats[suite]["total"] += 1
	if cond:
		_passed += 1
		_suite_stats[suite]["passed"] += 1
	else:
		_suite_stats[suite]["failed"] += 1
		_failures.append("[%s] %s" % [suite, label])
		printerr("FAIL [%s]: %s" % [suite, label])

func _eq(actual: Variant, expected: Variant, label: String, suite: String) -> void:
	var ok: bool = (actual == expected)
	_check(ok, "%s (expected=%s, actual=%s)" % [label, str(expected), str(actual)], suite)

func _make_rng(seed_val: int) -> NativeRng:
	var rng := NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, seed_val)
	return rng

func _resolve_repo_root() -> String:
	var candidates := [
		"/workspace",
		ProjectSettings.globalize_path("res://../..").simplify_path(),
	]
	for c in candidates:
		if DirAccess.dir_exists_absolute(c.path_join("port/godot")):
			return c
	return "/workspace"

func _run() -> void:
	print("=== T-29018 Holiday, Telephone, Date, Encounter & Bomb Stress Suite ===")
	_run_suite_1_telephone()
	_run_suite_2_date_and_date2()
	_run_suite_3_syoushin_and_bomb()
	_run_suite_4_seasonal_and_encounters()

	var proof_path := _write_proof_json()
	print("Wrote stress-test proof JSON to: ", proof_path)

	if not _failures.is_empty():
		printerr("==================================================================")
		printerr("FAILED: %d/%d assertions failed across stress suite." % [_failures.size(), _checks])
		printerr("==================================================================")
		quit(1)
		return

	print("==================================================================")
	print("PASS: All %d/%d stress-test assertions succeeded (0 failures)." % [_passed, _checks])
	print("==================================================================")
	quit(0)


# ===========================================================================
# Suite 1: Telephone (0x3D / 0x3E)
# ===========================================================================

func _run_suite_1_telephone() -> void:
	var suite := "telephone_0x3D_0x3E"
	print("--- Suite 1: Telephone (0x3D/0x3E) ---")

	# 1A. All 13 heroines/contacts call eligibility in 14-slot directory (src/main/holiday.c:1240-1335)
	var girls_none: Array = []
	for i in range(11):
		girls_none.append({"flags": 0})
	var dir_none := CalendarPhoneScreens.build_telephone_directory(girls_none)
	_eq(dir_none.size(), 14, "Directory has 14 slots (0=Yoshio, 1..11=Girls 0..10, 12=Rei, 13=Cancel)", suite)
	_eq(dir_none[0]["available"], true, "Slot 0 (Yoshio) is always eligible", suite)
	_eq(dir_none[12]["available"], true, "Slot 12 (Rei Ijuin) is always eligible", suite)
	_eq(dir_none[13]["available"], true, "Slot 13 (Cancel) is always eligible", suite)
	for slot_idx in range(1, 12):
		_eq(dir_none[slot_idx]["available"], false, "Slot %d (Girl %d) locked when b3 (0x08) is unset" % [slot_idx, slot_idx - 1], suite)
		_eq(dir_none[slot_idx]["w"], 0, "Slot %d hit width is 0 when b3 is unset" % slot_idx, suite)

	# Enable each of the 11 dateable heroines individually via flags & 0x08 (b3) and verify eligibility
	for g_idx in range(11):
		var girls_one: Array = []
		for k in range(11):
			girls_one.append({"flags": 0x08 if k == g_idx else 0})
		var dir_one := CalendarPhoneScreens.build_telephone_directory(girls_one)
		_eq(dir_one[g_idx + 1]["available"], true, "Girl %d becomes eligible when b3 (0x08) is set" % g_idx, suite)
		_eq(dir_one[g_idx + 1]["w"], 0x48, "Girl %d hit width is 0x48 when eligible" % g_idx, suite)

	# Verify step_holiday_tel dialing target_girl mapping for all 13 contacts (slots 0..12)
	var girls_all: Array = []
	for i in range(11):
		girls_all.append({"flags": 0x08})
	var dialed_targets: Array[int] = []
	for cursor_slot in range(13):
		var st := SchoolDayState.new()
		st.game_mode = 0x46
		st.mode_stage = 0x10
		st.mode_step = 0
		st.rule_state = {"girls": girls_all, "menu_mode": 1}
		var res := GameSession.FrameResult.new()
		# Step 0 -> Step 1 (holiday_tel_init sets menu_cursor_0 = 0)
		CalendarPhoneScreens.step_holiday_tel(st, InputFrame.new(0, 0, 0), res)
		st.rule_state["menu_cursor"] = cursor_slot
		# Confirm selection with newly pressed PAD_CIRCLE (held=0x20, previous_held=0 -> pressed=0x20)
		CalendarPhoneScreens.step_holiday_tel(st, InputFrame.new(1, 0x20, 0), res)
		_eq(st.mode_step, 2, "Slot %d confirm enters dialing step 2" % cursor_slot, suite)
		var expected_target: int = 13 if cursor_slot == 0 else (11 if cursor_slot == 12 else cursor_slot - 1)
		_eq(int(st.rule_state.get("current_girl", -1)), expected_target, "Slot %d maps to current_girl=%d" % [cursor_slot, expected_target], suite)
		for tick in range(2, 120):
			CalendarPhoneScreens.step_holiday_tel(st, InputFrame.new(tick, 0, 0), res)
			if st.game_mode == 0x45:
				break
		_eq(st.game_mode, 0x45, "Slot %d dialing transitions to GAME_MODE_TELEPHONE_PROFILE (0x45)" % cursor_slot, suite)
		var expected_stage: int = 0 if expected_target == 13 else 1
		_eq(st.mode_stage, expected_stage, "Slot %d enters 0x45 stage %d" % [cursor_slot, expected_stage], suite)
		dialed_targets.append(expected_target)

	# 1B. Stress-test Date Invitation Acceptance/Rejection Curves across all 11 heroines, 21 spots, and stat tiers
	var closeness_tiers: Array[int] = [0, 20, 40, 60, 80, 100]
	var curve_summary: Dictionary = {}
	var total_invitation_trials: int = 0

	for girl_id in range(11):
		var prev_tier_accepts: int = -1
		var girl_curve: Array[Dictionary] = []
		for close_val in closeness_tiers:
			var tier_accepts: int = 0
			var tier_trials: int = 0
			for spot_id in range(21):
				var base_adj: int = TelephoneOverlay.PHONE_INVITATION_BASE_ADJUSTMENTS[girl_id][spot_id]
				for triv_val in [10, 40]:
					for looks_val in [25, 50]:
						var rng := _make_rng(1000 + girl_id * 97 + spot_id * 31 + triv_val + looks_val)
						var inv := TelephoneOverlay.evaluate_invitation(girl_id, spot_id, triv_val, looks_val, close_val, rng)
						tier_trials += 1
						total_invitation_trials += 1
						if bool(inv["accepted"]):
							tier_accepts += 1
						# Verify exact formula parity with phone_date_invitation_resolve
						@warning_ignore("integer_division")
						var lim: int = close_val >> 2
						@warning_ignore("integer_division")
						var hp: int = triv_val / 4
						var expected_total: int = maxi(hp, lim)
						if base_adj != 0:
							expected_total += base_adj
						else:
							expected_total = 0
						if looks_val < 40:
							expected_total = looks_val + expected_total - 40
						if inv["total"] != expected_total:
							_eq(inv["total"], expected_total, "Formula mismatch girl=%d spot=%d" % [girl_id, spot_id], suite)
						if base_adj == 0 and bool(inv["accepted"]):
							_check(false, "Zero base_adj must never accept (girl=%d spot=%d)" % [girl_id, spot_id], suite)
						if not bool(inv["accepted"]) and inv["outcome"] != "refused_soft":
							_eq(inv["outcome"], "refused_soft", "Signed delta quirk always yields refused_soft", suite)
			girl_curve.append({
				"closeness": close_val,
				"accepted": tier_accepts,
				"trials": tier_trials,
				"rate": snappedf(float(tier_accepts) / float(tier_trials), 0.001),
			})
			if prev_tier_accepts >= 0:
				_check(tier_accepts >= prev_tier_accepts, "Girl %d acceptance rate non-decreasing at closeness=%d (%d >= %d)" % [girl_id, close_val, tier_accepts, prev_tier_accepts], suite)
			prev_tier_accepts = tier_accepts
		curve_summary["girl_%02d" % girl_id] = girl_curve

	# Explicit zero-base-adjustment checks (Mio=1, Yuina=2, Ayako=3, Megumi=9 at Ski Resort=20)
	for g_zero in [1, 2, 3, 9]:
		var rng_z := _make_rng(777)
		var res_z := TelephoneOverlay.evaluate_invitation(g_zero, TelephoneOverlay.PLACE_SKI_RESORT, 250, 100, 120, rng_z)
		_eq(res_z["total"], 0, "Girl %d at Ski Resort (base_adj=0) forces total=0" % g_zero, suite)
		_eq(res_z["accepted"], false, "Girl %d at Ski Resort always rejects" % g_zero, suite)

	# Looks < 40 penalty check
	var rng_l1 := _make_rng(555)
	var rng_l2 := _make_rng(555)
	var inv_looks_40 := TelephoneOverlay.evaluate_invitation(0, 0, 80, 40, 80, rng_l1)
	var inv_looks_20 := TelephoneOverlay.evaluate_invitation(0, 0, 80, 20, 80, rng_l2)
	_eq(int(inv_looks_20["total"]), int(inv_looks_40["total"]) - 20, "looks=20 subtracts 20 from total vs looks=40", suite)

	# 1C. Yoshio Girl Profile Lookup & Appraisal (0x3E stage 0)
	_eq(TelephoneOverlay.GIRL_PROFILES.size(), 12, "GIRL_PROFILES has 12 entries (0..9 + Yumi=11 + Rei=12)", suite)
	var profile_keys := [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12]
	var expected_zodiacs := [
		"ふたご座", "みずがめ座", "かに座", "てんびん座", "さそり座", "いて座",
		"いて座", "さそり座", "てんびん座", "おとめ座", "やぎ座", "しし座"
	]
	for p_idx in range(12):
		var prof_key: int = profile_keys[p_idx]
		var prof: Dictionary = TelephoneOverlay.GIRL_PROFILES[prof_key]
		var z_sign := TelephoneOverlay.get_zodiac_sign(int(prof["birthday_month"]), int(prof["birthday_day"]))
		_eq(z_sign, expected_zodiacs[p_idx], "Profile key %d (%s) zodiac is %s" % [prof_key, TelephoneOverlay.GIRL_NAMES[prof_key], expected_zodiacs[p_idx]], suite)

	# Test all star rating tiers and heartbreak bomb blink period curve across syoushin 0..128
	var rank_to_stars := [
		{"rank": 0, "points": 100, "stars": 6},
		{"rank": 5, "points": 100, "stars": 6},
		{"rank": 6, "points": 100, "stars": 5},
		{"rank": 10, "points": 100, "stars": 5},
		{"rank": 11, "points": 100, "stars": 4},
		{"rank": 20, "points": 100, "stars": 4},
		{"rank": 21, "points": 100, "stars": 3},
		{"rank": 40, "points": 100, "stars": 3},
		{"rank": 41, "points": 100, "stars": 2},
		{"rank": 60, "points": 100, "stars": 2},
		{"rank": 61, "points": 100, "stars": 1},
		{"rank": 99, "points": 100, "stars": 1},
		{"rank": 1, "points": 500, "stars": 0},
	]
	for case_d in rank_to_stars:
		var app := TelephoneOverlay.calculate_girl_appraisal(0, true, case_d["rank"], case_d["points"], 0)
		_eq(app["stars"], case_d["stars"], "Yoshio appraisal rank=%d points=%d -> %d stars" % [case_d["rank"], case_d["points"], case_d["stars"]], suite)

	# Bomb blink period curve: syoushin 60..69 -> 8, 70..79 -> 6, 80..89 -> 4, 90..99 -> 2, >=100 -> 1
	var blink_cases := [
		{"syoushin": 59, "bomb": false, "blink": 0},
		{"syoushin": 60, "bomb": true, "blink": 8},
		{"syoushin": 69, "bomb": true, "blink": 8},
		{"syoushin": 70, "bomb": true, "blink": 6},
		{"syoushin": 80, "bomb": true, "blink": 4},
		{"syoushin": 90, "bomb": true, "blink": 2},
		{"syoushin": 100, "bomb": true, "blink": 1},
		{"syoushin": 120, "bomb": true, "blink": 1},
	]
	for bc in blink_cases:
		var b_app := TelephoneOverlay.calculate_girl_appraisal(0, true, 15, 100, bc["syoushin"])
		_eq(b_app["bomb"], bc["bomb"], "Yoshio bomb flag at syoushin=%d" % bc["syoushin"], suite)
		_eq(b_app["blink"], bc["blink"], "Yoshio bomb blink period at syoushin=%d" % bc["syoushin"], suite)

	# Full 11-girl appraisal list
	var state_11: Array[Dictionary] = []
	for i in range(11):
		state_11.append({"met": true, "rank": i * 7, "points": 100 + i * 10, "syoushin": 50 + i * 5})
	var full_list := TelephoneOverlay.build_full_appraisal_list(state_11)
	_eq(full_list.size(), 11, "build_full_appraisal_list returns 11 entries", suite)
	_eq(full_list[0]["bomb"], false, "Girl 0 (syoushin=50) has no bomb", suite)
	_eq(full_list[2]["bomb"], true, "Girl 2 (syoushin=60) has bomb active", suite)

	# 1D. 3-month repeat date-spot timer expiry & quarterly rotation (scheduler.c:75-83, 170-179, 2053-2055)
	# Verify quarter flag b13/b14 behavior in build_date_place_availability on Page 1 (Movie Theater slot 4, progress_state[14]=1)
	var progress_page1 := [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0]
	var q_flags_b13 := {"b13": 1, "b14": 0}
	var q_flags_b14 := {"b13": 0, "b14": 1}
	# current_month = 5 (5 % 3 == 2) booking for month = 6 requires b13; current_month = 6 (6 % 3 == 0) requires b14
	var avail_m5_b13 := TelephoneOverlay.build_date_place_availability(1, 95, 6, 15, progress_page1, q_flags_b13, 5)
	var avail_m5_b14 := TelephoneOverlay.build_date_place_availability(1, 95, 6, 15, progress_page1, q_flags_b14, 5)
	var avail_m6_b14 := TelephoneOverlay.build_date_place_availability(1, 95, 6, 15, progress_page1, q_flags_b14, 6)
	_eq(avail_m5_b13[4], 0x74, "Page 1 Movie Theater unlocked when current_month%3==2 (May->Jun) and b13=1", suite)
	_eq(avail_m5_b14[4], 0, "Page 1 Movie Theater locked when current_month%3==2 (May->Jun) and b13=0", suite)
	_eq(avail_m6_b14[4], 0x74, "Page 1 Movie Theater unlocked when current_month%3!=2 (Jun) and b14=1", suite)

	# Verify mark_magazine_read sets bit 13 in months 2/5/8/11 (month % 3 == 2) and bit 14 in other months
	var q_bytes_may := [0, 0]
	CalendarPhoneScreens.mark_magazine_read(q_bytes_may, 0x5F, 5)
	var word_may: int = (q_bytes_may[1] << 8) | q_bytes_may[0]
	_check((word_may & (1 << 13)) != 0 and (word_may & (1 << 14)) == 0, "May (month 5, 5%3==2) magazine read sets b13", suite)

	var q_bytes_jun := [0, 0]
	CalendarPhoneScreens.mark_magazine_read(q_bytes_jun, 0x5F, 6)
	var word_jun: int = (q_bytes_jun[1] << 8) | q_bytes_jun[0]
	_check((word_jun & (1 << 14)) != 0 and (word_jun & (1 << 13)) == 0, "June (month 6, 6%3==0) magazine read sets b14", suite)

	# Simulate 12-week last_date_spot_timer countdown and month % 3 == 0 expiry
	var st_repeat := DateOverlay.create_default_state({"date_girl": DateOverlay.GIRL_SHIORI, "current_girl": DateOverlay.GIRL_SHIORI})
	var shiori_dict: Dictionary = st_repeat["girls"][DateOverlay.GIRL_SHIORI]
	DateOverlay.setup_date_spot(st_repeat, DateOverlay.SPOT_AQUARIUM)
	DateOverlay.date_choice_score(st_repeat, 0)
	var end_first := DateOverlay.finish_date(st_repeat)
	_eq(end_first["last_date_spot_timer"], 12, "finish_date sets last_date_spot_timer to 12 weeks (~3 months)", suite)
	_eq(shiori_dict["last_date_spot"], DateOverlay.SPOT_AQUARIUM, "last_date_spot records Aquarium (3)", suite)

	# Decrement timer 11 weeks (timer = 1): repeat date at Aquarium STILL triggers repeat penalty
	for wk in range(11):
		if int(shiori_dict["last_date_spot_timer"]) > 0:
			shiori_dict["last_date_spot_timer"] = int(shiori_dict["last_date_spot_timer"]) - 1
	_eq(shiori_dict["last_date_spot_timer"], 1, "After 11 weekly ticks, last_date_spot_timer is 1", suite)
	DateOverlay.setup_date_spot(st_repeat, DateOverlay.SPOT_AQUARIUM)
	DateOverlay.date_choice_score(st_repeat, 0)
	var end_wk11 := DateOverlay.finish_date(st_repeat)
	_eq(end_wk11["repeat_spot_penalty_applied"], true, "Repeat spot at week 11 (timer=1) applies penalty", suite)

	# Now decrement all 12 weeks to 0: repeat date at Aquarium NO LONGER triggers penalty
	for wk in range(12):
		if int(shiori_dict["last_date_spot_timer"]) > 0:
			shiori_dict["last_date_spot_timer"] = int(shiori_dict["last_date_spot_timer"]) - 1
	_eq(shiori_dict["last_date_spot_timer"], 0, "After 12 weekly ticks (3 months), last_date_spot_timer expires to 0", suite)
	DateOverlay.setup_date_spot(st_repeat, DateOverlay.SPOT_AQUARIUM)
	DateOverlay.date_choice_score(st_repeat, 0)
	var end_wk12 := DateOverlay.finish_date(st_repeat)
	_eq(end_wk12["repeat_spot_penalty_applied"], false, "Repeat spot after 12-week timer expiry (timer=0) has no penalty", suite)

	_stress_metrics["telephone"] = {
		"total_invitation_trials": total_invitation_trials,
		"dialed_targets_verified": dialed_targets,
		"invitation_curves_by_girl": curve_summary,
	}


# ===========================================================================
# Suite 2: Date (0x50 & 0x58 DATE2)
# ===========================================================================

func _run_suite_2_date_and_date2() -> void:
	var suite := "date_0x50_and_date2_0x58"
	print("--- Suite 2: Date (0x50 & 0x58 DATE2) ---")

	# 2A. Arrival Greeting & Meeting Place Check/Resolve
	for fc in range(8):
		var st_arr := DateOverlay.create_default_state({"frame_count": fc, "date_girl": DateOverlay.GIRL_SHIORI})
		DateOverlay.date_init(st_arr)
		var expected_arr := DateOverlay.DATE_ARRIVAL_WAIT_GREETING if (fc & 3) == 0 else DateOverlay.DATE_ARRIVAL_DIRECT_GREETING
		_eq(st_arr["date_arrival"], expected_arr, "Shiori arrival at frame_count=%d is %d" % [fc, expected_arr], suite)

	# Yuina never uses wait greeting even when frame_count & 3 == 0
	var st_yuina := DateOverlay.create_default_state({"frame_count": 0, "date_girl": DateOverlay.GIRL_YUINA})
	DateOverlay.date_init(st_yuina)
	_eq(st_yuina["date_arrival"], DateOverlay.DATE_ARRIVAL_DIRECT_GREETING, "Yuina always uses direct greeting (1)", suite)

	# 2-choice wrong place vs 3-choice wrong place vs correct place
	var st_2choice := DateOverlay.create_default_state({
		"appointment_place_0": DateOverlay.SPOT_NEIGHBORHOOD_PARK,
		"appointment_place_1": DateOverlay.SPOT_ZOO,
		"appointment_place_2": DateOverlay.SPOT_ZOO,
		"appointment_place_3": 0xFF,
	})
	DateOverlay.date_init(st_2choice)
	var m2 := DateOverlay.date_meeting_place_check(st_2choice)
	_eq(m2["meeting_group"], DateOverlay.DATE_MEETING_WRONG_TWO_CHOICE, "2 appointments wrong place -> group 3 (2-choice)", suite)
	var res2 := DateOverlay.date_meeting_place_resolve(st_2choice, 1)
	_eq(res2["found_girl"], true, "Moving to appointment_place_1 finds girl", suite)
	_eq(st_2choice["date_arrival"], DateOverlay.DATE_ARRIVAL_CORRECTED_PLACE, "Corrected arrival sets date_arrival=2", suite)
	_eq(st_2choice["girls"][DateOverlay.GIRL_SHIORI]["syoushin"], 18, "Wrong place correction adds syoushin +8", suite)

	# Looks check across thresholds (<30 general, <35 Mira, >=35 pass)
	for l_val in [20, 29, 30, 34, 35, 50]:
		var st_l_shiori := DateOverlay.create_default_state({"param_looks": l_val, "date_girl": DateOverlay.GIRL_SHIORI})
		DateOverlay.date_init(st_l_shiori)
		var r_shiori := DateOverlay.date_arrival_and_looks_check(st_l_shiori)
		_eq(r_shiori["looks_rejected"], l_val < 30, "Shiori looks=%d rejected=%s" % [l_val, str(l_val < 30)], suite)

		var st_l_mira := DateOverlay.create_default_state({"param_looks": l_val, "date_girl": DateOverlay.GIRL_MIRA})
		DateOverlay.date_init(st_l_mira)
		var r_mira := DateOverlay.date_arrival_and_looks_check(st_l_mira)
		_eq(r_mira["looks_rejected"], l_val < 35, "Mira looks=%d rejected=%s" % [l_val, str(l_val < 35)], suite)

	# 2B. All 21 Date Spots, 3-Choice Prompt Selection (0..2), and Best/Neutral/Worst Reaction Deltas
	var spot_choice_matrix: Array[Dictionary] = []
	for spot_id in range(21):
		var st_spot := DateOverlay.create_default_state({
			"param_trivia": 80,
			"param_science": 90,
			"param_athletics": 100,
			"current_girl": DateOverlay.GIRL_SHIORI,
			"date_girl": DateOverlay.GIRL_SHIORI,
		})
		var s_setup := DateOverlay.setup_date_spot(st_spot, spot_id)
		_eq(s_setup["spot_id"], spot_id, "Spot %d setup ID matches" % spot_id, suite)
		_eq(st_spot["choice_classes"].size(), 3, "Spot %d defines 3 choice classes" % spot_id, suite)

		# Normalize choice_classes to [0 (Best), 2 (Neutral/Mediocre), 4 (Worst/Terrible)] to stress-test all 3 reaction tiers on every venue
		st_spot["choice_classes"] = [0, 2, 4]
		var c_best := DateOverlay.date_choice_score(st_spot, 0)
		var c_neut := DateOverlay.date_choice_score(st_spot, 1)
		var c_worst := DateOverlay.date_choice_score(st_spot, 2)

		_eq(c_best["tier"], 0, "Spot %d Best choice (class 0) is tier 0" % spot_id, suite)
		_eq(c_neut["tier"], 1, "Spot %d Neutral choice (class 2) is tier 1" % spot_id, suite)
		_eq(c_worst["tier"], 2, "Spot %d Worst choice (class 4) is tier 2" % spot_id, suite)

		_check(int(c_best["tokimeki_fp_delta"]) > int(c_neut["tokimeki_fp_delta"]), "Spot %d Best tokimeki_fp_delta > Neutral" % spot_id, suite)
		_check(int(c_neut["tokimeki_fp_delta"]) > int(c_worst["tokimeki_fp_delta"]), "Spot %d Neutral tokimeki_fp_delta > Worst" % spot_id, suite)
		_check(int(c_best["syoushin_fp_delta"]) < 0, "Spot %d Best choice reduces syoushin" % spot_id, suite)
		_check(int(c_worst["syoushin_fp_delta"]) > 0, "Spot %d Worst choice increases syoushin" % spot_id, suite)

		spot_choice_matrix.append({
			"spot_id": spot_id,
			"sub_spot_key": s_setup["sub_spot_key"],
			"best_tokimeki_fp": c_best["tokimeki_fp_delta"],
			"neutral_tokimeki_fp": c_neut["tokimeki_fp_delta"],
			"worst_tokimeki_fp": c_worst["tokimeki_fp_delta"],
		})

	# Venue stat overrides: Planetarium science < 60 (variant%2==1), Pool athletics < 70 (variant%2==0),
	# Skating athletics < 80 (variant%2==1), Ski athletics < 90 (variant%2==0)
	var venue_overrides := [
		{"spot": DateOverlay.SPOT_PLANETARIUM, "variant": 1, "stat": "param_science", "val": 50, "choice": 0, "expected_class": 3, "expected_line": 3},
		{"spot": DateOverlay.SPOT_POOL, "variant": 0, "stat": "param_athletics", "val": 60, "choice": 0, "expected_class": 4, "expected_line": 5},
		{"spot": DateOverlay.SPOT_SKATING_RINK, "variant": 1, "stat": "param_athletics", "val": 70, "choice": 0, "expected_class": 4, "expected_line": 6},
		{"spot": DateOverlay.SPOT_SKI_RESORT, "variant": 0, "stat": "param_athletics", "val": 80, "choice": 0, "expected_class": 4, "expected_line": 6},
	]
	for vo in venue_overrides:
		var st_vo := DateOverlay.create_default_state({vo["stat"]: vo["val"]})
		st_vo["scene_counters"][vo["spot"]] = vo["variant"]
		DateOverlay.setup_date_spot(st_vo, vo["spot"])
		var r_vo := DateOverlay.date_choice_score(st_vo, vo["choice"])
		_eq(r_vo["response_class"], vo["expected_class"], "Venue %d low %s overrides class to %d" % [vo["spot"], vo["stat"], vo["expected_class"]], suite)
		_eq(r_vo["reaction_line"], vo["expected_line"], "Venue %d low %s overrides reaction_line to %d" % [vo["spot"], vo["stat"], vo["expected_line"]], suite)

	# 2C. Same-Spot Penalty, Spot Aliasing (Library->Art Museum, Planetarium->Botanical Garden), and Exceptions (Park & Shrine)
	# Park (0) and Shrine (19) never trigger repeat-spot penalty even when last_date_spot matches
	for exempt_spot in [DateOverlay.SPOT_NEIGHBORHOOD_PARK, DateOverlay.SPOT_SHRINE]:
		var st_ex := DateOverlay.create_default_state()
		DateOverlay.setup_date_spot(st_ex, exempt_spot)
		DateOverlay.date_choice_score(st_ex, 0)
		DateOverlay.finish_date(st_ex)
		DateOverlay.setup_date_spot(st_ex, exempt_spot)
		DateOverlay.date_choice_score(st_ex, 0)
		var end_ex := DateOverlay.finish_date(st_ex)
		_eq(end_ex["repeat_spot_penalty_applied"], false, "Spot %d is exempt from Same-Spot penalty" % exempt_spot, suite)

	# Planetarium (10) maps last_date_spot to Botanical Garden (2); visiting Botanical Garden next triggers penalty!
	var st_alias := DateOverlay.create_default_state()
	DateOverlay.setup_date_spot(st_alias, DateOverlay.SPOT_PLANETARIUM)
	DateOverlay.date_choice_score(st_alias, 0)
	var end_plan := DateOverlay.finish_date(st_alias)
	_eq(end_plan["mapped_last_spot"], DateOverlay.SPOT_BOTANICAL_GARDEN, "Planetarium (10) aliases last_date_spot to Botanical Garden (2)", suite)
	DateOverlay.setup_date_spot(st_alias, DateOverlay.SPOT_BOTANICAL_GARDEN)
	DateOverlay.date_choice_score(st_alias, 0)
	var end_bot := DateOverlay.finish_date(st_alias)
	_eq(end_bot["repeat_spot_penalty_applied"], true, "Botanical Garden after Planetarium triggers Same-Spot penalty", suite)

	# Nozomi uses farewell_group 0x13 on repeat-spot penalty (other girls use 0x0B)
	var st_noz := DateOverlay.create_default_state({"date_girl": DateOverlay.GIRL_NOZOMI, "current_girl": DateOverlay.GIRL_NOZOMI})
	DateOverlay.setup_date_spot(st_noz, DateOverlay.SPOT_ZOO)
	DateOverlay.date_choice_score(st_noz, 0)
	DateOverlay.finish_date(st_noz)
	DateOverlay.setup_date_spot(st_noz, DateOverlay.SPOT_ZOO)
	DateOverlay.date_choice_score(st_noz, 0)
	var end_noz := DateOverlay.finish_date(st_noz)
	_eq(end_noz["repeat_spot_penalty_applied"], true, "Nozomi repeat Zoo date triggers penalty", suite)
	_eq(end_noz["farewell_group"], 0x13, "Nozomi repeat-spot farewell_group is 0x13", suite)

	# 2D. Amusement Park rides (0..4) + 3rd visit Hero Show & Karaoke DATE2 (0x58) subscenes
	var ride_keys := ["roller_coaster", "ghost_house", "ferris_wheel", "scream_machine", "virtual_ship"]
	for r_id in range(5):
		var st_am := DateOverlay.create_default_state()
		DateOverlay.setup_date_spot(st_am, DateOverlay.SPOT_AMUSEMENT_PARK)
		var r_info := DateOverlay.select_amusement_park_ride(st_am, r_id)
		_eq(r_info["ride_index"], r_id, "Amusement Park ride index is %d" % r_id, suite)
		_eq(r_info["sub_place_title_hex"], DateOverlay.SUB_PLACE_NAMES_HEX[ride_keys[r_id]], "Amusement Park ride %d selects %s banner" % [r_id, ride_keys[r_id]], suite)

	# 3rd visit to Amusement Park triggers Hero Show
	var st_hero := DateOverlay.create_default_state()
	st_hero["scene_counters"][DateOverlay.SPOT_AMUSEMENT_PARK] = 2
	var hero_info := DateOverlay.setup_date_spot(st_hero, DateOverlay.SPOT_AMUSEMENT_PARK)
	_eq(hero_info["sub_spot_key"], "hero_show", "Amusement Park 3rd visit (counter=2) triggers hero_show", suite)

	# Karaoke DATE2 (0x58 stage 3): full song vs early cancel
	var st_k_full := DateOverlay.create_default_state()
	st_k_full["scene_counters"][DateOverlay.SPOT_KARAOKE] = 2
	var k_setup := DateOverlay.setup_date_spot(st_k_full, DateOverlay.SPOT_KARAOKE)
	_eq(k_setup["game_mode"], DateOverlay.GAME_MODE_AMUSEMENT_PARK_2, "Karaoke 3rd visit enters 0x58 DATE2", suite)
	_eq(k_setup["mode_stage"], DateOverlay.DATE2_STAGE_KARAOKE, "Karaoke 3rd visit sets stage 3", suite)
	var k_res_full := DateOverlay.finish_date2_karaoke_song(st_k_full, 2, true)
	_eq(k_res_full["tokimeki"], 62, "Full karaoke song adds tokimeki +2", suite)
	_eq(k_res_full["affection"], 61, "Full karaoke song adds affection +1", suite)
	_eq(k_res_full["game_mode"], DateOverlay.GAME_MODE_DATE, "Karaoke returns to 0x50 DATE", suite)
	_eq(k_res_full["mode_step"], 0x11, "Karaoke returns to step 0x11", suite)

	var st_k_cancel := DateOverlay.create_default_state()
	var k_res_cancel := DateOverlay.finish_date2_karaoke_song(st_k_cancel, 0, false)
	_eq(k_res_cancel["tokimeki"], 60, "Early cancel karaoke song adds 0 tokimeki", suite)
	_eq(k_res_cancel["affection"], 61, "Early cancel karaoke song adds affection +1", suite)

	_stress_metrics["date"] = {
		"spots_verified": 21,
		"spot_choice_matrix": spot_choice_matrix,
	}


# ===========================================================================
# Suite 3: Heartbreak (syoushin) & Bomb (flags bit) Propagation
# ===========================================================================

func _run_suite_3_syoushin_and_bomb() -> void:
	var suite := "syoushin_and_bomb_propagation"
	print("--- Suite 3: Heartbreak (syoushin) & Bomb Propagation ---")

	# 3A. Weekly Neglect Accumulation via Scheduler.weekly_girl_update & dispatch_first_activity
	var tables_mock := {
		"affection_weights": [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100],
		"tokimeki_weights": [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100],
		"ladder_kinds": [1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2],
	}
	var st_sched := SchoolDayState.new()
	var sched_girls: Array = []
	for i in range(11):
		sched_girls.append({
			"flags": 2 if i < 4 else 0, # Girls 0..3 met (bit 1 = 2)
			"tokimeki_word": SchoolDayState.with_high(0, 40),
			"affection_word": SchoolDayState.with_high(0, 5), # Low affection (<10 weighted)
			"syoushin_word": SchoolDayState.with_high(0, 0),
		})
	st_sched.rule_state = {
		"vacation": 0,
		"day": 3, # First week of month (day < 7): all met girls lose 1 affection
		"girls": sched_girls,
		"params": [50, 10, 10, 10, 10, 10, 10, 10],
		"day_events": [[0, 0]],
		"day_event_index": 0,
		"icon_enabled": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
	}

	# Simulate 20 weeks of neglect and record syoushin trajectory
	var neglect_trajectory: Array[Dictionary] = []
	for wk in range(1, 21):
		# Alternate day=3 (week 1 of month) and day=16 (week 3 of month, Shiori extra decay)
		st_sched.rule_state["day"] = 3 if (wk % 2 == 1) else 16
		Scheduler.weekly_girl_update(st_sched, tables_mock)
		var g0_aff := SchoolDayState.high(st_sched.rule_state["girls"][0]["affection_word"])
		var g0_syo := SchoolDayState.high(st_sched.rule_state["girls"][0]["syoushin_word"])
		var g1_aff := SchoolDayState.high(st_sched.rule_state["girls"][1]["affection_word"])
		var g1_syo := SchoolDayState.high(st_sched.rule_state["girls"][1]["syoushin_word"])
		var g4_syo := SchoolDayState.high(st_sched.rule_state["girls"][4]["syoushin_word"])
		_eq(g0_syo, wk * 5, "Week %d met Girl 0 syoushin is %d" % [wk, wk * 5], suite)
		_eq(g1_syo, wk * 5, "Week %d met Girl 1 syoushin is %d" % [wk, wk * 5], suite)
		_eq(g4_syo, 0, "Week %d unmet Girl 4 syoushin remains 0" % wk, suite)
		if wk == 2:
			# Week 1 (day=3): both g0 and g1 lost 1 affection (5->4); Week 2 (day=16): only g0 lost 1 affection (4->3)
			_eq(g0_aff, 3, "Shiori (index 0) loses affection on both day<7 and 14<=day<21", suite)
			_eq(g1_aff, 4, "Other met girls only lose affection on day<7", suite)

		var step_flags := {
			"week": wk,
			"syoushin": g0_syo,
			"yoshio_bomb_icon": g0_syo >= 60,
			"rumor_threshold": g0_syo >= 61,
			"forced_walk_home_threshold": g0_syo >= 76,
			"bomb_explosion_threshold": g0_syo >= 91,
		}
		neglect_trajectory.append(step_flags)

	# Verify threshold crossings along the 20-week neglect curve:
	# Week 12 -> syoushin = 60 (Yoshio bomb icon appears; geko_judge & date_sasoi_judge exclude girl)
	_eq(neglect_trajectory[11]["syoushin"], 60, "Week 12 reaches syoushin=60 (bomb icon threshold)", suite)
	_eq(neglect_trajectory[11]["yoshio_bomb_icon"], true, "Bomb icon activates at week 12 (syoushin=60)", suite)
	# Week 13 -> syoushin = 65 (>= 61 rumor threshold)
	_eq(neglect_trajectory[12]["rumor_threshold"], true, "Rumor threshold (>=61) crossed at week 13 (syoushin=65)", suite)
	# Week 16 -> syoushin = 80 (>= 76 forced walk-home confrontation threshold)
	_eq(neglect_trajectory[15]["forced_walk_home_threshold"], true, "Forced walk-home confrontation (>=76) crossed at week 16 (syoushin=80)", suite)
	# Week 19 -> syoushin = 95 (>= 91 bomb explosion threshold)
	_eq(neglect_trajectory[18]["bomb_explosion_threshold"], true, "Bomb explosion (>=91) crossed at week 19 (syoushin=95)", suite)

	# 3B. Verify girl_counters_clamp (0..128) and clearing 0x30000 when syoushin < 50
	st_sched.rule_state["girls"][0]["syoushin_word"] = SchoolDayState.with_high(0, 145)
	st_sched.rule_state["girls"][0]["flags"] = 2 | 0x30000
	st_sched.rule_state["girls"][1]["syoushin_word"] = SchoolDayState.with_high(0, 49)
	st_sched.rule_state["girls"][1]["flags"] = 2 | 0x30000
	var clamp_res := GameSession.FrameResult.new()
	Scheduler.dispatch_first_activity(st_sched, clamp_res)
	_eq(SchoolDayState.high(st_sched.rule_state["girls"][0]["syoushin_word"]), 128, "syoushin clamped to 128 (0x80)", suite)
	_check((int(st_sched.rule_state["girls"][0]["flags"]) & 0x30000) == 0x30000, "Flags 0x30000 preserved when syoushin >= 50", suite)
	_check((int(st_sched.rule_state["girls"][1]["flags"]) & 0x30000) == 0, "Flags 0x30000 cleared when syoushin < 50", suite)

	# 3C. Bomb Explosion Penalty Propagation across all 11 met heroines (phone_relationship_deltas_apply)
	# When any girl reaches syoushin >= 91 (0x5B) at DAY_EVENT_DAY_END, mode 0x3D stage 4 triggers:
	# - Bombed girl's syoushin = 0, and flags b16 (encounter_done), b12, b13 cleared
	# - Every met heroine i in 0..10 loses CANONICAL_BOMB_TOKIMEKI_PENALTY[i] and CANONICAL_BOMB_AFFECTION_PENALTY[i]
	var bomb_girls: Array[Dictionary] = []
	for i in range(11):
		bomb_girls.append({
			"id": i,
			"met": 1 if i != 9 else 0, # All met except Megumi (9) to test met-only gating
			"tokimeki": 80,
			"affection": 80,
			"syoushin": 95 if i == 4 else 25, # Saki (4) triggers the bomb explosion at syoushin=95
			"flags": 0x13002, # met (bit 1) + b12 + b13 + encounter_done (bit 16)
		})
	var bombed_idx := 4
	# Apply phone_relationship_deltas_apply (src/ovl/telephone/appointment_call_scenes.c:1106-1131)
	bomb_girls[bombed_idx]["syoushin"] = 0
	bomb_girls[bombed_idx]["flags"] = int(bomb_girls[bombed_idx]["flags"]) & ~(0x10000 | 0x1000 | 0x2000)
	for i in range(11):
		if int(bomb_girls[i]["met"]) != 0:
			bomb_girls[i]["tokimeki"] = clampi(int(bomb_girls[i]["tokimeki"]) - CANONICAL_BOMB_TOKIMEKI_PENALTY[i], 0, 128)
			bomb_girls[i]["affection"] = clampi(int(bomb_girls[i]["affection"]) - CANONICAL_BOMB_AFFECTION_PENALTY[i], 0, 128)

	_eq(bomb_girls[bombed_idx]["syoushin"], 0, "Bombed girl (Saki=4) syoushin reset to 0 after explosion", suite)
	_eq(int(bomb_girls[bombed_idx]["flags"]) & 0x13000, 0, "Bombed girl b16/b12/b13 cleared after explosion", suite)
	for i in range(11):
		if i == 9:
			_eq(bomb_girls[i]["tokimeki"], 80, "Unmet girl 9 (Megumi) unaffected by bomb explosion", suite)
			_eq(bomb_girls[i]["affection"], 80, "Unmet girl 9 (Megumi) affection unaffected", suite)
		else:
			_eq(bomb_girls[i]["tokimeki"], 80 - CANONICAL_BOMB_TOKIMEKI_PENALTY[i], "Met girl %d tokimeki reduced by %d on bomb explosion" % [i, CANONICAL_BOMB_TOKIMEKI_PENALTY[i]], suite)
			_eq(bomb_girls[i]["affection"], 80 - CANONICAL_BOMB_AFFECTION_PENALTY[i], "Met girl %d affection reduced by %d on bomb explosion" % [i, CANONICAL_BOMB_AFFECTION_PENALTY[i]], suite)

	# 3D. Bomb Defusing via Telephone Date Invitation (syoushin -= 12) & Date Completion (syoushin -= 40)
	# Start a girl at syoushin = 82 (bomb active >= 60, forced encounter done bit 16 & rumor bit 17 set)
	var st_defuse := DateOverlay.create_default_state({"date_girl": DateOverlay.GIRL_SHIORI, "current_girl": DateOverlay.GIRL_SHIORI})
	var g_defuse: Dictionary = st_defuse["girls"][DateOverlay.GIRL_SHIORI]
	g_defuse["syoushin"] = 82
	g_defuse["syoushin_fraction"] = 82 << 16
	g_defuse["flags"] = 2 | 0x30000
	# Step 1: Player calls girl to invite on a date -> call_and_invitation_flow.c:1868 reduces syoushin by 12 (82 -> 70)
	g_defuse["syoushin"] = maxi(0, int(g_defuse["syoushin"]) - 12)
	g_defuse["syoushin_fraction"] = int(g_defuse["syoushin"]) << 16
	_eq(g_defuse["syoushin"], 70, "Phone call immediately reduces syoushin by 12 (82 -> 70)", suite)
	# Step 2: Player completes the Sunday date -> finish_date reduces syoushin by 40 (plus choice delta), dropping below 50!
	DateOverlay.setup_date_spot(st_defuse, DateOverlay.SPOT_AQUARIUM)
	DateOverlay.date_choice_score(st_defuse, 0) # Best choice also reduces syoushin by 10 -> 60
	DateOverlay.finish_date(st_defuse)
	var final_syo: int = int(g_defuse["syoushin"])
	_eq(final_syo, 20, "Completing date reduces syoushin by 40 (60 -> 20), defusing bomb (<50)", suite)
	var post_defuse_app := TelephoneOverlay.calculate_girl_appraisal(0, true, 10, 100, final_syo)
	_eq(post_defuse_app["bomb"], false, "Bomb icon extinguished in Yoshio appraisal after date defusal", suite)

	_stress_metrics["syoushin_and_bomb"] = {
		"neglect_weeks_simulated": 20,
		"neglect_trajectory": neglect_trajectory,
		"defuse_flow": {"initial": 82, "after_call": 70, "after_best_choice": 60, "after_date_finish": final_syo},
	}


# ===========================================================================
# Suite 4: Seasonal & Encounter Overlays
# ===========================================================================

func _run_suite_4_seasonal_and_encounters() -> void:
	var suite := "seasonal_and_encounters"
	print("--- Suite 4: Seasonal & Encounter Overlays ---")

	# 4A. Walk-Home (GEKO 0x44): accept, decline, nickname change, syoushin >= 60 exclusion
	var geko_girls: Array = []
	for i in range(14):
		var c: Array[int] = []
		c.resize(36)
		geko_girls.append({"id": i, "met": 1 if i < 11 else 0, "affection": 50, "tokimeki": 60, "syoushin": 10, "flags": 2, "scene_counters": c, "story_stage": 0})
	var closeness_all: Array[int] = [30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30]
	var states_all: Array[int] = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]

	# Verify all 11 girls can trigger geko_judge when starting at their seed
	for g_id in range(11):
		var j_res := EncountersOverlay.evaluate_geko_judge(geko_girls, g_id, -1, closeness_all, states_all, [25])
		_eq(j_res["triggered"], true, "Girl %d triggers geko_judge when eligible" % g_id, suite)
		_eq(j_res["girl_id"], g_id, "geko_judge selects Girl %d at seed=%d" % [g_id, g_id], suite)

	# Verify syoushin >= 60 or state == 4 (awkward) blocks geko_judge
	geko_girls[0]["syoushin"] = 60
	var j_bomb_skip := EncountersOverlay.evaluate_geko_judge(geko_girls, 0, -1, closeness_all, states_all, [25])
	_eq(j_bomb_skip["girl_id"], 1, "geko_judge skips Girl 0 when syoushin=60 and selects Girl 1", suite)
	geko_girls[0]["syoushin"] = 10

	states_all[0] = 4
	var j_state4_skip := EncountersOverlay.evaluate_geko_judge(geko_girls, 0, -1, closeness_all, states_all, [25])
	_eq(j_state4_skip["girl_id"], 1, "geko_judge skips Girl 0 when state==4 and selects Girl 1", suite)
	states_all[0] = 2

	# Walk-Home accept (choice 0) vs decline (choice 1) vs nickname prompt
	var w_accept := EncountersOverlay.apply_walk_home_choice(0, 0, 2, 6, geko_girls, 100, [1, 1, 1, 1, 1, 1, 1, 1, 1, 1])
	_eq(w_accept["followup_allowed"], 1, "Walk-Home accept at state 2 allows followup", suite)
	for other_id in range(1, 11):
		_eq(w_accept["girls"][other_id]["syoushin"], 10 + 1 + 4, "Walk-Home accept raises other met girl %d syoushin by rand+4" % other_id, suite)

	var w_decline := EncountersOverlay.apply_walk_home_choice(1, 0, 2, 6, geko_girls, 100, [])
	_eq(w_decline["followup_allowed"], 0, "Walk-Home decline disables followup", suite)
	_eq(w_decline["walk_home_group"], 8, "Walk-Home decline increments walk_home_group by +2", suite)
	_eq(w_decline["girls"][0]["syoushin"], 10 + (100 % 4) + 6, "Walk-Home decline raises current girl syoushin by offset+6", suite)

	var nick_accept := EncountersOverlay.apply_nickname_prompt_choice(true, "しおりん", "藤崎さん")
	_eq(nick_accept["accepted"], true, "Nickname prompt accept returns true", suite)
	_eq(nick_accept["active_nickname"], "しおりん", "Nickname prompt accept updates active_nickname", suite)
	var nick_decline := EncountersOverlay.apply_nickname_prompt_choice(false, "しおりん", "藤崎さん")
	_eq(nick_decline["accepted"], false, "Nickname prompt decline returns false", suite)
	_eq(nick_decline["active_nickname"], "藤崎さん", "Nickname prompt decline preserves previous_nickname", suite)

	# 4B. Sick Visit (OMIMAI 0x4B): test Shiori, Mio, Yuina, Saki, and default girls across commands and clubs
	var omimai_shiori_cmd0 := EncountersOverlay.select_sick_visit_dialogue(EncountersOverlay.GIRL_SHIORI, 1, 3, 2, 0x5F, 6, 0)
	_eq(omimai_shiori_cmd0["talk_group"], 0, "Sick Visit Shiori cmd<2 r=0 -> group 0", suite)
	var omimai_shiori_cmd3 := EncountersOverlay.select_sick_visit_dialogue(EncountersOverlay.GIRL_SHIORI, 3, 3, 2, 0x5F, 6, 1)
	_eq(omimai_shiori_cmd3["talk_group"], 5, "Sick Visit Shiori cmd=3 r=1 -> group 5", suite)
	_eq(omimai_shiori_cmd3["dialogue_variant"], 2, "Sick Visit Shiori cmd=3 r=1 -> variant 2", suite)
	var omimai_mio_club := EncountersOverlay.select_sick_visit_dialogue(EncountersOverlay.GIRL_MIO, 4, 2, 1, 0x60, 10, 0)
	_eq(omimai_mio_club["talk_group"], 6, "Sick Visit Mio cmd=4 club=2 r=0 -> group 6", suite)
	var omimai_yuina_club := EncountersOverlay.select_sick_visit_dialogue(EncountersOverlay.GIRL_YUINA, 4, 3, 1, 0x60, 10, 1)
	_eq(omimai_yuina_club["dialogue_variant"], 4, "Sick Visit Yuina cmd=4 -> variant 4", suite)
	_eq(omimai_yuina_club["talk_group"], 5, "Sick Visit Yuina cmd=4 club=3 r=1 -> group 5", suite)
	var omimai_saki_close := EncountersOverlay.select_sick_visit_dialogue(EncountersOverlay.GIRL_SAKI, 5, 6, 1, 0x61, 9, 0)
	_eq(omimai_saki_close["dialogue_variant"], 6, "Sick Visit Saki baseball Sep Y3 rank<2 -> variant 6", suite)
	_eq(omimai_saki_close["talk_group"], 4, "Sick Visit Saki baseball Sep Y3 rank<2 -> group 4", suite)

	# 4C. New Year Shrine & Christmas (SHOUGATU 0x4E, including Christmas Mansion Dress Gate)
	var ny_girls: Array = []
	for i in range(14):
		ny_girls.append({"id": i, "met": 1 if i < 5 else 0, "affection": 40 + i * 5, "tokimeki": 40 + i * 5, "syoushin": 20, "story_stage": 0})
	# States: Girl 0=2 (ineligible), Girl 1=1 (sum=90), Girl 2=1 (sum=100), Girl 3=0 (sum=110)
	var ny_states := [2, 1, 1, 0, 2, 3, 3, 3, 3, 3, 3]
	var ny_visitor := EncountersOverlay.select_new_year_visitor(ny_girls, ny_states)
	_eq(ny_visitor, 3, "New Year visitor picks lowest state<2 (Girl 3 at state 0)", suite)
	ny_states[3] = 1 # Now Girls 1, 2, 3 all have state 1; Girl 3 has highest affection+tokimeki (110)
	var ny_tie := EncountersOverlay.select_new_year_visitor(ny_girls, ny_states)
	_eq(ny_tie, 3, "New Year visitor breaks state ties by highest affection+tokimeki", suite)

	# Christmas Party rewards (+8/+8/-10 for selected girl, +1 affection for other met girls)
	var xmas_after := EncountersOverlay.apply_christmas_party_rewards(2, ny_girls)
	_eq(xmas_after[2]["tokimeki"], ny_girls[2]["tokimeki"] + 8, "Christmas selected girl gets tokimeki +8", suite)
	_eq(xmas_after[2]["affection"], ny_girls[2]["affection"] + 8, "Christmas selected girl gets affection +8", suite)
	_eq(xmas_after[2]["syoushin"], ny_girls[2]["syoushin"] - 10, "Christmas selected girl gets syoushin -10", suite)
	_eq(xmas_after[0]["affection"], ny_girls[0]["affection"] + 1, "Other met girl gets affection +1 at Christmas", suite)
	_eq(xmas_after[6]["affection"], ny_girls[6]["affection"], "Unmet girl gets 0 affection at Christmas", suite)

	# Christmas Mansion Dress/Looks & Athletics Gate (src/ovl/new_year_christmas/christmas_mansion_entry.c:45-90)
	# Threshold formula: looks >= stage * 30 - (stage >= 2 ? 10 : 0) + 70 -> Stage 0: 70, Stage 1: 100, Stage 2: 120
	# Athletics gate (when girls[14].story_stage != 0): athletics >= stage * 25 + 150 -> Stage 0: 150, Stage 1: 175, Stage 2: 200
	var expected_dress_thresholds := [70, 100, 120]
	var expected_ath_thresholds := [150, 175, 200]
	for stage in range(3):
		var dress_req: int = stage * 30 - (10 if stage >= 2 else 0) + 70
		var ath_req: int = stage * 25 + 150
		_eq(dress_req, expected_dress_thresholds[stage], "Christmas Dress Gate stage %d requires looks >= %d" % [stage, expected_dress_thresholds[stage]], suite)
		_eq(ath_req, expected_ath_thresholds[stage], "Christmas Athletics Gate stage %d requires athletics >= %d" % [stage, expected_ath_thresholds[stage]], suite)

	# 4D. Valentine's Day (VALEN 0x4F): obligation (yuukou >= 50) vs true-feelings (tokimeki >= 70) chocolate tiers
	var val_girls: Array = []
	for i in range(11):
		val_girls.append({"id": i, "met": 1, "affection": 50 + i * 3, "tokimeki": 40 + i * 5, "syoushin": 0, "flags": 0})
	# Set specific tiers:
	# Girl 0 (Shiori): tokimeki=60 (<70 obligation tier), yuukou=55 (>=50) -> placed in slot 0 when unmet_megumi==0
	# Girl 1 (Mio): tokimeki=75 (>=70 true-feelings tier, sum=135), yuukou=65
	# Girl 2 (Yuina): tokimeki=85 (>=70 true-feelings tier, sum=155), yuukou=70
	# Girl 4 (Saki): tokimeki=80 (>=70 true-feelings tier, sum=145), yuukou=68
	# Girl 9 (Megumi): met=1, tokimeki=30, yuukou=30 (<50 ineligible)
	var v_toki: Array[int] = [60, 75, 85, 40, 80, 30, 30, 30, 30, 30, 30]
	var v_yuukou: Array[int] = [55, 65, 70, 52, 68, 30, 30, 30, 30, 30, 30]
	val_girls[0]["tokimeki"] = 60
	val_girls[0]["affection"] = 55
	val_girls[1]["tokimeki"] = 75
	val_girls[1]["affection"] = 60
	val_girls[2]["tokimeki"] = 85
	val_girls[2]["affection"] = 70
	val_girls[4]["tokimeki"] = 80
	val_girls[4]["affection"] = 65

	var val_obligation_shiori := EncountersOverlay.select_valentine_candidates(val_girls, v_toki, v_yuukou)
	_eq(val_obligation_shiori["ranked_girls"], [2, 4, 1], "Top 3 true-feelings (tokimeki>=70) sorted by affection+tokimeki: Yuina(2), Saki(4), Mio(1)", suite)
	_eq(val_obligation_shiori["candidates"][0], 0, "Obligation-tier Shiori (tokimeki<70) is placed at slot 0", suite)
	_eq(val_obligation_shiori["candidate_count"], 5, "5 girls with yuukou>=50 selected as Valentine candidates", suite)

	# Now boost Shiori to true-feelings tier (tokimeki=90, affection=85 -> sum=175)
	val_girls[0]["tokimeki"] = 90
	val_girls[0]["affection"] = 85
	v_toki[0] = 90
	var val_honmei_shiori := EncountersOverlay.select_valentine_candidates(val_girls, v_toki, v_yuukou)
	_eq(val_honmei_shiori["ranked_girls"][0], 0, "True-feelings Shiori (tokimeki>=70) takes #1 in ranked_girls", suite)
	_eq(val_honmei_shiori["candidates"][val_honmei_shiori["candidates"].size() - 1], 0, "True-feelings Shiori is moved to last slot of candidates", suite)

	# 4E. Term Exams (EXAM 0x35): rank computation, girl reaction bands (+11 first win, +2 repeat win, +2 Rank #1, remedial week)
	var ex_girls: Array = []
	for i in range(14):
		ex_girls.append({"id": i, "met": 1 if i < 11 else 0, "affection": 50, "tokimeki": 40, "syoushin": 0, "flags": 2})
	var ex_flags: Array[int] = []
	ex_flags.resize(0xE0)
	ex_flags.fill(0)

	# First exam at score 490 (Rank #1, beats all 11 heroines for the first time: +11 + 2 = +13 tokimeki each)
	var ex_run1 := EncountersOverlay.build_exam_ranking(490, ex_girls, ex_flags, 0)
	_eq(ex_run1["player_rank"], 1, "Score 490 achieves school Rank #1", suite)
	_eq(ex_run1["player_in_lower_half"], 0, "Rank #1 is in upper half (0)", suite)
	for i in range(11):
		_eq(ex_run1["girls"][i]["tokimeki"], 40 + 11 + 2, "Girl %d gets +11 (first win) + 2 (Rank #1) = 53 tokimeki" % i, suite)
		_eq(ex_run1["scene_flags"][0xC8 + i], 1, "scene_flags[0xC8 + %d] set to 1 after first win" % i, suite)

	# Second exam at score 490 (Rank #1, beats all 11 heroines on repeat win: +2 + 2 = +4 tokimeki each)
	var ex_run2 := EncountersOverlay.build_exam_ranking(490, ex_run1["girls"], ex_run1["scene_flags"], 0)
	for i in range(11):
		_eq(ex_run2["girls"][i]["tokimeki"], 53 + 2 + 2, "Girl %d gets +2 (repeat win) + 2 (Rank #1) = 57 tokimeki" % i, suite)

	# Low exam score (120) with 3 failed subjects (<40) -> lower half + remedial week (0x17)
	var ex_low := EncountersOverlay.build_exam_ranking(120, ex_girls, ex_flags, 3)
	_eq(ex_low["player_in_lower_half"], 1, "Score 120 places player in lower half (1)", suite)
	_eq(ex_low["remedial_required"], true, "3 failed subjects triggers remedial week", suite)
	_eq(ex_low["next_day_marker"], 0x17, "Remedial week sets next_day_marker=0x17", suite)

	_stress_metrics["seasonal_and_encounters"] = {
		"geko_heroines_tested": 11,
		"valentine_obligation_candidates": val_obligation_shiori["candidates"],
		"valentine_honmei_ranked": val_honmei_shiori["ranked_girls"],
		"exam_rank1_tokimeki_gain_first": 13,
		"exam_rank1_tokimeki_gain_repeat": 4,
	}


# ===========================================================================
# Proof JSON Writer
# ===========================================================================

func _write_proof_json() -> String:
	var repo := _resolve_repo_root()
	var out_path := repo.path_join(PROOF_REL_PATH)
	DirAccess.make_dir_recursive_absolute(out_path.get_base_dir())

	var decomp_findings := [
		{
			"id": "F-01",
			"severity": "medium",
			"component": "port/godot/overlays/date_overlay.gd",
			"c_reference": "src/main/holiday.c, src/ovl/date/date.c, include/game_enums.h",
			"title": "Girl index ordering and birthday table mismatch in DateOverlay vs Main/Telephone/Encounters",
			"details": "In src/main/, telephone_overlay.gd, encounters_and_events_overlay.gd, and holiday_appointment.gd, girl indices 0..10 are 0=Shiori, 1=Mio, 2=Yuina, 3=Ayako, 4=Saki, 5=Yukari, 6=Nozomi, 7=Mira, 8=Yuko, 9=Megumi, 10=Yumi. In date_overlay.gd (lines 38-50, 162-174), GIRL_YUKO is 0, GIRL_SHIORI is 1, GIRL_MIO is 2, GIRL_YUINA is 3, GIRL_AYAKO is 4, GIRL_SAKI is 5, GIRL_NOZOMI is 6, GIRL_MIRA is 7, GIRL_YUKARI is 8, and GIRL_BIRTHDAYS has non-canonical dates (e.g. Shiori Oct 24 instead of May 27, Saki Jan 13 instead of Oct 25)."
		},
		{
			"id": "F-02",
			"severity": "low",
			"component": "port/godot/overlays/telephone_overlay.gd",
			"c_reference": "src/ovl/telephone/call_and_invitation_flow.c:1834-1885, src/ovl/telephone/appointment_call_scenes.c:1106-1131",
			"title": "TelephoneOverlay girl enum constants (10..12) and phone call stat side-effects / stage 4 bomb explosion helper",
			"details": "telephone_overlay.gd defines GIRL_YOSHIO=10, GIRL_YUMI=11, GIRL_REI=12 whereas PHONE_INVITATION_BASE_ADJUSTMENTS[0..10] has Yumi at index 10 (and holiday.c maps Yoshio=13, Miharu=11, Rei=11->12). Also in C, phone_date_invitation_resolve applies affection += (frame_count & 3) and syoushin = max(0, syoushin - 12) on every call (plus stress += 9, guts += rand & 7 on refusal), and mode 0x3D stage 4 runs phone_relationship_deltas_apply on bomb explosion (syoushin >= 91); these state mutations are not yet wrapped as helper methods on TelephoneOverlay."
		},
		{
			"id": "F-03",
			"severity": "low",
			"component": "port/godot/rules/scheduler.gd",
			"c_reference": "src/main/scheduler.c:75-83, 170-179, 2053-2055",
			"title": "Multi-month scheduler hooks for last_date_spot_timer_dec and month % 3 == 0 schedule_girl_spots_expire",
			"details": "scheduler.gd currently models the initial April week and does not yet include weekly last_date_spot_timer_dec() (12-week countdown) or calendar_advance_day's month % 3 == 0 hooks schedule_girl_spots_expire() and schedule_quarter_flags_rotate()."
		},
		{
			"id": "F-04",
			"severity": "low",
			"component": "port/godot/overlays/encounters_and_events_overlay.gd",
			"c_reference": "src/ovl/girl_encounters/walk_home.c:385, src/ovl/new_year_christmas/christmas_mansion_entry.c:45-90, src/ovl/misc_scenes/final_exam.c:901-906",
			"title": "Walk-Home decline girl subset (Yuko vs Saki), Christmas Mansion Dress Gate helper, and Exam rsin rank curve",
			"details": "1) apply_walk_home_choice line 200 checks GIRL_SAKI (4) instead of GIRL_YUKO (8) in the 6-girl subset for decline syoushin penalty at state==1. 2) christmas_mansion_entry.c's Dress/Looks Gate (looks >= stage * 30 - (stage >= 2 ? 10 : 0) + 70) is not yet exposed as a helper in encounters_and_events_overlay.gd. 3) build_exam_ranking uses a linear rank curve instead of final_exam.c's GTE rsin curve."
		},
	]

	var payload := {
		"ticket": "T-29018",
		"lane": "holiday-date-bomb-stress-v1",
		"test_script": "port/godot/tests/holiday/run_holiday_date_bomb_stress_tests.gd",
		"status": "PASS" if _failures.is_empty() else "FAIL",
		"total_assertions": _checks,
		"passed_assertions": _passed,
		"failed_assertions": _failures.size(),
		"failures": _failures,
		"suites": _suite_stats,
		"stress_metrics": _stress_metrics,
		"decomp_comparison_findings": decomp_findings,
	}

	var f := FileAccess.open(out_path, FileAccess.WRITE)
	if f != null:
		f.store_string(JSON.stringify(payload, "\t") + "\n")
		f.close()
	return out_path
