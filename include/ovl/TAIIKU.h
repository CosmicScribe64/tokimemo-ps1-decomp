#ifndef OVL_TAIIKU_H
#define OVL_TAIIKU_H

#include "common.h"
#include "game.h"

/* Overlay-local externs (T-1300). */
void func_80136A28(void);
void func_80136AB0(void);
void func_80136B18(void);
void func_80136BB4(void);
void func_8013B200(void);
void func_8013B608(void);
void func_8013B78C(void);
void func_8013BB3C(void);
void func_8013BCEC(void);
void func_8013C33C(void);
void func_8013C4E4(s32 arg0);
void func_8013C5C8(s32 arg0);
void func_8013C694(void);
void func_8013CD34(void);
void func_8013CF88(void);
void func_8013D114(void);
void func_8013D2B4(void);
void func_8013D788(void);
void func_8013FC5C(void);
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

extern s32 D_8014A124;
extern s32 D_8014A128;
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
extern u8 D_8014A064;
extern u8 D_8014A11C;

extern u8 D_80148EC0[];
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
extern s32 D_801491E8;
extern s32 D_8014A3D4;
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

extern s32 D_80149940;
typedef struct TaiikuPair {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 val;
} TaiikuPair; /* size 0x4 */
extern TaiikuPair D_801499B0[];
extern volatile u16 D_801499B2; /* D_801499B0[0].val as a u16 flag; volatile is FAKE (func_8013C33C), T-11640 */

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
void func_8013F3A4(s32);
void func_8013F948(void);
void func_80147F58(s32);
void func_801482BC(s32);
void func_80147A6C(s32);
void func_80147B14(s32);
s32 func_80141CDC();
void func_80132E20(s32, s32);
s32 func_80140A90();
extern u8 D_8014A110;
void func_80143D24(void);
void func_80143DD0(void);
void func_80143F50(void);
typedef struct TaiikuRec {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} TaiikuRec; /* size 0x10 */
extern s32 D_80149944;

void func_801339C4();
extern s32 D_801492C4;
void func_8013DDB8(void);
s8 func_8013FE44();
void func_80144834(void);
void func_801448D4(void);
void func_801449A0(void);
void func_80144B40(void);
void func_80144C70(void);
void func_80144E54(void);
void func_80142FB8(void);
void func_80143814(void);
extern s32 D_8014A4F4;
extern s32 D_8014A4F8[12]; /* tpage buffer ids, loaded in func_80145640. T-16553 */
extern s32 D_8014A528[9];
s32 func_801485C4(void);
s32 func_80148678(void);
void func_801487B4(void);
void func_80148820(void);
void func_80148194(s32 arg0);
void func_80145EF4(void);
void func_80145F8C(void);
void func_8014607C(void);
void func_80146338(void);
void func_80148228(s32 arg0);
extern u8 D_8014A1FC;
void func_80146B8C(void);
void func_801488F0(s16 *);
void func_8013D9B4(void);
extern s16 D_80149204;
extern s32 D_801491E4;
extern u8 D_801491E0[4];
extern s32 D_8014994C;
extern s32 D_80149950;
typedef struct {
    s32 unk_00;
    u16 unk_04;
    u16 unk_06;
} TaiikuSel; /* size 8 */
extern TaiikuSel D_80149988;
extern s32 D_8014999C;
void func_8013B104(void);
void func_8013C780(void);
void func_8013C978(void);

extern u8 D_8014A3C4;
extern s32 D_8014A3D0;
extern s32 D_8014A3E0;
extern s32 D_8014A3DC;
void func_80146C20(void);
extern u8 D_8014A3D8[]; /* next free slot gets i + 2 for runner i (func_80147210) */
/* Four s16 values added to the D_8011ECD0 records (func_8014038C copies them to a local). */
typedef struct TkS16x4 {
    /* 0x0 */ s16 v[4];
} TkS16x4; /* size 0x8 */
extern TkS16x4 D_80149F8C;
extern TkS16x4 D_8014A360;
extern u8 D_801492AC[];
extern u8 D_8014A1D4[];
extern s32 D_8014A208[8]; /* tpage buffer ids, func_8014280C */
extern s32 D_8014A228[11]; /* tpage buffer ids, func_8014280C */
extern s16 D_8014A1E8[]; /* s16 table paired with D_8014A1D4 (func_80143DD0) */
void func_801452D0(void);
void func_80141964(void);
s32 func_801419F8(void);
void func_80141AE8(void);
void func_80141B28(void);
void func_80141B70(void);

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
extern u8 D_8014A120[]; /* next free slot gets i + 2 for runner i (func_801449A0) */
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

