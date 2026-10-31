extends RefCounted
## T-39314: the PsyQ libgpu layer of the facade (asm/libgpu.s, original object 0x8009C210-0x8009F678) plus the
## two libgs leaves that are plain libgpu calls (GsDrawOt, GsSortClear). Not a C file of the game: the
## underscore keeps the directory scan from registering it, engine.gd registers it as module "sdk_gpu".
##
## The game calls these through their symbol names (`func_8009F004`, `AddPrim`, ...; the names come from
## config/symbol_addrs_sdk.txt and the asm labels). Each function does what the library does on the game
## memory: primitive headers, ordering-table links, the library's own state words (0x800DD580-0x800DD6A4),
## the GP1 shadow table (0x800E6160) and the command lists the library builds in RAM (the DrawEnv packet inside
## the DRAWENV, the ClearImage list at 0x800E6120, the MoveImage list at 0x800DD630). What the library sends
## to the GPU through DMA and the data port goes to `engine.renderer` (Ps1Renderer) as the same words in the
## same order: `draw_ordering_table_from_memory` for linked lists (DMA2), `load_image`/`store_image` for
## CPU-VRAM transfers, `gp1` for display control. No function ever invents a value: a path the model does not
## cover refuses by name (see `_refuse`).
##
## Timing model (the contract's blocking-wait rule): the renderer draws a list before the call returns, so the
## GPU is idle and the DMA channels are free at every later call. The library's fast path (queue empty, DMA
## idle, no draw-sync callback: `func_8009E200`) is therefore the only path, and a call that finds the library
## state in another one (a queued job in 0x800DD68C/0x800DD690, a draw-sync callback in 0x800DD594) refuses.
## The interrupt mask the library saves around a queued call is a hardware register (I_MASK, not RAM): the
## model keeps it in `sdk_etc` (`irq_mask`). The VBlank counter `VSync(-1)` reads is RAM (0x800E35E0) that
## only the VBlank interrupt advances; the port never advances it, so the deadline words the library derives
## from it are stale-but-defined (documented in the spec of this module).
##
## Differential tests: config/port/engine-diff/sdk_gpu.json against emulator captures (tools/emu/func_capture);
## hand checks: tests/engine/sdk_gpu_test.gd.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "libgpu (asm/libgpu.s, asm/libgs.s)"
const C_FUNCTIONS: Array[String] = [
	"AddPrim", "AddPrims", "SetSemiTrans", "SetShadeTex",
	"func_8009EFDC", "func_8009EFF0", "func_8009F004", "func_8009F018", "func_8009F02C", "func_8009F040",
	"func_8009F054", "func_8009F068", "func_8009F07C", "func_8009F090", "func_8009F0A4", "func_8009F0B8",
	"func_8009F0CC", "func_8009F0E0", "func_8009F0F4", "func_8009F108", "func_8009F11C", "func_8009F130",
	"func_8009F150", "func_8009F170", "func_8009F190",
	"func_8009ECB0", "func_8009ED74", "func_8009D294",
	"func_8009C9F8", "func_8009CAAC", "func_8009CB9C", "func_8009CB3C",
	"func_8009C884", "func_8009C8E0", "func_8009C93C", "func_8009C7F8",
	"func_8009C674", "func_8009C210", "func_8009C5E0", "func_8009CC04", "func_8009CCF4",
	"func_8009AD00", "func_8009824C",
]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"AddPrim": {"args": ["ptr", "ptr"], "ret": "void"},
	"AddPrims": {"args": ["ptr", "ptr", "ptr"], "ret": "void"},
	"SetSemiTrans": {"args": ["ptr", "s32"], "ret": "void"},
	"SetShadeTex": {"args": ["ptr", "s32"], "ret": "void"},
	"func_8009EFDC": {"args": ["ptr"], "ret": "void"},
	"func_8009EFF0": {"args": ["ptr"], "ret": "void"},
	"func_8009F004": {"args": ["ptr"], "ret": "void"},
	"func_8009F018": {"args": ["ptr"], "ret": "void"},
	"func_8009F02C": {"args": ["ptr"], "ret": "void"},
	"func_8009F040": {"args": ["ptr"], "ret": "void"},
	"func_8009F054": {"args": ["ptr"], "ret": "void"},
	"func_8009F068": {"args": ["ptr"], "ret": "void"},
	"func_8009F07C": {"args": ["ptr"], "ret": "void"},
	"func_8009F090": {"args": ["ptr"], "ret": "void"},
	"func_8009F0A4": {"args": ["ptr"], "ret": "void"},
	"func_8009F0B8": {"args": ["ptr"], "ret": "void"},
	"func_8009F0CC": {"args": ["ptr"], "ret": "void"},
	"func_8009F0E0": {"args": ["ptr"], "ret": "void"},
	"func_8009F0F4": {"args": ["ptr"], "ret": "void"},
	"func_8009F108": {"args": ["ptr"], "ret": "void"},
	"func_8009F11C": {"args": ["ptr"], "ret": "void"},
	"func_8009F130": {"args": ["ptr"], "ret": "void"},
	"func_8009F150": {"args": ["ptr"], "ret": "void"},
	"func_8009F170": {"args": ["ptr"], "ret": "void"},
	"func_8009F190": {"args": ["ptr"], "ret": "void"},
	"func_8009ECB0": {"args": ["s32", "s32", "s32", "s32"], "ret": "s32"},
	"func_8009ED74": {"args": ["s32", "s32"], "ret": "s32"},
	"func_8009D294": {"args": ["ptr", "s32", "s32", "s32", "ptr"], "ret": "void"},
	"func_8009C9F8": {"args": ["ptr", "s32"], "ret": "ptr"},
	"func_8009CAAC": {"args": ["ptr", "s32"], "ret": "ptr"},
	"func_8009CB9C": {"args": ["ptr"], "ret": "void"},
	"func_8009CB3C": {"args": ["ptr"], "ret": "void"},
	"func_8009C884": {"args": ["ptr", "ptr"], "ret": "void"},
	"func_8009C8E0": {"args": ["ptr", "ptr"], "ret": "void"},
	"func_8009C93C": {"args": ["ptr", "s32", "s32"], "ret": "void"},
	"func_8009C7F8": {"args": ["ptr", "s32", "s32", "s32"], "ret": "void"},
	"func_8009C674": {"args": ["s32"], "ret": "s32"},
	"func_8009C210": {"args": ["s32"], "ret": "s32"},
	"func_8009C5E0": {"args": ["s32"], "ret": "void"},
	"func_8009CC04": {"args": ["ptr"], "ret": "ptr"},
	"func_8009CCF4": {"args": ["ptr"], "ret": "ptr"},
	"func_8009AD00": {"args": ["ptr"], "ret": "void"},
	"func_8009824C": {"args": ["s32", "s32", "s32", "ptr"], "ret": "void"},
}

