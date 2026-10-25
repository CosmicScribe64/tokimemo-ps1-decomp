extends RefCounted
## T-29012: boot_game_data_init and NAME_ENT profile commit, as byte stores.
## The result is an owned-state projection, not a command-ready game session.
const Rng = preload("res://core/native_rng.gd")
const SIZE: int = 0x1a8c
const UNSET: int = 255

class Outcome extends RefCounted:
	var error: String = ""
	var bytes: PackedByteArray = PackedByteArray()
	var selections: Array = []
	var rng_draws: int = 0

static func _zero(data: PackedByteArray, start: int, count: int) -> void:
	for i in count: data[start + i] = 0

static func _birthday(data: PackedByteArray, offset: int, month: int, day: int, blood: int) -> void:
	data.encode_u32(offset, (data.decode_u32(offset) & 0xfffff800) | (month & 15) | ((day & 31) << 4) | ((blood & 3) << 9))

static func boot_data(previous: PackedByteArray, tables: Dictionary) -> Outcome:
	var out := Outcome.new()
	if previous.size() != SIZE or not _content_valid(tables):
		out.error = "boot_game_data_init requires prior GameState and validated original character initialization data"
		return out
	var data: PackedByteArray = previous.duplicate()
	# boot_calendar_init. Tuesday is native enum 1.
	for pair in [[0x3d,0],[0x3e,95],[0x3f,4],[0x40,4],[0x41,1]]: data[pair[0]] = pair[1]
	data.encode_u16(0x42, 0)
	_zero(data, 0x44, 0x80)
	_zero(data, 0x66c, 0x30)
	_zero(data, 0xf64, 4)
	data[0x44] = 30
	data[0x64] = 5
	for day in [2,9,16,23,30]: data[0x44 + day] = 1
	data[0x44 + 29] = 2
	data[0x64 + 4] = 1
	data[0x84] = 31
	for day in [7,14,21,28]: data[0x84 + day] = 1
	for day in [3,4,5]: data[0x84 + day] = 2
	# boot_player_init clears the whole profile/parameter block before stores.
	_zero(data, 0xc4, 0x7c)
	data[0xf5] = 0xb0
	_birthday(data, 0xf8, 1, 1, 1)
	_zero(data, 0x135, 9)
	data.encode_u16(0xfe,100)
	for offset in [0x102,0x106,0x10a,0x10e]: data.encode_u16(offset,40)
	data.encode_u16(0x112,32)
	data.encode_u16(0x116,60)
	data.encode_u16(0x11a,5)
	for i in 21: data[0x120 + i] = 1 if i in [0,2,4,8,9,12,15] else 0
	for i in 0x7c: data[0x140 + i] = data[0xc4 + i]
	# boot_girls_init. Fields not assigned remain zero in all sixteen records.
	_zero(data, 0x1bc, 0x380)
	for i in 16:
		var base: int = 0x1bc + i * 0x38
		data.encode_u16(base + 6, 50)
		data[base + 14] = 0xb0
	data[0x1c8] = 0xca
	data[0x1c8 + 12 * 0x38] = 0x0a
	for i in 14: _birthday(data, 0x1cc + i * 0x38, tables.girl_birthdays[i*2], tables.girl_birthdays[i*2+1], 0)
	# boot_event_state_init includes saved_settings in its exact 0x100-byte clear.
	for span in [[0x54c,0x20],[0x66c,0x30],[0x69c,0x80],[0x56c,0x100],[0x724,4],[0x728,4],[0x72c,16],[0x75e,0x800]]:
		_zero(data, span[0], span[1])
	for offset in [0x71c,0x75d,0x75c,0xf5e]: data[offset] = 0
	out.bytes = data
	return out

static func _selection_valid(value: Variant, count: int) -> bool:
	return typeof(value) == TYPE_INT and (value == UNSET or (value >= 0 and value < count))

static func _profile_valid(profile: Dictionary) -> bool:
	return profile.size() == 3 and _selection_valid(profile.get("month"),12) and _selection_valid(profile.get("day"),31) and _selection_valid(profile.get("blood"),4)

static func finalize_birthday(profile: Dictionary, rng: Rng, caller: String) -> Outcome:
	var out := Outcome.new()
	if not _profile_valid(profile) or not rng.configuration_error().is_empty():
		out.error = "birthday selections or native RNG configuration invalid"
		return out
	var values: Array = [profile.month,profile.day,profile.blood]
	for i in 3:
		if values[i] == UNSET:
			values[i] = rng.next(caller + ":" + ["month","day","blood"][i]) % [12,31,3][i]
			out.rng_draws += 1
	if values[0] == 1 and values[1] >= 28: values[1] = 27
	if values[0] in [3,5,8,10] and values[1] >= 30: values[1] = 29
	out.selections = values
	return out

static func _compact(source: PackedByteArray, limit: int, width: int, space: PackedByteArray) -> PackedByteArray:
	var result := PackedByteArray()
	for i in range(0,limit,2):
		if (source[i] != space[0] or source[i+1] != space[1]) and (source[i] != 0 or source[i+1] != 0):
			result.append(source[i]); result.append(source[i+1])
	result.resize(width)
	return result