typedef union {
    s32 word;
    struct {
        s16 low;
        s16 high; /* D_8014929A */
    } halves;
} TaiHalves; /* size 0x4 */
extern TaiHalves D_80149298; /* defined in 80133C80.c */
void func_80133438(void);
void func_801345A4(void);
void func_801365C8(void);
s32 func_801368B8();
void func_801369F0(void);
s32 func_80136DA4();
void func_80136E80(void);
void func_80137544(void);
void func_80135480(void);
s32 func_80137440();
s32 func_80137C44();
s32 func_80137EA8();
void func_80134D94(void);
void func_80138224();
s32 func_80138680();
void func_801356E0(void);
void func_80135778(void);
s16 func_801388E4();

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
typedef struct TaiValue44 {
    /* 0x00 */ u8 pad_00[0x16];
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ u8 pad_18[0x2C];
} TaiValue44; /* size 0x44 */
extern TaiWords4 D_801498AC;
extern TaiWords11 D_801498BC;
extern s16 D_80149320[];
extern s32 D_8014929C;
extern s16 D_801492A0; /* func_80135480 */
extern u8 D_801492A4[2]; /* func_80135480 */
extern u8 D_801492A8[4]; /* func_80135480 */
#endif /* OVL_TAIIKU_W8_3_TYPES_H */
#ifndef TAIIKU_W8_CONTROLLER_TYPES_H
#define TAIIKU_W8_CONTROLLER_TYPES_H
typedef union TaiikuPosition {
    /* 0x00 */ s32 w;
    /* 0x00 */ s16 h[2];
} TaiikuPosition; /* size 0x04; word at 0x8014A3FC, signed high half at +2. T-9570 */
/* Game C runner records (801452D0.c): D_8014A3E4 is the player, D_8014A400[3] the three other
 * runners, two objects: func_801482BC indexes D_8014A400 by runner - 1 and the loops of
 * func_801470A0/func_80147120 by i; written as one 4-element array they compile differently.
 * T-9570, T-16550. */
typedef struct TaiikuController {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ u32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ TaiikuPosition position;
} TaiikuController; /* size 0x1C */
extern TaiikuController D_8014A3E4;
extern TaiikuController D_8014A400[3];
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

#ifndef OVL_TAIIKU_SLOT_T10020
#define OVL_TAIIKU_SLOT_T10020
/* Game C runner sprite records D_8014A454[4], one per runner of D_8014A3E4 (T-10020, T-10850, T-16550). */
typedef struct TaiikuSlot1C {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 pad_10[4];
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u16 unk_18;
    /* 0x1A */ u8 pad_1A[2];
} TaiikuSlot1C; /* size 0x1C */
extern TaiikuSlot1C D_8014A454[4];
#endif

#ifndef OVL_TAIIKU_T10010_SCROLL_POSITION
#define OVL_TAIIKU_T10010_SCROLL_POSITION
void func_8014430C(void);
extern TaiikuPosition D_8014A1CC; /* Scroll position; signed high half at +2. T-10010. */
void func_8013D254(void);
extern s32 D_80149948;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} Taiiku2x16; /* size 4 */
typedef struct {
    u16 unk_00;
    u16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s32 unk_08;
    s32 unk_0C;
} TaiikuA0; /* size 0x10 */
extern TaiikuA0 D_801499A0;
extern u8 D_801499DC;
extern s16 D_801499E0;

typedef struct {
    s32 unk_00;
    s32 unk_04;
} Taiiku2s32; /* size 8 */
typedef struct {
    s16 unk_00;
    s16 unk_02;
} Taiiku2s16; /* size 4 */
typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} Taiiku3s32; /* size 0xC */
/* 0x20-byte block at D_80149968 (T-11640): func_8013D114 reads the high halves of the 16.16 words at
 * 0x14 and 0x18 through the block's base address, so the words are fields of one object. */
typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s16 unk_1C[2]; /* [0] x, [1] y */
} TaiikuBall; /* size 0x20 */
extern TaiikuBall D_80149968;
extern Taiiku3s32 D_80149990;





/* Game A runner record (80133C80.c), 0x24 bytes. The 16.16 word at 0x20 is the track position,
 * read by its signed high half. Game C's TaiikuController has the same roles at other offsets
 * (speed 0x10, timers, position). T-16550. */
typedef struct TaiRunnerA {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02; /* index of the runner's display record (D_8011ECD0 record 46 + n) */
    /* 0x03 */ u8 unk_03; /* animation frame */
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05; /* state of the start sequence */
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
    /* 0x10 */ s32 unk_10; /* speed */
    /* 0x14 */ u32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ u32 unk_1C;
    /* 0x20 */ TaiHalves unk_20; /* 16.16 position */
} TaiRunnerA; /* size 0x24 */
/* Game B runner record (80142240.c), 0x24 bytes: a second layout for the same roles as
 * TaiRunnerA (speed 0x08, accumulated speed 0x0C, timers 0x10/0x18/0x1A, 16.16 position 0x14). T-16550. */
typedef struct TaiRunnerB {
    /* 0x00 */ u8 unk_00; /* animation frame */
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 pad_03;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08; /* speed */
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ u32 unk_10;
    /* 0x14 */ TaiHalves unk_14; /* 16.16 position */
    /* 0x18 */ u16 unk_18;
    /* 0x1A */ u16 unk_1A;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
} TaiRunnerB; /* size 0x24 */
/* D_8014A13C[4]: [0] the player, [1..3] the other runners; every access goes through
 * %lo(D_8014A13C). The u8 view it had made IDO keep the array base in a register for the
 * constant-index clamps of func_80143D24, which a struct member does not; a 4-element array does. T-16550. */
extern TaiRunnerB D_8014A13C[4];

/* D_80149208..D_80149297: the player and three other runners, one object. Every access in the
 * original goes through %lo(D_80149208); the matched loops index the other runners by i from 0
 * (cpu[i]), and func_80138680 indexes all four by runner number. T-16550. */
typedef struct TaiRunnersA {
    /* 0x00 */ TaiRunnerA player;
    /* 0x24 */ TaiRunnerA cpu[3];
} TaiRunnersA; /* size 0x90 */
extern TaiRunnersA D_80149208;

#endif


#ifndef OVL_TAIIKU_LIMITS_T10720
#define OVL_TAIIKU_LIMITS_T10720
/* Limit tables copied to the stack by func_80137090. */
typedef struct {
    /* 0x00 */ s32 v[3];
} TaiikuI3; /* size 0xC */
typedef struct {
    /* 0x00 */ s16 v[3];
} TaiikuH3; /* size 6 */
extern TaiikuI3 D_80149808;
extern TaiikuH3 D_80149814;
#endif

#ifndef OVL_TAIIKU_U16TBL_T10720
#define OVL_TAIIKU_U16TBL_T10720
typedef struct {
    u16 v[11];
} TaiikuU11; /* size 0x16 */
typedef struct {
    s16 v[11];
} TaiikuS11; /* size 0x16 */
extern TaiikuS11 D_80149EE8;
extern s32 D_80149DA4[];
extern TaiikuU11 D_80149F50;
extern TaiikuS11 D_80149F68;
extern s32 D_80149960;
#endif

#ifndef OVL_TAIIKU_T10850_TYPES
#define OVL_TAIIKU_T10850_TYPES

/* T-10850: stack copies in func_80147B14, two 0x14 and two 0x10 byte tables. */
typedef struct TaiikuHalf10 {
    /* 0x00 */ s16 h[10];
} TaiikuHalf10; /* size 0x14 */
typedef struct TaiikuHalf8 {
    /* 0x00 */ s16 h[8];
} TaiikuHalf8; /* size 0x10 */
extern TaiikuHalf10 D_8014A620;
extern TaiikuHalf10 D_8014A634;
extern TaiikuHalf8 D_8014A648;
extern TaiikuHalf8 D_8014A658;
extern u16 D_8014A4C4[8];
extern u16 D_8014A4D8[6];
extern u16 D_8014A4E8[6];
extern TaiikuHalf8 D_8014A668; /* copied to a stack table in func_80147F58. T-16553 */
extern u16 D_8014A4D4[8]; /* with D_8014A4C4 and D_8014A4E4: sprite frame offsets by animation step. T-16553 */
extern u16 D_8014A4E4[8];
#endif

