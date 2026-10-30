extends SceneTree
## T-39007 (review R12): explicit backup recovery, interrupted-save handling and
## system-record validation on real files. Snapshots are invented codec
## fixtures; no original game data is used.
const Save = preload("res://services/save_service.gd")
const SystemRecords = preload("res://services/system_record_service.gd")
const ClearSave = preload("res://services/clear_save_transaction.gd")
var failures: Array[String] = []
var checks: int = 0

func _initialize() -> void:
	var root: String = "/tmp/t39007-recovery-" + str(Time.get_ticks_usec())
	_backup_recovery(root.path_join("backup"))
	_staging(root.path_join("staging"))
	_damaged_without_backup(root.path_join("damaged"))
	_system_record(root.path_join("system"))
	_system_record_backup(root.path_join("system-backup"))
	if failures.is_empty():
		print("PASS: save recovery suite; %d checks" % checks)
		quit(0)
	else:
		for failure: String in failures:
			printerr("FAIL: ", failure)
		quit(1)

func _backup_recovery(root: String) -> void:
	var service: Save = Save.new(root)
	var first: Dictionary = _fixture(1)
	var second: Dictionary = _fixture(2)
	_check(service.save_slot("quick", first)["error"].is_empty() and service.save_slot("quick", second)["error"].is_empty(), "two saves publish a backup")
	_check(not service.recover_from_backup("quick")["error"].is_empty(), "recovery refuses while the current save is valid")
	var corrupt: PackedByteArray = FileAccess.get_file_as_bytes(root.path_join("quick.json")).slice(0, 40)
	_write(root.path_join("quick.json"), corrupt)
	var report: Dictionary = service.inspect_slot("quick")
	_check(report["primary"]["state"] == "invalid" and report["backup"]["state"] == "valid", "inspection reports damaged save and valid backup")
	_check(service.save_slot("quick", second)["error"].contains("explicit backup recovery"), "saving over a damaged file is refused")
	var backup_bytes: PackedByteArray = FileAccess.get_file_as_bytes(root.path_join("quick.json.bak"))
	var recovered: Dictionary = service.recover_from_backup("quick")
	_check(recovered["error"].is_empty(), "explicit recovery succeeds")
	_check(service.load_slot("quick")["snapshot"] == first, "recovered save is the backup snapshot")
	_check(FileAccess.get_file_as_bytes(root.path_join("quick.json.bak")) == backup_bytes, "backup bytes are retained")
	_check(FileAccess.get_file_as_bytes(root.path_join("quick.json.invalid-1")) == corrupt, "damaged save is set aside unchanged")
	_check(service.save_slot("quick", second)["error"].is_empty(), "saving works after recovery")

func _staging(root: String) -> void:
	var service: Save = Save.new(root)
	_check(service.save_slot("quick", _fixture(1))["error"].is_empty(), "staging fixture save")
	var stale: PackedByteArray = "interrupted".to_utf8_buffer()
	_write(root.path_join("quick.json.tmp"), stale)
	_check(service.inspect_slot("quick")["staging"] == ["quick.json.tmp"], "inspection lists interrupted files")
	_check(service.save_slot("quick", _fixture(2))["error"] == "save staging path already exists", "interrupted file blocks saving")
	_check(not service.recover_from_backup("quick")["error"].is_empty(), "recovery refuses while interrupted files exist")
	var moved: Dictionary = service.set_aside_staging("quick")
	_check(moved["error"].is_empty() and moved["moved"] == ["quick.json.tmp.interrupted-1"], "interrupted file is set aside")
	_check(FileAccess.get_file_as_bytes(root.path_join("quick.json.tmp.interrupted-1")) == stale, "set-aside bytes are retained")
	_check(not service.set_aside_staging("quick")["error"].is_empty(), "set-aside with nothing to move is refused")
	_check(service.save_slot("quick", _fixture(2))["error"].is_empty(), "saving works after setting aside")
	for index: int in range(2, Save.MAX_SET_ASIDE + 1):
		_write(root.path_join("quick.json.tmp.interrupted-%d" % index), stale)
	_write(root.path_join("quick.json.tmp"), stale)
	_check(service.set_aside_staging("quick")["error"].contains("too many"), "set-aside count is bounded")
	_check(FileAccess.file_exists(root.path_join("quick.json.tmp")), "bounded refusal leaves the file in place")

func _damaged_without_backup(root: String) -> void:
	var service: Save = Save.new(root)
	_check(service.save_slot("quick", _fixture(1))["error"].is_empty(), "single save without backup")
	_check(not service.set_aside_invalid("quick")["error"].is_empty(), "a valid save is never set aside")
	_write(root.path_join("quick.json"), "{".to_utf8_buffer())
	_check(not service.recover_from_backup("quick")["error"].is_empty(), "recovery without backup is refused")
	_check(service.set_aside_invalid("quick")["error"].is_empty(), "damaged save without backup is set aside")
	_check(FileAccess.file_exists(root.path_join("quick.json.invalid-1")) and not FileAccess.file_exists(root.path_join("quick.json")), "damaged save keeps its bytes under a new name")
	_check(service.save_slot("quick", _fixture(2))["error"].is_empty(), "saving works after setting the damaged save aside")

