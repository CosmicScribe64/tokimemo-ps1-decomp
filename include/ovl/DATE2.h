#ifndef OVL_DATE2_H
#define OVL_DATE2_H

#include "common.h"
#include "libgpu.h"

/* Main-exe data and functions used by DATE2 (old names, see T-1030). */
void func_8004284C(void);
void func_80044750(s32 arg0);
void func_80042878(s32 arg0);
void func_80042908(s32 arg0);
void func_80042940(s32 arg0);
void func_80062CD0(s32 arg0);
void bg_read_sub2(s32 arg0);
void dec_bg_show_switch(s32 arg0);
void func_80042808(void);
void func_80046318(s32 arg0, s32 arg1, s32 arg2);
void func_80083440(s32 arg0);
void func_800847B8(s32 arg0);
void get_p_name();
void read_bustup(void);
extern u8 D_800CA17C;
s32 get_g_zyotai_s();
s32 dec_bg_cd_read(s32 arg0, s32 arg1);
void func_800452C4(void);
void func_8004482C(void);
extern u8 D_800B5938;
extern u8 D_800E738A;
void func_80047560(void);
void hizuke_show(void);
void message_window_show(void);
void func_80066334(void);
void func_80066C08(s32 arg0);
void check_k_scroll(void);
void func_80083808(void);
void func_80083A10(void);
void k_disp_inc2(void);
void func_80138048(void);
void func_80041584(void);
void draw2d3d(s32 arg0, s32 arg1);
void back_clear_switch(s32 arg0);
void tpage_buf_clear(void);
void func_80048DAC(s32 arg0);
void func_80048EB8(s32 arg0);
void hizuke_init(void);
void message_window_init(void);
void func_80084E4C(void);
void func_8008585C(void);
void func_80083474(void);
void *memcpy();
void func_80082764();
void func_80044890();
s32 func_80044E8C(void);
extern void *D_800CA134;
extern void *D_800CA138;
extern s32 D_800CA13C, D_800CA140, D_800CA144;
extern s16 D_800CA148;
extern s32 D_800CA160, D_800CA164, D_800CA168;
extern s8 D_800B3D60;
extern u32 D_800E7384;
extern u8 D_800E69DD;
extern u8 D_800E71DF;
extern u8 D_800E69A2;
extern s32 D_80122CDC;
extern s32 D_80122D20;

extern s16 D_8013A428;
extern u8 D_8013A4C0;
extern s16 D_8013A42C;
extern u8 D_8013A4C4;
extern s32 D_8013A80C, D_8013A810, D_8013A814;
extern s32 D_8013B5EC, D_8013B5F0, D_8013B5F4;
extern u8 D_8013B5F8, D_8013B5FC;
extern s32 D_8013B5D4, D_8013B5D8, D_8013B5DC, D_8013B5E0;

/* DATE2 functions called across the overlay. */
void func_80132000(void);
void func_80133258(void);
void func_801335A4(void);
void func_80136D50(void);
void func_80137360(void);
void func_80137D50(void);
void func_80138100(void);
void func_80132670(void);
void func_80133620(void);
void func_80137A2C(void);

extern u8 D_8013A818;
extern s32 D_8013A384, D_8013A3B0, D_8013A3DC, D_8013A410, D_8013A414, D_8013A418;
extern u8 D_8013A430, D_8013A434, D_8013A438, D_8013A43C;
extern s32 D_8013A774, D_8013A7A8, D_8013A7DC;
extern s16 D_800CA14C;
extern s32 D_8013A5B0, D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8;
extern s32 D_8013A404, D_8013A408, D_8013A40C;

void func_801370E4(void);
void func_8009C884(RECT *rect, void *arg1);

extern u8 D_800E62BE;
void func_801348F0();
void func_80133AF0();
void func_801350A4();
void func_80135E50();

#endif /* OVL_DATE2_H */
