extends SceneTree
## Tests for T-29011:
## 1. Dialogue token substitutions & inline label extraction
## 2. Evidenced voice service gating & confirmation gates
## 3. DialogueTextboxPresenter native 320x240 rendering & NativeCanvas presentation
## 4. Visual proof PNG generation from real SERIFU data and choices

const TalkPlayer = preload("res://dialogue/talk_player.gd")
const Tokens = preload("res://dialogue/text_tokens.gd")
const Presenter = preload("res://presentation/dialogue_textbox_presenter.gd")
const Canvas = preload("res://presentation/native_canvas.gd")

const SOURCE_PATH := "CDROM/SERIFU/SERIFU.BIN"
const SOURCE_SHA := "5d6e1014c69769f18f7877ad801f04f872a9829c05d4c8509602adbccd5f22ee"
const PROFILE_ID := "serifu-serifu"
const SECTION_ID := "table-0"

const MOCK_CHOICE_TOKENS := [
	{"kind": "glyph", "raw_hex": "8140"},
	{"kind": "separator", "role": "choice_option", "code": "q", "raw_hex": "71"},
	{"kind": "glyph", "raw_hex": "8141"},
	{"kind": "separator", "role": "choice_option", "code": "q", "raw_hex": "71"},
	{"kind": "glyph", "raw_hex": "8142"},
]

class MockVoiceService:
	var playing: bool = false
	var finished: bool = false
	var last_clip: Dictionary = {}
	var stopped: bool = false

	func play_clip(speaker: String, clip_start: int, clip_end: int) -> void:
		playing = true
		finished = false
		last_clip = {"speaker": speaker, "clip_start": clip_start, "clip_end": clip_end}

	func is_voice_complete() -> bool:
		return finished

	func stop_clip() -> void:
		playing = false
		stopped = true

func _initialize() -> void:
	call_deferred("_run")

func _run() -> void:
	print("--- Running T-29011 dialogue presentation tests ---")
	_test_existing_contract_invariants()
	_test_substitution_resolution()
	_test_inline_label_extraction()
	_test_voice_gating_behavior()
	_test_textbox_presentation_and_captures()
	print("--- All T-29011 dialogue presentation tests passed successfully ---")
	quit(0)

func _test_existing_contract_invariants() -> void:
	# Verifies all baseline assertions from run_dialogue_tests.gd
	var player := TalkPlayer.new()
	var content: Dictionary = _make_content(3)
	content["content_sha256"] = player._component_digest(content)
	var route: Dictionary = _make_route(content)
	assert(player.load_content(content, route).is_empty())
	var menu: Dictionary = player.present_choice(0)
	assert(menu["order"] == [0, 1, 2])
	assert(menu["options"].size() == 3)
	assert(player.choose(menu["options"][1]["option_id"], false, true)["error"].contains("text completion"))
	assert(player.choose(menu["options"][1]["option_id"], true, false)["waiting"])
	assert(player.advance_line(true).contains("consumed"))
	var picked: Dictionary = player.choose(menu["options"][1]["option_id"], true, true)
	assert(picked["source_choice"] == 1)
	assert(player.advance_line(false).is_empty())
	assert(player.current_record()["kind"] == "line")
	assert(player.advance_line(true).is_empty())
	assert(player.current_record()["kind"] == "group_end")
	assert(player.advance_line(false).is_empty())
	assert(player.current_record()["kind"] == "group_end")
	assert(player.advance_line(true).is_empty())
	assert(player.current_record()["kind"] == "complete")

	var shuffled := TalkPlayer.three_choice_order(1)
	assert(shuffled == [1, 2, 0])
	for frame: int in range(12):
		var order: Array[int] = TalkPlayer.three_choice_order(frame)
		order.sort()
		assert(order == [0, 1, 2])

	var line_player := TalkPlayer.new()
	var line_content: Dictionary = _make_content(6)
	line_content["content_sha256"] = line_player._component_digest(line_content)
	assert(line_player.load_content(line_content, _make_route(line_content)).is_empty())
	assert(line_player.present_line()["speaker_id"] == "player_family")
	assert(line_player.confirm_line(false, true)["waiting"] == "typewriter")
	assert(line_player.confirm_line(true, false)["waiting"] == "confirm")
	assert(line_player.advance_line(false).is_empty())
	assert(line_player.confirm_line(true, true)["ready_to_advance"])
	assert(line_player.advance_line(false).is_empty())
	assert(line_player.advance_line(true).is_empty())
	assert(line_player.current_record()["kind"] == "group_end")

	# Unvoiced default voice rejection
	var voice_content: Dictionary = line_content.duplicate(true)
	voice_content["groups"][0]["records"][0]["voice"] = {
		"speaker": "runtime_current_girl",
		"pairs": [{"variant": 0, "clip_start": 1, "clip_end": 2}],
	}
	voice_content["content_sha256"] = line_player._component_digest(voice_content)
	var before_record: Dictionary = line_player.current_record()
	var voice_err: String = line_player.load_content(voice_content, _make_route(voice_content))
	assert(voice_err.contains("voice-bearing dialogue is unsupported until an evidenced voice service is available"))
	assert(line_player.current_record() == before_record)
	print("  [PASS] Baseline dialogue contract invariants verified")

