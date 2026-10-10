#ifndef OVL_DATE2_H
#define OVL_DATE2_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_800B5938 /* matched as u8 (main_api.h: u8[]) */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"

extern u8 D_800B5938;
void func_80138048(void);

extern s16 D_8013A428;
extern u8 D_8013A4C0;
extern s16 D_8013A42C;
extern u8 D_8013A4C4;
extern s32 D_8013A80C, D_8013A810, D_8013A814;
extern s32 D_8013B5EC, D_8013B5F0, D_8013B5F4;
extern u8 D_8013B5F8, D_8013B5FC;
extern s32 D_8013B5D4, D_8013B5D8, D_8013B5DC, D_8013B5E0;

/* DATE2 functions called across the overlay. */
void func_80132000(void);
void func_80133258(void);
void func_801335A4(void);
void func_80136D50(void);
void func_80137360(void);
void func_80137D50(void);
void func_80138100(void);
void func_80132670(void);
void func_80133620(void);
void func_80137A2C(void);

extern u8 D_8013A818;
extern s32 D_8013A384, D_8013A3B0, D_8013A3DC, D_8013A410, D_8013A414, D_8013A418;
extern u8 D_8013A430, D_8013A434, D_8013A438, D_8013A43C;
extern s32 D_8013A774, D_8013A7A8, D_8013A7DC;
extern s32 D_8013A5B0, D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8;
extern s32 D_8013A404, D_8013A408, D_8013A40C;

void func_801370E4(void);

void func_801348F0();
void func_80133AF0();
void func_801350A4();
void func_80135E50();

#endif /* OVL_DATE2_H */
