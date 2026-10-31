extends RefCounted
## T-39311: engine/frame_render_audio.gd on invented memory. Expectations are derived by hand from
## src/main/frame_render_audio.c. libgpu/libgte/libetc calls, the sound driver, the CD reads and the other files'
## functions are recording doubles.

const Support = preload("res://tests/engine/_draw_support.gd")
const Math = preload("res://core/native_math.gd")

var s: Support = Support.new()

const M32: int = 0xFFFFFFFF
const FRAME_CALLS: Array = ["func_80098650", "func_80098680", "func_80098B80", "func_80098D00", "func_80099540", "func_80099B10",
	"func_80099F04", "func_8009A200", "func_80099ED8", "func_8009A4D0", "func_8009C674", "func_800AD950", "func_8009C210",
	"func_8009ABF0", "func_8009AC20", "func_8009824C", "func_8009AD00", "disp_string", "pad_update", "view_models_update_draw",
	"csr_load_vram", "palette_load_vram", "actor_draw_all", "srn_set"]

func run() -> Array[String]:
	s.failures.clear()
	_test_small_setters()
	_test_queues()
	_test_flush()
	_test_squares_and_bars()
	_test_audio_state()
	_test_bank_load()
	_test_bank_poll_and_start()
	_test_draw_frame()
	s.shutdown_all()
	return s.failures

func _test_small_setters() -> void:
	var e = s.new_engine()
	e.call_c("draw2d3d", [0x101, 2])
	s.check(e.gs_get("draw_2d_enabled") == 1 and e.gs_get("draw_3d_enabled") == 2, "draw2d3d stores the u8 arguments")
	e.call_c("back_clear_switch", [5])
	s.check(e.gs_get("back_clear") == 1, "back_clear_switch(nonzero) = 1")
	e.call_c("back_clear_switch", [0])
	s.check(e.gs_get("back_clear") == 0, "back_clear_switch(0) = 0")
	e.call_c("func_80044434")

func _test_queues() -> void:
	var e = s.new_engine()
	var m = e.memory
	var q: int = e.gs_address("palette_queue")
	s.check(e.call_c("load_palette", [0x80190000, 0x1FF, 2, 3, 4]) == 1 and e.gs_get("palette_queue_count") == 1, "load_palette returns 1 and counts")
	s.check(m.u32(q) == 0x80190000 and s.bytes(e, q + 4, 4) == [0xFF, 2, 3, 4], "entry: source, then the u8 arguments (K&R narrowing of 0x1FF)")
	for i: int in 31:
		e.call_c("load_palette", [0x80190000 + i, 0, 0, 0, 0])
	s.check(e.gs_get("palette_queue_count") == 32 and e.call_c("load_palette", [1, 1, 1, 1, 1]) == 0 and e.gs_get("palette_queue_count") == 32, "a full queue (32 entries) returns 0 and changes nothing")
	var c: int = e.gs_address("csr_queue")
	s.check(e.call_c("load_csr_ab", [0x80191000, 0x12, 0x1234, 0x2345, 0x3456, 0x4567]) == 1, "load_csr_ab returns 1")
	s.check(m.u32(c) == 0x80191000 and s.bytes(e, c + 4, 4) == [0x12, 0, 0, 0] and [m.u16(c + 8), m.u16(c + 0xA), m.u16(c + 0xC), m.u16(c + 0xE)] == [0x1234, 0x2345, 0x3456, 0x4567], "load_csr_ab entry: unk_05..07 zero")
	e.call_c("load_csr_tp", [0x80192000, 0x25, 3, 1, 2, 3, 4])
	var t: int = c + 16
	s.check(m.u32(t) == 0x80192000 and s.bytes(e, t + 4, 4) == [0x25, 3, 0, 0] and [m.u16(t + 8), m.u16(t + 0xA), m.u16(t + 0xC), m.u16(t + 0xE)] == [1, 2, 3, 4] and e.gs_get("csr_queue_count") == 2, "load_csr_tp entry: unk_05 = arg2 (the width shift)")
	e.gs_set("csr_queue_count", 32)
	s.check(e.call_c("load_csr_tp", [1, 2, 3, 4, 5, 6, 7]) == 0 and e.call_c("load_csr_ab", [1, 2, 3, 4, 5, 6]) == 0, "full csr queue: both return 0")

