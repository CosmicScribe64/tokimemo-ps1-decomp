extends SceneTree
## T-29012: Authentic 320x240 Prologue Visual Presentation & 16x16 Tile Verification Suite

const ProloguePresenter = preload("res://presentation/prologue_presenter.gd")

const DEFAULT_HUD_MANIFEST: String = "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const DEFAULT_FONT_ROOT: String = "/workspace/.asset-cache/port/fonts/native-font-v2"
const DEFAULT_SCENE_PLATES_DIR: String = "/workspace/.claude/orchestrator/state/T-29012/prologue-visual-v1/scene_plates"
const DEFAULT_CAPTURE_DIR: String = "/workspace/.claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures"

var checks: int = 0
var failures: int = 0
var failure_messages: Array[String] = []


func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		failure_messages.append(label)
		push_error("FAIL: " + label)


func _initialize() -> void:
	print("--- Running Prologue 320x240 Visual Presentation Test Suite ---")

	_test_hud_screen_frame_rules()
	_test_hizuke_visibility_rules()
	_test_title_badge_slide_and_triangle()
	_test_speaker_and_dialogue_indentation()
	_test_scroll_and_text_marker()
	_test_render_and_capture_frames()

	print("---------------------------------------------------------------")
	print(("PASS" if failures == 0 else "FAIL") + ": %d prologue visual checks (%d failures)" % [checks, failures])
	quit(0 if failures == 0 else 1)


func _test_hud_screen_frame_rules() -> void:
	check(ProloguePresenter.get_hud_screen_frame_arg(0) == 0,
		"Stage 0 uses hud_screen_frame_draw(0) (src/ovl/name_entry/prologue.c:2814)")
	for stage_idx: int in range(1, 22):
		check(ProloguePresenter.get_hud_screen_frame_arg(stage_idx) == 2,
			"Stage %d uses hud_screen_frame_draw(2) (src/ovl/name_entry/prologue.c:2816)" % stage_idx)

	# Verify 10x8 tile grid for arg == 0 vs arg == 1 vs arg == 2 (src/main/hud_panels.c:1412-1438)
	var arg0_enabled_count: int = 0
	var arg1_enabled_count: int = 0
	var arg2_enabled_count: int = 0
	for row: int in range(8):
		for col: int in range(10):
			var en0: bool = ProloguePresenter.is_checker_tile_enabled(0, col, row)
			var en1: bool = ProloguePresenter.is_checker_tile_enabled(1, col, row)
			var en2: bool = ProloguePresenter.is_checker_tile_enabled(2, col, row)
			if en0:
				arg0_enabled_count += 1
			if en1:
				arg1_enabled_count += 1
			if en2:
				arg2_enabled_count += 1
			if row >= 6:
				check(en0 and en1 and en2, "Bottom 2 rows (row=%d, col=%d) enabled in modes 0, 1, 2" % [row, col])
			elif row == 5:
				if col == 0 or col == 9:
					check(en0 and en1 and en2, "Row 5 side columns (col=%d) enabled in modes 0, 1, 2" % col)
				else:
					check(en0 and not en1 and not en2, "Row 5 interior (col=%d) enabled only in mode 0" % col)
			else:
				check(en0 and not en2, "Rows 0..4 (row=%d, col=%d) enabled in mode 0 and disabled in mode 2" % [row, col])
	check(arg0_enabled_count == 80, "Mode 0 enables all 80 checkerboard tiles behind the 320x160 viewport")
	check(arg1_enabled_count == 40, "Mode 1 enables 40 outer border checkerboard tiles")
	check(arg2_enabled_count == 22, "Mode 2 enables 22 lower message-window border checkerboard tiles")


func _test_hizuke_visibility_rules() -> void:
	check(not ProloguePresenter.is_hizuke_displayed(0, 0, 0),
		"Stage 0 step 0 before hizuke_init() does not display date pillar")
	check(ProloguePresenter.is_hizuke_displayed(0, 1, 0),
		"Stage 0 step 1 displays date pillar via hizuke_show()")
	check(ProloguePresenter.is_hizuke_displayed(0, 2, 1),
		"Stage 0 step 2 substep 1 displays date pillar via hizuke_show()")
	check(ProloguePresenter.is_hizuke_displayed(1, 0, 0),
		"Stage 1 step 0 substep 0 displays date pillar before CD read completion (prologue.c:364)")
	check(not ProloguePresenter.is_hizuke_displayed(1, 0, 1),
		"Stage 1 step 0 substep 1 hides date pillar after hizuke_disp_switch(0) (prologue.c:371)")
	check(not ProloguePresenter.is_hizuke_displayed(1, 1, 1),
		"Stage 1 step 1 substep 1 keeps date pillar hidden")
	for stage_idx: int in range(2, 22):
		check(not ProloguePresenter.is_hizuke_displayed(stage_idx, 0, 0),
			"Stage %d keeps date pillar hidden after hizuke_disp_switch(0)" % stage_idx)


