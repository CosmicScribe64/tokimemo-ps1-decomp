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

#endif /* OVL_KANGEI_H */
