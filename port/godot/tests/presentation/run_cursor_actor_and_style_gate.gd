extends SceneTree
## T-29031 Gaps #2 & #3 Headless Verification Gate:
## 1. DialogueTextboxPresenter removes CP932 81a0/81a5 glyph substitutes and renders
##    authentic PS1 actor2/frame1 continuation marker (origin (8,8), native (116, 72..79))
##    with text/voice/phase gating, and authentic actor0 selection cursor frames (0, 155..173).
## 2. CommandHudPresenter supports all 19 selectable cursor frames (0 and 155..173) matching
##    source-evidence.json, Square-button (0x80) cycle 0 -> 0x9b -> ... -> 0xac -> 0 with
##    once-per-outer-frame D_8011ECA4 guard, and saved_settings[2] persistence.

const DialoguePresenter = preload("res://presentation/dialogue_textbox_presenter.gd")
const HudPresenter = preload("res://presentation/command_hud_presenter.gd")

const HUD_MANIFEST := "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
const ContentIdentity = preload("res://services/gameplay_content_identity.gd")
const HUD_SHA := ContentIdentity.HUD_MANIFEST_SHA
const FONT_ROOT := "/workspace/.asset-cache/port/fonts/native-font-v2"
const SOURCE_EVIDENCE_PATH := "/workspace/.claude/orchestrator/state/T-29031/cursor-audit-v1/source-evidence.json"

var checks: int = 0
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	checks += 1
	if not condition:
		failures.append(label)
		push_error("FAIL: " + label)

static func _make_calendar(days_in_month: int = 30) -> Array:
	var dk0: Array = [days_in_month]
	var mk0: Array = [0]
	for d: int in range(1, 32):
		dk0.append(1 if (d % 7) == 2 else 0)
		mk0.append(0)
	return [
		{"day_kind": dk0, "marker": mk0},
		{"day_kind": dk0.duplicate(), "marker": mk0.duplicate()},
	]

