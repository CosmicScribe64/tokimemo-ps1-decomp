/* game_start.c: src/ovl/TACO/80137F90.c:20 resets score, timer, models and player records; :56 uploads texture pages after load completion; :67 starts playable level by setting mode_stage to 0xB. Original object 0x80137F90-0x80138280 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_stage_game_start(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        shooter_game_start_step_init();
        break;
    case 1:
        shooter_game_start_step_wait();
        break;
    case 2:
        shooter_game_start_step_begin();
        break;
    }
    shooter_draw_hud();
    shooter_update_battle();
}
void shooter_game_start_step_init(void) {
    s32 var_s0;

    shooter_init_stars();
    shooter_star_brightness = 0x80;
    shooter_stages_cleared = 0;
    shooter_score = 0;
    shooter_background_flags = 0x80;
    shooter_time_left = 0x4650;
    shooter_clear_particle_tables();
    shooter_load_request = 0x10;
    game_state.clear_color_r = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_b = 0;
    shooter_play_se(0, 0x503);
    view_display_set_wide_or_standard(1);
    k_reset(0);
    back_clear_switch(1);
    shooter_init_camera();
    shooter_set_white_lights();
    view_model_slots_clear();
    shooter_hide_all_models();
    shooter_release_all_actor_models();
    /* FAKE: shared source line schedules the loop setup (T-9780). */
    var_s0 = 0; do {
        /* FAKE: integer address arithmetic preserves index-first addu (T-9780). */
        func_80099E30(0, (void *)(var_s0 + (u32)shooter_actors + 0x14));
        shooter_init_actor_model(0x801AC000, 0, var_s0 + (u32)shooter_actors);
        var_s0 += 0x88;
    } while (var_s0 != 0x29F8);
    shooter_init_ships();
    shooter_reset_ship_state();
    shooter_clear_player_shots();
    mode_step_next();
}

void shooter_game_start_step_wait(void) {
    shooter_update_stars(1, 1);
    if (shooter_load_request == 0) {
        audio_bgm_play_short(0, 0x203);
        load_csr_tp(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
        load_csr_tp(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
        load_csr_tp(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
        mode_step_next();
    }
}

s32 shooter_game_start_step_begin(void) {
    shooter_time_left = 0x4650;
    shooter_score = 0;
    shooter_actors->stats[1] = 0xFF;
    shooter_actors[8].stats[1] = 0xFF;
    mode_stage_set(0xB);
}
