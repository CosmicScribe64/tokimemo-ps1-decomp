#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80146560(void) {
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x56);
            return;
        case 0x60:
            func_80042908(0x57);
            return;
        case 0x61:
            func_80042908(0x58);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x59);
        return;
    case 2:
        func_80042908(0x5A);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x5B);
            return;
        case 0x60:
            func_80042908(0x5C);
            return;
        case 0x61:
            func_80042908(0x5D);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x5E);
            return;
        case 0x60:
            func_80042908(0x5F);
            return;
        case 0x61:
            func_80042908(0x60);
            return;
        }
        break;
    case 5:
        func_80042908(0x61);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80146700(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 2;
                break;
            case 2:
                D_800CA148 = 3;
                break;
            case 3:
                D_800CA148 = 4;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 5;
            break;
        case 2:
            D_800CA148 = 9;
            break;
        case 3:
            D_800CA148 = 0xA;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(6) < 2) {
            if (D_800E6280.unk_100.unk_02 < 0x78) {
                D_800CA148 += 1;
            }
            func_8004284C();
        } else {
            func_80042940(0xE);
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        D_800CA148 = 8;
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801469DC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xB;
                break;
            case 2:
                D_800CA148 = 0xC;
                break;
            case 3:
                D_800CA148 = 0xD;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0xE;
            break;
        case 2:
            D_800CA148 = 0x10;
            break;
        case 3:
            D_800CA148 = 0x11;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (func_8008667C(6) >= 2) {
            func_80042940(0xE);
        } else {
            func_8004284C();
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80146C8C(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x12;
                break;
            case 2:
                D_800CA148 = 0x13;
                break;
            case 3:
                D_800CA148 = 0x14;
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x15;
                break;
            case 2:
                D_800CA148 = 0x16;
                break;
            case 3:
                D_800CA148 = 0x17;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80146F70(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x467C, 1);
        func_8004284C();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_800866A0(0x622D);
        }
        break;
    case 10:
        func_80145844();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, func_8005751C(1));
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
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
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 3)) - 0x11D;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801472C4(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x27;
                break;
            case 2:
                D_800CA148 = 0x28;
                break;
            case 3:
                D_800CA148 = 0x29;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 3)) - 0x11D;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801475CC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x30;
                break;
            case 2:
                D_800CA148 = 0x31;
                break;
            case 3:
                D_800CA148 = 0x32;
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
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042908(0xB);
        break;
    case 8:
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        func_8004284C();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x33;
                break;
            case 2:
                D_800CA148 = 0x36;
                break;
            case 3:
                D_800CA148 = 0x39;
                break;
            }
            D_800CA148 += D_80122EA8;
            func_80148E68();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = func_8008667C(6);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x3C;
            break;
        case 2:
            D_800CA148 = 0x3D;
            break;
        case 3:
            D_800CA148 = 0x3E;
            break;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E69A2;
extern s8 D_800E71DF;
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_80147914(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3F;
                break;
            case 2:
                D_800CA148 = 0x40;
                break;
            case 3:
                D_800CA148 = 0x41;
                break;
            }
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 += 2;
        func_8004284C();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        D_800E7D4F = ((D_800E7D4F | 2) & 0xFFE3) | 8;
        main_state_set(0xD0);
        break;
    case 10:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        func_80042908(0x8C);
        break;
    case 11:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 12:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 14:
        func_800866A0(0x622D);
        break;
    case 15:
        addr_init_bustup();
        D_800E71DF = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 16:
        normal_date_bggirl_fadein();
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x48;
                break;
            case 2:
                D_800CA148 = 0x49;
                break;
            case 3:
                D_800CA148 = 0x4A;
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
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        D_800E7D4F = (D_800E7D4F | 2) & 0xFFE3;
        func_80042908(0x8D);
        break;
    case 21:
        D_80122E98 = 1;
        D_80122EB0 = (s32) D_800E7389;
        D_80122EB4 = D_800E738A + 1;
        func_80042908(0x8C);
        break;
    case 22:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        func_80084E4C();
        func_800847B8(6U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 23:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            func_8004284C();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 25:
        func_800866A0(0x622D);
        break;
    case 26:
        addr_init_bustup();
        D_800E71DF = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 27:
        normal_date_bggirl_fadein();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x4B;
                break;
            case 2:
                D_800CA148 = 0x4D;
                break;
            case 3:
                D_800CA148 = 0x4F;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            func_80148E68();
        }
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        temp_v0_4 = func_8008667C(6);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x51;
            break;
        case 2:
            D_800CA148 = 0x52;
            break;
        case 3:
            D_800CA148 = 0x53;
            break;
        }
        func_8004284C();
        break;
    case 31:
        normal_date_speak();
        break;
    case 32:
        normal_date_bggirl_fadeout();
        break;
    case 33:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        D_800E7D4F |= 1;
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80148094(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x54;
                break;
            case 2:
                D_800CA148 = 0x55;
                break;
            case 3:
                D_800CA148 = 0x56;
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        func_80083440(3U);
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bg_fadein();
        break;
    case 11:
        D_800CA148 = 0x47;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_in();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x57;
                break;
            case 2:
                D_800CA148 = 0x58;
                break;
            case 3:
                D_800CA148 = 0x59;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801483A8(void) {
    s32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x5A;
                break;
            case 2:
                D_800CA148 = 0x5D;
                break;
            case 3:
                D_800CA148 = 0x60;
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
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80148594(void) {
    s32 temp_v0;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x63;
                break;
            case 2:
                D_800CA148 = 0x67;
                break;
            case 3:
                D_800CA148 = 0x69;
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
        if (func_8008667C(6) >= 2) {
            func_80042940(0xB);
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
        D_800E6280.unk_F5F = 0xE;
        func_8004284C();
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
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801487DC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
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
            func_80148E68();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        D_800CA148 = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = func_8008667C(6);
        switch (temp_v0_2) {
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
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80148A28(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(6);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x71;
                break;
            case 2:
                D_800CA148 = 0x72;
                break;
            case 3:
                D_800CA148 = 0x73;
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
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x622D);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 6;
        func_80083474();
        cd_read_sectors(0x10U, 0x801E8000, 0x4B23);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(6);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x74;
                break;
            case 2:
                D_800CA148 = 0x75;
                break;
            case 3:
                D_800CA148 = 0x76;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 3)) - 0x11D;
            func_80148E68();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (D_800E6280.unk_03E == 0x61) {
            temp_v0_3 = func_8008667C(6);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x7D;
                break;
            case 2:
                D_800CA148 = 0x7F;
                break;
            case 3:
                D_800CA148 = 0x81;
                break;
            }
            if (D_800E6280.unk_108.unk_02 < 0x78) {
                D_800CA148 += 1;
            }
            func_8004284C();
        } else {
            func_80042940(0x11);
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_4 = func_8008667C(6);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x83;
            break;
        case 2:
            D_800CA148 = 0x84;
            break;
        case 3:
            D_800CA148 = 0x85;
            break;
        }
        func_8004284C();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
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

void func_80148E68(void) {
    func_800847B8(6);
    func_8013230C();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