#ifndef OVL_TAIIKU_CAMERA_T10890
#define OVL_TAIIKU_CAMERA_T10890
/* T-10890: three words at D_80122DD0 used by the rotated track grid. */
typedef struct TaiikuCamera0C {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 angle;
} TaiikuCamera0C; /* size 0x0C */
#endif

#ifndef OVL_TAIIKU_RESULT_TABLES_T10890
#define OVL_TAIIKU_RESULT_TABLES_T10890
/* Eleven parallel per-character result entries, copied by func_801346FC. */
typedef struct TaiText11 {
    /* 0x00 */ u8 *value[11];
} TaiText11; /* size 0x2C */
extern TaiikuS11 D_801493FC;
extern TaiText11 D_80149414;
extern TaiText11 D_80149440;
extern TaiWords11 D_8014946C;
extern TaiWords11 D_80149498;
extern u8 D_801491D4;
s32 func_80137784(s32 arg0);
#endif

#ifndef OVL_TAIIKU_SELECTION_T10890
#define OVL_TAIIKU_SELECTION_T10890
/* Copied as 0x58 bytes, indexed by eleven rows of eight actor flags. T-10890. */
typedef struct TaiFlags58 {
    /* 0x00 */ u8 value[11][8];
} TaiFlags58; /* size 0x58 */
extern TaiWords11 D_80149728;
extern TaiikuS11 D_80149754;
extern TaiikuS11 D_8014976C;
extern TaiFlags58 D_80149784;
extern u8 D_801491D8;
#endif

#ifndef OVL_TAIIKU_MOTION_CALLS_T10890
#define OVL_TAIIKU_MOTION_CALLS_T10890
/* Definitions in 80132BD0.c and 80138950.c. T-10890. */
void func_80133928(s32 arg0);
void func_8013ACBC(void);
void func_8013AE14(void);
void func_8013AEFC(void);
s32 func_8013AF74(void);
#endif

#ifndef OVL_TAIIKU_ACTOR_MOTION_T10890
#define OVL_TAIIKU_ACTOR_MOTION_T10890
/* One 16.16 position word; motion reads its signed high half. T-10890. */
extern TaiHalves D_801499C0;
/* Indexed at arg0=1..3; the leading byte is the preceding data padding. */
extern u8 D_801499CB[4];
s32 func_8013FB14(s32 arg0);
s16 func_8013FBA8(s32 arg0);
#endif

 
#ifndef OVL_TAIIKU_T10890_PAGE_TABLES
#define OVL_TAIIKU_T10890_PAGE_TABLES
/* T-10890: nine signed halfword choices; copied as 16 bytes plus 2 bytes. */
typedef struct TaiShorts12 {
    /* 0x00 */ s16 value[9];
} TaiShorts12; /* size 0x12; data symbols are aligned to four bytes */
extern TaiShorts12 D_801493C0;
extern TaiShorts12 D_801493D4;
extern TaiShorts12 D_801493E8;
/* Texture-page handles walked in func_80134060. */
extern s32 D_801492C8[5];
extern s32 D_801492DC[6];
extern s32 D_801492F4;
extern s32 D_801492F8[10];
extern s32 D_8014A070[6];
void func_80133C30(void);
void func_801330D4(void);
#endif

/* T-11080: six-entry s16 table in the data of 80133C80.c (read by func_80137C44). */
extern s16 D_801492B8[6];

#ifndef OVL_TAIIKU_WAIT3_T12020
#define OVL_TAIIKU_WAIT3_T12020
typedef struct TaiWait3 {
    /* 0x00 */ u16 value[3];
} TaiWait3; /* size 0x06 */
extern TaiWait3 D_801497F4;

#endif

/* Eleven character IDs used by func_80135AD0. T-12020. */
extern TaiikuS11 D_80149558;

/* Eleven animation limits copied by func_80136BB4. T-12020. */
extern TaiikuU11 D_801497DC;

