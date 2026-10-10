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

extern TcPos D_80128880[];
extern Tc10 D_80127080[];
extern TcObj50 D_80127480[];

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

extern TcActor *D_8015EDB4;

void func_8004ACC8(s32 arg0);
void func_80059688(s32 arg0);
void func_80140028(s32 arg0);
void func_80140E18(s32 arg0);
s32 func_80044C98(void);
void draw2d3d(s32 arg0, s32 arg1);
void load_palette(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8015ABF0(void);
void func_800450F4(s32 arg0, s32 arg1);
void func_80137CBC(s32 arg0);
extern s32 D_800E7CEC;
extern Tc10 D_80127090;
extern Tc10 D_801270A0;
extern s32 D_8015EDB0;
extern u8 D_8015EDBC;
extern u8 D_8015EDC0;

void func_80135F6C(void);
void func_80136988(void);
void func_80135A04(void);
s32 func_8013515C(void);
void func_8009C210(s32 arg0);
void func_800591D8(s32 arg0);
void func_80134FB8(void);
void func_8014A480(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5);
void func_8014F820(void);
void func_8013AFDC(void);
void func_8013A790(s32 arg0, s32 arg1);
void func_8014F210(void);
void func_8015185C(void);
void func_801335A0(s32 arg0, s32 arg1);
extern u8 D_8011ED1B;
extern u8 D_8015FE95;
extern u8 D_8015FEA0[];
extern u8 D_8015FEAC[];
extern u8 D_8015FEC4[];
extern s32 D_8015EDF0;
extern s32 D_8015EE00;

void func_80136FBC(s32 arg0, s32 arg1);
void func_80154960(void);
void func_80156A80(void);
void func_80159090(void);
void func_8015C208(void);
void func_80139FB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8013A580(s32 arg0);
void func_80132220(void);
void func_801334BC(void);
extern s32 D_8015F400;
extern u8 D_8015EDC4;

void func_8014394C(void);
void func_8013F874(s32 arg0, s32 arg1, s32 arg2);
void func_80143F40(void);
void func_80143B58(void);
void func_80133694(void);
void func_80144094(void);
void func_80144430(void);
void func_801446D0(void);
void func_80143FF4(void);
extern u8 D_8015EDCC;
extern s16 D_8015EDEC;

extern u8 *D_800E7CBC;
extern u8 *D_800E7CCC;
extern u8 *D_800E7CDC;
extern u8 D_8011ED57;
void func_80144E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80136024(void);
void func_801361E8(void);
void func_80136438(void);
void func_80136688(void);
void func_8015131C(s32 arg0);

void func_80146F74(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_8014FBD4(s32 arg0);
void func_80151264(u32 arg0);
void func_8014A79C(void);
s32 func_800AE0C0(s32 arg0);
void load_csr_tp(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_8014D2E0(void);
s32 func_80147B24(s32 arg0, void *arg1);
void func_8014B374(s32 arg0);
void func_8014B4B4(s32 arg0);
void func_8014EB24(s32 arg0);
void func_8014EF14(s32 arg0);
void func_8014F044(void);
void initView(void);
void initModelingData_init(s32 arg0);
void func_80146D60(s32 arg0);
void func_801471D0(s32 arg0);
void func_80147488(s32 arg0, s32 arg1, s32 arg2);
extern s32 D_8012749C;
extern s32 D_8015F3E0;
extern s32 D_8015F3E4;
extern s16 D_8015F3E8;

extern s32 D_801274A0;

void func_8009B310(s32 arg0, s32 arg1, s32 arg2);

void func_80144CC0(u8 *arg0);
void func_80146E3C(void);
void func_80144D90(s32 arg0);
void func_8014C3B4(s32 arg0);
void func_8014C8DC(void);

void func_8014C978(s32 arg0);
void func_8014D200(void);

/* T-1321 */
void func_8014DF50(s32 arg0);
typedef struct Tc14 {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 pad6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 padE[4];
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} Tc14; /* size 0x14 */
void func_80151F94(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
void func_801522B0(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3);
void func_80152C60(s32 arg0);
void func_80153068(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_8015F4F0;
extern s32 D_80160460;
extern s32 D_80160464;
void func_80153AA0(s32 arg0, s32 arg1, s32 arg2);
void func_80153E78(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
extern Tc14 D_8015E270[];
extern Tc14 D_8015E3B0[];
void func_801413B8(Tc14 *arg0, s32 arg1);
void func_801416E8(Tc14 *arg0, s32 arg1);
void func_80141B18(Tc14 *arg0, s32 arg1);
extern u8 D_8015FA68[];
s32 func_800AE0D0(void);
void func_80147C98(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);

#endif
