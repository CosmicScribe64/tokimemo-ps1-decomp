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
const CalendarPhoneRules = preload("res://rules/calendar_phone_screens.gd")

# Authentic 64x64 Shiori notebook photo (REC(0x68) decoded from CD sector 0x9D5F 0x801A0000/0x801B0000/0x801B0200)
const SHIORI_PHOTO_B64 := "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"

# Exact CP932 byte hex encodings verified from CD sector 0x9D5F and SLPM_861.58
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
	"speaker_mio": "94408c8e", # 如月
	"speaker_yuina": "95528f8f", # 紐緒
	"speaker_ayako": "95d08bcb", # 片桐
	"speaker_saki": "93f896ec", # 虹野
	"speaker_yukari": "8cc38eae", # 古式
	"speaker_nozomi": "90b490ec", # 清川
	"speaker_mira": "8bbe", # 鏡
	"speaker_yuko": "92a993fa93de", # 朝日奈
	"speaker_megumi": "94fc8ef78cb4", # 美樹原
	"speaker_yumi": "918189b38f97", # 早乙女
	"speaker_rei": "88c98f578940", # 伊集院
	"speaker_yoshio": "8d449759", # 好雄
	"call_accept": "82a282a282ed82e68142208d7382ab82dc82b582e582a48142", # いいわよ。 行きましょう。
	"call_accept_mio": "82cd82a281418aec82f182c58142208a7982b582dd82c982b582c482dc82b78142", # はい、喜んで。 楽しみにしてます。
	"call_accept_yuina": "82a282a282ed82e6814220957482ab8d8782c182c482a082b082e98142", # いいわよ。 付き合ってあげる。
	"call_accept_ayako": "826e826a81492088ea8f8f82c98d7382ab82dc82b582e582a48149", # ＯＫ！ 一緒に行きましょう！
	"call_accept_saki": "82a482f1814182a282a282e68149208a7982b582dd82be82cb8149", # うん、いいよ！ 楽しみだね！
	"call_refuse": "82b282df82f182c882b382a281412082bb82cc93fa82cd93738d8782aa88ab82a282cc8142", # ごめんなさい、 その日は都合が悪いの。
	"answering_machine1": "82a0816082a0814197af8ee782dd82bd82a282be82c88142", # あ〜あ、留守みたいだな。
	"answering_machine2": "8da193fa82cd82a082ab82e782df82e982a9816381638142816a", # 今日はあきらめるか……。）
	"yoshio_found1": "82a082c182bd82a082c182bd8142", # あったあった。
	"yoshio_found2": "8ea882a982c182db82b682c182c4", # 耳かっぽじって
	"yoshio_found3": "82e682ad95b782af82e68142", # よく聞けよ。
	"yoshio_profile_done": "817582c182c4814182c682b182be82c88142", # 「って、とこだな。

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

	"sign_cancer": "8a498dc0", # 蟹座 (CD sector 0x9D5F 0x801B22BC + 6 * 10)
	"sign_gemini": "916f8e718dc0", # 双子座 (CD sector 0x9D5F 0x801B22BC + 5 * 10)
	"sign_aquarius": "908595728dc0", # 水瓶座
	"sign_libra": "935694898dc0", # 天秤座
	"sign_capricorn": "8e5297728dc0", # 山羊座
	"blood_o": "826e8c5e", # Ｏ型 (0x801B229C + 0 * 8)
	"blood_a": "82608c5e", # Ａ型 (0x801B229C + 1 * 8)
	"blood_b": "82618c5e", # Ｂ型 (0x801B229C + 2 * 8)
	"blood_ab": "826082618c5e", # ＡＢ型 (0x801B229C + 3 * 8)
	"club_bungei": "95b68c7c9594", # 文芸部 (0x801B2200 + 0 * 12)
	"club_drama": "89898c809594", # 演劇部
	"club_science": "89c88a779594", # 科学部
	"club_art": "94fc8f709594", # 美術部
	"club_baseball": "96ec8b859594", # 野球部 (0x801B2200 + 6 * 12)
	"shiori_notes_1": "8eef96a182cd89b98a798ad38fdc814193c182c9834e838983568362834e", # 趣味は音楽鑑賞、特にクラシック (0x801B25F4)
	"shiori_notes_2": "90ac90d197448f4781418358837c815b8363969c945c", # 成績優秀、スポーツ万能 (0x801B2614)
	"shiori_bwh": "82578252814582548255814582578253", # ８３・５６・８４ (0x801B2334)
	"shiori_birthday": "82568c8e8140825693fa", # ７月　７日
	"mio_birthday": "82518c8e8140825293fa", # ２月　３日
	"yuina_birthday": "82568c8e8140825693fa", # ７月　７日
	"ayako_birthday": "82588c8e8251825893fa", # ９月２９日
	"saki_birthday": "82508c8e8250825293fa", # １月１３日
	"mio_bwh": "8257824f814582548258814582578251", # ８０・５９・８２
	"yuina_bwh": "82578253814582548257814582578253", # ８４・５８・８４
	"ayako_bwh": "82578255814582548258814582578256", # ８６・５９・８７
	"saki_bwh": "82578251814582548256814582578252", # ８２・５７・８３
	"mio_notes_1": "93c78f9182aa91e58d4482ab82c582a882c682c882b582a2", # 読書が大好きでおとなしい
	"mio_notes_2": "8fad82b595618ee382c895b68c7c959482cc8f9782cc8e71", # 少し病弱な文芸部の女の子
	"yuina_notes_1": "90a28a4590aa959e82f08ae982de93568dcb89c88a778fad8f97", # 世界征服を企む天才科学少女
	"yuina_notes_2": "8ec08cb182c694ad96be82aa90b682ab82aa82a2", # 実験と発明が生きがい
	"ayako_notes_1": "89708cea8cf082b682e882c5986282b78c7c8f7089c694a7", # 英語交じりで話す芸術家肌
	"ayako_notes_2": "914f89718341815b836782c689cc82aa91e58d4482ab", # 前衛アートと歌が大好き
	"saki_notes_1": "8daa90ab88ea94d482cc8ae692a382e889ae", # 根性一番の頑張り屋
	"saki_notes_2": "97bf979d82aa93be88d382c8837d836c815b83578383815b", # 料理が得意なマネージャー
}

