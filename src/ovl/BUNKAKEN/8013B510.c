#define MAIN_API_OVERRIDE_talk_names_set /* T-24134: matched caller passes a word; callee narrows it. */
#define MAIN_API_OVERRIDE_portrait_layout_set /* T-24134: matched caller passes a word; callee narrows it. */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void talk_names_set();
void portrait_layout_set();
s32 func_8013BD84(void);
void func_8013BFA0(void);

void func_8013B510(void) {
    switch (game_state.mode_step) {
    case 0:
        if (func_8013BD84() != 0) {
            mode_step_next();
        } else {
            mode_stage_set(1);
        }
        break;
    case 1:
        portrait_read_step(D_80155EA0[D_80122EBC]);
        break;
    case 2:
        addr_init_bustup();
        if (D_80122EBC == 0xB) {
            game_state.current_girl = GIRL_SHIORI;
        } else {
            game_state.current_girl = D_80122EBC;
        }
        portrait_show_checked();
        cd_read_sectors(D_80155F00[D_80122EBC], 0x801E8000, D_80155F0C[D_80122EBC]);
        break;
    case 3:
        normal_date_girl_in();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (D_80122EBC == 0xB) {
                talk_names_set(0);
            } else {
                /* FAKE: self-store preserves the original store in the call delay slot. T-24134. */
                talk_names_set(D_80122EBC = D_80122EBC);
            }
            D_80155ED0[D_80122EBC]();
            func_8013BFA0();
            talk_group = 0;
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        switch (D_80122EBC) {
        case 0:
            mode_step_set(0x11);
            break;
        case 1:
        case 2:
        case 3:
            mode_step_set(0x20);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            mode_step_set(7);
            break;
        }
        break;
    case 7:
        normal_date_two_select();
        break;
    case 8:
        if (D_80122CDC != 0) {
            portrait_layout_set(D_80155F3C[D_80122EBC]);
            func_80063668();
            if (D_80122EBC == 0xB) {
                D_80122EA4 = 0;
            } else {
                D_80122EA4 = D_80122EBC;
            }
        }
        mode_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (D_80122CDC == 0) {
            if ((D_80122EBC == 8) || (D_80122EBC == 9)) {
                mode_step_set(0xE);
            } else {
                mode_stage_set(3);
            }
        } else {
            mode_step_next();
        }
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_stage_set(1);
        break;
    case 14:
        talk_group = 8;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        mode_stage_set(3);
        break;
    case 17:
        /* The halfword conversion preserves the original temporary order. T-24134. */
        talk_group = (s16) ((u8) game_state.girls[0].flags.b[2] >> 4) + 1;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        game_state.girls[0].flags.f.b2 = 1;
        talk_group = 7;
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_two_select();
        break;
    case 22:
        if (D_80122CDC != 0) {
            portrait_layout_set(3);
            func_80063668();
        }
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (D_80122CDC != 0) {
            mode_step_set(0x1C);
        } else {
            /* The halfword conversion preserves the original temporary order. T-24134. */
            talk_group = (s16) ((u8) game_state.girls[0].flags.b[2] >> 4) + 9;
            mode_step_next();
        }
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        D_80122EB8 = (u8) game_state.girls[0].flags.b[2] >> 4;
        D_80122EBC = 0xFF;
        mode_stage_set(4);
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        talk_group = 0xF;
        D_80122EA4 = 0;
        mode_step_next();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        mode_stage_set(1);
        break;
    case 32:
        switch (D_80122EBC) {
        case 1:
            if (((u8) game_state.girls[1].flags.b[2] >> 4) == 0) {
                talk_group = 1;
            } else {
                talk_group = 2;
            }
            break;
        case 2:
            if (((u8) game_state.girls[2].flags.b[2] >> 4) == 2) {
                talk_group = 1;
            } else {
                talk_group = 2;
            }
            break;
        case 3:
            if (((u8) game_state.girls[3].flags.b[2] >> 4) == 4) {
                talk_group = 1;
            } else {
                talk_group = 2;
            }
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        game_state.girls[D_80122EBC].flags.f.b2 = 1;
        mode_step_next();
        break;
    case 33:
        normal_date_speak();
        break;
    case 34:
        talk_group = 3;
        mode_step_next();
        break;
    case 35:
        normal_date_speak();
        break;
    case 36:
        normal_date_two_select();
        break;
    case 37:
        if (D_80122CDC != 0) {
            if (D_80122EBC == 2) {
                portrait_layout_set(4);
            } else {
                portrait_layout_set(3);
            }
            func_80063668();
        }
        mode_step_next();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        if (D_80122CDC != 0) {
            mode_step_set(0x2B);
        } else {
            switch (D_80122EBC) {
            case 1:
                if (((u8) game_state.girls[1].flags.b[2] >> 4) == 0) {
                    talk_group = 5;
                } else {
                    talk_group = 6;
                }
                break;
            case 2:
                if (((u8) game_state.girls[2].flags.b[2] >> 4) == 2) {
                    talk_group = 5;
                } else {
                    talk_group = 6;
                }
                break;
            case 3:
                if (((u8) game_state.girls[3].flags.b[2] >> 4) == 4) {
                    talk_group = 5;
                } else {
                    talk_group = 6;
                }
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            mode_step_next();
        }
        break;
    case 40:
        normal_date_girl_out();
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        D_80122EB8 = (u8) game_state.girls[D_80122EBC].flags.b[2] >> 4;
        D_80122EBC = 0xFF;
        mode_stage_set(4);
        break;
    case 43:
        normal_date_girl_out();
        break;
    case 44:
        talk_group = 7;
        D_80122EA4 = D_80122EBC;
        mode_step_next();
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        mode_stage_set(1);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

s32 func_8013BD84(void) {
    s32 i;
    s32 best;
    s16 max;
    s16 v;
    u8 flags;

    best = 0;
    if ((get_h_yuukou(GIRL_SHIORI) >= 0x32U) && (get_h_yuukou(GIRL_SHIORI) >= 0x50U)) {
        max = game_state.girls[0].affection + game_state.girls[0].tokimeki;
    } else {
        max = 0;
    }
    flags = game_state.girls[0].flags.b[2];
    if (game_state.year == 0x60) {
        if (((flags >> 4) == 4) || ((flags >> 4) == 1)) {
            max = 0;
        }
    }
    for (i = 1; i < 11; i++) {
        if ((get_h_tokimeki(i) >= 0x32U) && (get_h_yuukou(i) >= 0x50U) && (game_state.girls[i].flags.f.met == 1)) {
            v = game_state.girls[i].affection + game_state.girls[i].tokimeki;
            if (game_state.year == 0x60) {
                if ((((u8) game_state.girls[i].flags.b[2] >> 4) == 4) || (((u8) game_state.girls[i].flags.b[2] >> 4) == 1)) {
                    v = 0;
                }
            }
            if (max < v) {
                best = i;
                max = v;
            }
        }
    }
    if ((get_h_tokimeki(best) >= 0x32U) && (get_h_yuukou(best) >= 0x50U)) {
        flags = game_state.girls[0].flags.b[2];
        if ((best == 0) && (game_state.year == 0x60)) {
            if (((flags >> 4) == 4) || ((flags >> 4) == 1)) {
                return 0;
            }
        }
        if ((best == 0) && ((flags >> 4) >= 6U)) {
            best = 11;
        }
        D_80122EBC = best;
        return 1;
    }
    return 0;
}


void func_8013BFA0(void) {
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
}
