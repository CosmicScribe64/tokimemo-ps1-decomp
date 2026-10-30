extends SceneTree
## T-30001 WP4: modern window-space dialogue box.
## Data-free checks always run. Optional data (mounted read-only):
##   MODERN_TEXT_CORPUS  approved text layer pack JSON (text_layer.json); it is
##                       opened with the real text_layer_store, and the 100
##                       longest English records, looked up by id, must wrap
##                       without clipping at 640x480 and 1920x1080 in both
##                       aspects and at font_scale 1.0 and 1.5.
##   MODERN_TEXT_FONT    .../.asset-cache/port/fonts/native-font-v2; reports how
##                       many sampled corpus lines the native-glyph English
##                       layout rejects (Japanese fallback), and checks that the
##                       modern box shows the longest records the native-font
##                       lookup rejects.
## The test prints counts only, never game text.

const Overlay = preload("res://presentation/modern_dialogue_overlay.gd")
const Layout = preload("res://presentation/native_frame_layout.gd")
const Presenter = preload("res://presentation/prologue_text_presenter.gd")
const TranslationStore = preload("res://services/text_translation_store.gd")
const TextLayerStore = preload("res://services/text_layer_store.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const SIZES := [Vector2(640, 480), Vector2(1920, 1080)]
const PAYLOAD := "ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12"

class FakeTranslations extends RefCounted:
	var rows := {}
	func replacement(payload_sha: String, line: int) -> Dictionary:
		if payload_sha != "ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12ab12":
			return {}
		return rows.get(str(line), {}).duplicate()

## Every code resolves to a 1-texel-wide glyph except `missing` (absent).
class StubFont extends RefCounted:
	var missing := PackedByteArray([0x82, 0x9a])
	func get_glyph(pair: PackedByteArray) -> Dictionary:
		if pair == missing:
			return {"status": "unsupported"}
		var pixels := PackedByteArray()
		pixels.resize(144)
		for y: int in 12:
			pixels[y * 12 + 4] = 1
		return {"status": "glyph", "pixels": pixels}

class FakeLayer extends RefCounted:
	var last_rejection := ""
	var records := {}
	func lookup_record(record_id: String, container_sha256: String, font: RefCounted) -> Dictionary:
		if font == null:
			last_rejection = "no original font is open"
			return {}
		if not records.has(record_id) or container_sha256 != "c0":
			last_rejection = "no approved record for this id"
			return {}
		last_rejection = ""
		return {"id": record_id, "text": records[record_id]}

var _checks := 0
var _failures: Array[String] = []
var _report := {}
var _fake_corpus: RefCounted

func _check(condition: bool, message: String) -> void:
	_checks += 1
	if not condition:
		_failures.append(message)

func _initialize() -> void:
	# A runtime error inside the coroutine would otherwise leave the tree running.
	create_timer(600.0).timeout.connect(func() -> void:
		printerr("FAIL: overlay suite timed out")
		quit(1))
	_run.call_deferred()

func _prefs(aspect: String, text_box: String, scale: float = 1.0) -> Dictionary:
	return {"display": {"aspect": aspect, "filter": "integer"}, "accessibility": {"text_box": text_box, "font_scale": scale}}

func _run() -> void:
	var overlay: Control = Overlay.new()
	overlay.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	root.add_child(overlay)
	var translations := FakeTranslations.new()
	translations.rows = {"0": {"speaker": "Shiori", "text": "A short line."},
		"1": {"speaker": "", "text": "word ".repeat(120).strip_edges()},
		"2": {"speaker": "", "text": "   "}}
	var layer := FakeLayer.new()
	layer.records = {"rec:1": "A record line from the generic layer."}
	overlay.set_stores(translations, layer, RefCounted.new())
	_check_visibility(overlay)
	_check_store_swap(overlay, translations, layer)
	_check_negatives(overlay, translations, layer)
	_check_scale(overlay)
	_check_real_stores(overlay)
	await _check_clip_detector()
	await _check_corpus(overlay, _fake_keys(["A short line.", "word ".repeat(400).strip_edges(), "x".repeat(300)]), "synthetic")
	var corpus_path := OS.get_environment("MODERN_TEXT_CORPUS")
	if not corpus_path.is_empty():
		var font_root := OS.get_environment("MODERN_TEXT_FONT")
		var records := _longest_records(corpus_path, 100)
		_check(records.size() == 100, "corpus yields 100 records (got %d)" % records.size())
		var store := TextLayerStore.new()
		_check(store.open_pack(corpus_path, FileAccess.get_sha256(corpus_path)), "real text layer store opens the corpus: " + store.error)
		if not font_root.is_empty():
			_check_native_rejected_reach_modern(overlay, store, _longest_records(corpus_path, 1 << 30), font_root)
		overlay.set_stores(null, store, null)
		await _check_corpus(overlay, records.map(func(r: Dictionary) -> Dictionary: return {"record_id": r.id, "container_sha256": r.container_sha}), "corpus")
		if not font_root.is_empty():
			_report_rejections(corpus_path, font_root)
	else:
		print("NOTE: MODERN_TEXT_CORPUS unset; corpus wrap pass skipped")
	print("REPORT: ", JSON.stringify(_report))
	if _failures.is_empty():
		print("PASS: modern dialogue overlay (%d checks)" % _checks)
		quit(0)
	else:
		for failure: String in _failures.slice(0, 40):
			printerr("FAIL: ", failure)
		printerr("FAIL count: %d" % _failures.size())
		quit(1)

func _check_visibility(overlay: Control) -> void:
	var layout := Layout.compute(Vector2(960, 720), {"aspect": "original"})
	overlay.apply_layout(layout, _prefs("original", "native"))
	_check(overlay.show_line({"payload_sha": PAYLOAD, "line": 0}), "line 0 resolves")
	_check(not overlay.visible, "text_box=native keeps the overlay hidden even with a translation")
	overlay.apply_layout(layout, _prefs("original", "modern"))
	_check(overlay.visible, "text_box=modern shows the resolved line")
	var panel: Panel = overlay.get_child(0)
	var band: Rect2 = layout.text_band
	var covered := band.grow_side(SIDE_TOP, 3.0 * layout.factor)
	_check(covered.encloses(Rect2(panel.position, panel.size)), "panel sits on the native text band (rows 160..239, top raised by the margin)")
	_check(panel.position.y <= band.position.y - layout.factor, "panel covers the native window border at row 159")
	var panel_rect := Rect2(panel.position, panel.size)
	_check(is_equal_approx(panel_rect.position.x, band.position.x) and is_equal_approx(panel_rect.end.x, band.end.x) and is_equal_approx(panel_rect.end.y, band.end.y),
		"panel covers the whole text band at the sides and bottom, leaving no native slivers (%s vs band %s)" % [str(panel_rect), str(band)])
	var style: StyleBoxFlat = panel.get_theme_stylebox("panel")
	_check(style.bg_color.a == 1.0 and style.corner_radius_top_left == 0 and style.corner_radius_bottom_right == 0, "panel is opaque with square corners, so no native pixels show at its corners")
	_check(overlay.active_key == {"payload_sha": PAYLOAD, "line": 0}, "show_line keeps the active key for refresh_line")
	overlay.apply_layout(layout, {"accessibility": {"text_box": "fancy"}})
	_check(not overlay.visible, "unknown text_box value falls back to native")
	overlay.apply_layout(layout, _prefs("original", "modern"))
	overlay.clear_line()
	_check(not overlay.visible and overlay.active_record.is_empty() and overlay.active_key.is_empty(), "clear_line hides the box and drops the key")
	_check(not overlay.refresh_line() and not overlay.visible, "refresh_line after clear_line shows nothing")
	_check(overlay.show_line({"record_id": "rec:1", "container_sha256": "c0"}) and overlay.visible, "text layer record resolves and shows")
	_check(overlay.mouse_filter == Control.MOUSE_FILTER_IGNORE and panel.mouse_filter == Control.MOUSE_FILTER_IGNORE, "overlay never eats pointer input")
	var wide := Layout.compute(Vector2(1920, 1080), {"aspect": "wide"})
	overlay.apply_layout(wide, _prefs("wide", "modern"))
	_check(is_equal_approx(panel.position.x, wide.text_band_wide.position.x) and is_equal_approx(panel.position.x + panel.size.x, wide.text_band_wide.end.x) and is_equal_approx(panel.position.y + panel.size.y, wide.text_band_wide.end.y), "wide panel covers the whole wide text band at the sides and bottom")
	_check(panel.size.x > wide.aperture.size.x and wide.text_band_wide.grow_side(SIDE_TOP, 3.0 * wide.factor).encloses(Rect2(panel.position, panel.size)), "wide mode widens the box into the wings")

## A language switch replaces the stores mid-line; refresh_line resolves the
## same key against the new stores without waiting for the next line.
func _check_store_swap(overlay: Control, translations: FakeTranslations, layer: FakeLayer) -> void:
	var layout := Layout.compute(Vector2(960, 720), {"aspect": "original"})
	overlay.apply_layout(layout, _prefs("original", "modern"))
	_check(overlay.show_line({"payload_sha": PAYLOAD, "line": 0}) and overlay.visible, "line 0 shows before the swap")
	var other := FakeTranslations.new()
	other.rows = {"0": {"speaker": "S", "text": "Another language."}}
	overlay.set_stores(other, layer, RefCounted.new())
	_check(overlay.refresh_line() and overlay.visible and overlay.active_record.text == "Another language.", "refresh_line picks the new store's text for the same line")
	overlay.set_stores(FakeTranslations.new(), layer, RefCounted.new())
	_check(not overlay.refresh_line() and not overlay.visible, "refresh_line hides the box when the new store lacks the line")
	overlay.set_stores(translations, layer, RefCounted.new())
	_check(overlay.refresh_line() and overlay.active_record.text == "A short line.", "refresh_line restores the original store's line")

func _check_negatives(overlay: Control, translations: FakeTranslations, layer: FakeLayer) -> void:
	var layout := Layout.compute(Vector2(960, 720), {"aspect": "original"})
	overlay.apply_layout(layout, _prefs("original", "modern"))
	for key: Dictionary in [{"payload_sha": PAYLOAD, "line": 99}, {"payload_sha": "00", "line": 0},
			{"payload_sha": PAYLOAD, "line": 2}, {"payload_sha": PAYLOAD, "line": "0"},
			{"record_id": "rec:missing", "container_sha256": "c0"}, {"record_id": "rec:1", "container_sha256": "bad"}, {}]:
		_check(not overlay.show_line(key) and not overlay.visible and not overlay.last_miss.is_empty(), "no approved translation hides the box: %s" % [key.keys()])
	overlay.set_stores(null, null, null)
	_check(not overlay.show_line({"payload_sha": PAYLOAD, "line": 0}) and overlay.last_miss == "no translation store", "missing store is a miss")
	overlay.set_stores(translations, layer, null)
	_check(not overlay.show_line({"record_id": "rec:1", "container_sha256": "c0"}), "text layer without font is a miss")
	overlay.set_stores(translations, layer, RefCounted.new())
	overlay.show_line({"payload_sha": PAYLOAD, "line": 0})
	overlay.apply_layout({"valid": false}, _prefs("original", "modern"))
	_check(not overlay.visible, "invalid layout hides the box")

func _check_scale(overlay: Control) -> void:
	var layout := Layout.compute(Vector2(1920, 1080), {"aspect": "original"})
	overlay.show_line({"payload_sha": PAYLOAD, "line": 0})
	overlay.apply_layout(layout, _prefs("original", "modern", 1.0))
	var base: int = overlay.last_fit.font_size
	overlay.apply_layout(layout, _prefs("original", "modern", 1.5))
	_check(overlay.last_fit.font_size > base, "font_scale 1.5 enlarges short text (%d -> %d)" % [base, overlay.last_fit.font_size])
	overlay.apply_layout(layout, _prefs("original", "modern", 9.0))
	_check(overlay.last_fit.font_size == roundi(layout.text_band.size.y * Overlay.BAND_FONT_SHARE * 1.5), "font_scale clamps to 1.5")
	# Ordinary long dialogue: the accessibility scale must still hold, and the
	# box must grow (to the frame top) before the font shrinks.
	var saved: RefCounted = overlay.translation_store
	var long_lines := FakeTranslations.new()
	for chars: int in [101, 202, 303]:
		long_lines.rows[str(chars)] = {"speaker": "Speaker", "text": _sentence(chars)}
	overlay.set_stores(long_lines, overlay.text_layer, overlay.text_layer_font)
	for window: Vector2 in SIZES:
		for aspect: String in ["original", "wide"]:
			var at := Layout.compute(window, {"aspect": aspect, "filter": "integer"})
			for chars: int in [101, 202, 303]:
				var tag := "%d chars at %s %s" % [chars, window, aspect]
				_check(overlay.show_line({"payload_sha": PAYLOAD, "line": chars}), tag + " resolves")
				var fonts := {}
				for text_scale: float in [1.0, 1.5]:
					overlay.apply_layout(at, _prefs(aspect, "modern", text_scale))
					var fit: Dictionary = overlay.last_fit
					fonts[text_scale] = fit.font_size
					_check(fit.fits and not fit.below_floor, "%s scale %.1f fits at or above the floor (font %d, floor %d)" % [tag, text_scale, fit.font_size, fit.floor])
					if fit.font_size < fit.preferred:
						# Shrinking happens only once the box is as tall as the frame
						# allows; the font is the largest that fits, so less than two
						# rows of slack may remain above the text.
						var limit: float = at.frame.position.y + maxf(2.0, roundf(3.0 * at.factor))
						_check(fit.grew and fit.box.position.y - limit < 2.0 * fit.font_size, "%s scale %.1f grows to the frame top before shrinking (top %.0f, limit %.0f)" % [tag, text_scale, fit.box.position.y, limit])
				_check(fonts[1.5] > fonts[1.0], "%s: font_scale 1.5 gives a larger font (%d vs %d)" % [tag, fonts[1.5], fonts[1.0]])
				_report["scale_%d_%dx%d_%s" % [chars, window.x, window.y, aspect]] = [fonts[1.0], fonts[1.5]]
	overlay.set_stores(saved, overlay.text_layer, overlay.text_layer_font)

static func _sentence(chars: int) -> String:
	var text := ""
	var words := ["the", "letter", "under", "a", "tree", "said", "something", "quiet", "about", "spring"]
	var index := 0
	while text.length() < chars:
		text += ("" if text.is_empty() else " ") + words[index % words.size()]
		index += 1
	return text.left(chars - 1) + "."

## Real stores, not fakes: an approved line the native glyph set cannot draw
## is rejected by the native lookups but must reach the modern box.
func _check_real_stores(overlay: Control) -> void:
	var folder := ProjectSettings.globalize_path("user://modern-overlay-tests")
	DirAccess.make_dir_recursive_absolute(folder)
	var sha := "cd34".repeat(16)
	var unsupported_text := "Caf\u00e9 \u266a, na\u00efve."
	var pack := {"schema": TranslationStore.SCHEMA, "language": "en", "source_payload_sha256": sha,
		"entries": {"0": {"speaker": "", "text": "Plain line."}, "1": {"speaker": "", "text": unsupported_text}}}
	var translations := TranslationStore.new()
	var pack_path := folder.path_join("translation.json")
	_check(translations.open_pack(pack_path, _write_json(pack_path, pack)), "real translation pack opens: " + translations.error)
	_check(translations.unsupported_entries.has("1") and translations.replacement(sha, 1).is_empty(), "native lookup rejects the line the original glyphs lack")
	var container_sha := "c".repeat(64)
	var layer_pack := {"schema": TextLayerStore.SCHEMA, "language": "en",
		"approval": {"approver": "test", "date": "2026-10-30", "instruction": "x", "scope": "x", "source": "x"},
		"font": {"source_path": "SLPM_86.053", "source_sha256": "a".repeat(64), "glyph_bank_sha256": "b".repeat(64)},
		"containers": {"CDROM/X.BIN": {"sha256": container_sha, "size": 10}},
		"entries": {"c:UNIT:1": {"text": "Needs a missing glyph.", "segments": [["g", "829a"]], "context_status": "ready_for_review", "container": "CDROM/X.BIN"}}}
	var layer := TextLayerStore.new()
	var layer_path := folder.path_join("layer.json")
	_check(layer.open_pack(layer_path, _write_json(layer_path, layer_pack)), "real text layer pack opens: " + layer.error)
	var font := StubFont.new()
	_check(layer.lookup_record("c:UNIT:1", container_sha, font).is_empty() and layer.last_rejection.contains("glyph"), "native lookup rejects the record for a missing glyph")
	var saved_translations: RefCounted = overlay.translation_store
	var saved_layer: RefCounted = overlay.text_layer
	var saved_font: RefCounted = overlay.text_layer_font
	overlay.set_stores(translations, layer, font)
	overlay.apply_layout(Layout.compute(Vector2(960, 720), {"aspect": "original"}), _prefs("original", "modern"))
	_check(overlay.show_line({"payload_sha": sha, "line": 1}) and overlay.visible and overlay.active_record.text == unsupported_text, "modern box shows the translation the native glyphs cannot draw")
	_check(overlay.show_line({"payload_sha": sha, "line": 0}) and overlay.active_record.text == "Plain line.", "modern box still shows supported lines")
	_check(not overlay.show_line({"payload_sha": sha, "line": 7}) and not overlay.visible, "unknown line stays hidden with real stores")
	_check(not overlay.show_line({"payload_sha": "0".repeat(64), "line": 1}), "other payload stays hidden with real stores")
	_check(overlay.show_line({"record_id": "c:UNIT:1", "container_sha256": container_sha}) and overlay.visible and overlay.active_record.text == "Needs a missing glyph.", "modern box shows the record rejected for a missing glyph")
	_check(not overlay.show_line({"record_id": "c:UNIT:1", "container_sha256": "d".repeat(64)}) and overlay.last_miss.contains("container"), "container hash still gates the modern lookup")
	_check(not overlay.show_line({"record_id": "c:UNIT:9", "container_sha256": container_sha}), "unknown record stays hidden with real stores")
	overlay.set_stores(saved_translations, saved_layer, saved_font)

func _write_json(path: String, value: Dictionary) -> String:
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(value))
	file.close()
	return FileAccess.get_sha256(path)

