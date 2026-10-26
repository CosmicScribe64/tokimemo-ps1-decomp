extends "res://presentation/command_hud_presenter.gd"
class_name VnScenePresenter
## T-29018: Unified VN Scene and Overlay Presenter.
## Consolidates all verified PS1 320x240 presentation primitives into a single reusable
## presenter class so all VN and overlay screens (TEL, DATE, DATE2, GEKO, EVENT, OMIMAI,
## VALEN, SHOUGATU, ENDING, OPTION, OLH, ETC) can render authentic native framebuffers
## without duplicating boilerplate.

const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"

# Authentic PS1 32x32 green checkerboard tile (hud_screen_frame_draw, hud_panels.c:1366)
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA5ElEQVR4nMWWsQ3DIBBFzycXlIzgUTIKZUZI6VEYI6XHcHkjpKRLdBRIECQMTvx/aSPeQ0L8m2h1byoij5UkCI1GXkLOb4fWTl78l8BiF7rv4wL7cyPvbofWMoHD5QdjzKkNQwjjAuZieCaAgCcBFDwKIOFRAAnXwzMSrmEkvCrQuhMteOt/uT8j4ZkAAp4EUPAogIRrZku22udaqaPpmSfmWu/39HlVIMjheYIJHP7nI9MtEC6GZwIIeBJAwaMAEh4FkHA9/LDAryYpRsKrAmff9t5uYSQ8E0DAkwAKHgWQcBX4AGcPlHlFD2CfAAAAAElFTkSuQmCC"

# PS1 Bayer 4x4 dither table
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Authentic PS1 menu highlight bar styles (menu_bar_color, menu_speaker_girl.c:461-469)
# Format: 0x80000000 | (ordering_slot << 24) | (rgb_color)
const MENU_BAR_STYLES := [
	0x832F7FAF, # Style 0: RGB(0x2F, 0x7F, 0xAF) Cyan-Blue
	0x822F7F2F, # Style 1: RGB(0x2F, 0x7F, 0x2F) Green
	0x812F4F8F, # Style 2: RGB(0x2F, 0x4F, 0x8F) Purple-Magenta
]

# 9 stats coordinates matching Layout.STAT_LAYOUT in native_hud.gd
const STAT_LAYOUT := [
	{"x": -0x49, "y": -0x58}, # condition
	{"x": 0x0F, "y": -0x58},  # liberal_arts
	{"x": 0x67, "y": -0x58},  # science
	{"x": -0x49, "y": -0x4A}, # art
	{"x": 0x0F, "y": -0x4A},  # athletics
	{"x": 0x67, "y": -0x4A},  # trivia
	{"x": -0x49, "y": -0x3C}, # looks
	{"x": 0x0F, "y": -0x3C},  # guts
	{"x": 0x67, "y": -0x3C},  # stress
]

var _checker_tile: Image
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []

func init_resources(font_root: String, hud_manifest_path: String = "") -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()

	# 1. Load authentic PS1 checkerboard tile
	_checker_tile = Image.new()
	var err: Error = _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64))
	if err != OK:
		return "Failed to decode checkerboard background tile"

	if hud_manifest_path.is_empty():
		var default_manifest := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
		if FileAccess.file_exists(default_manifest):
			hud_manifest_path = default_manifest

	# 2. Open HUD manifest or font directly
	if not hud_manifest_path.is_empty() and FileAccess.file_exists(hud_manifest_path):
		if not open_manifest(hud_manifest_path, HUD_SHA, font_root):
			return "Failed to open HUD manifest: " + error
	else:
		var font_err: String = _font.open_external_font(font_root)
		if not font_err.is_empty():
			return "Failed to open font: " + font_err

	_ensure_compositor_coverage()
	return ""

func _ensure_compositor_coverage() -> void:
	if _compositor != null and _compositor.last_coverage == null:
		_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
		_compositor.last_coverage.fill(Color.BLACK)

# ==============================================================================
# Checkerboard Frame Primitive
# ==============================================================================

