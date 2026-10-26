extends SceneTree
## T-29018: Verification Suite for VnScenePresenter & System Overlays (OPTION, OLH, ETC).
## Validates PS1 320x240 presentation primitives, state machine transitions, settings,
## Section 12a glyph bounds, and renders PNG visual proof captures.

const VnPresenter = preload("res://presentation/vn_scene_presenter.gd")
const Overlays = preload("res://overlays/system_overlays.gd")

const DEFAULT_FONT_ROOT := "/workspace/.asset-cache/port/fonts/native-font-v2"
const DEFAULT_HUD_MANIFEST := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const DEFAULT_CAPTURE_DIR := "/workspace/.claude/orchestrator/state/T-29018/shared-vn-and-system-overlays-v1/captures"

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
	print("==================================================================")
	print("--- Running T-29018 VnScenePresenter & System Overlays Tests ---")
	print("==================================================================")

	_test_checkerboard_frame_modes()
	_test_menu_bar_highlight_styles()
	_test_vn_dialogue_layout_and_shadow()
	_test_presentation_primitives()
	_test_options_overlay_state_machine()
	_test_help_overlay_state_machine()
	_test_title_menu_overlay_state_machine()
	_test_render_visual_captures_and_bounding_boxes()

	print("==================================================================")
	print(("PASS" if failures == 0 else "FAIL") + ": %d checks (%d failures)" % [checks, failures])
	print("==================================================================")
	quit(0 if failures == 0 else 1)

# ==============================================================================
# 1. Checkerboard Frame Tests
# ==============================================================================

func _test_checkerboard_frame_modes() -> void:
	print("[1/8] Testing checkerboard frame modes (hud_screen_frame_draw)...")

	var m0_count: int = 0
	var m1_count: int = 0
	var m2_count: int = 0

	for row in range(8):
		for col in range(10):
			var e0: bool = VnPresenter.is_checker_tile_enabled(0, col, row)
			var e1: bool = VnPresenter.is_checker_tile_enabled(1, col, row)
			var e2: bool = VnPresenter.is_checker_tile_enabled(2, col, row)

			if e0: m0_count += 1
			if e1: m1_count += 1
			if e2: m2_count += 1

			# Mode 2 specifics:
			if row < 5:
				check(not e2, "Mode 2 row %d col %d disabled" % [row, col])
			elif row == 5:
				if col == 0 or col == 9:
					check(e2, "Mode 2 row 5 pillar col %d enabled" % col)
				else:
					check(not e2, "Mode 2 row 5 interior col %d disabled" % col)
			elif row >= 6:
				check(e2, "Mode 2 row %d col %d enabled" % [row, col])

	check(m0_count == 80, "Mode 0 enables all 80 checkerboard tiles (10x8)")
	check(m1_count == 40, "Mode 1 enables 40 perimeter border tiles")
	check(m2_count == 22, "Mode 2 enables 22 lower message window tiles")

# ==============================================================================
# 2. Menu Bar Highlight Style Tests
# ==============================================================================

func _test_menu_bar_highlight_styles() -> void:
	print("[2/8] Testing menu_bar_show styles and Gouraud shading...")

	var presenter := VnPresenter.new()
	var dummy := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	dummy.fill(Color8(128, 128, 128, 255))

	# Test all 3 verified styles (0x832F7FAF, 0x822F7F2F, 0x812F4F8F)
	for style_idx in range(3):
		var target := dummy.duplicate()
		var rect := Rect2i(50, 50, 100, 20)
		presenter.draw_menu_bar_highlight(target, rect, style_idx)

		var inside: Color = target.get_pixel(60, 60)
		var outside: Color = target.get_pixel(10, 10)

		check(inside != outside, "Style %d modifies pixels inside highlight rect" % style_idx)
		check(outside == Color8(128, 128, 128, 255), "Style %d leaves outside untouched" % style_idx)
		check(inside.a > 0.9, "Style %d output has opaque alpha" % style_idx)

# ==============================================================================
# 3. VN Dialogue Layout & Shadow Tests
# ==============================================================================

func _test_vn_dialogue_layout_and_shadow() -> void:
	print("[3/8] Testing VN dialogue layouts (sndisp vs talk_render)...")

	var presenter := VnPresenter.new()
	var font_root: String = OS.get_environment("FONT_ROOT")
	if font_root.is_empty():
		font_root = DEFAULT_FONT_ROOT
	presenter.init_resources(font_root)

	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	# 1. sndisp layout (Y = 170, 186, 202)
	presenter.recorded_glyphs.clear()
	presenter.draw_vn_dialogue(target, Overlays.STR_HEX["title_options"], [Overlays.STR_HEX["val_fast"]], false, 2)
	check(presenter.recorded_glyphs.size() > 0, "sndisp layout recorded glyphs")
	var has_170: bool = false
	for g in presenter.recorded_glyphs:
		if g["pos"].y == 170:
			has_170 = true
			break
	check(has_170, "sndisp layout places row 0 at Y = 170")

	# 2. talk_render layout (Y = 168, 184, 200)
	presenter.recorded_glyphs.clear()
	presenter.draw_vn_dialogue(target, Overlays.STR_HEX["title_options"], [Overlays.STR_HEX["val_fast"]], true, 0)
	var has_168: bool = false
	for g in presenter.recorded_glyphs:
		if g["pos"].y == 168:
			has_168 = true
			break
	check(has_168, "talk_render layout places row 0 at Y = 168")

