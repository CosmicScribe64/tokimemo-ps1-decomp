/* swoop_level_flow.c: src/ovl/TACO/80138280.c:4 dispatches playable-level steps; :34 invokes shooter_update_stage1_script; :50 changes mode_stage to 0xC; swoop/debris script bodies src/ovl/TACO/swoop_and_debris_waves.c:4 and :174. Original object 0x80138280-0x80138450 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_stage_stage1(void) {
    if ((u32) game_state.mode_stage_timer < 0x60 && (game_state.frame_count & 0x10)) {
        shooter_draw_text("1ST STAGE", -0x40, 0, 2);
    }
    switch (game_state.mode_step) {
    case 0:
        shooter_stage1_step_init();
        break;
    case 1:
        shooter_stage1_step_run();
        break;
    case 2:
        shooter_stage1_step_end();
        break;
    }
    shooter_update_stars(1, 1);
    shooter_draw_hud();
    shooter_update_battle();
    shooter_check_battle_end();
}

void shooter_stage1_step_init(void) {
    shooter_load_request = 0x14;
    shooter_sound_bank_wait_state = 0;
    shooter_background_flags = 0x80;
    shooter_reset_enemies();
    mode_step_next();
}

void shooter_stage1_step_run(void) {
    shooter_update_stage1_script();
    if ((shooter_load_request == 0) && (shooter_sound_bank_wait_state == 0)) {
        shooter_sound_bank_wait_state = 1;
    }
    if (shooter_sound_bank_wait_state == 1) {
        shooter_sound_bank_poll_result = func_80044C98();
        if (shooter_sound_bank_poll_result == 1) {
            shooter_sound_bank_wait_state = 2;
        }
    }
}

void shooter_stage1_step_end(void) {
    if (shooter_load_request == 0) {
        shooter_load_request = 0x18;
        func_800450F4(1, 0x200);
        mode_stage_set(0xC);
    }
}
