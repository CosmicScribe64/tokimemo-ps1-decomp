extends RefCounted
## T-29013. Immutable original media and transactional Sunday draw/service staging.
## The app separately gates complete native HUD/prompt/font readiness.

const MediaStore = preload("res://services/school_day_media_store.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Holiday = preload("res://rules/holiday.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const ExactMedia = preload("res://services/native_activity_media.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const EXTENSION_SHA: String = "c960d279b9ff2cfeddc96afd4f2de43fa5365db6129155759085813e1ed2bf64"
const SOURCE_RAM_SHA: String = "7623f081b5907864709a2a8957eff40ac9c8a3524883a397e6be23db6ebb0ee6"
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const NEXT_ROOM_RGBA_SHA: String = ContentIdentity.NEXT_COMMAND_ROOM_RGBA_SHA
const ROUTE: Dictionary = {"room": 16318, "activity": 16379, "sd": 41487, "next_command": 16361}
const PROMPTS: Dictionary = {"holiday": "holiday_open_init/default", "next_command": "settings_command_screen_build/default"}
## T-39014: fresh source profile from tools/port/holiday_scene_content.py. It
## carries only the state holiday_open_init fixes; the other boundary fields
## belong to the live session and are never defaulted here.
const FRESH_SCHEMA: String = "tokimemo.holiday-scene-source.v1"
const FRESH_PROFILE_SHA: String = "f6a695efc026d9ebfe8896fc5d5d73c7d7a0ef6627e163b10693584912eb3cb5"
const FRESH_TRACK_SHA: String = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const FRESH_EXE_SHA: String = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const SCENE_FIELDS: Array[String] = ["background_brightness", "dial_keys", "dial_pos", "icon_count"]
const SESSION_FIELDS: Array[String] = ["boot_hold", "clubs", "icon_mode", "input_device", "screen_brightness", "tile_layer_enabled"]
const SESSION_BOUNDS: Dictionary = {"boot_hold": [-128, 127], "clubs": [0, 65535], "icon_mode": [0, 255],
	"input_device": [0, 255], "screen_brightness": [0, 255], "tile_layer_enabled": [-128, 127]}
const CLUB_RECORDS: int = 12

var error: String = ""
var tables: Dictionary = {}
var resources: Dictionary = {}
var backgrounds: Dictionary = {}
var _extension: Dictionary = {}
var _scene_owned: Dictionary = {}
var _actor: ActorService = ActorService.new()
var _ready: bool = false

func open(media: MediaStore, extension_path: String, next_room: Image) -> String:
	_reset()
	if _safe_file(extension_path) and FileAccess.get_size(extension_path) <= 65536 and FileAccess.get_sha256(extension_path) == FRESH_PROFILE_SHA:
		return _fail("fresh holiday scene profile lacks the live-session boundary fields " + ",".join(SESSION_FIELDS) + "; fresh-session initialization is unavailable")
	if not _next_room_valid(next_room):
		return _fail("next-command room differs from the reviewed C_02_02H pixels")
	if not _safe_file(extension_path) or FileAccess.get_size(extension_path) > 65536 or FileAccess.get_sha256(extension_path) != EXTENSION_SHA:
		return _fail("genuine April4 holiday extension path/hash is unavailable")
	var extension: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(extension_path)))
	if not extension is Dictionary or extension.get("schema") != "tokimemo.native-holiday-boundary-extension.v1" or extension.get("source_sha256") != SOURCE_RAM_SHA or extension.get("date") != [95, 4, 4, 1] or not extension.get("holiday") is Dictionary:
		return _fail("holiday extension is not bound to the genuine April4 source")
	return _open_media(media, next_room, extension["holiday"])

## Opens the fresh source profile. `session` must hold every session-owned
## field from the caller's own live state; nothing is defaulted. Each gate
## returns its own error, and all of them pass before any media is opened.
func open_fresh(media: MediaStore, profile_path: String, next_room: Image, session: Dictionary) -> String:
	_reset()
	var loaded: Dictionary = load_fresh_profile(profile_path)
	if not loaded["error"].is_empty():
		return _fail(loaded["error"])
	var holiday: Dictionary = compose_fresh(loaded["scene"], session)
	if holiday.is_empty():
		return _fail("live-session holiday fields must be exactly " + ",".join(SESSION_FIELDS) + " within their native game_state bounds")
	if not _next_room_valid(next_room):
		return _fail("next-command room differs from the reviewed C_02_02H pixels")
	return _open_media(media, next_room, holiday)

