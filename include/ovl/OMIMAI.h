#ifndef OVL_OMIMAI_H
#define OVL_OMIMAI_H

#include "common.h"
#include "game.h"

/* OMIMAI overlay (load address 0x80132000): externs and types. */

void func_80132AD0();
void func_80132EB0();
void func_801331B0();
void func_801320A0();
extern s32 D_80134A8C;
extern s32 D_80134ABC;
extern s32 D_80134AEC;
void func_80133424();
void func_801337EC();

/* three parallel tables per object (13 words each), indexed by a fixed slot per value of
 * D_800E71DF (func_8013285C, func_80132EB0, func_801340E4; T-0500) */
extern s32 D_80134A60[];
extern s32 D_80134A90[];
extern s32 D_80134AC0[];
extern s32 D_80134B60[];
extern s32 D_80134B94[];
extern s32 D_80134BC8[];
extern s32 D_80134C30[];
extern s32 D_80134C64[];
extern s32 D_80134C98[];
extern s32 D_80134C5C;
extern s32 D_80134C90;
extern s32 D_80134CC4;
extern u8 D_80134CCC;

#endif /* OVL_OMIMAI_H */
