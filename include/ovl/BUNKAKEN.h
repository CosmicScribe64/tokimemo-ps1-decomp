#ifndef OVL_BUNKAKEN_H
#define OVL_BUNKAKEN_H

#include "common.h"
#include "game.h"

/* BUNKAKEN overlay (load address 0x80132000): externs and types. */

extern u32 D_80155E90; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 D_80155E94;
extern u32 D_80155E98;

extern s32 D_80154B84;
extern s32 D_80154B8C;

/* defined in C in one object, called from another (T-0500) */
void func_80132034(void);
void func_80132104(void);
void func_8013216C(void);
void func_801321D4(void);
void func_8013223C(void);
void func_8013230C(void);
void func_80132374(void);
void func_801323DC(void);
void func_80132444(void);
void func_801324AC(void);
void func_801324E0(void);
extern s32 D_801562A0;
extern s32 D_801562A4;
extern s32 D_801562A8;
extern s32 D_801562AC;
extern s32 D_801562B0;
extern s32 D_801562B4;
extern s32 D_801562B8;
extern s32 D_801562BC;
extern s32 D_801562C0;
extern s32 D_801562C4;
extern s32 D_801562C8;
extern s32 D_801562CC;
extern s32 D_801562D0;
extern s32 D_801562D4;
extern s32 D_801562D8;
extern s32 D_801562DC;
extern s32 D_801562E0;
extern s32 D_801562E4;
extern s32 D_801562E8;
extern s32 D_801562EC;
extern s32 D_801562F0;
extern s32 D_801562F4;
extern s32 D_801562F8;
extern s32 D_801562FC;
extern s32 D_80156300;
extern s32 D_80156304;
extern s32 D_80156308;
extern s32 D_8015630C;
extern s32 D_80156310;
extern s32 D_80156314;
extern s32 D_80156318;
extern s32 D_8015631C;
extern s32 D_80156320;
extern s32 D_80156324;
extern s32 D_80156328;
extern s32 D_8015632C;
extern s32 D_80156330;
extern s32 D_80156334;
extern s32 D_80156338;

#endif
