extends RefCounted
## Birthday records of the weekly queue: src/main/scheduler.c
## day_event_queue_push_birthdays and src/main/menu_speaker_girl.c
## birth_day_check_days. The week builder calls the push for the school-trip
## and club-camp weeks and repeats the same checks inline for ordinary days
## (scheduler.c:698-716), so one function serves both. Only the type and girl
## bytes of a pushed record are written; scene and extra keep what the queue
## clear left.

const DAY_EVENT_GIRL_BIRTHDAY := 0x32
const DAY_EVENT_PLAYER_BIRTHDAY := 0x33
const GIRL_SHIORI := 0
const GIRL_PLAYER := 0xF
## GirlFlagBits (include/game/girl.h): met is bit 1, b6 bit 6.
const FLAG_MET := 0x02
const FLAG_B6 := 0x40
const QUEUE_LENGTH := 32


## birth_day_check_days: 1 when the subject's birthday word names month/day.
## Month 0xD (the month after December, from month + month_offset) reads as 1;
## no other month value is folded.
static func birth_day_check_days(data: Dictionary, subject: int, month: int, day: int) -> int:
	if month == 0xD:
		month = 1
	var subject_id: int = subject & 0xF
	var word: int
	if subject_id == GIRL_PLAYER:
		word = int(data["player_birthday"])
	else:
		word = int(data["girls"][subject_id]["birthday"])
	if month == (word & 0xF) and day == ((word & 0xFFFFFFFF) >> 4) & 0x1F:
		return 1
	return 0


## day_event_queue_push_birthdays. Returns {"index": next queue index,
## "pushed": [[type, girl], ...], "error": ""}. The source has no bound on the
## index; a push past the 32-record queue is refused instead of written.
static func push_birthdays(data: Dictionary, queue_index: int, month_offset: int, day: int) -> Dictionary:
	var out: Dictionary = {"index": queue_index, "pushed": [], "error": ""}
	var month: int = int(data["month"]) + month_offset
	if birth_day_check_days(data, GIRL_SHIORI, month, day) == 1:
		if not _push(data, out, DAY_EVENT_GIRL_BIRTHDAY, GIRL_SHIORI):
			return out
	for i: int in range(1, 0xB):
		var flags: int = int(data["girls"][i]["flags"])
		if (flags & FLAG_MET) != 0 and (flags & FLAG_B6) != 0:
			if birth_day_check_days(data, i, month, day) == 1:
				if not _push(data, out, DAY_EVENT_GIRL_BIRTHDAY, i):
					return out
	# player_flags.b[1] & 0xF: the health nibble; 3 is HEALTH_SICK.
	if ((int(data["player_flags"]) >> 8) & 0xF) != 3:
		if birth_day_check_days(data, GIRL_PLAYER, month, day) == 1:
			_push(data, out, DAY_EVENT_PLAYER_BIRTHDAY, -1)
	return out


static func _push(data: Dictionary, out: Dictionary, type: int, girl: int) -> bool:
	var index: int = int(out["index"])
	if index < 0 or index >= QUEUE_LENGTH:
		out["error"] = "day_event_queue_push_birthdays would write record %d past the 32-record queue" % index
		return false
	var record: Array = data["day_events"][index]
	record[0] = type
	if girl >= 0:
		record[1] = girl
	out["pushed"].append([type, int(record[1])])
	out["index"] = index + 1
	return true
