#ifndef OVL_EN_NICHI_H
#define OVL_EN_NICHI_H

#include "common.h"
#include "main_api.h"

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

#endif /* OVL_EN_NICHI_H */
