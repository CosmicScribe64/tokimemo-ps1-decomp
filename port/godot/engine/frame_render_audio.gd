extends RefCounted
## Port of src/main/frame_render_audio.c (original object 0x80043510-0x800451D0, T-39311 on the T-39301 engine).
## draw_frame (the per-frame 2D/3D render and present), the palette/texture/image upload queues and their
## flush to VRAM, the LoadSquare/MoveSquare/StoreSquare rectangle wrappers, the debug time bars, and the sound
## bank loading and BGM state. The C builds nothing itself here: it calls libgpu/libgte/libetc/sound-driver
## functions (func_80098680, func_80098B80, func_80098D00, func_80099540 ... func_8009AD00, func_8009C674,
## func_800AD950 = VSync, func_8009C884/8009C8E0/8009C93C, audio_command_submit, audio_bgm_bank_ready, ...),
## which refuse by name until their owners are ported (the harness replays them from captures). The wait
## VSync(0) / VSync(2) of draw_frame is therefore a refusal naming func_800AD950. Rectangles the C keeps as
## stack locals live at their original frame offsets (LoadSquare/MoveSquare/StoreSquare rect at +0x28,
## vram_palette_row_copy at +0x30). Contract: wiki/engine-porting-contract.md.
## Tests: tests/engine/frame_render_audio_test.gd, config/port/engine-diff/frame_render_audio.json.

const Math = preload("res://core/native_math.gd")
const Layout = preload("res://core/game_layout.gd")

const C_FILE: String = "src/main/frame_render_audio.c"
const C_FUNCTIONS: Array[String] = [
	"draw_frame", "draw2d3d", "back_clear_switch", "load_palette", "load_csr_ab", "load_csr_tp", "csr_load_vram",
	"palette_load_vram", "LoadSquare", "MoveSquare", "StoreSquare", "func_80044434", "draw_time_bars",
	"vram_palette_row_copy", "audio_command", "audio_bgm_stop", "audio_bgm_is_playing", "audio_bgm_current_set",
	"audio_bank_state_reset", "audio_bank_load_start", "audio_bank_transfer_poll", "audio_bank_transfer_start",
	"audio_bank_load_poll", "audio_bank_transfer_done", "audio_bank_start", "audio_bgm_play_short",
]
const C_UNPORTED: Array[String] = []
## load_palette, load_csr_ab and load_csr_tp are K&R (their u8/u16 parameters are narrowed on entry).
## audio_bank_start and audio_bgm_play_short return -1 on their error path and otherwise fall off the end,
## where the original's v0 is the result of the last audio_command call (1): the port returns that 1.
const C_SIGNATURES: Dictionary = {
	"draw_frame": {"args": ["s32"], "ret": "void"},
	"draw2d3d": {"args": ["u8", "u8"], "ret": "void"},
	"back_clear_switch": {"args": ["s32"], "ret": "void"},
	"load_palette": {"args": ["s32", "u8", "u8", "u8", "u8"], "ret": "s32"},
	"load_csr_ab": {"args": ["s32", "u8", "u16", "u16", "u16", "u16"], "ret": "s32"},
	"load_csr_tp": {"args": ["s32", "u8", "u8", "u16", "u16", "u16", "u16"], "ret": "s32"},
	"csr_load_vram": {"args": ["s32"], "ret": "void"},
	"palette_load_vram": {"args": ["s32"], "ret": "void"},
	"LoadSquare": {"args": ["u16", "u16", "u16", "u16", "ptr"], "ret": "void"},
	"MoveSquare": {"args": ["u16", "u16", "u16", "u16", "u16", "u16"], "ret": "void"},
	"StoreSquare": {"args": ["u16", "u16", "u16", "u16", "ptr"], "ret": "void"},
	"func_80044434": {"args": [], "ret": "void"},
	"draw_time_bars": {"args": [], "ret": "void"},
	"vram_palette_row_copy": {"args": ["s32", "s32", "s32"], "ret": "void"},
	"audio_command": {"args": ["s32"], "ret": "s32"},
	"audio_bgm_stop": {"args": ["s32"], "ret": "s32"},
	"audio_bgm_is_playing": {"args": [], "ret": "u8"},
	"audio_bgm_current_set": {"args": [], "ret": "u8"},
	"audio_bank_state_reset": {"args": [], "ret": "void"},
	"audio_bank_load_start": {"args": ["s32", "s32", "s32", "s32", "s32", "s32"], "ret": "void"},
	"audio_bank_transfer_poll": {"args": [], "ret": "s32"},
	"audio_bank_transfer_start": {"args": [], "ret": "s32"},
	"audio_bank_load_poll": {"args": [], "ret": "s32"},
	"audio_bank_transfer_done": {"args": ["s32"], "ret": "s32"},
	"audio_bank_start": {"args": ["s32", "s32"], "ret": "s32", "ret_defined": [-1]},
	"audio_bgm_play_short": {"args": ["s32", "s32"], "ret": "s32", "ret_defined": [-1]},
}