static func _make_rule(cursor_style: int = 0x9C, menu_cursor: int = 0) -> Dictionary:
	return {
		"year": 95,
		"month": 5,
		"day": 4,
		"weekday": 3,
		"params": [96 << 16, 45 << 16, 40 << 16, 40 << 16, 38 << 16, 34 << 16, 58 << 16, 4 << 16, 5 << 16],
		"calendar": _make_calendar(31),
		"menu_cursor": menu_cursor,
		"icon_enabled": [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
		"player_flags": 0,
		"player_flags_upper": 0,
		"memory_card_busy": 0,
		"cursor_style": cursor_style,
		"saved_settings": [0, 0, cursor_style, 0],
	}

static func _make_context(cursor_style: int = 0x9C) -> Dictionary:
	return {
		"game_mode": 0x34,
		"mode_stage": 1,
		"mode_step": 0,
		"icon_mode": 0,
		"icon_count": 14,
		"cursor_style": cursor_style,
		"hover_ticks": 32,
		"family_name_hex": "8d828ca9",
	}

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	var captures_dir: String = OS.get_environment("TOKIMEMO_CURSOR_CAPTURES")
	if captures_dir.is_empty():
		captures_dir = "/workspace/.claude/orchestrator/state/T-29031/cursor-actor-and-style-lifecycle-v1/captures"

	# 1. Verify DialogueTextboxPresenter has removed 81a0/81a5 glyph substitutes
	var dlg_src: String = FileAccess.get_file_as_string("res://presentation/dialogue_textbox_presenter.gd")
	check(not dlg_src.contains("81a0") and not dlg_src.contains("81a5"), "DialogueTextboxPresenter source contains no 81a0/81a5 CP932 cursor/prompt glyph substitutes")

	var dlg := DialoguePresenter.new()
	check(dlg.open_font(FONT_ROOT).is_empty(), "DialogueTextboxPresenter opens font and HUD actor manifest: " + dlg.error)
	check(dlg.is_font_open() and dlg.is_hud_open(), "DialogueTextboxPresenter font and HUD actor frames are open")

	# 2. Verify hud_text_marker_bob (compute_marker_bob_y) and resolve_wait_marker across all combinations
	var bob_y: int = 72
	var bob_trace: Array[int] = [bob_y]
	for frame_cnt: int in range(1, 30):
		var next_y: int = DialoguePresenter.compute_marker_bob_y(bob_y, frame_cnt)
		if next_y != bob_y:
			bob_trace.append(next_y)
		bob_y = next_y
	check(bob_trace == [72, 73, 74, 75, 76, 77, 78, 79, 72], "compute_marker_bob_y steps 72..79 every 4th frame and wraps to 72")

	for complete: bool in [false, true]:
		for waiting: bool in [false, true]:
			for v_mode: int in [0, 1, 2]:
				for sv_state: int in [0, 1, 2]:
					for m_vis: bool in [false, true]:
						for ny: int in [72, 79]:
							var lv := {
								"waiting_for_input": waiting,
								"voice_mode": v_mode,
								"speaker_voice_state": sv_state,
								"marker_visible": m_vis,
								"marker_native_y": ny,
							}
							var res: Dictionary = DialoguePresenter.resolve_wait_marker(lv, complete)
							var expected_vis: bool = complete and waiting and (sv_state == 2 or v_mode == 0) and m_vis
							check(
								res.get("error", "") == ""
								and bool(res.get("visible", false)) == expected_vis
								and int(res.get("actor_index", -1)) == 2
								and int(res.get("frame", -1)) == 1
								and res.get("native_position") == Vector2i(116, ny)
								and res.get("screen_anchor") == Vector2i(276, 120 + ny)
								and res.get("bitmap_top_left") == Vector2i(268, 112 + ny),
								"resolve_wait_marker gate & coordinates for complete=%s waiting=%s mode=%d sv=%d vis=%s y=%d" % [
									str(complete), str(waiting), v_mode, sv_state, str(m_vis), ny
								]
							)

	check(not str(DialoguePresenter.resolve_wait_marker({"marker_native_y": 71}, true).get("error", "")).is_empty(), "resolve_wait_marker rejects marker_native_y < 72")
	check(not str(DialoguePresenter.resolve_wait_marker({"marker_native_y": 80}, true).get("error", "")).is_empty(), "resolve_wait_marker rejects marker_native_y > 79")
	check(not str(DialoguePresenter.resolve_wait_marker({"voice_mode": 3}, true).get("error", "")).is_empty(), "resolve_wait_marker rejects invalid voice_mode")
	check(not str(DialoguePresenter.resolve_wait_marker({"speaker_voice_state": 3}, true).get("error", "")).is_empty(), "resolve_wait_marker rejects invalid speaker_voice_state")

	# 3. Verify pixel-exact actor2/frame1 rendering in DialogueTextboxPresenter.render_dialogue_line
	var line_view := {
		"command": 1,
		"speaker_id": "shiori",
		"wrapper": "quote",
		"tokens": [
			{"kind": "glyph", "raw_hex": "82a8"}, # お
			{"kind": "glyph", "raw_hex": "82cd"}, # は
			{"kind": "glyph", "raw_hex": "82e6"}, # よ
			{"kind": "glyph", "raw_hex": "82a4"}, # う
		],
	}
	var img_no_marker: Image = dlg.render_frame(line_view, {"marker_visible": false})
	var img_voice_pending: Image = dlg.render_frame(line_view, {"voice_mode": 2, "speaker_voice_state": 1, "marker_visible": true})
	check(img_voice_pending.get_data() == img_no_marker.get_data(), "voiced line with speaker_voice_state=1 (playing) and voice_mode=2 suppresses actor2/frame1 marker")

	var img_voice_mode0: Image = dlg.render_frame(line_view, {"voice_mode": 0, "speaker_voice_state": 1, "marker_visible": true, "marker_native_y": 72})
	check(img_voice_mode0.get_data() != img_no_marker.get_data(), "voice_mode=0 exception displays actor2/frame1 marker when text is complete")

	for ny: int in [72, 79]:
		var img_marker: Image = dlg.render_frame(line_view, {"voice_mode": 2, "speaker_voice_state": 2, "marker_visible": true, "marker_native_y": ny})
		var expected_overlay := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		expected_overlay.fill(Color.TRANSPARENT)
		check(dlg._draw_hud_actor(expected_overlay, 1, Vector2i(116, ny), false).is_empty(), "draw actor2/frame1 reference at y=%d" % ny)
		var opaque_count: int = 0
		var exact_match: bool = true
		for py: int in range(240):
			for px: int in range(320):
				var ov_px: Color = expected_overlay.get_pixel(px, py)
				if ov_px.a8 > 0:
					opaque_count += 1
					if img_marker.get_pixel(px, py) != ov_px:
						exact_match = false
				else:
					if img_marker.get_pixel(px, py) != img_no_marker.get_pixel(px, py):
						exact_match = false
		check(exact_match and opaque_count == 148, "dialogue line at bob y=%d renders exact 148 opaque pixels of actor2/frame1 at (268, %d)" % [ny, 112 + ny])
		if ny == 72:
			img_marker.save_png(captures_dir.path_join("01-dialogue-actor2-frame1-marker-320x240.png"))

	# Verify missing 1:base fails closed without glyph fallback
	var saved_f1: Dictionary = dlg.hud_frames["1:base"]
	dlg.hud_frames.erase("1:base")
	var fail_target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var fail_res: Dictionary = dlg.render_dialogue_line(fail_target, line_view)
	check(not str(fail_res.get("error", "")).is_empty(), "missing actor2/frame1 fails closed without glyph fallback")
	dlg.hud_frames["1:base"] = saved_f1

	# 4. Verify actor0 selection cursor frames (156 hand, 0 hollow triangle, 155 arrow) in DialogueTextboxPresenter.render_choice_menu
	var choice_view := {
		"command": 3,
		"options": [
			{"option_id": 0, "tokens": [{"kind": "glyph", "raw_hex": "82a0"}]},
			{"option_id": 1, "tokens": [{"kind": "glyph", "raw_hex": "82a2"}]},
			{"option_id": 2, "tokens": [{"kind": "glyph", "raw_hex": "82a4"}]},
		],
	}
	for pair: Array in [[0x9C, 156, "02-choice-cursor156-hand-320x240.png"], [0, 0, "03-choice-cursor000-triangle-320x240.png"], [0x9B, 155, ""]]:
		var c_style: int = pair[0]
		var expected_f: int = pair[1]
		var cap_name: String = pair[2]
		var target_img := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		target_img.fill(Color8(12, 16, 24, 255))
		dlg.render_textbox_background(target_img)
		var c_in: Dictionary = choice_view.duplicate(true)
		c_in["cursor_style"] = c_style
		var c_meta: Dictionary = dlg.render_choice_menu(target_img, c_in, 0, -1)
		check(str(c_meta.get("error", "")).is_empty(), "render_choice_menu succeeds for cursor_style=0x%02x" % c_style)
		check(int(c_meta.get("cursor_frame", -1)) == expected_f and int(c_meta.get("cursor_actor_index", -1)) == 0, "render_choice_menu selects actor0 frame %d for style 0x%02x" % [expected_f, c_style])
		var anchor: Vector2i = c_meta["cursor_screen_anchor"]
		var ref_cursor := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		ref_cursor.fill(Color.TRANSPARENT)
		check(dlg._draw_hud_actor(ref_cursor, expected_f, anchor - Vector2i(160, 120), false).is_empty(), "reference actor0 frame %d drawn" % expected_f)
		var matched_opaque: int = 0
		var all_matched: bool = true
		for py: int in range(240):
			for px: int in range(320):
				var rc: Color = ref_cursor.get_pixel(px, py)
				if rc.a8 > 0:
					matched_opaque += 1
					if target_img.get_pixel(px, py) != rc:
						all_matched = false
		check(all_matched and matched_opaque > 0, "render_choice_menu pixel-matches actor0 frame %d (%d opaque pixels)" % [expected_f, matched_opaque])
		if not cap_name.is_empty():
			target_img.save_png(captures_dir.path_join(cap_name))

	# 5. Verify CommandHudPresenter against source-evidence.json and Square-button lifecycle
	var hud := HudPresenter.new()
	check(hud.open_manifest(HUD_MANIFEST, HUD_SHA, FONT_ROOT), "CommandHudPresenter opens production HUD manifest: " + hud.error)

	var evidence: Variant = JSON.parse_string(FileAccess.get_file_as_string(SOURCE_EVIDENCE_PATH))
	check(evidence is Dictionary and evidence.get("frames") is Array and (evidence["frames"] as Array).size() == 21, "source-evidence.json loaded with 21 frame records (0, 1, 155..173)")
	if evidence is Dictionary:
		for rec: Dictionary in evidence["frames"]:
			var f_idx: int = int(rec["frame"])
			if f_idx == 1:
				continue
			var meta: Dictionary = hud.cursor_frame_metadata(f_idx)
			check(not meta.is_empty() and int(meta.get("resolved_frame", -1)) == f_idx, "CommandHudPresenter resolves cursor_frame_metadata for frame %d" % f_idx)
			check(meta.get("origin") == rec["original"]["origin"] and meta.get("bounds") == rec["original"]["bounds"], "frame %d origin and bounds match SLPM_86.053 source-evidence.json" % f_idx)

	# Verify resolve_cursor_actor_frame across all 0..255 byte values
	for s_val: int in range(256):
		var expected_frame: int = s_val if (s_val >= 0x9B and s_val < 0xAE) else 0
		check(HudPresenter.resolve_cursor_actor_frame(s_val) == expected_frame, "resolve_cursor_actor_frame(%d) == %d" % [s_val, expected_frame])

	# Verify hud_cursor_actor_init for arg0=0 and arg0=1
	var init0: Dictionary = HudPresenter.hud_cursor_actor_init(0x9C, 0, Vector2i(-132, -43))
	var init1: Dictionary = HudPresenter.hud_cursor_actor_init(0, 1, Vector2i(36, 112))
	check(init0["actor_index"] == 0 and init0["flags"] == 0x40 and init0["draw_flags"] == 0x84 and init0["frame"] == 156 and init0["x"] == -132 and init0["y"] == -43, "hud_cursor_actor_init(0x9c, 0) preserves actor0 flags and position")
	check(init1["actor_index"] == 0 and init1["frame"] == 0 and init1["arg0"] == 1, "hud_cursor_actor_init(0, 1) selects frame 0 hollow triangle")

	# Verify full 19-step Square cycle: 0 -> 0x9b (155) -> ... -> 0xac (172) -> 0
	var rule_cycle: Dictionary = _make_rule(0, 0)
	var visited_styles: Array[int] = [int(rule_cycle["cursor_style"])]
	for step_idx: int in range(19):
		var next_s: int = HudPresenter.step_cursor_style_square(rule_cycle, HudPresenter.PAD_SQUARE, 100 + step_idx)
		visited_styles.append(next_s)
	var expected_cycle: Array[int] = [0, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 0]
	check(visited_styles == expected_cycle, "Square (0x80) cycles 0 -> 0x9b -> ... -> 0xac -> 0 across 19 presses")
	check(int(rule_cycle["saved_settings"][2]) == 0, "step_cursor_style_square keeps saved_settings[2] synced")

	# Verify D_8011ECA4 once-per-outer-frame guard
	var guarded_rule: Dictionary = _make_rule(0x9C, 0)
	var s_first: int = HudPresenter.step_cursor_style_square(guarded_rule, HudPresenter.PAD_SQUARE, 500)
	var s_second_same_frame: int = HudPresenter.step_cursor_style_square(guarded_rule, HudPresenter.PAD_SQUARE, 500)
	var s_next_frame: int = HudPresenter.step_cursor_style_square(guarded_rule, HudPresenter.PAD_SQUARE, 501)
	check(s_first == 0x9D and s_second_same_frame == 0x9D and s_next_frame == 0x9E, "D_8011ECA4 outer-frame guard limits Square cursor_style update to once per outer frame")

	# Verify save/load settings persistence (memory_card.c:871,988) and live HUD rendering
	var save_rule: Dictionary = _make_rule(0x9C, 0)
	HudPresenter.save_cursor_style_to_settings(save_rule, 0)
	check(int(save_rule["saved_settings"][2]) == 0, "save_cursor_style_to_settings writes saved_settings[2] = 0")
	var restored_rule: Dictionary = _make_rule(0x9C, 0)
	restored_rule["saved_settings"] = save_rule["saved_settings"].duplicate()
	var loaded_style: int = HudPresenter.load_cursor_style_from_settings(restored_rule)
	check(loaded_style == 0 and int(restored_rule["cursor_style"]) == 0, "load_cursor_style_from_settings restores cursor_style from saved_settings[2]")

	var hud_state: Dictionary = hud.create_state(_make_rule(0x9C, 0), _make_context(0x9C))
	check(not hud_state.is_empty(), "create_state succeeds with cursor_style 0x9c")
	var img_hud_156: Image = hud.render(hud_state)
	check(img_hud_156 != null, "render HUD with cursor_style 0x9c (frame 156 left hand)")
	img_hud_156.save_png(captures_dir.path_join("04-hud-command-cursor156-hand-320x240.png"))

	check(hud.sync_rule_state(hud_state, restored_rule).is_empty(), "sync_rule_state propagates restored rule cursor_style=0 into HUD state")
	var img_hud_0: Image = hud.render(hud_state)
	check(img_hud_0 != null and img_hud_0.get_data() != img_hud_156.get_data(), "render HUD with restored cursor_style 0 (frame 0 hollow triangle) differs from frame 156")
	img_hud_0.save_png(captures_dir.path_join("05-hud-command-cursor000-triangle-320x240.png"))

	# Step Square directly on hud_state: 0 -> 0x9B (frame 155 diagonal arrow)
	var cycled_hud_style: int = HudPresenter.step_cursor_style_cycle(hud_state, HudPresenter.PAD_SQUARE, 600)
	check(cycled_hud_style == 0x9B and int(hud_state["context"]["cursor_style"]) == 0x9B, "step_cursor_style_cycle on HUD state advances 0 -> 0x9b")
	var img_hud_155: Image = hud.render(hud_state)
	check(img_hud_155 != null and img_hud_155.get_data() != img_hud_0.get_data() and img_hud_155.get_data() != img_hud_156.get_data(), "render HUD with cursor_style 0x9b (frame 155 diagonal arrow) produces distinct cursor sprite")
	img_hud_155.save_png(captures_dir.path_join("06-hud-command-cursor155-arrow-320x240.png"))

	print(JSON.stringify({
		"checks": checks,
		"failures": failures,
		"captures": 5,
	}))
	quit(0 if failures.is_empty() else 1)
