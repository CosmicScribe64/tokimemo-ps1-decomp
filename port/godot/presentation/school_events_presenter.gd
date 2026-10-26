class_name SchoolEventsPresenter
extends "res://presentation/command_hud_presenter.gd"
## T-29018 Lane 6: Authentic 320x240 PS1 presentation and rendering for:
## 1. Sports Festival Event Selection & Race HUD (体育祭)
## 2. Culture Festival Booth / Hallway Tour (文化祭)
## 3. School Trip Destination & Companion Prompt (修学旅行)
## 4. Club Practice Match / Summer Camp (練習試合 / 夏合宿)
##
## Enforces:
## - Exact CP932 hex byte encodings (computed via Python, never guessed)
## - Zero glyph bounding-box violations
## - Authentic PS1 HUD sprites (Frame 62 message window, Frame 113..115 title badge, Frame 156 hand cursor)

const HUD_SHA := "0a884e91775f48dc7369b206234bc92d0f236df7e9aa4a2d1c47535d8e498b88"
const RUNNER_INPUT_SPEED_DEFAULT := 0x1000

# Authentic PS1 32x32 green checkerboard tile (hud_screen_frame_draw, hud_panels.c:1366)
const CHECKER_TILE_B64 := "iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAA5ElEQVR4nMWWsQ3DIBBFzycXlIzgUTIKZUZI6VEYI6XHcHkjpKRLdBRIECQMTvx/aSPeQ0L8m2h1byoij5UkCI1GXkLOb4fWTl78l8BiF7rv4wL7cyPvbofWMoHD5QdjzKkNQwjjAuZieCaAgCcBFDwKIOFRAAnXwzMSrmEkvCrQuhMteOt/uT8j4ZkAAp4EUPAogIRrZku22udaqaPpmSfmWu/39HlVIMjheYIJHP7nI9MtEC6GZwIIeBJAwaMAEh4FkHA9/LDAryYpRsKrAmff9t5uYSQ8E0DAkwAKHgWQcBX4AGcPlHlFD2CfAAAAAElFTkSuQmCC"

