extends RefCounted
## T-29025: bounded extraction from authenticated original inputs only.
## No repository/source/cache data is consulted by this runtime adapter.
const Rules = preload("res://rules/new_game.gd")
const Entry = preload("res://flow/new_game_entry.gd")
const Glyphs = preload("res://presentation/native_glyphs.gd")
const EXE_SHA := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const OVERLAY_SHA := "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2"
const BASE := 0x80132000
const FONT_ROOT := ".asset-cache/port/fonts/native-font-v2"
const CHEAT_OFFSETS := [103824,103836,103844,103852,103860,103868,103880,103888,103896]

static func _sha(raw: PackedByteArray) -> String:
	var h := HashingContext.new()
	h.start(HashingContext.HASH_SHA256); h.update(raw)
	return h.finish().hex_encode()

# Python json.dumps(sort_keys=True,separators=(',',':')) compatibility for the
# integer/ASCII-only descriptors emitted here. Reject all unsupported values.
static func canonical_json(value: Variant) -> String:
	match typeof(value):
		TYPE_DICTIONARY:
			var keys: Array = []
			for original_key: Variant in value:
				if typeof(original_key) not in [TYPE_STRING,TYPE_STRING_NAME]: return ""
				keys.append(str(original_key))
			keys.sort()
			var parts: Array[String] = []
			for key: Variant in keys:
				if typeof(key) not in [TYPE_STRING,TYPE_STRING_NAME]: return ""
				var encoded := canonical_json(value[key])
				if encoded.is_empty(): return ""
				parts.append(JSON.stringify(str(key)) + ":" + encoded)
			return "{" + ",".join(parts) + "}"
		TYPE_ARRAY:
			var parts: Array[String] = []
			for item: Variant in value:
				var encoded := canonical_json(item)
				if encoded.is_empty(): return ""
				parts.append(encoded)
			return "[" + ",".join(parts) + "]"
		TYPE_STRING:
			for i in value.length():
				if value.unicode_at(i) > 127: return ""
			return JSON.stringify(value)
		TYPE_INT: return str(value)
		TYPE_BOOL: return "true" if value else "false"
		TYPE_NIL: return "null"
	return ""

static func _bytes(raw: PackedByteArray, start: int, length: int) -> Array:
	var result: Array = []
	for i in range(start,start + length): result.append(int(raw[i]))
	return result

static func _cstring(raw: PackedByteArray, start: int, limit: int) -> Array:
	if start < 0 or start >= raw.size(): return []
	for end in range(start,mini(raw.size(),start + limit)):
		if raw[end] == 0: return _bytes(raw,start,end-start)
	return []

# Port of new_game_tables._field_stores. This is bounded symbolic decoding,
# not execution: only unique load/mask/store witnesses are consumed below.
static func _field_stores(raw: PackedByteArray, base: int, start: int, end: int) -> Array:
	var regs: Array = []; regs.resize(32); regs[0] = ["constant",0]
	var stores: Array = []; var pending_call := 0
	for pc in range(start,end,4):
		var word := raw.decode_u32(pc-base)
		var op := word >> 26; var rs := (word >> 21)&31; var rt := (word >> 16)&31
		var imm := word&65535; var signed_imm := imm-65536 if imm&32768 else imm
		var left: Variant = regs[rs]
		if op == 15: regs[rt] = ["constant",imm<<16]
		elif op == 9:
			regs[rt] = ["constant",(left[1]+signed_imm)&0xffffffff] if left != null and left[0] == "constant" else null
		elif op in [36,37]:
			regs[rt] = ["field",(left[1]+signed_imm)&0xffffffff,255 if op==36 else 65535,0,pc] if left != null and left[0] == "constant" else null
		elif op in [12,13]:
			if left == null: regs[rt] = null
			elif left[0] == "constant": regs[rt] = ["constant",left[1]&imm if op==12 else left[1]|imm]
			elif op == 12: regs[rt] = ["field",left[1],left[2]&imm,left[3]&imm,left[4]]
			else: regs[rt] = ["field",left[1],left[2]&~imm,left[3]|imm,left[4]]
		elif op in [40,41]:
			var value: Variant = regs[rt]
			if left != null and left[0] == "constant" and value != null and value[0] == "field" and ((left[1]+signed_imm)&0xffffffff) == value[1]:
				stores.append({"address":value[1],"mask":value[2],"value":value[3],"width":1 if op==40 else 2,"load_pc":value[4],"store_pc":pc})
		elif op == 0:
			var rd := (word>>11)&31
			if (word&63) in [33,37] and rt == 0: regs[rd] = left
			elif (word&63) not in [8,9]: regs[rd] = null
		elif op in [2,3,4,5,6,7]:
			if op == 3: pending_call = 2
		elif op not in [40,41,43]: regs[rt] = null
		regs[0] = ["constant",0]
		if pending_call:
			pending_call -= 1
			if not pending_call:
				for register in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25,31]: regs[register] = null
	return stores

