#ifndef OVL_VALEN_H
#define OVL_VALEN_H

#include "common.h"
#include "game.h"

/* VALEN overlay (load address 0x80132000): externs and types. */

void bg_read_sub2();
void func_800847B8();
void func_80132348();
void func_80062CD0();
void func_80085B3C();
void func_80042940();
extern s16 D_80134520;
void func_80133C70();
void func_80132000();
extern u8 D_800E62BE;
extern s16 D_80134498;
void normal_date_girl_in();
extern u8 D_80134544;
extern s32 D_80134400;
extern s32 D_80134434;
extern s32 D_80134468;
extern s32 D_8013448C;
extern s32 D_80134490;
extern s32 D_80134494;
extern s16 D_8013449C;
void func_80046318(s32 arg0, s32 arg1, s32 arg2); /* overlay view: main defines it with u8 arg0 */

#endif /* OVL_VALEN_H */
