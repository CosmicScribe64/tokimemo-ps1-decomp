#ifndef OVL_OPTIONS_H
#define OVL_OPTIONS_H

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/layer.h"
#include "game/schedule_records.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/boot.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/debug.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/memory_card.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/prim.h"
#include "main/schedule.h"
#include "main/settings.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/layer.h"
#include "game/schedule_records.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/boot.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/debug.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/memory_card.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/prim.h"
#include "main/schedule.h"
#include "main/settings.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"

/* OPTION overlay data (bss, not in the overlay file). */
extern u8 options_voice_test_bank_start_argument;
extern u8 options_voice_bank_load_step;
extern u8 options_music_bank_load_step;
extern s32 **D_8013D360;
extern s32 options_debug_audio_bank;
extern u8 D_8013D3E0[];
extern u8 D_8013D3CC[];
extern u8 D_8013D3BC[];
extern volatile u16 options_debug_audio_track; /* volatile: both stores of options_debug_audio_play_request stay (T-11340) */
extern u8 options_voice_test_character_clip_index;
extern u8 options_voice_test_music_clip_index;
extern s8 D_8013D39C;
extern u8 options_voice_test_repeat_enabled;

void options_exit_fade_to_title(void);
void options_main_menu_init(void);
void options_menu_background_load(void);

void options_debug_audio_bank_load_music(void);
void options_debug_audio_bank_load_voice(void);
void options_mode_stage_gradient_mask_update(void);
void options_voice_test_background_gradient_draw(void);
void options_icon_mode_menu_build(void);
s32 options_main_menu_select(void);
void options_icon_mode_select(void);
void options_icon_mode_page_close(void);
void options_double_click_interval_select(void);
void options_double_click_indicator_update(void);
void options_menu_mode_page_initialize(void);
void options_menu_mode_select(void);
void options_menu_mode_page_exit_zoom(void);
void options_voice_mode_page_initialize(void);
void options_voice_mode_selection_update(void);
void options_voice_mode_page_exit_zoom(void);
void options_extras_page_initialize(void);
s32 sound_test_unlocked_track_count(void);
s32 options_extras_submenu_a_initialize(void);
s32 options_extras_submenu_a_selection_update(void);
void options_voice_test_fade_in(void);

void options_debug_audio_input_handle();
void options_debug_audio_play_request(s32 track_request);
void options_debug_audio_bank_command(void);
void options_debug_audio_bank_select(void);
void options_debug_audio_bank_start_primary(void);
void options_debug_audio_bank_start_secondary(void);
void options_debug_audio_bank_load_dispatch(void);
void options_voice_test_debug_status_draw(void);
s32 options_voice_test_actor_selection_update(void);
void options_debug_audio_name_display_toggle();

extern u8 D_8013D3F4;
extern s8 options_double_click_countdown;
void options_debug_audio_background_load(void);
void options_debug_audio_step_dispatch();
void options_debug_player_data_editor(void);
s32 options_debug_player_data_mode_step_dispatch(void);
void options_debug_girl_data_editor(void);
s32 options_debug_girl_data_mode_step_dispatch(void);
void options_debug_day_event_data_editor(void);
s32 options_debug_day_event_data_mode_step_dispatch(void);
void options_debug_scene_launcher_select(void);
s32 options_debug_scene_launcher_mode_step_dispatch(void);
void options_icon_mode_page_dispatch(void);
void options_icon_data_refresh(void);
void options_menu_mode_step_dispatch(void);
void options_voice_mode_page_dispatch(void);
s32 options_text_speed_menu_build(void);
s32 options_text_speed_select(void);
s32 options_text_speed_page_close(void);
s32 options_text_speed_mode_step_dispatch(void);
s32 options_cursor_speed_page_initialize(void);
s32 options_cursor_speed_select(void);
s32 options_cursor_speed_page_exit_zoom(void);
s32 options_cursor_speed_mode_step_dispatch(void);
s32 options_debug_calendar_data_reload(void);
void options_debug_calendar_page_initialize(void);
void options_debug_calendar_audio_wait(void);
s32 options_debug_calendar_mode_step_dispatch(void);
void options_page_zoom_in_update();

extern u8 options_debug_girl_index;
extern s32 D_8013E844[];

/* Sixteen word entries per settings row table, T-14400. */
extern s32 D_8013E744[];
extern s32 D_8013E784[];
extern s32 D_8013E7C4[];
extern s32 D_8013E804[];

/* Audio selection tables: 16-byte text rows, T-14400. */
extern u8 options_voice_test_character_clip_labels[63][16];
extern u8 options_voice_test_music_clip_labels[63][16];
extern u8 options_voice_test_character_labels[58][16];
extern u8 options_voice_test_character_index;
void options_debug_audio_music_load_begin(void);
void options_debug_audio_voice_load_begin(void);
void options_voice_test_selection_gradient_draw(void);
void options_debug_audio_voice_list_page(void);
void options_debug_audio_track_list_page(void);
void options_debug_audio_second_track_list_page(void);

