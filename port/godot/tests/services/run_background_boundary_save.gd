extends SceneTree
## T-29023. Exact V17 records reconstructed after genuine Boot preflight, plus
## explicit physical-cache permutations. This is not a fresh played route.

const Boot = preload("res://app/playable_boot.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
const InputFrame = preload("res://core/input_frame.gd")
const ROUTE_SHA: String = "45f648a812e2c0958231972899348fdadbbcb5b40b2f24c2f33cabcfa8c2c01d"
const BEFORE_SHA: String = "8c17b6fd3ddb4e80b25be4d81e9859d03460755a0f527d9f53d97520c98989f1"
const VERSIONS: Dictionary = {"source_version": "t29023-v17-cache-regression",
	"content_version": "original-preflight", "behavior_version": "bounded-physical-source-pair"}

var checks: int = 0
var failures: Array[String] = []
var cases: Array[Dictionary] = []
var diagnostics: Dictionary = {}
var app: Boot

func _initialize() -> void:
	call_deferred("_run")

func ck(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures.append(label)

func _install(point: Dictionary, lba: int) -> void:
	app.state.restore(point["state"])
	app.session.rng.restore(point["rng"])
	app.session.clock.restore(point["clock"])
	app.pending_services.assign(point["pending_services"])
	app.staged_services.assign(point["staged_services"])
	var input: Dictionary = point["input_latch"]
	app.session.latched_input = InputFrame.new(input["tick"], input["held"],
		(input["held"] & ~input["pressed"]) | input["released"])
	app.pad.clear_devices()
	app.pad.set("_previous", input["held"])
	app.staged_commands.assign(app.holiday_scene.backgrounds[lba])
	app.canvas.present(app.staged_commands)
	ck(app.session.state_hash() == point["state_hash"], "reconstruction retains the immutable captured state hash")

func logical() -> String:
	return JSON.stringify([app.state.projection(), app.session.rng.checkpoint(),
		app.session.clock.snapshot(), app.pending_services, app.staged_services,
		app.session.latched_input.snapshot(), app.session.state_hash(),
		app.controller.month_start.identity()])

func fingerprint() -> Dictionary:
	var pixels: Dictionary = {}
	for key: String in app.canvas.get("_textures"):
		pixels[key] = _sha(app.canvas.get("_textures")[key].get_image().get_data())
	return {"logical": logical(), "session": app.session, "controller": app.controller,
		"holiday": app.holiday_controller, "source": app.controller.month_start,
		"draw": app.staged_commands.duplicate(true), "canvas": app.canvas.get("_commands").duplicate(true),
		"texture_pixels": pixels}

func _run() -> void:
	app = Boot.new()
	app.canvas = Boot.Canvas.new()
	app.pad = Boot.Pad.new()
	app.status_label = Label.new()
	app.detail_label = Label.new()
	var error: String = app._load_inputs()
	if error.is_empty():
		error = app._reset()
	ck(error.is_empty(), "actual Boot original-input preflight: " + error)
	var path: String = OS.get_environment("TOKIMEMO_SAVE_RAW_ROUTE")
	ck(FileAccess.get_sha256(path) == ROUTE_SHA, "fixture is immutable V17 source route")
	if not error.is_empty() or FileAccess.get_sha256(path) != ROUTE_SHA:
		_finish()
		return
	var route: Dictionary = Boot._integers(JSON.parse_string(FileAccess.get_file_as_string(path)))
	var actual: Dictionary = route["final"]
	_install(actual, 16318)
	ck([app.state.rule_state["year"], app.state.rule_state["month"], app.state.rule_state["day"],
		app.state.rule_state["day_event_index"]] == [95, 5, 7, 15], "exact final fixture is May7 Sunday queue15")
	ck(app.state.rule_state["background_cache"] == [16318, 16131, 0, 0]
		and app.state.rule_state["background_show"] == 0, "exact V17 physical slot0 caused the original refusal")
	var raw_draw: Array = actual["draw_records"]["staged"]
	ck(raw_draw.size() == app.staged_commands.size(), "actual source draw count matches preflighted room")
	for index: int in range(raw_draw.size()):
		var command: Dictionary = app.staged_commands[index]
		ck(command["id"] == raw_draw[index]["id"]
			and command["position"] == Vector2i(raw_draw[index]["position"][0], raw_draw[index]["position"][1])
			and _sha(command["image"].get_data()) == raw_draw[index]["pixel_sha256"],
			"exact actual staged room identity/position/pixels match original source piece%d" % index)
	var adapter := Adapter.new(VERSIONS)
	var before_path: String = OS.get_environment("TOKIMEMO_BACKGROUND_BEFORE")
	ck(FileAccess.get_sha256(before_path) == BEFORE_SHA, "before adapter is the unchanged prior canonical source")
	if FileAccess.get_sha256(before_path) == BEFORE_SHA:
		var old_script: Script = load(before_path)
		var old_adapter: RefCounted = old_script.new(VERSIONS)
		var old: Dictionary = old_adapter.call("capture", app, "holiday_command")
		ck(old["error"].contains("holiday command boundary lacks validated room background draw"),
			"unchanged before adapter refuses the exact actual May7 state")
		diagnostics["actual_before_error"] = old["error"]
	var fixed: Dictionary = adapter.capture(app, "holiday_command")
	ck(fixed["error"].is_empty(), "fixed adapter captures the exact actual May7 state: " + fixed["error"])
	if not fixed["error"].is_empty():
		_finish()
		return
	_roundtrip(adapter, fixed["snapshot"], "exact actual May7")
	var holiday_points: Dictionary = {}
	var returned_points: Dictionary = {}
	for capture: Dictionary in route["captures"]:
		var point: Dictionary = capture["snapshot"]
		var core: Dictionary = point["state"]
		var rules: Dictionary = core["rule_state"]
		var date: String = "%d-%d" % [rules["month"], rules["day"]]
		if rules["weekday"] == 6 and [core["mode_stage"], core["mode_step"]] == [1, 0]:
			if core["game_mode"] == 0x46 and not holiday_points.has(date):
				holiday_points[date] = point
			elif core["game_mode"] == 0x34 and not returned_points.has(date):
				returned_points[date] = point
	for date: String in ["4-9", "4-16", "4-23", "4-30", "5-7"]:
		ck(holiday_points.has(date), "actual route contains the Sunday menu: " + date)
		if holiday_points.has(date):
			_matrix(adapter, holiday_points[date], false, date)
	for date: String in ["4-9", "4-16", "4-23", "4-30"]:
		ck(returned_points.has(date), "actual route contains the returned command: " + date)
		if returned_points.has(date):
			_matrix(adapter, returned_points[date], true, date)
	# Finite malicious/corrupt save descriptors for both source boundary roles.
	_install(actual, 16318)
	_negatives(adapter, "holiday_command", false)
	_install(returned_points["4-30"], 16361)
	_negatives(adapter, "next_command", true)
	_finish()

func _matrix(adapter: Adapter, point: Dictionary, returned: bool, date: String) -> void:
	for shown: int in [0, 1]:
		var lba: int = 16361 if returned else 16318
		var retained: int = 16379 if returned else 16131
		_install(point, lba)
		var data: Dictionary = app.state.rule_state
		data["background_cache"][shown] = lba
		data["background_cache"][1 - shown] = retained
		data["background_show"] = shown
		var pending: Dictionary = data["resources"].get("holiday_pending", {})
		if pending.get("lba") == lba:
			pending["slot"] = shown
		var role: String = "next_command" if returned else "holiday_command"
		var saved: Dictionary = adapter.capture(app, role)
		var label: String = "%s %s shown%d" % [date, role, shown]
		ck(saved["error"].is_empty(), "bounded physical ownership capture: %s: %s" % [label, saved["error"]])
		if saved["error"].is_empty():
			_roundtrip(adapter, saved["snapshot"], label)
		cases.append({"date": date, "role": role, "shown": shown,
			"capture_error": saved["error"], "variation": "explicit cache permutation of captured source records"})

func _roundtrip(adapter: Adapter, snapshot: Dictionary, label: String) -> void:
	var before: String = logical()
	var source: RefCounted = app.controller.month_start
	var pixels: Dictionary = fingerprint()["texture_pixels"]
	var error: String = adapter.restore(app, snapshot)
	ck(error.is_empty() and logical() == before, "restore preserves complete state/RNG/clocks/input/queues/hash: " + label + ": " + error)
	ck(app.controller.month_start == source and fingerprint()["texture_pixels"] == pixels,
		"restore preserves trusted source provider and original room pixels: " + label)

func _negatives(adapter: Adapter, role: String, returned: bool) -> void:
	var saved: Dictionary = adapter.capture(app, role)
	ck(saved["error"].is_empty(), "negative fixture is a fully accepted source boundary: " + role)
	if not saved["error"].is_empty():
		return
	var snapshot: Dictionary = saved["snapshot"]
	var shown_lba: int = 16361 if returned else 16318
	for kind: String in ["wrong-shown", "wrong-cache", "duplicate-room", "unknown-inactive", "missing-draw", "wrong-draw", "reordered-draw", "translated-draw", "actor"]:
		var bad: Dictionary = snapshot.duplicate(true)
		var data: Dictionary = bad["gameplay_state"]["state"]["rule_state"]
		var shown: int = data["background_show"]
		match kind:
			"wrong-shown": data["background_show"] = 1 - shown
			"wrong-cache": data["background_cache"][shown] = 0
			"duplicate-room": data["background_cache"][1 - shown] = shown_lba
			"unknown-inactive": data["background_cache"][1 - shown] = 12345
			"missing-draw": bad["gameplay_state"]["draw"].pop_back()
			"wrong-draw": bad["gameplay_state"]["draw"][0]["id"] = app.command_backgrounds[0][0]["id"]
			"reordered-draw": bad["gameplay_state"]["draw"].reverse()
			"translated-draw": bad["gameplay_state"]["draw"][0]["x"] += 1
			"actor": data["actor_state"]["96"] = {"id": 96, "flags": 5, "sequence_slot": 0,
				"sequence": {"counter": 0}}
		var before: Dictionary = fingerprint()
		var error: String = adapter.restore(app, bad)
		var wanted: String = "post-holiday command boundary lacks validated returned-room background draw" if returned else "holiday command boundary lacks validated room background draw"
		ck(error.contains(wanted), "exact source/draw policy rejects %s %s: %s" % [role, kind, error])
		ck(fingerprint() == before, "failed restore is atomic for simulation/source/pixels: " + role + " " + kind)
		diagnostics[role + "/" + kind] = error
	var before_renderer: Dictionary = fingerprint()
	var rebound: String = adapter.restore(app, snapshot, _reject_renderer)
	ck(rebound == "invented renderer rebind failure", "renderer failure propagates for " + role)
	ck(fingerprint() == before_renderer, "failed renderer restores original objects/state/RNG/clocks/source/pixels: " + role)

func _reject_renderer(restored: Boot) -> String:
	# Modify only newly installed state/caches, then refuse this transaction.
	restored.state.rule_state["params"][0] += 1
	restored.canvas.present([])
	return "invented renderer rebind failure"

static func _sha(bytes: PackedByteArray) -> String:
	var context := HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(bytes)
	return context.finish().hex_encode()

func _finish() -> void:
	var report: Dictionary = {"checks": checks, "failures": failures, "cases": cases,
		"diagnostics": diagnostics, "route_sha256": ROUTE_SHA, "before_sha256": BEFORE_SHA,
		"candidate_sha256": FileAccess.get_sha256("res://services/playable_save_adapter.gd"),
		"scope": "genuine original Boot preflight plus exact immutable V17 save-state reconstruction and explicit cache permutations; no new player-route/May7 activity/May8/visual acceptance"}
	var file: FileAccess = FileAccess.open(OS.get_environment("TOKIMEMO_PLAYABLE_OUTPUT").path_join("background-boundary-save.json"), FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify(report, "\t"))
		file.close()
	for failure: String in failures:
		push_error(failure)
	print(JSON.stringify({"checks": checks, "failures": failures, "physical_cases": cases.size()}))
	app.canvas.free()
	app.pad.free()
	app.status_label.free()
	app.detail_label.free()
	app.free()
	quit(0 if failures.is_empty() else 1)
