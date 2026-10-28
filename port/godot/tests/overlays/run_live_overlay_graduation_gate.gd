extends SceneTree
## T-29020 / T-29022: Headless Verification Gate for Live Interactive Overlays,
## Externalized Notebook Portrait & Live Caller Context, and Authentic PS1 Graduation Sequences.

const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const RuleFixtures = preload("res://tests/rules/run_rule_tests.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const TelephonePresenter = preload("res://presentation/telephone_overlay_presenter.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const Calendar = preload("res://rules/calendar.gd")
const PlayableBoot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")

var _checks: int = 0
var _failures: Array[String] = []


func _initialize() -> void:
	call_deferred("_run")


func _check(cond: bool, msg: String) -> void:
	_checks += 1
	if not cond:
		_failures.append(msg)
		printerr("FAIL: ", msg)
	else:
		print("PASS: ", msg)


func _empty(_state: Variant, _rng: Rng, _result: Session.FrameResult) -> void:
	pass


func _step_pad(session: Session, pad_mask: int) -> Session.FrameResult:
	session.latched_input = InputFrame.new(session.clock.logic_iterations - 1, pad_mask, 0)
	return session.step(InputFrame.new(session.clock.logic_iterations, 0), [])


func _resolve_repo_root() -> String:
	var candidates := [
		"/workspace",
		ProjectSettings.globalize_path("res://../../../../..").simplify_path(),
		ProjectSettings.globalize_path("res://../..").simplify_path(),
	]
	for c: String in candidates:
		if DirAccess.dir_exists_absolute(c.path_join(".asset-cache/port")):
			return c
	return "/workspace"


func _run() -> void:
	print("=== Suite 1: External Portrait Asset & Live Caller Date/Stats Context ===")
	_test_external_portrait_and_live_context()

	print("=== Suite 2: Interactive Live Overlay Input Dispatch & Delta Writeback ===")
	_test_live_interactive_overlay_dispatch()

	print("=== Suite 3: Authentic PS1 Graduation Sequences (Confession vs Bad Ending) ===")
	_test_graduation_confession_and_bad_ending()

	print("=== Suite 4: Subgames (RPG_BAT / TACO / TT) Writeback & PlayableBoot Live Overlay ===")
	_test_subgames_and_playable_boot_live_overlay()

	if not _failures.is_empty():
		printerr("================================================================")
		printerr("FAILED: run_live_overlay_graduation_gate (%d/%d failed)" % [_failures.size(), _checks])
		printerr("================================================================")
		quit(1)
		return

	print("================================================================")
	print("PASS: run_live_overlay_graduation_gate (%d assertions, 0 failures)" % _checks)
	print("================================================================")
	quit(0)


func _test_external_portrait_and_live_context() -> void:
	var presenter_src: String = FileAccess.get_file_as_string("res://presentation/telephone_overlay_presenter.gd")
	_check(not presenter_src.contains("SHIORI_PHOTO_B64"), "telephone_overlay_presenter.gd contains no SHIORI_PHOTO_B64 constant")
	_check(not presenter_src.contains("base64_to_raw"), "telephone_overlay_presenter.gd contains no Marshalls.base64_to_raw call")

	var repo_root: String = _resolve_repo_root()
	var hud_manifest: String = repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root: String = repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack: String = repo_root.path_join(".asset-cache/port/date-preview-v1")
	var joho_pack: String = repo_root.path_join(".asset-cache/port/joho-shiori-preview-v1")

	var presenter := TelephonePresenter.new()
	var opened: bool = presenter.open_telephone_preview(hud_manifest, font_root, date_pack, TelephonePresenter.HUD_SHA, joho_pack)
	_check(opened, "TelephoneOverlayPresenter opened external HUD, font, date, and JOHO photo packs cleanly: " + presenter.error)
	_check(presenter._shiori_notebook_photo != null and presenter._shiori_notebook_photo.get_size() == Vector2i(64, 64),
		"External Shiori notebook photo loaded as 64x64 RGBA8 Image")

	# Verify live caller date/stats context drives TelephoneOverlay and TelephoneOverlayPresenter
	var fixtures := RuleFixtures.new()
	var state: State = fixtures.synthetic_state()
	state.rule_state["year"] = 96
	state.rule_state["month"] = 11
	state.rule_state["day"] = 17
	state.rule_state["weekday"] = 6
	state.rule_state["day_count"] = 592
	state.rule_state["params"] = [115 << 16, 142 << 16, 138 << 16, 129 << 16, 131 << 16, 145 << 16, 122 << 16, 95 << 16, 14 << 16]
	Calendar.populate_month_calendar(state.rule_state, 96, 11)

	var tel_session: TelephoneOverlay.TelephoneSessionState = TelephoneOverlay.create_session_from_live_state(state.rule_state, 0)
	_check(tel_session.live_year == 96 and tel_session.live_month == 11 and tel_session.live_day == 17,
		"TelephoneSessionState captures live caller date 96-11-17 instead of hardcoded 95-04-09")
	_check(tel_session.live_stats == [115, 142, 138, 129, 131, 145, 122, 95, 14],
		"TelephoneSessionState extracts live caller parameter stats from Q16.16 params")

	presenter.set_live_context(TelephoneOverlay.extract_live_context(state.rule_state))
	var img_live_a: Image = presenter.render_girl_profile_notebook_screen(0)
	_check(img_live_a != null and img_live_a.get_size() == Vector2i(320, 240),
		"Notebook screen rendered 320x240 with live context A (96-11-17)")

	state.rule_state["year"] = 97
	state.rule_state["month"] = 8
	state.rule_state["day"] = 24
	state.rule_state["params"] = [88 << 16, 64 << 16, 72 << 16, 91 << 16, 105 << 16, 83 << 16, 77 << 16, 60 << 16, 25 << 16]
	presenter.set_live_context(TelephoneOverlay.extract_live_context(state.rule_state))
	var img_live_b: Image = presenter.render_girl_profile_notebook_screen(0)
	_check(img_live_a.get_data() != img_live_b.get_data(),
		"Notebook screen pixel output changes when live SchoolDayController date and stats change")
	_check(presenter.bounding_box_violations.is_empty(),
		"Zero glyph bounding-box violations across live notebook renders")


func _test_live_interactive_overlay_dispatch() -> void:
	var fixtures := RuleFixtures.new()
	var tables: Dictionary = fixtures.synthetic_tables()
	var state: State = fixtures.synthetic_state()
	state.rule_state["allow_multi_week"] = true
	state.rule_state["year"] = 95
	state.rule_state["month"] = 5
	state.rule_state["day"] = 14
	state.rule_state["weekday"] = 6
	state.rule_state["params"] = [100 << 16, 120 << 16, 120 << 16, 120 << 16, 120 << 16, 150 << 16, 130 << 16, 100 << 16, 10 << 16]
	state.rule_state["girls"][0]["affection_word"] = 70 << 16
	state.rule_state["girls"][0]["tokimeki_word"] = 75 << 16
	state.rule_state["girls"][0]["flags"] = 2
	Calendar.populate_month_calendar(state.rule_state, 95, 5)

	# Queue: [0] = 0x46 (TEL), [1] = 0x44 (GEKO walk-home), [2] = 0x80 (DATE), [3] = 0xff (end of queue)
	var queue: Array = []
	queue.append([0x46, 0, 0, 0])
	queue.append([0x44, 0, 0, 0])
	queue.append([0x80, 0, 3, 0])
	while queue.size() < 32:
		queue.append([0xff, 0, 0, 0])
	state.rule_state["day_events"] = queue
	state.rule_state["day_event_index"] = 0
	state.reset_game_mode_fields(0x31)

	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 42)
	var controller := Controller.new(tables)
	var attach_err: String = controller.attach_multi_week(session, state)
	_check(attach_err.is_empty(), "attach_multi_week succeeded for interactive overlay test")
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty

	# Step 1: 0x31 dispatches event[0] (0x46 -> native mode 0x5A / dispatcher mode 0x3D TEL) and initializes overlay_state
	var r0: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
	_check(r0.error.is_empty() and state.game_mode in [0x3D, 0x5A] and int(state.rule_state.get("overlay_state", {}).get("dispatcher_mode", -1)) == 0x3D, "Dispatched queue event 0x46 into mode 0x3D (TEL)")
	_check(state.rule_state.get("overlay_state") is Dictionary, "Initialized active overlay_state for TEL")

	# Step 2: Neutral input frames must NOT advance or auto-complete the overlay
	for _n: int in range(3):
		var rn: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
		_check(rn.error.is_empty() and state.game_mode in [0x3D, 0x5A] and int(state.rule_state["day_event_index"]) == 0,
			"Neutral input frame does not auto-skip TEL overlay")

	# Step 3: Directional input (PAD_DOWN) moves place cursor 0 -> 1
	var r_down: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_DOWN)
	_check(r_down.error.is_empty() and int(state.rule_state["overlay_state"].get("cursor", -1)) == 1,
		"PAD_DOWN routed through OverlayDispatcher moves TEL place cursor 0 -> 1")
	_check(int(state.rule_state["day_event_index"]) == 0 and state.game_mode in [0x3D, 0x5A],
		"TEL overlay stays active on place_select after cursor move")

	# Step 4: First confirm (PAD_CIRCLE) advances place_select -> day_select (does NOT complete overlay!)
	var r_place: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(r_place.error.is_empty() and String(state.rule_state["overlay_state"].get("subscreen", "")) == "day_select",
		"First PAD_CIRCLE advances TEL place_select -> day_select without completing overlay")
	_check(int(state.rule_state["day_event_index"]) == 0 and state.game_mode in [0x3D, 0x5A],
		"day_event_index remains 0 during TEL day_select")

	# Step 5: Cancel (PAD_CROSS) backs out day_select -> place_select
	var r_back: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CROSS)
	_check(r_back.error.is_empty() and String(state.rule_state["overlay_state"].get("subscreen", "")) == "place_select",
		"PAD_CROSS backs out TEL day_select -> place_select")

	# Step 6: Re-confirm place -> confirm day -> confirm call_response to complete TEL
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	var r_day: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(r_day.error.is_empty() and String(state.rule_state["overlay_state"].get("subscreen", "")) == "call_response",
		"Second PAD_CIRCLE advances TEL day_select -> call_response")
	_check(int(state.rule_state["day_event_index"]) == 0 and state.game_mode in [0x3D, 0x5A],
		"TEL overlay remains active on call_response until final confirm")

	var aff_before_tel: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var r_tel_done: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(r_tel_done.error.is_empty() and state.game_mode == 0x31 and int(state.rule_state["day_event_index"]) == 1,
		"Final PAD_CIRCLE completes TEL overlay, advances day_event_index 0 -> 1, and returns to 0x31")
	_check(not state.rule_state.has("overlay_state"), "Active overlay_state cleared after TEL completion")
	_check(int(state.rule_state["appointment_words"][0]) != 0,
		"Accepted TEL invitation booked appointment into live state.rule_state['appointment_words']")
	_check(State.high(int(state.rule_state["girls"][0]["affection_word"])) > aff_before_tel,
		"Completed TEL overlay wrote back affection delta to live SchoolDayController state")

	# Step 7: Advance into event[1] (0x44 -> native mode 0x59 / dispatcher mode 0x44 GEKO walk-home)
	var r_geko_enter: Session.FrameResult = _step_pad(session, 0)
	_check(r_geko_enter.error.is_empty() and state.game_mode in [0x44, 0x59] and int(state.rule_state.get("overlay_state", {}).get("dispatcher_mode", -1)) == 0x44, "Dispatched queue event 0x44 into mode 0x44 (GEKO)")
	var aff_before_geko: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var r_geko_stage1: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(r_geko_stage1.error.is_empty() and state.game_mode in [0x44, 0x59] and int(state.rule_state["day_event_index"]) == 1,
		"First PAD_CIRCLE in GEKO advances to stage 1 without auto-skipping")
	var r_geko_done: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(r_geko_done.error.is_empty() and state.game_mode == 0x31 and int(state.rule_state["day_event_index"]) == 2,
		"Second PAD_CIRCLE completes GEKO and advances day_event_index 1 -> 2")
	_check(State.high(int(state.rule_state["girls"][0]["affection_word"])) == aff_before_geko + 2,
		"Completed GEKO walk-home wrote +2 affection delta to live state")

	# Step 8: Advance into event[2] (0x80 -> mode 0x50 DATE)
	var r_date_enter: Session.FrameResult = _step_pad(session, 0)
	_check(r_date_enter.error.is_empty() and state.game_mode == 0x50, "Dispatched queue event 0x80 into mode 0x50 (DATE)")
	var dates_before: int = int(state.rule_state["girls"][0]["counters"][1])
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # stage 0 -> 1
	_step_pad(session, OverlayDispatcher.PAD_DOWN)   # choice cursor 0 -> 1
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # stage 1 -> 2
	_check(state.game_mode == 0x50 and int(state.rule_state["day_event_index"]) == 2,
		"DATE overlay remains active at stage 2 reaction before final confirm")
	var r_date_done: Session.FrameResult = _step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(r_date_done.error.is_empty() and state.game_mode == 0x31 and int(state.rule_state["day_event_index"]) == 3,
		"Final PAD_CIRCLE completes DATE overlay and advances day_event_index 2 -> 3")
	_check(int(state.rule_state["girls"][0]["counters"][1]) == dates_before + 1,
		"Completed DATE overlay incremented live date counter in state.rule_state['girls'][0]['counters'][1]")


