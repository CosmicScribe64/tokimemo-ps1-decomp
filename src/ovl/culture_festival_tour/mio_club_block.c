/* mio_club_block.c: src/ovl/BUNKAKEN/8013BFE0.c:7: Mio at her club (stages 44-47). Original object 0x8013BFE0-0x8013CF30 (config/objects/BUNKAKEN.txt). */
#define MAIN_API_OVERRIDE_culture_exhibit_club /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/culture_festival_tour.h"

extern u32 culture_exhibit_club;

void tour_stage_mio_club_route(void) {
    game_mode_scene_girl = 1;
    switch (culture_exhibit_club) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(TOUR_STAGE_MIO_CLUB_LITERATURE_SPEECH);
            return;
        }
        if ((girl_state_rank(1) == 4) && !game_state.girls[1].flags.f.b2) {
            game_mode_scene_girl = 0xFF;
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        }
        mode_stage_set(TOUR_STAGE_MIO_CLUB_LITERATURE_COLLECTION);
        return;
    case 1:
        mode_stage_set(TOUR_STAGE_MIO_CLUB_DRAMA);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_mio_club_literature_collection(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        tour_scratch_bytes = 0;
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_MIO;
            portrait_show_checked();
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        }
        break;
    case 5:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(1);
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
            if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
                if (g->year == 0x5F) {
                    talk_group = 0xD;
                } else {
                    talk_group = 0x16;
                }
            }
            tour_mio_club_script_begin();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            mode_step_set(0xC);
        } else if (!(g->girls[1].flags.f.b2)) {
            g->girls[1].flags.f.b2 = 1;
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
        girl_rank_2 = girl_state_rank(1);
        switch (girl_rank_2) {
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
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
            talk_group = 0xE;
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
            girl_rank_3 = girl_state_rank(1);
            switch (girl_rank_3) {
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
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
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

void tour_stage_mio_club_literature_speech(void) {
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
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (g->girls[1].flags.f.b2) {
                talk_group = 0x19;
            } else {
                talk_group = 0x1A;
            }
            tour_mio_club_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 3;
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_mio_club_script_begin();
            if (g->girls[1].flags.f.b2) {
                mode_step_set(0xB);
            } else {
                g->girls[1].flags.f.b2 = 1;
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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x5D41);
            mode_step_next();
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            tour_title_backstage_show();
            addr_init_bustup();
            g->current_girl = GIRL_MIO;
            portrait_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        girl_rank = girl_state_rank(1);
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
        if (declined_companion_girl == 1) {
            talk_group = 0x1E;
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
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

void tour_stage_mio_club_drama(void) {
    GameState *g = &game_state;
    s32 girl_rank;

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
            cd_read_sectors(7U, 0x801E8000, 0x4AB6);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x1F;
            tour_mio_club_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        cd_read_sectors(7U, 0x801E8000, 0x4AB6);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_mio_club_script_begin();
            if (g->girls[1].flags.f.b2) {
                mode_step_set(0xB);
            } else {
                talk_group = 5;
                g->girls[1].flags.f.b2 = 1;
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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x5D6E);
            mode_step_next();
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            tour_title_backstage_show();
            addr_init_bustup();
            g->current_girl = GIRL_MIO;
            portrait_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        girl_rank = girl_state_rank(1);
        switch (girl_rank) {
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
        }
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
            talk_group = 0x23;
        }
        talk_group = (talk_group + (g->year * 4)) - 0x17C;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(1) == 4) || (declined_companion_girl == 1)) {
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
        g->girls[1].flags.f.b2 = 1;
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

void tour_mio_club_script_begin(void) {
    talk_names_set(1);
    tour_script_mio_club();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
