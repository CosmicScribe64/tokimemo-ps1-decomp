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
const DayEventModes = preload("res://rules/day_event_modes.gd")
const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const TelephonePresenter = preload("res://presentation/telephone_overlay_presenter.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const Calendar = preload("res://rules/calendar.gd")
const PlayableBoot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")

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



## T-39006: TEL, GEKO, DATE, RPG_BAT and TEL_ACTION have no source-backed
## production handler, so OverlayDispatcher refuses them when saved state is
## present. Each input must fail the frame with the dispatcher's reason, keep
## the mode and leave saved state (overlay included) unchanged.
func _assert_refused(session: Session, state: State, modes: Array, label: String, reason: String, pads: Array) -> void:
	var before: Dictionary = state.rule_state.duplicate(true)
	for pad: int in pads:
		var res: Session.FrameResult = _step_pad(session, pad)
		_check(res.error.contains("refused with saved state") and res.error.contains(reason),
			"%s refuses saved-state input 0x%04X in production (%s)" % [label, pad, res.error])
		_check(state.game_mode in modes and state.rule_state == before,
			"%s refusal keeps the mode and leaves saved state unchanged (pad 0x%04X)" % [label, pad])


func _preview_rng() -> Rng:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 42)
	return rng


## Preview stepping (no saved state) exercises the overlay's input handling for
## rendering. It never writes saved state and its exit carries no deltas.
func _preview_step(controller: Controller, mode: int, overlay_state: Dictionary, pad: int) -> Dictionary:
	return controller.overlay_dispatcher.step_overlay(mode, overlay_state, pad, {}, _preview_rng())