func _test_graduation_confession_and_bad_ending() -> void:
	var controller_src: String = FileAccess.get_file_as_string("res://scenes/school_day_controller.gd")
	var ending_src: String = FileAccess.get_file_as_string("res://overlays/ending_overlay.gd")
	_check(not controller_src.contains("best_aff") and not controller_src.contains("= 79"),
		"school_day_controller.gd has zero fabricated affection > 79 graduation fallback")
	_check(not ending_src.contains("best_aff") and not ending_src.contains("= 79"),
		"ending_overlay.gd has zero fabricated affection > 79 graduation fallback")

	var fixtures := RuleFixtures.new()
	var tables: Dictionary = fixtures.synthetic_tables()

	# 1. Eligible Heroine Confession Route (Shiori = 0): confession -> staff_roll -> last_photo -> clear_save
	var state_ok: State = fixtures.synthetic_state()
	state_ok.rule_state["allow_multi_week"] = true
	state_ok.rule_state["year"] = 98
	state_ok.rule_state["month"] = 3
	state_ok.rule_state["day"] = 1
	state_ok.rule_state["weekday"] = 6
	state_ok.rule_state["params"] = [100 << 16, 145 << 16, 145 << 16, 145 << 16, 135 << 16, 130 << 16, 130 << 16, 145 << 16, 10 << 16]
	state_ok.rule_state["girls"][0]["affection_word"] = 120 << 16
	state_ok.rule_state["girls"][0]["tokimeki_word"] = 120 << 16
	state_ok.rule_state["girls"][0]["syoushin_word"] = 0
	state_ok.rule_state["girls"][0]["flags"] = 2 | (1 << 26)
	state_ok.rule_state["girls"][0]["counters"][1] = 8 # 8 dates with Shiori

	var cand_ok: Dictionary = EndingOverlay.evaluate_confession_candidates(state_ok.rule_state)
	_check(bool(cand_ok["eligible"]) and int(cand_ok["current_girl"]) == EndingOverlay.GIRL_SHIORI,
		"evaluate_confession_candidates selects Shiori (0) when real PS1 stat and date requirements are met")
	_check(cand_ok["phases"] == ["confession", "staff_roll", "last_photo", "clear_save"],
		"Eligible heroine graduation sequence is [confession, staff_roll, last_photo, clear_save]")

	var session_ok := Session.new()
	session_ok.rng.configure(Rng.OPENBIOS_PROFILE, 7)
	var controller_ok := Controller.new(tables)
	controller_ok.attach_multi_week(session_ok, state_ok)
	session_ok.draw_logic = _empty
	session_ok.cd_logic = _empty
	session_ok.audio_logic = _empty
	state_ok.reset_game_mode_fields(0x31)

	# Step 0x31 step 0 -> step 1 -> mode 0x5C (ENDING)
	session_ok.step(InputFrame.new(session_ok.clock.logic_iterations, 0), [])
	session_ok.step(InputFrame.new(session_ok.clock.logic_iterations, 0), [])
	_check(state_ok.game_mode == 0x5c, "Graduation Day 1998-03-01 transitions to mode 0x5C (ENDING)")
	_check(String(state_ok.rule_state["overlay_state"].get("phase", "")) == "confession",
		"Graduation overlay starts in 'confession' phase for eligible heroine")

	# Neutral frame must not advance phase
	session_ok.step(InputFrame.new(session_ok.clock.logic_iterations, 0), [])
	_check(String(state_ok.rule_state["overlay_state"].get("phase", "")) == "confession" and int(state_ok.rule_state.get("ending_completed", 0)) == 0,
		"Neutral frame does not advance graduation 'confession' phase")

	# Step through confession -> staff_roll -> last_photo -> clear_save -> complete
	_step_pad(session_ok, OverlayDispatcher.PAD_CIRCLE)
	_check(String(state_ok.rule_state["overlay_state"].get("phase", "")) == "staff_roll",
		"Confirm 1 advances graduation confession -> staff_roll")
	_step_pad(session_ok, OverlayDispatcher.PAD_CIRCLE)
	_check(String(state_ok.rule_state["overlay_state"].get("phase", "")) == "last_photo",
		"Confirm 2 advances graduation staff_roll -> last_photo")
	_step_pad(session_ok, OverlayDispatcher.PAD_CIRCLE)
	_check(String(state_ok.rule_state["overlay_state"].get("phase", "")) == "clear_save",
		"Confirm 3 advances graduation last_photo -> clear_save")
	_check(int(state_ok.rule_state.get("ending_completed", 0)) == 0,
		"Graduation is not marked completed before clear_save confirm")
	_step_pad(session_ok, OverlayDispatcher.PAD_CIRCLE)
	_check(int(state_ok.rule_state.get("ending_completed", 0)) == 1,
		"Confirm 4 completes clear_save and marks ending_completed = 1")
	_check(int(state_ok.rule_state.get("confessed_girl", -1)) == EndingOverlay.GIRL_SHIORI,
		"Completed confession ending records confessed_girl = 0 (Shiori)")
	_check(String(state_ok.rule_state.get("ending_type", "")) == "confession",
		"Completed confession ending records ending_type = 'confession'")
	_check(state_ok.rule_state.get("ending_phases", []) == ["confession", "staff_roll", "last_photo", "clear_save"],
		"Completed confession ending recorded full phase history [confession, staff_roll, last_photo, clear_save]")
	_check(bool(state_ok.rule_state.get("clear_save_completed", false)),
		"Completed confession ending recorded clear_save_completed = true")

	# 2. Ineligible Route -> Authentic PS1 Yoshio Bad Ending (NO fabricated affection fallback)
	var state_bad: State = fixtures.synthetic_state()
	state_bad.rule_state["allow_multi_week"] = true
	state_bad.rule_state["year"] = 98
	state_bad.rule_state["month"] = 3
	state_bad.rule_state["day"] = 1
	state_bad.rule_state["weekday"] = 6
	# Affection is 95 (> 79), which would have triggered the old fabricated fallback,
	# but date count is only 2 (< 8) and stats are below Shiori's minimums.
	state_bad.rule_state["params"] = [80 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 10 << 16]
	state_bad.rule_state["girls"][0]["affection_word"] = 95 << 16
	state_bad.rule_state["girls"][0]["tokimeki_word"] = 95 << 16
	state_bad.rule_state["girls"][0]["flags"] = 2
	state_bad.rule_state["girls"][0]["counters"][1] = 2

	var cand_bad: Dictionary = EndingOverlay.evaluate_confession_candidates(state_bad.rule_state)
	_check(not bool(cand_bad["eligible"]) and int(cand_bad["current_girl"]) == EndingOverlay.GIRL_YOSHIO,
		"Ineligible heroine with affection=95 does NOT fabricate confession; routes to GIRL_YOSHIO (13)")
	_check(cand_bad["phases"] == ["yoshio_bad_ending", "staff_roll", "clear_save"],
		"Ineligible graduation sequence is [yoshio_bad_ending, staff_roll, clear_save] without last_photo")

	var session_bad := Session.new()
	session_bad.rng.configure(Rng.OPENBIOS_PROFILE, 7)
	var controller_bad := Controller.new(tables)
	controller_bad.attach_multi_week(session_bad, state_bad)
	session_bad.draw_logic = _empty
	session_bad.cd_logic = _empty
	session_bad.audio_logic = _empty
	state_bad.reset_game_mode_fields(0x31)

	session_bad.step(InputFrame.new(session_bad.clock.logic_iterations, 0), [])
	session_bad.step(InputFrame.new(session_bad.clock.logic_iterations, 0), [])
	_check(state_bad.game_mode == 0x5c, "Ineligible Graduation Day transitions to mode 0x5C (ENDING)")
	_check(String(state_bad.rule_state["overlay_state"].get("phase", "")) == "yoshio_bad_ending",
		"Ineligible graduation starts in 'yoshio_bad_ending' phase")

	_step_pad(session_bad, OverlayDispatcher.PAD_CIRCLE)
	_check(String(state_bad.rule_state["overlay_state"].get("phase", "")) == "staff_roll",
		"Confirm 1 advances yoshio_bad_ending -> staff_roll")
	_step_pad(session_bad, OverlayDispatcher.PAD_CIRCLE)
	_check(String(state_bad.rule_state["overlay_state"].get("phase", "")) == "clear_save",
		"Confirm 2 advances staff_roll -> clear_save (skipping last_photo)")
	_step_pad(session_bad, OverlayDispatcher.PAD_CIRCLE)
	_check(int(state_bad.rule_state.get("ending_completed", 0)) == 1,
		"Confirm 3 completes clear_save on bad ending route")
	_check(int(state_bad.rule_state.get("confessed_girl", -1)) == EndingOverlay.GIRL_YOSHIO,
		"Ineligible graduation records confessed_girl = 13 (GIRL_YOSHIO) with no fabricated heroine")
	_check(String(state_bad.rule_state.get("ending_type", "")) == "yoshio_bad_ending",
		"Ineligible graduation records ending_type = 'yoshio_bad_ending'")
	_check(state_bad.rule_state.get("ending_phases", []) == ["yoshio_bad_ending", "staff_roll", "clear_save"],
		"Ineligible graduation recorded exact phases [yoshio_bad_ending, staff_roll, clear_save]")


