#ifndef OVL_BUNKASAI_H
#define OVL_BUNKASAI_H

#include "common.h"
#include "game.h"

/* BUNKASAI overlay (load address 0x80132000): externs and types. */

extern u32 D_801604F0; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 D_801604F4;
extern u32 D_801604F8;

void func_8006612C();
extern s32 D_8015D774;
void func_800847B8(s32 arg0);
void func_8004284C(void);
extern u32 D_800CA160;
extern u32 D_800CA164;
extern u32 D_800CA168;
extern s16 D_800CA14C;

#endif
