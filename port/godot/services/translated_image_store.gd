extends RefCounted
## T-39004: optional exact-source English image replacements. Null means fallback.
##
## Schema v1 holds frame rows only. Schema v2 adds a `kind` per row:
## - "frame": the v1 row (container sha, frame, palette, source PNG sha).
## - "page": a whole texture page (container sha, page index, source page PNG
##   sha, sha256 of the source page RGBA8 bytes) that replaces pixels only
##   inside its region rects.
## - "bs": an MDEC still (container sha, bs index, decoder identity, sha256 of
##   the decoded source RGBA8 bytes) that replaces pixels only inside its edit
##   regions.
## - "frame_part": one part of a multi-part HUD frame (container sha, frame,
##   palette, part index, the part's source PNG sha and `frame_parts_sha256`,
##   every part PNG sha of that frame in part order). It binds only when the
##   caller's whole frame has exactly those parts; parts without a row stay the
##   original (part_replacement).
## Both kinds compose onto the caller's source image only when that image's
## RGBA8 bytes hash to the row's recorded source; the caller's sha is a key,
## never trusted on its own. A page replacement leaves every pixel the STP mask
## marks unchanged. A bs still has no STP mask: MDEC output sets STP on every
## word, so any mask passed to bs_replacement is refused (null). Any identity
## mismatch returns null, so the caller draws the original.

const ExternalPath = preload("res://services/external_content_path.gd")
const MdecIdentity = preload("res://services/mdec_model_identity.gd")
const SCHEMA_V1 := "tokimemo.translated-image-pack.v1"
const SCHEMA_V2 := "tokimemo.translated-image-pack.v2"
const KINDS := ["frame", "page", "bs", "frame_part"]
const MAX_MANIFEST := 4 * 1024 * 1024
const MAX_IMAGE := 16 * 1024 * 1024
const MAX_ASSETS := 4096
const MAX_REGIONS := 64
const MAX_DECODED_BYTES := 256 * 1024 * 1024
const MAX_FRAME_PARTS := 64
var error: String = ""
var language: String = ""
var schema: String = ""
var _images: Dictionary = {}
var _pages: Dictionary = {}
var _stills: Dictionary = {}
var _parts: Dictionary = {}

func clear() -> void:
	_images.clear()
	_parts.clear()
	_pages.clear()
	_stills.clear()
	language = ""
	schema = ""
	error = ""

func _fail(message: String) -> bool:
	clear()
	error = message
	return false

