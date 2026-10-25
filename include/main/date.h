#ifndef MAIN_DATE_H
#define MAIN_DATE_H

#include "common.h"

#ifndef MAIN_API_OVERRIDE_normal_date_girl_in
void normal_date_girl_in();
#endif
s32 normal_date_girl_in_init(void);
s32 normal_date_girl_in_main(void);
#ifndef MAIN_API_OVERRIDE_normal_date_girl_out
void normal_date_girl_out(void);
#endif
void normal_date_girl_out_init(void);
s32 normal_date_girl_out_main(void);
s32 normal_date_bg_fadein(void);
#ifndef MAIN_API_OVERRIDE_normal_date_bg_fadeout
void normal_date_bg_fadeout(void);
#endif
s32 normal_date_bggirl_fadein(void);
s32 normal_date_bggirl_fadeout(void);
void normal_date_move_place(void);
void normal_date_move_place_init(void);
s32 normal_date_move_place_main(void);
void normal_date_three_select(void);
s32 normal_date_three_select_init(void);
s32 normal_date_three_select_main(void);
void normal_date_two_select(void);
void normal_date_two_select_init(void);
s32 normal_date_two_select_main(void);
void normal_date_speak_1line();
#ifndef MAIN_API_OVERRIDE_normal_date_speak
void normal_date_speak(void);
#endif
void normal_date_bg_out(void);

void place_init(void);
void place_init2(void);

#endif
