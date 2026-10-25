/* game_mode_dispatch.c: src/ovl/TACO/80132000.c:4: per-frame timer and stage selector dispatch the overlay handlers; body 5: game_state.mode_stage_timer += 1; 6: switch (game_state.mode_stage) {. Original object 0x80132000-0x80132220 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

void shooter_entry(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0:
        shooter_stage_load();
        break;
    case 1:
        shooter_stage_opening();
        break;
    case 2:
        shooter_stage_title();
        break;
    case 3:
        shooter_stage_menu();
        break;
    case 4:
        shooter_stage_settings();
        break;
    case 5:
        shooter_stage_scores();
        break;
    case 6:
        shooter_stage_demo();
        break;
    case 7:
        shooter_stage_how_to_play();
        break;
    case 10:
        shooter_stage_game_start();
        break;
    case 11:
        shooter_stage_stage1();
        break;
    case 12:
        shooter_stage_boss1();
        break;
    case 21:
        shooter_stage_stage2();
        break;
    case 22:
        shooter_stage_boss2();
        break;
    case 31:
        shooter_stage_stage3();
        break;
    case 32:
        shooter_stage_boss3();
        break;
    case 8:
        shooter_stage_result();
        break;
    case 9:
        shooter_stage_name_entry();
        break;
    case 45:
        shooter_stage_game_end();
        break;
    case 46:
        shooter_stage_exit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    shooter_frame_update();
    shooter_draw_models();
}

void shooter_palette_reset(void) {
    func_80058F9C(0x8018A000, 0, 0);
    func_80058F9C(0x8018A000, 1, 0);
    func_80058F9C(0x8018A000, 2, 0);
    func_80058F9C(0x8018A000, 3, 0);
    func_80058F9C(0x8018A000, 4, 0);
}
