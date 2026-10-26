extends SceneTree
## T-29013: Live Playable Holiday Command Loop Integration & Interactive Date Choice Slice.
## Verifies:
## 1. Live Holiday HUD (0x46) -> Icon 12 (0x30 Calendar) -> month flip -> X cancel -> 0x46 HUD.
## 2. Live Holiday HUD (0x46) -> Icon 8 (0x10 Phone Book) -> cursor navigation -> X cancel -> 0x46 HUD.
## 3. Live Holiday HUD (0x46) -> Icon 10 (0x20 Magazine) -> page cycle -> X cancel -> 0x46 HUD.
## 4. Live Holiday HUD (0x46) -> Icon 9 (0xFE Date Appointment) -> 0x50 Date Arrival (with fixed Date Pillar,
##    VN text layout y=48/64/80, speaker x=-120, quote x=-86, body x=-72, bobbing wait marker actor_pool[2] frame=1)
##    -> Interactive 3-Choice Date Menu (normal_date_three_select, y=48/64/80, x=-128/-114, menu_bar_show(1),
##    0x9C hand cursor at (120, 56 + row * 16)) -> Shiori's response + date_choice_score impression delta.

const PlayableBoot = preload("res://app/playable_boot.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const Pad = preload("res://ui/native_pad.gd")
const AppointmentRules = preload("res://rules/holiday_appointment.gd")
const HolidayRules = preload("res://rules/holiday.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Session = preload("res://core/game_session.gd")

const BOUNDARY_SHA: String = PlayableBoot.BOUNDARY_SHA
const TABLES_SHA: String = PlayableBoot.TABLES_SHA
const HUD_SHA: String = PlayableBoot.LiveHud.MANIFEST_SHA
const CONTEXT_SHA: String = PlayableBoot.LiveHud.CONTEXT_SHA
const TEXT_SHA: String = PlayableBoot.HolidayHud.TEXT_SHA

const PAD_UP: int = 0x1000
const PAD_RIGHT: int = 0x2000
const PAD_DOWN: int = 0x4000
const PAD_LEFT: int = 0x8000
const PAD_CONFIRM: int = 0x0020
const PAD_CANCEL: int = 0x0040

const PLACE_PARK: int = 0
const PLACE_AQUARIUM: int = 3
const GIRL_SHIORI: int = 0

var _passes: int = 0
var _failures: int = 0

func _ok(condition: bool, message: String) -> void:
	if condition:
		_passes += 1
	else:
		_failures += 1
		push_error("FAIL: " + message)

func _eq(actual: Variant, expected: Variant, message: String) -> void:
	if actual == expected:
		_passes += 1
	else:
		_failures += 1
		push_error("FAIL: %s (expected=%s actual=%s)" % [message, str(expected), str(actual)])

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var repo: String = OS.get_environment("TOKIMEMO_REPO_ROOT")
	if repo.is_empty():
		repo = ProjectSettings.globalize_path("res://../..").simplify_path()
	var captures_dir: String = OS.get_environment("TOKIMEMO_LIVE_LOOP_CAPTURES")
	if captures_dir.is_empty():
		captures_dir = repo.path_join(".claude/orchestrator/state/T-29013/holiday-live-loop-v2/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)

	_set_env(repo)

	var app: PlayableBoot = PlayableBoot.new()
	app.canvas = Canvas.new()
	app.pad = Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	app.continue_week = true
	var load_err: String = app._load_inputs()
	if load_err.is_empty():
		load_err = app._reset()
	_ok(load_err.is_empty() and app.failure.is_empty(), "playable_boot opened without failure: " + load_err + app.failure)
	_ok(app.holiday_controller != null, "holiday_controller initialized")
	_ok(app.calendar_phone_presenter != null, "calendar_phone_presenter initialized in playable_boot")
	_ok(app.date_appointment_presenter != null, "date_appointment_presenter initialized in playable_boot")
	if _failures > 0 or app.holiday_controller == null:
		quit(1)
		return
	_ok(app.holiday_controller.enable_subscreens, "holiday_controller.enable_subscreens is enabled")
	if _failures > 0:
		quit(1)
		return

	# Advance playable_boot from April 4 boundary to Sunday April 9 Holiday HUD (mode 0x46, stage 1, step 0)
	while app.session.clock.logic_iterations <= 245 and not app.stopped:
		var tick: int = app.session.clock.logic_iterations
		var held: int = 0x20 if tick >= 17 and tick < 23 else 0
		_step_app(app, InputFrame.new(tick, held, app.session.latched_input.held))
		if _failures > 0:
			break

	_eq(app.state.game_mode, 0x46, "reached Sunday Holiday mode 0x46")
	_eq(app.state.mode_stage, 1, "reached Sunday Holiday menu stage 1")
	_eq(app.state.mode_step, 0, "reached Sunday Holiday menu step 0")
	_eq(app.state.rule_state["icon_enabled"].slice(0, 14), [1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0], "Holiday HUD enables Sunday icons with 9/13 locked before appointment")
	_save_canvas_png(app, captures_dir.path_join("live_holiday_hud.png"))

	# =========================================================================
	# 1. Icon 12 (カレンダー Calendar) -> 0x30 -> month flip -> X cancel -> 0x46
	# =========================================================================
	app.state.rule_state["menu_cursor"] = 12
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, 0x46, "Calendar stays in game_mode 0x46")
	_eq(app.state.mode_stage, 0x30, "Icon 12 transitions to stage 0x30 (Calendar)")
	_eq(int(app.state.rule_state.get("calendar_month_offset", -1)), 0, "Calendar starts at month_offset 0 (April)")
	_save_canvas_png(app, captures_dir.path_join("live_calendar_april.png"))

	# Flip month forward (Right -> May, month_offset = 1)
	_press_app(app, PAD_RIGHT)
	_eq(app.state.mode_stage, 0x30, "Calendar remains in stage 0x30 on month flip")
	_eq(int(app.state.rule_state.get("calendar_month_offset", -1)), 1, "Right flips calendar_month_offset to 1 (May)")
	_save_canvas_png(app, captures_dir.path_join("live_calendar_may_flipped.png"))

	# Flip month back (Left -> April, month_offset = 0)
	_press_app(app, PAD_LEFT)
	_eq(int(app.state.rule_state.get("calendar_month_offset", -1)), 0, "Left flips calendar_month_offset back to 0 (April)")

	# Cancel (X -> 0x40) returns to Holiday HUD (0x46, stage 1, step 0)
	_press_app(app, PAD_CANCEL)
	_eq(app.state.game_mode, 0x46, "Calendar cancel returns to game_mode 0x46")
	_eq(app.state.mode_stage, 1, "Calendar cancel returns to Holiday HUD stage 1")
	_eq(app.state.mode_step, 0, "Calendar cancel returns to Holiday HUD step 0")
	_save_canvas_png(app, captures_dir.path_join("live_holiday_after_calendar.png"))

	# =========================================================================
	# 2. Icon 8 (電話 Phone Book) -> 0x10 -> cursor navigation -> X cancel -> 0x46
	# =========================================================================
	app.state.rule_state["menu_cursor"] = 8
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, 0x46, "Phone Book stays in game_mode 0x46")
	_eq(app.state.mode_stage, 0x10, "Icon 8 transitions to stage 0x10 (Phone Book)")
	_eq(int(app.state.rule_state["phone_state"]["cursor"]), 0, "Phone Book starts at cursor 0 (Yoshio)")
	_save_canvas_png(app, captures_dir.path_join("live_phone_book_yoshio.png"))

	# Down -> cursor 1 (Shiori)
	_press_app(app, PAD_DOWN)
	_eq(int(app.state.rule_state["phone_state"]["cursor"]), 1, "Phone Book Down moves cursor to 1 (Shiori)")
	_save_canvas_png(app, captures_dir.path_join("live_phone_book_shiori.png"))

	# Right -> cursor 12 (Ray), Down -> cursor 13 (Cancel)
	_press_app(app, PAD_RIGHT)
	_eq(int(app.state.rule_state["phone_state"]["cursor"]), 12, "Phone Book Right moves cursor to 12 (Ray)")
	_press_app(app, PAD_DOWN)
	_eq(int(app.state.rule_state["phone_state"]["cursor"]), 13, "Phone Book Down moves cursor to 13 (Cancel)")
	_save_canvas_png(app, captures_dir.path_join("live_phone_book_cancel.png"))

	# Cancel (X -> 0x40) returns to Holiday HUD (0x46, stage 1, step 0)
	_press_app(app, PAD_CANCEL)
	_eq(app.state.game_mode, 0x46, "Phone Book cancel returns to game_mode 0x46")
	_eq(app.state.mode_stage, 1, "Phone Book cancel returns to Holiday HUD stage 1")
	_eq(app.state.mode_step, 0, "Phone Book cancel returns to Holiday HUD step 0")
	_save_canvas_png(app, captures_dir.path_join("live_holiday_after_phone.png"))

	# =========================================================================
	# 3. Icon 10 (情報誌 Magazine) -> 0x20 -> page cycle -> X cancel -> 0x46
	# =========================================================================
	app.state.rule_state["menu_cursor"] = 10
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, 0x46, "Magazine stays in game_mode 0x46")
	_eq(app.state.mode_stage, 0x20, "Icon 10 transitions to stage 0x20 (Magazine)")
	_eq(int(app.state.rule_state["mag_state"]["substep"]), 0, "Magazine starts at substep 0 (page 0)")
	_save_canvas_png(app, captures_dir.path_join("live_magazine_page0.png"))

	# Circle (0x20) cycles to substep 2 (page 1)
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.mode_stage, 0x20, "Magazine stays in stage 0x20 on page flip")
	_eq(int(app.state.rule_state["mag_state"]["substep"]), 2, "Magazine flips to substep 2 (page 1)")
	_save_canvas_png(app, captures_dir.path_join("live_magazine_page1.png"))

	# Cancel (X -> 0x40) returns to Holiday HUD (0x46, stage 1, step 0)
	_press_app(app, PAD_CANCEL)
	_eq(app.state.game_mode, 0x46, "Magazine cancel returns to game_mode 0x46")
	_eq(app.state.mode_stage, 1, "Magazine cancel returns to Holiday HUD stage 1")
	_eq(app.state.mode_step, 0, "Magazine cancel returns to Holiday HUD step 0")
	_save_canvas_png(app, captures_dir.path_join("live_holiday_after_magazine.png"))

	# =========================================================================
	# 4. Icon 9 (デート Date Appointment) -> 0xFE -> 0x50 Date Arrival -> 3-Choice -> Response
	# =========================================================================
	# Populate today's appointment (1995-04-09): Shiori at Park (0) and Aquarium (3)
	var appt0: int = AppointmentRules.pack_appointment_word({
		"place": PLACE_PARK, "month": 4, "day": 9, "flag": 1, "girl": GIRL_SHIORI
	})
	var appt1: int = AppointmentRules.pack_appointment_word({
		"place": PLACE_AQUARIUM, "month": 4, "day": 9, "flag": 0, "girl": GIRL_SHIORI
	})
	app.state.rule_state["appointment_words"] = [appt0, appt1, 0, 0, 0, 0, 0, 0]
	HolidayRules.icons(app.state, false, true)
	app.state.rule_state["menu_cursor"] = 9
	_eq(int(app.state.rule_state["icon_enabled"][9]), 1, "Icon 9 (Date) enabled when appointment_words matches today")
	app._present_committed()
	_save_canvas_png(app, captures_dir.path_join("live_holiday_hud_appointment.png"))

	# Press Circle (0x20) on Icon 9 -> enters 0xFE stage_step 1 (2-place choice menu)
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, 0x46, "Date Appointment starts in game_mode 0x46")
	_eq(app.state.mode_stage, 0xFE, "Icon 9 transitions to stage 0xFE (Date Appointment)")
	_eq(app.state.mode_step, 1, "2-place appointment enters mode_step 1 (place choice menu)")
	_eq(int(app.state.rule_state["appointment_flow"]["place_cursor"]), 0, "Appointment place cursor starts at row 0 (Park)")
	_save_canvas_png(app, captures_dir.path_join("live_appointment_choice_row0.png"))

	# Press Down (0x4000) -> cursor moves to row 1 (Aquarium)
	_press_app(app, PAD_DOWN)
	_eq(int(app.state.rule_state["appointment_flow"]["place_cursor"]), 1, "Down moves place_cursor to row 1 (Aquarium)")
	_save_canvas_png(app, captures_dir.path_join("live_appointment_choice_row1.png"))

	# Press Up (0x1000) -> cursor moves back to row 0 (Park), then Circle (0x20) to select Park
	_press_app(app, PAD_UP)
	_eq(int(app.state.rule_state["appointment_flow"]["place_cursor"]), 0, "Up moves place_cursor back to row 0 (Park)")
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.mode_stage, 0xFE, "Place confirm stays in stage 0xFE")
	_eq(app.state.mode_step, 2, "Place selection advances to mode_step 2 (confirm presentation)")
	var chosen_place: int = int(app.state.rule_state["appointment_flow"]["chosen_place"])
	_ok(chosen_place in [PLACE_PARK, PLACE_AQUARIUM], "chosen_place is valid candidate")
	# Force chosen_place = Park (0) if RNG permuted order had Aquarium at row 0
	if chosen_place != PLACE_PARK:
		app.state.rule_state["appointment_flow"]["chosen_place"] = PLACE_PARK
		var hs_fix: AppointmentRules.HolidayState = app.holiday_controller._build_holiday_state(app.state)
		hs_fix.chosen_place = PLACE_PARK
		var confirm_fix: Dictionary = AppointmentRules.build_appointment_confirm_Presentation(hs_fix, app.state.rule_state["appointment_words"])
		app.holiday_controller._save_holiday_state(app.state, hs_fix, {"confirm_data": confirm_fix})
		app._present_committed()
	_save_canvas_png(app, captures_dir.path_join("live_appointment_confirm_park.png"))

	# Press Circle (0x20) -> launches GAME_MODE_DATE (0x50) Date Arrival scene
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, AppointmentRules.GAME_MODE_DATE, "Confirm transitions to GAME_MODE_DATE (0x50)")
	_eq(app.state.mode_stage, 0, "Date Arrival starts at mode_stage 0")
	_eq(app.state.mode_step, 0, "Date Arrival starts at mode_step 0")
	_save_canvas_png(app, captures_dir.path_join("live_date_arrival_park.png"))

	# Verify Root's 3 Date Arrival Decomp Parity Requirements on the rendered Date Arrival scene:
	var arrival_render: Dictionary = app.holiday_controller.render_active_subscreen(app.state, app.calendar_phone_presenter, app.date_appointment_presenter)
	_eq(str(arrival_render.get("error", "")), "", "Date Arrival render succeeded")
	# Fix 1: Date Pillar shadow rect & position
	_eq(arrival_render.get("pillar_shadow_rect"), Rect2i(294, 45, 20, 114), "Date Pillar shadow rect is Rect2i(294, 45, 20, 114) (Fix 1)")

	# Fix 2: VN Scene Text Layout (speaker at x=-120 -> screen 40, quote at x=-86 -> screen 74, body at x=-72 -> screen 88, y=48 -> screen 168)
	var arrival_boxes: Array = arrival_render.get("glyph_bboxes", [])
	_eq(arrival_render.get("bbox_violations", []).size(), 0, "0 glyph bbox violations on Date Arrival")
	_ok(arrival_boxes.size() >= 10, "Date Arrival rendered speaker + quote + body glyphs")
	_eq(arrival_boxes[0]["rect"].position, Vector2i(40, 168), "Speaker '藤崎' starts at screen (40, 168) [native (-120, 48)] (Fix 2)")
	_eq(arrival_boxes[2]["rect"].position, Vector2i(74, 168), "Opening quote '「' is at screen (74, 168) [native (-86, 48)] (Fix 2)")
	_eq(arrival_boxes[3]["rect"].position, Vector2i(88, 168), "Body line starts at screen (88, 168) [native (-72, 48)] (Fix 2)")

	# Fix 3: Blinking/bobbing wait marker actor_pool[2] (frame = 1) at (116, 72 + ((frame_count >> 2) % 7))
	var wait_marker_0: Dictionary = arrival_render.get("wait_marker", {})
	_eq(int(wait_marker_0.get("actor_index", -1)), 2, "Wait marker uses actor_pool[2] (Fix 3)")
	_eq(int(wait_marker_0.get("frame_id", -1)), 1, "Wait marker uses direct HUD frame 1 (f001.base.p00.png) (Fix 3)")
	_eq(int(wait_marker_0.get("native_x", -1)), 116, "Wait marker native_x is 116 (0x74) (Fix 3)")
	var bob_y0: int = int(wait_marker_0.get("native_y", -1))
	_ok(bob_y0 >= 72 and bob_y0 <= 78, "Wait marker native_y is in 72..78 (0x48..0x4E) (Fix 3)")

	# Advance 4 neutral frames to verify bobbing wait marker moves by +1px (mod 7)
	for i: int in range(4):
		_press_app(app, 0)
	var arrival_render_bob: Dictionary = app.holiday_controller.render_active_subscreen(app.state, app.calendar_phone_presenter, app.date_appointment_presenter)
	var bob_y1: int = int(arrival_render_bob.get("wait_marker", {}).get("native_y", -1))
	_eq(bob_y1, 72 + (((bob_y0 - 72) + 1) % 7), "Wait marker bobbed by +1px after 4 frames (hud_text_marker_bob) (Fix 3)")
	_save_canvas_png(app, captures_dir.path_join("live_date_arrival_park_bob.png"))

	# =========================================================================
	# 5. Advance from Date Arrival to 3-Choice Date Menu (normal_date_three_select)
	# =========================================================================
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, AppointmentRules.GAME_MODE_DATE, "3-Choice menu stays in GAME_MODE_DATE (0x50)")
	_eq(app.state.mode_stage, 1, "3-Choice menu is at mode_stage 1")
	_eq(app.state.mode_step, 0, "3-Choice menu is at mode_step 0")
	_eq(int(app.state.rule_state["appointment_flow"]["date_choice_cursor"]), 0, "3-Choice cursor starts at row 0")
	_save_canvas_png(app, captures_dir.path_join("live_date_three_choice_row0.png"))

	# Verify Fix 4 on 3-Choice Date Menu:
	var choice0_render: Dictionary = app.holiday_controller.render_active_subscreen(app.state, app.calendar_phone_presenter, app.date_appointment_presenter)
	_eq(str(choice0_render.get("error", "")), "", "3-Choice prompt render succeeded")
	_eq(choice0_render.get("bbox_violations", []).size(), 0, "0 glyph bbox violations on 3-Choice prompt")
	_eq(choice0_render.get("highlight_rect"), Rect2i(32, 168, 252, 16), "Row 0 menu_bar_show(1) highlight rect is Rect2i(32, 168, 252, 16) (Fix 4)")
	_eq(choice0_render.get("cursor_native_pos"), Vector2i(120, 56), "Row 0 0x9C hand cursor is at native (120, 56) (Fix 4)")
	var choice_boxes: Array = choice0_render.get("glyph_bboxes", [])
	_eq(choice_boxes[0]["rect"].position, Vector2i(32, 168), "Option 0 opening quote '『' is at screen (32, 168) [native (-128, 48)] (Fix 4)")
	_eq(choice_boxes[1]["rect"].position, Vector2i(46, 168), "Option 0 text starts at screen (46, 168) [native (-114, 48)] (Fix 4)")

	# Move cursor Down -> row 1, capture, Down -> row 2, capture, Down -> wraps to row 0
	_press_app(app, PAD_DOWN)
	_eq(int(app.state.rule_state["appointment_flow"]["date_choice_cursor"]), 1, "Down moves 3-choice cursor to row 1")
	var choice1_render: Dictionary = app.holiday_controller.render_active_subscreen(app.state, app.calendar_phone_presenter, app.date_appointment_presenter)
	_eq(choice1_render.get("highlight_rect"), Rect2i(32, 184, 252, 16), "Row 1 menu_bar_show(1) highlight rect is Rect2i(32, 184, 252, 16) (Fix 4)")
	_eq(choice1_render.get("cursor_native_pos"), Vector2i(120, 72), "Row 1 0x9C hand cursor is at native (120, 72) (Fix 4)")
	_save_canvas_png(app, captures_dir.path_join("live_date_three_choice_row1.png"))

	_press_app(app, PAD_DOWN)
	_eq(int(app.state.rule_state["appointment_flow"]["date_choice_cursor"]), 2, "Down moves 3-choice cursor to row 2")
	var choice2_render: Dictionary = app.holiday_controller.render_active_subscreen(app.state, app.calendar_phone_presenter, app.date_appointment_presenter)
	_eq(choice2_render.get("highlight_rect"), Rect2i(32, 200, 252, 16), "Row 2 menu_bar_show(1) highlight rect is Rect2i(32, 200, 252, 16) (Fix 4)")
	_eq(choice2_render.get("cursor_native_pos"), Vector2i(120, 88), "Row 2 0x9C hand cursor is at native (120, 88) (Fix 4)")
	_save_canvas_png(app, captures_dir.path_join("live_date_three_choice_row2.png"))

	# Wrap Down -> row 0 and press Circle (0x20) to confirm Option 0 (Good impression, score_class = 0, delta = +6)
	_press_app(app, PAD_DOWN)
	_eq(int(app.state.rule_state["appointment_flow"]["date_choice_cursor"]), 0, "Down from row 2 wraps 3-choice cursor to row 0")
	_press_app(app, PAD_CONFIRM)
	_eq(app.state.game_mode, AppointmentRules.GAME_MODE_DATE, "Response stays in GAME_MODE_DATE (0x50)")
	_eq(app.state.mode_stage, 1, "Response is at mode_stage 1")
	_eq(app.state.mode_step, 1, "Confirming 3-choice option advances to mode_step 1 (Shiori response)")
	_eq(int(app.state.rule_state["appointment_flow"]["date_chosen_option"]), 0, "Selected logical choice_id is 0 (Good impression)")
	_eq(int(app.state.rule_state["appointment_flow"]["date_score_class"]), 0, "Score class is 0 (Good)")
	_eq(int(app.state.rule_state["appointment_flow"]["date_score"]), 10, "date_choice_score added +10 for Shiori Park choice 0")
	_save_canvas_png(app, captures_dir.path_join("live_date_response_good.png"))

	var resp_render: Dictionary = app.holiday_controller.render_active_subscreen(app.state, app.calendar_phone_presenter, app.date_appointment_presenter)
	_eq(str(resp_render.get("error", "")), "", "Shiori response render succeeded")
	_eq(resp_render.get("bbox_violations", []).size(), 0, "0 glyph bbox violations on Shiori response")
	var resp_boxes: Array = resp_render.get("glyph_bboxes", [])
	_ok(resp_boxes.size() >= 18, "Shiori 2-line response rendered all glyphs")
	_eq(resp_boxes[0]["rect"].position, Vector2i(40, 168), "Response speaker '藤崎' at screen (40, 168) [native (-120, 48)]")
	_eq(resp_boxes[2]["rect"].position, Vector2i(74, 168), "Response opening quote '「' at screen (74, 168) [native (-86, 48)]")
	_eq(resp_boxes[3]["rect"].position, Vector2i(88, 168), "Response line 0 starts at screen (88, 168) [native (-72, 48)]")
	_eq(int(resp_render.get("wait_marker", {}).get("frame_id", -1)), 1, "Response wait marker actor_pool[2] frame=1 is active")

	# Also render and capture Neutral (choice_id = 1) and Bad (choice_id = 2) responses for visual proof
	var hs_ok: AppointmentRules.HolidayState = app.holiday_controller._build_holiday_state(app.state)
	var resp_ok: Dictionary = AppointmentRules.build_date_choice_response(PLACE_PARK, 1)
	var out_ok: Dictionary = app.date_appointment_presenter.render_date_choice_response(hs_ok, resp_ok)
	_eq(out_ok.get("bbox_violations", []).size(), 0, "0 glyph bbox violations on Neutral response")
	var img_ok: Image = out_ok["image"]
	img_ok.save_png(captures_dir.path_join("live_date_response_ok.png"))

	var resp_bad: Dictionary = AppointmentRules.build_date_choice_response(PLACE_PARK, 2)
	var out_bad: Dictionary = app.date_appointment_presenter.render_date_choice_response(hs_ok, resp_bad)
	_eq(out_bad.get("bbox_violations", []).size(), 0, "0 glyph bbox violations on Bad response")
	var img_bad: Image = out_bad["image"]
	img_bad.save_png(captures_dir.path_join("live_date_response_bad.png"))

	app.canvas.free()
	app.pad.free()
	app.status_label.free()
	app.detail_label.free()
	app.free()
	if _failures == 0:
		print("PASS: holiday live loop integration & interactive date choice tests (%d assertions)" % _passes)
		quit(0)
	else:
		push_error("FAIL: holiday live loop integration & interactive date choice tests (%d failures)" % _failures)
		quit(1)

