extends RefCounted
## Port of src/main/gpu_effects.c (original object 0x80049FF0-0x8004AD60, T-39311 on the T-39301 engine).
## Gradient lines, the rectangle fill, the background-layer slot setup, the disc effects (textured tiles and
## glow tiles) and the full-screen flashes. Like the C, every function builds libgpu primitives in RAM and
## links them into draw_ot_tags; the libgpu calls (AddPrim, SetSemiTrans, func_8009F11C SetLineG2,
## func_8009F068 SetPolyGT4, func_8009F0F4 SetTile, func_8009ECB0 GetTPage, func_8009ED74 GetClut) and libc
## (rand, printf) are call_c: no module ports them yet, so each refuses by its name (the differential harness
## replays them from the capture). rsin-free; abs is Math absi. Contract: wiki/engine-porting-contract.md.
## Tests: tests/engine/gpu_effects_test.gd, config/port/engine-diff/gpu_effects.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/gpu_effects.c"
const C_FUNCTIONS: Array[String] = [
	"prim_gradient_line", "prim_fill_rect", "prim_bg_slot_setup", "prim_disc_tiles", "prim_glow_disc",
	"prim_flash_gray", "prim_flash_color_wide",
]
const C_UNPORTED: Array[String] = []
## prim_gradient_line is a K&R function (s16 x0..y1, u32 c0, c1, u8 flags) declared s32 that never returns a
## value. prim_fill_rect takes RECT by value: o32 passes its 8 bytes in a1 (x | y << 16) and a2 (w | h << 16),
## so the function has four argument words (arg0, rect_xy, rect_wh, arg3); "c_params" is the C parameter count
## the signature checker compares.
const C_SIGNATURES: Dictionary = {
	"prim_gradient_line": {"args": ["s16", "s16", "s16", "s16", "u32", "u32", "u8"], "ret": "s32", "ret_undefined": "s32 in the C (K&R) with no return statement: v0 is whatever the register held"},
	"prim_fill_rect": {"args": ["s32", "u32", "u32", "s32"], "ret": "void", "c_params": 3},
	"prim_bg_slot_setup": {"args": ["ptr", "s32", "s32", "s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"prim_disc_tiles": {"args": ["s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"prim_glow_disc": {"args": ["s32", "s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"prim_flash_gray": {"args": ["s32"], "ret": "void"},
	"prim_flash_color_wide": {"args": ["s32"], "ret": "void"},
}

## include/game/layer.h BgLayer (size 0x24), layout of bg_layers (0x801220D0 + slot * 0x24); the generated
## core/game_layout.gd has no BgLayer struct.
const BG_LAYER_SIZE: int = 0x24
## layer_bg_maps (BgMap, size 0x10), layer_bg_cells (BgCell, 64 per slot, size 8), layer_bg_cell_indices
## (BgMapIndex, size 0x80): include/game/layer.h, config/symbol_addrs*.txt.
const BG_MAP_SIZE: int = 0x10
const BG_CELLS_PER_SLOT: int = 64
const BG_CELL_SIZE: int = 8
const BG_INDEX_SIZE: int = 0x80
## "BG_ERROR:p_line.c \n" (asm/data/main/gpu_effects.rodata.s, 0x800AFAE0)
const STR_BG_ERROR: int = 0x800AFAE0

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func _draw_buffer() -> int:
	return engine.memory.read_global("draw_buffer")

func _ot(buffer: int, slot: int) -> int:
	return Math.s32(engine.memory.global_address("draw_ot_tags") + Math.sll32(buffer, 10) + slot * 4)

func _origin_x(buffer: int) -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_x") + buffer * 2)

func _origin_y(buffer: int) -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_y") + buffer * 2)

func prim_gradient_line(x0: int, y0: int, x1: int, y1: int, c0: int, c1: int, flags: int) -> int:
	x0 = Math.s16(x0) # K&R parameters narrow on entry
	y0 = Math.s16(y0)
	x1 = Math.s16(x1)
	y1 = Math.s16(y1)
	c0 = Math.u32(c0)
	c1 = Math.u32(c1)
	flags = Math.u8(flags)
	var m: Object = engine.memory
	var line: int = Math.u32(engine.call_c("GetWorkBase", [0x14, _draw_buffer()]))
	engine.call_c("func_8009F11C", [line])
	if flags & 0x80:
		engine.call_c("SetSemiTrans", [line, 1])
	else:
		engine.call_c("SetSemiTrans", [line, 0])
	m.write_u8(line + 4, (c0 >> 16) & 0xFF)
	m.write_u8(line + 5, (c0 >> 8) & 0xFF)
	m.write_u8(line + 6, c0 & 0xFF)
	m.write_u8(line + 0xC, (c1 >> 16) & 0xFF)
	m.write_u8(line + 0xD, (c1 >> 8) & 0xFF)
	m.write_u8(line + 0xE, c1 & 0xFF)
	var db: int = _draw_buffer()
	m.write_s16(line + 0x8, _origin_x(db) + x0)
	m.write_s16(line + 0xA, _origin_y(db) + y0)
	m.write_s16(line + 0x10, _origin_x(db) + x1)
	m.write_s16(line + 0x12, _origin_y(db) + y1)
	engine.call_c("AddPrim", [_ot(_draw_buffer(), flags & 0x7F), line])
	return 0

## rect: o32 passes RECT {s16 x, y, w, h} in a1 (x low half, y high half) and a2 (w low, h high); the C passes
## each field as a s16-extended word (asm: lh from the spilled argument words).
func prim_fill_rect(arg0: int, rect_xy: int, rect_wh: int, arg3: int) -> void:
	engine.call_c("prim_gradient_rect", [Math.s16(rect_xy), Math.s16(rect_xy >> 16), Math.s16(rect_wh), Math.s16(rect_wh >> 16), 1, arg3, 1])

## Sets BG slot `slot` (bg_layers, layer_bg_maps, layer_bg_cells, layer_bg_cell_indices), uploads each cell
## texture through load_tpage_buf_lock and records the cell tables. Cells past index 0x40 print BG_ERROR.
func prim_bg_slot_setup(tex: int, slot: int, _count: int, w: int, h: int, clut: int, x: int, y: int, flag: int, tp: int) -> void:
	var m: Object = engine.memory
	var cellh: int = 0x78
	var bg: int = m.global_address("bg_layers") + slot * BG_LAYER_SIZE
	var map: int = m.global_address("layer_bg_maps") + slot * BG_MAP_SIZE
	var cells: int = m.global_address("layer_bg_cells") + slot * BG_CELLS_PER_SLOT * BG_CELL_SIZE
	var indices: int = m.global_address("layer_bg_cell_indices") + slot * BG_INDEX_SIZE
	m.write_u32(bg + 0x00, 0x1000000)
	m.write_s16(bg + 0x04, x)
	m.write_s16(bg + 0x06, y)
	m.write_s16(bg + 0x08, Math.sll32(w, 7))
	m.write_s16(bg + 0x0A, Math.s32(h * cellh))
	m.write_s16(bg + 0x0C, 0)
	m.write_s16(bg + 0x0E, 0)
	m.write_u8(bg + 0x10, 0x80)
	m.write_u8(bg + 0x11, 0x80)
	m.write_u8(bg + 0x12, 0x80)
	m.write_u32(bg + 0x14, map)
	m.write_s16(bg + 0x18, 0)
	m.write_s16(bg + 0x1A, 0)
	m.write_s16(bg + 0x1C, 0x1000)
	m.write_s16(bg + 0x1E, 0x1000)
	m.write_u32(bg + 0x20, 0)
	m.write_u8(map + 0, 0x80) # cellw
	m.write_u8(map + 1, cellh)
	m.write_u16(map + 2, w) # ncellw
	m.write_u16(map + 4, h) # ncellh
	m.write_u32(map + 8, cells) # base
	m.write_u32(map + 0xC, indices) # index
	for row: int in maxi(h, 0):
		for col: int in maxi(w, 0):
			if engine.halted():
				return
			engine.call_c("load_tpage_buf_lock", [Math.s32(tp + col + row * 32), m.s32(Math.u32(tex) + (row * w + col) * 4), 1])
	for row: int in maxi(h, 0):
		for col: int in maxi(w, 0):
			if engine.halted():
				return
			var i: int = Math.s32(row * w + col)
			m.write_u16(indices + i * 2, i)
			var cell: int = cells + i * BG_CELL_SIZE
			m.write_u8(cell + 0, 0) # u
			m.write_u8(cell + 1, row * 0x78) # v
			m.write_u16(cell + 6, tp + col) # tpage
			m.write_u16(cell + 2, engine.call_c("func_8009ED74", [0, clut])) # cba
			m.write_u16(cell + 4, 0) # flag
			if i > 0x40:
				engine.call_c("printf", [STR_BG_ERROR])
	m.write_u8(engine.gs_address("bg_layer_order") + slot, flag)

## Textured 16x16 tiles in a disc of radius arg2 around (arg0, arg1), step arg3 x arg4 (each clamped to
## 0x7F, at least 8), into OT slot arg5. Per tile: GetTPage, then two rand() calls for the clut position.
@warning_ignore("integer_division")
func prim_disc_tiles(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int) -> void:
	var m: Object = engine.memory
	arg3 &= 0x7F
	arg4 &= 0x7F
	if arg3 < 8:
		arg3 = 8
	if arg4 < 8:
		arg4 = 8
	var x: int = Math.s32(arg0 - arg2)
	while x < Math.s32(arg0 + arg2):
		if engine.halted():
			return
		var y: int = Math.s32(arg1 - arg2)
		while y < Math.s32(arg1 + arg2): # the C enters this loop through `if (y < ...) do ... while`
			if engine.halted():
				return
			var dx: int = Math.s32(arg0 - x)
			var dy: int = Math.s32(arg1 - y)
			if Math.s32(Math.s32(dx * dx) + Math.s32(dy * dy)) < Math.s32(arg2 * arg2):
				var poly: int = Math.u32(engine.call_c("GetWorkBase", [0x34, _draw_buffer()]))
				engine.call_c("func_8009F068", [poly])
				engine.call_c("SetSemiTrans", [poly, 0])
				var db: int = _draw_buffer()
				var ox: int = _origin_x(db)
				var oy: int = _origin_y(db)
				var sx: int = Math.s16(x)
				var sy: int = Math.s16(y)
				m.write_s16(poly + 0x08, ox + sx - arg3 / 2)
				m.write_s16(poly + 0x0A, oy + sy - arg4 / 2)
				m.write_s16(poly + 0x14, ox + sx - arg3 / 2 + Math.s16(arg3))
				m.write_s16(poly + 0x16, oy + sy - arg4 / 2)
				m.write_s16(poly + 0x20, ox + sx - arg3 / 2)
				m.write_s16(poly + 0x22, oy + sy - arg4 / 2 + Math.s16(arg4))
				m.write_s16(poly + 0x2C, ox + sx - arg3 / 2 + Math.s16(arg3))
				m.write_s16(poly + 0x2E, oy + sy - arg4 / 2 + Math.s16(arg4))
				m.write_u8(poly + 0x0C, 0xC0) # u0, v0
				m.write_u8(poly + 0x0D, 0x50)
				m.write_u8(poly + 0x18, 0xC0 + 0x10) # u1, v1
				m.write_u8(poly + 0x19, 0x50)
				m.write_u8(poly + 0x24, 0xC0) # u2, v2
				m.write_u8(poly + 0x25, 0x50 + 0x10)
				m.write_u8(poly + 0x30, 0xC0 + 0x10) # u3, v3
				m.write_u8(poly + 0x31, 0x50 + 0x10)
				for corner: int in [0x04, 0x10, 0x1C, 0x28]: # r, g, b of the four vertices
					m.write_u8(poly + corner, 0x80)
					m.write_u8(poly + corner + 1, 0x80)
					m.write_u8(poly + corner + 2, 0x80)
				m.write_u16(poly + 0x1A, engine.call_c("func_8009ECB0", [0, 0, 0x3C0, 0x100])) # tpage
				var rx: int = engine.call_c("rand") & 8
				var ry: int = engine.call_c("rand") & 8
				db = _draw_buffer()
				m.write_u16(poly + 0x0E, engine.call_c("func_8009ED74", [Math.s32(_origin_x(db) + sx + rx), Math.s32(sy + _origin_y(db) + ry)])) # clut
				engine.call_c("AddPrim", [_ot(_draw_buffer(), arg5), poly])
			y = Math.s32(y + arg4)
		x = Math.s32(x + arg3)

## Glow tiles (TILE, 0x10 bytes) in a disc, colour fading with the distance from the centre; x step for both
## tile sides (setWH(tile, arg3, arg3)), then dtd_on_tpage(0, 0, arg7, arg6 & 3, 0).
@warning_ignore("integer_division")
func prim_glow_disc(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int, arg7: int) -> void:
	var m: Object = engine.memory
	var red: int = (Math.u32(arg5 & 0xFF0000)) >> 16
	var green: int = (Math.u32(arg5 & 0xFF00)) >> 8
	var blue: int = arg5 & 0xFF
	arg3 &= 0x7F
	arg4 &= 0x7F
	if arg3 < 2:
		arg3 = 2
	if arg4 < 2:
		arg4 = 2
	var x: int = Math.s32(arg0 - arg2)
	while x < Math.s32(arg0 + arg2):
		if engine.halted():
			return
		var y: int = Math.s32(arg1 - arg2)
		while y < Math.s32(arg1 + arg2):
			if engine.halted():
				return
			var dx: int = Math.s32(arg0 - x)
			var dy: int = Math.s32(arg1 - y)
			if Math.s32(Math.s32(dx * dx) + Math.s32(dy * dy)) < Math.s32(arg2 * arg2):
				var tile: int = Math.u32(engine.call_c("GetWorkBase", [0x10, _draw_buffer()]))
				engine.call_c("func_8009F0F4", [tile])
				engine.call_c("SetSemiTrans", [tile, 1])
				var db: int = _draw_buffer()
				m.write_s16(tile + 0x8, _origin_x(db) + Math.s16(x) - arg3 / 2)
				m.write_s16(tile + 0xA, _origin_y(db) + Math.s16(y) - arg4 / 2)
				m.write_s16(tile + 0xC, arg3)
				m.write_s16(tile + 0xE, arg3)
				var abs_x: int = absi(Math.s32(arg0 - x))
				var abs_y: int = absi(Math.s32(arg1 - y))
				# red and green: (s32)(sum * (u32 colour)) / arg2 (u32 product, signed division); blue is s32 * s32
				var r: int = Math.s32(Math.s32((Math.s32(arg2 * 2 - abs_y - abs_x) * red) & 0xFFFFFFFF)) / arg2
				var g: int = Math.s32(Math.s32((Math.s32(arg2 * 2 - abs_y - abs_x) * green) & 0xFFFFFFFF)) / arg2
				var b: int = Math.s32(Math.s32(arg2 * 2 - abs_y - abs_x) * blue) / arg2
				if r >= 256:
					r = 255
				if g >= 256:
					g = 255
				if b >= 256:
					b = 255
				m.write_u8(tile + 4, r)
				m.write_u8(tile + 5, g)
				m.write_u8(tile + 6, b)
				engine.call_c("AddPrim", [_ot(_draw_buffer(), arg7), tile])
			y = Math.s32(y + arg4)
		x = Math.s32(x + arg3)
	engine.call_c("dtd_on_tpage", [0, 0, arg7, arg6 & 3, 0])

## Grey flash: level * 0x10101 below 0x100, white above; two half-screen boxes and a draw mode.
func prim_flash_gray(arg0: int) -> void:
	var level: int = arg0
	var color: int
	if level < 0x100:
		color = Math.s32(level * 0x10101)
	else:
		color = 0xFFFFFF
	engine.call_c("prim_gradient_rect", [-0xA0, -0x78, 0xA0, 0xF0, 1, color, 0x81])
	engine.call_c("prim_gradient_rect", [0, -0x78, 0xA0, 0xF0, 1, color, 0x81])
	engine.call_c("dtd_on_tpage", [0, 0, 1, 1, 1])

func prim_flash_color_wide(arg0: int) -> void:
	engine.call_c("prim_gradient_rect", [-0x100, -0x78, 0x100, 0xF0, 1, arg0, 0x81])
	engine.call_c("prim_gradient_rect", [0, -0x78, 0x100, 0xF0, 1, arg0, 0x81])
	engine.call_c("dtd_on_tpage", [0, 0, 1, 1, 1])
