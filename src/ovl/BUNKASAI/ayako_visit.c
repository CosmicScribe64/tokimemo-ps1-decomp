/* ayako_visit.c: src/ovl/BUNKASAI/8013C430.c:4: Ayako visit block, stages 65-79. Original object 0x8013C430-0x8013FD90 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013C430(void) {
    ((CultureFestivalDispatch *)&D_80122EB8)->unk_04 = 3;
    switch (((CultureFestivalDispatch *)&D_80122EB8)->unk_00) {
    case 0:
        if (game_state.year == 0x61) {
            mode_stage_set(0x43);
            return;
        }
        mode_stage_set(0x42);
        return;
    case 1:
        mode_stage_set(0x44);
        return;
    case 2:
        mode_stage_set(0x45);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x46);
            return;
        case 0x60:
            mode_stage_set(0x48);
            return;
        case 0x61:
            mode_stage_set(0x49);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            mode_stage_set(0x4A);
            return;
        case 0x60:
            mode_stage_set(0x4B);
            return;
        case 0x61:
            mode_stage_set(0x4C);
            return;
        }
        break;
    case 5:
        mode_stage_set(0x4D);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013C5A4(void) {
    GameState *g = &game_state;

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
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
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
        cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
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
            func_8013FC58();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        switch (girl_state_rank(3)) {
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
        if (g->year == 0x60) {
            talk_group += 5;
        }
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (girl_state_rank(3) == 4) {
            normal_date_girl_out();
        } else {
            switch (girl_state_rank(3)) {                    /* switch 4; irregular */
            case 0:
            case 1:
                talk_group = 0xF;
                break;
            case 2:
                talk_group = 0x11;
                break;
            case 3:
                talk_group = 0x13;
                break;
            }
            if (!(g->girls[3].syoushin & 3)) {
                talk_group = talk_group + 0x18;
            } else if ((g->year == 0x60) && (((u8) g->girls[3].flags.b[2] >> 4) == 4)) {
                talk_group = talk_group + 0x1E;
            } else {
                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group += 0xC;
                }

                if (!(g->girls[3].flags.f.b2)) {
                    talk_group = talk_group + 6;
                }
            }
            mode_step_next();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (girl_state_rank(3) < 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0x10);
        }
        break;
    case 15:
        normal_date_speak();
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
        if (girl_state_rank(3) < 4) {
            if (g->year == 0x5F) {
                g->girls[3].flags.f.b12 = 1;
            } else {
                g->girls[3].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void func_8013CA64(void) {
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
        portrait_read_step(0x58AF);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
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
            case 4:
                talk_group = 0x3C;
                break;
            }
            func_8013FC58();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_stage_set(0x4F);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013CCA8(void) {
    GameState *g = &game_state;

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
        if (g->year == 0x5F) {
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
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
            portrait_read_step(0x58AF);
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            func_8014338C();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
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
            case 4:
                talk_group = 0x7A;
                break;
            }
            func_8013FC58();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        mode_step_next();
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = (g->year * 3) - 0xAC;
            break;
        case 2:
            talk_group = (g->year * 3) - 0xAB;
            break;
        case 3:
            talk_group = (g->year * 3) - 0xAA;
            break;
        case 4:
            mode_step_set(0x11);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x11);
        } else if (g->year == 0x61) {
            mode_stage_set(0x4F);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x68;
                    break;
                case 2:
                    talk_group = 0x6A;
                    break;
                case 3:
                    talk_group = 0x6C;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {                /* switch 5; irregular */
                default:

                    break;
                case 0:
                case 1:
                    talk_group = 0x7C;
                    break;
                case 2:
                    talk_group = 0x7E;
                    break;
                case 3:
                    talk_group = 0x80;
                    break;
                }
                if (!(g->girls[3].flags.f.b2)) {
                    talk_group += 6;
                    g->girls[3].flags.f.b2 = 1;
                }
                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group += 0xC;
                }

                if (g->year == 0x60) {
                    talk_group = talk_group + 0x18;
                }
            }
            mode_step_next();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_speak();
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


