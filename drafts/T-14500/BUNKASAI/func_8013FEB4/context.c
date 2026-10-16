#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013FD90(void) {
    /* FAKE: D_80122EBC reached through D_80122EB8 keeps the selector load behind this store (as1 does not
     * move a load over a store through the same symbol); the real source has one object. T-9140 */
    (&D_80122EB8)[1] = 0xA;
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x9B);
            return;
        }
        func_80042908(0x9C);
        return;
    case 1:
        func_80042908(0x9D);
        return;
    case 2:
        func_80042908(0x9E);
        return;
    case 3:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x9F);
            return;
        }
        func_80042908(0x9E);
        return;
    case 4:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0xA0);
            return;
        }
        func_80042908(0xA1);
        return;
    case 5:
        func_80042908(0xA2);
        return;
    default:
        func_80046500();
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E71DF;
extern u8 D_800E6679;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_8013FEB4(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (D_800E738A) {
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
            func_80062CD0(0x6935);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E71DF = 0xA;
            func_80083474();
            D_800CA148 = 0x25;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 7;
                break;
            case 2:
                D_800CA148 = 8;
                break;
            case 3:
                D_800CA148 = 9;
                break;
            case 4:
                D_800CA148 = 0xA;
                break;
            }
            func_80142694();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(0xA) == 4) {
            func_80042940(0xC);
        } else {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {
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
        if (func_8008667C(0xA) == 4) {
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
        if (func_8008667C(0xA) != 4) {
            D_800E6679 |= 0x20;
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801401E8(void) {
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
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x6935);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
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
                D_800CA148 = 0xF;
                break;
            }
            func_80142694();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(0xA);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 4;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 6;
            break;
        case 4:
            func_8004284C();
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if (func_8008667C(0xA) < 2) {
            D_800CA148 = 0x75;
        }
        normal_date_girl_out();
        break;
    case 16:
        if ((func_8008667C(0xA) == 4) || (func_8008667C(0xA) < 2)) {
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

void func_80140558(void) {
    s32 temp_v0_2;
    u32 temp_v0;

    switch (D_800E6280.unk_110A) {
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
            func_800866A0(0x6935);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {
            case 0:
            case 1:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x11;
                } else {
                    D_800CA148 = 0x16;
                }
                break;
            case 2:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x12;
                } else {
                    D_800CA148 = 0x18;
                }
                break;
            case 3:
                if (D_800E6280.unk_03E == 0x60) {
                    D_800CA148 = 0x13;
                } else {
                    D_800CA148 = 0x1A;
                }
                break;
            case 4:
                D_800CA148 = 0x14;
                break;
            }
            func_80142694();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        if (D_800E6280.unk_03E == 0x60) {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
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
        if (func_8008667C(0xA) == 4) {
            func_80042940(0xF);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if (func_8008667C(0xA) == 4) {
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

void func_80140958(void) {
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
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x6935);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0xA;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x1C;
                break;
            case 2:
                D_800CA148 = 0x1D;
                break;
            case 3:
                D_800CA148 = 0x1E;
                break;
            case 4:
                D_800CA148 = 0x22;
                break;
            }
            if ((func_8008667C(0xA) < 4) && (D_800E6280.unk_03E == 0x61)) {
                D_800CA148 += 3;
            }
            func_80142694();
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
        main_state_set(0x80);
        break;
    case 12:
        func_800866A0(0x6935);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 14:
        if (func_8008667C(0xA) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {
            case 0:
            case 1:
                if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 2) {
                    D_800CA148 = 0x23;
                } else {
                    D_800CA148 = 0x40;
                }
                break;
            case 2:
                if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 2) {
                    D_800CA148 = 0x24;
                } else {
                    D_800CA148 = 0x41;
                }
                break;
            case 3:
                if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 2) {
                    D_800CA148 = 0x25;
                } else {
                    D_800CA148 = 0x42;
                }
                break;
            case 4:
                D_800CA148 = 0x29;
            }
            if ((func_8008667C(0xA) < 4) && (D_800E6280.unk_03E == 0x61) && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 2)) {
                D_800CA148 += 3;
            }
            func_80142694();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(0xA);
        switch (temp_v0_3) {
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
            func_80042940(0x15);
            break;
        }
        if ((func_8008667C(0xA) < 4) && (D_800E6280.unk_03E == 0x61)) {
            D_800CA148 += 3;
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        if ((D_800E6280.unk_03E == 0x61) && (func_8008667C(0xA) < 2)) {
            D_800CA148 = 0x75;
        }
        normal_date_girl_out();
        break;
    case 20:
        if ((func_8008667C(0xA) == 4) || ((D_800E6280.unk_03E == 0x61) && (func_8008667C(0xA) < 2))) {
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

void func_80140F4C(void) {
    s32 temp_v0_2;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
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
        func_800866A0(0x6935);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
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
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x2A;
                break;
            case 2:
                D_800CA148 = 0x2B;
                break;
            case 3:
                D_800CA148 = 0x2C;
                break;
            case 4:
                D_800CA148 = 0x3C;
                break;
            }
            func_80142694();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (func_8008667C(0xA) == 4) {
            normal_date_girl_out();
        } else {
            func_80042940(0xE);
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80042940(0x19);
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        ((BunkaReturnFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkaReturnFlags *)&D_800E7D4F)->unk_2_4 = 3;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        main_state_set(0xD0);
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
        func_800847B8(0xAU);
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
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 20:
        func_800866A0(0x6935);
        break;
    case 21:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 22:
        func_8007E884();
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0xA);
            switch (temp_v0_2) {                    /* switch 3; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x2D;
                break;
            case 2:
                D_800CA148 = 0x2E;
                break;
            case 3:
                D_800CA148 = 0x2F;
                break;
            }
            func_80142694();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (func_8008667C(0xA) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 26:
        ((BunkaReturnFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkaReturnFlags *)&D_800E7D4F)->unk_2_4 = 0;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
        func_80042908(0xA4);
        break;
    case 27:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xA3);
        break;
    case 28:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_801431D4();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(0xAU);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 29:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 30:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 31:
        func_800866A0(0x6935);
        break;
    case 32:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 33:
        if (func_8008667C(0xA) == 4) {
            normal_date_bg_fadein();
        } else {
            func_8007E884();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(0xA);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x30;
                break;
            case 2:
                D_800CA148 = 0x32;
                break;
            case 3:
                D_800CA148 = 0x34;
                break;
            case 4:
                D_800CA148 = 0x3E;
                break;
            }
            if ((u8) D_800E7D4E < 3U) {
                D_800CA148 += 1;
            }
            func_80142694();
        }
        break;
    case 35:
        normal_date_speak();
        break;
    case 36:
        func_8004284C();
        temp_v0_4 = func_8008667C(0xA);
        switch (temp_v0_4) {
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
            func_80042940(0x27);
            break;
        }
        break;
    case 37:
        normal_date_speak();
        break;
    case 38:
        normal_date_girl_out();
        break;
    case 39:
        normal_date_bg_fadeout();
        break;
    case 40:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 41:
        normal_date_speak();
        break;
    case 42:
        ((BunkaReturnFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801417DC(void) {
    s32 temp_v0;

    GameState *g = &D_800E6280;

    switch (g->unk_110A) {
    case 0x0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 0x1:
        normal_date_bg_fadeout();
        break;
    case 0x2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 0x3:
        func_800866A0(0x6C5F);
        break;
    case 0x4:
        if (func_8008667C(0xA) == 4) {
            func_801431D4();
            cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
            func_80042940(0x32);
        } else {
            func_801431D4();
            func_800AE0A0((void *)0x80197000, 0x80180000, 0x16800);
            func_80062CD0(0x6935);
            func_8004284C();
        }
        break;
    case 0x5:
        normal_date_bg_fadein();
        break;
    case 0x6:
        if (cd_read_sync_status() & 1) {
            func_800AE0A0((void *)0x801AE000, 0x80180000, 0x16800);
            addr_init_bustup();
            g->unk_F5F = 0xA;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 0x7:
        normal_date_speak();
        break;
    case 0x8:
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        func_8004284C();
        break;
    case 0x9:
        normal_date_girl_in();
        break;
    case 0xA:
        if (cd_read_sync_status() & 1) {
            func_80142694();
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {                      /* switch 2; irregular */
            case 0:
            case 1:
                D_800CA148 = 0x43;
                break;
            case 2:
                D_800CA148 = 0x51;
                break;
            case 3:
                D_800CA148 = 0x5D;
                break;
            }
        }
        break;
    case 0xB:
        normal_date_speak();
        break;
    case 0xC:
        normal_date_girl_out();
        break;
    case 0xD:
        func_800AE0A0((void *)0x80180000, 0x80197000, 0x16800);
        addr_init_bustup();
        g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
        func_80083474();
        break;
    case 0xE:
        normal_date_girl_in();
        break;
    case 0xF:
        normal_date_speak();
        break;
    case 0x10:
        normal_date_girl_out();
        break;
    case 0x11:
        func_800AE0A0((void *)0x80180000, 0x801AE000, 0x16800);
        addr_init_bustup();
        g->unk_F5F = 0xA;
        func_800847B8(g->unk_F5F);
        func_80083474();
        break;
    case 0x12:
        normal_date_girl_in();
        break;
    case 0x13:
        normal_date_speak();
        break;
    case 0x14:
        normal_date_girl_out();
        break;
    case 0x15:
        func_800AE0A0((void *)0x80180000, 0x80197000, 0x16800);
        addr_init_bustup();
        g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
        func_80083474();
        break;
    case 0x16:
        normal_date_girl_in();
        break;
    case 0x17:
        normal_date_speak();
        break;
    case 0x18:
        D_80122D20 = 1;
        g->unk_F5F = 0xA;
        func_800847B8(g->unk_F5F);
        func_8004284C();
        break;
    case 0x19:
        normal_date_speak();
        break;
    case 0x1A:
        D_80122D20 = 0;
        g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
        func_8004284C();
        break;
    case 0x1B:
        normal_date_speak();
        break;
    case 0x1C:
        D_80122D20 = 1;
        g->unk_F5F = 0xA;
        func_800847B8(g->unk_F5F);
        func_8004284C();
        break;
    case 0x1D:
        normal_date_speak();
        break;
    case 0x1E:
        D_80122D20 = 0;
        g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
        func_8004284C();
        break;
    case 0x1F:
        normal_date_speak();
        break;
    case 0x20:
        if (func_8008667C(0xA) == 3) {
            func_80042940(0x22);
        } else {
            D_80122D20 = 1;
            g->unk_F5F = 0xA;
        func_800847B8(g->unk_F5F);
            func_8004284C();
        }
        break;
    case 0x21:
        normal_date_speak();
        break;
    case 0x22:
        if (func_8008667C(0xA) == 3) {
            func_80042940(0x24);
        } else {
            D_80122D20 = 0;
            g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
            func_8004284C();
        }
        break;
    case 0x23:
        normal_date_speak();
        break;
    case 0x24:
        normal_date_girl_out();
        break;
    case 0x25:
        func_800AE0A0((void *)0x80180000, 0x801AE000, 0x16800);
        addr_init_bustup();
        g->unk_F5F = 0xA;
        func_800847B8(g->unk_F5F);
        func_80083474();
        break;
    case 0x26:
        normal_date_girl_in();
        break;
    case 0x27:
        normal_date_speak();
        break;
    case 0x28:
        if (func_8008667C(0xA) >= 2) {
            func_80042940(0x2A);
        } else {
            D_80122D20 = 1;
            g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
            func_8004284C();
        }
        break;
    case 0x29:
        normal_date_speak();
        break;
    case 0x2A:
        D_80122D20 = 0;
        g->unk_F5F = 0xA;
        func_800847B8(g->unk_F5F);
        func_8004284C();
        break;
    case 0x2B:
        if ((func_8008667C(0xA) == 3) || (func_8008667C(0xA) == 2)) {
            func_8004284C();
        } else {
            normal_date_speak();
        }
        break;
    case 0x2C:
        normal_date_girl_out();
        break;
    case 0x2D:
        normal_date_speak();
        break;
    case 0x2E:
        normal_date_bg_fadeout();
        break;
    case 0x2F:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 0x30:
        normal_date_speak();
        break;
    case 0x31:
        func_80142BC8();
        break;
    case 0x32:
        normal_date_bg_fadein();
        break;
    case 0x33:
        if (cd_read_sync_status() & 1) {
            func_80142694();
            D_800CA148 = 0x67;
            addr_init_bustup();
            g->unk_F5F = 0xD;
        func_800847B8(g->unk_F5F);
            func_80083474();
        }
        break;
    case 0x34:
    case 0x35:
        normal_date_girl_in();
        break;
    case 0x36:
        normal_date_speak();
        break;
    case 0x37:
        normal_date_girl_out();
        break;
    case 0x38:
        func_800847B8(0xAU);
        func_80042940(0x2D);
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_80141F70);

void func_80142298(void) {
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
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x6935);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0xA;
        func_80083474();
        cd_read_sectors(0x13U, 0x801E8000, 0x4B5E);
        break;
    case 10:
        func_8007E884();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0xA);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x6C;
                break;
            case 2:
                D_800CA148 = 0x6D;
                break;
            case 3:
                D_800CA148 = 0x6E;
                break;
            case 4:
                D_800CA148 = 0x72;
                break;
            }
            if ((func_8008667C(0xA) < 4) && (D_800E6280.unk_03E == 0x61)) {
                D_800CA148 += 3;
            }
            func_80142694();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_8004284C();
        temp_v0_2 = func_8008667C(0xA);
        switch (temp_v0_2) {
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
            func_80042940(0xF);
            break;
        }
        if ((func_8008667C(0xA) < 4) && (D_800E6280.unk_03E == 0x61)) {
            D_800CA148 += 3;
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        if ((D_800E6280.unk_03E == 0x61) && (func_8008667C(0xA) < 2)) {
            D_800CA148 = 0x75;
        }
        normal_date_girl_out();
        break;
    case 16:
        if ((func_8008667C(0xA) == 4) || ((D_800E6280.unk_03E == 0x61) && (func_8008667C(0xA) < 2))) {
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

void func_80142694(void) {
    func_800847B8(10);
    func_80132374();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
