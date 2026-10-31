extends RefCounted
## T-39300 (M1): core/rule_state_bridge.gd with an invented rule_state (synthetic values,
## the key set and shapes of flow/new_game_state_adapter.gd). The bridge must round-trip
## both ways, keep a mirrored state untouched, and leave port-only keys alone.

const Math = preload("res://core/native_math.gd")
const Bridge = preload("res://core/rule_state_bridge.gd")
const Memory = preload("res://core/game_memory.gd")
const Layout = preload("res://core/game_layout.gd")
const RouteState = preload("res://rules/new_game_route_state.gd")

var failures: Array[String] = []
var _seed: int = 12345

func run() -> Array[String]:
	failures.clear()
	_test_addresses()
	_test_round_trip()
	_test_pull_is_noop_on_mirror()
	_test_memory_edit_reaches_state()
	_test_core_fields()
	_test_key_coverage()
	_test_port_only_keys_untouched()
	return failures

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func _rand(limit: int) -> int:
	_seed = (_seed * 1103515245 + 12345) & 0x7fffffff
	return (_seed >> 8) % limit

func _bytes(count: int) -> Array:
	var out: Array = []
	for i: int in count:
		out.append(_rand(256))
	return out

func _fixture() -> Dictionary:
	var calendars: Array = []
	for i: int in 2:
		calendars.append({"day_kind": _bytes(32), "marker": _bytes(32)})
	var params: Array = []
	for i: int in 9:
		params.append(Math.s32((_rand(1000) << 16) | _rand(65536)))
	var girls: Array = []
	for i: int in 16:
		girls.append({"tokimeki_word": Math.s32(_rand(1 << 30) * 4 - (1 << 31)), "affection_word": _rand(1 << 20),
			"syoushin_word": -_rand(1 << 20), "flags": _rand(1 << 30) * 4, "birthday": _rand(1 << 30) * 4 + 3,
			"counters": _bytes(36)})
	var events: Array = []
	for i: int in 32:
		events.append(_bytes(4))
	var clubs: Array = []
	for i: int in 12:
		clubs.append({"practice_points": _rand(65536), "level": _rand(256), "flags": _rand(256)})
	var log: Array = []
	for i: int in 256:
		var entry: Dictionary = {}
		for key: String in Bridge.EVENT_LOG_FIELDS:
			entry[key] = _rand(256)
		log.append(entry)
	var disabled: Array = []
	for i: int in 14:
		disabled.append(_rand(2))
	var words: Array = []
	for i: int in 8:
		words.append(_rand(1 << 30) * 4)
	return {
		"year": 0x5f, "month": 4, "day": 17, "weekday": 3, "day_count": 0x1f2,
		"calendar": calendars, "params": params, "player_flags": 0xabcd, "player_flags_upper": 0x1234,
		"player_birthday": 0xf0000211, "girls": girls, "progress_state": _bytes(156),
		"day_events": events, "day_event_index": 9, "scene_flags": _bytes(252),
		"day_event_done_bits": 0x81, "day_event_count_today": 2, "daily_check_done": 1,
		"last_command": 0xfedcba98, "menu_cursor": -1, "menu_mode": 2, "menu_disabled": disabled,
		"icon_enabled": _bytes(15), "vacation": 3, "sd_anim_fast": 1, "memory_card_job": 5,
		"memory_card_busy": 1, "girl_last_date_spot_timer": _bytes(16), "appointment_words": words,
		"clubs": clubs, "event_log": log, "event_log_count": 200, "last_picked_girl": -1,
		"actor_state": {"96": {"flags": 1}}, "resources": {"available_commands": [1, 2]},
		"scene_work": -1024, "background_cache": [70000, -5, 0, 123456], "background_show": 1,
		"profile_family_bytes": _bytes(8), "profile_given_bytes": _bytes(8),
		"profile_nickname_bytes": _bytes(16), "voice_wait_frames": 4, "message_title_visible": true,
		"current_girl": 6,
	}

