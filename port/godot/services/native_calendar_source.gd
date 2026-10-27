extends RefCounted
## T-29028. Immutable CALROM numeric prefixes from the original36-month source.
## hud_screens.c::calendar_month_load writes only count+1 bytes in each row.
## This reader supplies values; gameplay owns calendar rotation and completion.

const ExternalPath = preload("res://services/external_content_path.gd")
const SOURCE_SHA: String = "775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867"
const SOURCE_SIZE: int = 305152
const LOAD_ADDRESS: int = 0x801a0000
const MONTH_COUNT: int = 36
const TABLE_SIZE: int = MONTH_COUNT * 4

var _months: Array[Dictionary] = []
var _identity: String = ""

func open_source(path: String) -> String:
	_clear()
	if not ExternalPath.is_external_file(path):
		return "calendar source requires a safe external original file"
	var source: FileAccess = FileAccess.open(path, FileAccess.READ)
	if source == null:
		return "calendar source could not be opened"
	if source.get_length() != SOURCE_SIZE:
		source.close()
		return "calendar source size differs from the original CALROM profile"
	var bytes: PackedByteArray = source.get_buffer(SOURCE_SIZE)
	source.close()
	if bytes.size() != SOURCE_SIZE or _sha(bytes) != SOURCE_SHA:
		return "calendar source bytes differ from the original CALROM identity"
	var prepared: Dictionary = _parse_bytes(bytes)
	if not prepared["error"].is_empty():
		return prepared["error"]
	_months.assign(prepared["months"])
	_identity = SOURCE_SHA
	return ""

func is_ready() -> bool:
	return _identity == SOURCE_SHA and _months.size() == MONTH_COUNT

func identity() -> String:
	return _identity if is_ready() else ""

func month(year: int, calendar_month: int) -> Dictionary:
	if not is_ready():
		return _bad_month("calendar source is not open")
	if year < 95 or year > 98 or calendar_month < 1 or calendar_month > 12:
		return _bad_month("calendar source supports native years95..98 and months1..12")
	var index: int = (year - 95) * 12 + calendar_month - 4
	if index < 0 or index >= MONTH_COUNT:
		return _bad_month("calendar source month is outside April95 through March98")
	return _months[index].duplicate(true)

static func _parse_bytes(bytes: PackedByteArray) -> Dictionary:
	if bytes.size() != SOURCE_SIZE:
		return {"error": "calendar numeric reader requires the exact source extent"}
	var prepared: Array[Dictionary] = []
	var extents: Array[Vector2i] = [Vector2i(0, TABLE_SIZE)]
	for index: int in range(MONTH_COUNT):
		var pointer: int = bytes.decode_u32(index * 4)
		var offset: int = pointer - LOAD_ADDRESS
		if offset < TABLE_SIZE or offset % 16 != 0 or offset + 0x2008 > bytes.size():
			return {"error": "calendar month pointer exceeds the source profile"}
		var numeric: int = offset + 0x2000
		var count: int = bytes.decode_u32(numeric)
		var first_weekday: int = bytes.decode_u32(numeric + 4)
		if count < 28 or count > 31 or first_weekday > 6:
			return {"error": "calendar count or first weekday exceeds native bounds"}
		var end: int = numeric + 8 + count * 8
		if end > bytes.size():
			return {"error": "calendar numeric month records are truncated"}
		for extent: Vector2i in extents:
			if offset < extent.y and end > extent.x:
				return {"error": "calendar tile/numeric source extents overlap"}
		extents.append(Vector2i(offset, end))
		var kinds: Array[int] = [count]
		var markers: Array[int] = [first_weekday]
		for day: int in range(count):
			var kind: int = bytes.decode_u32(numeric + 8 + day * 4)
			var marker: int = bytes.decode_u32(numeric + 8 + count * 4 + day * 4)
			if kind > 255 or marker > 255:
				return {"error": "calendar numeric word exceeds its native byte destination"}
			kinds.append(kind)
			markers.append(marker)
		prepared.append({"error": "", "day_kind": kinds, "marker": markers})
	return {"error": "", "months": prepared}

static func _bad_month(message: String) -> Dictionary:
	var kinds: Array[int] = []
	var markers: Array[int] = []
	return {"error": message, "day_kind": kinds, "marker": markers}

static func _sha(bytes: PackedByteArray) -> String:
	var context := HashingContext.new()
	context.start(HashingContext.HASH_SHA256)
	context.update(bytes)
	return context.finish().hex_encode()

func _clear() -> void:
	_identity = ""
	_months.clear()
