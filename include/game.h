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
extern u8 D_800B3DC0[];
extern u8 D_800B3DC7[]; /* stride 8 from func_8004E93C: likely a field of an 8-byte struct array */
extern u8 D_800B3F58[];
extern s16 D_800B3F60; /* lh/sh; also lbu elsewhere */
extern s16 D_800B3F62;
extern s16 D_800B3F64;
extern u8 D_800B3F66;
extern s16 D_800B3F68;
extern u8 D_800B3F6A;
extern s32 D_800B58F8;
extern s32 D_800B58FC;
extern u8 D_800B5938[];
extern u8 D_800B593C;
extern u8 D_800B5940;
extern s32 D_800B5948;
extern u8 D_800B6724[];
extern u8 D_800B672C[];
extern u8 D_800B6730[];
extern u8 D_800B6D30;
extern u8 D_800B6D34;
extern u8 D_800B6D38;
extern s32 D_800B6D3C;
extern s32 D_800B6D40;
extern s32 D_800CA120;
extern s32 D_800CA124;
extern s32 D_800CA128;
extern s32 D_800CA130;
extern s32 D_800E36C8[];
extern s32 D_800E36D0[];
extern u16 D_800E36E8;
extern u16 D_800E36EA;
extern u8 D_800E6280[];
extern u8 D_800E62B5;
extern u8 D_800E62B9;
extern u8 D_800E62BA;
extern u8 D_800E62BB; /* also read with lb elsewhere */
extern u8 D_800E699D;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_800E71F4;
extern u8 D_800E71F5;
extern s16 D_800E71FA;
extern s16 D_800E71FC;
extern s32 D_800E7200;
extern s32 D_800E7204;
extern s32 D_800E7208;
extern u8 D_800E7388;
extern u8 D_800E7392;
extern u8 D_800E7393;
extern u8 D_800E7394; /* also read with lhu elsewhere */
extern u8 D_800E7395;
extern u8 D_800E739C;
extern u8 D_800E739D;
extern s32 D_800E73A0;
extern u8 D_8011ECD0[];
extern s16 D_8011ECF6;
extern s16 D_8011ECFA;
extern s32 D_801230D0;
extern u8 D_80125128;
extern u8 D_80125129;
extern u8 D_8012512A;
extern s8 D_8012512B;
extern u8 D_80125130[];
extern s32 D_801255B0;
extern u8 D_801255B4;
extern u8 D_801255B5;
extern s32 D_801255D8;
extern u8 D_801255DC;
extern u8 D_80125C90[];
extern u16 D_80125CC0;
extern u32 D_80125D10;
extern s16 D_80125D3C;
extern u16 D_80125E60;
extern u16 D_80125E62;
extern s16 D_80125E64;
extern s16 D_80125E66;
extern s16 D_80125E68;

/* 8-byte record of the array at D_800B3DC0 (stride 8; see func_8004E93C). */
typedef struct Entry8 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ s8 unk_07;
} Entry8; /* size 0x08 */

#endif /* GAME_H */
