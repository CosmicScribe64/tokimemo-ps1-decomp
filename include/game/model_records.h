#ifndef GAME_MODEL_RECORDS_H
#define GAME_MODEL_RECORDS_H

#include "common.h"

/* 0x50-byte records at view_coordinates (16 entries; D_80122CA0 is the end address the loops compare against) */
typedef struct GraphicsCoordinate2 {
    /* 0x00 */ s32 flag;
    /* 0x04 */ u8 unk4[0x1C];
    /* 0x20 */ s32 translate_z;
    /* 0x24 */ u8 unk24[0x2C];
} GraphicsCoordinate2; /* size 0x50 */

#endif /* GAME_MODEL_RECORDS_H */
