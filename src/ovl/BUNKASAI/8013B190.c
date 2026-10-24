#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013B190(void) {
    CultureFestivalDispatch *state;

    state = (CultureFestivalDispatch *)&D_80122EB8;
    state->unk_04 = 2;
    game_state.girls[2].flags.f.b2 = 1;
    switch (state->unk_00) {
    case 2:
        mode_stage_set(0x3C);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x3D);
            return;
        case 0x60:
            mode_stage_set(0x3F);
            return;
        case 0x61:
            mode_stage_set(0x40);
            return;
        }
        break;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013B25C(void) {
    GameState *game = &game_state;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game->mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x525B);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_801431D4();
        addr_init_bustup();
        game->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
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
            case 4:
                talk_group = 5;
                break;
            }
            talk_group = (talk_group + (game->year * 4)) - 0x17C;
            func_8013C3C4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((game->year == 0x5F) && (girl_state_rank(2) == 4)) {
            talk_group = 0x45;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((game->year == 0x5F) && (girl_state_rank(2) == 4)) {
            normal_date_speak();
        } else {
            mode_step_set(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EAC = 3;
        D_80122EB0 = (s32) game->mode_stage;
        D_80122EB4 = game->mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 11:
        portrait_read_step(0x525B);
        break;
    case 12:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game->current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 13:
        normal_date_bggirl_fadein();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(2);
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
            case 4:
                talk_group = 0x11;
                break;
            }
            talk_group = (talk_group + (game->year * 4)) - 0x17C;
            if (game->year == 0x5F) {
                talk_group += 0xC;
            }
            func_8013C3C4();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 1;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013B688(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            portrait_read_start(0x522E);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            portrait_show_checked();
            cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                talk_group = 0x1E;
                break;
            case 3:
                talk_group = 0x20;
                break;
            case 4:
                talk_group = 0x22;
                break;
            }
            func_8013C3C4();
            if ((girl_state_rank(2) == 0) && !(game_state.girls[2].syoushin & 1)) {
                talk_group = 0x34;
                mode_step_set(0x1B);
            }
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (girl_state_rank(2) < 3) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        if (girl_state_rank(2) < 3) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 10:
        audio_command(0x202);
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 11:
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        mode_step_next();
        break;
    case 12:
        if (girl_state_rank(2) < 3) {
            normal_date_girl_in();
        } else {
            mode_step_next();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            func_8013C3C4();
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = (D_80122EA8 * 2) + 0x24;
                break;
            case 2:
                talk_group = (D_80122EA8 * 2) + 0x2A;
                break;
            case 3:
            case 4:
                talk_group = D_80122EA8 + 0x30;
                mode_step_set(0x18);
                break;
            }
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((girl_state_rank(2) < 2) && (D_80122EA8 != 1)) {
            audio_command(0x508);
        }
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(2) == 2) {
            talk_group = 0x33;
            mode_step_next();
        } else {
            mode_step_set(0x13);
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        talk_group = 1;
        mode_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80142BC8();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_bg_fadeout();
        break;
    case 26:
        mode_step_set(0x15);
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_girl_in();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        mode_step_set(0x13);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* T-12702: return flags precede state copies, as in sibling handlers. */
void func_8013BB94(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
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
            mode_step_next();
        }
        break;
    case 3:
        portrait_read_step(0x522E);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        portrait_show_checked();
        func_801431D4();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 5:
        normal_date_bggirl_fadein();
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                talk_group = 0x37;
                break;
            case 3:
                talk_group = 0x39;
                break;
            case 4:
                talk_group = 0x3B;
                break;
            }
            func_8013C3C4();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (girl_state_rank(2) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 11:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0xA4);
        break;
    case 12:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 13:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(2U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 14:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 16:
        portrait_read_step(0x522E);
        break;
    case 17:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 18:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
            case 2:
                talk_group = 0x38;
                break;
            case 3:
                talk_group = 0x3A;
                break;
            case 4:
                talk_group = 0x44;
                break;
            }
            func_8013C3C4();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 22:
        talk_group = 1;
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013C0AC(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x522E);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_801431D4();
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                talk_group = 0x3E;
                break;
            case 3:
                talk_group = 0x3F;
                break;
            case 4:
                talk_group = 0x40;
                break;
            }
            func_8013C3C4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 3;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 9:
        portrait_read_step(0x522E);
        break;
    case 10:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4A2C);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
            case 2:
                talk_group = 0x41;
                break;
            case 3:
                talk_group = 0x42;
                break;
            case 4:
                talk_group = 0x43;
                break;
            }
            func_8013C3C4();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        talk_group = 1;
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013C3C4(void) {
    talk_names_set(2);
    func_80132138();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}