func _test_substitution_resolution() -> void:
	# 1. Full context covering all standard substitution keys
	var context := {
		"player_form": "826a826b",    # ＫＬ
		"player_family": "93a18de8",  # 藤崎
		"player_given": "8e8d9044",   # 詩織
		"player_full": "93a18de88e8d9044", # 藤崎詩織
		"girl_name": "93f896ec",      # 虹野
		"yoshio_name": "8d449759",    # 好雄
		"context_s": "8272",          # Ｓ
		"context_o": "826e",          # Ｏ
		"context_w": "8276",          # Ｗ
		"context_a": "8260",          # Ａ
		"context_m": "826c",          # Ｍ
	}

	var raw_tokens := [
		{"kind": "substitution", "code": "P", "name": "player_form", "raw_hex": "50"},
		{"kind": "glyph", "raw_hex": "8141"}, # 、
		{"kind": "substitution", "code": "p", "name": "player_family", "raw_hex": "70"},
		{"kind": "substitution", "code": "m", "name": "player_given", "raw_hex": "6d"},
		{"kind": "substitution", "code": "F", "name": "player_full", "raw_hex": "46"},
		{"kind": "substitution", "code": "G", "name": "girl_name", "raw_hex": "47"},
		{"kind": "substitution", "code": "Y", "name": "yoshio_name", "raw_hex": "59"},
		{"kind": "substitution", "code": "S", "name": "context_s", "raw_hex": "53"},
		{"kind": "substitution", "code": "O", "name": "context_o", "raw_hex": "4f"},
		{"kind": "substitution", "code": "W", "name": "context_w", "raw_hex": "57"},
		{"kind": "substitution", "code": "A", "name": "context_a", "raw_hex": "41"},
		{"kind": "substitution", "code": "M", "name": "context_m", "raw_hex": "4d"},
	]

	# Snapshot original tokens array for immutability check
	var original_snapshot: Array = raw_tokens.duplicate(true)

	var res: Dictionary = Tokens.resolve_tokens(raw_tokens, context)
	assert(res["error"].is_empty())
	# Total glyphs: 2(P) + 1(glyph) + 2(p) + 2(m) + 4(F) + 2(G) + 2(Y) + 1(S) + 1(O) + 1(W) + 1(A) + 1(M) = 20
	assert(res["tokens"].size() == 20)
	assert(res["tokens"][0]["raw_hex"] == "826a")
	assert(res["tokens"][1]["raw_hex"] == "826b")
	assert(res["tokens"][2]["raw_hex"] == "8141")
	assert(res["tokens"][3]["raw_hex"] == "93a1")
	assert(res["tokens"][4]["raw_hex"] == "8de8")

	# Check immutability of raw_tokens
	assert(raw_tokens == original_snapshot)

	# 2. Player full synthesized from family + given
	var context_no_full := {
		"player_family": "93a18de8",
		"player_given": "8e8d9044",
	}
	var f_tokens := [{"kind": "substitution", "code": "F", "name": "player_full", "raw_hex": "46"}]
	var f_res: Dictionary = Tokens.resolve_tokens(f_tokens, context_no_full)
	assert(f_res["error"].is_empty())
	assert(f_res["tokens"].size() == 4)
	assert(f_res["tokens"][0]["raw_hex"] == "93a1")
	assert(f_res["tokens"][1]["raw_hex"] == "8de8")
	assert(f_res["tokens"][2]["raw_hex"] == "8e8d")
	assert(f_res["tokens"][3]["raw_hex"] == "9044")

	# 3. Fail closed on missing required context key
	var empty_context := {}
	var missing_res: Dictionary = Tokens.resolve_tokens(raw_tokens, empty_context)
	assert(missing_res["error"].contains("missing required substitution context key: player_form"))

	var partial_context := {"player_form": "826a826b"}
	var missing_p: Dictionary = Tokens.resolve_tokens(raw_tokens, partial_context)
	assert(missing_p["error"].contains("missing required substitution context key: player_family"))

	# 4. Fail closed on malformed substitution values (odd hex, non-hex, empty)
	var odd_hex_context := {"player_form": "826"}
	var odd_res: Dictionary = Tokens.resolve_tokens([{"kind": "substitution", "code": "P", "name": "player_form"}], odd_hex_context)
	assert(odd_res["error"].contains("must be non-empty 2-byte Shift-JIS hex"))

	var non_hex_context := {"player_form": "ZZZZ"}
	var non_res: Dictionary = Tokens.resolve_tokens([{"kind": "substitution", "code": "P", "name": "player_form"}], non_hex_context)
	assert(non_res["error"].contains("must be non-empty 2-byte Shift-JIS hex"))

	var empty_hex_context := {"player_form": ""}
	var empty_res: Dictionary = Tokens.resolve_tokens([{"kind": "substitution", "code": "P", "name": "player_form"}], empty_hex_context)
	assert(empty_res["error"].contains("must be non-empty 2-byte Shift-JIS hex"))

	# 5. PackedByteArray context value support
	var bytes_context := {"player_family": PackedByteArray([0x93, 0xa1, 0x8d, 0xe8])}
	var bytes_res: Dictionary = Tokens.resolve_tokens([{"kind": "substitution", "code": "p", "name": "player_family"}], bytes_context)
	assert(bytes_res["error"].is_empty())
	assert(bytes_res["tokens"].size() == 2)
	assert(bytes_res["tokens"][0]["raw_hex"] == "93a1")
	assert(bytes_res["tokens"][1]["raw_hex"] == "8de8")

	print("  [PASS] Token substitutions and fail-closed validation verified")