func _state() -> RouteState:
	var state := RouteState.new()
	state.rule_state = _fixture()
	state.opening = {"icon_mode": 1, "icon_count": 14, "screen_brightness": 0x80, "origin": "synthetic"}
	state.game_mode = 0x34
	state.mode_stage = 1
	state.frame_count = 21333
	return state

func _test_addresses() -> void:
	var state := _state()
	var mem := Memory.new()
	Bridge.push(state, mem)
	_check(mem.u8(0x800e6280 + 0x3e) == 0x5f and mem.u8(0x800e6280 + 0x3f) == 4 and mem.u16(0x800e6280 + 0x42) == 0x1f2, "calendar bytes")
	_check(mem.u8(0x800e6280 + 0x1108) == 0x34 and mem.u8(0x800e6280 + 0x1109) == 1, "mode bytes")
	_check(mem.u32(0x800e6280 + 0x10f8) == 21333, "frame_count word")
	_check(mem.s32(0x800e6280 + 0xfc) == state.rule_state["params"][0], "param word 0")
	_check(mem.s32(0x800e6280 + 0x1bc + 5 * 0x38 + 4) == state.rule_state["girls"][5]["affection_word"], "girls[5].affection word")
	_check(mem.s16(0x800e6280 + 0x1bc + 5 * 0x38 + 6) == Math.s16(state.rule_state["girls"][5]["affection_word"] >> 16), "affection value is the high half")
	_check(mem.s8(0x800e6280 + 0x1093) == -1 and mem.u8(0x800e6280 + 0x1092) == 2, "menu bytes")
	_check(mem.u8(0x800e6280 + 0xf90 + 5 * 8 + 7) == state.rule_state["menu_disabled"][5], "menu_items[5].disabled")
	_check(mem.u8(0x800e6280 + 0x75e + 3 * 8 + 6) == state.rule_state["event_log"][3]["day"], "event_log[3].day")
	_check(mem.s8(0x800b6740) == -1 and mem.s32(0x801230d0) == -1024, "globals outside GameState")
	_check(mem.u8(0x80125c90 + 14) == state.rule_state["icon_enabled"][14], "icon_enabled[14]")
	_check(mem.s32(0x800b5920 + 4) == -5 and mem.u8(0x800e6280 + 0xf6c) == 1 and mem.u8(0x800e6280 + 0xf6d) == 14, "background cache and opening fields")
	_check(mem.u8(0x800e6280 + 0xf5f) == 6 and mem.u8(0x800e6280 + 0xf5e) == 200, "current_girl, event_log_count")
	_check(mem.error.is_empty(), "push faults: " + mem.error)

func _test_round_trip() -> void:
	var source := _state()
	var mem := Memory.new()
	Bridge.push(source, mem)
	# A different state with every mapped value scrambled must become the source again.
	var other := RouteState.new()
	other.rule_state = _fixture()
	other.opening = {"icon_mode": 0, "icon_count": 0, "screen_brightness": 7, "origin": "synthetic"}
	_check(JSON.stringify(other.rule_state) != JSON.stringify(source.rule_state), "scrambled state differs")
	Bridge.pull(mem, other)
	for key: String in Bridge.MAPPED_KEYS:
		_check(JSON.stringify(other.rule_state[key]) == JSON.stringify(source.rule_state[key]), "memory -> state restores key " + key)
	_check(other.game_mode == 0x34 and other.mode_stage == 1 and other.frame_count == 21333, "core fields restored")
	_check(other.opening["icon_count"] == 14 and other.opening["origin"] == "synthetic", "opening fields restored, others kept")
	var mem2 := Memory.new()
	Bridge.push(other, mem2)
	_check(mem.ram == mem2.ram, "state -> memory is stable after the trip")
	_check(mem.error.is_empty() and mem2.error.is_empty(), "round trip faults")

