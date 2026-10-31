extends RefCounted
## T-39312: engine/hud_panels.gd on invented memory. Every expectation is derived by hand from src/main/hud_panels.c
## (not from the port); the emulator captures are replayed by the differential harness (config/port/engine-diff/hud_panels.json).

const GameEngine = preload("res://engine/engine.gd")

const ACTOR_SIZE: int = 0x44
const ACTOR_DRAW_FLAGS: int = 3 # Actor.draw_flags (include/game/display_records.h)
const QUAD_SIZE: int = 0x24 # SpriteQuad (include/game/*.h)

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
	_test_portrait()
	_test_status_bits()
	_test_parameter_change()
	_test_actor_flag_switches()
	_test_icons()
	_test_season_and_vacation()
	_test_title_hidden_forwards_a0()
	_test_refusal_names_the_callee()
	for engine in _made:
		engine.shutdown()
	_made.clear()
	return failures

func _actor(engine, index: int) -> int:
	return engine.memory.global_address("actor_pool") + index * ACTOR_SIZE

func _quad(engine, index: int) -> int:
	return engine.memory.global_address("sprite_quads") + index * QUAD_SIZE

func _test_portrait() -> void:
	var engine = _new()
	var m = engine.memory
	for index: int in [30, 31]:
		m.write_u8(_actor(engine, index) + ACTOR_DRAW_FLAGS, 0xFF)
	for index: int in 6:
		m.write_u32(_quad(engine, index), 0x01000000)
	engine.call_c("portrait_hide", [1]) # arg0 != 0: nothing happens
	_check(m.u8(_actor(engine, 31) + ACTOR_DRAW_FLAGS) == 0xFF and m.u32(_quad(engine, 5)) == 0x01000000, "portrait_hide(1) changes nothing")
	engine.call_c("portrait_hide", [0])
	_check(m.u8(_actor(engine, 31) + ACTOR_DRAW_FLAGS) == 0x7F and m.u8(_actor(engine, 30) + ACTOR_DRAW_FLAGS) == 0x7F, "portrait_hide(0) clears bit 7 of the draw flags of actors 30 and 31")
	for index: int in 6:
		_check(m.u32(_quad(engine, index)) == 0x81000000, "portrait_hide(0) sets bit 31 of sprite_quads[%d].flags" % index)
	# portrait_value_clear_if_high: (u32) D_800B5A64 >= 0xA5 clears it (u8 variable)
	m.write_global("D_800B5A64", 0xA4)
	engine.call_c("portrait_value_clear_if_high")
	_check(m.read_global("D_800B5A64") == 0xA4, "0xA4 stays")
	m.write_global("D_800B5A64", 0xA5)
	engine.call_c("portrait_value_clear_if_high")
	_check(m.read_global("D_800B5A64") == 0, "0xA5 is cleared")
	# portrait_expression_select: cases 0..5 set sprite_quads[1].unk_0C and [0].unk_0C to 7, 8, 9 (the odd cases add 0x40 to unk_0E)
	var expect_c: Array[int] = [7, 7, 8, 8, 9, 9]
	for variant: int in 6:
		for index: int in [0, 1]:
			m.write_u16(_quad(engine, index) + 0xC, 0)
			m.write_u8(_quad(engine, index) + 0xE, 0xD0)
		engine.call_c("portrait_expression_select", [variant])
		for index: int in [0, 1]:
			_check(m.u16(_quad(engine, index) + 0xC) == expect_c[variant], "portrait_expression_select(%d) sets unk_0C of quad %d to %d" % [variant, index, expect_c[variant]])
			var want_e: int = 0x10 if variant % 2 == 1 else 0xD0 # 0xD0 + 0x40 wraps in the u8
			_check(m.u8(_quad(engine, index) + 0xE) == want_e, "portrait_expression_select(%d): unk_0E of quad %d is 0x%X, got 0x%X" % [variant, index, want_e, m.u8(_quad(engine, index) + 0xE)])
	m.write_u16(_quad(engine, 1) + 0xC, 0x55)
	engine.call_c("portrait_expression_select", [6]) # no case 6
	_check(m.u16(_quad(engine, 1) + 0xC) == 0x55, "an expression variant above 5 changes nothing")

