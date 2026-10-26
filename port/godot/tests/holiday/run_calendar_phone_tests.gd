extends SceneTree
## T-29013: Verification tests and visual capture generation for:
## - Calendar Screen (icon 12)
## - Holiday Phone Book Screen (icon 8)
## - Holiday Magazine Screen (icon 10)

const Rules = preload("res://rules/calendar_phone_screens.gd")
const Presenter = preload("res://presentation/calendar_phone_presenter.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")

var _failures: Array[String] = []
var _pass_count: int = 0

func _init() -> void:
	print("==================================================================")
	print("Starting T-29013: Calendar, Phone Book & Magazine Screen Tests")
	print("==================================================================")

	_test_gte_rsin()
	_test_biorhythm_math()
	_test_calendar_cell_geometry()
	_test_calendar_rules_and_flow()
	_test_phone_directory_and_rules()
	_test_phone_dialing_flow()
	_test_magazine_rules_and_flow()
	_test_visual_rendering_and_captures()

	print("==================================================================")
	print("T-29013 Test Results: %d passed, %d failures" % [_pass_count, _failures.size()])
	if _failures.is_empty():
		print("ALL T-29013 TESTS PASSED PERFECTLY!")
		quit(0)
	else:
		for f in _failures:
			printerr("FAIL: ", f)
		quit(1)

func _check(condition: bool, description: String) -> void:
	if condition:
		_pass_count += 1
		print("  [PASS] ", description)
	else:
		_failures.append(description)
		printerr("  [FAIL] ", description)

func _test_gte_rsin() -> void:
	print("\n--- Testing PS1 GTE rsin() Accuracy ---")
	_check(Rules.rsin(0) == 0, "rsin(0) == 0")
	_check(Rules.rsin(0x400) == 4096, "rsin(0x400 = 90 deg) == 4096 (1.0 fixed point)")
	_check(Rules.rsin(0x800) == 0, "rsin(0x800 = 180 deg) == 0")
	_check(Rules.rsin(0xC00) == -4096, "rsin(0xC00 = 270 deg) == -4096 (-1.0 fixed point)")
	_check(Rules.rsin(0x1000) == 0, "rsin(0x1000 = 360 deg) == 0")
	_check(Rules.rsin(-0x400) == -4096, "rsin(-0x400) == -4096 (odd symmetry)")
	_check(Rules.rsin(0x200) == 2896, "rsin(0x200 = 45 deg) == 2896 (~0.7071 * 4096)")

func _test_biorhythm_math() -> void:
	print("\n--- Testing Biorhythm Birth Phase and Wave Calculation ---")
	# Birthday April 4 (month 4, day 4): birthday word = 4 | (4 << 4) = 0x44 = 68
	var b_offset: int = Rules.biorhythm_birth_offset(68)
	_check(b_offset == 5101, "Birthday April 4 phase offset == 5101 (hud_screens.c:1710)")

	# Early April (April 2): birthday word = 4 | (2 << 4) = 0x24 = 36
	var b_early: int = Rules.biorhythm_birth_offset(36)
	_check(b_early == 5469, "Birthday April 2 early-April phase offset == 5469 (hud_screens.c:1712)")

	# Verify wave range on day 5 (April 5)
	var val_p: int = Rules.biorhythm_value(5, 68, true, false, false, -32)
	var val_e: int = Rules.biorhythm_value(5, 68, false, true, false, -32)
	var val_i: int = Rules.biorhythm_value(5, 68, false, false, true, -32)
	_check(val_p >= -32 and val_p <= 32, "Physical wave on graph is within [-32, 32] pixel amplitude: %d" % val_p)
	_check(val_e >= -32 and val_e <= 32, "Emotional wave on graph is within [-32, 32] pixel amplitude: %d" % val_e)
	_check(val_i >= -32 and val_i <= 32, "Intellectual wave on graph is within [-32, 32] pixel amplitude: %d" % val_i)

func _test_calendar_cell_geometry() -> void:
	print("\n--- Testing Calendar Cell Coordinate Mapping ---")
	# Month 0 (April 1995), day 1: starts on Saturday (col 6, row 0)
	var pos_apr1: Vector2i = Rules.calendar_cell_pos(0, 0, 6)
	_check(pos_apr1.x == -0x78, "April 1 (first day) x coordinate == -0x78")
	_check(pos_apr1.y == -0x30, "April 1 y coordinate == -0x30")

	# Month 1 (May 1995), day 1: starts on Monday (col 1, row 0)
	var pos_may1: Vector2i = Rules.calendar_cell_pos(1, 0, 1)
	_check(pos_may1.x == 0x88 - 0x78 + 2 * 0x10, "May 1 x coordinate matches column offset")

