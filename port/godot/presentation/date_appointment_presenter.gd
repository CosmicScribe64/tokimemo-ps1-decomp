class_name DateAppointmentPresenter
extends "res://presentation/command_hud_presenter.gd"

const HolidayAppointmentRules = preload("res://rules/holiday_appointment.gd")
const Layout = preload("res://ui/native_hud.gd")

const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA5ElEQVR4nMWWsQ3DIBBFzycXlIzgUTIKZUZI6VEYI6XHcHkjpKRLdBRIECQMTvx/aSPeQ0L8m2h1byoij5UkCI1GXkLOb4fWTl78l8BiF7rv4wL7cyPvbofWMoHD5QdjzKkNQwjjAuZieCaAgCcBFDwKIOFRAAnXwzMSrmEkvCrQuhMteOt/uT8j4ZkAAp4EUPAogIRrZku22udaqaPpmSfmWu/39HlVIMjheYIJHP7nI9MtEC6GZwIIeBJAwaMAEh4FkHA9/LDAryYpRsKrAmff9t5uYSQ8E0DAkwAKHgWQcBX4AGcPlHlFD2CfAAAAAElFTkSuQmCC"
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
	# Exact port of gnsx (src/main/menu_speaker_girl.c:447-468) for Holiday HUD sndisp
	var byte_len := speaker_hex.length() / 2
	if byte_len == 2:
		return -114
	if byte_len == 4:
		return -121
	return -128


static func talk_speaker_x_for_hex(speaker_hex: String) -> int:
	# Exact port of talk_speak_part speaker x (-0x68 - (strlen(speaker) * 4), src/main/scene_library.c:1342, 1579, 1645)
	return HolidayAppointmentRules.talk_speaker_x_for_hex(speaker_hex)


