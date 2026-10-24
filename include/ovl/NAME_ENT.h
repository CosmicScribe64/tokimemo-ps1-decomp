#ifndef OVL_NAME_ENT_H
#define OVL_NAME_ENT_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_800E8BEE /* matched as u8 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_D_80120721 /* matched as u8 (main_api.h: s8) */

#include "common.h"
#include "psx_types.h"
#include "main_api.h"
s32 file_select_format_prompt(void);
s32 name_entry_reading_hit_test(void);
void prologue_line_03_run(void);
void prologue_line_04_run(void);
void prologue_scene_05_transition_run(void);
void prologue_yoshio_greeting_run(void);
void prologue_line_11_run(void);
void prologue_rei_banner_run(void);
void prologue_rei_lines_13_and_14_run(void);
void prologue_lines_15_and_16_run(void);
void prologue_line_17_run(void);
void prologue_shiori_load_run(void);
void prologue_shiori_line_18_run(void);
void prologue_shiori_line_19_run(void);
void prologue_lines_20_to_31_run(void);
void prologue_lines_32_to_36_run(void);
void prologue_line_37_run(void);
void prologue_lines_38_to_41_run(void);
void prologue_flash_run(void);
void prologue_closing_run(void);
void prologue_finish_run(void);
extern s32 file_select_menu_mode_lock;
extern u8 D_800E8BEE;
void file_select_settings_restore(void);
void file_select_buttons_input(void);
void prologue_petals_update(s32 arg0);
void prologue_names_crossfade(void);
void prologue_board_load(void);
void prologue_board_scroll(void);
void file_select_loaded_greeting(void);
void file_select_loaded_leave(void);
extern u8 file_select_no_space_text[];
void name_entry_new_game_setup(void);
void file_select_slot_header_draw(void);
extern s32 file_select_information_page;
void file_select_cursor_to_slot(s32 arg0);
void name_entry_load_init(void);
void name_entry_load_fade_in(void);
void opening_story_mode_stage(void);
void opening_movie(void);
extern u8 file_select_no_memory_card_text_1[];
extern u8 file_select_no_memory_card_text_2[];
void file_select_init_settings(void);
void file_select_init_assets(void);
void file_select_init_menus(void);
extern u8 file_select_memory_card_check_text_1[];
extern u8 file_select_memory_card_check_text_2[];
void prologue_arrival_scene_init(void);
void prologue_arrival_line(void);
void prologue_arrival_bank_start(void);
extern u8 name_entry_family_name[8]; /* name-entry buffers 0 and 1 (name_entry_nickname is buffer 2); sized like their definitions, T-17900 */
extern u8 name_entry_given_name[8];
extern u8 name_entry_nickname[];
extern u8 file_select_create_slot_text_1[];
extern u8 file_select_create_slot_text_2[];
extern u8 file_select_create_slot_text_3[];
extern u8 file_select_play_unsaved_text_1[];
extern u8 file_select_play_unsaved_text_2[];
extern u8 file_select_debug_load_valid_text[];
extern u8 file_select_debug_load_failed_text[];
void file_select_menu_mode_unlock(void);
void file_select_slot_summary_draw(void);
extern u8 file_select_new_choose_text_1[];
extern u8 file_select_new_choose_text_2[];
extern u8 file_select_system_file_text_1[];
extern u8 file_select_system_file_text_2[];
extern u8 file_select_system_file_text_3[];
extern u8 file_select_new_text_1[];
extern u8 file_select_new_text_2[];
extern u8 file_select_new_text_3[];
extern u8 file_select_copy_text_1[];
extern u8 file_select_copy_text_2[];
extern u8 file_select_copy_text_3[];
extern u8 file_select_clear_text_1[];
extern u8 file_select_clear_text_2[];
extern u8 file_select_clear_text_3[];

extern u8 D_80120721;
extern s32 name_entry_reading_column;
extern s32 name_entry_reading_row;
extern s32 name_entry_cell_column;
extern s32 name_entry_cell_row;
extern s32 name_entry_tab_hover;
extern s32 name_entry_tab_y[];
extern u8 name_entry_preset_family_names[];
extern u8 name_entry_preset_given_names[];
extern u8 name_entry_preset_nicknames[];
void name_entry_mode_stage_load(void);
void name_entry_mode_stage_input(void);
void name_entry_mode_stage_shiori(void);
/* T-11650: called by name_entry_load_fade_in, name_entry_mode_stage_input and name_entry_mode_stage_shiori before their definitions. */
void name_entry_input_actors_init(void);
void name_entry_background_setup(void);
void name_entry_cell_cursor_update(void);
void name_entry_step_init(void);
void name_entry_frames_draw(s32 arg0);
void name_entry_family_name_input(void);
void name_entry_given_name_init(void);
void name_entry_given_name_input(void);
void name_entry_nickname_init(void);
void name_entry_nickname_input(void);
void name_entry_birthday_frames_draw(s32 arg0);
void name_entry_birthday_init(void);
void name_entry_birthday_input(void);
void name_entry_confirm_init(void);
void name_entry_confirm_frames_draw(s32 arg0);
void name_entry_empty_callback(void);
void name_entry_confirm_input(void);
void name_entry_shiori_birthday_init(void);
void name_entry_shiori_birthday_input(void);
void name_entry_shiori_confirm_init(void);
void name_entry_shiori_confirm_input(void);
void name_entry_commit(void);
s32 name_entry_exit_to_prologue(void);

