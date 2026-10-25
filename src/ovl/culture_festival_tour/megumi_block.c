/* megumi_block.c: src/ovl/BUNKAKEN/80150D30.c:4: Megumi block (stages 122-130). Original object 0x80150D30-0x80152680 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/culture_festival_tour.h"

void tour_stage_megumi_route(void) {
    switch (culture_exhibit_club) {
    case 0:
        switch (game_state.year) {
        case 0x60:
            mode_stage_set(TOUR_STAGE_MEGUMI_LITERATURE_ANTHOLOGY);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_MEGUMI_LITERATURE_SPEECH);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(TOUR_STAGE_MEGUMI_DRAMA);
        return;
    case 2:
        mode_stage_set(TOUR_STAGE_MEGUMI_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x60:
            mode_stage_set(TOUR_STAGE_MEGUMI_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_MEGUMI_SCIENCE);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x60:
            mode_stage_set(TOUR_STAGE_MEGUMI_ART_SKETCH);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_MEGUMI_ART_OIL);
            return;
        }
        break;
    case 5:
        mode_stage_set(TOUR_STAGE_MEGUMI_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_megumi_literature_anthology(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 2;
                break;
            case 2:
                talk_group = 3;
                break;
            case 3:
                talk_group = 4;
                break;
            }
            tour_megumi_script_begin();
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
        girl_rank_2 = girl_state_rank(9);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 5;
            break;
        case 2:
            talk_group = 7;
            break;
        case 3:
            talk_group = 9;
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

void tour_stage_megumi_literature_speech(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xB;
                break;
            case 2:
                talk_group = 0xC;
                break;
            case 3:
                talk_group = 0xD;
                break;
            }
            tour_megumi_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x64D0);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(9);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0xE;
                break;
            case 2:
                talk_group = 0xF;
                break;
            case 3:
                talk_group = 0x10;
                break;
            }
            tour_megumi_script_begin();
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

void tour_stage_megumi_drama(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
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
            tour_megumi_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
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
            portrait_read_step(0x64D0);
        }
        break;
    case 10:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(9);
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
            if (game_state.year == 0x61) {
                talk_group += 3;
            }
            tour_megumi_script_begin();
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

void tour_stage_megumi_science(void) {
    s32 girl_rank;
    s32 girl_rank_2;

    switch (game_state.mode_step) {
    case 0:
        if (culture_exhibit_club == 2) {
            dec_bg_cd_read(0x40FC, 1);
        } else {
            dec_bg_cd_read(0x4105, 1);
        }
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x1A;
                break;
            case 2:
                talk_group = 0x1B;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
            if (culture_exhibit_club == 3) {
                talk_group += 9;
            }
            tour_megumi_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 1;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x64D0);
        break;
    case 9:
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(9);
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
            }
            if (game_state.year == 0x61) {
                talk_group += 3;
            }
            tour_megumi_script_begin();
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

void tour_stage_megumi_computer_game(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x26;
                break;
            case 2:
                talk_group = 0x27;
                break;
            case 3:
                talk_group = 0x28;
                break;
            }
            tour_megumi_script_begin();
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
        ((ExtraModeFlags *)&D_800E7D4F)->stage_select = 0;
        g->saved_game_mode = g->game_mode;
        g->saved_mode_stage = g->mode_stage;
        g->saved_mode_step = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_TT_NARRATOR);
        break;
    case 8:
        culture_festival_return_gate = 1;
        saved_return_stage = (s32) g->mode_stage;
        saved_return_step = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_RESUME);
        break;
    case 9:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        message_window_init();
        text_marker_hide();
        talk_names_set(9U);
        culture_festival_return_gate = 0;
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
        portrait_read_step(0x64D0);
        break;
    case 13:
        addr_init_bustup();
        g->current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        if (girl_state_rank(9) < 2) {
            portrait_layout_set(0U);
        }
        cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(9);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x29;
                break;
            case 2:
                talk_group = 0x2A;
                break;
            case 3:
                talk_group = 0x2B;
                break;
            }
            tour_megumi_script_begin();
        }
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
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_megumi_art_sketch(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x2C;
                break;
            case 2:
                talk_group = 0x2E;
                break;
            case 3:
                talk_group = 0x2C;
                break;
            }
            tour_megumi_script_begin();
            portrait_layout_set(0U);
            portrait_face_update();
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

void tour_stage_megumi_art_oil(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
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
            tour_megumi_script_begin();
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
        girl_rank_2 = girl_state_rank(9);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x33;
            break;
        case 2:
            talk_group = 0x34;
            break;
        case 3:
            talk_group = 0x35;
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

void tour_stage_megumi_brass(void) {
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
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(9);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x36;
                break;
            case 2:
                talk_group = 0x37;
                break;
            case 3:
                talk_group = 0x38;
                break;
            }
            tour_megumi_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        festival_dispatch_variant = 0;
        saved_return_stage = (s32) game_state.mode_stage;
        saved_return_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x64D0);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_MEGUMI;
            portrait_show_checked();
            cd_read_sectors(9U, 0x801E8000, 0x4B3E);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(9);
            switch (girl_rank_2) {
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
            if (game_state.year == 0x61) {
                talk_group += 3;
            }
            tour_megumi_script_begin();
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

void tour_megumi_script_begin(void) {
    talk_names_set(9);
    tour_script_megumi();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