static func _c_string(source: PackedByteArray) -> PackedByteArray:
	var result := PackedByteArray()
	for value in source:
		if value == 0: break
		result.append(value)
	return result

static func _copy(data: PackedByteArray, offset: int, values: PackedByteArray) -> void:
	for i in values.size(): data[offset+i] = values[i]

static func _content_valid(tables: Dictionary) -> bool:
	# The adapter validates original identities before constructing this contract.
	if tables.size() != 7: return false
	for field in ["compatibility","initial_scores","space","cheat_names","girl_birthdays","girl_bloods","girl_fixed_clubs"]:
		if not tables.has(field): return false
	for field in ["compatibility","initial_scores","space","girl_birthdays","girl_bloods","girl_fixed_clubs"]:
		if not tables[field] is Array: return false
		for value in tables[field]:
			if typeof(value) != TYPE_INT or value < 0 or value > 255: return false
	if tables.compatibility.size() != 16 or tables.initial_scores.size() != 20 or tables.space.size() != 2: return false
	if tables.girl_birthdays.size() != 28 or tables.girl_bloods.size() != 12: return false
	for i in 14:
		if tables.girl_birthdays[i*2] < 1 or tables.girl_birthdays[i*2] > 12 or tables.girl_birthdays[i*2+1] < 1 or tables.girl_birthdays[i*2+1] > 31: return false
	for blood in tables.girl_bloods:
		if blood > 3: return false
	if tables.girl_fixed_clubs.size() != 7: return false
	for club in tables.girl_fixed_clubs:
		if club > 11: return false
	for value in tables.compatibility:
		if value < 1 or value > 6: return false
	if not tables.cheat_names is Array or tables.cheat_names.size() != 9: return false
	for name in tables.cheat_names:
		if not name is Array or name.size() > 16: return false
		for value in name:
			if typeof(value) != TYPE_INT or value < 1 or value > 255: return false
	return true

static func setup(previous: PackedByteArray, names: Dictionary, player: Dictionary, shiori: Dictionary, tables: Dictionary, rng: Rng) -> Outcome:
	var out := Outcome.new()
	if previous.size() != SIZE or not _profile_valid(player) or not _profile_valid(shiori) or not _content_valid(tables) or not rng.configuration_error().is_empty():
		out.error = "New Game requires prior state, profiles, original tables and configured RNG"
		return out
	if names.size() != 3:
		out.error = "native name buffers are required"
		return out
	for field in ["family","given","nickname"]:
		if not names.get(field) is PackedByteArray or names[field].size() != (10 if field == "nickname" else 6):
			out.error = "native name buffers must be exactly 6/6/10 bytes"
			return out
	var checkpoint: Dictionary = rng.checkpoint()
	var p: Outcome = finalize_birthday(player,rng,"name_entry_birthday_finalize:player")
	var s: Outcome = finalize_birthday(shiori,rng,"name_entry_birthday_finalize:shiori")
	var data: PackedByteArray = boot_data(previous,tables).bytes
	var family: PackedByteArray = _compact(names.family,6,8,PackedByteArray(tables.space))
	var given: PackedByteArray = _compact(names.given,6,8,PackedByteArray(tables.space))
	var nickname: PackedByteArray = _compact(names.nickname,10,16,PackedByteArray(tables.space))
	_copy(data,0xd4,family); _copy(data,0xdc,given); _copy(data,0xe4,nickname)
	var full: PackedByteArray = _c_string(family) + _c_string(given)
	full.resize(16)
	_copy(data,0xc4,full)
	_birthday(data,0xf8,p.selections[0]+1,p.selections[1]+1,(p.selections[2]+1)&3)
	_birthday(data,0x1cc,s.selections[0]+1,s.selections[1]+1,(s.selections[2]+1)&3)
	var club: int = ((s.selections[1]+1)*3+s.selections[0]+1)%11
	data[0x1ca] = club << 4
	var compatibility: int = tables.compatibility[((s.selections[2]+1)&3) + ((p.selections[2]+1)&3)*4]-1
	data.encode_u16(0x1c2,tables.initial_scores[compatibility*3])
	data.encode_u16(0x1be,tables.initial_scores[compatibility*3+1])
	data.encode_u16(0x1c6,tables.initial_scores[compatibility*3+2])
	for girl in range(1,5):
		var low: int = (girl-1)*2
		var assigned: int
		if club == low: assigned = low+1
		elif club == low+1: assigned = low
		else: assigned = low+1 if (rng.next("name_entry_new_game_setup:girl"+str(girl))&1) else low
		data[0x1ca+girl*0x38] = assigned << 4
	for i in 7: data[0x1ca+(i+5)*0x38] = int(tables.girl_fixed_clubs[i])<<4
	for i in 12:
		var offset: int = 0x1cc+(i+1)*0x38
		data.encode_u32(offset,(data.decode_u32(offset)&~0x600)|(int(tables.girl_bloods[i])<<9))
	# name_entry_cheat_names_apply compares raw UI buffers with original strings.
	var cheats: Array = tables.cheat_names
	if _c_string(names.nickname) == PackedByteArray(cheats[0]):
		for i in 9: data.encode_u16(0xfe+i*4,573)
	if _c_string(names.family) == PackedByteArray(cheats[1]) and _c_string(names.given) == PackedByteArray(cheats[2]):
		data.encode_u16(0x1be,32); data.encode_u16(0x1c2,64)
		for i in range(1,9):
			if data[0x1ca+i*0x38]>>4 != club:
				data.encode_u16(0x1be+i*0x38,127); data.encode_u16(0x1c2+i*0x38,127)
				data[0x1c8+i*0x38] |= 10
	for group in 2:
		var start: int = 3+group*3
		if _c_string(names.family) == PackedByteArray(cheats[start]) and _c_string(names.given) == PackedByteArray(cheats[start+1]) and _c_string(names.nickname) == PackedByteArray(cheats[start+2]):
			for i in 9: data.encode_u16(0xfe+i*4,0 if group == 0 else 999)
	if not rng.error.is_empty():
		rng.restore(checkpoint)
		out.error = "native RNG failure during New Game"
		return out
	out.bytes = data
	out.selections = [p.selections,s.selections]
	out.rng_draws = rng.draw_count-int(checkpoint.draw_count)
	return out

