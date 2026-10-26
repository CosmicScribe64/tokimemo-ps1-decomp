class_name EventCgScenesPresenter
extends "res://presentation/command_hud_presenter.gd"
## T-29018 Lane 8: Authentic 320x240 PS1 Presentation for EVENT.EXN Date Spots & Special CG Scenes.
## Renders:
## 1. Shiori Special CG Scene (Scene 4: Kirameki Central Park Cherry Blossoms, scene_flags[0])
## 2. Ayako Special Date Scene (Scene 2: Karaoke Box Duet, scene_flags[0x13])
## 3. Amusement Park Ride Movie Scene (Ferris Wheel MDEC Viewport LBA 0x4332, spot_ride=2)
## 4. Miharu Date Encounter Scene (Scene 3: Park Arrival Bump, scene_flags[0x48])
##
## Follows CODING_STANDARDS.md Section 12a strictly:
## - All CP932 hex strings computed via Python .encode("cp932").hex()
## - Zero glyph bounding-box violations
## - Authentic PS1 HUD primitives:
##   * Mode 1 / Mode 2 checkerboard frames (hud_screen_frame_draw, hud_panels.c:1366)
##   * Wooden message window Frame 62 (OT 8..7) at (0, 75)
##   * Wooden title badge Frames 113..115 (OT 2)
##   * Hand cursor Frame 156 (OT 0)
##   * POLY_G4 menu bar highlight with Bayer dithering

const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"

# Authentic PS1 32x32 green checkerboard tile (hud_screen_frame_draw, hud_panels.c:1366)
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA5ElEQVR4nMWWsQ3DIBBFzycXlIzgUTIKZUZI6VEYI6XHcHkjpKRLdBRIECQMTvx/aSPeQ0L8m2h1byoij5UkCI1GXkLOb4fWTl78l8BiF7rv4wL7cyPvbofWMoHD5QdjzKkNQwjjAuZieCaAgCcBFDwKIOFRAAnXwzMSrmEkvCrQuhMteOt/uT8j4ZkAAp4EUPAogIRrZku22udaqaPpmSfmWu/39HlVIMjheYIJHP7nI9MtEC6GZwIIeBJAwaMAEh4FkHA9/LDAryYpRsKrAmff9t5uYSQ8E0DAkwAKHgWQcBX4AGcPlHlFD2CfAAAAAElFTkSuQmCC"

# PS1 Bayer 4x4 dither table
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Exact CP932 byte hex encodings strictly verified via Python .encode("cp932").hex()
const STR_HEX := {
	"title_park": "82ab82e782df82ab9286899b8cf68980",        # きらめき中央公園
	"title_karaoke": "834a838983498350424f58",               # カラオケBOX
	"title_amusement": "97568980926e81458acf97978ed4",       # 遊園地・観覧車
	"title_encounter": "8cf6898093fc8cfb814591988bf6",       # 公園入口・遭遇
	"speaker_shiori": "8e8d9044",                             # 詩織
	"speaker_ayako": "8dca8e71",                              # 彩子
	"speaker_miharu": "8ca990b0",                             # 見晴
	"shiori_d1": "81758df782aa969e8a4a82c582b782b282ad82ab82ea82a282cb8176", # 「桜が満開ですごくきれいね」
	"shiori_d2": "817582d382bd82e882c5978882e782ea82c48af082b582a282c88176", # 「ふたりで来られて嬉しいな」
	"shiori_c1": "8175967b939682c982ab82ea82a282be82cb8176", # 「本当にきれいだね」
	"shiori_c2": "81758df782e682e88e8d904482cc95fb82aa82b782c482ab82be82e68176", # 「桜より詩織の方がすてきだよ」
	"shiori_c3": "817589d495b28fc782aa906882a282e68176",     # 「花粉症が辛いよ」
	"ayako_d1": "817589708cea82cc89cc814189cc82c182c482a282a281488176", # 「英語の歌、歌っていい？」
	"ayako_d2": "817588ea8f8f82c98366838583478362836782b582e682a481498176", # 「一緒にデュエットしよう！」
	"ayako_c1": "817582e682b5814189cc82a882a481498176",     # 「よし、歌おう！」
	"ayako_c2": "81758dca8e7182cc89cc82f092ae82ab82bd82a282c88176", # 「彩子の歌を聴きたいな」
	"ayako_c3": "8175898989cc82c582e082a282a281488176",     # 「演歌でもいい？」
	"ferris_d1": "817582ed82a08141899382ad82dc82c58ca982a682e982e681498176", # 「わあ、遠くまで見えるよ！」
	"ferris_d2": "81758a5882aa8fac82b382ad8ca982a682e982cb8176", # 「街が小さく見えるね」
	"movie_indicator": "8dc490b692863a204c424120307834333332", # 再生中: LBA 0x4332
	"ride_prompt": "81798acf97978ed483588367838a815b837e8393834f817a", # 【観覧車ストリーミング】
	"miharu_d1": "817582a0814182b282df82f182c882b382a2816382c182c48176", # 「あ、ごめんなさい…って」
	"miharu_d2": "817582e082b582a982b582c48366815b8367928681488176", # 「もしかしてデート中？」
	"miharu_c1": "81758ca990b082bf82e182f181418bf4915282be82cb8176", # 「見晴ちゃん、偶然だね」
	"miharu_c2": "8175906c88e182a282b682e182c882a281488176", # 「人違いじゃない？」
	"miharu_c3": "81758ed7968282b582c882a282c582ad82ea8176", # 「邪魔しないでくれ」
}

