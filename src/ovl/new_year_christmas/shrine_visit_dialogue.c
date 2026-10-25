/* shrine_visit_dialogue.c: ; src/ovl/SHOUGATU/80133D10.c:14-34 initializes the shrine-visit title and dialogue state. Original object 0x80133D10-0x80134120 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_shrine_dialogue_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before shrine_dialogue_callbacks: its stack slot sits above shrine_dialogue_callbacks (T-3330) */
    FnTbl23 shrine_dialogue_callbacks;

    shrine_dialogue_callbacks = D_80143CC0;
    mode_step_index = game_state.mode_step;
    shrine_dialogue_callbacks.f[mode_step_index](0x80);
}

/* FAKE: the (u16) cast on the first store and `var_v0 = var_v0;` keep the redundant second store and the original's temporary numbering (t1); found with the permuter, T-9380 */
void new_year_shrine_dialogue_init(void) {
    s16 shrine_group;

    strcpy(scene_place_title, "初詣");
    shrine_group = (new_year_fallback_dialogue_flag != 0) ? 0 : 0xF;
    if (new_year_shrine_invitation_flag == 0) {
        shrine_group += 2;
    }
    new_year_dialogue_group = (u16)shrine_group;
    if (new_year_shrine_invitation_flag == 0) {
        shrine_group = shrine_group;
        new_year_dialogue_group = shrine_group;
        if (game_state.current_girl == GIRL_YUKARI) {
            shrine_group = 2 + shrine_group;
            new_year_dialogue_group = shrine_group;
        }
    }
    mode_step_next();
}

void new_year_shrine_talk_selector_gate(void) {
    if (new_year_fallback_dialogue_flag != 0) {
        new_year_talk_run_current();
        return;
    }
    mode_step_next();
}

void new_year_shrine_outcome_branch_action(void) {
    if (talk_choice != 0) {
        face_expression_3();
        return;
    }
    face_expression_1();
}

void new_year_shrine_log_and_background_load(void) {
    if ((new_year_fallback_dialogue_flag == 0) && (((u32) game_state.frame_count % 3U) == 0)) {
        if (new_year_shiori_route_flag != 0) {
            event_log_add(5, 3);
        } else {
            event_log_add(5, 0);
        }
    }
    bg_read_sub2(0x41ED);
    mode_step_next();
}

void new_year_shrine_yukari_variant_transition(void) {
    if ((game_state.current_girl != GIRL_YUKARI) || (new_year_fallback_dialogue_flag != 0) || (new_year_shrine_invitation_flag == 0)) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}

void new_year_shrine_conditional_step_skip(void) {
    if (new_year_fallback_dialogue_flag != 0) {
        game_state.mode_step += 4;
    }
    mode_step_next();
}

void new_year_shrine_talk_state_gate(void) {
    if (new_year_shrine_invitation_flag == 0) {
        mode_step_next();
        return;
    }
    new_year_talk_run_current();
}

void new_year_shrine_yukari_talk_gate(void) {
    if ((game_state.current_girl == GIRL_YUKARI) && (new_year_shrine_invitation_flag == 0)) {
        new_year_talk_run_current();
        return;
    }
    mode_step_next();
}

void new_year_shrine_talk_variant_gate(void) {
    if (new_year_shrine_invitation_flag != 0) {
        new_year_talk_run_current();
        return;
    }
    mode_step_next();
}

void new_year_shrine_outcome_parameters_apply(void) {
    if (talk_choice != 0) {
        game_state.girls[5].affection -= 1;
        game_state.girls[5].tokimeki -= 1;
        game_state.girls[5].syoushin += 0xA;
    } else {
        game_state.girls[5].tokimeki += 1;
    }
    check_para_limit();
    mode_step_next();
}
