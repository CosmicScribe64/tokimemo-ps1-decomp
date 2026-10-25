/* yuina_club_block.c: src/ovl/BUNKAKEN/80140F60.c:4: Yuina at her club (stages 48-52). Original object 0x80140F60-0x801428D0 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void tour_stage_yuina_club_route(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 2;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 2:
        if ((girl_state_rank(2) == 4) && !(game_state.girls[2].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        }
        mode_stage_set(TOUR_STAGE_YUINA_CLUB_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_YUINA_CLUB_COMPUTER_FORTUNE);
            break;
        case 0x60:
            mode_stage_set(TOUR_STAGE_YUINA_CLUB_COMPUTER_GAME);
            break;
        case 0x61:
            mode_stage_set(TOUR_STAGE_YUINA_CLUB_COMPUTER_EXPERIMENT);
            break;
        }
        if ((girl_state_rank(2) == 4) && !(game_state.girls[2].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        }
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_yuina_club_science(void) {
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
            portrait_read_start(0x525B);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUINA;
            portrait_show_checked();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            tour_yuina_club_script_begin();
            girl_rank = girl_state_rank(2);
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
                mode_step_set(0xA);
                break;
            }
            if (D_80122EA4 == 2) {
                mode_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[2].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            talk_group = 4;
            g->girls[2].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(2);
        switch (girl_rank_2) {
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
        if (D_80122EA4 == 2) {
            talk_group = 8;
        }
        talk_group = (talk_group + (g->year * 8)) - 0x2F8;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 3;
        }
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 14:
        portrait_read_step(0x525B);
        break;
    case 15:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        break;
    case 16:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(2);
            switch (girl_rank_3) {
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
            case 4:
                talk_group = 0xC;
                break;
            }
            if (D_80122EA4 == 2) {
                talk_group = 0xC;
            }
            talk_group = (talk_group + (g->year * 8)) - 0x2F8;
            tour_yuina_club_script_begin();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
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

void tour_stage_yuina_club_computer_fortune(void) {
    GameState *g = &game_state;
    s32 girl_rank_3;
    u32 girl_rank;
    u32 girl_rank_2;

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
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUINA;
            portrait_show_checked();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            tour_yuina_club_script_begin();
            girl_rank = girl_state_rank(2);
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
                mode_step_set(0xA);
                break;
            }
            if (D_80122EA4 == 2) {
                mode_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[2].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            g->girls[2].flags.f.b2 = 1;
            talk_group = 4;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(2);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x1D;
            break;
        case 2:
            talk_group = 0x1E;
            break;
        case 3:
            talk_group = 0x1F;
            break;
        case 4:
            talk_group = 0x20;
            break;
        }
        if (D_80122EA4 == 2) {
            talk_group = 0x20;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(2) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        audio_command(0x202);
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_FORTUNE_TELLING);
        break;
    case 14:
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        mode_step_next();
        break;
    case 15:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            tour_yuina_club_script_begin();
            girl_rank_3 = girl_state_rank(2);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x21;
                break;
            case 2:
                talk_group = 0x22;
                break;
            case 3:
                talk_group = 0x23;
                break;
            }
            if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
                talk_group = D_80122EA8 + 0x24;
            }
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
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

void tour_stage_yuina_club_computer_game(void) {
    GameState *g = &game_state;
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;
    s32 girl_rank_4;

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
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUINA;
            portrait_show_checked();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(2);
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
            if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
                talk_group = 0x2A;
            }
            tour_yuina_club_script_begin();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[2].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            g->girls[2].flags.f.b2 = 1;
            talk_group = 4;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(2);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x27;
            break;
        case 2:
            talk_group = 0x28;
            break;
        case 3:
            talk_group = 0x29;
            break;
        }
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_set(0xC);
        } else {
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
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
        portrait_read_step(0x522E);
        break;
    case 19:
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        break;
    case 20:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            tour_yuina_club_script_begin();
            girl_rank_3 = girl_state_rank(2);
            switch (girl_rank_3) {
            case 0:
            case 1:
                talk_group = 0x2B;
                break;
            case 2:
                talk_group = 0x2D;
                break;
            case 3:
                talk_group = 0x2F;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
            if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
                talk_group = 0x34;
            }
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        mode_step_next();
        girl_rank_4 = girl_state_rank(2);
        switch (girl_rank_4) {
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
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_set(0x1A);
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        talk_group = 0;
        mode_step_next();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_yuina_club_computer_experiment(void) {
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
            portrait_read_start(0x522E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUINA;
            portrait_show_checked();
            cd_read_sectors(9U, 0x801E8000, 0x4A38);
        }
        break;
    case 5:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            tour_yuina_club_script_begin();
            girl_rank = girl_state_rank(2);
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
            if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
                mode_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[2].flags.f.b2) {
            mode_step_set(0xA);
        } else {
            talk_group = 4;
            g->girls[2].flags.f.b2 = 1;
            mode_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        girl_rank_2 = girl_state_rank(2);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x35;
            break;
        case 2:
            talk_group = 0x36;
            break;
        case 3:
            talk_group = 0x37;
            break;
        }
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            talk_group = 0x38;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 3;
        }
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 14:
        portrait_read_step(0x522E);
        break;
    case 15:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(9U, 0x801E8000, 0x4A38);
        break;
    case 16:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(2);
            switch (girl_rank_3) {
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
            }
            if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
                talk_group = 0x3C;
            }
            tour_yuina_club_script_begin();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) {
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

void tour_yuina_club_script_begin(void) {
    talk_names_set(2);
    tour_script_yuina_club();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
