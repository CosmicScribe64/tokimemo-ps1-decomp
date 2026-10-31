extends RefCounted
## Port of src/main/scene_library.c (original object 0x8007C030-0x80085E30, T-39312).
## 
## Contract: wiki/engine-porting-contract.md. Tests: config/port/engine-diff/scene_library.json (emulator captures) and
## tests/engine/scene_library_test.gd (hand checks written from the C).
## Each function follows the C statement by statement: integer widths and K&R promotion are applied where the C applies
## them (a 32-bit wrap is written out only where the value range allows an overflow), memory is read and written
## with the C types, calls go through engine.call_c, loops that call out test engine.halted().

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "src/main/scene_library.c"
const C_FUNCTIONS: Array[String] = [
	"SD_GetCDLevel",
	"SD_DetectCDPeak",
	"SD_CalcCDAve",
	"getCDlevel",
	"addr_init_weekly_sd",
	"addr_init_holiday_sd",
	"addr_init_club_sd",
	"addr_init_else_sd",
	"addr_init_bustup",
	"scene_wait_cd_read",
	"scene_bg_show",
	"scene_bg_hide",
	"scene_bg_wipe_in",
	"scene_bg_wipe_in_step",
	"set_tarao_bg",
	"set_tarao_sprt",
	"set_tarao_rect",
	"normal_date_bg_out",
	"scene_bg_wipe_out_step",
	"normal_date_girl_in",
	"normal_date_girl_in_init",
	"normal_date_girl_in_main",
	"normal_date_girl_suddenin",
	"normal_date_girl_out",
	"normal_date_girl_out_init",
	"normal_date_girl_out_main",
	"normal_date_girl_suddenout",
	"normal_date_bg_fadein",
	"normal_date_bggirl_fadein",
	"normal_date_bg_fadeout",
	"normal_date_bggirl_fadeout",
	"normal_date_move_place",
	"normal_date_move_place_init",
	"normal_date_move_place_main",
	"place_init",
	"place_init2",
	"change_dec_bg",
	"dec_init",
	"bg_read_sub2",
	"check_k_scroll",
	"wait_sub_sub",
	"talk_speak_part",
	"talk_voice_skip_check",
	"talk_render",
	"talk_run",
	"make_three_select",
	"junban_init",
	"normal_date_three_select",
	"normal_date_three_select_init",
	"normal_date_three_select_main",
	"normal_date_two_select",
	"normal_date_two_select_init",
	"normal_date_two_select_main",
	"face_expression_mood",
	"face_expression_0",
	"face_expression_1",
	"face_expression_2",
	"face_expression_3",
	"face_expression_4",
	"portrait_layout_set",
	"portrait_show_checked",
	"face_show",
	"face_show_checked_hidden",
	"portrait_wink_update",
	"scene_frame_update",
	"hizuke_hide",
	"hizuke_appear",
	"read_bustup",
	"read_bustup_uniform",
	"read_bustup_swimsuit",
	"bustup_return",
	"return_step",
	"k_disp_inc2",
	"yosi_trans",
	"change_yoshio",
	"change_girl",
	"talk_names_set",
	"select_girl",
	"select_girl2",
	"select_girl_init",
	"select_girl_main",
	"sprite_brightness",
	"day_plus",
	"check_para_limit",
	"text_marker_hide",
	"scene_mouth_lock_clear",
	"scene_still_show",
	"scene_anim_show",
	"scene_still_layer_set",
	"scene_still_fade_in",
	"scene_still_fade_out",
	"event_face0",
	"event_face1",
	"event_face2",
	"event_face3",
	"event_face_kuchi0",
	"event_face_kuchi1",
	"event_kuchi_ani_no",
	"event_kuchi_ani_yes",
	"ev_me_on",
	"ev_me_on_continue",
	"_sprite_set_box_shade_tarao",
	"talk_clear",
	"don_init2",
	"don_wait",
	"normal_date_speak_1line",
	"normal_date_speak_012",
	"normal_date_speak",
	"vram_bustup_clear",
	"event_log_add",
	"portrait_quad_offsets",
]
const C_UNPORTED: Array[String] = [
]
const C_SIGNATURES: Dictionary = {
	"SD_GetCDLevel": {"args": ["ptr"], "ret": "void"},
	"SD_DetectCDPeak": {"args": ["ptr", "ptr", "u16", "u16"], "ret": "void"},
	"SD_CalcCDAve": {"args": ["ptr", "ptr", "u16", "u16"], "ret": "void"},
	"getCDlevel": {"args": [], "ret": "s16"},
	"addr_init_weekly_sd": {"args": ["s32"], "ret": "void"},
	"addr_init_holiday_sd": {"args": ["s32"], "ret": "void"},
	"addr_init_club_sd": {"args": ["s32"], "ret": "void"},
	"addr_init_else_sd": {"args": ["s32"], "ret": "void"},
	"addr_init_bustup": {"args": [], "ret": "void"},
	"scene_wait_cd_read": {"args": [], "ret": "void"},
	"scene_bg_show": {"args": [], "ret": "void"},
	"scene_bg_hide": {"args": [], "ret": "void"},
	"scene_bg_wipe_in": {"args": [], "ret": "void"},
	"scene_bg_wipe_in_step": {"args": [], "ret": "s32"},
	"set_tarao_bg": {"args": ["s32"], "ret": "void"},
	"set_tarao_sprt": {"args": ["s16", "s16", "s16", "s16", "u8", "u8", "s16", "u8", "u8", "u8"], "ret": "void"},
	"set_tarao_rect": {"args": ["s16", "s16", "s16", "s16", "s16", "u8", "u8", "u8"], "ret": "void"},
	"normal_date_bg_out": {"args": [], "ret": "void"},
	"scene_bg_wipe_out_step": {"args": [], "ret": "s32"},
	"normal_date_girl_in": {"args": [], "ret": "void"},
	"normal_date_girl_in_init": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_girl_in_main": {"args": [], "ret": "s32", "ret_defined": [0]},
	"normal_date_girl_suddenin": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_girl_out": {"args": [], "ret": "void"},
	"normal_date_girl_out_init": {"args": [], "ret": "void"},
	"normal_date_girl_out_main": {"args": [], "ret": "s32", "ret_defined": [0]},
	"normal_date_girl_suddenout": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_bg_fadein": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_bggirl_fadein": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_bg_fadeout": {"args": [], "ret": "void"},
	"normal_date_bggirl_fadeout": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_move_place": {"args": [], "ret": "void"},
	"normal_date_move_place_init": {"args": [], "ret": "void"},
	"normal_date_move_place_main": {"args": [], "ret": "s32", "ret_defined": [0]},
	"place_init": {"args": [], "ret": "void"},
	"place_init2": {"args": [], "ret": "void"},
	"change_dec_bg": {"args": [], "ret": "void"},
	"dec_init": {"args": [], "ret": "s32", "ret_defined": [0]},
	"bg_read_sub2": {"args": ["s32"], "ret": "void"},
	"check_k_scroll": {"args": [], "ret": "void"},
	"wait_sub_sub": {"args": ["s16"], "ret": "void"},
	"talk_speak_part": {"args": ["ptr", "u8", "u8", "ptr", "u16", "u16", "u8"], "ret": "s32"},
	"talk_voice_skip_check": {"args": [], "ret": "void"},
	"talk_render": {"args": ["s16", "s16", "u8", "ptr", "s32", "ptr", "ptr", "u8"], "ret": "s32"},
	"talk_run": {"args": ["u8", "u8", "u8"], "ret": "s32"},
	"make_three_select": {"args": ["ptr"], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"junban_init": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_three_select": {"args": [], "ret": "void"},
	"normal_date_three_select_init": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"normal_date_three_select_main": {"args": [], "ret": "s32", "ret_defined": [0]},
	"normal_date_two_select": {"args": [], "ret": "void"},
	"normal_date_two_select_init": {"args": [], "ret": "void"},
	"normal_date_two_select_main": {"args": [], "ret": "s32", "ret_defined": [0]},
	"face_expression_mood": {"args": [], "ret": "void"},
	"face_expression_0": {"args": [], "ret": "void"},
	"face_expression_1": {"args": [], "ret": "void"},
	"face_expression_2": {"args": [], "ret": "void"},
	"face_expression_3": {"args": [], "ret": "void"},
	"face_expression_4": {"args": [], "ret": "void"},
	"portrait_layout_set": {"args": ["u8"], "ret": "void"},
	"portrait_show_checked": {"args": [], "ret": "s32", "ret_defined": [0]},
	"face_show": {"args": [], "ret": "void"},
	"face_show_checked_hidden": {"args": [], "ret": "s32", "ret_defined": [0]},
	"portrait_wink_update": {"args": [], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"scene_frame_update": {"args": [], "ret": "void"},
	"hizuke_hide": {"args": [], "ret": "void"},
	"hizuke_appear": {"args": [], "ret": "void"},
	"read_bustup": {"args": [], "ret": "void"},
	"read_bustup_uniform": {"args": [], "ret": "void"},
	"read_bustup_swimsuit": {"args": [], "ret": "void"},
	"bustup_return": {"args": [], "ret": "void"},
	"return_step": {"args": [], "ret": "void"},
	"k_disp_inc2": {"args": [], "ret": "void"},
	"yosi_trans": {"args": [], "ret": "void"},
	"change_yoshio": {"args": [], "ret": "void"},
	"change_girl": {"args": [], "ret": "void"},
	"talk_names_set": {"args": ["u8"], "ret": "void"},
	"select_girl": {"args": [], "ret": "void"},
	"select_girl2": {"args": [], "ret": "void"},
	"select_girl_init": {"args": ["u8"], "ret": "void"},
	"select_girl_main": {"args": ["s32"], "ret": "s32", "ret_defined": [0]},
	"sprite_brightness": {"args": ["s16", "u8"], "ret": "void"},
	"day_plus": {"args": [], "ret": "void"},
	"check_para_limit": {"args": [], "ret": "void"},
	"text_marker_hide": {"args": [], "ret": "void"},
	"scene_mouth_lock_clear": {"args": [], "ret": "void"},
	"scene_still_show": {"args": ["s32", "s32", "s32", "s32", "s32", "s32"], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"scene_anim_show": {"args": ["s32", "s32", "s32", "s16"], "ret": "s32", "ret_defined": [0]},
	"scene_still_layer_set": {"args": ["u8"], "ret": "void"},
	"scene_still_fade_in": {"args": [], "ret": "void"},
	"scene_still_fade_out": {"args": [], "ret": "void"},
	"event_face0": {"args": [], "ret": "void"},
	"event_face1": {"args": [], "ret": "void"},
	"event_face2": {"args": [], "ret": "void"},
	"event_face3": {"args": [], "ret": "void"},
	"event_face_kuchi0": {"args": [], "ret": "void"},
	"event_face_kuchi1": {"args": [], "ret": "void"},
	"event_kuchi_ani_no": {"args": [], "ret": "void"},
	"event_kuchi_ani_yes": {"args": [], "ret": "void"},
	"ev_me_on": {"args": [], "ret": "void"},
	"ev_me_on_continue": {"args": [], "ret": "void"},
	"_sprite_set_box_shade_tarao": {"args": ["s32", "s32", "s32", "s32", "s32", "u32", "u32"], "ret": "void"},
	"talk_clear": {"args": [], "ret": "void"},
	"don_init2": {"args": [], "ret": "void"},
	"don_wait": {"args": [], "ret": "void"},
	"normal_date_speak_1line": {"args": [], "ret": "void"},
	"normal_date_speak_012": {"args": [], "ret": "void"},
	"normal_date_speak": {"args": [], "ret": "void"},
	"vram_bustup_clear": {"args": [], "ret": "void"},
	"event_log_add": {"args": ["u8", "u8"], "ret": "s32", "ret_undefined": "the C has no return statement with a value (v0 is whatever the register held)"},
	"portrait_quad_offsets": {"args": ["u8"], "ret": "void"},
}

## game_state (GAME_STATE_BASE of core/game_layout.gd, config/symbol_addrs.txt).
const GS: int = 0x800E6280
## dec_bg_last_screen_mode (config/symbol_addrs*.txt)
const G_DEC_BG_LAST_SCREEN_MODE: int = 0x800B5948
## talk_group (config/symbol_addrs*.txt)
const G_TALK_GROUP: int = 0x800CA148
## talk_line (config/symbol_addrs*.txt)
const G_TALK_LINE: int = 0x800CA14C
## girl_short_name_table (config/symbol_addrs*.txt)
const G_GIRL_SHORT_NAME_TABLE: int = 0x800B35F4
## cd_read_checked (config/symbol_addrs*.txt)
const G_CD_READ_CHECKED: int = 0x800B3D60
## bg_lba_cache (config/symbol_addrs*.txt)
const G_BG_LBA_CACHE: int = 0x800B5920
## D_800B5939 (config/symbol_addrs*.txt)
const G_D_800B5939: int = 0x800B5939
## bg_brightness (config/symbol_addrs*.txt)
const G_BG_BRIGHTNESS: int = 0x800B593C
## bg_fade_amount (config/symbol_addrs*.txt)
const G_BG_FADE_AMOUNT: int = 0x800B5940
## D_800B5A60 (config/symbol_addrs*.txt)
const G_D_800B5A60: int = 0x800B5A60
## sd_table_weekday (config/symbol_addrs*.txt)
const G_SD_TABLE_WEEKDAY: int = 0x800C9F60
## sd_table_holiday (config/symbol_addrs*.txt)
const G_SD_TABLE_HOLIDAY: int = 0x800C9FF8
## sd_table_club (config/symbol_addrs*.txt)
const G_SD_TABLE_CLUB: int = 0x800CA048
## sd_table_else (config/symbol_addrs*.txt)
const G_SD_TABLE_ELSE: int = 0x800CA108
## face_images (config/symbol_addrs*.txt)
const G_FACE_IMAGES: int = 0x800CA120
## face_frame_tables (config/symbol_addrs*.txt)
const G_FACE_FRAME_TABLES: int = 0x800CA124
## face_sequences (config/symbol_addrs*.txt)
const G_FACE_SEQUENCES: int = 0x800CA128
## face_palette (config/symbol_addrs*.txt)
const G_FACE_PALETTE: int = 0x800CA130
## talk_tables (config/symbol_addrs*.txt)
const G_TALK_TABLES: int = 0x800CA134
## talk_active_line_ptr (config/symbol_addrs*.txt)
const G_TALK_ACTIVE_LINE_PTR: int = 0x800CA138
## talk_active_text_table (config/symbol_addrs*.txt)
const G_TALK_ACTIVE_TEXT_TABLE: int = 0x800CA13C
## talk_active_command_table (config/symbol_addrs*.txt)
const G_TALK_ACTIVE_COMMAND_TABLE: int = 0x800CA140
## talk_active_voice_table (config/symbol_addrs*.txt)
const G_TALK_ACTIVE_VOICE_TABLE: int = 0x800CA144
## talk_text_table (config/symbol_addrs*.txt)
const G_TALK_TEXT_TABLE: int = 0x800CA160
## talk_command_table (config/symbol_addrs*.txt)
const G_TALK_COMMAND_TABLE: int = 0x800CA164
## talk_voice_table (config/symbol_addrs*.txt)
const G_TALK_VOICE_TABLE: int = 0x800CA168
## name_code_family (config/symbol_addrs*.txt)
const G_NAME_CODE_FAMILY: int = 0x800CA16C
## name_code_given (config/symbol_addrs*.txt)
const G_NAME_CODE_GIVEN: int = 0x800CA174
## name_code_player_form (config/symbol_addrs*.txt)
const G_NAME_CODE_PLAYER_FORM: int = 0x800CA17C
## name_code_girl_short (config/symbol_addrs*.txt)
const G_NAME_CODE_GIRL_SHORT: int = 0x800CA188
## girl_name_buf (config/symbol_addrs*.txt)
const G_GIRL_NAME_BUF: int = 0x800CA190
## scene_place_title (config/symbol_addrs*.txt)
const G_SCENE_PLACE_TITLE: int = 0x800CA19C
## scene_place_title_2 (config/symbol_addrs*.txt)
const G_SCENE_PLACE_TITLE_2: int = 0x800CA1DC
## name_code_s (config/symbol_addrs*.txt)
const G_NAME_CODE_S: int = 0x800CA23C
## name_code_o (config/symbol_addrs*.txt)
const G_NAME_CODE_O: int = 0x800CA248
## name_code_w (config/symbol_addrs*.txt)
const G_NAME_CODE_W: int = 0x800CA250
## name_code_a (config/symbol_addrs*.txt)
const G_NAME_CODE_A: int = 0x800CA25C
## name_code_m (config/symbol_addrs*.txt)
const G_NAME_CODE_M: int = 0x800CA280
## talk_choice_order (config/symbol_addrs*.txt)
const G_TALK_CHOICE_ORDER: int = 0x800CA2A4
## talk_voice_waiting (config/symbol_addrs*.txt)
const G_TALK_VOICE_WAITING: int = 0x800CA2A8
## talk_voice_wait_frames (config/symbol_addrs*.txt)
const G_TALK_VOICE_WAIT_FRAMES: int = 0x800CA2AC
## select_girl_menu_enabled (config/symbol_addrs*.txt)
const G_SELECT_GIRL_MENU_ENABLED: int = 0x800CA2B0
## talk_text_scroll_pending (config/symbol_addrs*.txt)
const G_TALK_TEXT_SCROLL_PENDING: int = 0x800CA2BC
## talk_next_row_y (config/symbol_addrs*.txt)
const G_TALK_NEXT_ROW_Y: int = 0x800CA2C0
## talk_voice_cancelled (config/symbol_addrs*.txt)
const G_TALK_VOICE_CANCELLED: int = 0x800CA2C4
## D_800CA2C8 (config/symbol_addrs*.txt)
const G_D_800CA2C8: int = 0x800CA2C8
## scene_trip_day_state (config/symbol_addrs*.txt)
const G_SCENE_TRIP_DAY_STATE: int = 0x800CA2F0
## select_girl_menu_x (config/symbol_addrs*.txt)
const G_SELECT_GIRL_MENU_X: int = 0x800CA300
## select_girl_menu_y (config/symbol_addrs*.txt)
const G_SELECT_GIRL_MENU_Y: int = 0x800CA318
## select_girl_menu_width (config/symbol_addrs*.txt)
const G_SELECT_GIRL_MENU_WIDTH: int = 0x800CA330
## select_girl_menu_height (config/symbol_addrs*.txt)
const G_SELECT_GIRL_MENU_HEIGHT: int = 0x800CA348
## scene_mouth_frames_reversed_saved (config/symbol_addrs*.txt)
const G_SCENE_MOUTH_FRAMES_REVERSED_SAVED: int = 0x800CA360
## talk_voice_speaker_by_girl (config/symbol_addrs*.txt)
const G_TALK_VOICE_SPEAKER_BY_GIRL: int = 0x800CA36C
## draw_ot_tags (config/symbol_addrs*.txt)
const G_DRAW_OT_TAGS: int = 0x800E8CA0
## draw_buffer (config/symbol_addrs*.txt)
const G_DRAW_BUFFER: int = 0x8011ECA0
## actor_pool (config/symbol_addrs*.txt)
const G_ACTOR_POOL: int = 0x8011ECD0
## D_8011ECD3 (config/symbol_addrs*.txt)
const G_D_8011ECD3: int = 0x8011ECD3
## cursor_x (config/symbol_addrs*.txt)
const G_CURSOR_X: int = 0x8011ECF6
## cursor_y (config/symbol_addrs*.txt)
const G_CURSOR_Y: int = 0x8011ECFA
## D_8011ED5B (config/symbol_addrs*.txt)
const G_D_8011ED5B: int = 0x8011ED5B
## D_8011F113 (config/symbol_addrs*.txt)
const G_D_8011F113: int = 0x8011F113
## D_80120652 (config/symbol_addrs*.txt)
const G_D_80120652: int = 0x80120652
## D_80120657 (config/symbol_addrs*.txt)
const G_D_80120657: int = 0x80120657
## D_80120666 (config/symbol_addrs*.txt)
const G_D_80120666: int = 0x80120666
## D_80120696 (config/symbol_addrs*.txt)
const G_D_80120696: int = 0x80120696
## D_8012069B (config/symbol_addrs*.txt)
const G_D_8012069B: int = 0x8012069B
## D_801206AA (config/symbol_addrs*.txt)
const G_D_801206AA: int = 0x801206AA
## sprite_quads (config/symbol_addrs*.txt)
const G_SPRITE_QUADS: int = 0x801217D0
## talk_work (config/symbol_addrs*.txt)
const G_TALK_WORK: int = 0x80122CD0
## talk_choice (config/symbol_addrs*.txt)
const G_TALK_CHOICE: int = 0x80122CDC
## D_80122CF4 (config/symbol_addrs*.txt)
const G_D_80122CF4: int = 0x80122CF4
## D_80122CF8 (config/symbol_addrs*.txt)
const G_D_80122CF8: int = 0x80122CF8
## D_80122CFC (config/symbol_addrs*.txt)
const G_D_80122CFC: int = 0x80122CFC
## D_80122D04 (config/symbol_addrs*.txt)
const G_D_80122D04: int = 0x80122D04
## D_80122D0C (config/symbol_addrs*.txt)
const G_D_80122D0C: int = 0x80122D0C
## D_80122D10 (config/symbol_addrs*.txt)
const G_D_80122D10: int = 0x80122D10
## D_80122D1C (config/symbol_addrs*.txt)
const G_D_80122D1C: int = 0x80122D1C
## D_80122D20 (config/symbol_addrs*.txt)
const G_D_80122D20: int = 0x80122D20
## D_80122D2C (config/symbol_addrs*.txt)
const G_D_80122D2C: int = 0x80122D2C
## talk_menu_active (config/symbol_addrs*.txt)
const G_TALK_MENU_ACTIVE: int = 0x80122D30
## D_80122D3C (config/symbol_addrs*.txt)
const G_D_80122D3C: int = 0x80122D3C
## talk_rank_override (config/symbol_addrs*.txt)
const G_TALK_RANK_OVERRIDE: int = 0x80122D48
## bg_still_bitstream (config/symbol_addrs*.txt)
const G_BG_STILL_BITSTREAM: int = 0x80125C04
## D_80125CA4 (config/symbol_addrs*.txt)
const G_D_80125CA4: int = 0x80125CA4
## D_80125CA8 (config/symbol_addrs*.txt)
const G_D_80125CA8: int = 0x80125CA8
## D_80125CAC (config/symbol_addrs*.txt)
const G_D_80125CAC: int = 0x80125CAC
## D_80125CB0 (config/symbol_addrs*.txt)
const G_D_80125CB0: int = 0x80125CB0
## audio_state (config/symbol_addrs*.txt)
const G_AUDIO_STATE: int = 0x80125D10
## D_80125D14 (config/symbol_addrs*.txt)
const G_D_80125D14: int = 0x80125D14
## audio_cd_level (config/symbol_addrs*.txt)
const G_AUDIO_CD_LEVEL: int = 0x80125E60
## D_80125E62 (config/symbol_addrs*.txt)
const G_D_80125E62: int = 0x80125E62
## D_80126080 (config/symbol_addrs*.txt)
const G_D_80126080: int = 0x80126080

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

## Samples CD audio and updates the stereo level-meter values.
func SD_GetCDLevel(arg0: int) -> void:
	arg0 = Math.u32(arg0)
	var shift: int = 0
	var left: int = 0
	var right: int = 0
	engine.memory.write_u16(arg0, 0)
	engine.memory.write_u16(arg0 + 0x2, 0)
	if (Math.u32(engine.memory.read_global("D_80125D14")) >> 24) == 0:
		if not ((engine.memory.u32(G_AUDIO_STATE) & 0x4000) != 0) and Math.s32(engine.call_c("func_80094B80", [0])) == 1:
			if Math.s32(engine.call_c("SsVabTransCompleted", [0])) == 1:
				engine.call_c("func_80094C60", [0])
				engine.call_c("func_80094C90", [Math.s32(G_D_80126080), 0x200])
				engine.call_c("func_80094B80", [1])
				engine.call_c("func_80094C60", [0x400])
				engine.call_c("func_80094C90", [Math.s32(G_D_80126080 + 0x400), 0x200])
				engine.call_c("func_80094B80", [1])
				match engine.memory.u16(arg0 + 0x8) & 0xF:
					1:
						engine.call_c("SD_DetectCDPeak", [arg0, arg0 + 0x2, 0, engine.memory.u16(arg0 + 0x6)])
					2:
						engine.call_c("SD_CalcCDAve", [arg0, arg0 + 0x2, 0, engine.memory.u16(arg0 + 0x6)])
				left = engine.memory.u16(arg0)
				right = engine.memory.u16(arg0 + 0x2)
				if (engine.memory.u16(arg0 + 0x8) & 0xF0) == 0x20:
					left = Math.u32(left * left) >> 15
					right = Math.u32(right * right) >> 15
				match engine.memory.u16(arg0 + 0x8) & 0xF00:
					0x100:
						if left < 0x3333:
							left = Math.u32(left * 4)
						else:
							left = (left >> 2) + 0xC000
						if right < 0x3333:
							right = Math.u32(right * 4)
						else:
							right = (right >> 2) + 0xC000
					0x400:
						if left < 0x9999:
							left = _div(Math.u32(left * 4), 6)
						else:
							left = Math.u32((Math.u32(left * 6) >> 2) + 0xFFFF7FFF)
						if right < 0x9999:
							right = _div(Math.u32(right * 4), 6)
						else:
							right = Math.u32((Math.u32(right * 6) >> 2) + 0xFFFF7FFF)
					0x800:
						if left < 0xB333:
							left = _div(Math.u32(left * 2), 7)
						else:
							left = Math.u32(_div(Math.u32(left * 8), 3) + 0xFFFE5555)
						if right < 0xB333:
							right = _div(Math.u32(right * 2), 7)
						else:
							right = Math.u32(_div(Math.u32(right * 8), 3) + 0xFFFE5555)
				shift = Math.s32(0x10 - engine.memory.u16(arg0 + 0x4))
				engine.memory.write_u16(arg0, (left >> (shift & 31)) & 0xFFFF)
				engine.memory.write_u16(arg0 + 0x2, (right >> (shift & 31)) & 0xFFFF)

## Finds sampled left and right CD audio peak levels.
func SD_DetectCDPeak(l: int, r: int, pos: int, n: int) -> void:
	l = Math.u32(l)
	r = Math.u32(r)
	pos = Math.u16(pos)
	n = Math.u16(n)
	var i: int = 0
	var step: int = 0
	var a: int = 0
	var b: int = 0
	i = 0
	if n != 0:
		step = _div(0x100, n)
		while true:
			a = engine.memory.s16(G_D_80126080 + pos * 2)
			b = engine.memory.s16(G_D_80126080 + (pos + 0x200) * 2)
			if (a & 0x8000) != 0:
				a = Math.s16(~(a - 1))
			if (b & 0x8000) != 0:
				b = Math.s16(~(b - 1))
			if engine.memory.u16(l) < Math.u32(a):
				engine.memory.write_u16(l, a)
			if engine.memory.u16(r) < Math.u32(b):
				engine.memory.write_u16(r, b)
			pos = Math.u16(pos + step)
			i = Math.u16(i + 1)
			if not (i < n):
				break
	engine.memory.write_u16(l, engine.memory.u16(l) * 2)
	engine.memory.write_u16(r, engine.memory.u16(r) * 2)

## Calculates average absolute left and right CD audio levels.
func SD_CalcCDAve(left: int, right: int, start: int, count: int) -> void:
	left = Math.u32(left)
	right = Math.u32(right)
	start = Math.u16(start)
	count = Math.u16(count)
	var left_sum: int = 0
	var right_sum: int = 0
	var index: int = 0
	var left_sample: int = 0
	var right_sample: int = 0
	left_sum = 0
	right_sum = 0
	index = 0
	while index < count:
		left_sample = engine.memory.s16(G_D_80126080 + start * 2)
		right_sample = engine.memory.s16(G_D_80126080 + (start + 0x200) * 2)
		if (left_sample & 0x8000) != 0:
			left_sample = Math.s16(~(left_sample - 1))
		left_sum = Math.s32(left_sum + left_sample)
		if (right_sample & 0x8000) != 0:
			right_sample = Math.s16(~(right_sample - 1))
		right_sum = Math.s32(right_sum + right_sample)
		start = Math.u16(start + _div(0x100, count))
		index = Math.u16(index + 1)
	engine.memory.write_u16(left, (_div(Math.u32(left_sum * 2), count)) & 0xFFFF)
	engine.memory.write_u16(right, (_div(Math.u32(right_sum * 2), count)) & 0xFFFF)

## Returns the current averaged CD audio level when the meter is active.
func getCDlevel() -> int:
	if (engine.memory.u32(G_AUDIO_STATE) & 0x400) != 0:
		return Math.s16((engine.memory.read_global("audio_cd_level") + engine.memory.read_global("D_80125E62")) >> 1)
	return 0

## Resolves the three weekday scene-data addresses for an indexed entry.
func addr_init_weekly_sd(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var p: int = 0
	engine.memory.write_global("D_80125CA4", 0x801E0000)
	p = Math.u32(G_SD_TABLE_WEEKDAY + (_mod(arg0, 0x19) * 3) * 2)
	engine.memory.write_global("D_80125CB0", engine.memory.u16(p) + 0x801E0000)
	engine.memory.write_global("D_80125CA8", engine.memory.u16(p + 0x2) + 0x801E0000)
	engine.memory.write_global("D_80125CAC", engine.memory.u16(p + 0x4) + 0x801E0000)

## Resolves the three holiday scene-data addresses for an indexed entry.
func addr_init_holiday_sd(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var p: int = 0
	engine.memory.write_global("D_80125CA4", 0x801E0000)
	p = Math.u32(G_SD_TABLE_HOLIDAY + (_mod(arg0, 0xD) * 3) * 2)
	engine.memory.write_global("D_80125CB0", engine.memory.u16(p) + 0x801E0000)
	engine.memory.write_global("D_80125CA8", engine.memory.u16(p + 0x2) + 0x801E0000)
	engine.memory.write_global("D_80125CAC", engine.memory.u16(p + 0x4) + 0x801E0000)

## Resolves the three club scene-data addresses for an indexed table entry.
func addr_init_club_sd(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var p: int = 0
	engine.memory.write_global("D_80125CA4", 0x801E0000)
	p = Math.u32(G_SD_TABLE_CLUB + (_mod(arg0, 0x20) * 3) * 2)
	engine.memory.write_global("D_80125CB0", engine.memory.u16(p) + 0x801E0000)
	engine.memory.write_global("D_80125CA8", engine.memory.u16(p + 0x2) + 0x801E0000)
	engine.memory.write_global("D_80125CAC", engine.memory.u16(p + 0x4) + 0x801E0000)

## Resolves the three miscellaneous scene-data addresses for an indexed entry.
func addr_init_else_sd(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var p: int = 0
	engine.memory.write_global("D_80125CA4", 0x801E0000)
	p = Math.u32(G_SD_TABLE_ELSE + (_mod(arg0, 3) * 3) * 2)
	engine.memory.write_global("D_80125CB0", engine.memory.u16(p) + 0x801E0000)
	engine.memory.write_global("D_80125CA8", engine.memory.u16(p + 0x2) + 0x801E0000)
	engine.memory.write_global("D_80125CAC", engine.memory.u16(p + 0x4) + 0x801E0000)

## Set the portrait sprite addresses for the current girl
func addr_init_bustup() -> void:
	engine.memory.write_global("face_images", 0x80180084)
	engine.memory.write_global("face_frame_tables", 0x80180098)
	engine.memory.write_global("face_sequences", 0x8018009C)
	engine.memory.write_global("face_palette", 0x801800AC)

func scene_wait_cd_read() -> void:
	var pad: int = 0
	var sp2B: int = 0
	var sp2A: int = 0
	var sp29: int = 0
	if (Math.u8(engine.call_c("cd_read_sync_status")) & 1) != 0:
		engine.call_c("mode_step_next")
		return
	var _t1: int = engine.memory.s32(GS + 0x1104)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))
	if Math.u32(_t1) >= 0x401:
		engine.call_c("cd_command_reset")
		sp29 = engine.memory.read_global("D_800B5939")
		sp2A = engine.memory.u8(G_DEC_BG_LAST_SCREEN_MODE)
		sp2B = engine.memory.read_global("bg_brightness")
		engine.call_c("dec_bg_reset")
		engine.memory.write_u8(G_DEC_BG_LAST_SCREEN_MODE, sp2A)
		engine.memory.write_global("bg_brightness", sp2B)
		engine.memory.write_global("D_800B5939", sp29)
		engine.gs_set("mode_step", Math.u32(engine.gs_get("mode_step") - 1))

func scene_bg_show() -> void:
	engine.memory.write_global("D_80122CF8", 1)
	engine.memory.write_global("bg_brightness", 0x80)
	engine.call_c("mode_step_next")

func scene_bg_hide() -> void:
	engine.memory.write_global("D_80122CF8", 0)
	engine.memory.write_global("bg_brightness", 0)
	engine.call_c("mode_step_next")

func scene_bg_wipe_in() -> void:
	if Math.s32(engine.call_c("scene_bg_wipe_in_step")) == 1:
		engine.call_c("mode_step_next")

func scene_bg_wipe_in_step() -> int:
	var top_y: int = 0
	var bottom_y: int = 0
	var left_x: int = 0
	var outer: int = 0
	var g: int = 0
	var width: int = 0
	g = GS
	engine.call_c("srand", [Math.s32(engine.memory.u32(g + 0x10F8))])
	if engine.memory.u32(g + 0x1104) == 0:
		engine.memory.write_global("D_80122D3C", _mod(engine.memory.u8(g + 0x3F), 7))
	if engine.memory.read_global("D_80122D3C") != 6:
		engine.memory.write_global("D_80122CF8", 2)
	engine.memory.write_global("bg_brightness", engine.memory.read_global("bg_brightness") + 8)
	if engine.memory.read_global("bg_brightness") >= 0x80:
		engine.memory.write_global("bg_brightness", 0x80)
	match engine.memory.read_global("D_80122D3C"):
		1:
			outer = Math.s16(engine.memory.u32(g + 0x1104) * 5)
			while outer < 0x50:
				if engine.halted():
					break
				engine.call_c("prim_gradient_line", [-0xA0, Math.s16(-0x29 - outer), 0xA0, Math.s16(-0x29 - outer), 0, 0, 0xD])
				engine.call_c("prim_gradient_line", [-0xA0, Math.s16(outer - 0x29), 0xA0, Math.s16(outer - 0x29), 0, 0, 0xD])
				outer = Math.s16(outer + 1)
		2:
			outer = Math.s16(engine.memory.u32(g + 0x1104))
			while outer < 0x10:
				if engine.halted():
					break
				left_x = 0
				while left_x < 0xA:
					if engine.halted():
						break
					top_y = Math.s16(left_x * 0x10 + outer - 0x78)
					engine.call_c("prim_gradient_line", [-0xA0, top_y, 0xA0, top_y, 0, 0, 0xD])
					left_x = Math.s16(left_x + 1)
				outer = Math.s16(outer + 1)
		3:
			outer = Math.s16(engine.memory.u32(g + 0x1104) << 5)
			while outer < 0x1E0:
				if engine.halted():
					break
				if outer < 0xA0:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0xA0), -0x78, -0xA0, Math.s16(outer - 0x78), 0, 0, 0xD])
				elif outer < 0x140:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0xA0), -0x78, Math.s16(outer - 0x140), 0x27, 0, 0, 0xD])
				else:
					engine.call_c("prim_gradient_line", [0xA0, Math.s16(outer - 0x1B8), Math.s16(outer - 0x140), 0x27, 0, 0, 0xD])
				outer = Math.s16(outer + 1)
		4:
			outer = Math.s16(engine.memory.u32(g + 0x1104) << 6)
			while outer < 0x1E0:
				if engine.halted():
					break
				if outer < 0xA0:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0xA0), -0x78, -0xA0, Math.s16(outer - 0x78), 0, 0, 0xD])
				elif outer < 0x140:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0xA0), -0x78, Math.s16(outer - 0x140), 0x27, 0, 0, 0xD])
				else:
					engine.call_c("prim_gradient_line", [0xA0, Math.s16(outer - 0x1B8), Math.s16(outer - 0x140), 0x27, 0, 0, 0xD])
				outer = Math.s16(outer + 4)
			outer = Math.s16(engine.memory.u32(g + 0x1104) << 6)
			while outer < 0x1E0:
				if engine.halted():
					break
				if outer < 0xA0:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0x9F), -0x78, -0xA0, Math.s16(outer - 0x77), 0, 0, 0xD])
				elif outer < 0x140:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0x9F), -0x78, Math.s16(outer - 0x13F), 0x27, 0, 0, 0xD])
				else:
					engine.call_c("prim_gradient_line", [0xA0, Math.s16(outer - 0x1B7), Math.s16(outer - 0x13F), 0x27, 0, 0, 0xD])
				outer = Math.s16(outer + 4)
			if (Math.u32(engine.memory.u32(g + 0x1104) << 6)) < 0x1E0:
				left_x = 0
			else:
				left_x = Math.s16(Math.u32(engine.memory.u32(g + 0x1104) << 6) - 0x1E0)
			outer = left_x
			while outer < 0x1E0:
				if engine.halted():
					break
				if outer < 0xA0:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0x9E), -0x78, -0xA0, Math.s16(outer - 0x76), 0, 0, 0xD])
				elif outer < 0x140:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0x9E), -0x78, Math.s16(outer - 0x13E), 0x27, 0, 0, 0xD])
				else:
					engine.call_c("prim_gradient_line", [0xA0, Math.s16(outer - 0x1B6), Math.s16(outer - 0x13E), 0x27, 0, 0, 0xD])
				outer = Math.s16(outer + 4)
			outer = left_x
			while outer < 0x1E0:
				if engine.halted():
					break
				if outer < 0xA0:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0x9D), -0x78, -0xA0, Math.s16(outer - 0x75), 0, 0, 0xD])
				elif outer < 0x140:
					engine.call_c("prim_gradient_line", [Math.s16(outer - 0x9D), -0x78, Math.s16(outer - 0x13D), 0x27, 0, 0, 0xD])
				else:
					engine.call_c("prim_gradient_line", [0xA0, Math.s16(outer - 0x1B5), Math.s16(outer - 0x13D), 0x27, 0, 0, 0xD])
				outer = Math.s16(outer + 4)
		5:
			outer = Math.s16(engine.memory.u32(g + 0x1104) * 0xA)
			while outer < 0xA0:
				if engine.halted():
					break
				width = 0xA0 - outer
				left_x = Math.s16(width - 0xA0)
				top_y = Math.s16(_div(width, 2) - 0x78)
				engine.call_c("prim_gradient_line", [left_x, top_y, Math.s16(0x9F - width), top_y, 0, 0, 0xD])
				bottom_y = Math.s16(0x27 - _div(width, 2))
				engine.call_c("prim_gradient_line", [left_x, bottom_y, Math.s16(0x9F - width), bottom_y, 0, 0, 0xD])
				engine.call_c("prim_gradient_line", [left_x, top_y, left_x, bottom_y, 0, 0, 0xD])
				engine.call_c("prim_gradient_line", [Math.s16(0xA0 - width), top_y, Math.s16(0x9F - width), bottom_y, 0, 0, 0xD])
				outer = Math.s16(outer + 1)
		6:
			if engine.memory.u32(g + 0x1104) < 0x10:
				engine.call_c("set_tarao_bg", [engine.memory.u32(g + 0x1104) >> 1])
			else:
				engine.call_c("set_tarao_bg", [8])
		_:
			outer = 0
			while outer < 0xF:
				if engine.halted():
					break
				if (outer << 5) < 0xA0:
					left_x = Math.s16(engine.memory.u32(g + 0x1104) * 2)
					while left_x < 0x20:
						if engine.halted():
							break
						engine.call_c("prim_gradient_line", [Math.s16((outer << 5) + left_x - 0xA0), -0x78, -0xA0, Math.s16((outer << 5) + left_x - 0x78), 0, 0, 0xD])
						left_x = Math.s16(left_x + 1)
				elif (outer << 5) < 0x140:
					left_x = Math.s16(engine.memory.u32(g + 0x1104) * 2)
					while left_x < 0x20:
						if engine.halted():
							break
						engine.call_c("prim_gradient_line", [Math.s16((outer << 5) + left_x - 0xA0), -0x78, Math.s16((outer << 5) + left_x - 0x140), 0x28, 0, 0, 0xD])
						left_x = Math.s16(left_x + 1)
				else:
					left_x = Math.s16(engine.memory.u32(g + 0x1104) * 2)
					while left_x < 0x20:
						if engine.halted():
							break
						engine.call_c("prim_gradient_line", [0xA0, Math.s16((outer << 5) + left_x - 0x1B8), Math.s16((outer << 5) + left_x - 0x140), 0x28, 0, 0, 0xD])
						left_x = Math.s16(left_x + 1)
				outer = Math.s16(outer + 1)
	engine.memory.write_u32(g + 0x1104, Math.u32(engine.memory.u32(g + 0x1104) + 1))
	if engine.memory.u32(g + 0x1104) >= 0x10:
		engine.memory.write_global("D_80122CF8", 2)
		return 1
	return 0

