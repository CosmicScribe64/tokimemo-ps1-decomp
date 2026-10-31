extends SceneTree
## T-29013: Verification tests and visual capture generation for:
## - Calendar Screen (icon 12)
## - Holiday Phone Book Screen (icon 8)
## - Holiday Magazine Screen (icon 10)

const NativeRng = preload("res://core/native_rng.gd")
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

func _phone_state(girls: Array) -> State:
	var state := State.new()
	state.game_mode = 0x46
	state.mode_stage = 0x10
	state.mode_step = 0
	state.rule_state["girls"] = girls
	state.rule_state["scene_work"] = 0
	# The holiday menu enters the phone book only with menu_mode 1 (holiday.gd).
	state.rule_state["menu_mode"] = 1
	return state

func _test_phone_dialing_flow() -> void:
	print("\n--- Testing Holiday Phone Dialing and Audio Sequence ---")
	var girls: Array = []
	for i in range(11):
		girls.append({"flags": 0x08 if i in [0, 10] else 0})
	var state := _phone_state(girls)
	var result := Session.FrameResult.new()

	# holiday_tel_init: telephone_class_init sets menu_cursor_0 = 0
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), result)
	_check(state.mode_step == 1, "Phone book advances to step 1 (selection)")
	_check(state.rule_state["menu_cursor"] == 0, "menu_cursor_0 initialized to item 0 (Yoshio)")

	# Item 13 (cancel) returns to the holiday menu stage 0
	state.rule_state["menu_cursor"] = 13
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(2, 0x20, 0), result)
	_check(state.mode_stage == 0 and state.mode_step == 0, "Confirming item 13 (cancel) is mode_stage_set(0)")

	# Cross also returns to stage 0
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x40, 0), Session.FrameResult.new())
	_check(state.mode_stage == 0, "Cross is mode_stage_set(0)")

	# menu_mode 1 cannot hold menu_cursor_0 == -1 into the CIRCLE test: menu_cursor_update
	# wraps it to count - 1 (item 13, cancel) first. The -1 case (SELECT) is in
	# _test_phone_book_cursor_branches.
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	state.rule_state["menu_cursor"] = -1
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x20, 0), result)
	_check(state.mode_stage == 0 and state.rule_state["menu_cursor"] == 13 and result.audio.is_empty(),
		"menu_cursor_0 -1 wraps to item 13 before CIRCLE, which cancels")

	_test_phone_book_cursor_branches(girls)

	# Yoshio: 0x504 then the row-10 digits at 8-frame spacing, then 0x45 stage 0
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x20, 0), result)
	_check(state.mode_step == 2 and state.rule_state["current_girl"] == 13, "Item 0 sets current_girl = GIRL_YOSHIO (13)")
	_check(result.audio.size() == 1 and result.audio[0].get("command") == 0x50A, "Select sound 0x50A played")
	var sounds: Array[int] = []
	var frames: Array[int] = []
	var restored: int = 0
	for tick in range(2, 200):
		result = Session.FrameResult.new()
		Rules.step_holiday_tel(state, InputFrame.new(tick, 0, 0), result)
		for a in result.audio:
			if a.get("kind") == "native_audio_command":
				sounds.append(int(a["command"]))
				frames.append(tick)
			elif a.get("kind") == "native_restore_bgm":
				restored += 1
		if state.game_mode != 0x46:
			break
	_check(sounds == [0x504, 0x505, 0x505, 0x505, 0x507, 0x507, 0x506, 0x505],
		"Yoshio dials 0x504 then row 10 0002210 (got %s)" % str(sounds))
	var spaced: bool = true
	for k in range(1, frames.size()):
		if frames[k] - frames[k - 1] not in [8, 32]:
			spaced = false
	_check(spaced, "Dial sounds sit on (timer & 7) == 7 frames")
	_check(state.game_mode == 0x45 and state.mode_stage == 0, "Yoshio hands over to 0x45 stage 0")
	_check(state.rule_state["menu_cursor"] == 0 and restored == 1, "Yoshio: menu_cursor_0 = 0 and one restore_bgm")

	# Item 12 (rival): current_girl 11, row 11 digits (equal to row 12), scene_work 11, 0x45 stage 1
	# (emulator, spec m-holiday 8.3: 0x504, 0x505 0x507 0x506 0x505 0x507 0x506 0x505)
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	state.rule_state["menu_cursor"] = 12
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x20, 0), Session.FrameResult.new())
	_check(state.rule_state["current_girl"] == 11, "Item 12 selects GIRL_MIHARU (11), the rival's phone slot")
	sounds.clear()
	restored = 0
	for tick in range(2, 200):
		result = Session.FrameResult.new()
		Rules.step_holiday_tel(state, InputFrame.new(tick, 0, 0), result)
		for a in result.audio:
			if a.get("kind") == "native_audio_command":
				sounds.append(int(a["command"]))
			elif a.get("kind") == "native_restore_bgm":
				restored += 1
		if state.game_mode != 0x46:
			break
	_check(sounds == [0x504, 0x505, 0x507, 0x506, 0x505, 0x507, 0x506, 0x505],
		"Item 12 dial tones match the emulator observation (got %s)" % str(sounds))
	_check(state.game_mode == 0x45 and state.mode_stage == 1 and state.rule_state["scene_work"] == 11
		and state.rule_state["menu_cursor"] == 12 and restored == 1,
		"Item 12 hands over to 0x45 stage 1 with scene_work 0xB, menu_cursor_0 12, restore_bgm")

	# Item 11 (Yumi, girl 10): stage 1, scene_work 10, menu_cursor_0 11
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	state.rule_state["menu_cursor"] = 11
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x20, 0), Session.FrameResult.new())
	state.substep = 11
	Rules.step_holiday_tel(state, InputFrame.new(2, 0, 0), Session.FrameResult.new())
	_check(state.game_mode == 0x45 and state.mode_stage == 1 and state.rule_state["scene_work"] == 10
		and state.rule_state["menu_cursor"] == 11, "Item 11 (girl 10) hands over to 0x45 stage 1, not Yoshio's stage 0")

	# Negative: an out-of-range current_girl at step 2 is refused, not dialed
	state = _phone_state(girls)
	state.mode_step = 2
	state.rule_state["current_girl"] = 12 + 2
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), result)
	_check(not result.error.is_empty() and state.game_mode == 0x46, "Out-of-range current_girl is refused")

	# Digit table rows 11 and 12 are both the rival's number (D_800B3778)
	_check(Rules.GIRL_PHONE_DIGITS.size() == 13, "13 digit rows")
	_check(Rules.GIRL_PHONE_DIGITS[11] == [0, 2, 1, 0, 2, 1, 0] and Rules.GIRL_PHONE_DIGITS[12] == [0, 2, 1, 0, 2, 1, 0],
		"Rows 11 and 12 are 0210210")
	_test_phone_dial_shortcut()

