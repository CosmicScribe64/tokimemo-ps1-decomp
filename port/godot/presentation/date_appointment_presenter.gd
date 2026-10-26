class_name DateAppointmentPresenter
extends "res://presentation/command_hud_presenter.gd"

const HolidayAppointmentRules = preload("res://rules/holiday_appointment.gd")
const Layout = preload("res://ui/native_hud.gd")

const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA/UlEQVR4nMWWvQ2DMBBGj1MKlxmBURjFZcqUlIzCCCkpXaZMSekRXLojshNZwUQYzM/3FQidjd9DQtwV1MiBoui6IW015UYbTbJVi/YWrW4nAuW1pFufL9B3ilpZLdrLBA7HBSHEpgOttfkC4mT4SAABDwIouBdAwr0AEu5enpFwF0bC/wqkvokUPLUen89I+EgAAQ8CKLgXQMJdCiIalFHZ/ZzUa1K6VxUZa/y9rOXs4xd3iXv/mn7+MB/Qbxy8+9afibmCCRw+8iezWsCeDB8JIOBBAAUPAij4ZoE9hhlGwl2yBPYc43hu8Wj4ROBs+EgAAQ8CKHgQQMEd9w2udJiVow0PSQAAAABJRU5ErkJggg=="
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

var _date_pack_root := ""
var _date_manifest: Dictionary = {}
var _holiday_room_image: Image
var _date_bg_images: Dictionary = {}
var _shiori_portrait_image: Image
var _checker_tile: Image
var _opened_date_pack := false


func open_date_preview(hud_manifest_path: String, font_root: String, date_pack_root: String, hud_sha: String = HUD_SHA) -> bool:
	_opened_date_pack = false
	_checker_tile = Image.new()
	if _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64)) != OK:
		error = "failed to decode checkerboard background tile"
		return false
	if not super.open_manifest(hud_manifest_path, hud_sha, font_root):
		return false

	var manifest_path := date_pack_root.path_join("manifest.json")
	if not FileAccess.file_exists(manifest_path):
		error = "date preview manifest is missing: " + manifest_path
		return false

	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.port.date_preview.v1":
		error = "date preview manifest schema mismatch"
		return false

	_date_pack_root = date_pack_root
	_date_manifest = parsed

	var room_info: Dictionary = _date_manifest["holiday_room_background"]
	_holiday_room_image = _load_png(date_pack_root.path_join(String(room_info["png"])))
	if _holiday_room_image == null:
		return false

	_date_bg_images.clear()
	for bg_info: Dictionary in _date_manifest["date_backgrounds"]:
		var bg_img := _load_png(date_pack_root.path_join(String(bg_info["png"])))
		if bg_img == null:
			return false
		_date_bg_images[int(bg_info["place_id"])] = bg_img

	var portrait_info: Dictionary = _date_manifest["shiori_portrait"]
	_shiori_portrait_image = _load_png(date_pack_root.path_join(String(portrait_info["static_png"])))
	if _shiori_portrait_image == null:
		return false

	_opened_date_pack = true
	error = ""
	return true


func _load_png(path: String) -> Image:
	var img := Image.new()
	if img.load(path) != OK:
		error = "failed to load PNG: " + path
		return null
	img.convert(Image.FORMAT_RGBA8)
	return img


static func speaker_x_for_hex(speaker_hex: String) -> int:
	# Exact port of gnsx (src/main/menu_speaker_girl.c:447-468)
	var byte_len := speaker_hex.length() / 2
	if byte_len == 2:
		return -114
	if byte_len == 4:
		return -121
	return -128


static func default_hud_view(state: HolidayAppointmentRules.HolidayState) -> Dictionary:
	var icon_steps: Array[int] = []
	var icon_elapsed: Array[int] = []
	for i in range(14):
		icon_steps.append(0)
		icon_elapsed.append(0)
	return {
		"year": state.year,
		"month": state.month,
		"day": state.day,
		"weekday": 6,
		"day_kind": 1,
		"parameters": [97, 43, 40, 40, 38, 33, 59, 4, 3],
		"selected": clampi(state.menu_cursor, 0, 13),
		"icon_enabled": state.icon_enabled.slice(0, 14),
		"cursor_frame": 156,
		"icon_steps": icon_steps,
		"icon_elapsed": icon_elapsed,
	}


