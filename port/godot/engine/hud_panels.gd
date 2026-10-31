extends RefCounted
## Port of src/main/hud_panels.c (original object 0x80062CD0-0x800674B0, T-39312).
## 
## Contract: wiki/engine-porting-contract.md. Tests: config/port/engine-diff/hud_panels.json (emulator captures) and
## tests/engine/hud_panels_test.gd (hand checks written from the C).
## Each function follows the C statement by statement: integer widths and K&R promotion are applied where the C applies
## them (a 32-bit wrap is written out only where the value range allows an overflow), memory is read and written
## with the C types, calls go through engine.call_c, loops that call out test engine.halted().

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/hud_panels.c"
const C_FUNCTIONS: Array[String] = [
	"portrait_read_start",
	"portrait_hide",
	"portrait_build",
	"portrait_value_clear_if_high",
	"portrait_expression_select",
	"portrait_face_update",
	"bustup_speech",
	"bustup_wink",
	"parameter_change",
	"parameter_disp_switch",
	"parameter_show_init",
	"parameter_show",
	"hizuke_disp_switch",
	"hizuke_init",
	"hizuke_show",
	"message_disp_switch",
	"message_window_init",
	"message_window_show",
	"icon_disp_switch",
	"icon_can_use_set",
	"get_icon_can_use",
	"hud_icon_bar_init",
	"hud_icon_layout_apply",
	"hud_icon_memory_card_animation_set",
	"hud_icon_move_allowed",
	"hud_icon_bar_update",
	"message_window_title_show",
	"message_window_set_title_hidden",
	"message_window_set_title",
	"message_window_title_update",
	"hud_season_index",
	"hud_in_summer_months",
	"hud_summer_phase",
	"hud_vacation_for_date",
	"hud_screen_frame_draw",
	"hud_vram_init",
	"hud_season_bgm_load",
]
const C_UNPORTED: Array[String] = [
]
const C_SIGNATURES: Dictionary = {
	"portrait_read_start": {"args": ["s32"], "ret": "void"},
	"portrait_hide": {"args": ["s32"], "ret": "void"},
	"portrait_build": {"args": ["s32", "ptr", "ptr", "ptr", "s32"], "ret": "void"},
	"portrait_value_clear_if_high": {"args": [], "ret": "void"},
	"portrait_expression_select": {"args": ["u32"], "ret": "void"},
	"portrait_face_update": {"args": [], "ret": "void"},
	"bustup_speech": {"args": [], "ret": "void"},
	"bustup_wink": {"args": [], "ret": "void"},
	"parameter_change": {"args": ["s32", "s32"], "ret": "void"},
	"parameter_disp_switch": {"args": ["s32"], "ret": "void"},
	"parameter_show_init": {"args": [], "ret": "void"},
	"parameter_show": {"args": [], "ret": "void"},
	"hizuke_disp_switch": {"args": ["s32"], "ret": "void"},
	"hizuke_init": {"args": [], "ret": "void"},
	"hizuke_show": {"args": [], "ret": "void"},
	"message_disp_switch": {"args": ["s32"], "ret": "void"},
	"message_window_init": {"args": [], "ret": "void"},
	"message_window_show": {"args": [], "ret": "void"},
	"icon_disp_switch": {"args": ["s32"], "ret": "void"},
	"icon_can_use_set": {"args": ["s32", "s32"], "ret": "void"},
	"get_icon_can_use": {"args": ["s32"], "ret": "s32"},
	"hud_icon_bar_init": {"args": [], "ret": "void"},
	"hud_icon_layout_apply": {"args": ["s32"], "ret": "void"},
	"hud_icon_memory_card_animation_set": {"args": ["u8"], "ret": "void"},
	"hud_icon_move_allowed": {"args": ["s32", "s32", "s32"], "ret": "s32"},
	"hud_icon_bar_update": {"args": ["s32"], "ret": "void"},
	"message_window_title_show": {"args": ["s32"], "ret": "void"},
	"message_window_set_title_hidden": {"args": ["ptr"], "ret": "void"},
	"message_window_set_title": {"args": ["ptr"], "ret": "void"},
	"message_window_title_update": {"args": [], "ret": "void"},
	"hud_season_index": {"args": [], "ret": "s32"},
	"hud_in_summer_months": {"args": [], "ret": "s32"},
	"hud_summer_phase": {"args": [], "ret": "s32"},
	"hud_vacation_for_date": {"args": ["s32", "s32"], "ret": "s32"},
	"hud_screen_frame_draw": {"args": ["s32"], "ret": "void"},
	"hud_vram_init": {"args": [], "ret": "void"},
	"hud_season_bgm_load": {"args": [], "ret": "void"},
}

## game_state (GAME_STATE_BASE of core/game_layout.gd, config/symbol_addrs.txt).
const GS: int = 0x800E6280
## D_800B374C (config/symbol_addrs*.txt)
const G_D_800B374C: int = 0x800B374C
## D_800B5A60 (config/symbol_addrs*.txt)
const G_D_800B5A60: int = 0x800B5A60
## D_800B5A64 (config/symbol_addrs*.txt)
const G_D_800B5A64: int = 0x800B5A64
## D_800B5A68 (config/symbol_addrs*.txt)
const G_D_800B5A68: int = 0x800B5A68
## D_800B5A98 (config/symbol_addrs*.txt)
const G_D_800B5A98: int = 0x800B5A98
## D_800B5A9C (config/symbol_addrs*.txt)
const G_D_800B5A9C: int = 0x800B5A9C
## D_800B5AA0 (config/symbol_addrs*.txt)
const G_D_800B5AA0: int = 0x800B5AA0
## D_800B5AA4 (config/symbol_addrs*.txt)
const G_D_800B5AA4: int = 0x800B5AA4
## D_800B5AE4 (config/symbol_addrs*.txt)
const G_D_800B5AE4: int = 0x800B5AE4
## D_800B5B24 (config/symbol_addrs*.txt)
const G_D_800B5B24: int = 0x800B5B24
## D_800B5B28 (config/symbol_addrs*.txt)
const G_D_800B5B28: int = 0x800B5B28
## D_800B5BA8 (config/symbol_addrs*.txt)
const G_D_800B5BA8: int = 0x800B5BA8
## D_800B5BC8 (config/symbol_addrs*.txt)
const G_D_800B5BC8: int = 0x800B5BC8
## D_800B5BCC (config/symbol_addrs*.txt)
const G_D_800B5BCC: int = 0x800B5BCC
## D_800B5BD0 (config/symbol_addrs*.txt)
const G_D_800B5BD0: int = 0x800B5BD0
## D_800B5BD4 (config/symbol_addrs*.txt)
const G_D_800B5BD4: int = 0x800B5BD4
## D_800B5BD8 (config/symbol_addrs*.txt)
const G_D_800B5BD8: int = 0x800B5BD8
## D_800B5BE8 (config/symbol_addrs*.txt)
const G_D_800B5BE8: int = 0x800B5BE8
## D_800B5BF8 (config/symbol_addrs*.txt)
const G_D_800B5BF8: int = 0x800B5BF8
## D_800C6200 (config/symbol_addrs*.txt)
const G_D_800C6200: int = 0x800C6200
## D_800C6220 (config/symbol_addrs*.txt)
const G_D_800C6220: int = 0x800C6220
## actor_default_texture_blocks (config/symbol_addrs*.txt)
const G_ACTOR_DEFAULT_TEXTURE_BLOCKS: int = 0x800C9730
## sprite_frame_table (config/symbol_addrs*.txt)
const G_SPRITE_FRAME_TABLE: int = 0x800C975C
## actor_default_sequence_table (config/symbol_addrs*.txt)
const G_ACTOR_DEFAULT_SEQUENCE_TABLE: int = 0x800C9A14
## D_800C9A54 (config/symbol_addrs*.txt)
const G_D_800C9A54: int = 0x800C9A54
## draw_ot_tags (config/symbol_addrs*.txt)
const G_DRAW_OT_TAGS: int = 0x800E8CA0
## draw_buffer (config/symbol_addrs*.txt)
const G_DRAW_BUFFER: int = 0x8011ECA0
## actor_pool (config/symbol_addrs*.txt)
const G_ACTOR_POOL: int = 0x8011ECD0
## D_8011ED15 (config/symbol_addrs*.txt)
const G_D_8011ED15: int = 0x8011ED15
## D_8011ED16 (config/symbol_addrs*.txt)
const G_D_8011ED16: int = 0x8011ED16
## D_8011ED17 (config/symbol_addrs*.txt)
const G_D_8011ED17: int = 0x8011ED17
## D_8011ED18 (config/symbol_addrs*.txt)
const G_D_8011ED18: int = 0x8011ED18
## D_8011ED19 (config/symbol_addrs*.txt)
const G_D_8011ED19: int = 0x8011ED19
## D_8011ED20 (config/symbol_addrs*.txt)
const G_D_8011ED20: int = 0x8011ED20
## D_8011ED24 (config/symbol_addrs*.txt)
const G_D_8011ED24: int = 0x8011ED24
## D_8011ED28 (config/symbol_addrs*.txt)
const G_D_8011ED28: int = 0x8011ED28
## D_8011ED2C (config/symbol_addrs*.txt)
const G_D_8011ED2C: int = 0x8011ED2C
## D_8011ED3A (config/symbol_addrs*.txt)
const G_D_8011ED3A: int = 0x8011ED3A
## D_8011ED3E (config/symbol_addrs*.txt)
const G_D_8011ED3E: int = 0x8011ED3E
## D_8011ED48 (config/symbol_addrs*.txt)
const G_D_8011ED48: int = 0x8011ED48
## D_8011ED4C (config/symbol_addrs*.txt)
const G_D_8011ED4C: int = 0x8011ED4C
## D_8011F113 (config/symbol_addrs*.txt)
const G_D_8011F113: int = 0x8011F113
## D_8011F4CA (config/symbol_addrs*.txt)
const G_D_8011F4CA: int = 0x8011F4CA
## D_8011F4CB (config/symbol_addrs*.txt)
const G_D_8011F4CB: int = 0x8011F4CB
## D_8011F50E (config/symbol_addrs*.txt)
const G_D_8011F50E: int = 0x8011F50E
## D_8011F50F (config/symbol_addrs*.txt)
const G_D_8011F50F: int = 0x8011F50F
## D_8012059A (config/symbol_addrs*.txt)
const G_D_8012059A: int = 0x8012059A
## sprite_quads (config/symbol_addrs*.txt)
const G_SPRITE_QUADS: int = 0x801217D0
## D_80125C10 (config/symbol_addrs*.txt)
const G_D_80125C10: int = 0x80125C10
## D_80125C80 (config/symbol_addrs*.txt)
const G_D_80125C80: int = 0x80125C80
## icon_enabled (config/symbol_addrs*.txt)
const G_ICON_ENABLED: int = 0x80125C90
## D_80125CA0 (config/symbol_addrs*.txt)
const G_D_80125CA0: int = 0x80125CA0

var engine # GameEngine (engine/engine.gd); set when the engine registers this module

@warning_ignore("integer_division")
func _div(left: int, right: int) -> int:
	if right == 0:
		engine.refuse("division", "C integer division by zero")
		return 0
	return left / right # GDScript integer division truncates toward zero, as the C does

func _mod(left: int, right: int) -> int:
	if right == 0:
		engine.refuse("modulo", "C integer remainder by zero")
		return 0
	return left % right # the sign follows the dividend, as in C

## A local the C may read before it assigns it holds stale stack bytes there; the port refuses instead of
## inventing a value (the harness shows the refusal if a capture ever takes that path).
func _uninit(function_name: String, local_name: String, value: int, is_set: bool) -> int:
	if not is_set:
		engine.refuse("uninitialised local " + local_name, "%s reads %s before the C assigns it (stale stack bytes)" % [function_name, local_name])
	return value

