extends SceneTree
## T-29030 / T-29020 / T-29028 / Gate G3:
## Reachable Gameplay Families Live Entry, Interactive Multi-Frame Stepping,
## Outcome Writeback, 320x240 Rendering, and Clean Scheduler Return Gate
## + 3-List G3 Reachability Reconciliation (NativeRouteRegistry 68 modes,
## DayEventModes scheduler opcodes, and SchoolDayController/OverlayDispatcher).

const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Registry = preload("res://core/native_route_registry.gd")
const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const Calendar = preload("res://rules/calendar.gd")
const OrdinaryWeek = preload("res://rules/ordinary_week.gd")
const SchoolDay = preload("res://rules/school_day.gd")
const WeekEnd = preload("res://rules/week_end.gd")
const DayEventModes = preload("res://rules/day_event_modes.gd")
const OverlayDispatcher = preload("res://app/overlay_dispatcher.gd")
const TelephoneOverlay = preload("res://overlays/telephone_overlay.gd")
const DateOverlay = preload("res://overlays/date_overlay.gd")
const EndingOverlay = preload("res://overlays/ending_overlay.gd")
const SchoolEventsOverlay = preload("res://overlays/school_events_overlay.gd")
const SubgameOverlays = preload("res://overlays/subgame_overlays.gd")
const EventCgOverlay = preload("res://overlays/event_cg_scenes_overlay.gd")
const SystemOverlays = preload("res://overlays/system_overlays.gd")
const RuleFixtures = preload("res://tests/rules/run_rule_tests.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")

const MANIFEST_PATH := "/workspace/.asset-cache/port/school-day-tables-v1-final/manifest.json"
const RECONCILIATION_JSON_PATH := "/workspace/.claude/orchestrator/state/T-29030/reachable-families-live-entry-and-return-gate-v1/g3-reachability-reconciliation.json"

const PAD_UP := OverlayDispatcher.PAD_UP
const PAD_RIGHT := OverlayDispatcher.PAD_RIGHT
const PAD_DOWN := OverlayDispatcher.PAD_DOWN
const PAD_LEFT := OverlayDispatcher.PAD_LEFT
const PAD_CIRCLE := OverlayDispatcher.PAD_CIRCLE
const PAD_CROSS := OverlayDispatcher.PAD_CROSS
const PAD_SQUARE := OverlayDispatcher.PAD_SQUARE
const PAD_START := OverlayDispatcher.PAD_START

var _pass_count: int = 0
var _fail_count: int = 0
var _family_evidence: Dictionary = {}


class InGameEtcOverlay extends SystemOverlays.TitleMenuOverlay:
	func process_input(pad_pressed: int) -> Dictionary:
		var res: Dictionary = super.process_input(pad_pressed)
		if (pad_pressed & 0x0020) != 0 or (pad_pressed & 0x0040) != 0:
			res["action"] = "return_to_game"
		return res


class InGameOptionOverlay extends SystemOverlays.OptionsOverlay:
	func process_input(pad_pressed: int) -> Dictionary:
		var res: Dictionary = super.process_input(pad_pressed)
		if String(res.get("action", "")) == "exit":
			res["action"] = "return_to_game"
		return res


func _initialize() -> void:
	call_deferred("_run")


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
	print("=== Reachable Gameplay Families Live Entry & Return Gate (G3 / T-29030) ===")
	var tables: Dictionary = _load_tables(MANIFEST_PATH)
	if tables.is_empty():
		var fixtures := RuleFixtures.new()
		tables = fixtures.synthetic_tables()
	_check(not tables.is_empty(), "Loaded rule tables for G3 gate")

	_test_a_telephone_profile_invitation(tables)
	_test_b_date_choice_cg_ride_movie(tables)
	_test_c_walk_home_and_encounter(tables)
	_test_d_health_rest_and_club_commands(tables)
	_test_e_seasonal_and_school_events(tables)
	_test_f_borrowed_system_overlays(tables)
	_test_g_subgames_live_entry_and_return(tables)
	_test_h_graduation_and_ending_flows(tables)
	_reconcile_and_emit_g3_lists()

	print("=== G3 Reachable Gameplay Families Gate Summary: %d PASS, %d FAIL ===" % [_pass_count, _fail_count])
	quit(0 if _fail_count == 0 else 1)


func _check(cond: bool, msg: String) -> void:
	if cond:
		_pass_count += 1
	else:
		_fail_count += 1
		push_error("FAIL: " + msg)
		print("  FAIL: " + msg)


func _image_diff_pixels(img_a: Image, img_b: Image) -> int:
	if img_a == null or img_b == null:
		return 0
	if img_a.get_width() != img_b.get_width() or img_a.get_height() != img_b.get_height():
		return -1
	var diffs: int = 0
	for y in range(0, img_a.get_height(), 2):
		for x in range(0, img_a.get_width(), 2):
			if img_a.get_pixel(x, y) != img_b.get_pixel(x, y):
				diffs += 1
	return diffs


func _create_live_controller_session(tables: Dictionary) -> Dictionary:
	var fixtures := RuleFixtures.new()
	var state: State = fixtures.synthetic_state()
	state.rule_state["allow_multi_week"] = true
	state.rule_state["year"] = 95
	state.rule_state["month"] = 5
	state.rule_state["day"] = 14
	state.rule_state["weekday"] = 6
	state.rule_state["params"] = [100 << 16, 120 << 16, 120 << 16, 120 << 16, 120 << 16, 150 << 16, 130 << 16, 100 << 16, 10 << 16]
	state.rule_state["girls"][0]["affection_word"] = 90 << 16
	state.rule_state["girls"][0]["tokimeki_word"] = 90 << 16
	state.rule_state["girls"][0]["flags"] = 2
	Calendar.populate_month_calendar(state.rule_state, 95, 5)

	var repo_root: String = _resolve_repo_root()
	var hud_manifest: String = repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var font_root: String = repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var date_pack: String = repo_root.path_join(".asset-cache/port/date-preview-v1")

	var controller := Controller.new(tables)
	var err_open: String = controller.overlay_dispatcher.open_resources(font_root, hud_manifest, date_pack)
	_check(err_open.is_empty(), "controller.overlay_dispatcher.open_resources() succeeds (%s)" % err_open)

	var session := Session.new()
	session.rng.configure(Rng.OPENBIOS_PROFILE, 42)
	var err_attach: String = controller.attach_multi_week(session, state)
	_check(err_attach.is_empty(), "controller.attach_multi_week() succeeds")
	session.draw_logic = _empty
	session.cd_logic = _empty
	session.audio_logic = _empty
	return {
		"controller": controller,
		"state": state,
		"session": session,
	}


func _queue_day_event_and_step(ctx: Dictionary, opcode: int, arg1: int = 0, arg2: int = 0, arg3: int = 0, start_idx: int = 0) -> int:
	var state: State = ctx["state"]
	var session: Session = ctx["session"]
	var queue: Array = []
	while queue.size() < 32:
		queue.append([0xFF, 0, 0, 0])
	if start_idx > 0:
		queue[0] = [0x00, 0, 0, 15]
	queue[start_idx] = [opcode, arg1, arg2, arg3]
	state.rule_state["day_events"] = queue
	state.rule_state["day_event_index"] = start_idx
	state.reset_game_mode_fields(0x31)

	var res: Session.FrameResult = _step_pad(session, 0)
	_check(res.error.is_empty(), "Queue dispatch for event 0x%02X has no error (%s)" % [opcode, res.error])
	return state.game_mode


## T-39006: an event family without a source-backed production handler is
## refused when saved state is present. Every input must fail the frame, keep
## the native mode, leave saved state (overlay included) unchanged and carry no
## deltas. The overlay must already be initialized (a neutral frame or
## start_overlay).
func _assert_production_rejection(ctx: Dictionary, native_mode: int, label: String, pads: Array = [PAD_CIRCLE, PAD_CROSS, PAD_DOWN]) -> void:
	var state: State = ctx["state"]
	var session: Session = ctx["session"]
	var controller: Controller = ctx["controller"]
	var before: Dictionary = state.rule_state.duplicate(true)
	for pad: int in pads:
		var res: Session.FrameResult = _step_pad(session, pad)
		_check(res.error.contains("refused with saved state"),
			"%s refuses saved-state input 0x%04X in production (%s)" % [label, pad, res.error])
		_check(state.game_mode == native_mode and state.rule_state == before,
			"%s refusal keeps mode 0x%02X and leaves saved state unchanged" % [label, native_mode])
	var direct: Dictionary = controller.overlay_dispatcher.step_overlay(native_mode,
		state.rule_state.get("overlay_state", {}), PAD_CIRCLE, state.rule_state, session.rng)
	_check(not bool(direct.get("completed", true)) and (direct.get("deltas", {}) as Dictionary).is_empty()
		and state.rule_state == before, "%s refusal completes nothing and carries no deltas" % label)


## A refused family has no production exit. The gate drops its overlay and puts
## the harness back on the day screen (0x31) before testing the next family.
func _leave_refused_family(state: State) -> void:
	state.rule_state.erase("overlay_state")
	state.reset_game_mode_fields(0x31)


func _preview_rng() -> Rng:
	var rng := Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 42)
	return rng


