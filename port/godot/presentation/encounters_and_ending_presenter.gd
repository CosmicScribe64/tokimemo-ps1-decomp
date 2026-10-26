extends "res://presentation/prologue_presenter.gd"
## T-29018: Authentic 320x240 PS1 Presenter for Lane 3 & 4 Overlays:
## 1. After-School Walk Home (GEKO / 下校) 2-choice invitation prompt
##    (k_reset(1), x_taku_menu_set, x_taku_string_set, menu_bar_show, hand cursor).
## 2. Term Exam Results (MODE_EXAM / 結果発表) notebook scoreboard & reaction
##    (cal_base_show / exam_panel_draw_big, exam_panel_draw_header, hizuke_show, message_window_show).
## 3. Valentine's Day (VALEN / バレンタイン) chocolate presentation scene
##    (hud_screen_frame_draw(2), message_window_show, hizuke_show, gnsx, sndi).
## 4. Legendary Tree Confession (ENDING / 卒業式・伝説の樹の下で) scene
##    (hud_screen_frame_draw(2), message_window_show, hizuke_show, gnsx, sndi).

const MESSAGE_WINDOW_INTERIOR_RECT: Rect2i = Rect2i(27, 166, 266, 52)
const NOTEBOOK_INTERIOR_RECT: Rect2i = Rect2i(0, 33, 283, 125)
const DITHER: Array[int] = [-4, 0, -3, 1, 2, -2, 3, -1, -3, 1, -4, 0, 3, -1, 2, -2]

# Pre-computed CP932 hex constants via Python .encode("cp932").hex() (Section 12a rule)
const TITLE_GEKO_HEX: String = "89ba8d5a" # 下校 (2 glyphs)
const TITLE_EXAM_RESULTS_HEX: String = "8c8b89ca94ad955c" # 結果発表 (4 glyphs)
const TITLE_VALENTINE_HEX: String = "836f838c8393835e83438393" # バレンタイン (6 glyphs)
const TITLE_ENDING_HEX: String = "91b28bc68eae" # 卒業式 (3 glyphs)

const SPEAKER_SHIORI_HEX: String = "93a18de8" # 藤崎
const SPEAKER_PLAYER_HEX: String = "8d828ca9" # 高見

# Walk Home (GEKO) 2-choice prompt strings (x_taku_string_set at x=-120 -> screen 40)
const CHOICE_ACCEPT_HEX: String = "817788ea8f8f82c98b4182e98178" # 『一緒に帰る』 (7 glyphs)
const CHOICE_DECLINE_HEX: String = "817788ea906c82c58b4182e98178" # 『一人で帰る』 (7 glyphs)

# Term Exam Results strings (inside NOTEBOOK_INTERIOR_RECT Rect2i(0, 33, 283, 125))
const EXAM_NOTEBOOK_TITLE_HEX: String = "8afa96968e8e8cb190ac90d194ad955c" # 期末試験成績発表 (8 glyphs)
const EXAM_ROW_1_HEX: String = "825088ca81408d828ca98cf6906c8140825382578254935f" # １位　高見公人　４８５点 (12 glyphs)
const EXAM_ROW_2_HEX: String = "825188ca814093a18de88e8d90448140825382568251935f" # ２位　藤崎詩織　４７２点 (12 glyphs)
const EXAM_ROW_3_HEX: String = "825288ca814095528f8f8c8b93de8140825382548257935f" # ３位　紐緒結奈　４５８点 (12 glyphs)
const EXAM_ROW_4_HEX: String = "825388ca814094408c8e96a28f8f8140825382528254935f" # ４位　如月未緒　４３５点 (12 glyphs)
const EXAM_ROW_5_HEX: String = "825488ca81408bbe96a397858140814082528258824f935f" # ５位　鏡魅羅　　３９０点 (12 glyphs)
const EXAM_LINE_1_HEX: String = "81698da193fa82cd8e8e8cb18c8b89ca82cc94ad955c82cc93fa82be8142" # （今日は試験結果の発表の日だ。 (15 glyphs)
const EXAM_LINE_2_HEX: String = "82e282c182bd815b814188ea94d482be8149" # やったー、一番だ！ (9 glyphs)
const EXAM_LINE_3_HEX: String = "82b182ea82c58e8e8cb182cc92e989a482be82c88142816a" # これで試験の帝王だな。） (12 glyphs)

