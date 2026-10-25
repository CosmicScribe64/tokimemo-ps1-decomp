/* shrine_actor_sequences.c: ; src/ovl/SHOUGATU/80134120.c:23-45 dispatches shrine-scene actor sequences. Original object 0x80134120-0x80134890 (config/objects/SHOUGATU.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* Word argument at this call; T-9520. */
#include "common.h"
#include "ovl/new_year_christmas.h"
void talk_names_set();

void new_year_shrine_actor_mode_step_dispatch(void) {
    void (**p)();
    FnTbl25 tbl;

    tbl = D_80143D24;
    if (new_year_fallback_dialogue_flag != 0) {
        p = &tbl.f[game_state.mode_step];
        if (*p == normal_date_girl_in || tbl.f[game_state.mode_step] == normal_date_girl_out) {
            tbl.f[game_state.mode_step] = mode_step_next;
            /* FAKE: empty double test (decomp-permuter, score 0); it only shifts the register choice of base, index and pointer to the original's; real source unknown. T-4090 */
            if (!game_state.mode_step && !game_state.mode_step) {
            }
        }
    }
    tbl.f[game_state.mode_step](0x80);
}

void new_year_shrine_saki_status_talk_gate(void) {
    u16 six;
    u32 result;
    u32 mode;

    /* FAKE: the narrow call cast reserves a temporary; the dead raw-byte
     * test retains separate raw/shifted ranges, and u16 six keeps the branch
     * operand order. T-10010. */
    six = 6;
    if (new_year_fallback_dialogue_flag == 0) {
        result = (u8) get_g_zyotai_s(game_state.current_girl) & 0x7F;
        if ((game_state.current_girl == GIRL_SAKI) && (result >= 2U)) {
            mode = (u8) game_state.girls[4].flags.b[2] >> 4;
            if (game_state.girls[4].flags.b[2]) {
            }
            if ((mode == six) && (new_year_dialogue_line == 1)) {
                new_year_dialogue_line = 2;
            } else if ((mode != six) && (new_year_dialogue_line == 0)) {
                new_year_dialogue_line = 1;
            }
        }
    }
    new_year_talk_run_current();
}

void new_year_shrine_conditional_audio_update(void) {
    new_year_talk_run_current();
    if ((new_year_dialogue_line == 2) && (game_state.substep == 0) && (D_8011ED5B & 0x80)) {
        audio_command(0x202);
        game_state.substep += 1;
    }
    if ((game_state.substep == 0) && (new_year_dialogue_line == 3)) {
        audio_command(0x202);
        game_state.substep += 1;
    }
}

void new_year_shrine_date_girl_save(void) {
    game_state.date_girl = game_state.current_girl;
    mode_step_next();
}

void new_year_shrine_background_and_log_setup(void) {
    if ((new_year_fallback_dialogue_flag == 0) && !(game_state.frame_count & 1) && (game_state.event_log[game_state.event_log_count - 1].scene != 5)) {
        if (new_year_shiori_route_flag != 0) {
            event_log_add(5, 4);
        } else {
            event_log_add(5, 1);
        }
    }
    bg_read_sub2(0x41F6);
    mode_step_next();
}

void new_year_shrine_choice_effect_apply(void) {
    s32 pad; /* FAKE: unused local declared first puts tbl at sp+0x28 as in the original; real source unknown. T-9340 */
    SjisStr3 tbl;

    tbl = D_80143D88;
    new_year_omikuji_scene_selector = talk_choice;
    new_year_dialogue_group += talk_choice;
    strcpy(name_code_w, tbl.s[talk_choice]);
    if (new_year_fallback_dialogue_flag == 0 && game_state.date_girl == GIRL_AYAKO && talk_choice == 2) {
        strcpy(name_code_w, tbl.s[1]);
    }
    if (talk_choice == 0) {
        game_state.param_liberal_arts.value += 0xA;
        game_state.param_science.value += 0xA;
        game_state.current_girl = GIRL_SOTOI;
    } else if (talk_choice == 1) {
        game_state.param_condition.value += 0x19;
        game_state.current_girl = GIRL_SOTOI;
    }
    check_para_limit();
    mode_step_next();
}

void new_year_shrine_choice_selector_adjust(void) {
    u8 sel = new_year_omikuji_scene_selector; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 2:
        break;
    case 0:
        new_year_dialogue_group += 3;
        break;
    case 1:
        new_year_dialogue_group += 2;
        break;
    }
    mode_step_next();
}

void new_year_shrine_status_choice_adjust(void) {
    s32 v;

    if (new_year_fallback_dialogue_flag == 0) {
        /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
        v = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
        if ((v != 0) && (v != 1)) {
            if (v != 2) {
                new_year_dialogue_group += 2;
            } else {
                new_year_dialogue_group += 1;
            }
        }
    }
    mode_step_next();
}

void new_year_shrine_status_choice_adjust_alt(void) {
    s32 v;

    if (new_year_fallback_dialogue_flag == 0) {
        /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
        v = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
        switch (v) {
        case 0:
        case 1:
            new_year_dialogue_group += 2;
            break;
        case 2:
            new_year_dialogue_group += 1;
            break;
        }
    } else {
        new_year_dialogue_group += 2;
    }
    mode_step_next();
}

void new_year_shrine_choice_step_skip(void) {
    if (talk_choice != 2) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

/* FAKE: empty do loops and size ^ 0 preserve the two original multiplications
 * and temporary allocation; keep statement-per-line layout. Source unknown.
 * The word-argument prototype override above is required here. T-9520. */
void new_year_shrine_selected_girl_reward_apply(void)
{
    u32 size;
    s32 index;
    talk_names_set(talk_choice);
    size = 0x38;
    index = talk_choice;
    do { } while (0);
    ((GameState *) (((u32) (&game_state)) + (index * size)))->girls[0].tokimeki += 5;
    ((GameState *) (((u32) (&game_state)) + (index * (size ^ 0))))->girls[0].affection += 1;
    do { check_para_limit(size, index); } while (0);
    game_state.current_girl = GIRL_SOTOI;
    mode_step_next();
}

void new_year_shrine_choice_save(void) {
    talk_choice_saved = talk_choice;
    mode_step_next();
}

/* FAKE: passing girl to talk_names_set colors the entry range to $a0, and the outer * 0 subtree reserves $v1 so game_state.current_girl is promoted to $a0 after get_g_zyotai_s without emitting instructions in ugen. T-24137 */
void new_year_shrine_date_girl_restore(void) {
    u32 state;
    s32 girl;

    girl = game_state.date_girl;
    game_state.current_girl = girl;
    talk_names_set(girl);
    talk_choice = talk_choice_saved;
    state = (u8)get_g_zyotai_s(GIRL_NOZOMI) & 0x7F;
    if ((((((new_year_dialogue_group * 0) + (new_year_dialogue_group * 0) + (new_year_dialogue_group * 0)) * 0) + game_state.current_girl) == ((game_state.current_girl * 0) + 6)) && (state < 2) && (new_year_fallback_dialogue_flag == 0)) {
        new_year_dialogue_text_table = D_80143A60;
        new_year_dialogue_command_table = D_80143A9C;
        new_year_dialogue_voice_table = D_80143AD8;
    }
    mode_step_next();
}