## Exact 10x8 tile enable mask from hud_screen_frame_draw(a) (src/main/hud_panels.c:1412-1438).
## Mode 0: Full screen (all 80 tiles).
## Mode 1: Outer border frame (rows 1..5, cols 1..8 disabled -> 40 tiles).
## Mode 2: Dialog/bottom frame (rows 0..4 disabled; row 5 cols 0, 9; rows 6..7 cols 0..9 -> 22 tiles).
static func is_checker_tile_enabled(mode: int, col: int, row: int) -> bool:
	if col < 0 or col >= 10 or row < 0 or row >= 8:
		return false
	match mode:
		0:
			return true
		1:
			if row >= 1 and row < 6 and col >= 1 and col < 9:
				return false
			return true
		2:
			if row < 5:
				return false
			if row == 5:
				return col == 0 or col == 9
			return true
		_:
			return true

## Exact hud_screen_frame_draw(0/1/2) with UV scroll (src/main/hud_panels.c:1366-1461).
func draw_checkerboard_frame(target: Image, mode: int, bg_uv: Vector2i = Vector2i.ZERO) -> void:
	if _checker_tile == null:
		return
	var u0: int = bg_uv.x & 31
	var v0: int = bg_uv.y & 31
	for row in range(8):
		for col in range(10):
			if not is_checker_tile_enabled(mode, col, row):
				continue
			var base_x: int = col * 32
			var base_y: int = row * 32
			for dy in range(32):
				var py: int = base_y + dy
				if py < 0 or py >= 240:
					continue
				var vt: int = (v0 + dy) & 31
				for dx in range(32):
					var px: int = base_x + dx
					if px < 0 or px >= 320:
						continue
					var ut: int = (u0 + dx) & 31
					target.set_pixelv(Vector2i(px, py), _checker_tile.get_pixel(ut, vt))

# ==============================================================================
# PS1 Presentation Primitives
# ==============================================================================

## Top wooden parameter panel (Frame 61, OT 12..11) with 9 stats x 3 digits.
func draw_parameter_panel(target: Image, stats: Array[int]) -> void:
	_ensure_compositor_coverage()
	# Gradient box: Rect2i(28, 23, 264, 42)
	_compositor._gradient(target, Rect2i(28, 23, 264, 42))
	# Frame 61 (wooden parameter frame) at (0, -72)
	_draw_actor(target, 61, Vector2i(0, -72))

	for stat_idx in range(mini(stats.size(), STAT_LAYOUT.size())):
		var val: int = stats[stat_idx]
		var layout: Dictionary = STAT_LAYOUT[stat_idx]
		var d_hundreds: int = (val / 100) % 10
		var d_tens: int = (val / 10) % 10
		var d_ones: int = val % 10
		_draw_actor(target, d_hundreds + 0x44, Vector2i(layout["x"], layout["y"]))
		_draw_actor(target, d_tens + 0x44, Vector2i(layout["x"] + 8, layout["y"]))
		_draw_actor(target, d_ones + 0x44, Vector2i(layout["x"] + 16, layout["y"]))

## Bottom wooden message window (Frame 62, OT 8..7).
func draw_message_window(target: Image) -> void:
	_ensure_compositor_coverage()
	# OT 8: prim_gradient_rect(-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0) -> Rect2i(27, 166, 266, 52)
	_compositor._gradient(target, Rect2i(27, 166, 266, 52))
	# OT 7: Actor 1 Frame 62 (0x3E) at (0, 75)
	_draw_actor(target, 62, Vector2i(0, 75))

## Exact PS1 POLY_G4 case 2 gradient base panel (cal_base_show in hud_screens.c:640, OT 5).
func draw_cal_base(target: Image, rect: Rect2i) -> void:
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
			var base_v: float = 224.0 if (fx + fy <= 1.0) else (224.0 - (fx + fy - 1.0) * 45.0)
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var v5: int = clampi((int(base_v) + d) >> 3, 0, 31)
			var v8: int = v5 << 3
			target.set_pixelv(Vector2i(px, py), Color8(v8, v8, v8, 255))