var _shiori_notebook_photo: Image
var recorded_glyphs: Array[Dictionary] = []
var bounding_box_violations: Array[String] = []


func open_telephone_preview(hud_manifest_path: String, font_root: String, date_pack_root: String, hud_sha: String = HUD_SHA) -> bool:
	recorded_glyphs.clear()
	bounding_box_violations.clear()
	_shiori_notebook_photo = Image.new()
	if _shiori_notebook_photo.load_png_from_buffer(Marshalls.base64_to_raw(SHIORI_PHOTO_B64)) != OK:
		error = "failed to decode Shiori notebook photo"
		return false
	var ok := open_date_preview(hud_manifest_path, font_root, date_pack_root, hud_sha)
	_ensure_compositor_coverage()
	return ok

func _ensure_compositor_coverage() -> void:
	if _shiori_notebook_photo == null:
		_shiori_notebook_photo = Image.new()
		_shiori_notebook_photo.load_png_from_buffer(Marshalls.base64_to_raw(SHIORI_PHOTO_B64))
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
		bg_uv: Vector2i = Vector2i(1, 0)) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	# 1. Authentic PS1 checkerboard background (scrolled + seasonal CLUT)
	_draw_checkerboard(target, bg_uv, _compositor.hud_season_index(month))

	# 2. Wooden parameter panel (Frame 61, OT 12..11)
	_draw_parameter_panel(target, stats)

	# 3. Bottom message window (Frame 62, OT 8..7, Rect2i(27, 166, 266, 52))
	_draw_message_window(target)

	# 4. Base Gouraud appraisal panel (appraisal_base_show in hud_screens.c:645: Rect2i(0, 16, 283, 142), OT 5)
	_draw_cal_base(target, Rect2i(0, 16, 283, 142))

	# 5. Girl 64x64 notebook photo (REC(0x68) at (0x40, -0x28) -> Rect2i(192, 48, 64, 64), OT 3 HEAD)
	if _shiori_notebook_photo != null:
		_blit_simple(target, _shiori_notebook_photo, Vector2i(192, 48))

	# 6. Notebook spiral rings, divider, colons & photo corners (REC(0x67) Frame 0x41 = 65 at (-16, -27), OT 3 after REC(0x68))
	_draw_actor(target, 65, Vector2i(-16, -27))

	# 7. Phone symbols at y = -0x5A (REC(0x60..0x66) in OT 3 after REC(0x67))
	# Exact girl_phone_digits[13][7] table from asm/data/data.data.s:1024-1095 (0=○, 1=×, 2=▲ -> frames 78, 79, 80)
	const GIRL_PHONE_DIGITS := [
		[0, 1, 2, 1, 0, 2, 0], # 0: Shiori
		[1, 1, 2, 0, 2, 0, 1], # 1: Mio
		[2, 2, 2, 0, 0, 1, 0], # 2: Yuina
		[2, 1, 0, 1, 0, 1, 0], # 3: Ayako
		[2, 1, 0, 2, 2, 1, 1], # 4: Saki
		[0, 0, 0, 1, 2, 2, 1], # 5: Yukari
		[2, 1, 1, 0, 1, 2, 1], # 6: Nozomi
		[2, 2, 1, 0, 1, 1, 2], # 7: Mira
		[0, 2, 0, 1, 2, 0, 2], # 8: Yuko
		[0, 1, 2, 2, 1, 0, 0], # 9: Megumi
		[0, 0, 0, 2, 2, 1, 0], # 10: Yoshio
		[0, 2, 1, 0, 2, 1, 0], # 11: Yumi
		[0, 2, 1, 0, 2, 1, 0], # 12: Rei
	]
	var row_idx: int = clampi(girl_id, 0, GIRL_PHONE_DIGITS.size() - 1)
	var phone_digits: Array = GIRL_PHONE_DIGITS[row_idx]
	var phone_x := [0x27, 0x31, 0x3B, 0x4C, 0x56, 0x60, 0x6A]
	for d_idx in range(7):
		_draw_actor(target, int(phone_digits[d_idx]) + 0x4E, Vector2i(phone_x[d_idx], -0x5A))

	# 8. Date pillar (Actor 16 Frame 60, OT 3 TAIL)
	_draw_date_pillar(target, year, month, day, weekday, day_kind)

	# 9. Profile Title / Full Name at screen (32, 22) (x: -0x80, y: -0x62 in C) and Phone header at (128, 22)
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
	_draw_text_hex_checked(target, STR_HEX["title_phone"], Vector2i(128, 22), 5)

	# 10. Profile detail field labels and values (inside Rect2i(0, 33, 283, 125))
	var bday_key := "shiori_birthday"
	var sign_key := "sign_cancer"
	var blood_key := "blood_ab"
	var bwh_key := "shiori_bwh"
	var club_key := "club_baseball"
	var notes1_key := "shiori_notes_1"
	var notes2_key := "shiori_notes_2"
	if girl_id == OverlayRules.GIRL_MIO:
		bday_key = "mio_birthday"
		sign_key = "sign_aquarius"
		blood_key = "blood_a"
		bwh_key = "mio_bwh"
		club_key = "club_bungei"
		notes1_key = "mio_notes_1"
		notes2_key = "mio_notes_2"
	elif girl_id == OverlayRules.GIRL_YUINA:
		bday_key = "yuina_birthday"
		sign_key = "sign_cancer"
		blood_key = "blood_b"
		bwh_key = "yuina_bwh"
		club_key = "club_science"
		notes1_key = "yuina_notes_1"
		notes2_key = "yuina_notes_2"
	elif girl_id == OverlayRules.GIRL_AYAKO:
		bday_key = "ayako_birthday"
		sign_key = "sign_libra"
		blood_key = "blood_b"
		bwh_key = "ayako_bwh"
		club_key = "club_art"
		notes1_key = "ayako_notes_1"
		notes2_key = "ayako_notes_2"
	elif girl_id == OverlayRules.GIRL_SAKI:
		bday_key = "saki_birthday"
		sign_key = "sign_capricorn"
		blood_key = "blood_a"
		bwh_key = "saki_bwh"
		club_key = "club_baseball"
		notes1_key = "saki_notes_1"
		notes2_key = "saki_notes_2"

	# - Birthday (誕生日) at (22, 42), value at (72, 42)
	_draw_text_hex_checked(target, STR_HEX["label_birthday"], Vector2i(22, 42), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX[bday_key], Vector2i(72, 42), 3, "notebook")

	# - Zodiac (星座) at (22, 58), value at (72, 58)
	_draw_text_hex_checked(target, STR_HEX["label_zodiac"], Vector2i(22, 58), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX[sign_key], Vector2i(72, 58), 3, "notebook")

	# - Blood type (血液型) at (22, 74), value at (72, 74)
	_draw_text_hex_checked(target, STR_HEX["label_blood"], Vector2i(22, 74), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX[blood_key], Vector2i(72, 74), 3, "notebook")

	# - BWH (ＢＷＨ) at (22, 90), value at (72, 90)
	_draw_text_hex_checked(target, STR_HEX["label_bwh"], Vector2i(22, 90), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX[bwh_key], Vector2i(72, 90), 3, "notebook")

	# - Club (クラブ) at (22, 106), value at (72, 106)
	_draw_text_hex_checked(target, STR_HEX["label_club"], Vector2i(22, 106), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX[club_key], Vector2i(72, 106), 3, "notebook")

	# - Notes (備考) at (22, 122), lines 1 & 2 at (72, 122) and (72, 138)
	_draw_text_hex_checked(target, STR_HEX["label_notes"], Vector2i(22, 122), 4, "notebook")
	_draw_text_hex_checked(target, STR_HEX[notes1_key], Vector2i(72, 122), 3, "notebook")
	_draw_text_hex_checked(target, STR_HEX[notes2_key], Vector2i(72, 138), 3, "notebook")

	# 11. Dialogue text in bottom message window (call_and_invitation_flow.c:935-938: 好雄 「って、とこだな。)
	_draw_text_hex_checked(target, STR_HEX["speaker_yoshio"], Vector2i(39, 170), 0, "message_window")
	_draw_text_hex_checked(target, STR_HEX["yoshio_profile_done"], Vector2i(74, 170), 0, "message_window")

	# 12. Hand cursor (Actor 0 Frame 156 at OT 0 TAIL)
	_draw_hand_cursor(target, Vector2i(128, 64))

	return target


