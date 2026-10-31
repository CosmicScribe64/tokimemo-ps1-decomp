extends RefCounted
## T-39301: the function-differential harness itself (tests/engine/engine_diff.gd) on invented captures:
## it must pass a correct port and fail loudly (with GameState field names) on a wrong one, check the
## unported callees it replays, and refuse exclusions without a reason. Invented data only.

const GameEngine = preload("res://engine/engine.gd")
const Differ = preload("res://tests/engine/engine_diff.gd")
const Memory = preload("res://core/game_memory.gd")

var failures: Array[String] = []
var _made: Array = []

## Engines are shut down at the end of run() so the engine <-> module cycles are freed.
func _new():
	var engine = GameEngine.new()
	# a callee nobody ported is replayed from the capture (T-39312 ported message_window_title_show: forget it here)
	for function_name: String in ["message_window_title_show"]:
		engine._functions.erase(function_name)
		engine._callables.erase(function_name)
	_made.append(engine)
	return engine

func _shutdown_all() -> void:
	for engine in _made:
		engine.shutdown()
	_made.clear()

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	_test_pass_and_fail()
	_test_callees()
	_test_exclusions_and_returns()
	_test_names_and_json()
	_shutdown_all()
	return failures

## Diff of two RAM images as a capture stores it: [[offset, "hex"], ...].
static func _diff(a: PackedByteArray, b: PackedByteArray) -> Array:
	var runs: Array = []
	var start: int = -1
	for i: int in range(Memory.RAM_SIZE):
		if a[i] != b[i]:
			if start < 0:
				start = i
		elif start >= 0:
			runs.append([start, b.slice(start, i).hex_encode()])
			start = -1
	if start >= 0:
		runs.append([start, b.slice(start).hex_encode()])
	return runs

## The invented entry state: mode_step 5, step timer 7, substep 3, game mode 0x31.
func _entry() -> PackedByteArray:
	var memory := Memory.new()
	var gs = memory.game_state()
	gs.set_field("game_mode", 0x31)
	gs.set_field("mode_step", 5)
	gs.at("mode_step_timer").set_field("w", 7)
	gs.set_field("substep", 3)
	return memory.snapshot()

func _hit(entry: PackedByteArray, exit_ram: PackedByteArray, a0: int = 0, callees: Array = []) -> Dictionary:
	return {"a0": a0, "a1": 0, "a2": 0, "a3": 0, "sp": 0x801FFE00, "stack": [], "ret_v0": 0, "ret_v1": 0,
		"exit_diff": _diff(entry, exit_ram), "callees": callees}

func _expected_step_next(entry: PackedByteArray) -> PackedByteArray:
	var memory := Memory.new()
	memory.restore(entry)
	var gs = memory.game_state()
	gs.set_field("mode_step", 6)
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("substep", 0)
	return memory.snapshot()

func _test_pass_and_fail() -> void:
	var engine = _new()
	var differ = Differ.new()
	var entry: PackedByteArray = _entry()
	var good: PackedByteArray = _expected_step_next(entry)
	var signature: Dictionary = engine.modules.game_mode.C_SIGNATURES.mode_step_next
	var result: Dictionary = differ.run_hit(engine, "mode_step_next", _hit(entry, good), entry, signature, [])
	_check(result.ok and result.problems.is_empty() and result.compared_bytes == Memory.RAM_SIZE - 0x4000 - 16, "a correct port passes and compares the whole RAM minus the dead stack and the argument home: %s %s" % [result.problems, result.compared_bytes])
	# the original left substep alone (the port clears it): a difference named by its GameState field
	var memory := Memory.new()
	memory.restore(good)
	memory.game_state().set_field("substep", 3)
	result = differ.run_hit(engine, "mode_step_next", _hit(entry, memory.snapshot()), entry, signature, [])
	_check(not result.ok and "\n".join(result.problems).contains("GameState.substep"), "a wrong expectation fails and names GameState.substep: %s" % [result.problems])
	# a different step value: v0 is compared for an s32 function
	var hit: Dictionary = _hit(entry, good)
	hit.ret_v0 = 9
	result = differ.run_hit(engine, "mode_step_next", hit, entry, signature, [])
	_check(not result.ok and "\n".join(result.problems).contains("v0 = 0x0"), "v0 is compared for a non-void return: %s" % [result.problems])
	# ... and not for a void one
	hit = _hit(entry, good)
	hit.ret_v0 = 9
	result = differ.run_hit(engine, "mode_step_next", hit, entry, {"args": [], "ret": "void"}, [])
	_check(result.ok, "v0 is not compared for a void function")
	# an exclusion with a reason hides a field; without a reason it is an error
	memory.restore(good)
	memory.game_state().set_field("substep", 3)
	var wrong: Dictionary = _hit(entry, memory.snapshot())
	result = differ.run_hit(engine, "mode_step_next", wrong, entry, signature, [{"field": "GameState.substep", "reason": "invented test reason", "port": "free"}])
	_check(result.ok, "an excluded field that the port may write is not compared: %s" % [result.problems])
	result = differ.run_hit(engine, "mode_step_next", wrong, entry, signature, [{"field": "GameState.substep", "reason": "invented test reason"}])
	_check(not result.ok and "\n".join(result.problems).contains("port wrote") and "\n".join(result.problems).contains("GameState.substep"),
		"exclusions are one-sided: the original may differ, the port must leave the bytes as at entry: %s" % [result.problems])
	# a not-comparable hit passes only inside its allowance
	result = differ.run_hit(engine, "mode_step_next", wrong, entry, signature, [], [{"field": "GameState.substep", "reason": "t"}])
	_check(not result.ok and result.within_allowance, "a difference inside the allowance is within_allowance")
	result = differ.run_hit(engine, "mode_step_next", wrong, entry, signature, [], [{"field": "GameState.mode_step", "reason": "t"}])
	_check(not result.within_allowance, "a difference outside the allowance is not")
	result = differ.run_hit(engine, "mode_step_next", wrong, entry, signature, [{"field": "GameState.substep"}])
	_check(not result.ok and "\n".join(result.problems).contains("needs a reason"), "an exclusion without a reason is refused: %s" % [result.problems])