func _test_inline_label_extraction() -> void:
	# Inline speaker label with '<' marker (monologue with inline speaker)
	var paren_tokens := [
		{"kind": "inline_label", "marker": "<", "label_raw_hex": "8d449759", "raw_hex": "3c8d449759"},
		{"kind": "glyph", "raw_hex": "8140"},
		{"kind": "glyph", "raw_hex": "8142"},
	]
	var paren_res: Dictionary = Tokens.resolve_tokens(paren_tokens, {})
	assert(paren_res["error"].is_empty())
	assert(paren_res["inline_label"] != null)
	assert(paren_res["wrapper"] == "parenthesis")
	assert(paren_res["speaker_id"] == "8d449759")
	assert(paren_res["tokens"].size() == 2) # inline_label excluded from body tokens
	assert(paren_res["tokens"][0]["raw_hex"] == "8140")

	# Inline speaker label with '>' marker (bracket quote)
	var quote_tokens := [
		{"kind": "inline_label", "marker": ">", "label_raw_hex": "93a18de8", "raw_hex": "3e93a18de8"},
		{"kind": "glyph", "raw_hex": "8140"},
	]
	var quote_res: Dictionary = Tokens.resolve_tokens(quote_tokens, {})
	assert(quote_res["error"].is_empty())
	assert(quote_res["inline_label"] != null)
	assert(quote_res["wrapper"] == "bracket")
	assert(quote_res["speaker_id"] == "93a18de8")
	assert(quote_res["tokens"].size() == 1)

	print("  [PASS] Inline speaker label extraction verified")