## Preview stepping (no saved state) moves an overlay for rendering checks
## only. It never writes saved state.
func _preview(controller: Controller, mode: int, overlay_state: Dictionary, pads: Array) -> Dictionary:
	var current: Dictionary = overlay_state.duplicate(true)
	for pad: int in pads:
		current = controller.overlay_dispatcher.step_overlay(mode, current, pad, {}, _preview_rng())["state"]
	return current


## An entered family refused in production: checks the refusal, then a preview
## run that populates `key`, renders 320x240 and exits without rewards.
func _check_refused_family(ctx: Dictionary, native_mode: int, render_mode_id: int, label: String, key: String, key_pads: Array, exit_pads: Array, refused_pads: Array = [PAD_CIRCLE, PAD_CROSS, PAD_DOWN]) -> void:
	var state: State = ctx["state"]
	var controller: Controller = ctx["controller"]
	var entry: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, native_mode, label, refused_pads)
	var shown: Dictionary = _preview(controller, native_mode, entry, key_pads)
	_check(shown.has(key), "%s preview populates %s" % [label, key])
	var img: Image = controller.overlay_dispatcher.render_mode(render_mode_id, shown).get("image")
	_check(img != null and img.get_size() == Vector2i(320, 240), "%s renders 320x240" % label)
	_check_preview_exit_without_rewards(controller, native_mode, shown, exit_pads, label)
	_leave_refused_family(state)


## Steps a preview to its exit and checks no step carries reward deltas.
func _check_preview_exit_without_rewards(controller: Controller, mode: int, overlay_state: Dictionary, pads: Array, label: String) -> void:
	var current: Dictionary = overlay_state.duplicate(true)
	var last: Dictionary = {}
	var deltas_empty: bool = true
	for pad: int in pads:
		last = controller.overlay_dispatcher.step_overlay(mode, current, pad, {}, _preview_rng())
		deltas_empty = deltas_empty and (last.get("deltas", {}) as Dictionary).is_empty()
		current = last["state"]
	_check(bool(last.get("completed", false)) and deltas_empty, "%s preview exits without reward deltas" % label)


# ---------------------------------------------------------------------------
# (a) Telephone / Profile / Invitation (0x3D TEL, 0x3E TEL_ACTION, native 0x43, 0x45; event 0x46 is GEKO)
# ---------------------------------------------------------------------------
func _test_a_telephone_profile_invitation(tables: Dictionary) -> void:
	print("--- [G3.a] Telephone / Profile / Invitation (0x3D TEL, 0x3E TEL_ACTION, native 0x43, 0x45, event 0x46) ---")
	var ctx: Dictionary = _create_live_controller_session(tables)
	var controller: Controller = ctx["controller"]
	var state: State = ctx["state"]
	var session: Session = ctx["session"]

	# 1. Notebook Profile Appraisal (native 0x45 -> dispatcher 0x3E GAME_MODE_TEL_ACTION)
	controller.start_overlay(state, 0x45, {"girl_id": 0})
	_check(state.game_mode == 0x45 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x3E,
		"Entered native mode 0x45 -> dispatcher 0x3E (GAME_MODE_TEL_ACTION)")

	for _n in range(3):
		var res_wait: Session.FrameResult = _step_pad(session, 0)
		_check(res_wait.error.is_empty() and state.game_mode == 0x45, "TEL_ACTION (0x45) waits on neutral input")

	# T-39006: TEL_ACTION has no source-backed production handler; with saved state every input is refused.
	var ovl_prof_entry: Dictionary = state.rule_state.get("overlay_state", {}).duplicate(true)
	_assert_production_rejection(ctx, 0x45, "TEL_ACTION (0x45)")
	var ovl_prof: Dictionary = _preview(controller, 0x45, ovl_prof_entry, [PAD_DOWN])
	_check(int(ovl_prof.get("cursor", -1)) == 1 and int(ovl_prof.get("girl_id", -1)) == 1, "TEL_ACTION preview cursor moved to girl 1")
	var ren_prof: Dictionary = controller.overlay_dispatcher.render_mode(0x45, ovl_prof)
	var img_prof: Image = ren_prof.get("image")
	_check(ren_prof.get("error", "") == "" and img_prof != null and img_prof.get_size() == Vector2i(320, 240),
		"TEL_ACTION renders 320x240 notebook profile screen")

	_check_preview_exit_without_rewards(controller, 0x45, ovl_prof, [PAD_CIRCLE, PAD_CIRCLE], "TEL_ACTION")
	_leave_refused_family(state)

	# 2. Girl invitation via queue event 0x46 -> native 0x5A. overlay_loader.c enters
	# D_801426D0, the GEKO invitation handler (src/ovl/girl_encounters/invite_entry.c),
	# so the dispatcher id is GEKO, not TEL; GEKO is refused with saved state.
	var aff0_before: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var tok0_before: int = State.high(int(state.rule_state["girls"][0]["tokimeki_word"]))
	state.rule_state["appointment_words"] = [0, 0, 0, 0, 0, 0, 0, 0]
	var mode_inv: int = _queue_day_event_and_step(ctx, 0x46, 0, 0, 0)
	_check(mode_inv == 0x5A and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == OverlayDispatcher.GAME_MODE_GEKO,
		"Queue event 0x46 dispatches live to native 0x5A / dispatcher 0x44 (GEKO invitation)")
	for _n in range(3):
		var res_wait_inv: Session.FrameResult = _step_pad(session, 0)
		_check(res_wait_inv.error.is_empty() and state.game_mode == 0x5A, "GEKO invitation (0x5A) waits on neutral input")
	var inv_res: Session.FrameResult = _step_pad(session, PAD_CIRCLE)
	_check(inv_res.error.contains(OverlayDispatcher.PRODUCTION_REJECTIONS[OverlayDispatcher.GAME_MODE_GEKO]),
		"GEKO invitation (0x5A) is refused as GEKO, not TEL (%s)" % inv_res.error)
	_assert_production_rejection(ctx, 0x5A, "GEKO invitation (0x5A)")
	_leave_refused_family(state)

	# 3. Telephone (native 0x43 GAME_MODE_TELEPHONE -> dispatcher 0x3D GAME_MODE_TEL)
	controller.start_overlay(state, 0x43)
	_check(state.game_mode == 0x43 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x3D,
		"Entered native mode 0x43 -> dispatcher 0x3D (GAME_MODE_TEL)")

	for _n in range(3):
		var res_wait2: Session.FrameResult = _step_pad(session, 0)
		_check(res_wait2.error.is_empty() and state.game_mode == 0x43, "GAME_MODE_TEL waits on neutral input")

	# T-39006: TEL has no source-backed production handler (phone_date_invitation_resolve writes are
	# not ported). With saved state it is refused: no appointment, no affection or tokimeki write.
	var ovl_tel: Dictionary = state.rule_state.get("overlay_state", {}).duplicate(true)
	var appointments_before: Array = (state.rule_state["appointment_words"] as Array).duplicate()
	_assert_production_rejection(ctx, 0x43, "GAME_MODE_TEL (0x43)")
	_check(state.rule_state["appointment_words"] == appointments_before
		and State.high(int(state.rule_state["girls"][0]["affection_word"])) == aff0_before
		and State.high(int(state.rule_state["girls"][0]["tokimeki_word"])) == tok0_before,
		"GAME_MODE_TEL refusal books no appointment and writes no affection or tokimeki")

	# Preview stepping (no saved state) drives the screens for rendering checks only.
	# Stage 0: place_select -> move cursor down, render place menu, confirm place
	var ovl_place: Dictionary = _preview(controller, 0x43, ovl_tel, [PAD_DOWN])
	_check(String(ovl_place.get("subscreen", "")) == "place_select" and int(ovl_place.get("cursor", -1)) == 1,
		"TEL preview stage 0 place_select cursor moved to 1")
	var img_place: Image = controller.overlay_dispatcher.render_mode(0x3D, ovl_place).get("image")
	_check(img_place != null and img_place.get_size() == Vector2i(320, 240), "TEL place_select renders 320x240 image")

	# Confirm place -> Stage 1: day_select
	var ovl_day0: Dictionary = _preview(controller, 0x43, ovl_place, [PAD_CIRCLE])
	_check(int(ovl_day0.get("stage", -1)) == 1 and String(ovl_day0.get("subscreen", "")) == "day_select",
		"TEL advanced to stage 1 (day_select)")
	var img_day0: Image = controller.overlay_dispatcher.render_mode(0x3D, ovl_day0).get("image")

	# Move cursor on calendar day slots and verify green calendar highlight + hand cursor shift pixels
	var ovl_day1: Dictionary = _preview(controller, 0x43, ovl_day0, [PAD_DOWN])
	var img_day1: Image = controller.overlay_dispatcher.render_mode(0x3D, ovl_day1).get("image")
	_check(_image_diff_pixels(img_day0, img_day1) > 0,
		"TEL day_select cursor movement shifts green calendar highlight + hand cursor pixels")

	# Confirm day -> Stage 2: call_response
	# The preview's invitation evaluation is not asserted: its inputs are invented.
	var ovl_call: Dictionary = _preview(controller, 0x43, ovl_day1, [PAD_CIRCLE])
	_check(int(ovl_call.get("stage", -1)) == 2 and String(ovl_call.get("subscreen", "")) == "call_response",
		"TEL preview advanced to stage 2 (call_response)")

	# Verify call_response renders 320x240 and actor2/frame1 prompt marker toggles pixels
	var ovl_marker_off: Dictionary = ovl_call.duplicate(true)
	ovl_marker_off["native_prompt"] = {
		"schema": "tokimemo.phone-wait-marker-view.v1",
		"source_function": "talk_text_step",
		"waiting_for_input": true,
		"k_disp_inc_result": 1,
		"voice_mode": 0,
		"speaker_voice_state": 0,
		"marker_visible": false,
		"marker_native_y": 0x48,
	}
	var ovl_marker_on: Dictionary = ovl_call.duplicate(true)
	ovl_marker_on["native_prompt"] = {
		"schema": "tokimemo.phone-wait-marker-view.v1",
		"source_function": "talk_text_step",
		"waiting_for_input": true,
		"k_disp_inc_result": 1,
		"voice_mode": 0,
		"speaker_voice_state": 0,
		"marker_visible": true,
		"marker_native_y": 0x48,
	}
	var img_marker_off: Image = controller.overlay_dispatcher.render_mode(0x3D, ovl_marker_off).get("image")
	var img_marker_on: Image = controller.overlay_dispatcher.render_mode(0x3D, ovl_marker_on).get("image")
	_check(_image_diff_pixels(img_marker_off, img_marker_on) > 0, "TEL call_response actor2/frame1 prompt marker toggles pixels")

	_check_preview_exit_without_rewards(controller, 0x43, ovl_call, [PAD_CIRCLE], "GAME_MODE_TEL")
	_leave_refused_family(state)

	_family_evidence["telephone_profile_invitation"] = {
		"native_modes": ["0x43", "0x45", "0x5A"],
		"day_event_opcodes": ["0x46"],
		"dispatcher_modes": ["0x3D", "0x3E", "0x44"],
		"source_functions": ["telephone_main", "phone_menu_step", "phone_evaluate_invitation", "phone_book_appointment"],
		"entry_and_caller": "start_overlay 0x43/0x45 -> SchoolDayController._overlay_mode -> OverlayDispatcher; queue 0x46 -> native 0x5A enters GEKO (D_801426D0), not TEL",
		"required_content_services": ["OverlayDispatcher.telephone_presenter", "TelephoneOverlay.GIRL_PROFILES", "TelephoneOverlay.PHONE_INVITATION_BASE_ADJUSTMENTS"],
		"return_and_state_effects": "T-39006: refused with saved state (no appointment, affection or tokimeki write); preview steps render and exit without rewards",
		"verified_live": false,
	}