## With the real native font: count the corpus records the native-font
## lookup rejects, and check the modern box shows every one of them.
func _check_native_rejected_reach_modern(overlay: Control, store: RefCounted, records: Array, font_root: String) -> void:
	var font := Glyphs.new()
	var failure: String = font.open_external_font(font_root)
	_check(failure.is_empty(), "native glyphs open: " + failure)
	if not failure.is_empty():
		return
	overlay.set_stores(null, store, font)
	overlay.apply_layout(Layout.compute(Vector2(960, 720), {"aspect": "original"}), _prefs("original", "modern"))
	var rejected := 0
	var shown := 0
	var reasons := {}
	for record: Dictionary in records:
		if not store.lookup_record(record.id, record.container_sha, font).is_empty():
			continue
		rejected += 1
		reasons[store.last_rejection] = int(reasons.get(store.last_rejection, 0)) + 1
		if overlay.show_line({"record_id": record.id, "container_sha256": record.container_sha}) and overlay.visible:
			shown += 1
	_report["text_layer_native_lookup"] = {"records": records.size(), "rejected": rejected, "shown_modern": shown, "reasons": reasons}
	_check(shown == rejected, "every native-rejected text layer record reaches the modern box (%d of %d)" % [shown, rejected])

## Negative control: the overflow check used below must notice a Label that
## is too small for its text.
func _check_clip_detector() -> void:
	var label := Label.new()
	label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	label.text = "word ".repeat(200)
	label.position = Vector2.ZERO
	label.size = Vector2(200, 40)
	root.add_child(label)
	await process_frame
	# A wrapping Label grows to its content instead of hiding rows, so overflow
	# shows up as the Label extending past the rect it was given; the corpus
	# pass checks exactly that ("body leaves panel").
	_check(label.size.y > 40.0, "clip detector sees an undersized Label grow past its rect (%.0f)" % label.size.y)
	label.queue_free()

