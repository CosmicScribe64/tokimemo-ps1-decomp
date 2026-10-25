#ifndef MAIN_PRIM_H
#define MAIN_PRIM_H

#include "libgpu.h"

#include "common.h"

#ifndef MAIN_API_OVERRIDE_prim_gradient_rect
void prim_gradient_rect(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
#endif
#ifndef MAIN_API_OVERRIDE_prim_gradient_line
s32 prim_gradient_line();
#endif

void prim_fill_rect(s32 arg0, RECT rect, s32 arg3);
s32 GetWorkBase(s32 arg0, s32 arg1);
void safe_env(s32 arg0);
void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void dtd_on(s32 arg0);
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void prim_glow_disc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
#ifndef MAIN_API_OVERRIDE_prim_flash_gray
void prim_flash_gray(s32 arg0);
#endif
void prim_flash_color_wide(s32 arg0);
void _sprite_set_box_shade_tarao(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u32 arg5, u32 arg6);

extern u8 prim_arena_buffers[];

#endif
