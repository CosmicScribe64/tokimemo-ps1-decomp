#ifndef OVL_TACO_H
#define OVL_TACO_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_func_8009B430 /* implicit declaration: called with 3 and with 4 arguments (main_api.h: 4) */

#include "common.h"
#include "psx_types.h"
#include "game.h"

void func_8015C2CC(s32 arg0);
extern s16 D_801606AC[], D_801606C4[], D_80160858[], D_8016090C[], D_80160A60[];

/* state handlers of the loading screen driver func_80133870 (T-16562) */
s32 func_80133980(void);
void func_80133AF4(void);
void func_80133C08(void);
void func_80133E98(void);
s32 func_80134500(void);

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

/* Object handle (T-16560): GsDOBJ2 layout; unk0 attribute (0x80000000 hides it), unk4[0] the
 * TcObj50 coordinate object, unk4[1] the model, unk4[2] the id. func_8009B430 (GsLinkObject4) fills it. */
typedef struct Tc10 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4[3];
} Tc10; /* size 0x10 */

/*
 * Coordinate object (T-16560). Same layout as libgs GsCOORDINATE2: a flag word (0 marks the local
 * matrix as changed), the local matrix (rotation unk4, translation unk18..unk20), the world matrix
 * the library computes, and the param/super/sub links. func_80099E30 (GsInitCoordinate2) takes
 * a pointer to one. The translation stays three scalars because matched code reads them as such.
 */
typedef struct TcObj50 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4[3][3];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ MATRIX unk24;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ struct TcObj50 *unk48;
    /* 0x4C */ struct TcObj50 *unk4C;
} TcObj50; /* size 0x50 */

extern TcPos D_80128880[];
extern Tc10 D_80127080[];
extern TcObj50 D_80127480[];

/*
 * Actor record, 79 of them at D_8015EDB4 (loops stop at byte offset 0x29F8). T-16560: unk0[1] holds
 * flag bits (0x80, 0x40); +0x04 is the actor's object handle and +0x14 its coordinate object
 * (func_8013801C passes &actor->unk14 to func_80099E30). unk74..unk7A and unk7C..unk82 are copied
 * as two eight-byte TcPos values (lwl/lwr pairs) into by-value TcPos arguments.
 */