func _fake_keys(lines: Array) -> Array:
	var translations := FakeTranslations.new()
	var keys: Array = []
	for index: int in lines.size():
		translations.rows[str(index)] = {"speaker": "Speaker" if index % 3 == 0 else "", "text": lines[index]}
		keys.append({"payload_sha": PAYLOAD, "line": index})
	_fake_corpus = translations
	return keys

## keys are line keys for the stores set on the overlay; the synthetic pass
## uses the fake that _fake_keys built.
func _check_corpus(overlay: Control, keys: Array, tag: String) -> void:
	if tag == "synthetic":
		overlay.set_stores(_fake_corpus, null, null)
	var body: Label = overlay.get_child(0).get_child(1)
	var worst := {"min_font": 999, "grew": 0, "clipped": 0, "below_floor": 0, "below_floor_cases": [], "scale_not_larger": 0}
	var font_at_1 := {}
	for size: Vector2 in SIZES:
		for aspect: String in ["original", "wide"]:
			for text_scale: float in [1.0, 1.5]:
				var layout := Layout.compute(size, {"aspect": aspect, "filter": "integer"})
				overlay.apply_layout(layout, _prefs(aspect, "modern", text_scale))
				for index: int in keys.size():
					_check(overlay.show_line(keys[index]), "%s key %d resolves: %s" % [tag, index, overlay.last_miss])
					await process_frame
					var panel: Panel = overlay.get_child(0)
					var box := Rect2(panel.position, panel.size)
					var reasons: Array[String] = []
					if body.get_visible_line_count() < body.get_line_count(): reasons.append("label rows hidden")
					if not overlay.last_fit.fits: reasons.append("measured text taller than box")
					if not layout.frame.encloses(box): reasons.append("box leaves frame")
					if body.position.y + body.size.y > panel.size.y + 0.5: reasons.append("body leaves panel")
					if not reasons.is_empty():
						worst.clipped += 1
						_failures.append("%s line %d clips at %s %s scale %.1f (rows %d/%d, font %d): %s" % [tag, index, size, aspect, text_scale, body.get_visible_line_count(), body.get_line_count(), overlay.last_fit.font_size, ", ".join(reasons)])
					_checks += 1
					var font_px: int = overlay.last_fit.font_size
					var slot := "%s/%s/%d" % [size, aspect, index]
					if text_scale == 1.0:
						font_at_1[slot] = font_px
					elif font_px < int(font_at_1.get(slot, 0)):
						worst.scale_not_larger += 1
					if overlay.last_fit.below_floor:
						worst.below_floor += 1
						worst.below_floor_cases.append("%dx%d %s %.1f chars %d font %d floor %d" % [size.x, size.y, aspect, text_scale, body.text.length(), font_px, overlay.last_fit.floor])
					worst.min_font = mini(worst.min_font, font_px)
					worst.grew += 1 if overlay.last_fit.grew else 0
	_check(worst.scale_not_larger == 0, "%s: font_scale 1.5 never gives a smaller font than 1.0 (%d cases)" % [tag, worst.scale_not_larger])
	_report[tag] = worst