void file_select_no_space_check(void);
void file_select_no_space_hint(void);

void prologue_petals_init(void);
extern s32 prologue_scene_counter;

void prologue_line_start(void);
void prologue_line_run(void);
s32 prologue_lip_synchronization_lines_20_to_31(void);
extern s8 prologue_line;
void opening_story_load();
void opening_story_scene();
s32 file_select_mode_stage_loaded(void);
extern s32 opening_second_narration_started;

extern s32 file_select_iris_radius;
extern s32 file_select_iris_x;
extern s32 file_select_iris_y;
extern s32 file_select_iris_x_speed;
extern s32 file_select_iris_y_speed;
void file_select_slot_information_show(void);
void name_entry_actor_animation_start(s32 arg0);

extern u8 name_entry_small_digit_uv[]; /* u8 (u, v) pairs, one per decimal digit */
extern u8 name_entry_retained_family_name[];
extern u8 name_entry_retained_given_name[];
extern u8 name_entry_retained_nickname[];
extern s8 name_entry_retained_player_month;
extern s8 name_entry_retained_player_day;
extern s8 name_entry_retained_player_blood;
extern s8 name_entry_retained_shiori_month;
extern s8 name_entry_retained_shiori_day;
extern s8 name_entry_retained_shiori_blood;
extern s8 name_entry_retained_state;

extern s32 name_entry_hiragana_rows;
extern s32 name_entry_hiragana_rows_01;
extern s32 name_entry_hiragana_rows_02;
extern s32 name_entry_hiragana_rows_03;
extern s32 name_entry_hiragana_rows_04;
extern s32 name_entry_hiragana_rows_05;
extern s32 name_entry_hiragana_rows_06;
extern s32 name_entry_hiragana_rows_08;
extern u8 name_entry_wide_digit_uv[];
/* T-9520: globals used by the matched wave-8 functions. */
extern s32 name_entry_hiragana_rows_07;
extern s32 name_entry_hiragana_rows_09;
extern s32 name_entry_hiragana_rows_10;
extern s32 name_entry_hiragana_rows_11;
extern s32 name_entry_hiragana_rows_12;
extern s32 name_entry_hiragana_rows_13;
extern s32 name_entry_hiragana_rows_14;
extern s32 name_entry_hiragana_rows_15;
extern s32 name_entry_hiragana_rows_16;
extern s32 name_entry_katakana_rows;
extern s32 name_entry_katakana_rows_01;
extern s32 name_entry_katakana_rows_02;
extern s32 name_entry_katakana_rows_03;
extern s32 name_entry_katakana_rows_04;
extern s32 name_entry_katakana_rows_05;
extern s32 name_entry_katakana_rows_06;
extern s32 name_entry_katakana_rows_07;
extern s32 name_entry_katakana_rows_08;
extern s32 name_entry_katakana_rows_09;
extern s32 name_entry_katakana_rows_10;
extern s32 name_entry_katakana_rows_11;
extern s32 name_entry_katakana_rows_12;
extern s32 name_entry_katakana_rows_13;
extern s32 name_entry_katakana_rows_14;
extern s32 name_entry_katakana_rows_15;
extern s32 name_entry_katakana_rows_16;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void name_entry_day_grid_numbers_draw(void);
s32 name_entry_profile_store(void);
void name_entry_cheat_names_apply(void);
void file_select_page_clear_counts_draw(void);
void file_select_system_file_rebuild(void);
s32 file_select_system_file_create(void);
/* T-24132: guarded drafts in src/ovl/NAME_ENT/file_select.c. */
void file_select_girl_icons_visibility_update(void);
s32 file_select_page_indicator_update(s32 arg0);

#endif /* OVL_NAME_ENT_H */
extern u8 name_entry_cursor;
void name_entry_birthday_finalize();
extern s32 name_entry_shiori_prompt_clip_start;
extern s32 name_entry_shiori_prompt_clip_end;
extern s32 name_entry_birthday_prompt_clip_start;
extern s32 name_entry_birthday_prompt_clip_end;
void name_entry_birthday_actors_init();
void name_entry_birthday_text_draw();
extern s32 name_entry_tab;
void name_entry_preset_chord_check();
void name_entry_digit_sprite_draw(s32 arg0, s32 arg1, s32 arg2);
void name_entry_number_sprites_draw();



