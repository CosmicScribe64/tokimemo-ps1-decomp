#ifndef OVL_KANGEI_H
#define OVL_KANGEI_H

#include "common.h"
#include "game.h"

/* KANGEI overlay (load address 0x80132000): externs and types. */

extern s8 D_80139AE4;
extern s8 D_80139AC0;
extern s8 D_8013A2A8;
extern s16 D_80139ADC;
void func_80133C84(void);
void bg_read_sub2(s32 arg0);

extern u8 D_800E69DD;
extern s8 D_80139AC4;
extern s32 D_80139DFC;
extern s32 D_80139A50;
extern s32 D_80139A54;
extern s32 D_80139A58;
extern s32 D_80122CDC;
extern s8 D_80139DF0;
extern s32 D_80139AD0;
extern s32 D_80139AD4;
extern s32 D_80139AD8;
extern s32 D_8013A210;
extern s32 D_8013A244;
extern s32 D_8013A278;
void func_80042808(void);
void func_80042940(s32 arg0);
void func_80085B3C(s32 arg0, s32 arg1);
void normal_date_two_select(void);

#endif /* OVL_KANGEI_H */
