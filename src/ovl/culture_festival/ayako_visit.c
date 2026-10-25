/* ayako_visit.c: src/ovl/BUNKASAI/8013C430.c:4: Ayako visit block, stages 65-79. Original object 0x8013C430-0x8013FD90 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/culture_festival.h"

void culture_ayako_visit_entry(void) {
    ((CultureFestivalDispatch *)&culture_exhibit_club)->girl = 3;
    switch (((CultureFestivalDispatch *)&culture_exhibit_club)->club_id) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_SPEECH_CONTEST);
            return;
        }
        mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_LITERATURE_EXHIBIT);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_DRAMA_PLAY);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_EXPERIMENT_SHOW);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_COMPUTER_EXPERIMENT_SHOW);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_PORTRAIT_DRAWING);
            return;
        case 0x60:
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_OPEN_SKETCHING);
            return;
        case 0x61:
            mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_OIL_PAINTING_EXHIBIT);
            return;
        }
        break;
    case 5:
        mode_stage_set(CULTURE_STAGE_AYAKO_VISIT_CONCERT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_ayako_visit_literature_exhibit(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            if (g->year == 0x5F) {
                talk_group = 0x24;
            } else {
                talk_group = 0x25;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 1;

                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 4;
                break;
            }
            culture_ayako_visit_script_a_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 5;

            break;
        case 2:
            talk_group = 6;
            break;
        case 3:
            talk_group = 7;
            break;
        case 4:
            talk_group = 8;
            break;
        }
        if (g->year == 0x60) {
            talk_group += 5;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(3) == 4) {
            normal_date_girl_out();
        } else {
            switch (girl_state_rank(3)) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 0xF;
                break;
            case 2:
                talk_group = 0x11;
                break;
            case 3:
                talk_group = 0x13;
                break;
            }
            if (!(g->girls[3].syoushin & 3)) {
                talk_group = talk_group + 0x18;
            } else if ((g->year == 0x60) && (((u8) g->girls[3].flags.b[2] >> 4) == 4)) {
                talk_group = talk_group + 0x1E;
            } else {
                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group += 0xC;
                }

                if (!(g->girls[3].flags.f.b2)) {
                    talk_group = talk_group + 6;
                }
            }
            mode_step_next();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (girl_state_rank(3) < 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0x10);
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (girl_state_rank(3) < 4) {
            if (g->year == 0x5F) {
                g->girls[3].flags.f.b12 = 1;
            } else {
                g->girls[3].flags.f.b13 = 1;
            }
        }
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_ayako_visit_speech_contest(void) {
    u32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        talk_group = 0x26;
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x58AF);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x39;
                break;
            case 2:
                talk_group = 0x3A;
                break;
            case 3:
                talk_group = 0x3B;
                break;
            case 4:
                talk_group = 0x3C;
                break;
            }
            culture_ayako_visit_script_a_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_stage_set(CULTURE_STAGE_AYAKO_FOLLOWUP);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_ayako_visit_drama_play(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (g->year == 0x5F) {
            talk_group = 0x27;
        } else {
            talk_group = 0x28;
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x58AF);
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            culture_backstage_title_show();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x6E;

                break;
            case 2:
                talk_group = 0x6F;
                break;
            case 3:
                talk_group = 0x70;
                break;
            case 4:
                talk_group = 0x7A;
                break;
            }
            culture_ayako_visit_script_a_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = (g->year * 3) - 0xAC;
            break;
        case 2:
            talk_group = (g->year * 3) - 0xAB;
            break;
        case 3:
            talk_group = (g->year * 3) - 0xAA;
            break;
        case 4:
            mode_step_set(0x11);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x11);
        } else if (g->year == 0x61) {
            mode_stage_set(CULTURE_STAGE_AYAKO_FOLLOWUP);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x68;
                    break;
                case 2:
                    talk_group = 0x6A;
                    break;
                case 3:
                    talk_group = 0x6C;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {                /* switch 5; irregular */
                default:

                    break;
                case 0:
                case 1:
                    talk_group = 0x7C;
                    break;
                case 2:
                    talk_group = 0x7E;
                    break;
                case 3:
                    talk_group = 0x80;
                    break;
                }
                if (!(g->girls[3].flags.f.b2)) {
                    talk_group += 6;
                    g->girls[3].flags.f.b2 = 1;
                }
                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group += 0xC;
                }

                if (g->year == 0x60) {
                    talk_group = talk_group + 0x18;
                }
            }
            mode_step_next();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        talk_group = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_ayako_visit_experiment_show(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 1;

                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 0xC;
                break;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xE);
        } else if (!(g->girls[3].flags.f.b2)) {
            mode_step_set(0xC);
        } else {
            if (((u8) g->girls[3].flags.b[2] >> 4) == 4) {
                talk_group = 4;
            } else {
                talk_group = 5;
            }

            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 6;
            break;
        case 2:
            talk_group = 7;
            break;
        case 3:
            talk_group = 8;
            break;
        }
        if (!(g->girls[3].flags.f.b2)) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 16:
        portrait_read_step(0x58AF);
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        }
        break;
    case 18:
        if (girl_state_rank(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = (g->year * 3) - 0x10F;
                break;
            case 2:
                talk_group = (g->year * 3) - 0x10E;
                break;
            case 3:
                talk_group = (g->year * 3) - 0x10D;
                break;
            case 4:
                talk_group = 0xD;
                break;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x19);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    if (g->year == 0x60) {
                        talk_group = 0xB5;
                    } else {
                        talk_group = 0x17;
                    }
                    break;
                case 2:
                    talk_group = 0x19;
                    break;
                case 3:
                    talk_group = 0x1B;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {
                case 0:
                case 1:
                    talk_group = 0x1D;
                    break;
                case 2:
                    talk_group = 0x1F;
                    break;
                case 3:
                    talk_group = 0x21;
                    break;
                case 4:
                    mode_step_set(0x19);
                    break;
                }

                if (!(g->girls[3].flags.f.b2)) {
                    g->girls[3].flags.f.b2 = 1;
                    talk_group += 6;
                }
                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group += 0xC;
                }
                talk_group = (talk_group + (g->year * 0x18)) - 0x8E8;
            }
            mode_step_next();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_bg_fadeout();
        break;
    case 26:
        talk_group = 0;
        mode_step_next();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_ayako_visit_computer_fortune(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x2A;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x65;

                break;
            case 2:
                talk_group = 0x66;
                break;
            case 3:
                talk_group = 0x67;
                break;
            case 4:
                talk_group = 0x68;
                break;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(3) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 12:
        audio_command(0x202);
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        mode_stage_next();
        break;
    case 13:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(3) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x6A;

                break;
            case 2:
                talk_group = 0x6D;
                break;
            case 3:
                talk_group = 0x70;
                break;
            case 4:
                talk_group = 0x73;
                break;
            }
            talk_group += fortune_result_index;
            culture_ayako_visit_script_b_start();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x15);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x17;
                    break;
                case 2:
                    talk_group = 0x19;
                    break;
                case 3:
                    talk_group = 0x1B;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {                /* switch 5; irregular */
                default:

                    break;
                case 0:
                case 1:
                    talk_group = 0x76;
                    break;
                case 2:
                    talk_group = 0x78;
                    break;
                case 3:
                    talk_group = 0x7A;
                    break;
                }
                if (!(g->girls[3].flags.f.b2)) {
                    g->girls[3].flags.f.b2 = 1;
                    talk_group += 6;
                }

                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group = talk_group + 0xC;
                }
            }
            mode_step_next();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
            if (girl_state_rank(3) == 3) {
                talk_group = 0x87;
            }
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_bg_fadeout();
        break;
    case 22:
        talk_group = 0;
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


