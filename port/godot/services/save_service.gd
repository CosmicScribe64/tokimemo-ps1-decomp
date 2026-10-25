extends RefCounted
## T-29006 version 2: exact integers and ordered data records. No engine objects.
## Storage runs outside session callbacks. Rename publication is not directory fsync.

const FORMAT: String = "tokimemo.port-save"
const VERSION: String = "2"
const MAX_BYTES: int = 8 * 1024 * 1024
const MAX_NODES: int = 250000
const MAX_DEPTH: int = 48
var storage_root: String

func _init(root: String) -> void:
	storage_root = root

func validate_snapshot(snapshot: Dictionary) -> String:
	var budget: Array[int] = [0]
	var packed: Dictionary = _pack(snapshot, 0, budget)
	return packed["error"] if not packed["error"].is_empty() else _snapshot_error(snapshot)

func save_slot(slot: String, snapshot: Dictionary) -> Dictionary:
	var paths: Dictionary = _paths(slot)
	if not paths["error"].is_empty():
		return paths
	var budget: Array[int] = [0]
	var checked: Dictionary = _pack(snapshot, 0, budget)
	if not checked["error"].is_empty():
		return checked
	var validation: String = _snapshot_error(snapshot)
	if not validation.is_empty():
		return _fail(validation)
	var body: Dictionary = {"format": FORMAT, "version": VERSION, "payload": checked["value"]}
	var checksum: String = _canonical(body).sha256_text()
	var encoded: String = _canonical({"body": body, "checksum_sha256": checksum})
	if encoded.to_utf8_buffer().size() > MAX_BYTES:
		return _fail("save exceeds size limit")
	if DirAccess.make_dir_recursive_absolute(storage_root) != OK:
		return _fail("cannot create save directory")
	# Recheck after directory creation and refuse existing staging paths.
	paths = _paths(slot)
	if not paths["error"].is_empty():
		return paths
	for path: String in [paths["temp"], paths["backup_temp"]]:
		if FileAccess.file_exists(path) or DirAccess.dir_exists_absolute(path):
			return _fail("save staging path already exists")
	var previous: Dictionary = {}
	if FileAccess.file_exists(paths["save"]):
		previous = _read(paths["save"])
		if not previous["error"].is_empty():
			return _fail("existing save is invalid; use explicit backup recovery")
	if not _write(paths["temp"], encoded.to_utf8_buffer()):
		DirAccess.remove_absolute(paths["temp"])
		return _fail("temporary save write failed")
	var staged: Dictionary = _read(paths["temp"])
	if not staged["error"].is_empty() or staged.get("checksum_sha256", "") != checksum:
		DirAccess.remove_absolute(paths["temp"])
		return _fail("temporary save validation failed")
	if not previous.is_empty():
		var bytes: PackedByteArray = FileAccess.get_file_as_bytes(paths["save"])
		if not _write(paths["backup_temp"], bytes):
			_clean_staging(paths)
			return _fail("previous save backup write failed")
		var backup: Dictionary = _read(paths["backup_temp"])
		if not backup["error"].is_empty() or backup.get("checksum_sha256", "") != previous["checksum_sha256"]:
			_clean_staging(paths)
			return _fail("previous save backup validation failed")
		if DirAccess.rename_absolute(paths["backup_temp"], paths["backup"]) != OK:
			_clean_staging(paths)
			return _fail("previous save backup publication failed")
	if DirAccess.rename_absolute(paths["temp"], paths["save"]) != OK:
		_clean_staging(paths)
		return _fail("save publication failed; previous slot retained")
	return {"error": "", "checksum_sha256": checksum}

func load_slot(slot: String, backup: bool = false) -> Dictionary:
	var paths: Dictionary = _paths(slot)
	if not paths["error"].is_empty():
		return paths
	return _read(paths["backup"] if backup else paths["save"])