func _test_calendar_rules_and_flow() -> void:
	print("\n--- Testing Calendar Screen State Machine and Mark Generation ---")
	var state := State.new()
	state.game_mode = 0x46 # Holiday
	state.mode_stage = 0x24 # Calendar subscreen
	state.mode_step = 0

	var input := InputFrame.new(0, 0, 0)
	var result := Session.FrameResult.new()

	# Step 0: Init
	Rules.step_calendar(state, input, result)
	_check(state.mode_step == 1, "Calendar step 0 advances to step 1")
	_check(result.presentation.size() == 1, "Calendar init presentation event emitted")
	_check(result.presentation[0]["title"] == "カレンダー", "Calendar title is カレンダー")

	# Step 1: Wait for PAD_CROSS
	input = InputFrame.new(1, 0x20, 0) # PAD_CIRCLE pressed (should not exit)
	result = Session.FrameResult.new()
	Rules.step_calendar(state, input, result)
	_check(state.mode_step == 1, "Calendar ignores CIRCLE input and remains in step 1")

	input = InputFrame.new(2, 0x40, 0) # PAD_CROSS pressed (exit)
	result = Session.FrameResult.new()
	Rules.step_calendar(state, input, result)
	_check(state.mode_step == 2, "Calendar advances to step 2 on CROSS")

	# Step 2: Exit
	result = Session.FrameResult.new()
	Rules.step_calendar(state, input, result)
	_check(state.mode_stage == 0 and state.mode_step == 2, "Calendar exits to mode_stage = 0, step = 2")
	_check(result.transitions.size() == 1, "Calendar exit transition emitted")

	# Test calendar mark builder (calendar_marks_build)
	var appointments := [
		{"active": true, "girl": 0, "month": 4, "day": 15, "period": 1}
	]
	var marks: Array[Dictionary] = Rules.build_calendar_marks(95, 4, 68, [], appointments, [], 0)
	_check(marks.size() >= 2, "Calendar marks built player birthday and appointment marks: %d" % marks.size())
	_check(marks[0]["frame"] == 0x6E and marks[0]["side"] == 0 and marks[0]["day_index"] == 3,
		"Player birthday mark placed on April 4")
	_check(marks[1]["frame"] == 0x62 and marks[1]["side"] == 0 and marks[1]["day_index"] == 14,
		"Appointment mark placed on April 15")

func _test_phone_directory_and_rules() -> void:
	print("\n--- Testing Holiday Phone Book Directory Rules ---")
	var girls: Array = []
	for i in range(11):
		girls.append({"flags": 0}) # b3 is 0

	var dir_closed: Array[Dictionary] = Rules.build_telephone_directory(girls)
	_check(dir_closed.size() == 14, "Phone directory has exactly 14 slots")
	_check(dir_closed[0]["name"] == "早乙女好雄" and dir_closed[0]["available"], "Slot 0 is Yoshio and available")
	_check(dir_closed[12]["name"] == "伊集院レイ" and dir_closed[12]["available"], "Slot 12 is Ray and available")
	_check(dir_closed[13]["name"] == "取り消し" and dir_closed[13]["available"], "Slot 13 is Cancel and available")
	_check(not dir_closed[1]["available"], "Slot 1 (Shiori) is unavailable when b3 flag is 0")

	girls[0]["flags"] = 0x08 # Set b3 flag for Shiori
	var dir_open: Array[Dictionary] = Rules.build_telephone_directory(girls)
	_check(dir_open[1]["name"] == "藤崎詩織" and dir_open[1]["available"], "Slot 1 (Shiori) becomes available when b3 flag is set")

	# Test 2D slot coordinates
	_check(dir_open[0]["x"] == -0x88 and dir_open[0]["y"] == -0x38, "Slot 0 coordinate is (-0x88, -0x38)")
	_check(dir_open[13]["x"] == 2 * 0x58 - 0x88, "Slot 13 x coordinate matches column 2")
	_check(dir_open[13]["y"] == (13 % 5) * 0x10 - 0x38 + 0x10, "Slot 13 y coordinate includes +0x10 offset")