func _test_title_badge_slide_and_triangle() -> void:
	# Initial state at title_timer = -64: off-screen
	var s_init: Dictionary = ProloguePresenter.compute_title_badge_state(-64, false)
	check(not bool(s_init["visible"]) and int(s_init["next_timer"]) == -63,
		"Title badge at timer -64 is off-screen and advances timer to -63")

	# Settled window [0..119]: offset == (0, 0), rect == Rect2i(16, 30, 78, 20)
	var s_settled: Dictionary = ProloguePresenter.compute_title_badge_state(0, false)
	check(bool(s_settled["visible"]) and s_settled["rect"] == Rect2i(16, 30, 78, 20) and s_settled["text_pos"] == Vector2i(18, 32),
		"Title badge at timer 0 is settled at Rect2i(16, 30, 78, 20) with text_pos (18, 32)")

	var s_end_settle: Dictionary = ProloguePresenter.compute_title_badge_state(119, false)
	check(bool(s_end_settle["visible"]) and s_end_settle["rect"] == Rect2i(16, 30, 78, 20),
		"Title badge at timer 119 remains settled at Rect2i(16, 30, 78, 20)")

	# Done state (>= 184) without Triangle held: hidden
	var s_done: Dictionary = ProloguePresenter.compute_title_badge_state(184, false)
	check(not bool(s_done["visible"]) and int(s_done["next_timer"]) == 184,
		"Title badge at timer 184 without PAD_TRIANGLE is hidden")

	# Done state with PAD_TRIANGLE held: re-displayed at settled position (hud_panels.c:1218-1226)
	var s_triangle: Dictionary = ProloguePresenter.compute_title_badge_state(184, true)
	check(bool(s_triangle["visible"]) and s_triangle["rect"] == Rect2i(16, 30, 78, 20) and s_triangle["text_pos"] == Vector2i(18, 32),
		"Title badge with PAD_TRIANGLE held re-displays at settled Rect2i(16, 30, 78, 20)")


func _test_speaker_and_dialogue_indentation() -> void:
	check(ProloguePresenter.get_speaker_screen_pos(ProloguePresenter.STR_HEX["speaker_fukuhara"]) == Vector2i(39, 170),
		"2-char speaker 福原 renders at (39, 170) per gnsx (menu_speaker_girl.c:522-535)")
	check(ProloguePresenter.get_speaker_screen_pos(ProloguePresenter.STR_HEX["speaker_fujisaki"]) == Vector2i(39, 170),
		"2-char speaker 藤崎 renders at (39, 170) per gnsx")
	check(ProloguePresenter.get_speaker_screen_pos(ProloguePresenter.STR_HEX["speaker_girl_a"]) == Vector2i(32, 170),
		"3-char speaker 女子Ａ renders at (32, 170) per gnsx")
	check(ProloguePresenter.get_speaker_screen_pos("8260") == Vector2i(46, 170),
		"1-char speaker renders at (46, 170) per gnsx")

	var r_monologue0: Dictionary = ProloguePresenter.format_sndi_row(ProloguePresenter.STR_HEX["line1_row0"], 0, 0x0F)
	check(int(r_monologue0["x"]) == 74 and str(r_monologue0["hex"]).begins_with(ProloguePresenter.STR_HEX["bracket_double_open"]),
		"Monologue row 0 (style 0x0F) prepends 『 and starts at x=74")

	var r_monologue1: Dictionary = ProloguePresenter.format_sndi_row(ProloguePresenter.STR_HEX["line1_row1"], 1, 0x0F)
	check(int(r_monologue1["x"]) == 88 and str(r_monologue1["hex"]) == ProloguePresenter.STR_HEX["line1_row1"],
		"Monologue row 1 (style 0x0F) starts at indented x=88 without prefix")

	var r_spoken0: Dictionary = ProloguePresenter.format_sndi_row(ProloguePresenter.STR_HEX["line18_row0"], 0, 0x00)
	check(int(r_spoken0["x"]) == 74 and str(r_spoken0["hex"]).begins_with(ProloguePresenter.STR_HEX["bracket_quote_open"]),
		"Spoken dialogue row 0 (style 0x00) prepends 「 and starts at x=74")


