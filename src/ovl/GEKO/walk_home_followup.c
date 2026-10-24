/* walk_home_followup.c: src/ovl/GEKO/801338F0.c:12,81,143; follow-up callback dispatch, club/topic flags and dialogue group selection. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801338F0-0x80134040 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_strcpy /* FAKE: T-25002 implicit-int strcpy view (s32 return) keeps the original return register allocation in encounter_walk_home_followup_init. */
#include "common.h"
#include "ovl/GEKO.h"

extern s32 strcpy();

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 encounter_walk_home_followup_steps;

void encounter_walk_home_followup_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = encounter_walk_home_followup_steps;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void encounter_walk_home_mira_topic_talk(void) {
    u32 cur;

    cur = encounter_walk_home_topic;
    if (cur == 0x64) {
        encounter_walk_home_talk();
        return;
    }
    mode_step_next();
}

void encounter_walk_home_followup_skip_check(void) {
    u32 cur;
    u16 raw;

    cur = encounter_walk_home_topic;
    if (cur == 0x64) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (game_state.current_girl == GIRL_SAKI) {
        if (encounter_walk_home_group == 0x26) {
            raw = game_state.player_flags.h;
            if (((raw >> 0xC) == 6) && ((game_state.girls[4].flags.b[2] >> 4) == 6)) {
                encounter_walk_home_group = 0x28;
            } else if (((raw >> 0xC) == 7) && ((game_state.girls[4].flags.b[2] >> 4) == 7)) {
                encounter_walk_home_group = 0x29;
            } else {
                encounter_walk_home_group = 0x2A;
            }
        } else {
            mode_step_next();
        }
    } else {
        mode_step_next();
    }
    mode_step_next();
}

s32 encounter_walk_home_followup_talk() {
    /* FAKE: `c50` is a word local above `state` that puts `state` at 0x2B(sp) and avoids cvt_pass widening on encounter_walk_home_topic so uopt keeps bne reg,$v1; `| 0` commutes xor $v0,$v1,reg. T-24090 */
    u32 c50;
    u8 state;

    state = get_g_zyotai_h(game_state.current_girl);
    encounter_walk_home_talk();
    if (*(s16 *)&encounter_walk_home_line == 2 && game_state.current_girl == GIRL_YUKO && (c50 = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(3U);
    }
    if (*(s16 *)&encounter_walk_home_line == 4 && game_state.current_girl == GIRL_YUKO && (c50 = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(state);
    }
    if ((*(s16 *)&encounter_walk_home_line == 1 || *(s16 *)&encounter_walk_home_line == 5) && game_state.current_girl == GIRL_YUMI && (c50 = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(2U);
    }
    if (*(s16 *)&encounter_walk_home_line == 3 && game_state.current_girl == GIRL_YUMI && (c50 = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(3U);
    }
}

void encounter_walk_home_followup_init(void) {
    s32 v;
    u32 g;
    GekoNames12 names;
    u32 temp;

    names = encounter_club_names;
    strcpy(&D_800CA280, names.name[game_state.player_flags.h >> 12]);
    game_state.girls[game_state.current_girl].flags.f.b0 = 1;
    if (encounter_walk_home_topic == 10) {
        game_state.girls[game_state.current_girl].flags.f.b6 = 1;
    }
    encounter_walk_home_group = 0x16;
    v = get_g_zyotai_s(game_state.current_girl);
    /* FAKE: T-25002 reading encounter_walk_home_topic through the word local g keeps bne t8,a0 operand order */
    if ((g = encounter_walk_home_topic) == 2U) {
        g = game_state.current_girl;
        encounter_walk_home_topic = 0;
        if (g == 0) {
            /* FAKE: T-11910 the volatile club view keeps this read before the record load (uopt otherwise sinks it to
             * the first compare), giving the original ugen order; also used in the 1/2/3 branch. */
            v = game_state.player_flags.vh >> 12;
            temp = game_state.girls[g].flags.b[2] >> 4;
            g = temp;
            if (v == g && (u32)v < 6) {
                encounter_walk_home_topic_offer(2);
            } else if (v != g && (u32)v < 6 && g < 6) {
                encounter_walk_home_topic_offer(3);
            } else if ((u32)v < 6 && g >= 6) {
                encounter_walk_home_topic_offer(4);
            } else {
                encounter_walk_home_topic_offer(5);
            }
        } else if (g == 1 || g * 1 == 2 || g == 3 /* FAKE: T-25002 g * 1 puts the shared constant 2 first in the beq */) {
            v = game_state.player_flags.vh >> 12;
            g = game_state.girls[game_state.current_girl].flags.b[2] >> 4;
            if (v == g) {
                encounter_walk_home_topic_offer(2);
            } else if (v != g && (u32)v < 6) {
                encounter_walk_home_topic_offer(3);
            } else {
                encounter_walk_home_topic_offer(5);
            }
        } else if (g == 4 || g == 5 || g == 10) {
            if ((game_state.player_flags.h >> 12) < 6) {
                encounter_walk_home_topic_offer(3);
            } else if ((u8)(v & 0x7F) < 2) {
                encounter_walk_home_topic_offer(4);
            } else {
                encounter_walk_home_topic_offer(5);
            }
        } else if (g == 6 || g == 7 || g == 8 || g == 9) {
            if ((game_state.player_flags.h >> 12) < 6) {
                encounter_walk_home_topic_offer(2);
            } else {
                encounter_walk_home_topic_offer(3);
            }
        }
    }
    mode_step_next();
}

void encounter_walk_home_topic_group_select(void) {
    u32 cur;

    encounter_walk_home_group = (encounter_walk_home_group + encounter_walk_home_topic) - 1;
    cur = encounter_walk_home_topic;
    if (cur == 0x64) {
        encounter_walk_home_group = 0x28;
    }
    mode_step_next();
}

void encounter_walk_home_followup_closing_group(void) {
    encounter_walk_home_group = 0x27;
    mode_step_next();
}

void encounter_walk_home_followup_bg_load(void) {
    bg_read_sub2(0x42D8);
    mode_step_next();
}
