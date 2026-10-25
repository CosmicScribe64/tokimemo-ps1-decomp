#ifndef GAME_LAYER_H
#define GAME_LAYER_H

#include "common.h"
#include "main/layer.h"
#include "game/girl.h"

#ifndef GAME_SPRITE_SLOT24_T9740
#define GAME_SPRITE_SLOT24_T9740
/* T-9740: 0x24-byte records at bg_layers, cleared by bg_layers_clear. */
typedef struct BgLayer {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ u8 unk_10;
    /* 0x11 */ u8 unk_11;
    /* 0x12 */ u8 unk_12;
    /* 0x13 */ u8 pad_13;
    /* 0x14 */ u32 unk_14;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ u32 unk_20;
} BgLayer; /* size 0x24 */
#endif

#ifndef GAME_T17015_SRN_REC
#define GAME_T17015_SRN_REC
/* layer_records: 0x28-byte records of the srn_* screen routines. srn_init fills one,
 * srn_vram_set and srn_set read it. T-17015. */
typedef struct SrnLayer {
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04[2];
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ u32 *unk_08;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 unk_10;
    /* 0x11 */ u8 unk_11;
    /* 0x12 */ u8 unk_12;
    /* 0x13 */ u8 unk_13;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u16 unk_18;
    /* 0x1A */ u8 unk_1A[2];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u32 unk_20;
    /* 0x24 */ u8 *unk_24;
} SrnLayer; /* size 0x28 */
/* Record i of layer_records. srn_set indexes the global array directly: with a record pointer as1
 * cannot move the record loads above the stack stores (T-25006). */
#define SRN_REC(i) (((SrnLayer *)layer_records)[i])
#endif

#ifndef GAME_T25006_MOD_REC
#define GAME_T25006_MOD_REC
/* layer_tileset_records: 0x14-byte tile-set records of the mod_* routines (mod_set_gpu reads one). T-25006. */
typedef struct ModTileSet {
    /* 0x00 */ u32 *unk_00;
    /* 0x04 */ u32 *unk_04;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E[6];
} ModTileSet; /* size 0x14 */
#endif

/* T-25010 (moved from include/ovl/OPTION.h): the cell maps, cells and cell indexes of the two BG slots,
 * BgLayer (game.h, GsBG layout) at bg_layers + n * 0x24, whose +0x14 points at map n. Main
 * prim_bg_slot_setup sets up slot n through these three tables; OPTION func_801328C0 fills slot 0 itself (T-20910). */
typedef struct BgCell {
    /* 0x00 */ u8 u;
    /* 0x01 */ u8 v;
    /* 0x02 */ u16 cba;
    /* 0x04 */ u16 flag;
    /* 0x06 */ u16 tpage;
} BgCell; /* size 0x08, PsyQ libgs GsCELL layout */

typedef struct BgMap {
    /* 0x00 */ u8 cellw;
    /* 0x01 */ u8 cellh;
    /* 0x02 */ u16 ncellw;
    /* 0x04 */ u16 ncellh;
    /* 0x06 */ u8 pad_06[2];
    /* 0x08 */ BgCell *base;
    /* 0x0C */ u16 *index;
} BgMap; /* size 0x10, PsyQ libgs GsMAP layout */

/* The cell index of one slot (map->index), one u16 per map position. */
typedef struct BgMapIndex {
    /* 0x00 */ u16 v[64];
} BgMapIndex; /* size 0x80 */

#endif /* GAME_LAYER_H */
