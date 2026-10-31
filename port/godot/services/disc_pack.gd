extends RefCounted
## T-39303 (M2, layer 4 of port-completion-plan): the installed disc pack reader.
##
## The pack (tools/port/disc_pack_content.py) holds the original files the game
## reads, each stored as whole 2048-byte sectors with its LBA, so the engine's CD
## calls (cd_read_sectors and friends) can read by LBA. This reader opens the
## pack, checks the manifest hash, and checks each file's SHA-256 on every read
## (the bytes returned are the bytes hashed). It never guesses: a read outside
## every file, across a file boundary, into a Form 2/XA stream or from a changed
## file fails with the reason and returns no bytes.
##
## Results are Dictionaries: {"ok": true, ...} or {"ok": false, "error": String}.

const ExternalPath = preload("res://services/external_content_path.gd")
const Memory = preload("res://core/game_memory.gd")

const SCHEMA := "tokimemo.disc-pack.v1"
const MANIFEST_NAME := "disc_pack.json"
const FILES_DIR := "files"
const SECTOR_SIZE := 2048
const MAX_FILES := 20000
const MAX_MANIFEST_BYTES := 16 * 1024 * 1024
const MAX_FILE_BYTES := 64 * 1024 * 1024
const MAX_READ_SECTORS := 4096
const RECORD_KEYS := ["path", "lba", "sectors", "size", "stored_size", "mode", "form", "sha256", "content_sha256"]
const EXCLUDED_KEYS := ["path", "lba", "sectors", "size", "reason"]

var root: String = ""
var manifest_sha256: String = ""
var volume_id: String = ""
## Packed file records sorted by LBA; `_by_path` maps path to its record.
var _files: Array[Dictionary] = []
var _starts := PackedInt64Array()
var _by_path: Dictionary = {}
var _excluded: Array[Dictionary] = []
## The last verified file, kept so repeated reads of one file hash it once.
var _cached_path: String = ""
var _cached_bytes := PackedByteArray()

## Opens the pack at `pack_root` (a directory holding disc_pack.json and files/).
## `expected_manifest_sha256`, when not empty, is the authenticated manifest hash
## (the installed inventory records it). Returns "" or the refusal.
func open(pack_root: String, expected_manifest_sha256: String = "") -> String:
	_reset()
	if not ExternalPath.is_external_path(pack_root) or not DirAccess.dir_exists_absolute(pack_root):
		return "disc pack must be an external directory without links"
	var manifest_path: String = pack_root.path_join(MANIFEST_NAME)
	if not FileAccess.file_exists(manifest_path):
		return "disc pack has no " + MANIFEST_NAME
	var size: int = FileAccess.get_size(manifest_path)
	if size <= 0 or size > MAX_MANIFEST_BYTES:
		return "disc pack manifest size is outside bounds"
	var text: String = FileAccess.get_file_as_string(manifest_path)
	var actual: String = FileAccess.get_sha256(manifest_path)
	if not expected_manifest_sha256.is_empty() and actual != expected_manifest_sha256:
		return "disc pack manifest hash differs from the expected hash"
	var json := JSON.new()
	if json.parse(text) != OK or not json.data is Dictionary:
		return "disc pack manifest is not a JSON object"
	var problem: String = _load_manifest(json.data)
	if not problem.is_empty():
		_reset()
		return problem
	root = pack_root
	manifest_sha256 = actual
	return ""

## Opens the disc_pack role of a validated installed profile (the dictionary
## InstalledLaunchProfile.read_pack returns), with its inventoried manifest hash.
func open_installed(profile: Dictionary) -> String:
	var paths: Variant = profile.get("paths")
	var manifests: Variant = profile.get("role_manifests")
	if not profile.get("valid", false) or not paths is Dictionary or not paths.has("disc_pack") \
			or not manifests is Dictionary or not manifests.has("disc_pack"):
		return "the installed profile has no disc_pack role"
	return open(paths.disc_pack, manifests.disc_pack.sha256)

func is_open() -> bool:
	return not root.is_empty()

func file_count() -> int:
	return _files.size()

func excluded_count() -> int:
	return _excluded.size()

## The record of a packed file (a copy), or {} when `path` is not packed.
func lookup_path(path: String) -> Dictionary:
	var record: Variant = _by_path.get(path)
	return (record as Dictionary).duplicate() if record is Dictionary else {}

## The record of the packed file whose sectors include `lba` (a copy), or {}.
func lookup_lba(lba: int) -> Dictionary:
	var index: int = _index_of(lba)
	return _files[index].duplicate() if index >= 0 else {}

## The excluded entry (a stream or reference with no data in the pack) whose
## sectors include `lba`, or {}.
func excluded_at(lba: int) -> Dictionary:
	for entry: Dictionary in _excluded:
		if lba >= int(entry.lba) and lba < int(entry.lba) + int(entry.sectors):
			return entry.duplicate()
	return {}

