extends RefCounted
## Port of src/main/actor_sprites.c (original object 0x80047550-0x800490C0, T-39311 on the T-39301 engine).
## Screen shake, the actor-pool sprite draw (actor_draw_all), the texture-page buffer and its slot search,
## actor clearing and the screen-brightness fade. Everything lives in game memory (game_state.tpage_buf,
## tpage_slot_state, the 160 x 0x44-byte actor_pool, brightness and shake globals); the sprite quad that
## actor_draw_all hands to func_80098680 (GsSortSprite) is the C's stack local at +0xC8 of its 0xF0 frame.
## libgte/libgpu/CD-cursor calls (func_80098680, load_csr_tp, printf) are call_c and refuse by name until their
## owners are ported (the differential harness replays them). rsin runs inline (Math.rsin ports the asm).
## Contract: wiki/engine-porting-contract.md. Tests: tests/engine/actor_sprites_test.gd,
## config/port/engine-diff/actor_sprites.json.

const Math = preload("res://core/native_math.gd")
## rsin and abs are libgte/libc leaf functions called in the C. They are NOT routed through call_c: they are
## pure (no memory, no state), core/native_math.gd already ports rsin from asm/libgte_8009FA5C.s (Math.rsin,
## checked by its own tests and the taco-tables witness) and abs is absi. A call_c would only refuse until an SDK
## module lists them; this is the one deliberate deviation from "every call goes through call_c".
const Layout = preload("res://core/game_layout.gd")

const C_FILE: String = "src/main/actor_sprites.c"
const C_FUNCTIONS: Array[String] = [
	"draw_shake_start", "draw_shake_update", "actor_draw_all", "tpage_buf_clear_all", "tpage_buf_clear",
	"_sys_default_tpage_set", "load_tpage_buf_lock", "search_load_tpage_buf_lock", "search_tpage_multi",
	"search_tpage", "search_tpage8", "tpage_buffer_print", "actor_draw_enable", "actor_deactivate",
	"actor_clear_upper", "actor_clear_range", "actor_clear", "draw_brightness_get", "draw_brightness_reset",
	"draw_brightness_step", "draw_brightness_set_rate",
]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"draw_shake_start": {"args": [], "ret": "void"},
	"draw_shake_update": {"args": [], "ret": "void"},
	"actor_draw_all": {"args": [], "ret": "void"},
	"tpage_buf_clear_all": {"args": [], "ret": "void"},
	"tpage_buf_clear": {"args": [], "ret": "void"},
	"_sys_default_tpage_set": {"args": [], "ret": "void"},
	"load_tpage_buf_lock": {"args": ["s32", "s32", "u8"], "ret": "void"},
	"search_load_tpage_buf_lock": {"args": ["s32", "u8"], "ret": "s32"},
	"search_tpage_multi": {"args": ["s32", "u8"], "ret": "s32"},
	"search_tpage": {"args": ["s16", "s32", "u8"], "ret": "s32"},
	"search_tpage8": {"args": ["s16", "s32", "u8"], "ret": "s32"},
	"tpage_buffer_print": {"args": [], "ret": "void"},
	"actor_draw_enable": {"args": ["s32"], "ret": "void"},
	"actor_deactivate": {"args": ["s32"], "ret": "void"},
	"actor_clear_upper": {"args": [], "ret": "void"},
	"actor_clear_range": {"args": ["s32"], "ret": "void"},
	"actor_clear": {"args": ["s32"], "ret": "void"},
	"draw_brightness_get": {"args": [], "ret": "u8"},
	"draw_brightness_reset": {"args": [], "ret": "void"},
	"draw_brightness_step": {"args": [], "ret": "void"},
	"draw_brightness_set_rate": {"args": ["u8"], "ret": "void"},
}