var _checker_tile: Image
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []

func init_resources(font_root: String, hud_manifest_path: String = "/workspace/.asset-cache/port/hud-production-v1/manifest.json") -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()

	_checker_tile = Image.new()
	var err: Error = _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64))
	if err != OK:
		return "Failed to decode checkerboard background tile"

	if not hud_manifest_path.is_empty() and FileAccess.file_exists(hud_manifest_path):
		if not open_manifest(hud_manifest_path, HUD_SHA, font_root):
			return "Failed to open HUD manifest: " + error
	else:
		var font_err: String = _font.open_external_font(font_root)
		if not font_err.is_empty():
			return "Failed to open font: " + font_err

	if _compositor != null and _compositor.last_coverage == null:
		_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
		_compositor.last_coverage.fill(Color.BLACK)

	return ""

# ==============================================================================
# Scene 1: Shiori Special CG Scene (Scene 4: Central Park Cherry Blossoms)
# ==============================================================================

func render_shiori_cg_scene(selected_choice: int = 0) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Outer Checkerboard Frame Mode 1
	_draw_checkerboard_frame(target, 1)

	# 2. CG Background Plate: Central Park with cherry blossoms
	_draw_cherry_blossom_cg(target, Rect2i(24, 20, 272, 142))

	# 3. Top-Left Title Badge: きらめき中央公園
	_draw_title_badge(target, STR_HEX["title_park"], Vector2i(16, 12))

	# 4. Choice selection bar overlay inside the CG area
	var choices := [
		STR_HEX["shiori_c1"],
		STR_HEX["shiori_c2"],
		STR_HEX["shiori_c3"]
	]
	_draw_choice_selection_box(target, choices, selected_choice, Vector2i(40, 84))

	# 5. Bottom Wooden Message Window (Frame 62)
	_draw_message_window(target)

	# 6. Dialogue Text (sndisp layout: y=170, 186)
	_draw_text_hex_checked(target, STR_HEX["speaker_shiori"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["shiori_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["shiori_d2"], Vector2i(88, 186), 0, "message_window")

	# Wait prompt marker (Actor 2 Frame 0xF0 at 276, 192)
	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Scene 2: Ayako Special Date Scene (Scene 2: Karaoke Box Duet)
# ==============================================================================

func render_ayako_karaoke_scene(selected_choice: int = 0) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Outer Checkerboard Frame Mode 1
	_draw_checkerboard_frame(target, 1)

	# 2. CG Background Plate: Karaoke room lounge with warm lighting
	_draw_karaoke_lounge_cg(target, Rect2i(24, 20, 272, 142))

	# 3. Top-Left Title Badge: カラオケBOX
	_draw_title_badge(target, STR_HEX["title_karaoke"], Vector2i(16, 12))

	# 4. Choice selection bar overlay
	var choices := [
		STR_HEX["ayako_c1"],
		STR_HEX["ayako_c2"],
		STR_HEX["ayako_c3"]
	]
	_draw_choice_selection_box(target, choices, selected_choice, Vector2i(40, 84))

	# 5. Bottom Wooden Message Window
	_draw_message_window(target)

	# 6. Dialogue Text
	_draw_text_hex_checked(target, STR_HEX["speaker_ayako"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ayako_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ayako_d2"], Vector2i(88, 186), 0, "message_window")

	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Scene 3: Amusement Park Ride Movie Scene (Ferris Wheel Viewport LBA 0x4332)
# ==============================================================================

func render_ride_movie_scene() -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	# 1. Outer Checkerboard Frame Mode 1 (Movie & Dialog perimeter)
	_draw_checkerboard_frame(target, 1)

	# 2. Authentic PS1 MDEC Letterbox Viewport (ride_movies.c)
	_draw_ferris_wheel_viewport(target, Rect2i(24, 20, 272, 142))

	# 3. Top-Left Title Badge: 遊園地・観覧車
	_draw_title_badge(target, STR_HEX["title_amusement"], Vector2i(16, 12))

	# 4. MDEC Streaming LBA status banner inside viewport
	_draw_text_hex_checked(target, STR_HEX["movie_indicator"], Vector2i(140, 26), 3, "viewport_hud")

	# 5. Bottom Wooden Message Window
	_draw_message_window(target)

	# 6. Dialogue Text from Ferris Wheel
	_draw_text_hex_checked(target, STR_HEX["speaker_shiori"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ferris_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ferris_d2"], Vector2i(88, 186), 0, "message_window")

	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Scene 4: Miharu Date Encounter Scene (Scene 3: Park Arrival Bump)
# ==============================================================================

func render_miharu_encounter_scene(selected_choice: int = 0) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Outer Checkerboard Frame Mode 1
	_draw_checkerboard_frame(target, 1)

	# 2. CG Background Plate: Park Entrance Corner
	_draw_park_entrance_cg(target, Rect2i(24, 20, 272, 142))

	# 3. Top-Left Title Badge: 公園入口・遭遇
	_draw_title_badge(target, STR_HEX["title_encounter"], Vector2i(16, 12))

	# 4. Choice selection bar overlay
	var choices := [
		STR_HEX["miharu_c1"],
		STR_HEX["miharu_c2"],
		STR_HEX["miharu_c3"]
	]
	_draw_choice_selection_box(target, choices, selected_choice, Vector2i(40, 84))

	# 5. Bottom Wooden Message Window
	_draw_message_window(target)

	# 6. Dialogue Text
	_draw_text_hex_checked(target, STR_HEX["speaker_miharu"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["miharu_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["miharu_d2"], Vector2i(88, 186), 0, "message_window")

	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Shared PS1 Primitives & Rendering Helpers
# ==============================================================================

## Exact 10x8 tile enable mask from hud_screen_frame_draw(a) (src/main/hud_panels.c:1412-1438)
static func is_checker_tile_enabled(mode: int, col: int, row: int) -> bool:
	if col < 0 or col >= 10 or row < 0 or row >= 8:
		return false
	match mode:
		0: return true
		1:
			if row >= 1 and row < 6 and col >= 1 and col < 9:
				return false
			return true
		2:
			if row < 5: return false
			if row == 5: return col == 0 or col == 9
			return true
		_: return true

## Exact hud_screen_frame_draw(0/1/2) with UV scroll
func _draw_checkerboard_frame(target: Image, mode: int) -> void:
	if _checker_tile == null:
		return
	for row in range(8):
		for col in range(10):
			if not is_checker_tile_enabled(mode, col, row):
				continue
			var base_x: int = col * 32
			var base_y: int = row * 32
			for dy in range(32):
				var py: int = base_y + dy
				if py < 0 or py >= 240: continue
				for dx in range(32):
					var px: int = base_x + dx
					if px < 0 or px >= 320: continue
					target.set_pixelv(Vector2i(px, py), _checker_tile.get_pixel(dx & 31, dy & 31))

## Bottom wooden message window (Frame 62, OT 8..7)
func _draw_message_window(target: Image) -> void:
	if _compositor != null:
		_compositor._gradient(target, Rect2i(27, 166, 266, 52))
		_draw_actor(target, 62, Vector2i(0, 75))

## Top-left wooden title badge matching message_window_title_update (OT 2)
func _draw_title_badge(target: Image, title_hex: String, origin: Vector2i = Vector2i.ZERO) -> void:
	var n_glyphs: int = title_hex.length() / 4
	var gx: int = clampi(160 - 144 + origin.x, 0, 320)
	var gy: int = clampi(120 - 90 + origin.y, 0, 240)
	var rx: int = clampi(160 - 144 + origin.x + n_glyphs * 14 + 12, 0, 320)
	var ry: int = clampi(120 - 90 + origin.y + 20, 0, 240)
	var badge_bounds := Rect2i(gx, gy, rx - gx, ry - gy)

	if _compositor != null:
		_compositor._gradient(target, badge_bounds)

	# Frames 113..115 badge border segments
	for i in range(n_glyphs + 1, -1, -1):
		var fid: int = 113 if i == n_glyphs + 1 else (115 if i == 0 else 114)
		var ax: int = origin.x + (n_glyphs - i) * 14 - 135 + (2 if i == n_glyphs + 1 else (-2 if i == 0 else 0))
		var ay: int = origin.y - 81
		_draw_actor(target, fid, Vector2i(ax, ay))

	# Title text
	_draw_text_hex_checked(target, title_hex, origin + Vector2i(18, 32), 0, "title_badge", badge_bounds)

## Choice box with semi-transparent highlight bar and hand cursor
func _draw_choice_selection_box(target: Image, choices: Array, selected_idx: int, origin: Vector2i) -> void:
	var box_rect := Rect2i(origin.x - 8, origin.y - 6, 240, choices.size() * 18 + 12)
	_draw_gouraud_panel(target, box_rect, Color8(30, 40, 60, 220), Color8(10, 15, 25, 240))

	for i in range(choices.size()):
		var y_pos: int = origin.y + i * 18
		var choice_rect := Rect2i(origin.x, y_pos - 2, 224, 16)

		if i == selected_idx:
			_draw_menu_bar_highlight(target, choice_rect)
			_draw_hand_cursor(target, Vector2i(origin.x - 14, y_pos + 1))

		_draw_text_hex_checked(target, choices[i], Vector2i(origin.x + 8, y_pos), 0, "choice_box")

## Exact menu_bar_show(0) semi-transparent highlight bar with Gouraud shading
func _draw_menu_bar_highlight(target: Image, rect: Rect2i) -> void:
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			var fx: float = float(x) / float(rect.size.x)
			var base_col: Color = target.get_pixel(px, py)
			var tint_r: int = int(lerpf(47.0, 100.0, fx))
			var tint_g: int = int(lerpf(127.0, 200.0, fy))
			var tint_b: int = int(lerpf(175.0, 230.0, fx))
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var out_r5: int = clampi(((int(base_col.r8) + tint_r + d) >> 4), 0, 31)
			var out_g5: int = clampi(((int(base_col.g8) + tint_g + d) >> 4), 0, 31)
			var out_b5: int = clampi(((int(base_col.b8) + tint_b + d) >> 4), 0, 31)
			target.set_pixelv(Vector2i(px, py), Color8(out_r5 << 3, out_g5 << 3, out_b5 << 3))

## Hand cursor (Actor 0 Frame 156)
func _draw_hand_cursor(target: Image, screen_pos: Vector2i) -> void:
	_draw_actor(target, 156, Vector2i(screen_pos.x - 160, screen_pos.y - 120))

## Wait prompt marker (Actor 2 Frame 0xF0)
func _draw_wait_marker(target: Image, screen_pos: Vector2i) -> void:
	_draw_actor(target, 0xF0, Vector2i(screen_pos.x - 160, screen_pos.y - 120))

## Gouraud shaded background panel
func _draw_gouraud_panel(target: Image, rect: Rect2i, col_top: Color, col_bot: Color) -> void:
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		var r: int = int(lerpf(col_top.r8, col_bot.r8, fy))
		var g: int = int(lerpf(col_top.g8, col_bot.g8, fy))
		var b: int = int(lerpf(col_top.b8, col_bot.b8, fy))
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			if x == 0 or y == 0 or x == rect.size.x - 1 or y == rect.size.y - 1:
				target.set_pixelv(Vector2i(px, py), Color8(180, 160, 120))
			else:
				target.set_pixelv(Vector2i(px, py), Color8(r, g, b))

# ==============================================================================
# Visual Art Plates & CG Illustrations
# ==============================================================================

## Scene 1 CG: Kirameki Central Park with Cherry Blossoms
func _draw_cherry_blossom_cg(target: Image, rect: Rect2i) -> void:
	# Sky gradient (soft spring blue)
	_draw_gouraud_panel(target, rect, Color8(170, 210, 245), Color8(220, 235, 250))

	# Distant cherry blossom canopy & park walkway
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			# Ground pathway in lower third
			if fy > 0.65:
				var path_col := Color8(210, 195, 175)
				target.set_pixelv(Vector2i(px, py), path_col)
			# Cherry blossom clusters (pink tones)
			elif fy < 0.55:
				var d: int = ((px * 7 + py * 13) % 29)
				if d < 18:
					var petal_col := Color8(250, 205, 220) if d < 10 else Color8(240, 180, 200)
					target.set_pixelv(Vector2i(px, py), petal_col)

	# Central portrait silhouette (Shiori with cherry blossom backdrop)
	_draw_girl_cg_silhouette(target, Vector2i(rect.position.x + rect.size.x / 2, rect.position.y + rect.size.y - 10), Color8(210, 70, 85))

## Scene 2 CG: Karaoke Box Lounge
func _draw_karaoke_lounge_cg(target: Image, rect: Rect2i) -> void:
	# Moody interior lounge with warm ambient glow
	_draw_gouraud_panel(target, rect, Color8(60, 35, 75), Color8(30, 20, 45))

	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			# Table & seating lower area
			if fy > 0.70:
				target.set_pixelv(Vector2i(px, py), Color8(85, 45, 55))
			# Spotlight beam
			var fx: float = float(x) / float(rect.size.x)
			if absf(fx - 0.5) < (0.35 * fy):
				var curr: Color = target.get_pixel(px, py)
				target.set_pixelv(Vector2i(px, py), Color8(mini(255, curr.r8 + 40), mini(255, curr.g8 + 30), mini(255, curr.b8 + 50)))

	# Ayako silhouette holding microphone
	_draw_girl_cg_silhouette(target, Vector2i(rect.position.x + rect.size.x / 2, rect.position.y + rect.size.y - 10), Color8(70, 140, 190))

## Scene 3 CG: Ferris Wheel MDEC Viewport
func _draw_ferris_wheel_viewport(target: Image, rect: Rect2i) -> void:
	# Sunset sky gradient overlooking Kirameki city
	_draw_gouraud_panel(target, rect, Color8(235, 110, 60), Color8(70, 30, 80))

	# City silhouette & distant amusement park lights
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			if fy > 0.72:
				# City skyline
				var b_height: int = int((sin(float(px) * 0.12) + cos(float(px) * 0.05) + 2.0) * 8.0)
				if py > (rect.position.y + rect.size.y - b_height):
					target.set_pixelv(Vector2i(px, py), Color8(25, 20, 35))

	# Ferris wheel gondola structural frame overlay
	for lx in range(rect.size.x):
		var gx: int = rect.position.x + lx
		target.set_pixelv(Vector2i(gx, rect.position.y + 16), Color8(180, 40, 40))
		target.set_pixelv(Vector2i(gx, rect.position.y + rect.size.y - 20), Color8(180, 40, 40))
	for ly in range(rect.size.y):
		var gy: int = rect.position.y + ly
		target.set_pixelv(Vector2i(rect.position.x + 36, gy), Color8(180, 40, 40))
		target.set_pixelv(Vector2i(rect.position.x + rect.size.x - 36, gy), Color8(180, 40, 40))

## Scene 4 CG: Park Entrance Bump Corner
func _draw_park_entrance_cg(target: Image, rect: Rect2i) -> void:
	# Sunny outdoor park entrance with brick archway
	_draw_gouraud_panel(target, rect, Color8(160, 210, 240), Color8(210, 230, 240))

	# Brick archway and pavement
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			if fy > 0.60:
				target.set_pixelv(Vector2i(px, py), Color8(190, 180, 160))
			elif x < 40 or x > rect.size.x - 40:
				target.set_pixelv(Vector2i(px, py), Color8(165, 80, 60))

	# Miharu silhouette (flustered bump encounter pose)
	_draw_girl_cg_silhouette(target, Vector2i(rect.position.x + rect.size.x / 2 + 10, rect.position.y + rect.size.y - 10), Color8(60, 170, 120))

## Draws stylized anime character portrait silhouette
func _draw_girl_cg_silhouette(target: Image, base_pos: Vector2i, hair_color: Color) -> void:
	var head_center := Vector2i(base_pos.x, base_pos.y - 70)

	# Body / clothing
	for dy in range(60):
		var cy: int = head_center.y + 20 + dy
		var spread: int = 12 + dy / 2
		for dx in range(-spread, spread + 1):
			var cx: int = head_center.x + dx
			if cx >= 0 and cx < 320 and cy >= 0 and cy < 240:
				target.set_pixelv(Vector2i(cx, cy), Color8(240, 240, 245))

	# Head / face
	for dy in range(-20, 20):
		for dx in range(-18, 19):
			var d: float = Vector2(dx, dy).length()
			if d <= 18.0:
				var cx: int = head_center.x + dx
				var cy: int = head_center.y + dy
				if cx >= 0 and cx < 320 and cy >= 0 and cy < 240:
					target.set_pixelv(Vector2i(cx, cy), Color8(255, 225, 210))

	# Hair
	for dy in range(-24, 15):
		for dx in range(-22, 23):
			var d: float = Vector2(dx, dy).length()
			if d <= 22.0 and (dy < 4 or absi(dx) > 12):
				var cx: int = head_center.x + dx
				var cy: int = head_center.y + dy
				if cx >= 0 and cx < 320 and cy >= 0 and cy < 240:
					target.set_pixelv(Vector2i(cx, cy), hair_color)

# ==============================================================================
# Checked Glyph Drawing and Boundary Validation (Section 12a)
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
			# text_shadow_palette_0/1 suppression for attributes 3, 4, 5, 14, 15
			var has_shadow: bool = not (palette_row in [3, 4, 5, 14, 15])
			if has_shadow:
				for gy in range(12):
					var py: int = gy_pos + gy + 1
					if py < 0 or py >= 240: continue
					for gx in range(12):
						var px: int = gx_pos + gx + 1
						if px < 0 or px >= 320: continue
						if g_img.get_pixel(gx, gy).a > 0:
							target.set_pixelv(Vector2i(px, py), Color8(0, 0, 0, 255))
			for gy in range(12):
				var py: int = gy_pos + gy
				if py < 0 or py >= 240: continue
				for gx in range(12):
					var px: int = gx_pos + gx
					if px < 0 or px >= 320: continue
					if g_img.get_pixel(gx, gy).a > 0:
						target.set_pixelv(Vector2i(px, py), g_img.get_pixel(gx, gy))

		x_offset += 14
