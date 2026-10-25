extends RefCounted
## T-29006. Explicit saves for the captured command and first-result boundaries.
## Call between committed steps, outside GameSession callbacks. Immutable media
## is revalidated/reloaded by the app; only logical render IDs enter the save.

const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/school_day_state.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
const Sequence = preload("res://presentation/native_sequence.gd")
const Save = preload("res://services/save_service.gd")
const SCHEMA: String = "tokimemo.playable-boundary.v1"
const HOST_FIELDS: Array[String] = ["initial_params", "stopped", "failure", "captured_actor",
	"captured_effect", "trace", "focus_paused", "playback_paused", "_window_focused",
	"_application_focused", "_resume_input_pending", "_replay_requested", "smoke", "smoke_command",
	"continue_week", "checkpoints", "staged_hud_refresh", "supported_stop"]
var versions: Dictionary

func _init(profile_versions: Dictionary) -> void:
	versions = profile_versions.duplicate(true)

func capture(app: Node, checkpoint_id: String) -> Dictionary:
	var session: Session = app.get("session")
	if session == null:
		return _fail("preloaded playable session is required")
	var boundary_error: String = _boundary_error(session.state, checkpoint_id)
	if not boundary_error.is_empty():
		return _fail(boundary_error)
	if app.get("focus_paused") or app.get("_resume_input_pending") or app.get("video_capture_busy"):
		return _fail("save requires a focused, idle application boundary")
	if session.rng.get("_failure_receiver").is_valid():
		return _fail("save cannot run inside a session step")
	var media: Dictionary = _trusted_media(app)
	if not media["error"].is_empty():
		return media
	var host: Dictionary = {}
	for key: String in HOST_FIELDS:
		host[key] = app.get(key)
	var draw: Array = []
	for command: Dictionary in app.get("staged_commands"):
		draw.append({"id": command["id"], "x": command["position"].x, "y": command["position"].y})
	var pad: Node = app.get("pad")
	var snapshot: Dictionary = {"boundary": "stable", "source_version": versions.get("source_version"),
		"content_version": versions.get("content_version"), "behavior_version": versions.get("behavior_version"),
		"gameplay_state": {"schema": SCHEMA, "checkpoint_id": checkpoint_id,
			"state": session.state.projection(), "input_latch": session.latched_input.snapshot(),
			"session_hash": session.state_hash(), "controller_identity": _identity(app, media),
			"host": host.duplicate(true), "pad": {"devices": pad.get("_devices").duplicate(true),
				"previous": pad.get("_previous")}, "draw": draw,
			"staged_services": app.get("staged_services").duplicate(true)},
		"native_rng": session.rng.checkpoint(), "native_clocks": session.clock.snapshot(),
		"queued_service_events": app.get("pending_services").duplicate(true)}
	var prepared: Dictionary = _prepare(app, snapshot)
	if not prepared["error"].is_empty():
		return _fail(prepared["error"])
	return {"error": "", "snapshot": snapshot}