## T-39002 installed route: opens the media with the fresh profile's
## scene-owned values only. The session-owned fields are composed on every
## holiday entry from the live records (compose_session), never at open; until
## then initial_extension() stays empty.
func open_scene(media: MediaStore, profile_path: String, next_room: Image) -> String:
	_reset()
	var loaded: Dictionary = load_fresh_profile(profile_path)
	if not loaded["error"].is_empty():
		return _fail(loaded["error"])
	if not _next_room_valid(next_room):
		return _fail("next-command room differs from the reviewed C_02_02H pixels")
	var failure: String = _open_media(media, next_room, {})
	if not failure.is_empty():
		return failure
	_scene_owned = loaded["scene"]
	return ""

## The six session-owned fields from their live owners when the day screen
## dispatches a Sunday: icon_mode and input_device from the HUD session
## (boot_settings_defaults_set, pad_defaults_set), screen_brightness from the
## route state, tile_layer_enabled and boot_hold from the last
## settings_command_screen_setup_reset record `setup` (home_screens.c:136-137;
## no modeled mode writes them since), clubs as each ClubRecord's flags, the
## field holiday_icons_set_available reads (holiday.c:307).
## Returns {"error", "session"}.
static func live_session(state: State, hud: RefCounted, setup: Variant) -> Dictionary:
	if not setup is Dictionary or not setup.get("tile_layer_enabled") is int or not setup.get("boot_hold") is int:
		return {"error": "no settings_command_screen_setup_reset record owns tile_layer_enabled/boot_hold"}
	var clubs: Array = []
	for club: Variant in state.rule_state.get("clubs", []):
		if not club is Dictionary or not club.get("flags") is int:
			return {"error": "club records lack their flags"}
		clubs.append(int(club["flags"]))
	if hud == null or not hud.intact():
		return {"error": "no intact HUD session owns icon_mode/input_device"}
	var brightness: Variant = state.opening.get("screen_brightness") if "opening" in state else null
	if not brightness is int:
		return {"error": "route state has no screen_brightness"}
	return {"error": "", "session": {"boot_hold": int(setup["boot_hold"]), "clubs": clubs, "icon_mode": hud.icon_mode,
		"input_device": hud.input_device, "screen_brightness": int(brightness),
		"tile_layer_enabled": int(setup["tile_layer_enabled"])}}

## The holiday extension of one holiday entry: the opened scene-owned values
## with exactly the live session fields (compose_fresh); {} on refusal.
func compose_session(session: Dictionary) -> Dictionary:
	if not _ready or _scene_owned.is_empty():
		return {}
	return compose_fresh(_scene_owned, session)

## Authenticates the fresh profile bytes and returns only its scene-owned values.
static func load_fresh_profile(path: String) -> Dictionary:
	if not _safe_file(path) or FileAccess.get_size(path) > 65536 or FileAccess.get_sha256(path) != FRESH_PROFILE_SHA:
		return {"error": "fresh holiday scene profile path/hash is unavailable", "scene": {}}
	var profile: Variant = _integers(JSON.parse_string(FileAccess.get_file_as_string(path)))
	if not profile is Dictionary or profile.get("schema") != FRESH_SCHEMA or profile.get("version") != 1 \
		or not profile.get("provenance") is Dictionary or profile["provenance"].get("original_input_sha256") != FRESH_TRACK_SHA \
		or not profile["provenance"].get("iso_entry") is Dictionary or profile["provenance"]["iso_entry"].get("sha256") != FRESH_EXE_SHA \
		or not profile.get("scene_owned") is Dictionary or not profile.get("session_owned_fields") is Dictionary:
		return {"error": "fresh holiday scene profile is not bound to the original track/executable", "scene": {}}
	var scene: Dictionary = profile["scene_owned"]
	var owned: Array = scene.keys()
	owned.sort()
	var session_keys: Array = profile["session_owned_fields"].keys()
	session_keys.sort()
	if owned != SCENE_FIELDS or session_keys != SESSION_FIELDS:
		return {"error": "fresh holiday scene profile field ownership differs", "scene": {}}
	return {"error": "", "scene": scene.duplicate(true)}

