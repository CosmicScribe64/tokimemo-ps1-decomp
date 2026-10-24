#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80137640(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x3B);
            return;
        case 0x60:
            main_stage_set(0x3C);
            return;
        case 0x61:
            main_stage_set(0x3D);
            return;
        default:
            game_reset_to_title();
            return;
        }
        break;
    case 1:
        main_stage_set(0x3E);
        return;
    case 2:
        main_stage_set(0x3F);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x40);
            return;
        case 0x60:
            main_stage_set(0x41);
            return;
        case 0x61:
            main_stage_set(0x42);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x43);
            return;
        case 0x60:
            main_stage_set(0x44);
            return;
        case 0x61:
            main_stage_set(0x45);
            return;
        }
        break;
    case 5:
        if (game_state.year == 0x61) {
            main_stage_set(0x47);
            return;
        }
        main_stage_set(0x46);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80137804(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 2;
                break;
            case 2:
            case 3:
                dialogue_group = 4;
                break;
            }
            func_80139DEC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 3;
            break;
        case 2:
        case 3:
            dialogue_group = 5;
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
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80137A38(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 6;
                break;
            case 2:
                dialogue_group = 7;
                break;
            case 3:
                dialogue_group = 8;
                break;
            }
            func_80139DEC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 9;
            break;
        case 2:
            dialogue_group = 0xA;
            break;
        case 3:
            dialogue_group = 0xB;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(4) == 3) {
            normal_date_bggirl_fadeout();
        } else {
            main_step_set(0xD);
        }
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        dialogue_group = 0;
        main_step_next();
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
        game_reset_to_title();
        break;
    }
}

void func_80137CD8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0xD;
                break;
            case 2:
            case 3:
                dialogue_group = 0xE;
                break;
            }
            func_80139DEC();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x66BF);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SAKI;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0xF;
                break;
            case 2:
            case 3:
                dialogue_group = 0x10;
                break;
            }
            func_80139DEC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80137FB8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x11;
                break;
            case 2:
                dialogue_group = 0x12;
                break;
            case 3:
                dialogue_group = 0x13;
                break;
            }
            func_80139DEC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x66BF);
        }
        break;
    case 10:
        func_80145844();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        game_state.current_girl = GIRL_SAKI;
        bustup_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            if (game_state.year == 0x5F) {
                temp_v0_2 = girl_state_rank(4);
                switch (temp_v0_2) {
                case 0:
                case 1:
                    dialogue_group = 0x14;
                    break;
                case 2:
                    dialogue_group = 0x15;
                    break;
                case 3:
                    dialogue_group = 0x16;
                    break;
                }
            } else if (girl_state_rank(4) < 2) {
                dialogue_group = (game_state.year * 2) - 0xA9;
            } else {
                dialogue_group = (game_state.year * 2) - 0xA8;
            }
            func_80139DEC();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        dialogue_group = 0;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80138340(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x1B;
                break;
            case 2:
            case 3:
                dialogue_group = 0x1C;
                break;
            }
            func_80139DEC();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x66BF);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_SAKI;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (game_state.year) {
            case 0x5F:
                dialogue_group = 0x1D;
                break;
            case 0x60:
                dialogue_group = 0x21;
                break;
            case 0x61:
                dialogue_group = 0x23;
                break;
            }
            if (girl_state_rank(4) >= 2) {
                dialogue_group += 1;
            }
            func_80139DEC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if ((game_state.year == 0x5F) && (girl_state_rank(4) >= 2)) {
            normal_date_girl_out();
        } else {
            main_step_set(0x11);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_in();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
        break;
    case 18:
        dialogue_group = 0;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801386C0(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x25;
                break;
            case 2:
                dialogue_group = 0x26;
                break;
            case 3:
                dialogue_group = 0x27;
                break;
            }
            func_80139DEC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        snd_command(0x202);
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0xB);
        break;
    case 8:
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        main_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x28;
                break;
            case 2:
                dialogue_group = 0x2B;
                break;
            case 3:
                dialogue_group = 0x2E;
                break;
            }
            dialogue_group += D_80122EA8;
            func_80139DEC();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x31;
            break;
        case 2:
            dialogue_group = 0x32;
            break;
        case 3:
            dialogue_group = 0x33;
            break;
        }
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        dialogue_group = 0;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80138A08(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x34;
                break;
            case 2:
            case 3:
                dialogue_group = 0x35;
                break;
            }
            func_80139DEC();
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
        g->saved_mode = g->mode;
        g->saved_stage = g->stage;
        g->saved_step = g->step + 1;
        main_stage_set(0x8D);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_stage_set(0x8C);
        break;
    case 9:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(4U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 10:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 12:
        bustup_read_step(0x66BF);
        break;
    case 13:
        addr_init_bustup();
        g->current_girl = GIRL_SAKI;
        bustup_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x36;
                break;
            case 2:
            case 3:
                dialogue_group = 0x38;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                dialogue_group += 1;
            }
            func_80139DEC();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        temp_v0_3 = girl_state_rank(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x3A;
            break;
        case 2:
        case 3:
            dialogue_group = 0x3B;
            break;
        }
        main_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_bggirl_fadeout();
        break;
    case 20:
        dialogue_group = 0;
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80138EB8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x3C;
                break;
            case 2:
            case 3:
                dialogue_group = 0x3D;
                break;
            }
            func_80139DEC();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x66BF);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SAKI;
        bustup_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x3E;
                break;
            case 2:
            case 3:
                dialogue_group = 0x3F;
                break;
            }
            func_80139DEC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80139184(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x40;
                break;
            case 2:
            case 3:
                dialogue_group = 0x43;
                break;
            }
            func_80139DEC();
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
        normal_date_bggirl_fadeout();
        break;
    case 11:
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80139364(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x46;
                break;
            case 2:
            case 3:
                dialogue_group = 0x49;
                break;
            }
            func_80139DEC();
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
        game_state.current_girl = GIRL_SOTOI;
        main_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bg_fadeout();
        break;
    case 11:
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8013954C(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x4C;
                break;
            case 2:
            case 3:
                dialogue_group = 0x4E;
                break;
            }
            func_80139DEC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(4);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x4D;
            break;
        case 2:
        case 3:
            dialogue_group = 0x4F;
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
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80139780(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x50;
                break;
            case 2:
            case 3:
                dialogue_group = 0x51;
                break;
            }
            func_80139DEC();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x66BF);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SAKI;
        bustup_show_checked();
        cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x52;
                break;
            case 2:
            case 3:
                dialogue_group = 0x53;
                break;
            }
            if (game_state.year == 0x60) {
                dialogue_group += 2;
            }
            func_80139DEC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80139A6C(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(4);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x50;
                break;
            case 2:
            case 3:
                dialogue_group = 0x51;
                break;
            }
            func_80139DEC();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x66BF);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SAKI;
            bustup_show_checked();
            cd_read_sectors(0xCU, 0x801E8000, 0x4B52);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(4);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x56;
                break;
            case 2:
            case 3:
                dialogue_group = 0x5A;
                break;
            }
            func_80139DEC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (game_state.param_art.value < 0x78) {
            dialogue_group += 1;
        }
        main_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_3 = girl_state_rank(4);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x59;
            break;
        case 2:
        case 3:
            dialogue_group = 0x5D;
            break;
        }
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
        break;
    case 18:
        dialogue_group = 0;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80139DEC(void) {
    dialogue_names_set(4);
    func_8013223C();
    dialogue_line = 0;
    dialogue_text_table = D_80155E90;
    dialogue_command_table = D_80155E94;
    dialogue_voice_table = D_80155E98;
    main_step_next();
}
