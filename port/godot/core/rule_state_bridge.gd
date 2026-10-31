extends RefCounted
## T-39300 (M1): two-way bridge between the port's hand-built state (core/game_state.gd
## fields and rules/school_day_state.gd `rule_state`) and the game memory model
## (core/game_memory.gd, original layout from core/game_layout.gd).
##
## push(): state -> memory. Writes every mapped key the state holds, with the C widths.
## pull(): memory -> state. Reads the same regions back; a key or field is assigned only
## when the memory value differs from the state's value at the C width, so a state that
## mirrors the memory keeps its own representation (s32 or u32 spelling, key set,
## array identity) and pull() on an unchanged memory is a no-op.
## Neither direction touches port-only keys (PORT_ONLY_KEYS): they have no home in the
## original's RAM. Neither is called by the frame loop: the bridge is an explicit
## adapter for modules that move onto the memory model and for dumps.
##
## Sources: field widths include/game/game_state.h (GameState), girl.h, club.h,
## day_event.h, calendar.h, appointment.h, menu_records.h; adapter producers
## flow/new_game_state_adapter.gd; globals config/symbol_addrs*.txt
## (icon_enabled 0x80125C90, scene_work 0x801230D0, bg_lba_cache 0x800B5920,
## dec_bg_show 0x800B5938, schedule_last_picked_girl 0x800B6740).

const Math = preload("res://core/native_math.gd")
const Layout = preload("res://core/game_layout.gd")
const Memory = preload("res://core/game_memory.gd")

const GS: int = Layout.GAME_STATE_BASE
const ICON_ENABLED_COUNT: int = 15
const GIRL_COUNT: int = 16
const PARAM_COUNT: int = 9
const MENU_DISABLED_COUNT: int = 14

## rule_state keys with a home in memory.
const MAPPED_KEYS: Array[String] = [
	"year", "month", "day", "weekday", "day_count", "calendar", "params", "player_flags",
	"player_flags_upper", "player_birthday", "girls", "progress_state", "day_events",
	"day_event_index", "scene_flags", "day_event_done_bits", "day_event_count_today",
	"daily_check_done", "last_command", "menu_cursor", "menu_mode", "menu_disabled",
	"icon_enabled", "vacation", "sd_anim_fast", "memory_card_job", "memory_card_busy",
	"girl_last_date_spot_timer", "appointment_words", "clubs", "event_log",
	"event_log_count", "last_picked_girl", "scene_work", "background_cache",
	"background_show", "profile_family_bytes", "profile_given_bytes",
	"profile_nickname_bytes", "current_girl"]

## Keys the route modules keep for presentation, overlays and tests; the original has
## no RAM field for them (or the port has not identified it yet: they stay here until a
## ported C function owns them).
const PORT_ONLY_KEYS: Array[String] = [
	"actor_state", "resources", "voice_wait_frames", "message_title_visible",
	"overlay_state", "allow_multi_week", "holiday", "ending_completed", "ending_phases",
	"ending_type", "appointment_flow", "confessed_girl", "last_completed_overlay",
	"clear_save_completed", "talk_result_code", "phone_state", "param_values", "stats",
	"stress", "calendar_month_offset", "mag_state", "quarter_flags", "album_clear_mask",
	"system_album_clear_mask", "health_low_nibble", "day_kind", "f"]

## `opening` dictionary keys (rules/new_game_route_state.gd) that are GameState fields.
const OPENING_FIELDS: Dictionary = {
	"icon_mode": [Layout.GAME_STATE_ICON_MODE, 1],
	"icon_count": [Layout.GAME_STATE_ICON_COUNT, 1],
	"screen_brightness": [Layout.GAME_STATE_SCREEN_BRIGHTNESS, 1],
}