## Draws the background mosaic wipe as a grid of textured strips.
func set_tarao_bg(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var row: int = 0
	var col: int = 0
	var x: int = 0
	var y: int = 0
	var row_y: int = 0
	var offset: int = 0
	row = 0
	while row < 0x14:
		if engine.halted():
			break
		row_y = row * 8
		col = 0
		while col < 0x10:
			if engine.halted():
				break
			x = col * 0xA
			offset = _mod(col, 2) * 8
			y = Math.s32(row_y + offset)
			engine.call_c("set_tarao_sprt", [x, y, 0xA, arg0, x, y, 0xC, 0x80, 0x80, 0x80])
			col = Math.s16(col + 2)
		col = 1
		while col < 0x10:
			if engine.halted():
				break
			x = col * 0xA
			offset = _mod(col, 2) * 8
			y = Math.s32(Math.s32(row_y + offset) - arg0)
			engine.call_c("set_tarao_sprt", [x, y, 0xA, arg0, x, y, 0xC, 0x80, 0x80, 0x80])
			col = Math.s16(col + 2)
		row = Math.s16(row + 1)
	if engine.memory.read_global("D_800B5939") != 0:
		engine.call_c("goto_tpage", [0x140, 0x100, 0xC, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x2C0, 0, 0xC, 0, 2])
	row = 0
	while row < 0x14:
		if engine.halted():
			break
		row_y = row * 8
		col = 0x10
		while col < 0x20:
			if engine.halted():
				break
			x = col * 0xA
			offset = _mod(col, 2) * 8
			y = Math.s32(row_y + offset)
			engine.call_c("set_tarao_sprt", [x, y, 0xA, arg0, Math.s32(x - 0x80), y, 0xC, 0x80, 0x80, 0x80])
			col = Math.s16(col + 2)
		col = 0x11
		while col < 0x20:
			if engine.halted():
				break
			x = col * 0xA
			offset = _mod(col, 2) * 8
			y = Math.s32(Math.s32(row_y + offset) - arg0)
			engine.call_c("set_tarao_sprt", [x, y, 0xA, arg0, Math.s32(x - 0x80), y, 0xC, 0x80, 0x80, 0x80])
			col = Math.s16(col + 2)
		row = Math.s16(row + 1)
	if engine.memory.read_global("D_800B5939") != 0:
		engine.call_c("goto_tpage", [0x1E0, 0x100, 0xC, 0, 2])
	else:
		engine.call_c("goto_tpage", [0x360, 0, 0xC, 0, 2])

## K&R parameters preserve promoted callers; the helper narrows them. T-10870.
## Adds a textured sprite with the supplied position, UV and color.
func set_tarao_sprt(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int, arg7: int, arg8: int, arg9: int) -> void:
	arg0 = Math.s16(arg0)
	arg1 = Math.s16(arg1)
	arg2 = Math.s16(arg2)
	arg3 = Math.s16(arg3)
	arg4 = Math.u8(arg4)
	arg5 = Math.u8(arg5)
	arg6 = Math.s16(arg6)
	arg7 = Math.u8(arg7)
	arg8 = Math.u8(arg8)
	arg9 = Math.u8(arg9)
	var p: int = 0
	p = Math.u32(engine.call_c("GetWorkBase", [0x14, engine.memory.read_global("draw_buffer")]))
	engine.call_c("func_8009F0A4", [p])
	engine.call_c("SetSemiTrans", [p, 0])
	engine.call_c("SetShadeTex", [p, 0])
	engine.memory.write_u8(p + 0x4, arg7)
	engine.memory.write_u8(p + 0x5, arg8)
	engine.memory.write_u8(p + 0x6, arg9)
	engine.memory.write_u16(p + 0x8, arg0)
	engine.memory.write_u16(p + 0xA, arg1)
	engine.memory.write_u16(p + 0x10, arg2)
	engine.memory.write_u16(p + 0x12, arg3)
	engine.memory.write_u8(p + 0xC, arg4)
	engine.memory.write_u8(p + 0xD, arg5)
	if engine.memory.read_global("draw_buffer") == 0:
		engine.memory.write_u16(p + 0xA, engine.memory.s16(p + 0xA) + 0xF0)
	engine.call_c("AddPrim", [Math.u32(Math.u32(G_DRAW_OT_TAGS + (engine.memory.read_global("draw_buffer") << 10)) + arg6 * 4), Math.s32(p)])

## Adds a colored rectangle primitive to the current draw ordering table.
func set_tarao_rect(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int, arg7: int) -> void:
	arg0 = Math.s16(arg0)
	arg1 = Math.s16(arg1)
	arg2 = Math.s16(arg2)
	arg3 = Math.s16(arg3)
	arg4 = Math.s16(arg4)
	arg5 = Math.u8(arg5)
	arg6 = Math.u8(arg6)
	arg7 = Math.u8(arg7)
	var p: int = 0
	p = Math.u32(engine.call_c("GetWorkBase", [0x10, engine.memory.read_global("draw_buffer")]))
	engine.call_c("func_8009F0F4", [p])
	engine.call_c("SetSemiTrans", [p, 0])
	engine.call_c("SetShadeTex", [p, 0])
	engine.memory.write_u8(p + 0x4, arg5)
	engine.memory.write_u8(p + 0x5, arg6)
	engine.memory.write_u8(p + 0x6, arg7)
	engine.memory.write_u16(p + 0x8, arg0)
	engine.memory.write_u16(p + 0xA, arg1)
	engine.memory.write_u16(p + 0xC, arg2)
	engine.memory.write_u16(p + 0xE, arg3)
	if engine.memory.read_global("draw_buffer") == 0:
		engine.memory.write_u16(p + 0xA, engine.memory.s16(p + 0xA) + 0xF0)
	engine.call_c("AddPrim", [Math.u32(Math.u32(G_DRAW_OT_TAGS + (engine.memory.read_global("draw_buffer") << 10)) + arg4 * 4), Math.s32(p)])

## Advances the normal-date scene after the background wipe finishes.
func normal_date_bg_out() -> void:
	if Math.s32(engine.call_c("scene_bg_wipe_out_step")) == 1:
		engine.call_c("mode_step_next")

func scene_bg_wipe_out_step() -> int:
	var line_y: int = 0
	var bottom_y: int = 0
	var inner: int = 0
	var outer: int = 0
	var limit: int = 0
	var coordinate: int = 0
	engine.memory.write_global("bg_brightness", Math.u32(engine.memory.read_global("bg_brightness") - 8))
	if engine.memory.read_global("bg_brightness") >= 0x81:
		engine.memory.write_global("bg_brightness", 0)
	if engine.memory.u32(GS + 0x1104) == 0:
		engine.memory.write_global("D_80122D3C", _mod(engine.gs_get("month"), 7))
	if engine.memory.read_global("D_80122D3C") == 6:
		engine.memory.write_global("D_80122CF8", 0)
	match engine.memory.read_global("D_80122D3C"):
		1:
			limit = Math.s16(engine.memory.u32(GS + 0x1104) * 5)
			if limit > 0x50:
				limit = 0x50
			outer = 0
			while outer < limit:
				if engine.halted():
					break
				engine.call_c("prim_gradient_line", [-0xA0, Math.s16(outer - 0x78), 0xA0, Math.s16(outer - 0x78), 0, 0, 6])
				engine.call_c("prim_gradient_line", [-0xA0, Math.s16(0x27 - outer), 0xA0, Math.s16(0x27 - outer), 0, 0, 6])
				outer = Math.s16(outer + 1)
		2:
			limit = Math.s16(engine.memory.u16(GS + 0x1104))
			if limit >= 0x11:
				limit = 0x10
			outer = 0
			if limit > 0:
				while true:
					if engine.halted():
						break
					inner = 0
					while true:
						if engine.halted():
							break
						line_y = Math.s16(inner * 0x10 - outer - 0x69)
						engine.call_c("prim_gradient_line", [-0xA0, line_y, 0xA0, line_y, 0, 0, 6])
						inner = Math.s16(inner + 1)
						if not (inner < 0xA):
							break
					outer = Math.s16(outer + 1)
					if not (outer < limit):
						break
		3:
			limit = Math.s16(engine.memory.u32(GS + 0x1104) << 5)
			if limit >= 0x1E1:
				limit = 0x1E0
			outer = 0
			if limit > 0:
				while true:
					if engine.halted():
						break
					if outer >= 0x141:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0xA0), -0x78, -0xA0, Math.s16(0x1DF - outer - 0x78), 0, 0, 0xD])
					elif outer >= 0xA1:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0xA0), -0x78, Math.s16(0x1DF - outer - 0x140), 0x27, 0, 0, 0xD])
					else:
						engine.call_c("prim_gradient_line", [0xA0, Math.s16(0x1DF - outer - 0x1B8), Math.s16(0x1DF - outer - 0x140), 0x27, 0, 0, 0xD])
					outer = Math.s16(outer + 1)
					if not (outer < limit):
						break
		4:
			limit = Math.s16(engine.memory.u32(GS + 0x1104) << 6)
			if limit >= 0x1E1:
				limit = 0x1E0
			outer = 0
			if limit > 0:
				while true:
					if engine.halted():
						break
					if outer >= 0x141:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0xA0), -0x78, -0xA0, Math.s16(0x1DF - outer - 0x78), 0, 0, 0xD])
					elif outer >= 0xA1:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0xA0), -0x78, Math.s16(0x1DF - outer - 0x140), 0x27, 0, 0, 0xD])
					else:
						engine.call_c("prim_gradient_line", [0xA0, Math.s16(0x1DF - outer - 0x1B8), Math.s16(0x1DF - outer - 0x140), 0x27, 0, 0, 0xD])
					outer = Math.s16(outer + 4)
					if not (outer < limit):
						break
				outer = 0
			if limit > 0:
				while true:
					if engine.halted():
						break
					if outer >= 0x141:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0x9F), -0x78, -0xA0, Math.s16(0x1DF - outer - 0x77), 0, 0, 0xD])
					elif outer >= 0xA1:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0x9F), -0x78, Math.s16(0x1DF - outer - 0x13F), 0x27, 0, 0, 0xD])
					else:
						engine.call_c("prim_gradient_line", [0xA0, Math.s16(0x1DF - outer - 0x1B7), Math.s16(0x1DF - outer - 0x13F), 0x27, 0, 0, 0xD])
					outer = Math.s16(outer + 4)
					if not (outer < limit):
						break
				outer = 0
			limit = Math.s16(Math.u32(engine.memory.u32(GS + 0x1104) << 6) - 0x1E0)
			if limit >= 0x1E1:
				limit = 0x1E0
			if limit > 0:
				while true:
					if engine.halted():
						break
					if outer >= 0x141:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0x9E), -0x78, -0xA0, Math.s16(0x1DF - outer - 0x76), 0, 0, 0xD])
					elif outer >= 0xA1:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0x9E), -0x78, Math.s16(0x1DF - outer - 0x13E), 0x27, 0, 0, 0xD])
					else:
						engine.call_c("prim_gradient_line", [0xA0, Math.s16(0x1DF - outer - 0x1B6), Math.s16(0x1DF - outer - 0x13E), 0x27, 0, 0, 0xD])
					outer = Math.s16(outer + 4)
					if not (outer < limit):
						break
				outer = 0
			if limit > 0:
				while true:
					if engine.halted():
						break
					if outer >= 0x141:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0x9D), -0x78, -0xA0, Math.s16(0x1DF - outer - 0x75), 0, 0, 0xD])
					elif outer >= 0xA1:
						engine.call_c("prim_gradient_line", [Math.s16(0x1DF - outer - 0x9D), -0x78, Math.s16(0x1DF - outer - 0x13D), 0x27, 0, 0, 0xD])
					else:
						engine.call_c("prim_gradient_line", [0xA0, Math.s16(0x1DF - outer - 0x1B5), Math.s16(0x1DF - outer - 0x13D), 0x27, 0, 0, 0xD])
					outer = Math.s16(outer + 4)
					if not (outer < limit):
						break
		5:
			limit = Math.s16(engine.memory.u32(GS + 0x1104) * 0xA)
			if limit >= 0xA1:
				limit = 0xA0
			outer = 0
			if limit > 0:
				while true:
					if engine.halted():
						break
					inner = Math.s16(outer - 0xA0)
					coordinate = _div(outer, 2)
					line_y = Math.s16(coordinate - 0x78)
					engine.call_c("prim_gradient_line", [inner, line_y, Math.s16(0x9F - outer), line_y, 0, 0, 0xD])
					bottom_y = Math.s16(0x27 - coordinate)
					engine.call_c("prim_gradient_line", [inner, bottom_y, Math.s16(0x9F - outer), bottom_y, 0, 0, 0xD])
					engine.call_c("prim_gradient_line", [inner, line_y, inner, bottom_y, 0, 0, 0xD])
					engine.call_c("prim_gradient_line", [Math.s16(0xA0 - outer), line_y, Math.s16(0x9F - outer), bottom_y, 0, 0, 0xD])
					outer = Math.s16(outer + 1)
					if not (outer < limit):
						break
		6:
			if engine.memory.u32(GS + 0x1104) < 0x10:
				engine.call_c("set_tarao_bg", [Math.s32(8 - (engine.memory.u32(GS + 0x1104) >> 1))])
			else:
				engine.call_c("set_tarao_bg", [0])
		_:
			limit = Math.s16(engine.memory.u32(GS + 0x1104) * 2)
			if limit >= 0x21:
				limit = 0x20
			outer = 0
			while outer < 0xF:
				if engine.halted():
					break
				inner = 0
				if (outer << 5) < 0xA0:
					coordinate = (outer << 5) + 0x1F
					if limit > 0:
						while true:
							if engine.halted():
								break
							engine.call_c("prim_gradient_line", [Math.s16(Math.s32(coordinate - inner) - 0xA0), -0x78, -0xA0, Math.s16(Math.s32(coordinate - inner) - 0x78), 0, 0, 0xD])
							inner = Math.s16(inner + 1)
							if not (inner < limit):
								break
				elif (outer << 5) < 0x140:
					coordinate = (outer << 5) + 0x1F
					if limit > 0:
						while true:
							if engine.halted():
								break
							engine.call_c("prim_gradient_line", [Math.s16(Math.s32(coordinate - inner) - 0xA0), -0x78, Math.s16(Math.s32(coordinate - inner) - 0x140), 0x28, 0, 0, 0xD])
							inner = Math.s16(inner + 1)
							if not (inner < limit):
								break
				else:
					coordinate = (outer << 5) + 0x1F
					if limit > 0:
						while true:
							if engine.halted():
								break
							engine.call_c("prim_gradient_line", [0xA0, Math.s16(Math.s32(coordinate - inner) - 0x1B8), Math.s16(Math.s32(coordinate - inner) - 0x140), 0x28, 0, 0, 0xD])
							inner = Math.s16(inner + 1)
							if not (inner < limit):
								break
				outer = Math.s16(outer + 1)
	engine.memory.write_u32(GS + 0x1104, Math.u32(engine.memory.u32(GS + 0x1104) + 1))
	if engine.memory.u32(GS + 0x1104) >= 0x12:
		engine.memory.write_global("D_80122CF8", 0)
		return 1
	return 0

