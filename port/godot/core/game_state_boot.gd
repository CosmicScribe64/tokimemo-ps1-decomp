extends RefCounted
## T-39300 (M1): the boot stores of GameState that the game memory model needs and that
## rule_state does not carry. Each function ports named C statements as plain stores into
## core/game_memory.gd; nothing here is read from a capture.
##
## Order for a New Game memory (new_game_memory()): game_state_init defaults, then
## boot_settings_defaults_set and pad_defaults_set, then the port's state through
## core/rule_state_bridge.gd (the sourced route values win), then the profile names that
## name_entry_profile_store derives. Calls that need the engine (tpage_buf_clear,
## search_tpage, actor_clear_range, dec_bg_reset, audio_bank_state_reset, menu_bar_color,
## hud_cursor_*, memory_card_block_clear and the draw/menu/text/overlay writes of later
## modes) are not ported here: their fields stay zero and are listed by
## tools/port/compare_game_memory.py against the emulator (T-39300 comments, T-39301).

const Memory = preload("res://core/game_memory.gd")
const Layout = preload("res://core/game_layout.gd")
const Bridge = preload("res://core/rule_state_bridge.gd")

const GS: int = Layout.GAME_STATE_BASE
## include/game_enums.h and include/main/settings.h values used below.
const INPUT_PAD: int = 1
const VOICE_CANCELABLE: int = 2
const MENU_MODE_A: int = 1
const ICON_MODE_TYPE_A: int = 0
const SETTINGS_CURSOR_SPEED_NORMAL: int = 3
const SETTINGS_CLICK_MODE_SINGLE: int = 0
const SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT: int = 0
const OVL_ENTRY_DEFAULT: int = 0x80132000

static func _u8(mem: Memory, offset: int, value: int) -> void:
	mem.write_u8(GS + offset, value)

static func _u16(mem: Memory, offset: int, value: int) -> void:
	mem.write_u16(GS + offset, value)

static func _u32(mem: Memory, offset: int, value: int) -> void:
	mem.write_u32(GS + offset, value)

## src/main/game_init.c game_state_init(1), the stores that are plain GameState and
## global assignments (lines 142-204). bzero(&game_state, 0x1A8C) is the memory's zero start.
static func apply_game_state_init(mem: Memory) -> void:
	_u16(mem, Layout.GAME_STATE_VIDEO_WIDTH, 0x140)
	_u16(mem, Layout.GAME_STATE_VIDEO_HEIGHT, 0xF0)
	_u16(mem, Layout.GAME_STATE_VIDEO_INTERLACE, 0)
	_u8(mem, Layout.GAME_STATE_CLEAR_COLOR_R, 0)
	_u8(mem, Layout.GAME_STATE_CLEAR_COLOR_G, 0)
	_u8(mem, Layout.GAME_STATE_CLEAR_COLOR_B, 0)
	_u16(mem, Layout.GAME_STATE_DRAW_ORIGIN_X, 0xA0)
	_u16(mem, Layout.GAME_STATE_DRAW_ORIGIN_X + 2, 0xA0)
	_u16(mem, Layout.GAME_STATE_DRAW_ORIGIN_Y, 0x168)
	_u16(mem, Layout.GAME_STATE_DRAW_ORIGIN_Y + 2, 0x78)
	_u32(mem, Layout.GAME_STATE_FRAME_COUNT, 0)
	_u32(mem, Layout.GAME_STATE_GAME_MODE_TIMER, 0)
	_u32(mem, Layout.GAME_STATE_MODE_STAGE_TIMER, 0)
	_u32(mem, Layout.GAME_STATE_MODE_STEP_TIMER, 0)
	_u8(mem, Layout.GAME_STATE_GAME_MODE, 0x10)
	_u8(mem, Layout.GAME_STATE_MODE_STAGE, 0)
	_u8(mem, Layout.GAME_STATE_MODE_STEP, 0)
	_u32(mem, Layout.GAME_STATE_PAD_PRESSED, 0)
	_u32(mem, Layout.GAME_STATE_PAD_PREV, 0)
	_u32(mem, Layout.GAME_STATE_PAD_HELD, 0)
	_u8(mem, Layout.GAME_STATE_PAUSED, 0)
	_u8(mem, Layout.GAME_STATE_RESTART_REQUEST, 0)
	_u32(mem, Layout.GAME_STATE_FRAME_SCANLINE_LIMIT, 0x106)
	# *(s32 *)&game_state.menu_hit[3] = 0x9F (a word store over the byte selectors).
	_u32(mem, Layout.GAME_STATE_MENU_HIT + 3, 0x9F)
	# draw_brightness_reset (src/main/actor_sprites.c:711).
	_u8(mem, Layout.GAME_STATE_SCREEN_BRIGHTNESS, 0x80)
	_u8(mem, Layout.GAME_STATE_BRIGHTNESS_STEP, 0)
	_u8(mem, Layout.GAME_STATE_BACK_CLEAR, 0)
	_u8(mem, Layout.GAME_STATE_DRAW_3D_ENABLED, 0)
	_u8(mem, Layout.GAME_STATE_DRAW_2D_ENABLED, 0)
	_u8(mem, Layout.GAME_STATE_SOUND_ACTIVE, 1)
	_u32(mem, Layout.GAME_STATE_QUADS_ENABLED, 1)
	# sprite_tables_clear: sprite_quads_clear(0, 0x40) zeroes quad_layer[0..0x3F];
	# bg_layers_clear(0, 2) sets bg_layer_order[0..1] = 0x80 (the sprite_quads and
	# bg_layers tables themselves are outside GameState and not modelled yet).
	for i: int in 0x40:
		_u8(mem, Layout.GAME_STATE_QUAD_LAYER + i, 0)
	for i: int in 2:
		_u8(mem, Layout.GAME_STATE_BG_LAYER_ORDER + i, 0x80)
	# pad_defaults_set (game_init.c:287).
	_u8(mem, Layout.GAME_STATE_INPUT_DEVICE, INPUT_PAD)
	_u8(mem, Layout.GAME_STATE_CURSOR_SPEED, SETTINGS_CURSOR_SPEED_NORMAL)
	_u8(mem, Layout.GAME_STATE_MENU_MODE, MENU_MODE_A)
	mem.write_u32(mem.global_address("ovl_entry"), OVL_ENTRY_DEFAULT)
	# Birthday word of the placeholder profile (month 1, day 1); boot_player_init sets it again.
	_u32(mem, Layout.GAME_STATE_PLAYER_BIRTHDAY, 1 | (1 << 4))

