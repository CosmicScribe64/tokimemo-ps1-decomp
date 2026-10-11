#ifndef OVL_TT_H
#define OVL_TT_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_func_800462C8 /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */
#define MAIN_API_OVERRIDE_func_800AD950 /* implicit int declaration: a void prototype changes the register allocation of func_80133A60 */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"
extern u8 D_80156784;
extern u8 D_80156780;
extern u8 *D_80158A74;
void func_80138300(void);
typedef struct TtRec2 {
    /* 0x0 */ u8 flag;
    /* 0x1 */ u8 pad;
    /* 0x2 */ s16 val;
} TtRec2; /* size 0x4 */
extern u8 *D_80158A6C;
extern u8 *D_80158AA4;
extern u8 *D_80158AA8;
extern u8 *D_80158A8C;
extern u8 *D_80158A84;
extern u8 *D_80158A78;
extern u8 *D_80158A94;
extern u8 *D_80158A98;
void func_8014D260(s32 arg0, s32 arg1);
extern u8 *D_80158A60;
extern u8 *D_80158AB4;
void func_8013E320(void);
extern s32 D_8014E34C;

extern u8 *D_80158A64;
extern s32 *D_80158A70;
void func_8013BE9C(u8 *arg0);
extern u8 *D_8015694C[];
void func_80142AF0(void *arg0, s32 arg1);
extern u8 D_80152C54[];
extern u8 D_80155C58[];
extern u8 D_80155E2C[];
extern u8 D_80155D2C[];
void func_8013B6C8(void);
void func_8013B928(void);
extern u8 D_80155A8C[];
extern s16 D_80155A84[];
extern u8 D_80155A80[];
extern s16 D_80155A78[];
extern u8 *D_801569E8;
extern u8 D_80155A34[];
extern u8 D_80155A4C[];
extern s16 D_80155A64[];
extern s16 D_80155A6C[];
extern u8 D_80155A74[];
extern u8 D_801559F0[];
extern u8 D_80155A08[];
extern s16 D_80155A20[];
extern s16 D_80155A28[];
extern u8 D_80155A30[];
extern u8 *D_80158AAC;
extern u8 *D_80158A68;
extern u8 *D_80158AB0;
extern u8 D_80152CA4[];
extern u8 D_80152CB0[];
extern u8 D_80152CC4[];
extern u8 D_80151A50[];
extern u8 D_80150978[];
extern u8 D_80155B54[];
void func_8013AE3C(void);
extern u8 *D_80158A90;
extern u8 D_80152CD0[];
extern u8 D_80152CE4[];

extern u8 *D_80158A9C;

extern u8 *D_80158A80;
extern u8 *D_80158A88;
extern u8 *D_80158A7C;
extern u8 *D_80158AA0;

extern u8 D_80155C78[];
void func_801487D0(s16 arg0, s16 arg1, u8 arg2);
void func_80148974();

extern u8 D_80155C7E[];
extern u8 D_80155C84[];
extern s16 D_8014ED50;
void func_80148DB0(void);

void func_8013A3E0(void *arg0, u8 *arg1, s32 arg2);
extern u8 D_80155D02;

void func_80143580(void);
typedef struct TtOtTag {
    /* 0x00 */ u32 addr : 24;
    /* 0x03 */ u32 len : 8;
} TtOtTag; /* size 0x04 */
void func_8014AFB8(u8 *arg0, u16 arg1, s32 arg2);
#endif /* OVL_TT_H */
void func_8013EC1C(void);
void func_801478D0();

#ifndef OVL_TT_W8_3_TYPES_H
#define OVL_TT_W8_3_TYPES_H
/* State-machine records at D_80158A80, walked by func_8014AB48. T-9530. */
typedef struct TtRecord54 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 pad_01[3];
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 pad_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 pad_10[0x18];
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ u8 pad_30[0x12];
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
    /* 0x44 */ u16 unk_44;
    /* 0x46 */ u8 pad_46[0xE];
} TtRecord54; /* size 0x54 */
#endif /* OVL_TT_W8_3_TYPES_H */

#ifndef OVL_TT_MATCHED_VIEWS_T9520
#define OVL_TT_MATCHED_VIEWS_T9520
typedef struct TtState58 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 pad10[0x48];
} TtState58; /* size 0x58; D_80158A84 walks eight records, T-9520 */
typedef struct TtSceneView {
    /* 0x000 */ u8 pad0[6];
    /* 0x006 */ u16 unk_06;
    /* 0x008 */ u16 unk_08;
    /* 0x00A */ u8 padA[0xA];
    /* 0x014 */ s16 unk_14;
    /* 0x016 */ s16 unk_16;
    /* 0x018 */ u8 pad18[0x34E];
    /* 0x366 */ u16 unk_366;
    /* 0x368 */ u8 pad368[0x3C];
    /* 0x3A4 */ u16 unk_3A4;
    /* 0x3A6 */ s16 unk_3A6;
} TtSceneView; /* minimum accessed extent 0x3A8; T-9520 */
typedef struct TtPoint14 {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
} TtPoint14; /* size 0x14; func_801320F0 reads these coordinates */
#ifndef OVL_TT_T9510_ANIM_TYPES
#define OVL_TT_T9510_ANIM_TYPES
/* T-9510: D_80151A50 is a table of pointers to frame-array pointers. */
typedef struct TtAnimFrame {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
} TtAnimFrame; /* size 0x04 */

/* The animation fields used by func_801478D0; remaining extent unknown. */
typedef struct TtAnimState {
    /* 0x00 */ u8 pad0[5];
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 unk_09;
} TtAnimState; /* accessed prefix size 0x0A */
#endif
