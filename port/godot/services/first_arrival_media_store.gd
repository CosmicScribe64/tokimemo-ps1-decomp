extends RefCounted
## Finite source-backed stage-0 transfers and prepared pixels; no host I/O in rules.
## The tenth background sector belongs to P_01_02, never to the MDEC decoder.
const External = preload("res://services/external_content_path.gd")
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const BG_SHA := "280b440f4f005d53340afd5389cd755a5bbb574fa30c36013df484d039fd7aab"
const BG_TRANSFER_SHA := "7df0f23cba36accc6f4a5e2b73921def4b7eb986c11f556999f3b848f90b6922"
const BG_ADJACENT_SHA := "5065feceb3135b339eae443d62322d3d70554f864e7dcf0652ba5785b2c18848"
const BG_PNG_SHA := "01615ba01ae49c662040d7e1d91248862db3ee66d216088477e9a4098b8fb03b"
const ACTOR_SHA := "bb224a47fa1ac9c102dd4701d3d3e0819ba30cac1e5637183886b8be4f95340c"
const BG_ID := "arrival-bg-494b-native10"
const ACTOR_ID := "arrival-o01-hidden-stage0"
const BG_FILE := "native/arrival-bg-transfer.bin"
const ACTOR_FILE := "actor/O_01.NBN"
const PNG_FILE := "background/arrival-stage0.background.png"
var _ready := false
var _background := PackedByteArray()
var _actor := PackedByteArray()
var _image: Image
var _manifest_sha := ""
var _uploads: Array[Dictionary] = []

func open(root: String, approved_manifest_sha: String) -> String:
	if _ready:
		return "first-arrival resources are immutable; prepare a new store for another pack"
	if not External.is_external_path(root) or not _sha_string(approved_manifest_sha):
		return "first-arrival media needs an approved external non-symlink pack"
	var manifest_path := root.path_join("manifest.json")
	if not External.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > 2*1024*1024 or FileAccess.get_sha256(manifest_path) != approved_manifest_sha:
		return "first-arrival media manifest is missing, changed or outside bounds"
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not parsed is Dictionary or parsed.get("schema") is not String or parsed.schema != "tokimemo.first-arrival-media.v1" or not parsed.get("source") is Dictionary or parsed.source.get("sha256") is not String or parsed.source.sha256 != TRACK_SHA:
		return "first-arrival media release/schema is unsupported"
	if not parsed.get("runtime_profile") is Dictionary or not parsed.get("native_transfers") is Dictionary or not parsed.get("files") is Dictionary:
		return "first-arrival native transfer profile is missing"
	var profile: Dictionary = parsed.runtime_profile
	if profile.get("id") is not String or profile.id != "stage0_hidden_actor.v1" or not _equals_int(profile.get("stage"),0) or not _equals_int(profile.get("actor96_initial_draw_flags"),0x24) or profile.get("actor96_visible_rendering") is not bool or profile.actor96_visible_rendering or not _equals_int(profile.get("background_slot"),1) or not _equals_int(profile.get("background_brightness"),128):
		return "first-arrival runtime profile exceeds hidden stage0 support"
	var bg: Variant = parsed.native_transfers.get("background")
	var actor: Variant = parsed.native_transfers.get("actor")
	if not _transfer_description(bg,BG_FILE,BG_TRANSFER_SHA,0x494b,10,0x80162000) or not _transfer_description(actor,ACTOR_FILE,ACTOR_SHA,0xabab,13,0x80180000):
		return "first-arrival native read extent differs from the original source"
	if not _equals_int(bg.get("decoder_size"),18432) or bg.get("decoder_sha256") is not String or bg.decoder_sha256 != BG_SHA or not bg.get("adjacent_extent") is Dictionary:
		return "first-arrival background decoder extent is not the nine-sector file"
	var adjacent: Dictionary = bg.adjacent_extent
	if not _equals_int(adjacent.get("lba"),0x4954) or not _equals_int(adjacent.get("size"),2048) or adjacent.get("path") is not String or adjacent.path != "CDROM/BS/P_01_02.BS" or adjacent.get("sha256") is not String or adjacent.sha256 != BG_ADJACENT_SHA:
		return "first-arrival tenth sector lacks its adjacent-file identity"
	if parsed.files.is_empty() or parsed.files.size() > 64:
		return "first-arrival file inventory is outside bounds"
	var selected: Dictionary = {}
	var total := 0
	for relative: Variant in parsed.files:
		if not relative is String or not _relative(relative):
			return "first-arrival media inventory has an unsafe path"
		var record: Variant = parsed.files[relative]
		if not record is Dictionary or record.size() != 3 or not record.has_all(["kind","size","sha256"]) or not record.kind is String or record.kind not in ["png","original_bytes"] or not _integer(record.get("size")) or record.size < 0 or record.size > 512*1024 or not _sha_string(record.get("sha256")):
			return "first-arrival media file identity is malformed"
		total += int(record.size)
		if total > 2*1024*1024:
			return "first-arrival media inventory exceeds its byte bound"
		var path := root.path_join(relative)
		if not External.is_external_file(path) or FileAccess.get_size(path) != int(record.size):
			return "first-arrival media file is missing, linked or has changed size"
		var file := FileAccess.open(path,FileAccess.READ)
		if file == null or file.get_length() != int(record.size):
			return "first-arrival media file could not be opened at its bounded extent"
		var bytes := file.get_buffer(int(record.size))
		if bytes.size() != int(record.size) or file.get_length() != int(record.size) or not External.is_external_file(path) or _sha(bytes) != record.sha256:
			return "first-arrival media file hash/extent changed during preparation"
		if relative in [BG_FILE,ACTOR_FILE,PNG_FILE,"background/P_01_01.BS"]:
			selected[relative] = bytes
	if not selected.has_all([BG_FILE,ACTOR_FILE,PNG_FILE,"background/P_01_01.BS"]):
		return "first-arrival native transfers, decoder bytes or prepared pixels are missing"
	var background: PackedByteArray = selected[BG_FILE]
	var actor_bytes: PackedByteArray = selected[ACTOR_FILE]
	if background.size() != 20480 or _sha(background) != BG_TRANSFER_SHA or _sha(background.slice(0,18432)) != BG_SHA or _sha(background.slice(18432)) != BG_ADJACENT_SHA or selected["background/P_01_01.BS"] != background.slice(0,18432):
		return "first-arrival background native and decoder extents differ"
	if actor_bytes.size() != 26624 or _sha(actor_bytes) != ACTOR_SHA or actor_bytes.decode_u32(0xa8) != 0x801800d0:
		return "first-arrival actor read or texture pointer differs from source"
	if _sha(selected[PNG_FILE]) != BG_PNG_SHA:
		return "first-arrival prepared background pixels differ from the reviewed decoder"
	var image := Image.new()
	if image.load_png_from_buffer(selected[PNG_FILE]) != OK or image.get_size() != Vector2i(320,160):
		return "first-arrival prepared background pixels are invalid"
	image.convert(Image.FORMAT_RGBA8)
	_background = background
	_actor = actor_bytes
	_image = image
	_manifest_sha = approved_manifest_sha
	_uploads = _actor_uploads(actor_bytes)
	_ready = true
	return ""