# ---------------------------------------------------------------------------
# (b) Date / Choice / Special CG / Ride Movie (0x50 DATE, 0x58 AMUSEMENT_PARK_DATE, 0x5D/0x5E EVENT_CG, events 0x80, 0x49, 0x45)
# ---------------------------------------------------------------------------
func _test_b_date_choice_cg_ride_movie(tables: Dictionary) -> void:
	print("--- [G3.b] Date / Choice / Special CG / Ride Movie (0x50 DATE, 0x58 AMUSEMENT_PARK_DATE, 0x5E EVENT_CG) ---")
	var ctx: Dictionary = _create_live_controller_session(tables)
	var controller: Controller = ctx["controller"]
	var state: State = ctx["state"]
	var session: Session = ctx["session"]

	# 1. Live scheduler dispatch for event 0x80 -> native 0x50 (GAME_MODE_DATE)
	var aff_before: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var tok_before: int = State.high(int(state.rule_state["girls"][0]["tokimeki_word"]))
	var dc_before: int = int(state.rule_state["girls"][0]["counters"][1])
	var mode_entered: int = _queue_day_event_and_step(ctx, 0x80, 0, DateOverlay.SPOT_AQUARIUM, 0)
	_check(mode_entered == 0x50, "Day event 0x80 dispatches live to native mode 0x50 (GAME_MODE_DATE)")

	for _n in range(2):
		_step_pad(session, 0)
		_check(state.game_mode == 0x50, "GAME_MODE_DATE (0x50) waits on neutral input frames")

	var img_arrival: Image = controller.overlay_dispatcher.render_mode(0x50, state.rule_state["overlay_state"]).get("image")
	_check(img_arrival != null and img_arrival.get_size() == Vector2i(320, 240), "GAME_MODE_DATE stage 0 arrival renders 320x240")

	# T-39006: date spot state comes from DateOverlay.create_default_state, not saved state, so
	# with saved state the date is refused: no affection, tokimeki or date_count write.
	var ovl_date: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x50, "GAME_MODE_DATE (0x50)")
	_check(State.high(int(state.rule_state["girls"][0]["affection_word"])) == aff_before
		and State.high(int(state.rule_state["girls"][0]["tokimeki_word"])) == tok_before
		and int(state.rule_state["girls"][0]["counters"][1]) == dc_before,
		"GAME_MODE_DATE refusal writes no affection, tokimeki or date_count")

	# Preview stepping (no saved state): 3-choice prompt with 3 distinct cursor positions (actor0 selection cursor)
	var ovl_c0: Dictionary = _preview(controller, 0x50, ovl_date, [PAD_CIRCLE])
	_check(int(ovl_c0.get("stage", -1)) == 1, "GAME_MODE_DATE preview advances to stage 1 (3-choice prompt)")
	var ovl_c1: Dictionary = _preview(controller, 0x50, ovl_c0, [PAD_DOWN])
	var ovl_c2: Dictionary = _preview(controller, 0x50, ovl_c1, [PAD_DOWN])
	var img_c0: Image = controller.overlay_dispatcher.render_mode(0x50, ovl_c0).get("image")
	var img_c1: Image = controller.overlay_dispatcher.render_mode(0x50, ovl_c1).get("image")
	var img_c2: Image = controller.overlay_dispatcher.render_mode(0x50, ovl_c2).get("image")
	_check(_image_diff_pixels(img_c0, img_c1) > 0 and _image_diff_pixels(img_c1, img_c2) > 0 and _image_diff_pixels(img_c0, img_c2) > 0,
		"GAME_MODE_DATE 3-choice prompt renders 3 distinct actor0 selection cursor frames")

	# Select choice 0 (wrap 2 -> 0) and advance to stage 2; the preview exit carries no rewards.
	var ovl_react: Dictionary = _preview(controller, 0x50, ovl_c2, [PAD_DOWN, PAD_CIRCLE])
	_check(int(ovl_react.get("stage", -1)) == 2, "GAME_MODE_DATE preview advances to stage 2 (spot reaction)")
	var img_react: Image = controller.overlay_dispatcher.render_mode(0x50, ovl_react).get("image")
	_check(img_react != null and img_react.get_size() == Vector2i(320, 240), "GAME_MODE_DATE stage 2 reaction renders 320x240")
	_check_preview_exit_without_rewards(controller, 0x50, ovl_react, [PAD_CIRCLE], "GAME_MODE_DATE")
	_leave_refused_family(state)

	# 2. Amusement Park / Karaoke Date2 (event 0x49 -> native 0x58 -> GAME_MODE_AMUSEMENT_PARK_DATE)
	var mode_amuse: int = _queue_day_event_and_step(ctx, 0x49, 3, 0, 0)
	_check(mode_amuse == 0x58, "Day event 0x49 dispatches live to native mode 0x58 (GAME_MODE_AMUSEMENT_PARK_DATE)")
	_step_pad(session, 0)
	_check(state.game_mode == 0x58, "AMUSEMENT_PARK_DATE waits on neutral input")
	var ovl_amuse_entry: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x58, "AMUSEMENT_PARK_DATE (0x58)")
	ovl_amuse_entry["subscene_kind"] = "amusement_coaster"
	var ovl_amuse: Dictionary = _preview(controller, 0x58, ovl_amuse_entry, [PAD_CIRCLE])
	_check(ovl_amuse.has("coaster_score"), "AMUSEMENT_PARK_DATE preview evaluates coaster choice score")
	var img_amuse: Image = controller.overlay_dispatcher.render_mode(0x58, ovl_amuse).get("image")
	_check(img_amuse != null and img_amuse.get_size() == Vector2i(320, 240), "AMUSEMENT_PARK_DATE renders 320x240")
	_check_preview_exit_without_rewards(controller, 0x58, ovl_amuse, [PAD_CIRCLE], "AMUSEMENT_PARK_DATE")
	_leave_refused_family(state)

	# 3. Special Event CG & Ride Movie (event 0x45 -> native 0x5D / 0x5E -> GAME_MODE_EVENT_CG)
	var mode_cg: int = _queue_day_event_and_step(ctx, 0x45, 0, 0, 0)
	_check(mode_cg == 0x5D and int(state.rule_state.get("overlay_state", {}).get("dispatcher_mode", -1)) == 0x5E,
		"Day event 0x45 dispatches live to native mode 0x5D -> dispatcher 0x5E (GAME_MODE_EVENT_CG)")
	_step_pad(session, 0)
	_check(state.game_mode == 0x5D, "GAME_MODE_EVENT_CG waits on neutral input")
	var ovl_cg_entry: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x5D, "GAME_MODE_EVENT_CG (0x5D)")
	ovl_cg_entry["scene_kind"] = "shiori_cg"
	ovl_cg_entry["spot_place"] = EventCgOverlay.SPOT_KIRAMEKI_CENTRAL_PARK
	var ovl_cg0: Dictionary = _preview(controller, 0x5D, ovl_cg_entry, [PAD_CIRCLE])
	var ovl_cg1: Dictionary = _preview(controller, 0x5D, ovl_cg0, [PAD_DOWN])
	var img_cg0: Image = controller.overlay_dispatcher.render_mode(0x5E, ovl_cg0).get("image")
	var img_cg1: Image = controller.overlay_dispatcher.render_mode(0x5E, ovl_cg1).get("image")
	_check(_image_diff_pixels(img_cg0, img_cg1) > 0, "GAME_MODE_EVENT_CG choice cursor shifts pixels")
	var ovl_cg2: Dictionary = _preview(controller, 0x5D, ovl_cg1, [PAD_CIRCLE])
	_check(ovl_cg2.has("choice_result"), "GAME_MODE_EVENT_CG preview records special scene choice_result")
	_check_preview_exit_without_rewards(controller, 0x5D, ovl_cg2, [PAD_CIRCLE], "GAME_MODE_EVENT_CG")
	_leave_refused_family(state)

	# Ride movie LBA dispatch check on 0x5E (refused with saved state; LBA checked in preview)
	controller.start_overlay(state, 0x5E, {"girl_id": 0, "scene_kind": "ride_movie", "ride_id": 2})
	var ovl_ride_entry: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x5E, "GAME_MODE_EVENT_CG ride_movie (0x5E)")
	var ovl_ride: Dictionary = _preview(controller, 0x5E, ovl_ride_entry, [PAD_CIRCLE])
	var ride_info: Dictionary = ovl_ride.get("ride_movie", {})
	_check(int(ride_info.get("lba", 0)) > 0, "GAME_MODE_EVENT_CG ride_movie dispatches valid STR movie LBA")
	var img_ride: Image = controller.overlay_dispatcher.render_mode(0x5E, ovl_ride).get("image")
	_check(img_ride != null and img_ride.get_size() == Vector2i(320, 240), "GAME_MODE_EVENT_CG ride_movie renders 320x240")
	_check_preview_exit_without_rewards(controller, 0x5E, ovl_ride, [PAD_CROSS], "GAME_MODE_EVENT_CG ride_movie")
	_leave_refused_family(state)

	_family_evidence["date_choice_cg_ride_movie"] = {
		"native_modes": ["0x50", "0x58", "0x5D", "0x5E"],
		"day_event_opcodes": ["0x45", "0x49", "0x80"],
		"dispatcher_modes": ["0x50", "0x58", "0x5E"],
		"source_functions": ["date_main", "date_setup_spot", "date_arrival_and_looks_check", "date_choice_score", "date2_main", "event_cg_main"],
		"entry_and_caller": "DayEventModes 0x80/0x49/0x45 -> native 0x50/0x58/0x5D -> SchoolDayController._overlay_mode -> OverlayDispatcher",
		"required_content_services": ["OverlayDispatcher.date_presenter", "OverlayDispatcher.event_cg_presenter", "DateOverlay", "EventCgOverlay"],
		"return_and_state_effects": "T-39006: DATE, AMUSEMENT_PARK_DATE and EVENT_CG are refused with saved state (no affection, tokimeki or date_count write); preview steps render and exit without rewards",
		"verified_live": false,
	}