func _test_phone_dialing_flow() -> void:
	print("\n--- Testing Holiday Phone Dialing and Audio Sequence ---")
	var state := State.new()
	state.game_mode = 0x46 # Holiday
	state.mode_stage = 0x1E # Holiday Tel
	state.mode_step = 0

	var girls: Array = []
	for i in range(11):
		girls.append({"flags": 0x08 if i == 0 else 0})
	state.rule_state["girls"] = girls

	var tel_state := {}
	var input := InputFrame.new(0, 0, 0)
	var result := Session.FrameResult.new()

	# Init
	Rules.step_holiday_tel(state, input, result, tel_state)
	_check(state.mode_step == 1, "Phone book advances to step 1 (selection)")
	_check(tel_state["cursor"] == 0, "Cursor initialized to Slot 0 (Yoshio)")

	# Move to Cancel slot (slot 13) and press CIRCLE: should exit to holiday menu
	tel_state["cursor"] = 13
	input = InputFrame.new(2, 0x20, 0) # PAD_CIRCLE
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, input, result, tel_state)
	_check(state.mode_stage == 0, "Confirming slot 13 (Cancel) exits to holiday menu stage 0")

	# Reset and select Yoshio (Slot 0)
	state.mode_stage = 0x1E
	state.mode_step = 1
	tel_state["cursor"] = 0
	input = InputFrame.new(3, 0x20, 0)
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, input, result, tel_state)
	_check(state.mode_step == 2, "Confirming contact advances to step 2 (dialing)")
	_check(tel_state["target_girl"] == 10, "Target girl for Yoshio is index 10")
	var has_select_sound: bool = false
	for a in result.audio:
		if a.get("command") == 0x50A:
			has_select_sound = true
	_check(has_select_sound, "Select sound 0x50A played")

	# Timer step 6 -> 7 emits dial tone 0x504
	state.mode_step_timer = 6
	input = InputFrame.new(4, 0, 0)
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, input, result, tel_state)
	var has_dial_tone: bool = false
	for a in result.audio:
		if a.get("command") == 0x504:
			has_dial_tone = true
	_check(has_dial_tone, "Dial tone command 0x504 emitted at timer tick 7")

	# Advance to substep 11 (call connect)
	tel_state["substep"] = 11
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, input, result, tel_state)
	_check(state.game_mode == 0x3E, "Call connected transitions to GAME_MODE_TELEPHONE_PROFILE (0x3E)")

func _test_magazine_rules_and_flow() -> void:
	print("\n--- Testing Holiday Magazine Rules and Reading Status ---")
	var quarter_flags: Array = [0, 0]
	# April 1995: Year 95, Month 4 -> Spring Quarter (quarter 0)
	Rules.mark_magazine_read(quarter_flags, 95, 4)
	var flag_word: int = (quarter_flags[1] << 8) | quarter_flags[0]
	_check((flag_word & 1) != 0, "Magazine read sets b0 for year 95 spring quarter")
	_check((flag_word & (1 << 13)) != 0 and (flag_word & (1 << 14)) != 0,
		"Magazine read sets b13 and b14 for year 95 month 4 special case")

	var hint_idx: int = Rules.get_magazine_hint_index(95, 4)
	_check(hint_idx == 0, "April 1995 hint index is 0 (biorhythm legend)")

	var issue: Dictionary = Rules.get_magazine_issue_data(95, 4)
	_check(issue["title"] == "春号", "Issue title is 春号")
	_check(issue["subtitle"] == "ＫＮＭ交響楽団", "Issue subtitle is ＫＮＭ交響楽団")
	_check(issue["article_lines"].size() == 3, "Issue has exactly 3 article lines")

	# Stepping flow
	var state := State.new()
	state.game_mode = 0x46
	state.mode_stage = 0x20
	state.mode_step = 0
	var mag_state := {}
	var input := InputFrame.new(0, 0, 0)
	var result := Session.FrameResult.new()

	Rules.step_holiday_magazine(state, input, result, mag_state)
	_check(state.mode_step == 1, "Magazine init advances to step 1")

	# Advance to hint
	input = InputFrame.new(1, 0x20, 0) # PAD_CIRCLE
	result = Session.FrameResult.new()
	Rules.step_holiday_magazine(state, input, result, mag_state)
	_check(mag_state["substep"] == 2, "Button press advances magazine to substep 2 (hint)")

	# Advance to exit
	input = InputFrame.new(2, 0x20, 0)
	result = Session.FrameResult.new()
	Rules.step_holiday_magazine(state, input, result, mag_state)
	_check(state.mode_step == 2, "Second button press advances to step 2 (exit)")

	result = Session.FrameResult.new()
	Rules.step_holiday_magazine(state, input, result, mag_state)
	_check(state.mode_stage == 0, "Magazine exits to holiday menu stage 0")