# Exact CP932 byte hex encodings strictly computed via Python .encode("cp932").hex()
const STR_HEX := {
	"title_sports": "91cc88e78dd5",                          # 体育祭
	"title_culture": "95b689bb8dd5",                         # 文化祭
	"title_trip": "8f438a7797b78d73",                        # 修学旅行
	"title_practice": "97fb8f4b8e8e8d87",                    # 練習試合
	"title_camp": "89c48d878f68",                            # 夏合宿
	"folk_dance": "83748348815b834e835f83938358",            # フォークダンス
	"dash_100m": "8250824f824f828d9196",                     # １００ｍ走
	"obstacle_race": "8fe18a5195a88ba39196",                 # 障害物競走
	"three_legged": "93f1906c8e4f8b72",                      # 二人三脚
	"bread_eating": "83708393904882a28ba39196",              # パン食い競走
	"place_1st": "825088ca",                                 # １位
	"place_2nd": "825188ca",                                 # ２位
	"place_3rd": "825288ca",                                 # ３位
	"place_4th": "825388ca",                                 # ４位
	"dest_kyoto": "8b9e9373",                                # 京都
	"dest_okinawa": "89ab93ea",                              # 沖縄
	"dest_hokkaido": "966b8a4393b9",                          # 北海道
	"group_activity": "8179926391cc8d7393ae9286817a",        # 【団体行動中】
	"exhibit_visit": "93578ea68ca98a77",                     # 展示見学
	"class_booth": "96cd8b5b9358",                           # 模擬店
	"club_drama": "89898c8095948cf68989",                    # 演劇部公演
	"club_brass": "908191748a79959489899174",                # 吹奏楽部演奏
	"club_science": "89c88a77959493578ea6",                  # 科学部展示
	"club_computer": "9364945d959493578ea6",                 # 電脳部展示
	"club_literature": "95b68c7c959483588373815b8360",       # 文芸部スピーチ
	"club_baseball": "96ec8b859594",                         # 野球部
	"club_soccer": "83548362834a815b9594",                   # サッカー部
	"club_tennis": "8365836a83589594",                       # テニス部
	"club_swimming": "9085896a9594",                         # 水泳部
	"club_basketball": "836f835883509594",                   # バスケ部
	"night_stalls": "898f93fa",                              # 縁日
	"stall_shooting": "8ecb9349",                            # 射的
	"stall_goldfish": "8be08b9b82b782ad82a2",                # 金魚すくい
	"stall_ring": "97d6938a82b0",                            # 輪投げ
	"prompt_tour": "88ea8f8f82c989f182eb82a482e6",          # 一緒に回ろうよ
	"prompt_who": "924e82f0975582a882a482a982c88148",        # 誰を誘おうかな？
	"prompt_go_together": "88ea8f8f82c98d7382b182a48149",    # 一緒に行こう！
	"response_yes": "82a682a6814182a282a282ed82e6",          # ええ、いいわよ
	"response_sorry": "82b282df82f182c882b382a28163",        # ごめんなさい…
	"match_ongoing": "8e8e8d879286",                         # 試合中
	"match_victory": "8f9f97988149",                         # 勝利！
	"match_defeat": "9473966b8163",                          # 敗北…
	"practice_result": "97fb8f4b90ac89ca",                   # 練習成果
	"label_stamina": "8358835e837e8369",                     # スタミナ
	"label_speed": "83588373815b8368",                       # スピード
	"label_time": "835e83438380",                            # タイム
	"label_rank": "8f8788ca",                                # 順位
	"label_score": "93be935f",                               # 得点
	"girl_shiori": "93a18de88e8d9044",                       # 藤崎詩織
	"girl_mio": "94408c8e96a28f8f",                          # 如月未緒
	"girl_yuina": "95528f8f8c8b93de",                        # 紐緒結奈
	"girl_ayako": "95d08bcb8dca8e71",                        # 片桐彩子
	"girl_saki": "93f896ec8db98af3",                         # 虹野沙希
	"girl_yukari": "8cc38eae82e482a982e8",                   # 古式ゆかり
	"girl_nozomi": "90b490ec965d",                           # 清川望
	"girl_mira": "8bbe96a39785",                             # 鏡魅羅
	"girl_yuko": "92a993fa93de975b8e71",                     # 朝日奈夕子
	"girl_megumi": "94fc8ef78cb488a4",                       # 美樹原愛
	"girl_yumi": "918189b38f97974494fc",                     # 早乙女優美
	"girl_miharu": "8ad997d18ca990b0",                       # 館林見晴
	"girl_ray": "88c98f578940838c8343",                      # 伊集院レイ
}

var _checker_tile: Image
var _shiori_portrait_image: Image
var _saki_portrait_image: Image
var _sports_track_bg: Image
var _culture_hallway_bg: Image
var _school_trip_kyoto_bg: Image
var _practice_baseball_bg: Image
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

