extends RefCounted
## T-39301: engine/_libc.gd on invented memory (hand-derived from the PS1 libc contracts). The emulator
## captures (config/port/engine-diff/libc.json) compare the same functions against the BIOS/SDK code.

const GameEngine = preload("res://engine/engine.gd")

var failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func _put(engine, address: int, text: String) -> void:
	var bytes: PackedByteArray = text.to_ascii_buffer()
	bytes.append(0)
	engine.memory.write_bytes(address, bytes)

func _get_string(engine, address: int) -> String:
	var bytes := PackedByteArray()
	var cursor: int = address
	while engine.memory.u8(cursor) != 0 and bytes.size() < 64:
		bytes.append(engine.memory.u8(cursor))
		cursor += 1
	return bytes.get_string_from_ascii()

func run() -> Array[String]:
	failures.clear()
	var engine = GameEngine.new()
	var a: int = 0x80100000
	var b: int = 0x80100100
	_put(engine, a, "HELLO")
	_check(engine.call_c("strlen", [a]) == 5, "strlen")
	_check(engine.call_c("strcpy", [b, a]) == b and _get_string(engine, b) == "HELLO", "strcpy returns dst and copies the NUL")
	_check(engine.call_c("strcat", [b, a]) == b and _get_string(engine, b) == "HELLOHELLO", "strcat appends")
	engine.memory.fill(b, 16, 0x55)
	_check(engine.call_c("strncpy", [b, a, 8]) == b and engine.memory.read_bytes(b, 9) == PackedByteArray([72, 69, 76, 76, 79, 0, 0, 0, 0x55]), "strncpy pads with NULs up to n and no further")
	_put(engine, b, "AB")
	_check(engine.call_c("strncat", [b, a, 3]) == b and _get_string(engine, b) == "ABHEL" and engine.memory.u8(b + 5) == 0, "strncat appends at most n bytes and a NUL")
	_check(engine.call_c("strcmp", [a, a]) == 0 and engine.call_c("strcmp", [b, a]) < 0 and engine.call_c("strcmp", [a, b]) > 0, "strcmp sign")
	_check(engine.call_c("memcpy", [b, a, 3]) == b and engine.memory.read_bytes(b, 3) == "HEL".to_ascii_buffer(), "memcpy returns dst")
	engine.call_c("memcpy", [b, a, 0])
	_check(engine.memory.u8(b) == 72, "memcpy of 0 bytes does nothing")
	# memmove (asm/libc_memmove.s): dst < src copies forward and returns dst + n; dst >= src copies backward, returns dst
	_put(engine, a, "0123456789")
	_check(engine.call_c("memmove", [a, a + 2, 5]) == a + 5 and _get_string(engine, a) == "2345656789", "memmove dst < src: forward copy, returns dst + n")
	_put(engine, a, "0123456789")
	_check(engine.call_c("memmove", [a + 2, a, 5]) == a + 2 and _get_string(engine, a) == "0101234789", "memmove dst > src: backward copy keeps overlapping data, returns dst")
	_check(engine.call_c("memmove", [a, a + 4, 0]) == a and engine.call_c("memmove", [a, a + 4, -3]) == a, "memmove of n <= 0 copies nothing and returns dst")
	_put(engine, a, "WXYZ")
	engine.call_c("bzero", [a, 2])
	_check(engine.memory.read_bytes(a, 4) == PackedByteArray([0, 0, 89, 90]), "bzero clears n bytes")
	# a bad address refuses and freezes
	engine.call_c("strlen", [0x00300000])
	_check(engine.halted(), "a string at an invalid address faults")
	engine.shutdown()
	return failures