## Reads `count` sectors of 2048 bytes from `lba`. The whole range must lie in
## one packed file. Returns {"ok", "data", "path", "lba", "sectors"} or the error.
func read_sectors(lba: int, count: int) -> Dictionary:
	if not is_open():
		return _fail("disc pack is not open")
	if count < 1 or count > MAX_READ_SECTORS:
		return _fail("sector count %d is outside 1..%d" % [count, MAX_READ_SECTORS])
	if lba < 0:
		return _fail("LBA %d is negative" % lba)
	var index: int = _index_of(lba)
	if index < 0:
		return _fail(_outside_message(lba))
	var record: Dictionary = _files[index]
	var first: int = lba - int(record.lba)
	if first + count > int(record.sectors):
		return _fail("read of %d sectors at LBA 0x%X runs past the end of %s (%d sectors from LBA 0x%X)" % [
			count, lba, record.path, int(record.sectors), int(record.lba)])
	var stored: Dictionary = _verified_bytes(record)
	if not stored.ok:
		return stored
	var bytes: PackedByteArray = stored.data
	return {"ok": true, "data": bytes.slice(first * SECTOR_SIZE, (first + count) * SECTOR_SIZE),
		"path": record.path, "lba": lba, "sectors": count}

## cd_read_sectors: reads `count` sectors at `lba` into `memory` at the PS1
## `address`. Nothing is written unless the read succeeds and the range lies in
## main RAM. Returns {"ok", "path", "bytes"} or the error.
func read_into_memory(memory: Memory, address: int, lba: int, count: int) -> Dictionary:
	var length: int = count * SECTOR_SIZE
	if count >= 1 and not _in_ram(address, length):
		return _fail("destination 0x%08X (+0x%X) is outside main RAM" % [address, length])
	var result: Dictionary = read_sectors(lba, count)
	if not result.ok:
		return result
	var fault: String = memory.error
	memory.write_bytes(address, result.data)
	if memory.error != fault:
		return _fail("memory refused the write: " + memory.error)
	return {"ok": true, "path": result.path, "bytes": length}

## All the ISO bytes of a packed file (its `size`, without the sector padding).
func read_file(path: String) -> Dictionary:
	var record: Variant = _by_path.get(path)
	if not record is Dictionary:
		return _fail("no packed file " + path)
	var stored: Dictionary = _verified_bytes(record)
	if not stored.ok:
		return stored
	var bytes: PackedByteArray = stored.data
	return {"ok": true, "data": bytes.slice(0, int(record.size)), "path": path}

## Hashes every packed file against the manifest (an install-time or QA pass; reads do
## their own per-file check). Returns {"ok", "files", "bytes"} or the first error.
func verify_all() -> Dictionary:
	if not is_open():
		return _fail("disc pack is not open")
	var total: int = 0
	for record: Dictionary in _files:
		var stored: Dictionary = _verified_bytes(record)
		if not stored.ok:
			return stored
		total += int(record.stored_size)
	return {"ok": true, "files": _files.size(), "bytes": total}

# ---- internals

func _reset() -> void:
	root = ""
	manifest_sha256 = ""
	volume_id = ""
	_files = []
	_starts = PackedInt64Array()
	_by_path = {}
	_excluded = []
	_cached_path = ""
	_cached_bytes = PackedByteArray()

static func _fail(message: String) -> Dictionary:
	return {"ok": false, "error": message}

static func _in_ram(address: int, length: int) -> bool:
	if address < 0 or address > 0xffffffff or (address >> 29) not in [0, 4, 5]:
		return false
	return (address & 0x1fffffff) + length <= Memory.RAM_SIZE

func _index_of(lba: int) -> int:
	var low: int = 0
	var high: int = _starts.size() - 1
	var found: int = -1
	while low <= high:
		var middle: int = (low + high) >> 1
		if _starts[middle] <= lba:
			found = middle
			low = middle + 1
		else:
			high = middle - 1
	if found >= 0 and lba < _starts[found] + int(_files[found].sectors):
		return found
	return -1

func _outside_message(lba: int) -> String:
	var excluded: Dictionary = excluded_at(lba)
	if not excluded.is_empty():
		return "LBA 0x%X is in %s, which is not readable as data (%s)" % [lba, excluded.path, excluded.reason]
	return "LBA 0x%X is outside every file in the disc pack" % lba