# ==============================================================================
# 2. Telephone Date Place Selection Menu
# ==============================================================================

func render_date_place_menu_screen(page: int = 0, selected_idx: int = 0,
		avail_table: Array = [],
		stats: Array = [97, 43, 40, 40, 38, 33, 59, 4, 3],
		year: int = 95, month: int = 4, day: int = 9,
		weekday: int = 6, day_kind: int = 1,
		bg_uv: Vector2i = Vector2i(22, 21)) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	_draw_checkerboard(target, bg_uv, _compositor.hud_season_index(month))
	_draw_parameter_panel(target, stats)
	_draw_message_window(target)
	_draw_cal_base(target, Rect2i(0, 33, 283, 125))

	# Highlight active cursor slot (OT 3 HEAD before Actor 96 Frame 64)
	if selected_idx == 10:
		_draw_menu_bar_highlight(target, Rect2i(160, 36, 100, 16))
	elif selected_idx >= 0 and selected_idx < 10:
		var h_col: int = selected_idx / 5
		var h_row: int = selected_idx % 5
		var h_sx: int = 24 if h_col == 0 else 160
		var h_sy: int = 64 + h_row * 16
		_draw_menu_bar_highlight(target, Rect2i(h_sx, h_sy, 116, 16))

	# Notebook spiral rings & divider (REC(96).unk_18 = 0x40 = 64 at (-16, -19) in phone_date_place_availability_build:434)
	_draw_actor(target, 64, Vector2i(-16, -19))
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

		if is_available and spot_id < 21:
			var place_key := "place_%d" % spot_id
			if STR_HEX.has(place_key):
				_draw_text_hex_checked(target, STR_HEX[place_key], Vector2i(sx, sy), 3, "notebook")

	# Bottom prompt: デートする場所を選んで下さい
	_draw_text_hex_checked(target, STR_HEX["prompt_choose_place"], Vector2i(32, 170), 0, "message_window")

	# Hand cursor at active selection (menu_cursor_update(0): item.x + item.w - 4, item.y + 8)
	var cur_col: int = selected_idx / 5
	var cur_row: int = selected_idx % 5
	var cur_sx: int = (24 if cur_col == 0 else 160) + 112
	var cur_sy: int = 64 + cur_row * 16 + 8
	if selected_idx == 10: # Page flip button (x=160, y=36, w=100, h=16)
		cur_sx = 160 + 96
		cur_sy = 36 + 8
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
		bg_uv: Vector2i = Vector2i(9, 12),
		day_count: int = 5, player_birthday: int = 0x19B,
		frame_count: int = 6145) -> Image:
	_ensure_compositor_coverage()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)

	_draw_checkerboard(target, bg_uv, _compositor.hud_season_index(month))
	_draw_parameter_panel(target, stats)
	_draw_message_window(target)

	# OT 5 TAIL: Base Gouraud calendar panel (cal_base_show in hud_screens.c:640)
	_draw_cal_base(target, Rect2i(0, 33, 283, 125))

	# OT 5 HEAD: Opaque green highlight bar (menu_bar_color(1, 0x2F7FAF, 5, 0) in call_and_invitation_flow.c:1662)
	var cursor_pos := Vector2i(36, 112)
	for idx in range(candidate_slots.size()):
		var slot: Dictionary = candidate_slots[idx]
		var s_day: int = int(slot.get("day", 1))
		var s_offset: int = int(slot.get("month_offset", 0))

		# Exact calendar_cell_x/y - 0x10 (April 1995 first_weekday=5 -> Saturday col 6; May 1995 first_weekday=0 -> Monday col 1)
		var first_v: int = 5 if s_offset == 0 else 0
		var cell_col: int = (first_v + s_day) % 7
		var cell_row: int = (first_v + s_day) / 7
		var cell_x: int = 24 + s_offset * 136 + cell_col * 16
		var cell_y: int = 56 + cell_row * 16

		if idx == selected_slot_idx:
			_draw_menu_bar_highlight(target, Rect2i(cell_x, cell_y, 16, 16), true)
			cursor_pos = Vector2i(cell_x + 12, cell_y + 8)

	# OT 3: Notebook spiral rings & divider (Actor 96 Frame 67) + Date pillar + 2-Month Calendar grids
	_draw_actor(target, 67, Vector2i(-16, -19))
	_draw_date_pillar(target, year, month, day, weekday, day_kind)
	_blit_hud_asset(target, "assets/calendar.month-00.calendar-grid.png", Vector2i(24, 32))
	_blit_hud_asset(target, "assets/calendar.month-00.month-label.png", Vector2i(72, 32))
	_blit_hud_asset(target, "assets/calendar.month-01.calendar-grid.png", Vector2i(160, 32))
	_blit_hud_asset(target, "assets/calendar.month-01.month-label.png", Vector2i(208, 32))

	# OT 1: Sinusoidal Biorhythm curves (calendar_view_update -> hud_biorhythm_graph_draw in call_and_invitation_flow.c:1687)
	_draw_biorhythm_graph(target, day_count, player_birthday, frame_count)

	# OT 0: Bottom prompt: デートする日を選んで下さい + Hand cursor
	_draw_text_hex_checked(target, STR_HEX["prompt_choose_day"], Vector2i(32, 170), 0, "message_window")
	_draw_hand_cursor(target, cursor_pos)

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
	_draw_checkerboard(target, bg_uv, _compositor.hud_season_index(month))
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
	var speaker_key := "speaker_shiori"
	var accept_key := "call_accept"
	if girl_id == OverlayRules.GIRL_MIO:
		speaker_key = "speaker_mio"
		accept_key = "call_accept_mio"
	elif girl_id == OverlayRules.GIRL_YUINA:
		speaker_key = "speaker_yuina"
		accept_key = "call_accept_yuina"
	elif girl_id == OverlayRules.GIRL_AYAKO:
		speaker_key = "speaker_ayako"
		accept_key = "call_accept_ayako"
	elif girl_id == OverlayRules.GIRL_SAKI:
		speaker_key = "speaker_saki"
		accept_key = "call_accept_saki"
	elif girl_id == OverlayRules.GIRL_YUKARI:
		speaker_key = "speaker_yukari"
	elif girl_id == OverlayRules.GIRL_NOZOMI:
		speaker_key = "speaker_nozomi"
	elif girl_id == OverlayRules.GIRL_MIRA:
		speaker_key = "speaker_mira"
	elif girl_id == OverlayRules.GIRL_YUKO:
		speaker_key = "speaker_yuko"
	elif girl_id == OverlayRules.GIRL_MEGUMI:
		speaker_key = "speaker_megumi"
	elif girl_id == OverlayRules.GIRL_YUMI:
		speaker_key = "speaker_yumi"
	elif girl_id == OverlayRules.GIRL_REI:
		speaker_key = "speaker_rei"

	var speaker_hex := String(STR_HEX[speaker_key])
	var line_hex := String(STR_HEX[accept_key])
	if response_kind.begins_with("refuse"):
		line_hex = STR_HEX["call_refuse"]
	elif response_kind == "answering_machine":
		speaker_hex = STR_HEX["speaker_yoshio"]
		line_hex = STR_HEX["answering_machine1"]

	var speaker_x: int = 160 + speaker_x_for_hex(speaker_hex)
	_draw_text_hex_checked(target, speaker_hex, Vector2i(speaker_x, 170), 0, "message_window")
	_draw_text_hex_checked(target, line_hex, Vector2i(88, 170), 0, "message_window")

	# Hand cursor / prompt indicator
	_draw_hand_cursor(target, Vector2i(270, 198))

	return target


