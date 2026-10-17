#ifndef OVL_EN_NICHI_H
#define OVL_EN_NICHI_H

#include "common.h"
#include "psx_types.h"
#include "main_api.h"
#include "game.h" /* DisplayActor44 view of D_8011ECD0 (T-18200) */

typedef struct EnObj {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ Fix32 unk_14; /* 16.16 x: words in func_801339D4, the integer half .h[1] elsewhere (T-20920) */
    /* 0x18 */ Fix32 unk_18; /* 16.16 y */
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20; /* 16.16 step of the motion of func_801339D4 (T-20920) */
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
} EnObj; /* size 0x30, D_8013984C[10] */

/* The ten objects; splat labels D_80139862..D_801398AC are fields of records 0..2 (T-18200). */
extern EnObj D_8013984C[10];

/* EN_NICHI data (overlay rodata/data) */
extern s32 D_80139B1C;
extern s32 D_80139B14;
extern s32 D_80139210;
/* T-18200: asset addresses read by func_80132450 and func_80132834; D_80139204 points at three tpage addresses. */
extern s32 D_80139200;
extern s32 *D_80139204;
extern s32 D_80139208;
extern s32 D_8013920C;
extern s32 D_80139214;
extern s32 D_80139218;

/* EN_NICHI functions */
void func_801320C0(void);
void func_80135FBC(s32 arg0, s32 arg1);
void func_80136B10(void);
void func_801340CC(void);
void func_80134258(s32 arg0);
void func_801344F0(s32 arg0, s32 arg1);
void func_80134784(s32 *arg0, s32 *arg1);
void func_80133F00(); /* K&R definition */
void func_80133924(void);
void func_80133CA8(void);
void func_801330D0(void);
s32 func_80135A48(s32 arg0, s32 arg1); /* implicit int, no return value */
void func_80135600(void);
void func_801359D0(void);
void func_80135F54(void);
void func_801369F4(s32 arg0);
s32 func_80132B40(void);
void func_80132D44(void);
void func_80132E3C(void);
void func_8013556C(void);
void func_80136A2C(void);
void func_801338EC(void);
void func_80132ADC(void);

extern s32 D_801391E0;
extern s32 D_801391E4;
extern s32 D_801391E8;
extern s32 D_801391EC;
extern s16 D_801391F0;
extern s32 D_801391F4;
extern s32 D_801391F8;
extern s32 D_801391FC;

extern s32 D_80139AF8;
s32 func_801333B0();
void func_801337E0(void);

void func_8013358C();
void func_80132EE8();
void func_80132DC4();

extern s32 D_80139AFC;
extern s32 D_80139278[10];
void func_80135ACC(s32 arg0);
void func_80134C1C();
void func_801339D4();
void func_80134800();
extern s32 D_8013921C;

void func_801321EC(void);
void func_80132308(void);
void func_80132378(void);
extern s32 D_80139220;
extern s32 D_80139224;
extern s32 D_801392A4;
extern s32 D_801392A8;

extern s32 D_80139B20;

s32 func_80132A74();
void func_80134D74();
void func_80134F2C();
void func_80135044();
void func_8013515C();
s32 func_80135274();

extern u16 D_80139C58;
extern u8 D_80139C34[8];
extern u8 D_80139C3C[8];
extern u8 D_80139C44[8];
extern s16 D_80139C4C[6];
void func_80137E2C(void);
s32 func_80137FEC();
#endif /* OVL_EN_NICHI_H */
extern s32 D_80139B24;

#ifndef OVL_EN_NICHI_T9720_TABLES
#define OVL_EN_NICHI_T9720_TABLES
/* Local copies in func_80137FEC: twelve halfwords and 33 callbacks. */
typedef struct EnHalf12 {
    /* 0x00 */ s16 value[12];
} EnHalf12; /* size 0x18 */
typedef struct EnFn33 {
    /* 0x00 */ s32 (*f[33])();
} EnFn33; /* size 0x84 */
extern EnHalf12 D_80139D04;
extern EnFn33 D_80139D1C;
#endif


