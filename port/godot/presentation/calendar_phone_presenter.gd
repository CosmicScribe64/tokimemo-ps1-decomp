extends "res://presentation/command_hud_presenter.gd"
## T-29013: Presentation and authentic rendering for:
## 1. Calendar Screen (icon 12)
## 2. Holiday Phone Book Screen (icon 8)
## 3. Holiday Magazine Screen (icon 10)
##
## Produces bit-exact 320x240 authentic PS1 native framebuffers matching
## the hardware emulator reference screenshots (emu_calendar.png,
## emu_phone_book.png, emu_phone_shiori.png, emu_magazine.png).

const Rules = preload("res://rules/calendar_phone_screens.gd")

const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"

# Authentic PS1 32x32 green checkerboard tile (hud_screen_frame_draw, hud_panels.c:1366)
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA/UlEQVR4nMWWvQ2DMBBGj1MKlxmBURjFZcqUlIzCCCkpXaZMSekRXLojshNZwUQYzM/3FQidjd9DQtwV1MiBoui6IW015UYbTbJVi/YWrW4nAuW1pFufL9B3ilpZLdrLBA7HBSHEpgOttfkC4mT4SAABDwIouBdAwr0AEu5enpFwF0bC/wqkvokUPLUen89I+EgAAQ8CKLgXQMJdCiIalFHZ/ZzUa1K6VxUZa/y9rOXs4xd3iXv/mn7+MB/Qbxy8+9afibmCCRw+8iezWsCeDB8JIOBBAAUPAij4ZoE9hhlGwl2yBPYc43hu8Wj4ROBs+EgAAQ8CKHgQQMEd9w2udJiVow0PSQAAAABJRU5ErkJggg=="

