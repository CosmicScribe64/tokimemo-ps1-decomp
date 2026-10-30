extends SceneTree
## T-39201 (Phase 4 English WP-A): NativeTextFlow contract. The synthetic part
## needs no game data; the original-font part runs when FONT_ROOT (or the
## default /workspace/.asset-cache/port/fonts/native-font-v2) opens.
const Flow = preload("res://presentation/native_text_flow.gd")
const Layout = preload("res://presentation/native_latin_layout.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const Textbox = preload("res://presentation/dialogue_textbox_presenter.gd")
var failures: Array[String] = []

class StubFont extends RefCounted:
	## Every code is a 3-texel-wide glyph (advance 5) except the full-width
	## space (advance 4) and `missing`.
	var missing := PackedByteArray([0x82, 0x9a])
	func get_glyph(pair: PackedByteArray) -> Dictionary:
		if pair == missing:
			return {"status": "unsupported"}
		var pixels := PackedByteArray()
		pixels.resize(144)
		if pair != PackedByteArray([0x81, 0x40]):
			for y: int in 12:
				for x: int in [4, 5, 6]:
					pixels[y * 12 + x] = 1
		return {"status": "glyph", "pixels": pixels}

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

## Full-width code units for ASCII letters and spaces.
static func fw(text: String) -> PackedByteArray:
	var codes := PackedByteArray()
	for character: String in text:
		var value := character.unicode_at(0)
		var code := 0x8140
		if value >= 0x41 and value <= 0x5A:
			code = 0x8260 + value - 0x41
		elif value >= 0x61 and value <= 0x7A:
			code = 0x8281 + value - 0x61
		codes.append_array(PackedByteArray([code >> 8, code & 0xFF]))
	return codes

static func g(text: String) -> Dictionary:
	return {"kind": "glyphs", "codes": fw(text)}

static func t(raw: int, kind: String) -> Dictionary:
	return {"kind": "token", "raw": PackedByteArray([raw]), "token_kind": kind}

func _initialize() -> void:
	var font := StubFont.new()
	var row := t(0x20, "separator")
	# Marker rows: each translator row is kept and placed by NativeLatin.
	var marked := Flow.classify([g("ab"), row, g("cd")], {"width": 40, "rows": 3}, font, {})
	check(marked.outcome == "fits_markers" and marked.rows.size() == 2, "marker rows fit as given: " + str(marked))
	if marked.rows.size() == 2:
		check(marked.rows[0].codes == fw("ab") and marked.rows[0].x == Layout.offsets(font, fw("ab"), 40), "row offsets come from NativeLatin.offsets")
		check(marked.rows[0].x == PackedInt32Array([-4, 1]) and marked.rows[0].width == 10, "proportional offsets and width")
	check(Flow.layout([g("ab"), row, g("cd")], {"width": 40, "rows": 3}, font, {}).size() == 2, "layout returns the rows")
	# A terminal ' ' before the NUL closes the last row and adds none.
	var terminal := Flow.classify([g("ab"), t(0x20, "terminal_byte_before_nul")], {"width": 40, "rows": 1}, font, {})
	check(terminal.outcome == "fits_markers" and terminal.rows.size() == 1, "terminal row byte adds no row")
	check(Flow.classify([g("ab"), t(0x0A, "source_storage_control"), g("c")], {"width": 40, "rows": 2}, font, {}).rows.size() == 2, "line feed breaks the row")
	# Word re-wrap: "aaaa bbbb cccc" is 64 px; a 45 px box takes two words a row.
	var wrapped := Flow.classify([g("aaaa bbbb cccc")], {"width": 45, "rows": 3}, font, {})
	check(wrapped.outcome == "fits_rewrap" and wrapped.reason == "rewrap_kept_breaks" and wrapped.rows.size() == 2, "long row re-wraps at spaces: " + str(wrapped))
	if wrapped.rows.size() == 2:
		check(wrapped.rows[0].codes == fw("aaaa bbbb") and wrapped.rows[1].codes == fw("cccc"), "greedy wrap keeps word order")
	# Too many marker rows, but joining them fits.
	var joined := Flow.classify([g("a"), row, g("b"), row, g("c"), row, g("d")], {"width": 40, "rows": 2}, font, {})
	check(joined.outcome == "fits_rewrap" and joined.reason == "rewrap_joined_rows" and joined.rows.size() == 1, "short marker rows join: " + str(joined))
	# Overflow: empty layout and a reason.
	check(Flow.layout([g("aaaa bbbb cccc dddd")], {"width": 45, "rows": 1}, font, {}).is_empty(), "overflow returns empty")
	var over := Flow.classify([g("aaaa bbbb cccc dddd")], {"width": 45, "rows": 1}, font, {})
	check(over.outcome == "overflow" and over.reason == "rewrap_width:rows_exceed_max" and over.limit == "width" and over.needed_rows == 2,
		"a re-wrap that runs out of rows at this width is a width failure: " + str(over))
	var long_word := Flow.classify([g("abcdefghijk")], {"width": 45, "rows": 3}, font, {})
	check(long_word.outcome == "overflow" and long_word.reason == "rewrap_width:word_wider_than_box" and long_word.limit == "width", "a word wider than the box overflows on width")
	var scrolled := Flow.classify([g("aaaaaaaa"), row, g("bbbbbbbb"), row, g("cccccccc"), row, g("dddddddd")], {"width": 45, "rows": 3}, font, {})
	check(scrolled.outcome == "overflow" and scrolled.reason == "rewrap_rows:marker_rows_exceed_max" and scrolled.limit == "rows",
		"more marker rows than the box are named: " + str(scrolled))
	# Four 2-letter marker rows join into three rows at 30 px (two words a row)
	# ("aa bb" is 24 px) but not at 20 px: the same record overflows in the
	# narrower box only, and the reason names the marker-row count.
	var four_rows := [g("aa"), row, g("bb"), row, g("cc"), row, g("dd")]
	check(Flow.classify(four_rows, {"width": 30, "rows": 3}, font, {}).outcome == "fits_rewrap", "four short rows join at 30 px")
	var narrow := Flow.classify(four_rows, {"width": 20, "rows": 3}, font, {})
	check(narrow.outcome == "overflow" and narrow.reason == "rewrap_rows:marker_rows_exceed_max" and narrow.needed_rows == 4, "narrower box overflows: " + str(narrow))
	# Names expand before measuring: G with 4 W is 20 px.
	var four_w: PackedByteArray = Flow.worst_case_names()["G"]
	check(four_w == PackedByteArray([0x82, 0x76, 0x82, 0x76, 0x82, 0x76, 0x82, 0x76]), "worst case name is 4 full-width W")
	check(Flow.worst_case_names()["F"].size() == 16, "full name worst case is two fields")
	var named := Flow.classify([g("Hi "), t(0x47, "substitution"), g("x")], {"width": 60, "rows": 1}, font, Flow.worst_case_names())
	check(named.outcome == "fits_markers" and named.rows[0].codes == fw("Hi ") + four_w + fw("x"), "girl name expands inline")
	check(named.rows[0].width == 5 + 5 + 4 + 20 + 5, "expanded name is measured")
	check(Flow.classify([g("Hi "), t(0x47, "substitution"), g("x")], {"width": 30, "rows": 1}, font, Flow.worst_case_names()).outcome == "overflow", "4xW name can overflow a box the short text fits")
	var player := Flow.classify([t(0x50, "substitution"), g("a")], {"width": 60, "rows": 1}, font, {"P": fw("Ab")})
	check(player.outcome == "fits_markers" and player.rows[0].codes == fw("Aba"), "player form P expands")
	check(Flow.classify([t(0x50, "substitution"), g("a")], {"width": 60, "rows": 1}, font, {}).reason == "name_unavailable:P", "missing name rejects")
	check(Flow.classify([t(0x50, "substitution"), g("a")], {"width": 60, "rows": 1}, font, {"P": PackedByteArray([0x41, 0x42])}).reason == "name_not_full_width:P", "half-width name bytes reject")
	# Tokens the flow cannot draw keep the original.
	check(Flow.classify([g("a"), t(0x3C, "unknown_renderer_bytes")], {"width": 60, "rows": 1}, font, {}).reason == "token:unknown_renderer_bytes", "unknown renderer bytes reject")
	check(Flow.classify([t(0x3C, "inline_label"), g("a")], {"width": 60, "rows": 1}, font, {}).outcome == "rejected_token", "inline label rejects")
	check(Flow.classify([g("a"), {"kind": "token", "raw": PackedByteArray([0x25, 0x64]), "token_kind": "format_directive"}], {"width": 60, "rows": 1}, font, {}).reason == "token:format_directive", "format directive rejects")
	check(Flow.classify([g("a"), t(0x71, "separator"), g("b")], {"width": 60, "rows": 1}, font, {}).reason == "variant_separator_unsplit", "unsplit q rejects")
	check(Flow.layout([g("a"), t(0x3C, "unknown_renderer_bytes")], {"width": 60, "rows": 1}, font, {}).is_empty(), "rejected layout is empty")
	var variants := Flow.split_variants([g("a"), t(0x71, "separator"), g("b"), row, g("c"), t(0x71, "separator"), g("d")])
	check(variants.size() == 3 and variants[1].size() == 3, "split_variants splits at q")
	check(Flow.classify(variants[1], {"width": 60, "rows": 2}, font, {}).rows.size() == 2, "a variant lays out alone")
	check(Flow.classify([g("a"), row], {"width": 60, "rows": 1}, StubFont.new(), {}).outcome == "fits_markers", "trailing row token is not a row")
	# Missing glyph, bad boxes, malformed input.
	check(Flow.classify([{"kind": "glyphs", "codes": PackedByteArray([0x82, 0x9a])}], {"width": 60, "rows": 1}, font, {}).reason == "glyph_unavailable", "missing glyph rejects")
	check(Flow.classify([g("a")], "no_such_box", font, {}).reason == "invalid_box_or_font", "unknown box class")
	check(Flow.classify([g("a")], {"width": 0, "rows": 1}, font, {}).reason == "invalid_box_or_font", "zero-width box")
	check(Flow.classify([], "dialogue_4x3", font, {}).reason == "no_segments", "empty record")
	check(Flow.classify([row], "dialogue_4x3", font, {}).reason == "no_glyph_text", "markers only")
	check(Flow.classify([g("a")], "dialogue_4x3", null, {}).reason == "invalid_box_or_font", "no font")
	for name: String in Flow.BOX_CLASSES:
		check(not Flow.box(name).is_empty() and str(Flow.box(name).get("evidence", "")).length() > 0 \
			and str(Flow.box(name).get("source", "")) in ["port_runtime", "c_reference", "design", "provisional"], "box class has evidence and source: " + name)
	check(Flow.box("dialogue_4x3").width + 32 == 292 and Flow.box("dialogue_4x3_speaker").width + 88 == 292, "4:3 C reference boxes end at the inner window edge")
	# The runtime classes are the presenters' runtime_box formula.
	check(Flow.WINDOW_INNER_RIGHT == Textbox.TEXTBOX_INNER.end.x and Flow.CLOSE_CELL == Textbox.GLYPH_ADVANCE_X, "runtime geometry matches the textbox presenter")
	var runtime := {
		"textbox_line": Flow.runtime_box(Flow.TEXTBOX_BODY_X, Flow.WINDOW_INNER_RIGHT, true, 3),
		"textbox_line_label": Flow.runtime_box(Flow.TEXTBOX_LABEL_BODY_X, Flow.WINDOW_INNER_RIGHT, true, 3),
		"textbox_choice": Flow.runtime_box(Flow.CHOICE_BODY_X, Flow.CHOICE_RIGHT, true, 1),
		"vn_body_speaker": Flow.runtime_box(Flow.VN_SPEAKER_BODY_X, Flow.WINDOW_INNER_RIGHT, true, 3),
		"vn_body_quoted": Flow.runtime_box(Flow.VN_QUOTED_BODY_X, Flow.WINDOW_INNER_RIGHT, true, 3),
	}
	for name: String in runtime:
		check(Flow.box(name).source == "port_runtime" and Flow.box(name).width == runtime[name].width and Flow.box(name).rows == runtime[name].rows,
			"runtime class equals runtime_box: " + name)
	check([Flow.box("textbox_line").width, Flow.box("textbox_line_label").width, Flow.box("textbox_choice").width,
		Flow.box("vn_body_speaker").width, Flow.box("vn_body_quoted").width] == [224, 192, 200, 176, 190], "runtime widths")
	_original_font()
	print("Native text flow failures: ", failures)
	quit(0 if failures.is_empty() else 1)

## With the original font: 'W' advances, a 4xW name and an English row in the
## runtime labelled textbox box, all through NativeLatin.
func _original_font() -> void:
	var root := OS.get_environment("FONT_ROOT")
	if root.is_empty():
		root = "/workspace/.asset-cache/port/fonts/native-font-v2"
	var font := Glyphs.new()
	var opened: String = font.open_external_font(root)
	if not opened.is_empty():
		print("SKIP original font checks: ", opened)
		return
	var four_w: PackedByteArray = Flow.worst_case_names()["G"]
	var offsets := Layout.offsets(font, four_w, 400)
	check(offsets.size() == 4, "original font has full-width W")
	var metrics := Flow.Metrics.new(font)
	var w_advance := metrics.advance(0x8276)
	check(w_advance > 4 and w_advance <= 14, "W advance within one original cell: %d" % w_advance)
	check(metrics.width(four_w) == 4 * w_advance, "4xW width is four W advances")
	var english := Flow.classify([g("Good morning, "), t(0x47, "substitution"), g(" how are you")], "textbox_line_label", font, Flow.worst_case_names())
	check(english.outcome in ["fits_markers", "fits_rewrap"], "a short line with a 4xW name fits the labelled textbox box: " + str(english.outcome))
	for placed: Dictionary in english.rows:
		check(placed.width <= Flow.box("textbox_line_label").width, "row stays inside the box")
		check(placed.x == Layout.offsets(font, placed.codes, Flow.box("textbox_line_label").width), "original-font offsets equal NativeLatin")