#ifndef OVL_OPTION_DISPATCH_T10020
#define OVL_OPTION_DISPATCH_T10020
void options_debug_audio_menu_update(void);
#endif

#ifndef OVL_OPTION_GRID_T10010
#define OVL_OPTION_GRID_T10010
/* Sixteen menu rectangles stored as separate coordinate/extent arrays. */
typedef struct OptionGrid80 {
    /* 0x00 */ s16 x[16];
    /* 0x20 */ s16 y[16];
    /* 0x40 */ s16 w[16];
    /* 0x60 */ s16 h[16];
} OptionGrid80; /* size 0x80 */
extern u32 *D_8013D2C4[];
extern s32 options_sound_test_advance_armed;
#endif

/* Menu rows of options_debug_audio_menu_init as four parallel arrays (x, y, w, h), passed to menu_set (T-17800). */
extern s16 D_8013D364[7];
extern s16 D_8013D372[7];
extern s16 D_8013D380[7];
extern s16 D_8013D38E[7];

/* Word tables indexed by `options_voice_test_character_index % 58` and by `options_voice_test_music_clip_index % 63` / `options_voice_test_character_clip_index % 63`, read with lw
 * (options_debug_audio_bank_load_music, options_debug_audio_bank_load_voice); each ends where the next label starts (T-17800). */
extern s32 options_voice_test_character_bank_load_commands[58];
extern s32 options_voice_test_character_bank_load_arguments[58];
extern s32 options_voice_test_music_bank_load_arg_1[63];
extern s32 options_voice_test_music_bank_load_arg_2[63];
extern s32 options_voice_test_music_bank_load_arg_3[63];
extern s32 options_voice_test_character_bank_load_arg_1[63];
extern s32 options_voice_test_character_bank_load_arg_2[63];
extern s32 options_voice_test_character_bank_load_arg_3[63];

/* Scroll state of options_background_palette_cycle_update (T-17800). */
extern s32 D_8013D300;
extern s32 D_8013D304;
extern s32 D_8013D308;

/* Four pointers walked up to the next label, D_8013D360 (options_menu_mode_page_initialize, T-17800). */
extern u8 *D_8013D350[4];

/* The BG slot tables layer_bg_maps, layer_bg_cells, layer_bg_cell_indices and their types are in main_api.h (T-25010). */

/* Views of shared records, moved here from the .c files (T-17800). */

/* Four s16 rows filled by options_page_row_geometry_fill for menu_set. */
typedef struct Arrs4x8 {
    /* 0x00 */ s16 a[8];
    /* 0x10 */ s16 b[8];
    /* 0x20 */ s16 c[8];
    /* 0x30 */ s16 d[8];
} Arrs4x8; /* size 0x40 */

void options_page_row_geometry_fill(Arrs4x8 *option_row_geometry);

/* Packed GirlRecord settings fields, T-14400. */
typedef struct OptionCharBits {
    /* 0x00 */ u32 b0 : 1;
    /* 0x00 */ u32 b1 : 1;
    /* 0x00 */ u32 b2 : 1;
    /* 0x00 */ u32 b3 : 1;
    /* 0x00 */ u32 unk_04 : 2;
    /* 0x00 */ u32 b6 : 1;
    /* 0x00 */ u32 b7 : 1;
    /* 0x00 */ u32 b8 : 1;
    /* 0x00 */ u32 f9 : 3;
    /* 0x00 */ u32 b12 : 1;
    /* 0x00 */ u32 b13 : 1;
    /* 0x00 */ u32 unk_14 : 6;
    /* 0x00 */ u32 f20 : 4;
    /* 0x00 */ u32 f24 : 8;
} OptionCharBits; /* size 0x04 */



/* Packed date word at GameState +0xF8 and GirlRecord +0x10: BirthdayBits (main_api.h) plus the two bits at 9-10,
 * read as `& 0xF`, `<< 0x17 >> 0x1B`, `<< 0x15 >> 0x1E` (options_debug_calendar_and_player_birthday_editor, T-17800). */
typedef struct OptionDateBits {
    /* 0x00 */ u32 f0 : 4;
    /* 0x00 */ u32 f4 : 5;
    /* 0x00 */ u32 f9 : 2;
    /* 0x00 */ u32 unk_11 : 21;
} OptionDateBits; /* size 0x04 */


/* 8-bit bitfield view used to prevent uopt from CSE-ing the second debug_hud_enabled load in options_main_menu_select (T-24117). */
typedef struct OptionByteBf {
    /* 0x00 */ u8 x : 8;
} OptionByteBf; /* size 0x01 */

#endif /* OVL_OPTIONS_H */