## Slide the girl's portrait in
func normal_date_girl_in() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("normal_date_girl_in_init")
		1:
			engine.call_c("normal_date_girl_in_main")
		_:
			engine.call_c("game_mode_reset_to_title")
	engine.call_c("bg_fade_draw", [1])

## Initializes the fade-in for the date-partner portrait.
func normal_date_girl_in_init() -> int:
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, 0)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.memory.write_global("bg_fade_amount", 0)
	engine.memory.write_global("D_80122CF8", 0)
	engine.memory.write_global("D_80122CF4", 1)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))
	return 0 # the C falls off the end here (v0 undefined)

## Raises portrait brightness to 0x80, then shows the face sprites and advances.
func normal_date_girl_in_main() -> int:
	if engine.memory.read_global("bg_fade_amount") < 0x80:
		engine.memory.write_global("bg_fade_amount", engine.memory.read_global("bg_fade_amount") + 8)
	else:
		engine.call_c("mode_step_next")
		engine.memory.write_global("D_80122CF8", 1)
		engine.memory.write_u32(G_SPRITE_QUADS, engine.memory.s32(G_SPRITE_QUADS) & 0x7FFFFFFF)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, engine.memory.s32(G_SPRITE_QUADS + 0x24) & 0x7FFFFFFF)
		return 0
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, engine.memory.read_global("bg_fade_amount"))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, engine.memory.u8(G_SPRITE_QUADS + 0x38))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, engine.memory.read_global("bg_fade_amount"))
	return 0 # the C falls off the end here (v0 undefined)

## Shows the date-partner portrait immediately at full brightness.
func normal_date_girl_suddenin() -> int:
	engine.memory.write_global("D_80122CF4", 1)
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, 0x80)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0x80)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0x80)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

## Slide the girl's portrait out
func normal_date_girl_out() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("normal_date_girl_out_init")
		1:
			engine.call_c("normal_date_girl_out_main")
		_:
			engine.call_c("game_mode_reset_to_title")
	engine.call_c("bg_fade_draw", [1])

## Initializes the fade-out for the date-partner portrait.
func normal_date_girl_out_init() -> void:
	engine.memory.write_global("bg_fade_amount", 0x80)
	engine.memory.write_global("D_80122CF8", 0)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))

## Fades out and hides the date-partner portrait.
func normal_date_girl_out_main() -> int:
	if engine.memory.read_global("bg_fade_amount") >= 8:
		engine.memory.write_global("bg_fade_amount", Math.u32(engine.memory.read_global("bg_fade_amount") - 8))
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) & 0xFFFFFF7F)
		engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) & 0xFFFFFF7F)
		engine.call_c("mode_step_next")
		engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
		engine.memory.write_global("D_80122CF4", 0)
		engine.memory.write_global("D_80122CF8", 1)
		return 0
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, engine.memory.read_global("bg_fade_amount"))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, engine.memory.u8(G_SPRITE_QUADS + 0x38))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, engine.memory.read_global("bg_fade_amount"))
	return 0 # the C falls off the end here (v0 undefined)

## Hides the date-partner portrait immediately.
func normal_date_girl_suddenout() -> int:
	engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) & 0xFFFFFF7F)
	engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) & 0xFFFFFF7F)
	engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
	engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0x80)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0x80)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.memory.write_global("D_80122CF4", 0)
	engine.memory.write_global("D_80122CF8", 1)
	engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

## Fade the scene background in
func normal_date_bg_fadein() -> int:
	engine.memory.write_global("D_80122CF8", 1)
	engine.memory.write_global("bg_brightness", engine.memory.read_global("bg_brightness") + 4)
	if engine.memory.read_global("bg_brightness") >= 0x80:
		engine.memory.write_global("bg_brightness", 0x80)
	if engine.memory.read_global("bg_brightness") >= 0x7D:
		engine.memory.write_global("bg_brightness", 0x80)
		engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

## s32 without a return value: the last chain value is then kept in v0 (andi v0,a0,0xff). T-11590
## Fade background and portrait in together
func normal_date_bggirl_fadein() -> int:
	var x: int = 0
	engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) | 0x80)
	engine.call_c("normal_date_bg_fadein")
	x = engine.memory.read_global("bg_brightness")
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, x)
	engine.memory.write_u32(G_SPRITE_QUADS, engine.memory.s32(G_SPRITE_QUADS) & 0x7FFFFFFF)
	engine.memory.write_u32(G_SPRITE_QUADS + 0x24, engine.memory.s32(G_SPRITE_QUADS + 0x24) & 0x7FFFFFFF)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, x)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, x)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.memory.write_global("D_80122CF4", 1)
	return 0 # the C falls off the end here (v0 undefined)

## Fade the scene background out
func normal_date_bg_fadeout() -> void:
	engine.memory.write_global("D_80122CF8", 1)
	engine.memory.write_global("bg_brightness", Math.u32(engine.memory.read_global("bg_brightness") - 4))
	if engine.memory.read_global("bg_brightness") >= 0x81:
		engine.memory.write_global("bg_brightness", 0)
	if engine.memory.read_global("bg_brightness") < 4:
		engine.memory.write_global("D_80122CF8", 0)
		engine.memory.write_global("bg_brightness", 0)
		engine.call_c("mode_step_next")

