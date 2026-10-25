extends SceneTree
## Rest continuation uses the current canonical adapter, with no save schema change.

const Harness = preload("res://tests/app/run_command7_app_tests.gd")
const Boot = preload("res://app/playable_boot.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const Save = preload("res://services/save_service.gd")

var failures: Array[String] = []
var checks: int = 0

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func step(app: Boot) -> Array:
	var tick: int = app.session.clock.logic_iterations
	Harness.device_recipe(app, 7, tick)
	app._physics_process(1.0 / 60.0)
	return [app.trace[-1]["state_hash"], app.trace[-1]["event_hash"]]

func _initialize() -> void:
	var harness: Harness = Harness.new()
	var source: Boot = harness.make_app()
	var resumed: Boot = harness.make_app()
	var versions: Dictionary = {"source_version": "rest-week-source-v1", "content_version": "command7-canonical-v2",
		"behavior_version": "captured-ordinary-rest-week-v1"}
	var adapter: Adapter = Adapter.new(versions)
	var disk: Save = Save.new(OS.get_environment("TOKIMEMO_REST_SAVE_OUTPUT").path_join("rest-save"))
	var command: Dictionary = adapter.capture(source, "command")
	check(command["error"].is_empty(), "command snapshot accepts current canonical configuration")
	if not command["error"].is_empty():
		finish(harness, [source, resumed])
		return
	check(disk.save_slot("command", command["snapshot"])["error"].is_empty(), "command snapshot publishes through current codec")
	var loaded: Dictionary = disk.load_slot("command")
	check(loaded["error"].is_empty() and adapter.restore(resumed, loaded["snapshot"]).is_empty(), "command restore preserves full rest-week policy")
	var original_hashes: Array = []
	var command_hashes: Array = []
	while source.state.rule_state["day_event_index"] != 1 and source.session.clock.logic_iterations < 301:
		original_hashes.append(step(source))
		command_hashes.append(step(resumed))
	check(original_hashes.size() == 163 and original_hashes == command_hashes, "all163 command-to-rest-result hashes survive command reload")
	check(not source.stopped and source.continue_week and source.checkpoints.size() == 2, "first-result boundary remains a continuing stable checkpoint")
	var first: Dictionary = adapter.capture(source, "first_result")
	check(first["error"].is_empty(), "unstopped rest first-result captures without changing save adapter")
	var restored_first: Boot = harness.make_app()
	if first["error"].is_empty():
		check(disk.save_slot("first", first["snapshot"])["error"].is_empty(), "continuing first-result snapshot publishes")
		loaded = disk.load_slot("first")
		check(loaded["error"].is_empty() and adapter.restore(restored_first, loaded["snapshot"]).is_empty(), "continuing first-result restores into fresh app")
		check(not restored_first.stopped and restored_first.continue_week, "restore retains explicit week continuation")
		var suffix: Array = []
		var restored_suffix: Array = []
		while not source.stopped and source.session.clock.logic_iterations < 601:
			var before: int = source.trace.size()
			var row: Array = step(source)
			var copy: Array = step(restored_first)
			step(resumed)
			if source.trace.size() == before + 1:
				suffix.append(row)
				restored_suffix.append(copy)
		check(suffix.size() == 172 and suffix == restored_suffix, "all172 remaining rest-week pairs survive first-result reload")
		check(source.trace == resumed.trace and source.trace == restored_first.trace, "all335 complete route trace rows equal uninterrupted")
		check(source.session.state_hash() == resumed.session.state_hash() and source.session.state_hash() == restored_first.session.state_hash(), "final state/input/RNG/clock hashes equal across both restore paths")
		check(source.stopped and source.failure.is_empty() and not source.supported_stop.is_empty(), "all restored paths retain the supported holiday refusal")
		check(source.checkpoints == resumed.checkpoints and source.checkpoints == restored_first.checkpoints and source.checkpoints.size() == 15, "all15 native checkpoint records survive both restores")
		check(not adapter.capture(source, "first_result")["error"].is_empty(), "later weekday boundary is not silently granted a new save checkpoint")
	finish(harness, [source, resumed, restored_first])

func finish(harness: Harness, apps: Array) -> void:
	check(harness.failures.is_empty(), "all real input/media preflights succeeded")
	for app: Boot in apps:
		harness.dispose(app)
	harness.free()
	var output: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_REST_SAVE_OUTPUT").path_join("rest-week-save.json"), FileAccess.WRITE)
	output.store_string(JSON.stringify({"checks": checks, "failures": failures}, "\t"))
	output.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures}))
	quit(0 if failures.is_empty() else 1)
