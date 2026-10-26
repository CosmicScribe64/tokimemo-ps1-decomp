extends RefCounted
## T-29013. Immutable original media and transactional Sunday draw/service staging.
## The app separately gates complete native HUD/prompt/font readiness.

const MediaStore = preload("res://services/school_day_media_store.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Holiday = preload("res://rules/holiday.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const EXTENSION_SHA: String = "c960d279b9ff2cfeddc96afd4f2de43fa5365db6129155759085813e1ed2bf64"
const SOURCE_RAM_SHA: String = "7623f081b5907864709a2a8957eff40ac9c8a3524883a397e6be23db6ebb0ee6"
const NEXT_ROOM_RGBA_SHA: String = "8ea14e666785f6d7e9db056c6e96d4a6a11011e544e9fc59a1786584b84315f2"
const ROUTE: Dictionary = {"room": 16318, "activity": 16379, "sd": 41487, "next_command": 16361}
const PROMPTS: Dictionary = {"holiday": "holiday_open_init/default", "next_command": "settings_command_screen_build/default"}

var error: String = ""
var tables: Dictionary = {}
var resources: Dictionary = {}
var backgrounds: Dictionary = {}
var _extension: Dictionary = {}
var _actor: ActorService = ActorService.new()
var _ready: bool = false

func open(media: MediaStore, extension_path: String, next_room: Image) -> String:
	_ready = false
	tables.clear()
	resources.clear()
	backgrounds.clear()
	_extension.clear()
	error = ""
	if next_room == null or next_room.is_empty() or next_room.get_size() != Vector2i(320, 88) or _image_sha(next_room) != NEXT_ROOM_RGBA_SHA:
		return _fail("next-command room differs from the reviewed C_02_02H pixels")
	if not _safe_file(extension_path) or FileAccess.get_size(extension_path) > 65536 or FileAccess.get_sha256(extension_path) != EXTENSION_SHA:
		return _fail("genuine April4 holiday extension path/hash is unavailable")
	var extension: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(extension_path)))
	if not extension is Dictionary or extension.get("schema") != "tokimemo.native-holiday-boundary-extension.v1" or extension.get("source_sha256") != SOURCE_RAM_SHA or extension.get("date") != [95, 4, 4, 1] or not extension.get("holiday") is Dictionary:
		return _fail("holiday extension is not bound to the genuine April4 source")
	var prepared_tables: Dictionary = _integers(media.get_route_tables())
	if prepared_tables.size() != 6:
		return _fail("complete validated holiday media v3 route tables are required")
	if prepared_tables.get("seasonal_room_background_lbas", []).get(0) != ROUTE["room"] or prepared_tables.get("weekday_room_background_lbas", []).get(0) != ROUTE["next_command"] or prepared_tables.get("holiday_activity_background_lbas", []).get(0) != ROUTE["activity"] or prepared_tables.get("holiday_sd_non_summer", []).get(0) != 1 or prepared_tables.get("holiday_sd_lbas", []).get(1) != ROUTE["sd"]:
		return _fail("holiday content route differs from the captured April9 profile")
	var prepared_backgrounds: Dictionary = {}
	for pair: Array in [["command_0_background", 16131], ["command_7_background", 16283], ["holiday_room_background", ROUTE["room"]], ["holiday_command_0_background", ROUTE["activity"]]]:
		var bg: Dictionary = media.get_background(pair[0])
		if bg.is_empty() or bg.get("source", {}).get("lba") != pair[1]:
			return _fail("required original holiday/retained background is unavailable: " + pair[0])
		var commands: Array[Dictionary] = Canvas.background_commands(bg["image_data"], bg["id"])
		if commands.is_empty():
			return _fail("original holiday background geometry is unsupported")
		prepared_backgrounds[pair[1]] = commands
	prepared_backgrounds[ROUTE["next_command"]] = Canvas.background_commands(next_room,
		"native-next-command-room:C_02_02H:" + NEXT_ROOM_RGBA_SHA)
	var steps: Array = _integers(media.get_sequence("holiday_command_0_sd", 0))
	if steps.is_empty():
		return _fail("holiday SD profile1 sequence0 is unavailable")
	var frames: Dictionary = {}
	for step: Dictionary in steps:
		var frame: Dictionary = media.get_frame("holiday_command_0_sd", step["frame_index"])
		if frame.is_empty():
			return _fail("required holiday source frame is unavailable")
		var bounds: Array = frame["bounds"]
		var origin: Array = frame["origin"]
		frames[str(step["frame_index"])] = {"id": frame["id"], "image": frame["image"],
			"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
			"origin": Vector2i(int(origin[0]), int(origin[1]))}
	tables = prepared_tables.duplicate(true)
	resources = {"sequences": {"0": steps}, "frames": frames}
	backgrounds = prepared_backgrounds
	_extension = extension["holiday"].duplicate(true)
	_ready = true
	return ""