# Valentine's Day strings
const VALEN_LINE_1_HEX: String = "817582b182ea8141836f838c8393835e8343839382cc" # 「これ、バレンタインの (11 glyphs)
const VALEN_LINE_2_HEX: String = "836083878352838c815b836782e6814282a282c282e08ae692a382e9" # チョコレートよ。いつも頑張る (14 glyphs)
const VALEN_LINE_3_HEX: String = "8d828ca982ad82f182c98163816382cd82a281428176" # 高見くんに……はい。」 (11 glyphs)

# Legendary Tree Confession strings
const ENDING_LINE_1_HEX: String = "81758e84814182b882c182c68d828ca982ad82f182cc82b182c682aa" # 「私、ずっと高見くんのことが (14 glyphs)
const ENDING_LINE_2_HEX: String = "8d4482ab82c582b582bd816381638149" # 好きでした……！ (8 glyphs)
const ENDING_LINE_3_HEX: String = "82bb82ce82c982a282c482ad82ea82dc82b782a981488176" # そばにいてくれますか？」 (12 glyphs)


var _shiori_bustup: Image = null

func load_manifest(hud_manifest_path: String, font_root: String) -> String:
	var shiori_path := "/workspace/.asset-cache/port/date-preview-v1/portraits/shiori_hu10/shiori_hu10_21938.static-frame.png"
	if not FileAccess.file_exists(shiori_path):
		shiori_path = ProjectSettings.globalize_path("res://../../.asset-cache/port/date-preview-v1/portraits/shiori_hu10/shiori_hu10_21938.static-frame.png")
	if FileAccess.file_exists(shiori_path):
		_shiori_bustup = Image.load_from_file(shiori_path)
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


func _draw_school_gate_with_shiori(canvas: Image, autumn_afternoon: bool, holding_chocolate: bool) -> void:
	var sky_top := Color8(235, 165, 118) if autumn_afternoon else Color8(135, 192, 238)
	var sky_mid := Color8(248, 206, 156) if autumn_afternoon else Color8(192, 224, 248)
	for y: int in range(160):
		var t: float = float(y) / 159.0
		canvas.fill_rect(Rect2i(0, y, 320, 1), sky_top.lerp(sky_mid, t))

	# School building & windows
	canvas.fill_rect(Rect2i(20, 32, 200, 96), Color8(218, 206, 192))
	canvas.fill_rect(Rect2i(16, 28, 208, 6), Color8(142, 128, 118))
	for wy: int in [44, 68, 92]:
		for wx: int in range(32, 204, 28):
			canvas.fill_rect(Rect2i(wx, wy, 18, 14), Color8(86, 112, 142))
			canvas.fill_rect(Rect2i(wx + 1, wy + 1, 7, 12), Color8(168, 196, 218))

	# Gate pillar & courtyard pavement
	canvas.fill_rect(Rect2i(0, 122, 320, 38), Color8(164, 154, 146))
	canvas.fill_rect(Rect2i(36, 78, 26, 56), Color8(152, 88, 72))
	canvas.fill_rect(Rect2i(34, 74, 30, 5), Color8(210, 198, 186))
	canvas.fill_rect(Rect2i(44, 88, 10, 26), Color8(228, 218, 196))

	# Trees along the gate
	canvas.fill_rect(Rect2i(215, 16, 105, 86), Color8(72, 118, 68))
	canvas.fill_rect(Rect2i(230, 28, 84, 64), Color8(94, 146, 82))
	canvas.fill_rect(Rect2i(258, 92, 18, 42), Color8(96, 68, 48))

	_draw_shiori_bustup(canvas, 160, 38, holding_chocolate)


func _draw_shiori_bustup(canvas: Image, center_x: int, top_y: int, holding_chocolate: bool) -> void:
	if _shiori_bustup != null and _compositor != null:
		_compositor._blit(canvas, _shiori_bustup, null, Vector2i(81, 8), false)
	else:
		canvas.fill_rect(Rect2i(center_x - 28, top_y + 6, 56, 78), Color8(188, 64, 76))

	if holding_chocolate:
		canvas.fill_rect(Rect2i(center_x - 18, top_y + 76, 36, 26), Color8(238, 182, 204))
		canvas.fill_rect(Rect2i(center_x - 18, top_y + 76, 36, 4), Color8(212, 58, 88))
		canvas.fill_rect(Rect2i(center_x - 3, top_y + 76, 6, 26), Color8(212, 58, 88))
		canvas.fill_rect(Rect2i(center_x - 8, top_y + 72, 16, 6), Color8(232, 64, 96))


