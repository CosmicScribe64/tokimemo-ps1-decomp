extends SceneTree
## T-39023 (T-39018 WP-06): synthetic tests of input_script_player.gd. A fake
## game node counts iterations in its own _physics_process and sees the pad only
## through _input, like the production scenes. Checks that each edge reaches
## exactly the scripted iterations, that segments anchor on observed state, and
## that unmapped buttons, timeouts and wrong expect_next states fail loudly.
## Run: godot --headless --path port/godot --script res://tests/support/run_input_script_player_tests.gd
const Player = preload("res://tests/support/input_script_player.gd")
const MASKS := {JOY_BUTTON_DPAD_UP: 0x1000, JOY_BUTTON_DPAD_RIGHT: 0x2000, JOY_BUTTON_A: 0x20, JOY_BUTTON_START: 0x800}

class FakeGame extends Node:
	var device := 0
	var frame_count := 100
	var mode_step := 1
	var held := 0
	var transient := false
	var samples: Dictionary = {}
	func _input(event: InputEvent) -> void:
		if event is InputEventJoypadButton and event.device == device:
			var mask: int = MASKS.get(event.button_index, 0)
			held = (held | mask) if event.pressed else (held & ~mask)
	func _physics_process(_delta: float) -> void:
		frame_count += 1
		samples[frame_count] = held
		if transient and mode_step == 2: mode_step = 3
		if held & 0x800 and mode_step == 1: mode_step = 2
	func observe() -> Dictionary:
		return {"frame_count": frame_count, "game_mode": 0x14, "mode_stage": 1, "mode_step": mode_step, "substep": 0}

var checks := 0
var failed := 0
var ticks := 0
var runs: Array = []

func check(value: bool, message: String) -> void:
	checks += 1
	if not value:
		failed += 1
		printerr("FAIL: " + message)

func script(segments: Array) -> Dictionary:
	return {"schema": "tokimemo.input-script.v1", "segments": segments}

func seg_a(expect_step: int = 2) -> Dictionary:
	return {"id": "a", "wait_until": {"game_mode": "0x14", "mode_stage": 1, "mode_step": 1},
		"events": [{"at": 5, "buttons": ["RIGHT"], "hold": 3}, {"at": 20, "buttons": ["START"], "hold": 2}],
		"expect_next": {"game_mode": "0x14", "mode_stage": 1, "mode_step": expect_step}}

func start(device: int, data: Dictionary) -> Dictionary:
	var game := FakeGame.new()
	game.device = device
	var player := Player.new()
	player.device = device
	var error := player.configure(data, game.observe)
	root.add_child(player)
	root.add_child(game)
	var run := {"game": game, "player": player, "error": error}
	runs.append(run)
	return run

func _initialize() -> void:
	var probe := Player.new()
	var refused := probe.configure(script([{"id": "x", "wait_until": {"mode_stage": 1},
		"events": [{"at": 1, "buttons": ["L2"], "hold": 1}]}]), func() -> Dictionary: return {})
	check(refused.contains("L2"), "analog L2 is refused, not approximated: " + refused)
	check(probe.configure({"schema": "other"}, func() -> Dictionary: return {}) != "", "foreign schema refused")
	probe.free()
	start(0, script([seg_a(), {"id": "b", "wait_until": {"game_mode": "0x14", "mode_stage": 1, "mode_step": 2},
		"events": [{"at": 4, "buttons": ["CIRCLE", "UP"], "hold": 1}]}]))
	start(1, script([{"id": "never", "wait_until": {"mode_step": 9}, "timeout_iterations": 10, "events": []}]))
	start(2, script([seg_a(3)]))
	start(3, script([seg_a(2)])).game.transient = true
	# Repeated observation of one iteration is not a second boundary: no duplicate edges.
	var twice := Player.new()
	var counter := {"fc": 100}
	twice.configure(script([{"id": "a", "wait_until": {"mode_step": 1}, "events": [{"at": 2, "buttons": ["START"], "hold": 1}]}]),
		func() -> Dictionary: return {"frame_count": counter.fc, "game_mode": 0x14, "mode_stage": 1, "mode_step": 1, "substep": 0})
	twice.device = 7
	for fc: int in [100, 101, 101, 101, 102, 102]:
		counter.fc = fc
		twice.boundary()
	check(twice.delivered.size() == 2 and twice.delivered[0].frame_count == 102, "one press and one release despite repeated observations: %s" % [twice.delivered])
	twice.free()
	var skip := Player.new()
	skip.configure(script([{"id": "a", "wait_until": {"mode_step": 1}, "events": [{"at": 3, "buttons": ["START"], "hold": 1}]}]),
		func() -> Dictionary: return {"frame_count": counter.fc, "game_mode": 0x14, "mode_stage": 1, "mode_step": 1, "substep": 0})
	skip.device = 8
	for fc: int in [100, 101, 104]:
		counter.fc = fc
		skip.boundary()
	check(skip.failure.contains("without a boundary"), "skipped boundary with a due edge fails: " + skip.failure)
	skip.free()
	# A segment whose expect_next never comes fails after EXPECT_LIMIT iterations instead of hanging.
	var stuck := Player.new()
	stuck.configure(script([{"id": "a", "wait_until": {"mode_step": 1}, "events": [{"at": 1, "buttons": ["START"], "hold": 1}],
		"expect_next": {"mode_step": 2}}]),
		func() -> Dictionary: return {"frame_count": counter.fc, "game_mode": 0x14, "mode_stage": 1, "mode_step": 1, "substep": 0})
	stuck.device = 9
	for fc: int in range(100, 100 + Player.EXPECT_LIMIT + 10):
		counter.fc = fc
		stuck.boundary()
	check(stuck.failure.contains("still in its own state"), "missing expect_next fails instead of hanging: " + stuck.failure)
	stuck.free()
	_expect_state_cases()
	physics_frame.connect(_tick)