func initial_extension() -> Dictionary:
	return _extension.duplicate(true) if _ready else {}

func handles(state: State) -> bool:
	var data: Dictionary = state.rule_state
	return state.game_mode == 0x46 or ([data.get("year"), data.get("month"), data.get("day")] == [95, 4, 9] and data.get("day_event_index") in [9, 10] and state.game_mode in [0x31, 0x34])

func stage(state: State, tick: int, result: Session.FrameResult) -> Dictionary:
	if not _ready:
		return _bad("holiday media adapter is not completely preflighted")
	if not handles(state):
		return _bad("holiday adapter received a state outside its captured route")
	var failure: String = Holiday.validate(state)
	if not failure.is_empty():
		return _bad(failure)
	var data: Dictionary = state.rule_state
	var commands: Array[Dictionary] = []
	var services: Array[Dictionary] = []
	var screens: Array[Dictionary] = []
	for intent: Dictionary in result.presentation:
		match intent.get("kind"):
			"holiday_load":
				failure = _load_error(state, intent)
				if not failure.is_empty():
					return _bad(failure)
				var payload: Dictionary = {"request": intent["request"], "lba": intent["lba"], "ready": true}
				if intent["request"] != "sd":
					payload["decode_result"] = 1 # Native decode success, never the chosen cache slot.
				services.append({"service": "holiday.resources", "payload": payload})
			"holiday_background_show":
				var slot: Variant = intent.get("slot")
				if intent.size() != 5 or intent.get("request") not in ["room", "activity", "next_command"] or not intent.get("lba") is int or intent.get("lba") != ROUTE[intent["request"]] or not slot is int or slot not in [0, 1] or not intent.get("cached") is bool or data["background_show"] != slot or data["background_cache"][slot] != intent["lba"] or not _show_phase(state, intent["request"]):
					return _bad("holiday background show differs from source request/cache identity")
			"holiday_screen_init":
				if intent.size() != 4 or not PROMPTS.has(intent.get("screen")) or intent.get("prompt") != PROMPTS[intent["screen"]] or not intent.get("icon_count") is int or intent.get("icon_count") != 14 or not _screen_phase(state, intent["screen"]):
					return _bad("holiday native screen/prompt identity is unsupported")
				screens.append(intent.duplicate(true))
			"holiday_actors":
				if intent.size() != 3 or not intent.get("sd_set_id") is int or intent.get("sd_set_id") != 1 or not intent.get("actors") is Array or intent["actors"].size() != 1 or not _matches_actor(intent["actors"][0], state) or [state.game_mode, state.mode_stage, state.mode_step] != [0x46, 5, 2]:
					return _bad("holiday actor differs from captured profile1/sequence0 descriptor")
				var initialized: Dictionary = _actor.initialize(intent["actors"], resources)
				if not initialized["error"].is_empty():
					return _bad(initialized["error"])
				data["actor_state"] = initialized["actor_state"]
	var shown: int = data["background_cache"][data["background_show"]]
	if shown != 0:
		if not backgrounds.has(shown):
			return _bad("active holiday cache slot has no validated original image")
		commands.append_array(backgrounds[shown])
	if not data["actor_state"].is_empty():
		if state.game_mode != 0x46 or state.mode_stage != 5 or state.mode_step != 2:
			return _bad("holiday actor draw arrived outside its source activity phase")
		var drawn: Dictionary = _actor.native_draw(data["actor_state"], resources, tick, Vector2i(160, 120))
		if not drawn["error"].is_empty():
			return _bad(drawn["error"])
		data["actor_state"] = drawn["actor_state"]
		commands.append_array(drawn["draw_commands"])
		services.append_array(drawn["services"])
		for draw: Dictionary in drawn["draw_commands"]:
			result.presentation.append({"kind": "native_actor_draw", "id": draw["id"], "x": draw["position"].x, "y": draw["position"].y})
	return {"error": "", "draw_commands": commands, "services": services, "screens": screens, "active_lba": shown}