func _read(path: String) -> Dictionary:
	var file: FileAccess = FileAccess.open(path, FileAccess.READ)
	if file == null:
		return _fail("save file is unavailable")
	var length: int = file.get_length()
	if length <= 0 or length > MAX_BYTES:
		file.close()
		return _fail("save file size is invalid")
	var bytes: PackedByteArray = file.get_buffer(length)
	var read_ok: bool = bytes.size() == length and file.get_error() == OK
	file.close()
	if not read_ok:
		return _fail("save file is truncated")
	var parser: JSON = JSON.new()
	if parser.parse(bytes.get_string_from_utf8()) != OK or not parser.data is Dictionary:
		return _fail("save JSON is malformed or truncated")
	var envelope: Dictionary = parser.data
	if not _keys(envelope, ["body", "checksum_sha256"]) or not envelope["body"] is Dictionary or not envelope["checksum_sha256"] is String:
		return _fail("save envelope schema differs")
	var body: Dictionary = envelope["body"]
	if not _keys(body, ["format", "version", "payload"]) or body["format"] != FORMAT or body["version"] != VERSION:
		return _fail("unsupported save format/version; explicit migration required")
	var budget: Array[int] = [0]
	var decoded: Dictionary = _unpack(body["payload"], 0, budget)
	if not decoded["error"].is_empty() or not decoded.get("value") is Dictionary:
		return _fail("save data encoding is invalid: " + decoded["error"])
	if _canonical(body).sha256_text() != envelope["checksum_sha256"]:
		return _fail("save checksum mismatch")
	var snapshot: Dictionary = decoded["value"]
	var validation: String = _snapshot_error(snapshot)
	if not validation.is_empty():
		return _fail(validation)
	return {"error": "", "snapshot": snapshot, "checksum_sha256": envelope["checksum_sha256"]}

func _snapshot_error(snapshot: Dictionary) -> String:
	if not _keys(snapshot, ["boundary", "source_version", "content_version", "behavior_version", "gameplay_state", "native_rng", "native_clocks", "queued_service_events"]):
		return "snapshot fields differ from stable-boundary contract"
	if snapshot["boundary"] != "stable":
		return "only explicit stable boundaries may be saved"
	for key: String in ["source_version", "content_version", "behavior_version"]:
		if not snapshot[key] is String or not _identifier(snapshot[key], 96):
			return "unsafe version identifier: " + key
	for key: String in ["gameplay_state", "native_rng", "native_clocks"]:
		if not snapshot[key] is Dictionary:
			return "snapshot record required: " + key
	if not snapshot["queued_service_events"] is Array:
		return "ordered service event array required"
	return ""

func _pack(value: Variant, depth: int, budget: Array[int]) -> Dictionary:
	budget[0] += 1
	if depth > MAX_DEPTH or budget[0] > MAX_NODES:
		return _fail("save structure exceeds bounds")
	if value == null or value is bool or value is String:
		if value is String and value.length() > 65536:
			return _fail("save string exceeds bounds")
		return {"error": "", "value": value}
	if value is int:
		return {"error": "", "value": {"i64": str(value)}}
	if value is Array or value is Dictionary:
		var items: Array = []
		for key: Variant in value:
			var item: Variant = key if value is Array else value[key]
			if value is Dictionary and (not key is String or key.length() > 256):
				return _fail("save dictionary keys must be bounded strings")
			var encoded: Dictionary = _pack(item, depth + 1, budget)
			if not encoded["error"].is_empty():
				return encoded
			items.append(encoded["value"] if value is Array else [key, encoded["value"]])
		return {"error": "", "value": items if value is Array else {"ordered_map": items}}
	return _fail("save accepts data only; floats and engine objects are unsupported")

