extends RefCounted
## Virtual memory card: the 15 files the original keeps on a card (file 0 the
## system file, files 1-14 the albums), each the exact 0x2000-byte block that
## memory_card_file_write builds (services/memory_card_image.gd). The whole card
## is one record in the atomic SaveService envelope, so a write keeps the
## previous card as a backup and a failed write leaves it unchanged.
##
## The results are the source poll results (m-flow 6.2): check 1 / 0xF1 / 0xFF,
## scan 2, system read 3, save read 4, system write 0x13, save write 0x14, and
## 0xD0 when a file job cannot start. The frame latencies of the polls belong to
## the callers (as in flow/title_file_select.gd); this service answers at once.

const Save = preload("res://services/save_service.gd")
const Card = preload("res://services/memory_card_image.gd")

const FORMAT: String = "tokimemo.memory-card"
const SLOT: String = "memory-card-1"
const FILE_COUNT: int = 15
const RESULT_OK: int = 1
const RESULT_UNFORMATTED: int = 0xF1
const RESULT_NO_CARD: int = 0xFF
const RESULT_SCANNED: int = 2
const RESULT_SYSTEM_READ: int = 3
const RESULT_SAVE_READ: int = 4
const RESULT_SYSTEM_WRITTEN: int = 0x13
const RESULT_SAVE_WRITTEN: int = 0x14
const RESULT_FILE_ERROR: int = 0xD0

var _save: Save
## False models an empty card port (check result 0xFF).
var inserted: bool = true


func _init(storage_root: String) -> void:
	_save = Save.new(storage_root, FORMAT, Callable(get_script(), "record_error"))


## memory_card_check_poll's final result: 1 formatted, 0xF1 unformatted (no
## card record yet), 0xFF no card. A card record that exists but does not
## validate is reported as no card with recovery_required, never as
## unformatted, so the file screens cannot offer to format over it; the
## SaveService backup and set-aside paths recover it.
func check() -> Dictionary:
	if not inserted:
		return {"error": "", "result": RESULT_NO_CARD, "recovery_required": false}
	var inspected: Dictionary = _save.inspect_slot(SLOT)
	if not inspected.error.is_empty():
		return {"error": inspected.error, "result": RESULT_NO_CARD, "recovery_required": true}
	if not inspected.staging.is_empty():
		return {"error": "interrupted card write", "result": RESULT_NO_CARD, "recovery_required": true}
	if inspected.primary.state == "missing":
		return {"error": "", "result": RESULT_UNFORMATTED, "recovery_required": false}
	var card: Dictionary = _load()
	if not card.error.is_empty():
		return {"error": card.error, "result": RESULT_NO_CARD, "recovery_required": true}
	return {"error": "", "result": RESULT_OK if card.record.formatted else RESULT_UNFORMATTED,
		"recovery_required": false}


## memory_card_scan_poll (src/main/memory_card.c:326). Step 0 tests file 0 up
## to three times and sets only slot_info_from_system_data (1 found, 0 after
## the third miss); steps 1..14 write slot_status[1..14]. slot_status[0] and
## [15] are never written by the scan: the caller's 16 entries are returned
## with only [1..14] replaced (memory_card_state_reset clears [0..14] before).
## A virtual card answers every try the same, so a missing file 0 takes 3.
func scan(slot_status: Array) -> Dictionary:
	if slot_status.size() != 16 or not slot_status.all(func(v: Variant) -> bool: return v is int and v >= 0 and v <= 0xFF):
		return {"error": "scan needs the caller's 16 slot_status bytes", "result": RESULT_FILE_ERROR}
	var card: Dictionary = _ready_card()
	if not card.error.is_empty():
		return card
	var system_found: bool = card.record.files.has("0")
	var status: Array = slot_status.duplicate()
	for i in range(1, FILE_COUNT):
		status[i] = 1 if card.record.files.has(str(i)) else 0
	return {"error": "", "result": RESULT_SCANNED, "slot_status": status,
		"slot_info_from_system_data": 1 if system_found else 0, "system_file_tries": 1 if system_found else 3}


## memory_card_system_read_poll / memory_card_save_read_poll.
func read_file(file_index: int) -> Dictionary:
	var card: Dictionary = _ready_card()
	if not card.error.is_empty():
		return card
	var hex: Variant = card.record.files.get(str(file_index))
	if file_index < 0 or file_index >= FILE_COUNT or hex == null:
		return {"error": "file %d cannot be opened" % file_index, "result": RESULT_FILE_ERROR}
	return {"error": "", "result": RESULT_SYSTEM_READ if file_index == 0 else RESULT_SAVE_READ,
		"block": (hex as String).hex_decode()}