func _test_visual_rendering_and_captures() -> void:
	print("\n--- Testing Visual Rendering and 320x240 Captures ---")
	var presenter := Presenter.new()
	var font_root := "/workspace/.asset-cache/port/fonts/native-font-v2"
	var hud_manifest := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"

	var init_err: String = presenter.init_resources(font_root, hud_manifest)
	_check(init_err.is_empty(), "Presenter initialized with authentic font and HUD manifest: " + init_err)

	# 1. Render Calendar Screen matching emu_calendar.png
	var cal_img: Image = presenter.render_calendar_screen(5, 0x19B, 0)
	_check(cal_img != null and cal_img.get_width() == 320 and cal_img.get_height() == 240,
		"Calendar screen rendered at exact 320x240 dimensions")

	# 2. Render Holiday Phone Book Screen (Yoshio) matching emu_phone_book.png
	var phone_yoshio_img: Image = presenter.render_holiday_phone_screen(0, [], 0, "福原")
	_check(phone_yoshio_img != null and phone_yoshio_img.get_width() == 320 and phone_yoshio_img.get_height() == 240,
		"Holiday phone book screen (Yoshio) rendered at exact 320x240 dimensions")

	# 3. Render Holiday Phone Book Screen (Shiori) matching emu_phone_shiori.png
	var phone_shiori_img: Image = presenter.render_holiday_phone_screen(1, [], 0, "福原")
	_check(phone_shiori_img != null and phone_shiori_img.get_width() == 320 and phone_shiori_img.get_height() == 240,
		"Holiday phone book screen (Shiori) rendered at exact 320x240 dimensions")

	# 4. Render Holiday Magazine Screen matching emu_magazine.png
	var mag_img: Image = presenter.render_holiday_magazine_screen(0)
	_check(mag_img != null and mag_img.get_width() == 320 and mag_img.get_height() == 240,
		"Holiday magazine screen rendered at exact 320x240 dimensions")

	# 5. Automated pixel/glyph bounding-box assertion check
	print("\n--- Automated Glyph & Bounding Box Check ---")
	_check(presenter.bounding_box_violations.is_empty(),
		"Zero bounding box violations reported by presenter (%d violations)" % presenter.bounding_box_violations.size())

	var mw_overflows := 0
	var nb_overflows := 0
	for rec in presenter.recorded_glyphs:
		var p: Vector2i = rec["pos"]
		var region: String = rec["region"]
		if region == "message_window":
			if p.x < 27 or p.x + 12 > 293 or p.y < 166 or p.y + 12 > 218:
				mw_overflows += 1
		elif region == "notebook":
			if p.x < 0 or p.x + 12 > 283 or p.y < 33 or p.y + 12 > 158:
				nb_overflows += 1

	_check(mw_overflows == 0,
		"Assertion: NO glyph in bottom message window is drawn outside Rect2i(27, 166, 266, 52) (overflows: %d)" % mw_overflows)
	_check(nb_overflows == 0,
		"Assertion: NO notebook glyph in Calendar or Phone Book is drawn outside cal_base_show Rect2i(0, 33, 283, 125) (overflows: %d)" % nb_overflows)

	# Save 4 captures
	var cap_dir := "/workspace/.claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures"
	DirAccess.make_dir_recursive_absolute(cap_dir)

	var cal_cap_path := cap_dir.path_join("T-29013-calendar-screen-april-may.png")
	var phone_yoshio_path := cap_dir.path_join("T-29013-holiday-phone-book.png")
	var phone_shiori_path := cap_dir.path_join("T-29013-holiday-phone-shiori.png")
	var mag_cap_path := cap_dir.path_join("T-29013-holiday-magazine.png")

	_check(cal_img.save_png(cal_cap_path) == OK, "Saved calendar capture to: " + cal_cap_path)
	_check(phone_yoshio_img.save_png(phone_yoshio_path) == OK, "Saved phone book (Yoshio) capture to: " + phone_yoshio_path)
	_check(phone_shiori_img.save_png(phone_shiori_path) == OK, "Saved phone book (Shiori) capture to: " + phone_shiori_path)
	_check(mag_img.save_png(mag_cap_path) == OK, "Saved magazine capture to: " + mag_cap_path)