## menu_cursor_update(0) case 1 branches the phone book must follow (menu_speaker_girl.c:181).
## Directory with Shiori (item 1) and Yumi (item 11) known: selectable items 0, 1, 11, 12, 13.
func _test_phone_book_cursor_branches(girls: Array) -> void:
	print("\n--- Testing phone-book menu_cursor_update branches ---")
	var state := _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	var result := Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x4000, 0), result)
	_check(state.rule_state["menu_cursor"] == 1, "DOWN moves from item 0 to item 1")
	Rules.step_holiday_tel(state, InputFrame.new(2, 0x4000, 0), result)
	_check(state.rule_state["menu_cursor"] == 11, "DOWN skips the disabled items 2..10")
	Rules.step_holiday_tel(state, InputFrame.new(3, 0x1000, 0), result)
	_check(state.rule_state["menu_cursor"] == 1, "UP skips back over the disabled items")
	Rules.step_holiday_tel(state, InputFrame.new(4, 0x8000, 0), result)
	_check(state.rule_state["menu_cursor"] == 0, "LEFT is the same as UP")
	Rules.step_holiday_tel(state, InputFrame.new(5, 0x1000, 0), result)
	_check(state.rule_state["menu_cursor"] == 13, "UP from item 0 wraps to item 13")

	# Negative: UP+DOWN (0x5000) is no case of the 0xF000 switch, so nothing moves
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x5000, 0), result)
	_check(state.rule_state["menu_cursor"] == 0 and result.presentation.is_empty(), "UP+DOWN does not move the cursor")

	# Negative: a held shoulder button (pad_held & 0xF) blocks movement
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x4004, 0x4), Session.FrameResult.new())
	_check(state.rule_state["menu_cursor"] == 0, "Held L1 + DOWN does not move the cursor")
	Rules.step_holiday_tel(state, InputFrame.new(2, 0x4000, 0), Session.FrameResult.new())
	_check(state.rule_state["menu_cursor"] == 1, "DOWN moves again once L1 is released")

	# SELECT: menu_mode++ and menu_cursor_0 = -1; CIRCLE on that frame is then ignored
	state = _phone_state(girls)
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x120, 0), result)
	_check(state.rule_state["menu_mode"] == 2 and state.rule_state["menu_cursor"] == -1, "SELECT steps menu_mode to 2 and clears menu_cursor_0")
	_check(state.mode_step == 1 and result.audio.is_empty() and result.error.is_empty(), "CIRCLE with SELECT selects nothing")
	# Negative: menu_mode 2 (pointer control) is refused, not run as pad control
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(2, 0x4000, 0), result)
	_check(not result.error.is_empty() and state.rule_state["menu_cursor"] == -1,
		"Phone book with menu_mode 2 is refused")
	# Negative: a state without the native menu_mode field is refused
	state = _phone_state(girls)
	state.rule_state.erase("menu_mode")
	Rules.step_holiday_tel(state, InputFrame.new(0, 0, 0), Session.FrameResult.new())
	result = Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0x4000, 0), result)
	_check(not result.error.is_empty(), "Phone book without menu_mode is refused")

