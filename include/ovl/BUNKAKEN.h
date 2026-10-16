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
/* T-18500: three 13-word tables of text addresses (in the loaded scene data at 0x801B0000), filled by
 * func_80148ED0; func_8014A0E0 copies row k of each table into D_800CA160/164/168. */
extern s32 D_801562A0[3][13];

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

/* T-18500: forward calls of func_80144F70 within 80144F70.c. */
void func_80145500(void);
void func_80145550(void);
void func_80145A64(void);

/* T-11390: forward calls within 80143DD0.c. */
void func_80144D38(void);
void func_80144E9C(void);

#ifndef OVL_BUNKAKEN_T12300_DECLS
#define OVL_BUNKAKEN_T12300_DECLS
/* T-12300: scene handlers call the existing definition in 8013BFE0.c. */
extern u8 D_80155F50;

void func_8013CEC0(void);
#endif

#ifndef OVL_BUNKAKEN_T18500_H
#define OVL_BUNKAKEN_T18500_H
/* T-18500: per-heroine tables of the date loaders func_8013B510 and func_80143DD0, indexed by
 * D_80122EBC (12 heroines). */
extern s32 D_80155EA0[12];           /* argument of func_800866A0 */
extern void (*D_80155ED0[12])(void); /* called once the scene data is loaded */
extern u8 D_80155F00[12];            /* first argument of cd_read_sectors */
extern s32 D_80155F0C[12];           /* sector of cd_read_sectors */
extern u8 D_80155F3C[20];            /* argument of func_80083440; the last 8 bytes are zero */

/* T-18500: 11-byte table copied to the stack by func_80144F70 (lw, lw, lwl/lwr), indexed by D_80122EC0. */
typedef struct BunkakenBytes11 {
    /* 0x00 */ u8 b[11];
} BunkakenBytes11; /* size 0x0B */
extern BunkakenBytes11 D_80155F90;

/* T-18500: three byte tables copied to the stack by func_80145550 and read at
 * [D_80122EBC][D_80122EB8]; the table is chosen by GameState.unk_03E - 0x5F. */
typedef struct BunkakenBytes9x6 {
    /* 0x00 */ u8 b[9][6];
} BunkakenBytes9x6; /* size 0x36 */
typedef struct BunkakenBytes11x6 {
    /* 0x00 */ u8 b[11][6];
} BunkakenBytes11x6; /* size 0x42 */
extern BunkakenBytes9x6 D_80155F9C;
extern BunkakenBytes11x6 D_80155FD4;
extern BunkakenBytes11x6 D_80156018;

/* T-18500: 3 x 3 strings of 33 bytes copied to the stack by func_8014A5EC and read at
 * [GameState.unk_03E - 0x5F][D_80122CDC]. */
typedef struct BunkakenNames3x3 {
    /* 0x00 */ u8 text[3][3][33];
} BunkakenNames3x3; /* size 0x129 */
extern BunkakenNames3x3 D_8015633C;
#endif
