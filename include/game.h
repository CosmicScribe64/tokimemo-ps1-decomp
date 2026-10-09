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
void func_80083440(s32 arg0);
void func_80090D20(void);
void func_8009C674(s32 arg0);
void func_8009C7F8(RECT *rect, s32 arg1, s32 arg2, s32 arg3);
void func_8009C884(RECT *rect, void *arg1);
void func_8009C8E0(RECT *rect, void *arg1);
void func_8009C93C(RECT *rect, s32 arg1, s32 arg2);
void func_80044750(s32 arg0);
void func_800462BC(u8 arg0, s32 arg1, s32 *arg2);
void func_80045414(s32 arg0, s32 arg1, u8 *arg2);

extern s32 D_800E7374;
extern s32 D_800E7384;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern s32 D_8011ECA8;
extern u8 D_800B3220;
extern u8 D_80123120[];
extern u8 D_800AFBF0[];
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

/* T-1000 (batch D, 8005A0B0 / 80061710) */
void func_80042940(s32 arg0);
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
void func_8006612C();
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
extern s32 D_80122740;
extern s32 D_80122744;
extern s32 D_80122748;
extern s32 D_8012274C;
extern s32 D_80122750;
extern s32 D_80122754;
extern s32 D_80122758;
extern s32 D_8012275C;
void hizuke_show();
void message_window_show();
void func_8006BA40();
void func_80066334();
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
u8 func_800460CC(void);
s32 func_8005751C();
void dec_bg_reset();
extern s32 D_800B594C;
extern u16 D_800E62C2;
s32 func_80060EA0();
void func_80057640();
void hizuke_init();
void message_window_init();
void parameter_disp_switch();
void func_8006764C();
void func_80044890();
extern u8 D_800B3220;
extern s32 D_800E7200;
extern s32 D_801230D0;
s32 func_80046500(void);
void menu_check();
void menu_bar_show();
void func_8004FC10();
extern s8 D_800E7314;
s32 func_80054388();
void func_80053D10(void);
void func_80065900();
extern u8 D_800E739B;
void back_clear_switch(s32 arg0);
void func_80059048();
extern u8 D_800E62B6;
extern u8 D_800E62B7;
extern u8 D_800E62B8;
extern u8 D_801220E0;
extern u8 D_801220E1;
extern u8 D_801220E2;
void func_8005AD70(void);
void func_8005B39C(void);
extern s32 D_800B37D4[][2];
extern s16 D_800B59F4;
extern s16 D_800B59F0;
extern s32 D_800E7378;
extern u8 D_8011F3FF;
extern u8 D_8011F414;
extern s16 D_8011F422;
extern s16 D_8011F426;
extern u8 D_800E6375;
extern s8 D_800E7325;
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

#endif /* GAME_H */
