extends RefCounted
## Actual bank image installation into owned software sound RAM.
## This is not a sequence synthesizer and cannot authorize music playback.
## Layout: shipped libsnd opening/transfer routines; sparse ordinal evidence is
## frozen separately in T-29012/sparse-vab-v1/proof.json.

var sound_ram: PackedByteArray = PackedByteArray()
var banks: Dictionary = {}


class Outcome extends RefCounted:
	var error: String = ""
	var installed: bool = false
	var sample_bytes: int = 0


func _init() -> void:
	sound_ram.resize(512 * 1024)


static func _sha(raw: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(raw)
	return hash.finish().hex_encode()


func checkpoint() -> Dictionary:
	return {
		"sound_ram": sound_ram.duplicate(),
		"banks": banks.duplicate(true),
	}


func restore(value: Dictionary) -> String:
	if not value.get("sound_ram") is PackedByteArray or value.sound_ram.size() != 512 * 1024 or not value.get("banks") is Dictionary:
		return "invalid software sound RAM checkpoint"
	sound_ram = value.sound_ram.duplicate()
	banks = value.banks.duplicate(true)
	return ""


func install(slot: int, address: int, capacity: int, header: PackedByteArray, body: PackedByteArray, identity: Dictionary) -> Outcome:
	var out := Outcome.new()
	if slot < 0 or slot > 3 or address < 0 or capacity <= 0 or address + capacity > sound_ram.size() or (address & 7):
		out.error = "invalid native bank slot or sound RAM extent"
		return out

	if header.size() < 0xa20 or header.size() > 65536 or body.size() > 512 * 1024:
		out.error = "bank byte buffers exceed bounded VAB limits"
		return out

	if typeof(identity.get("header_sha256")) != TYPE_STRING or typeof(identity.get("body_sha256")) != TYPE_STRING or _sha(header) != identity.header_sha256 or _sha(body) != identity.body_sha256:
		out.error = "VAB bytes differ from authenticated original bank identity"
		return out

	if header.slice(0, 4).get_string_from_ascii() != "pBAV" or header.decode_u32(4) not in [6, 7] or header.decode_u32(12) != header.size() + body.size():
		out.error = "unsupported VAB magic, version or paired extent"
		return out

	var programs: int = header.decode_u16(0x12)
	var sample_count: int = header.decode_u16(0x16) + 1
	var size_table: int = 0x820 + programs * 512
	if programs > 128 or sample_count > 256 or size_table + 512 > header.size():
		out.error = "VAB table extent invalid"
		return out

	var samples: Array = []
	var cursor: int = 0
	for index in sample_count:
		var size: int = header.decode_u16(size_table + index * 2) * 8
		if (index == 0 and size != 0) or (size & 15) != 0:
			out.error = "unsupported VAG sentinel or block alignment"
			return out
		samples.append({
			"index": index,
			"offset": cursor,
			"size": size,
			"sound_address": address + cursor,
		})
		cursor += size

	if cursor > body.size() or cursor > capacity:
		out.error = "VAB sample extent exceeds body or native slot capacity"
		return out

	var active: Array = []
	var total_tones: int = 0
	for index in 128:
		var count: int = header[0x20 + index * 16]
		if count > 16:
			out.error = "VAB program exceeds sixteen tones"
			return out
		if count > 0:
			active.append(index)
		total_tones += count

	if active.size() > programs or total_tones != header.decode_u16(0x14):
		out.error = "VAB active programs or tone count invalid"
		return out

	var program_rows: Dictionary = {}
	var mutable_header: PackedByteArray = header.duplicate()
	var ordinal: int = 0
	for program in 128:
		# SDK writes the current packed ordinal into every ProgAtr+8 slot.
		mutable_header.encode_u32(0x28 + program * 16, ordinal)
		var count: int = header[0x20 + program * 16]
		if count == 0:
			continue
		var tones: Array = []
		for tone_index in count:
			var offset: int = 0x820 + (ordinal * 16 + tone_index) * 32
			var vag: int = header.decode_u16(offset + 22)
			if header.decode_u16(offset + 20) != program or vag == 0 or vag >= sample_count:
				out.error = "VAB tone owner or sample reference invalid"
				return out
			tones.append({
				"raw": header.slice(offset, offset + 32),
				"vag": vag,
				"sample": samples[vag].duplicate(true),
			})
		program_rows[program] = {
			"raw": header.slice(0x20 + program * 16, 0x30 + program * 16),
			"ordinal": ordinal,
			"tones": tones,
		}
		ordinal += 1

	# Body transfer uses the summed sample extent, not the PCM decode result.
	# No mutation occurs until every structural and identity check has passed.
	for offset in cursor:
		sound_ram[address + offset] = body[offset]

	banks[slot] = {
		"address": address,
		"sample_bytes": cursor,
		"header": mutable_header,
		"programs": program_rows,
		"samples": samples,
		"identity": identity.duplicate(true),
		"playback_capability": false,
	}
	out.installed = true
	out.sample_bytes = cursor
	return out