func descriptions() -> Dictionary:
	if not _ready: return {}
	return {"backgrounds":{str(0x494b):{"id":BG_ID,"sha256":BG_TRANSFER_SHA,"slot":1,"decoded":true}},
		"actors":{str(0xabab):{"id":ACTOR_ID,"sha256":ACTOR_SHA,"sectors":13,"destination":0x80180000,
			"native_composition":false,"hidden_stage0_profile":"o01_hidden.v1"}}}

func transfer(resource_id: String) -> Dictionary:
	if not _ready: return {"error":"first-arrival media is not prepared"}
	if resource_id == BG_ID:
		return {"error":"","bytes":_background.duplicate(),"destination":0x80162000,"sha256":BG_TRANSFER_SHA}
	if resource_id == ACTOR_ID:
		return {"error":"","bytes":_actor.duplicate(),"destination":0x80180000,"sha256":ACTOR_SHA}
	return {"error":"unknown first-arrival transfer resource"}

func background_image() -> Image:
	return _image.duplicate() if _ready else null

func initial_actor() -> Dictionary:
	if not _ready: return {}
	# prologue_arrival_scene_init after actor_clear(96); visibility stays off.
	return {"index":96,"unk_01":10,"flags":1,"quad_flags":0x41000000,"draw_flags":0x24,
		"frame_table":0x801800ac,"sequence_table":0x801800c0,"tpage_table":0x801800a8,
		"unk_06":2,"unk_07":0,"sequence_slot":0,"unk_05":128,"x":-160,"y":-120,"brightness":128,
		"frame_index":0,"sequence_counter":0,"resource_id":ACTOR_ID,"source_sha256":ACTOR_SHA}

