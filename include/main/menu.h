#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "common.h"

extern s16 menu_vram_view_x;
extern s16 menu_vram_view_y;
extern u32 menu_bar_style[16];
#ifndef MAIN_API_OVERRIDE_menu_set
s32 menu_set();
#endif
s32 menu_check();
s32 menu_check_1();
s32 menu_check_2();
void menu_cursor_update();
void menu_bar_color(s32 menu_index, s32 rgb_color, s32 ordering_slot, s32 style_flags);
void menu_bar_show();

void x_taku_menu_set(s32 arg0, s32 arg1);
void x_taku_string_set(s32 first_label, s32 second_label, s32 third_label, s32 choice_count);

#endif
