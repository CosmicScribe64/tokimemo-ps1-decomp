#ifndef MAIN_SPRITE_H
#define MAIN_SPRITE_H

#include "common.h"
#include "game/draw_records.h"

extern u8 sprite_frame_table[];
extern SpriteQuad sprite_quads[]; /* at least 12 records */
void sprite_tables_clear(void);
void sprite_quads_clear(s32 arg0, s32 arg1);
void sprite_brightness(s16 sprite_index, u8 brightness);

#ifndef MAIN_API_OVERRIDE_func_80084C98
void func_80084C98();
#endif

#endif
