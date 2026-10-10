#ifndef OVL_OMIMAI_H
#define OVL_OMIMAI_H

#include "common.h"
#include "game.h"

/* OMIMAI overlay (load address 0x80132000): externs and types. */

void bg_read_sub2();
void func_80085B3C();
void func_80132AD0();
void func_80132EB0();
void func_801331B0();
void func_8004E58C();
void k_reset();
void addr_init_bustup();
void func_80084E4C();
void func_801320A0();
void normal_date_speak_1line();
void func_80042808();
extern s32 D_800E6378;
extern s32 D_80134A8C;
extern s32 D_80134ABC;
extern s32 D_80134AEC;
void check_k_scroll();
void func_80083808();
void func_80083A10();
void k_disp_inc2();
void func_80133424();
void func_80041584();
void back_clear_switch();
void tpage_buf_clear();
void func_80048DAC();
void func_8008585C();
void func_801337EC();
extern s32 D_800E7368;
void func_80046318(s32 arg0, s32 arg1, s32 arg2); /* overlay view: main defines it with u8 arg0 */

/* matched against the main-exe u8 prototype */
void draw2d3d(u8 arg0, u8 arg1);
extern s32 D_800B3688[];
extern s32 D_800B36C8[];
extern s32 D_800B3708[];

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

extern u16 D_800E6374;
void func_800AE0F0(void *dst, void *src); /* strcpy (SDK libc) */

#endif /* OVL_OMIMAI_H */