#ifndef OVL_TAIIKU_T13600_DECLS
#define OVL_TAIIKU_T13600_DECLS
/* T-13600: scene dispatcher and helpers defined later in 80138950.c. */
void func_80139330(void);
void func_8013A63C(void);
void func_8013A6FC(void);
void func_8013CCFC(void);
void func_8013B0AC(void);
void func_8013F35C(void);
#endif

#ifndef OVL_TAIIKU_RUNNERS_T11640
#define OVL_TAIIKU_RUNNERS_T11640
/* T-11640: TAIIKU's view of the D_8011ECD0 display records (DisplayActor44 layout). The bytes at
 * 0x01 and 0x06 are signed here: func_801400D8 stores them with constants that the original keeps
 * apart from the unsigned switch constants of its later loops, which a u8 field merges. */
typedef struct TaiikuActor44 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ s8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u8 pad_08[4];
    /* 0x0C */ u8 *unk_0C;
    /* 0x10 */ u8 *unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ u8 pad_16[2];
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 pad_1E[8];
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 pad_28[2];
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ u8 pad_2C[8];
    /* 0x34 */ u8 *unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ u8 pad_3C[7];
    /* 0x43 */ u8 unk_43;
} TaiikuActor44; /* size 0x44 */
/* T-11640: per-runner tables of func_8013FC5C (runners 1..3; D_801499B4 holds their 16.16 positions). */
extern TaiHalves D_801499B4[]; /* 16.16 positions; D_801499B0[1..3] seen from func_8013FB14 */
extern TaiikuI3 D_80149F80; /* speed added to each position, copied to the stack */
extern u16 D_801499C4[];
extern u8 D_801499CC[];
extern s32 D_80149DF4[];
extern s32 D_80149DAC[];
/* Tables func_80138AA8 copies to the stack and then into D_80149DA4/D_80149DAC/D_80149DF4. */
typedef struct {
    /* 0x00 */ s32 v[2];
} TaiikuW2; /* size 8 */
typedef struct {
    /* 0x00 */ s32 v[18];
} TaiikuW18; /* size 0x48 */
typedef struct {
    /* 0x00 */ s32 v[5];
} TaiikuW5; /* size 0x14 */
extern TaiikuW2 D_80149E2C;
extern TaiikuW18 D_80149E34;
extern TaiikuW5 D_80149E7C;
extern s16 D_80149966;
/* GTE layouts used by func_8013D2B4 and func_8013FEF0 (moved from 80138950.c, T-11640). */
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} TaiikuSVec; /* SVECTOR layout */

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
} TaiikuVec; /* VECTOR layout */

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 t[3];
} TaiikuMat; /* MATRIX layout, size 0x20 */

/* Track grid tables filled by func_80139AB0 (80 entries each). */
extern s16 D_801499E4[];
extern s16 D_80149A84[];
extern s16 D_80149B24[];
extern s16 D_80149BC4[];
extern s16 D_80149C64[];
extern s16 D_80149D04[];
void func_80139AB0(void);
void func_80139DDC(void);
void func_80139E34(void);
void func_8013FEF0(void);
void func_801400D8(void);
void func_80140338(void);
void func_8014038C(void);
void func_80140414(void);
void func_80145044(void);
void func_801450C0(void);
void func_80145148(void);
void func_8013384C(); /* K&R definition in 80132BD0.c (u8 second parameter) */
void func_80139FE4(void);
void func_8013A314(void);
void func_8014045C(void);
void func_801404DC(void);
void func_8014064C(void);
void func_801406A8(); /* K&R definition */
void func_80140738(s32 arg0, s32 arg1);
void func_8013BEF8(void);
void func_8013C0E4(void);
/* Tables func_80138DF8 copies to the stack and indexes by D_80149940. */
extern TaiWords11 D_80149E90;
extern TaiWords11 D_80149EBC;
typedef struct {
    /* 0x00 */ s16 v[16];
} TaiikuH16; /* size 0x20 */
typedef struct {
    /* 0x00 */ u8 v[16];
} TaiikuB16; /* size 0x10 */
/* Scene tables of func_8013A314, copied to the stack: x, y and frame of records 96..111. */
extern TaiikuH16 D_80149F00;
extern TaiikuH16 D_80149F20;
extern TaiikuB16 D_80149F40;
#endif

