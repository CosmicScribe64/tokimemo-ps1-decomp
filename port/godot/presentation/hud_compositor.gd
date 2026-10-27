extends RefCounted
## Native primitive compositor. Only the validated presenter injects content.
## The v4 rendering, ordering, glyph and five-bit blending math is preserved.

const Layout = preload("res://ui/native_hud.gd")
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]
## T-29014 Section 12a point 1: Canonical 32x32 spring checkerboard texture at VRAM (128..159, 128..159)
## normalized to (D_800B5BCC & 31) == 0, (D_800B5BD0 & 31) == 0 (hud_panels.c:1449-1450, OT 15).
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA8ElEQVR4nM2WoRHDMAxFX3MBhoWBGaWjFHaEwo6SMQIDO0KgRyg0a1nuco6lnO2TKyrL76EvXSY/fRFqvI48Vi89EWudF6b7Ldnvsn+uVKKAc67o8xBCvoAFPClgBT8UsIRHAtbwnUAL+CbQCg7QtYRDYQ5ocK3vnMvPgRpwyMyBWvBDAUt4JGANB+gHN4hD/uNZ50V8I84/X/jgk/1e2/XaPlcFghfvif++B0pD5kzIJQUs4EkBK/ihgCU8ErCG7wRawDeBVnCAriUcCnNAg5/ZLdk5UAMOmTlQC34oYAmPBKzhAL1f3uKQts+10u6JH97tla2x++MhAAAAAElFTkSuQmCC"
## Exact 4-season BGR555 CLUTs at D_800B5B28 (asm/data/data_800B5950.data.s:336-369, indices 6..15)
## uploaded to VRAM (0xA0, 0x1E1) by hud_screen_frame_draw (src/main/hud_panels.c:1462-1495).
const SEASON_CHECKER_CLUTS := [
	# 0: Spring (months 3, 4, 5)
	[0x73FC, 0x63D8, 0x5BD6, 0x57B5, 0x5394, 0x5325, 0x4F04, 0x4AE3, 0x3200, 0x3180],
	# 1: Summer (months 6, 7, 8)
	[0x7FF2, 0x7FB0, 0x7F6E, 0x7F4D, 0x7F2C, 0x7205, 0x6DE4, 0x69C3, 0x5180, 0x40A0],
	# 2: Autumn (months 9, 10, 11)
	[0x5BFF, 0x53BF, 0x4B7E, 0x475D, 0x433C, 0x1619, 0x11F8, 0x0DD7, 0x0D4E, 0x00B0],
	# 3: Winter (months 12, 1, 2)
	[0x7FDE, 0x7FBD, 0x7B7B, 0x775A, 0x7339, 0x5210, 0x4DEF, 0x49CE, 0x356A, 0x318C],
]

var error := ""
var descriptor: Dictionary = {}
var last_coverage: Image
var _images: Dictionary = {}
var _font: RefCounted
var _checker_tile: Image
var _season_tiles: Dictionary = {}
var _checker_uv := Vector2i.ZERO
var _checker_season := 0
var _checker_fade := 255

func set_checker_uv(uv: Vector2i) -> void:
	_checker_uv = uv

func set_checker_season(season: int, fade: int = 255) -> void:
	_checker_season = season & 3
	_checker_fade = clampi(fade, 239, 255)

static func hud_season_index(month: int) -> int:
	# Exact port of hud_season_index (src/main/hud_panels.c:1279-1299)
	match month:
		6, 7, 8:
			return 1
		9, 10, 11:
			return 2
		1, 2, 12:
			return 3
		_:
			return 0

static func compute_checker_uv(frame_count: int, health_mode: int = 0) -> Vector2i:
	var f := maxi(frame_count, 0)
	match health_mode & 15:
		1:
			return Vector2i((f + 1) >> 1, -((f + 1) >> 2))
		2:
			return Vector2i((f + 1) >> 2, -((f + 1) >> 3))
		3:
			var v := 0
			for i in range(1, f + 1):
				if (i & 0x15) == 0x15:
					v -= 1
			return Vector2i((f + 1) >> 3, v)
		_:
			# hud_screen_frame_draw default mode 0 (src/main/hud_panels.c:1408-1409):
			# if ((game_state.frame_count & 1) == 1) D_800B5BD0--;
			# D_800B5BCC++;
			return Vector2i(f, -((f + 1) >> 1))