func _press_app(app: Node2D, mask: int) -> void:
	var tick: int = app.session.clock.logic_iterations
	app.session.latched_input = InputFrame.new(tick - 1, mask, 0)
	_step_app(app, InputFrame.new(tick, 0, mask))

func _step_app(app: Node2D, input: InputFrame) -> void:
	var tick: int = app.session.clock.logic_iterations
	var events: Array[Session.ServiceEvent] = []
	for descriptor: Dictionary in app.pending_services:
		events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, descriptor["service"], descriptor["payload"]))
	app.staged_commands.clear()
	app.staged_services.clear()
	app.staged_hud_refresh = false
	var result: Session.FrameResult = app.session.step(input, events)
	_ok(result.committed and result.error.is_empty(), "step %d committed cleanly: %s" % [tick, result.error])
	if not result.committed:
		return
	app.pending_services = app.staged_services.duplicate(true)
	app.trace.append({
		"iteration": tick,
		"state_hash": result.state_hash,
		"event_hash": result.event_hash,
		"mode": app.state.game_mode,
		"stage": app.state.mode_stage,
		"step": app.state.mode_step,
		"substep": app.state.substep,
		"frame_count": app.state.frame_count,
		"params": app.state.rule_state["params"].duplicate(),
		"rng": app.session.rng.state,
		"presentation": result.presentation,
		"audio": result.audio,
	})
	var present_err: String = app._present_committed()
	_eq(present_err, "", "step %d presented cleanly: %s" % [tick, present_err])

