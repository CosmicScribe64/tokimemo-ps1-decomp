#ifndef OVL_EN_NICHI_H
#define OVL_EN_NICHI_H

#include "common.h"
#include "psx_types.h"
#include "main_api.h"

typedef struct EnObj {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ u8 unk_14[2];
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ u8 unk_18[2];
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u8 unk_20[4];
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
} EnObj; /* size 0x30: record of D_8013984C */

typedef struct Rec30 {
    /* 0x00 */ s32 val;
    /* 0x04 */ u8 unk_04[0x2C];
} Rec30; /* size 0x30 */
extern Rec30 D_80139868[];

/* EN_NICHI data (overlay rodata/data) */
extern s32 D_80139B1C;
extern s32 D_80139B14;
extern s32 D_80139210;

/* EN_NICHI functions */
void func_801320C0(void);
void func_80135FBC(s32 arg0, s32 arg1);
void func_80136B10(void);
void func_801340CC(void);
void func_80134258(s32 arg0);
void func_80133924(void);
void func_80133CA8(void);
void func_801330D0(void);
void func_80135A48(s32 arg0, s32 arg1);
void func_80135600(void);
void func_801359D0(void);
void func_80135F54(void);
void func_801369F4(s32 arg0);
s32 func_80132B40(void);
void func_80132D44(void);
void func_80132E3C(void);
void func_8013556C(void);
void func_80136A2C(void);
void func_801338EC(void);
void func_80132ADC(void);

extern u8 D_8013984C[];

extern s32 D_801391E0;
extern s32 D_801391E4;
extern s32 D_801391E8;
extern s32 D_801391EC;
extern s16 D_801391F0;
extern s32 D_801391F4;
extern s32 D_801391F8;
extern s32 D_801391FC;

extern s32 D_80139AF8;
void func_801333B0();
void func_8013358C();
void func_80132EE8();
void func_80132DC4();

extern s32 D_80139AFC;
extern s32 D_80139278[];
void func_80135ACC();
void func_80134C1C();
void func_801339D4();
void func_80134800();
extern s32 D_8013921C;

void func_801321EC(void);
void func_80132308(void);
void func_80132378(void);
extern s32 D_80139220;
extern s32 D_80139224;
extern s32 D_801392A4;
extern s32 D_801392A8;

extern s32 D_80139B20;

s32 func_80132A74();
void func_80134D74();
void func_80134F2C();
void func_80135044();
void func_8013515C();
s32 func_80135274();

extern u16 D_80139C58;
extern u8 D_80139C34[];
extern u8 D_80139C3C[];
extern u8 D_80139C44[];
extern s16 D_80139C4C[];
void func_80137E2C(void);
s32 func_80137FEC();
#endif /* OVL_EN_NICHI_H */
extern s32 D_80139B24;

#ifndef W9_EN_RESET_TABLE
#define W9_EN_RESET_TABLE
typedef struct EnResetSlot8 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
} EnResetSlot8; /* size 0x08, 15 rows of 12 records, T-9710. */
extern EnResetSlot8 D_801392AC[15][12];
#endif
