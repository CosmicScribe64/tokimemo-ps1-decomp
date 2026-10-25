/* formation_level_flow.c: src/ovl/TACO/80138450.c:4 dispatches playable-level steps; :42 invokes shooter_update_stage2_script; :59 changes mode_stage to 0x16; formation script bodies src/ovl/shooter_3d/formation_and_circling_waves.c:4 and :199. Original object 0x80138450-0x80138680 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_stage_stage2(void) {
    if ((u32)game_state.mode_stage_timer < 0x60 && (game_state.frame_count & 0x10)) {
        shooter_draw_text((s32)"2ND STAGE", -0x40, 0, 2);
    }
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        shooter_stage2_step_init();
        break;
    case 1:
        shooter_stage2_step_run();
        break;
    case 2:
        shooter_stage2_step_end();
        break;
    }
    shooter_draw_hud();
    shooter_update_battle();
    shooter_check_battle_end();
}

void shooter_stage2_step_init(void) {
    audio_bgm_play_short(0, 0x203);
    audio_command(0x24);
    shooter_set_white_lights();
    shooter_reset_ship_state();
    shooter_clear_player_shots();
    game_state.clear_color_r = 0x78;
    game_state.clear_color_g = 0xA8;
    game_state.clear_color_b = 0xF0;
    shooter_background_flags = 3;
    shooter_load_request = SHOOTER_LOAD_SOUND_BANK_STAGE2;
    shooter_sound_bank_wait_state = 0;
    shooter_reset_enemies();
    mode_step_next();
}

void shooter_stage2_step_run(void) {
    shooter_update_stage2_script();
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

void shooter_stage2_step_end(void) {
    if (shooter_load_request == 0) {
        shooter_reset_enemies();
        shooter_load_request = SHOOTER_LOAD_BOSS2;
        audio_bgm_play_short(1, 0x201);
        mode_stage_set(0x16);
    }
}
