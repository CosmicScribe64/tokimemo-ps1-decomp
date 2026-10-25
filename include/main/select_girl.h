#ifndef MAIN_SELECT_GIRL_H
#define MAIN_SELECT_GIRL_H

#include "common.h"

#ifndef MAIN_API_OVERRIDE_select_girl_init
void select_girl_init(s32 arg0);
#endif
s32 select_girl_main(s32 omit_back);

s32 select_girl_by_scores(void);

extern s16 select_girl_menu_x[12];
extern s16 select_girl_menu_y[12];
extern s16 select_girl_menu_height[12];
extern s16 select_girl_menu_width[];

#endif
