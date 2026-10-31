extends RefCounted
## T-39314: engine/_sdk_etc.gd, _sdk_card.gd, _sdk_gte.gd and the T-39314 additions of _libc.gd on invented memory.
## Expectations are derived by hand from the asm (asm/libetc.s, asm/libgte_8009FA5C.s) and the BIOS event layout
## (psx-spx), and the captures (config/port/engine-diff/sdk_etc.json, sdk_card.json, sdk_gte.json, libc.json) compare
## them with the original.

const GameEngine = preload("res://engine/engine.gd")
const MemoryCardService = preload("res://services/memory_card_service.gd")

var failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	_vsync()
	_events()
	_card_jobs()
	_card_refusals()
	_gte()
	_libc()
	return failures

func _vsync() -> void:
	var engine = GameEngine.new()
	engine.memory.write_u32(0x800E35E0, 1234)
	_check(engine.call_c("func_800AD950", [-1]) == 1234, "VSync(-1) returns the counter word")
	_check(not engine.halted(), "VSync(-1) does not refuse")
	engine.call_c("func_800AD950", [0])
	_check(engine.halted() and engine.refusal.contains("func_800AD950"), "VSync(0) refuses naming func_800AD950")
	engine.shutdown()
	engine = GameEngine.new()
	engine.call_c("func_800AD950", [2])
	_check(engine.halted() and engine.refusal.contains("blocking wait"), "VSync(2) refuses (blocking wait)")
	engine.shutdown()
	engine = GameEngine.new()
	engine.call_c("func_800AD950", [1])
	_check(engine.halted(), "VSync(1) refuses (hardware counter)")
	engine.shutdown()

func _events() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	_check(engine.call_c("EnterCriticalSection") == 1, "EnterCriticalSection returns 1 with interrupts enabled")
	_check(engine.call_c("EnterCriticalSection") == 0, "EnterCriticalSection returns 0 when already inside")
	engine.call_c("ExitCriticalSection")
	_check(engine.call_c("EnterCriticalSection") == 1, "ExitCriticalSection re-enables interrupts")
	# a fresh kernel gets the default table; the first event is entry 0 -> handle 0xF1000000, then 1, 2
	var first: int = engine.call_c("OpenEvent", [0xF4000001, 4, 0x2000, 0]) & 0xFFFFFFFF
	var second: int = engine.call_c("OpenEvent", [0xF4000001, 0x8000, 0x2000, 0]) & 0xFFFFFFFF
	_check(first == 0xF1000000 and second == 0xF1000001, "OpenEvent hands out the first free entries (got 0x%X, 0x%X)" % [first, second])
	var table: int = m.u32(0x80000120)
	_check(table != 0 and m.u32(table) == 0xF4000001 and m.u32(table + 4) == 0x1000 and m.u32(table + 8) == 4 and m.u32(table + 12) == 0x2000, "OpenEvent fills class, status (disabled 0x1000), spec, mode")
	_check(engine.call_c("TestEvent", [first]) == 0, "TestEvent on a disabled event is 0")
	_check(engine.call_c("EnableEvent", [first]) == 1 and m.u32(table + 4) == 0x2000, "EnableEvent sets status 0x2000")
	_check(engine.call_c("TestEvent", [first]) == 0, "TestEvent on an enabled, undelivered event is 0")
	engine.modules["sdk_card"].deliver_event(0xF4000001, 4)
	_check(m.u32(table + 4) == 0x4000, "delivery marks the matching enabled event 0x4000")
	_check(m.u32(table + 0x1C + 4) == 0x1000, "delivery leaves other specs alone")
	_check(engine.call_c("TestEvent", [first]) == 1 and m.u32(table + 4) == 0x2000, "TestEvent returns 1 once and re-arms the event")
	_check(engine.call_c("TestEvent", [first]) == 0, "TestEvent a second time is 0")
	_check(engine.call_c("TestEvent", [0xF1000032]) == 0 and engine.call_c("TestEvent", [5]) == 0, "TestEvent on an invalid handle is 0")
	_check(engine.call_c("CloseEvent", [first]) == 1 and m.u32(table) == 0 and m.u32(table + 4) == 0, "CloseEvent frees the entry")
	_check(engine.call_c("OpenEvent", [0xF0000011, 0x100, 0x2000, 0]) & 0xFFFFFFFF == 0xF1000000, "a freed entry is reused")
	# an event with a callback cannot be delivered (interrupt code is not modelled)
	engine.call_c("OpenEvent", [0xF4000001, 0x77, 0x1000, 0x80042000])
	engine.call_c("EnableEvent", [0xF1000002])
	engine.modules["sdk_card"].deliver_event(0xF4000001, 0x77)
	_check(engine.halted() and engine.refusal.contains("callback"), "delivery to a callback event refuses")
	engine.shutdown()