func portrait_read_start(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	engine.call_c("cd_read_sectors", [0x2D, -0x7FE80000, arg0])
	engine.memory.write_global("D_8011F50F", 0)
	engine.memory.write_global("D_8011F4CB", 0)
	engine.memory.write_global("D_800B5A60", 0)

func portrait_hide(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	if arg0 == 0:
		engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) & 0x7F)
		engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) & 0x7F)
		engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x48, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x48)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x6C, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x6C)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x90, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x90)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0xB4, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0xB4)) | 0x80000000)

func portrait_build(arg0: int, arg1: int, arg2: int, arg3: int, expression_variant: int) -> void:
	arg0 = Math.s32(arg0)
	arg1 = Math.u32(arg1)
	arg2 = Math.u32(arg2)
	arg3 = Math.u32(arg3)
	expression_variant = Math.s32(expression_variant)
	var x0: int = 0
	var y0: int = 0
	var w0: int = 0
	var h0: int = 0
	var x1: int = 0
	var y1: int = 0
	var origin_x: int = 0
	var origin_x_set: bool = false
	var origin_y: int = 0
	var origin_y_set: bool = false
	var w1: int = 0
	var h1: int = 0
	var temp_v0: int = 0
	engine.call_c("load_palette", [arg0, 0x10, 1, 2, 0])
	engine.call_c("load_tpage_buf_lock", [5, engine.memory.s32(arg1), 1])
	engine.call_c("load_tpage_buf_lock", [6, engine.memory.s32(arg1 + 0x4), 1])
	engine.call_c("load_tpage_buf_lock", [7, engine.memory.s32(arg1 + 0x8), 1])
	engine.call_c("load_tpage_buf_lock", [8, engine.memory.s32(arg1 + 0xC), 1])
	engine.call_c("load_tpage_buf_lock", [9, engine.memory.s32(arg1 + 0x10), 1])
	engine.call_c("actor_clear", [0x1F])
	engine.memory.write_u8(G_ACTOR_POOL + 0x83D, 0xC)
	engine.memory.write_u8(G_ACTOR_POOL + 0x83E, 0)
	engine.memory.write_u32(G_ACTOR_POOL + 0x874, 0xC1000000)
	engine.memory.write_u8(G_ACTOR_POOL + 0x83F, 0x8C)
	engine.memory.write_u32(G_ACTOR_POOL + 0x848, arg2)
	engine.memory.write_u32(G_ACTOR_POOL + 0x84C, arg3)
	engine.memory.write_u32(G_ACTOR_POOL + 0x870, arg1)
	engine.memory.write_u16(G_ACTOR_POOL + 0x852, 0)
	engine.memory.write_u8(G_ACTOR_POOL + 0x842, 2)
	engine.memory.write_u8(G_ACTOR_POOL + 0x87F, 0x10)
	engine.memory.write_u16(G_ACTOR_POOL + 0x866, -0x20)
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, 0)
	engine.call_c("actor_clear", [0x1E])
	engine.memory.write_u8(G_ACTOR_POOL + 0x7F9, 0xC)
	engine.memory.write_u8(G_ACTOR_POOL + 0x7FA, 0)
	engine.memory.write_u32(G_ACTOR_POOL + 0x830, 0xC1000000)
	engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, 0x8C)
	engine.memory.write_u32(G_ACTOR_POOL + 0x804, arg2)
	engine.memory.write_u32(G_ACTOR_POOL + 0x808, arg3)
	engine.memory.write_u32(G_ACTOR_POOL + 0x82C, arg1)
	engine.memory.write_u16(G_ACTOR_POOL + 0x80E, 1)
	engine.memory.write_u8(G_ACTOR_POOL + 0x7FE, 2)
	engine.memory.write_u8(G_ACTOR_POOL + 0x83B, 0x10)
	engine.memory.write_u16(G_ACTOR_POOL + 0x822, -0x20)
	engine.memory.write_u8(G_ACTOR_POOL + 0x7FF, 0)
	temp_v0 = engine.memory.u32(arg2)
	if engine.memory.s16(temp_v0) >= 6:
		origin_x = engine.memory.s16(temp_v0 + 0x2); origin_x_set = true
		origin_y = engine.memory.s16(temp_v0 + 0x4); origin_y_set = true
		x0 = engine.memory.s16(temp_v0 + 0x8 + 0x46)
		y0 = engine.memory.s16(temp_v0 + 0x8 + 0x48)
		x1 = engine.memory.s16(temp_v0 + 0x8 + 0x38)
		y1 = engine.memory.s16(temp_v0 + 0x8 + 0x3A)
		engine.memory.write_u32(G_SPRITE_QUADS, 0x81000000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x8, 0x10)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xA, 0x10)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x30)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x10, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x12, 0x1F0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x14, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x15, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x18, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x1A, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x1C, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x1E, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x20, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x4, engine.memory.s16(G_ACTOR_POOL + 0x862) + x0 - _uninit("portrait_build", "origin_x", origin_x, origin_x_set))
		engine.memory.write_u16(G_SPRITE_QUADS + 0x6, engine.memory.s16(G_ACTOR_POOL + 0x866) + y0 - _uninit("portrait_build", "origin_y", origin_y, origin_y_set))
		engine.memory.write_u8(GS + 0x10A5, 0xB)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, 0x81000000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x2C, 0x30)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x2E, 0x20)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x33, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x34, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x36, 0x1F0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x38, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x39, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x3C, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x3E, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x40, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x42, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x44, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x28, engine.memory.s16(G_ACTOR_POOL + 0x862) + x1 - _uninit("portrait_build", "origin_x", origin_x, origin_x_set))
		engine.memory.write_u16(G_SPRITE_QUADS + 0x2A, engine.memory.s16(G_ACTOR_POOL + 0x866) + y1 - _uninit("portrait_build", "origin_y", origin_y, origin_y_set))
		engine.memory.write_u8(GS + 0x10A5 + 0x1, 0xB)
	if engine.memory.s16(temp_v0) >= 4:
		x0 = engine.memory.s16(temp_v0 + 0x8 + 0x1C)
		y0 = engine.memory.s16(temp_v0 + 0x8 + 0x1E)
		w0 = engine.memory.s16(temp_v0 + 0x8 + 0x26)
		h0 = engine.memory.s16(temp_v0 + 0x8 + 0x28)
		x1 = engine.memory.s16(temp_v0 + 0x8 + 0x2A)
		y1 = engine.memory.s16(temp_v0 + 0x8 + 0x2C)
		w1 = engine.memory.s16(temp_v0 + 0x8 + 0x34)
		h1 = engine.memory.s16(temp_v0 + 0x8 + 0x36)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x48, -0x3F000000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x50, 0x10)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x52, w0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x56, Math.s8(h0))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x57, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x58, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x5A, 0x1F0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x5C, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x5D, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x5E, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x60, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x62, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x64, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x66, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x68, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x4C, engine.memory.s16(G_ACTOR_POOL + 0x862) + x0 - _uninit("portrait_build", "origin_x", origin_x, origin_x_set))
		engine.memory.write_u16(G_SPRITE_QUADS + 0x4E, engine.memory.s16(G_ACTOR_POOL + 0x866) + y0 - _uninit("portrait_build", "origin_y", origin_y, origin_y_set))
		engine.memory.write_u8(GS + 0x10A5 + 0x2, 0x8B)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x6C, -0x3F000000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x74, w1)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x76, h1)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x7A, 0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x7B, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x7C, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x7E, 0x1F0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x80, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x81, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x82, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x84, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x86, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x88, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x8A, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x8C, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x70, engine.memory.s16(G_ACTOR_POOL + 0x862) + x1 - _uninit("portrait_build", "origin_x", origin_x, origin_x_set))
		engine.memory.write_u16(G_SPRITE_QUADS + 0x72, engine.memory.s16(G_ACTOR_POOL + 0x866) + y1 - _uninit("portrait_build", "origin_y", origin_y, origin_y_set))
		engine.memory.write_u8(GS + 0x10A5 + 0x3, 0x8B)
	if engine.memory.s16(temp_v0) >= 7:
		x0 = engine.memory.s16(temp_v0 + 0x8 + 0x54)
		y0 = engine.memory.s16(temp_v0 + 0x8 + 0x56)
		w0 = engine.memory.s16(temp_v0 + 0x8 + 0x5E)
		h0 = engine.memory.s16(temp_v0 + 0x8 + 0x60)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x90, -0x3F000000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x98, 0x10)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x9A, w0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x9E, Math.s8(h0))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x9F, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xA0, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xA2, 0x1F0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xA4, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xA5, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xA6, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xA8, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xAA, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xAC, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xAE, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0xB0, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0x94, engine.memory.s16(G_ACTOR_POOL + 0x862) + x0 - _uninit("portrait_build", "origin_x", origin_x, origin_x_set))
		engine.memory.write_u16(G_SPRITE_QUADS + 0x96, engine.memory.s16(G_ACTOR_POOL + 0x866) + y0 - _uninit("portrait_build", "origin_y", origin_y, origin_y_set))
		engine.memory.write_u8(GS + 0x10A5 + 0x4, 0x8B)
	if engine.memory.s16(temp_v0) == 8:
		x1 = engine.memory.s16(temp_v0 + 0x8 + 0x62)
		y1 = engine.memory.s16(temp_v0 + 0x8 + 0x64)
		w1 = engine.memory.s16(temp_v0 + 0x8 + 0x6C)
		h1 = engine.memory.s16(temp_v0 + 0x8 + 0x6E)
		engine.memory.write_u32(G_SPRITE_QUADS + 0xB4, -0x3F000000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xBC, w1)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xBE, h1)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xC2, 0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xC3, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xC4, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xC6, 0x1F0)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xC8, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xC9, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xCA, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xCC, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xCE, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xD0, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xD2, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0xD4, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + 0xB8, engine.memory.s16(G_ACTOR_POOL + 0x862) + x1 - _uninit("portrait_build", "origin_x", origin_x, origin_x_set))
		engine.memory.write_u16(G_SPRITE_QUADS + 0xBA, engine.memory.s16(G_ACTOR_POOL + 0x866) + y1 - _uninit("portrait_build", "origin_y", origin_y, origin_y_set))
		engine.memory.write_u8(GS + 0x10A5 + 0x5, 0x8B)
	engine.call_c("portrait_expression_select", [Math.u32(expression_variant)])
	engine.call_c("portrait_value_clear_if_high") # the C also passes 1 argument(s) the callee ignores

func portrait_value_clear_if_high() -> void:
	if engine.memory.read_global("D_800B5A64") >= 0xA5:
		engine.memory.write_global("D_800B5A64", 0)

func portrait_expression_select(expression_variant: int) -> void:
	expression_variant = Math.u32(expression_variant)
	match expression_variant:
		0:
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 7)
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 7)
			return
		1:
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 7)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, engine.memory.u8(G_SPRITE_QUADS + 0x32) + 0x40)
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 7)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, engine.memory.u8(G_SPRITE_QUADS + 0xE) + 0x40)
			return
		2:
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 8)
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 8)
			return
		3:
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 8)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, engine.memory.u8(G_SPRITE_QUADS + 0x32) + 0x40)
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 8)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, engine.memory.u8(G_SPRITE_QUADS + 0xE) + 0x40)
			return
		4:
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 9)
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 9)
			return
		5:
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 9)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, engine.memory.u8(G_SPRITE_QUADS + 0x32) + 0x40)
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 9)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, engine.memory.u8(G_SPRITE_QUADS + 0xE) + 0x40)
			return