## Actor record (include/game/display_records.h, ActorFixed view), 0x44 bytes, 160 records at actor_pool.
const ACTOR_SIZE: int = 0x44
const A_UNK_01: int = 0x01
const A_FLAGS: int = 0x02
const A_DRAW_FLAGS: int = 0x03
const A_UNK_04: int = 0x04
const A_UNK_05: int = 0x05
const A_UNK_06: int = 0x06
const A_UNK_07: int = 0x07
const A_UNK_08: int = 0x08
const A_FRAME_TABLE: int = 0x0C
const A_SEQUENCE_TABLE: int = 0x10
const A_SEQUENCE_SLOT: int = 0x16
const A_FRAME: int = 0x18
const A_UNK_1A: int = 0x1A
const A_UNK_1C: int = 0x1C
const A_UNK_20: int = 0x20
const A_X: int = 0x24
const A_Y: int = 0x28
const A_DX: int = 0x2C
const A_DY: int = 0x30
const A_TPAGE_TABLE: int = 0x34
const A_QUAD_FLAGS: int = 0x38
const A_UNK_40: int = 0x40
const A_UNK_43: int = 0x43
## SpriteQuad (core/game_layout.gd SPRITE_QUAD_*), the spr local at +0xC8 of the actor_draw_all frame
## (asm/matchings/main/actor_sprites/actor_draw_all.s: addiu a0, sp, 0xC8; frame 0xF0), and the uninitialised
## `i` of the first search_tpage call, which the C reads from its own stack slot +0x90 (lw a1, 0x90(sp)).
const SPR_FRAME_OFFSET: int = 0xC8
const I_FRAME_OFFSET: int = 0x90
## Slot of the `gs` local (sw t6, 0xEC(sp) at the function start); func_80098680 receives the bytes behind spr.
const GS_FRAME_OFFSET: int = 0xEC
## The `tpage` local starts as the stale u16 at +0x8E (asm: lhu s2, 0x8E(sp)).
const TPAGE_FRAME_OFFSET: int = 0x8E
## The loop bound of actor_draw_all: *(s32 *)&game_state.menu_hit[3] (0x800E7318).
const MENU_HIT_BOUND_OFFSET: int = 0x1098
## Rodata strings (asm/data/main/actor_sprites.rodata.s)
const STR_CLS: int = 0x800AF960
const STR_PRINT_TPAGE_BUF: int = 0x800AF978
const STR_TPAGE_ENTRY: int = 0x800AF98C
const STR_CR: int = 0x800AF998
const STR_CLASS_CLEAR: int = 0x800AF99C
## game_state.pad_held bit PAD_R2 (include/game_enums.h)
const PAD_R2: int = 0x0002

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

func _actor(index: int) -> int:
	return Math.u32(engine.memory.global_address("actor_pool") + index * ACTOR_SIZE)

func _tpage_buf(index: int) -> int:
	return engine.gs_address("tpage_buf") + index * 4

func _slot_state(index: int) -> int:
	return engine.gs_address("tpage_slot_state") + index * 4

func draw_shake_start() -> void:
	engine.memory.write_global("draw_shake_counter", 4)

## While the shake counter runs: q = (s16) rsin(sum(0..counter-1) * 4) / 4 / (counter + 0x100); the draw origin
## moves by q and the counter grows by 4 (cleared above 0x80). Idle: the origin is (0xA0, 0x168) / (0xA0, 0x78).
@warning_ignore("integer_division")
func draw_shake_update() -> void:
	var m: Object = engine.memory
	var counter: int = m.read_global("draw_shake_counter")
	var ox: int = engine.gs_address("draw_origin_x")
	var oy: int = engine.gs_address("draw_origin_y")
	if counter != 0:
		var total: int = 0
		for i: int in counter:
			total += i
		var q: int = Math.s16(Math.rsin(Math.s32(total * 4))) / 4 / Math.s32(counter + 0x100)
		var off: int = Math.s16(q)
		m.write_s16(ox, Math.s16(q) + 0xA0)
		m.write_s16(ox + 2, off + 0xA0)
		m.write_s16(oy, off + 0x168)
		m.write_s16(oy + 2, off + 0x78)
		m.write_global("draw_shake_x", off + 0xA0)
		m.write_global("draw_shake_y", off + 0x78)
		m.write_global("draw_shake_counter", counter + 4)
		if m.read_global("draw_shake_counter") > 0x80:
			m.write_global("draw_shake_counter", 0)
	else:
		m.write_global("draw_shake_x", 0xA0)
		m.write_global("draw_shake_y", 0x78)
		m.write_s16(ox, 0xA0)
		m.write_s16(ox + 2, 0xA0)
		m.write_s16(oy, 0x168)
		m.write_s16(oy + 2, 0x78)

