extends RefCounted
## Runtime HUD capability. Source data is immutable; session state belongs to callers.

const Glyphs = preload("res://presentation/native_glyphs.gd")
const NativeCompositor = preload("res://presentation/hud_compositor.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const SCHEMA := "tokimemo.native-command-hud.v1"
const PROFILE := "spring_type_a_ordinary.v1"
const INPUTS := {
	"executable": ["SLPM_86.053", 667648, "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"],
	"mainrom": ["CDROM/FUKUTEST/MAINROM.NBN", 108544, "f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c"],
	"calendar": ["CDROM/FUKUTEST/CALROM.NBN", 305152, "775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867"],
	"room": ["CDROM/BS/C_02_02H.BS", 16936, "ea4f9201486e8bfc21b03706b4427a2e3e0865f2be7a30f5057802618747ec96"]}
const ROOM_PROOF := {
	"manifest_sha256": "27eff05f51fcd4904a5c54215bcd9d0506d413d131143c3ff51f864ea1823e63",
	"metadata_sha256": "4d37c46cf3b582e40d94206d509255340a7c9af236f2a3d6facc130185a5a510",
	"image_sha256": "d1e8cc097c6602e257923b8c2cb422c6b476e0666fad8e4e568e697df1160202",
	"original_sha256": "ea4f9201486e8bfc21b03706b4427a2e3e0865f2be7a30f5057802618747ec96"}
const READERS := ["tools.assets.image_packs._parse", "tools.assets.image_packs_calendar.export_calendar_ui", "tools.assets.text_binary._spans"]
const SOURCE_EVIDENCE := ["src/main/home_screens.c", "src/main/hud_panels.c", "src/main/hud_screens.c",
	"src/main/actor_sprites.c", "src/main/menu_speaker_girl.c", "src/main/text_system.c",
	"src/main/gpu_primitives.c", "src/main/frame_render_audio.c", "tools/assets/image_packs.py",
	"tools/assets/image_packs_calendar.py", "tools/assets/text_binary.py",
	"asm/data/main/home_screens.rodata.s", "asm/data/main/menu_speaker_girl.rodata.s"]
const RULE_KEYS := ["year", "month", "day", "weekday", "params", "calendar", "menu_cursor", "icon_enabled", "player_flags", "player_flags_upper", "memory_card_busy"]
const CONTEXT_KEYS := ["game_mode", "mode_stage", "mode_step", "icon_mode", "icon_count", "cursor_style", "hover_ticks", "family_name_hex"]

var error := ""
var last_coverage: Image
var _manifest: Dictionary = {}
var _root := ""
var _images: Dictionary = {}
var _specs: Dictionary = {}
var _font = Glyphs.new()
var _compositor = NativeCompositor.new()

func open_manifest(path: String, expected_sha256: String, font_root: String) -> bool:
	_reset()
	if not _sha_string(expected_sha256) or not _safe_path(path) or not _safe_path(font_root) or not FileAccess.file_exists(path) or FileAccess.get_size(path) > 2 * 1024 * 1024 or FileAccess.get_sha256(path) != expected_sha256:
		return _fail("HUD manifest path/hash is not the caller's reviewed capability")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not _validate_manifest(parsed):
		return _fail(error)
	_root = path.get_base_dir()
	var root_files := DirAccess.get_files_at(_root)
	var root_dirs := DirAccess.get_directories_at(_root)
	if root_files.size() != 1 or root_files[0] != path.get_file() or root_dirs.size() != 1 or root_dirs[0] != "assets" or not _safe_path(_root.path_join("assets")):
		return _fail("HUD packet root contains unlisted files or directories")
	if DirAccess.get_files_at(_root.path_join("assets")).size() != _specs.size() or not DirAccess.get_directories_at(_root.path_join("assets")).is_empty():
		return _fail("HUD payload directory differs from manifest")
	for relative: String in _specs:
		var spec: Dictionary = _specs[relative]
		var absolute := _root.path_join(relative)
		if not _safe_path(absolute) or FileAccess.get_size(absolute) != int(spec["size"]) or FileAccess.get_sha256(absolute) != spec["sha256"]:
			return _fail("HUD payload path/size/hash differs: " + relative)
		var image := Image.new()
		if image.load(absolute) != OK or image.get_size() != Vector2i(spec["width"], spec["height"]):
			return _fail("HUD payload decode/shape differs: " + relative)
		_images[relative] = image
	var font_error: String = _font.open_external_font(font_root)
	if not font_error.is_empty():
		return _fail(font_error)
	for text: Dictionary in parsed["content"]["strings"].values():
		if not _glyphs_supported(str(text["raw_hex"]).hex_decode()):
			return _fail("source narration references unavailable native glyphs")
	_manifest = parsed.duplicate(true)
	_compositor.configure(_manifest, _images, _font)
	error = ""
	return true

func create_state(rule: Dictionary, context: Dictionary) -> Dictionary:
	var state := {"rule": {}, "context": context.duplicate(true), "icon_steps": [], "icon_elapsed": []}
	for key: String in RULE_KEYS:
		if not rule.has(key):
			error = "live rule state is missing " + key
			return {}
		state["rule"][key] = rule[key]
	state["rule"] = state["rule"].duplicate(true)
	state["icon_steps"].resize(14)
	state["icon_steps"].fill(0)
	state["icon_elapsed"].resize(14)
	state["icon_elapsed"].fill(0)
	return {} if _view(state).is_empty() else state

func sync_rule_state(state: Dictionary, rule: Dictionary, context: Dictionary = {}) -> String:
	var candidate: Dictionary = state.duplicate(true)
	if not context.is_empty():
		candidate["context"] = context.duplicate(true)
	candidate["rule"] = {}
	for key: String in RULE_KEYS:
		if not rule.has(key):
			return "live rule state is missing " + key
		candidate["rule"][key] = rule[key]
	if _view(candidate).is_empty():
		return error
	state.clear()
	state.merge(candidate.duplicate(true))
	return ""

func set_animation_phase(state: Dictionary, steps: Array, elapsed: Array) -> String:
	var candidate: Dictionary = state.duplicate(true)
	candidate["icon_steps"] = steps.duplicate()
	candidate["icon_elapsed"] = elapsed.duplicate()
	if _view(candidate).is_empty():
		return error
	state["icon_steps"] = steps.duplicate()
	state["icon_elapsed"] = elapsed.duplicate()
	return ""

func build_actors(state: Dictionary) -> Array[Dictionary]:
	var view := _view(state)
	if view.is_empty():
		return []
	var actors: Array[Dictionary] = _compositor.build_actors(view)
	error = _compositor.error
	return actors

func advance_icons(state: Dictionary, native_ticks: int) -> String:
	var view := _view(state)
	if view.is_empty():
		return error
	var message: String = _compositor.advance_icons(view, native_ticks)
	if message.is_empty():
		state["icon_steps"] = view["icon_steps"].duplicate()
		state["icon_elapsed"] = view["icon_elapsed"].duplicate()
	return message

func render(state: Dictionary, previous_frame: Image = null) -> Image:
	last_coverage = null
	var view := _view(state)
	if view.is_empty():
		return null
	if previous_frame != null and previous_frame.get_format() not in [Image.FORMAT_RGB8, Image.FORMAT_RGBA8]:
		error = "previous native framebuffer must use RGB8 or RGBA8 pixels"
		return null
	var name: String = state["context"]["family_name_hex"]
	var name_size: int = name.length() / 2
	var name_x: int = -114 if name_size == 2 else (-121 if name_size == 4 else -128)
	var strings: Dictionary = _manifest["content"]["strings"]
	var runs: Array[Dictionary] = [{"hex": name, "position": [name_x, 50], "attribute": 0},
		{"hex": str(strings["narration_prefix"]["raw_hex"]) + str(strings["ordinary_command"]["raw_hex"]), "position": [-86, 50], "attribute": 0}]
	_compositor.set_text_runs(runs)
	var image: Image = _compositor.render(view, previous_frame)
	error = _compositor.error
	last_coverage = _compositor.last_coverage if image != null else null
	return image

func canvas_command(state: Dictionary, previous_frame: Image = null) -> Dictionary:
	var image := render(state, previous_frame)
	if image == null:
		return {}
	return {"id": "native-command-hud:v1:" + _sha(image.get_data()), "image": image,
		"position": Vector2i.ZERO, "blend_mode": 0}

func activity_command(state: Dictionary, scene_commands: Array[Dictionary], parameter_colors: Array) -> Dictionary:
	var view := _view(state)
	if view.is_empty():
		return {}
	if state["context"]["game_mode"] not in [0x40, 0x31, 0x32]:
		error = "activity HUD requires the bounded school-day/dispatcher/week-end mode"
		return {}
	if parameter_colors.size() != 9 or not parameter_colors.all(func(value: Variant) -> bool: return value is int and value in [0, 0x43, 0x44]):
		error = "activity parameter colors exceed source palette selectors"
		return {}
	view["parameter_colors"] = parameter_colors.duplicate()
	_compositor.set_text_runs([]) # school_day_screen_init resets native text.
	var image: Image = _compositor.render(view, null, scene_commands, false)
	error = _compositor.error
	return {} if image == null else {"id": "native-activity-hud:v1:" + _sha(image.get_data()),
		"image": image, "position": Vector2i.ZERO, "blend_mode": 0}

func calendar_source_commands(state: Dictionary) -> Array[Dictionary]:
	var view := _view(state)
	if view.is_empty():
		return []
	var month_index: int = (int(view["year"]) - 95) * 12 + int(view["month"]) - 4
	if month_index < 0 or month_index >= 35:
		error = "source calendar does not contain a current/next month pair"
		return []
	var commands: Array[Dictionary] = []
	for record: Dictionary in _manifest["content"]["calendar"]:
		var side: int = int(record["month_index"]) - month_index
		if side < 0 or side > 1:
			continue
		var image: Image = _images[record["png"]["path"]]
		var position := Vector2i(24 if side == 0 else 160, 32)
		if record["variant"] == "calendar-grid":
			var extent := 126 if side == 0 else 127
			image = image.get_region(Rect2i(0, 0, extent, extent))
		else:
			position.x = 72 if side == 0 else 208
			image = image.duplicate()
		commands.append({"image": image, "position": position, "priority": 3,
			"month_index": int(record["month_index"]), "variant": record["variant"]})
	return commands

func _view(state: Dictionary) -> Dictionary:
	error = ""
	if _manifest.is_empty() or not _keys(state, ["rule", "context", "icon_steps", "icon_elapsed"]) or not _keys(state.get("rule"), RULE_KEYS) or not _keys(state.get("context"), CONTEXT_KEYS):
		error = "HUD requires an open capability and live rule/context state"
		return {}
	var rule: Dictionary = state["rule"]
	var context: Dictionary = state["context"]
	for key: String in CONTEXT_KEYS:
		if key != "family_name_hex" and not _integer(context[key]):
			error = "HUD context is not an integer: " + key
			return {}
	var style: int = context["cursor_style"]
	var command_screen: bool = (context["game_mode"] == 0x34 and context["mode_stage"] == 1 and context["mode_step"] == 0) or context["game_mode"] == 0x30
	var activity: bool = (context["game_mode"] == 0x40 and context["mode_stage"] in [0, 1, 2] and context["mode_step"] in [0, 1, 2]) or (context["game_mode"] == 0x31 and context["mode_stage"] == 0 and context["mode_step"] == 0) or (context["game_mode"] == 0x32 and context["mode_stage"] in [0, 1, 0xff] and context["mode_step"] == 0)
	if not (command_screen or activity) or context["icon_mode"] != 0 or context["icon_count"] != 14 or context["hover_ticks"] != 32 or not (style == 0 or (style >= 0x9b and style < 0xae)):
		error = "HUD supports bounded TypeA command/school-day modes and settled hover only"
		return {}
	var family: Variant = context["family_name_hex"]
	if not family is String or family.length() not in [4, 8, 12] or not _hex(family) or not _glyphs_supported(family.hex_decode()):
		error = "live family name requires one to three supported native code units"
		return {}
	for key: String in ["year", "month", "day", "weekday", "menu_cursor", "player_flags", "player_flags_upper", "memory_card_busy"]:
		if not _integer(rule[key]):
			error = "live rule field is not an integer: " + key
			return {}
	var year: int = rule["year"]
	var month: int = rule["month"]
	var day: int = rule["day"]
	var month_index: int = (year - 95) * 12 + month - 4
	if month not in [3, 4, 5] or month_index < 0 or month_index > 35 or int(rule["weekday"]) < 0 or int(rule["weekday"]) > 6 or day < 1 or day > 31 or int(rule["menu_cursor"]) < 0 or int(rule["menu_cursor"]) >= 14 or rule["memory_card_busy"] != 0:
		error = "HUD state is outside the spring TypeA/date/idle-card capability"
		return {}
	if not _numbers(rule["params"], 9, 0, 0x3e7ffff) or not _numbers(rule["icon_enabled"], 15, 0, 1) or not rule["calendar"] is Array or rule["calendar"].size() != 2:
		error = "live parameter/icon/calendar records differ from native dimensions"
		return {}
	var calendar_bytes: Array = []
	for calendar: Variant in rule["calendar"]:
		if not _keys(calendar, ["day_kind", "marker"]) or not _numbers(calendar["day_kind"], 32, 0, 255) or not _numbers(calendar["marker"], 32, 0, 255):
			error = "live calendar must contain two native byte arrays"
			return {}
		calendar_bytes.append_array(calendar["day_kind"])
		calendar_bytes.append_array(calendar["marker"])
	if int(calendar_bytes[0]) < 28 or int(calendar_bytes[0]) > 31 or day > int(calendar_bytes[0]):
		error = "live date exceeds its native calendar day count"
		return {}
	var flags: int = rule["player_flags"]
	var upper: int = rule["player_flags_upper"]
	if flags < 0 or flags > 65535 or upper < 0 or upper > 65535:
		error = "live player flags exceed native halfwords"
		return {}
	var tomorrow: int = calendar_bytes[32 + day + 1]
	var next_week: int = calendar_bytes[32 + day + 6]
	var club: int = flags >> 12
	if command_screen and (tomorrow in [5, 11] or (tomorrow == 8 and club != 11) or (next_week == 12 and club < 6) or (upper & 1) != 0 or ((flags >> 8) & 15) == 3):
		error = "forced, sick, or event-specific command narration is outside ordinary HUD capability"
		return {}
	if not _numbers(state["icon_steps"], 14, 0, 31) or not _numbers(state["icon_elapsed"], 14, 0, 65535):
		error = "live icon animation state is malformed"
		return {}
	for icon: int in range(14):
		var sequence: Array = _manifest["tables"]["sequences"][icon]["steps"]
		var step: int = state["icon_steps"][icon]
		if step >= sequence.size() or int(state["icon_elapsed"][icon]) >= int(sequence[step]["duration"]):
			error = "live icon phase is outside the source sequence"
			return {}
	var parameters: Array[int] = []
	for word: Variant in rule["params"]:
		parameters.append(int(word) >> 16)
	return {"year": year, "month": month, "day": day, "weekday": int(rule["weekday"]),
		"day_kind": int(calendar_bytes[day]), "parameters": parameters, "selected": int(rule["menu_cursor"]),
		"icon_enabled": rule["icon_enabled"].slice(0, 14), "cursor_frame": style,
		"icon_steps": state["icon_steps"].duplicate(), "icon_elapsed": state["icon_elapsed"].duplicate()}

func _validate_manifest(value: Variant) -> bool:
	if not _keys(value, ["schema", "version", "profile", "provenance", "tables", "frames", "content"]) or value["schema"] != SCHEMA or not _integer(value["version"]) or int(value["version"]) != 1:
		return _bad("HUD manifest schema/version/fields differ")
	var profile: Variant = value["profile"]
	if not _keys(profile, ["id", "months", "icon_mode", "icon_count", "room_draw_mode", "hover_mode", "memory_card_busy"]) or profile["id"] != PROFILE or not _numbers_equal(profile["months"], [3, 4, 5]) or profile["hover_mode"] != "settled":
		return _bad("HUD profile fields differ")
	for pair: Array in [["icon_mode", 0], ["icon_count", 14], ["room_draw_mode", 0], ["memory_card_busy", 0]]:
		if not _integer(profile[pair[0]]) or int(profile[pair[0]]) != pair[1]:
			return _bad("HUD capability value differs")
	var provenance: Variant = value["provenance"]
	if not _keys(provenance, ["inputs", "readers", "source_evidence", "room_packet"]) or not _keys(provenance["inputs"], INPUTS.keys()) or provenance["readers"] != READERS or provenance["room_packet"] != ROOM_PROOF:
		return _bad("HUD provenance fields differ")
	for name: String in INPUTS:
		var item: Variant = provenance["inputs"][name]
		if not _keys(item, ["path", "size", "sha256"]) or item["path"] != INPUTS[name][0] or not _integer(item["size"]) or int(item["size"]) != INPUTS[name][1] or item["sha256"] != INPUTS[name][2]:
			return _bad("HUD original input identity differs")
	if not provenance["source_evidence"] is Array or provenance["source_evidence"].size() != 13:
		return _bad("HUD source evidence table differs")
	var evidence_paths: Dictionary = {}
	for item: Variant in provenance["source_evidence"]:
		if not _keys(item, ["path", "sha256"]) or not item["path"] is String or item["path"] not in SOURCE_EVIDENCE or not _relative_source(item["path"]) or not _sha_string(item["sha256"]) or evidence_paths.has(item["path"]):
			return _bad("HUD source evidence record differs")
		evidence_paths[item["path"]] = true
	var tables: Variant = value["tables"]
	if not _keys(tables, ["icon_positions", "sequences", "draw_origin", "glyph_advance", "back_clear", "clear_color"]) or not _numbers_equal(tables["draw_origin"], [160, 120]) or not _integer(tables["glyph_advance"]) or int(tables["glyph_advance"]) != 14 or not tables["back_clear"] is bool or tables["back_clear"] or not _numbers_equal(tables["clear_color"], [0, 0, 0]) or not tables["icon_positions"] is Array or tables["icon_positions"].size() != 14:
		return _bad("HUD layout tables differ")
	for point: Variant in tables["icon_positions"]:
		if not _numbers(point, 2, -160, 160):
			return _bad("HUD icon position differs")
	if not tables["sequences"] is Array or tables["sequences"].size() != 16:
		return _bad("HUD sequence count differs")
	for index: int in range(16):
		var sequence: Variant = tables["sequences"][index]
		if not _keys(sequence, ["sequence_index", "exe_address", "steps"]) or not _integer(sequence["sequence_index"]) or int(sequence["sequence_index"]) != index or not _integer(sequence["exe_address"]) or not sequence["steps"] is Array or sequence["steps"].is_empty() or sequence["steps"].size() > 32:
			return _bad("HUD sequence record differs")
		for step: Variant in sequence["steps"]:
			if not _keys(step, ["frame_index", "duration"]) or not _range(step["frame_index"], 0, 173) or not _range(step["duration"], 1, 65535):
				return _bad("HUD sequence step differs")
	if not value["frames"] is Dictionary or value["frames"].size() != 219:
		return _bad("HUD frame count differs")
	for key: Variant in value["frames"]:
		var frame: Variant = value["frames"][key]
		if not _keys(frame, ["frame_index", "variant", "bounds", "origin", "parts"]) or not _range(frame["frame_index"], 0, 173) or not frame["variant"] is String or key != "%d:%s" % [int(frame["frame_index"]), frame["variant"]] or not _numbers(frame["bounds"], 4, -32768, 32767) or not _numbers(frame["origin"], 2, -32768, 32767) or not _range(frame["bounds"][2], 1, 640) or not _range(frame["bounds"][3], 1, 512):
			return _bad("HUD frame geometry differs")
		var variant: String = frame["variant"]
		var index: int = frame["frame_index"]
		if variant != "base" and not (index == 2 and variant in ["icon-51", "icon-52", "icon-53"]) and not (index >= 0x44 and index < 0x59 and variant in ["hud-43", "hud-44"]):
			return _bad("HUD frame palette selection differs")
		if not frame["parts"] is Array or frame["parts"].is_empty() or frame["parts"].size() > 64:
			return _bad("HUD frame part count differs")
		for part_index: int in range(frame["parts"].size()):
			var part: Variant = frame["parts"][part_index]
			if not _keys(part, ["part_index", "x", "y", "width", "height", "exe_record_address", "palette_exe_address", "source_palette_words", "preview_palette_words", "png", "stp_png"]) or not _integer(part["part_index"]) or int(part["part_index"]) != part_index or not _range(part["x"], -32768, 32767) or not _range(part["y"], -32768, 32767) or not _range(part["width"], 1, 128) or not _range(part["height"], 1, 128) or not _range(part["exe_record_address"], 0x800c7420, 0x800c95df) or not _range(part["palette_exe_address"], 0x800c6020, 0x800c801f) or not _numbers(part["source_palette_words"], 16, 0, 65535) or not _numbers(part["preview_palette_words"], 16, 0, 65535):
				return _bad("HUD primitive/source palette record differs")
			var address: int = part["palette_exe_address"]
			for color: int in range(16):
				var word: int = part["source_palette_words"][color]
				var forced: bool = (word & 0x7fff) != 0 and ((address >= 0x800c6200 and address < 0x800c6220) or (address >= 0x800c62c0 and address < 0x800c6340))
				if int(part["preview_palette_words"][color]) != (word | 0x8000 if forced else word):
					return _bad("HUD boot STP palette transformation differs")
			if not _payload(part["png"], "color", int(part["width"]), int(part["height"])) or not _payload(part["stp_png"], "stp", int(part["width"]), int(part["height"])):
				return false
	var content: Variant = value["content"]
	if not _keys(content, ["room", "calendar", "strings"]) or not _payload(content["room"], "color", 320, 88) or content["room"]["sha256"] != ROOM_PROOF["image_sha256"]:
		return _bad("HUD room content differs")
	if not content["calendar"] is Array or content["calendar"].size() != 72:
		return _bad("HUD calendar content count differs")
	var calendars: Dictionary = {}
	for record: Variant in content["calendar"]:
		if not _keys(record, ["month_index", "year", "month", "variant", "source_region", "palette_exe_address", "source_palette_words", "png", "stp_png"]) or not _range(record["month_index"], 0, 35) or record["variant"] not in ["calendar-grid", "month-label"] or not _numbers(record["source_palette_words"], 16, 0, 65535):
			return _bad("HUD calendar source record differs")
		var month_index: int = record["month_index"]
		var absolute: int = 1995 * 12 + 3 + month_index
		var ident := "%d:%s" % [month_index, record["variant"]]
		var grid: bool = record["variant"] == "calendar-grid"
		if calendars.has(ident) or not _integer(record["year"]) or not _integer(record["month"]) or int(record["year"]) != int(absolute / 12) or int(record["month"]) != absolute % 12 + 1 or not _numbers_equal(record["source_region"], [0, 0, 128, 128] if grid else [48, 0, 24, 16]) or not _integer(record["palette_exe_address"]) or int(record["palette_exe_address"]) != (0x800c61a0 if grid else 0x800c6180):
			return _bad("HUD calendar month/crop/palette differs")
		calendars[ident] = true
		if not _payload(record["png"], "color", 128 if grid else 24, 128 if grid else 16) or not _payload(record["stp_png"], "stp", 128 if grid else 24, 128 if grid else 16):
			return false
	if not _keys(content["strings"], ["ordinary_command", "narration_prefix"]):
		return _bad("HUD source string set differs")
	for key: String in content["strings"]:
		var text: Variant = content["strings"][key]
		var ordinary: bool = key == "ordinary_command"
		if not _keys(text, ["raw_hex", "address", "label", "assembly", "assembly_sha256", "length", "sha256"]) or not text["raw_hex"] is String or not _hex(text["raw_hex"]) or not _integer(text["length"]) or int(text["length"]) != (26 if ordinary else 2) or str(text["raw_hex"]).length() != int(text["length"]) * 2 or not _sha_string(text["sha256"]) or not _sha_string(text["assembly_sha256"]) or not _integer(text["address"]) or int(text["address"]) != (0x800aff24 if ordinary else 0x800afb74) or text["label"] != ("D_800AFF24" if ordinary else "D_800AFB74") or text["assembly"] != ("asm/data/main/home_screens.rodata.s" if ordinary else "asm/data/main/menu_speaker_girl.rodata.s") or _sha(str(text["raw_hex"]).hex_decode()) != text["sha256"]:
			return _bad("HUD source string proof differs")
	return true

func _payload(value: Variant, kind: String, width: int, height: int) -> bool:
	if not _keys(value, ["path", "size", "sha256", "width", "height", "kind"]) or not value["path"] is String or not _asset_path(value["path"]) or not _range(value["size"], 33, 2 * 1024 * 1024) or not _sha_string(value["sha256"]) or not _integer(value["width"]) or not _integer(value["height"]) or int(value["width"]) != width or int(value["height"]) != height or value["kind"] != kind:
		return _bad("HUD payload descriptor differs")
	if _specs.has(value["path"]) and _specs[value["path"]] != value:
		return _bad("HUD payload path has conflicting descriptors")
	_specs[value["path"]] = value.duplicate(true)
	return true

func _glyphs_supported(codes: PackedByteArray) -> bool:
	if codes.is_empty() or codes.size() % 2 != 0:
		return false
	for offset: int in range(0, codes.size(), 2):
		if codes[offset] == 0 or codes[offset + 1] == 0 or _font.get_glyph(codes.slice(offset, offset + 2)).get("status") != "glyph":
			return false
	return true

func _reset() -> void:
	error = ""
	_manifest.clear()
	_root = ""
	_images.clear()
	_specs.clear()
	last_coverage = null
	_font = Glyphs.new()
	_compositor = NativeCompositor.new()

func _fail(message: String) -> bool:
	_reset()
	error = message
	return false

func _bad(message: String) -> bool:
	error = message
	return false

static func _keys(value: Variant, expected: Array) -> bool:
	if not value is Dictionary or value.size() != expected.size():
		return false
	for key: Variant in expected:
		if not value.has(key):
			return false
	return true

static func _integer(value: Variant) -> bool:
	return value is int or (value is float and is_finite(value) and floor(value) == value)

static func _range(value: Variant, low: int, high: int) -> bool:
	return _integer(value) and value >= low and value <= high

static func _numbers(value: Variant, count: int, low: int, high: int) -> bool:
	if not value is Array or value.size() != count:
		return false
	for number: Variant in value:
		if not _range(number, low, high):
			return false
	return true

static func _sha_string(value: Variant) -> bool:
	return value is String and value.length() == 64 and _hex(value)

static func _numbers_equal(value: Variant, expected: Array) -> bool:
	if not value is Array or value.size() != expected.size():
		return false
	for index: int in range(expected.size()):
		if not _integer(value[index]) or int(value[index]) != int(expected[index]):
			return false
	return true

static func _hex(value: String) -> bool:
	if value.is_empty() or value.length() % 2 != 0:
		return false
	for character: String in value:
		if not "0123456789abcdef".contains(character):
			return false
	return true

static func _asset_path(value: String) -> bool:
	if not value.begins_with("assets/") or not value.ends_with(".png") or value.get_file() != value.trim_prefix("assets/") or value.contains(".."):
		return false
	for character: String in value.trim_prefix("assets/"):
		if not "abcdefghijklmnopqrstuvwxyz0123456789_.-".contains(character):
			return false
	return true

static func _relative_source(value: String) -> bool:
	return not value.is_absolute_path() and not value.contains("..") and not value.contains("\\") and (value.begins_with("src/") or value.begins_with("tools/") or value.begins_with("asm/"))

static func _safe_path(path: String) -> bool:
	return ExternalPath.is_external_path(path)

static func _sha(data: PackedByteArray) -> String:
	var hashing := HashingContext.new()
	hashing.start(HashingContext.HASH_SHA256)
	hashing.update(data)
	return hashing.finish().hex_encode()