func _test_status_bits() -> void:
	var engine = _new()
	var m = engine.memory
	var gs = m.game_state()
	# bustup_speech: voice_mode (read as u8) == 0 and !(D_8011F50E & 1): D_8011F50E = (D_8011F50E & 0xFF) | 0x11
	gs.set_field("voice_mode", 0)
	m.write_global("D_8011F50E", 0x20)
	engine.call_c("bustup_speech")
	_check(m.read_global("D_8011F50E") == 0x31, "bustup_speech sets 0x11 into the flags")
	m.write_global("D_8011F50E", 0x21)
	engine.call_c("bustup_speech")
	_check(m.read_global("D_8011F50E") == 0x21, "bustup_speech leaves the flags alone when bit 0 is set")
	gs.set_field("voice_mode", 0x80) # an s8 field read as u8: non-zero
	m.write_global("D_8011F50E", 0x20)
	engine.call_c("bustup_speech")
	_check(m.read_global("D_8011F50E") == 0x20, "bustup_speech does nothing while voice playback is on (voice_mode 0x80 reads as non-zero)")
	m.write_global("D_8011F4CA", 0x40)
	engine.call_c("bustup_wink")
	_check(m.read_global("D_8011F4CA") == 0x51, "bustup_wink sets 0x11 when bit 0 is clear")
	m.write_global("D_8011F4CA", 0x01)
	engine.call_c("bustup_wink")
	_check(m.read_global("D_8011F4CA") == 0x01, "bustup_wink does nothing when bit 0 is set")

func _test_parameter_change() -> void:
	var engine = _new()
	var m = engine.memory
	var base: int = engine.gs_address("param_condition") # the nine records start at game_state + 0xFC
	# parameter_change(id, delta): word at +0xFC + 4*id += delta; a negative word becomes 0; an s16 at +0xFE >= 0x3E8 becomes 0x3E7
	m.write_u32(base + 4, 100)
	engine.call_c("parameter_change", [1, -150])
	_check(m.u32(base + 4) == 0, "a negative result is clamped to 0")
	m.write_u32(base + 8, 0x03E70000)
	engine.call_c("parameter_change", [2, 0x10000])
	_check(m.u32(base + 8) == 0x03E70000 | 0x00000000 and m.s16(base + 8 + 2) == 0x3E7, "the high half 0x3E8 is capped to 0x3E7 (word 0x03E70000): got 0x%X" % m.u32(base + 8))
	m.write_u32(base, 0x7FFFFFFF)
	engine.call_c("parameter_change", [0, 1]) # the s32 sum wraps negative, then clamps to 0
	_check(m.u32(base) == 0, "0x7FFFFFFF + 1 wraps to a negative word and becomes 0")
	m.write_u32(base + 12, 0x00010005)
	engine.call_c("parameter_change", [3, 7])
	_check(m.u32(base + 12) == 0x0001000C, "a normal change adds the delta to the whole word")

func _test_actor_flag_switches() -> void:
	var engine = _new()
	var m = engine.memory
	for index: int in 100:
		m.write_u8(_actor(engine, index) + ACTOR_DRAW_FLAGS, 0x0F)
	engine.call_c("parameter_disp_switch", [1])
	for index: int in 100:
		var in_group: bool = index >= 32 and index < 60
		_check(m.u8(_actor(engine, index) + ACTOR_DRAW_FLAGS) == (0x8F if in_group else 0x0F), "parameter_disp_switch(1) sets bit 7 of actors 32..59 only (actor %d)" % index)
	engine.call_c("parameter_disp_switch", [2]) # anything but 1 hides
	_check(m.u8(_actor(engine, 32) + ACTOR_DRAW_FLAGS) == 0x0F and m.u8(_actor(engine, 59) + ACTOR_DRAW_FLAGS) == 0x0F, "parameter_disp_switch(2) clears bit 7")
	engine.call_c("hizuke_disp_switch", [1])
	_check(m.u8(_actor(engine, 16) + ACTOR_DRAW_FLAGS) == 0x8F and m.u8(_actor(engine, 23) + ACTOR_DRAW_FLAGS) == 0x8F and m.u8(_actor(engine, 24) + ACTOR_DRAW_FLAGS) == 0x0F and m.u8(_actor(engine, 15) + ACTOR_DRAW_FLAGS) == 0x0F, "hizuke_disp_switch(1) sets bit 7 of actors 16..23")
	engine.call_c("hizuke_disp_switch", [0])
	_check(m.u8(_actor(engine, 16) + ACTOR_DRAW_FLAGS) == 0x0F, "hizuke_disp_switch(0) clears it")
	var flags: int = m.global_address("D_8011ED17")
	m.write_u8(flags, 0x04)
	engine.call_c("message_disp_switch", [1])
	_check(m.u8(flags) == 0x84, "message_disp_switch(1) sets bit 7")
	engine.call_c("message_disp_switch", [0])
	_check(m.u8(flags) == 0x04, "message_disp_switch(0) clears bit 7")

