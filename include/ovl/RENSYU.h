#ifndef OVL_RENSYU_H
#define OVL_RENSYU_H

#include "common.h"
#include "game.h"

/* RENSYU overlay (load address 0x80132000): externs and types. */

extern s32 D_801342B0;
extern s32 D_801342B4;
extern s32 D_801342B8;
extern s32 D_801343D0;
extern s32 D_801343D4;
extern s32 D_801343D8;
extern s16 D_801343DC;
extern s32 D_801343E0;
extern u8 D_801342D0;
extern u8 D_801342C4;

/* T-19020: GameState.unk_66C[i] seen as a halfword pair; the low 4 bits of the halfword at +2 are a counter
 * (incremented as lhu, +1, andi 0xF, merged with the lbu of the same byte, sb). */
typedef struct RensyuClubCount {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 count : 4;
    /* 0x02 */ u16 unk_02_4 : 12;
} RensyuClubCount; /* size 0x04 */

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_801340B0(void);
void func_80133FB4(void);
void func_8013405C(void);
void func_80134008(void);
void func_80133F60(void);

#endif /* OVL_RENSYU_H */