func _draw_legendary_tree_backdrop(canvas: Image) -> void:
	var sky_top := Color8(122, 178, 236)
	var sky_bottom := Color8(224, 236, 252)
	for y: int in range(160):
		var t: float = float(y) / 159.0
		canvas.fill_rect(Rect2i(0, y, 320, 1), sky_top.lerp(sky_bottom, t))

	canvas.fill_rect(Rect2i(0, 116, 320, 44), Color8(112, 168, 96))
	canvas.fill_rect(Rect2i(0, 130, 320, 30), Color8(92, 148, 78))

	canvas.fill_rect(Rect2i(28, 36, 58, 98), Color8(104, 68, 46))
	canvas.fill_rect(Rect2i(20, 112, 74, 22), Color8(92, 58, 38))
	canvas.fill_rect(Rect2i(42, 42, 14, 82), Color8(126, 86, 60))

	canvas.fill_rect(Rect2i(0, 0, 320, 52), Color8(246, 196, 212))
	canvas.fill_rect(Rect2i(0, 24, 260, 36), Color8(252, 214, 226))
	canvas.fill_rect(Rect2i(18, 8, 280, 28), Color8(238, 174, 196))

	var petals: Array[Vector2i] = [
		Vector2i(112, 68), Vector2i(134, 84), Vector2i(228, 62),
		Vector2i(252, 92), Vector2i(276, 76), Vector2i(96, 104),
	]
	for p: Vector2i in petals:
		canvas.fill_rect(Rect2i(p.x, p.y, 4, 3), Color8(252, 208, 222))

	_draw_shiori_bustup(canvas, 172, 38, false)


# ============================================================================
# Scene 1: After-School Walk Home (GEKO) 2-Choice Invitation Prompt
# ============================================================================

func render_walk_home_invitation_frame(selected_choice: int = 0) -> Image:
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

	_draw_checkerboard_frame(target, 2, 64, 64)
	_draw_school_gate_with_shiori(target, true, false)
	_draw_message_window(target)
	_draw_date_pillar(target, 95, 10, 18, 2, 0) # 95 10/18 Wed
	_draw_title_badge(target, TITLE_GEKO_HEX, Vector2i.ZERO, 2)

	# menu_bar_show() highlight bar at choice_idx (x=40, y=170 or 186, w=240, h=16)
	var choice_idx: int = clampi(selected_choice, 0, 1)
	var bar_screen_y: int = 170 if choice_idx == 0 else 186
	_draw_menu_bar_highlight(target, Rect2i(40, bar_screen_y, 240, 16))

	# x_taku_string_set draws option 0 and option 1 inside the highlight bar, with hand cursor pointing from the left
	_draw_text_hex_checked(target, CHOICE_ACCEPT_HEX, Vector2i(56, 170), 0, "message_window")
	_draw_text_hex_checked(target, CHOICE_DECLINE_HEX, Vector2i(56, 186), 0, "message_window")

	# Hand cursor (Frame 156, width 24px) at x=30..54 pointing cleanly at x=56
	_draw_actor(target, 156, Vector2i(30 - 160, bar_screen_y + 2 - 120))

	_draw_black_border_1px(target)
	return target


# ============================================================================
# Scene 2: Term Exam Results (MODE_EXAM) Notebook Scoreboard & Reaction
# ============================================================================

