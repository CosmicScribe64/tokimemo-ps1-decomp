/* nozomi_block.c: src/ovl/BUNKAKEN/80146560.c:4: Nozomi block (stages 85-97). Original object 0x80146560-0x80148ED0 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80146560(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x56);
            return;
        case 0x60:
            mode_stage_set(0x57);
            return;
        case 0x61:
            mode_stage_set(0x58);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    case 1:
        mode_stage_set(0x59);
        return;
    case 2:
        mode_stage_set(0x5A);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x5B);
            return;
        case 0x60:
            mode_stage_set(0x5C);
            return;
        case 0x61:
            mode_stage_set(0x5D);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x5E);
            return;
        case 0x60:
            mode_stage_set(0x5F);
            return;
        case 0x61:
            mode_stage_set(0x60);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x61);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80146700(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
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
            func_80148E68();
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
        temp_v0_2 = girl_state_rank(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 5;
            break;
        case 2:
            talk_group = 9;
            break;
        case 3:
            talk_group = 0xA;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(6) < 2) {
            if (game_state.param_liberal_arts.value < 0x78) {
                talk_group += 1;
            }
            mode_step_next();
        } else {
            mode_step_set(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        talk_group = 8;
        mode_step_next();
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

void func_801469DC(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
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
            func_80148E68();
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
        temp_v0_2 = girl_state_rank(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0xE;
            break;
        case 2:
            talk_group = 0x10;
            break;
        case 3:
            talk_group = 0x11;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(6) >= 2) {
            mode_step_set(0xE);
        } else {
            mode_step_next();
        }
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        normal_date_bggirl_fadein();
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

void func_80146C8C(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x12;
                break;
            case 2:
                talk_group = 0x13;
                break;
            case 3:
                talk_group = 0x14;
                break;
            }
            func_80148E68();
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
        portrait_read_step(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x15;
                break;
            case 2:
                talk_group = 0x16;
                break;
            case 3:
                talk_group = 0x17;
                break;
            }
            func_80148E68();
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

void func_80146F70(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
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
            }
            func_80148E68();
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
            portrait_read_step(0x622D);
        }
        break;
    case 10:
        func_80145844();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
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
            }
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            func_80148E68();
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

void func_801472C4(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
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
            func_80148E68();
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
        portrait_read_step(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
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
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            func_80148E68();
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

void func_801475CC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
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
            func_80148E68();
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
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xB);
        break;
    case 8:
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        mode_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x33;
                break;
            case 2:
                talk_group = 0x36;
                break;
            case 3:
                talk_group = 0x39;
                break;
            }
            talk_group += D_80122EA8;
            func_80148E68();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(6);
        switch (temp_v0_3) {
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
        mode_step_next();
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

void func_80147914(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
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
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        talk_group += 2;
        mode_step_next();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 2;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_TT_GAME);
        break;
    case 10:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 11:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(6U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 12:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 14:
        portrait_read_step(0x622D);
        break;
    case 15:
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 16:
        normal_date_bggirl_fadein();
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x48;
                break;
            case 2:
                talk_group = 0x49;
                break;
            case 3:
                talk_group = 0x4A;
                break;
            }
            func_80148E68();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0x8D);
        break;
    case 21:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0x8C);
        break;
    case 22:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(6U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 23:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 25:
        portrait_read_step(0x622D);
        break;
    case 26:
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 27:
        normal_date_bggirl_fadein();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0x4B;
                break;
            case 2:
                talk_group = 0x4D;
                break;
            case 3:
                talk_group = 0x4F;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
            func_80148E68();
        }
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        temp_v0_4 = girl_state_rank(6);
        switch (temp_v0_4) {
        case 0:
        case 1:
            talk_group = 0x51;
            break;
        case 2:
            talk_group = 0x52;
            break;
        case 3:
            talk_group = 0x53;
            break;
        }
        mode_step_next();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        normal_date_bggirl_fadeout();
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

void func_80148094(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x54;
                break;
            case 2:
                talk_group = 0x55;
                break;
            case 3:
                talk_group = 0x56;
                break;
            }
            func_80148E68();
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
        portrait_read_step(0x622D);
        break;
    case 9:
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        portrait_layout_set(3U);
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bg_fadein();
        break;
    case 11:
        talk_group = 0x47;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_in();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x57;
                break;
            case 2:
                talk_group = 0x58;
                break;
            case 3:
                talk_group = 0x59;
                break;
            }
            func_80148E68();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bggirl_fadeout();
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

void func_801483A8(void) {
    s32 temp_v0;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x5A;
                break;
            case 2:
                talk_group = 0x5D;
                break;
            case 3:
                talk_group = 0x60;
                break;
            }
            func_80148E68();
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

void func_80148594(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x63;
                break;
            case 2:
                talk_group = 0x67;
                break;
            case 3:
                talk_group = 0x69;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        if (girl_state_rank(6) >= 2) {
            mode_step_set(0xB);
        } else {
            normal_date_speak();
        }
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
        game_state.current_girl = GIRL_SOTOI;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_801487DC(void) {
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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x6B;
                break;
            case 2:
                talk_group = 0x6C;
                break;
            case 3:
                talk_group = 0x6D;
                break;
            }
            func_80148E68();
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
        temp_v0_2 = girl_state_rank(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x6E;
            break;
        case 2:
            talk_group = 0x6F;
            break;
        case 3:
            talk_group = 0x70;
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

void func_80148A28(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

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
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(6);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x71;
                break;
            case 2:
                talk_group = 0x72;
                break;
            case 3:
                talk_group = 0x73;
                break;
            }
            func_80148E68();
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
        portrait_read_step(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_NOZOMI;
        portrait_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x74;
                break;
            case 2:
                talk_group = 0x75;
                break;
            case 3:
                talk_group = 0x76;
                break;
            }
            talk_group = (talk_group + (game_state.year * 3)) - 0x11D;
            func_80148E68();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (game_state.year == 0x61) {
            temp_v0_3 = girl_state_rank(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0x7D;
                break;
            case 2:
                talk_group = 0x7F;
                break;
            case 3:
                talk_group = 0x81;
                break;
            }
            if (game_state.param_art.value < 0x78) {
                talk_group += 1;
            }
            mode_step_next();
        } else {
            mode_step_set(0x11);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_4 = girl_state_rank(6);
        switch (temp_v0_4) {
        case 0:
        case 1:
            talk_group = 0x83;
            break;
        case 2:
            talk_group = 0x84;
            break;
        case 3:
            talk_group = 0x85;
            break;
        }
        mode_step_next();
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
        func_80145530();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80148E68(void) {
    talk_names_set(6);
    func_8013230C();
    talk_line = 0;
    talk_text_table = D_80155E90;
    talk_command_table = D_80155E94;
    talk_voice_table = D_80155E98;
    mode_step_next();
}
