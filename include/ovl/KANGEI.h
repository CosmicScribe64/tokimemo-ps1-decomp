#ifndef OVL_KANGEI_H
#define OVL_KANGEI_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_func_80083440 /* matched with void(s32) (main_api.h: void(u8)) */
#define MAIN_API_OVERRIDE_D_800B3688 /* matched as s32 (main_api.h: s32[]) */
#define MAIN_API_OVERRIDE_D_800B36C8 /* matched as s32 (main_api.h: s32[]) */
#define MAIN_API_OVERRIDE_D_800B3708 /* matched as s32 (main_api.h: s32[]) */

#include "common.h"
#include "game.h"

/* KANGEI overlay (load address 0x80132000): externs and types. */

extern s8 D_80139AE4;
extern s8 D_80139AC0;
extern s8 D_8013A2A8;
extern s16 D_80139ADC;
void func_80133C84(void);

extern s8 D_80139AC4;
extern s32 D_80139DFC;
extern s16 D_801D4094;
extern s32 D_80139A50;
extern s32 D_80139A54;
extern s32 D_80139A58;
extern s8 D_80139DF0;
typedef struct KSub {
    /* 0x00 */ u8 unk_00[8];
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} KSub; /* size unknown, only 0x00-0x0F used */

typedef struct KObj {
    /* 0x00 */ u8 unk_00[0x34];
    /* 0x34 */ KSub *unk_34;
} KObj; /* size unknown, only 0x00-0x37 used */

extern KObj *D_80139AD0;
extern s32 D_80139AD4;
extern s32 D_80139AD8;
extern s32 D_8013A210;
extern s32 D_8013A244;
extern s32 D_8013A278;

extern s32 D_801398B0;
extern s32 D_801398E0;
void func_80132090(void);
void func_8013260C(void);
void func_80132E74(void);
void func_80135F84(void);
void func_80137B90(void);
void func_80137E04(void);

extern s16 D_80139AE0;
extern u8 D_8013A2B0;
extern s32 D_80139DB0;
extern s32 D_80139DB4;
extern s32 D_80139DB8;
extern s32 D_80139DBC;
extern s32 D_80139DC0;
extern s32 D_80139DC4;
extern s32 D_80139DC8;
extern s8 D_80139AC8;
void func_80133A54(void);
void func_80133EF8(void);
void func_801349D4(void);
void func_801392D4(void);

extern s32 D_80139D10;
extern s32 D_80139D14;
extern s32 D_80139D18;
extern s16 D_80139D1C;
extern s32 D_80139D20;
extern s32 D_80139D24;
extern s32 D_80139D28;
extern s16 D_80139D2C;
extern s32 D_8013980C;
void func_80135438(void);

extern s32 D_801398E8;
extern s32 D_801398EC;
extern s32 D_801398F0;
extern s32 D_801398F4;
void func_80139540(void);
void func_80083440(s32 arg0); /* overlay view; main_only.h has the u8 view */
extern s32 D_80139DCC;
extern s32 D_80139DD0;
extern s32 D_80139DD4;
extern s32 D_80139DD8;
extern s32 D_80139DDC;
extern s32 D_80139DE0;
extern s16 D_80139DE4;
extern s16 D_80139DE8;
extern s32 D_80139DEC;
void func_80132214();

void func_80135634();

void func_801355F0();

extern s32 D_80139E10;

extern s32 D_800B36C8;
extern s32 D_800B3708;
extern s32 D_800B3688;

void func_801325D0();

/* defined in C in one object, called from another (T-0500) */
void func_80132E40(void);
extern s32 D_8013A170;
extern s32 D_8013A174;
extern s32 D_8013A178;
extern s32 D_8013A17C;
extern s32 D_8013A180;
extern s32 D_8013A184;
extern s32 D_8013A188;
extern s32 D_8013A18C;
extern s32 D_8013A190;
extern s32 D_8013A194;
extern s32 D_8013A198;
extern s32 D_8013A19C;
extern s32 D_8013A1A0;
extern s32 D_8013A1A4;
extern s32 D_8013A1A8;
extern s32 D_8013A1AC;
extern s32 D_8013A1B0;
extern s32 D_8013A1B4;
extern s32 D_8013A1B8;
extern s32 D_8013A1BC;
extern s32 D_8013A1C0;
extern s32 D_8013A1C4;
extern s32 D_8013A1C8;
extern s32 D_8013A1CC;
extern s32 D_8013A1D0;
extern s32 D_8013A1D4;
extern s32 D_8013A1D8;
extern s32 D_8013A1DC;
extern s32 D_8013A1E0;
extern s32 D_8013A1E4;
extern s32 D_8013A1E8;
extern s32 D_8013A1EC;
extern s32 D_8013A1F0;
extern s32 D_8013A1F4;
extern s32 D_8013A1F8;
extern s32 D_8013A1FC;
extern s32 D_8013A200;
extern s32 D_8013A204;
extern s32 D_8013A208;
extern s32 D_8013A20C;
extern s32 D_8013A214;
extern s32 D_8013A218;
extern s32 D_8013A21C;
extern s32 D_8013A220;
extern s32 D_8013A224;
extern s32 D_8013A228;
extern s32 D_8013A22C;
extern s32 D_8013A230;
extern s32 D_8013A234;
extern s32 D_8013A238;
extern s32 D_8013A23C;
extern s32 D_8013A240;
extern s32 D_8013A248;
extern s32 D_8013A24C;
extern s32 D_8013A250;
extern s32 D_8013A254;
extern s32 D_8013A258;
extern s32 D_8013A25C;
extern s32 D_8013A260;
extern s32 D_8013A264;
extern s32 D_8013A268;
extern s32 D_8013A26C;
extern s32 D_8013A270;
extern s32 D_8013A274;
extern s32 D_8013A27C;
extern s32 D_8013A280;
extern s32 D_8013A284;
extern s32 D_8013A288;
extern s32 D_8013A28C;
extern s32 D_8013A290;
extern s32 D_8013A294;
extern s32 D_8013A298;
extern s32 D_8013A29C;
extern s32 D_8013A2A0;
extern s32 D_8013A2A4;

