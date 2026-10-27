extends SceneTree
## T-29026. External-original sequence coverage and actor-service lifecycle.
## This service test does not establish actual-app or emulator acceptance.

const ExactMedia = preload("res://services/native_activity_media.gd")
const OrdinaryMedia = preload("res://services/ordinary_command_media_adapter.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const State = preload("res://rules/school_day_state.gd")

var checks: int = 0
var failures: Array[String] = []
var lifecycles: Array[Dictionary] = []

func check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)

func _initialize() -> void:
	var ordinary := OrdinaryMedia.new()
	var error: String = ordinary.open_packs(OS.get_environment("TOKIMEMO_MEDIA_PACK"))
	check(error.is_empty(), "validated original media opens: " + error)
	if not error.is_empty():
		_finish()
		return
	var exact := ExactMedia.new()
	check(exact.open(ordinary).is_empty(), "exact activity media preflights source sequences")
	for pair: Array in [["command_0_sd", 4], ["holiday_command_0_sd", 2], ["command_7_sd", 1]]:
		var component: String = pair[0]
		var resources: Dictionary = ExactMedia.from_media(ordinary.media, component)
		check(resources.get("sequences", {}).size() == pair[1], component + " preserves every original slot")
		if resources.is_empty():
			continue
		for slot: int in range(pair[1]):
			var source: Array = OrdinaryMedia._integers(ordinary.media.get_sequence(component, slot))
			check(resources["sequences"].get(str(slot)) == source,
				"resource slot is the exact source sequence: %s/%d" % [component, slot])
			_test_lifecycle(component, slot, resources, source, 1)
			_test_lifecycle(component, slot, resources, source, 0x82)
		if pair[1] > 1:
			check(resources["sequences"]["0"] != resources["sequences"]["1"],
				component + " result1 is not a clone of result0")
			var zero_last: int = resources["sequences"]["0"][-1]["frame_index"]
			var one_last: int = resources["sequences"]["1"][-1]["frame_index"]
			check(resources["frames"][str(zero_last)]["id"] != resources["frames"][str(one_last)]["id"],
				component + " distinct result endpoints use distinct source texture identities")
		check(ordinary.media.get_sequence(component, pair[1]).is_empty(),
			component + " unavailable next slot remains absent")
	check(ExactMedia.from_media(ordinary.media, "missing_component").is_empty(),
		"missing component is never replaced with command0")
	var command0: Dictionary = exact.get_resources(0)
	check(command0.get("sequences", {}).size() == 4, "weekday command0 API exposes all source slots")
	check(exact.get_resources(6).is_empty(), "missing command6 resources are never replaced with command0")
	_test_selection(exact)
	_test_missing_resources(command0)
	_finish()

func _descriptor(slot: int, period: int = 1) -> Dictionary:
	# Invented service input with the supported source actor flags and geometry.
	return {"id": 96, "sequence_slot": slot, "x": -80, "y": -64,
		"flags": 5, "draw_flags": 0x84, "quad_flags": 0x01000000, "blink_period": period}

