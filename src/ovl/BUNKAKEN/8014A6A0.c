#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_8014A6A0(void) {
    ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0;
    switch (((BunkaDispatch8 *)&D_80122EB8)->unk_00) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            if ((func_8008667C(0) == 4) && !(D_800E6280.unk_1BC[0].unk_0C.f.b2)) {
                ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x21);
            return;
        case 0x60:
            if ((func_8008667C(0) == 4) && !(D_800E6280.unk_1BC[0].unk_0C.f.b2)) {
                ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x21);
            return;
        case 0x61:
            func_80042908(0x22);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x23);
        return;
    case 2:
        if ((func_8008667C(0) == 4) && !(D_800E6280.unk_1BC[0].unk_0C.f.b2)) {
            ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0xFF;
            func_80042908(5);
            return;
        }
        func_80042908(0x24);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x25);
            break;
        case 0x60:
            func_80042908(0x26);
            break;
        case 0x61:
            func_80042908(0x27);
            break;
        }
        if ((func_8008667C(0) == 4) && !(D_800E6280.unk_1BC[0].unk_0C.f.b2)) {
            ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0xFF;
            func_80042908(5);
            return;
        }
        return;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            if ((func_8008667C(0) == 4) && !(D_800E6280.unk_1BC[0].unk_0C.f.b2)) {
                ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x28);
            return;
        case 0x60:
            func_80042908(0x29);
            return;
        case 0x61:
            if ((func_8008667C(0) == 4) && !(D_800E6280.unk_1BC[0].unk_0C.f.b2)) {
                ((BunkaDispatch8 *)&D_80122EB8)->unk_04 = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x2A);
            return;
        }
        break;
    case 5:
        func_80042908(0x2B);
        return;
    default:
        func_80046500();
        break;
    }
}