## Per actor record i = 0 .. *(s32 *)&game_state.menu_hit[3] (re-read each iteration like the C): draw every
## part of the current frame as a sprite quad (func_80098680), then advance the animation and the position.
## An actor whose texture or frame table pointer has bit 31 clear prints "cls..." and is skipped. The C's
## `tpage` local is uninitialised when the quad flags select neither 0 nor 0x01000000 before the first
## assignment: like the original it then holds the stale halfword at frame +0x8E (and later the previous
## record's page).
@warning_ignore("integer_division")
func actor_draw_all() -> void:
	var m: Object = engine.memory
	var spr: int = engine.frame_base() + SPR_FRAME_OFFSET
	var gs: int = Layout.GAME_STATE_BASE
	m.write_u32(engine.frame_base() + GS_FRAME_OFFSET, gs) # the C's `gs` local lives at +0xEC, right after spr
	var col: int = engine.call_c("draw_brightness_get")
	# search_tpage(0x7FFF, i, 0) with i uninitialised: the C reads its own stack slot (lw a1, 0x90(sp))
	engine.call_c("search_tpage", [0x7FFF, m.s32(engine.frame_base() + I_FRAME_OFFSET), 0])
	# `tpage` (u16 local in $s2) starts as the stale halfword at +0x8E of the frame (lhu s2, 0x8E(sp)), like `i` above.
	var tpage: int = m.u16(engine.frame_base() + TPAGE_FRAME_OFFSET)
	var i: int = 0
	while i <= m.s32(gs + MENU_HIT_BOUND_OFFSET):
		if engine.halted():
			return
		var a: int = _actor(i)
		var draw_flags: int = m.u8(a + A_DRAW_FLAGS)
		var fc: int = engine.gs_get("frame_count")
		if (draw_flags & 0x80) \
				and ((draw_flags & 0x40) != 0x40 or fc % (m.u8(a + A_UNK_06) + 1) != 0) \
				and not (m.u8(a + A_FLAGS) & 0x80):
			if not (m.u32(m.u32(a + A_TPAGE_TABLE)) & 0x80000000) or not (m.u32(m.u32(a + A_FRAME_TABLE)) & 0x80000000):
				engine.call_c("printf", [STR_CLS, i, engine.gs_get("game_mode"), engine.gs_get("mode_stage"), engine.gs_get("mode_step"), engine.gs_get("substep")])
			else:
				var step: int = 0
				var slot: int = 0
				var anim: int
				if m.u8(a + A_FLAGS) & 0x40:
					anim = m.u32(m.u32(a + A_FRAME_TABLE) + m.s16(a + A_FRAME) * 4)
				else:
					step = m.s16(a + A_FRAME)
					slot = m.s16(a + A_SEQUENCE_SLOT)
					var row: int = m.u32(m.u32(a + A_SEQUENCE_TABLE) + slot * 4)
					anim = m.u32(m.u32(a + A_FRAME_TABLE) + m.s16(row + (step * 2 + 1) * 2) * 4)
				var ox: int = m.s16(anim + 2)
				var oy: int = m.s16(anim + 4)
				var bx: int = m.s16(a + A_X + 2)
				var by: int = m.s16(a + A_Y + 2)
				var parts: int = anim + 8
				var cnt: int = m.s16(anim)
				if cnt != 0:
					var j: int = cnt - 1
					while j >= 0:
						if engine.halted():
							return
						if (m.u8(a + A_DRAW_FLAGS) & 8) and j >= 4:
							j = 3
						var part: int = Math.u32(parts + j * 14)
						m.write_s32(spr + 0x00, m.s32(a + A_QUAD_FLAGS))
						var p_x: int = m.s16(part + 0)
						var p_y: int = m.s16(part + 2)
						var p_attr: int = m.s16(part + 8)
						var p_w: int = m.s16(part + 0xA)
						var p_h: int = m.s16(part + 0xC)
						if not (m.u8(a + A_DRAW_FLAGS) & 1):
							if p_attr & 2:
								m.write_s16(spr + 4, bx + ox)
								m.write_s16(spr + 0x18, (p_w - ox) + p_x)
								m.write_s16(spr + 0x1C, -0x1000)
							else:
								m.write_s16(spr + 4, bx + ox)
								m.write_s16(spr + 0x18, ox - p_x)
								m.write_s16(spr + 0x1C, 0x1000)
							if p_attr & 1:
								m.write_s16(spr + 6, by + oy)
								m.write_s16(spr + 0x1A, (p_h - oy) + p_y)
								m.write_s16(spr + 0x1E, -0x1000)
							else:
								m.write_s16(spr + 6, by + oy)
								m.write_s16(spr + 0x1A, oy - p_y)
								m.write_s16(spr + 0x1E, 0x1000)
						else:
							if p_attr & 2:
								m.write_s16(spr + 4, bx + ox)
								m.write_s16(spr + 0x18, (p_w - ox) + p_x)
								m.write_s16(spr + 0x1C, 0x1000)
							else:
								m.write_s16(spr + 4, bx + ox)
								m.write_s16(spr + 0x18, ox - p_x)
								m.write_s16(spr + 0x1C, -0x1000)
							if p_attr & 1:
								m.write_s16(spr + 6, by + oy)
								m.write_s16(spr + 0x1A, (p_h - oy) + p_y)
								m.write_s16(spr + 0x1E, 0x1000)
							else:
								m.write_s16(spr + 6, by + oy)
								m.write_s16(spr + 0x1A, oy - p_y)
								m.write_s16(spr + 0x1E, 0x1000)
						if m.u8(a + A_DRAW_FLAGS) & 4:
							m.write_s16(spr + 4, m.s16(spr + 4) - ox)
							m.write_s16(spr + 6, m.s16(spr + 6) - oy)
						if m.s16(a + A_UNK_1A) != 0x1000 and m.s16(a + A_UNK_1C) != 0x1000:
							m.write_s16(spr + 8, p_w + 1)
							m.write_u16(spr + 0xA, p_h + 1)
						else:
							m.write_s16(spr + 8, p_w)
							m.write_u16(spr + 0xA, p_h)
						var uses8: bool = (m.u8(a + A_UNK_04) & 8) == 8
						var which: String = "search_tpage8" if uses8 else "search_tpage"
						match m.s32(a + A_QUAD_FLAGS) & 0x03000000:
							0:
								tpage = engine.call_c(which, [m.s16(part + 4), i, 2]) & 0xFFFF
							0x01000000:
								tpage = engine.call_c(which, [m.s16(part + 4), i, 1]) & 0xFFFF
							_:
								pass # 0x02000000 / 0x03000000: no assignment, the previous (or stale stack) tpage is used
						var tpx: int = tpage & 0x1F
						var tpy: int = (tpage & 0x20) * 4
						m.write_s16(spr + 0xC, tpx)
						if uses8:
							var uv8: int = m.s16(part + 4) & 0xFF
							m.write_u8(spr + 0xE, (uv8 & 0xF) * 8)
							m.write_u8(spr + 0xF, ((uv8 >> 4) * 8) + tpy)
						else:
							var uv: int = m.s16(part + 4) & 0x3F
							m.write_u8(spr + 0xE, (uv & 7) * 0x10)
							m.write_u8(spr + 0xF, ((uv >> 3) * 0x10) + tpy)
						var p_color: int = m.s16(part + 6)
						var a43: int = m.u8(a + A_UNK_43)
						var a05: int = m.u8(a + A_UNK_05)
						if a43 != 0:
							m.write_s16(spr + 0x10, 0)
							match a43 & 0x60:
								0x20:
									m.write_s16(spr + 0x10, 0x100)
								0x40:
									m.write_s16(spr + 0x10, 0x120)
								_:
									if a05 & 0x80:
										m.write_s16(spr + 0x10, _clut_x(a05 & 0x7F, p_color, m.s16(spr + 0x10)))
							m.write_s16(spr + 0x12, (a43 & 0x1F) + 0x1E0)
						else:
							m.write_s16(spr + 0x10, _clut_x(a05 & 0x7F, p_color, 0, true))
							if a05 & 0x80:
								m.write_s16(spr + 0x12, ((p_color & 0xF00) >> 8) + 0x1F0)
							else:
								m.write_s16(spr + 0x12, ((p_color & 0xF00) >> 8) + 0x1E0)
						if m.u8(a + A_DRAW_FLAGS) & 0x20:
							m.write_u8(spr + 0x14, col)
							m.write_u8(spr + 0x15, col)
							m.write_u8(spr + 0x16, col)
						elif m.u8(a + A_UNK_40) != 0 or m.u8(a + A_UNK_40 + 1) != 0 or m.u8(a + A_UNK_40 + 2) != 0:
							m.write_u8(spr + 0x14, m.u8(a + A_UNK_40))
							m.write_u8(spr + 0x15, m.u8(a + A_UNK_40 + 1))
							m.write_u8(spr + 0x16, m.u8(a + A_UNK_40 + 2))
						else:
							m.write_u8(spr + 0x14, m.u8(a + A_UNK_07))
							m.write_u8(spr + 0x15, m.u8(a + A_UNK_07))
							m.write_u8(spr + 0x16, m.u8(a + A_UNK_07))
						m.write_s32(spr + 0x20, m.s32(a + A_UNK_20))
						if m.u8(a + A_DRAW_FLAGS) & 8:
							if m.s32(spr + 0x20) != 0 or m.s16(spr + 0x1C) != 0x1000 or m.s16(spr + 0x1E) != 0x1000:
								m.write_s16(spr + 8, m.u16(spr + 8) - 1)
								m.write_u16(spr + 0xA, m.u16(spr + 0xA) - 1)
						if m.s16(a + A_UNK_1A) != 0x1000 or m.s16(a + A_UNK_1C) != 0x1000:
							m.write_s16(spr + 0x1C, m.s16(a + A_UNK_1A) * (m.s16(spr + 0x1C) / 0x1000))
							m.write_s16(spr + 0x1E, m.s16(a + A_UNK_1C) * (m.s16(spr + 0x1E) / 0x1000))
						var ot: int = Math.s32(m.global_address("draw_ot_table") + m.read_global("draw_buffer") * 0x14)
						if m.u8(a + A_UNK_01) == 0:
							engine.call_c("func_80098680", [spr, ot, 0])
						else:
							engine.call_c("func_80098680", [spr, ot, (m.u8(a + A_UNK_01) - 1) & 0xFFFF])
						j -= 1
				if not (m.u8(a + A_FLAGS) & 0x40) and (m.u8(a + A_FLAGS) & 1) == 1:
					_advance(a, slot, step)
				if m.u8(a + A_DRAW_FLAGS) & 0x10:
					if m.s32(a + A_DX) != 0:
						m.write_s32(a + A_X, m.s32(a + A_X) + m.s32(a + A_DX))
					if m.s32(a + A_DY) != 0:
						m.write_s32(a + A_Y, m.s32(a + A_Y) + m.s32(a + A_DY))
		i += 1

