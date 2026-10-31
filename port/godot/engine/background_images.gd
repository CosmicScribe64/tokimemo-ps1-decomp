extends RefCounted
## Port of src/main/background_images.c (original object 0x80057390-0x80058D20, T-39311 on the T-39301 engine).
## The MDEC still-background cache (two LBAs per layer, the shown-buffer flags), the decode-to-VRAM request, the
## screen capture, the wipe, and the background draw/fade/crossfade that build SPRT primitives in RAM and link
## them into the ordering table. libgpu/MDEC calls (AddPrim, SetSemiTrans, SetShadeTex, func_8009F0A4 SetSprt,
## func_8009C93C MoveImage, func_8009C674 DrawSync, bg_still_buffers_set, bg_still_decode_to_vram) are call_c and
## refuse by name until their owners are ported; cd_read_sectors_nowait, goto_tpage, prim_gradient_line and the
## other ported functions run. Contract: wiki/engine-porting-contract.md.
## Tests: tests/engine/background_images_test.gd, config/port/engine-diff/background_images.json.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/background_images.c"
const C_FUNCTIONS: Array[String] = [
	"set_dec_bri", "get_last_gamen_mode", "dec_bg_reset", "dec_bg_show_switch", "dec_bg_show_set", "dec_bg_cd_read",
	"dec_bg_decode_to_vram", "bg_capture_screen", "bg_wipe_step", "dec_bg_draw", "bg_fade_set", "bg_fade_draw",
	"bg_crossfade_draw",
]
const C_UNPORTED: Array[String] = []
## dec_bg_decode_to_vram returns 0 or 1 on its handled (layer, buffer) pairs and falls off the end for any other
## layer or flag value; bg_wipe_step is declared s32 and returns nothing.
const C_SIGNATURES: Dictionary = {
	"set_dec_bri": {"args": ["u8"], "ret": "void"},
	"get_last_gamen_mode": {"args": [], "ret": "s32"},
	"dec_bg_reset": {"args": [], "ret": "void"},
	"dec_bg_show_switch": {"args": ["s32"], "ret": "void"},
	"dec_bg_show_set": {"args": ["s32", "s32"], "ret": "void"},
	"dec_bg_cd_read": {"args": ["s32", "s32"], "ret": "s32"},
	"dec_bg_decode_to_vram": {"args": ["s32"], "ret": "s32", "ret_defined": [0, 1]},
	"bg_capture_screen": {"args": ["s32"], "ret": "void"},
	"bg_wipe_step": {"args": [], "ret": "s32", "ret_undefined": "s32 in the C with no return statement"},
	"dec_bg_draw": {"args": ["s32"], "ret": "void"},
	"bg_fade_set": {"args": ["u8"], "ret": "void"},
	"bg_fade_draw": {"args": ["s32"], "ret": "void"},
	"bg_crossfade_draw": {"args": ["s32"], "ret": "void"},
}

## Data symbol the generated layout lacks (s32, background_images.c .data order, config/symbol_addrs*.txt has
## no entry; reported in the ticket): D_800B5934.
const D_800B5934: int = 0x800B5934
## bg_capture_screen keeps its RECT at +0x34 of its 0x40 frame (asm/matchings/main/background_images).
const CAPTURE_RECT: int = 0x34
## SPRT (include/psx_types.h): r0 g0 b0 at +4, x0 +8, y0 +0xA, u0 +0xC, v0 +0xD, w +0x10, h +0x12; size 0x14.
const SPRT_SIZE: int = 0x14

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func _show(index: int) -> int:
	return engine.memory.global_address("dec_bg_show") + index

func _db() -> int:
	return engine.memory.read_global("draw_buffer")

func _ox() -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_x") + _db() * 2)

func _oy() -> int:
	return engine.memory.s16(engine.gs_address("draw_origin_y") + _db() * 2)

func _slot(slot_bytes: int) -> int:
	return Math.s32(engine.memory.global_address("draw_ot_tags") + Math.sll32(_db(), 10) + slot_bytes)

func set_dec_bri(arg0: int) -> void:
	engine.memory.write_global("bg_brightness", Math.u8(arg0))

func get_last_gamen_mode() -> int:
	return engine.memory.read_global("dec_bg_last_screen_mode")

