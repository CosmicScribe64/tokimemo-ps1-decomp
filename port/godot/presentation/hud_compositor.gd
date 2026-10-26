extends RefCounted
## Native primitive compositor. Only the validated presenter injects content.
## The v4 rendering, ordering, glyph and five-bit blending math is preserved.

const Layout = preload("res://ui/native_hud.gd")
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]
## T-29014 Section 12a point 1: 32x32 spring checkerboard frame tile (hud_screen_frame_draw, OT 15)
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA6ElEQVR4nMWWOw7CMAyGXatDRsaOHTkGR2HkCIwcpcdg7NgjdPQRGLOBEolKUZ4Kwv+/5vF9XmwP5215Uyb7c6XleqHezKeZbrsU7zCBw7kDa+1PHxtj+gWsEjwpoAmPBLThgQACfgig4F4ACfcCSLgrvrsP1OC18+//jIQnBTThkYA2PBBAwF1GWbfsI7k/SGx5nJYiL/EjvZRhkfw+0DLPS2nZJ5jA4X82mW4BowRPCmjCIwFteCCAgB8CKLgXQMK9ABLuiu/uAzV462xhJDwpoAmPBLThgQAC7jJOZso+apnnpbTsEx+OkpOxqxlT+wAAAABJRU5ErkJggg=="

var error := ""
var descriptor: Dictionary = {}
var last_coverage: Image
var _images: Dictionary = {}
var _font: RefCounted
var _checker_tile: Image

func configure(manifest: Dictionary, images: Dictionary, font: RefCounted) -> void:
	descriptor = {"frames": manifest["frames"].duplicate(true),
		"sequences": manifest["tables"]["sequences"].duplicate(true),
		"icon_positions": manifest["tables"]["icon_positions"].duplicate(true),
		"room": manifest["content"]["room"].duplicate(true), "text_runs": []}
	_images = images
	_font = font

func set_text_runs(runs: Array[Dictionary]) -> void:
	descriptor["text_runs"] = runs.duplicate(true)

func build_actors(state: Dictionary) -> Array[Dictionary]:
	error = ""
	if descriptor.is_empty():
		error = "HUD pack is not open"
		return []
	var actors: Array[Dictionary] = []
	var date: Dictionary = {}
	for key: String in ["year", "month", "day", "weekday", "day_kind"]:
		if not _integer(state.get(key)):
			error = "native HUD field is not an integer: " + key
			return []
		date[key] = int(state.get(key, -1))
	var calendar: Dictionary = Layout.build_calendar(date)
	var parameters: Dictionary = {}
	if not state.get("parameters") is Array or state["parameters"].size() != 9:
		error = "nine source parameter values are required"
		return []
	for i: int in range(9):
		if not _integer(state["parameters"][i]):
			error = "native HUD parameter is not an integer"
			return []
		parameters[Layout.STAT_LAYOUT[i]["name"]] = int(state["parameters"][i])
	var stats: Dictionary = Layout.build_parameters(parameters)
	if calendar.get("status") != "supported" or stats.get("status") != "supported":
		error = "date/parameter state is outside source bounds"
		return []
	for record: Dictionary in calendar["actors"] + stats["actors"]:
		if record["visible"]:
			if record["actor_index"] >= 32 and record["actor_index"] < 59 and state.has("parameter_colors"):
				record["color_mode"] = state["parameter_colors"][(record["actor_index"] - 32) / 3]
			actors.append(_actor(record["actor_index"], 4 if record["actor_index"] < 32 else 11,
				record["sprite_id"], Vector2i(record["x"], record["y"]),
				"hud-%02x" % record["color_mode"] if record["color_mode"] else "base"))
	actors.append(_actor(1, 7, 0x3e, Vector2i(0, 75)))
	if not _integer(state.get("selected")) or not _integer(state.get("cursor_frame")):
		error = "native HUD selection/cursor frame is not an integer"
		return []
	for key: String in ["icon_enabled", "icon_steps", "icon_elapsed"]:
		if not state.get(key) is Array or state[key].size() != 14:
			error = "native command HUD requires fourteen icon records"
			return []
	var selected: int = state["selected"]
	if selected < 0 or selected >= 14 or int(state["cursor_frame"]) < 0 or int(state["cursor_frame"]) >= 174:
		error = "native command selection and fourteen enabled flags are required"
		return []
	for i: int in range(14):
		if not _integer(state["icon_enabled"][i]) or int(state["icon_enabled"][i]) not in [0, 1] or not _integer(state["icon_steps"][i]) or not _integer(state["icon_elapsed"][i]):
			error = "native icon state differs from source integer/flag fields"
			return []
		var position := Vector2i(descriptor["icon_positions"][i][0], descriptor["icon_positions"][i][1])
		var enabled: bool = state["icon_enabled"][i] == 1
		var hovering: bool = enabled and i == selected
		var variant := "icon-52" if hovering else ("icon-53" if enabled else "icon-51")
		actors.append(_actor(64 + i, 3 if not enabled or hovering else 4, 2, position, variant))
		var sequence: Array = descriptor["sequences"][i]["steps"]
		var step: int = state["icon_steps"][i]
		if step < 0 or step >= sequence.size():
			error = "icon sequence step is outside the original sequence"
			return []
		actors.append(_actor(80 + i, 2 if hovering else 3, sequence[step]["frame_index"], position))
	var cursor_position := Vector2i(descriptor["icon_positions"][selected][0] + 6,
		descriptor["icon_positions"][selected][1] - 2)
	actors.append(_actor(0, 1, state["cursor_frame"], cursor_position, "base", false))
	return actors