# ---------------------------------------------------------------------------
# (c) Walk-Home & Encounter (event 0x44 -> native 0x59 -> dispatcher 0x44 GEKO)
# ---------------------------------------------------------------------------
func _test_c_walk_home_and_encounter(tables: Dictionary) -> void:
	print("--- [G3.c] Walk-Home & After-School Encounter (event 0x44 -> native 0x59 / dispatcher 0x44 GEKO) ---")
	var ctx: Dictionary = _create_live_controller_session(tables)
	var controller: Controller = ctx["controller"]
	var state: State = ctx["state"]
	var session: Session = ctx["session"]

	var aff_before: int = State.high(int(state.rule_state["girls"][0]["affection_word"]))
	var tok_before: int = State.high(int(state.rule_state["girls"][0]["tokimeki_word"]))
	# Note: in OrdinaryWeek.dispatch, walk-home (0x44) requires day_event_index > 0 (preceded by weekday activity)
	var mode_entered: int = _queue_day_event_and_step(ctx, 0x44, 0, 0, 0, 1)
	_check(mode_entered == 0x59 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x44,
		"Day event 0x44 dispatches live to native mode 0x59 -> dispatcher 0x44 (GAME_MODE_GEKO)")

	for _n in range(2):
		_step_pad(session, 0)
		_check(state.game_mode == 0x59, "GAME_MODE_GEKO waits on neutral input")

	# T-39006: walk-home topic inputs are fixed and the roster is invented, so with saved state
	# GEKO is refused: no affection, tokimeki or syoushin write.
	var ovl_geko: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x59, "GAME_MODE_GEKO (0x59)")
	_check(State.high(int(state.rule_state["girls"][0]["affection_word"])) == aff_before
		and State.high(int(state.rule_state["girls"][0]["tokimeki_word"])) == tok_before,
		"GAME_MODE_GEKO refusal writes no affection or tokimeki")

	# Preview stepping (no saved state) for rendering only.
	var img_g0: Image = controller.overlay_dispatcher.render_mode(0x44, ovl_geko).get("image")
	var ovl_g1: Dictionary = _preview(controller, 0x59, ovl_geko, [PAD_DOWN])
	var img_g1: Image = controller.overlay_dispatcher.render_mode(0x44, ovl_g1).get("image")
	_check(img_g0 != null and img_g0.get_size() == Vector2i(320, 240) and _image_diff_pixels(img_g0, img_g1) > 0,
		"GAME_MODE_GEKO renders 320x240 and shifts invitation choice cursor pixels")

	var ovl_topic: Dictionary = _preview(controller, 0x59, ovl_g1, [PAD_UP, PAD_CIRCLE])
	_check(ovl_topic.has("walk_home_topic") and ovl_topic.has("walk_home_outcome"),
		"GAME_MODE_GEKO preview selects walk-home conversation topic & outcome")
	_check_preview_exit_without_rewards(controller, 0x59, ovl_topic, [PAD_CIRCLE], "GAME_MODE_GEKO")
	_leave_refused_family(state)

	_family_evidence["walk_home_encounter"] = {
		"native_modes": ["0x44", "0x59"],
		"day_event_opcodes": ["0x44"],
		"dispatcher_modes": ["0x44"],
		"source_functions": ["geko_main", "select_walk_home_topic", "apply_walk_home_choice", "finish_walk_home"],
		"entry_and_caller": "DayEventModes 0x44 -> native 0x59 -> dispatcher 0x44 -> SchoolDayController._overlay_mode",
		"required_content_services": ["OverlayDispatcher.encounters_ending_presenter", "EncountersOverlay"],
		"return_and_state_effects": "T-39006: refused with saved state (no affection, tokimeki or syoushin write); preview steps render and exit without rewards",
		"verified_live": false,
	}


# ---------------------------------------------------------------------------
# (d) Health / Rest & Club Commands (commands 0..7, command 4 club, command 7 rest)
# ---------------------------------------------------------------------------
func _test_d_health_rest_and_club_commands(tables: Dictionary) -> void:
	print("--- [G3.d] Health / Rest & Club Commands (commands 0..7, club 0..10 practice_points, stress recovery) ---")
	var fixtures := RuleFixtures.new()
	var state: State = fixtures.synthetic_state()
	state.rule_state["allow_multi_week"] = true

	# 1. Verify Rest command (command 7) reduces stress (params[8]) via SchoolDay.apply_parameters
	state.rule_state["params"][8] = 60 << 16
	var stress_before: int = State.high(int(state.rule_state["params"][8]))
	var err_rest: String = SchoolDay.apply_parameters(state, 7, 0, 0, tables)
	_check(err_rest.is_empty(), "SchoolDay.apply_parameters(command=7) succeeds (%s)" % err_rest)
	var stress_after: int = State.high(int(state.rule_state["params"][8]))
	_check(stress_after < stress_before, "Rest command 7 reduces player stress parameter (%d -> %d)" % [stress_before, stress_after])

	# 2. Verify Sick Rest background & SD resolution (condition_bucket=1, command=7)
	var sick_res: Dictionary = SchoolDay.resolve_bg_and_sd(7, 0, false, 1, 0, 0, 0)
	_check(int(sick_res.get("sd_lba", 0)) == SchoolDay.SICK_REST_SD_LBA,
		"Sick rest (condition_bucket=1, command=7) resolves SICK_REST_SD_LBA")

	# 3. Verify Club command (command 4) across all 11 clubs (0..10) + practice_points accumulation
	state.rule_state["clubs"] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	for club_id in range(11):
		state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~(0xF << 12)) | ((club_id & 0xF) << 12)
		var err_club: String = SchoolDay.apply_parameters(state, 4, 0, 0, tables)
		_check(err_club.is_empty(), "SchoolDay.apply_parameters(command=4, club=%d) succeeds" % club_id)
		var pts_before: int = int(state.rule_state["clubs"][club_id])
		SchoolDay.increment_practice_points(state.rule_state, club_id)
		_check(int(state.rule_state["clubs"][club_id]) == pts_before + 1,
			"SchoolDay.increment_practice_points increments club %d practice points" % club_id)

	_family_evidence["health_rest_and_club_commands"] = {
		"native_modes": ["0x34", "0x30", "0x31", "0x40", "0x32"],
		"day_event_opcodes": ["0x00", "0xFC", "0xFF"],
		"dispatcher_modes": [],
		"source_functions": ["command_screen_step", "school_day_apply_parameters", "increment_practice_points", "resolve_bg_and_sd"],
		"entry_and_caller": "SchoolDayController._command (0x34) -> _schedule (0x30) -> _day_screen (0x31) -> _school_day (0x40) -> _week_end (0x32)",
		"required_content_services": ["tokimemo.port-rule-tables.v1 (command_deltas, weekday_background_lbas, weekday_sd_lbas)", "SchoolDay.CLUB_ACTIVITY_DELTAS"],
		"return_and_state_effects": "Updates rule_state.params[0..8] (including stress reduction on command 7) and rule_state.clubs[0..10] practice points",
		"verified_live": true,
	}