## expect_state is checked on the expect_next state: the right transition with the
## wrong entered values (a missed kana, a random preset name) or values the
## observer cannot report must fail, never pass.
func _expect_state_cases() -> void:
	var segment := {"id": "family", "wait_until": {"mode_step": 1}, "events": [{"at": 2, "buttons": ["START"], "hold": 1}],
		"expect_next": {"mode_step": 2},
		"expect_state": {"names": {"family": "82bf814081400000"}, "player": {"month": 1, "blood": 1}}}
	var cases := [
		["entered values match (hex case-insensitive)", {"family": "82BF814081400000"}, {"month": 1, "day": 255, "blood": 1}, ""],
		["blank name became a preset", {"family": "8e528add81400000"}, {"month": 1, "day": 255, "blood": 1}, "names.family: expected 82bf814081400000"],
		["wrong blood", {"family": "82bf814081400000"}, {"month": 1, "day": 2, "blood": 0}, "player.blood: expected 1, observed 0"],
		["values not observable", null, null, "names.family: expected 82bf814081400000, observed unavailable"]]
	for row: Array in cases:
		var state := {"fc": 100, "step": 1}
		var names: Variant = row[1]
		var profile: Variant = row[2]
		var player := Player.new()
		player.device = 10
		player.configure(script([segment.duplicate(true)]), func() -> Dictionary:
			var observed := {"frame_count": state.fc, "game_mode": 0x14, "mode_stage": 1, "mode_step": state.step, "substep": 0}
			if names != null: observed["names"] = names
			if profile != null: observed["player"] = profile
			return observed)
		for fc: int in range(100, 106):
			state.fc = fc
			state.step = 1 if fc < 104 else 2
			player.boundary()
		var want: String = row[3]
		if want.is_empty():
			check(player.failure == "" and player.done, "%s: passes (%s)" % [row[0], player.failure])
		else:
			check(player.failure.contains("expect_state failed") and player.failure.contains(want), "%s: fails (%s)" % [row[0], player.failure])
		player.free()
	var direct := Player.state_mismatches({"shiori": {"month": 0, "day": 0}}, {"shiori": {"month": 0, "day": 3}})
	check(direct.size() == 1 and direct[0] == "shiori.day: expected 0, observed 3", "state_mismatches names the differing field: %s" % [direct])

func _tick() -> void:
	ticks += 1
	if ticks < 90: return
	physics_frame.disconnect(_tick)
	var ok: Dictionary = runs[0]
	var game: FakeGame = ok.game
	var player: Player = ok.player
	check(ok.error == "", "valid script configures: " + str(ok.error))
	check(player.failure == "" and player.done, "script finished without failure: " + player.failure)
	check(player.anchors == {"a": 100, "b": 120}, "anchors on observed state: %s" % [player.anchors])
	for fc: int in range(101, 130):
		var right: bool = game.samples.get(fc, 0) & 0x2000 != 0
		check(right == (fc >= 105 and fc <= 107), "RIGHT held exactly at iterations 105-107 (fc %d: %s)" % [fc, right])
		var start_held: bool = game.samples.get(fc, 0) & 0x800 != 0
		check(start_held == (fc >= 120 and fc <= 121), "START held exactly at iterations 120-121 (fc %d)" % fc)
		var circle: bool = game.samples.get(fc, 0) & 0x1020 == 0x1020
		check(circle == (fc == 124), "CIRCLE+UP chord exactly at iteration 124 (fc %d)" % fc)
	check(game.mode_step == 2, "START changed the fake game only through its own _input")
	check(player.delivered.size() == 8, "four presses and four releases delivered: %d" % player.delivered.size())
	var timeout: Player = runs[1].player
	check(timeout.failure.contains("not reached within 10"), "timeout fails loudly: " + timeout.failure)
	var wrong: Player = runs[2].player
	check(wrong.failure.contains("expected"), "wrong expect_next state fails: " + wrong.failure)
	check(runs[2].game.held == 0, "failure releases every held button")
	check(runs[3].player.failure == "" and runs[3].game.mode_step == 3, "a one-iteration expect_next state is seen: " + runs[3].player.failure)
	print(("PASS" if failed == 0 else "FAIL") + ": input script player %d checks, %d failed" % [checks, failed])
	quit(1 if failed else 0)
