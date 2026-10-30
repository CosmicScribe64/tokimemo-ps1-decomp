extends RefCounted
## T-39201 (Phase 4 English WP-A): shared multi-row flow for translated text
## drawn with the original full-width glyphs.
##
## Input is a text-layer record's `segments` (services/text_layer_store.gd:
## glyph runs and protected source tokens). Output is rows of full-width code
## units with x offsets from NativeLatin.offsets (native_latin_layout.gd, not
## modified here). The fit policy is the Phase 4 default
## (wiki/phase4-design-2026-10-29.md, English, "Fit policy"):
##
## 1. markers: the translator's row tokens (' ' separators, row_separator,
##    line_feed) give the rows; every row fits and there are at most `rows`;
## 2. rewrap: words (split at the full-width space 0x8140) are re-flowed,
##    first keeping the marker breaks, then joining all rows;
## 3. otherwise the record overflows and the caller keeps the original.
##
## Name tokens ('P', 'G', ... from NativeTextTokens.NAME_CODES) expand from
## caller-supplied full-width code units before measuring. Any other token
## (unknown_renderer_bytes, inline_label, format directives, conditional
## markers) rejects the record: the glyph run cannot reproduce what the
## original renderer does with those bytes. A record with choice/reply
## separators ('q') must be split with split_variants() first.
##
## The corpus fit runner (tests/translation/run_english_corpus_fit.gd) calls
## prepare() and fit() exactly as layout() does, so the fit metric exists once.

const NativeLatin = preload("res://presentation/native_latin_layout.gd")
const Tokens = preload("res://dialogue/text_tokens.gd")

const SPACE := 0x8140
const ROW_BYTE := 0x20
const LINE_FEED_BYTE := 0x0A
const VARIANT_BYTE := 0x71
const OUTCOMES := ["fits_markers", "fits_rewrap", "overflow", "rejected_token"]

## Port runtime geometry: where DialogueTextboxPresenter and
## VnScenePresenter start an English body and where it must end. The
## presenters size their boxes with runtime_box() from their own origins; the
## port_runtime classes below are the same formula over these constants, and
## run_native_text_flow_tests.gd checks that both agree. These are the port's
## native drawing origins. They differ from the C talk_render origins
## (x 32 / x 88) that the c_reference classes record.
## DialogueTextboxPresenter.TEXTBOX_INNER.end.x (28 + 264).
const WINDOW_INNER_RIGHT := 292
## The closing bracket/quote cell drawn after the body (GLYPH_ADVANCE_X).
const CLOSE_CELL := 14
## Textbox body: label origin x 40 plus the opening bracket cell.
const TEXTBOX_BODY_X := 40 + 14
## Textbox body after a two-glyph label (shiori/yoshio: 2 x 14 + 4 gap).
const TEXTBOX_LABEL_BODY_X := 40 + 2 * 14 + 4 + 14
## Textbox choice row: bracket at x 46, body from x 60 to the highlight end
## (Rect2i(44, y, 230, 14) ends at x 274).
const CHOICE_BODY_X := 46 + 14
const CHOICE_RIGHT := 44 + 230
## draw_vn_dialogue body after a speaker label and its opening quote
## (88 + 14), and without a speaker after an opening quote at x 74 (74 + 14).
const VN_SPEAKER_BODY_X := 88 + 14
const VN_QUOTED_BODY_X := 74 + 14