# ==============================================================================
# 4. Presentation Primitives Tests
# ==============================================================================

func _test_presentation_primitives() -> void:
	print("[4/8] Testing shared presentation primitives...")

	var presenter := VnPresenter.new()
	var font_root: String = OS.get_environment("FONT_ROOT")
	if font_root.is_empty():
		font_root = DEFAULT_FONT_ROOT
	presenter.init_resources(font_root)

	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	target.fill(Color.BLACK)

	# 1. Parameter panel
	var stats: Array[int] = [99, 50, 45, 40, 35, 30, 25, 20, 15]
	presenter.draw_parameter_panel(target, stats)

	# 2. Message window
	presenter.draw_message_window(target)

	# 3. Cal base gradient panel
	presenter.draw_cal_base(target, Rect2i(24, 32, 272, 128))

	# 4. Date pillar
	presenter.draw_date_pillar(target, 95, 4, 15, 5, 0, 0)

	# 5. Title badge
	presenter.draw_title_badge(target, Overlays.STR_HEX["title_options"], Vector2i.ZERO, 5)

	# 6. Hand cursor
	presenter.draw_hand_cursor(target, Vector2i(100, 100))

	check(target.get_pixel(100, 100) != Color.BLACK, "Primitives successfully render into framebuffer")

# ==============================================================================
# 5. Options Overlay State Machine Tests
# ==============================================================================

func _test_options_overlay_state_machine() -> void:
	print("[5/8] Testing OptionsOverlay state machine & settings...")

	var opt := Overlays.create_options()
	var s: Dictionary = opt.get_settings()
	check(s["text_speed"] == 1, "Default text speed is 1 (Normal)")
	check(s["icon_mode"] == 0, "Default icon mode is 0 (Normal 14 icons)")
	check(s["voice_mode"] == 1, "Default voice mode is 1 (Voiced)")
	check(s["window_style"] == 0, "Default window style is 0 (Type A)")

	# Cursor navigation Down
	opt.process_input(Overlays.PAD_DOWN)
	check(opt.cursor == 1, "Down moves cursor to 1")

	# Cursor wrap Up
	opt.process_input(Overlays.PAD_UP)
	opt.process_input(Overlays.PAD_UP)
	check(opt.cursor == 7, "Up wraps cursor to 7")

	# Setting adjustment on Item 0 (Speed)
	opt.cursor = 0
	opt.process_input(Overlays.PAD_RIGHT)
	check(opt.text_speed == 2, "Right increases text speed to 2")
	opt.process_input(Overlays.PAD_LEFT)
	check(opt.text_speed == 1, "Left decreases text speed to 1")

	# Window style adjustment on Item 3
	opt.cursor = 3
	opt.process_input(Overlays.PAD_RIGHT)
	check(opt.window_style == 1, "Right changes window style to 1 (Type B)")

	# Scroll pattern formulas (0..5)
	for p in range(6):
		opt.checker_scroll_pattern = p
		opt.update_scroll_offsets(10)
	check(opt.scroll_u != 0 or opt.scroll_v != 0, "Scroll offsets advance under patterns")

	# Exit via Circle on item 7
	opt.cursor = 7
	var res: Dictionary = opt.process_input(Overlays.PAD_CIRCLE)
	check(res["action"] == "exit" and opt.exited, "Circle on Exit triggers overlay exit")

# ==============================================================================
# 6. Help Overlay State Machine Tests
# ==============================================================================

func _test_help_overlay_state_machine() -> void:
	print("[6/8] Testing HelpOverlay state machine & categories...")

	var help := Overlays.create_help()
	check(help.mode_stage == Overlays.HelpOverlay.STAGE_MENU_INPUT, "Initial stage is category menu input")

	# Navigation Down
	help.process_input(Overlays.PAD_DOWN)
	check(help.cursor == 1, "Down moves cursor to category 1 (MemCard)")

	# Select category 1
	var res: Dictionary = help.process_input(Overlays.PAD_CIRCLE)
	check(res["action"] == "enter_topic" and help.mode_stage == Overlays.HelpOverlay.STAGE_MEMCARD,
		"Circle enters Memory Card topic (0x20)")

	# Back to menu via Cross
	res = help.process_input(Overlays.PAD_CROSS)
	check(res["action"] == "back_to_menu" and help.mode_stage == Overlays.HelpOverlay.STAGE_MENU_INPUT,
		"Cross in topic returns to category menu")

	# Instant exit via Start button (help_questions_and_answers_page.c:39-41)
	res = help.process_input(Overlays.PAD_START)
	check(res["action"] == "return_to_game" and help.exited,
		"Start button triggers immediate return to game")

