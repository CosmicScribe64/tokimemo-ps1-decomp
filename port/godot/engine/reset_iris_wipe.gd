extends RefCounted
## Port of src/main/reset_iris_wipe.c (original object 0x80046500-0x80047550, T-39301), the part the
## game-mode and CD code need: game_mode_reset_to_title and the two decimal helpers, plus (T-39311) the 4-bit
## iris/radial wipe texture builder and the iris wipe draw (GT4 quadrants and edge tiles built in RAM).
## Contract: wiki/engine-porting-contract.md. Test: config/port/engine-diff/reset_iris_wipe.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/reset_iris_wipe.c"
const C_FUNCTIONS: Array[String] = [
	"game_mode_reset_to_title", "cd_decimal_digit", "cd_binary_coded_decimal_to_integer",
	"cd_iris_texture_build", "cd_iris_wipe_draw",
]
## Functions of the C file that no module ports yet (empty when the file is complete).
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"game_mode_reset_to_title": {"args": [], "ret": "s32"},
	"cd_decimal_digit": {"args": ["s16", "u8"], "ret": "s32"},
	"cd_binary_coded_decimal_to_integer": {"args": ["s32"], "ret": "s32"},
	"cd_iris_texture_build": {"args": ["ptr", "s32", "s32"], "ret": "void"},
	"cd_iris_wipe_draw": {"args": ["s32", "s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "s32", "ret_undefined": "s32 in the C with no return statement"},
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

## Builds the 128 x 128, 4-bit iris texture (16 words per row, 2048 words) at arg0 and uploads it with
## LoadSquare(page x, page y, 32, 128): a radial shade (arg2 / 16 & 0xFF nonzero: shade 15 down to 1 by the
## first ring (15 - shade)^2 * 64 that contains the pixel's distance squared) or a flat disc (colour arg2 & 15
## inside radius 126, or colour 1 outside radius 126 when the colour is 0). Page: x = (arg1 & 15) << 6,
## y = (arg1 & 0x10) * 16 + (arg1 & 0x20) * 4.
@warning_ignore("integer_division")
func cd_iris_texture_build(arg0: int, arg1: int, arg2: int) -> void:
	var m: Object = engine.memory
	for x: int in 16:
		for y: int in 128:
			m.write_u32(arg0 + (x + y * 16) * 4, 0)
	if (arg2 / 16) & 0xFF:
		for x: int in 128:
			for y: int in 128:
				var d: int = x * x + y * y
				var shade: int = 15
				while shade != 0:
					if d < (((15 - shade) * (15 - shade)) << 6):
						var at: int = arg0 + (x / 8 + y * 16) * 4
						m.write_u32(at, m.u32(at) | (shade << ((x % 8) * 4)))
						break
					shade -= 1
	else:
		var color: int = arg2 & 15
		for x: int in 128:
			for y: int in 128:
				var d: int = x * x + y * y
				var at: int = arg0 + (x / 8 + y * 16) * 4
				if color != 0:
					if d < 0x3F01:
						m.write_u32(at, m.u32(at) | (color << ((x % 8) * 4)))
				elif d >= 0x3F02:
					m.write_u32(at, m.u32(at) | (1 << ((x % 8) * 4)))
	engine.call_c("LoadSquare", [(arg1 & 15) << 6, Math.u16(((arg1 & 16) * 16) + ((arg1 & 32) * 4)), 32, 128, arg0])

## The iris wipe: four POLY_GT4 quadrants (each 128 texels scaled to radius = (arg3 << 7) / 4096 around
## (arg4, arg5), mirrored by the quadrant: x/y signs), grey 0x80 or the colour of arg6 (semi-transparent when
## its top nibble is set), each followed by dtd_on_tpage; then, when (u8)(arg6 >> 24) & 0xF is nonzero, four
## TILE bars covering the screen outside the iris (added only when their extent is positive). `order` = arg7 & 15
## is the ordering-table slot.
@warning_ignore("integer_division")
func cd_iris_wipe_draw(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int, arg7: int) -> int:
	var m: Object = engine.memory
	var ot_base: int = m.global_address("draw_ot_tags")
	var order: int = arg7 & 0xF
	var row: int = (arg0 & 0x20) * 4
	var alpha: int = Math.u8(Math.sra32(arg6, 0x1C))
	for quadrant: int in 4:
		if engine.halted():
			return 0
		var db: int = m.read_global("draw_buffer")
		var primitive: int = Math.u32(engine.call_c("GetWorkBase", [0x34, db]))
		engine.call_c("func_8009F068", [primitive])
		if alpha & 0xF:
			engine.call_c("SetSemiTrans", [primitive, 1])
		else:
			engine.call_c("SetSemiTrans", [primitive, 0])
		var red: int = 0x80
		var green: int = 0x80
		var blue: int = 0x80
		if alpha & 0xF:
			red = (Math.srl32(arg6, 0x10) & 0xFFFF) & 0xFF
			green = Math.sra32(arg6, 8) & 0xFF
			blue = arg6 & 0xFF
		for corner: int in [0x04, 0x10, 0x1C, 0x28]:
			m.write_u8(primitive + corner, red)
			m.write_u8(primitive + corner + 1, green)
			m.write_u8(primitive + corner + 2, blue)
		db = m.read_global("draw_buffer")
		var ox: int = m.s16(engine.gs_address("draw_origin_x") + db * 2)
		var oy: int = m.s16(engine.gs_address("draw_origin_y") + db * 2)
		var radius: int = Math.sll32(arg3, 7) / 4096
		# vertex offsets of the quadrant: (x1, y1, x2, y2, x3, y3) as multiples of the radius
		var sx: int = 1 if quadrant < 2 else -1 # x1 and x3 direction: + for quadrants 0, 1; - for 2, 3
		var sy: int = 1 if quadrant == 0 or quadrant == 2 else -1 # y2 and y3 direction: + for 0, 2; - for 1, 3
		m.write_s16(primitive + 0x08, ox + arg4)
		m.write_s16(primitive + 0x0A, oy + arg5)
		m.write_s16(primitive + 0x14, ox + arg4 + sx * radius)
		m.write_s16(primitive + 0x16, oy + arg5)
		m.write_s16(primitive + 0x20, ox + arg4)
		m.write_s16(primitive + 0x22, oy + arg5 + sy * radius)
		m.write_s16(primitive + 0x2C, ox + arg4 + sx * radius)
		m.write_s16(primitive + 0x2E, oy + arg5 + sy * radius)
		m.write_u8(primitive + 0x0C, 0)
		m.write_u8(primitive + 0x0D, (arg0 & 0x20) * 4)
		m.write_u8(primitive + 0x18, 0x7F)
		m.write_u8(primitive + 0x19, (arg0 & 0x20) * 4)
		m.write_u8(primitive + 0x24, 0)
		m.write_u8(primitive + 0x25, row + 0x7F)
		m.write_u8(primitive + 0x30, 0x7F)
		m.write_u8(primitive + 0x31, row + 0x7F)
		m.write_u16(primitive + 0x1A, engine.call_c("func_8009ECB0", [0, 0, (arg0 & 0xF) << 6, ((arg0 & 0x20) * 4) + ((arg0 & 0x10) * 0x10)]))
		m.write_u16(primitive + 0x0E, engine.call_c("func_8009ED74", [arg1, arg2]))
		engine.call_c("AddPrim", [Math.s32(ot_base + Math.sll32(m.read_global("draw_buffer"), 10) + order * 4), primitive])
		engine.call_c("dtd_on_tpage", [0, 0, order, 1, 0])
	if Math.u8(Math.sra32(arg6, 0x18)) & 0xF:
		for quadrant: int in 4:
			if engine.halted():
				return 0
			var db2: int = m.read_global("draw_buffer")
			var tile: int = Math.u32(engine.call_c("GetWorkBase", [0x10, db2]))
			engine.call_c("func_8009F0F4", [tile])
			engine.call_c("SetSemiTrans", [tile, 0])
			engine.call_c("SetShadeTex", [tile, 0])
			m.write_u8(tile + 4, (Math.srl32(arg6, 0x10) & 0xFFFF) & 0xFF)
			m.write_u8(tile + 5, (Math.sra32(arg6, 8) & 0xFFFF) & 0xFF)
			m.write_u8(tile + 6, arg6 & 0xFFFF & 0xFF)
			db2 = m.read_global("draw_buffer")
			var tox: int = m.s16(engine.gs_address("draw_origin_x") + db2 * 2)
			var toy: int = m.s16(engine.gs_address("draw_origin_y") + db2 * 2)
			var r: int = Math.sll32(arg3, 7) / 4096
			var extent: int
			match quadrant:
				0:
					m.write_s16(tile + 8, tox - 0xA0)
					m.write_s16(tile + 0xA, toy - 0x78)
					m.write_s16(tile + 0xC, 0x140)
					extent = (arg5 - r) + 0x78
					m.write_s16(tile + 0xE, extent)
				1:
					m.write_s16(tile + 8, tox - 0xA0)
					m.write_s16(tile + 0xA, toy - 0x78)
					extent = (arg4 - r) + 0xA0
					m.write_s16(tile + 0xC, extent)
					m.write_s16(tile + 0xE, 0xF0)
				2:
					m.write_s16(tile + 8, tox - 0xA0)
					extent = (-arg5 - r) + 0x78
					m.write_s16(tile + 0xA, toy + arg5 + r)
					m.write_s16(tile + 0xE, extent)
					m.write_s16(tile + 0xC, 0x140)
				_:
					m.write_s16(tile + 8, tox + arg4 + r)
					extent = (-arg4 - r) + 0xA0
					m.write_s16(tile + 0xA, toy - 0x78)
					m.write_s16(tile + 0xC, extent)
					m.write_s16(tile + 0xE, 0xF0)
			if extent > 0:
				engine.call_c("AddPrim", [Math.s32(ot_base + Math.sll32(m.read_global("draw_buffer"), 10) + order * 4), tile])
	return 0