func portrait_face_update() -> void:
	var max_: int = 0
	var level: int = 0
	var n: int = 0
	var saved: int = 0
	var i: int = 0
	var girl_id: int = 0
	var face: int = 0
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, 0x80)
	engine.memory.write_u32(G_ACTOR_POOL + 0x874, engine.memory.s32(G_ACTOR_POOL + 0x874) & 0x7FFFFFFF)
	engine.memory.write_u32(G_SPRITE_QUADS, engine.memory.s32(G_SPRITE_QUADS) & 0x7FFFFFFF)
	engine.memory.write_u32(G_SPRITE_QUADS + 0x24, engine.memory.s32(G_SPRITE_QUADS + 0x24) & 0x7FFFFFFF)
	saved = engine.memory.s16(G_ACTOR_POOL + 0x810)
	if engine.memory.u8(GS + 0x3C) != 0:
		if (Math.u8(engine.call_c("cd_audio_state")) & 4) != 0:
			level = Math.s16(engine.call_c("getCDlevel")) + 1
		else:
			level = 0
		engine.memory.write_u16(G_D_80125C80 + (engine.gs_get("frame_count") & 3) * 2, level)
		max_ = 0
		i = 0
		while i < 4:
			if max_ < engine.memory.s16(G_D_80125C80 + i * 2):
				max_ = engine.memory.s16(G_D_80125C80 + i * 2)
			i = Math.s32(i + 1)
		n = 0
		girl_id = engine.gs_get("current_girl")
		i = 0
		while i < 3:
			if engine.memory.u8(G_D_800B5A68 + girl_id * 3 + i) < Math.u32(level):
				n = Math.s32(i + 1)
			i = Math.s32(i + 1)
		if n >= 4:
			n = 3
		if engine.memory.u8(G_SPRITE_QUADS + 0xF) == 0 or not ((engine.gs_get("frame_count") & 1) != 0):
			if level == 0:
				engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0)
			elif n == 0:
				engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0)
			elif n == 1:
				engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0x10)
			elif n == 2:
				engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0x20)
			elif n >= 3:
				engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0x30)
		if (engine.gs_get("pad_held") & 0x80) != 0:
			engine.memory.write_global("D_800B5A98", 0)
		if engine.memory.read_global("D_800B5A98") < level:
			engine.memory.write_global("D_800B5A98", level)
	else:
		face = engine.memory.s16(G_ACTOR_POOL + 0x854)
		engine.memory.write_u8(G_SPRITE_QUADS + 0xF, Math.s32(face * 0x10))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x33, Math.s32(saved << 5))

## Sets the speech portrait state when voice playback is inactive.
func bustup_speech() -> void:
	if engine.memory.u8(GS + 0x3C) == 0 and not ((engine.memory.read_global("D_8011F50E") & 1) != 0):
		engine.memory.write_global("D_8011F50E", (engine.memory.read_global("D_8011F50E") & 0xFF) | 0x11)

## Sets the portrait wink state when no wink is already active.
func bustup_wink() -> void:
	if not ((engine.memory.read_global("D_8011F4CA") & 1) != 0):
		engine.memory.write_global("D_8011F4CA", (engine.memory.read_global("D_8011F4CA") & 0xFF) | 0x11)

## nine +0xFC records, reached by byte offset: the word view (value) and the s16 at +2 (cap)
func parameter_change(parameter_id: int, delta: int) -> void:
	parameter_id = Math.s32(parameter_id)
	delta = Math.s32(delta)
	var p: int = 0
	p = Math.u32(GS + parameter_id * 4)
	engine.memory.write_u32(p + 0xFC, Math.s32(engine.memory.s32(p + 0xFC) + delta))
	if engine.memory.s32(p + 0xFC) < 0:
		engine.memory.write_u32(p + 0xFC, 0)
	if engine.memory.s16(p + 0xFE) >= 0x3E8:
		engine.memory.write_u16(p + 0xFE, 0x3E7)

## Shows or hides the parameter-panel actor group.
func parameter_disp_switch(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var i: int = 0
	if arg0 == 1:
		i = 0
		while i < 0x1C:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x3, (engine.memory.u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x3)) | 0x80)
			i = Math.s32(i + 1)
	else:
		i = 0
		while i < 0x1C:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x3, (engine.memory.u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x3)) & 0x7F)
			i = Math.s32(i + 1)