#ifndef OVL_NAME_ENT_T9770_SELECTION_GLOBALS
#define OVL_NAME_ENT_T9770_SELECTION_GLOBALS
extern s32 name_entry_page_scroll; /* Word reset in name_entry_reading_select. T-9770 */
extern s32 name_entry_page_text_length; /* strlen of the help text in name_entry_symbol_page_draw */
extern u8 *name_entry_prompt_text; /* help title shown by name_entry_kanji_page_draw: name_entry_family_prompt_text, name_entry_given_prompt_text or name_entry_nickname_prompt_text */
extern s32 name_entry_reading; /* Word selection and old-value snapshot. T-9770 */
#endif

#ifndef OVL_NAME_ENT_T10010_INPUT_GLOBALS
#define OVL_NAME_ENT_T10010_INPUT_GLOBALS
extern u8 name_entry_given_prompt_text[]; /* Name-entry character buffer. T-10010. */
extern s32 name_entry_given_prompt_clip_start; /* Word arguments to cd_audio_request_set. */
extern s32 name_entry_given_prompt_clip_end;
extern s32 name_entry_nickname_prompt_clip_start;
extern s32 name_entry_nickname_prompt_clip_end;
#endif

#ifndef OVL_NAME_ENT_T10860_H
#define OVL_NAME_ENT_T10860_H
/* Four blood-type text pointers; stride four, T-10860. */
extern u8 *prologue_blood_texts[4];
#endif

#ifndef OVL_NAME_ENT_PROFILE_FLAGS_H
#define OVL_NAME_ENT_PROFILE_FLAGS_H
/* GameState +0x1CC: month/day/blood-type fields read by 80149E3C. T-10860. */
typedef struct NameProfileFlags {
    /* 0x00 */ u32 month : 4;
    /* 0x00 */ u32 day : 5;
    /* 0x00 */ u32 blood_type : 2;
    /* 0x00 */ u32 unk_0B : 21;
} NameProfileFlags; /* size 0x04 */
#endif

#ifndef OVL_NAME_ENT_T10860_FORWARD_H
#define OVL_NAME_ENT_T10860_FORWARD_H
void prologue_palette_adjust(void);
void prologue_background_slot_set(u32 arg0, s32 arg1);
void prologue_white_palette_set(void);
#endif

#ifndef OVL_NAME_ENT_T10860_ACTOR_HELPER_H
#define OVL_NAME_ENT_T10860_ACTOR_HELPER_H
/* Side-effect-only actor setup; no returned value (801473AC assembly). */
void prologue_lip_synchronization_line_11(void);
#endif

#ifndef OVL_NAME_ENT_T10860_FADE_HELPER_H
#define OVL_NAME_ENT_T10860_FADE_HELPER_H
/* Original K&R fallthrough signature, required by 80147BEC's live ranges. */
s32 prologue_white_boxes_draw(void);
#endif

#ifndef OVL_NAME_ENT_DIALOGUE_TABLES_H
#define OVL_NAME_ENT_DIALOGUE_TABLES_H
/* Paired command words, indexed at stride 8 by the dialogue byte (T-10860). */
typedef struct NameCommandPair8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} NameCommandPair8; /* size 0x08 */
extern NameCommandPair8 prologue_line_clips[43];
/* At least 43 dialogue pairs; this splat label also covers trailing untyped data. */
extern NameCommandPair8 prologue_line_clips_second[];
extern u8 prologue_line_speakers[44];
extern s32 prologue_voice_started;
void prologue_actor96_set_scene_06(void);
void prologue_actor99_set_scene_06(void);
#endif

#ifndef OVL_NAME_ENT_T10860_LEVEL_HISTORY
#define OVL_NAME_ENT_T10860_LEVEL_HISTORY
/* T-10860: last getCDlevel sample and four signed samples, indexed by frame & 3. */
extern s32 prologue_cd_level_last;
extern s32 prologue_cd_level_history[4];
#endif

#ifndef OVL_NAME_ENT_T10860_DIALOGUE_TEXT
#define OVL_NAME_ENT_T10860_DIALOGUE_TEXT
/* T-10860: 43 text pointers, immediately before the 44-byte speaker table. */
extern u8 *prologue_line_texts[43];
#endif

#ifndef OVL_NAME_ENT_T10860_SECOND_BOB_HELPER
#define OVL_NAME_ENT_T10860_SECOND_BOB_HELPER
/* T-10860: integer fallthrough view keeps caller 80149840's state in v1. */
s32 prologue_actors_reset_shiori_line_19(void);
#endif

/* The album buffer is also read by main/hud_screens.c. T-10880. */
#include "game.h"

