/* shiori_club_block.c: src/ovl/BUNKAKEN/8014A6A0.c:4: Shiori at her club (stages 32-43). Original object 0x8014A6A0-0x8014E440 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void tour_stage_shiori_club_route(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_LITERATURE_COLLECTION);
            return;
        case 0x60:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_LITERATURE_COLLECTION);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_LITERATURE_SPEECH);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(TOUR_STAGE_SHIORI_CLUB_DRAMA);
        return;
    case 2:
        if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        }
        mode_stage_set(TOUR_STAGE_SHIORI_CLUB_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_COMPUTER_FORTUNE);
            break;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_COMPUTER_GAME);
            break;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_COMPUTER_EXPERIMENT);
            break;
        }
        if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        }
        return;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ART_PORTRAIT);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ART_SKETCH);
            return;
        case 0x61:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ART_OIL);
            return;
        }
        break;
    case 5:
        mode_stage_set(TOUR_STAGE_SHIORI_CLUB_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_shiori_club_literature_collection(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

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
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {                      /* switch 2; irregular */
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
            }
            if ((D_80122EA4 == 0) || (girl_state_rank(0) == 4)) {
                if (g->year == 0x5F) {
                    talk_group = 0xD;
                } else {
                    talk_group = 0x16;
                }
            }
            tour_shiori_club_script_begin();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (!(g->girls[0].flags.f.b2) && (girl_state_rank(0) != 4)) {
            g->girls[0].flags.f.b2 = 1;
            talk_group = 4;
            mode_step_next();
        } else {
            mode_step_set(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((D_80122EA4 == 0) || (girl_state_rank(0) == 4)) {
            mode_step_set(0xC);
        } else {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                if (g->year == 0x5F) {
                    talk_group = 7;
                } else {
                    talk_group = 0x10;
                }
                break;
            case 2:
                if (g->year == 0x5F) {
                    talk_group = 9;
                } else {
                    talk_group = 0x12;
                }
                break;
            case 3:
                if (g->year == 0x5F) {
                    talk_group = 0xB;
                } else {
                    talk_group = 0x14;
                }
                break;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            talk_group = 0xE;
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            girl_rank_3 = girl_state_rank(0);
            switch (girl_rank_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                if (g->year == 0x5F) {
                    talk_group = 8;
                } else {
                    talk_group = 0x11;
                }
                break;
            case 2:
                if (g->year == 0x5F) {
                    talk_group = 0xA;
                } else {
                    talk_group = 0x13;
                }
                break;
            case 3:
                if (g->year == 0x5F) {
                    talk_group = 0xC;
                } else {
                    talk_group = 0x15;
                }
                break;
            }
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
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
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_literature_speech(void) {
    GameState *g = &game_state;
    u32 girl_rank;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (g->girls[0].flags.f.b2) {
                talk_group = 0x19;
            } else {
                talk_group = 0x1A;
            }
            tour_shiori_club_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_title_clear();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            if (g->girls[0].flags.f.b2) {
                mode_step_set(0xB);
            } else {
                g->girls[0].flags.f.b2 = 1;
                talk_group = 4;
            }
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 6;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x544A);
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            tour_title_backstage_show();
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        girl_rank = girl_state_rank(0);
        switch (girl_rank) {
        case 0:
        case 1:
            talk_group = 0x1B;
            break;
        case 2:
            talk_group = 0x1C;
            break;
        case 3:
            talk_group = 0x1D;
            break;
        case 4:
            talk_group = 0x1E;
            break;
        }
        if (D_80122EA4 == 0) {
            talk_group = 0x1E;
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
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
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_shiori_club_drama(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x1F;
            tour_shiori_club_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_title_clear();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            if (g->girls[0].flags.f.b2) {
                mode_step_set(0xB);
            } else {
                talk_group = 5;
                g->girls[0].flags.f.b2 = 1;
            }
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 6;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x5477);
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            tour_title_backstage_show();
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        switch (girl_state_rank(0)) {
        case 0:
        case 1:
            talk_group = 0x20;

            break;
        case 2:
            talk_group = 0x21;
            break;
        case 3:
            talk_group = 0x22;
            break;
        case 4:
            talk_group = 0x23;
            break;
        }

        if (D_80122EA4 == 0) {
            talk_group = 0x23;
        }
        talk_group = (talk_group + (g->year * 4)) - 0x17C;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        g->girls[0].flags.f.b2 = 1;
        talk_group = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_science(void) {
    GameState *g = &game_state;
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_3;

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
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x54D1);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
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
                talk_group = 0x32;
                break;
            }
            if (D_80122EA4 == 0) {
                talk_group = 0x32;
            }
            tour_shiori_club_script_begin();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            talk_group = 4;
            g->girls[0].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        mode_step_next();
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x2C;
            break;
        case 2:
            talk_group = 0x2E;
            break;
        case 3:
            talk_group = 0x30;
            break;
        case 4:
            mode_step_set(0xC);
            break;
        }
        if (D_80122EA4 == 0) {
            mode_step_set(0xC);
        }
        talk_group = (talk_group + (g->year * 8)) - 0x2F8;
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 2;
        }
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 14:
        portrait_read_step(0x54D1);
        break;
    case 15:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(0);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x2D;
                break;
            case 2:
                talk_group = 0x2F;
                break;
            case 3:
                talk_group = 0x31;
                break;
            case 4:
                talk_group = 0x33;
                break;
            }
            if (D_80122EA4 == 0) {
                talk_group = 0x33;
            }
            talk_group = (talk_group + (g->year * 8)) - 0x2F8;
            tour_shiori_club_script_begin();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        talk_group = 0;
        mode_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_computer_fortune(void) {
    GameState *g = &game_state;
    s32 girl_rank_2;
    s32 girl_rank_3;
    s32 girl_rank_4;
    u32 girl_rank;

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
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
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
                talk_group = 0x56;
                break;
            }
            if (D_80122EA4 == 0) {
                talk_group = 0x56;
            }
            tour_shiori_club_script_begin();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            g->girls[0].flags.f.b2 = 1;
            talk_group = 4;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 0x44;
            break;
        case 2:
            talk_group = 0x4A;
            break;
        case 3:
            talk_group = 0x50;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_set(0xD);
        } else {
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
        audio_command(0x202);
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_FORTUNE_TELLING);
        break;
    case 14:
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        mode_step_next();
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            girl_rank_3 = girl_state_rank(0);
            switch (girl_rank_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 0x45;
                break;
            case 2:
                talk_group = 0x4B;
                break;
            case 3:
                talk_group = 0x51;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                talk_group = 0x57;
                mode_step_set(0x12);
            }
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        talk_group += D_80122EA8;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        girl_rank_4 = girl_state_rank(0);
        switch (girl_rank_4) {                        /* switch 5; irregular */
        case 0:
        case 1:
            talk_group = 0x49;
            break;
        case 2:
            talk_group = 0x4F;
            break;
        case 3:
            talk_group = 0x55;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_set(0x16);
        } else {
            mode_step_next();
        }
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        talk_group = 0;
        mode_step_next();
        break;
    case 23:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 24:
        normal_date_bg_fadeout();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_computer_game(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;
    s32 girl_rank_4;
    s32 girl_rank_5;

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
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
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
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                talk_group = 0x69;
            }
            tour_shiori_club_script_begin();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            g->girls[0].flags.f.b2 = 1;
            talk_group = 4;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x5A;
            break;
        case 2:
            talk_group = 0x5F;
            break;
        case 3:
            talk_group = 0x64;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_set(0xC);
        } else {
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        g->saved_game_mode = g->game_mode;
        g->saved_mode_stage = g->mode_stage;
        g->saved_mode_step = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_TT_NARRATOR);
        break;
    case 14:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_RESUME);
        break;
    case 15:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(8U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 16:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 18:
        portrait_read_step(0x544A);
        break;
    case 19:
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        break;
    case 20:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            girl_rank_3 = girl_state_rank(0);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x5B;
                break;
            case 2:
                talk_group = 0x60;
                break;
            case 3:
                talk_group = 0x65;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                talk_group = 0x6A;
            }
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        mode_step_next();
        girl_rank_4 = girl_state_rank(0);
        switch (girl_rank_4) {
        case 0:
        case 1:
            talk_group = 0x5C;
            break;
        case 2:
            talk_group = 0x61;
            break;
        case 3:
            talk_group = 0x66;
            break;
        }
        if ((u8) D_800E7D4E < 2U) {
            talk_group += 1;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_set(0x1B);
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        girl_rank_5 = girl_state_rank(0);
        switch (girl_rank_5) {
        case 0:
        case 1:
            talk_group = 0x5E;
            break;
        case 2:
            talk_group = 0x63;
            break;
        case 3:
            talk_group = 0x68;
            break;
        }
        mode_step_next();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 28:
        talk_group = 0;
        mode_step_next();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_shiori_club_computer_experiment(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

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
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x544A);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {                      /* switch 2; irregular */
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
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                mode_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            talk_group = 4;
            g->girls[0].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 0x6B;
            break;
        case 2:
            talk_group = 0x6C;
            break;
        case 3:
            talk_group = 0x6D;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            talk_group = 0x71;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 2;
        }
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 14:
        portrait_read_start(0x544A);
        mode_step_next();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(0);
            switch (girl_rank_3) {                    /* switch 4; irregular */
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
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                talk_group = 0x72;
            }
            tour_shiori_club_script_begin();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        talk_group = 0;
        mode_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_art_portrait(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x5828);
        break;
    case 4:
        memcpy((void *)0x80197000, 0x80180000, 0x16800);
        mode_step_next();
        break;
    case 5:
        portrait_read_step(0x54A4);
        break;
    case 6:
        memcpy((void *)0x801AE000, 0x80180000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        portrait_show_checked();
        tour_window_title_set();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        break;
    case 7:
        normal_date_bg_fadein();
        break;
    case 8:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                talk_group = 0x85;
                mode_step_set(0x1E);
            } else {
                girl_rank = girl_state_rank(0);
                switch (girl_rank) {                  /* switch 2; irregular */
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
                }
            }
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (g->girls[0].flags.f.b2) {
            mode_step_set(0xD);
        } else {
            talk_group = 4;
            g->girls[0].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 0x73;
            break;
        case 2:
            talk_group = 0x79;
            break;
        case 3:
            talk_group = 0x7F;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        talk_names_set(0xCU);
        g->current_girl = GIRL_REI;
        D_80122D20 = 1;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        memcpy((void *)0x80180000, 0x80197000, 0x16800);
        mode_step_next();
        break;
    case 19:
        D_80122D20 = 0;
        addr_init_bustup();
        portrait_show_checked();
        portrait_layout_set(0U);
        break;
    case 20:
        normal_date_girl_in();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        talk_names_set(0U);
        g->current_girl = GIRL_SHIORI;
        D_80122D20 = 1;
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        talk_names_set(0xCU);
        g->current_girl = GIRL_REI;
        D_80122D20 = 0;
        mode_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_girl_out();
        break;
    case 27:
        memcpy((void *)0x80180000, 0x801AE000, 0x16800);
        mode_step_next();
        break;
    case 28:
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        portrait_show_checked();
        talk_names_set(0U);
        break;
    case 29:
        normal_date_girl_in();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 32:
        normal_date_bg_fadeout();
        break;
    case 33:
        talk_group = 0;
        mode_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_art_sketch(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            if (girl_state_rank(0) == 4) {
                portrait_read_start(0x6C5F);
            } else {
                portrait_read_start(0x54A4);
            }
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            /* FAKE: the empty pointer test keeps the GameState base in v1
             * in later cases; the test itself is eliminated. T-10630. */
            if (!g) {}
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x86;
                break;
            case 2:
                talk_group = 0x87;
                break;
            case 3:
                talk_group = 0x88;
                break;
            }
            addr_init_bustup();
            if (girl_state_rank(0) == 4) {
                talk_group = 0x89;
                g->current_girl = GIRL_YOSHIO;
                talk_names_set(0xDU);
            } else {
                g->current_girl = GIRL_SHIORI;
            }
            portrait_show_checked();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (girl_state_rank(0) == 4) {
            talk_group = 0x8C;
        } else if (g->girls[0].flags.f.b2) {
            talk_group = 0x8A;
        } else {
            g->girls[0].flags.f.b2 = 1;
            talk_group = 0x8B;
        }
        mode_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0) == 4) {
            mode_step_set(0x14);
        } else {
            mode_step_next();
        }
        break;
    case 11:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {                        /* switch 3; irregular */
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
        }
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        talk_group = 0x90;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        talk_group = 0;
        g->girls[0].flags.f.b2 = 1;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        tour_finish_visit();
        break;
    case 20:
        normal_date_two_select();
        break;
    case 21:
        if (D_80122CDC != 0) {
            talk_group = 0x94;
        } else {
            talk_group = 0x91;
        }
        mode_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        if (D_80122CDC != 0) {
            mode_step_set(0x10);
        } else {
            talk_names_set(0U);
            if (g->girls[0].flags.f.b2) {
                talk_group = 0x92;
            } else {
                talk_group = 0x93;
            }
            mode_step_next();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        mode_step_set(0x11);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_art_oil(void) {
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x54A4);
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 4:
        tour_window_title_set();
        mode_step_next();
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {                      /* switch 2; irregular */
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
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                mode_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (game_state.girls[0].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            talk_group = 4;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 0x95;
            break;
        case 2:
            talk_group = 0x97;
            break;
        case 3:
            talk_group = 0x99;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            talk_group = 0x9B;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            talk_group = 0x9C;
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            girl_rank_3 = girl_state_rank(0);
            switch (girl_rank_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 0x96;
                break;
            case 2:
                talk_group = 0x98;
                break;
            case 3:
                talk_group = 0x9A;
                break;
            }
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
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
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_shiori_club_brass(void) {
    s32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x9E;
            tour_shiori_club_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_title_clear();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_shiori_club_script_begin();
            if (game_state.girls[0].flags.f.b2) {
                mode_step_set(0xB);
            } else {
                talk_group = 5;
            }
            portrait_read_start(0x544A);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 6;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            portrait_show_checked();
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            tour_title_backstage_show();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        girl_rank = girl_state_rank(0);
        switch (girl_rank) {                          /* switch 2; irregular */
        case 0:
        case 1:
            talk_group = 0x9F;
            break;
        case 2:
            talk_group = 0xA0;
            break;
        case 3:
            talk_group = 0xA1;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            talk_group = 0xA2;
        }
        talk_group = (talk_group + (game_state.year * 4)) - 0x17C;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
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
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_shiori_club_script_begin(void) {
    talk_names_set(0);
    tour_script_shiori_club();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