static func _hash(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character: String in value:
		if character not in "0123456789abcdef":
			return false
	return true

static func _relative(value: Variant) -> bool:
	if not value is String or value.is_empty() or value.contains("\\") or value.contains(":") or value.begins_with("/"):
		return false
	for part: String in value.split("/"):
		if part in ["", ".", ".."]:
			return false
	return true

static func _integer(value: Variant) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and float(value) == floor(float(value))

static func _index(value: Variant) -> bool:
	return _integer(value) and int(value) >= 0 and int(value) <= 65535

static func _key(container_sha: String, frame: int, palette: String, png_sha: String) -> String:
	return container_sha + "|" + str(frame) + "|" + palette + "|" + png_sha

static func _part_key(container_sha: String, frame: int, palette: String, part_index: int, png_sha: String) -> String:
	return container_sha + "|part|" + str(frame) + "|" + palette + "|" + str(part_index) + "|" + png_sha

static func _part_shas(value: Variant) -> Array:
	# Every part PNG sha of one frame, in part order; an invalid list returns [].
	if not value is Array or value.size() < 2 or value.size() > MAX_FRAME_PARTS:
		return []
	for sha: Variant in value:
		if not _hash(sha):
			return []
	return value.duplicate()

static func _page_key(container_sha: String, page: int, png_sha: String) -> String:
	return container_sha + "|page|" + str(page) + "|" + png_sha

static func _still_key(container_sha: String, bs_index: int, decoder: String, rgba_sha: String) -> String:
	return container_sha + "|bs|" + str(bs_index) + "|" + decoder + "|" + rgba_sha

## The MDEC decoder identity that bs rows bind to: SHA-256 over the model id
## and each implementation file hash, one `name=sha` line per file in name order.
## tools/translation_assets.py decoder_identity() computes the same value.
static func decoder_identity(model_id: String, implementations: Dictionary) -> String:
	var names: Array = implementations.keys()
	names.sort()
	var text := model_id
	for name: Variant in names:
		text += "\n" + str(name) + "=" + str(implementations[name])
	return text.sha256_text()

static func current_decoder_identity() -> String:
	return decoder_identity(MdecIdentity.ID, MdecIdentity.IMPLEMENTATIONS)

static func _rgba(image: Image) -> Image:
	var copy: Image = image.duplicate()
	if copy.is_compressed() and copy.decompress() != OK:
		return null
	if copy.get_format() != Image.FORMAT_RGBA8:
		copy.convert(Image.FORMAT_RGBA8)
	return copy

static func rgba_sha256(image: Image) -> String:
	var copy := _rgba(image)
	return "" if copy == null else _data_sha256(copy)

static func _regions(value: Variant, width: int, height: int) -> Array:
	# Each region is [x, y, w, h] inside the image; an invalid list returns [].
	if not value is Array or value.is_empty() or value.size() > MAX_REGIONS:
		return []
	var result: Array = []
	for rect: Variant in value:
		if not rect is Array or rect.size() != 4:
			return []
		for item: Variant in rect:
			if not _integer(item):
				return []
		var x := int(rect[0])
		var y := int(rect[1])
		var w := int(rect[2])
		var h := int(rect[3])
		if x < 0 or y < 0 or w < 1 or h < 1 or x + w > width or y + h > height:
			return []
		result.append(Rect2i(x, y, w, h))
	return result

static func _binary_alpha(image: Image, opaque_only: bool) -> bool:
	var data := image.get_data()
	for offset: int in range(3, data.size(), 4):
		var alpha: int = data[offset]
		if alpha != 255 and (opaque_only or alpha != 0):
			return false
	return true

func open_pack(manifest_path: String, expected_sha256: String, allow_drafts: bool = false) -> bool:
	clear()
	if not _hash(expected_sha256) or not ExternalPath.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > MAX_MANIFEST or FileAccess.get_sha256(manifest_path) != expected_sha256:
		return _fail("translated image manifest path/hash is invalid")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or parsed.get("schema") not in [SCHEMA_V1, SCHEMA_V2] or parsed.get("language") != "en" or not parsed.get("assets") is Array or not parsed.get("development_drafts") is bool or not _hash(parsed.get("snapshot_sha256")):
		return _fail("unsupported translated image pack")
	var version_two: bool = parsed["schema"] == SCHEMA_V2
	if parsed["development_drafts"] and not allow_drafts:
		return _fail("draft translation pack needs explicit development opt-in")
	if parsed["assets"].size() > MAX_ASSETS:
		return _fail("translation pack exceeds asset limit")
	var frames: Dictionary = {}
	var pages: Dictionary = {}
	var stills: Dictionary = {}
	var parts: Dictionary = {}
	var decoded_bytes := 0
	for row: Variant in parsed["assets"]:
		if not row is Dictionary:
			return _fail("invalid translation source identity")
		var kind: Variant = row.get("kind", "frame") if version_two else "frame"
		if not kind is String:
			return _fail("unsupported translation record kind")
		if not _hash(row.get("source_container_sha256")) or not _hash(row.get("output_sha256")) or not _relative(row.get("output")):
			return _fail("invalid translation source identity")
		if not allow_drafts and row.get("style") != "approve":
			return _fail("replacement lacks exact current style approval")
		var size: Variant = row.get("dimensions")
		if not size is Array or size.size() != 2 or not _integer(size[0]) or not _integer(size[1]) or int(size[0]) < 1 or int(size[1]) < 1 or int(size[0]) > 4096 or int(size[1]) > 4096:
			return _fail("invalid replacement dimensions")
		var width := int(size[0])
		var height := int(size[1])
		var key := ""
		var regions: Array = []
		var frame_shas: Array = []
		if kind == "frame" or kind == "frame_part":
			if not _hash(row.get("source_png_sha256")) or not _index(row.get("frame")):
				return _fail("invalid translation source identity")
			if not row.get("palette") is String or row["palette"].is_empty() or row["palette"].contains("|"):
				return _fail("invalid translation mapping or output path")
			key = _key(row["source_container_sha256"], int(row["frame"]), row["palette"], row["source_png_sha256"])
			if kind == "frame_part":
				frame_shas = _part_shas(row.get("frame_parts_sha256"))
				if frame_shas.is_empty() or not _index(row.get("part_index")) or int(row["part_index"]) >= frame_shas.size() or frame_shas[int(row["part_index"])] != row["source_png_sha256"]:
					return _fail("invalid frame part identity")
				key = _part_key(row["source_container_sha256"], int(row["frame"]), row["palette"], int(row["part_index"]), row["source_png_sha256"])
		elif kind == "page":
			if not _hash(row.get("source_png_sha256")) or not _index(row.get("page")) or not _hash(row.get("source_rgba_sha256")):
				return _fail("invalid page source identity")
			regions = _regions(row.get("regions"), width, height)
			if regions.is_empty():
				return _fail("invalid page replacement regions")
			key = _page_key(row["source_container_sha256"], int(row["page"]), row["source_png_sha256"])
		elif kind == "bs":
			if not _index(row.get("bs_index")) or not _hash(row.get("decoder_identity_sha256")) or not _hash(row.get("source_rgba_sha256")):
				return _fail("invalid bs source identity")
			regions = _regions(row.get("regions"), width, height)
			if regions.is_empty():
				return _fail("invalid bs replacement regions")
			key = _still_key(row["source_container_sha256"], int(row["bs_index"]), row["decoder_identity_sha256"], row["source_rgba_sha256"])
		else:
			return _fail("unsupported translation record kind")
		decoded_bytes += width * height * 4
		if decoded_bytes > MAX_DECODED_BYTES:
			return _fail("translation pack exceeds decoded memory limit")
		var path := manifest_path.get_base_dir().path_join(row["output"])
		if not ExternalPath.is_external_file(path) or FileAccess.get_size(path) > MAX_IMAGE or FileAccess.get_sha256(path) != row["output_sha256"]:
			return _fail("translation PNG path/hash is invalid")
		var image := Image.new()
		if image.load_png_from_buffer(FileAccess.get_file_as_bytes(path)) != OK or image.get_width() != width or image.get_height() != height:
			return _fail("translation PNG dimensions/format differ")
		if frames.has(key) or pages.has(key) or stills.has(key) or parts.has(key):
			return _fail("duplicate translation source mapping")
		if kind == "frame":
			frames[key] = image
			continue
		if kind == "frame_part":
			parts[key] = {"image": image, "frame_parts_sha256": frame_shas}
			continue
		image = _rgba(image)
		if image == null or not _binary_alpha(image, kind == "bs"):
			return _fail("translation PNG alpha is not representable")
		if kind == "page":
			pages[key] = {"image": image, "regions": regions, "source_rgba_sha256": row["source_rgba_sha256"]}
		else:
			stills[key] = {"image": image, "regions": regions, "source_rgba_sha256": row["source_rgba_sha256"]}
	_images = frames
	_parts = parts
	_pages = pages
	_stills = stills
	language = "en"
	schema = parsed["schema"]
	return true

func replacement(container_sha: String, frame: int, palette: String, source_png_sha: String) -> Image:
	var value: Image = _images.get(_key(container_sha, frame, palette, source_png_sha))
	return value.duplicate() if value != null else null

## One part of a multi-part frame. `frame_part_shas` is the caller's PNG sha of
## every part of that frame in part order; the row binds only when it equals the
## row's list exactly. Null keeps the original part.
func part_replacement(container_sha: String, frame: int, palette: String, part_index: int, frame_part_shas: Array) -> Image:
	if part_index < 0 or part_index >= frame_part_shas.size():
		return null
	var record: Variant = _parts.get(_part_key(container_sha, frame, palette, part_index, str(frame_part_shas[part_index])))
	if record == null or record["frame_parts_sha256"] != frame_part_shas:
		return null
	return record["image"].duplicate()

static func _mask_set(mask: Image, x: int, y: int) -> bool:
	var color := mask.get_pixel(x, y)
	return color.r8 != 0 or color.g8 != 0 or color.b8 != 0

static func _data_sha256(image: Image) -> String:
	var context := HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(image.get_data())
	return context.finish().hex_encode()

## Copies the record's region pixels onto an RGBA8 copy of `source`. Refuses
## (null) unless the source RGBA8 bytes hash to the record's source identity.
static func _compose(record: Dictionary, source: Image, stp_mask: Image) -> Image:
	var replacement_image: Image = record["image"]
	if source == null or source.get_size() != replacement_image.get_size():
		return null
	if stp_mask != null and stp_mask.get_size() != source.get_size():
		return null
	var result := _rgba(source)
	if result == null or _data_sha256(result) != record["source_rgba_sha256"]:
		return null
	for rect: Rect2i in record["regions"]:
		for y: int in range(rect.position.y, rect.end.y):
			for x: int in range(rect.position.x, rect.end.x):
				if stp_mask != null and _mask_set(stp_mask, x, y):
					continue
				result.set_pixel(x, y, replacement_image.get_pixel(x, y))
	return result

static func _same_regions(expected: Array, requested: Array) -> bool:
	if requested.size() != expected.size():
		return false
	for index: int in range(requested.size()):
		var rect: Variant = requested[index]
		if rect is Array and rect.size() == 4:
			rect = Rect2i(int(rect[0]), int(rect[1]), int(rect[2]), int(rect[3]))
		if not rect is Rect2i or rect != expected[index]:
			return false
	return true

## Compose a page replacement onto `source`. `regions` (when not empty) must
## equal the pack's region rects in order; an empty list accepts the pack's.
## `source` must be the page whose RGBA8 bytes the row recorded: a page of the
## same size with other pixels (palette variant, stale cache) returns null.
## Set texels of `stp_mask` keep the source pixel.
func page_replacement(container_sha: String, page: int, source_png_sha: String, regions: Array, source: Image, stp_mask: Image = null) -> Image:
	var record: Variant = _pages.get(_page_key(container_sha, page, source_png_sha))
	if record == null or (not regions.is_empty() and not _same_regions(record["regions"], regions)):
		return null
	return _compose(record, source, stp_mask)

## Compose a bs replacement onto `source`, the caller's own decode. The decoded
## RGBA8 bytes of `source` must hash to the pack's source identity. Call it with
## four arguments: a decoded still has no STP mask (every MDEC output word sets
## STP), so a non-null `stp_mask` is a caller error and returns null rather
## than a silent copy of the Japanese source.
func bs_replacement(container_sha: String, bs_index: int, decoder_identity_sha: String, source: Image, stp_mask: Image = null) -> Image:
	if source == null or stp_mask != null:
		return null
	var record: Variant = _stills.get(_still_key(container_sha, bs_index, decoder_identity_sha, rgba_sha256(source)))
	if record == null:
		return null
	return _compose(record, source, null)

func asset_count() -> int:
	return _images.size() + _pages.size() + _stills.size() + _parts.size()

func kind_counts() -> Dictionary:
	return {"frame": _images.size(), "page": _pages.size(), "bs": _stills.size(), "frame_part": _parts.size()}
