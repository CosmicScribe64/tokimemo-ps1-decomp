extends RefCounted
## T-29014: Unified media adapter for ordinary commands 0–7 (including Command 4 Club).
## Connects canonical MediaStore (commands 0, 7), SpringSchoolDayContentStore (1–3),
## Weekday56ContentStore (5, 6), and ClubCommandContentStore (4, clubs 0..10).

const MediaStore = preload("res://services/school_day_media_store.gd")
const SpringStore = preload("res://services/spring_school_day_content_store.gd")
const Weekday56Store = preload("res://services/weekday_56_content_store.gd")
const ClubStore = preload("res://services/club_command_content_store.gd")
const Canvas = preload("res://presentation/native_canvas.gd")

const MEDIA_ROUTES: Dictionary = {
	0: {"background_lba": 16131, "sd_set_id": 3, "sd_lba": 42541},
	1: {"background_lba": 16141, "sd_set_id": 7, "sd_lba": 42705},
	2: {"background_lba": 16151, "sd_set_id": 11, "sd_lba": 42869},
	3: {"background_lba": 16160, "sd_set_id": 13, "sd_lba": 42951},
	5: {"background_lba": 16268, "sd_set_id": 24, "sd_lba": 43402},
	6: {"background_lba": 16277, "sd_set_id": 20, "sd_lba": 43238},
	7: {"background_lba": 16283, "sd_set_id": 16, "sd_lba": 43074},
}

const CLUB_ROUTES: Dictionary = {
	0: {"background_lba": 16215, "sd_set_id": 3, "sd_lba": 27957},
	1: {"background_lba": 16224, "sd_set_id": 31, "sd_lba": 29105},
	2: {"background_lba": 16234, "sd_set_id": 7, "sd_lba": 28121},
	3: {"background_lba": 16244, "sd_set_id": 11, "sd_lba": 28285},
	4: {"background_lba": 16252, "sd_set_id": 29, "sd_lba": 29023},
	5: {"background_lba": 16260, "sd_set_id": 25, "sd_lba": 28859},
	6: {"background_lba": 16168, "sd_set_id": 13, "sd_lba": 28367},
	7: {"background_lba": 16177, "sd_set_id": 15, "sd_lba": 28449},
	8: {"background_lba": 16206, "sd_set_id": 21, "sd_lba": 28695},
	9: {"background_lba": 16197, "sd_set_id": 19, "sd_lba": 28613},
	10: {"background_lba": 16187, "sd_set_id": 17, "sd_lba": 28531},
}

var media: MediaStore
var spring_store: SpringStore
var weekday56_store: Weekday56Store
var club_store: ClubStore

var command_resources: Dictionary = {}
var command_backgrounds: Dictionary = {}
var club_resources: Dictionary = {}
var club_backgrounds: Dictionary = {}
var error: String = ""

func open_packs(media_path: String, spring_path: String = "", weekday56_path: String = "", club_path: String = "") -> String:
	_clear()
	if media_path.is_empty():
		return _fail("mandatory canonical media pack path is required")
	media = MediaStore.new()
	if not media.open_pack(media_path):
		return _fail("mandatory media preflight: " + media.error_message())

	var err: String = _load_canonical_media()
	if not err.is_empty():
		return _fail(err)

	if not spring_path.is_empty():
		spring_store = SpringStore.new()
		if not spring_store.open_pack(spring_path):
			return _fail("spring content preflight: " + spring_store.error_message())
		err = _load_spring_commands(spring_path)
		if not err.is_empty():
			return _fail(err)

	if not weekday56_path.is_empty():
		weekday56_store = Weekday56Store.new()
		if not weekday56_store.open_pack(weekday56_path):
			return _fail("weekday56 content preflight: " + weekday56_store.error_message())
		err = _load_weekday56_commands()
		if not err.is_empty():
			return _fail(err)

	if not club_path.is_empty():
		club_store = ClubStore.new()
		if not club_store.open_pack(club_path):
			return _fail("club content preflight: " + club_store.error_message())
		err = _load_club_commands()
		if not err.is_empty():
			return _fail(err)

	return ""

func get_available_commands() -> Array[int]:
	var result: Array[int] = []
	for cmd: int in command_resources:
		result.append(cmd)
	if not club_resources.is_empty():
		result.append(4)
	result.sort()
	return result

func has_command(command: int, club_id: int = -1) -> bool:
	if command == 4:
		return club_resources.has(club_id) and club_backgrounds.has(club_id)
	return command_resources.has(command) and command_backgrounds.has(command)