## s32 without a return value, as normal_date_bggirl_fadein. T-11590
## Fade background and portrait out together
func normal_date_bggirl_fadeout() -> int:
	var x: int = 0
	if engine.memory.read_global("bg_brightness") < 8:
		engine.memory.write_global("D_80122CF4", 0)
		engine.memory.write_global("D_80122CF8", 0)
		engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) & 0xFFFFFF7F)
		engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) & 0xFFFFFF7F)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
		engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
	engine.call_c("normal_date_bg_fadeout")
	x = engine.memory.read_global("bg_brightness")
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, x)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, x)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, x)
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	return 0 # the C falls off the end here (v0 undefined)

## Runs the normal-date background transition between places.
func normal_date_move_place() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("normal_date_move_place_init")
			return
		1:
			engine.call_c("normal_date_move_place_main")
			return
		_:
			engine.call_c("game_mode_reset_to_title")
			return

## Initializes the normal-date place transition and crossfade.
func normal_date_move_place_init() -> void:
	engine.call_c("k_reset", [1])
	engine.memory.write_global("bg_fade_amount", 0x80)
	engine.memory.write_global("D_80122CF8", 0)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))
	engine.call_c("bg_crossfade_draw", [1])

## Fades the outgoing date background and switches to the decoded destination.
func normal_date_move_place_main() -> int:
	if engine.memory.read_global("bg_fade_amount") >= 8:
		engine.memory.write_global("bg_fade_amount", Math.u32(engine.memory.read_global("bg_fade_amount") - 8))
	else:
		engine.call_c("bg_crossfade_draw", [1])
		engine.call_c("dec_bg_show_switch", [1])
		engine.call_c("mode_step_next")
		engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x16, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
		engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
		engine.memory.write_global("D_80122CF8", 1)
		return 0
	engine.memory.write_u8(G_SPRITE_QUADS + 0x3A, engine.memory.read_global("bg_fade_amount"))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x39, engine.memory.u8(G_SPRITE_QUADS + 0x3A))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x38, engine.memory.u8(G_SPRITE_QUADS + 0x39))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x16, engine.memory.u8(G_SPRITE_QUADS + 0x38))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x15, engine.memory.u8(G_SPRITE_QUADS + 0x16))
	engine.memory.write_u8(G_SPRITE_QUADS + 0x14, engine.memory.u8(G_SPRITE_QUADS + 0x15))
	engine.call_c("bg_crossfade_draw", [1])
	return 0 # the C falls off the end here (v0 undefined)

## Clears dialogue and installs the first stored place title before advancing.
func place_init() -> void:
	engine.call_c("k_reset", [0])
	engine.call_c("message_window_set_title", [G_SCENE_PLACE_TITLE])
	engine.call_c("mode_step_next")

## Clears dialogue and installs the second stored place title before advancing.
func place_init2() -> void:
	engine.call_c("k_reset", [0])
	engine.call_c("message_window_set_title", [G_SCENE_PLACE_TITLE_2])
	engine.call_c("mode_step_next")

## Shows the decoded background and advances the scene step.
func change_dec_bg() -> void:
	engine.call_c("dec_bg_show_switch", [1])
	engine.call_c("mode_step_next")

## Validates and decodes the current still-background bitstream.
func dec_init() -> int:
	if (engine.memory.u32(Math.u32(engine.memory.read_global("bg_still_bitstream"))) & 0xFFFF0000) != 0x38000000 or ((engine.memory.u32(Math.u32(engine.memory.read_global("bg_still_bitstream")) + 0x4)) & 0xFF010000) != 0x10000:
		engine.memory.write_u32(G_BG_LBA_CACHE + 0x8, 0)
		engine.memory.write_u32(G_BG_LBA_CACHE + 0xC, 0)
		engine.call_c("mode_step_next")
		engine.gs_set("mode_step", Math.u32(engine.gs_get("mode_step") - 3))
		return 0
	engine.call_c("dec_bg_decode_to_vram", [1])
	engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

## Background read helper: starts the read of the scene background
func bg_read_sub2(arg0: int) -> void:
	arg0 = Math.s32(arg0)
	var t: int = 0
	engine.memory.write_global("cd_read_checked", 0)
	t = Math.s32(engine.call_c("dec_bg_cd_read", [arg0, 1]))
	if Math.u32(t) == engine.memory.read_global("D_800B5939"):
		engine.call_c("mode_step_next")
		engine.call_c("mode_step_next")
		engine.call_c("mode_step_next")
	elif Math.u32(t) == (Math.u32(1 - engine.memory.read_global("D_800B5939"))):
		engine.call_c("mode_step_next")
		engine.call_c("mode_step_next")

## Scrolls active dialogue entries upward when text reaches the bottom of the window.
func check_k_scroll() -> void:
	var sp38: int = 0
	var sp48: int = 0
	var temp_v1: int = 0
	var temp_a0: int = 0
	var temp_v0: int = 0
	var var_a0: int = 0
	var var_s0: int = 0
	var temp_s2: int = 0
	var temp_v0_2: int = 0
	var temp_v0_3: int = 0
	engine.call_c("get_ksys")
	temp_v0 = Math.s32(engine.call_c("get_now_k"))
	temp_s2 = temp_v0 & 0xFF
	sp48 = Math.u32(engine.call_c("get_k_work", [temp_v0]))
	var_a0 = Math.s32(temp_s2 + 1)
	temp_a0 = var_a0
	sp38 = temp_a0
	engine.call_c("get_k_work", [temp_a0])
	if engine.memory.s16(sp48 + 0x2) >= 0x60:
		var_a0 = sp38
		if engine.memory.read_global("talk_text_scroll_pending") != 0:
			var_s0 = 1
			if temp_s2 >= 2:
				while true:
					if engine.halted():
						break
					temp_v0_2 = Math.u32(engine.call_c("get_k_work", [var_s0]))
					temp_v1 = engine.memory.s16(temp_v0_2 + 0x2)
					if Math.u32(temp_v1) == 0x30:
						engine.memory.write_u8(temp_v0_2 + 0x7, 0)
					else:
						engine.memory.write_u16(temp_v0_2 + 0x2, Math.s16(temp_v1 - 0x10))
					var_s0 = Math.s16(var_s0 + 1)
					if not (Math.u32(var_s0) < temp_s2):
						break
			engine.memory.write_global("talk_text_scroll_pending", 0)
			engine.memory.write_u16(sp48 + 0x2, 0x50)
			engine.memory.write_u8(sp48 + 0x7, 1)
			var_a0 = sp38
	temp_v0_3 = Math.u32(engine.call_c("get_k_work", [var_a0]))
	if engine.memory.s16(temp_v0_3 + 0x2) >= 0x60:
		engine.memory.write_global("talk_text_scroll_pending", 1)
		engine.memory.write_u8(temp_v0_3 + 0x7, 0)

## Waits the requested number of frames before advancing the mode step.
func wait_sub_sub(wait_frames: int) -> void:
	wait_frames = Math.s16(wait_frames)
	var elapsed_frames: int = 0
	elapsed_frames = Math.s32(engine.memory.s32(GS + 0x1104) + 1)
	engine.memory.write_u32(GS + 0x1104, elapsed_frames)
	if Math.u32(elapsed_frames) >= Math.u32(wait_frames):
		engine.call_c("mode_step_next")

