#ifndef OVL_EN_NICHI_H
#define OVL_EN_NICHI_H

#include "common.h"

/* Main-exe data and functions used by EN_NICHI (old names, see T-0750). */
extern u8 D_800E62BE;
extern u8 D_800E738D;
extern u8 D_80121531;
void func_8004284C(void);
void func_80044750(s32 arg0);
s32 func_800460CC(void);
void func_80046318(s32 arg0, s32 arg1, s32 arg2);

typedef struct Rec30 {
    /* 0x00 */ s32 val;
    /* 0x04 */ u8 unk_04[0x2C];
} Rec30; /* size 0x30 */
extern Rec30 D_80139868[];

extern u8 D_800E71EF;
extern s32 D_800E7374;
extern s32 D_800E7200;
extern s16 D_8011ECF6;
extern s16 D_8011ECFA;
void func_8006BC28(s32 arg0);
void load_palette(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80085E30(s32 arg0, s32 arg1);
void func_8006BD6C(s32 arg0);
void func_800AE120(s32 arg0);
void hizuke_disp_switch(s32 arg0);
void func_80065F34(s32 arg0);
void message_disp_switch(s32 arg0);
void func_8006764C(s32 arg0);

/* EN_NICHI data (overlay rodata/data) */
extern s32 D_80139B1C;
extern s32 D_80139B14;
extern s32 D_80139210;

/* EN_NICHI functions */
void func_801320C0(void);
void func_80135FBC(s32 arg0, s32 arg1);
void func_80136B10(void);
void func_801340CC(void);
void func_80134258(s32 arg0);
void func_80133924(void);
void func_80133CA8(void);
void func_801330D0(void);
void func_80135A48(s32 arg0, s32 arg1);
void func_80135600(void);
void func_801359D0(void);
void func_80135F54(void);
void func_801369F4(s32 arg0);
void func_80132B40(void);
void func_80132D44(void);
void func_80132E3C(void);
void func_8013556C(void);
void func_80136A2C(void);
void func_801338EC(void);
void func_80132ADC(void);

#endif /* OVL_EN_NICHI_H */
