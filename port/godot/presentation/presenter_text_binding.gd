extends RefCounted
## Phase 4 WP-I: presentation-only English binding for presenters that draw
## original Shift-JIS strings (date, phone, subgame, ending). A binding maps
## the exact original bytes a presenter draws to a text-layer record; the
## presenter asks translate() at its draw seam and keeps the original bytes
## whenever the answer is empty. Nothing here touches game state or timing.
##
## Binding rows (caller-supplied; this class proves only byte identity):
##   {"key": label, "source_hex": original hex as drawn,
##    "container": path, "container_sha256": sha, "offset": file offset}
##     -> TextLayerStore.lookup_source (offset + full source hash checked), or
##   {"key": label, "source_hex": ..., "record_id": logical id,
##    "container_sha256": sha or ""} -> TextLayerStore.lookup_record.
## A source with a 0x20 row break, a record carrying tokens, a glyph the font
## lacks, or a run wider than the original is rejected and logged, and the
## original is drawn (the WP-A strict fallback).

const NativeLatin = preload("res://presentation/native_latin_layout.gd")

var _bound: Dictionary = {}
var _log: Array[Dictionary] = []

func clear() -> void:
	_bound.clear()
	_log.clear()

func bound_count() -> int:
	return _bound.size()

## Rows that stayed Japanese with the reason, in binding order.
func fallback_log() -> Array[Dictionary]:
	return _log.duplicate(true)

## Replaces every binding. `advance` is the presenter's original per-glyph
## step; a translated run may use at most units * advance pixels.
func bind(layer: RefCounted, font: RefCounted, bindings: Array, advance: int) -> int:
	clear()
	if layer == null:
		return 0
	for row: Variant in bindings:
		var reason := _bind_row(layer, font, row, advance)
		if not reason.is_empty():
			_log.append({"key": str(row.get("key", "")) if row is Dictionary else "", "reason": reason})
	return _bound.size()

func _bind_row(layer: RefCounted, font: RefCounted, row: Variant, advance: int) -> String:
	if not row is Dictionary or not row.get("source_hex") is String or not row.get("key") is String:
		return "binding row is malformed"
	var source_hex: String = row.source_hex
	if source_hex.is_empty() or source_hex.length() % 4 != 0 or source_hex.hex_decode().hex_encode() != source_hex:
		return "source is not whole code units"
	var raw: PackedByteArray = source_hex.hex_decode()
	for index: int in range(0, raw.size(), 2):
		if raw[index] < 0x81:
			return "multi-row or single-byte source needs the shared text flow"
	if _bound.has(source_hex):
		return "source bytes already bound"
	var found: Dictionary
	if row.has("record_id"):
		if not row.record_id is String or not row.get("container_sha256", "") is String:
			return "record binding is malformed"
		found = layer.lookup_record(row.record_id, row.get("container_sha256", ""), font)
	elif row.get("container") is String and row.get("container_sha256") is String and typeof(row.get("offset")) == TYPE_INT:
		found = layer.lookup_source(row.container, row.container_sha256, row.offset, raw, font)
	else:
		return "binding names no record or source location"
	if found.is_empty():
		return str(layer.last_rejection)
	if not found.has("codes"):
		return "record carries control tokens"
	var codes: PackedByteArray = found.codes
	var glyph_x := NativeLatin.offsets(font, codes, raw.size() / 2 * advance)
	if glyph_x.is_empty():
		return "translated run does not fit the original run"
	_bound[source_hex] = {"key": row.key, "id": found.id, "hex": codes.hex_encode(), "glyph_x": glyph_x}
	return ""

## {hex, glyph_x, id, key} for bound original bytes, else {}.
func translate(source_hex: String) -> Dictionary:
	return _bound.get(source_hex, {})

## Compositor text runs with bound originals replaced. Runs that already
## carry glyph_x are translated runs and pass through unchanged.
func apply_runs(runs: Array[Dictionary]) -> Array[Dictionary]:
	if _bound.is_empty():
		return runs
	var result: Array[Dictionary] = []
	for run: Dictionary in runs:
		var bound: Dictionary = {} if run.has("glyph_x") else translate(str(run.get("hex", "")))
		if bound.is_empty():
			result.append(run)
			continue
		var copy := run.duplicate(true)
		copy["hex"] = bound.hex
		copy["glyph_x"] = bound.glyph_x
		result.append(copy)
	return result