func _test_icons() -> void:
	var engine = _new()
	var m = engine.memory
	var table: int = m.global_address("icon_enabled")
	engine.call_c("icon_can_use_set", [3, 5]) # any non-zero value enables
	_check(m.u8(table + 3) == 1 and engine.call_c("get_icon_can_use", [3]) == 1, "icon_can_use_set(3, 5) stores 1")
	engine.call_c("icon_can_use_set", [3, 0])
	_check(m.u8(table + 3) == 0 and engine.call_c("get_icon_can_use", [3]) == 0, "icon_can_use_set(3, 0) stores 0")
	m.write_u8(table + 4, 2) # get_icon_can_use compares with 1
	_check(engine.call_c("get_icon_can_use", [4]) == 0, "a stored value other than 1 reads as disabled")
	# hud_icon_memory_card_animation_set(arg0): D_8012059A[0] = 0xE (arg0 != 0) or 0xD; D_8012059A[1] reset when >= 4 (or >= 6)
	var words: int = m.global_address("D_8012059A")
	m.write_u16(words + 2, 4)
	engine.call_c("hud_icon_memory_card_animation_set", [1])
	_check(m.s16(words) == 0xE and m.s16(words + 2) == 0, "animation set (1): 0xE, counter 4 resets")
	m.write_u16(words + 2, 3)
	engine.call_c("hud_icon_memory_card_animation_set", [0x100]) # a u8 parameter: 0x100 is 0
	_check(m.s16(words) == 0xD and m.s16(words + 2) == 3, "animation set (0x100 narrows to 0): 0xD, counter 3 stays")
	m.write_u16(words + 2, 6)
	engine.call_c("hud_icon_memory_card_animation_set", [0])
	_check(m.s16(words + 2) == 0, "counter 6 resets in the second form")
	# icon_disp_switch: icon_count records of 0x44 bytes: byte 0x1543 and 0x1103 from the pool base get bit 7
	var pool: int = m.global_address("actor_pool")
	engine.gs_set("icon_count", 3)
	engine.call_c("icon_disp_switch", [1])
	for i: int in 4:
		var set: bool = i < 3
		_check(m.u8(pool + 0x1543 + i * ACTOR_SIZE) == (0x80 if set else 0) and m.u8(pool + 0x1103 + i * ACTOR_SIZE) == (0x80 if set else 0), "icon_disp_switch(1) sets bit 7 for icons 0..2 only (icon %d)" % i)
	engine.call_c("icon_disp_switch", [0])
	_check(m.u8(pool + 0x1543) == 0 and m.u8(pool + 0x1103 + 2 * ACTOR_SIZE) == 0, "icon_disp_switch(0) clears bit 7")
	engine.gs_set("icon_count", 0)
	m.write_u8(pool + 0x1543, 0x80)
	engine.call_c("icon_disp_switch", [0])
	_check(m.u8(pool + 0x1543) == 0x80, "icon_disp_switch with icon_count 0 touches nothing")