static func default_hud_view(state: HolidayAppointmentRules.HolidayState) -> Dictionary:
	var icon_steps: Array[int] = []
	var icon_elapsed: Array[int] = []
	for i in range(14):
		icon_steps.append(0)
		icon_elapsed.append(0)
	var params: Array[int] = []
	if state.parameters.size() == 9:
		params = state.parameters.duplicate()
	else:
		params = [97, 43, 40, 40, 38, 33, 59, 4, 3]
	return {
		"year": state.year,
		"month": state.month,
		"day": state.day,
		"weekday": state.weekday,
		"day_kind": state.day_kind,
		"parameters": params,
		"selected": clampi(state.menu_cursor, 0, 13),
		"icon_enabled": state.icon_enabled.slice(0, 14),
		"cursor_frame": NativeCompositor.resolve_cursor_frame(state.cursor_style),
		"checker_uv": NativeCompositor.compute_checker_uv(state.frame_count, (state.player_flags >> 8) & 15),
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
	_draw_checkerboard(base, view["checker_uv"])
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


func _render_date_base_scene(state: HolidayAppointmentRules.HolidayState, place_id: int) -> Image:
	var bg_image: Image = _date_bg_images.get(place_id, _date_bg_images.get(0))
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
	_compositor.last_coverage.fill(Color.BLACK)

	# 1. hud_screen_frame_draw(2) in src/ovl/date/entry.c:736 tiles checkerboard behind bottom message window
	var checker_uv := NativeCompositor.compute_checker_uv(state.frame_count, (state.player_flags >> 8) & 15)
	var checker_season := NativeCompositor.hud_season_index(state.month)
	_draw_checkerboard(image, checker_uv, checker_season)

	# 2. Date spot 320x160 background at (0, 0) (dec_bg_draw(1) in place_init)
	_compositor._blit(image, bg_image, null, Vector2i(0, 0), false)

	# 3. Shiori Spring portrait (actor_pool[31] at (0, -32), origin (79, 80) -> screen (81, 8))
	var portrait_info: Dictionary = _date_manifest["shiori_portrait"]
	var portrait_pos := Vector2i(int(portrait_info["screen_x"]), int(portrait_info["screen_y"]))
	_compositor._blit(image, _shiori_portrait_image, null, portrait_pos, false)

	# 4. Date HUD: hizuke_init() + hizuke_show() (src/main/hud_panels.c:639-738, src/ovl/date/entry.c:687, 740)
	# Draws Date Pillar at standard coordinates (record["x"], record["y"]) and shadow Rect2i(294, 45, 20, 114),
	# plus bottom message window (actor 1 at (0, 75) + gradient Rect2i(27, 166, 266, 52)).
	var date_info := {
		"year": state.year,
		"month": state.month,
		"day": state.day,
		"weekday": state.weekday,
		"day_kind": state.day_kind,
	}
	var cal: Dictionary = Layout.build_calendar(date_info)
	var commands: Array[Dictionary] = []
	for record: Dictionary in cal["actors"]:
		if record["visible"]:
			commands.append(_compositor._actor(
				record["actor_index"],
				4,
				record["sprite_id"],
				Vector2i(record["x"], record["y"]),
				"hud-%02x" % record["color_mode"] if record["color_mode"] else "base"
			))
	commands.append(_compositor._actor(1, 7, 0x3e, Vector2i(0, 75)))
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

	# 5. Top-left place title banner (place_title_sprite_init + place_title_show)
	var pinfo := HolidayAppointmentRules.place_name(place_id)
	_draw_place_title_banner(image, String(pinfo["bytes_hex"]))

	return image


func _draw_vn_wait_marker(image: Image, frame_count: int = 0) -> Dictionary:
	# Exact port of actor_pool[2] configured by hud_text_marker_set (src/main/hud_screens.c:1740-1758),
	# bobbed by hud_text_marker_bob (src/main/hud_screens.c:1760-1768), and enabled by talk_run
	# when dialogue->line_wait != 0 and check_end_k() != 0 (src/main/scene_library.c:2111-2112).
	var spec := HolidayAppointmentRules.hud_text_marker_actor_spec(frame_count, true)
	var marker_cmd: Dictionary = _compositor._actor(
		int(spec["actor_index"]),
		int(spec["priority"]),
		int(spec["frame"]),
		Vector2i(int(spec["x"]), int(spec["y"]))
	)
	_compositor._draw_actor(image, marker_cmd)
	return spec


static func _compute_glyph_bboxes(runs: Array) -> Dictionary:
	var boxes: Array[Dictionary] = []
	var violations: Array[Dictionary] = []
	var msg_box := Rect2i(27, 166, 266, 52)
	for run: Dictionary in runs:
		var hex_str := String(run.get("hex", ""))
		var pos: Array = run.get("position", [0, 0])
		var x0: int = 160 + int(pos[0])
		var y0: int = 120 + int(pos[1])
		var char_count: int = hex_str.length() / 4
		for idx: int in range(char_count):
			var rect := Rect2i(x0 + idx * 14, y0, 16, 16)
			boxes.append({"rect": rect, "code_hex": hex_str.substr(idx * 4, 4)})
			if not msg_box.encloses(rect):
				violations.append({"rect": rect, "box": msg_box})
	return {"glyph_bboxes": boxes, "bbox_violations": violations}


func render_date_arrival_scene(state: HolidayAppointmentRules.HolidayState, launch_payload: Dictionary) -> Dictionary:
	var place_id := int(launch_payload.get("chosen_place", 0))
	var image := _render_date_base_scene(state, place_id)
	var pinfo := HolidayAppointmentRules.place_name(place_id)

	# Arrival greeting from CDROM/SERIFU/DATE_FU.BIN (group 6, line 1) via talk_speak_part + talk_render
	# (src/main/scene_library.c:1185, 1340-1346, 1681-2052):
	# Speaker at (-120, 48), opening quote "「" at (-86, 48), body lines at (-72, 48 + row * 16).
	var arrival_info: Dictionary = _date_manifest["shiori_date_script"]["arrival_direct_greeting"]
	var speaker_hex := String(arrival_info["speaker_bytes_hex"])
	var speaker_text := String(arrival_info.get("speaker", "藤崎"))
	var speak_payload := HolidayAppointmentRules.talk_speak_runs(
		String(arrival_info["bytes_hex"]),
		String(arrival_info["text"]),
		speaker_hex,
		speaker_text,
		HolidayAppointmentRules.TALK_CMD_GIRL,
		0
	)
	var runs: Array[Dictionary] = speak_payload["text_runs"]
	var arrival_lines: Array[Dictionary] = speak_payload["lines"]
	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)

	# Blinking/bobbing wait marker actor_pool[2] at (116, 72..78) when line_wait != 0
	var marker_spec := {}
	if state.date_line_wait != 0:
		marker_spec = _draw_vn_wait_marker(image, state.frame_count)
	var bbox_info := _compute_glyph_bboxes(runs)

	return {
		"id": "date_arrival_scene",
		"image": image,
		"commands": [{"id": "date_arrival_scene", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
		"arrival_lines": arrival_lines,
		"wait_marker": marker_spec,
		"pillar_shadow_rect": Rect2i(294, 45, 20, 114),
		"glyph_bboxes": bbox_info["glyph_bboxes"],
		"bbox_violations": bbox_info["bbox_violations"],
		"place_name": String(pinfo["text"]),
	}


func render_date_three_choice_prompt(state: HolidayAppointmentRules.HolidayState, prompt_data: Dictionary = {}, selected_row: int = -1) -> Dictionary:
	# Exact port of normal_date_three_select (src/main/scene_library.c:1583-1593, 2196-2253) +
	# menu_bar_show(1) & menu_cursor_update(1) (src/main/menu_speaker_girl.c:414-468):
	var place_id := state.chosen_place if state.chosen_place != 0xFF else int(prompt_data.get("place_id", 0))
	var prompt := prompt_data if not prompt_data.is_empty() else HolidayAppointmentRules.build_date_three_choice_prompt(place_id, state.date_choice_order)
	var row := clampi(selected_row if selected_row >= 0 else state.date_choice_cursor, 0, 2)
	var image := _render_date_base_scene(state, place_id)

	# 1. menu_bar_show(1): style 0x822F7F2F (case 0 semi-transparent green bar) at Rect2i(32, 168 + row * 16, 252, 16)
	var bar_rect := Rect2i(
		160 + HolidayAppointmentRules.DATE_THREE_SELECT_X,
		120 + HolidayAppointmentRules.DATE_THREE_SELECT_Y0 + row * HolidayAppointmentRules.DATE_THREE_SELECT_STEP_Y,
		HolidayAppointmentRules.DATE_THREE_SELECT_W,
		HolidayAppointmentRules.DATE_THREE_SELECT_H
	)
	_draw_semi_bar(image, bar_rect, Vector3i(0x2F >> 3, 0x7F >> 3, 0x2F >> 3))

	# 2. 3 choice rows: "『" at x = -128, option text + "』" at x = -114, y = 48, 64, 80
	var runs: Array[Dictionary] = []
	for r: Dictionary in prompt.get("text_runs", []):
		runs.append(r)
	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)

	# 3. Cursor actor_pool[0] resolved via hud_cursor_actor_init(0) at (120, 56 + row * 16)
	var cursor_pos := Vector2i(
		HolidayAppointmentRules.DATE_THREE_SELECT_CURSOR_X,
		HolidayAppointmentRules.DATE_THREE_SELECT_CURSOR_Y0 + row * HolidayAppointmentRules.DATE_THREE_SELECT_STEP_Y
	)
	var cursor_frame := NativeCompositor.resolve_cursor_frame(state.cursor_style)
	var cursor_cmd: Dictionary = _compositor._actor(0, 1, cursor_frame, cursor_pos, "base", false)
	_compositor._draw_actor(image, cursor_cmd)
	var bbox_info := _compute_glyph_bboxes(runs)

	return {
		"id": "date_three_choice_prompt",
		"image": image,
		"commands": [{"id": "date_three_choice_prompt", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
		"rows": prompt.get("rows", []),
		"selected_row": row,
		"bar_rect": bar_rect,
		"highlight_rect": bar_rect,
		"cursor_position": cursor_pos,
		"cursor_native_pos": cursor_pos,
		"cursor_frame": cursor_frame,
		"glyph_bboxes": bbox_info["glyph_bboxes"],
		"bbox_violations": bbox_info["bbox_violations"],
	}


func render_date_choice_response(state: HolidayAppointmentRules.HolidayState, response_data: Dictionary = {}) -> Dictionary:
	# Exact port of talk_speak_part case 0x2 (src/main/scene_library.c:1604-1658) + wait marker actor_pool[2]:
	var place_id := state.chosen_place if state.chosen_place != 0xFF else int(response_data.get("place_id", 0))
	var choice_id := state.date_chosen_option if state.date_chosen_option >= 0 else int(response_data.get("choice_id", 0))
	var response := response_data if not response_data.is_empty() else HolidayAppointmentRules.build_date_choice_response(place_id, choice_id)
	var image := _render_date_base_scene(state, place_id)

	var runs: Array[Dictionary] = []
	for r: Dictionary in response.get("text_runs", []):
		runs.append(r)
	_compositor.set_text_runs(runs)
	_compositor._draw_text(image)

	var marker_spec := {}
	if state.date_line_wait != 0:
		marker_spec = _draw_vn_wait_marker(image, state.frame_count)
	var bbox_info := _compute_glyph_bboxes(runs)

	return {
		"id": "date_choice_response",
		"image": image,
		"commands": [{"id": "date_choice_response", "image": image, "position": Vector2i.ZERO, "blend_mode": 0}],
		"text_runs": runs,
		"reply_lines": response.get("reply_lines", []),
		"reply_text": String(response.get("reply_text", "")),
		"choice_id": choice_id,
		"score_class": int(response.get("score_class", 0)),
		"wait_marker": marker_spec,
		"glyph_bboxes": bbox_info["glyph_bboxes"],
		"bbox_violations": bbox_info["bbox_violations"],
	}


func _render_holiday_without_command_icons(view: Dictionary, show_menu_bar: bool, selected_row: int) -> Image:
	var all_actors: Array[Dictionary] = _compositor.build_actors(view)
	var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var checker_uv: Vector2i = view["checker_uv"] if view.get("checker_uv") is Vector2i else Vector2i.ZERO
	var checker_season: int = NativeCompositor.hud_season_index(int(view.get("month", 4)))
	_draw_checkerboard(image, checker_uv, checker_season)
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

	if show_menu_bar:
		# menu_cursor_update(1) for x_taku_menu_set(1, count): cursor_x = -120 + 240 - 4 = 116, cursor_y = 50 + row * 16 + 8 = 58 + row * 16
		var cursor_frame := int(view.get("cursor_frame", 156))
		var cursor_cmd: Dictionary = _compositor._actor(0, 1, cursor_frame, Vector2i(116, 58 + selected_row * 16), "base", false)
		_compositor._draw_actor(image, cursor_cmd)

	return image


func _draw_checkerboard(target: Image, uv: Vector2i = Vector2i.ZERO, season: int = 0, fade: int = 255) -> void:
	if _checker_tile == null:
		target.fill(Color8(40, 200, 160))
		return
	_compositor._draw_checkerboard(target, uv, season, fade)


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