## Initializes font and HUD manifest resources
func init_resources(font_root: String, hud_manifest_path: String = "/workspace/.asset-cache/port/hud-production-v1/manifest.json") -> String:
	recorded_glyphs.clear()
	bounding_box_violations.clear()

	_checker_tile = Image.new()
	var err: Error = _checker_tile.load_png_from_buffer(Marshalls.base64_to_raw(CHECKER_TILE_B64))
	if err != OK:
		return "Failed to decode checkerboard background tile"

	_shiori_portrait_image = _load_first_existing_image(".asset-cache/port/date-preview-v1/portraits/shiori_hu10/shiori_hu10_21938.static-frame.png")
	_saki_portrait_image = _load_first_existing_image(".asset-cache/media/color-corpus/assets/asset-a7381e4092cd55c0303a0ff5b975e606f237c63b0734d82928d3a8f0cec08d9f/asset-a7381e4092cd55c0303a0ff5b975e606f237c63b0734d82928d3a8f0cec08d9f.static-frame.png")
	_sports_track_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-4158af293c4b912c914e8eec7c0041a5fa2d3915ca5daf59856d6bba08e5227f/asset-4158af293c4b912c914e8eec7c0041a5fa2d3915ca5daf59856d6bba08e5227f.background.png")
	_culture_hallway_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-46a94b7f32df30001822781ffe069ae871c7ab83088fc2d1d5cfe017ed8b69d5/asset-46a94b7f32df30001822781ffe069ae871c7ab83088fc2d1d5cfe017ed8b69d5.background.png")
	_school_trip_kyoto_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-660077ec1e7ddc9ddf814666990ba229de4d1b7e22e1fe57173f6243b78743d5/asset-660077ec1e7ddc9ddf814666990ba229de4d1b7e22e1fe57173f6243b78743d5.background.png")
	_practice_baseball_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-7fea10a955a7b843b21ebe43c03bb2f2455fb53c4df4916ca6f5dad273708c03/asset-7fea10a955a7b843b21ebe43c03bb2f2455fb53c4df4916ca6f5dad273708c03.background.png")

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

func _get_girl_name_hex(girl_id: int) -> String:
	match girl_id:
		1: return STR_HEX["girl_mio"]
		2: return STR_HEX["girl_yuina"]
		3: return STR_HEX["girl_ayako"]
		4: return STR_HEX["girl_saki"]
		5: return STR_HEX["girl_yukari"]
		6: return STR_HEX["girl_nozomi"]
		7: return STR_HEX["girl_mira"]
		8: return STR_HEX["girl_yuko"]
		9: return STR_HEX["girl_megumi"]
		10: return STR_HEX["girl_yumi"]
		11: return STR_HEX["girl_miharu"]
		12: return STR_HEX["girl_ray"]
		_: return STR_HEX["girl_shiori"]

