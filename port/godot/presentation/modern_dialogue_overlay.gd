extends Control
## T-30001 WP4: opt-in window-space English text box over the native message
## window. It draws in the displayed layer only, above the TextureRect that
## shows the native Image, and never reads or writes that Image, so the
## fidelity Image is unchanged by construction.
##
## It shows the approved translation for the active source line and nothing
## otherwise (the native text then stays visible). It is visible only when
## accessibility.text_box == "modern". Text comes from the existing stores:
##   {"payload_sha": String, "line": int}  -> text_translation_store.replacement_any
##   {"record_id": String, "container_sha256": String}
##                                          -> text_layer_store.lookup_record_text
## Both lookups keep approved lines the native glyph set cannot draw, since
## this box uses a window font; those lines are the reason it exists.
## The box sits on NativeFrameLayout.text_band (native rows 160..239, its top
## edge raised by the margin to cover the native window border), widened to
## text_band_wide in wide mode. Text wraps at the preferred size (which
## follows accessibility.font_scale); a long line first grows the box upward
## to the frame top, and only then does the font shrink, down to a floor that
## also follows font_scale. Only a line that overflows the whole frame at the
## floor goes below it (last_fit.below_floor).

const Layout = preload("res://presentation/native_frame_layout.gd")
const TEXT_BOXES := ["native", "modern"]
const MIN_FONT_SIZE := 10
## Preferred text height as a share of the text band height, before font_scale.
const BAND_FONT_SHARE := 0.16
## Smallest size the box shrinks to before the last resort, as a share of the
## text band height, before font_scale.
const FLOOR_FONT_SHARE := 0.1

var translation_store: RefCounted
var text_layer: RefCounted
var text_layer_font: RefCounted
## {"speaker", "text", "id"} for the active line, empty when none resolves.
var active_record: Dictionary = {}
## The key passed to the last show_line, empty after clear_line.
var active_key: Dictionary = {}
## Why the last show_line found nothing ("" after a hit).
var last_miss := ""
## {"font_size", "text_height", "box", "grew", "fits"} from the last layout.
var last_fit: Dictionary = {}
var _layout: Dictionary = {}
var _prefs: Dictionary = {}
var _panel: Panel
var _speaker: Label
var _body: Label

func _init() -> void:
	mouse_filter = Control.MOUSE_FILTER_IGNORE
	visible = false
	_panel = Panel.new()
	_panel.mouse_filter = Control.MOUSE_FILTER_IGNORE
	var style := StyleBoxFlat.new()
	style.bg_color = Color(0.05, 0.06, 0.14, 1.0)
	style.border_color = Color(0.85, 0.85, 0.95, 1.0)
	style.set_border_width_all(2)
	# Square corners: the box covers the whole text band, so rounded corners
	# would let native checker pixels show at the band's corners.
	style.set_corner_radius_all(0)
	_panel.add_theme_stylebox_override("panel", style)
	add_child(_panel)
	_speaker = _label()
	_speaker.add_theme_color_override("font_color", Color(1.0, 0.86, 0.55))
	_speaker.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	_body = _label()
	_body.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART

func _label() -> Label:
	var label := Label.new()
	label.mouse_filter = Control.MOUSE_FILTER_IGNORE
	label.clip_text = false
	label.add_theme_color_override("font_color", Color.WHITE)
	_panel.add_child(label)
	return label

static func text_box_preference(prefs: Dictionary) -> String:
	var value: Variant = prefs.get("text_box")
	var accessibility: Variant = prefs.get("accessibility")
	if accessibility is Dictionary and accessibility.has("text_box"):
		value = accessibility.text_box
	return value if value is String and value in TEXT_BOXES else "native"

static func font_scale_preference(prefs: Dictionary) -> float:
	var value: Variant = prefs.get("font_scale")
	var accessibility: Variant = prefs.get("accessibility")
	if accessibility is Dictionary and accessibility.has("font_scale"):
		value = accessibility.font_scale
	return clampf(float(value), 1.0, 1.5) if (value is float or value is int) and is_finite(float(value)) else 1.0

func set_stores(translations: RefCounted, layer: RefCounted = null, layer_font: RefCounted = null) -> void:
	translation_store = translations
	text_layer = layer
	text_layer_font = layer_font

## Resolves and shows the line; returns true when an approved record exists.
func show_line(key: Dictionary) -> bool:
	active_key = key.duplicate(true)
	active_record = _resolve(key)
	_refresh()
	return not active_record.is_empty()

## Resolves the active key again against the current stores. Callers use it
## when the language bundle or text pack changes in the middle of a line.
func refresh_line() -> bool:
	return show_line(active_key) if not active_key.is_empty() else false

func clear_line() -> void:
	active_key = {}
	active_record = {}
	last_miss = "cleared"
	_refresh()

func apply_layout(layout: Dictionary, prefs: Dictionary) -> void:
	_layout = layout
	_prefs = prefs
	_refresh()

func is_shown() -> bool:
	return visible

