extends RefCounted
## T-39311: engine/actor_sprites.gd on invented memory. Expectations are derived by hand from
## src/main/actor_sprites.c. load_csr_tp, printf and func_80098680 are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF

func run() -> Array[String]:
	s.failures.clear()
	_test_shake()
	_test_brightness()
	_test_tpage_buffer()
	_test_searches()
	_test_actor_clear()
	_test_actor_draw_all()
	s.shutdown_all()
	return s.failures

func _tb(e, i: int) -> int:
	return e.gs_address("tpage_buf") + i * 4

func _ss(e, i: int) -> int:
	return e.gs_address("tpage_slot_state") + i * 4

func _test_shake() -> void:
	var e = s.new_engine()
	var m = e.memory
	e.call_c("draw_shake_start")
	s.check(m.read_global("draw_shake_counter") == 4, "draw_shake_start: counter 4")
	m.write_global("draw_shake_counter", 0)
	e.call_c("draw_shake_update")
	var ox: int = e.gs_address("draw_origin_x")
	var oy: int = e.gs_address("draw_origin_y")
	s.check([m.s16(ox), m.s16(ox + 2), m.s16(oy), m.s16(oy + 2), m.read_global("draw_shake_x"), m.read_global("draw_shake_y")] == [0xA0, 0xA0, 0x168, 0x78, 0xA0, 0x78], "idle: origins (0xA0, 0x168), (0xA0, 0x78), shake position (0xA0, 0x78)")
	# counter 0x10: sum = 0 + 1 + ... + 15 = 120; q = rsin(480) / 4 / (0x10 + 0x100)
	m.write_global("draw_shake_counter", 0x10)
	e.call_c("draw_shake_update")
	var q: int = Math.rsin(480) / 4 / 0x110
	s.check(q != 0, "the test needs a nonzero shake (rsin(480) = %d)" % Math.rsin(480))
	s.check([m.s16(ox), m.s16(ox + 2), m.s16(oy), m.s16(oy + 2), m.read_global("draw_shake_x"), m.read_global("draw_shake_y"), m.read_global("draw_shake_counter")]
		== [0xA0 + q, 0xA0 + q, 0x168 + q, 0x78 + q, 0xA0 + q, 0x78 + q, 0x14], "running: origin moves by q, counter += 4")
	m.write_global("draw_shake_counter", 0x7C)
	e.call_c("draw_shake_update")
	s.check(m.read_global("draw_shake_counter") == 0x80, "0x7C + 4 = 0x80 is not above 0x80")
	e.call_c("draw_shake_update")
	s.check(m.read_global("draw_shake_counter") == 0, "0x80 + 4 > 0x80: counter cleared")

func _test_brightness() -> void:
	var e = s.new_engine()
	e.gs_set("screen_brightness", 0xF0)
	e.gs_set("brightness_step", 0x20)
	e.call_c("draw_brightness_step")
	s.check(e.call_c("draw_brightness_get") == 0xFF, "0xF0 + 0x20 >= 0x100: 0xFF")
	e.gs_set("screen_brightness", 0x10)
	e.call_c("draw_brightness_step")
	s.check(e.gs_get("screen_brightness") == 0x30, "0x10 + 0x20")
	e.gs_set("screen_brightness", 5)
	e.gs_set("brightness_step", 0xF0) # s8 -16
	e.call_c("draw_brightness_step")
	s.check(e.gs_get("screen_brightness") == 0, "5 - 16 < 0: 0")
	e.gs_set("screen_brightness", 0x40)
	e.call_c("draw_brightness_step")
	s.check(e.gs_get("screen_brightness") == 0x30, "0x40 - 16")
	e.gs_set("brightness_step", 0)
	e.call_c("draw_brightness_step")
	s.check(e.gs_get("screen_brightness") == 0x30, "step 0: unchanged")
	e.call_c("draw_brightness_set_rate", [0x1FF])
	s.check(e.gs_get("brightness_step") == 0xFF, "set_rate stores the low byte")
	e.call_c("draw_brightness_reset")
	s.check(e.gs_get("screen_brightness") == 0x80 and e.gs_get("brightness_step") == 0, "reset: 0x80, step 0")