#ifndef W9_EN_RESET_TABLE
#define W9_EN_RESET_TABLE
/* A 16.16 point (x, y): func_80135ACC reads and writes the words, the other users the integer
 * halves .h[1] (T-20920; T-9710 had four halfwords). */
typedef struct EnFixPos {
    /* 0x00 */ Fix32 unk_00;
    /* 0x04 */ Fix32 unk_04;
} EnFixPos; /* size 0x08 */
extern EnFixPos D_801392AC[15][12]; /* the point grid: 15 rows of 12 (func_801354E0, func_80135600, func_80135ACC, func_80135FBC) */
#endif

#ifndef OVL_EN_NICHI_DISTANCE_T10020
#define OVL_EN_NICHI_DISTANCE_T10020
extern s32 D_80139B00;
extern u8 D_80139B08[12];
/* Exclusive end of the ten status bytes beginning at D_80139B08. */
extern u8 D_80139B12[];
#endif

#ifndef OVL_EN_NICHI_RESET_T10020
#define OVL_EN_NICHI_RESET_T10020
extern s32 D_801392A0;
extern s32 D_80139AF4;
extern s32 D_80139B04;
extern s32 D_80139B18;
void func_801354E0(void);
#endif

#ifndef OVL_EN_NICHI_TABLES_T10020
#define OVL_EN_NICHI_TABLES_T10020
void func_8013216C(void);
void func_80132278(void);
void func_8013275C(void);
void func_80132A0C(void);
extern u8 D_80139A2C[8]; /* follows the D_8013984C records, read as their end (T-10810); object kind -> sprite flag (T-18200) */
extern EnFixPos D_80139228[10]; /* pop-up centres (T-10810), ten up to D_80139278 */
#endif

#ifndef OVL_EN_NICHI_T10850_TYPES
#define OVL_EN_NICHI_T10850_TYPES
/* T-10850: 64 words copied to the local motion lookup table. */
typedef union EnWords64 {
    /* 0x000 */ s32 words[64];
    /* 0x000 */ s32 value[8][8]; /* matrix access in func_80135600, T-14000 */
} EnWords64; /* size 0x100 */
extern EnWords64 D_80139B28;
#endif

#ifndef OVL_EN_NICHI_T18200_TYPES
#define OVL_EN_NICHI_T18200_TYPES
/* T-18200: sprite frames of the objects, six halfwords per row; func_80133CA8 reads row
 * func_80134008(i) for kinds 1..3 and row 8 + func_80134008(i) for kind 4. Ends at D_80139AF4. */
extern s16 D_80139A34[16][6];

/* T-18200: tables that func_80136DEC and func_80138CCC copy to the stack as one block
 * (lw/sw runs, then an lhu/sh tail for the odd sizes); element type from the drafts' reads. */
typedef struct EnHalf22 {
    /* 0x00 */ s16 value[22];
} EnHalf22; /* size 0x2C */
typedef struct EnBytes22 {
    /* 0x00 */ u8 value[22];
} EnBytes22; /* size 0x16 */
typedef struct EnHalf64 {
    /* 0x00 */ s16 value[64];
} EnHalf64; /* size 0x80 */
typedef struct EnHalf13 {
    /* 0x00 */ s16 value[13];
} EnHalf13; /* size 0x1A */
extern EnHalf22 D_80139C68; /* func_80136DEC: 22 display records, +0x14 */
extern EnHalf22 D_80139C94; /* func_80136DEC: +0x22 */
extern EnHalf22 D_80139CC0; /* func_80136DEC: +0x26 */
extern EnBytes22 D_80139CEC; /* func_80136DEC: +0x01 */
extern EnHalf64 D_80139DA0; /* func_80138CCC: picked by rand() & 0x3F */
extern EnHalf13 D_80139E20; /* func_80138CCC: one value per girl, 13 */
#endif
