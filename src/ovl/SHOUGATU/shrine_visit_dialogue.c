/* shrine_visit_dialogue.c: ; src/ovl/SHOUGATU/80133D10.c:14-34 initializes the shrine-visit title and dialogue state. Original object 0x80133D10-0x80134120 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void new_year_shrine_dialogue_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before shrine_dialogue_callbacks: its stack slot sits above shrine_dialogue_callbacks (T-3330) */
    FnTbl23 shrine_dialogue_callbacks;

    shrine_dialogue_callbacks = D_80143CC0;
    mode_step_index = game_state.mode_step;
    shrine_dialogue_callbacks.f[mode_step_index](0x80);
}

/* FAKE: the (u16) cast on the first store and `var_v0 = var_v0;` keep the redundant second store and the original's temporary numbering (t1); found with the permuter, T-9380 */
void new_year_shrine_dialogue_init(void) {
    s16 var_v0;

    strcpy(scene_place_title, "初詣");
    var_v0 = (D_80143B20 != 0) ? 0 : 0xF;
    if (D_80143B18 == 0) {
        var_v0 += 2;
    }
    D_80143B00 = (u16)var_v0;
    if (D_80143B18 == 0) {
        var_v0 = var_v0;
        D_80143B00 = var_v0;
        if (game_state.current_girl == GIRL_YUKARI) {
            var_v0 = 2 + var_v0;
            D_80143B00 = var_v0;
        }
    }
    mode_step_next();
}

void new_year_shrine_talk_selector_gate(void) {
    if (D_80143B20 != 0) {
        new_year_talk_run_current();
        return;
    }
    mode_step_next();
}

void new_year_shrine_outcome_branch_action(void) {
    if (D_80122CDC != 0) {
        face_expression_3();
        return;
    }
    face_expression_1();
}

void new_year_shrine_log_and_background_load(void) {
    if ((D_80143B20 == 0) && (((u32) game_state.frame_count % 3U) == 0)) {
        if (D_80143B24 != 0) {
            event_log_add(5, 3);
        } else {
            event_log_add(5, 0);
        }
    }
    bg_read_sub2(0x41ED);
    mode_step_next();
}

void new_year_shrine_yukari_variant_transition(void) {
    if ((game_state.current_girl != GIRL_YUKARI) || (D_80143B20 != 0) || (D_80143B18 == 0)) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}

void new_year_shrine_conditional_step_skip(void) {
    if (D_80143B20 != 0) {
        game_state.mode_step += 4;
    }
    mode_step_next();
}

void new_year_shrine_talk_state_gate(void) {
    if (D_80143B18 == 0) {
        mode_step_next();
        return;
    }
    new_year_talk_run_current();
}

void new_year_shrine_yukari_talk_gate(void) {
    if ((game_state.current_girl == GIRL_YUKARI) && (D_80143B18 == 0)) {
        new_year_talk_run_current();
        return;
    }
    mode_step_next();
}

void new_year_shrine_talk_variant_gate(void) {
    if (D_80143B18 != 0) {
        new_year_talk_run_current();
        return;
    }
    mode_step_next();
}

void new_year_shrine_outcome_parameters_apply(void) {
    if (D_80122CDC != 0) {
        game_state.girls[5].affection -= 1;
        game_state.girls[5].tokimeki -= 1;
        game_state.girls[5].syoushin += 0xA;
    } else {
        game_state.girls[5].tokimeki += 1;
    }
    check_para_limit();
    mode_step_next();
}