func _test_tpage_buffer() -> void:
	var e = s.new_engine()
	var m = e.memory
	e.call_c("tpage_buf_clear_all")
	var ok: bool = true
	for i: int in 64:
		ok = ok and m.s32(_tb(e, i)) == (0 if (i & 0xF) < 5 else -1)
	s.check(ok, "tpage_buf_clear_all: 0 for i & 0xF < 5, -1 otherwise")
	m.write_global("D_800C9734", 0x61)
	m.write_global("D_800C9738", 0x62)
	m.write_global("D_800C973C", 0x63)
	m.write_global("D_800C974C", 0x31)
	m.write_global("D_800C9750", 0x30)
	m.write_global("D_800C9754", 0x29)
	m.write_global("D_800C9758", 0x28)
	m.write_global("D_800C5FCC", 0x27)
	m.write_global("D_800C5FD0", 0x59)
	m.write_u32(m.global_address("actor_default_texture_blocks"), 0x60)
	for i: int in 64:
		m.write_s32(_tb(e, i), 0x1234)
	e.call_c("tpage_buf_clear")
	var expect: Dictionary = {}
	for i: int in 64:
		expect[i] = 0 if (i & 0xF) < 5 else -1
	for i: int in 5:
		for base: int in [11, 43, 21, 53]:
			expect[base + i] = 0x77
	for pair: Array in [[60, 0x60], [61, 0x61], [62, 0x62], [63, 0x63], [31, 0x31], [30, 0x30], [29, 0x29], [28, 0x28], [27, 0x27], [59, 0x59]]:
		expect[pair[0]] = pair[1]
	ok = true
	for i: int in 64:
		if m.s32(_tb(e, i)) != expect[i]:
			ok = false
			s.failures.append("tpage_buf[%d] = %d, expected %d" % [i, m.s32(_tb(e, i)), expect[i]])
	s.check(ok, "tpage_buf_clear: reset pattern plus the system defaults")
	# load_tpage_buf_lock: K&R u8 third argument, VRAM half by slot
	var log: Array = []
	s.record(e, ["load_csr_tp"], log)
	e.call_c("load_tpage_buf_lock", [3, 0x777, 0x1FF])
	e.call_c("load_tpage_buf_lock", [0x25, 0x778, 1])
	s.check(m.s32(_tb(e, 3)) == 0x777 and m.s32(_tb(e, 0x25)) == 0x778, "stores the resource id")
	s.check(log == [["load_csr_tp", 0x777, 3, 0xFF, 0, 0, 0x80, 0x80], ["load_csr_tp", 0x778, 5, 1, 0, 0x80, 0x80, 0x80]], "load_csr_tp(id, slot & 0x1F, (u8) arg2, 0, 0 or 0x80, 0x80, 0x80): %s" % [log])
	# print
	log.clear()
	s.record(e, ["printf"], log)
	e.call_c("tpage_buffer_print")
	s.check(log.is_empty(), "no PAD_R2: nothing printed")
	e.gs_set("pad_held", 2)
	e.call_c("tpage_buffer_print")
	s.check(log.size() == 1 + 64 + 16 and log[0] == ["printf", 0x800AF978] and log[1] == ["printf", 0x800AF98C, 0, 0] and log[5] == ["printf", 0x800AF998] and log[6] == ["printf", 0x800AF98C, 4, 0], "header, 64 entries, a CR after every fourth: %d calls" % log.size())