func _system_record(root: String) -> void:
	var records: SystemRecords = SystemRecords.new(root)
	_check(records.load_record()["state"] == "new", "missing system record loads as new")
	var good: Dictionary = SystemRecords.default_record()
	good["girl_clear_counters"][3] = 2
	_check(records.publish(good)["error"].is_empty(), "valid system record publishes")
	var cases: Dictionary = {
		"extra field": func(r: Dictionary) -> void: r["all_endings_cleared"] = true,
		"counter above cap": func(r: Dictionary) -> void: r["girl_clear_counters"][0] = 100,
		"negative counter": func(r: Dictionary) -> void: r["girl_clear_counters"][0] = -1,
		"short counters": func(r: Dictionary) -> void: r["girl_clear_counters"].pop_back(),
		"boolean counter": func(r: Dictionary) -> void: r["girl_clear_counters"][0] = true,
		"album flag": func(r: Dictionary) -> void: r["album_state"] = [2, 0],
		"album count mismatch": func(r: Dictionary) -> void: r["album_state"] = [1, 3],
		"album entry size": func(r: Dictionary) -> void: r["album_entries"] = [[0, 1, 2]],
		"missing field": func(r: Dictionary) -> void: r.erase("album_entries"),
	}
	var before: PackedByteArray = FileAccess.get_file_as_bytes(root.path_join("system.json"))
	for name: String in cases:
		var bad: Dictionary = good.duplicate(true)
		cases[name].call(bad)
		_check(not records.publish(bad)["error"].is_empty(), "system record rejects " + name)
	_check(FileAccess.get_file_as_bytes(root.path_join("system.json")) == before, "rejected records leave the file unchanged")
	# A hand-edited file with a fabricated field fails its checksum and schema.
	var text: String = FileAccess.get_file_as_string(root.path_join("system.json"))
	_write(root.path_join("system.json"), text.replace("girl_clear_counters", "girl_clear_counterz").to_utf8_buffer())
	_check(not records.load_record()["error"].is_empty(), "edited system record is rejected on load")
	var host: ClearSave = ClearSave.new(root)
	_check(not host.load_error.is_empty(), "clear-save host reports the unreadable record")
	var refused: Dictionary = host.clear_save(0, 1)
	_check(refused["ok"] == false and refused["error"].contains("recover"), "clear save over an unreadable record fails with a recovery hint")
	# Only one record was published, so there is no backup to restore.
	_check(not records.storage.recover_from_backup(SystemRecords.SLOT)["error"].is_empty(), "system record recovery without a backup is refused")
	_check(records.storage.set_aside_invalid(SystemRecords.SLOT)["error"].is_empty()
		and FileAccess.file_exists(root.path_join("system.json.invalid-1")), "damaged system record without a backup is set aside")
	_check(records.load_record()["state"] == "new", "system record starts new after the damaged one is set aside")

func _system_record_backup(root: String) -> void:
	var records: SystemRecords = SystemRecords.new(root)
	var first: Dictionary = SystemRecords.default_record()
	first["girl_clear_counters"][2] = 1
	var second: Dictionary = first.duplicate(true)
	second["girl_clear_counters"][2] = 2
	_check(records.publish(first)["error"].is_empty() and records.publish(second)["error"].is_empty(), "two system records publish")
	_write(root.path_join("system.json"), "{".to_utf8_buffer())
	_check(not records.load_record()["error"].is_empty(), "corrupted system record is rejected on load")
	_check(records.storage.recover_from_backup(SystemRecords.SLOT)["error"].is_empty(), "system record recovers from its backup")
	var restored: Dictionary = records.load_record()
	_check(restored["error"].is_empty() and restored["record"]["girl_clear_counters"][2] == 1, "recovery restores the earlier system record")
	_check(FileAccess.file_exists(root.path_join("system.json.invalid-1")), "corrupted system record is kept under a new name")

func _fixture(marker: int) -> Dictionary:
	return {"boundary": "stable", "source_version": "src-a", "content_version": "content-a", "behavior_version": "behavior-a",
		"gameplay_state": {"marker": marker, "ordered": {"b": 2, "a": 1}}, "native_rng": {"state": 1}, "native_clocks": {"vblank": 2},
		"queued_service_events": []}

func _write(path: String, bytes: PackedByteArray) -> void:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	file.store_buffer(bytes)
	file.close()

func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)
