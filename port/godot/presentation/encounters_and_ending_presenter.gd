extends "res://presentation/prologue_presenter.gd"
## T-29018: 320x240 presenter for Lane 3 & 4 overlays:
## 1. After-School Walk Home (GEKO / 下校) 2-choice invitation prompt
##    (k_reset(1), x_taku_menu_set, x_taku_string_set, menu_bar_show, hand cursor).
## 2. Term Exam Results (MODE_EXAM / 結果発表) notebook scoreboard & reaction
##    (cal_base_show / exam_panel_draw_big, exam_panel_draw_header, hizuke_show, message_window_show).
## 3. Valentine's Day (VALEN / バレンタイン) chocolate presentation scene
##    (hud_screen_frame_draw(2), message_window_show, hizuke_show, gnsx, sndi).
## 4. Legendary Tree Confession (ENDING / 卒業式・伝説の樹の下で) scene
##    (hud_screen_frame_draw(2), message_window_show, hizuke_show, gnsx, sndi).

const MESSAGE_WINDOW_INTERIOR_RECT: Rect2i = Rect2i(27, 166, 266, 52)
const NOTEBOOK_INTERIOR_RECT: Rect2i = Rect2i(0, 6, 320, 152)
const DITHER: Array[int] = [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Pre-computed CP932 hex constants via Python .encode("cp932").hex() (Section 12a rule)
const TITLE_GEKO_HEX: String = "89ba8d5a" # 下校 (2 glyphs)
const TITLE_EXAM_RESULTS_HEX: String = "8c8b89ca94ad955c" # 結果発表 (4 glyphs)
const TITLE_VALENTINE_HEX: String = "836f838c8393835e83438393" # バレンタイン (6 glyphs)
const TITLE_ENDING_HEX: String = "91b28bc68eae" # 卒業式 (3 glyphs)

const SPEAKER_SHIORI_HEX: String = "93a18de8" # 藤崎
const SPEAKER_PLAYER_HEX: String = "8d828ca9" # 高見

const SPEAKER_BY_GIRL_HEX: Dictionary = {
	0: "93a18de8",     # 藤崎 (Shiori)
	1: "94408c8e",     # 如月 (Mio)
	2: "95528f8f",     # 紐緒 (Yuina)
	3: "95d08bcb",     # 片桐 (Ayako)
	4: "93f896ec",     # 虹野 (Saki)
	5: "8cc38eae",     # 古式 (Yukari)
	6: "90b490ec",     # 清川 (Nozomi)
	7: "8bbe",         # 鏡 (Mira)
	8: "92a993fa93de", # 朝日奈 (Yuko)
	9: "94fc8ef78cb4", # 美樹原 (Megumi)
	10: "918189b38f97",# 早乙女 (Yumi)
	11: "8ad997d1",    # 館林 (Miharu)
	12: "88c98f578940",# 伊集院 (Rei)
	13: "8d449759",    # 好雄 (Yoshio)
}

const GEKO_LINE_BY_GIRL_HEX: Dictionary = {
	0: "93a18de8817588ea8f8f82c98b4182e882dc82b582e582a48176", # 藤崎「一緒に帰りましょう」
	1: "94408c8e817582b288ea8f8f82b582c482e082a282a282c582b782a98176", # 如月「ご一緒してもいいですか」
	2: "95528f8f817588ea8f8f82c98b4182c182c482a082b082e982ed8176", # 紐緒「一緒に帰ってあげるわ」
	3: "95d08bcb817588ea8f8f82c98b4182e882dc82b582e581498176", # 片桐「一緒に帰りましょ！」
	4: "93f896ec817582cb82a6814188ea8f8f82c98b4182eb82a482e681498176", # 虹野「ねえ、一緒に帰ろうよ！」
}

const VALEN_LINES_BY_GIRL_HEX: Dictionary = {
	1: [
		"817582a082cc814182b182ea836f838c8393835e8343839382cc", # 「あの、これバレンタインの
		"836083878352838c815b836782c582b7814282e682a982c182bd82e7", # チョコレートです。よかったら
		"8ef382af8ee682c182c482ad82be82b382a28163816381428176", # 受け取ってください……。」
	],
	2: [
		"817582b182ea814193c190bb836083878352838c815b836782e68142", # 「これ、特製チョコレートよ。
		"95cf82c896f282cd93fc82c182c482a282c882a282a982e78141", # 変な薬は入っていないから、
		"8ab48ed382b582c48ef382af8ee682e882c882b382a281428176", # 感謝して受け取りなさい。」
	],
	3: [
		"8175836e83628373815b836f838c8393835e834383938149", # 「ハッピーバレンタイン！
		"8341815b836782c883608387835282e6814182e682a982c182bd82e7", # アートなチョコよ、よかったら
		"8ef382af8ee682c182c482bf82e582a482be82a282cb81498176", # 受け取ってちょうだいね！」
	],
	4: [
		"817582b182ea81418ee88dec82e882cc83608387835282c882cc8149", # 「これ、手作りのチョコなの！
		"88ea90b68c9c96bd8dec82c182bd82a982e78141", # 一生懸命作ったから、
		"904882d782c482ad82ea82e982c68af082b582a282c881498176", # 食べてくれると嬉しいな！」
	],
}

const ENDING_LINES_BY_GIRL_HEX: Dictionary = {
	1: [
		"81758e84814182b882c182c68d828ca982b382f182cc82b182c682aa", # 「私、ずっと高見さんのことが
		"8d4482ab82c582b582bd816381638149", # 好きでした……！
		"82b882c182c688ea8f8f82c982a282c489ba82b382a281428176", # ずっと一緒にいて下さい。」
	],
	2: [
		"81758e8482cc8ca48b8682c988ea94d4954b977682c882cc82cd8141", # 「私の研究に一番必要なのは、
		"82a082c882bd82be82c182bd82cc816381638149", # あなただったの……！
		"82b882c182c68e8482c9957482ab8d8782c182c482e682cb81428176", # ずっと私に付き合ってよね。」
	],
	3: [
		"81758341834381458389837581458386815b8149", # 「アイ・ラブ・ユー！
		"82b882c182c682a082c882bd82aa8d4482ab82be82c182bd82cc8149", # ずっとあなたが好きだったの！
		"82b182ea82a982e782e088ea8f8f82c982a282c482cb81498176", # これからも一緒にいてね！」
	],
	4: [
		"81758e84814182b882c182c68d828ca982ad82f182cc82b182c682aa", # 「私、ずっと高見くんのことが
		"91e58d4482ab82be82c182bd82e6816381638149", # 大好きだったよ……！
		"82b182ea82a982e782e0899e898782b382b982c482cb81498176", # これからも応援させてね！」
	],
}

# Walk Home (GEKO) 2-choice prompt strings (x_taku_string_set at x=-120 -> screen 40)
const CHOICE_ACCEPT_HEX: String = "817788ea8f8f82c98b4182e98178" # 『一緒に帰る』 (7 glyphs)
const CHOICE_DECLINE_HEX: String = "817788ea906c82c58b4182e98178" # 『一人で帰る』 (7 glyphs)

# Term Exam Results strings (inside 2-column EXAMINE.NBN wooden scoreboard Rect2i(0, 6, 320, 152))
const EXAM_ROW_1_HEX: String = "825088ca8d828ca98cf6906c825382578254935f" # １位高見公人４８５点 (10 glyphs)
const EXAM_ROW_2_HEX: String = "825188ca93a18de88e8d9044825382568251935f" # ２位藤崎詩織４７２点 (10 glyphs)
const EXAM_ROW_3_HEX: String = "825288ca95528f8f8c8b93de825382548257935f" # ３位紐緒結奈４５８点 (10 glyphs)
const EXAM_ROW_4_HEX: String = "825388ca94408c8e96a28f8f825382528254935f" # ４位如月未緒４３５点 (10 glyphs)
const EXAM_ROW_5_HEX: String = "825488ca8bbe96a39785814082528258824f935f" # ５位鏡魅羅　３９０点 (10 glyphs)
const EXAM_RIGHT_ROW_1_HEX: String = "825588ca95d08bcb8dca8e7182528253824f935f" # ６位片桐彩子３４０点 (10 glyphs)
const EXAM_RIGHT_ROW_2_HEX: String = "825688ca93f896ec8db98af3825282508254935f" # ７位虹野沙希３１５点 (10 glyphs)
const EXAM_RIGHT_ROW_3_HEX: String = "825788ca90b490ec965d8140825182588253935f" # ８位清川望　２９４点 (10 glyphs)
const EXAM_RIGHT_ROW_4_HEX: String = "825888ca92a993fa93de975b825182558254935f" # ９位朝日奈夕２６５点 (10 glyphs)
const EXAM_RIGHT_ROW_5_HEX: String = "8250824f88ca8d449759814082518250824f935f" # １０位好雄　２１０点 (10 glyphs)
const EXAM_LINE_1_HEX: String = "81698da193fa82cd8e8e8cb18c8b89ca82cc94ad955c82cc93fa82be8142" # （今日は試験結果の発表の日だ。 (15 glyphs)
const EXAM_LINE_2_HEX: String = "82e282c182bd815b814188ea94d482be8149" # やったー、一番だ！ (9 glyphs)
const EXAM_LINE_3_HEX: String = "82b182ea82c58e8e8cb182cc92e989a482be82c88142816a" # これで試験の帝王だな。） (12 glyphs)

# Valentine's Day strings
const VALEN_LINE_1_HEX: String = "817582b182ea8141836f838c8393835e8343839382cc" # 「これ、バレンタインの (11 glyphs)
const VALEN_LINE_2_HEX: String = "836083878352838c815b836782e6814282e682a982c182bd" # チョコレートよ。よかった (12 glyphs)
const VALEN_LINE_3_HEX: String = "82e78ef382af8ee682c182c482cb8163816382cd82a281428176" # ら受け取ってね……はい。」 (13 glyphs)

# Legendary Tree Confession strings
const ENDING_LINE_1_HEX: String = "81758e84814182b882c182c68d828ca982ad82f182cc82b182c682aa" # 「私、ずっと高見くんのことが (14 glyphs)
const ENDING_LINE_2_HEX: String = "8d4482ab82c582b582bd816381638149" # 好きでした……！ (8 glyphs)
const ENDING_LINE_3_HEX: String = "82bb82ce82c982a282c482ad82ea82dc82b782a981488176" # そばにいてくれますか？」 (12 glyphs)


var _shiori_bustup: Image = null
var _school_gate_bg: Image = null
var _school_cherry_gate_bg: Image = null
var _legendary_tree_bg: Image = null
var _exam_frame_img: Image = null

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

func load_manifest(hud_manifest_path: String, font_root: String) -> String:
	_shiori_bustup = _load_first_existing_image(".asset-cache/port/date-preview-v1/portraits/shiori_hu10/shiori_hu10_21938.static-frame.png")
	_school_gate_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-05ca56dffdcfe0b85cdc0ec238630e1f24010982a83d969d7b8941585a2a770a/asset-05ca56dffdcfe0b85cdc0ec238630e1f24010982a83d969d7b8941585a2a770a.background.png")
	_school_cherry_gate_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-9169639902fed8e42549b1c83fd9e49544147d656c7f37ff90333768b8cd9f12/asset-9169639902fed8e42549b1c83fd9e49544147d656c7f37ff90333768b8cd9f12.background.png")
	_legendary_tree_bg = _load_first_existing_image(".asset-cache/media/background-native-v5/assets/asset-7fc1cccbcbbcc95a3810264eb0afa456abb9d08b238dcaed57185cbff4f07a32/asset-7fc1cccbcbbcc95a3810264eb0afa456abb9d08b238dcaed57185cbff4f07a32.background.png")
	_exam_frame_img = _load_first_existing_image(".asset-cache/media/exam-original-probe/exam.frame-00.png")
	return init_resources(font_root, hud_manifest_path, "")


func check_glyph_containment_violations() -> Array[String]:
	return bounding_box_violations.duplicate()


func _reset_tracking() -> void:
	recorded_glyphs.clear()
	bounding_box_violations.clear()
	_ensure_compositor_coverage()


func _draw_notebook_text_hex_checked(target: Image, hex_code: String, pos: Vector2i, palette_row: int = 0) -> void:
	var prev_count: int = recorded_glyphs.size()
	_draw_text_hex_checked(target, hex_code, pos, palette_row, "notebook")
	for idx: int in range(prev_count, recorded_glyphs.size()):
		var gpos: Vector2i = recorded_glyphs[idx]["pos"]
		var rect := Rect2i(gpos.x, gpos.y, 12, 12)
		if not NOTEBOOK_INTERIOR_RECT.encloses(rect):
			bounding_box_violations.append(
				"Notebook glyph %s at (%d, %d) overflows NOTEBOOK_INTERIOR_RECT %s" % [
					str(recorded_glyphs[idx]["code"]), gpos.x, gpos.y, str(NOTEBOOK_INTERIOR_RECT)
				]
			)


func _draw_cal_base(target: Image, rect: Rect2i) -> void:
	# Exact PS1 POLY_G4 case 2 (prim_gradient_rect with arg5=0xE0E0E0, arg6=2 in gpu_primitives.c:225-230):
	# v0 = v1 = v2 = 224, v3 = 179 (224 * 8 / 10).
	for y: int in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240:
			continue
		var fy: float = float(y) / float(rect.size.y)
		for x: int in range(rect.size.x):
			var px: int = rect.position.x + x
			if px < 0 or px >= 320:
				continue
			var fx: float = float(x) / float(rect.size.x)
			var base_v: float = 224.0 if (fx + fy <= 1.0) else (224.0 - (fx + fy - 1.0) * 45.0)
			var d: int = DITHER[(py & 3) * 4 + (px & 3)]
			var v5: int = clampi((int(base_v) + d) >> 3, 0, 31)
			var v8: int = v5 << 3
			target.set_pixelv(Vector2i(px, py), Color8(v8, v8, v8, 255))


func _draw_menu_bar_highlight(target: Image, rect: Rect2i) -> void:
	# Exact PS1 POLY_G4 case 0 for menu_bar_style[0] = 0x832F7FAF (menu_speaker_girl.c:463, gpu_primitives.c:231-237)
	for y: int in range(rect.size.y):
		var py: int = rect.position.y + y
		if py < 0 or py >= 240:
			continue
		var fy: float = float(y) / float(rect.size.y)
		for x: int in range(rect.size.x):
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
			var back: Color = target.get_pixel(px, py)
			var out_r5: int = clampi((back.r8 >> 4) + (f_rb5 >> 1), 0, 31)
			var out_g5: int = clampi((back.g8 >> 4) + (f_g5 >> 1), 0, 31)
			var out_b5: int = clampi((back.b8 >> 4) + (f_rb5 >> 1), 0, 31)
			target.set_pixelv(Vector2i(px, py), Color8(out_r5 << 3, out_g5 << 3, out_b5 << 3, 255))


func _draw_school_gate_with_shiori(canvas: Image, autumn_afternoon: bool, _holding_chocolate: bool) -> void:
	var bg: Image = _school_gate_bg if autumn_afternoon else (_school_cherry_gate_bg if _school_cherry_gate_bg != null else _school_gate_bg)
	if bg != null:
		canvas.blit_rect(bg, Rect2i(0, 0, mini(320, bg.get_width()), mini(160, bg.get_height())), Vector2i(0, 0))
	_draw_shiori_bustup(canvas, 160, 38)


func _draw_shiori_bustup(canvas: Image, _center_x: int, _top_y: int) -> void:
	if _shiori_bustup != null and _compositor != null:
		_compositor._blit(canvas, _shiori_bustup, null, Vector2i(81, 8), false)


func _draw_legendary_tree_backdrop(canvas: Image) -> void:
	if _legendary_tree_bg != null:
		canvas.blit_rect(
			_legendary_tree_bg,
			Rect2i(0, 0, mini(320, _legendary_tree_bg.get_width()), mini(160, _legendary_tree_bg.get_height())),
			Vector2i(0, 0)
		)
	_draw_shiori_bustup(canvas, 172, 38)


# ============================================================================
# Scene 1: After-School Walk Home (GEKO) 2-Choice Invitation Prompt
# ============================================================================

func render_walk_home_invitation_frame(selected_choice: int = 0, girl_id: int = -1, state: Dictionary = {}) -> Image:
	# Mirrors encounter_walk_home_draw (src/ovl/girl_encounters/walk_home.c:72-81) +
	# encounter_walk_home_choice_prompt_init (walk_home.c:340-363):
	# - hud_screen_frame_draw(2)
	# - message_window_show()
	# - hizuke_show() (1995-10-18 Wed)
	# - message_window_title_update("下校")
	# - k_reset(1) + x_taku_menu_set(menu, 2) + x_taku_string_set("『一緒に帰る』", "『一人で帰る』")
	# - menu_bar_show() (OT=3, POLY_G4 case 0 for 0x832F7FAF at x=40, y=170 or 186, w=240, h=16)
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	if state.has("cursor"):
		selected_choice = int(state["cursor"])
	if state.has("girl_id"):
		girl_id = int(state["girl_id"])

	_draw_checkerboard_frame(target, 2, 64, 64, 2) # Month 10 (Oct) -> Season 2 (Autumn Warm-Amber)
	_draw_school_gate_with_shiori(target, true, false)
	_draw_message_window(target)
	_draw_date_pillar(target, 95, 10, 18, 2, 0) # 95 10/18 Wed
	_draw_title_badge(target, TITLE_GEKO_HEX, Vector2i.ZERO, 2)

	# menu_bar_show() highlight bar at choice_idx (x=40, y=170 or 186, w=240, h=16)
	var choice_idx: int = clampi(selected_choice, 0, 1)
	var bar_screen_y: int = 170 if choice_idx == 0 else 186
	_draw_menu_bar_highlight(target, Rect2i(40, bar_screen_y, 240, 16))

	# x_taku_string_set draws option 0 and option 1 inside the highlight bar
	_draw_text_hex_checked(target, CHOICE_ACCEPT_HEX, Vector2i(56, 170), 0, "message_window")
	_draw_text_hex_checked(target, CHOICE_DECLINE_HEX, Vector2i(56, 186), 0, "message_window")

	if girl_id >= 0:
		var geko_line_hex: String = String(GEKO_LINE_BY_GIRL_HEX.get(girl_id, GEKO_LINE_BY_GIRL_HEX[0]))
		_draw_text_hex_checked(target, geko_line_hex, Vector2i(56, 202), 0, "message_window")

	# Hand cursor (Frame 156) at right end of highlight bar (x = 116 -> screen 276, y = bar_screen_y + 8)
	_draw_actor(target, 156, Vector2i(276 - 160, bar_screen_y + 8 - 120))

	_draw_black_border_1px(target)
	return target


# ============================================================================
# Scene 2: Term Exam Results (MODE_EXAM) Notebook Scoreboard & Reaction
# ============================================================================

func render_term_exam_results_frame(state: Dictionary = {}) -> Image:
	# Mirrors exam_entry + exam_results_show + exam_results_react (src/ovl/misc_scenes/final_exam.c:663-776):
	# - icon_disp_switch(0), parameter_disp_switch(0), hizuke_disp_switch(0)
	# - exam_panel_draw_big(): prim_gradient_rect(-0xA0, -0x68, 0x140, 0x8E, 5, 0xE0E0E0, 2) -> Rect2i(0, 16, 320, 142)
	# - exam_panel_draw_header(): prim_gradient_rect(-0x41, -0x70, 0x84, 0x12, 4, 0x30E00, 0) -> Rect2i(95, 8, 132, 18)
	# - actor_pool[96]: EXAMINE.NBN wooden 2-column frame + "期末試験結果発表" plaque at (0, -0x28) -> Rect2i(0, 6, 320, 148)
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	_draw_checkerboard_frame(target, 0, 32, 32, 1) # Month 7 (Jul) -> Season 1 (Summer Sky-Blue), mode 0 fills top y=0..15 strip

	# 1. exam_panel_draw_big() (OT=5)
	_draw_cal_base(target, Rect2i(0, 16, 320, 142))
	if state.has("cursor"):
		var cur_row: int = clampi(int(state["cursor"]), 0, 4)
		_draw_menu_bar_highlight(target, Rect2i(14, 36 + cur_row * 16, 144, 16))
	# 2. exam_panel_draw_header() (OT=4): dark-green backdrop behind "期末試験結果発表" plaque
	_compositor._gradient(target, Rect2i(95, 8, 132, 18))
	# 3. actor_pool[96] (OT=3): EXAMINE.NBN wooden 2-column frame at (0, 6)
	if _exam_frame_img != null and _compositor != null:
		_compositor._blit(target, _exam_frame_img, null, Vector2i(0, 6), false)

	# 4. Ranked student rows inside left column (exam_row_x = -152 -> screen x = 16, exam_row_y = 38 + i*16)
	#    and right column (exam_row_x = +8 -> screen x = 176, exam_row_y = 38 + i*16)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_1_HEX, Vector2i(16, 38), 4)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_2_HEX, Vector2i(16, 54), 3)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_3_HEX, Vector2i(16, 70), 3)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_4_HEX, Vector2i(16, 86), 3)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_5_HEX, Vector2i(16, 102), 3)
	_draw_notebook_text_hex_checked(target, EXAM_RIGHT_ROW_1_HEX, Vector2i(176, 38), 3)
	_draw_notebook_text_hex_checked(target, EXAM_RIGHT_ROW_2_HEX, Vector2i(176, 54), 3)
	_draw_notebook_text_hex_checked(target, EXAM_RIGHT_ROW_3_HEX, Vector2i(176, 70), 3)
	_draw_notebook_text_hex_checked(target, EXAM_RIGHT_ROW_4_HEX, Vector2i(176, 86), 3)
	_draw_notebook_text_hex_checked(target, EXAM_RIGHT_ROW_5_HEX, Vector2i(176, 102), 3)

	_draw_message_window(target)

	var speaker_pos: Vector2i = get_speaker_screen_pos(SPEAKER_PLAYER_HEX)
	_draw_text_hex_checked(target, SPEAKER_PLAYER_HEX, speaker_pos, 0, "message_window")
	_draw_text_hex_checked(target, EXAM_LINE_1_HEX, Vector2i(74, 170), 0, "message_window")
	_draw_text_hex_checked(target, EXAM_LINE_2_HEX, Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, EXAM_LINE_3_HEX, Vector2i(88, 202), 0, "message_window")
	_draw_actor(target, 1, Vector2i(276 - 160, 192 - 120))

	_draw_black_border_1px(target)
	return target


# ============================================================================
# Scene 3: Valentine's Day (VALEN) Chocolate Presentation Scene
# ============================================================================

func render_valentine_chocolate_frame(girl_id: int = 0, state: Dictionary = {}) -> Image:
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	if state.has("girl_id"):
		girl_id = int(state["girl_id"])

	_draw_checkerboard_frame(target, 2, 96, 48, 3) # Month 2 (Feb) -> Season 3 (Winter Slate-Gray)
	_draw_school_gate_with_shiori(target, false, true)
	_draw_message_window(target)
	_draw_date_pillar(target, 96, 2, 14, 2, 0) # 96 2/14 Wed
	_draw_title_badge(target, TITLE_VALENTINE_HEX, Vector2i.ZERO, 6)

	if state.has("cursor"):
		var cur_row: int = clampi(int(state["cursor"]), 0, 2)
		_draw_menu_bar_highlight(target, Rect2i(72, 170 + cur_row * 16, 212, 16))

	var speaker_hex: String = String(SPEAKER_BY_GIRL_HEX.get(girl_id, SPEAKER_SHIORI_HEX))
	var lines: Array = VALEN_LINES_BY_GIRL_HEX.get(girl_id, [VALEN_LINE_1_HEX, VALEN_LINE_2_HEX, VALEN_LINE_3_HEX])
	var speaker_pos: Vector2i = get_speaker_screen_pos(speaker_hex)
	_draw_text_hex_checked(target, speaker_hex, speaker_pos, 0, "message_window")
	_draw_text_hex_checked(target, String(lines[0]), Vector2i(74, 170), 0, "message_window")
	_draw_text_hex_checked(target, String(lines[1]), Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, String(lines[2]), Vector2i(88, 202), 0, "message_window")
	_draw_actor(target, 1, Vector2i(276 - 160, 192 - 120))

	_draw_black_border_1px(target)
	return target


# ============================================================================
# Scene 4: Legendary Tree Confession (ENDING) Graduation Day Scene
# ============================================================================

func render_legendary_tree_confession_frame(girl_id: int = 0, state: Dictionary = {}) -> Image:
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	if state.has("girl_id"):
		girl_id = int(state["girl_id"])

	_draw_checkerboard_frame(target, 2, 128, 128)
	_draw_legendary_tree_backdrop(target)
	_draw_message_window(target)
	_draw_date_pillar(target, 98, 3, 1, 6, 1) # 98 3/1 Sun
	_draw_title_badge(target, TITLE_ENDING_HEX, Vector2i.ZERO, 3)

	if state.has("cursor"):
		var cur_row: int = clampi(int(state["cursor"]), 0, 2)
		_draw_menu_bar_highlight(target, Rect2i(72, 170 + cur_row * 16, 212, 16))

	var speaker_hex: String = String(SPEAKER_BY_GIRL_HEX.get(girl_id, SPEAKER_SHIORI_HEX))
	var lines: Array = ENDING_LINES_BY_GIRL_HEX.get(girl_id, [ENDING_LINE_1_HEX, ENDING_LINE_2_HEX, ENDING_LINE_3_HEX])
	var speaker_pos: Vector2i = get_speaker_screen_pos(speaker_hex)
	_draw_text_hex_checked(target, speaker_hex, speaker_pos, 0, "message_window")
	_draw_text_hex_checked(target, String(lines[0]), Vector2i(74, 170), 0, "message_window")
	_draw_text_hex_checked(target, String(lines[1]), Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, String(lines[2]), Vector2i(88, 202), 0, "message_window")
	_draw_actor(target, 1, Vector2i(276 - 160, 192 - 120))

	_draw_black_border_1px(target)
	return target
