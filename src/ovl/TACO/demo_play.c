/* demo_play.c: src/ovl/TACO/80137590.c:20 initializes demo actors and starts shared enemy script; :84 exits on user input or synthesizes held fire and timed directional pad input. Original object 0x80137590-0x80137A60 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

s32 shooter_stage_demo(void) {
    switch (game_state.mode_step) {
    case 0:
        shooter_demo_step_init();
        break;
    case 1:
        shooter_demo_step_run();
        break;
    }
    if (game_state.frame_count & 0x10) {
        shooter_draw_text("DEMO PLAY", -0x40, 0, 2);
    }
}

/* s32 without a return (v0 live at the exit) and the u8 view of the selector reproduce the original switch; the empty
   default adds the basic blocks that keep the 0x801AC000 constant out of a third s register. T-16561 */
s32 shooter_demo_step_init(void) {
    s32 i;
    s32 end;

    switch (*(u8 *)&game_state.substep) {
    case 0:
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
        func_800591D8(1);
        k_reset(0);
        back_clear_switch(1);
        shooter_init_camera();
        shooter_set_white_lights();
        func_80059048();
        shooter_hide_all_models();
        shooter_release_all_actor_models();
        i = 0;
        end = 0x29F8;
        do {
            /* FAKE: the (u8 *)i + (u32)ptr form puts the index first in the addu, as the original; plain ptr + i puts the pointer first. T-16561 */
            func_80099E30(0, (u8 *)i + (u32)shooter_actors + 0x14);
            shooter_init_actor_model(0x801AC000, 0, (u8 *)i + (u32)shooter_actors);
            i += 0x88;
        } while (i != end);
        shooter_init_ships();
        shooter_reset_ship_state();
        shooter_clear_player_shots();
        D_8011ED17 &= 0x7F;
        game_state.substep += 1;
        break;
    case 1:
        if (shooter_load_request == 0) {
            shooter_reset_enemies();
            shooter_background_flags = 0x80;
            shooter_actors[0].stats[1] = 0xFF;
            shooter_actors[8].stats[1] = 0xFF;
            switch (game_state.frame_count & 3) {
            case 0:
                shooter_actors[0].weapon = 0;
                break;
            case 1:
                shooter_actors[0].weapon = 1;
                break;
            }
            load_csr_tp(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
            load_csr_tp(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
            load_csr_tp(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
            mode_step_next();
        }
        break;
    default:
        break;
    }
}

void shooter_demo_step_run(void) {
    u32 t;

    shooter_update_stage1_script();
    if (game_state.pad_pressed != 0) {
        shooter_release_all_actor_models();
        shooter_hide_all_models();
        func_80059048();
        shooter_reset_enemies();
        func_800591D8(0);
        mode_stage_set(2);
    } else {
        game_state.pad_pressed |= PAD_CIRCLE;
        t = game_state.mode_stage_timer & 0x1FF;
        game_state.pad_held |= PAD_CIRCLE;
        if (t >= 0x79 && t < 0x8C) {
            game_state.pad_held |= PAD_RIGHT;
        }
        if (t >= 0xAB && t < 0xBE) {
            game_state.pad_held |= PAD_DOWN;
        }
        if (t >= 0xDD && t < 0xF0) {
            game_state.pad_held |= PAD_LEFT;
        }
        if (t >= 0x10F && t < 0x122) {
            game_state.pad_held |= PAD_UP;
        }
        if (t >= 0x191 && t < 0x1B8) {
            game_state.pad_held |= 0x3000;
        }
        if (t >= 0x1CD && t < 0x1E0) {
            game_state.pad_held |= 0xC000;
        }
        if (t >= 0x191 && t < 0x1B8) {
            game_state.pad_held |= 0x6000;
        }
        if (t >= 0x1CD && t < 0x1E0) {
            game_state.pad_held |= 0x9000;
        }
        shooter_update_stars(1, 1);
        shooter_draw_hud();
        shooter_update_battle();
    }
    if ((u32) game_state.mode_stage_timer >= 0x709U) {
        shooter_release_all_actor_models();
        shooter_hide_all_models();
        func_80059048();
        shooter_reset_enemies();
        func_800591D8(0);
        mode_stage_set(2);
    }
}
