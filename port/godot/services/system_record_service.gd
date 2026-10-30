extends RefCounted
## T-39007 (review R11): persistent port system record, the counterpart of
## memory-card file 0 (`SystemSaveImage`, wiki spec m-flow 6.5). Only the parts
## the port maintains are stored; every other key is rejected:
##   girl_clear_counters  system_data.flags[16], per-girl ending counters, cap
##                        0x63 (src/ovl/misc_scenes/staff_roll_and_clear_save.c
##                        clear_save_write_step)
##   album_state          system_data.album_state[0..1]: saved flag, u8 count
##   album_entries        packed album copy of the event log, up to 256 records
##                        of 8 bytes (src/ovl/misc_scenes/album_save_flow.c
##                        album_event_log_pack)
## Storage reuses SaveService: checksummed envelope, staged write, previous
## file kept as backup, explicit recovery.

const SaveService = preload("res://services/save_service.gd")
const FORMAT: String = "tokimemo.port-system"
const SLOT: String = "system"
const GIRL_COUNT: int = 16
const CLEAR_COUNTER_CAP: int = 0x63
const ALBUM_CAPACITY: int = 256
const ALBUM_RECORD_BYTES: int = 8
const KEYS: Array[String] = ["girl_clear_counters", "album_state", "album_entries"]

var storage: SaveService

func _init(root: String) -> void:
	storage = SaveService.new(root, FORMAT, validate)

static func default_record() -> Dictionary:
	var counters: Array = []
	counters.resize(GIRL_COUNT)
	counters.fill(0)
	return {"girl_clear_counters": counters, "album_state": [0, 0], "album_entries": []}

static func validate(record: Dictionary) -> String:
	if record.size() != KEYS.size() or not KEYS.all(func(key: String) -> bool: return record.has(key)):
		return "system record fields differ from the supported set"
	if not _bytes(record["girl_clear_counters"], GIRL_COUNT, CLEAR_COUNTER_CAP):
		return "girl clear counters must be 16 integers from 0 to 99"
	if not _bytes(record["album_state"], 2, 0xFF) or record["album_state"][0] not in [0, 1]:
		return "album state must be a saved flag and an 8-bit count"
	var entries: Variant = record["album_entries"]
	if not entries is Array or entries.size() > ALBUM_CAPACITY:
		return "album entries must be at most 256 records"
	for entry: Variant in entries:
		if not _bytes(entry, ALBUM_RECORD_BYTES, 0xFF):
			return "album entry must be 8 bytes"
	if record["album_state"][1] != entries.size() & 0xFF:
		return "album count differs from the stored entries"
	return ""

## Missing file: a fresh record ("new"). Invalid file: an error; the caller
## offers recovery and must not write over it.
func load_record() -> Dictionary:
	var report: Dictionary = storage.inspect_slot(SLOT)
	if not report["error"].is_empty():
		return report
	if report["primary"]["state"] == "missing":
		return {"error": "", "state": "new", "record": default_record(), "checksum_sha256": ""}
	var loaded: Dictionary = storage.load_slot(SLOT)
	if not loaded["error"].is_empty():
		return {"error": "system record is unreadable: " + loaded["error"]}
	return {"error": "", "state": "loaded", "record": loaded["snapshot"], "checksum_sha256": loaded["checksum_sha256"]}

## Publishes and reads the file back. Success is reported only when the read
## record equals what was written.
func publish(record: Dictionary) -> Dictionary:
	var saved: Dictionary = storage.save_slot(SLOT, record)
	if not saved["error"].is_empty():
		return saved
	var read: Dictionary = storage.load_slot(SLOT)
	if not read["error"].is_empty():
		return {"error": "system record read-back failed: " + read["error"]}
	if read["checksum_sha256"] != saved["checksum_sha256"] or read["snapshot"] != record:
		return {"error": "system record read-back differs from the written record"}
	return {"error": "", "checksum_sha256": saved["checksum_sha256"], "record": read["snapshot"]}

static func _bytes(value: Variant, count: int, maximum: int) -> bool:
	if not value is Array or value.size() != count:
		return false
	for item: Variant in value:
		if not item is int or item < 0 or item > maximum:
			return false
	return true
