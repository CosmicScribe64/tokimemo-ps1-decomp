extends RefCounted
## T-39002 stage 21: the five prologue_finish_run bank reads.
## prologue.c::prologue_finish_run substep 0 calls
## audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD3D5, 0xD387, 0xD37D). After
## audio_bank_state_reset the a == 0 branch of frame_render_audio.c issues five
## whole-sector nowait reads, in READS order. Each payload is the exact sector
## read, including the over-read into the following ISO file, exported by
## tools/port/prologue_finish_bank_export.py. This reader only authenticates
## bytes; it grants no CD completion and no SPU transfer.
const ExternalPath = preload("res://services/external_content_path.gd")
const SCHEMA := "tokimemo.prologue-finish-bank.v1"
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
## The bank arguments exactly as prologue_finish_run passes them (a, b..f).
const SOURCE_ARGS := [0, 0xbf98, 0xbf79, 0xd3d5, 0xd387, 0xd37d]
## field, argument letter, lba, sectors, destination, bytes, sha256 of the
## sector read, then [path, lba, file size, file sha256, sectors in read] for
## every ISO file the read covers (first entry is the requested file).
const READS := [
	["se_header","b",0xbf98,3,0x80010000,6144,"653eda42f86a4de8eef40a49a1efb19fa1d4cda67e8a2de9043a31e3faf87552",[
		["CDROM/VAB_SE/COM_MIX.VH",0xbf98,3104,"37d22463afde8c0fb7ba0e6b4e16e9ceba4270b582478065d7af3ffdb710e509",2],
		["CDROM/VAB_SE/EFF1.BAK",0xbf9a,3616,"cb4b46ac40b16ed4c843cddef50605f72b3de228b1fcdd901066540761d3a015",1]]],
	["se_body","c",0xbf79,0x20,0x801b8000,65536,"8a1034f0413cd0232e63448ac54573528bd18248636f5a129f5d34725af392a9",[
		["CDROM/VAB_SE/COM_MIX.VB",0xbf79,62400,"80c7fcb3527b56d02a2b86908731c0252fd981373666f11a9ec8c763aeab37d6",31],
		["CDROM/VAB_SE/COM_MIX.VH",0xbf98,3104,"37d22463afde8c0fb7ba0e6b4e16e9ceba4270b582478065d7af3ffdb710e509",1]]],
	["sequence","f",0xd37d,0x25,0x8001c000,75776,"df78bc6a6237b90c5f5fe323ec2ae0760b84159f642665b76a543a7ba904965b",[
		["CDROM/VAB_SEQ/SPRING.SEQ",0xd37d,18516,"3d3dbe0581ae2c94510f432ad7d35cd10b2b9b6238668f7a158ba62e16ebeffd",10],
		["CDROM/VAB_SEQ/SPRING.VB",0xd387,158992,"7c227a51635865e40d33723f4be40489c0dfebb3eb8362acee114eba1a06a42d",27]]],
	["music_body","e",0xd387,0x50,0x801c8000,163840,"69ad06b80c84e616ead7b2132118c07116b02534bfd25a09bddba44c640afbb2",[
		["CDROM/VAB_SEQ/SPRING.VB",0xd387,158992,"7c227a51635865e40d33723f4be40489c0dfebb3eb8362acee114eba1a06a42d",78],
		["CDROM/VAB_SEQ/SPRING.VH",0xd3d5,8736,"3c54d7c565950192877ef84fc95a6e5c3791d2ec72345b6beec59e3b2594bef8",2]]],
	["music_header","d",0xd3d5,6,0x80012000,12288,"e3958c5a8e7918716f9d2062d43c878c828a2bb611ec242a4084360ee31a6e5c",[
		["CDROM/VAB_SEQ/SPRING.VH",0xd3d5,8736,"3c54d7c565950192877ef84fc95a6e5c3791d2ec72345b6beec59e3b2594bef8",5],
		["CDROM/VAB_SEQ/STUDIUM.SEQ",0xd3da,21816,"3dad349e5ca1c201b501dd326b5b42d194dd101aa38b380d1ef1f3b183b280cb",1]]],
]
const ID_PREFIX := "prologue.finish_bank."
var _root := ""
var _reads: Dictionary = {}

static func resource_id(field: String) -> String:
	return ID_PREFIX + field

