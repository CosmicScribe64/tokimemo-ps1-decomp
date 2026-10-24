#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

void func_8013CF30(void) {
    switch (D_80122EB8) {
    case 0:
        if (game_state.year == 0x60) {
            mode_stage_set(0x84);
            return;
        }
        mode_stage_set(0x85);
        return;
    case 1:
        mode_stage_set(0x86);
        return;
    case 2:
        mode_stage_set(0x87);
        return;
    case 3:
        if (game_state.year == 0x60) {
            mode_stage_set(0x88);
            return;
        }
        mode_stage_set(0x87);
        return;
    case 4:
        if (game_state.year == 0x60) {
            mode_stage_set(0x89);
            return;
        }
        mode_stage_set(0x8A);
        return;
    case 5:
        mode_stage_set(0x8B);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013D048(void) {
    GameState *g = &game_state;
    s32 temp_v0;

    switch (g->mode_step) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_8013EAC8();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        talk_group = 0;
        mode_step_next();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013D1F4(void) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(10);
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
            }
            func_8013EAC8();
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
        portrait_read_step(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(10);
            switch (temp_v0_2) {
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
            func_8013EAC8();
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

void func_8013D4D8(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->mode_step) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_8013EAC8();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
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
            portrait_read_step(0x6935);
        }
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            addr_init_bustup();
            g->current_girl = GIRL_YUMI;
            portrait_show_checked();
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            if (g->year == 0x61) {
                talk_group += 3;
            }
            func_8013EAC8();
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

void func_8013D82C(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->mode_step) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            if (D_80122EB8 == 3) {
                talk_group += 3;
            }
            func_8013EAC8();
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            if (D_80122EB8 == 2) {
                temp_v0_2 = girl_state_rank(0xA);
                switch (temp_v0_2) {                /* switch 3; irregular */
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
                if (g->year == 0x61) {
                    talk_group += 3;
                }
            } else {
                temp_v0_3 = girl_state_rank(0xA);
                switch (temp_v0_3) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x20;
                    break;
                case 2:
                    talk_group = 0x21;
                    break;
                case 3:
                    talk_group = 0x22;
                    break;
                }
            }
            func_8013EAC8();
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

void func_8013DBE8(void) {
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
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_8013EAC8();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 3;
        g->saved_game_mode = g->game_mode;
        g->saved_mode_stage = g->mode_stage;
        g->saved_mode_step = g->mode_step + 1;
        game_mode_set(GAME_MODE_TT_GAME);
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
        hizuke_init();
        text_marker_hide();
        talk_names_set(0xAU);
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
        portrait_read_step(0x6935);
        break;
    case 13:
        addr_init_bustup();
        g->current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            func_8013EAC8();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        g->saved_game_mode = g->game_mode;
        g->saved_mode_stage = g->mode_stage;
        g->saved_mode_step = g->mode_step + 1;
        mode_stage_set(0x8D);
        break;
    case 19:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 20:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(0xAU);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 21:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 22:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 23:
        portrait_read_step(0x6935);
        break;
    case 24:
        addr_init_bustup();
        g->current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 25:
        normal_date_bggirl_fadein();
        break;
    case 26:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(0xA);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 0x29;
                break;
            case 2:
                talk_group = 0x2B;
                break;
            case 3:
                talk_group = 0x2D;
                break;
            }
            if ((u8) D_800E7D4E < 3U) {
                talk_group += 1;
            }
            func_8013EAC8();
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_bggirl_fadeout();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013E2C0(void) {
    GameState *g = &game_state;
    s32 temp_v0;

    switch (g->mode_step) {
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
            cd_read_sectors(0x2DU, 0x80197000, 0x6C5F);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x35;
                break;
            case 2:
                talk_group = 0x37;
                break;
            case 3:
                talk_group = 0x39;
                break;
            }
            func_8013EAC8();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        if (girl_state_rank(0xA) == 3) {
            mode_step_set(0xE);
        } else {
            mode_step_next();
        }
        break;
    case 10:
        normal_date_bg_fadeout();
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
    case 14:
        memcpy((void *)0x80180000, 0x80197000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_YOSHIO;
        talk_names_set(0xDU);
        portrait_show_checked();
        break;
    case 15:
        normal_date_girl_in();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        mode_step_set(0xA);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013E578(void) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(10);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x3C;
                break;
            case 2:
                talk_group = 0x3D;
                break;
            case 3:
                talk_group = 0x3E;
                break;
            }
            func_8013EAC8();
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
        temp_v0_2 = girl_state_rank(10);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x3F;
            break;
        case 2:
            talk_group = 0x40;
            break;
        case 3:
            talk_group = 0x41;
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

void func_8013E7C4(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->mode_step) {
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
            cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x42;
                break;
            case 2:
                talk_group = 0x43;
                break;
            case 3:
                talk_group = 0x44;
                break;
            }
            func_8013EAC8();
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x6935);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUMI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4B72);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            if (g->year == 0x61) {
                talk_group += 3;
            }
            func_8013EAC8();
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

void func_8013EAC8(void) {
    talk_names_set(10);
    func_801324AC();
    talk_line = 0;
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
    mode_step_next();
}
