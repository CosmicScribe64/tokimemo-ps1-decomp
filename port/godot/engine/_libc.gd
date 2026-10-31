extends RefCounted
## T-39301: the C library layer of the facade (not a C file of the game; the underscore keeps the directory
## scan from registering it, engine.gd registers it as module "libc"). The game calls these through BIOS
## A-function stubs (asm/libapi_1.s, libapi_2.s: `addiu $t1, N; jr $t2` into the A0 table) and, for memmove,
## SDK C code (asm/libc_memmove.s). Semantics are the PS1 libc ones the game relies on, on game memory:
## the functions return the destination pointer (memmove: see below), copy bytes forward in order, and
## touch RAM only through the engine's memory (so a refusal or fault freezes them like any module).
## Differential tests: config/port/engine-diff/libc.json against emulator captures (OpenBIOS executes the
## BIOS ones in the emulator; real BIOS code is equivalent for these routines).
## memset is not in the game (no symbol), so it is not provided; add it with a capture when a caller appears.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "libc (asm/libapi_1.s, libapi_2.s, libc_memmove.s)"
const C_FUNCTIONS: Array[String] = ["memcpy", "memmove", "strlen", "strcpy", "strncpy", "strcat", "strncat", "strcmp", "bzero"]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"memcpy": {"args": ["ptr", "ptr", "u32"], "ret": "ptr"},
	"memmove": {"args": ["ptr", "ptr", "s32"], "ret": "ptr"},
	"strlen": {"args": ["ptr"], "ret": "s32"},
	"strcpy": {"args": ["ptr", "ptr"], "ret": "ptr"},
	"strncpy": {"args": ["ptr", "ptr", "s32"], "ret": "ptr"},
	"strcat": {"args": ["ptr", "ptr"], "ret": "ptr"},
	"strncat": {"args": ["ptr", "ptr", "s32"], "ret": "ptr"},
	"strcmp": {"args": ["ptr", "ptr"], "ret": "s32"},
	"bzero": {"args": ["ptr", "s32"], "ret": "void"},
}

var engine # GameEngine

func memcpy(dst: int, src: int, count: int) -> int:
	var n: int = Math.u32(count)
	if n > 0:
		engine.memory.write_bytes(dst, engine.memory.read_bytes(src, n))
	return Math.u32(dst)

## SDK memmove (asm/libc_memmove.s): dst < src (unsigned) copies forward and returns the advanced dst
## (dst + n, the register the loop leaves), else copies backward and returns dst; n <= 0 (signed) copies nothing.
func memmove(dst: int, src: int, count: int) -> int:
	var n: int = Math.s32(count)
	var a: int = Math.u32(dst)
	var b: int = Math.u32(src)
	if n <= 0:
		return a
	var data: PackedByteArray = engine.memory.read_bytes(b, n)
	engine.memory.write_bytes(a, data)
	return Math.u32(a + n) if a < b else a

func _c_string(address: int) -> PackedByteArray:
	var bytes := PackedByteArray()
	var cursor: int = Math.u32(address)
	while not engine.halted():
		var value: int = engine.memory.u8(cursor)
		if value == 0:
			break
		bytes.append(value)
		cursor += 1
		if bytes.size() > 0x200000:
			engine.refuse("strlen", "unterminated string at 0x%08X" % address)
	return bytes

func strlen(s: int) -> int:
	return _c_string(s).size()

func strcpy(dst: int, src: int) -> int:
	var text: PackedByteArray = _c_string(src)
	text.append(0)
	engine.memory.write_bytes(dst, text)
	return Math.u32(dst)

func strncpy(dst: int, src: int, count: int) -> int:
	var n: int = Math.s32(count)
	if n > 0:
		var text: PackedByteArray = _c_string(src)
		var out := PackedByteArray()
		out.resize(n)
		out.fill(0)
		for i: int in mini(text.size(), n):
			out[i] = text[i]
		engine.memory.write_bytes(dst, out)
	return Math.u32(dst)

func strcat(dst: int, src: int) -> int:
	var end: int = Math.u32(dst) + _c_string(dst).size()
	var text: PackedByteArray = _c_string(src)
	text.append(0)
	engine.memory.write_bytes(end, text)
	return Math.u32(dst)

func strncat(dst: int, src: int, count: int) -> int:
	var n: int = Math.s32(count)
	var end: int = Math.u32(dst) + _c_string(dst).size()
	var text: PackedByteArray = _c_string(src)
	var out: PackedByteArray = text.slice(0, maxi(0, mini(text.size(), n)))
	out.append(0)
	engine.memory.write_bytes(end, out)
	return Math.u32(dst)

## Difference of the first differing bytes (0 when equal).
func strcmp(a: int, b: int) -> int:
	var left: PackedByteArray = _c_string(a)
	var right: PackedByteArray = _c_string(b)
	left.append(0)
	right.append(0)
	for i: int in mini(left.size(), right.size()):
		if left[i] != right[i]:
			return left[i] - right[i]
	return 0

func bzero(dst: int, count: int) -> void:
	var n: int = Math.s32(count)
	if n > 0:
		engine.memory.fill(dst, n, 0)
