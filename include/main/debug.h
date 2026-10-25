#ifndef MAIN_DEBUG_H
#define MAIN_DEBUG_H

#include "common.h"

void debug_font_load(void);
void debug_print_int_color(s32 a0, s32 a1, s32 a2, s32 a3);
void debug_print_hex(s32 a, s32 b, s32 c);
void debug_print_string(s32 arg0, s32 arg1, s32 arg2);
s32 debug_print_string_color(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern u8 debug_font_uv[];

#endif
