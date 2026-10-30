extends RefCounted
## T-39002: the two CD reads of the mode 0x34 command reload.
## home_screens.c::settings_command_screen_reload_calendar substep 1:
## cd_read_sectors(0x95, 0x801A0000, 0x9B94) (CALROM.NBN, whole file).
## home_screens.c::settings_room_bg_load_start -> background_images.c
## dec_bg_cd_read(D_800B5950[hud_season_index()], 0): on a slot-0 cache miss
## cd_read_sectors_nowait(0xA, 0x80162000, lba). D_800B5950[0] (spring) is
## 0x3FE9, C_02_02H.BS (9 sectors) plus one sector of C_02_02N.BS.
## Each payload is the exact sector read exported by
## tools/port/command_reload_export.py. This reader only authenticates bytes;
## it grants no CD completion, upload or decode.
const ExternalPath = preload("res://services/external_content_path.gd")
const SCHEMA := "tokimemo.command-reload-reads.v1"
const TRACK_SHA := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const MANIFEST := "command_reload.json"
## D_800B5950 as the executable holds it (s32[4] by hud_season_index()).
const ROOM_TABLE := [0x3fe9, 0x3ff2, 0x3fd7, 0x3fe0]
## field, lba, sectors, destination, bytes, sha256 of the sector read, then
## [path, lba, file size, file sha256, sectors in read] per covered ISO file.
const READS := [
	["hud_calendar",0x9b94,0x95,0x801a0000,305152,"775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867",[
		["CDROM/FUKUTEST/CALROM.NBN",0x9b94,305152,"775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867",149]]],
	["room_spring",0x3fe9,0xa,0x80162000,20480,"31cf1a0907a6c754325b363c282567f52d60da8089cdebe6cd456999945c56b2",[
		["CDROM/BS/C_02_02H.BS",0x3fe9,16936,"ea4f9201486e8bfc21b03706b4427a2e3e0865f2be7a30f5057802618747ec96",9],
		["CDROM/BS/C_02_02N.BS",0x3ff2,16940,"9ee3e7443d9641ccabedc812c4329cbffe2581fb932e0e2936d908b1fc054a55",1]]],
]
const ID_PREFIX := "command_reload."
var _root := ""
var _reads: Dictionary = {}

static func resource_id(field: String) -> String:
	return ID_PREFIX + field

static func _same_int(value: Variant, wanted: int) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and value == wanted

static func _check_record(record: Variant, row: Array) -> bool:
	if not record is Dictionary: return false
	if record.get("field") != row[0] or not _same_int(record.get("lba"),row[1]) or not _same_int(record.get("sectors"),row[2]) or not _same_int(record.get("destination"),row[3]): return false
	if not _same_int(record.get("size"),row[4]) or record.get("sha256") != row[5] or record.get("path") != "reads/%s.bin" % row[0]: return false
	var segments: Variant = record.get("segments")
	if not segments is Array or segments.size() != row[6].size(): return false
	var offset := 0
	for index in range(segments.size()):
		var segment: Variant = segments[index]
		var want: Array = row[6][index]
		if not segment is Dictionary or segment.get("path") != want[0] or not _same_int(segment.get("lba"),want[1]) or not _same_int(segment.get("file_size"),want[2]) or segment.get("file_sha256") != want[3] or not _same_int(segment.get("sectors_in_read"),want[4]) or not _same_int(segment.get("read_offset"),offset) or segment.get("sector_padding_zero") != true:
			return false
		offset += int(want[4]) * 2048
	return offset == int(row[4]) and record.get("primary_file") == row[6][0][0]

func open_pack(directory: String) -> String:
	if not _root.is_empty(): return "command reload reader already owns an immutable pack"
	if not ExternalPath.is_external_path(directory) or not DirAccess.dir_exists_absolute(directory):
		return "command reload pack requires a canonical external directory"
	var manifest_path := directory.path_join(MANIFEST)
	if not ExternalPath.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > 64 * 1024:
		return "command reload manifest is missing, linked or oversized"
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not manifest is Dictionary or manifest.get("schema") != SCHEMA or manifest.get("release") != "original-ja-ps-the-best" or manifest.get("track_sha256") != TRACK_SHA:
		return "command reload manifest schema, release or disc differs"
	var order: Variant = manifest.get("order")
	var records: Variant = manifest.get("reads")
	if not order is Array or order != READS.map(func(row: Array) -> String: return row[0]) or not records is Dictionary or records.size() != READS.size():
		return "command reload read set differs from the source reads"
	var table: Variant = records.get("room_spring", {}).get("room_table") if records.get("room_spring") is Dictionary else null
	if not table is Array or table.size() != 4:
		return "command reload room read lacks the executable's D_800B5950 table"
	for index in range(4):
		if not _same_int(table[index], ROOM_TABLE[index]):
			return "command reload D_800B5950 differs from the executable"
	var loaded: Dictionary = {}
	for row: Array in READS:
		if not _check_record(records.get(row[0]),row):
			return "command reload read identity differs: " + row[0]
		var path := directory.path_join("reads/%s.bin" % row[0])
		if not ExternalPath.is_external_file(path) or FileAccess.get_size(path) != row[4] or FileAccess.get_sha256(path) != row[5]:
			return "command reload sector read is missing or changed: " + row[0]
		loaded[row[0]] = {"bytes":FileAccess.get_file_as_bytes(path),"field":row[0],"lba":row[1],"sectors":row[2],"destination":row[3],"sha256":row[5],"primary_sha256":row[6][0][3]}
	_root = directory
	_reads = loaded
	return ""

func identity() -> String:
	return "" if _root.is_empty() else TRACK_SHA

## D_800B5950[season] (hud_season_index() 0..3).
static func room_lba(season: int) -> int:
	return int(ROOM_TABLE[season]) if season >= 0 and season < 4 else -1

## Queue description of one read, empty when the pack is not open.
func description(field: String) -> Dictionary:
	if _root.is_empty() or not _reads.has(field): return {}
	var item: Dictionary = _reads[field]
	return {"id":resource_id(field),"sha256":item.sha256,"lba":item.lba,"sectors":item.sectors,
		"destination":item.destination,"native_composition":true,"primary_sha256":item.primary_sha256}

func transfer(id: String) -> Dictionary:
	for row: Array in READS:
		if resource_id(row[0]) == id and _reads.has(row[0]):
			var item: Dictionary = _reads[row[0]]
			return {"error":"","bytes":item.bytes.duplicate(),"destination":item.destination,"sha256":item.sha256,"lba":item.lba,"sectors":item.sectors,"primary_sha256":item.primary_sha256}
	return {"error":"unknown command reload read"}
