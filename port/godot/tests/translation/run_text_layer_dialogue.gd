extends SceneTree
## T-39207 (Phase 4 English WP-G): English text layer hooks on NativeTalkPlayer,
## DialogueTextboxPresenter and VnScenePresenter.draw_vn_dialogue.
##
## Synthetic dialogue rows and a synthetic text-layer pack (no game text);
## the original font and HUD actor resources are read from .asset-cache
## (FONT_ROOT, default /workspace/.asset-cache/port/fonts/native-font-v2;
## HUD_MANIFEST/HUD_SHA256, default hud-production-v1). Each hook passes
## the triplet: native frame, English frame (changes only inside the message
## window), cleared frame byte-identical to native; every rejected lookup or
## fit also leaves the native frame.
const Store = preload("res://services/text_layer_store.gd")
const TalkPlayer = preload("res://dialogue/talk_player.gd")
const Textbox = preload("res://presentation/dialogue_textbox_presenter.gd")
const Vn = preload("res://presentation/vn_scene_presenter.gd")
const Flow = preload("res://presentation/native_text_flow.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const SOURCE_PATH := "CDROM/SERIFU/SERIFU.BIN"
const SOURCE_SHA := "5d6e1014c69769f18f7877ad801f04f872a9829c05d4c8509602adbccd5f22ee"
const SECTION := "table-0"
const GIRL_NAME := "8e8d9044"
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

static func rid(line: int) -> String:
	return "talk:%s:%s:g0000:l%04d" % [SOURCE_PATH, SECTION, line]

## Full-width code units of ASCII letters, digits, space and . , ! ?
static func fw(text: String) -> String:
	var punctuation := {" ": 0x8140, ".": 0x8144, ",": 0x8143, "!": 0x8149, "?": 0x8148}
	var result := ""
	for character: String in text:
		var value := character.unicode_at(0)
		var code: int = punctuation.get(character, 0)
		if value >= 0x41 and value <= 0x5A:
			code = 0x8260 + value - 0x41
		elif value >= 0x61 and value <= 0x7A:
			code = 0x8281 + value - 0x61
		elif value >= 0x30 and value <= 0x39:
			code = 0x824F + value - 0x30
		result += "%04x" % code
	return result

static func entry(text: String, segments: Array) -> Dictionary:
	return {"text": text, "segments": segments, "context_status": "ready_for_review", "container": SOURCE_PATH}

## One word (no spaces, so re-wrap cannot split it) whose original-font width
## is over the vn_body_quoted box (190 px, closing quote cell reserved) but
## well under the same body without that cell (204 px).
func _close_cell_word(font_root: String) -> String:
	var font := Glyphs.new()
	if not font.open_external_font(font_root).is_empty():
		return ""
	var metrics := Flow.Metrics.new(font)
	var word := ""
	while metrics.width(fw(word + "m").hex_decode()) <= Flow.box("vn_body_quoted").width - 4:
		word += "m"
	while metrics.width(fw(word).hex_decode()) <= Flow.box("vn_body_quoted").width:
		word += "i"
	return word

func _pack(close_cell_word: String) -> Store:
	var q := ["t", "71", "separator"]
	var row := ["t", "20", "separator"]
	var entries := {
		rid(0): entry("Go left[[T001:choice_option]]Go right[[T002:choice_option]]Stay", [["g", fw("Go left")], q, ["g", fw("Go right")], q, ["g", fw("Stay")]]),
		rid(1): entry("Yes, gladly.[[T001:reply_variant]]No way.", [["g", fw("Yes, gladly.")], q, ["g", fw("No way.")]]),
		rid(2): entry("Hello there,[[T001:row]]how are you, [[T001:girl_name]]?", [["g", fw("Hello there,")], row, ["g", fw("how are you, ")], ["t", "47", "substitution"], ["g", fw("?")]]),
		rid(3): entry("overflow", [["g", fw("Supercalifragilisticexpialidociousness")]]),
		rid(4): entry("unknown", [["g", fw("Hi")], ["t", "3c", "unknown_renderer_bytes"]]),
		rid(5): entry("two options only", [["g", fw("A")], q, ["g", fw("B")]]),
		rid(6): entry("long option", [["g", fw("Go")], q, ["g", fw("Go Go Go Go Go Go Go Go Go Go Go Go Go Go Go Go Go")], q, ["g", fw("Go")]]),
		rid(7): entry("fits only without the closing quote cell", [["g", fw(close_cell_word)], row, ["g", fw(close_cell_word)], row, ["g", fw(close_cell_word)]]),
	}
	var pack := {"schema": Store.SCHEMA, "language": "en",
		"approval": {"approver": "synthetic", "date": "2026-10-30", "instruction": "test", "scope": "test", "source": "test"},
		"font": {"source_path": "SLPM_86.053", "source_sha256": "a".repeat(64), "glyph_bank_sha256": "b".repeat(64)},
		"containers": {SOURCE_PATH: {"sha256": SOURCE_SHA, "size": 4096}},
		"entries": entries}
	var folder := ProjectSettings.globalize_path("user://text-layer-dialogue")
	DirAccess.make_dir_recursive_absolute(folder)
	var path := folder.path_join("pack.json")
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(pack))
	file.close()
	var store := Store.new()
	check(store.open_pack(path, FileAccess.get_sha256(path)), "synthetic pack opens: " + store.error)
	return store

