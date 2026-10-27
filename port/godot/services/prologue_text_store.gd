extends RefCounted
## Authenticated original prologue text views. No timing, audio or state writes.
const ExternalPath = preload("res://services/external_content_path.gd")
const PAYLOAD_SHA := "40428c547eb89f2c1b0363bfdcc9cf3e4e806bc878bc4a6317e312ccaff86855"
const MAX_BYTES := 2 * 1024 * 1024
var _bundle: Dictionary = {}

func open(root: String, manifest_sha: String) -> String:
	_bundle.clear()
	if not _safe(root) or not _sha_valid(manifest_sha): return "prologue root or manifest identity is invalid"
	var manifest_path := root.path_join("manifest.json")
	if not _safe(manifest_path) or not FileAccess.file_exists(manifest_path) or FileAccess.get_size(manifest_path) > 4096 or FileAccess.get_sha256(manifest_path) != manifest_sha:
		return "prologue manifest is missing, linked, oversized or changed"
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not manifest is Dictionary or manifest.size() != 6 or manifest.get("schema") != "tokimemo.prologue-text-pack.v1" or manifest.get("payload") != "prologue.json" or manifest.get("sha256") != PAYLOAD_SHA or manifest.get("line_count") != 43 or manifest.get("runtime_execution_proven") != false:
		return "prologue manifest fields differ from the reviewed contract"
	var payload_path := root.path_join("prologue.json")
	if not _safe(payload_path) or not FileAccess.file_exists(payload_path): return "prologue payload is missing or linked"
	var file := FileAccess.open(payload_path,FileAccess.READ)
	if file == null: return "prologue payload cannot be read"
	var length := file.get_length()
	if length <= 0 or length > MAX_BYTES or manifest.get("size") != length:
		file.close(); return "prologue payload size differs"
	var raw := file.get_buffer(length)
	var complete := raw.size() == length and file.get_length() == length
	file.close()
	if not complete or not _safe(payload_path) or _sha(raw) != PAYLOAD_SHA: return "prologue payload identity differs"
	var parsed: Variant = JSON.parse_string(raw.get_string_from_utf8())
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.prologue-text.v1" or parsed.get("version") != 1 or not parsed.get("lines") is Array or parsed.lines.size() != 43:
		return "prologue data schema differs"
	for i in 43:
		if parsed.lines[i].get("line_id") != i: return "prologue line IDs differ"
	_bundle = parsed
	return ""

func initial_constants() -> Dictionary:
	if _bundle.is_empty(): return {}
	return {"initial_mascot":int(_bundle.initial_mascot.value)}

func resolve(line_id: int, state: Dictionary) -> Dictionary:
	if _bundle.is_empty(): return _failure("prologue text is not open")
	if line_id not in range(43) and line_id not in [100,101]: return _failure("prologue line ID is unsupported")
	var speaker := 15
	var raw := PackedByteArray()
	var source_tokens: Array = []
	var clip1: Variant = null
	var clip2: Variant = null
	if line_id < 43:
		var source: Dictionary = _bundle.lines[line_id]
		speaker = int(source.speaker)
		raw = _hex(str(source.storage.text.raw_hex))
		source_tokens = source.storage.text.tokens.duplicate(true)
		clip1 = _integers(source.clip1); clip2 = _integers(source.clip2)
		for token: Dictionary in source_tokens:
			if token.kind not in ["source_character","row_separator","substitution","conditional_substitution"]:
				return _failure("prologue source token is unsupported")
	else:
		var built := _dynamic(line_id,state)
		if not built.error.is_empty(): return built
		raw = built.raw
	var label := _speaker(speaker,state)
	if not label.error.is_empty(): return label
	var split := _split_rows(raw)
	if split.size() > 6: return _failure("prologue row splitter capacity is unproven")
	var rows: Array = []
	var all_tokens: Array = []
	var display_raw := PackedByteArray()
	for row_index in split.size():
		var expanded := _expand(split[row_index],speaker,state)
		if not expanded.error.is_empty(): return expanded
		var row_raw: PackedByteArray = expanded.raw
		var prefix := "none"
		if row_index == 0:
			prefix = "player_quote_open" if speaker == 15 else "parenthesis_open" if speaker == 31 else "none" if speaker == 47 else "girl_quote_open"
		if prefix != "none":
			if not _bundle.get("renderer_pieces",{}).has(prefix): return _failure("prologue decoration source piece is unavailable")
			var decorated := _piece(_bundle.renderer_pieces[prefix]); decorated.append_array(row_raw); row_raw = decorated
		var tokenized := _glyph_tokens(row_raw,row_index)
		if not tokenized.error.is_empty(): return tokenized
		if row_index > 0:
			display_raw.append(32)
			all_tokens.append({"kind":"row_separator","raw_hex":"20","row":row_index-1})
		display_raw.append_array(row_raw); all_tokens.append_array(tokenized.tokens)
		rows.append({"row":row_index,"raw":row_raw,"raw_hex":row_raw.hex_encode(),"tokens":tokenized.tokens,
			"screen_x":-128 if speaker == 255 else -86 if row_index == 0 else -72,
			"screen_y":50 if row_index == 0 else 66 if row_index == 1 else 82,"prefix":prefix})
	return {"error":"","line_id":line_id,"speaker":speaker,"speaker_raw":label.raw,"speaker_hex":label.raw.hex_encode(),
		"raw":display_raw,"raw_hex":display_raw.hex_encode(),"tokens":all_tokens,"rows":rows,
		"source_raw":raw,"source_tokens":source_tokens,"clip1":clip1,"clip2":clip2,
		"audio_request":line_id < 43 and (speaker&15) != 15,"runtime_execution_proven":false}