func _test_lifecycle(component: String, slot: int, resources: Dictionary, source: Array, period: int) -> void:
	var actor_service := ActorService.new()
	var initialized: Dictionary = actor_service.initialize([_descriptor(slot, period)], resources)
	var label: String = "%s/%d period%d" % [component, slot, period]
	check(initialized["error"].is_empty(), "actor initializes from exact slot: " + label)
	if not initialized["error"].is_empty():
		return
	var actor_state: Dictionary = initialized["actor_state"]
	var expected_frames: Array[int] = []
	for step: Dictionary in source:
		var duration: int = maxi(1, int(ceil(float(step["duration"]) / (2 if period == 0x82 else 1))))
		for _draw: int in range(duration):
			expected_frames.append(step["frame_index"])
	check(expected_frames.size() > 1 and actor_state["96"]["flags"] == 5,
		"actor begins active and requires real draws: " + label)
	var actual_ids: Array[String] = []
	for iteration: int in range(expected_frames.size()):
		var before: Dictionary = actor_state.duplicate(true)
		var drawn: Dictionary = actor_service.native_draw(actor_state, resources, iteration, Vector2i(160, 120))
		check(drawn["error"].is_empty(), "native draw succeeds: %s tick%d" % [label, iteration])
		if not drawn["error"].is_empty():
			return
		check(actor_state == before, "draw preparation preserves caller actor state: %s tick%d" % [label, iteration])
		var terminal: bool = iteration == expected_frames.size() - 1
		check(drawn["draw_commands"].size() == 1 and drawn["draw_commands"][0]["id"] == resources["frames"][str(expected_frames[iteration])]["id"],
			"old source frame is drawn before advancing: %s tick%d" % [label, iteration])
		actual_ids.append(drawn["draw_commands"][0]["id"])
		check(drawn["services"].size() == (1 if terminal else 0),
			"terminal service appears only at real sequence end: %s tick%d" % [label, iteration])
		actor_state = drawn["actor_state"]
		check((actor_state["96"]["flags"] & 1) == (0 if terminal else 1),
			"native active bit follows source duration: %s tick%d" % [label, iteration])
		if terminal:
			check(drawn["services"][0] == {"service": "school_day.actor96", "payload": {
				"active": false, "counter": 0, "sequence_slot": slot}},
				"terminal payload preserves exact source result slot/counter: " + label)
	var retained: Dictionary = actor_service.native_draw(actor_state, resources, expected_frames.size(), Vector2i(160, 120))
	check(retained["error"].is_empty() and retained["services"].is_empty()
		and retained["draw_commands"][0]["id"] == actual_ids[-1],
		"inactive actor retains its final source frame without a second completion: " + label)
	lifecycles.append({"component": component, "slot": slot, "period": period,
		"draws_to_terminal": expected_frames.size(), "draw_ids": actual_ids})

func _selection_state(command: int, month: int = 4) -> State:
	# A deliberately minimal invented resolver input; no player route is claimed.
	var state := State.new()
	state.game_mode = 0x40
	state.rule_state = {"day_events": [[0, command, 0, 15]], "day_event_index": 0,
		"player_flags": 11 << 12, "month": month, "vacation": 0}
	return state

func _test_selection(exact: ExactMedia) -> void:
	var selected: Dictionary = exact.resolve(_selection_state(0))
	check(selected["error"].is_empty() and selected.get("resources", {}).get("sequences", {}).size() == 4,
		"loaded non-summer command0 resolves without sequence truncation")
	check(not exact.resolve(_selection_state(6))["error"].is_empty(),
		"unloaded command6 selection is explicit unsupported")
	check(not exact.resolve(_selection_state(0, 7))["error"].is_empty(),
		"unloaded summer profile is never replaced with non-summer command0")
	var unready := ExactMedia.new()
	check(not unready.resolve(_selection_state(0))["error"].is_empty(),
		"unopened exact media cannot resolve a route")

func _test_missing_resources(resources: Dictionary) -> void:
	var actor_service := ActorService.new()
	check(not actor_service.initialize([_descriptor(99)], resources)["error"].is_empty(),
		"unavailable requested slot99 is never replaced with slot0")
	var missing: Dictionary = resources.duplicate(true)
	var frame_key: String = str(missing["sequences"]["0"][0]["frame_index"])
	missing["frames"].erase(frame_key)
	check(not actor_service.initialize([_descriptor(0)], missing)["error"].is_empty(),
		"missing required frame prevents actor initialization")
	var initialized: Dictionary = actor_service.initialize([_descriptor(0)], resources)
	if not initialized["error"].is_empty():
		check(false, "negative test source actor initializes")
		return
	var actor_state: Dictionary = initialized["actor_state"]
	var before: Dictionary = actor_state.duplicate(true)
	check(not actor_service.native_draw(actor_state, missing, 0, Vector2i(160, 120))["error"].is_empty()
		and actor_state == before, "disappearing frame refuses draw without actor-state mutation")
	missing = resources.duplicate(true)
	missing["sequences"].erase("0")
	check(not actor_service.native_draw(actor_state, missing, 0, Vector2i(160, 120))["error"].is_empty()
		and actor_state == before, "disappearing slot refuses draw without actor-state mutation")

func _finish() -> void:
	var report: Dictionary = {"scope": "original-media service lifecycle only; no actual-app or emulator acceptance",
		"checks": checks, "failures": failures, "lifecycles": lifecycles}
	var path: String = OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("exact-activity-media.json")
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(report, "\t"))
		file.close()
	for message: String in failures:
		push_error(message)
	print(JSON.stringify({"checks": checks, "failures": failures, "lifecycles": lifecycles.size()}))
	quit(0 if failures.is_empty() else 1)
