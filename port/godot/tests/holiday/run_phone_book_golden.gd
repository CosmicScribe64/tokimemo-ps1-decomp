extends SceneTree
## Holiday phone book (mode 0x46 stage 0x10) against the original game.
## Fixture: tests/fixtures/dates_phone_golden_g2.json, derived from PCSX-Redux runs
## in .asset-cache/emu/dates-phone-golden-v1 (state route-select-008, Sunday 95/5/28).
## Tier: rules replay seeded from golden RAM (girl flag words), not the installed route.
## The same pad presses as the emulator run are replayed frame by frame from the
## first holiday_tel frame (41); every sound, its frame, the hand-over frame and
## the fields written for TEL (0x45) must match.

const Rules = preload("res://rules/calendar_phone_screens.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")

const FIXTURE := "res://tests/fixtures/dates_phone_golden_g2.json"
const PAD := {"DOWN": 0x4000, "UP": 0x1000, "LEFT": 0x8000, "RIGHT": 0x2000, "CIRCLE": 0x20, "CROSS": 0x40}
const FIRST_TEL_FRAME := 41

var _failures: Array[String] = []
var _checks: int = 0

func _check(ok: bool, label: String) -> void:
	_checks += 1
	if ok:
		print("  [PASS] " + label)
	else:
		_failures.append(label)
		print("  [FAIL] " + label)

func _initialize() -> void:
	var fixture: Variant = JSON.parse_string(FileAccess.get_file_as_string(FIXTURE))
	_check(fixture is Dictionary and fixture.get("schema") == "tokimemo.dates-phone-golden.v1", "golden fixture loads")
	if fixture is Dictionary:
		for name: String in ["g2-yoshio", "g2-shiori", "g2-rival"]:
			_replay(name, fixture["girl_flags"], int(fixture["start"]["menu_mode"]), fixture["runs"][name])
		_negative_wrong_table(fixture)
	print("phone book golden: %d checks, %d failures" % [_checks, _failures.size()])
	quit(1 if not _failures.is_empty() else 0)

static func _pad_presses(spec: String) -> Dictionary:
	var presses: Dictionary = {}
	for item: String in spec.split(","):
		var frame: int = int(item.get_slice(":", 0))
		var button: String = item.get_slice(":", 1).get_slice("/", 0)
		presses[frame] = PAD[button]
	return presses

static func _state(girl_flags: Array, menu_mode: int) -> State:
	var girls: Array = []
	for flags: Variant in girl_flags:
		girls.append({"flags": int(flags), "tokimeki_word": 0})
	var state := State.new()
	state.game_mode = 0x46
	state.set_stage(0x10)
	state.rule_state = {"girls": girls, "scene_work": 16, "current_girl": 8, "menu_cursor": 8,
		"menu_mode": menu_mode}
	return state

## Replays frames 41.. and returns {sounds, handover_frame, state}.
static func _run(girl_flags: Array, menu_mode: int, spec: String, last_frame: int) -> Dictionary:
	var presses: Dictionary = _pad_presses(spec)
	var state := _state(girl_flags, menu_mode)
	var sounds: Array = []
	var handover: int = -1
	for frame: int in range(FIRST_TEL_FRAME, last_frame):
		var result := Session.FrameResult.new()
		Rules.step_holiday_tel(state, InputFrame.new(frame, int(presses.get(frame, 0)), 0), result)
		if not result.error.is_empty():
			return {"error": result.error}
		for a: Dictionary in result.audio:
			if a.get("kind") == "native_audio_command":
				sounds.append([frame, int(a["command"])])
		if state.game_mode != 0x46:
			handover = frame
			break
	return {"error": "", "sounds": sounds, "handover_frame": handover, "state": state}

func _replay(name: String, girl_flags: Array, menu_mode: int, run: Dictionary) -> void:
	print("--- %s ---" % name)
	var out: Dictionary = _run(girl_flags, menu_mode, String(run["input"]), int(run["overlay_load_frame"]) + 2)
	_check(String(out["error"]).is_empty(), "%s replays without refusal" % name)
	if not String(out["error"]).is_empty():
		return
	var expected: Array = run["sounds"]
	var got: Array = out["sounds"]
	var same: bool = got.size() == expected.size()
	for i: int in range(mini(got.size(), expected.size())):
		same = same and int(got[i][0]) == int(expected[i][0]) and int(got[i][1]) == int(expected[i][1])
	_check(same, "%s sounds and frames match the original (got %s)" % [name, str(got)])
	# handover_frame is the frame of the original's game_mode_set(0x45) breakpoint hit
	# (dates-phone-golden-v2), not inferred from the overlay load frame.
	_check(int(out["handover_frame"]) == int(run["handover_frame"]),
		"%s hands over to 0x45 on the original's game_mode_set frame (port %d, original %d)" % [name, out["handover_frame"], run["handover_frame"]])
	var state: State = out["state"]
	var final: Dictionary = run["final"]
	_check(state.game_mode == int(final["game_mode"]) and state.mode_stage == int(final["mode_stage"]),
		"%s target 0x%02X stage %d" % [name, final["game_mode"], final["mode_stage"]])
	_check(int(state.rule_state["menu_cursor"]) == int(final["menu_cursor_0"])
		and int(state.rule_state["current_girl"]) == int(final["current_girl"])
		and int(state.rule_state["scene_work"]) == int(final["scene_work_at_handover"]),
		"%s menu_cursor_0 %d, current_girl %d, scene_work %d" % [name, final["menu_cursor_0"], final["current_girl"], final["scene_work_at_handover"]])

## Negative: the comparison must catch the old rows 11/12 and a grid navigation.
func _negative_wrong_table(fixture: Dictionary) -> void:
	print("--- negatives ---")
	var rival: Dictionary = fixture["runs"]["g2-rival"]
	var wrong_row: Array = [0, 0, 2, 1, 2, 0, 1] # the port's former row 11
	var expected_digits: Array = []
	for pair: Array in rival["sounds"]:
		if int(pair[1]) >= 0x505 and int(pair[1]) <= 0x507:
			expected_digits.append(int(pair[1]) - 0x505)
	# holiday_tel_exit reads girl_phone_digits[current_girl] and the rival's current_girl is 11.
	# Row 12 (same bytes) is read only by the dial shortcut; tools/port/test_phone_digits_executable.py
	# checks it against the executable.
	_check(expected_digits == Rules.GIRL_PHONE_DIGITS[11] and expected_digits != wrong_row,
		"original rival dial tones equal row 11 (current_girl 11) and reject the former row")
	# Shiori run with Shiori's phone_known cleared: DOWN skips to item 12, so the run diverges.
	var flags: Array = (fixture["girl_flags"] as Array).duplicate()
	flags[0] = int(flags[0]) & ~0x08
	var out: Dictionary = _run(flags, int(fixture["start"]["menu_mode"]), String(fixture["runs"]["g2-shiori"]["input"]), 400)
	_check(String(out["error"]).is_empty() and int(out["state"].rule_state["current_girl"]) == 11,
		"without girls[0].phone_known the same presses call the rival, not Shiori")
