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
#endif


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

#ifndef OVL_TT_W9_RENDER_TYPES
#define OVL_TT_W9_RENDER_TYPES
typedef union TtPositionWord {
    /* 0x00 */ s32 w;
    /* 0x00 */ struct { s16 lo; s16 hi; } h;
} TtPositionWord; /* size 0x04 */
typedef union TtHalfwordBytes {
    /* 0x00 */ u16 h;
    /* 0x00 */ struct { u8 lo; u8 hi; } b;
} TtHalfwordBytes; /* size 0x02; actor +0x40 has both lhu and lbu accesses. T-9710. */
typedef struct TtActor78 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 pad_01;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u8 pad_12[2];
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ u8 pad_18[8];
    /* 0x20 */ TtPositionWord unk_20;
    /* 0x24 */ TtPositionWord unk_24;
    /* 0x28 */ u8 pad_28[0x10];
    /* 0x38 */ u8 unk_38;
    /* 0x39 */ u8 unk_39;
    /* 0x3A */ u8 unk_3A;
    /* 0x3B */ u8 unk_3B;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ TtHalfwordBytes unk_40;
    /* 0x42 */ u16 unk_42;
    /* 0x44 */ u16 unk_44;
    /* 0x46 */ u8 unk_46;
    /* 0x47 */ u8 unk_47;
    /* 0x48 */ u8 pad_48[4];
    /* 0x4C */ u8 unk_4C;
    /* 0x4D */ u8 unk_4D;
    /* 0x4E */ u8 unk_4E;
    /* 0x4F */ u8 unk_4F;
    /* 0x50 */ u8 unk_50;
    /* 0x51 */ u8 unk_51;
    /* 0x52 */ u8 pad_52[6];
    /* 0x58 */ u16 unk_58;
    /* 0x5A */ u16 unk_5A;
    /* 0x5C */ u8 unk_5C;
    /* 0x5D */ u8 unk_5D;
    /* 0x5E */ u8 pad_5E[0x10];
    /* 0x6E */ s16 unk_6E;
    /* 0x70 */ u8 pad_70[8];
} TtActor78; /* size 0x78; allocator walks 64 slots in 0x1E00 bytes. T-9710. */

#endif

#ifndef W9_TT_func_8014B198
#define W9_TT_func_8014B198
void func_8014B198(u8 *arg0);
#endif

#ifndef W9_TT_func_8014B414
#define W9_TT_func_8014B414
void func_8014B414(u8 *arg0);
#endif

#ifndef W9_TT_func_8014B5A4
#define W9_TT_func_8014B5A4
void func_8014B5A4(u8 *arg0);
#endif

#ifndef W9_TT_func_8014BD68
#define W9_TT_func_8014BD68
void func_8014BD68(u8 *arg0);
#endif

#ifndef W9_TT_func_8014BF54
#define W9_TT_func_8014BF54
void func_8014BF54(void *arg0);
#endif

#ifndef W9_TT_func_8014C1BC
#define W9_TT_func_8014C1BC
void func_8014C1BC(u8 *arg0);
#endif

#ifndef W9_TT_func_8014C4AC
#define W9_TT_func_8014C4AC
void func_8014C4AC(u8 *arg0);
#endif

#ifndef W9_TT_func_8014C580
#define W9_TT_func_8014C580
void func_8014C580(u8 *arg0);
#endif
