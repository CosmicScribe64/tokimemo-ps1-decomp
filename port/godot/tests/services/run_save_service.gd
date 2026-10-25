extends SceneTree
## Invented codec/filesystem fixtures. Actual controller continuation is separate.
const Save = preload("res://services/save_service.gd")
var failures: Array[String] = []
var checks: int = 0

func _initialize() -> void:
	var root: String = "/tmp/t29006-codec-" + str(Time.get_ticks_usec())
	var service: Save = Save.new(root)
	var first: Dictionary = _fixture()
	_check(service.save_slot("slot", first)["error"].is_empty(), "initial snapshot saves")
	_check(JSON.stringify(service.load_slot("slot")["snapshot"]) == JSON.stringify(first), "all exact integers and dictionary insertion orders round-trip")
	var second: Dictionary = first.duplicate(true)
	second["gameplay_state"]["ordered"]["z"] = 9
	_check(service.save_slot("slot", second)["error"].is_empty(), "atomic replacement saves")
	_check(service.save_slot("slot", first)["error"].is_empty(), "atomic replacement can replace an existing backup")
	_check(service.load_slot("slot", true)["snapshot"] == second, "backup is the previous validated save")
	var bytes: PackedByteArray = FileAccess.get_file_as_bytes(root.path_join("slot.json"))
	for id: String in ["../outside", "/absolute", "slot/child", "slot..", "", "x".repeat(33)]:
		_check(not service.save_slot(id, first)["error"].is_empty(), "unsafe slot refused")
	var bad_root: Save = Save.new(root + "/../escape")
	_check(not bad_root.save_slot("slot", first)["error"].is_empty(), "unnormalized root refused")
	var virtual_root: Save = Save.new("res://unsafe-save-root")
	_check(not virtual_root.save_slot("slot", first)["error"].is_empty(), "engine resource save root refused")
	var bad: Dictionary = first.duplicate(true)
	bad["gameplay_state"]["float"] = 0.5
	_check(not service.save_slot("slot", bad)["error"].is_empty(), "float value refused")
	bad = first.duplicate(true)
	bad["gameplay_state"]["object"] = RefCounted.new()
	_check(not service.save_slot("slot", bad)["error"].is_empty(), "engine object refused")
	bad = first.duplicate(true)
	bad["boundary"] = "anywhere"
	_check(not service.save_slot("slot", bad)["error"].is_empty(), "unstable boundary refused")
	bad = first.duplicate(true)
	bad["gameplay_state"]["too_long"] = "a".repeat(65537)
	_check(not service.save_slot("slot", bad)["error"].is_empty(), "structural string bound enforced")
	bad = first.duplicate(true)
	bad["extra"] = 0
	_check(not service.save_slot("slot", bad)["error"].is_empty(), "unknown snapshot fields refused")
	_check(FileAccess.get_file_as_bytes(root.path_join("slot.json")) == bytes, "invalid saves leave current bytes unchanged")
	_write(root.path_join("slot.json.tmp"), "stale staging sentinel".to_utf8_buffer())
	_check(not service.save_slot("slot", first)["error"].is_empty() and FileAccess.get_file_as_bytes(root.path_join("slot.json")) == bytes,
		"existing staging file refuses transaction without truncating current save")
	DirAccess.remove_absolute(root.path_join("slot.json.tmp"))
	DirAccess.remove_absolute(root.path_join("slot.json.bak"))
	DirAccess.make_dir_absolute(root.path_join("slot.json.bak"))
	_check(not service.save_slot("slot", first)["error"].is_empty() and FileAccess.get_file_as_bytes(root.path_join("slot.json")) == bytes,
		"unpublishable backup path leaves current save unchanged")
	DirAccess.remove_absolute(root.path_join("slot.json.bak"))
	var dir: DirAccess = DirAccess.open(root)
	var link_error: Error = dir.create_link(root.path_join("slot.json"), root.path_join("linked.json"))
	_check(link_error == OK and dir.is_link("linked.json"), "synthetic symlink fixture is created")
	_check(not service.load_slot("linked")["error"].is_empty(), "linked save cannot be read")
	_check(not service.save_slot("linked", first)["error"].is_empty(), "linked save cannot be overwritten")
	_write(root.path_join("slot.json"), "{\"body\":".to_utf8_buffer())
	_check(not service.load_slot("slot")["error"].is_empty(), "truncated JSON refused")
	_check(not service.save_slot("slot", first)["error"].is_empty(), "corrupt current save cannot replace a known-good backup")
	_write(root.path_join("slot.json"), bytes)
	var parser: JSON = JSON.new()
	parser.parse(bytes.get_string_from_utf8())
	var envelope: Dictionary = parser.data
	envelope["body"]["version"] = "999"
	_write(root.path_join("slot.json"), service._canonical(envelope).to_utf8_buffer())
	_check(not service.load_slot("slot")["error"].is_empty(), "unknown version refused")
	parser.parse(bytes.get_string_from_utf8())
	envelope = parser.data
	envelope["checksum_sha256"] = "0".repeat(64)
	_write(root.path_join("slot.json"), service._canonical(envelope).to_utf8_buffer())
	_check(service.load_slot("slot")["error"].contains("checksum"), "checksum tamper refused")
	for value: String in ["9223372036854775808", "-9223372036854775809", "00", "-0", "+1", "1e3"]:
		_check(not service._unpack({"i64": value}, 0, [0])["error"].is_empty(), "noncanonical/overflow exact integer refused")
	_check(not service._unpack({"ordered_map": [["x", null], ["x", null]]}, 0, [0])["error"].is_empty(), "duplicate ordered-map keys refused")
	_check(not service._unpack(123.0, 0, [0])["error"].is_empty(), "untagged JSON numeric value refused")
	# Structure overflow is tested independently of the byte-size cap.
	var deep: Variant = null
	for index: int in range(50):
		deep = [deep]
	_check(not service._pack(deep, 0, [0])["error"].is_empty(), "nesting bound enforced")
	if failures.is_empty():
		print("PASS: save codec/filesystem suite; %d checks" % checks)
		quit(0)
	else:
		for message: String in failures:
			push_error(message)
		quit(1)

func _fixture() -> Dictionary:
	return {"boundary": "stable", "source_version": "invented-source-v1", "content_version": "invented-content-v1",
		"behavior_version": "invented-behavior-v1", "gameplay_state": {"ordered": {"z": 1, "a": 2},
			"extremes": [9223372036854775807, -9223372036854775807 - 1, 9007199254740993]},
		"native_rng": {"profile": "invented-only", "state": 0}, "native_clocks": {"logic": 0}, "queued_service_events": []}

func _write(path: String, bytes: PackedByteArray) -> void:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	file.store_buffer(bytes)
	file.close()

func _check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