# ---------------------------------------------------------------------------
# (e) Seasonal & School Events
# ---------------------------------------------------------------------------
func _test_e_seasonal_and_school_events(tables: Dictionary) -> void:
	print("--- [G3.e] Seasonal & School Events (Entrance/Semester, Sports, Culture, Exam, Camp, Trip, Holiday, Master, Stall) ---")
	var ctx: Dictionary = _create_live_controller_session(tables)
	var controller: Controller = ctx["controller"]
	var state: State = ctx["state"]
	var session: Session = ctx["session"]
	controller.overlay_dispatcher.title_overlay = InGameEtcOverlay.new()

	# 1. Entrance / Semester Ceremony Notice (event 0xFD -> native 0x39 -> dispatcher 0x3F ETC)
	var mode_notice: int = _queue_day_event_and_step(ctx, 0xFD, 0, 0, 0)
	_check(mode_notice == 0x39 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x3F,
		"Ceremony notice event 0xFD dispatches live to native 0x39 -> dispatcher 0x3F")
	_step_pad(session, 0)
	_check(state.game_mode == 0x39, "Ceremony notice (0x39) waits on neutral input")
	_step_pad(session, PAD_CIRCLE)
	_check(state.game_mode == 0x31, "Ceremony notice (0x39) returns cleanly to 0x31")

	# T-39006: the families below have no source-backed production handler yet (fixed inputs,
	# the invented roster or fixed rewards in step_mode). With saved state each one is refused
	# and writes nothing. Entry dispatch is checked live; screens are checked in preview only.
	# 2. Sports Festival (event 0x20 -> native 0x72 -> dispatcher 0x47 TAIIKU)
	var ath_before: int = State.high(int(state.rule_state["params"][4]))
	var mode_sports: int = _queue_day_event_and_step(ctx, 0x20, 0, 0, 0)
	_check(mode_sports == 0x72 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x47,
		"Day event 0x20 dispatches live to native 0x72 -> dispatcher 0x47 (TAIIKU)")
	_step_pad(session, 0)
	_check(state.game_mode == 0x72, "TAIIKU (0x72) waits on neutral input")
	var ovl_sp: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x72, "TAIIKU (0x72)")
	_check(State.high(int(state.rule_state["params"][4])) == ath_before, "TAIIKU refusal writes no athletics delta")
	var img_sp1: Image = controller.overlay_dispatcher.render_mode(0x47, ovl_sp).get("image")
	var ovl_sp2: Dictionary = _preview(controller, 0x72, ovl_sp, [PAD_DOWN])
	var img_sp2: Image = controller.overlay_dispatcher.render_mode(0x47, ovl_sp2).get("image")
	_check(img_sp1 != null and img_sp1.get_size() == Vector2i(320, 240) and _image_diff_pixels(img_sp1, img_sp2) > 0,
		"TAIIKU renders 320x240 HUD and preview updates competition/cursor pixels")
	_check_preview_exit_without_rewards(controller, 0x72, ovl_sp2, [PAD_CIRCLE, PAD_CIRCLE, PAD_CIRCLE], "TAIIKU")
	_leave_refused_family(state)

	# 3. Culture Festival (event 0x24 -> native 0x70 BUNKASAI for club<6, native 0x71 BUNKAKEN for club>=6, plus 0x80 BUNKA_SD)
	# This gate's saved state has no clubs[].practice_points, event_log or timer_ticks, so the
	# source-backed BUNKASAI handler refuses it (its accepted path is covered by
	# tests/rules/run_route_state_parity_tests.gd).
	state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~(0xF << 12)) | (1 << 12) # club 1 (<6)
	var mode_cult_a: int = _queue_day_event_and_step(ctx, 0x24, 0, 0, 0)
	_check(mode_cult_a == 0x70 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x48,
		"Culture event 0x24 (club<6) dispatches to native 0x70 -> dispatcher 0x48 (BUNKASAI)")
	# BUNKASAI only acts on CIRCLE; CROSS is "cancel_rejected" with no effect (no source exit).
	var cult_before: Dictionary = state.rule_state.duplicate(true)
	var cult_cross: Session.FrameResult = _step_pad(session, PAD_CROSS)
	var cult_after: Dictionary = state.rule_state.duplicate(true)
	cult_before.erase("overlay_state")
	cult_after.erase("overlay_state")
	_check(cult_cross.error.is_empty() and state.game_mode == 0x70 and cult_after == cult_before
		and String(state.rule_state["overlay_state"].get("action", "")) == "cancel_rejected",
		"BUNKASAI CROSS has no source exit: refused without completion or saved-state writes")
	_check_refused_family(ctx, 0x70, 0x48, "BUNKASAI (0x70)", "booth_prep", [PAD_CIRCLE], [PAD_CIRCLE], [PAD_CIRCLE])

	state.rule_state["player_flags"] = (state.rule_state["player_flags"] & ~(0xF << 12)) | (7 << 12) # club 7 (>=6)
	var mode_cult_b: int = _queue_day_event_and_step(ctx, 0x24, 0, 0, 0)
	_check(mode_cult_b == 0x71 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x4A,
		"Culture event 0x24 (club>=6) dispatches to native 0x71 -> dispatcher 0x4A (BUNKAKEN)")
	_check_refused_family(ctx, 0x71, 0x4A, "BUNKAKEN (0x71)", "visitor_girl", [PAD_CIRCLE], [PAD_CIRCLE])

	# BUNKA_SD (native 0x80 -> dispatcher 0x49)
	controller.start_overlay(state, 0x80, {"girl_id": 1, "club_id": 1})
	_check_refused_family(ctx, 0x80, 0x49, "BUNKA_SD (0x80)", "stage_show", [PAD_CIRCLE], [PAD_CIRCLE])

	# 4. Exam Results (event 0x40 -> native 0x35 / dispatcher 0x35 EXAM)
	var hum_before: int = State.high(int(state.rule_state["params"][1]))
	var mode_exam: int = _queue_day_event_and_step(ctx, 0x40, 0, 0, 0)
	_check(mode_exam == 0x35, "Day event 0x40 dispatches live to native mode 0x35 (EXAM)")
	_check_refused_family(ctx, 0x35, 0x35, "EXAM (0x35)", "exam_ranking", [PAD_CIRCLE], [PAD_CIRCLE])
	_check(State.high(int(state.rule_state["params"][1])) == hum_before, "EXAM refusal writes no academic parameter delta")

	# 5. Summer Club Camp (event 0x42 -> native 0x5F CLUB_CAMP -> dispatcher 0x4D KANGEI;
	# include/game_enums.h names the KANGEI overlay for native 0x5F/0x60)
	var day_count_before_camp: int = int(state.rule_state["day_count"])
	var mode_camp: int = _queue_day_event_and_step(ctx, 0x42, 0, 0, 0)
	_check(mode_camp == 0x5F and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x4D,
		"Day event 0x42 dispatches live to native 0x5F -> dispatcher 0x4D (KANGEI camp)")
	_check_refused_family(ctx, 0x5F, 0x4D, "KANGEI (0x5F)", "camp_day", [PAD_CIRCLE], [PAD_CIRCLE])
	_check(int(state.rule_state["day_count"]) == day_count_before_camp, "KANGEI refusal does not advance the calendar")
	_check(DayEventModes.dispatcher_mode_for(0x60) == 0x4D and DayEventModes.dispatcher_mode_for(0x62) == 0x4B,
		"native 0x60 maps to KANGEI and 0x62 to OMIMAI")
	_check(not DayEventModes.NATIVE_TO_DISPATCHER_MODE.has(0x3A), "native 0x3A (OLH help) stays unbound until its command-screen exit is ported")

	# 6. School Trip (event 0x41 -> native 0x51 -> dispatcher 0x4C SHUGAKU)
	var day_count_before_trip: int = int(state.rule_state["day_count"])
	var mode_trip: int = _queue_day_event_and_step(ctx, 0x41, 0, 0, 0)
	_check(mode_trip == 0x51 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x4C,
		"Day event 0x41 dispatches live to native 0x51 -> dispatcher 0x4C (SHUGAKU school trip)")
	_check_refused_family(ctx, 0x51, 0x4C, "SHUGAKU (0x51)", "corridor_event", [PAD_CIRCLE, PAD_CIRCLE], [PAD_CIRCLE])
	_check(int(state.rule_state["day_count"]) == day_count_before_trip, "SHUGAKU refusal does not advance the calendar")

	# 7. Holiday / Seasonal Encounters via queue opcodes:
	#    0x2C -> 0x41 (KANGEI 0x4D), 0x28 -> 0x42 (SHOUGATU 0x4E), 0x31 -> 0x54 (VALEN 0x4F),
	#    0x47 -> 0x61 (OMIMAI 0x4B), 0x25 -> 0x74 (MASTER 0x52)
	var seasonal_cases: Array = [
		[0x2C, 0x41, 0x4D, "camp_day"],
		[0x28, 0x42, 0x4E, "christmas_girls"],
		[0x31, 0x54, 0x4F, "valentine_candidates"],
		[0x47, 0x61, 0x4B, "sick_visit_dialogue"],
		[0x25, 0x74, 0x52, "master_grade"],
	]
	for sc in seasonal_cases:
		var op_s: int = int(sc[0])
		var exp_native: int = int(sc[1])
		var exp_disp: int = int(sc[2])
		var exp_k: String = String(sc[3])
		var ent_m: int = _queue_day_event_and_step(ctx, op_s, 0, 0, 0)
		_check(ent_m == exp_native and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == exp_disp,
			"Seasonal event 0x%02X dispatches live to native 0x%02X / dispatcher 0x%02X" % [op_s, exp_native, exp_disp])
		_check_refused_family(ctx, exp_native, exp_disp, "Seasonal mode 0x%02X" % exp_native, exp_k, [PAD_CIRCLE], [PAD_CIRCLE])

	# 8. Festival Stall Games (native 0x82 -> dispatcher 0x53 EN_NICHI)
	controller.start_overlay(state, 0x82, {"girl_id": 0})
	_check(state.game_mode == 0x82 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x53,
		"Entered native 0x82 -> dispatcher 0x53 (EN_NICHI)")
	_check_refused_family(ctx, 0x82, 0x53, "EN_NICHI (0x82)", "stall_result", [PAD_CIRCLE], [PAD_CIRCLE])

	_family_evidence["seasonal_and_school_events"] = {
		"native_modes": ["0x35", "0x39", "0x41", "0x42", "0x47", "0x48", "0x49", "0x4A", "0x4B", "0x4C", "0x4D", "0x4E", "0x4F", "0x51", "0x52", "0x53", "0x54", "0x55", "0x56", "0x57", "0x5F", "0x61", "0x70", "0x71", "0x72", "0x73", "0x74", "0x75", "0x76", "0x80", "0x82"],
		"day_event_opcodes": ["0x20", "0x24", "0x25", "0x26", "0x27", "0x28", "0x2C", "0x30", "0x31", "0x32", "0x33", "0x40", "0x41", "0x42", "0x47", "0xFD"],
		"dispatcher_modes": ["0x35", "0x3F", "0x47", "0x48", "0x49", "0x4A", "0x4B", "0x4C", "0x4D", "0x4E", "0x4F", "0x51", "0x52", "0x53"],
		"source_functions": ["taiiku_main", "bunkasai_main", "bunka_sd_main", "bunkaken_main", "exam_main", "rensyu_main", "shugaku_main", "kangei_main", "shougatu_main", "valen_main", "omimai_main", "master_main", "en_nichi_main"],
		"entry_and_caller": "DayEventModes scheduler opcodes -> SchoolDayController._day_screen -> _overlay_mode -> OverlayDispatcher",
		"required_content_services": ["OverlayDispatcher.school_events_presenter", "OverlayDispatcher.encounters_ending_presenter", "SchoolEventsOverlay", "EndingOverlay"],
		"return_and_state_effects": "T-39006: the ceremony notice (ETC menu) returns to 0x31; every event family here is refused with saved state (no parameter, affection, tokimeki or calendar write; BUNKASAI also lacks clubs/event_log/timer_ticks in this fixture); preview steps render and exit without rewards",
		"verified_live": false,
	}


