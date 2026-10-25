/* invitation.c: src/ovl/BUNKAKEN/8013B510.c:8: stage 0: invitation and invite pick. Original object 0x8013B510-0x8013BFE0 (config/objects/BUNKAKEN.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* T-24134: matched caller passes a word; callee narrows it. */
#define MAIN_API_OVERRIDE_portrait_layout_set /* T-24134: matched caller passes a word; callee narrows it. */
#include "common.h"
#include "ovl/culture_festival_tour.h"

void talk_names_set();
void portrait_layout_set();
s32 tour_invitation_pick_girl(void);
void tour_script_tables_apply(void);

void tour_stage_invitation(void) {
    switch (game_state.mode_step) {
    case 0:
        if (tour_invitation_pick_girl() != 0) {
            mode_step_next();
        } else {
            mode_stage_set(TOUR_STAGE_ASK_COMPANION);
        }
        break;
    case 1:
        portrait_read_step(tour_portrait_lba[game_mode_scene_girl]);
        break;
    case 2:
        addr_init_bustup();
        if (game_mode_scene_girl == 0xB) {
            game_state.current_girl = GIRL_SHIORI;
        } else {
            game_state.current_girl = game_mode_scene_girl;
        }
        portrait_show_checked();
        cd_read_sectors(tour_invite_script_sectors[game_mode_scene_girl], 0x801E8000, tour_invite_script_lba[game_mode_scene_girl]);
        break;
    case 3:
        normal_date_girl_in();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (game_mode_scene_girl == 0xB) {
                talk_names_set(0);
            } else {
                /* FAKE: self-store preserves the original store in the call delay slot. T-24134. */
                talk_names_set(game_mode_scene_girl = game_mode_scene_girl);
            }
            tour_invite_script_setter[game_mode_scene_girl]();
            tour_script_tables_apply();
            talk_group = 0;
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        switch (game_mode_scene_girl) {
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
        if (talk_choice != 0) {
            portrait_layout_set(tour_portrait_layout[game_mode_scene_girl]);
            portrait_face_update();
            if (game_mode_scene_girl == 0xB) {
                declined_companion_girl = 0;
            } else {
                declined_companion_girl = game_mode_scene_girl;
            }
        }
        mode_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (talk_choice == 0) {
            if ((game_mode_scene_girl == 8) || (game_mode_scene_girl == 9)) {
                mode_step_set(0xE);
            } else {
                mode_stage_set(TOUR_STAGE_CLUB_MENU);
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
        mode_stage_set(TOUR_STAGE_ASK_COMPANION);
        break;
    case 14:
        talk_group = 8;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        mode_stage_set(TOUR_STAGE_CLUB_MENU);
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
        if (talk_choice != 0) {
            portrait_layout_set(3);
            portrait_face_update();
        }
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (talk_choice != 0) {
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
        culture_exhibit_club = (u8) game_state.girls[0].flags.b[2] >> 4;
        game_mode_scene_girl = 0xFF;
        mode_stage_set(TOUR_STAGE_ROUTE_VISIT);
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        talk_group = 0xF;
        declined_companion_girl = 0;
        mode_step_next();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        mode_stage_set(TOUR_STAGE_ASK_COMPANION);
        break;
    case 32:
        switch (game_mode_scene_girl) {
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
        game_state.girls[game_mode_scene_girl].flags.f.b2 = 1;
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
        if (talk_choice != 0) {
            if (game_mode_scene_girl == 2) {
                portrait_layout_set(4);
            } else {
                portrait_layout_set(3);
            }
            portrait_face_update();
        }
        mode_step_next();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        if (talk_choice != 0) {
            mode_step_set(0x2B);
        } else {
            switch (game_mode_scene_girl) {
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
        culture_exhibit_club = (u8) game_state.girls[game_mode_scene_girl].flags.b[2] >> 4;
        game_mode_scene_girl = 0xFF;
        mode_stage_set(TOUR_STAGE_ROUTE_VISIT);
        break;
    case 43:
        normal_date_girl_out();
        break;
    case 44:
        talk_group = 7;
        declined_companion_girl = game_mode_scene_girl;
        mode_step_next();
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        mode_stage_set(TOUR_STAGE_ASK_COMPANION);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

s32 tour_invitation_pick_girl(void) {
    s32 i;
    s32 best_girl;
    s16 best_score;
    s16 candidate_score;
    u8 shiori_flags;

    best_girl = 0;
    if ((get_h_yuukou(GIRL_SHIORI) >= 0x32U) && (get_h_yuukou(GIRL_SHIORI) >= 0x50U)) {
        best_score = game_state.girls[0].affection + game_state.girls[0].tokimeki;
    } else {
        best_score = 0;
    }
    shiori_flags = game_state.girls[0].flags.b[2];
    if (game_state.year == 0x60) {
        if (((shiori_flags >> 4) == 4) || ((shiori_flags >> 4) == 1)) {
            best_score = 0;
        }
    }
    for (i = 1; i < 11; i++) {
        if ((get_h_tokimeki(i) >= 0x32U) && (get_h_yuukou(i) >= 0x50U) && (game_state.girls[i].flags.f.met == 1)) {
            candidate_score = game_state.girls[i].affection + game_state.girls[i].tokimeki;
            if (game_state.year == 0x60) {
                if ((((u8) game_state.girls[i].flags.b[2] >> 4) == 4) || (((u8) game_state.girls[i].flags.b[2] >> 4) == 1)) {
                    candidate_score = 0;
                }
            }
            if (best_score < candidate_score) {
                best_girl = i;
                best_score = candidate_score;
            }
        }
    }
    if ((get_h_tokimeki(best_girl) >= 0x32U) && (get_h_yuukou(best_girl) >= 0x50U)) {
        shiori_flags = game_state.girls[0].flags.b[2];
        if ((best_girl == 0) && (game_state.year == 0x60)) {
            if (((shiori_flags >> 4) == 4) || ((shiori_flags >> 4) == 1)) {
                return 0;
            }
        }
        if ((best_girl == 0) && ((shiori_flags >> 4) >= 6U)) {
            best_girl = 11;
        }
        game_mode_scene_girl = best_girl;
        return 1;
    }
    return 0;
}


void tour_script_tables_apply(void) {
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
}