func _card_jobs() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	engine.call_c("_card_info", [0])
	_check(engine.halted() and engine.refusal.contains("_card_info"), "_card_info refuses without a memory card attached")
	engine.shutdown()
	engine = GameEngine.new()
	m = engine.memory
	engine.memory_card = MemoryCardService.new(OS.get_temp_dir().path_join("sdk-card-test-none"))
	engine.memory_card.inserted = false
	engine.call_c("_card_info", [0])
	_check(engine.halted() and engine.refusal.contains("no card in the slot"), "_card_info refuses for an empty slot")
	engine.shutdown()
	engine = GameEngine.new()
	m = engine.memory
	engine.memory_card = MemoryCardService.new(OS.get_temp_dir().path_join("sdk-card-test-blank"))
	var handle: int = engine.call_c("OpenEvent", [0xF4000001, 4, 0x2000, 0])
	engine.call_c("EnableEvent", [handle])
	var frame_address: int = engine.gs_address("frame_count")
	m.write_u32(frame_address, 100)
	_check(engine.call_c("_card_info", [0]) == 1, "_card_info returns 1")
	m.write_u32(frame_address, 101)
	_check(engine.call_c("TestEvent", [handle]) == 0, "the info event is not delivered one frame later")
	m.write_u32(frame_address, 102)
	_check(engine.call_c("TestEvent", [handle]) == 1, "the info event is delivered INFO_FRAMES (2) frames later")
	_check(engine.call_c("TestEvent", [handle]) == 0, "the event is seen once")
	m.write_u32(frame_address, 110)
	_check(engine.call_c("_card_load", [0]) == 1, "_card_load returns 1")
	m.write_u32(frame_address, 181)
	_check(engine.call_c("TestEvent", [handle]) == 0, "the load event is not delivered after 71 frames")
	m.write_u32(frame_address, 182)
	_check(engine.call_c("TestEvent", [handle]) == 1, "the load event is delivered LOAD_FRAMES (72) frames later")
	_check(not engine.halted(), "the card job path does not refuse")
	engine.shutdown()

func _card_refusals() -> void:
	for pair: Array in [["_card_status", [0]], ["_card_clear", [0]], ["open", [0x80100000, 2]], ["close", [2]], ["read", [2, 0x80100000, 0x2000]],
			["write", [2, 0x80100000, 0x2000]], ["format", [0x80100000]], ["delete", [0x80100000]]]:
		var engine = GameEngine.new()
		engine.call_c(pair[0], pair[1])
		_check(engine.halted() and engine.refusal.contains(pair[0]), "%s refuses by name (got '%s')" % [pair[0], engine.refusal])
		engine.shutdown()

func _gte() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# an invented table T[i] = i for 0..0x400 at 0x800DD6C0 (the library indexes it at shifted bases)
	for i: int in 0x401:
		m.write_s16(0x800DD6C0 + i * 2, i)
	# rsin(0x400) = T[0x400]; rsin(0x500) = T[0x800 - 0x500]; rsin(0x900) = -T[0x100] (D_800DC6C0 + a * 2 = T + (a - 0x800));
	# rsin(0xD00) = -T[0x1000 - 0xD00]; rsin(-0x100) = -rsin(0x100)
	_check(engine.call_c("rsin", [0x400]) == 0x400 and engine.call_c("rsin", [0x500]) == 0x300, "rsin first half-wave")
	_check(engine.call_c("rsin", [0x900]) == -0x100 and engine.call_c("rsin", [0xD00]) == -0x300, "rsin second half-wave is negative")
	_check(engine.call_c("rsin", [-0x100]) == -0x100 and engine.call_c("rsin", [0x1100]) == 0x100, "rsin is odd and periodic with 4096")
	# rcos(0) = T[0x400]; rcos(0x500) = -T[0x100]; rcos(0x900) = -T[0xC00 - 0x900]; rcos(0xD00) = T + (0xD00 * 2 - 0x1000 - 0x800 ...)
	_check(engine.call_c("func_800A0140", [0]) == 0x400 and engine.call_c("func_800A0140", [0x500]) == -0x100, "rcos first half")
	_check(engine.call_c("func_800A0140", [0x900]) == -0x300 and engine.call_c("func_800A0140", [-0x100]) == 0x300, "rcos second half and evenness")
	engine.shutdown()