func _test_flush() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["LoadSquare"], log)
	var m = e.memory
	e.call_c("load_csr_ab", [0x80191000, 0, 10, 20, 30, 40])
	e.call_c("load_csr_tp", [0x80192000, 0x25, 1, 5, 6, 64, 8])
	e.call_c("csr_load_vram", [0])
	# slot 0: as queued; slot 0x25: x = 5 + ((0x25 & 0xF) << 6) = 5 + 0x140, y = 6 + ((0x25 & 0x10) * 0x10) = 6, width 64 >> 1
	s.check(log == [["LoadSquare", 10, 20, 30, 40, 0x80191000], ["LoadSquare", 5 + 0x140, 6, 32, 8, 0x80192000]], "csr_load_vram: %s" % [log])
	s.check(e.gs_get("csr_queue_count") == 0, "the queue is emptied")
	log.clear()
	e.call_c("csr_load_vram", [0])
	s.check(log.is_empty(), "empty queue: nothing")
	# palette_load_vram: 512 source words, entries (source, unk_04, unk_05, unk_06 = STP mode, unk_07 = bank of 128 words)
	var src: int = 0x801A0000
	for j: int in 512:
		m.write_u32(src + j * 4, 0)
	m.write_u32(src + 0 * 4, 0x80008001)
	m.write_u32(src + 5 * 4, 0x00018000)
	m.write_u32(src + 127 * 4, 0x80000000)
	m.write_u32(src + 128 * 4, 0x80008000)
	m.write_u32(src + 247 * 4, 0x80008000)
	m.write_u32(src + 248 * 4, 0x00010002)
	m.write_u32(src + 256 * 4, 0x00000001)
	m.write_u32(src + 384 * 4, 0x80008000)
	m.write_u32(src + 496 * 4, 0x00020000)
	m.write_u32(src + 511 * 4, 0x00000003)
	for entry: Array in [[5, 0, 0, 0], [6, 0, 2, 1], [7, 0, 3, 2], [8, 0, 4, 3], [9, 0, 1, 0], [40, 0, 0, 1], [0x45, 0, 0, 1]]:
		e.call_c("load_palette", [src, entry[0], entry[1], entry[2], entry[3]])
	log.clear()
	e.call_c("palette_load_vram", [0])
	s.check([m.u32(src), m.u32(src + 5 * 4), m.u32(src + 127 * 4)] == [0x00000001, 0x00010000, 0], "mode 0: STP bits cleared in words 0..127 (0x80008001 -> 1, 0x00018000 -> 0x10000, 0x80000000 -> 0)")
	s.check([m.u32(src + 128 * 4), m.u32(src + 247 * 4), m.u32(src + 248 * 4)] == [0, 0, 0x80018002], "mode 2: 120 words cleared, the last 8 set (0x00010002 -> 0x80018002)")
	s.check([m.u32(src + 256 * 4), m.u32(src + 257 * 4)] == [0x00008001, 0], "mode 3: nonzero halves get their STP bit")
	s.check([m.u32(src + 384 * 4), m.u32(src + 496 * 4), m.u32(src + 511 * 4)] == [0, 0x80020000, 0x00008003], "mode 4: 112 cleared, 16 set")
	var rows: Array = log.filter(func(x): return x[0] == "LoadSquare")
	s.check(rows == [["LoadSquare", 0, 5 + 0x1E0, 0x100, 1, src], ["LoadSquare", 0, 6 + 0x1E0, 0x100, 1, src + 0x200], ["LoadSquare", 0, 7 + 0x1E0, 0x100, 1, src + 0x400],
		["LoadSquare", 0, 8 + 0x1E0, 0x100, 1, src + 0x600], ["LoadSquare", 0, 9 + 0x1E0, 0x100, 1, src], ["LoadSquare", 0x100, 8 + 0x1E0, 0x20, 1, src + 0x200], ["LoadSquare", 0x120, 5 + 0x1E0, 0x20, 1, src + 0x200]],
		"LoadSquare(x, y, width, 1, source + first words): %s" % [rows])
	s.check(e.gs_get("palette_queue_count") == 0, "queue emptied")

