#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80140F60(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 2;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 2:
        if ((girl_state_rank(2) == 4) && !(game_state.girls[2].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            mode_stage_set(5);
            return;
        }
        mode_stage_set(0x31);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x32);
            break;
        case 0x60:
            mode_stage_set(0x33);
            break;
        case 0x61:
            mode_stage_set(0x34);
            break;
        }
        if ((girl_state_rank(2) == 4) && !(game_state.girls[2].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            mode_stage_set(5);
            return;
        }
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void func_80141088(void) {
    GameState *g = &game_state;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
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
            func_80145844();
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
            func_80142864();
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
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
        temp_v0_2 = girl_state_rank(2);
        switch (temp_v0_2) {
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
        func_80145844();
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
            temp_v0_3 = girl_state_rank(2);
            switch (temp_v0_3) {
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
            func_80142864();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80141654(void) {
    GameState *g = &game_state;
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_80145844();
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
            func_80142864();
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
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
        temp_v0_2 = girl_state_rank(2);
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
        mode_stage_set(0xB);
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
            func_80142864();
            temp_v0_3 = girl_state_rank(2);
            switch (temp_v0_3) {
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80141B80(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

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
            func_80145844();
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
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
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
            func_80142864();
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
        temp_v0_2 = girl_state_rank(2);
        switch (temp_v0_2) {
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
        mode_stage_set(0x8D);
        break;
    case 14:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 15:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
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
            func_80142864();
            temp_v0_3 = girl_state_rank(2);
            switch (temp_v0_3) {
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
        temp_v0_4 = girl_state_rank(2);
        switch (temp_v0_4) {
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801422C4(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

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
            func_80145844();
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
            func_80142864();
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
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
        temp_v0_2 = girl_state_rank(2);
        switch (temp_v0_2) {
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
        func_80145844();
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
            temp_v0_3 = girl_state_rank(2);
            switch (temp_v0_3) {
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
            func_80142864();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80142864(void) {
    talk_names_set(2);
    func_8013216C();
    talk_line = 0;
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
    mode_step_next();
}