# ---------------------------------------------------------------------------
# (f) Borrowed System Overlays & Menus (0x36 OPTION, 0x3F ETC, 0x37 OLH)
# ---------------------------------------------------------------------------
func _test_f_borrowed_system_overlays(tables: Dictionary) -> void:
	print("--- [G3.f] Borrowed System Overlays & Menus (0x36 OPTION, 0x3F ETC, 0x37 OLH) ---")
	var ctx: Dictionary = _create_live_controller_session(tables)
	var controller: Controller = ctx["controller"]
	var state: State = ctx["state"]
	var session: Session = ctx["session"]
	controller.overlay_dispatcher.title_overlay = InGameEtcOverlay.new()
	controller.overlay_dispatcher.options_overlay = InGameOptionOverlay.new()

	# 1. Online Help / Q&A (native 0x37 -> dispatcher 0x37 GAME_MODE_OLH)
	controller.start_overlay(state, 0x37, {"cursor": 0, "stage": 1})
	_check(state.game_mode == 0x37, "Entered native mode 0x37 (GAME_MODE_OLH)")
	_step_pad(session, 0)
	_check(state.game_mode == 0x37, "GAME_MODE_OLH waits on neutral input")
	var img_olh0: Image = controller.overlay_dispatcher.render_mode(0x37, state.rule_state["overlay_state"]).get("image")
	_step_pad(session, PAD_DOWN)
	var img_olh1: Image = controller.overlay_dispatcher.render_mode(0x37, state.rule_state["overlay_state"]).get("image")
	_check(img_olh0 != null and img_olh0.get_size() == Vector2i(320, 240) and _image_diff_pixels(img_olh0, img_olh1) > 0,
		"GAME_MODE_OLH Q&A green row highlight + hand cursor at (188, 68+16r) shifts pixels on PAD_DOWN")
	_step_pad(session, PAD_START)
	_check(state.game_mode == 0x31, "GAME_MODE_OLH returns cleanly to 0x31 on PAD_START")

	# 2. Options Screen (native 0x36 -> dispatcher 0x36 GAME_MODE_OPTION)
	controller.start_overlay(state, 0x36, {"cursor": 0, "stage": 1})
	_check(state.game_mode == 0x36, "Entered native mode 0x36 (GAME_MODE_OPTION)")
	_step_pad(session, 0)
	_check(state.game_mode == 0x36, "GAME_MODE_OPTION waits on neutral input")
	var img_opt0: Image = controller.overlay_dispatcher.render_mode(0x36, state.rule_state["overlay_state"]).get("image")
	_step_pad(session, PAD_DOWN)
	var img_opt1: Image = controller.overlay_dispatcher.render_mode(0x36, state.rule_state["overlay_state"]).get("image")
	_check(img_opt0 != null and img_opt0.get_size() == Vector2i(320, 240) and _image_diff_pixels(img_opt0, img_opt1) > 0,
		"GAME_MODE_OPTION cursor movement changes rendered 320x240 image")
	_step_pad(session, PAD_CROSS)
	_check(state.game_mode == 0x31, "GAME_MODE_OPTION returns cleanly to 0x31 on PAD_CROSS")

	# 3. Borrowed Title / Save-Load / Etc Overlay (native 0x3F -> dispatcher 0x3F GAME_MODE_ETC)
	controller.start_overlay(state, 0x3F, {"cursor": 0, "stage": 1})
	_check(state.game_mode == 0x3F, "Entered native mode 0x3F (GAME_MODE_ETC)")
	_step_pad(session, 0)
	_check(state.game_mode == 0x3F, "GAME_MODE_ETC waits on neutral input")
	var img_etc: Image = controller.overlay_dispatcher.render_mode(0x3F, state.rule_state["overlay_state"]).get("image")
	_check(img_etc != null and img_etc.get_size() == Vector2i(320, 240), "GAME_MODE_ETC renders 320x240")
	_step_pad(session, PAD_CIRCLE)
	_check(state.game_mode == 0x31, "GAME_MODE_ETC returns cleanly to 0x31")

	_family_evidence["borrowed_system_overlays"] = {
		"native_modes": ["0x01", "0x36", "0x37", "0x3F", "0x5B"],
		"day_event_opcodes": ["0x43", "0x48", "0x4A"],
		"dispatcher_modes": ["0x36", "0x37", "0x3F"],
		"source_functions": ["option_main", "olh_main", "etc_main"],
		"entry_and_caller": "SchoolDayController.start_overlay / DayEventModes 0x43/0x48/0x4A -> _overlay_mode -> OverlayDispatcher",
		"required_content_services": ["OverlayDispatcher.vn_presenter", "SystemOverlays.OptionsOverlay", "SystemOverlays.HelpOverlay", "SystemOverlays.TitleMenuOverlay"],
		"return_and_state_effects": "Returns to 0x31; updates options settings dictionary in overlay state",
		"verified_live": true,
	}