func _test_squares_and_bars() -> void:
	var log: Array = []
	var e = s.new_engine()
	var rects: Array = []
	s.record(e, ["func_8009C884", "func_8009C8E0", "func_8009C93C"], log, {}, {
		"func_8009C884": func(a: Array) -> void: rects.append(s.bytes(e, a[0], 8)),
		"func_8009C8E0": func(a: Array) -> void: rects.append(s.bytes(e, a[0], 8)),
		"func_8009C93C": func(a: Array) -> void: rects.append(s.bytes(e, a[0], 8))})
	var frame: int = e.sp - 0x30
	e.call_c("LoadSquare", [0x11, 0x22, 0x33, 0x44, 0x80190000])
	s.check(log[0] == ["func_8009C884", frame + 0x28, 0x80190000] and rects[0] == [0x11, 0, 0x22, 0, 0x33, 0, 0x44, 0], "LoadSquare: RECT at frame + 0x28 then func_8009C884(&rect, data): %s" % [log[0]])
	e.call_c("StoreSquare", [1, 2, 3, 4, 0x80191000])
	s.check(log[1] == ["func_8009C8E0", frame + 0x28, 0x80191000] and rects[1] == [1, 0, 2, 0, 3, 0, 4, 0], "StoreSquare: func_8009C8E0")
	e.call_c("MoveSquare", [5, 6, 7, 8, 0x1FFFF, 10])
	s.check(log[2] == ["func_8009C93C", frame + 0x28, 0xFFFF, 10] and rects[2] == [5, 0, 6, 0, 7, 0, 8, 0], "MoveSquare: func_8009C93C(&rect, (u16) x, (u16) y)")
	e.call_c("vram_palette_row_copy", [3, 5, 9])
	var f2: int = e.sp - 0x38
	s.check(log[3] == ["func_8009C93C", f2 + 0x30, 0x100, 9 + 0x1E0] and rects[3] == [0x50, 0, (3 + 0x1E0) & 0xFF, (3 + 0x1E0) >> 8, 0x10, 0, 1, 0], "vram_palette_row_copy: rect (x << 4, row + 0x1E0, 16, 1) at +0x30: %s" % [rects[3]])
	# draw_time_bars: stamps 0, 264, 528, 792 -> colour steps blue, green, yellow, red; heights stamp * 16 / 264
	var log2: Array = []
	var e2 = s.new_engine()
	s.record(e2, ["prim_fill_rect"], log2)
	for i: int in 4:
		e2.memory.write_global("draw_horizontal_blank_stamps", 264 * i, i)
	e2.call_c("draw_time_bars")
	var expect: Array = []
	var colors: Array = [0x80, 0x8000, 0x808000, 0x800000]
	for i: int in 4:
		var h: int = 16 * i
		expect.append(["prim_fill_rect", i + 6, (0x8C + i * 4) | ((0x4C - h) << 16), 4 | (h << 16), colors[i]])
	expect.append(["prim_fill_rect", 10, 0x88 | (0x3C << 16), 4 | (16 << 16), 0x808080])
	s.check(log2 == expect, "draw_time_bars: four bars and the frame: %s" % [log2])

