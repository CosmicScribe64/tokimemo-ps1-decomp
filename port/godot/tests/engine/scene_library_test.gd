extends RefCounted
## T-39312: engine/scene_library.gd on invented memory. Every expectation is derived by hand from
## src/main/scene_library.c (not from the port); the emulator captures are replayed by the differential harness
## (config/port/engine-diff/scene_library.json).

const GameEngine = preload("res://engine/engine.gd")

const QUAD_SIZE: int = 0x24
const ACTOR_SIZE: int = 0x44

var failures: Array[String] = []
var _made: Array = []

func _new():
	var engine = GameEngine.new()
	_made.append(engine)
	return engine

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	_test_step_functions()
	_test_sprite_brightness()
	_test_check_para_limit()
	_test_event_log_add()
	_test_day_plus()
	_test_cd_level()
	_test_don_wait()
	_shutdown()
	return failures

func _shutdown() -> void:
	for engine in _made:
		engine.shutdown()
	_made.clear()

## The one-step scene commands: store(s), then mode_step_next (mode_step + 1, step timer and substep cleared).
func _test_step_functions() -> void:
	var engine = _new()
	var m = engine.memory
	engine.gs_set("date_girl", 7)
	engine.gs_set("current_girl", 2)
	engine.gs_set("mode_step", 4)
	engine.call_c("change_girl")
	_check(engine.gs_get("current_girl") == 7 and engine.gs_get("mode_step") == 5, "change_girl: current_girl = date_girl, then the next step")
	engine.call_c("change_yoshio")
	_check(engine.gs_get("current_girl") == 13 and engine.gs_get("mode_step") == 6, "change_yoshio: current_girl = GIRL_YOSHIO (13)")
	m.write_global("D_80120666", 99)
	m.write_global("D_801206AA", 5)
	engine.call_c("event_face0")
	_check(m.read_global("D_80120666") == 2 and m.read_global("D_801206AA") == 0 and engine.gs_get("mode_step") == 7, "event_face0: D_80120666 = 2, D_801206AA = 0")
	m.write_global("D_80120666", 99)
	engine.call_c("event_face_kuchi0")
	_check(m.read_global("D_80120666") == 2 and engine.gs_get("mode_step") == 8, "event_face_kuchi0: D_80120666 = 2")
	m.write_global("D_80122D10", 9)
	m.write_global("D_80120652", 9)
	engine.call_c("event_kuchi_ani_no")
	_check(m.read_global("D_80122D10") == 0 and m.read_global("D_80120652") == 4 and engine.gs_get("mode_step") == 9, "event_kuchi_ani_no: D_80122D10 = 0, D_80120652 = 4")
	# hizuke_hide: hizuke_disp_switch(0), then the next step
	var pool: int = m.global_address("actor_pool")
	m.write_u8(pool + 16 * ACTOR_SIZE + 3, 0xFF)
	engine.call_c("hizuke_hide")
	_check(m.u8(pool + 16 * ACTOR_SIZE + 3) == 0x7F and engine.gs_get("mode_step") == 10, "hizuke_hide clears bit 7 of actor 16 and steps")
	# face_expression_N: portrait_layout_set(N), then the next step (stubbed here)
	var seen: Array = []
	engine.set_stub("portrait_layout_set", func(args: Array) -> int:
		seen.append(args.duplicate())
		return 0)
	for index: int in 5:
		engine.call_c("face_expression_%d" % index)
	_check(seen == [[0], [1], [2], [3], [4]] and engine.gs_get("mode_step") == 15, "face_expression_0..4 call portrait_layout_set(0..4) and step: %s" % [seen])
	seen.clear()
	engine.set_stub("get_g_zyotai_h", func(args: Array) -> int:
		return 0x185 if args == [6] else -1)
	engine.gs_set("current_girl", 6)
	engine.call_c("face_expression_mood")
	_check(seen == [[5]], "face_expression_mood passes get_g_zyotai_h(current_girl) & 0x7F: %s" % [seen])

func _test_sprite_brightness() -> void:
	var engine = _new()
	var m = engine.memory
	var base: int = m.global_address("sprite_quads")
	engine.call_c("sprite_brightness", [3, 0x1A5]) # u8 parameter: 0xA5
	for offset: int in [0x14, 0x15, 0x16]:
		_check(m.u8(base + 3 * QUAD_SIZE + offset) == 0xA5, "sprite_brightness writes the low byte of the brightness at +0x%X" % offset)
	_check(m.u8(base + 3 * QUAD_SIZE + 0x17) == 0 and m.u8(base + 2 * QUAD_SIZE + 0x16) == 0, "and nothing around it")
	engine.call_c("sprite_brightness", [-1, 7]) # s16 index -1: the previous record (index * 36)
	_check(m.u8(base - QUAD_SIZE + 0x14) == 7, "a negative s16 index addresses backwards")

func _test_check_para_limit() -> void:
	var engine = _new()
	var m = engine.memory
	var base: int = engine.gs_address("param_condition")
	# nine words: a negative s16 value (high half) -> 0; a value >= 0x3E7 -> word 0x03E70000; otherwise unchanged
	var words: Array = [0xFFFF0007, 0x03E70000, 0x03E80001, 0x03E60005, 0x00000000, 0x7FFF0000, 0x80000000, 0x00010002, 0x03E7FFFF]
	var want: Array = [0, 0x03E70000, 0x03E70000, 0x03E60005, 0, 0x03E70000, 0, 0x00010002, 0x03E70000]
	for i: int in 9:
		m.write_u32(base + 4 * i, words[i])
	engine.call_c("check_para_limit")
	for i: int in 9:
		_check(m.u32(base + 4 * i) == want[i], "check_para_limit word %d: 0x%X -> 0x%X, got 0x%X" % [i, words[i], want[i], m.u32(base + 4 * i)])

