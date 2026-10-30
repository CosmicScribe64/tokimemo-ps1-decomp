extends RefCounted
## T-39016: generic source-keyed English text layer. Screens call a lookup at
## the point where they decode an original string; an empty result means the
## caller keeps the original bytes. The pack is built by
## tools/port/build_text_pack.py from the approved English records.
##
## Physical keys follow the NUL-terminated record ids of
## tools/assets/text_binary.py: `<container>:<offset:08x>:<sha12>`, where sha12
## covers the source bytes through the first NUL; the builder rejects ids
## hashed over other extents. Logical ids (talk:, c:,
## dictionary:, ...) are looked up by id and, when the pack binds a container,
## by that container's SHA-256 as well. The pack never proves where a record
## is drawn; that proof stays with each consumer.

const ExternalPath = preload("res://services/external_content_path.gd")
const SCHEMA := "tokimemo.text-layer-pack.v1"
const MAX_PACK := 64 * 1024 * 1024
const MAX_ENTRIES := 131072
const MAX_TEXT := 8192
const FULL_WIDTH_SPACE := 0x8140

var error := ""
var language := ""
var manifest := ""
var digest := ""
var font_glyph_bank_sha256 := ""
## Reason for the most recent empty lookup; empty after a successful lookup.
var last_rejection := ""
var _containers: Dictionary = {}
var _entries: Dictionary = {}

func clear() -> void:
	_containers.clear()
	_entries.clear()
	language = ""
	manifest = ""
	digest = ""
	font_glyph_bank_sha256 = ""
	last_rejection = ""
	error = ""

func entry_count() -> int:
	return _entries.size()

func _fail(message: String) -> bool:
	clear()
	error = message
	return false

func open_pack(path: String, expected_sha256: String) -> bool:
	clear()
	if not _sha_text(expected_sha256) or not ExternalPath.is_external_file(path) or FileAccess.get_size(path) > MAX_PACK or FileAccess.get_sha256(path) != expected_sha256:
		return _fail("Text layer pack path/hash is invalid.")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	if not parsed is Dictionary or parsed.get("schema") != SCHEMA or not parsed.get("language") is String or parsed.language.is_empty() or parsed.language.length() > 32:
		return _fail("Text layer pack schema is unsupported.")
	if not parsed.get("approval") is Dictionary or not parsed.approval.get("approver") is String or parsed.approval.approver.is_empty():
		return _fail("Text layer pack lacks its approval record.")
	var font: Variant = parsed.get("font")
	if not font is Dictionary or font.get("source_path") != "SLPM_86.053" or not _sha_text(font.get("source_sha256")) or not _sha_text(font.get("glyph_bank_sha256")):
		return _fail("Text layer pack lacks its original font identity.")
	var containers: Variant = parsed.get("containers")
	var entries: Variant = parsed.get("entries")
	if not containers is Dictionary or not entries is Dictionary or entries.size() > MAX_ENTRIES or entries.is_empty():
		return _fail("Text layer pack tables are invalid.")
	var prepared_containers := {}
	for key: Variant in containers:
		var row: Variant = containers[key]
		if not key is String or not _container_path(key) or not row is Dictionary or not _sha_text(row.get("sha256")) or not _integer(row.get("size")) or int(row.size) <= 0:
			return _fail("Text layer container identity is invalid.")
		prepared_containers[key] = row.sha256
	var prepared := {}
	for key: Variant in entries:
		var entry: Variant = entries[key]
		var checked := _prepare(key, entry, prepared_containers)
		if checked.is_empty():
			return _fail("Text layer entry is invalid: " + str(key).left(96))
		prepared[key] = checked
	_containers = prepared_containers
	_entries = prepared
	language = parsed.language
	font_glyph_bank_sha256 = font.glyph_bank_sha256
	manifest = path
	digest = expected_sha256
	return true

func _prepare(key: Variant, entry: Variant, containers: Dictionary) -> Dictionary:
	if not key is String or key.is_empty() or key.length() > 512 or not entry is Dictionary:
		return {}
	if not entry.get("text") is String or entry.text.is_empty() or entry.text.length() > MAX_TEXT or not entry.get("context_status") is String or not entry.get("segments") is Array or entry.segments.is_empty():
		return {}
	var result := {"id": key, "text": entry.text, "context_status": entry.context_status, "segments": [], "container": "", "offset": -1, "raw_sha256": "", "raw_length": 0}
	if entry.has("container"):
		if not entry.container is String or not containers.has(entry.container):
			return {}
		result.container = entry.container
	if entry.has("offset") or entry.has("raw_sha256") or entry.has("raw_length"):
		if result.container.is_empty() or not _integer(entry.get("offset")) or int(entry.offset) < 0 or not _sha_text(entry.get("raw_sha256")) or not _integer(entry.get("raw_length")) or int(entry.raw_length) < 1:
			return {}
		if key != source_id_from_digest(result.container, int(entry.offset), entry.raw_sha256):
			return {}
		result.offset = int(entry.offset)
		result.raw_sha256 = entry.raw_sha256
		result.raw_length = int(entry.raw_length)
	var glyphs := 0
	for segment: Variant in entry.segments:
		if not segment is Array or segment.is_empty():
			return {}
		if segment[0] == "g" and segment.size() == 2 and _hex_units(segment[1], 2):
			var codes: PackedByteArray = str(segment[1]).hex_decode()
			for offset: int in range(0, codes.size(), 2):
				if codes[offset] < 0x81 or codes[offset + 1] < 0x40:
					return {}
			result.segments.append({"kind": "glyphs", "codes": codes})
			glyphs += 1
		elif segment[0] == "t" and segment.size() == 3 and _hex_units(segment[1], 1) and segment[2] is String:
			result.segments.append({"kind": "token", "raw": str(segment[1]).hex_decode(), "token_kind": segment[2]})
		else:
			return {}
	return result if glyphs > 0 else {}

