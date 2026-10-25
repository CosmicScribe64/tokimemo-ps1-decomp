/* walk_home_followup.c: src/ovl/GEKO/801338F0.c:12,81,143; follow-up callback dispatch, club/topic flags and dialogue group selection. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801338F0-0x80134040 (config/objects/GEKO.txt). */
#define MAIN_API_OVERRIDE_strcpy /* FAKE: T-25002 implicit-int strcpy view (s32 return) keeps the original return register allocation in encounter_walk_home_followup_init. */
#include "common.h"
#include "ovl/GEKO.h"

extern s32 strcpy();

typedef struct {
    void (*callbacks[19])(); /* HYPOTHESIS: GEKO grouping vocabulary remains unconfirmed; T-27051; src/ovl/GEKO/801338F0.c:9; func_801338F0 at line12 copies this exact19-callback table and dispatches by mode_step; scene number is the actual talk_work selector, not a narrative identity. */
} EncounterWalkHomeFollowupSteps; /* size 0x4C */
extern EncounterWalkHomeFollowupSteps encounter_walk_home_followup_steps;

void encounter_walk_home_followup_dispatch(void) {
    s32 mode_step_index; /* declared before mode_step_callbacks: its stack slot sits above mode_step_callbacks (T-3330) */
    EncounterWalkHomeFollowupSteps mode_step_callbacks;

    mode_step_callbacks = encounter_walk_home_followup_steps;
    mode_step_index = game_state.mode_step;
    mode_step_callbacks.callbacks[mode_step_index](0x80);
}

void encounter_walk_home_mira_topic_talk(void) {
    u32 topic;

    topic = encounter_walk_home_topic;
    if (topic == 0x64) {
        encounter_walk_home_talk();
        return;
    }
    mode_step_next();
}

void encounter_walk_home_followup_skip_check(void) {
    u32 topic;
    u16 player_flags_word;

    topic = encounter_walk_home_topic;
    if (topic == 0x64) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (game_state.current_girl == GIRL_SAKI) {
        if (encounter_walk_home_group == 0x26) {
            player_flags_word = game_state.player_flags.h;
            if (((player_flags_word >> 0xC) == 6) && ((game_state.girls[4].flags.b[2] >> 4) == 6)) {
                encounter_walk_home_group = 0x28;
            } else if (((player_flags_word >> 0xC) == 7) && ((game_state.girls[4].flags.b[2] >> 4) == 7)) {
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
    /* FAKE: `topic_copy` is a word local above `girl_state_result` that puts `girl_state_result` at 0x2B(sp) and avoids cvt_pass widening on encounter_walk_home_topic so uopt keeps bne reg,$v1; `| 0` commutes xor $v0,$v1,reg. T-24090 */
    u32 topic_copy;
    u8 girl_state_result;

    girl_state_result = get_g_zyotai_h(game_state.current_girl);
    encounter_walk_home_talk();
    if (*(s16 *)&encounter_walk_home_line == 2 && game_state.current_girl == GIRL_YUKO && (topic_copy = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(3U);
    }
    if (*(s16 *)&encounter_walk_home_line == 4 && game_state.current_girl == GIRL_YUKO && (topic_copy = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(girl_state_result);
    }
    if ((*(s16 *)&encounter_walk_home_line == 1 || *(s16 *)&encounter_walk_home_line == 5) && game_state.current_girl == GIRL_YUMI && (topic_copy = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(2U);
    }
    if (*(s16 *)&encounter_walk_home_line == 3 && game_state.current_girl == GIRL_YUMI && (topic_copy = encounter_walk_home_topic) == 1U && (game_state.mode_step_timer.u++ | 0) == 1U) {
        portrait_layout_set(3U);
    }
}

void encounter_walk_home_followup_init(void) {
    s32 girl_state_or_player_club;
    u32 topic_girl_or_club;
    EncounterClubNames club_names;
    u32 girl_club;

    club_names = encounter_club_names;
    strcpy(&name_code_m, club_names.by_club[game_state.player_flags.h >> 12]);
    game_state.girls[game_state.current_girl].flags.f.b0 = 1;
    if (encounter_walk_home_topic == ENCOUNTER_TOPIC_BIRTHDAY) {
        game_state.girls[game_state.current_girl].flags.f.b6 = 1;
    }
    encounter_walk_home_group = 0x16;
    girl_state_or_player_club = get_g_zyotai_s(game_state.current_girl);
    /* FAKE: T-25002 reading encounter_walk_home_topic through the word local topic_girl_or_club keeps bne t8,a0 operand order */
    if ((topic_girl_or_club = encounter_walk_home_topic) == 2U) {
        topic_girl_or_club = game_state.current_girl;
        encounter_walk_home_topic = ENCOUNTER_TOPIC_NONE;
        if (topic_girl_or_club == 0) {
            /* FAKE: T-11910 the volatile club view keeps this read before the record load (uopt otherwise sinks it to
             * the first compare), giving the original ugen order; also used in the 1/2/3 branch. */
            girl_state_or_player_club = game_state.player_flags.vh >> 12;
            girl_club = game_state.girls[topic_girl_or_club].flags.b[2] >> 4;
            topic_girl_or_club = girl_club;
            if (girl_state_or_player_club == topic_girl_or_club && (u32)girl_state_or_player_club < 6) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CULTURE_FESTIVAL_MARKER);
            } else if (girl_state_or_player_club != topic_girl_or_club && (u32)girl_state_or_player_club < 6 && topic_girl_or_club < 6) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_THREE);
            } else if ((u32)girl_state_or_player_club < 6 && topic_girl_or_club >= 6) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_FOUR);
            } else {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_FIVE);
            }
        } else if (topic_girl_or_club == 1 || topic_girl_or_club * 1 == 2 || topic_girl_or_club == 3 /* FAKE: T-25002 topic_girl_or_club * 1 puts the shared constant 2 first in the beq */) {
            girl_state_or_player_club = game_state.player_flags.vh >> 12;
            topic_girl_or_club = game_state.girls[game_state.current_girl].flags.b[2] >> 4;
            if (girl_state_or_player_club == topic_girl_or_club) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CULTURE_FESTIVAL_MARKER);
            } else if (girl_state_or_player_club != topic_girl_or_club && (u32)girl_state_or_player_club < 6) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_THREE);
            } else {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_FIVE);
            }
        } else if (topic_girl_or_club == 4 || topic_girl_or_club == 5 || topic_girl_or_club == 10) {
            if ((game_state.player_flags.h >> 12) < 6) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_THREE);
            } else if ((u8)(girl_state_or_player_club & 0x7F) < 2) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_FOUR);
            } else {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_FIVE);
            }
        } else if (topic_girl_or_club == 6 || topic_girl_or_club == 7 || topic_girl_or_club == 8 || topic_girl_or_club == 9) {
            if ((game_state.player_flags.h >> 12) < 6) {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CULTURE_FESTIVAL_MARKER);
            } else {
                encounter_walk_home_topic_offer(ENCOUNTER_TOPIC_CLUB_VARIANT_THREE);
            }
        }
    }
    mode_step_next();
}

void encounter_walk_home_topic_group_select(void) {
    u32 topic;

    encounter_walk_home_group = (encounter_walk_home_group + encounter_walk_home_topic) - 1;
    topic = encounter_walk_home_topic;
    if (topic == 0x64) {
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
