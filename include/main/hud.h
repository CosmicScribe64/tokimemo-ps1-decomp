#ifndef MAIN_HUD_H
#define MAIN_HUD_H

#include "common.h"
#include "game/word_views.h"

extern s16 hud_mascot_choice;
void hud_icon_bar_init(void);
void hud_icon_layout_apply();  /* K&R: with an (s32) prototype OPTION.bin no longer matches (T-25108) */
void hud_icon_memory_card_animation_set(u8 arg0);
void hud_icon_bar_update(s32 a);
s32 hud_season_index(void);
s32 hud_in_summer_months(void);
void hud_screen_frame_draw(s32 a);
void hud_season_bgm_load(void);
void hud_day_bgm_load(void);
void hud_mascot_display_switch(s32 visible);
void hud_mascot_set();
void hud_mascot_update(void);
void hud_save_slot_info_show(s32, s32);
s32 hud_magazine_mark_read();
void hud_magazine_page_build();
void hud_appointment_list_sort();
void hud_club_list_menu(void);
void hud_sprite_light_effect_draw(void);
void hud_cursor_update(void);
void hud_cursor_actor_init(s32 arg0);
void hud_cursor_show(s32 arg0);
s32 hud_biorhythm_value(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 hud_biorhythm_birth_offset(void);
void hud_text_marker_set(s32 arg0);
u8 *hud_confirm_button_label(void);
u8 *hud_cancel_button_label(void);
void hud_appointment_add(WordView appointment_entry);

extern u8 D_800B3750[][3];
extern s8 D_800B5A9C; /* T-25108: last selection index seen by hud_icon_bar_update */
extern u8 D_800B5AA0; /* T-25108: blink countdown of hud_icon_bar_update, 0x20 on a selection change */
/* T-17000: two tables of (x, y) halfword pairs that hud_icon_bar_init copies pair by pair into the pair
 * table at D_80125C10 (`lh`/`sh` at +0 and +2, step 4); the copy of the second table from D_800B5AF0
 * (pair 3) runs to D_800B5B20, its end. 16 + 15 pairs fill 0x800B5AA4..0x800B5B20. */
extern s16 D_800B5AA4[16][2];
extern s16 D_800B5AE4[15][2];
extern u32 D_800B5B28[4][8];
extern u32 D_800B5BA8[8];
extern u8 D_800B5BCC;
extern u8 D_800B5BD0;
extern s32 D_800B5BD8[];
extern s32 D_800B5BE8[];
extern s32 D_800B5BF8[];
extern u8 D_800B5C2C[][17];
extern u8 D_800B5D0C[];
extern u8 D_800B5D1C[][17];
extern u8 D_800B5DFC[];
extern u8 D_800B5E0C[];
extern u8 D_800B5E25[];
extern u8 D_800B5E3E[];
extern u8 D_800B5E57[];
extern u8 D_800B5E70[];
extern u8 D_800B5E89[];
extern u8 D_800B5EA4[][3][36];
#ifndef MAIN_API_OVERRIDE_D_800B66A4
extern s32 D_800B66A4[];
#endif
extern s16 D_800B66E0[];
extern s16 D_800B66F4[];
extern u8 D_800B6724[];
extern u8 D_800B6728[];
extern u8 D_800B672C[];
extern u8 D_800B6730[];
extern u8 D_8011ED59;
extern u8 D_8011ED5A;
extern u8 D_8011ED5C;
extern u8 D_8011ED5D;
extern u8 *D_8011ED64;
extern u8 *D_8011ED68;
extern s16 D_8011ED70;
extern s16 D_8011ED7E;
extern u8 *D_8011ED8C;
extern s32 D_8011ED90;
extern s32 D_8011F41C;
extern s16 D_8012059A[];
extern u16 D_800B670A[];

s32 cd_decimal_digit(s16 arg0, u8 arg1);
void parameter_disp_switch();
void parameter_show_init(void);
void parameter_show(void);
void hizuke_disp_switch(s32 arg0);
void hizuke_init(void);
void hizuke_show(void);
void icon_disp_switch(s32 a);
void icon_can_use_set(s32 icon_index, s32 enabled);

extern u8 D_800C6220[];
extern s16 D_8011ED82;
extern u8 D_8011F113;
#ifndef MAIN_API_OVERRIDE_D_8011F414
extern u8 D_8011F414;
#endif
#define HUD_BIORHYTHM_SCORE_SCALE 16 /* T-27062 command and career score scale */
#define HUD_BIORHYTHM_ZERO_WAVE_PROBE_SCALE 3 /* T-27062 zero-return probe scale */
#define HUD_BIORHYTHM_DISCARDED_PROBE_SCALE 2 /* T-27062 unused wrapper discards result */
#define HUD_BIORHYTHM_GRAPH_SCALE -32 /* T-27070 signed scale passed by hud_biorhythm_graph_draw */
#define HUD_BIORHYTHM_ZERO_FAILURES_NONE 0 /* T-27062 no zero-return probes so far */
#define HUD_BIORHYTHM_ZERO_FAILURES_ALL_THREE 3 /* T-27062 all three probes returned zero */
extern u8 D_80125C90[];

#endif