## spr.unk_10 (clut x) from the part's colour word: modes 1, 2, 4, 8 mask 0xF0, 0xE0, 0xC0, 0x80 (>> 4, * 0x10);
## every other mode keeps `keep` (0 when `zero_default`).
func _clut_x(mode: int, p_color: int, keep: int, zero_default: bool = false) -> int:
	match mode:
		1:
			return ((p_color & 0xF0) >> 4) * 0x10
		2:
			return ((p_color & 0xE0) >> 4) * 0x10
		4:
			return ((p_color & 0xC0) >> 4) * 0x10
		8:
			return ((p_color & 0x80) >> 4) * 0x10
	return 0 if zero_default else keep

## The animation step of actor_draw_all for a record with flags bit 0 set and bit 6 clear: timer unk_08 advances
## (by the low 7 bits of unk_06 when bit 7 is set, else by one every unk_06 frames), then frame advances or
## reverses at the end of a step and at the end of the sequence (flags bits 0x10, 0x20, 2, 4, 8).
func _advance(a: int, slot: int, step: int) -> void:
	var m: Object = engine.memory
	var u6: int = m.u8(a + A_UNK_06)
	if (u6 & 0x80) and (u6 & 0x7F) != 0:
		m.write_s16(a + A_UNK_08, m.s16(a + A_UNK_08) + (u6 & 0x7F))
	elif u6 != 0:
		if engine.gs_get("frame_count") % u6 == 0:
			m.write_s16(a + A_UNK_08, m.s16(a + A_UNK_08) + 1)
	var seq: int = m.u32(m.u32(a + A_SEQUENCE_TABLE) + slot * 4)
	if m.s16(a + A_UNK_08) >= m.s16(seq + (step * 2 + 2) * 2):
		if (m.u8(a + A_FLAGS) & 0x10) == 0x10:
			m.write_s16(a + A_FRAME, m.s16(a + A_FRAME) + 1)
			m.write_s16(a + A_UNK_08, 0)
			m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) & ~0x20)
		elif (m.u8(a + A_FLAGS) & 0x20) == 0x20:
			m.write_s16(a + A_FRAME, m.s16(a + A_FRAME) - 1)
			m.write_s16(a + A_UNK_08, 0)
			if m.s16(a + A_FRAME) <= 0:
				m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) & 0xFFDE)
		else:
			m.write_s16(a + A_FRAME, m.s16(a + A_FRAME) + 1)
			m.write_s16(a + A_UNK_08, 0)
	if m.s16(a + A_FRAME) >= m.s16(seq):
		if (m.u8(a + A_FLAGS) & 2) == 2:
			m.write_s16(a + A_FRAME, 0)
			m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) & ~3)
		elif (m.u8(a + A_FLAGS) & 4) == 4:
			m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) & 0xFFFA)
			m.write_s16(a + A_FRAME, m.s16(a + A_FRAME) - 1)
		elif (m.u8(a + A_FLAGS) & 8) == 8:
			m.write_s16(a + A_FRAME, 0)
			m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) & 0xFFFC)
			m.write_u8(a + A_DRAW_FLAGS, m.u8(a + A_DRAW_FLAGS) & ~0x80)
		elif (m.u8(a + A_FLAGS) & 0x10) == 0x10:
			m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) & ~0x10)
			m.write_u8(a + A_FLAGS, m.u8(a + A_FLAGS) | 0x20)
			if m.u8(a + A_DRAW_FLAGS) & 8:
				if m.s16(seq) >= 3:
					m.write_s16(a + A_FRAME, m.s16(a + A_FRAME) - 3)
				else:
					m.write_s16(a + A_FRAME, 0)
			elif m.s16(seq) >= 2:
				m.write_s16(a + A_FRAME, m.s16(a + A_FRAME) - 1)
			else:
				m.write_s16(a + A_FRAME, 0)
		else:
			m.write_s16(a + A_FRAME, 0)

