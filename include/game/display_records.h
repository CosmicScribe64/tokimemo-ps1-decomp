#ifndef GAME_DISPLAY_RECORDS_H
#define GAME_DISPLAY_RECORDS_H

#include "common.h"

/* T-10890: parameter/name display view of the actor_pool record pool. */

#ifndef GAME_DISPLAY_ACTOR44_T10890
#define GAME_DISPLAY_ACTOR44_T10890
/* One record of the 0x44-byte display-record pools (T-20900): main actor_pool (160 records, actor_pool_96 is
 * record 96), EVENT D_800E9620 and GYOZI D_80129F40 (the same pool at the addresses those two overlays were
 * linked against). Field widths are the union of every unit's accesses: the original asm has lbu/sb only at
 * the byte fields (no lb through the pool symbols), lh/sh at the halfwords and lw/sw at +0x0C, +0x10, +0x34
 * and +0x38 (the other word accesses are whole-record copies). +0x24 .. +0x33 are also read as 16.16
 * fixed-point words: ActorFixed is that view of the same record. The per-unit views this replaces are
 * listed in wiki/data-types.md ("Shared-type unification"); TAIIKU's TaiikuActor44 stays (signed bytes). */
typedef struct Actor {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 flags;
    /* 0x03 */ u8 draw_flags;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 blink_period;
    /* 0x07 */ u8 brightness;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *frame_table;
    /* 0x10 */ u8 *sequence_table;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 sequence_slot;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 scale_x; /* HYPOTHESIS: 0x1000 = 1.0 (actor_clear default) */
    /* 0x1C */ s16 scale_y; /* HYPOTHESIS: 0x1000 = 1.0 */
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 rotation;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 x_fraction;
    /* 0x26 */ s16 x;
    /* 0x28 */ s16 y_fraction;
    /* 0x2A */ s16 y;
    /* 0x2C */ s16 dx_fraction; /* HYPOTHESIS: x step per frame */
    /* 0x2E */ s16 dx; /* HYPOTHESIS: integer part of the x step; the parameter panel (parameter_show) also uses it as a 0/1/2 animation phase */
    /* 0x30 */ s16 dy_fraction; /* HYPOTHESIS: y step per frame */
    /* 0x32 */ s16 dy; /* HYPOTHESIS: integer part of the y step */
    /* 0x34 */ u8 *tpage_table;
    /* 0x38 */ s32 quad_flags;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} Actor; /* size 0x44 */

/* 16.16 fixed-point word: word arithmetic through w, the signed integer part (the coordinate the halfword
 * view of a display record reads at +2) through h[1]. One type for the former MASTER, KANGEI, DATE, TAIIKU and
 * NAME_ENT copies (T-20900). */
typedef union Fix32 {
    /* 0x00 */ s32 w;
    /* 0x00 */ s16 h[2];
} Fix32; /* size 0x04 */

/* Actor with +0x24 and +0x28 as 16.16 fixed-point words and +0x2C, +0x30 as words (their
 * steps): the view of the units that move a record in fixed point (MASTER, KANGEI, DATE, EVENT, NAME_ENT,
 * TAIIKU, TEL). C89 has no anonymous unions, so the halfword users (most of the tree) keep Actor.
 * T-20900. */
typedef struct ActorFixed {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 flags;
    /* 0x03 */ u8 draw_flags;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *frame_table;
    /* 0x10 */ u8 *sequence_table;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 sequence_slot;
    /* 0x18 */ s16 frame;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20; /* rotate, copied as one word by actor_draw_all (lw 0x20, T-25606) */
    /* 0x24 */ Fix32 x;
    /* 0x28 */ Fix32 y;
    /* 0x2C */ s32 dx;
    /* 0x30 */ s32 dy;
    /* 0x34 */ u8 *tpage_table;
    /* 0x38 */ s32 quad_flags;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} ActorFixed; /* size 0x44 */

/* T-10890: image metadata consumed by portrait_build; eight observed parts. */
typedef struct SpritePart {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
} SpritePart; /* size 0x0E */

typedef struct SpriteFrame {
    /* 0x00 */ s16 count;
    /* 0x02 */ s16 origin_x;
    /* 0x04 */ s16 origin_y;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ SpritePart part[8];
} SpriteFrame; /* size 0x78 */

/* T-10890: two packed PSX 5:5:5 colours, interpolated as one word. */
typedef struct ColorPair555 {
    /* 0x00 */ u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 pad_0 : 1;
    /* 0x02 */ u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 pad_1 : 1;
} ColorPair555; /* size 0x04 */
#endif

#endif /* GAME_DISPLAY_RECORDS_H */