## Box classes as data. Widths are the pixels a run may use from its origin;
## NativeLatin keeps the last glyph's shadow inside that width. Screen
## coordinates are native 320x240 (x = 160 + the C x argument). "source"
## says whether a class is a box the port draws ("port_runtime"), a C
## call-site reference ("c_reference"), a design box or a provisional one.
const BOX_CLASSES := {
	# Runtime boxes. The English hooks fit against these, and the corpus
	# headline screens measure them.
	"textbox_line": {"width": WINDOW_INNER_RIGHT - TEXTBOX_BODY_X - CLOSE_CELL, "rows": 3, "pitch": 16, "aspect": "4:3",
		"source": "port_runtime",
		"evidence": "presentation/dialogue_textbox_presenter.gd render_dialogue_line: body x 54 (40 + bracket) to TEXTBOX_INNER x 292, less the closing bracket cell"},
	"textbox_line_label": {"width": WINDOW_INNER_RIGHT - TEXTBOX_LABEL_BODY_X - CLOSE_CELL, "rows": 3, "pitch": 16, "aspect": "4:3",
		"source": "port_runtime",
		"evidence": "presentation/dialogue_textbox_presenter.gd render_dialogue_line: shiori/yoshio label, body x 86 to x 292, less the closing bracket cell"},
	"textbox_choice": {"width": CHOICE_RIGHT - CHOICE_BODY_X - CLOSE_CELL, "rows": 1, "pitch": 16, "aspect": "4:3",
		"source": "port_runtime",
		"evidence": "presentation/dialogue_textbox_presenter.gd render_choice_menu: body x 60 to the highlight end x 274, less the closing bracket cell"},
	"vn_body_speaker": {"width": WINDOW_INNER_RIGHT - VN_SPEAKER_BODY_X - CLOSE_CELL, "rows": 3, "pitch": 16, "aspect": "4:3",
		"source": "port_runtime",
		"evidence": "presentation/vn_scene_presenter.gd draw_vn_dialogue: speaker body x 88 + opening quote to x 292, less the closing quote cell"},
	"vn_body_quoted": {"width": WINDOW_INNER_RIGHT - VN_QUOTED_BODY_X - CLOSE_CELL, "rows": 3, "pitch": 16, "aspect": "4:3",
		"source": "port_runtime",
		"evidence": "presentation/vn_scene_presenter.gd draw_vn_dialogue: no speaker, opening quote at x 74, body x 88 to x 292, less the closing quote cell"},
	# C call-site references (not drawn by any port presenter today).
	# talk_render(-0x80, ...) narration and effects in talk_speak_part
	# (src/main/scene_library.c): body from x 32 to the message window's inner
	# right edge x 292 (DialogueTextboxPresenter.TEXTBOX_INNER).
	"dialogue_4x3": {"width": 260, "rows": 3, "pitch": 16, "aspect": "4:3", "source": "c_reference",
		"evidence": "src/main/scene_library.c talk_speak_part talk_render(-0x80); TEXTBOX_INNER right edge 292"},
	# talk_render(-0x48, ...) after the speaker label and its bracket
	# (set_kanji_string(-0x56, ...)): body from x 88 to x 292.
	"dialogue_4x3_speaker": {"width": 204, "rows": 3, "pitch": 16, "aspect": "4:3", "source": "c_reference",
		"evidence": "src/main/scene_library.c talk_speak_part talk_render(-0x48) after the label bracket at -0x56"},
	# Opt-in widescreen box (T-30001 Class B): at most 380 px of body.
	"dialogue_wide": {"width": 380, "rows": 3, "pitch": 16, "aspect": "wide", "source": "design",
		"evidence": "wiki/phase4-design-2026-10-29.md fit policy: second box class up to 380 px, widescreen only"},
	# The wide box keeps the 56 px speaker column (x 32..88) of the 4:3 layout.
	"dialogue_wide_speaker": {"width": 324, "rows": 3, "pitch": 16, "aspect": "wide", "source": "design",
		"evidence": "dialogue_wide minus the talk_render -0x80..-0x48 speaker column"},
	# talk_render(-0x72, ..., close, open, 1) for choice commands 3/8/0x13: label
	# at x 46, body from x 60; menu_set items x -0x80 w 0xFC put the cursor
	# at item.x + w - 4 = x 280; the closing bracket cell (14) precedes it.
	"choice_cell": {"width": 206, "rows": 1, "pitch": 16, "aspect": "4:3", "source": "c_reference",
		"evidence": "src/main/scene_library.c talk_speak_part case 0x3/0x8/0x13; normal_date_three_select_init menu_set"},
	# Command HUD narration: the 14 original 14-px cells
	# (presentation/command_hud_presenter.gd set_translated_text).
	"hud_14_cell": {"width": 196, "rows": 1, "pitch": 16, "aspect": "4:3", "source": "port_runtime",
		"evidence": "presentation/command_hud_presenter.gd original 14-cell narration row"},
	# Telephone top banner: the port's gradient box Rect2i(28, 23, 264, 42)
	# with a 4 px inset each side and two 16 px rows. Port-derived, not yet
	# tied to a C call site.
	"phone_banner": {"width": 256, "rows": 2, "pitch": 16, "aspect": "4:3", "source": "provisional",
		"evidence": "presentation/telephone_overlay_presenter.gd gradient box Rect2i(28, 23, 264, 42); provisional"},
}

