#ifndef OVL_ENDING_H
#define OVL_ENDING_H

#include "common.h"
#include "libgpu.h"

/* Main-exe data and functions used by ENDING. */
void func_80042878(s32 arg0);
void func_80042908(s32 arg0);
s32 func_8004284C(void);
void func_80044750(s32 arg0);
void bg_read_sub2(s32 arg0);

void func_80046318(s32 arg0, s32 arg1, s32 arg2);
void func_80042808(void);
void normal_date_girl_out(void);
extern u8 D_800E71DF;
extern u8 D_8013C3E0;
extern s32 D_80122CE4;
extern s32 D_8013C360;
extern s8 D_8013C364;
extern s8 D_8011F4CA;
extern s16 D_8011F4D0;
extern s16 D_8011F4DE;
extern s16 D_8011F4E0;
extern u8 D_800E73A4;
extern u8 D_800E683E;
void func_80132000(void);
void func_80133C10(void);
void func_80134D20(void);
void func_8013B6A0(void);
void load_palette(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_800CA130;
void func_800AE0F0(void *dst, void *src);
extern u8 D_800CA188[];
extern u8 D_800E69DD;
extern u8 D_8013BFB8[];
extern u8 D_8013BFC0[];
extern s16 D_80120658;
extern s16 D_80120668;
void func_80083440(s32 arg0);
void func_80062CD0(s32 arg0);
u32 get_h_tokimeki();
void normal_date_speak(void);
extern s32 D_800E6480;
void func_8009C8E0(RECT *rect, void *arg1);
void func_80042940(s32 arg0);
void func_80041584(void);
void func_80048EB8(s32 arg0);
void func_8004E500(s32 arg0);
void func_8004E58C(void);
void func_8006BD6C(s32 arg0);
extern s32 D_80122CD4;
extern s32 D_80122CD0;
extern s32 D_800E7384;
void k_disp_start(s32 arg0);
void func_80044890();
s32 func_80044E8C();
extern s16 D_800CA148;
extern s16 D_800CA14C;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
extern s32 D_8013C2B4;
extern s32 D_8013C2F8;
extern s32 D_8013C33C;
void func_800462C8();
extern u8 D_800E7D34;
void func_800AE0A0(s32 dst, s32 src, s32 n);
extern s8 D_80120652;
extern s16 D_80120666;
extern u8 D_80120696;
extern u8 D_80121874;
void sprite_brightness();
extern s16 D_8011F536;
extern s16 D_8011F4F2;
extern s32 D_801217D0;
extern u8 D_8011ECD0[];

extern u8 *D_800CA134;
extern u8 *D_800CA138;
extern s32 D_800CA13C;
extern s32 D_800CA140;
extern s32 D_800CA144;
extern s32 D_80122D20;
void k_speed_set(s32 arg0);
void k_reset(s32 arg0);

extern s32 D_8013C34C;
extern s32 D_8013C350;
extern s32 D_8013C354;
extern s32 D_8013C358;
extern s32 D_8013C35C;
void func_80132B04(s32 arg0, s32 arg1, s32 arg2);
extern s32 D_800E7518;
extern s32 D_800E751C;
extern s32 D_800E7520;
extern u8 D_8013CA38;

void tpage_buf_clear_all(void);

#endif /* OVL_ENDING_H */
