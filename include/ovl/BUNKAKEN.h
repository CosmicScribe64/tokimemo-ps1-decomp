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
void func_800847B8(s32 arg0);

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

#endif