func render_term_exam_results_frame() -> Image:
	# Mirrors exam_entry + exam_results_show + exam_results_react (src/ovl/misc_scenes/final_exam.c:94-127, 635-776):
	# Renders the 320x240 HUD with Gouraud exam scoreboard panel (Rect2i(0, 33, 283, 125)),
	# header banner (exam_panel_draw_header), date pillar, title badge ("結果発表"),
	# ranked student rows, and 3-row Rank #1 reaction.
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	_draw_checkerboard_frame(target, 1, 32, 32)

	# Base Gouraud scoreboard panel inside Rect2i(0, 33, 283, 125) (cal_base_show / exam_panel_draw_big)
	_draw_cal_base(target, NOTEBOOK_INTERIOR_RECT)
	# Header dark-green banner inside scoreboard (exam_panel_draw_header)
	_compositor._gradient(target, Rect2i(98, 35, 124, 16))

	# Exam scoreboard header and top 5 ranked rows inside NOTEBOOK_INTERIOR_RECT Rect2i(0, 33, 283, 125)
	_draw_notebook_text_hex_checked(target, EXAM_NOTEBOOK_TITLE_HEX, Vector2i(104, 37), 0)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_1_HEX, Vector2i(48, 58), 4)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_2_HEX, Vector2i(48, 76), 3)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_3_HEX, Vector2i(48, 94), 3)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_4_HEX, Vector2i(48, 112), 3)
	_draw_notebook_text_hex_checked(target, EXAM_ROW_5_HEX, Vector2i(48, 130), 3)

	_draw_message_window(target)
	_draw_date_pillar(target, 95, 7, 15, 5, 0) # 95 7/15 Sat
	_draw_title_badge(target, TITLE_EXAM_RESULTS_HEX, Vector2i.ZERO, 4)

	var speaker_pos: Vector2i = get_speaker_screen_pos(SPEAKER_PLAYER_HEX)
	_draw_text_hex_checked(target, SPEAKER_PLAYER_HEX, speaker_pos, 0, "message_window")
	_draw_text_hex_checked(target, EXAM_LINE_1_HEX, Vector2i(74, 170), 0, "message_window")
	_draw_text_hex_checked(target, EXAM_LINE_2_HEX, Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, EXAM_LINE_3_HEX, Vector2i(88, 202), 0, "message_window")

	_draw_black_border_1px(target)
	return target


# ============================================================================
# Scene 3: Valentine's Day (VALEN) Chocolate Presentation Scene
# ============================================================================

func render_valentine_chocolate_frame() -> Image:
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	_draw_checkerboard_frame(target, 2, 96, 48)
	_draw_school_gate_with_shiori(target, false, true)
	_draw_message_window(target)
	_draw_date_pillar(target, 96, 2, 14, 2, 0) # 96 2/14 Wed
	_draw_title_badge(target, TITLE_VALENTINE_HEX, Vector2i.ZERO, 6)

	var speaker_pos: Vector2i = get_speaker_screen_pos(SPEAKER_SHIORI_HEX)
	_draw_text_hex_checked(target, SPEAKER_SHIORI_HEX, speaker_pos, 0, "message_window")
	_draw_text_hex_checked(target, VALEN_LINE_1_HEX, Vector2i(74, 170), 0, "message_window")
	_draw_text_hex_checked(target, VALEN_LINE_2_HEX, Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, VALEN_LINE_3_HEX, Vector2i(88, 202), 0, "message_window")

	_draw_black_border_1px(target)
	return target


# ============================================================================
# Scene 4: Legendary Tree Confession (ENDING) Graduation Day Scene
# ============================================================================

func render_legendary_tree_confession_frame() -> Image:
	_reset_tracking()
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	_draw_checkerboard_frame(target, 2, 128, 128)
	_draw_legendary_tree_backdrop(target)
	_draw_message_window(target)
	_draw_date_pillar(target, 98, 3, 1, 6, 1) # 98 3/1 Sun
	_draw_title_badge(target, TITLE_ENDING_HEX, Vector2i.ZERO, 3)

	var speaker_pos: Vector2i = get_speaker_screen_pos(SPEAKER_SHIORI_HEX)
	_draw_text_hex_checked(target, SPEAKER_SHIORI_HEX, speaker_pos, 0, "message_window")
	_draw_text_hex_checked(target, ENDING_LINE_1_HEX, Vector2i(74, 170), 0, "message_window")
	_draw_text_hex_checked(target, ENDING_LINE_2_HEX, Vector2i(88, 186), 0, "message_window")
	_draw_text_hex_checked(target, ENDING_LINE_3_HEX, Vector2i(88, 202), 0, "message_window")

	_draw_black_border_1px(target)
	return target