func render_holiday_appointment_notice(state: HolidayAppointmentRules.HolidayState, notice: Dictionary) -> Dictionary:
	var view := default_hud_view(state)
	var speaker_hex := String(notice.get("speaker_hex", "93a18de8"))
	var runs: Array[Dictionary] = [
		{
			"hex": speaker_hex,
			"position": [speaker_x_for_hex(speaker_hex), 50],
			"attribute": 0,
		},
	]
	for line: Dictionary in notice.get("sndisp_lines", []):
		runs.append({
			"hex": String(line["bytes_hex"]),
			"position": [int(line["x"]), int(line["y"])],
			"attribute": 0,
		})
	_compositor.set_text_runs(runs)
	var base := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	_draw_checkerboard(base)
	var backgrounds: Array[Dictionary] = [
		{"image": _holiday_room_image, "position": Vector2i(0, 72)},
	]
	var image: Image = _compositor.render(view, base, backgrounds, false)
	return {
		"id": "holiday_appointment_notice",
		"image": image,
		"commands": [{"id": "holiday_appointment_notice", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
	}


func render_appointment_place_choice(state: HolidayAppointmentRules.HolidayState, menu_data: Dictionary, selected_row: int = 0) -> Dictionary:
	var view := default_hud_view(state)
	var runs: Array[Dictionary] = []
	for row: Dictionary in menu_data.get("rows", []):
		runs.append({
			"hex": String(row["bytes_hex"]),
			"position": [int(row["x"]), int(row["y"])],
			"attribute": 0,
		})
	_compositor.set_text_runs(runs)
	var image := _render_holiday_without_command_icons(view, true, selected_row)
	return {
		"id": "appointment_place_choice",
		"image": image,
		"commands": [{"id": "appointment_place_choice", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
		"selected_row": selected_row,
	}


func render_appointment_place_confirm(state: HolidayAppointmentRules.HolidayState, confirm_data: Dictionary) -> Dictionary:
	var view := default_hud_view(state)
	var speaker_hex := String(confirm_data.get("speaker_hex", "93a18de8"))
	var runs: Array[Dictionary] = [
		{
			"hex": speaker_hex,
			"position": [speaker_x_for_hex(speaker_hex), 50],
			"attribute": 0,
		},
	]
	for line: Dictionary in confirm_data.get("sndisp_lines", []):
		runs.append({
			"hex": String(line["bytes_hex"]),
			"position": [int(line["x"]), int(line["y"])],
			"attribute": 0,
		})
	_compositor.set_text_runs(runs)
	var image := _render_holiday_without_command_icons(view, false, 0)
	return {
		"id": "appointment_place_confirm",
		"image": image,
		"commands": [{"id": "appointment_place_confirm", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
	}


func render_date_arrival_scene(state: HolidayAppointmentRules.HolidayState, launch_payload: Dictionary) -> Dictionary:
	var place_id := int(launch_payload.get("chosen_place", 0))
	var bg_image: Image = _date_bg_images.get(place_id, _date_bg_images.get(0))
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	# hud_screen_frame_draw(2) in src/ovl/date/entry.c:736 tiles checkerboard behind bottom message window
	_draw_checkerboard(image)

	# 1. Date spot 320x160 background at (0, 0) (dec_bg_draw(1) in place_init)
	_compositor._blit(image, bg_image, null, Vector2i(0, 0), false)

	# 2. Shiori Spring portrait (actor_pool[31] at (0, -32), origin (79, 80) -> screen (81, 8))
	var portrait_info: Dictionary = _date_manifest["shiori_portrait"]
	var portrait_pos := Vector2i(int(portrait_info["screen_x"]), int(portrait_info["screen_y"]))
	_compositor._blit(image, _shiori_portrait_image, null, portrait_pos, false)

	# 3. Date HUD: hizuke_draw(0, -38) shifts the calendar panel by (0, -38) to top-right,
	# plus bottom message window (actor 1 at (0, 75) + gradient Rect2i(27, 166, 266, 52)).
	var date_info := {
		"year": state.year,
		"month": state.month,
		"day": state.day,
		"weekday": 6,
		"day_kind": 1,
	}
	var cal: Dictionary = Layout.build_calendar(date_info)
	var commands: Array[Dictionary] = []
	for record: Dictionary in cal["actors"]:
		if record["visible"]:
			commands.append(_compositor._actor(
				record["actor_index"],
				4,
				record["sprite_id"],
				Vector2i(record["x"], record["y"] - 38),
				"hud-%02x" % record["color_mode"] if record["color_mode"] else "base"
			))
	commands.append(_compositor._actor(1, 7, 0x3e, Vector2i(0, 75)))
	commands.append({"kind": "gradient", "id": -1, "priority": 8, "rect": Rect2i(27, 166, 266, 52)})
	commands.append({"kind": "gradient", "id": -1, "priority": 5, "rect": Rect2i(294, 7, 20, 114)})
	commands.sort_custom(func(a: Dictionary, b: Dictionary) -> bool:
		return a["priority"] > b["priority"] if a["priority"] != b["priority"] else a["id"] > b["id"]
	)
	for cmd: Dictionary in commands:
		if cmd.get("kind") == "gradient":
			_compositor._gradient(image, cmd["rect"])
		else:
			_compositor._draw_actor(image, cmd)

	# 4. Top-left place title banner (place_title_sprite_init + place_title_show)
	var pinfo := HolidayAppointmentRules.place_name(place_id)
	_draw_place_title_banner(image, String(pinfo["bytes_hex"]))

	# 5. Arrival greeting from CDROM/SERIFU/DATE_FU.BIN (group 6, line 1)
	var arrival_info: Dictionary = _date_manifest["shiori_date_script"]["arrival_direct_greeting"]
	var speaker_hex := String(arrival_info["speaker_bytes_hex"])
	var arrival_lines := HolidayAppointmentRules.sndisp_lines(
		String(arrival_info["bytes_hex"]),
		String(arrival_info["text"]),
		0,
		HolidayAppointmentRules.SPEAKER_STYLE_GIRL
	)
	var runs: Array[Dictionary] = [
		{
			"hex": speaker_hex,
			"position": [speaker_x_for_hex(speaker_hex), 50],
			"attribute": 0,
		},
	]
	for line: Dictionary in arrival_lines:
		runs.append({
			"hex": String(line["bytes_hex"]),
			"position": [int(line["x"]), int(line["y"])],
			"attribute": 0,
		})
	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)

	return {
		"id": "date_arrival_scene",
		"image": image,
		"commands": [{"id": "date_arrival_scene", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
		"arrival_lines": arrival_lines,
		"place_name": String(pinfo["text"]),
	}


func _render_holiday_without_command_icons(view: Dictionary, show_menu_bar: bool, selected_row: int) -> Image:
	var all_actors: Array[Dictionary] = _compositor.build_actors(view)
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	_draw_checkerboard(image)
	_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
	_compositor.last_coverage.fill(Color.BLACK)

	# Holiday room background at (0, 72)
	_compositor._blit(image, _holiday_room_image, null, Vector2i(0, 72), false)

	# Keep parameter panel (32..58), calendar panel (<32), and message box (1);
	# hide command icons (64..93) and cursor (0) per holiday_Command_hud_hide + holiday_cursor_Sprt_hide.
	var commands: Array[Dictionary] = []
	for actor: Dictionary in all_actors:
		var aid := int(actor["id"])
		if aid == 0 or (aid >= 64 and aid <= 93):
			continue
		commands.append(actor)
	commands.append({"kind": "gradient", "id": -1, "priority": 12, "rect": Rect2i(28, 23, 264, 42)})
	commands.append({"kind": "gradient", "id": -1, "priority": 8, "rect": Rect2i(27, 166, 266, 52)})
	commands.append({"kind": "gradient", "id": -1, "priority": 5, "rect": Rect2i(294, 45, 20, 114)})
	commands.sort_custom(func(a: Dictionary, b: Dictionary) -> bool:
		return a["priority"] > b["priority"] if a["priority"] != b["priority"] else a["id"] > b["id"]
	)
	for cmd: Dictionary in commands:
		if cmd.get("kind") == "gradient":
			_compositor._gradient(image, cmd["rect"])
		else:
			_compositor._draw_actor(image, cmd)

	if show_menu_bar:
		# menu_bar_show(1): style 0x822F7F2F semi-transparent highlight bar at (40, 170 + row * 16, 240, 16)
		var bar_rect := Rect2i(40, 170 + selected_row * 16, 240, 16)
		_draw_semi_bar(image, bar_rect, Vector3i(0x2F >> 3, 0x7F >> 3, 0x2F >> 3))

	_compositor._draw_text(image)
	return image


func _draw_checkerboard(target: Image) -> void:
	if _checker_tile == null:
		target.fill(Color8(40, 200, 160))
		return
	var w := _checker_tile.get_width()
	var h := _checker_tile.get_height()
	for ty in range(0, 240, 32):
		for tx in range(0, 320, 32):
			target.blit_rect(_checker_tile, Rect2i(0, 0, w, h), Vector2i(tx, ty))


func _draw_semi_bar(target: Image, rect: Rect2i, _rgb5: Vector3i) -> void:
	# Exact PS1 POLY_G4 case 0 for menu_bar_style[1] = 0x822F7F2F (menu_speaker_girl.c:464, gpu_primitives.c:231-237):
	# v0=(214,246,214), v1=(120,248,120), v2=(26,250,26), v3=(188,252,188)
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240:
			continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320:
				continue
			var fx: float = float(x) / float(rect.size.x)
			var fr_rb: float
			var fr_g: float
			if fx + fy <= 1.0:
				fr_rb = 214.0 + fx * (120.0 - 214.0) + fy * (26.0 - 214.0)
				fr_g = 246.0 + fx * (248.0 - 246.0) + fy * (250.0 - 246.0)
			else:
				var u: float = 1.0 - fx
				var v: float = 1.0 - fy
				fr_rb = 188.0 + u * (26.0 - 188.0) + v * (120.0 - 188.0)
				fr_g = 252.0 + u * (250.0 - 252.0) + v * (248.0 - 252.0)
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var f_rb5: int = clampi((int(fr_rb) + d) >> 3, 0, 31)
			var f_g5: int = clampi((int(fr_g) + d) >> 3, 0, 31)
			var back := target.get_pixel(px, py)
			var out_r5: int = clampi((back.r8 >> 4) + (f_rb5 >> 1), 0, 31)
			var out_g5: int = clampi((back.g8 >> 4) + (f_g5 >> 1), 0, 31)
			var out_b5: int = clampi((back.b8 >> 4) + (f_rb5 >> 1), 0, 31)
			target.set_pixelv(Vector2i(px, py), Color8(out_r5 << 3, out_g5 << 3, out_b5 << 3, 255))


func _draw_place_title_banner(target: Image, place_hex: String) -> void:
	# message_window_set_title + message_window_title_update settled state (src/main/hud_panels.c:1173-1260):
	var glyph_count := place_hex.length() / 4
	var grad_rect := Rect2i(160 - 144, 120 - 90, glyph_count * 14 + 8, 20)
	_compositor._gradient(target, grad_rect)
	for i in range(glyph_count + 1, -1, -1):
		var ax := (glyph_count - i) * 14 - 135
		var ay := -81
		var sprite_id := 0x72
		if i == 0:
			sprite_id = 0x73
			ax -= 2
		elif i == glyph_count + 1:
			sprite_id = 0x71
			ax += 2
		var actor_cmd: Dictionary = _compositor._actor(i + 3, 2, sprite_id, Vector2i(ax, ay))
		_compositor._draw_actor(target, actor_cmd)
	var prev_runs: Array = _compositor.descriptor["text_runs"].duplicate(true)
	var banner_runs: Array[Dictionary] = [
		{"hex": place_hex, "position": [-142, -88], "attribute": 0},
	]
	_compositor.set_text_runs(banner_runs)
	_compositor._draw_text(target)
	_compositor.set_text_runs(prev_runs)