## Create the digit actors of the parameter panel and snapshot the profile
func parameter_show_init() -> void:
	var i: int = 0
	var value: int = 0
	var snapshot: int = 0
	var digit: int = 0
	engine.call_c("LoadSquare", [0x120, 0x1E0, 0x10, 5, G_D_800C6220])
	i = 0
	while i != 0x1C:
		if engine.halted():
			break
		engine.call_c("actor_clear", [Math.s32(i + 0x20)])
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x1, 0xB)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x2, 0x40)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x38, 0x40000000)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x3, 0x84)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0xC, G_SPRITE_FRAME_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x10, G_ACTOR_DEFAULT_SEQUENCE_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x34, G_ACTOR_DEFAULT_TEXTURE_BLOCKS)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x14, engine.memory.read_global("D_800C9A54"))
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x4, 8)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x20) * 68 + 0x5, 1)
		i = Math.s32(i + 1)
	value = engine.memory.s16(GS + 0xFE)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0x898, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x8A6, -0x49)
	engine.memory.write_u16(G_ACTOR_POOL + 0x8AA, -0x58)
	value = engine.memory.s16(GS + 0xFE)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0x8DC, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x8EA, -0x41)
	engine.memory.write_u16(G_ACTOR_POOL + 0x8EE, -0x58)
	value = engine.memory.s16(GS + 0xFE)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0x920, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x92E, -0x39)
	engine.memory.write_u16(G_ACTOR_POOL + 0x932, -0x58)
	if engine.memory.s16(GS + 0x120 + 0x5A) < engine.memory.s16(GS + 0xFE):
		engine.memory.write_u8(G_ACTOR_POOL + 0x94B, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0x907, engine.memory.u8(G_ACTOR_POOL + 0x94B))
		engine.memory.write_u8(G_ACTOR_POOL + 0x8C3, engine.memory.u8(G_ACTOR_POOL + 0x907))
		engine.memory.write_u16(G_ACTOR_POOL + 0x936, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0x8F2, engine.memory.s16(G_ACTOR_POOL + 0x936))
		engine.memory.write_u16(G_ACTOR_POOL + 0x8AE, engine.memory.s16(G_ACTOR_POOL + 0x8F2))
	elif engine.memory.s16(GS + 0xFE) < engine.memory.s16(GS + 0x120 + 0x5A):
		engine.memory.write_u8(G_ACTOR_POOL + 0x94B, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0x907, engine.memory.u8(G_ACTOR_POOL + 0x94B))
		engine.memory.write_u8(G_ACTOR_POOL + 0x8C3, engine.memory.u8(G_ACTOR_POOL + 0x907))
		engine.memory.write_u16(G_ACTOR_POOL + 0x936, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0x8F2, engine.memory.s16(G_ACTOR_POOL + 0x936))
		engine.memory.write_u16(G_ACTOR_POOL + 0x8AE, engine.memory.s16(G_ACTOR_POOL + 0x8F2))
	value = engine.memory.s16(GS + 0x102)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0x964, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x972, 0xF)
	engine.memory.write_u16(G_ACTOR_POOL + 0x976, -0x58)
	value = engine.memory.s16(GS + 0x102)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0x9A8, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x9B6, 0x17)
	engine.memory.write_u16(G_ACTOR_POOL + 0x9BA, -0x58)
	value = engine.memory.s16(GS + 0x102)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0x9EC, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x9FA, 0x1F)
	engine.memory.write_u16(G_ACTOR_POOL + 0x9FE, -0x58)
	if engine.memory.s16(GS + 0x120 + 0x5E) < engine.memory.s16(GS + 0x102):
		engine.memory.write_u8(G_ACTOR_POOL + 0xA17, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0x9D3, engine.memory.u8(G_ACTOR_POOL + 0xA17))
		engine.memory.write_u8(G_ACTOR_POOL + 0x98F, engine.memory.u8(G_ACTOR_POOL + 0x9D3))
		engine.memory.write_u16(G_ACTOR_POOL + 0xA02, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0x9BE, engine.memory.s16(G_ACTOR_POOL + 0xA02))
		engine.memory.write_u16(G_ACTOR_POOL + 0x97A, engine.memory.s16(G_ACTOR_POOL + 0x9BE))
	elif engine.memory.s16(GS + 0x102) < engine.memory.s16(GS + 0x120 + 0x5E):
		engine.memory.write_u8(G_ACTOR_POOL + 0xA17, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0x9D3, engine.memory.u8(G_ACTOR_POOL + 0xA17))
		engine.memory.write_u8(G_ACTOR_POOL + 0x98F, engine.memory.u8(G_ACTOR_POOL + 0x9D3))
		engine.memory.write_u16(G_ACTOR_POOL + 0xA02, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0x9BE, engine.memory.s16(G_ACTOR_POOL + 0xA02))
		engine.memory.write_u16(G_ACTOR_POOL + 0x97A, engine.memory.s16(G_ACTOR_POOL + 0x9BE))
	value = engine.memory.s16(GS + 0x106)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xA30, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xA3E, 0x67)
	engine.memory.write_u16(G_ACTOR_POOL + 0xA42, -0x58)
	value = engine.memory.s16(GS + 0x106)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xA74, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xA82, 0x6F)
	engine.memory.write_u16(G_ACTOR_POOL + 0xA86, -0x58)
	value = engine.memory.s16(GS + 0x106)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xAB8, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xAC6, 0x77)
	engine.memory.write_u16(G_ACTOR_POOL + 0xACA, -0x58)
	if engine.memory.s16(GS + 0x120 + 0x62) < engine.memory.s16(GS + 0x106):
		engine.memory.write_u8(G_ACTOR_POOL + 0xAE3, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xA9F, engine.memory.u8(G_ACTOR_POOL + 0xAE3))
		engine.memory.write_u8(G_ACTOR_POOL + 0xA5B, engine.memory.u8(G_ACTOR_POOL + 0xA9F))
		engine.memory.write_u16(G_ACTOR_POOL + 0xACE, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xA8A, engine.memory.s16(G_ACTOR_POOL + 0xACE))
		engine.memory.write_u16(G_ACTOR_POOL + 0xA46, engine.memory.s16(G_ACTOR_POOL + 0xA8A))
	elif engine.memory.s16(GS + 0x106) < engine.memory.s16(GS + 0x120 + 0x62):
		engine.memory.write_u8(G_ACTOR_POOL + 0xAE3, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xA9F, engine.memory.u8(G_ACTOR_POOL + 0xAE3))
		engine.memory.write_u8(G_ACTOR_POOL + 0xA5B, engine.memory.u8(G_ACTOR_POOL + 0xA9F))
		engine.memory.write_u16(G_ACTOR_POOL + 0xACE, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xA8A, engine.memory.s16(G_ACTOR_POOL + 0xACE))
		engine.memory.write_u16(G_ACTOR_POOL + 0xA46, engine.memory.s16(G_ACTOR_POOL + 0xA8A))
	value = engine.memory.s16(GS + 0x10A)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xAFC, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xB0A, -0x49)
	engine.memory.write_u16(G_ACTOR_POOL + 0xB0E, -0x4A)
	value = engine.memory.s16(GS + 0x10A)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xB40, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xB4E, -0x41)
	engine.memory.write_u16(G_ACTOR_POOL + 0xB52, -0x4A)
	value = engine.memory.s16(GS + 0x10A)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xB84, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xB92, -0x39)
	engine.memory.write_u16(G_ACTOR_POOL + 0xB96, -0x4A)
	if engine.memory.s16(GS + 0x120 + 0x66) < engine.memory.s16(GS + 0x10A):
		engine.memory.write_u8(G_ACTOR_POOL + 0xBAF, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xB6B, engine.memory.u8(G_ACTOR_POOL + 0xBAF))
		engine.memory.write_u8(G_ACTOR_POOL + 0xB27, engine.memory.u8(G_ACTOR_POOL + 0xB6B))
		engine.memory.write_u16(G_ACTOR_POOL + 0xB9A, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xB56, engine.memory.s16(G_ACTOR_POOL + 0xB9A))
		engine.memory.write_u16(G_ACTOR_POOL + 0xB12, engine.memory.s16(G_ACTOR_POOL + 0xB56))
	elif engine.memory.s16(GS + 0x10A) < engine.memory.s16(GS + 0x120 + 0x66):
		engine.memory.write_u8(G_ACTOR_POOL + 0xBAF, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xB6B, engine.memory.u8(G_ACTOR_POOL + 0xBAF))
		engine.memory.write_u8(G_ACTOR_POOL + 0xB27, engine.memory.u8(G_ACTOR_POOL + 0xB6B))
		engine.memory.write_u16(G_ACTOR_POOL + 0xB9A, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xB56, engine.memory.s16(G_ACTOR_POOL + 0xB9A))
		engine.memory.write_u16(G_ACTOR_POOL + 0xB12, engine.memory.s16(G_ACTOR_POOL + 0xB56))
	value = engine.memory.s16(GS + 0x10E)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xBC8, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xBD6, 0xF)
	engine.memory.write_u16(G_ACTOR_POOL + 0xBDA, -0x4A)
	value = engine.memory.s16(GS + 0x10E)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xC0C, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xC1A, 0x17)
	engine.memory.write_u16(G_ACTOR_POOL + 0xC1E, -0x4A)
	value = engine.memory.s16(GS + 0x10E)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xC50, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xC5E, 0x1F)
	engine.memory.write_u16(G_ACTOR_POOL + 0xC62, -0x4A)
	if engine.memory.s16(GS + 0x120 + 0x6A) < engine.memory.s16(GS + 0x10E):
		engine.memory.write_u8(G_ACTOR_POOL + 0xC7B, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xC37, engine.memory.u8(G_ACTOR_POOL + 0xC7B))
		engine.memory.write_u8(G_ACTOR_POOL + 0xBF3, engine.memory.u8(G_ACTOR_POOL + 0xC37))
		engine.memory.write_u16(G_ACTOR_POOL + 0xC66, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xC22, engine.memory.s16(G_ACTOR_POOL + 0xC66))
		engine.memory.write_u16(G_ACTOR_POOL + 0xBDE, engine.memory.s16(G_ACTOR_POOL + 0xC22))
	elif engine.memory.s16(GS + 0x10E) < engine.memory.s16(GS + 0x120 + 0x6A):
		engine.memory.write_u8(G_ACTOR_POOL + 0xC7B, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xC37, engine.memory.u8(G_ACTOR_POOL + 0xC7B))
		engine.memory.write_u8(G_ACTOR_POOL + 0xBF3, engine.memory.u8(G_ACTOR_POOL + 0xC37))
		engine.memory.write_u16(G_ACTOR_POOL + 0xC66, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xC22, engine.memory.s16(G_ACTOR_POOL + 0xC66))
		engine.memory.write_u16(G_ACTOR_POOL + 0xBDE, engine.memory.s16(G_ACTOR_POOL + 0xC22))
	value = engine.memory.s16(GS + 0x112)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xC94, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xCA2, 0x67)
	engine.memory.write_u16(G_ACTOR_POOL + 0xCA6, -0x4A)
	value = engine.memory.s16(GS + 0x112)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xCD8, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xCE6, 0x6F)
	engine.memory.write_u16(G_ACTOR_POOL + 0xCEA, -0x4A)
	value = engine.memory.s16(GS + 0x112)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xD1C, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xD2A, 0x77)
	engine.memory.write_u16(G_ACTOR_POOL + 0xD2E, -0x4A)
	if engine.memory.s16(GS + 0x120 + 0x6E) < engine.memory.s16(GS + 0x112):
		engine.memory.write_u8(G_ACTOR_POOL + 0xD47, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xD03, engine.memory.u8(G_ACTOR_POOL + 0xD47))
		engine.memory.write_u8(G_ACTOR_POOL + 0xCBF, engine.memory.u8(G_ACTOR_POOL + 0xD03))
		engine.memory.write_u16(G_ACTOR_POOL + 0xD32, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xCEE, engine.memory.s16(G_ACTOR_POOL + 0xD32))
		engine.memory.write_u16(G_ACTOR_POOL + 0xCAA, engine.memory.s16(G_ACTOR_POOL + 0xCEE))
	elif engine.memory.s16(GS + 0x112) < engine.memory.s16(GS + 0x120 + 0x6E):
		engine.memory.write_u8(G_ACTOR_POOL + 0xD47, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xD03, engine.memory.u8(G_ACTOR_POOL + 0xD47))
		engine.memory.write_u8(G_ACTOR_POOL + 0xCBF, engine.memory.u8(G_ACTOR_POOL + 0xD03))
		engine.memory.write_u16(G_ACTOR_POOL + 0xD32, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xCEE, engine.memory.s16(G_ACTOR_POOL + 0xD32))
		engine.memory.write_u16(G_ACTOR_POOL + 0xCAA, engine.memory.s16(G_ACTOR_POOL + 0xCEE))
	value = engine.memory.s16(GS + 0x116)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xD60, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xD6E, -0x49)
	engine.memory.write_u16(G_ACTOR_POOL + 0xD72, -0x3C)
	value = engine.memory.s16(GS + 0x116)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xDA4, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xDB2, -0x41)
	engine.memory.write_u16(G_ACTOR_POOL + 0xDB6, -0x3C)
	value = engine.memory.s16(GS + 0x116)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xDE8, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xDF6, -0x39)
	engine.memory.write_u16(G_ACTOR_POOL + 0xDFA, -0x3C)
	if engine.memory.s16(GS + 0x120 + 0x72) < engine.memory.s16(GS + 0x116):
		engine.memory.write_u8(G_ACTOR_POOL + 0xE13, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xDCF, engine.memory.u8(G_ACTOR_POOL + 0xE13))
		engine.memory.write_u8(G_ACTOR_POOL + 0xD8B, engine.memory.u8(G_ACTOR_POOL + 0xDCF))
		engine.memory.write_u16(G_ACTOR_POOL + 0xDFE, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xDBA, engine.memory.s16(G_ACTOR_POOL + 0xDFE))
		engine.memory.write_u16(G_ACTOR_POOL + 0xD76, engine.memory.s16(G_ACTOR_POOL + 0xDBA))
	elif engine.memory.s16(GS + 0x116) < engine.memory.s16(GS + 0x120 + 0x72):
		engine.memory.write_u8(G_ACTOR_POOL + 0xE13, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xDCF, engine.memory.u8(G_ACTOR_POOL + 0xE13))
		engine.memory.write_u8(G_ACTOR_POOL + 0xD8B, engine.memory.u8(G_ACTOR_POOL + 0xDCF))
		engine.memory.write_u16(G_ACTOR_POOL + 0xDFE, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xDBA, engine.memory.s16(G_ACTOR_POOL + 0xDFE))
		engine.memory.write_u16(G_ACTOR_POOL + 0xD76, engine.memory.s16(G_ACTOR_POOL + 0xDBA))
	value = engine.memory.s16(GS + 0x11A)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xE2C, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xE3A, 0xF)
	engine.memory.write_u16(G_ACTOR_POOL + 0xE3E, -0x3C)
	value = engine.memory.s16(GS + 0x11A)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xE70, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xE7E, 0x17)
	engine.memory.write_u16(G_ACTOR_POOL + 0xE82, -0x3C)
	value = engine.memory.s16(GS + 0x11A)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xEB4, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xEC2, 0x1F)
	engine.memory.write_u16(G_ACTOR_POOL + 0xEC6, -0x3C)
	if engine.memory.s16(GS + 0x120 + 0x76) < engine.memory.s16(GS + 0x11A):
		engine.memory.write_u8(G_ACTOR_POOL + 0xEDF, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xE9B, engine.memory.u8(G_ACTOR_POOL + 0xEDF))
		engine.memory.write_u8(G_ACTOR_POOL + 0xE57, engine.memory.u8(G_ACTOR_POOL + 0xE9B))
		engine.memory.write_u16(G_ACTOR_POOL + 0xECA, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xE86, engine.memory.s16(G_ACTOR_POOL + 0xECA))
		engine.memory.write_u16(G_ACTOR_POOL + 0xE42, engine.memory.s16(G_ACTOR_POOL + 0xE86))
	elif engine.memory.s16(GS + 0x11A) < engine.memory.s16(GS + 0x120 + 0x76):
		engine.memory.write_u8(G_ACTOR_POOL + 0xEDF, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xE9B, engine.memory.u8(G_ACTOR_POOL + 0xEDF))
		engine.memory.write_u8(G_ACTOR_POOL + 0xE57, engine.memory.u8(G_ACTOR_POOL + 0xE9B))
		engine.memory.write_u16(G_ACTOR_POOL + 0xECA, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xE86, engine.memory.s16(G_ACTOR_POOL + 0xECA))
		engine.memory.write_u16(G_ACTOR_POOL + 0xE42, engine.memory.s16(G_ACTOR_POOL + 0xE86))
	value = engine.memory.s16(GS + 0x11E)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 3]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xEF8, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xF06, 0x67)
	engine.memory.write_u16(G_ACTOR_POOL + 0xF0A, -0x3C)
	value = engine.memory.s16(GS + 0x11E)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 2]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xF3C, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xF4A, 0x6F)
	engine.memory.write_u16(G_ACTOR_POOL + 0xF4E, -0x3C)
	value = engine.memory.s16(GS + 0x11E)
	digit = Math.s32(engine.call_c("cd_decimal_digit", [Math.s16(value), 1]))
	engine.memory.write_u16(G_ACTOR_POOL + 0xF80, Math.s32(digit + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0xF8E, 0x77)
	engine.memory.write_u16(G_ACTOR_POOL + 0xF92, -0x3C)
	if engine.memory.s16(GS + 0x120 + 0x7A) < engine.memory.s16(GS + 0x11E):
		engine.memory.write_u8(G_ACTOR_POOL + 0xFAB, 0x43)
		engine.memory.write_u8(G_ACTOR_POOL + 0xF67, engine.memory.u8(G_ACTOR_POOL + 0xFAB))
		engine.memory.write_u8(G_ACTOR_POOL + 0xF23, engine.memory.u8(G_ACTOR_POOL + 0xF67))
		engine.memory.write_u16(G_ACTOR_POOL + 0xF96, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xF52, engine.memory.s16(G_ACTOR_POOL + 0xF96))
		engine.memory.write_u16(G_ACTOR_POOL + 0xF0E, engine.memory.s16(G_ACTOR_POOL + 0xF52))
	elif engine.memory.s16(GS + 0x11E) < engine.memory.s16(GS + 0x120 + 0x7A):
		engine.memory.write_u8(G_ACTOR_POOL + 0xFAB, 0x44)
		engine.memory.write_u8(G_ACTOR_POOL + 0xF67, engine.memory.u8(G_ACTOR_POOL + 0xFAB))
		engine.memory.write_u8(G_ACTOR_POOL + 0xF23, engine.memory.u8(G_ACTOR_POOL + 0xF67))
		engine.memory.write_u16(G_ACTOR_POOL + 0xF96, 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xF52, engine.memory.s16(G_ACTOR_POOL + 0xF96))
		engine.memory.write_u16(G_ACTOR_POOL + 0xF0E, engine.memory.s16(G_ACTOR_POOL + 0xF52))
	engine.memory.write_u16(G_ACTOR_POOL + 0xFC4, 0x3D)
	engine.memory.write_u16(G_ACTOR_POOL + 0xFD2, 0)
	engine.memory.write_u16(G_ACTOR_POOL + 0xFD6, -0x48)
	snapshot = GS + 0x120 + 0x20
	engine.call_c("memcpy", [snapshot, GS + 0xC4, 0x7C])

## Draws the parameter panel and updates its parameter indicators.
func parameter_show() -> void:
	var i: int = 0
	var d: int = 0
	var step: int = 0
	var tab: int = 0
	if engine.gs_get("sd_anim_fast") != 0:
		step = 4
	else:
		step = 8
	tab = G_ACTOR_POOL
	if (engine.memory.u8(tab + 0x883) & 0x80) != 0:
		engine.call_c("prim_gradient_rect", [-0x84, -0x61, 0x108, 0x2A, 0xC, 0x30E00, 0])
		engine.call_c("dtd_on", [0xC])
		i = 0x20
		while i < 0x38:
			if engine.memory.s16(tab + i * 68 + 0x2E) == 1:
				d = _div(0x1000, step)
				engine.memory.write_u16(tab + i * 68 + 0x8, Math.s32(engine.memory.s16(tab + i * 68 + 0x8) + d))
				engine.memory.write_u16(tab + i * 68 + 0x1A, Math.s32(engine.memory.s16(tab + i * 68 + 0x1A) - d))
				if d >= engine.memory.s16(tab + i * 68 + 0x1A):
					engine.memory.write_u16(tab + i * 68 + 0x2E, 2)
			elif engine.memory.s16(tab + i * 68 + 0x2E) == 2:
				d = _div(0x1000, step)
				engine.memory.write_u16(tab + i * 68 + 0x8, Math.s32(engine.memory.s16(tab + i * 68 + 0x8) - d))
				engine.memory.write_u16(tab + i * 68 + 0x1A, Math.s32(engine.memory.s16(tab + i * 68 + 0x1A) + d))
				if engine.memory.s16(tab + i * 68 + 0x1A) >= 0x1000:
					engine.memory.write_u16(tab + i * 68 + 0x1A, 0x1000)
					engine.memory.write_u16(tab + i * 68 + 0x2E, 0)
					engine.memory.write_u16(tab + i * 68 + 0x8, 0)
			i = Math.s32(i + 1)
		i = 0x38
		while i < 0x3B:
			if engine.memory.s16(tab + i * 68 + 0x2E) == 1:
				d = _div(0x1000, step)
				engine.memory.write_u16(tab + i * 68 + 0x1C, Math.s32(engine.memory.s16(tab + i * 68 + 0x1C) - d))
				if d >= engine.memory.s16(tab + i * 68 + 0x1C):
					engine.memory.write_u16(tab + i * 68 + 0x2E, 2)
			elif engine.memory.s16(tab + i * 68 + 0x2E) == 2:
				engine.memory.write_u16(tab + i * 68 + 0x1C, engine.memory.s16(tab + i * 68 + 0x1C) + _div(0x1000, step))
				if engine.memory.s16(tab + i * 68 + 0x1C) >= 0x1000:
					engine.memory.write_u16(tab + i * 68 + 0x1C, 0x1000)
					engine.memory.write_u16(tab + i * 68 + 0x2E, 0)
			i = Math.s32(i + 1)

## Show or hide the calendar date HUD
func hizuke_disp_switch(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var i: int = 0
	if arg0 == 1:
		i = 0
		while i < 8:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x3, (engine.memory.u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x3)) | 0x80)
			i = Math.s32(i + 1)
	else:
		i = 0
		while i < 8:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x3, (engine.memory.u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x3)) & 0x7F)
			i = Math.s32(i + 1)

