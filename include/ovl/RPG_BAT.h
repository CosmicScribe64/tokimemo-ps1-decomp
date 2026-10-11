#ifndef OVL_RPG_BAT_H
#define OVL_RPG_BAT_H

#include "common.h"
#include "game.h"

extern s32 D_8015EDD4;
extern s32 D_8015EDD8;
extern s32 D_8015EDDC;
extern s32 D_8015EDE0;
extern s32 D_8015EDE4;

extern s32 D_8015EC28;
extern s32 D_8015EC2C;
extern s32 D_8015EC6C;
extern s32 D_8015E744;
extern s32 D_8015ED98;
extern s32 D_8015EBDC;
extern s32 D_8015EDC4;
extern s32 D_8015EDF0;
extern s32 D_8015EE1C;
s32 func_8013E9A8(s32);
void func_8013EA60(s32, s32, s32, s32);
extern s32 D_8015EBAC;
extern s32 D_8015EBB0;
void func_8014EBA8(void);
void func_8014EBD0(void);
void func_8014F230(void);
extern s32 D_8015EE10;
void func_8014F258(void);
void func_8014F500(void);
void func_8014F524(void);

void func_80141C70(void);
void func_80141E38(void);
void func_80137280(void);
void func_8014EF8C(void);

extern s32 D_8015ED64[][4];
extern s32 D_8015ED94;

extern s32 D_8015E740;
extern s32 D_8015E748;
void func_8013415C(void);
void func_80134570(void);
void func_80134984(void);
void func_80135B4C(void);
void func_80135D90(void);
void func_80135E80(void);
extern s32 D_8015EDA8[2]; /* defined in RPG_BAT/8014E780.c (T-9010) */
/* Counters that functions of RPG_BAT/8014E780.c reset together: one variable each, because the
 * original shares one `lui $at` between the stores (T-9010). Defined in RPG_BAT/8014E780.c. */
typedef struct RpgBatWords3 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} RpgBatWords3; /* size 0x0C */
typedef struct RpgBatWords4 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} RpgBatWords4; /* size 0x10 */
extern RpgBatWords3 D_8015EDB0;
extern RpgBatWords4 D_8015EE00;
extern RpgBatWords4 D_8015EE30;
extern RpgBatWords3 D_8015EE44;
extern RpgBatWords3 D_8015EE50;
void func_80139510(void);
void func_8013F82C(void);
void func_801596E8(void);
extern s32 D_8015EB98;
extern s32 D_8015EBB4;
extern s32 D_8015EBB8;
extern s32 D_8015EC14;
extern s32 D_8015EC74;
void func_801373A8(void);
extern s32 D_8015EB9C;
s32 func_80144C84();
s32 func_801452C0();
s32 func_801453B4();
extern s32 D_8015EBBC;
void func_8013E7C0(s32 a, s32 b, s32 c, s32 d);
void func_8013E810(s32 a, s32 b, s32 c, s32 d);
void func_8013E97C(s32 a, s32 b, s32 c);

void func_8014F080(s32 arg0);
void func_8014EF3C(void);

void func_8013F0F4(s32 a, s32 b, s32 c);
void func_8013F15C(s32 a, s32 b, s32 c);
s32 func_8013E90C(s32 i);
s32 func_80156870(void);

void func_80153F3C(s32, s32, s32, s32, s32, s32);
extern s32 D_8015EC38;
extern s32 D_8015EC3C;
extern s32 D_8015EC40;
extern s32 D_8015EC44;
extern s32 D_8015ED28;
extern s32 D_8015ED2C;
extern s32 D_8015ED30;
extern s32 D_8015ED34;
extern s32 D_8015EC48;
extern s32 D_8015EC4C;
void func_80155868(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* defined in C in one object, called from another (T-0500) */
s32 func_8013EA00(s32 arg0, s32 arg1);

extern s16 D_8015E5A0[][12];
extern s32 D_8015EE60;
extern s32 D_8015EE64;
void func_8013E8B8(s32 a0, s32 a1, s32 a2);
extern s32 D_8015EE6C;
extern s32 D_8015EE70;
extern s32 D_8015EE78;
extern s32 D_8015EE7C;
extern s32 D_8015EE84;
extern s32 D_8015EE88;
/* T-5000: six words read and written as one object (the original keeps loads after the stores). */
typedef struct RpgRec18 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
} RpgRec18; /* size 0x18 */
extern RpgRec18 D_8015EC58;
void func_80150DE0(void);
extern s32 D_8015EEE8;
extern s32 D_8015EC20;
extern s32 D_8015EC24;
extern s32 D_8015ED9C;
extern s32 D_8015EDC8;
extern s32 D_8015EDE8;
extern s32 D_8015EDF4;
extern s32 D_8015EE40;
void func_8013C1CC(void);
void func_8013C2A0(void);
void func_8013C32C(void);
void func_8013C890(void);
void func_8013CC90(void);
void func_801439E0();
void func_8014B294(void);
s32 func_8014B79C(void);
void func_8014C4FC(void);
void func_8014C804(void);
void func_8014CB0C(void);
void func_8014CE14(void);
extern s32 D_8015EBF0;
extern s32 D_8015EBF4;
extern s32 D_8015EBF8;
void func_8013BBF8();
/* Battle counters; one struct because as1 then keeps the access order of the original (T-4030, func_801452C0). */
typedef struct RpgStat {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u8 pad_0C[0x2C];
    /* 0x38 */ s32 unk_38;
} RpgStat; /* size 0x3C */
extern RpgStat D_8015EBFC;

extern s32 D_8015EE0C;

extern s32 D_8015ED3C[];

extern s32 D_8015EBE4;

extern s32 D_8015ED8C[2]; /* [1] is D_8015ED90; one base symbol keeps loads behind stores (T-4070) */

void func_80134A70();
void func_8013E300();
void func_80136530();
void func_8013C5D0();
void func_80147F10();
void func_80143830();
void func_80141C30();
void func_8013A480();
void func_80132000();
void func_801454F0();
void func_801496F0();
void func_8014B738();
void func_8014ED80();
void func_8014EE98();
void func_80150F84();
extern u8 D_8015E814;
extern s32 D_8015EC18;
void func_8013F1C4();
extern u8 D_8015E904[];
extern u8 D_8015E9CC[];
extern s32 D_8015EC50;
extern s32 D_8015EC54;

extern s32 D_8015EDEC;
void func_8014EF6C(void);
void func_8014F1D4(s32 arg0);
void func_8014F278();
void func_8014F350();
void func_8013F250();
void func_8013F220();

extern s32 D_8015EBCC;
extern s32 D_8015EBD0;
extern s32 D_8015EBD4;
#endif
extern u8 D_8015E9E0[];
extern u8 D_8015EB70[];
extern u8 D_8015E828[];
extern u8 D_8015E8F0[];