void func_8013D1F8(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        mode_step_next();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
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
                talk_group = 0xC;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xE);
        } else if (!(g->girls[3].flags.f.b2)) {
            mode_step_set(0xC);
        } else {
            if (((u8) g->girls[3].flags.b[2] >> 4) == 4) {
                talk_group = 4;
            } else {
                talk_group = 5;
            }

            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 6;
            break;
        case 2:
            talk_group = 7;
            break;
        case 3:
            talk_group = 8;
            break;
        }
        if (!(g->girls[3].flags.f.b2)) {
            talk_group += 3;
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
        portrait_read_step(0x58AF);
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        }
        break;
    case 18:
        if (girl_state_rank(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = (g->year * 3) - 0x10F;
                break;
            case 2:
                talk_group = (g->year * 3) - 0x10E;
                break;
            case 3:
                talk_group = (g->year * 3) - 0x10D;
                break;
            case 4:
                talk_group = 0xD;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x19);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    if (g->year == 0x60) {
                        talk_group = 0xB5;
                    } else {
                        talk_group = 0x17;
                    }
                    break;
                case 2:
                    talk_group = 0x19;
                    break;
                case 3:
                    talk_group = 0x1B;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {
                case 0:
                case 1:
                    talk_group = 0x1D;
                    break;
                case 2:
                    talk_group = 0x1F;
                    break;
                case 3:
                    talk_group = 0x21;
                    break;
                case 4:
                    mode_step_set(0x19);
                    break;
                }

                if (!(g->girls[3].flags.f.b2)) {
                    g->girls[3].flags.f.b2 = 1;
                    talk_group += 6;
                }
                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group += 0xC;
                }
                talk_group = (talk_group + (g->year * 0x18)) - 0x8E8;
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
        normal_date_speak();
        break;
    case 25:
        normal_date_bg_fadeout();
        break;
    case 26:
        talk_group = 0;
        mode_step_next();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void func_8013D8E0(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
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
            portrait_read_start(0x58AF);
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
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x65;

                break;
            case 2:
                talk_group = 0x66;
                break;
            case 3:
                talk_group = 0x67;
                break;
            case 4:
                talk_group = 0x68;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(3) == 4) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 12:
        audio_command(0x202);
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        mode_stage_next();
        break;
    case 13:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 14:
        if (girl_state_rank(3) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x6A;

                break;
            case 2:
                talk_group = 0x6D;
                break;
            case 3:
                talk_group = 0x70;
                break;
            case 4:
                talk_group = 0x73;
                break;
            }
            talk_group += D_80122EA8;
            func_8013FCBC();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x15);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x17;
                    break;
                case 2:
                    talk_group = 0x19;
                    break;
                case 3:
                    talk_group = 0x1B;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {                /* switch 5; irregular */
                default:

                    break;
                case 0:
                case 1:
                    talk_group = 0x76;
                    break;
                case 2:
                    talk_group = 0x78;
                    break;
                case 3:
                    talk_group = 0x7A;
                    break;
                }
                if (!(g->girls[3].flags.f.b2)) {
                    g->girls[3].flags.f.b2 = 1;
                    talk_group += 6;
                }

                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group = talk_group + 0xC;
                }
            }
            mode_step_next();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
            if (girl_state_rank(3) == 3) {
                talk_group = 0x87;
            }
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


/* FAKE: scalar byte selector view preserves its target live range
 * (T-10630). The flag writes precede the saved-state copies so IDO emits
 * the original case-16 temporary order. */