static func tokens(units: Array) -> Array:
	var result: Array = []
	for unit: Variant in units:
		if unit is String:
			result.append({"kind": "glyph", "raw_hex": unit})
		else:
			result.append(unit)
	return result

static func line_row(line: int, command: int, units: Array, voice: bool = false) -> Dictionary:
	var token_list := tokens(units)
	var raw := ""
	for token: Dictionary in token_list:
		raw += str(token.raw_hex)
	var result := {"id": rid(line), "line": line, "command": command, "command_name": "row",
		"source_locator": {"path": SOURCE_PATH, "source_sha256": SOURCE_SHA, "text_offset": 200 + line * 16, "command_offset": 100 + line},
		"text": {"raw_hex": raw, "tokens": token_list, "unknown_token_count": 0, "inline_label": null}}
	if voice:
		result["voice"] = {"speaker": "girl", "pairs": [{"variant": 0, "clip_start": 0, "clip_end": 0}, {"variant": 1, "clip_start": 0, "clip_end": 0}]}
	return result

func _player() -> TalkPlayer:
	var choice_sep := {"kind": "separator", "role": "choice_option", "code": "q", "raw_hex": "71"}
	var reply_sep := {"kind": "separator", "role": "reply_variant", "code": "q", "raw_hex": "71"}
	var rows := [
		line_row(0, 3, ["82a0", choice_sep, "82a2", choice_sep, "82a4"]),
		line_row(1, 2, ["82a0", "82a2", reply_sep, "82a4"], true),
		line_row(2, 6, ["82a0", "82a2", {"kind": "separator", "code": " ", "raw_hex": "20"}, "82a4", {"kind": "substitution", "code": "G", "raw_hex": "47"}]),
		{"id": rid(3), "line": 3, "command": 255, "command_name": "end",
			"source_locator": {"path": SOURCE_PATH, "source_sha256": SOURCE_SHA, "text_offset": null, "command_offset": 110}},
	]
	var content := {"schema": "tokimemo.dialogue-content.v1", "profile": "serifu-native-v1",
		"source": {"path": SOURCE_PATH, "sha256": SOURCE_SHA, "size_bytes": 4096, "reader_schema": "tokimemo.text.v1"},
		"profile_id": "serifu-serifu", "section_id": SECTION,
		"groups": [{"index": 0, "records": rows}],
		"voice_timing": "preserved_extents_runtime_mapping_unproved",
		"text_encoding": "original_bytes_with_reader_token_view",
		"semantic_coverage_complete": false, "content_sha256": ""}
	var player := TalkPlayer.new()
	content["content_sha256"] = player._component_digest(content)
	var route := {"source_path": SOURCE_PATH, "source_sha256": SOURCE_SHA, "profile_id": "serifu-serifu",
		"section_id": SECTION, "content_sha256": content["content_sha256"]}
	var loaded := player.load_content(content, route, {"allow_voiced": true})
	check(loaded.is_empty(), "synthetic dialogue loads: " + loaded)
	player.set_substitution_context({"girl_name": GIRL_NAME})
	return player