## Glyph metrics through NativeLatin._metrics, cached per code unit. The cache
## only speeds up the word search; the final rows always come from
## NativeLatin.offsets.
class Metrics extends RefCounted:
	var font: RefCounted
	var _cache: Dictionary = {}

	func _init(source_font: RefCounted) -> void:
		font = source_font

	## Advance of one code unit, or -1 when the glyph is unavailable.
	func advance(code: int) -> int:
		if not _cache.has(code):
			var metric: Vector2i = NativeLatin._metrics(font, PackedByteArray([code >> 8, code & 0xFF]))
			_cache[code] = -1 if metric.x < 0 else metric.y
		return _cache[code]

	## Width of a code-unit run, or -1 when a glyph is unavailable.
	func width(codes: PackedByteArray) -> int:
		var total := 0
		for index: int in range(0, codes.size(), 2):
			var step := advance(codes[index] << 8 | codes[index + 1])
			if step < 0:
				return -1
			total += step
		return total

## The runtime box formula: a body from `body_x` to `right_x`, less the
## closing bracket cell when the presenter draws one after the body.
static func runtime_box(body_x: int, right_x: int, reserve_close_cell: bool, rows: int) -> Dictionary:
	return {"width": right_x - body_x - (CLOSE_CELL if reserve_close_cell else 0), "rows": rows, "pitch": 16}

## Resolves a box class name, or checks a caller box {width, rows}.
static func box(box_class: Variant) -> Dictionary:
	if box_class is String:
		return BOX_CLASSES.get(box_class, {})
	if box_class is Dictionary and box_class.get("width") is int and box_class.get("rows") is int \
			and int(box_class.width) > 0 and int(box_class.rows) > 0:
		return box_class
	return {}

## Splits segments at 'q' separator tokens (choice options, reply variants).
## Returns one segment array per variant; a record without 'q' gives one.
static func split_variants(segments: Array) -> Array:
	var variants: Array = [[]]
	for segment: Variant in segments:
		if segment is Dictionary and segment.get("kind") == "token" and segment.get("raw") == PackedByteArray([VARIANT_BYTE]):
			variants.append([])
		else:
			variants[-1].append(segment)
	return variants

## Worst-case name buffers: 4 full-width 'W' (0x8276) per 8-byte name field,
## 8 for the two-field full name ('F').
static func worst_case_names() -> Dictionary:
	var four := PackedByteArray()
	for _index: int in 4:
		four.append_array(PackedByteArray([0x82, 0x76]))
	var result := {}
	for code: String in Tokens.NAME_CODES:
		result[code] = four + four if code == "F" else four
	return result

