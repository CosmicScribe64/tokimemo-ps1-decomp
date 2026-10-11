#ifndef OVL_TAIIKU_H
#define OVL_TAIIKU_H

#include "common.h"
#include "game.h"

/* Overlay-local externs (T-1300). */
void func_80136A28(void);
void func_80136AB0(void);
void func_80136B18(void);
void func_80136BB4(void);
void func_8013CD34(void);
void func_8013CF88(void);
void func_8013D114(void);
void func_8013D2B4(void);
void func_80146884(void);
void func_8014692C(void);
void func_80146994(void);
void func_80146A60(void);
void func_801470A0(void);
void func_80147120(void);
void func_80147210(void);
void func_80147318(void);
void func_801331D0(void);
void func_801330FC(void);
void func_80147928(void);
void func_80147594(void);
void func_80143C84(void);
void func_80143CC0(void);
void func_8014756C(void);

extern s32 D_8014A128;
extern s32 D_8014A144;
extern s32 D_8014A158;
extern s32 D_80148F00;
extern s32 D_80148F04;
extern s32 D_80148F08;
extern s32 D_801491B0;
extern s32 D_801491B4;
extern s32 D_801491B8;
extern s32 D_8014A0A0;
extern s32 D_8014A0A4;
extern s32 D_8014A0A8;
extern s32 D_8014A0D0;
extern s32 D_8014A0D4;
extern s32 D_8014A0D8;
extern u8 D_801499D8;
extern u8 D_8014A11C;

extern s32 D_80148EF0;
extern s32 D_80148EF4;
extern s32 D_80148EF8;
extern s32 D_80148EFC;
extern s32 D_801491C0;
extern s32 D_801491C4;
extern s32 D_801491C8;
extern s32 D_801491CC;
extern s32 D_80149FA0;
extern s32 D_80149FA4;
extern s32 D_80149FA8;
extern s32 D_80149FAC;
extern s32 D_8014A0B0;
extern s32 D_8014A0B4;
extern s32 D_8014A0B8;
extern s32 D_8014A0BC;
extern s32 D_8014A12C;
extern s32 D_8014A148;
extern u32 D_8014A14C;
extern s32 D_801491E8;
extern s32 D_80149218;
extern s32 D_80149220;
extern u32 D_80149224;
extern s32 D_8014A3D4;
extern u32 D_8014A3F0;
extern s32 D_8014A3F4;
extern s32 D_8014A3F8;
extern s32 D_8014A370;
extern s32 D_8014A374;
extern s32 D_8014A378;
extern s32 D_8014A37C;
extern s32 D_8014A380;
extern s32 D_8014A384;
extern s32 D_8014A388;

extern s32 D_8014A0E0;
extern s32 D_8014A0E4;
extern s32 D_8014A0E8;
extern s16 D_8014A0EC;
extern s32 D_8014A0F0;
extern s32 D_8014A0F4;
extern s32 D_8014A0F8;
extern s32 D_8014A0FC;
extern s32 D_8014A100;
extern s32 D_8014A104;
extern s32 D_8014A108;
extern s32 D_8014A390;
extern s32 D_8014A394;
extern s32 D_8014A398;
extern s16 D_8014A39C;
extern s32 D_8014A3A0;
extern s32 D_8014A3A4;
extern s32 D_8014A3A8;
extern s32 D_8014A3AC;
extern s32 D_8014A3B0;
extern s32 D_8014A3B4;
extern s32 D_8014A3B8;

extern u8 D_8014A454[];
extern s16 D_80149984;
extern s16 D_80149986;
extern s32 D_80149940;
typedef struct TaiikuPair {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 val;
} TaiikuPair; /* size 0x4 */
extern TaiikuPair D_801499B0[];

extern s32 D_80149FB0;
extern s32 D_80149FC0;
extern s32 D_80149FD0;
extern s32 D_80149FE8;
extern s32 D_80149FF8;
extern s32 D_8014A008;
extern s16 D_8014A020;
extern s16 D_8014A030;
extern s16 D_8014A040;
void func_80136EC0(void);
void func_8013703C(void);
void func_80137090(void);
void func_80137228(void);
void func_8013732C(void);
extern u16 D_801499A2;
void func_8013F3A4(s32);
void func_8013F948(void);
void func_80147F58(s32);
void func_801482BC(s32);
void func_80147A6C(s32);
void func_80147B14(s32);
s32 func_80141CDC();
void func_80132E20(s32, s32);
s32 func_80140A90();
extern s8 D_8014A110;
void func_80143D24(void);
void func_80143DD0(void);
void func_80143F50(void);
extern u16 D_8014A154;
typedef struct TaiikuRec {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} TaiikuRec; /* size 0x10 */
extern s32 D_80149944;

