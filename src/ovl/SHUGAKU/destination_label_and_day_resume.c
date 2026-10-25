/* destination_label_and_day_resume.c: src/ovl/SHUGAKU/8013AA70.c:4-58; invokes exit callbacks, fills destination label, restores HUD/audio, advances the calendar, applies trip relationship deltas and resumes day events; not callback-table initialization. Original object 0x8013AA70-0x8013AD10 (config/objects/SHUGAKU.txt). */
#include "common.h"
#include "ovl/SHUGAKU.h"

void trip_destination_exit_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripSceneCallbacks tbl;

    tbl = D_8013CC20;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void trip_location_outcome_update(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        strcpy(name_code_s, "京都");
        break;
    case 1:
        strcpy(name_code_s, "沖縄");
        break;
    case 2:
        strcpy(name_code_s, "北海道");
        break;
    }
    D_800CA2DC += D_800CA2F4 * (1 - D_800CA2EC);
    mode_step_next();
}

s32 trip_day_event_complete(void) {
    if (game_state.substep == 0) {
        dec_bg_reset();
        hud_day_bgm_load();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if ((u32)game_state.mode_step_timer.w++ >= 0x400U) {
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
    calendar_advance_day();
    if (D_800CA2EC != 0) {
        game_state.girls[10].affection += 1;
    } else {
        game_state.girls[game_state.current_girl].tokimeki += 1;
        game_state.girls[game_state.current_girl].affection += 1;
    }
    check_para_limit();
    return day_event_next(1);
}
