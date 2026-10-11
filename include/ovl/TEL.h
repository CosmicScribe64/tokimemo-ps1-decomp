#ifndef OVL_TEL_H
#define OVL_TEL_H

#include "common.h"
#include "main_api.h"

extern u8 D_801405E4;
extern u8 *D_80140AD8[];
extern u8 D_80140B04[];
extern u8 D_80140B5C[];

void func_80135FFC(void);

void func_801360E4(void);
extern u8 *D_80140D18[];
extern u8 D_80140D44[];
extern u8 D_80140D9C[];
void func_801361CC(void);
extern u8 *D_80140F0C[];
extern u8 D_80140F38[];
extern u8 D_80140F90[];
void func_801362B4(void);
void func_80132060(void);
void func_80134BE0(void);
void func_80132000(void);
s32 func_8013AA30(void);
s32 func_8013B54C(void);
void func_8013BF50(void);
s32 func_8013A40C(void);
s32 func_8013AC0C(void);
s32 func_8013A88C(void);
s32 func_8013A820(void);
s32 func_8013BFDC(void);
s32 func_8013BF70(void);
s32 func_8013C4D8(void);
s32 func_8013C46C(void);
s32 func_8013C9D4(void);
s32 func_8013C968(void);
s32 func_8013CBA0(void);
s32 func_8013CB34(void);
void func_8013A76C(void);
s32 func_8013A478(void);
void func_8013BE38(void);
s32 func_8013A79C(void);

void func_801396EC(void);

#endif /* OVL_TEL_H */
void func_80132138(void);
void func_80134DA0(void);

#ifndef OVL_TEL_W9_2_TABLES_H
#define OVL_TEL_W9_2_TABLES_H
/* Signed halfword changes indexed by the status nibble. T-9720. */
extern s16 D_801444A0[];
#endif

#ifndef OVL_TEL_W9_2_BYTE_TABLES_H
#define OVL_TEL_W9_2_BYTE_TABLES_H
/* Per-girl byte penalties read by func_8013BE38, eleven entries, T-9720. */
extern u8 D_801444B8[11];
extern u8 D_801444C4[11];
#endif