func _test_audio_state() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["audio_command_submit"], log)
	s.check(e.call_c("audio_command", [0x12345]) == 1 and log == [["audio_command_submit", 0x2345]], "audio_command submits the low 16 bits and returns 1")
	var m = e.memory
	m.write_global("audio_bgm_playing", 7)
	m.write_global("audio_bgm_set", 9)
	s.check(e.call_c("audio_bgm_is_playing") == 7 and e.call_c("audio_bgm_current_set") == 9, "getters")
	# bank headers: not 0x40 -> -1 everywhere
	log.clear()
	s.check(e.call_c("audio_bgm_stop", [1]) == -1 and e.call_c("audio_bank_start", [0, 3]) == -1 and e.call_c("audio_bgm_play_short", [1, 3]) == -1 and log.is_empty(), "bank header word & 0xFFFF != 0x40: -1, nothing submitted")
	m.write_u32(0x8001C000, 0x12340040)
	m.write_u32(0x8002E800, 0x00000040)
	var r: int = e.call_c("audio_bgm_stop", [0])
	s.check(r == 0 and log == [["audio_command_submit", 0x1200]] and m.read_global("audio_bgm_playing") == 0, "audio_bgm_stop(0): command 0x1200, playing = 0, returns arg0 & 0xFF")
	log.clear()
	m.write_global("audio_bgm_set", 1)
	m.write_global("audio_bgm_sequence", 4)
	r = e.call_c("audio_bgm_stop", [0x101])
	s.check(r == 1 and log.map(func(x): return x[1]) == [0x23, 0x24, 0x2E, 4 | 0x200] and m.read_global("audio_bgm_playing") == 0x101 & 0xFF, "audio_bgm_stop(nonzero): audio_bank_start(set, sequence): %s" % [log])
	log.clear()
	s.check(e.call_c("audio_bank_start", [0, 0x1AB]) == 1 and log.map(func(x): return x[1]) == [0x22, 0x21, 0x2F, 0x3AB] and m.read_global("audio_bgm_set") == 0 and m.read_global("audio_bgm_sequence") == 0xAB and m.read_global("audio_bgm_playing") == 1, "audio_bank_start(0, seq): commands 0x22, 0x21, 0x2F, seq | 0x200; the u8 stores")
	log.clear()
	e.call_c("audio_bgm_play_short", [0, 5])
	s.check(log.map(func(x): return x[1]) == [0x22, 0x2F, 0x205], "audio_bgm_play_short(0): 0x22, 0x2F, seq | 0x200")
	log.clear()
	e.call_c("audio_bgm_play_short", [2, 5])
	s.check(log.map(func(x): return x[1]) == [0x23, 0x2E, 0x205], "audio_bgm_play_short(bank 2): 0x23, 0x2E, seq | 0x200")
	# reset
	m.write_global("D_800B3D54", 9, 0)
	m.write_global("D_800B3D54", 9, 1)
	m.write_global("D_800B3D30", 1)
	m.write_global("D_800B3D3C", 4)
	m.write_u8(0x800B3D28, 5)
	m.write_u8(0x800B3D4C, 6)
	e.call_c("audio_bank_state_reset")
	s.check(m.read_global("audio_bgm_set") == 0xFF and m.read_global("D_800B3D30") == 0 and m.read_global("D_800B3D3C") == 0 and m.u8(0x800B3D28) == 0 and m.u8(0x800B3D4C) == 0 and m.read_global("D_800B3D54", 1) == 0 and m.read_global("audio_bgm_sequence") == 0, "audio_bank_state_reset clears the bank state, set = 0xFF")

