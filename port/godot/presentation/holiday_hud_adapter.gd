extends "res://presentation/command_hud_presenter.gd"
## T-29010. Additive source HUD for the bounded April9 holiday/return route.
## Content opens before capability admission. Composition never mutates callers.

const Media = preload("res://services/school_day_media_store.gd")
const LiveState = preload("res://rules/school_day_state.gd")
const HolidayRules = preload("res://rules/holiday.gd")
const Canvas = preload("res://presentation/native_canvas.gd")
const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"
const MEDIA_MANIFEST_SHA := "d9e38432d4eac758a774ba4248eed0629148a3050b8a714d1b99c08c1db82005"
const TEXT_SHA := "5c801d82f8bd7761d4163ab4acdf057bb85de053bba1c7f946b457e574eb5f1f"
const NEXT_ROOM_RGBA_SHA := "8ea14e666785f6d7e9db056c6e96d4a6a11011e544e9fc59a1786584b84315f2"
const REQUESTS := {"room": 16318, "activity": 16379, "next_command": 16361}
const PROMPTS := {"holiday": "holiday_open_init/default", "next_command": "settings_command_screen_build/default"}
const TEXT_SOURCES := ["asm/data/main/holiday.rodata.s", "asm/matchings/main/holiday/holiday_open_init.s",
	"src/main/holiday.c", "src/main/text_system.c", "tools/assets/text_binary.py"]
var _opened: bool = false
var _media: Media = Media.new()
var _backgrounds: Dictionary = {}
var _background_commands: Dictionary = {}
var _actor_specs: Dictionary = {}
var _holiday_text: Dictionary = {}

func open(hud_manifest: String, hud_sha: String, font_root: String,
	holiday_media_root: String, media_sha: String, text_manifest: String, text_sha: String) -> bool:
	_clear_holiday()
	if hud_sha != HUD_SHA or media_sha != MEDIA_MANIFEST_SHA or text_sha != TEXT_SHA:
		return _holiday_fail("holiday HUD requires the three reviewed content identities")
	if not _safe_path(holiday_media_root) or FileAccess.get_sha256(holiday_media_root.path_join("manifest.json")) != media_sha:
		return _holiday_fail("holiday media path/hash differs from the reviewed source packet")
	if not _safe_path(text_manifest) or FileAccess.get_size(text_manifest) > 65536 or FileAccess.get_sha256(text_manifest) != text_sha:
		return _holiday_fail("holiday prompt path/hash differs from the reviewed source packet")
	if not super.open_manifest(hud_manifest, hud_sha, font_root):
		return _holiday_fail(error)
	if not _media.open_pack(holiday_media_root):
		return _holiday_fail(_media.error_message())
	if not _load_text(text_manifest):
		return _holiday_fail(error)
	var tables: Dictionary = _media.get_route_tables()
	if tables.size() != 6 or tables.get("seasonal_room_background_lbas", []).get(0) != 16318 or tables.get("weekday_room_background_lbas", []).get(0) != 16361 or tables.get("holiday_activity_background_lbas", []).get(0) != 16379 or tables.get("holiday_sd_non_summer", []).get(0) != 1 or tables.get("holiday_sd_lbas", []).get(1) != 41487:
		return _holiday_fail("holiday room/activity/return tables differ from the bounded original route")
	for pair: Array in [["command_0_background", 16131], ["command_7_background", 16283],
		["holiday_room_background", 16318], ["holiday_command_0_background", 16379]]:
		var record: Dictionary = _media.get_background(pair[0])
		if record.is_empty() or record.get("source", {}).get("lba") != pair[1] or not _add_background(pair[1], record["id"], record["image_data"]):
			return _holiday_fail("required original background is unavailable: " + pair[0])
	var room: Image = _images[_manifest["content"]["room"]["path"]].duplicate()
	room.convert(Image.FORMAT_RGBA8)
	if _sha(room.get_data()) != NEXT_ROOM_RGBA_SHA or not _add_background(16361,
		"native-next-command-room:C_02_02H:" + NEXT_ROOM_RGBA_SHA, room):
		return _holiday_fail("returned command room pixels differ from original C_02_02H")
	for slot: int in [0, 1]:
		var sequence: Array = _media.get_sequence("holiday_command_0_sd", slot)
		if sequence.is_empty():
			return _holiday_fail("holiday source sequence is missing: %d" % slot)
		for step: Dictionary in sequence:
			var frame: Dictionary = _media.get_frame("holiday_command_0_sd", int(step["frame_index"]))
			if frame.is_empty():
				return _holiday_fail("holiday actor96 source frame is missing")
			var bounds: Array = frame["bounds"]
			var origin: Array = frame["origin"]
			var position := Vector2i(80, 56) + Vector2i(bounds[0], bounds[1]) - Vector2i(origin[0], origin[1])
			_actor_specs[frame["id"]] = _command_spec(frame["id"], frame["image"], position)
	_opened = true
	error = ""
	return true