void func_801339C4(void);
extern u8 D_801492C4;
void func_8013DDB8(void);
s8 func_8013FE44();
void func_80144834(void);
void func_801448D4(void);
void func_801449A0(void);
void func_80144B40(void);
void func_80144C70(void);
extern u8 D_8014A4F4;
extern u8 D_8014A1FC;
void func_80146B8C(void);
void func_801488F0(s16 *);
void func_8013D9B4(s32, u8);
extern s16 D_80149204;
extern u32 D_8014921C;
extern s32 D_801491E4;
extern u8 D_801491E0[4];
extern s32 D_8014994C;
extern s32 D_80149950;
extern s32 D_80149974;
extern s32 D_80149988;
extern s32 D_8014999C;
void func_8013B104(void);
void func_8013C780(void);
void func_8013C978(void);
extern s32 D_80149990;
extern TaiikuPair D_801499B4[];

extern u8 D_8014A3C4;
extern s32 D_8014A3D0;
extern u32 D_8014A3EC;
extern u8 D_8014A460;
extern s32 D_8014A3E0;
extern s32 D_8014A3DC;
extern s16 D_8014A3FE;
void func_80146C20(void);
extern u8 D_8014A3D8[];
/* Four s16 values added to the D_8011ECD0 records (func_8014038C copies them to a local). */
typedef struct TkS16x4 {
    /* 0x0 */ s16 v[4];
} TkS16x4; /* size 0x8 */
extern TkS16x4 D_80149F8C;
extern TkS16x4 D_8014A360;
extern u8 D_801492AC[];
extern u8 D_8014A1D4[];
void func_801452D0(void);

typedef struct {
    s32 v[11];
} TaiikuTbl11; /* size 0x2C */
extern u8 D_801491DC;
extern TaiikuTbl11 D_801498E8;
extern TaiikuTbl11 D_80149914;
void func_8014684C(void);
void func_80147068(void);
void func_80147520(void);
void func_80147F10(void);
extern s16 D_8014A3DE;
extern u8 D_8014A120[];
extern s32 D_8014A130[];
extern s16 D_8014A1CE;

#endif
extern s32 D_80149954;
extern s16 D_80149958;
extern s16 D_8014995A;
extern u8 D_801499D4[];
void func_8013B5C0(void);
extern u8 D_8014A118;

typedef struct TkCnt {
    /* 0x0 */ u8 unk_00;
    /* 0x4 */ u32 unk_04;
    /* 0x8 */ u32 unk_08;
} TkCnt; /* size 0xC */

extern s16 D_8014929A;
void func_80133438(void);
void func_801345A4(void);
s32 func_801365C8();
s32 func_801368B8();
void func_801369F0(void);
s32 func_80136DA4();
void func_80136E80(void);
s32 func_80137440();
s32 func_80137C44();
s32 func_80137EA8();
void func_80134D94(void);
void func_80138224();

#ifndef OVL_TAIIKU_W8_3_TYPES_H
#define OVL_TAIIKU_W8_3_TYPES_H
/* Local random-selection table: eleven rows of ten signed halfwords.
 * Both the block copy and row stride are visible in func_80135D24. T-9530. */
typedef struct TaiRandomTable {
    /* 0x00 */ s16 value[11][10];
} TaiRandomTable; /* size 0xDC */
extern TaiRandomTable D_80149570;
extern s16 D_801491EC;