# ==============================================================================
# 7. Title Menu Overlay State Machine Tests
# ==============================================================================

func _test_title_menu_overlay_state_machine() -> void:
	print("[7/8] Testing TitleMenuOverlay state machine...")

	var title_menu := Overlays.create_title_menu()
	check(title_menu.cursor == 0, "Initial cursor is 0 (Start)")

	# Toggle cursor between 0 and 1
	title_menu.process_input(Overlays.PAD_DOWN)
	check(title_menu.cursor == 1, "Down moves cursor to 1 (Option)")

	# Confirm on Option
	var res: Dictionary = title_menu.process_input(Overlays.PAD_CIRCLE)
	check(title_menu.stage == Overlays.TitleMenuOverlay.STAGE_CONFIRM, "Circle enters confirm stage")

	# Wait out confirm delay
	for _i in range(16):
		res = title_menu.process_input(0)
	check(res.get("action") == "dispatch" and res.get("mode") == Overlays.TitleMenuOverlay.MODE_OPTION,
		"Confirm dispatches to Option screen mode 0x36")

# ==============================================================================
# 8. Visual Proof Captures & Section 12a Verification
# ==============================================================================

func _test_render_visual_captures_and_bounding_boxes() -> void:
	print("[8/8] Rendering authentic 320x240 captures & verifying Section 12a bounds...")

	var repo := ProjectSettings.globalize_path("res://../..").simplify_path()
	var font_root: String = OS.get_environment("FONT_ROOT")
	if font_root.is_empty():
		font_root = repo.path_join(".asset-cache/port/fonts/native-font-v2")
	var capture_dir: String = OS.get_environment("CAPTURE_DIR")
	if capture_dir.is_empty():
		capture_dir = repo.path_join(".claude/orchestrator/state/T-29018/shared-vn-and-system-overlays-v1/captures")

	DirAccess.make_dir_recursive_absolute(capture_dir)

	var presenter := VnPresenter.new()
	var init_err: String = presenter.init_resources(font_root)
	check(init_err.is_empty(), "Presenter init resources: " + init_err)

	var renders: Array[Dictionary] = []

	# 1. Options Menu Capture
	var opt := Overlays.create_options()
	opt.cursor = 0
	var img_options: Image = opt.render(presenter)
	renders.append({"name": "capture_options_menu.png", "image": img_options})

	# 2. Help Screen Capture
	var help := Overlays.create_help()
	help.cursor = 0
	var img_help: Image = help.render(presenter)
	renders.append({"name": "capture_help_screen.png", "image": img_help})

	# 3. Title Screen Capture
	var title_menu := Overlays.create_title_menu()
	title_menu.cursor = 0
	var img_title: Image = title_menu.render(presenter)
	renders.append({"name": "capture_title_screen.png", "image": img_title})

	# 4. Checkerboard Modes Capture (full 10x8 scrolling grid + message window)
	var img_checker := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	presenter.draw_checkerboard_frame(img_checker, 0, Vector2i(10, -5))
	presenter.draw_message_window(img_checker)
	presenter.draw_vn_dialogue(img_checker, "93a18de8", ["8358835e815b8367837b835e839382c5"], false, 4)
	renders.append({"name": "capture_checkerboard_modes.png", "image": img_checker})

	# 5. VN Dialogue Capture
	var img_vn := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	presenter.draw_checkerboard_frame(img_vn, 0, Vector2i(6, -3))
	presenter.draw_message_window(img_vn)
	var dialogue_lines: Array[String] = [
		"8358835e815b8367837b835e839382c5", # スタートボタンで (8 glyphs)
		"8351815b838082c996df82ea82dc82b7", # ゲームに戻れます (8 glyphs)
	]
	presenter.draw_vn_dialogue(img_vn, "93a18de8", dialogue_lines, false, 4)
	renders.append({"name": "capture_vn_dialogue.png", "image": img_vn})

	# Verify Section 12a bounding box gate
	check(presenter.bounding_box_violations.is_empty(),
		"Zero glyph bounding-box violations: " + str(presenter.bounding_box_violations))
	check(presenter.recorded_glyphs.size() > 50,
		"Recorded %d native SJIS glyphs rendered across overlays" % presenter.recorded_glyphs.size())

	# Save PNG visual proofs
	for item: Dictionary in renders:
		var fname: String = item["name"]
		var img: Image = item["image"]
		check(img != null and img.get_size() == Vector2i(320, 240),
			"%s rendered at exact 320x240 PS1 resolution" % fname)
		if img != null:
			var out_path: String = capture_dir.path_join(fname)
			var save_err: Error = img.save_png(out_path)
			check(save_err == OK, "Saved visual proof %s" % out_path)