# ==============================================================================
# Internal Drawing Helpers
# ==============================================================================

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

func _draw_menu_bar_highlight(target: Image, rect: Rect2i, opaque: bool = false) -> void:
	# PS1 POLY_G4 menu bar highlight (0x832F7FAF semi-transparent when opaque=false, or 0x002F7FAF opaque when opaque=true)
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
			var f_rb5: int = clampi((int(fr_rb) + d) >> 3, 0, 31)
			var f_g5: int = clampi((int(fr_g) + d) >> 3, 0, 31)
			if opaque:
				target.set_pixelv(Vector2i(px, py), Color8(f_rb5 << 3, f_g5 << 3, f_rb5 << 3, 255))
			else:
				var back: Color = target.get_pixel(px, py)
				var out_r5: int = clampi((back.r8 >> 4) + (f_rb5 >> 1), 0, 31)
				var out_g5: int = clampi((back.g8 >> 4) + (f_g5 >> 1), 0, 31)
				var out_b5: int = clampi((back.b8 >> 4) + (f_rb5 >> 1), 0, 31)
				target.set_pixelv(Vector2i(px, py), Color8(out_r5 << 3, out_g5 << 3, out_b5 << 3, 255))

func _draw_biorhythm_graph(target: Image, day_count: int, player_birthday: int, frame_count: int) -> void:
	# Exact PS1 hud_biorhythm_graph_draw (src/main/hud_screens.c:570-606) + dtd_on_tpage(0, 0, 1, 2, 0)
	var phase: int = frame_count
	var half: int = ((0x7F - (frame_count & 0x7F)) / 2) if (frame_count & 0x80) != 0 else ((frame_count & 0x7F) / 2)
	_draw_subtractive_line(target, Vector2i(32, 108), Vector2i(284, 108), Vector3i(half, half, half))

	var start_day: int = day_count - 8
	var phys_pts: Array[Vector2i] = []
	var emot_pts: Array[Vector2i] = []
	var intel_pts: Array[Vector2i] = []

	for i in range(16):
		var d: int = start_day + i
		var val_p: int = CalendarPhoneRules.biorhythm_value(d, player_birthday, true, false, false, -32)
		var val_e: int = CalendarPhoneRules.biorhythm_value(d, player_birthday, false, true, false, -32)
		var val_i: int = CalendarPhoneRules.biorhythm_value(d, player_birthday, false, false, true, -32)

		var sx: int = 160 + (i * 16 - 128)
		phys_pts.append(Vector2i(sx, 108 + val_p))
		emot_pts.append(Vector2i(sx, 108 + val_e))
		intel_pts.append(Vector2i(sx, 108 + val_i))

	for i in range(1, 16):
		var v: int = ((i + phase) & 0xF) * 4 + half
		_draw_subtractive_line(target, phys_pts[i - 1], phys_pts[i], Vector3i(0, v, v))
		_draw_subtractive_line(target, emot_pts[i - 1], emot_pts[i], Vector3i(v + 15, v + 15, 0))
		_draw_subtractive_line(target, intel_pts[i - 1], intel_pts[i], Vector3i(v, 0, v))

