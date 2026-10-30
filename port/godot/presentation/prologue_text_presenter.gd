extends RefCounted
## T-29025: render committed numeric text work using external source glyphs.
## Timing belongs to PrologueTextWork. Rendering never advances simulation.
const Glyphs = preload("res://presentation/native_glyphs.gd")
const TEXT_BOUNDS := Rect2i(27,166,266,52)
const ORIGIN := Vector2i(160,120)
const ADVANCE := 14
const TRANSLATED_LEFT := 34
const TRANSLATED_RIGHT := 286
var _font := Glyphs.new()
var _open := false
# Bounded immutable-source raster plans; never cache mutable game/text state.
var _verified_views: Dictionary = {}
var _glyph_plans: Dictionary = {}
const MAX_VERIFIED_VIEWS := 64
const MAX_GLYPH_PLANS := 256

func open_font(root: String) -> String:
	_verified_views.clear()
	_glyph_plans.clear()
	_open = false
	var failure: String = _font.open_external_font(root)
	_open = failure.is_empty()
	return failure

func preflight(view: Dictionary) -> String:
	var view_error: Variant = view.get("error")
	if not _open or not view_error is String or not view_error.is_empty():
		return "prologue text needs an open original font and resolved line view"
	if not view.get("speaker_raw") is PackedByteArray or not view.get("rows") is Array:
		return "prologue text view lacks source speaker/row bytes"
	var arrays: Array = [view.speaker_raw]
	for row: Variant in view.rows:
		if not row is Dictionary or not row.get("raw") is PackedByteArray:
			return "prologue source row is malformed"
		arrays.append(row.raw)
	var view_key := ""
	for raw: PackedByteArray in arrays:
		if raw.is_empty() or raw.size() % 2 != 0:
			return "prologue source row is empty or incomplete"
		view_key += ":"+raw.hex_encode()
	if _verified_views.has(view_key): return ""
	for raw: PackedByteArray in arrays:
		for offset in range(0,raw.size(),2):
			if _font.get_glyph(raw.slice(offset,offset+2)).get("status") != "glyph":
				return "prologue source code unit is unavailable in the original font"
	if _verified_views.size() >= MAX_VERIFIED_VIEWS: _verified_views.clear()
	_verified_views[view_key] = true

	return ""

func render_into(target: Image, view: Dictionary, work: Dictionary, brightness: int) -> Dictionary:
	var failure := preflight(view)
	if not failure.is_empty():
		return {"error":failure}
	if target == null or target.get_size() != Vector2i(320,240) or brightness < 0 or brightness > 128:
		return {"error":"prologue text target/brightness is outside native bounds"}
	if work.get("line_id") != view.get("line_id") or not work.get("entries") is Array or typeof(work.get("pos")) != TYPE_INT or typeof(work.get("total")) != TYPE_INT or work.pos < 0 or work.pos > work.total:
		return {"error":"committed prologue text work does not match the resolved line"}
	var remaining: int = work.pos
	var glyphs := 0
	var pixels := 0
	var plan: Array[Dictionary] = []
	for entry: Variant in work.entries:
		if not entry is Dictionary or typeof(entry.get("row")) != TYPE_INT or typeof(entry.get("glyph_count")) != TYPE_INT or typeof(entry.get("x")) != TYPE_INT or typeof(entry.get("y")) != TYPE_INT or typeof(entry.get("active")) != TYPE_BOOL or entry.get("attribute") != 0:
			return {"error":"prologue text work entry is outside the native attribute contract"}
		var row: int = entry.row
		if row < -1 or row >= view.rows.size():
			return {"error":"prologue text work selects an unknown source row"}
		var raw: PackedByteArray = view.speaker_raw if row == -1 else view.rows[row].raw
		if entry.glyph_count != raw.size() / 2:
			return {"error":"prologue glyph count differs from the prepared source row"}
		var visible: int = clampi(remaining,0,entry.glyph_count)
		remaining -= int(entry.glyph_count)
		if not entry.active:
			continue
		for index in range(visible):
			var pair: PackedByteArray = raw.slice(index*2,index*2+2)
			var position: Vector2i = ORIGIN + Vector2i(entry.x + index*ADVANCE,entry.y)
			var item: Dictionary = _glyph_plan(pair,position,brightness)
			if not item.error.is_empty():
				return item
			plan.append(item)
			pixels += item.pixels.size()
			glyphs += 1
	# Validate every foreground/shadow texel before touching the destination.
	# An overflow is a failed layout contract, never silently clipped away.
	for glyph: Dictionary in plan:
		for pixel: Dictionary in glyph.pixels:
			if not TEXT_BOUNDS.has_point(pixel.position):
				return {"error":"prologue glyph or shadow overflows the native message bounds","position":pixel.position}
	# Every text SPRT is head-inserted. Later glyphs draw before earlier glyphs;
	# each glyph's shadow draws before its foreground.
	for index in range(plan.size()-1,-1,-1):
		for pixel: Dictionary in plan[index].pixels:
			target.set_pixelv(pixel.position,pixel.color)
	return {"error":"","glyphs":glyphs,"pixels":pixels,"bounding_box_violations":0}

