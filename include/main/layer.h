#ifndef MAIN_LAYER_H
#define MAIN_LAYER_H

#include "common.h"

extern u8 layer_records[];
s32 srn_vram_set(s32, s32, s32, s32);
void move_255_line(s32 arg0, s32 arg1, s32 arg2);
void mod_trans_vram(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 get_fnt(u8 *output, u8 *source, s32 glyph, s32 divisor, s32 width, u8 flags);
void srn_init(s32, u32 *, s32);
void layer_tileset_init(s32 arg0, u32 *arg1, s32 arg2);

extern u8 layer_tileset_records[];

#endif