# ---- library state (addresses of the D_800DDxxx data labels in asm/data/data_800CA380.data.s)
const GRAPH_TYPE: int = 0x800DD588 ## u8 D_800DD588: GetGraphType (set by the GPU probe in ResetGraph)
const QUEUE_MODE: int = 0x800DD589 ## u8 D_800DD589: SetGraphQueue mode (1 = queued drawing)
const DEBUG_LEVEL: int = 0x800DD58A ## u8 D_800DD58A: SetGraphDebug level
const REVERSE: int = 0x800DD58B ## u8 D_800DD58B: SetGraphReverse
const SCREEN_W: int = 0x800DD58C ## s16 D_800DD58C
const SCREEN_H: int = 0x800DD58E ## s16 D_800DD58E
const QUEUE_ACTIVE: int = 0x800DD590 ## u32 D_800DD590
const SYNC_CALLBACK: int = 0x800DD594 ## u32 D_800DD594: DrawSyncCallback function
const DRAWENV_COPY: int = 0x800DD598 ## 0x5C bytes: the DRAWENV PutDrawEnv last stored (GetDrawEnv)
const DISPENV_COPY: int = 0x800DD5F4 ## 0x14 bytes: the DISPENV PutDispEnv last stored
const MOVE_LIST: int = 0x800DD630 ## 5 words: the MoveImage DMA list (tag, 0x80000000, src, dst, size)
const OT_TERMINATOR: int = 0x800DD644 ## the 0x04FFFFFF node every OT ends in
const LAST_FN: int = 0x800DD67C ## D_800DD67C/680/684: function, address, parameter of the last queued call
const QUEUE_HEAD: int = 0x800DD68C
const QUEUE_TAIL: int = 0x800DD690
const SAVED_MASK_QUEUE: int = 0x800DD694 ## the interrupt mask E200 saved
const SAVED_MASK_RESET: int = 0x800DD69C ## the interrupt mask E7A0 saved
const DEADLINE: int = 0x800DD6A0 ## D_800DD6A0 = VSync(-1) + 0xF0 (EA18), D_800DD6A4 = 0
const CLEAR_LIST: int = 0x800E6120 ## ClearImage's DMA list (12 words)
const CLEAR_TILE_ENV: int = 0x800E6140 ## the second node of the unaligned ClearImage list
const GP1_SHADOW: int = 0x800E6160 ## 256 bytes: the last byte written for each GP1 command number
const GS_CUR_BUFFER: int = 0x80129628 ## s16 D_80129628 (libgs GsCurDrawBuffer index)
const GS_CLEAR_PACKET: int = 0x800E6100 ## 16-byte BG1 clear packets per buffer (func_8009824C)
const GS_BUF_X: int = 0x80129724 ## s16[2] D_80129724
const GS_BUF_Y: int = 0x80129728 ## s16[2] D_80129728
const GS_CLEAR_W: int = 0x801296A8 ## u16
const GS_CLEAR_H: int = 0x801296AC ## u32/u16
const GS_CLEAR_FLAG: int = 0x801296A1 ## u8

const VSYNC_COUNTER: int = 0x800E35E0 ## D_800E35E0: the counter VSync(-1) returns
const DEADLINE_FRAMES: int = 0xF0