## Matched: T-24021. The two incremented-Y calls need an early argument definition.
func talk_speak_part(line_text: int, speak_code: int, choice: int, voice_table: int, group_index: int, line_index: int, text_color: int) -> int:
	line_text = Math.u32(line_text)
	speak_code = Math.u8(speak_code)
	choice = Math.u8(choice)
	voice_table = Math.u32(voice_table)
	group_index = Math.u16(group_index)
	line_index = Math.u16(line_index)
	text_color = Math.u8(text_color)
	var temp_s1: int = 0
	var var_v1: int = 0
	var dialogue: int = 0
	var line_length: int = 0
	var var_a1_2: int = 0
	var render_text: int = 0
	var p: int = 0
	var render_x: int = 0
	dialogue = G_TALK_WORK
	engine.memory.write_bytes(engine.frame_base() + 0x58, engine.memory.read_bytes(G_TALK_VOICE_SPEAKER_BY_GIRL, 16))
	if speak_code != 0xFF:
		engine.call_c("strcpy", [engine.frame_base() + 0x7C, line_text])
	engine.memory.write_u32(dialogue + 0x20, 0)
	engine.memory.write_global("talk_voice_waiting", 0)
	temp_s1 = Math.s16(Math.s32(engine.memory.s32(dialogue + 0x58) * 0x10) + 0x30)
	match speak_code:
		7:
			if ((engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0) and engine.memory.u8(GS + 0x3C) != 0:
				var _t1: int = engine.memory.read_global("talk_voice_wait_frames")
				engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
				if _t1 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
					engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
					engine.call_c("audio_command", [0x300])
				if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
					if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
						engine.memory.write_global("talk_voice_waiting", 1)
						engine.call_c("talk_voice_skip_check")
						if engine.memory.read_global("talk_voice_waiting") == 1:
							return 0
				else:
					engine.memory.write_global("talk_voice_wait_frames", 0)
					engine.call_c("audio_queue_clear")
					engine.call_c("audio_command", [0x74])
					return 0x64
				engine.memory.write_u32(dialogue + 0x20, 1)
				engine.memory.write_global("talk_voice_wait_frames", 0)
			if (engine.memory.u8(engine.frame_base() + 0x7C)) == 0x3E:
				engine.call_c("strncpy", [engine.frame_base() + 0x74, engine.frame_base() + 0x7C + 0x1, 4])
				engine.memory.write_u8(engine.frame_base() + 0x74 + 0x4, 0)
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2000, engine.frame_base() + 0x74])
				engine.call_c("set_kanji_string", [-0x78, temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2004, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C + 0x5), 0, 0x800B2008, 0x800B200C, 0])
			elif (engine.memory.u8(engine.frame_base() + 0x7C)) == 0x2D:
				engine.call_c("strncpy", [engine.frame_base() + 0x74, engine.frame_base() + 0x7C + 0x1, 6])
				engine.memory.write_u8(engine.frame_base() + 0x74 + 0x6, 0)
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2010, engine.frame_base() + 0x74])
				engine.call_c("set_kanji_string", [-0x80, temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2014, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C + 0x7), 0, 0x800B2018, 0x800B201C, 0])
			elif (engine.memory.u8(engine.frame_base() + 0x7C)) == 0x2B:
				engine.call_c("strncpy", [engine.frame_base() + 0x74, engine.frame_base() + 0x7C + 0x1, 2])
				engine.memory.write_u8(engine.frame_base() + 0x74 + 0x2, 0)
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2020, engine.frame_base() + 0x74])
				engine.call_c("set_kanji_string", [-0x70, temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2024, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C + 0x3), 0, 0x800B2028, 0x800B202C, 0])
			elif (engine.memory.u8(engine.frame_base() + 0x7C)) == 0x3C:
				engine.call_c("strncpy", [engine.frame_base() + 0x74, engine.frame_base() + 0x7C + 0x1, 4])
				engine.memory.write_u8(engine.frame_base() + 0x74 + 0x4, 0)
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2030, engine.frame_base() + 0x74])
				engine.call_c("set_kanji_string", [-0x78, temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2034, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C + 0x5), 0, 0x800B2038, 0x800B203C, 0])
			else:
				render_x = Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B2040]))
				if render_x == 0:
					render_x = (render_x & 0) - 0x28
					temp_s1 = Math.s16(temp_s1 + 0x10)
					engine.call_c("talk_render", [render_x, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B204C, 0x800B2050, 0])
				elif (((Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B2054]))) == 0 or (Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B205C]))) == 0) or (Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B2064]))) == 0) or (Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B206C]))) == 0:
					engine.call_c("talk_render", [-0x80, temp_s1, 2, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2078, 0x800B207C, 0])
				elif (Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B2080]))) == 0 or (Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B208C]))) == 0:
					engine.call_c("talk_render", [-0x80, temp_s1, 1, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2094, 0x800B2098, 0])
				else:
					render_x = Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B209C]))
					if render_x == 0:
						render_x = (render_x & 0) - 0x28
						temp_s1 = Math.s16(temp_s1 + 0x10)
						engine.call_c("talk_render", [render_x, temp_s1, 2, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20A8, 0x800B20AC, 0])
					else:
						engine.call_c("talk_render", [-0x80, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20B0, 0x800B20B4, 0])
			engine.memory.write_u32(dialogue + 0x14, 1)
		0x10:
			engine.call_c("talk_render", [-0x80, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20B8, 0x800B20BC, 0])
			engine.memory.write_u32(dialogue + 0x30, 1)
		0x11:
			engine.call_c("talk_render", [-0x48, engine.memory.read_global("talk_next_row_y"), text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20C0, 0x800B20C4, 0])
			engine.memory.write_u32(dialogue + 0x30, 1)
		9:
			if (engine.memory.u8(engine.frame_base() + 0x7C)) != 0:
				if (Math.s32(engine.call_c("strcmp", [engine.frame_base() + 0x7C, 0x800B20C8]))) == 0:
					engine.call_c("talk_render", [-0x80, temp_s1, 1, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20D0, 0x800B20D4, 0])
				engine.call_c("talk_render", [-0x80, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20D8, 0x800B20DC, 0])
		0:
			line_length = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x7C]))
			if (engine.memory.u8(engine.frame_base() + 0x7C + line_length - 1)) == 0x6E and (engine.memory.u8(engine.frame_base() + 0x7C + line_length - 2)) == 0x6E:
				if engine.memory.s32(dialogue + 0xC) == 0:
					engine.memory.write_u8(engine.frame_base() + 0x7C + line_length - 2, 0)
					line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_FAMILY]))
					engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B20E0, G_NAME_CODE_FAMILY])
					engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
					engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B20E4, 0])
					engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20E8, 0x800B20EC, 0])
					engine.memory.write_u32(dialogue + 0x14, 1)
			else:
				line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_FAMILY]))
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B20F0, G_NAME_CODE_FAMILY])
				engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B20F4, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B20F8, 0x800B20FC, 0])
				engine.memory.write_u32(dialogue + 0x14, 1)
		0xE:
			line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_FAMILY]))
			engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2100, G_NAME_CODE_FAMILY])
			engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
			engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2104, 0])
			engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2108, 0x800B210C, 0])
		1:
			line_length = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x7C]))
			if ((engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0) and (engine.memory.u8(GS + 0x3C) != 0 and (((engine.memory.u8(engine.frame_base() + 0x7C + line_length - 1)) != 0x6E or (engine.memory.u8(engine.frame_base() + 0x7C + line_length - 2)) != 0x6E) or engine.memory.s32(dialogue + 0xC) == 0)):
				var _t2: int = engine.memory.read_global("talk_voice_wait_frames")
				engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
				if _t2 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
					engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
					engine.call_c("audio_command", [0x300])
				if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
					if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
						engine.memory.write_global("talk_voice_waiting", 1)
						engine.call_c("talk_voice_skip_check")
						if engine.memory.read_global("talk_voice_waiting") == 1:
							return 0
				else:
					engine.memory.write_global("talk_voice_wait_frames", 0)
					engine.call_c("audio_queue_clear")
					engine.call_c("audio_command", [0x74])
					return 0x64
				engine.memory.write_u32(dialogue + 0x20, 1)
				engine.memory.write_global("talk_voice_wait_frames", 0)
			p = engine.frame_base() + 0x7C + line_length
			if engine.memory.u8(p - 0x1) == 0x6E and engine.memory.u8(p - 0x2) == 0x6E:
				if engine.memory.s32(dialogue + 0xC) == 0:
					engine.memory.write_u8(p - 0x2, 0)
					line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_GIRL_SHORT]))
					engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2110, G_NAME_CODE_GIRL_SHORT])
					engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
					engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2114, 0])
					engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2118, 0x800B211C, 0])
					engine.memory.write_u32(dialogue + 0x14, 1)
			else:
				line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_GIRL_SHORT]))
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2120, G_NAME_CODE_GIRL_SHORT])
				engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2124, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2128, 0x800B212C, 0])
				engine.memory.write_u32(dialogue + 0x14, 1)
		0xF:
			if ((engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0) and engine.memory.u8(GS + 0x3C) != 0:
				var _t3: int = engine.memory.read_global("talk_voice_wait_frames")
				engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
				if _t3 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
					engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
					engine.call_c("audio_command", [0x300])
				if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
					if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
						engine.memory.write_global("talk_voice_waiting", 1)
						engine.call_c("talk_voice_skip_check")
						if engine.memory.read_global("talk_voice_waiting") == 1:
							return 0
				else:
					engine.memory.write_global("talk_voice_wait_frames", 0)
					engine.call_c("audio_queue_clear")
					engine.call_c("audio_command", [0x74])
					return 0x64
				engine.memory.write_u32(dialogue + 0x20, 1)
				engine.memory.write_global("talk_voice_wait_frames", 0)
			line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_GIRL_SHORT]))
			engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2130, G_NAME_CODE_GIRL_SHORT])
			engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
			engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2134, 0])
			engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2138, 0x800B213C, 0])
			engine.memory.write_u32(dialogue + 0x48, 1)
		0xC:
			engine.memory.write_global("talk_voice_cancelled", 0)
			line_length = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x7C]))
			if ((engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0) and engine.memory.u8(GS + 0x3C) != 0:
				var _t4: int = engine.memory.read_global("talk_voice_wait_frames")
				engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
				if _t4 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
					engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
					engine.call_c("audio_command", [0x300])
				if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
					if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
						engine.memory.write_global("talk_voice_waiting", 1)
						engine.call_c("talk_voice_skip_check")
						if engine.memory.read_global("talk_voice_waiting") == 1:
							return 0
				else:
					engine.memory.write_global("talk_voice_wait_frames", 0)
					engine.call_c("audio_queue_clear")
					engine.call_c("audio_command", [0x74])
					return 0x64
				engine.memory.write_u32(dialogue + 0x20, 1)
				engine.memory.write_global("talk_voice_wait_frames", 0)
			p = engine.frame_base() + 0x7C + line_length
			if engine.memory.u8(p - 0x1) == 0x6E and engine.memory.u8(p - 0x2) == 0x6E:
				if engine.memory.s32(dialogue + 0xC) == 0:
					engine.memory.write_u8(p - 0x2, 0)
					line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_GIRL_SHORT]))
					engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2140, G_NAME_CODE_GIRL_SHORT])
					engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
					engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2144, 0])
					engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2148, 0x800B214C, 0])
					engine.memory.write_u32(dialogue + 0x30, 1)
			else:
				line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_GIRL_SHORT]))
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2150, G_NAME_CODE_GIRL_SHORT])
				engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2154, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2158, 0x800B215C, 0])
				engine.memory.write_u32(dialogue + 0x30, 1)
		0xD:
			line_length = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x7C]))
			if (((engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0) and engine.memory.u8(GS + 0x3C) != 0) and engine.memory.read_global("talk_voice_cancelled") == 0:
				var _t5: int = engine.memory.read_global("talk_voice_wait_frames")
				engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
				if _t5 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
					engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
					engine.call_c("audio_command", [0x300])
				if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
					if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
						engine.memory.write_global("talk_voice_waiting", 1)
						engine.call_c("talk_voice_skip_check")
						if engine.memory.read_global("talk_voice_waiting") == 1:
							return 0
				else:
					engine.memory.write_global("talk_voice_wait_frames", 0)
					engine.call_c("audio_queue_clear")
					engine.call_c("audio_command", [0x74])
					return 0x64
				engine.memory.write_u32(dialogue + 0x20, 1)
				engine.memory.write_global("talk_voice_wait_frames", 0)
			p = engine.frame_base() + 0x7C + line_length
			if engine.memory.u8(p - 0x1) == 0x6E and engine.memory.u8(p - 0x2) == 0x6E:
				if engine.memory.s32(dialogue + 0xC) == 0:
					engine.memory.write_u8(p - 0x2, 0)
					engine.call_c("talk_render", [-0x48, engine.memory.read_global("talk_next_row_y"), text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2160, 0x800B2164, 0])
					engine.memory.write_u32(dialogue + 0x14, 1)
			else:
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2168, 0])
				engine.call_c("talk_render", [-0x48, engine.memory.read_global("talk_next_row_y"), text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B216C, 0x800B2170, 0])
				engine.memory.write_u32(dialogue + 0x14, 1)
		0x12:
			engine.call_c("strlen", [engine.frame_base() + 0x7C])
			if (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0:
				if engine.memory.u8(GS + 0x3C) != 0:
					if engine.memory.read_global("talk_voice_cancelled") == 0:
						var _t6: int = engine.memory.read_global("talk_voice_wait_frames")
						engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
						if _t6 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
							engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
							engine.call_c("audio_command", [0x300])
						if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
							if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
								engine.memory.write_global("talk_voice_waiting", 1)
								engine.call_c("talk_voice_skip_check")
								if engine.memory.read_global("talk_voice_waiting") == 1:
									return 0
						else:
							engine.memory.write_global("talk_voice_wait_frames", 0)
							engine.call_c("audio_queue_clear")
							engine.call_c("audio_command", [0x74])
							return 0x64
						engine.memory.write_u32(dialogue + 0x20, 1)
						engine.memory.write_global("talk_voice_wait_frames", 0)
			engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2174, 0])
			engine.call_c("talk_render", [-0x48, engine.memory.read_global("talk_next_row_y"), text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2178, 0x800B217C, 0])
			engine.memory.write_u32(dialogue + 0x30, 1)
		0xB:
			line_length = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x7C]))
			if (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0:
				if engine.memory.u8(GS + 0x3C) != 0:
					var _t7: int = engine.memory.read_global("talk_voice_wait_frames")
					engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
					if _t7 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
						engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), 0])
						engine.call_c("audio_command", [0x300])
					if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
						if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
							engine.memory.write_global("talk_voice_waiting", 1)
							engine.call_c("talk_voice_skip_check")
							if engine.memory.read_global("talk_voice_waiting") == 1:
								return 0
					else:
						engine.memory.write_global("talk_voice_wait_frames", 0)
						engine.call_c("audio_queue_clear")
						engine.call_c("audio_command", [0x74])
						return 0x64
					engine.memory.write_u32(dialogue + 0x20, 1)
					engine.memory.write_global("talk_voice_wait_frames", 0)
			p = engine.frame_base() + 0x7C + line_length
			if engine.memory.u8(p - 0x1) == 0x6E and engine.memory.u8(p - 0x2) == 0x6E:
				if engine.memory.s32(dialogue + 0xC) == 0:
					engine.memory.write_u8(p - 0x2, 0)
					engine.call_c("set_kanji_string", [Math.s16(-0x68 - Math.s16(engine.call_c("strlen", [0x800B2180])) * 4), temp_s1, text_color, 0x800B2188, 0])
					engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B2190, 0])
					engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2194, 0x800B2198, 0])
					engine.memory.write_u32(dialogue + 0x14, 1)
			else:
				engine.call_c("set_kanji_string", [Math.s16(-0x68 - Math.s16(engine.call_c("strlen", [0x800B219C])) * 4), temp_s1, text_color, 0x800B21A4, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B21AC, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B21B0, 0x800B21B4, 0])
				engine.memory.write_u32(dialogue + 0x14, 1)
		0xA:
			line_length = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x7C]))
			if (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4))) != 0 or (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4)) != 0:
				if engine.memory.u8(GS + 0x3C) != 0:
					var _t8: int = engine.memory.read_global("talk_voice_wait_frames")
					engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
					if _t8 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
						engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4)), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + 0x4), 0xD])
						engine.call_c("audio_command", [0x300])
					if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
						if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
							engine.memory.write_global("talk_voice_waiting", 1)
							engine.call_c("talk_voice_skip_check")
							if engine.memory.read_global("talk_voice_waiting") == 1:
								return 0
					else:
						engine.memory.write_global("talk_voice_wait_frames", 0)
						engine.call_c("audio_queue_clear")
						engine.call_c("audio_command", [0x74])
						return 0x64
					engine.memory.write_u32(dialogue + 0x20, 1)
					engine.memory.write_global("talk_voice_wait_frames", 0)
			p = engine.frame_base() + 0x7C + line_length
			if engine.memory.u8(p - 0x1) == 0x6E and engine.memory.u8(p - 0x2) == 0x6E:
				if engine.memory.s32(dialogue + 0xC) == 0:
					engine.memory.write_u8(p - 0x2, 0)
					engine.call_c("set_kanji_string", [Math.s16(-0x68 - Math.s16(engine.call_c("strlen", [0x800B21B8])) * 4), temp_s1, text_color, 0x800B21C0, 0])
					engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B21C8, 0])
					engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B21CC, 0x800B21D0, 0])
					engine.memory.write_u32(dialogue + 0x14, 1)
			else:
				engine.call_c("set_kanji_string", [Math.s16(-0x68 - Math.s16(engine.call_c("strlen", [0x800B21D4])) * 4), temp_s1, text_color, 0x800B21DC, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B21E4, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B21E8, 0x800B21EC, 0])
				engine.memory.write_u32(dialogue + 0x14, 1)
		4, 6:
			line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_FAMILY]))
			engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B21F0, G_NAME_CODE_FAMILY])
			engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
			engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B21F4, 0])
			engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B21F8, 0x800B21FC, 0])
			engine.memory.write_u32(dialogue + 0x14, 1)
		3:
			if choice != 0xFF:
				engine.call_c("junban_init")
				engine.call_c("make_three_select", [engine.frame_base() + 0x7C])
			else:
				engine.memory.write_u8(G_TALK_CHOICE_ORDER, 0)
				engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x1, 1)
				engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x2, 2)
			engine.call_c("k_disp_start", [Math.s32(engine.call_c("talk_render", [-0x72, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2200, 0x800B2204, 1]))])
		8:
			engine.memory.write_u8(G_TALK_CHOICE_ORDER, 0)
			engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x1, 1)
			engine.call_c("k_disp_start", [Math.s32(engine.call_c("talk_render", [-0x72, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2208, 0x800B220C, 1]))])
		0x13:
			engine.memory.write_u8(G_TALK_CHOICE_ORDER, 0)
			engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x1, 1)
			engine.call_c("k_disp_start", [Math.s32(engine.call_c("talk_render", [-0x72, temp_s1, text_color, Math.s32(engine.frame_base() + 0x7C), 0, 0x800B2210, 0x800B2214, 1]))])
		2, 5:
			if (engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + (Math.s32(engine.memory.s32(dialogue + 0xC) * 2)) * 4)) != 0 and engine.memory.u8(GS + 0x3C) != 0:
				var _t9: int = engine.memory.read_global("talk_voice_wait_frames")
				engine.memory.write_global("talk_voice_wait_frames", engine.memory.read_global("talk_voice_wait_frames") + 1)
				if _t9 == 0 and engine.memory.s32(dialogue + 0x14) == 0:
					engine.call_c("cd_audio_request_set", [engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + (Math.s32(engine.memory.s32(dialogue + 0xC) * 2)) * 4), engine.memory.s32(engine.memory.u32(engine.memory.u32(voice_table + group_index * 4) + line_index * 4) + (Math.s32(Math.s32(engine.memory.s32(dialogue + 0xC) * 2) + 1)) * 4), engine.memory.u8(engine.frame_base() + 0x58 + engine.gs_get("current_girl"))])
					engine.call_c("audio_command", [0x300])
				if engine.memory.read_global("talk_voice_wait_frames") < 0x400:
					if not ((Math.u8(engine.call_c("cd_audio_state")) & 4) != 0) and engine.memory.s32(dialogue + 0x14) == 0:
						engine.memory.write_global("talk_voice_waiting", 1)
						engine.call_c("talk_voice_skip_check")
						if engine.memory.read_global("talk_voice_waiting") == 1:
							return 0
				else:
					engine.memory.write_global("talk_voice_wait_frames", 0)
					engine.call_c("audio_queue_clear")
					engine.call_c("audio_command", [0x74])
					return 0x64
				engine.memory.write_u32(dialogue + 0x20, 1)
				engine.memory.write_global("talk_voice_wait_frames", 0)
			line_length = 0
			var_a1_2 = 0
			var_v1 = 0
			while true:
				if (((((((((((engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x20 or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x6D) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x4D) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x41) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x57) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x4F) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x46) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x59) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x70) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x47) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x50) or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x53:
					var_v1 = Math.s16(var_v1 - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0x71 or (engine.memory.u8(engine.frame_base() + 0x7C + var_v1)) == 0:
					engine.memory.write_u8(engine.frame_base() + 0x7C + var_v1, 0)
					p = engine.frame_base() + 0x7C + line_length
					var_a1_2 = Math.s16(var_a1_2 + 1)
					if choice < Math.u32(var_a1_2):
						break
					line_length = Math.s16(var_v1 + 1)
					var_v1 = Math.s16(var_v1 - 1)
				var_v1 = Math.s16(var_v1 + 2)
			if engine.memory.u8(p) != 0x6E:
				render_text = p
				line_length = Math.s16(engine.call_c("strlen", [G_NAME_CODE_GIRL_SHORT]))
				engine.call_c("sprintf", [engine.frame_base() + 0x68, 0x800B2218, G_NAME_CODE_GIRL_SHORT])
				engine.call_c("set_kanji_string", [Math.s16(-0x68 - line_length * 4), temp_s1, text_color, engine.frame_base() + 0x68, 0])
				engine.call_c("set_kanji_string", [-0x56, temp_s1, text_color, 0x800B221C, 0])
				engine.call_c("talk_render", [-0x48, temp_s1, text_color, Math.s32(render_text), 0, 0x800B2220, 0x800B2224, 0])
				engine.memory.write_u32(dialogue + 0x14, 1)
		0xFF:
			engine.call_c("mode_step_next")
		_:
			engine.call_c("game_mode_reset_to_title")
	return 0

func talk_voice_skip_check() -> void:
	if engine.memory.read_global("talk_voice_wait_frames") > 0x200:
		if (engine.gs_get("pad_pressed") & 0x600060) != 0:
			engine.call_c("audio_command", [0xB4])
			engine.memory.write_global("talk_voice_waiting", 0)
			engine.memory.write_global("talk_voice_wait_frames", 0)
			engine.memory.write_global("talk_voice_cancelled", 1)

## Split a script string into lines and queue them with name code expansion
func talk_render(x: int, y: int, text_color: int, input_text: int, arg4: int, line_suffix: int, speaker_label: int, label_enabled: int) -> int:
	x = Math.s16(x)
	y = Math.s16(y)
	text_color = Math.u8(text_color)
	input_text = Math.u32(input_text)
	arg4 = Math.s32(arg4)
	line_suffix = Math.u32(line_suffix)
	speaker_label = Math.u32(speaker_label)
	label_enabled = Math.u8(label_enabled)
	var i: int = 0
	var length: int = 0
	var scan: int = 0
	var copy: int = 0
	var final_y: int = 0
	var token: int = 0
	var in_index: int = 0
	var row: int = 0
	var out_index: int = 0
	engine.memory.write_bytes(engine.frame_base() + 0x80, PackedByteArray([129, 99, 0]))
	engine.memory.write_bytes(engine.frame_base() + 0x7C, PackedByteArray([129, 100, 0]))
	engine.memory.write_bytes(engine.frame_base() + 0x78, PackedByteArray([129, 117, 0]))
	engine.memory.write_bytes(engine.frame_base() + 0x74, PackedByteArray([129, 66, 0]))
	engine.memory.write_bytes(engine.frame_base() + 0x70, PackedByteArray([129, 65, 0]))
	if engine.memory.u8(input_text) == 0:
		return 0
	engine.call_c("strcpy", [engine.frame_base() + 0x84, 0x800B223C])
	engine.call_c("strcpy", [engine.frame_base() + 0x8C, input_text])
	engine.memory.write_global("D_80122D1C", 1)
	i = Math.s16(engine.call_c("strlen", [engine.frame_base() + 0x8C]))
	scan = 0
	while scan < i:
		if ((((((((((((engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x6D or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x4D) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x41) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x57) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x4F) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x46) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x59) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x53) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x50) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x70) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x47) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x20) or (engine.memory.u8(engine.frame_base() + 0x8C + scan)) == 0x71:
			scan = Math.s16(scan - 1)
		elif (((((engine.memory.u8(engine.frame_base() + 0x8C + scan)) != (engine.memory.u8(engine.frame_base() + 0x80)) or (engine.memory.u8(engine.frame_base() + 0x8C + scan + 1)) != (engine.memory.u8(engine.frame_base() + 0x80 + 0x1))) and ((engine.memory.u8(engine.frame_base() + 0x8C + scan)) != (engine.memory.u8(engine.frame_base() + 0x7C)) or (engine.memory.u8(engine.frame_base() + 0x8C + scan + 1)) != (engine.memory.u8(engine.frame_base() + 0x7C + 0x1)))) and ((engine.memory.u8(engine.frame_base() + 0x8C + scan)) != (engine.memory.u8(engine.frame_base() + 0x78)) or (engine.memory.u8(engine.frame_base() + 0x8C + scan + 1)) != (engine.memory.u8(engine.frame_base() + 0x78 + 0x1)))) and ((engine.memory.u8(engine.frame_base() + 0x8C + scan)) != (engine.memory.u8(engine.frame_base() + 0x74)) or (engine.memory.u8(engine.frame_base() + 0x8C + scan + 1)) != (engine.memory.u8(engine.frame_base() + 0x74 + 0x1)))) and ((engine.memory.u8(engine.frame_base() + 0x8C + scan)) != (engine.memory.u8(engine.frame_base() + 0x70)) or (engine.memory.u8(engine.frame_base() + 0x8C + scan + 1)) != (engine.memory.u8(engine.frame_base() + 0x70 + 0x1))):
			engine.memory.write_global("D_80122D1C", 0)
		scan = Math.s16(scan + 2)
	i = 0
	row = 0
	scan = 0
	while true: # C label parse_next: the goto is a continue
		if engine.halted():
			break
		token = engine.memory.u8(engine.frame_base() + 0x8C + scan)
		if (((((((((token == 0x6D or token == 0x4D) or token == 0x41) or token == 0x57) or token == 0x4F) or token == 0x46) or token == 0x59) or token == 0x53) or token == 0x50) or token == 0x70) or token == 0x47:
			scan = Math.s16(scan - 1)
			scan = Math.s16(scan + 2)
			continue
		if token == 0x20 or token == 0x71:
			engine.memory.write_u8(engine.frame_base() + 0x8C + scan, 0)
			if label_enabled != 0:
				engine.call_c("set_kanji_string", [Math.s16(x - 0xE), Math.s16(y + row), text_color, speaker_label, arg4])
			engine.call_c("strncpy", [engine.frame_base() + 0x2A4, Math.u32(engine.frame_base() + 0x8C + i), 0x40])
			length = Math.s16(scan - i)
			out_index = 0
			in_index = 0
			while out_index < length:
				if engine.halted():
					break
				if (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x50:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_PLAYER_FORM + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x53:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_S])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_S + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_S]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_S])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_S])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x47:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_GIRL_NAME_BUF + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x70:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_FAMILY + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x59:
					engine.call_c("strcpy", [engine.frame_base() + 0x28C, 0x800B2240])
					copy = 0
					if (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C]))) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(engine.frame_base() + 0x28C + copy))
							copy = Math.s16(copy + 1)
							if not (copy < (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x46:
					engine.call_c("sprintf", [engine.frame_base() + 0x28C, 0x800B2248, G_NAME_CODE_FAMILY, G_NAME_CODE_GIVEN])
					copy = 0
					if (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C]))) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(engine.frame_base() + 0x28C + copy))
							copy = Math.s16(copy + 1)
							if not (copy < (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x6D:
					engine.call_c("strcpy", [engine.frame_base() + 0x28C, G_NAME_CODE_GIVEN])
					copy = 0
					if (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C]))) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(engine.frame_base() + 0x28C + copy))
							copy = Math.s16(copy + 1)
							if not (copy < (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x4F:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_O])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_O + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_O]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_O])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_O])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x57:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_W])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_W + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_W]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_W])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_W])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x41:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_A])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_A + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_A]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_A])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_A])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x4D:
					copy = 0
					if Math.s32(engine.call_c("strlen", [G_NAME_CODE_M])) > 0:
						while true:
							if engine.halted():
								break
							engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_M + copy))
							copy = Math.s16(copy + 1)
							if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_M]))):
								break
					engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
					length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_M])) - 1))
					out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_M])))
					out_index = Math.s16(out_index - 2)
					in_index = Math.s16(in_index - 1)
				out_index = Math.s16(out_index + 2)
				in_index = Math.s16(in_index + 2)
			if label_enabled != 0:
				engine.call_c("strcat", [engine.frame_base() + 0x2A4, line_suffix])
			if (engine.memory.u8(engine.frame_base() + 0x2A4)) == (engine.memory.u8(engine.frame_base() + 0x84)) and (engine.memory.u8(engine.frame_base() + 0x2A4 + 0x1)) == (engine.memory.u8(engine.frame_base() + 0x84 + 0x1)):
				engine.call_c("set_kanji_string", [Math.s16(x - 0xE), Math.s16(y + row), text_color, engine.frame_base() + 0x2A4, arg4])
			else:
				engine.call_c("set_kanji_string", [x, Math.s16(y + row), text_color, engine.frame_base() + 0x2A4, arg4])
			row = Math.s16(row + 0x10)
			i = Math.s16(scan + 1)
			scan = Math.s16(scan - 1)
			scan = Math.s16(scan + 2)
			continue
		if token != 0:
			scan = Math.s16(scan + 2)
			continue
		if label_enabled != 0:
			engine.call_c("set_kanji_string", [Math.s16(x - 0xE), Math.s16(y + row), text_color, speaker_label, arg4])
		final_y = y + row
		engine.call_c("strncpy", [engine.frame_base() + 0x2A4, Math.u32(engine.frame_base() + 0x8C + i), 0x40])
		length = Math.s16(scan - i)
		out_index = 0
		in_index = 0
		while out_index < length:
			if engine.halted():
				break
			if (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x50:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_PLAYER_FORM + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_PLAYER_FORM])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x53:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_S])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_S + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_S]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_S])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_S])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x47:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_GIRL_NAME_BUF + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_GIRL_NAME_BUF])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x70:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_FAMILY + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_FAMILY])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x59:
				engine.call_c("strcpy", [engine.frame_base() + 0x28C, 0x800B2250])
				copy = 0
				if (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C]))) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(engine.frame_base() + 0x28C + copy))
						copy = Math.s16(copy + 1)
						if not (copy < (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x46:
				engine.call_c("sprintf", [engine.frame_base() + 0x28C, 0x800B2258, G_NAME_CODE_FAMILY, G_NAME_CODE_GIVEN])
				copy = 0
				if (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C]))) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(engine.frame_base() + 0x28C + copy))
						copy = Math.s16(copy + 1)
						if not (copy < (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x6D:
				engine.call_c("strcpy", [engine.frame_base() + 0x28C, G_NAME_CODE_GIVEN])
				copy = 0
				if (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C]))) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(engine.frame_base() + 0x28C + copy))
						copy = Math.s16(copy + 1)
						if not (copy < (Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x28C])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x4F:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_O])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_O + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_O]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_O])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_O])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x57:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_W])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_W + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_W]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_W])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_W])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x41:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_A])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_A + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_A]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_A])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_A])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			elif (engine.memory.u8(engine.frame_base() + 0x2A4 + out_index)) == 0x4D:
				copy = 0
				if Math.s32(engine.call_c("strlen", [G_NAME_CODE_M])) > 0:
					while true:
						if engine.halted():
							break
						engine.memory.write_u8(engine.frame_base() + 0x2A4 + out_index + copy, engine.memory.u8(G_NAME_CODE_M + copy))
						copy = Math.s16(copy + 1)
						if not (copy < Math.s32(engine.call_c("strlen", [G_NAME_CODE_M]))):
							break
				engine.call_c("strncpy", [engine.frame_base() + 0x2A4 + out_index + copy, Math.u32(Math.u32(engine.frame_base() + 0x8C + i) + in_index) + 0x1, -out_index - copy + 0x40])
				length = Math.s16(length + Math.s32(Math.s32(engine.call_c("strlen", [G_NAME_CODE_M])) - 1))
				out_index = Math.s16(out_index + Math.s32(engine.call_c("strlen", [G_NAME_CODE_M])))
				out_index = Math.s16(out_index - 2)
				in_index = Math.s16(in_index - 1)
			out_index = Math.s16(out_index + 2)
			in_index = Math.s16(in_index + 2)
		if engine.memory.u8(line_suffix) != 0:
			engine.call_c("strcat", [engine.frame_base() + 0x2A4, line_suffix])
		engine.memory.write_global("talk_next_row_y", Math.s32(final_y + 0x10))
		if (engine.memory.u8(engine.frame_base() + 0x2A4)) == (engine.memory.u8(engine.frame_base() + 0x84)) and (engine.memory.u8(engine.frame_base() + 0x2A4 + 0x1)) == (engine.memory.u8(engine.frame_base() + 0x84 + 0x1)):
			final_y = Math.s32(engine.call_c("set_kanji_string", [Math.s16(x - 0xE), Math.s16(y + row), text_color, engine.frame_base() + 0x2A4, arg4]))
		else:
			final_y = Math.s32(engine.call_c("set_kanji_string", [x, Math.s16(y + row), text_color, engine.frame_base() + 0x2A4, arg4]))
		return final_y
	return 0 # only reached after a refusal (the C loop above ends in a return)