func _test_voice_gating_behavior() -> void:
	var player := TalkPlayer.new()
	var mock_service := MockVoiceService.new()
	player.set_voice_service(mock_service)

	# Voiced content with command 1 (girl speech)
	var voiced_content: Dictionary = _make_content(1)
	voiced_content["groups"][0]["records"][0]["voice"] = {
		"speaker": "runtime_current_girl",
		"pairs": [{"variant": 0, "clip_start": 0x100, "clip_end": 0x200}],
	}
	voiced_content["content_sha256"] = player._component_digest(voiced_content)

	# Loading with allow_voiced option or voice service succeeds
	var load_err: String = player.load_content(voiced_content, _make_route(voiced_content), {"allow_voiced": true})
	assert(load_err.is_empty())

	# Presenting voiced line
	var line_view: Dictionary = player.present_line()
	assert(line_view["error"].is_empty())
	assert(line_view["voice_required"] == true)
	assert(line_view["voice_info"]["clip_start"] == 0x100)
	assert(line_view["voice_info"]["clip_end"] == 0x200)

	# Typewriter not complete -> waiting on typewriter
	var gate1: Dictionary = player.confirm_line(false, true, true)
	assert(gate1["waiting"] == "typewriter")

	# Voice not complete -> waiting on voice
	mock_service.finished = false
	var gate2: Dictionary = player.confirm_line(true, true, false)
	assert(gate2["waiting"] == "voice")

	# Advance blocked until line confirmed
	var adv_err: String = player.advance_line(true)
	assert(adv_err.contains("must complete its native input gate"))

	# Confirm not pressed -> waiting on confirm
	mock_service.finished = true
	var gate3: Dictionary = player.confirm_line(true, false, true)
	assert(gate3["waiting"] == "confirm")

	# All gates satisfied -> ready to advance
	var gate4: Dictionary = player.confirm_line(true, true, true)
	assert(gate4["ready_to_advance"] == true)
	assert(player.advance_line(true).is_empty())
	assert(player.current_record()["kind"] == "group_end")

	# Voice cancel/override test
	var player2 := TalkPlayer.new()
	player2.load_content(voiced_content, _make_route(voiced_content), {"allow_voiced": true})
	assert(player2.confirm_line(true, true, false)["waiting"] == "voice")
	player2.cancel_voice()
	var override_gate: Dictionary = player2.confirm_line(true, true, false)
	assert(override_gate["ready_to_advance"] == true)

	print("  [PASS] Voice gating, clip metadata emission, and cancel overrides verified")

