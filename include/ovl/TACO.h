#ifndef OVL_TACO_H
#define OVL_TACO_H

#include "common.h"
#include "game.h"

/* Overlay-local externs (T-1300). */
void func_80059E00();
void func_8013F468(void);
void func_8013F7D8(void);
void func_8013F9C0(void);
void func_8013FBF0(void);
void func_8004ADE4(void);

extern s32 D_801604C0;
extern s32 D_801604C4;
extern s32 D_801604C8;
extern s32 D_801604CC;
extern s32 D_801604D0;
extern s32 D_8015F3F0;
extern s32 D_8015F3F4;
extern s32 D_8015F3F8;
extern s32 D_8015F3FC;
extern s32 D_8015F4DC;
extern s32 D_8015F4E0;
extern s32 D_8015F4E4;
extern s32 D_8015F4E8;
extern s32 D_8015F4F8;

/* TACO tables; field names after the first user, no semantics known. */
typedef struct TcPos {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 unk6;
} TcPos; /* size 0x08 */

typedef struct Tc10 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4[3];
} Tc10; /* size 0x10 */

typedef struct TcObj50 {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x2C];
} TcObj50; /* size 0x50 */

typedef struct TcEnt1C {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x10];
} TcEnt1C; /* size 0x1C */

extern TcPos D_80128880[];
extern Tc10 D_80127080[];
extern TcObj50 D_80127480[];
extern s32 D_8015F294[];

typedef struct TcActor {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ u8 pad4[0x60];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ s16 unk6C;
    /* 0x6E */ s16 unk6E;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ s16 unk7C;
    /* 0x7E */ s16 unk7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ u8 unk84[4];
} TcActor; /* size 0x88 */

typedef struct TcSlot {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[0xB];
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} TcSlot; /* size 0x14 */

extern TcActor *D_8015EDB4;
extern s16 D_8015F5F0;
extern s16 D_8015F5F4;
extern s16 D_8015F5F6;
extern void func_8013D21C(void);

typedef struct TcPack {
    /* 0x00 */ u8 b[0x10];
} TcPack; /* size 0x10 */

typedef struct TcSlotB {
    /* 0x00 */ TcPack pack;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} TcSlotB; /* size 0x14 */

extern TcSlotB D_8015E9F0[];
extern s16 D_8015E9F6;

typedef struct TcPair {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} TcPair; /* size 0x08 */

void func_8004ACC8(s32 arg0);
void func_80059688(s32 arg0);
void func_80140028(s32 arg0);
void func_80140E18(s32 arg0);
s32 func_80044C98(void);
void func_800438DC(s32 arg0, s32 arg1);
void func_80043914(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8015ABF0(void);
void func_800450F4(s32 arg0, s32 arg1);
void func_80137CBC(s32 arg0);
extern s32 D_800E7CEC;
extern Tc10 D_80127090;
extern Tc10 D_801270A0;
extern s32 D_8015EDB0;
extern u8 D_8015EDBC;
extern u8 D_8015EDC0;
extern TcPair D_8015F440[];

#endif
