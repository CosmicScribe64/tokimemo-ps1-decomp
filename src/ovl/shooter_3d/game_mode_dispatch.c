/* game_mode_dispatch.c: src/ovl/TACO/80132000.c:4: per-frame timer and stage selector dispatch the overlay handlers; body 5: game_state.mode_stage_timer += 1; 6: switch (game_state.mode_stage) {. Original object 0x80132000-0x80132220 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"

void shooter_entry(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case SHOOTER_STAGE_LOAD:
        shooter_stage_load();
        break;
    case SHOOTER_STAGE_OPENING:
        shooter_stage_opening();
        break;
    case SHOOTER_STAGE_TITLE:
        shooter_stage_title();
        break;
    case SHOOTER_STAGE_MENU:
        shooter_stage_menu();
        break;
    case SHOOTER_STAGE_SETTINGS:
        shooter_stage_settings();
        break;
    case SHOOTER_STAGE_SCORES:
        shooter_stage_scores();
        break;
    case SHOOTER_STAGE_DEMO:
        shooter_stage_demo();
        break;
    case 7:
        shooter_stage_how_to_play();
        break;
    case SHOOTER_STAGE_GAME_START:
        shooter_stage_game_start();
        break;
    case SHOOTER_STAGE_STAGE1:
        shooter_stage_stage1();
        break;
    case SHOOTER_STAGE_BOSS1:
        shooter_stage_boss1();
        break;
    case SHOOTER_STAGE_STAGE2:
        shooter_stage_stage2();
        break;
    case SHOOTER_STAGE_BOSS2:
        shooter_stage_boss2();
        break;
    case SHOOTER_STAGE_STAGE3:
        shooter_stage_stage3();
        break;
    case SHOOTER_STAGE_BOSS3:
        shooter_stage_boss3();
        break;
    case SHOOTER_STAGE_RESULT:
        shooter_stage_result();
        break;
    case SHOOTER_STAGE_NAME_ENTRY:
        shooter_stage_name_entry();
        break;
    case SHOOTER_STAGE_GAME_END:
        shooter_stage_game_end();
        break;
    case SHOOTER_STAGE_EXIT:
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
    view_link_object(0x8018A000, 0, 0);
    view_link_object(0x8018A000, 1, 0);
    view_link_object(0x8018A000, 2, 0);
    view_link_object(0x8018A000, 3, 0);
    view_link_object(0x8018A000, 4, 0);
}