func _test_searches() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["load_csr_tp"], log)
	var m = e.memory
	for i: int in 64:
		m.write_s32(_tb(e, i), 7)
	m.write_s32(_tb(e, 4), -1)
	e.call_c("actor_clear", [2])
	var table: int = 0x80190000
	for i: int in 3:
		m.write_u32(table + i * 4, [0xAAA, 0xBBB, 0xCCC][i])
	m.write_u32(e.memory.global_address("actor_pool") + 2 * 0x44 + 0x34, table)
	s.check(e.call_c("search_tpage", [0x7FFF, 0, 0]) == 0, "search_tpage(0x7FFF) returns 0")
	s.check(m.s32(_ss(e, 4)) == 0x7FFF and m.s32(_ss(e, 3)) == 0x7FFE and m.s32(_ss(e, 63)) == 0x7FFE, "it marks free (tpage_buf == -1) slots 0x7FFF and the others 0x7FFE")
	# search by actor 2, arg0 = 0x80 -> index 0x80 >> 6 = 2 -> key 0xCCC; the first free slot is 4
	var r: int = e.call_c("search_tpage", [0x80, 2, 9])
	s.check(r == 4 and m.s32(_ss(e, 4)) == 0xCCC and log == [["load_csr_tp", 0xCCC, 4, 9, 0, 0, 0x80, 0x80]], "new key: first free slot 4 gets it and a load: %s" % [log])
	log.clear()
	s.check(e.call_c("search_tpage", [0x80, 2, 9]) == 4 and log.is_empty(), "same key again: slot 4, no load")
	# phase 1: slot 39 (i = 7, j = 2) marked 0x7FFE whose buffer holds the key wins before slot 4
	m.write_s32(_ss(e, 39), 0x7FFE)
	m.write_s32(_tb(e, 39), 0xCCC)
	s.check(e.call_c("search_tpage", [0x80, 2, 9]) == 39, "a 0x7FFE slot whose buffer holds the key is found first")
	# search_tpage8: index arg0 >> 8
	s.check(e.call_c("search_tpage8", [0x200, 2, 9]) == 39, "search_tpage8 uses arg0 >> 8")
	# upper half: only slot 0x25 free
	for i: int in 64:
		m.write_s32(_ss(e, i), 0x1234)
	m.write_s32(_ss(e, 0x25), 0x7FFF)
	log.clear()
	s.check(e.call_c("search_tpage", [0x80, 2, 3]) == 0x25 and log == [["load_csr_tp", 0xCCC, 0x25, 3, 0, 0x80, 0x80, 0x80]], "slot >= 0x20: load_csr_tp with y 0x80: %s" % [log])
	# none free: slot 0x3F is returned
	for i: int in 64:
		m.write_s32(_ss(e, i), 0x1234)
	s.check(e.call_c("search_tpage", [0x40, 2, 3]) == 0x3F, "no match, no free slot: 0x3F")
	# search_tpage_multi (key is arg0 itself)
	for i: int in 64:
		m.write_s32(_ss(e, i), 0x1234)
		m.write_s32(_tb(e, i), 9)
	m.write_s32(_ss(e, 0xA), 0x7FFF)
	log.clear()
	s.check(e.call_c("search_tpage_multi", [0x4242, 6]) == 0xA and log == [["load_csr_tp", 0x4242, 0xA, 6, 0, 0, 0x80, 0x80]], "search_tpage_multi: free slot 0xA")
	m.write_s32(_ss(e, 0x12), 0x5151)
	s.check(e.call_c("search_tpage_multi", [0x5151, 6]) == 0x12, "phase 2: slot already holding the key")
	# search_load_tpage_buf_lock
	for i: int in 64:
		m.write_s32(_tb(e, i), 1)
	m.write_s32(_tb(e, 5), -1)
	m.write_s32(_tb(e, 0x25), -1)
	log.clear()
	s.check(e.call_c("search_load_tpage_buf_lock", [0x99, 0x101]) == 5 and m.s32(_tb(e, 5)) == 0x99 and log == [["load_csr_tp", 0x99, 5, 1, 0, 0, 0x80, 0x80]], "search_load_tpage_buf_lock: first -1 slot from 5, (u8) tag: %s" % [log])
	log.clear()
	s.check(e.call_c("search_load_tpage_buf_lock", [0x98, 2]) == 0x25 and log == [["load_csr_tp", 0x98, 5, 2, 0, 0x80, 0x80, 0x80]], "slot 0x25: upper half")
	s.check(e.call_c("search_load_tpage_buf_lock", [0x97, 2]) == -1, "none free: -1")

