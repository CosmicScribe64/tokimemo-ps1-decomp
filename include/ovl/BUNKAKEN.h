#ifndef OVL_BUNKAKEN_H
#define OVL_BUNKAKEN_H

#include "common.h"
#include "game.h"

/* BUNKAKEN overlay (load address 0x80132000): externs and types. */

extern u32 D_80155E90; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 D_80155E94;
extern u32 D_80155E98;

extern s32 D_80154B84;
extern s32 D_80154B8C;

/* defined in C in one object, called from another (T-0500) */
void func_80132034(void);
void func_80132104(void);
void func_8013216C(void);
void func_801321D4(void);
void func_8013223C(void);
void func_8013230C(void);
void func_80132374(void);
void func_801323DC(void);
void func_80132444(void);
void func_801324AC(void);
void func_801324E0(void);
extern s32 D_801562A0;
extern s32 D_801562A4;
extern s32 D_801562A8;
extern s32 D_801562AC;
extern s32 D_801562B0;
extern s32 D_801562B4;
extern s32 D_801562B8;
extern s32 D_801562BC;
extern s32 D_801562C0;
extern s32 D_801562C4;
extern s32 D_801562C8;
extern s32 D_801562CC;
extern s32 D_801562D0;
extern s32 D_801562D4;
extern s32 D_801562D8;
extern s32 D_801562DC;
extern s32 D_801562E0;
extern s32 D_801562E4;
extern s32 D_801562E8;
extern s32 D_801562EC;
extern s32 D_801562F0;
extern s32 D_801562F4;
extern s32 D_801562F8;
extern s32 D_801562FC;
extern s32 D_80156300;
extern s32 D_80156304;
extern s32 D_80156308;
extern s32 D_8015630C;
extern s32 D_80156310;
extern s32 D_80156314;
extern s32 D_80156318;
extern s32 D_8015631C;
extern s32 D_80156320;
extern s32 D_80156324;
extern s32 D_80156328;
extern s32 D_8015632C;
extern s32 D_80156330;
extern s32 D_80156334;
extern s32 D_80156338;

/* func_80152F4C copies a 4-by-4 word grid and indexes two two-bit fields. */
typedef struct BunkakenGrid4 { /* size 0x40 */
    /* 0x00 */ s32 values[4][4];
} BunkakenGrid4;
extern BunkakenGrid4 D_801564E0;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8013B4A0(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80139DEC(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80143D68(void);

/* T-12701: forward declaration for the scene handlers. */
void func_80142864(void);

#endif

#ifndef OVL_BUNKAKEN_T9960
#define OVL_BUNKAKEN_T9960
/* Byte at D_800E7D4F: continuation flags and three-bit mode (T-9960). */
typedef struct BunkakenModeFlags {
    /* 0x00 */ u8 b0 : 1;
    /* 0x00 */ u8 b1 : 1;
    /* 0x00 */ u8 mode : 3;
    /* 0x00 */ u8 pad : 3;
} BunkakenModeFlags; /* size 0x01 */

void func_80140EF4(void);

void func_8014E3DC(void);

/* Local text-pointer tables copied by func_80152680 (T-9960). */
typedef struct BunkakenTexts12 {
    /* 0x00 */ u8 *text[12];
} BunkakenTexts12; /* size 0x30 */
typedef struct BunkakenTexts11 {
    /* 0x00 */ u8 *text[11];
} BunkakenTexts11; /* size 0x2C */
typedef struct BunkakenTexts4 {
    /* 0x00 */ u8 *text[4];
} BunkakenTexts4; /* size 0x10 */
extern BunkakenTexts12 D_80156470;
extern BunkakenTexts11 D_801564A0;
extern BunkakenTexts4 D_801564CC;
extern u8 D_801564DC;
s32 func_80152F4C(void);
#endif

#ifndef BUNKA_DISPATCH8_T10630_H
#define BUNKA_DISPATCH8_T10630_H
/* T-10630: adjacent route/character words at D_80122EB8, used by both overlays. */
typedef struct BunkaDispatch8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} BunkaDispatch8; /* size 0x08 */
#endif

#ifndef OVL_BUNKAKEN_T10860_H
#define OVL_BUNKAKEN_T10860_H
void func_801362EC(void);
void func_801375D0(void);
void func_80145530(void);
void func_80137640(void);
void func_8014E440(void);

#endif

/* T-10860: forward call within 80132000.c. */
void func_80134FFC(void);
void func_8013EAC8(void);
void func_80148E68(void);
void func_80150CC8(void);
void func_80152618(void);

/* T-11390: forward calls within 80143DD0.c. */
void func_80144D38(void);
void func_80144E9C(void);

#ifndef OVL_BUNKAKEN_T12300_DECLS
#define OVL_BUNKAKEN_T12300_DECLS
/* T-12300: scene handlers call the existing definition in 8013BFE0.c. */
extern u8 D_80155F50;

void func_8013CEC0(void);
#endif