typedef struct TcActor {
    /* 0x00 */ u8 unk0[2];
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ Tc10 unk4;
    /* 0x14 */ TcObj50 unk14;
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
s32 func_801407D4(s32 *arg0, TcPos arg1);
void func_80140E18(s32 arg0);
void func_8015ABF0(void);
void func_801563E4(s32 arg0);
s32 func_80156178(void);
void func_80154F74(void);
void func_801553AC(void);
void func_8015595C(void);
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
s32 func_8014A480(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5);
void func_8014F820(void);
void func_8013AFDC(void);
void func_8013A790(s32 arg0, s32 arg1);
s32 func_8013760C();
void func_8013788C();
void func_8014F210(void);
void func_8015185C(void);
s32 func_801335A0();
s32 func_80134A18();
void func_80134EBC(void);
void func_8013506C();
void func_80135220(void);
void func_80135278(void);
void func_801352CC(void);
void func_80135330(void);
void func_80135350(void);
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
void func_8015163C(s32 arg0, s32 arg1);
void func_80151264(u32 arg0);
void func_801517AC(s32 arg0);
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
#ifndef TACO_OVERRIDE_func_801471D0 /* 80146D60.c defines it with a pointer parameter (T-19015) */
void func_801471D0(s32 arg0);
#endif
void func_80147488(s32 arg0, s16 *arg1, s32 arg2);
extern s32 D_8015F3E0[2]; /* two model slots, T-19011 */
extern s16 *D_8015F82C[2]; /* model data pointers for func_80147488, T-19002 */
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
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} Tc14; /* size 0x14; same record as TacoParticle14 (two TcPos, four bytes), T-16560 */
void func_80151F94(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
void func_801522B0(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3);
void func_80152C60(s32 arg0);
void func_80153068(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_8015F4F0;
extern s32 D_80160460;
extern s32 D_80160464;
extern s32 D_80160470;
extern u16 *D_801FAA84;
extern s32 D_8015F490[5];
extern s32 D_8015F4A4;
extern s32 D_8015F4A8;
void func_80153AA0(s32 arg0, s32 arg1, s32 arg2);
void func_80153E78(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
extern Tc14 D_8015E270[];
/* result screen (T-19006): counter and bonus drawers of 8013B990.c */
s32 func_8013C1F4(void);
s32 func_8013C440(void);
void func_8013BAC4(Tc14 *arg0, s32 arg1, s32 arg2);
extern Tc14 D_8015E3B0[];
extern u16 D_8015F580;
extern u16 D_8015F584;
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
s32 func_801472D4(); /* Returns -1 for unaligned input buffers; T-9610. */
void func_8014B2A4(s32 arg0);
void func_8014B8B4(s32 arg0, s32 arg1, s32 arg2);
void func_8014B790(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, u32 arg4);
void func_8014DA6C(s32 arg0);
s32 func_8014E5F8(s32 arg0);
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
void func_801347F4(s32 *arg0);
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
s32 func_801357B0();
extern s8 D_8015EDB8;

void func_8013873C(void);
void func_801387C0(void);
void func_80138840(void);
extern Tc14 D_8015E4F0[];
extern Tc14 D_8015E630[];
extern Tc14 D_8015E770[];

void func_801493D4(void);
void func_80149D1C(void);
void func_80151C30();

void func_801478F8();
void func_801521EC();
void func_80147400();
extern s32 D_8015FC94;
extern s32 D_8015FC98[];
typedef struct TcPair8 {
    s32 a;
    s32 b;
} TcPair8;
extern TcPair8 D_8015FAE0[];
extern s32 D_8015E238[]; /* word pairs, read as a flat table by func_80133738 (T-16561) */
typedef struct TcPair2 {
    u8 a;
    u8 b;
} TcPair2;
extern TcPair2 D_8015FBF8[];
extern TcPair8 D_80160290[]; /* read at i - 1, T-16563 */
extern TcPair2 D_801603A8[]; /* read at i - 1, T-16563 */
extern TcPair8 D_8015F840[]; /* read at i - 1, T-19013 */
extern TcPair2 D_8015F930[]; /* read at i - 1, T-19013 */
/* FAKE: not data of its own. D_801603D4 + 4 * i is D_80160444 + 4 * (i - 0x1C); IDO keeps the symbol as the
 * induction base, so only this base gives func_80152808 its zero-offset pointer walk (D_80160448). T-16563 */
extern s16 *D_801603D4[];
void func_801490B4(void);

void func_80149B7C(void);

/* T-9520: globals used by the matched wave-8 functions. */
extern u8 D_8015EDDC;
extern u8 D_8015EDE0;
extern u8 D_8015EDE4;

void func_8013F430(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8015A164(s32 arg0);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80134450(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8014488C(void);
void func_80144998(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80136C00(void);
void func_80136D00(void);
void func_80136D78(void);
void func_80136F4C(void);
void func_80136E08(void);
s32 func_80136C60(void);
void func_80136CD8(void);

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
void func_80148184(void);
void func_80148308(void);
void func_801484E4(void);
void func_8014866C(void);
void func_801487A4(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4);
void func_8014AB50();
void func_80148B58();
extern u8 D_8015F404[];
extern u8 D_8015F410[];
extern u8 D_8015F41C[];
void func_80137264();
void func_801371B4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_80152110(s32 arg0, s32 arg1, s32 arg2);
void func_801531E8();
extern u8 D_8015E22C[];
void func_8014A070(void);
void func_8014A1D4(void);
void func_8014A234(void);
void func_8014A390(void);
void func_8014A42C(void);
extern s32 D_80160138[];
extern u8 D_80160216[];
extern s32 D_801601D0;
extern s32 D_801601D4;
extern u8 D_8016023C;
extern u8 D_8016023D;
extern s32 D_8015FEF8[];
extern u8 D_8015FFE2[];
extern s32 D_80160108[];
extern s32 D_80160168[];
extern s32 D_801601E0[];
extern u8 D_8016020A[];
extern u8 D_80160222[];
extern u8 D_80160240[];
extern s16 *D_80160268[];
extern s16 *D_80160280;
void func_80150F44(s32 arg0);
void func_8014C578(s32 arg0);
void func_80150B3C(s32 arg0);
extern s32 D_8015FD18[];
extern u8 D_8015FE1A[];
void func_80151678(void);
extern s32 D_801601C8;
extern s32 D_801601CC;
extern u8 D_8016023A;
extern u8 D_8016023B;
void func_8014F9B0(void);
void func_8014FDC0(void);
void func_801508D0(void);
void func_80150DE8(void);
void func_8014D530(s32 arg0, s32 arg1);
void func_80153774(s32 arg0);
void func_8015AB18(s32 arg0);
void func_8015AB80(s32 arg0);

#ifndef OVL_TACO_MATCHED_VIEWS_T9520
#define OVL_TACO_MATCHED_VIEWS_T9520
/* Coordinate object: MATRIX payload at +4, total stride 0x50. T-9610. */
typedef struct TcTransformObj50 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ MATRIX transform;
    /* 0x24 */ u8 pad24[0x2C];
} TcTransformObj50; /* size 0x50, full word view of TcObj50 */
void func_8013B2A0(void);
void func_8013B398(void);
void func_8013B3C4(void);
#endif

/* Position and velocity copies/loads use two existing eight-byte TcPos records. */
typedef struct TacoParticle14 {
    /* 0x00 */ TcPos unk_00;
    /* 0x08 */ TcPos unk_08;
    /* 0x10 */ u8 unk_10;
    /* 0x11 */ u8 unk_11;
    /* 0x12 */ u8 unk_12;
    /* 0x13 */ u8 unk_13;
} TacoParticle14; /* size 0x14 */
extern TacoParticle14 D_8015E9F0[];

extern TacoParticle14 D_8015E8B0[];

#ifndef OVL_TACO_INITIALIZATION_T9780
#define OVL_TACO_INITIALIZATION_T9780
void func_8013801C(void);
void func_801571E8(void);
void func_801572D8(void);
void func_801573C8(void);
void func_80157AE0(void);
void func_8015800C(void);
void func_80158AB0(void);
void func_801589B0(void);
#endif

#ifndef OVL_TACO_DIRECTIONS_T9780
#define OVL_TACO_DIRECTIONS_T9780
typedef struct TcDirection {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
} TcDirection; /* size 0x08 */
typedef struct TcDirections28 {
    /* 0x00 */ TcDirection pos[5];
} TcDirections28; /* size 0x28 */
typedef struct TcRotations14 {
    /* 0x00 */ s32 rotation[5];
} TcRotations14; /* size 0x14 */
extern TcDirections28 D_80160480;
extern TcRotations14 D_801604A8;
#endif

#ifndef OVL_TACO_ACTOR_INIT_T10020
#define OVL_TACO_ACTOR_INIT_T10020
extern u8 D_8015FA44[];
extern u8 D_8015FA50[];
#endif

#ifndef OVL_TACO_MENU_RESOURCE_T10020
#define OVL_TACO_MENU_RESOURCE_T10020
extern s32 D_801603A0;
extern s32 D_801603A4;
extern u8 D_801603EC;
extern u8 D_801603ED;
void func_8015266C(u32 arg0);
void func_80152764(s32 arg0);
void func_80152944(s32 arg0, s32 arg1);
void func_80152DB4(s32 arg0, s32 arg1, s32 arg2);
void func_801523D0(s32 arg0, s32 arg1, s32 arg2);
extern s16 *D_80160444[]; /* six pointers: [0] 0x23's model, [1..4] the walk of func_80152944; T-16563 */
#endif

#ifndef OVL_TACO_T10010_SELECTION
#define OVL_TACO_T10010_SELECTION
void func_8013546C(void);
#endif

#ifndef OVL_TACO_T10010_SCORE_TABLE
#define OVL_TACO_T10010_SCORE_TABLE
extern s32 D_8015F500; /* T-19006 */
extern s32 D_8015F504[]; /* Word score table indexed modulo 17. T-10010. */

extern Tc14 D_8015EB30[16];
extern Tc14 D_8015EC70[16]; /* the table after D_8015EB30; also its end address (T-16560) */
void func_8013DEE4();
extern u8 D_8015FA80[];
void func_801483B0(void);
void func_80148D6C(void);
void func_8013E778();
void func_8013EAA4();
void func_801428DC(void);
u8 func_80142FE8(void);
void func_80142AF0(void);
void func_80142EC4(void);
void func_80142EE4(void);
void func_80142F0C(void);
u8 func_80142F34(void);
void func_80142DE0(void);
void func_80142820(void);

#endif

/* Font UV pairs (u, v), T-16562: digits 0-9, letters A-Z, then '.', '-', ' ' and a fallback glyph. */
extern u8 D_8015F5A0[20];
extern u8 D_8015F5B4[52];
extern u8 D_8015F5E8, D_8015F5E9, D_8015F5EA, D_8015F5EB, D_8015F5EC, D_8015F5ED, D_8015F5EE, D_8015F5EF;

extern u8 D_80161ECD;
extern s32 D_8016024C[];
extern s32 D_80160264;
extern u8 D_80161E20[]; /* trigger count per kind (T-10810) */
extern s32 *D_80161E60[]; /* per kind: table of (s32, s32) pairs (T-10810) */
