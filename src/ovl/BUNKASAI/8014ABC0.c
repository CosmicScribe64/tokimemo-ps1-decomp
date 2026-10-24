#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8014ABC0(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&D_80122EB8;
    dispatch->unk_04 = 5;
    switch (dispatch->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x63);
            return;
        }
        mode_stage_set(0x62);
        return;
    case 1:
        mode_stage_set(0x64);
        return;
    case 2:
        mode_stage_set(0x65);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x66);
            return;
        case 0x60:
            mode_stage_set(0x68);
            return;
        case 0x61:
            mode_stage_set(0x65);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x69);
            return;
        case 0x60:
            mode_stage_set(0x6A);
            return;
        case 0x61:
            mode_stage_set(0x6B);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x6C);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8014AD34(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
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
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
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
                talk_group = 4;
                break;
            }
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x10;
            break;
        case 2:
            talk_group = 0x11;
            break;
        case 3:
            talk_group = 0x12;
            break;
        case 4:
            talk_group = 0x13;
            break;
        }
        if (game_state.year == 0x60) {
            talk_group += 4;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(5);
        switch (temp_v0_3) {
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
        case 4:
            talk_group = 0x36;
            mode_step_next();
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
        if (girl_state_rank(5) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 16:
        mode_step_next();
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
        if (girl_state_rank(5) != 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[5].flags.f.b12 = 1;
            } else {
                game_state.girls[5].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8014B15C(void) {
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
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x5FB7);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 6;
                break;
            }
            func_8014DD28();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
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
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((girl_state_rank(5) == 4) || (girl_state_rank(5) == 2)) {
            mode_step_set(0x12);
        } else {
            mode_step_next();
        }
        break;
    case 16:
        if (girl_state_rank(5) == 3) {
            talk_group = 0xA;
        } else {
            talk_group = 8;
        }
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x15);
        } else {
            talk_group = 0x6F;
            mode_step_next();
        }
        break;
    case 20:
        normal_date_speak();
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

void func_8014B548(void) {
    s32 temp_v0_3;
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
            portrait_read_step(0x5FB7);
        }
        break;
    case 9:
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 6;
                break;
            }
            func_8014DD28();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = (game_state.year * 3) - 0x102;
            break;
        case 2:
            talk_group = (game_state.year * 3) - 0x101;
            break;
        case 3:
            talk_group = (game_state.year * 3) - 0x100;
            break;
        case 4:
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x12);
        } else {
            mode_step_next();
        }
        break;
    case 16:
        temp_v0_3 = girl_state_rank(5);
        switch (temp_v0_3) {                        /* switch 4; irregular */
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
        mode_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_girl_out();
        break;
    case 19:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x15);
        } else {
            talk_group = 0x6F;
            mode_step_next();
        }
        break;
    case 20:
        normal_date_speak();
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