const MAX_MANIFEST_BYTES: int = 65536

static func _external_manifest_path_valid(path: String) -> bool:
	# The current original-input runtime supports canonical absolute POSIX paths.
	# Inspect every component before opening a file, including the leaf itself.
	if not path.begins_with("/") or path.begins_with("//") or path.contains("\\") or path.contains(":") or path.simplify_path() != path: return false
	var project: String = ProjectSettings.globalize_path("res://").trim_suffix("/")
	if path == project or path.begins_with(project+"/"): return false
	var root: DirAccess = DirAccess.open("/")
	if root == null: return false
	var current: String = ""
	for component in path.split("/",false):
		if component == "." or component == "..": return false
		current += "/"+component
		if root.is_link(current): return false
	return FileAccess.file_exists(path)

static func load_tables(path: String) -> Dictionary:
	# Payload digest pins authenticate the original values after JSON decoding.
	# Resource packs may contain data only; this loader never executes pack code.
	if not _external_manifest_path_valid(path): return {}
	var file: FileAccess = FileAccess.open(path,FileAccess.READ)
	if file == null: return {}
	var length: int = file.get_length()
	if length <= 0 or length > MAX_MANIFEST_BYTES: return {}
	var raw_json: PackedByteArray = file.get_buffer(length)
	if raw_json.size() != length or file.get_length() != length or not _external_manifest_path_valid(path): return {}
	var bundle: Variant = JSON.parse_string(raw_json.get_string_from_utf8())
	if not bundle is Dictionary or bundle.get("schema") != "tokimemo.new-game-tables.v1": return {}
	if not bundle.get("proof") is Dictionary or bundle.proof.get("overlay_sha256") != "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2": return {}
	if not bundle.get("tables") is Dictionary: return {}
	var tables: Dictionary = bundle.tables
	if tables.size() != 7: return {}
	var pins: Dictionary = {
		"girl_birthdays":"39a0cacfc6eba63c61497c2f8c12778d495cdc38a776fea6a53fde5404602f94",
		"girl_bloods":"cc5df113b389158e8294fffb81104648bb6aee44d332b4107462786dbc8df240",
		"girl_fixed_clubs":"e28e3af87748164d486c21cd6841e9edec9359dd9eeff8517bc1297b287ad5e7",
		"compatibility":"15f30eae0f0c6ee69a5c9545421e46b9462f9326d503b8d12a6d9fbded1fe6e4",
		"initial_scores":"9c19e50ed151d648396d4f7eb81df29aa1730a38ab152b5ce704b85b7925048f",
		"space":"5845a3f69ea59cfc12219a5272b8be0b9f9bb60648ed002627e6637041b1bff3",
		"cheat_names":"1bb4c75cceaa83089bdb6cfde8b29ec3684c54360663b8c17af6832a53ede05b"}
	for key in pins:
		if not tables.get(key) is Array: return {}
		var rows: Array = tables[key] if key == "cheat_names" else [tables[key]]
		var raw := PackedByteArray()
		for row in rows:
			if not row is Array: return {}
			for i in row.size():
				if typeof(row[i]) != TYPE_FLOAT and typeof(row[i]) != TYPE_INT: return {}
				if row[i] < 0 or row[i] > 255 or float(row[i]) != float(int(row[i])): return {}
				row[i] = int(row[i]); raw.append(row[i])
			if key == "cheat_names": raw.append(0)
		var hash := HashingContext.new()
		hash.start(HashingContext.HASH_SHA256); hash.update(raw)
		if hash.finish().hex_encode() != pins[key]: return {}
	return tables if _content_valid(tables) else {}