func advance_icons(state: Dictionary, native_ticks: int) -> String:
	if native_ticks < 0 or native_ticks > 3600:
		return "native icon tick count is outside bounded update range"
	if build_actors(state).is_empty():
		return error
	for tick: int in range(native_ticks):
		for icon: int in range(14):
			if state["icon_enabled"][icon] != 1:
				continue
			var sequence: Array = descriptor["sequences"][icon]["steps"]
			var step: int = state["icon_steps"][icon]
			var elapsed: int = int(state["icon_elapsed"][icon]) + 1
			if elapsed >= int(sequence[step]["duration"]):
				step = (step + 1) % sequence.size()
				elapsed = 0
			state["icon_steps"][icon] = step
			state["icon_elapsed"][icon] = elapsed
	return ""

func render(state: Dictionary, previous_frame: Image = null,
	scene_commands: Array[Dictionary] = [], command_room: bool = true) -> Image:
	error = ""
	if descriptor.is_empty():
		error = "HUD pack is not open"
		return null
	var actors := build_actors(state)
	if not error.is_empty():
		return null
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	if previous_frame != null:
		if previous_frame.get_size() != Vector2i(320, 240):
			error = "previous native framebuffer must be 320x240"
			return null
		image = previous_frame.duplicate()
	else:
		_draw_checkerboard(image)
	last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
	last_coverage.fill(Color.BLACK)
	# dec_bg_draw(0), draw_shake_update: 160/120 + (-160,-48) => 0/72.
	if command_room:
		_blit(image, _image(descriptor["room"]), null, Vector2i(0, 72), false)
	else:
		for command: Dictionary in scene_commands:
			if not command.get("image") is Image or not command.get("position") is Vector2i:
				error = "activity composition requires prevalidated native image commands"
				return null
			_blit(image, command["image"], null, command["position"], false)
	var commands: Array[Dictionary] = actors.duplicate(true)
	commands.append({"kind": "gradient", "id": -1, "priority": 12, "rect": Rect2i(28, 23, 264, 42)})
	commands.append({"kind": "gradient", "id": -1, "priority": 8, "rect": Rect2i(27, 166, 266, 52)})
	commands.append({"kind": "gradient", "id": -1, "priority": 5, "rect": Rect2i(294, 45, 20, 114)})
	commands.sort_custom(func(a: Dictionary, b: Dictionary) -> bool:
		return a["priority"] > b["priority"] if a["priority"] != b["priority"] else a["id"] > b["id"])
	for command: Dictionary in commands:
		if command["id"] == 0:
			continue
		if command.get("kind") == "gradient":
			_gradient(image, command["rect"])
		else:
			_draw_actor(image, command)
	_draw_text(image)
	for actor: Dictionary in actors:
		if actor["id"] == 0:
			_draw_actor(image, actor)
	return null if not error.is_empty() else image

func _draw_checkerboard(target: Image) -> void:
	if _checker_tile == null:
		_checker_tile = Image.new()
		if _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64)) != OK:
			target.fill(Color8(40, 200, 160))
			return
	var w := _checker_tile.get_width()
	var h := _checker_tile.get_height()
	for ty in range(0, 240, 32):
		for tx in range(0, 320, 32):
			target.blit_rect(_checker_tile, Rect2i(0, 0, w, h), Vector2i(tx, ty))

