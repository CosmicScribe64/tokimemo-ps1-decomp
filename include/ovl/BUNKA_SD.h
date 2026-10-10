#ifndef OVL_BUNKA_SD_H
#define OVL_BUNKA_SD_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_set_kanji_string /* matched with s32() (main_api.h: void(s32,s32,s32,void*,s32)) */

#include "common.h"
#include "main_api.h"

s32 set_kanji_string();

/* BUNKA_SD functions called across the overlay. */
void func_80132940(void);
void func_80134938(void);
void func_80135630(void);
void func_80135CD4(void);
void func_80136578(void);
void func_80138810(s32 arg0, s32 arg1, s32 arg2);
void func_80132708(void);

void func_80134B88(void);
void func_80134D94(void);
void func_80133984(void);
void func_80136F3C(void);
void func_80137FEC(void);
void func_801377D4(s32 arg0, s32 arg1, s32 arg2);

void func_80135F38(void);
void func_801361B8(void);
void func_80138D48(void);
void func_80138B3C(void);
void func_801341A0(s32 arg0, s32 arg1, s32 arg2);
void func_80136904(void);
void func_801369AC(void);
void func_80137620(void);
void func_80137824(s32 arg0);
extern s32 D_8013C700;
extern s32 D_8013C814[];
extern s32 D_8013CEC8, D_8013CECC, D_8013CED0, D_8013CED4;

extern s32 D_8013B7EC;
extern s32 D_8013C6C8, D_8013C6D4, D_8013C6D8, D_8013C6DC, D_8013C6E0, D_8013C6E4;
extern s32 D_8013CEBC, D_8013CED8;
extern s32 D_8013D988, D_8013D994, D_8013D998, D_8013D99C, D_8013D9A0, D_8013D9A4;

void func_80134260();
void func_80134418();
void func_801389DC();
void func_80138CAC();
void func_8013987C();
void func_801388D0();
void func_80138C34(void);
s32 func_80135160(void);
s32 func_801359B0(void);
s32 func_80136270(void);

extern u8 D_8013B810[];
extern u8 D_8013B938[];
extern u8 *D_8013C6CC;
extern u8 *D_8013C6D0;
#endif /* OVL_BUNKA_SD_H */