func _test_bank_load() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, ["cd_done_count", "cd_load_destination_base_set", "cd_read_sectors_nowait"], log, {"cd_done_count": 0x42})
	var m = e.memory
	e.call_c("audio_bank_load_start", [0, 100, 200, 300, 400, 500])
	s.check(log == [["cd_done_count"], ["cd_load_destination_base_set", 0x801B8000], ["cd_read_sectors_nowait", 3, 0x80010000, 100], ["cd_read_sectors_nowait", 0x20, 0x801B8000, 200],
		["cd_read_sectors_nowait", 0x25, 0x8001C000, 500], ["cd_read_sectors_nowait", 0x50, 0x801C8000, 400], ["cd_read_sectors_nowait", 6, 0x80012000, 300]], "set 0, all five files: %s" % [log])
	s.check([m.read_global("D_800B3D30"), m.read_global("D_800B3D34"), m.read_global("D_800B3D20"), m.read_global("D_800B3D24"), m.u8(0x800B3D50), m.read_global("audio_bgm_set")] == [100, 300, 0x42, 0, 1, 0xFF], "remembered lbas, wait count, flags")
	# again with the same lbas: bank part and second part skipped: D20 = 0x42 - 2 - 3, both done flags set
	log.clear()
	e.call_c("audio_bank_load_start", [0, 100, 200, 300, 400, 500])
	s.check(log.map(func(x): return x[0]) == ["cd_done_count", "cd_load_destination_base_set"] and m.read_global("D_800B3D20") == 0x42 - 5 and m.u8(0x800B3D28) == 1 and m.u8(0x800B3D2C) == 1, "same files already loaded: nothing read, D_800B3D20 -= 2 and -= 3")
	# zero lbas: each skipped read subtracts 1 (b, c, f, e, d: five times), and the u8 wraps
	log.clear()
	var e2 = s.new_engine()
	s.record(e2, ["cd_done_count", "cd_load_destination_base_set", "cd_read_sectors_nowait"], log, {"cd_done_count": 3})
	e2.memory.write_global("D_800B3D30", 77)
	e2.memory.write_global("D_800B3D34", 77)
	e2.call_c("audio_bank_load_start", [0, 0, 0, 0, 0, 0])
	s.check(log.map(func(x): return x[0]) == ["cd_done_count", "cd_load_destination_base_set"] and e2.memory.read_global("D_800B3D20") == (3 - 5) & 0xFF, "all lbas zero: five skips, D_800B3D20 wraps as a u8 (%d)" % e2.memory.read_global("D_800B3D20"))
	# set 1
	log.clear()
	var e3 = s.new_engine()
	s.record(e3, ["cd_done_count", "cd_load_destination_base_set", "cd_read_sectors_nowait"], log, {"cd_done_count": 9})
	e3.call_c("audio_bank_load_start", [1, 10, 20, 30, 40, 50])
	s.check(log.slice(2) == [["cd_read_sectors_nowait", 3, 0x8001A000, 10], ["cd_read_sectors_nowait", 0x35, 0x801E0000, 20], ["cd_read_sectors_nowait", 0x25, 0x8002E800, 50], ["cd_read_sectors_nowait", 0x50, 0x801B8000, 40], ["cd_read_sectors_nowait", 6, 0x80016000, 30]], "set 1 uses the other addresses: %s" % [log.slice(2)])
	s.check([e3.memory.read_global("D_800B3D38"), e3.memory.read_global("D_800B3D3C")] == [10, 30], "set 1 remembers in D_800B3D38 and D_800B3D3C")

