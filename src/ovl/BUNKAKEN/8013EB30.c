#define MAIN_API_OVERRIDE_portrait_layout_set /* T-12701: K&R call; callee narrows the argument. */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void portrait_layout_set();

void func_8013EB30(void) {
    switch (D_80122EB8) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x71);
            return;
        }
        mode_stage_set(0x70);
        return;
    case 1:
        mode_stage_set(0x72);
        return;
    case 2:
        mode_stage_set(0x73);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x74);
            return;
        case 0x60:
            mode_stage_set(0x75);
            return;
        case 0x61:
            mode_stage_set(0x73);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x76);
            return;
        case 0x60:
            mode_stage_set(0x77);
            return;
        case 0x61:
            mode_stage_set(0x78);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x79);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013EC98(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
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
            func_80140EF4();
            if (g->year == 0x60) {
                talk_group += 3;
            }
        }
        break;
    case 5:
        normal_date_speak();
        if (girl_state_rank(8) < 3) {
            if (talk_line >= 3) {
                portrait_layout_set(5U);
            }
        }
        break;
    case 6:
        if (girl_state_rank(8) == 3) {
            mode_step_set(9);
        } else {
            talk_group = 1;
            normal_date_bggirl_fadeout();
        }
        break;
    case 7:
        normal_date_speak();
        portrait_layout_set(3U);
        break;
    case 8:
        normal_date_bggirl_fadein();
        break;
    case 9:
        temp_v0_2 = girl_state_rank(8);
        switch (temp_v0_2) {                        /* switch 3; irregular */
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
        mode_step_next();
        break;
    case 10:
        normal_date_speak();
        if (talk_line >= 4) {
            portrait_layout_set(get_g_zyotai_h(GIRL_YUKO) & 0x7F);
        }
        break;
    case 11:
        normal_date_girl_out();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013EFA4(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
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
            func_80140EF4();
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
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5012);
        break;
    case 9:
        addr_init_bustup();
        g->current_girl = GIRL_YUKO;
        portrait_show_checked();
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(8);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0xE;
                break;
            case 2:
                talk_group = 0x10;
                break;
            case 3:
                talk_group = 0x12;
                break;
            }
            func_80140EF4();
        }
        break;
    case 12:
        normal_date_speak();
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

void func_8013F29C(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
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
            func_80140EF4();
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
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x467C, 1);
        mode_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            portrait_read_step(0x5012);
        }
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            addr_init_bustup();
            g->current_girl = GIRL_YUKO;
            portrait_show_checked();
            func_80145844();
            D_80125CA0 = 0xC0;
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(8);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
            talk_group = (talk_group + (g->year * 3)) - 0x11D;
            func_80140EF4();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        temp_v0_3 = girl_state_rank(8);
        switch (temp_v0_3) {                        /* switch 4; irregular */
        case 0:
        case 1:
            talk_group = 0xF;
            break;
        case 2:
            talk_group = 0x11;
            break;
        case 3:
            if (g->year == 0x60) {
                talk_group = 0x13;
            } else {
                talk_group = 0x20;
            }
            break;
        }
        mode_step_next();
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
        talk_group = 0;
        mode_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013F6AC(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->mode_step) {
    case 0:
        if (D_80122EB8 == 3) {
            dec_bg_cd_read(0x4105, 1);
        } else {
            dec_bg_cd_read(0x40FC, 1);
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            func_80140EF4();
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
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5012);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUKO;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(8);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x24;
                break;
            case 2:
                talk_group = 0x25;
                break;
            case 3:
                talk_group = 0x26;
                break;
            }
            talk_group = (talk_group + (g->year * 3)) - 0x11D;
            func_80140EF4();
        }
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

