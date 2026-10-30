extends RefCounted
## T-29026. Exact immutable weekday resources; no command or sequence substitution.
## Source: school_day.c::get_weekly_bg_sector, school_day_sd_set_select,
## school_day_sd_load. Original pointer-table slots retain their own frames.

const MediaStore = preload("res://services/school_day_media_store.gd")
const OrdinaryMedia = preload("res://services/ordinary_command_media_adapter.gd")
const State = preload("res://rules/school_day_state.gd")
const SchoolDay = preload("res://rules/school_day.gd")

var _ordinary: OrdinaryMedia
var _canonical: Dictionary = {}

func open(ordinary: OrdinaryMedia) -> String:
	_ordinary = null
	_canonical.clear()
	if ordinary == null or ordinary.media == null:
		return "exact activity media requires an open original media store"
	var prepared: Dictionary = {}
	for command: int in [0, 7]:
		if not ordinary.has_command(command):
			continue
		var loaded: Dictionary = from_media(ordinary.media, "command_%d_sd" % command)
		if loaded.is_empty():
			return "original command-%d sequence/frame set is unavailable" % command
		prepared[command] = loaded
	_canonical = prepared
	_ordinary = ordinary
	return ""

func get_resources(command: int, club: int = -1) -> Dictionary:
	if _ordinary == null:
		return {}
	return _canonical[command].duplicate(true) if _canonical.has(command) else _ordinary.get_resources(command, club)

func resolve(state: State) -> Dictionary:
	if _ordinary == null or state == null or state.game_mode != 0x40:
		return {"error": "exact activity media requires an active school-day command"}
	var data: Dictionary = state.rule_state
	var event: Array = data["day_events"][data["day_event_index"]]
	var command: int = event[1]
	if event[0] != 0 or command not in range(8):
		return {"error": "active queue record is not an ordinary command"}
	var club: int = (data["player_flags"] >> 12) & 15 if command == 4 else -1
	if not _ordinary.has_command(command, club):
		return {"error": "exact original media is unavailable for command %d / club %d" % [command, club]}
	var route: Dictionary = SchoolDay.resolve_bg_and_sd(command, club if command == 4 else 0,
		data["month"] in [6, 7, 8, 9], 1 if ((data["player_flags"] >> 8) & 15) == 3 else 0,
		data["vacation"], 0)
	if not route.get("error", "").is_empty():
		return route
	var loaded_route: Dictionary = OrdinaryMedia.CLUB_ROUTES[club] if command == 4 else OrdinaryMedia.MEDIA_ROUTES[command]
	for key: String in ["background_lba", "sd_set_id", "sd_lba"]:
		if route[key] != loaded_route[key]:
			return {"error": "original command %d variant is not loaded: %s=%d" % [command, key, route[key]]}
	var resources: Dictionary = _canonical[command] if _canonical.has(command) else _ordinary.get_resources(command, club)
	return {"error": "", "route": route, "resources": resources,
		"background_commands": _ordinary.get_background_commands(command, club)}

static func from_media(media: MediaStore, component: String) -> Dictionary:
	var sequences: Dictionary = {}
	var frames: Dictionary = {}
	# The validated reader caps source sequence indices at 32767. Original packs
	# expose contiguous pointer-table records, with a finite maximum 512 records.
	for slot: int in range(512):
		var steps: Array = OrdinaryMedia._integers(media.get_sequence(component, slot))
		if steps.is_empty():
			break
		sequences[str(slot)] = steps
		for step: Dictionary in steps:
			var key: String = str(step["frame_index"])
			if frames.has(key):
				continue
			var frame: Dictionary = media.get_frame(component, step["frame_index"])
			if frame.is_empty():
				return {}
			var bounds: Array = frame["bounds"]
			var origin: Array = frame["origin"]
			frames[key] = {"id": frame["id"], "image": frame["image"],
				"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
				"origin": Vector2i(int(origin[0]), int(origin[1]))}
	return {"sequences": sequences, "frames": frames} if not sequences.is_empty() else {}
