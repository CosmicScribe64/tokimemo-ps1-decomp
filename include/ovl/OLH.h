#ifndef OVL_OLH_H
#define OVL_OLH_H

#include "common.h"
#include "common.h"
#include "game_enums.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/prim.h"
#include "main/state.h"
#include "main/text.h"

/* Main-exe functions used by OLH come from game.h (T-1200). */

/* OLH-local functions called from C (T-0500) */
void help_system_settings_menu_draw(void);
void help_system_settings_menu_input(void);
s32 help_system_settings_page_1_render(void);
s32 help_system_settings_page_2_render(void);
s32 help_system_settings_page_3_render(void);
s32 help_system_settings_page_4_render(void);
s32 help_system_settings_page_5_render(void);
s32 help_system_settings_page_6_render(void);
s32 help_system_settings_page_7_render(void);
void help_memory_card_menu_draw(void);
void help_memory_card_menu_input(void);
s32 help_memory_card_page_1_render(void);
s32 help_memory_card_page_2_render(void);
s32 help_memory_card_page_3_render(void);
s32 help_memory_card_page_4_render(void);
s32 help_memory_card_page_5_render(void);
s32 help_memory_card_page_6_render(void);
s32 help_memory_card_page_7_render(void);
void help_biorhythm_menu_draw(void);
void help_biorhythm_menu_input(void);
s32 help_biorhythm_page_1_render(void);
s32 help_biorhythm_page_2_render(void);
s32 help_biorhythm_page_3_render(void);
s32 help_biorhythm_page_4_render(void);
s32 help_biorhythm_page_5_render(void);
s32 help_biorhythm_page_6_render(void);
s32 help_biorhythm_page_7_render(void);
void help_icon_menu_draw(void);
void help_icon_menu_input(void);
s32 help_icon_page_1_render(void);
s32 help_icon_page_2_render(void);
s32 help_icon_page_3_render(void);
s32 help_icon_page_4_render(void);
s32 help_icon_page_5_render(void);
s32 help_icon_page_6_render(void);
s32 help_icon_page_7_render(void);
void help_subgame_menu_draw(void);
void help_subgame_menu_input(void);
s32 help_subgame_page_1_render(void);
s32 help_subgame_page_2_render(void);
s32 help_subgame_page_3_render(void);
s32 help_subgame_page_4_render(void);
s32 help_subgame_page_5_render(void);
s32 help_subgame_page_6_render(void);
s32 help_subgame_page_7_render(void);
void help_small_tips_menu_draw(void);
void help_small_tips_menu_input(void);
s32 help_small_tips_page_1_render(void);
s32 help_small_tips_page_2_render(void);
s32 help_small_tips_page_3_render(void);
s32 help_small_tips_page_4_render(void);
s32 help_small_tips_page_5_render(void);
s32 help_small_tips_page_6_render(void);
s32 help_small_tips_page_7_render(void);
void help_file_select_menu_draw(void);
void help_file_select_menu_input(void);
s32 help_file_select_page_1_render(void);
s32 help_file_select_page_2_render(void);
s32 help_file_select_page_3_render(void);
s32 help_file_select_page_4_render(void);
s32 help_file_select_page_5_render(void);
s32 help_file_select_page_6_render(void);
s32 help_file_select_page_7_render(void);
extern s32 D_8013BF60;
extern u8 *D_8013CEB0;
extern s32 D_8013BF6C;
extern s32 D_8013BF70;
extern s32 D_8013D6C4;
extern s32 D_8013D6C8;
extern s32 D_8013D6CC;
extern s32 D_8013AF20;

extern s32 D_8013C780;

extern s32 D_8013B740;
extern s32 D_8013B744;
extern s32 D_8013B74C;
extern s32 D_8013B750;
extern s32 D_8013B754;
extern s32 D_8013B758;
extern u8 *D_8013CEA0;
extern u8 *D_8013CEA4;
extern u8 *D_8013CEA8;
extern u8 *D_8013CEAC;
extern u8 *D_8013CEB4;

extern s32 D_8013AF24;
extern s32 D_8013AF28;
extern s32 D_8013AF2C;
extern s32 D_8013AF30;
extern s32 D_8013AF34;
extern s32 D_8013AF38;
extern s32 D_80139EE0;
extern s32 D_8013A700;
extern s32 D_8013A708;
extern s32 D_8013A710;
extern s32 D_8013A714;
extern s32 D_8013A718;
extern s32 D_8013BF74;
extern s32 D_8013BF64;
extern s32 D_8013BF68;
extern s32 D_8013BF78;
extern s32 D_8013D6D0;
extern s32 D_8013D6D4;
extern s32 D_8013D6D8;
extern s32 D_8013D6C0;
extern s32 D_8013AF3C;
extern s32 D_8013C79C;
/* T-11190: declarations added by tools/bulk_m2c.py. */
void help_category_menu_draw(void);
void help_category_menu_input(void);
void help_return_to_command_screen(void);
void help_background_gradient_draw(void);

#endif /* OVL_OLH_H */
extern s32 D_8013A704;
extern s32 D_8013A70C;
extern s32 D_8013C784;
extern s32 D_8013C788;
extern s32 D_8013C78C;
extern s32 D_8013C790;
extern s32 D_8013C794;
extern s32 D_8013C798;
extern s32 D_8013B748;