func _unpack(value: Variant, depth: int, budget: Array[int]) -> Dictionary:
	budget[0] += 1
	if depth > MAX_DEPTH or budget[0] > MAX_NODES:
		return _fail("save structure exceeds bounds")
	if value == null or value is bool or value is String:
		if value is String and value.length() > 65536:
			return _fail("save string exceeds bounds")
		return {"error": "", "value": value}
	if value is Array:
		var items: Array = []
		for item: Variant in value:
			var decoded: Dictionary = _unpack(item, depth + 1, budget)
			if not decoded["error"].is_empty():
				return decoded
			items.append(decoded["value"])
		return {"error": "", "value": items}
	if not value is Dictionary or value.size() != 1:
		return _fail("untyped numeric or object value in save")
	if value.has("i64"):
		if not value["i64"] is String or not _i64(value["i64"]):
			return _fail("exact integer exceeds signed 64-bit bounds")
		return {"error": "", "value": int(value["i64"])}
	if not value.get("ordered_map") is Array:
		return _fail("unknown save data tag")
	var record: Dictionary = {}
	for pair: Variant in value["ordered_map"]:
		if not pair is Array or pair.size() != 2 or not pair[0] is String or pair[0].length() > 256 or record.has(pair[0]):
			return _fail("ordered dictionary entry is malformed or duplicated")
		var decoded: Dictionary = _unpack(pair[1], depth + 1, budget)
		if not decoded["error"].is_empty():
			return decoded
		record[pair[0]] = decoded["value"]
	return {"error": "", "value": record}

func _i64(text: String) -> bool:
	if text.is_empty() or text == "-0":
		return false
	var digits: String = text.trim_prefix("-")
	if digits.is_empty() or (digits.length() > 1 and digits.begins_with("0")):
		return false
	for byte: int in digits.to_utf8_buffer():
		if byte < 48 or byte > 57:
			return false
	var maximum: String = "9223372036854775808" if text.begins_with("-") else "9223372036854775807"
	return digits.length() < maximum.length() or (digits.length() == maximum.length() and digits <= maximum)

func _canonical(value: Variant) -> String:
	if value is Dictionary:
		var keys: Array = value.keys()
		keys.sort()
		var items: Array[String] = []
		for key: String in keys:
			items.append(JSON.stringify(key) + ":" + _canonical(value[key]))
		return "{" + ",".join(items) + "}"
	if value is Array:
		var items: Array[String] = []
		for item: Variant in value:
			items.append(_canonical(item))
		return "[" + ",".join(items) + "]"
	return JSON.stringify(value)

func _paths(slot: String) -> Dictionary:
	if not _identifier(slot, 32) or slot.contains(".") or slot.contains("+"):
		return _fail("unsafe save slot identifier")
	if not storage_root.is_absolute_path() or storage_root != storage_root.simplify_path() or storage_root.contains("://") or storage_root.contains("\\") or storage_root.ends_with("/"):
		return _fail("save root must be a normalized absolute directory")
	var paths: Dictionary = {"error": "", "save": storage_root.path_join(slot + ".json"),
		"backup": storage_root.path_join(slot + ".json.bak"), "temp": storage_root.path_join(slot + ".json.tmp"),
		"backup_temp": storage_root.path_join(slot + ".json.bak.tmp")}
	for path: String in [storage_root, paths["save"], paths["backup"], paths["temp"], paths["backup_temp"]]:
		if _linked(path):
			return _fail("save path contains a symbolic link or reparse point")
		if path != storage_root and DirAccess.dir_exists_absolute(path):
			return _fail("save file path is a directory")
	return paths

func _linked(path: String) -> bool:
	var current: String = path
	while current != current.get_base_dir():
		var parent: DirAccess = DirAccess.open(current.get_base_dir())
		if parent != null and parent.is_link(current.get_file()):
			return true
		current = current.get_base_dir()
	return false

func _identifier(text: String, maximum: int) -> bool:
	if text.is_empty() or text.length() > maximum:
		return false
	for byte: int in text.to_utf8_buffer():
		if not (byte >= 48 and byte <= 57 or byte >= 65 and byte <= 90 or byte >= 97 and byte <= 122 or byte in [45, 46, 95, 43]):
			return false
	return true

func _keys(record: Dictionary, expected: Array) -> bool:
	return record.size() == expected.size() and expected.all(func(key: String) -> bool: return record.has(key))

func _write(path: String, bytes: PackedByteArray) -> bool:
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return false
	file.store_buffer(bytes)
	file.flush()
	var ok: bool = file.get_error() == OK
	file.close()
	return ok

func _clean_staging(paths: Dictionary) -> void:
	DirAccess.remove_absolute(paths["temp"])
	DirAccess.remove_absolute(paths["backup_temp"])

func _fail(reason: String) -> Dictionary:
	return {"error": reason}
