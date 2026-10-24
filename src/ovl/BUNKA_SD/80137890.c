#include "common.h"
#include "ovl/BUNKA_SD.h"

/* Implicit int (K&R), no return value: the switch copy in $v0 stays live as the result. */
s32 func_80137890(void) {
    switch (*(u8 *)&game_state.mode_stage) { /* FAKE: T-22003, scalar view keeps the selector in $v1 with the $v0 switch copy */
    case 0x0:
        hud_screen_frame_draw(2);
        hizuke_disp_switch(0);
        message_window_show();
        mode_stage_next();
        break;
    case 0x1:
        switch (D_80122EB8) {
        case 2:
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
        case 3:
            mode_stage_set(0xF3);
            break;
        case 5:
            mode_stage_set(0xF4);
            break;
        case 0:
            mode_stage_set(0xF5);
            break;
        case 1:
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
        func_801362EC();
        break;
    case 0xF1:
        func_801351DC();
        break;
    case 0xF2:
        func_80135A2C();
        break;
    case 0xF3:
        func_801345BC();
        break;
    case 0xF4:
        func_80138ADC();
        break;
    case 0xF5:
        func_801322EC();
        break;
    case 0xF6:
        func_801336AC();
        break;
    case 0xF7:
        func_80137CAC();
        break;
    case 0xF8:
        func_80136B3C();
        break;
    }
}
