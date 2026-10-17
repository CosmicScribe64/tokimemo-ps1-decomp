#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80148040(void) {
    BunkaDispatch8 *dispatch = (BunkaDispatch8 *)&D_80122EB8;
    dispatch->unk_04 = 0;
    switch (dispatch->unk_00) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x11);
            return;
        }
        func_80042908(0x10);
        return;
    case 1:
        func_80042908(0x12);
        return;
    case 2:
        func_80042908(0x13);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x14);
            return;
        case 0x60:
            func_80042908(0x16);
            return;
        case 0x61:
            func_80042908(0x17);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x18);
            return;
        case 0x60:
            func_80042908(0x19);
            return;
        case 0x61:
            func_80042908(0x1A);
            return;
        }
        break;
    case 5:
        func_80042908(0x1B);
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801481B0(void) {
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
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            if (D_800E6280.unk_03E == 0x5F) {
                D_800CA148 = 0x24;
            } else {
                D_800CA148 = 0x25;
            }
        }
        break;
    case 5:
    case 9:
    case 11:
    case 16:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0xA;
                break;
            case 2:
                D_800CA148 = 0xB;
                break;
            case 3:
                D_800CA148 = 0xC;
                break;
            case 4:
                D_800CA148 = 0xD;
                break;
            }
            if (D_800E6280.unk_03E == 0x60) {
                D_800CA148 += 4;
            }
            func_8014AB54();
        }
        break;
    case 10:
        if (func_8008667C(0) == 4) {
            func_80042940(0xC);
        } else {
            temp_v0_2 = func_8008667C(0);
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
    case 12:
        D_800CA148 = 0x90;
        normal_date_girl_out();
        break;
    case 13:
        if (func_8008667C(0) == 4) {
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
    case 17:
        if (func_8008667C(0) < 4) {
            if (D_800E6280.unk_03E == 0x5F) {
                D_800E6280.unk_1BC[0].unk_0C.f.b12 = 1;
            } else {
                D_800E6280.unk_1BC[0].unk_0C.f.b13 = 1;
            }
        }
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80148524(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
    case 6:
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
    case 12:
    case 14:
    case 18:
        normal_date_speak();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        main_state_set(0x80);
        break;
    case 8:
        func_800866A0(0x544A);
        break;
    case 9:
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
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
            case 4:
                D_800CA148 = 0x15;
                break;
            }
            func_8014AB54();
        }
        break;
    case 13:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
            D_800CA148 = 4;
            break;
        case 1:
            D_800CA148 = 5;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            D_800CA148 = 7;
            break;
        }
        func_8004284C();
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        normal_date_bg_fadeout();
        break;
    case 17:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80148800(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40F4, 1);
        func_8004284C();
        break;
    case 1:
    case 5:
    case 17:
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
        if (D_800E6280.unk_03E == 0x5F) {
            D_800CA148 = 0x27;
        } else {
            D_800CA148 = 0x28;
        }
        break;
    case 4:
    case 12:
    case 14:
    case 19:
        normal_date_speak();
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
            func_800866A0(0x544A);
        }
        break;
    case 9:
        dec_bg_show_set(1, func_8005751C(1));
        func_8014338C();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x16;
                break;
            case 2:
                D_800CA148 = 0x17;
                break;
            case 3:
                D_800CA148 = 0x18;
                break;
            case 4:
                D_800CA148 = 0x19;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 4)) - 0x17C;
            func_8014AB54();
        }
        break;
    case 13:
        func_8004284C();
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
            D_800CA148 = 4;
            break;
        case 1:
            D_800CA148 = 5;
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            D_800CA148 = 7;
            func_80042940(0xF);
            break;
        }
        break;
    case 15:
        normal_date_girl_out();
        break;
    case 16:
        if (func_8008667C(0) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80148B84(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        func_8004284C();
        break;
    case 1:
    case 20:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            D_800CA148 = 0x29;
        }
        break;
    case 5:
    case 9:
    case 16:
    case 18:
    case 22:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x22;
                break;
            case 2:
                D_800CA148 = 0x23;
                break;
            case 3:
                D_800CA148 = 0x24;
                break;
            case 4:
                D_800CA148 = 0x25;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 8)) - 0x2F8;
            func_8014AB54();
        }
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
        func_800866A0(0x544A);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 14:
        if (func_8008667C(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x26;
                break;
            case 2:
                D_800CA148 = 0x27;
                break;
            case 3:
                D_800CA148 = 0x28;
                break;
            case 4:
                D_800CA148 = 0x29;
                break;
            }
            D_800CA148 = (D_800CA148 + (D_800E6280.unk_03E * 8)) - 0x2F8;
            func_8014AB54();
        }
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 0x76;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            func_80042940(0x14);
            break;
        }
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80148F98(void) {
    s32 temp_v0_4;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        func_8004284C();
        break;
    case 1:
    case 22:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
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
            D_800E6280.unk_F5F = 0;
            func_80083474();
        }
        break;
    case 5:
    case 9:
    case 16:
    case 18:
    case 20:
    case 24:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x3A;
                break;
            case 2:
                D_800CA148 = 0x3F;
                break;
            case 3:
                D_800CA148 = 0x44;
                break;
            case 4:
                D_800CA148 = 0x49;
                break;
            }
            func_8014AB54();
        }
        break;
    case 10:
    case 21:
        normal_date_girl_out();
        break;
    case 11:
        if (func_8008667C(0) == 4) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 12:
        func_80044750(0x202);
        D_80122EB0 = (s32) D_800E6280.unk_1109;
        D_80122EB4 = D_800E6280.unk_110A + 1;
        func_80042808();
        break;
    case 13:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 14:
        if (func_8008667C(0) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x3B;
                break;
            case 2:
                D_800CA148 = 0x40;
                break;
            case 3:
                D_800CA148 = 0x45;
                break;
            case 4:
                D_800CA148 = 0x4B;
                break;
            }
            D_800CA148 += D_80122EA8;
            func_8014AB54();
        }
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x3E;
            break;
        case 2:
            D_800CA148 = 0x43;
            break;
        case 3:
            if (D_80122EA8 == 2) {
                D_800CA148 = 0x48;
            } else {
                func_80042940(0x13);
            }
            break;
        case 4:
            func_80042940(0x16);
            break;
        }
        break;
    case 19:
        temp_v0_4 = func_8008667C(0);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 0x76;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 23:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 25:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80149404(void) {
    s32 temp_v0_4;
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
        func_800866A0(0x544A);
        break;
    case 4:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
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
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x4E;
                break;
            case 2:
                D_800CA148 = 0x4F;
                break;
            case 3:
                D_800CA148 = 0x50;
                break;
            case 4:
                D_800CA148 = 0x51;
                break;
            }
            func_8014AB54();
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (func_8008667C(0) == 4) {
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
        if (func_8008667C(0) == 4) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 15:
        ((Flags7D4F *)&D_800E7D4F)->b1 = 1;
        ((Flags7D4F *)&D_800E7D4F)->unk_2_4 = 0;
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
        func_800847B8(0U);
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
        func_800866A0(0x544A);
        break;
    case 21:
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 22:
        if (func_8008667C(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 23:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
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
            if ((u8) D_800E7D4E < 2U) {
                D_800CA148 += 1;
            }
            func_8014AB54();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        func_8004284C();
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 0x5B;
            break;
        case 2:
            D_800CA148 = 0x5C;
            break;
        case 3:
            D_800CA148 = 0x5D;
            break;
        case 4:
            func_80042940(0x1E);
            break;
        }
        break;
    case 26:
        normal_date_speak();
        break;
    case 27:
        temp_v0_4 = func_8008667C(0);
        switch (temp_v0_4) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 0x76;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 28:
        normal_date_speak();
        break;
    case 29:
        normal_date_girl_out();
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
        ((Flags7D4F *)&D_800E7D4F)->b0 = 1;
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80149A88(void) {
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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
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
            D_800E6280.unk_F5F = 0;
            func_80083474();
        }
        break;
    case 5:
    case 9:
    case 16:
    case 18:
    case 22:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x5E;
                break;
            case 2:
                D_800CA148 = 0x5F;
                break;
            case 3:
                D_800CA148 = 0x60;
                break;
            case 4:
                D_800CA148 = 0x61;
                break;
            }
            func_8014AB54();
        }
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
        func_800866A0(0x544A);
        break;
    case 13:
        func_801431D4();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        D_800E6280.unk_F5F = 0;
        func_80083474();
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        break;
    case 14:
        if (func_8008667C(0) == 4) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = func_8008667C(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                D_800CA148 = 0x62;
                break;
            case 2:
                D_800CA148 = 0x63;
                break;
            case 3:
                D_800CA148 = 0x64;
                break;
            case 4:
                D_800CA148 = 0x65;
                break;
            }
            func_8014AB54();
        }
        break;
    case 17:
        func_8004284C();
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 0x76;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            func_80042940(0x14);
            break;
        }
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        normal_date_bg_fadeout();
        break;
    case 21:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 23:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_80149E68(void) {
    s32 temp_v0_2;
    s32 temp_v0_3;
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
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            D_800CA148 = 0x2B;
        }
        break;
    case 5:
    case 9:
    case 11:
    case 13:
    case 15:
    case 19:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x66;
                break;
            case 2:
                D_800CA148 = 0x6A;
                break;
            case 3:
                D_800CA148 = 0x6E;
                break;
            case 4:
                D_800CA148 = 0x72;
                break;
            }
            func_8014AB54();
        }
        break;
    case 10:
        if (func_8008667C(0) == 4) {
            func_80042940(0x10);
        } else {
            if ((u16) D_800E6280.unk_66C[4].unk_00 < 0x64U) {
                D_800CA148 += 1;
            }
            func_8004284C();
        }
        break;
    case 12:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x69;
            break;
        case 2:
            D_800CA148 = 0x6D;
            break;
        case 3:
            D_800CA148 = 0x71;
            break;
        }
        func_8004284C();
        break;
    case 14:
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 0x76;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        }
        func_8004284C();
        break;
    case 16:
        normal_date_girl_out();
        break;
    case 17:
        normal_date_bg_fadeout();
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 20:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014A1AC(void) {
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        func_8004284C();
        break;
    case 1:
    case 15:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            D_800CA148 = 0x2C;
        }
        break;
    case 5:
    case 9:
    case 11:
    case 14:
    case 17:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x73;
                break;
            case 2:
                D_800CA148 = 0x74;
                break;
            case 3:
                D_800CA148 = 0x79;
                break;
            case 4:
                D_800CA148 = 0x7A;
                break;
            }
            func_8014AB54();
        }
        break;
    case 10:
        func_8004284C();
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 1;
            break;
        case 2:
            D_800CA148 = 0x75;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            D_800CA148 = 0x7B;
            func_80042940(0xC);
            break;
        }
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        temp_v0_3 = func_8008667C(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            func_80042940(0xF);
            break;
        case 2:
        case 3:
            D_800CA148 = 0x78;
            func_8004284C();
            break;
        case 4:
            func_8004284C();
            break;
        }
        break;
    case 16:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 18:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014A49C(void) {
    u32 temp_v0;
    u32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
    case 13:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_801431D4();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x544A);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 0;
            func_80083474();
            D_800CA148 = 0x2D;
        }
        break;
    case 5:
    case 9:
    case 15:
        normal_date_speak();
        break;
    case 6:
        cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        func_8004284C();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x7C;
                break;
            case 2:
                D_800CA148 = 0x7D;
                break;
            case 3:
                D_800CA148 = 0x7E;
                break;
            case 4:
                D_800CA148 = 0x7F;
                break;
            }
            func_8014AB54();
        }
        break;
    case 10:
        temp_v0_2 = func_8008667C(0);
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
            D_800CA148 = 0x80;
            break;
        }
        func_8004284C();
        break;
    case 11:
        if (func_8008667C(0) == 4) {
            normal_date_girl_out();
        } else {
            normal_date_speak();
        }
        break;
    case 12:
        if (func_8008667C(0) == 4) {
            normal_date_speak();
        } else {
            normal_date_girl_out();
        }
        break;
    case 14:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 16:
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014A778(void) {
    GameState *g;
    u32 temp_v0;
    u32 temp_v0_2;

    g = &D_800E6280;
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
        func_800866A0(0x544A);
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            func_8014338C();
            D_80125CA0 = 0xC0;
            addr_init_bustup();
            g->unk_F5F = 0;
            func_80083474();
            cd_read_sectors(0x11U, 0x801E8000, 0x49BF);
        }
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0 = func_8008667C(0);
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
            if (g->unk_03E == 0x60) {
                D_800CA148 += 7;
            }
            if (g->unk_03E == 0x61) {
                D_800CA148 += 0xB;
            }
            func_8014AB54();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        temp_v0_2 = func_8008667C(0);
        switch (temp_v0_2) {
        case 0:
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 0x85;
            } else {
                D_800CA148 = 4;
            }
            break;
        case 1:
            if (g->unk_03E == 0x5F) {
                D_800CA148 = 0x86;
            } else {
                D_800CA148 = 5;
            }
            break;
        case 2:
            D_800CA148 = 2;
            break;
        case 3:
            D_800CA148 = 3;
            break;
        case 4:
            D_800CA148 = 7;
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
        func_80142BC8();
        break;
    default:
        func_80046500();
        break;
    }
}


void func_8014AB54(void) {
    func_800847B8(0);
    func_80132000();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
