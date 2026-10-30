extends RefCounted
## Source prologue.c:1189 and sound_driver.c::audio_bank_layout_init.
const Memory = preload("res://services/native_vab_memory.gd")
const SLOT := 2
const ADDRESS := 0x39010
const CAPACITY := 0x28000
const MUTATED_HEADER_SHA := "b7f44729c24b134fae1a72049e254248b6fc6ba759b39fe1c02fdb06a24e47f4"
const INPUTS := {
	"music_header":[10272,"f5ae3b750f10fec1a1cd0698ab1b2540de93bde3f5b4bb62dfa8da23848e489c"],
	"music_body":[139680,"be8281abca7901556494ded265dd7f686cd5e58179db2f0d7dcb0ae95f90bb21"],
	"sequence":[6416,"18dd4e4a9050cdf2fea53c4ab799c301a64c0bc36e4a059d9d5f3717b0628574"],
}

static func read_inputs(root: String) -> Dictionary:
	var inputs: Dictionary = {}
	for field: String in INPUTS:
		var path := root.path_join("reads/prologue_rei-%s.bin" % field)
		var bytes := FileAccess.get_file_as_bytes(path)
		if bytes.size() != INPUTS[field][0] or FileAccess.get_sha256(path) != INPUTS[field][1]:
			return {"error":"Original Rei bank input differs: " + field}
		inputs[field] = bytes
	return {"error":"","inputs":inputs}

static func prepare(root: String) -> Dictionary:
	var read := read_inputs(root)
	if not read.error.is_empty(): return read
	var memory := Memory.new()
	var installed := memory.install(SLOT,ADDRESS,CAPACITY,read.inputs.music_header,read.inputs.music_body,
		{"header_sha256":INPUTS.music_header[1],"body_sha256":INPUTS.music_body[1]})
	if not installed.error.is_empty() or not installed.installed:
		return {"error":"Original Rei alternate VAB install failed: " + installed.error}
	return {"error":"","memory":memory.checkpoint(),"sequence":read.inputs.sequence,
		"inputs":read.inputs,"metadata":{"role":"prologue_rei","slot":SLOT,"address":ADDRESS}}

static func publish(memory: Memory, prepared: Dictionary) -> String:
	var inputs: Variant = prepared.get("inputs")
	if not inputs is Dictionary: return "Rei original publication inputs are missing"
	for field: String in INPUTS:
		if not inputs.get(field) is PackedByteArray or inputs[field].size() != INPUTS[field][0] or _sha(inputs[field]) != INPUTS[field][1]:
			return "Rei native publication input changed: " + field
	var source: Variant = prepared.get("memory")
	if not source is Dictionary or not source.get("banks") is Dictionary or not source.banks.has(SLOT):
		return "Rei alternate-bank preparation is missing"
	var bank: Variant = source.banks[SLOT]
	if not bank is Dictionary or bank.get("address") != ADDRESS or bank.get("sample_bytes") != INPUTS.music_body[0] or bank.get("header_sha256") != MUTATED_HEADER_SHA or not bank.get("header") is PackedByteArray or not source.get("sound_ram") is PackedByteArray:
		return "Rei alternate-bank metadata differs"
	if _sha(bank.header) != MUTATED_HEADER_SHA or _sha(source.sound_ram.slice(ADDRESS,ADDRESS+INPUTS.music_body[0])) != INPUTS.music_body[1] or not prepared.get("sequence") is PackedByteArray or _sha(prepared.sequence) != INPUTS.sequence[1]:
		return "Rei alternate-bank prepared bytes changed"
	var expected := Memory.new()
	var rebuilt := expected.install(SLOT,ADDRESS,CAPACITY,inputs.music_header,inputs.music_body,
		{"header_sha256":INPUTS.music_header[1],"body_sha256":INPUTS.music_body[1]})
	if not rebuilt.error.is_empty() or not rebuilt.installed or expected.banks[SLOT] != bank:
		return "Rei alternate-bank derived sample metadata changed"
	# Merge only slot2. Restoring the entire preparation would erase currently
	# playing slot1 and the SE bank, unlike the source's alternate transfer.
	var merged := memory.checkpoint()
	merged.banks[SLOT] = source.banks[SLOT].duplicate(true)
	var bytes: PackedByteArray = source.sound_ram.slice(ADDRESS,ADDRESS+INPUTS.music_body[0])
	for index in bytes.size(): merged.sound_ram[ADDRESS+index] = bytes[index]
	return memory.restore(merged)

static func _sha(bytes: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256); hash.update(bytes)
	return hash.finish().hex_encode()