## Record id of a physical source string. `text_bytes` may stop before or at
## its terminator; the hash always covers the bytes through the first NUL.
static func source_id(container: String, offset: int, text_bytes: PackedByteArray) -> String:
	return source_id_from_digest(container, offset, _sha(terminated(text_bytes)))

static func source_id_from_digest(container: String, offset: int, raw_sha256: String) -> String:
	return "%s:%08x:%s" % [container, offset, raw_sha256.left(12)]

static func terminated(text_bytes: PackedByteArray) -> PackedByteArray:
	var end := text_bytes.find(0)
	if end >= 0:
		return text_bytes.slice(0, end + 1)
	var result := text_bytes.duplicate()
	result.append(0)
	return result

## Decode-time lookup for a string read from `container` at file `offset`.
## Every identity (id, container hash, offset, full source hash and length)
## must agree, and every glyph must exist in `font` (a NativeGlyphs instance
## opened by the caller); otherwise the result is empty.
func lookup_source(container: String, container_sha256: String, offset: int, text_bytes: PackedByteArray, font: RefCounted) -> Dictionary:
	var raw := terminated(text_bytes)
	var entry: Dictionary = _entries.get(source_id(container, offset, raw), {})
	if entry.is_empty():
		return _reject("no approved record for this source location")
	if entry.offset != offset or entry.container != container or entry.raw_length != raw.size() or entry.raw_sha256 != _sha(raw):
		return _reject("source bytes differ from the record identity")
	return _checked(entry, container_sha256, font)

## Lookup by logical record id. A record bound to a container also requires
## the caller's container hash.
func lookup_record(record_id: String, container_sha256: String, font: RefCounted) -> Dictionary:
	var entry: Dictionary = _entries.get(record_id, {})
	if entry.is_empty():
		return _reject("no approved record for this id")
	return _checked(entry, container_sha256, font)

func _checked(entry: Dictionary, container_sha256: String, font: RefCounted) -> Dictionary:
	if not entry.container.is_empty() and _containers.get(entry.container, "") != container_sha256:
		return _reject("container hash differs from the pack")
	if font == null:
		return _reject("no original font is open")
	var codes := PackedByteArray()
	var tokens := false
	for segment: Dictionary in entry.segments:
		if segment.kind == "token":
			tokens = true
			continue
		for index: int in range(0, segment.codes.size(), 2):
			var pair: PackedByteArray = segment.codes.slice(index, index + 2)
			var glyph: Dictionary = font.get_glyph(pair)
			if glyph.get("status") != "glyph" or ((pair[0] << 8 | pair[1]) != FULL_WIDTH_SPACE and not PackedByteArray(glyph.get("pixels", PackedByteArray())).has(1)):
				return _reject("translated text needs a glyph the original font lacks")
		codes.append_array(segment.codes)
	var result := {"id": entry.id, "text": entry.text, "context_status": entry.context_status, "segments": entry.segments.duplicate(true)}
	if not tokens:
		result["codes"] = codes
	last_rejection = ""
	return result

func _reject(reason: String) -> Dictionary:
	last_rejection = reason
	return {}

static func _container_path(value: String) -> bool:
	if value == "SLPM_86.053":
		return true
	if not value.begins_with("CDROM/") or value.contains("..") or value.contains("\\") or value.contains(":"):
		return false
	for part: String in value.split("/"):
		if part.is_empty():
			return false
	return true

static func _integer(value: Variant) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and float(value) == floor(float(value))

static func _sha_text(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character: String in value:
		if character not in "0123456789abcdef":
			return false
	return true

static func _hex_units(value: Variant, unit: int) -> bool:
	if not value is String or value.is_empty() or value.length() % (unit * 2) != 0:
		return false
	for character: String in value:
		if character not in "0123456789abcdef":
			return false
	return true

static func _sha(data: PackedByteArray) -> String:
	var hashing := HashingContext.new()
	hashing.start(HashingContext.HASH_SHA256)
	hashing.update(data)
	return hashing.finish().hex_encode()