func _test_actor_clear() -> void:
	var e = s.new_engine()
	var m = e.memory
	var pool: int = m.global_address("actor_pool")
	var p: int = pool + 3 * 0x44
	for i: int in 0x44:
		m.write_u8(p + i, 0xEE)
		m.write_u8(p + 0x44 + i, 0xEE)
	e.call_c("actor_clear", [3])
	var expect: Dictionary = {0x06: 1, 0x07: 0x80, 0x1A: 0, 0x1B: 0x10, 0x1C: 0, 0x1D: 0x10}
	var bad: Array = []
	for i: int in 0x44:
		var want: int = 0
		if i in [0x0A, 0x0B, 0x1E, 0x1F, 0x3C, 0x3D, 0x3E, 0x3F]:
			want = 0xEE # the C writes none of these fields
		elif expect.has(i):
			want = expect[i]
		elif i >= 0x0C and i < 0x10:
			want = [0x5C, 0x97, 0x0C, 0x80][i - 0x0C]
		elif i >= 0x10 and i < 0x14:
			want = [0x14, 0x9A, 0x0C, 0x80][i - 0x10]
		elif i >= 0x34 and i < 0x38:
			want = [0x30, 0x97, 0x0C, 0x80][i - 0x34]
		if m.u8(p + i) != want:
			bad.append("+0x%02X = %02X (want %02X)" % [i, m.u8(p + i), want])
	s.check(bad.is_empty(), "actor_clear defaults (tables 0x800C975C / 0x800C9A14 / 0x800C9730, scales 0x1000, brightness 0x80, period 1): %s" % [bad])
	s.check(m.u8(p + 0x44) == 0xEE, "the next record is untouched")
	m.write_u8(pool + 0xA0 * 0x44, 0xEE)
	e.call_c("actor_clear", [0xA0])
	s.check(m.u8(pool + 0xA0 * 0x44) == 0xEE, "index 0xA0 and above: nothing")
	# clear range / upper through a recording double that also disturbs the cursor globals
	var log: Array = []
	s.record(e, ["actor_clear"], log, {}, {"actor_clear": func(_a: Array) -> void:
		m.write_global("cursor_x", 99)})
	m.write_global("cursor_x", 11)
	m.write_global("cursor_y", 22)
	m.write_global("D_8011ECE8", 33)
	e.call_c("actor_clear_range", [5])
	s.check(log.map(func(x): return x[1]) == [1, 2, 3, 4], "actor_clear_range(5): records 1..4")
	s.check([m.read_global("cursor_x"), m.read_global("cursor_y"), m.read_global("D_8011ECE8")] == [11, 22, 33], "the cursor globals are restored")
	log.clear()
	e.call_c("actor_clear_range", [0])
	s.check(log.size() == 0x9F and log[-1][1] == 0x9F, "arg0 = 0: records 1..0x9F")
	log.clear()
	e.call_c("actor_clear_range", [0xA1])
	s.check(log.size() == 0x9F, "arg0 >= 0xA1 clamps to 0xA0")
	log.clear()
	e.call_c("actor_clear_upper")
	s.check(log.size() == 0x40 and log[0][1] == 0x60 and log[-1][1] == 0x9F, "actor_clear_upper: 0x60..0x9F")
	# deactivate
	var e2 = s.new_engine()
	var log2: Array = []
	s.record(e2, ["printf"], log2)
	var pool2: int = e2.memory.global_address("actor_pool")
	for i: int in 170:
		e2.memory.write_u8(pool2 + i * 0x44 + 3, 0xFF)
	e2.call_c("actor_deactivate", [3])
	s.check(log2 == [["printf", 0x800AF99C, 3]] and e2.memory.u8(pool2 + 2 * 0x44 + 3) == 0 and e2.memory.u8(pool2 + 3 * 0x44 + 3) == 0xFF, "count 3: printf and records 0..2")
	e2.call_c("actor_deactivate", [500])
	s.check(e2.memory.u8(pool2 + 159 * 0x44 + 3) == 0 and e2.memory.u8(pool2 + 160 * 0x44 + 3) == 0xFF, "count 500 stops at 160")
	e2.memory.write_u8(pool2 + 5 * 0x44 + 3, 0xFF)
	e2.call_c("actor_deactivate", [0])
	s.check(e2.memory.u8(pool2 + 5 * 0x44 + 3) == 0, "count 0: all 160")
	e2.call_c("actor_draw_enable", [7])
	s.check(e2.gs_get("actor_draw_enabled") == 1, "draw_enable(nonzero) = 1")
	e2.call_c("actor_draw_enable", [0])
	s.check(e2.gs_get("actor_draw_enabled") == 0, "draw_enable(0) = 0")