# ==============================================================================
# Scene 1: Sports Festival Event Selection & Race HUD (体育祭)
# ==============================================================================
func render_sports_festival_hud(state: Dictionary = {}) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Extracted 320x160 PS1 Sports Festival Track Background (CDROM/BS/D_01_81B.BS)
	_draw_athletic_track_background(target)
	if _shiori_portrait_image != null and _compositor != null:
		_compositor._blit(target, _shiori_portrait_image, null, Vector2i(81, 8), false)

	# 2. Top-left wooden title badge: 体育祭 (OT 2)
	_draw_title_badge(target, STR_HEX["title_sports"], Vector2i.ZERO)

	# 3. Bottom message window (Rect2i(27, 166, 266, 52), Month 10 Autumn)
	_draw_message_window(target, 10, Vector2i(14, 8))

	if state.has("cursor"):
		var cur_row: int = clampi(int(state["cursor"]), 0, 2)
		_draw_menu_bar_highlight(target, Rect2i(32, 168 + cur_row * 16, 252, 16))

	var place: int = int(state.get("player_place", 1))
	var place_hex: String = STR_HEX["place_1st"]
	if place == 2: place_hex = STR_HEX["place_2nd"]
	elif place == 3: place_hex = STR_HEX["place_3rd"]
	elif place >= 4: place_hex = STR_HEX["place_4th"]

	var girl_hex: String = _get_girl_name_hex(int(state.get("girl_id", 0)))
	var comp: int = int(state.get("competition", 0))
	var comp_hex: String = STR_HEX["dash_100m"]
	if comp == 1: comp_hex = STR_HEX["obstacle_race"]
	elif comp == 2: comp_hex = STR_HEX["three_legged"]
	elif comp == 3: comp_hex = STR_HEX["bread_eating"]

	var comp_x: int = 112 if girl_hex.length() > 16 else 104
	var place_x: int = maxi(192, comp_x + (comp_hex.length() / 4) * 14 + 6)
	_draw_text_hex_checked(target, girl_hex, Vector2i(38, 170), 0, "message_window")
	_draw_text_hex_checked(target, comp_hex, Vector2i(comp_x, 170), 0, "message_window")
	_draw_text_hex_checked(target, place_hex, Vector2i(place_x, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["label_speed"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["label_stamina"], Vector2i(164, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["prompt_go_together"], Vector2i(88, 202), 0, "message_window")
	_draw_actor(target, 1, Vector2i(276 - 160, 192 - 120))

	return target

# ==============================================================================
# Scene 2: Culture Festival Booth / Tour (文化祭)
# ==============================================================================
func render_culture_festival_booth(state: Dictionary = {}) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Extracted 320x160 PS1 School Hallway Background (CDROM/BS/D_01_70.BS) + Shiori Portrait
	_draw_school_hallway_background(target)
	if _shiori_portrait_image != null and _compositor != null:
		_compositor._blit(target, _shiori_portrait_image, null, Vector2i(81, 8), false)

	# 2. Top-left wooden title badge: 文化祭 (OT 2)
	_draw_title_badge(target, STR_HEX["title_culture"], Vector2i.ZERO)

	# 3. Bottom message window (Rect2i(27, 166, 266, 52), Month 11 Autumn)
	_draw_message_window(target, 11, Vector2i(16, 10))
	var selected_booth: int = clampi(int(state.get("selected_booth", state.get("cursor", 0))), 0, 2)
	_draw_menu_bar_highlight(target, Rect2i(32, 168 + selected_booth * 16, 252, 16))
	var girl_hex: String = _get_girl_name_hex(int(state.get("girl_id", 0)))
	var tour_x: int = 122 if girl_hex.length() > 16 else 114
	_draw_text_hex_checked(target, STR_HEX["class_booth"], Vector2i(48, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["exhibit_visit"], Vector2i(136, 170), 0, "message_window")
	_draw_text_hex_checked(target, girl_hex, Vector2i(48, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["prompt_tour"], Vector2i(tour_x, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["response_yes"], Vector2i(48, 202), 0, "message_window")
	_draw_hand_cursor(target, Vector2i(276, 176 + selected_booth * 16))

	return target

# ==============================================================================
# Scene 3: School Trip Destination & Companion Prompt (修学旅行)
# ==============================================================================
func render_school_trip_companion(state: Dictionary = {}) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Extracted 320x160 PS1 Kyoto Temple Background (CDROM/BS/D_11_18.BS) + Saki Nijino Portrait (NI10.BIN)
	_draw_scenic_travel_background(target)
	var girl_id: int = int(state.get("girl_id", 4))
	var portrait: Image = _shiori_portrait_image if girl_id == 0 else (_saki_portrait_image if _saki_portrait_image != null else _shiori_portrait_image)
	if portrait != null and _compositor != null:
		_compositor._blit(target, portrait, null, Vector2i(81, 8), false)

	# 2. Top-left wooden title badge: 修学旅行 (OT 2)
	_draw_title_badge(target, STR_HEX["title_trip"], Vector2i.ZERO)

	# 3. Bottom message window (Rect2i(27, 166, 266, 52), Month 9 Autumn)
	_draw_message_window(target, 9, Vector2i(20, 12))
	if state.has("cursor"):
		var cur_row: int = clampi(int(state["cursor"]), 0, 2)
		_draw_menu_bar_highlight(target, Rect2i(32, 168 + cur_row * 16, 252, 16))
	var girl_hex: String = _get_girl_name_hex(girl_id)
	var group_x: int = 112 if girl_hex.length() > 16 else 104
	_draw_text_hex_checked(target, girl_hex, Vector2i(38, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["group_activity"], Vector2i(group_x, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["prompt_go_together"], Vector2i(88, 186), 0, "message_window")
	if state.has("destination"):
		var dest: int = int(state["destination"])
		var dest_hex: String = STR_HEX["dest_kyoto"]
		if dest == 1: dest_hex = STR_HEX["dest_okinawa"]
		elif dest == 2: dest_hex = STR_HEX["dest_hokkaido"]
		_draw_text_hex_checked(target, dest_hex, Vector2i(216, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["response_yes"], Vector2i(88, 202), 0, "message_window")
	_draw_actor(target, 1, Vector2i(276 - 160, 192 - 120))

	return target

# ==============================================================================
# Scene 4: Club Practice Match / Summer Camp (練習試合 / 夏合宿)
# ==============================================================================
func render_club_practice_match_camp(state: Dictionary = {}) -> Image:
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Extracted 320x160 PS1 Baseball Stadium Background (CDROM/BS/D_03_01.BS) + Shiori Portrait
	_draw_sports_field_background(target)
	if _shiori_portrait_image != null and _compositor != null:
		_compositor._blit(target, _shiori_portrait_image, null, Vector2i(81, 8), false)

	# 2. Top-left wooden title badge: 練習試合 (OT 2)
	_draw_title_badge(target, STR_HEX["title_practice"], Vector2i.ZERO)

	var club_hex: String = STR_HEX["club_baseball"]
	var club_id: int = int(state.get("club_id", 6))
	if club_id == 7: club_hex = STR_HEX["club_soccer"]
	elif club_id == 8: club_hex = STR_HEX["club_tennis"]
	elif club_id == 9: club_hex = STR_HEX["club_swimming"]
	elif club_id == 10: club_hex = STR_HEX["club_basketball"]

	var victory: bool = bool(state.get("victory", true))
	var outcome_hex: String = STR_HEX["match_victory"] if victory else STR_HEX["match_defeat"]
	var girl_hex: String = _get_girl_name_hex(int(state.get("girl_id", 0)))
	var club_x: int = 112 if girl_hex.length() > 16 else 104

	# 3. Bottom message window (Rect2i(27, 166, 266, 52), Month 8 Summer)
	_draw_message_window(target, int(state.get("month", 8)), state.get("bg_uv", Vector2i(18, 12)))
	if state.has("cursor"):
		var cur_row: int = clampi(int(state["cursor"]), 0, 2)
		_draw_menu_bar_highlight(target, Rect2i(32, 168 + cur_row * 16, 252, 16))
	_draw_text_hex_checked(target, girl_hex, Vector2i(38, 170), 0, "message_window")
	_draw_text_hex_checked(target, club_hex, Vector2i(club_x, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["practice_result"], Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, outcome_hex, Vector2i(156, 186), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["response_yes"], Vector2i(88, 202), 0, "message_window")
	_draw_actor(target, 1, Vector2i(276 - 160, 192 - 120))

	return target

# ==============================================================================
# Helper Drawing Routines
# ==============================================================================

## Draws authentic PS1 checkerboard (scrolled + seasonal CLUT)
func _draw_checkerboard(target: Image, bg_uv: Vector2i = Vector2i.ZERO, month: int = 10) -> void:
	var tile: Image = _checker_tile
	if _compositor != null:
		var season_tile: Image = _compositor._get_season_checker_tile(_compositor.hud_season_index(month), 255)
		if season_tile != null:
			tile = season_tile
	if tile == null:
		target.fill(Color8(40, 180, 140))
		return
	var u0: int = bg_uv.x & 31
	var v0: int = bg_uv.y & 31
	for py in range(240):
		var ty: int = (py + v0) & 31
		for px in range(320):
			var tx: int = (px + u0) & 31
			target.set_pixelv(Vector2i(px, py), tile.get_pixel(tx, ty))

## Background: Sports Festival Running Track (CDROM/BS/D_01_81B.BS)
func _draw_athletic_track_background(target: Image) -> void:
	_draw_checkerboard(target, Vector2i(14, 8), 10)
	if _sports_track_bg != null:
		target.blit_rect(_sports_track_bg, Rect2i(0, 0, mini(320, _sports_track_bg.get_width()), mini(160, _sports_track_bg.get_height())), Vector2i(0, 0))

## Background: School Hallway (CDROM/BS/D_01_70.BS)
func _draw_school_hallway_background(target: Image) -> void:
	_draw_checkerboard(target, Vector2i(16, 10), 11)
	if _culture_hallway_bg != null:
		target.blit_rect(_culture_hallway_bg, Rect2i(0, 0, mini(320, _culture_hallway_bg.get_width()), mini(160, _culture_hallway_bg.get_height())), Vector2i(0, 0))

## Background: Scenic Travel Vista (CDROM/BS/D_11_18.BS Kyoto)
func _draw_scenic_travel_background(target: Image) -> void:
	_draw_checkerboard(target, Vector2i(20, 12), 9)
	if _school_trip_kyoto_bg != null:
		target.blit_rect(_school_trip_kyoto_bg, Rect2i(0, 0, mini(320, _school_trip_kyoto_bg.get_width()), mini(160, _school_trip_kyoto_bg.get_height())), Vector2i(0, 0))

## Background: Sports Field / Baseball Stadium (CDROM/BS/D_03_01.BS)
func _draw_sports_field_background(target: Image) -> void:
	_draw_checkerboard(target, Vector2i(18, 12), 8)
	if _practice_baseball_bg != null:
		target.blit_rect(_practice_baseball_bg, Rect2i(0, 0, mini(320, _practice_baseball_bg.get_width()), mini(160, _practice_baseball_bg.get_height())), Vector2i(0, 0))

func _draw_actor(target: Image, frame_id: int, pos: Vector2i, variant: String = "base", semi: bool = true) -> void:
	if _compositor == null:
		return
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

## Draws wooden title badge matching message_window_set_title + message_window_title_update
func _draw_title_badge(target: Image, title_hex: String, _origin: Vector2i = Vector2i.ZERO) -> void:
	var n_glyphs: int = title_hex.length() / 4
	if _compositor != null:
		_compositor._gradient(target, Rect2i(16, 16, n_glyphs * 14 + 8, 20))
		for i in range(n_glyphs + 1, -1, -1):
			var fid: int = 113 if i == n_glyphs + 1 else (115 if i == 0 else 114)
			var ax: int = (n_glyphs - i) * 14 - 135 + (2 if i == n_glyphs + 1 else (-2 if i == 0 else 0))
			_draw_actor(target, fid, Vector2i(ax, -95))
	_draw_text_hex_checked(target, title_hex, Vector2i(18, 18), 0, "title_badge")

## Draws authentic PS1 Gouraud-shaded panel
func _draw_gouraud_panel(target: Image, rect: Rect2i, col_top: Color, col_bottom: Color) -> void:
	for y in range(rect.size.y):
		var py := rect.position.y + y
		if py < 0 or py >= 240: continue
		var factor := float(y) / float(rect.size.y)
		var r := int(lerpf(float(col_top.r8), float(col_bottom.r8), factor))
		var g := int(lerpf(float(col_top.g8), float(col_bottom.g8), factor))
		var b := int(lerpf(float(col_top.b8), float(col_bottom.b8), factor))
		var c := Color8(r, g, b)

		for x in range(rect.size.x):
			var px := rect.position.x + x
			if px < 0 or px >= 320: continue
			if x == 0 or y == 0:
				target.set_pixelv(Vector2i(px, py), Color8(255, 255, 255))
			elif x == rect.size.x - 1 or y == rect.size.y - 1:
				target.set_pixelv(Vector2i(px, py), Color8(100, 100, 100))
			else:
				target.set_pixelv(Vector2i(px, py), c)

## Draws bottom message window (hud_screen_frame_draw(2) + Rect2i(27, 166, 266, 52) + Frame 62)
func _draw_message_window(target: Image, month: int = 10, bg_uv: Vector2i = Vector2i(14, 8)) -> void:
	var tile: Image = _checker_tile
	if _compositor != null:
		var season_tile: Image = _compositor._get_season_checker_tile(_compositor.hud_season_index(month), 255)
		if season_tile != null:
			tile = season_tile
	if tile != null:
		var u0: int = bg_uv.x & 31
		var v0: int = bg_uv.y & 31
		for py in range(160, 240):
			var ty: int = (py + v0) & 31
			for px in range(320):
				var tx: int = (px + u0) & 31
				target.set_pixelv(Vector2i(px, py), tile.get_pixel(tx, ty))
	if _compositor != null:
		_compositor._gradient(target, Rect2i(27, 166, 266, 52))
		_draw_actor(target, 62, Vector2i(0, 75))

## Draws semi-transparent green menu highlight bar (menu_bar_show(0): 0x832F7FAF)
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
			var tint_r: int = int(lerpf(50.0, 120.0, fx))
			var tint_g: int = int(lerpf(220.0, 245.0, fy))
			var tint_b: int = int(lerpf(70.0, 140.0, fx))
			var out_r: int = (int(base_col.r8) + tint_r) / 2
			var out_g: int = (int(base_col.g8) + tint_g) / 2
			var out_b: int = (int(base_col.b8) + tint_b) / 2
			target.set_pixelv(Vector2i(px, py), Color8(out_r, out_g, out_b))

## Draws a horizontal progress/status bar
func _draw_progress_bar(target: Image, rect: Rect2i, pct: float, fill_color: Color) -> void:
	for y in range(rect.size.y):
		for x in range(rect.size.x):
			var px := rect.position.x + x
			var py := rect.position.y + y
			if x == 0 or y == 0 or x == rect.size.x - 1 or y == rect.size.y - 1:
				target.set_pixelv(Vector2i(px, py), Color8(60, 60, 60))
			else:
				var fill_width := int(float(rect.size.x - 2) * clampf(pct, 0.0, 1.0))
				if x <= fill_width:
					target.set_pixelv(Vector2i(px, py), fill_color)
				else:
					target.set_pixelv(Vector2i(px, py), Color8(30, 30, 30))

## Draws mini runner lane preview
func _draw_runner_lane_preview(target: Image, rect: Rect2i, track_pos: int) -> void:
	_draw_gouraud_panel(target, rect, Color8(220, 200, 180), Color8(180, 160, 140))
	for lane in range(4):
		var ly: int = rect.position.y + 16 + lane * 18
		for lx in range(rect.size.x - 8):
			target.set_pixelv(Vector2i(rect.position.x + 4 + lx, ly), Color8(240, 240, 240))
		var rx: int = rect.position.x + 8 + ((track_pos / 0x40000 + lane * 4) % (rect.size.x - 16))
		target.set_pixelv(Vector2i(rx, ly - 2), Color8(240, 40, 40) if lane == 0 else Color8(40, 40, 220))
		target.set_pixelv(Vector2i(rx, ly - 1), Color8(240, 40, 40) if lane == 0 else Color8(40, 40, 220))

## Draws companion portrait frame using extracted HU10 portrait when available
func _draw_portrait_placeholder(target: Image, rect: Rect2i, _girl_name: String) -> void:
	_draw_gouraud_panel(target, rect, Color8(230, 220, 210), Color8(190, 180, 170))
	if _shiori_portrait_image != null:
		var sw: int = _shiori_portrait_image.get_width()
		var sh: int = _shiori_portrait_image.get_height()
		var inner := Rect2i(rect.position.x + 2, rect.position.y + 2, rect.size.x - 4, rect.size.y - 4)
		for y in range(inner.size.y):
			var sy: int = clampi((y * mini(sh, 130)) / maxi(1, inner.size.y), 0, sh - 1)
			for x in range(inner.size.x):
				var sx: int = clampi((x * sw) / maxi(1, inner.size.x), 0, sw - 1)
				var pcol: Color = _shiori_portrait_image.get_pixel(sx, sy)
				if pcol.a > 0.1:
					target.set_pixelv(Vector2i(inner.position.x + x, inner.position.y + y), pcol)
		return
	var center_x := rect.position.x + rect.size.x / 2
	var head_y := rect.position.y + 30
	for y in range(rect.size.y - 8):
		for x in range(rect.size.x - 8):
			var px := rect.position.x + 4 + x
			var py := rect.position.y + 4 + y
			var dist := Vector2(px - center_x, py - head_y).length()
			if dist < 18.0:
				target.set_pixelv(Vector2i(px, py), Color8(220, 150, 130))
			elif py > head_y + 12 and absi(px - center_x) < (py - head_y):
				target.set_pixelv(Vector2i(px, py), Color8(70, 90, 150))

## Draws hand cursor (Actor 0 Frame 156)
func _draw_hand_cursor(target: Image, screen_pos: Vector2i) -> void:
	_draw_actor(target, 156, Vector2i(screen_pos.x - 160, screen_pos.y - 120))

# ==============================================================================
# Checked Glyph Drawing and Boundary Validation
# ==============================================================================
func _draw_text_hex_checked(target: Image, hex_code: String, pos: Vector2i, palette_row: int = 0, region: String = "") -> void:
	var codes: PackedByteArray = hex_code.hex_decode()
	var x_offset: int = 0

	for i in range(0, codes.size(), 2):
		if i + 1 >= codes.size(): break
		var code_unit := codes.slice(i, i + 2)
		var glyph_res: Dictionary = _font.get_glyph_image(code_unit, palette_row & 7, (palette_row >> 3) & 1)
		var gx_pos: int = pos.x + x_offset
		var gy_pos: int = pos.y

		recorded_glyphs.append({"code": code_unit.hex_encode(), "pos": Vector2i(gx_pos, gy_pos), "region": region})

		if region == "message_window":
			if gx_pos < 27 or gx_pos + 12 > 293 or gy_pos < 166 or gy_pos + 12 > 218:
				bounding_box_violations.append("Message window glyph %s at (%d, %d) overflows Rect2i(27, 166, 266, 52)" % [code_unit.hex_encode(), gx_pos, gy_pos])
		elif region == "content_box":
			if gx_pos < 20 or gx_pos + 12 > 300 or gy_pos < 40 or gy_pos + 12 > 160:
				bounding_box_violations.append("Content box glyph %s at (%d, %d) overflows Rect2i(20, 40, 280, 120)" % [code_unit.hex_encode(), gx_pos, gy_pos])
		elif region == "title_badge":
			if gx_pos < 8 or gx_pos + 12 > 130 or gy_pos < 6 or gy_pos + 12 > 48:
				bounding_box_violations.append("Title badge glyph %s at (%d, %d) overflows Rect2i(8, 6, 122, 42)" % [code_unit.hex_encode(), gx_pos, gy_pos])

		if glyph_res.get("status") == "glyph":
			var g_img: Image = glyph_res["image"]
			var has_shadow: bool = not (palette_row in [3, 4, 5, 14, 15])
			if has_shadow:
				for gy in range(12):
					var spy: int = gy_pos + gy + 1
					if spy < 0 or spy >= 240: continue
					for gx in range(12):
						var spx: int = gx_pos + gx + 1
						if spx < 0 or spx >= 320: continue
						var pcol: Color = g_img.get_pixel(gx, gy)
						if pcol.a > 0.05:
							target.set_pixel(spx, spy, Color8(0, 0, 0, 255))

			for gy in range(12):
				var dy: int = gy_pos + gy
				if dy < 0 or dy >= 240: continue
				for gx in range(12):
					var dx: int = gx_pos + gx
					if dx < 0 or dx >= 320: continue
					var pcol: Color = g_img.get_pixel(gx, gy)
					if pcol.a > 0.05:
						target.set_pixel(dx, dy, pcol)

		x_offset += 14
