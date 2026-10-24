#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80158EB0(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 9;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 0:
        if (game_state.year == 0x60) {
            mode_stage_set(0x92);
            return;
        }
        mode_stage_set(0x93);
        return;
    case 1:
        mode_stage_set(0x94);
        return;
    case 2:
        mode_stage_set(0x95);
        return;
    case 3:
        if (game_state.year == 0x60) {
            mode_stage_set(0x96);
            return;
        }
        mode_stage_set(0x95);
        return;
    case 4:
        if (game_state.year == 0x60) {
            mode_stage_set(0x97);
            return;
        }
        mode_stage_set(0x98);
        return;
    case 5:
        mode_stage_set(0x99);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80158FD4(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x64D0);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MEGUMI;
            portrait_show_checked();
            talk_group = 0x25;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0xC;
                break;
            case 2:
                talk_group = 0xD;
                break;
            case 3:
                talk_group = 0xE;
                break;
            case 4:
                talk_group = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(9) == 4) {
            mode_step_set(0xC);
        } else {
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 3;
                break;
            case 3:
                talk_group = 4;
                break;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        if ((girl_state_rank(9) < 2) || (girl_state_rank(9) == 4)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(9) != 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[9].flags.f.b12 = 1;
            } else {
                game_state.girls[9].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80159344(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        talk_group = 0x26;
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x64D0);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            case 4:
                talk_group = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        temp_v0_2 = girl_state_rank(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 3;
            break;
        case 3:
            talk_group = 4;
            break;
        case 4:
            mode_step_set(0xF);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if ((girl_state_rank(9) < 2) || (girl_state_rank(9) == 4)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
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

void func_80159698(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        talk_group = 0x28;
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x64D0);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
            case 0:
            case 1:
                if (game_state.year == 0x60) {
                    talk_group = 0x12;
                } else {
                    talk_group = 0x15;
                }
                break;
            case 2:
                if (game_state.year == 0x60) {
                    talk_group = 0x13;
                } else {
                    talk_group = 0x16;
                }
                break;
            case 3:
                if (game_state.year == 0x60) {
                    talk_group = 0x14;
                } else {
                    talk_group = 0x17;
                }
                break;
            case 4:
                talk_group = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        temp_v0_2 = girl_state_rank(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            portrait_layout_set(0U);
            func_80063668();
            talk_group = 7;
            break;
        case 2:
            talk_group = 3;
            break;
        case 3:
            talk_group = 4;
            break;
        case 4:
            mode_step_set(0x10);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((girl_state_rank(9) < 2) && (game_state.year == 0x60)) {
            talk_group += 1;
        }
        mode_step_next();
        /* fallthrough */
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        if ((girl_state_rank(9) == 4) || (girl_state_rank(9) < 2)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        talk_group = 0;
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80159AD4(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (game_state.mode_step) {
    case 0:
        if (((u16) game_state.player_flags.h >> 0xC) == 2) {
            dec_bg_cd_read(0x40FC, 1);
        } else {
            dec_bg_cd_read(0x4105, 1);
        }
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x64D0);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MEGUMI;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x19;
                break;
            case 3:
                talk_group = 0x1A;
                break;
            case 4:
                talk_group = 0x1B;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 12:
        portrait_read_step(0x64D0);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        break;
    case 14:
        if (girl_state_rank(9) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                if (game_state.year == 0x60) {
                    talk_group = 0x1C;
                } else {
                    talk_group = 0x20;
                }
                break;
            case 2:
                if (game_state.year == 0x60) {
                    talk_group = 0x1D;
                } else {
                    talk_group = 0x21;
                }
                break;
            case 3:
                if (game_state.year == 0x60) {
                    talk_group = 0x1E;
                } else {
                    talk_group = 0x22;
                }
                break;
            case 4:
                talk_group = 0x1F;
                break;
            }
            func_8015AF20();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        mode_step_next();
        temp_v0_3 = girl_state_rank(9);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 3;
            break;
        case 3:
            talk_group = 4;
            break;
        case 4:
            mode_step_set(0x15);
            break;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        if (girl_state_rank(9) < 2) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 21:
        normal_date_bg_fadeout();
        break;
    case 22:
        talk_group = 0;
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80159FBC(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
        portrait_read_step(0x64D0);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        func_801431D4();
        talk_group = 0x2A;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
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
            case 4:
                talk_group = 0x26;
                break;
            }
            func_8015AF20();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(9) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(9) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 15:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0xA4);
        break;
    case 16:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 17:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(9U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 18:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 20:
        portrait_read_step(0x64D0);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        break;
    case 22:
        if (girl_state_rank(9) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x28;
                break;
            case 2:
                talk_group = 0x29;
                break;
            case 3:
                talk_group = 0x2A;
                break;
            case 4:
                talk_group = 0x2B;
                break;
            }
            func_8015AF20();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        mode_step_next();
        temp_v0_3 = girl_state_rank(9);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 3;
            break;
        case 3:
            talk_group = 4;
            break;
        case 4:
            mode_step_set(0x1D);
            break;
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_girl_out();
        break;
    case 28:
        if (girl_state_rank(9) < 2) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 29:
        normal_date_bg_fadeout();
        break;
    case 30:
        talk_group = 0;
        mode_step_next();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8015A5DC(void) {
    u32 temp_v0;

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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x64D0);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MEGUMI;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x2D;
                break;
            case 2:
                talk_group = 0x30;
                break;
            case 3:
                talk_group = 0x33;
                break;
            case 4:
                talk_group = 0x36;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(9) != 4) {
            portrait_layout_set(0U);
            func_80063668();
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8015A870(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x64D0);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MEGUMI;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x39;
                break;
            case 2:
                talk_group = 0x3B;
                break;
            case 3:
                talk_group = 0x3C;
                break;
            case 4:
                talk_group = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        mode_step_next();
        temp_v0_2 = girl_state_rank(9);
        switch (temp_v0_2) {
        case 2:
            talk_group = 3;
            break;
        case 3:
            talk_group = 4;
            break;
        case 4:
            mode_step_set(0xC);
            break;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        if (girl_state_rank(9) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        talk_group = 0;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8015AB58(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        talk_group = 0x2F;
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x64D0);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MEGUMI;
        portrait_show_checked();
        cd_read_sectors(0xAU, 0x801E8000, 0x4B33);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(9);
            switch (temp_v0) {
            case 0:
            case 1:
                if (game_state.year == 0x60) {
                    talk_group = 0x3D;
                } else {
                    talk_group = 0x40;
                }
                break;
            case 2:
                if (game_state.year == 0x60) {
                    talk_group = 0x3E;
                } else {
                    talk_group = 0x41;
                }
                break;
            case 3:
                if (game_state.year == 0x60) {
                    talk_group = 0x3F;
                } else {
                    talk_group = 0x42;
                }
                break;
            case 4:
                talk_group = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        temp_v0_2 = girl_state_rank(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            portrait_layout_set(0U);
            func_80063668();
            talk_group = 7;
            break;
        case 2:
            talk_group = 3;
            break;
        case 3:
            talk_group = 4;
            break;
        case 4:
            mode_step_set(0xF);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if ((girl_state_rank(9) == 4) || (girl_state_rank(9) < 2)) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        talk_group = 0;
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

void func_8015AF20(void) {
    talk_names_set(9);
    func_80132340();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}
