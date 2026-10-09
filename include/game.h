#ifndef GAME_H
#define GAME_H

#include "common.h"

/* Game globals referenced by decompiled functions. Types are inferred from the
 * access width in the decompiled functions (grep over asm/nonmatchings/game);
 * where other functions use another width it is noted, so decide the real
 * type when those users are decompiled. Names stay as splat placeholders
 * until understood. */
extern u8 D_800B3D40;
extern u8 D_800B3D44;
extern s32 D_800B3D70; /* only sw and &D_800B3D70 seen */
extern u8 D_800B3D80;
extern u8 D_800B3DC7[]; /* stride 8 from func_8004E93C: likely a field of an 8-byte struct array */
extern s16 D_800B3F60; /* lh/sh; also lbu elsewhere */
extern s16 D_800B3F64;
extern u8 D_800B3F66;
extern u8 D_800B3F6A;
extern s32 D_800B58F8;
extern s32 D_800B58FC;
extern u8 D_800B5938[];
extern u8 D_800B593C;
extern u8 D_800B5940;
extern s32 D_800B5948;
extern u8 D_800B5A64;
extern u16 D_800E36E8;
extern u16 D_800E36EA;
extern u8 D_800E62B5;
extern u8 D_800E62B9;
extern u8 D_800E62BA;
extern u8 D_800E62BB; /* also read with lb elsewhere */
extern u8 D_800E699D;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern s32 D_800E7384;
extern u8 D_800E7388;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern u8 D_800E7392;
extern u8 D_800E7393;
extern u8 D_800E7394; /* also read with lhu elsewhere */
extern u8 D_800E7395;
extern u8 D_800E739C;
extern s32 D_800E73A0;
extern u8 D_8011F4CA;
extern u8 D_80120652;
extern u8 D_80120696;
extern u8 D_8012512A;
extern s32 D_801255B0;
extern u8 D_801255B4;
extern u8 D_801255B5;
extern s32 D_801255D8;
extern u8 D_801255DC;
extern u8 D_80125C90[];
extern u16 D_80125CC0;
extern s16 D_80125D3C;

#endif /* GAME_H */
