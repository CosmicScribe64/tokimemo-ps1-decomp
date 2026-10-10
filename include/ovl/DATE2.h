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
extern s32 D_8013A380;
extern s32 D_8013A388;
extern s32 D_8013A38C;
extern s32 D_8013A390;
extern s32 D_8013A394;
extern s32 D_8013A398;
extern s32 D_8013A39C;
extern s32 D_8013A3A0;
extern s32 D_8013A3A4;
extern s32 D_8013A3A8;
extern s32 D_8013A3AC;
extern s32 D_8013A3B4;
extern s32 D_8013A3B8;
extern s32 D_8013A3BC;
extern s32 D_8013A3C0;
extern s32 D_8013A3C4;
extern s32 D_8013A3C8;
extern s32 D_8013A3CC;
extern s32 D_8013A3D0;
extern s32 D_8013A3D4;
extern s32 D_8013A3D8;
extern s32 D_8013A3E0;
extern s32 D_8013A3E4;
extern s32 D_8013A3E8;
extern s32 D_8013A3EC;
extern s32 D_8013A3F0;
extern s32 D_8013A3F4;
extern s32 D_8013A3F8;
extern s32 D_8013A3FC;
extern s32 D_8013A400;

extern s32 D_8013A79C, D_8013A7D0, D_8013A804;
extern s32 D_8013A6B0;
extern s32 D_8013A6B4;
extern s32 D_8013A6B8;
extern s32 D_8013A6BC;
extern s32 D_8013A6C0;
extern s32 D_8013A6C4;
extern s32 D_8013A6C8;
extern s32 D_8013A6CC;
extern s32 D_8013A6D0;
extern s32 D_8013A6D4;
extern s32 D_8013A6D8;
extern s32 D_8013A6DC;
extern s32 D_8013A6E0;
extern s32 D_8013A6E4;
extern s32 D_8013A6E8;
extern s32 D_8013A6EC;
extern s32 D_8013A6F0;
extern s32 D_8013A6F4;
extern s32 D_8013A6F8;
extern s32 D_8013A6FC;
extern s32 D_8013A700;
extern s32 D_8013A704;
extern s32 D_8013A708;
extern s32 D_8013A70C;
extern s32 D_8013A710;
extern s32 D_8013A714;
extern s32 D_8013A718;
extern s32 D_8013A71C;
extern s32 D_8013A720;
extern s32 D_8013A724;
extern s32 D_8013A728;
extern s32 D_8013A72C;
extern s32 D_8013A730;
extern s32 D_8013A734;
extern s32 D_8013A738;
extern s32 D_8013A73C;
extern s32 D_8013A740;
extern s32 D_8013A744;
extern s32 D_8013A748;
extern s32 D_8013A770;
extern s32 D_8013A778;
extern s32 D_8013A77C;
extern s32 D_8013A780;
extern s32 D_8013A784;
extern s32 D_8013A788;
extern s32 D_8013A78C;
extern s32 D_8013A790;
extern s32 D_8013A794;
extern s32 D_8013A798;
extern s32 D_8013A7A0;
extern s32 D_8013A7A4;
extern s32 D_8013A7AC;
extern s32 D_8013A7B0;
extern s32 D_8013A7B4;
extern s32 D_8013A7B8;
extern s32 D_8013A7BC;
extern s32 D_8013A7C0;
extern s32 D_8013A7C4;
extern s32 D_8013A7C8;
extern s32 D_8013A7CC;
extern s32 D_8013A7D4;
extern s32 D_8013A7D8;
extern s32 D_8013A7E0;
extern s32 D_8013A7E4;
extern s32 D_8013A7E8;
extern s32 D_8013A7EC;
extern s32 D_8013A7F0;
extern s32 D_8013A7F4;
extern s32 D_8013A7F8;
extern s32 D_8013A7FC;
extern s32 D_8013A800;
extern s32 D_8013A808;
extern s32 D_8013B540;
extern s32 D_8013B544;
extern s32 D_8013B548;
extern s32 D_8013B54C;
extern s32 D_8013B550;
extern s32 D_8013B554;
extern s32 D_8013B558;
extern s32 D_8013B55C;
extern s32 D_8013B560;
extern s32 D_8013B564;
extern s32 D_8013B568;
extern s32 D_8013B56C;
extern s32 D_8013B570;
extern s32 D_8013B574;
extern s32 D_8013B578;
extern s32 D_8013B57C;
extern s32 D_8013B580;
extern s32 D_8013B584;
extern s32 D_8013B588;
extern s32 D_8013B58C;
extern s32 D_8013B590;
extern s32 D_8013B594;
extern s32 D_8013B598;
extern s32 D_8013B59C;
extern s32 D_8013B5A0;
extern s32 D_8013B5A4;
extern s32 D_8013B5A8;
extern s32 D_8013B5AC;
extern s32 D_8013B5B0;
extern s32 D_8013B5B4;
extern s32 D_8013B5B8;
extern s32 D_8013B5BC;
extern s32 D_8013B5C0;

#endif /* OVL_DATE2_H */
