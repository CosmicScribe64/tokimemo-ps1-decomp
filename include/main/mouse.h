#ifndef MAIN_MOUSE_H
#define MAIN_MOUSE_H

#include "common.h"

extern s8 *mouse_pad_buffers[];
extern s32 mouse_position_x[];
extern s32 mouse_position_y[];
extern s32 mouse_x_min;
extern s32 mouse_x_max;
extern s32 mouse_y_min;
extern s32 mouse_y_max;
extern u16 mouse_sense_x;
void SenseMouse(u16 arg0, u16 arg1);
void SetMouse(s32 arg0, u32 arg1, u32 arg2);
void mouse_update(s32 port);

extern u16 mouse_sense_y;

#endif