func get_resources(command: int, club_id: int = -1) -> Dictionary:
	if command == 4:
		return club_resources.get(club_id, {}).duplicate(true)
	return command_resources.get(command, {}).duplicate(true)

func get_background_commands(command: int, club_id: int = -1) -> Array[Dictionary]:
	var commands: Array[Dictionary] = []
	var source_list: Array = club_backgrounds.get(club_id, []) if command == 4 else command_backgrounds.get(command, [])
	for cmd_dict: Dictionary in source_list:
		commands.append(cmd_dict.duplicate(true))
	return commands

func validate_variant(command: int, club_id: int = 11, month: int = 4, extra: int = 15, vacation: int = 0, health: int = 0) -> String:
	if command not in [0, 1, 2, 3, 4, 5, 6, 7]:
		return "unsupported ordinary command: %d" % command
	if command == 4:
		if club_id < 0 or club_id > 10:
			return "unsupported club command with no active club (club_id %d)" % club_id
		if not has_command(4, club_id):
			return "required route content capability is unavailable for command 4 club %d" % club_id
	else:
		if not has_command(command):
			return "required route content capability is unavailable for command %d" % command
	if month not in [3, 4, 5]:
		return "seasonal variant outside spring months 3-5 is not supported"
	if vacation != 0:
		return "vacation content is unavailable"
	if health != 0:
		return "sick/rest variants are not supported in this tranche"
	if command in [1, 2, 3, 4, 5, 6] and extra != 15:
		return "companion variants for command %d are not supported in this tranche" % command
	return ""

func _load_canonical_media() -> String:
	for command: int in [0, 7]:
		var bg: Dictionary = media.get_background("command_%d_background" % command)
		if command == 7 and bg.is_empty():
			continue
		var commands: Array[Dictionary] = Canvas.background_commands(bg["image_data"], bg["id"])
		if commands.is_empty():
			return "required command-%d active background geometry unavailable" % command
		var key: String = "command_%d_sd" % command
		var steps: Array = _integers(media.get_sequence(key, 0))
		if steps.is_empty():
			return "required command-%d sequence is missing" % command
		var frames: Dictionary = {}
		for step: Dictionary in steps:
			var frame: Dictionary = media.get_frame(key, step["frame_index"])
			if frame.is_empty():
				return "required command-%d frame is missing" % command
			var bounds: Array = frame["bounds"]
			var origin: Array = frame["origin"]
			frames[str(step["frame_index"])] = {
				"id": frame["id"],
				"image": frame["image"],
				"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
				"origin": Vector2i(int(origin[0]), int(origin[1]))
			}
		command_resources[command] = {"sequences": {"0": steps}, "frames": frames}
		command_backgrounds[command] = commands
	return ""

func _load_spring_commands(spring_path: String) -> String:
	for command: int in [1, 2, 3]:
		var comp: Dictionary = spring_store.get_command(command)
		if comp.is_empty():
			return "spring content store missing command %d" % command
		var bg_files: Dictionary = comp.get("background", {}).get("files", {})
		var bg_path: String = ""
		for fname: String in bg_files:
			if fname.ends_with(".background.png"):
				bg_path = spring_path.path_join("assets").path_join(bg_files[fname]["path"])
				break
		if bg_path.is_empty() or not FileAccess.file_exists(bg_path):
			return "command %d background image missing in spring pack" % command
		var bg_img := Image.new()
		if bg_img.load(bg_path) != OK or bg_img.get_size() != Vector2i(320, 88):
			return "command %d background image invalid or wrong size" % command
		var bg_commands: Array[Dictionary] = Canvas.background_commands(bg_img, "command_%d_background" % command)
		if bg_commands.is_empty():
			return "command %d background geometry unavailable" % command

		var sd: Dictionary = comp.get("sd", {})
		var seq_dict: Dictionary = {}
		for seq: Dictionary in sd.get("sequences", []):
			seq_dict[str(int(seq["sequence_index"]))] = _integers(seq["steps"])
		if not seq_dict.has("0"):
			return "command %d missing sequence 0" % command

		var frames: Dictionary = {}
		for frame_meta: Dictionary in sd.get("frames", []):
			var f_idx: int = int(frame_meta["frame_index"])
			var png_info: Dictionary = frame_meta.get("png", {})
			var frame_path: String = spring_path.path_join("assets").path_join(png_info.get("path", ""))
			if not FileAccess.file_exists(frame_path):
				return "command %d frame %d image missing" % [command, f_idx]
			var frame_img := Image.new()
			if frame_img.load(frame_path) != OK or frame_img.is_empty():
				return "command %d frame %d image load failed" % [command, f_idx]
			var bounds: Array = frame_meta["bounds"]
			var origin: Array = frame_meta["origin"]
			frames[str(f_idx)] = {
				"id": "command_%d_frame_%d" % [command, f_idx],
				"image": frame_img,
				"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
				"origin": Vector2i(int(origin[0]), int(origin[1]))
			}
		command_resources[command] = {"sequences": seq_dict, "frames": frames}
		command_backgrounds[command] = bg_commands
	return ""

