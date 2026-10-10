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

s32 strcmp(u8 *a, u8 *b);
void strcpy(u8 *dst, u8 *src);
extern s32 D_801343F0;
extern s32 D_801343F4;
extern s32 D_801343F8;
extern s32 D_801343FC;
extern s32 D_80134404;
extern s32 D_80134408;
extern s32 D_8013440C;
extern s32 D_80134410;
extern s32 D_80134414;
extern s32 D_80134418;
extern s32 D_8013441C;
extern s32 D_80134420;
extern s32 D_80134424;
extern s32 D_80134428;
extern s32 D_8013442C;
extern s32 D_80134430;
extern s32 D_80134438;
extern s32 D_8013443C;
extern s32 D_80134440;
extern s32 D_80134444;
extern s32 D_80134448;
extern s32 D_8013444C;
extern s32 D_80134450;
extern s32 D_80134454;
extern s32 D_80134458;
extern s32 D_8013445C;
extern s32 D_80134460;
extern s32 D_80134464;
extern s32 D_8013446C;
extern s32 D_80134470;
extern s32 D_80134474;
extern s32 D_80134478;
extern s32 D_8013447C;
extern s32 D_80134480;
extern s32 D_80134484;
extern s32 D_80134488;

#endif /* OVL_VALEN_H */