func get_background(request: String, lba: int) -> Dictionary:
	error = ""
	if not _opened or not REQUESTS.has(request) or REQUESTS[request] != lba or not _backgrounds.has(lba):
		error = "holiday background request differs from the authenticated source identity"
		return {}
	var record: Dictionary = _backgrounds[lba]
	return {"id": record["id"], "lba": lba, "image_data": record["image"].duplicate()}

func validate_profile(profile: Dictionary) -> String:
	error = ""
	if not _opened:
		return "holiday HUD capability is not open"
	return "" if _valid_profile(profile, {"icon_mode": 0, "icon_count": 14}) else error

func compose(rule: Dictionary, core: Dictionary, profile: Dictionary,
	history: Array[Dictionary], scene_commands: Array[Dictionary], active_lba: int) -> Dictionary:
	error = ""
	last_coverage = null
	if not _opened:
		return _compose_fail("holiday HUD capability is not open")
	var checked: LiveState = LiveState.new()
	var defaults: Dictionary = checked.projection()
	if not _keys(core, defaults.keys()):
		return _compose_fail("holiday HUD needs the complete live core/rule projection")
	for key: String in defaults:
		if typeof(core[key]) != typeof(defaults[key]):
			return _compose_fail("holiday core field type differs: " + key)
	if core["rule_state"] != rule:
		return _compose_fail("holiday core and rule projections disagree")
	checked.restore(core)
	var problem: String = HolidayRules.validate(checked)
	if not problem.is_empty():
		return _compose_fail(problem)
	if not _valid_profile(profile, rule["holiday"]):
		return _compose_fail(error)
	var mode: int = core["game_mode"]
	var stage: int = core["mode_stage"]
	var step: int = core["mode_step"]
	if not ((mode == 0x46 and ((stage == 0 and step in [0, 1]) or (stage == 1 and step == 0) or (stage == 5 and step in [0, 1, 2]))) or (mode == 0x31 and stage == 0 and step == 0) or (mode == 0x34 and stage in [0, 1] and step in [0, 1, 2, 3])):
		return _compose_fail("holiday HUD mode/stage/step is outside the bounded source route")
	var index: int = rule["day_event_index"]
	var sunday_index: int = index if mode == 0x46 else index - 1
	if sunday_index < 0 or sunday_index >= rule["day_events"].size():
		return _compose_fail("holiday HUD has no current/prior Sunday queue record")
	var sunday: Array = rule["day_events"][sunday_index]
	if sunday[0] != 0x10 or sunday[1] != 0 or sunday[2] not in [0, 1] or (mode != 0x46 and rule["day_events"][index][0] != 0xff):
		return _compose_fail("holiday HUD queue/command differs from the actual Sunday route")
	var holiday: Dictionary = rule["holiday"]
	if holiday["tile_layer_enabled"] != 0 or holiday["boot_hold"] != 0 or holiday["screen_brightness"] != 128 or holiday["background_brightness"] not in [0, 128]:
		return _compose_fail("holiday tile/boot/brightness state requires an unproved presentation branch")
	var replay: Dictionary = _replay(history, core, rule)
	if not replay["error"].is_empty():
		return _compose_fail(replay["error"])
	if replay["retain"]:
		return {"error": "", "commands": [], "retain": true}
	if active_lba != rule["background_cache"][rule["background_show"]]:
		return _compose_fail("holiday active LBA differs from the live source cache slot")
	var prepared: Dictionary = _scene_commands(scene_commands, active_lba, rule)
	if not prepared["error"].is_empty():
		return _compose_fail(prepared["error"])
	if rule["icon_enabled"].slice(0, 14) != replay["flags"]:
		return _compose_fail("holiday live icon flags differ from the source screen/lock history")
	var view: Dictionary = {"year": rule["year"], "month": rule["month"], "day": rule["day"],
		"weekday": rule["weekday"], "day_kind": rule["calendar"][0]["day_kind"][rule["day"]],
		"parameters": [], "selected": rule["menu_cursor"], "icon_enabled": replay["flags"],
		"cursor_frame": profile["cursor_style"], "icon_steps": replay["steps"],
		"icon_elapsed": replay["elapsed"], "parameter_colors": replay["colors"]}
	var season: Dictionary = HudSeason.view(rule)
	if not season["error"].is_empty():
		return _compose_fail(season["error"])
	view["checker_season"] = season["checker_season"]
	view["checker_fade"] = season["checker_fade"]
	for word: int in rule["params"]:
		var value: int = word >> 16
		if value < 0 or value > 999:
			return _compose_fail("holiday display parameter is outside native 0..999")
		view["parameters"].append(value)
	var runs: Array[Dictionary] = []
	if replay["text"] != "":
		var name: String = profile["family_name_hex"]
		var name_size: int = name.length() / 2
		var name_x: int = -114 if name_size == 2 else (-121 if name_size == 4 else -128)
		var narration: String = _holiday_text["raw_hex"] if replay["text"] == "holiday" else _manifest["content"]["strings"]["ordinary_command"]["raw_hex"]
		runs = [{"hex": name, "position": [name_x, 50], "attribute": 0},
			{"hex": _manifest["content"]["strings"]["narration_prefix"]["raw_hex"] + narration,
			"position": [-86, 50], "attribute": 0}]
	_compositor.set_text_runs(runs)
	var backgrounds: Array[Dictionary] = prepared["backgrounds"]
	if holiday["background_brightness"] == 0:
		for command: Dictionary in backgrounds:
			command["image"].fill(Color.BLACK)
	var frame_cnt: int = int(core.get("frame_count", history.size()))
	var health_mode: int = (int(rule.get("player_flags", 0)) >> 8) & 15
	_compositor.set_checker_uv(
		NativeCompositor.compute_checker_uv(frame_cnt, health_mode)
	)
	var image: Image = _compositor.render(view, null, backgrounds, false)
	if image == null:
		return _compose_fail(_compositor.error)
	# Original actor96 and cursor0 share layer1: OT traversal draws96 first.
	# HUD panels are earlier in the OT; the source actor may overlap them.
	for command: Dictionary in prepared["actors"]:
		_compositor._blit(image, command["image"], null, command["position"], false)
	for actor: Dictionary in _compositor.build_actors(view):
		if actor["id"] == 0:
			_compositor._draw_actor(image, actor)
	if not _compositor.error.is_empty():
		return _compose_fail(_compositor.error)
	last_coverage = _compositor.last_coverage
	return {"error": "", "retain": false, "commands": [{"id": "native-holiday-hud:v1:" + _sha(image.get_data()),
		"image": image, "position": Vector2i.ZERO, "blend_mode": 0}]}