## Pixels that differ between two frames, and how many lie outside `rect`.
static func diff(a: Image, b: Image, rect: Rect2i) -> Vector2i:
	var changed := 0
	var outside := 0
	for y: int in a.get_height():
		for x: int in a.get_width():
			if a.get_pixel(x, y) != b.get_pixel(x, y):
				changed += 1
				if not rect.has_point(Vector2i(x, y)):
					outside += 1
	return Vector2i(changed, outside)

func _initialize() -> void:
	var font_root := OS.get_environment("FONT_ROOT")
	if font_root.is_empty():
		font_root = "/workspace/.asset-cache/port/fonts/native-font-v2"
	var close_cell_word := _close_cell_word(font_root)
	check(not close_cell_word.is_empty(), "original font opens for the close-cell probe")
	var layer := _pack(close_cell_word if not close_cell_word.is_empty() else "x")
	var player := _player()
	var box := Textbox.new()
	# The presenter's default HUD pin (DEFAULT_HUD_SHA) matches no manifest in
	# the current cache, so the regenerated HUD manifest is opened explicitly
	# first (HUD_MANIFEST/HUD_SHA256); open_font then reuses it.
	var hud_manifest := OS.get_environment("HUD_MANIFEST")
	if hud_manifest.is_empty():
		hud_manifest = "/workspace/.asset-cache/port/hud-production-v1/manifest.json"
	var hud_sha := OS.get_environment("HUD_SHA256")
	if hud_sha.is_empty():
		hud_sha = "c19f4306cd0b5947db81caa1fa2caaa9bf93916b14af836c63a06130511ab09d"
	check(box.open_hud_manifest(hud_manifest, hud_sha).is_empty(), "HUD actor resources open: " + box.error)
	check(box.open_font(font_root).is_empty(), "original font opens: " + box.error)
	if not failures.is_empty():
		_finish()
		return
	_talk_player_views(player)
	_choice_triplet(box, layer, player)
	_reply_variant(box, layer, player)
	_line_triplet(box, layer, player)
	_vn_triplet(font_root, layer)
	_vn_close_cell(font_root, layer, close_cell_word)
	_finish()

## NativeTalkPlayer passes the row id, the trusted source hash and name codes.
func _talk_player_views(player: TalkPlayer) -> void:
	var menu := player.present_choice(0)
	check(menu.record_id == rid(0) and menu.source_sha256 == SOURCE_SHA, "choice view carries id and source hash")
	check(menu.name_codes.get("G") == GIRL_NAME.hex_decode(), "choice view carries name codes")
	check(TalkPlayer.name_codes_for({}).is_empty(), "no context, no name codes")
	check(TalkPlayer.name_codes_for({"player_family": "82a0", "player_given": "82a2"}).get("F") == "82a082a2".hex_decode(), "full name joins family and given")

## Choice menu triplet; options map to the record's variants by source index.
func _choice_triplet(box: Textbox, layer: Store, player: TalkPlayer) -> void:
	var menu := player.present_choice(1)
	check(menu.order == [1, 2, 0], "frame 1 rotates the three options")
	var options := {"selected_index": 0}
	var native := box.render_frame(menu, options)
	check(box.last_text_source == "native", "unbound menu is native")
	box.set_translated_text(layer)
	var english := box.render_frame(menu, options)
	check(box.last_text_source == "english" and box.text_rejection.is_empty(), "menu draws English: " + box.text_rejection)
	check(box.last_text_box.width == Flow.box("textbox_choice").width and box.last_text_box.rows == 1, "menu fits the textbox_choice class: " + str(box.last_text_box))
	var changes := diff(native, english, Rect2i(44, 168, 249, 51))
	check(changes.x > 0 and changes.y == 0, "English options change only option rows: %s" % changes)
	# Display row 0 shows source option 1 ("Go right"): its glyphs differ from
	# a menu whose source order is identity.
	var unrotated := menu.duplicate(true)
	unrotated.options = [menu.options[2], menu.options[0], menu.options[1]]
	for index: int in 3:
		unrotated.options[index].display_position = index
	check(box.render_frame(unrotated, options).get_data() != english.get_data(), "options follow source_index, not display order")
	var partial := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var revealed := box.render_choice_menu(partial, menu, 0, 3)
	check(revealed.revealed_glyphs == 3 and revealed.total_glyphs == ("Go left" + "Go right" + "Stay").length(), "menu reveal counts English code units: %s" % str(revealed))
	for broken: Dictionary in [
			{"record_id": rid(5)}, {"record_id": rid(6)}, {"source_sha256": "0".repeat(64)},
			{"record_id": rid(99)}, {"record_id": "speaktst-layout:X:1"}]:
		var view := menu.duplicate(true)
		view.merge(broken, true)
		var frame := box.render_frame(view, options)
		check(frame.get_data() == native.get_data() and box.last_text_source == "native" and not box.text_rejection.is_empty(),
			"rejected menu keeps native frame: %s -> %s" % [str(broken), box.text_rejection])
	box.set_translated_text(null)
	check(box.render_frame(menu, options).get_data() == native.get_data(), "cleared menu is byte-identical to native")
	player.choose(menu.options[0].option_id, true, true)  # display 0 = source option 1
	player.advance_line(true)

