/* valentine_home_scene.c: ; src/ovl/VALEN/801339D0.c:22-38 sets the front-of-home location and handles its transition. Original object 0x801339D0-0x80133C70 (config/objects/VALEN.txt). */
#include "ovl/valentine.h"

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_80134630;

void valentine_return_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_80134630;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void valentine_home_place_setup(void) {
    if (strcmp(scene_place_title_2, "自宅前") == 0) {
        mode_step_next();
        return;
    }
    strcpy(scene_place_title_2, "自宅前");
    place_init2();
}

s32 valentine_day_event_complete(void) {
    if (game_state.substep == 0) {
        hud_day_bgm_load();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if (game_state.mode_step_timer.u++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    return day_event_next(1);
}

void valentine_candidate_result_route(void) {
    if (valentine_unmet_megumi_selected != 0) {
        game_state.girls[9].flags.f.met = 1;
    }
    if (valentine_candidate_count >= 7) {
        valentine_talk_group = 0;
    } else if (valentine_candidate_count >= 5) {
        valentine_talk_group = 1;
    } else if (valentine_candidate_count >= 3) {
        valentine_talk_group = 2;
    } else if (valentine_candidate_count > 0) {
        valentine_talk_group = 3;
    } else {
        valentine_talk_group = 4;
    }
    valentine_talk_text_table = D_801343F4;
    valentine_talk_command_table = D_80134428;
    valentine_talk_voice_table = D_8013445C;
    mode_step_next();
}

void valentine_background_event_log(void) {
    bg_read_sub2(0x414C);
    mode_step_next();
}
