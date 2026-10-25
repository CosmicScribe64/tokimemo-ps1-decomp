/* club_year_show_dispatch.c: src/ovl/BUNKA_SD/80137890.c:4-91; reads club selector and calendar year at14-51, dispatches show callbacks at63-89 and restores the caller overlay mode/stage/step. Original object 0x80137890-0x80137B40 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "ovl/BUNKA_SD.h"

/* Implicit int (K&R), no return value: the switch copy in $v0 stays live as the result. */
s32 stage_show_club_year_show_mode_stage_dispatch(void) {
    switch (*(u8 *)&game_state.mode_stage) { /* FAKE: T-22003, scalar view keeps the selector in $v1 with the $v0 switch copy */
    case 0x0:
        hud_screen_frame_draw(2);
        hizuke_disp_switch(0);
        message_window_show();
        mode_stage_next();
        break;
    case 0x1:
        switch (D_80122EB8) {
        case CLUB_SCIENCE:
            switch (game_state.year) {
            case 0x5F:
                mode_stage_set(0xF0);
                break;
            case 0x60:
                mode_stage_set(0xF1);
                break;
            case 0x61:
                mode_stage_set(0xF2);
                break;
            }
            break;
        case CLUB_COMPUTER:
            mode_stage_set(0xF3);
            break;
        case CLUB_BRASS_BAND:
            mode_stage_set(0xF4);
            break;
        case CLUB_LITERATURE:
            mode_stage_set(0xF5);
            break;
        case CLUB_DRAMA:
            switch (game_state.year) {
            case 0x5F:
                mode_stage_set(0xF6);
                break;
            case 0x60:
                mode_stage_set(0xF7);
                break;
            case 0x61:
                mode_stage_set(0xF8);
                break;
            }
            break;
        default:
            break;
        }
        break;
    case 0x10:
        if ((u32) ((u16) game_state.player_flags.h >> 0xC) < 6U) {
            game_mode_set(GAME_MODE_CULTURE_FESTIVAL_A);
        } else {
            game_mode_set(GAME_MODE_CULTURE_FESTIVAL_B);
        }
        mode_stage_set((u8)D_80122EB0);
        mode_step_set((u8)D_80122EB4);
        break;
    case 0xF0:
        stage_show_science_first_show_input_update();
        break;
    case 0xF1:
        stage_show_science_second_show_input_update();
        break;
    case 0xF2:
        stage_show_science_final_show_input_update();
        break;
    case 0xF3:
        stage_show_computer_show_input_update();
        break;
    case 0xF4:
        stage_show_brass_band_input_update();
        break;
    case 0xF5:
        stage_show_input_update();
        break;
    case 0xF6:
        stage_show_scene_a_input_update();
        break;
    case 0xF7:
        stage_show_scene_c_input_update();
        break;
    case 0xF8:
        stage_show_scene_b_input_update();
        break;
    }
}
