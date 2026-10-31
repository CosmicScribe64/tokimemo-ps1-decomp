extends RefCounted
## Renders JSON.stringify(words) for the state-hash word list
##   words = core ints + UTF-8 bytes of JSON.stringify(record) for each record
## (GameState.to_words and its overrides) without building the per-byte
## Array[int]: the text is identical, so GameSession.state_hash digests are
## unchanged. Each record's byte rendering is cached under the record's
## var_to_bytes() encoding, which is exact in value, type and order (1 and 1.0,
## or reordered keys, encode differently), so an unchanged record is never
## re-serialised and any change re-renders it. Equal encodings imply equal
## JSON text; a different encoding only costs a re-render.

static var _decimal := PackedStringArray()

var _keys: Array[PackedByteArray] = []
var _texts := PackedStringArray()

static func _byte_text(bytes: PackedByteArray) -> String:
	if _decimal.is_empty():
		for value in 256: _decimal.append(str(value))
	var parts := PackedStringArray()
	parts.resize(bytes.size())
	for index in bytes.size(): parts[index] = _decimal[bytes[index]]
	return ",".join(parts)

## core_json is JSON.stringify(core words); it must be a non-empty list.
func render(core_json: String, records: Array) -> String:
	var parts := PackedStringArray([core_json.left(-1)])
	if _keys.size() != records.size():
		_keys.resize(records.size())
		_texts.resize(records.size())
	for index in records.size():
		var key := var_to_bytes(records[index])
		if _keys[index].is_empty() or _keys[index] != key:
			var bytes := JSON.stringify(records[index]).to_utf8_buffer()
			_texts[index] = _byte_text(bytes)
			_keys[index] = key
		if not _texts[index].is_empty(): parts.append(_texts[index])
	return ",".join(parts) + "]"