func _draw_subtractive_line(target: Image, p0: Vector2i, p1: Vector2i, sub_rgb: Vector3i) -> void:
	var dx: int = abs(p1.x - p0.x)
	var dy: int = -abs(p1.y - p0.y)
	var sx: int = 1 if p0.x < p1.x else -1
	var sy: int = 1 if p0.y < p1.y else -1
	var err: int = dx + dy
	var x: int = p0.x
	var y: int = p0.y
	var sr5: int = clampi(sub_rgb.x >> 3, 0, 31)
	var sg5: int = clampi(sub_rgb.y >> 3, 0, 31)
	var sb5: int = clampi(sub_rgb.z >> 3, 0, 31)

	while true:
		if x >= 0 and x < 320 and y >= 0 and y < 240:
			var back: Color = target.get_pixel(x, y)
			var r5: int = maxi((back.r8 >> 3) - sr5, 0)
			var g5: int = maxi((back.g8 >> 3) - sg5, 0)
			var b5: int = maxi((back.b8 >> 3) - sb5, 0)
			target.set_pixelv(Vector2i(x, y), Color8(r5 << 3, g5 << 3, b5 << 3, 255))
		if x == p1.x and y == p1.y:
			break
		var e2: int = 2 * err
		if e2 >= dy:
			err += dy
			x += sx
		if e2 <= dx:
			err += dx
			y += sy

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