#ifndef OVL_NAME_ENT_T10880_FUNCTIONS
#define OVL_NAME_ENT_T10880_FUNCTIONS
void file_select_page_parameters_draw(void);
void file_select_mode_stage_list(void);
/* Original implicit-int display dispatcher falls through. */
s32 file_select_slot_status_overlay_draw(void);
extern s32 file_select_unused_word_0;
extern s32 file_select_unused_word_1;
extern s32 file_select_unused_word_2;
#endif

#ifndef OVL_NAME_ENT_T10890_BIRTHDAY
#define OVL_NAME_ENT_T10890_BIRTHDAY
/* T-10890: byte selections at four-byte intervals; read with lbu, 255 means unset. */
extern u8 name_entry_player_month;
extern u8 name_entry_player_day;
extern u8 name_entry_player_blood;
extern u8 name_entry_shiori_month;
extern u8 name_entry_shiori_day;
extern u8 name_entry_shiori_blood;
void name_entry_birthday_hit_test(s32 arg0);
#endif

#ifndef OVL_NAME_ENT_T10890_CHARACTER_TABLES
#define OVL_NAME_ENT_T10890_CHARACTER_TABLES
/* T-10890: 17 pointers per kana table (CD28..CD6C and CD6C..CDB0).
 * Existing scalar declarations retain the first-pointer view for older callers. */
typedef struct NameKanaPages44 {
    /* 0x00 */ u8 *text[17];
} NameKanaPages44; /* size 0x44; index = row * 3 + column / 6 */
extern u8 *name_entry_reading_labels[45];
extern u16 *name_entry_candidate_counts;
extern u8 **name_entry_candidate_lists;
void name_entry_character_put(s32 arg0);
void name_entry_overlay_tables_set(void);
#endif

#ifndef OVL_NAME_ENT_T11010_H
#define OVL_NAME_ENT_T11010_H
extern s32 name_entry_confirm_clip_start; /* Word arguments to cd_audio_request_set. */
extern s32 name_entry_confirm_clip_end;
#endif

#ifndef OVL_NAME_ENT_T11010_OPTIONS_H
#define OVL_NAME_ENT_T11010_OPTIONS_H
void file_select_empty_callback(void);
void file_select_icon_layout_validate(void);
/* Option values read back by file_select_settings_restore (one word each; only the low byte is read). */
extern s32 file_select_saved_options_mask;
extern u8 file_select_saved_text_speed;
extern u8 file_select_saved_icon_mode;
extern u8 file_select_saved_menu_mode;
extern u8 file_select_saved_cursor_speed;
extern u8 file_select_saved_voice_mode;
extern u8 file_select_saved_click_mode;
extern u8 file_select_saved_double_click_interval;
extern u8 file_select_saved_confirm_button;
#endif

#ifndef OVL_NAME_ENT_T11010_B_H
#define OVL_NAME_ENT_T11010_B_H
extern u8 name_entry_family_prompt_text[]; /* Argument block passed to name_entry_hiragana_page_draw and friends. */
#endif

#ifndef OVL_NAME_ENT_T11010_C_H
#define OVL_NAME_ENT_T11010_C_H
void file_select_flash_update(void);
extern u8 name_entry_nickname_prompt_text[];
#endif

#ifndef OVL_NAME_ENT_T11010_D_H
#define OVL_NAME_ENT_T11010_D_H
extern s32 name_entry_family_prompt_clip_start; /* Word arguments to cd_audio_request_set. */
extern s32 name_entry_family_prompt_clip_end;
#endif

#ifndef OVL_NAME_ENT_T11010_E_H
#define OVL_NAME_ENT_T11010_E_H
extern u8 name_entry_nickname_end; /* loop bound and terminator: labels inside name_entry_nickname, the original addresses them by name */
extern u8 name_entry_nickname_end_1;
#endif

/* T-11080 forward declarations. */
void file_select_page_girl_stars_draw(void);

/* T-11120 forward declarations. */
void file_select_slot_markers_hide(void);
s32 file_select_system_button_click(void);

#ifndef OVL_NAME_ENT_T17900_TABLES
#define OVL_NAME_ENT_T17900_TABLES
/* T-17900: sixteen flag counters, cleared and counted four at a time (end address name_entry_retained_family_name) and
 * copied into SystemData.flags (system_data) by file_select_system_file_rebuild and file_select_system_file_create. */
extern u8 file_select_rebuild_counts[16];
/* T-17900: second sixteen-byte flag table of file_select_system_file_rebuild (walked four at a time up to D_8014D168). */
extern u8 file_select_rebuild_old_counts[16];
/* T-17900: sprite (u, v) of each decimal digit, indexed by value % 10 in file_select_number_draw; the label
 * covers 0x64 bytes, the rest is zero. */
extern u8 file_select_digit_uv[10][2];
#endif
