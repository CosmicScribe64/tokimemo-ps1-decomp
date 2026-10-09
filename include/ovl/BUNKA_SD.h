#ifndef OVL_BUNKA_SD_H
#define OVL_BUNKA_SD_H

#include "common.h"

/* Main-exe data and functions used by BUNKA_SD (old names, see T-1030). */
void func_8004284C(void);
s32 func_8004500C();
void k_speed_set(s32 arg0);
s32 set_kanji_string();
void k_disp_start(s32 arg0);
extern u32 D_800E7384;
extern u8 D_8011ECD0[];

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
s32 func_800460EC(void);
void func_80045414(s32 arg0, s32 arg1, s32 arg2);
void func_80042940(s32 arg0);
void func_80044750(s32 arg0);
void func_801341A0(s32 arg0, s32 arg1, s32 arg2);
void func_80136904(void);
void func_801369AC(void);
extern s32 D_80122EAC;
void func_80041584(void);
void draw2d3d(s32 arg0, s32 arg1);
void back_clear_switch(s32 arg0);
void tpage_buf_clear(void);
void func_80048DAC(s32 arg0);
void func_80048E78(void);
void k_reset(s32 arg0);
s32 get_k_speed(void);
void hizuke_disp_switch(s32 arg0);
void func_8008585C(void);
void func_80137620(void);
void func_80137824(s32 arg0);
extern s8 D_800E62B6, D_800E62B7, D_800E62B8;
extern s32 D_80122EB8;
extern s32 D_8013C700;
extern s32 D_8013C814[];
extern s32 D_8013CEC8, D_8013CECC, D_8013CED0, D_8013CED4;
extern u8 D_80120696, D_801206DA, D_8012071F, D_80120723, D_80120762, D_80120763, D_80120767;
extern u8 D_8012071E;
extern u8 D_80120652;
extern u32 D_800E7380;
extern s32 D_800E7200;
extern u8 D_800E738A;

extern s32 D_8013B7EC;
extern s32 D_8013C6C8, D_8013C6D4, D_8013C6D8, D_8013C6DC, D_8013C6E0, D_8013C6E4;
extern s32 D_8013CEBC, D_8013CED8;
extern s32 D_8013D988, D_8013D994, D_8013D998, D_8013D99C, D_8013D9A0, D_8013D9A4;

#endif /* OVL_BUNKA_SD_H */
