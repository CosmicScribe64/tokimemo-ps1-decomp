extends "res://core/game_state.gd"
## T-29004: bounded native rule records, loaded from a validated boundary.
## Every additional field, including actor counters/readiness, is transactional.

const CanonicalWords = preload("res://core/canonical_words.gd")
const _SCRIPT_PATH := "res://rules/school_day_state.gd"

var rule_state: Dictionary = {}
# State-hash text cache (not state: never projected, restored or hashed).
var _words_text := CanonicalWords.new()

func projection() -> Dictionary:
	var output: Dictionary = super.projection()
	output["rule_state"] = rule_state.duplicate(true)
	return output

func restore(checkpoint_data: Dictionary) -> void:
	# Base restore iterates the virtual projection, including rule_state.
	super.restore(checkpoint_data.duplicate(true))

func to_words() -> Array[int]:
	var words: Array[int] = super.to_words()
	# Fixed canonical JSON extends the core hash to every nested rule field.
	# No opaque engine object or Image may enter rule_state.
	for byte: int in JSON.stringify(rule_state).to_utf8_buffer():
		words.append(byte)
	return words

## JSON.stringify(to_words()) without the per-byte list (GameState.words_json).
## A subclass script falls back to its own to_words().
func words_json() -> String:
	if get_script().resource_path != _SCRIPT_PATH: return JSON.stringify(to_words())
	return _words_text.render(JSON.stringify(core_words()), [rule_state])

## Refusal for a consumer of a rule field that the route stored without a
## sourced value (resources.unsourced, written by flow/fresh_gameplay_handoff.gd
## from the state adapter's list). Empty when the field is sourced.
static func unsourced_error(data: Dictionary, field: String) -> String:
	var resources: Variant = data.get("resources")
	if resources is Dictionary and resources.get("unsourced") is Dictionary and resources["unsourced"].has(field):
		return "%s is not sourced on this route (%s); refusing to read it" % [field, str(resources["unsourced"][field])]
	return ""

func structural_error() -> String:
	if not _plain_record(rule_state, 0):
		return "rule state must contain bounded primitive records, never engine objects"
	var required: Array[String] = ["year", "month", "day", "weekday", "day_count",
		"calendar", "params", "player_flags", "player_flags_upper", "player_birthday",
		"girls", "progress_state", "day_events", "day_event_index", "scene_flags",
		"day_event_done_bits", "day_event_count_today", "daily_check_done", "last_command",
		"menu_cursor", "icon_enabled", "vacation", "sd_anim_fast", "memory_card_job",
		"memory_card_busy", "girl_last_date_spot_timer", "appointment_words",
		"last_picked_girl", "actor_state", "resources", "scene_work", "background_cache", "background_show"]
	for key: String in required:
		if not rule_state.has(key):
			return "school-day boundary missing field: " + key
	var arrays: Array[String] = ["calendar", "params", "girls", "progress_state", "day_events",
		"scene_flags", "icon_enabled", "girl_last_date_spot_timer", "appointment_words", "background_cache"]
	for key: String in required:
		if key in arrays:
			if not rule_state[key] is Array:
				return "school-day boundary array expected: " + key
		elif key in ["actor_state", "resources"]:
			if not rule_state[key] is Dictionary:
				return "school-day boundary record expected: " + key
		elif not rule_state[key] is int:
			return "school-day boundary integer expected: " + key
	if rule_state["params"].size() != 9 or rule_state["girls"].size() != 16 or rule_state["day_events"].size() != 32:
		return "school-day parameter/girl/queue dimensions differ from native records"
	if rule_state["calendar"].size() != 2 or rule_state["scene_flags"].size() != 252 or rule_state["progress_state"].size() != 156:
		return "school-day calendar/flag dimensions differ from native records"
	for value: Variant in rule_state["calendar"]:
		if not value is Dictionary:
			return "calendar record must be an object"
		var calendar: Dictionary = value
		if not calendar.get("day_kind") is Array or not calendar.get("marker") is Array or calendar["day_kind"].size() != 32 or calendar["marker"].size() != 32:
			return "calendar record needs two 32-byte arrays"
		for kind: String in ["day_kind", "marker"]:
			for item: Variant in calendar[kind]:
				if not item is int or item < 0 or item > 255:
					return "calendar entries must be bytes"
	for value: Variant in rule_state["girls"]:
		if not value is Dictionary:
			return "girl record must be an object"
		var girl: Dictionary = value
		for key: String in ["tokimeki_word", "affection_word", "syoushin_word", "flags", "birthday", "counters"]:
			if not girl.has(key):
				return "girl record missing field: " + key
			if key != "counters" and not girl[key] is int:
				return "girl word must be an integer: " + key
		if not girl["counters"] is Array or girl["counters"].size() != 36:
			return "girl scene-counter extent differs from native record"
		for item: Variant in girl["counters"]:
			if not item is int or item < 0 or item > 255:
				return "girl scene counters must be bytes"
	for event: Variant in rule_state["day_events"]:
		if not event is Array or event.size() != 4:
			return "day-event record must contain four bytes"
		for item: Variant in event:
			if not item is int or item < 0 or item > 255:
				return "day-event entries must be bytes"
	for key: String in ["params", "progress_state", "scene_flags", "icon_enabled", "girl_last_date_spot_timer", "appointment_words"]:
		for item: Variant in rule_state[key]:
			if not item is int:
				return "native integer array expected: " + key
	if rule_state["girl_last_date_spot_timer"].size() != 16 or rule_state["appointment_words"].size() != 8 or rule_state["icon_enabled"].size() != 15:
		return "date-spot/appointment/icon dimensions differ from native records"
	if rule_state["background_cache"].size() != 4 or rule_state["background_show"] not in [0, 1]:
		return "native background cache/show record is invalid"
	for item: Variant in rule_state["background_cache"]:
		if not item is int:
			return "background cache entries must be native LBAs"
	if rule_state["day_event_index"] < 0 or rule_state["day_event_index"] >= 32:
		return "day-event index outside native queue"
	for key: String in rule_state["actor_state"]:
		var actor: Variant = rule_state["actor_state"][key]
		if key not in ["96", "97"] or not actor is Dictionary or not actor.get("flags") is int or not actor.get("sequence_slot") is int or not actor.get("sequence") is Dictionary or not actor["sequence"].get("counter") is int:
			return "school-day actor state lacks native flags/slot/counter"
	return ""

func _plain_record(value: Variant, depth: int) -> bool:
	if depth > 16:
		return false
	if value is int or value is bool or value is String:
		return true
	if value is Array:
		for item: Variant in value:
			if not _plain_record(item, depth + 1):
				return false
		return true
	if value is Dictionary:
		for key: Variant in value:
			if not key is String or not _plain_record(value[key], depth + 1):
				return false
		return true
	return false

static func high(word: int) -> int:
	return Math.s16(Math.u32(word) >> 16)

static func with_high(word: int, integer_part: int) -> int:
	return Math.s32((Math.u16(integer_part) << 16) | (word & 0xffff))