func talk_run(choice: int, advance_line: int, text_color: int) -> int:
	choice = Math.u8(choice)
	advance_line = Math.u8(advance_line)
	text_color = Math.u8(text_color)
	var tables: int = 0
	var dialogue: int = 0
	var group: int = 0
	var result: int = 0
	var command: int = 0
	tables = G_TALK_TABLES
	dialogue = G_TALK_WORK
	if engine.memory.s32(dialogue + 0x30) != 0:
		if Math.s32(engine.call_c("check_end_k")) != 0 and ((engine.memory.s32(dialogue + 0x20) == 0 or (engine.memory.s32(dialogue + 0x20) == 1 and Math.u8(engine.call_c("cd_audio_state")) != 4)) or engine.memory.u8(GS + 0x3C) == 0):
			engine.memory.write_u32(dialogue + 0x30, 0)
			engine.memory.write_global("talk_voice_wait_frames", 0)
		elif (engine.gs_get("pad_pressed") & 0x400040) != 0 and engine.memory.u8(GS + 0x3C) == 2:
			engine.call_c("audio_command", [0xB4])
			engine.memory.write_u32(dialogue + 0x30, 2)
			engine.memory.write_global("talk_voice_wait_frames", 0)
			engine.memory.write_global("talk_voice_cancelled", 1)
		if (Math.s32(engine.call_c("check_end_k")) != 0 and engine.memory.s32(dialogue + 0x30) == 2) and Math.u8(engine.call_c("cd_audio_state")) != 4:
			engine.memory.write_u32(dialogue + 0x30, 0)
			engine.memory.write_global("talk_voice_wait_frames", 0)
	elif engine.memory.s32(dialogue + 0x48) != 0:
		if Math.s32(engine.call_c("check_end_k")) != 0 and ((engine.memory.s32(dialogue + 0x20) == 0 or (engine.memory.s32(dialogue + 0x20) == 1 and Math.u8(engine.call_c("cd_audio_state")) != 4)) or engine.memory.u8(GS + 0x3C) == 0):
			engine.call_c("k_reset", [1])
			engine.memory.write_u32(dialogue + 0x48, 0)
			engine.memory.write_global("talk_voice_wait_frames", 0)
	elif engine.memory.s32(dialogue + 0x14) != 0:
		if (engine.gs_get("pad_pressed") & 0x400040) != 0 and engine.memory.u8(GS + 0x3C) == 2:
			engine.call_c("audio_command", [0xB4])
			engine.memory.write_global("talk_voice_cancelled", 1)
			engine.memory.write_u32(dialogue + 0x14, 2)
			engine.memory.write_global("talk_voice_wait_frames", 0)
		if (Math.s32(engine.call_c("check_end_k")) != 0 and engine.memory.s32(dialogue + 0x14) == 2) and Math.u8(engine.call_c("cd_audio_state")) != 4:
			engine.call_c("k_reset", [1])
			engine.memory.write_u32(dialogue + 0x14, 0)
			engine.memory.write_global("D_8011ED5B", engine.memory.read_global("D_8011ED5B") & 0xFF7F)
			engine.memory.write_global("talk_voice_wait_frames", 0)
			engine.memory.write_u32(dialogue + 0x20, 0)
			return 1
		if ((engine.gs_get("pad_pressed") & 0x200020) != 0 and Math.s32(engine.call_c("check_end_k")) != 0) and (((engine.memory.s32(dialogue + 0x20) == 0 or engine.memory.s32(dialogue + 0x20) == 0xA) or (engine.memory.s32(dialogue + 0x20) == 1 and Math.u8(engine.call_c("cd_audio_state")) != 4)) or engine.memory.u8(GS + 0x3C) == 0):
			engine.call_c("k_reset", [1])
			engine.memory.write_u32(dialogue + 0x14, 0)
			engine.memory.write_global("D_8011ED5B", engine.memory.read_global("D_8011ED5B") & 0xFF7F)
			engine.memory.write_global("talk_voice_wait_frames", 0)
			engine.memory.write_u32(dialogue + 0x20, 0)
			return 1
		if Math.s32(engine.call_c("check_end_k")) != 0 and (((engine.memory.s32(dialogue + 0x20) == 0 or engine.memory.s32(dialogue + 0x20) == 0xA) or (engine.memory.s32(dialogue + 0x20) == 1 and Math.u8(engine.call_c("cd_audio_state")) != 4)) or engine.memory.u8(GS + 0x3C) == 0):
			engine.memory.write_global("D_8011ED5B", engine.memory.read_global("D_8011ED5B") | 0x80)
			engine.call_c("hud_text_marker_bob")
		else:
			engine.memory.write_global("D_8011ED5B", engine.memory.read_global("D_8011ED5B") & 0x7F)
	else:
		engine.memory.write_global("D_800CA2C8", 0)
		engine.memory.write_u32(GS + 0x1104, 0)
		group = engine.memory.s16(engine.memory.u32(tables))
		result = Math.s32(engine.call_c("talk_speak_part", [engine.memory.u32(engine.memory.u32(engine.memory.u32(tables + 0x8) + group * 4) + engine.memory.s16(engine.memory.u32(tables + 0x4)) * 4), engine.memory.u8(engine.memory.u32(engine.memory.u32(tables + 0xC) + group * 4) + engine.memory.s16(engine.memory.u32(tables + 0x4))), choice, engine.memory.u32(tables + 0x10), group, engine.memory.s16(engine.memory.u32(tables + 0x4)), text_color]))
		if (advance_line != 0 and engine.memory.read_global("talk_voice_waiting") == 0) and (result & 0xFF) != 0x64:
			if (engine.memory.u8(engine.memory.u32(engine.memory.u32(tables + 0xC) + engine.memory.s16(engine.memory.u32(tables)) * 4) + engine.memory.s16(engine.memory.u32(tables + 0x4)))) == 0xFF:
				engine.memory.write_u16(engine.memory.u32(tables), Math.s16(engine.memory.s16(engine.memory.u32(tables)) + 1))
				engine.memory.write_u16(engine.memory.u32(tables + 0x4), 0)
			else:
				engine.memory.write_u16(engine.memory.u32(tables + 0x4), Math.s16(engine.memory.s16(engine.memory.u32(tables + 0x4)) + 1))
	if (engine.memory.s32(dialogue + 0x30) != 0 or engine.memory.s32(dialogue + 0x14) != 0) or engine.memory.s32(dialogue + 0x48) != 0:
		if Math.s32(engine.call_c("check_end_k")) == 0 and engine.memory.u8(GS + 0x3C) == 0:
			command = engine.memory.u8(engine.memory.u32(engine.memory.u32(tables + 0xC) + engine.memory.s16(engine.memory.u32(tables)) * 4) + engine.memory.s16(engine.memory.u32(tables + 0x4)) - 1)
			if ((((((command == 1 or command == 2) or command == 5) or command == 0xA) or command == 0xB) or command == 0xC) or command == 0xD) and engine.memory.s32(dialogue + 0x2C) == 0:
				if (engine.memory.s32(dialogue + 0x24) != 0 and engine.memory.s32(dialogue + 0x4C) == 0) and engine.memory.s32(dialogue + 0x50) == 0:
					engine.call_c("bustup_speech")
				elif (engine.memory.s32(dialogue + 0x34) != 0 and engine.memory.s32(dialogue + 0x40) == 0) and engine.memory.s32(dialogue + 0x4C) == 0:
					engine.call_c("portrait_event_face_mouth_animation_start")
	return 0

## Reorders the three encoded dialogue choices using the current choice order.
func make_three_select(source_text: int) -> int:
	source_text = Math.u32(source_text)
	var i: int = 0
	var n: int = 0
	var start: int = 0
	n = 0
	start = 0
	i = 0
	while true:
		if engine.halted():
			break
		if engine.memory.u8(source_text + i) == 0x50 or engine.memory.u8(source_text + i) == 0x47:
			i = Math.s16(i - 1)
		elif engine.memory.u8(source_text + i) == 0x71 or engine.memory.u8(source_text + i) == 0x20:
			engine.memory.write_u8(source_text + i, 0)
			var _t2: int = engine.frame_base() + 0x64
			var _t1: int = n
			n = Math.s16(n + 1)
			engine.call_c("strncpy", [_t2 + _t1 * 64, source_text + start, 0x40])
			start = Math.s16(i + 1)
			i = Math.s16(i - 1)
		elif engine.memory.u8(source_text + i) == 0:
			engine.call_c("strncpy", [engine.frame_base() + 0x64 + n * 64, source_text + start, 0x40])
			break
		i = Math.s16(i + 2)
	engine.call_c("sprintf", [source_text, 0x800B2260, engine.frame_base() + 0x64 + engine.memory.u8(G_TALK_CHOICE_ORDER) * 64, engine.frame_base() + 0x64 + engine.memory.u8(G_TALK_CHOICE_ORDER + 0x1) * 64, engine.frame_base() + 0x64 + engine.memory.u8(G_TALK_CHOICE_ORDER + 0x2) * 64])
	return 0 # the C falls off the end here (v0 undefined)

## Initializes a frame-dependent ordering for three dialogue choices.
func junban_init() -> int:
	engine.memory.write_u8(G_TALK_CHOICE_ORDER, _mod(engine.gs_get("frame_count"), 3))
	if engine.memory.u8(G_TALK_CHOICE_ORDER) == 0:
		engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x1, engine.memory.u8(G_TALK_CHOICE_ORDER) + (engine.gs_get("frame_count") & 1) + 1)
		engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x2, engine.memory.u8(G_TALK_CHOICE_ORDER) + ((Math.u32(engine.gs_get("frame_count") + 1)) & 1) + 1)
	elif engine.memory.u8(G_TALK_CHOICE_ORDER) == 1:
		engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x1, Math.u32(Math.u32(engine.memory.u8(G_TALK_CHOICE_ORDER) - 1) + (engine.gs_get("frame_count") & 1) * 2))
		engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x2, Math.u32(Math.u32(engine.memory.u8(G_TALK_CHOICE_ORDER) - 1) + ((Math.u32(engine.gs_get("frame_count") + 1)) & 1) * 2))
	else:
		engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x1, Math.u32(Math.u32(engine.memory.u8(G_TALK_CHOICE_ORDER) - 1) - (engine.gs_get("frame_count") & 1)))
		engine.memory.write_u8(G_TALK_CHOICE_ORDER + 0x2, Math.u32(Math.u32(engine.memory.u8(G_TALK_CHOICE_ORDER) - 1) - ((Math.u32(engine.gs_get("frame_count") + 1)) & 1)))
	return 0 # the C falls off the end here (v0 undefined)

## Runs the three-choice date-dialogue menu.
func normal_date_three_select() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("normal_date_three_select_init")
			return
		1:
			engine.call_c("normal_date_three_select_main")
			return
		_:
			engine.call_c("game_mode_reset_to_title")
			return

## s32 without a return value: the last chain value stays in v0 (the 0x10 height). T-11590
## Initializes the three-choice date-dialogue menu and its cursor state.
func normal_date_three_select_init() -> int:
	var pad: int = 0
	engine.memory.write_u16(engine.frame_base() + 0x4C + 0x4, -0x80)
	engine.memory.write_u16(engine.frame_base() + 0x4C + 0x2, engine.memory.s16(engine.frame_base() + 0x4C + 0x4))
	engine.memory.write_u16(engine.frame_base() + 0x4C, engine.memory.s16(engine.frame_base() + 0x4C + 0x2))
	engine.memory.write_u16(engine.frame_base() + 0x44, 0x30)
	engine.memory.write_u16(engine.frame_base() + 0x44 + 0x2, 0x40)
	engine.memory.write_u16(engine.frame_base() + 0x44 + 0x4, 0x50)
	engine.memory.write_u16(engine.frame_base() + 0x3C + 0x4, 0xFC)
	engine.memory.write_u16(engine.frame_base() + 0x3C + 0x2, engine.memory.s16(engine.frame_base() + 0x3C + 0x4))
	engine.memory.write_u16(engine.frame_base() + 0x3C, engine.memory.s16(engine.frame_base() + 0x3C + 0x2))
	engine.memory.write_u16(engine.frame_base() + 0x34 + 0x4, 0x10)
	engine.memory.write_u16(engine.frame_base() + 0x34 + 0x2, engine.memory.s16(engine.frame_base() + 0x34 + 0x4))
	engine.memory.write_u16(engine.frame_base() + 0x34, engine.memory.s16(engine.frame_base() + 0x34 + 0x2))
	engine.call_c("menu_set", [1, 3, engine.frame_base() + 0x4C, engine.frame_base() + 0x44, engine.frame_base() + 0x3C, engine.frame_base() + 0x34])
	engine.gs_set("menu_cursor_1", 0)
	engine.memory.write_global("talk_menu_active", 1)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))
	return 0 # the C falls off the end here (v0 undefined)

## Handles selection from the three-choice date-dialogue menu.
func normal_date_three_select_main() -> int:
	engine.call_c("menu_check", [1, engine.memory.read_global("cursor_x"), engine.memory.read_global("cursor_y")])
	engine.call_c("menu_cursor_update", [1])
	engine.call_c("menu_bar_show", [1])
	if (engine.gs_get("pad_pressed") & 0x200020) != 0:
		if Math.s32(engine.call_c("check_end_k")) != 0:
			if engine.memory.u8(GS + 0xF90 + 0xE) != 0:
				engine.memory.write_global("talk_choice", engine.memory.u8(G_TALK_CHOICE_ORDER))
			elif engine.memory.u8(GS + 0xF90 + 0x1E) != 0:
				engine.memory.write_global("talk_choice", engine.memory.u8(G_TALK_CHOICE_ORDER + 0x1))
			elif engine.memory.u8(GS + 0xF90 + 0x2E) != 0:
				engine.memory.write_global("talk_choice", engine.memory.u8(G_TALK_CHOICE_ORDER + 0x2))
			else:
				return 0
			engine.memory.write_global("D_8011ECD3", engine.memory.read_global("D_8011ECD3") & 0xFF7F)
			engine.memory.write_global("talk_menu_active", 0)
			engine.call_c("k_reset", [1])
			engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

## Runs the two-choice date-dialogue menu.
func normal_date_two_select() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("normal_date_two_select_init")
			return
		1:
			engine.call_c("normal_date_two_select_main")
			return
		_:
			engine.call_c("game_mode_reset_to_title")
			return

## Initializes the two-choice date-dialogue menu geometry and state.
func normal_date_two_select_init() -> void:
	var pad: int = 0
	engine.memory.write_u16(engine.frame_base() + 0x40 + 0x2, -0x80)
	engine.memory.write_u16(engine.frame_base() + 0x40, engine.memory.s16(engine.frame_base() + 0x40 + 0x2))
	engine.memory.write_u16(engine.frame_base() + 0x3C, 0x30)
	engine.memory.write_u16(engine.frame_base() + 0x3C + 0x2, 0x40)
	engine.memory.write_u16(engine.frame_base() + 0x38 + 0x2, 0xFC)
	engine.memory.write_u16(engine.frame_base() + 0x38, engine.memory.s16(engine.frame_base() + 0x38 + 0x2))
	engine.memory.write_u16(engine.frame_base() + 0x34 + 0x2, 0x10)
	engine.memory.write_u16(engine.frame_base() + 0x34, engine.memory.s16(engine.frame_base() + 0x34 + 0x2))
	engine.call_c("menu_set", [1, 2, engine.frame_base() + 0x40, engine.frame_base() + 0x3C, engine.frame_base() + 0x38, engine.frame_base() + 0x34])
	engine.gs_set("menu_cursor_1", 0)
	engine.memory.write_global("talk_menu_active", 1)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))

## Handles selection from the two-choice date-dialogue menu.
func normal_date_two_select_main() -> int:
	engine.call_c("menu_check", [1, engine.memory.read_global("cursor_x"), engine.memory.read_global("cursor_y")])
	engine.call_c("menu_cursor_update", [1])
	engine.call_c("menu_bar_show", [1])
	if (engine.gs_get("pad_pressed") & 0x200020) != 0:
		if Math.s32(engine.call_c("check_end_k")) != 0:
			if engine.memory.u8(GS + 0xF90 + 0xE) != 0:
				engine.memory.write_global("talk_choice", 0)
			elif engine.memory.u8(GS + 0xF90 + 0x1E) != 0:
				engine.memory.write_global("talk_choice", 1)
			else:
				return 0
			engine.memory.write_global("D_8011ECD3", engine.memory.read_global("D_8011ECD3") & 0xFF7F)
			engine.memory.write_global("talk_menu_active", 0)
			engine.call_c("k_reset", [1])
			engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

func face_expression_mood() -> void:
	engine.call_c("portrait_layout_set", [(Math.s32(engine.call_c("get_g_zyotai_h", [engine.gs_get("current_girl")]))) & 0x7F])
	engine.call_c("mode_step_next")

func face_expression_0() -> void:
	engine.call_c("portrait_layout_set", [0])
	engine.call_c("mode_step_next")

func face_expression_1() -> void:
	engine.call_c("portrait_layout_set", [1])
	engine.call_c("mode_step_next")

func face_expression_2() -> void:
	engine.call_c("portrait_layout_set", [2])
	engine.call_c("mode_step_next")

func face_expression_3() -> void:
	engine.call_c("portrait_layout_set", [3])
	engine.call_c("mode_step_next")

func face_expression_4() -> void:
	engine.call_c("portrait_layout_set", [4])
	engine.call_c("mode_step_next")

func portrait_layout_set(arg0: int) -> void:
	arg0 = Math.u8(arg0)
	engine.call_c("portrait_quad_offsets", [arg0])
	engine.call_c("portrait_value_clear_if_high") # the C also passes 1 argument(s) the callee ignores

func portrait_show_checked() -> int:
	var expression_variant: int = 0
	if ((((((((Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")))) != 0x801820AC or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0x4))) != 0x801860AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0x8))) != 0x8018A0AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0xC))) != 0x8018E0AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0x10))) != 0x801920AC) or Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_sequences")))) != 0x8018005C) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_sequences")) + 0x4))) != 0x80180070) or Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_frame_tables")))) != 0x80180000) and engine.gs_get("game_mode") >= 0x50) and engine.gs_get("game_mode") < 0x70:
		engine.gs_set("mode_step", Math.u32(engine.gs_get("mode_step") - 3))
		engine.call_c("mode_step_next")
		return 0
	if (engine.gs_get("game_mode") >= 0x50 and engine.gs_get("game_mode") < 0x70) and Math.s32(engine.call_c("cd_file_checksum_ok", [0x2D, 0x80180000])) == 0:
		engine.call_c("mode_step_next")
		engine.gs_set("mode_step", Math.u32(engine.gs_get("mode_step") - 3))
		return 0
	expression_variant = (Math.s32(engine.call_c("get_g_zyotai_h", [engine.gs_get("current_girl")]))) & 0x7F
	if engine.gs_get("current_girl") >= 0xC:
		expression_variant = 0
	engine.call_c("portrait_build", [engine.memory.read_global("face_palette"), engine.memory.read_global("face_images"), engine.memory.read_global("face_frame_tables"), engine.memory.read_global("face_sequences"), expression_variant])
	engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

func face_show() -> void:
	var expression_variant: int = 0
	expression_variant = (Math.s32(engine.call_c("get_g_zyotai_h", [engine.gs_get("current_girl")]))) & 0x7F
	if engine.gs_get("current_girl") >= 0xC:
		expression_variant = 0
	engine.call_c("portrait_build", [engine.memory.read_global("face_palette"), engine.memory.read_global("face_images"), engine.memory.read_global("face_frame_tables"), engine.memory.read_global("face_sequences"), expression_variant])
	engine.call_c("mode_step_next")

