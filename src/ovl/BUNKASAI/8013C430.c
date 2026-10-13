#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013C430(void) {
    ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 3;
    switch (((BunkaDispatch8 *)&D_80122EB8)->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x43);
            return;
        }
        func_80042908(0x42);
        return;
    case 1:
        func_80042908(0x44);
        return;
    case 2:
        func_80042908(0x45);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x46);
            return;
        case 0x60:
            func_80042908(0x48);
            return;
        case 0x61:
            func_80042908(0x49);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x4A);
            return;
        case 0x60:
            func_80042908(0x4B);
            return;
        case 0x61:
            func_80042908(0x4C);
            return;
        }
        break;
    case 5:
        func_80042908(0x4D);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013C5A4(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 3;
            func_80083474();
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 0x24;
            } else {
                D_800CA148 = 0x25;
            }
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 1;

                break;
            case 2:
                D_800CA148 = 2;
                break;
            case 3:
                D_800CA148 = 3;
                break;
            case 4:
                D_800CA148 = 4;
                break;
            }
            func_8013FC58();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        switch (func_8008667C(3)) {
        case 0:
        case 1:
            D_800CA148 = 5;

            break;
        case 2:
            D_800CA148 = 6;
            break;
        case 3:
            D_800CA148 = 7;
            break;
        case 4:
            D_800CA148 = 8;
            break;
        }
        if (g->unk_03E == 0x60) {
            D_800CA148 += 5;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (func_8008667C(3) == 4) {
            normal_date_girl_out();
        } else {
            switch (func_8008667C(3)) {                    /* switch 4; irregular */
            case 0:
            case 1:
                D_800CA148 = 0xF;
                break;
            case 2:
                D_800CA148 = 0x11;
                break;
            case 3:
                D_800CA148 = 0x13;
                break;
            }
            if (!(g->unk_1BC[3].unk_0A & 3)) {
                D_800CA148 = D_800CA148 + 0x18;
            } else if ((g->unk_03E == 0x60) && (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 4)) {
                D_800CA148 = D_800CA148 + 0x1E;
            } else {
                if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                    D_800CA148 += 0xC;
                }

                if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                    D_800CA148 = D_800CA148 + 6;
                }
            }
            func_8004284C();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if (func_8008667C(3) < 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0x10);
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (func_8008667C(3) < 4) {
            if (g->unk_03E == 0x5F) {
                g->unk_1BC[3].unk_0C.f.b12 = 1;
            } else {
                g->unk_1BC[3].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8013CA64(void) {
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        D_800CA148 = 0x26;
        func_8004284C();
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E934();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x58AF);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        func_8007C740();
        D_800E6280.unk_F5F = 3;
        func_80083474();
        cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x39;
                break;
            case 2:
                D_800CA148 = 0x3A;
                break;
            case 3:
                D_800CA148 = 0x3B;
                break;
            case 4:
                D_800CA148 = 0x3C;
                break;
            }
            func_8013FC58();
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80042908(0x4F);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013CCA8(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        if (g->unk_03E == 0x5F) {
            D_800CA148 = 0x27;
        } else {
            D_800CA148 = 0x28;
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x58AF);
        }
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8014338C();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->unk_F5F = 3;
            func_80083474();
            cd_read_sectors(0x19U, 0x801E8000, 0x4ABD);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0x6E;

                break;
            case 2:
                D_800CA148 = 0x6F;
                break;
            case 3:
                D_800CA148 = 0x70;
                break;
            case 4:
                D_800CA148 = 0x7A;
                break;
            }
            func_8013FC58();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        switch (func_8008667C(3)) {
        case 0:
        case 1:
            D_800CA148 = (g->unk_03E * 3) - 0xAC;
            break;
        case 2:
            D_800CA148 = (g->unk_03E * 3) - 0xAB;
            break;
        case 3:
            D_800CA148 = (g->unk_03E * 3) - 0xAA;
            break;
        case 4:
            func_80042940(0x11);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(3) == 4) {
            func_80042940(0x11);
        } else if (g->unk_03E == 0x61) {
            func_80042908(0x4F);
        } else {
            if (!(g->unk_1BC[3].unk_0A & 3)) {
                switch (func_8008667C(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    D_800CA148 = 0x68;
                    break;
                case 2:
                    D_800CA148 = 0x6A;
                    break;
                case 3:
                    D_800CA148 = 0x6C;
                    break;
                }
            } else {
                switch (func_8008667C(3)) {                /* switch 5; irregular */
                default:

                    break;
                case 0:
                case 1:
                    D_800CA148 = 0x7C;
                    break;
                case 2:
                    D_800CA148 = 0x7E;
                    break;
                case 3:
                    D_800CA148 = 0x80;
                    break;
                }
                if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                    D_800CA148 += 6;
                    g->unk_1BC[3].unk_0C.f.b2 = 1;
                }
                if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                    D_800CA148 += 0xC;
                }

                if (g->unk_03E == 0x60) {
                    D_800CA148 = D_800CA148 + 0x18;
                }
            }
            func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8013D1F8(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            g->unk_F5F = 3;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 1;

                break;
            case 2:
                D_800CA148 = 2;
                break;
            case 3:
                D_800CA148 = 3;
                break;
            case 4:
                D_800CA148 = 0xC;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(3) == 4) {
            func_80042940(0xE);
        } else if (!(g->unk_1BC[3].unk_0C.f.b2)) {
            func_80042940(0xC);
        } else {
            if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 4) {
                D_800CA148 = 4;
            } else {
                D_800CA148 = 5;
            }

            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        switch (func_8008667C(3)) {
        case 0:
        case 1:
            D_800CA148 = 6;
            break;
        case 2:
            D_800CA148 = 7;
            break;
        case 3:
            D_800CA148 = 8;
            break;
        }
        if (!(g->unk_1BC[3].unk_0C.f.b2)) {
            D_800CA148 += 3;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 16:
        func_800866A0(0x58AF);
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->unk_F5F = 3;
            func_80083474();
            cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        }
        break;
    case 18:
        if (func_8008667C(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = (g->unk_03E * 3) - 0x10F;
                break;
            case 2:
                D_800CA148 = (g->unk_03E * 3) - 0x10E;
                break;
            case 3:
                D_800CA148 = (g->unk_03E * 3) - 0x10D;
                break;
            case 4:
                D_800CA148 = 0xD;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (func_8008667C(3) == 4) {
            func_80042940(0x19);
        } else {
            if (!(g->unk_1BC[3].unk_0A & 3)) {
                switch (func_8008667C(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    if (g->unk_03E == 0x60) {
                        D_800CA148 = 0xB5;
                    } else {
                        D_800CA148 = 0x17;
                    }
                    break;
                case 2:
                    D_800CA148 = 0x19;
                    break;
                case 3:
                    D_800CA148 = 0x1B;
                    break;
                }
            } else {
                switch (func_8008667C(3)) {
                case 0:
                case 1:
                    D_800CA148 = 0x1D;
                    break;
                case 2:
                    D_800CA148 = 0x1F;
                    break;
                case 3:
                    D_800CA148 = 0x21;
                    break;
                case 4:
                    func_80042940(0x19);
                    break;
                }

                if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                    g->unk_1BC[3].unk_0C.f.b2 = 1;
                    D_800CA148 += 6;
                }
                if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                    D_800CA148 += 0xC;
                }
                D_800CA148 = (D_800CA148 + (g->unk_03E * 0x18)) - 0x8E8;
            }
            func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8013D8E0(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x2A;
            addr_init_bustup();
            g->unk_F5F = 3;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0x65;

                break;
            case 2:
                D_800CA148 = 0x66;
                break;
            case 3:
                D_800CA148 = 0x67;
                break;
            case 4:
                D_800CA148 = 0x68;
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
        if (func_8008667C(3) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 12:
        func_80044750(0x202);
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042808();
        break;
    case 13:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(3) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0x6A;

                break;
            case 2:
                D_800CA148 = 0x6D;
                break;
            case 3:
                D_800CA148 = 0x70;
                break;
            case 4:
                D_800CA148 = 0x73;
                break;
            }
            D_800CA148 += D_80122EA8;
            func_8013FCBC();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (func_8008667C(3) == 4) {
            func_80042940(0x15);
        } else {
            if (!(g->unk_1BC[3].unk_0A & 3)) {
                switch (func_8008667C(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    D_800CA148 = 0x17;
                    break;
                case 2:
                    D_800CA148 = 0x19;
                    break;
                case 3:
                    D_800CA148 = 0x1B;
                    break;
                }
            } else {
                switch (func_8008667C(3)) {                /* switch 5; irregular */
                default:

                    break;
                case 0:
                case 1:
                    D_800CA148 = 0x76;
                    break;
                case 2:
                    D_800CA148 = 0x78;
                    break;
                case 3:
                    D_800CA148 = 0x7A;
                    break;
                }
                if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                    g->unk_1BC[3].unk_0C.f.b2 = 1;
                    D_800CA148 += 6;
                }

                if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                    D_800CA148 = D_800CA148 + 0xC;
                }
            }
            func_8004284C();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
            if (func_8008667C(3) == 3) {
                D_800CA148 = 0x87;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


/* FAKE: scalar byte selector view preserves its target live range
 * (T-10630). The flag writes precede the saved-state copies so IDO emits
 * the original case-16 temporary order. */
void func_8013DE3C(void) {

    switch (*(u8 *)&D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x58AF);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 3;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2A;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 1;
                break;
            case 2:
                D_800CA148 = 2;
                break;
            case 3:
                D_800CA148 = 3;
                break;
            case 4:
                D_800CA148 = 0x90;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        func_8004284C();
        switch (func_8008667C(3)) {
        case 0:
        case 1:
            D_800CA148 = 0x8D;
            break;
        case 2:
            D_800CA148 = 0x8E;
            break;
        case 3:
            D_800CA148 = 0x8F;
            break;
        case 4:
            func_80042940(0xD);
            break;
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (func_8008667C(3) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(3) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 16:
        ((BunkaReturnFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkaReturnFlags *)&D_800E7D4F)->unk_2_4 = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 17:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 18:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(3U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 19:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 20:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 21:
        func_800866A0(0x58AF);
        break;
    case 22:
        addr_init_bustup();
        D_800E6280.unk_F5F = 3;
        func_80083474();
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        break;
    case 23:
        if (func_8008667C(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0x92;
                break;
            case 2:
                D_800CA148 = 0x94;
                break;
            case 3:
                D_800CA148 = 0x96;
                break;
            case 4:
                D_800CA148 = 0x98;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            func_8013FCBC();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_8004284C();
        if (!(D_800E6280.unk_1BC[3].unk_0A & 3)) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0x17;
                break;
            case 2:
                D_800CA148 = 0x19;
                break;
            case 3:
                D_800CA148 = 0x1B;
                break;
            case 4:
                func_80042940(0x1E);
                break;
            }
        } else {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0x9A;
                break;
            case 2:
                D_800CA148 = 0x9C;
                break;
            case 3:
                D_800CA148 = 0x9E;
                break;
            case 4:
                func_80042940(0x1E);
                break;
            }
            if (!D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_800CA148 += 6;
            }
            if (((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                D_800CA148 = D_800CA148 + 0xC;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        ((BunkaReturnFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8013E59C(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800CA148 = 0x29;
            addr_init_bustup();
            g->unk_F5F = 3;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 1;

                break;
            case 2:
                D_800CA148 = 2;
                break;
            case 3:
                D_800CA148 = 3;
                break;
            case 4:
                D_800CA148 = 0xC;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(3) == 4) {
            func_80042940(0xE);
        } else if (!(g->unk_1BC[3].unk_0C.f.b2)) {
            func_80042940(0xC);
        } else {
            if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 4) {
                D_800CA148 = 4;
            } else {
                D_800CA148 = 5;
            }

            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        switch (func_8008667C(3)) {
        case 0:
        case 1:
            D_800CA148 = 6;
            break;
        case 2:
            D_800CA148 = 7;
            break;
        case 3:
            D_800CA148 = 8;
            break;
        }
        if (!(g->unk_1BC[3].unk_0C.f.b2)) {
            D_800CA148 += 3;
        }
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        D_80122EAC = 0;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 16:
        func_800866A0(0x58AF);
        break;
    case 17:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 3;
        func_80083474();
        cd_read_sectors(0x17U, 0x801E8000, 0x4AD6);
        break;
    case 18:
        if (func_8008667C(3) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = 0xB2;

                break;
            case 2:
                D_800CA148 = 0xB3;
                break;
            case 3:
                D_800CA148 = 0xB4;
                break;
            case 4:
                D_800CA148 = 0xD;
                break;
            }
            func_8013FCBC();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (func_8008667C(3) == 4) {
            func_80042940(0x19);
        } else {
            if (!(g->unk_1BC[3].unk_0A & 3)) {
                switch (func_8008667C(3)) {                /* switch 4; irregular */
                case 0:
                case 1:
                    D_800CA148 = 0x17;
                    break;
                case 2:
                    D_800CA148 = 0x19;
                    break;
                case 3:
                    D_800CA148 = 0x1B;
                    break;
                }
            } else {
                switch (func_8008667C(3)) {                /* switch 5; irregular */
                case 0:
                case 1:
                    D_800CA148 = 0x4D;
                    break;
                case 2:
                    D_800CA148 = 0x4F;
                    break;
                case 3:
                D_800CA148 = 0x51;
                break;
                }
                if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                    g->unk_1BC[3].unk_0C.f.b2 = 1;
                    D_800CA148 += 6;
                }

                if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                    D_800CA148 = D_800CA148 + 0xC;
                }
            }
            func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 27:
        normal_date_speak();
        break;
    case 28:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8013EBDC(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            g->unk_F5F = 3;
            func_80083474();
            D_800CA148 = 0x2B;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 1;
                break;
            case 2:
                D_800CA148 = 4;
                break;
            case 3:
                D_800CA148 = 7;
                break;
            case 4:
                D_800CA148 = 0xA;
                break;
            }
            func_8013FD20();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (func_8008667C(3) == 4) {
            func_80042940(0xE);
        } else {
            if ((u16) g->unk_66C[4].unk_00 < 0x64U) {
                D_800CA148 += 1;
            }
            func_8004284C();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        if (!(g->unk_1BC[3].unk_0A & 3)) {
            temp_v0_2 = func_8008667C(3);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x18;
                break;
            case 2:
                D_800CA148 = 0x1A;
                break;
            case 3:
                D_800CA148 = 0x1C;
                break;
            }
        } else {
            temp_v0_3 = func_8008667C(3);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0xC;
                break;
            case 2:
                D_800CA148 = 0xE;
                break;
            case 3:
                D_800CA148 = 0x10;
                break;
            }
            if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                D_800CA148 += 6;
                g->unk_1BC[3].unk_0C.f.b2 = 1;
            }
        }
        func_8004284C();
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        func_8007E5A0();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_8007E934();
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013EFB8(void) {
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E6280.unk_F5F = 3;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x1E;
                break;
            case 2:
                D_800CA148 = 0x1F;
                break;
            case 3:
                D_800CA148 = 0x20;
                break;
            case 4:
                D_800CA148 = 0x22;
                break;
            }
            func_8013FD20();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        func_8007E5A0();
        break;
    case 11:
        if (func_8008667C(3) != 4) {
            D_800CA148 = 0x21;
        }
        func_8004284C();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_8007E934();
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        func_80085A60();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013F234(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x58AF);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            g->unk_F5F = 3;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        func_8004284C();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x24;
                break;
            case 2:
                D_800CA148 = 0x25;
                break;
            case 3:
                D_800CA148 = 0x26;
                break;
            case 4:
                D_800CA148 = 0x27;
                break;
            }
            func_8013FD20();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        if (func_8008667C(3) == 4) {
            func_80042940(0xC);
        } else if (!(g->unk_1BC[3].unk_0A & 3)) {
            temp_v0_2 = func_8008667C(3);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x18;
                break;
            case 2:
                D_800CA148 = 0x1A;
                break;
            case 3:
                D_800CA148 = 0x1C;
                break;
            }
            func_8004284C();
        } else {
            temp_v0_3 = func_8008667C(3);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x29;
                break;
            case 2:
                D_800CA148 = 0x2B;
                break;
            case 3:
                D_800CA148 = 0x2D;
                break;
            }
            if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                D_800CA148 += 6;
                g->unk_1BC[3].unk_0C.f.b2 = 1;
            }
            func_8004284C();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        func_8007E5A0();
        break;
    case 13:
        func_80085A60();
        break;
    case 14:
        func_8007E934();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        func_80085A60();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013F5D4(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (g->unk_03E == 0x5F) {
            D_800CA148 = 0x2E;
        } else {
            D_800CA148 = 0x2F;
        }

        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x58AF);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 3;
        func_80083474();
        cd_read_sectors(0xFU, 0x801E8000, 0x4AED);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            switch (func_8008667C(3)) {
            case 0:
            case 1:
                D_800CA148 = (g->unk_03E * 3) - 0xE8;
                break;
            case 2:
                D_800CA148 = (g->unk_03E * 3) - 0xE7;
                break;
            case 3:
                D_800CA148 = (g->unk_03E * 3) - 0xE6;
                break;
            case 4:
                D_800CA148 = 0x3E;
                break;
            }
            func_8013FD20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (func_8008667C(3) == 4) {
            func_80042940(0xF);
        } else if (!(g->unk_1BC[3].unk_0A & 3)) {
            switch (func_8008667C(3)) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x18;
                break;
            case 2:
                D_800CA148 = 0x1A;
                break;
            case 3:
                D_800CA148 = 0x1C;
                break;
            }
            func_8004284C();
        } else {
            switch (func_8008667C(3)) {                    /* switch 4; irregular */

            case 0:
            case 1:
                D_800CA148 = 0x40;
                break;
            case 2:
                D_800CA148 = 0x42;
                break;
            case 3:
                D_800CA148 = 0x44;
                break;
            }
            if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                D_800CA148 += 6;
                g->unk_1BC[3].unk_0C.f.b2 = 1;
            }
            D_800CA148 = (D_800CA148 + (g->unk_03E * 0xC)) - 0x474;
            func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8013FA20(void) {
}

void func_8013FA28(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        if (func_8008667C(3) == 4) {
            func_80042940(2);
            return;
        }
        temp_v0 = func_8008667C(3);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = 0x50;
            break;
        case 2:
            D_800CA148 = 0x52;
            break;
        case 3:
            D_800CA148 = 0x54;
            break;
        }
        if (!(g->unk_1BC[3].unk_0A & 3)) {
            D_800CA148 = D_800CA148 + 0x18;
        } else {
            if (!(g->unk_1BC[3].unk_0C.f.b2)) {
                D_800CA148 += 6;
                g->unk_1BC[3].unk_0C.f.b2 = 1;
            }

            if (((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) {
                D_800CA148 = D_800CA148 + 0xC;
            }
        }
        func_8004284C();
        return;
    case 1:
        func_80085A60();
        return;
    case 2:
        func_8007E5A0();
        return;
    case 3:
        func_80085A60();
        return;
    case 4:
        func_8007E934();
        return;
    case 5:
        D_800CA148 = 0;
        func_8004284C();
        return;
    case 6:
        func_80085A60();
        return;
    case 7:
        if ((((u16) g->unk_0F4.h >> 0xC) == 0) && (func_8008667C(3) != 4)) {
            if (g->unk_03E == 0x5F) {
                g->unk_1BC[3].unk_0C.f.b12 = 1;
            }
            if (g->unk_03E == 0x60) {
                g->unk_1BC[3].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013FC58(void) {
    func_800847B8(3);
    func_8013216C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

void func_8013FCBC(void) {
    func_800847B8(3);
    func_801321A0();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

void func_8013FD20(void) {
    func_800847B8(3);
    func_801321D4();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