## Merges scene-owned values with exactly the session-owned fields; {} on any
## missing, extra, non-integer or out-of-range session field. Ranges follow
## include/game/game_state.h: icon_mode, input_device and screen_brightness
## are u8; tile_layer_enabled and boot_hold are s8; clubs is ClubRecord[12],
## which the port holds as one u16 per record (rules/school_day.gd wraps it at
## 65535). Route limits such as icon_mode 0 stay in rules/holiday.gd.
static func compose_fresh(scene: Dictionary, session: Dictionary) -> Dictionary:
	if session.size() != SESSION_FIELDS.size():
		return {}
	var holiday: Dictionary = scene.duplicate(true)
	for key: String in SESSION_FIELDS:
		if not session.has(key):
			return {}
		var value: Variant = _integers(session[key])
		var bounds: Array = SESSION_BOUNDS[key]
		if key == "clubs":
			if not value is Array or value.size() != CLUB_RECORDS:
				return {}
			for club: Variant in value:
				if not club is int or club < bounds[0] or club > bounds[1]:
					return {}
		elif not value is int or value < bounds[0] or value > bounds[1]:
			return {}
		holiday[key] = value
	return holiday

func _reset() -> void:
	_ready = false
	tables.clear()
	resources.clear()
	backgrounds.clear()
	_extension.clear()
	_scene_owned.clear()
	error = ""

static func _next_room_valid(next_room: Image) -> bool:
	return next_room != null and not next_room.is_empty() and next_room.get_size() == Vector2i(320, 88) and _image_sha(next_room) == NEXT_ROOM_RGBA_SHA

func _open_media(media: MediaStore, next_room: Image, holiday: Dictionary) -> String:
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
	var prepared_resources: Dictionary = ExactMedia.from_media(media, "holiday_command_0_sd")
	if prepared_resources.is_empty():
		return _fail("original holiday SD profile1 sequences/frames are unavailable")
	tables = prepared_tables.duplicate(true)
	resources = prepared_resources
	backgrounds = prepared_backgrounds
	_extension = holiday.duplicate(true)
	_ready = true
	return ""

func initial_extension() -> Dictionary:
	return _extension.duplicate(true) if _ready else {}

var enable_subscreens: bool = false

func handles(state: State) -> bool:
	var data: Dictionary = state.rule_state
	var index: int = data.get("day_event_index", 0)
	var after_sunday: bool = index > 0 and data["day_events"][index][0] == 0xff and data["day_events"][index - 1][0] in [0x10, 0x80]
	return state.game_mode in [0x46, 0x50] or (after_sunday and state.game_mode in [0x31, 0x34])

func stage(state: State, tick: int, result: Session.FrameResult) -> Dictionary:
	if not _ready:
		return _bad("holiday media adapter is not completely preflighted")
	if not handles(state):
		return _bad("holiday adapter received a state outside its captured route")
	var allow_subscreens: bool = enable_subscreens or state.game_mode == 0x50 or (state.game_mode == 0x46 and state.mode_stage in [0x10, 0x20, 0x30, 0xFE])
	var failure: String = Holiday.validate(state, allow_subscreens)
	if not failure.is_empty():
		return _bad(failure)
	var data: Dictionary = state.rule_state
	if data["month"] not in [3, 4, 5] or data["vacation"] != 0 or ((data["player_flags"] >> 8) & 15) != 0 or data["player_flags"] >> 12 != 11:
		return _bad("holiday media supports healthy spring no-club command0 only")
	var shown: int = data["background_cache"][data["background_show"]]
	if state.game_mode == 0x50 or (state.game_mode == 0x46 and state.mode_stage in [0x10, 0x20, 0x30, 0xFE]):
		return {"error": "", "draw_commands": [], "services": [], "screens": [], "active_lba": shown}
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
	if not value is Dictionary or value.size() != expected.size() or expected["sequence_slot"] not in [0, 1]:
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