## post_apply is an optional downstream renderer rebind returning an error String.
## It must touch only the newly installed session/owned caches and perform no I/O.
func restore(app: Node, snapshot: Dictionary, post_apply: Callable = Callable()) -> String:
	var live_session: Session = app.get("session")
	if live_session == null or live_session.rng.get("_failure_receiver").is_valid():
		return "load requires an idle preloaded session outside a step"
	if app.get("focus_paused") or app.get("playback_paused") or app.get("_resume_input_pending") \
		or not app.get("_window_focused") or not app.get("_application_focused") or app.get("video_capture_busy"):
		return "load requires the current application to be focused and idle"
	var data_error: String = Save.new("").validate_snapshot(snapshot)
	if not data_error.is_empty():
		return data_error
	snapshot = snapshot.duplicate(true)
	var prepared: Dictionary = _prepare(app, snapshot)
	if not prepared["error"].is_empty():
		return prepared["error"]
	var old: Dictionary = {"session": app.get("session"), "state": app.get("state"),
		"controller": app.get("controller"), "pending_services": app.get("pending_services"),
		"staged_services": app.get("staged_services"), "staged_commands": app.get("staged_commands")}
	var old_host: Dictionary = {}
	for key: String in HOST_FIELDS:
		old_host[key] = app.get(key)
	var pad: Node = app.get("pad")
	var old_pad: Dictionary = {"devices": pad.get("_devices"), "previous": pad.get("_previous")}
	var canvas: Node = app.get("canvas")
	var old_canvas: Dictionary = {"commands": canvas.get("_commands"), "textures": canvas.get("_textures").duplicate()}
	var old_labels: Array[String] = [app.get("status_label").text, app.get("detail_label").text]
	var data: Dictionary = snapshot["gameplay_state"]
	app.set("session", prepared["session"])
	app.set("state", prepared["session"].state)
	app.set("controller", prepared["controller"])
	app.set("pending_services", _records(snapshot["queued_service_events"]))
	app.set("staged_services", _records(data["staged_services"]))
	app.set("staged_commands", prepared["draw"])
	for key: String in HOST_FIELDS:
		app.set(key, _records(data["host"][key]) if key in ["trace", "checkpoints"] else data["host"][key])
	pad.set("_devices", data["pad"]["devices"].duplicate(true))
	pad.set("_previous", data["pad"]["previous"])
	var error: String = canvas.call("present", prepared["draw"])
	if error.is_empty():
		app.call("_update_labels")
		if post_apply.is_valid():
			var checked: Variant = post_apply.call(app)
			error = checked if checked is String else "renderer rebind must return an error String"
	if error.is_empty():
		return ""
	# Retain the original objects until all rebind work commits; rollback covers
	# nested rule/actor state, RNG trace, clocks, input, queues and renderer cache.
	for key: String in old:
		app.set(key, old[key])
	for key: String in old_host:
		app.set(key, old_host[key])
	pad.set("_devices", old_pad["devices"])
	pad.set("_previous", old_pad["previous"])
	canvas.set("_commands", old_canvas["commands"])
	canvas.set("_textures", old_canvas["textures"])
	canvas.call("queue_redraw")
	app.get("status_label").text = old_labels[0]
	app.get("detail_label").text = old_labels[1]
	return error

