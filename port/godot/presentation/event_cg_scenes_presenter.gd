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
	"title_karaoke": "834a8389834983508261826e8277",         # カラオケＢＯＸ
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
	"ferris_c1": "817582b782b282ad8a7982b582a982c182bd82cb8176", # 「すごく楽しかったね」
	"ferris_c2": "817582bf82e582c182c68368834c8368834c82b582bd82e68176", # 「ちょっとドキドキしたよ」
	"ferris_c3": "817582e082a48fe682e982cc82cd82e282df82e682a481638176", # 「もう乗るのはやめよう…」
	"movie_indicator": "8dc490b6928681468acf97978ed4",         # 再生中：観覧車
	"ride_prompt": "81798acf97978ed483588367838a815b837e8393834f817a", # 【観覧車ストリーミング】
	"miharu_d1": "817582a0814182b282df82f182c882b382a2816382c182c48176", # 「あ、ごめんなさい…って」
	"miharu_d2": "817582e082b582a982b582c48366815b8367928681488176", # 「もしかしてデート中？」
	"miharu_c1": "81758ca990b082bf82e182f181418bf4915282be82cb8176", # 「見晴ちゃん、偶然だね」
	"miharu_c2": "8175906c88e182a282b682e182c882a281488176", # 「人違いじゃない？」
	"miharu_c3": "81758ed7968282b582c882a282c582ad82ea8176", # 「邪魔しないでくれ」
}

var default_choice: int = 0
var _checker_tile: Image
var _park_bg_image: Image
var _karaoke_bg_image: Image
var _ferris_bg_image: Image
var _park_entrance_bg_image: Image
var _shiori_portrait_image: Image
var _ayako_portrait_image: Image
var _miharu_portrait_image: Image
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []

static func _load_first_existing_image(rel_path: String) -> Image:
	var candidates := [
		"/workspace/" + rel_path,
		"/work/" + rel_path,
		ProjectSettings.globalize_path("res://../../" + rel_path).simplify_path(),
	]
	for p in candidates:
		if FileAccess.file_exists(p):
			return Image.load_from_file(p)
	return null