func _load_text(path: String) -> bool:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not _keys(parsed, ["schema", "version", "provenance", "strings"]) or parsed["schema"] != "tokimemo.holiday-hud-text.v1" or not _integer(parsed["version"]) or int(parsed["version"]) != 1:
		return _bad("holiday text schema differs")
	var provenance: Variant = parsed["provenance"]
	if not _keys(provenance, ["executable", "reader", "witnesses"]) or provenance["reader"] != "tools.assets.text_binary._spans" or not _keys(provenance["executable"], ["name", "size", "sha256"]):
		return _bad("holiday text reader provenance differs")
	var executable: Dictionary = provenance["executable"]
	if executable["name"] != "SLPM_86.053" or executable["sha256"] != Media.EXE_SHA or not _integer(executable["size"]) or int(executable["size"]) != 667648 or not provenance["witnesses"] is Array or provenance["witnesses"].size() != TEXT_SOURCES.size():
		return _bad("holiday text executable/source witnesses differ")
	var seen: Dictionary = {}
	for witness: Variant in provenance["witnesses"]:
		if not _keys(witness, ["path", "sha256"]) or witness["path"] not in TEXT_SOURCES or seen.has(witness["path"]) or not _sha_string(witness["sha256"]):
			return _bad("holiday text source witness is invalid")
		seen[witness["path"]] = witness["sha256"]
	if not _keys(parsed["strings"], ["holiday_default"]):
		return _bad("holiday prompt set differs")
	var text: Variant = parsed["strings"]["holiday_default"]
	if not _keys(text, ["label", "address", "assembly", "assembly_sha256", "length", "sha256", "raw_hex"]) or text["label"] != "D_800B1678" or not _integer(text["address"]) or int(text["address"]) != 0x800b1678 or text["assembly"] != TEXT_SOURCES[0] or text["assembly_sha256"] != seen[TEXT_SOURCES[0]] or not _integer(text["length"]) or int(text["length"]) != 24 or not text["raw_hex"] is String or text["raw_hex"].length() != 48 or not _hex(text["raw_hex"]) or _sha(text["raw_hex"].hex_decode()) != text["sha256"] or not _glyphs_supported(text["raw_hex"].hex_decode()):
		return _bad("holiday default prompt label/payload/glyph proof differs")
	_holiday_text = text.duplicate(true)
	return true