func _dynamic(line_id: int, state: Dictionary) -> Dictionary:
	if not state.get("girls") is Array or state.girls.is_empty() or not state.girls[0] is Dictionary or typeof(state.girls[0].get("birthday")) != TYPE_INT:
		return _failure("retained birthday state is unavailable")
	var word: int = state.girls[0].birthday
	if word < 0 or word > 0xffffffff: return _failure("retained birthday word is invalid")
	var pieces: Dictionary = _bundle.dynamic_pieces
	var raw := PackedByteArray()
	if line_id == 100:
		var month := word&15; var day := (word>>4)&31
		if month < 1 or month > 12 or day < 1 or day > 31: return _failure("native birthday fields are invalid")
		raw.append_array(_piece(pieces.birthday_numeric_rows[month]))
		raw.append_array(_piece(pieces.month_suffix).slice(0,2))
		if day >= 10: raw.append_array(_piece(pieces.birthday_numeric_rows[day/10]).slice(0,2))
		raw.append_array(_piece(pieces.birthday_numeric_rows[day%10]).slice(0,2))
		raw.append_array(_piece(pieces.day_suffix).slice(0,4))
	else:
		raw.append_array(_piece(pieces.blood_rows[(word>>9)&3]))
		raw.append_array(_piece(pieces.blood_suffix).slice(0,4))
	return {"error":"","raw":raw}

func _speaker(speaker: int, state: Dictionary) -> Dictionary:
	if (speaker&15) == 15: return _name(state,"profile_family_bytes",8)
	if speaker in [30,46]: return {"error":"","raw":_piece(_bundle.speaker_labels[str(speaker)])}
	if speaker < 0 or speaker >= 16: return _failure("speaker selector is unsupported")
	return {"error":"","raw":_piece(_bundle.speaker_labels.girl_short_name_rows[speaker])}

func _expand(raw: PackedByteArray, speaker: int, state: Dictionary) -> Dictionary:
	if raw.size() >= 64: return _failure("native bounded text copy has no proven terminator")
	var expanded := raw.duplicate()
	for i in expanded.size():
		if expanded[i] == 80 and i%2 == 0:
			if (speaker&15) != 15: return _failure("current-girl player-name form is unsupported")
			var family := _name(state,"profile_family_bytes",8)
			var given := _name(state,"profile_given_bytes",8)
			if not family.error.is_empty(): return family
			if not given.error.is_empty(): return given
			var name: PackedByteArray = family.raw.duplicate(); name.append_array(given.raw.slice(0,6))
			var out := expanded.slice(0,i); out.append_array(name); out.append_array(expanded.slice(i+1)); expanded = out
			break
	if (speaker&15) == 15:
		for i in expanded.size():
			if expanded[i] == 71 and i%2 == 0: return _failure("current-girl name substitution is unsupported")
	return {"error":"","raw":expanded}

static func _split_rows(raw: PackedByteArray) -> Array:
	var rows: Array = []; var start := 0; var parity := 0
	for i in raw.size():
		if raw[i] == 32 and parity%2 == 0:
			rows.append(raw.slice(start,i)); start = i+1; parity = -1
		elif raw[i] in [80,71] and parity%2 == 0: parity = -1
		parity += 1
	rows.append(raw.slice(start)); return rows

static func _name(state: Dictionary, key: String, width: int) -> Dictionary:
	if not state.get(key) is Array or state[key].size() != width: return _failure("retained player name is unavailable")
	var raw := PackedByteArray()
	var ended := false
	for value: Variant in state[key]:
		if typeof(value) != TYPE_INT or value < 0 or value > 255: return _failure("retained name byte is invalid")
		if value == 0: ended = true
		if not ended: raw.append(value)
	if not ended or raw.size()%2 != 0: return _failure("retained name lacks a complete terminated code-unit extent")
	var valid := _glyph_tokens(raw,-1)
	if not valid.error.is_empty(): return valid
	return {"error":"","raw":raw}

static func _glyph_tokens(raw: PackedByteArray, row: int) -> Dictionary:
	if raw.size()%2 != 0: return _failure("resolved text has an unsupported terminal byte")
	var tokens: Array = []
	for i in range(0,raw.size(),2):
		if raw[i] == 0 or raw[i+1] == 0: return _failure("resolved text has an embedded terminator")
		if not ((raw[i] >= 0x81 and raw[i] <= 0x9f) or (raw[i] >= 0xe0 and raw[i] <= 0xfc)) or raw[i+1] < 0x40 or raw[i+1] > 0xfc or raw[i+1] == 0x7f:
			return _failure("resolved text has an unsupported double-byte code unit")
		tokens.append({"kind":"glyph","row":row,"offset":i,"raw_hex":raw.slice(i,i+2).hex_encode()})
	return {"error":"","tokens":tokens}

static func _piece(value: Dictionary) -> PackedByteArray: return _hex(str(value.text.raw_hex))
static func _integers(values: Array) -> Array:
	var result: Array = []
	for value: Variant in values: result.append(int(value))
	return result
static func _hex(value: String) -> PackedByteArray: return value.hex_decode()
static func _sha(raw: PackedByteArray) -> String:
	var hash := HashingContext.new(); hash.start(HashingContext.HASH_SHA256); hash.update(raw); return hash.finish().hex_encode()
static func _sha_valid(value: String) -> bool:
	if value.length() != 64: return false
	for ch in value:
		if ch not in "0123456789abcdef": return false
	return true
static func _safe(path: String) -> bool:
	return ExternalPath.is_external_path(path)
static func _failure(message: String) -> Dictionary: return {"error":message}