## memory_card_system_write_poll / memory_card_save_write_poll. The writer
## creates a missing file (memory_card_file_write); the block must carry the
## game's header and title for that file.
func write_file(file_index: int, block: PackedByteArray) -> Dictionary:
	var card: Dictionary = _ready_card()
	if not card.error.is_empty():
		return card
	if file_index < 0 or file_index >= FILE_COUNT:
		return {"error": "file index outside 0..14", "result": RESULT_FILE_ERROR}
	var header: String = Card.file_header_error(block, file_index)
	if not header.is_empty():
		return {"error": header, "result": RESULT_FILE_ERROR}
	var record: Dictionary = card.record.duplicate(true)
	record.files[str(file_index)] = block.hex_encode()
	var saved: Dictionary = _store(record)
	if not saved.error.is_empty():
		return {"error": saved.error, "result": RESULT_FILE_ERROR}
	return {"error": "", "result": RESULT_SYSTEM_WRITTEN if file_index == 0 else RESULT_SAVE_WRITTEN}


## memory_card_file_create: a one-block file that holds zeros until written.
func create_file(file_index: int) -> Dictionary:
	var card: Dictionary = _ready_card()
	if not card.error.is_empty():
		return card
	if file_index < 0 or file_index >= FILE_COUNT:
		return {"error": "file index outside 0..14", "result": -1}
	var record: Dictionary = card.record.duplicate(true)
	if not record.files.has(str(file_index)):
		var empty := PackedByteArray()
		empty.resize(Card.BLOCK_SIZE)
		record.files[str(file_index)] = empty.hex_encode()
	var saved: Dictionary = _store(record)
	return {"error": saved.error, "result": 1 if saved.error.is_empty() else -1}


## memory_card_file_delete.
func delete_file(file_index: int) -> Dictionary:
	var card: Dictionary = _ready_card()
	if not card.error.is_empty():
		return card
	var record: Dictionary = card.record.duplicate(true)
	if not record.files.erase(str(file_index)):
		return {"error": "file %d does not exist" % file_index, "result": -1}
	var saved: Dictionary = _store(record)
	return {"error": saved.error, "result": 1 if saved.error.is_empty() else -1}


## memory_card_format: an empty formatted card.
func format() -> Dictionary:
	var state: Dictionary = check()
	if state.result == RESULT_NO_CARD:
		return {"error": state.error if not state.error.is_empty() else "no card", "result": RESULT_NO_CARD}
	var saved: Dictionary = _store({"formatted": true, "files": {}})
	return {"error": saved.error, "result": 1 if saved.error.is_empty() else -1}


## Copies the files of a raw card image the original wrote (.mcd) onto this
## card. Only the game's own files are taken, and each must carry its header.
func import_raw_card(raw: PackedByteArray) -> Dictionary:
	if not inserted:
		return {"error": "no card", "result": RESULT_NO_CARD}
	var state: Dictionary = check()
	if state.recovery_required:
		return {"error": "card record needs recovery: " + String(state.error), "result": RESULT_NO_CARD, "recovery_required": true}
	var listing: Dictionary = Card.raw_card_files(raw)
	if not listing.error.is_empty():
		return listing
	var files: Dictionary = {}
	for i in FILE_COUNT:
		var name: String = Card.file_name(i)
		if listing.files.has(name):
			var header: String = Card.file_header_error(listing.files[name], i)
			if not header.is_empty():
				return {"error": "%s: %s" % [name, header]}
			files[str(i)] = (listing.files[name] as PackedByteArray).hex_encode()
	return _store({"formatted": true, "files": files})


## A missing or unformatted record answers 0xF1; a damaged, unreadable or
## interrupted record answers 0xFF with recovery_required, as check() does.
func _ready_card() -> Dictionary:
	var state: Dictionary = check()
	if state.result != RESULT_OK:
		var reason: String = String(state.error)
		if reason.is_empty():
			reason = "no card" if state.result == RESULT_NO_CARD else "card is not formatted"
		return {"error": reason, "result": state.result, "recovery_required": state.recovery_required}
	return _load()


func _load() -> Dictionary:
	var loaded: Dictionary = _save.load_slot(SLOT)
	if not loaded.error.is_empty():
		return {"error": loaded.error}
	return {"error": "", "record": loaded.snapshot}


func _store(record: Dictionary) -> Dictionary:
	return _save.save_slot(SLOT, record)


static func record_error(record: Dictionary) -> String:
	if record.size() != 2 or not record.get("formatted") is bool or not record.get("files") is Dictionary:
		return "memory card record must hold formatted and files"
	for key: Variant in record.files:
		if not key is String or not (key as String).is_valid_int() or int(key) < 0 or int(key) >= FILE_COUNT or str(int(key)) != key:
			return "memory card file key must be 0..14"
		var hex: Variant = record.files[key]
		if not hex is String or (hex as String).length() != Card.BLOCK_SIZE * 2 or not _is_hex(hex):
			return "memory card file must be 0x2000 bytes"
	if not record.formatted and not record.files.is_empty():
		return "an unformatted card holds no files"
	return ""


static func _is_hex(text: String) -> bool:
	for c: int in text.to_ascii_buffer():
		if not (c >= 48 and c <= 57 or c >= 97 and c <= 102):
			return false
	return true