static func _field_value(stores: Array, address: int, mask: int, width: int, shift: int) -> int:
	var matches: Array = []
	for store: Dictionary in stores:
		if store.address == address and store.mask == mask and store.width == width:
			matches.append(store.value >> shift)
	return int(matches[0]) if matches.size() == 1 else -1

static func _rules(exe: PackedByteArray, overlay: PackedByteArray) -> Dictionary:
	var tables := {"compatibility":_bytes(overlay,0x1b03c,16),"initial_scores":_bytes(overlay,0x1b04c,20),"space":_bytes(exe,0x72e44,2),"cheat_names":[],"girl_birthdays":[],"girl_bloods":[],"girl_fixed_clubs":[]}
	for offset: int in CHEAT_OFFSETS: tables.cheat_names.append(_cstring(overlay,offset,16))
	var birthdays := _field_stores(exe.slice(2048),0x80041000,0x80041d04,0x80041f48)
	var setup := _field_stores(overlay,BASE,0x801390c8,0x80139440)
	for i in 14:
		var address := 0x800e6280+0x1cc+i*0x38
		tables.girl_birthdays.append(_field_value(birthdays,address,0xf0,1,0))
		tables.girl_birthdays.append(_field_value(birthdays,address,0xfe0f,2,4))
	for i in range(1,13): tables.girl_bloods.append(_field_value(setup,0x800e6280+0x1cd+i*0x38,0xf9,1,1))
	for i in range(5,12): tables.girl_fixed_clubs.append(_field_value(setup,0x800e6280+0x1ca+i*0x38,15,1,4))
	return {"schema":"tokimemo.new-game-tables.v1","tables":tables,"proof":{"overlay_sha256":OVERLAY_SHA,"exe_sha256":EXE_SHA}}

static func _entry(exe: PackedByteArray, overlay: PackedByteArray) -> Dictionary:
	var content := {"space":_bytes(exe,0x72e44,2)}
	for field: String in ["hiragana","katakana"]:
		var rows: Array = []; var offset := 0x1ad28 if field == "hiragana" else 0x1ad6c
		for i in 17:
			var pointer := overlay.decode_u32(offset+i*4)-BASE
			var row := _cstring(overlay,pointer,33)
			if pointer < 0 or pointer >= overlay.size() or row.size() > 11: return {}
			rows.append(row)
		content[field] = rows
	for field: String in ["preset_family","preset_given","preset_nickname"]:
		var offset := 0x1ae6c if field == "preset_family" else (0x1aee4 if field == "preset_given" else 0x1af5c)
		var width := 12 if field == "preset_nickname" else 8
		var rows: Array = []
		for i in 15: rows.append(_bytes(overlay,offset+i*width,width))
		content[field] = rows
	return content

# Reviewed descriptor provenance/geometry only; payloads are read from the exe.
const FONT_TEMPLATE := '{"metadata":{"evidence":["src/main/text_system.c::load_string","src/main/text_system.c::text_glyph_expand","include/main/text.h:FontRange"],"format":{"encoding":"1bpp_msb_first","height":12,"upload_bpp":4,"upload_height":12,"upload_width":16,"width":12},"glyph_bank_sha256":"6c60c8863b4e15407c5fced0f316351e9cf2e18492518f812aa7606218d4159a","glyph_count":3418,"glyph_offset":484688,"glyph_stride":18,"id":"font:SLPM_86.053","range_length":184,"range_offset":473132,"source_path":"SLPM_86.053","source_sha256":"bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"},"palettes":[{"address":2148220844,"colors_per_row":2,"label":"text_palette_0","length":32,"rows":8,"sha256":"823eec240e10ca4376e7baff6a497fe999691fc0586c1192794a981a56e83baf"},{"address":2148220876,"colors_per_row":2,"label":"text_palette_1","length":32,"rows":8,"sha256":"b46dc6e92ec649b6d5c83e076add649b1ac535562daa03faa5fbd7703e15a13c"},{"address":2148220908,"colors_per_row":2,"label":"text_shadow_palette_0","length":32,"rows":8,"sha256":"455e0322b0c5b9b4c62e2332cde38c695eecb67cd3472225c5192bb4fa6b6807"},{"address":2148220940,"colors_per_row":2,"label":"text_shadow_palette_1","length":32,"rows":8,"sha256":"04385d6ccbc3d3975c367b6f1dfcba621b27145c3c091b6282b388ffd424ad10"}],"payload":"glyph_bank.bin","payload_sha256":"6c60c8863b4e15407c5fced0f316351e9cf2e18492518f812aa7606218d4159a","payload_size_bytes":61524,"schema":"tokimemo.native-font.v1","schema_version":1,"source_proof":{"assembly_palette_reader":"tools.assets.text_binary._spans","palette_assembly_path":"asm/data/main/text_system.data.s","proof_reader":"tools.assets.text_font.verify_font_source_proof","reader":"tools.assets.text_font.font_metadata","source_path":"SLPM_86.053","source_sha256":"bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae","verified_data_labels":[{"address":2148220972,"initialized_bytes_verified":184,"label":"text_font_range_table"},{"address":2148232528,"initialized_bytes_verified":61524,"label":"text_font_glyphs"}]}}'