void func_8013F9E8(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x2D;
                break;
            case 2:
                talk_group = 0x2E;
                break;
            case 3:
                talk_group = 0x2F;
                break;
            }
            func_80140EF4();
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_set(0xB);
        break;
    case 8:
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        mode_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(8);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x30;
                break;
            case 2:
                talk_group = 0x33;
                break;
            case 3:
                talk_group = 0x36;
                break;
            }
            talk_group += D_80122EA8;
            func_80140EF4();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(8);
        switch (temp_v0_3) {                        /* switch 4; irregular */
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013FD40(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

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
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x3C;
                break;
            case 2:
                talk_group = 0x3E;
                break;
            case 3:
                talk_group = 0x40;
                break;
            }
            func_80140EF4();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_bggirl_fadeout();
        break;
    case 8:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 4;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(MODE_TT_GAME);
        break;
    case 9:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 10:
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
    case 11:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 13:
        portrait_read_step(0x5012);
        break;
    case 14:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 15:
        normal_date_bggirl_fadein();
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(8);
            switch (temp_v0_2) {
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
            func_80140EF4();
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0x8D);
        break;
    case 20:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 21:
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
    case 22:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 24:
        portrait_read_step(0x5012);
        break;
    case 25:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKO;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 26:
        normal_date_bggirl_fadein();
        break;
    case 27:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(8);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0x45;
                break;
            case 2:
                talk_group = 0x47;
                break;
            case 3:
                talk_group = 0x49;
                break;
            }
            if ((u8) D_800E7D4E < 4U) {
                talk_group += 1;
            }
            func_80140EF4();
        }
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        temp_v0_4 = girl_state_rank(8);
        switch (temp_v0_4) {
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
        mode_step_next();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        normal_date_girl_out();
        break;
    case 32:
        normal_date_bg_fadeout();
        break;
    case 33:
        talk_group = 0;
        mode_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801404B0(void) {
    GameState *g = &game_state;
    s32 temp_v0;

    switch (g->mode_step) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x57;
                break;
            case 2:
                talk_group = 0x5A;
                break;
            case 3:
                talk_group = 0x5D;
                break;
            }
            func_80140EF4();
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
        normal_date_girl_out();
        break;
    case 11:
        normal_date_bg_fadeout();
        break;
    case 12:
        talk_group = 0;
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801406AC(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            func_80140EF4();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80140878(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->mode_step) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x66;
                break;
            case 2:
                talk_group = 0x67;
                break;
            case 3:
                talk_group = 0x68;
                break;
            }
            func_80140EF4();
        }
        break;
    case 5:
        normal_date_speak();
        if (girl_state_rank(8) < 3) {
            if (talk_line >= 3) {
                portrait_layout_set(5U);
            }
        }
        break;
    case 6:
        if (girl_state_rank(8) == 3) {
            mode_step_set(9);
        } else {
            talk_group = 1;
            normal_date_bggirl_fadeout();
        }
        break;
    case 7:
        normal_date_speak();
        portrait_layout_set(3U);
        break;
    case 8:
        normal_date_bggirl_fadein();
        break;
    case 9:
        temp_v0_2 = girl_state_rank(8);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 0x69;
            break;
        case 3:
            talk_group = 0xA;
            break;
        }
        mode_step_next();
        break;
    case 10:
        normal_date_speak();
        if (talk_line >= 4) {
            portrait_layout_set(get_g_zyotai_h(GIRL_YUKO) & 0x7F);
        }
        break;
    case 11:
        normal_date_girl_out();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80140B64(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(8);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                talk_group = 0x6A;
                break;
            case 2:
                talk_group = 0x6B;
                break;
            case 3:
                talk_group = 0x6C;
                break;
            }
            func_80140EF4();
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
        game_mode_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5012);
        break;
    case 9:
        addr_init_bustup();
        g->current_girl = GIRL_YUKO;
        portrait_show_checked();
        func_80145844();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x10U, 0x801E8000, 0x49A7);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(8);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x6D;
                break;
            case 2:
                talk_group = 0x6E;
                break;
            case 3:
                talk_group = 0x6F;
                break;
            }
            talk_group = (talk_group + (g->year * 3)) - 0x11D;
            func_80140EF4();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_3 = girl_state_rank(8);
        switch (temp_v0_3) {                        /* switch 4; irregular */
        case 0:
        case 1:
            talk_group = 0xF;
            break;
        case 2:
            talk_group = 0x11;
            break;
        case 3:
            talk_group = 0x20;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80140EF4(void) {
    talk_names_set(8);
    func_801323DC();
    talk_line = 0;
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
    mode_step_next();
}
