#ifndef MAIN_ENTRY_UI_H
#define MAIN_ENTRY_UI_H

#include "common.h"
#include "game/layer.h"

extern s32 D_800B394C[];
extern s32 D_800B3A3C[];
extern u8 D_800B3C70[];
extern u8 D_800B3C88[];
extern s8 D_800B3DB0;
extern s8 D_800B5BD4;
#ifndef MAIN_API_OVERRIDE_D_800C5FCC
extern s32 D_800C5FCC;
#endif
extern u8 D_800C5FD4[];
extern u8 D_800C6010[];
extern s16 D_800C6014;
#ifndef MAIN_API_OVERRIDE_actor_default_texture_blocks
extern u8 actor_default_texture_blocks[];
#endif
extern u8 actor_default_sequence_table[];
extern s16 D_800C9A54;
extern s32 face_images;
extern s32 face_frame_tables;
extern s32 face_sequences;
extern s32 face_palette;
extern s32 D_800CA13C;
extern s32 D_800CA140;
extern s32 D_800CA144;
extern s16 D_800CA150;
extern u8 scene_place_title[];
extern u8 scene_place_title_2[];
extern u8 name_code_o[]; /* text buffer (SHOUGATU func_801359FC, T-17500) */
extern s16 D_800CA236;
extern u8 name_code_s[];
extern u8 name_code_w[];
extern u8 name_code_a[];
extern u8 name_code_m[];
extern s16 talk_voice_wait_frames;
extern u8 select_girl_menu_enabled[];
extern s16 talk_next_row_y;
extern s16 D_800CA2E8;
extern s8 scene_trip_day_state;
extern u8 D_800E7D11[];
extern u8 D_800E7D14[];
extern u8 D_800E7D15[];
extern u8 D_800E7D1F;
extern u8 D_800E7D37;
extern s32 D_800E7D50;
extern u8 D_800E7D68;
#ifndef MAIN_API_OVERRIDE_D_800E8BEE
extern s8 D_800E8BEE;
#endif
extern u8 D_800E8BEF;
extern s32 draw_ot_table;
extern s32 D_800E8C84;
extern u8 draw_ot_tags[];
extern u8 D_800E90A0[];
extern s8 D_8011ECA4;
extern s16 D_8011ED2C;
extern s16 D_8011ED3A;
extern s16 D_8011ED3E;
extern s32 D_8011ED4C;
extern u8 D_8011ED58[];
extern u8 D_8011F3FF[];
extern s16 D_8011F524;
#ifndef MAIN_API_OVERRIDE_D_80120656
extern s8 D_80120656;
#endif
extern s16 D_80120666;
extern s16 D_8012066A;
extern s8 D_80120693;
extern s16 D_801206AA;
#ifndef MAIN_API_OVERRIDE_D_801206DA
extern s8 D_801206DA;
#endif
extern s8 D_801206DE;
extern s16 D_801206EE;
extern s8 D_8012071B;
extern s8 D_8012071D;
#ifndef MAIN_API_OVERRIDE_D_80120721
extern s8 D_80120721;
#endif
extern s16 D_80120732;
extern s8 D_8012075F;
extern s16 D_80120776;
extern u8 D_801207A6;
extern s16 D_801207BA;
extern s32 D_801207D8;
extern u8 D_801207EA;
extern s32 D_801207F4;
extern s32 D_801207F8;
extern s16 D_801207FE;
extern s32 D_8012081C;
extern u8 D_8012082E;
extern s32 D_80120838;
extern s32 D_8012083C;
extern s16 D_80120844;
extern s32 D_80120860;
extern s16 D_80120912[];
extern s8 D_80120BE3;
extern s16 D_801213B0;
extern s16 D_80121548;
extern s8 D_80121603;
extern s8 D_8012163F;
extern u8 D_8012170F;
extern s16 D_80121726;
extern s16 D_80121728;
extern s16 D_801220D4;
extern s16 D_801220D6;
extern s16 D_801220E8;
extern s16 D_801220EA;
extern s16 D_801220EE;
extern s32 D_801220F0;
extern s32 D_801220F4;
extern BgMap layer_bg_maps[2];
extern BgCell layer_bg_cells[2][64];
extern BgMapIndex layer_bg_cell_indices[2];
extern s32 D_80122750;
extern s32 D_80122758;
extern s32 D_8012275C;
#ifndef MAIN_API_OVERRIDE_view_light_0
extern s32 view_light_0;
#endif
extern s32 D_80122768;
extern s32 view_light_1;
extern s32 D_80122778;
extern s32 view_light_2;
extern s32 D_80122784;
extern s32 D_80122788;
extern u8 D_801227A4[];
extern s32 D_80122CD4;
extern s32 D_80122CDC;
extern s32 D_80122CE0;
extern s32 D_80122CE4;
extern s32 D_80122CEC;
extern s32 D_80122CF0;
extern s32 D_80122CF4;
extern s32 D_80122CF8;
extern s32 D_80122CFC;
extern s32 D_80122D00;
extern s32 D_80122D04;
extern s32 D_80122D0C;
extern s32 D_80122D10;
extern s32 D_80122D20;
extern s32 D_80122D2C;
extern s32 D_80122D30;
extern s32 D_80122D40;
extern s32 D_80122D44;
extern s32 D_80122D4C;
extern u8 D_80122DD0[];
extern s32 audio_bank_wait_done;
extern s32 D_80122EA4;
extern s32 D_80122EAC;
#ifndef MAIN_API_OVERRIDE_D_80122EB8
extern s32 D_80122EB8;
#endif
extern s32 game_mode_scene_girl; /* GirlId or 0xFF; shared main scene work used by the festival overlays; not EVENT storage. */
extern s32 D_80122EC0;
extern s32 D_801230D0;
extern s32 bg_still_bitstream;
extern u8 D_80125C10[];
extern u8 view_base_matrix[];
extern s16 D_80125CA0;
extern s32 D_80125CA4;
extern s32 D_80125CA8;
extern s32 D_80125CAC;
extern s32 D_80125CB0;
extern s32 D_80122E98;
extern s32 D_80122EA8;
extern s32 D_80122EB0;
extern s32 D_80122EB4;
extern u8 D_8011EDE0[];
extern u8 D_8011EEF0[];
/* T-11190: declarations added by tools/bulk_m2c.py. */
/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s16 D_80120BB8;

#endif