# ---------------------------------------------------------------------------
# (g) Subgames (native 0x81 RPG_BATTLE, 0x90 TACO, 0xD0 TT)
# ---------------------------------------------------------------------------
func _test_g_subgames_live_entry_and_return(tables: Dictionary) -> void:
	print("--- [G3.g] Subgames Live Entry and Production Refusal (native 0x81 RPG_BATTLE, 0x90 TACO, 0xD0 TT) ---")
	var ctx: Dictionary = _create_live_controller_session(tables)
	var controller: Controller = ctx["controller"]
	var state: State = ctx["state"]
	var session: Session = ctx["session"]

	# 1. RPG_BATTLE (native 0x81 -> dispatcher 0x57 GAME_MODE_RPG_BATTLE)
	controller.start_overlay(state, 0x81, {"enemy_hp": 180})
	_check(state.game_mode == 0x81 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x57,
		"Entered native 0x81 -> dispatcher 0x57 (GAME_MODE_RPG_BATTLE)")
	for _n in range(3):
		_step_pad(session, 0)
		_check(state.game_mode == 0x81, "RPG_BATTLE does not auto-exit on neutral input")
	# T-39006: the battle uses a fixed rand_seed instead of native rand(), so with saved state
	# it is refused and writes no talk_result_code. Screens are checked in preview only.
	var ovl_rpg: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	var talk_before: Variant = state.rule_state.get("talk_result_code")
	_assert_production_rejection(ctx, 0x81, "RPG_BATTLE (0x81)")
	_check(state.rule_state.get("talk_result_code") == talk_before, "RPG_BATTLE refusal writes no talk_result_code")
	var img_rpg0: Image = controller.overlay_dispatcher.render_mode(0x57, ovl_rpg).get("image")
	var img_rpg1: Image = controller.overlay_dispatcher.render_mode(0x57, _preview(controller, 0x81, ovl_rpg, [PAD_DOWN])).get("image")
	_check(img_rpg0 != null and img_rpg0.get_size() == Vector2i(320, 240) and _image_diff_pixels(img_rpg0, img_rpg1) > 0,
		"RPG_BATTLE renders 320x240 and preview command cursor shifts pixels")
	var ovl_victory: Dictionary = _preview(controller, 0x81, ovl_rpg, [PAD_CIRCLE]) # Attack defeats the 180 HP enemy
	_check(bool(ovl_victory.get("victory", false)), "RPG_BATTLE preview reaches victory stage 10 and waits for confirm")
	_check_preview_exit_without_rewards(controller, 0x81, ovl_victory, [PAD_CIRCLE], "RPG_BATTLE")
	_leave_refused_family(state)

	# 2. TACO Shooter (native 0x90 -> dispatcher 0x59 GAME_MODE_TACO). The step_mode
	# monolith invents score, roster and ranking, so saved-state play is refused,
	# including neutral frames. Screens are checked in preview only.
	controller.start_overlay(state, 0x90, {"waves_per_stage": 1, "target_stages": 1})
	_check(state.game_mode == 0x90 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x59,
		"Entered native 0x90 -> dispatcher 0x59 (GAME_MODE_TACO)")
	var ovl_taco: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0x90, "TACO (0x90)", [0, PAD_CIRCLE, PAD_CROSS])
	for key: String in ["high_scores", "high_score_names", "stages_cleared", "score", "total_score", "menu_hit_3", "subgame_return_payload"]:
		_check(not state.rule_state.has(key), "TACO refusal leaves no %s in rule_state" % key)
	var img_taco0: Image = controller.overlay_dispatcher.render_mode(0x59, ovl_taco).get("image")
	var img_taco1: Image = controller.overlay_dispatcher.render_mode(0x59, _preview(controller, 0x90, ovl_taco, [PAD_CIRCLE, PAD_CIRCLE])).get("image")
	_check(img_taco0 != null and img_taco0.get_size() == Vector2i(320, 240) and _image_diff_pixels(img_taco0, img_taco1) > 0,
		"TACO preview renders 320x240 and shots update pixels")
	_leave_refused_family(state)

	# 3. TwinBee Time Attack TT (native 0xD0 -> dispatcher 0x5A GAME_MODE_TT), refused the same way.
	controller.start_overlay(state, 0xD0, {"target_stages": 1, "boss_hp": 16})
	_check(state.game_mode == 0xD0 and int(state.rule_state["overlay_state"].get("dispatcher_mode", -1)) == 0x5A,
		"Entered native 0xD0 -> dispatcher 0x5A (GAME_MODE_TT)")
	var ovl_tt: Dictionary = state.rule_state["overlay_state"].duplicate(true)
	_assert_production_rejection(ctx, 0xD0, "TT (0xD0)", [0, PAD_UP, PAD_SQUARE])
	for key: String in ["tt_stages_cleared", "tt_last_time_frames", "tt_rankings"]:
		_check(not state.rule_state.has(key), "TT refusal leaves no %s in rule_state" % key)
	var img_tt0: Image = controller.overlay_dispatcher.render_mode(0x5A, ovl_tt).get("image")
	_check(img_tt0 != null and img_tt0.get_size() == Vector2i(320, 240), "TT preview renders 320x240")
	_leave_refused_family(state)

	# Native 0x57/0x59/0x5A are GEKO modes, never the dispatcher's RPG/TACO/TT ids.
	for geko_mode: int in [0x57, 0x59, 0x5A]:
		_check(DayEventModes.dispatcher_mode_for(geko_mode) == OverlayDispatcher.GAME_MODE_GEKO,
			"native 0x%02X resolves to GEKO, not a minigame" % geko_mode)

	_family_evidence["subgames_rpg_taco_tt"] = {
		"native_modes": ["0x81", "0x90", "0xD0"],
		"dispatcher_modes": ["0x57", "0x59", "0x5A"],
		"source_functions": ["rpg_battle_main", "taco_shooter_main", "tt_twinbee_main"],
		"entry_and_caller": "SchoolDayController.start_overlay / DayEventModes 0x81/0x90/0xD0 -> _overlay_mode -> OverlayDispatcher",
		"required_content_services": ["OverlayDispatcher.subgame_presenter", "SubgameOverlays"],
		"return_and_state_effects": "RPG_BAT, TACO and TT are refused with saved state and write nothing; their source results live in system_data, not rule_state",
		"verified_live": false,
	}