## Core fields (core/game_state.gd): name -> [GameState offset or absolute address, width, signed].
## An absolute address (>= 0x80000000) is outside GameState.
const CORE_FIELDS: Dictionary = {
	"game_mode": [Layout.GAME_STATE_GAME_MODE, 1, false],
	"mode_stage": [Layout.GAME_STATE_MODE_STAGE, 1, false],
	"mode_step": [Layout.GAME_STATE_MODE_STEP, 1, false],
	"substep": [Layout.GAME_STATE_SUBSTEP, 1, false],
	"game_mode_scratch": [Layout.GAME_STATE_GAME_MODE_SCRATCH, 1, true],
	"mode_stage_scratch": [Layout.GAME_STATE_MODE_STAGE_SCRATCH, 1, true],
	"ovl_target_game_mode": [Layout.GAME_STATE_OVL_TARGET_GAME_MODE, 1, false],
	"frame_count": [Layout.GAME_STATE_FRAME_COUNT, 4, false],
	"timer_ticks": [Layout.GAME_STATE_TIMER_TICKS, 4, true],
	"game_mode_timer": [Layout.GAME_STATE_GAME_MODE_TIMER, 4, true],
	"mode_stage_timer": [Layout.GAME_STATE_MODE_STAGE_TIMER, 4, true],
	"mode_step_timer": [Layout.GAME_STATE_MODE_STEP_TIMER, 4, true],
	"system_seed": [Layout.SYSTEM_DATA_BASE + Layout.SYSTEM_DATA_SEED, 4, true],
}
## Core booleans (stored as one byte, 0 or 1).
const CORE_FLAGS: Dictionary = {
	"paused": Layout.GAME_STATE_PAUSED,
	"restart_request": Layout.GAME_STATE_RESTART_REQUEST,
	"sound_active": Layout.GAME_STATE_SOUND_ACTIVE,
}

# ---- public

## state -> memory. `state` is a core/game_state.gd object (rule_state and opening are
## used when the object has them).
static func push(state: Object, mem: Memory) -> void:
	for field: String in CORE_FIELDS:
		var row: Array = CORE_FIELDS[field]
		mem.write_int(_address(row[0]), row[1], int(state.get(field)))
	for field: String in CORE_FLAGS:
		mem.write_u8(GS + CORE_FLAGS[field], 1 if state.get(field) else 0)
	var rule: Variant = state.get("rule_state")
	var opening: Variant = state.get("opening")
	if rule is Dictionary:
		_push_rule_state(rule, mem, _placeholders(opening))
	if opening is Dictionary:
		for key: String in OPENING_FIELDS:
			if opening.has(key):
				var row: Array = OPENING_FIELDS[key]
				mem.write_int(GS + row[0], row[1], int(opening[key]))

## memory -> state, assigning only values that differ at the C width.
static func pull(mem: Memory, state: Object) -> void:
	for field: String in CORE_FIELDS:
		var row: Array = CORE_FIELDS[field]
		var value: int = mem.read_int(_address(row[0]), row[1], row[2])
		if not _same(int(state.get(field)), value, row[1]):
			state.set(field, value)
	for field: String in CORE_FLAGS:
		var flag: bool = mem.u8(GS + CORE_FLAGS[field]) != 0
		if bool(state.get(field)) != flag:
			state.set(field, flag)
	var rule: Variant = state.get("rule_state")
	var opening: Variant = state.get("opening")
	if rule is Dictionary:
		_pull_rule_state(mem, rule, _placeholders(opening))
	if opening is Dictionary:
		for key: String in OPENING_FIELDS:
			if opening.has(key):
				var row: Array = OPENING_FIELDS[key]
				var value: int = mem.read_int(GS + row[0], row[1], false)
				if not _same(int(opening[key]), value, row[1]):
					opening[key] = value

# ---- helpers

## Keys the state holds only as a placeholder (opening.unread_before_owned_write, written by
## flow/new_game_state_adapter.gd: never read before an owned write). The original's value
## for them is the initial data/bss value, so neither direction touches them.
static func _placeholders(opening: Variant) -> Array:
	if opening is Dictionary and opening.get("unread_before_owned_write") is Array:
		return opening["unread_before_owned_write"]
	return []

static func _address(offset_or_address: int) -> int:
	return offset_or_address if offset_or_address >= 0x80000000 else GS + offset_or_address

static func _same(a: int, b: int, width: int) -> bool:
	match width:
		1:
			return Math.u8(a) == Math.u8(b)
		2:
			return Math.u16(a) == Math.u16(b)
	return Math.u32(a) == Math.u32(b)