func _save_canvas_png(app: Node2D, path: String) -> void:
	var out: Image = app._last_rendered_image
	_ok(out != null and not out.is_empty(), "app has _last_rendered_image for " + path.get_file())
	if out == null or out.is_empty():
		return
	var err: Error = out.save_png(path)
	_eq(err, OK, "saved 320x240 capture " + path.get_file())

func _set_env(repo: String) -> void:
	OS.set_environment("TOKIMEMO_BOUNDARY", repo.path_join(".claude/orchestrator/state/T-29004/evidence/development-boundary.json"))
	OS.set_environment("TOKIMEMO_BOUNDARY_SHA", BOUNDARY_SHA)
	OS.set_environment("TOKIMEMO_TABLES", repo.path_join(".asset-cache/port/school-day-tables-v1-final/manifest.json"))
	OS.set_environment("TOKIMEMO_TABLES_SHA", TABLES_SHA)
	OS.set_environment("TOKIMEMO_MEDIA_PACK", repo.path_join(".asset-cache/port/holiday-command0-current-964ce"))
	OS.set_environment("TOKIMEMO_HUD_MANIFEST", repo.path_join(".asset-cache/port/hud-production-v1/manifest.json"))
	OS.set_environment("TOKIMEMO_HUD_SHA", HUD_SHA)
	OS.set_environment("TOKIMEMO_FONT_ROOT", repo.path_join(".asset-cache/port/fonts/native-font-v2"))
	OS.set_environment("TOKIMEMO_HUD_CONTEXT", repo.path_join(".claude/orchestrator/state/T-29010/app-integration-v1/proof/development-hud-context.json"))
	OS.set_environment("TOKIMEMO_HUD_CONTEXT_SHA", CONTEXT_SHA)
	OS.set_environment("TOKIMEMO_HOLIDAY_EXTENSION", repo.path_join(".claude/orchestrator/state/T-29013/evidence/holiday-boundary-extension.json"))
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT", repo.path_join(".asset-cache/port/holiday-hud-text-v1/manifest.json"))
	OS.set_environment("TOKIMEMO_HOLIDAY_TEXT_SHA", TEXT_SHA)
	OS.set_environment("TOKIMEMO_DATE_PACK", repo.path_join(".asset-cache/port/date-preview-v1"))
	OS.set_environment("TOKIMEMO_REQUIRE_HUD", "1")
	OS.set_environment("TOKIMEMO_REQUIRE_HOLIDAY", "1")
