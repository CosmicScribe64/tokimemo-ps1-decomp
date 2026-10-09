#ifndef OVL_MASTER_H
#define OVL_MASTER_H

#include "common.h"
#include "game.h"

/* MASTER overlay (load address 0x80132000): externs and types. */

extern u32 D_8013C2D0; /* three pointers set by func_80132000 and siblings */
extern u32 D_8013C2D4;
extern u32 D_8013C2D8;
extern u32 D_8013C4A0; /* same, second group */
extern u32 D_8013C4A4;
extern u32 D_8013C4A8;

extern u32 D_8013C750; /* third group */
extern u32 D_8013C754;
extern u32 D_8013C758;
extern s32 D_8013C710;
extern s32 D_8013C714;
extern s32 D_8013C718;
extern s16 D_8013C71C;
extern s32 D_8013C720;
extern s32 D_8013C724;
extern s32 D_8013C728;
extern s16 D_8013C72C;
extern s32 D_8013C730;
extern s32 D_8013C740;
extern s32 D_8013C744;
extern s32 D_8013C748;
extern s16 D_8013C74C;
extern s32 D_8013C760;
extern s32 D_8013C764;
extern s32 D_8013C768;
extern s16 D_8013C76C;
extern s32 D_8013C770;
void func_80042808();
void Default_Disp();
void func_80138490();
extern s32 D_800E644C;
extern s32 D_8013C4B0;
extern s32 D_8013C4B4;
extern s32 D_8013C4B8;
extern s32 D_8013C4BC;
extern s32 D_8013C4C0;
extern s32 D_8013C4C4;
extern s16 D_8013C4C8;
extern s16 D_8013C4CC;
extern s32 D_8013C4D0;
void func_80048F64();
void func_800674B0();
void func_8006D6E0();
void func_80072B5C();
void func_8008585C();
void func_80138EC0();
extern s8 D_80120651;
extern s8 D_80120653;
extern u8 D_80120655;
extern s8 D_80120656;
extern s8 D_80120657;
extern s32 D_8012065C;
extern s32 D_80120660;
extern s16 D_80120664;
extern s16 D_80120668;
extern s16 D_8012066A;
extern s16 D_8012066C;
extern s16 D_80120676;
extern s16 D_8012067A;
extern s32 D_80120684;
extern s32 D_80120688;
extern s8 D_80120693;
void func_80138F70();

#endif /* OVL_MASTER_H */
