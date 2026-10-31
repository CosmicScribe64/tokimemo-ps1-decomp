extends RefCounted
## Port of src/main/gpu_primitives.c (original object 0x800490C0-0x80049FF0, T-39311 on the T-39301 engine).
## Primitive work-area allocation (SetWorkBase/GetWorkBase), the FT4 sprite primitive, the tpage/draw-mode
## and one-pixel-tile primitives, the pulsing light triangle and the Gouraud box. The C builds libgpu
## primitives in RAM and links them into the ordering tables (draw_ot_tags); the port does exactly that:
## every libgpu call (AddPrim, SetSemiTrans, SetShadeTex, func_8009F040 SetPolyFT4, func_8009F004 SetPolyG3,
## func_8009F054 SetPolyG4, func_8009F0B8 SetTile, func_8009D294 SetDrawMode, func_8009ECB0 GetTPage,
## func_8009ED74 GetClut) is a call_c: no module ports libgpu/libgte yet, so each refuses by its
## name (the differential harness replays them from the capture). rsin and abs are pure leaf functions that
## core/native_math.gd already ports from the asm (Math.rsin; abs is absi), so they run inline. Contract: wiki/engine-porting-contract.md.
## Tests: tests/engine/gpu_primitives_test.gd (hand checks), config/port/engine-diff/gpu_primitives.json.

const Math = preload("res://core/native_math.gd")
## rsin and abs are libgte/libc leaf functions called in the C. They are NOT routed through call_c: they are
## pure (no memory, no state), core/native_math.gd already ports rsin from asm/libgte_8009FA5C.s (Math.rsin,
## checked by its own tests and the taco-tables witness) and abs is absi. A call_c would only refuse until an SDK
## module lists them; this is the one deliberate deviation from "every call goes through call_c".
const Layout = preload("res://core/game_layout.gd")

