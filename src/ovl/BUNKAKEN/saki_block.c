/* saki_block.c: src/ovl/BUNKAKEN/80137640.c:4: Saki block (stages 58-71). Original object 0x80137640-0x80139E50 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void tour_stage_saki_route(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SAKI_LITERATURE_POEMS);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SAKI_LITERATURE_ANTHOLOGY);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SAKI_LITERATURE_SPEECH);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(TOUR_STAGE_SAKI_DRAMA);
        return;
    case 2:
        mode_stage_set(TOUR_STAGE_SAKI_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SAKI_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SAKI_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SAKI_COMPUTER_EXPERIMENT);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SAKI_ART_PORTRAIT);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SAKI_ART_SKETCH);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SAKI_ART_OIL);
            return;
        }
        break;
    case 5:
        if (game_state.year == 0x61) {
            mode_stage_set(TOUR_STAGE_SAKI_BRASS_1997);
            return;
        }
        mode_stage_set(TOUR_STAGE_SAKI_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_saki_literature_poems(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 2;
                break;
            case 2:
            case 3:
                talk_group = 4;
                break;
            }
            tour_saki_script_begin();
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
        girl_rank_2 = girl_state_rank(4);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 3;
            break;
        case 2:
        case 3:
            talk_group = 5;
            break;
        }
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
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_literature_anthology(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
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
            tour_saki_script_begin();
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
        girl_rank_2 = girl_state_rank(4);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 9;
            break;
        case 2:
            talk_group = 0xA;
            break;
        case 3:
            talk_group = 0xB;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(4) == 3) {
            normal_date_bggirl_fadeout();
        } else {
            mode_step_set(0xD);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        talk_group = 0;
        mode_step_next();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_literature_speech(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xD;
                break;
            case 2:
            case 3:
                talk_group = 0xE;
                break;
            }
            tour_saki_script_begin();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x66BF);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SAKI;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(4);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0xF;
                break;
            case 2:
            case 3:
                talk_group = 0x10;
                break;
            }
            tour_saki_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_drama(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x11;
                break;
            case 2:
                talk_group = 0x12;
                break;
            case 3:
                talk_group = 0x13;
                break;
            }
            tour_saki_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
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
            portrait_read_step(0x66BF);
        }
        break;
    case 10:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        game_state.current_girl = GIRL_SAKI;
        portrait_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            if (game_state.year == 0x5F) {
                girl_rank_2 = girl_state_rank(4);
                switch (girl_rank_2) {
                case 0:
                case 1:
                    talk_group = 0x14;
                    break;
                case 2:
                    talk_group = 0x15;
                    break;
                case 3:
                    talk_group = 0x16;
                    break;
                }
            } else if (girl_state_rank(4) < 2) {
                talk_group = (game_state.year * 2) - 0xA9;
            } else {
                talk_group = (game_state.year * 2) - 0xA8;
            }
            tour_saki_script_begin();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_science(void) {
    s32 girl_rank;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x1B;
                break;
            case 2:
            case 3:
                talk_group = 0x1C;
                break;
            }
            tour_saki_script_begin();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x66BF);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SAKI;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (game_state.year) {
            case 0x5F:
                talk_group = 0x1D;
                break;
            case 0x60:
                talk_group = 0x21;
                break;
            case 0x61:
                talk_group = 0x23;
                break;
            }
            if (girl_state_rank(4) >= 2) {
                talk_group += 1;
            }
            tour_saki_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if ((game_state.year == 0x5F) && (girl_state_rank(4) >= 2)) {
            normal_date_girl_out();
        } else {
            mode_step_set(0x11);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_in();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
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

void tour_stage_saki_computer_fortune(void) {
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
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
            tour_saki_script_begin();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(TOUR_STAGE_FORTUNE_TELLING);
        break;
    case 8:
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        mode_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(4);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x28;
                break;
            case 2:
                talk_group = 0x2B;
                break;
            case 3:
                talk_group = 0x2E;
                break;
            }
            talk_group += D_80122EA8;
            tour_saki_script_begin();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(4);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x31;
            break;
        case 2:
            talk_group = 0x32;
            break;
        case 3:
            talk_group = 0x33;
            break;
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
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_computer_game(void) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x34;
                break;
            case 2:
            case 3:
                talk_group = 0x35;
                break;
            }
            tour_saki_script_begin();
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
        talk_names_set(4U);
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
        portrait_read_step(0x66BF);
        break;
    case 13:
        addr_init_bustup();
        g->current_girl = GIRL_SAKI;
        portrait_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(4);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x36;
                break;
            case 2:
            case 3:
                talk_group = 0x38;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
            tour_saki_script_begin();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        girl_rank_3 = girl_state_rank(4);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x3A;
            break;
        case 2:
        case 3:
            talk_group = 0x3B;
            break;
        }
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_bggirl_fadeout();
        break;
    case 20:
        talk_group = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_computer_experiment(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x3C;
                break;
            case 2:
            case 3:
                talk_group = 0x3D;
                break;
            }
            tour_saki_script_begin();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x66BF);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SAKI;
        portrait_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(4);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x3E;
                break;
            case 2:
            case 3:
                talk_group = 0x3F;
                break;
            }
            tour_saki_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_art_portrait(void) {
    s32 girl_rank;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x40;
                break;
            case 2:
            case 3:
                talk_group = 0x43;
                break;
            }
            tour_saki_script_begin();
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
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_art_sketch(void) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x46;
                break;
            case 2:
            case 3:
                talk_group = 0x49;
                break;
            }
            tour_saki_script_begin();
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
        game_state.current_girl = GIRL_SOTOI;
        mode_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bg_fadeout();
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
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_art_oil(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x4C;
                break;
            case 2:
            case 3:
                talk_group = 0x4E;
                break;
            }
            tour_saki_script_begin();
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
        girl_rank_2 = girl_state_rank(4);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x4D;
            break;
        case 2:
        case 3:
            talk_group = 0x4F;
            break;
        }
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
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_brass(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x50;
                break;
            case 2:
            case 3:
                talk_group = 0x51;
                break;
            }
            tour_saki_script_begin();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x66BF);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SAKI;
        portrait_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(4);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x52;
                break;
            case 2:
            case 3:
                talk_group = 0x53;
                break;
            }
            if (game_state.year == 0x60) {
                talk_group += 2;
            }
            tour_saki_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_saki_brass_1997(void) {
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

    switch (game_state.mode_step) {
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
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(4);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x50;
                break;
            case 2:
            case 3:
                talk_group = 0x51;
                break;
            }
            tour_saki_script_begin();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x66BF);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SAKI;
            portrait_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(4);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x56;
                break;
            case 2:
            case 3:
                talk_group = 0x5A;
                break;
            }
            tour_saki_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (game_state.param_art.value < 0x78) {
            talk_group += 1;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        girl_rank_3 = girl_state_rank(4);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x59;
            break;
        case 2:
        case 3:
            talk_group = 0x5D;
            break;
        }
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
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

void tour_saki_script_begin(void) {
    talk_names_set(4);
    tour_script_saki();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