## The `count` longest non-empty records as {"id", "container_sha", "text"}.
func _longest_records(path: String, count: int) -> Array:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not parsed is Dictionary or not parsed.get("entries") is Dictionary or not parsed.get("containers") is Dictionary:
		return []
	var rows: Array = []
	for key: Variant in parsed.entries:
		var entry: Dictionary = parsed.entries[key]
		var text: Variant = entry.get("text")
		if text is String and not text.strip_edges().is_empty():
			var container := str(entry.get("container", ""))
			var container_sha := ""
			if not container.is_empty() and parsed.containers.get(container) is Dictionary:
				container_sha = str(parsed.containers[container].get("sha256", ""))
			rows.append({"id": key, "container_sha": container_sha, "text": text.strip_edges()})
	rows.sort_custom(func(a: Dictionary, b: Dictionary) -> bool: return a.text.length() > b.text.length() if a.text.length() != b.text.length() else a.id < b.id)
	return rows.slice(0, count)

func _longest_lines(path: String, count: int) -> Array:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	_check(parsed is Dictionary and parsed.get("schema") == "tokimemo.text-layer-pack.v1", "corpus is a text layer pack")
	_check(parsed is Dictionary and parsed.get("approval") is Dictionary and not str(parsed.approval.get("approver", "")).is_empty(), "corpus carries its approval record")
	if not parsed is Dictionary or not parsed.get("entries") is Dictionary:
		return []
	var texts: Array = []
	for key: Variant in parsed.entries:
		var text: Variant = parsed.entries[key].get("text")
		if text is String and not text.strip_edges().is_empty():
			texts.append(text.strip_edges())
	texts.sort_custom(func(a: String, b: String) -> bool: return a.length() > b.length() if a.length() != b.length() else a < b)
	_report["corpus_entries"] = texts.size()
	_report["longest_chars"] = [texts[0].length(), texts[count - 1].length()] if texts.size() >= count else []
	return texts.slice(0, count)

