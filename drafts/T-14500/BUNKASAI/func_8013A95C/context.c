#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80138A70(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 8;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x87);
            return;
        }
        func_80042908(0x86);
        return;
    case 1:
        func_80042908(0x88);
        return;
    case 2:
        func_80042908(0x89);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x8A);
            return;
        case 0x60:
            func_80042908(0x8C);
            return;
        case 0x61:
            func_80042908(0x89);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x8D);
            return;
        case 0x60:
            func_80042908(0x8E);
            return;
        case 0x61:
            func_80042908(0x8F);
            return;
        }
        break;
    case 5:
        func_80042908(0x90);
        return;
    default:
        func_80046500();
        return;
    }
}

/* T-12220: bit-field stores preserve the original byte-load register allocation. */
void func_80138BE4(void) {
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
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x5012);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 8;
            func_80083474();
            if (D_800E6280.unk_03E == 0x5F) {
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
        func_8004284C();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
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
                D_800CA148 = 4;
                break;
            }
            if (D_800E6280.unk_03E == 0x60) {
                D_800CA148 += 5;
            }
            func_8013B120();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        if (func_8008667C(8) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
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
        if (func_8008667C(8) != 4) {
            if (D_800E6280.unk_03E == 0x5F) {
                D_800E6280.unk_1BC[8].unk_0C.f.b12 = 1;
            } else {
                D_800E6280.unk_1BC[8].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80138EF8(void) {
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
        func_800866A0(0x5012);
        break;
    case 9:
        addr_init_bustup();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        func_801431D4();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
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
            case 4:
                D_800CA148 = 0xE;
                break;
            }
            func_8013B120();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        if (func_8008667C(8) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801391B8(void) {
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
        if (D_800E6280.unk_03E == 0x5F) {
            D_800CA148 = 0x27;
        } else {
            D_800CA148 = 0x28;
        }
        normal_date_bg_fadein();
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
            func_800866A0(0x5012);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        addr_init_bustup();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        func_8014338C();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_8013B120();
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10D;
                break;
            case 2:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10C;
                break;
            case 3:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0x10B;
                break;
            case 4:
                D_800CA148 = 0x19;
                break;
            }
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_girl_out();
        break;
    case 14:
        if (func_8008667C(8) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80139504(void) {
    u32 temp_v0;
    u32 temp_v0_2;

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
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5012);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x29;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
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
            func_8013B120();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_bggirl_fadeout();
        break;
    case 12:
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 13:
        func_800866A0(0x5012);
        break;
    case 14:
        addr_init_bustup();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        func_801431D4();
        D_80125CA0 = 0xC0;
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 15:
        if (func_8008667C(8) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(8);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0xFE;
                break;
            case 2:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0xFD;
                break;
            case 3:
                D_800CA148 = (D_800E6280.unk_03E * 3) - 0xFC;
                break;
            case 4:
                D_800CA148 = 0x28;
                break;
            }
            func_8013B120();
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        if (func_8008667C(8) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
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

void func_80139920(void) {
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
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5012);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 8;
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
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x29;
                break;
            case 2:
                D_800CA148 = 0x2A;
                break;
            case 3:
                D_800CA148 = 0x2B;
                break;
            case 4:
                D_800CA148 = 0x2C;
                break;
            }
            func_8013B120();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        normal_date_girl_out();
        break;
    case 12:
        if (func_8008667C(8) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 13:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 14:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 15:
        if (func_8008667C(8) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            func_8013B120();
            temp_v0_2 = func_8008667C(8);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x2E;
                break;
            case 2:
                D_800CA148 = 0x31;
                break;
            case 3:
                D_800CA148 = 0x34;
                break;
            case 4:
                D_800CA148 = 0x37;
                break;
            }
            D_800CA148 += D_80122EA8;
        }
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_8004284C();
        temp_v0_3 = func_8008667C(8);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x3A;
            break;
        case 2:
            D_800CA148 = 0x3B;
            break;
        case 3:
            D_800CA148 = 0x3C;
            break;
        case 4:
            func_80042940(0x15);
            break;
        }
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        if (func_8008667C(8) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
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

void func_80139D84(void) {
    s32 temp_v0_2;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v0_4;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8005742C(0x4105, 1);
        func_8004284C();
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5012);
        break;
    case 4:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2A;
        break;
    case 5:
        func_8007E81C();
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        func_8007E390();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3D;
                break;
            case 2:
                D_800CA148 = 0x3E;
                break;
            case 3:
                D_800CA148 = 0x3F;
                break;
            case 4:
                D_800CA148 = 0x40;
                break;
            }
            func_8013B120();
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        if (func_8008667C(8) == 4) {
            func_8007E5A0();
        } else {
            func_80042940(0xE);
        }
        break;
    case 12:
        func_80085A60();
        break;
    case 13:
        func_80042940(0x19);
        break;
    case 14:
        func_8007E99C();
        break;
    case 15:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 4;
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
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 18:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 20:
        func_800866A0(0x5012);
        break;
    case 21:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 22:
        func_8007E884();
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(8);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x42;
                break;
            case 2:
                D_800CA148 = 0x43;
                break;
            case 3:
                D_800CA148 = 0x44;
                break;
            }
            func_8013B120();
        }
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        if (func_8008667C(8) == 4) {
            func_8004284C();
        } else {
            func_8007E5A0();
        }
        break;
    case 26:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b1 = 1;
        ((BunkasaiModeFlags *)&D_800E7D4F)->mode = 0;
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
        func_80064E84();
        func_800649D4();
        func_80084E4C();
        func_800847B8(8U);
        D_80122E98 = 0;
        func_8004284C();
        break;
    case 29:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_800573AC();
            func_80057390(0);
            func_8005742C(0x4105, 1);
            func_8004284C();
        }
        break;
    case 30:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 31:
        func_800866A0(0x5012);
        break;
    case 32:
        func_8007C740();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        break;
    case 33:
        if (func_8008667C(8) == 4) {
            func_8007E81C();
        } else {
            func_8007E884();
        }
        break;
    case 34:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = func_8008667C(8);
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = 0x45;
                break;
            case 2:
                D_800CA148 = 0x47;
                break;
            case 3:
                D_800CA148 = 0x49;
                break;
            case 4:
                D_800CA148 = 0x4B;
                break;
            }
            if ((u8) D_800E7D4E < 4U) {
                D_800CA148 += 1;
            }
            func_8013B120();
        }
        break;
    case 35:
        func_80085A60();
        break;
    case 36:
        func_8004284C();
        temp_v0_4 = func_8008667C(8);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 0x3A;
            break;
        case 2:
            D_800CA148 = 0x3B;
            break;
        case 3:
            D_800CA148 = 0x3C;
            break;
        case 4:
            func_80042940(0x27);
            break;
        }
        break;
    case 37:
        func_80085A60();
        break;
    case 38:
        func_8007E5A0();
        break;
    case 39:
        func_8007E934();
        break;
    case 40:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 41:
        func_80085A60();
        break;
    case 42:
        ((BunkasaiModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013A614(void) {
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
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 3:
        func_800866A0(0x5012);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 8;
        func_80083474();
        D_800CA148 = 0x2B;
        func_801431D4();
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x56;
                break;
            case 2:
                D_800CA148 = 0x59;
                break;
            case 3:
                D_800CA148 = 0x5C;
                break;
            case 4:
                D_800CA148 = 0x5F;
                break;
            }
            func_8013B120();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (func_8008667C(8) == 4) {
            func_80042940(0xF);
        } else {
            if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x64U) {
                D_800CA148 += 1;
            }
            func_8004284C();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(8);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x3A;
            break;
        case 2:
            D_800CA148 = 0x3B;
            break;
        case 3:
            D_800CA148 = 0x3C;
            break;
        }
        func_8004284C();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if (func_8008667C(8) == 4) {
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

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E71DF;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_8013A95C(void) {
    u32 temp_v0;

    switch (D_800E738A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
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
        func_800866A0(0x5012);
        break;
    case 4:
        addr_init_bustup();
        D_800E71DF = 8;
        func_80083474();
        func_801431D4();
        D_800CA148 = 0x2C;
        break;
    case 5:
        normal_date_bg_fadein();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        cd_read_sectors(0x14U, 0x801E8000, 0x4991);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(8);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x61;
                break;
            case 2:
                D_800CA148 = 0x63;
                break;
            case 3:
                D_800CA148 = 0x65;
                break;
            case 4:
                D_800CA148 = 0x67;
                break;
            }
            func_8013B120();
        }
        break;
    case 10:
        normal_date_speak();
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
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_8013ABA0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80138A70", func_8013AE08);

void func_8013B120(void) {
    func_800847B8(8);
    func_8013230C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
