#ifndef OVL_MASTER_H
#define OVL_MASTER_H

#include "common.h"
#include "game.h"

/* MASTER overlay (load address 0x80132000): externs and types. */

extern u32 D_8013C2D0; /* three pointers set by func_80132000 and siblings */
extern u32 D_8013C2D4;
extern u32 D_8013C2D8;
extern u32 D_8013C4A0; /* same, second group */
extern u32 D_8013C4A4;
extern u32 D_8013C4A8;

extern u32 D_8013C750; /* third group */
extern u32 D_8013C754;
extern u32 D_8013C758;
extern s32 D_8013C710;
extern s32 D_8013C714;
extern s32 D_8013C718;
extern s16 D_8013C71C;
extern s32 D_8013C720;
extern s32 D_8013C724;
extern s32 D_8013C728;
extern s16 D_8013C72C;
extern s32 D_8013C730;
extern s32 D_8013C740;
extern s32 D_8013C744;
extern s32 D_8013C748;
extern s16 D_8013C74C;
extern s32 D_8013C760;
extern s32 D_8013C764;
extern s32 D_8013C768;
extern s16 D_8013C76C;
extern s32 D_8013C770;
void func_80138490();
extern s32 D_8013C4B0;
extern s32 D_8013C4B4;
extern s32 D_8013C4B8;
extern s32 D_8013C4BC;
extern s32 D_8013C4C0;
extern s32 D_8013C4C4;
extern s16 D_8013C4C8;
extern s16 D_8013C4CC;
extern s32 D_8013C4D0;
void func_80138EC0();
void func_80138F70();
extern s32 D_8013C4D4;
extern s32 D_8013C4D8;
extern s32 D_8013C4DC;
extern s32 D_8013C4E0;
extern s32 D_8013C4E4;
extern s32 D_8013C4E8;
extern s32 D_8013C4EC;
extern s32 D_8013C4F0;
extern s32 D_8013C4F4;
extern s32 D_8013C4F8;
extern s32 D_8013C4FC;
extern s32 D_8013C500;
extern s32 D_8013C504;
extern s32 D_8013C508;
extern s32 D_8013C50C;
extern s32 D_8013C510;
extern s32 D_8013C514;
extern s32 D_8013C518;
extern s32 D_8013C51C;
extern s32 D_8013C520;
extern s32 D_8013C524;
extern s32 D_8013C528;
extern s32 D_8013C52C;
extern s32 D_8013C530;
extern s32 D_8013C534;
extern s32 D_8013C538;
extern s32 D_8013C53C;
extern s32 D_8013C540;
extern s32 D_8013C544;
extern s32 D_8013C548;
extern s32 D_8013C54C;
extern s32 D_8013C550;
extern s32 D_8013C554;
extern s32 D_8013C558;
extern s32 D_8013C55C;
extern s32 D_8013C560;
extern s32 D_8013C564;
extern s32 D_8013C568;
extern s32 D_8013C56C;
extern u8 D_8013C790;
extern s32 D_8013C2E0;
extern u8 D_8013C784;

/* T-9960: called by the preceding state machine in the same object. */
void func_80138E58(void);

/* T-9960: eleven word-sized text ids copied into func_801350A8's frame. */
typedef struct MasterWords11 {
    /* 0x00 */ s32 v[11];
} MasterWords11; /* size 0x2C */
extern MasterWords11 D_8013C64C;

#endif /* OVL_MASTER_H */

#ifndef OVL_MASTER_T10630_H
#define OVL_MASTER_T10630_H

typedef struct MasterPoint2 {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} MasterPoint2; /* size 0x04 */
extern s32 D_8013C570;
extern u8 D_8013C578;
extern u8 D_8013C588;
extern MasterPoint2 D_8013C58C[10];
extern s16 D_8013C5B4[10];
#endif

#ifndef OVL_MASTER_T10630_TABLES_H
#define OVL_MASTER_T10630_TABLES_H
/* Message and threshold tables copied into local frames, T-10630. */
typedef struct MasterHalfwords13 {
    /* 0x00 */ s16 v[13];
} MasterHalfwords13; /* size 0x1A */
typedef struct MasterHalfwords5 {
    /* 0x00 */ u16 v[5];
} MasterHalfwords5; /* size 0x0A */
extern MasterHalfwords13 D_8013C6C0;
extern MasterHalfwords13 D_8013C6DC;
/* T-13001: 0x1A-byte copies, indexed signed halfword reads in each consumer. */
extern MasterHalfwords13 D_8013C7E8;
extern MasterHalfwords13 D_8013C804;
extern MasterHalfwords13 D_8013C820;
extern MasterHalfwords5 D_8013C640;
extern u8 D_8013C584;
extern s32 D_8013C574;
extern u8 D_8013C57C[5];
#endif

/* Calls from the large scene handlers to later definitions in this object. */
void func_80137F94(void);
void func_80138088(void);
void func_80138374(void);
void func_80138F10(void);

#ifndef OVL_MASTER_T10630_CLUB_BITS_H
#define OVL_MASTER_T10630_CLUB_BITS_H
#endif

/* T-10630: eleven resource IDs copied by the neighboring selector. */
extern MasterWords11 D_8013C678;

/* T-18600: called by func_80133374 before its definition. */
void func_8013348C(void);

/* T-13001: later helpers called by the typed-table scene handlers. */
void func_8013B354(void);
void func_8013B804(void);

#ifndef OVL_MASTER_T18600_H
#define OVL_MASTER_T18600_H
/* T-18600: tables copied whole into a local frame and called or indexed there (T-3330 idiom). */
typedef struct FnTbl4 {
    /* 0x00 */ void (*f[4])();
} FnTbl4; /* size 0x10 */
typedef struct FnTbl7 {
    /* 0x00 */ void (*f[7])();
} FnTbl7; /* size 0x1C */
typedef struct FnTbl30 {
    /* 0x00 */ void (*f[30])();
} FnTbl30; /* size 0x78 */
typedef struct PtrTbl3 {
    /* 0x00 */ void *p[3];
} PtrTbl3; /* size 0x0C */
/* Thirteen words indexed by D_8013C2E0 or D_8013C790: text ids and thresholds. */
typedef struct MasterWords13 {
    /* 0x00 */ s32 v[13];
} MasterWords13; /* size 0x34 */

extern FnTbl4 D_8013C2E4;        /* func_80132140's scene states */
extern FnTbl30 D_8013C5C8;       /* func_80133A2C's scene states */
extern PtrTbl3 D_8013C700;
extern FnTbl7 D_8013C798;        /* func_80139150's scene states */
/* func_801326B4's eight stack tables, in frame order sp1A8..sp3C */
extern MasterWords13 D_8013C2F4;
extern MasterWords13 D_8013C328;
extern MasterWords13 D_8013C35C;
extern MasterWords13 D_8013C390;
extern MasterWords13 D_8013C3C4;
extern MasterWords13 D_8013C3F8;
extern MasterWords13 D_8013C42C;
extern MasterWords13 D_8013C460; /* followed by three zero words of padding */
extern MasterWords13 D_8013C7B4; /* func_80139810 */
extern MasterHalfwords13 D_8013C6A4; /* func_80135CF4 */

/* Second baseball event (func_80139CC8), the twin of D_8013C574..D_8013C584. */
extern u8 D_8013C780;
extern u8 D_8013C788[5];         /* like D_8013C57C[5]; D_8013C78C is element 4 */
extern s16 D_8013C794;
#endif
