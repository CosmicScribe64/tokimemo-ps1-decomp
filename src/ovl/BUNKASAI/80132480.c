#include "common.h"
#include "ovl/BUNKASAI.h"

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80132480", func_80132480);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80132480", func_801325F4);

void func_801328D4(void) {
    u32 temp_v0;

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
        D_80122EB0 = (s32) g->unk_1109;
        D_80122EB4 = g->unk_110A + 1;
        func_80042878(0x80);
        break;
    case 8:
        func_800866A0(0x522E);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 2;
        func_80083474();
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 10:
        if (func_8008667C(2) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 11:
        if (func_800460CC() & 1) {
            func_80135448();
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
                func_80042940(0x11);
                break;
            case 4:
                D_800CA148 = 0x1E;
                func_80042940(0x1E);
                break;
            }
        }
        break;
    case 12:
        if (func_8008667C(2) < 2) {
            D_800CA148 = 0x11;
        } else {
            D_800CA148 = 0x13;
        }
        func_8004284C();
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
        func_80042940(0x23);
        break;
    case 17:
        D_800CA148 = 0x15;
        func_8004284C();
        /* fallthrough */
    case 18:
        normal_date_speak();
        break;
    case 19:
        if (!(g->unk_1BC[2].unk_0A & 3)) {
            D_800CA148 = 0x21;
            func_80042940(0x1A);
        } else {
            if (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                D_800CA148 = 0x16;
            } else {
                D_800CA148 = 0x17;
            }
            func_8004284C();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (g->unk_1BC[2].unk_0C.f.b2) {
            D_800CA148 = 0x1A;
            func_80042940(0x18);
        } else {
            if (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                D_800CA148 = 0x18;
            } else {
                D_800CA148 = 0x19;
            }
            g->unk_1BC[2].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 22:
        normal_date_speak();
        break;
    case 23:
        D_800CA148 = 0x1B;
        func_8004284C();
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        D_800CA148 = 0x1C;
        func_8004284C();
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
        func_80042940(0x23);
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 37:
        normal_date_speak();
        break;
    case 38:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80132DB4(void) {
    s32 temp_v0;

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
        if (func_800460CC() & 1) {
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
        func_80042878(0x80);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        func_8004284C();
        break;
    case 8:
        func_800866A0(0x522E);
        break;
    case 9:
        addr_init_bustup();
        g->unk_F5F = 2;
        func_80083474();
        dec_bg_cd_read(0x40F4, 1);
        break;
    case 10:
        if (func_800460CC() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8014338C();
            D_80125CA0 = 0xC0;
            func_80046318(0x1AU, 0x801E8000, 0x4A12);
            func_8004284C();
        }
        break;
    case 11:
        if (func_800460CC() & 1) {
            func_80135448();
            if (func_8008667C(2) == 4) {
                D_800CA148 = 0x29;
                func_80042940(0x15);
            }
        }
        break;
    case 12:
        func_8007E884();
        break;
    case 13:
        func_8004284C();
        temp_v0 = func_8008667C(2);
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0:
        case 1:
            switch (g->unk_03E) {                   /* switch 3; irregular */
            case 0x5F:
                D_800CA148 = 0x23;
                break;
            case 0x60:
                D_800CA148 = 0x2C;
                break;
            case 0x61:
                D_800CA148 = 0x32;
                break;
            }
            break;
        case 2:
            switch (g->unk_03E) {                   /* switch 4; irregular */
            case 0x5F:
                D_800CA148 = 0x25;
                break;
            case 0x60:
                D_800CA148 = 0x2E;
                break;
            case 0x61:
                D_800CA148 = 0x34;
                break;
            }
            break;
        case 3:
            switch (g->unk_03E) {                   /* switch 5; irregular */
            case 0x5F:
                D_800CA148 = 0x27;
                break;
            case 0x60:
                D_800CA148 = 0x30;
                break;
            case 0x61:
                D_800CA148 = 0x36;
                func_80042940(0x1C);
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
        D_800CA148 = 0;
        func_8004284C();
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
        func_80042940(0x11);
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        if (!(g->unk_1BC[2].unk_0A & 3)) {
            D_800CA148 = 0x3F;
            func_80042940(0x24);
        } else {
            if (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                D_800CA148 = 0x37;
            } else {
                D_800CA148 = 0x38;
            }
            func_8004284C();
        }
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        if (g->unk_1BC[2].unk_0C.f.b2) {
            D_800CA148 = 0x3B;
            func_80042940(0x22);
        } else {
            if (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                D_800CA148 = 0x39;
            } else {
                D_800CA148 = 0x3A;
            }
            g->unk_1BC[2].unk_0C.f.b2 = 1;
            func_8004284C();
        }
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        D_800CA148 = 0x3C;
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        D_800CA148 = 0x3D;
        func_8004284C();
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
        func_80042940(0x11);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801333CC(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (func_800460CC() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 2;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x41;
                break;
            case 2:
                D_800CA148 = 0x42;
                break;
            case 3:
                D_800CA148 = 0x43;
                break;
            case 4:
                D_800CA148 = 0x44;
                break;
            }
            if (D_800E6280.unk_03E == 0x61) {
                D_800CA148 += 4;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if ((D_800E6280.unk_1BC[2].unk_0C.f.b2) && (D_800E6280.unk_03E != 0x61)) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x49;
                break;
            case 2:
                D_800CA148 = 0x4A;
                break;
            case 3:
                D_800CA148 = 0x4B;
                break;
            case 4:
                D_800CA148 = 0x4C;
                break;
            }
            func_8004284C();
        } else {
            func_80042940(0xC);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if (D_800E6280.unk_03E == 0x61) {
            temp_v0_3 = func_8008667C(2);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x4F;
                break;
            case 2:
                D_800CA148 = 0x50;
                break;
            case 3:
                D_800CA148 = 0x51;
                break;
            case 4:
                D_800CA148 = 0x52;
                break;
            }
        } else if (func_8008667C(2) == 4) {
            D_800CA148 = 0x4E;
        } else {
            D_800CA148 = 0x4D;
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 16:
        func_800866A0(0x522E);
        break;
    case 17:
        if (func_800460CC() & 1) {
            func_801431D4();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            D_800E6280.unk_F5F = 2;
            func_80083474();
            func_80046318(0x1AU, 0x801E8000, 0x4A12);
        }
        break;
    case 18:
        if (func_8008667C(2) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 19:
        if (func_800460CC() & 1) {
            temp_v0_4 = func_8008667C(2);
            switch (temp_v0_4) {
            case 0:
            case 1:
                D_800CA148 = 0x53;
                break;
            case 2:
                D_800CA148 = 0x55;
                break;
            case 3:
                D_800CA148 = 0x57;
                break;
            case 4:
                D_800CA148 = 0x59;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 7)) - 0x299;
            func_80135448();
        }
        break;
    case 20:
        normal_date_speak();
        break;
    case 21:
        if (func_8008667C(2) == 4) {
            func_80042940(0x17);
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80133990(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            D_800CA148 = 0x2A;
            addr_init_bustup();
            D_800E6280.unk_F5F = 2;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x68;
                break;
            case 2:
                D_800CA148 = 0x69;
                break;
            case 3:
                D_800CA148 = 0x6A;
                break;
            case 4:
                D_800CA148 = 0x6C;
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
        if (func_8008667C(2) < 3) {
            func_8004284C();
        } else {
            normal_date_speak();
        }
        break;
    case 12:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 13:
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(2) >= 3) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (func_800460CC() & 1) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = (D_80122EA8 * 2) + 0x6E;
                break;
            case 2:
                D_800CA148 = (D_80122EA8 * 2) + 0x74;
                break;
            case 3:
            case 4:
                D_800CA148 = D_80122EA8 + 0x7A;
                break;
            }
            func_80135448();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if ((func_8008667C(2) < 2) && (D_80122EA8 != 1)) {
            func_80044750(0x508);
        }
        func_8004284C();
        break;
    case 18:
        if (func_8008667C(2) < 3) {
            normal_date_speak();
        } else {
            func_80042940(0x17);
        }
        break;
    case 19:
        if (func_8008667C(2) < 2) {
            D_800CA148 = 0x7D;
        } else {
            D_800CA148 = 0x7F;
        }
        func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80133E18(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
        func_800866A0(0x522E);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 2;
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
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x81;
                break;
            case 2:
                D_800CA148 = 0x82;
                break;
            case 3:
                D_800CA148 = 0x83;
                break;
            case 4:
                D_800CA148 = 0x84;
                break;
            }
            func_80135448();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (func_8008667C(2) == 4) {
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
        if (func_8008667C(2) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 15:
        ((BunkaReturnFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkaReturnFlags *)&D_800E7D4F)->unk_2_4 = 0;
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
        func_800847B8(2U);
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
        func_800866A0(0x522E);
        break;
    case 21:
        addr_init_bustup();
        D_800E6280.unk_F5F = 2;
        func_80083474();
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 22:
        if (func_8008667C(2) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 23:
        if (func_800460CC() & 1) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x86;
                break;
            case 2:
                D_800CA148 = 0x88;
                break;
            case 3:
                D_800CA148 = 0x8A;
                break;
            case 4:
                D_800CA148 = 0x8C;
                break;
            }
            func_80135448();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (func_8008667C(2) == 4) {
            func_80042940(0x1B);
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        ((BunkaReturnFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801343B4(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            D_800CA148 = 0x2A;
            addr_init_bustup();
            D_800E6280.unk_F5F = 2;
            func_80083474();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x8E;
                break;
            case 2:
                D_800CA148 = 0x8F;
                break;
            case 3:
                D_800CA148 = 0x90;
                break;
            case 4:
                D_800CA148 = 0x91;
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042878(0x80);
        break;
    case 12:
        func_800866A0(0x522E);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 2;
        func_80083474();
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 14:
        if (func_8008667C(2) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 15:
        if (func_800460CC() & 1) {
            temp_v0_2 = func_8008667C(2);
            switch (temp_v0_2) {
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
                D_800CA148 = 0x97;
                break;
            }
            func_80135448();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        if (func_8008667C(2) == 4) {
            func_80042940(0x13);
        } else {
            if (func_8008667C(2) == 3) {
                D_800CA148 = 0x95;
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

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80132480", func_801347A8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80132480", func_80134A90);

void func_80134CF8(void) {
    u32 temp_v0;

    GameState *g = &D_800E6280;

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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x522E);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (func_800460CC() & 1) {
            addr_init_bustup();
            g->unk_F5F = 2;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (func_800460CC() & 1) {
            temp_v0 = func_8008667C(2);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xAE;
                break;
            case 2:
                D_800CA148 = 0xB0;
                break;
            case 3:
                D_800CA148 = 0xB2;
                break;
            case 4:
                D_800CA148 = 0xBB;
                break;
            }
            func_80135448();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(2) == 3) {
            func_80042940(0x11);
        } else {
            func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80142BC8();
        break;
    case 17:
        if (!(g->unk_1BC[2].unk_0A & 3)) {
            D_800CA148 = 0xB9;
            func_80042940(0x14);
        } else {
            if (!(g->unk_1BC[2].unk_0C.f.b2)) {
                g->unk_1BC[2].unk_0C.f.b2 = 1;
                if (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                    D_800CA148 = 0xB5;
                } else {
                    D_800CA148 = 0xB6;
                }
            } else if (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) {
                D_800CA148 = 0xB3;
            } else {
                D_800CA148 = 0xB4;
            }
            func_8004284C();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        D_800CA148 = 0xB7;
        func_8004284C();
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
        func_80042940(0xD);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801350A8(void) {
    s32 temp_v0;

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
        func_80042878(0x80);
        break;
    case 8:
        func_800866A0(0x522E);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        g->unk_F5F = 2;
        func_80083474();
        func_80046318(0x1AU, 0x801E8000, 0x4A12);
        break;
    case 10:
        if (func_8008667C(2) == 4) {
            D_800CA148 = 0xC3;
            normal_date_bg_fadein();
        } else {
            func_8004284C();
        }
        break;
    case 11:
        if (func_800460CC() & 1) {
            func_80135448();
        }
        break;
    case 12:
        if (func_8008667C(2) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 13:
        if (func_8008667C(2) == 4) {
            normal_date_girl_in();
        } else {
            func_8007E884();
        }
        break;
    case 14:
        temp_v0 = func_8008667C(2);
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0:
        case 1:
            D_800CA148 = 0xBD;
            break;
        case 2:
            D_800CA148 = 0xBF;
            break;
        case 3:
            D_800CA148 = 0xC1;
            break;
        }
        if (func_8008667C(2) < 4) {
            if (g->unk_03E == 0x60) {
                D_800CA148 += 9;
            }
            if (g->unk_03E == 0x61) {
                D_800CA148 += 0xF;
            }
        }
        func_8004284C();
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

void func_80135448(void) {
    func_800847B8(2);
    func_80132104();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