func _valid_profile(profile: Dictionary, holiday: Dictionary) -> bool:
	if not _keys(profile, ["family_name_hex", "icon_mode", "icon_count", "cursor_style"]):
		return _bad("holiday HUD needs the four explicit live profile fields")
	for key: String in ["icon_mode", "icon_count", "cursor_style"]:
		if not profile[key] is int:
			return _bad("holiday live profile needs native integer fields")
	var style: int = profile["cursor_style"]
	if profile["icon_mode"] != 0 or profile["icon_count"] != 14 or holiday["icon_mode"] != profile["icon_mode"] or holiday["icon_count"] != profile["icon_count"] or not (style == 0 or style >= 0x9b and style < 0xae):
		return _bad("holiday HUD profile exceeds source TypeA/count14/cursor capability")
	var name: Variant = profile["family_name_hex"]
	if not name is String or name.length() not in [4, 8, 12] or not _hex(name) or not _glyphs_supported(name.hex_decode()):
		return _bad("holiday family name needs one to three supported native code units")
	return true

func _replay(history: Array[Dictionary], core: Dictionary, rule: Dictionary) -> Dictionary:
	var screen: String = ""
	var text: String = ""
	var flags: Array = []
	var steps: Array = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	var elapsed: Array = steps.duplicate()
	var colors: Array = [0, 0, 0, 0, 0, 0, 0, 0, 0]
	var previous_params: Array = []
	var previous_frame: int = -1
	if history.is_empty():
		return {"error": "holiday HUD requires committed presentation history"}
	for row: Dictionary in history:
		for key: String in ["mode", "stage", "step", "substep", "frame_count"]:
			if not row.get(key) is int:
				return {"error": "holiday history lacks native integer " + key}
		if not _numbers(row.get("params"), 9, 0, 0x3e7ffff) or not row.get("presentation") is Array or (previous_frame >= 0 and row["frame_count"] != previous_frame + 1):
			return {"error": "holiday history parameter/event/frame order differs"}
		previous_frame = row["frame_count"]
		if row["mode"] in [0x30, 0x40, 0x32]:
			# The following ordinary week owns its committed canvas. A later
			# Sunday must retain that frame until its own screen init occurs.
			screen = ""
			text = ""
		for intent: Variant in row["presentation"]:
			if not intent is Dictionary:
				return {"error": "holiday history contains a non-record presentation intent"}
			if intent.get("kind") == "holiday_screen_init":
				if not _keys(intent, ["kind", "screen", "prompt", "icon_count"]) or not PROMPTS.has(intent["screen"]) or intent["prompt"] != PROMPTS[intent["screen"]] or intent["icon_count"] != 14:
					return {"error": "holiday history has an unsupported screen/prompt identity"}
				screen = intent["screen"]
				text = screen
				flags = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
				for icon: int in ([9, 13] if screen == "holiday" else [4, 8, 9, 10]):
					flags[icon] = 0
				if screen == "holiday" and not HolidayRules.club_icon_available(rule):
					flags[4] = 0
				steps.fill(0)
				elapsed.fill(0)
				colors.fill(0)
				previous_params = row["params"].duplicate()
		if screen.is_empty():
			continue
		if row["mode"] == 0x46 and row["stage"] == 5 and (row["step"] > 0 or row["substep"] > 0):
			text = ""
			flags.fill(0)
			flags[0] = 1
		for stat: int in range(9):
			var before: int = int(previous_params[stat]) >> 16
			var after: int = int(row["params"][stat]) >> 16
			if before != after:
				colors[stat] = 0x43 if after > before else 0x44
		previous_params = row["params"].duplicate()
		for icon: int in range(14):
			if flags[icon] == 0:
				continue
			var sequence: Array = _manifest["tables"]["sequences"][icon]["steps"]
			elapsed[icon] += 1
			if elapsed[icon] >= int(sequence[steps[icon]]["duration"]):
				steps[icon] = (steps[icon] + 1) % sequence.size()
				elapsed[icon] = 0
	var tail: Dictionary = history[-1]
	if [tail["mode"], tail["stage"], tail["step"], tail["substep"], tail["frame_count"], tail["params"]] != [core["game_mode"], core["mode_stage"], core["mode_step"], core["substep"], core["frame_count"], rule["params"]]:
		return {"error": "holiday history does not end at the current committed state"}
	return {"error": "", "retain": screen.is_empty(), "text": text, "flags": flags,
		"steps": steps, "elapsed": elapsed, "colors": colors}

