/* megumi_block.c: src/ovl/BUNKAKEN/80150D30.c:4: Megumi block (stages 122-130). Original object 0x80150D30-0x80152680 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80150D30(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x60:
            mode_stage_set(0x7B);
            return;
        case 0x61:
            mode_stage_set(0x7C);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(0x7D);
        return;
    case 2:
        mode_stage_set(0x7E);
        return;
    case 3:
        switch (game_state.year) {
        case 0x60:
            mode_stage_set(0x7F);
            return;
        case 0x61:
            mode_stage_set(0x7E);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x60:
            mode_stage_set(0x80);
            return;
        case 0x61:
            mode_stage_set(0x81);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x82);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80150E88(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
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
        temp_v0_2 = girl_state_rank(9);
        switch (temp_v0_2) {
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80151104(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
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
        portrait_read_step(0x64D0);
        break;
    case 9:
        func_80145844();
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
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
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
            func_80152618();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801513E8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
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
            portrait_read_step(0x64D0);
        }
        break;
    case 10:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_80145844();
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
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
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
            func_80152618();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80151738(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        if (D_80122EB8 == 2) {
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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            if (D_80122EB8 == 3) {
                talk_group += 9;
            }
            func_80152618();
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
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
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
            func_80152618();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80151A78(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
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
        mode_stage_set(0x8D);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 9:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        text_marker_hide();
        talk_names_set(9U);
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
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
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
            func_80152618();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80151EC0(void) {
    s32 temp_v0;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
            portrait_layout_set(0U);
            func_80063668();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801520B4(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
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
        temp_v0_2 = girl_state_rank(9);
        switch (temp_v0_2) {
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80152300(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            func_80152618();
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
        portrait_read_step(0x64D0);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_80145844();
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
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
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
            func_80152618();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80152618(void) {
    talk_names_set(9);
    func_80132444();
    talk_line = 0;
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
    mode_step_next();
}