## (length byte, code byte) of the primitive initialisers: asm/libgpu.s, one two-store function each.
## Names: the libgpu SetPoly*/SetSprt*/SetTile*/SetLine* functions, identified by header length and GP0 code
## (POLY_F3 0x20 len 4, POLY_FT3 0x24 len 7, POLY_G3 0x30 len 6, POLY_GT3 0x34 len 9, POLY_F4 0x28 len 5,
## POLY_FT4 0x2C len 9, POLY_G4 0x38 len 8, POLY_GT4 0x3C len 12, SPRT_8 0x74, SPRT_16 0x7C, SPRT 0x64,
## TILE_1 0x68, TILE_8 0x70, TILE_16 0x78, TILE 0x60, LINE_F2 0x40, LINE_G2 0x50, LINE_F3 0x48, LINE_G3 0x58,
## LINE_F4 0x4C, LINE_G4 0x5C), psx-spx GP0 command table.
const INIT_LEN_CODE: Dictionary = {
	"func_8009EFDC": [4, 0x20], "func_8009EFF0": [7, 0x24], "func_8009F004": [6, 0x30], "func_8009F018": [9, 0x34],
	"func_8009F02C": [5, 0x28], "func_8009F040": [9, 0x2C], "func_8009F054": [8, 0x38], "func_8009F068": [12, 0x3C],
	"func_8009F07C": [3, 0x74], "func_8009F090": [3, 0x7C], "func_8009F0A4": [4, 0x64], "func_8009F0B8": [2, 0x68],
	"func_8009F0CC": [2, 0x70], "func_8009F0E0": [2, 0x78], "func_8009F0F4": [3, 0x60], "func_8009F108": [3, 0x40],
	"func_8009F11C": [4, 0x50],
}
## Polylines also write the 0x55555555 terminator word: name -> [length, code, terminator offset].
const INIT_LINE_POLY: Dictionary = {
	"func_8009F130": [5, 0x48, 0x14], "func_8009F150": [7, 0x58, 0x1C], "func_8009F170": [6, 0x4C, 0x18],
	"func_8009F190": [9, 0x5C, 0x24],
}

var engine # GameEngine

func _m() -> RefCounted:
	return engine.memory

func _refuse(function_name: String, reason: String) -> void:
	engine.refuse(function_name, reason)

# ---- primitives and ordering tables

## AddPrim (asm/libgpu.s 0x8009EF2C): link `p` in front of the node `ot` points to.
func AddPrim(ot: int, p: int) -> void:
	var m := _m()
	var p_tag: int = m.u32(p)
	var ot_tag: int = m.u32(ot)
	m.write_u32(p, (p_tag & 0xFF000000) | (ot_tag & 0xFFFFFF))
	m.write_u32(ot, (m.u32(ot) & 0xFF000000) | (p & 0xFFFFFF))

## AddPrims: link the chain `first`..`last` in front of the node `ot` points to.
func AddPrims(ot: int, first: int, last: int) -> void:
	var m := _m()
	var last_tag: int = m.u32(last)
	var ot_tag: int = m.u32(ot)
	m.write_u32(last, (last_tag & 0xFF000000) | (ot_tag & 0xFFFFFF))
	m.write_u32(ot, (m.u32(ot) & 0xFF000000) | (first & 0xFFFFFF))

## SetSemiTrans: bit 1 of the GP0 code byte (p + 7).
func SetSemiTrans(p: int, on: int) -> void:
	var m := _m()
	var code: int = m.u8(p + 7)
	m.write_u8(p + 7, (code | 2) if on != 0 else (code & 0xFD))

## SetShadeTex: bit 0 of the GP0 code byte (p + 7) (1 = no shading/texture modulation).
func SetShadeTex(p: int, on: int) -> void:
	var m := _m()
	var code: int = m.u8(p + 7)
	m.write_u8(p + 7, (code | 1) if on != 0 else (code & 0xFE))

func _init_prim(p: int, name: String) -> void:
	var row: Array = INIT_LEN_CODE[name] if INIT_LEN_CODE.has(name) else INIT_LINE_POLY[name]
	var m := _m()
	m.write_u8(p + 3, row[0])
	m.write_u8(p + 7, row[1])
	if row.size() > 2:
		m.write_u32(p + row[2], 0x55555555)

func func_8009EFDC(p: int) -> void: _init_prim(p, "func_8009EFDC") # SetPolyF3
func func_8009EFF0(p: int) -> void: _init_prim(p, "func_8009EFF0") # SetPolyFT3
func func_8009F004(p: int) -> void: _init_prim(p, "func_8009F004") # SetPolyG3
func func_8009F018(p: int) -> void: _init_prim(p, "func_8009F018") # SetPolyGT3
func func_8009F02C(p: int) -> void: _init_prim(p, "func_8009F02C") # SetPolyF4
func func_8009F040(p: int) -> void: _init_prim(p, "func_8009F040") # SetPolyFT4
func func_8009F054(p: int) -> void: _init_prim(p, "func_8009F054") # SetPolyG4
func func_8009F068(p: int) -> void: _init_prim(p, "func_8009F068") # SetPolyGT4
func func_8009F07C(p: int) -> void: _init_prim(p, "func_8009F07C") # SetSprt8
func func_8009F090(p: int) -> void: _init_prim(p, "func_8009F090") # SetSprt16
func func_8009F0A4(p: int) -> void: _init_prim(p, "func_8009F0A4") # SetSprt
func func_8009F0B8(p: int) -> void: _init_prim(p, "func_8009F0B8") # SetTile1
func func_8009F0CC(p: int) -> void: _init_prim(p, "func_8009F0CC") # SetTile8
func func_8009F0E0(p: int) -> void: _init_prim(p, "func_8009F0E0") # SetTile16
func func_8009F0F4(p: int) -> void: _init_prim(p, "func_8009F0F4") # SetTile
func func_8009F108(p: int) -> void: _init_prim(p, "func_8009F108") # SetLineF2
func func_8009F11C(p: int) -> void: _init_prim(p, "func_8009F11C") # SetLineG2
func func_8009F130(p: int) -> void: _init_prim(p, "func_8009F130") # SetLineF3
func func_8009F150(p: int) -> void: _init_prim(p, "func_8009F150") # SetLineG3
func func_8009F170(p: int) -> void: _init_prim(p, "func_8009F170") # SetLineF4
func func_8009F190(p: int) -> void: _init_prim(p, "func_8009F190") # SetLineG4