func identity() -> String:
	return _manifest_sha if _ready else ""

func upload_requests() -> Array[Dictionary]:
	return _uploads.duplicate(true)

func validate_upload(value: Variant) -> String:
	if not _ready or not value is Dictionary or not value.get("id") is String:
		return "first-arrival native upload lacks an authenticated source identity"
	for expected: Dictionary in _uploads:
		if value.id == expected.id:
			return "" if _same(value,expected) else "first-arrival native upload differs from its exact source operation"
	return "unknown first-arrival native upload operation"

func upload_source(operation_id: String) -> Dictionary:
	for request: Dictionary in _uploads:
		if request.id == operation_id:
			if request.source.get("resource_id") != ACTOR_ID:
				return {"error":"native gray palette requires the preceding runtime scratch operation"}
			return {"error":"","bytes":_actor.slice(request.source.offset,request.source.offset+request.source.size),
				"sha256":request.source.sha256}
	return {"error":"unknown first-arrival upload source"}

static func _actor_uploads(actor: PackedByteArray) -> Array[Dictionary]:
	var requests: Array[Dictionary] = []
	requests.append({"id":"actor96.tpage","operation":"load_tpage_buf_lock","slot":0x1a,"mode":1,
		"source":_upload_extent(actor,0xd0,0x4000),"destination":{"x_words":0x280,"y":0x100,"width_words":64,"height":128}})
	for row in range(16):
		requests.append({"id":"actor96.palette."+str(row),"operation":"load_palette","palette_index":16+row,
			"mode":2,"call_source_address":0x80184490,"source_row":row,"source":_upload_extent(actor,0x4490+row*512,512),
			"destination":{"x_words":0,"y":0x1f0+row,"width_words":256,"height":1}})
	# The C bitfield write preserves destination STP bits. A provider must own
	# the live scratch destination; original source bytes alone cannot supply it.
	requests.append({"id":"actor96.gray","operation":"color_palette_to_gray","source":_upload_extent(actor,0x4610,512),
		"destination_address":0x800e78bc,"preserve_destination_stp":true})
	requests.append({"id":"actor96.gray_palette","operation":"load_palette","palette_index":13,"mode":2,"source_row":0,
		"source":{"after_operation":"actor96.gray","runtime_address":0x800e78bc,"size":512},
		"destination":{"x_words":0,"y":0x1ed,"width_words":256,"height":1}})
	return requests

static func _upload_extent(actor: PackedByteArray, offset: int, size: int) -> Dictionary:
	return {"resource_id":ACTOR_ID,"address":0x80180000+offset,"offset":offset,"size":size,"sha256":_sha(actor.slice(offset,offset+size))}

static func _same(value: Variant, expected: Variant) -> bool:
	if typeof(value) != typeof(expected): return false
	if expected is Dictionary:
		if value.size() != expected.size() or not value.has_all(expected.keys()): return false
		for key: Variant in expected:
			if not _same(value[key],expected[key]): return false
		return true
	if expected is Array:
		if value.size() != expected.size(): return false
		for index in expected.size():
			if not _same(value[index],expected[index]): return false
		return true
	return value == expected

static func _transfer_description(value: Variant, file: String, hash_value: String, lba: int, sectors: int, destination: int) -> bool:
	return value is Dictionary and value.get("file") is String and value.file == file and value.get("sha256") is String and value.sha256 == hash_value and _equals_int(value.get("lba"),lba) and _equals_int(value.get("size"),sectors*2048) and _equals_int(value.get("sectors"),sectors) and _equals_int(value.get("destination"),destination)

static func _integer(value: Variant) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and floor(float(value)) == float(value) and value >= 0 and value <= 0xffffffff

static func _equals_int(value: Variant, expected: int) -> bool:
	return _integer(value) and int(value) == expected

static func _relative(value: String) -> bool:
	if value.is_empty() or value.length() > 256 or value.begins_with("/") or value.contains("\\") or value.contains(":"): return false
	for component: String in value.split("/",true):
		if component in ["",".",".."]: return false
	return true

static func _sha_string(value: Variant) -> bool:
	if not value is String or value.length() != 64: return false
	for code in value.to_ascii_buffer():
		if not (code >= 48 and code <= 57 or code >= 97 and code <= 102): return false
	return true

static func _sha(bytes: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(bytes)
	return hash.finish().hex_encode()