## Data symbols the generated layout lacks (u8 each; frame_render_audio.c, config/symbol_addrs*.txt has no
## entry; addresses from the object's .data order in src/main/frame_render_audio.c): reported in the ticket.
const D_800B3D28: int = 0x800B3D28
const D_800B3D2C: int = 0x800B3D2C
const D_800B3D4C: int = 0x800B3D4C
const D_800B3D50: int = 0x800B3D50
## draw_frame's frame is 0xA8 bytes; sp50 (8 words) and sp70 (12 words) are its libgte work areas.
const FRAME_SP50: int = 0x50
const FRAME_SP70: int = 0x70
## The two sound banks' headers (first word & 0xFFFF == 0x40 when loaded).
const BANK_A: int = 0x8001C000
const BANK_B: int = 0x8002E800
const D_80122740: int = 0x80122740
const SPRITE_QUAD_SIZE: int = 0x24
const BG_LAYER_SIZE: int = 0x24

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func _ot() -> int:
	return Math.s32(engine.memory.global_address("draw_ot_table") + engine.memory.read_global("draw_buffer") * 0x14)

func _stamp(index: int, value: int) -> void:
	engine.memory.write_global("draw_horizontal_blank_stamps", value, index)

## The per-frame render. Order as in the C: stamp 1, brightness fade, actors, the tile layer, the 64 sprite
## quads and the string and BG layers (when 2D and quads are on), the 3D model objects, and with arg0 != 0 the
## present: stamps, VRAM queue flushes, DrawSync/VSync waits, pad_update. Always ends with func_8009C210(1),
## func_8009ABF0, func_8009AC20, the optional back clear and func_8009AD00 (the ordering-table draw).
func draw_frame(arg0: int) -> void:
	var m: Object = engine.memory
	_stamp(1, engine.call_c("func_80098650"))
	engine.call_c("draw_brightness_step")
	if engine.gs_get("actor_draw_enabled") == 1:
		engine.call_c("actor_draw_all")
	if Math.s8(engine.gs_get("tile_layer_enabled")) != 0:
		engine.call_c("srn_set", [0])
	if engine.gs_get("draw_2d_enabled") != 0 and engine.gs_get("quads_enabled") != 0:
		var rec: int = m.global_address("sprite_quads")
		var quad_layer: int = engine.gs_address("quad_layer")
		for i: int in 0x40:
			if engine.halted():
				return
			var v: int = m.u8(quad_layer + i)
			if v & 0x80:
				engine.call_c("func_80098680", [rec, _ot(), Math.u8(v - 1) & 0x7F])
			else:
				engine.call_c("func_80098B80", [rec, _ot(), Math.u8(v - 1) & 0x7F])
			rec += SPRITE_QUAD_SIZE
		engine.call_c("disp_string")
		if engine.gs_get("quads_enabled") != 0:
			for i: int in 2:
				if engine.halted():
					return
				var order: int = m.u8(engine.gs_address("bg_layer_order") + i)
				if order < 0x80:
					engine.call_c("func_80098D00", [m.global_address("bg_layers") + i * BG_LAYER_SIZE, _ot(), Math.u16(order - 1)])
	if engine.gs_get("draw_3d_enabled") != 0:
		engine.call_c("func_80099540", [D_80122740])
		var obj: int = m.global_address("model_objects")
		var sp50: int = engine.frame_base() + FRAME_SP50
		var sp70: int = engine.frame_base() + FRAME_SP70
		while true: # do { ... obj += 4 words } while (obj != &D_80122740)
			if engine.halted():
				return
			engine.call_c("func_80099B10", [m.u32(obj + 4), sp50, sp70])
			engine.call_c("func_80099F04", [sp50])
			engine.call_c("func_8009A200", [m.u32(obj + 4), sp70])
			engine.call_c("func_80099ED8", [sp70])
			engine.call_c("func_8009A4D0", [obj, _ot(), 6, 0x1F800000])
			obj += 16
			if obj == D_80122740:
				break
		engine.call_c("view_models_update_draw")
	if arg0 != 0:
		_stamp(2, engine.call_c("func_80098650"))
		engine.call_c("csr_load_vram", [0])
		engine.call_c("palette_load_vram", [0])
		var mode: int = engine.gs_get("game_mode")
		var stage: int = engine.gs_get("mode_stage")
		if mode != 0x90 or stage < 10 or stage == 12:
			engine.call_c("func_8009C674", [0])
		_stamp(3, engine.call_c("func_80098650"))
		if engine.gs_get("game_mode") == 0x90 and engine.gs_get("mode_stage") >= 10:
			engine.call_c("func_800AD950", [2])
			engine.call_c("func_8009C210", [1])
		elif engine.call_c("func_80098650") < engine.gs_get("frame_scanline_limit"):
			engine.call_c("func_800AD950", [0])
		_stamp(0, engine.call_c("func_80098650"))
		engine.call_c("pad_update")
	engine.call_c("func_8009C210", [1])
	engine.call_c("func_8009ABF0")
	engine.call_c("func_8009AC20")
	if engine.gs_get("back_clear") != 0:
		engine.call_c("func_8009824C", [engine.gs_get("clear_color_r"), engine.gs_get("clear_color_g"), engine.gs_get("clear_color_b"), _ot()])
	engine.call_c("func_8009AD00", [_ot()])