## tpage_buf[i] = 0 for i & 0xF < 5, else -1 (64 entries).
func tpage_buf_clear_all() -> void:
	for i: int in 0x40:
		engine.memory.write_s32(_tpage_buf(i), 0 if (i & 0xF) < 5 else -1)

func tpage_buf_clear() -> void:
	for i: int in 0x40:
		engine.memory.write_s32(_tpage_buf(i), 0 if (i & 0xF) < 5 else -1)
	engine.call_c("_sys_default_tpage_set")

## Default system pages: 0x77 in entries 11-15, 43-47, 21-25, 53-57, and the resident texture-block words in
## 60-63, 31-28, 27 and 59 (D_800C9730, D_800C9734.., D_800C974C.., D_800C5FCC, D_800C5FD0).
func _sys_default_tpage_set() -> void:
	var m: Object = engine.memory
	for i: int in 5:
		m.write_s32(_tpage_buf(i + 11), 0x77)
		m.write_s32(_tpage_buf(i + 43), 0x77)
		m.write_s32(_tpage_buf(i + 21), 0x77)
		m.write_s32(_tpage_buf(i + 53), 0x77)
	m.write_s32(_tpage_buf(60), m.s32(m.global_address("actor_default_texture_blocks")))
	m.write_s32(_tpage_buf(61), m.read_global("D_800C9734"))
	m.write_s32(_tpage_buf(62), m.read_global("D_800C9738"))
	m.write_s32(_tpage_buf(63), m.read_global("D_800C973C"))
	m.write_s32(_tpage_buf(31), m.read_global("D_800C974C"))
	m.write_s32(_tpage_buf(30), m.read_global("D_800C9750"))
	m.write_s32(_tpage_buf(29), m.read_global("D_800C9754"))
	m.write_s32(_tpage_buf(28), m.read_global("D_800C9758"))
	m.write_s32(_tpage_buf(27), m.read_global("D_800C5FCC"))
	m.write_s32(_tpage_buf(59), m.read_global("D_800C5FD0"))

