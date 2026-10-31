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
const NativeRng = preload("res://core/native_rng.gd")

const C_FILE: String = "libc (asm/libapi_1.s, libapi_2.s, libc_memmove.s)"
const C_FUNCTIONS: Array[String] = ["memcpy", "memmove", "strlen", "strcpy", "strncpy", "strcat", "strncat", "strcmp", "bzero",
	"rand", "srand", "abs", "printf", "sprintf"]
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
	"rand": {"args": [], "ret": "s32"},
	"srand": {"args": ["s32"], "ret": "void"},
	"abs": {"args": ["s32"], "ret": "s32"},
	# variadic (T-39314): "args" are the required ones, "variadic" the number of extra integer arguments a caller
	# may add (call_c pads the rest with 0); the GDScript method declares all of them
	"printf": {"args": ["ptr"], "ret": "s32", "variadic": 6, "ret_undefined": "the BIOS returns the number of characters it printed; the port drops the text and every caller in the game discards the value"},
	"sprintf": {"args": ["ptr", "ptr"], "ret": "s32", "variadic": 6},
}

## T-39314: where the BIOS keeps the rand/srand seed. A0[2f]/A0[30] are BIOS kernel routines, so the seed is
## kernel RAM, not game RAM. 0x8000850C is where OpenBIOS (the BIOS the emulator captures run on) keeps it:
## every rand/srand capture (config/port/engine-diff/libc.json, captures libc-*, gpu-*) changes exactly this
## word. The retail BIOS keeps its seed in kernel RAM as well, at an address this project has not observed.
const BIOS_RAND_SEED: int = 0x8000850C
var _rng := NativeRng.new()

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


# ---- T-39314: rand, srand, abs, printf, sprintf

## rand (BIOS A0[2f]): the LCG core/native_rng.gd models (state * 0x41C64E6D + 0x3039, result bits 30..16). The
## state is the seed word in RAM, so a saved RAM image carries the generator; the RNG object is only the
## algorithm (its trace is not kept).
func rand() -> int:
	_rng.profile = NativeRng.OPENBIOS_PROFILE
	_rng.error = ""
	_rng.state = engine.memory.u32(BIOS_RAND_SEED)
	var value: int = _rng.next("rand")
	engine.memory.write_u32(BIOS_RAND_SEED, _rng.state)
	_rng.trace.clear()
	return value

## srand (BIOS A0[30]): the seed word becomes the argument.
func srand(seed_value: int) -> void:
	engine.memory.write_u32(BIOS_RAND_SEED, Math.u32(seed_value))

## abs (BIOS A0[0e]): two's complement; abs(-2^31) stays -2^31.
func abs(value: int) -> int:
	var v: int = Math.s32(value)
	return Math.s32(-v) if v < 0 else v

## printf (BIOS A0[3f]) writes its formatted text to the BIOS standard output (a TTY console device) and
## changes no game memory: the formatting reads the arguments and strings only. The port has no console, so the
## text is dropped; the capture comparison (config/port/engine-diff/libc.json, printf hits) fails if a call ever
## writes game RAM. Returns 0: the value is not used by the game's callers (they discard it).
func printf(_fmt: int, _a1: int = 0, _a2: int = 0, _a3: int = 0, _a4: int = 0, _a5: int = 0, _a6: int = 0) -> int:
	return 0

## sprintf (SDK libc SPRINTF.OBJ, asm/libc_sprintf.s): see _sprintf_format.
func sprintf(dst: int, fmt: int, a1: int = 0, a2: int = 0, a3: int = 0, a4: int = 0, a5: int = 0, a6: int = 0) -> int:
	var arguments: Array[int] = [a1, a2, a3, a4, a5, a6]
	var out: PackedByteArray = _sprintf_format(fmt, arguments)
	if engine.halted():
		return 0
	var text := out.duplicate()
	text.append(0)
	engine.memory.write_bytes(dst, text)
	return out.size()