## The stored bytes of `record`, read in one pass and hashed before any byte is used.
func _verified_bytes(record: Dictionary) -> Dictionary:
	var path: String = record.path
	if path == _cached_path:
		return {"ok": true, "data": _cached_bytes}
	var file: FileAccess = FileAccess.open(root.path_join(FILES_DIR).path_join(path), FileAccess.READ)
	if file == null:
		return _fail("packed file is missing or unreadable: " + path)
	var stored_size: int = int(record.stored_size)
	if file.get_length() != stored_size:
		return _fail("packed file %s has %d bytes, expected %d" % [path, file.get_length(), stored_size])
	var bytes: PackedByteArray = file.get_buffer(stored_size)
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(bytes)
	if bytes.size() != stored_size or hash.finish().hex_encode() != record.sha256:
		return _fail("packed file %s does not match its SHA-256" % path)
	_cached_path = path
	_cached_bytes = bytes
	return {"ok": true, "data": bytes}

func _load_manifest(manifest: Dictionary) -> String:
	if manifest.get("schema") != SCHEMA or not _integer(manifest.get("version")) or int(manifest.version) != 1:
		return "unsupported disc pack schema"
	if not _integer(manifest.get("sector_size")) or int(manifest.sector_size) != SECTOR_SIZE:
		return "disc pack sector size is not 2048"
	if not manifest.get("volume_id") is String or not manifest.get("files") is Array or not manifest.get("excluded") is Array:
		return "disc pack manifest is malformed"
	var files: Array = manifest.files
	if files.is_empty() or files.size() > MAX_FILES or manifest.excluded.size() > MAX_FILES:
		return "disc pack file count is outside bounds"
	var records: Array[Dictionary] = []
	var by_path: Dictionary = {}
	for item: Variant in files:
		var problem: String = _check_record(item)
		if not problem.is_empty():
			return problem
		var record: Dictionary = {}
		for key: String in RECORD_KEYS:
			record[key] = int(item[key]) if _integer(item[key]) else item[key]
		if by_path.has(record.path):
			return "disc pack lists %s twice" % record.path
		by_path[record.path] = record
		records.append(record)
	records.sort_custom(func(a: Dictionary, b: Dictionary) -> bool: return a.lba < b.lba)
	var starts := PackedInt64Array()
	for index: int in records.size():
		if index > 0 and int(records[index].lba) < int(records[index - 1].lba) + int(records[index - 1].sectors):
			return "disc pack extents of %s and %s overlap" % [records[index - 1].path, records[index].path]
		starts.append(int(records[index].lba))
	var excluded: Array[Dictionary] = []
	for item: Variant in manifest.excluded:
		if not item is Dictionary or item.size() != EXCLUDED_KEYS.size():
			return "disc pack excluded entry is malformed"
		for key: String in EXCLUDED_KEYS:
			if not item.has(key):
				return "disc pack excluded entry is malformed"
		if not item.path is String or not item.reason is String or not _integer(item.lba) \
				or not _integer(item.sectors) or not _integer(item.size):
			return "disc pack excluded entry is malformed"
		excluded.append({"path": item.path, "lba": int(item.lba), "sectors": int(item.sectors),
			"size": int(item.size), "reason": item.reason})
	_files = records
	_starts = starts
	_by_path = by_path
	_excluded = excluded
	volume_id = manifest.volume_id
	return ""

func _check_record(item: Variant) -> String:
	if not item is Dictionary or item.size() != RECORD_KEYS.size():
		return "disc pack file record is malformed"
	for key: String in RECORD_KEYS:
		if not item.has(key):
			return "disc pack file record is malformed"
	if not item.path is String or not _safe_path(item.path):
		return "disc pack file record has an unsafe path"
	for key: String in ["lba", "sectors", "size", "stored_size", "mode", "form"]:
		if not _integer(item[key]):
			return "disc pack record %s has a bad %s" % [item.path, key]
	if int(item.size) < 1 or int(item.sectors) != (int(item.size) + SECTOR_SIZE - 1) >> 11 \
			or int(item.stored_size) != int(item.sectors) * SECTOR_SIZE or int(item.stored_size) > MAX_FILE_BYTES:
		return "disc pack record %s has inconsistent sizes" % item.path
	if int(item.mode) != 2 or int(item.form) != 1:
		return "disc pack record %s is not a mode 2 form 1 data file" % item.path
	if not _sha(item.sha256) or not _sha(item.content_sha256):
		return "disc pack record %s has a malformed hash" % item.path
	return ""

static func _safe_path(path: String) -> bool:
	if path.is_empty() or path.begins_with("/") or path.contains("\\") or path.contains(":"):
		return false
	for component: String in path.split("/", true):
		if component in ["", ".", ".."]:
			return false
	return true

static func _integer(value: Variant) -> bool:
	return typeof(value) in [TYPE_INT, TYPE_FLOAT] and is_finite(float(value)) \
		and float(value) == floor(float(value)) and value >= 0 and value <= 0xffffffff

static func _sha(value: Variant) -> bool:
	if not value is String or value.length() != 64:
		return false
	for character: String in value:
		if character not in "0123456789abcdef":
			return false
	return true