func _test_bank_poll_and_start() -> void:
	var log: Array = []
	var e = s.new_engine()
	var done: Array = [0x10]
	s.record(e, ["audio_command_submit", "cd_load_destination_base_set", "audio_bgm_bank_ready"], log, {"audio_bgm_bank_ready": 1})
	e.set_stub("cd_done_count", func(_a: Array) -> int:
		return done[0])
	var m = e.memory
	m.write_global("D_800B3D20", 0x10)
	s.check(e.call_c("audio_bank_transfer_poll") == -1 and e.call_c("audio_bank_load_poll") == -1, "fewer than 5 completions: -1, nothing done")
	done[0] = 0x15
	m.write_global("D_800B3D24", 0)
	# first poll: counter 0 -> audio_bank_transfer_start and the counter becomes 1; with neither done flag: 0x11 on 0x801B8000
	s.check(e.call_c("audio_bank_transfer_poll") == -1 and m.u8(0x800B3D4C) == 1 and log == [["cd_load_destination_base_set", 0x801B8000], ["audio_command_submit", 0x11]], "first call starts the transfer: %s" % [log])
	log.clear()
	# second poll: counter 2, audio_bgm_bank_ready(0) and (1) nonzero -> done: D_800B3D54[0] = 0 and 1
	m.write_global("D_800B3D54", 5, 0)
	s.check(e.call_c("audio_bank_transfer_poll") == 1 and m.read_global("D_800B3D54", 0) == 0 and log == [["audio_bgm_bank_ready", 0], ["audio_bgm_bank_ready", 1]], "second call: transfer done -> 1: %s" % [log])
	# the counter wraps/limits: 0xC0 + 1 ends the wait without asking
	log.clear()
	m.write_u8(0x800B3D4C, 0xC0)
	s.check(e.call_c("audio_bank_transfer_poll") == 1 and log.is_empty(), "++counter > 0xC0 returns 1 without audio_bgm_bank_ready")
	m.write_u8(0x800B3D4C, 0xFF)
	e.set_stub("audio_bgm_bank_ready", func(_a: Array) -> int:
		return 0)
	s.check(e.call_c("audio_bank_transfer_poll") == -1 and m.u8(0x800B3D4C) == 0, "the u8 counter wraps (0xFF + 1 = 0): not above 0xC0, not done: -1")
	# transfer_start variants
	log.clear()
	m.write_global("D_800B3D24", 0)
	m.write_u8(0x800B3D28, 1)
	m.write_u8(0x800B3D2C, 0)
	e.call_c("audio_bank_transfer_start")
	s.check(log == [["cd_load_destination_base_set", 0x801C8000], ["audio_command_submit", 0x14]], "set 0, first done: 0x14 from 0x801C8000")
	log.clear()
	m.write_u8(0x800B3D28, 0)
	m.write_u8(0x800B3D2C, 1)
	e.call_c("audio_bank_transfer_start")
	s.check(log == [["cd_load_destination_base_set", 0x801B8000], ["audio_command_submit", 0x13]], "set 0, second done: 0x13")
	m.write_u8(0x800B3D28, 1)
	log.clear()
	s.check(e.call_c("audio_bank_transfer_start") == 0 and log.is_empty(), "set 0, both done: 0")
	m.write_global("D_800B3D24", 1)
	m.write_u8(0x800B3D28, 0)
	m.write_u8(0x800B3D2C, 0)
	e.call_c("audio_bank_transfer_start")
	m.write_u8(0x800B3D2C, 1)
	e.call_c("audio_bank_transfer_start")
	m.write_u8(0x800B3D28, 1)
	m.write_u8(0x800B3D2C, 0)
	e.call_c("audio_bank_transfer_start")
	s.check(log.map(func(x): return x[1]) == [0x801B8000, 0x12, 0x801E0000, 0x16, 0x801B8000, 0x15], "set 1: 0x12 (neither), 0x16 from 0x801E0000, 0x15: %s" % [log])
	# load_poll issues the sequence commands
	log.clear()
	e.set_stub("audio_bgm_bank_ready", func(_a: Array) -> int:
		return 1)
	m.write_global("D_800B3D24", 1)
	m.write_u8(0x800B3D4C, 3)
	s.check(e.call_c("audio_bank_load_poll") == 1 and log.map(func(x): return x[1]) == [0x23, 0x24, 0x2E], "audio_bank_load_poll, set 1: 0x23, 0x24, 0x2E")
	log.clear()
	m.write_global("D_800B3D24", 0)
	m.write_u8(0x800B3D4C, 3)
	s.check(e.call_c("audio_bank_load_poll") == 1 and log.map(func(x): return x[1]) == [0x22, 0x21, 0x2F], "set 0: 0x22, 0x21, 0x2F")
	# transfer_done asks banks (2, 3) for set != 0 and stops after a zero
	log.clear()
	var order: Array = []
	e.set_stub("audio_bgm_bank_ready", func(a: Array) -> int:
		order.append(a[0])
		return 0)
	s.check(e.call_c("audio_bank_transfer_done", [5]) == 0 and order == [2], "set != 0: bank 2 first; a zero stops the check")

