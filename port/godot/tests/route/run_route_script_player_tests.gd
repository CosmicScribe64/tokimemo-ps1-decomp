extends SceneTree
## T0 tests of tests/route/route_script_player.gd against the shared rule
## (tools/emu/route_script.py Rule). Synthetic observed states only; this proves
## the player's rule, never a route. The parity case reads
## tests/fixtures/route_rule_parity.json, written by
## tools/emu/test_route_script.py from the Python rule, and must give the same
## deliveries, anchors, skips, checkpoints and failure.
## Run: godot --headless --path port/godot --script res://tests/route/run_route_script_player_tests.gd
const Player = preload("res://tests/route/route_script_player.gd")
const PARITY := "res://tests/fixtures/route_rule_parity.json"
var checks := 0
var failures := 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures += 1
		printerr("FAIL: " + message)

func _initialize() -> void:
	_test_parity()
	_test_refusals()
	_test_pulse_until_and_skip()
	_test_input_slip()
	_test_pinned_stop_segment()
	print("%s: %d checks, %d failures" % ["PASS" if failures == 0 else "FAIL", checks, failures])
	quit(1 if failures else 0)

static func _row(fc: int, mode: int, stage := 0, step := 0, extra := {}) -> Dictionary:
	var row := {"frame_count": fc, "game_mode": mode, "mode_stage": stage, "mode_step": step, "substep": 0}
	row.merge(extra, true)
	return row

func _script(segments: Array, checkpoints := []) -> Dictionary:
	return {"schema": "tokimemo.input-script.v2", "name": "synthetic", "segments": segments, "checkpoints": checkpoints, "unsupported": []}

func _test_parity() -> void:
	var parsed = JSON.parse_string(FileAccess.get_file_as_string(PARITY))
	check(parsed is Dictionary, "parity fixture readable")
	if not parsed is Dictionary: return
	for case: Dictionary in parsed.cases:
		var player := Player.new()
		check(player.configure(case.script).is_empty(), case.name + ": configure")
		for row: Dictionary in case.rows:
			var state := row.duplicate()
			var pad := int(state.get("pad_held", -1))
			state.erase("pad_held")
			player.feed(state, pad)
			if player.done: break
		var expected: Dictionary = case.expected
		check(_norm(player.delivered) == _norm(expected.delivered), case.name + ": delivered %s vs %s" % [player.delivered, expected.delivered])
		check(_norm(player.anchors) == _norm(expected.anchors), case.name + ": anchors %s vs %s" % [player.anchors, expected.anchors])
		check(JSON.stringify(player.skipped) == JSON.stringify(expected.skipped), case.name + ": skipped")
		check(_norm(player.unsampled) == _norm(expected.unsampled), case.name + ": unsampled %s vs %s" % [player.unsampled, expected.unsampled])
		var ids: Array = player.fired.map(func(f): return f.id)
		var want_ids: Array = expected.checkpoints.map(func(f): return f.id)
		check(ids == want_ids, case.name + ": checkpoints %s vs %s" % [ids, want_ids])
		check(player.failure == expected.failure, case.name + ": failure '%s' vs '%s'" % [player.failure, expected.failure])
		check(player.stop == expected.stop, case.name + ": stop '%s' vs '%s'" % [player.stop, expected.stop])

## JSON round trip so integers from the fixture (floats) and from the player compare equal.
static func _norm(value: Variant) -> String:
	return JSON.stringify(JSON.parse_string(JSON.stringify(value)), "", true)

func _test_refusals() -> void:
	var player := Player.new()
	check(player.configure({"schema": "other"}) != "", "unknown schema refused")
	player = Player.new()
	check(player.configure(_script([{"id": "r", "kind": "repeat", "count": 2, "body": []}])).contains("unexpanded"), "repeat refused")
	player = Player.new()
	check(player.configure(_script([{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": [{"at": 1, "buttons": ["L2"], "hold": 2}]}])).contains("L2"), "L2 refused (analog axis in Godot)")

func _test_pulse_until_and_skip() -> void:
	var player := Player.new()
	player.configure(_script([{"id": "p", "kind": "pulse", "wait_until": {"game_mode": "0x30"},
		"pulse": {"buttons": ["CIRCLE"], "hold": 2, "every": 5, "offset": 1, "max": 3, "until": {"game_mode": "0x34", "mode_stage": 1, "mode_step": 0},
			"skip_when": [{"game_mode": "0x34"}]}}]))
	var sent: Array = []
	for fc in range(100, 130):
		var mode := 0x30 if fc < 106 else 0x34
		sent.append_array(player.feed(_row(fc, mode, 1 if fc >= 112 else 0)))
		if player.done: break
	# Pulses due for 101 and 106 are sent (observed 0x30); the one due for 111 is skipped (observed 0x34 stage 0).
	check(player.delivered.filter(func(d): return d.pressed).map(func(d): return d.frame_count) == [101, 106], "pulse deliveries %s" % [player.delivered])
	check(player.failure.is_empty() and player.done, "until ends the pulse segment: %s" % player.failure)
	player = Player.new()
	player.configure(_script([{"id": "x", "kind": "pulse", "wait_until": {"game_mode": "0x30"},
		"pulse": {"buttons": ["CIRCLE"], "hold": 1, "every": 4, "max": 2, "until": {"game_mode": "0x34"}}}]))
	for fc in range(200, 220):
		player.feed(_row(fc, 0x30))
		if player.done: break
	check(player.failure.begins_with("segment x: pulse exhausted after 2 presses"), "exhausted pulse fails: " + player.failure)

func _test_input_slip() -> void:
	var player := Player.new()
	player.configure(_script([{"id": "a", "wait_until": {"game_mode": "0x34"}, "events": [{"at": 2, "buttons": ["CIRCLE"], "hold": 2}]}]))
	player.feed(_row(10, 0x34), 0)
	player.feed(_row(11, 0x34), 0)
	player.feed(_row(12, 0x34), 0)
	check(player.failure.begins_with("input slip: CIRCLE due for iteration 12"), "a press missing from the sampled pad is an input slip: " + player.failure)

## The port observer's verdict (run_installed_long_route.gd): a stop is pinned
## only by a row with the same text AND the segment the player stopped on.
func _test_pinned_stop_segment() -> void:
	var text := "source_transaction: required route content capability is unavailable for selected command"
	var script := _script([{"id": "w001-confirm", "kind": "pulse", "wait_until": {"game_mode": "0x34"},
		"pulse": {"buttons": ["CIRCLE"], "hold": 2, "every": 60, "offset": 30, "max": 3, "until": {"game_mode": "0x30"}}},
		{"id": "w005-confirm", "kind": "pulse", "wait_until": {"game_mode": "0x34"},
		"pulse": {"buttons": ["CIRCLE"], "hold": 2, "every": 60, "offset": 30, "max": 3, "until": {"game_mode": "0x30"}}}])
	script["unsupported"] = [{"side": "port", "stop": text, "at_segment": "w001-confirm", "reason": "first weekly confirm"}]
	check(not Player.pinned_row(script, "port", text, "w001-confirm").is_empty(), "pinned at its segment")
	check(Player.pinned_row(script, "port", text, "w005-confirm").is_empty(), "same text at a later segment is not pinned")
	check(Player.pinned_row(script, "native", text, "w001-confirm").is_empty(), "other side is not pinned")
	check(Player.pinned_row(script, "port", "", "w001-confirm").is_empty(), "empty stop is never pinned")
	var player := Player.new()
	player.configure(script)
	check(player.current_segment_id() == "w001-confirm", "segment at stop is the current segment")
