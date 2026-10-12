#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80158EB0(void) {
    ((BunkSelectWords *)&D_80122EB8)->unk_04 = 9;
    switch (((BunkSelectWords *)&D_80122EB8)->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x92);
            return;
        }
        func_80042908(0x93);
        return;
    case 1:
        func_80042908(0x94);
        return;
    case 2:
        func_80042908(0x95);
        return;
    case 3:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x96);
            return;
        }
        func_80042908(0x95);
        return;
    case 4:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x97);
            return;
        }
        func_80042908(0x98);
        return;
    case 5:
        func_80042908(0x99);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80158FD4(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x64D0);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 9;
            func_80083474();
            D_800CA148 = 0x25;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xC;
                break;
            case 2:
                D_800CA148 = 0xD;
                break;
            case 3:
                D_800CA148 = 0xE;
                break;
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(9) == 4) {
            func_80042940(0xC);
        } else {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 1;
                break;
            case 2:
                D_800CA148 = 3;
                break;
            case 3:
                D_800CA148 = 4;
                break;
            }
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
        if ((func_8008667C(9) < 2) || (func_8008667C(9) == 4)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (func_8008667C(9) != 4) {
            if (D_800E6280.unk_03E == 0x5F) {
                D_800E6280.unk_1BC[9].unk_0C.f.b12 = 1;
            } else {
                D_800E6280.unk_1BC[9].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80159344(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        D_800CA148 = 0x26;
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_800866A0(0x64D0);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
        func_80083474();
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xF;
                break;
            case 2:
                D_800CA148 = 0x10;
                break;
            case 3:
                D_800CA148 = 0x11;
                break;
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        temp_v0_2 = func_8008667C(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 3;
            break;
        case 3:
            D_800CA148 = 4;
            break;
        case 4:
            func_80042940(0xF);
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
        if ((func_8008667C(9) < 2) || (func_8008667C(9) == 4)) {
            normal_date_speak();
        } else {
            func_8004284C();
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80159698(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        D_800CA148 = 0x28;
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        normal_date_bg_fadeout();
        break;
    case 6:
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 8:
        if (func_800460CC() & 1) {
            func_800866A0(0x64D0);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
        func_80083474();
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x12;
                } else {
                    D_800CA148 = 0x15;
                }
                break;
            case 2:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x13;
                } else {
                    D_800CA148 = 0x16;
                }
                break;
            case 3:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x14;
                } else {
                    D_800CA148 = 0x17;
                }
                break;
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        temp_v0_2 = func_8008667C(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            func_80083440(0U);
            func_80063668();
            D_800CA148 = 7;
            break;
        case 2:
            D_800CA148 = 3;
            break;
        case 3:
            D_800CA148 = 4;
            break;
        case 4:
            func_80042940(0x10);
            break;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((func_8008667C(9) < 2) && (D_800E6280.unk_03E == 0x60)) {
            D_800CA148 += 1;
        }
        func_8004284C();
        /* fallthrough */
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        if ((func_8008667C(9) == 4) || (func_8008667C(9) < 2)) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 18:
        normal_date_bg_fadeout();
        break;
    case 19:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80159AD4(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 2) {
            dec_bg_cd_read(0x40FC, 1);
        } else {
            dec_bg_cd_read(0x4105, 1);
        }
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x64D0);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 9;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x18;
                break;
            case 2:
                D_800CA148 = 0x19;
                break;
            case 3:
                D_800CA148 = 0x1A;
                break;
            case 4:
                D_800CA148 = 0x1B;
                break;
            }
            func_8015AF20();
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 12:
        func_800866A0(0x64D0);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
        func_80083474();
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        break;
    case 14:
        if (func_8008667C(9) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 15:
        if (func_800460CC() & 1) {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x1C;
                } else {
                    D_800CA148 = 0x20;
                }
                break;
            case 2:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x1D;
                } else {
                    D_800CA148 = 0x21;
                }
                break;
            case 3:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x1E;
                } else {
                    D_800CA148 = 0x22;
                }
                break;
            case 4:
                D_800CA148 = 0x1F;
                break;
            }
            func_8015AF20();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(9);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 3;
            break;
        case 3:
            D_800CA148 = 4;
            break;
        case 4:
            func_80042940(0x15);
            break;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        if (func_8008667C(9) < 2) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
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

void func_80159FBC(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
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
        func_800866A0(0x64D0);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
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
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x23;
                break;
            case 2:
                D_800CA148 = 0x24;
                break;
            case 3:
                D_800CA148 = 0x25;
                break;
            case 4:
                D_800CA148 = 0x26;
                break;
            }
            func_8015AF20();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (func_8008667C(9) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(9) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 15:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 16:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 17:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(9U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 18:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 19:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 20:
        func_800866A0(0x64D0);
        break;
    case 21:
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
        func_80083474();
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        break;
    case 22:
        if (func_8008667C(9) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 23:
        if (func_800460CC() & 1) {
            temp_v0_2 = func_8008667C(9);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x28;
                break;
            case 2:
                D_800CA148 = 0x29;
                break;
            case 3:
                D_800CA148 = 0x2A;
                break;
            case 4:
                D_800CA148 = 0x2B;
                break;
            }
            func_8015AF20();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        func_8004284C();
        temp_v0_3 = func_8008667C(9);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 3;
            break;
        case 3:
            D_800CA148 = 4;
            break;
        case 4:
            func_80042940(0x1D);
            break;
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        normal_date_girl_out();
        break;
    case 28:
        if (func_8008667C(9) < 2) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 29:
        normal_date_bg_fadeout();
        break;
    case 30:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015A5DC(void) {
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x64D0);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 9;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x2D;
                break;
            case 2:
                D_800CA148 = 0x30;
                break;
            case 3:
                D_800CA148 = 0x33;
                break;
            case 4:
                D_800CA148 = 0x36;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(9) != 4) {
            func_80083440(0U);
            func_80063668();
        }
        func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015A870(void) {
    s32 temp_v0_2;
    u32 temp_v0;

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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x64D0);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 9;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x39;
                break;
            case 2:
                D_800CA148 = 0x3B;
                break;
            case 3:
                D_800CA148 = 0x3C;
                break;
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        func_8004284C();
        temp_v0_2 = func_8008667C(9);
        switch (temp_v0_2) {
        case 2:
            D_800CA148 = 3;
            break;
        case 3:
            D_800CA148 = 4;
            break;
        case 4:
            func_80042940(0xC);
            break;
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        if (func_8008667C(9) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 14:
        normal_date_bg_fadeout();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015AB58(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_8014338C();
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        D_800CA148 = 0x2F;
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_800866A0(0x64D0);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 9;
        func_80083474();
        func_80046318(0xAU, 0x801E8000, 0x4B33);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(9);
            switch (temp_v0) {
            case 0:
            case 1:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x3D;
                } else {
                    D_800CA148 = 0x40;
                }
                break;
            case 2:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x3E;
                } else {
                    D_800CA148 = 0x41;
                }
                break;
            case 3:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x3F;
                } else {
                    D_800CA148 = 0x42;
                }
                break;
            case 4:
                D_800CA148 = 5;
                break;
            }
            func_8015AF20();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        temp_v0_2 = func_8008667C(9);
        switch (temp_v0_2) {
        case 0:
        case 1:
            func_80083440(0U);
            func_80063668();
            D_800CA148 = 7;
            break;
        case 2:
            D_800CA148 = 3;
            break;
        case 3:
            D_800CA148 = 4;
            break;
        case 4:
            func_80042940(0xF);
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
        if ((func_8008667C(9) == 4) || (func_8008667C(9) < 2)) {
            normal_date_speak();
        } else {
            func_8004284C();
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8015AF20(void) {
    func_800847B8(9);
    func_80132340();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