func _test_draw_frame() -> void:
	var log: Array = []
	var e = s.new_engine()
	s.record(e, FRAME_CALLS, log, {"func_80098650": 100})
	var m = e.memory
	m.write_global("draw_buffer", 1)
	var ot: int = 0x800E8C70 + 0x14
	e.call_c("draw_frame", [0])
	s.check(log.map(func(x): return x[0]) == ["func_80098650", "func_8009C210", "func_8009ABF0", "func_8009AC20", "func_8009AD00"] and log[-1] == ["func_8009AD00", ot], "everything off, no present: stamp, C210(1), ABF0, AC20, AD00(ot): %s" % [log])
	s.check(m.read_global("draw_horizontal_blank_stamps", 1) == 100, "stamp 1 is func_80098650()")
	# present path: mode != 0x90 -> C674(0); stamp, limit 0x200: 100 < 0x200 -> VSync(0)
	log.clear()
	e.gs_set("frame_scanline_limit", 0x200)
	e.call_c("draw_frame", [1])
	s.check(log.map(func(x): return x[0]) == ["func_80098650", "func_80098650", "csr_load_vram", "palette_load_vram", "func_8009C674", "func_80098650", "func_80098650", "func_800AD950", "func_80098650", "pad_update", "func_8009C210", "func_8009ABF0", "func_8009AC20", "func_8009AD00"],
		"present path: %s" % [log.map(func(x): return x[0])])
	s.check(log[7] == ["func_800AD950", 0], "VSync(0) while the scanline is below the limit")
	log.clear()
	e.gs_set("game_mode", 0x90)
	e.gs_set("mode_stage", 10)
	e.call_c("draw_frame", [1])
	s.check(log.map(func(x): return x[0]).count("func_8009C674") == 0 and log.filter(func(x): return x[0] == "func_800AD950") == [["func_800AD950", 2]] and log.filter(func(x): return x[0] == "func_8009C210").size() == 2, "mode 0x90 stage >= 10: no DrawSync(0), VSync(2) and an extra C210(1)")
	e.gs_set("mode_stage", 12)
	log.clear()
	e.call_c("draw_frame", [1])
	s.check(log.map(func(x): return x[0]).count("func_8009C674") == 1, "stage 12 still calls func_8009C674")
	e.gs_set("game_mode", 0)
	# 2D layer: sprite quads by quad_layer, string, BG layers
	e.gs_set("draw_2d_enabled", 1)
	e.gs_set("quads_enabled", 1)
	m.write_u8(e.gs_address("quad_layer"), 0x81)
	m.write_u8(e.gs_address("quad_layer") + 1, 5)
	m.write_u8(e.gs_address("bg_layer_order"), 3)
	m.write_u8(e.gs_address("bg_layer_order") + 1, 0x80)
	e.gs_set("back_clear", 1)
	e.gs_set("clear_color_r", 1)
	e.gs_set("clear_color_g", 2)
	e.gs_set("clear_color_b", 3)
	log.clear()
	e.call_c("draw_frame", [0])
	var q: int = m.global_address("sprite_quads")
	s.check(log[1] == ["func_80098680", q, ot, 0] and log[2] == ["func_80098B80", q + 0x24, ot, 4] and log[3] == ["func_80098B80", q + 0x48, ot, 0x7F], "quads: bit 7 -> func_80098680 with (v - 1) & 0x7F, else func_80098B80 (v = 0 -> 0x7F): %s" % [log.slice(1, 4)])
	var names: Array = log.map(func(x): return x[0])
	s.check(names.count("func_80098B80") == 63 and names.count("func_80098680") == 1 and names[65] == "disp_string" and log[66] == ["func_80098D00", m.global_address("bg_layers"), ot, 2] and names[67] == "func_8009C210", "64 quads, disp_string, one BG layer (order 3 -> pri 2; 0x80 skipped): %s" % [names.slice(64, 69)])
	s.check(log.filter(func(x): return x[0] == "func_8009824C") == [["func_8009824C", 1, 2, 3, ot]], "back clear with the clear colour")
	# 3D: func_80099540(&D_80122740), 16 model objects x 5 calls, view_models_update_draw
	e.gs_set("draw_2d_enabled", 0)
	e.gs_set("draw_3d_enabled", 1)
	e.gs_set("back_clear", 0)
	m.write_u32(m.global_address("model_objects") + 4, 0xABCD0000)
	log.clear()
	e.call_c("draw_frame", [0])
	var sp50: int = e.sp - 0xA8 + 0x50
	var sp70: int = e.sp - 0xA8 + 0x70
	s.check(log[1] == ["func_80099540", 0x80122740] and log[2] == ["func_80099B10", 0xABCD0000, sp50, sp70] and log[3] == ["func_80099F04", sp50] and log[4] == ["func_8009A200", 0xABCD0000, sp70] and log[5] == ["func_80099ED8", sp70] and log[6] == ["func_8009A4D0", 0x80122640, ot, 6, 0x1F800000], "3D: first model object: %s" % [log.slice(1, 7)])
	names = log.map(func(x): return x[0])
	s.check(names.count("func_8009A4D0") == 16 and log.filter(func(x): return x[0] == "func_8009A4D0")[15][1] == 0x80122640 + 15 * 16 and names[1 + 1 + 16 * 5] == "view_models_update_draw", "16 objects of 16 bytes, then view_models_update_draw")