## K&R: tpage_buf[arg0] = arg1, then load_csr_tp for slot arg0 & 0x1F (the upper half of VRAM for arg0 >= 0x20).
func load_tpage_buf_lock(arg0: int, arg1: int, arg2: int) -> void:
	arg2 = Math.u8(arg2)
	engine.memory.write_s32(Math.u32(arg0 * 4 + engine.gs_address("tpage_buf")), arg1)
	if arg0 < 0x20:
		engine.call_c("load_csr_tp", [Math.u32(arg1), Math.u8(arg0 & 0x1F), arg2, 0, 0, 0x80, 0x80])
	else:
		engine.call_c("load_csr_tp", [Math.u32(arg1), Math.u8(arg0 & 0x1F), arg2, 0, 0x80, 0x80, 0x80])

## First free (-1) slot from 5 to 0x3F gets arg0 and a load_csr_tp; returns its index, -1 when full.
func search_load_tpage_buf_lock(arg0: int, arg1: int) -> int:
	arg1 = Math.u8(arg1)
	var m: Object = engine.memory
	var v1: int = 5
	while v1 < 0x40:
		if engine.halted():
			return -1
		if m.s32(_tpage_buf(v1)) == -1:
			m.write_s32(_tpage_buf(v1), arg0)
			if v1 < 0x20:
				engine.call_c("load_csr_tp", [Math.u32(arg0), Math.u8(v1) & 0x1F, arg1, 0, 0, 0x80, 0x80])
				return v1
			engine.call_c("load_csr_tp", [Math.u32(arg0), Math.u8(v1) & 0x1F, arg1, 0, 0x80, 0x80, 0x80])
			return v1
		v1 += 1
	return -1