## Create the calendar date actors (month, day, weekday) for the HUD
func hizuke_init() -> void:
	var i: int = 0
	var digit: int = 0
	var day_kind: int = 0
	i = engine.gs_get("month")
	i = engine.gs_get("day")
	i = 0
	while i != 8:
		if engine.halted():
			break
		engine.call_c("actor_clear", [Math.s32(i + 0x10)])
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x1, 4)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x2, 0x40)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x38, 0x40000000)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x3, 0x84)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0xC, G_SPRITE_FRAME_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x10, G_ACTOR_DEFAULT_SEQUENCE_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x34, G_ACTOR_DEFAULT_TEXTURE_BLOCKS)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x14, engine.memory.read_global("D_800C9A54"))
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x4, 8)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x10) * 68 + 0x5, 1)
		i = Math.s32(i + 1)
	engine.memory.write_u16(G_ACTOR_POOL + 0x49C, Math.s32(Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("year"), 2])) + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x4AA, 0x8C)
	engine.memory.write_u16(G_ACTOR_POOL + 0x4AE, -0x41)
	engine.memory.write_u16(G_ACTOR_POOL + 0x4E0, Math.s32(Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("year"), 1])) + 0x44))
	engine.memory.write_u16(G_ACTOR_POOL + 0x4EE, 0x94)
	engine.memory.write_u16(G_ACTOR_POOL + 0x4F2, -0x41)
	if engine.gs_get("month") >= 0xA:
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("month"), 2]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x524, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x532, 0x8C)
		engine.memory.write_u16(G_ACTOR_POOL + 0x536, -0x22)
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("month"), 1]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x568, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x576, 0x94)
		engine.memory.write_u16(G_ACTOR_POOL + 0x57A, -0x22)
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x50F, 0)
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("month"), 2]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x524, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x532, 0xFF)
		engine.memory.write_u16(G_ACTOR_POOL + 0x536, 0xFF)
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("month"), 1]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x568, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x576, 0x90)
		engine.memory.write_u16(G_ACTOR_POOL + 0x57A, -0x22)
	if engine.gs_get("day") >= 0xA:
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("day"), 2]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5AC, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5BA, 0x8C)
		engine.memory.write_u16(G_ACTOR_POOL + 0x5BE, -0x1)
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("day"), 1]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5F0, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5FE, 0x94)
		engine.memory.write_u16(G_ACTOR_POOL + 0x602, -0x1)
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x597, 0)
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("day"), 2]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5AC, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5BA, 0xFF)
		engine.memory.write_u16(G_ACTOR_POOL + 0x5BE, 0xFF)
		digit = Math.s32(engine.call_c("cd_decimal_digit", [engine.gs_get("day"), 1]))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5F0, Math.s32(digit + 0x44))
		engine.memory.write_u16(G_ACTOR_POOL + 0x5FE, 0x90)
		engine.memory.write_u16(G_ACTOR_POOL + 0x602, -0x1)
	engine.memory.write_u16(G_ACTOR_POOL + 0x634, engine.gs_get("weekday") + 0x52)
	engine.memory.write_u16(G_ACTOR_POOL + 0x642, 0x92)
	engine.memory.write_u16(G_ACTOR_POOL + 0x646, 0x1F)
	if engine.gs_get("weekday") == 5:
		engine.memory.write_u8(G_ACTOR_POOL + 0x65F, 0x43)
	elif engine.gs_get("weekday") == 6:
		engine.memory.write_u8(G_ACTOR_POOL + 0x65F, 0x44)
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x65F, 0)
	day_kind = engine.memory.u8(GS + 0x44 + engine.gs_get("day"))
	if day_kind == 1 or day_kind == 2:
		engine.memory.write_u8(G_ACTOR_POOL + 0x65F, 0x44)
	engine.memory.write_u16(G_ACTOR_POOL + 0x458, 0x3C)
	engine.memory.write_u16(G_ACTOR_POOL + 0x466, 0x90)
	engine.memory.write_u16(G_ACTOR_POOL + 0x46A, -0x12)

## Per-frame update of the calendar date HUD
func hizuke_show() -> void:
	if (engine.memory.read_global("D_8011F113") & 0x80) != 0:
		engine.call_c("prim_gradient_rect", [0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0])
		engine.call_c("dtd_on", [5])

## Enables or disables the message-window display flag.
func message_disp_switch(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	if arg0 == 1:
		engine.memory.write_global("D_8011ED17", engine.memory.read_global("D_8011ED17") | 0x80)
	else:
		engine.memory.write_global("D_8011ED17", engine.memory.read_global("D_8011ED17") & 0x7F)

## Create the message window actor
func message_window_init() -> void:
	engine.call_c("actor_clear", [1])
	engine.memory.write_global("D_8011ED15", 7)
	engine.memory.write_global("D_8011ED16", 0x40)
	engine.memory.write_global("D_8011ED4C", 0x40000000)
	engine.memory.write_global("D_8011ED17", 0x84)
	engine.memory.write_global("D_8011ED20", G_SPRITE_FRAME_TABLE)
	engine.memory.write_global("D_8011ED24", G_ACTOR_DEFAULT_SEQUENCE_TABLE)
	engine.memory.write_global("D_8011ED48", G_ACTOR_DEFAULT_TEXTURE_BLOCKS)
	engine.memory.write_global("D_8011ED28", engine.memory.read_global("D_800C9A54"))
	engine.memory.write_global("D_8011ED18", 8)
	engine.memory.write_global("D_8011ED19", 1)
	engine.memory.write_global("D_8011ED2C", 0x3E)
	engine.memory.write_global("D_8011ED3A", 0)
	engine.memory.write_global("D_8011ED3E", 0x4B)

## Per-frame update of the message window
func message_window_show() -> void:
	if (engine.memory.read_global("D_8011ED17") & 0x80) != 0:
		engine.call_c("prim_gradient_rect", [-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0])
		engine.call_c("dtd_on", [8])

## Show or hide the command icons
func icon_disp_switch(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var i: int = 0
	var p: int = 0
	var g: int = 0
	g = GS
	if arg0 != 0:
		i = 0
		if engine.gs_get("icon_count") != 0:
			p = G_ACTOR_POOL
			while true:
				engine.memory.write_u8(p + 0x1543, engine.memory.u8(p + 0x1543) | 0x80)
				engine.memory.write_u8(p + 0x1103, engine.memory.u8(p + 0x1103) | 0x80)
				i = Math.u32(i + 1)
				p = p + 0x44
				if not (i < engine.memory.u8(g + 0xF6D)):
					break
	else:
		i = 0
		if engine.gs_get("icon_count") != 0:
			p = G_ACTOR_POOL
			while true:
				engine.memory.write_u8(p + 0x1543, engine.memory.u8(p + 0x1543) & 0x7F)
				engine.memory.write_u8(p + 0x1103, engine.memory.u8(p + 0x1103) & 0x7F)
				i = Math.u32(i + 1)
				p = p + 0x44
				if not (i < engine.memory.u8(g + 0xF6D)):
					break

## Enable or disable one of the 14 command icons
func icon_can_use_set(icon_index: int, enabled: int) -> void:
	icon_index = Math.s32(icon_index)
	enabled = Math.s32(enabled)
	if enabled != 0:
		engine.memory.write_u8(G_ICON_ENABLED + icon_index, 1)
	else:
		engine.memory.write_u8(G_ICON_ENABLED + icon_index, 0)

## Returns whether the indexed command icon is enabled.
func get_icon_can_use(icon_index: int) -> int:
	icon_index = Math.s32(icon_index)
	if engine.memory.u8(G_ICON_ENABLED + icon_index) == 1:
		return 1
	return 0

func hud_icon_bar_init() -> void:
	var g: int = 0
	var i: int = 0
	var zero_position_pair_count: int = 0
	var j: int = 0
	g = GS
	if engine.memory.u8(g + 0xF6C) == 2:
		engine.memory.write_u8(g + 0xF6D, 0xF)
	i = 0
	while Math.u32(i) < engine.memory.u8(g + 0xF6D):
		engine.memory.write_u8(G_ICON_ENABLED + i, 1)
		i = Math.s32(i + 1)
	i = 0
	while Math.u32(i) < engine.memory.u8(g + 0xF6D):
		if engine.halted():
			break
		engine.call_c("actor_clear", [Math.s32(i + 0x50)])
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1, 3)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x2, 1)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x38, 0x40000000)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x3, 0x84)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0xC, G_SPRITE_FRAME_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x10, G_ACTOR_DEFAULT_SEQUENCE_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x34, G_ACTOR_DEFAULT_TEXTURE_BLOCKS)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x14, engine.memory.read_global("D_800C9A54"))
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x4, 8)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x5, 1)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x16, i)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x6, 1)
		engine.call_c("actor_clear", [Math.s32(i + 0x40)])
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x1, 4)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x2, 0x40)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x38, 0x40000000)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x3, 0x84)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0xC, G_SPRITE_FRAME_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x10, G_ACTOR_DEFAULT_SEQUENCE_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x34, G_ACTOR_DEFAULT_TEXTURE_BLOCKS)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x14, engine.memory.read_global("D_800C9A54"))
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x4, 8)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x5, 1)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x18, 2)
		i = Math.s32(i + 1)
	engine.memory.write_u8(G_ACTOR_POOL + 0x18F9, 3)
	engine.memory.write_u8(G_ACTOR_POOL + 0x14B9, 4)
	engine.memory.write_u16(G_ACTOR_POOL + 0x190E, 0xF)
	engine.memory.write_u16(G_ACTOR_POOL + 0x191E, 0)
	engine.memory.write_u16(G_ACTOR_POOL + 0x1922, 0)
	engine.memory.write_u16(G_ACTOR_POOL + 0x14CE, 0xF)
	engine.memory.write_u16(G_ACTOR_POOL + 0x14DE, 0)
	engine.memory.write_u16(G_ACTOR_POOL + 0x14E2, 0)
	match engine.memory.u8(g + 0xF6C):
		0:
			i = 0
			while Math.u32(i) < engine.memory.u8(g + 0xF6D):
				engine.memory.write_u16(G_D_80125C10 + Math.s32(i * 2) * 2, engine.memory.s16(G_D_800B5AE4 + i * 4))
				engine.memory.write_u16(G_D_80125C10 + (Math.s32(Math.s32(i * 2) + 1)) * 2, engine.memory.s16(G_D_800B5AE4 + i * 4 + 0x2))
				i = Math.s32(i + 1)
		1:
			i = 0
			while Math.u32(i) < engine.memory.u8(g + 0xF6D):
				engine.memory.write_u16(G_D_80125C10 + Math.s32(i * 2) * 2, engine.memory.s16(G_D_800B5AA4 + i * 4))
				engine.memory.write_u16(G_D_80125C10 + (Math.s32(Math.s32(i * 2) + 1)) * 2, engine.memory.s16(G_D_800B5AA4 + i * 4 + 0x2))
				i = Math.s32(i + 1)
		2:
			zero_position_pair_count = 0
			j = 0
			while j < 0xF:
				if (engine.memory.s16(G_D_80125C10 + Math.s32(j * 2) * 2)) == 0 and (engine.memory.s16(G_D_80125C10 + (Math.s32(Math.s32(j * 2) + 1)) * 2)) == 0:
					zero_position_pair_count = Math.s32(zero_position_pair_count + 1)
				if zero_position_pair_count >= 2:
					zero_position_pair_count = 0
					while zero_position_pair_count < 0xF:
						engine.memory.write_u16(G_D_80125C10 + Math.s32(zero_position_pair_count * 2) * 2, engine.memory.s16(G_D_800B5AE4 + zero_position_pair_count * 4))
						engine.memory.write_u16(G_D_80125C10 + (Math.s32(Math.s32(zero_position_pair_count * 2) + 1)) * 2, engine.memory.s16(G_D_800B5AE4 + zero_position_pair_count * 4 + 0x2))
						zero_position_pair_count = Math.s32(zero_position_pair_count + 1)
					break
				j = Math.s32(j + 1)
	engine.call_c("hud_icon_layout_apply", [engine.memory.u8(g + 0xF6D)])
	engine.call_c("LoadSquare", [0x120, 0x1F1, 0x10, 1, G_D_800C6200])
	engine.call_c("LoadSquare", [0x120, 0x1F2, 0x10, 1, G_D_800C6220])
	engine.call_c("LoadSquare", [0x120, 0x1F3, 0x10, 1, G_D_800C6220 + 0x20])