func _load_error(state: State, intent: Dictionary) -> String:
	var request: Variant = intent.get("request")
	if not request is String or request not in ROUTE or intent.size() != (5 if request == "sd" else 4) or not intent.get("lba") is int or not intent.get("sectors") is int or intent.get("lba") != ROUTE[request] or intent.get("sectors") != (41 if request == "sd" else 10) or (request == "sd" and (not intent.get("sd_set_id") is int or intent.get("sd_set_id") != 1)):
		return "holiday resource request differs from captured original identity/extent"
	var pending: Dictionary = state.rule_state["resources"].get("holiday_pending", {})
	if pending.get("request") != request or pending.get("lba") != intent["lba"] or pending.get("ready") != false:
		return "holiday resource request lacks its matching native pending record"
	var valid: bool = (state.game_mode == 0x46 and ((request == "room" and state.mode_stage == 0 and state.mode_step == 1) or (state.mode_stage == 5 and state.mode_step == 0 and ((request == "activity" and state.substep == 1) or (request == "sd" and state.substep == 3))))) or (request == "next_command" and state.game_mode == 0x34 and state.mode_stage == 0 and state.mode_step == 3)
	return "" if valid else "holiday resource request arrived outside its source wait phase"

static func _matches_actor(value: Variant, state: State) -> bool:
	var expected: Dictionary = Holiday.actor(state)
	if not value is Dictionary or value.size() != expected.size() or expected["sequence_slot"] != 0:
		return false
	for key: String in expected:
		if not value.get(key) is int or value[key] != expected[key]:
			return false
	return true

static func _show_phase(state: State, request: String) -> bool:
	if request == "room":
		return [state.game_mode, state.mode_stage, state.mode_step] == [0x46, 1, 0]
	if request == "activity":
		return [state.game_mode, state.mode_stage, state.mode_step, state.substep] == [0x46, 5, 0, 2]
	return [state.game_mode, state.mode_stage, state.mode_step] == [0x34, 1, 0]

static func _screen_phase(state: State, screen: String) -> bool:
	if screen == "holiday":
		return state.game_mode == 0x46 and ((state.mode_stage == 0 and state.mode_step == 1) or (state.mode_stage == 1 and state.mode_step == 0))
	return state.game_mode == 0x34 and ((state.mode_stage == 0 and state.mode_step == 3) or (state.mode_stage == 1 and state.mode_step == 0))

static func _safe_file(path: String) -> bool:
	return ExternalPath.is_external_file(path)

static func _image_sha(image: Image) -> String:
	var normalized: Image = image.duplicate()
	normalized.convert(Image.FORMAT_RGBA8)
	var context: HashingContext = HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(normalized.get_data())
	return context.finish().hex_encode()

static func _integers(value: Variant) -> Variant:
	if value is Dictionary:
		var output: Dictionary = {}
		for key: String in value:
			output[key] = _integers(value[key])
		return output
	if value is Array:
		var output: Array = []
		for item: Variant in value:
			output.append(_integers(item))
		return output
	return int(value) if value is float and is_finite(value) and value == floor(value) else value

func _fail(message: String) -> String:
	error = message
	return message

func _bad(message: String) -> Dictionary:
	return {"error": message}