## Scalar key: write at push, assign-if-different at pull. `signed` is the spelling the
## existing producers use (adapter decode_s32 or decode_u32), kept so values compare equal.
static func _push_scalar(rule: Dictionary, key: String, mem: Memory, address: int, width: int) -> void:
	if rule.has(key):
		mem.write_int(address, width, int(rule[key]))

static func _pull_scalar(rule: Dictionary, key: String, mem: Memory, address: int, width: int, signed: bool) -> void:
	if not rule.has(key):
		return
	var value: int = mem.read_int(address, width, signed)
	if not rule[key] is int or not _same(int(rule[key]), value, width):
		rule[key] = value

## Byte array key of fixed length.
static func _put_bytes(rule: Dictionary, key: String, mem: Memory, address: int, count: int) -> void:
	if not rule.has(key) or not rule[key] is Array:
		return
	var values: Array = rule[key]
	for i: int in mini(count, values.size()):
		mem.write_u8(address + i, int(values[i]))

static func _get_bytes(rule: Dictionary, key: String, mem: Memory, address: int, count: int) -> void:
	if not rule.has(key) or not rule[key] is Array:
		return
	var values: Array = rule[key]
	for i: int in mini(count, values.size()):
		var value: int = mem.u8(address + i)
		if not values[i] is int or Math.u8(int(values[i])) != value:
			values[i] = value

## Word array key (u32/s32 elements).
static func _put_words(rule: Dictionary, key: String, mem: Memory, address: int, count: int) -> void:
	if not rule.has(key) or not rule[key] is Array:
		return
	var values: Array = rule[key]
	for i: int in mini(count, values.size()):
		mem.write_u32(address + i * 4, int(values[i]))

static func _get_words(rule: Dictionary, key: String, mem: Memory, address: int, count: int, signed: bool) -> void:
	if not rule.has(key) or not rule[key] is Array:
		return
	var values: Array = rule[key]
	for i: int in mini(count, values.size()):
		var value: int = mem.u32(address + i * 4)
		if not values[i] is int or Math.u32(int(values[i])) != value:
			values[i] = Math.s32(value) if signed else value

# ---- rule_state