func hud_icon_layout_apply(icon_count: int) -> void:
	icon_count = Math.s32(icon_count)
	var pad: int = 0
	var i: int = 0
	if icon_count != 0xF:
		icon_count = 0xE
		engine.gs_set("icon_count", 0xE)
	i = 0
	while i < icon_count:
		engine.memory.write_u16(engine.frame_base() + 0xC0 + i * 2, engine.memory.s16(G_D_80125C10 + i * 4) - 0xA)
		engine.memory.write_u16(engine.frame_base() + 0xA0 + i * 2, engine.memory.s16(G_D_80125C10 + i * 4 + 0x2) - 0xA)
		engine.memory.write_u16(engine.frame_base() + 0x80 + i * 2, 0x14)
		engine.memory.write_u16(engine.frame_base() + 0x60 + i * 2, 0x10)
		i = Math.s32(i + 1)
	i = 0
	engine.call_c("menu_set", [0, icon_count & 0xFF, engine.frame_base() + 0xC0, engine.frame_base() + 0xA0, engine.frame_base() + 0x80, engine.frame_base() + 0x60])
	i = 0
	while i < icon_count:
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x16, i)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x26, engine.memory.s16(G_D_80125C10 + i * 4))
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x2A, engine.memory.s16(G_D_80125C10 + i * 4 + 0x2))
		i = Math.s32(i + 1)
	i = 0
	while i < icon_count:
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x26, engine.memory.s16(G_D_80125C10 + i * 4))
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x2A, engine.memory.s16(G_D_80125C10 + i * 4 + 0x2))
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x18, 2)
		i = Math.s32(i + 1)
	if icon_count == 0xF:
		engine.memory.write_u16(G_ACTOR_POOL + 0x190E, 0xF)
		engine.memory.write_u16(G_ACTOR_POOL + 0x191E, engine.memory.s16(G_D_80125C10 + 0x38))
		engine.memory.write_u16(G_ACTOR_POOL + 0x1922, engine.memory.s16(G_D_80125C10 + 0x3A))
		engine.memory.write_u16(G_ACTOR_POOL + 0x14D0, 2)
		engine.memory.write_u16(G_ACTOR_POOL + 0x14DE, engine.memory.s16(G_D_80125C10 + 0x38))
		engine.memory.write_u16(G_ACTOR_POOL + 0x14E2, engine.memory.s16(G_D_80125C10 + 0x3A))
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x14BB, 0)
		engine.memory.write_u8(G_ACTOR_POOL + 0x18FB, 0)

func hud_icon_memory_card_animation_set(arg0: int) -> void:
	arg0 = Math.u8(arg0)
	if arg0 != 0:
		engine.memory.write_u16(G_D_8012059A, 0xE)
		if engine.memory.s16(G_D_8012059A + 0x2) >= 4:
			engine.memory.write_u16(G_D_8012059A + 0x2, 0)
	else:
		engine.memory.write_u16(G_D_8012059A, 0xD)
		if engine.memory.s16(G_D_8012059A + 0x2) >= 6:
			engine.memory.write_u16(G_D_8012059A + 0x2, 0)

## Returns 0 when record idx of D_80125C10 moved by (dx, dy) leaves the screen (x0 < -160, x0 + 19 > 160, y0
## outside -120..120) or when one of its four corners lies in the 20 x 16 box of another of the 15 records, else
## 1. The corners are two local arrays filled from repeated expressions and tested in a 4-trip loop: IDO unrolls
## that loop, keeps the eight corner values in registers (one pair per test, so the outer loop runs out of
## caller-saved registers and spills the shared subexpressions to sp+0x20..0x28) and leaves the 15-trip loop
## rolled. T-25108.
func hud_icon_move_allowed(icon_index: int, delta_x: int, delta_y: int) -> int:
	icon_index = Math.s32(icon_index)
	delta_x = Math.s32(delta_x)
	delta_y = Math.s32(delta_y)
	var p: int = 0
	var i: int = 0
	var j: int = 0
	var move_allowed: int = 0
	var q: int = 0
	var tx: int = 0
	var ty: int = 0
	p = G_D_80125C10 + Math.s32(icon_index * 4)
	move_allowed = 1
	engine.memory.write_u32(engine.frame_base() + 0x30, Math.s32(engine.memory.s16(p) + delta_x))
	engine.memory.write_u32(engine.frame_base() + 0x40, Math.s32(engine.memory.s16(p + 0x2) + delta_y))
	engine.memory.write_u32(engine.frame_base() + 0x30 + 0x4, Math.s32(Math.s32(engine.memory.s16(p) + delta_x) + 0x13))
	engine.memory.write_u32(engine.frame_base() + 0x40 + 0x4, Math.s32(engine.memory.s16(p + 0x2) + delta_y))
	engine.memory.write_u32(engine.frame_base() + 0x30 + 0x8, Math.s32(engine.memory.s16(p) + delta_x))
	engine.memory.write_u32(engine.frame_base() + 0x40 + 0x8, Math.s32(Math.s32(engine.memory.s16(p + 0x2) + delta_y) + 0xF))
	engine.memory.write_u32(engine.frame_base() + 0x30 + 0xC, Math.s32(Math.s32(engine.memory.s16(p) + delta_x) + 0x13))
	engine.memory.write_u32(engine.frame_base() + 0x40 + 0xC, Math.s32(Math.s32(engine.memory.s16(p + 0x2) + delta_y) + 0xF))
	if (engine.memory.s32(engine.frame_base() + 0x30)) < (-0xA0):
		return 0
	if (engine.memory.s32(engine.frame_base() + 0x30 + 0x4)) > 0xA0:
		return 0
	if (engine.memory.s32(engine.frame_base() + 0x40)) < (-0x78):
		return 0
	if (engine.memory.s32(engine.frame_base() + 0x40 + 0x4)) > 0x78:
		return 0
	i = 0
	while i < 0xF:
		if i != icon_index:
			tx = engine.memory.s16(G_D_80125C10 + Math.s32(i * 4))
			ty = engine.memory.s16(G_D_80125C10 + Math.s32(i * 4) + 0x2)
			j = 0
			while j < 4:
				if (((engine.memory.s32(engine.frame_base() + 0x30 + j * 4)) >= tx and (engine.memory.s32(engine.frame_base() + 0x30 + j * 4)) < Math.s32(tx + 0x14)) and (engine.memory.s32(engine.frame_base() + 0x40 + j * 4)) >= ty) and (engine.memory.s32(engine.frame_base() + 0x40 + j * 4)) < Math.s32(ty + 0x10):
					move_allowed = 0
				j = Math.s32(j + 1)
		i = Math.s32(i + 1)
	return move_allowed

