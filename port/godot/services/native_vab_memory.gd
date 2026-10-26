extends RefCounted
## Actual bank image installation into owned software sound RAM.
## This is not a sequence synthesizer and cannot authorize music playback.
## Layout: shipped libsnd opening/transfer routines (asm/libsnd.s::func_8008C210 /
## SsVabOpenHeadSticky); sparse ordinal and ProgAtr sample-end table mutations
## match PS1 hardware witness dumps in connected-control-v2/native/receipt.json.

const SOUND_RAM_BYTES: int = 512 * 1024
const MAX_SLOT_INDEX: int = 3
const SPU_ALIGNMENT_MASK: int = 7
const HEADER_MIN_BYTES: int = 0x0a20
const HEADER_MAX_BYTES: int = 65536
const VAB_VERSION_6: int = 6
const VAB_VERSION_7: int = 7
const HEADER_VERSION_OFFSET: int = 4
const HEADER_TOTAL_SIZE_OFFSET: int = 12
const HEADER_PROGRAMS_OFFSET: int = 0x12
const HEADER_TONES_OFFSET: int = 0x14
const HEADER_VAGS_OFFSET: int = 0x16
const PROG_ATR_BASE_OFFSET: int = 0x20
const PROG_ATR_STRIDE: int = 16
const PROG_ATR_ORDINAL_OFFSET: int = 8
const PROG_ATR_EVEN_END_OFFSET: int = 0x0c
const PROG_ATR_ODD_END_OFFSET: int = 0x0e
const VAG_ATR_BASE_OFFSET: int = 0x0820
const VAG_ATR_PROGRAM_STRIDE: int = 512
const VAG_ATR_TONE_STRIDE: int = 32
const VAG_ATR_PROG_FIELD_OFFSET: int = 20
const VAG_ATR_VAG_FIELD_OFFSET: int = 22
const MAX_PROGRAMS: int = 128
const MAX_SAMPLES: int = 256
const MAX_TONES_PER_PROGRAM: int = 16
const VAG_SIZE_SHIFT: int = 8
const VAG_BLOCK_ALIGN_MASK: int = 15
const SPU_ADDR_SHIFT: int = 3
const U16_MASK: int = 0xffff

var sound_ram: PackedByteArray = PackedByteArray()
var banks: Dictionary = {}


class Outcome extends RefCounted:
	var error: String = ""
	var installed: bool = false
	var sample_bytes: int = 0


func _init() -> void:
	sound_ram.resize(SOUND_RAM_BYTES)


static func _sha(raw: PackedByteArray) -> String:
	var hash_ctx: HashingContext = HashingContext.new()
	hash_ctx.start(HashingContext.HASH_SHA256)
	hash_ctx.update(raw)
	return hash_ctx.finish().hex_encode()


func checkpoint() -> Dictionary:
	return {
		"sound_ram": sound_ram.duplicate(),
		"banks": banks.duplicate(true),
	}


func restore(value: Dictionary) -> String:
	if not value.get("sound_ram") is PackedByteArray or value["sound_ram"].size() != SOUND_RAM_BYTES or not value.get("banks") is Dictionary:
		return "invalid software sound RAM checkpoint"
	var saved_ram: PackedByteArray = value["sound_ram"]
	var saved_banks: Dictionary = value["banks"]
	sound_ram = saved_ram.duplicate()
	banks = saved_banks.duplicate(true)
	return ""