func _scene_commands(commands: Array[Dictionary], lba: int, rule: Dictionary) -> Dictionary:
	var expected: Array = []
	if lba != 0:
		if not _background_commands.has(lba):
			return {"error": "active holiday background has no verified original content"}
		expected = _background_commands[lba]
	var actor_count: int = 1 if rule["actor_state"].has("96") else 0
	if commands.size() != expected.size() + actor_count:
		return {"error": "holiday draw command count differs from active background/actor96"}
	var backgrounds: Array[Dictionary] = []
	var actors: Array[Dictionary] = []
	for index: int in range(commands.size()):
		var command: Dictionary = commands[index]
		var spec: Dictionary = {}
		if index < expected.size():
			spec = expected[index]
		elif _actor_specs.has(command.get("id")):
			spec = _actor_specs[command["id"]]
		if spec.is_empty() or not command.get("image") is Image or not command.get("position") is Vector2i or command.get("id") != spec["id"] or command["position"] != spec["position"] or command.get("blend_mode", 0) != 0:
			return {"error": "holiday draw identity/geometry/order differs from verified content"}
		var source: Image = command["image"]
		if source.is_empty() or source.get_format() not in [Image.FORMAT_RGB8, Image.FORMAT_RGBA8] or source.get_size() != spec["image"].get_size():
			return {"error": "holiday draw image shape/format differs from verified content"}
		var owned: Image = source.duplicate()
		owned.convert(Image.FORMAT_RGBA8)
		if _sha(owned.get_data()) != spec["sha256"]:
			return {"error": "holiday draw pixels differ from the authenticated original image"}
		var prepared: Dictionary = {"id": spec["id"], "image": owned, "position": spec["position"]}
		if index < expected.size():
			backgrounds.append(prepared)
		else:
			actors.append(prepared)
	return {"error": "", "backgrounds": backgrounds, "actors": actors}

func _add_background(lba: int, id: String, source: Image) -> bool:
	if source == null or source.get_size() != Vector2i(320, 88):
		return false
	var image: Image = source.duplicate()
	image.convert(Image.FORMAT_RGBA8)
	_backgrounds[lba] = {"id": id, "image": image}
	var specs: Array[Dictionary] = []
	for command: Dictionary in Canvas.background_commands(image, id):
		specs.append(_command_spec(command["id"], command["image"], command["position"]))
	_background_commands[lba] = specs
	return true

func _command_spec(id: String, source: Image, position: Vector2i) -> Dictionary:
	var image: Image = source.duplicate()
	image.convert(Image.FORMAT_RGBA8)
	return {"id": id, "image": image, "position": position, "sha256": _sha(image.get_data())}

func _clear_holiday() -> void:
	_opened = false
	_media = Media.new()
	_backgrounds.clear()
	_background_commands.clear()
	_actor_specs.clear()
	_holiday_text.clear()
	super._reset()

func _holiday_fail(message: String) -> bool:
	_clear_holiday()
	error = message
	return false

func _compose_fail(message: String) -> Dictionary:
	error = message
	return {"error": message, "commands": [], "retain": false}