## Cursor step of the 15-icon menu: every third frame of the pad repeat (`unk_F6C % 3 == 2`) moves the selected
## icon's position in D_80125C10 and the cursor record by the pad direction (`unk_F6F` pixels), unless
## hud_icon_move_allowed rejects the move, then clamps the icon to x -0x8A..0x80, y -0x5C..0x5C. Without pad
## input it runs the menu cursor check instead. A selection change restarts the 0x20-frame blink of D_800B5AA0,
## which scales the selected icons with rsin while `a` is 0; the loop sets the display modes of the icon records
## 64 + i and 80 + i, and the tail sets the page counter limits as hud_icon_memory_card_animation_set does. The
## selection index is read through `g`: the stores to D_80125C10 then reload the index expression (the original
## recomputes the address for the y store), and uopt keeps the index in $s3, re-read after each call.
func hud_icon_bar_update(a: int) -> void:
	a = Math.s32(a)
	var delta_x: int = 0
	var delta_y: int = 0
	var visible_icon_count: int = 0
	var previous_cursor_index: int = 0
	var i: int = 0
	var g: int = 0
	g = GS
	previous_cursor_index = Math.u8(engine.memory.s8(g + 0x1093))
	delta_x = 0
	delta_y = 0
	if (engine.memory.s32(g + 0xF80) & 1) != 0:
		if (_mod(engine.memory.u8(g + 0xF6C), 3)) == 2:
			match engine.memory.s32(g + 0xF80) & 0xA000:
				0x8000:
					delta_x = Math.s32(-engine.memory.u8(g + 0xF6F))
				0x2000:
					delta_x = engine.memory.u8(g + 0xF6F)
			match Math.u32(engine.memory.s32(g + 0xF80)) & 0x5000:
				0x1000:
					delta_y = Math.s32(-engine.memory.u8(g + 0xF6F))
				0x4000:
					delta_y = engine.memory.u8(g + 0xF6F)
			if (Math.s32(engine.call_c("hud_icon_move_allowed", [engine.memory.s8(g + 0x1093), delta_x, delta_y]))) == 0:
				delta_x = 0
				delta_y = 0
			engine.memory.write_u16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4, Math.s32(engine.memory.s16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4) + delta_x))
			engine.memory.write_u16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4 + 0x2, Math.s32(engine.memory.s16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4 + 0x2) + delta_y))
			engine.memory.write_u16(G_ACTOR_POOL + 0x26, Math.s32(engine.memory.s16(G_ACTOR_POOL + 0x26) + delta_x))
			engine.memory.write_u16(G_ACTOR_POOL + 0x2A, Math.s32(engine.memory.s16(G_ACTOR_POOL + 0x2A) + delta_y))
			engine.call_c("hud_icon_layout_apply", [engine.memory.u8(g + 0xF6D)])
			if (engine.memory.s16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4)) > 0x80:
				engine.memory.write_u16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4, 0x80)
			if (engine.memory.s16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4)) < (-0x8A):
				engine.memory.write_u16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4, -0x8A)
			if (engine.memory.s16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4 + 0x2)) < (-0x5C):
				engine.memory.write_u16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4 + 0x2, -0x5C)
			if (engine.memory.s16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4 + 0x2)) > 0x5C:
				engine.memory.write_u16(G_D_80125C10 + engine.memory.s8(g + 0x1093) * 4 + 0x2, 0x5C)
	else:
		if Math.u32(a) < 0:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(a + 0x40) * 68 + 0x1, 3)
		engine.call_c("menu_check", [0, engine.memory.s16(G_ACTOR_POOL + 0x26), engine.memory.s16(G_ACTOR_POOL + 0x2A)])
		engine.call_c("menu_cursor_update", [0])
	if previous_cursor_index != Math.u32(engine.memory.read_global("D_800B5A9C")):
		engine.memory.write_global("D_800B5A9C", engine.memory.s8(g + 0x1093))
		engine.memory.write_global("D_800B5AA0", 0x20)
	elif engine.memory.read_global("D_800B5AA0") != 0:
		engine.memory.write_global("D_800B5AA0", Math.u32(engine.memory.read_global("D_800B5AA0") - 1))
	visible_icon_count = 0xE
	if engine.memory.u8(g + 0xF6D) == 0xF:
		visible_icon_count = 0xF
	if engine.memory.u8(g + 0xF6C) == 2:
		visible_icon_count = 0xF
	i = 0
	while i < visible_icon_count:
		if engine.halted():
			break
		if engine.memory.u8(G_ICON_ENABLED + i) == 0:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x2, 0)
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x43, 0x51)
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x1, 3)
		else:
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x2, 1)
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x1, 4)
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1, 3)
			if engine.memory.u8(g + 0xF90 + i * 16 + 0x6) != 0:
				engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x43, 0x52)
				engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1, 2)
				engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x1, 3)
				if a == 0:
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1A, Math.s32(Math.s32(engine.call_c("rsin", [Math.u32((Math.u32(0x20 - engine.memory.read_global("D_800B5AA0"))) << 6)])) + 0x1000))
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1C, Math.s32(Math.s32(engine.call_c("rsin", [Math.u32((Math.u32(0x20 - engine.memory.read_global("D_800B5AA0"))) << 6)])) + 0x1000))
				else:
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1A, 0x1000)
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1C, 0x1000)
			else:
				engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x43, 0x53)
				if a == 0:
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1A, 0x1000)
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1C, 0x1000)
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x1A, 0x1000)
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x40) * 68 + 0x1C, 0x1000)
				else:
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1A, 0x1000)
					engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 0x50) * 68 + 0x1C, 0x1000)
		i = Math.s32(i + 1)
	if engine.memory.u8(g + 0x35) != 0:
		engine.memory.write_u16(G_D_8012059A, 0xE)
		if engine.memory.s16(G_D_8012059A + 0x2) >= 4:
			engine.memory.write_u16(G_D_8012059A + 0x2, 0)
	else:
		engine.memory.write_u16(G_D_8012059A, 0xD)
		if engine.memory.s16(G_D_8012059A + 0x2) >= 6:
			engine.memory.write_u16(G_D_8012059A + 0x2, 0)

func message_window_title_show(show: int) -> void:
	show = Math.s32(show)
	var i: int = 0
	if engine.memory.read_global("D_800B5B24") == 0 and engine.memory.read_global("D_800B5B24") >= 0xB:
		return
	if show == 1:
		i = 0
		while i < (engine.memory.read_global("D_800B5B24") + 2):
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x3, (engine.memory.u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x3)) | 0x80)
			i = Math.s32(i + 1)
		engine.call_c("k_disp_switch", [0, 1])
	else:
		if (engine.memory.u8(G_ACTOR_POOL + 0xCF) & 0x80) != 0:
			engine.call_c("k_disp_switch", [0, 0])
		i = 0
		while i < (engine.memory.read_global("D_800B5B24") + 2):
			engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x3, (engine.memory.u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x3)) & 0x7F)
			i = Math.s32(i + 1)
		engine.memory.write_global("D_80125CA0", 0xB8)

func message_window_set_title_hidden(arg0: int) -> void:
	arg0 = Math.u32(arg0)
	engine.call_c("message_window_set_title", [arg0]) # the C passes no argument: a0 of the caller is forwarded
	engine.memory.write_global("D_80125CA0", 0xB8)

func message_window_set_title(arg0: int) -> void:
	arg0 = Math.u32(arg0)
	var p: int = 0
	var i: int = 0
	engine.memory.write_global("D_80125CA0", -0x40)
	i = 0
	while i < (engine.memory.read_global("D_800B5B24") + 2):
		if engine.halted():
			break
		engine.call_c("actor_clear", [Math.s32(i + 3)])
		i = Math.s32(i + 1)
	engine.call_c("set_kanji_string", [-0xCE, -0x98, 0, arg0, 0])
	engine.memory.write_global("D_800B5B24", _div(Math.s32(engine.call_c("strlen", [arg0])), 2))
	i = 0
	while i < (engine.memory.read_global("D_800B5B24") + 2):
		if engine.halted():
			break
		engine.call_c("actor_clear", [Math.s32(i + 3)])
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x1, 2)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x2, 0x40)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x38, 0x40000000)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x3, 0xA4)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0xC, G_SPRITE_FRAME_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x10, G_ACTOR_DEFAULT_SEQUENCE_TABLE)
		engine.memory.write_u32(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x34, G_ACTOR_DEFAULT_TEXTURE_BLOCKS)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x14, engine.memory.read_global("D_800C9A54"))
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x4, 8)
		engine.memory.write_u8(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x5, 1)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x18, 0x72)
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x26, Math.s32(Math.s32(Math.s32((Math.s32(engine.memory.read_global("D_800B5B24") - i)) * 0xE) + engine.memory.read_global("D_80125CA0")) - 0x87))
		engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x2A, engine.memory.read_global("D_80125CA0") - 0x51)
		i = Math.s32(i + 1)
	p = Math.u32(G_ACTOR_POOL + engine.memory.read_global("D_800B5B24") * 0x44)
	engine.memory.write_u16(p + 0x110 + 0x18, 0x71)
	engine.memory.write_u16(p + 0x110 + 0x26, engine.memory.s16(p + 0x110 + 0x26) + 2)
	engine.memory.write_u16(G_ACTOR_POOL + 0xE4, 0x73)
	engine.memory.write_u16(G_ACTOR_POOL + 0xF2, engine.memory.s16(G_ACTOR_POOL + 0xF2) - 2)

func message_window_title_update() -> void:
	var x: int = 0
	var y: int = 0
	var work: int = 0
	var i: int = 0
	if (engine.memory.u8(G_ACTOR_POOL + 0xCF) & 0x80) != 0:
		work = Math.u32(engine.call_c("get_k_work", [0]))
		if engine.memory.read_global("D_80125CA0") < 0:
			engine.memory.write_global("D_80125CA0", engine.memory.read_global("D_80125CA0") + 1)
			i = 0
			while i < (engine.memory.read_global("D_800B5B24") + 2):
				engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x26, Math.s32(Math.s32(Math.s32((Math.s32(engine.memory.read_global("D_800B5B24") - i)) * 0xE) + engine.memory.read_global("D_80125CA0")) - 0x87))
				engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x2A, engine.memory.read_global("D_80125CA0") - 0x51)
				i = Math.s32(i + 1)
			engine.memory.write_u16(G_ACTOR_POOL + 0xF2, engine.memory.s16(G_ACTOR_POOL + 0xF2) - 2)
			engine.memory.write_u16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26, engine.memory.s16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26) + 2)
			x = engine.memory.read_global("D_80125CA0") - 0x90
			y = engine.memory.read_global("D_80125CA0") - 0x5A
			engine.call_c("prim_gradient_rect", [x, y, engine.memory.read_global("D_800B5B24") * 0xE + 8, 0x14, 2, 0x30E00, 0])
			engine.memory.write_u16(work, engine.memory.s16(work) + 1)
			engine.memory.write_u16(work + 0x2, engine.memory.s16(work + 0x2) + 1)
			return
		if engine.memory.read_global("D_80125CA0") < 0x78:
			engine.memory.write_global("D_80125CA0", engine.memory.read_global("D_80125CA0") + 1)
			x = -0x90
			y = -0x5A
			engine.call_c("prim_gradient_rect", [x, y, engine.memory.read_global("D_800B5B24") * 0xE + 8, 0x14, 2, 0x30E00, 0])
			engine.memory.write_u16(work, -0x8E)
			engine.memory.write_u16(work + 0x2, -0x58)
			return
		if engine.memory.read_global("D_80125CA0") < 0xB8:
			engine.memory.write_global("D_80125CA0", engine.memory.read_global("D_80125CA0") + 1)
			i = 0
			while i < (engine.memory.read_global("D_800B5B24") + 2):
				engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x26, Math.s32(Math.s32(Math.s32((Math.s32(engine.memory.read_global("D_800B5B24") - i)) * 0xE) + 0x78 - engine.memory.read_global("D_80125CA0")) - 0x87))
				engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x2A, 0x78 - engine.memory.read_global("D_80125CA0") - 0x51)
				i = Math.s32(i + 1)
			engine.memory.write_u16(G_ACTOR_POOL + 0xF2, engine.memory.s16(G_ACTOR_POOL + 0xF2) - 2)
			engine.memory.write_u16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26, engine.memory.s16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26) + 2)
			x = 0x78 - engine.memory.read_global("D_80125CA0") - 0x90
			y = 0x78 - engine.memory.read_global("D_80125CA0") - 0x5A
			engine.call_c("prim_gradient_rect", [x, y, engine.memory.read_global("D_800B5B24") * 0xE + 8, 0x14, 2, 0x30E00, 0])
			engine.memory.write_u16(work, engine.memory.s16(work) - 1)
			engine.memory.write_u16(work + 0x2, engine.memory.s16(work + 0x2) - 1)
			return
		if (engine.gs_get("pad_held") & 0x10) != 0:
			i = 0
			while i < (engine.memory.read_global("D_800B5B24") + 2):
				engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x26, Math.s32(Math.s32((Math.s32(engine.memory.read_global("D_800B5B24") - i)) * 0xE) - 0x87))
				engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x2A, -0x51)
				i = Math.s32(i + 1)
			engine.memory.write_u16(G_ACTOR_POOL + 0xF2, engine.memory.s16(G_ACTOR_POOL + 0xF2) - 2)
			engine.memory.write_u16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26, engine.memory.s16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26) + 2)
			x = -0x90
			y = -0x5A
			engine.call_c("prim_gradient_rect", [x, y, engine.memory.read_global("D_800B5B24") * 0xE + 8, 0x14, 2, 0x30E00, 0])
			engine.memory.write_u16(work, -0x8E)
			engine.memory.write_u16(work + 0x2, -0x58)
			return
		i = 0
		while i < (engine.memory.read_global("D_800B5B24") + 2):
			engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x26, Math.s32(Math.s32((Math.s32(engine.memory.read_global("D_800B5B24") - i)) * 0xE) - 0xC7))
			engine.memory.write_u16(G_ACTOR_POOL + Math.s32(i + 3) * 68 + 0x2A, -0x91)
			i = Math.s32(i + 1)
		engine.memory.write_u16(G_ACTOR_POOL + 0xF2, engine.memory.s16(G_ACTOR_POOL + 0xF2) - 2)
		engine.memory.write_u16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26, engine.memory.s16(G_ACTOR_POOL + (engine.memory.read_global("D_800B5B24") + 4) * 68 + 0x26) + 2)
		x = -0xD0
		y = -0x9A
		engine.call_c("prim_gradient_rect", [x, y, engine.memory.read_global("D_800B5B24") * 0xE + 8, 0x14, 2, 0x30E00, 0])
		engine.memory.write_u16(work, -0xCE)
		engine.memory.write_u16(work + 0x2, -0x98)