func face_show_checked_hidden() -> int:
	if (((Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")))) != 0x801820AC or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0x4))) != 0x801860AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0x8))) != 0x8018A0AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0xC))) != 0x8018E0AC) or (Math.u32(engine.memory.s32(Math.u32(engine.memory.read_global("face_images")) + 0x10))) != 0x801920AC:
		engine.gs_set("mode_step", Math.u32(engine.gs_get("mode_step") - 4))
		return 0
	if Math.s32(engine.call_c("cd_file_checksum_ok", [0x2D, 0x80180000])) == 0:
		engine.call_c("mode_step_next")
		engine.gs_set("mode_step", Math.u32(engine.gs_get("mode_step") - 5))
		return 0
	engine.call_c("portrait_build", [engine.memory.read_global("face_palette"), engine.memory.read_global("face_images"), engine.memory.read_global("face_frame_tables"), engine.memory.read_global("face_sequences"), (Math.u8(engine.call_c("get_g_zyotai_h", [engine.gs_get("current_girl")]))) & 0x7F])
	engine.memory.write_u8(G_ACTOR_POOL + 0x843, 0)
	engine.memory.write_u32(G_ACTOR_POOL + 0x874, Math.u32(engine.memory.s32(G_ACTOR_POOL + 0x874)) | 0x81000000)
	engine.memory.write_u32(G_ACTOR_POOL + 0x830, Math.u32(engine.memory.s32(G_ACTOR_POOL + 0x830)) | 0x81000000)
	engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x81000000)
	engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x81000000)
	engine.call_c("mode_step_next")
	return 0 # the C falls off the end here (v0 undefined)

## s32 without a return value: the ~0x80 mask is then loaded into v0 (li v0,-129) as in the original, although
## the byte clears use andi 0xFF7F. T-11590
func portrait_wink_update() -> int:
	if engine.memory.read_global("D_80122CF4") != 0:
		engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) | 0x80)
		engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) | 0x80)
		engine.memory.write_u32(G_SPRITE_QUADS, engine.memory.s32(G_SPRITE_QUADS) & 0x7FFFFFFF)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, engine.memory.s32(G_SPRITE_QUADS + 0x24) & 0x7FFFFFFF)
		engine.call_c("portrait_face_update")
		if engine.memory.read_global("D_80122D20") != 0:
			engine.memory.write_u8(G_SPRITE_QUADS + 0xF, 0)
		if (_mod(engine.gs_get("frame_count"), 0xF0)) == 0:
			engine.call_c("bustup_wink")
	elif engine.memory.read_global("D_80122D04") != 0:
		engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) & 0xFFFFFF7F)
		engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) & 0xFFFFFF7F)
		engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
		if engine.memory.read_global("D_80122D10") == 1:
			if (_mod(engine.gs_get("frame_count"), 0xF0)) == 0:
				engine.call_c("portrait_event_face_mouth_animation_start")
		elif engine.memory.read_global("D_80122D10") == 0:
			engine.call_c("portrait_event_face_mouth_lipsync")
		if (_mod(engine.gs_get("frame_count"), 0xF0)) == 0 and engine.memory.read_global("D_80122D0C") == 0:
			engine.call_c("portrait_event_face_eyes_animation_start")
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x83F, engine.memory.u8(G_ACTOR_POOL + 0x83F) & 0xFFFFFF7F)
		engine.memory.write_u8(G_ACTOR_POOL + 0x7FB, engine.memory.u8(G_ACTOR_POOL + 0x7FB) & 0xFFFFFF7F)
		engine.memory.write_u32(G_SPRITE_QUADS, Math.u32(engine.memory.s32(G_SPRITE_QUADS)) | 0x80000000)
		engine.memory.write_u32(G_SPRITE_QUADS + 0x24, Math.u32(engine.memory.s32(G_SPRITE_QUADS + 0x24)) | 0x80000000)
	return 0 # the C falls off the end here (v0 undefined)

func scene_frame_update() -> void:
	if engine.memory.read_global("D_80122CF8") != 0:
		engine.call_c("dec_bg_draw", [1])
	if (engine.memory.read_global("D_80122D2C") == 0 and engine.memory.read_global("D_80122D04") == 0) and (Math.u32(engine.gs_get("pad_held")) & 0xF000F000) != 0:
		engine.memory.write_u8(G_ACTOR_POOL + 0x3, engine.memory.u8(G_ACTOR_POOL + 0x3) | 0x80)
	if engine.memory.read_global("D_80122D04") != 0 or engine.memory.read_global("D_80122D2C") != 0:
		engine.memory.write_u8(G_ACTOR_POOL + 0x3, engine.memory.u8(G_ACTOR_POOL + 0x3) & 0x7F)
	if engine.memory.read_global("talk_menu_active") == 1:
		engine.memory.write_u8(G_ACTOR_POOL + 0x3, engine.memory.u8(G_ACTOR_POOL + 0x3) | 0x80)
	if (engine.memory.u8(G_ACTOR_POOL + 0x3) & 0x80) != 0:
		engine.call_c("hud_cursor_update")

## Hides the date display and advances the scene step.
func hizuke_hide() -> void:
	engine.call_c("hizuke_disp_switch", [0])
	engine.call_c("mode_step_next")

## Shows the date display and advances the scene step.
func hizuke_appear() -> void:
	engine.call_c("hizuke_disp_switch", [1])
	engine.call_c("mode_step_next")

## Starts loading the current character portrait, using seasonal files for heroines.
func read_bustup() -> void:
	match engine.gs_get("current_girl"):
		0:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x55B2])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x55DF])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x560C])
			else:
				engine.call_c("portrait_read_start", [0x5639])
		1:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x5D9B])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x5DC8])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x5DF5])
			else:
				engine.call_c("portrait_read_start", [0x5E22])
		2:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x5288])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x52B5])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x52E2])
			else:
				engine.call_c("portrait_read_start", [0x530F])
		3:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x5909])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x5936])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x5963])
			else:
				engine.call_c("portrait_read_start", [0x5990])
		4:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x6719])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x6746])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x6773])
			else:
				engine.call_c("portrait_read_start", [0x67A0])
		5:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x6011])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x603E])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x606B])
			else:
				engine.call_c("portrait_read_start", [0x6098])
		6:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x6287])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x62B4])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x62E1])
			else:
				engine.call_c("portrait_read_start", [0x630E])
		7:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x5B52])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x5B7F])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x5BAC])
			else:
				engine.call_c("portrait_read_start", [0x5BD9])
		8:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x503F])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x506C])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x5099])
			else:
				engine.call_c("portrait_read_start", [0x50C6])
		9:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x64FD])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x652A])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x6557])
			else:
				engine.call_c("portrait_read_start", [0x6584])
		0xA:
			if engine.gs_get("month") >= 3 and engine.gs_get("month") < 6:
				engine.call_c("portrait_read_start", [0x698F])
			elif engine.gs_get("month") >= 6 and engine.gs_get("month") < 9:
				engine.call_c("portrait_read_start", [0x69BC])
			elif engine.gs_get("month") >= 9 and engine.gs_get("month") < 0xC:
				engine.call_c("portrait_read_start", [0x69E9])
			else:
				engine.call_c("portrait_read_start", [0x6A16])
		0xD:
			engine.call_c("portrait_read_start", [0x6C8C])
	engine.call_c("mode_step_next")

## Starts loading the current character school-uniform portrait, using seasonal variants where available.
func read_bustup_uniform() -> void:
	match engine.gs_get("current_girl"):
		0:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x541D])
			else:
				engine.call_c("portrait_read_start", [0x544A])
		1:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x5D14])
			else:
				engine.call_c("portrait_read_start", [0x5D41])
		2:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x5201])
			else:
				engine.call_c("portrait_read_start", [0x522E])
		3:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x5882])
			else:
				engine.call_c("portrait_read_start", [0x58AF])
		4:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x6692])
			else:
				engine.call_c("portrait_read_start", [0x66BF])
		5:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x5F8A])
			else:
				engine.call_c("portrait_read_start", [0x5FB7])
		6:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x6200])
			else:
				engine.call_c("portrait_read_start", [0x622D])
		7:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x5AF8])
			else:
				engine.call_c("portrait_read_start", [0x5B25])
		8:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x4FE5])
			else:
				engine.call_c("portrait_read_start", [0x5012])
		9:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x64A3])
			else:
				engine.call_c("portrait_read_start", [0x64D0])
		0xA:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x6908])
			else:
				engine.call_c("portrait_read_start", [0x6935])
		0xC:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x57FB])
			else:
				engine.call_c("portrait_read_start", [0x5828])
		0xB:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x6B51])
			else:
				engine.call_c("portrait_read_start", [0x6B7E])
		0xD:
			if engine.gs_get("month") >= 6 and engine.gs_get("month") < 0xA:
				engine.call_c("portrait_read_start", [0x6C32])
			else:
				engine.call_c("portrait_read_start", [0x6C5F])
		0xE:
			engine.call_c("portrait_read_start", [0x6B24])
	engine.call_c("mode_step_next")

## Starts loading the current heroine swimsuit portrait.
func read_bustup_swimsuit() -> void:
	match engine.gs_get("current_girl"):
		0:
			engine.call_c("portrait_read_start", [0x5666])
		1:
			engine.call_c("portrait_read_start", [0x5E4F])
		2:
			engine.call_c("portrait_read_start", [0x533C])
		3:
			engine.call_c("portrait_read_start", [0x59BD])
		4:
			engine.call_c("portrait_read_start", [0x67CD])
		5:
			engine.call_c("portrait_read_start", [0x60C5])
		6:
			engine.call_c("portrait_read_start", [0x633B])
		7:
			engine.call_c("portrait_read_start", [0x5C06])
		8:
			if engine.gs_get("year") == 0x5F:
				engine.call_c("portrait_read_start", [0x50F3])
			elif engine.gs_get("year") == 0x60:
				engine.call_c("portrait_read_start", [0x5120])
			else:
				engine.call_c("portrait_read_start", [0x514D])
		9:
			engine.call_c("portrait_read_start", [0x65B1])
		0xA:
			engine.call_c("portrait_read_start", [0x6A43])
	engine.call_c("mode_step_next")

## Starts loading the current character portrait.
func bustup_return() -> void:
	engine.call_c("read_bustup")

## Restore the main mode, stage and step saved in saved_mode, saved_stage and saved_step by calling
## game_mode_set, mode_stage_set and mode_step_set
func return_step() -> void:
	engine.call_c("game_mode_set", [engine.gs_get("saved_game_mode")])
	engine.call_c("mode_stage_set", [engine.gs_get("saved_mode_stage")])
	engine.call_c("mode_step_set", [engine.gs_get("saved_mode_step")])

## Advances dialogue text and temporarily speeds it while a fast-text input is held.
func k_disp_inc2() -> void:
	var pad: int = 0
	var sp2B: int = 0
	var _t1: int = engine.memory.read_global("D_80122CFC")
	engine.memory.write_global("D_80122CFC", Math.s32(engine.memory.read_global("D_80122CFC") - 1))
	if _t1 < 1:
		engine.memory.write_global("D_80122CFC", 0)
		sp2B = Math.u8(engine.call_c("get_k_speed"))
		if (engine.gs_get("pad_held") & 0x600060) != 0:
			engine.call_c("k_speed_set", [0])
		engine.call_c("k_disp_inc")
		engine.call_c("k_speed_set", [sp2B])

## Restarts sequence zero in sound bank zero and advances the scene step.
func yosi_trans() -> void:
	engine.call_c("audio_bank_start", [0, 0x200])
	engine.call_c("mode_step_next")

## Selects Yoshio as the current girl slot for the scene.
func change_yoshio() -> void:
	engine.gs_set("current_girl", 0xD)
	engine.call_c("mode_step_next")

## Selects the date partner as the current girl.
func change_girl() -> void:
	engine.gs_set("current_girl", engine.gs_get("date_girl"))
	engine.call_c("mode_step_next")

func talk_names_set(girl_id: int) -> void:
	girl_id = Math.u8(girl_id)
	var t: int = 0
	engine.call_c("strcpy", [G_NAME_CODE_FAMILY, GS + 0xD4])
	engine.call_c("strcpy", [G_NAME_CODE_GIVEN, GS + 0xDC])
	if girl_id < 0xE:
		t = girl_id
		engine.call_c("get_p_name", [G_NAME_CODE_PLAYER_FORM, t])
		engine.call_c("strcpy", [G_NAME_CODE_GIRL_SHORT, engine.memory.u32(G_GIRL_SHORT_NAME_TABLE + girl_id * 4)])
		engine.call_c("get_g_name", [G_GIRL_NAME_BUF, t])

## Runs the heroine-selection menu with the back entry available.
func select_girl() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("select_girl_init", [0])
			return
		1:
			engine.call_c("select_girl_main", [0])
			return
		_:
			engine.call_c("game_mode_reset_to_title")
			return

## Runs the heroine-selection menu without the back entry.
func select_girl2() -> void:
	match engine.memory.u32(GS + 0x1104):
		0:
			engine.call_c("select_girl_init", [1])
			return
		1:
			engine.call_c("select_girl_main", [1])
			return
		_:
			engine.call_c("game_mode_reset_to_title")
			return

## Builds and initializes the available-heroine selection menu.
func select_girl_init(omit_back: int) -> void:
	omit_back = Math.u8(omit_back)
	var i: int = 0
	var length: int = 0
	var result: int = 0
	i = 0
	while i < 0xB:
		if engine.halted():
			break
		engine.memory.write_u16(G_SELECT_GIRL_MENU_X + i * 2, (_div(i, 3) << 6) - 0x80)
		engine.memory.write_u16(G_SELECT_GIRL_MENU_Y + i * 2, _mod(i, 3) * 0x10 + 0x30)
		if engine.memory.u8(G_SELECT_GIRL_MENU_ENABLED + i) != 0:
			engine.call_c("strcpy", [engine.frame_base() + 0x64, engine.memory.u32(G_GIRL_SHORT_NAME_TABLE + i * 4)])
			result = Math.s32(engine.call_c("set_kanji_string", [engine.memory.s16(G_SELECT_GIRL_MENU_X + i * 2), engine.memory.s16(G_SELECT_GIRL_MENU_Y + i * 2), 0, engine.frame_base() + 0x64, 0]))
			length = Math.s32(engine.call_c("strlen", [engine.frame_base() + 0x64]))
			engine.memory.write_u16(G_SELECT_GIRL_MENU_WIDTH + i * 2, Math.s32(_div(length, 2) * 0xE))
		else:
			engine.memory.write_u16(G_SELECT_GIRL_MENU_WIDTH + i * 2, 0)
		engine.memory.write_u16(G_SELECT_GIRL_MENU_HEIGHT + i * 2, 0x10)
		i = Math.s16(i + 1)
	engine.memory.write_u16(G_SELECT_GIRL_MENU_X + 0x16, 0x40)
	engine.memory.write_u16(G_SELECT_GIRL_MENU_Y + 0x16, 0x50)
	engine.memory.write_u16(G_SELECT_GIRL_MENU_WIDTH + 0x16, 0x2A)
	engine.memory.write_u16(G_SELECT_GIRL_MENU_HEIGHT + 0x16, 0x10)
	if omit_back != 0:
		engine.memory.write_u16(G_SELECT_GIRL_MENU_WIDTH + 0x16, 0)
	if omit_back == 0:
		result = Math.s32(engine.call_c("set_kanji_string", [0x40, 0x50, 0, 0x800B226C, 0]))
	engine.call_c("k_disp_start", [result])
	engine.call_c("menu_set", [1, Math.u32(0xC - omit_back) & 0xFF, G_SELECT_GIRL_MENU_X, G_SELECT_GIRL_MENU_Y, G_SELECT_GIRL_MENU_WIDTH, G_SELECT_GIRL_MENU_HEIGHT])
	i = 0
	while i < 0xB:
		if engine.memory.s16(G_SELECT_GIRL_MENU_WIDTH + i * 2) != 0:
			engine.gs_set("menu_cursor_1", i)
			break
		i = Math.s16(i + 1)
	engine.memory.write_global("talk_menu_active", 1)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))

## Handles selection of an available heroine entry.
func select_girl_main(omit_back: int) -> int:
	omit_back = Math.s32(omit_back)
	var girl_index: int = 0
	var candidate_count: int = 0
	engine.call_c("menu_check", [1, engine.memory.read_global("cursor_x"), engine.memory.read_global("cursor_y")])
	engine.call_c("menu_cursor_update", [1])
	engine.call_c("menu_bar_show", [1])
	if (engine.gs_get("pad_pressed") & 0x200020) != 0:
		candidate_count = Math.u32(0xC - Math.u8(omit_back))
		girl_index = 0
		while Math.u32(girl_index) < candidate_count:
			if engine.halted():
				break
			if engine.memory.u8(GS + girl_index * 0x10 + 0xF9E) != 0 and engine.memory.s16(G_SELECT_GIRL_MENU_WIDTH + girl_index * 2) != 0:
				engine.memory.write_global("talk_choice", girl_index)
				engine.memory.write_global("D_8011ECD3", engine.memory.read_global("D_8011ECD3") & 0xFF7F)
				engine.memory.write_global("talk_menu_active", 0)
				engine.call_c("k_reset", [1])
				engine.call_c("mode_step_next")
				return 0
			girl_index = Math.s16(girl_index + 1)
	return 0 # the C falls off the end here (v0 undefined)

## Sets all three tint channels for one sprite quad.
func sprite_brightness(sprite_index: int, brightness: int) -> void:
	sprite_index = Math.s16(sprite_index)
	brightness = Math.u8(brightness)
	var p: int = 0
	p = Math.u32(G_SPRITE_QUADS + sprite_index * 0x24)
	engine.memory.write_u8(p + 0x16, brightness)
	engine.memory.write_u8(p + 0x15, brightness)
	engine.memory.write_u8(p + 0x14, brightness)

## Advances the calendar by one day and refreshes the date display.
func day_plus() -> void:
	var sp28: int = 0
	var sp2B: int = 0
	engine.memory.write_global("scene_trip_day_state", 0)
	sp2B = engine.memory.read_global("D_8011F113")
	engine.call_c("calendar_advance_day")
	engine.call_c("hizuke_init")
	if (sp2B & 0x80) != 0:
		engine.call_c("hizuke_disp_switch", [1])
	else:
		engine.call_c("hizuke_disp_switch", [0])
	engine.call_c("hizuke_show")
	engine.call_c("mode_step_next")

## Clamp girl tokimeki/affection (0..0x80), syoushin (0..0xFF) and the nine parameters (0..999); the queue reader
## then clamps all three girl counters to 0x80 (girl_counters_clamp)
func check_para_limit() -> void:
	var i: int = 0
	i = 0
	while i < 0xB:
		if engine.memory.s16(GS + 0x1BC + i * 56 + 0x2) < 0:
			engine.memory.write_u32(GS + 0x1BC + i * 56, 0)
		if engine.memory.s16(GS + 0x1BC + i * 56 + 0x2) >= 0x80:
			engine.memory.write_u16(GS + 0x1BC + i * 56 + 0x2, 0x80)
		if engine.memory.s16(GS + 0x1BC + i * 56 + 0x6) < 0:
			engine.memory.write_u32(GS + 0x1BC + i * 56 + 0x4, 0)
		if engine.memory.s16(GS + 0x1BC + i * 56 + 0x6) >= 0x80:
			engine.memory.write_u16(GS + 0x1BC + i * 56 + 0x6, 0x80)
		if engine.memory.s16(GS + 0x1BC + i * 56 + 0xA) < 0:
			engine.memory.write_u32(GS + 0x1BC + i * 56 + 0x8, 0)
		if engine.memory.s16(GS + 0x1BC + i * 56 + 0xA) >= 0xFF:
			engine.memory.write_u16(GS + 0x1BC + i * 56 + 0xA, 0xFF)
		i = Math.s16(i + 1)
	i = 0
	while i < 9:
		if engine.memory.s16(GS + 0xFC + i * 4 + 0x2) < 0:
			engine.memory.write_u32(GS + 0xFC + i * 4, 0)
		if engine.memory.s16(GS + 0xFC + i * 4 + 0x2) >= 0x3E7:
			engine.memory.write_u32(GS + 0xFC + i * 4, 0x3E70000)
		i = Math.s16(i + 1)

func text_marker_hide() -> void:
	engine.call_c("hud_text_marker_set", [0])

func scene_mouth_lock_clear() -> void:
	engine.memory.write_global("D_80122D20", 0)
	engine.call_c("mode_step_next")

