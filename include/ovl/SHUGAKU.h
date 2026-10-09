#ifndef OVL_SHUGAKU_H
#define OVL_SHUGAKU_H

#include "common.h"
#include "game.h"

/* SHUGAKU overlay (load address 0x80132000): externs and types. */

extern s16 D_800CA2DC;
void func_80042940(s32 arg0);
void bg_read_sub2(s32 arg0);

extern u8 D_800E69A1;
extern u8 D_800E69DD;
extern s8 D_800B5BD4;
extern s8 D_8013C2E4;
extern u8 D_8013C2EC;
void func_80042908(s32 arg0);
void func_800847B8(u8 arg0);
void func_80085B3C(s32 arg0, s32 arg1);
void dec_bg_show_switch(s32 arg0);
void get_g_zyotai_s(u8 arg0);

#endif /* OVL_SHUGAKU_H */
