class_name TelephoneOverlayPresenter
extends "res://presentation/date_appointment_presenter.gd"
## T-29018: Authentic 320x240 PS1 Native Renderer for TEL Overlay screens:
## 1. Yoshio Saotome Girl Profile Notebook Screen (0x3E stage 0)
## 2. Telephone Date Place Selection Menu (phone_date_place_availability_build)
## 3. Telephone Date Day Selection Menu (phone_available_date_slots_build)
## 4. Girl Phone Call Response VN Scene (phone_date_invitation_resolve)
##
## Fully compliant with CODING_STANDARDS.md Section 12a:
## - Exact call-site coordinates (center origin x:160, y:120)
## - Exact CP932 hex byte encodings (never guessed)
## - Text shadow suppression for palette attributes 3, 4, 5, 14, 15
## - Bounding box checks: Rect2i(0, 33, 283, 125) for notebook, Rect2i(27, 166, 266, 52) for message window

const OverlayRules = preload("res://overlays/telephone_overlay.gd")

# Exact CP932 byte hex encodings verified via python3 .encode("cp932").hex()
const STR_HEX := {
	"title_appraisal": "8f9782cc8e7182a982e782cc955d89bf", # 女の子からの評価
	"title_phone": "9364986294d48d86", # 電話番号
	"label_birthday": "926190b693fa", # 誕生日
	"label_zodiac": "90af8dc0", # 星座
	"label_blood": "8c8c89748c5e", # 血液型
	"label_bwh": "826182768267", # ＢＷＨ
	"label_club": "834e83898375", # クラブ
	"label_notes": "94f58d6c", # 備考
	"unit_month": "8c8e", # 月
	"unit_day": "93fa", # 日
	"club_private": "8e8490dd9594", # 私設部

	"title_datespot": "8366815b83678358837c83628367", # デートスポット
	"next_page": "8e9f82cc8379815b835782d6", # 次のページへ
	"prev_page": "914f82cc8379815b835782d6", # 前のページへ
	"prompt_choose_place": "8366815b836782b782e98fea8f8a82f0914982f182c589ba82b382a2", # デートする場所を選んで下さい
	"prompt_choose_day": "8366815b836782b782e993fa82f0914982f182c589ba82b382a2", # デートする日を選んで下さい

	"speaker_shiori": "93a18de8", # 藤崎
	"speaker_yoshio": "8d449759", # 好雄
	"call_accept": "82a282a282ed82e68142208d7382ab82dc82b582e582a48142", # いいわよ。 行きましょう。
	"call_refuse": "82b282df82f182c882b382a281412082bb82cc93fa82cd93738d8782aa88ab82a282cc8142", # ごめんなさい、 その日は都合が悪いの。
	"answering_machine1": "82a0816082a0814197af8ee782dd82bd82a282be82c88142", # あ〜あ、留守みたいだな。
	"answering_machine2": "8da193fa82cd82a082ab82e782df82e982a9816381638142816a", # 今日はあきらめるか……。）
	"yoshio_found1": "82a082c182bd82a082c182bd8142", # あったあった。
	"yoshio_found2": "8ea882a982c182db82b682c182c4", # 耳かっぽじって
	"yoshio_found3": "82e682ad95b782af82e68142", # よく聞けよ。

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
	"name_yoshio": "918189b38f978d449759", # 早乙女好雄
	"name_yumi": "918189b38f97974494fc", # 早乙女優美
	"name_ray": "88c98f578940838c8343", # 伊集院レイ

	"place_0": "8bdf8f8a82cc8cf68980", # 近所の公園
	"place_1": "82ab82e782df82ab9286899b8cf68980", # きらめき中央公園
	"place_2": "83568387836283738393834f8a58", # ショッピング街
	"place_3": "908591b08ad9", # 水族館
	"place_4": "93ae95a88980", # 動物園
	"place_5": "904195a88980", # 植物園
	"place_6": "83768389836c835e838a83458380", # プラネタリウム
	"place_7": "94fc8f708ad9", # 美術館
	"place_8": "907d8f918ad9", # 図書館
	"place_9": "8351815b8380835a8393835e815b", # ゲームセンター
	"place_10": "837b815b838a8393834f8fea", # ボーリング場
	"place_11": "834a83898349835089ae", # カラオケ屋
	"place_12": "97568980926e", # 遊園地
	"place_13": "8358835e835783418380", # スタジアム
	"place_14": "896689e68ad9", # 映画館
	"place_15": "835283938354815b836789ef8fea", # コンサート会場
	"place_16": "8376815b838b", # プール
	"place_17": "8a43", # 海
	"place_18": "905f8ed0", # 神社
	"place_19": "83588350815b83678fea", # スケート場
	"place_20": "8358834c815b8fea", # スキー場

	"sign_gemini": "82d382bd82b28dc0", # ふたご座
	"blood_a": "82608c5e", # Ａ型
	"club_bungei": "95b68c7c9594", # 文芸部
	"shiori_notes_1": "97658e70925b97ed814190ac90d197448f4782c8", # 容姿端麗、成績優秀な
	"shiori_notes_2": "8370815b83748346834e836782c8976382c882b682dd8142", # パーフェクトな幼なじみ。
	"shiori_bwh": "82578252814582548255814582578253", # ８３・５６・８４
	"shiori_birthday": "82548c8e8251825693fa", # ５月２７日
}