func _test_subgames_and_playable_boot_live_overlay() -> void:
	var fixtures := RuleFixtures.new()
	var tables: Dictionary = fixtures.synthetic_tables()
	var state: State = fixtures.synthetic_state()
	state.rule_state["allow_multi_week"] = true
	state.rule_state["year"] = 96
	state.rule_state["month"] = 10
	state.rule_state["day"] = 18
	state.rule_state["weekday"] = 4
	state.rule_state["params"] = [110 << 16, 125 << 16, 130 << 16, 120 << 16, 140 << 16, 135 << 16, 118 << 16, 105 << 16, 12 << 16]
	Calendar.populate_month_calendar(state.rule_state, 96, 10)

	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 19)
	var controller := Controller.new(tables)
	controller.attach_multi_week(session, state)
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty

	# 1. RPG_BAT (0x57) live session loop & talk_result_code writeback
	controller.start_overlay(state, 0x57, {"enemy_hp": 180})
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # Defeats enemy (180 HP) -> victory stage 10
	_check(state.game_mode == 0x57 and bool(state.rule_state["overlay_state"].get("victory", false)),
		"RPG_BAT (0x57) reaches victory stage 10 and waits for player confirm")
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # Exits victory screen -> 0x31
	_check(state.game_mode == 0x31 and int(state.rule_state.get("talk_result_code", -1)) == 0,
		"Completed RPG_BAT wrote back talk_result_code=0 to state.rule_state")

	# 2. TACO (0x59) live session loop: does NOT exit on 2nd fire press; writes back high_scores & stages_cleared
	controller.start_overlay(state, 0x59, {"waves_per_stage": 1, "target_stages": 1})
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(state.game_mode == 0x59 and state.rule_state.has("overlay_state"),
		"TACO (0x59) does NOT exit on 2nd fire press in live SchoolDayController session")
	_step_pad(session, OverlayDispatcher.PAD_SQUARE)
	_step_pad(session, OverlayDispatcher.PAD_SQUARE)
	state.rule_state["overlay_state"]["boss"]["hp"] = 2
	_step_pad(session, OverlayDispatcher.PAD_CROSS)  # Boss defeated -> GAME_END (45)
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # -> RESULT (8)
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # -> SCORES (5)
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # -> EXIT (46) -> 0x31
	_check(state.game_mode == 0x31 and int(state.rule_state.get("stages_cleared", 0)) == 1,
		"Completed TACO (0x59) wrote back stages_cleared=1 to state.rule_state")
	_check((state.rule_state.get("high_scores", []) as Array).size() == 3,
		"Completed TACO (0x59) wrote back 3-slot high_scores array to state.rule_state")

	# 3. TT (0x5A) live session loop: does NOT collect bells on PAD_UP or exit on 3rd confirm; writes back tt_stages_cleared
	controller.start_overlay(state, 0x5A, {"target_stages": 1, "boss_hp": 16})
	_step_pad(session, OverlayDispatcher.PAD_UP)
	_check(int(state.rule_state["overlay_state"].get("score", -1)) == 0,
		"TT (0x5A) PAD_UP does NOT arbitrarily collect bells in live session")
	for _i: int in range(3):
		_step_pad(session, OverlayDispatcher.PAD_CIRCLE)
	_check(state.game_mode == 0x5A and state.rule_state.has("overlay_state"),
		"TT (0x5A) does NOT exit after 3 confirms in live session")
	_step_pad(session, OverlayDispatcher.PAD_SQUARE) # Ground bomb defeats boss (hp=16) -> STAGE_CLEAR (3)
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # -> RANKING_VIEW (7)
	_step_pad(session, OverlayDispatcher.PAD_CIRCLE) # -> EXIT (9) -> 0x31
	_check(state.game_mode == 0x31 and int(state.rule_state.get("tt_stages_cleared", 0)) == 1,
		"Completed TT (0x5A) wrote back tt_stages_cleared=1 to state.rule_state")
	_check((state.rule_state.get("tt_rankings", []) as Array).size() == 3,
		"Completed TT (0x5A) wrote back 3-slot tt_rankings table to state.rule_state")

	# 4. PlayableBoot live overlay context rendering & step_active_overlay integration
	var repo_root: String = _resolve_repo_root()
	var hud_manifest: String = repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root: String = repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack: String = repo_root.path_join(".asset-cache/port/date-preview-v1")

	var boot := PlayableBoot.new()
	boot.state = state
	boot.session = session
	boot.controller = controller
	boot.canvas = Canvas.new()
	boot.overlay_dispatcher = OverlayDispatcher.new()
	var od_err: String = boot.overlay_dispatcher.open_resources(font_root, hud_manifest, date_pack)
	_check(od_err.is_empty(), "PlayableBoot OverlayDispatcher opened resources cleanly")
	controller.overlay_dispatcher = boot.overlay_dispatcher

	controller.start_overlay(state, 0x3E, {"girl_id": 0})
	_check(boot._has_active_overlay(state), "PlayableBoot._has_active_overlay detects active 0x3E overlay")
	var render_ctx: Dictionary = boot._build_overlay_render_state(state)
	_check(int(render_ctx.get("year", 0)) == 96 and int(render_ctx.get("month", 0)) == 10 and int(render_ctx.get("day", 0)) == 18,
		"PlayableBoot._build_overlay_render_state injects live caller date 96-10-18")
	_check(render_ctx.get("stats", []) == [110, 125, 130, 120, 140, 135, 118, 105, 12],
		"PlayableBoot._build_overlay_render_state injects live caller stats from state.rule_state['params']")
	var present_err: String = boot._present_committed()
	_check(present_err.is_empty() and boot._last_rendered_image != null and boot._last_rendered_image.get_size() == Vector2i(320, 240),
		"PlayableBoot._present_committed renders 320x240 frame for active overlay with live state context")
	var step_res: Dictionary = boot.step_active_overlay(OverlayDispatcher.PAD_DOWN)
	_check(not bool(step_res.get("completed", true)) and int(state.rule_state["overlay_state"].get("girl_id", -1)) == 1,
		"PlayableBoot.step_active_overlay steps TEL_ACTION notebook girl_id 0 -> 1 via OverlayDispatcher")
	boot.canvas.set("_textures", {})
	boot.canvas.set("_commands", [])
	boot.canvas.free()
	boot.free()