static func resolve_cursor_frame(cursor_style: int) -> int:
	# hud_cursor_actor_init (src/main/hud_screens.c:1463-1479):
	# cursor_style == 0 -> frame 0 (pink hollow triangle); 0x9B..0xAD -> frame cursor_style.
	if cursor_style >= 0x9B and cursor_style < 0xAE:
		return cursor_style
	return 0

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
		var uv: Vector2i = state["checker_uv"] if state.get("checker_uv") is Vector2i else _checker_uv
		var season: int = int(state["checker_season"]) if state.has("checker_season") else (
			hud_season_index(int(state["month"])) if state.has("month") else _checker_season
		)
		var fade: int = int(state.get("checker_fade", _checker_fade))
		_draw_checkerboard(image, uv, season, fade)
	last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
	last_coverage.fill(Color.BLACK)
	# dec_bg_draw(0), draw_shake_update: 160/120 + (-160,-48) => 0/72.
	var fg_scene_commands: Array[Dictionary] = []
	if command_room:
		_blit(image, _image(descriptor["room"]), null, Vector2i(0, 72), false)
	else:
		for command: Dictionary in scene_commands:
			if not command.get("image") is Image or not command.get("position") is Vector2i:
				error = "activity composition requires prevalidated native image commands"
				return null
			var cid := str(command.get("id", ""))
			var is_bg: bool = cid.ends_with(":left") or cid.ends_with(":right") or (
				command["position"] in [Vector2i(0, 72), Vector2i(192, 72)] and command["image"].get_height() == 88
			)
			if is_bg:
				_blit(image, command["image"], null, command["position"], false)
			else:
				fg_scene_commands.append(command)
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
	# school_day_sd_actors_init / holiday_activity_sd_setup sets actor_pool[96..97].unk_01 = 1
	# (OT layer 0, drawn on top of HUD panels at OT 12..2 and before cursor actor 0).
	for command: Dictionary in fg_scene_commands:
		_blit(image, command["image"], null, command["position"], false)
	for actor: Dictionary in actors:
		if actor["id"] == 0:
			_draw_actor(image, actor)
	return null if not error.is_empty() else image

func _get_season_checker_tile(season: int = 0, fade: int = 255) -> Image:
	if _checker_tile == null:
		_checker_tile = Image.new()
		if _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64)) != OK:
			return null
	var s := season & 3
	var f := clampi(fade, 239, 255)
	if s == 0 and f == 255:
		return _checker_tile
	var cache_key := "%d:%d" % [s, f]
	if _season_tiles.has(cache_key):
		return _season_tiles[cache_key]
	# Build blended or target 10-entry CLUT (indices 6..15) per hud_screen_frame_draw (hud_panels.c:1469-1495)
	var spring_clut: Array = SEASON_CHECKER_CLUTS[0]
	var next_clut: Array = SEASON_CHECKER_CLUTS[s]
	var prev_clut: Array = SEASON_CHECKER_CLUTS[(s + 3) & 3]
	var color_map: Dictionary = {}
	for idx in range(spring_clut.size()):
		var c0: int = int(spring_clut[idx])
		var key_rgb5 := Vector3i(c0 & 31, (c0 >> 5) & 31, (c0 >> 10) & 31)
		var cn: int = int(next_clut[idx])
		var r5: int = cn & 31
		var g5: int = (cn >> 5) & 31
		var b5: int = (cn >> 10) & 31
		if f < 255:
			var co: int = int(prev_clut[idx])
			var ow: int = 255 - f
			var nw: int = f - 239
			r5 = (ow * (co & 31) + nw * r5) >> 4
			g5 = (ow * ((co >> 5) & 31) + nw * g5) >> 4
			b5 = (ow * ((co >> 10) & 31) + nw * b5) >> 4
		color_map[key_rgb5] = Color8(r5 * 8, g5 * 8, b5 * 8, 255)
	var w := _checker_tile.get_width()
	var h := _checker_tile.get_height()
	var out := Image.create(w, h, false, Image.FORMAT_RGBA8)
	for y in range(h):
		for x in range(w):
			var px := _checker_tile.get_pixel(x, y)
			var k := Vector3i(int(round(px.r * 255.0)) >> 3, int(round(px.g * 255.0)) >> 3, int(round(px.b * 255.0)) >> 3)
			out.set_pixel(x, y, color_map.get(k, px))
	_season_tiles[cache_key] = out
	return out

func _draw_checkerboard(target: Image, uv: Vector2i = _checker_uv, season: int = _checker_season, fade: int = _checker_fade) -> void:
	var base_tile := _get_season_checker_tile(season, fade)
	if base_tile == null:
		target.fill(Color8(40, 200, 160))
		return
	var w := base_tile.get_width()
	var h := base_tile.get_height()
	var u0 := int(uv.x) & (w - 1)
	var v0 := int(uv.y) & (h - 1)
	var tile_to_draw: Image = base_tile
	if u0 != 0 or v0 != 0:
		tile_to_draw = Image.create(w, h, false, base_tile.get_format())
		var rw := w - u0
		var rh := h - v0
		tile_to_draw.blit_rect(base_tile, Rect2i(u0, v0, rw, rh), Vector2i(0, 0))
		if u0 > 0:
			tile_to_draw.blit_rect(base_tile, Rect2i(0, v0, u0, rh), Vector2i(rw, 0))
		if v0 > 0:
			tile_to_draw.blit_rect(base_tile, Rect2i(u0, 0, rw, v0), Vector2i(0, rh))
		if u0 > 0 and v0 > 0:
			tile_to_draw.blit_rect(base_tile, Rect2i(0, 0, u0, v0), Vector2i(rw, rh))
	for ty in range(0, 240, 32):
		for tx in range(0, 320, 32):
			target.blit_rect(tile_to_draw, Rect2i(0, 0, w, h), Vector2i(tx, ty))

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
