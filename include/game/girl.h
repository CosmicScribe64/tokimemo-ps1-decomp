#ifndef GAME_GIRL_H
#define GAME_GIRL_H

#include "common.h"
#include "game/word_views.h"

/* Flag word of LegacyGirlRecord; EVENT tests bits 12-14 one at a time (sll; bltz/bgez in the original). */
typedef struct LegacyGirlFlags {
    u32 pad : 12;
    u32 b12 : 1;
    u32 b13 : 1;
    u32 b14 : 1;
    u32 rest : 17;
} LegacyGirlFlags; /* size 0x04 */

/* 0x34-byte record of the table at D_800B0A04 (12 records; EVENT indexes it with stride 0x34). */
typedef struct LegacyGirlRecord {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ LegacyGirlFlags unk_0C;
    /* 0x10 */ u8 pad10[7];
    /* 0x17 */ u8 unk_17;
    /* 0x18 */ u8 pad18[0xA];
    /* 0x22 */ u8 unk_22;
    /* 0x23 */ u8 pad23[4];
    /* 0x27 */ u8 unk_27;
    /* 0x28 */ u8 pad28[5];
    /* 0x2D */ u8 unk_2D;
    /* 0x2E */ u8 pad2E[6];
} LegacyGirlRecord; /* size 0x34 */

/* Flag word at +0x0C of a GirlRecord record (GameState +0x1C8 + 0x38 * i). The original tests single bits
 * with sll; bltz/bgez (bits 1, 2, 3, 6, 12-14 seen) and reads bits 9-11 as one value (sll 20; srl 29);
 * a bit-field store loads the byte into another register than its lui. T-9020 (was a view per file). */
typedef struct GirlFlagBits {
    /* 0x00 */ u32 b0 : 1;
    /* 0x00 */ u32 met : 1;
    /* 0x00 */ u32 b2 : 1;
    /* 0x00 */ u32 b3 : 1;
    /* 0x00 */ u32 b4 : 1;
    /* 0x00 */ u32 b5 : 1;
    /* 0x00 */ u32 b6 : 1;
    /* 0x00 */ u32 b7 : 1;
    /* 0x00 */ u32 b8 : 1;
    /* 0x00 */ u32 story_stage : 3; /* HYPOTHESIS: 3-bit per-girl story counter (+1 by SHOUGATU events, compared to 3/4 in main and ENDING) */
    /* 0x00 */ u32 b12 : 1;
    /* 0x00 */ u32 b13 : 1;
    /* 0x00 */ u32 b14 : 1;
    /* 0x00 */ u32 b15 : 1;
    /* 0x00 */ u32 encounter_done : 1; /* HYPOTHESIS: name: set by day_event_dispatch when it starts the forced encounter (mode 0x59) for a met girl with syoushin >= 0x4C; cleared again by girl_counters_clamp whenever her syoushin is below 0x32, so the encounter can fire again later */
    /* 0x00 */ u32 rumor_done : 1; /* HYPOTHESIS: name: set by day_event_dispatch for every met girl with syoushin >= 0x3D when it queues the rumor report (mode 0x44); cleared by girl_counters_clamp whenever her syoushin is below 0x32 */
    /* 0x00 */ u32 b18 : 1;            /* T-20940: holiday_activity_play tests it (sll 13) and sets it (byte +2, ori 4) */
    /* 0x00 */ u32 b19 : 1;
    u32 club : 4;
    u32 rest : 8;
} GirlFlagBits; /* size 0x04 */

/* The GirlRecord flag word: whole, by halves and bytes, or by bits. */
typedef union GirlFlags {
    /* 0x00 */ s32 w;
    /* 0x00 */ u32 u;
    /* 0x00 */ u16 h[2];
    /* 0x00 */ u8 b[4];
    /* 0x00 */ GirlFlagBits f;
} GirlFlags; /* size 0x04 */

/* 0x38-byte record of the table at GameState +0x1BC (16 records: a loop walks it with stride 0x38 up
 * to +0x53C; DATE, KANGEI and SHOUGATU index it; most code reads one record per girl, indexed by
 * GameState.current_girl). The first 0xC bytes are walked as four 0xC-byte steps elsewhere. +0x0C and
 * +0x10 are bit-field words read through the word and through single bytes (+0x10: bits 0-3 and
 * 4-8 are compared with the month and day at GameState +0x3F/+0x40). */
typedef struct GirlRecord {
    /* 0x00 */ s16 tokimeki_fraction;
    /* 0x02 */ s16 tokimeki;
    /* 0x04 */ s16 affection_fraction;
    /* 0x06 */ s16 affection;
    /* 0x08 */ s16 syoushin_fraction;
    /* 0x0A */ s16 syoushin; /* HYPOTHESIS: resentment counter; developer term kept (syoushin_up): +5 per weekly queue build while weighted affection < 10; check_para_limit allows 0..0xFF but girl_counters_clamp (run at the top of every queue-reader pass) clamps it to 0..0x80, so 0x80 is the working cap; >= 0x3C blocks encounters and phone calls; >= 0x4C forces an encounter (flag b16, re-armed once syoushin is below 0x32); >= 0x3D at day end queues a rumor report (flag b17, same re-arm) */
    /* 0x0C */ GirlFlags flags;
    /* 0x10 */ WordView birthday;
    /* 0x14 */ u8 scene_counters[0x24]; /* HYPOTHESIS: 0x24 per-girl progress bytes indexed by event number (DATE, GEKO, TEL read them; [0] of girl 11 is decremented once per weekly queue build by day_event_queue_build_week) */
} GirlRecord; /* size 0x38 */

#endif /* GAME_GIRL_H */
