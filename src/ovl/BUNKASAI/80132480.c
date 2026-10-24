#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80132480(void) {
    CultureFestivalDispatch *dispatch = (CultureFestivalDispatch *)&D_80122EB8;
    dispatch->unk_04 = 2;
    switch (dispatch->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            main_stage_set(0x30);
            return;
        }
        main_stage_set(0x2F);
        return;
    case 1:
        main_stage_set(0x31);
        return;
    case 2:
        main_stage_set(0x32);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x33);
            return;
        case 0x60:
            main_stage_set(0x35);
            return;
        case 0x61:
            main_stage_set(0x36);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x37);
            return;
        case 0x60:
            main_stage_set(0x38);
            return;
        case 0x61:
            main_stage_set(0x39);
            return;
        }
        break;
    case 5:
        main_stage_set(0x3A);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

/* T-12220: bit-field stores preserve the original byte-load register allocation. */
void func_801325F4(void) {
    u32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
            if (game_state.year == 0x5F) {
                dialogue_group = 0x24;
            } else {
                dialogue_group = 0x25;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 3;
                break;
            case 3:
                dialogue_group = 5;
                break;
            case 4:
                dialogue_group = 7;
                break;
            }
            if (game_state.year == 0x60) {
                dialogue_group += 8;
            }
            func_80135448();
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
        dialogue_group = 0;
        main_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(2) < 4) {
            if (game_state.year == 0x5F) {
                game_state.girls[2].flags.f.b12 = 1;
            } else {
                game_state.girls[2].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801328D4(void) {
    u32 temp_v0;

    GameState *g = &game_state;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        dialogue_group = 0x26;
        main_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x522E);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        bustup_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_80135448();
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
                main_step_set(0x11);
                break;
            case 4:
                dialogue_group = 0x1E;
                main_step_set(0x1E);
                break;
            }
        }
        break;
    case 12:
        if (girl_state_rank(2) < 2) {
            dialogue_group = 0x11;
        } else {
            dialogue_group = 0x13;
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
        normal_date_speak();
        break;
    case 16:
        main_step_set(0x23);
        break;
    case 17:
        dialogue_group = 0x15;
        main_step_next();
        /* fallthrough */
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (!(g->girls[2].syoushin & 3)) {
            dialogue_group = 0x21;
            main_step_set(0x1A);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                dialogue_group = 0x16;
            } else {
                dialogue_group = 0x17;
            }
            main_step_next();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (g->girls[2].flags.f.b2) {
            dialogue_group = 0x1A;
            main_step_set(0x18);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                dialogue_group = 0x18;
            } else {
                dialogue_group = 0x19;
            }
            g->girls[2].flags.f.b2 = 1;
            main_step_next();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        dialogue_group = 0x1B;
        main_step_next();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        dialogue_group = 0x1C;
        main_step_next();
        /* fallthrough */
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_girl_out();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        main_step_set(0x23);
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        normal_date_girl_in();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        normal_date_girl_out();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        normal_date_bg_fadeout();
        break;
    case 36:
        dialogue_group = 0;
        main_step_next();
        break;
    case 37:
        normal_date_speak();
        break;
    case 38:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80132DB4(void) {
    s32 temp_v0;

    GameState *g = &game_state;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (g->year == 0x5F) {
            dialogue_group = 0x27;
        } else {
            dialogue_group = 0x28;
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        main_step_next();
        break;
    case 8:
        bustup_read_step(0x522E);
        break;
    case 9:
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        bustup_show_checked();
        dec_bg_cd_read(0x40F4, 1);
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            func_8014338C();
            D_80125CA0 = 0xC0;
            cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
            main_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_80135448();
            if (girl_state_rank(2) == 4) {
                dialogue_group = 0x29;
                main_step_set(0x15);
            }
        }
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        main_step_next();
        temp_v0 = girl_state_rank(2);
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0:
        case 1:
            switch (g->year) {                   /* switch 3; irregular */
            case 0x5F:
                dialogue_group = 0x23;
                break;
            case 0x60:
                dialogue_group = 0x2C;
                break;
            case 0x61:
                dialogue_group = 0x32;
                break;
            }
            break;
        case 2:
            switch (g->year) {                   /* switch 4; irregular */
            case 0x5F:
                dialogue_group = 0x25;
                break;
            case 0x60:
                dialogue_group = 0x2E;
                break;
            case 0x61:
                dialogue_group = 0x34;
                break;
            }
            break;
        case 3:
            switch (g->year) {                   /* switch 5; irregular */
            case 0x5F:
                dialogue_group = 0x27;
                break;
            case 0x60:
                dialogue_group = 0x30;
                break;
            case 0x61:
                dialogue_group = 0x36;
                main_step_set(0x1C);
                break;
            }
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
        normal_date_speak();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        dialogue_group = 0;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    case 21:
        normal_date_bg_fadein();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_in();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        normal_date_girl_out();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        main_step_set(0x11);
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        if (!(g->girls[2].syoushin & 3)) {
            dialogue_group = 0x3F;
            main_step_set(0x24);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                dialogue_group = 0x37;
            } else {
                dialogue_group = 0x38;
            }
            main_step_next();
        }
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        if (g->girls[2].flags.f.b2) {
            dialogue_group = 0x3B;
            main_step_set(0x22);
        } else {
            if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                dialogue_group = 0x39;
            } else {
                dialogue_group = 0x3A;
            }
            g->girls[2].flags.f.b2 = 1;
            main_step_next();
        }
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        dialogue_group = 0x3C;
        main_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        dialogue_group = 0x3D;
        main_step_next();
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        normal_date_girl_out();
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        main_step_set(0x11);
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801333CC(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;

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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
            dialogue_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x41;
                break;
            case 2:
                dialogue_group = 0x42;
                break;
            case 3:
                dialogue_group = 0x43;
                break;
            case 4:
                dialogue_group = 0x44;
                break;
            }
            if (game_state.year == 0x61) {
                dialogue_group += 4;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((game_state.girls[2].flags.f.b2) && (game_state.year != 0x61)) {
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x49;
                break;
            case 2:
                dialogue_group = 0x4A;
                break;
            case 3:
                dialogue_group = 0x4B;
                break;
            case 4:
                dialogue_group = 0x4C;
                break;
            }
            main_step_next();
        } else {
            main_step_set(0xC);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (game_state.year == 0x61) {
            temp_v0_3 = girl_state_rank(2);
            switch (temp_v0_3) {
            case 0:
            case 1:
                dialogue_group = 0x4F;
                break;
            case 2:
                dialogue_group = 0x50;
                break;
            case 3:
                dialogue_group = 0x51;
                break;
            case 4:
                dialogue_group = 0x52;
                break;
            }
        } else if (girl_state_rank(2) == 4) {
            dialogue_group = 0x4E;
        } else {
            dialogue_group = 0x4D;
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
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 16:
        bustup_read_step(0x522E);
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
            cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        }
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
            temp_v0_4 = girl_state_rank(2);
            switch (temp_v0_4) {
            case 0:
            case 1:
                dialogue_group = 0x53;
                break;
            case 2:
                dialogue_group = 0x55;
                break;
            case 3:
                dialogue_group = 0x57;
                break;
            case 4:
                dialogue_group = 0x59;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 7)) - 0x299;
            func_80135448();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(2) == 4) {
            main_step_set(0x17);
        } else {
            normal_date_girl_out();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_bg_fadeout();
        break;
    case 24:
        dialogue_group = 0;
        main_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80133990(void) {
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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dialogue_group = 0x2A;
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x68;
                break;
            case 2:
                dialogue_group = 0x69;
                break;
            case 3:
                dialogue_group = 0x6A;
                break;
            case 4:
                dialogue_group = 0x6C;
                break;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(2) < 3) {
            main_step_next();
        } else {
            normal_date_speak();
        }
        break;
    case 12:
        snd_command(0x202);
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_next();
        break;
    case 13:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 14:
        if (girl_state_rank(2) >= 3) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = (D_80122EA8 * 2) + 0x6E;
                break;
            case 2:
                dialogue_group = (D_80122EA8 * 2) + 0x74;
                break;
            case 3:
            case 4:
                dialogue_group = D_80122EA8 + 0x7A;
                break;
            }
            func_80135448();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if ((girl_state_rank(2) < 2) && (D_80122EA8 != 1)) {
            snd_command(0x508);
        }
        main_step_next();
        break;
    case 18:
        if (girl_state_rank(2) < 3) {
            normal_date_speak();
        } else {
            main_step_set(0x17);
        }
        break;
    case 19:
        if (girl_state_rank(2) < 2) {
            dialogue_group = 0x7D;
        } else {
            dialogue_group = 0x7F;
        }
        main_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_bg_fadeout();
        break;
    case 24:
        dialogue_group = 0;
        main_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80133E18(void) {
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
        bustup_read_step(0x522E);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        bustup_show_checked();
        func_801431D4();
        dialogue_group = 0x2A;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x81;
                break;
            case 2:
                dialogue_group = 0x82;
                break;
            case 3:
                dialogue_group = 0x83;
                break;
            case 4:
                dialogue_group = 0x84;
                break;
            }
            func_80135448();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (girl_state_rank(2) == 4) {
            normal_date_girl_out();
        } else {
            main_step_set(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        main_step_next();
        break;
    case 14:
        if (girl_state_rank(2) == 4) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 15:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_mode = game_state.mode;
        game_state.saved_stage = game_state.stage;
        game_state.saved_step = game_state.step + 1;
        main_stage_set(0xA4);
        break;
    case 16:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0xA3);
        break;
    case 17:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(2U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 18:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 20:
        bustup_read_step(0x522E);
        break;
    case 21:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        bustup_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 22:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x86;
                break;
            case 2:
                dialogue_group = 0x88;
                break;
            case 3:
                dialogue_group = 0x8A;
                break;
            case 4:
                dialogue_group = 0x8C;
                break;
            }
            func_80135448();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (girl_state_rank(2) == 4) {
            main_step_set(0x1B);
        } else {
            normal_date_girl_out();
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_bg_fadeout();
        break;
    case 28:
        dialogue_group = 0;
        main_step_next();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801343B4(void) {
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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dialogue_group = 0x2A;
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x8E;
                break;
            case 2:
                dialogue_group = 0x8F;
                break;
            case 3:
                dialogue_group = 0x90;
                break;
            case 4:
                dialogue_group = 0x91;
                break;
            }
            func_80135448();
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
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 12:
        bustup_read_step(0x522E);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUINA;
        bustup_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 14:
        if (girl_state_rank(2) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x92;
                break;
            case 2:
                dialogue_group = 0x94;
                break;
            case 3:
                dialogue_group = 0x96;
                break;
            case 4:
                dialogue_group = 0x97;
                break;
            }
            func_80135448();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(2) == 4) {
            main_step_set(0x13);
        } else {
            if (girl_state_rank(2) == 3) {
                dialogue_group = 0x95;
            }
            normal_date_girl_out();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        dialogue_group = 0;
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

void func_801347A8(void) {
    u32 temp_v0;

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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
            dialogue_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x98;
                break;
            case 2:
                dialogue_group = 0x9D;
                break;
            case 3:
                dialogue_group = 0xA2;
                break;
            case 4:
                dialogue_group = 0xA7;
                break;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            normal_date_girl_out();
        } else {
            if ((u16) game_state.clubs[4].exp < 0x64U) {
                bustup_layout_set(4U);
                dialogue_group += 2;
            }
            main_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(2) == 4) {
            main_step_set(0xE);
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        dialogue_group = 0;
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

void func_80134A90(void) {
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
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_YUINA;
            bustup_show_checked();
            dialogue_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0xA9;
                break;
            case 2:
                dialogue_group = 0xAA;
                break;
            case 3:
                dialogue_group = 0xAB;
                break;
            case 4:
                dialogue_group = 0xAC;
                break;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        dialogue_group = 0xAD;
        main_step_next();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        dialogue_group = 0;
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

void func_80134CF8(void) {
    u32 temp_v0;

    GameState *g = &game_state;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x522E);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_YUINA;
            bustup_show_checked();
            dialogue_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        main_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(2);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0xAE;
                break;
            case 2:
                dialogue_group = 0xB0;
                break;
            case 3:
                dialogue_group = 0xB2;
                break;
            case 4:
                dialogue_group = 0xBB;
                break;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(2) == 3) {
            main_step_set(0x11);
        } else {
            main_step_next();
        }
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bg_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80142BC8();
        break;
    case 17:
        if (!(g->girls[2].syoushin & 3)) {
            dialogue_group = 0xB9;
            main_step_set(0x14);
        } else {
            if (!(g->girls[2].flags.f.b2)) {
                g->girls[2].flags.f.b2 = 1;
                if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                    dialogue_group = 0xB5;
                } else {
                    dialogue_group = 0xB6;
                }
            } else if (((u8) g->girls[2].flags.b[2] >> 4) == 2) {
                dialogue_group = 0xB3;
            } else {
                dialogue_group = 0xB4;
            }
            main_step_next();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        dialogue_group = 0xB7;
        main_step_next();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        normal_date_girl_out();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        main_step_set(0xD);
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801350A8(void) {
    s32 temp_v0;

    GameState *g = &game_state;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (g->year == 0x5F) {
            dialogue_group = 0x2E;
        } else {
            dialogue_group = 0x2F;
        }
        main_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x522E);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_YUINA;
        bustup_show_checked();
        cd_read_sectors(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 10:
        if (girl_state_rank(2) == 4) {
            dialogue_group = 0xC3;
            normal_date_bg_fadein();
        } else {
            main_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_80135448();
        }
        break;
    case 12:
        if (girl_state_rank(2) == 4) {
            normal_date_speak();
        } else {
            main_step_next();
        }
        break;
    case 13:
        if (girl_state_rank(2) == 4) {
            normal_date_girl_in();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 14:
        temp_v0 = girl_state_rank(2);
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0:
        case 1:
            dialogue_group = 0xBD;
            break;
        case 2:
            dialogue_group = 0xBF;
            break;
        case 3:
            dialogue_group = 0xC1;
            break;
        }
        if (girl_state_rank(2) < 4) {
            if (g->year == 0x60) {
                dialogue_group += 9;
            }
            if (g->year == 0x61) {
                dialogue_group += 0xF;
            }
        }
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        dialogue_group = 0;
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

void func_80135448(void) {
    dialogue_names_set(2);
    func_80132104();
    dialogue_line = 0;
    dialogue_text_table = D_801604F0;
    dialogue_command_table = D_801604F4;
    dialogue_voice_table = D_801604F8;
    main_step_next();
}
