#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "libgpu.h"

/* Game globals referenced by decompiled functions. Types are inferred from the
 * access width in the decompiled functions (grep over asm/nonmatchings/game);
 * where other functions use another width it is noted, so decide the real
 * type when those users are decompiled. Names stay as splat placeholders
 * until understood. */
extern u8 D_800B3D40;
extern u8 D_800B3D44;
extern u8 D_800B3D60;
extern s32 D_800B3D70; /* only sw and &D_800B3D70 seen */
extern u8 D_800B3D80;
extern u8 D_800B3DC0[];
extern u8 D_800B3DC7[]; /* stride 8 from k_disp_switch: likely a field of an 8-byte struct array */
extern u8 D_800B3F58[];
extern s16 D_800B3F60; /* lh/sh; also lbu elsewhere */
extern s16 D_800B3F62;
extern s16 D_800B3F64;
extern u8 D_800B3F66;
extern s16 D_800B3F68;
extern u8 D_800B3F6A;
extern s32 D_800B58F8;
extern s32 D_800B58FC;
extern u8 D_800B5938[]; /* flags; D_800B593C and D_800B5940 are also declared as scalars */
extern u8 D_800B593C;
extern u8 D_800B5940;
extern s32 D_800B5948;
extern u8 D_800B6724[];
extern u8 D_800B672C[];
extern u8 D_800B6730[];
extern u8 D_800B6D30;
extern u8 D_800B6D34;
extern u8 D_800B6D38;
extern s32 D_800B6D3C;
extern s32 D_800B6D40;
extern s32 D_800CA120;
extern s32 D_800CA124;
extern s32 D_800CA128;
extern s32 D_800CA130;
extern s32 D_800E36C8[];
extern s32 D_800E36D0[];
extern u16 D_800E36E8;
extern u16 D_800E36EA;
extern u8 D_800E6280[];
extern u8 D_800E62B5;
extern u8 D_800E62B9;
extern u8 D_800E62BA;
extern u8 D_800E62BB; /* also read with lb elsewhere */
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E699D;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_800E71F4;
extern u8 D_800E71F5;
extern s16 D_800E71FA;
extern s16 D_800E71FC;
extern s32 D_800E7200;
extern s32 D_800E7204;
extern s32 D_800E7208;
extern u8 D_800E7388;
extern u8 D_800E7392;
extern u8 D_800E7393;
extern u8 D_800E7394; /* also read with lhu elsewhere */
extern u8 D_800E7395;
extern u8 D_800E739C;
extern u8 D_800E739D;
extern s32 D_800E73A0;
extern u8 D_8011ECD0[];
extern s16 D_8011ECF6;
extern s16 D_8011ECFA;
extern s32 D_801230D0;
extern u8 D_80125128;
extern u8 D_80125129;
extern s32 D_800E7D10;
extern u8 D_8012512A;
extern s8 D_8012512B;
extern u8 D_80125130[];
extern s32 D_801255B0;
extern u8 D_801255B4;
extern u8 D_801255B5;
extern s32 D_801255D8;
extern u8 D_801255DC;
extern u8 D_80125C90[];
extern u16 D_80125CC0;
extern u32 D_80125D10;
extern s16 D_80125D3C;
extern u16 D_80125E60;
extern u16 D_80125E62;
extern s16 D_80125E64;
extern s16 D_80125E66;
extern s16 D_80125E68;

/* 8-byte record of the array at D_800B3DC0 (stride 8; see k_disp_switch). */
typedef struct Entry8 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ s8 unk_07;
} Entry8; /* size 0x08 */

/* Sync-wait object of strSync (field meanings unknown). */
typedef struct SyncObj {
    /* 0x00 */ u8 unk_00[0x10];
    /* 0x10 */ Entry8 tbl[2];
    /* 0x20 */ s32 idx;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 unk_28[4];
    /* 0x2C */ s32 flag;
} SyncObj; /* size 0x30 */

/* Prototypes for functions called from decompiled code. Argument types come
 * from the call sites only; unprototyped (empty parentheses) where unknown. */