func _setup_actor(e) -> Dictionary:
	var m = e.memory
	var pool: int = m.global_address("actor_pool")
	e.call_c("actor_clear", [0])
	var addrs: Dictionary = {"frame_table": 0x801B0000, "tpage_table": 0x801B1000, "seq_table": 0x801B2000, "row": 0x801B3000, "anim": 0x801C0000}
	m.write_u32(addrs.frame_table, addrs.anim) # frame table entry 0
	m.write_u32(addrs.tpage_table, 0x80000123) # bit 31 set; entry 0
	m.write_u32(addrs.seq_table, addrs.row)
	m.write_u8(pool + 3, 0x80)
	m.write_u32(pool + 0x0C, addrs.frame_table)
	m.write_u32(pool + 0x10, addrs.seq_table)
	m.write_u32(pool + 0x34, addrs.tpage_table)
	m.write_s16(pool + 0x26, 100) # x.h[1]
	m.write_s16(pool + 0x2A, 50) # y.h[1]
	m.write_u8(pool + 5, 1)
	m.write_u8(pool + 7, 0x55)
	m.write_s16(addrs.row + 2, 0) # frame index of step 0
	var anim: int = addrs.anim
	m.write_s16(anim + 0, 1) # count
	m.write_s16(anim + 2, 2) # origin x
	m.write_s16(anim + 4, 3) # origin y
	var part: int = anim + 8
	for pair: Array in [[0, 4], [2, 5], [4, 0x42], [6, 0x123], [8, 3], [0xA, 16], [0xC, 8]]:
		m.write_s16(part + pair[0], pair[1])
	addrs["pool"] = pool
	return addrs

