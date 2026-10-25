/* solo_block.c: src/ovl/BUNKAKEN/80139E50.c:4: solo exhibits (stages 5-10, 12-17) and the solo script helper. Original object 0x80139E50-0x8013B510 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void tour_stage_solo_route(void) {
    switch (D_80122EB8) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(TOUR_STAGE_SOLO_LITERATURE_SPEECH);
            return;
        }
        mode_stage_set(TOUR_STAGE_SOLO_LITERATURE_COLLECTION);
        return;
    case 1:
        mode_stage_set(TOUR_STAGE_SOLO_DRAMA);
        return;
    case 2:
        mode_stage_set(TOUR_STAGE_SOLO_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SOLO_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SOLO_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SOLO_COMPUTER_EXPERIMENT);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SOLO_ART_PORTRAIT);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SOLO_ART_SKETCH);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SOLO_ART_OIL);
            return;
        }
        break;
    case 5:
        mode_stage_set(TOUR_STAGE_SOLO_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_solo_literature_collection(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 10:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
    case 8:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (game_state.year == 0x5F) {
                talk_group = 0x30;
            } else {
                talk_group = 0x33;
            }
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 7:
    case 9:
    case 12:
        normal_date_speak();
        break;
    case 11:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 13:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_literature_speech(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 12:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x36;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 11:
    case 14:
        normal_date_speak();
        break;
    case 7:
        D_80122EAC = 4;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 9:
        normal_date_bg_fadein();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x37;
            tour_solo_script_begin();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_drama(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 13:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
    case 10:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x38;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 12:
    case 15:
        normal_date_speak();
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
            tour_window_title_set();
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x39;
            tour_solo_script_begin();
        }
        break;
    case 14:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 16:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_science(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 12:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
    case 9:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x3A;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 11:
    case 14:
        normal_date_speak();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            talk_group = game_state.year - 0x24;
            tour_solo_script_begin();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_computer_fortune(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
    case 10:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x3E;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 9:
    case 12:
        normal_date_speak();
        break;
    case 6:
        audio_command(0x202);
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(TOUR_STAGE_FORTUNE_TELLING);
        break;
    case 7:
        D_80122EBC = 0xFF;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            talk_group = D_80122EA8 + 0x3F;
            tour_solo_script_begin();
        }
        break;
    case 11:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 13:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_computer_game(void) {
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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            tour_window_title_set();
            talk_group = 0x42;
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        mode_step_next();
        break;
    case 6:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        g->saved_game_mode = g->game_mode;
        g->saved_mode_stage = g->mode_stage;
        g->saved_mode_step = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_TT_NARRATOR);
        break;
    case 7:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(TOUR_STAGE_RESUME);
        break;
    case 8:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        message_window_init();
        text_marker_hide();
        talk_names_set(0U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 9:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 11:
        normal_date_bg_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x43;
            tour_solo_script_begin();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_computer_experiment(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 12:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
    case 9:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x44;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 11:
    case 14:
        normal_date_speak();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x45;
            tour_solo_script_begin();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_art_portrait(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x46;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 8:
        normal_date_speak();
        break;
    case 7:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 9:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_art_sketch(void) {
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
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            tour_solo_script_begin();
            talk_names_set(0xDU);
            talk_group = 0x47;
        }
        break;
    case 5:
        portrait_read_step(0x6C5F);
        break;
    case 6:
        addr_init_bustup();
        game_state.current_girl = GIRL_YOSHIO;
        portrait_show_checked();
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
        normal_date_two_select();
        break;
    case 11:
        if (D_80122CDC != 0) {
            talk_group = 0x4B;
            mode_step_set(0x11);
        } else {
            mode_step_next();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        mode_step_set(0x14);
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        talk_group = 0x52;
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

void tour_stage_solo_art_oil(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 10:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
    case 8:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x4C;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 7:
    case 9:
    case 12:
        normal_date_speak();
        break;
    case 11:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 13:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_solo_brass(void) {
    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 1:
    case 6:
    case 12:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            tour_window_title_set();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 3:
    case 9:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x4F;
            tour_solo_script_begin();
        }
        break;
    case 5:
    case 11:
    case 14:
        normal_date_speak();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            if (game_state.year == 0x60) {
                talk_group = 0x51;
            } else {
                talk_group = 0x50;
            }
            tour_solo_script_begin();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_solo_script_begin(void) {
    talk_names_set(0);
    tour_script_common();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
