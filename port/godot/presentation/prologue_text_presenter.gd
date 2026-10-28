extends RefCounted
## T-29025: render committed numeric text work using external source glyphs.
## Timing belongs to PrologueTextWork. Rendering never advances simulation.
const Glyphs = preload("res://presentation/native_glyphs.gd")
const TEXT_BOUNDS := Rect2i(27,166,266,52)
const ORIGIN := Vector2i(160,120)
const ADVANCE := 14
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