void func_8013DE3C(void) {

    switch (*(u8 *)&game_state.mode_step) {
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
        portrait_read_step(0x58AF);
        break;
    case 4:
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
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
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
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
                talk_group = 0x90;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        mode_step_next();
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 0x8D;
            break;
        case 2:
            talk_group = 0x8E;
            break;
        case 3:
            talk_group = 0x8F;
            break;
        case 4:
            mode_step_set(0xD);
            break;
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(3) == 4) {
            normal_date_girl_out();
        } else {
            mode_step_set(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (girl_state_rank(3) == 4) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = game_state.mode_step + 1;
        mode_stage_set(0xA4);
        break;
    case 17:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.mode_stage;
        D_80122EB4 = game_state.mode_step + 1;
        mode_stage_set(0xA3);
        break;
    case 18:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        talk_names_set(3U);
        D_80122E98 = 0;
        mode_step_next();
        break;
    case 19:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            mode_step_next();
        }
        break;
    case 20:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 21:
        portrait_read_step(0x58AF);
        break;
    case 22:
        addr_init_bustup();
        game_state.current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        break;
    case 23:
        if (girl_state_rank(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x92;
                break;
            case 2:
                talk_group = 0x94;
                break;
            case 3:
                talk_group = 0x96;
                break;
            case 4:
                talk_group = 0x98;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                talk_group += 1;
            }
            func_8013FCBC();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        mode_step_next();
        if (!(game_state.girls[3].syoushin & 3)) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x17;
                break;
            case 2:
                talk_group = 0x19;
                break;
            case 3:
                talk_group = 0x1B;
                break;
            case 4:
                mode_step_set(0x1E);
                break;
            }
        } else {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0x9A;
                break;
            case 2:
                talk_group = 0x9C;
                break;
            case 3:
                talk_group = 0x9E;
                break;
            case 4:
                mode_step_set(0x1E);
                break;
            }
            if (!game_state.girls[3].flags.f.b2) {
                talk_group += 6;
            }
            if (((u8) game_state.girls[3].flags.b[2] >> 4) == 5) {
                talk_group = talk_group + 0xC;
            }
        }
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_bg_fadeout();
        break;
    case 31:
        talk_group = 0;
        mode_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void func_8013E59C(void) {
    GameState *g = &game_state;

    switch (g->mode_step) {
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
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            talk_group = 0x29;
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
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
                talk_group = 0xC;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xE);
        } else if (!(g->girls[3].flags.f.b2)) {
            mode_step_set(0xC);
        } else {
            if (((u8) g->girls[3].flags.b[2] >> 4) == 4) {
                talk_group = 4;
            } else {
                talk_group = 5;
            }

            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        switch (girl_state_rank(3)) {
        case 0:
        case 1:
            talk_group = 6;
            break;
        case 2:
            talk_group = 7;
            break;
        case 3:
            talk_group = 8;
            break;
        }
        if (!(g->girls[3].flags.f.b2)) {
            talk_group += 3;
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
        portrait_read_step(0x58AF);
        break;
    case 17:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        break;
    case 18:
        if (girl_state_rank(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = 0xB2;

                break;
            case 2:
                talk_group = 0xB3;
                break;
            case 3:
                talk_group = 0xB4;
                break;
            case 4:
                talk_group = 0xD;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0x19);
        } else {
            if (!(g->girls[3].syoushin & 3)) {
                switch (girl_state_rank(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    talk_group = 0x17;
                    break;
                case 2:
                    talk_group = 0x19;
                    break;
                case 3:
                    talk_group = 0x1B;
                    break;
                }
            } else {
                switch (girl_state_rank(3)) {                /* switch 5; irregular */
                case 0:
                case 1:
                    talk_group = 0x4D;
                    break;
                case 2:
                    talk_group = 0x4F;
                    break;
                case 3:
                talk_group = 0x51;
                break;
                }
                if (!(g->girls[3].flags.f.b2)) {
                    g->girls[3].flags.f.b2 = 1;
                    talk_group += 6;
                }

                if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                    talk_group = talk_group + 0xC;
                }
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
        normal_date_speak();
        break;
    case 25:
        normal_date_bg_fadeout();
        break;
    case 26:
        talk_group = 0;
        mode_step_next();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}


void func_8013EBDC(void) {
    GameState *g = &game_state;
    s32 temp_v0_2;
    s32 temp_v0_3;
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
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x2B;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 1;
                break;
            case 2:
                talk_group = 4;
                break;
            case 3:
                talk_group = 7;
                break;
            case 4:
                talk_group = 0xA;
                break;
            }
            func_8013FD20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
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
        if (!(g->girls[3].syoushin & 3)) {
            temp_v0_2 = girl_state_rank(3);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
        } else {
            temp_v0_3 = girl_state_rank(3);
            switch (temp_v0_3) {
            case 0:
            case 1:
                talk_group = 0xC;
                break;
            case 2:
                talk_group = 0xE;
                break;
            case 3:
                talk_group = 0x10;
                break;
            }
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }
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
        normal_date_speak();
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

void func_8013EFB8(void) {
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
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
            switch (temp_v0) {
            case 0:
            case 1:
                talk_group = 0x1E;
                break;
            case 2:
                talk_group = 0x1F;
                break;
            case 3:
                talk_group = 0x20;
                break;
            case 4:
                talk_group = 0x22;
                break;
            }
            func_8013FD20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_girl_out();
        break;
    case 11:
        if (girl_state_rank(3) != 4) {
            talk_group = 0x21;
        }
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
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013F234(void) {
    GameState *g = &game_state;
    s32 temp_v0_2;
    s32 temp_v0_3;
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
            portrait_read_start(0x58AF);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->current_girl = GIRL_AYAKO;
            portrait_show_checked();
            talk_group = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        mode_step_next();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(3);
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
            case 4:
                talk_group = 0x27;
                break;
            }
            func_8013FD20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xC);
        } else if (!(g->girls[3].syoushin & 3)) {
            temp_v0_2 = girl_state_rank(3);
            switch (temp_v0_2) {
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
            mode_step_next();
        } else {
            temp_v0_3 = girl_state_rank(3);
            switch (temp_v0_3) {
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
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }
            mode_step_next();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_speak();
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
        func_80142BC8();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_8013F5D4(void) {
    GameState *g = &game_state;

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
        if (g->year == 0x5F) {
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
        D_80122EB0 = (s32) g->mode_stage;
        D_80122EB4 = g->mode_step + 1;
        game_mode_set(GAME_MODE_CULTURE_STAGE);
        break;
    case 8:
        portrait_read_step(0x58AF);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->current_girl = GIRL_AYAKO;
        portrait_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (girl_state_rank(3)) {
            case 0:
            case 1:
                talk_group = (g->year * 3) - 0xE8;
                break;
            case 2:
                talk_group = (g->year * 3) - 0xE7;
                break;
            case 3:
                talk_group = (g->year * 3) - 0xE6;
                break;
            case 4:
                talk_group = 0x3E;
                break;
            }
            func_8013FD20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(3) == 4) {
            mode_step_set(0xF);
        } else if (!(g->girls[3].syoushin & 3)) {
            switch (girl_state_rank(3)) {                    /* switch 3; irregular */
            case 0:
            case 1:
                talk_group = 0x18;
                break;
            case 2:
                talk_group = 0x1A;
                break;
            case 3:
                talk_group = 0x1C;
                break;
            }
            mode_step_next();
        } else {
            switch (girl_state_rank(3)) {                    /* switch 4; irregular */

            case 0:
            case 1:
                talk_group = 0x40;
                break;
            case 2:
                talk_group = 0x42;
                break;
            case 3:
                talk_group = 0x44;
                break;
            }
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }
            talk_group = (talk_group + (g->year * 0xC)) - 0x474;
            mode_step_next();
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


void func_8013FA20(void) {
}

void func_8013FA28(void) {
    GameState *g = &game_state;
    s32 temp_v0;

    switch (g->mode_step) {
    case 0:
        if (girl_state_rank(3) == 4) {
            mode_step_set(2);
            return;
        }
        temp_v0 = girl_state_rank(3);
        switch (temp_v0) {
        case 0:
        case 1:
            talk_group = 0x50;
            break;
        case 2:
            talk_group = 0x52;
            break;
        case 3:
            talk_group = 0x54;
            break;
        }
        if (!(g->girls[3].syoushin & 3)) {
            talk_group = talk_group + 0x18;
        } else {
            if (!(g->girls[3].flags.f.b2)) {
                talk_group += 6;
                g->girls[3].flags.f.b2 = 1;
            }

            if (((u8) g->girls[3].flags.b[2] >> 4) == 5) {
                talk_group = talk_group + 0xC;
            }
        }
        mode_step_next();
        return;
    case 1:
        normal_date_speak();
        return;
    case 2:
        normal_date_girl_out();
        return;
    case 3:
        normal_date_speak();
        return;
    case 4:
        normal_date_bg_fadeout();
        return;
    case 5:
        talk_group = 0;
        mode_step_next();
        return;
    case 6:
        normal_date_speak();
        return;
    case 7:
        if ((((u16) g->player_flags.h >> 0xC) == 0) && (girl_state_rank(3) != 4)) {
            if (g->year == 0x5F) {
                g->girls[3].flags.f.b12 = 1;
            }
            if (g->year == 0x60) {
                g->girls[3].flags.f.b13 = 1;
            }
        }
        func_80142BC8();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013FC58(void) {
    talk_names_set(3);
    func_8013216C();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}

void func_8013FCBC(void) {
    talk_names_set(3);
    func_801321A0();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}

void func_8013FD20(void) {
    talk_names_set(3);
    func_801321D4();
    talk_line = 0;
    talk_text_table = D_801604F0;
    talk_command_table = D_801604F4;
    talk_voice_table = D_801604F8;
    mode_step_next();
}
