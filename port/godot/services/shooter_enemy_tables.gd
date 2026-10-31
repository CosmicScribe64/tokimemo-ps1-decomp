extends RefCounted
## T-29024. Original finite enemy tables; not a model/render-ready capability.
## Producer: tools/port/taco_tables.py (enemy-tables.json); the pin is its output.
const Manifest = preload("res://services/shooter_combat_tables.gd")
const MANIFEST_SHA256: String = "f06ebbc60ea3ba8a15ea85da3d05ea84e7dc33ef1cdd8086942b6cdcc542dd53"
const EXE_SHA256: String = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
var tables: Dictionary = {}
var error: String = ""

static func validate(data: Dictionary) -> String:
	var shapes: Dictionary = {"model_base":17,"joint_count":17,"joint_poses":17,
		"scale":17,"radius":17,"zero_position_a":4,"zero_position_b":4,"sine_quarter":1025}
	for key: String in shapes:
		if not data.get(key) is Array or data[key].size() != shapes[key]: return "native enemy table shape invalid: "+key
		var low: int = -2147483648 if key in ["model_base","joint_poses"] else -32768
		var high: int = 2147483647 if key in ["model_base","joint_poses"] else 32767
		if key in ["joint_count","radius"]: low = 0; high = 255
		for value: Variant in data[key]:
			if not value is int or value < low or value > high: return "native enemy table width invalid: "+key
	if not data.get("keys_d") is Array or data["keys_d"].size() < 6 or data["keys_d"].size() > 260 or (data["keys_d"].size() & 1) != 0:
		return "native enemy key-script shape invalid"
	for value: Variant in data["keys_d"]:
		if not value is int or value < -32768 or value > 32767: return "native enemy key-script width invalid"
	var keys: Array = data["keys_d"]
	if keys[0] < 0 or keys[1] < keys[0] or keys[1] >= data["joint_count"][13] or keys[2] <= 0 or keys[3] <= 0 or keys[-2] < keys[2]:
		return "native class13 key-script bounds invalid"
	return ""

func open_manifest(path: String) -> String:
	tables.clear()
	error = "TACO enemy tables are missing or unauthenticated"
	if not Manifest._path_valid(path): return error
	var file: FileAccess = FileAccess.open(path,FileAccess.READ)
	if file == null: return error
	var size: int = file.get_length()
	if size <= 0 or size > 262144: return error
	var raw: PackedByteArray = file.get_buffer(size)
	if raw.size() != size or file.get_length() != size or not Manifest._path_valid(path): return error
	var hash: HashingContext = HashingContext.new()
	hash.start(HashingContext.HASH_SHA256); hash.update(raw)
	if hash.finish().hex_encode() != MANIFEST_SHA256: return error
	var parsed: Variant = JSON.parse_string(raw.get_string_from_utf8())
	if not parsed is Dictionary or parsed.get("schema") != "tokimemo.taco-enemy-tables.v3" or parsed.get("overlay_sha256") != Manifest.OVERLAY_SHA256 or parsed.get("exe_sha256") != EXE_SHA256:
		return error
	if not parsed.get("tables") is Dictionary: return error
	for key: String in parsed["tables"]:
		if not parsed["tables"][key] is Array: return error
		for i: int in range(parsed["tables"][key].size()):
			var number: Variant = parsed["tables"][key][i]
			if typeof(number) not in [TYPE_INT,TYPE_FLOAT] or not is_finite(float(number)) or float(int(number)) != float(number): return error
			parsed["tables"][key][i] = int(number)
	var diagnostic: String = validate(parsed["tables"])
	if not diagnostic.is_empty(): return error
	tables = parsed["tables"].duplicate(true)
	error = ""
	return ""