static func _push_rule_state(rule: Dictionary, mem: Memory, placeholders: Array) -> void:
	_push_scalar(rule, "year", mem, GS + Layout.GAME_STATE_YEAR, 1)
	_push_scalar(rule, "month", mem, GS + Layout.GAME_STATE_MONTH, 1)
	_push_scalar(rule, "day", mem, GS + Layout.GAME_STATE_DAY, 1)
	_push_scalar(rule, "weekday", mem, GS + Layout.GAME_STATE_WEEKDAY, 1)
	_push_scalar(rule, "day_count", mem, GS + Layout.GAME_STATE_DAY_COUNT, 2)
	_push_scalar(rule, "player_flags", mem, GS + Layout.GAME_STATE_PLAYER_FLAGS, 2)
	_push_scalar(rule, "player_flags_upper", mem, GS + Layout.GAME_STATE_PLAYER_FLAGS_UPPER, 2)
	_push_scalar(rule, "player_birthday", mem, GS + Layout.GAME_STATE_PLAYER_BIRTHDAY, 4)
	_push_scalar(rule, "vacation", mem, GS + Layout.GAME_STATE_VACATION, 1)
	_push_scalar(rule, "sd_anim_fast", mem, GS + Layout.GAME_STATE_SD_ANIM_FAST, 1)
	_push_scalar(rule, "memory_card_job", mem, GS + Layout.GAME_STATE_MEMORY_CARD_JOB, 1)
	_push_scalar(rule, "memory_card_busy", mem, GS + Layout.GAME_STATE_MEMORY_CARD_BUSY, 1)
	_push_scalar(rule, "day_event_index", mem, GS + Layout.GAME_STATE_DAY_EVENT_INDEX, 1)
	_push_scalar(rule, "day_event_done_bits", mem, GS + Layout.GAME_STATE_DAY_EVENT_DONE_BITS, 1)
	_push_scalar(rule, "day_event_count_today", mem, GS + Layout.GAME_STATE_DAY_EVENT_COUNT_TODAY, 1)
	_push_scalar(rule, "daily_check_done", mem, GS + Layout.GAME_STATE_DAILY_CHECK_DONE, 1)
	_push_scalar(rule, "last_command", mem, GS + Layout.GAME_STATE_LAST_COMMAND, 4)
	_push_scalar(rule, "menu_cursor", mem, GS + Layout.GAME_STATE_MENU_CURSOR_0, 1)
	_push_scalar(rule, "menu_mode", mem, GS + Layout.GAME_STATE_MENU_MODE, 1)
	_push_scalar(rule, "event_log_count", mem, GS + Layout.GAME_STATE_EVENT_LOG_COUNT, 1)
	_push_scalar(rule, "current_girl", mem, GS + Layout.GAME_STATE_CURRENT_GIRL, 1)
	if "last_picked_girl" not in placeholders:
		_push_scalar(rule, "last_picked_girl", mem, mem.global_address("schedule_last_picked_girl"), 1)
	_push_scalar(rule, "scene_work", mem, mem.global_address("scene_work"), 4)
	_push_scalar(rule, "background_show", mem, mem.global_address("dec_bg_show"), 1)
	_put_bytes(rule, "progress_state", mem, GS + Layout.GAME_STATE_PROGRESS_STATE, 0x9c)
	_put_bytes(rule, "scene_flags", mem, GS + Layout.GAME_STATE_SCENE_FLAGS, 0xfc)
	_put_bytes(rule, "girl_last_date_spot_timer", mem, GS + Layout.GAME_STATE_GIRL_LAST_DATE_SPOT_TIMER, 16)
	_put_bytes(rule, "profile_family_bytes", mem, GS + Layout.GAME_STATE_PLAYER_FAMILY_NAME, 8)
	_put_bytes(rule, "profile_given_bytes", mem, GS + Layout.GAME_STATE_PLAYER_GIVEN_NAME, 8)
	_put_bytes(rule, "profile_nickname_bytes", mem, GS + Layout.GAME_STATE_PLAYER_NICKNAME, 16)
	_put_bytes(rule, "icon_enabled", mem, mem.global_address("icon_enabled"), ICON_ENABLED_COUNT)
	_put_words(rule, "params", mem, GS + Layout.GAME_STATE_PARAM_CONDITION, PARAM_COUNT)
	_put_words(rule, "appointment_words", mem, GS + Layout.GAME_STATE_U54C, 8)
	_put_words(rule, "background_cache", mem, mem.global_address("bg_lba_cache"), 4)
	_push_calendar(rule, mem)
	_push_girls(rule, mem)
	_push_day_events(rule, mem)
	_push_clubs(rule, mem)
	_push_event_log(rule, mem)
	_push_menu_disabled(rule, mem)