## src/main/game_init.c boot_settings_defaults_set (lines 470-484).
static func apply_boot_settings_defaults(mem: Memory) -> void:
	_u8(mem, Layout.GAME_STATE_CURRENT_GIRL, 0)
	_u8(mem, Layout.GAME_STATE_TEXT_LINES_TOTAL, 0)
	_u8(mem, Layout.GAME_STATE_TEXT_FIRST_LINE, 0)
	_u8(mem, Layout.GAME_STATE_TEXT_SCROLL_STATE, 0)
	_u8(mem, Layout.GAME_STATE_UNK_F63, 0)
	_u8(mem, Layout.GAME_STATE_ICON_MODE, ICON_MODE_TYPE_A)
	_u8(mem, Layout.GAME_STATE_VOICE_MODE, VOICE_CANCELABLE)
	_u8(mem, Layout.GAME_STATE_CLICK_MODE, SETTINGS_CLICK_MODE_SINGLE)
	_u8(mem, Layout.GAME_STATE_CLICK_TIMER, 0)
	_u8(mem, Layout.GAME_STATE_DOUBLE_CLICK_INTERVAL, 0x20)
	_u8(mem, Layout.GAME_STATE_CONFIRM_BUTTON, SETTINGS_CONFIRM_BUTTON_MOUSE_LEFT)
	_u8(mem, Layout.GAME_STATE_ICON_COUNT, 0xE)
	_u8(mem, Layout.GAME_STATE_SD_ANIM_FAST, 0)

## The part of src/ovl/name_entry/name_entry.c name_entry_profile_store (line 2457) that
## derives from the family and given names already in memory:
## strncpy(full, family, 8); strncat(full, given, 8); strncpy(player_full_name, full, 0x10).
static func apply_profile_full_name(mem: Memory) -> void:
	var full := PackedByteArray()
	for source: int in [Layout.GAME_STATE_PLAYER_FAMILY_NAME, Layout.GAME_STATE_PLAYER_GIVEN_NAME]:
		for i: int in 8:
			var byte: int = mem.u8(GS + source + i)
			if byte == 0:
				break
			full.append(byte)
	full.resize(0x10)
	mem.write_bytes(GS + Layout.GAME_STATE_PLAYER_FULL_NAME, full)

## origin value flow/new_game_state_adapter.gd stores in `opening`: the state's core fields
## and rule_state keys are sourced by the New Game route.
const ADAPTER_ORIGIN_PREFIX: String = "source-fresh-no-card"

static func built_by_adapter(state: Object) -> bool:
	var opening: Variant = state.get("opening")
	return opening is Dictionary and str(opening.get("origin", "")).begins_with(ADAPTER_ORIGIN_PREFIX)

## Fills `mem` (a fresh Memory when omitted; load the executable into it first when the
## installed one is available) for a New Game state: defaults, then the state's sourced
## values, then derived names. A state the New Game adapter did not build never set the
## core fields, so it is refused (mem.error set, nothing written) rather than letting
## its zeroes replace the C defaults (sound_active 1, game_mode 0x10).
static func new_game_memory(state: Object, mem: Memory = null) -> Memory:
	if mem == null:
		mem = Memory.new()
	if not built_by_adapter(state):
		mem._fault("state was not built by the New Game adapter (opening.origin); its core fields are not sourced")
		return mem
	apply_game_state_init(mem)
	apply_boot_settings_defaults(mem)
	Bridge.push(state, mem)
	apply_profile_full_name(mem)
	return mem
