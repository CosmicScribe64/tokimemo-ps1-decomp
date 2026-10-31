extends RefCounted
## Port of src/main/reset_iris_wipe.c (original object 0x80046500-0x80047550, T-39301), the part the
## game-mode and CD code need: game_mode_reset_to_title and the two decimal helpers. The 4-bit iris/radial
## wipe texture and draw functions (graphics primitives) are not ported yet and are listed in C_UNPORTED.
## Contract: wiki/engine-porting-contract.md. Test: config/port/engine-diff/reset_iris_wipe.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/reset_iris_wipe.c"
const C_FUNCTIONS: Array[String] = [
	"game_mode_reset_to_title", "cd_decimal_digit", "cd_binary_coded_decimal_to_integer",
]
## Functions of the C file that no module ports yet (empty when the file is complete).
const C_UNPORTED: Array[String] = ["cd_iris_texture_build", "cd_iris_wipe_draw"]
const C_SIGNATURES: Dictionary = {
	"game_mode_reset_to_title": {"args": [], "ret": "s32"},
	"cd_decimal_digit": {"args": ["s16", "u8"], "ret": "s32"},
	"cd_binary_coded_decimal_to_integer": {"args": ["s32"], "ret": "s32"},
}

const GAME_MODE_TITLE: int = 0x12

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

## Resets everything the title screen needs and returns -1. game_mode_set and cd_command_reset are ported
## (engine/game_mode.gd, engine/cd_queue.gd); every other callee belongs to a C file no module ports yet.
## The calls run in the C order and call_c is inert once one of them refuses, so the state at the refusal
## is the state the original has at that call.
func game_mode_reset_to_title() -> int:
	engine.call_c("ovl_load_reset")
	engine.call_c("game_state_init", [0])
	engine.call_c("game_mode_set", [GAME_MODE_TITLE])
	engine.call_c("sprite_tables_clear")
	engine.call_c("actor_deactivate", [0])
	engine.call_c("back_clear_switch", [1])
	engine.call_c("dec_bg_reset")
	engine.call_c("audio_bank_state_reset")
	engine.call_c("tpage_buf_clear")
	engine.call_c("audio_command", [0x7F])
	engine.call_c("audio_shutdown")
	engine.call_c("audio_init")
	engine.call_c("cd_command_reset")
	engine.call_c("boot_game_data_init")
	engine.call_c("boot_settings_defaults_set")
	return -1

## Decimal digit `arg1` (1 = ones) of `arg0`, 0 when arg1 >= 6.
@warning_ignore("integer_division")
func cd_decimal_digit(arg0: int, arg1: int) -> int:
	var value: int = Math.s16(arg0)
	var place: int = Math.u8(arg1)
	var p: int = 1
	if place >= 6:
		return 0
	var i: int = 0
	if place != 0:
		while true: # do { i += 1; p *= 10; } while (i < arg1)
			i += 1
			p *= 10
			if i >= place:
				break
	if p / 10 == 0:
		# arg1 == 0 divides by zero; the original traps and no caller passes 0. Refuse, never invent a digit.
		engine.refuse("cd_decimal_digit", "division by zero (digit place 0)")
		return 0
	return (value / (p / 10)) % 10

## Packed BCD word to integer, eight nibbles, on s32 arithmetic (the product wraps like the R3000 mult).
func cd_binary_coded_decimal_to_integer(arg0: int) -> int:
	var value: int = Math.s32(arg0)
	var result: int = 0
	var mul: int = 1
	for i: int in 8:
		var masked: int = Math.s32(value & Math.s32(0xF << (i * 4)))
		result = Math.add32(result, Math.sra32(Math.s32(mul * masked), i * 4))
		mul = Math.s32(mul * 10)
	return result