func dec_bg_reset() -> void:
	var m: Object = engine.memory
	for i: int in 4:
		m.write_global("bg_lba_cache", 0, i)
	m.write_u8(_show(1), 0)
	m.write_u8(_show(0), 0)
	m.write_global("bg_brightness", 0x80)
	m.write_global("dec_bg_last_screen_mode", -1)
	m.write_s32(D_800B5934, 0)
	m.write_u8(_show(2), 0)

func dec_bg_show_switch(arg0: int) -> void:
	engine.memory.write_u8(_show(arg0), 1 - engine.memory.u8(_show(arg0)))

func dec_bg_show_set(arg0: int, arg1: int) -> void:
	engine.memory.write_u8(_show(arg0), arg1 & 1)

## Starts reading background file `arg0` (cd_read_sectors_nowait(0xA, 0x80162000, lba)) unless layer arg1's cache
## already holds it: returns 0 or 1 for a hit in the first or second cached LBA, -1 for a read started (the LBA
## then replaces the entry selected by the shown flag: bg_lba_cache[3] - dec_bg_show[1] * 4 words for layer 1,
## bg_lba_cache[1] - dec_bg_show[0] * 4 for layer 0).
func dec_bg_cd_read(arg0: int, arg1: int) -> int:
	var m: Object = engine.memory
	var cache: int = m.global_address("bg_lba_cache")
	if arg1 == 1:
		if arg0 == m.read_global("bg_lba_cache", 2):
			return 0
		if arg0 == m.read_global("bg_lba_cache", 3):
			return 1
		engine.call_c("cd_read_sectors_nowait", [0xA, 0x80162000, arg0])
		m.write_s32(Math.u32(cache + 12 - m.u8(_show(1)) * 4), arg0)
		return -1
	if arg0 == m.read_global("bg_lba_cache", 0):
		return 0
	if arg0 == m.read_global("bg_lba_cache", 1):
		return 1
	engine.call_c("cd_read_sectors_nowait", [0xA, 0x80162000, arg0])
	m.write_s32(Math.u32(cache + 4 - m.u8(_show(0)) * 4), arg0)
	return -1

## Decodes the loaded still into the VRAM buffer that is not shown: layer 0 (320 x 88) at (0x2C0, 0xA0) or
## (0x140, 0x1A0), layer 1 (320 x 160) at (0x2C0, 0) or (0x140, 0x100); returns the buffer (0 or 1).
func dec_bg_decode_to_vram(arg0: int) -> int:
	var m: Object = engine.memory
	m.write_global("bg_decode_result", 0)
	engine.call_c("bg_still_buffers_set", [0x80167000, 0x80162000, 0x80162000])
	if arg0 == 0:
		match 1 - m.u8(_show(arg0)):
			0:
				m.write_global("bg_decode_result", engine.call_c("bg_still_decode_to_vram", [0x2C0, 0xA0, 0x140, 0x58]))
				return 0
			1:
				m.write_global("bg_decode_result", engine.call_c("bg_still_decode_to_vram", [0x140, 0x1A0, 0x140, 0x58]))
				return 1
	elif arg0 == 1:
		match 1 - m.u8(_show(arg0)):
			0:
				m.write_global("bg_decode_result", engine.call_c("bg_still_decode_to_vram", [0x2C0, 0, 0x140, 0xA0]))
				return 0
			1:
				m.write_global("bg_decode_result", engine.call_c("bg_still_decode_to_vram", [0x140, 0x100, 0x140, 0xA0]))
				return 1
	return 0

## Copies the current draw buffer's top arg0 rows (RECT {0, buffer * 0xF0, 0x140, arg0} on the stack) into the
## hidden still buffer (MoveImage to (0x2C0, 0) or (0x140, 0x100)), points layer 1's shown flag at it and clears
## the wipe counter; v = 1 - D_800B5939 also clears bg_lba_cache[2 + v].
func bg_capture_screen(arg0: int) -> void:
	var m: Object = engine.memory
	var rect: int = engine.frame_base() + CAPTURE_RECT
	m.write_u16(rect + 0, 0)
	m.write_u16(rect + 2, _db() * 0xF0)
	m.write_u16(rect + 4, 0x140)
	m.write_u16(rect + 6, arg0)
	var v: int = 1 - m.read_global("D_800B5939")
	m.write_s32(Math.u32(m.global_address("bg_lba_cache") + 8 + v * 4), 0)
	match v:
		0:
			engine.call_c("func_8009C93C", [rect, 0x2C0, 0])
		1:
			engine.call_c("func_8009C93C", [rect, 0x140, 0x100])
	engine.call_c("dec_bg_show_set", [1, Math.u32(1 - m.read_global("D_800B5939"))])
	m.write_global("bg_wipe_count", 0)
	engine.call_c("func_8009C674", [0])

