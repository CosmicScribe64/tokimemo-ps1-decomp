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

extern s32 D_8013C620;
extern s32 D_8013C624;
extern s32 D_8013C628;
extern s16 D_8013C62C;
extern s32 D_8013C630;
extern s32 D_8013C634;
extern s32 D_8013C638;
extern s32 D_8013C63C;
extern s32 D_8013C640;
extern s32 D_8013C644;
extern s32 D_8013C648;
extern s32 D_8013C64C;
extern s32 D_8013C650;
extern s32 D_8013C654;
extern s32 D_8013C658;
extern s32 D_8013C65C;
extern s32 D_8013C660;

extern s32 D_8013C6A8;
extern s32 D_8013C6AC;
extern s32 D_8013C6B0;
extern s16 D_8013C6B4;
extern s32 D_8013C6B8;
extern s32 D_8013C6BC;
extern s32 D_8013C6C0;
extern s32 D_8013C6C4;
extern s32 D_8013C6C8;
extern s32 D_8013C6CC;
extern s32 D_8013C6D0;
extern s32 D_8013C6D4;
extern s32 D_8013C6D8;
extern s32 D_8013C6DC;
extern s32 D_8013C6E0;
extern s32 D_8013C6E4;
extern s32 D_8013C6E8;

extern s32 D_8013C6EC;
extern s32 D_8013C6F0;
extern s32 D_8013C6F4;
extern s16 D_8013C6F8;
extern s32 D_8013C6FC;
extern s32 D_8013C700;
extern s32 D_8013C704;
extern s32 D_8013C708;
extern s32 D_8013C70C;
extern s32 D_8013C710;
extern s32 D_8013C714;
extern s32 D_8013C718;
extern s32 D_8013C71C;
extern s32 D_8013C720;
extern s32 D_8013C724;
extern s32 D_8013C728;
extern s32 D_8013C72C;

extern s32 D_8013C730;
extern s32 D_8013C734;
extern s32 D_8013C738;
extern s16 D_8013C73C;
extern s32 D_8013C740;
extern s32 D_8013C744;
extern s32 D_8013C748;
extern s32 D_8013C74C;
extern s32 D_8013C750;
extern s32 D_8013C754;
extern s32 D_8013C758;
extern s32 D_8013C75C;
extern s32 D_8013C760;
extern s32 D_8013C764;
extern s32 D_8013C768;
extern s32 D_8013C76C;
extern s32 D_8013C770;

extern s32 D_8013C7B8;
extern s32 D_8013C7BC;
extern s32 D_8013C7C0;
extern s16 D_8013C7C4;
extern s32 D_8013C7C8;
extern s32 D_8013C7CC;
extern s32 D_8013C7D0;
extern s32 D_8013C7D4;
extern s32 D_8013C7D8;
extern s32 D_8013C7DC;
extern s32 D_8013C7E0;
extern s32 D_8013C7E4;
extern s32 D_8013C7E8;
extern s32 D_8013C7EC;
extern s32 D_8013C7F0;
extern s32 D_8013C7F4;
extern s32 D_8013C7F8;

extern s32 D_8013C7FC;
extern s32 D_8013C800;
extern s32 D_8013C804;
extern s16 D_8013C808;
extern s32 D_8013C80C;
extern s32 D_8013C810;
extern s32 D_8013C814;
extern s32 D_8013C818;
extern s32 D_8013C81C;
extern s32 D_8013C820;
extern s32 D_8013C824;
extern s32 D_8013C828;
extern s32 D_8013C82C;
extern s32 D_8013C830;
extern s32 D_8013C834;
extern s32 D_8013C838;
extern s32 D_8013C83C;

extern s32 D_800E6758;

void func_801396A4(s16 arg0, u8 arg1);

extern s32 D_800E8C70;
extern u8 D_800E8CA0[];
extern u8 *D_800E8C74;
extern s32 D_800E8C84;
extern u8 D_800E90A0[];
extern u8 *D_800E8C88;
extern s16 D_800E6280;
void func_80097D90();
void func_80098380();
void func_80098490();
void func_80098530();
void InitGeom();

void func_801341D8();

extern s32 D_8013C5F0;
extern s32 D_8013C5F4;
extern s32 D_8013C5F8;
extern s16 D_8013C5FC;
extern s32 D_8013C600;
extern s32 D_8013C604;
extern s32 D_8013C608;
extern s32 D_8013C60C;
extern s32 D_8013C610;
extern s32 D_8013C614;
extern s32 D_8013C618;
extern s32 D_8013C61C;

void func_80139B48();
void dtd_on();

void func_80139AA0();

extern s32 D_8013CBB0;
extern s32 D_8013CBB4;
extern s32 D_8013CBB8;
extern s16 D_8013CBBC;
extern s32 D_8013CBC0;
extern s32 D_8013CBC4;
extern s32 D_8013CBC8;
extern s32 D_8013CBCC;
extern s32 D_8013CBD0;
extern s32 D_8013CBD4;
extern s32 D_8013CBD8;
extern s32 D_8013CBDC;

#endif /* OVL_ENDING_H */