func _test_callees() -> void:
	var engine = _new()
	var differ = Differ.new()
	var entry: PackedByteArray = _entry()
	# game_mode_set(0x30): stores, then message_window_title_show(0) (ovl_target_game_mode is 0 here)
	var memory := Memory.new()
	memory.restore(entry)
	var gs = memory.game_state()
	gs.set_field("game_mode", 0x30)
	gs.set_field("mode_step", 0)
	gs.at("mode_step_timer").set_field("w", 0)
	gs.set_field("substep", 0)
	var before_call: PackedByteArray = memory.snapshot()
	memory.write_u32(0x80100000, 0xCAFE) # the callee's recorded effect
	var exit_ram: PackedByteArray = memory.snapshot()
	var record: Dictionary = {"name": "message_window_title_show", "a0": 0, "a1": 0, "a2": 0, "a3": 0, "ret_v0": 0, "ret_v1": 0,
		"pre_diff": _diff(entry, before_call), "effect_diff": _diff(before_call, exit_ram)}
	var hit: Dictionary = _hit(entry, exit_ram, 0x30, [record])
	var signature: Dictionary = engine.modules.game_mode.C_SIGNATURES.game_mode_set
	var result: Dictionary = differ.run_hit(engine, "game_mode_set", hit, entry, signature, [])
	_check(result.ok and result.callees_replayed == 1, "an unported callee is replayed from the capture and its effect applied: %s" % [result.problems])
	# the port reaches the callee in a different state than the original did
	var bad: Dictionary = record.duplicate()
	bad.pre_diff = _diff(entry, entry) # the original reached it with the entry memory unchanged
	result = differ.run_hit(engine, "game_mode_set", _hit(entry, exit_ram, 0x30, [bad]), entry, signature, [])
	_check(not result.ok and "\n".join(result.problems).contains("at the call of message_window_title_show"), "the memory at the callee call is checked: %s" % [result.problems])
	# wrong argument
	bad = record.duplicate()
	bad.a0 = 5
	result = differ.run_hit(engine, "game_mode_set", _hit(entry, exit_ram, 0x30, [bad]), entry, signature, [])
	_check(not result.ok and "\n".join(result.problems).contains("argument 0 = 0x0, the original passed 0x5"), "callee arguments are compared: %s" % [result.problems])
	# the C did not call the callee at all (the port does): a refusal, reported
	result = differ.run_hit(engine, "game_mode_set", _hit(entry, exit_ram, 0x30, []), entry, signature, [])
	_check(not result.ok and "\n".join(result.problems).contains("not ported"), "a callee the original did not call is a failure: %s" % [result.problems])
	# the C called one more callee than the port reached
	result = differ.run_hit(engine, "mode_step_next", _hit(entry, _expected_step_next(entry), 0, [record]), entry, engine.modules.game_mode.C_SIGNATURES.mode_step_next, [])
	_check(not result.ok and "\n".join(result.problems).contains("the C called 1 unported callee"), "a callee the port never reached is a failure: %s" % [result.problems])

func _test_exclusions_and_returns() -> void:
	var differ = Differ.new()
	var regions: Array = differ._exclusions([], 0x801FFE00, [] as Array[String])
	_check(regions.size() == 2 and regions[0][0] == 0x1FFE00 - 0x4000 and regions[0][1] == 0x1FFE00 and regions[1][0] == 0x1FFE00 and regions[1][1] == 0x1FFE10,
		"the dead stack and the o32 argument home area are always excluded: %s" % [regions])
	var pieces: Array = Differ._subtract([10, 50], [[0, 20, "a"], [30, 35, "b"], [49, 60, "c"]])
	_check(pieces == [[20, 30], [35, 49]], "_subtract leaves what the regions do not cover: %s" % [pieces])
	_check(Differ._diff_runs(PackedByteArray([1, 2, 3]).duplicate(), PackedByteArray([1, 2, 3]).duplicate()).is_empty(), "equal images have no runs")

func _test_names_and_json() -> void:
	var differ = Differ.new()
	_check(differ.describe(0x800E7388) == "GameState.game_mode" and differ.describe(0x800E738B) == "GameState.game_mode_scratch", "describe names GameState fields")
	_check(differ.describe(0x80125128) == "global cd_queue_head" and differ.describe(0x80125131).begins_with("global cd_command_ring"), "describe names globals")
	_check(Differ.field_range("GameState.frame_count") == [0xE7378, 0xE737C], "field_range gives the RAM range of a field: %s" % [Differ.field_range("GameState.frame_count")])
	var hit: Dictionary = Differ.parse_hit('{"a0": 4294967295, "exit_diff": [[947076, "00"]], "callees": []}')
	_check(hit.a0 == 0xFFFFFFFF and hit.exit_diff[0][0] == 947076, "capture JSON numbers come back as ints")
