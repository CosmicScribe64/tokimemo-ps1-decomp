#ifndef OVL_TACO_H
#define OVL_TACO_H

#include "common.h"
#include "game.h"

void func_8013F468(void);
void func_8013F7D8(void);
void func_8013F9C0(void);
void func_8013FBF0(void);

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

void func_80140028(s32 arg0);
void func_80140E18(s32 arg0);
void func_8015ABF0(void);
void func_80137CBC(s32 arg0);
extern Tc10 D_80127090;
extern Tc10 D_801270A0;
extern s32 D_8015EDB0;
extern u8 D_8015EDBC;
extern u8 D_8015EDC0;

void func_80135F6C(void);
void func_80136988(void);
void func_80135A04(void);
s32 func_8013515C(void);
void func_80134FB8(void);
void func_8014A480(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5);
void func_8014F820(void);
void func_8013AFDC(void);
void func_8013A790(s32 arg0, s32 arg1);
void func_8013760C();
void func_8013788C();
void func_8014F210(void);
void func_8015185C(void);
void func_801335A0(s32 arg0, s32 arg1);
/* func_8014927C: a 12-byte id table and three tables passed by address */
extern u8 D_8015FCA8[];
extern u8 D_8015FCB4[];
extern u8 D_8015FCC0[];
extern u8 D_8015FCD8[];
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
void func_8015C208();
void func_80139FB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8013A580(s32 arg0);
void func_80132220(void);
void func_801334BC(void);
extern s32 D_8015F400;
extern u8 D_8015EDC4;

void func_8014394C(void);
void func_8013F874(s32 arg0, s32 arg1, s32 arg2);
s32 func_80143F40(void);
void func_80143B58(void);
void func_80133694(void);
void func_80144094(void);
void func_80144430(void);
void func_801446D0(void);
void func_80143FF4(void);
extern u8 D_8015EDCC;
extern s16 D_8015EDEC;

void func_80144E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80136024(void);
void func_801361E8(void);
void func_80136438(void);
void func_80136688(void);
void func_8015131C(s32 arg0);

void func_80146F74(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_8014FBD4(s32 arg0);
void func_80151264(u32 arg0);
void func_8014927C(void);
void func_8014A79C(void);
void func_8014D2E0(void);
s32 func_80147B24(s32 arg0, void *arg1);
void func_8014B374(s32 arg0);
void func_8014B4B4(s32 arg0);
void func_8014EB24(s32 arg0);
void func_8014EF14(s32 arg0);
void func_8014F044(void);
void func_80146D60(s32 arg0);
void func_801471D0(s32 arg0);
void func_80147488(s32 arg0, s32 arg1, s32 arg2);
extern s32 D_8015F3E0;
extern s32 D_8015F3E4;
extern s32 D_8015F82C;
extern s32 D_8015F830;
extern s16 D_8015F3E8;

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
void func_80147C98(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);
extern s32 D_8015F4F4;
void func_80143B34();
extern u8 D_80161D9C[];
void func_8015C0A0();

void func_80143730();

extern u8 D_8015E220;

extern u8 D_8015E221;

extern u8 D_8015E222[];

extern s32 D_8015F290[];

extern s16 *D_80160034[];
extern s16 *D_801603F0[];
void func_8014BED8(void);
void func_8014BCEC(s32 arg0);
extern u8 D_8015FE94[];
void func_8014F5F0();
void func_80147674(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8014BE14(s32 arg0);
void func_80147928(s32 arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 arg5);
void func_801511C0(s32 arg0, s32 arg1);
extern s16 *D_801600A8;
extern s16 *D_801600AC[];
extern s16 *D_801600B4;
extern s16 *D_801600B8[];
extern s16 *D_80160040[];
extern s16 *D_80160054[];
extern s16 *D_80160078[];
extern s16 *D_801600A0[];
void func_8014C2E4(void);
extern s32 D_8015FEF0[];
extern u8 D_8015FFE0[];
void func_801472D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8014B2A4(s32 arg0);
void func_8014B8B4(s32 arg0, s32 arg1, s32 arg2);
void func_8014B790(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, u32 arg4);
void func_8014DA6C(s32 arg0);
s32 func_8014E1A0(s32 arg0);
void func_8014E048(void);
extern s16 *D_801600C0[];
extern s16 *D_801600D4[];
extern s16 *D_801600E0[];
extern s16 *D_801600E8;
extern s16 *D_801600EC[];
void func_8014C0D4(s32 arg0);
void func_8014C1DC(s32 arg0);
void func_8014A8E0(void);
void func_8014F2A0(void);
void func_8013920C();
void func_80139424();
void func_80139AB0();
void func_80139BCC();
void func_80139B54();
void func_80139394();
void func_801393F4();
void func_8014D3B4(s32 arg0);
void func_8014D918();
void func_8014DC94();
void func_8014E4A4();
void func_8014EA4C();
void func_8014EC4C();
void func_8014EE3C();
void func_80135418(void);
void func_80138160(void);
void func_801438F0();
extern s8 D_8015EDD0;
extern s32 D_8015EDD4;
void func_801532C4();
void func_801536BC();

void func_801534E0(s32 arg0);

void func_8014394C();
void func_801345D4();
void func_8013474C();
void func_801347F4(s32 arg0);
extern s32 D_8015F440[];
s32 func_80137BD8(void);
void func_80137D00(void);
void func_80137E48(void);
void func_80137F54(void);
s32 func_8013587C(void);
void func_801359B4(void);
void func_80135994(void);
s32 func_80135744(void);

void func_80138520(void);
void func_801385B4(void);
void func_80138634(void);
s32 func_8015ACCC(s32 a, s32 b, TcPos c, TcPos d, TcPos e, s32 f, s32 g);
extern TcPos D_801604D4;
extern TcPos D_801604E4;

extern TcPos D_801604DC;

void func_80138348(void);
void func_8013838C(void);
void func_8013840C(void);
extern s32 D_8016001C[];

void func_8014EDCC(s32 arg0);
void func_8014B5F0(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80135944(void);
extern s32 D_8015EDF8;
extern s32 D_8015EDFC;
extern u8 D_8015EE04[];
extern u8 D_8015EE08[];
extern u8 D_8015EE0C[];
extern s32 D_8015F4D8;
void func_8013A764(void);
typedef struct TacoRnd {
    s32 a;
    s32 b;
} TacoRnd;
extern TacoRnd *D_8015F4EC;
extern u8 D_8015EDD8;
void func_801357B0();
extern s8 D_8015EDB8;

void func_8013873C(void);
void func_801387C0(void);
void func_80138840(void);
#endif

typedef struct TacoCam {
    /* 0x0 */ s16 a;
    /* 0x2 */ s16 b;
    /* 0x4 */ s16 c;
    /* 0x6 */ s16 d;
} TacoCam; /* size 0x8 */
extern TacoCam D_8015F5F0;
void func_80139118(void);
void func_80158CDC(s16 arg0, s16 arg1, s16 arg2);
void func_80158DBC(s16 arg0, s16 arg1, s16 arg2);
extern s8 D_8015EDE8;
