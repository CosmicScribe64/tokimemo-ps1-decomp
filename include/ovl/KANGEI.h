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
s32 func_80133A54(void);
void func_80133EF8(void);
void func_801349D4(void);
void func_801392D4(void);

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

void func_80134374(void);

void func_801370A0(void);
extern u8 D_80139DF4[];
extern u8 D_80139E0C;
void func_80136238(void);
void func_80136630();
void func_801365C4(void);
void func_80136FDC(void);
void func_80136520(void);
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
/* T-18400: picture sets. Each set is three pointers into loaded data (D_80139AD0, D_80139AD4,
 * D_80139AD8 get the three components of the current one); the tables hold one row per component. */
extern s32 D_80139950[3][8]; /* size 0x60, filled by func_80132090 */
extern s32 D_80139A50[3]; /* size 0x0C, filled by func_80132E40 */
extern s32 D_80139A5C[3][8]; /* size 0x60, filled by func_80132E40 */
extern s32 D_8013A170[2][3][13]; /* size 0x138, filled by func_80137B90 */
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

/* T-9580: overlay view keeps the packed records at their GameState offset;
 * T-24122 adds the 4-bit club nibble at +0x0F4 (bits 12..15) for func_80134724
 * and 32-bit fixed-point stat words at +0x100..+0x118 for func_80133A54. */
typedef struct KangeiStateView {
    /* 0x000 */ u8 pad_000[0x0F4];
    /* 0x0F4 */ u16 club_low : 12;
    /* 0x0F4 */ u16 club : 4;
    /* 0x0F6 */ u8 pad_0F6[0x00A];
    /* 0x100 */ s32 unk_100;
    /* 0x104 */ s32 unk_104;
    /* 0x108 */ s32 unk_108;
    /* 0x10C */ s32 unk_10C;
    /* 0x110 */ s32 unk_110;
    /* 0x114 */ s32 unk_114;
    /* 0x118 */ s32 unk_118;
    /* 0x11C */ u8 pad_11C[0x550];
    /* 0x66C */ KangeiModeFlags modes[12];
} KangeiStateView; /* size 0x69C */

/* T-18400: overlay callees of the unmatched functions, called before their definitions. */
void func_801335A0(void);
void func_801335F0(void);
void func_801337B4(void);
s32 func_801338EC(void);
void func_80133A14(void);
void func_80133C10(void);
void func_80134120(void);
s32 func_80135678(void);
void func_80136018(void);
void func_80136D58(void);
void func_80138078(void);
void func_80136810(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8);
/* T-18400: club name tables that func_8013769C copies to the stack whole and passes
 * name[D_800E6280.unk_0F4.h >> 12] to func_800AE0F0 (as EVENT's EventTableE7, T-9940). */
typedef struct KangeiNames20 {
    /* 0x00 */ u8 name[11][20];
} KangeiNames20; /* size 0xDC */
typedef struct KangeiNames21 {
    /* 0x00 */ u8 name[11][21];
} KangeiNames21; /* size 0xE7; the inline copy ends with a three-byte tail */
extern KangeiNames20 D_80139FAC;
extern KangeiNames21 D_8013A088;
/* T-18400: two image records copied into display record 96 by func_80135CE0
 * (unk_00 to +0x34, unk_04 to +0x0C, unk_08 to +0x10, unk_0C to +0x14); filled by func_801355F0. */
typedef struct KangeiImage10 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 pad_0E[2];
} KangeiImage10; /* size 0x10 */
extern KangeiImage10 D_80139D10[2];
/* T-18400: fade colour of func_80136630 (byte loads and stores, one word apart). */
extern u8 D_80139E00;
extern u8 D_80139E04;
extern u8 D_80139E08;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80138158(void);

#endif /* OVL_KANGEI_H */
extern u8 D_80139DF8;
void func_80138D40(void);

#ifndef OVL_KANGEI_SPRITE_T9740
#define OVL_KANGEI_SPRITE_T9740


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