func init_resources(font_root: String, hud_manifest_path: String = "/workspace/.asset-cache/port/hud-production-v1/manifest.json") -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()

	_checker_tile = Image.new()
	var err: Error = _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64))
	if err != OK:
		return "Failed to decode checkerboard background tile"

	_park_bg_image = _load_first_existing_image(".asset-cache/port/date-preview-v1/backgrounds/park_17112.background.png")
	_karaoke_bg_image = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-95c9c3981048bc82948aa0156f9f889627102ac0dce9c450798788d2f943130e/asset-95c9c3981048bc82948aa0156f9f889627102ac0dce9c450798788d2f943130e.background.png")
	_ferris_bg_image = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-934e76c18fc92b9885f85cfa63887b432433a926cdac676d5198e1af353c3159/asset-934e76c18fc92b9885f85cfa63887b432433a926cdac676d5198e1af353c3159.background.png")
	_park_entrance_bg_image = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-53ac5a6ef2afadb8782efe2f2d76f029400797732b3ce813fb631967bb3c2292/asset-53ac5a6ef2afadb8782efe2f2d76f029400797732b3ce813fb631967bb3c2292.background.png")

	_shiori_portrait_image = _load_first_existing_image(".asset-cache/port/date-preview-v1/portraits/shiori_hu10/shiori_hu10_21938.static-frame.png")
	_ayako_portrait_image = _load_first_existing_image(".asset-cache/media/color-corpus/assets/asset-d2142a796276bbacf540bc1aa9fd0b4ebd82421a91951f2951e908c867c93873/asset-d2142a796276bbacf540bc1aa9fd0b4ebd82421a91951f2951e908c867c93873.static-frame.png")
	_miharu_portrait_image = _load_first_existing_image(".asset-cache/media/color-corpus/assets/asset-6e1623b6f414f105a7f2f0d2c2adb6df9569076c3ceb84fd17ff4089fffc48ec/asset-6e1623b6f414f105a7f2f0d2c2adb6df9569076c3ceb84fd17ff4089fffc48ec.static-frame.png")

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
	var choice_idx: int = clampi(selected_choice, 0, 2)
	var choices := [STR_HEX["shiori_c1"], STR_HEX["shiori_c2"], STR_HEX["shiori_c3"]]

	# 1. Outer Checkerboard Frame Mode 2 (Spring month=4)
	_draw_checkerboard_frame(target, 2, 4, Vector2i(14, 8))

	# 2. Extracted 320x160 PS1 Background Plate + HU10 Shiori Portrait
	_draw_cherry_blossom_cg(target, Rect2i(0, 0, 320, 160))

	# 3. Top-Left Title Badge: きらめき中央公園
	_draw_title_badge(target, STR_HEX["title_park"], Vector2i.ZERO)

	# 3b. Interactive Choice Selection Box + Hand Cursor
	_draw_choice_selection_box(target, choices, choice_idx, Vector2i(52, 86))

	# 4. Bottom Wooden Message Window (Frame 62)
	_draw_message_window(target)

	# 5. Dialogue Text (sndisp layout: y=170, 186, 202)
	var choice_x: int = 74 if choice_idx == 1 else 88
	_draw_text_hex_checked(target, STR_HEX["speaker_shiori"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["shiori_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["shiori_d2"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, choices[choice_idx], Vector2i(choice_x, 202), 0, "message_window")

	# Wait prompt marker (Actor 2 Frame 1 at 276, 192)
	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Scene 2: Ayako Special Date Scene (Scene 2: Karaoke Box Duet)
# ==============================================================================

func render_ayako_karaoke_scene(selected_choice: int = 0) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var choice_idx: int = clampi(selected_choice, 0, 2)
	var choices := [STR_HEX["ayako_c1"], STR_HEX["ayako_c2"], STR_HEX["ayako_c3"]]

	# 1. Outer Checkerboard Frame Mode 2 (Summer month=7)
	_draw_checkerboard_frame(target, 2, 7, Vector2i(18, 12))

	# 2. Extracted 320x160 PS1 Background Plate + AS01 Ayako Portrait
	_draw_karaoke_lounge_cg(target, Rect2i(0, 0, 320, 160))

	# 3. Top-Left Title Badge: カラオケBOX
	_draw_title_badge(target, STR_HEX["title_karaoke"], Vector2i.ZERO)

	# 3b. Interactive Choice Selection Box + Hand Cursor
	_draw_choice_selection_box(target, choices, choice_idx, Vector2i(52, 86))

	# 4. Bottom Wooden Message Window
	_draw_message_window(target)

	# 5. Dialogue Text (sndisp layout: y=170, 186, 202)
	_draw_text_hex_checked(target, STR_HEX["speaker_ayako"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ayako_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ayako_d2"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, choices[choice_idx], Vector2i(88, 202), 0, "message_window")

	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Scene 3: Amusement Park Ride Movie Scene (Ferris Wheel Viewport LBA 0x4332)
# ==============================================================================

func render_ride_movie_scene(selected_choice: int = -1) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)
	var choice_idx: int = clampi(default_choice if selected_choice < 0 else selected_choice, 0, 2)
	var choices := [STR_HEX["ferris_c1"], STR_HEX["ferris_c2"], STR_HEX["ferris_c3"]]

	# 1. Outer Checkerboard Frame Mode 2 (Autumn month=10)
	_draw_checkerboard_frame(target, 2, 10, Vector2i(22, 14))

	# 2. Extracted 320x160 PS1 Ferris Wheel Gondola Background (I_11_01.BS) + Shiori Portrait
	_draw_ferris_wheel_viewport(target, Rect2i(0, 0, 320, 160))

	# 3. Top-Left Title Badge: 遊園地・観覧車
	_draw_title_badge(target, STR_HEX["title_amusement"], Vector2i.ZERO)

	# 3b. Interactive Choice Selection Box + Hand Cursor
	_draw_choice_selection_box(target, choices, choice_idx, Vector2i(52, 86))

	# 4. Bottom Wooden Message Window
	_draw_message_window(target)

	# 5. Dialogue Text from Ferris Wheel
	_draw_text_hex_checked(target, STR_HEX["speaker_shiori"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ferris_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["ferris_d2"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, choices[choice_idx], Vector2i(88, 202), 0, "message_window")

	_draw_wait_marker(target, Vector2i(276, 192))

	return target

# ==============================================================================
# Scene 4: Miharu Date Encounter Scene (Scene 3: Park Arrival Bump)
# ==============================================================================

func render_miharu_encounter_scene(selected_choice: int = 0) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var choice_idx: int = clampi(selected_choice, 0, 2)
	var choices := [STR_HEX["miharu_c1"], STR_HEX["miharu_c2"], STR_HEX["miharu_c3"]]

	# 1. Outer Checkerboard Frame Mode 2 (Winter month=1)
	_draw_checkerboard_frame(target, 2, 1, Vector2i(26, 18))

	# 2. Extracted 320x160 PS1 Park Entrance Background (D_24_03.BS) + TA10 Miharu Portrait
	_draw_park_entrance_cg(target, Rect2i(0, 0, 320, 160))

	# 3. Top-Left Title Badge: 公園入口・遭遇
	_draw_title_badge(target, STR_HEX["title_encounter"], Vector2i.ZERO)

	# 3b. Interactive Choice Selection Box + Hand Cursor
	_draw_choice_selection_box(target, choices, choice_idx, Vector2i(52, 86))

	# 4. Bottom Wooden Message Window
	_draw_message_window(target)

	# 5. Dialogue Text (sndisp layout: y=170, 186, 202)
	_draw_text_hex_checked(target, STR_HEX["speaker_miharu"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["miharu_d1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["miharu_d2"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, choices[choice_idx], Vector2i(88, 202), 0, "message_window")

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

## Exact hud_screen_frame_draw(0/1/2) with UV scroll and 4-season CLUT
func _draw_checkerboard_frame(target: Image, mode: int, month: int = 4, bg_uv: Vector2i = Vector2i.ZERO) -> void:
	var tile: Image = _checker_tile
	if _compositor != null:
		var season_tile: Image = _compositor._get_season_checker_tile(_compositor.hud_season_index(month), 255)
		if season_tile != null:
			tile = season_tile
	if tile == null:
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
				if py < 0 or py >= 240: continue
				var vt: int = (dy + v0) & 31
				for dx in range(32):
					var px: int = base_x + dx
					if px < 0 or px >= 320: continue
					var ut: int = (dx + u0) & 31
					target.set_pixelv(Vector2i(px, py), tile.get_pixel(ut, vt))

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

		_draw_text_hex_checked(target, choices[i], Vector2i(origin.x + 8, y_pos), 0, "choice_box", box_rect)

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

## Wait prompt marker (Actor 2 Frame 1: D_8011ED70 = 1)
func _draw_wait_marker(target: Image, screen_pos: Vector2i) -> void:
	_draw_actor(target, 1, Vector2i(screen_pos.x - 160, screen_pos.y - 120))

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

## Scene 1 CG: Kirameki Central Park with Cherry Blossoms + HU10 Shiori Portrait
func _draw_cherry_blossom_cg(target: Image, _rect: Rect2i) -> void:
	if _park_bg_image != null:
		target.blit_rect(_park_bg_image, Rect2i(0, 0, mini(320, _park_bg_image.get_width()), mini(160, _park_bg_image.get_height())), Vector2i(0, 0))
	if _shiori_portrait_image != null and _compositor != null:
		_compositor._blit(target, _shiori_portrait_image, null, Vector2i(81, 8), false)

## Scene 2 CG: Karaoke Box / Concert Stage (I_28_02.BS) + AS01 Ayako Portrait
func _draw_karaoke_lounge_cg(target: Image, _rect: Rect2i) -> void:
	var bg: Image = _karaoke_bg_image if _karaoke_bg_image != null else _park_bg_image
	if bg != null:
		target.blit_rect(bg, Rect2i(0, 0, mini(320, bg.get_width()), mini(160, bg.get_height())), Vector2i(0, 0))
	var portrait: Image = _ayako_portrait_image if _ayako_portrait_image != null else _shiori_portrait_image
	if portrait != null and _compositor != null:
		_compositor._blit(target, portrait, null, Vector2i(81, 8), false)

## Scene 3 CG: Ferris Wheel Gondola Viewport (I_11_01.BS) + HU10 Shiori Portrait
func _draw_ferris_wheel_viewport(target: Image, _rect: Rect2i) -> void:
	var bg: Image = _ferris_bg_image if _ferris_bg_image != null else _park_bg_image
	if bg != null:
		target.blit_rect(bg, Rect2i(0, 0, mini(320, bg.get_width()), mini(160, bg.get_height())), Vector2i(0, 0))
	if _shiori_portrait_image != null and _compositor != null:
		_compositor._blit(target, _shiori_portrait_image, null, Vector2i(81, 8), false)

## Scene 4 CG: Park Entrance Bump Corner (D_24_03.BS) + TA10 Miharu Portrait
func _draw_park_entrance_cg(target: Image, _rect: Rect2i) -> void:
	var bg: Image = _park_entrance_bg_image if _park_entrance_bg_image != null else _park_bg_image
	if bg != null:
		target.blit_rect(bg, Rect2i(0, 0, mini(320, bg.get_width()), mini(160, bg.get_height())), Vector2i(0, 0))
	var portrait: Image = _miharu_portrait_image if _miharu_portrait_image != null else _shiori_portrait_image
	if portrait != null and _compositor != null:
		_compositor._blit(target, portrait, null, Vector2i(81, 8), false)

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
		elif (region == "title_badge" or region == "choice_box") and custom_bounds.size.x > 0:
			if gx_pos < custom_bounds.position.x or gx_pos + 12 > custom_bounds.position.x + custom_bounds.size.x or gy_pos < custom_bounds.position.y or gy_pos + 12 > custom_bounds.position.y + custom_bounds.size.y:
				bounding_box_violations.append(
					"%s glyph %s at (%d, %d) overflows %s" % [
						region, code_unit.hex_encode(), gx_pos, gy_pos, str(custom_bounds)
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
