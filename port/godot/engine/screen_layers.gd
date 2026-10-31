extends RefCounted
## Port of src/main/screen_layers.c (original object 0x8004C3F0-0x8004E500, T-39311 on the T-39301 engine).
## Screen-page rendering (srn_tpage_show), the SRN layer VRAM upload and draw (srn_vram_set, srn_set, srn_init),
## the MOD tile-set upload and draw (mod_trans_vram, mod_set_gpu, mod_set, layer_tileset_init), the glyph fetch
## (get_fnt) and move_255_line. Like the C, drawing builds libgpu primitives in RAM and links them into
## draw_ot_tags; libgpu/libgte/VRAM calls (AddPrim, SetSemiTrans, SetShadeTex, func_8009F040, func_8009F07C,
## func_8009ECB0, func_8009ED74, func_8009C674, func_80098B80, LoadSquare, MoveSquare) and other C files'
## functions (draw_brightness_get, search_tpage_multi, printf) are call_c, so each refuses by name until its
## owner is ported (the differential harness replays them from the capture). Contract: wiki/engine-porting-contract.md.
## Tests: tests/engine/screen_layers_test.gd, config/port/engine-diff/screen_layers.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/screen_layers.c"
const C_FUNCTIONS: Array[String] = [
	"srn_tpage_show", "srn_vram_set", "move_255_line", "mod_trans_vram", "get_fnt", "func_8004DAC4", "mod_set_gpu",
	"mod_set", "srn_set", "srn_init", "layer_tileset_init",
]
const C_UNPORTED: Array[String] = []
## srn_vram_set returns -1 on its error path only and falls off the end otherwise; srn_set returns -1 or 0 on
## its explicit paths and falls off the end after the last row; get_fnt is declared s32 and never returns
## (the FAKE of the C). search_tpage_multi (not in this file) is viewed as u16 here.
const C_SIGNATURES: Dictionary = {
	"srn_tpage_show": {"args": ["u8", "u8", "u8", "ptr", "s32", "s32", "s16", "s16", "s32"], "ret": "void"},
	"srn_vram_set": {"args": ["s32", "s32", "s32", "s32"], "ret": "s32", "ret_defined": [-1]},
	"move_255_line": {"args": ["s32", "s32", "s32"], "ret": "void"},
	"mod_trans_vram": {"args": ["s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"get_fnt": {"args": ["ptr", "ptr", "s32", "s32", "s32", "u8"], "ret": "s32", "ret_undefined": "s32 in the C (FAKE, T-24601) with no return statement: callers discard v0"},
	"func_8004DAC4": {"args": ["s32"], "ret": "void"},
	"mod_set_gpu": {"args": ["s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"mod_set": {"args": ["s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"srn_set": {"args": ["s32"], "ret": "s32", "ret_defined": [-1, 0]},
	"srn_init": {"args": ["s32", "ptr", "s32"], "ret": "void"},
	"layer_tileset_init": {"args": ["s32", "ptr", "s32"], "ret": "void"},
}

## layer_records: SrnLayer, 0x28 bytes each (include/game/layer.h); layer_tileset_records: ModTileSet, 0x14 bytes.
const SRN_SIZE: int = 0x28
const MOD_SIZE: int = 0x14
## "ERROR" (asm/data/main/screen_layers.rodata.s, 0x800AFB00)
const STR_ERROR: int = 0x800AFB00
## Original stack frame offsets (asm/matchings/main/screen_layers/): mod_trans_vram buf (0x88), get_fnt
## pixels (0x8C), mod_set sp (0xB0).
const MOD_TRANS_VRAM_BUF: int = 0x88
const GET_FNT_PIXELS: int = 0x8C
const MOD_SET_QUAD: int = 0xB0

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func _db() -> int:
	return engine.memory.read_global("draw_buffer")

func _ot(buffer: int, slot: int) -> int:
	return Math.s32(engine.memory.global_address("draw_ot_tags") + Math.sll32(buffer, 10) + slot * 4)

func _ox(buffer: int) -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_x") + buffer * 2)

func _oy(buffer: int) -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_y") + buffer * 2)

func _records() -> int:
	return engine.memory.global_address("layer_records")

func _tilesets() -> int:
	return engine.memory.global_address("layer_tileset_records")

## One FT4 quad of srn_tpage_show: allocate, SetPolyFT4, semi-trans flag, SetShadeTex, brightness, uv (8 bytes:
## u0 v0 u1 v1 u2 v2 u3 v3), xy (x0 y0 x1 y1 x2 y2 x3 y3), GetTPage, GetClut, AddPrim at `slot` of the buffer.
func _show_quad(semi: int, uv: Array, xy: Array, tpage_args: Array, arg6: int, arg7: int, slot: int) -> void:
	var m: Object = engine.memory
	var poly: int = Math.u32(engine.call_c("GetWorkBase", [0x28, _db()]))
	engine.call_c("func_8009F040", [poly])
	if semi != 0:
		engine.call_c("SetSemiTrans", [poly, 1])
	else:
		engine.call_c("SetSemiTrans", [poly, 0])
	engine.call_c("SetShadeTex", [poly, 0])
	var bright: int = engine.gs_get("screen_brightness")
	m.write_u8(poly + 4, bright)
	m.write_u8(poly + 5, bright)
	m.write_u8(poly + 6, bright)
	var uv_at: Array = [0xC, 0xD, 0x14, 0x15, 0x1C, 0x1D, 0x24, 0x25]
	for i: int in 8:
		m.write_u8(poly + uv_at[i], uv[i])
	var xy_at: Array = [0x8, 0xA, 0x10, 0x12, 0x18, 0x1A, 0x20, 0x22]
	for i: int in 8:
		m.write_s16(poly + xy_at[i], xy[i])
	if tpage_args.is_empty():
		# the tpage argument uses the C's uninitialised divisor / pageWidth (colour mode outside 0..2): refuse at the
		# first use, after the work area, SetPolyFT4, SetShadeTex and the stores ran like in the original
		engine.refuse("srn_tpage_show", "colour mode outside 0..2: the C leaves divisor and pageWidth uninitialised")
		return
	m.write_u16(poly + 0x16, engine.call_c("func_8009ECB0", tpage_args))
	m.write_u16(poly + 0xE, engine.call_c("func_8009ED74", [arg6, arg7]))
	engine.call_c("AddPrim", [slot, poly])

## Five FT4 quads: three 256-wide pages at x, x + 0x100, x + 0x200 (scrolled left by arg4 & 0xFF) and the two
## 1-pixel columns between them (the first column goes to OT entry arg8 - 1). arg1 is the colour mode (0, 1, 2:
## divisor 4, 2, 1 and page width 0x40, 0x80, 0x100); any other value leaves the C's divisor and pageWidth
## uninitialised, which the port refuses instead of inventing. arg5 is unused.
@warning_ignore("integer_division")
func srn_tpage_show(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int, arg7: int, arg8: int) -> void:
	arg0 = Math.u8(arg0) # u8 parameters of the prototype
	arg1 = Math.u8(arg1)
	arg2 = Math.u8(arg2)
	arg6 = Math.s16(arg6)
	arg7 = Math.s16(arg7)
	var divisor: int
	var page_width: int
	var valid: bool = true
	match arg1:
		0:
			divisor = 4
			page_width = 0x40
		1:
			divisor = 2
			page_width = 0x80
		2:
			divisor = 1
			page_width = 0x100
		_:
			divisor = 0
			page_width = 0
			valid = false
	var m: Object = engine.memory
	var rx: int = m.s16(arg3 + 0)
	var ry: int = m.s16(arg3 + 2)
	var h: int = m.s16(arg3 + 6)
	var v: int = (arg0 & 0x20) * 4
	var scroll: int = arg4 & 0xFF
	var page_x: int = ((arg0 & 0xF) << 6) + (arg4 & 0x7F00) / divisor if valid else 0
	var page_y: int = (arg0 & 0x10) * 0x10 + (arg0 & 0x20) * 4
	var col_u: int = arg4 / 256 * 4
	var tp_low: int = arg2 & 3
	# left page
	var ox: int = _ox(_db())
	var oy: int = _oy(_db())
	_show_quad(arg2, [0, v, 0xFF, v, 0, v + h, 0xFF, v + h],
		[ox - scroll + rx, oy + ry, ox - scroll + rx + 0xFF, oy + ry, ox - scroll + rx, oy + ry + h, ox - scroll + rx + 0xFF, oy + ry + h],
		[arg1, tp_low, page_x, page_y] if valid else [], arg6, arg7, _ot(_db(), arg8))
	if engine.halted():
		return
	# 1-pixel column after the left page
	ox = _ox(_db())
	oy = _oy(_db())
	_show_quad(arg2, [col_u + 3, 0, col_u + 4, 0, col_u + 3, h, col_u + 4, h],
		[ox + (0xFF - scroll) + rx, oy + ry, ox + (0xFF - scroll) + rx + 1, oy + ry, ox + (0xFF - scroll) + rx, oy + ry + h, ox - scroll + rx + 0x100, oy + ry + h],
		[arg1, tp_low, 0x3C0, 0], arg6, arg7, _ot(_db(), arg8) - 4)
	if engine.halted():
		return
	# middle page
	ox = _ox(_db())
	oy = _oy(_db())
	_show_quad(arg2, [0, v, 0xFF, v, 0, v + h, 0xFF, v + h],
		[ox - scroll + rx + 0x100, oy + ry, ox - scroll + rx + 0x1FF, oy + ry, ox - scroll + rx + 0x100, oy + ry + h, ox - scroll + rx + 0x1FF, oy + ry + h],
		[arg1, tp_low, page_x + page_width, page_y], arg6, arg7, _ot(_db(), arg8))
	if engine.halted():
		return
	# 1-pixel column after the middle page
	ox = _ox(_db())
	oy = _oy(_db())
	_show_quad(arg2, [col_u + 7, 0, col_u + 8, 0, col_u + 7, h, col_u + 8, h],
		[ox + (0xFF - scroll) + rx + 0x100, oy + ry, ox + (0xFF - scroll) + rx + 0x101, oy + ry, ox + (0xFF - scroll) + rx + 0x100, oy + ry + h, ox - scroll + rx + 0x200, oy + ry + h],
		[arg1, tp_low, 0x3C0, 0], arg6, arg7, _ot(_db(), arg8))
	if engine.halted():
		return
	# right page
	ox = _ox(_db())
	oy = _oy(_db())
	_show_quad(arg2, [0, v, 0xFF, v, 0, v + h, 0xFF, v + h],
		[ox - scroll + rx + 0x200, oy + ry, ox - scroll + rx + 0x2FF, oy + ry, ox - scroll + rx + 0x200, oy + ry + h, ox - scroll + rx + 0x2FF, oy + ry + h],
		[arg1, tp_low, page_x + page_width * 2, page_y], arg6, arg7, _ot(_db(), arg8))

## Uploads the nonzero tiles of screen-layer record arg0 to VRAM page arg1 through mod_trans_vram (tiles whose
## position is inside arg2 x arg3). The error path (a zero cell size) prints "ERROR" and returns -1; the normal
## path falls off the end (v0 undefined).
@warning_ignore("integer_division")
func srn_vram_set(arg0: int, arg1: int, arg2: int, arg3: int) -> int:
	var m: Object = engine.memory
	engine.call_c("func_8009C674", [0])
	var rec: int = Math.u32(_records() + arg0 * SRN_SIZE)
	var w0: int = m.u8(rec + 0xE)
	var h0: int = m.u8(rec + 0xF)
	var tw: int = m.u8(rec + 0x10)
	var th: int = m.u8(rec + 0x11)
	var ox: int = m.u8(rec + 0x12)
	var oy: int = m.u8(rec + 0x13)
	var cw: int = m.u8(rec + 0x14)
	var ch: int = m.u8(rec + 0x15)
	var src: int = m.u32(rec + 8)
	if h0 == 0 or w0 == 0:
		engine.call_c("printf", [STR_ERROR])
		return -1
	var left: int = m.s32(rec + 0x1C)
	var row: int = 0
	var cols: int = cw * tw / w0
	var rows: int = ch * th / h0
	if rows > 0:
		while true: # do { ... row++; } while (row < rows)
			if engine.halted():
				return 0
			var col: int = 0
			if cols > 0:
				while true: # do { ... col++; } while (col < cols)
					if engine.halted():
						return 0
					if Math.s32((col - 1) * tw + ox * w0) < arg2 and Math.s32((row - 1) * th + oy * h0) < arg3:
						if m.u32(src) & 0xFFFF != 0:
							engine.call_c("mod_trans_vram", [0, m.u32(src) & 0xFFFF, arg1, tw * col, th * row])
					left = Math.s32(left - 1)
					src = Math.u32(src + 4)
					if left == 0:
						row = rows
						col = cols
					col += 1
					if col >= cols:
						break
			row += 1
			if row >= rows:
				break
	if m.s8(m.global_address("D_800B3DB0")) != 0:
		engine.call_c("move_255_line", [arg1, Math.s32(arg1 + 0xB), 0xF])
	return 0

## Copies the rightmost column of each of the arg1 - arg0 + 1 source texture pages (i * 0x40 + 0x3F, 0, 1 x 0xFF)
## into the destination pages at ((arg2 & 0xF) << 6) + i - arg0, ((arg2 & 0x10) << 4).
func move_255_line(arg0: int, arg1: int, arg2: int) -> void:
	var i: int = arg0
	while i <= arg1:
		if engine.halted():
			return
		engine.call_c("MoveSquare", [Math.u16(i * 0x40 + 0x3F), 0, 1, 0xFF, ((arg2 & 0xF) << 6) + i - arg0, Math.u16((arg2 & 0x10) * 0x10)])
		i += 1

## Uploads frame `frame` of tile-set record idx: per tile word (glyph number in bits 16-23, source bank in
## bits 24-31, get_fnt flags in bits 0-7) get_fnt fills buf (the frame's 256-byte local at +0x88) and LoadSquare
## copies it into page `page` at x / 2 + column offset and y + row offset. The C is a K&R-promotion build
## (-cckr): `xh / ratio` and `... / 2 / ratio` divide by the u8 ratio as unsigned int.
@warning_ignore("integer_division")
func mod_trans_vram(idx: int, frame: int, page: int, x: int, y: int) -> void:
	var m: Object = engine.memory
	var buf: int = engine.frame_base() + MOD_TRANS_VRAM_BUF
	var p: int = Math.u32(_tilesets() + idx * MOD_SIZE)
	var size: int = 8 if m.u8(p + 0xE) == 8 else 16
	var ratio: int
	match m.u8(p + 8):
		1, 2:
			ratio = 2
		_:
			ratio = 1
	var xh: int = x / 2
	var rows: int = m.u8(p + 0xD)
	var cols: int = m.u8(p + 0xC)
	var data: int = Math.u32(Math.s32(rows * frame * cols) * 4 + m.u32(p + 4))
	var row: int = 0
	if rows != 0:
		while true: # do { ... row++; } while (row < rows)
			if engine.halted():
				return
			var col: int = 0
			if cols != 0:
				var ypos: int = Math.s32(((page & 0x10) << 4) + ((page & 0x20) << 2) + y + row * size)
				while true: # do { ... col++; } while (col < cols)
					if engine.halted():
						return
					var word: int = m.u32(data)
					var uv: int = (word & 0xFF0000) >> 16
					var bank: int = (word & 0xFF000000) >> 24
					var flags: int = word & 0xFF
					engine.call_c("get_fnt", [buf, m.u32(m.u32(p) + bank * 4), uv, ratio, size, flags])
					var dest_x: int = Math.s32(((page & 0xF) << 6) + Math.u32(xh) / ratio + Math.u32(col * size / 2) / ratio)
					engine.call_c("LoadSquare", [dest_x, ypos, Math.u32(size / 2) / ratio, size, buf])
					data = Math.u32(data + 4)
					col += 1
					cols = m.u8(p + 0xC)
					if col >= cols:
						break
				rows = m.u8(p + 0xD)
			row += 1
			if row >= rows:
				break

## Glyph `glyph` of the font atlas at `source` (128 bytes per row, width x width cells, `divisor` pixels per
## byte) into `output`, row by row through memcpy, then flag 1 flips the rows and flag 2 mirrors the 8 bytes of
## each row (the 256-byte copy `pixels` lives at +0x8C of the C frame). A zero tile count (width > 0x80) or
## divisor is a division by zero in the original: refused.
@warning_ignore("integer_division")
func get_fnt(output: int, source: int, glyph: int, divisor: int, width: int, flags: int) -> int:
	var m: Object = engine.memory
	var pixels: int = engine.frame_base() + GET_FNT_PIXELS
	flags = Math.u8(flags)
	var row: int = 0
	if width > 0:
		var tiles_per_row: int = 0x80 / width
		if tiles_per_row == 0 or divisor == 0:
			engine.refuse("get_fnt", "division by zero (tiles per row %d, divisor %d)" % [tiles_per_row, divisor])
			return 0
		var row_offset: int = 0
		while true: # do { ... } while (row != width)
			if engine.halted():
				return 0
			var dst: int = Math.u32(row_offset / divisor + output)
			var src: int = Math.u32(Math.s32(Math.s32((glyph % tiles_per_row) * width) / divisor) + source + Math.s32(Math.sll32(Math.s32((glyph / tiles_per_row) * width + row), 7) / divisor))
			engine.call_c("memcpy", [dst, src, width / divisor])
			row += 1
			row_offset += width
			if row == width:
				break
	if flags & 1:
		engine.call_c("memcpy", [pixels, output, 0x100])
		var mirror_row: int = 0
		while mirror_row < width:
			if engine.halted():
				return 0
			engine.call_c("memcpy", [output + (width - mirror_row - 1) * width, pixels + mirror_row * width, width])
			mirror_row += 1
	if flags & 2:
		engine.call_c("memcpy", [pixels, output, 0x100])
		var r: int = 0
		while r < width:
			if engine.halted():
				return 0
			var b: int = width * r
			m.write_u8(output + b + 4, m.u8(pixels + b + 3))
			m.write_u8(output + b + 5, m.u8(pixels + b + 2))
			m.write_u8(output + b + 6, m.u8(pixels + b + 1))
			m.write_u8(output + b + 7, m.u8(pixels + b))
			m.write_u8(output + b, m.u8(pixels + b + 7))
			m.write_u8(output + b + 1, m.u8(pixels + b + 6))
			m.write_u8(output + b + 2, m.u8(pixels + b + 5))
			m.write_u8(output + b + 3, m.u8(pixels + b + 4))
			r += 1
	return 0

## `s32 *p = &arg0;`: only the argument spill into the caller's home area (dead after the return, excluded by the
## harness); nothing observable, so nothing is written.
func func_8004DAC4(_arg0: int) -> void:
	pass

## Mode (1 or 2 pixels per byte) of a tile-set record's byte at +8: 2 for 1 and 2, else 1.
func _tileset_mode(p: int) -> int:
	match engine.memory.u8(p + 8):
		1, 2:
			return 2
		_:
			return 1

## One frame of tile-set record idx as 8x8 SPRT_8 sprites at (x + 0xA0 + col * 8, y + 0x168 + row * 8 - buffer *
## 0xF0), with a goto_tpage whenever the texture page changes (page of the previous tile) and a final one.
func mod_set_gpu(idx: int, frame: int, x: int, y: int, cx: int, cy: int, pri: int) -> void:
	var m: Object = engine.memory
	var bright: int = engine.call_c("draw_brightness_get")
	var p: int = Math.u32(_tilesets() + idx * MOD_SIZE)
	var mode: int = _tileset_mode(p)
	var last: int = 0xFFFF
	var data: int = Math.u32(Math.s32(m.u8(p + 0xD) * frame * m.u8(p + 0xC)) * 4 + m.u32(p + 4))
	var row: int = 0
	while row < m.u8(p + 0xD):
		var col: int = 0
		while col < m.u8(p + 0xC):
			if engine.halted():
				return
			var word: int = m.u32(data)
			var uv: int = (word & 0xFF0000) >> 16
			var page: int = (word & 0xFF000000) >> 24
			var clut: int = (word & 0xFF00) >> 8
			var s: int = Math.u32(engine.call_c("GetWorkBase", [0x10, _db()]))
			engine.call_c("func_8009F07C", [s])
			engine.call_c("SetSemiTrans", [s, 0])
			var xpos: int = Math.s32(x + 0xA0 + col * 8)
			m.write_s16(s + 8, xpos)
			var ypos: int = Math.s32(row * 8 + y + 0x168)
			m.write_s16(s + 0xA, Math.s32(ypos - _db() * 0xF0))
			m.write_u8(s + 0xC, (uv & 0xF) * 8)
			m.write_u8(s + 0xD, ((uv & 0xF0) >> 4) * 8)
			m.write_u8(s + 6, bright)
			m.write_u8(s + 5, bright)
			m.write_u8(s + 4, bright)
			var tpage: int = engine.call_c("search_tpage_multi", [m.u32(m.u32(p) + page * 4), mode]) & 0xFFFF
			if last != tpage:
				if last != 0xFFFF:
					engine.call_c("goto_tpage", [(last & 0xF) << 6, (last & 0x10) << 4, Math.s32(pri + 1), 0, 0])
				last = tpage
			m.write_u16(s + 0xE, engine.call_c("func_8009ED74", [Math.s32(cx + clut * 16), cy]))
			engine.call_c("AddPrim", [Math.s32(m.global_address("draw_ot_tags") + Math.sll32(_db(), 10) + pri * 4 + 4), s])
			col += 1
			data = Math.u32(data + 4)
		row += 1
	engine.call_c("goto_tpage", [(last & 0xF) << 6, (last & 0x10) << 4, Math.s32(pri + 1), 0, 0])

## One frame of tile-set record idx through the sprite renderer: a SpriteQuad (local at +0xB0 of the frame) per
## tile, handed to func_80098B80 (GsSortSprite) with the u16 priority. The quad's pad byte at +0x17 is never
## written, so the stack byte stays as it was.
func mod_set(idx: int, frame: int, x: int, y: int, cx: int, cy: int, pri: int) -> void:
	var m: Object = engine.memory
	var sp: int = engine.frame_base() + MOD_SET_QUAD
	var bright: int = engine.call_c("draw_brightness_get")
	var p: int = Math.u32(_tilesets() + idx * MOD_SIZE)
	var mode: int = _tileset_mode(p)
	m.write_s32(sp + 0x00, 0 if mode == 2 else 0x01000000) # flags: case 1, 2 -> 0; the others 0x01000000
	var rows: int = m.u8(p + 0xD)
	var cols: int = m.u8(p + 0xC)
	var w: int = m.u8(p + 0xE)
	m.write_u16(sp + 0xA, m.u8(p + 0xF)) # sp.h
	m.write_s16(sp + 0x18, 0) # anchor_x
	m.write_s16(sp + 0x1A, 0) # anchor_y
	m.write_u8(sp + 0x14, bright)
	m.write_u8(sp + 0x15, bright)
	m.write_u8(sp + 0x16, bright)
	m.write_s32(sp + 0x20, 0)
	m.write_s16(sp + 0x1C, 0x1000)
	m.write_s16(sp + 0x1E, 0x1000)
	var row: int = 0
	m.write_s16(sp + 0x8, w) # sp.w
	var data: int = Math.u32(Math.s32(rows * frame * cols) * 4 + m.u32(p + 4))
	if rows != 0:
		while true: # do { ... row++; } while (row < rows)
			if engine.halted():
				return
			var col: int = 0
			if cols != 0:
				while true: # do { ... col++; } while (col < cols)
					if engine.halted():
						return
					m.write_s16(sp + 0x4, Math.s32(x + col * w))
					m.write_s16(sp + 0x6, Math.s32(row * w + y))
					var word: int = m.u32(data)
					var uv: int = (word & 0xFF0000) >> 16
					var bank: int = (word & 0xFF000000) >> 24
					var clut: int = (word & 0xFF00) >> 8
					m.write_s16(sp + 0xC, engine.call_c("search_tpage_multi", [m.u32(m.u32(p) + bank * 4), mode]))
					match w:
						16:
							m.write_u8(sp + 0xE, (uv & 7) * 16)
							m.write_u8(sp + 0xF, ((uv & 0xF8) >> 4) * 16)
						_: # 8 and every other width
							m.write_u8(sp + 0xE, (uv & 0xF) * 8)
							m.write_u8(sp + 0xF, ((uv & 0xF0) >> 4) * 8)
					m.write_s16(sp + 0x10, Math.s32(cx + clut * 16))
					m.write_s16(sp + 0x12, cy)
					engine.call_c("func_80098B80", [sp, Math.s32(m.global_address("draw_ot_table") + _db() * 0x14), Math.u16(pri)])
					col += 1
					data = Math.u32(data + 4)
					cols = m.u8(p + 0xC)
					if col >= cols:
						break
				rows = m.u8(p + 0xD)
			row += 1
			if row >= rows:
				break

## Draws the visible tiles of screen-layer record arg0 (through mod_set_gpu with tile-set record 0, the cell
## index in the table word's low half, and layer record 0's byte +0xC as priority). Returns -1 for a zero cell
## size, 0 when the last tile or the screen bottom is reached; after the last row it falls off the end.
@warning_ignore("integer_division")
func srn_set(arg0: int) -> int:
	var m: Object = engine.memory
	var rec: int = Math.u32(_records() + arg0 * SRN_SIZE)
	var w0: int = m.u8(rec + 0xE)
	var h0: int = m.u8(rec + 0xF)
	var tw: int = m.u8(rec + 0x10)
	var th: int = m.u8(rec + 0x11)
	var ox: int = m.u8(rec + 0x12)
	var oy: int = m.u8(rec + 0x13)
	var cw: int = m.u8(rec + 0x14)
	var ch: int = m.u8(rec + 0x15)
	var xoff: int = m.s16(rec + 2)
	var yoff: int = m.s16(rec + 6)
	var src: int = m.u32(rec + 8)
	if h0 == 0 or w0 == 0:
		return -1
	var cols: int = cw * tw / w0
	var rows: int = ch * th / h0
	var wid: int = m.u16(rec + 0x16)
	var hgt: int = m.u16(rec + 0x18)
	var left: int = m.s32(rec + 0x1C)
	var row: int = 0
	while row < rows:
		if engine.halted():
			return 0
		if th * row + yoff + h0 * oy >= -0x77:
			var col: int = 0
			while col < cols:
				if engine.halted():
					return 0
				if th * row + yoff >= 0x79:
					return 0
				var x: int = tw * col + xoff
				if tw * col + xoff + ox * w0 >= -0x9F:
					if m.u32(src) & 0xFFFF != 0:
						var idx: int = m.u32(src) & 0xFFFF
						engine.call_c("mod_set_gpu", [0, idx, x, th * row + yoff, wid, hgt, m.u8(_records() + 0xC)])
				left = Math.s32(left - 1)
				src = Math.u32(src + 4)
				if left == 0:
					return 0
				col += 1
		row += 1
	return 0

## Fills screen-layer record arg0 from the descriptor words at arg1 (arg2 selects the tile-set record).
func srn_init(arg0: int, arg1: int, arg2: int) -> void:
	var m: Object = engine.memory
	var p: int = Math.u32(_records() + arg0 * SRN_SIZE)
	var w0: int = m.u32(arg1)
	var w1: int = m.u32(arg1 + 4)
	var w2: int = m.u32(arg1 + 8)
	m.write_u8(p + 0xD, (w0 & 0xFF00) >> 8)
	m.write_s16(p + 2, 0)
	m.write_s16(p + 6, 0)
	m.write_u8(p + 0xC, 0)
	m.write_u32(p + 8, arg1 + 5 * 4)
	m.write_u8(p + 0xE, (w1 & 0xFF000000) >> 24)
	m.write_u8(p + 0xF, (w1 & 0xFF0000) >> 16)
	m.write_u8(p + 0x10, (w1 & 0xFF00) >> 8)
	m.write_u8(p + 0x11, w1 & 0xFF)
	m.write_u8(p + 0x12, (w2 & 0xFF000000) >> 24)
	m.write_u8(p + 0x13, (w2 & 0xFF0000) >> 16)
	m.write_u8(p + 0x14, (w2 & 0xFF00) >> 8)
	m.write_u8(p + 0x15, w2 & 0xFF)
	m.write_s16(p + 0x16, 0)
	m.write_s16(p + 0x18, 0x1E0)
	m.write_u32(p + 0x1C, m.u32(arg1 + 4 * 4))
	m.write_u32(p + 0x20, m.u32(arg1 + 3 * 4))
	m.write_u32(p + 0x24, _tilesets() + arg2 * MOD_SIZE)

## Fills tile-set record arg0 from the descriptor words at arg1 (arg2: the bank-pointer table).
func layer_tileset_init(arg0: int, arg1: int, arg2: int) -> void:
	var m: Object = engine.memory
	var p: int = Math.u32(_tilesets() + arg0 * MOD_SIZE)
	var w0: int = m.u32(arg1)
	var w1: int = m.u32(arg1 + 4)
	m.write_s32(p, arg2)
	m.write_u32(p + 4, arg1 + 4 * 4)
	m.write_u8(p + 8, (w0 & 0xFF000000) >> 24)
	m.write_s16(p + 0xA, (w0 & 0xFFFF) >> 24) # always 0: the mask leaves 16 bits
	m.write_u8(p + 0xC, (w1 & 0xFF000000) >> 24)
	m.write_u8(p + 0xD, (w1 & 0xFF0000) >> 16)
	m.write_u8(p + 0xE, (w1 & 0xFF00) >> 8)
	m.write_u8(p + 0xF, w1 & 0xFF)
	m.write_s16(p + 0x10, m.u8(arg1 + 8))
	m.write_s16(p + 0x12, m.u8(arg1 + 0xC))