typedef struct TaiWords4 {
    /* 0x00 */ s32 value[4];
} TaiWords4; /* size 0x10 */
typedef struct TaiWords11 {
    /* 0x00 */ s32 value[11];
} TaiWords11; /* size 0x2C */
typedef struct TaiSlot24 {
    /* 0x00 */ u8 pad_00[2];
    /* 0x02 */ u8 index;
    /* 0x03 */ u8 pad_03[0x1D];
    /* 0x20 */ union {
        s32 word;
        struct {
            s16 low;
            s16 high;
        } halves;
    } unk_20;
} TaiSlot24; /* size 0x24 */
typedef struct TaiValue44 {
    /* 0x00 */ u8 pad_00[0x16];
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ u8 pad_18[0x2C];
} TaiValue44; /* size 0x44 */
extern TaiWords4 D_801498AC;
extern TaiWords11 D_801498BC;
extern s16 D_80149320[];
extern u8 D_80149208[];
extern s32 D_8014929C;
#endif /* OVL_TAIIKU_W8_3_TYPES_H */
#ifndef TAIIKU_W8_CONTROLLER_TYPES_H
#define TAIIKU_W8_CONTROLLER_TYPES_H
typedef union TaiikuPosition {
    /* 0x00 */ s32 w;
    /* 0x00 */ s16 h[2];
} TaiikuPosition; /* size 0x04; word at 0x8014A3FC, signed high half at +2. T-9570 */
typedef struct TaiikuController {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 pad_01[3];
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ u32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ TaiikuPosition position;
} TaiikuController; /* size 0x1C; extends TkCnt through the fixed-point position. T-9570 */
extern TaiikuController D_8014A3E4;
typedef struct TaiikuScene44 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 pad_03[0x15];
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ u8 pad_1A[0x0A];
    /* 0x24 */ TaiikuPosition position;
    /* 0x28 */ u8 pad_28[0x1C];
} TaiikuScene44; /* size 0x44; camera record 104 and related records. T-9570 */

#endif


/* T-9510: coordinate bounds loaded with LH in func_8013C5C8. */
extern s16 D_801499A4;
extern s16 D_801499A6;

#ifndef OVL_TAIIKU_T9510_COORD_TYPES
#define OVL_TAIIKU_T9510_COORD_TYPES
/* T-9510: the two coordinate words are read both whole and by high half. */
typedef union TkCoord32 {
    /* 0x00 */ s32 word;
    struct {
        /* 0x00 */ s16 lo;
        /* 0x02 */ s16 hi;
    } halves;
} TkCoord32; /* size 0x04 */

typedef struct TkCoordsView {
    /* 0x0000 */ u8 pad0[0x19E8];
    /* 0x19E8 */ TkCoord32 pos[2];
} TkCoordsView; /* accessed prefix size 0x19F0 */
#endif

typedef struct TaiikuWords48 {
    /* 0x00 */ s32 unk_00[12][4];
} TaiikuWords48; /* size 0xC0 */
extern TaiikuWords48 D_80149030;
extern TaiikuWords48 D_801490F0;

extern s32 D_8014A1D0;


#ifndef OVL_TAIIKU_RANDOM_T9610
#define OVL_TAIIKU_RANDOM_T9610
extern TaiRandomTable D_8014964C;
#endif

#ifndef OVL_TAIIKU_W9_2_INIT_FIELDS_H
#define OVL_TAIIKU_W9_2_INIT_FIELDS_H
/* Workspace address words and asset halfwords used by func_80140810. T-9720. */
extern s32 D_80149FB4;
extern s32 D_80149FB8;
extern s32 D_80149FBC;
extern s32 D_80149FC4;
extern s32 D_80149FC8;
extern s32 D_80149FCC;
extern s32 D_80149FD4;
extern s32 D_80149FD8;
extern s32 D_80149FDC;
extern s32 D_80149FE0;
extern s32 D_80149FE4;
extern s32 D_80149FEC;
extern s32 D_80149FF0;
extern s32 D_80149FF4;
extern s32 D_80149FFC;
extern s32 D_8014A000;
extern s32 D_8014A004;
extern s32 D_8014A00C;
extern s32 D_8014A010;
extern s32 D_8014A014;
extern s32 D_8014A018;
extern s32 D_8014A01C;
extern s16 D_8014A024;
extern s16 D_8014A028;
extern s16 D_8014A02C;
extern s16 D_8014A034;
extern s16 D_8014A038;
extern s16 D_8014A03C;
extern s16 D_8014A044;
extern s16 D_8014A048;
extern s16 D_8014A04C;
extern s16 D_8014A050;
extern s16 D_8014A054;
extern s16 D_8018EC40;
extern s16 D_8018EC44;
extern s16 D_8018EC48;
extern s16 D_8018EC4C;
extern s16 D_8018EC50;
extern s16 D_8018EC54;
extern s16 D_8018EC58;
extern s16 D_8018EC5C;
extern s16 D_8018EC60;
extern s16 D_8018EC64;
extern s16 D_8018EC68;
extern s16 D_8018EC6C;
extern s16 D_8018EC70;
extern s16 D_8018EC74;
/* T-9930: calls from the race-state dispatcher; matched definitions below it. */
void func_80143AC8(void);
void func_80143C04(void);
void func_80144768(void);
#endif