void func_800415B4(s32 arg0, s32 arg1);
void func_8004164C(s32 arg0, s32 arg1);
void func_800418B0(void);
void func_800419FC(void);
void func_80041C2C(void);
void func_80041F48(void);
void func_8004284C(void);
void func_8004B358(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80042908(s32 arg0);
s32 func_8004500C();
void birth_day_check_days(s32 arg0, u8 arg1, u8 arg2);
void dec_bg_show_switch(s32 arg0);
void hizuke_disp_switch(s32 arg0);
void func_8006D138(void);
void func_80090D20(void);
void func_8009C674(s32 arg0);
void func_8009C7F8(RECT *rect, s32 arg1, s32 arg2, s32 arg3);
void func_8009C884(RECT *rect, void *arg1);
void func_8009C8E0(RECT *rect, void *arg1);
void func_8009C93C(RECT *rect, s32 arg1, s32 arg2);
void func_80044750(s32 arg0);
void func_800462BC(u8 arg0, s32 arg1, s32 *arg2);
void func_80045414(s32 arg0, s32 arg1, u8 *arg2);
void func_80048F64(s32 arg0);
void _sys_default_tpage_set(void);

extern s32 D_800E7374;
extern s32 D_800E7384;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern s32 D_8011ECA8;
extern u8 D_800B3220;
extern u8 D_80123120[];
extern s8 D_800E8BEE;
extern u8 D_800AFDF0[];
extern u8 D_800B3D24;
extern u8 D_800B3D54[];
void bzero(void *p, s32 n);
void func_80042488(void);
void card_ev_set(void);
void func_80042C30(void);
void CloseEvent(s32 ev);
void func_8004AE54(s32 a0, s32 a1, s32 a2, s32 a3);
s32 format(u8 *path);
s32 func_80054AF4(s32 arg0);
s32 printf(u8 *fmt, ...);

void k_disp_start(s32 arg0);
void k_reset(s32 arg0);
void func_80059BE8(void);
extern s32 D_800B3D68;
extern u8 D_801255C0[];
s32 func_80087E4C(s32 arg0, u8 *arg1);
extern s32 D_800E7380;
extern u8 D_800E7389;
extern s8 D_800E738C;

void func_80059B40(void);
void func_80042878(s32 arg0);
void Sw_Start(void);
void Hw_Start(void);
s32 InitCARD(s32 arg0);
s32 StartCARD(void);
void _bu_init(void);
s32 _card_auto(s32 arg0);
u32 get_h_tokimeki(s32 arg0);
u32 get_h_yuukou(s32 arg0);
extern s32 D_800B3D6C;
void func_80049A40(s16 a, s16 b, s16 c, s16 d, s32 e, s32 f, s32 g);
s32 TestEvent(s32 ev);
extern s32 D_8011ECBC;
extern s32 D_8011ECC0;
extern s32 D_8011ECC4;
extern s32 D_8011ECC8;
extern s32 D_8011ECAC;
extern s32 D_8011ECB0;
extern s32 D_8011ECB4;
extern s32 D_8011ECB8;

void func_80042960(void);
extern s32 D_800E737C;
extern s8 D_800E738B;
extern s8 D_800E62BC;
extern s8 D_800E71E0;
extern s8 D_800E71E1;
extern s8 D_800E71E2;
extern s8 D_800E71E3;
extern s8 D_800E71EC;
extern s8 D_800E71ED;
extern s8 D_800E71EE;
extern s8 D_800E71F0;
extern s8 D_800E71F1;
extern s8 D_800E71F3;
s32 func_80087954(s32 arg0, u8 *arg1);
extern u8 D_801255C8[];

s32 close(s32 fd);
s32 open(u8 *name, s32 mode);
void func_80056070(u8 *buf, s32 arg1);
extern s32 D_800B58E4;
s32 OpenEvent(u32 desc, s32 spec, s32 mode, void (*func)(void));
s32 EnableEvent(s32 ev);
s32 SetRCnt(u32 spec, u32 target, u32 mode);
s32 StartRCnt(u32 spec);
s32 func_80079E00(s32 arg0);

/* Stack request of the memory-card file functions (func_80054694, func_80054704): name buffer plus a retry
 * counter; layout read from the frame (counter at +0x20 in func_80054704). */
typedef struct FileReq {
    /* 0x00 */ u8 name[32];
    /* 0x20 */ s32 retry;
} FileReq; /* size 0x24 */

void func_80065F34(s32 arg0);
extern s16 D_800CA2AC;
extern u8 D_800E738E;

void dtd_on_tpage(s32 a, s32 b, s32 c, s32 d, s32 e);

extern u8 D_800E8BF0[];
extern u8 D_800E8C30[];
void InitPAD(u8 *buf1, s32 len1, u8 *buf2, s32 len2);
void StartPAD(void);
void ChangeClearPAD(s32 arg0);
void func_8009AD30(s32 arg0);
void func_8009AD50(s32 arg0);
void func_8009AD60(s32 arg0);
void func_80099540(s32 *p);
extern s32 D_80122740;
extern s32 D_80122744;
extern s32 D_80122748;
extern s32 D_8012274C;
extern s32 D_80122750;
extern s32 D_80122754;
extern s32 D_80122758;
extern s32 D_8012275C;
extern s32 D_8011ECA0;
extern u8 D_800E8CA0[];
void AddPrim(void *ot, s32 prim);
void func_8009D294(s32 a, s32 b, s32 c, s32 d, s32 e);
void safe_env(s32 arg0);
s32 GetWorkBase(s32 arg0, s32 arg1);

void set_kanji_string();
s32 strlen(u8 *s);
void DecDCTReset(s32 mode);
void func_800869C8(s32 arg0);
void func_80088150(u8 *p, s32 n);
void func_80088180(s32 a, s32 b, s32 c, s32 d, s32 e);
void strKickCD(s32 arg0);
extern u8 *D_80125C58;
extern s32 D_800B5900;

s32 func_8009ECB0(s32 a, s32 b, s32 c, s32 d);

void func_8004B19C(s32 a, s32 b, s32 c);
s32 GetSp(void);
extern u8 D_800E7D34;

extern u8 D_800B3D48;

void func_80053CC0(void);

void func_8009F0B8(u8 *p);
u8 Sw_Test(void);

extern s32 D_800B5920;
extern s32 D_800B5924;
extern s32 D_800B5928;
extern s32 D_800B592C;
extern u8 D_800B5939;

void func_800869A4(s32 a);
extern s32 D_80125C04;
extern s32 D_80125C08;
extern s32 D_80125C0C;
void k_disp_goto_line_end(void);
s32 k_disp_inc(void);
s32 check_end_k(void);
void func_80097D90(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_80098380(void);
void func_80098490(s32 a, s32 b, s32 c, s32 d);
void func_80098530(void);
void InitGeom(void);
void func_800985A0(void);
s32 func_80098370(void);
void func_800410AC(void);
extern s32 D_800E8C70;
extern u8 *D_800E8C74;
extern s32 D_800E8C84;
extern u8 *D_800E8C88;
extern u8 D_800E90A0[];

void func_800789E0(void);
void func_8007BDE8(void);
void func_8007B844(void);
void func_800452C4(void);
void func_80041878(void);
void func_80042058(void);
void func_8004482C(void);
void dec_bg_reset(void);
void tpage_buf_clear(void);

/* T-1020 batch F */
extern s32 D_800E36F0;
extern s32 D_800E36F4;
void Vblnk_Timer_Init(void);

s32 func_80044E8C();
extern s32 D_80122EA0;
extern s16 D_80120666;

s32 Vblnk_Timer(void);
void func_8006BA40();

void read_bustup();
void func_8006C848();
s32 func_8007B5EC(u16 a);
void func_80047550(void);
extern s32 D_80122D20;

extern u8 D_800E69DD;
extern s32 D_80122CF8;

extern s32 D_80122D0C;
extern u8 D_80120696;
extern s16 D_801206AA;
extern s32 D_80122D10;
extern u8 D_80120652;
extern s32 D_80125D14;
void func_8007A50C(void);

s32 func_8007C8D8();
s32 func_8007D8AC();
void func_8006612C(); /* callers pass one pointer, or nothing */
extern u8 D_800CA19C[];
extern u8 D_800CA1DC[];
void func_80085CD4(u8 a);
void func_800634FC(); /* defined with no parameter; one caller passes a u8 anyway */

void func_80042940(s32 arg0);
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E69A2;
extern s16 D_80125D4C;
void func_8007AF0C(void);
void func_80079E9C(void);
void func_80079F00(void);
void func_80058398();
void func_8007A98C();
extern s32 D_80125D18;
extern s32 D_80125D1C;
extern s32 D_80125D20;
extern s32 D_80125D24;
extern s32 D_80125D28;
extern u8 D_80125D5C;
extern u8 D_80125D5D;

s32 func_80046500(void);
void normal_date_move_place_main(void);
void normal_date_three_select_init(void);
void normal_date_three_select_main(void);
void normal_date_two_select_init(void);
void normal_date_two_select_main(void);
void select_girl_init(void);
void select_girl_main(void);
extern u8 D_80125D54;
void func_8008BEE0();
void func_8008BFB0();
void func_8007A868(void);
void func_8007A924();

void normal_date_move_place_init(void);

extern u8 *D_800CA134;
extern u8 *D_800CA138;
extern s32 D_800CA13C;
extern s32 D_800CA140;
extern s32 D_800CA144;
extern s16 D_800CA148; /* also address-taken by the date code */
extern s16 D_800CA14C;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
void func_80082764();

void func_80048E78();
void icon_disp_switch(s32 a);
void func_80075C24(void);
void func_80066334();
void func_80065B0C(s32 a);
u8 func_800460CC(void);
s32 func_8004636C();
void func_80042458(void);

void func_8007B5CC();
void func_8007B568();
extern s32 D_80122CDC;

void k_speed_set(u8 a);
u8 get_k_speed(void);
extern u8 D_800CA2A8;
extern u8 D_800CA2C4;
/* T-1000 (batch D, 8005A0B0 / 80061710) */
void func_80048E78(void);
void k_sub_disp_start();
extern u8 D_800B00FC[];
extern u8 D_800B0194[];
extern s8 D_800E7313;
void icon_disp_switch();
void func_8004E58C(void);
void func_8006B648(void);
void func_800625C0(void);
void func_80042808(void);
void xa_wait(void);
void LoadSquare(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4);
void load_palette();
extern u8 D_800C9A60[];
extern u8 D_800C9D60[];
void draw2d3d(u8 arg0, u8 arg1);
void func_80048DAC(s32 arg0);
void func_80041584(void);
void set_dec_bri(u8 arg0);
void k_sub_reset_point_set(void);
void sndisp();
extern u8 D_800B0B64[];
extern u8 D_800B0B6C[];
extern s8 D_800E7322;
extern s8 D_800E739A;
void func_8004AC18();
void cal_sprite_init(void);
void func_80067E34(void);
void func_80048DD0();
void func_8006BC28();
void func_8006BD6C();
extern u8 D_800AFF6C[];
extern u8 D_800AFF78[];
void func_80062210(void);
void func_80062524(void);
void func_800623E4(void);
void func_80048EB8();
s32 dec_bg_cd_read(s32 arg0, s32 arg1);
void dec_bg_show_set(s32 arg0, s32 arg1);
s32 func_80066A2C();
extern s32 D_800B5950[];
void hizuke_show();
void message_window_show();
void func_800578F4();
void func_80066C08();
void func_80065B0C();
void parameter_show();
void func_80068EC0();
void pre_syogatu_init();
void uwasa0();
void func_80060B24();
void func_80061634();
s32 func_80044774(s32 arg0);
void func_80046290();
s32 func_8005B43C();
s32 func_8005B2BC();
s32 func_8005B040();
s32 func_8005AF28();
s32 func_8005AFC8(void);
void func_8005B830(void);
void func_8005B8A0(void);
void func_8005B8E0(void);
u8 func_800460CC();
s32 func_8005751C();
void dec_bg_reset();
extern s32 D_800B594C;
s32 func_80060EA0();
void func_80057640();
void message_window_init();
void parameter_disp_switch();
void func_8006764C();
void menu_check();
void menu_bar_show();
void func_8004FC10();
extern s8 D_800E7314;
s32 func_80054388();
void func_80053D10(void);
void back_clear_switch(s32 arg0);
void func_80059048();
void func_8005AD70(void);
void func_8005B39C(void);
extern u8 D_800E6375;
extern s8 D_800E7325;
/* First word of a table of 36-byte sprite entries; byte users take the address: (u8 *)&D_801217D0 + idx * 36. */
extern s32 D_801217D0;
extern s16 D_801217D4;
extern s16 D_801217D6;
extern s16 D_801217D8;
extern s16 D_801217DA;
extern s16 D_801217DC;
extern u8 D_801217DE;
extern u8 D_801217DF;
extern s16 D_801217E0;
extern s16 D_801217E2;
extern u8 D_801217E4;
extern u8 D_801217E5;
extern u8 D_801217E6;
extern s16 D_801217E8;
extern s16 D_801217EA;
extern s16 D_801217EC;
extern s16 D_801217EE;
extern s32 D_801217F0;
extern s16 D_800B5C08;
extern u8 D_800E78BC[];
extern s32 D_800E793C;
extern s32 D_800E7940;
extern u8 D_800E7944;
extern s32 D_800E7948;
extern s32 D_800E794C;
extern u8 D_80125C10[];
extern s16 D_80125C50;
extern s16 D_80125C52;
extern u8 D_80125C54;
extern u8 D_80125C55;
void func_8005C4CC(s32 arg0);
void memcpy();
void icon_can_use_set();
void func_8005AC70(s32 arg0);
void func_80068938();
void func_800676AC();
s32 get_g_zyotai_s(s32 arg0);
void func_8009AD70();
void GsSetAmbient();
void func_8009B340();
extern s32 D_80122760;
extern s32 D_80122764;
extern s32 D_80122768;
extern s32 D_80122770;
extern s32 D_80122774;
extern s32 D_80122778;
extern s32 D_80122780;
extern s32 D_80122784;
extern s32 D_80122788;
extern u8 D_8012276C;
extern u8 D_8012276D;
extern u8 D_8012276E;
extern u8 D_8012277C;
extern u8 D_8012277D;
extern u8 D_8012277E;
extern u8 D_8012278C;
extern u8 D_8012278D;
extern u8 D_8012278E;

/* T-1010: batch E (src/main/80062CD0.c, src/main/8006CB30.c) */
extern u8 D_800B5A64;
extern u16 D_80125CA0;
extern u8 D_800E6376;
void func_80066104();
void func_8006C334(s32 a, s32 b, s32 c, s32 d);
void parameter_show_init(void);
void hizuke_init(void);
void message_window_init(void);
extern u8 D_8011F50F;
extern u8 D_8011F4CB;
extern u8 D_800B5A60;
extern u8 D_800E699C;
extern u8 D_800E62BD;
s32 func_80044E8C(void);
void func_800578F4(s32 a);
void func_80072338(void);
void yuukou_down(void);
void syoushin_up(void);
void _schedule_init(void);
void week_day_exit0(void);
void week_day_main0(void);
u8 func_800460EC(void);
s32 get_last_gamen_mode(void);
void dtd_on(s32 a);
extern u8 D_8011F113;
extern u8 D_8011ED17;
extern s16 D_8011ED82;
void func_80072998(void);
void func_8007132C(void);
void func_80072B20(void);
void func_80070FD0(void);
s32 func_80072B5C(s32 a);
extern u8 D_800B3C6C;
void parameter_show(void);
void message_window_show(void);
void hizuke_show(void);
void func_80067870(void);
void func_80066C08(s32 a);
void func_80067438(void);
void func_80070F80(void);
s32 func_80066A2C(void);
s32 func_80066A84(void);
void func_80044890(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern s32 D_800B5BD8[];
extern s32 D_800B5BE8[];
extern s32 D_800B5BF8[];
void func_80072CA0(void);
void func_80072C68(void);
void func_800737A0(void);
extern u8 D_800C975C[];
extern u8 D_800C9A14[];
extern u8 D_800C9730[];
extern s16 D_800C9A54;
extern u8 D_8011ED15;
extern u8 D_8011ED16;
extern u8 D_8011ED18;
extern u8 D_8011ED19;
extern u8 D_80120651;
extern u8 D_80120653;
extern u8 D_80120654;
extern u8 D_80120655;
extern u8 D_8011F0CD;
extern u8 D_8011F0CE;
extern u8 D_8011F0CF;
extern u8 D_8011F0D0;
extern u8 D_8011F0D1;
extern u8 D_8011F10F;
extern u8 D_8011ED59;
extern u8 D_8011ED5A;
extern u8 D_8011ED5B;
extern u8 D_8011ED5C;
extern u8 D_8011ED5D;
extern s16 D_8011ED28;
extern s16 D_8011ED2C;
extern s16 D_8011ED3A;
extern s16 D_8011ED3E;
extern s16 D_80120664;
extern s16 D_80120668;
extern s16 D_80120676;
extern s16 D_8012067A;
extern s16 D_8011F0E0;
extern s16 D_8011F0E2;
extern s16 D_8011F0E4;
extern s16 D_8011F0F2;
extern s16 D_8011F0F6;
extern s16 D_8011ED70;
extern s16 D_8011ED7E;
extern s32 D_8011ED4C;
extern s32 D_80120688;
extern s32 D_8011F104;
extern s32 D_8011ED90;
extern u8 *D_8011ED20;
extern u8 *D_8011ED24;
extern u8 *D_8011ED48;
extern u8 *D_8012065C;
extern u8 *D_80120660;
extern u8 *D_80120684;
extern u8 *D_8011F0D8;
extern u8 *D_8011F0DC;
extern u8 *D_8011F100;
extern u8 *D_8011ED64;
extern u8 *D_8011ED68;
extern u8 *D_8011ED8C;
void func_80067F04(void);
extern u8 D_800B6728[];
void func_8006764C(s32 on);
extern u8 D_800E62E4[];
s16 func_800688F0(s32 arg0, s32 arg1);
s16 func_80068898(s32 arg0, s32 arg1);
s32 func_8006C700(void);
s32 get_weekly_bg_sector(void);
extern s32 D_800E71E8;

extern u8 D_800E62BE;
extern u8 D_8011ECD3;
void func_8006BA40(void);
void func_800726F0(void);extern u8 D_800E69AC[];
void func_8006D038(void);

extern u8 D_8011F4CA;
extern u8 D_8011F50E;
extern u8 D_8011F3FF[];
extern s16 D_8012059A[];
/* T-1340: first switch functions */
s32 _card_status(s32 chan);
extern u8 D_800CA2FC;
s32 func_8007E390(void);

/* T-2090: wave 2 main */
extern u8 D_800E71EF;
extern u8 D_800E7312;
extern u32 D_80123110;
extern s32 D_80125CA4;
extern s32 D_80125CA8;
extern s32 D_80125CAC;
extern s32 D_80125CB0;
extern u16 D_800C9F60[];
extern u16 D_800C9FF8[];
extern u16 D_800CA048[];
extern u16 D_800CA108[];
void func_80047560();
void func_8005B1A8();
s32 func_8005B0F4();
s32 func_80075C84();
s32 func_80076000();
s32 GetGp();
s32 SetSp();
void InitHeap();
void func_80042134();
s32 func_80046274(void);
void SD_GetCDLevel();
void func_8007AB24();
void func_8008B750();
void func_80090D60();
s32 func_80079524();
void func_8008FD68();
void func_8008FEB0();
void func_8008FED0();
void func_80090DB0();
void func_800949B0();
extern u16 D_80125D34;
extern s32 D_80125120;
extern s32 D_80125124;
s32 func_80075FA0();
extern s32 D_800B5960[];
void func_8007B144();
void func_8007AEC0();
extern s16 D_80125D3A;
void func_8009B560();
void func_80078C48();
void func_80078FE0();
void func_80079014();
void func_80079070();
void func_80078A94();
void func_80057710();
s32 func_8005352C();
extern u8 D_800E62C1;

#endif /* GAME_H */
