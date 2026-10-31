extends SceneTree
## Virtual memory card service: source poll results, atomic storage, relaunch,
## damaged-record refusal, and import of the original-written golden card
## (TOKIMEMO_MEMCARD_GOLDEN, optional).
const Service = preload("res://services/memory_card_service.gd")
const Card = preload("res://services/memory_card_image.gd")

var failures: Array[String] = []
var checks: int = 0


func _initialize() -> void:
	var root: String = "/tmp/memory-card-service-" + str(Time.get_ticks_usec())
	var card := Service.new(root)
	_check(card.check().result == Service.RESULT_UNFORMATTED, "a new card record is unformatted (0xF1)")
	_check(card.scan(_zero16()).result == Service.RESULT_UNFORMATTED, "scan refuses an unformatted card")
	card.inserted = false
	_check(card.check().result == Service.RESULT_NO_CARD and card.format().result == Service.RESULT_NO_CARD, "no card answers 0xFF and cannot be formatted")
	card.inserted = true
	_check(card.format().result == 1 and card.check().result == Service.RESULT_OK, "format gives a formatted card (1)")
	var scan: Dictionary = card.scan(_zero16())
	_check(scan.result == Service.RESULT_SCANNED and scan.slot_status.size() == 16 and scan.slot_status.max() == 0, "empty card scans to all zero")
	_check(scan.slot_info_from_system_data == 0 and scan.system_file_tries == 3, "a missing system file clears slot_info_from_system_data after three tries")
	_check(card.scan([0]).result == Service.RESULT_FILE_ERROR, "scan refuses a short slot_status")
	_check(card.read_file(1).result == Service.RESULT_FILE_ERROR, "reading a missing file is 0xD0")

	var state := PackedByteArray()
	state.resize(0x1A90)
	state[0x3E] = 96
	state[0x3F] = 5
	state[0x40] = 1
	var built: Dictionary = Card.file_build(3, state, _icons(), {"k_speed": 1, "cursor_style": 0})
	_check(built.error.is_empty(), "codec builds album 3")
	_check(card.write_file(4, built.block).result == Service.RESULT_FILE_ERROR, "album 3 cannot be written as file 4")
	_check(card.write_file(3, built.block).result == Service.RESULT_SAVE_WRITTEN, "album write answers 0x14")
	_check(card.scan(_zero16()).slot_status[3] == 1, "scan sees the new file")
	var back: Dictionary = card.read_file(3)
	_check(back.result == Service.RESULT_SAVE_READ and back.block == built.block, "save read answers 4 with the same bytes")
	var target := PackedByteArray()
	target.resize(0x1A90)
	_check(Card.game_bank_load(back.block, target).result == 1 and target[0x3F] == 5, "the read block loads through the codec")

	# Relaunch: a new service instance on the same storage sees the same card.
	var again := Service.new(root)
	_check(again.check().result == Service.RESULT_OK and again.read_file(3).block == built.block, "card survives a relaunch")
	state[0x3F] = 6
	var second: Dictionary = Card.file_build(3, state, _icons(), {"k_speed": 1, "cursor_style": 0})
	_check(again.write_file(3, second.block).result == Service.RESULT_SAVE_WRITTEN, "overwrite succeeds")
	_check(FileAccess.file_exists(root.path_join(Service.SLOT + ".json.bak")), "overwrite keeps the previous card as a backup")
	_check(again.delete_file(3).result == 1 and again.scan(_zero16()).slot_status[3] == 0, "delete removes the file")
	_check(again.delete_file(3).result == -1, "deleting a missing file fails")
	_check(again.create_file(5).result == 1 and again.scan(_zero16()).slot_status[5] == 1, "create makes a one-block file")

	# A damaged card record is never reported as unformatted.
	var path: String = root.path_join(Service.SLOT + ".json")
	var bytes: PackedByteArray = FileAccess.get_file_as_bytes(path)
	bytes[bytes.size() / 2] ^= 0x01
	var f := FileAccess.open(path, FileAccess.WRITE)
	f.store_buffer(bytes)
	f.close()
	var damaged: Dictionary = again.check()
	_check(damaged.result == Service.RESULT_NO_CARD and damaged.recovery_required, "damaged record needs recovery, not a format")
	_check(again.format().result == Service.RESULT_NO_CARD, "format refuses to overwrite a damaged record")
	for call: Dictionary in [again.scan(_zero16()), again.read_file(5), again.write_file(3, second.block), again.create_file(6), again.delete_file(5)]:
		_check(call.result == Service.RESULT_NO_CARD and call.get("recovery_required") == true,
			"every card call on a damaged record answers 0xFF with recovery_required, never 0xF1")
	_check(again.import_raw_card(PackedByteArray()).get("recovery_required") == true, "import refuses to replace a damaged record")
	_check(FileAccess.get_file_as_bytes(path) == bytes, "the damaged record is left untouched")

	# Validator negatives.
	_check(not Service.record_error({"formatted": true, "files": {"15": ""}}).is_empty(), "file 15 refused")
	_check(not Service.record_error({"formatted": true, "files": {"1": "zz"}}).is_empty(), "short or non-hex file refused")
	_check(not Service.record_error({"formatted": false, "files": {"1": "00".repeat(0x2000)}}).is_empty(), "unformatted card with files refused")
	_check(not Service.record_error({"formatted": true}).is_empty(), "missing files refused")

	var golden_dir: String = OS.get_environment("TOKIMEMO_MEMCARD_GOLDEN")
	if golden_dir.is_empty():
		print("SKIP golden import: TOKIMEMO_MEMCARD_GOLDEN is not set")
	else:
		var raw: PackedByteArray = FileAccess.get_file_as_bytes(golden_dir.path_join("memcard1.mcd"))
		var imported := Service.new(root + "-golden")
		_check(imported.import_raw_card(raw).error.is_empty(), "golden card imports")
		var marker: Array = _zero16()
		marker[0] = 0x5A
		marker[15] = 0xA5
		var scanned: Dictionary = imported.scan(marker)
		var s: Array = scanned.slot_status
		_check(s[0] == 0x5A and s[15] == 0xA5, "scan leaves slot_status[0] and [15] as the caller had them")
		_check(s[1] == 1 and s.slice(2, 15).max() == 0 and scanned.slot_info_from_system_data == 1 and scanned.system_file_tries == 1,
			"imported card: album 1 in slot_status, the system file in slot_info_from_system_data")
		var no_card := Service.new(root + "-golden-absent")
		no_card.inserted = false
		_check(no_card.import_raw_card(raw).result == Service.RESULT_NO_CARD, "import needs an inserted card")
		var listing: Dictionary = Card.raw_card_files(raw)
		_check(imported.read_file(1).block == listing.files[Card.file_name(1)] and imported.read_file(0).result == Service.RESULT_SYSTEM_READ,
			"imported files read back byte for byte")

	if failures.is_empty():
		print("PASS: memory card service %d checks" % checks)
		quit(0)
	else:
		for message: String in failures:
			push_error(message)
		print("FAIL: memory card service %d of %d checks" % [failures.size(), checks])
		quit(1)


func _zero16() -> Array:
	var a: Array = []
	a.resize(16)
	a.fill(0)
	return a


func _icons() -> Dictionary:
	var palettes := PackedByteArray()
	palettes.resize(Card.ICON_COUNT * Card.ICON_PALETTE_SIZE)
	var images := PackedByteArray()
	images.resize(Card.ICON_COUNT * Card.ICON_IMAGE_SIZE)
	return {"palettes": palettes, "images": images}


func _check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
