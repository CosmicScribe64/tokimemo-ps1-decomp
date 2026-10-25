/* script_setters_shiori_yukari_blocks.c: src/ovl/BUNKAKEN/80132000.c:4: 28 script table setters, Shiori-companion block (stages 18-31), Yukari block (72-84). Original object 0x80132000-0x80137640 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void tour_script_invite_shiori(void) {
    tour_script_text = 0x801E8A20;
    tour_script_commands = 0x801E8BC8;
    tour_script_voices = 0x801E92F4;
}

void tour_script_shiori_club(void) {
    tour_script_text = 0x801EE270;
    tour_script_commands = 0x801EE93C;
    tour_script_voices = 0x801F2DA4;
}

void tour_script_invite_shiori_no_culture_club(void) {
    tour_script_text = 0x801E829C;
    tour_script_commands = 0x801E82E8;
    tour_script_voices = 0x801E8538;
}

void tour_script_shiori_companion(void) {
    tour_script_text = 0x801EBDEC;
    tour_script_commands = 0x801EC320;
    tour_script_voices = 0x801EF54C;
}

void tour_script_invite_mio(void) {
    tour_script_text = 0x801E8630;
    tour_script_commands = 0x801E872C;
    tour_script_voices = 0x801E8BEC;
}

void tour_script_mio_club(void) {
    tour_script_text = 0x801E9C78;
    tour_script_commands = 0x801E9E44;
    tour_script_voices = 0x801EB0AC;
}

void tour_script_invite_yuina(void) {
    tour_script_text = 0x801E860C;
    tour_script_commands = 0x801E8704;
    tour_script_voices = 0x801E8BA8;
}

void tour_script_yuina_club(void) {
    tour_script_text = 0x801EA384;
    tour_script_commands = 0x801EA614;
    tour_script_voices = 0x801EBFF8;
}

void tour_script_invite_ayako(void) {
    tour_script_text = 0x801E862C;
    tour_script_commands = 0x801E8724;
    tour_script_voices = 0x801E8BC8;
}

void tour_script_ayako_club(void) {
    tour_script_text = 0x801EA570;
    tour_script_commands = 0x801EA834;
    tour_script_voices = 0x801EC1DC;
}

void tour_script_invite_saki(void) {
    tour_script_text = 0x801E83F4;
    tour_script_commands = 0x801E8458;
    tour_script_voices = 0x801E87C8;
}

void tour_script_saki(void) {
    tour_script_text = 0x801EB18C;
    tour_script_commands = 0x801EB518;
    tour_script_voices = 0x801ED920;
}

void tour_script_invite_yukari(void) {
    tour_script_text = 0x801E8408;
    tour_script_commands = 0x801E845C;
    tour_script_voices = 0x801E8754;
}

void tour_script_yukari(void) {
    tour_script_text = 0x801EC9D4;
    tour_script_commands = 0x801ECE08;
    tour_script_voices = 0x801EFD50;
}

void tour_script_invite_nozomi(void) {
    tour_script_text = 0x801E8308;
    tour_script_commands = 0x801E8350;
    tour_script_voices = 0x801E85F4;
}

void tour_script_nozomi(void) {
    tour_script_text = 0x801EC134;
    tour_script_commands = 0x801EC63C;
    tour_script_voices = 0x801EF698;
}

void tour_script_invite_mira(void) {
    tour_script_text = 0x801E8494;
    tour_script_commands = 0x801E84E4;
    tour_script_voices = 0x801E8830;
}

void tour_script_mira(void) {
    tour_script_text = 0x801ED8DC;
    tour_script_commands = 0x801EDD8C;
    tour_script_voices = 0x801F15D8;
}

void tour_script_invite_yuko(void) {
    tour_script_text = 0x801E8564;
    tour_script_commands = 0x801E85F4;
    tour_script_voices = 0x801E89D0;
}

void tour_script_yuko(void) {
    tour_script_text = 0x801EC430;
    tour_script_commands = 0x801EC8BC;
    tour_script_voices = 0x801EF910;
}

void tour_script_invite_megumi(void) {
    tour_script_text = 0x801E838C;
    tour_script_commands = 0x801E83E4;
    tour_script_voices = 0x801E86FC;
}

void tour_script_megumi(void) {
    tour_script_text = 0x801EA150;
    tour_script_commands = 0x801EA3E0;
    tour_script_voices = 0x801EC088;
}

void tour_script_invite_yumi(void) {
    tour_script_text = 0x801E840C;
    tour_script_commands = 0x801E845C;
    tour_script_voices = 0x801E87A8;
}

void tour_script_yumi(void) {
    tour_script_text = 0x801EB950;
    tour_script_commands = 0x801EBC94;
    tour_script_voices = 0x801EE3D0;
}

void tour_script_common(void) {
    tour_script_text = 0x801E92FC;
    tour_script_commands = 0x801E9598;
    tour_script_voices = 0x801EA298;
}

void tour_script_narrator(void) {
    tour_script_text = 0x801E81BC;
    tour_script_commands = 0x801E81CC;
    tour_script_voices = 0x801E82CC;
}

void tour_script_fortune(void) {
    tour_script_text = 0x801E81CC;
    tour_script_commands = 0x801E8204;
    tour_script_voices = 0x801E82E4;
}

void tour_script_birthday(void) {
    tour_script_text = 0x801E8A10;
    tour_script_commands = 0x801E8B7C;
    tour_script_voices = 0x801E93D4;
}

void tour_stage_shiori_companion_route(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_LITERATURE_POEMS);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_LITERATURE_ANTHOLOGY);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_LITERATURE_SPEECH);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_DRAMA);
        return;
    case 2:
        mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_COMPUTER_EXPERIMENT);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_ART_PORTRAIT);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_ART_SKETCH);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_ART_OIL);
            return;
        }
        break;
    case 5:
        if (game_state.year == 0x61) {
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_BRASS_1997);
            return;
        }
        mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_shiori_companion_literature_poems(void) {
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 6;
                break;
            case 3:
                talk_group = 9;
                break;
            }
            tour_shiori_companion_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group = 0x89;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 2;
            break;
        case 2:
            talk_group = 7;
            break;
        case 3:
            talk_group = 0xA;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0) == 3) {
            if (game_state.param_liberal_arts.value >= 0x78) {
                talk_group = 0xB;
            } else {
                talk_group = 0xC;
            }
        } else if (game_state.param_liberal_arts.value >= 0x78) {
            talk_group = 3;
        } else {
            talk_group = 4;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 5;
            break;
        case 2:
            talk_group = 8;
            break;
        case 3:
            talk_group = 0xD;
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


void tour_stage_shiori_companion_literature_anthology(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0xE;
                break;
            case 2:
                talk_group = 0x11;
                break;
            case 3:
                talk_group = 0x14;
                break;
            }
            tour_shiori_companion_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group = 0x89;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0xF;
            break;
        case 2:
            talk_group = 0x12;
            break;
        case 3:
            talk_group = 0x15;
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

void tour_stage_shiori_companion_literature_speech(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
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
            tour_shiori_companion_script_begin();
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
        portrait_read_step(0x544A);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
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
            tour_shiori_companion_script_begin();
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


void tour_stage_shiori_companion_drama(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
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
            }
            talk_group = (talk_group + (game_state.year * 6)) - 0x23A;
            tour_shiori_companion_script_begin();
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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 10:
        portrait_read_step(0x544A);
        break;
    case 11:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x1E;
                break;
            case 2:
                talk_group = 0x20;
                break;
            case 3:
                talk_group = 0x22;
                break;
            }
            talk_group = (talk_group + (game_state.year * 6)) - 0x23A;
            tour_shiori_companion_script_begin();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_bggirl_fadeout();
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


void tour_stage_shiori_companion_science(void) {
    s32 girl_rank;
    s32 girl_rank_2;

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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x2F;
                break;
            case 2:
                talk_group = 0x30;
                break;
            case 3:
                talk_group = 0x31;
                break;
            }
            tour_shiori_companion_script_begin();
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
        portrait_read_step(0x544A);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x32;
                break;
            case 2:
                talk_group = 0x33;
                break;
            case 3:
                talk_group = 0x34;
                break;
            }
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            tour_shiori_companion_script_begin();
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


void tour_stage_shiori_companion_computer_fortune(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x3B;
                break;
            case 2:
                talk_group = 0x40;
                break;
            case 3:
                talk_group = 0x45;
                break;
            }
            tour_shiori_companion_script_begin();
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
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        mode_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x3C;
                break;
            case 2:
                talk_group = 0x41;
                break;
            case 3:
                talk_group = 0x46;
                break;
            }
            talk_group += D_80122EA8;
            tour_shiori_companion_script_begin();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x3F;
            break;
        case 2:
            talk_group = 0x44;
            break;
        case 3:
            talk_group = 0x49;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_bggirl_fadeout();
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


void tour_stage_shiori_companion_computer_game(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x4A;
                break;
            case 2:
                talk_group = 0x4E;
                break;
            case 3:
                talk_group = 0x52;
                break;
            }
            tour_shiori_companion_script_begin();
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
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(TOUR_STAGE_TT_NARRATOR);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
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
        talk_names_set(0U);
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
        portrait_read_step(0x544A);
        break;
    case 13:
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x4B;
                break;
            case 2:
                talk_group = 0x4F;
                break;
            case 3:
                talk_group = 0x53;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
            tour_shiori_companion_script_begin();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x4D;
            break;
        case 2:
            talk_group = 0x51;
            break;
        case 3:
            talk_group = 0x55;
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


void tour_stage_shiori_companion_computer_experiment(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
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
            tour_shiori_companion_script_begin();
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
        portrait_read_step(0x544A);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x57;
                break;
            case 2:
                talk_group = 0x59;
                break;
            case 3:
                talk_group = 0x5B;
                break;
            }
            tour_shiori_companion_script_begin();
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


void tour_stage_shiori_companion_art_portrait(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x5C;
                break;
            case 2:
                talk_group = 0x5F;
                break;
            case 3:
                talk_group = 0x62;
                break;
            }
            tour_shiori_companion_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if (girl_state_rank(0) == 3) {
            mode_step_set(0xC);
        } else {
            mode_step_next();
        }
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (girl_state_rank(0) == 2) {
            talk_group = 0x61;
        } else {
            talk_group = 0x5E;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bggirl_fadeout();
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

void tour_stage_shiori_companion_art_sketch(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x63;
                break;
            case 2:
                talk_group = 0x65;
                break;
            case 3:
                talk_group = 0x67;
                break;
            }
            tour_shiori_companion_script_begin();
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
        normal_date_bg_fadeout();
        break;
    case 9:
        talk_group = 0;
        mode_step_next();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_shiori_companion_art_oil(void) {
    s32 girl_rank;
    s32 girl_rank_2;
    s32 girl_rank_3;

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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x69;
                break;
            case 2:
                talk_group = 0x6E;
                break;
            case 3:
                talk_group = 0x71;
                break;
            }
            tour_shiori_companion_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group = 0x89;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x6A;
            break;
        case 2:
            talk_group = 0x6F;
            break;
        case 3:
            talk_group = 0x72;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0) == 3) {
            if (game_state.param_art.value >= 0x78) {
                talk_group = 0x73;
            } else {
                talk_group = 0x74;
            }
        } else if (game_state.param_art.value >= 0x78) {
            talk_group = 0x6B;
        } else {
            talk_group = 0x6C;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x6D;
            break;
        case 2:
            talk_group = 0x70;
            break;
        case 3:
            talk_group = 0x75;
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


void tour_stage_shiori_companion_brass(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x76;
                break;
            case 2:
                talk_group = 0x77;
                break;
            case 3:
                talk_group = 0x78;
                break;
            }
            tour_shiori_companion_script_begin();
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
        portrait_read_step(0x544A);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x79;
                break;
            case 2:
                talk_group = 0x7A;
                break;
            case 3:
                talk_group = 0x7B;
                break;
            }
            if (game_state.year == 0x60) {
                talk_group += 3;
            }
            tour_shiori_companion_script_begin();
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


void tour_stage_shiori_companion_brass_1997(void) {
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
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(0);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x76;
                break;
            case 2:
                talk_group = 0x77;
                break;
            case 3:
                talk_group = 0x78;
                break;
            }
            tour_shiori_companion_script_begin();
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
        portrait_read_step(0x544A);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(0);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x7F;
                break;
            case 2:
                talk_group = 0x83;
                break;
            case 3:
                talk_group = 0x87;
                break;
            }
            tour_shiori_companion_script_begin();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(0) == 2) {
            if (game_state.param_art.value >= 0x78) {
                talk_group = 0x84;
            } else {
                talk_group = 0x85;
            }
        } else if (game_state.param_art.value >= 0x78) {
            talk_group = 0x80;
        } else {
            talk_group = 0x81;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x82;
            break;
        case 2:
            talk_group = 0x86;
            break;
        case 3:
            talk_group = 0x88;
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


void tour_shiori_companion_script_begin(void) {
    talk_names_set(0);
    tour_script_shiori_companion();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}

void tour_stage_yukari_route(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_YUKARI_LITERATURE_POEMS);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_YUKARI_LITERATURE_ANTHOLOGY);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_YUKARI_LITERATURE_SPEECH);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(TOUR_STAGE_YUKARI_DRAMA);
        return;
    case 2:
        mode_stage_set(TOUR_STAGE_YUKARI_SCIENCE);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_YUKARI_COMPUTER_FORTUNE);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_YUKARI_COMPUTER_GAME);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_YUKARI_COMPUTER_EXPERIMENT);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(TOUR_STAGE_YUKARI_ART_PORTRAIT);
            return;
        case 0x60:
            mode_stage_set(TOUR_STAGE_YUKARI_ART_SKETCH);
            return;
        case 0x61:
            mode_stage_set(TOUR_STAGE_YUKARI_ART_OIL);
            return;
        }
        break;
    case 5:
        mode_stage_set(TOUR_STAGE_YUKARI_BRASS);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_stage_yukari_literature_poems(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
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
            tour_yukari_script_begin();
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
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 9;
            break;
        case 3:
            talk_group = 0xA;
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

void tour_stage_yukari_literature_anthology(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
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
            }
            tour_yukari_script_begin();
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
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0xB;
            break;
        case 2:
            talk_group = 0xD;
            break;
        case 3:
            talk_group = 0xF;
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

void tour_stage_yukari_literature_speech(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
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
            tour_yukari_script_begin();
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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(5);
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
            tour_yukari_script_begin();
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


void tour_stage_yukari_drama(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x17;
                break;
            case 2:
                talk_group = 0x18;
                break;
            case 3:
                talk_group = 0x19;
                break;
            }
            tour_yukari_script_begin();
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
            portrait_read_step(0x5FB7);
        }
        break;
    case 10:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(5);
            switch (girl_rank_2) {
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
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            tour_yukari_script_begin();
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


void tour_stage_yukari_science(void) {
    s32 girl_rank;
    s32 girl_rank_2;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x23;
                break;
            case 2:
                talk_group = 0x24;
                break;
            case 3:
                talk_group = 0x25;
                break;
            }
            tour_yukari_script_begin();
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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(5);
            switch (girl_rank_2) {
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
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            tour_yukari_script_begin();
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


void tour_stage_yukari_computer_fortune(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x2F;
                break;
            case 2:
                talk_group = 0x30;
                break;
            case 3:
                talk_group = 0x31;
                break;
            }
            tour_yukari_script_begin();
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
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        mode_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            talk_group = D_80122EA8 + 0x32;
            tour_yukari_script_begin();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        girl_rank_2 = girl_state_rank(5);
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


void tour_stage_yukari_computer_game(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x38;
                break;
            case 2:
                talk_group = 0x39;
                break;
            case 3:
                talk_group = 0x3A;
                break;
            }
            tour_yukari_script_begin();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group += 2;
        mode_step_next();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 1;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_TT_GAME);
        break;
    case 10:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(TOUR_STAGE_RESUME);
        break;
    case 11:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(5U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 12:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 14:
        portrait_read_step(0x5FB7);
        break;
    case 15:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 16:
        normal_date_bggirl_fadein();
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(5);
            switch (girl_rank_2) {
            case 0:
            case 1:
                talk_group = 0x3E;
                break;
            case 2:
                talk_group = 0x3F;
                break;
            case 3:
                talk_group = 0x40;
                break;
            }
            tour_yukari_script_begin();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(TOUR_STAGE_TT_NARRATOR);
        break;
    case 21:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(TOUR_STAGE_RESUME);
        break;
    case 22:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(5U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 23:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 25:
        portrait_read_step(0x5FB7);
        break;
    case 26:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 27:
        normal_date_bggirl_fadein();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            girl_rank_3 = girl_state_rank(5);
            switch (girl_rank_3) {
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
            tour_yukari_script_begin();
        }
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_bggirl_fadeout();
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
        tour_finish_visit();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void tour_stage_yukari_computer_experiment(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x23;
                break;
            case 2:
                talk_group = 0x24;
                break;
            case 3:
                talk_group = 0x25;
                break;
            }
            tour_yukari_script_begin();
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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(5);
            switch (girl_rank_2) {
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
            tour_yukari_script_begin();
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


void tour_stage_yukari_art_portrait(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x4A;
                break;
            case 2:
                talk_group = 0x4D;
                break;
            case 3:
                talk_group = 0x50;
                break;
            }
            tour_yukari_script_begin();
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

void tour_stage_yukari_art_sketch(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x53;
                break;
            case 2:
                talk_group = 0x55;
                break;
            case 3:
                talk_group = 0x57;
                break;
            }
            tour_yukari_script_begin();
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

void tour_stage_yukari_art_oil(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x59;
                break;
            case 2:
                talk_group = 0x5A;
                break;
            case 3:
                talk_group = 0x5B;
                break;
            }
            tour_yukari_script_begin();
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
        girl_rank_2 = girl_state_rank(5);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x5C;
            break;
        case 2:
            talk_group = 0x5D;
            break;
        case 3:
            talk_group = 0x5E;
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

void tour_stage_yukari_brass(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            girl_rank = girl_state_rank(5);
            switch (girl_rank) {
            case 0:
            case 1:
                talk_group = 0x5F;
                break;
            case 2:
                talk_group = 0x60;
                break;
            case 3:
                talk_group = 0x61;
                break;
            }
            tour_yukari_script_begin();
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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        tour_window_title_set();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            girl_rank_2 = girl_state_rank(5);
            switch (girl_rank_2) {
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
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            tour_yukari_script_begin();
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


void tour_yukari_script_begin(void) {
    talk_names_set(5);
    tour_script_yukari();
    talk_line = 0;
    talk_text_table = tour_script_text;
    talk_command_table = tour_script_commands;
    talk_voice_table = tour_script_voices;
    mode_step_next();
}
