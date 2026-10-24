#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_8014A6A0(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x21);
            return;
        case 0x60:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x21);
            return;
        case 0x61:
            main_stage_set(0x22);
            return;
        default:
            game_reset_to_title();
            return;
        }
        break;
    case 1:
        main_stage_set(0x23);
        return;
    case 2:
        if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            main_stage_set(5);
            return;
        }
        main_stage_set(0x24);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x25);
            break;
        case 0x60:
            main_stage_set(0x26);
            break;
        case 0x61:
            main_stage_set(0x27);
            break;
        }
        if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
            ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
            main_stage_set(5);
            return;
        }
        return;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x28);
            return;
        case 0x60:
            main_stage_set(0x29);
            return;
        case 0x61:
            if ((girl_state_rank(0) == 4) && !(game_state.girls[0].flags.f.b2)) {
                ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 0xFF;
                main_stage_set(5);
                return;
            }
            main_stage_set(0x2A);
            return;
        }
        break;
    case 5:
        main_stage_set(0x2B);
        return;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8014A9A8(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x544A);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            }
            if ((D_80122EA4 == 0) || (girl_state_rank(0) == 4)) {
                if (g->year == 0x5F) {
                    dialogue_group = 0xD;
                } else {
                    dialogue_group = 0x16;
                }
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (!(g->girls[0].flags.f.b2) && (girl_state_rank(0) != 4)) {
            g->girls[0].flags.f.b2 = 1;
            dialogue_group = 4;
            main_step_next();
        } else {
            main_step_set(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((D_80122EA4 == 0) || (girl_state_rank(0) == 4)) {
            main_step_set(0xC);
        } else {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                if (g->year == 0x5F) {
                    dialogue_group = 7;
                } else {
                    dialogue_group = 0x10;
                }
                break;
            case 2:
                if (g->year == 0x5F) {
                    dialogue_group = 9;
                } else {
                    dialogue_group = 0x12;
                }
                break;
            case 3:
                if (g->year == 0x5F) {
                    dialogue_group = 0xB;
                } else {
                    dialogue_group = 0x14;
                }
                break;
            }
            main_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            dialogue_group = 0xE;
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                if (g->year == 0x5F) {
                    dialogue_group = 8;
                } else {
                    dialogue_group = 0x11;
                }
                break;
            case 2:
                if (g->year == 0x5F) {
                    dialogue_group = 0xA;
                } else {
                    dialogue_group = 0x13;
                }
                break;
            case 3:
                if (g->year == 0x5F) {
                    dialogue_group = 0xC;
                } else {
                    dialogue_group = 0x15;
                }
                break;
            }
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
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
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014AF48(void) {
    GameState *g = &game_state;
    u32 temp_v0;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (g->girls[0].flags.f.b2) {
                dialogue_group = 0x19;
            } else {
                dialogue_group = 0x1A;
            }
            func_8014E3DC();
        }
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
        func_80145A30();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            if (g->girls[0].flags.f.b2) {
                main_step_set(0xB);
            } else {
                g->girls[0].flags.f.b2 = 1;
                dialogue_group = 4;
            }
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        dialogue_group = 6;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x544A);
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            func_801459FC();
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        temp_v0 = girl_state_rank(0);
        switch (temp_v0) {
        case 0:
        case 1:
            dialogue_group = 0x1B;
            break;
        case 2:
            dialogue_group = 0x1C;
            break;
        case 3:
            dialogue_group = 0x1D;
            break;
        case 4:
            dialogue_group = 0x1E;
            break;
        }
        if (D_80122EA4 == 0) {
            dialogue_group = 0x1E;
        }
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
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
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8014B378(void) {
    GameState *g = &game_state;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dialogue_group = 0x1F;
            func_8014E3DC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        func_80145A30();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            if (g->girls[0].flags.f.b2) {
                main_step_set(0xB);
            } else {
                dialogue_group = 5;
                g->girls[0].flags.f.b2 = 1;
            }
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        dialogue_group = 6;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x5477);
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            func_801459FC();
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
        }
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        switch (girl_state_rank(0)) {
        case 0:
        case 1:
            dialogue_group = 0x20;

            break;
        case 2:
            dialogue_group = 0x21;
            break;
        case 3:
            dialogue_group = 0x22;
            break;
        case 4:
            dialogue_group = 0x23;
            break;
        }

        if (D_80122EA4 == 0) {
            dialogue_group = 0x23;
        }
        dialogue_group = (dialogue_group + (g->year * 4)) - 0x17C;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        g->girls[0].flags.f.b2 = 1;
        dialogue_group = 0;
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014B7B8(void) {
    GameState *g = &game_state;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x54D1);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            case 4:
                dialogue_group = 0x32;
                break;
            }
            if (D_80122EA4 == 0) {
                dialogue_group = 0x32;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            main_step_set(0xA);
        } else {
            dialogue_group = 4;
            g->girls[0].flags.f.b2 = 1;
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        main_step_next();
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x2C;
            break;
        case 2:
            dialogue_group = 0x2E;
            break;
        case 3:
            dialogue_group = 0x30;
            break;
        case 4:
            main_step_set(0xC);
            break;
        }
        if (D_80122EA4 == 0) {
            main_step_set(0xC);
        }
        dialogue_group = (dialogue_group + (g->year * 8)) - 0x2F8;
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 2;
        }
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 14:
        bustup_read_step(0x54D1);
        break;
    case 15:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
                dialogue_group = 0x2D;
                break;
            case 2:
                dialogue_group = 0x2F;
                break;
            case 3:
                dialogue_group = 0x31;
                break;
            case 4:
                dialogue_group = 0x33;
                break;
            }
            if (D_80122EA4 == 0) {
                dialogue_group = 0x33;
            }
            dialogue_group = (dialogue_group + (g->year * 8)) - 0x2F8;
            func_8014E3DC();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        dialogue_group = 0;
        main_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014BD80(void) {
    GameState *g = &game_state;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    u32 temp_v0;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x544A);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            case 4:
                dialogue_group = 0x56;
                break;
            }
            if (D_80122EA4 == 0) {
                dialogue_group = 0x56;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            main_step_set(0xA);
        } else {
            g->girls[0].flags.f.b2 = 1;
            dialogue_group = 4;
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            dialogue_group = 0x44;
            break;
        case 2:
            dialogue_group = 0x4A;
            break;
        case 3:
            dialogue_group = 0x50;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_set(0xD);
        } else {
            main_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        snd_command(0x202);
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_stage_set(0xB);
        break;
    case 14:
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        main_step_next();
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                dialogue_group = 0x45;
                break;
            case 2:
                dialogue_group = 0x4B;
                break;
            case 3:
                dialogue_group = 0x51;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                dialogue_group = 0x57;
                main_step_set(0x12);
            }
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        dialogue_group += D_80122EA8;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        temp_v0_4 = girl_state_rank(0);
        switch (temp_v0_4) {                        /* switch 5; irregular */
        case 0:
        case 1:
            dialogue_group = 0x49;
            break;
        case 2:
            dialogue_group = 0x4F;
            break;
        case 3:
            dialogue_group = 0x55;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_set(0x16);
        } else {
            main_step_next();
        }
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        dialogue_group = 0;
        main_step_next();
        break;
    case 23:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 24:
        normal_date_bg_fadeout();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014C374(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x544A);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                dialogue_group = 0x69;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            main_step_set(0xA);
        } else {
            g->girls[0].flags.f.b2 = 1;
            dialogue_group = 4;
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x5A;
            break;
        case 2:
            dialogue_group = 0x5F;
            break;
        case 3:
            dialogue_group = 0x64;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_set(0xC);
        } else {
            main_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 13:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        g->saved_mode = g->mode;
        g->saved_stage = g->stage;
        g->saved_step = g->step + 1;
        main_stage_set(0x8D);
        break;
    case 14:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_stage_set(0x8C);
        break;
    case 15:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(8U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 16:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 18:
        bustup_read_step(0x544A);
        break;
    case 19:
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        break;
    case 20:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 21:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
                dialogue_group = 0x5B;
                break;
            case 2:
                dialogue_group = 0x60;
                break;
            case 3:
                dialogue_group = 0x65;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                dialogue_group = 0x6A;
            }
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        main_step_next();
        temp_v0_4 = girl_state_rank(0);
        switch (temp_v0_4) {
        case 0:
        case 1:
            dialogue_group = 0x5C;
            break;
        case 2:
            dialogue_group = 0x61;
            break;
        case 3:
            dialogue_group = 0x66;
            break;
        }
        if ((u8) D_800E7D4E < 2U) {
            dialogue_group += 1;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_set(0x1B);
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        temp_v0_5 = girl_state_rank(0);
        switch (temp_v0_5) {
        case 0:
        case 1:
            dialogue_group = 0x5E;
            break;
        case 2:
            dialogue_group = 0x63;
            break;
        case 3:
            dialogue_group = 0x68;
            break;
        }
        main_step_next();
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 28:
        dialogue_group = 0;
        main_step_next();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8014CB4C(void) {
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
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x544A);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                main_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->girls[0].flags.f.b2) {
            main_step_set(0xA);
        } else {
            dialogue_group = 4;
            g->girls[0].flags.f.b2 = 1;
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            dialogue_group = 0x6B;
            break;
        case 2:
            dialogue_group = 0x6C;
            break;
        case 3:
            dialogue_group = 0x6D;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            dialogue_group = 0x71;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 2;
        }
        D_80122EB0 = (s32) g->stage;
        D_80122EB4 = g->step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 14:
        bustup_read_start(0x544A);
        main_step_next();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                dialogue_group = 0x6E;
                break;
            case 2:
                dialogue_group = 0x6F;
                break;
            case 3:
                dialogue_group = 0x70;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                dialogue_group = 0x72;
            }
            func_8014E3DC();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        dialogue_group = 0;
        main_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014D0FC(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 3:
        bustup_read_step(0x5828);
        break;
    case 4:
        memcpy((void *)0x80197000, 0x80180000, 0x16800);
        main_step_next();
        break;
    case 5:
        bustup_read_step(0x54A4);
        break;
    case 6:
        memcpy((void *)0x801AE000, 0x80180000, 0x16800);
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        bustup_show_checked();
        func_80145844();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        break;
    case 7:
        normal_date_bg_fadein();
        break;
    case 8:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                dialogue_group = 0x85;
                main_step_set(0x1E);
            } else {
                temp_v0 = girl_state_rank(0);
                switch (temp_v0) {                  /* switch 2; irregular */
                case 0:
                case 1:
                    dialogue_group = 1;
                    break;
                case 2:
                    dialogue_group = 2;
                    break;
                case 3:
                    dialogue_group = 3;
                    break;
                }
            }
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (g->girls[0].flags.f.b2) {
            main_step_set(0xD);
        } else {
            dialogue_group = 4;
            g->girls[0].flags.f.b2 = 1;
            main_step_next();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            dialogue_group = 0x73;
            break;
        case 2:
            dialogue_group = 0x79;
            break;
        case 3:
            dialogue_group = 0x7F;
            break;
        }
        main_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        dialogue_names_set(0xCU);
        g->current_girl = GIRL_REI;
        D_80122D20 = 1;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        memcpy((void *)0x80180000, 0x80197000, 0x16800);
        main_step_next();
        break;
    case 19:
        D_80122D20 = 0;
        addr_init_bustup();
        bustup_show_checked();
        bustup_layout_set(0U);
        break;
    case 20:
        normal_date_girl_in();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        dialogue_names_set(0U);
        g->current_girl = GIRL_SHIORI;
        D_80122D20 = 1;
        main_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        dialogue_names_set(0xCU);
        g->current_girl = GIRL_REI;
        D_80122D20 = 0;
        main_step_next();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_girl_out();
        break;
    case 27:
        memcpy((void *)0x80180000, 0x801AE000, 0x16800);
        main_step_next();
        break;
    case 28:
        addr_init_bustup();
        g->current_girl = GIRL_SHIORI;
        bustup_show_checked();
        dialogue_names_set(0U);
        break;
    case 29:
        normal_date_girl_in();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 32:
        normal_date_bg_fadeout();
        break;
    case 33:
        dialogue_group = 0;
        main_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014D65C(void) {
    GameState *g = &game_state;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            if (girl_state_rank(0) == 4) {
                bustup_read_start(0x6C5F);
            } else {
                bustup_read_start(0x54A4);
            }
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            /* FAKE: the empty pointer test keeps the GameState base in v1
             * in later cases; the test itself is eliminated. T-10630. */
            if (!g) {}
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                dialogue_group = 0x86;
                break;
            case 2:
                dialogue_group = 0x87;
                break;
            case 3:
                dialogue_group = 0x88;
                break;
            }
            addr_init_bustup();
            if (girl_state_rank(0) == 4) {
                dialogue_group = 0x89;
                g->current_girl = GIRL_YOSHIO;
                dialogue_names_set(0xDU);
            } else {
                g->current_girl = GIRL_SHIORI;
            }
            bustup_show_checked();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (girl_state_rank(0) == 4) {
            dialogue_group = 0x8C;
        } else if (g->girls[0].flags.f.b2) {
            dialogue_group = 0x8A;
        } else {
            g->girls[0].flags.f.b2 = 1;
            dialogue_group = 0x8B;
        }
        main_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0) == 4) {
            main_step_set(0x14);
        } else {
            main_step_next();
        }
        break;
    case 11:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            dialogue_group = 0x8D;
            break;
        case 2:
            dialogue_group = 0x8E;
            break;
        case 3:
            dialogue_group = 0x8F;
            break;
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
        dialogue_group = 0x90;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        dialogue_group = 0;
        g->girls[0].flags.f.b2 = 1;
        main_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_80145530();
        break;
    case 20:
        normal_date_two_select();
        break;
    case 21:
        if (D_80122CDC != 0) {
            dialogue_group = 0x94;
        } else {
            dialogue_group = 0x91;
        }
        main_step_next();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        if (D_80122CDC != 0) {
            main_step_set(0x10);
        } else {
            dialogue_names_set(0U);
            if (g->girls[0].flags.f.b2) {
                dialogue_group = 0x92;
            } else {
                dialogue_group = 0x93;
            }
            main_step_next();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        main_step_set(0x11);
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014DB34(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

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
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            bustup_read_start(0x54A4);
            main_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 4:
        func_80145844();
        main_step_next();
        break;
    case 5:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 2;
                break;
            case 3:
                dialogue_group = 3;
                break;
            }
            if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
                main_step_set(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (game_state.girls[0].flags.f.b2) {
            main_step_set(0xA);
        } else {
            dialogue_group = 4;
            main_step_next();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            dialogue_group = 0x95;
            break;
        case 2:
            dialogue_group = 0x97;
            break;
        case 3:
            dialogue_group = 0x99;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            dialogue_group = 0x9B;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            dialogue_group = 0x9C;
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            temp_v0_3 = girl_state_rank(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                dialogue_group = 0x96;
                break;
            case 2:
                dialogue_group = 0x98;
                break;
            case 3:
                dialogue_group = 0x9A;
                break;
            }
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
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
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8014DFB8(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dialogue_group = 0x9E;
            func_8014E3DC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        func_80145A30();
        cd_read_sectors(0x17U, 0x801E8000, 0x49EB);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8014E3DC();
            if (game_state.girls[0].flags.f.b2) {
                main_step_set(0xB);
            } else {
                dialogue_group = 5;
            }
            bustup_read_start(0x544A);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        dialogue_group = 6;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_SHIORI;
            bustup_show_checked();
            dec_bg_cd_read(0x40F4, 1);
        }
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_801459FC();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 15:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        temp_v0 = girl_state_rank(0);
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0:
        case 1:
            dialogue_group = 0x9F;
            break;
        case 2:
            dialogue_group = 0xA0;
            break;
        case 3:
            dialogue_group = 0xA1;
            break;
        }
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            dialogue_group = 0xA2;
        }
        dialogue_group = (dialogue_group + (game_state.year * 4)) - 0x17C;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) {
            main_step_next();
        } else {
            normal_date_girl_out();
        }
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
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8014E3DC(void) {
    dialogue_names_set(0);
    func_80132034();
    dialogue_line = 0;
    dialogue_text_table = D_80155E90;
    dialogue_command_table = D_80155E94;
    dialogue_voice_table = D_80155E98;
    main_step_next();
}