## s32 without a return value: the last chain value stays in v0 (andi v0,a0,0xffff). T-11590
func scene_still_show(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int) -> int:
	arg0 = Math.s32(arg0)
	arg1 = Math.s32(arg1)
	arg2 = Math.s32(arg2)
	arg3 = Math.s32(arg3)
	arg4 = Math.s32(arg4)
	arg5 = Math.s32(arg5)
	var i: int = 0
	engine.memory.write_global("D_80122D04", 0)
	engine.memory.write_global("D_80122D0C", 0)
	engine.memory.write_global("D_80122D10", 0)
	engine.call_c("LoadSquare", [0x140, 0x80, 0x40, 0x80, Math.u32(arg0)])
	engine.call_c("LoadSquare", [0x180, 0x80, 0x40, 0x80, Math.u32(arg1)])
	engine.call_c("LoadSquare", [0x1C0, 0x80, 0x40, 0x80, Math.u32(arg2)])
	engine.call_c("LoadSquare", [0x200, 0x80, 0x40, 0x80, Math.u32(arg3)])
	engine.call_c("LoadSquare", [0x240, 0x80, 0x40, 0x80, Math.u32(arg4)])
	engine.call_c("LoadSquare", [0x280, 0x80, 0x40, 0x80, Math.u32(arg5)])
	engine.memory.write_u32(GS + 0x1228 + 0x94, 1)
	engine.memory.write_u32(GS + 0x1228 + 0x98, 1)
	engine.memory.write_u32(GS + 0x1228 + 0x9C, 1)
	engine.memory.write_u32(GS + 0x1228 + 0xA0, 1)
	engine.memory.write_u32(GS + 0x1228 + 0xA4, 1)
	engine.memory.write_u32(GS + 0x1228 + 0xA8, 1)
	i = 0x3A
	while i < 0x40:
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0xC, i - 0x35)
		engine.memory.write_u32(G_SPRITE_QUADS + i * 36, 0xC1000000)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x8, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0xA, 0x80)
		engine.memory.write_u8(G_SPRITE_QUADS + i * 36 + 0xE, 0)
		engine.memory.write_u8(G_SPRITE_QUADS + i * 36 + 0xF, 0x80)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x10, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x12, 0x1F1)
		engine.memory.write_u8(G_SPRITE_QUADS + i * 36 + 0x14, 0)
		engine.memory.write_u8(G_SPRITE_QUADS + i * 36 + 0x15, 0)
		engine.memory.write_u8(G_SPRITE_QUADS + i * 36 + 0x16, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x18, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x1A, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x1C, 0x1000)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x1E, 0x1000)
		engine.memory.write_u32(G_SPRITE_QUADS + i * 36 + 0x20, 0)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x4, _mod(i - 0x3A, 3) * 0x80 - 0xA0)
		engine.memory.write_u16(G_SPRITE_QUADS + i * 36 + 0x6, _div(i - 0x3A, 3) * 0x80 - 0x78)
		engine.memory.write_u8(GS + 0x10A5 + i, 0xB)
		i = Math.s16(i + 1)
	engine.memory.write_u16(G_SPRITE_QUADS + 0x8E4, 0x40)
	engine.memory.write_u16(G_SPRITE_QUADS + 0x878, engine.memory.s16(G_SPRITE_QUADS + 0x8E4))
	engine.memory.write_u16(G_SPRITE_QUADS + 0x8E6, 0x20)
	engine.memory.write_u16(G_SPRITE_QUADS + 0x8C2, engine.memory.u16(G_SPRITE_QUADS + 0x8E6))
	engine.memory.write_u16(G_SPRITE_QUADS + 0x89E, engine.memory.u16(G_SPRITE_QUADS + 0x8C2))
	return 0 # the C falls off the end here (v0 undefined)

func scene_anim_show(arg0: int, arg1: int, arg2: int, arg3: int) -> int:
	arg0 = Math.s32(arg0)
	arg1 = Math.s32(arg1)
	arg2 = Math.s32(arg2)
	arg3 = Math.s16(arg3)
	var i: int = 0
	var p: int = 0
	if arg0 == 0:
		engine.call_c("actor_clear", [0x60])
		engine.call_c("actor_clear", [0x61])
		return 0
	i = 0
	while i < 2:
		if engine.halted():
			break
		engine.call_c("actor_clear", [i + 0x60])
		p = G_ACTOR_POOL + (i + 0x60) * 68
		engine.memory.write_u8(p + 0x1, 0xA)
		engine.memory.write_u8(p + 0x2, 0)
		engine.memory.write_u32(p + 0x38, 0x41000000)
		engine.memory.write_u8(p + 0x3, 4)
		engine.memory.write_u32(p + 0xC, Math.u32(arg0))
		engine.memory.write_u32(p + 0x10, Math.u32(arg1))
		engine.memory.write_u32(p + 0x34, Math.u32(arg2))
		engine.memory.write_u16(p + 0x16, 0)
		engine.memory.write_u16(p + 0x18, 0)
		engine.memory.write_u8(p + 0x6, 2)
		engine.memory.write_u8(p + 0x43, 0x11)
		engine.memory.write_u16(p + 0x26, -0xA0)
		engine.memory.write_u16(p + 0x2A, -0x78)
		engine.memory.write_u8(p + 0x7, 0)
		engine.memory.write_u8(p + 0x5, 0)
		engine.memory.write_u8(p + 0x4, 0)
		engine.memory.write_u16(p + 0x14, arg3)
		i = Math.s16(i + 1)
	engine.memory.write_u8(G_ACTOR_POOL + 0x1982, 4)
	if arg3 >= 2:
		engine.memory.write_global("scene_mouth_frames_reversed_saved", 0)
		engine.memory.write_u16(G_ACTOR_POOL + 0x1996, 2)
	else:
		engine.memory.write_u8(G_ACTOR_POOL + 0x1983, 0)
	return 0 # the C falls off the end here (v0 undefined)

func scene_still_layer_set(arg0: int) -> void:
	arg0 = Math.u8(arg0)
	var i: int = 0
	var t: int = 0
	if arg0 != 0:
		if engine.memory.u8(G_ACTOR_POOL + 0x1983) != 0:
			t = engine.memory.read_global("D_800B5A60")
			engine.memory.write_global("D_800B5A60", engine.memory.read_global("scene_mouth_frames_reversed_saved"))
			engine.memory.write_global("scene_mouth_frames_reversed_saved", t)
			engine.memory.write_u8(G_ACTOR_POOL + 0x1983, engine.memory.u8(G_ACTOR_POOL + 0x1983) | 0x80)
		if engine.memory.u8(G_ACTOR_POOL + 0x19C7) != 0:
			engine.memory.write_u8(G_ACTOR_POOL + 0x19C7, engine.memory.u8(G_ACTOR_POOL + 0x19C7) | 0x80)
		i = 0x3A
		while i < 0x40:
			engine.memory.write_u32(G_SPRITE_QUADS + i * 36, engine.memory.s32(G_SPRITE_QUADS + i * 36) & 0x7FFFFFFF)
			i = Math.s16(i + 1)
	else:
		engine.memory.write_global("D_800B5A60", engine.memory.read_global("scene_mouth_frames_reversed_saved"))
		if engine.memory.u8(G_ACTOR_POOL + 0x1983) != 0:
			engine.memory.write_u8(G_ACTOR_POOL + 0x1983, engine.memory.u8(G_ACTOR_POOL + 0x1983) & 0xFFFFFF7F)
		if engine.memory.u8(G_ACTOR_POOL + 0x19C7) != 0:
			engine.memory.write_u8(G_ACTOR_POOL + 0x19C7, engine.memory.u8(G_ACTOR_POOL + 0x19C7) & 0xFFFFFF7F)
		i = 0x3A
		while i < 0x40:
			engine.memory.write_u32(G_SPRITE_QUADS + i * 36, Math.u32(engine.memory.s32(G_SPRITE_QUADS + i * 36)) | 0x80000000)
			i = Math.s16(i + 1)

func scene_still_fade_in() -> void:
	var var_s0: int = 0
	engine.call_c("normal_date_bg_fadein")
	if engine.memory.read_global("bg_brightness") == 4:
		engine.call_c("scene_still_layer_set", [1])
	engine.memory.write_global("D_80122D04", 1)
	engine.call_c("message_window_title_show", [0])
	engine.memory.write_global("D_80120657", engine.memory.read_global("bg_brightness"))
	var_s0 = 0x3A
	engine.memory.write_global("D_8012069B", engine.memory.read_global("bg_brightness"))
	while true:
		if engine.halted():
			break
		engine.call_c("sprite_brightness", [var_s0, engine.memory.read_global("bg_brightness")])
		var_s0 = Math.s16(var_s0 + 1)
		if not (var_s0 < 0x40):
			break

func scene_still_fade_out() -> void:
	var var_s0: int = 0
	var temp_s0: int = 0
	var state: int = 0
	temp_s0 = engine.gs_get("mode_step")
	engine.call_c("normal_date_bg_fadeout")
	state = engine.gs_get("mode_step")
	if state != temp_s0:
		engine.call_c("scene_still_layer_set", [0])
		engine.memory.write_global("D_80122D04", 0)
		engine.call_c("message_window_title_show", [1])
	engine.memory.write_global("D_80120657", engine.memory.read_global("bg_brightness"))
	var_s0 = 0x3A
	engine.memory.write_global("D_8012069B", engine.memory.read_global("bg_brightness"))
	while true:
		if engine.halted():
			break
		engine.call_c("sprite_brightness", [var_s0, engine.memory.read_global("bg_brightness")])
		var_s0 = Math.s16(var_s0 + 1)
		if not (var_s0 < 0x40):
			break

## Selects face animation values 2 and 0, then advances the event step.
func event_face0() -> void:
	engine.memory.write_global("D_80120666", 2)
	engine.memory.write_global("D_801206AA", 0)
	engine.call_c("mode_step_next")

## Selects face animation values 3 and 1, then advances the event step.
func event_face1() -> void:
	engine.memory.write_global("D_80120666", 3)
	engine.memory.write_global("D_801206AA", 1)
	engine.call_c("mode_step_next")

## Selects face animation values 6 and 4, then advances the event step.
func event_face2() -> void:
	engine.memory.write_global("D_80120666", 6)
	engine.memory.write_global("D_801206AA", 4)
	engine.call_c("mode_step_next")

## Selects face animation values 7 and 5, then advances the event step.
func event_face3() -> void:
	engine.memory.write_global("D_80120666", 7)
	engine.memory.write_global("D_801206AA", 5)
	engine.call_c("mode_step_next")

## Selects face animation value 2 and advances the event step.
func event_face_kuchi0() -> void:
	engine.memory.write_global("D_80120666", 2)
	engine.call_c("mode_step_next")

## Selects face animation value 3 and advances the event step.
func event_face_kuchi1() -> void:
	engine.memory.write_global("D_80120666", 3)
	engine.call_c("mode_step_next")

## Selects voice-driven event mouth animation and advances the event step.
func event_kuchi_ani_no() -> void:
	engine.memory.write_global("D_80122D10", 0)
	engine.memory.write_global("D_80120652", 4)
	engine.call_c("mode_step_next")

## Selects periodic event mouth animation and advances the event step.
func event_kuchi_ani_yes() -> void:
	engine.memory.write_global("D_80122D10", 1)
	engine.memory.write_global("D_80120652", 0)
	engine.call_c("mode_step_next")

## Enables actor 97 eye animation and permits periodic blink restarts.
func ev_me_on() -> void:
	engine.memory.write_global("D_80122D0C", 0)
	engine.memory.write_global("D_80120696", 5)
	engine.call_c("mode_step_next")

## Enables actor 97 eye animation and suppresses periodic blink restarts.
func ev_me_on_continue() -> void:
	engine.memory.write_global("D_80122D0C", 1)
	engine.memory.write_global("D_80120696", 5)
	engine.call_c("mode_step_next")

## Draws a four-vertex shaded rectangle from two supplied colors.
func _sprite_set_box_shade_tarao(arg0: int, arg1: int, arg2: int, arg3: int, arg4: int, arg5: int, arg6: int) -> void:
	arg0 = Math.s32(arg0)
	arg1 = Math.s32(arg1)
	arg2 = Math.s32(arg2)
	arg3 = Math.s32(arg3)
	arg4 = Math.s32(arg4)
	arg5 = Math.u32(arg5)
	arg6 = Math.u32(arg6)
	var gs: int = 0
	var p: int = 0
	var v4: int = 0
	p = Math.u32(engine.call_c("GetWorkBase", [0x24, engine.memory.read_global("draw_buffer")]))
	engine.call_c("func_8009F054", [p])
	engine.call_c("SetSemiTrans", [p, 0])
	gs = GS
	v4 = (arg5 >> 16) & 0xFF
	engine.memory.write_u8(p + 0x4, (arg6 >> 16) & 0xFF)
	engine.memory.write_u8(p + 0x5, (arg6 >> 8) & 0xFF)
	engine.memory.write_u8(p + 0x6, arg6 & 0xFF)
	engine.memory.write_u8(p + 0xC, (arg6 >> 16) & 0xFF)
	engine.memory.write_u8(p + 0xD, (arg6 >> 8) & 0xFF)
	engine.memory.write_u8(p + 0xE, arg6 & 0xFF)
	engine.memory.write_u8(p + 0x14, v4)
	engine.memory.write_u8(p + 0x15, (arg5 >> 8) & 0xFF)
	engine.memory.write_u8(p + 0x16, arg5 & 0xFF)
	engine.memory.write_u8(p + 0x1C, (arg5 >> 16) & 0xFF)
	engine.memory.write_u8(p + 0x1D, (arg5 >> 8) & 0xFF)
	engine.memory.write_u8(p + 0x1E, arg5 & 0xFF)
	engine.memory.write_u16(p + 0x8, Math.s32(engine.memory.s16(gs + 0x14 + engine.memory.read_global("draw_buffer") * 2) + arg0))
	engine.memory.write_u16(p + 0xA, Math.s32(engine.memory.s16(gs + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg1))
	engine.memory.write_u16(p + 0x10, Math.s32(Math.s32(engine.memory.s16(gs + 0x14 + engine.memory.read_global("draw_buffer") * 2) + arg0) + arg2))
	engine.memory.write_u16(p + 0x12, Math.s32(engine.memory.s16(gs + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg1))
	engine.memory.write_u16(p + 0x18, Math.s32(engine.memory.s16(gs + 0x14 + engine.memory.read_global("draw_buffer") * 2) + arg0))
	engine.memory.write_u16(p + 0x1A, Math.s32(Math.s32(engine.memory.s16(gs + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg1) + arg3))
	engine.memory.write_u16(p + 0x20, Math.s32(Math.s32(engine.memory.s16(gs + 0x14 + engine.memory.read_global("draw_buffer") * 2) + arg0) + arg2))
	engine.memory.write_u16(p + 0x22, Math.s32(Math.s32(engine.memory.s16(gs + 0x18 + engine.memory.read_global("draw_buffer") * 2) + arg1) + arg3))
	engine.call_c("AddPrim", [Math.u32(Math.u32(G_DRAW_OT_TAGS + (engine.memory.read_global("draw_buffer") << 10)) + arg4 * 4), Math.s32(p)])

func talk_clear() -> void:
	var i: int = 0
	i = 5
	while i < 0x1F:
		engine.memory.write_u32(G_TALK_WORK + i * 4, 0)
		i = Math.s16(i + 1)
	engine.memory.write_global("talk_voice_wait_frames", 0)
	engine.call_c("hud_cursor_actor_init", [0])
	engine.memory.write_global("D_8011ECD3", engine.memory.read_global("D_8011ECD3") & 0xFF7F)
	engine.call_c("k_reset", [0])
	engine.call_c("message_window_set_title", [-0x7FF4DD8C])
	engine.call_c("message_window_title_show", [0])
	engine.call_c("hud_mascot_display_switch", [0])

## Starts the screen-shake effect and advances the scene step.
func don_init2() -> void:
	engine.call_c("draw_shake_start")
	engine.call_c("mode_step_next")

## Waits through the shake interval, then resets text and advances.
func don_wait() -> void:
	var _t1: int = engine.memory.s32(GS + 0x1104)
	engine.memory.write_u32(GS + 0x1104, Math.s32(engine.memory.s32(GS + 0x1104) + 1))
	if Math.u32(_t1) >= 0x81:
		engine.call_c("k_reset", [1])
		engine.call_c("mode_step_next")

## Installs dialogue tables and runs one date-dialogue line.
func normal_date_speak_1line() -> void:
	engine.memory.write_global("talk_tables", G_TALK_GROUP)
	engine.memory.write_global("talk_active_line_ptr", G_TALK_LINE)
	engine.memory.write_global("talk_active_text_table", engine.memory.read_global("talk_text_table"))
	engine.memory.write_global("talk_active_command_table", engine.memory.read_global("talk_command_table"))
	engine.memory.write_global("talk_active_voice_table", engine.memory.read_global("talk_voice_table"))
	if (Math.s32(engine.call_c("talk_run", [engine.memory.read_global("talk_choice"), 1, 0]))) == 1:
		engine.call_c("mode_step_next")

## Installs the dialogue tables and runs the multi-line date dialogue.
func normal_date_speak_012() -> void:
	engine.memory.write_global("talk_tables", G_TALK_GROUP)
	engine.memory.write_global("talk_active_line_ptr", G_TALK_LINE)
	engine.memory.write_global("talk_active_text_table", engine.memory.read_global("talk_text_table"))
	engine.memory.write_global("talk_active_command_table", engine.memory.read_global("talk_command_table"))
	engine.memory.write_global("talk_active_voice_table", engine.memory.read_global("talk_voice_table"))
	engine.call_c("talk_run", [0xFF, 1, 0])

## Wire the dialogue table pointers and run the talk runner for the current scene girl
func normal_date_speak() -> void:
	engine.memory.write_global("talk_tables", G_TALK_GROUP)
	engine.memory.write_global("talk_active_line_ptr", G_TALK_LINE)
	engine.memory.write_global("talk_active_text_table", engine.memory.read_global("talk_text_table"))
	engine.memory.write_global("talk_active_command_table", engine.memory.read_global("talk_command_table"))
	engine.memory.write_global("talk_active_voice_table", engine.memory.read_global("talk_voice_table"))
	engine.call_c("talk_run", [engine.memory.read_global("talk_choice"), 1, 0])

## Resets portrait actors, frees their texture-cache slots and advances the scene step.
func vram_bustup_clear() -> void:
	var i: int = 0
	engine.call_c("actor_clear", [0x1F])
	engine.call_c("actor_clear", [0x1E])
	i = 5
	while i < 0xA:
		engine.memory.write_u32(GS + 0x1228 + i * 4, -0x1)
		i = Math.s16(i + 1)
	engine.call_c("mode_step_next")

func event_log_add(scene_id: int, scene_flag: int) -> int:
	scene_id = Math.u8(scene_id)
	scene_flag = Math.u8(scene_flag)
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8, engine.gs_get("current_girl"))
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x2, scene_id)
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x1, scene_flag)
	if engine.memory.s32(G_TALK_RANK_OVERRIDE) == 0 or engine.memory.s32(G_TALK_RANK_OVERRIDE) >= 7:
		engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x3, Math.s32(engine.call_c("get_g_zyotai_h", [engine.gs_get("current_girl")])))
	else:
		engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x3, Math.s32(engine.memory.s32(G_TALK_RANK_OVERRIDE) - 1))
	engine.memory.write_u32(G_TALK_RANK_OVERRIDE, 0)
	if engine.gs_get("current_girl") == 0xC:
		engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x3, 0)
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x4, engine.gs_get("year"))
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x5, engine.gs_get("month"))
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x6, engine.gs_get("day"))
	engine.memory.write_u8(GS + 0x75E + engine.gs_get("event_log_count") * 8 + 0x7, engine.gs_get("weekday"))
	engine.gs_set("event_log_count", engine.gs_get("event_log_count") + 1)
	if engine.gs_get("event_log_count") >= 0xFF:
		engine.gs_set("event_log_count", 0xFE)
	return 0 # the C falls off the end here (v0 undefined)

func portrait_quad_offsets(expression_index: int) -> void:
	expression_index = Math.u8(expression_index)
	match expression_index:
		0:
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 7)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x30)
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 7)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0)
		1:
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 7)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x70)
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 7)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0x40)
		2:
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 8)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x30)
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 8)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0)
		3:
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 8)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x70)
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 8)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0x40)
		4:
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 9)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x30)
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 9)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0)
		5:
			engine.memory.write_u16(G_SPRITE_QUADS + 0xC, 9)
			engine.memory.write_u8(G_SPRITE_QUADS + 0xE, 0x70)
			engine.memory.write_u16(G_SPRITE_QUADS + 0x30, 9)
			engine.memory.write_u8(G_SPRITE_QUADS + 0x32, 0x40)