static func _font(exe: PackedByteArray) -> Dictionary:
	var descriptor: Dictionary = _integer_tree(JSON.parse_string(FONT_TEMPLATE))
	var ranges: Array = []
	for i in 46: ranges.append({"first":exe.decode_u16(473132+i*4),"glyph":exe.decode_u16(473134+i*4)})
	descriptor.metadata.ranges = ranges
	descriptor.metadata.range_raw_hex = exe.slice(473132,473316).hex_encode()
	for palette: Dictionary in descriptor.palettes:
		var offset: int = int(palette.address)-0x80041000+2048
		var raw := exe.slice(offset,offset+32)
		var words: Array = []
		for i in 16: words.append(raw.decode_u16(i*2))
		palette.raw_hex = raw.hex_encode(); palette.words = words
	return descriptor

static func _safe_root(path: String) -> bool:
	if not path.is_absolute_path() or path.begins_with("res://") or path.begins_with("user://") or ".." in path.split("/"): return false
	var current := "/"
	for part: String in path.split("/",false):
		current = current.path_join(part)
		var parent := DirAccess.open(current.get_base_dir())
		if parent != null and parent.is_link(current.get_file()): return false
	return true

static func _write(path: String, raw: PackedByteArray) -> bool:
	if not _safe_root(path) or FileAccess.file_exists(path): return false
	if DirAccess.make_dir_recursive_absolute(path.get_base_dir()) != OK: return false
	if not _safe_root(path): return false
	var file := FileAccess.open(path,FileAccess.WRITE)
	if file == null: return false
	file.store_buffer(raw); file.flush()
	var good := file.get_error() == OK
	file.close()
	return good and FileAccess.get_sha256(path) == _sha(raw)

static func stage(exe: PackedByteArray, overlay: PackedByteArray, runtime_root: String) -> Dictionary:
	if exe.size() != 667648 or _sha(exe) != EXE_SHA or overlay.size() != 196608 or _sha(overlay) != OVERLAY_SHA:
		return {"error":"original New Game input identity differs"}
	if not _safe_root(runtime_root): return {"error":"runtime staging root is unsafe"}
	var rules := canonical_json(_rules(exe,overlay)).to_utf8_buffer()
	var entry := canonical_json(_entry(exe,overlay)).to_utf8_buffer()
	var descriptor := (canonical_json(_font(exe))+"\n").to_utf8_buffer()
	if _sha(entry) != Entry.CONTENT_SHA or _sha(descriptor) != Glyphs.DESCRIPTOR_SHA256:
		return {"error":"derived original entry/font identity differs"}
	var roles := {"name_entry_rules":"new-game/rules.json","name_entry_content":"new-game/entry.json","font_root":FONT_ROOT}
	var outputs := {roles.name_entry_rules:rules,roles.name_entry_content:entry,FONT_ROOT+"/font.json":descriptor,FONT_ROOT+"/glyph_bank.bin":exe.slice(484688,546212)}
	# The backend supplies a fresh private staging directory. Refuse collisions
	# before writing any file; a failed stage must be discarded by its owner.
	for relative: String in outputs:
		var target := runtime_root.path_join(relative)
		if not _safe_root(target) or FileAccess.file_exists(target) or DirAccess.dir_exists_absolute(target): return {"error":"runtime staging output already exists or is unsafe"}
	for relative: String in outputs:
		if not _write(runtime_root.path_join(relative),outputs[relative]): return {"error":"cannot persist derived New Game content"}
	if Rules.load_tables(runtime_root.path_join(roles.name_entry_rules)).is_empty() or Entry.load_content(runtime_root.path_join(roles.name_entry_content)).is_empty():
		return {"error":"derived New Game loader authentication failed"}
	var glyphs := Glyphs.new()
	var font_error := glyphs.open_external_font(runtime_root.path_join(FONT_ROOT))
	if not font_error.is_empty(): return {"error":"derived native font authentication failed: "+font_error}
	return {"error":"","roles":roles}

static func _integer_tree(value: Variant) -> Variant:
	if value is Dictionary:
		var out := {}
		for key: String in value: out[key] = _integer_tree(value[key])
		return out
	if value is Array:
		var out: Array = []
		for item: Variant in value: out.append(_integer_tree(item))
		return out
	if typeof(value) == TYPE_FLOAT: return int(value)
	return value