func _test_textbox_presentation_and_captures() -> void:
	var presenter := Presenter.new()
	var font_root: String = "/workspace/.asset-cache/port/fonts/native-font-v2"
	var font_err: String = presenter.open_font(font_root)
	assert(font_err.is_empty(), "Failed to open external font: " + font_err)
	assert(presenter.is_font_open())

	var captures_dir := "/workspace/.claude/orchestrator/state/T-29011/substitutions-voice-v1/captures"

	# 1. Render actual SERIFU monologue from real table-0
	var actual_serifu_path := "/workspace/.asset-cache/port/T-29011-serifu-table0-v2/dialogue.json"
	var serifu_data: Variant = JSON.parse_string(FileAccess.get_file_as_string(actual_serifu_path))
	assert(serifu_data is Dictionary)

	var serifu_player := TalkPlayer.new()
	var serifu_route := {
		"source_path": serifu_data["source"]["path"],
		"source_sha256": serifu_data["source"]["sha256"],
		"profile_id": serifu_data["profile_id"],
		"section_id": serifu_data["section_id"],
		"content_sha256": serifu_data["content_sha256"],
	}
	assert(serifu_player.load_content(serifu_data, serifu_route).is_empty())
	var serifu_line0: Dictionary = serifu_player.present_line()
	assert(serifu_line0["error"].is_empty())
	assert(serifu_line0["wrapper"] == "parenthesis")

	# Capture A: Complete SERIFU monologue line (4 logical rows scrolled to last 3 visible rows)
	var render_meta: Dictionary = presenter.render_dialogue_line(
		Image.create(320, 240, false, Image.FORMAT_RGBA8), serifu_line0
	)
	assert(render_meta["error"].is_empty())
	assert(render_meta["visible_row_count"] <= 3, "Textbox must never display more than 3 visible rows")
	assert(render_meta["first_visible_row"] == 1, "4-row SERIFU monologue must scroll row 0 off via check_k_scroll")

	var img_complete: Image = presenter.render_frame(serifu_line0)
	_assert_no_pixels_outside_textbox(img_complete, "capture_serifu_monologue")
	var path_complete: String = captures_dir.path_join("capture_serifu_monologue.png")
	var err_save1: Error = img_complete.save_png(path_complete)
	assert(err_save1 == OK, "Failed to save capture_serifu_monologue.png")
	assert(FileAccess.file_exists(path_complete) and FileAccess.get_size(path_complete) > 0)

	# Capture B: Partial typewriter reveal (5 glyphs)
	var img_typewriter: Image = presenter.render_frame(serifu_line0, {"visible_glyph_count": 5})
	_assert_no_pixels_outside_textbox(img_typewriter, "capture_serifu_typewriter")
	var path_typewriter: String = captures_dir.path_join("capture_serifu_typewriter.png")
	var err_save2: Error = img_typewriter.save_png(path_typewriter)
	assert(err_save2 == OK, "Failed to save capture_serifu_typewriter.png")
	assert(FileAccess.file_exists(path_typewriter) and FileAccess.get_size(path_typewriter) > 0)

	# 2. Render 3-Choice Menu
	var choice_player := TalkPlayer.new()
	var choice_content: Dictionary = _make_content(3)
	choice_content["content_sha256"] = choice_player._component_digest(choice_content)
	assert(choice_player.load_content(choice_content, _make_route(choice_content)).is_empty())
	var choice_view: Dictionary = choice_player.present_choice(0)
	assert(choice_view["error"].is_empty())

	# Capture C: 3-Choice prompt with Option 0 selected
	var img_choice0: Image = presenter.render_frame(choice_view, {"selected_index": 0})
	_assert_no_pixels_outside_textbox(img_choice0, "capture_choices_option0")
	var path_choice0: String = captures_dir.path_join("capture_choices_option0.png")
	var err_save3: Error = img_choice0.save_png(path_choice0)
	assert(err_save3 == OK, "Failed to save capture_choices_option0.png")
	assert(FileAccess.file_exists(path_choice0) and FileAccess.get_size(path_choice0) > 0)

	# Capture D: 3-Choice prompt with Option 1 selected
	var img_choice1: Image = presenter.render_frame(choice_view, {"selected_index": 1})
	_assert_no_pixels_outside_textbox(img_choice1, "capture_choices_option1")
	var path_choice1: String = captures_dir.path_join("capture_choices_option1.png")
	var err_save4: Error = img_choice1.save_png(path_choice1)
	assert(err_save4 == OK, "Failed to save capture_choices_option1.png")
	assert(FileAccess.file_exists(path_choice1) and FileAccess.get_size(path_choice1) > 0)

	# 3. Render line with speaker label and substitutions expanded
	var sub_context := {
		"player_family": "93a18de8",  # 藤崎
		"player_given": "8e8d9044",   # 詩織
		"player_full": "93a18de88e8d9044",
		"girl_name": "93f896ec",      # 虹野
		"yoshio_name": "8d449759",    # 好雄
	}
	var voiced_sub_content: Dictionary = _make_content(11) # Shiori speech
	voiced_sub_content["groups"][0]["records"][0]["text"]["tokens"] = [
		{"kind": "substitution", "code": "p", "name": "player_family", "raw_hex": "70"},
		{"kind": "glyph", "raw_hex": "82ad"}, # く
		{"kind": "glyph", "raw_hex": "82f1"}, # ん
		{"kind": "glyph", "raw_hex": "8141"}, # 、
		{"kind": "glyph", "raw_hex": "82a8"}, # お
		{"kind": "glyph", "raw_hex": "82cd"}, # は
		{"kind": "glyph", "raw_hex": "82e6"}, # よ
		{"kind": "glyph", "raw_hex": "82a4"}, # う
		{"kind": "glyph", "raw_hex": "8142"}, # 。
	]
	voiced_sub_content["groups"][0]["records"][0]["text"]["raw_hex"] = "7082ad82f1814182a882cd82e682a48142"
	voiced_sub_content["groups"][0]["records"][0]["voice"] = {
		"speaker": "shiori",
		"pairs": [{"variant": 0, "clip_start": 50, "clip_end": 100}],
	}
	voiced_sub_content["content_sha256"] = choice_player._component_digest(voiced_sub_content)

	var shiori_player := TalkPlayer.new()
	shiori_player.set_substitution_context(sub_context)
	assert(shiori_player.load_content(voiced_sub_content, _make_route(voiced_sub_content), {"allow_voiced": true}).is_empty())
	var shiori_line: Dictionary = shiori_player.present_line()
	assert(shiori_line["error"].is_empty())

	# Capture E: Speaker label + substitutions expanded
	var img_speaker_sub: Image = presenter.render_frame(shiori_line)
	_assert_no_pixels_outside_textbox(img_speaker_sub, "capture_dialogue_speaker_substitution")
	var path_speaker_sub: String = captures_dir.path_join("capture_dialogue_speaker_substitution.png")
	var err_save5: Error = img_speaker_sub.save_png(path_speaker_sub)
	assert(err_save5 == OK, "Failed to save capture_dialogue_speaker_substitution.png")
	assert(FileAccess.file_exists(path_speaker_sub) and FileAccess.get_size(path_speaker_sub) > 0)

	# 4. Canvas presentation integration check
	var canvas := Canvas.new()
	var canvas_err: String = presenter.present_to_canvas(canvas, img_complete, "serifu_frame")
	assert(canvas_err.is_empty(), "NativeCanvas present failed: " + canvas_err)
	canvas.free()

	print("  [PASS] Textbox presentation, NativeCanvas integration, and visual captures verified")