/* FAKE: scalar byte selector view preserves its target live range
 * (T-10630). The flag writes precede the saved-state copies so IDO emits
 * the original case-16 temporary order. */
void culture_ayako_visit_computer_game(void) {

    switch (*(u8 *)&game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x58AF);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        portrait_show_checked();
        culture_exhibit_title_show();
        talk_group = 0x2A;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 0x90;
                break;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        mode_step_next();
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 0x8D;
            break;
        case 2:
            talk_group = 0x8E;
            break;
        case 3:
            talk_group = 0x8F;
            break;
        case 4:
            mode_step_set(0xD);
            break;
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(3) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(3) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_LAUNCH);
        break;
    case 17:
        culture_festival_return_gate = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        mode_stage_set(CULTURE_STAGE_GAME_RESUME);
        break;
    case 18:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(3U);
        culture_festival_return_gate = 0;
        mode_step_next();
        break;
    case 19:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 20:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 21:
        portrait_read_step(0x58AF);
        break;
    case 22:
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        break;
    case 23:
        if (girl_state_rank(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x92;
                break;
            case 2:
                talk_group = 0x94;
                break;
            case 3:
                talk_group = 0x96;
                break;
            case 4:
                talk_group = 0x98;
                break;
            }
            if ((u8) tt_stages_cleared < 2U) {
                talk_group += 1;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        mode_step_next();
        if (!(game_state.girls[3].syoushin & 3)) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x17;
                break;
            case 2:
                talk_group = 0x19;
                break;
            case 3:
                talk_group = 0x1B;
                break;
            case 4:
                mode_step_set(0x1E);
                break;
            }
        } else {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x9A;
                break;
            case 2:
                talk_group = 0x9C;
                break;
            case 3:
                talk_group = 0x9E;
                break;
            case 4:
                mode_step_set(0x1E);
                break;
            }
            if (!game_state.girls[3].flags.f.b2) {
                talk_group += 6;
            }
            if (((u8) game_state.girls[3].flags.b[2] >> 4) == 5) {
                talk_group = talk_group + 0xC;
            }
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_bg_fadeout();
        break;
    case 31:
        talk_group = 0;
        mode_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_ayako_visit_computer_experiment_show(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x29;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 1;

                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 0xC;
                break;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xE);
        } else if (!(g->girls[3].flags.f.b2)) {
            mode_step_set(0xC);
        } else {
            if (((u8) g->girls[3].flags.b[2] >> 4) == 4) {
                talk_group = 4;
            } else {
                talk_group = 5;
            }

            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 6;
            break;
        case 2:
            talk_group = 7;
            break;
        case 3:
            talk_group = 8;
            break;
        }
        if (!(g->girls[3].flags.f.b2)) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 16:
        portrait_read_step(0x58AF);
        break;
    case 17:
        culture_exhibit_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        break;
    case 18:
        if (girl_state_rank(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0xB2;

                break;
            case 2:
                talk_group = 0xB3;
                break;
            case 3:
                talk_group = 0xB4;
                break;
            case 4:
                talk_group = 0xD;
                break;
            }
            culture_ayako_visit_script_b_start();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x19);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x17;
                    break;
                case 2:
                    talk_group = 0x19;
                    break;
                case 3:
                    talk_group = 0x1B;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {                /* switch 5; irregular */
                case 0:
                case 1:
                    talk_group = 0x4D;
                    break;
                case 2:
                    talk_group = 0x4F;
                    break;
                case 3:
                talk_group = 0x51;
                break;
                }
                if (!(g->girls[3].flags.f.b2)) {
                    g->girls[3].flags.f.b2 = 1;
                    talk_group += 6;
                }

                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group = talk_group + 0xC;
                }
            }
            mode_step_next();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_bg_fadeout();
        break;
    case 26:
        talk_group = 0;
        mode_step_next();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_ayako_visit_portrait_drawing(void) {
    GameState *g = &game_state;
    s32 girl_rank_2;
    s32 girl_rank_3;
    u32 girl_rank;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 4;
                break;
            case 3:
                talk_group = 7;
                break;
            case 4:
                talk_group = 0xA;
                break;
            }
            culture_ayako_visit_script_c_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xE);
        } else {
            if ((u16) g->clubs[4].practice_points < 0x64U) {
                talk_group += 1;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (!(g->girls[3].syoushin & 3)) {
            girl_rank_2 = girl_state_rank(3);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
        } else {
            girl_rank_3 = girl_state_rank(3);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0xC;
                break;
            case 2:
                talk_group = 0xE;
                break;
            case 3:
                talk_group = 0x10;
                break;
            }
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_ayako_visit_open_sketching(void) {
    u32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x1E;
                break;
            case 2:
                talk_group = 0x1F;
                break;
            case 3:
                talk_group = 0x20;
                break;
            case 4:
                talk_group = 0x22;
                break;
            }
            culture_ayako_visit_script_c_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(3) != 4) {
            talk_group = 0x21;
        }
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_ayako_visit_oil_painting_exhibit(void) {
    GameState *g = &game_state;
    s32 girl_rank_2;
    s32 girl_rank_3;
    u32 girl_rank;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_exhibit_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(3);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x24;
                break;
            case 2:
                talk_group = 0x25;
                break;
            case 3:
                talk_group = 0x26;
                break;
            case 4:
                talk_group = 0x27;
                break;
            }
            culture_ayako_visit_script_c_start();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xC);
        } else if (!(g->girls[3].syoushin & 3)) {
            girl_rank_2 = girl_state_rank(3);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
            mode_step_next();
        } else {
            girl_rank_3 = girl_state_rank(3);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x29;
                break;
            case 2:
                talk_group = 0x2B;
                break;
            case 3:
                talk_group = 0x2D;
                break;
            }
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_ayako_visit_concert(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            culture_backstage_title_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (g->year == 0x5F) {
            talk_group = 0x2E;
        } else {
            talk_group = 0x2F;
        }

        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 2;
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x58AF);
        break;
    case 9:
        culture_backstage_title_show();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = (g->year * 3) - 0xE8;
                break;
            case 2:
                talk_group = (g->year * 3) - 0xE7;
                break;
            case 3:
                talk_group = (g->year * 3) - 0xE6;
                break;
            case 4:
                talk_group = 0x3E;
                break;
            }
            culture_ayako_visit_script_c_start();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xF);
        } else if (!(g->girls[3].syoushin & 3)) {
            switch (girl_state_rank(3)) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
            mode_step_next();
        } else {
            switch (girl_state_rank(3)) {                    /* switch 4; irregular */

            case 0:
            case 1:
                talk_group = 0x40;
                break;
            case 2:
                talk_group = 0x42;
                break;
            case 3:
                talk_group = 0x44;
                break;
            }
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }
            talk_group = (talk_group + (g->year * 0xC)) - 0x474;
            mode_step_next();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        culture_scene_end();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void culture_ayako_visit_unused_stage(void) {
}