## GetTPage (0x8009ECB0): the texture-page word. The layout depends on the GPU type the library probed
## (GetGraphType, byte 0x800DD588): types 1 and 2 use the wider fields.
func func_8009ECB0(tp: int, abr: int, x: int, y: int) -> int:
	var m := _m()
	var graph: int = m.u8(GRAPH_TYPE)
	if graph == 1 or graph == 2:
		return ((tp & 3) << 9) | ((abr & 3) << 7) | ((y & 0x300) >> 3) | ((x & 0x3FF) >> 6)
	return ((tp & 3) << 7) | ((abr & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2)

## GetClut (0x8009ED74).
func func_8009ED74(x: int, y: int) -> int:
	return (((x >> 4) & 0x3F) | (y << 6)) & 0xFFFF

## The texture-window command word of SetDrawMode (0x8009D7C4); 0 for a null window.
func _window_word(tw: int) -> int:
	if tw == 0:
		return 0
	var m := _m()
	var x: int = m.u8(tw) >> 3
	var w: int = ((-m.s16(tw + 4)) & 0xFF) >> 3
	var y: int = m.u8(tw + 2) >> 3
	var h: int = ((-m.s16(tw + 6)) & 0xFF) >> 3
	return 0xE2000000 | (x << 10) | (y << 15) | (h << 5) | w

## The E1 (draw mode) word (get_mode, 0x8009D4F8).
func _mode_word(dfe: int, dtd: int, tpage: int) -> int:
	var graph: int = _m().u8(GRAPH_TYPE)
	var word: int
	var low: int
	if graph == 1 or graph == 2:
		word = 0xE1000800 if dtd != 0 else 0xE1000000
		low = (tpage & 0x27FF) | (0x1000 if dfe != 0 else 0)
	else:
		word = 0xE1000200 if dtd != 0 else 0xE1000000
		low = (tpage & 0x9FF) | (0x400 if dfe != 0 else 0)
	return word | low

## SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw) (0x8009D294).
func func_8009D294(p: int, dfe: int, dtd: int, tpage: int, tw: int) -> void:
	var m := _m()
	m.write_u8(p + 3, 2)
	m.write_u32(p + 4, _mode_word(dfe, dtd, tpage & 0xFFFF))
	m.write_u32(p + 8, _window_word(tw))

## ClearOTag(ot, n) (0x8009C9F8): link the n entries forward, the last one to the terminator node.
## Returns the address of the last entry.
func func_8009C9F8(ot: int, n: int) -> int:
	var m := _m()
	var count: int = n - 1
	var at: int = ot
	while count != 0:
		m.write_u8(at + 3, 0)
		m.write_u32(at, (m.u32(at) & 0xFF000000) | ((at + 4) & 0xFFFFFF))
		count = Math.s32(count - 1)
		at += 4
		if at > ot + 0x400000:
			_refuse("ClearOTag", "ordering table of %d entries is outside RAM" % n)
			return at
	m.write_u32(at, OT_TERMINATOR & 0xFFFFFF)
	return at

## ClearOTagR(ot, n) (0x8009CAAC): the reverse table. The DMA6 (OTC) fill links entry k to entry k-1 and
## ends in 0x00FFFFFF; the library then points entry 0 at the terminator node.
func func_8009CAAC(ot: int, n: int) -> int:
	var m := _m()
	_deadline_set()
	var words: int = Math.s32(n)
	if words > 0:
		var top: int = ot + words * 4 - 4
		for k: int in words:
			var at: int = top - k * 4
			m.write_u32(at, ((at - 4) & 0xFFFFFF) if k < words - 1 else 0xFFFFFF)
	m.write_u32(ot, OT_TERMINATOR & 0xFFFFFF)
	return ot

# ---- the library's queue (func_8009E200) and its hardware

## func_8009EA18: the draw-timeout deadline = VSync(-1) + 0xF0, counter reset.
func _deadline_set() -> void:
	var m := _m()
	m.write_u32(DEADLINE, Math.add32(m.u32(VSYNC_COUNTER), DEADLINE_FRAMES))
	m.write_u32(DEADLINE + 4, 0)

## True when the library state is the empty-queue fast path: nothing queued, no draw-sync callback.
func _idle(function_name: String) -> bool:
	var m := _m()
	if m.u32(QUEUE_HEAD) != m.u32(QUEUE_TAIL):
		_refuse(function_name, "the library's drawing queue holds jobs (0x800DD68C != 0x800DD690): asynchronous drawing is not modelled")
		return false
	if m.u32(SYNC_CALLBACK) != 0:
		_refuse(function_name, "a DrawSyncCallback is set (0x800DD594): its interrupt is not modelled")
		return false
	return true

## func_8009E200 (queue a job; the fast path runs it at once): records the call in 0x800DD67C.. and returns.
## `fn` is the driver function address the library queues; `run` is what that driver does to the GPU.
func _queue_fast(function_name: String, fn: int, address: int, param: int) -> void:
	var m := _m()
	_deadline_set()
	var sys = engine.modules["sdk_etc"]
	var mask: int = sys.irq_mask_swap(0)
	m.write_u32(QUEUE_ACTIVE, 1)
	m.write_u32(SAVED_MASK_QUEUE, mask)
	m.write_u32(LAST_FN, fn)
	m.write_u32(LAST_FN + 4, address)
	m.write_u32(LAST_FN + 8, param)
	sys.irq_mask_swap(m.u32(SAVED_MASK_QUEUE))

func _renderer_ok(function_name: String) -> bool:
	if engine.halted():
		return false
	if not engine.renderer_ready():
		_refuse(function_name, "the generic renderer is not available (%s)" % engine.renderer.last_error)
		return false
	return true

## The GP0 list send of func_8009E168 (DMA2 linked list): the renderer walks the list at `address`.
func _send_list(function_name: String, address: int) -> void:
	if not _renderer_ok(function_name):
		return
	var result: Dictionary = engine.renderer.draw_ordering_table_from_memory(_m(), address)
	if not str(result.get("error", "")).is_empty():
		_refuse(function_name, "renderer: " + str(result.error))

## cw (func_8009E0E8): a GP1 word. The library keeps the last byte sent per command number.
func _cw(function_name: String, word: int) -> void:
	if not _renderer_ok(function_name):
		return
	engine.renderer.gp1(word)
	_m().write_u8(GP1_SHADOW + ((word >> 24) & 0xFF), word & 0xFF)

## DrawOTag(ot) (0x8009CB9C): queue E168 (the DMA list send) with the table address.
func func_8009CB9C(ot: int) -> void:
	if not _idle("DrawOTag") or not _renderer_ok("DrawOTag"):
		return
	_queue_fast("DrawOTag", 0x8009E168, ot, 0)
	_send_list("DrawOTag", ot)

## DrawPrim(p) (0x8009CB3C): DrawSync(0), then the primitive's words go to GP0 through the FIFO (func_8009E11C).
func func_8009CB3C(p: int) -> void:
	var m := _m()
	var words: int = m.u8(p + 3)
	if func_8009C674(0) != 0 or not _renderer_ok("DrawPrim"):
		return
	var data := PackedByteArray()
	data.append_array(m.read_bytes(p + 4, words * 4))
	engine.renderer.gp1(0x04000000)
	var error: String = engine.renderer.gp0_stream(data)
	if not error.is_empty():
		_refuse("DrawPrim", "renderer: " + error)

## DrawSync(mode) (0x8009C674 -> driver 0x8009E8D4). The renderer finishes a list before DrawOTag returns, so
## the GPU and the queue are idle at every call: DrawSync(0) returns 0 after refreshing the timeout words;
## DrawSync(n != 0) returns the queued-job count, 0. A non-empty queue refuses (jobs the model never creates).
func func_8009C674(mode: int) -> int:
	var m := _m()
	if m.u32(QUEUE_HEAD) != m.u32(QUEUE_TAIL):
		_refuse("DrawSync", "the library's drawing queue holds jobs (0x800DD68C != 0x800DD690): asynchronous drawing is not modelled")
		return 0
	if mode == 0:
		_deadline_set()
	return 0

# ---- CPU/VRAM transfers

## The rectangle clamp of the transfer drivers (0x8009D9FC/0x8009DBFC/0x8009DE50): width and height are
## limited to the library's screen size - 1 and at least 1; written back into the user's RECT.
func _clamp_rect(rect: int) -> void:
	var m := _m()
	var w: int = m.s16(rect + 4)
	if w > 0:
		w = mini(w, m.s16(SCREEN_W) - 1)
	else:
		w = 1
	m.write_u16(rect + 4, w)
	var h: int = m.s16(rect + 6)
	if h > 0:
		h = mini(h, m.s16(SCREEN_H) - 1)
	else:
		h = 1
	m.write_u16(rect + 6, h)

## LoadImage(rect, buf) (0x8009C884 -> driver 0x8009DBFC).
func func_8009C884(rect: int, buf: int) -> void:
	if not _idle("LoadImage") or not _renderer_ok("LoadImage"):
		return
	var m := _m()
	_queue_fast("LoadImage", 0x8009DBFC, rect, buf)
	_deadline_set() # the driver calls func_8009EA18 first
	_clamp_rect(rect)
	var w: int = m.s16(rect + 4)
	var h: int = m.s16(rect + 6)
	var pixels: int = w * h
	if (pixels + 1) / 2 <= 0:
		return
	var data: PackedByteArray = m.read_bytes(buf, pixels * 2)
	var error: String = engine.renderer.load_image(Rect2i(m.s16(rect), m.s16(rect + 2), w, h), data)
	if not error.is_empty():
		_refuse("LoadImage", "renderer: " + error)

## StoreImage(rect, buf) (0x8009C8E0 -> driver 0x8009DE50): VRAM to RAM, the same clamp.
func func_8009C8E0(rect: int, buf: int) -> void:
	if not _idle("StoreImage") or not _renderer_ok("StoreImage"):
		return
	var m := _m()
	_queue_fast("StoreImage", 0x8009DE50, rect, buf)
	_deadline_set()
	_clamp_rect(rect)
	var w: int = m.s16(rect + 4)
	var h: int = m.s16(rect + 6)
	var pixels: int = w * h
	var data: PackedByteArray = engine.renderer.store_image(Rect2i(m.s16(rect), m.s16(rect + 2), w, h))
	if data.size() != pixels * 2:
		_refuse("StoreImage", "renderer: " + engine.renderer.last_error)
		return
	if pixels & 1:
		# the GPU sends (w*h+1)/2 whole words; what its last halfword holds for an odd pixel count is not specified
		_refuse("StoreImage", "StoreImage of an odd number of pixels (%d x %d): the padding halfword the GPU sends is not specified" % [w, h])
		return
	m.write_bytes(buf, data)

## MoveImage(rect, x, y) (0x8009C93C): the 5-word copy list at 0x800DD630 (words 0 and 1 are static data of the
## library: the node header and the 0x80000000 command), sent as a DMA list.
func func_8009C93C(rect: int, x: int, y: int) -> void:
	var m := _m()
	if m.s16(rect + 4) == 0 or m.s16(rect + 6) == 0:
		return
	if not _idle("MoveImage") or not _renderer_ok("MoveImage"):
		return
	m.write_u32(MOVE_LIST + 8, m.u32(rect))
	m.write_u32(MOVE_LIST + 12, ((y << 16) | (x & 0xFFFF)) & 0xFFFFFFFF)
	m.write_u32(MOVE_LIST + 16, m.u32(rect + 4))
	_queue_fast("MoveImage", 0x8009E168, MOVE_LIST, 0)
	_send_list("MoveImage", MOVE_LIST)

## ClearImage(rect, r, g, b) (0x8009C7F8 -> driver 0x8009D9FC): the clamped fill list at 0x800E6120.
func func_8009C7F8(rect: int, r: int, g: int, b: int) -> void:
	if not _idle("ClearImage") or not _renderer_ok("ClearImage"):
		return
	var m := _m()
	var color: int = (b & 0xFF) << 16 | (g & 0xFF) << 8 | (r & 0xFF)
	_queue_fast("ClearImage", 0x8009D9FC, rect, color)
	_deadline_set()
	_clamp_rect(rect)
	var x: int = m.u16(rect)
	var y: int = m.u16(rect + 2)
	var w: int = m.u16(rect + 4)
	var h: int = m.u16(rect + 6)
	var xy: int = m.u32(rect)
	var wh: int = m.u32(rect + 4)
	if (x & 0x3F) == 0 and (w & 0x3F) == 0:
		# aligned: one fill (GP0 02h), after E6 (mask bits off)
		m.write_u32(CLEAR_LIST, 0x04FFFFFF)
		m.write_u32(CLEAR_LIST + 4, 0xE6000000)
		m.write_u32(CLEAR_LIST + 8, 0x02000000 | color)
		m.write_u32(CLEAR_LIST + 12, xy)
		m.write_u32(CLEAR_LIST + 16, wh)
	else:
		# unaligned: a TILE with the draw area opened to the whole VRAM, then the area restored
		m.write_u32(CLEAR_LIST, 0x07000000 | (CLEAR_TILE_ENV & 0xFFFFFF))
		m.write_u32(CLEAR_LIST + 0x10, 0xE6000000)
		m.write_u32(CLEAR_LIST + 4, 0xE3000000)
		m.write_u32(CLEAR_LIST + 0xC, 0xE5000000)
		m.write_u32(CLEAR_LIST + 8, 0xE4FFFFFF)
		m.write_u32(CLEAR_LIST + 0x14, 0x60000000 | color)
		m.write_u32(CLEAR_LIST + 0x18, xy)
		m.write_u32(CLEAR_LIST + 0x1C, wh)
		m.write_u32(CLEAR_TILE_ENV, 0x03FFFFFF)
		# the library reads the draw area and offset back from the GPU (GP1(10h) information 3, 4, 5) and restores
		# them. They are what the last PutDrawEnv sent, and the library keeps that DR_ENV packet in the DRAWENV copy
		# (0x800DD5B8: E3 area start, E4 area end, E5 offset), so the restore words are read from there; the
		# renderer's own environment is not consulted (it is not part of the RAM image a capture carries).
		m.write_u32(CLEAR_TILE_ENV + 4, 0xE3000000 | (m.u32(DRAWENV_COPY + 0x20) & 0xFFFFFF))
		m.write_u32(CLEAR_TILE_ENV + 8, 0xE4000000 | (m.u32(DRAWENV_COPY + 0x24) & 0xFFFFFF))
		m.write_u32(CLEAR_TILE_ENV + 12, 0xE5000000 | (m.u32(DRAWENV_COPY + 0x28) & 0xFFFFFF))
	_send_list("ClearImage", CLEAR_LIST)

# ---- DrawEnv / DispEnv

## The clip corner word (0x8009D5E4 for E3, 0x8009D6B0 for E4): x or y <= 0 give 1 (as the library does), values
## are limited to the screen size - 1.
func _clip_word(command: int, x: int, y: int) -> int:
	var m := _m()
	var graph: int = m.u8(GRAPH_TYPE)
	var cx: int = Math.s16(x)
	var cy: int = Math.s16(y)
	if cx <= 0:
		cx = 1
	else:
		cx = mini(cx, m.s16(SCREEN_W) - 1)
	if cy <= 0:
		cy = 1
	else:
		cy = mini(cy, m.s16(SCREEN_H) - 1)
	if graph == 1 or graph == 2:
		return (command << 24) | ((cy & 0xFFF) << 12) | (cx & 0xFFF)
	return (command << 24) | ((cy & 0x3FF) << 10) | (cx & 0x3FF)

func _offset_word(x: int, y: int) -> int:
	var graph: int = _m().u8(GRAPH_TYPE)
	if graph == 1 or graph == 2:
		return 0xE5000000 | ((y & 0xFFF) << 12) | (x & 0xFFF)
	return 0xE5000000 | ((y & 0x7FF) << 11) | (x & 0x7FF)

## func_8009D2E8: the DR_ENV packet (tag + up to 11 words) built into `packet` from the DRAWENV `env`.
func _drawenv_packet(packet: int, env: int) -> void:
	var m := _m()
	m.write_u32(packet + 4, _clip_word(0xE3, m.s16(env), m.s16(env + 2)))
	var x2: int = Math.u16(m.u16(env + 4) + m.u16(env)) - 1
	var y2: int = Math.u16(m.u16(env + 6) + m.u16(env + 2)) - 1
	m.write_u32(packet + 8, _clip_word(0xE4, Math.s16(x2), Math.s16(y2)))
	m.write_u32(packet + 0xC, _offset_word(m.s16(env + 8), m.s16(env + 0xA)))
	var mode: int = _mode_word(m.u8(env + 0x17), m.u8(env + 0x16), m.u16(env + 0x14))
	m.write_u32(packet + 0x10, mode)
	var window: int = _window_word(env + 0xC)
	m.write_u32(packet + 0x14, window)
	m.write_u32(packet + 0x18, 0xE6000000)
	var words: int = 7
	if m.u8(env + 0x18) != 0:
		# the background fill: clipped copy of the clip RECT in a local
		var rx: int = m.u16(env)
		var ry: int = m.u16(env + 2)
		var rw: int = m.s16(env + 4)
		var rh: int = m.s16(env + 6)
		if rw > 0:
			rw = mini(rw, m.s16(SCREEN_W) - 1)
		else:
			rw = 1
		if rh > 0:
			rh = mini(rh, m.s16(SCREEN_H) - 1)
		else:
			rh = 1
		var color: int = (m.u8(env + 0x1A) << 8) | (m.u8(env + 0x1B) << 16) | m.u8(env + 0x19)
		if (rx & 0x3F) != 0 or (rw & 0x3F) != 0:
			rx = Math.u16(rx - m.u16(env + 8))
			ry = Math.u16(ry - m.u16(env + 0xA))
			m.write_u32(packet + words * 4, 0x60000000 | color)
			words += 1
			m.write_u32(packet + words * 4, (ry << 16) | rx)
			words += 1
			m.write_u32(packet + words * 4, ((rh & 0xFFFF) << 16) | (rw & 0xFFFF))
			words += 1
		else:
			m.write_u32(packet + words * 4, 0x02000000 | color)
			words += 1
			m.write_u32(packet + words * 4, (ry << 16) | rx)
			words += 1
			m.write_u32(packet + words * 4, ((rh & 0xFFFF) << 16) | (rw & 0xFFFF))
			words += 1
	m.write_u8(packet + 3, words - 1)

## PutDrawEnv(env) (0x8009CC04): builds the DR_ENV inside the DRAWENV (+0x1C), sends it as a DMA list, keeps a
## copy of the DRAWENV at 0x800DD598. Returns env.
func func_8009CC04(env: int) -> int:
	if not _idle("PutDrawEnv") or not _renderer_ok("PutDrawEnv"):
		return env
	var m := _m()
	_drawenv_packet(env + 0x1C, env)
	m.write_u32(env + 0x1C, m.u32(env + 0x1C) | 0xFFFFFF)
	_queue_fast("PutDrawEnv", 0x8009E168, env + 0x1C, 0)
	_send_list("PutDrawEnv", env + 0x1C)
	m.write_bytes(DRAWENV_COPY, m.read_bytes(env, 0x5C))
	return env

## The display start x for the GPU types 1 and 2 (0x8009D844).
func _display_x(env: int) -> int:
	var m := _m()
	var graph: int = m.u8(GRAPH_TYPE)
	var reverse: int = m.u8(REVERSE)
	if graph == 1:
		if reverse != 0:
			return 0x400 - m.s16(env + 4) - m.s16(env)
		return m.s16(env)
	if graph == 2:
		if reverse != 0:
			var w: int = m.s16(env + 4)
			return 0x400 - (w / 2) - m.s16(env)
		var x: int = m.s16(env)
		return x / 2
	return m.s16(env)

## PutDispEnv(env) (0x8009CCF4): GP1 05 (display start), 06/07 (horizontal/vertical range) when the screen
## rectangle changed, 08 (display mode) when the display flags changed; stores the DISPENV copy. Returns env.
func func_8009CCF4(env: int) -> int:
	var m := _m()
	if not _renderer_ok("PutDispEnv"):
		return env
	var graph: int = m.u8(GRAPH_TYPE)
	var start: int
	if graph == 1 or graph == 2:
		start = ((m.u16(env + 2) & 0xFFF) << 12) | (Math.u16(_display_x(env)) & 0xFFF)
	else:
		start = ((m.u16(env + 2) & 0x3FF) << 10) | (m.u16(env) & 0x3FF)
	_cw("PutDispEnv", 0x05000000 | start)
	var sys = engine.modules["sdk_etc"]
	if m.u32(DISPENV_COPY + 8) != m.u32(env + 8) or m.u32(DISPENV_COPY + 0xC) != m.u32(env + 0xC):
		var mode: int = sys.video_mode()
		m.write_u8(env + 0x12, mode)
		var x1: int = m.s16(env + 8) * 10 + 0x260
		var screen_y: int = m.s16(env + 0xA)
		var y1: int = screen_y + (0x13 if mode != 0 else 0x10)
		var screen_w: int = m.s16(env + 0xC)
		var x2: int = x1 + screen_w * 10 if screen_w != 0 else x1 + 0xA00
		var screen_h: int = m.s16(env + 0xE)
		var y2: int = y1 + (screen_h if screen_h != 0 else 0xF0)
		if x1 < 0x1F4:
			x1 = 0x1F4
		elif x1 >= 0xCDB:
			x1 = 0xCDA
		if x2 < x1 + 0x50:
			x2 = x1 + 0x50
		elif x2 >= 0xCDB:
			x2 = 0xCDA
		if y1 < 0:
			y1 = 0
		elif mode == 0:
			if y1 >= 0xFF:
				y1 = 0xFE
		elif y1 >= 0x137:
			y1 = 0x136
		if y2 < y1 + 1:
			y2 = y1 + 1
		elif mode == 0:
			if y2 >= 0x101:
				y2 = 0x100
		elif y2 >= 0x139:
			y2 = 0x138
		_cw("PutDispEnv", 0x06000000 | (x1 & 0xFFF) | ((x2 & 0xFFF) << 12))
		_cw("PutDispEnv", 0x07000000 | (y1 & 0x3FF) | ((y2 & 0x3FF) << 10))
	if m.u32(DISPENV_COPY + 0x10) != m.u32(env + 0x10) or m.u32(DISPENV_COPY) != m.u32(env) or m.u32(DISPENV_COPY + 4) != m.u32(env + 4):
		var mode2: int = sys.video_mode()
		m.write_u8(env + 0x12, mode2)
		var display: int = 0x08000000
		if mode2 == 1:
			display |= 0x08
		if m.u8(env + 0x11) != 0:
			display |= 0x10
		if m.u8(env + 0x10) != 0:
			display |= 0x20
		if m.u8(REVERSE) != 0:
			display |= 0x80
		var w: int = m.s16(env + 4)
		if w < 0x119:
			pass
		elif w < 0x161:
			display |= 0x01
		elif w < 0x191:
			display |= 0x40
		elif w < 0x231:
			display |= 0x02
		else:
			display |= 0x03
		var h: int = m.s16(env + 6)
		if h >= (0x121 if m.u8(env + 0x12) != 0 else 0x101):
			display |= 0x24
		_cw("PutDispEnv", display)
	m.write_bytes(DISPENV_COPY, m.read_bytes(env, 0x14))
	return env

## SetDispMask(mask) (0x8009C5E0): GP1(03h) = display on/off; mask 0 also invalidates the stored DISPENV.
func func_8009C5E0(mask: int) -> void:
	var m := _m()
	if not _renderer_ok("SetDispMask"):
		return
	if mask == 0:
		m.fill(DEBUG_LEVEL + 0x6A, 0x14, 0xFF)
	_cw("SetDispMask", 0x03000000 if mask != 0 else 0x03000001)

## ResetGraph(mode) (0x8009C210). Only the reset-the-queue form (mode & 7 != 0) is modelled: mode 0 is the full
## boot reset (GPU probe, ResetCallback, driver tables), which the engine never repeats mid-run, so it refuses.
func func_8009C210(mode: int) -> int:
	var m := _m()
	if (mode & 7) == 0:
		_refuse("ResetGraph", "ResetGraph(%d): the full GPU reset (probe, ResetCallback, driver table) is a boot-time call and is not modelled" % mode)
		return 0
	if not _renderer_ok("ResetGraph"):
		return 0
	var sys = engine.modules["sdk_etc"]
	var mask: int = sys.irq_mask_swap(0)
	m.write_u32(QUEUE_TAIL, 0)
	m.write_u32(SAVED_MASK_RESET, mask)
	m.write_u32(QUEUE_HEAD, 0)
	# driver 0x8009E7A0 case 1: DMA2 stop (0x401), DPCR bit 11, GP1(02h) acknowledge, GP1(01h) reset the FIFO
	engine.renderer.gp1(0x02000000)
	engine.renderer.gp1(0x01000000)
	sys.irq_mask_swap(m.u32(SAVED_MASK_RESET))
	return 0

# ---- libgs leaves

## GsDrawOt(ot) (0x8009AD00): DrawOTag(ot->org), org at +0x10.
func func_8009AD00(ot: int) -> void:
	engine.call_c("func_8009CB9C", [_m().u32(ot + 0x10)])

## GsSortClear(r, g, b, ot) (0x8009824C): the background clear packet of the current buffer (16 bytes at
## 0x800E6100 + 16 * buffer: r g b code at +4..+6, x y w h) linked into the table.
func func_8009824C(r: int, g: int, b: int, ot: int) -> void:
	var m := _m()
	var buffer: int = m.s16(GS_CUR_BUFFER)
	var packet: int = GS_CLEAR_PACKET + buffer * 16
	m.write_u8(packet + 4, r)
	m.write_u8(packet + 5, g)
	m.write_u8(packet + 6, b)
	m.write_u16(packet + 8, m.u16(GS_BUF_X + buffer * 2))
	m.write_u16(packet + 0xE, m.u16(GS_CLEAR_W))
	m.write_u16(packet + 0xA, m.u16(GS_BUF_Y + buffer * 2))
	if m.u8(GS_CLEAR_FLAG) != 0:
		var height: int = m.s32(GS_CLEAR_H)
		var value: int = height * 3
		value = (value + ((value >> 31) & 1)) >> 1
		m.write_u16(packet + 0xC, value)
	else:
		m.write_u16(packet + 0xC, m.u16(GS_CLEAR_H))
	engine.call_c("AddPrim", [m.u32(ot + 0x10), packet])
