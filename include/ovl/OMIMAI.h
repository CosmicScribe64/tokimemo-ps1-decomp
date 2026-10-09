#ifndef OVL_OMIMAI_H
#define OVL_OMIMAI_H

#include "common.h"
#include "game.h"

/* OMIMAI overlay (load address 0x80132000): externs and types. */

void bg_read_sub2();
void func_80085B3C();
void func_80046318();
void func_80132AD0();
void func_80132EB0();
extern s16 D_800CA148;
extern s16 D_800CA14C;
void func_801331B0();
void func_8004E58C();
void k_reset();
void addr_init_bustup();
void func_80084E4C();
void func_801320A0();
void normal_date_speak_1line();
void func_80042808();
extern s32 D_800E6378;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
extern s32 D_80134A8C;
extern s32 D_80134ABC;
extern s32 D_80134AEC;
void func_80046500();
void hizuke_show();
void message_window_show();
void func_80066334();
void func_80066C08();
void check_k_scroll();
void func_80083808();
void func_80083A10();
void k_disp_inc2();
void func_80133424();
extern u8 D_800E7389;
void func_80041584();
void draw2d3d();
void back_clear_switch();
void tpage_buf_clear();
void func_80048DAC();
void func_80048EB8();
void hizuke_init();
void message_window_init();
void func_8008585C();
void func_801337EC();
extern s8 D_800E7322;
extern s32 D_800E7368;

#endif /* OVL_OMIMAI_H */