static func _pull_rule_state(mem: Memory, rule: Dictionary, placeholders: Array) -> void:
	_pull_scalar(rule, "year", mem, GS + Layout.GAME_STATE_YEAR, 1, false)
	_pull_scalar(rule, "month", mem, GS + Layout.GAME_STATE_MONTH, 1, false)
	_pull_scalar(rule, "day", mem, GS + Layout.GAME_STATE_DAY, 1, false)
	_pull_scalar(rule, "weekday", mem, GS + Layout.GAME_STATE_WEEKDAY, 1, false)
	_pull_scalar(rule, "day_count", mem, GS + Layout.GAME_STATE_DAY_COUNT, 2, false)
	_pull_scalar(rule, "player_flags", mem, GS + Layout.GAME_STATE_PLAYER_FLAGS, 2, false)
	_pull_scalar(rule, "player_flags_upper", mem, GS + Layout.GAME_STATE_PLAYER_FLAGS_UPPER, 2, false)
	_pull_scalar(rule, "player_birthday", mem, GS + Layout.GAME_STATE_PLAYER_BIRTHDAY, 4, false)
	_pull_scalar(rule, "vacation", mem, GS + Layout.GAME_STATE_VACATION, 1, false)
	_pull_scalar(rule, "sd_anim_fast", mem, GS + Layout.GAME_STATE_SD_ANIM_FAST, 1, false)
	_pull_scalar(rule, "memory_card_job", mem, GS + Layout.GAME_STATE_MEMORY_CARD_JOB, 1, false)
	_pull_scalar(rule, "memory_card_busy", mem, GS + Layout.GAME_STATE_MEMORY_CARD_BUSY, 1, false)
	_pull_scalar(rule, "day_event_index", mem, GS + Layout.GAME_STATE_DAY_EVENT_INDEX, 1, false)
	_pull_scalar(rule, "day_event_done_bits", mem, GS + Layout.GAME_STATE_DAY_EVENT_DONE_BITS, 1, false)
	_pull_scalar(rule, "day_event_count_today", mem, GS + Layout.GAME_STATE_DAY_EVENT_COUNT_TODAY, 1, false)
	_pull_scalar(rule, "daily_check_done", mem, GS + Layout.GAME_STATE_DAILY_CHECK_DONE, 1, false)
	_pull_scalar(rule, "last_command", mem, GS + Layout.GAME_STATE_LAST_COMMAND, 4, false)
	_pull_scalar(rule, "menu_cursor", mem, GS + Layout.GAME_STATE_MENU_CURSOR_0, 1, true)
	_pull_scalar(rule, "menu_mode", mem, GS + Layout.GAME_STATE_MENU_MODE, 1, false)
	_pull_scalar(rule, "event_log_count", mem, GS + Layout.GAME_STATE_EVENT_LOG_COUNT, 1, false)
	_pull_scalar(rule, "current_girl", mem, GS + Layout.GAME_STATE_CURRENT_GIRL, 1, false)
	if "last_picked_girl" not in placeholders:
		_pull_scalar(rule, "last_picked_girl", mem, mem.global_address("schedule_last_picked_girl"), 1, true)
	_pull_scalar(rule, "scene_work", mem, mem.global_address("scene_work"), 4, true)
	_pull_scalar(rule, "background_show", mem, mem.global_address("dec_bg_show"), 1, false)
	_get_bytes(rule, "progress_state", mem, GS + Layout.GAME_STATE_PROGRESS_STATE, 0x9c)
	_get_bytes(rule, "scene_flags", mem, GS + Layout.GAME_STATE_SCENE_FLAGS, 0xfc)
	_get_bytes(rule, "girl_last_date_spot_timer", mem, GS + Layout.GAME_STATE_GIRL_LAST_DATE_SPOT_TIMER, 16)
	_get_bytes(rule, "profile_family_bytes", mem, GS + Layout.GAME_STATE_PLAYER_FAMILY_NAME, 8)
	_get_bytes(rule, "profile_given_bytes", mem, GS + Layout.GAME_STATE_PLAYER_GIVEN_NAME, 8)
	_get_bytes(rule, "profile_nickname_bytes", mem, GS + Layout.GAME_STATE_PLAYER_NICKNAME, 16)
	_get_bytes(rule, "icon_enabled", mem, mem.global_address("icon_enabled"), ICON_ENABLED_COUNT)
	_get_words(rule, "params", mem, GS + Layout.GAME_STATE_PARAM_CONDITION, PARAM_COUNT, true)
	_get_words(rule, "appointment_words", mem, GS + Layout.GAME_STATE_U54C, 8, false)
	_get_words(rule, "background_cache", mem, mem.global_address("bg_lba_cache"), 4, true)
	_pull_calendar(mem, rule)
	_pull_girls(mem, rule)
	_pull_day_events(mem, rule)
	_pull_clubs(mem, rule)
	_pull_event_log(mem, rule)
	_pull_menu_disabled(mem, rule)

## rule_state.calendar[2]: {day_kind[32], marker[32]} = GameState.month_calendar[2].
static func _push_calendar(rule: Dictionary, mem: Memory) -> void:
	if not rule.get("calendar") is Array:
		return
	var records: Array = rule["calendar"]
	for i: int in mini(2, records.size()):
		var base: int = GS + Layout.GAME_STATE_MONTH_CALENDAR + i * Layout.SIZEOF_MONTH_CALENDAR
		var record: Dictionary = records[i]
		_put_bytes(record, "day_kind", mem, base + Layout.MONTH_CALENDAR_DAY_KIND, 32)
		_put_bytes(record, "marker", mem, base + Layout.MONTH_CALENDAR_MARKER, 32)