## Reply variant (command 2): talk_speak_part draws the variant its talk_run
## caller passes (the live talk_choice global). The player's own choose() is
## not proof: the global is also written outside talk menus (girl select).
func _reply_variant(box: Textbox, layer: Store, player: TalkPlayer) -> void:
	# The menu before this reply chose source option 1 through choose().
	var unproved := player.present_line()
	check(unproved.get("error") == "" and unproved.record_id == rid(1), "reply row is current: " + str(unproved.get("error")))
	check(not unproved.has("variant_index"), "choose() alone gives no variant index")
	var view := player.present_line({"talk_choice": 1})
	check(view.get("variant_index") == 1, "variant index is the caller's talk_choice")
	check(player.present_line({"talk_choice": 0}).get("variant_index") == 0, "another caller talk_choice gives that variant")
	check(not player.present_line().has("variant_index"), "a caller talk_choice does not persist to the next call")
	for bad: Variant in [-1, "1", 1.0, null]:
		check(not player.present_line({"talk_choice": bad}).has("variant_index"), "talk_choice must be a non-negative int: " + str(bad))
	var native := box.render_frame(view)
	check(box.render_frame(unproved).get_data() == native.get_data(), "unbound reply is native")
	box.set_translated_text(layer)
	var chosen := box.render_frame(view)
	check(box.last_text_source == "english", "chosen reply draws English: " + box.text_rejection)
	var other := view.duplicate(true)
	other.variant_index = 0
	check(box.render_frame(other).get_data() != chosen.get_data(), "variant index picks the variant")
	var unknown := view.duplicate(true)
	unknown.erase("variant_index")
	check(box.render_frame(unknown).get_data() == native.get_data() and box.text_rejection.contains("variant index"), "reply without a variant index stays native")
	var out_of_range := view.duplicate(true)
	out_of_range.variant_index = 0xFF
	check(box.render_frame(out_of_range).get_data() == native.get_data() and box.text_rejection.contains("variant index"), "talk_choice 0xFF (talk_run(0xFF, ...)) stays native")
	# A fresh talk: a talk menu is answered, then a non-talk menu (girl
	# select, `talk_choice = girl_index`) rewrites the global before the reply.
	# The player never sees that write; without the caller's live value the
	# reply must stay Japanese rather than show the talk menu's variant.
	var fresh := _player()
	fresh.choose(fresh.present_choice(0).options[0].option_id, true, true)
	check(fresh.advance_line(true).is_empty(), "fresh player advances past its menu")
	var after_other_menu := fresh.present_line()
	check(not after_other_menu.has("variant_index"), "a reply after a non-talk choice has no variant index")
	var stale := box.render_frame(after_other_menu)
	check(stale.get_data() == native.get_data() and box.last_text_source == "native" and box.text_rejection.contains("variant index"),
		"a reply after a non-talk choice stays Japanese: " + box.text_rejection)
	var girl_select := fresh.present_line({"talk_choice": 1})
	check(girl_select.get("variant_index") == 1 and box.render_frame(girl_select).get_data() == chosen.get_data(),
		"the caller's live talk_choice selects the variant the original draws")
	box.set_translated_text(null)
	check(box.render_frame(view).get_data() == native.get_data(), "cleared reply is native")
	player.confirm_line(true, true)
	player.advance_line(true)