func _prepare(app: Node, snapshot: Dictionary) -> Dictionary:
	var data_error: String = Save.new("").validate_snapshot(snapshot)
	if not data_error.is_empty():
		return _fail(data_error)
	if not _keys(snapshot, ["boundary", "source_version", "content_version", "behavior_version",
		"gameplay_state", "native_rng", "native_clocks", "queued_service_events"]) or snapshot.get("boundary") != "stable":
		return _fail("stable snapshot schema differs")
	for key: String in ["source_version", "content_version", "behavior_version"]:
		if not versions.get(key) is String or snapshot[key] != versions[key]:
			return _fail("save compatibility differs: " + key)
	var data: Variant = snapshot["gameplay_state"]
	if not data is Dictionary or not _keys(data, ["schema", "checkpoint_id", "state", "input_latch", "session_hash", "controller_identity", "host", "pad", "draw", "staged_services"]) or data["schema"] != SCHEMA:
		return _fail("playable save schema differs")
	var media: Dictionary = _trusted_media(app)
	if not media["error"].is_empty():
		return media
	if not data["controller_identity"] is String or data["controller_identity"] != _identity(app, media):
		return _fail("preloaded controller/content identity differs")
	var state: State = State.new()
	if not data["state"] is Dictionary or not _same_keys(data["state"], state.projection()):
		return _fail("complete state projection differs")
	var defaults: Dictionary = state.projection()
	for key: String in defaults:
		if typeof(defaults[key]) != typeof(data["state"][key]):
			return _fail("state field type differs: " + key)
		if defaults[key] is int:
			var maximum: int = 255 if key in ["game_mode", "mode_stage", "mode_step", "substep", "game_mode_scratch", "mode_stage_scratch", "ovl_target_game_mode"] else 4294967295
			if data["state"][key] < 0 or data["state"][key] > maximum:
				return _fail("native state width differs: " + key)
	state.restore(data["state"])
	var error: String = state.structural_error()
	if not error.is_empty():
		return _fail(error)
	error = _boundary_error(state, data["checkpoint_id"])
	if not error.is_empty():
		return _fail(error)
	var session: Session = Session.new()
	var clocks: Variant = snapshot["native_clocks"]
	if not clocks is Dictionary or not _same_keys(clocks, session.clock.snapshot()):
		return _fail("clock snapshot differs")
	for key: String in clocks:
		if key == "paused":
			if not clocks[key] is bool or clocks[key]:
				return _fail("paused clock is outside supported stable boundaries")
		elif not clocks[key] is int or clocks[key] < 0 or clocks[key] >= 9223372036854775807:
			return _fail("clock counter is invalid: " + key)
	if clocks["phase"] >= 1000000000:
		return _fail("rational clock phase is invalid")
	var rng: Variant = snapshot["native_rng"]
	error = _rng_error(rng)
	if not error.is_empty():
		return _fail(error)
	var latch: Variant = data["input_latch"]
	if not latch is Dictionary or not _keys(latch, ["tick", "held", "pressed", "released"]):
		return _fail("input latch differs")
	for key: String in latch:
		if not latch[key] is int or (key != "tick" and (latch[key] < 0 or latch[key] > 4294967295)):
			return _fail("input latch has invalid integer fields")
	if latch["tick"] != clocks["logic"] - 1:
		return _fail("input latch tick differs from clock")
	var previous: int = (latch["held"] & ~latch["pressed"]) | latch["released"]
	var input: InputFrame = InputFrame.new(latch["tick"], latch["held"], previous)
	if input.snapshot() != latch:
		return _fail("input edges are inconsistent")
	var host: Variant = data["host"]
	if not host is Dictionary or not _keys(host, HOST_FIELDS):
		return _fail("host controller snapshot differs")
	for key: String in HOST_FIELDS:
		if typeof(host[key]) != typeof(app.get(key)):
			return _fail("host controller field type differs: " + key)
	if host["focus_paused"] or host["playback_paused"] or host["_resume_input_pending"] or not host["_window_focused"] or not host["_application_focused"] or not host["failure"].is_empty():
		return _fail("host is outside supported stable focus/error policy")
	if host["initial_params"].size() != 9 or not host["initial_params"].all(func(word: Variant) -> bool: return word is int):
		return _fail("initial parameter projection differs")
	if not _record_array(host["trace"]) or not _record_array(host["checkpoints"]) or not _record_array(snapshot["queued_service_events"]) or not _record_array(data["staged_services"]):
		return _fail("service/trace records must be arrays of data records")
	# Both currently supported user boundaries have no undelivered service work.
	if not snapshot["queued_service_events"].is_empty() or not data["staged_services"].is_empty():
		return _fail("stable boundary contains undelivered services")
	var pad: Variant = data["pad"]
	if not pad is Dictionary or not _keys(pad, ["devices", "previous"]) or not pad["devices"] is Dictionary or not pad["previous"] is int or pad["previous"] != latch["held"]:
		return _fail("pad/latch snapshot differs")
	for key: Variant in pad["devices"]:
		if not key is String or not pad["devices"][key] is int or pad["devices"][key] < 0 or pad["devices"][key] > 65535:
			return _fail("device record differs")
	var controller: Controller = Controller.new(app.get("controller").tables)
	# Launch capabilities come only from the live app's preflighted media maps.
	# Saved rule resources and extra save fields cannot grant a new command.
	if not controller.get("command_capabilities") is Array:
		return _fail("restored controller lacks authenticated command capabilities")
	var capabilities: Array[int] = []
	capabilities.assign(media["capabilities"])
	controller.set("command_capabilities", capabilities)
	error = controller.attach(session, state)
	if not error.is_empty():
		return _fail(error)
	session.clock.restore(clocks)
	session.rng.restore(rng)
	session.latched_input = input
	var old_session: Session = app.get("session")
	session.draw_logic = old_session.draw_logic
	session.cd_logic = old_session.cd_logic
	session.audio_logic = old_session.audio_logic
	if not data["session_hash"] is String or session.state_hash() != data["session_hash"]:
		return _fail("restored complete session hash differs")
	var actor_resources: Dictionary = {}
	if not state.rule_state["actor_state"].is_empty():
		var command: int = state.rule_state["day_events"][state.rule_state["day_event_index"]][1]
		if state.game_mode != 0x40 or not media["resources"].has(command):
			return _fail("saved actor has no active validated command resource set")
		actor_resources = media["resources"][command]
	error = _actor_error(state.rule_state["actor_state"], actor_resources, clocks["logic"])
	if not error.is_empty():
		return _fail(error)
	var draw: Dictionary = _draw_commands(media, data["draw"])
	if not draw["error"].is_empty():
		return draw
	return {"error": "", "session": session, "controller": controller, "draw": draw["commands"]}