## One wipe step: brightness falls by 8 (to 0 when it passes 0x80), the count grows, and 15 bands of gradient
## lines (width min(2 * count, 0x20) each) are drawn through prim_gradient_line in three regions.
func bg_wipe_step() -> int:
	var m: Object = engine.memory
	m.write_global("bg_brightness", m.read_global("bg_brightness") - 8)
	if m.read_global("bg_brightness") >= 0x81:
		m.write_global("bg_brightness", 0)
	m.write_global("bg_wipe_count", m.read_global("bg_wipe_count") + 1)
	var doubled: int = Math.s32(m.read_global("bg_wipe_count") * 2)
	var width: int = Math.s16(doubled)
	if Math.s16(doubled) > 0x20:
		width = 0x20
	var band: int = 0
	while true: # do { ... band++ } while (band < 0xF)
		if engine.halted():
			return 0
		var offset: int = band << 5
		var x: int = 0
		var edge: int = offset + 0x1F
		while x < width:
			if engine.halted():
				return 0
			var point: int = edge - x
			if offset < 0xA0:
				engine.call_c("prim_gradient_line", [Math.s16(point - 0xA0), -0x78, -0xA0, Math.s16(point - 0x78), 0, 0, 0xD])
			elif offset < 0x140:
				engine.call_c("prim_gradient_line", [Math.s16(point - 0xA0), -0x78, Math.s16(point - 0x140), 0x28, 0, 0, 0xD])
			else:
				engine.call_c("prim_gradient_line", [0xA0, Math.s16(point - 0x1B8), Math.s16(point - 0x140), 0x28, 0, 0, 0xD])
			x += 1
		band += 1
		if band >= 0xF:
			break
	return 0

## One SPRT: allocate 0x14 bytes, SetSprt, semi-transparency, SetShadeTex, grey `color` in r0, g0, b0, then
## position (the draw origin plus `x_off`, `y_off`), u0 = 0, v0, and the size; linked into OT byte offset
## `slot_bytes` (0x28 slot 10, 0x30 slot 12, 0x34 slot 13) of the current buffer.
func _sprt(semi: int, color: int, x_off: int, y_off: int, v0: int, w: int, h: int, slot_bytes: int) -> void:
	var m: Object = engine.memory
	var sprite: int = Math.u32(engine.call_c("GetWorkBase", [SPRT_SIZE, _db()]))
	engine.call_c("func_8009F0A4", [sprite])
	engine.call_c("SetSemiTrans", [sprite, semi])
	engine.call_c("SetShadeTex", [sprite, 0])
	m.write_u8(sprite + 4, color)
	m.write_u8(sprite + 5, color)
	m.write_u8(sprite + 6, color)
	m.write_s16(sprite + 8, _ox() + x_off)
	m.write_s16(sprite + 0xA, _oy() + y_off)
	m.write_u8(sprite + 0xD, v0)
	m.write_u8(sprite + 0xC, 0)
	m.write_s16(sprite + 0x10, w)
	m.write_s16(sprite + 0x12, h)
	engine.call_c("AddPrim", [_slot(slot_bytes), sprite])

## The left (x - 0xA0, width 0xC0) and right (x + 0x20, width 0x80) sprites of the draw/fade functions. `mode`
## selects the picture rows: 1 -> height 0xA0 from the top, 2 -> 0xF0 (dec_bg_draw only), else 0x58 rows from
## v 0xA0 with y - 0x30.
func _side(mode: int, right: bool, semi: int, color: int, slot_bytes: int, allow_two: bool) -> void:
	var x_off: int = 0x20 if right else -0xA0
	var w: int = 0x80 if right else 0xC0
	if mode == 1:
		_sprt(semi, color, x_off, -0x78, 0, w, 0xA0, slot_bytes)
	elif allow_two and mode == 2:
		_sprt(semi, color, x_off, -0x78, 0, w, 0xF0, slot_bytes)
	else:
		_sprt(semi, color, x_off, -0x30, 0xA0, w, 0x58, slot_bytes)

