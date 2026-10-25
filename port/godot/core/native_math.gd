extends RefCounted
## T-29002: explicit R3000 word operations; no floating-point intermediates.
## These are instruction semantics, not a claim that undefined C is portable.

const MASK32: int = 0xffffffff
const MIN64: int = -9223372036854775807 - 1

class WordPair extends RefCounted:
	var lo: int
	var hi: int

	func _init(low_word: int, high_word: int) -> void:
		lo = low_word
		hi = high_word

class Division extends RefCounted:
	var quotient: int = 0
	var remainder: int = 0
	var error: String = ""

	func _init(q: int = 0, r: int = 0, failure: String = "") -> void:
		quotient = q
		remainder = r
		error = failure

static func u8(value: int) -> int:
	return value & 0xff

static func s8(value: int) -> int:
	return ((value & 0xff) ^ 0x80) - 0x80

static func u16(value: int) -> int:
	return value & 0xffff

static func s16(value: int) -> int:
	return ((value & 0xffff) ^ 0x8000) - 0x8000

static func u32(value: int) -> int:
	return value & MASK32

static func s32(value: int) -> int:
	return ((value & MASK32) ^ 0x80000000) - 0x80000000

static func add32(left: int, right: int) -> int:
	return s32(u32(left) + u32(right))

static func sub32(left: int, right: int) -> int:
	return s32(u32(left) - u32(right))

static func unsigned_less32(left: int, right: int) -> bool:
	return u32(left) < u32(right)

static func signed_less32(left: int, right: int) -> bool:
	return s32(left) < s32(right)

static func sll32(value: int, count: int) -> int:
	return s32(u32(value) << (count & 31))

static func srl32(value: int, count: int) -> int:
	return u32(value) >> (count & 31)

static func sra32(value: int, count: int) -> int:
	return s32(value) >> (count & 31)

static func multu32(left: int, right: int) -> WordPair:
	# A full unsigned product may exceed INT64_MAX. Each limb stays below 2^34.
	var a: int = u32(left)
	var b: int = u32(right)
	var low_product: int = (a & 0xffff) * (b & 0xffff)
	var middle: int = (a >> 16) * (b & 0xffff) + (low_product >> 16)
	var upper: int = middle >> 16
	middle = (middle & 0xffff) + (a & 0xffff) * (b >> 16)
	return WordPair.new(
		((middle & 0xffff) << 16) | (low_product & 0xffff),
		u32((a >> 16) * (b >> 16) + upper + (middle >> 16)))

static func mult32(left: int, right: int) -> WordPair:
	var product: WordPair = multu32(left, right)
	if s32(left) < 0:
		product.hi = u32(product.hi - u32(right))
	if s32(right) < 0:
		product.hi = u32(product.hi - u32(left))
	return product

@warning_ignore("integer_division")
static func divmod32(dividend: int, divisor: int) -> Division:
	var a: int = s32(dividend)
	var b: int = s32(divisor)
	# Raw DIV behavior. Compiled C may branch to BREAK before reaching DIV.
	if b == 0:
		return Division.new(-1 if a >= 0 else 1, a)
	if a == -2147483648 and b == -1:
		return Division.new(-2147483648, 0)
	return Division.new(a / b, a % b)

@warning_ignore("integer_division")
static func divmodu32(dividend: int, divisor: int) -> Division:
	var a: int = u32(dividend)
	var b: int = u32(divisor)
	if b == 0:
		return Division.new(MASK32, a)
	return Division.new(a / b, a % b)

@warning_ignore("integer_division")
static func divmod64(dividend: int, divisor: int) -> Division:
	# Guard Godot's only signed division overflow, including release builds.
	if divisor == 0:
		return Division.new(0, 0, "signed64 division by zero")
	if dividend == MIN64 and divisor == -1:
		return Division.new(0, 0, "signed64 quotient is outside int64")
	return Division.new(dividend / divisor, dividend % divisor)

static func parameter_change_word(word: int, delta: int) -> int:
	# src/main/hud_panels.c::parameter_change: sh cap keeps the fraction.
	var result: int = add32(word, delta)
	if result < 0:
		result = 0
	if s16(result >> 16) >= 1000:
		result = (999 << 16) | (result & 0xffff)
	return result

static func check_parameter_limit_word(word: int) -> int:
	# src/main/scene_library.c::check_para_limit: sw clears it at >= 999.
	var high: int = s16(u32(word) >> 16)
	if high < 0:
		return 0
	if high >= 999:
		return 999 << 16
	return s32(word)
