#ifndef MAIN_DRAW_H
#define MAIN_DRAW_H

#include "common.h"

extern s32 draw_buffer;
void draw_shake_update();
void draw_debug_number(s32 a0, s32 a1, s32 a2);

extern s16 draw_shake_x;
extern s16 draw_shake_y;
extern s32 draw_horizontal_blank_stamps[4]; /* four timestamps, draw_frame and draw_time_bars (T-25003) */

void cd_iris_texture_build(void *arg0, s32 arg1, s32 arg2);
void draw2d3d(u8 arg0, u8 arg1);
void back_clear_switch(s32 arg0);
void draw_shake_start(void);
u8 draw_brightness_get(void);
void Scroll(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);
#ifndef MAIN_API_OVERRIDE_Rot_2D
void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4);
#endif
void set_tarao_sprt();
void set_tarao_bg(s32 arg0);

extern u8 draw_shake_counter;
extern u8 draw_work_buffers[];

#endif
