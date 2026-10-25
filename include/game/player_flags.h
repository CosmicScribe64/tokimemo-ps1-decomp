#ifndef GAME_PLAYER_FLAGS_H
#define GAME_PLAYER_FLAGS_H

#include "common.h"

/* Packed icon choice at GameState +0xF68, bits 4..7. */
typedef struct LastCommandBits {
    /* 0x00 */ u16 low : 4;
    u16 icon : 4;
    u16 rest : 8;
} LastCommandBits; /* size 0x02 */

#ifndef TOKIMEMO_SCHEDULE_GIRL_FLAGS_H
#define TOKIMEMO_SCHEDULE_GIRL_FLAGS_H
/* Additional views of GirlRecord.unk_0C, used by the scheduling routines. T-10880. */
typedef struct ScheduleGirlFlagBits {
    /* 0x00 */ u32 unk_00 : 16;
    /* 0x00 */ u32 b16 : 1;
    /* 0x00 */ u32 b17 : 1;
    /* 0x00 */ u32 b18 : 1;
    /* 0x00 */ u32 unk_19 : 1;
    /* 0x00 */ u32 unk_20 : 4;
    /* 0x00 */ u32 unk_24 : 8;
} ScheduleGirlFlagBits; /* size 0x04 */
#endif

/* Flag word at GameState +0xF64 (game_state.quarter_flags) as single bits. hud_magazine_mark_read sets them with
 * bit-field stores, which load the byte into another register than its lui (T-25103): bits 0-11 by
 * year (0x5F, 0x60, 0x61) and month group (2-4, 5-7, 8-10, 11-1), bit 13 in months 2, 5, 8 and 11,
 * bit 14 in the other months, bit 15 in months 5-7. */
typedef struct QuarterFlagBits {
    /* 0x00 */ u32 b0 : 1;
    /* 0x00 */ u32 b1 : 1;
    /* 0x00 */ u32 b2 : 1;
    /* 0x00 */ u32 b3 : 1;
    /* 0x00 */ u32 b4 : 1;
    /* 0x00 */ u32 b5 : 1;
    /* 0x00 */ u32 b6 : 1;
    /* 0x00 */ u32 b7 : 1;
    /* 0x00 */ u32 b8 : 1;
    /* 0x00 */ u32 b9 : 1;
    /* 0x00 */ u32 b10 : 1;
    /* 0x00 */ u32 b11 : 1;
    /* 0x00 */ u32 b12 : 1;
    /* 0x00 */ u32 b13 : 1;
    /* 0x00 */ u32 b14 : 1;
    /* 0x00 */ u32 b15 : 1;
    /* 0x00 */ u32 rest : 16;
} QuarterFlagBits; /* size 0x04 */

#endif /* GAME_PLAYER_FLAGS_H */