void culture_ayako_visit_followup(void) {
    GameState *g = &game_state;
    s32 girl_rank;

    switch (g->mode_step) {
    case 0:
        if (girl_state_rank(3) == 4) {
            mode_step_set(2);
            return;
        }
        girl_rank = girl_state_rank(3);
        switch (girl_rank) {
        case 0:
        case 1:
            talk_group = 0x50;
            break;
        case 2:
            talk_group = 0x52;
            break;
        case 3:
            talk_group = 0x54;
            break;
        }
        if (!(g->girls[3].syoushin & 3)) {
            talk_group = talk_group + 0x18;
        } else {
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }

            if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                talk_group = talk_group + 0xC;
            }
        }
        mode_step_next();
        return;
    case 1:
        normal_date_speak();
        return;
    case 2:
        normal_date_girl_out();
        return;
    case 3:
        normal_date_speak();
        return;
    case 4:
        normal_date_bg_fadeout();
        return;
    case 5:
        talk_group = 0;
        mode_step_next();
        return;
    case 6:
        normal_date_speak();
        return;
    case 7:
        if ((((u16) g->player_flags.h >> 0xC) == 0) && (girl_state_rank(3) != 4)) {
            if (g->year == 0x5F) {
                g->girls[3].flags.f.b12 = 1;
            }
            if (g->year == 0x60) {
                g->girls[3].flags.f.b13 = 1;
            }
        }
        culture_scene_end();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_ayako_visit_script_a_start(void) {
    talk_names_set(3);
    culture_script_set_ayako_visit_a();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}

void culture_ayako_visit_script_b_start(void) {
    talk_names_set(3);
    culture_script_set_ayako_visit_b();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}

void culture_ayako_visit_script_c_start(void) {
    talk_names_set(3);
    culture_script_set_ayako_visit_c();
    talk_line = 0;
    talk_text_table = culture_script_text_table;
    talk_command_table = culture_script_command_table;
    talk_voice_table = culture_script_voice_table;
    mode_step_next();
}