void func_8014A9A8(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            if ((D_80122EA4 == 0) || (func_8008667C(0) == 4)) {
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xD;
                } else {
                    D_800CA148 = 0x16;
                }
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (!(g->unk_1BC[0].unk_0C.f.b2) && (func_8008667C(0) != 4)) {
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        } else {
            func_80042940(0xA);
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((D_80122EA4 == 0) || (func_8008667C(0) == 4)) {
            func_80042940(0xC);
        } else {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 7;
                } else {
                    D_800CA148 = 0x10;
                }
                break;
            case 2:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 9;
                } else {
                    D_800CA148 = 0x12;
                }
                break;
            case 3:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xB;
                } else {
                    D_800CA148 = 0x14;
                }
                break;
            }
            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            D_800CA148 = 0xE;
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 8;
                } else {
                    D_800CA148 = 0x11;
                }
                break;
            case 2:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xA;
                } else {
                    D_800CA148 = 0x13;
                }
                break;
            case 3:
                if (g->unk_03E == 0x5F) {
                    D_800CA148 = 0xC;
                } else {
                    D_800CA148 = 0x15;
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
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
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
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014AF48(void) {
    GameState *g = &D_800E6280;
    u32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            func_80046318(0x17U, 0x801E8000, 0x49EB);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (func_800460CC() & 1) {
            if (g->unk_1BC[0].unk_0C.f.b2) {
                D_800CA148 = 0x19;
            } else {
                D_800CA148 = 0x1A;
            }
            func_8014E3DC();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E934();
        break;
    case 7:
        D_80122EAC = 2;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_80145A30();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        func_8004284C();
        break;
    case 9:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            if (g->unk_1BC[0].unk_0C.f.b2) {
                func_80042940(0xB);
            } else {
                g->unk_1BC[0].unk_0C.f.b2 = 1;
                D_800CA148 = 4;
            }
            func_8005742C(0x40F4, 1);
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_800460CC() & 1) {
            func_800866A0(0x544A);
        }
        break;
    case 14:
        if (func_800460CC() & 1) {
            func_80057418(1, func_8005751C(1));
            func_801459FC();
            func_8007C740();
            g->unk_F5F = 0;
            func_80083474();
        }
        break;
    case 15:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 16:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = 0x1B;
            break;
        case 2:
            D_800CA148 = 0x1C;
            break;
        case 3:
            D_800CA148 = 0x1D;
            break;
        case 4:
            D_800CA148 = 0x1E;
            break;
        }
        if (D_80122EA4 == 0) {
            D_800CA148 = 0x1E;
        }
        func_8004284C();
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 19:
        func_8007E934();
        break;
    case 20:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 21:
        func_80085A60();
        break;
    case 22:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014B378(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80046318(0x17U, 0x801E8000, 0x49EB);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            D_800CA148 = 0x1F;
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
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_80145A30();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        func_8004284C();
        break;
    case 9:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            if (g->unk_1BC[0].unk_0C.f.b2) {
                func_80042940(0xB);
            } else {
                D_800CA148 = 5;
                g->unk_1BC[0].unk_0C.f.b2 = 1;
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
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (func_800460CC() & 1) {
            func_800866A0(0x5477);
        }
        break;
    case 14:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_801459FC();
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
        }
        break;
    case 15:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        switch (func_8008667C(0)) {
        case 0:
        case 1:
            D_800CA148 = 0x20;

            break;
        case 2:
            D_800CA148 = 0x21;
            break;
        case 3:
            D_800CA148 = 0x22;
            break;
        case 4:
            D_800CA148 = 0x23;
            break;
        }

        if (D_80122EA4 == 0) {
            D_800CA148 = 0x23;
        }
        D_800CA148 = (D_800CA148 + (g->unk_03E * 4)) - 0x17C;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 19:
        normal_date_bg_fadeout();
        break;
    case 20:
        g->unk_1BC[0].unk_0C.f.b2 = 1;
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014B7B8(void) {
    GameState *g = &D_800E6280;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x54D1);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
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
                D_800CA148 = 0x32;
                break;
            }
            if (D_80122EA4 == 0) {
                D_800CA148 = 0x32;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 4;
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_8004284C();
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x2C;
            break;
        case 2:
            D_800CA148 = 0x2E;
            break;
        case 3:
            D_800CA148 = 0x30;
            break;
        case 4:
            func_80042940(0xC);
            break;
        }
        if (D_80122EA4 == 0) {
            func_80042940(0xC);
        }
        D_800CA148 = (D_800CA148 + (g->unk_03E * 8)) - 0x2F8;
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 2;
        }
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 14:
        func_800866A0(0x54D1);
        break;
    case 15:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 0;
        func_80083474();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        break;
    case 16:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (func_800460CC() & 1) {
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x2D;
                break;
            case 2:
                D_800CA148 = 0x2F;
                break;
            case 3:
                D_800CA148 = 0x31;
                break;
            case 4:
                D_800CA148 = 0x33;
                break;
            }
            if (D_80122EA4 == 0) {
                D_800CA148 = 0x33;
            }
            D_800CA148 = (D_800CA148 + (g->unk_03E * 8)) - 0x2F8;
            func_8014E3DC();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014BD80(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    u32 temp_v0;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
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
                D_800CA148 = 0x56;
                break;
            }
            if (D_80122EA4 == 0) {
                D_800CA148 = 0x56;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x44;
            break;
        case 2:
            D_800CA148 = 0x4A;
            break;
        case 3:
            D_800CA148 = 0x50;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_80042940(0xD);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        func_80044750(0x202);
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0xB);
        break;
    case 14:
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        func_8004284C();
        break;
    case 15:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x45;
                break;
            case 2:
                D_800CA148 = 0x4B;
                break;
            case 3:
                D_800CA148 = 0x51;
                break;
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x57;
                func_80042940(0x12);
            }
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        D_800CA148 += D_80122EA8;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        temp_v0_4 = func_8008667C(0);
        switch (temp_v0_4) {                        /* switch 5; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x49;
            break;
        case 2:
            D_800CA148 = 0x4F;
            break;
        case 3:
            D_800CA148 = 0x55;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_80042940(0x16);
        } else {
            func_8004284C();
        }
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 23:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
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
        func_80046500();
        break;
    }
}


void func_8014C374(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;

    switch (g->unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (func_800460CC() & 1) {
            func_8007C740();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            func_8007E390();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
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
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x69;
            }
            func_8014E3DC();
        }
        break;
    case 7:
        func_80085A60();
        break;
    case 8:
        if (g->unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            D_800CA148 = 4;
            func_8004284C();
        }
        break;
    case 9:
        func_80085A60();
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x5A;
            break;
        case 2:
            D_800CA148 = 0x5F;
            break;
        case 3:
            D_800CA148 = 0x64;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_80042940(0xC);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        func_80085A60();
        break;
    case 12:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 13:
        ((BunkakenModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkakenModeFlags *)&D_800E7D4F)->mode = 0;
        g->unk_720 = g->unk_1108;
        g->unk_721 = g->unk_1109;
        g->unk_722 = g->unk_110A + 1;
        func_80042908(0x8D);
        break;
    case 14:
        D_80122E98 = 1;
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042908(0x8C);
        break;
    case 15:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 16:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 17:
        if (func_800460CC() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 18:
        func_800866A0(0x544A);
        break;
    case 19:
        func_8007C740();
        g->unk_F5F = 0;
        func_80083474();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        break;
    case 20:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 21:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x5B;
                break;
            case 2:
                D_800CA148 = 0x60;
                break;
            case 3:
                D_800CA148 = 0x65;
                break;
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x6A;
            }
        }
        break;
    case 22:
        func_80085A60();
        break;
    case 23:
        func_8004284C();
        temp_v0_4 = func_8008667C(0);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x5C;
            break;
        case 2:
            D_800CA148 = 0x61;
            break;
        case 3:
            D_800CA148 = 0x66;
            break;
        }
        if ((u8) D_800E7D4E < 2U) {
            D_800CA148 += 1;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_80042940(0x1B);
        }
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        temp_v0_5 = func_8008667C(0);
        switch (temp_v0_5) {
        case 0:
        case 1:
            D_800CA148 = 0x5E;
            break;
        case 2:
            D_800CA148 = 0x63;
            break;
        case 3:
            D_800CA148 = 0x68;
            break;
        }
        func_8004284C();
        break;
    case 26:
        func_80085A60();
        break;
    case 27:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8007E934();
        } else {
            func_8007E99C();
        }
        break;
    case 28:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 29:
        func_80085A60();
        break;
    case 30:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014CB4C(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                func_80042940(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (g->unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 4;
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x6B;
            break;
        case 2:
            D_800CA148 = 0x6C;
            break;
        case 3:
            D_800CA148 = 0x6D;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            D_800CA148 = 0x71;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            D_80122EAC = 1;
        } else {
            D_80122EAC = 2;
        }
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 14:
        func_80062CD0(0x544A);
        func_8004284C();
        break;
    case 15:
        if (func_800460CC() & 1) {
            func_80145844();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 16:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 17:
        if (func_800460CC() & 1) {
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
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
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x72;
            }
            func_8014E3DC();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014D0FC(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5828);
        break;
    case 4:
        func_800AE0A0((void *)0x80197000, 0x80180000, 0x16800);
        func_8004284C();
        break;
    case 5:
        func_800866A0(0x54A4);
        break;
    case 6:
        func_800AE0A0((void *)0x801AE000, 0x80180000, 0x16800);
        addr_init_bustup();
        g->unk_F5F = 0;
        func_80083474();
        func_80145844();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        break;
    case 7:
        normal_date_bg_fadein();
        break;
    case 8:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 9:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                D_800CA148 = 0x85;
                func_80042940(0x1E);
            } else {
                temp_v0 = func_8008667C(0);
                switch (temp_v0) {                  /* switch 2; irregular */
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
                }
            }
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (g->unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xD);
        } else {
            D_800CA148 = 4;
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x73;
            break;
        case 2:
            D_800CA148 = 0x79;
            break;
        case 3:
            D_800CA148 = 0x7F;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_800847B8(0xCU);
        g->unk_F5F = 0xC;
        D_80122D20 = 1;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        func_800AE0A0((void *)0x80180000, 0x80197000, 0x16800);
        func_8004284C();
        break;
    case 19:
        D_80122D20 = 0;
        addr_init_bustup();
        func_80083474();
        func_80083440(0U);
        break;
    case 20:
        normal_date_girl_in();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_800847B8(0U);
        g->unk_F5F = 0;
        D_80122D20 = 1;
        func_8004284C();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        func_800847B8(0xCU);
        g->unk_F5F = 0xC;
        D_80122D20 = 0;
        func_8004284C();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_girl_out();
        break;
    case 27:
        func_800AE0A0((void *)0x80180000, 0x801AE000, 0x16800);
        func_8004284C();
        break;
    case 28:
        addr_init_bustup();
        g->unk_F5F = 0;
        func_80083474();
        func_800847B8(0U);
        break;
    case 29:
        normal_date_girl_in();
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 32:
        normal_date_bg_fadeout();
        break;
    case 33:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014D65C(void) {
    GameState *g = &D_800E6280;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80046318(0x17U, 0x801E8000, 0x49EB);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            if (func_8008667C(0) == 4) {
                func_80062CD0(0x6C5F);
            } else {
                func_80062CD0(0x54A4);
            }
        }
        break;
    case 5:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(0);
            /* FAKE: the empty pointer test keeps the GameState base in v1
             * in later cases; the test itself is eliminated. T-10630. */
            if (!g) {}
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x86;
                break;
            case 2:
                D_800CA148 = 0x87;
                break;
            case 3:
                D_800CA148 = 0x88;
                break;
            }
            addr_init_bustup();
            if (func_8008667C(0) == 4) {
                D_800CA148 = 0x89;
                g->unk_F5F = 0xD;
                func_800847B8(0xDU);
            } else {
                g->unk_F5F = 0;
            }
            func_80083474();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_8008667C(0) == 4) {
            D_800CA148 = 0x8C;
        } else if (g->unk_1BC[0].unk_0C.f.b2) {
            D_800CA148 = 0x8A;
        } else {
            g->unk_1BC[0].unk_0C.f.b2 = 1;
            D_800CA148 = 0x8B;
        }
        func_8004284C();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(0) == 4) {
            func_80042940(0x14);
        } else {
            func_8004284C();
        }
        break;
    case 11:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
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
        }
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        D_800CA148 = 0x90;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        D_800CA148 = 0;
        g->unk_1BC[0].unk_0C.f.b2 = 1;
        func_8004284C();
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
            D_800CA148 = 0x94;
        } else {
            D_800CA148 = 0x91;
        }
        func_8004284C();
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        normal_date_girl_out();
        break;
    case 24:
        if (D_80122CDC != 0) {
            func_80042940(0x10);
        } else {
            func_800847B8(0U);
            if (g->unk_1BC[0].unk_0C.f.b2) {
                D_800CA148 = 0x92;
            } else {
                D_800CA148 = 0x93;
            }
            func_8004284C();
        }
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        normal_date_bg_fadeout();
        break;
    case 27:
        func_80042940(0x11);
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014DB34(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x54A4);
            func_8004284C();
        }
        break;
    case 3:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            func_80046318(0x17U, 0x801E8000, 0x49EB);
        }
        break;
    case 4:
        func_80145844();
        func_8004284C();
        break;
    case 5:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 6:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {                      /* switch 2; irregular */
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
            }
            if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
                func_80042940(0xA);
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if (D_800E6280.unk_1BC[0].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 4;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x95;
            break;
        case 2:
            D_800CA148 = 0x97;
            break;
        case 3:
            D_800CA148 = 0x99;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            D_800CA148 = 0x9B;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadeout();
        } else {
            D_800CA148 = 0x9C;
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            normal_date_bg_fadein();
        } else {
            temp_v0_3 = func_8008667C(0);
            switch (temp_v0_3) {                    /* switch 4; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x96;
                break;
            case 2:
                D_800CA148 = 0x98;
                break;
            case 3:
                D_800CA148 = 0x9A;
                break;
            }
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
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
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014DFB8(void) {
    s32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_80145844();
            func_80057418(1, func_8005751C(1));
            func_80046318(0x17U, 0x801E8000, 0x49EB);
            func_8004284C();
        }
        break;
    case 3:
        func_8007E81C();
        break;
    case 4:
        if (func_800460CC() & 1) {
            D_800CA148 = 0x9E;
            func_8014E3DC();
        }
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8007E934();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_80145A30();
        func_80046318(0x17U, 0x801E8000, 0x49EB);
        func_8004284C();
        break;
    case 9:
        if (func_800460CC() & 1) {
            func_8014E3DC();
            if (D_800E6280.unk_1BC[0].unk_0C.f.b2) {
                func_80042940(0xB);
            } else {
                D_800CA148 = 5;
            }
            func_80062CD0(0x544A);
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        D_800CA148 = 6;
        func_8004284C();
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        if (func_800460CC() & 1) {
            func_8007C740();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            func_8005742C(0x40F4, 1);
        }
        break;
    case 14:
        if (func_800460CC() & 1) {
            func_801459FC();
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 15:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 16:
        temp_v0 = func_8008667C(0);
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0:
        case 1:
            D_800CA148 = 0x9F;
            break;
        case 2:
            D_800CA148 = 0xA0;
            break;
        case 3:
            D_800CA148 = 0xA1;
            break;
        }
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            D_800CA148 = 0xA2;
        }
        D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 4)) - 0x17C;
        func_8004284C();
        break;
    case 17:
        func_80085A60();
        break;
    case 18:
        if ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 19:
        func_8007E934();
        break;
    case 20:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 21:
        func_80085A60();
        break;
    case 22:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8014E3DC(void) {
    func_800847B8(0);
    func_80132034();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