func _assert_no_pixels_outside_textbox(image: Image, label: String) -> void:
	var bg_fill := Color8(12, 16, 24, 255)
	var box := Presenter.TEXTBOX_RECT
	for y: int in range(image.get_height()):
		for x: int in range(image.get_width()):
			if box.has_point(Vector2i(x, y)):
				continue
			var px := image.get_pixel(x, y)
			assert(px == bg_fill, "%s drew pixel outside TEXTBOX_RECT at (%d, %d): %s" % [label, x, y, str(px)])

func _make_route(content: Dictionary) -> Dictionary:
	return {
		"source_path": content["source"]["path"],
		"source_sha256": content["source"]["sha256"],
		"profile_id": content["profile_id"],
		"section_id": content["section_id"],
		"content_sha256": content["content_sha256"],
	}

func _make_content(command: int) -> Dictionary:
	var row: Dictionary = {
		"id": "talk:%s:%s:g0000:l0000" % [SOURCE_PATH, SECTION_ID],
		"line": 0,
		"command": command,
		"command_name": "choice_three" if command == 3 else "player_parenthesis_b" if command in [4, 6] else "girl",
		"source_locator": {
			"path": SOURCE_PATH,
			"source_sha256": SOURCE_SHA,
			"text_offset": 200,
			"command_offset": 100,
		},
		"text": {
			"raw_hex": "8140718141718142",
			"tokens": MOCK_CHOICE_TOKENS,
			"unknown_token_count": 0,
			"inline_label": null,
		},
	}
	var terminator: Dictionary = {
		"id": "talk:%s:%s:g0000:l0001" % [SOURCE_PATH, SECTION_ID],
		"line": 1,
		"command": 255,
		"command_name": "end",
		"source_locator": {
			"path": SOURCE_PATH,
			"source_sha256": SOURCE_SHA,
			"text_offset": null,
			"command_offset": 101,
		},
	}
	return {
		"schema": "tokimemo.dialogue-content.v1",
		"profile": "serifu-native-v1",
		"source": {
			"path": SOURCE_PATH,
			"sha256": SOURCE_SHA,
			"size_bytes": 4096,
			"reader_schema": "tokimemo.text.v1",
		},
		"profile_id": PROFILE_ID,
		"section_id": SECTION_ID,
		"groups": [{"index": 0, "records": [row, terminator]}],
		"voice_timing": "preserved_extents_runtime_mapping_unproved",
		"text_encoding": "original_bytes_with_reader_token_view",
		"semantic_coverage_complete": false,
		"content_sha256": "",
	}