static func descriptor_key(sectors: int, destination: int, lba: int) -> String:
	return "%d:%d:%d" % [sectors, destination, lba]

static func _same_int(value: Variant, wanted: int) -> bool:
	# JSON numbers parse as float; accept only exact finite integral values.
	return (value is int or value is float) and is_finite(float(value)) and value == wanted

static func _check_record(record: Variant, row: Array) -> bool:
	if not record is Dictionary: return false
	if record.get("field") != row[0] or record.get("argument") != row[1] or not _same_int(record.get("lba"),row[2]) or not _same_int(record.get("sectors"),row[3]) or not _same_int(record.get("destination"),row[4]): return false
	if not _same_int(record.get("size"),row[5]) or record.get("sha256") != row[6] or record.get("path") != "reads/%s.bin" % row[0]: return false
	var segments: Variant = record.get("segments")
	if not segments is Array or segments.size() != row[7].size(): return false
	var offset := 0
	for index in range(segments.size()):
		var segment: Variant = segments[index]
		var want: Array = row[7][index]
		if not segment is Dictionary or segment.get("path") != want[0] or not _same_int(segment.get("lba"),want[1]) or not _same_int(segment.get("file_size"),want[2]) or segment.get("file_sha256") != want[3] or not _same_int(segment.get("sectors_in_read"),want[4]) or not _same_int(segment.get("read_offset"),offset) or segment.get("sector_padding_zero") != true:
			return false
		offset += int(want[4]) * 2048
	return offset == int(row[5]) and record.get("primary_file") == row[7][0][0] and _same_int(record.get("over_read_sectors"),int(row[3]) - int(ceil(float(row[7][0][2]) / 2048.0)))

func open_pack(directory: String) -> String:
	if not _root.is_empty(): return "finish bank reader already owns an immutable pack"
	if not ExternalPath.is_external_path(directory) or not DirAccess.dir_exists_absolute(directory):
		return "finish bank pack requires a canonical external directory"
	var manifest_path := directory.path_join("finish_bank.json")
	if not ExternalPath.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > 64 * 1024:
		return "finish bank manifest is missing, linked or oversized"
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not manifest is Dictionary or manifest.get("schema") != SCHEMA or manifest.get("release") != "original-ja-ps-the-best" or manifest.get("track_sha256") != TRACK_SHA:
		return "finish bank manifest schema, release or disc differs"
	if manifest.get("native_transfer_supported") != false:
		return "finish bank metadata cannot grant a native SPU transfer"
	var order: Variant = manifest.get("order")
	var records: Variant = manifest.get("reads")
	if not order is Array or order != READS.map(func(row: Array) -> String: return row[0]) or not records is Dictionary or records.size() != READS.size():
		return "finish bank read order differs from audio_bank_load_start"
	var loaded: Dictionary = {}
	for row: Array in READS:
		if not _check_record(records.get(row[0]),row):
			return "finish bank read identity differs: " + row[0]
		var path := directory.path_join("reads/%s.bin" % row[0])
		if not ExternalPath.is_external_file(path) or FileAccess.get_size(path) != row[5] or FileAccess.get_sha256(path) != row[6]:
			return "finish bank sector read is missing or changed: " + row[0]
		loaded[row[0]] = {"bytes":FileAccess.get_file_as_bytes(path),"field":row[0],"lba":row[2],"sectors":row[3],"destination":row[4],"sha256":row[6]}
	_root = directory
	_reads = loaded
	return ""

func identity() -> String:
	return "" if _root.is_empty() else TRACK_SHA

## Queue descriptions keyed by sectors:destination:lba, in source read order.
func descriptions() -> Dictionary:
	var out: Dictionary = {}
	if _root.is_empty(): return out
	for row: Array in READS:
		out[descriptor_key(row[3],row[4],row[2])] = {"id":resource_id(row[0]),"sha256":row[6],"field":row[0],"lba":row[2],"sectors":row[3],"destination":row[4]}
	return out

func transfer(id: String) -> Dictionary:
	for row: Array in READS:
		if resource_id(row[0]) == id and _reads.has(row[0]):
			var item: Dictionary = _reads[row[0]]
			return {"error":"","bytes":item.bytes.duplicate(),"destination":item.destination,"sha256":item.sha256,"lba":item.lba,"sectors":item.sectors}
	return {"error":"unknown finish bank read"}
