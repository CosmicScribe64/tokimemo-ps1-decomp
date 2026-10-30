extends RefCounted
## T-39016: proportional placement of translated text in original glyphs.
## The original renderer advances every code unit by 14 pixels. Translated
## runs keep each original texel, trim empty side bearings and leave one
## shadow pixel plus one clear pixel between glyphs; the full-width space
## advances 4 pixels. This is the translation-only spacing already used by
## presentation/prologue_text_presenter.gd (_latin_metrics), applied here to
## any single-row run.

const SPACE := 0x8140

## Returns the x offset of each glyph relative to the run origin, or an empty
## array when a glyph is unavailable or the run is wider than `max_width`.
static func offsets(font: RefCounted, codes: PackedByteArray, max_width: int) -> PackedInt32Array:
	var result := PackedInt32Array()
	if font == null or codes.is_empty() or codes.size() % 2 != 0:
		return result
	var cursor := 0
	for index: int in range(0, codes.size(), 2):
		var metric := _metrics(font, codes.slice(index, index + 2))
		if metric.x < 0 or cursor + metric.y > max_width:
			return PackedInt32Array()
		result.append(cursor - metric.x)
		cursor += metric.y
	return result

## x = left bearing to trim, y = advance; x < 0 when the glyph is missing.
static func _metrics(font: RefCounted, pair: PackedByteArray) -> Vector2i:
	if (pair[0] << 8 | pair[1]) == SPACE:
		return Vector2i(0, 4)
	var glyph: Dictionary = font.get_glyph(pair)
	if glyph.get("status") != "glyph":
		return Vector2i(-1, 0)
	var pixels: PackedByteArray = glyph["pixels"]
	var left := 12
	var right := -1
	for index: int in range(pixels.size()):
		if pixels[index] != 0:
			left = mini(left, index % 12)
			right = maxi(right, index % 12)
	return Vector2i(left, right - left + 3) if right >= left else Vector2i(0, 4)