func install(slot: int, address: int, capacity: int, header: PackedByteArray, body: PackedByteArray, identity: Dictionary) -> Outcome:
	var out: Outcome = Outcome.new()
	if slot < 0 or slot > MAX_SLOT_INDEX or address < 0 or capacity <= 0 or address + capacity > sound_ram.size() or (address & SPU_ALIGNMENT_MASK) != 0:
		out.error = "invalid native bank slot or sound RAM extent"
		return out

	if header.size() < HEADER_MIN_BYTES or header.size() > HEADER_MAX_BYTES or body.size() > SOUND_RAM_BYTES:
		out.error = "bank byte buffers exceed bounded VAB limits"
		return out

	if typeof(identity.get("header_sha256")) != TYPE_STRING or typeof(identity.get("body_sha256")) != TYPE_STRING or _sha(header) != identity["header_sha256"] or _sha(body) != identity["body_sha256"]:
		out.error = "VAB bytes differ from authenticated original bank identity"
		return out

	var version: int = header.decode_u32(HEADER_VERSION_OFFSET)
	var total_size: int = header.decode_u32(HEADER_TOTAL_SIZE_OFFSET)
	if header.slice(0, HEADER_VERSION_OFFSET).get_string_from_ascii() != "pBAV" or version not in [VAB_VERSION_6, VAB_VERSION_7] or total_size != header.size() + body.size():
		out.error = "unsupported VAB magic, version or paired extent"
		return out

	var programs: int = header.decode_u16(HEADER_PROGRAMS_OFFSET)
	var sample_count: int = header.decode_u16(HEADER_VAGS_OFFSET) + 1
	var size_table: int = VAG_ATR_BASE_OFFSET + programs * VAG_ATR_PROGRAM_STRIDE
	if programs > MAX_PROGRAMS or sample_count > MAX_SAMPLES or size_table + VAG_ATR_PROGRAM_STRIDE > header.size():
		out.error = "VAB table extent invalid"
		return out

	var samples: Array = []
	var cursor: int = 0
	for index: int in range(sample_count):
		var size: int = header.decode_u16(size_table + index * 2) * VAG_SIZE_SHIFT
		if (index == 0 and size != 0) or (size & VAG_BLOCK_ALIGN_MASK) != 0:
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
	for index: int in range(MAX_PROGRAMS):
		var count: int = header[PROG_ATR_BASE_OFFSET + index * PROG_ATR_STRIDE]
		if count > MAX_TONES_PER_PROGRAM:
			out.error = "VAB program exceeds sixteen tones"
			return out
		if count > 0:
			active.append(index)
		total_tones += count

	if active.size() > programs or total_tones != header.decode_u16(HEADER_TONES_OFFSET):
		out.error = "VAB active programs or tone count invalid"
		return out

	var program_rows: Dictionary = {}
	var mutable_header: PackedByteArray = header.duplicate()
	var ordinal: int = 0
	for program: int in range(MAX_PROGRAMS):
		# SDK Loop 1 (func_8008C210): writes the current packed ordinal into every ProgAtr+8 slot.
		mutable_header.encode_u32(PROG_ATR_BASE_OFFSET + program * PROG_ATR_STRIDE + PROG_ATR_ORDINAL_OFFSET, ordinal)
		var count: int = header[PROG_ATR_BASE_OFFSET + program * PROG_ATR_STRIDE]
		if count == 0:
			continue
		var tones: Array = []
		for tone_index: int in range(count):
			var offset: int = VAG_ATR_BASE_OFFSET + (ordinal * MAX_TONES_PER_PROGRAM + tone_index) * VAG_ATR_TONE_STRIDE
			var vag: int = header.decode_u16(offset + VAG_ATR_VAG_FIELD_OFFSET)
			if header.decode_u16(offset + VAG_ATR_PROG_FIELD_OFFSET) != program or vag == 0 or vag >= sample_count:
				out.error = "VAB tone owner or sample reference invalid"
				return out
			var sample_entry: Dictionary = samples[vag]
			tones.append({
				"raw": header.slice(offset, offset + VAG_ATR_TONE_STRIDE),
				"vag": vag,
				"sample": sample_entry.duplicate(true),
			})
		var prog_start: int = PROG_ATR_BASE_OFFSET + program * PROG_ATR_STRIDE
		program_rows[program] = {
			"raw": header.slice(prog_start, prog_start + PROG_ATR_STRIDE),
			"ordinal": ordinal,
			"tones": tones,
		}
		ordinal += 1

	# SDK Loop 2 (func_8008C210 lines 1047..1074): writes the cumulative SPU shifted
	# sample-end address ((address + spu_accum) >> 3) into ProgAtr+0x0C (even sample)
	# and ProgAtr+0x0E (odd sample) across 0..sample_count-1.
	var spu_accum: int = 0
	for sample_index: int in range(sample_count):
		var sample_record: Dictionary = samples[sample_index]
		var sample_size: int = int(sample_record["size"])
		spu_accum += sample_size
		var prog_slot: int = sample_index >> 1
		var sub_offset: int = PROG_ATR_EVEN_END_OFFSET if (sample_index & 1) == 0 else PROG_ATR_ODD_END_OFFSET
		var field_offset: int = PROG_ATR_BASE_OFFSET + prog_slot * PROG_ATR_STRIDE + sub_offset
		var shifted_end: int = ((address + spu_accum) >> SPU_ADDR_SHIFT) & U16_MASK
		mutable_header.encode_u16(field_offset, shifted_end)

	# Body transfer uses the summed sample extent, not the PCM decode result.
	# No mutation occurs until every structural and identity check has passed.
	for offset: int in range(cursor):
		sound_ram[address + offset] = body[offset]

	banks[slot] = {
		"address": address,
		"sample_bytes": cursor,
		"header": mutable_header,
		"header_sha256": _sha(mutable_header),
		"programs": program_rows,
		"samples": samples,
		"identity": identity.duplicate(true),
		"playback_capability": false,
	}
	out.installed = true
	out.sample_bytes = cursor
	return out