func _boundary_error(state: State, checkpoint_id: Variant) -> String:
	if state.paused or state.restart_request:
		return "paused/restart simulation is outside supported save boundaries"
	if checkpoint_id == "command":
		if [state.game_mode, state.mode_stage, state.mode_step] == [0x34, 1, 0]:
			return ""
	elif checkpoint_id == "first_result":
		if [state.game_mode, state.mode_stage, state.mode_step] == [0x31, 0, 0] and state.rule_state.get("day_event_index") == 1:
			return ""
	return "checkpoint is outside the command/first-result save boundaries"

func _rng_error(value: Variant) -> String:
	if not value is Dictionary or not _keys(value, ["profile", "state", "draw_count", "trace", "error", "step_error"]):
		return "RNG checkpoint differs"
	if value["profile"] != Rng.OPENBIOS_PROFILE or value["error"] != "" or value["step_error"] != "":
		return "RNG profile/error differs"
	if not value["state"] is int or value["state"] < 0 or value["state"] > 4294967295 or not value["draw_count"] is int or not value["trace"] is Array or value["draw_count"] != value["trace"].size():
		return "RNG state/count differs"
	var replay: Rng = Rng.new()
	for index: int in range(value["trace"].size()):
		var entry: Variant = value["trace"][index]
		if not entry is Dictionary or not _keys(entry, ["caller", "before", "after", "result"]) or not entry["caller"] is String or entry["caller"].is_empty():
			return "RNG trace record differs"
		for key: String in ["before", "after", "result"]:
			if not entry[key] is int:
				return "RNG trace word differs"
		if index == 0:
			replay.configure(Rng.OPENBIOS_PROFILE, entry["before"])
		if replay.state != entry["before"] or replay.next(entry["caller"]) != entry["result"] or replay.state != entry["after"]:
			return "RNG trace cannot be replayed"
	if not value["trace"].is_empty() and replay.state != value["state"]:
		return "RNG trace final state differs"
	return ""

func _actor_error(actors: Dictionary, resources: Dictionary, logic: int) -> String:
	for key: String in actors:
		var actor: Dictionary = actors[key]
		if not _keys(actor, ["id", "sequence_slot", "x", "y", "flags", "draw_flags", "quad_flags", "sequence"]):
			return "actor projection fields differ"
		for field: String in ["id", "sequence_slot", "x", "y", "flags", "draw_flags", "quad_flags"]:
			if not actor[field] is int:
				return "actor integer field differs"
		if str(actor["id"]) != key or actor["flags"] not in [0, 5] or actor["draw_flags"] != 0x84 or actor["quad_flags"] != 0x01000000:
			return "actor native identity/flags differ"
		var steps: Variant = resources.get("sequences", {}).get(str(actor["sequence_slot"]))
		if not steps is Array or not actor["sequence"] is Dictionary or not actor["sequence"].get("period") is int:
			return "actor immutable sequence is unavailable"
		var sequence: Sequence = Sequence.new()
		if not sequence.configure(steps, actor["sequence"]["period"]) or not sequence.restore(actor["sequence"]):
			return "actor native sequence snapshot differs"
		if sequence.last_iteration >= logic:
			return "actor sequence draw is ahead of simulation"
	return ""

func _draw_commands(media: Dictionary, descriptors: Variant) -> Dictionary:
	if not descriptors is Array:
		return _fail("render ID descriptors differ")
	var images: Dictionary = {}
	for command: int in media["backgrounds"]:
		for background: Dictionary in media["backgrounds"][command]:
			var error: String = _register_image(images, background["id"], background["image"])
			if not error.is_empty():
				return _fail(error)
	for command: int in media["resources"]:
		for frame: Dictionary in media["resources"][command]["frames"].values():
			var error: String = _register_image(images, frame["id"], frame["image"])
			if not error.is_empty():
				return _fail(error)
	var commands: Array[Dictionary] = []
	for entry: Variant in descriptors:
		if not entry is Dictionary or not _keys(entry, ["id", "x", "y"]) or not entry["id"] is String or not images.has(entry["id"]) or not entry["x"] is int or not entry["y"] is int:
			return _fail("saved render identity/position is unavailable")
		if entry["x"] < -32768 or entry["x"] > 32767 or entry["y"] < -32768 or entry["y"] > 32767:
			return _fail("saved render identity/position is unavailable")
		commands.append({"id": entry["id"], "image": images[entry["id"]], "position": Vector2i(entry["x"], entry["y"])})
	return {"error": "", "commands": commands}

