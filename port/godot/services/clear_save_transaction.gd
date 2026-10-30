extends RefCounted
## T-39007 (review R11): clear-save and album-save persistence, run by the app
## host between committed session steps (never inside a session callback).
##
## Source order, src/ovl/misc_scenes/staff_roll_and_clear_save.c
## clear_save_write_step: substep 0 adds 1 (cap 0x63) to
## girl_cleared[current_girl & 0xF] and system_data.flags[...] before any
## write; a failed write goes to step 0xFF and back to the card check, so a
## retry adds 1 again. The in-memory counters are never rolled back. This file
## keeps that order for the system counters. Completion is acknowledged only
## with a receipt built from a successful read-back.
##
## Port difference: the source also writes the clear into the selected game
## slot. The finished-ending state is not a stable save boundary of the port's
## session codec, so no game slot is written here.

const SystemRecords = preload("res://services/system_record_service.gd")
const SERVICE: String = "ending.clear_save"
const SAVED_GIRLS: int = 13

var records: SystemRecords
var record: Dictionary = {}
var load_error: String = ""
var handled_attempt: int = 0

func _init(root: String) -> void:
	records = SystemRecords.new(root)
	var loaded: Dictionary = records.load_record()
	load_error = loaded["error"]
	if load_error.is_empty():
		record = loaded["record"]

## Returns the completion payload for a pending clear-save request, or {} when
## the ending is not waiting for one. Each request attempt is written once.
func service_request(overlay_state: Dictionary) -> Dictionary:
	var request: Variant = overlay_state.get("clear_save_request")
	if overlay_state.get("clear_save_status") != "requested" or not request is Dictionary:
		return {}
	if not request.get("girl") is int or not request.get("attempt") is int or request["attempt"] <= handled_attempt:
		return {}
	handled_attempt = request["attempt"]
	return {"service": SERVICE, "payload": clear_save(request["girl"], request["attempt"])}

## One write attempt: increment first (source order), then publish.
func clear_save(girl: int, attempt: int) -> Dictionary:
	var failure: Dictionary = {"ok": false, "girl": girl & 0xF, "attempt": attempt}
	if not load_error.is_empty():
		failure["error"] = load_error + "; recover the system record from the title screen"
		return failure
	var index: int = girl & 0xF
	if record["girl_clear_counters"][index] < SystemRecords.CLEAR_COUNTER_CAP:
		record["girl_clear_counters"][index] += 1
	var published: Dictionary = records.publish(record.duplicate(true))
	if not published["error"].is_empty():
		failure["error"] = published["error"]
		return failure
	return {"ok": true, "girl": index, "attempt": attempt, "receipt": receipt_for(published["record"], index, published["checksum_sha256"])}

## Source album save (album_save_step_write substep 0, album_event_log_pack):
## clear the album area, copy every event-log record of current_girl & 0xF when
## the calendar date is set, store the u8 count and the saved flag, then write.
func album_save(girl: int, event_log: Variant, calendar: Array) -> Dictionary:
	if not load_error.is_empty():
		return {"error": load_error + "; recover the system record from the title screen"}
	if not event_log is Array or event_log.size() != SystemRecords.ALBUM_CAPACITY:
		return {"error": "album save needs the 256-record session event log; this session does not keep one"}
	var entries: Array = []
	var dated: bool = calendar.size() == 3 and calendar.all(func(value: Variant) -> bool: return value is int and value != 0)
	for entry: Variant in event_log:
		if not SystemRecords._bytes(entry, SystemRecords.ALBUM_RECORD_BYTES, 0xFF):
			return {"error": "event log record must be 8 bytes"}
		if dated and entry[0] == girl & 0xF:
			entries.append(entry.duplicate())
	record["album_entries"] = entries
	record["album_state"] = [1, entries.size() & 0xFF]
	var published: Dictionary = records.publish(record.duplicate(true))
	if not published["error"].is_empty():
		return published
	return {"error": "", "checksum_sha256": published["checksum_sha256"], "count": entries.size()}

static func receipt_for(saved: Dictionary, girl: int, checksum: String) -> Dictionary:
	var mask: int = 0
	for index: int in range(SAVED_GIRLS):
		if saved["girl_clear_counters"][index] > 0:
			mask |= 1 << index
	return {"service": SERVICE, "girl": girl, "counter": saved["girl_clear_counters"][girl],
		"album_clear_mask": mask, "checksum_sha256": checksum}

## Checks a receipt against the file on disk. A receipt not produced by a
## successful write of the current file is rejected.
func verify_receipt(receipt: Dictionary) -> String:
	if receipt.get("service") != SERVICE or not receipt.get("girl") is int or not receipt.get("checksum_sha256") is String:
		return "clear-save receipt is malformed"
	var loaded: Dictionary = records.storage.load_slot(SystemRecords.SLOT)
	if not loaded["error"].is_empty():
		return "clear-save receipt has no readable system record: " + loaded["error"]
	if loaded["checksum_sha256"] != receipt["checksum_sha256"]:
		return "clear-save receipt does not match the stored system record"
	var expected: Dictionary = receipt_for(loaded["snapshot"], receipt["girl"] & 0xF, loaded["checksum_sha256"])
	if expected != receipt:
		return "clear-save receipt fields differ from the stored system record"
	return ""