## Monologue line (command 6) with a girl-name token.
func _line_triplet(box: Textbox, layer: Store, player: TalkPlayer) -> void:
	var view := player.present_line()
	check(view.get("error") == "" and view.record_id == rid(2) and view.source_sha256 == SOURCE_SHA, "line view carries id and source hash")
	check(not view.has("variant_index"), "only reply commands carry a variant index")
	var native := box.render_frame(view)
	box.set_translated_text(layer)
	var english := box.render_frame(view)
	check(box.last_text_source == "english" and box.text_rejection.is_empty(), "line draws English: " + box.text_rejection)
	check(box.last_text_box.width == Flow.box("textbox_line").width and box.last_text_box.rows == 3, "unlabelled line fits the textbox_line class: " + str(box.last_text_box))
	var labelled := view.duplicate(true)
	labelled.speaker_id = "shiori"
	box.render_frame(labelled)
	check(box.last_text_box.width == Flow.box("textbox_line_label").width, "labelled line fits the textbox_line_label class: " + str(box.last_text_box))
	var changes := diff(native, english, Textbox.TEXTBOX_INNER)
	check(changes.x > 0 and changes.y == 0, "English changes only the window interior: %s" % changes)
	var target := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	var result := box.render_dialogue_line(target, view, 4)
	var expected_units := ("Hello there," + "how are you, " + "?").length() + GIRL_NAME.length() / 4
	check(result.revealed_glyphs == 4 and result.total_glyphs == expected_units and not result.text_complete, "one English code unit per tick: %s" % str(result))
	check(result.visible_row_count == 1, "partial reveal shows the first row only")
	var no_names := view.duplicate(true)
	no_names.name_codes = {}
	var rejected_cases := [
		[{"source_sha256": "0".repeat(64)}, "container hash"],
		[{"record_id": rid(99)}, "no approved record"],
		[{"record_id": rid(3)}, "overflow"],
		[{"record_id": rid(4)}, "unknown_renderer_bytes"],
		[{"record_id": ""}, "source-proved"],
		[{"name_codes": {}}, "name_unavailable"],
	]
	for case: Array in rejected_cases:
		var broken := view.duplicate(true)
		broken.merge(case[0], true)
		var frame := box.render_frame(broken)
		check(frame.get_data() == native.get_data() and str(box.text_rejection).contains(case[1]),
			"rejected line keeps native frame: %s -> %s" % [case[1], box.text_rejection])
	var empty := Store.new()
	box.set_translated_text(empty)
	check(box.render_frame(view).get_data() == native.get_data(), "an unopened layer keeps native")
	box.set_translated_text(null)
	check(box.render_frame(view).get_data() == native.get_data() and box.last_text_source == "native", "cleared line is byte-identical to native")

## VnScenePresenter.draw_vn_dialogue triplet with the original quote glyphs.
func _vn_triplet(font_root: String, layer: Store) -> void:
	var vn := Vn.new()
	check(vn.open_font_only(font_root), "VN font opens: " + vn.error)
	var body: Array[String] = ["817582a082a2", "82a48176"]
	var translation := {"record_id": rid(2), "source_sha256": SOURCE_SHA, "name_codes": {"G": GIRL_NAME.hex_decode()}}
	var native := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(native, "", body, false, -1, translation)
	check(vn.last_dialogue_source == "native", "unbound VN body is native")
	var plain := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(plain, "", body)
	check(plain.get_data() == native.get_data(), "translation argument alone changes nothing")
	check(vn.set_translated_text(layer) == 0, "no HUD manifest, no narration bound")
	vn.bounding_box_violations.clear()
	var english := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(english, "", body, false, -1, translation)
	check(vn.last_dialogue_source == "english" and vn.dialogue_text_rejection.is_empty(), "VN body draws English: " + vn.dialogue_text_rejection)
	check(vn.bounding_box_violations.is_empty(), "English glyphs stay in the message window: " + str(vn.bounding_box_violations))
	check(vn.last_dialogue_box.width == Flow.box("vn_body_quoted").width, "quoted VN body fits the vn_body_quoted class: " + str(vn.last_dialogue_box))
	var closing: Dictionary = vn.recorded_glyphs[-1] if not vn.recorded_glyphs.is_empty() else {}
	check(closing.get("code") == "8176" and Rect2i(27, 166, 266, 52).encloses(Rect2i(closing.pos, Vector2i(12, 12))),
		"the original closing quote is drawn inside the window: " + str(closing))
	var changes := diff(native, english, Rect2i(27, 166, 266, 52))
	check(changes.x > 0 and changes.y == 0, "VN English changes only the window: %s" % changes)
	check(english.get_pixelv(Vector2i(74, 170)) == native.get_pixelv(Vector2i(74, 170)), "original opening quote stays at x 74")
	var speaker := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(speaker, "93a18de8", body, true, -1, translation)
	check(vn.last_dialogue_source == "english", "speaker layout draws English from x 88")
	check(vn.last_dialogue_box.width == Flow.box("vn_body_speaker").width, "speaker VN body fits the vn_body_speaker class: " + str(vn.last_dialogue_box))
	for broken: Dictionary in [{"source_sha256": "0".repeat(64)}, {"record_id": rid(3)}, {"record_id": rid(4)}, {"record_id": rid(1)}, {"record_id": ""}]:
		var case := translation.duplicate(true)
		case.merge(broken, true)
		var frame := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		vn.draw_vn_dialogue(frame, "", body, false, -1, case)
		check(frame.get_data() == native.get_data() and vn.last_dialogue_source == "native" and not vn.dialogue_text_rejection.is_empty(),
			"rejected VN body keeps native: %s -> %s" % [str(broken), vn.dialogue_text_rejection])
	vn.set_translated_text(null)
	var cleared := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(cleared, "", body, false, -1, translation)
	check(cleared.get_data() == native.get_data(), "cleared VN body is byte-identical to native")

