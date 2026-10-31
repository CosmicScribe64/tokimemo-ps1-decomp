extends RefCounted
## Test fixture: loads the current/next month rows from the original CALROM
## (NativeCalendarSource, TOKIMEMO_CALENDAR_SOURCE) into a rule state, the same
## rows hud_screens.c::calendar_month_load writes. There is no generated
## calendar: without the original file, install() returns an error and the
## caller must stop.

const CalendarSource = preload("res://services/native_calendar_source.gd")

static func open_from_env() -> CalendarSource:
	var source := CalendarSource.new()
	var path: String = OS.get_environment("TOKIMEMO_CALENDAR_SOURCE")
	if path.is_empty() or not source.open_source(path).is_empty():
		return null
	return source

static func install(data: Dictionary, source: CalendarSource, year: int, month: int) -> String:
	if source == null or not source.is_ready():
		return "calendar fixture requires TOKIMEMO_CALENDAR_SOURCE (the original CALROM)"
	if not data.get("calendar") is Array or data["calendar"].size() != 2:
		return "calendar fixture needs the native two-record calendar"
	var next_year: int = year + (1 if month == 12 else 0)
	var next_month: int = 1 if month == 12 else month + 1
	var rows: Array[Dictionary] = [source.month(year, month)]
	# 98-03 is the last CALROM month; the source never loads a following row.
	rows.append(source.month(next_year, next_month) if not (year == 98 and month == 3) else {"error": "", "day_kind": [], "marker": []})
	for side: int in range(2):
		if not String(rows[side]["error"]).is_empty():
			return rows[side]["error"]
		var record: Dictionary = data["calendar"][side]
		record["day_kind"].fill(0)
		record["marker"].fill(0)
		for key: String in ["day_kind", "marker"]:
			for index: int in range(rows[side][key].size()):
				record[key][index] = rows[side][key][index]
	data["resources"]["native_calendar_source"] = source.identity()
	return ""

static var _shared: CalendarSource = null

## Route harnesses: install the original rows or report why not. The error is
## printed; the following rollover then refuses, so the harness fails visibly.
static func install_or_report(data: Dictionary, year: int, month: int) -> String:
	if _shared == null:
		_shared = open_from_env()
	var error: String = install(data, _shared, year, month)
	if not error.is_empty():
		push_error("calendar fixture: " + error)
	return error
