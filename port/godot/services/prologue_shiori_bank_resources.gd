extends RefCounted
## T-39002: stage10/substep8 prologue_shiori bank load (prologue.c ~1735).
## audio_bank_load_start(1,0,0,0xCBD4,0xCBAA,0xCBA5) issues three whole-sector
## nowait CD reads over the buffers the still-playing Rei sequence uses:
##   CBA5 x0x25 -> 0x8002E800 (sequence pack; Rei score is read from 0x8002E850)
##   CBAA x0x50 -> 0x801B8000 (body staging; no SPU effect until the transfer)
##   CBD4 x6    -> 0x80016000 (slot2 VAB header/program table)
## The bytes are exact sector reads, exported by
## tools/port/native_audio/export_shiori_bank.py and checked against the disc.
const INPUTS := {
	"sequence_pack":[75776,"0545fd982409dd4d105e2a327ca813f17edd7ab37b0494c7006397319fda796d"],
	"music_body":[163840,"97f14d89b05e5c530a33f8e3ef2411ec03fa908c32fe38aecfc274fcba140450"],
	"music_header":[12288,"622d0390afea51bccf234361a42ec93bbe3024874caa977d61b981af2d662bdd"],
}
const ORDER := ["sequence_pack","music_body","music_header"]

## MEASURED CONSTANT (not source-defined: CD-drive timing). Landing schedule of
## the three reads, in audio pumps after R, the pump of the frame that ran
## stage10/substep8. Each row is [pump, region, cumulative sectors resident
## before that pump's libsnd tick]; region 0=CBA5, 1=CBAA, 2=CBD4. Taken
## verbatim from the cba5/cbaa/cbd4 columns of
## .asset-cache/emu/shiori-landing-m2/r1/frames.jsonl (R = vsync 2206); all 8
## runs in .asset-cache/emu/shiori-landing-m2/summary.json share it exactly,
## and .asset-cache/emu/shiori-landing-m1/results.json (5 runs) agrees up to
## its +1 sampling convention. Sector order is the read order (prefix count).
## PCSX-Redux CD seek model, not a hardware capture; see T-39002 2026-10-29.
const LANDING_SCHEDULE := [
	[3,0,1],[15,0,4],[16,0,6],[17,0,9],[18,0,11],[19,0,14],[20,0,16],[21,0,19],[22,0,21],
	[23,0,24],[24,0,26],[25,0,29],[26,0,31],[27,0,34],[28,0,36],[29,0,37],
	[31,1,1],[43,1,2],[44,1,4],[45,1,7],[46,1,9],[47,1,12],[48,1,14],[49,1,17],[50,1,19],
	[51,1,22],[52,1,24],[53,1,27],[54,1,29],[55,1,32],[56,1,34],[57,1,37],[58,1,39],
	[59,1,42],[60,1,44],[61,1,47],[62,1,49],[63,1,52],[64,1,54],[65,1,57],[66,1,59],
	[67,1,62],[68,1,64],[69,1,67],[70,1,69],[71,1,72],[72,1,74],[73,1,77],[74,1,79],[75,1,80],
	[77,2,1],[89,2,2],[90,2,4],[91,2,6],
]
## cd_read_count reaches 52 (all three reads done) at R+91 in every run.
const READS_COMPLETE_PUMP := 91
## Measured landmarks used by tests (same sources as above).
const MEASURED := {"cba5_first":3,"cba5_complete":29,"cbaa_first":31,"cbaa_complete":75,
	"cbd4_first":77,"cbd4_complete":91,"fade_finish":112,"poll":181,"dispose":182}

static func read_inputs(root: String) -> Dictionary:
	var inputs: Dictionary = {}
	for field: String in ORDER:
		var path := root.path_join("reads/prologue_shiori-%s.bin" % field)
		if not FileAccess.file_exists(path): return {"error":"Original Shiori bank input is missing: " + field}
		var bytes := FileAccess.get_file_as_bytes(path)
		if bytes.size() != INPUTS[field][0] or FileAccess.get_sha256(path) != INPUTS[field][1]:
			return {"error":"Original Shiori bank input differs: " + field}
		inputs[field] = bytes
	return {"error":"","inputs":inputs}

static func validate(inputs: Variant) -> String:
	if not inputs is Dictionary or inputs.size() != ORDER.size(): return "Shiori original inputs are missing"
	for field: String in ORDER:
		if not inputs.get(field) is PackedByteArray or inputs[field].size() != INPUTS[field][0] or _sha(inputs[field]) != INPUTS[field][1]:
			return "Shiori native input changed: " + field
	return ""

static func native_inputs(inputs: Dictionary) -> Array:
	return [inputs.sequence_pack,inputs.music_body,inputs.music_header]

static func flat_schedule() -> PackedInt32Array:
	var flat := PackedInt32Array()
	for row: Array in LANDING_SCHEDULE: flat.append_array(PackedInt32Array(row))
	return flat

## Cumulative resident sectors per region after the landings of pump n.
static func landed_at(pump: int) -> Array:
	var counts := [0,0,0]
	for row: Array in LANDING_SCHEDULE:
		if row[0] <= pump: counts[row[1]] = row[2]
	return counts

static func _sha(bytes: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256); hash.update(bytes)
	return hash.finish().hex_encode()