## Expands tokens into rows of code units. Returns {"rows": Array of
## PackedByteArray} or {"outcome": "rejected_token", "reason": ...}.
static func prepare(segments: Array, name_codes: Dictionary) -> Dictionary:
	if segments.is_empty():
		return _rejected("no_segments")
	var rows: Array = [PackedByteArray()]
	for segment: Variant in segments:
		if not segment is Dictionary:
			return _rejected("malformed_segment")
		if segment.get("kind") == "glyphs":
			var codes: PackedByteArray = segment.get("codes", PackedByteArray())
			if codes.is_empty() or codes.size() % 2 != 0:
				return _rejected("malformed_glyph_run")
			rows[-1].append_array(codes)
			continue
		if segment.get("kind") != "token":
			return _rejected("malformed_segment")
		var raw: PackedByteArray = segment.get("raw", PackedByteArray())
		var kind := str(segment.get("token_kind", ""))
		if raw.size() != 1:
			return _rejected("token:" + (kind if not kind.is_empty() else "multi_byte"))
		var byte := raw[0]
		if byte == VARIANT_BYTE:
			return _rejected("variant_separator_unsplit")
		if (byte == ROW_BYTE or byte == LINE_FEED_BYTE) and kind in ["separator", "row_separator", "source_storage_control", "terminal_byte_before_nul"]:
			rows.append(PackedByteArray())
			continue
		var letter := char(byte)
		if kind == "substitution" and Tokens.NAME_CODES.has(letter):
			var value: Variant = name_codes.get(letter)
			if not value is PackedByteArray or value.is_empty() or value.size() % 2 != 0:
				return _rejected("name_unavailable:" + letter)
			for index: int in range(0, value.size(), 2):
				if value[index] < 0x81:
					return _rejected("name_not_full_width:" + letter)
			rows[-1].append_array(value)
			continue
		return _rejected("token:" + (kind if not kind.is_empty() else "unknown"))
	# A terminal ' ' before the NUL closes the last row; it adds no row.
	while rows.size() > 1 and rows[-1].is_empty():
		rows.pop_back()
	var glyphs := 0
	for row: PackedByteArray in rows:
		glyphs += row.size()
	if glyphs == 0:
		return _rejected("no_glyph_text")
	return {"rows": rows}

## Fits prepared rows into a box. Returns {"outcome", "reason", "rows":
## [{"codes", "x", "width"}], "slack", "marker_rows", "needed_rows"}.
static func fit(prepared: Dictionary, box_class: Variant, metrics: Metrics) -> Dictionary:
	if prepared.has("outcome"):
		return prepared
	var spec := box(box_class)
	if spec.is_empty() or metrics == null or metrics.font == null:
		return {"outcome": "overflow", "reason": "invalid_box_or_font", "rows": []}
	var max_width: int = spec.width
	var max_rows: int = spec.rows
	var marker_rows: Array = prepared.rows
	var widest := 0
	var marker_fit := marker_rows.size() <= max_rows
	for row: PackedByteArray in marker_rows:
		var width := metrics.width(row)
		if width < 0:
			return _rejected("glyph_unavailable")
		widest = maxi(widest, width)
		if width > max_width:
			marker_fit = false
	if marker_fit:
		return _finish("fits_markers", "markers", marker_rows, spec, metrics, marker_rows.size())
	var kept := _wrap_rows(marker_rows, max_width, metrics)
	if kept.has("rows") and kept.rows.size() <= max_rows:
		return _finish("fits_rewrap", "rewrap_kept_breaks", kept.rows, spec, metrics, marker_rows.size())
	var joined_source := PackedByteArray()
	for row: PackedByteArray in marker_rows:
		if not joined_source.is_empty() and not row.is_empty():
			joined_source.append_array(PackedByteArray([SPACE >> 8, SPACE & 0xFF]))
		joined_source.append_array(row)
	var joined := _wrap_rows([joined_source], max_width, metrics)
	if joined.has("rows") and joined.rows.size() <= max_rows:
		return _finish("fits_rewrap", "rewrap_joined_rows", joined.rows, spec, metrics, marker_rows.size())
	# The reason names the limit the last re-wrap hit at this width:
	# "rewrap_width:word_wider_than_box" when one word is wider than the box;
	# "rewrap_width:rows_exceed_max" when the translator's rows fit the row
	# count but wrapping them at this width needs more rows;
	# "rewrap_rows:marker_rows_exceed_max" when the translator wrote more rows
	# than the box holds and joining them at this width does not bring the
	# count down. Whether a record would fit a wider box is a comparison
	# between boxes; the corpus runner reports it per screen.
	var reason := "rewrap_width:rows_exceed_max"
	var limit := "width"
	if joined.has("reason"):
		reason = "rewrap_width:" + str(joined.reason)
	elif marker_rows.size() > max_rows:
		reason = "rewrap_rows:marker_rows_exceed_max"
		limit = "rows"
	return {"outcome": "overflow", "reason": reason, "limit": limit, "rows": [], "marker_rows": marker_rows.size(),
		"needed_rows": joined.rows.size() if joined.has("rows") else -1, "widest_marker_row": widest,
		"slack": max_width - widest}