## Date pillar wooden frame (Frame 60, OT 5..4) with year, month, day, and weekday kanji.
func draw_date_pillar(target: Image, year: int, month: int, day: int, weekday: int, day_kind: int = 0, y_offset: int = 0) -> void:
	_ensure_compositor_coverage()
	# OT 5: prim_gradient_rect(0x86, -0x4B + y_offset, 0x14, 0x72, 5, 0x30E00, 0) -> Rect2i(294, 45 + y_offset, 20, 114)
	_compositor._gradient(target, Rect2i(294, 45 + y_offset, 20, 114))
	# OT 4: Actor 16 Frame 60 (0x3C) at (144, -18 + y_offset)
	_draw_actor(target, 60, Vector2i(144, -18 + y_offset))

	# Year digits (Actors 17, 18)
	_draw_actor(target, ((year / 10) % 10) + 0x44, Vector2i(140, -65 + y_offset))
	_draw_actor(target, (year % 10) + 0x44, Vector2i(148, -65 + y_offset))

	# Month digit(s) (Actors 19, 20)
	if month >= 10:
		_draw_actor(target, ((month / 10) % 10) + 0x44, Vector2i(140, -34 + y_offset))
		_draw_actor(target, (month % 10) + 0x44, Vector2i(148, -34 + y_offset))
	else:
		_draw_actor(target, month + 0x44, Vector2i(144, -34 + y_offset))

	# Day digit(s) (Actors 21, 22)
	if day >= 10:
		_draw_actor(target, ((day / 10) % 10) + 0x44, Vector2i(140, -1 + y_offset))
		_draw_actor(target, (day % 10) + 0x44, Vector2i(148, -1 + y_offset))
	else:
		_draw_actor(target, day + 0x44, Vector2i(144, -1 + y_offset))

	# Weekday kanji (Actor 23: frame weekday + 0x52)
	var var_name := "hud-44" if (weekday == 6 or day_kind in [1, 2]) else ("hud-43" if weekday == 5 else "base")
	_draw_actor(target, weekday + 0x52, Vector2i(146, 31 + y_offset), var_name)

## Top-left wooden title badge matching message_window_title_update (OT 2).
func draw_title_badge(target: Image, title_hex: String, origin: Vector2i = Vector2i.ZERO, n_glyphs: int = 0) -> void:
	_ensure_compositor_coverage()
	if n_glyphs <= 0:
		n_glyphs = title_hex.length() / 4
	var gx: int = clampi(160 - 144 + origin.x, 0, 320)
	var gy: int = clampi(120 - 90 + origin.y, 0, 240)
	var rx: int = clampi(160 - 144 + origin.x + n_glyphs * 14 + 8, 0, 320)
	var ry: int = clampi(120 - 90 + origin.y + 20, 0, 240)
	if rx > gx and ry > gy:
		_compositor._gradient(target, Rect2i(gx, gy, rx - gx, ry - gy))

	# OT 1: Badge frame pieces (Frames 113..115)
	for i in range(n_glyphs + 1, -1, -1):
		var fid: int = 113 if i == n_glyphs + 1 else (115 if i == 0 else 114)
		var ax: int = origin.x + (n_glyphs - i) * 14 - 135 + (2 if i == n_glyphs + 1 else (-2 if i == 0 else 0))
		var ay: int = origin.y - 81
		_draw_actor(target, fid, Vector2i(ax, ay))

	# OT 0: Title text
	_draw_text_hex_checked(target, title_hex, origin + Vector2i(18, 32), 0, "title_badge",
		Rect2i(gx, gy, rx - gx, ry - gy))

## Authentic PS1 hand cursor (Frame 156, OT 0 frontmost).
func draw_hand_cursor(target: Image, screen_pos: Vector2i) -> void:
	_draw_actor(target, 156, Vector2i(screen_pos.x - 160, screen_pos.y - 120))

