/* swarm_level_flow.c: src/ovl/TACO/80138680.c:4 dispatches playable-level steps; :39 invokes shooter_update_stage3_script; :56 changes mode_stage to 0x20; swarm script bodies src/ovl/shooter_3d/swarm_and_orbit_waves.c:4 and :64. Original object 0x80138680-0x80138890 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_stage_stage3(void) {
    if ((u32)game_state.mode_stage_timer < 0x60 && (game_state.frame_count & 0x10)) {
        shooter_draw_text("3RD STAGE", -0x40, 0, 2);
    }
    switch (game_state.mode_step) {
    case 0:
        shooter_stage3_step_init();
        break;
    case 1:
        shooter_stage3_step_run();
        break;
    case 2:
        shooter_stage3_step_end();
        break;
    }
    shooter_draw_hud();
    shooter_update_battle();
    shooter_check_battle_end();
}

void shooter_stage3_step_init(void) {
    game_state.clear_color_r = 8;
    game_state.clear_color_g = 0x10;
    game_state.clear_color_b = 0x60;
    shooter_background_flags = 4;
    shooter_reset_enemies();
    shooter_reset_ship_state();
    shooter_clear_player_shots();
    shooter_load_request = SHOOTER_LOAD_SOUND_BANK_STAGE3;
    shooter_sound_bank_wait_state = 0;
    audio_bgm_play_short(0, 0x200);
    mode_step_next();
}

void shooter_stage3_step_run(void) {
    shooter_update_stage3_script();
    if ((shooter_load_request == 0) && (shooter_sound_bank_wait_state == 0)) {
        shooter_sound_bank_wait_state = 1;
    }
    if (shooter_sound_bank_wait_state == 1) {
        shooter_sound_bank_poll_result = audio_bank_transfer_poll();
        if (shooter_sound_bank_poll_result == 1) {
            shooter_sound_bank_wait_state = 2;
        }
    }
}

void shooter_stage3_step_end(void) {
    if (shooter_load_request == 0) {
        shooter_load_request = SHOOTER_LOAD_BOSS3;
        shooter_reset_enemies();
        audio_bgm_play_short(1, 0x200);
        mode_stage_set(0x20);
    }
}
