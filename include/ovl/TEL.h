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


#ifndef OVL_TEL_T10040
#define OVL_TEL_T10040
/* Dialogue pointer tables and packed word streams, T-10040. */
extern s32 D_80140554;
extern s32 D_80140558;
extern s32 D_8014055C;
extern u8 *D_801434D4[];
extern u32 D_80143500[];
extern u32 D_80143558[];
extern u8 *D_801437BC[];
extern u8 *D_80143958[];
extern u32 D_80143984[];
extern u32 D_801439DC[];
extern u8 *D_80143BA0[];
extern u32 D_80143BCC[];
extern u32 D_80143C24[];
extern u32 D_80144E3C[];
extern u32 D_80144E44[];
extern u32 D_80144E4C[];
extern u32 D_80144EAC[];

/* Forward declarations for the original object, T-10040. */
void func_8013226C(void);
void func_80132FC4(void);
void func_80133D54(void);
void func_801345C4(void);
void func_801352F0(void);
void func_80136724(void);

typedef struct TelRec44 {
    /* 0x00 */ u8 pad00[3];
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 pad04[0x14];
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 pad1E[0x12];
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ u8 pad34[0x10];
} TelRec44; /* size 0x44; view of D_8011ECD0 records, T-10040. */

/* Dialogue text pointers and XA tables, T-10040. */
extern u8 *D_801410BC[];
extern u32 D_801410D4[];
extern u32 D_80141104[];
extern u8 *D_801410C8;
extern u8 *D_801410CC;

/* Forward calls within 80132000.c, T-10040. */
void func_8013586C(void);
void func_80135A44(void);
void func_80135C4C(void);
void func_80135E28(void);

/* Sixteen day/month entries, 0x40-byte tables at +0x0564/+0x05A4. */
extern s32 D_80140564[16];
extern s32 D_801405A4[16];
s32 func_8013655C(void);

/* Dialogue text pointers and XA tables, T-10040. */
extern u32 D_8014109C[];
extern u32 D_801410AC[];
extern u8 *D_80141094;
extern u8 *D_80141098;

/* Dialogue text pointers and XA tables, T-10040. */
extern u32 D_8014104C[];
extern u32 D_80141064[];
extern u8 *D_80141040;
extern u8 *D_80141044;
extern u8 *D_80141048;

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144210[];
extern u32 D_80144240[];
extern u8 *D_801441F8;
extern u8 *D_801441FC;
extern u8 *D_80144200;
extern u8 *D_80144208;
extern u8 *D_8014420C;
extern u8 *D_80144204;
void func_801384F0(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144298[];
extern u32 D_801442E8[];
extern u8 *D_80144270;
extern u8 *D_80144274;
extern u8 *D_80144278;
extern u8 *D_80144280;
extern u8 *D_80144284;
extern u8 *D_80144288;
extern u8 *D_8014428C;
extern u8 *D_80144290;
extern u8 *D_80144294;
extern u8 *D_8014427C;
void func_8013883C(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144350[];
extern u32 D_80144380[];
extern u8 *D_80144338;
extern u8 *D_8014433C;
extern u8 *D_80144340;
extern u8 *D_80144348;
extern u8 *D_8014434C;
extern u8 *D_80144344;
void func_80138D20(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_801443C8[];
extern u32 D_801443F8[];
extern u8 *D_801443B0;
extern u8 *D_801443B4;
extern u8 *D_801443B8;
extern u8 *D_801443C0;
extern u8 *D_801443C4;
extern u8 *D_801443BC;
void func_80139064(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144440[];
extern u32 D_80144470[];
extern u8 *D_80144428;
extern u8 *D_8014442C;
extern u8 *D_80144430;
extern u8 *D_80144438;
extern u8 *D_8014443C;
extern u8 *D_80144434;
void func_801393A8(void);

/* T-10040: dialogue and XA data used by func_8013586C. */
extern u32 D_80141084[];
extern u8 *D_8014107C;
extern u8 *D_80141080;
void func_8013639C(void);

/* T-10040: dialogue tables for the record-counter state. */
extern u32 D_801441A8[];
extern u32 D_801441D0[];
extern u8 *D_80144194;
extern u8 *D_80144198;
extern u8 *D_8014419C;
extern u8 *D_801441A0;
extern u8 *D_801441A4;
void func_801380D8(void);
#endif
