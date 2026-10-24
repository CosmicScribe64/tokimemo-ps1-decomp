#include "common.h"
#include "ovl/BUNKASAI.h"

void func_801511E0(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0;
    game_state.girls[0].flags.f.b2 = 1;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            main_stage_set(3);
            return;
        }
        main_stage_set(2);
        return;
    case 1:
        main_stage_set(4);
        return;
    case 2:
        main_stage_set(5);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(6);
            return;
        case 0x60:
            main_stage_set(8);
            return;
        case 0x61:
            main_stage_set(9);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0xA);
            return;
        case 0x60:
            main_stage_set(0xB);
            return;
        case 0x61:
            main_stage_set(0xC);
            return;
        }
        break;
    case 5:
        main_stage_set(0xD);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80151364(void) {
    GameState *game = &game_state;
    u32 temp_v0;

    switch (game->step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x544A);
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 2;
                break;
            case 2:
                dialogue_group = 4;
                break;
            case 3:
                dialogue_group = 6;
                break;
            case 4:
                dialogue_group = 8;
                break;
            }
            if (game->year == 0x60) {
                dialogue_group += 0xC;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bggirl_fadeout();
        dialogue_group = 0;
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (girl_state_rank(0) == 4) {
            if ((u16) game->clubs[0].exp >= (u32) ((game->year * 0x64) - 0x24B8)) {
                dialogue_group = 0xC;
            } else {
                dialogue_group = 0xD;
            }
        } else {
            if ((u16) game->clubs[0].exp >= (u32) ((game->year * 0x64) - 0x24B8)) {
                dialogue_group = 0xA;
            } else {
                dialogue_group = 0xB;
            }
        }

        /* FAKE: pointer reassignment splits the state/base live ranges. T-10040, T-24030. */
        if ((game = &game_state)->year == 0x60) {
            dialogue_group = dialogue_group + 0xC;
        }
        main_step_next();
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
        dialogue_group = 1;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801516B0(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x544A);
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x1A;
                break;
            case 2:
                dialogue_group = 0x1C;
                break;
            case 3:
                dialogue_group = 0x1E;
                break;
            case 4:
                dialogue_group = 0x20;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 2;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 9:
        bustup_read_step(0x544A);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x1B;
                break;
            case 2:
                dialogue_group = 0x1D;
                break;
            case 3:
                dialogue_group = 0x1F;
                break;
            case 4:
                dialogue_group = 0x21;
                break;
            }
            func_80153FA4();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bggirl_fadeout();
        break;
    case 16:
        dialogue_group = 1;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80151A10(void) {
    u8 year_copy;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_start(0x5477);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (game_state.year == 0x5F) {
                temp_v0 = girl_state_rank(0);
                switch (temp_v0) {
                case 0:
                case 1:
                    dialogue_group = 0x22;
                    break;
                case 2:
                    dialogue_group = 0x23;
                    break;
                case 3:
                    dialogue_group = 0x24;
                    break;
                case 4:
                    dialogue_group = 0x25;
                    break;
                }
            } else {
                temp_v0_2 = girl_state_rank(0);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    dialogue_group = 0x26;

                    break;
                case 2:
                case 3:
                    dialogue_group = 0x27;
                    break;
                case 4:
                    dialogue_group = 0x28;
                    break;
                }
                dialogue_group = (dialogue_group + (game_state.year * 3)) - 0x120;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if ((game_state.year == 0x60) && (girl_state_rank(0) == 4)) {
            dialogue_group = 0xB1;
            normal_date_girl_out();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        if ((game_state.year == 0x60) && (girl_state_rank(0) == 4)) {
            normal_date_speak();
        } else {
            main_step_set(0xA);
        }
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x5477);
        }
        break;
    case 13:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            if (girl_state_rank(0) == 4) {
                if ((u16) game_state.clubs[1].exp >= (u32) (((year_copy = game_state.year) * 0x64) - 0x24B8)) {
                    dialogue_group = 0x2E;
                } else {
                    dialogue_group = 0x2F;
                }
            } else {
                if ((u16) game_state.clubs[1].exp >= (u32) (((year_copy = game_state.year) * 0x64) - 0x24B8)) {
                    dialogue_group = 0x2C;
                } else {
                    dialogue_group = 0x2D;
                }
            }

            if ((year_copy = game_state.year) == 0x61) {
                dialogue_group = dialogue_group + 4;
            }
            func_80153FA4();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        dialogue_group = 1;
        main_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80151F10(void) {
    u8 year;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_start(0x54D1);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x34;

                break;
            case 2:
                dialogue_group = 0x35;
                break;
            case 3:
                dialogue_group = 0x36;
                break;
            case 4:
                dialogue_group = 0x37;
                break;
            }

            if ((year = game_state.year) == 0x60) {
                dialogue_group += 0xC;
            }

            if ((year = game_state.year) == 0x61) {
                dialogue_group = dialogue_group + 0x14;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 2;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 9:
        bustup_read_step(0x54D1);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            if ((year = game_state.year) == 0x5F) {
                temp_v0_2 = girl_state_rank(0);
                switch (temp_v0_2) {
                case 0:
                    dialogue_group = 0x39;

                    break;
                case 1:
                    dialogue_group = 0x38;
                    break;
                case 2:
                    dialogue_group = 0x3A;
                    break;
                case 3:
                    dialogue_group = 0x3C;
                    break;
                case 4:
                    dialogue_group = 0x3E;
                    break;
                }
                if (((u16) game_state.clubs[2].exp < 0x64U) && (girl_state_rank(0) >= 2)) {
                    dialogue_group += 1;
                }
            } else {
                temp_v0_3 = girl_state_rank(0);
                switch (temp_v0_3) {
                case 0:
                case 1:
                    dialogue_group = 0x44;

                    break;
                case 2:
                    dialogue_group = 0x45;
                    break;
                case 3:
                    dialogue_group = 0x46;
                    break;
                case 4:
                    dialogue_group = 0x47;
                    break;
                }
                if ((year = game_state.year) == 0x61) {
                    dialogue_group += 8;
                }
            }
            func_80153FA4();
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
        dialogue_group = 1;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801523A0(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x544A);
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x50;
                break;
            case 2:
                dialogue_group = 0x56;
                break;
            case 3:
                dialogue_group = 0x5C;
                break;
            case 4:
                dialogue_group = 0x62;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (girl_state_rank(0) == 4) {
            normal_date_girl_out();
        } else {
            normal_date_speak();
        }
        break;
    case 8:
        if (girl_state_rank(0) == 4) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 9:
        snd_command(0x202);
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_next();
        break;
    case 10:
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        main_step_next();
        break;
    case 11:
        if (girl_state_rank(0) == 4) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            func_80153FA4();
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x52;
                break;
            case 2:
                dialogue_group = 0x58;
                break;
            case 3:
                dialogue_group = 0x5E;
                break;
            case 4:
                dialogue_group = 0x64;
                main_step_set(0xE);
                break;
            }
        }
        break;
    case 13:
        dialogue_group += 1;
        main_step_next();
        break;
    case 14:
        dialogue_group += D_80122EA8;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(0) == 4) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        dialogue_group = 1;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801527A8(void) {
    /* FAKE: unused frame words preserve the observed 0x40 frame
     * and the real length spill at sp+0x36; original locals unknown. T-10040. */
    s32 frame_pad[2];
    s16 sp36;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        bustup_read_step(0x544A);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        func_801431D4();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 5:
        normal_date_bggirl_fadein();
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x67;
                break;
            case 2:
                dialogue_group = 0x6C;
                break;
            case 3:
                dialogue_group = 0x71;
                break;
            case 4:
                dialogue_group = 0x76;
                break;
            }
            func_80153FA4();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (girl_state_rank(0) == 4) {
            normal_date_girl_out();
        } else {
            main_step_set(0xB);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        main_step_set(0x10);
        break;
    case 11:
        normal_date_two_select();
        break;
    case 12:
        if (D_80122CDC != 0) {
            normal_date_speak();
        } else {
            main_step_set(0x10);
        }
        break;
    case 13:
        normal_date_two_select();
        break;
    case 14:
        if (D_80122CDC != 0) {
            normal_date_speak();
        } else {
            main_step_set(0x10);
        }
        break;
    case 15:
        main_step_set(0x1B);
        break;
    case 16:
        if (girl_state_rank(0) == 4) {
            main_step_next();
        } else {
            switch (game_state.substep) {
            case 0:
                sp36 = strlen(game_state.player_family_name);
                k_reset(1);
                set_kanji_string((s16) (-0x68 - (sp36 * 4)), 0x32, 0, (s32) game_state.player_family_name, 0);
                set_kanji_string(-0x56, 0x32, 0, (s32) "『それじゃ、やってみせるかな。", 0);
                k_disp_start(2);
                game_state.substep += 1;
                D_80160AD0 = 0;
                break;
            case 1:
                D_80160AD0 += 1;
                if ((D_80160AD0 >= 0x10) && (check_end_k() != 0)) {
                    D_8011ED5B ^= 0x80;
                    D_80160AD0 = 0;
                }
                if ((game_state.pad_pressed & 0xF0) && (check_end_k() != 0)) {
                    k_reset(1);
                    D_8011ED5B &= 0x7F;
                    game_state.substep += 1;
                }
                break;
            case 2:
                normal_date_girl_out();
                break;
            }
        }
        break;
    case 17:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_mode = game_state.mode;
        game_state.saved_stage = game_state.stage;
        game_state.saved_step = game_state.step + 1;
        main_stage_set(0xA4);
        break;
    case 18:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0xA3);
        break;
    case 19:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(0U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 20:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 22:
        bustup_read_step(0x544A);
        break;
    case 23:
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 24:
        if (girl_state_rank(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 25:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x6A;
                break;
            case 2:
                dialogue_group = 0x6F;
                break;
            case 3:
                dialogue_group = 0x74;
                break;
            case 4:
                dialogue_group = 0xB2;
                break;
            }
            if (((u8) D_800E7D4E < 2U) && (girl_state_rank(0) < 4)) {
                dialogue_group += 1;
            }
            func_80153FA4();
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if (girl_state_rank(0) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 28:
        dialogue_group = 1;
        main_step_next();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80152F08(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x544A);
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x78;
                break;
            case 2:
                dialogue_group = 0x79;
                break;
            case 3:
                dialogue_group = 0x7A;
                break;
            case 4:
                dialogue_group = 0x7B;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        D_80122EAC = 2;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 9:
        bustup_read_step(0x544A);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x7C;
                break;
            case 2:
                dialogue_group = 0x7D;
                break;
            case 3:
                dialogue_group = 0x7E;
                break;
            case 4:
                dialogue_group = 0x7F;
                break;
            }
            func_80153FA4();
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
        dialogue_group = 1;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80153260(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x54A4);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_801431D4();
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x80;
                break;
            case 2:
                dialogue_group = 0x81;
                break;
            case 3:
                dialogue_group = 0x82;
                break;
            case 4:
                dialogue_group = 0x83;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        dialogue_group = 0;
        normal_date_bggirl_fadeout();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bggirl_fadein();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x84;
            break;
        case 2:
            dialogue_group = 0x86;
            break;
        case 3:
            dialogue_group = 0x86;
            break;
        case 4:
            dialogue_group = 0x88;
            break;
        }
        if ((u16) game_state.clubs[4].exp < 0x64U) {
            dialogue_group += 1;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        dialogue_group = 1;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80153530(void) {
    u32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x54A4);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_801431D4();
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x8A;
                break;
            case 2:
                dialogue_group = 0x8C;
                break;
            case 3:
                dialogue_group = 0x8D;
                break;
            case 4:
                dialogue_group = 0x8E;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        if ((girl_state_rank(0) == 2) || (girl_state_rank(0) == 3)) {
            main_step_set(0xA);
        } else {
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((girl_state_rank(0) == 2) || (girl_state_rank(0) == 3)) {
            normal_date_girl_out();
        } else {
            main_step_next();
        }
        break;
    case 11:
        normal_date_bg_fadeout();
        break;
    case 12:
        dialogue_group = 1;
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801537BC(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_start(0x54A4);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        }
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x90;
                break;
            case 2:
                dialogue_group = 0x91;
                break;
            case 3:
                dialogue_group = 0x92;
                break;
            case 4:
                dialogue_group = 0x93;
                break;
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        dialogue_group = 0;
        main_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
        case 2:
        case 3:
            dialogue_group = 0x94;
            break;
        case 4:
            dialogue_group = 0x97;
            break;
        }
        if ((u16) game_state.clubs[4].exp < 0x12CU) {
            dialogue_group += 1;
        }
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        dialogue_group = 1;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80153A98(void) {
    u8 year;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x544A);
        }
        break;
    case 3:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 4:
        normal_date_bggirl_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            if (game_state.year == 0x5F) {
                temp_v0 = girl_state_rank(0);
                switch (temp_v0) {
                case 0:
                case 1:
                    dialogue_group = 0x99;
                    break;
                case 2:
                    dialogue_group = 0x9A;
                    break;
                case 3:
                    dialogue_group = 0x9B;
                    break;
                case 4:
                    dialogue_group = 0x9C;
                    break;
                }
            } else {
                temp_v0_2 = girl_state_rank(0);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    dialogue_group = 0x9E;
                    break;
                case 2:
                    dialogue_group = 0x9F;
                    break;
                case 3:
                    dialogue_group = 0x9F;
                    break;
                case 4:
                    dialogue_group = 0xA0;
                    break;
                }
                if (game_state.year == 0x61) {
                    dialogue_group += 4;
                }
            }
            func_80153FA4();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        if (girl_state_rank(0) == 4) {
            normal_date_girl_out();
        } else {
            main_step_set(9);
        }
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        if (girl_state_rank(0) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 10:
        D_80122EAC = 3;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 11:
        bustup_read_step(0x544A);
        break;
    case 12:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x18U, 0x801E8000, 0x49D0);
        break;
    case 13:
        normal_date_bggirl_fadein();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
            case 2:
            case 3:
                dialogue_group = 0xA6;
                break;
            case 4:
                dialogue_group = 0xA8;
                break;
            }
            if ((year = game_state.year) == 0x61) {
                dialogue_group += 6;
            }
            if ((u16) game_state.clubs[5].exp < (u32) (((year = game_state.year) * 0x64) - 0x24B8)) {
                if (girl_state_rank(0) == 4) {
                    dialogue_group += 2;
                } else {
                    dialogue_group += 1;
                }
            }
            func_80153FA4();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(0) == 4) {
            normal_date_girl_out();
        } else {
            main_step_set(0x12);
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if (girl_state_rank(0) == 4) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        dialogue_group = 1;
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80153FA4(void) {
    dialogue_names_set(0);
    func_80132034();
    dialogue_line = 0;
    dialogue_text_table = D_801604F0;
    dialogue_command_table = D_801604F4;
    dialogue_voice_table = D_801604F8;
    main_step_next();
}