## Exact menu_bar_show(0/1/2) semi-transparent highlight bar (menu_speaker_girl.c:461-487).
## Evaluates exact POLY_G4 case 0 vertex color scaling and 50/50 blend (abr = 0).
func draw_menu_bar_highlight(target: Image, rect: Rect2i, style_idx: int = 0) -> void:
	var style_val: int = MENU_BAR_STYLES[style_idx % MENU_BAR_STYLES.size()]
	var r: int = (style_val >> 16) & 0xFF
	var g: int = (style_val >> 8) & 0xFF
	var b: int = style_val & 0xFF

	# PS1 POLY_G4 case 0 vertex colors (gpu_primitives.c:231-237):
	# v0: *10, v1: *8, v2: *6, v3: *4 (wrapped mod 256)
	var v0_r: float = float((r * 10) & 0xFF)
	var v0_g: float = float((g * 10) & 0xFF)
	var v0_b: float = float((b * 10) & 0xFF)

	var v1_r: float = float((r * 8) & 0xFF)
	var v1_g: float = float((g * 8) & 0xFF)
	var v1_b: float = float((b * 8) & 0xFF)

	var v2_r: float = float((r * 6) & 0xFF)
	var v2_g: float = float((g * 6) & 0xFF)
	var v2_b: float = float((b * 6) & 0xFF)

	var v3_r: float = float((r * 4) & 0xFF)
	var v3_g: float = float((g * 4) & 0xFF)
	var v3_b: float = float((b * 4) & 0xFF)

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

			var fr_r: float
			var fr_g: float
			var fr_b: float
			if fx + fy <= 1.0:
				fr_r = v0_r + fx * (v1_r - v0_r) + fy * (v2_r - v0_r)
				fr_g = v0_g + fx * (v1_g - v0_g) + fy * (v2_g - v0_g)
				fr_b = v0_b + fx * (v1_b - v0_b) + fy * (v2_b - v0_b)
			else:
				var u: float = 1.0 - fx
				var v: float = 1.0 - fy
				fr_r = v3_r + u * (v2_r - v3_r) + v * (v1_r - v3_r)
				fr_g = v3_g + u * (v2_g - v3_g) + v * (v1_g - v3_g)
				fr_b = v3_b + u * (v2_b - v3_b) + v * (v1_b - v3_b)

			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var f_r5: int = clampi((int(fr_r) + d) >> 3, 0, 31)
			var f_g5: int = clampi((int(fr_g) + d) >> 3, 0, 31)
			var f_b5: int = clampi((int(fr_b) + d) >> 3, 0, 31)

			var back: Color = target.get_pixel(px, py)
			# PS1 abr=0 (50% Back + 50% Front) in 5-bit color space
			var out_r5: int = clampi((back.r8 >> 4) + (f_r5 >> 1), 0, 31)
			var out_g5: int = clampi((back.g8 >> 4) + (f_g5 >> 1), 0, 31)
			var out_b5: int = clampi((back.b8 >> 4) + (f_b5 >> 1), 0, 31)

			target.set_pixelv(Vector2i(px, py), Color8(out_r5 << 3, out_g5 << 3, out_b5 << 3, 255))

# ==============================================================================
# VN Dialogue Presentation Primitive
# ==============================================================================

## Unified VN Dialogue renderer supporting both:
## - sndisp layout: Y = 170, 186, 202 (is_vn_layout = false)
## - talk_render layout: Y = 168, 184, 200 (is_vn_layout = true)
## With text_shadow_palette_0/1 shadow suppression and Actor 2 Frame 0xF0 wait marker at (276, 192 + marker_bob_y).
func draw_vn_dialogue(target: Image, speaker_hex: String, body_lines_hex: Array[String],
		is_vn_layout: bool = false, marker_bob_y: int = -1) -> void:
	var y_slots: Array = [168, 184, 200] if is_vn_layout else [170, 186, 202]

	# 1. Render speaker label if provided
	if not speaker_hex.is_empty():
		var glyph_count: int = speaker_hex.length() / 4
		var speaker_x: int = 39 # default 2-glyph (福原, 藤崎)
		if glyph_count == 1:
			speaker_x = 46
		elif glyph_count >= 3:
			speaker_x = 32
		_draw_text_hex_checked(target, speaker_hex, Vector2i(speaker_x, y_slots[0]), 0, "message_window")

	# 2. Render up to 3 body dialogue lines
	for row_idx in range(mini(3, body_lines_hex.size())):
		var line_hex: String = body_lines_hex[row_idx]
		if line_hex.is_empty():
			continue
		var y_pos: int = y_slots[row_idx]
		# If speaker is present on row 0, row 0 starts at x=88 (indented)
		# Subsequent rows also indent at x=88; if no speaker, starts at x=40 (or x=74 if quote-wrapped)
		var x_pos: int = 88 if not speaker_hex.is_empty() else (74 if line_hex.begins_with("8175") or line_hex.begins_with("8177") else 40)
		_draw_text_hex_checked(target, line_hex, Vector2i(x_pos, y_pos), 0, "message_window")

	# 3. Actor 2 Frame 0xF0 wait marker at (276, 192 + bob)
	if marker_bob_y >= 0:
		var marker_screen := Vector2i(276, 192 + marker_bob_y)
		# Attempt actor blit via compositor
		_draw_actor(target, 0xF0, Vector2i(marker_screen.x - 160, marker_screen.y - 120))
		# Authentic PS1 prompt marker fallback glyph: 81a5 (▼)
		_draw_text_hex_checked(target, "81a5", marker_screen, 0, "message_window")