static func _pull_calendar(mem: Memory, rule: Dictionary) -> void:
	if not rule.get("calendar") is Array:
		return
	var records: Array = rule["calendar"]
	for i: int in mini(2, records.size()):
		var base: int = GS + Layout.GAME_STATE_MONTH_CALENDAR + i * Layout.SIZEOF_MONTH_CALENDAR
		var record: Dictionary = records[i]
		_get_bytes(record, "day_kind", mem, base + Layout.MONTH_CALENDAR_DAY_KIND, 32)
		_get_bytes(record, "marker", mem, base + Layout.MONTH_CALENDAR_MARKER, 32)

## rule_state.girls[16]: GirlRecord (0x38): three ParamValue words, flags, birthday, counters.
static func _girl_base(i: int) -> int:
	return GS + Layout.GAME_STATE_GIRLS + i * Layout.SIZEOF_GIRL_RECORD

static func _push_girls(rule: Dictionary, mem: Memory) -> void:
	if not rule.get("girls") is Array:
		return
	var girls: Array = rule["girls"]
	for i: int in mini(GIRL_COUNT, girls.size()):
		var base: int = _girl_base(i)
		var girl: Dictionary = girls[i]
		_push_scalar(girl, "tokimeki_word", mem, base + Layout.GIRL_RECORD_TOKIMEKI_FRACTION, 4)
		_push_scalar(girl, "affection_word", mem, base + Layout.GIRL_RECORD_AFFECTION_FRACTION, 4)
		_push_scalar(girl, "syoushin_word", mem, base + Layout.GIRL_RECORD_SYOUSHIN_FRACTION, 4)
		_push_scalar(girl, "flags", mem, base + Layout.GIRL_RECORD_FLAGS, 4)
		_push_scalar(girl, "birthday", mem, base + Layout.GIRL_RECORD_BIRTHDAY, 4)
		_put_bytes(girl, "counters", mem, base + Layout.GIRL_RECORD_SCENE_COUNTERS, 36)

static func _pull_girls(mem: Memory, rule: Dictionary) -> void:
	if not rule.get("girls") is Array:
		return
	var girls: Array = rule["girls"]
	for i: int in mini(GIRL_COUNT, girls.size()):
		var base: int = _girl_base(i)
		var girl: Dictionary = girls[i]
		_pull_scalar(girl, "tokimeki_word", mem, base + Layout.GIRL_RECORD_TOKIMEKI_FRACTION, 4, true)
		_pull_scalar(girl, "affection_word", mem, base + Layout.GIRL_RECORD_AFFECTION_FRACTION, 4, true)
		_pull_scalar(girl, "syoushin_word", mem, base + Layout.GIRL_RECORD_SYOUSHIN_FRACTION, 4, true)
		_pull_scalar(girl, "flags", mem, base + Layout.GIRL_RECORD_FLAGS, 4, false)
		_pull_scalar(girl, "birthday", mem, base + Layout.GIRL_RECORD_BIRTHDAY, 4, false)
		_get_bytes(girl, "counters", mem, base + Layout.GIRL_RECORD_SCENE_COUNTERS, 36)

## rule_state.day_events[32]: four bytes each (DayEvent: type, girl, scene, extra).
static func _push_day_events(rule: Dictionary, mem: Memory) -> void:
	if not rule.get("day_events") is Array:
		return
	var events: Array = rule["day_events"]
	for i: int in mini(32, events.size()):
		if events[i] is Array:
			var record: Dictionary = {"b": events[i]}
			_put_bytes(record, "b", mem, GS + Layout.GAME_STATE_DAY_EVENTS + i * Layout.SIZEOF_DAY_EVENT, 4)

static func _pull_day_events(mem: Memory, rule: Dictionary) -> void:
	if not rule.get("day_events") is Array:
		return
	var events: Array = rule["day_events"]
	for i: int in mini(32, events.size()):
		if events[i] is Array:
			var record: Dictionary = {"b": events[i]}
			_get_bytes(record, "b", mem, GS + Layout.GAME_STATE_DAY_EVENTS + i * Layout.SIZEOF_DAY_EVENT, 4)