## The conversions the library implements that the game uses: %% %c %s %d %i %u %x %X %o with flags - 0 + space,
## width, precision and the h/l length modifiers. Anything else refuses by name (no value is invented).
func _sprintf_format(fmt: int, arguments: Array[int]) -> PackedByteArray:
	var out := PackedByteArray()
	var format: PackedByteArray = _c_string(fmt)
	var next_arg: int = 0
	var i: int = 0
	while i < format.size() and not engine.halted():
		var c: int = format[i]
		i += 1
		if c != 0x25:
			out.append(c)
			continue
		var left: bool = false
		var zero: bool = false
		var plus: bool = false
		var space: bool = false
		while i < format.size() and format[i] in [0x2D, 0x30, 0x2B, 0x20]:
			match format[i]:
				0x2D: left = true
				0x30: zero = true
				0x2B: plus = true
				0x20: space = true
			i += 1
		var width: int = 0
		while i < format.size() and format[i] >= 0x30 and format[i] <= 0x39:
			width = width * 10 + format[i] - 0x30
			i += 1
		var precision: int = -1
		if i < format.size() and format[i] == 0x2E:
			i += 1
			precision = 0
			while i < format.size() and format[i] >= 0x30 and format[i] <= 0x39:
				precision = precision * 10 + format[i] - 0x30
				i += 1
		var length: int = 0 # 0 int, 1 short, 2 long (the R3000 int and long are both 32 bits)
		while i < format.size() and format[i] in [0x68, 0x6C]:
			length = 1 if format[i] == 0x68 else 2
			i += 1
		if i >= format.size():
			engine.refuse("sprintf", "format ends inside a conversion at 0x%08X" % fmt)
			break
		var conv: int = format[i]
		i += 1
		var body := PackedByteArray()
		var sign_text: String = ""
		match conv:
			0x25:
				body.append(0x25)
			0x63:
				body.append(_next_argument(arguments, next_arg) & 0xFF)
				next_arg += 1
			0x73:
				var address: int = _next_argument(arguments, next_arg)
				next_arg += 1
				body = _c_string(address)
				if precision >= 0 and body.size() > precision:
					body = body.slice(0, precision)
				zero = false
			0x64, 0x69, 0x75, 0x78, 0x58, 0x6F:
				var value: int = _next_argument(arguments, next_arg)
				next_arg += 1
				var signed: bool = conv == 0x64 or conv == 0x69
				if length == 1:
					value = Math.s16(value) if signed else Math.u16(value)
				else:
					value = Math.s32(value) if signed else Math.u32(value)
				var digits: String
				if signed:
					if value < 0:
						sign_text = "-"
					elif plus:
						sign_text = "+"
					elif space:
						sign_text = " "
					digits = str(absi(value))
				elif conv == 0x75:
					digits = str(value)
				elif conv == 0x6F:
					digits = "%o" % value
				elif conv == 0x78:
					digits = "%x" % value
				else:
					digits = "%X" % value
				if precision >= 0:
					zero = false
					while digits.length() < precision:
						digits = "0" + digits
					if precision == 0 and value == 0:
						digits = ""
				body = (sign_text + digits).to_ascii_buffer()
			_:
				engine.refuse("sprintf", "conversion %%%c at 0x%08X is not modelled" % [conv, fmt])
				break
		var pad: int = width - body.size()
		if pad > 0:
			if left:
				out.append_array(body)
				for _k: int in pad:
					out.append(0x20)
			elif zero and conv != 0x63 and conv != 0x73 and conv != 0x25:
				var sign_bytes: int = sign_text.length()
				out.append_array(body.slice(0, sign_bytes))
				for _k: int in pad:
					out.append(0x30)
				out.append_array(body.slice(sign_bytes))
			else:
				for _k: int in pad:
					out.append(0x20)
				out.append_array(body)
		else:
			out.append_array(body)
	return out

func _next_argument(arguments: Array[int], index: int) -> int:
	if index >= arguments.size():
		engine.refuse("sprintf", "more than %d conversion arguments" % arguments.size())
		return 0
	return arguments[index]