func _load_weekday56_commands() -> String:
	for command: int in [5, 6]:
		var bg_img: Image = weekday56_store.get_background(command)
		if bg_img == null or bg_img.get_size() != Vector2i(320, 88):
			return "weekday56 command %d background image invalid" % command
		var bg_commands: Array[Dictionary] = Canvas.background_commands(bg_img, "command_%d_background" % command)
		if bg_commands.is_empty():
			return "weekday56 command %d background geometry unavailable" % command

		var comp: Dictionary = weekday56_store.get_command(command)
		if comp.is_empty():
			return "weekday56 missing command %d component" % command
		var sd: Dictionary = comp.get("sd", {})
		var seq_dict: Dictionary = {}
		for seq: Dictionary in sd.get("sequences", []):
			seq_dict[str(int(seq["sequence_index"]))] = _integers(seq["steps"])
		if not seq_dict.has("0"):
			return "weekday56 command %d missing sequence 0" % command

		var frames: Dictionary = {}
		for frame_meta: Dictionary in sd.get("frames", []):
			var f_idx: int = int(frame_meta["frame_index"])
			var frame_data: Dictionary = weekday56_store.get_sd_frame(command, f_idx)
			if frame_data.is_empty() or frame_data.get("image") == null:
				return "weekday56 command %d frame %d load failed" % [command, f_idx]
			var bounds: Array = frame_data["bounds"]
			var origin: Array = frame_data["origin"]
			frames[str(f_idx)] = {
				"id": "command_%d_frame_%d" % [command, f_idx],
				"image": frame_data["image"],
				"bounds": Rect2i(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3])),
				"origin": Vector2i(int(origin[0]), int(origin[1]))
			}
		command_resources[command] = {"sequences": seq_dict, "frames": frames}
		command_backgrounds[command] = bg_commands
	return ""

func _load_club_commands() -> String:
	for club_id: int in range(11):
		var bg_img: Image = club_store.get_background(club_id)
		if bg_img == null or bg_img.get_size() != Vector2i(320, 88):
			return "club %d background image invalid or missing" % club_id
		var bg_commands: Array[Dictionary] = Canvas.background_commands(bg_img, "command_4_club_%d_bg" % club_id)
		if bg_commands.is_empty():
			return "club %d background geometry unavailable" % club_id

		var comp: Dictionary = club_store.get_club(club_id)
		if comp.is_empty():
			return "club content store missing club %d" % club_id
		var sd: Dictionary = comp.get("sd", {})
		var seq_dict: Dictionary = {}
		for seq: Dictionary in sd.get("sequences", []):
			seq_dict[str(int(seq["sequence_index"]))] = _integers(seq["steps"])
		if not seq_dict.has("0"):
			return "club %d missing sequence 0" % club_id

		var frames: Dictionary = {}
		for frame_meta: Dictionary in sd.get("frames", []):
			var f_idx: int = int(frame_meta["frame_index"])
			var frame_data: Dictionary = club_store.get_sd_frame(club_id, f_idx)
			if frame_data.is_empty() or frame_data.get("image") == null:
				return "club %d frame %d load failed" % [club_id, f_idx]
			frames[str(f_idx)] = frame_data
		club_resources[club_id] = {"sequences": seq_dict, "frames": frames}
		club_backgrounds[club_id] = bg_commands
	return ""

func _clear() -> void:
	media = null
	spring_store = null
	weekday56_store = null
	club_store = null
	command_resources.clear()
	command_backgrounds.clear()
	club_resources.clear()
	club_backgrounds.clear()
	error = ""

func _fail(message: String) -> String:
	_clear()
	error = message
	return message

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
	if value is float and is_finite(value) and value == floor(value) and abs(value) <= 4294967295.0:
		return int(value)
	return value