#ifndef OVL_TAIIKU_STACK_TABLES_T16550
#define OVL_TAIIKU_STACK_TABLES_T16550
/* Constant tables that unmatched functions copy to the stack as one struct (lw/sw runs in the
 * asm, plus lh/sh for a halfword tail). Element type from the drafts' use of the stack copy,
 * length from the copy. T-16550. */
typedef struct TaiBytes10 {
    /* 0x00 */ u8 v[10];
} TaiBytes10; /* size 0xA */
typedef struct TaiShorts12x {
    /* 0x00 */ s16 v[12];
} TaiShorts12x; /* size 0x18 */
typedef struct TaiShorts20 {
    /* 0x00 */ s16 v[20];
} TaiShorts20; /* size 0x28 */
typedef struct TaiShorts23 {
    /* 0x00 */ s16 v[23];
} TaiShorts23; /* size 0x2E */
typedef struct TaiWords6 {
    /* 0x00 */ s32 v[6];
} TaiWords6; /* size 0x18 */
typedef struct TaiRec16 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} TaiRec16; /* size 0x10 */
typedef struct TaiWords7 {
    /* 0x00 */ s32 v[7];
} TaiWords7; /* size 0x1C */
typedef struct TaiWords8 {
    /* 0x00 */ s32 v[8];
} TaiWords8; /* size 0x20 */
typedef struct TaiWords10 {
    /* 0x00 */ s32 v[10];
} TaiWords10; /* size 0x28 */
typedef struct TaiWords9 {
    /* 0x00 */ s32 v[9];
} TaiWords9; /* size 0x24 */
typedef struct TaiWords12 {
    /* 0x00 */ s32 v[12];
} TaiWords12; /* size 0x30 */
extern s32 D_8014A56C; /* func_80145370 */
extern TaiWords12 D_8014A570; /* func_80145370 */
extern TaiWords9 D_8014A5A0; /* func_80145370 */
extern TaiShorts23 D_80148F28; /* func_80132BD0 */
extern TaiShorts23 D_80148F58; /* func_80132BD0 */
extern TaiShorts23 D_80148F88; /* func_80132BD0 */
extern TaiBytes10 D_80148FE8; /* func_80133258 */
extern TaiikuHalf10 D_80148FF4; /* func_80133258 */
extern TaiikuHalf10 D_80149008; /* func_80133258 */
extern TaiikuHalf10 D_8014901C; /* func_80133258 */
extern s32 D_80149364; /* func_80133CFC */
extern TaiikuW5 D_80149368; /* func_80133CFC */
extern TaiWords7 D_8014937C; /* func_80133CFC */
extern TaiWords10 D_80149398; /* func_80133CFC */
extern TaiWords10 D_801494C4; /* func_80134F98 */
extern TaiikuS11 D_801494F0; /* func_80135480 */
extern TaiShorts20 D_80149508; /* func_80135830 */
extern TaiShorts20 D_80149530; /* func_80135830 */
extern TkS16x4 D_8014985C; /* func_80137EA8 */
extern TaiBytes10 D_80149864; /* func_80138224 */
extern TaiikuHalf10 D_80149870; /* func_80138224 */
extern TaiikuHalf10 D_80149884; /* func_80138224 */
extern TaiikuHalf10 D_80149898; /* func_80138224 */
extern TaiWords6 D_8014A088; /* initialiser of the auto array tbl in func_80141280 (its .data, after D_8014A070), end bound in func_80145640 */
extern TaiikuHalf10 D_8014A27C; /* func_80142400 */
extern TaiikuHalf10 D_8014A290; /* func_80142400 */
extern TaiWords8 D_8014A2A4; /* func_80142400 */
extern TaiShorts12 D_8014A2F0; /* func_80143814 */
extern TaiShorts12 D_8014A304; /* func_80143814 */
extern TkS16x4 D_8014A318; /* func_80144464 */
extern TaiShorts12x D_8014A5C4; /* func_80146338 */
extern TaiShorts12x D_8014A5DC; /* func_80146338 */
extern TkS16x4 D_8014A5F4; /* func_80146D18 */
#endif

/* Result of func_80141CDC: set to 1 by its last state. T-16550. */
extern s32 D_8014A0C0;