func _shortcut_state(phone_known: Array[int], remedial: bool) -> State:
	var girls: Array = []
	for i in range(16):
		girls.append({"flags": 0x08 if i in phone_known else 0, "tokimeki_word": 0})
	var state := State.new()
	state.game_mode = 0x46
	state.mode_stage = 1
	state.rule_state = {
		"girls": girls, "scene_work": 0, "menu_cursor": 9,
		"player_flags_upper": 1 if remedial else 0, "menu_mode": 1,
		"params": [0, 0, 0, 0, 0, 0, 0, 0, 0x00100000],
		"holiday": {"dial_keys": [255, 255, 255, 255, 255, 255, 255, 0], "dial_pos": 0},
	}
	return state

func _dial(state: State, rng: NativeRng, keys: Array, x: int, y: int) -> bool:
	var matched: bool = false
	for key in keys:
		var pad: int = [0x20, 0x40, 0x10][key]
		matched = Rules.holiday_phone_dial_shortcut(state, pad, x, y, rng, Session.FrameResult.new())
	return matched

func _test_phone_dial_shortcut() -> void:
	print("\n--- Testing holiday_phone_dial_shortcut ---")
	# Emulator (m-holiday 8.2): circle cross triangle cross circle triangle circle on girl 0
	var state := _shortcut_state([0], false)
	var rng := NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	var expected_rng := NativeRng.new()
	expected_rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	var bonus: int = expected_rng.next("expected") & 0x7FFF
	var matched: bool = _dial(state, rng, [0, 1, 2, 1, 0, 2, 0], -0x48, 0)
	_check(matched, "Shiori's number on the phone picture matches")
	_check(state.rule_state["holiday"]["dial_keys"].slice(0, 7) == [0, 1, 2, 1, 0, 2, 0], "Ring holds 0,1,2,1,0,2,0")
	_check(state.mode_stage == 0x10 and state.mode_step == 2 and state.substep == 11, "Enters phone stage 0x10 step 2 substep 11")
	_check(state.rule_state["current_girl"] == 0 and state.rule_state["menu_cursor"] == 0 and state.rule_state["scene_work"] == 0,
		"current_girl 0, scene_work 0, menu_cursor_0 0 (telephone_class_init resets it on the shortcut frame)")
	_check(state.rule_state["girls"][0]["tokimeki_word"] == bonus, "Tokimeki word gains rand() & 0x7FFF")
	_check(state.rule_state["params"][8] == 0x00100000, "No stress change outside the remedial week")
	# Next frame of the phone book hands over without dial tones
	var result := Session.FrameResult.new()
	Rules.step_holiday_tel(state, InputFrame.new(1, 0, 0), result)
	_check(state.game_mode == 0x45 and state.mode_stage == 1, "Shortcut then goes straight to 0x45 stage 1")
	_check(state.rule_state["menu_cursor"] == 1, "holiday_tel_exit sets menu_cursor_0 1 on the next frame")

	# Remedial week: stress +0x20 on the integer half
	state = _shortcut_state([0], true)
	rng = NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	_dial(state, rng, [0, 1, 2, 1, 0, 2, 0], -0x48, 0)
	_check(state.rule_state["params"][8] == 0x00300000, "Remedial week adds 0x20 to stress")

	# Negative: girl without phone_known does not match and draws no rand()
	state = _shortcut_state([], false)
	rng = NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	_check(not _dial(state, rng, [0, 1, 2, 1, 0, 2, 0], -0x48, 0), "No match without phone_known")
	_check(rng.draw_count == 0 and state.mode_stage == 1, "No rand() and no stage change without a match")

	# Negative: pointer outside the hot spot records nothing
	state = _shortcut_state([0], false)
	rng = NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	_check(not _dial(state, rng, [0, 1, 2, 1, 0, 2, 0], -0x40, 0), "x = -0x40 is outside the hot spot")
	_check(state.rule_state["holiday"]["dial_pos"] == 0 and state.rule_state["holiday"]["dial_keys"][0] == 255, "Ring untouched outside the hot spot")

	# Row 12 (rival) maps to slot 11 with menu_cursor_0 12; row 11 never sets a slot
	state = _shortcut_state([12], false)
	rng = NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	_check(_dial(state, rng, [0, 2, 1, 0, 2, 1, 0], -0x4F, -0xF), "Rival's number matches at the hot spot's low corner")
	_check(state.rule_state["current_girl"] == 11 and state.rule_state["scene_work"] == 11 and state.rule_state["menu_cursor"] == 0,
		"Row 12 sets current_girl 11, scene_work 11; menu_cursor_0 12 is reset to 0 by telephone_class_init")
	Rules.step_holiday_tel(state, InputFrame.new(1, 0, 0), Session.FrameResult.new())
	_check(state.rule_state["menu_cursor"] == 12 and state.game_mode == 0x45, "holiday_tel_exit then sets menu_cursor_0 12")
	_check(state.rule_state["girls"][12]["tokimeki_word"] != 0 and state.rule_state["girls"][11]["tokimeki_word"] == 0,
		"Bonus goes to girls[12], the matched row")

	state = _shortcut_state([11], false)
	rng = NativeRng.new()
	rng.configure(NativeRng.OPENBIOS_PROFILE, 1)
	_check(_dial(state, rng, [0, 2, 1, 0, 2, 1, 0], -0x48, 0), "Row 11 matches when girls[11] has phone_known")
	_check(state.rule_state["menu_cursor"] == 0 and state.rule_state["scene_work"] == 0 and not state.rule_state.has("current_girl"),
		"Row 11 sets no slot (source quirk); telephone_class_init still resets menu_cursor_0 to 0")

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

	# 1. Render Calendar Screen matching emu_calendar.png (phase=40, title settled at -28,-28)
	var cal_img: Image = presenter.render_calendar_screen(5, 0x19B, 40)
	_check(cal_img != null and cal_img.get_width() == 320 and cal_img.get_height() == 240,
		"Calendar screen rendered at exact 320x240 dimensions")

	# 2. Render Holiday Phone Book Screen (Yoshio) matching emu_phone_book.png
	var phone_yoshio_img: Image = presenter.render_holiday_phone_screen(0, [], 0, "福原")
	_check(phone_yoshio_img != null and phone_yoshio_img.get_width() == 320 and phone_yoshio_img.get_height() == 240,
		"Holiday phone book screen (Yoshio) rendered at exact 320x240 dimensions")

	# 3. Render Holiday Phone Book Screen (Shiori dialing substep 1) matching emu_phone_shiori.png
	var phone_shiori_img: Image = presenter.render_holiday_phone_screen(1, [], 1, "福原")
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

	# 6. Full-frame 16x16 tile hotspot verification against PS1 emulator reference captures
	var emu_dir := "/workspace/.claude/orchestrator/state/T-29013/calendar-phone-screens-v1/emu-reference"
	var emu_mag_path := emu_dir.path_join("emu_magazine.png")
	if FileAccess.file_exists(emu_mag_path):
		var emu_mag := Image.new()
		if emu_mag.load(emu_mag_path) == OK:
			var mag_hotspots := _count_16x16_hotspots(emu_mag, mag_img, Rect2i(0, 0, 320, 240), 20.0)
			_check(mag_hotspots == 0, "16x16 tile pass: Magazine full frame (320x240) has 0 tiles > 20 MAE vs emu_magazine.png (got %d)" % mag_hotspots)
	var emu_pb_path := emu_dir.path_join("emu_phone_book.png")
	if FileAccess.file_exists(emu_pb_path):
		var emu_pb := Image.new()
		if emu_pb.load(emu_pb_path) == OK:
			var nb_hotspots := _count_16x16_hotspots(emu_pb, phone_yoshio_img, Rect2i(0, 0, 320, 240), 20.0)
			_check(nb_hotspots == 0, "16x16 tile pass: Phone Book (Yoshio) full frame (320x240) has 0 tiles > 20 MAE vs emu_phone_book.png (got %d)" % nb_hotspots)
	var emu_shiori_path := emu_dir.path_join("emu_phone_shiori.png")
	if FileAccess.file_exists(emu_shiori_path):
		var emu_shiori := Image.new()
		if emu_shiori.load(emu_shiori_path) == OK:
			var shiori_hotspots := _count_16x16_hotspots(emu_shiori, phone_shiori_img, Rect2i(0, 0, 320, 240), 20.0)
			_check(shiori_hotspots == 0, "16x16 tile pass: Phone Book (Shiori) full frame (320x240) has 0 tiles > 20 MAE vs emu_phone_shiori.png (got %d)" % shiori_hotspots)
	var emu_cal_path := emu_dir.path_join("emu_calendar.png")
	if FileAccess.file_exists(emu_cal_path):
		var emu_cal := Image.new()
		if emu_cal.load(emu_cal_path) == OK:
			var cal_hotspots := _count_16x16_hotspots(emu_cal, cal_img, Rect2i(0, 0, 320, 240), 20.0)
			_check(cal_hotspots == 0, "16x16 tile pass: Calendar full frame (320x240) has 0 tiles > 20 MAE vs emu_calendar.png (got %d)" % cal_hotspots)


func _count_16x16_hotspots(emu: Image, gd: Image, region: Rect2i, threshold: float) -> int:
	var hot := 0
	for ty in range(region.position.y, region.position.y + region.size.y, 16):
		for tx in range(region.position.x, region.position.x + region.size.x, 16):
			var err := 0.0
			var cnt := 0
			for y in range(maxi(1, ty), mini(ty + 16, region.position.y + region.size.y)):
				for x in range(maxi(1, tx), mini(tx + 16, region.position.x + region.size.x)):
					var pe := emu.get_pixel(x, y)
					var pg := gd.get_pixel(x, y)
					err += (absf(pe.r8 - pg.r8) + absf(pe.g8 - pg.g8) + absf(pe.b8 - pg.b8)) / 3.0
					cnt += 1
			if cnt > 0 and (err / float(cnt)) > threshold:
				hot += 1
	return hot