## A VN body whose rows fit only if the closing quote cell is not reserved.
## The presenter must reserve it: the record falls back to the original body
## and the closing quote stays inside Rect2i(27, 166, 266, 52).
func _vn_close_cell(font_root: String, layer: Store, word: String) -> void:
	var vn := Vn.new()
	check(vn.open_font_only(font_root), "VN font opens: " + vn.error)
	var probe := Glyphs.new()
	probe.open_external_font(font_root)
	var segments: Array = []
	for index: int in 3:
		if index > 0:
			segments.append({"kind": "token", "raw": PackedByteArray([0x20]), "token_kind": "separator"})
		segments.append({"kind": "glyphs", "codes": fw(word).hex_decode()})
	var quoted: int = Flow.box("vn_body_quoted").width
	var width := Flow.Metrics.new(probe).width(fw(word).hex_decode())
	check(width > quoted and width <= quoted + Flow.CLOSE_CELL - 4, "probe word is wider than the box only by part of the quote cell: %d" % width)
	var without_cell := Flow.classify(segments, Flow.runtime_box(Flow.VN_QUOTED_BODY_X, Flow.WINDOW_INNER_RIGHT, false, 3), probe, {})
	check(without_cell.outcome == "fits_markers", "control: the body fits when the quote cell is not reserved: " + str(without_cell.get("reason")))
	var body: Array[String] = ["817582a082a2", "82a48176"]
	var translation := {"record_id": rid(7), "source_sha256": SOURCE_SHA, "name_codes": {}}
	var native := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(native, "", body)
	vn.set_translated_text(layer)
	vn.bounding_box_violations.clear()
	vn.recorded_glyphs.clear()
	var frame := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	vn.draw_vn_dialogue(frame, "", body, false, -1, translation)
	check(vn.last_dialogue_box.width == quoted, "the closing quote cell is reserved: " + str(vn.last_dialogue_box))
	check(frame.get_data() == native.get_data() and vn.last_dialogue_source == "native" and vn.dialogue_text_rejection.contains("word_wider_than_box"),
		"a body that needs the quote cell keeps the original: " + vn.dialogue_text_rejection)
	check(vn.bounding_box_violations.is_empty(), "no glyph leaves the message window: " + str(vn.bounding_box_violations))
	var closing: Dictionary = vn.recorded_glyphs[-1] if not vn.recorded_glyphs.is_empty() else {}
	check(closing.get("code") == "8176" and Rect2i(27, 166, 266, 52).encloses(Rect2i(closing.pos, Vector2i(12, 12))),
		"the closing quote stays inside the window: " + str(closing))
	vn.set_translated_text(null)

func _finish() -> void:
	print("Text layer dialogue failures: ", failures)
	quit(0 if failures.is_empty() else 1)