func _test_pull_is_noop_on_mirror() -> void:
	var state := _state()
	var mem := Memory.new()
	Bridge.push(state, mem)
	var before_words: Array[int] = state.to_words()
	var before_json: String = JSON.stringify(state.rule_state)
	Bridge.pull(mem, state)
	_check(JSON.stringify(state.rule_state) == before_json, "pull on a mirrored memory keeps rule_state")
	_check(state.to_words() == before_words, "pull on a mirrored memory keeps the state hash words")
	# A producer that spells a word as unsigned instead of signed keeps its spelling.
	state.rule_state["girls"][2]["tokimeki_word"] = Math.u32(state.rule_state["girls"][2]["tokimeki_word"])
	var spelled: int = state.rule_state["girls"][2]["tokimeki_word"]
	Bridge.push(state, mem)
	Bridge.pull(mem, state)
	_check(state.rule_state["girls"][2]["tokimeki_word"] == spelled, "equal values keep their representation")

func _test_memory_edit_reaches_state() -> void:
	var state := _state()
	var mem := Memory.new()
	Bridge.push(state, mem)
	mem.write_s16(0x800e6280 + 0x1bc + 2 * 0x38 + 2, 321)
	mem.write_u8(0x800e6280 + 0x3f, 5)
	mem.write_u8(0x800e6280 + 0x56c + 10, 77)
	mem.write_s32(0x800b5920, 99)
	mem.write_u8(0x800e6280 + 0x1110, 1)
	Bridge.pull(mem, state)
	_check(Math.s16(state.rule_state["girls"][2]["tokimeki_word"] >> 16) == 321, "girl word high half edited in memory")
	_check(state.rule_state["month"] == 5 and state.rule_state["scene_flags"][10] == 77, "month and scene flag edited in memory")
	_check(state.rule_state["background_cache"][0] == 99 and state.paused, "global and core flag edited in memory")

func _test_core_fields() -> void:
	var state := _state()
	state.system_seed = -123456
	state.timer_ticks = 0x7fffffff
	state.game_mode_scratch = -2
	state.paused = true
	state.sound_active = true
	state.mode_step_timer = -7
	var mem := Memory.new()
	Bridge.push(state, mem)
	_check(mem.s32(0x800e7d10) == -123456 and mem.s32(0x800e6280 + 0x10f4) == 0x7fffffff, "seed and timer words")
	_check(mem.u8(0x800e6280 + 0x110b) == 0xfe and mem.u8(0x800e6280 + 0x1110) == 1 and mem.u8(0x800e6280 + 0x34) == 1, "scratch byte and flags")
	var other := RouteState.new()
	other.rule_state = _fixture()
	Bridge.pull(mem, other)
	_check(other.system_seed == -123456 and other.timer_ticks == 0x7fffffff and other.game_mode_scratch == -2, "core fields come back")
	_check(other.paused and other.sound_active and not other.restart_request and other.mode_step_timer == -7, "core flags and signed timers")

func _test_key_coverage() -> void:
	var fixture: Dictionary = _fixture()
	for key: String in fixture:
		_check(key in Bridge.MAPPED_KEYS or key in Bridge.PORT_ONLY_KEYS, "key neither mapped nor listed port-only: " + key)
	for key: String in Bridge.MAPPED_KEYS:
		_check(fixture.has(key), "mapped key missing from the fixture: " + key)
		_check(key not in Bridge.PORT_ONLY_KEYS, "key is both mapped and port-only: " + key)

func _test_port_only_keys_untouched() -> void:
	var state := _state()
	var mem := Memory.new()
	Bridge.push(state, mem)
	var kept: String = JSON.stringify([state.rule_state["actor_state"], state.rule_state["resources"],
		state.rule_state["voice_wait_frames"], state.rule_state["message_title_visible"]])
	mem.fill(0x800e6280, 0x1a90, 0xa5)
	Bridge.pull(mem, state)
	_check(JSON.stringify([state.rule_state["actor_state"], state.rule_state["resources"],
		state.rule_state["voice_wait_frames"], state.rule_state["message_title_visible"]]) == kept, "port-only keys survive a pull")
	_check(not state.rule_state.has("overlay_state"), "pull adds no keys")
