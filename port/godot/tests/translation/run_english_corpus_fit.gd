extends SceneTree
## T-39201 (Phase 4 English WP-A): corpus fit report for the English text layer.
##
## Every representable record of the built text-layer pack goes through the
## runtime path: TextLayerStore.lookup_record (with the record's container
## hash and the original font), then NativeTextFlow.prepare/fit, the same
## calls NativeTextFlow.layout makes for a screen. Names expand to the
## worst case (4 full-width 'W' per 8-byte field). Each record is classified
## against every box class as fits_markers, fits_rewrap, overflow or
## rejected_token, with a reason. Records with choice/reply separators ('q')
## are split first; a record takes its worst variant.
##
## Environment:
##   TEXT_LAYER_PACK / TEXT_LAYER_SHA256  built pack (tools/port/build_text_pack.py)
##   TEXT_LAYER_COVERAGE                  optional coverage.json of that build;
##                                        its rejected counts are carried over
##                                        so every input record is classified
##   FONT_ROOT                            .../.asset-cache/port/fonts/native-font-v2
##   FIT_OUT                              output directory; fit.json must not exist
## Writes FIT_OUT/fit.json (ids and counts, no game text) and
## FIT_OUT/readability-sample.png (original glyph renders of a few records).
const Store = preload("res://services/text_layer_store.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const Flow = preload("res://presentation/native_text_flow.gd")
const RANK := {"fits_markers": 0, "fits_rewrap": 1, "overflow": 2, "rejected_token": 3}
const PHYSICAL_PREFIXES := ["SLPM_86.053:", "CDROM/"]
## Headline screens: which records, measured in which box. Only talk records
## have a proven draw site (talk_speak_part); the other families are reported
## per box class only. The 4:3 screens use the port_runtime classes, the
## boxes the English hooks fit against (NativeTextFlow.runtime_box). Each
## names the C call-site class as `reference`; an overflow that fits the
## reference box is counted as `overflow_fits_reference` (it fails only
## because the port's body origin is narrower than the C one).
const SCREENS := {
	"talk_speaker_4x3": {"family": "talk", "choice": false, "box": "textbox_line_label", "reference": "dialogue_4x3_speaker"},
	"talk_narration_4x3": {"family": "talk", "choice": false, "box": "textbox_line", "reference": "dialogue_4x3"},
	"talk_vn_speaker_4x3": {"family": "talk", "choice": false, "box": "vn_body_speaker", "reference": "dialogue_4x3_speaker"},
	"talk_vn_quoted_4x3": {"family": "talk", "choice": false, "box": "vn_body_quoted", "reference": "dialogue_4x3"},
	"talk_speaker_wide": {"family": "talk", "choice": false, "box": "dialogue_wide_speaker", "reference": ""},
	"talk_narration_wide": {"family": "talk", "choice": false, "box": "dialogue_wide", "reference": ""},
	"talk_choice_4x3": {"family": "talk", "choice": true, "box": "textbox_choice", "reference": "choice_cell"},
	"speaktst_layout_4x3": {"family": "speaktst-layout", "choice": false, "box": "textbox_line", "reference": "dialogue_4x3"},
}
const MARGIN_SCREEN := "talk_speaker_4x3"
const MARGIN_COUNT := 200

var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func _initialize() -> void:
	var out_dir := OS.get_environment("FIT_OUT")
	check(not out_dir.is_empty() and DirAccess.dir_exists_absolute(out_dir), "FIT_OUT names an existing directory")
	check(not FileAccess.file_exists(out_dir.path_join("fit.json")), "fit.json does not exist yet")
	var font := Glyphs.new()
	var font_root := OS.get_environment("FONT_ROOT")
	if font_root.is_empty():
		font_root = "/workspace/.asset-cache/port/fonts/native-font-v2"
	var font_error: String = font.open_external_font(font_root)
	check(font_error.is_empty(), "original font opens: " + font_error)
	var layer := Store.new()
	var started := Time.get_ticks_msec()
	check(layer.open_pack(OS.get_environment("TEXT_LAYER_PACK"), OS.get_environment("TEXT_LAYER_SHA256")), "text layer opens: " + layer.error)
	if not failures.is_empty():
		_finish()
		return
	print("Pack entries: ", layer.entry_count(), " open_ms: ", Time.get_ticks_msec() - started)
	check(layer.font_glyph_bank_sha256 == font._metadata.get("glyph_bank_sha256", ""), "pack and font name the same glyph bank")
	var metrics := Flow.Metrics.new(font)
	var names := Flow.worst_case_names()
	var boxes: Array = Flow.BOX_CLASSES.keys()
	var by_family := {}
	var reasons := {}
	var screens := {}
	for screen: String in SCREENS:
		screens[screen] = {"box": SCREENS[screen].box, "box_width": Flow.box(SCREENS[screen].box).width,
			"reference": SCREENS[screen].reference, "records": 0, "outcomes": _zero_outcomes(), "reasons": {},
			"overflow_fits_reference": 0}
	var margins: Array = []
	var overflows: Array = []
	var classified := 0
	var lookup_rejected := {}
	var talk_units := 0
	var talk_width := 0
	var samples: Array = []
	started = Time.get_ticks_msec()
	var ids: Array = layer._entries.keys()
	ids.sort()
	for record_id: String in ids:
		var entry: Dictionary = layer._entries[record_id]
		var container_sha: String = layer._containers.get(entry.container, "")
		var found: Dictionary = layer.lookup_record(record_id, container_sha, font)
		var family := _family(record_id)
		if not by_family.has(family):
			by_family[family] = {"records": 0}
			for name: String in boxes:
				by_family[family][name] = _zero_outcomes()
			reasons[family] = {}
		by_family[family].records += 1
		classified += 1
		if found.is_empty():
			lookup_rejected[layer.last_rejection] = int(lookup_rejected.get(layer.last_rejection, 0)) + 1
			for name: String in boxes:
				by_family[family][name]["rejected_token"] += 1
				_count(reasons[family], name + ":lookup:" + layer.last_rejection)
			continue
		var is_choice: bool = str(found.text).contains(":choice_option]]")
		var variants: Array = Flow.split_variants(found.segments)
		var prepared: Array = []
		for variant: Array in variants:
			prepared.append(Flow.prepare(variant, names))
		var per_box := {}
		for name: String in boxes:
			var worst := {}
			for item: Dictionary in prepared:
				var result: Dictionary = Flow.fit(item, name, metrics)
				if worst.is_empty() or RANK[result.outcome] > RANK[worst.outcome] \
						or (RANK[result.outcome] == RANK[worst.outcome] and int(result.get("slack", 0)) < int(worst.get("slack", 0))):
					worst = result
			per_box[name] = worst
			by_family[family][name][worst.outcome] += 1
			if worst.outcome in ["overflow", "rejected_token"]:
				_count(reasons[family], name + ":" + worst.outcome + ":" + worst.reason)
		for screen: String in SCREENS:
			var spec: Dictionary = SCREENS[screen]
			if spec.family != family or spec.choice != is_choice:
				continue
			var result: Dictionary = per_box[spec.box]
			screens[screen].records += 1
			screens[screen].outcomes[result.outcome] += 1
			var fits_reference := false
			if result.outcome == "overflow" and not str(spec.reference).is_empty():
				fits_reference = per_box[spec.reference].outcome in ["fits_markers", "fits_rewrap"]
				if fits_reference:
					screens[screen].overflow_fits_reference += 1
			if result.outcome in ["overflow", "rejected_token"]:
				_count(screens[screen].reasons, result.outcome + ":" + result.reason + (":fits_reference_box" if fits_reference else ""))
			if screen == MARGIN_SCREEN:
				if result.outcome in ["fits_markers", "fits_rewrap"]:
					margins.append({"id": record_id, "outcome": result.outcome, "slack": int(result.slack), "rows": int(result.needed_rows)})
					if samples.size() < 10 and record_id.ends_with("l0000") and result.rows.size() >= 2:
						samples.append(result.rows)
				elif result.outcome == "overflow":
					overflows.append({"id": record_id, "reason": result.reason, "fits_reference_box": fits_reference,
						"needed_rows": int(result.get("needed_rows", -1)),
						"marker_rows": int(result.get("marker_rows", -1)), "slack": int(result.get("slack", 0))})
		if family == "talk" and not is_choice:
			for item: Dictionary in prepared:
				if item.has("rows"):
					for row: PackedByteArray in item.rows:
						var width := metrics.width(row)
						if width > 0:
							talk_units += row.size() / 2
							talk_width += width
	var elapsed := Time.get_ticks_msec() - started
	margins.sort_custom(func(a: Dictionary, b: Dictionary) -> bool: return a.slack < b.slack or (a.slack == b.slack and a.id < b.id))
	overflows.sort_custom(func(a: Dictionary, b: Dictionary) -> bool: return a.needed_rows > b.needed_rows or (a.needed_rows == b.needed_rows and a.id < b.id))
	for screen: String in screens:
		var row: Dictionary = screens[screen]
		var fits: int = row.outcomes.fits_markers + row.outcomes.fits_rewrap
		row["fit_percent"] = snappedf(100.0 * fits / maxf(1.0, row.records), 0.01)
	var coverage := _coverage()
	var report := {
		"schema": "tokimemo.english-fit.v2",
		"pack": {"sha256": layer.digest, "entries": layer.entry_count(), "glyph_bank_sha256": layer.font_glyph_bank_sha256},
		"names": "worst case: 4 full-width W per 8-byte name field, 8 for F (family + given)",
		"box_classes": Flow.BOX_CLASSES,
		"classified": classified,
		"crashes": 0,
		"elapsed_ms": elapsed,
		"input_records": coverage.get("records", -1),
		"not_representable": coverage.get("rejected", {}),
		"lookup_rejected": lookup_rejected,
		"by_family": by_family,
		"reasons": reasons,
		"screens": screens,
		"glyph_metrics": _glyph_metrics(metrics, font),
		"talk_dialogue_mean_px_per_code_unit": snappedf(float(talk_width) / maxf(1.0, talk_units), 0.01),
		"original_px_per_code_unit": 14,
		"tightest_fits": margins.slice(0, MARGIN_COUNT),
		"worst_overflows": overflows.slice(0, MARGIN_COUNT),
		"margin_screen": MARGIN_SCREEN,
	}
	check(classified == layer.entry_count(), "every pack record is classified")
	if coverage.has("records"):
		var not_in_pack := 0
		for value: Variant in coverage.get("rejected", {}).values():
			not_in_pack += int(value)
		report["all_input_records_classified"] = classified + not_in_pack == int(coverage.records)
		check(report.all_input_records_classified, "pack records plus builder rejections cover every input record")
	if failures.is_empty():
		var file := FileAccess.open(out_dir.path_join("fit.json"), FileAccess.WRITE)
		file.store_string(JSON.stringify(report, " ", true) + "\n")
		file.close()
		_render_samples(samples, font, out_dir.path_join("readability-sample.png"))
		for screen: String in screens:
			print("%s (%s, %d px): %d records, fit %.2f%% %s, overflow fitting %s: %d" % [screen, screens[screen].box, screens[screen].box_width, screens[screen].records, screens[screen].fit_percent, str(screens[screen].outcomes), screens[screen].reference, screens[screen].overflow_fits_reference])
	print("classified=%d elapsed_ms=%d" % [classified, elapsed])
	_finish()

func _family(record_id: String) -> String:
	for prefix: String in PHYSICAL_PREFIXES:
		if record_id.begins_with(prefix):
			return "physical"
	return record_id.get_slice(":", 0)

func _zero_outcomes() -> Dictionary:
	return {"fits_markers": 0, "fits_rewrap": 0, "overflow": 0, "rejected_token": 0}

func _count(table: Dictionary, key: String) -> void:
	table[key] = int(table.get(key, 0)) + 1

func _coverage() -> Dictionary:
	var path := OS.get_environment("TEXT_LAYER_COVERAGE")
	if path.is_empty() or not FileAccess.file_exists(path):
		return {}
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	return parsed if parsed is Dictionary else {}

## Advance and lit-row extent of the Latin letters and digits in the
## original font, for the readability decision.
func _glyph_metrics(metrics: Flow.Metrics, font: RefCounted) -> Dictionary:
	var groups := {"upper": [0x8260, 26], "lower": [0x8281, 26], "digit": [0x824F, 10]}
	var result := {}
	for group: String in groups:
		var first: int = groups[group][0]
		var advances: Array = []
		var heights: Array = []
		for code: int in range(first, first + int(groups[group][1])):
			advances.append(metrics.advance(code))
			var glyph: Dictionary = font.get_glyph(PackedByteArray([code >> 8, code & 0xFF]))
			var top := 12
			var bottom := -1
			var pixels: PackedByteArray = glyph.get("pixels", PackedByteArray())
			for index: int in pixels.size():
				if pixels[index] != 0:
					top = mini(top, index / 12)
					bottom = maxi(bottom, index / 12)
			heights.append(bottom - top + 1 if bottom >= top else 0)
		var total := 0
		for value: int in advances:
			total += value
		result[group] = {"advance_min": advances.min(), "advance_max": advances.max(),
			"advance_mean": snappedf(float(total) / advances.size(), 0.01),
			"ink_height_min": heights.min(), "ink_height_max": heights.max()}
	result["space_advance"] = metrics.advance(Flow.SPACE)
	return result

## Draws a few fitting records into labelled-textbox geometry (body x 86, rows at y
## 170/186/202) with the original glyph colors and drop shadow, 1x and 3x.
func _render_samples(samples: Array, font: RefCounted, path: String) -> void:
	if samples.is_empty():
		return
	var image := Image.create(320, 56 * samples.size(), false, Image.FORMAT_RGBA8)
	image.fill(Color8(24, 32, 64, 255))
	for index: int in samples.size():
		var top := index * 56 + 6
		for row_index: int in samples[index].size():
			var row: Dictionary = samples[index][row_index]
			var codes: PackedByteArray = row.codes
			for glyph_index: int in range(row.x.size()):
				var pair := codes.slice(glyph_index * 2, glyph_index * 2 + 2)
				var glyph: Dictionary = font.get_glyph_image(pair, 0)
				if glyph.get("status") != "glyph":
					continue
				var origin := Vector2i(Flow.TEXTBOX_LABEL_BODY_X + row.x[glyph_index], top + row_index * 16)
				var tile: Image = glyph.image
				for y: int in 12:
					for x: int in 12:
						if tile.get_pixel(x, y).a > 0:
							image.set_pixel(origin.x + x + 1, origin.y + y + 1, Color.BLACK)
				for y: int in 12:
					for x: int in 12:
						if tile.get_pixel(x, y).a > 0:
							image.set_pixel(origin.x + x, origin.y + y, tile.get_pixel(x, y))
	image.save_png(path)
	var large := image.duplicate()
	large.resize(image.get_width() * 3, image.get_height() * 3, Image.INTERPOLATE_NEAREST)
	large.save_png(path.get_basename() + "-3x.png")

func _finish() -> void:
	print("English corpus fit failures: ", failures)
	quit(0 if failures.is_empty() else 1)
