extends SceneTree
## T-29028. Original numeric differential fixture remains external and ignored.
## This store test supplies no gameplay readiness/completion or HUD behavior.

const CalendarSource = preload("res://services/native_calendar_source.gd")

var checks: int = 0
var failures: Array[String] = []
var months: Array[Dictionary] = []

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok:
		failures.append(label)

func _initialize() -> void:
	var source_path: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	var expected_path: String = OS.get_environment("TOKIMEMO_CALENDAR_EXPECTED")
	var output_root: String = OS.get_environment("TOKIMEMO_CALENDAR_OUTPUT")
	var source := CalendarSource.new()
	check(not source.is_ready() and source.identity().is_empty(), "new reader is closed")
	check(not source.month(95, 4)["error"].is_empty(), "unopened month is refused")
	var opened: String = source.open_source(source_path)
	check(opened.is_empty() and source.is_ready(), "original source opens: " + opened)
	if not opened.is_empty():
		_finish(output_root)
		return
	check(source.identity() == CalendarSource.SOURCE_SHA, "identity is the verified original hash")
	var expected: Variant = JSON.parse_string(FileAccess.get_file_as_string(expected_path))
	check(expected is Dictionary and expected.get("months") is Array and expected["months"].size() == 36,
		"canonical parser's ignored36-month fixture is available")
	if not expected is Dictionary or not expected.get("months") is Array or expected["months"].size() != 36:
		_finish(output_root)
		return
	for index: int in range(36):
		var absolute_month: int = 95 * 12 + 3 + index
		var year: int = absolute_month / 12
		var calendar_month: int = absolute_month % 12 + 1
		var value: Dictionary = source.month(year, calendar_month)
		var wanted: Dictionary = expected["months"][index]
		check(value["error"].is_empty(), "source month%d is supported" % index)
		check(_same_words(value["day_kind"], wanted["day_kind"]) and _same_words(value["marker"], wanted["marker"]),
			"month%d matches every original parser numeric word" % index)
		var count: int = value["day_kind"][0]
		check(value["day_kind"].size() == count + 1 and value["marker"].size() == count + 1,
			"month%d contains only the native-written prefix" % index)
		check(typeof(value["day_kind"][0]) == TYPE_INT and typeof(value["marker"][0]) == TYPE_INT,
			"month%d exposes integer records" % index)
		months.append({"year": year, "month": calendar_month,
			"day_kind": value["day_kind"].duplicate(), "marker": value["marker"].duplicate()})
		value["day_kind"][1] = value["day_kind"][1] ^ 1
		value["marker"][1] = value["marker"][1] ^ 1
		var unchanged: Dictionary = source.month(year, calendar_month)
		check(_same_words(unchanged["day_kind"], wanted["day_kind"]) and _same_words(unchanged["marker"], wanted["marker"]),
			"month%d result mutations cannot change immutable source rows" % index)
	for invalid: Array in [[94,12], [95,3], [98,4], [99,1], [1995,4], [95,0], [95,13]]:
		check(not source.month(invalid[0], invalid[1])["error"].is_empty(), "outside source month is refused: " + str(invalid))
	# The invalid binary is invented; no original bytes are copied into res://.
	var bad_path: String = output_root.path_join("invented-calendar.bin")
	var invented: PackedByteArray = PackedByteArray()
	invented.resize(CalendarSource.SOURCE_SIZE)
	var invalid_file: FileAccess = FileAccess.open(bad_path, FileAccess.WRITE)
	check(invalid_file != null, "external invented invalid source file can be written")
	if invalid_file != null:
		invalid_file.store_buffer(invented)
		invalid_file.close()
		var refused: String = source.open_source(bad_path)
		check(not refused.is_empty() and not source.is_ready() and source.identity().is_empty(),
			"failed hash reopen clears readiness and identity")
		check(not source.month(95, 4)["error"].is_empty(), "failed reopen retains no prior month rows")
	for unsafe: String in ["res://project.godot", "relative/CALROM.NBN", "/", "//repo/CALROM.NBN", source_path + "/../CALROM.NBN"]:
		check(not source.open_source(unsafe).is_empty() and not source.is_ready(), "unsafe source path refused: " + unsafe)
	check(source.open_source(source_path).is_empty() and source.is_ready(), "valid reopen restores the exact source")
	_finish(output_root)

func _same_words(actual: Array, expected: Array) -> bool:
	# Godot JSON loads numeric fixture words as floats; the API returns typed
	# Array[int]. Compare exact numeric words without changing production types.
	if actual.size() != expected.size():
		return false
	for index: int in range(actual.size()):
		if typeof(actual[index]) != TYPE_INT or expected[index] != floor(expected[index]) or actual[index] != int(expected[index]):
			return false
	return true

func _finish(output_root: String) -> void:
	var report: Dictionary = {"checks": checks, "failures": failures, "months": months,
		"scope": "read-only original CALROM numeric prefixes and API negatives; no gameplay/service/HUD acceptance"}
	var output: FileAccess = FileAccess.open(output_root.path_join("calendar-source.json"), FileAccess.WRITE)
	if output != null:
		output.store_string(JSON.stringify(report, "\t"))
		output.close()
	for failure: String in failures:
		push_error(failure)
	print(JSON.stringify({"checks": checks, "failures": failures, "months": months.size()}))
	quit(0 if failures.is_empty() else 1)