func draw2d3d(arg0: int, arg1: int) -> void:
	engine.gs_set("draw_2d_enabled", Math.u8(arg0))
	engine.gs_set("draw_3d_enabled", Math.u8(arg1))

func back_clear_switch(arg0: int) -> void:
	engine.gs_set("back_clear", 1 if arg0 != 0 else 0)

## Queues a palette upload: entry {source, unk_04..unk_07} at palette_queue[count]; returns 1, or 0 when the
## 32-entry queue is full. K&R: the u8 parameters are narrowed.
func load_palette(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> int:
	var m: Object = engine.memory
	var index: int = engine.gs_get("palette_queue_count")
	if index < 32:
		var entry: int = engine.gs_address("palette_queue") + index * 8
		m.write_s32(entry, arg0)
		m.write_u8(entry + 4, arg1)
		m.write_u8(entry + 5, arg2)
		m.write_u8(entry + 6, arg3)
		m.write_u8(entry + 7, arg4)
		engine.gs_set("palette_queue_count", index + 1)
		return 1
	return 0

## Queues an image transfer (slot arg1, rectangle arg2..arg5) into csr_queue[count]; 1, or 0 when full.
func load_csr_ab(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int) -> int:
	var m: Object = engine.memory
	var index: int = engine.gs_get("csr_queue_count")
	if index < 32:
		var entry: int = engine.gs_address("csr_queue") + index * 16
		m.write_s32(entry, arg0)
		m.write_u8(entry + 4, arg1)
		m.write_u8(entry + 5, 0)
		m.write_u8(entry + 6, 0)
		m.write_u8(entry + 7, 0)
		m.write_u16(entry + 8, arg2)
		m.write_u16(entry + 0xA, arg3)
		m.write_u16(entry + 0xC, arg4)
		m.write_u16(entry + 0xE, arg5)
		engine.gs_set("csr_queue_count", index + 1)
		return 1
	return 0

## load_csr_ab with a width shift (unk_05 = arg2) for a texture-page image; 1, or 0 when full.
func load_csr_tp(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int) -> int:
	var m: Object = engine.memory
	var index: int = engine.gs_get("csr_queue_count")
	if index < 32:
		var entry: int = engine.gs_address("csr_queue") + index * 16
		m.write_s32(entry, arg0)
		m.write_u8(entry + 4, arg1)
		m.write_u8(entry + 5, arg2)
		m.write_u8(entry + 6, 0)
		m.write_u8(entry + 7, 0)
		m.write_u16(entry + 8, arg3)
		m.write_u16(entry + 0xA, arg4)
		m.write_u16(entry + 0xC, arg5)
		m.write_u16(entry + 0xE, arg6)
		engine.gs_set("csr_queue_count", index + 1)
		return 1
	return 0

## Flushes csr_queue through LoadSquare (slot 0: the rectangle as queued; slot n: x + ((n & 0xF) << 6),
## y + ((n & 0x10) << 4), width >> unk_05) and empties it.
func csr_load_vram(_arg0: int) -> void:
	var m: Object = engine.memory
	var queue: int = engine.gs_address("csr_queue")
	var i: int = 0
	if engine.gs_get("csr_queue_count") != 0:
		while true: # do { ... i++ } while (i < (u8) count)
			if engine.halted():
				return
			var e: int = queue + i * 16
			var slot: int = m.u8(e + 4)
			if slot == 0:
				engine.call_c("LoadSquare", [m.u16(e + 8), m.u16(e + 0xA), m.u16(e + 0xC), m.u16(e + 0xE), m.u32(e)])
			else:
				var x: int = Math.u16(m.u16(e + 8) + ((slot & 0xF) << 6))
				var y: int = Math.u16(m.u16(e + 0xA) + ((slot & 0x10) * 0x10))
				var w: int = m.u16(e + 0xC) >> m.u8(e + 5)
				engine.call_c("LoadSquare", [x, y, w, m.u16(e + 0xE), m.u32(e)])
			i += 1
			if i >= engine.gs_get("csr_queue_count"):
				break
	engine.gs_set("csr_queue_count", 0)

## Uploads the queued palettes (palette_queue[0 .. count-1]) and empties the queue: unk_04 < 32 is a 256-colour
## row at (0, 0x1E0 + unk_04) whose STP bits unk_06 sets or clears (0: clear 128 words; 2: clear 120 then set 8;
## 3: set 128; 4: clear 112 then set 16; 1 and others: unchanged), otherwise a 32-colour row at x 0x100 (unk_04
## < 0x40) or 0x120. The colours start unk_07 * 128 words into the source.
func palette_load_vram(_arg0: int) -> void:
	var m: Object = engine.memory
	var queue: int = engine.gs_address("palette_queue")
	var index: int = 0
	if engine.gs_get("palette_queue_count") != 0:
		while true: # do { ... index++ } while (index < count)
			if engine.halted():
				return
			var e: int = queue + index * 8
			var source: int = m.u32(e)
			var unk_04: int = m.u8(e + 4)
			var y: int = (unk_04 & 31) + 0x1E0
			var first: int = m.u8(e + 7) << 7
			var x: int
			var width: int
			if unk_04 < 32:
				width = 0x100
				x = 0
				match m.u8(e + 6):
					0:
						_stp_clear(source, first, first + 128)
					2:
						_stp_clear(source, first, first + 120)
						_stp_set(source, first + 120, first + 128)
					3:
						_stp_set(source, first, first + 128)
					4:
						_stp_clear(source, first, first + 112)
						_stp_set(source, first + 112, first + 128)
					_:
						pass # 1 and default: empty on purpose
			else:
				width = 0x20
				x = 0x100 if unk_04 < 0x40 else 0x120
			engine.call_c("LoadSquare", [x, y, width, 1, Math.u32(m.u32(e) + first * 4)])
			index += 1
			if index >= engine.gs_get("palette_queue_count"):
				break
	engine.gs_set("palette_queue_count", 0)

## source[j] words j in [from, to): a set top STP bit (0x80000000) and a set low one (0x8000) are cleared
## through the mask 0x7FFF7FFF (the C tests and masks twice, so the result is the same).
func _stp_clear(source: int, from: int, to: int) -> void:
	var m: Object = engine.memory
	for j: int in range(from, to):
		var at: int = Math.u32(source + j * 4)
		if m.u32(at) & 0x80000000:
			m.write_u32(at, m.u32(at) & 0x7FFF7FFF)
		if m.u32(at) & 0x8000:
			m.write_u32(at, m.u32(at) & 0x7FFF7FFF)

## source[j] words j in [from, to): a nonzero colour in a half gets that half's STP bit set.
func _stp_set(source: int, from: int, to: int) -> void:
	var m: Object = engine.memory
	for j: int in range(from, to):
		var at: int = Math.u32(source + j * 4)
		if m.u32(at) & 0x7FFF0000:
			m.write_u32(at, m.u32(at) | 0x80000000)
		if m.u32(at) & 0x7FFF:
			m.write_u32(at, m.u32(at) | 0x8000)

## RECT {x, y, w, h} on the C stack (at +0x28 of the 0x30 frame), then func_8009C884(&rect, data) (LoadImage).
func LoadSquare(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> void:
	var rect: int = engine.frame_base() + 0x28
	_write_rect(rect, arg0, arg1, arg2, arg3)
	engine.call_c("func_8009C884", [rect, arg4])

func MoveSquare(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int) -> void:
	var rect: int = engine.frame_base() + 0x28
	_write_rect(rect, arg0, arg1, arg2, arg3)
	engine.call_c("func_8009C93C", [rect, Math.u16(arg4), Math.u16(arg5)])

func StoreSquare(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int) -> void:
	var rect: int = engine.frame_base() + 0x28
	_write_rect(rect, arg0, arg1, arg2, arg3)
	engine.call_c("func_8009C8E0", [rect, arg4])

func _write_rect(rect: int, x: int, y: int, w: int, h: int) -> void:
	engine.memory.write_u16(rect + 0, x)
	engine.memory.write_u16(rect + 2, y)
	engine.memory.write_u16(rect + 4, w)
	engine.memory.write_u16(rect + 6, h)

func func_80044434() -> void:
	pass

## Debug bars: four 4-pixel columns at x = 0x8C + 4 i whose height is stamp * 16 / 264 (bottom at y 0x4C) and
## whose colour steps with stamp / 264 (>= 3 red, >= 2 yellow, > 0 green, else blue), then a 16-high frame.
## RECT is passed by value: a1 = x | y << 16, a2 = w | h << 16 (see prim_fill_rect).
@warning_ignore("integer_division")
func draw_time_bars() -> void:
	var m: Object = engine.memory
	var colors: Array[int] = []
	for i: int in 4:
		var c: int = m.read_global("draw_horizontal_blank_stamps", i) / 264
		if c >= 3:
			c = 0x800000
		elif c >= 2:
			c = 0x808000
		elif c > 0:
			c = 0x8000
		else:
			c = 0x80
		colors.append(c)
	for i: int in 4:
		if engine.halted():
			return
		var x: int = 0x8C + i * 4
		var h: int = Math.s16(Math.s32(m.read_global("draw_horizontal_blank_stamps", i) * 16) / 264)
		var y: int = Math.s16(0x4C - h)
		engine.call_c("prim_fill_rect", [i + 6, (Math.u16(x)) | (Math.u16(y) << 16), 4 | (Math.u16(h) << 16), colors[i]])
	engine.call_c("prim_fill_rect", [4 + 6, 0x88 | (0x3C << 16), 4 | (16 << 16), 0x808080])

## Copies the 16 x 1 palette row (arg1 << 4, arg0 + 0x1E0) to (0x100, arg2 + 0x1E0) through func_8009C93C.
func vram_palette_row_copy(arg0: int, arg1: int, arg2: int) -> void:
	var rect: int = engine.frame_base() + 0x30
	_write_rect(rect, arg1 << 4, arg0 + 0x1E0, 0x10, 1)
	engine.call_c("func_8009C93C", [rect, 0x100, arg2 + 0x1E0])

## Submits the low 16 bits to the sound driver queue (audio_command_submit, sound_driver.c); returns 1.
func audio_command(arg0: int) -> int:
	engine.call_c("audio_command_submit", [arg0 & 0xFFFF])
	return 1

## -1 when the bank header word (first word & 0xFFFF) is not 0x40; else stops (arg0 == 0: command 0x1200) or
## starts bank arg0's current sequence, and records arg0 as playing. The C calls audio_bank_start with a third
## argument (arg0) that the callee ignores.
func audio_bgm_stop(arg0: int) -> int:
	var p: int = BANK_B if arg0 != 0 else BANK_A
	if (engine.memory.s32(p) & 0xFFFF) != 0x40:
		return -1
	if arg0 != 0:
		engine.call_c("audio_bank_start", [engine.memory.read_global("audio_bgm_set"), engine.memory.read_global("audio_bgm_sequence")])
	else:
		engine.call_c("audio_command", [0x1200])
	engine.memory.write_global("audio_bgm_playing", arg0)
	return arg0 & 0xFF

func audio_bgm_is_playing() -> int:
	return engine.memory.read_global("audio_bgm_playing")

func audio_bgm_current_set() -> int:
	return engine.memory.read_global("audio_bgm_set")

func audio_bank_state_reset() -> void:
	var m: Object = engine.memory
	m.write_global("D_800B3D54", 0, 0)
	m.write_global("D_800B3D54", 0, 1)
	m.write_global("audio_bgm_set", 0xFF)
	m.write_global("D_800B3D30", 0)
	m.write_global("D_800B3D34", 0)
	m.write_global("D_800B3D38", 0)
	m.write_global("D_800B3D3C", 0)
	m.write_u8(D_800B3D28, 0)
	m.write_u8(D_800B3D2C, 0)
	m.write_u8(D_800B3D4C, 0)
	m.write_u8(D_800B3D50, 0)
	m.write_global("audio_bgm_sequence", 0)

func _d20_sub(n: int) -> void:
	engine.memory.write_global("D_800B3D20", engine.memory.read_global("D_800B3D20") - n)

## Starts reading the sound bank files of set `a` (0: bank A, else bank B) from the CD (lba b: bank header,
## c: data, d/e/f: the second set of files) and records how many CD completions to wait for in D_800B3D20.
## A file whose lba equals the one already loaded (D_800B3D30/34 or 38/3C) is skipped and counted as done.
func audio_bank_load_start(a: int, b: int, c: int, d: int, e: int, f: int) -> void:
	var m: Object = engine.memory
	m.write_global("audio_bgm_set", 0xFF)
	m.write_u8(D_800B3D28, 0)
	m.write_u8(D_800B3D2C, 0)
	m.write_global("D_800B3D24", a)
	m.write_u8(D_800B3D4C, 0)
	m.write_u8(D_800B3D50, 1)
	m.write_global("D_800B3D20", engine.call_c("cd_done_count"))
	var first: String = "D_800B3D30" if a == 0 else "D_800B3D38"
	var second: String = "D_800B3D34" if a == 0 else "D_800B3D3C"
	var head_dest: int = 0x80010000 if a == 0 else 0x8001A000
	var data_cmd: int = 0x20 if a == 0 else 0x35
	var data_dest: int = 0x801B8000 if a == 0 else 0x801E0000
	var third_dest: int = 0x8001C000 if a == 0 else 0x8002E800
	var fourth_dest: int = 0x801C8000 if a == 0 else 0x801B8000
	var fifth_dest: int = 0x80012000 if a == 0 else 0x80016000
	engine.call_c("cd_load_destination_base_set", [0x801B8000])
	if m.read_global(first) == b:
		_d20_sub(2)
		m.write_u8(D_800B3D28, 1)
	else:
		if b != 0:
			engine.call_c("cd_read_sectors_nowait", [3, head_dest, b])
			m.write_global(first, b)
		else:
			_d20_sub(1)
			m.write_u8(D_800B3D28, 1)
		if c != 0:
			engine.call_c("cd_read_sectors_nowait", [data_cmd, data_dest, c])
		else:
			_d20_sub(1)
			m.write_u8(D_800B3D28, 1)
	if m.read_global(second) == d:
		_d20_sub(3)
		m.write_u8(D_800B3D2C, 1)
		return
	if f != 0:
		engine.call_c("cd_read_sectors_nowait", [0x25, third_dest, f])
		m.write_global(second, d)
	else:
		_d20_sub(1)
		m.write_u8(D_800B3D2C, 1)
	if e != 0:
		engine.call_c("cd_read_sectors_nowait", [0x50, fourth_dest, e])
	else:
		_d20_sub(1)
		m.write_u8(D_800B3D2C, 1)
	if d != 0:
		engine.call_c("cd_read_sectors_nowait", [6, fifth_dest, d])
		m.write_global(second, d)
		return
	_d20_sub(1)
	m.write_u8(D_800B3D2C, 1)

## True when at least 5 CD completions (cd_done_count) have passed since audio_bank_load_start.
func _transfer_ready() -> bool:
	var done: int = engine.call_c("cd_done_count")
	return Math.u32(((done - engine.memory.read_global("D_800B3D20")) + 0x100) & 0xFF) >= 5

## After the files arrived: the first call starts the SPU transfer, later ones count up to 0xC0 polls or wait
## for audio_bank_transfer_done; then the entry D_800B3D54[bank] is cleared and 1 returned, else -1.
func audio_bank_transfer_poll() -> int:
	var m: Object = engine.memory
	if _transfer_ready():
		if m.u8(D_800B3D4C) == 0:
			engine.call_c("audio_bank_transfer_start")
			m.write_u8(D_800B3D4C, m.u8(D_800B3D4C) + 1)
		else:
			m.write_u8(D_800B3D4C, m.u8(D_800B3D4C) + 1) # ++ as the value of the compare
			if m.u8(D_800B3D4C) > 0xC0 or engine.call_c("audio_bank_transfer_done", [m.read_global("D_800B3D24")]) != 0:
				m.write_global("D_800B3D54", 0, m.read_global("D_800B3D24"))
				return 1
	return -1

## Issues the SPU transfer command for the files that were loaded (bank A: 0x11 both, 0x13 or 0x14 one; bank
## B: 0x12, 0x16, 0x15), after setting the CD load destination; 0 when neither file was loaded.
func audio_bank_transfer_start() -> int:
	var m: Object = engine.memory
	var which: int = m.read_global("D_800B3D24")
	var first_done: bool = m.u8(D_800B3D28) != 0
	var second_done: bool = m.u8(D_800B3D2C) != 0
	if which == 0:
		if not first_done and not second_done:
			engine.call_c("cd_load_destination_base_set", [0x801B8000])
			engine.call_c("audio_command", [0x11])
			return 1
		if not first_done:
			engine.call_c("cd_load_destination_base_set", [0x801B8000])
			engine.call_c("audio_command", [0x13])
			return 1
		if not second_done:
			engine.call_c("cd_load_destination_base_set", [0x801C8000])
			engine.call_c("audio_command", [0x14])
			return 1
		return 0
	if not first_done and not second_done:
		engine.call_c("cd_load_destination_base_set", [0x801B8000])
		engine.call_c("audio_command", [0x12])
		return 1
	if not first_done:
		engine.call_c("cd_load_destination_base_set", [0x801E0000])
		engine.call_c("audio_command", [0x16])
		return 1
	if not second_done:
		engine.call_c("cd_load_destination_base_set", [0x801B8000])
		engine.call_c("audio_command", [0x15])
		return 1
	return 0

## audio_bank_transfer_poll that also starts the bank's sequence commands (0x22, 0x21, 0x2F for bank A;
## 0x23, 0x24, 0x2E for bank B) when the transfer is done.
func audio_bank_load_poll() -> int:
	var m: Object = engine.memory
	if _transfer_ready():
		if m.u8(D_800B3D4C) == 0:
			engine.call_c("audio_bank_transfer_start")
			m.write_u8(D_800B3D4C, m.u8(D_800B3D4C) + 1)
		else:
			m.write_u8(D_800B3D4C, m.u8(D_800B3D4C) + 1)
			if m.u8(D_800B3D4C) > 0xC0 or engine.call_c("audio_bank_transfer_done", [m.read_global("D_800B3D24")]) != 0:
				if m.read_global("D_800B3D24") == 0:
					engine.call_c("audio_command", [0x22])
					engine.call_c("audio_command", [0x21])
					engine.call_c("audio_command", [0x2F])
				else:
					engine.call_c("audio_command", [0x23])
					engine.call_c("audio_command", [0x24])
					engine.call_c("audio_command", [0x2E])
				m.write_global("D_800B3D54", 0, m.read_global("D_800B3D24"))
				return 1
	return -1

## Both banks of the set are ready: audio_bgm_bank_ready(0) and (1) for set 0, (2) and (3) otherwise (the second
## is only asked when the first is nonzero).
func audio_bank_transfer_done(arg0: int) -> int:
	var low: int = 0 if arg0 == 0 else 2
	if engine.call_c("audio_bgm_bank_ready", [low]) != 0 and engine.call_c("audio_bgm_bank_ready", [low + 1]) != 0:
		return 1
	return 0

## Starts sequence arg1 of loaded bank arg0 (-1 when the bank header is not 0x40): the bank's three setup
## commands, then (arg1 | 0x200). Returns the v0 of the last audio_command (1).
func audio_bank_start(arg0: int, arg1: int) -> int:
	var p: int = BANK_B if arg0 != 0 else BANK_A
	if (engine.memory.s32(p) & 0xFFFF) != 0x40:
		return -1
	if arg0 == 0:
		engine.call_c("audio_command", [0x22])
		engine.call_c("audio_command", [0x21])
		engine.call_c("audio_command", [0x2F])
		engine.call_c("audio_command", [(arg1 | 0x200) & 0xFFFF])
	else:
		engine.call_c("audio_command", [0x23])
		engine.call_c("audio_command", [0x24])
		engine.call_c("audio_command", [0x2E])
		engine.call_c("audio_command", [(arg1 | 0x200) & 0xFFFF])
	engine.memory.write_global("audio_bgm_set", arg0)
	engine.memory.write_global("audio_bgm_sequence", arg1)
	engine.memory.write_global("audio_bgm_playing", 1)
	return 1

## audio_bank_start without the 0x21 / 0x24 command.
func audio_bgm_play_short(arg0: int, arg1: int) -> int:
	var p: int = BANK_B if arg0 != 0 else BANK_A
	if (engine.memory.s32(p) & 0xFFFF) != 0x40:
		return -1
	if arg0 == 0:
		engine.call_c("audio_command", [0x22])
		engine.call_c("audio_command", [0x2F])
		engine.call_c("audio_command", [(arg1 | 0x200) & 0xFFFF])
	else:
		engine.call_c("audio_command", [0x23])
		engine.call_c("audio_command", [0x2E])
		engine.call_c("audio_command", [(arg1 | 0x200) & 0xFFFF])
	engine.memory.write_global("audio_bgm_set", arg0)
	engine.memory.write_global("audio_bgm_sequence", arg1)
	engine.memory.write_global("audio_bgm_playing", 1)
	return 1