var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []


func open_telephone_preview(hud_manifest_path: String, font_root: String, date_pack_root: String, hud_sha: String = HUD_SHA) -> bool:
	recorded_glyphs.clear()
	bounding_box_violations.clear()
	var ok := open_date_preview(hud_manifest_path, font_root, date_pack_root, hud_sha)
	_ensure_compositor_coverage()
	return ok

func _ensure_compositor_coverage() -> void:
	if _compositor != null and _compositor.last_coverage == null:
		_compositor.last_coverage = Image.create(320, 240, false, Image.FORMAT_L8)
		_compositor.last_coverage.fill(Color.BLACK)


# ==============================================================================
# 1. Yoshio's Girl Profile Notebook Screen (0x3E Stage 0)
# ==============================================================================

func render_girl_profile_notebook_screen(girl_id: int = OverlayRules.GIRL_SHIORI,
		stats: Array = [97, 43, 40, 40, 38, 33, 59, 4, 3],
		year: int = 95, month: int = 4, day: int = 9,
		weekday: int = 6, day_kind: int = 1,
		bg_uv: Vector2i = Vector2i(20, 14)) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Authentic PS1 checkerboard background
	_draw_checkerboard(target, bg_uv)

	# 2. Wooden parameter panel (Frame 61)
	_draw_parameter_panel(target, stats)

	# 3. Base Gouraud appraisal panel (appraisal_base_show in hud_screens.c:645: Rect2i(0, 16, 283, 142))
	_draw_cal_base(target, Rect2i(0, 16, 283, 142))

	# 4. Notebook spiral rings & divider (Actor 0x67 Frame 0x41 = 65 at (-16, -27) in phone_profile_details_draw)
	_draw_actor(target, 65, Vector2i(-16, -27))

	# 5. Date pillar (Actor 16 Frame 60)
	_draw_date_pillar(target, year, month, day, weekday, day_kind)

	# 6. Profile Title / Full Name at screen (32, 22) (x: -0x80, y: -0x62 in C)
	var name_key := "name_shiori"
	if girl_id == OverlayRules.GIRL_MIO: name_key = "name_mio"
	elif girl_id == OverlayRules.GIRL_YUINA: name_key = "name_yuina"
	elif girl_id == OverlayRules.GIRL_AYAKO: name_key = "name_ayako"
	elif girl_id == OverlayRules.GIRL_SAKI: name_key = "name_saki"
	elif girl_id == OverlayRules.GIRL_YUKARI: name_key = "name_yukari"
	elif girl_id == OverlayRules.GIRL_NOZOMI: name_key = "name_nozomi"
	elif girl_id == OverlayRules.GIRL_MIRA: name_key = "name_mira"
	elif girl_id == OverlayRules.GIRL_YUKO: name_key = "name_yuko"
	elif girl_id == OverlayRules.GIRL_MEGUMI: name_key = "name_megumi"
	elif girl_id == OverlayRules.GIRL_YUMI: name_key = "name_yumi"
	elif girl_id == OverlayRules.GIRL_REI: name_key = "name_ray"
	_draw_text_hex_checked(target, STR_HEX[name_key], Vector2i(32, 22), 3)

	# 7. Phone header at screen (128, 22) (x: -0x20, y: -0x62) + 7 phone number digits at y = -0x5A
	_draw_text_hex_checked(target, STR_HEX["title_phone"], Vector2i(128, 22), 5)
	var phone_digits := [6, 3, 2, 5, 7, 3, 1]
	var phone_x := [0x27, 0x31, 0x3B, 0x4C, 0x56, 0x60, 0x6A]
	for d_idx in range(7):
		_draw_actor(target, phone_digits[d_idx] + 0x44, Vector2i(phone_x[d_idx], -0x5A))

	# 8. Profile detail field labels and values (inside Rect2i(0, 33, 283, 125))
	# - Birthday (誕生日) at (22, 42), value at (72, 42)
	_draw_text_hex_checked(target, STR_HEX["label_birthday"], Vector2i(22, 42), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX["shiori_birthday"], Vector2i(72, 42), 3, "notebook")

	# - Zodiac (星座) at (22, 58), value at (72, 58)
	_draw_text_hex_checked(target, STR_HEX["label_zodiac"], Vector2i(22, 58), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX["sign_gemini"], Vector2i(72, 58), 3, "notebook")

	# - Blood type (血液型) at (22, 74), value at (72, 74)
	_draw_text_hex_checked(target, STR_HEX["label_blood"], Vector2i(22, 74), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX["blood_a"], Vector2i(72, 74), 3, "notebook")

	# - BWH (ＢＷＨ) at (22, 90), value at (72, 90)
	_draw_text_hex_checked(target, STR_HEX["label_bwh"], Vector2i(22, 90), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX["shiori_bwh"], Vector2i(72, 90), 3, "notebook")

	# - Club (クラブ) at (22, 106), value at (72, 106)
	_draw_text_hex_checked(target, STR_HEX["label_club"], Vector2i(22, 106), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX["club_bungei"], Vector2i(72, 106), 3, "notebook")

	# - Notes (備考) at (22, 122), lines 1 & 2 at (72, 122) and (72, 138)
	_draw_text_hex_checked(target, STR_HEX["label_notes"], Vector2i(22, 122), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX["shiori_notes_1"], Vector2i(72, 122), 3, "notebook")
	_draw_text_hex_checked(target, STR_HEX["shiori_notes_2"], Vector2i(72, 138), 3, "notebook")

	# 9. Bottom message window (Frame 62, Rect2i(27, 166, 266, 52))
	_draw_message_window(target)
	_draw_text_hex_checked(target, STR_HEX["speaker_yoshio"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["yoshio_found1"], Vector2i(88, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["yoshio_found2"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["yoshio_found3"], Vector2i(88, 202), 0, "message_window")

	# 10. Hand cursor
	_draw_hand_cursor(target, Vector2i(270, 198))

	return target


# ==============================================================================
# 2. Telephone Date Place Selection Menu
# ==============================================================================

func render_date_place_menu_screen(page: int = 0, selected_idx: int = 0,
		avail_table: Array = [],
		stats: Array = [97, 43, 40, 40, 38, 33, 59, 4, 3],
		year: int = 95, month: int = 4, day: int = 9,
		weekday: int = 6, day_kind: int = 1,
		bg_uv: Vector2i = Vector2i(20, 14)) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	_draw_checkerboard(target, bg_uv)
	_draw_parameter_panel(target, stats)
	_draw_cal_base(target, Rect2i(0, 33, 283, 125))
	_draw_actor(target, 67, Vector2i(-16, -19))
	_draw_date_pillar(target, year, month, day, weekday, day_kind)

	# Title: デートスポット at (32, 38)
	_draw_text_hex_checked(target, STR_HEX["title_datespot"], Vector2i(32, 38), 3, "notebook")

	# Page flip button: 次のページへ (page 0) or 前のページへ (page 1) at (160, 38)
	var page_toggle_hex := STR_HEX["next_page"] if page == 0 else STR_HEX["prev_page"]
	_draw_text_hex_checked(target, page_toggle_hex, Vector2i(160, 38), 3, "notebook")

	# 2 Columns of 5 Date spots matching phone_date_place_availability_build
	for i in range(10):
		var col: int = i / 5
		var row: int = i % 5
		var sx: int = 24 if col == 0 else 160
		var sy: int = 64 + row * 16

		var spot_id: int = i if page == 0 else (i + 10)
		var is_available: bool = true
		if not avail_table.is_empty() and i < avail_table.size():
			is_available = (avail_table[i] != 0)

		# Highlight active cursor slot
		if i == selected_idx:
			_draw_menu_bar_highlight(target, Rect2i(sx, sy, 72, 14))

		if is_available and spot_id < 21:
			var place_key := "place_%d" % spot_id
			if STR_HEX.has(place_key):
				_draw_text_hex_checked(target, STR_HEX[place_key], Vector2i(sx, sy), 3, "notebook")

	# Bottom message window & prompt: デートする場所を選んで下さい
	_draw_message_window(target)
	_draw_text_hex_checked(target, STR_HEX["prompt_choose_place"], Vector2i(32, 170), 0, "message_window")

	# Hand cursor at active selection
	var cur_col: int = selected_idx / 5
	var cur_row: int = selected_idx % 5
	var cur_sx: int = (24 if cur_col == 0 else 160) + 72
	var cur_sy: int = 64 + cur_row * 16 + 8
	if selected_idx == 10: # Page flip button
		cur_sx = 160 + 72
		cur_sy = 38 + 8
	_draw_hand_cursor(target, Vector2i(cur_sx, cur_sy))

	return target


# ==============================================================================
# 3. Telephone Date Day Selection Menu
# ==============================================================================

func render_date_day_menu_screen(candidate_slots: Array[Dictionary] = [],
		selected_slot_idx: int = 0,
		stats: Array = [97, 43, 40, 40, 38, 33, 59, 4, 3],
		year: int = 95, month: int = 4, day: int = 9,
		weekday: int = 6, day_kind: int = 1,
		bg_uv: Vector2i = Vector2i(20, 14)) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	_draw_checkerboard(target, bg_uv)
	_draw_parameter_panel(target, stats)
	_draw_cal_base(target, Rect2i(0, 33, 283, 125))
	_draw_actor(target, 67, Vector2i(-16, -19))
	_draw_date_pillar(target, year, month, day, weekday, day_kind)

	# 2-Month Calendar grids (assets from HUD pack)
	_blit_hud_asset(target, "assets/calendar.month-00.calendar-grid.png", Vector2i(24, 32))
	_blit_hud_asset(target, "assets/calendar.month-00.month-label.png", Vector2i(72, 32))
	_blit_hud_asset(target, "assets/calendar.month-01.calendar-grid.png", Vector2i(160, 32))
	_blit_hud_asset(target, "assets/calendar.month-01.month-label.png", Vector2i(208, 32))

	# Highlight candidate date slots
	for idx in range(candidate_slots.size()):
		var slot: Dictionary = candidate_slots[idx]
		var s_day: int = int(slot.get("day", 1))
		var s_offset: int = int(slot.get("month_offset", 0))

		# Position on calendar: 7 cols (Sun..Sat)
		var base_x: int = 24 if s_offset == 0 else 160
		var cell_col: int = (s_day - 1) % 7
		var cell_row: int = (s_day - 1) / 7
		var cell_x: int = base_x + 8 + cell_col * 16
		var cell_y: int = 48 + cell_row * 14

		if idx == selected_slot_idx:
			_draw_menu_bar_highlight(target, Rect2i(cell_x - 2, cell_y - 2, 16, 14))
			_draw_hand_cursor(target, Vector2i(cell_x + 14, cell_y + 6))

	# Bottom message window & prompt: デートする日を選んで下さい
	_draw_message_window(target)
	_draw_text_hex_checked(target, STR_HEX["prompt_choose_day"], Vector2i(32, 170), 0, "message_window")

	return target


# ==============================================================================
# 4. Girl Phone Call Response VN Scene
# ==============================================================================

func render_girl_phone_call_vn_scene(girl_id: int = OverlayRules.GIRL_SHIORI,
		response_kind: String = "accepted",
		stats: Array = [97, 43, 40, 40, 38, 33, 59, 4, 3],
		year: int = 95, month: int = 4, day: int = 9,
		weekday: int = 6, day_kind: int = 1,
		bg_uv: Vector2i = Vector2i(20, 14)) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Background: Checkerboard + Holiday room background
	_draw_checkerboard(target, bg_uv)
	if _holiday_room_image != null:
		_compositor._blit(target, _holiday_room_image, null, Vector2i(0, 72), false)

	# 2. Shiori bustup portrait (HU10)
	if _shiori_portrait_image != null:
		_compositor._blit(target, _shiori_portrait_image, null, Vector2i(81, 8), false)

	# 3. Wooden parameter panel & date pillar
	_draw_parameter_panel(target, stats)
	_draw_date_pillar(target, year, month, day, weekday, day_kind)

	# 4. Bottom message window
	_draw_message_window(target)

	# 5. Dialogue text
	var speaker_hex := STR_HEX["speaker_shiori"]
	var line_hex := STR_HEX["call_accept"]
	if response_kind.begins_with("refuse"):
		line_hex = STR_HEX["call_refuse"]
	elif response_kind == "answering_machine":
		speaker_hex = STR_HEX["speaker_yoshio"]
		line_hex = STR_HEX["answering_machine1"]

	_draw_text_hex_checked(target, speaker_hex, Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, line_hex, Vector2i(88, 170), 0, "message_window")

	# Hand cursor / prompt indicator
	_draw_hand_cursor(target, Vector2i(270, 198))

	return target


# ==============================================================================
# Internal Drawing Helpers
# ==============================================================================

func _draw_checkerboard(target: Image, bg_uv: Vector2i = Vector2i.ZERO) -> void:
	if _checker_tile == null:
		target.fill(Color8(40, 200, 160))
		return
	var u0: int = bg_uv.x & 31
	var v0: int = bg_uv.y & 31
	for py in range(240):
		var ty: int = (py + v0) & 31
		for px in range(320):
			var tx: int = (px + u0) & 31
			target.set_pixelv(Vector2i(px, py), _checker_tile.get_pixel(tx, ty))

func _draw_cal_base(target: Image, rect: Rect2i) -> void:
	# Exact PS1 POLY_G4 case 2 (prim_gradient_rect with arg5=0xE0E0E0, arg6=2 in gpu_primitives.c:225-230):
	# v0 = v1 = v2 = 224, v3 = 179 (224 * 8 / 10).
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
			var fx: float = float(x) / float(rect.size.x)
			var base_v: float = 224.0 if (fx + fy <= 1.0) else (224.0 - (fx + fy - 1.0) * 45.0)
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var v5: int = clampi((int(base_v) + d) >> 3, 0, 31)
			var v8: int = v5 << 3
			target.set_pixelv(Vector2i(px, py), Color8(v8, v8, v8, 255))

func _draw_parameter_panel(target: Image, stats: Array) -> void:
	# Gradient box: Rect2i(28, 23, 264, 42)
	_compositor._gradient(target, Rect2i(28, 23, 264, 42))
	# Frame 61 (wooden parameter frame) at (0, -72)
	_draw_actor(target, 61, Vector2i(0, -72))

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

	var var_name := "hud-44" if (weekday == 6 or day_kind in [1, 2]) else ("hud-43" if weekday == 5 else "base")
	_draw_actor(target, weekday + 0x52, Vector2i(146, 31), var_name)

func _draw_message_window(target: Image) -> void:
	# Gradient box: Rect2i(27, 166, 266, 52)
	_compositor._gradient(target, Rect2i(27, 166, 266, 52))
	# Frame 62 (wooden message window frame) at (0, 75)
	_draw_actor(target, 62, Vector2i(0, 75))

func _draw_hand_cursor(target: Image, screen_pos: Vector2i) -> void:
	# Frame 156 (hand cursor) bounds=[0, 0, 24, 16], origin=[0, 3]
	_draw_actor(target, 156, Vector2i(screen_pos.x - 160, screen_pos.y - 120))

func _draw_menu_bar_highlight(target: Image, rect: Rect2i) -> void:
	# PS1 POLY_G4 menu bar highlight (0x832F7FAF, w=72, h=14)
	for y in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240: continue
		var fy: float = float(y) / float(rect.size.y)
		for x in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320: continue
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
			var r5: int = clampi((int(fr_rb) + d) >> 3, 0, 31)
			var g5: int = clampi((int(fr_g) + d) >> 3, 0, 31)
			var b5: int = clampi((int(fr_rb) + d) >> 3, 0, 31)
			target.set_pixelv(Vector2i(px, py), Color8(r5 << 3, g5 << 3, b5 << 3, 255))

func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i, palette_row: int = 0, region: String = "") -> void:
	var codes: PackedByteArray = hex_code.hex_decode()
	var x_offset: int = 0
	var y_offset: int = 0
	var i: int = 0

	while i < codes.size():
		if codes[i] == 0x20:
			x_offset = 0
			y_offset += 16
			i += 1
			continue
		if i + 1 >= codes.size(): break
		var code_unit := codes.slice(i, i + 2)
		i += 2
		var glyph_res: Dictionary = _font.get_glyph_image(code_unit, palette_row & 7, (palette_row >> 3) & 1)
		var gx_pos: int = pos.x + x_offset
		var gy_pos: int = pos.y + y_offset

		recorded_glyphs.append({"code": code_unit.hex_encode(), "pos": Vector2i(gx_pos, gy_pos), "region": region})

		# Enforce bounding box assertions per Section 12a
		if region == "message_window":
			if gx_pos < 27 or gx_pos + 12 > 293 or gy_pos < 166 or gy_pos + 12 > 218:
				bounding_box_violations.append("Message window glyph %s at (%d, %d) overflows Rect2i(27, 166, 266, 52)" % [code_unit.hex_encode(), gx_pos, gy_pos])
		elif region == "notebook":
			if gx_pos < 0 or gx_pos + 12 > 283 or gy_pos < 33 or gy_pos + 12 > 158:
				bounding_box_violations.append("Notebook glyph %s at (%d, %d) overflows cal_base_show Rect2i(0, 33, 283, 125)" % [code_unit.hex_encode(), gx_pos, gy_pos])

		if glyph_res.get("status") == "glyph":
			var g_img: Image = glyph_res["image"]
			# Shadow suppression for palettes 3, 4, 5, 14, 15
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