func _test_event_log_add() -> void:
	var engine = _new()
	var m = engine.memory
	var log_base: int = engine.gs_address("event_log")
	var override: int = engine.modules.scene_library.G_TALK_RANK_OVERRIDE # not in the generated layout (T-39312 ticket: missing data symbols)
	engine.set_stub("get_g_zyotai_h", func(_args: Array) -> int: return 3)
	engine.gs_set("current_girl", 5)
	engine.gs_set("year", 2)
	engine.gs_set("month", 11)
	engine.gs_set("day", 23)
	engine.gs_set("weekday", 4)
	engine.gs_set("event_log_count", 10)
	engine.call_c("event_log_add", [0x1FF, 0x102]) # u8 parameters: 0xFF and 0x02
	var rec: int = log_base + 10 * 8
	_check([m.u8(rec), m.u8(rec + 1), m.u8(rec + 2), m.u8(rec + 3), m.u8(rec + 4), m.u8(rec + 5), m.u8(rec + 6), m.u8(rec + 7)] == [5, 2, 0xFF, 3, 2, 11, 23, 4], "event_log_add record: girl, flag, scene, rank (zyotai 3), year, month, day, weekday")
	_check(engine.gs_get("event_log_count") == 11 and m.s32(override) == 0, "the count advances and talk_rank_override is cleared")
	# talk_rank_override 1..6 replaces the rank by override - 1; 7 and above are ignored
	m.write_u32(override, 4)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 11 * 8 + 3) == 3, "override 4 gives rank 3 (override - 1)")
	m.write_u32(override, 6)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 12 * 8 + 3) == 5, "override 6 gives rank 5")
	m.write_u32(override, 7)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 13 * 8 + 3) == 3, "override 7 is ignored: the zyotai rank")
	# the rival (GIRL_REI = 12) always gets rank 0
	engine.gs_set("current_girl", 12)
	engine.call_c("event_log_add", [1, 0])
	_check(m.u8(log_base + 14 * 8 + 3) == 0, "GIRL_REI: rank 0")
	# the count saturates at 0xFE when it reaches 0xFF
	engine.gs_set("event_log_count", 0xFD)
	engine.call_c("event_log_add", [1, 0])
	_check(engine.gs_get("event_log_count") == 0xFE, "0xFD + 1 = 0xFE stays")
	engine.call_c("event_log_add", [1, 0])
	_check(engine.gs_get("event_log_count") == 0xFE, "0xFE + 1 = 0xFF is replaced by 0xFE")

func _test_day_plus() -> void:
	var engine = _new()
	var order: Array = []
	for name: String in ["calendar_advance_day", "hizuke_init", "hizuke_disp_switch", "hizuke_show"]:
		engine.set_stub(name, func(args: Array) -> int:
			order.append([name, args.duplicate()])
			return 0)
	engine.memory.write_global("scene_trip_day_state", 5)
	engine.memory.write_global("D_8011F113", 0x80)
	engine.gs_set("mode_step", 1)
	engine.call_c("day_plus")
	_check(order == [["calendar_advance_day", []], ["hizuke_init", []], ["hizuke_disp_switch", [1]], ["hizuke_show", []]], "day_plus with the date shown: %s" % [order])
	_check(engine.memory.read_global("scene_trip_day_state") == 0 and engine.gs_get("mode_step") == 2, "scene_trip_day_state cleared, next step")
	order.clear()
	engine.memory.write_global("D_8011F113", 0x7F)
	engine.call_c("day_plus")
	_check(order[2] == ["hizuke_disp_switch", [0]], "day_plus with the date hidden passes 0: %s" % [order])

func _test_cd_level() -> void:
	var engine = _new()
	var m = engine.memory
	m.write_global("audio_cd_level", 0x30)
	m.write_global("D_80125E62", 0x11)
	m.write_u32(m.global_address("audio_state"), 0)
	_check(engine.call_c("getCDlevel") == 0, "getCDlevel is 0 without bit 0x400 of state_flags")
	m.write_u32(m.global_address("audio_state"), 0x400)
	_check(engine.call_c("getCDlevel") == (0x30 + 0x11) >> 1, "getCDlevel = (audio_cd_level + D_80125E62) >> 1")
	m.write_global("audio_cd_level", 0xFFFF)
	m.write_global("D_80125E62", 0xFFFF)
	_check(engine.call_c("getCDlevel") == -1, "(u32)(0xFFFF + 0xFFFF) >> 1 = 0xFFFF, returned as an s16: -1")

## don_wait: after 0x81 frames of the wait counter, k_reset(1) and the next step.
func _test_don_wait() -> void:
	var engine = _new()
	var seen: Array = []
	engine.set_stub("k_reset", func(args: Array) -> int:
		seen.append(args.duplicate())
		return 0)
	var counter: int = engine.gs_address("mode_step_timer")
	engine.gs_set("mode_step", 3)
	engine.memory.write_u32(counter, 0x7F)
	engine.call_c("don_wait")
	_check(seen.is_empty() and engine.memory.u32(counter) == 0x80 and engine.gs_get("mode_step") == 3, "don_wait counts up below 0x81 and does nothing else")
	engine.memory.write_u32(counter, 0x80) # the test reads the old value (0x80) +1 -> 0x81
	engine.call_c("don_wait")
	_check(seen.is_empty(), "the old value 0x80 is still below 0x81")
	engine.call_c("don_wait")
	_check(seen == [[1]] and engine.gs_get("mode_step") == 4, "at 0x81: k_reset(1), then the next step: %s step %d" % [seen, engine.gs_get("mode_step")])
