#ifndef OVL_DATE2_H
#define OVL_DATE2_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_800B5938 /* matched as u8 (main_api.h: u8[]) */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"
#include "game.h"

extern u8 D_800B5938;
void func_80138048(void);

extern s16 D_8013B5D0;
extern s16 D_8013A428;
extern u8 D_8013A4C0;
extern s16 D_8013A42C;
extern u8 D_8013A4C4;
extern s32 D_8013A80C, D_8013A810, D_8013A814;
extern s32 D_8013B5EC, D_8013B5F0, D_8013B5F4;
extern s16 D_8013B5F8;
extern s16 D_8013B5FC;
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
extern s16 D_8013A430, D_8013A434, D_8013A438, D_8013A43C;
extern s32 D_8013A774, D_8013A7A8, D_8013A7DC;
extern s32 D_8013A5B0, D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8;
extern s32 D_8013A404, D_8013A408, D_8013A40C;

void func_801370E4(void);
void func_801365DC(s32 arg0);

s32 func_801348F0(); /* returns 0 on most paths (or v0,zero,zero) T-11880 */
void func_80133AF0();
s32 func_801350A4(); /* returns 0 on the header-error path only T-11880 */
s32 func_80135E50();
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
void func_801385CC();
void func_801388C0();
void func_80138BEC(void);
s32 func_80138A3C(void);
void func_80136660(void);
void func_8013856C(void);
void func_8013850C(void);

extern u8 D_8013B5C4;
extern s16 D_8013B5C8;
void func_80138E0C(void);
void func_80138F8C(void);
void func_801391A0(void);
void func_801393B4(void);
/* Copied by DATE2 threshold walkers; lengths follow their signed halfword loops. */
typedef struct Date2Threshold30 { /* size 0x3C */
    /* 0x00 */ s16 values[30];
} Date2Threshold30;
extern Date2Threshold30 D_8013B6B8;
void func_801395C8();

/* func_80138F8C copies 0x66 bytes and scans 51 signed halfwords. */
typedef struct Date2Threshold51 { /* size 0x66 */
    /* 0x00 */ s16 values[51];
} Date2Threshold51;
extern Date2Threshold51 D_8013B6F4;

/* func_801391A0: local copy is 0x4e bytes, scanned as 39 halfwords. */
typedef struct Date2Threshold39 { /* size 0x4e */
    /* 0x00 */ s16 values[39];
} Date2Threshold39;
extern Date2Threshold39 D_8013B75C;

/* func_801393B4: local copy is 0xa2 bytes, scanned as 81 halfwords. */
typedef struct Date2Threshold81 { /* size 0xa2 */
    /* 0x00 */ s16 values[81];
} Date2Threshold81;
extern Date2Threshold81 D_8013B7AC;

/* func_80138A90 copies four pairs of word-sized load parameters plus two fallback pairs. */
typedef struct Date2LoadPair { /* size 0x08 */
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} Date2LoadPair;
typedef struct Date2LoadPairs4 { /* size 0x20 */
    /* 0x00 */ Date2LoadPair pair[4];
} Date2LoadPairs4;
extern Date2LoadPairs4 D_8013B688;
extern Date2LoadPair D_8013B6A8, D_8013B6B0;
extern u8 D_8013B5CC;

/* T-18800: func_80135E50's three moving dots, as in its DATE twin func_801489D0 (D_8015CFA4..):
 * x and y positions and their steps, three s16 each at a stride of 8 bytes. D_8013A5CC is the
 * step counter (sh/lhu), D_8013A5D0 the scene timer of func_80133AF0 (sh/lh). */
extern u16 D_8013A5CC;
extern s16 D_8013A5D0;
extern s16 D_8013A5D4[3];
extern s16 D_8013A5DC[3];
extern s16 D_8013A5E4[3];
extern s16 D_8013A5EC[3];

/* T-18800: the four menu labels of func_801385CC and func_801388C0: their widths (passed to
 * menu_set as one s16[4], tested in a loop) and the 21-byte strings copied to the stack whole
 * (0x54 bytes, the label at 0x50 + 21 * i). */
extern s16 D_8013B5E4[4];
typedef struct Date2Labels4x21 { /* size 0x54 */
    /* 0x00 */ u8 v[4][21];
} Date2Labels4x21;
extern Date2Labels4x21 D_8013B634;

/* T-11880: func_801348F0 (the DATE movie view, same guard and layout as include/ovl/DATE.h). */
#ifndef OVL_DATE_MOVIE_T10830_H
#define OVL_DATE_MOVIE_T10830_H
/* Overlay access view at D_800B5920 (see include/ovl/DATE.h). */
typedef struct DateMovieWork {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 pad0C[12];
    /* 0x18 */ u8 unk_18;
    /* 0x19 */ u8 unk_19;
    /* 0x1A */ u8 unk_1A;
    /* 0x1B */ u8 unk_1B;
    /* 0x1C */ u8 unk_1C;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ u8 unk_20;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
} DateMovieWork; /* size 0x2C, minimum observed extent */

/* First two words of the bitstream reached through D_80125C04. */
typedef struct DateMovieHeader8 {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
} DateMovieHeader8; /* size 0x08, minimum observed extent */
#endif
void func_80132BB0(void);

#endif /* OVL_DATE2_H */