func _identity(app: Node, media: Dictionary) -> String:
	var assets: Array = []
	var commands: Array = media["resources"].keys()
	commands.sort()
	for command: int in commands:
		var resources: Dictionary = media["resources"][command]
		var backgrounds: Array = []
		for background: Dictionary in media["backgrounds"][command]:
			backgrounds.append([background["id"], _image_hash(background["image"]),
				[background["position"].x, background["position"].y]])
		var frames: Array = []
		for key: String in resources["frames"]:
			var frame: Dictionary = resources["frames"][key]
			frames.append([key, frame["id"], _image_hash(frame["image"]),
				[frame["bounds"].position.x, frame["bounds"].position.y, frame["bounds"].size.x, frame["bounds"].size.y],
				[frame["origin"].x, frame["origin"].y]])
		assets.append([command, resources["sequences"], backgrounds, frames])
	return JSON.stringify([app.get("controller").tables, app.get("boundary"),
		media["capabilities"], assets]).sha256_text()

func _trusted_media(app: Node) -> Dictionary:
	var resources: Variant = app.get("command_resources")
	var backgrounds: Variant = app.get("command_backgrounds")
	var live_caps: Variant = app.get("controller").get("command_capabilities")
	if not resources is Dictionary or not backgrounds is Dictionary or not live_caps is Array \
		or resources.is_empty() or not resources.has(0) or resources.size() != backgrounds.size():
		return _fail("preloaded command media configuration is incomplete")
	var commands: Array = resources.keys()
	for command: Variant in commands:
		if not command is int or command not in [0, 7] or not backgrounds.has(command):
			return _fail("unsupported preloaded command media key")
	for capability: Variant in live_caps:
		if not capability is int:
			return _fail("launch capability is not an integer command ID")
	commands.sort()
	var capabilities: Array = live_caps.duplicate()
	capabilities.sort()
	if commands != capabilities:
		return _fail("launch capabilities differ from validated command media")
	for command: Variant in commands:
		if not resources[command] is Dictionary \
			or not resources[command].get("sequences") is Dictionary or not resources[command].get("frames") is Dictionary \
			or not backgrounds[command] is Array:
			return _fail("unsupported preloaded command media record")
		for key: Variant in resources[command]["frames"]:
			if not key is String:
				return _fail("preloaded frame key must be a string index")
		for frame: Variant in resources[command]["frames"].values():
			if not frame is Dictionary or not frame.get("id") is String or not frame.get("image") is Image \
				or not frame.get("bounds") is Rect2i or not frame.get("origin") is Vector2i:
				return _fail("preloaded command frame is incomplete")
		for background: Variant in backgrounds[command]:
			if not background is Dictionary or not background.get("id") is String or not background.get("image") is Image \
				or not background.get("position") is Vector2i:
				return _fail("preloaded command background is incomplete")
	return {"error": "", "resources": resources, "backgrounds": backgrounds, "capabilities": live_caps.duplicate()}

func _register_image(images: Dictionary, ident: String, image: Image) -> String:
	if ident.is_empty() or image.is_empty():
		return "validated image identity is empty"
	if images.has(ident):
		var previous: Image = images[ident]
		if previous.get_size() != image.get_size() or previous.get_format() != image.get_format() or _image_hash(previous) != _image_hash(image):
			return "validated image identity refers to conflicting payloads"
	images[ident] = image
	return ""

func _image_hash(image: Image) -> String:
	var hash: HashingContext = HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(JSON.stringify([image.get_width(), image.get_height(), image.get_format(),
		image.has_mipmaps()]).to_utf8_buffer())
	hash.update(image.get_data())
	return hash.finish().hex_encode()

func _keys(record: Dictionary, expected: Array) -> bool:
	return record.size() == expected.size() and expected.all(func(key: String) -> bool: return record.has(key))

func _same_keys(a: Dictionary, b: Dictionary) -> bool:
	return _keys(a, b.keys())

func _record_array(value: Variant) -> bool:
	return value is Array and value.all(func(item: Variant) -> bool: return item is Dictionary)

func _records(value: Array) -> Array[Dictionary]:
	var output: Array[Dictionary] = []
	output.assign(value.duplicate(true))
	return output

func _fail(reason: String) -> Dictionary:
	return {"error": reason}
