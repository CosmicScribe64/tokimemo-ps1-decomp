extends SceneTree
## T-29026. Synthetic base-icon predicate cases from holiday_icons_set_available.
## This tests the phase before separate health/remedial overrides. It does not
## establish complete sick-menu behavior, content readiness, or a played route.

const Rules = preload("res://rules/holiday.gd")
const State = preload("res://rules/school_day_state.gd")
const HolidayHud = preload("res://presentation/holiday_hud_adapter.gd")

var checks: int = 0
var failures: Array[String] = []
var cases: Array[Dictionary] = []
var hud: HolidayHud = HolidayHud.new()

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func _initialize() -> void:
	# Invented animation records are sufficient for the pure history replay seam.
	# No original graphics, text, tables, or production capability is embedded.
	var sequences: Array = []
	for icon: int in range(14):
		sequences.append({"steps": [{"duration": 3}]})
	hud.set("_manifest", {"tables": {"sequences": sequences}})
	_case("healthy Sunday NONE can join", 6, 11, _records(), true)
	_case("healthy Saturday NONE cannot use club", 5, 11, _records(), false)
	_case("synthetic April29,1995 Saturday NONE disables icon4 before input", 5, 11,
		_records(), false, 0, 0, 0, [95, 4, 29])
	for member: int in range(11):
		_case("Saturday member%d can use club" % member, 5, member, _records(), true)
		_case("Sunday all eleven quit disables member%d" % member, 6, member, _records(11, 1), false)
	_case("Sunday all eleven quit disables NONE", 6, 11, _records(11, 1), false)
	_case("Sunday ten quit still permits club", 6, 11, _records(10, 1), true)
	var last_record_only: Array = _records()
	last_record_only[11] = 1
	_case("record11 quit flag is excluded", 6, 11, last_record_only, true)
	var ten_and_last: Array = _records(10, 1)
	ten_and_last[11] = 1
	_case("ten eligible quits plus excluded record11 still permits club", 6, 11, ten_and_last, true)
	_case("all eleven high record bits do not count as quit", 6, 11, _records(11, 0xfe), true)
	_case("bit0 counts even when other record bits are present", 6, 3, _records(11, 0x81), false)
	for weekday: int in range(6):
		_case("non-Sunday%d NONE uses upper club nibble" % weekday, weekday, 11, _records(), false)
		_case("non-Sunday%d membership ignores quit history" % weekday, weekday, 0, _records(11, 1), true)
	for health: int in [0, 1, 2, 3, 15]:
		for remedial: int in [0, 1]:
			var phase: String = "base preoverride health%d remedial%d" % [health, remedial]
			_case(phase + " Saturday NONE", 5, 11, _records(), false, health, remedial)
			_case(phase + " Saturday member", 5, 2, _records(11, 1), true, health, remedial)
			_case(phase + " Sunday NONE can join", 6, 11, _records(), true, health, remedial)
			_case(phase + " Sunday member all quit", 6, 2, _records(11, 1), false, health, remedial)
	_case("bits above native club nibble do not change NONE", 5, 11, _records(), false, 0, 0, 1 << 20)
	_case("bits above native club nibble do not change membership", 5, 1, _records(), true, 0, 0, 1 << 20)
	_finish()

func _records(count: int = 0, flags: int = 0) -> Array:
	var records: Array = []
	records.resize(12)
	records.fill(0)
	for index: int in range(count):
		records[index] = flags
	return records