func _glyph_plan(pair: PackedByteArray, position: Vector2i, brightness: int) -> Dictionary:
	var key := pair.hex_encode()+":"+str(position.x)+":"+str(position.y)+":"+str(brightness)
	if _glyph_plans.has(key): return _glyph_plans[key].duplicate(true)

	var foreground: Dictionary = _font.get_glyph_image(pair,0,0,brightness)
	if foreground.get("status") != "glyph":
		return {"error":"prepared prologue glyph disappeared"}
	var pixels: Array[Dictionary] = []
	var image: Image = foreground.image
	# disp_string queues foreground then shadow into a head-inserted OT. The
	# shadow therefore draws first; source zero palette words suppress it.
	# disp_string sets the shaded shadow primitive's RGB to zero. Its CLUT
	# controls texel presence, but source modulation makes visible texels black.
	var shadow: Dictionary = _font.get_glyph_image(pair,0,0,0,true)
	if shadow.get("status") != "glyph":
		return {"error":"prepared prologue shadow glyph disappeared"}
	if shadow.foreground_word != 0:
		var shadow_image: Image = shadow.image
		for y in range(12):
			for x in range(12):
				var shadow_color: Color = shadow_image.get_pixel(x,y)
				if shadow_color.a != 0:
					pixels.append({"position":position+Vector2i(x+1,y+1),"color":shadow_color})
	for y in range(12):
		for x in range(12):
			var color: Color = image.get_pixel(x,y)
			if color.a != 0:
				pixels.append({"position":position+Vector2i(x,y),"color":color})
	var result := {"error":"","pixels":pixels}
	if _glyph_plans.size() >= MAX_GLYPH_PLANS: _glyph_plans.clear()
	_glyph_plans[key] = result.duplicate(true)
	return result

func render_accessible(target: Image, view: Dictionary, work: Dictionary, brightness: int, scale: float, backdrop: float, include_inactive: bool = false) -> Dictionary:
	# Validate the same native content/work contract before adapting presentation.
	var checked := render_into(Image.create(320, 240, false, Image.FORMAT_RGBA8), view, work, brightness)
	if not str(checked.get("error", "")).is_empty(): return checked
	var glyph_size := maxi(12, roundi(12 * clampf(scale, 1.0, 1.5)))
	var advance := glyph_size + 2
	var capacity := 300 / advance
	var rows: Array = []
	var remaining: int = work.pos
	for entry: Dictionary in work.entries:
		var raw: PackedByteArray = view.speaker_raw if entry.row == -1 else view.rows[entry.row].raw
		var visible := clampi(remaining, 0, entry.glyph_count)
		remaining -= int(entry.glyph_count)
		if not entry.active and not include_inactive: continue
		var row: Array = []
		for index: int in visible:
			row.append(raw.slice(index * 2, index * 2 + 2))
			if row.size() == capacity: rows.append(row); row = []
		if not row.is_empty(): rows.append(row)
	var first := maxi(0, rows.size() - 4)
	var line_height := glyph_size + 4
	var top := 230 - 4 * line_height
	var bounds := Rect2i(6, top - 4, 308, 4 * line_height + 8)
	for y: int in range(bounds.position.y, bounds.end.y):
		for x: int in range(bounds.position.x, bounds.end.x):
			var old := target.get_pixel(x, y)
			target.set_pixel(x, y, old.lerp(Color(0.025, 0.04, 0.1, 1.0), clampf(backdrop, 0.0, 1.0)))
	for row_index: int in range(first, rows.size()):
		for column: int in rows[row_index].size():
			var glyph: Dictionary = _font.get_glyph_image(rows[row_index][column], 0, 0, brightness)
			var image: Image = glyph.image.duplicate()
			image.resize(glyph_size, glyph_size, Image.INTERPOLATE_NEAREST)
			target.blend_rect(image, Rect2i(Vector2i.ZERO, image.get_size()), Vector2i(10 + column * advance, top + (row_index - first) * line_height))
	return {"error": "", "glyphs": checked.glyphs, "bounding_box_violations": 0}