## The three searches share one shape: (1) a slot i in 15..5, j in 3..0 marked 0x7FFE whose buffer entry equals
## `key`; (2) any slot already holding `key`; (3) the first free (0x7FFF) slot gets `key` and a load_csr_tp
## (upper half of VRAM from slot 0x20); slot 0x3F is returned when none is free.
func _search_slots(key: int, tag: int) -> int:
	var m: Object = engine.memory
	for i: int in range(0xF, 4, -1):
		for j: int in range(3, -1, -1):
			if m.s32(_slot_state(i + j * 16)) == 0x7FFE and m.s32(_tpage_buf(i + j * 16)) == key:
				return (j * 16 + i) & 0xFFFF
	for i: int in 0x40:
		if key == m.s32(_slot_state(i)):
			return i & 0xFFFF
	for i: int in 0x40:
		if engine.halted():
			return 0
		if m.s32(_slot_state(i)) == 0x7FFF:
			m.write_s32(_slot_state(i), key)
			if i < 0x20:
				engine.call_c("load_csr_tp", [Math.u32(key), i & 0xFF, tag, 0, 0, 0x80, 0x80])
			else:
				engine.call_c("load_csr_tp", [Math.u32(key), i & 0xFF, tag, 0, 0x80, 0x80, 0x80])
			return i & 0xFFFF
		if i == 0x3F:
			return 0x3F
	return 0x3F

func search_tpage_multi(arg0: int, arg1: int) -> int:
	return _search_slots(arg0, Math.u8(arg1))

## arg0 == 0x7FFF marks every slot free (0x7FFF where tpage_buf is -1) or taken (0x7FFE) and returns 0;
## otherwise the key is the actor's tpage table word at index arg0 >> shift (6 for search_tpage, 8 for
## search_tpage8).
func _search_actor(arg0: int, arg1: int, arg2: int, shift: int) -> int:
	arg0 = Math.s16(arg0)
	var m: Object = engine.memory
	if arg0 == 0x7FFF:
		for i: int in 64:
			if m.s32(_tpage_buf(i)) == -1:
				m.write_s32(_slot_state(i), 0x7FFF)
			else:
				m.write_s32(_slot_state(i), 0x7FFE)
		return 0
	var record: int = _actor(arg1)
	var index: int = Math.s16(arg0 >> shift)
	var key_at: int = Math.u32(m.u32(record + A_TPAGE_TABLE) + index * 4)
	for i: int in range(0xF, 4, -1):
		for j: int in range(3, -1, -1):
			if m.s32(_slot_state(i + j * 16)) == 0x7FFE and m.s32(_tpage_buf(i + j * 16)) == m.s32(key_at):
				return (j * 16 + i) & 0xFFFF
	for i: int in 0x40:
		if m.s32(_slot_state(i)) == m.s32(key_at):
			return i & 0xFFFF
	for i: int in 0x40:
		if engine.halted():
			return 0
		if m.s32(_slot_state(i)) == 0x7FFF:
			m.write_s32(_slot_state(i), m.s32(key_at))
			if i < 0x20:
				engine.call_c("load_csr_tp", [m.u32(key_at), i & 0xFF, arg2, 0, 0, 0x80, 0x80])
			else:
				engine.call_c("load_csr_tp", [m.u32(key_at), i & 0xFF, arg2, 0, 0x80, 0x80, 0x80])
			return i & 0xFFFF
		if i == 0x3F:
			return 0x3F
	return 0x3F

func search_tpage(arg0: int, arg1: int, arg2: int) -> int:
	return _search_actor(arg0, arg1, Math.u8(arg2), 6)

func search_tpage8(arg0: int, arg1: int, arg2: int) -> int:
	return _search_actor(arg0, arg1, Math.u8(arg2), 8)

## With PAD_R2 held: prints "PRINT_TPAGE_BUF\n" and the 64 buffer words (a "\r" after every fourth).
func tpage_buffer_print() -> void:
	if engine.gs_get("pad_held") & PAD_R2:
		engine.call_c("printf", [STR_PRINT_TPAGE_BUF])
		for i: int in 0x40:
			if engine.halted():
				return
			engine.call_c("printf", [STR_TPAGE_ENTRY, i, engine.memory.s32(_tpage_buf(i))])
			if (i & 3) == 3:
				engine.call_c("printf", [STR_CR])