func _test_season_and_vacation() -> void:
	var engine = _new()
	var season: Array[int] = [0, 3, 3, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 0]
	var summer: Array[int] = [0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0]
	for month: int in 14:
		engine.gs_set("month", month)
		_check(engine.call_c("hud_season_index") == season[month], "hud_season_index(month %d) = %d" % [month, season[month]])
		_check(engine.call_c("hud_in_summer_months") == summer[month], "hud_in_summer_months(month %d) = %d" % [month, summer[month]])
	# hud_summer_phase: the summer flag, -1 from day 0x18 of September and from day 0x19 of May
	var cases: Array = [[9, 0x17, 1], [9, 0x18, -1], [5, 0x18, 0], [5, 0x19, -1], [6, 0xFF, 1], [10, 0x20, 0]]
	for c: Array in cases:
		engine.gs_set("month", c[0])
		engine.gs_set("day", c[1])
		_check(engine.call_c("hud_summer_phase") == c[2], "hud_summer_phase(month %d, day %d) = %d" % c)
	var vacation: Array = [[8, 1, 1], [7, 0x17, 0], [7, 0x18, 1], [0xC, 0x19, 2], [0xC, 0x18, 0], [0xD, 7, 2], [0xD, 8, 0], [1, 7, 2],
		[1, 8, 0], [3, 0x18, 3], [3, 0x17, 0], [4, 3, 3], [4, 4, 0], [2, 1, 0]]
	for c: Array in vacation:
		_check(engine.call_c("hud_vacation_for_date", [c[0], c[1]]) == c[2], "hud_vacation_for_date(%d, %d) = %d" % c)

## message_window_set_title_hidden(): the C definition is (void) but every caller passes the title and the function
## forwards a0 to message_window_set_title (the wrapper is a pass-through), then sets D_80125CA0 = 0xB8.
func _test_title_hidden_forwards_a0() -> void:
	var engine = _new()
	var seen: Array = []
	engine.set_stub("message_window_set_title", func(args: Array) -> int:
		seen.append(args.duplicate())
		return 0)
	engine.call_c("message_window_set_title_hidden", [0x800B1590])
	_check(seen == [[0x800B1590]], "the title pointer is forwarded: %s" % [seen])
	_check(engine.memory.read_global("D_80125CA0") == 0xB8, "D_80125CA0 = 0xB8 after the call")

func _test_refusal_names_the_callee() -> void:
	var engine = _new()
	var calls: Array = []
	engine.set_stub("prim_gradient_rect", func(args: Array) -> int:
		calls.append(["prim_gradient_rect", args.duplicate()])
		return 0)
	engine.set_stub("dtd_on", func(args: Array) -> int:
		calls.append(["dtd_on", args.duplicate()])
		return 0)
	# hizuke_show: with bit 7 of D_8011F113 set, prim_gradient_rect(0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0); dtd_on(5)
	engine.memory.write_global("D_8011F113", 0x80)
	engine.call_c("hizuke_show")
	_check(calls == [["prim_gradient_rect", [0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0]], ["dtd_on", [5]]], "hizuke_show draws the date panel: %s" % [calls])
	calls.clear()
	engine.memory.write_global("D_8011F113", 0x7F)
	engine.call_c("hizuke_show")
	_check(calls.is_empty(), "without the flag hizuke_show draws nothing")
	# message_window_show: bit 7 of D_8011ED17: prim_gradient_rect(-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0); dtd_on(8)
	engine.memory.write_global("D_8011ED17", 0x80)
	engine.call_c("message_window_show")
	_check(calls == [["prim_gradient_rect", [-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0]], ["dtd_on", [8]]], "message_window_show draws the window: %s" % [calls])
	# with the stub removed and the function forgotten, the call refuses by name and leaves the memory as it was at the call
	engine.clear_stubs()
	engine._functions.erase("prim_gradient_rect")
	engine._callables.erase("prim_gradient_rect")
	engine.memory.write_global("D_8011F113", 0x80)
	engine.call_c("hizuke_show")
	_check(engine.halted() and engine.refusal.begins_with("hizuke_show -> prim_gradient_rect"), "an unported callee is refused by name: %s" % engine.refusal)
