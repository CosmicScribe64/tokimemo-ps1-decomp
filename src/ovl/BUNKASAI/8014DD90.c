#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8014DD90(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&D_80122EB8;
    dispatch->unk_04 = 6;
    switch (dispatch->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x6F);
            return;
        }
        mode_stage_set(0x6E);
        return;
    case 1:
        mode_stage_set(0x70);
        return;
    case 2:
        mode_stage_set(0x71);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x72);
            return;
        case 0x60:
            mode_stage_set(0x74);
            return;
        case 0x61:
            mode_stage_set(0x71);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x75);
            return;
        case 0x60:
            mode_stage_set(0x76);
            return;
        case 0x61:
            mode_stage_set(0x77);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x78);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014DF04(void) {
    GameState *g = &game_state;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (g->mode_step) {
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
            portrait_read_start(0x622D);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_NOZOMI;
            portrait_show_checked();
            if (g->year == 0x5F) {
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
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 4;
                break;
            case 2:
                talk_group = 5;
                break;
            case 3:
                talk_group = 6;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            if (g->year == 0x60) {
                talk_group += 4;
            }
            func_801508C4();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        mode_step_next();
        temp_v0_2 = girl_state_rank(6);
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
            mode_step_set(0xC);
            break;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        talk_group = 0x78;
        normal_date_girl_out();
        break;
    case 13:
        if (girl_state_rank(6) == 4) {
            normal_date_speak();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 14:
        talk_group = 0;
        if (girl_state_rank(6) == 4) {
            normal_date_bg_fadeout();
        } else {
            mode_step_next();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(6) != 4) {
            if (g->year == 0x5F) {
                g->girls[6].flags.f.b12 = 1;
            } else {
                g->girls[6].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014E2AC(void) {
    GameState *g = &game_state;
    s32 temp_v0_2;
    u32 temp_v0;

    switch (g->mode_step) {
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x622D);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0xC;
                break;
            case 2:
                talk_group = 0xD;
                break;
            case 3:
                talk_group = 0xF;
                break;
            case 4:
                talk_group = 0x10;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(6) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        temp_v0_2 = girl_state_rank(6);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 1;
            break;
        case 3:
            talk_group = 3;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(6) == 4) {
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

void func_8014E5EC(void) {
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
            portrait_read_step(0x622D);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
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
                talk_group = 0x1B;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(6) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        temp_v0_2 = girl_state_rank(6);
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
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if (girl_state_rank(6) == 4) {
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

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014E9D0(void) {
    GameState *g = &game_state;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            portrait_read_start(0x622D);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_NOZOMI;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                if (g->year == 0x60) {
                    talk_group = 0x21;
                } else {
                    talk_group = 0x1D;
                }
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
            func_801508C4();
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
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 12:
        portrait_read_step(0x622D);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        if (((u16) g->player_flags.h >> 0xC) == 3) {
            portrait_layout_set(3U);
        }
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 14:
        if (girl_state_rank(6) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x2B;
                } else {
                    talk_group = (g->year * 3) - 0xFB;
                }
                break;
            case 2:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x2C;
                } else {
                    talk_group = (g->year * 3) - 0xFA;
                }
                break;
            case 3:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x2D;
                } else {
                    talk_group = (g->year * 3) - 0xF9;
                }
                break;
            case 4:
                talk_group = 0x2E;
                break;
            }
            func_801508C4();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        mode_step_next();
        temp_v0_3 = girl_state_rank(6);
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
            mode_step_set(0x13);
            break;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (girl_state_rank(6) == 4) {
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

void func_8014EF24(void) {
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
            portrait_read_start(0x622D);
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
            game_state.current_girl = GIRL_NOZOMI;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
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
            case 4:
                talk_group = 0x32;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(6) == 4) {
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
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(6) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_801508C4();
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x34;
                break;
            case 2:
                talk_group = 0x37;
                break;
            case 3:
                talk_group = 0x3A;
                break;
            case 4:
                talk_group = 0x3D;
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
        temp_v0_3 = girl_state_rank(6);
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

void func_8014F36C(void) {
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_4;
    u32 temp_v0_5;

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
        portrait_read_step(0x622D);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
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
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x40;
                break;
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
            func_801508C4();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(6) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_set(0x1B);
        break;
    case 14:
        temp_v0_2 = girl_state_rank(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x45;
            break;
        case 2:
            talk_group = 0x46;
            break;
        case 3:
            talk_group = 0x47;
            break;
        }
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
        break;
    case 17:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 2;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(MODE_TT_GAME);
        break;
    case 18:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 19:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(6U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 20:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 22:
        portrait_read_step(0x622D);
        break;
    case 23:
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 24:
        normal_date_bggirl_fadein();
        break;
    case 25:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0x4B;
                break;
            case 2:
                talk_group = 0x4C;
                break;
            case 3:
                talk_group = 0x4D;
                break;
            }
            func_801508C4();
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if (girl_state_rank(6) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 28:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0xA4);
        break;
    case 29:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 30:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(6U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 31:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 32:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 33:
        portrait_read_step(0x622D);
        break;
    case 34:
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 35:
        if (girl_state_rank(6) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 36:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = girl_state_rank(6);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x4E;
                break;
            case 2:
                talk_group = 0x50;
                break;
            case 3:
                talk_group = 0x52;
                break;
            case 4:
                talk_group = 0x54;
                break;
            }
            func_801508C4();
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
        }
        break;
    case 37:
        normal_date_speak();
        break;
    case 38:
        mode_step_next();
        temp_v0_5 = girl_state_rank(6);
        switch (temp_v0_5) {
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
            mode_step_set(0x29);
            break;
        }
        break;
    case 39:
        normal_date_speak();
        break;
    case 40:
        normal_date_girl_out();
        break;
    case 41:
        normal_date_bg_fadeout();
        break;
    case 42:
        talk_group = 0;
        mode_step_next();
        break;
    case 43:
        normal_date_speak();
        break;
    case 44:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* T-12703: existing aggregate and flag views reproduce the scene family. */
void func_8014FC70(void) {
    GameState *g = &game_state;
    s32 temp_v0_2;
    u32 temp_v0;

    switch (g->mode_step) {
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
            portrait_read_start(0x622D);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_NOZOMI;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x56;
                break;
            case 2:
                talk_group = 0x59;
                break;
            case 3:
                talk_group = 0x5C;
                break;
            case 4:
                talk_group = 0x5F;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(6) == 4) {
            mode_step_set(0xE);
        } else {
            if ((u16) g->clubs[4].practice_points < 0x64U) {
                talk_group += 1;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(6);
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
        talk_group = 0x78;
        normal_date_girl_out();
        break;
    case 15:
        if (girl_state_rank(6) == 4) {
            normal_date_speak();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 16:
        talk_group = 0;
        if (girl_state_rank(6) == 4) {
            normal_date_bg_fadeout();
        } else {
            mode_step_next();
        }
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

void func_8014FFE4(void) {
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
            portrait_read_start(0x622D);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_NOZOMI;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x60;
                break;
            case 2:
                talk_group = 0x62;
                break;
            case 3:
                talk_group = 0x64;
                break;
            case 4:
                talk_group = 0x66;
                break;
            }
            func_801508C4();
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

void func_80150234(void) {
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
            portrait_read_start(0x622D);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_NOZOMI;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x68;
                break;
            case 2:
                talk_group = 0x69;
                break;
            case 3:
                talk_group = 0x6A;
                break;
            case 4:
                talk_group = 0x6B;
                break;
            }
            func_801508C4();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(6);
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
        if (girl_state_rank(6) == 4) {
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

void func_8015052C(void) {
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
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x622D);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4B13);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = (game_state.year * 3) - 0xB0;
                break;
            case 2:
                talk_group = (game_state.year * 3) - 0xAF;
                break;
            case 3:
                talk_group = (game_state.year * 3) - 0xAE;
                break;
            case 4:
                talk_group = 0x76;
                break;
            }
            func_801508C4();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(6);
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
        if (girl_state_rank(6) == 4) {
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

void func_801508C4(void) {
    talk_names_set(6);
    func_801322A4();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}
