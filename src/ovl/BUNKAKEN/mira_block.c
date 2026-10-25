/* mira_block.c: src/ovl/BUNKAKEN/8014E440.c:4: Mira block (stages 98-110). Original object 0x8014E440-0x80150D30 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void tour_stage_mira_route(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_MIRA_LITERATURE_POEMS);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_MIRA_LITERATURE_ANTHOLOGY);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_MIRA_LITERATURE_SPEECH);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(TOUR_STAGE_MIRA_DRAMA);
        return;
    case 2:
        mode_stage_set(TOUR_STAGE_MIRA_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_MIRA_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_MIRA_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_MIRA_COMPUTER_EXPERIMENT);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_MIRA_ART_PORTRAIT);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_MIRA_ART_SKETCH);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_MIRA_ART_OIL);
            return;
        }
        break;
    case 5:
        mode_stage_set(TOUR_STAGE_MIRA_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_mira_literature_poems(void) {
    GameState *g = &game_state;
    s32 girl_rank;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            tour_mira_script_begin();
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 3;
                break;
            case 2:
                talk_group = 5;
                mode_step_set(0xE);
                break;
            case 3:
                talk_group = 8;
                mode_step_set(0x12);
                break;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        talk_group = 4;
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        talk_group = 0;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        tour_finish_visit();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        talk_group = 2;
        normal_date_bggirl_fadeout();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        talk_group = 6;
        normal_date_bggirl_fadein();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_bg_fadeout();
        break;
    case 22:
        mode_step_set(0xB);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_literature_anthology(void) {
    GameState *g = &game_state;
    s32 girl_rank;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            tour_mira_script_begin();
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0xA;
                break;
            case 2:
                talk_group = 0xC;
                mode_step_set(9);
                break;
            case 3:
                talk_group = 0xD;
                mode_step_set(0xE);
                break;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        talk_group = 0xB;
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        talk_group = 0;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        tour_finish_visit();
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
        mode_step_set(0xB);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_literature_speech(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0xF;
                break;
            case 2:
                talk_group = 0x10;
                break;
            case 3:
                talk_group = 0x11;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5B25);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(7);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x12;
                break;
            case 2:
                talk_group = 0x13;
                break;
            case 3:
                talk_group = 0x14;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0x10);
        } else {
            normal_date_girl_out();
        }
        break;
    case 14:
        talk_group = 0x15;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_drama(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x16;
                break;
            case 2:
                talk_group = 0x17;
                break;
            case 3:
                talk_group = 0x18;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x5B25);
        }
        break;
    case 10:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(7);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x19;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1B;
                break;
            }
            talk_group = (talk_group + (g->year * 3)) - 0x11D;
            tour_mira_script_begin();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (girl_state_rank(7) >= 2) {
            talk_group = 0x15;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 15:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0x11);
        } else {
            normal_date_speak();
        }
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
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_science(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x22;
                break;
            case 2:
                talk_group = 0x23;
                break;
            case 3:
                talk_group = 0x24;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5B25);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(7);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x25;
                break;
            case 2:
                talk_group = 0x26;
                break;
            case 3:
                talk_group = 0x27;
                break;
            }
            talk_group = (talk_group + (g->year * 3)) - 0x11D;
            tour_mira_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(7) < 2) {
            if (g->year == 0x5F) {
                mode_step_set(0x15);
            } else {
                mode_step_set(0x11);
            }
        } else {
            mode_step_next();
        }
        break;
    case 14:
        mode_step_next();
        break;
    case 15:
        talk_group = 0x15;
        normal_date_girl_out();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
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
    case 21:
        talk_group = 0x2E;
        normal_date_girl_out();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_in();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        mode_step_set(0x11);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_computer_fortune(void) {
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
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x30;
                break;
            case 2:
                talk_group = 0x31;
                break;
            case 3:
                talk_group = 0x32;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        audio_command(0x202);
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_FORTUNE_TELLING);
        break;
    case 8:
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        mode_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(7);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x33;
                break;
            case 2:
                talk_group = 0x36;
                break;
            case 3:
                talk_group = 0x39;
                break;
            }
            talk_group += D_80122EA8;
            tour_mira_script_begin();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(7);
        switch (girl_rank_3) {                        /* switch 4; irregular */
        case 0:
        case 1:
            talk_group = 0x3C;
            break;
        case 2:
            talk_group = 0x3D;
            break;
        case 3:
            talk_group = 0x3E;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0x10);
        } else {
            talk_group = 0x15;
            normal_date_girl_out();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_computer_game(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x3F;
                break;
            case 2:
                talk_group = 0x40;
                break;
            case 3:
                talk_group = 0x41;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        g->saved_game_mode = g->game_mode;
        g->saved_mode_stage = g->mode_stage;
        g->saved_mode_step = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_TT_NARRATOR);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_RESUME);
        break;
    case 9:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(7U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 10:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 12:
        portrait_read_step(0x5B25);
        break;
    case 13:
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            tour_mira_script_begin();
        }
        break;
    case 16:
        if (girl_state_rank(7) < 2) {
            if ((u8) D_800E7D4E < 2U) {
                talk_group = 0x43;
            } else {
                talk_group = 0x42;
            }
            mode_step_next();
        } else {
            mode_step_set(0x12);
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        girl_rank_2 = girl_state_rank(7);
        switch (girl_rank_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 0x44;
            break;
        case 2:
            talk_group = 0x45;
            break;
        case 3:
            talk_group = 0x46;
            break;
        }
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0x16);
        } else {
            talk_group = 0x15;
            normal_date_girl_out();
        }
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 23:
        talk_group = 0;
        mode_step_next();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_computer_experiment(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x47;
                break;
            case 2:
                talk_group = 0x48;
                break;
            case 3:
                talk_group = 0x49;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5B25);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(7);
            switch (girl_rank_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x4A;
                break;
            case 2:
                talk_group = 0x4B;
                break;
            case 3:
                talk_group = 0x4C;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0xF);
        } else {
            talk_group = 0x15;
            normal_date_girl_out();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 16:
        talk_group = 0;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_art_portrait(void) {
    GameState *g = &game_state;
    s32 girl_rank;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x4D;
                break;
            case 2:
                talk_group = 0x50;
                break;
            case 3:
                talk_group = 0x53;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0xC);
        } else {
            talk_group = 0x15;
            normal_date_girl_out();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 13:
        talk_group = 0;
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_art_sketch(void) {
    s32 girl_rank;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x56;
                break;
            case 2:
                talk_group = 0x58;
                break;
            case 3:
                talk_group = 0x5A;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        game_state.current_girl = GIRL_SOTOI;
        mode_step_next();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        talk_group = 0;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_art_oil(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            tour_mira_script_begin();
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x5C;
                break;
            case 2:
                talk_group = 0x5E;
                break;
            case 3:
                talk_group = 0x60;
                break;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if (girl_state_rank(7) < 2) {
            talk_group = 1;
        } else {
            talk_group = 2;
        }
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        girl_rank_2 = girl_state_rank(7);
        switch (girl_rank_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 0x5D;
            break;
        case 2:
            talk_group = 0x5F;
            break;
        case 3:
            talk_group = 0x61;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0xC);
        } else {
            talk_group = 0x15;
            normal_date_girl_out();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 13:
        talk_group = 0;
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_mira_brass(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(7);
            switch (girl_rank) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x62;
                break;
            case 2:
                talk_group = 0x63;
                break;
            case 3:
                talk_group = 0x64;
                break;
            }
            tour_mira_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5B25);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0x13U, 0x801E8000, 0x4A52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(7);
            switch (girl_rank_2) {                    /* switch 3; irregular */
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
            }
            talk_group = (talk_group + (g->year * 3)) - 0x11D;
            tour_mira_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(7) < 2) {
            mode_step_set(0xF);
        } else {
            talk_group = 0x15;
            normal_date_girl_out();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(7) < 2) {
            normal_date_bggirl_fadeout();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 16:
        talk_group = 0;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_mira_script_begin(void) {
    talk_names_set(7);
    tour_script_mira();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