func _report_rejections(corpus_path: String, font_root: String) -> void:
	var presenter := Presenter.new()
	var failure: String = presenter.open_font(font_root)
	_check(failure.is_empty(), "native font opens: " + failure)
	if not failure.is_empty():
		return
	var parsed: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(corpus_path))
	var keys: Array = parsed.entries.keys()
	keys.sort()
	var all_entries: Array = []
	var unsupported_all := 0
	for key: String in keys:
		var entry := {"speaker": "", "text": str(parsed.entries[key].get("text", ""))}
		all_entries.append(entry)
		if not Presenter.translation_supported(entry):
			unsupported_all += 1
	# Full layout is glyph-by-glyph; sample every 50th record plus the longest 100.
	var sample: Array = []
	for index: int in range(0, all_entries.size(), 50):
		sample.append(all_entries[index])
	var sampled := Overlay.native_layout_rejections(presenter, sample)
	var longest := Overlay.native_layout_rejections(presenter, _longest_lines(corpus_path, 100).map(func(t: String) -> Dictionary: return {"speaker": "", "text": t}))
	_report["native_rejections"] = {"all_unsupported_glyphs": unsupported_all, "all_total": all_entries.size(),
		"sample_every_50th": sampled, "longest_100": longest}
	_check(sampled.total > 0 and sampled.rejected <= sampled.total, "rejection metric is computed")
