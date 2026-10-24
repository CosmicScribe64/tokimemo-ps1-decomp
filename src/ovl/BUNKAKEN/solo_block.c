/* solo_block.c: src/ovl/BUNKAKEN/80139E50.c:4: solo exhibits (stages 5-10, 12-17) and the solo script helper. Original object 0x80139E50-0x8013B510 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80139E50(void) {
    switch (D_80122EB8) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(7);
            return;
        }
        mode_stage_set(6);
        return;
    case 1:
        mode_stage_set(8);
        return;
    case 2:
        mode_stage_set(9);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0xA);
            return;
        case 0x60:
            mode_stage_set(0xC);
            return;
        case 0x61:
            mode_stage_set(0xD);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0xE);
            return;
        case 0x60:
            mode_stage_set(0xF);
            return;
        case 0x61:
            mode_stage_set(0x10);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x11);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80139FB8(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013A118(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145844();
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
            func_8013B4A0();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013A2FC(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
            func_80145844();
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x39;
            func_8013B4A0();
        }
        break;
    case 14:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013A518(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            talk_group = game_state.year - 0x24;
            func_8013B4A0();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013A6F4(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        mode_stage_set(0xB);
        break;
    case 7:
        D_80122EBC = 0xFF;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            talk_group = D_80122EA8 + 0x3F;
            func_8013B4A0();
        }
        break;
    case 11:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013A8CC(void) {
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
            func_80145844();
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
        mode_stage_set(0x8D);
        break;
    case 7:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 8:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
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
            func_8013B4A0();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013ABF0(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        mode_step_next();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x45;
            func_8013B4A0();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013ADC4(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013AF00(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013B170(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013B2AC(void) {
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
            func_80145844();
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
            func_8013B4A0();
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
        func_80145844();
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
            func_8013B4A0();
        }
        break;
    case 13:
        talk_group = 0x52;
        mode_step_next();
        break;
    case 15:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013B4A0(void) {
    talk_names_set(0);
    func_801324E0();
    talk_line = 0;
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
    mode_step_next();
}