func _draw_actor(target: Image, actor: Dictionary) -> void:
	var key := "%d:%s" % [actor["frame"], actor["variant"]]
	if not descriptor["frames"].has(key):
		error = "source HUD frame/palette is unavailable: " + key
		return
	var record: Dictionary = descriptor["frames"][key]
	# actor_draw_all queues parts count-1..0 into the head-linked OT, so GPU
	# traversal draws source part0..count-1. Blend each primitive separately.
	for part: Dictionary in record["parts"]:
		var position: Vector2i = Vector2i(160, 120) + actor["position"] + Vector2i(part["x"], part["y"]) - Vector2i(record["origin"][0], record["origin"][1])
		_blit(target, _image(part["png"]), _image(part["stp_png"]), position, actor["semi"])

func _draw_text(target: Image) -> void:
	for run: Dictionary in descriptor["text_runs"]:
		var codes: PackedByteArray = str(run["hex"]).hex_decode()
		var attribute: int = int(run.get("attribute", 0))
		var palette_row: int = attribute & 7
		var palette_family: int = (attribute >> 3) & 1
		var has_shadow: bool = attribute not in [3, 4, 5, 14, 15]
		for i: int in range(0, codes.size(), 2):
			var glyph: Dictionary = _font.get_glyph_image(codes.slice(i, i + 2), palette_row, palette_family)
			if glyph.get("status") != "glyph":
				error = "source text glyph is unavailable"
				return
			var foreground: Image = glyph["image"]
			var position := Vector2i(160 + int(run["position"][0]) + (i / 2) * 14, 120 + int(run["position"][1]))
			if has_shadow:
				var shadow: Image = foreground.duplicate()
				for y: int in range(12):
					for x: int in range(12):
						if shadow.get_pixel(x, y).a > 0:
							shadow.set_pixel(x, y, Color.BLACK)
				_blit(target, shadow, null, position + Vector2i.ONE, false)
			_blit(target, foreground, null, position, false)

func _image(spec: Dictionary) -> Image:
	var image: Image = _images.get(spec.get("path", ""))
	if image == null:
		error = "validated HUD payload is unavailable"
	return image

func _blit(target: Image, source: Image, stp: Image, position: Vector2i, semi: bool) -> void:
	if source == null:
		return
	for y: int in range(source.get_height()):
		for x: int in range(source.get_width()):
			var p := position + Vector2i(x, y)
			if p.x < 0 or p.y < 0 or p.x >= 320 or p.y >= 240:
				continue
			var c := source.get_pixel(x, y)
			if c.a == 0:
				continue
			var rgb := Vector3i(c.r8 >> 3, c.g8 >> 3, c.b8 >> 3)
			if semi and stp != null and stp.get_pixel(x, y).r8 != 0:
				var back := target.get_pixelv(p)
				rgb = Vector3i((back.r8 >> 4) + (rgb.x >> 1), (back.g8 >> 4) + (rgb.y >> 1), (back.b8 >> 4) + (rgb.z >> 1))
			target.set_pixelv(p, Color8(rgb.x << 3, rgb.y << 3, rgb.z << 3))
			last_coverage.set_pixelv(p, Color.WHITE)

func _gradient(target: Image, rect: Rect2i) -> void:
	for y: int in range(rect.size.y):
		for x: int in range(rect.size.x):
			var factor: float = 10.0 - 2.0 * x / rect.size.x - 4.0 * y / rect.size.y
			var d: int = DITHER[((rect.position.y + y) & 3) * 4 + ((rect.position.x + x) & 3)]
			var red: int = clampi((int(3 * factor) + d) >> 3, 0, 31)
			var green: int = clampi((int(14 * factor) + d) >> 3, 0, 31)
			target.set_pixelv(rect.position + Vector2i(x, y), Color8(red << 3, green << 3, 0))
			last_coverage.set_pixelv(rect.position + Vector2i(x, y), Color.WHITE)

static func _actor(index: int, layer: int, frame: int, position: Vector2i,
	variant := "base", semi := true) -> Dictionary:
	return {"kind": "actor", "id": index, "priority": layer - 1, "frame": frame,
		"position": position, "variant": variant, "semi": semi}

static func _integer(value: Variant) -> bool:
	return (value is int or (value is float and is_finite(value) and floor(value) == value)) and not value is bool