func actor_draw_enable(arg0: int) -> void:
	engine.gs_set("actor_draw_enabled", 1 if arg0 != 0 else 0)

## Prints "<count> CLASS_CLEAR" and clears draw_flags of the first `count` (at most 160) records, all 160 for 0.
func actor_deactivate(count: int) -> void:
	engine.call_c("printf", [STR_CLASS_CLEAR, count])
	if engine.halted():
		return
	var m: Object = engine.memory
	if count != 0:
		var i: int = 0
		while i < count and i < 160:
			m.write_u8(_actor(i) + A_DRAW_FLAGS, 0)
			i += 1
	else:
		for i: int in 160:
			m.write_u8(_actor(i) + A_DRAW_FLAGS, 0)

func actor_clear_upper() -> void:
	for i: int in range(0x60, 0xA0):
		if engine.halted():
			return
		engine.call_c("actor_clear", [i])

## Clears records 1 .. min(arg0, 160) - 1 (all of them for 0 and for arg0 > 160) and restores the cursor
## globals (cursor_x, cursor_y, D_8011ECE8) that actor_clear touches through its callees.
func actor_clear_range(arg0: int) -> void:
	var m: Object = engine.memory
	var a: int = m.read_global("cursor_x")
	var b: int = m.read_global("cursor_y")
	var c: int = m.read_global("D_8011ECE8")
	var n: int
	if arg0 == 0:
		n = 0xA0
	else:
		n = 0xA0 if arg0 >= 0xA1 else arg0
	var i: int = 1
	while i < n:
		if engine.halted():
			return
		engine.call_c("actor_clear", [i])
		i += 1
	m.write_global("cursor_x", a)
	m.write_global("cursor_y", b)
	m.write_global("D_8011ECE8", c)

## Resets record `actor_index` (< 0xA0) to the defaults: tables point at sprite_frame_table (0x800C975C),
## actor_default_sequence_table (0x800C9A14) and actor_default_texture_blocks (0x800C9730), scale 0x1000,
## brightness 0x80, blink period 1, everything else 0.
func actor_clear(actor_index: int) -> void:
	if actor_index >= 0xA0:
		return
	var m: Object = engine.memory
	var p: int = _actor(actor_index)
	m.write_u8(p + 0, 0)
	m.write_s32(p + 0x38, 0)
	m.write_u8(p + 1, 0)
	m.write_u8(p + 4, 0)
	m.write_u8(p + 5, 0)
	m.write_u8(p + 2, 0)
	m.write_u8(p + 3, 0)
	m.write_u8(p + 6, 1)
	m.write_s16(p + 8, 0)
	m.write_s32(p + 0xC, m.global_address("sprite_frame_table"))
	m.write_s32(p + 0x10, m.global_address("actor_default_sequence_table"))
	m.write_s32(p + 0x34, m.global_address("actor_default_texture_blocks"))
	m.write_s16(p + 0x14, 0)
	m.write_s16(p + 0x16, 0)
	m.write_s16(p + 0x18, 0)
	m.write_s16(p + 0x1A, 0x1000)
	m.write_s16(p + 0x1C, 0x1000)
	m.write_u8(p + 0x43, 0)
	m.write_u8(p + 7, 0x80)
	m.write_s32(p + 0x20, 0)
	m.write_s32(p + 0x24, 0)
	m.write_s32(p + 0x28, 0)
	m.write_s32(p + 0x2C, 0)
	m.write_s32(p + 0x30, 0)
	m.write_u8(p + 0x40, 0)
	m.write_u8(p + 0x41, 0)
	m.write_u8(p + 0x42, 0)

func draw_brightness_get() -> int:
	return engine.gs_get("screen_brightness")

func draw_brightness_reset() -> void:
	engine.gs_set("screen_brightness", 0x80)
	engine.gs_set("brightness_step", 0)

## brightness += (s8) brightness_step, clamped to 0xFF (positive step) or 0 (negative step).
func draw_brightness_step() -> void:
	var step: int = Math.s8(engine.gs_get("brightness_step"))
	if step != 0:
		var v: int = engine.gs_get("screen_brightness") + step
		if step > 0:
			if v >= 0x100:
				v = 0xFF
		else:
			if v < 0:
				v = 0
		engine.gs_set("screen_brightness", v)

func draw_brightness_set_rate(arg0: int) -> void:
	engine.gs_set("brightness_step", Math.u8(arg0))