static func _latin_codes(text: String) -> PackedByteArray:
	var result := PackedByteArray()
	var punctuation := {" ":0x8140, ",":0x8143, ".":0x8144, ":":0x8146, ";":0x8147, "?":0x8148, "!":0x8149, "'":0x8166, "’":0x8166, "\"":0x8168, "-":0x817c, "–":0x817c, "(":0x8169, ")":0x816a, "[":0x816d, "]":0x816e, "/":0x815e, "+":0x817b, "=":0x8181}
	for character: String in text:
		var unicode := character.unicode_at(0)
		var code := 0
		if unicode >= 65 and unicode <= 90: code = 0x8260 + unicode - 65
		elif unicode >= 97 and unicode <= 122: code = 0x8281 + unicode - 97
		elif unicode >= 48 and unicode <= 57: code = 0x824f + unicode - 48
		elif punctuation.has(character): code = punctuation[character]
		else: return PackedByteArray()
		result.append(code >> 8); result.append(code & 255)
	return result

static func translation_supported(translated: Dictionary) -> bool:
	var text := str(translated.get("text", ""))
	return not text.strip_edges().is_empty() and (str(translated.get("speaker", "")).is_empty() or not _latin_codes(translated.speaker).is_empty()) and not _latin_codes(text).is_empty()

func _latin_metrics(pair: PackedByteArray) -> Vector2i:
	# Translation-only spacing: retain every original texel, trim empty side
	# bearings and leave one shadow pixel plus one clear pixel between letters.
	if pair == PackedByteArray([0x81,0x40]): return Vector2i(0,4)
	var glyph: Dictionary = _font.get_glyph_image(pair,0,0,128)
	if glyph.get("status") != "glyph": return Vector2i(-1,0)
	var left := 12
	var right := -1
	for y: int in 12:
		for x: int in 12:
			if glyph.image.get_pixel(x,y).a != 0: left = mini(left,x); right = maxi(right,x)
	return Vector2i(left,right-left+3) if right >= left else Vector2i(0,4)

func translation_layout(translated: Dictionary, native_speaker: PackedByteArray) -> Array:
	if not translation_supported(translated): return []
	var original_speaker := str(translated.get("speaker", "")).is_empty()
	var speaker: PackedByteArray = native_speaker if original_speaker else _latin_codes(translated.speaker)
	var rows: Array = [[]]
	var cursor := 28 if original_speaker else TRANSLATED_LEFT
	for offset: int in range(0,speaker.size(),2):
		var pair := speaker.slice(offset,offset+2)
		var metric := Vector2i(0,14) if original_speaker else _latin_metrics(pair)
		if metric.x < 0 or cursor + metric.y > (292 if original_speaker else TRANSLATED_RIGHT): return []
		rows[0].append({"pair":pair,"x":cursor-metric.x}); cursor += metric.y
	rows.append([]); cursor = TRANSLATED_LEFT
	for word: String in str(translated.text).split(" ",false):
		var raw := _latin_codes(word)
		var width := 0
		for offset: int in range(0,raw.size(),2):
			var metric := _latin_metrics(raw.slice(offset,offset+2))
			if metric.x < 0: return []
			width += metric.y
		if cursor > TRANSLATED_LEFT and cursor + 4 + width > TRANSLATED_RIGHT: rows.append([]); cursor = TRANSLATED_LEFT
		elif cursor > TRANSLATED_LEFT: cursor += 4
		for offset: int in range(0,raw.size(),2):
			var pair := raw.slice(offset,offset+2)
			var metric := _latin_metrics(pair)
			if cursor + metric.y > TRANSLATED_RIGHT: rows.append([]); cursor = TRANSLATED_LEFT
			if rows.size() > 3: return []
			rows[-1].append({"pair":pair,"x":cursor-metric.x}); cursor += metric.y
	return rows if rows.size() <= 3 else []

func render_translation(target: Image, view: Dictionary, work: Dictionary, brightness: int, translated: Dictionary) -> Dictionary:
	var failure := preflight(view)
	if not failure.is_empty(): return {"error":failure}
	var rows := translation_layout(translated, view.speaker_raw)
	if rows.is_empty(): return {"error":"Translation does not fit the original font and three-row textbox"}
	var total := 0
	for row: Array in rows: total += row.size()
	var remaining := total if work.total <= 0 else mini(total, floori(float(work.pos) / work.total * total))
	var plan: Array[Dictionary] = []
	for line: int in rows.size():
		for glyph: Dictionary in rows[line]:
			if remaining <= 0: break
			remaining -= 1
			var item := _glyph_plan(glyph.pair, Vector2i(glyph.x,170+line*16), brightness)
			if not item.error.is_empty(): return item
			for pixel: Dictionary in item.pixels:
				if not TEXT_BOUNDS.has_point(pixel.position): return {"error":"Translated glyph exceeds the native textbox"}
			plan.append(item)
	for index: int in range(plan.size()-1,-1,-1):
		for pixel: Dictionary in plan[index].pixels: target.set_pixelv(pixel.position,pixel.color)
	return {"error":"", "glyphs":plan.size(), "bounding_box_violations":0, "rows":rows.size()}
