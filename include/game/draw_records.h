#ifndef GAME_DRAW_RECORDS_H
#define GAME_DRAW_RECORDS_H

#include "common.h"

/* Two records at +0x1C, one per draw_buffer (SetWorkBase stores a work address and two words). */
typedef struct PrimArena {
    /* 0x00 */ s32 size;
    /* 0x04 */ s32 used;
    /* 0x08 */ s32 base;
} PrimArena; /* size 0x0C */

/* 32 records at +0x1328 (palette_load_vram). */
typedef struct PaletteLoad {
    /* 0x00 */ u32 *source;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
} PaletteLoad; /* size 0x08 */

/* 33 records at +0x142C (csr_load_vram). */
typedef struct CsrLoad {
    /* 0x00 */ s32 source;
    /* 0x04 */ u8 slot;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
} CsrLoad; /* size 0x10 */

/* 0x24-byte record of the table at sprite_quads (at least 12 records: ENDING walks 12; indexed with
 * stride 0x24, and code that reads entry i - 1 makes splat name D_801217AC). Record 0 is set field by
 * field in settings_room_sick_icon_show; records 0-5 have a flag in bit 31 of unk_00. */
typedef struct SpriteQuad {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ s16 w;
    /* 0x0A */ u16 h;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ u8 tint_r;
    /* 0x15 */ u8 tint_g;
    /* 0x16 */ u8 tint_b;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s16 anchor_x;
    /* 0x1A */ s16 anchor_y;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20;
} SpriteQuad; /* size 0x24 */

#endif /* GAME_DRAW_RECORDS_H */