# ---------------------------------------------------------------------------
# (h) Graduation & Ending (0x5C / 0x63 / 0xC1 ENDING + 0xC2 CREDITS + Clear Save)
# ---------------------------------------------------------------------------
func _test_h_graduation_and_ending_flows(tables: Dictionary) -> void:
	print("--- [G3.h] Graduation & Ending Flows (0x5C/0x63/0xC1 Confession 4-phase vs Bad Ending 3-phase) ---")
	# 1. Heroine Confession Ending (4 phases: confession -> staff_roll -> last_photo -> clear_save)
	var ctx_win: Dictionary = _create_live_controller_session(tables)
	var controller_win: Controller = ctx_win["controller"]
	var state_win: State = ctx_win["state"]
	var session_win: Session = ctx_win["session"]

	state_win.rule_state["year"] = 98
	state_win.rule_state["month"] = 3
	state_win.rule_state["day"] = 1
	state_win.rule_state["weekday"] = 6
	state_win.rule_state["params"] = [100 << 16, 145 << 16, 145 << 16, 145 << 16, 135 << 16, 130 << 16, 130 << 16, 145 << 16, 10 << 16]
	state_win.rule_state["girls"][0]["affection_word"] = 120 << 16
	state_win.rule_state["girls"][0]["tokimeki_word"] = 120 << 16
	state_win.rule_state["girls"][0]["syoushin_word"] = 0
	state_win.rule_state["girls"][0]["flags"] = 2 | (1 << 26)
	state_win.rule_state["girls"][0]["counters"][1] = 8
	state_win.reset_game_mode_fields(0x31)

	# Step 0x31 step 0 -> step 1 -> mode 0x5C (ENDING)
	_step_pad(session_win, 0)
	_step_pad(session_win, 0)
	_check(state_win.game_mode == 0x5C, "Graduation Day 1998-03-01 transitions live to mode 0x5C (ENDING)")
	_step_pad(session_win, 0)
	_check(state_win.game_mode == 0x5C and int(state_win.rule_state.get("ending_completed", 0)) == 0,
		"ENDING (0x5C) waits for player input on neutral frames")
	var img_end: Image = controller_win.overlay_dispatcher.render_mode(0x5C, state_win.rule_state["overlay_state"]).get("image")
	_check(img_end != null and img_end.get_size() == Vector2i(320, 240), "ENDING renders 320x240 confession frame")

	for _step in range(4):
		_step_pad(session_win, PAD_CIRCLE)
	# T-39007: the fourth confirm requests the clear save; the host storage
	# transaction answers with the completion that finishes the ending.
	var clear_host: ClearSave = ClearSave.new("/tmp/t39007-g3-" + str(Time.get_ticks_usec()))
	var clear_service: Dictionary = clear_host.service_request(state_win.rule_state["overlay_state"])
	var clear_tick: int = session_win.clock.logic_iterations
	session_win.step(InputFrame.new(clear_tick, 0), [Session.ServiceEvent.new(clear_tick, Session.Phase.BEFORE_MODE,
		Session.ServiceKind.COMPLETION, clear_service.get("service", ""), clear_service.get("payload", {}))])
	_check(int(state_win.rule_state.get("ending_completed", 0)) == 1, "Heroine confession ending completes after 4 interactive phases")
	_check(int(state_win.rule_state.get("confessed_girl", -1)) == EndingOverlay.GIRL_SHIORI, "Confession ending selects Shiori (girl 0)")
	_check(String(state_win.rule_state.get("ending_type", "")) == "confession", "Confession ending records ending_type = 'confession'")
	_check(state_win.rule_state.get("ending_phases", []) == ["confession", "staff_roll", "last_photo", "clear_save"],
		"Confession ending records all 4 phases [confession, staff_roll, last_photo, clear_save]")
	_check(int(state_win.rule_state.get("album_clear_mask", 0)) == (1 << 0), "Confession ending updates album_clear_mask bit 0")
	_check(state_win.mode_stage == 0xFF, "ENDING sets terminal mode_stage = 0xFF")

	# 2. Yoshio Bad Ending (3 phases: yoshio_bad_ending -> staff_roll -> clear_save)
	var ctx_bad: Dictionary = _create_live_controller_session(tables)
	var state_bad: State = ctx_bad["state"]
	var session_bad: Session = ctx_bad["session"]
	state_bad.rule_state["year"] = 98
	state_bad.rule_state["month"] = 3
	state_bad.rule_state["day"] = 1
	state_bad.rule_state["weekday"] = 6
	state_bad.rule_state["params"] = [80 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 90 << 16, 10 << 16]
	state_bad.rule_state["girls"][0]["affection_word"] = 95 << 16
	state_bad.rule_state["girls"][0]["tokimeki_word"] = 95 << 16
	state_bad.rule_state["girls"][0]["flags"] = 2
	state_bad.rule_state["girls"][0]["counters"][1] = 2
	state_bad.reset_game_mode_fields(0x31)

	_step_pad(session_bad, 0)
	_step_pad(session_bad, 0)
	_check(state_bad.game_mode == 0x5C, "Ineligible Graduation Day transitions to mode 0x5C (ENDING)")
	for _step in range(2):
		_step_pad(session_bad, PAD_CIRCLE)
	# T-39007: cross answers "no" to the clear-save prompt, so nothing is stored.
	_step_pad(session_bad, EndingOverlay.PAD_CROSS)
	_check(int(state_bad.rule_state.get("ending_completed", 0)) == 1, "Bad ending completes after 3 interactive phases")
	_check(not bool(state_bad.rule_state.get("clear_save_completed", true)), "Declined bad-ending clear save is not marked saved")
	_check(int(state_bad.rule_state.get("confessed_girl", -1)) == EndingOverlay.GIRL_YOSHIO, "Bad ending selects GIRL_YOSHIO (13)")
	_check(String(state_bad.rule_state.get("ending_type", "")) == "yoshio_bad_ending", "Bad ending records ending_type = 'yoshio_bad_ending'")
	_check(state_bad.rule_state.get("ending_phases", []) == ["yoshio_bad_ending", "staff_roll", "clear_save"],
		"Bad ending records 3 phases [yoshio_bad_ending, staff_roll, clear_save]")

	_family_evidence["graduation_and_ending"] = {
		"native_modes": ["0x5C", "0x63", "0xC1", "0xC2"],
		"dispatcher_modes": ["0x3F", "0x5C"],
		"source_functions": ["ending_main", "evaluate_confession_candidates", "create_graduation_sequence", "step_graduation_sequence"],
		"entry_and_caller": "OrdinaryWeek 1998-03-01 graduation check / native 0x5C/0x63 -> SchoolDayController._overlay_mode -> EndingOverlay",
		"required_content_services": ["OverlayDispatcher.encounters_ending_presenter", "EndingOverlay", "SaveService.record_completed_route"],
		"return_and_state_effects": "Terminates route (mode_stage=0xFF); writes ending_completed=1, confessed_girl, ending_type, ending_phases, clear_save_completed, album_clear_mask",
		"verified_live": true,
	}


# ---------------------------------------------------------------------------
# Reconcile the 3 G3 Lists & Emit g3-reachability-reconciliation.json
# ---------------------------------------------------------------------------
func _reconcile_and_emit_g3_lists() -> void:
	print("--- [G3 Reconciliation] Auditing 68 NativeRouteRegistry modes, DayEventModes, and OverlayDispatcher ---")
	var native_ids: Array[int] = Registry.all_native_ids()
	_check(native_ids.size() == 68, "NativeRouteRegistry.all_native_ids() contains exactly 68 native mode IDs (got %d)" % native_ids.size())

	var overlay_native_modes: Array[int] = DayEventModes.all_overlay_native_modes()
	_check(overlay_native_modes.size() >= 30, "DayEventModes.all_overlay_native_modes() covers all interactive overlay native modes (%d)" % overlay_native_modes.size())

	var registry_entries: Array[Dictionary] = []
	var counts_by_classification: Dictionary = {}
	for mode_id: int in native_ids:
		var desc: Dictionary = Registry.describe(mode_id)
		_check(not desc.is_empty(), "NativeRouteRegistry.describe(0x%02X) is non-empty" % mode_id)
		var cls: String = String(desc.get("classification", ""))
		counts_by_classification[cls] = int(counts_by_classification.get(cls, 0)) + 1
		var disp_mode: int = DayEventModes.dispatcher_mode_for(mode_id)
		var catalog_has: bool = OverlayDispatcher.OVERLAY_CATALOG.has(disp_mode)
		registry_entries.append({
			"native_id_int": mode_id,
			"native_id_hex": "0x%02X" % mode_id,
			"family": String(desc.get("family", "")),
			"classification": cls,
			"capability": String(desc.get("capability", "")),
			"dispatcher_mode_hex": "0x%02X" % disp_mode if disp_mode >= 0 else "none",
			"in_overlay_catalog": catalog_has,
			"bound_in_school_day_controller": (mode_id in [0x15, 0x30, 0x31, 0x32, 0x34, 0x38, 0x40]) or overlay_native_modes.has(mode_id),
		})

	var day_event_table: Array[Dictionary] = []
	var opcodes_to_audit: Array[int] = [
		0x00, 0x10, 0x20, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2C, 0x30,
		0x31, 0x32, 0x33, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
		0x47, 0x48, 0x49, 0x4A, 0x80, 0xFC, 0xFD, 0xFE, 0xFF,
	]
	for op: int in opcodes_to_audit:
		var m: int = DayEventModes.mode_for(op, 0)
		var dm: int = DayEventModes.dispatcher_mode_for(m, op)
		var is_core_scheduler: bool = m in [0x32, 0x34, 0x38, 0x40, 0x46]
		_check(is_core_scheduler or OverlayDispatcher.OVERLAY_CATALOG.has(dm),
			"Day event opcode 0x%02X resolves to valid core mode 0x%02X or OVERLAY_CATALOG dispatcher mode 0x%02X" % [op, m, dm])
		day_event_table.append({
			"day_event_opcode_hex": "0x%02X" % op,
			"native_mode_hex": "0x%02X" % m,
			"dispatcher_mode_hex": "0x%02X" % dm,
			"is_core_scheduler_mode": is_core_scheduler,
			"overlay_name": String(OverlayDispatcher.OVERLAY_CATALOG.get(dm, {}).get("overlay", "CORE_SCHEDULER")),
		})

	var report: Dictionary = {
		"schema": "tokimemo.g3-reachability-reconciliation.v1",
		"ticket": "T-29030",
		"parent_tickets": ["T-29020", "T-29028"],
		"gate": "G3",
		"native_route_registry_total": native_ids.size(),
		"counts_by_classification": counts_by_classification,
		"day_event_opcodes_audited": day_event_table.size(),
		"overlay_catalog_modes_total": OverlayDispatcher.OVERLAY_CATALOG.size(),
		"reachable_gameplay_families": _family_evidence,
		"day_event_dispatch_reconciliation": day_event_table,
		"native_route_registry_68_modes": registry_entries,
		"checks_passed": _pass_count,
		"checks_failed": _fail_count,
	}

	DirAccess.make_dir_recursive_absolute(RECONCILIATION_JSON_PATH.get_base_dir())
	var f := FileAccess.open(RECONCILIATION_JSON_PATH, FileAccess.WRITE)
	_check(f != null, "Write reconciliation JSON to " + RECONCILIATION_JSON_PATH)
	if f != null:
		f.store_string(JSON.stringify(report, "  ") + "\n")
		f.close()


func _to_int_tree(v: Variant) -> Variant:
	if v is Array:
		var arr: Array = []
		for item: Variant in v:
			arr.append(_to_int_tree(item))
		return arr
	if v is float or v is int:
		return int(v)
	return v


func _load_tables(manifest_path: String) -> Dictionary:
	if not FileAccess.file_exists(manifest_path):
		return {}
	var manifest_text: String = FileAccess.get_file_as_string(manifest_path)
	var manifest: Variant = JSON.parse_string(manifest_text)
	if not (manifest is Dictionary):
		return {}
	var tables_var: Variant = manifest.get("tables", {})
	if not (tables_var is Dictionary):
		return {}
	var out: Dictionary = {}
	for k: String in Controller.TABLE_SHAPES.keys():
		if not tables_var.has(k):
			return {}
		out[k] = _to_int_tree(tables_var[k])
	return out