const C_FILE: String = "src/main/gpu_primitives.c"
const C_FUNCTIONS: Array[String] = [
	"SetWorkBase", "GetWorkBase", "sprite_set_gpu_poly_ft4", "goto_tpage", "safe_env", "dtd_on_tpage", "dtd_on",
	"_sprite_set_light_effect1", "prim_gradient_rect",
]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"SetWorkBase": {"args": ["s32", "s32"], "ret": "void"},
	"GetWorkBase": {"args": ["s32", "s32"], "ret": "s32"},
	"sprite_set_gpu_poly_ft4": {"args": ["ptr", "s32", "s32"], "ret": "void"},
	"goto_tpage": {"args": ["s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"safe_env": {"args": ["s32"], "ret": "void"},
	"dtd_on_tpage": {"args": ["s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"dtd_on": {"args": ["s32"], "ret": "void"},
	"_sprite_set_light_effect1": {"args": ["s32", "s32", "s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"prim_gradient_rect": {"args": ["s32", "s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
}

## PrimArena (include/game/draw_records.h): size 0, used 4, base 8; GameState.prim_arena[2], stride 0xC.
const ARENA_STRIDE: int = 0xC
const ARENA_SIZE: int = 0x10C00

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

# ---- helpers (not C functions)

## &draw_ot_tags[(buffer << 10) + slot * 4] (draw_ot_tags is u8[]).
func _ot(buffer: int, slot: int) -> int:
	return Math.s32(engine.memory.global_address("draw_ot_tags") + Math.sll32(buffer, 10) + slot * 4)

func _draw_buffer() -> int:
	return engine.memory.read_global("draw_buffer")

func _origin_x(buffer: int) -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_x") + buffer * 2)

func _origin_y(buffer: int) -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_y") + buffer * 2)

# ---- the C functions

## Resets a draw-buffer primitive arena: base = arg0, size 0x10C00, used 0.
func SetWorkBase(arg0: int, arg1: int) -> void:
	var arena: int = engine.gs_address("prim_arena") + arg1 * ARENA_STRIDE
	engine.memory.write_s32(arena + 8, arg0)
	engine.memory.write_s32(arena + 0, ARENA_SIZE)
	engine.memory.write_s32(arena + 4, 0)

## base + used * 4; used += ((arg0 + 3) / 4) * 4 (C division truncates toward zero, as GDScript does).
@warning_ignore("integer_division")
func GetWorkBase(arg0: int, arg1: int) -> int:
	var arena: int = engine.gs_address("prim_arena") + arg1 * ARENA_STRIDE
	var m: Object = engine.memory
	var ret: int = Math.s32(m.s32(arena + 8) + m.s32(arena + 4) * 4)
	m.write_s32(arena + 4, Math.add32(m.s32(arena + 4), Math.s32(((arg0 + 3) / 4) * 4)))
	return ret

## FT4 sprite quad (SpriteQuad at arg0) into OT slot arg1 of buffer arg2; nothing when flags bit 31 is set.
func sprite_set_gpu_poly_ft4(arg0: int, arg1: int, arg2: int) -> void:
	var m: Object = engine.memory
	if m.s32(arg0 + Layout.SPRITE_QUAD_FLAGS) & 0x80000000:
		return
	var p: int = Math.u32(engine.call_c("GetWorkBase", [0x28, arg2]))
	engine.call_c("func_8009F040", [p])
	if m.s32(arg0 + Layout.SPRITE_QUAD_FLAGS) & 0x40000000:
		engine.call_c("SetSemiTrans", [p, 1])
	else:
		engine.call_c("SetSemiTrans", [p, 0])
	engine.call_c("SetShadeTex", [p, 0])
	m.write_u8(p + 4, m.u8(arg0 + Layout.SPRITE_QUAD_TINT_R))
	m.write_u8(p + 5, m.u8(arg0 + Layout.SPRITE_QUAD_TINT_G))
	m.write_u8(p + 6, m.u8(arg0 + Layout.SPRITE_QUAD_TINT_B))
	var ox: int = _origin_x(arg2)
	var oy: int = _origin_y(arg2)
	var x: int = m.s16(arg0 + Layout.SPRITE_QUAD_X)
	var y: int = m.s16(arg0 + Layout.SPRITE_QUAD_Y)
	var ax: int = m.s16(arg0 + Layout.SPRITE_QUAD_ANCHOR_X)
	var ay: int = m.s16(arg0 + Layout.SPRITE_QUAD_ANCHOR_Y)
	var w: int = m.u16(arg0 + Layout.SPRITE_QUAD_W) # (u16)arg0->w
	var h: int = m.u16(arg0 + Layout.SPRITE_QUAD_H) # SpriteQuad.h is u16
	var u: int = m.u8(arg0 + Layout.SPRITE_QUAD_UNK_0E)
	var v: int = m.u8(arg0 + Layout.SPRITE_QUAD_UNK_0F)
	m.write_s16(p + 0x8, ox + x - ax)
	m.write_s16(p + 0xA, oy + y - ay)
	m.write_s16(p + 0x10, ox + x - ax + w)
	m.write_s16(p + 0x12, oy + y - ay)
	m.write_s16(p + 0x18, ox + x - ax)
	m.write_s16(p + 0x1A, oy + y - ay + h)
	m.write_s16(p + 0x20, ox + x - ax + w)
	m.write_s16(p + 0x22, oy + y - ay + h)
	m.write_u8(p + 0xC, u)
	m.write_u8(p + 0xD, v)
	m.write_u8(p + 0x14, u + w)
	m.write_u8(p + 0x15, v)
	m.write_u8(p + 0x1C, u)
	m.write_u8(p + 0x1D, v + h)
	m.write_u8(p + 0x24, u + w)
	m.write_u8(p + 0x25, v + h)
	m.write_s16(p + 0x16, m.u16(arg0 + Layout.SPRITE_QUAD_UNK_0C) | 0x80)
	var clut: int = engine.call_c("func_8009ED74", [m.s16(arg0 + Layout.SPRITE_QUAD_UNK_10), m.s16(arg0 + Layout.SPRITE_QUAD_UNK_12)])
	m.write_s16(p + 0xE, clut)
	engine.call_c("AddPrim", [_ot(arg2, arg1), p])

## Draw-mode primitive (tpage from GetTPage) into slot arg2 of the current buffer, then a safe_env tile.
func goto_tpage(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> void:
	var tpage: int = Math.u16(engine.call_c("func_8009ECB0", [arg4, arg3, arg0, arg1]))
	var work: int = Math.u32(engine.call_c("GetWorkBase", [0xC, _draw_buffer()]))
	engine.call_c("func_8009D294", [work, 0, 0, tpage, 0])
	engine.call_c("AddPrim", [_ot(_draw_buffer(), arg2), work])
	engine.call_c("safe_env", [arg2])

## One-pixel tile at (0, 0) in OT slot arg0 of the current buffer.
func safe_env(arg0: int) -> void:
	var p: int = Math.u32(engine.call_c("GetWorkBase", [0xC, _draw_buffer()]))
	engine.call_c("func_8009F0B8", [p])
	engine.memory.write_s16(p + 8, 0)
	engine.memory.write_s16(p + 0xA, 0)
	engine.call_c("AddPrim", [_ot(_draw_buffer(), arg0), p])

## goto_tpage with the dither flag set (func_8009D294(work, 0, 1, tpage, 0)).
func dtd_on_tpage(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> void:
	var work: int = Math.u32(engine.call_c("GetWorkBase", [0xC, _draw_buffer()]))
	var tpage: int = engine.call_c("func_8009ECB0", [arg4, arg3, arg0, arg1])
	engine.call_c("func_8009D294", [work, 0, 1, tpage, 0])
	engine.call_c("AddPrim", [_ot(_draw_buffer(), arg2), work])
	engine.call_c("safe_env", [arg2])

## Dither-enabled draw mode (tpage 0) and a tile in OT slot arg0.
func dtd_on(arg0: int) -> void:
	var work: int = Math.u32(engine.call_c("GetWorkBase", [0xC, _draw_buffer()]))
	engine.call_c("func_8009D294", [work, 0, 1, 0, 0])
	engine.call_c("AddPrim", [_ot(_draw_buffer(), arg0), work])
	engine.call_c("safe_env", [arg0])

## Pulsing POLY_G3 light. The three pulse values call rsin in the C order (r, g, b), then abs(rsin(...)) for
## the width. frame_count * 0xFF0 wraps on 32 bits; the (rsin << 5) / 4096 term is an s32 shift and a
## truncating division; the colour stores keep the low byte.
@warning_ignore("integer_division")
func _sprite_set_light_effect1(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int, arg7: int, arg8: int) -> void:
	var m: Object = engine.memory
	var fc: int = engine.gs_get("frame_count")
	var r: int = Math.add32(Math.s32(Math.sll32(Math.rsin(Math.s32(fc * 0xFF0)), 5) / 4096), (arg7 >> 16) & 0xFF)
	var g: int = Math.add32(Math.s32(Math.sll32(Math.rsin(Math.s32(fc * 0xFF0)), 5) / 4096), (arg7 >> 8) & 0xFF)
	var b: int = Math.add32(Math.s32(Math.sll32(Math.rsin(Math.s32(fc * 0xFF0)), 5) / 4096), arg7 & 0xFF)
	# FAKE T-25115 in the C: signed sum (frame_count + arg8).
	var s: int = absi(Math.rsin(Math.s32(Math.add32(Math.s32(engine.gs_get("frame_count")), arg8) * 0xFF0)))
	var p: int = Math.u32(engine.call_c("GetWorkBase", [0x1C, _draw_buffer()]))
	engine.call_c("func_8009F004", [p])
	engine.call_c("SetSemiTrans", [p, 1])
	m.write_u8(p + 4, r * 3 / 5)
	m.write_u8(p + 5, g * 3 / 5)
	m.write_u8(p + 6, b * 3 / 5)
	m.write_u8(p + 0xC, r * 0 / 5)
	m.write_u8(p + 0xD, g * 0 / 5)
	m.write_u8(p + 0xE, b * 0 / 5)
	m.write_u8(p + 0x14, r * 0 / 5)
	m.write_u8(p + 0x15, g * 0 / 5)
	m.write_u8(p + 0x16, b * 0 / 5)
	var db: int = _draw_buffer()
	if arg4 == 0:
		m.write_s16(p + 0x8, _origin_x(db) + arg0)
		m.write_s16(p + 0xA, _origin_y(db) + arg1)
		var w: int = Math.s32(s * 8) / 4096
		m.write_s16(p + 0x10, _origin_x(db) + arg2 - w)
		m.write_s16(p + 0x12, _origin_y(db) + arg3)
		m.write_s16(p + 0x18, _origin_x(db) + arg2 + w)
		m.write_s16(p + 0x1A, _origin_y(db) + arg3)
	else:
		m.write_s16(p + 0x8, _origin_x(db) + arg0)
		m.write_s16(p + 0xA, _origin_y(db) + arg1)
		m.write_s16(p + 0x10, _origin_x(db) + arg2)
		m.write_s16(p + 0x12, _origin_y(db) + arg3)
		m.write_s16(p + 0x18, _origin_x(db) + arg4)
		m.write_s16(p + 0x1A, _origin_y(db) + arg5)
	engine.call_c("AddPrim", [_ot(_draw_buffer(), arg6), p])

## Gouraud box (POLY_G4). arg6 bit 7: semi-transparent; arg6 & 0x7F picks the vertex colours (cases 6, 5, 4,
## 3, 1, 2, 0/default). Every store is a u8 (the low byte of the product).
@warning_ignore("integer_division")
func prim_gradient_rect(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int) -> void:
	var m: Object = engine.memory
	var p: int = Math.u32(engine.call_c("GetWorkBase", [0x24, _draw_buffer()]))
	engine.call_c("func_8009F054", [p])
	if arg6 & 0x80:
		engine.call_c("SetSemiTrans", [p, 1])
	else:
		engine.call_c("SetSemiTrans", [p, 0])
	var r: int = (arg5 >> 16) & 0xFF
	var g: int = (arg5 >> 8) & 0xFF
	var b: int = arg5 & 0xFF
	# vertex n colour at p + 4 + 8 * n (r, g, b bytes); vertices are 0, 1, 2, 3 = +4, +0xC, +0x14, +0x1C
	match arg6 & 0x7F:
		6:
			_rgb(p + 0x4, r * 0, g * 0, b * 0)
			_rgb(p + 0xC, r, g, b)
			_rgb(p + 0x14, r * 0, g * 0, b * 0)
			_rgb(p + 0x1C, r, g, b)
		5:
			_rgb(p + 0x4, r, g, b)
			_rgb(p + 0xC, r * 0, g * 0, b * 0)
			_rgb(p + 0x14, r, g, b)
			_rgb(p + 0x1C, r * 0, g * 0, b * 0)
		4:
			_rgb(p + 0x4, r * 0, g * 0, b * 0)
			_rgb(p + 0xC, r * 0, g * 0, b * 0)
			_rgb(p + 0x14, r, g, b)
			_rgb(p + 0x1C, r, g, b)
		3:
			# the C stores (u8)r * 8, (u8)g * 8 and (u8)g * 8 (the blue channel gets green) in all four vertices
			for off: int in [0x1C, 0x14, 0xC, 0x4]:
				_rgb(p + off, r * 8, g * 8, g * 8)
		1:
			for off: int in [0x1C, 0x14, 0xC, 0x4]:
				_rgb(p + off, r, g, b)
		2:
			_rgb(p + 0x1C, r * 8 / 10, g * 8 / 10, b * 8 / 10)
			for off: int in [0x4, 0x14, 0xC]:
				_rgb(p + off, r * 10 / 10, g * 10 / 10, b * 10 / 10)
		_:
			_rgb(p + 0x1C, r * 4, g * 4, b * 4)
			_rgb(p + 0x14, r * 6, g * 6, b * 6)
			_rgb(p + 0xC, r * 8, g * 8, b * 8)
			_rgb(p + 0x4, r * 10, g * 10, b * 10)
	var db: int = _draw_buffer()
	m.write_s16(p + 0x8, _origin_x(db) + arg0)
	m.write_s16(p + 0xA, _origin_y(db) + arg1)
	m.write_s16(p + 0x10, _origin_x(db) + arg0 + arg2)
	m.write_s16(p + 0x12, _origin_y(db) + arg1)
	m.write_s16(p + 0x18, _origin_x(db) + arg0)
	m.write_s16(p + 0x1A, _origin_y(db) + arg1 + arg3)
	m.write_s16(p + 0x20, _origin_x(db) + arg0 + arg2)
	m.write_s16(p + 0x22, _origin_y(db) + arg1 + arg3)
	engine.call_c("AddPrim", [_ot(_draw_buffer(), arg4), p])

func _rgb(at: int, r: int, g: int, b: int) -> void:
	engine.memory.write_u8(at, r)
	engine.memory.write_u8(at + 1, g)
	engine.memory.write_u8(at + 2, b)