void func_80134374(void);
extern s32 D_80139950;
extern s32 D_80139954;
extern s32 D_80139958;
extern s32 D_8013995C;
extern s32 D_80139960;
extern s32 D_80139964;
extern s32 D_80139968;
extern s32 D_8013996C;
extern s32 D_80139970;
extern s32 D_80139974;
extern s32 D_80139978;
extern s32 D_8013997C;
extern s32 D_80139980;
extern s32 D_80139984;
extern s32 D_80139988;
extern s32 D_8013998C;
extern s32 D_80139990;
extern s32 D_80139994;
extern s32 D_80139998;
extern s32 D_8013999C;
extern s32 D_801399A0;
extern s32 D_801399A4;
extern s32 D_801399A8;
extern s32 D_801399AC;

void func_801370A0(void);
extern u8 D_80139DF4[];
extern u8 D_80139E0C;
void func_80136238(void);
void func_80136630();
void func_801365C4(void);
void func_80136FDC(void);
void func_80136520(void);
extern s32 D_80139A5C;
extern s32 D_80139A60;
extern s32 D_80139A64;
extern s32 D_80139A68;
extern s32 D_80139A6C;
extern s32 D_80139A70;
extern s32 D_80139A74;
extern s32 D_80139A78;
extern s32 D_80139A7C;
extern s32 D_80139A80;
extern s32 D_80139A84;
extern s32 D_80139A88;
extern s32 D_80139A8C;
extern s32 D_80139A90;
extern s32 D_80139A94;
extern s32 D_80139A98;
extern s32 D_80139A9C;
extern s32 D_80139AA0;
extern s32 D_80139AA4;
extern s32 D_80139AA8;
extern s32 D_80139AAC;
extern s32 D_80139AB0;
extern s32 D_80139AB4;
extern s32 D_80139AB8;
void func_801383D0(void);
s32 func_80132290(void);
extern u8 D_8013A2AC;
void func_801341E8(void);

void func_80138F54(void);
void func_80134DB0(void);

typedef struct KangeiWords11 {
    /* 0x00 */ s32 unk_00[11];
} KangeiWords11; /* size 0x2C */
typedef struct KangeiWords2 {
    /* 0x00 */ s32 unk_00[2];
} KangeiWords2; /* size 0x08 */
extern KangeiWords11 D_80139BA8;
extern KangeiWords2 D_80139BD4;
extern KangeiWords2 D_80139BDC;
extern KangeiWords2 D_80139BE4;
/* T-18400: the same four tables for func_80132C24 (summer variant of func_8013454C). */
extern KangeiWords11 D_80139A08;
extern KangeiWords2 D_80139A34;
extern KangeiWords2 D_80139A3C;
extern KangeiWords2 D_80139A44;
extern s8 D_80139ABC;

/* T-9580: bits 25..26 of the packed +0x66C record word. */
typedef struct KangeiModeFlags {
    u32 low : 25;
    u32 mode : 2;
    u32 high : 5;
} KangeiModeFlags; /* size 0x04 */

/* T-9580: overlay view keeps the packed records at their GameState offset. */
typedef struct KangeiStateView {
    /* 0x000 */ u8 pad_000[0x66C];
    /* 0x66C */ KangeiModeFlags modes[12];
} KangeiStateView; /* size 0x69C */

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80138158(void);

#endif /* OVL_KANGEI_H */
extern u8 D_80139DF8;
void func_80138D40(void);

#ifndef OVL_KANGEI_SPRITE_T9740
#define OVL_KANGEI_SPRITE_T9740
/* 16.16 position: word arithmetic, signed high halfword coordinate (MASTER's MasterFixed32, T-18400). */
typedef union KangeiFixed32 {
    /* 0x00 */ s32 w;
    /* 0x00 */ s16 h[2];
} KangeiFixed32; /* size 0x04 */

/* T-9740: 0x44-byte D_8011ECD0 records used by func_80137484; T-18400: fields of the
 * MASTER clone func_801370A0 (MasterSprite44 layout). */
typedef struct KangeiSprite44 {
    /* 0x00 */ u8 pad_00[2];
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 pad_04[3];
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ u8 pad_0A[0xC];
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ u8 pad_1A[0xC];
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ KangeiFixed32 unk_28;
    /* 0x2C */ u8 pad_2C[0x18];
} KangeiSprite44; /* size 0x44 */

/* T-18400: the ten falling streaks of func_801370A0 (MASTER's D_8013C58C/D_8013C5B4). */
typedef struct KangeiPoint2 {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} KangeiPoint2; /* size 0x04 */
extern KangeiPoint2 D_80139F10[10];
extern s16 D_80139F38[10];
void func_80137484(void);
#endif

/* Integer definition retains the original fade-out result register (T-9740). */
s32 func_80137544(void);

#ifndef OVL_KANGEI_DISPATCH_T10030
#define OVL_KANGEI_DISPATCH_T10030
typedef struct KangeiDispatchTable80 {
    /* 0x00 */ void (*functions[32])(s32);
} KangeiDispatchTable80; /* size 0x80; T-10030 */
extern KangeiDispatchTable80 D_80139D30;
void func_801354AC(void);

#endif
