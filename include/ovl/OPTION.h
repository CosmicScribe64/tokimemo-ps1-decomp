#ifndef OVL_OPTION_H
#define OVL_OPTION_H

#include "common.h"
#include "main_api.h"
#include "game.h"

/* OPTION overlay data (bss, not in the overlay file). */
extern u8 D_8013D3A8;
extern u8 D_8013D3B0;
extern u8 D_8013D3B4;
extern s32 **D_8013D360;
extern s32 D_8013E730;
extern u8 D_8013D3E0[];
extern u8 D_8013D3CC[];
extern u8 D_8013D3BC[];
extern volatile u16 D_8013D3B8; /* volatile: both stores of func_80139E3C stay (T-11340) */
extern u8 D_8013D3A4;
extern u8 D_8013D3A0;
extern s8 D_8013D39C;

void func_80132AB8(void);
void func_80132BA8(void);
void func_80133010(void);

void func_8013A09C(void);
void func_8013A1BC(void);
void func_801320C0(void);
void func_8013A484(void);
void func_80133CB8(void);
s32 func_80133330(void);
void func_80133F50(void);
void func_801342A8(void);
void func_80135110(void);
void func_801352FC(void);
void func_8013554C(void);
void func_801359A4(void);
void func_80135C0C(void);
void func_80136688(void);
void func_801368FC(void);
void func_80136A50(void);
void func_80136C30(void);
s32 func_80136F3C(void);
s32 func_801373C8(void);
s32 func_801376A8(void);
void func_80139878(void);

void func_80139A30();
s32 func_801394F4(void);
void func_801399C0();

extern u8 D_8013D3F4;
extern s8 D_8013D180;
void func_80138A24(void);
void func_80138A8C();
void func_8013AF10(void);
s32 func_8013B3BC(void);
void func_8013B568(void);
s32 func_8013BE78(void);
void func_8013BFF8(void);
s32 func_8013C3B0(void);
void func_8013C52C(void);
s32 func_8013C72C(void);
void func_8013447C(void);
void func_80134210(void);
void func_80135DE0(void);
void func_80136BC8(void);
s32 func_80133718(void);
s32 func_8013394C(void);
s32 func_80133AD8(void);
s32 func_80133C4C(void);
s32 func_80135E48(void);
s32 func_801361FC(void);
s32 func_80136434(void);
s32 func_8013661C(void);
s32 func_8013AC30(void);
void func_8013A604(void);
void func_8013ACD4(void);
s32 func_8013AD10(void);
void func_80132384();

extern u8 D_8013E740;
extern s32 D_8013E844[];

/* Sixteen word entries per settings row table, T-14400. */
extern s32 D_8013E744[];
extern s32 D_8013E784[];
extern s32 D_8013E7C4[];
extern s32 D_8013E804[];

/* Audio selection tables: 16-byte text rows, T-14400. */
extern u8 D_8013DD58[63][16];
extern u8 D_8013D968[63][16];
extern u8 D_8013D3F8[58][16];
extern u8 D_8013D3DC;
void func_8013A304(void);
void func_8013A31C(void);
void func_8013A4FC(void);
void func_80138DF8(void);
void func_80138C20(void);
void func_80138FC0(void);

#ifndef OVL_OPTION_DISPATCH_T10020
#define OVL_OPTION_DISPATCH_T10020
void func_8013990C(void);
#endif

#ifndef OVL_OPTION_GRID_T10010
#define OVL_OPTION_GRID_T10010
/* Sixteen menu rectangles stored as separate coordinate/extent arrays. */
typedef struct OptionGrid80 {
    /* 0x00 */ s16 x[16];
    /* 0x20 */ s16 y[16];
    /* 0x40 */ s16 w[16];
    /* 0x60 */ s16 h[16];
} OptionGrid80; /* size 0x80 */
extern u32 *D_8013D2C4[];
extern s32 D_8013D2FC;
#endif

/* Menu rows of func_80139198 as four parallel arrays (x, y, w, h), passed to menu_set (T-17800). */
extern s16 D_8013D364[7];
extern s16 D_8013D372[7];
extern s16 D_8013D380[7];
extern s16 D_8013D38E[7];

/* Word tables indexed by `D_8013D3DC % 58` and by `D_8013D3A0 % 63` / `D_8013D3A4 % 63`, read with lw
 * (func_8013A09C, func_8013A1BC); each ends where the next label starts (T-17800). */