func _libc() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# rand: state' = state * 0x41C64E6D + 0x3039; result = (state' >> 16) & 0x7FFF. srand(1): 0x41C64E6D + 0x3039 = 0x41C67EA6 -> 0x41C6
	engine.call_c("srand", [1])
	_check(m.u32(0x8000850C) == 1, "srand stores the seed in the BIOS seed word")
	_check(engine.call_c("rand") == 0x41C6 and m.u32(0x8000850C) == 0x41C67EA6, "rand from seed 1")
	var second: int = engine.call_c("rand")
	var expected_state: int = (0x41C67EA6 * 0x41C64E6D + 0x3039) & 0xFFFFFFFF
	_check(second == ((expected_state >> 16) & 0x7FFF) and m.u32(0x8000850C) == expected_state, "rand advances the RAM seed")
	_check(engine.call_c("abs", [-5]) == 5 and engine.call_c("abs", [7]) == 7 and engine.call_c("abs", [0x80000000]) == -2147483648, "abs, including abs(INT_MIN)")
	# printf drops its text and changes no memory
	m.write_bytes(0x80100000, "value %d\n".to_ascii_buffer() + PackedByteArray([0]))
	var before: PackedByteArray = m.snapshot()
	engine.call_c("printf", [0x80100000, 5])
	_check(m.snapshot() == before and not engine.halted(), "printf writes nothing")
	# sprintf (asm/libc_sprintf.s): the conversions below, formatted by hand
	engine.shutdown()
	engine = GameEngine.new()
	m = engine.memory
	m.write_bytes(0x80100100, "ab".to_ascii_buffer() + PackedByteArray([0]))
	m.write_bytes(0x80100200, "[%s|%d|%5d|%-5d|%05d]".to_ascii_buffer() + PackedByteArray([0]))
	var n: int = engine.call_c("sprintf", [0x80100300, 0x80100200, 0x80100100, 42, 42, 42, 42])
	var text: String = m.read_bytes(0x80100300, n).get_string_from_ascii()
	_check(text == "[ab|42|   42|42   |00042]" and m.u8(0x80100300 + n) == 0, "sprintf formats %%s %%d with widths (got '%s')" % text)
	m.write_bytes(0x80100200, "%x %X %c %% %u %i".to_ascii_buffer() + PackedByteArray([0]))
	n = engine.call_c("sprintf", [0x80100300, 0x80100200, 255, 255, 65, 0xFFFFFFFF, -3])
	text = m.read_bytes(0x80100300, n).get_string_from_ascii()
	_check(text == "ff FF A % 4294967295 -3" and not engine.halted(), "sprintf %%x %%X %%c %%u %%i (got '%s')" % text)
	m.write_bytes(0x80100200, "%03d|%+d|% d|%.3d|%5.2s|".to_ascii_buffer() + PackedByteArray([0]))
	n = engine.call_c("sprintf", [0x80100300, 0x80100200, 7, 7, 7, 7, 0x80100100])
	text = m.read_bytes(0x80100300, n).get_string_from_ascii()
	_check(text == "007|+7| 7|007|   ab|" and not engine.halted(), "sprintf flags and precision (got '%s')" % text)
	m.write_bytes(0x80100200, "%f".to_ascii_buffer() + PackedByteArray([0]))
	engine.call_c("sprintf", [0x80100300, 0x80100200, 1])
	_check(engine.halted() and engine.refusal.contains("sprintf"), "an unmodelled conversion refuses by name")
	engine.shutdown()