func _case(label: String, weekday: int, club: int, records: Array, available: bool,
	health: int = 0, remedial: int = 0, other_flags: int = 0, date: Array = []) -> void:
	var state := State.new()
	var icons: Array = []
	icons.resize(15)
	icons.fill(73)
	var disabled: Array = []
	disabled.resize(14)
	disabled.fill(22)
	var params: Array = []
	params.resize(9)
	params.fill(27 << 16)
	state.rule_state = {"weekday": weekday, "player_flags": (club << 12) | (health << 8) | other_flags,
		"player_flags_upper": remedial, "holiday": {"clubs": records.duplicate(), "icon_count": 0},
		"icon_enabled": icons, "menu_disabled": disabled, "menu_cursor": 5, "last_command": 0xa7,
		"resources": {"available_holiday_commands": [0]}, "params": params}
	if not date.is_empty():
		state.rule_state["year"] = date[0]
		state.rule_state["month"] = date[1]
		state.rule_state["day"] = date[2]
	var original: Dictionary = state.rule_state.duplicate(true)
	check(Rules.club_icon_available(state.rule_state) == available, label + ": source base predicate")
	check(state.rule_state == original, label + ": predicate is read-only")
	for weekday_screen: bool in [false, true]:
		state.rule_state = original.duplicate(true)
		var role: String = "weekday screen" if weekday_screen else "holiday screen"
		Rules.icons(state, weekday_screen)
		var expected: Array = []
		expected.resize(14)
		expected.fill(1)
		for locked: int in ([4, 8, 9, 10] if weekday_screen else [9, 13]):
			expected[locked] = 0
		if not weekday_screen and not available:
			expected[4] = 0
		check(state.rule_state["icon_enabled"].slice(0, 14) == expected,
			label + ": " + role + " changes only the source locked icons")
		check(state.rule_state["icon_enabled"][14] == 73,
			label + ": " + role + " leaves the extra icon record untouched")
		check(state.rule_state["holiday"]["icon_count"] == 14 and state.rule_state["menu_disabled"].all(func(value: Variant) -> bool: return value == 0),
			label + ": " + role + " preserves fourteen-icon/menu initialization")
		check(state.rule_state["menu_cursor"] == (7 if weekday_screen else 5),
			label + ": " + role + " preserves source cursor ownership")
		check(state.rule_state["holiday"]["clubs"] == records and state.rule_state["player_flags"] == original["player_flags"]
			and state.rule_state["player_flags_upper"] == remedial,
			label + ": " + role + " preserves membership, history, health, and remedial inputs")
		var replay: Dictionary = _replay(state.rule_state, weekday_screen)
		check(replay["error"].is_empty(), label + ": " + role + " synthetic history replays: " + replay["error"])
		if replay["error"].is_empty():
			check(replay["flags"] == expected, label + ": " + role + " rule/HUD icon flags agree")
			check(replay["elapsed"][4] == (1 if expected[4] == 1 else 0),
				label + ": " + role + " animates icon4 only when available")
	cases.append({"label": label, "weekday": weekday, "club": club, "health": health,
		"remedial": remedial, "date": date.duplicate(), "expected_base_club_available": available})

func _replay(rule: Dictionary, weekday_screen: bool) -> Dictionary:
	var mode: int = 0x34 if weekday_screen else 0x46
	var screen: String = "next_command" if weekday_screen else "holiday"
	var prompt: String = "settings_command_screen_build/default" if weekday_screen else "holiday_open_init/default"
	var core: Dictionary = {"game_mode": mode, "mode_stage": 1, "mode_step": 0,
		"substep": 0, "frame_count": 12}
	var history: Array[Dictionary] = [{"mode": mode, "stage": 1, "step": 0, "substep": 0,
		"frame_count": 12, "params": rule["params"].duplicate(),
		"presentation": [{"kind": "holiday_screen_init", "screen": screen, "prompt": prompt, "icon_count": 14}]}]
	var before: Dictionary = rule.duplicate(true)
	var replay: Dictionary = hud._replay(history, core, rule)
	check(rule == before, screen + ": HUD replay is read-only")
	return replay

func _finish() -> void:
	var report: Dictionary = {"scope": "synthetic base club-icon predicate and HUD replay, before health/remedial overrides; no sick-menu or player-route readiness claim",
		"checks": checks, "failures": failures, "cases": cases}
	var output: String = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT")
	if not output.is_empty():
		var file: FileAccess = FileAccess.open(output.path_join("holiday-club-icon.json"), FileAccess.WRITE)
		if file != null:
			file.store_string(JSON.stringify(report, "\t"))
			file.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "synthetic_base_cases": cases.size()}))
	quit(0 if failures.is_empty() else 1)