## rule_state.clubs[12]: {practice_points u16, level u8, flags u8} = ClubRecord.
static func _push_clubs(rule: Dictionary, mem: Memory) -> void:
	if not rule.get("clubs") is Array:
		return
	var clubs: Array = rule["clubs"]
	for i: int in mini(12, clubs.size()):
		var base: int = GS + Layout.GAME_STATE_CLUBS + i * Layout.SIZEOF_CLUB_RECORD
		var club: Dictionary = clubs[i]
		_push_scalar(club, "practice_points", mem, base + Layout.CLUB_RECORD_PRACTICE_POINTS, 2)
		_push_scalar(club, "level", mem, base + Layout.CLUB_RECORD_LEVEL, 1)
		_push_scalar(club, "flags", mem, base + Layout.CLUB_RECORD_FLAGS, 1)

static func _pull_clubs(mem: Memory, rule: Dictionary) -> void:
	if not rule.get("clubs") is Array:
		return
	var clubs: Array = rule["clubs"]
	for i: int in mini(12, clubs.size()):
		var base: int = GS + Layout.GAME_STATE_CLUBS + i * Layout.SIZEOF_CLUB_RECORD
		var club: Dictionary = clubs[i]
		_pull_scalar(club, "practice_points", mem, base + Layout.CLUB_RECORD_PRACTICE_POINTS, 2, false)
		_pull_scalar(club, "level", mem, base + Layout.CLUB_RECORD_LEVEL, 1, false)
		_pull_scalar(club, "flags", mem, base + Layout.CLUB_RECORD_FLAGS, 1, false)

## rule_state.event_log[256]: EventLogEntry (eight byte fields).
const EVENT_LOG_FIELDS: Array[String] = ["girl", "flag", "scene", "rank", "year", "month", "day", "weekday"]

static func _push_event_log(rule: Dictionary, mem: Memory) -> void:
	if not rule.get("event_log") is Array:
		return
	var log: Array = rule["event_log"]
	for i: int in mini(256, log.size()):
		var base: int = GS + Layout.GAME_STATE_EVENT_LOG + i * Layout.SIZEOF_EVENT_LOG_ENTRY
		var entry: Dictionary = log[i]
		for k: int in EVENT_LOG_FIELDS.size():
			_push_scalar(entry, EVENT_LOG_FIELDS[k], mem, base + k, 1)

static func _pull_event_log(mem: Memory, rule: Dictionary) -> void:
	if not rule.get("event_log") is Array:
		return
	var log: Array = rule["event_log"]
	for i: int in mini(256, log.size()):
		var base: int = GS + Layout.GAME_STATE_EVENT_LOG + i * Layout.SIZEOF_EVENT_LOG_ENTRY
		var entry: Dictionary = log[i]
		for k: int in EVENT_LOG_FIELDS.size():
			_pull_scalar(entry, EVENT_LOG_FIELDS[k], mem, base + k, 1, false)

## rule_state.menu_disabled[14] = menu_items[i].disabled.
static func _push_menu_disabled(rule: Dictionary, mem: Memory) -> void:
	if not rule.get("menu_disabled") is Array:
		return
	var values: Array = rule["menu_disabled"]
	for i: int in mini(MENU_DISABLED_COUNT, values.size()):
		mem.write_u8(GS + Layout.GAME_STATE_MENU_ITEMS + i * Layout.SIZEOF_MENU_ITEM + Layout.MENU_ITEM_DISABLED, int(values[i]))

static func _pull_menu_disabled(mem: Memory, rule: Dictionary) -> void:
	if not rule.get("menu_disabled") is Array:
		return
	var values: Array = rule["menu_disabled"]
	for i: int in mini(MENU_DISABLED_COUNT, values.size()):
		var value: int = mem.u8(GS + Layout.GAME_STATE_MENU_ITEMS + i * Layout.SIZEOF_MENU_ITEM + Layout.MENU_ITEM_DISABLED)
		if not values[i] is int or Math.u8(int(values[i])) != value:
			values[i] = value