func _test_actor_draw_all() -> void:
	var e = s.new_engine()
	var m = e.memory
	var log: Array = []
	var snaps: Array = []
	var a: Dictionary = _setup_actor(e)
	e.memory.write_global("draw_buffer", 1)
	e.set_stub("search_tpage", func(args: Array) -> int:
		log.append(["search_tpage"] + args.map(func(x): return x & M32))
		return 0x25)
	e.set_stub("func_80098680", func(args: Array) -> int:
		log.append(["func_80098680"] + args.map(func(x): return x & M32))
		snaps.append(s.bytes(e, args[0], 0x24))
		return 0)
	e.call_c("actor_draw_all")
	# the first search_tpage(0x7FFF, i, 0) is replaced by the stub too (it is the C's first call)
	s.check(log.size() == 3 and log[0][1] == 0x7FFF and log[1] == ["search_tpage", 0x42, 0, 2], "search_tpage(0x7FFF, ...) then search_tpage(part tpage, record, 2): %s" % [log])
	var sp: int = e.sp - 0xF0 + 0xC8
	s.check(log[2] == ["func_80098680", sp, 0x800E8C70 + 0x14, 0], "func_80098680(&spr, &draw_ot_table + buffer * 0x14, 0): %s" % [log[2]])
	var q: Array = snaps[0]
	# x = 100 + 2, anchor_x = (16 - 2) + 4 = 18, scale -0x1000 (attr bit 2); y = 50 + 3, anchor_y = (8 - 3) + 5 = 10, scale -0x1000 (attr bit 1)
	s.check(q.slice(0, 12) == [0, 0, 0, 0, 102, 0, 53, 0, 16, 0, 8, 0], "flags, x = bx + ox, y = by + oy, w = 16, h = 8: %s" % [q.slice(0, 12)])
	s.check(q.slice(0x18, 0x20) == [18, 0, 10, 0, 0, 0xF0, 0, 0xF0], "anchors and the mirrored scales: %s" % [q.slice(0x18, 0x20)])
	# tpage 0x25: tpx = 5, tpy = (0x25 & 0x20) * 4 = 0x80; uv = 0x42 & 0x3F = 2 -> u = (2 & 7) * 16 = 0x20, v = (2 >> 3) * 16 + 0x80
	s.check(q.slice(0xC, 0x10) == [5, 0, 0x20, 0x80], "tpage x, u, v + tpy: %s" % [q.slice(0xC, 0x10)])
	# unk_05 = 1: clut x = ((0x123 & 0xF0) >> 4) * 0x10 = 0x20; clut y = ((0x123 & 0xF00) >> 8) + 0x1E0
	s.check(q.slice(0x10, 0x14) == [0x20, 0, 0xE1, 1], "clut x, y: %s" % [q.slice(0x10, 0x14)])
	s.check(q.slice(0x14, 0x17) == [0x55, 0x55, 0x55], "tint = unk_07 when unk_40..42 are zero")
	# tint from draw_flags 0x20 (brightness) and from unk_40..42
	e.gs_set("screen_brightness", 0x66)
	m.write_u8(a.pool + 3, 0xA0)
	log.clear()
	snaps.clear()
	e.call_c("actor_draw_all")
	s.check(snaps[0].slice(0x14, 0x17) == [0x66, 0x66, 0x66], "draw_flags & 0x20: the screen brightness")
	m.write_u8(a.pool + 3, 0x80)
	m.write_u8(a.pool + 0x41, 0x31)
	snaps.clear()
	e.call_c("actor_draw_all")
	s.check(snaps[0].slice(0x14, 0x17) == [0, 0x31, 0], "any of unk_40..42 nonzero: the three bytes")
	# scale: unk_1A = 0x2000 -> w + 1, h + 1 branch needs both unk_1A and unk_1C != 0x1000
	m.write_u8(a.pool + 0x41, 0)
	m.write_s16(a.pool + 0x1A, 0x2000)
	m.write_s16(a.pool + 0x1C, 0x3000)
	snaps.clear()
	e.call_c("actor_draw_all")
	s.check(snaps[0].slice(8, 12) == [17, 0, 9, 0], "both scales != 0x1000: w = unk_0A + 1, h = unk_0C + 1")
	# scale multiply: unk_1C = 0x2000 * (-0x1000 / 0x1000) = -0x2000, unk_1E = 0x3000 * -1 = -0x3000
	s.check(snaps[0].slice(0x1C, 0x20) == [0x00, 0xE0, 0x00, 0xD0], "scales: unk_1A * (spr.unk_1C / 0x1000) and unk_1C * (spr.unk_1E / 0x1000): %s" % [snaps[0].slice(0x1C, 0x20)])
	# the unset tpage local refuses
	var e2 = s.new_engine()
	_setup_actor(e2)
	e2.memory.write_s32(e2.memory.global_address("actor_pool") + 0x38, 0x02000000)
	e2.set_stub("search_tpage", func(_args: Array) -> int:
		return 0)
	e2.call_c("actor_draw_all")
	s.check(e2.halted() and e2.refusal.contains("tpage"), "quad flags 0x02000000 with no earlier tpage: refused (%s)" % e2.refusal)
	# a table pointer without bit 31 prints and skips the record
	var e3 = s.new_engine()
	var a3: Dictionary = _setup_actor(e3)
	e3.memory.write_u32(a3.tpage_table, 0x00000123)
	var plog: Array = []
	s.record(e3, ["printf", "search_tpage", "func_80098680"], plog)
	e3.gs_set("game_mode", 0x12)
	e3.gs_set("mode_stage", 3)
	e3.gs_set("mode_step", 4)
	e3.gs_set("substep", 5)
	e3.call_c("actor_draw_all")
	s.check(plog.filter(func(x): return x[0] == "printf") == [["printf", 0x800AF960, 0, 0x12, 3, 4, 5]] and plog.filter(func(x): return x[0] == "func_80098680").is_empty(), "bit 31 clear: printf(the cls line with i, mode, stage, step, substep), nothing drawn: %s" % [plog])
	# animation advance: flags 1, period unk_06 = 1 (frame_count % 1 == 0 -> unk_08++); with no parts (count 0)
	var e4 = s.new_engine()
	var a4: Dictionary = _setup_actor(e4)
	var m4 = e4.memory
	m4.write_s16(a4.anim, 0)
	m4.write_u8(a4.pool + 2, 1)
	m4.write_u8(a4.pool + 6, 1)
	m4.write_s16(a4.row, 3) # frames in the sequence
	m4.write_s16(a4.row + 4, 1) # step 0 lasts 1
	e4.set_stub("search_tpage", func(_args: Array) -> int:
		return 0)
	e4.call_c("actor_draw_all")
	s.check(m4.s16(a4.pool + 0x18) == 1 and m4.s16(a4.pool + 8) == 0 and m4.u8(a4.pool + 2) == 1, "unk_08 reaches the step length: frame + 1, timer 0")
	m4.write_s16(a4.row, 2)
	e4.call_c("actor_draw_all") # frame 1 -> step 1: row[1 * 2 + 2] = unused 0, timer 1 >= 0: frame 2 >= seq[0] = 2
	s.check(m4.s16(a4.pool + 0x18) == 0, "frame reaches the sequence end with no loop flag: back to 0 (frame %d)" % m4.s16(a4.pool + 0x18))
	# movement: draw_flags & 0x10 adds dx, dy (32-bit)
	m4.write_u8(a4.pool + 3, 0x90)
	m4.write_s32(a4.pool + 0x2C, 0x10000)
	m4.write_s32(a4.pool + 0x24, 0xFFFF0000)
	e4.call_c("actor_draw_all")
	s.check(m4.u32(a4.pool + 0x24) == 0 and m4.s32(a4.pool + 0x28) == 50 << 16, "x.w += dx wraps at 32 bits; dy == 0 leaves y alone")