func hud_season_index() -> int:
	match engine.gs_get("month"):
		6, 7, 8:
			return 1
		9, 0xA, 0xB:
			return 2
		1, 2, 0xC:
			return 3
		_:
			return 0

func hud_in_summer_months() -> int:
	match engine.gs_get("month"):
		6, 7, 8, 9:
			return 1
		_:
			return 0

func hud_summer_phase() -> int:
	var summer_phase: int = 0
	summer_phase = Math.s32(engine.call_c("hud_in_summer_months"))
	if engine.gs_get("month") == 9:
		if engine.gs_get("day") >= 0x18:
			summer_phase = -0x1
	if engine.gs_get("month") == 5:
		if engine.gs_get("day") >= 0x19:
			summer_phase = -0x1
	return summer_phase

func hud_vacation_for_date(month: int, day_of_month: int) -> int:
	month = Math.s32(month)
	day_of_month = Math.s32(day_of_month)
	if month == 8:
		return 1
	if month == 7 and day_of_month >= 0x18:
		return 1
	if month == 0xC and day_of_month >= 0x19:
		return 2
	if month == 0xD and day_of_month < 8:
		return 2
	if month == 1 and day_of_month < 8:
		return 2
	if month == 3 and day_of_month >= 0x18:
		return 3
	if month == 4 and day_of_month < 4:
		return 3
	return 0

func hud_screen_frame_draw(a: int) -> void:
	a = Math.s32(a)
	var x: int = 0
	var y: int = 0
	var old_index: int = 0
	var next_index: int = 0
	var blend: int = 0
	var fade: int = 0
	var row: int = 0
	var col: int = 0
	var spr: int = 0
	var current: int = 0
	var old: int = 0
	var next: int = 0
	var out: int = 0
	var old_weight: int = 0
	var next_weight: int = 0
	current = Math.s32(engine.call_c("hud_season_index"))
	match engine.memory.u8(GS + 0xF4 + 0x1) & 0xF:
		1:
			if (engine.gs_get("frame_count") & 3) == 3:
				engine.memory.write_global("D_800B5BD0", Math.u32(engine.memory.read_global("D_800B5BD0") - 1))
			if (engine.gs_get("frame_count") & 1) == 1:
				engine.memory.write_global("D_800B5BCC", engine.memory.read_global("D_800B5BCC") + 1)
		2:
			if (engine.gs_get("frame_count") & 7) == 7:
				engine.memory.write_global("D_800B5BD0", Math.u32(engine.memory.read_global("D_800B5BD0") - 1))
			if (engine.gs_get("frame_count") & 3) == 3:
				engine.memory.write_global("D_800B5BCC", engine.memory.read_global("D_800B5BCC") + 1)
		3:
			if (engine.gs_get("frame_count") & 0x15) == 0x15:
				engine.memory.write_global("D_800B5BD0", Math.u32(engine.memory.read_global("D_800B5BD0") - 1))
			if (engine.gs_get("frame_count") & 7) == 7:
				engine.memory.write_global("D_800B5BCC", engine.memory.read_global("D_800B5BCC") + 1)
		4:
			engine.memory.write_global("D_800B5BD0", Math.u32(engine.memory.read_global("D_800B5BD0") + 0x10 - (Math.s32(engine.call_c("rand")) & 0x1F)))
			engine.memory.write_global("D_800B5BCC", Math.u32(engine.memory.read_global("D_800B5BCC") + 0x10 - (Math.s32(engine.call_c("rand")) & 0x1F)))
		5:
			engine.memory.write_global("D_800B5BD0", Math.u32(engine.memory.read_global("D_800B5BD0") + _div(Math.s32(engine.call_c("rsin", [engine.gs_get("frame_count")])), 0x20)))
			engine.memory.write_global("D_800B5BCC", Math.u32(engine.memory.read_global("D_800B5BCC") + _div(Math.s32(engine.call_c("func_800A0140", [Math.s32(engine.gs_get("frame_count"))])), 0x20)))
		_:
			if (engine.gs_get("frame_count") & 1) == 1:
				engine.memory.write_global("D_800B5BD0", Math.u32(engine.memory.read_global("D_800B5BD0") - 1))
			engine.memory.write_global("D_800B5BCC", engine.memory.read_global("D_800B5BCC") + 1)
	row = 0
	while row < 8:
		col = 0
		while col < 0xA:
			engine.memory.write_u8(engine.frame_base() + 0x6C + col * 8 + row, 1)
			col = Math.s32(col + 1)
		row = Math.s32(row + 1)
	match a:
		0:
			pass
		1:
			row = 1
			while row < 6:
				col = 1
				while col < 9:
					engine.memory.write_u8(engine.frame_base() + 0x6C + col * 8 + row, 0)
					col = Math.s32(col + 1)
				row = Math.s32(row + 1)
		2:
			row = 0
			while row < 6:
				col = 0
				while col < 0xA:
					engine.memory.write_u8(engine.frame_base() + 0x6C + col * 8 + row, 0)
					col = Math.s32(col + 1)
				row = Math.s32(row + 1)
			engine.memory.write_u8(engine.frame_base() + 0x6C + 0x6, 1)
			engine.memory.write_u8(engine.frame_base() + 0x6C + 0x48 + 0x6, 1)
			engine.memory.write_u8(engine.frame_base() + 0x6C + 0x5, 1)
			engine.memory.write_u8(engine.frame_base() + 0x6C + 0x48 + 0x5, 1)
	row = 0
	while row < 8:
		if engine.halted():
			break
		col = 0
		while col < 0xA:
			if engine.halted():
				break
			if (engine.memory.u8(engine.frame_base() + 0x6C + col * 8 + row)) != 0:
				spr = Math.u32(engine.call_c("GetWorkBase", [0x14, engine.memory.read_global("draw_buffer")]))
				engine.call_c("func_8009F0A4", [spr])
				engine.call_c("SetSemiTrans", [spr, 0])
				x = Math.s32(col * 0x20)
				y = Math.s32(Math.u32(Math.u32(row) * 0x20) + 0xF0)
				engine.memory.write_u16(spr + 0x8, x)
				engine.memory.write_u16(spr + 0xA, Math.s32(y - Math.s32(engine.memory.read_global("draw_buffer") * 0xF0)))
				engine.memory.write_u8(spr + 0xC, (engine.memory.read_global("D_800B5BCC") & 0x1F) + 0x80)
				engine.memory.write_u8(spr + 0xD, (engine.memory.read_global("D_800B5BD0") & 0x1F) + 0x80)
				engine.memory.write_u16(spr + 0x12, 0x20)
				engine.memory.write_u16(spr + 0x10, 0x20)
				engine.memory.write_u8(spr + 0x4, engine.gs_get("screen_brightness"))
				engine.memory.write_u8(spr + 0x5, engine.gs_get("screen_brightness"))
				engine.memory.write_u8(spr + 0x6, engine.gs_get("screen_brightness"))
				engine.memory.write_u16(spr + 0xE, Math.s32(engine.call_c("func_8009ED74", [0xA0, 0x1E1])))
				engine.call_c("AddPrim", [Math.u32(Math.u32(G_DRAW_OT_TAGS + (engine.memory.read_global("draw_buffer") << 10)) + Math.u8(engine.memory.read_global("D_800B5BD4")) * 4), Math.s32(spr)])
			col = Math.s32(col + 1)
		row = Math.s32(row + 1)
	engine.call_c("goto_tpage", [0x3C0, 0x100, Math.u8(engine.memory.read_global("D_800B5BD4")), 0, 0])
	if (engine.gs_get("frame_count") & 3) == 3 and engine.memory.read_global("D_800B5BC8") >= 0xEF:
		engine.memory.write_global("D_800B5BC8", engine.memory.read_global("D_800B5BC8") + 1)
		fade = engine.memory.read_global("D_800B5BC8")
		if fade == 0xFF:
			current = Math.s32(current + 1)
			engine.call_c("LoadSquare", [0xA0, 0x1E1, 0x10, 1, G_D_800B5B28 + (Math.s32(current + 3) & 3) * 32])
			return
		if fade >= 0xF0:
			next_weight = Math.s32(fade - 0xEF)
			old_weight = 0xFF - fade
			old_index = Math.s32(current + 3) & 3
			next_index = Math.s32(current + 4) & 3
			old = G_D_800B5B28 + old_index * 32
			next = G_D_800B5B28 + next_index * 32
			out = G_D_800B5BA8
			col = 0
			while col < 8:
				blend = Math.u32(Math.u32(old_weight * (engine.memory.u32(old) & 0x1F)) + Math.u32(next_weight * (engine.memory.u32(next) & 0x1F)))
				engine.memory.write_u32(out, (engine.memory.u32(out) & 0xFFFFFFE0) | (blend >> 4 & 0x1F))
				blend = Math.u32(Math.u32(old_weight * ((engine.memory.u32(old) >> 5) & 0x1F)) + Math.u32(next_weight * ((engine.memory.u32(next) >> 5) & 0x1F)))
				engine.memory.write_u32(out, (engine.memory.u32(out) & 0xFFFFFC1F) | ((blend >> 4 & 0x1F) << 5))
				blend = Math.u32(Math.u32(old_weight * ((engine.memory.u32(old) >> 10) & 0x1F)) + Math.u32(next_weight * ((engine.memory.u32(next) >> 10) & 0x1F)))
				engine.memory.write_u32(out, (engine.memory.u32(out) & 0xFFFF83FF) | ((blend >> 4 & 0x1F) << 10))
				blend = Math.u32(Math.u32(old_weight * ((engine.memory.u32(old) >> 16) & 0x1F)) + Math.u32(next_weight * ((engine.memory.u32(next) >> 16) & 0x1F)))
				engine.memory.write_u32(out, (engine.memory.u32(out) & 0xFFE0FFFF) | ((blend >> 4 & 0x1F) << 16))
				blend = Math.u32(Math.u32(old_weight * ((engine.memory.u32(old) >> 21) & 0x1F)) + Math.u32(next_weight * ((engine.memory.u32(next) >> 21) & 0x1F)))
				engine.memory.write_u32(out, (engine.memory.u32(out) & 0xFC1FFFFF) | ((blend >> 4 & 0x1F) << 21))
				blend = Math.u32(Math.u32(old_weight * ((engine.memory.u32(old) >> 26) & 0x1F)) + Math.u32(next_weight * ((engine.memory.u32(next) >> 26) & 0x1F)))
				engine.memory.write_u32(out, (engine.memory.u32(out) & 0x83FFFFFF) | ((blend >> 4 & 0x1F) << 26))
				old = old + 0x4
				next = next + 0x4
				out = out + 0x4
				col = Math.s32(col + 1)
			engine.call_c("LoadSquare", [0xA0, 0x1E1, 0x10, 1, G_D_800B5BA8])

func hud_vram_init() -> void:
	var i: int = 0
	var j: int = 0
	engine.call_c("LoadSquare", [0x3E0, 0x180, 0x20, 0x80, engine.memory.read_global("D_800B374C")])
	i = 0
	while i < 0x20:
		j = 0
		while j < 4:
			engine.memory.write_u32(GS + 0x163C + (Math.s32(Math.s32(i * 4) + j)) * 4, 0)
			j = Math.s32(j + 1)
		i = Math.s32(i + 1)
	engine.call_c("LoadSquare", [0x3E0, 0x1C0, 4, 0x10, GS + 0x163C])

func hud_season_bgm_load() -> void:
	var v: int = 0
	engine.call_c("audio_command", [5])
	engine.call_c("audio_command", [0x71])
	v = Math.s32(engine.call_c("hud_season_index")) & 3
	engine.call_c("audio_bank_load_start", [0, 0xBF98, 0xBF79, engine.memory.s32(G_D_800B5BE8 + v * 4), engine.memory.s32(G_D_800B5BF8 + v * 4), engine.memory.s32(G_D_800B5BD8 + v * 4)])