## Runtime entry point: rows of {"codes", "x", "width"} relative to the box
## origin (row r is drawn at origin.y + r * pitch), or [] when the record does
## not fit or carries a token the flow cannot draw.
static func layout(segments: Array, box_class: Variant, font: RefCounted, name_codes: Dictionary) -> Array:
	var result := classify(segments, box_class, font, name_codes)
	return result.rows if result.outcome in ["fits_markers", "fits_rewrap"] else []

## layout() with the outcome and its reason.
static func classify(segments: Array, box_class: Variant, font: RefCounted, name_codes: Dictionary) -> Dictionary:
	return fit(prepare(segments, name_codes), box_class, Metrics.new(font))

static func _wrap_rows(rows: Array, max_width: int, metrics: Metrics) -> Dictionary:
	var space_width := metrics.advance(SPACE)
	var result: Array = []
	for row: PackedByteArray in rows:
		var line := PackedByteArray()
		var line_width := 0
		for word: PackedByteArray in _words(row):
			var word_width := metrics.width(word)
			if word_width < 0:
				return {"reason": "glyph_unavailable"}
			if word_width > max_width:
				return {"reason": "word_wider_than_box"}
			if line.is_empty():
				line = word
				line_width = word_width
			elif line_width + space_width + word_width <= max_width:
				line.append_array(PackedByteArray([SPACE >> 8, SPACE & 0xFF]))
				line.append_array(word)
				line_width += space_width + word_width
			else:
				result.append(line)
				line = word
				line_width = word_width
		result.append(line)
	return {"rows": result}

static func _words(row: PackedByteArray) -> Array:
	var words: Array = []
	var current := PackedByteArray()
	for index: int in range(0, row.size(), 2):
		if (row[index] << 8 | row[index + 1]) == SPACE:
			if not current.is_empty():
				words.append(current)
			current = PackedByteArray()
		else:
			current.append_array(row.slice(index, index + 2))
	if not current.is_empty():
		words.append(current)
	return words

static func _finish(outcome: String, reason: String, rows: Array, spec: Dictionary, metrics: Metrics, marker_rows: int) -> Dictionary:
	var placed: Array = []
	var widest := 0
	for row: PackedByteArray in rows:
		var offsets := PackedInt32Array()
		var width := 0
		if not row.is_empty():
			offsets = NativeLatin.offsets(metrics.font, row, spec.width)
			if offsets.is_empty():
				return {"outcome": "overflow", "reason": "native_latin_rejected", "rows": [], "marker_rows": marker_rows}
			width = metrics.width(row)
		widest = maxi(widest, width)
		placed.append({"codes": row, "x": offsets, "width": width})
	return {"outcome": outcome, "reason": reason, "rows": placed, "slack": int(spec.width) - widest,
		"marker_rows": marker_rows, "needed_rows": rows.size()}

static func _rejected(reason: String) -> Dictionary:
	return {"outcome": "rejected_token", "reason": reason, "rows": []}