void func_8014B9D8(void) {
    s32 temp_v0_4;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x24;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x25;
            break;
        case 2:
            talk_group = 0x26;
            break;
        case 3:
            talk_group = 0x27;
            break;
        case 4:
            talk_group = 0x28;
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
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 16:
        portrait_read_step(0x5FB7);
        break;
    case 17:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 18:
        if (girl_state_rank(5) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(5);
            switch (temp_v0_3) {
            case 0:
            case 1:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x32;
                } else {
                    talk_group = (g->year * 3) - 0xF4;
                }
                break;
            case 2:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x33;
                } else {
                    talk_group = (g->year * 3) - 0xF3;
                }
                break;
            case 3:
                if (((u16) g->player_flags.h >> 0xC) == 3) {
                    talk_group = 0x34;
                } else {
                    talk_group = (g->year * 3) - 0xF2;
                }
                break;
            case 4:
                talk_group = 0xF;
            }
            func_8014DD28();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x1A);
        } else {
            temp_v0_4 = girl_state_rank(5);
            switch (temp_v0_4) {                    /* switch 5; irregular */
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
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        if (((u16) g->player_flags.h >> 0xC) == 3) {
            talk_group = 0x35;
        } else {
            talk_group = 0x6F;
        }
        mode_step_next();
        break;
    case 25:
        normal_date_speak();
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
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8014BFE0(void) {
    s32 temp_v0_4;
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
            portrait_read_start(0x5FB7);
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
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x37;
            break;
        case 2:
            talk_group = 0x38;
            break;
        case 3:
            talk_group = 0x39;
            break;
        case 4:
            talk_group = 0x3A;
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
        if (girl_state_rank(5) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 14:
        audio_command(0x202);
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_next();
        break;
    case 15:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 16:
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_8014DD28();
        }
        break;
    case 18:
        mode_step_next();
        break;
    case 19:
        talk_group = D_80122EA8 + 0x3D;
        if (girl_state_rank(5) == 4) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        mode_step_next();
        temp_v0_3 = girl_state_rank(5);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x43;
            break;
        case 2:
            talk_group = 0x44;
            break;
        case 3:
            talk_group = 0x45;
            break;
        case 4:
            mode_step_set(0x1C);
            break;
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        temp_v0_4 = girl_state_rank(5);
        switch (temp_v0_4) {                        /* switch 5; irregular */
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
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        talk_group = 0x6F;
        mode_step_next();
        break;
    case 27:
        normal_date_speak();
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
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8014C528(void) {
    s32 temp_v0_3;
    u32 temp_v0;
    u32 temp_v0_2;
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
        portrait_read_step(0x5FB7);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
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
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            func_8014DD28();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 0x46;
        mode_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x47;
            break;
        case 2:
            talk_group = 0x48;
            break;
        case 3:
            talk_group = 0x49;
            break;
        case 4:
            talk_group = 0x4A;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(5) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0x12);
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        mode_step_set(0x1D);
        break;
    case 18:
        normal_date_bggirl_fadeout();
        break;
    case 19:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 1;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        game_mode_set(GAME_MODE_TT_GAME);
        break;
    case 20:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 21:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(5U);
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
        portrait_read_step(0x5FB7);
        break;
    case 25:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 26:
        normal_date_bggirl_fadein();
        break;
    case 27:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(5);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 0x4F;
                break;
            case 2:
                talk_group = 0x50;
                break;
            case 3:
                talk_group = 0x51;
                break;
            }
            func_8014DD28();
        }
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 30:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0xA4);
        break;
    case 31:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 32:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(5U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 33:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 35:
        portrait_read_step(0x5FB7);
        break;
    case 36:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 37:
        if (girl_state_rank(5) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 38:
        if (cd_read_sync_status() & 1) {
            temp_v0_4 = girl_state_rank(5);
            switch (temp_v0_4) {
            case 0:
            case 1:
                talk_group = 0x52;
                break;
            case 2:
                talk_group = 0x53;
                break;
            case 3:
                talk_group = 0x54;
                break;
            case 4:
                talk_group = 0x55;
                break;
            }
            func_8014DD28();
        }
        break;
    case 39:
        normal_date_speak();
        break;
    case 40:
        mode_step_next();
        temp_v0_5 = girl_state_rank(5);
        switch (temp_v0_5) {
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
        case 4:
            mode_step_set(0x2B);
            break;
        }
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        talk_group = 0x6F;
        normal_date_girl_out();
        break;
    case 43:
        if (girl_state_rank(5) == 4) {
            normal_date_bg_fadeout();
        } else {
            normal_date_speak();
        }
        break;
    case 44:
        talk_group = 0;
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 45:
        normal_date_speak();
        break;
    case 46:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8014CE94(void) {
    s32 temp_v0_3;
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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x57;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x58;
            break;
        case 2:
            talk_group = 0x59;
            break;
        case 3:
            talk_group = 0x5A;
            break;
        case 4:
            talk_group = 0x5B;
            break;
        }
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (girl_state_rank(5) == 4) {
            mode_step_set(0x10);
        } else {
            temp_v0_3 = girl_state_rank(5);
            switch (temp_v0_3) {                    /* switch 4; irregular */
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
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        mode_step_next();
        break;
    case 18:
        mode_step_next();
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        talk_group = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8014D240(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 7;
                break;
            }
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        talk_group = 0x5C;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x5D;
            break;
        case 2:
            talk_group = 0x5E;
            break;
        case 3:
            talk_group = 0x5F;
            break;
        case 4:
            talk_group = 0x60;
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
        talk_group = 0x61;
        mode_step_next();
        break;
    case 16:
        normal_date_speak();
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

void func_8014D558(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
            portrait_read_start(0x5FB7);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUKARI;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
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
                talk_group = 4;
                break;
            }
            func_8014DD28();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x62;
            break;
        case 2:
            talk_group = 0x63;
            break;
        case 3:
            talk_group = 0x64;
            break;
        case 4:
            talk_group = 0x65;
            break;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(5);
        switch (temp_v0_3) {
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
        case 4:
            talk_group = 0x36;
            mode_step_next();
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
        if (girl_state_rank(5) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 16:
        mode_step_next();
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

void func_8014D8F0(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

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
        portrait_read_step(0x5FB7);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        portrait_show_checked();
        cd_read_sectors(0xDU, 0x801E8000, 0x4A65);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 5;
                break;
            case 2:
                talk_group = 2;
                break;
            case 3:
                talk_group = 3;
                break;
            case 4:
                talk_group = 6;
                break;
            }
            func_8014DD28();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = (game_state.year * 3) - 0xB7;
            break;
        case 2:
            talk_group = (game_state.year * 3) - 0xB6;
            break;
        case 3:
            talk_group = (game_state.year * 3) - 0xB5;
            break;
        case 4:
            talk_group = 0xE;
            break;
        }
        mode_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        mode_step_next();
        temp_v0_3 = girl_state_rank(5);
        switch (temp_v0_3) {
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
        case 4:
            mode_step_set(0x11);
            break;
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        talk_group = 0x6F;
        mode_step_next();
        break;
    case 19:
        if (girl_state_rank(5) == 4) {
            mode_step_next();
        } else {
            normal_date_speak();
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

void func_8014DD28(void) {
    talk_names_set(5);
    func_80132270();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}