func _resolve(key: Dictionary) -> Dictionary:
	last_miss = ""
	if key.get("payload_sha") is String and (key.get("line") is int):
		if translation_store == null or not (translation_store.has_method("replacement_any") or translation_store.has_method("replacement")):
			last_miss = "no translation store"
			return {}
		# Prefer the lookup that keeps lines the native glyph set cannot draw:
		# this box uses a window font, so those lines are exactly its purpose.
		var row: Dictionary = translation_store.replacement_any(key.payload_sha, key.line) if translation_store.has_method("replacement_any") \
			else translation_store.replacement(key.payload_sha, key.line)
		if row.is_empty() or not row.get("text") is String or str(row.text).strip_edges().is_empty():
			last_miss = "no approved translation for this line"
			return {}
		return {"speaker": str(row.get("speaker", "")), "text": str(row.text).strip_edges(), "id": "%s:%d" % [str(key.payload_sha).left(12), key.line]}
	if key.get("record_id") is String:
		if text_layer == null or not (text_layer.has_method("lookup_record_text") or text_layer.has_method("lookup_record")):
			last_miss = "no text layer"
			return {}
		var container_sha := str(key.get("container_sha256", ""))
		var record: Dictionary = text_layer.lookup_record_text(key.record_id, container_sha) if text_layer.has_method("lookup_record_text") \
			else text_layer.lookup_record(key.record_id, container_sha, text_layer_font)
		if record.is_empty() or not record.get("text") is String or str(record.text).strip_edges().is_empty():
			last_miss = "no approved record: %s" % str(text_layer.get("last_rejection"))
			return {}
		return {"speaker": "", "text": str(record.text).strip_edges(), "id": key.record_id}
	last_miss = "unsupported line key"
	return {}

func _refresh() -> void:
	var wanted: bool = text_box_preference(_prefs) == "modern" and not active_record.is_empty() and _layout.get("valid", false)
	visible = wanted
	if not wanted:
		return
	var band: Rect2 = _layout.text_band_wide if _layout.get("mode") == "wide" else _layout.text_band
	var frame: Rect2 = _layout.frame
	var factor: float = _layout.factor
	var margin := maxf(2.0, roundf(3.0 * factor))
	var pad := maxf(4.0, roundf(4.0 * factor))
	var inner_width := maxf(band.size.x - 2.0 * pad, 1.0)
	var text_scale := font_scale_preference(_prefs)
	var preferred := maxi(MIN_FONT_SIZE, roundi(band.size.y * BAND_FONT_SHARE * text_scale))
	var floor_px := mini(preferred, maxi(roundi(MIN_FONT_SIZE * text_scale), roundi(band.size.y * FLOOR_FONT_SHARE * text_scale)))
	var speaker_text: String = active_record.speaker
	_speaker.visible = not speaker_text.is_empty()
	_speaker.text = speaker_text
	_body.text = active_record.text
	# The box covers the whole band at the sides and bottom, so no 1-3 native
	# px of checker frame it, and at the top it reaches `margin` above the
	# band to hide the native window's top border (native rows 159..163 in
	# the opening) instead of leaving a sliver of it above the box.
	var box := Rect2(band.position.x, band.position.y - margin, band.size.x, band.size.y + margin)
	var available := box.size.y - 2.0 * pad
	# The box grows upward to the frame top before the font shrinks, so the
	# accessibility scale holds for ordinary lines.
	var top_limit := frame.position.y + margin
	var max_available := box.end.y - top_limit - 2.0 * pad
	var font_px := preferred
	var text_height := _measure(inner_width, font_px)
	if text_height > max_available:
		font_px = _largest_fitting(inner_width, max_available, floor_px, preferred - 1)
		if font_px < 0:
			# Last resort for lines too long even at the scaled floor: shrink
			# below it rather than hide text, and report it in last_fit.
			font_px = maxi(_largest_fitting(inner_width, max_available, MIN_FONT_SIZE, floor_px - 1), MIN_FONT_SIZE)
		text_height = _measure(inner_width, font_px)
	var grew := false
	if text_height > available:
		var top := maxf(top_limit, box.end.y - (text_height + 2.0 * pad))
		box = Rect2(box.position.x, top, box.size.x, box.end.y - top)
		grew = true
	_panel.position = box.position
	_panel.size = box.size
	var cursor := pad
	if _speaker.visible:
		_speaker.position = Vector2(pad, cursor)
		cursor += _speaker.size.y
	_body.position = Vector2(pad, cursor)
	_body.size = Vector2(inner_width, maxf(box.size.y - cursor - pad, 1.0))
	last_fit = {"font_size": font_px, "preferred": preferred, "floor": floor_px, "below_floor": font_px < floor_px,
		"text_height": text_height, "box": box, "grew": grew, "fits": text_height <= box.size.y - 2.0 * pad + 0.5}

## Largest font size in lo..hi whose text fits `height`, or -1 when none does.
func _largest_fitting(width: float, height: float, lo: int, hi: int) -> int:
	var best := -1
	while lo <= hi:
		var mid := (lo + hi) >> 1
		if _measure(width, mid) <= height:
			best = mid
			lo = mid + 1
		else:
			hi = mid - 1
	return best

## Height the speaker and body Labels need at this width and font size, taken
## from the Labels themselves so wrapping and line spacing match what is drawn.
func _measure(width: float, font_px: int) -> float:
	var height := 0.0
	for label: Label in [_speaker, _body]:
		if not label.visible:
			continue
		label.add_theme_font_size_override("font_size", font_px)
		label.size = Vector2(width, 0.0)
		label.size = Vector2(width, label.get_minimum_size().y)
		height += label.size.y
	return height

## Counts how many {"speaker", "text"} entries the native-glyph English path
## rejects (the line then falls back to Japanese). presenter is an opened
## prologue_text_presenter; native_speaker is passed to translation_layout.
static func native_layout_rejections(presenter: RefCounted, entries: Array, native_speaker := PackedByteArray()) -> Dictionary:
	var unsupported := 0
	var overflow := 0
	for entry: Variant in entries:
		if not entry is Dictionary:
			unsupported += 1
			continue
		if not presenter.translation_supported(entry):
			unsupported += 1
		elif presenter.translation_layout(entry, native_speaker).is_empty():
			overflow += 1
	return {"total": entries.size(), "unsupported_glyphs": unsupported, "layout_overflow": overflow,
		"rejected": unsupported + overflow}