# ==============================================================================
# Helper Drawing Routines
# ==============================================================================

func _draw_actor(target: Image, frame_id: int, pos: Vector2i, variant: String = "base", semi: bool = true) -> void:
	if _compositor == null or not _compositor.descriptor.has("frames"):
		return
	var actor := {
		"kind": "actor",
		"id": 96,
		"priority": 0,
		"frame": frame_id,
		"position": pos,
		"variant": variant,
		"semi": semi,
	}
	_compositor._draw_actor(target, actor)

func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i, palette_row: int = 0,
		region: String = "", custom_bounds: Rect2i = Rect2i()) -> void:
	var codes: PackedByteArray = hex_code.hex_decode()
	var x_offset: int = 0

	for i in range(0, codes.size(), 2):
		if i + 1 >= codes.size():
			break
		var code_unit := codes.slice(i, i + 2)
		var glyph_res: Dictionary = _font.get_glyph_image(code_unit, palette_row & 7, (palette_row >> 3) & 1)
		var gx_pos: int = pos.x + x_offset
		var gy_pos: int = pos.y

		recorded_glyphs.append({
			"code": code_unit.hex_encode(),
			"pos": Vector2i(gx_pos, gy_pos),
			"region": region,
		})

		# Enforce bounding box assertions per Section 12a
		if region == "message_window":
			if gx_pos < 27 or gx_pos + 12 > 293 or gy_pos < 166 or gy_pos + 12 > 218:
				bounding_box_violations.append(
					"Message window glyph %s at (%d, %d) overflows Rect2i(27, 166, 266, 52)" % [
						code_unit.hex_encode(), gx_pos, gy_pos
					]
				)
		elif region == "title_badge" and custom_bounds.size.x > 0:
			if gx_pos < custom_bounds.position.x or gx_pos + 12 > custom_bounds.position.x + custom_bounds.size.x or gy_pos < custom_bounds.position.y or gy_pos + 12 > custom_bounds.position.y + custom_bounds.size.y:
				bounding_box_violations.append(
					"Title badge glyph %s at (%d, %d) overflows %s" % [
						code_unit.hex_encode(), gx_pos, gy_pos, str(custom_bounds)
					]
				)

		if glyph_res.get("status") == "glyph":
			var g_img: Image = glyph_res["image"]
			# text_shadow_palette_0/1 suppression for attributes 3, 4, 5, 14, 15 (0x0000 transparent)
			var has_shadow: bool = not (palette_row in [3, 4, 5, 14, 15])
			if has_shadow:
				for gy in range(12):
					var py: int = gy_pos + gy + 1
					if py < 0 or py >= 240:
						continue
					for gx in range(12):
						var px: int = gx_pos + gx + 1
						if px < 0 or px >= 320:
							continue
						if g_img.get_pixel(gx, gy).a > 0:
							target.set_pixelv(Vector2i(px, py), Color8(0, 0, 0, 255))
			for gy in range(12):
				var py: int = gy_pos + gy
				if py < 0 or py >= 240:
					continue
				for gx in range(12):
					var px: int = gx_pos + gx
					if px < 0 or px >= 320:
						continue
					if g_img.get_pixel(gx, gy).a > 0:
						target.set_pixelv(Vector2i(px, py), g_img.get_pixel(gx, gy))

		x_offset += 14
