#define MAIN_API_OVERRIDE_portrait_layout_set /* K&R call; callee narrows the argument. T-10830. */
#include "common.h"
#include "ovl/BUNKASAI.h"

void portrait_layout_set();

void func_80143770(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&D_80122EB8;
    dispatch->unk_04 = 7;
    switch (dispatch->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x7B);
            return;
        }
        mode_stage_set(0x7A);
        return;
    case 1:
        mode_stage_set(0x7C);
        return;
    case 2:
        mode_stage_set(0x7D);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x7E);
            return;
        case 0x60:
            mode_stage_set(0x80);
            return;
        case 0x61:
            mode_stage_set(0x7D);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x81);
            return;
        case 0x60:
            mode_stage_set(0x82);
            return;
        case 0x61:
            mode_stage_set(0x83);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x84);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_801438E4(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            portrait_read_start(0x5B25);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MIRA;
            portrait_show_checked();
            if (game_state.year == 0x5F) {
                talk_group = 0x24;
            } else {
                talk_group = 0x25;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
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
            if (game_state.year == 0x60) {
                talk_group += 4;
            }
            func_80145FC0();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(7);
        switch (temp_v0_2) {
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
            mode_step_next();
            break;
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
        normal_date_bg_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(7) != 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[7].flags.f.b12 = 1;
            } else {
                game_state.girls[7].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80143C38(void) {
    s32 temp_v0_2;
    u32 temp_v0;

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
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5B25);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0xD;
                break;
            case 2:
                talk_group = 0xE;
                break;
            case 3:
                talk_group = 0xF;
                break;
            case 4:
                talk_group = 0x10;
                break;
            }
            func_80145FC0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(7) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        temp_v0_2 = girl_state_rank(7);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 4;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(7) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
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

void func_80143F90(void) {
    s32 temp_v0_2;
    u32 temp_v0;

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
        if (game_state.year == 0x5F) {
            talk_group = 0x27;
        } else {
            talk_group = 0x28;
        }
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
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        mode_step_next();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x5B25);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = (game_state.year * 3) - 0x10B;
                break;
            case 2:
                talk_group = (game_state.year * 3) - 0x10A;
                break;
            case 3:
                talk_group = (game_state.year * 3) - 0x109;
                break;
            case 4:
                talk_group = 0x10;
                break;
            }
            func_80145FC0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(7) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        temp_v0_2 = girl_state_rank(7);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 2:
            talk_group = 2;
            break;
        case 3:
            talk_group = 4;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(7) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
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

void func_80144374(void) {
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_2;

    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        if (((u16) g->player_flags.h >> 0xC) == 2) {
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
            portrait_read_start(0x5B25);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_MIRA;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x1B;
                break;
            case 2:
                talk_group = 0x1C;
                break;
            case 3:
                talk_group = 0x1D;
                break;
            case 4:
                talk_group = 0x1E;
                break;
            }
            func_80145FC0();
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 12:
        portrait_read_step(0x5B25);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        break;
    case 14:
        if (girl_state_rank(7) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(7);
            switch (temp_v0_2) {
            case 0:
            case 1:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x28;
                } else {
                    talk_group = (g->year * 3) - 0xFE;
                }
                break;
            case 2:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x29;
                } else {
                    talk_group = (g->year * 3) - 0xFD;
                }
                break;
            case 3:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x2A;
                } else {
                    talk_group = (g->year * 3) - 0xFC;
                }
                break;
            case 4:
                talk_group = 0x2B;
            }
            func_80145FC0();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        temp_v0_3 = girl_state_rank(7);
        switch (temp_v0_3) {                        /* switch 4; irregular */
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
        if ((g->year == 0x5F) && (girl_state_rank(7) < 3)) {
            mode_step_set(0x13);
        } else if (girl_state_rank(7) == 4) {
            mode_step_set(0x13);
        } else {
            mode_step_next();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (girl_state_rank(7) == 4) {
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
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801448D8(void) {
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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x5B25);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x2A;
            addr_init_bustup();
            game_state.current_girl = GIRL_MIRA;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x2C;
                break;
            case 2:
                talk_group = 0x2D;
                break;
            case 3:
                talk_group = 0x2E;
                break;
            case 4:
                talk_group = 0x2F;
                break;
            }
            func_80145FC0();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(7) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 12:
        audio_command(0x202);
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 13:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(7) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80145FC0();
            temp_v0_2 = girl_state_rank(7);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x31;
                break;
            case 2:
                talk_group = 0x34;
                break;
            case 3:
                talk_group = 0x37;
                break;
            case 4:
                talk_group = 0x3A;
                break;
            }
            talk_group += D_80122EA8;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        mode_step_next();
        temp_v0_3 = girl_state_rank(7);
        switch (temp_v0_3) {
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
            mode_step_set(0x14);
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
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80144D20(void) {
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
        portrait_read_step(0x5B25);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_MIRA;
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
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x3D;
                break;
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
            func_80145FC0();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(7) == 4) {
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
        if (girl_state_rank(7) == 4) {
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
        talk_names_set(7U);
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
        portrait_read_step(0x5B25);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        break;
    case 22:
        if (girl_state_rank(7) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(7);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x42;
                break;
            case 2:
                talk_group = 0x44;
                break;
            case 3:
                talk_group = 0x46;
                break;
            case 4:
                talk_group = 0x48;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
            func_80145FC0();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        mode_step_next();
        temp_v0_3 = girl_state_rank(7);
        switch (temp_v0_3) {
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
            mode_step_set(0x1C);
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
        normal_date_bg_fadeout();
        break;
    case 29:
        talk_group = 0;
        mode_step_next();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80145334(void) {
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
            portrait_read_start(0x5B25);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MIRA;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
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
            case 4:
                talk_group = 0x53;
                break;
            }
            func_80145FC0();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(7) == 4) {
            mode_step_set(0xE);
        } else {
            if (((u16) game_state.clubs[4].practice_points < 0x64U) && ((talk_group += 1, (girl_state_rank(7) == 2)) || (girl_state_rank(7) == 3))) {
                portrait_layout_set(4U);
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        if ((u16) game_state.clubs[4].practice_points < 0x64U) {
            if ((girl_state_rank(7) == 2) || (girl_state_rank(7) == 3)) {
                if (talk_line == 5) {
                    portrait_layout_set(get_g_zyotai_h(GIRL_MIRA));
                }
            }
        }
        break;
    case 12:
        temp_v0_2 = girl_state_rank(7);
        switch (temp_v0_2) {                        /* switch 3; irregular */
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
        mode_step_next();
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
        talk_group = 0;
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

void func_801456E0(void) {
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
            portrait_read_start(0x5B25);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MIRA;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x54;
                break;
            case 2:
                talk_group = 0x56;
                break;
            case 3:
                talk_group = 0x58;
                break;
            case 4:
                talk_group = 0x5A;
                break;
            }
            func_80145FC0();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bg_fadeout();
        break;
    case 13:
        talk_group = 0;
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80145930(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            portrait_read_start(0x5B25);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_MIRA;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
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
            case 4:
                talk_group = 0x5F;
                break;
            }
            func_80145FC0();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(7);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 1;
            mode_step_next();
            break;
        case 2:
            talk_group = 2;
            mode_step_next();
            break;
        case 3:
            talk_group = 3;
            mode_step_next();
            break;
        case 4:
            normal_date_girl_out();
            break;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(7) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        talk_group = 0;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80145C28(void) {
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
        if (game_state.year == 0x5F) {
            talk_group = 0x2E;
        } else {
            talk_group = 0x2F;
        }
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
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5B25);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_MIRA;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A41);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(7);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = (game_state.year * 3) - 0xBC;
                break;
            case 2:
                talk_group = (game_state.year * 3) - 0xBB;
                break;
            case 3:
                talk_group = (game_state.year * 3) - 0xBA;
                break;
            case 4:
                talk_group = 0x6A;
                break;
            }
            func_80145FC0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(7);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 1;
            mode_step_next();
            break;
        case 2:
            talk_group = 2;
            mode_step_next();
            break;
        case 3:
            talk_group = 3;
            mode_step_next();
            break;
        case 4:
            normal_date_girl_out();
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(7) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        talk_group = 0;
        mode_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80145FC0(void) {
    talk_names_set(7);
    func_801322D8();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}