## A refused family has no production exit. The gate drops its overlay, puts
## the harness back on the day screen (0x31) and points the queue at the next
## event, as the G3 gate does.
func _leave_refused_family(state: State, next_index: int = -1) -> void:
	state.rule_state.erase("overlay_state")
	if next_index >= 0:
		state.rule_state["day_event_index"] = next_index
	state.reset_game_mode_fields(0x31)

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

	print("=== Suite 2: Interactive Live Overlay Dispatch, Production Refusals & Preview Stepping ===")
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

	# Queue: [0] = 0x46 (GEKO invitation), [1] = 0x44 (GEKO walk-home), [2] = 0x80 (DATE), [3] = 0xff (end of queue)
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

	# Step 1: 0x31 dispatches event[0]. Queue 0x46 enters native 0x5A, whose
	# overlay_loader.c entry D_801426D0 is the GEKO invitation handler
	# (src/ovl/girl_encounters/invite_entry.c), so the dispatcher id is GEKO.
	var r0: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
	_check(r0.error.is_empty() and state.game_mode == 0x5A and int(state.rule_state.get("overlay_state", {}).get("dispatcher_mode", -1)) == OverlayDispatcher.GAME_MODE_GEKO,
		"Dispatched queue event 0x46 into native 0x5A (GEKO invitation)")
	_check(state.rule_state.get("overlay_state") is Dictionary, "Initialized active overlay_state for the invitation")

	# Step 2: Neutral input frames must NOT advance or auto-complete the overlay
	for _n: int in range(3):
		var rn: Session.FrameResult = session.step(InputFrame.new(session.clock.logic_iterations, 0), [])
		_check(rn.error.is_empty() and state.game_mode == 0x5A and int(state.rule_state["day_event_index"]) == 0,
			"Neutral input frame does not auto-skip the invitation overlay")

	# Step 3 (T-39006): GEKO is refused with saved state; the invitation books
	# no appointment and writes no affection.
	var aff_before_inv: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var appt_before_inv: Array = (state.rule_state.get("appointment_words", []) as Array).duplicate()
	_assert_refused(session, state, [0x5A], "GEKO invitation", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_GEKO],
		[OverlayDispatcher.PAD_DOWN, OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CROSS])
	_check(int(state.rule_state["day_event_index"]) == 0 and state.rule_state.has("overlay_state"),
		"Refused invitation stays on event 0 with its overlay")
	_check((state.rule_state.get("appointment_words", []) as Array) == appt_before_inv
		and State.high(int(state.rule_state["girls"][0]["affection_word"])) == aff_before_inv,
		"Refused invitation books no appointment and writes no affection")

	# TEL (native 0x43 GAME_MODE_TELEPHONE): phone_date_invitation_resolve writes
	# and the appointment flag are not ported, so saved-state input is refused.
	_leave_refused_family(state, 0)
	controller.start_overlay(state, 0x43)
	_check(int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == OverlayDispatcher.GAME_MODE_TEL,
		"Native 0x43 enters dispatcher TEL")
	var aff_before_tel: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var appt_before_tel: Array = (state.rule_state.get("appointment_words", []) as Array).duplicate()
	_assert_refused(session, state, [0x43], "TEL", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_TEL],
		[OverlayDispatcher.PAD_DOWN, OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CROSS])
	_check((state.rule_state.get("appointment_words", []) as Array) == appt_before_tel
		and State.high(int(state.rule_state["girls"][0]["affection_word"])) == aff_before_tel,
		"Refused TEL books no appointment and writes no affection")

	# Preview TEL (no saved state): place cursor, place/day/call_response subscreens and an exit without deltas.
	var tel_mode: int = state.game_mode
	var tel: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	var p: Dictionary = _preview_step(controller, tel_mode, tel, OverlayDispatcher.PAD_DOWN)
	_check(int(p["state"].get("cursor", -1)) == 1 and not bool(p.get("completed", true)),
		"TEL preview PAD_DOWN moves the place cursor 0 -> 1")
	p = _preview_step(controller, tel_mode, p["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(String(p["state"].get("subscreen", "")) == "day_select" and not bool(p.get("completed", true)),
		"TEL preview first PAD_CIRCLE advances place_select -> day_select")
	p = _preview_step(controller, tel_mode, p["state"], OverlayDispatcher.PAD_CROSS)
	_check(String(p["state"].get("subscreen", "")) == "place_select",
		"TEL preview PAD_CROSS backs out day_select -> place_select")
	p = _preview_step(controller, tel_mode, p["state"], OverlayDispatcher.PAD_CIRCLE)
	p = _preview_step(controller, tel_mode, p["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(String(p["state"].get("subscreen", "")) == "call_response" and not bool(p.get("completed", true)),
		"TEL preview second PAD_CIRCLE advances day_select -> call_response")
	p = _preview_step(controller, tel_mode, p["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(bool(p.get("completed", false)) and (p.get("deltas", {}) as Dictionary).is_empty(),
		"TEL preview final PAD_CIRCLE exits with no deltas")
	_leave_refused_family(state, 1)

	# Step 4: event[1] (0x44 -> native mode 0x59 / dispatcher mode 0x44 GEKO walk-home), refused with saved state.
	var r_geko_enter: Session.FrameResult = _step_pad(session, 0)
	_check(r_geko_enter.error.is_empty() and state.game_mode in [0x44, 0x59] and int(state.rule_state.get("overlay_state", {}).get("dispatcher_mode", -1)) == 0x44, "Dispatched queue event 0x44 into mode 0x44 (GEKO)")
	_assert_refused(session, state, [0x44, 0x59], "GEKO", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_GEKO],
		[OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CIRCLE])
	var geko_mode: int = state.game_mode
	p = _preview_step(controller, geko_mode, state.rule_state["overlay_state"].duplicate(true), OverlayDispatcher.PAD_CIRCLE)
	_check(not bool(p.get("completed", true)), "GEKO preview first PAD_CIRCLE advances without completing")
	p = _preview_step(controller, geko_mode, p["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(bool(p.get("completed", false)) and (p.get("deltas", {}) as Dictionary).is_empty(),
		"GEKO preview second PAD_CIRCLE exits with no deltas")
	_leave_refused_family(state, 2)

	# Step 5: event[2] (0x80 -> mode 0x50 DATE), refused with saved state.
	var r_date_enter: Session.FrameResult = _step_pad(session, 0)
	_check(r_date_enter.error.is_empty() and state.game_mode == 0x50, "Dispatched queue event 0x80 into mode 0x50 (DATE)")
	var dates_before: int = int(state.rule_state["girls"][0]["counters"][1])
	_assert_refused(session, state, [0x50], "DATE", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_DATE],
		[OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_DOWN, OverlayDispatcher.PAD_CIRCLE])
	_check(int(state.rule_state["girls"][0]["counters"][1]) == dates_before,
		"Refused DATE leaves the date counter unchanged")
	p = _preview_step(controller, 0x50, state.rule_state["overlay_state"].duplicate(true), OverlayDispatcher.PAD_CIRCLE)
	p = _preview_step(controller, 0x50, p["state"], OverlayDispatcher.PAD_DOWN)
	p = _preview_step(controller, 0x50, p["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(not bool(p.get("completed", true)), "DATE preview stays active at the stage 2 reaction")
	p = _preview_step(controller, 0x50, p["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(bool(p.get("completed", false)) and (p.get("deltas", {}) as Dictionary).is_empty(),
		"DATE preview final PAD_CIRCLE exits with no deltas")
	_leave_refused_family(state, 3)


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
	# T-39007: Confirm 4 answers "yes" to the clear-save prompt. The ending waits
	# for the host's storage completion; a forged saved state does not finish it.
	_check(int(state_ok.rule_state.get("ending_completed", 0)) == 0
		and state_ok.rule_state["overlay_state"].get("clear_save_status") == "requested",
		"Confirm 4 requests the clear save and waits for storage")
	var forged: Dictionary = state_ok.rule_state["overlay_state"].duplicate(true)
	state_ok.rule_state["overlay_state"]["clear_save_status"] = "saved"
	state_ok.rule_state["overlay_state"]["clear_save_completed"] = true
	state_ok.rule_state["overlay_state"]["clear_save_receipt"] = {"service": "ending.clear_save", "girl": 0, "counter": 1,
		"album_clear_mask": 0x1FFF, "checksum_sha256": "7c3a".repeat(16)}
	for forged_pad: int in [0, 0x0800, 0x1000]:
		_step_pad(session_ok, forged_pad)
		_check(int(state_ok.rule_state.get("ending_completed", 0)) == 0
			and not bool(state_ok.rule_state.get("clear_save_completed", false)),
			"Forged saved status with a well-formed receipt does not complete the clear save (pad 0x%04x)" % forged_pad)
	state_ok.rule_state["overlay_state"] = forged
	var save_root: String = "/tmp/t39007-graduation-" + str(Time.get_ticks_usec())
	var host: ClearSave = ClearSave.new(save_root)
	var service: Dictionary = host.service_request(state_ok.rule_state["overlay_state"])
	_check(service.get("payload", {}).get("ok") == true, "Host clear-save transaction published the system record")
	_check(host.service_request(state_ok.rule_state["overlay_state"]).is_empty(), "One request attempt is written once")
	var tick: int = session_ok.clock.logic_iterations
	session_ok.step(InputFrame.new(tick, 0), [Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
		Session.ServiceKind.COMPLETION, service["service"], service["payload"])])
	_check(int(state_ok.rule_state.get("ending_completed", 0)) == 1,
		"Clear-save completion with a read-back receipt completes the ending")
	_check(int(state_ok.rule_state.get("confessed_girl", -1)) == EndingOverlay.GIRL_SHIORI,
		"Completed confession ending records confessed_girl = 0 (Shiori)")
	_check(String(state_ok.rule_state.get("ending_type", "")) == "confession",
		"Completed confession ending records ending_type = 'confession'")
	_check(state_ok.rule_state.get("ending_phases", []) == ["confession", "staff_roll", "last_photo", "clear_save"],
		"Completed confession ending recorded full phase history [confession, staff_roll, last_photo, clear_save]")
	_check(bool(state_ok.rule_state.get("clear_save_completed", false)),
		"Completed confession ending recorded clear_save_completed = true")
	_check(int(state_ok.rule_state.get("album_clear_mask", 0)) == 1
		and ClearSave.new(save_root).record["girl_clear_counters"][0] == 1,
		"Album clear mask comes from the stored per-girl clear counter")

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
	_step_pad(session_bad, EndingOverlay.PAD_CROSS)
	_check(int(state_bad.rule_state.get("ending_completed", 0)) == 1
		and not bool(state_bad.rule_state.get("clear_save_completed", true)),
		"Cross declines the clear save and completes the bad ending unsaved")
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

	# Native RPG_BAT/TACO/TT are 0x81/0x90/0xD0 (include/game_enums.h). Native
	# 0x57/0x59/0x5A are GEKO modes and must never reach a minigame.
	for geko_mode: int in [0x57, 0x59, 0x5A]:
		_check(DayEventModes.dispatcher_mode_for(geko_mode) == OverlayDispatcher.GAME_MODE_GEKO,
			"native 0x%02X maps to GEKO, not a minigame" % geko_mode)
	# The queue path passes the opcode too; overlay_loader.c enters GEKO for all four.
	for pair: Array in [[0x32, 0x57], [0x33, 0x56], [0x44, 0x59], [0x46, 0x5A]]:
		_check(DayEventModes.dispatcher_mode_for(pair[1], pair[0]) == OverlayDispatcher.GAME_MODE_GEKO,
			"queue 0x%02X / native 0x%02X resolves to GEKO" % pair)

	# 1. RPG_BAT (native 0x81), T-39006: refused with saved state.
	controller.start_overlay(state, 0x81, {"enemy_hp": 180})
	var talk_before: Variant = state.rule_state.get("talk_result_code")
	_assert_refused(session, state, [0x81], "RPG_BAT", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_RPG_BATTLE],
		[OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CIRCLE])
	_check(state.rule_state.get("talk_result_code") == talk_before, "Refused RPG_BAT writes no talk_result_code")
	var rpg: Dictionary = _preview_step(controller, 0x81, state.rule_state["overlay_state"].duplicate(true), OverlayDispatcher.PAD_CIRCLE)
	_check(bool(rpg["state"].get("victory", false)) and not bool(rpg.get("completed", true)),
		"RPG_BAT preview reaches victory stage 10 and waits for confirm")
	rpg = _preview_step(controller, 0x81, rpg["state"], OverlayDispatcher.PAD_CIRCLE)
	_check(bool(rpg.get("completed", false)) and (rpg.get("deltas", {}) as Dictionary).is_empty(),
		"RPG_BAT preview exits with no deltas")
	_leave_refused_family(state)

	# 2. TACO (native 0x90): the synthetic monolith is refused with saved state,
	# including neutral frames, and writes nothing to the save.
	controller.start_overlay(state, 0x90, {"waves_per_stage": 1, "target_stages": 1})
	_assert_refused(session, state, [0x90], "TACO", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_TACO],
		[0, OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CROSS])
	for key: String in ["high_scores", "high_score_names", "stages_cleared", "score", "total_score", "menu_hit_3", "system_data_flag", "subgame_return_payload"]:
		_check(not state.rule_state.has(key), "Refused TACO leaves no %s in rule_state" % key)
	_leave_refused_family(state)

	# 3. TT (native 0xD0): refused the same way.
	controller.start_overlay(state, 0xD0, {"target_stages": 1, "boss_hp": 16})
	_assert_refused(session, state, [0xD0], "TT", OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_TT],
		[0, OverlayDispatcher.PAD_UP, OverlayDispatcher.PAD_SQUARE])
	for key: String in ["tt_stages_cleared", "tt_last_time_frames", "tt_rankings"]:
		_check(not state.rule_state.has(key), "Refused TT leaves no %s in rule_state" % key)
	_leave_refused_family(state)

	# Negative: a completed preview carrying a forged payload still writes nothing.
	var forged: Dictionary = {"completed": true, "high_scores": [1, 2, 3], "tt_rankings": [1], "rpg_result_code": 4, "loot_rewards": [1],
		"return_payload": {"score": 9}}
	var before_forged: Dictionary = state.rule_state.duplicate(true)
	controller._apply_overlay_deltas(state, forged)
	_check(state.rule_state == before_forged, "minigame delta keys are never written to rule_state")

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
	# T-39006: TEL_ACTION reads the invented preview roster, so saved-state input is refused.
	var step_res: Dictionary = boot.step_active_overlay(OverlayDispatcher.PAD_DOWN)
	_check(String(step_res.get("error", "")).contains(OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_TEL_ACTION])
		and not bool(step_res.get("completed", true)) and (step_res.get("deltas", {}) as Dictionary).is_empty()
		and int(state.rule_state["overlay_state"].get("girl_id", -1)) == 0,
		"PlayableBoot.step_active_overlay refuses saved-state TEL_ACTION input and keeps girl_id 0")

	# Native 0x56/0x57/0x59/0x5A entered from their queue events render and step
	# GEKO on the live PlayableBoot path, never the RPG_BAT/TACO/TT ids they share.
	for pair: Array in [[0x32, 0x57], [0x33, 0x56], [0x44, 0x59], [0x46, 0x5A]]:
		_leave_refused_family(state)
		var q_ovl: Dictionary = controller.start_overlay(state, int(pair[1]), {}, [int(pair[0]), 2, 0, 0])
		_check(state.game_mode == int(pair[1]) and int(q_ovl.get("dispatcher_mode", -1)) == OverlayDispatcher.GAME_MODE_GEKO,
			"queue 0x%02X enters native 0x%02X with dispatcher GEKO" % pair)
		_check(String(boot.overlay_dispatcher.describe_native_mode(int(pair[1]), q_ovl).get("overlay", "")) == "GEKO",
			"describe_native_mode(0x%02X) after queue 0x%02X is GEKO" % [int(pair[1]), int(pair[0])])
		_check(String(boot.overlay_dispatcher.describe_native_mode(int(pair[1])).get("overlay", "")) == "GEKO",
			"describe_native_mode(0x%02X) without state is GEKO" % int(pair[1]))
		boot._last_rendered_image = null
		var q_err: String = boot._present_committed()
		var live_img: Image = boot._last_rendered_image
		var geko_img: Image = boot.overlay_dispatcher.render_mode(OverlayDispatcher.GAME_MODE_GEKO, boot._build_overlay_render_state(state)).get("image")
		_check(q_err.is_empty() and live_img != null and geko_img != null and live_img.get_data() == geko_img.get_data(),
			"PlayableBoot renders the GEKO frame for queue 0x%02X / native 0x%02X" % pair)
		var bare_state: Dictionary = boot._build_overlay_render_state(state)
		bare_state.erase("dispatcher_mode")
		bare_state.erase("event_opcode")
		var bare_img: Image = boot.overlay_dispatcher.render_native_mode(int(pair[1]), bare_state).get("image")
		_check(bare_img != null and bare_img.get_data() == geko_img.get_data(),
			"render_native_mode(0x%02X) without a resolved id still renders GEKO" % int(pair[1]))
		var q_step: Dictionary = boot.step_active_overlay(OverlayDispatcher.PAD_CIRCLE)
		_check(String(q_step.get("error", "")).contains(OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_GEKO]),
			"saved-state step of native 0x%02X is refused as GEKO, not a minigame" % int(pair[1]))
	_leave_refused_family(state)

	# Negative: step_overlay with a completed TT/TACO state and saved state
	# writes nothing, refused or not. The OPTION case is a production menu
	# mode, so it reaches step_mode and the completion path (it needs the
	# opened resources, so this runs on the PlayableBoot dispatcher).
	for forged_mode: int in [0xD0, 0x90, 0x36]:
		var forged_ovl: Dictionary = controller.overlay_dispatcher.init_overlay(forged_mode, state.rule_state)
		forged_ovl["completed"] = true
		forged_ovl["tt_stages_cleared"] = 4
		forged_ovl["return_payload"] = {"tt_stages_cleared": 4, "rankings": [1], "elapsed_frames": 9, "high_scores": [1, 2, 3], "stages_cleared": 3}
		var rs_before: Dictionary = state.rule_state.duplicate(true)
		for pad: int in [OverlayDispatcher.PAD_CIRCLE, OverlayDispatcher.PAD_CROSS, OverlayDispatcher.PAD_START]:
			var forged_out: Dictionary = controller.overlay_dispatcher.step_overlay(forged_mode, forged_ovl, pad, state.rule_state)
			_check((forged_out.get("deltas", {}) as Dictionary).is_empty(),
				"completed forged payload in native 0x%02X yields no deltas (pad 0x%04X)" % [forged_mode, pad])
			if forged_mode == 0x36:
				_check(bool(forged_out.get("completed", false)) and String(forged_out.get("action", "")) != "rejected",
					"forged OPTION state reaches the completion path (pad 0x%04X)" % pad)
		_check(state.rule_state == rs_before, "completed forged payload in native 0x%02X leaves rule_state unchanged" % forged_mode)
	boot.canvas.set("_textures", {})
	boot.canvas.set("_commands", [])
	boot.canvas.free()
	boot.free()