## Draws background layer `a` (-1: nothing): a left sprite and a right sprite in OT slot 13 with the layer's
## brightness, each followed by goto_tpage selecting the shown VRAM buffer (0x140/0x200 at y 0x100 when the shown
## flag is set, else 0x2C0/0x380 at y 0).
func dec_bg_draw(a: int) -> void:
	var m: Object = engine.memory
	if a == -1:
		return
	m.write_global("dec_bg_last_screen_mode", a)
	var bright: int = m.read_global("bg_brightness")
	_side(a, false, 0, bright, 0x34, true)
	if m.u8(_show(a)) != 0:
		engine.call_c("goto_tpage", [0x140, 0x100, 0xD, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xD, 0, 2])
	_side(a, true, 0, m.read_global("bg_brightness"), 0x34, true)
	if m.u8(_show(a)) != 0:
		engine.call_c("goto_tpage", [0x200, 0x100, 0xD, 0, 2])
		return
	engine.call_c("goto_tpage", [0x380, 0, 0xD, 0, 2])

func bg_fade_set(arg0: int) -> void:
	engine.memory.write_global("bg_fade_amount", Math.u8(arg0))

## Fade between the old (semi-transparent, grey 0x80 - fade) and new (opaque, grey fade) stills: four sprites
## and their goto_tpage calls, slots 10 then 13.
func bg_fade_draw(arg0: int) -> void:
	var m: Object = engine.memory
	var fade: int = m.read_global("bg_fade_amount")
	_side(arg0, false, 1, 0x80 - fade, 0x28, false)
	if m.u8(_show(arg0)) != 0:
		engine.call_c("goto_tpage", [0x140, 0x100, 0xA, 1, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xA, 1, 2])
	_side(arg0, true, 1, 0x80 - m.read_global("bg_fade_amount"), 0x28, false)
	if m.u8(_show(arg0)) != 0:
		engine.call_c("goto_tpage", [0x200, 0x100, 0xA, 1, 2])
	else:
		engine.call_c("goto_tpage", [0x380, 0, 0xA, 1, 2])
	_side(arg0, false, 0, m.read_global("bg_fade_amount"), 0x34, false)
	if m.u8(_show(arg0)) != 0:
		engine.call_c("goto_tpage", [0x140, 0x100, 0xD, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xD, 0, 2])
	_side(arg0, true, 0, m.read_global("bg_fade_amount"), 0x34, false)
	if m.u8(_show(arg0)) != 0:
		engine.call_c("goto_tpage", [0x200, 0x100, 0xD, 0, 2])
		return
	engine.call_c("goto_tpage", [0x380, 0, 0xD, 0, 2])

## bg_fade_draw with six sprites (OT slots 10, 12, 13) whose tpages alternate with the shown flag.
func bg_crossfade_draw(arg0: int) -> void:
	var m: Object = engine.memory
	var fade: int = m.read_global("bg_fade_amount")
	var shown: Callable = func() -> bool:
		return m.u8(_show(arg0)) != 0
	_side(arg0, false, 1, 0x80 - fade, 0x28, false)
	if not shown.call():
		engine.call_c("goto_tpage", [0x140, 0x100, 0xA, 1, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xA, 1, 2])
	_side(arg0, true, 1, 0x80 - m.read_global("bg_fade_amount"), 0x28, false)
	if not shown.call():
		engine.call_c("goto_tpage", [0x200, 0x100, 0xA, 1, 2])
	else:
		engine.call_c("goto_tpage", [0x380, 0, 0xA, 1, 2])
	_side(arg0, false, 0, m.read_global("bg_fade_amount"), 0x30, false)
	if shown.call():
		engine.call_c("goto_tpage", [0x140, 0x100, 0xC, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xC, 0, 2])
	_side(arg0, true, 0, m.read_global("bg_fade_amount"), 0x30, false)
	if shown.call():
		engine.call_c("goto_tpage", [0x200, 0x100, 0xC, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x380, 0, 0xC, 0, 2])
	_side(arg0, false, 0, m.read_global("bg_fade_amount"), 0x34, false)
	if not shown.call():
		engine.call_c("goto_tpage", [0x140, 0x100, 0xD, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xD, 0, 2])
	_side(arg0, true, 0, m.read_global("bg_fade_amount"), 0x34, false)
	if not shown.call():
		engine.call_c("goto_tpage", [0x200, 0x100, 0xD, 0, 2])
		return
	engine.call_c("goto_tpage", [0x380, 0, 0xD, 0, 2])