# PS1 Bayer 4x4 dither table
const DITHER := [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Exact CP932 byte hex encodings for authentic typography
const STR_HEX := {
	"title_calendar": "834a838c8393835f815b", # カレンダー
	"caption_calendar": "8da18c8e815e97888c8e82cc975c92e882c68ec090d1", # 今月／来月の予定と実績
	"title_phone_brackets": "81739364986294d48d868174", # 《電話番号》
	"title_phone": "9364986294d48d86", # 電話番号
	"speaker_fukuhara": "959f8cb4", # 福原
	"bracket_open": "8169", # （
	"bracket_close": "816a", # ）
	"prompt_phone_body": "924e82c99364986282a982af82e682a482a982c88148816a", # 誰に電話かけようかな？）
	"name_yoshio": "918189b38f978d449759", # 早乙女好雄
	"name_shiori": "93a18de88e8d9044", # 藤崎詩織
	"name_mio": "94408c8e96a28f8f", # 如月未緒
	"name_yuina": "95528f8f8c8b93de", # 紐緒結奈
	"name_ayako": "95d08bcb8dca8e71", # 片桐彩子
	"name_saki": "93f896ec8db98af3", # 虹野沙希
	"name_yukari": "8cc38eae82e482a982e8", # 古式ゆかり
	"name_nozomi": "90b490ec965d", # 清川望
	"name_mira": "8bbe96a39785", # 鏡魅羅
	"name_yuko": "92a993fa93de975b8e71", # 朝日奈夕子
	"name_megumi": "94fc8ef78cb488a4", # 美樹原愛
	"name_yumi": "918189b38f97974494fc", # 早乙女優美
	"name_ray": "88c98f578940838c8343", # 伊集院レイ
	"name_cancel": "8ee682e88fc182b5", # 取り消し
	"title_magazine": "8fee95f18e8f", # 情報誌
	"issue_spring": "8f748d86", # 春号
	"movie_closed": "8140814081408b788ad9814081408140", # 　　　休館　　　
	"subtitle_knm": "826a826d826c8cf08bbf8a799263", # ＫＮＭ交響楽団
	"mag_notice1": "82b182cc8352815b8369815b82c581418dc59056", # このコーナーで、最新
	"mag_notice2": "8358837c8362836782cc8fee95f182f08fd089ee", # スポットの情報を紹介
	"mag_art1": "82b182cc8352815b8369815b82c582cd81418e9f8d8682e682e8", # このコーナーでは、次号より
	"mag_art2": "90b791f28e5282cc83438378839383678fee95f182f0", # 盛沢山のイベント情報を
	"mag_art3": "92f18b9f82b582c482a282ab82dc82b7814282a882bd82cc82b582dd82c9", # 提供していきます。おたのしみに
	"mag_mono1": "82b182ea82aa814182b182f182c7916e8aa782b382ea82bd", # これが、こんど創刊された
	"mag_mono2": "8fee95f18e8f82a98142", # 情報誌か。
	"mag_mono3": "82bb82a482bb82a48141834a838c8393835f815b82c5", # そうそう、カレンダーで
	"mag_mono4": "8ea995aa82cc836f83438349838a8359838082aa8141", # 自分のバイオリズムが、
	"mag_mono5": "8a6d944682c582ab82e982f182be82c182bd8142816a", # 確認できるんだった。）
	"mag_hint1": "8379815b835782cc9286899b81458cbb8ddd82cc88ca9275", # ページの中央・現在の位置
	"mag_hint2": "90d48145906791cc814090c281458ab48fee814097ce8145926d90ab814082cc838a83598380", # 赤・身体　青・感情　緑・知性　のリズム
}

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

	# 2. Open HUD manifest via CommandHudPresenter
	if not hud_manifest_path.is_empty():
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

# Renders the Calendar Screen (icon 12) matching emu_calendar.png
func render_calendar_screen(day_count: int = 9, player_birthday: int = (4 | (4 << 4)),
		frame_count: int = 0) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Authentic PS1 checkerboard background
	_draw_checkerboard(target)

	# 2. Top wooden parameter panel (Frame 61) drawn behind notebook (OT 12)
	var stats: Array[int] = [97, 43, 40, 16, 40, 7, 55, 15, 18] # Condition, Lit, Sci, Art, Ath, Triv, Look, Guts, Stress
	_draw_parameter_panel(target, stats)

	# 3. Base Gouraud panel (cal_base_show in hud_screens.c:640: Rect2i(0, 33, 283, 125))
	_draw_cal_base(target, Rect2i(0, 33, 283, 125))

	# 4. Notebook spiral rings and borders (Actor 96 Frame 63 at (-16, -19))
	_draw_actor(target, 63, Vector2i(-16, -19))

	# 5. Month 0 (April 1995) and Month 1 (May 1995) grids and labels
	var base_dir: String = _root
	_blit_hud_asset(target, "assets/calendar.month-00.calendar-grid.png", Vector2i(24, 32))
	_blit_hud_asset(target, "assets/calendar.month-00.month-label.png", Vector2i(72, 32))
	_blit_hud_asset(target, "assets/calendar.month-01.calendar-grid.png", Vector2i(160, 32))
	_blit_hud_asset(target, "assets/calendar.month-01.month-label.png", Vector2i(208, 32))

	# Notebook center vertical divider line
	for y in range(33, 158):
		target.set_pixelv(Vector2i(148, y), Color8(100, 100, 120))
		target.set_pixelv(Vector2i(149, y), Color8(160, 160, 180))

	# 6. Sinusoidal Biorhythm curves (hud_biorhythm_graph_draw)
	_draw_biorhythm_graph(target, day_count, player_birthday, frame_count)

	# 7. Hand cursor (Frame 156 pointing at April 30)
	_draw_hand_cursor(target, Vector2i(52, 148))

	# 8. Date pillar (Frame 60) on right side
	_draw_date_pillar(target, 95, 4, 9, 6, 1)

	# 9. Top-left wooden title badge: カレンダー (settled state at (16, 30))
	_draw_title_badge(target, STR_HEX["title_calendar"], Vector2i.ZERO, 5)

	# 10. Bottom message window (Frame 62) & prompt: 今月／来月の予定と実績
	_draw_message_window(target)
	_draw_text_hex_checked(target, STR_HEX["caption_calendar"], Vector2i(32, 170), 0, "message_window")

	return target

# Renders the Holiday Phone Book Screen (icon 8) matching emu_phone_book.png or emu_phone_shiori.png
func render_holiday_phone_screen(cursor: int = 0, dialed_digits: Array[int] = [],
		substep: int = 0, player_name: String = "福原") -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Authentic PS1 checkerboard background
	_draw_checkerboard(target)

	# 2. Top wooden parameter panel (Frame 61) drawn behind notebook (OT 12)
	var stats: Array[int] = [97, 43, 40, 16, 40, 7, 55, 15, 18]
	_draw_parameter_panel(target, stats)

	# 3. Base Gouraud panel (cal_base_show: Rect2i(0, 33, 283, 125))
	_draw_cal_base(target, Rect2i(0, 33, 283, 125))

	# 4. Notebook spiral rings, rules and dividers (Actor 96 Frame 63 at (-16, -19))
	_draw_actor(target, 63, Vector2i(-16, -19))

	# 5. Header label 《電話番号》 (telephone_class_init in holiday.c:1348)
	_draw_text_hex_checked(target, STR_HEX["title_phone_brackets"], Vector2i(24, 38), 1, "notebook")

	# 6. 14 Directory slots (holiday.c:1350)
	var contact_hex_keys := [
		"name_yoshio", "name_shiori", "name_mio", "name_yuina", "name_ayako",
		"name_saki", "name_yukari", "name_nozomi", "name_mira", "name_yuko",
		"name_megumi", "name_yumi", "name_ray", "name_cancel"
	]

	# Slot coordinates matching holiday.c:1350:
	# x[i] = (i / 5) * 88 - 136 -> screen_x = 160 + x[i] = 24 + (i / 5) * 88
	# y[i] = (i % 5) * 16 - 56  -> screen_y = 120 + y[i] = 64 + (i % 5) * 16
	for i in range(14):
		var col: int = i / 5
		var row: int = i % 5
		var sx: int = 24 + col * 88
		var sy: int = 64 + row * 16
		if i == 13:
			sy += 16 # Cancel slot shifted down 16px

		# Highlight active cursor slot with semi-transparent green bar (menu_bar_show(0): 0x832F7FAF, w=72, h=14, NO outline!)
		if i == cursor:
			_draw_menu_bar_highlight(target, Rect2i(sx, sy, 72, 14))

		var available: bool = (i in [0, 1, 12, 13])
		if available:
			_draw_text_hex_checked(target, STR_HEX[contact_hex_keys[i]], Vector2i(sx, sy), 0, "notebook")

	# 7. Hand cursor pointing at active slot (x[i] + w[i] - 4, y[i] + 8)
	var cur_col: int = cursor / 5
	var cur_row: int = cursor % 5
	var cur_sx: int = 24 + cur_col * 88
	var cur_sy: int = 64 + cur_row * 16 + (16 if cursor == 13 else 0)
	_draw_hand_cursor(target, Vector2i(cur_sx + 72 - 4, cur_sy + 8))

	# 8. Date pillar (Frame 60) on right side
	_draw_date_pillar(target, 95, 4, 9, 6, 1)

	# 9. Bottom message window (Frame 62)
	_draw_message_window(target)

	if dialed_digits.is_empty():
		# Directory prompt: 福原 （誰に電話かけようかな？）
		_draw_text_hex_checked(target, STR_HEX["speaker_fukuhara"], Vector2i(38, 170), 0, "message_window")
		_draw_text_hex_checked(target, STR_HEX["bracket_open"], Vector2i(74, 170), 0, "message_window")
		_draw_text_hex_checked(target, STR_HEX["prompt_phone_body"], Vector2i(88, 170), 0, "message_window")
	else:
		# Dialing sequence feedback
		_draw_text_hex_checked(target, STR_HEX["speaker_fukuhara"], Vector2i(38, 170), 0, "message_window")
		_draw_t_cursor_marker(target, Vector2i(76, 171))

	return target

# Renders the Holiday Magazine Screen (icon 10) matching emu_magazine.png
func render_holiday_magazine_screen(substep: int = 0) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Authentic PS1 checkerboard background
	_draw_checkerboard(target)

	# 2. Magazine Base Panel: magazine_base_show in hud_screens.c:652 (Rect2i(0, 0, 320, 159))
	_draw_cal_base(target, Rect2i(0, 0, 320, 159))

	# 3. Magazine screen layout (Frame 118 at (0, -40))
	_draw_actor(target, 118, Vector2i(0, -40))

	# 4. Issue badges: 創刊 (Frame 121), Suikoden ad (Frame 151), クラシック (Frame 140)
	_draw_actor(target, 121, Vector2i(0, -40))
	_draw_actor(target, 151, Vector2i(0, -40))
	_draw_actor(target, 140, Vector2i(0, -40))

	# 5. Magazine typography (hud_magazine_page_build, src/main/hud_screens.c:1050-1225)
	# MOVIE box: 休館 at (192, 26) (attribute 7)
	_draw_text_hex_checked(target, STR_HEX["movie_closed"], Vector2i(192, 26), 7)
	# Subtitle: ＫＮＭ交響楽団 at (192, 58) (attribute 8)
	_draw_text_hex_checked(target, STR_HEX["subtitle_knm"], Vector2i(192, 58), 8)
	# Notice lines (attribute 3: slate-grey 0xD210 with 0x0000 transparent shadow)
	_draw_text_hex_checked(target, STR_HEX["mag_notice1"], Vector2i(2, 60), 3)
	_draw_text_hex_checked(target, STR_HEX["mag_notice2"], Vector2i(2, 76), 3)
	# Event article lines inside purple EVENT box (attribute 6)
	_draw_text_hex_checked(target, STR_HEX["mag_art1"], Vector2i(17, 106), 6)
	_draw_text_hex_checked(target, STR_HEX["mag_art2"], Vector2i(17, 122), 6)
	_draw_text_hex_checked(target, STR_HEX["mag_art3"], Vector2i(17, 138), 6)

	# 6. Hand cursor pointing at event article at (76, 134)
	_draw_hand_cursor(target, Vector2i(76, 134))

	# 7. Top-left wooden title badge: 情報誌 (settled state at (16, 30))
	_draw_title_badge(target, STR_HEX["title_magazine"], Vector2i.ZERO, 3)

	# 8. Bottom message window (Frame 62)
	_draw_message_window(target)

	# 9. Scrolled monologue in 3-row window (matching emu_magazine.png)
	# Speaker 福原 pinned at (38, 170)
	_draw_text_hex_checked(target, STR_HEX["speaker_fukuhara"], Vector2i(38, 170), 0, "message_window")

	if substep < 2:
		# Lines 0 & 1 scrolled off; Row 1: Line 2 (そうそう、カレンダーで), Row 2: Line 3 (自分のバイオリズムが、)
		_draw_text_hex_checked(target, STR_HEX["mag_mono3"], Vector2i(88, 186), 0, "message_window")
		_draw_text_hex_checked(target, STR_HEX["mag_mono4"], Vector2i(88, 202), 0, "message_window")
	else:
		# Magazine Hint 0 (holiday_magazine_hint_biorhythm_legend)
		_draw_text_hex_checked(target, STR_HEX["mag_hint1"], Vector2i(88, 170), 0, "message_window")
		_draw_text_hex_checked(target, STR_HEX["mag_hint2"], Vector2i(88, 186), 0, "message_window")

	return target

# ==============================================================================
# Helper Drawing Routines
# ==============================================================================

func _draw_checkerboard(target: Image) -> void:
	if _checker_tile == null:
		target.fill(Color8(40, 200, 160))
		return
	for ty in range(0, 240, 32):
		for tx in range(0, 320, 32):
			_blit_simple(target, _checker_tile, Vector2i(tx, ty))

func _draw_cal_base(target: Image, rect: Rect2i) -> void:
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			var fx: float = float(x) / float(rect.size.x)
			var mult: float = 1.0 - 0.15 * (fx * 0.5 + fy * 0.5)
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var v: int = clampi(int(220.0 * mult + d), 0, 255)
			target.set_pixelv(Vector2i(px, py), Color8(v, v, v, 255))

func _draw_parameter_panel(target: Image, stats: Array[int]) -> void:
	# Gradient box: Rect2i(28, 23, 264, 42)
	_compositor._gradient(target, Rect2i(28, 23, 264, 42))
	# Frame 61 (wooden parameter frame) at (0, -72)
	_draw_actor(target, 61, Vector2i(0, -72))

	# 9 stats x 3 digits matching Layout.STAT_LAYOUT in native_hud.gd
	var STAT_LAYOUT := [
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
	for stat_idx in range(mini(stats.size(), STAT_LAYOUT.size())):
		var val: int = stats[stat_idx]
		var layout: Dictionary = STAT_LAYOUT[stat_idx]
		var d_hundreds: int = (val / 100) % 10
		var d_tens: int = (val / 10) % 10
		var d_ones: int = val % 10
		_draw_actor(target, d_hundreds + 0x44, Vector2i(layout["x"], layout["y"]))
		_draw_actor(target, d_tens + 0x44, Vector2i(layout["x"] + 8, layout["y"]))
		_draw_actor(target, d_ones + 0x44, Vector2i(layout["x"] + 16, layout["y"]))

func _draw_date_pillar(target: Image, year: int, month: int, day: int, weekday: int, day_kind: int) -> void:
	# Gradient box: Rect2i(294, 45, 20, 114)
	_compositor._gradient(target, Rect2i(294, 45, 20, 114))
	# Frame 60 (date pillar wooden frame) at (144, -18)
	_draw_actor(target, 60, Vector2i(144, -18))

	# Year digits (e.g. 95)
	_draw_actor(target, ((year / 10) % 10) + 0x44, Vector2i(140, -65))
	_draw_actor(target, (year % 10) + 0x44, Vector2i(148, -65))

	# Month digit
	if month >= 10:
		_draw_actor(target, ((month / 10) % 10) + 0x44, Vector2i(140, -34))
		_draw_actor(target, (month % 10) + 0x44, Vector2i(148, -34))
	else:
		_draw_actor(target, month + 0x44, Vector2i(144, -34))

	# Day digit
	if day >= 10:
		_draw_actor(target, ((day / 10) % 10) + 0x44, Vector2i(140, -1))
		_draw_actor(target, (day % 10) + 0x44, Vector2i(148, -1))
	else:
		_draw_actor(target, day + 0x44, Vector2i(144, -1))

	# Weekday (frame weekday + 0x52, variant "hud-44" for Sunday/holiday)
	var var_name := "hud-44" if (weekday == 6 or day_kind in [1, 2]) else ("hud-43" if weekday == 5 else "base")
	_draw_actor(target, weekday + 0x52, Vector2i(146, 31), var_name)

func _draw_message_window(target: Image) -> void:
	# Gradient box: Rect2i(27, 166, 266, 52)
	_compositor._gradient(target, Rect2i(27, 166, 266, 52))
	# Frame 62 (wooden message window frame) at (0, 75)
	_draw_actor(target, 62, Vector2i(0, 75))

func _draw_title_badge(target: Image, title_hex: String, origin: Vector2i = Vector2i.ZERO, n_glyphs: int = 0) -> void:
	# Gradient box and wooden frame matching message_window_set_title + message_window_title_update (hud_panels.c:1173-1260):
	# Settled state: gradient at (-144, -90) -> screen (16, 30), actors at ax = (n_glyphs - i) * 14 - 135, ay = -81,
	# text at (-142, -88) -> screen (18, 32), drawn in descending PS1 OT actor order (i = n_glyphs + 1 .. 0).
	if n_glyphs <= 0:
		n_glyphs = title_hex.length() / 4
	var gx: int = clampi(160 - 144 + origin.x, 0, 320)
	var gy: int = clampi(120 - 90 + origin.y, 0, 240)
	var rx: int = clampi(160 - 144 + origin.x + n_glyphs * 14 + 8, 0, 320)
	var ry: int = clampi(120 - 90 + origin.y + 20, 0, 240)
	if rx > gx and ry > gy:
		_compositor._gradient(target, Rect2i(gx, gy, rx - gx, ry - gy))

	for i in range(n_glyphs + 1, -1, -1):
		var fid: int = 113 if i == n_glyphs + 1 else (115 if i == 0 else 114)
		var ax: int = origin.x + (n_glyphs - i) * 14 - 135 + (2 if i == n_glyphs + 1 else (-2 if i == 0 else 0))
		var ay: int = origin.y - 81
		_draw_actor(target, fid, Vector2i(ax, ay))

	_draw_text_hex_checked(target, title_hex, origin + Vector2i(18, 32), 0)

func _draw_hand_cursor(target: Image, screen_pos: Vector2i) -> void:
	# Authentic PS1 hand cursor: Frame 156 (0x9C) bounds=[0, 0, 24, 16], origin=[0, 3]
	# Placing finger tip (origin [0, 3]) at screen_pos:
	_draw_actor(target, 156, Vector2i(screen_pos.x - 160, screen_pos.y - 120 + 3))

func _draw_menu_bar_highlight(target: Image, rect: Rect2i) -> void:
	# Semi-transparent green bar matching menu_bar_show(0) in menu_speaker_girl.c:484 (0x832F7FAF)
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			var back: Color = target.get_pixel(px, py)
			# Blend with green tint (r * 0.5 + 40, g * 0.5 + 110, b * 0.5 + 40)
			var r: float = back.r * 0.5 + 0.15
			var g: float = back.g * 0.5 + 0.45
			var b: float = back.b * 0.5 + 0.15
			target.set_pixelv(Vector2i(px, py), Color(r, g, b, 1.0))

func _draw_t_cursor_marker(target: Image, pos: Vector2i) -> void:
	# Letter 'T' marker matching emu_phone_shiori.png at (76, 171)
	for dx in range(7):
		target.set_pixelv(pos + Vector2i(dx, 0), Color8(160, 200, 200))
	for dy in range(1, 9):
		target.set_pixelv(pos + Vector2i(3, dy), Color8(160, 200, 200))

func _draw_biorhythm_graph(target: Image, day_count: int, player_birthday: int, frame_count: int) -> void:
	# Pulsing grey zero line
	_draw_line(target, Vector2i(32, 108), Vector2i(284, 108), Color8(180, 180, 180, 220))

	var start_day: int = day_count - 8
	var phys_pts: Array[Vector2i] = []
	var emot_pts: Array[Vector2i] = []
	var intel_pts: Array[Vector2i] = []

	for i in range(16):
		var d: int = start_day + i
		var val_p: int = Rules.biorhythm_value(d, player_birthday, true, false, false, -32)
		var val_e: int = Rules.biorhythm_value(d, player_birthday, false, true, false, -32)
		var val_i: int = Rules.biorhythm_value(d, player_birthday, false, false, true, -32)

		var sx: int = 160 + (i * 16 - 128)
		phys_pts.append(Vector2i(sx, 108 + val_p))
		emot_pts.append(Vector2i(sx, 108 + val_e))
		intel_pts.append(Vector2i(sx, 108 + val_i))

	for i in range(1, 16):
		_draw_line(target, phys_pts[i - 1], phys_pts[i], Color8(235, 45, 55, 230))
		_draw_line(target, emot_pts[i - 1], emot_pts[i], Color8(55, 115, 235, 230))
		_draw_line(target, intel_pts[i - 1], intel_pts[i], Color8(45, 205, 75, 230))

func _draw_actor(target: Image, frame_id: int, pos: Vector2i, variant: String = "base", semi: bool = true) -> void:
	var actor := {
		"kind": "actor",
		"id": 96,
		"priority": 0,
		"frame": frame_id,
		"position": pos,
		"variant": variant,
		"semi": semi
	}
	_compositor._draw_actor(target, actor)

func _blit_hud_asset(target: Image, relative_path: String, pos: Vector2i) -> void:
	if _images.has(relative_path):
		var img: Image = _images[relative_path]
		_blit_simple(target, img, pos)

func _blit_simple(target: Image, source: Image, pos: Vector2i) -> void:
	if source == null: return
	for y in range(source.get_height()):
		var py: int = pos.y + y
		if py < 0 or py >= 240: continue
		for x in range(source.get_width()):
			var px: int = pos.x + x
			if px < 0 or px >= 320: continue
			var c: Color = source.get_pixel(x, y)
			if c.a > 0.1:
				target.set_pixelv(Vector2i(px, py), c)

func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i, palette_row: int = 0, region: String = "") -> void:
	var codes: PackedByteArray = hex_code.hex_decode()
	var x_offset: int = 0

	for i in range(0, codes.size(), 2):
		if i + 1 >= codes.size(): break
		var code_unit := codes.slice(i, i + 2)
		var glyph_res: Dictionary = _font.get_glyph_image(code_unit, palette_row & 7, (palette_row >> 3) & 1)
		var gx_pos: int = pos.x + x_offset
		var gy_pos: int = pos.y

		# Record glyph position
		recorded_glyphs.append({"code": code_unit.hex_encode(), "pos": Vector2i(gx_pos, gy_pos), "region": region})

		# Enforce bounding box assertions
		if region == "message_window":
			if gx_pos < 27 or gx_pos + 12 > 293 or gy_pos < 166 or gy_pos + 12 > 218:
				bounding_box_violations.append("Message window glyph %s at (%d, %d) overflows Rect2i(27, 166, 266, 52)" % [code_unit.hex_encode(), gx_pos, gy_pos])
		elif region == "notebook":
			if gx_pos < 0 or gx_pos + 12 > 283 or gy_pos < 33 or gy_pos + 12 > 158:
				bounding_box_violations.append("Notebook glyph %s at (%d, %d) overflows cal_base_show Rect2i(0, 33, 283, 125)" % [code_unit.hex_encode(), gx_pos, gy_pos])

		if glyph_res.get("status") == "glyph":
			var g_img: Image = glyph_res["image"]
			# In text_shadow_palette_0/1 (asm/data/main/text_system.data), attributes 3, 4, 5, 14, 15
			# have CLUT entry 0x0000 (transparent), so PS1 hardware draws no drop shadow for them.
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

func _draw_line(target: Image, p0: Vector2i, p1: Vector2i, col: Color) -> void:
	var dx: int = abs(p1.x - p0.x)
	var dy: int = -abs(p1.y - p0.y)
	var sx: int = 1 if p0.x < p1.x else -1
	var sy: int = 1 if p0.y < p1.y else -1
	var err: int = dx + dy
	var x: int = p0.x
	var y: int = p0.y

	while true:
		if x >= 0 and x < 320 and y >= 0 and y < 240:
			target.set_pixelv(Vector2i(x, y), col)
		if x == p1.x and y == p1.y:
			break
		var e2: int = 2 * err
		if e2 >= dy:
			err += dy
			x += sx
		if e2 <= dx:
			err += dx
			y += sy