extern s32 D_8013D798[58];
extern s32 D_8013D880[58];
extern s32 D_8013E148[63];
extern s32 D_8013E244[63];
extern s32 D_8013E340[63];
extern s32 D_8013E43C[63];
extern s32 D_8013E538[63];
extern s32 D_8013E634[63];

/* Scroll state of func_801326BC (T-17800). */
extern s32 D_8013D300;
extern s32 D_8013D304;
extern s32 D_8013D308;

/* Four pointers walked up to the next label, D_8013D360 (func_8013554C, T-17800). */
extern u8 *D_8013D350[4];

/* Main-exe bss read only here (CODING_STANDARDS 8a: the types live in this header while no other unit
 * declares the symbols): the cell maps, cells and cell indexes of the two BG slots, SpriteSlot24 (game.h,
 * GsBG layout) at D_801220D0 + n * 0x24, whose +0x14 points at map n. Main func_8004A1A0 (asm) sets up
 * slot n through these three tables; func_801328C0 fills slot 0 itself (T-20910). */
typedef struct SpriteCell8 {
    /* 0x00 */ u8 u;
    /* 0x01 */ u8 v;
    /* 0x02 */ u16 cba;
    /* 0x04 */ u16 flag;
    /* 0x06 */ u16 tpage;
} SpriteCell8; /* size 0x08, PsyQ libgs GsCELL layout */

typedef struct SpriteMap10 {
    /* 0x00 */ u8 cellw;
    /* 0x01 */ u8 cellh;
    /* 0x02 */ u16 ncellw;
    /* 0x04 */ u16 ncellh;
    /* 0x06 */ u8 pad_06[2];
    /* 0x08 */ SpriteCell8 *base;
    /* 0x0C */ u16 *index;
} SpriteMap10; /* size 0x10, PsyQ libgs GsMAP layout */

/* The cell index of one slot (map->index), one u16 per map position. */
typedef struct SpriteIndex80 {
    /* 0x00 */ u16 v[64];
} SpriteIndex80; /* size 0x80 */

extern SpriteMap10 D_80122120[2];
extern SpriteCell8 D_80122140[2][64];
extern SpriteIndex80 D_80122540[2];

/* Views of shared records, moved here from the .c files (T-17800). */

/* Four s16 rows filled by func_801325A0 for menu_set. */
typedef struct Arrs4x8 {
    /* 0x00 */ s16 a[8];
    /* 0x10 */ s16 b[8];
    /* 0x20 */ s16 c[8];
    /* 0x30 */ s16 d[8];
} Arrs4x8; /* size 0x40 */

void func_801325A0(Arrs4x8 *p);

/* Packed Rec38 settings fields, T-14400. */
typedef struct OptionCharBits {
    /* 0x00 */ u32 b0 : 1;
    /* 0x00 */ u32 b1 : 1;
    /* 0x00 */ u32 b2 : 1;
    /* 0x00 */ u32 b3 : 1;
    /* 0x00 */ u32 unk_04 : 2;
    /* 0x00 */ u32 b6 : 1;
    /* 0x00 */ u32 b7 : 1;
    /* 0x00 */ u32 b8 : 1;
    /* 0x00 */ u32 f9 : 3;
    /* 0x00 */ u32 b12 : 1;
    /* 0x00 */ u32 b13 : 1;
    /* 0x00 */ u32 unk_14 : 6;
    /* 0x00 */ u32 f20 : 4;
    /* 0x00 */ u32 f24 : 8;
} OptionCharBits; /* size 0x04 */



/* Packed date word at GameState +0xF8 and Rec38 +0x10: GsDateBits (main_api.h) plus the two bits at 9-10,
 * read as `& 0xF`, `<< 0x17 >> 0x1B`, `<< 0x15 >> 0x1E` (func_8013A7EC, T-17800). */
typedef struct OptionDateBits {
    /* 0x00 */ u32 f0 : 4;
    /* 0x00 */ u32 f4 : 5;
    /* 0x00 */ u32 f9 : 2;
    /* 0x00 */ u32 unk_11 : 21;
} OptionDateBits; /* size 0x04 */


/* 8-bit bitfield view used to prevent uopt from CSE-ing the second D_800B3220 load in func_80133330 (T-24117). */
typedef struct OptionByteBf {
    /* 0x00 */ u8 x : 8;
} OptionByteBf; /* size 0x01 */

#endif /* OVL_OPTION_H */