func _test_scroll_and_text_marker() -> void:
	var line19_rows: Array[String] = [
		ProloguePresenter.STR_HEX["line19_row0"],
		ProloguePresenter.STR_HEX["line19_row1"],
		ProloguePresenter.STR_HEX["line19_row2"],
		ProloguePresenter.STR_HEX["line19_row3"],
		ProloguePresenter.STR_HEX["line19_row4"],
	]
	var vis_initial: Array[Dictionary] = ProloguePresenter.build_visible_dialogue_rows(line19_rows, 0x00, 0)
	check(vis_initial.size() == 3
		and vis_initial[0]["pos"] == Vector2i(74, 170)
		and vis_initial[1]["pos"] == Vector2i(88, 186)
		and vis_initial[2]["pos"] == Vector2i(88, 202),
		"Line 19 initial visible window shows rows 0..2 at (74, 170), (88, 186), (88, 202)")

	var vis_scrolled: Array[Dictionary] = ProloguePresenter.build_visible_dialogue_rows(line19_rows, 0x00, 2)
	check(vis_scrolled.size() == 3
		and int(vis_scrolled[0]["block_row"]) == 2 and vis_scrolled[0]["pos"] == Vector2i(88, 170)
		and int(vis_scrolled[1]["block_row"]) == 3 and vis_scrolled[1]["pos"] == Vector2i(88, 186)
		and int(vis_scrolled[2]["block_row"]) == 4 and vis_scrolled[2]["pos"] == Vector2i(88, 202),
		"Line 19 scrolled to first_visible_row=2 shows rows 2..4 at x=88, y=170, 186, 202")

	var marker_state: Dictionary = ProloguePresenter.compute_text_marker_state(true, 0x48, 0)
	check(bool(marker_state["enabled"]) and int(marker_state["sprite_id"]) == 0xF0 and int(marker_state["y"]) == 0x49 and marker_state["screen_pos"] == Vector2i(276, 193),
		"Text prompt marker (hud_text_marker_set / hud_text_marker_bob) uses sprite 0xF0 at x=0x74 (276) and bobs y in 0x48..0x4F")

	var marker_wrap: Dictionary = ProloguePresenter.compute_text_marker_state(true, 0x4F, 0)
	check(int(marker_wrap["y"]) == 0x48,
		"Text prompt marker wraps back from 0x4F to 0x48")


func _test_render_and_capture_frames() -> void:
	var hud_manifest: String = OS.get_environment("PROLOGUE_HUD_MANIFEST")
	if hud_manifest.is_empty():
		hud_manifest = DEFAULT_HUD_MANIFEST
	var font_root: String = OS.get_environment("PROLOGUE_FONT_ROOT")
	if font_root.is_empty():
		font_root = DEFAULT_FONT_ROOT
	var plates_dir: String = OS.get_environment("PROLOGUE_SCENE_PLATES_DIR")
	if plates_dir.is_empty():
		plates_dir = DEFAULT_SCENE_PLATES_DIR
	var capture_dir: String = OS.get_environment("PROLOGUE_VISUAL_CAPTURE_DIR")
	if capture_dir.is_empty():
		capture_dir = DEFAULT_CAPTURE_DIR

	var presenter := ProloguePresenter.new()
	var init_err: String = presenter.init_resources(font_root, hud_manifest, plates_dir)
	check(init_err.is_empty(), "ProloguePresenter initialized font root, HUD manifest, and scene plates: " + init_err)
	if not init_err.is_empty():
		return

	DirAccess.make_dir_recursive_absolute(capture_dir)

	var renders: Array[Dictionary] = [
		{
			"name": "godot_prologue_stage0_arrival.png",
			"image": presenter.render_stage0_arrival(false, 94, 81),
		},
		{
			"name": "godot_prologue_stage0_arrival_with_badge.png",
			"image": presenter.render_stage0_arrival(true, 104, 76),
		},
		{
			"name": "godot_prologue_stage1_board.png",
			"image": presenter.render_stage1_board(174, 169),
		},
		{
			"name": "godot_prologue_stage13_shiori_line18.png",
			"image": presenter.render_stage13_shiori_line18(72, 220),
		},
		{
			"name": "godot_prologue_stage14_shiori_line19.png",
			"image": presenter.render_stage14_shiori_line19(79, 216, 2),
		},
	]

	check(presenter.bounding_box_violations.is_empty(),
		"Zero bounding-box violations across all 5 rendered Prologue frames: " + str(presenter.bounding_box_violations))
	check(presenter.recorded_glyphs.size() > 80,
		"Recorded %d native 12x12 SJIS glyph blits across all 5 Prologue frames" % presenter.recorded_glyphs.size())

	for item: Dictionary in renders:
		var fname: String = str(item["name"])
		var img: Image = item["image"]
		check(img != null and img.get_size() == Vector2i(320, 240),
			"Rendered %s has exact native 320x240 resolution" % fname)
		if img != null:
			var out_path: String = capture_dir.path_join(fname)
			var save_err: Error = img.save_png(out_path)
			check(save_err == OK, "Saved %s to %s" % [fname, out_path])
